### Library Information

| Target Device | MPU 9250 |
| --- | --- |

### Feature

- Get Acceleration Value
- Get Gyro Value
- Get Magnetometer Value ( AK 8963 packaged in MPU 9250)

### User Manual

- **Download Library from GitHub.**
  
    [repo url](https://github.com/potato-CYH/MPU9250-_Library.git)
    

### library docment

### MPU9250 Initializaton

## `**int mpu9250_init(char *device);**`

- **parameter**
    - `**devce*` :  I2C device file root ( ex : ”/dev/i2c-x” )
- **return type** : `*int*`
- **Return**
    - return value `-1`  : initiallization Failed
    - return `integer` : initiallization Success    

### AK8963 Initializaton

## `**int ak8963_init(char *device);**`

- **parameter**
    - `**devce*` :  I2C device file root ( ex : ”/dev/i2c-x” )
- **return type** : `*int*`
- **Return**
    - return value `-1`  : initiallization Failed
    - return `integer` : initiallization Success    

### MPU9250 Accelerometer Configuration

## `**void acc_config(int fd, char _set_full_scale);**`

- **return type** : `*void*`
- **parameter**
    - `fd` : initiallization value from *mpu9250_init()*
    - `scale` : set prescaler
        
        
        | 0x00 | +- 2g |
        | --- | --- |
        | 0x01 | +- 4g |
        | 0x02 | +- 8g |
        | 0x03 | +- 16g |
      
### MPU9250 Gyroscope Configuration

## `void gyro_config(int fd, char _set_full_scale);`

- **return type** : `*void*`
- **parameter**
    - `fd` : initiallization value from *mpu9250_init()*
    - `scale` : set prescaler
        
        
        | 0x00 | +250 dps |
        | --- | --- |
        | 0x01 | +500 dps |
        | 0x02 | +1000 dps  |
        | 0x03 | +2000 dps |
    
### AK8963 Configuration

## `void mag_config(int fd);`

- **return type** : `*void*`
    
### MPU9250 Accelerometer Read

## `short *read_acc_data(int fd);`

- **return type** : `*short` (array pointer)
- **parameter**
    - `fd` : initiallization value from *mpu9250_init()*
- **Return**
    - return `*{accel_x, accel_y, accel_z}*`
    
### MPU9250 Gyroscope Read

## `short *read_gyro_data(int fd);`

- **return type** : `*short` (array pointer)
- **parameter**
    - `fd` : initiallization value from *mpu9250_init()*
- **Return**
    - return `*{gyro_x, gyro_y, gyro_z}*`
    
### AK8963 Magnetometer Read

## `short* read_mag_data(int id);`

- **return type** : `*short` (array pointer)
- **parameter**
    - `fd` : initiallization value from *mpu9250_init()*
- **Return**
    - return `*{mag_x, mag_y, mag_z}*`
    
### MPU9250 Disconnect

## `void finalization(int fd);`

- **return type** : `*void*`
- **parameter**
    - `fd` : initiallization value from *mpu9250_init()*
    

**** Attention : Must call finalization() when you finish using sensor.**
    
### AK8963 Disconnect

## `void ak8963_finalization(int fd);`

- **return type** : `*void*`
- **parameter**
    - `fd` : initiallization value from *ak8963_init()*

**** Attention : Must call finalization() when you finish using sensor.**
