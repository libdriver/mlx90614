<div align=center>
<img src="/doc/image/logo.png"/>
</div>

## LibDriver MLX90614

[English](/README.md) | [ 简体中文](/README_CN.md)

The MLX90614 is an Infra Red thermometer for noncontact temperature measurements. Both the IR sensitive thermopile detector chip and the signal conditioning ASSP are integrated in the same TO-39 can.Thanks to its low noise amplifier, 17-bit ADC and powerful DSP unit, a high accuracy and resolution of the thermometer is achieved.The thermometer comes factory calibrated with a digital PWM and SMBus (System Management Bus) output.As a standard, the 10-bit PWM is configured to continuously transmit the measured temperature in range of -20…120˚C, with an output resolution of 0.14˚C.The factory default POR setting is SMBus. 

LibDriver MLX90614 is the full function driver of MLX90614 launched by LibDriver. It provides temperature reading, id reading and other functions.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver MLX90614 source files.

/interface includes LibDriver MLX90614 IIC platform independent template。

/test includes LibDriver MLX90614 driver test code and this code can test the chip necessary function simply。

/example includes LibDriver MLX90614 sample code.

/doc includes LibDriver MLX90614 offline document.

/datasheet includes MLX90614 datasheet。

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

### Install

Reference /interface IIC platform independent template and finish your platform IIC driver.

Add /src, /interface and /example to your project.

### Usage

#### example basic

```C
volatile uint8_t res;
volatile uint32_t i;
volatile float ambient;
volatile float object;

/* init */
res = mlx90614_basic_init();
if (res)
{
    return 1;
}

for (i = 0; i < 3; i++)
{
    res = mlx90614_basic_read((float *)&ambient, (float *)&object);
    if (res)
    {
        mlx90614_basic_deinit();

        return 1;
    }

    /* print the data */
    mlx90614_interface_debug_print("mlx90614: %d/%d.\n", i + 1, 3);
    mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);

    /* delay 1000 ms */
    mlx90614_interface_delay_ms(1000);
}

mlx90614_basic_deinit();
```

#### example advance

```C
volatile uint8_t res;
volatile uint32_t i;
volatile float ambient;
volatile float object;

/* init */
res = mlx90614_advance_init();
if (res)
{
    return 1;
}

/* exit sleep */
res = mlx90614_advance_exit_sleep();
if (res)
{
    mlx90614_advance_deinit();

    return 1;
}

/* delay 2000 ms */
mlx90614_interface_delay_ms(2000);

/* read id */
res = mlx90614_advance_get_id((uint16_t *)id);
if (res)
{
    mlx90614_advance_deinit();

    return 1;
}

mlx90614_interface_debug_print("mlx90614: get id is 0x%02X 0x%02X 0x%02X 0x%02X.\n", id[0], id[1], id[2], id[3]);

for (i = 0; i < 3; i++)
{
    res = mlx90614_advance_read((float *)&ambient, (float *)&object);
    if (res)
    {
        mlx90614_advance_deinit();

        return 1;
    }

    /* print the data */
    mlx90614_interface_debug_print("mlx90614: %d/%d.\n", i + 1, 3);
    mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);

    /* delay 1000 ms */
    mlx90614_interface_delay_ms(1000);
}

/* enter sleep */
res = mlx90614_advance_enter_sleep();
if (res)
{
    mlx90614_advance_deinit();

    return 1;
}

mlx90614_advance_deinit();
```

### Document

Online documents: https://www.libdriver.com/docs/mlx90614/index.html

Offline documents: /doc/html/index.html

### Contributing

Please sent an e-mail to lishifenging@outlook.com

### License

Copyright (C) LibDriver 2015-2021 All rights reserved 



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please sent an e-mail to lishifenging@outlook.com