#pragma once

#include <21Domain/VideoRequesterManager/IVideoRequesterManager.h>
#include <02Platform/Component/Component.h>
#include <01Base/Aspect/Log.h>

class VideoRequesterManager : public Component, public IVideoRequesterManager {
public:
	VideoRequesterManager(int nClassId = _VideoRequesterManager_Id,
		const char* pcClassName = _VideoRequesterManager_Name)
		: Component(nClassId, pcClassName)
	{
		this->RegisterExceptions();
	}
	virtual ~VideoRequesterManager() {
	}

protected:
	virtual void RegisterExceptions() {
	}

	void Initialize(Event* pEvent) {
		LOG_HEADER("VideoRequesterManage::Register");
		LOG_FOOTER("VideoRequesterManage::Register");
	}

	void Start(Event* pEvent) {
		LOG_HEADER("VideoRequesterManage::Start");
		LOG_FOOTER("VideoRequesterManage::Start");
	}

	void ProcessAEvent(Event* pEvent) {
		switch (pEvent->GetType()) {
		case (unsigned)IVideoRequesterManager::EEventType::eInitialize:
			this->Initialize(pEvent);
			break;
		case (unsigned)IVideoRequesterManager::EEventType::eStart:
			this->Start(pEvent);
			break;
		default:
			Component::ProcessAEvent(pEvent);
			break;
		}
	}
};
