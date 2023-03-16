#pragma once

#include <21Domain/VideoProviderManager/IVideoProviderManager.h>
#include <02Platform/Component/Component.h>
#include <01Base/Aspect/Log.h>

#include <21Domain/VideoRequesterManager/IVideoRequesterManager.h>
#include <13PTechnical/PRemote/IPStub.h>

class VideoProviderManager : public Component, public IVideoProviderManager {
public:
	VideoProviderManager(int nClassId = _VideoProviderManager_Id,
		const char* pcClassName = _VideoProviderManager_Name)
		: Component(nClassId, pcClassName)
	{
		this->RegisterExceptions();
	}
	virtual ~VideoProviderManager() {
	}

protected:
	virtual void RegisterExceptions() {
	}

	void Initialize(Event* pEvent) {
		if (pEvent->IsReply()) {
			if (pEvent->GetReplyType() == (int)IVideoRequesterManager::EEventType::eInitialize) {
				this->SendReplyEvent(
					(int)IVideoProviderManager::EReceivers::ePStub, 
					(int)IPStub::EEventType::eInitialize);
			} else {
				LOG_FOOTER("VideoProviderManager::Register");
			}
		}
		else {
			LOG_HEADER("VideoProviderManager::Register");
			this->SendReplyEvent(
				(int)IVideoProviderManager::EReceivers::eVideoRequesterManager,
				(int)IVideoRequesterManager::EEventType::eInitialize);
		}
	}

	void Start(Event* pEvent) {
		if (pEvent->IsReply()) {
			if (pEvent->GetReplyType() == (int)IVideoRequesterManager::EEventType::eStart) {
				this->SendReplyEvent(
					(int)IVideoProviderManager::EReceivers::ePStub, 
					(int)IPStub::EEventType::eSend);
			} else {
				this->SendReplyEvent(
					(int)IVideoProviderManager::EReceivers::ePStub, 
					(int)IPStub::EEventType::eSend);
//				LOG_FOOTER("VideoProviderManager::Start");
			}
		}
		else {
			LOG_HEADER("VideoProviderManager::Start");
			this->SendReplyEvent(
				(int)IVideoProviderManager::EReceivers::eVideoRequesterManager, 
				(int)IVideoRequesterManager::EEventType::eStart);
		}
	}

	void ProcessAEvent(Event* pEvent) {
		switch (pEvent->GetType()) {
		case (unsigned)IVideoProviderManager::EEventType::eInitialize:
			this->Initialize(pEvent);
			break;
		case (unsigned)IVideoProviderManager::EEventType::eStart:
			this->Start(pEvent);
			break;
		default:
			Component::ProcessAEvent(pEvent);
			break;
		}
	}
};
