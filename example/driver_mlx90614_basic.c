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
 * @file      driver_mlx90614_basic.c
 * @brief     driver mlx90614 basic source file
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

#include "driver_mlx90614_basic.h"

static mlx90614_handle_t gs_handle;        /**< mlx90614 handle */

/**
 * @brief  basic example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t mlx90614_basic_init(void)
{
    uint8_t res;
    
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
    if (res != 0)
    {
        mlx90614_interface_debug_print("mlx90614: set addr failed.\n");
        
        return 1;
    }
    
    /* mlx90614 init */
    res = mlx90614_init(&gs_handle);
    if (res != 0)
    {
        mlx90614_interface_debug_print("mlx90614: init failed.\n");
        
        return 1;
    }
    
    /* pwm to smbus */
    res = mlx90614_pwm_to_smbus(&gs_handle);
    if (res != 0)
    {
        mlx90614_interface_debug_print("mlx90614: pwm to smbus failed.\n");
        (void)mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    
    /* exit sleep mode */
    res = mlx90614_exit_sleep_mode(&gs_handle);
    if (res != 0)
    {
        mlx90614_interface_debug_print("mlx90614: exit sleep mode failed.\n");
        (void)mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t mlx90614_basic_deinit(void)
{
    uint8_t res;
    
    /* deinit */
    res = mlx90614_deinit(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     basic example read
 * @param[in] *ambient points to a ambient buffer
 * @param[in] *object points to a object buffer
 * @return    status code
 *            - 0 success
 *            - 1 read failed
 * @note      none
 */
uint8_t mlx90614_basic_read(float *ambient, float *object)
{
    uint8_t res;
    uint16_t raw;
    
    /* read ambient */
    res = mlx90614_read_ambient(&gs_handle, (uint16_t *)&raw, ambient);
    if (res != 0)
    {
        return 1;
    }
    
    /* read object1 */
    res = mlx90614_read_object1(&gs_handle, (uint16_t *)&raw, object);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}
