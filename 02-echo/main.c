#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

const uint LED_PIN = 25;

int main() {
    
    stdio_init_all();


    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, false);  
  
    sleep_ms(2000);
    
  
    printf("USB чтение символов инициализировано\n");
    printf("Ожидание ввода...\n\n");
    printf("Команды: 'e' - включить LED, 'd' - выключить LED, 'v' - версия\n");
   
    while (true) {
     
        char symbol = getchar();
        
      
        printf("received char: %c [ ASCII code: %d ]\n", symbol, symbol);
        switch(symbol)
        {
            case 'e':  // Включить светодиод
                gpio_put(LED_PIN, true);
                printf("led enable done\n");
                break;
                
            case 'd':  // Выключить светодиод
                gpio_put(LED_PIN, false);
                printf("led disable done\n");
                break;
                
            case 'v':  // Вывести версию
                printf("Device: %s, Version: %s\n", DEVICE_NAME, DEVICE_VRSN);
                break;
                
            default:  
                break;
        }

    }
    
    return 0;
}