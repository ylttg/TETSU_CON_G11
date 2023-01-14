/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2016, 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_dac.h
* Version      : Code Generator for RL78/G11 V1.02.03.02 [20 Nov 2019]
* Device(s)    : R5F1056A
* Tool-Chain   : CCRL
* Description  : This file implements device driver for DAC module.
* Creation Date: 2022/12/27
***********************************************************************************************************************/
#ifndef DAC_H
#define DAC_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    D/A converter mode register (DAM) 
*/
/* D/A conversion operation control (DACE1) */
#define _00_DA1_CONVERSION_STOP            (0x00U) /* stops D/A conversion operation */
#define _20_DA1_CONVERSION_ENABLE          (0x20U) /* enables D/A conversion operation */
/* D/A conversion operation control (DACE0) */
#define _00_DA0_CONVERSION_STOP            (0x00U) /* stops D/A conversion operation */
#define _10_DA0_CONVERSION_ENABLE          (0x10U) /* enables D/A conversion operation */
/* D/A converter operation mode selection (DAMD1) */
#define _00_DA1_NORMAL_MODE                (0x00U) /* normal mode */
#define _02_DA1_TIME_OUTPUT_MODE           (0x02U) /* real-time output mode */


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _6C_DA0_COUVERSION_VALUE           (0x6CU) /* set the analog voltage value */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_DAC0_Create(void);
void R_DAC0_Start(void);
void R_DAC0_Stop(void);
void R_DAC0_Set_ConversionValue(uint8_t regvalue);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
