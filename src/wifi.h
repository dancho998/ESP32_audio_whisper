#include "freertos/FreeRTOS.h"
#ifndef WIFI_H
#define WIFI_H
extern EventGroupHandle_t s_wifi_event_group;
#define WIFI_CONNECTED_BIT BIT0
void wifi_init(void);
#endif // WIFI_H