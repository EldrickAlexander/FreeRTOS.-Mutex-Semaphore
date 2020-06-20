/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Author: Eldrick Alexander
 */
 

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

#include "semphr.h"				// Semaphore Library


/*******************************************************************************
* Definitions
******************************************************************************/
SemaphoreHandle_t xMutex;		// Mutex definition


/* Task priorities. */
#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void hello_task(void *pvParameters);
static void mood_task(void *pvParameters);
static void goodbye_task(void *pvParamters);


int main(void) {

	xMutex = xSemaphoreCreateMutex();	// Create Mutex

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    xTaskCreate(hello_task, "Hello_task", configMINIMAL_STACK_SIZE + 10, NULL, hello_task_PRIORITY, NULL);
    xTaskCreate(mood_task, "Mood_task", configMINIMAL_STACK_SIZE + 10, NULL, hello_task_PRIORITY, NULL);
    xTaskCreate(goodbye_task, "Goodbye_task", configMINIMAL_STACK_SIZE + 10, NULL, hello_task_PRIORITY, NULL);

    vTaskStartScheduler();

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;
        __asm volatile ("nop");
    }
    return 0 ;
}

static void hello_task(void *pvParameters)
{
    for (;;)
    {
    	xSemaphoreTake(xMutex, portMAX_DELAY);
    	PRINTF("Hello world.\r\n");
    	taskYIELD();
    	PRINTF("I´m working with FreeRTOS \n");
    	taskYIELD();
    	PRINTF("My name is 'Proyecto FreeRTOS.c'\n");
    	xSemaphoreGive(xMutex);
    	taskYIELD();
    }
}

static void mood_task(void *pvParameters)
{
    for (;;)
    {
    	xSemaphoreTake(xMutex, portMAX_DELAY);
        PRINTF("I'm fine, and you?.\r\n");
        taskYIELD();
        PRINTF("Today is a good day\n");
        taskYIELD();
        PRINTF("Better than  yesterday\n");
        xSemaphoreGive(xMutex);
        taskYIELD();
    }
}


static void goodbye_task(void *pvParameters)
{
    for (;;)
    {
    	xSemaphoreTake(xMutex, portMAX_DELAY);

        PRINTF("I have to go, bye!.\r\n");
        taskYIELD();
        PRINTF("See you later!\n");
        taskYIELD();
        PRINTF("Take care. \n");

        xSemaphoreGive(xMutex);
        taskYIELD();
    }
}
