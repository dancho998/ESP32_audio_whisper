#include "freertos/FreeRTOS.h"
#include "mic.h"
#include "wifi.h"
#include <stdio.h>
#include <string.h>
#include "freertos/task.h"
#include "driver/i2s.h"
#include "esp_system.h"
#include "esp_log.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"

#define I2S_NUM         (0)
#define SAMPLE_RATE     (52000)
#define ADC_INPUT       (ADC1_CHANNEL_7) // GPIO35
#define AUDIO_CHUNK_LEN (8192)
#define SERVER_IP       "0.0.0.0" //change to your IP
#define SERVER_PORT     12345

static const char *TAG = "MIC_TASK";
static uint16_t audio_buffer[AUDIO_CHUNK_LEN/2];

static void mic_task(void *pvParameters)
{
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN,
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = 16,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S_MSB,
        .intr_alloc_flags = 0,
        .dma_buf_count = 4,
        .dma_buf_len = 1024,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0
    };
    ESP_ERROR_CHECK(i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL));
    ESP_ERROR_CHECK(i2s_set_adc_mode(ADC_UNIT_1, ADC_INPUT));
    ESP_ERROR_CHECK(i2s_adc_enable(I2S_NUM));

    ESP_LOGI(TAG, "Waiting for WiFi connection before streaming...");
    while (s_wifi_event_group == NULL) {
    vTaskDelay(pdMS_TO_TICKS(10));
    }
    xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    ESP_LOGI(TAG, "WiFi connected! Proceeding to TCP socket init...");

    struct sockaddr_in server_addr = {0};
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        vTaskDelete(NULL);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    ESP_LOGI(TAG, "Connecting to PC %s:%d", SERVER_IP, SERVER_PORT);
    int err = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (err != 0) {
        ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
        close(sock);
        vTaskDelete(NULL);
    }
    ESP_LOGI(TAG, "Connected! Streaming audio...");

    while (1) {
        size_t bytes_read = 0;
        esp_err_t res = i2s_read(I2S_NUM, (void*)audio_buffer, AUDIO_CHUNK_LEN, &bytes_read, portMAX_DELAY);
        if (res != ESP_OK) {
            ESP_LOGE(TAG, "i2s_read failed: %s", esp_err_to_name(res));
            continue;
        }
        int sent = 0;
        uint8_t* buffer_bytes = (uint8_t*)audio_buffer;
        while (sent < bytes_read) {
            int r = send(sock, buffer_bytes + sent, bytes_read - sent, 0);
            if (r < 0) {
                ESP_LOGE(TAG, "Socket send error. Closed.");
                close(sock);
                vTaskDelete(NULL);
            }
            sent += r;
        }
    }
    // (Never reached)
}

void mic_start(void) {
    xTaskCreate(mic_task, "mic_task", 8192, NULL, 5, NULL);
}