#include <stdio.h>          // GPIO functionality
#include "pico/stdlib.h"    // core subset of Pico SDK libraries used by most executables along with additional utility methods
#include "FreeRTOS.h"       // FreeRTOS API access 
#include "semphr.h"         // RTOS semaphore functionality
#include "task.h"           // RTOS task functionality
#include "GPIO.hpp"         // GPIO setup method
#include "Modbus.h"         // Modbus functionality 
#include <array>            // standard template library (STL) - std::array class is a wrapper around traditional C-style array

// array of pointers to structures of type modbusHandler_t used "handle" the Modbus communication protocol
modbusHandler_t ModbusHandle1;
// unsigned interger array of length [X] (2 bytes of memory each; value range: 0 - 65,535)
uint16_t ModbusDataset1[8];
// gpio pin # used to set the relay state
#define RELAY_CONTROL_PIN 22

// creates static instance(s) of the C++ GPIO_Pin class named xxxPin, connects it to a pin, and sets the pin direction
static pico_cpp::GPIO_Pin ledPin(PICO_DEFAULT_LED_PIN,pico_cpp::PinType::Output);
static pico_cpp::GPIO_Pin relayPin(RELAY_CONTROL_PIN,pico_cpp::PinType::Output);

void vTaskCode( void * pvParameters )
{
    for( ;; )
    {       
        // sets the state of the pin by performing a bitwise AND (&) operation on the bit(s) specified (value specified in hex: 0xXX)
        gpio_put(PICO_DEFAULT_LED_PIN, ModbusDataset1[0] & 0x01); 
        gpio_put(RELAY_CONTROL_PIN, ModbusDataset1[0] & 0x02); 
        vTaskDelay(100);                                
    }
}

#define UART0_TX_PIN 0
#define UART0_RX_PIN 1

void initSerial()
{
    uart_init(uart0, 921600);
    gpio_set_function(UART0_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART0_RX_PIN, GPIO_FUNC_UART);
    uart_set_fifo_enabled(uart0, false);
}

int main() {

    BaseType_t xReturned;
    TaskHandle_t xHandle = NULL;

    initSerial();

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
                    vTaskCode,          // function that implements the task 
                    "Modbus task",      // text name for the task 
                    512,                // stack size in words (not bytes) 
                    ( void * ) 1,       // parameter passed into the task
                    tskIDLE_PRIORITY,   // priority at which the task is created 
                    &xHandle );         // pass out task handle 

    // Master or Slave?
    ModbusHandle1.uModbusType = MB_SLAVE;  
    // use UART port #                                       
    ModbusHandle1.port = uart0;                                                             
    // slave address (if master, set to 0)
    ModbusHandle1.u8id = 1;
    // modbus master response timeout             
    ModbusHandle1.u16timeOut = 1000;
    // disables the RS-485 flow control requirement (transceiver uses automatic flow control)      
    ModbusHandle1.EN_Port = NULL;  
    // set pin if manually flow control is required         
    // ModbusHandle1.EN_Pin = X;
    // Modbus handle register array
    ModbusHandle1.u16regs = ModbusDataset1;
    // assigns the # of elements in the dataset array to the u16size member of the modbus handler
    ModbusHandle1.u16regsize = sizeof(ModbusDataset1)/sizeof(ModbusDataset1[0]);
    // sets the hardware type used by the modbus stack; assigns the value USART_HW to the xTypeHW member of the modbus handler
    ModbusHandle1.xTypeHW = USART_HW;

    // initialize the handle - passes a pointer to the modbus handler object to the modbus initialization function; functions access the object directly
    ModbusInit(&ModbusHandle1);

    // start capturing traffic on the handle
    ModbusStart(&ModbusHandle1);

    vTaskStartScheduler();
    while(1)
    {
        // we should never get here
        configASSERT(0);
    }
}