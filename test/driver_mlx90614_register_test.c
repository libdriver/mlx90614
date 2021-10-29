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
 * @file      driver_mlx90614_register_test.c
 * @brief     driver mlx90614 register test source file
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

#include "driver_mlx90614_register_test.h"
#include <stdlib.h>

static mlx90614_handle_t gs_handle;        /**< mlx90614 handle */

/**
 * @brief  register test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t mlx90614_register_test(void)
{
    volatile uint8_t res;
    volatile uint8_t addr, addr_check;
    volatile uint16_t value, value_check;
    volatile double s, s_check;
    volatile uint16_t id[4];
    volatile uint16_t flag;
    mlx90614_fir_length_t len;
    mlx90614_iir_t iir;
    mlx90614_mode_t mode;
    mlx90614_ir_sensor_t sensor;
    mlx90614_ks_t ks;
    mlx90614_kt2_t kt2;
    mlx90614_gain_t gain;
    mlx90614_bool_t enable;
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
    
    /* start register test */
    mlx90614_interface_debug_print("mlx90614: start register test.\n");
    
    /* mlx90614_set_addr/mlx90614_get_addr test */
    mlx90614_interface_debug_print("mlx90614: mlx90614_set_addr/mlx90614_get_addr test.\n");
    
    /* generate the addr */
    addr = rand() % 256;
    
    /* set the addr */
    res = mlx90614_set_addr(&gs_handle, addr);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set addr failed.\n");
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set addr 0x%02X.\n", addr);
    
    /* get the addr */
    res = mlx90614_get_addr(&gs_handle, (uint8_t *)&addr_check);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get addr failed.\n");
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check addr %s.\n", addr_check == addr ? "ok" : "error");
    
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
    
    /* mlx90614_set_fir_length/mlx90614_get_fir_length test */
    mlx90614_interface_debug_print("mlx90614: mlx90614_set_fir_length/mlx90614_get_fir_length test.\n");
    
    /* set fir length 8 */
    res = mlx90614_set_fir_length(&gs_handle, MLX90614_FIR_LENGTH_8);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set fir length 8.\n");
    res = mlx90614_get_fir_length(&gs_handle, &len);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check fir length %s.\n", len == MLX90614_FIR_LENGTH_8 ? "ok" : "error");
    
    /* set fir length 16 */
    res = mlx90614_set_fir_length(&gs_handle, MLX90614_FIR_LENGTH_16);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set fir length 16.\n");
    res = mlx90614_get_fir_length(&gs_handle, &len);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check fir length %s.\n", len == MLX90614_FIR_LENGTH_16 ? "ok" : "error");

    /* set fir length 32 */
    res = mlx90614_set_fir_length(&gs_handle, MLX90614_FIR_LENGTH_32);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set fir length 32.\n");
    res = mlx90614_get_fir_length(&gs_handle, &len);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check fir length %s.\n", len == MLX90614_FIR_LENGTH_32 ? "ok" : "error");
    
    /* set fir length 64 */
    res = mlx90614_set_fir_length(&gs_handle, MLX90614_FIR_LENGTH_64);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set fir length 64.\n");
    res = mlx90614_get_fir_length(&gs_handle, &len);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check fir length %s.\n", len == MLX90614_FIR_LENGTH_64 ? "ok" : "error");
    
    /* set fir length 128 */
    res = mlx90614_set_fir_length(&gs_handle, MLX90614_FIR_LENGTH_128);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set fir length 128.\n");
    res = mlx90614_get_fir_length(&gs_handle, &len);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check fir length %s.\n", len == MLX90614_FIR_LENGTH_128 ? "ok" : "error");
    
    /* set fir length 256 */
    res = mlx90614_set_fir_length(&gs_handle, MLX90614_FIR_LENGTH_256);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set fir length 256.\n");
    res = mlx90614_get_fir_length(&gs_handle, &len);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check fir length %s.\n", len == MLX90614_FIR_LENGTH_256 ? "ok" : "error");
    
    /* set fir length 512 */
    res = mlx90614_set_fir_length(&gs_handle, MLX90614_FIR_LENGTH_512);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set fir length 512.\n");
    res = mlx90614_get_fir_length(&gs_handle, &len);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check fir length %s.\n", len == MLX90614_FIR_LENGTH_512 ? "ok" : "error");
    
    /* set fir length 1024 */
    res = mlx90614_set_fir_length(&gs_handle, MLX90614_FIR_LENGTH_1024);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set fir length 1024.\n");
    res = mlx90614_get_fir_length(&gs_handle, &len);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get fir length failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check fir length %s.\n", len == MLX90614_FIR_LENGTH_1024 ? "ok" : "error");
    
    /* mlx90614_set_iir/mlx90614_get_iir test */
    mlx90614_interface_debug_print("mlx90614: mlx90614_set_iir/mlx90614_get_iir test.\n");
    
    /* set iir a1 0.5 b1 0.5 */
    res = mlx90614_set_iir(&gs_handle, MLX90614_IIR_A1_0P5_B1_0P5);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set iir a1 0.5 b1 0.5.\n");
    res = mlx90614_get_iir(&gs_handle, &iir);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check iir %s.\n", iir == MLX90614_IIR_A1_0P5_B1_0P5 ? "ok" : "error");
    
    /* set iir a1 0.25 b1 0.75 */
    res = mlx90614_set_iir(&gs_handle, MLX90614_IIR_A1_0P25_B1_0P75);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set iir a1 0.25 b1 0.75.\n");
    res = mlx90614_get_iir(&gs_handle, &iir);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check iir %s.\n", iir == MLX90614_IIR_A1_0P25_B1_0P75 ? "ok" : "error");
    
    /* set iir a1 0.166 b1 0.83 */
    res = mlx90614_set_iir(&gs_handle, MLX90614_IIR_A1_0P166_B1_0P83);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set iir a1 0.166 b1 0.83.\n");
    res = mlx90614_get_iir(&gs_handle, &iir);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check iir %s.\n", iir == MLX90614_IIR_A1_0P166_B1_0P83 ? "ok" : "error");
    
    /* set iir a1 0.125 b1 0.875 */
    res = mlx90614_set_iir(&gs_handle, MLX90614_IIR_A1_0P125_B1_0P875);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set iir a1 0.125 b1 0.875.\n");
    res = mlx90614_get_iir(&gs_handle, &iir);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check iir %s.\n", iir == MLX90614_IIR_A1_0P125_B1_0P875 ? "ok" : "error");
    
    /* set iir a1 1 b1 0 */
    res = mlx90614_set_iir(&gs_handle, MLX90614_IIR_A1_1_B1_0);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set iir a1 1 b1 0.\n");
    res = mlx90614_get_iir(&gs_handle, &iir);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check iir %s.\n", iir == MLX90614_IIR_A1_1_B1_0 ? "ok" : "error");
    
    /* set iir a1 0.8 b1 0.2 */
    res = mlx90614_set_iir(&gs_handle, MLX90614_IIR_A1_0P8_B1_0P2);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set iir a1 0.8 b1 0.2.\n");
    res = mlx90614_get_iir(&gs_handle, &iir);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check iir %s.\n", iir == MLX90614_IIR_A1_0P8_B1_0P2 ? "ok" : "error");
    
    /* set iir a1 0.666 b1 0.333 */
    res = mlx90614_set_iir(&gs_handle, MLX90614_IIR_A1_0P666_B1_0P333);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set iir a1 0.666 b1 0.333.\n");
    res = mlx90614_get_iir(&gs_handle, &iir);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check iir %s.\n", iir == MLX90614_IIR_A1_0P666_B1_0P333 ? "ok" : "error");
    
    /* set iir a1 0.571 b1 0.428 */
    res = mlx90614_set_iir(&gs_handle, MLX90614_IIR_A1_0P571_B1_0P428);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set iir a1 0.571 b1 0.428.\n");
    res = mlx90614_get_iir(&gs_handle, &iir);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get iir failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check iir %s.\n", iir == MLX90614_IIR_A1_0P571_B1_0P428 ? "ok" : "error");
    
    /* mlx90614_set_mode/mlx90614_get_mode test */
    mlx90614_interface_debug_print("mlx90614: mlx90614_set_mode/mlx90614_get_mode test.\n");
    
    /* set ta tobj1 mode */
    res = mlx90614_set_mode(&gs_handle, MLX90614_MODE_TA_TOBJ1);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set mode failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set ta tobj1 mode.\n");
    res = mlx90614_get_mode(&gs_handle, &mode);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get mode failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check mode %s.\n", mode == MLX90614_MODE_TA_TOBJ1 ? "ok" : "error");

    /* set ta tobj2 mode */
    res = mlx90614_set_mode(&gs_handle, MLX90614_MODE_TA_TOBJ2);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set mode failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set ta tobj2 mode.\n");
    res = mlx90614_get_mode(&gs_handle, &mode);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get mode failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check mode %s.\n", mode == MLX90614_MODE_TA_TOBJ2 ? "ok" : "error");
    
    /* set tobj2 mode */
    res = mlx90614_set_mode(&gs_handle, MLX90614_MODE_TOBJ2);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set mode failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set tobj2 mode.\n");
    res = mlx90614_get_mode(&gs_handle, &mode);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get mode failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check mode %s.\n", mode == MLX90614_MODE_TOBJ2 ? "ok" : "error");
    
    /* set tobj1 tobj2 mode */
    res = mlx90614_set_mode(&gs_handle, MLX90614_MODE_TOBJ1_TOBJ2);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set mode failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set tobj1 tobj2 mode.\n");
    res = mlx90614_get_mode(&gs_handle, &mode);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get mode failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check mode %s.\n", mode == MLX90614_MODE_TOBJ1_TOBJ2 ? "ok" : "error");
    
    /* mlx90614_set_ir_sensor/mlx90614_get_ir_sensor test */
    mlx90614_interface_debug_print("mlx90614: mlx90614_set_ir_sensor/mlx90614_get_ir_sensor test.\n");
    
    /* set ir sensor single */
    res = mlx90614_set_ir_sensor(&gs_handle, MLX90614_IR_SENSOR_SINGLE);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set ir sensor failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set ir sensor single.\n");
    res = mlx90614_get_ir_sensor(&gs_handle, &sensor);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get ir sensor failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check ir sensor %s.\n", sensor == MLX90614_IR_SENSOR_SINGLE ? "ok" : "error");
    
    /* set ir sensor dual */
    res = mlx90614_set_ir_sensor(&gs_handle, MLX90614_IR_SENSOR_DUAL);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set ir sensor failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set ir sensor dual.\n");
    res = mlx90614_get_ir_sensor(&gs_handle, &sensor);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get ir sensor failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check ir sensor %s.\n", sensor == MLX90614_IR_SENSOR_DUAL ? "ok" : "error");
    
    /* mlx90614_set_ks/mlx90614_get_ks test */
    mlx90614_interface_debug_print("mlx90614: mlx90614_set_ks/mlx90614_get_ks test.\n");
    
    /* set ks positive */
    res = mlx90614_set_ks(&gs_handle, MLX90614_KS_POSITIVE);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set ks failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set ks positive.\n");
    res = mlx90614_get_ks(&gs_handle, &ks);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get ks failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check ks %s.\n", ks == MLX90614_KS_POSITIVE ? "ok" : "error");
    
    /* set ks negative */
    res = mlx90614_set_ks(&gs_handle, MLX90614_KS_NEGATIVE);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set ks failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set ks negative.\n");
    res = mlx90614_get_ks(&gs_handle, &ks);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get ks failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check ks %s.\n", ks == MLX90614_KS_NEGATIVE ? "ok" : "error");
    
    /* mlx90614_set_kt2/mlx90614_get_kt2 test */
    mlx90614_interface_debug_print("mlx90614: mlx90614_set_kt2/mlx90614_get_kt2 test.\n");
    
    /* set kt2 positive */
    res = mlx90614_set_kt2(&gs_handle, MLX90614_KT2_POSITIVE);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set kt2 failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set kt2 positive.\n");
    res = mlx90614_get_kt2(&gs_handle, &kt2);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get kt2 failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check kt2 %s.\n", kt2 == MLX90614_KT2_POSITIVE ? "ok" : "error");
    
    /* set kt2 negative */
    res = mlx90614_set_kt2(&gs_handle, MLX90614_KT2_NEGATIVE);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set kt2 failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set kt2 negative.\n");
    res = mlx90614_get_kt2(&gs_handle, &kt2);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get kt2 failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check kt2 %s.\n", kt2 == MLX90614_KT2_NEGATIVE ? "ok" : "error");
    
    /* mlx90614_set_gain/mlx90614_get_gain test */
    mlx90614_interface_debug_print("mlx90614: mlx90614_set_gain/mlx90614_get_gain test.\n");
    
    /* set gain 1 */
    res = mlx90614_set_gain(&gs_handle, MLX90614_GAIN_1);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set gain 1.\n");
    res = mlx90614_get_gain(&gs_handle, &gain);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check gain %s.\n", gain == MLX90614_GAIN_1 ? "ok" : "error");
    
    /* set gain 3 */
    res = mlx90614_set_gain(&gs_handle, MLX90614_GAIN_3);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set gain 3.\n");
    res = mlx90614_get_gain(&gs_handle, &gain);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check gain %s.\n", gain == MLX90614_GAIN_3 ? "ok" : "error");
    
    /* set gain 6 */
    res = mlx90614_set_gain(&gs_handle, MLX90614_GAIN_6);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set gain 6.\n");
    res = mlx90614_get_gain(&gs_handle, &gain);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check gain %s.\n", gain == MLX90614_GAIN_6 ? "ok" : "error");
    
    /* set gain 12.5 */
    res = mlx90614_set_gain(&gs_handle, MLX90614_GAIN_12P5);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set gain 12.5.\n");
    res = mlx90614_get_gain(&gs_handle, &gain);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check gain %s.\n", gain == MLX90614_GAIN_12P5 ? "ok" : "error");
    
    /* set gain 25 */
    res = mlx90614_set_gain(&gs_handle, MLX90614_GAIN_25);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set gain 25.\n");
    res = mlx90614_get_gain(&gs_handle, &gain);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check gain %s.\n", gain == MLX90614_GAIN_25 ? "ok" : "error");
    
    /* set gain 50 */
    res = mlx90614_set_gain(&gs_handle, MLX90614_GAIN_50);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set gain 50.\n");
    res = mlx90614_get_gain(&gs_handle, &gain);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check gain %s.\n", gain == MLX90614_GAIN_50 ? "ok" : "error");
    
    /* set gain 100 */
    res = mlx90614_set_gain(&gs_handle, MLX90614_GAIN_100);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set gain 100.\n");
    res = mlx90614_get_gain(&gs_handle, &gain);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get gain failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check gain %s.\n", gain == MLX90614_GAIN_100 ? "ok" : "error");
    
    /* mlx90614_set_sensor_test/mlx90614_get_sensor_test test */
    mlx90614_interface_debug_print("mlx90614: mlx90614_set_sensor_test/mlx90614_get_sensor_test test.\n");
    
    /* enable sensor test */
    res = mlx90614_set_sensor_test(&gs_handle, MLX90614_BOOL_TRUE);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set sensor test failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: enable sensor test.\n");
    res = mlx90614_get_sensor_test(&gs_handle, &enable);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get sensor test failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check sensor test %s.\n", enable == MLX90614_BOOL_TRUE ? "ok" : "error");
    
    /* disable sensor test */
    res = mlx90614_set_sensor_test(&gs_handle, MLX90614_BOOL_FALSE);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set sensor test failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: disable sensor test.\n");
    res = mlx90614_get_sensor_test(&gs_handle, &enable);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get sensor test failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check sensor test %s.\n", enable == MLX90614_BOOL_FALSE ? "ok" : "error");
    
    /* mlx90614_set_repeat_sensor_test/mlx90614_get_repeat_sensor_test test */
    mlx90614_interface_debug_print("mlx90614: mlx90614_set_repeat_sensor_test/mlx90614_get_repeat_sensor_test test.\n");
    
    /* enable repeat sensor test */
    res = mlx90614_set_repeat_sensor_test(&gs_handle, MLX90614_BOOL_TRUE);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set repeat sensor test failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: enable repeat sensor test.\n");
    res = mlx90614_get_repeat_sensor_test(&gs_handle, &enable);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get repeat sensor test failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check repeat sensor test %s.\n", enable == MLX90614_BOOL_TRUE ? "ok" : "error");
    
    /* disable repeat sensor test */
    res = mlx90614_set_repeat_sensor_test(&gs_handle, MLX90614_BOOL_FALSE);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set repeat sensor test failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: disable repeat sensor test.\n");
    res = mlx90614_get_repeat_sensor_test(&gs_handle, &enable);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get repeat sensor test failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check repeat sensor test %s.\n", enable == MLX90614_BOOL_FALSE ? "ok" : "error");
    
    /* mlx90614_set_emissivity_correction_coefficient/mlx90614_get_emissivity_correction_coefficient test */
    mlx90614_interface_debug_print("mlx90614: mlx90614_set_emissivity_correction_coefficient/mlx90614_get_emissivity_correction_coefficient test.\n");
    
    value = rand() % 65536;
    res = mlx90614_set_emissivity_correction_coefficient(&gs_handle, value);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: set emissivity correction coefficient failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: set emissivity correction coefficient %d.\n", value);
    res = mlx90614_get_emissivity_correction_coefficient(&gs_handle, (uint16_t *)&value_check);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get emissivity correction coefficient failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: check emissivity correction coefficient %s.\n", value_check == value ? "ok" : "error");
    
    /* mlx90614_emissivity_correction_coefficient_convert_to_register/mlx90614_emissivity_correction_coefficient_convert_to_data test */
    mlx90614_interface_debug_print("mlx90614: mlx90614_emissivity_correction_coefficient_convert_to_register/mlx90614_emissivity_correction_coefficient_convert_to_data test.\n");
    
    s = (double)(rand() % 65536) / 65536.0;
    res = mlx90614_emissivity_correction_coefficient_convert_to_register(&gs_handle, s, (uint16_t *)&value);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: emissivity correction coefficient convert to register failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: %f convert to register 0x%02X.\n", s, value);
    res = mlx90614_emissivity_correction_coefficient_convert_to_data(&gs_handle, value, (double *)&s_check);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: emissivity correction coefficient convert to data failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: 0x%02X convert to data %f.\n", value, s_check);
    
    /* mlx90614_get_id test */
    mlx90614_interface_debug_print("mlx90614: mlx90614_get_id test.\n");
    
    res = mlx90614_get_id(&gs_handle, (uint16_t *)id);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get id failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: get id is 0x%02X 0x%02X 0x%02X 0x%02X.\n", id[0], id[1], id[2], id[3]);
    
    /* mlx90614_enter_sleep_mode test */
    mlx90614_interface_debug_print("mlx90614: mlx90614_enter_sleep_mode test.\n");
    
    res = mlx90614_enter_sleep_mode(&gs_handle);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: enter sleep mode failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: enter sleep mode.\n");
    
    /* mlx90614_exit_sleep_mode test */
    mlx90614_interface_debug_print("mlx90614: mlx90614_exit_sleep_mode test.\n");
    
    res = mlx90614_exit_sleep_mode(&gs_handle);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: exit sleep mode failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: exit sleep mode.\n");
    
    /* mlx90614_get_flag test */
    mlx90614_interface_debug_print("mlx90614: mlx90614_get_flag test.\n");
    
    res = mlx90614_get_flag(&gs_handle, (uint16_t *)&flag);
    if (res)
    {
        mlx90614_interface_debug_print("mlx90614: get flag failed.\n");
        mlx90614_deinit(&gs_handle);
        
        return 1;
    }
    mlx90614_interface_debug_print("mlx90614: get flag 0x%04X.\n", flag);
    
    /* finish register test */
    mlx90614_interface_debug_print("mlx90614: finish register test.\n");
    mlx90614_deinit(&gs_handle);
    
    return 0;
}
