[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.png"/>
</div>

## LibDriver MLX90614
[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mlx90614/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Das MLX90614 ist ein Infrarot-Thermometer für berührungslose Temperaturmessungen. Sowohl der IR-empfindliche Thermopile-Detektorchip als auch die Signalkonditionierungs-ASSP sind in demselben TO-39-Gehäuse integriert. Dank seines rauscharmen Verstärkers, 17-Bit-ADC und der leistungsstarken DSP-Einheit wird eine hohe Genauigkeit und Auflösung des Thermometers erreicht Das Thermometer wird werkseitig mit einem digitalen PWM- und SMBus-Ausgang (System Management Bus) kalibriert geliefert. Standardmäßig ist der 10-Bit-PWM so konfiguriert, dass er die gemessene Temperatur im Bereich von -20 bis 120 °C mit einer Ausgangsauflösung von 0,14 kontinuierlich überträgt ˚C.Die werkseitige POR-Standardeinstellung ist SMBus.

LibDriver MLX90614 ist der Treiber mit vollem Funktionsumfang von MLX90614, der von LibDriver gestartet wurde. Es bietet Temperaturablesung, ID-Lesung und andere Funktionen. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver MLX90614-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver MLX90614 IIC。

/test enthält den Testcode des LibDriver MLX90614-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen。

/example enthält LibDriver MLX90614-Beispielcode.

/doc enthält das LibDriver MLX90614-Offlinedokument.

/Datenblatt enthält MLX90614-Datenblatt。

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC-Treiber fertig.

Fügen Sie /src, /interface und /example zu Ihrem Projekt hinzu.

### Nutzung

#### example basic

```C
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

### Dokument

Online-Dokumente: https://www.libdriver.com/docs/mlx90614/index.html

Offline-Dokumente: /doc/html/index.html

### Beitrag

Bitte senden Sie eine E-Mail an lishifenging@outlook.com

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com