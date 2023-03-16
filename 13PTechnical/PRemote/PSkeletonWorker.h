#pragma once

#include <12PPlatform/PComponent/PComponentPart.h>
#include <13PTechnical/PRemote/IPSkeletonWorker.h>
#include <01Base/Aspect/Exception.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXBUF 1024

class PSkeletonWorker : public PComponentPart, public IPSkeletonWorker
{
private:
    int m_nSockfdClient;
public:
	PSkeletonWorker(int nSockfdClient, int nComponentId = _PSkeletonWorker_Id, const char* sComponentName = _PSkeletonWorker_Name)
    : PComponentPart(nComponentId, sComponentName)
    , m_nSockfdClient(nSockfdClient)
    {
    }
	~PSkeletonWorker() override {
    }

    void Start() override {
        PComponentPart::Start();
        this->SetEState(IComponent::EState::eRunning);
    }
    void Stop() override {
        PComponentPart::Stop();
        this->SetEState(IComponent::EState::eStopped);
    }

    void RunThread() override {
        int result;
        char buf[MAXBUF];
        
        while (this->GetEState() == IComponent::EState::eRunning) {
            LOG_NEWLINE("PSkeletonWorker::Run", this->GetObjectId());
            // read
            memset(buf, 0x00, MAXBUF);
            result = read(m_nSockfdClient, buf, MAXBUF);
            if (result < 0) {
                throw Exception((int)(IPSkeleton::EException::eRead), "eRead", result);
            }
            // write
            result = write(m_nSockfdClient, buf, MAXBUF);
            if(result <= 0) {
                throw Exception((int)(IPSkeleton::EException::eWrite), "eWrite", result);
            }
        }
        close(m_nSockfdClient);
    }
};