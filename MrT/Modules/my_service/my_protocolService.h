/**
  *@file my_protocolService.h
  *@brief generated code for my_protocol packet service
  *@author make_protocol.py
  *@date 02/20/20
  *@hash C7E5198B
  */

#pragma once
/***********************************************************
        THIS FILE IS AUTOGENERATED. DO NOT MODIFY
***********************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "Utilities/PolyPacket/poly_service.h"

#ifdef __cplusplus
}
#endif

#define MP_SERVICE_HASH 0xC7E5198B

/*******************************************************************************
  Enums
*******************************************************************************/

/*******************************************************************************
  Bits/Flags
*******************************************************************************/

/*******************************************************************************
  Global Descriptors
*******************************************************************************/
//Declare extern packet descriptors
extern poly_packet_desc_t* MP_PACKET_PING;
extern poly_packet_desc_t* MP_PACKET_ACK;
extern poly_packet_desc_t* MP_PACKET_GETDATA;
extern poly_packet_desc_t* MP_PACKET_WHOAREYOU;
extern poly_packet_desc_t* MP_PACKET_MYNAMEIS;
extern poly_packet_desc_t* MP_PACKET_SETNAME;
extern poly_packet_desc_t* MP_PACKET_SENSORDATA;

extern poly_packet_desc_t* MP_STRUCT_DEVICE;


//Declare extern field descriptors
extern poly_field_desc_t* MP_FIELD_ICD;
extern poly_field_desc_t* MP_FIELD_DEVICENAME;
extern poly_field_desc_t* MP_FIELD_TEMP;
extern poly_field_desc_t* MP_FIELD_HUMIDITY;

/*
 *@brief The main type dealt with by the user
 */

typedef poly_packet_t mp_packet_t;
typedef mp_packet_t mp_struct_t;


/*******************************************************************************
  Service Functions
*******************************************************************************/
/**
  *@brief initializes protocol service
  *@param ifaces number of interfaces to use
  *@param depth spool size for each interface
  */
void mp_service_init(int interfaceCount, int depth);

/**
  *@brief tears down service
  *@note probably not needed based on lifecycle of service
  *@ but useful for detecting memory leaks
  */
void mp_service_teardown();


/**
  *@brief handles packets and dispatches to handler
  *@param req incoming message
  *@param resp response to message
  *@param number of bytes
  */
HandlerStatus_e mp_service_dispatch(mp_packet_t* req, mp_packet_t* resp);

/**
  *@brief processes data in buffers
  */
void mp_service_process();

/**
  *@brief registers a callback to let the service know how to send bytes for a given interface
  *@param iface index of interface to register with
  *@param txBytesCallBack a function pointer for the callback
  */
void mp_service_register_bytes_tx( int iface, poly_tx_bytes_callback txBytesCallBack);

/**
  *@brief registers a callback to let the service know how to send entire packets
  *@param iface index of interface to register with
  *@param txPacketCallBack a function pointer for the callback
  */
void mp_service_register_packet_tx( int iface, poly_tx_packet_callback txPacketCallBack);

/**
  *@brief 'Feeds' bytes to service at given interface for processing
  *@param iface index of interface to send on
  *@param data data to be processed
  *@param number of bytes
  */
void mp_service_feed(int iface, uint8_t* data, int len);

/**
  *@brief sets retry behavior for interface of service
  *@param iface index of interface to send on
  *@param retries number of retries
  *@param timeoutMs MS value for timeout before retry
  */
void mp_service_set_retry(int iface, uint16_t retries, uint32_t timeoutMs);

/**
  *@brief handles json message, and shortcuts the servicing proccess. used for http requests
  *@param req incoming json message string
  *@param resp response data
  *@param number of bytes
  */
HandlerStatus_e mp_handle_json(const char* req,int len, char* resp);

/**
  *@brief 'Feeds' json message to service
  *@param iface index of interface to send on
  *@param msg data to be processed
  *@param number of bytes
  */
void mp_service_feed_json(int iface, const char* msg, int len);

/**
  *@brief sends packet over given interface
  *@param packet packet to be sent
  *@param iface index of interface to send on
  */
HandlerStatus_e mp_send( int iface, mp_packet_t* packet);

/**
  *@brief tells the service time has passed so it can track packets timeouts/retries on the spool
  *@param ms number of milliseconds passed
  *@note this only sets flags/statuses. Nothing is handled until the next call to process the service. So it is fine to call this from a systick handler
  */
void mp_tick(uint32_t ms);

/**
  *@brief enables/disables the auto acknowledgement function of the service
  *@param enable true enable auto acks, false disables them
  */
void mp_auto_ack(bool enable);

/**
  *@brief enables/disables the txReady of an interface
  *@param enable true enable auto acks, false disables them
  */
void mp_enable_tx(int iface);
void mp_disable_tx(int iface);


/*******************************************************************************
  Meta-Packet Functions
*******************************************************************************/

/**
  *@brief initializes a new {proto.prefix}_packet_t
  *@param desc ptr to packet descriptor to model packet from
  */
void mp_packet_build(mp_packet_t* packet, poly_packet_desc_t* desc);
#define mp_struct_build(packet,desc) mp_packet_build(packet,desc)



/**
  *@brief recrusively cleans packet and its contents if it still has ownership
  *@param packet packet to clean
  */
void mp_clean(mp_packet_t* packet);

/**
  *@brief converts packet to json
  *@param packet ptr to packet to convert
  *@param buf buffer to store string
  *@return length of string
  */
#define mp_print_json(packet,buf) poly_packet_print_json((packet), buf, false)

/**
  *@brief parses packet from a buffer of data
  *@param packet ptr to packet to be built
  *@param buf buffer to parse
  *@return status of parse attempt
  */
#define mp_parse(packet,buf,len) poly_packet_parse_buffer((packet), buf, len)

/**
  *@brief Copies all fields present in both packets from src to dst
  *@param dst ptr to packet to copy to
  *@param src ptr to packet to copy from
  */
#define mp_packet_copy(dst,src) poly_packet_copy((dst),(src) )

/**
  *@brief packs packet into a byte array
  *@param packet ptr to packet to be packed
  *@param buf buffer to store data
  *@return length of packed data
  */
#define mp_pack(packet, buf) poly_packet_pack((packet), buf)

/**
  *@brief gets the descriptor for the packet
  *@param packet ptr to packet to be checked
  */
#define mp_getDesc(packet) ((packet).mDesc)


/*******************************************************************************
  Meta-Packet setters
*******************************************************************************/
void mp_setIcd(mp_packet_t* packet, uint32_t val);
void mp_setDeviceName(mp_packet_t* packet, const char* val);
void mp_setTemp(mp_packet_t* packet, int val);
void mp_setHumidity(mp_packet_t* packet, int val);

/*******************************************************************************
  Meta-Packet getters
*******************************************************************************/

/**
  *@brief checks to see if field is present in packet
  *@param packet ptr to packet to be packed
  *@param field ptr to field desc
  *@return true if field is present
  */
#define mp_hasField(packet, field) poly_packet_has((packet), field)

uint32_t mp_getIcd(mp_packet_t* packet);
void mp_getDeviceName(mp_packet_t* packet, char* val);
int mp_getTemp(mp_packet_t* packet);
int mp_getHumidity(mp_packet_t* packet);

/*******************************************************************************
  Quick send functions

  These are convenience one-liner functions for sending messages.
  They also handle their own clean up and are less bug prone than building your own packets
*******************************************************************************/

/**
  *@brief Sends a ping
  *@param iface interface to ping
  *@note a ping is just an ACK without the ack flag set in the token
  */
HandlerStatus_e mp_sendPing(int iface);

HandlerStatus_e mp_sendGetData(int iface);
HandlerStatus_e mp_sendWhoAreYou(int iface);
HandlerStatus_e mp_sendMyNameIs(int iface);
HandlerStatus_e mp_sendSetName(int iface);
HandlerStatus_e mp_sendSensorData(int iface);

/*******************************************************************************
  Packet Handlers
*******************************************************************************/
/*@brief Handler for Ping packets */
HandlerStatus_e mp_Ping_handler(mp_packet_t* mp_Ping, mp_packet_t* mp_Ack);

/*@brief Handler for Ack packets */
HandlerStatus_e mp_Ack_handler(mp_packet_t* mp_Ack);

/*@brief Handler for getData packets */
HandlerStatus_e mp_GetData_handler(mp_packet_t* mp_getData, mp_packet_t* mp_sensorData);

/*@brief Handler for whoAreYou packets */
HandlerStatus_e mp_WhoAreYou_handler(mp_packet_t* mp_whoAreYou, mp_packet_t* mp_myNameIs);

/*@brief Handler for myNameIs packets */
HandlerStatus_e mp_MyNameIs_handler(mp_packet_t* mp_myNameIs);

/*@brief Handler for setName packets */
HandlerStatus_e mp_SetName_handler(mp_packet_t* mp_setName);

/*@brief Handler for sensorData packets */
HandlerStatus_e mp_SensorData_handler(mp_packet_t* mp_sensorData);

/*@brief Catch-All Handler for unhandled packets */
HandlerStatus_e mp_default_handler(mp_packet_t * mp_packet, mp_packet_t * mp_response);