# Customize the service 
[index](../README.md)

Now that we have our service working, lets make it do something useful. When we created the protocol description, we set up a poly_struct named 'Device'. We are going to use this to store and serve information about the device.


First we will make changes to our application layer, we will add our struct, and clean up some un-used sections while we are there 

**app_my_protocol.c:8**
```bash
/***********************************************************
        Application Layer
***********************************************************/

#include "app_my_protocol.h"
mrt_uart_handle_t ifac0;

mp_struct_t myDevice; /* create device struct for storing/serving our data */

static inline HandlerStatus_e iface0_write(uint8_t* data, int len)
{
  /* Place code for writing bytes on interface 0 here */
  MRT_UART_TX(ifac0, data, len, 10);

  return PACKET_SENT;
}


/*******************************************************************************
  App Init/end
*******************************************************************************/
void app_my_protocol_init(mrt_uart_handle_t uart_handle)
{
  /* Set ifac0 to uart handle, this can use any peripheral, but uart is the most common case */
  ifac0 = uart_handle; //set interface to uart handle

  //initialize service
  mp_service_init(1,16);

  mp_struct_build(&myDevice, MP_STRUCT_DEVICE); /* builds the generic poly_struct into a Device struct */
  mp_setDeviceName(&myDevice, "Jerry");         /* set the 'Name' field of the device struct */

  mp_service_register_bytes_tx(0, iface0_write);

}
```

**Next we can fill out our packet handlers.** 

The only packets we need to handle are: getdata, whoAreYou, and setName. the rest of the handlers can be deleted. (They are define weakly in the service layer)

app_my_protocol.c:63
```c
/*******************************************************************************
  Packet handlers
*******************************************************************************/
/**
  *@brief Handler for receiving getData packets
  *@param getData incoming getData packet
  *@param sensorData sensorData packet to respond with
  *@return handling mp_status
  */
HandlerStatus_e mp_GetData_handler(mp_packet_t* mp_getData, mp_packet_t* mp_sensorData)
{

  mp_packet_copy(mp_sensorData, &myDevice); /* copy fields from 'myDevice' into the response packet*/

  return PACKET_HANDLED;  /* Make sure to change this to PACKET_HANDLED*/
}

/**
  *@brief Handler for receiving whoAreYou packets
  *@param whoAreYou incoming whoAreYou packet
  *@param myNameIs myNameIs packet to respond with
  *@return handling mp_status
  */
HandlerStatus_e mp_WhoAreYou_handler(mp_packet_t* mp_whoAreYou, mp_packet_t* mp_myNameIs)
{
  
  mp_packet_copy(mp_myNameIs, &myDevice);  /* copy fields from 'myDevice' into the response packet*/

  return PACKET_HANDLED;   /* Make sure to change this to PACKET_HANDLED*/
}

/**
  *@brief Handler for receiving setName packets
  *@param setName incoming setName packet
  *@return handling mp_status
  */
HandlerStatus_e mp_SetName_handler(mp_packet_t* mp_setName)
{
  
  mp_packet_copy(&myDevice, mp_setName); /* Copy fields from incoming packet to 'myDevice' */

  return PACKET_HANDLED; /* Make sure to change this to PACKET_HANDLED*/
}
```

** Now we will use the sensor data from our device driver to set the fields of 'myDevice'

make it available to our main.c

app_my_protocol.h:11
```c
extern mp_struct_t myDevice;
``` 

Then add code to set the values in our main loop:

main.c:129
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

        mp_setTemp(&myDevice, temperature);
        mp_setHumidity(&myDevice, humidity);

      } 
    }
    app_my_protocol_process(); /* process our service*/
    ticks++;
    MRT_DELAY_MS(10);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
```

build!

[Next Step](poly-cli.md)