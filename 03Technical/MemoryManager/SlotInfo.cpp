#include <03Technical/MemoryManager/SlotInfo.h>
#include <03Technical/MemoryManager/SlotList.h>
#include <01Base/Aspect/Log.h>

IMemory* SlotInfo::s_pMemory = nullptr;

void* SlotInfo::operator new (size_t szThis, const char* sMessage) {
    void* pAllocated = SlotInfo::s_pMemory->SafeMalloc(szThis, sMessage);
    return pAllocated;
}
void SlotInfo::operator delete(void* pObject) {
    SlotInfo::s_pMemory->SafeFree(pObject);
}
// dummy
void SlotInfo::operator delete(void* pObject, const char* sMessage) {
     throw Exception((unsigned)IMemory::EException::_eNotSupport, "SlotInfo::delete", (size_t)pObject);
}

SlotInfo::SlotInfo(Slot *pSlot, const char *sMessage, SlotList *pSlotList,
        int nObjectId, const char* sObjectName) 
    : RootObject(nObjectId, sObjectName)
    , m_pSlot(pSlot)
    , m_pSlotList(pSlotList)
    , m_pNext(nullptr)
{
    strcpy(m_sMessage, sMessage);
}
SlotInfo::~SlotInfo() {}
void SlotInfo::Initialize() {
    RootObject::Initialize();
}
void SlotInfo::Finalize() {
    RootObject::Finalize();
}

void SlotInfo::Show(const char* sMessage) {
    MLOG_NEWLINE("SlotInfo::Show()", sMessage, (size_t)m_pSlot, m_sMessage);
}