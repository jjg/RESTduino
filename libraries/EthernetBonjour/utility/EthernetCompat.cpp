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

#include <utility/EthernetCompat.h>

#if defined(__ETHERNET_COMPAT_BONJOUR__)

#if defined(ARDUINO) && ARDUINO > 18   // Arduino 0019 or later

#include <utility/socket.h>
#include <utility/w5100.h>
extern "C" {
   #include "Arduino.h"
}

#define TXBUF_BASE      0x4000
#define SMASK           0x07FF

const uint8_t ECSockClosed       = SnSR::CLOSED;
const uint8_t ECSnCrSockSend     = Sock_SEND;
const uint8_t ECSnCrSockRecv     = Sock_RECV;
const uint8_t ECSnMrUDP          = SnMR::UDP;
const uint8_t ECSnMrMulticast    = SnMR::MULTI;

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  inline static void initSS()    { DDRB  |=  _BV(4); };
  inline static void setSS()     { PORTB &= ~_BV(4); };
  inline static void resetSS()   { PORTB |=  _BV(4); };
#else
  inline static void initSS()    { DDRB  |=  _BV(2); };
  inline static void setSS()     { PORTB &= ~_BV(2); };
  inline static void resetSS()   { PORTB |=  _BV(2); };
#endif

uint16_t ethernet_compat_write_private(uint16_t _addr, uint8_t *_buf, uint16_t _len)
{
   for (int i=0; i<_len; i++) {
      setSS();    
      SPI.transfer(0xF0);
      SPI.transfer(_addr >> 8);
      SPI.transfer(_addr & 0xFF);
      _addr++;
      SPI.transfer(_buf[i]);
      resetSS();
   }
   return _len;
}

void ethernet_compat_init(uint8_t* macAddr, uint8_t* ipAddr, uint16_t rxtx_bufsize)
{
   W5100.init();
	W5100.setMACAddress(macAddr);
	W5100.setIPAddress(ipAddr);
}

uint8_t ethernet_compat_socket(int s, uint8_t proto, uint16_t port, uint8_t flag)
{
   return socket(s, proto, port, flag);
}

void ethernet_compat_close(int s)
{
   close(s);
}

uint16_t ethernet_compat_read_SnTX_WR(int socket)
{
   return W5100.readSnTX_WR(socket);
}

void ethernet_compat_write_data(int socket, uint8_t* src, uint8_t* dst, uint16_t len)
{
   uint16_t size;
   uint16_t dst_mask;
   uint16_t dst_ptr, dst_ptr_base;

   dst_mask = (uint16_t)dst & SMASK;
   dst_ptr_base = TXBUF_BASE + socket * W5100Class::SSIZE;
   dst_ptr = dst_ptr_base + dst_mask;

   if( (dst_mask + len) > W5100Class::SSIZE ) 
   {
 	size = W5100Class::SSIZE - dst_mask;
     ethernet_compat_write_private(dst_ptr, (uint8_t *) src, size);
     src += size;
 	  ethernet_compat_write_private(dst_ptr_base, (uint8_t *) src, len - size);
   } 
   else
     ethernet_compat_write_private(dst_ptr, (uint8_t *) src, len);
}

uint16_t ethernet_compat_read_SnRX_RSR(int socket)
{
   return W5100.readSnRX_RSR(socket);
}

uint16_t ethernet_compat_read_SnRX_RD(int socket)
{
   return W5100.readSnRX_RD(socket);
}

void ethernet_compat_read_data(int socket, uint8_t* src, uint8_t* dst, uint16_t len)
{
   W5100.read_data(socket, src, dst, len);
}

uint8_t ethernet_compat_read_SnSr(int socket)
{
   return W5100.readSnSR(socket);
}

uint8_t ethernet_compat_read_SnCR(int socket)
{
   return W5100.readSnCR(socket);
}

void ethernet_compat_write_DHAR(int socket, uint8_t* macAddr)
{
   W5100.writeSnDHAR(socket, macAddr);
}

void ethernet_compat_write_SnDIPR(int socket, uint8_t* serverIpAddr)
{
   W5100.writeSnDIPR(socket, serverIpAddr);
}

void ethernet_compat_write_SnDPORT(int socket, uint16_t port)
{
   W5100.writeSnDPORT(socket, port);
}

void ethernet_compat_write_SnTX_WR(int socket, uint16_t ptr)
{
   W5100.writeSnTX_WR(socket, ptr);
}

void ethernet_compat_write_SnCR(int socket, uint8_t cmd)
{
   W5100.writeSnCR(socket, cmd);
}

void ethernet_compat_write_SnRX_RD(int socket, uint16_t ptr)
{
   W5100.writeSnRX_RD(socket, ptr);
}

void ethernet_compat_read_SIPR(uint8_t* dst)
{
   W5100.readSIPR(dst);
}

void ethernet_compat_write_SIPR(uint8_t* ipAddr)
{
   W5100.writeSIPR(ipAddr);
}

void ethernet_compat_write_GAR(uint8_t* gatewayAddr)
{
   W5100.writeGAR(gatewayAddr);
}

void ethernet_compat_write_SUBR(uint8_t* subnetMask)
{
   W5100.writeSUBR(subnetMask);
}

#else // Arduino before 0019

extern "C" {
   #include "wiring.h"
   #include "../Ethernet/utility/types.h"
   #include "../Ethernet/utility/spi.h"
   #include "../Ethernet/utility/socket.h"
   #include "../Ethernet/utility/w5100.h"
}

const uint8_t ECSockClosed       = SOCK_CLOSED;
const uint8_t ECSnCrSockSend     = Sn_CR_SEND;
const uint8_t ECSnCrSockRecv     = Sn_CR_RECV;
const uint8_t ECSnMrUDP          = Sn_MR_UDP;
const uint8_t ECSnMrMulticast    = Sn_MR_MULTI;

void ethernet_compat_init(uint8_t* macAddr, uint8_t* ipAddr, uint16_t rxtx_bufsize)
{
   iinchip_init();
 	setSHAR(macAddr);
 	setSIPR(ipAddr);
 	
 	sysinit(rxtx_bufsize, rxtx_bufsize);
}

uint8_t ethernet_compat_socket(int s, uint8_t proto, uint16_t port, uint8_t flag)
{
   return socket(s, proto, port, flag);
}

void ethernet_compat_close(int s)
{
   close(s);
}

uint16_t ethernet_compat_read_SnTX_WR(int socket)
{
   uint16_t ptr = IINCHIP_READ(Sn_TX_WR0(socket));
 	return ((ptr & 0x00ff) << 8) + IINCHIP_READ(Sn_TX_WR0(socket) + 1);
}

void ethernet_compat_write_data(int socket, uint8_t* data, uint8_t* dst, uint16_t len)
{
   write_data(socket, (vuint8*)data, (vuint8*)dst, len);
}

uint16_t ethernet_compat_read_SnRX_RSR(int socket)
{
   uint16_t ptr = IINCHIP_READ(Sn_RX_RSR0(socket));
   return ((ptr & 0x00ff) << 8) + IINCHIP_READ(Sn_RX_RSR0(socket) + 1);
}

uint16_t ethernet_compat_read_SnRX_RD(int socket)
{
   uint16_t ptr = IINCHIP_READ(Sn_RX_RD0(socket));
   return ((ptr & 0x00ff) << 8) + IINCHIP_READ(Sn_RX_RD0(socket) + 1);
}

void ethernet_compat_read_data(int socket, uint8_t* src, uint8_t* dst, uint16_t len)
{
   read_data(socket, (vuint8*)src, (vuint8*)dst, len);
}

uint8_t ethernet_compat_read_SnSr(int socket)
{
   return IINCHIP_READ(Sn_SR(socket));
}

uint8_t ethernet_compat_read_SnCR(int socket)
{
   return IINCHIP_READ(Sn_CR(socket));
}

void ethernet_compat_write_DHAR(int socket, uint8_t* macAddr)
{
   for (uint8_t i=0; i<6; i++)
      IINCHIP_WRITE((Sn_DHAR0(socket) + i), macAddr[i]);
}

void ethernet_compat_write_SnDIPR(int socket, uint8_t* serverIpAddr)
{
   for (int i=0; i<4; i++)
      IINCHIP_WRITE((Sn_DIPR0(socket) + i), serverIpAddr[i]);
}

void ethernet_compat_write_SnDPORT(int socket, uint16_t port)
{
   IINCHIP_WRITE(Sn_DPORT0(socket), (uint8_t)((port & 0xff00) >> 8));
   IINCHIP_WRITE((Sn_DPORT0(socket) + 1), (uint8_t)(port & 0x00ff));
}

void ethernet_compat_write_SnTX_WR(int socket, uint16_t ptr)
{
   IINCHIP_WRITE(Sn_TX_WR0(socket), (vuint8)((ptr & 0xff00) >> 8));
   IINCHIP_WRITE((Sn_TX_WR0(socket) + 1), (vuint8)(ptr & 0x00ff));
}

void ethernet_compat_write_SnCR(int socket, uint8_t cmd)
{
   IINCHIP_WRITE(Sn_CR(socket), cmd);
}

void ethernet_compat_write_SnRX_RD(int socket, uint16_t ptr)
{
   IINCHIP_WRITE(Sn_RX_RD0(socket),(vuint8)((ptr & 0xff00) >> 8));
   IINCHIP_WRITE((Sn_RX_RD0(socket) + 1),(vuint8)(ptr & 0x00ff));
}

void ethernet_compat_read_SIPR(uint8_t* dst)
{
   getSIPR(dst);
}

void ethernet_compat_write_SIPR(uint8_t* ipAddr)
{
   setSIPR(ipAddr);
}

void ethernet_compat_write_GAR(uint8_t* gatewayAddr)
{
   setGAR(gatewayAddr);
}

void ethernet_compat_write_SUBR(uint8_t* subnetMask)
{
   setSUBR(subnetMask);
}

#endif // Arduino 0018 or earlier
#endif // __ETHERNET_COMPAT_BONJOUR__
