### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LidDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. MLX90614

#### 3.1 Command Instruction

1. Show mlx90614 chip and driver information.

   ```shell
   mlx90614 (-i | --information)
   ```

2. Show mlx90614 help.

   ```shell
   mlx90614 (-h | --help)
   ```

3. Show mlx90614 pin connections of the current board.

   ```shell
   mlx90614 (-p | --port)
   ```

4. Run mlx90614 register test.

   ```shell
   mlx90614 (-t reg | --test=reg)
   ```

5. Run mlx90614 read test, num means test times.

   ```shell
   mlx90614 (-t read | --test=read) [--times=<num>]
   ```

6. Run mlx90614 basic read function, num means read times.

   ```shell
   mlx90614 (-e read | --example=read) [--times=<num>]
   ```

7. Run mlx90614 advance read function, num means read times.  

   ```shell
   mlx90614 (-e advance-read | --example=advance-read) [--times=<num>]
   ```

8. Run mlx90614 advance read id function.

   ```shell
   mlx90614 (-e advance-id | --example=advance-id)
   ```

9. Run mlx90614 advance sleep function.

   ```shell
   mlx90614 (-e advance-sleep | --example=advance-sleep)
   ```

10. Run mlx90614 advance wake up function.

    ```shell
    mlx90614 (-e advance-wake | --example=advance-wake)
    ```

#### 3.2 Command Example

```shell
mlx90614 -i

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
mlx90614 -p

mlx90614: SCL connected to GPIOB PIN8.
mlx90614: SDA connected to GPIOB PIN9.
```

```shell
mlx90614 -t reg

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
mlx90614: set addr 0xB0.
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
mlx90614: set emissivity correction coefficient 8651.
mlx90614: check emissivity correction coefficient ok.
mlx90614: mlx90614_emissivity_correction_coefficient_convert_to_register/mlx90614_emissivity_correction_coefficient_convert_to_data test.
mlx90614: 0.481583 convert to register 0x7B49.
mlx90614: 0x7B49 convert to data 0.481590.
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
mlx90614 -t read --times=2

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
mlx90614: ambient is -240.23C object is -103.15C.
mlx90614: channel1 raw is 0x87D5 channel2 raw is 0x87CF.
mlx90614: ambient is -241.67C object is -102.31C.
mlx90614: channel1 raw is 0x07F7 channel2 raw is 0x87CC.
mlx90614: set fir length 16.
mlx90614: ambient is -267.01C object is -78.61C.
mlx90614: channel1 raw is 0x0014 channel2 raw is 0x800F.
mlx90614: ambient is -267.01C object is -78.55C.
mlx90614: channel1 raw is 0x0014 channel2 raw is 0x800D.
mlx90614: set fir length 32.
mlx90614: ambient is 388.97C object is -93.23C.
mlx90614: channel1 raw is 0x0010 channel2 raw is 0x8004.
mlx90614: ambient is 388.97C object is -93.77C.
mlx90614: channel1 raw is 0x000F channel2 raw is 0x800A.
mlx90614: set fir length 64.
mlx90614: ambient is 397.53C object is -89.29C.
mlx90614: channel1 raw is 0x0005 channel2 raw is 0x0008.
mlx90614: ambient is 397.53C object is -89.43C.
mlx90614: channel1 raw is 0x0005 channel2 raw is 0x0001.
mlx90614: set fir length 128.
mlx90614: ambient is -232.75C object is -100.07C.
mlx90614: channel1 raw is 0x800D channel2 raw is 0x0011.
mlx90614: ambient is -232.77C object is -99.29C.
mlx90614: channel1 raw is 0x800D channel2 raw is 0x001A.
mlx90614: set fir length 256.
mlx90614: ambient is -193.73C object is -142.41C.
mlx90614: channel1 raw is 0x8036 channel2 raw is 0x0045.
mlx90614: ambient is -193.75C object is -135.95C.
mlx90614: channel1 raw is 0x8034 channel2 raw is 0x0050.
mlx90614: set fir length 512.
mlx90614: ambient is 530.49C object is -213.79C.
mlx90614: channel1 raw is 0x807E channel2 raw is 0x00B4.
mlx90614: ambient is 530.47C object is -212.01C.
mlx90614: channel1 raw is 0x8081 channel2 raw is 0x00BB.
mlx90614: set fir length 1024.
mlx90614: ambient is 29.07C object is -13.81C.
mlx90614: channel1 raw is 0x811F channel2 raw is 0x0173.
mlx90614: ambient is 29.07C object is -13.39C.
mlx90614: channel1 raw is 0x811A channel2 raw is 0x0161.
mlx90614: set iir a1 0.5 b1 0.5.
mlx90614: ambient is 681.67C object is -200.65C.
mlx90614: channel1 raw is 0x8118 channel2 raw is 0x019A.
mlx90614: ambient is 681.65C object is -196.81C.
mlx90614: channel1 raw is 0x8114 channel2 raw is 0x0197.
mlx90614: set iir a1 0.25 b1 0.75.
mlx90614: ambient is 681.63C object is -189.95C.
mlx90614: channel1 raw is 0x8112 channel2 raw is 0x0174.
mlx90614: ambient is 681.63C object is -183.91C.
mlx90614: channel1 raw is 0x810C channel2 raw is 0x0177.
mlx90614: set iir a1 0.166 b1 0.83.
mlx90614: ambient is 20.07C object is -83.73C.
mlx90614: channel1 raw is 0x8112 channel2 raw is 0x0161.
mlx90614: ambient is 26.87C object is -35.79C.
mlx90614: channel1 raw is 0x810F channel2 raw is 0x0188.
mlx90614: set iir a1 0.125 b1 0.875.
mlx90614: ambient is 28.89C object is -9.45C.
mlx90614: channel1 raw is 0x8101 channel2 raw is 0x0153.
mlx90614: ambient is 28.89C object is -8.83C.
mlx90614: channel1 raw is 0x8101 channel2 raw is 0x015F.
mlx90614: set iir a1 1 b1 0.
mlx90614: ambient is 28.85C object is -9.19C.
mlx90614: channel1 raw is 0x8104 channel2 raw is 0x017A.
mlx90614: ambient is 28.85C object is -8.63C.
mlx90614: channel1 raw is 0x8101 channel2 raw is 0x0179.
mlx90614: set iir a1 0.8 b1 0.2.
mlx90614: ambient is 28.83C object is -7.91C.
mlx90614: channel1 raw is 0x80FD channel2 raw is 0x015E.
mlx90614: ambient is 28.83C object is -7.75C.
mlx90614: channel1 raw is 0x80FD channel2 raw is 0x014F.
mlx90614: set iir a1 0.666 b1 0.333.
mlx90614: ambient is 681.49C object is -113.69C.
mlx90614: channel1 raw is 0x80F6 channel2 raw is 0x018D.
mlx90614: ambient is 681.47C object is -109.11C.
mlx90614: channel1 raw is 0x80F5 channel2 raw is 0x0167.
mlx90614: set iir a1 0.571 b1 0.428.
mlx90614: ambient is 681.45C object is -140.53C.
mlx90614: channel1 raw is 0x80F9 channel2 raw is 0x0161.
mlx90614: ambient is 681.45C object is -109.07C.
mlx90614: channel1 raw is 0x80F6 channel2 raw is 0x0177.
mlx90614: set ir sensor single.
mlx90614: ambient is 28.75C object is -5.19C.
mlx90614: channel1 raw is 0x80ED channel2 raw is 0x0194.
mlx90614: ambient is 28.73C object is -6.05C.
mlx90614: channel1 raw is 0x80F2 channel2 raw is 0x0194.
mlx90614: set ir sensor dual.
mlx90614: ambient is 681.43C object is -154.49C.
mlx90614: channel1 raw is 0x80EF channel2 raw is 0x017E.
mlx90614: ambient is 681.41C object is -130.03C.
mlx90614: channel1 raw is 0x80F2 channel2 raw is 0x016F.
mlx90614: set ks positive.
mlx90614: ambient is 681.41C object is -218.21C.
mlx90614: channel1 raw is 0x80EE channel2 raw is 0x014B.
mlx90614: ambient is 681.41C object is -83.43C.
mlx90614: channel1 raw is 0x80F5 channel2 raw is 0x015A.
mlx90614: set ks negative.
mlx90614: ambient is 28.65C object is -5.69C.
mlx90614: channel1 raw is 0x80F0 channel2 raw is 0x014F.
mlx90614: ambient is 28.67C object is -5.77C.
mlx90614: channel1 raw is 0x80EE channel2 raw is 0x0144.
mlx90614: set kt2 positive.
mlx90614: ambient is 28.63C object is -5.37C.
mlx90614: channel1 raw is 0x80EE channel2 raw is 0x016D.
mlx90614: ambient is 28.63C object is -5.07C.
mlx90614: channel1 raw is 0x80EB channel2 raw is 0x014E.
mlx90614: set kt2 negative.
mlx90614: ambient is 28.61C object is -4.77C.
mlx90614: channel1 raw is 0x80EA channel2 raw is 0x0184.
mlx90614: ambient is 28.61C object is -4.25C.
mlx90614: channel1 raw is 0x80E3 channel2 raw is 0x018D.
mlx90614: set gain 1.
mlx90614: ambient is 28.57C object is -3.95C.
mlx90614: channel1 raw is 0x80E4 channel2 raw is 0x015D.
mlx90614: ambient is 28.57C object is -3.93C.
mlx90614: channel1 raw is 0x80E4 channel2 raw is 0x015E.
mlx90614: set gain 3.
mlx90614: ambient is 681.27C object is -135.87C.
mlx90614: channel1 raw is 0x80DE channel2 raw is 0x0160.
mlx90614: ambient is 681.27C object is -128.27C.
mlx90614: channel1 raw is 0x80DC channel2 raw is 0x0146.
mlx90614: set gain 6.
mlx90614: ambient is 681.27C object is -130.23C.
mlx90614: channel1 raw is 0x80DC channel2 raw is 0x015F.
mlx90614: ambient is 681.27C object is -137.53C.
mlx90614: channel1 raw is 0x80DF channel2 raw is 0x0194.
mlx90614: set gain 12.5.
mlx90614: ambient is 28.51C object is -4.33C.
mlx90614: channel1 raw is 0x80E4 channel2 raw is 0x0164.
mlx90614: ambient is 28.49C object is -3.93C.
mlx90614: channel1 raw is 0x80E4 channel2 raw is 0x0166.
mlx90614: set gain 25.
mlx90614: ambient is 28.47C object is -1.51C.
mlx90614: channel1 raw is 0x80D5 channel2 raw is 0x017A.
mlx90614: ambient is 28.49C object is -3.51C.
mlx90614: channel1 raw is 0x80E2 channel2 raw is 0x0171.
mlx90614: set gain 50.
mlx90614: ambient is 28.45C object is -2.41C.
mlx90614: channel1 raw is 0x80D9 channel2 raw is 0x016E.
mlx90614: ambient is 28.45C object is -1.67C.
mlx90614: channel1 raw is 0x80D4 channel2 raw is 0x013A.
mlx90614: set gain 100.
mlx90614: ambient is 28.45C object is -1.45C.
mlx90614: channel1 raw is 0x80D4 channel2 raw is 0x015E.
mlx90614: ambient is 28.45C object is -1.55C.
mlx90614: channel1 raw is 0x80D4 channel2 raw is 0x015F.
mlx90614: set emissivity correction coefficient 55400.
mlx90614: ambient is 681.17C object is -135.57C.
mlx90614: channel1 raw is 0x8079 channel2 raw is 0x015A.
mlx90614: ambient is 681.17C object is -119.55C.
mlx90614: channel1 raw is 0x807B channel2 raw is 0x0151.
mlx90614: set ta tobj1 mode.
mlx90614: ambient is 681.17C object is -96.35C.
mlx90614: channel1 raw is 0x8018 channel2 raw is 0x0167.
mlx90614: ambient is 681.15C object is -95.53C.
mlx90614: channel1 raw is 0x8016 channel2 raw is 0x0157.
mlx90614: set ta tobj2 mode.
mlx90614: ambient is 681.17C object is -90.93C.
mlx90614: channel1 raw is 0x8023 channel2 raw is 0x016E.
mlx90614: ambient is 681.15C object is -90.79C.
mlx90614: channel1 raw is 0x001F channel2 raw is 0x0169.
mlx90614: set tobj2 mode.
mlx90614: object is -90.79C.
mlx90614: channel1 raw is 0x0004 channel2 raw is 0x015E.
mlx90614: object is -90.79C.
mlx90614: channel1 raw is 0x800B channel2 raw is 0x0157.
mlx90614: set tobj1 tobj2 mode.
mlx90614: object1 is 28.21C object2 is 28.37C.
mlx90614: channel1 raw is 0x8008 channel2 raw is 0x016F.
mlx90614: object1 is 28.23C object2 is 28.37C.
mlx90614: channel1 raw is 0x8008 channel2 raw is 0x0175.
mlx90614: finish read test.
```

```shell
mlx90614 -e read --times=3

mlx90614: 1/3.
mlx90614: ambient is 27.77C object is 26.05C.
mlx90614: 2/3.
mlx90614: ambient is 27.79C object is 25.07C.
mlx90614: 3/3.
mlx90614: ambient is 27.79C object is 25.03C.
```

```shell
mlx90614 -e advance-read --times=3

mlx90614: 1/3.
mlx90614: ambient is 27.73C object is 26.91C.
mlx90614: 2/3.
mlx90614: ambient is 27.75C object is 26.99C.
mlx90614: 3/3.
mlx90614: ambient is 27.75C object is 26.91C.
```

```shell
mlx90614 -e advance-id

mlx90614: get id is 0x6001 0xEBA8 0x8213 0xE0D2.
```

```shell
mlx90614 -e advance-sleep

mlx90614: enter sleep.
```

```shell
mlx90614 -e advance-wake

mlx90614: exit sleep.
```

```shell
mlx90614 -h

Usage:
  mlx90614 (-i | --information)
  mlx90614 (-h | --help)
  mlx90614 (-p | --port)
  mlx90614 (-t reg | --test=reg)
  mlx90614 (-t read | --test=read) [--times=<num>]
  mlx90614 (-e read | --example=read) [--times=<num>]
  mlx90614 (-e advance-read | --example=advance-read) [--times=<num>]
  mlx90614 (-e advance-id | --example=advance-id)
  mlx90614 (-e advance-sleep | --example=advance-sleep)
  mlx90614 (-e advance-wake | --example=advance-wake)

Options:
  -e <read | advance-read | advance-id | advance-sleep | advance-wake>, --example=
     <read | advance-read | advance-id | advance-sleep | advance-wake>
                       Run the driver example.
  -h, --help           Show the help.
  -i, --information    Show the chip information.
  -p, --port           Display the pin connections of the current board.
  -t <reg | read>, --test=<reg | read>
                       Run the driver test.
      --times=<num>    Set the running times.([default: 3])
```

