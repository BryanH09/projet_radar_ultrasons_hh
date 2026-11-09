/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    UDP_Socket.c
 * Purpose: UDP Socket Code Template
 * Rev.:    V7.0.0
 *----------------------------------------------------------------------------*/
//! [code_UDP_Socket]
#include "rl_net.h"
#include "stm32f7xx.h"                  // Device header
#include "AppWizard.h"
#include <stdio.h>

int32_t udp_sock;  
// UDP socket handle
uint8_t *sendbuf;
 
// Notify the user application about UDP socket events.
uint32_t udp_cb_func (int32_t socket, const  NET_ADDR *addr, const uint8_t *buf, uint32_t len) {
	
  // Data received
	int i,angle,dist;
	char cmd[32];
	
	sendbuf = netUDP_GetBuffer(32);
	if (len!=0)
	{
		for (i=0;i<len;i++)
		{
			cmd[i]=buf[i];
		}
		cmd[i]= '\0';
		if ((sscanf(cmd, "ANGLE %d",&angle))==1)
		{
			sprintf(sendbuf,"angle %d",angle);
			
			
		}
		if (cmd[0]=="D")
		{
			sprintf(sendbuf,"distance = %d",dist);
			
	
		}
		netUDP_Send (udp_sock, &addr, sendbuf, 32);
		
		
	}
	return (0);
}

 
// Allocate and initialize the socket.
void Init_Ethernet (void) {
 
  netInitialize ();
	
  // Initialize UDP socket and open port 2000
  udp_sock = netUDP_GetSocket (udp_cb_func);
  if (udp_sock > 0) {
    netUDP_Open (udp_sock, 5001);
  }
}

//! [code_UDP_Socket]
