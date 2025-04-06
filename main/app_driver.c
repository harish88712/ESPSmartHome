/**
 * @file app_driver.c
 * @brief Handles hardware-level control including GPIO initialization, DHT11 sensor reading,
 *        LED control, and reset button configuration. Periodically updates RainMaker with 
 *        sensor data and raises alerts when temperature exceeds 30°C.
 */

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <sdkconfig.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_types.h> 
#include <esp_rmaker_standard_params.h> 
#include <driver/gpio.h>
#include "app_reset.h"
#include "app_priv.h"
#include "dht.h"  
#include "esp_log.h"

// GPIO pin definitions
#define LED_GPIO GPIO_NUM_10         // Output GPIO for LED
#define BUTTON_GPIO 5                // GPIO for reset button
#define SWITCH_GPIO GPIO_NUM_4       // Optional GPIO for switch
#define DHT_PIN GPIO_NUM_2           // GPIO connected to DHT11 sensor
#define DHT_TYPE DHT_TYPE_DHT11      // DHT sensor type

// Button press duration definitions (in seconds)
#define WIFI_RESET_BUTTON_TIMEOUT 3
#define FACTORY_RESET_BUTTON_TIMEOUT 11

// Timer for periodic sensor reading
static TimerHandle_t sensor_timer;

// Global variables to store latest temperature and humidity values
static float g_temperature = DEFAULT_TEMPERATURE;
static float g_humidity = DEFAULT_HUMIDITY;

/**
 * @brief Timer callback function that reads DHT11 sensor and updates RainMaker cloud.
 */
static void app_sensor_update(TimerHandle_t handle)
{
    if (dht_read_float_data(DHT_TYPE, DHT_PIN, &g_humidity, &g_temperature) == ESP_OK) {
        ESP_LOGI("DHT", "Temperature: %.1f°C, Humidity: %.1f%%", g_temperature, g_humidity);
        
        // Update temperature to RainMaker
        esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_type(temp_sensor_device, ESP_RMAKER_PARAM_TEMPERATURE),
            esp_rmaker_float(g_temperature));

        // Update humidity to RainMaker
        esp_rmaker_param_t *humidity_param = esp_rmaker_device_get_param_by_name(humidity_sensor_device, "Humidity");
        if (humidity_param) {
            esp_rmaker_param_update_and_report(humidity_param, esp_rmaker_float(g_humidity));
        } else {
            ESP_LOGE("RainMaker", "Failed to get Humidity parameter!");
        }

        // Raise alert if temperature is too high
        if (g_temperature > 30) {
            char alert_msg[ESP_RMAKER_MAX_ALERT_LEN];
            snprintf(alert_msg, sizeof(alert_msg), "🔥 High Temperature Alert: %.1f°C!", g_temperature);
            ESP_LOGW("ALERT", "%s", alert_msg);
            esp_rmaker_raise_alert(alert_msg);
        }
    } else {
        ESP_LOGE("DHT", "Failed to read from DHT sensor!");
    }
}

// Accessor functions to get current temperature and humidity
float app_get_current_temperature() { return g_temperature; }
float app_get_current_humidity() { return g_humidity; }

/**
 * @brief Initializes the timer to read sensor data periodically.
 */
esp_err_t app_sensor_init(void)
{
    sensor_timer = xTimerCreate("app_sensor_update_tm", pdMS_TO_TICKS(REPORTING_PERIOD * 1000),
                                pdTRUE, NULL, app_sensor_update);
    if (sensor_timer) {
        xTimerStart(sensor_timer, 0);
        return ESP_OK;
    }
    return ESP_FAIL;
}

/**
 * @brief Initializes GPIOs, sensor update timer, and reset button functionality.
 */
void app_driver_init()
{
    // Initialize LED output pin
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_GPIO, 0);  // Turn off LED initially

    // Start sensor reading timer
    app_sensor_init();

    // Register button for Wi-Fi and factory reset
    app_reset_button_register(app_reset_button_create(BUTTON_GPIO, BUTTON_ACTIVE_LOW),
                              WIFI_RESET_BUTTON_TIMEOUT, FACTORY_RESET_BUTTON_TIMEOUT);
}
