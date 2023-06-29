#ifndef _DRV_FLASH_H
#define _DRV_FLASH_H

#include "hal_nuc123.h"

#define BLD_START_ADDR                 0x00000000
#define BLD_MAX_SIZE                   0x00001000
#define APP1_START_ADDR                (BLD_START_ADDR + BLD_MAX_SIZE)
#define APP1_MAX_SIZE                  0x00007800
#define APP2_START_ADDR                (APP1_START_ADDR + APP1_MAX_SIZE)
#define APP2_MAX_SIZE                  APP1_MAX_SIZE

#define USER_START_ADDR                0x0001f000
#define USER_MAX_SIZE                  0x00001000

#endif 

