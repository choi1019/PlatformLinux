#pragma once

#include <09Application/typedef.h>
#define _LifecycleManager_Id _GET_CLASS_UID(_ELayer_Application::_eLifecycleManager)
#define _LifecycleManager_Name "LifecycleManager"

#include <01Base/Object/ValueObject.h>
#include <02Platform/Scheduler/Scheduler.h>

class ILifecycleManager {
public:
	enum class EEventType {
		eBegin = _LifecycleManager_Id,

		eInitializeAsALifecycleManager,
		eRegisterSchedulers,
		eInitializeSchedulers,
		eStartSchedulers,
		eRegisterComponents,
		eAllocateComponents,
		eAssociateSendersNReceivers,
		eAssociateSourcesNTargets,
		eInitializeComponents,

		eStartSystem,
		eStopSystem,

		eFinalizeAsALifecycleManager,
		eFinalizeComponents,
		eStopComponents,
		eStopSchedulers,
		eFinalizeSchedulers,	
		eDeleteComponents,	
		eEnd
	};


	enum class EReceivers {
		eBegin = _LifecycleManager_Id,
		eMainScheduler,
		eLifecycleManager,
		eExceptionManager,
		eLogManager,
		eEnd
	};

	enum class EException {
		eErrorCodeBegin = _LifecycleManager_Id,
		eEventTypeError,
		eSchedulerNotRegistered,
		eComponentNotRegistered,
		eInitializationReplyError,
		eErrorCodeEnd
	};

	class ParamInitializeAsALifecycleManager : public ValueObject {
	private:
		Scheduler* m_pMainScheduler;
	public:
		ParamInitializeAsALifecycleManager(Scheduler* pMainScheduler) {
			m_pMainScheduler = pMainScheduler;
		}
		~ParamInitializeAsALifecycleManager() {
		}
		Scheduler* GetPMainScheduler() {
			return this->m_pMainScheduler;
		}
	};
};
