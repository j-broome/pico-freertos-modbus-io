#ifndef THIRD_PARTY_MODBUS_LIB_CONFIG_MODBUSCONFIG_H_
#define THIRD_PARTY_MODBUS_LIB_CONFIG_MODBUSCONFIG_H_

/* Uncomment the following line to enable support for Modbus RTU over USB CDC profile. */
//#define ENABLE_USB_CDC 1

/* Uncomment the following line to enable support for Modbus TCP. Only tested for Nucleo144-F429Z. */
//#define ENABLE_TCP 1

/* RTU Timeouts based on channel baud rate
Baud Rate   RTU Timeout(uS) (3.5 bytes)     RTU Timeout (counts at 1kHz)
  9600                3646                      5
  19200               1823                      3
  38400               911                       2
  76800               456                       2
  115200              304                       2
  256000              137                       2
*/

#define T35  2              // timer T35 period (in ticks) for end frame detection.
#define MAX_BUFFER  128	    // max size for the communication buffer in bytes
#define TIMEOUT_MODBUS 1000 // master query timeout (in ticks)
#define MAX_M_HANDLERS 2    // max # of modbus handlers that can work concurrently
#define MAX_TELEGRAMS 2     // max # of telegrams in master queue

#endif /* THIRD_PARTY_MODBUS_LIB_CONFIG_MODBUSCONFIG_H_ */