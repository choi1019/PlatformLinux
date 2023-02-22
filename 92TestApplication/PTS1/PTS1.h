#pragma once

#include <92TestApplication/typedef.h>
#define _PTS1_Id _GET_TESTLAYER_UID(_ELayer_TestApplication::_ePTS1)
#define _PTS1_Name "PTS1"
#include <92TestApplication/PTS1/Config.h>

#include <91TestPlatform/TestCase/TestSuite.h>
#include <13PTechnical/PMemoryManager/PMemoryStatic.h>
#include <13PTechnical/PMemoryManager/PMemoryDynamic.h>

#include <92TestApplication/PTS1/PTC11/PTC11.h>
#include <92TestApplication/PTS1/PTC12/PTC12.h>
#include <92TestApplication/PTS1/PTC13/PTC13.h>
#include <92TestApplication/PTS1/PTC14/PTC14.h>

template <int SIZE_SYSTEM_MEMORY, int SIZE_USER_MEMORY, int SIZE_PAGE, int SIZE_SLOT_UNIT>
class PTS1: public TestSuite {
private:
	size_t m_szSystemMemory;
	char* m_pSystemMemeoryAllocated;
	PMemoryStatic* m_pMemoryStatic;

	size_t m_szUserMemory;
	char* m_pUserMemeoryAllocated;
	PMemoryDynamic* m_pMemoryDynamic;

public:
	PTS1(
		unsigned classId = _PTS1_Id,
		const char* pClassName = _PTS1_Name)
		: TestSuite(classId, pClassName)
		, m_pMemoryStatic(nullptr)
		, m_pMemoryDynamic(nullptr)
		, m_pSystemMemeoryAllocated(nullptr)
		, m_pUserMemeoryAllocated(nullptr)
	{
	}
	virtual ~PTS1() {
	}

	void Initialize() {
		try {
			// system memory allocation
			m_szSystemMemory = SIZE_SYSTEM_MEMORY;
			m_pSystemMemeoryAllocated = new char[m_szSystemMemory];
			m_pMemoryStatic = new(m_pSystemMemeoryAllocated, m_szSystemMemory, "PTS1") PMemoryStatic();
			m_pMemoryStatic->Initialize();
			m_pMemoryStatic->Show("m_pMemoryStatic::Initialize()");

			// aplication memorty allocation
			m_szUserMemory = SIZE_USER_MEMORY;
			m_pUserMemeoryAllocated = new char[m_szUserMemory];
			PMemoryDynamic::s_pPageList = new("PageList") PageList((size_t)m_pUserMemeoryAllocated, m_szUserMemory, SIZE_PAGE);
			m_pMemoryDynamic = new("PTS1") PMemoryDynamic(SIZE_SLOT_UNIT);
			m_pMemoryDynamic->Initialize();
			m_pMemoryDynamic->Show("m_pMemoryDynamic::Initialize()");

			this->add(new("PTC11") PTC11());
			this->add(new("PTC12") PTC12());
			this->add(new("PTC13") PTC13());
			this->add(new("PTC14") PTC14());
			this->add(new("PTC11") PTC11());
			this->add(new("PTC12") PTC12());
		}
		catch (Exception& exception) {
			exception.Println();
			exit(1);
		}
	}

	void Finalize() {
		try {
			// delete m_pMemory;
			m_pMemoryDynamic->Finalize();
			m_pMemoryDynamic->Show("");
			delete m_pMemoryDynamic;
			delete[] m_pUserMemeoryAllocated;

			m_pMemoryStatic->Finalize();
			m_pMemoryStatic->Show("");
			delete m_pMemoryStatic;
			delete[] m_pSystemMemeoryAllocated;
		}
		catch (Exception& exception) {
			exception.Println();
			exit(2);
		}
	}
};

