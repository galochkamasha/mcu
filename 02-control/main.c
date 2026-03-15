#include "stdio-task/stdio-task.h"
#include "stdio.h"
#include "stdlib.h"
#include "pico/stdlib.h"
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

void command_help_callback(const char* args);

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

void led_blink_set_period_ms_callback(const char* args)
{
	uint32_t period_ms = 0;
	sscanf(args, "%u", &period_ms);
	if (period_ms==0) {printf("Error: blinking period cannot be 0ms"); return;}
	led_task_set_blink_period_ms(period_ms);
}

void command_mem_callback(const char* args)
{
	uint32_t address;
	sscanf(args, "%x", &address);
	uint32_t *ptr = (uint32_t*)address;
	printf("0x%08X\n", *ptr);

}

void command_wmem_callback(const char* args)
{
	uint32_t address, znach;
	sscanf(args, "%x %x", &address, &znach);
	uint32_t *ptr = (uint32_t*)address;
	*ptr=znach;
}

api_t device_api[] =
{
	{"version", version_callback, "get device name and firmware version"},
	{"on", led_on_callback, "make led to turn on"},
	{"off", led_off_callback, "make led to turn off"},
	{"blink", led_blink_callback, "make led blink"},
	{"set_period", led_blink_set_period_ms_callback, "changing blinking period"},
	{"command_help", command_help_callback, "print commands description"},
	{"mem", command_mem_callback, "print smth located at that address"},
	{"wmem", command_wmem_callback, "поменять значение в заданной ячейке на заданное"},
	{NULL, NULL, NULL},
};


void command_help_callback(const char* args)
{
	int i = 0;
	while (device_api[i].command_name != NULL)
	{
		printf("Команда %s: %s\n", device_api[i].command_name, device_api[i].command_help);
		i++;
	}
}


int main()
{
	stdio_task_init();
	stdio_init_all();
	protocol_task_init(device_api);
	led_task_init();

	while(1)
	{
		char* command = stdio_task_handle();
		led_task_handle();
		protocol_task_handle(command);
	}
}