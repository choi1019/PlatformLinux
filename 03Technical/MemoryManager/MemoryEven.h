#pragma once

#include <03Technical/typedef.h>
#define _MemoryEven_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryEven)
#define _MemoryEven_Name "MemoryEven"

#include <03Technical/MemoryManager/SlotList.h>
#include <03Technical/MemoryManager/SlotInfo.h>

class MemoryEven : public SlotList {
private:
	SlotInfo *m_pSlotInfoHead;

	void AddSlotInfo(Slot *pSlot, const char *sMessage);
	void DeleteSlotInfo(Slot *pSlot);
	SlotInfo *GetPSlotInfo(Slot *pSlot);
public:
	SlotInfo *GetPSlotInfoHead() { return this->m_pSlotInfoHead; }
protected:
	// critical section
	virtual void Lock() {};
	virtual void UnLock() {};

public:
	// for head MemoryEven
	MemoryEven(size_t szSlot, 
		int nClassId = _MemoryEven_Id,
		const char* pClassName = _MemoryEven_Name);
	// for normal MemoryEven
	MemoryEven(size_t szSlot, int numMaxSlots, int numPagesRequired, SlotList *pSlotListHead,
		int nClassId = _MemoryEven_Id,
		const char* pClassName = _MemoryEven_Name);
	virtual ~MemoryEven();
	virtual void Initialize();
	virtual void Finalize();

	void* SafeMalloc(size_t szAllocate, const char* sMessage) override;
	void* SafeFree(void *pObject) override;

	// maintenance
	virtual void Show(const char* pTitle);
};

