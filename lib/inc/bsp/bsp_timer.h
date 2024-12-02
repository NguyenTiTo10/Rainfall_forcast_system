#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "stdint.h"
#include <rom/ets_sys.h>
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#ifndef BSP_TIMER_H
#define BSP_TIMER_H

void bsp_delay (uint32_t time);
void bsp_timer_ets_delay_us (uint32_t time_us);


#endif