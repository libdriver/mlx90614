/**
 * Copyright (C) LibDriver 2015-2021 All rights reserved
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
 * @file      driver_mlx90614_read_test.c
 * @brief     driver mlx90614 read test source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-10-03
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/10/03  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_mlx90614_read_test.h"
#include <stdlib.h>

static mlx90614_handle_t gs_handle;        /**< mlx90614 handle */

/**
 * @brief     read test
 * @param[in] times is the test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t mlx90614_read_test(uint32_t times)
{
    volatile uint8_t res;
    volatile float ambient;
    volatile float object;
    volatile float object2;
    volatile uint16_t raw;
    volatile uint16_t value;
    volatile uint16_t raw2;
    volatile uint32_t i;
    mlx90614_info_t info;
    
    /* link interface function */
    DRIVER_MLX90614_LINK_INIT(&gs_handle, mlx90614_handle_t);
    DRIVER_MLX90614_LINK_IIC_INIT(&gs_handle, mlx90614_interface_iic_init);
    DRIVER_MLX90614_LINK_IIC_DEINIT(&gs_handle, mlx90614_interface_iic_deinit);
    DRIVER_MLX90614_LINK_IIC_READ(&gs_handle, mlx90614_interface_iic_read);
    DRIVER_MLX90614_LINK_IIC_WRITE(&gs_handle, mlx90614_interface_iic_write);
    DRIVER_MLX90614_LINK_SCL_WRITE(&gs_handle, mlx90614_interface_scl_write);
    DRIVER_MLX90614_LINK_SDA_WRITE(&gs_handle, mlx90614_interface_sda_write);
    DRIVER_MLX90614_LINK_DELAY_MS(&gs_handle, mlx90614_interface_delay_ms);
    DRIVER_MLX90614_LINK_DEBUG_PRINT(&gs_handle, mlx90614_interface_debug_print);
    
    /* get information */
    res = mlx90614_info(&info);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        mlx90614_interface_debug_print("mlx90614: chip is %s.\n", info.chip_name);
        mlx90614_interface_debug_print("mlx90614: manufacturer is %s.\n", info.manufacturer_name);
        mlx90614_interface_debug_print("mlx90614: interface is %s.\n", info.interface);
        mlx90614_interface_debug_print("mlx90614: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        mlx90614_interface_debug_print("mlx90614: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        mlx90614_interface_debug_print("mlx90614: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        mlx90614_interface_debug_print("mlx90614: max current is %0.2fmA.\n", info.max_current_ma);
        mlx90614_interface_debug_print("mlx90614: max temperature is %0.1fC.\n", info.temperature_max);
        mlx90614_interface_debug_print("mlx90614: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start read test */
    mlx90614_interface_debug_print("mlx90614: start read test.\n");
    
    /* set address */
    res = mlx90614_set_addr(&gs_handle, MLX90614_ADDRESS_DEFAULT);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set addr failed.\n");
        
        return 1;
    }
    
    /* mlx90614 init */
    res = mlx90614_init(&gs_handle);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: init failed.\n");
        
        return 1;
    }
    
    /* pwm to smbus */
    res = mlx90614_pwm_to_smbus(&gs_handle);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: pwm to smbus failed.\n");
        
        return 1;
    }
    
    /* exit sleep mode */
    res = mlx90614_exit_sleep_mode(&gs_handle);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: exit sleep mode failed.\n");
        
        return 1;
    }
    
    /* set mode */
    res = mlx90614_set_mode(&gs_handle, MLX90614_MODE_TA_TOBJ1);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set mode failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }

    /* set fir length 8 */
    res = mlx90614_set_fir_length(&gs_handle, MLX90614_FIR_LENGTH_8);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set fir length 8.\n");
        
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set fir length 16 */
    res = mlx90614_set_fir_length(&gs_handle, MLX90614_FIR_LENGTH_16);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set fir length 16.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set fir length 32 */
    res = mlx90614_set_fir_length(&gs_handle, MLX90614_FIR_LENGTH_32);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set fir length 32.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set fir length 64 */
    res = mlx90614_set_fir_length(&gs_handle, MLX90614_FIR_LENGTH_64);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set fir length 64.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set fir length 128 */
    res = mlx90614_set_fir_length(&gs_handle, MLX90614_FIR_LENGTH_128);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set fir length 128.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set fir length 256 */
    res = mlx90614_set_fir_length(&gs_handle, MLX90614_FIR_LENGTH_256);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set fir length 256.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set fir length 512 */
    res = mlx90614_set_fir_length(&gs_handle, MLX90614_FIR_LENGTH_512);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set fir length 512.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set fir length 1024 */
    res = mlx90614_set_fir_length(&gs_handle, MLX90614_FIR_LENGTH_1024);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set fir length 1024.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set iir a1 0.5 b1 0.5 */
    res = mlx90614_set_iir(&gs_handle, MLX90614_IIR_A1_0P5_B1_0P5);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set iir a1 0.5 b1 0.5.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set iir a1 0.25 b1 0.75 */
    res = mlx90614_set_iir(&gs_handle, MLX90614_IIR_A1_0P25_B1_0P75);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set iir a1 0.25 b1 0.75.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set iir a1 0.166 b1 0.83 */
    res = mlx90614_set_iir(&gs_handle, MLX90614_IIR_A1_0P166_B1_0P83);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set iir a1 0.166 b1 0.83.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set iir a1 0.125 b1 0.875 */
    res = mlx90614_set_iir(&gs_handle, MLX90614_IIR_A1_0P125_B1_0P875);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set iir a1 0.125 b1 0.875.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set iir a1 1 b1 0 */
    res = mlx90614_set_iir(&gs_handle, MLX90614_IIR_A1_1_B1_0);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set iir a1 1 b1 0.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set iir a1 0.8 b1 0.2 */
    res = mlx90614_set_iir(&gs_handle, MLX90614_IIR_A1_0P8_B1_0P2);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set iir a1 0.8 b1 0.2.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set iir a1 0.666 b1 0.333 */
    res = mlx90614_set_iir(&gs_handle, MLX90614_IIR_A1_0P666_B1_0P333);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set iir a1 0.666 b1 0.333.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set iir a1 0.571 b1 0.428 */
    res = mlx90614_set_iir(&gs_handle, MLX90614_IIR_A1_0P571_B1_0P428);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set iir a1 0.571 b1 0.428.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set ir sensor single */
    res = mlx90614_set_ir_sensor(&gs_handle, MLX90614_IR_SENSOR_SINGLE);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set ir sensor failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set ir sensor single.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set ir sensor dual */
    res = mlx90614_set_ir_sensor(&gs_handle, MLX90614_IR_SENSOR_DUAL);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set ir sensor failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set ir sensor dual.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set ks positive */
    res = mlx90614_set_ks(&gs_handle, MLX90614_KS_POSITIVE);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set ks failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set ks positive.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set ks negative */
    res = mlx90614_set_ks(&gs_handle, MLX90614_KS_NEGATIVE);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set ks failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set ks negative.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set kt2 positive */
    res = mlx90614_set_kt2(&gs_handle, MLX90614_KT2_POSITIVE);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set kt2 failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set kt2 positive.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set kt2 negative */
    res = mlx90614_set_kt2(&gs_handle, MLX90614_KT2_NEGATIVE);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set kt2 failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set kt2 negative.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set gain 1 */
    res = mlx90614_set_gain(&gs_handle, MLX90614_GAIN_1);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set gain 1.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set gain 3 */
    res = mlx90614_set_gain(&gs_handle, MLX90614_GAIN_3);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set gain 3.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set gain 6 */
    res = mlx90614_set_gain(&gs_handle, MLX90614_GAIN_6);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set gain 6.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set gain 12.5 */
    res = mlx90614_set_gain(&gs_handle, MLX90614_GAIN_12P5);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set gain 12.5.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set gain 25 */
    res = mlx90614_set_gain(&gs_handle, MLX90614_GAIN_25);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set gain 25.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set gain 50 */
    res = mlx90614_set_gain(&gs_handle, MLX90614_GAIN_50);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set gain 50.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set gain 100 */
    res = mlx90614_set_gain(&gs_handle, MLX90614_GAIN_100);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set gain 100.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    value = rand() % 65536;
    res = mlx90614_set_emissivity_correction_coefficient(&gs_handle, value);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set emissivity correction coefficient failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set emissivity correction coefficient %d.\n", value);
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set ta tobj1 mode */
    res = mlx90614_set_mode(&gs_handle, MLX90614_MODE_TA_TOBJ1);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set mode failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set ta tobj1 mode.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set ta tobj2 mode */
    res = mlx90614_set_mode(&gs_handle, MLX90614_MODE_TA_TOBJ2);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set mode failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set ta tobj2 mode.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read ambient */
        res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, (float *)&ambient);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
            
            return 1;
        }
        
        /* read object2 */
        res = mlx90614_read_object2(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object2 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: ambient is %0.2fC object is %0.2fC.\n", ambient, object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set tobj2 mode */
    res = mlx90614_set_mode(&gs_handle, MLX90614_MODE_TOBJ2);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set mode failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set tobj2 mode.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read object2 */
        res = mlx90614_read_object2(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object2 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: object is %0.2fC.\n", object);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* set tobj1 tobj2 mode */
    res = mlx90614_set_mode(&gs_handle, MLX90614_MODE_TOBJ1_TOBJ2);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set mode failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set tobj1 tobj2 mode.\n");
    
    /* delay 3000 ms */
    mlx90614_interface_delay_ms(3000);
    
    /* read data */
    for (i = 0; i < times; i++)
    {
        volatile uint16_t channel_1;
        volatile uint16_t channel_2;
        
        /* read object1 */
        res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, (float *)&object);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
            
            return 1;
        }
        
        /* read object2 */
        res = mlx90614_read_object2(&gs_handle, (uint16_t *)&raw2, (float *)&object2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read object2 failed.\n");
            
            return 1;
        }
        
        /* read raw ir channel */
        res = mlx90614_read_raw_ir_channel(&gs_handle, (uint16_t *)&channel_1, (uint16_t *)&channel_2);
        if (res)
        {
            mlx90614_interface_debug_print("mlx90614: read raw ir channel failed.\n");
            
            return 1;
        }
        
        /* print the data */
        mlx90614_interface_debug_print("mlx90614: object1 is %0.2fC object2 is %0.2fC.\n", object, object2);
        mlx90614_interface_debug_print("mlx90614: channel1 raw is 0x%04X channel2 raw is 0x%04X.\n", channel_1, channel_2);
        
        /* delay 1000 ms */
        mlx90614_interface_delay_ms(1000);
    }
    
    /* finish read test */
    mlx90614_interface_debug_print("mlx90614: finish read test.\n");
    mlx90614_deinit(&gs_handle);
    
    return 0;
}
