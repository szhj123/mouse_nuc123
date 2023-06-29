/********************************************************
* @file       hal_mouse_sensor.c
* @author     szhj13
* @version    V1.0
* @date       2023-04-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_mouse_sensor.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

inline void Hal_Sensor_Pin_Reuse(void )
{
    SYS->GPC_MFP = SYS_GPC_MFP_PC0_GPIO | SYS_GPC_MFP_PC1_GPIO | SYS_GPC_MFP_PC2_GPIO | SYS_GPC_MFP_PC3_GPIO;
    SYS->ALT_MFP = SYS_ALT_MFP_PC0_GPIO | SYS_ALT_MFP_PC1_GPIO | SYS_ALT_MFP_PC2_GPIO| SYS_ALT_MFP_PC3_GPIO;

    PC->PMD = (PC->PMD & ~GPIO_PMD_PMD0_Msk) | (1 << GPIO_PMD_PMD0_Pos);
    PC->PMD = (PC->PMD & ~GPIO_PMD_PMD1_Msk) | (1 << GPIO_PMD_PMD1_Pos);
    PC->PMD = (PC->PMD & ~GPIO_PMD_PMD2_Msk) | (1 << GPIO_PMD_PMD2_Pos);
}

inline void Hal_Sensor_Cs_Enable(void )
{
    PC->DOUT &= ~(1 << 0);

    PB->DOUT |= (1 << 10);
}

inline void Hal_Sensor_Cs_Disable(void )
{
    PC->DOUT |= (1 << 0);
}


