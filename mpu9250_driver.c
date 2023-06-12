#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/uaccess.h>

#define NOD_MAJOR 210
#define NOD_NAME "mpu9250"

#define MPU9250_ADDRESS 0x68
#define MAG_ADDRESS 0x0C

#define PWR_MGMT_1 0x6B
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

#define CONFIG_ACC _IO(0, 3)
#define READ_ACC _IO(0, 4)
#define CONFIG_GYRO _IO(0, 5)
#define READ_GYRO _IO(0, 6)

static struct i2c_adapter *mpu9250_adap;
static struct i2c_client *mpu9250_client;

static int i2c_write_data(struct i2c_client *client, u8 reg, u8 data)
{
    int ret = i2c_smbus_write_byte_data(client, reg, data);
    if (ret < 0)
    {
        pr_err("Failed to write data to I2C device\n");
        return ret;
    }
    return 0;
}

static u8 i2c_read_data(struct i2c_client *client, u8 reg)
{
    int ret = i2c_smbus_read_byte_data(client, reg);
    if (ret < 0)
    {
        pr_err("Failed to read data from I2C device\n");
        return ret;
    }
    pr_info("Read data from I2C device: 0x%X\n", ret);
    return ret;
}

static int dev_open(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "MPU 9250 Device File Opened\n");
    return 0;
}

static int dev_release(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "MPU 9250 Device File Released\n");
    return 0;
}

static long dev_ioctl(struct file *filp, unsigned cmd, unsigned long arg)
{
    // long long int buf;
    int ret;
    // struct Data data;
    switch (cmd)
    {

    case CONFIG_ACC:
    {
        char full_scale[1];
        ret = copy_from_user((void *)full_scale, (void *)(arg), 1);
        printk("acc full scale mode is %X\n", full_scale[0]);
        i2c_write_data(mpu9250_client, 0x1B, full_scale[0]);
        printk("write acc config data to I2C\n");

        break;
    }
    case READ_ACC:
    {
        char reg[6] = {ACCEL_XOUT_H, ACCEL_XOUT_L, ACCEL_YOUT_H, ACCEL_YOUT_L, ACCEL_ZOUT_H, ACCEL_ZOUT_L};
        short data[3];
        u8 buf[6];
        int i  = 0;
        for (i = 0; i < 6; i++)
        {
            buf[i] = i2c_read_data(mpu9250_client, reg[i]);
        }

        data[0] = (buf[0] << 8) | buf[1];
        data[1] = (buf[2] << 8) | buf[3];
        data[2] = (buf[4] << 8) | buf[5];
        printk("read acc data : x = %hd  y = %hd  z = %hd\n", data[0], data[1], data[2]);

        ret = copy_to_user((void *)arg, (void *)data, 3);
        printk("copied acc data to user space.\n");

        break;
    }
    case CONFIG_GYRO:
    {

        char full_scale[1];

        ret = copy_from_user((void *)full_scale, (void *)(arg), 1);
        printk("gyro full scale mode is %X\n", full_scale[0]);

        i2c_write_data(mpu9250_client, 0x1C, full_scale[0]);
        printk("write gyro config data to I2C\n");
        break;
    }
    case READ_GYRO:
    {
        char reg[6] = {GYRO_XOUT_H,GYRO_XOUT_L,GYRO_YOUT_H,GYRO_YOUT_L,GYRO_ZOUT_H,GYRO_ZOUT_L};
        short data[3];
        u8 buf[6];
        int i = 0;

        for(i = 0; i < 6; i++){
            buf[i] = i2c_read_data(mpu9250_client, reg[i]);
        }

        data[0] = (buf[0] << 8) | buf[1];
        data[1] = (buf[2] << 8) | buf[3];
        data[2] = (buf[4] << 8) | buf[5];
        printk("read gyro data : x = %hd  y = %hd  z = %hd\n", data[0], data[1], data[2]);
        
        ret = copy_to_user((void*)arg, (void*)data, 3);
        printk("copied gyro data to user space.\n");

        break;
	}

    }
	return 0;
}

static struct file_operations fops = {
  .open = dev_open,
  .release = dev_release,
  .unlocked_ioctl = dev_ioctl,

};


static int mpu9250_init(void)
{
    struct i2c_board_info board_info;
    int ret;

    // Initialize I2C adapter
    mpu9250_adap = i2c_get_adapter(0);
    if (!mpu9250_adap) {
        pr_err("Failed to get I2C adapter\n");
        return -ENODEV;
    }

    // Probe
    memset(&board_info, 0, sizeof(struct i2c_board_info));

    board_info.addr = MPU9250_ADDRESS;
    strlcpy(board_info.type, "mpu9250", I2C_NAME_SIZE);

    mpu9250_client = i2c_new_device(mpu9250_adap, &board_info);
    if (!mpu9250_client) {
        pr_err("Failed to create mpu9250 device\n");
        i2c_put_adapter(mpu9250_adap);
        return -ENODEV;
    }
    
   // Configure MPU9250
    ret = i2c_write_data(mpu9250_client, PWR_MGMT_1, 0x00);
    if (ret < 0) {
        goto error;
    }

    if (register_chrdev(NOD_MAJOR, NOD_NAME, &fops) < 0) {
		printk("INIT FAIL\n");
	}
	printk(KERN_INFO "Init Success\n");

    return 0;

    error:
        // Unregister I2C device
        i2c_unregister_device(mpu9250_client);

        // Cleanup and release I2C adapter
        i2c_put_adapter(mpu9250_adap);
        return ret;
}

static void mpu9250_exit(void)
{
        // Unregister I2C device
        i2c_unregister_device(mpu9250_client);

        // Cleanup and release I2C adapter
        i2c_put_adapter(mpu9250_adap);

        // Unregister char device
        unregister_chrdev(NOD_MAJOR, NOD_NAME);
        printk(KERN_INFO "I2C Exit Success\n");
}

    module_init(mpu9250_init);
    module_exit(mpu9250_exit);

    MODULE_LICENSE("GPL");
    MODULE_AUTHOR("mydream11123");
    MODULE_DESCRIPTION("MPU9250 Device Driver");
