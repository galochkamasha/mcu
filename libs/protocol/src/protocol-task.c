#include "protocol-task.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>

static api_t* api = NULL;  // Исправлено: {0} -> NULL
static int commands_count = 0;

void protocol_task_init(api_t* commands)
{
    commands_count = 0;
    api = commands;
    int i = 0;
    while (api[i].command_name != NULL)
    {
        commands_count += 1;
        i++;
    }
}

void protocol_task_handle(char* command_string)
{
    if (!command_string)
    {
        return;
    }

    char* command_name = command_string;
    const char* command_args = NULL;

    char* space_symbol = strchr(command_string, ' ');

    if (space_symbol)
    {
        *space_symbol = '\0';
        command_args = space_symbol + 1;
    }
    else
    {
        command_args = "";
    }
    
    // Отладка (можно убрать после проверки)
    printf("Command name: '%s' | Command args: '%s'\n", command_name, command_args);
    
    for (int i = 0; i < commands_count; i++)
    {
        if (strcmp(command_name, api[i].command_name) == 0)
        {
            // ИСПРАВЛЕНО: используем command_callback, а не callback
            api[i].command_callback(command_args);
            return;
        }
    }
    printf("Command not found\n");
    return;
}