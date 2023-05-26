#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define MPU9250_ADDRESS 0x68
#define MAG_ADDRESS 0x0C

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

int mpu9250_init(char *device)
{
    char buf[2];
    int fd;
    int addr = MPU9250_ADDRESS;
    int mag_addr = MAG_ADDRESS;

    if ((fd = open(device, O_RDWR)) < 0)
    {
        perror("Failed to open i2c bus");
        return fd;
    }

    // Set slave address for MPU9250
    if (ioctl(fd, I2C_SLAVE, addr) < 0)
    {
        perror("Failed to set i2c slave address for MPU9250");
        return -1;
    }
   
    // Configure MPU9250
    buf[0] = 0x6B; // PWR_MGMT_1
    buf[1] = 0x00; // Set clock source to internal 8MHz oscillator
    write(fd, buf, 2);
    return fd;
}

void acc_config(int fd, char _set_full_scale)
{
    char buf[2] = {0x1B, _set_full_scale};//{gyro config, Set full-scale range}
    write(fd, buf, 2);
}

void gyro_config(int fd, char _set_full_scale)
{
    char buf[2] = {0x1C, _set_full_scale};        //{accel config, Set full-scale range}
    write(fd, buf, 2);
}

short *read_acc_data(int fd)
{
    char buf[10];
    static short data[3];
    buf[0] = ACCEL_XOUT_H;
    write(fd, buf, 1);
    read(fd, buf, 6);

    short accel_x = (buf[0] << 8) | buf[1];
    short accel_y = (buf[2] << 8) | buf[3];
    short accel_z = (buf[4] << 8) | buf[5];

    data[0] = accel_x;
    data[1] = accel_y;
    data[2] = accel_z;
    
    return data;
}


short *read_gyro_data(int fd)
{
    char buf[10];
    static short data[3];

    buf[0] = GYRO_XOUT_H;
    write(fd, buf, 1);
    read(fd, buf, 6);
  
    short gyro_x = (buf[0] << 8) | buf[1];
    short gyro_y = (buf[2] << 8) | buf[3];
    short gyro_z = (buf[4] << 8) | buf[5];

    data[0] = gyro_x;
    data[1] = gyro_y;
    data[2] = gyro_z;
    return data;
}

void mpu9250_finalization(int fd)
{
    close(fd);
}
