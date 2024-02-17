[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MLX90614

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mlx90614/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

MLX90614は、非接触温度測定用の赤外線温度計です。 IRセンシティブサーモパイル検出器チップとシグナルコンディショニングASSPの両方が同じTO-39缶に統合されています。その低ノイズアンプ、17ビットADC、および強力なDSPユニットのおかげで、温度計の高精度と解像度が実現されます。 温度計は、デジタルPWMおよびSMBus（システム管理バス）出力で工場出荷時に校正されています。標準として、10ビットPWMは、測定温度を-20〜120℃の範囲で連続的に送信するように構成され、出力分解能は0.14です。 ℃。工場出荷時のデフォルトのPOR設定はSMBusです。

LibDriver MLX90614は、LibDriverによって起動されたMLX90614の全機能ドライバーです。 温度読み取り、ID読み取り、その他の機能を提供します。 LibDriverはMISRAに準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver MLX90614のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver MLX90614用のプラットフォームに依存しないIICバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver MLX90614ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver MLX90614プログラミング例が含まれています。

/ docディレクトリには、LibDriver MLX90614オフラインドキュメントが含まれています。

/ datasheetディレクトリには、MLX90614データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIICバステンプレートを参照して、指定したプラットフォームのIICバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

```c
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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/mlx90614/index.html](https://www.libdriver.com/docs/mlx90614/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。