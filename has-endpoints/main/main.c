/*
 * Copyright 2010-2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * Additions Copyright 2016 Espressif Systems (Shanghai) PTE LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
/**
 * @file subscribe_publish_sample.c
 * @brief simple MQTT publish and subscribe on the same topic
 *
 * This example takes the parameters from the build configuration and establishes a connection to the AWS IoT MQTT Platform.
 * It subscribes and publishes to the same topic - "test_topic/esp32"
 *
 * Some setup is required. See example README for details.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "esp_types.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "nvs.h"
#include "nvs_flash.h"

#include "aws_iot_config.h"
#include "aws_iot_log.h"
#include "aws_iot_version.h"
#include "aws_iot_mqtt_client_interface.h"

#include "nvs.h"
#include "nvs_flash.h"

#include "aws_iot_config.h"
#include "aws_iot_log.h"
#include "aws_iot_version.h"
#include "aws_iot_mqtt_client_interface.h"


#define GPIO_OUTPUT_IO_0    	2 // Board LED
#define GPIO_OUTPUT_IO_1    	14 // BJT atuador
#define GPIO_OUTPUT_PIN_SEL  	((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_1))

#define GPIO_INPUT_IO_0     	12
#define GPIO_INPUT_PIN_SEL 		(1ULL<<GPIO_INPUT_IO_0)

#define ESP_INTR_FLAG_DEFAULT 	0
#define N_MULTISAMPLES			4
#define N_STRING				50
#define DEFAULT_VREF    		1100     
#define AOP_GAIN	2   

static char mqtt_string[50];
uint8_t aws_flag = 0;
#define TEMPERATURE_SENSOR
#define OCCUPANCY_SENSOR
//#define RELAY_ACTUATOR
#define IOT

#if defined (IOT)
#include "aws_iot.h"
#endif

static esp_adc_cal_characteristics_t *adc_chars_temperature;
static esp_adc_cal_characteristics_t *adc_chars_internal_temperature;
static esp_adc_cal_value_t val_type_temperature;
static esp_adc_cal_value_t val_type_internal_temperature;
static xQueueHandle gpio_evt_queue = NULL;

void occupancy_sensor_init(void);
void temperature_sensor_init(void);
void internal_temperature_sensor_init(void);
void relay_actuator_init(void);

/*
 * Interrupts
 */
static void IRAM_ATTR gpio_isr_handler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

/*
 * FreeRTOS tasks
 */

static void occupancy_sensor_task(void* arg) {
    static uint32_t io_num;
    static uint32_t actual_io_value = 0;
    static uint32_t previous_io_value = 0;
    for(;;) {
		if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY))  {
			previous_io_value = actual_io_value;
			actual_io_value = gpio_get_level(io_num);
			if(actual_io_value != previous_io_value) {
				// Occupancy sensor change event
				//printf("GPIO[%d] intr, val: %d\r\n", io_num, gpio_get_level(io_num));
				if(actual_io_value) { // EVENT1 - Without presence	
					printf("Porta ABERTA!\r\n");
					sprintf(mqtt_string, "Porta ABERTA!");
				}
				else { // EVENT2 - Presence
					printf("Porta FECHADA\r\n");
					sprintf(mqtt_string, "Porta FECHADA!");
				}
				aws_flag = 1;
			} // if(actual_io_value == previous_io_value)
    	}
		vTaskDelay(100 / portTICK_RATE_MS);
    }
}

static void temperature_sensor_task(void* arg) {
	static uint16_t adc1_raw[N_MULTISAMPLES]; 
	static uint16_t adc1_md = 0;
	static uint32_t adc1_voltage_raw = 0;
	static uint32_t adc1_voltage_md = 0;
	float temperature = 0;

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_0);
	adc_chars_temperature = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    val_type_temperature = esp_adc_cal_characterize(
		ADC_UNIT_1, ADC_ATTEN_DB_0, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars_temperature);
	//esp_sleep_enable_timer_wakeup(5000000); // Tempo em us
	for(;;) {
		adc1_md = 0;
		for(uint8_t i = 0; i < N_MULTISAMPLES; i++) {
			adc1_raw[i] = adc1_get_raw(ADC1_CHANNEL_7);
			adc1_md += adc1_raw[i];
		}
		adc1_md /= N_MULTISAMPLES;
		adc1_voltage_raw = esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_7), adc_chars_temperature)/AOP_GAIN;
		adc1_voltage_md = esp_adc_cal_raw_to_voltage(adc1_md, adc_chars_temperature)/AOP_GAIN;
		temperature = ((float)(((float)adc1_voltage_md / 10.0) + (adc1_voltage_md / 100.0)) - 2);
		
		printf("ADC1_RAW=%umV\tADC1_MED=%umV\tT=%.1f°C\r\n", adc1_voltage_raw, adc1_voltage_md, temperature);
		sprintf(mqtt_string, "ADC1_RAW=%umV\tADC1_MED=%umV\tT=%.1f°C\r\n", adc1_voltage_raw, adc1_voltage_md, temperature);
		aws_flag = 1;
		vTaskDelay(5000 / portTICK_RATE_MS); // Para não ter problema de WDT na task idle
		
	}
}

static void internal_temperature_sensor_task(void* arg) {
	static uint16_t adc_hall_raw[N_MULTISAMPLES]; 
    static uint16_t adc_hall = 0; 
	static uint16_t adc_hall_md = 0; 
	static uint32_t adc_hall_voltage = 0;
	adc1_config_width(ADC_WIDTH_BIT_12);
	adc_chars_internal_temperature = calloc(1, sizeof(esp_adc_cal_characteristics_t));
	val_type_internal_temperature = esp_adc_cal_characterize(
	ADC_UNIT_1, ADC_ATTEN_DB_0, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars_internal_temperature);
	
	for(;;) {
		for(uint8_t i = 0; i < N_MULTISAMPLES; i++) {
			adc_hall = hall_sensor_read();
			adc_hall_md += adc_hall_raw[i];
		}
    	adc_hall_md /= N_MULTISAMPLES;
		adc_hall_voltage = esp_adc_cal_raw_to_voltage(adc_hall_md, adc_chars_internal_temperature);
		printf("ADC_HALL=%umV\t%u\r\n", adc_hall, adc_hall_voltage);
		sprintf(mqtt_string, "ADC_HALL=%umV\t%u\r\n", adc_hall, adc_hall_voltage);
		vTaskDelay(1000 / portTICK_RATE_MS);
		//aws_flag = 1;
	}
}


static void relay_actuator_task(void* arg) {
	
	
	for(;;) {

		/*
		gpio_set_level(GPIO_OUTPUT_IO_1, 0);
		vTaskDelay(300 / portTICK_RATE_MS);
		gpio_set_level(GPIO_OUTPUT_IO_1, 1);
		vTaskDelay(300 / portTICK_RATE_MS);
		*/
		vTaskDelay(10000 / portTICK_RATE_MS);

	}
}
 
/// App main ///
void app_main() {
	    // Initialize NVS.
#if defined (IOT)
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    initialise_wifi();
    xTaskCreatePinnedToCore(&aws_iot_task, "aws_iot_task", 9216, NULL, 5, NULL, 1);
#endif
	//internal_temperature_sensor_init();
#if defined (OCCUPANCY_SENSOR)
    occupancy_sensor_init();
#endif
#if defined (TEMPERATURE_SENSOR)
	temperature_sensor_init();
#endif
#if defined (RELAY_ACTUATOR)
	relay_actuator_init();
#endif
	// Suspends itself
	vTaskSuspend(NULL);
	// Should never get here
	printf("ERROR: app_main did not suspended itself!");
	while(1);
}

/*
 *
 * 
 */
void occupancy_sensor_init(void) {
	gpio_config_t gpio_conf;
	
	//disable interrupt
	gpio_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	//set as output mode
	gpio_conf.mode = GPIO_MODE_OUTPUT;
	//bit mask of the pins that you want to set,e.g.GPIO18/19
	gpio_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	//disable pull-down mode
	gpio_conf.pull_down_en = 0;
	//disable pull-up mode
	gpio_conf.pull_up_en = 0;
	//configure GPIO with the given settings
	gpio_config(&gpio_conf);
	
	//interrupt of rising edge
	gpio_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
	//bit mask of the pins, use GPIO4/5 here
	gpio_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
	//set as input mode
	gpio_conf.mode = GPIO_MODE_INPUT;
	//disable pull-up mode
	gpio_conf.pull_up_en = 1;
	//disable pull-up mode
	gpio_conf.pull_down_en = 0;
	gpio_config(&gpio_conf);

    //change gpio intrrupt type for one pin
    gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_ANYEDGE);
    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //install gpio isr service

		
	gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);

	//start gpio task
    xTaskCreate(occupancy_sensor_task, "OCUPPANCY_SENSOR_TASK", 2048, NULL, 1, NULL);
}

void temperature_sensor_init(void) {
	xTaskCreate(temperature_sensor_task, "TEMPERATURE_SENSOR_TASK", 2048, NULL, 1, NULL);
}

void internal_temperature_sensor_init(void) {
	xTaskCreate(internal_temperature_sensor_task, "INTERNAL_TEMPERATURE_SENSOR_TASK", 2048, NULL, 1, NULL);
}

 void relay_actuator_init(void) {
	gpio_config_t gpio_conf;
	
	//disable interrupt
	gpio_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	//set as output mode
	gpio_conf.mode = GPIO_MODE_OUTPUT;
	//bit mask of the pins that you want to set,e.g.GPIO18/19
	gpio_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	//disable pull-down mode
	gpio_conf.pull_down_en = 0;
	//disable pull-up mode
	gpio_conf.pull_up_en = 0;
	//configure GPIO with the given settings
	gpio_config(&gpio_conf);

	gpio_set_level(GPIO_OUTPUT_IO_1, 0);
	//start gpio task
    //xTaskCreate(relay_actuator_task, "RELAY_ACTUATOR_TASK", 2048, NULL, 1, NULL);
	
	
}