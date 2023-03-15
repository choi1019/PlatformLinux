#pragma once

#include <12PPlatform/PComponent/PComponent.h>
#include <13PTechnical/PRemote/IPSkeleton.h>
#include <13PTechnical/PRemote/PSkeletonWorker.h>

#include <01Base/Aspect/Exception.h>
#include <01Base/StdLib/Map.h>

#include <sys/socket.h>
//#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>

class PSkeleton : public PComponent, public IPSkeleton
{
private:
        int m_nSockfdServer;       
        struct sockaddr_in m_sockaddrServer;
        in_addr_t m_inAddressIP;
        int m_nNumPort;
public:
	PSkeleton(int nNumPort, 
            const char *sAddressIP = nullptr, 
            int nComponentId = _PSkeleton_Id, 
            const char* sComponentName = _PSkeleton_Name)
    : PComponent(nComponentId, sComponentName)
    , m_nNumPort(nNumPort)
    {
        if (sAddressIP == nullptr) {
            m_inAddressIP = INADDR_ANY;
        } else {
            m_inAddressIP = inet_addr(sAddressIP);
        }
    }
	~PSkeleton() override {
    }

    void Initialize(Event *pEvent) {
        // socket
        int result = (m_nSockfdServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
        if(result < 0){
            throw Exception((int)(IPSkeleton::EException::eSocket), "eSocket", result);
        }

        // bind
        bzero(&m_sockaddrServer, sizeof(m_sockaddrServer));
        m_sockaddrServer.sin_family = AF_INET;
        m_sockaddrServer.sin_addr.s_addr = m_inAddressIP;
        m_sockaddrServer.sin_port = htons(m_nNumPort);    
        bind(m_nSockfdServer, (struct sockaddr *) &m_sockaddrServer, sizeof(m_sockaddrServer));

        // listen
        result = listen(m_nSockfdServer, 5);
        if(result < 0){
            throw Exception((int)(IPSkeleton::EException::eListen), "eListen", result);
        }
    }

    void Finalize(Event *pEvent) {
        close(m_nSockfdServer);
    }

    void RunThread() {
        this->SetEState(IComponent::EState::eRunning);

        int result;
        int m_nSockfdClient; 
        struct sockaddr_in sockaddrClient;
        int nSkeletonWorkerId = (int)IPSkeleton::EParts::ePSkeletonPart;  

        while(this->GetEState() == IComponent::EState::eRunning){
                        // accept
            socklen_t soclenClient = sizeof(sockaddrClient);
            m_nSockfdClient = accept(m_nSockfdServer, (struct sockaddr *) &sockaddrClient, &soclenClient);
            if(m_nSockfdClient < 0){
                throw Exception((int)(IPSkeleton::EException::eAccept), "eAccept", result);
            }

            //LOG_NEWLINE("PSkeleton::Start", inet_ntoa(sockaddrClient.sin_addr));
            PSkeletonWorker *pPSkeletonWorker = new("PSkeletonWorker") PSkeletonWorker(m_nSockfdClient);
            this->AddPart(nSkeletonWorkerId++, pPSkeletonWorker);
            pPSkeletonWorker->Start();
        }
    }

    void Start(Event *pEvent) {
        PComponent::Start();
    }

    void Stop(Event *pEvent) {
        for (PairComponentPart pairComponentPart: *GetPComponentParts()) {
            ((PComponentPart*)(pairComponentPart.second))->Stop();
        }
        PComponent::Stop();
    }

    void ProcessAEvent(Event* pEvent) override {
		switch (pEvent->GetType()) {
		case (int)IPSkeleton::EEventType::eInitialize:
			this->Initialize(pEvent);
			break;
		case (int)IPSkeleton::EEventType::eFinalize:
			this->Finalize(pEvent);
			break;
		case (int)IPSkeleton::EEventType::eStart:
			this->Start(pEvent);
			break;
		case (int)IPSkeleton::EEventType::eStop:
			this->Stop(pEvent);
			break;
		default:
			PComponent::ProcessAEvent(pEvent);
			break;
		}
	}
};