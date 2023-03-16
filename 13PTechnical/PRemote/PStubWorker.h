#pragma once

#include <13PTechnical/typedef.h>
#define _PStubWorker_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePStubWorker) 
#define _PStubWorker_Name "PStubWorker"

#include <12PPlatform/PComponent/PComponentPart.h>
#include <13PTechnical/PRemote/IPStub.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

#include <sys/socket.h> /* 소켓 관련 함수 */
#include <arpa/inet.h>  /* 소켓 지원을 위한 각종 함수 */
#include <unistd.h>     /* 각종 시스템 함수 */

#define MAXLINE     1024

class PStubWorker : public PComponentPart
{
private:
    int m_nNumPort;
    in_addr_t m_inAddressIP;
    int m_sockfdServer;

public:
	PStubWorker(
        int nNumPort, 
        in_addr_t inAddressIP, 
        int nComponentId = _PStubWorker_Id, 
        const char* sComponentName = _PStubWorker_Name) 
    : PComponentPart(nComponentId, sComponentName)
    , m_nNumPort(nNumPort)
    , m_inAddressIP(inAddressIP)
    {
    }
	~PStubWorker() override {
    }

    void Start(Event *pEvent) {
        PComponentPart::Fork();
        PComponentPart::Join();
    }
    void Stop(Event *pEvent) {
     }

    void RunThread() override {
        // create a socket
        if((m_sockfdServer = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            throw Exception((int)(IPStub::EException::eSocket));
        }

        // connect
        struct sockaddr_in serveraddr;
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = m_inAddressIP;
        serveraddr.sin_port = htons(m_nNumPort);
        int result = connect(m_sockfdServer, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
        if (result < 0){
            throw Exception((int)(IPStub::EException::eConnect), "eConnect", result);
        }
        // write
        char buf[MAXLINE];
        memset(buf, 0x00, MAXLINE);
        buf[0] = 't';
        buf[1] = 'e';
        buf[2] = 's';
        buf[3] = 't';
        buf[4] = '\0';
        result = write(m_sockfdServer, buf, MAXLINE);
        if(result <= 0){
            throw Exception((int)(IPStub::EException::eWrite), "eWrite", result);
        }

        // reply
        memset(buf, 0x00, MAXLINE);
        result = read(m_sockfdServer, buf, MAXLINE);
        if(result <= 0){
            throw Exception((int)(IPStub::EException::eRead), "eRead", result);
        }
        LOG_NEWLINE("PStub::Send and Reply - ", buf);
        fflush(stdout);
        close(m_sockfdServer); 
    }
};