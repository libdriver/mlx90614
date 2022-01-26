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
 * @file      driver_mlx90614.c
 * @brief     driver mlx90614 source file
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

#include "driver_mlx90614.h"
#include <math.h>

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Melexis MLX90614"        /**< chip name */
#define MANUFACTURER_NAME         "Melexis"                 /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        4.5f                      /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                      /**< chip max supply voltage */
#define MAX_CURRENT               2.5f                      /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                    /**< chip min operating temperature */
#define TEMPERATURE_MAX           125.0f                    /**< chip max operating temperature */
#define DRIVER_VERSION            1000                      /**< driver version */

/**
 * @brief chip command definition
 */
#define COMMAND_READ_FLAGS         0xF0        /**< read flags command */
#define COMMAND_ENTER_SLEEP        0xFF        /**< enter sleep command */

/**
 * @brief chip register definition
 */
#define MLX90614_REG_RAM_RAW_DATA_IR_CHANNEL_1        0x04        /**< ram raw data ir channel 1 register */
#define MLX90614_REG_RAM_RAW_DATA_IR_CHANNEL_2        0x05        /**< ram raw data ir channel 2 register */
#define MLX90614_REG_RAM_TA                           0x06        /**< ram ta register */
#define MLX90614_REG_RAM_TOBJ1                        0x07        /**< ram tobj1 register */
#define MLX90614_REG_RAM_TOBJ2                        0x08        /**< ram tobj2 register */
#define MLX90614_REG_EEPROM_TO_MAX                    0x20        /**< eeprom to max register */
#define MLX90614_REG_EEPROM_TO_MIN                    0x21        /**< eeprom to min register */
#define MLX90614_REG_EEPROM_PWM_CTRL                  0x22        /**< eeprom pwm ctrl register */
#define MLX90614_REG_EEPROM_TA_RANGE                  0x23        /**< eeprom ta range register */
#define MLX90614_REG_EEPROM_EMISSIVITY                0x24        /**< eeprom emissivity register */
#define MLX90614_REG_EERPOM_CONFIG1                   0x25        /**< eeprom config1 register */
#define MLX90614_REG_EERPOM_ADDRESS                   0x2E        /**< eeprom address register */
#define MLX90614_REG_EERPOM_ID1_NUMBER                0x3C        /**< eeprom id1 number register */
#define MLX90614_REG_EERPOM_ID2_NUMBER                0x3D        /**< eeprom id2 number register */
#define MLX90614_REG_EERPOM_ID3_NUMBER                0x3E        /**< eeprom id3 number register */
#define MLX90614_REG_EERPOM_ID4_NUMBER                0x3F        /**< eeprom id4 number register */

/**
 * @brief     calculate the crc
 * @param[in] *addr points to a buffer address
 * @param[in] len is the buffer length
 * @return    crc
 * @note      none
 */
static uint8_t _mlx90614_calculate_crc(uint8_t *addr, uint8_t len)
{
    volatile uint8_t crc = 0;
  
    while (len--) 
    {
        volatile uint8_t i;
        volatile uint8_t in_byte = *addr++;
    
        for (i = 8; i; i--)                                          /* 8 times */
        {
            volatile uint8_t carry = (crc ^ in_byte ) & 0x80;        /* set carry */
            
            crc <<= 1;                                               /* left shift 1 */
            if (carry)                                               /* check carry */
            {
                crc ^= 0x7;                                          /* xor 0x7 */
            }
            in_byte <<= 1;                                           /* left shift 1 */
    }
  }
    
  return crc;                                                        /* return crc */
}


/**
 * @brief      read the data from the bus
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[in]  command is the sent command
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t _mlx90614_read(mlx90614_handle_t *handle, uint8_t command, uint16_t *data)
{
    volatile uint8_t pec;
    volatile uint8_t arr[5];
    volatile uint8_t buf[3];
    volatile uint8_t times = 0x03;
    
    repeat:
    if (handle->iic_read(handle->iic_addr, command, (uint8_t *)buf, 3))        /* read data */
    {
        return 1;                                                              /* return error */
    }
    
    arr[0] = handle->iic_addr;                                                 /* set read addr */
    arr[1] = command;                                                          /* set command */ 
    arr[2] = handle->iic_addr + 1;                                             /* set write addr */
    arr[3] = buf[0];                                                           /* set buf 0 */
    arr[4] = buf[1];                                                           /* set buf 1 */
    pec = _mlx90614_calculate_crc((uint8_t *)arr, 5);                          /* calculate pec */
    if ((pec != buf[2]) && (times != 0))                                       /* check */
    {
        times--;                                                               /* times-- */
        handle->delay_ms(5);                                                   /* delay ms */
        
        goto repeat;                                                           /* goto repeat */
    }
    if (times == 0)                                                            /* timeout */
    {
        return 1;                                                              /* return error */
    }
    
    *data = ((uint16_t)buf[1] << 8) | buf[0];                                  /* get data */
  
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief     write the data to the bus
 * @param[in] *handle points to a mlx90614 handle structure
 * @param[in] command is the sent command
 * @param[in] data is the sent data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t _mlx90614_write(mlx90614_handle_t *handle, uint8_t command, uint16_t data)
{
    volatile uint8_t data_l;
    volatile uint8_t data_h;
    volatile uint8_t arr[4];
    volatile uint8_t buf[3];
    volatile uint8_t pec;
    
    if (command & 0x20)                                                                /* if eeprom */
    {
        data_l = 0x00;                                                                 /* get lsb */
        data_h = 0x00;                                                                 /* get msb */
        arr[0] = handle->iic_addr;                                                     /* set address */
        arr[1] = command;                                                              /* set command */
        arr[2] = data_l;                                                               /* set lsb */
        arr[3] = data_h;                                                               /* set msb */
        pec = _mlx90614_calculate_crc((uint8_t *)arr, 4);                              /* calculate pec */
        buf[0] = data_l;                                                               /* set lsb */
        buf[1] = data_h;                                                               /* set msb */
        buf[2] = pec;                                                                  /* set pec */
        if (handle->iic_write(handle->iic_addr, command, (uint8_t *)buf, 3))           /* write command */
        {
            return 1;                                                                  /* return error */
        }
        handle->delay_ms(10);                                                          /* delay 10 ms */
        data_l = data & 0xFF;                                                          /* get lsb */
        data_h = (data & 0xFF00) >> 8;                                                 /* get msb */
        arr[0] = handle->iic_addr;                                                     /* set address */
        arr[1] = command;                                                              /* set command */
        arr[2] = data_l;                                                               /* set lsb */
        arr[3] = data_h;                                                               /* set msb */
        pec = _mlx90614_calculate_crc((uint8_t *)arr, 4);                              /* calculate pec */
        buf[0] = data_l;                                                               /* set lsb */
        buf[1] = data_h;                                                               /* set msb */
        buf[2] = pec;                                                                  /* set pec */
        if (handle->iic_write(handle->iic_addr, command, (uint8_t *)buf, 3))           /* write data */
        {
            return 1;                                                                  /* return error */
        }
        handle->delay_ms(10);                                                          /* delay 10 ms */
        
        return 0;                                                                      /* success return 0 */
    }
    else                                                                               /* ram */
    {
        data_l = data & 0xFF;                                                          /* get lsb */
        data_h = (data & 0xFF00) >> 8;                                                 /* get msb */
        arr[0] = handle->iic_addr;                                                     /* set address */
        arr[1] = command;                                                              /* set command */
        arr[2] = data_l;                                                               /* set lsb */
        arr[3] = data_h;                                                               /* set msb */
        pec = _mlx90614_calculate_crc((uint8_t *)arr, 4);                              /* calculate pec */
        buf[0] = data_l;                                                               /* set lsb */
        buf[1] = data_h;                                                               /* set msb */
        buf[2] = pec;                                                                  /* set pec */
        
        return handle->iic_write(handle->iic_addr, command, (uint8_t *)buf, 3);        /* write data */
    }
}

/**
 * @brief     set the address
 * @param[in] *handle points to a mlx90614 handle structure
 * @param[in] addr is the iic address
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t mlx90614_set_addr(mlx90614_handle_t *handle, uint8_t addr)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    
    handle->iic_addr = addr;        /* set addr */
  
    return 0;                       /* success return 0 */
}

/**
 * @brief      get the address
 * @param[in]  *handle points to a mlx90614 handle structure
 * @param[out] *addr point to a iic address buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t mlx90614_get_addr(mlx90614_handle_t *handle, uint8_t *addr)
{
    if (handle == NULL)              /* check handle */
    {
        return 2;                    /* return error */
    }
    
    *addr = handle->iic_addr;        /* get addr */
  
    return 0;                        /* success return 0 */
}

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
uint8_t mlx90614_write_addr(mlx90614_handle_t *handle, uint8_t addr)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    handle->iic_addr = 0x00;                                                             /* set 0x00 */
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_ADDRESS, (uint16_t *)&prev);        /* read eeprom address */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read eeprom address failed.\n");                  /* read eeprom address failed */
        
        return 1;                                                                        /* return error */
    }
    prev |= addr;                                                                        /* set address */
    res = _mlx90614_write(handle, MLX90614_REG_EERPOM_ADDRESS, prev);                    /* write eeprom address */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: write eeprom address failed.\n");                 /* write eeprom address failed */
        
        return 1;                                                                        /* return error */
    }
    handle->iic_addr = addr;                                                             /* set addrress */
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t mlx90614_read_addr(mlx90614_handle_t *handle, uint8_t *addr)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    handle->iic_addr = 0x00;                                                             /* set 0x00 */
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_ADDRESS, (uint16_t *)&prev);        /* read eeprom address */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read eeprom address failed.\n");                  /* read eeprom address failed */
        
        return 1;                                                                        /* return error */
    }
    handle->iic_addr = (prev >> 0) & 0x0F;                                               /* set iic address */
    *addr = handle->iic_addr;                                                            /* get addr */
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t mlx90614_set_fir_length(mlx90614_handle_t *handle, mlx90614_fir_length_t len)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t *)&prev);        /* read config */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read config failed.\n");                          /* read config failed */
        
        return 1;                                                                        /* return error */
    }
    prev &= ~(0x07 << 8);                                                                /* clear config */
    prev |= len << 8;                                                                    /* set length */
    
    return _mlx90614_write(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t )prev);        /* write config */
}

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
uint8_t mlx90614_get_fir_length(mlx90614_handle_t *handle, mlx90614_fir_length_t *len)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t *)&prev);        /* read config */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read config failed.\n");                          /* read config failed */
        
        return 1;                                                                        /* return error */
    }
    prev &= (0x07 << 8);                                                                 /* get config */
    *len = (mlx90614_fir_length_t)(0x07 & (prev >> 8));                                  /* get length */
  
    return 0;                                                                            /* success return 0 */
}

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
uint8_t mlx90614_set_iir(mlx90614_handle_t *handle, mlx90614_iir_t iir)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t *)&prev);        /* read config */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read config failed.\n");                          /* read config failed */
        
        return 1;                                                                        /* return error */
    }
    prev &= ~0x07;                                                                       /* clear config */
    prev |= iir;                                                                         /* set param */
    
    return _mlx90614_write(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t )prev);        /* wrtie config */
}

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
uint8_t mlx90614_get_iir(mlx90614_handle_t *handle, mlx90614_iir_t *iir)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t *)&prev);        /* read config */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read config failed.\n");                          /* read config failed */
        
        return 1;                                                                        /* return error */
    }
    prev &= 0x07;                                                                        /* get config */
    *iir = (mlx90614_iir_t)(0x07 & prev);                                                /* get param */
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t mlx90614_set_mode(mlx90614_handle_t *handle, mlx90614_mode_t mode)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t *)&prev);        /* read config */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read config failed.\n");                          /* read config failed */
        
        return 1;                                                                        /* return error */
    }
    prev &= ~(0x03 << 4);                                                                /* clear config */
    prev |= mode << 4;                                                                   /* set param */
    
    return _mlx90614_write(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t )prev);        /* wrtie config */
}

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
uint8_t mlx90614_get_mode(mlx90614_handle_t *handle, mlx90614_mode_t *mode)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
  
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t *)&prev);        /* read config */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read config failed.\n");                          /* read config failed */
        
        return 1;                                                                        /* return error */
    }
    prev &= (0x03 << 4);                                                                 /* clear config */
    *mode = (mlx90614_mode_t)(0x03 & (prev >> 4));                                       /* get mode */
  
    return 0;                                                                            /* success return 0 */
}

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
uint8_t mlx90614_set_ir_sensor(mlx90614_handle_t *handle, mlx90614_ir_sensor_t sensor)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t *)&prev);        /* read config */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read config failed.\n");                          /* read config failed */
        
        return 1;                                                                        /* return error */
    }
    prev &= ~(0x01 << 6);                                                                /* clear config */
    prev |= sensor << 6;                                                                 /* set param */
   
    return _mlx90614_write(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t )prev);        /* wrtie config */
}

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
uint8_t mlx90614_get_ir_sensor(mlx90614_handle_t *handle, mlx90614_ir_sensor_t *sensor)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t *)&prev);        /* read config */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read config failed.\n");                          /* return error */
        
        return 1;
    }
    prev &= (0x01 << 6);                                                                 /* get config */
    *sensor = (mlx90614_ir_sensor_t)(0x01 & (prev >> 6));                                /* ge sensor */
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t mlx90614_set_ks(mlx90614_handle_t *handle, mlx90614_ks_t ks)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t *)&prev);        /* read config */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read config failed.\n");                          /* read config failed */
        
        return 1;                                                                        /* return error */
    }
    prev &= ~(0x01 << 7);                                                                /* clear config */
    prev |= ks << 7;                                                                     /* set param */
    
    return _mlx90614_write(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t )prev);        /* wrtie config */
}

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
uint8_t mlx90614_get_ks(mlx90614_handle_t *handle, mlx90614_ks_t *ks)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
  
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t *)&prev);        /* read config */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read config failed.\n");                          /* read config failed */
        
        return 1;                                                                        /* return error */
    }
    prev &= (0x01 << 7);                                                                 /* get config */
    *ks = (mlx90614_ks_t)(0x01 & (prev >> 7));                                           /* get ks */
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t mlx90614_set_kt2(mlx90614_handle_t *handle, mlx90614_kt2_t kt2)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t *)&prev);        /* read config */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read config failed.\n");                          /* read config failed. */
        
        return 1;                                                                        /* return error */
    }
    prev &= ~(0x01 << 14);                                                               /* clear config */
    prev |= kt2 << 14;                                                                   /* set param */
    
    return _mlx90614_write(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t )prev);        /* wrtie config */
}

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
uint8_t mlx90614_get_kt2(mlx90614_handle_t *handle, mlx90614_kt2_t *kt2)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t *)&prev);        /* read config */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read config failed.\n");                          /* read config failed */
        
        return 1;                                                                        /* return error */
    }
    prev &= (0x01 << 14);                                                                /* get config */
    *kt2 = (mlx90614_kt2_t)(0x01 & (prev >> 14));                                        /* get kt2 */
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t mlx90614_set_gain(mlx90614_handle_t *handle, mlx90614_gain_t gain)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t *)&prev);        /* read config */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read config failed.\n");                          /* read config failed */
        
        return 1;                                                                        /* return error */
    }
    prev &= ~(0x07 << 11);                                                               /* clear config */
    prev |= gain << 11;                                                                  /* set param */
    
    return _mlx90614_write(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t )prev);        /* wrtie config */
}

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
uint8_t mlx90614_get_gain(mlx90614_handle_t *handle, mlx90614_gain_t *gain)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t *)&prev);        /* read config */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read config failed.\n");                          /* read config failed */
        
        return 1;                                                                        /* return error */
    }
    prev &= 0x07 << 11;                                                                  /* get config */
    *gain = (mlx90614_gain_t)(0x07 & (prev >> 11));                                      /* get param */
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t mlx90614_set_sensor_test(mlx90614_handle_t *handle, mlx90614_bool_t enable)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t *)&prev);        /* read config */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read config failed.\n");                          /* read config failed */
        
        return 1;                                                                        /* return error */
    }
    prev &= ~(0x01 << 15);                                                               /* clear config */
    prev |= enable << 15;                                                                /* set param */
    
    return _mlx90614_write(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t )prev);        /* wrtie config */
}

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
uint8_t mlx90614_get_sensor_test(mlx90614_handle_t *handle, mlx90614_bool_t *enable)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t *)&prev);        /* read config */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read config failed.\n");                          /* read config failed */
        
        return 1;                                                                        /* return error */
    }
    prev &= 0x01 << 15;                                                                  /* get config */
    *enable = (mlx90614_bool_t)(0x01 & (prev >> 15));                                    /* set param */
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t mlx90614_set_repeat_sensor_test(mlx90614_handle_t *handle, mlx90614_bool_t enable)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t *)&prev);        /* read config */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read config failed.\n");                          /* read config failed */
        
        return 1;                                                                        /* return error */
    }
    prev &= ~(0x01 << 3);                                                                /* clear config */
    prev |= enable << 3;                                                                 /* set param */
    
    return _mlx90614_write(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t )prev);        /* wrtie config */
}

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
uint8_t mlx90614_get_repeat_sensor_test(mlx90614_handle_t *handle, mlx90614_bool_t *enable)
{
    volatile uint8_t res;
    volatile uint16_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_CONFIG1, (uint16_t *)&prev);        /* read config */
    if (res)                                                                             /* check result */
    {
        handle->debug_print("mlx90614: read config failed.\n");                          /* read config failed */
        
        return 1;                                                                        /* return error */
    }
    prev &= 0x01 << 3;                                                                   /* get config */
    *enable = (mlx90614_bool_t)(0x01 & (prev >> 3));                                     /* set param */
   
    return 0;                                                                            /* success return 0 */
}

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
uint8_t mlx90614_set_emissivity_correction_coefficient(mlx90614_handle_t *handle, uint16_t value)
{
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    
    return _mlx90614_write(handle, MLX90614_REG_EEPROM_EMISSIVITY, value);        /* wrtie config */
}

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
uint8_t mlx90614_get_emissivity_correction_coefficient(mlx90614_handle_t *handle, uint16_t *value)
{
    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }
    
    return _mlx90614_read(handle, MLX90614_REG_EEPROM_EMISSIVITY, value);        /* wrtie config */
}

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
uint8_t mlx90614_emissivity_correction_coefficient_convert_to_register(mlx90614_handle_t *handle, double s, uint16_t *reg)
{
    if (handle == NULL)                                           /* check handle */
    {
        return 2;                                                 /* return error */
    }
    if (handle->inited != 1)                                      /* check handle initialization */
    {
        return 3;                                                 /* return error */
    }
    if (s > 1.0)                                                  /* check s */
    {
        handle->debug_print("mlx90614: s is over 1.0.\n");        /* return error */
        
        return 4;                                                 /* return error */
    }
    
    *reg = (uint16_t)(round((double)65535 * s));                  /* set reg */
    
    return 0;                                                     /* success return 0 */
}

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
uint8_t mlx90614_emissivity_correction_coefficient_convert_to_data(mlx90614_handle_t *handle, uint16_t reg, double *s)
{
    if (handle == NULL)                      /* check handle */
    {
         return 2;                           /* return error */
    }
    if (handle->inited != 1)                 /* check handle initialization */
    {
         return 3;                           /* return error */
    }
    
    *s = (double) reg / 65535;               /* convert reg */
    
    return 0;                                /* success return 0 */
}

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
uint8_t mlx90614_read_raw_ir_channel(mlx90614_handle_t *handle, uint16_t *channel_1, uint16_t *channel_2)
{
    volatile uint8_t res;
    
    if (handle == NULL)                                                                                 /* check handle */
    {
         return 2;                                                                                      /* return error */
    }
    if (handle->inited != 1)                                                                            /* check handle initialization */
    {
         return 3;                                                                                      /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_RAM_RAW_DATA_IR_CHANNEL_1, (uint16_t *)channel_1);        /* read data */
    if (res)                                                                                            /* check result */
    {
        handle->debug_print("mlx90614: read raw channel 1 failed.\n");                                  /* read raw channel 1 failed */
        
        return 1;                                                                                       /* return error */
    }
    res = _mlx90614_read(handle, MLX90614_REG_RAM_RAW_DATA_IR_CHANNEL_2, (uint16_t *)channel_2);        /* read data */
    if (res)                                                                                            /* check result */
    {
        handle->debug_print("mlx90614: read raw channel 2 failed.\n");                                  /* read raw channel 2 failed */
        
        return 1;                                                                                       /* return error */
    }
    
    return 0;                                                                                           /* success return 0 */
}

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
uint8_t mlx90614_read_ambient(mlx90614_handle_t *handle, uint16_t *raw, float *celsius)
{
    volatile uint8_t res;
    
    if (handle == NULL)                                                        /* check handle */
    {
         return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
         return 3;                                                             /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_RAM_TA, (uint16_t *)raw);        /* read data */
    if (res)                                                                   /* check result */
    {
        handle->debug_print("mlx90614: read raw ta failed.\n");                /* read raw ta failed */
        
        return 1;                                                              /* return error */
    }
    *celsius = (float)(*raw) * 0.02f - 273.15f;                                /* get celsius */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t mlx90614_read_object1(mlx90614_handle_t *handle, uint16_t *raw, float *celsius)
{
    volatile uint8_t res;
    
    if (handle == NULL)                                                           /* check handle */
    {
         return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
         return 3;                                                                /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_RAM_TOBJ1, (uint16_t *)raw);        /* read data */
    if (res)                                                                      /* check result */
    {
        handle->debug_print("mlx90614: read ram tobj1 failed.\n");                /* read ram tobj1 failed. */
        
        return 1;                                                                 /* return error */
    }
    if ((*raw) & 0x8000)                                                          /* check result */
    {
        handle->debug_print("mlx90614: flag error.\n");                           /* flag error */
        
        return 4;                                                                 /* return error */
    }
    *celsius = (float)(*raw) * 0.02f - 273.15f;                                   /* get celsius */
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t mlx90614_read_object2(mlx90614_handle_t *handle, uint16_t *raw, float *celsius)
{
    volatile uint8_t res;
    
    if (handle == NULL)                                                           /* check handle */
    {
         return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
         return 3;                                                                /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_RAM_TOBJ2, (uint16_t *)raw);        /* read data */
    if (res)                                                                      /* check result */
    {
        handle->debug_print("mlx90614: read ram tobj2 failed.\n");                /* read ram tobj2 failed. */
        
        return 1;                                                                 /* return error */
    }
    if ((*raw) & 0x8000)                                                          /* check result */
    {
        handle->debug_print("mlx90614: flag error.\n");                           /* flag error */
        
        return 4;                                                                 /* return error */
    }
    *celsius = (float)(*raw) * 0.02f - 273.15f;                                   /* get celsius */
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t mlx90614_get_id(mlx90614_handle_t *handle, uint16_t id[4])
{
    volatile uint8_t res;
    
    if (handle == NULL)                                                                      /* check handle */
    {
         return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
         return 3;                                                                           /* return error */
    }
    
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_ID1_NUMBER, (uint16_t *)&id[0]);        /* read data */
    if (res)                                                                                 /* check result */
    {
        handle->debug_print("mlx90614: read id1 failed.\n");                                 /* read id1 failed */
        
        return 1;                                                                            /* return error */
    }
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_ID2_NUMBER, (uint16_t *)&id[1]);        /* read data */
    if (res)                                                                                 /* check result */
    {
        handle->debug_print("mlx90614: read id2 failed.\n");                                 /* read id2 failed */
        
        return 1;                                                                            /* return error */
    }
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_ID3_NUMBER, (uint16_t *)&id[2]);        /* read data */
    if (res)                                                                                 /* check result */
    {
        handle->debug_print("mlx90614: read id3 failed.\n");                                 /* read id3 failed */
        
        return 1;                                                                            /* return error */
    }
    res = _mlx90614_read(handle, MLX90614_REG_EERPOM_ID4_NUMBER, (uint16_t *)&id[3]);        /* read data */
    if (res)                                                                                 /* check result */
    {
        handle->debug_print("mlx90614: read id4 failed.\n");                                 /* read id4 failed */
        
        return 1;                                                                            /* return error */
    }
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t mlx90614_get_flag(mlx90614_handle_t *handle, uint16_t *flag)
{
    if (handle == NULL)                                                                       /* check handle */
    {
         return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
         return 3;                                                                            /* return error */
    }
    
    return handle->iic_read(handle->iic_addr, COMMAND_READ_FLAGS, (uint8_t *)flag, 1);        /* read config */
}

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
uint8_t mlx90614_enter_sleep_mode(mlx90614_handle_t *handle)
{
    volatile uint8_t crc;
    volatile uint8_t buf[2];
    
    if (handle == NULL)                                                                         /* check handle */
    {
         return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                    /* check handle initialization */
    {
         return 3;                                                                              /* return error */
    }
    
    buf[0] = handle->iic_addr;                                                                  /* set iic address */
    buf[1] = COMMAND_ENTER_SLEEP;                                                               /* set command */
    crc = _mlx90614_calculate_crc((uint8_t *)buf, 2);                                           /* set crc */
    
    return handle->iic_write(handle->iic_addr, COMMAND_ENTER_SLEEP, (uint8_t *)&crc, 1);        /* write config */
}

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
uint8_t mlx90614_pwm_to_smbus(mlx90614_handle_t *handle)
{
    volatile uint8_t res;
    
    if (handle == NULL)                                              /* check handle */
    {
         return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                         /* check handle initialization */
    {
         return 3;                                                   /* return error */
    }
    
    res = handle->scl_write(0);                                      /* set scl low */
    if (res)                                                         /* check result */
    {
        handle->debug_print("mlx90614: write scl failed.\n");        /* write scl failed */
        
        return 1;                                                    /* return error */
    }
    handle->delay_ms(5);                                             /* delay 5 ms */
    res = handle->scl_write(1);                                      /* set scl high */
    if (res)                                                         /* check result */
    {
        handle->debug_print("mlx90614: write scl failed.\n");        /* write scl failed */
        
        return 1;                                                    /* return error */
    }
    
    return 0;                                                        /* success return 0 */
}

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
uint8_t mlx90614_exit_sleep_mode(mlx90614_handle_t *handle)
{
    volatile uint8_t res;
    
    if (handle == NULL)                                              /* check handle */
    {
         return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                         /* check handle initialization */
    {
         return 3;                                                   /* return error */
    }
    
    res = handle->scl_write(1);                                      /* write scl 1 */
    if (res)                                                         /* check result */
    {
        handle->debug_print("mlx90614: write scl failed.\n");        /* write scl failed */
        
        return 1;                                                    /* return error */
    }
    res = handle->sda_write(1);                                      /* write sda 1 */
    if (res)                                                         /* check result */
    {
        handle->debug_print("mlx90614: write sda failed.\n");        /* write sda failed */
        
        return 1;                                                    /* return error */
    }
    handle->delay_ms(1);                                             /* delay 1 ms */
    res = handle->sda_write(0);                                      /* write sda 0 */
    if (res)                                                         /* check result */
    {
        handle->debug_print("mlx90614: write sda failed.\n");        /* write sda failed */
        
        return 1;                                                    /* return error */
    }    
    handle->delay_ms(50);                                            /* delay 50 ms */
    res = handle->sda_write(1);                                      /* write sda 1 */
    if (res)                                                         /* return error */
    {
        handle->debug_print("mlx90614: write sda failed.\n");        /* write sda failed */
        
        return 1;                                                    /* return error */
    }
    handle->delay_ms(260);                                           /* delay 260 ms */
    
    return 0;                                                        /* success return 0 */
}

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
uint8_t mlx90614_init(mlx90614_handle_t *handle)
{
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->debug_print == NULL)                                 /* check debug_print */
    {
        return 3;                                                    /* return error */
    }
    if (handle->iic_init == NULL)                                    /* check iic_init */
    {
        handle->debug_print("mlx90614: iic_init is null.\n");        /* iic_init is null */
        
        return 3;                                                    /* return error */
    }
    if (handle->iic_deinit == NULL)                                  /* check iic_deinit */
    {
        handle->debug_print("mlx90614: iic_deinit is null.\n");      /* iic_deinit is null */
        
        return 3;                                                    /* return error */
    }
    if (handle->iic_read == NULL)                                    /* check iic_read */
    {
        handle->debug_print("mlx90614: iic_read is null.\n");        /* iic_read is null */
        
        return 3;                                                    /* return error */
    }
    if (handle->iic_write == NULL)                                   /* check iic_write */
    {
        handle->debug_print("mlx90614: iic_write is null.\n");       /* iic_write is null */
        
        return 3;                                                    /* return error */
    }
    if (handle->scl_write == NULL)                                   /* check scl_write */
    {
        handle->debug_print("mlx90614: scl_write is null.\n");       /* scl_write is null */
        
        return 3;                                                    /* return error */
    }
    if (handle->sda_write == NULL)                                   /* check sda_write */
    {
        handle->debug_print("mlx90614: sda_write is null.\n");       /* sda_write is null */
        
        return 3;                                                    /* return error */
    }
    if (handle->delay_ms == NULL)                                    /* check delay_ms */
    {
        handle->debug_print("mlx90614: delay_ms is null.\n");        /* delay_ms is null */
        
        return 3;                                                    /* return error */
    }
    
    if (handle->iic_init())                                          /* iic init */
    {
        handle->debug_print("mlx90614: iic init failed.\n");         /* iic init failed */
        
        return 1;                                                    /* return error */
    }
    handle->inited = 1;                                              /* flag finish initialization */

    return 0;                                                        /* success return 0 */
}

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
uint8_t mlx90614_deinit(mlx90614_handle_t *handle)
{
    volatile uint8_t res;
    volatile uint8_t crc;
    volatile uint8_t buf[2];
    
    if (handle == NULL)                                                                        /* check handle */
    {
         return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
         return 3;                                                                             /* return error */
    }
    
    buf[0] = handle->iic_addr;                                                                 /* set iic address */
    buf[1] = COMMAND_ENTER_SLEEP;                                                              /* set command */
    crc = _mlx90614_calculate_crc((uint8_t *)buf, 2);                                          /* set crc */
    res = handle->iic_write(handle->iic_addr, COMMAND_ENTER_SLEEP, (uint8_t *)&crc, 1);        /* write config */
    if (res)                                                                                   /* check result */
    {
        handle->debug_print("mlx90614: power down failed.\n");                                 /* power down failed */
        
        return 4;                                                                              /* return error */
    }
    res = handle->iic_deinit();                                                                /* iic deinit */
    if (res)                                                                                   /* check result */
    {
        handle->debug_print("mlx90614: iic deinit failed.\n");                                 /* iic deinit failed */
        
        return 1;                                                                              /* return error */
    }
    handle->inited = 0;                                                                        /* flag close */
    
    return 0;                                                                                  /* success return 0 */
}

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
uint8_t mlx90614_set_reg(mlx90614_handle_t *handle, uint8_t reg, uint16_t data)
{
    if (handle == NULL)                               /* check handle */
    {
        return 2;                                     /* return error */
    }
    if (handle->inited != 1)                          /* check handle initialization */
    {
        return 3;                                     /* return error */
    }
  
    return _mlx90614_write(handle, reg, data);        /* write data */
}

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
uint8_t mlx90614_get_reg(mlx90614_handle_t *handle, uint8_t reg, uint16_t *data)
{
    if (handle == NULL)                               /* check handle */
    {
        return 2;                                     /* return error */
    }
    if (handle->inited != 1)                          /* check handle initialization */
    {
        return 3;                                     /* return error */
    }
  
    return _mlx90614_read(handle, reg, data);        /* write data */
}

/**
 * @brief      get chip information
 * @param[out] *info points to a mlx90614 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t mlx90614_info(mlx90614_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(mlx90614_info_t));                       /* initialize mlx90614 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver verison */
    
    return 0;                                                       /* success return 0 */
}
