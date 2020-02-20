/**
  *@file app_my_protocol.h
  *@brief generated protocol source code
  *@author make_protocol.py
  *@date 02/19/20
  */

#include "my_protocolService.h"
#include "Platforms/Common/mrt_platform.h"

extern mp_struct_t myDevice;


/**
  *@brief Initialize the packet service
  */
void app_my_protocol_init(mrt_uart_handle_t uart_handle);

/**
  *@brief ends service
  */
void app_my_protocol_end();

/**
  *@brief process the data for the packet service
  */
void app_my_protocol_process();