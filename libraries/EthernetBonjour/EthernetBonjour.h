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

#if !defined(__ETHERNET_BONJOUR_H__)
#define __ETHERNET_BONJOUR_H__ 1

extern "C" {
   #include <inttypes.h>
}

typedef uint8_t byte;

typedef enum _MDNSState_t {
   MDNSStateIdle,
   MDNSStateQuerySent
} MDNSState_t;

typedef enum _MDNSError_t {
   MDNSTryLater = 3,
   MDNSNothingToDo = 2,
   MDNSSuccess  = 1,
   MDNSInvalidArgument = -1,
   MDNSOutOfMemory = -2,
   MDNSSocketError = -3,
   MDNSAlreadyProcessingQuery = -4,
   MDNSNotFound = -5,
   MDNSServerError = -6,
   MDNSTimedOut = -7
} MDNSError_t;

typedef struct _MDNSDataInternal_t {
   uint32_t xid;
   uint32_t lastQueryFirstXid;
} MDNSDataInternal_t;

typedef enum _MDNSServiceProtocol_t {
   MDNSServiceTCP,
   MDNSServiceUDP
} MDNSServiceProtocol_t;

typedef MDNSServiceProtocol_t MDNSServiceProtocol;

typedef struct _MDNSServiceRecord_t {
   uint16_t                port;
   MDNSServiceProtocol_t   proto;
   uint8_t*                name;
   uint8_t*                servName;
   uint8_t*                textContent;
} MDNSServiceRecord_t;

typedef void (*BonjourNameFoundCallback)(const char*, const byte[4]);
typedef void (*BonjourServiceFoundCallback)(const char*, MDNSServiceProtocol_t, const char*,
                                            const byte[4], unsigned short, const char*);

#define  NumMDNSServiceRecords   (8)

class EthernetBonjourClass
{
private:
   MDNSDataInternal_t    _mdnsData;
   int                  _socket;
   MDNSState_t           _state;
   uint8_t*             _bonjourName;
   MDNSServiceRecord_t* _serviceRecords[NumMDNSServiceRecords];
   unsigned long        _lastAnnounceMillis;
   
   uint8_t*             _resolveNames[2];
   unsigned long        _resolveLastSendMillis[2];
   unsigned long        _resolveTimeouts[2];
   
   MDNSServiceProtocol_t _resolveServiceProto;
   
   BonjourNameFoundCallback      _nameFoundCallback;
   BonjourServiceFoundCallback   _serviceFoundCallback;

   MDNSError_t _processMDNSQuery();
   MDNSError_t _sendMDNSMessage(uint32_t peerAddress, uint32_t xid, int type, int serviceRecord);

   int _startMDNSSession();
   int _closeMDNSSession();
   
   void _writeDNSName(const uint8_t* name, uint16_t* pPtr, uint8_t* buf, int bufSize,
                      int zeroTerminate);
   void _writeMyIPAnswerRecord(uint16_t* pPtr, uint8_t* buf, int bufSize);
   void _writeServiceRecordName(int recordIndex, uint16_t* pPtr, uint8_t* buf, int bufSize, int tld);
   void _writeServiceRecordPTR(int recordIndex, uint16_t* pPtr, uint8_t* buf, int bufSize,
                               uint32_t ttl);
   
   int _initQuery(uint8_t idx, const char* name, unsigned long timeout);
   void _cancelQuery(uint8_t idx);
   
   uint8_t* _findFirstDotFromRight(const uint8_t* str);
   
   void _removeServiceRecord(int idx);
   
   int _matchStringPart(const uint8_t** pCmpStr, int* pCmpLen, const uint8_t* buf,
                        int dataLen);
   
   const uint8_t* _postfixForProtocol(MDNSServiceProtocol_t proto);
   
   void _finishedResolvingName(char* name, const byte ipAddr[4]);
public:
   EthernetBonjourClass();
   ~EthernetBonjourClass();
   
   int begin();
   int begin(const char* bonjourName);
   void run();
   
   int setBonjourName(const char* bonjourName);
   
   int addServiceRecord(const char* name, uint16_t port, MDNSServiceProtocol_t proto);
   int addServiceRecord(const char* name, uint16_t port, MDNSServiceProtocol_t proto,
                        const char* textContent);
   
   void removeServiceRecord(uint16_t port, MDNSServiceProtocol_t proto);
   void removeServiceRecord(const char* name, uint16_t port, MDNSServiceProtocol_t proto);
      
   void removeAllServiceRecords();
   
   void setNameResolvedCallback(BonjourNameFoundCallback newCallback);
   int resolveName(const char* name, unsigned long timeout);
   void cancelResolveName();
   int isResolvingName();
   
   void setServiceFoundCallback(BonjourServiceFoundCallback newCallback);
   int startDiscoveringService(const char* serviceName, MDNSServiceProtocol_t proto,
                               unsigned long timeout);
   void stopDiscoveringService();
   int isDiscoveringService();
};

extern EthernetBonjourClass EthernetBonjour;

#endif // __ETHERNET_BONJOUR_H__
