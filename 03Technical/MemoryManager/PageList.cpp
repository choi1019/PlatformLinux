#include <03Technical/MemoryManager/PageList.h>
#include <03Technical/MemoryManager/SlotList.h>

PageList::PageList(
    size_t pMemeoryAllocated, 
    size_t szMemoryAllocated, 
    size_t szPage,
    int nClassId,
    const char* pClassName)
    : MemoryObject(nClassId, pClassName)
    , m_pMemeoryAllocated(pMemeoryAllocated)
    , m_szPage(szPage)
{
    if (szMemoryAllocated < m_szPage) {
        throw Exception((unsigned)(IMemory::EException::_eMemoryAllocatedIsSmallerThanAPage)
                            , "PageList", "PageList", "MemoryTooSmall");
    }
    this->m_numPagesMax = szMemoryAllocated / m_szPage;
    this->m_numPagesAvaiable = this->m_numPagesMax;

    // operator new[] for pointer array
    // this->m_apPageIndices = new("") PageIndex*[m_numPagesMax];
    this->m_apPageIndices = (PageIndex**)(MemoryObject::operator new(sizeof(PageIndex*)*m_numPagesMax, "m_apPageIndices**"));
    for (unsigned index = 0; index < this->m_numPagesMax; index++) {
        m_apPageIndices[index] = new((String("m_apPageIndices-")+index).c_str()) PageIndex(index, pMemeoryAllocated);
        pMemeoryAllocated = pMemeoryAllocated + m_szPage;
    }
}
PageList::~PageList() {
}
void PageList::Initialize() {
    MemoryObject::Initialize();
    SlotList::s_pPageList = this;
    
}
void PageList::Finalize() {
    MemoryObject::Finalize();
}

PageIndex* PageList::AllocatePages(unsigned numPagesRequired, SlotList *pSlotList) {
    if (m_numPagesAvaiable < numPagesRequired) {
        throw Exception((unsigned)IMemory::EException::_eNoMorePage, "Memory", "Malloc", "_eNoMorePage");
    } else {
        // if numPagesRequired > 1
        unsigned numPagesAllocated = numPagesRequired;
        unsigned indexFound = 0;
        for (unsigned index = 0; index < m_numPagesMax; index++) {
            if ((m_apPageIndices[index]->IsAllocated())) {
                // if the page is discontinued, reset index
                numPagesAllocated = numPagesRequired;
                indexFound = index + 1;
            }
            else {
                numPagesAllocated--;
                if (numPagesAllocated == 0) {
                    // found - page info is set in a head page
                    m_apPageIndices[indexFound]->SetNumAllocated(numPagesRequired);
                    m_apPageIndices[indexFound]->SetPSlotList(pSlotList);
                    for (unsigned i = 0; i < numPagesRequired; i++) {
                        // multiple pages needed
                        m_apPageIndices[indexFound + i]->SetIsAllocated(true);
                    }
                    m_numPagesAvaiable = m_numPagesAvaiable - numPagesRequired;
                    return m_apPageIndices[indexFound];
                }
            }
        }
        // not found
        // need compaction =====
        throw Exception((unsigned)IMemory::EException::_eNoMorePage, "Memory", "Malloc", "_eNoMorePage");
    }
}

void PageList::FreePages(unsigned indexFree) {
    unsigned numPagesAllocated = m_apPageIndices[indexFree]->GetNumAllocated();
    for (unsigned i = 0; i < numPagesAllocated; i++) {
        m_apPageIndices[indexFree + i]->SetNumAllocated(1);
        m_apPageIndices[indexFree + i]->SetIsAllocated(false);
    }
    m_numPagesAvaiable = m_numPagesAvaiable + numPagesAllocated;
}

void PageList::Show(const char* pTitle) {
    MLOG_HEADER("PageList::Show(szPage,numPagesAvaiable,numPagesMax)", m_szPage, m_numPagesAvaiable, m_numPagesMax);
    for (unsigned i=0; i< m_numPagesMax; i++) {
        m_apPageIndices[i]->Show("Page");
    }
    MLOG_FOOTER("PageList::Show");
}