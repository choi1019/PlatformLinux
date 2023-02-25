#include <13PTechnical/PMemoryManager/PMemoryManager.h>

#include <13PTechnical/PMemoryManager/Config.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

#include <13PTechnical/PMemoryManager/PMemoryEven.h>
#include <02Platform/EventQueue/Event.h>
#include <03Technical/MemoryManager/SlotInfo.h>

size_t PMemoryManager::s_szSystemMemory = SIZE_MEMORY_SYSTEM;
char* PMemoryManager::s_pSystemMemeory = nullptr;
PMemoryStatic* PMemoryManager::s_pMemoryStatic = nullptr;

size_t PMemoryManager::s_szApplicationMemory = SIZE_MEMORY_APPLICATION;
char* PMemoryManager::s_pApplicationMemeory = nullptr;;
size_t PMemoryManager::s_szPage = SIZE_PAGE;
PageList *PMemoryManager::s_pPageList = nullptr;

size_t PMemoryManager::s_szSlotUnit = SIZE_SLOT_UNIT;
PMemoryDynamic* PMemoryManager::s_pMemoryDynamic = nullptr;;

void PMemoryManager::Allocate() {
    try {
        MLOG_HEADER("PMemoryManager::Allocate");
            // system memory
            s_pSystemMemeory = new char[s_szSystemMemory];
            s_pMemoryStatic = new(s_pSystemMemeory, s_szSystemMemory, "s_pMemoryStatic") PMemoryStatic();
            BaseObject::s_pMemory = s_pMemoryStatic;        
            SHOW_STATIC("PMemoryManager::Allocate");

            // aplication memory
            s_pApplicationMemeory = new char[s_szApplicationMemory];
            PageList *pPageList = new("pPageList") PageList((size_t)s_pApplicationMemeory, s_szApplicationMemory, s_szPage);
            // memory even 
            SlotList::s_pPageList = pPageList;
            Event::s_pMemory = new("Event::s_pMemory") PMemoryEven(sizeof(Event));
            // memory dynamic
            MemoryDynamic::s_pPageList = pPageList;
            s_pMemoryDynamic = new("s_pMemoryDynamic") PMemoryDynamic(s_szSlotUnit);
            ValueObject::s_pMemory = s_pMemoryDynamic;
            SHOW_DYNAMIC("PMemoryDynamic::Allocate");

        MLOG_FOOTER("PMemoryManager::Allocate");
    } catch (Exception& exception) {
        exception.Println();
    }
}

void PMemoryManager::Delocate() {
    try {
        MLOG_HEADER("PMemoryManager::Delocate");
        
        s_pMemoryDynamic->Finalize();
        SHOW_DYNAMIC("PMemoryDynamic::Delocate");
        delete s_pMemoryDynamic;

        //Event::s_pMemory->Finalize();
        delete Event::s_pMemory;

        s_pPageList->Finalize();
        delete s_pPageList;
        delete[] s_pApplicationMemeory;

        s_pMemoryStatic->Finalize();
        delete s_pMemoryStatic;
        delete[] s_pSystemMemeory;

        MLOG_FOOTER("PMemoryManager::Delocate");
    } catch (Exception& exception) {

    }
}