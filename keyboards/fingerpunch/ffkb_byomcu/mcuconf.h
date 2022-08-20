#pragma once

#include_next "mcuconf.h"

#undef STM32_SPI_USE_SPI1
#define STM32_SPI_USE_SPI1 TRUE

#undef STM32_SPI_USE_SPI2
#define STM32_SPI_USE_SPI2 FALSE

#undef STM32_ST_USE_TIMER
#define STM32_ST_USE_TIMER 5