#include <19PApplication/PLifecycleManager/PLifecycleManager.h>

#include <12PPlatform/PScheduler/PScheduler.h>

#include <13PTechnical/PTimer/PTimerRTC.h>
#include <13PTechnical/PTimer/PTimerLinux.h>
#include <13PTechnical/PTimer/PTimerLinux2.h>

#include <13PTechnical/PRemote/PSkeleton.h>
#include <13PTechnical/PRemote/PStub.h>

#include <21Domain/VideoManager/VideoManager.h>
#include <21Domain/VideoProviderManager/VideoProviderManager.h>
#include <21Domain/VideoRequesterManager/VideoRequesterManager.h>

PLifecycleManager::PLifecycleManager(
		unsigned typeId,
		const char* pClassName)
		: LifecycleManager(typeId, pClassName)
	{}
PLifecycleManager::~PLifecycleManager() {
}

void PLifecycleManager::RegisterUserShedulers() {
	this->RegisterAScheduler((int)EComponents::ePScheduler0, new("eScheduler0") PScheduler());
	this->RegisterAScheduler((int)EComponents::ePScheduler1, new("eScheduler1") PScheduler());
	this->RegisterAScheduler((int)EComponents::ePScheduler2, new("eScheduler2") PScheduler());
}
void PLifecycleManager::RegisterUserComponents() {
	this->RegisterAComponent((int)EComponents::ePStub, new("ePStub") PStub(10000));
	this->RegisterAComponent((int)EComponents::ePSkeleton, new("ePSkeleton") PSkeleton(10000));
	this->RegisterAComponent((int)EComponents::eVideoManager, new("eVideoManager") VideoManager());
	this->RegisterAComponent((int)EComponents::eVideoProviderManager, new("eVideoProviderManager") VideoProviderManager());
	this->RegisterAComponent((int)EComponents::eVideoRequesterManager, new("eVideoRequesterManager") VideoRequesterManager());
	// this->RegisterAComponent((int)EComponents::eTimerLinux, new("eTimerLinux") PTimerLinux(900));
	// this->RegisterAComponent((int)EComponents::eTimerLinux1, new("eTimerLinux") PTimerLinux2(500));
	// this->RegisterAComponent((int)EComponents::eTimerLinux2, new("eTimerLinux") PTimerLinux2(1000));
	// this->RegisterAComponent((int)EComponents::eTimerRTC, new("PTimerRTC") PTimerRTC(2000));
}
void PLifecycleManager::AllocateUserComponents() {


	this->AllocateAComponent((int)EComponents::eVideoManager, (int)EComponents::ePScheduler0);
		this->AllocateAComponent((int)EComponents::ePSkeleton, (int)EComponents::ePScheduler0);
	this->AllocateAComponent((int)EComponents::eVideoProviderManager, (int)EComponents::ePScheduler0);
	this->AllocateAComponent((int)EComponents::eVideoRequesterManager, (int)EComponents::ePScheduler0);
	this->AllocateAComponent((int)EComponents::ePStub, (int)EComponents::ePScheduler0);
	
	// this->AllocateAComponent((int)EComponents::eTimerLinux, (int)EComponents::eScheduler2);
	// this->AllocateAComponent((int)EComponents::eTimerLinux1, (int)EComponents::eScheduler1);
	//	this->AllocateAComponent((int)EComponents::eTimerLinux2, (int)EComponents::eScheduler2);
}
void PLifecycleManager::AssociateUserSendersNReceivers() {
	// this->AssociateASenderNAReceiver(
	// (int)EComponents::eVideoManager, (int)IVideoManager::ESources::eTimer, (int)EComponents::eTimer33);
	this->AssociateASenderNAReceiver(
		(int)EComponents::eVideoManager, (int)IVideoManager::EReceivers::eVideoProviderManager, 
		(int)EComponents::eVideoProviderManager);
	this->AssociateASenderNAReceiver(
		(int)EComponents::eVideoProviderManager, (int)IVideoProviderManager::EReceivers::eVideoRequesterManager, 
		(int)EComponents::eVideoRequesterManager);
	this->AssociateASenderNAReceiver(
		(int)EComponents::eVideoProviderManager, (int)IVideoProviderManager::EReceivers::ePStub,
		(int)EComponents::ePStub);
}
void PLifecycleManager::AssociateUserSourcesNTargets() {
	// this->AssociateASourceNATarget((int)EComponents::eTimer33, (int)Timer::EGroups::eGroup1, (int)EComponents::eVideoManager);
}

void PLifecycleManager::InitializeComponents(Event *pEvent) {
	if (pEvent->IsReply()) {
		if (pEvent->GetReplyType() == (int)IPSkeleton::EEventType::eInitialize) {
			this->SendReplyEvent((int)EComponents::eVideoManager, (int)IVideoManager::EEventType::eInitialize);
		} else {
			LOG_FOOTER(" PLifecycleManager::InitializeComponents");
		}
	} else {
		LOG_HEADER(" PLifecycleManager::InitializeComponents");
		this->SendReplyEvent((int)EComponents::ePSkeleton, (int)IPSkeleton::EEventType::eInitialize);
	}
}
void PLifecycleManager::StartSystem(Event *pEvent) {
	if (pEvent->IsReply()) {
		if (pEvent->GetReplyType() == (int)IPSkeleton::EEventType::eStart) {
			this->SendReplyEvent((int)EComponents::eVideoManager, (int)IVideoManager::EEventType::eStart);
		} else {
			LOG_FOOTER(" PLifecycleManager::StartSystem");
		}
	} else {
		LOG_HEADER(" PLifecycleManager::StartSystem");
		this->SendReplyEvent((int)EComponents::ePSkeleton, (int)IPSkeleton::EEventType::eStart);
	}
//	this->SendReplyEvent((int)EComponents::eStub, (int)IComponent::EEventType::eStart);
	// this->SendReplyEvent((int)EComponents::eTimerLinux, (int)IComponent::EEventType::eStart);
	// this->SendReplyEvent((int)EComponents::eTimerLinux1, (int)IComponent::EEventType::eStart);
	// this->SendReplyEvent((int)EComponents::eTimerLinux2, (int)IComponent::EEventType::eStart);
	// this->SendReplyEvent((int)EComponents::eTimerRTC, (int)IComponent::EEventType::eStart);
}

void PLifecycleManager::StopSystem(Event *pEvent) {
	// this->SendReplyEvent((int)EComponents::eTimerLinux, (int)Component::EEventType::eStop);
}
