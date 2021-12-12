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
#include "iic.h"
#include <stdlib.h>

/**
 * @brief     mlx90614 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t mlx90614(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
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
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            mlx90614_interface_debug_print("mlx90614: SCL connected to GPIO3(BCM).\n");
            mlx90614_interface_debug_print("mlx90614: SDA connected to GPIO2(BCM).\n");
            
            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show mlx90614 help */
            
            help:
            
            mlx90614_interface_debug_print("mlx90614 -i\n\tshow mlx90614 chip and driver information.\n");
            mlx90614_interface_debug_print("mlx90614 -h\n\tshow mlx90614 help.\n");
            mlx90614_interface_debug_print("mlx90614 -p\n\tshow mlx90614 pin connections of the current board.\n");
            mlx90614_interface_debug_print("mlx90614 -t reg\n\trun mlx90614 register test.\n");
            mlx90614_interface_debug_print("mlx90614 -t read <times>\n\trun mlx90614 read test.times means test times.\n");
            mlx90614_interface_debug_print("mlx90614 -c basic read <times>\n\trun mlx90614 basic read function.times means read times.\n");
            mlx90614_interface_debug_print("mlx90614 -c advance read <times>\n\trun mlx90614 advance read function.times means read times.\n");
            mlx90614_interface_debug_print("mlx90614 -c advance id\n\trun mlx90614 advance read id function.\n");
            mlx90614_interface_debug_print("mlx90614 -c advance sleep\n\trun mlx90614 advance sleep function.\n");
            mlx90614_interface_debug_print("mlx90614 -c advance wake\n\trun mlx90614 advance wake up function.\n");
            
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 3)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
            /* reg test */
            if (strcmp("reg", argv[2]) == 0)
            {
                if (mlx90614_register_test())
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 4)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
            /* read test */
            if (strcmp("read", argv[2]) == 0)
            {
                if (mlx90614_read_test(atoi(argv[3])))
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* run function */
        else if (strcmp("-c", argv[1]) == 0)
        {
            /* advance function */
            if (strcmp("advance", argv[2]) == 0)
            {
                if (strcmp("id", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    volatile uint16_t id[4];
                    
                    /* init */
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
                    
                    mlx90614_interface_debug_print("mlx90614: get id is 0x%02X 0x%02X 0x%02X 0x%02X.\n", id[0], id[1], id[2], id[3]);
                    
                    return mlx90614_advance_deinit();
                }
                else if (strcmp("sleep", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    volatile uint16_t id[4];
                    
                    /* init */
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
                    
                    mlx90614_interface_debug_print("mlx90614: enter sleep.\n");
                    
                    return mlx90614_advance_deinit();
                }
                else if (strcmp("wake", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    volatile uint16_t id[4];
                    
                    /* init */
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
                    
                    mlx90614_interface_debug_print("mlx90614: exit sleep.\n");
                    
                    return mlx90614_advance_deinit();
                }
                /* param is invalid */
                else
                {
                    return 5;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 5)
    {
        /* run function */
        if (strcmp("-c", argv[1]) == 0)
        {
            /*  basic function */
            if (strcmp("basic", argv[2]) == 0)
            {
                if (strcmp("read", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    volatile uint32_t i, times;
                    volatile float ambient;
                    volatile float object;
                    
                    /* init */
                    res = mlx90614_basic_init();
                    if (res)
                    {
                        return 1;
                    }
                    
                    times = atoi(argv[4]);
                    for (i = 0; i < times; i++)
                    {
                        res = mlx90614_basic_read((float *)&ambient, (float *)&object);
                        if (res)
                        {
                            mlx90614_basic_deinit();
                            
                            return 1;
                        }

                        /* print the data */
                        mlx90614_interface_debug_print("mlx90614: %d/%d.\n", i + 1, times);
                        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
                        
                        /* delay 1000 ms */
                        mlx90614_interface_delay_ms(1000);
                    }
                    
                    return mlx90614_basic_deinit();
                }
                else
                {
                    return 5;
                }
            }
            /*  advance function */
            else if (strcmp("advance", argv[2]) == 0)
            {
                if (strcmp("read", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    volatile uint32_t i, times;
                    volatile float ambient;
                    volatile float object;
                    
                    /* init */
                    res = mlx90614_advance_init();
                    if (res)
                    {
                        return 1;
                    }
                        
                    /* delay 2000 ms */
                    mlx90614_interface_delay_ms(2000);
                    
                    times = atoi(argv[4]);
                    for (i = 0; i < times; i++)
                    {
                        res = mlx90614_advance_read((float *)&ambient, (float *)&object);
                        if (res)
                        {
                            mlx90614_advance_deinit();
                            
                            return 1;
                        }

                        /* print the data */
                        mlx90614_interface_debug_print("mlx90614: %d/%d.\n", i + 1, times);
                        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
                        
                        /* delay 1000 ms */
                        mlx90614_interface_delay_ms(1000);
                    }
                    
                    return mlx90614_advance_deinit();
                }
                else
                {
                    return 5;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    /* param is invalid */
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
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
        mlx90614_interface_debug_print("mlx90614: unknow status code.\n");
    }

    return 0;
}
