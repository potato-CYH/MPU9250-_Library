#ifndef _MPU9250_
#define _MPU9250_

#include "mpu9250.c"


int mpu9250_init();
void cofig_acc(int fd, char _set_full_scale);
short *read_acc_data(int fd);
void config_gyro(int fd, char _set_full_scale);
short *read_gyro_data(int fd);
void finalization(int fd);

#endif

