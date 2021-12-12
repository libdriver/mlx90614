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
 * @file      driver_mlx90614_advance.c
 * @brief     driver mlx90614 advance source file
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

#include "driver_mlx90614_advance.h"

static mlx90614_handle_t gs_handle;        /**< mlx90614 handle */

/**
 * @brief  advance example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t mlx90614_advance_init(void)
{
    volatile uint8_t res, addr, lsb, msb;
    volatile uint16_t reg;
    
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
    
    /* set default fir length */
    res = mlx90614_set_fir_length(&gs_handle, MLX90614_ADVANCE_DEFAULT_FIR_LENGTH);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default iir */
    res = mlx90614_set_iir(&gs_handle, MLX90614_ADVANCE_DEFAULT_IIR);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default mode */
    res = mlx90614_set_mode(&gs_handle, MLX90614_ADVANCE_DEFAULT_MODE);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set mode failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default ir sensor */
    res = mlx90614_set_ir_sensor(&gs_handle, MLX90614_ADVANCE_DEFAULT_IR_SENSOR);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set ir sensor failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default ks */
    res = mlx90614_set_ks(&gs_handle, MLX90614_ADVANCE_DEFAULT_KS);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set ks failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default kt2 */
    res = mlx90614_set_kt2(&gs_handle, MLX90614_ADVANCE_DEFAULT_KT2);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set kt2 failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default gain */
    res = mlx90614_set_gain(&gs_handle, MLX90614_ADVANCE_DEFAULT_GAIN);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default sensor test */
    res = mlx90614_set_sensor_test(&gs_handle, MLX90614_ADVANCE_DEFAULT_SENSOR_TEST);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set sensor test failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default repeat sensor test */
    res = mlx90614_set_repeat_sensor_test(&gs_handle, MLX90614_ADVANCE_DEFAULT_REPEAT_SENSOR_TEST);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set repeat sensor test failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    
    /* convert emissivity correction coefficient */
    res = mlx90614_emissivity_correction_coefficient_convert_to_register(&gs_handle, 
                                                                         MLX90614_ADVANCE_DEFAULT_EMISSIVITY_CORRECTION_COEFFICIENT,
                                                                        (uint16_t *)&reg);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: emissivity correction coefficient convert to register failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default emissivity correction coefficient */
    res = mlx90614_set_emissivity_correction_coefficient(&gs_handle, reg);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set emissivity correction coefficient failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief  advance example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t mlx90614_advance_deinit(void)
{
    volatile uint8_t res;
    
    /* deinit */
    res = mlx90614_deinit(&gs_handle);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: deinit failed.\n");
        
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  advance example enter sleep
 * @return status code
 *         - 0 success
 *         - 1 enter sleep failed
 * @note   none
 */
uint8_t mlx90614_advance_enter_sleep(void)
{
    volatile uint8_t res;
    
    /* enter sleep */
    res = mlx90614_enter_sleep_mode(&gs_handle);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: enter sleep failed.\n");
        
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  advance example exit sleep
 * @return status code
 *         - 0 success
 *         - 1 exit sleep failed
 * @note   none
 */
uint8_t mlx90614_advance_exit_sleep(void)
{
    volatile uint8_t res;
    
    /* exit sleep */
    res = mlx90614_exit_sleep_mode(&gs_handle);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: exit sleep failed.\n");
        
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example read
 * @param[in] *ambient points to a ambient buffer
 * @param[in] *object points to a object buffer
 * @return    status code
 *            - 0 success
 *            - 1 read failed
 * @note      none
 */
uint8_t mlx90614_advance_read(float *ambient, float *object)
{
    volatile uint8_t res;
    volatile uint16_t raw;
    
    /* read ambient */
    res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, ambient);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: read ambient failed.\n");
        
        return 1;
    }
    
    /* read object1 */
    res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, object);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: read object1 failed.\n");
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief     advance example get id
 * @param[in] *id points to a id buffer
 * @return    status code
 *            - 0 success
 *            - 1 get id failed
 * @note      none
 */
uint8_t mlx90614_advance_get_id(uint16_t id[4])
{
    volatile uint8_t res;
    
    /* read id */
    res = mlx90614_get_id(&gs_handle, id);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get id failed.\n");
        
        return 1;
    }
    else
    {
        return 0;
    }
}
