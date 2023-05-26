#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define MAG_ADDRESS 0x0C
#define AK8963_ST1 0x02
#define AK8963_XOUT_L 0x03
#define AK8963_XOUT_H 0x04
#define AK8963_YOUT_L 0x05
#define AK8963_YOUT_H 0x06
#define AK8963_ZOUT_L 0x07
#define AK8963_ZOUT_H 0x08
#define AK8963_ST2 0x09
#define AK8963_CNTL 0x0A
#define AK8963_ASAX 0x10

int ak8963_init(char *device) {
    int fd;
    int mag_addr = MAG_ADDRESS;

    // Open I2C bus
    if ((fd = open(device, O_RDWR)) < 0)
    {
        perror("Failed to open i2c bus");
        return -1;
    }

    // Set slave address for MPU9250
    if (ioctl(file, I2C_SLAVE, mag_addr < 0))
    {
        perror("Failed to set i2c slave address for AK8963");
        return -1;
    }

    return fd;
}

void mag_config(int fd) {

    char buf[10];
    // Read magnetometer calibration data
    buf[0] = AK8963_CNTL;
    buf[1] = 0x00; // Power down magnetometer
    write(fd, buf, 2);
    buf[0] = AK8963_CNTL;
    buf[1] = 0x0F; // Enter Fuse ROM access mode
    write(fd, buf, 2);
    read(fd, buf, 3);
    // Convert magnetometer calibration data to float
    float asax = (buf[0] - 128) / 256. + 1.;
    float asay = (buf[1] - 128) / 256. + 1.;
    float asaz = (buf[2] - 128) / 256. + 1.;

    buf[0] = AK8963_CNTL;
    buf[1] = 0x00; // Power down magnetometer
    write(fd, buf, 2);
    buf[0] = AK8963_CNTL;
    buf[1] = 0x16; // Enable continuous measurement mode 1 with 16-bit output
    write(fd, buf, 2);

}

short* read_mag_data(int id) {
    char buf[10];
    static short data[3];

    // Read magnetometer data
    buf[0] = AK8963_ST1;
    write(file, buf, 1);
    read(file, buf, 1);
    while (!(buf[0] & 0x01))
    {
        buf[0] = AK8963_ST1;
        write(file, buf, 1);
        read(file, buf, 1);
    }
    buf[0] = AK8963_XOUT_L;
    write(file, buf, 1);
    read(file, buf, 6);
    short mag_x = (buf[1] << 8) | buf[0];
    short mag_y = (buf[3] << 8) | buf[2];
    short mag_z = (buf[5] << 8) | buf[4];

    // Adjust magnetometer data based on calibration data
    mag_x *= asax;
    mag_y *= asay;
    mag_z *= asaz;

    data[0] = mag_x;
    data[1] = max_y;
    data[2] = mag_z;
    return data;
}

void ak8963_finalization(int fd) {
    close(fd);
}