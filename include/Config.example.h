#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_SSID       "YOUR_WIFI_NAME"
#define WIFI_PASSWORD   "YOUR_WIFI_PASSWORD"

#define APP_KEY         "YOUR_SINRIC_APP_KEY"
#define APP_SECRET      "YOUR_SINRIC_APP_SECRET"
#define DEVICE_ID       "YOUR_DEVICE_ID"

#define RELAY_PIN       D1
#define LED_PIN         D4

#define WEATHER_API_KEY "YOUR_OPENWEATHER_API_KEY"
#define WEATHER_CITY    "Siliguri"
#define WEATHER_COUNTRY "IN"

#define OLED_SDA        D2
#define OLED_SCL        D5
#define OLED_WIDTH      128
#define OLED_HEIGHT     64
#define OLED_ADDRESS    0x3C

#define WEATHER_INTERVAL 600000UL
#define LIGHT_DISPLAY_TIME 3000UL

#endif