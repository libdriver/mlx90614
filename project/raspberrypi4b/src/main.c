/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-03-13
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/03/13  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_mlx90614_basic.h"
#include "driver_mlx90614_advance.h"
#include "driver_mlx90614_register_test.h"
#include "driver_mlx90614_read_test.h"
#include <getopt.h>
#include <stdlib.h>

/**
 * @brief     mlx90614 full function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t mlx90614(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"times", required_argument, NULL, 1},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* running times */
            case 1 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* run reg test */
        if (mlx90614_register_test() != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_read", type) == 0)
    {
        /* run read test */
        if (mlx90614_read_test(times) != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float ambient;
        float object;
        
        /* basic init */
        res = mlx90614_basic_init();
        if (res)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* read data */
            res = mlx90614_basic_read((float *)&ambient, (float *)&object);
            if (res)
            {
                mlx90614_basic_deinit();
                
                return 1;
            }
            
            /* output */
            mlx90614_interface_debug_print("mlx90614: %d/%d.\n", i + 1, times);
            mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
            
            /* delay 1000 ms */
            mlx90614_interface_delay_ms(1000);
        }
        
        /* basic deinit */
        (void)mlx90614_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_advance-read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float ambient;
        float object;
        
        /* advance init */
        res = mlx90614_advance_init();
        if (res)
        {
            return 1;
        }
            
        /* delay 2000 ms */
        mlx90614_interface_delay_ms(2000);
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* read data */
            res = mlx90614_advance_read((float *)&ambient, (float *)&object);
            if (res)
            {
                mlx90614_advance_deinit();
                
                return 1;
            }
            
            /* output */
            mlx90614_interface_debug_print("mlx90614: %d/%d.\n", i + 1, times);
            mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
            
            /* delay 1000 ms */
            mlx90614_interface_delay_ms(1000);
        }
        
        /* advance deinit */
        (void)mlx90614_advance_deinit();
        
        return 0;
    }
    else if (strcmp("e_advance-id", type) == 0)
    {
        uint8_t res;
        uint16_t id[4];
        
        /* advance init */
        res = mlx90614_advance_init();
        if (res)
        {
            mlx90614_advance_deinit();
            
            return 1;
        }
        
        /* read id */
        res = mlx90614_advance_get_id((uint16_t *)id);
        if (res)
        {
            mlx90614_advance_deinit();
            
            return 1;
        }
        
        /* output */
        mlx90614_interface_debug_print("mlx90614: get id is 0x%02X 0x%02X 0x%02X 0x%02X.\n", id[0], id[1], id[2], id[3]);
        
        /* advance deinit */
        (void)mlx90614_advance_deinit();
        
        return 0;
    }
    else if (strcmp("e_advance-sleep", type) == 0)
    {
        uint8_t res;
        
        /* advance init */
        res = mlx90614_advance_init();
        if (res)
        {
            mlx90614_advance_deinit();
            
            return 1;
        }
        
        /* enter sleep */
        res = mlx90614_advance_enter_sleep();
        if (res)
        {
            mlx90614_advance_deinit();
            
            return 1;
        }
        
        /* output */
        mlx90614_interface_debug_print("mlx90614: enter sleep.\n");
        
        /* advance deinit */
        (void)mlx90614_advance_deinit();
        
        return 0;
    }
    else if (strcmp("e_advance-wake", type) == 0)
    {
        uint8_t res;
        
        /* advance init */
        res = mlx90614_advance_init();
        if (res)
        {
            mlx90614_advance_deinit();
            
            return 1;
        }
        
        /* exit sleep */
        res = mlx90614_advance_exit_sleep();
        if (res)
        {
            mlx90614_advance_deinit();
            
            return 1;
        }
        
        /* output */
        mlx90614_interface_debug_print("mlx90614: exit sleep.\n");
        
        /* advance deinit */
        (void)mlx90614_advance_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        mlx90614_interface_debug_print("Usage:\n");
        mlx90614_interface_debug_print("  mlx90614 (-i | --information)\n");
        mlx90614_interface_debug_print("  mlx90614 (-h | --help)\n");
        mlx90614_interface_debug_print("  mlx90614 (-p | --port)\n");
        mlx90614_interface_debug_print("  mlx90614 (-t reg | --test=reg)\n");
        mlx90614_interface_debug_print("  mlx90614 (-t read | --test=read) [--times=<num>]\n");
        mlx90614_interface_debug_print("  mlx90614 (-e read | --example=read) [--times=<num>]\n");
        mlx90614_interface_debug_print("  mlx90614 (-e advance-read | --example=advance-read) [--times=<num>]\n");
        mlx90614_interface_debug_print("  mlx90614 (-e advance-id | --example=advance-id)\n");
        mlx90614_interface_debug_print("  mlx90614 (-e advance-sleep | --example=advance-sleep)\n");
        mlx90614_interface_debug_print("  mlx90614 (-e advance-wake | --example=advance-wake)\n");
        mlx90614_interface_debug_print("\n");
        mlx90614_interface_debug_print("Options:\n");
        mlx90614_interface_debug_print("  -e <read | advance-read | advance-id | advance-sleep | advance-wake>, --example=\n");
        mlx90614_interface_debug_print("     <read | advance-read | advance-id | advance-sleep | advance-wake>\n");
        mlx90614_interface_debug_print("                       Run the driver example.\n");
        mlx90614_interface_debug_print("  -h, --help           Show the help.\n");
        mlx90614_interface_debug_print("  -i, --information    Show the chip information.\n");
        mlx90614_interface_debug_print("  -p, --port           Display the pin connections of the current board.\n");
        mlx90614_interface_debug_print("  -t <reg | read>, --test=<reg | read>\n");
        mlx90614_interface_debug_print("                       Run the driver test.\n");
        mlx90614_interface_debug_print("      --times=<num>    Set the running times.([default: 3])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        mlx90614_info_t info;
        
        /* print mlx90614 info */
        mlx90614_info(&info);
        mlx90614_interface_debug_print("mlx90614: chip is %s.\n", info.chip_name);
        mlx90614_interface_debug_print("mlx90614: manufacturer is %s.\n", info.manufacturer_name);
        mlx90614_interface_debug_print("mlx90614: interface is %s.\n", info.interface);
        mlx90614_interface_debug_print("mlx90614: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        mlx90614_interface_debug_print("mlx90614: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        mlx90614_interface_debug_print("mlx90614: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        mlx90614_interface_debug_print("mlx90614: max current is %0.2fmA.\n", info.max_current_ma);
        mlx90614_interface_debug_print("mlx90614: max temperature is %0.1fC.\n", info.temperature_max);
        mlx90614_interface_debug_print("mlx90614: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        mlx90614_interface_debug_print("mlx90614: SCL connected to GPIO3(BCM).\n");
        mlx90614_interface_debug_print("mlx90614: SDA connected to GPIO2(BCM).\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = mlx90614(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        mlx90614_interface_debug_print("mlx90614: run failed.\n");
    }
    else if (res == 5)
    {
        mlx90614_interface_debug_print("mlx90614: param is invalid.\n");
    }
    else
    {
        mlx90614_interface_debug_print("mlx90614: unknown status code.\n");
    }

    return 0;
}
