/**
 * @file app_priv.h
 * @brief Header file containing shared declarations, default values, and function prototypes
 *        used across the application (driver and main logic).
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <esp_rmaker_core.h>

// Default values for DHT11 sensor readings
#define DEFAULT_TEMPERATURE 0.0
#define DEFAULT_HUMIDITY    0.0

// Sensor data reporting interval in seconds
#define REPORTING_PERIOD    5

// Extern declarations for devices to be accessible across files
extern esp_rmaker_device_t *temp_sensor_device;
extern esp_rmaker_device_t *humidity_sensor_device;
extern esp_rmaker_device_t *switch_device;

// Function prototypes
void app_driver_init(void);
float app_get_current_temperature();
float app_get_current_humidity();
