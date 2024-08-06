[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MLX90614 

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mlx90614/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

迈来芯 MLX90614 是一款用于非接触式温度测量的红外温度计。IR 敏感型热电堆检测器芯片和信号调节 ASIC 都集成在同一 TO-39 罐封装中。MLX90614 集成有低噪声放大器、17 位 ADC 和强大的 DSP 单元，因此温度计兼具高精度和高分辨率。该温度计出厂前已经过校准，可通过数字 SMBus 输出整个温度范围内的测量温度（分辨率为 0.02°C）。用户可以将数字输出配置为脉宽调制 (PWM)。标准情况下，将 10 位 PWM 配置为以 0.14°C 的分辨率连续传输介于 -20 和 120°C 之间的测量温度。

LibDriver MLX90614 是LibDriver推出的MLX90614 全功能驱动，该驱动提供温度读取，ID读取等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver MLX90614的源文件。

/interface目录包含了LibDriver MLX90614 与平台无关的IIC总线模板。

/test目录包含了LibDriver MLX90614 驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver MLX90614 编程范例。

/doc目录包含了LibDriver MLX90614 离线文档。

/datasheet目录包含了MLX90614 数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的IIC总线模板，完成指定平台的IIC总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

#### example basic

```C
#include "driver_mlx90614_basic.h"

uint8_t res;
uint32_t i;
float ambient;
float object;

/* init */
res = mlx90614_basic_init();
if (res != 0)
{
    return 1;
}

...
    
for (i = 0; i < 3; i++)
{
    res = mlx90614_basic_read((float *)&ambient, (float *)&object);
    if (res != 0)
    {
        (void)mlx90614_basic_deinit();

        return 1;
    }

    /* print the data */
    mlx90614_interface_debug_print("mlx90614: %d/%d.\n", i + 1, 3);
    mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);

    /* delay 1000 ms */
    mlx90614_interface_delay_ms(1000);
    
    ...
        
}

...
    
(void)mlx90614_basic_deinit();

return 0;
```

#### example advance

```C
#include "driver_mlx90614_advance.h"

uint8_t res;
uint32_t i;
float ambient;
float object;

/* init */
res = mlx90614_advance_init();
if (res != 0)
{
    return 1;
}

/* exit sleep */
res = mlx90614_advance_exit_sleep();
if (res != 0)
{
    (void)mlx90614_advance_deinit();

    return 1;
}

...
    
/* delay 2000 ms */
mlx90614_interface_delay_ms(2000);

/* read id */
res = mlx90614_advance_get_id((uint16_t *)id);
if (res != 0)
{
    (void)mlx90614_advance_deinit();

    return 1;
}

mlx90614_interface_debug_print("mlx90614: get id is 0x%02X 0x%02X 0x%02X 0x%02X.\n", id[0], id[1], id[2], id[3]);

...
    
for (i = 0; i < 3; i++)
{
    res = mlx90614_advance_read((float *)&ambient, (float *)&object);
    if (res != 0)
    {
        (void)mlx90614_advance_deinit();

        return 1;
    }

    /* print the data */
    mlx90614_interface_debug_print("mlx90614: %d/%d.\n", i + 1, 3);
    mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);

    /* delay 1000 ms */
    mlx90614_interface_delay_ms(1000);
    
    ...
}

/* enter sleep */
res = mlx90614_advance_enter_sleep();
if (res != 0)
{
    (void)mlx90614_advance_deinit();

    return 1;
}

...
    
(void)mlx90614_advance_deinit();

return 0;
```

### 文档

在线文档: [https://www.libdriver.com/docs/mlx90614/index.html](https://www.libdriver.com/docs/mlx90614/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。