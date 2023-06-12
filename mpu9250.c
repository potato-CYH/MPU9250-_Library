#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include<unistd.h>


int mpu9250_init()
{
  int fd = open("/dev/mpu9250", O_RDWR);
  return fd;
}

void config_acc(int fd, char _set_full_scale){
  
  char full_scale[1] = {_set_full_scale};
  ioctl(fd, _IO(0, 3), full_scale);
}

short *read_acc_data(int fd){
  static short ret[3];

  ioctl(fd, _IO(0, 4), ret);
  return ret;
}

void config_gyro(int fd, char _set_full_scale){
  char full_scale[1] = {_set_full_scale};
  ioctl(fd, _IO(0, 5), full_scale);
}

short *read_gyro_data(int fd){
  static short ret[3];

  ioctl(fd, _IO(0, 6), ret);
  return ret;
}


void finalization(int fd){
  close(fd);
}