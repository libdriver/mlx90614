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
 * @file      driver_mlx90614.h
 * @brief     driver mlx90614 header file
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

#ifndef _DRIVER_MLX90614_H_
#define _DRIVER_MLX90614_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup mlx90614_driver mlx90614 driver function
 * @brief    mlx90614 driver modules
 * @{
 */

/**
 * @addtogroup mlx90614_basic_driver
 * @{
 */

/**
 * @brief mlx90614 address enumeration definition
 */
typedef enum
{
    MLX90614_ADDRESS_DEFAULT = 0xB4,        /**< default address */
} mlx90614_address_t;

/**
 * @}
 */

/**
 * @addtogroup mlx90614_advance_driver
 * @{
 */

/**
 * @brief mlx90614 flag enumeration definition
 */
typedef enum
{
    MLX90614_FLAG_EE_BUSY = (1 << 7),        /**< the previous write/erase eeprom access is still in progress, high active */
    MLX90614_FLAG_EE_DEAD = (1 << 5),        /**< eeprom double error has occurred, high active */
    MLX90614_FLAG_INIT    = (1 << 4),        /**< por initialization routine is still ongoing, low active */
} mlx90614_flag_t;

/**
 * @brief mlx90614 mode enumeration definition
 */
typedef enum
{
    MLX90614_MODE_TA_TOBJ1    = 0x00,        /**< ta and tobj1 mode */
    MLX90614_MODE_TA_TOBJ2    = 0x01,        /**< ta and tobj2 mode */
    MLX90614_MODE_TOBJ2       = 0x02,        /**< tobj2 mode */
    MLX90614_MODE_TOBJ1_TOBJ2 = 0x03,        /**< tobj1 and tobj2 mode */
} mlx90614_mode_t;

/**
 * @brief mlx90614 gain enumeration definition
 */
typedef enum
{
    MLX90614_GAIN_1    = 0x00,        /**< gain 1 */
    MLX90614_GAIN_3    = 0x01,        /**< gain 3 */
    MLX90614_GAIN_6    = 0x02,        /**< gain 6 */
    MLX90614_GAIN_12P5 = 0x03,        /**< gain 12.5 */
    MLX90614_GAIN_25   = 0x04,        /**< gain 25 */
    MLX90614_GAIN_50   = 0x05,        /**< gain 50 */
    MLX90614_GAIN_100  = 0x06,        /**< gain 100 */
} mlx90614_gain_t;

/**
 * @brief mlx90614 bool enumeration definition
 */
typedef enum
{
    MLX90614_BOOL_FALSE = 0x00,        /**< false */
    MLX90614_BOOL_TRUE  = 0x01,        /**< true */
} mlx90614_bool_t;

/**
 * @brief mlx90614 kt2 enumeration definition
 */
typedef enum
{
    MLX90614_KT2_POSITIVE = 0x00,        /**< positive */
    MLX90614_KT2_NEGATIVE = 0x01,        /**< negative */
} mlx90614_kt2_t;

/**
 * @brief mlx90614 ks enumeration definition
 */
typedef enum
{
    MLX90614_KS_POSITIVE = 0x00,
    MLX90614_KS_NEGATIVE = 0x01,
} mlx90614_ks_t;

/**
 * @brief mlx90614 ir sensor enumeration definition
 */
typedef enum
{
    MLX90614_IR_SENSOR_SINGLE = 0x00,        /**< single ir sensor */
    MLX90614_IR_SENSOR_DUAL   = 0x01,        /**< dual ir sensor */
} mlx90614_ir_sensor_t;

/**
 * @brief mlx90614 fir length enumeration definition
 */
typedef enum
{
    MLX90614_FIR_LENGTH_8    = 0x00,        /**< length 8 */
    MLX90614_FIR_LENGTH_16   = 0x01,        /**< length 16 */
    MLX90614_FIR_LENGTH_32   = 0x02,        /**< length 32 */
    MLX90614_FIR_LENGTH_64   = 0x03,        /**< length 64 */
    MLX90614_FIR_LENGTH_128  = 0x04,        /**< length 128 */
    MLX90614_FIR_LENGTH_256  = 0x05,        /**< length 256 */
    MLX90614_FIR_LENGTH_512  = 0x06,        /**< length 512 */
    MLX90614_FIR_LENGTH_1024 = 0x07,        /**< length 1024 */
} mlx90614_fir_length_t;

/**
 * @brief mlx90614 iir enumeration definition
 */
typedef enum
{
    MLX90614_IIR_A1_0P5_B1_0P5     = 0x00,        /**< a1 = 0.5 b1 = 0.5 */
    MLX90614_IIR_A1_0P25_B1_0P75   = 0x01,        /**< a1 = 0.25 b1 = 0.75 */
    MLX90614_IIR_A1_0P166_B1_0P83  = 0x02,        /**< a1 = 0.166 b1 = 0.83 */
    MLX90614_IIR_A1_0P125_B1_0P875 = 0x03,        /**< a1 = 0.125 b1 = 0.875 */
    MLX90614_IIR_A1_1_B1_0         = 0x04,        /**< a1 = 1 b1 = 0 */
    MLX90614_IIR_A1_0P8_B1_0P2     = 0x05,        /**< a1 = 0.8 b1 = 0.2 */
    MLX90614_IIR_A1_0P666_B1_0P333 = 0x06,        /**< a1 = 0.666 b1 = 0.333 */
    MLX90614_IIR_A1_0P571_B1_0P428 = 0x07,        /**< a1 = 0.571 b1 = 0.428 */
} mlx90614_iir_t;

/**
 * @}
 */

/**
 * @addtogroup mlx90614_basic_driver
 * @{
 */

/**
 * @brief mlx90614 handle structure definition
 */
typedef struct mlx90614_handle_s
{
    uint8_t iic_addr;                                                                   /**< iic device address */
    uint8_t (*iic_init)(void);                                                          /**< point to a iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to a iic_deinit function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to a iic_read function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to a iic_write function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    uint16_t (*debug_print)(char *fmt, ...);                                            /**< point to a debug_print function address */
    uint8_t (*scl_write)(uint8_t v);                                                    /**< point to a scl_write address */
    uint8_t (*sda_write)(uint8_t v);                                                    /**< point to a sda_write address */
    uint8_t inited;                                                                     /**< inited flag */
} mlx90614_handle_t;

/**
 * @brief mlx90614 information structure definition
 */
typedef struct mlx90614_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} mlx90614_info_t;

/**
 * @}
 */

/**
 * @defgroup mlx90614_link_driver mlx90614 link driver function
 * @brief    mlx90614 link driver modules
 * @ingroup  mlx90614_driver
 * @{
 */

/**
 * @brief     initialize mlx90614_handle_t structure
 * @param[in] HANDLE points to a mlx90614 handle structure
 * @param[in] STRUCTURE is mlx90614_handle_t
 * @note      none
 */
#define DRIVER_MLX90614_LINK_INIT(HANDLE, STRUCTURE)         memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE points to a mlx90614 handle structure
 * @param[in] FUC points to a iic_init function address
 * @note      none
 */
#define DRIVER_MLX90614_LINK_IIC_INIT(HANDLE, FUC)           (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE points to a mlx90614 handle structure
 * @param[in] FUC points to a iic_deinit function address
 * @note      none
 */
#define DRIVER_MLX90614_LINK_IIC_DEINIT(HANDLE, FUC)         (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE points to a mlx90614 handle structure
 * @param[in] FUC points to a iic_read function address
 * @note      none
 */
#define DRIVER_MLX90614_LINK_IIC_READ(HANDLE, FUC)           (HANDLE)->iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE points to a mlx90614 handle structure
 * @param[in] FUC points to a iic_write function address
 * @note      none
 */
#define DRIVER_MLX90614_LINK_IIC_WRITE(HANDLE, FUC)          (HANDLE)->iic_write = FUC

/**
 * @brief     link scl_write function
 * @param[in] HANDLE points to a mlx90614 handle structure
 * @param[in] FUC points to a scl_write function address
 * @note      none
 */
#define DRIVER_MLX90614_LINK_SCL_WRITE(HANDLE, FUC)          (HANDLE)->scl_write = FUC

/**
 * @brief     link sda_write function
 * @param[in] HANDLE points to a mlx90614 handle structure
 * @param[in] FUC points to a sda_write function address
 * @note      none
 */
#define DRIVER_MLX90614_LINK_SDA_WRITE(HANDLE, FUC)          (HANDLE)->sda_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to a mlx90614 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_MLX90614_LINK_DELAY_MS(HANDLE, FUC)           (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to a mlx90614 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_MLX90614_LINK_DEBUG_PRINT(HANDLE, FUC)        (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup mlx90614_basic_driver mlx90614 basic driver function
 * @brief    mlx90614 basic driver modules
 * @ingroup  mlx90614_driver
 * @{
 */

/**
 * @brief      get chip information
 * @param[out] *info points to a mlx90614 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t mlx90614_info(mlx90614_info_t *info);

/**
 * @brief     set the address
 * @param[in] *handle points to a mlx90614 handle structure
 * @param[in] addr is the iic address
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t mlx90614_set_addr(mlx90614_handle_t *handle, uint8_t addr);

/**
 * @brief      get the address
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[out] *addr point to a iic address buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t mlx90614_get_addr(mlx90614_handle_t *handle, uint8_t *addr);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to a mlx90614 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t mlx90614_init(mlx90614_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to a mlx90614 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 * @note      none
 */
uint8_t mlx90614_deinit(mlx90614_handle_t *handle);

/**
 * @brief      read the object1
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[out] *raw points to a raw data buffer
 * @param[out] *celsius points to a celsius data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read object1 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 flag error
 * @note       none
 */
uint8_t mlx90614_read_object1(mlx90614_handle_t *handle, uint16_t *raw, float *celsius);

/**
 * @brief      read the object2
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[out] *raw points to a raw data buffer
 * @param[out] *celsius points to a celsius data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read object2 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 flag error
 * @note       none
 */
uint8_t mlx90614_read_object2(mlx90614_handle_t *handle, uint16_t *raw, float *celsius);

/**
 * @brief      read the ir channel raw data
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[out] *channel_1 points to a channel 1 raw data buffer
 * @param[out] *channel_2 points to a channel 2 raw data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read raw ir channel failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mlx90614_read_raw_ir_channel(mlx90614_handle_t *handle, uint16_t *channel_1, uint16_t *channel_2);

/**
 * @brief      read the ambient
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[out] *raw points to a raw data buffer
 * @param[out] *celsius points to a celsius data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read ambient failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mlx90614_read_ambient(mlx90614_handle_t *handle, uint16_t *raw, float *celsius);

/**
 * @}
 */

/**
 * @brief     write the address
 * @param[in] *handle points to a mlx90614 handle structure
 * @param[in] addr is the iic address
 * @return    status code
 *            - 0 success
 *            - 1 write addr failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mlx90614_write_addr(mlx90614_handle_t *handle, uint8_t addr);

/**
 * @brief      read the address
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[out] *addr points to a iic address buffer
 * @return     status code
 *             - 0 success
 *             - 1 read addr failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mlx90614_read_addr(mlx90614_handle_t *handle, uint8_t *addr);

/**
 * @defgroup mlx90614_advance_driver mlx90614 advance driver function
 * @brief    mlx90614 advance driver modules
 * @ingroup  mlx90614_driver
 * @{
 */

/**
 * @brief     set the ir sensor fir length
 * @param[in] *handle points to a mlx90614 handle structure
 * @param[in] len is the fir length
 * @return    status code
 *            - 0 success
 *            - 1 set fir length failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mlx90614_set_fir_length(mlx90614_handle_t *handle, mlx90614_fir_length_t len);

/**
 * @brief      get the ir sensor fir length
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[out] *len points to a fir length buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fir length failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mlx90614_get_fir_length(mlx90614_handle_t *handle, mlx90614_fir_length_t *len);

/**
 * @brief     set the iir param
 * @param[in] *handle points to a mlx90614 handle structure
 * @param[in] iir is the iir param
 * @return    status code
 *            - 0 success
 *            - 1 set fir failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mlx90614_set_iir(mlx90614_handle_t *handle, mlx90614_iir_t iir);

/**
 * @brief      get the iir param
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[out] *iir points to a iir param buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fir failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mlx90614_get_iir(mlx90614_handle_t *handle, mlx90614_iir_t *iir);

/**
 * @brief     set the mode
 * @param[in] *handle points to a mlx90614 handle structure
 * @param[in] mode is the chip mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mlx90614_set_mode(mlx90614_handle_t *handle, mlx90614_mode_t mode);

/**
 * @brief      get the mode
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[out] *mode points to a chip mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mlx90614_get_mode(mlx90614_handle_t *handle, mlx90614_mode_t *mode);

/**
 * @brief     set the ir sensor mode
 * @param[in] *handle points to a mlx90614 handle structure
 * @param[in] sensor is the ir sensor mode
 * @return    status code
 *            - 0 success
 *            - 1 set ir sensor failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mlx90614_set_ir_sensor(mlx90614_handle_t *handle, mlx90614_ir_sensor_t sensor);

/**
 * @brief      get the ir sensor mode
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[out] *sensor points to a ir sensor mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get ir sensor failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mlx90614_get_ir_sensor(mlx90614_handle_t *handle, mlx90614_ir_sensor_t *sensor);

/**
 * @brief     set the ks param
 * @param[in] *handle points to a mlx90614 handle structure
 * @param[in] ks is the ks param
 * @return    status code
 *            - 0 success
 *            - 1 set ks failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mlx90614_set_ks(mlx90614_handle_t *handle, mlx90614_ks_t ks);

/**
 * @brief      get the ks param
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[out] *ks points to a ks param buffer
 * @return     status code
 *             - 0 success
 *             - 1 get ks failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mlx90614_get_ks(mlx90614_handle_t *handle, mlx90614_ks_t *ks);

/**
 * @brief     set the kt2 param
 * @param[in] *handle points to a mlx90614 handle structure
 * @param[in] kt2 is the kt2 param
 * @return    status code
 *            - 0 success
 *            - 1 set kt2 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mlx90614_set_kt2(mlx90614_handle_t *handle, mlx90614_kt2_t kt2);

/**
 * @brief      get the kt2 param
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[out] *kt2 points to a kt2 param buffer
 * @return     status code
 *             - 0 success
 *             - 1 get kt2 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mlx90614_get_kt2(mlx90614_handle_t *handle, mlx90614_kt2_t *kt2);

/**
 * @brief     set the gain param
 * @param[in] *handle points to a mlx90614 handle structure
 * @param[in] gain is the gain param
 * @return    status code
 *            - 0 success
 *            - 1 set gain failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mlx90614_set_gain(mlx90614_handle_t *handle, mlx90614_gain_t gain);

/**
 * @brief      get the gain param
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[out] *gain points to a gain param buffer
 * @return     status code
 *             - 0 success
 *             - 1 get gain failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mlx90614_get_gain(mlx90614_handle_t *handle, mlx90614_gain_t *gain);

/**
 * @brief     enable or disable the sensor test
 * @param[in] *handle points to a mlx90614 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set sensor test failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mlx90614_set_sensor_test(mlx90614_handle_t *handle, mlx90614_bool_t enable);

/**
 * @brief      get the sensor test status
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get sensor test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mlx90614_get_sensor_test(mlx90614_handle_t *handle, mlx90614_bool_t *enable);

/**
 * @brief     enable or disable the repeat sensor test
 * @param[in] *handle points to a mlx90614 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set repeat sensor test failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mlx90614_set_repeat_sensor_test(mlx90614_handle_t *handle, mlx90614_bool_t enable);

/**
 * @brief      get the repeat sensor test status
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get repeat sensor test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mlx90614_get_repeat_sensor_test(mlx90614_handle_t *handle, mlx90614_bool_t *enable);

/**
 * @brief     set the emissivity correction coefficient
 * @param[in] *handle points to a mlx90614 handle structure
 * @param[in] value is the set value
 * @return    status code
 *            - 0 success
 *            - 1 set emissivity correction coefficient failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mlx90614_set_emissivity_correction_coefficient(mlx90614_handle_t *handle, uint16_t value);

/**
 * @brief      get the emissivity correction coefficient
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[out] *value points to a value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get emissivity correction coefficient failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mlx90614_get_emissivity_correction_coefficient(mlx90614_handle_t *handle, uint16_t *value);

/**
 * @brief      convert the emissivity correction coefficient to the register raw data
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[in]  s is the emissivity correction coefficient
 * @param[out] *reg points to a register raw data buffer
 * @return     status code
 *             - 0 success
 *             - 1 emissivity correction coefficient convert to register failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 s is over 1.0
 * @note       none
 */
uint8_t mlx90614_emissivity_correction_coefficient_convert_to_register(mlx90614_handle_t *handle, double s, uint16_t *reg);

/**
 * @brief      emissivity correction coefficient convert to data
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[in]  reg is the raw data
 * @param[out] *s points to a converted data buffer
 * @return     status code
 *             - 0 success
 *             - 1 emissivity correction coefficient convert to data failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mlx90614_emissivity_correction_coefficient_convert_to_data(mlx90614_handle_t *handle, uint16_t reg, double *s);

/**
 * @brief      get the chip id
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[out] *id points to a id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get id failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mlx90614_get_id(mlx90614_handle_t *handle, uint16_t id[4]);

/**
 * @brief     enter to sleep mode
 * @param[in] *handle points to a mlx90614 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 enter sleep mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mlx90614_enter_sleep_mode(mlx90614_handle_t *handle);

/**
 * @brief     exit from sleep mode
 * @param[in] *handle points to a mlx90614 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 exit sleep mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mlx90614_exit_sleep_mode(mlx90614_handle_t *handle);

/**
 * @brief      get the flag
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[out] *flag points to a flag buffer
 * @return     status code
 *             - 0 success
 *             - 1 get flag failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mlx90614_get_flag(mlx90614_handle_t *handle, uint16_t *flag);

/**
 * @brief     change pwm mode to smbus mode
 * @param[in] *handle points to a mlx90614 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 pwm to smbus failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mlx90614_pwm_to_smbus(mlx90614_handle_t *handle);

/**
 * @}
 */

/**
 * @defgroup mlx90614_extern_driver mlx90614 extern driver function
 * @brief    mlx90614 extern driver modules
 * @ingroup  mlx90614_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle points to a mlx90614 handle structure
 * @param[in] reg is the iic register address
 * @param[in] data is the written data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mlx90614_set_reg(mlx90614_handle_t *handle, uint8_t reg, uint16_t data);

/**
 * @brief      get the chip register
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mlx90614_get_reg(mlx90614_handle_t *handle, uint8_t reg, uint16_t *data);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
