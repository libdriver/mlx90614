[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MLX90614 

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mlx90614/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

邁來芯 MLX90614 是一款用於非接觸式溫度測量的紅外溫度計。 IR 敏感型熱電堆檢測器芯片和信號調節 ASIC 都集成在同一 TO-39 罐封裝中。 MLX90614 集成有低噪聲放大器、17 位 ADC 和強大的 DSP 單元，因此溫度計兼具高精度和高分辨率。該溫度計出廠前已經過校準，可通過數字 SMBus 輸出提供整個溫度範圍內的測量溫度（分辨率為 0.02°C）。用戶可以將數字輸出配置為脈寬調製 (PWM)。標準情況下，將 10 位 PWM 配置為以 0.14°C 的分辨率連續傳輸介於 -20 和 120°C 之間的測量溫度。

LibDriver MLX90614 是LibDriver推出的MLX90614 全功能驅動，該驅動提供溫度讀取，ID讀取等功能並且它符合MISRA標準。   

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver MLX90614的源文件。

/interface目錄包含了LibDriver MLX90614 與平台無關的IIC總線模板。

/test目錄包含了LibDriver MLX90614 驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver MLX90614 編程範例。

/doc目錄包含了LibDriver MLX90614 離線文檔。

/datasheet目錄包含了MLX90614 數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC總線模板，完成指定平台的IIC總線驅動。

將/src目錄，/interface目錄和/example目錄加入工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/mlx90614/index.html](https://www.libdriver.com/docs/mlx90614/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。