# Toggle LED 
[index](../README.md)

Now we can use the MrT abstraction layer for stm32. We are going to blink the LED on the board just as a basic example. Add the following code snippets:

**main.c:26** (in the USER CODE INCLUDES section)
```c
#include "Platforms/Common/mrt_platform.h" /* This will include the stm32 layer based on the MRT_PLATFORM symbol we set*/
```
<br>


**main.c:108** ( in the USER CODE WHILE section)
```c
/** STM32 HAL does not have a type for pins, all of its functions use (port,pin). MRT_GPIO() is a macro that wraps them 
  * This is so that device drivers have a single struct for pins 
  */
MRT_GPIO_WRITE(MRT_GPIO(LED_GRN),HIGH);     //set the pin high
MRT_DELAY_MS(1000);                         //wait 1000 ms
MRT_GPIO_WRITE(MRT_GPIO(LED_GRN),LOW);      //set the pin low
MRT_DELAY_MS(1000);                         //wait 1000 ms

```

Now build and run the project, the green LED on the board should blink!

[Next Step](create-device.md)