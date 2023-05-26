#ifndef _MPU9250_
#define _MPU9250_

#include "mpu9250.c"
#include "ak8963.c"

int mpu9250_init(char *device);
int ak8963_init(char *device);
void acc_config(int fd, char _set_full_scale);
void gyro_config(int fd, char _set_full_scale);
void mag_config(int fd)
short *read_acc_data(int fd);
short *read_gyro_data(int fd);
short* read_mag_data(int id)
void mpu9250_finalization(int fd);
void ak8963_finalization(int fd);

#endif