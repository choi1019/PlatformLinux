#pragma once

#include <02Platform/typedef.h>
#define Component_Id _GET_CLASS_UID(_ELayer_Platform::_eComponent) 
#define Component_Name "Component"

#include <01Base/Object/ValueObject.h>
#include <01Base/StdLib/Vector.h>

#include <02Platform/Component/UId.h>

class IComponent {
public:
	enum class EState {
		eBegin = Component_Id,
		eCreated,
		eAllocated,
		eAssociated,
		eTargeted,
		eInitialized,
		eRunning,
		ePaused,
		eStopped,
		eFinalized,
		eEnd
	};

	enum class EException {
		eBegin = Component_Id,
		eNotAllocated,
		eReceiverNotFound,
		eNotAssociated,
		eNotTargeted,
		eEventNotSupported,
		eNotStopped,
		eEnd
	};

	enum class EEventType {
		eBegin = Component_Id,

		eAssociateAReceiver,
		eAssociateATarget,

		eEnd
	};

	enum class EParts {
		eBegin = Component_Id,
		eEnd
	};
	enum class EReceivers {
		eBegin = Component_Id,
		eThis,
		eLifecycleManager,
		eScheduler,
		eExceptionManager,
		eLogManager,
		eEnd
	};
	enum class ETargetGroups {
		eBegin = Component_Id,
		eEnd
	};
	enum class EAttributes {
		eBegin = Component_Id,
		eEnd
	};

	class ParamAssociateAReceiver : public ValueObject {
	private:
		unsigned m_uReceiverName;
		UId m_uIdReceiverComponent;
	public:
		ParamAssociateAReceiver(unsigned uReceiverName, UId uIdReceiverComponent)
			: m_uReceiverName(uReceiverName)
			, m_uIdReceiverComponent(uIdReceiverComponent)
		{}
		~ParamAssociateAReceiver() {}
		unsigned GetReceiverName() { return this->m_uReceiverName; }
		UId GetUIdReceiverComponent() { return this->m_uIdReceiverComponent; }
	};

	class ParamAssociateATarget : public ValueObject {
	private:
		unsigned m_uGroupName;
		Vector<UId> m_vUIdTargetComponents;
	public:
		ParamAssociateATarget(unsigned uGroupName)
			: m_uGroupName(uGroupName)
		{}
		~ParamAssociateATarget() {}

		unsigned GetGroupName() { return this->m_uGroupName; }
		Vector<UId>& GetVUIdTargetComponents() { return this->m_vUIdTargetComponents; }
	};
};
