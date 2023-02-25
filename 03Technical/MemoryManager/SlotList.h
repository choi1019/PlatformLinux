#pragma once

#include <03Technical/typedef.h>
#define _SlotList_Id _GET_CLASS_UID(_ELayer_Technical::_eSlotList)
#define _SlotList_Name "SlotList"

#include <01Base/Memory/IMemory.h>
#include <03Technical/MemoryManager/MemoryObject.h>
#include <03Technical/MemoryManager/PageList.h>

class Slot {
public:
	Slot* pNext;
};

class SlotList : public MemoryObject, public IMemory {
public:
	enum class EException {
		eBegin = _SlotList_Id,
		eNoMoreSlot,
		eFreeError,
		eEnd
	};

	// for recycle
	static SlotList* s_pSlotListRecycle;
	static PageList* s_pPageList;

	void* operator new(size_t szThis, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, const char* sMessage);

protected:
	int m_numPagesRequired;
	int m_idxPage;
	PageIndex* m_pPageIndex;
	int m_nCountSlotLists;

	size_t m_szSlot;
	unsigned m_numMaxSlots;
	unsigned m_numSlots;
	bool m_bGarbage;
	Slot* m_pSlotHead;

	// head SlotList of the same size
	SlotList *m_pSlotListHead;
	SlotList* m_pSibling;
	SlotList* m_pNext;

protected:
	virtual void Lock() {};
	virtual void UnLock() {};
public:
	int GetIdxPage() { return this->m_idxPage; }
	PageIndex *GetPPageIndex() { return this->m_pPageIndex; }
	// as a head SlotList
	int GetCountSlotLists() { return this->m_nCountSlotLists; }

	size_t GetSzSlot() { return this->m_szSlot; }
	unsigned GetNumSlots() { return this->m_numSlots; }
	bool IsGarbage() { return this->m_bGarbage; }

	SlotList* GetPSlotListHead() { return this->m_pSlotListHead;}
	SlotList* GetPNext() { return this->m_pNext; }
	void SetPNext(SlotList* pNext) { this->m_pNext = pNext; }
	SlotList* GetPSibling() { return this->m_pSibling; }
	void SetPSibling(SlotList* pSibling) { this->m_pSibling = pSibling; }

	Slot *GetPSlotHead() { return this->m_pSlotHead; }
	Slot *AllocateASlot(const char* sMessage);
	void DelocateASlot(Slot *pSlot);

public:
	// for head SlotList
	SlotList(size_t szSlot, 
		int nClassId = _SlotList_Id,
		const char* pClassName = _SlotList_Name);
	// for normal SlotList
	SlotList(size_t szSlot, int numMaxSlots, int numPagesRequired, SlotList *pSlotListHead,
		int nClassId = _SlotList_Id,
		const char* pClassName = _SlotList_Name);
	virtual ~SlotList();
	virtual void Initialize();
	virtual void Finalize();

	virtual void* SafeMalloc(size_t szSlot, const char* pcName);
	virtual void* SafeFree(void* pSlot);

	// maintenance
	virtual void Show(const char* pTitle);
};

