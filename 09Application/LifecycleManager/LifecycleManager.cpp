#include <09Application/LifecycleManager/LifecycleManager.h>

// #include <02Platform/Component/Component.h>
// #include <02Platform/Scheduler/Scheduler.h>
#include <09Application/Main/IMain.h>

#include <01Base/Aspect/Directory.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

LifecycleManager::LifecycleManager(int nClassId, const char* pcClassName)
	: Component(nClassId, pcClassName)
	, m_pMainScheduler(nullptr)
{
	this->RegisterEventTypes();
	this->RegisterExceptions();
}

LifecycleManager::~LifecycleManager() {
}

void LifecycleManager::Initialize() {
	Component::Initialize();
}
void LifecycleManager::Finalize() {
	Component::Finalize();
}

void LifecycleManager::RegisterEventTypes() {
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eStopSystem] = "eStopSystem";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eInitializeAsALifecycleManager] = "eInitializeAsALifecycleManager";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eFinalizeAsALifecycleManager] = "eFinalizeAsALifecycleManager";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eRegisterSchedulers] = "eRegisterSchedulers";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eInitializeSchedulers] = "eInitializeSchedulers";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eStartSchedulers] = "eStartSchedulers";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eFinalizeSchedulers] = "eFinalizeSchedulers";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eStopSchedulers] = "eStopSchedulers";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eRegisterComponents] = "eRegisterComponents";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eAllocateComponents] = "eAllocateComponents";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eAssociateSendersNReceivers] = "eAssociateSendersNReceivers";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eAssociateSourcesNTargets] = "eAssociateSourcesNTargets";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eInitializeComponents] = "eInitializeComponents";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eFinalizeComponents] = "eFinalizeComponents";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eStartSystem] = "eStartSystem";
}

void  LifecycleManager::RegisterExceptions() {
	Directory::s_dirExceptions[(unsigned)ILifecycleManager::EException::eEventTypeError] = "eEventTypeError";
	Directory::s_dirExceptions[(unsigned)ILifecycleManager::EException::eSchedulerNotRegistered] = "eSchedulerNotRegistered";
	Directory::s_dirExceptions[(unsigned)ILifecycleManager::EException::eComponentNotRegistered] = "eComponentNotRegistered";
	Directory::s_dirExceptions[(unsigned)ILifecycleManager::EException::eInitializationReplyError] = "eInitializationReplyError";
}

Scheduler* LifecycleManager::FindAScheduler(UId uId) {
	for (auto itr : this->m_mapSchedulers) {
		if (itr.second->GetUId() == uId) {
			return itr.second;
		}
	}
	return nullptr;
}
Component* LifecycleManager::FindAComponent(UId uId) {
	for (auto itr : this->m_mapComponents) {
		if (itr.second->GetUId() == uId) {
			return itr.second;
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////
void LifecycleManager::RegisterAScheduler(int name, Scheduler* pScheduler) {
	this->m_mapSchedulers[name] = pScheduler;
	this->RegisterAComponent(name, pScheduler);
	LOG_NEWLINE("- RegisterAScheduler: ", name, Directory::s_dirComponents[pScheduler->GetComponentId()]);
}
void LifecycleManager::RegisterSystemSchedulers() {
	this->RegisterAScheduler((int)ILifecycleManager::EReceivers::eMainScheduler, m_pMainScheduler);
}
void LifecycleManager::RegisterSchedulers(Event* pEvent) {
	LOG_HEADER("LifecycleManager::RegisterSchedulers");
	this->RegisterSystemSchedulers();
	this->RegisterUserShedulers();
	LOG_FOOTER("LifecycleManager::RegisterSchedulers");
}
void LifecycleManager::InitializeSchedulers(Event* pEvent) {
	LOG_HEADER("LifecycleManager::InitializeSchedulers");
	for (auto itr : m_mapSchedulers) {
		itr.second->InitializeAsAScheduler(0);
		LOG_NEWLINE(Directory::s_dirComponents[itr.second->GetComponentId()]);
	}
	LOG_FOOTER("LifecycleManager::InitializeSchedulers");
}
void LifecycleManager::StartSchedulers(Event* pEvent) {
	if (pEvent->IsReply()) {
		LOG_NEWLINE("- StartSchedulers -", Directory::s_dirComponents[pEvent->GetUIdSource().GetComponentId()]);
		if (pEvent->IsAllReplied()) {
			LOG_FOOTER("LifecycleManager::StartSchedulers");
		}
	}
	else {
		LOG_HEADER("LifecycleManager::StartSchedulers");
		for (auto itr : m_mapSchedulers) {
			itr.second->Fork();
			this->SendReplyEvent(itr.second->GetUId(), (int)IScheduler::EEventType::eStart);
		}
	}
}
/////////////////////////////////////////////////////////////////////////
void LifecycleManager::RegisterAComponent(int name, Component* pComponent) {
	this->m_mapComponents[name] = pComponent;
	LOG_NEWLINE("- RegisterAComponent: ", name, Directory::s_dirComponents[pComponent->GetComponentId()]);
}
void LifecycleManager::RegisterSystemComponents() {
	this->RegisterAComponent((int)ILifecycleManager::EReceivers::eLifecycleManager, this);
}
// Register Components
void LifecycleManager::RegisterComponents(Event* pEvent) {
	LOG_HEADER("LifecycleManager::RegisterComponents");
	this->RegisterSystemComponents();
	this->RegisterUserComponents();
	LOG_FOOTER("LifecycleManager::RegisterComponents");
}
/////////////////////////////////////////////////////////////////////////
void LifecycleManager::AllocateAComponent(int componentName, int schedulerName) {
	this->m_mapAllocations[componentName] = schedulerName;
	LOG_NEWLINE("- AllocateAComponent: ", 
		componentName, m_mapComponents[componentName]->GetClassName(),
		schedulerName, m_mapSchedulers[schedulerName]->GetClassName()
		);
}
void LifecycleManager::AllocateSystemComponents() {
	this->AllocateAComponent((int)ILifecycleManager::EReceivers::eLifecycleManager, (int)ILifecycleManager::EReceivers::eMainScheduler);
}
// Allocate Components
void LifecycleManager::AllocateComponents(Event* pEvent) {
	if (pEvent->IsReply()) {
		if (pEvent->IsAllReplied()) {
			LOG_FOOTER("LifecycleManager::AllocateComponents");
		}
	}
	else {
		LOG_HEADER("LifecycleManager::AllocateComponents");
		this->AllocateSystemComponents();
		this->AllocateUserComponents();

		for (auto& itrMapAllocation : this->m_mapAllocations) {
			// find target scheduler
			Component* pTargetScheduler = this->m_mapSchedulers[itrMapAllocation.second];
			if (pTargetScheduler == nullptr) {
				throw Exception((int)ILifecycleManager::EException::eSchedulerNotRegistered, this->GetClassName(), __func__, "eSchedulerNotRegistered");
			}
			// find component to be allocated
			Component* pComponent = this->m_mapComponents[itrMapAllocation.first];
			if (pComponent == nullptr) {
				throw Exception((int)ILifecycleManager::EException::eComponentNotRegistered, this->GetClassName(), __func__, "eComponentNotRegistered");
			}

			UId uIdTargetScheduler = pTargetScheduler->GetUId();
			Scheduler::ParamAllocateAComponent* pParamAllocateAComponent =
				new("ParamAllocateAComponent") Scheduler::ParamAllocateAComponent(pComponent);
			this->SendReplyEvent(
				uIdTargetScheduler
				, (int)IScheduler::EEventType::eAllocateAComponent
				, UNDEFINED
				, pParamAllocateAComponent
			);
		}
	}
}

/////////////////////////////////////////////////////////////////////////
void LifecycleManager::AssociateASenderNAReceiver(int senderName, int receiverId, int receiverName) {
	this->m_mapSendersNReceivers[MapPairSender(senderName, receiverId)] = receiverName;
	Component* pSenderComponent = this->m_mapComponents[senderName];
	Component* pReceiverComponent = this->m_mapComponents[receiverName];

	LOG_NEWLINE("- AssociateASenderNAReceiver: ", 
		senderName, pSenderComponent->GetClassName(), receiverId,
		receiverName, pReceiverComponent->GetClassName()
		);
}

void LifecycleManager::AssociateSystemSendersNReceivers() {
	// Components -> LifecycleManager
	for (auto itr : this->m_mapComponents) {
		this->AssociateASenderNAReceiver(
			itr.first, (int)Component::EReceivers::eLifecycleManager, (int)ILifecycleManager::EReceivers::eLifecycleManager);
	}
	// lifecycleManager -> components
	for (auto itr : this->m_mapComponents) {
		this->AssociateASenderNAReceiver(
			(int)ILifecycleManager::EReceivers::eLifecycleManager, itr.first, itr.first);
	}
}
// associate senders and recievers
void LifecycleManager::AssociateSendersNReceivers(Event* pEvent) {
	if (pEvent->IsReply()) {
		if (pEvent->IsAllReplied()) {
			LOG_FOOTER("LifecycleManager::AssociateSendersNReceivers");
		}
	}
	else {
		LOG_HEADER("LifecycleManager::AssociateSendersNReceivers");
		this->AssociateSystemSendersNReceivers();
		this->AssociateUserSendersNReceivers();

		// <<senderName, sender.receiverName>, receiverName>
		for (auto& itrSendersNReceivers : this->m_mapSendersNReceivers) {
			Component* pSender = this->m_mapComponents[itrSendersNReceivers.first.first];
			Component* pReceiver = this->m_mapComponents[itrSendersNReceivers.second];

			Component::ParamAssociateAReceiver* pParamAssociateAReceiver
				= new("ParamAssociateAReceiver") Component::ParamAssociateAReceiver(itrSendersNReceivers.first.second, pReceiver->GetUId());
			this->SendReplyEvent(
				pSender->GetUId(),
				(int)Component::EEventType::eAssociateAReceiver,
				0,
				pParamAssociateAReceiver);
		}

	}
}
/////////////////////////////////////////////////////////////////////////
// <sourceName, source.GroupName + vector<tarGetName>*
void LifecycleManager::AssociateASourceNATarget(int nSourceName, int nGroupId, int nTarGetName) {
	auto itr = this->m_mapSourcesNTargets.Find(MapPairSource(nSourceName, nGroupId));
	if (itr == this->m_mapSourcesNTargets.end()) {
		this->m_mapSourcesNTargets[MapPairSource(nSourceName, nGroupId)] = new("VectorTargetNames") VectorTargetNames();
	}
	this->m_mapSourcesNTargets[MapPairSource(nSourceName, nGroupId)]->Add(nTarGetName);

	Component* pSourceComponent = this->m_mapComponents[nSourceName];
	Component* pTargetComponent = this->m_mapComponents[nTarGetName];

	LOG_NEWLINE("- AssociateASourceNATarget: ", 
		nSourceName, m_mapComponents[nSourceName]->GetClassName(), nGroupId,
		nTarGetName, m_mapSchedulers[nTarGetName]->GetClassName()
		);
}
void LifecycleManager::AssociateSystemSourcesNTargets() {
}
// associate a source and targets
void LifecycleManager::AssociateSourcesNTargets(Event* pEvent) {
	if (pEvent->IsReply()) {
		if (pEvent->IsAllReplied()) {
			LOG_FOOTER("LifecycleManager::AssociateSourcesNTargets");
		}
	}
	else {
		LOG_HEADER("LifecycleManager::AssociateSourcesNTargets");
		this->AssociateSystemSourcesNTargets();
		this->AssociateUserSourcesNTargets();

		// sourceName, source.groupName + vector<tarGetName>*
		for (auto itrSource : this->m_mapSourcesNTargets) {
			Component* pSourceComponent = this->m_mapComponents[itrSource.first.first];
			Component::ParamAssociateATarget* pParamAssociateATarget
				= new("ParamAssociateATarget") Component::ParamAssociateATarget(itrSource.first.second);
			for (auto itrTargetName : *itrSource.second) {
				Component* pTargetComponent = this->m_mapComponents[itrTargetName];
				pParamAssociateATarget->GetVUIdTargetComponents().Add(pTargetComponent->GetUId());
			}
			this->SendReplyEvent(
				pSourceComponent->GetUId(),
				(int)Component::EEventType::eAssociateATarget,
				itrSource.first.second,
				pParamAssociateATarget);
		}
	}
}
/////////////////////////////////////////////////////////////////////////
// Initialize Components
void LifecycleManager::InitializeComponents(Event* pEvent) {
	if (pEvent->IsReply()) {
		LOG_NEWLINE("- InitializeComponents: ", 
			Directory::s_dirComponents[pEvent->GetUIdSource().GetComponentId()]);
		if (pEvent->IsAllReplied()) {
			LOG_FOOTER("LifecycleManager::InitializeComponents");
		}
	}
	else {
		LOG_HEADER("LifecycleManager::InitializeComponents");
		for (auto& itrMapComponents : this->m_mapComponents) {
			this->SendReplyEvent(itrMapComponents.second->GetUId(), (int)Component::EEventType::eInitialize);
		}
	}
}
/////////////////////////////////////////////////////////////////////////
//  InitializeAsALifecycleManager
void LifecycleManager::InitializeAsALifecycleManager(Event* pEvent) {
	if (pEvent->IsReply()) {
		switch (pEvent->GetReplyType()) {
		case (int)ILifecycleManager::EEventType::eRegisterSchedulers:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eInitializeSchedulers); break;
		case (int)ILifecycleManager::EEventType::eInitializeSchedulers:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eStartSchedulers); break;
		case (int)ILifecycleManager::EEventType::eStartSchedulers:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eRegisterComponents); break;
		case (int)ILifecycleManager::EEventType::eRegisterComponents:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eAllocateComponents); break;
		case (int)ILifecycleManager::EEventType::eAllocateComponents:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eAssociateSendersNReceivers); break;
		case (int)ILifecycleManager::EEventType::eAssociateSendersNReceivers:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eAssociateSourcesNTargets); break;
		case (int)ILifecycleManager::EEventType::eAssociateSourcesNTargets:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eInitializeComponents); break;
		case (int)ILifecycleManager::EEventType::eInitializeComponents:
			LOG_FOOTER("LifecycleManager::InitializeAsALifecycleManager"); break;
		default:
			throw Exception((int)ILifecycleManager::EException::eInitializationReplyError, 
					"LifecycleManager::InitializeAsALifecycleManager", __LINE__); 
			break;
		}
	}
	else {
		LOG_HEADER("LifecycleManager::InitializeAsALifecycleManager");
		this->m_mapSchedulers.Clear();
		this->m_mapComponents.Clear();
		this->m_mapAllocations.Clear();
		this->m_mapSendersNReceivers.Clear();
		this->m_mapSourcesNTargets.Clear();
		this->m_pMainScheduler = nullptr; 		

		// unmarshalling
		ParamInitializeAsALifecycleManager* pParamInitializeAsALifecycleManager = (ParamInitializeAsALifecycleManager*)pEvent->GetPArg();
		this->m_pMainScheduler = pParamInitializeAsALifecycleManager->GetPMainScheduler();
		this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eRegisterSchedulers);
	}
}
/////////////////////////////////////////////////////////////////////////


// start System
void LifecycleManager::StartSystem(Event* pEvent) {
	if (pEvent->IsReply()) {
		LOG_NEWLINE("- StartSystem: ", 
			Directory::s_dirComponents[pEvent->GetUIdSource().GetComponentId()]);
		if (pEvent->IsAllReplied()) {
			LOG_FOOTER("LifecycleManager::StartSystem");
		}
	}
	else {
		LOG_HEADER("LifecycleManager::StartSystem");
		this->StartSystem();
	}
}
// Stop System
void LifecycleManager::StopSystem(Event* pEvent) {
	LOG_HEADER("LifecycleManager::StopSystem");
	this->SendNoReplyEvent(
		(int)ILifecycleManager::EReceivers::eMainScheduler,
		(int)IMain::EEventType::eFinalizeAsAMain);
	LOG_FOOTER("LifecycleManager::StopSystem");
}

/////////////////////////////////////////////////////////////////////////
// FinalizeAsALifecycleManager
void LifecycleManager::StopComponents(Event* pEvent) {
	if (pEvent->IsReply()) {
		LOG_NEWLINE("- StopComponents: ", 
			Directory::s_dirComponents[pEvent->GetUIdSource().GetComponentId()]);
		if (pEvent->IsAllReplied()) {
			LOG_FOOTER("LifecycleManager::StopComponents");
		}
	}
	else {
		LOG_HEADER("LifecycleManager::StopComponents");
		for (auto& itr : this->m_mapComponents) {
			this->SendReplyEvent(itr.second->GetUId(), (int)Component::EEventType::eStop);
		}
	}
}

void LifecycleManager::FinalizeComponents(Event* pEvent) {
	if (pEvent->IsReply()) {
		LOG_NEWLINE("- FinalizeComponents: ", 
			Directory::s_dirComponents[pEvent->GetUIdSource().GetComponentId()]);
		if (pEvent->IsAllReplied()) {
			LOG_FOOTER("LifecycleManager::FinalizeComponents");
		}
	}
	else {
		LOG_HEADER("LifecycleManager::FinalizeComponents");
		for (auto& itr : this->m_mapComponents) {
			this->SendReplyEvent(itr.second->GetUId(), (int)Component::EEventType::eFinalize);
		}
	}
}

void LifecycleManager::StopSchedulers(Event* pEvent) {
	if (pEvent->IsReply()) {
		LOG_NEWLINE("- StopSchedulers: ", 
			Directory::s_dirComponents[pEvent->GetUIdSource().GetComponentId()]);
		if (pEvent->IsAllReplied()) {
			for (auto itr : m_mapSchedulers) {
				itr.second->Join();
			}
			LOG_FOOTER("LifecycleManager::StopSchedulers");
		}
	}
	else {
		LOG_HEADER("LifecycleManager::StopSchedulers");
		for (auto itr : m_mapSchedulers) {
			this->SendReplyEvent(itr.second->GetUId(), (int)IScheduler::EEventType::eStop);
		}
	}
}

void LifecycleManager::FinalizeSchedulers(Event* pEvent) {
	LOG_HEADER("LifecycleManager::FinalizeSchedulers");
	for (auto itr : m_mapSchedulers) {
		itr.second->FinalizeAsAScheduler();
		LOG_NEWLINE("- FinalizeSchedulers: ", 
			Directory::s_dirComponents[itr.second->GetComponentId()]);
	}
	LOG_FOOTER("LifecycleManager::FinalizeSchedulers");
}

void LifecycleManager::DeleteComponents(Event* pEvent) {
	LOG_HEADER("LifecycleManager::DeleteComponents");
	for (auto itr : m_mapComponents) {
		MLOG_NEWLINE("- LifecycleManager::DeleteComponents: ", (size_t) itr.second,
			Directory::s_dirComponents[itr.second->GetComponentId()]);
	}
	for (auto itr : m_mapComponents) {
		if (itr.second != this->m_pMainScheduler && itr.second != this) {
			MLOG_NEWLINE("- LifecycleManager::DeleteComponents: ", 
					Directory::s_dirComponents[itr.second->GetComponentId()]);
			delete itr.second;
		}
	}
	LOG_FOOTER("LifecycleManager::DeleteComponents");
}

void LifecycleManager::FinalizeAsALifecycleManager(Event* pEvent) {
	if (pEvent->IsReply()) {
		switch (pEvent->GetReplyType()) {
		case (int)ILifecycleManager::EEventType::eStopComponents:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eFinalizeComponents);
			break;
		case (int)ILifecycleManager::EEventType::eFinalizeComponents:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eStopSchedulers);
			break;
		case (int)ILifecycleManager::EEventType::eStopSchedulers:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eFinalizeSchedulers);
			break;
		case (int)ILifecycleManager::EEventType::eFinalizeSchedulers:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eDeleteComponents);
			break;
		case (int)ILifecycleManager::EEventType::eDeleteComponents:
			LOG_FOOTER("LifecycleManager::FinalizeAsALifecycleManager");
			break;
		default:
			throw Exception((int)ILifecycleManager::EException::eInitializationReplyError, 
					"LifecycleManager::FinalizeAsALifecycleManager", __LINE__);
			break;
		}
	}
	else {
		LOG_HEADER("LifecycleManager::FinalizeAsALifecycleManager");
		this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eStopComponents);
	}
}

void LifecycleManager::ProcessAEvent(Event* pEvent) {
	switch (pEvent->GetType()) {
	case (int)ILifecycleManager::EEventType::eInitializeAsALifecycleManager:
		this->InitializeAsALifecycleManager(pEvent); break;
	case (int)ILifecycleManager::EEventType::eRegisterSchedulers:
		this->RegisterSchedulers(pEvent);  break;
	case (int)ILifecycleManager::EEventType::eInitializeSchedulers:
		this->InitializeSchedulers(pEvent); break;
	case (int)ILifecycleManager::EEventType::eStartSchedulers:
		this->StartSchedulers(pEvent); break;
	case (int)ILifecycleManager::EEventType::eRegisterComponents:
		this->RegisterComponents(pEvent); break;
	case (int)ILifecycleManager::EEventType::eAllocateComponents:
		this->AllocateComponents(pEvent); break;
	case (int)ILifecycleManager::EEventType::eAssociateSendersNReceivers:
		this->AssociateSendersNReceivers(pEvent); break;
	case (int)ILifecycleManager::EEventType::eAssociateSourcesNTargets:
		this->AssociateSourcesNTargets(pEvent); break;
	case (int)ILifecycleManager::EEventType::eInitializeComponents:
		this->InitializeComponents(pEvent); break;

	case (int)ILifecycleManager::EEventType::eStartSystem:
		this->StartSystem(pEvent); break;
	case (int)ILifecycleManager::EEventType::eStopSystem:
		this->StopSystem(pEvent); break;

	case (int)ILifecycleManager::EEventType::eFinalizeAsALifecycleManager:
		this->FinalizeAsALifecycleManager(pEvent); break;
	case (int)ILifecycleManager::EEventType::eStopComponents:
		this->StopComponents(pEvent); break;
	case (int)ILifecycleManager::EEventType::eFinalizeComponents:
		this->FinalizeComponents(pEvent); break;
	case (int)ILifecycleManager::EEventType::eStopSchedulers:
		this->StopSchedulers(pEvent); break;
	case (int)ILifecycleManager::EEventType::eFinalizeSchedulers:
		this->FinalizeSchedulers(pEvent); break;
	case (int)ILifecycleManager::EEventType::eDeleteComponents:
		this->DeleteComponents(pEvent); break;
	default:
		Component::ProcessAEvent(pEvent); break;
	}
}
