//  Copyright (C) 2010 Georg Kaindl
//  http://gkaindl.com
//
//  This file is part of Arduino EthernetBonjour.
//
//  EthernetBonjour is free software: you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public License
//  as published by the Free Software Foundation, either version 3 of
//  the License, or (at your option) any later version.
//
//  EthernetBonjour is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with EthernetBonjour. If not, see
//  <http://www.gnu.org/licenses/>.
//

#if !defined(__ETHERNET_COMPAT_H__)
#define __ETHERNET_COMPAT_H__

#define __ETHERNET_COMPAT_BONJOUR__

#include <stdint.h>

extern const uint8_t ECSockClosed;
extern const uint8_t ECSnCrSockSend;
extern const uint8_t ECSnCrSockRecv;
extern const uint8_t ECSnMrUDP;
extern const uint8_t ECSnMrMulticast;

void ethernet_compat_init(uint8_t* macAddr, uint8_t* ipAddr, uint16_t rxtx_bufsize);

uint8_t ethernet_compat_socket(int s, uint8_t proto, uint16_t port, uint8_t flag);
void ethernet_compat_close(int s);

void ethernet_compat_read_data(int socket, uint8_t* src, uint8_t* dst, uint16_t len);
void ethernet_compat_write_data(int socket, uint8_t* data, uint8_t* dst, uint16_t len);

uint16_t ethernet_compat_read_SnTX_WR(int socket);
uint16_t ethernet_compat_read_SnRX_RSR(int socket);
uint16_t ethernet_compat_read_SnRX_RD(int socket);
uint8_t ethernet_compat_read_SnSr(int socket);
uint8_t ethernet_compat_read_SnCR(int socket);

void ethernet_compat_write_DHAR(int socket, uint8_t* macAddr);
void ethernet_compat_write_SnDIPR(int socket, uint8_t* serverIpAddr);
void ethernet_compat_write_SnDPORT(int socket, uint16_t port);
void ethernet_compat_write_SnTX_WR(int socket, uint16_t ptr);
void ethernet_compat_write_SnCR(int socket, uint8_t cmd);
void ethernet_compat_write_SnRX_RD(int socket, uint16_t ptr);

void ethernet_compat_read_SIPR(uint8_t* dst);
void ethernet_compat_write_SIPR(uint8_t* ipAddr);
void ethernet_compat_write_GAR(uint8_t* gatewayAddr);
void ethernet_compat_write_SUBR(uint8_t* subnetMask);

#endif // __ETHERNET_COMPAT_H__