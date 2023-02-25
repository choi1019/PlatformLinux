#pragma once

#include <03Technical/typedef.h>
#define _MemoryDynamic_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryDynamic)
#define _MemoryDynamic_Name "MemoryDynamic"

#include <01Base/Memory/IMemory.h>
#include <03Technical/MemoryManager/MemoryObject.h>
#include <03Technical/MemoryManager/PageList.h>
#include <03Technical/MemoryManager/MemoryEven.h>

class MemoryDynamic :public MemoryObject, public IMemory
{
public:
	static PageList* s_pPageList;

	void* operator new(size_t szThis, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, const char* sMessage);

private:
	// attributes
	unsigned m_szUnit;
	MemoryEven* m_pMemoryEvenHead;
	unsigned m_szUnitExponentOf2;

protected:
	// critical section
	virtual void Lock() = 0;
	virtual void UnLock() = 0;

	void* Malloc(size_t szObject, const char* sMessage);
	void* Free(void* pObject);

public:
	// constructors and destructors
	MemoryDynamic(
		unsigned szSlotUnit,
		int nClassId = _MemoryDynamic_Id,
		const char* pClassName = _MemoryDynamic_Name);
	virtual ~MemoryDynamic();;

	virtual void Initialize();
	virtual void Finalize();

	// methods
	void* SafeMalloc(size_t szAllocate, const char* sMessage) override;
	void* SafeFree(void* pObject) override;

	// maintenance
	void Show(const char* pTitle) override;
};
