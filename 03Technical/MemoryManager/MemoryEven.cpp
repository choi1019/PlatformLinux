#include <03Technical/MemoryManager/MemoryEven.h>
#include <03Technical/MemoryManager/SlotInfo.h>
#include <01Base/Aspect/Log.h>

// for head MemoryEven
MemoryEven::MemoryEven(size_t szSlot, int nClassId, const char* pClassName)
    : SlotList(szSlot, nClassId, pClassName)
    , m_pSlotInfoHead(nullptr)
    {
    }
// for normal MemoryEven
MemoryEven::MemoryEven(size_t szSlot, int numMaxSlots, int numPagesRequired, SlotList *pSlotListHead,
            int nClassId,     const char* pClassName)
    : SlotList(szSlot, numMaxSlots, numPagesRequired, pSlotListHead, nClassId, pClassName)
    , m_pSlotInfoHead(nullptr)
    {
    }
MemoryEven::~MemoryEven(){}

void MemoryEven::Initialize() {
    SlotList::Initialize();
}
void MemoryEven::Finalize() {
    SlotList::Finalize();
}

void MemoryEven::AddSlotInfo(Slot *pSlot, const char *sMessage) {
    SlotInfo *pSlotInfo = new("SlotInfo") SlotInfo(pSlot, sMessage, this);
    pSlotInfo->SetPNext(m_pSlotInfoHead);
    m_pSlotInfoHead = pSlotInfo;
    MLOG_NEWLINE("AddSlotInfo", (size_t)pSlot, sMessage);
}

void MemoryEven::DeleteSlotInfo(Slot *pSlot) {
    SlotInfo *pPrevious = nullptr;
    SlotInfo *pCurrent = m_pSlotInfoHead;
    while (pCurrent != nullptr) {
        if (pCurrent->GetPSlot() == pSlot) {
            if (pPrevious == nullptr) {
                m_pSlotInfoHead = pCurrent->GetPNext();
            } else {
                pPrevious->SetPNext(pCurrent->GetPNext());
            }
            MLOG_NEWLINE("DeleteSlotInfo", (size_t)pSlot, pCurrent->GetSMessage());
            delete pCurrent;
            return;
        }
        pPrevious = pCurrent;
        pCurrent = pCurrent->GetPNext();
    }
    LOG_NEWLINE("Error-MemoryEvent::Delete", (size_t)pSlot);
    throw EXCEPTION(-1);
}

SlotInfo *MemoryEven::GetPSlotInfo(Slot *pSlot) {
    SlotInfo *pSlotInfo = m_pSlotInfoHead;
    while (pSlotInfo != nullptr) {
        if (pSlotInfo->GetPSlot() == pSlot) {
            return pSlotInfo;
        }
    }
    return nullptr;
}

void* MemoryEven::SafeMalloc(size_t szSlot, const char* sMessage) {
    MemoryEven *pTargetSlotList = nullptr;

    MemoryEven *pSibling = this;
    while (pSibling != nullptr) {
        if (pSibling->m_numSlots > 0) {
            pTargetSlotList = pSibling;
            break;
        }
        pSibling = (MemoryEven *)pSibling->GetPSibling();
    }
    if (pTargetSlotList == nullptr) {
        // add a new SlotList
        pTargetSlotList = new("SlotList") MemoryEven(m_szSlot, m_numMaxSlots, m_numPagesRequired, this);
        m_nCountSlotLists++;
        // insert a new sibling
        pTargetSlotList->SetPSibling(this->GetPSibling());
        this->SetPSibling(pTargetSlotList);        
    }
    Slot *pSlot = pTargetSlotList->AllocateASlot(sMessage);
    this->AddSlotInfo(pSlot, sMessage);
    NEW_DYNAMIC("MemoryEven", (size_t)pSlot, sMessage, "(szSlot, numSlots)", this->GetSzSlot(), this->GetNumSlots());   
    return pSlot;
}

void* MemoryEven::SafeFree(void* pObject) {
    Lock();
    SlotList *pSlotList = (SlotList *)SlotList::SafeFree(pObject);
    DELETE_DYNAMIC(pObject, this->GetPSlotInfo((Slot *)pObject)->GetSMessage());
    this->DeleteSlotInfo((Slot*)pObject);
    UnLock();
    return nullptr;
}

// maintenance
void MemoryEven::Show(const char* pTitle) {
    MLOG_HEADER("SlotList-Head(", m_szSlot, ")::Show(numMaxSlots)", m_numMaxSlots, pTitle);
    SlotList *pSibling = this->GetPSibling();
    while (pSibling != nullptr) {
        MLOG_HEADER("Slots-", pSibling->GetIdxPage(), "(m_numSlots)", pSibling->GetNumSlots());
            Slot* pSlot = pSibling->GetPSlotHead();
            while (pSlot != nullptr) {
                MLOG_NEWLINE("Slot-", (size_t)pSlot);
                pSlot = pSlot->pNext;
            }
        MLOG_FOOTER("Slots-", pSibling->GetIdxPage());
        MLOG_HEADER("SlotsInfo-");
            SlotInfo* pSlotInfo = ((MemoryEven*)pSibling)->GetPSlotInfoHead();
            while (pSlotInfo != nullptr) {
                pSlotInfo->Show(">");
                pSlotInfo = pSlotInfo->GetPNext();
            }
        MLOG_FOOTER("SlotsInfo-");
        pSibling = pSibling->GetPSibling();
    }
    MLOG_FOOTER("SlotList-Head(", m_szSlot, ")::Show()");
};
