#include <stdio.h>
#include <stdlib.h>

#include "mpu9250.h"

int main(int argc, char** argv){
    
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


}
