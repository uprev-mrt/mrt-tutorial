

# Create a PolyPacket Service 
[index](../README.md)

Now That we have a working device driver, lets create a messaging protocol so we can ask the device for data over the com port. 

first we will need to add in some more MrT modules to support polypacket. back out to your projects root directory and open mrt-config again:

```bash
cd /path/to/project/root
mrt-config MrT
```

Select the following modules to import :
- Utilities/PolyPacket
- Utilities/JSON
- Utilities/COBS 

Once they are imported, create your protocol template:

```bash
poly-make -t my_protocol
```

There should now be a file named ```my_protocol.yml``` in the root of your project. You can keep this wherever you want, but I find it handy to have it in the root of the project when debugging. 


now modify the file to match the my_protocol.yml in the doc folder. For a detailed eplanation of the document reference [PolyPacket.wiki/Defining-a-protocol](https://github.com/up-rev/PolyPacket/wiki/Defining-a-Protocol)

Once the descriptor is filled out, create a directory for your service, and then generate your service with an application layer:

```bash
mkdir MrT/Modules/my_service
poly-make -i my_protocol.yml -a -o MrT/Modules/my_service/
```
>*Add "-d doc " to create an ICD in the doc folder*

This will generate 4 files: 
- **my_protocolService.h** - header for service, you should never need to edit this
- **my_protocolService.c** - source for service, you should never need to edit this
- **app_my_protocol.h** - header for application layer
- **app_my_protocol.c** - source for application layer, this is where you will fill out packet handlers

**First include the service:**

main.c:28:
```c
#include "my_service/app_my_protocol.h"
```

**Next intialize the app layer**

main.c:118:
```c
  /* OR we could use the configuration we created with: HTS_LOAD_CONFIG_AUTO_1HZ(&hts) */

  app_my_protocol_init(&huart1); /* initialize the app layer and give it a uart interface */

  /* For the UART RX, we are going to use some low level tricks for the stm32, because their HAL layer is not great at receiving
   * unkown lengths of data. This will set us up with an interrupt everytime a new byte comes in. its just cleaner and less hassle
   */
  UART_MASK_COMPUTATION(&huart1);									/* Sets Uart1's internal data mask based on STMCUBE configuration*/
  SET_BIT(huart1.Instance->CR1, USART_CR1_PEIE | USART_CR1_RXNEIE); /* Enable the interrupts for STM32 UART receive */

  /* USER CODE END 2 */
```

**Then add a call to process our service in the main loop**

main.c:118:
```c
/* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    

    /* Every 500 ms see if new data is ready, and read it */
    MRT_EVERY( 100, ticks)   /* convenience macro for systick timing*/
    {
      if(hts_check_flag(&hts, &hts.mStatus, ( HTS_STATUS_TEMP_READY | HTS_STATUS_HUM_READY )  )) /*wait until both flags are set */
      {
        temperature = hts_read_temp(&hts);
        humidity = hts_read_humidity(&hts);
      } 
    }
    app_my_protocol_process(); /* process our service*/
    ticks++;
    MRT_DELAY_MS(10);
    

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
```
<br>

**Now lets use the uart interrupt to feed our service**

stm32l4xx_it.c:26
```c
#include "my_service/app_my_protocol.h"
```

stm32l4xx_it.c:201
```c
/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

    /* If RX-Not-Empty flag is set, then we have a byte of data */
    if(huart1.Instance->ISR & UART_FLAG_RXNE)
    {
      uint8_t data = (uint8_t)(huart1.Instance->RDR & (uint8_t)huart1.Mask); /* Mask Off Data */
      mp_service_feed(0, &data ,1); /* feed the byte to our service */
    }

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}
```

**Since we are using the interrupt we can disable the uart_read in our application layer:**

**app_my_protocol.c:66** - *comment out iface0_read()*
```c
void app_my_protocol_process()
{
  /* read in new data from iface 0*/
  // iface0_read();

  /* process the actual service */
  mp_service_process();

}
```


<br>


Before we add anything else, lets test our service. Find the com port that the device is on in device manager. in my case it is COM3

>for WSL, COM ports are mapped to /dev/ttyS\<Port Number>

**open the poly-packet interpretter:**
```bash
poly-packet -i my_protocol.yml 
```

**inside of poly-packet** *connect over serial with a baud of 115200*
```bash
connect serial:/dev/ttyS3:115200
ping
```
> note: every protocol is built with a ping and ack packet

You should see your 'Ping packet go out, and an ack returned'

```bash
______     _      ______          _        _   
| ___ \   | |     | ___ \        | |      | |  
| |_/ /__ | |_   _| |_/ /_ _  ___| | _____| |_ 
|  __/ _ \| | | | |  __/ _` |/ __| |/ / _ \ __|
| | | (_) | | |_| | | | (_| | (__|   <  __/ |_ 
\_|  \___/|_|\__, \_|  \__,_|\___|_|\_\___|\__|    [my_protocol protocol]
              __/ |                            
             |___/                             

 Port Opened : /dev/ttyS3

 --> { "packetType" : "Ping", "icd" : 3174876862}
 <-- { "packetType" : "Ack"}
```

>tip: inside poly-packet, you can press tab to see available packets to send

[Next Step](custom-proto.md)