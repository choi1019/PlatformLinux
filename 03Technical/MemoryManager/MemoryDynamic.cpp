#include <03Technical/MemoryManager/MemoryDynamic.h>
#include <03Technical/MemoryManager/SlotInfo.h>

#include <01Base/Object/ValueObject.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

#include <math.h>

PageList* MemoryDynamic::s_pPageList = nullptr;

void* MemoryDynamic::operator new(size_t szThis, const char* sMessage) {
    void* pMemoryDynamic = BaseObject::s_pMemory->SafeMalloc(szThis, sMessage);    
    SlotList::s_pSlotListRecycle = nullptr;
    return pMemoryDynamic;
}
void MemoryDynamic::operator delete(void* pObject) {
    MemoryObject::s_pMemory->SafeFree(pObject);
 }
void MemoryDynamic::operator delete(void* pObject, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "MemoryDynamic::delete", __LINE__);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
MemoryDynamic::MemoryDynamic(unsigned szSlotUnit, int nClassId, const char* pClassName)
    : MemoryObject(nClassId, pClassName)
    , m_szUnit(szSlotUnit)
{
    this->m_pMemoryEvenHead = new("MemoryDynamic::m_pMemoryEvenHead") MemoryEven(0);
    this->m_szUnitExponentOf2 = (unsigned)(log2(static_cast<double>(this->m_szUnit)));

    SlotInfo::s_pMemory = new("SlotInfo::s_pMemory") SlotList(sizeof(SlotInfo));
}

MemoryDynamic::~MemoryDynamic() {
}

void MemoryDynamic::Initialize() {
    MemoryObject::Initialize();
}
void MemoryDynamic::Finalize() {
    MemoryObject::Finalize();
}

// methods
void* MemoryDynamic::Malloc(size_t szObject, const char* sMessage) {
     // compute slot size
    size_t szSlot = szObject;
    szSlot >>= m_szUnitExponentOf2;
    szSlot <<= m_szUnitExponentOf2;
    if (szSlot < szObject) {
        szSlot += m_szUnit;
    }
    // find the MemoryEven
    MemoryEven *pTargetMemoryEven = nullptr;
    MemoryEven *pPrevious = nullptr;
    MemoryEven *pCurrent = m_pMemoryEvenHead;
    while (pCurrent != nullptr) {
        if (pCurrent->GetSzSlot() == szSlot) {
            // arleady exist
            pTargetMemoryEven = pCurrent;
        } else if (pCurrent->GetSzSlot() < szSlot) {
            if (pCurrent->GetPNext() == nullptr) {
                // add a new MemoryEven
                pTargetMemoryEven = new("MemoryEvenHead") MemoryEven(szSlot);
                pCurrent->SetPNext(pTargetMemoryEven);
                pTargetMemoryEven->SetPNext(nullptr);
            }
        } else if (pCurrent->GetSzSlot() > szSlot) {
            // insert a new MemoryEven
            pTargetMemoryEven = new("MemoryEvenHead") MemoryEven(szSlot);
            pPrevious->SetPNext(pTargetMemoryEven);
            pTargetMemoryEven->SetPNext(pCurrent);
        }
        // found
        if (pTargetMemoryEven != nullptr) {
            Slot *pTargetSlot = (Slot *)pTargetMemoryEven->SafeMalloc(szSlot, sMessage);
            return pTargetSlot;
        }
        pPrevious = pCurrent;
        pCurrent = (MemoryEven *)pCurrent->GetPNext();
    } 
    throw Exception((unsigned)IMemory::EException::_eSlotlistAllocationFailed, " MemoryDynamic::Malloc", __LINE__);
}

void* MemoryDynamic::Free(void* pObject) {
    MemoryEven *pPrevious = m_pMemoryEvenHead; 
    MemoryEven *pCurrent = (MemoryEven *)pPrevious->GetPNext(); 
    while (pCurrent != nullptr) {
        void* pSlotList = pCurrent->SafeFree(pObject);
        if (pSlotList != nullptr) {
            // delete size head?
            if (pCurrent->GetCountSlotLists() == 0) {
                pPrevious->SetPNext(pCurrent->GetPNext());
                delete pCurrent;
            }
            return pCurrent;
        }
        pCurrent = (MemoryEven *)pCurrent->GetPNext();
    }
    throw Exception((unsigned)IMemory::EException::_eSlotlistFreeFailed, "MemoryDynamic::Free", __LINE__);
    return nullptr;
}

void* MemoryDynamic::SafeMalloc(size_t szAllocate, const char* pcName)
{
    Lock();
    void* pMemoryAllocated = this->Malloc(szAllocate, pcName);
    UnLock();
    return pMemoryAllocated;
}
void* MemoryDynamic::SafeFree(void* pObject) {
    Lock();
    void* result = this->Free(pObject);
    UnLock();
    return result;
}

// maintenance
void MemoryDynamic::Show(const char* pTitle) {
    MLOG_HEADER("MemoryDynamic::Show(pTitle)", pTitle);
    s_pPageList->Show("MemoryDynamic::Show");
    MemoryEven* pMemoryEven = this->m_pMemoryEvenHead;
    while (pMemoryEven != nullptr) {
        pMemoryEven->Show("Slot Head");
        pMemoryEven = (MemoryEven *)pMemoryEven->GetPNext();
    }
    SlotInfo::s_pMemory->Show("SlotInfo Head ");
    MLOG_FOOTER("MemoryDynamic::Show");
};