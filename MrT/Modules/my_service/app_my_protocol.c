/**
  *@file app_my_protocol.c
  *@brief generated protocol source code
  *@author make_protocol.py
  *@date 02/19/20
  */

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

void app_my_protocol_end()
{
}

/*******************************************************************************
  App Process
*******************************************************************************/

void app_my_protocol_process()
{
  /* read in new data from iface 0*/
 // iface0_read();

  /* process the actual service */
  mp_service_process();

}


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

