#pragma once

#include <91TestPlatform/typedef.h>
#define _TestCase_Id _GET_TESTCLASS_UID(_ELayer_TestPlatform::_eTestCase)
#define _TestCase_Name "TestCase"

#include <91TestPlatform/TestObject/TestObject.h>
#include <91TestPlatform/TestAspect/TestException.h>
#include <91TestPlatform/TestAspect/TestLog.h>

class TestCase: public TestObject
{
public:
	enum class EAssertion {
		_eBegin = _TestCase_Id,
		_eNotNull,
		_eEqual,
		_eNotEqual,
		_eGT,
		_eEnd
	};

public:
	TestCase(
		unsigned nClassId = _TestCase_Id,
		const char *pClassName = _TestCase_Name)
		: TestObject(nClassId, pClassName)
	{}
	virtual ~TestCase()
	{}

	virtual void Initialize() {}
	virtual void Finalize() {}
	virtual void Run() {}

public:
	void BeforeInitialize() { 
		TESTLOG_HEADER(this->GetClassName(), "Initialize", "Start");
	}
	void BeforeRun() {
		TESTLOG_FOOTER(this->GetClassName(), "Initialize", "End");
    	TESTLOG_HEADER(this->GetClassName(), "Run", "Start");
	}
	void AfterRun() {
		TESTLOG_FOOTER(this->GetClassName(), "Run", "End");
		TESTLOG_HEADER(this->GetClassName(), "Finalize", "Start");
	}
	void AfterFinalize() {
    	TESTLOG_FOOTER(this->GetClassName(), "Finalize", "End\n")
	}

protected:
	void AssertNotNull(void* pObject) {
		if (pObject == nullptr) {
			throw TestException((unsigned)EAssertion::_eNotNull, this->GetClassName(), __func__, "Test Failed");
		}
	}
	void AssertEqual(unsigned x, unsigned y) {
		if (x != y) {
			throw TestException((unsigned)EAssertion::_eEqual, this->GetClassName(), __func__, "Test Failed");
		}
	}
	void AssertEqual(int x, int y) {
		if (x != y) {
			throw TestException((unsigned)EAssertion::_eEqual, this->GetClassName(), __func__, "Test Failed");
		}
	}
	void AssertEqual(long long x, long long y) {
		if (x != y) {
			throw TestException((unsigned)EAssertion::_eEqual, this->GetClassName(), __func__, "Test Failed");
		}
	}
};

