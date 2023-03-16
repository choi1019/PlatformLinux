#pragma once

#include <21Domain/typedef.h>
#define _VideoProviderManager_Id _GET_CLASS_UID(_ELayer_Domain::_eVideoProviderManager)
#define _VideoProviderManager_Name "VideoProviderManager"

#include <01Base/Object/ValueObject.h>
#include <01Base/StdLib/String.h>

class IVideoProviderManager {
public:
	///////////////////////////////////////
	// Component Parts
	///////////////////////////////////////
	enum class EPart {
		eBegin = _VideoProviderManager_Id,
		eVideoProvider,
		eEnd
	};

	////////////////////////////////////////
	// Required interface
	////////////////////////////////////////
	enum class EReceivers {
		eBegin = _VideoProviderManager_Id,
		eVideoRequesterManager,
		ePStub,
		eEnd
	};

	///////////////////////////////////////
	// Provuded Interface
	///////////////////////////////////////
	// event type
	enum class EEventType {
		eBegin = _VideoProviderManager_Id,
		eInitialize,
		eStart,
		eEnd
	};

	//////////////////////////////////////
	// Process : Parameter & Result Type
	//////////////////////////////////////
	// paramenter
	class ParamCompute : public ValueObject {
	private:
		int m_nAtt1;
		String m_sAtt2;

	public:
		ParamCompute(int nAtt1, String sAtt2) {
			m_nAtt1 = nAtt1;
			m_sAtt2 = sAtt2;
		}
		~ParamCompute() {
		}
		int GetAtt1() { return this->m_nAtt1; }
		String GetAtt2() { return this->m_sAtt2; }
	};

	// result
	class Result : public ValueObject {
	private:
		int m_nAtt1;
		String m_sAtt2;
	public:
		Result(int nAtt1, String sAtt2) {
			m_nAtt1 = nAtt1;
			m_sAtt2 = sAtt2;
		}
		~Result() {
		}
		int GetAtt1() { return this->m_nAtt1; }
		String GetAtt2() { return this->m_sAtt2; }
	};
};