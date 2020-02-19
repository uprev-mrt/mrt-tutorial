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
static uint8_t iface0_rx_buf[512];
static char printBuf[512];

static inline HandlerStatus_e iface0_write(uint8_t* data, int len)
{
  /* Place code for writing bytes on interface 0 here */
  MRT_UART_TX(ifac0, data, len, 10);

  return PACKET_SENT;
}


static inline void iface0_read()
{
  int len = 16;
  /* Place code for reading bytes from interface 0 here */
  //TODO read bytes from interface to iface0_rx_buf
  if(MRT_UART_RX(ifac0, iface0_rx_buf, 16, 100) == HAL_TIMEOUT  )
  {
    len -= ifac0->RxXferCount;
  }


  if(len > 0)
  {
	  mp_service_feed(0,iface0_rx_buf, len);
  }
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
  *@return handling mp_status
  */
HandlerStatus_e mp_GetData_handler(mp_packet_t* mp_getData)
{
  /*  Get Required Fields in packet */





  return PACKET_NOT_HANDLED;
}

/**
  *@brief Handler for receiving whoAreYou packets
  *@param whoAreYou incoming whoAreYou packet
  *@return handling mp_status
  */
HandlerStatus_e mp_WhoAreYou_handler(mp_packet_t* mp_whoAreYou)
{
  /*  Get Required Fields in packet */





  return PACKET_NOT_HANDLED;
}

/**
  *@brief Handler for receiving myNameIs packets
  *@param myNameIs incoming myNameIs packet
  *@return handling mp_status
  */
HandlerStatus_e mp_MyNameIs_handler(mp_packet_t* mp_myNameIs)
{
  /*  Get Required Fields in packet */
  char deviceName[1];  //Name of device

  mp_getDeviceName(mp_myNameIs, deviceName);




  return PACKET_NOT_HANDLED;
}

/**
  *@brief Handler for receiving sensorData packets
  *@param sensorData incoming sensorData packet
  *@return handling mp_status
  */
HandlerStatus_e mp_SensorData_handler(mp_packet_t* mp_sensorData)
{
  /*  Get Required Fields in packet */
  int temp;  //Temperature in 100ths of a degree. i.e. 4500 = 45.00 deg C
  int humidity;  //Humidity in 100ths of a %. i.e. 5650 = 56.5%Rh

  temp = mp_getTemp(mp_sensorData);
  humidity = mp_getHumidity(mp_sensorData);




  return PACKET_NOT_HANDLED;
}


/**
  *@brief catch-all handler for any packet not handled by its default handler
  *@param metaPacket ptr to mp_packet_t containing packet
  *@param mp_response ptr to response
  *@return handling mp_status
  */
HandlerStatus_e mp_default_handler( mp_packet_t * mp_packet, mp_packet_t * mp_response)
{


  return PACKET_NOT_HANDLED;
}
