#include <02Platform/Component/ComponentPart.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

#include <01Base/Aspect/Directory.h>
#include <01Base/StdLib/String.h>
#include <02Platform/EventQueue/EventQueue.h>

ComponentPart::ComponentPart(unsigned uClassId, const char* acClassName)
	: BaseObject(uClassId, acClassName)
	, m_pUId(nullptr)
	, m_pmReceivers(nullptr)
	, m_pmTargetsGroups(nullptr)
//	, m_pSequenceHead(nullptr)
	, m_pEventParent(nullptr) 
{
}

ComponentPart::~ComponentPart() {
}

void ComponentPart::Initialize() {
	BaseObject::Initialize();
}

void ComponentPart::Finalize() {
	BaseObject::Finalize();
}

UId ComponentPart::FindUid(int nReceiverName) {
	auto iterator = this->m_pmReceivers->Find(nReceiverName);
	if (iterator == m_pmReceivers->end()) {
		throw Exception(
			(unsigned)EException::eNotAssociated,
			this->GetClassName(),
			__func__,
			String("Receiver(") + String(nReceiverName) + ") is not Associated"
		);
	}
	return iterator->second;
}

// for transaction
void ComponentPart::BeginSequence(Event* pEvent) {
	m_pEventParent = pEvent;
	if (pEvent->IsReply()) {
		// nested, 
		if (pEvent->GetPParent() != nullptr) {
			pEvent->SetReplyType(pEvent->GetType());
			pEvent->SetType(pEvent->GetPParent()->GetType());
			pEvent->GetPParent()->DecrementCountChildren();

			m_pEventParent = pEvent->GetPParent();
		}
	} 
}

void ComponentPart::EndSequence(Event*pEvent) {
	if (pEvent->IsReply()) {
		if (pEvent->GetPParent() != nullptr) {
			if (pEvent->GetPParent()->GetCoundChildren() == 0) {
				if (pEvent->GetPParent()->IsSynchronous()) {
					ReplyEvent(pEvent->GetPParent());
				} 
			}
		}
		delete pEvent;	
	} else {
		if (pEvent->GetCoundChildren() == 0) {
			if (pEvent->IsSynchronous()) {
				ReplyEvent(pEvent);
			} else {
				delete pEvent;					
			}
		}
	}
}

///////////////////////////////////////
// send a event
///////////////////////////////////////
void ComponentPart::SendAEvent(Event* pEvent) {
	if (pEvent->GetUIdTarget().GetPEventQueue() == nullptr) {
		throw Exception(
			(unsigned)EException::eNotAllocated,
			this->GetClassName(),
			__func__,
			"EventQueue is not allocated"
		);
	}
	pEvent->GetUIdTarget().GetPEventQueue()->PushBack(pEvent);
}

///////////////////////////////////////
// reply a event
///////////////////////////////////////
void ComponentPart::ReplyEvent(Event* pEvent) {
	// set pEvent as a Reply
	pEvent->SetBReply(true);
	// swap source and destination
	pEvent->SetUIdTarget(pEvent->GetUIdSource());
	pEvent->SetUIdSource(*m_pUId);
	// Send event
	pEvent->GetUIdTarget().GetPEventQueue()->PushBack(pEvent);
}

///////////////////////////////////////
// send a synchronous event
///////////////////////////////////////
void ComponentPart::SendReplyEvent(UId uIdTarget, int nEventType, long long lArg, ValueObject* pArg, int ReplyType)
{
	Event* pEvent = new("Event") Event(*m_pUId, uIdTarget, nEventType, lArg, pArg, ReplyType);
	// event is synchronous
	pEvent->SetBSynchronous(true);
	if (m_pEventParent != nullptr) {
		pEvent->SetPParent(m_pEventParent);
		// parent event wait replying untill all the children finish
		pEvent->GetPParent()->IncrementCountChildren();
	}
	this->SendAEvent(pEvent);
}
void ComponentPart::SendReplyEvent(int nReceiverName, int nEventType, long long lArg, ValueObject* pArg, int ReplyType)
{
	UId uIdTarget = this->FindUid(nReceiverName);
	this->SendReplyEvent(uIdTarget, nEventType, lArg, pArg, ReplyType);
}

///////////////////////////////////////
// send an asynchronous event
///////////////////////////////////////
void ComponentPart::SendNoReplyEvent(UId uIdTarget, int nEventType, long long lArg, ValueObject* pArg)
{
	Event* pEvent = new("Event") Event(*m_pUId, uIdTarget, nEventType, lArg, pArg);
	pEvent->SetBSynchronous(false);
	this->SendAEvent(pEvent);
}
void ComponentPart::SendNoReplyEvent(int nReceiverName, int nEventType, long long lArg, ValueObject* pArg)
{
	UId uIdTarget = this->FindUid(nReceiverName);
	this->SendNoReplyEvent(uIdTarget, nEventType, lArg, pArg);
}

///////////////////////////////////////
// semd target events
///////////////////////////////////////
void ComponentPart::SendTargetEvents(unsigned groupName, unsigned eventType, long long lArg, ValueObject* pArg) {
	for (auto& itr : *(this->m_pmTargetsGroups->Find(groupName)->second))
	{
		this->SendNoReplyEvent(itr, eventType, lArg, pArg);
	}
}

