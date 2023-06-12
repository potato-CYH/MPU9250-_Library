
# HC-SR04 Linux Device Driver Manual
**`( Filter not included )`**

  
| Device Type |  IMU Sensor ( MPU-9250) |
|--|--|
| Target Kernel |  Linux  |
|  Target Architecture |  ARM (Cortex-A57, aarch64)|
| Target Board |  Jetson Nano |
| License  |  GPL |
| Author | mydream11123@naver.com |


### 1. Hardware Conntection Guide

  

![image](https://github.com/potato-CYH/Algorithm_Study_Helper/assets/57744586/c0b6e59f-2d45-4141-87e1-841a4f7b11cf)


  

- Connect MPU-9250 **`SDA`**  to  **`I2C0 SDA`** on your board.

- Connect MPU-9250 **`SCL`** to **`I2C0 SCL / CLK`** on your board.

  

### 2. Device Driver Installation Guide

  

- 2-1. Clone this repository into your target board ( **Network must be conntected** )

- 2-2. Place folder ( cloned at “*section 2.1”* ) in your project folder.

- 2-5. Open Terminal In the cloned folder.  

- 2-6. Enter command below in order.

```c
$ sudo mknod /dev/mpu9250 c 210  0
$ sudo chmod 666 /dev/mpu9250
$ make all
$ sudo insmod ./mpu9250_driver.ko
```

  

### 3. Programming Guide with Device Driver
  
- You can use example app at **`./app`** and **`app.c`**

- You must call **`mpu9250_init()`** before using config or read function.

- When you want to finish using driver, must call **`finallization()`**

  

- Example

```c
int fd = mpu9250_init();
 
if(fd < 0){
    printf("Error %d\n", fd);
}
else{    
    
    config_acc(fd, 0x00);
    config_gyro(fd, 0x00);

    while(1){

        short *acc_data = read_acc_data(fd);
        short *gyro_data = read_gyro_data(fd);

        printf("acc_x : %d , acc_y : %d , acc_z : %d\n", acc_data[0], acc_data[1], acc_data[2]);
        printf("gyro_x : %d , gyro_y : %d , gyro_z : %d\n", gyro_data[0], gyro_data[1], gyro_data[2]);

        usleep(100000);
    }
}

finalization(fd);
return 0;
```
