#pragma once

#include <13PTechnical/typedef.h>
#define _PSkeleton_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePSkeleton) 
#define _PSkeleton_Name "Skeleton"

class IPSkeleton {
public:
enum class EException {
		eBegin = _PSkeleton_Id,
        eSocket,
        eBind,
        eListen,
        eAccept,
		eRead,
		eWrite,
		eEnd
	};

	enum class EEventType {
		eBegin = _PSkeleton_Id,
		eInitialize,
		eStart,
		eStop,
		eEnd
	};

	enum class EParts {
		eBegin = _PSkeleton_Id,
		ePSkeletonPart,
		eEnd
	};
	enum class EReceivers {
		eBegin = _PSkeleton_Id,
		Skeleton_Id
	};
	enum class ETargetGroups {
		eBegin = _PSkeleton_Id,
		eEnd
	};
	enum class EAttributes {
		eBegin = _PSkeleton_Id,
		eEnd
	};

	// class ParamAssociateAReceiver : public ValueObject {
	// private:
	// 	unsigned m_uReceiverName;
	// 	UId m_uIdReceiverComponent;
	// public:
	// 	ParamAssociateAReceiver(unsigned uReceiverName, UId uIdReceiverComponent)
	// 		: m_uReceiverName(uReceiverName)
	// 		, m_uIdReceiverComponent(uIdReceiverComponent)
	// 	{}
	// 	~ParamAssociateAReceiver() {}
	// 	unsigned GetReceiverName() { return this->m_uReceiverName; }
	// 	UId GetUIdReceiverComponent() { return this->m_uIdReceiverComponent; }
	// };

	// class ParamAssociateATarget : public ValueObject {
	// private:
	// 	unsigned m_uGroupName;
	// 	Vector<UId> m_vUIdTargetComponents;
	// public:
	// 	ParamAssociateATarget(unsigned uGroupName)
	// 		: m_uGroupName(uGroupName)
	// 	{}
	// 	~ParamAssociateATarget() {}

	// 	unsigned GetGroupName() { return this->m_uGroupName; }
	// 	Vector<UId>& GetVUIdTargetComponents() { return this->m_vUIdTargetComponents; }
	// };
};
