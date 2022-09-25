### 1. Chip

#### 1.1 Chip Info

chip name : Raspberry Pi 4B.

iic pin: SCL/SDA GPIO3/GPIO2.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```
#### 2.2 Configuration

```shell
sudo vim /boot/config.txt

# add or change
dtparam=i2c_arm=on,i2c_arm_baudrate=80000

# reboot the device
reboot
```

#### 2.3 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.4 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(mlx90614 REQUIRED)
```

### 3. MLX90614

#### 3.1 Command Instruction

​          mlx90614 is a basic command which can test all mlx90614 driver function:

​          -i        show mlx90614 chip and driver information.

​          -h       show mlx90614 help.

​          -p       show mlx90614 pin connections of the current board.

​          -t (reg | read <times>)

​          -t reg        run mlx90614 register test.

​          -t read <times>        run mlx90614 read test. times means test times. 

​          -c (basic read <times> | advance read <times> | advance id | advance sleep | advance wake)

​          -c basic read <times>          run mlx90614 basic read function. times means read times.

​          -c advance read <times>          run mlx90614 advance read function. times means read times.  

​          -c advance id          run mlx90614 advance read id function.

​          -c advance sleep          run mlx90614 advance sleep function.

​          -c advance wake         run mlx90614 advance wake up function.

#### 3.2 Command Example

```shell
./mlx90614 -i

mlx90614: chip is Melexis MLX90614.
mlx90614: manufacturer is Melexis.
mlx90614: interface is IIC.
mlx90614: driver version is 1.0.
mlx90614: min supply voltage is 4.5V.
mlx90614: max supply voltage is 5.5V.
mlx90614: max current is 2.50mA.
mlx90614: max temperature is 125.0C.
mlx90614: min temperature is -40.0C.
```

```shell
./mlx90614 -p

mlx90614: SCL connected to GPIO3(BCM).
mlx90614: SDA connected to GPIO2(BCM).
```

```shell
./mlx90614 -t reg

mlx90614: chip is Melexis MLX90614.
mlx90614: manufacturer is Melexis.
mlx90614: interface is IIC.
mlx90614: driver version is 1.0.
mlx90614: min supply voltage is 4.5V.
mlx90614: max supply voltage is 5.5V.
mlx90614: max current is 2.50mA.
mlx90614: max temperature is 125.0C.
mlx90614: min temperature is -40.0C.
mlx90614: start register test.
mlx90614: mlx90614_set_addr/mlx90614_get_addr test.
mlx90614: set addr 0x68.
mlx90614: check addr ok.
mlx90614: mlx90614_set_fir_length/mlx90614_get_fir_length test.
mlx90614: set fir length 8.
mlx90614: check fir length ok.
mlx90614: set fir length 16.
mlx90614: check fir length ok.
mlx90614: set fir length 32.
mlx90614: check fir length ok.
mlx90614: set fir length 64.
mlx90614: check fir length ok.
mlx90614: set fir length 128.
mlx90614: check fir length ok.
mlx90614: set fir length 256.
mlx90614: check fir length ok.
mlx90614: set fir length 512.
mlx90614: check fir length ok.
mlx90614: set fir length 1024.
mlx90614: check fir length ok.
mlx90614: mlx90614_set_iir/mlx90614_get_iir test.
mlx90614: set iir a1 0.5 b1 0.5.
mlx90614: check iir ok.
mlx90614: set iir a1 0.25 b1 0.75.
mlx90614: check iir ok.
mlx90614: set iir a1 0.166 b1 0.83.
mlx90614: check iir ok.
mlx90614: set iir a1 0.125 b1 0.875.
mlx90614: check iir ok.
mlx90614: set iir a1 1 b1 0.
mlx90614: check iir ok.
mlx90614: set iir a1 0.8 b1 0.2.
mlx90614: check iir ok.
mlx90614: set iir a1 0.666 b1 0.333.
mlx90614: check iir ok.
mlx90614: set iir a1 0.571 b1 0.428.
mlx90614: check iir ok.
mlx90614: mlx90614_set_mode/mlx90614_get_mode test.
mlx90614: set ta tobj1 mode.
mlx90614: check mode ok.
mlx90614: set ta tobj2 mode.
mlx90614: check mode ok.
mlx90614: set tobj2 mode.
mlx90614: check mode ok.
mlx90614: set tobj1 tobj2 mode.
mlx90614: check mode ok.
mlx90614: mlx90614_set_ir_sensor/mlx90614_get_ir_sensor test.
mlx90614: set ir sensor single.
mlx90614: check ir sensor ok.
mlx90614: set ir sensor dual.
mlx90614: check ir sensor ok.
mlx90614: mlx90614_set_ks/mlx90614_get_ks test.
mlx90614: set ks positive.
mlx90614: check ks ok.
mlx90614: set ks negative.
mlx90614: check ks ok.
mlx90614: mlx90614_set_kt2/mlx90614_get_kt2 test.
mlx90614: set kt2 positive.
mlx90614: check kt2 ok.
mlx90614: set kt2 negative.
mlx90614: check kt2 ok.
mlx90614: mlx90614_set_gain/mlx90614_get_gain test.
mlx90614: set gain 1.
mlx90614: check gain ok.
mlx90614: set gain 3.
mlx90614: check gain ok.
mlx90614: set gain 6.
mlx90614: check gain ok.
mlx90614: set gain 12.5.
mlx90614: check gain ok.
mlx90614: set gain 25.
mlx90614: check gain ok.
mlx90614: set gain 50.
mlx90614: check gain ok.
mlx90614: set gain 100.
mlx90614: check gain ok.
mlx90614: mlx90614_set_sensor_test/mlx90614_get_sensor_test test.
mlx90614: enable sensor test.
mlx90614: check sensor test ok.
mlx90614: disable sensor test.
mlx90614: check sensor test ok.
mlx90614: mlx90614_set_repeat_sensor_test/mlx90614_get_repeat_sensor_test test.
mlx90614: enable repeat sensor test.
mlx90614: check repeat sensor test ok.
mlx90614: disable repeat sensor test.
mlx90614: check repeat sensor test ok.
mlx90614: mlx90614_set_emissivity_correction_coefficient/mlx90614_get_emissivity_correction_coefficient test.
mlx90614: set emissivity correction coefficient 44295.
mlx90614: check emissivity correction coefficient ok.
mlx90614: mlx90614_emissivity_correction_coefficient_convert_to_register/mlx90614_emissivity_correction_coefficient_convert_to_data test.
mlx90614: 0.325272 convert to register 0x5345.
mlx90614: 0x5345 convert to data 0.325277.
mlx90614: mlx90614_get_id test.
mlx90614: get id is 0x6001 0xEBA8 0x8213 0xE0D2.
mlx90614: mlx90614_enter_sleep_mode test.
mlx90614: enter sleep mode.
mlx90614: mlx90614_exit_sleep_mode test.
mlx90614: exit sleep mode.
mlx90614: mlx90614_get_flag test.
mlx90614: get flag 0x00FF.
mlx90614: finish register test.
```

```shell
./mlx90614 -t read 2

mlx90614: chip is Melexis MLX90614.
mlx90614: manufacturer is Melexis.
mlx90614: interface is IIC.
mlx90614: driver version is 1.0.
mlx90614: min supply voltage is 4.5V.
mlx90614: max supply voltage is 5.5V.
mlx90614: max current is 2.50mA.
mlx90614: max temperature is 125.0C.
mlx90614: min temperature is -40.0C.
mlx90614: start read test.
mlx90614: set fir length 8.
mlx90614: ambient is -240.05C object is -170.77C.
mlx90614: channel1 raw is 0x8679 channel2 raw is 0x8675.
mlx90614: ambient is -241.77C object is -75.67C.
mlx90614: channel1 raw is 0x0695 channel2 raw is 0x06AA.
mlx90614: set fir length 16.
mlx90614: ambient is 384.65C object is -89.63C.
mlx90614: channel1 raw is 0x0010 channel2 raw is 0x800B.
mlx90614: ambient is 384.65C object is -89.63C.
mlx90614: channel1 raw is 0x0010 channel2 raw is 0x801D.
mlx90614: set fir length 32.
mlx90614: ambient is -262.77C object is -88.01C.
mlx90614: channel1 raw is 0x0007 channel2 raw is 0x8006.
mlx90614: ambient is -262.75C object is -87.97C.
mlx90614: channel1 raw is 0x0007 channel2 raw is 0x8009.
mlx90614: set fir length 64.
mlx90614: ambient is 397.45C object is -94.91C.
mlx90614: channel1 raw is 0x800D channel2 raw is 0x0012.
mlx90614: ambient is 397.45C object is -94.99C.
mlx90614: channel1 raw is 0x800A channel2 raw is 0x0012.
mlx90614: set fir length 128.
mlx90614: ambient is 418.31C object is -113.93C.
mlx90614: channel1 raw is 0x802D channel2 raw is 0x004B.
mlx90614: ambient is 418.29C object is -113.85C.
mlx90614: channel1 raw is 0x802D channel2 raw is 0x003E.
mlx90614: set fir length 256.
mlx90614: ambient is 455.83C object is -106.45C.
mlx90614: channel1 raw is 0x8076 channel2 raw is 0x00A7.
mlx90614: ambient is 455.85C object is -106.47C.
mlx90614: channel1 raw is 0x8076 channel2 raw is 0x0099.
mlx90614: set fir length 512.
mlx90614: ambient is -119.23C object is -115.49C.
mlx90614: channel1 raw is 0x8103 channel2 raw is 0x0147.
mlx90614: ambient is -119.21C object is -115.21C.
mlx90614: channel1 raw is 0x8102 channel2 raw is 0x0155.
mlx90614: set fir length 1024.
mlx90614: ambient is 27.43C object is 17.61C.
mlx90614: channel1 raw is 0x8244 channel2 raw is 0x02A2.
mlx90614: ambient is 27.45C object is 17.57C.
mlx90614: channel1 raw is 0x8247 channel2 raw is 0x0299.
mlx90614: set iir a1 0.5 b1 0.5.
mlx90614: ambient is 27.47C object is 17.99C.
mlx90614: channel1 raw is 0x8231 channel2 raw is 0x029A.
mlx90614: ambient is 27.47C object is 17.89C.
mlx90614: channel1 raw is 0x8232 channel2 raw is 0x0296.
mlx90614: set iir a1 0.25 b1 0.75.
mlx90614: ambient is 27.45C object is 17.91C.
mlx90614: channel1 raw is 0x8233 channel2 raw is 0x01C9.
mlx90614: ambient is 27.47C object is 18.03C.
mlx90614: channel1 raw is 0x8227 channel2 raw is 0x02AC.
mlx90614: set iir a1 0.166 b1 0.83.
mlx90614: ambient is 27.47C object is 18.07C.
mlx90614: channel1 raw is 0x822B channel2 raw is 0x02A7.
mlx90614: ambient is 27.47C object is 18.07C.
mlx90614: channel1 raw is 0x8229 channel2 raw is 0x02AA.
mlx90614: set iir a1 0.125 b1 0.875.
mlx90614: ambient is 27.47C object is 18.15C.
mlx90614: channel1 raw is 0x8227 channel2 raw is 0x0291.
mlx90614: ambient is 27.47C object is 18.13C.
mlx90614: channel1 raw is 0x8228 channel2 raw is 0x02BF.
mlx90614: set iir a1 1 b1 0.
mlx90614: ambient is 27.53C object is 18.21C.
mlx90614: channel1 raw is 0x8226 channel2 raw is 0x02DB.
mlx90614: ambient is 27.53C object is 18.21C.
mlx90614: channel1 raw is 0x8226 channel2 raw is 0x02BD.
mlx90614: set iir a1 0.8 b1 0.2.
mlx90614: ambient is 27.53C object is 18.11C.
mlx90614: channel1 raw is 0x822B channel2 raw is 0x02E1.
mlx90614: ambient is 27.53C object is 18.05C.
mlx90614: channel1 raw is 0x8230 channel2 raw is 0x029E.
mlx90614: set iir a1 0.666 b1 0.333.
mlx90614: ambient is 27.57C object is 17.89C.
mlx90614: channel1 raw is 0x8239 channel2 raw is 0x02C8.
mlx90614: ambient is 27.55C object is 17.87C.
mlx90614: channel1 raw is 0x823B channel2 raw is 0x02E8.
mlx90614: set iir a1 0.571 b1 0.428.
mlx90614: ambient is 27.57C object is 18.05C.
mlx90614: channel1 raw is 0x8231 channel2 raw is 0x02C7.
mlx90614: ambient is 27.57C object is 18.09C.
mlx90614: channel1 raw is 0x822E channel2 raw is 0x02CF.
mlx90614: set ir sensor single.
mlx90614: ambient is 27.57C object is 17.89C.
mlx90614: channel1 raw is 0x823B channel2 raw is 0x02C0.
mlx90614: ambient is 27.59C object is 17.85C.
mlx90614: channel1 raw is 0x8246 channel2 raw is 0x02C0.
mlx90614: set ir sensor dual.
mlx90614: ambient is 680.47C object is -184.71C.
mlx90614: channel1 raw is 0x8243 channel2 raw is 0x02C5.
mlx90614: ambient is 680.49C object is -185.59C.
mlx90614: channel1 raw is 0x823E channel2 raw is 0x02CE.
mlx90614: set ks positive.
mlx90614: ambient is 680.49C object is -188.25C.
mlx90614: channel1 raw is 0x8233 channel2 raw is 0x02AE.
mlx90614: ambient is 680.49C object is -187.77C.
mlx90614: channel1 raw is 0x8235 channel2 raw is 0x02C2.
mlx90614: set ks negative.
mlx90614: ambient is 680.51C object is -187.61C.
mlx90614: channel1 raw is 0x8235 channel2 raw is 0x02C5.
mlx90614: ambient is 680.49C object is -186.43C.
mlx90614: channel1 raw is 0x823A channel2 raw is 0x02C5.
mlx90614: set kt2 positive.
mlx90614: ambient is 27.65C object is 18.07C.
mlx90614: channel1 raw is 0x8234 channel2 raw is 0x02D4.
mlx90614: ambient is 27.63C object is 17.99C.
mlx90614: channel1 raw is 0x8239 channel2 raw is 0x02BB.
mlx90614: set kt2 negative.
mlx90614: ambient is 27.67C object is 18.01C.
mlx90614: channel1 raw is 0x8239 channel2 raw is 0x02CF.
mlx90614: ambient is 27.65C object is 17.99C.
mlx90614: channel1 raw is 0x823B channel2 raw is 0x02AA.
mlx90614: set gain 1.
mlx90614: ambient is 27.69C object is 17.97C.
mlx90614: channel1 raw is 0x823D channel2 raw is 0x02CD.
mlx90614: ambient is 27.69C object is 17.91C.
mlx90614: channel1 raw is 0x8241 channel2 raw is 0x029D.
mlx90614: set gain 3.
mlx90614: ambient is 27.69C object is 17.93C.
mlx90614: channel1 raw is 0x8246 channel2 raw is 0x02B4.
mlx90614: ambient is 27.71C object is 17.87C.
mlx90614: channel1 raw is 0x8243 channel2 raw is 0x02CF.
mlx90614: set gain 6.
mlx90614: ambient is 27.71C object is 17.75C.
mlx90614: channel1 raw is 0x824C channel2 raw is 0x01DC.
mlx90614: ambient is 27.73C object is 17.77C.
mlx90614: channel1 raw is 0x824A channel2 raw is 0x02CB.
mlx90614: set gain 12.5.
mlx90614: ambient is 27.73C object is 17.85C.
mlx90614: channel1 raw is 0x8246 channel2 raw is 0x02E0.
mlx90614: ambient is 27.75C object is 17.91C.
mlx90614: channel1 raw is 0x8246 channel2 raw is 0x02C0.
mlx90614: set gain 25.
mlx90614: ambient is 27.79C object is 17.87C.
mlx90614: channel1 raw is 0x8249 channel2 raw is 0x02B3.
mlx90614: ambient is 27.77C object is 17.91C.
mlx90614: channel1 raw is 0x8247 channel2 raw is 0x02B6.
mlx90614: set gain 50.
mlx90614: ambient is 27.81C object is 17.81C.
mlx90614: channel1 raw is 0x824F channel2 raw is 0x01E2.
mlx90614: ambient is 27.81C object is 17.85C.
mlx90614: channel1 raw is 0x824A channel2 raw is 0x02BE.
mlx90614: set gain 100.
mlx90614: ambient is 27.81C object is 17.77C.
mlx90614: channel1 raw is 0x8251 channel2 raw is 0x02C9.
mlx90614: ambient is 27.83C object is 17.81C.
mlx90614: channel1 raw is 0x8250 channel2 raw is 0x0297.
mlx90614: set emissivity correction coefficient 8651.
mlx90614: ambient is 27.81C object is -124.21C.
mlx90614: channel1 raw is 0x8254 channel2 raw is 0x02C4.
mlx90614: ambient is 27.83C object is -126.17C.
mlx90614: channel1 raw is 0x8252 channel2 raw is 0x02AC.
mlx90614: set ta tobj1 mode.
mlx90614: ambient is 27.83C object is -124.45C.
mlx90614: channel1 raw is 0x8251 channel2 raw is 0x02A6.
mlx90614: ambient is 27.85C object is -124.23C.
mlx90614: channel1 raw is 0x824F channel2 raw is 0x0299.
mlx90614: set ta tobj2 mode.
mlx90614: ambient is 27.81C object is 27.83C.
mlx90614: channel1 raw is 0x8258 channel2 raw is 0x02C3.
mlx90614: ambient is 27.83C object is 27.85C.
mlx90614: channel1 raw is 0x8251 channel2 raw is 0x029C.
mlx90614: set tobj2 mode.
mlx90614: object is 27.83C.
mlx90614: channel1 raw is 0x8257 channel2 raw is 0x02AB.
mlx90614: object is 27.85C.
mlx90614: channel1 raw is 0x825B channel2 raw is 0x02C0.
mlx90614: set tobj1 tobj2 mode.
mlx90614: object1 is -127.33C object2 is 27.85C.
mlx90614: channel1 raw is 0x8252 channel2 raw is 0x02A0.
mlx90614: object1 is -126.67C object2 is 27.85C.
mlx90614: channel1 raw is 0x8252 channel2 raw is 0x02CF.
mlx90614: finish read test.
```

```shell
./mlx90614 -c basic read 3

mlx90614: 1/3.
mlx90614: ambient is 24.13C object is 22.67C.
mlx90614: 2/3.
mlx90614: ambient is 24.39C object is 20.07C.
mlx90614: 3/3.
mlx90614: ambient is 24.45C object is 19.37C.
```

```shell
./mlx90614 -c advance read 3

mlx90614: 1/3.
mlx90614: ambient is 25.17C object is 20.89C.
mlx90614: 2/3.
mlx90614: ambient is 25.19C object is 20.87C.
mlx90614: 3/3.
mlx90614: ambient is 25.27C object is 20.99C.
```

```shell
./mlx90614 -c advance id

mlx90614: get id is 0x6001 0xEBA8 0x8213 0xE0D2.
```

```shell
./mlx90614 -c advance sleep

mlx90614: enter sleep.
```

```shell
./mlx90614 -c advance wake

mlx90614: exit sleep.
```

```shell
./mlx90614 -h

mlx90614 -i
	show mlx90614 chip and driver information.
mlx90614 -h
	show mlx90614 help.
mlx90614 -p
	show mlx90614 pin connections of the current board.
mlx90614 -t reg
	run mlx90614 register test.
mlx90614 -t read <times>
	run mlx90614 read test.times means test times.
mlx90614 -c basic read <times>
	run mlx90614 basic read function.times means read times.
mlx90614 -c advance read <times>
	run mlx90614 advance read function.times means read times.
mlx90614 -c advance id
	run mlx90614 advance read id function.
mlx90614 -c advance sleep
	run mlx90614 advance sleep function.
mlx90614 -c advance wake
	run mlx90614 advance wake up function.
```

