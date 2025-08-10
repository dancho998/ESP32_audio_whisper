#include "freertos/FreeRTOS.h"
#include "wifi.h"
#include "mic.h"

void app_main(void)
{
    wifi_init();
    mic_start();
}