#include "hardware/adc.h"
#include "stdio.h"
#include "pico/stdlib.h"
#include "adc-task.h"

const uint LEG_ADC = 26;
const uint ADC_CHANNEL = 0;
const uint ADC_CHANNEL_TEMP = 4;
adc_task_state_t adc_state = ADC_TASK_STATE_IDLE;
uint64_t adc_next_measure_time = 0;
uint ADC_TASK_MEAS_PERIOD = 100000; // 100 мс

void adc_task_init()
{
    adc_init();
    adc_gpio_init(LEG_ADC);
    adc_set_temp_sensor_enabled(true);
}

float voltage_measure()
{
    adc_select_input(ADC_CHANNEL);
    uint16_t voltage_count = adc_read();
    return voltage_count/4095.0 * 3.3;
}

float temp_measure()
{
    adc_select_input(ADC_CHANNEL_TEMP);
    uint16_t temp_counts = adc_read();
    float temp_V = temp_counts/4095.0 * 3.3;
    return 27.0f - (temp_V - 0.706f) / 0.001721f;
}

void adc_task_set_state(adc_task_state_t state)
{
    adc_state = state;
    if (state == ADC_TASK_STATE_RUN) {
        
        adc_next_measure_time = time_us_64();
    }
}

void adc_task_handle()
{
    if (adc_state == ADC_TASK_STATE_RUN)
    {
        uint64_t current_time = time_us_64();
        
        
        if (current_time >= adc_next_measure_time)
        {
            float v = voltage_measure();
            float t = temp_measure();
            printf("%f %f\n", v, t);
            
            
            adc_next_measure_time = current_time + ADC_TASK_MEAS_PERIOD;
        }
    }
}