//
//  SNMPMsg.cpp
//  SimpleSNMP
//
//  Created by thanhvu on 7/13/19.
//  Copyright © 2019 thanhvu. All rights reserved.
//

#include "SNMPMsg.h"
#include "DataType.h"
#include "util.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <thread>

SNMPMsg::SNMPMsg(u_int8_t *raw, int packageLength)
{
    SNMPDataType dataType = (SNMPDataType)raw[0];
    u_int8_t msgLength = raw[1];
    
    if (packageLength < msgLength + 2 || dataType != SNMPDataTypeSequence)
    {
        this->isValid = false;
        return;
    }
    
    u_int8_t *bodyRaw = (uint8_t *)malloc(msgLength);
    memcpy(bodyRaw, raw+kBodyIndex, msgLength);
    
    int currentIndex = 0;
    // Parse version
    u_int8_t versionLength = bodyRaw[currentIndex+kLengthIndex];
    u_int8_t versionRawLength = versionLength + kLengthOfDataTypeAndLengthField;
    u_int8_t *versionRaw = (uint8_t *)malloc(versionRawLength);
    memcpy(versionRaw, bodyRaw+currentIndex, versionRawLength);
    currentIndex+=versionRawLength;
    
    this->version = new SNMPFieldVersion(versionRaw, versionRawLength);
    free(versionRaw);
    
    // Parse community
    SNMPDataType communityDataType = (SNMPDataType)bodyRaw[currentIndex++];
    u_int8_t communityLength = bodyRaw[currentIndex++];
    u_int8_t *communityBody = (u_int8_t *)malloc(communityLength);
    memcpy(communityBody, bodyRaw+currentIndex, communityLength);
    currentIndex+=communityLength;
    
    if (communityDataType != SNMPDataTypeOctetString)
    {
        this->isValid = false;
        return;
    }
    
    this->community = string((char *)communityBody, communityLength);
    free(communityBody);
    
    // Parse PDU
    u_int8_t pduBodyLength = bodyRaw[currentIndex+kLengthIndex];
    u_int8_t pduRawLength = pduBodyLength + kLengthOfDataTypeAndLengthField;
    u_int8_t *pduRaw = (uint8_t *)malloc(pduRawLength);
    memcpy(pduRaw, bodyRaw+currentIndex, pduRawLength);
    currentIndex+=pduRawLength;
    
    this->pdu = new SNMPPDU(pduRaw, pduRawLength);
    free(pduRaw);
    free(bodyRaw);
}

u_int8_t* SNMPMsg::rawValuePtr(int &length)
{
    u_int8_t buffer[1000];
    
    int currentIndex = 0;
    buffer[currentIndex++] = SNMPDataTypeSequence;
    buffer[currentIndex++] = 0; // Temp length
    
    int versionLength;
    u_int8_t *versionRaw = this->version->rawValuePtr(versionLength);
    
    int communityLength = (int)this->community.length();
    
    int pduLength;
    u_int8_t *pduRaw = this->pdu->rawValuePtr(pduLength);
    
    memcpy(buffer+currentIndex, versionRaw, versionLength);
    currentIndex+= versionLength;
    
    // Set community
    buffer[currentIndex++] = SNMPDataTypeOctetString;
    buffer[currentIndex++] = communityLength;
    memcpy(buffer+currentIndex, (u_int8_t *)this->community.c_str(), communityLength);
    currentIndex+= communityLength;
    
    memcpy(buffer+currentIndex, pduRaw, pduLength);
    currentIndex+= pduLength;
    
    buffer[kLengthIndex] = currentIndex - 2;
    length = currentIndex;
    
    
    free(versionRaw);
    free(pduRaw);
    return copyToHeap(buffer, length);
}

void SNMPMsg::send(char* server, int maxTry, int timeout, SNMPCallback callback)
{
    thread sendThread(&SNMPMsg::trySend, this, server, 1, maxTry, timeout, callback);
    
    sendThread.detach();
}

void SNMPMsg::trySend(char* server, int current, int maxTry, int timeout, SNMPCallback callback)
{
    int receivedDataLength;
    u_int8_t *data = doSend(server, receivedDataLength, timeout);
    if (receivedDataLength <= 0)
    {
        if (current < maxTry)
        {
            trySend(server, current+1, maxTry, timeout, callback);
            return;
        }
        else
        {
            callback(this, NULL);
            return;
        }
    }
    
    SNMPMsg *msg = new SNMPMsg(data, (int)receivedDataLength);
    callback(this, msg);
}

u_int8_t * SNMPMsg::doSend(char* server, int &receivedLength, int timeout)
{
    int packageLength;
    u_int8_t *package = this->rawValuePtr(packageLength);
    int sockfd;
    char buffer[1000];
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(161);
    serverAddr.sin_addr.s_addr = inet_addr(server);
    
    if(sendto(sockfd, package, packageLength, 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        printf("error");
        exit(2);
    }
    
    socklen_t len;
    int timoutSecond = timeout/1000;
    int timeoutUsec = timeout%1000;
    struct timeval timeoutInteval;
    timeoutInteval.tv_sec = timoutSecond;
    timeoutInteval.tv_usec = timeoutUsec;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeoutInteval, sizeof(timeoutInteval)) < 0)
    {
        printf("Set socket timeout error");
    }
    
    receivedLength = (int)recvfrom(sockfd, (char *)buffer, 1000, MSG_WAITALL, (struct sockaddr *) &server, &len);
    
    if (receivedLength > 0)
    {
        return copyToHeap((u_int8_t *)buffer, receivedLength);
    }
    
    return NULL;
}
