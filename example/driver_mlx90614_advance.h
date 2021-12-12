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
 * @file      driver_mlx90614_advance.h
 * @brief     driver mlx90614 advance header file
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

#ifndef _DRIVER_MLX90614_ADVANCE_H_
#define _DRIVER_MLX90614_ADVANCE_H_

#include "driver_mlx90614_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup mlx90614_example_driver
 * @{
 */

/**
 * @brief mlx90614 advance example default definition
 */
#define MLX90614_ADVANCE_DEFAULT_FIR_LENGTH                               MLX90614_FIR_LENGTH_1024        /**< fir length 1024 */
#define MLX90614_ADVANCE_DEFAULT_IIR                                      MLX90614_IIR_A1_1_B1_0          /**< iir a1 1 b1 0 */
#define MLX90614_ADVANCE_DEFAULT_MODE                                     MLX90614_MODE_TA_TOBJ1          /**< ta tobj1 mode */
#define MLX90614_ADVANCE_DEFAULT_IR_SENSOR                                MLX90614_IR_SENSOR_DUAL         /**< dual ir sensor mode */
#define MLX90614_ADVANCE_DEFAULT_KS                                       MLX90614_KS_POSITIVE            /**< positive ks */
#define MLX90614_ADVANCE_DEFAULT_KT2                                      MLX90614_KT2_POSITIVE           /**< positive kt2 */
#define MLX90614_ADVANCE_DEFAULT_GAIN                                     MLX90614_GAIN_100               /**< gain 100 */
#define MLX90614_ADVANCE_DEFAULT_SENSOR_TEST                              MLX90614_BOOL_FALSE             /**< false */
#define MLX90614_ADVANCE_DEFAULT_REPEAT_SENSOR_TEST                       MLX90614_BOOL_FALSE             /**< false */
#define MLX90614_ADVANCE_DEFAULT_EMISSIVITY_CORRECTION_COEFFICIENT        1.0                             /**< 1.0 */

/**
 * @brief  advance example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t mlx90614_advance_init(void);

/**
 * @brief  advance example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t mlx90614_advance_deinit(void);

/**
 * @brief  advance example enter sleep
 * @return status code
 *         - 0 success
 *         - 1 enter sleep failed
 * @note   none
 */
uint8_t mlx90614_advance_enter_sleep(void);

/**
 * @brief  advance example exit sleep
 * @return status code
 *         - 0 success
 *         - 1 exit sleep failed
 * @note   none
 */
uint8_t mlx90614_advance_exit_sleep(void);

/**
 * @brief     advance example read
 * @param[in] *ambient points to a ambient buffer
 * @param[in] *object points to a object buffer
 * @return    status code
 *            - 0 success
 *            - 1 read failed
 * @note      none
 */
uint8_t mlx90614_advance_read(float *ambient, float *object);

/**
 * @brief     advance example get id
 * @param[in] *id points to a id buffer
 * @return    status code
 *            - 0 success
 *            - 1 get id failed
 * @note      none
 */
uint8_t mlx90614_advance_get_id(uint16_t id[4]);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
