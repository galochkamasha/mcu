#include "stdio-task/stdio-task.h"
#include "stdio.h"
#include "stdlib.h"
#include "pico/stdlib.h"
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"
#include "adc-task/adc-task.h"

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

void version_callback(const char* args)
{
	printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}

void led_on_callback(const char* args)
{
	printf("led turned on\n");
	led_task_state_set(LED_STATE_ON);
}

void led_off_callback(const char* args)
{
	printf("led turned off\n");
	led_task_state_set(LED_STATE_OFF);

}

void led_blink_callback(const char* args)
{
	printf("led started blinking\n");
	led_task_state_set(LED_STATE_BLINK);
}

void get_adc_callback(const char* args)
{
	float voltage_V = voltage_measure();
	printf("%f\n", voltage_V);
}

void get_temp_callback(const char* args)
{
	float temp_C = temp_measure();
	printf("%f\n", temp_C);
}

void tm_start_callback(const char* args)
{
	adc_task_set_state(ADC_TASK_STATE_RUN);
}

void tm_stop_callback(const char* args)
{
	adc_task_set_state(ADC_TASK_STATE_IDLE);
}

api_t device_api[] =
{
	{"version", version_callback, "get device name and firmware version"},
	{"on", led_on_callback, "make led to turn on"},
	{"off", led_off_callback, "make led to turn off"},
	{"blink", led_blink_callback, "make led blink"},
	{"get_adc", get_adc_callback, "measure adc's voltage"},
	{"get_temp", get_temp_callback, "measure temperature"},
	{"tm_start", tm_start_callback, "start measuring V and T"},
	{"tm_stop", tm_stop_callback, "stop measuring V and T"},
	{NULL, NULL, NULL}
};

int main()
{
	stdio_task_init();
	stdio_init_all();
	protocol_task_init(device_api);
	led_task_init();
	adc_task_init();

	while(1)
	{
		char* command = stdio_task_handle();
		led_task_handle();
		protocol_task_handle(command);
		adc_task_handle();
	}
}