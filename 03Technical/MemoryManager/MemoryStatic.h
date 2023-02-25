#pragma once

#include <03Technical/typedef.h>
#define _MemoryStatic_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryStatic)
#define _MemoryStatic_Name "MemoryStatic"

#include <01Base/Memory/IMemory.h>
#include <03Technical/MemoryManager/MemoryObject.h>

class MemoryStatic : public MemoryObject, public IMemory {
public:
	// memory for static allocation - no delete
	static void* s_pAllocated;
	static size_t s_szAllocated;
	static void* s_pCurrent;
	static size_t s_szCurrent;

	void* operator new(size_t szThis, void* pMemoryAllocated, size_t szMemoryllocated, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, void* pMemoryAllocated, size_t szMemoryllocated);

protected:
	virtual void Lock() = 0;
	virtual void UnLock() = 0;

	void* Malloc(size_t szObject, const char* sMessage = "");
	void* Free(void* pObject);

public:
	MemoryStatic(
		int nClassId = _MemoryStatic_Id,
		const char* pClassName = _MemoryStatic_Name);
	virtual ~MemoryStatic();
	virtual void Initialize();
	virtual void Finalize();

		// methods
	void* SafeMalloc(size_t szAllocate, const char* sMessage = "") override;
	void* SafeFree(void* pObject) override;

	// maintenance
	void Show(const char* sMessage) override;
};