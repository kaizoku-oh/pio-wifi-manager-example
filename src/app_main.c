#include <esp_log.h>
#include <esp_wifi.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "wifi_manager.h"

#define TAG "APP_MAIN"

static void _app_task(void *pvParameter)
{
  while(1)
  {
    ESP_LOGI(TAG, "Running application");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

static void _wifi_manager_sta_got_ip_cb(void *pvParameter)
{
  ip_event_got_ip_t *pstIPAddr;
  char tcIpString[sizeof("xxx.xxx.xxx.xxx")];

  pstIPAddr = (ip_event_got_ip_t*)pvParameter;
  /* transform IP to human readable string */
  esp_ip4addr_ntoa(&pstIPAddr->ip_info.ip, tcIpString, IP4ADDR_STRLEN_MAX);
  ESP_LOGI(TAG, "I have a connection and my IP is %s!", tcIpString);
}

static void _wifi_manager_sta_discon_cb(void *pvParameter)
{
  wifi_event_sta_disconnected_t *pstWifiDiscon;

  pstWifiDiscon = (wifi_event_sta_disconnected_t*)pvParameter;
  ESP_LOGW(TAG, "Station disconnected with reason code: %d", pstWifiDiscon->reason);
}

void app_main(void)
{
  /* start the wifi manager */
  wifi_manager_start();
  /* register a callback to be called when station is connected to AP */
  wifi_manager_set_callback(WM_EVENT_STA_GOT_IP, &_wifi_manager_sta_got_ip_cb);
  /* register a callback to be called when station is disconnected from AP */
  wifi_manager_set_callback(WM_EVENT_STA_DISCONNECTED, &_wifi_manager_sta_discon_cb);
  /* start the app task */
  xTaskCreate(&_app_task, "app_task", 2048, NULL, 5, NULL);
}