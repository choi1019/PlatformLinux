#pragma once

#include <91TestPlatform/typedef.h>
#define _TestObject_Id _GET_TESTCLASS_UID(_ELayer_TestPlatform::_eTestObject)
#define _TestObject_Name "TestObject"

class TestObject
{
public:
	// class variable
	static unsigned s_uCounter;

	// static members
	void* operator new(size_t szThis, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, const char* sMessage);

private:
	unsigned m_uObjectId;
	unsigned m_nClassId;
	const char* m_pcClassName;
	size_t m_szThis;

public:
	// getters and setters
	inline unsigned GetObjectId() { return this->m_uObjectId; }
	inline unsigned GetClassId() { return this->m_nClassId; }
	inline const char* GetClassName() { return this->m_pcClassName; }

	inline size_t GetSize() { return this->m_szThis; }
	inline void SetSize(size_t szThis) { this->m_szThis = szThis; }

public:
	TestObject(
		unsigned nClassId = _TestObject_Id,
		const char *pcClassName = _TestObject_Name);
	virtual ~TestObject();

	void BeforeInitialize();
	virtual void Initialize();
	void BeforeRun();
	virtual void Run();
	void AfterRun();
	virtual void Finalize();
	void AfterFinalize();
};

