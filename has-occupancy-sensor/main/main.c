#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_types.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define GPIO_OUTPUT_IO_0    2
#define GPIO_OUTPUT_IO_1    15
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_1))
#define GPIO_INPUT_IO_0     16
#define GPIO_INPUT_PIN_SEL  (1ULL<<GPIO_INPUT_IO_0)
#define ESP_INTR_FLAG_DEFAULT 0
#define N_MULTISAMPLES	10
#define DEFAULT_VREF    1100        

static esp_adc_cal_characteristics_t *adc_chars;
static esp_adc_cal_value_t val_type;
static xQueueHandle gpio_evt_queue = NULL;
void occupancy_sensor_init(gpio_config_t* gpio_config);

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void check_efuse(void)
{
}

static void occupancy_sensor_task(void* arg)
{
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
				}
				else { // EVENT2 - Presence
					printf("Porta FECHADA\r\n");
				}
			} // if(actual_io_value == previous_io_value)
    	}
		vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
static void read_adc_sensor(void* arg)
{
	check_efuse();
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
	adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, adc_chars);
    
    static uint16_t adc1_raw[N_MULTISAMPLES]; 
	static uint16_t adc1_md = 0;
	static uint32_t adc1_voltage = 0;

for(;;) {
		adc1_md = 0;
		for(uint8_t i = 0; i < N_MULTISAMPLES; i++) {
			adc1_raw[i] = adc1_get_raw(ADC1_CHANNEL_7);
			adc1_md += adc1_raw[i];
		}
		adc1_md /= N_MULTISAMPLES;
		adc1_voltage = esp_adc_cal_raw_to_voltage(adc1_md, adc_chars);
		printf("ADC1_MD=%u\tADC1_V=%u\r\n\r\n", adc1_md, adc1_voltage);
		vTaskDelay(1000 / portTICK_RATE_MS);
	}
}

static void read_hall_sensor(void* arg) {
	
    adc1_config_width(ADC_WIDTH_BIT_12);
    static uint16_t adc_hall = 0; 
	static uint32_t adc1_voltage = 0;
	
	for(;;) {
    	adc_hall = hall_sensor_read();
		adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
   		val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, adc_chars);

		printf("ADCHALL=%u\t%u\r\n", adc_hall, adc1_voltage);
		vTaskDelay(2000 / portTICK_RATE_MS);
	}
	for(;;) {
		vTaskDelay(2000 / portTICK_RATE_MS);
	}
}
 

void app_main()
{
    gpio_config_t io_conf;
    occupancy_sensor_init(&io_conf);
	xTaskCreate(read_adc_sensor, "READ_ADC1_TASK", 2048, NULL, 1, NULL);
	xTaskCreate(read_hall_sensor, "READ_HALL_TASK", 2048, NULL, 1, NULL);
    /*
    for(;;) {
        //printf("cnt: %d\n", cnt++);
        vTaskDelay(5000 / portTICK_RATE_MS);
    }
    */
}

void occupancy_sensor_init(gpio_config_t* gpio_conf) {
	//disable interrupt
	gpio_conf->intr_type = GPIO_PIN_INTR_DISABLE;
	//set as output mode
	gpio_conf->mode = GPIO_MODE_OUTPUT;
	//bit mask of the pins that you want to set,e.g.GPIO18/19
	gpio_conf->pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	//disable pull-down mode
	gpio_conf->pull_down_en = 0;
	//disable pull-up mode
	gpio_conf->pull_up_en = 0;
	//configure GPIO with the given settings
	gpio_config(gpio_conf);

	//interrupt of rising edge
	gpio_conf->intr_type = GPIO_PIN_INTR_POSEDGE;
	//bit mask of the pins, use GPIO4/5 here
	gpio_conf->pin_bit_mask = GPIO_INPUT_PIN_SEL;
	//set as input mode
	gpio_conf->mode = GPIO_MODE_INPUT;
	//disable pull-up mode
	gpio_conf->pull_up_en = 0;
	//disable pull-up mode
	gpio_conf->pull_down_en = 0;
	gpio_config(gpio_conf);

    //change gpio intrrupt type for one pin
    gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_ANYEDGE);
    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(occupancy_sensor_task, "OCP_SENSOR_TASK", 2048, NULL, 1, NULL);
    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);
}
