#pragma once

#include <01Base/typedef.h>
#define _Log_Id _GET_CLASS_UID(_ELayer_Base::_eLog)
#define _Log_Name "Log"

#include <01Base/Aspect/Aspect.h>
#include <01Base/StdLib/String.h>

class Log : public Aspect
{
protected:
	String m_sObject;
	String m_sFunction;
	String m_sMessage0;
	String m_sMessage1;
	String m_sMessage2;
	String m_sMessage3;
	String m_sMessage4;
//	time_t m_tTime;

public:
	String GetSource() { return this->m_sObject; }
	String GetFunction() { return this->m_sFunction; }
	String GetMessage0() { return this->m_sMessage0; }
	String GetMessage1() { return this->m_sMessage1; }
	String GetMessage2() { return this->m_sMessage2; }
	String GetMessage3() { return this->m_sMessage3; }
	String GetMessage4() { return this->m_sMessage4; }
	//	String GetTime() { return String(m_tTime); }

public:
	Log(String sObject = "",
		String sFunction = "",
		String sMessage0 = "",
		String sMessage1 = "",
		String sMessage2 = "",
		String sMessage3 = "",
		String sMessage4 = "",
		unsigned classId = _Log_Id,
		const char* pClassName = _Log_Name)
		: Aspect(classId, pClassName)
		, m_sObject(sObject)
		, m_sFunction(sFunction)
		, m_sMessage0(sMessage0)
		, m_sMessage1(sMessage1)
		, m_sMessage2(sMessage2)
		, m_sMessage3(sMessage3)
		, m_sMessage4(sMessage4)

	{
//		m_tTime = time(0);

	}
	virtual ~Log() {}

	virtual void Initialize() {
		Aspect::Initialize();
	}
	virtual void Finalize() {
		Aspect::Finalize();
	}
	/*
	void PrintTime() {
		printf(
			"\n%s%s: %s %s %s %s"
			, this->GetTab()
			, String(m_tTime).c_str()
			, m_sObject.c_str()
			, m_sFunction.c_str()
			, m_sMessage1.c_str()
			, m_sMessage2.c_str()
		);
	}
	*/
	void Print() {
		printf(
			"%s%s %s %s %s %s %s %s"
			, Aspect::GetTab()
			, m_sObject.c_str()
			, m_sFunction.c_str()
			, m_sMessage0.c_str()
			, m_sMessage1.c_str()
			, m_sMessage2.c_str()
			, m_sMessage3.c_str()
			, m_sMessage4.c_str()
			//			,String(m_tTime).c_str()
		);
		fflush(stdout);
	}
	void Println() {
		printf(
			"\n%s%s %s %s %s %s %s %s"
			, Aspect::GetTab()
			, m_sObject.c_str()
			, m_sFunction.c_str()
			, m_sMessage0.c_str()
			, m_sMessage1.c_str()
			, m_sMessage2.c_str()
			, m_sMessage3.c_str()
			, m_sMessage4.c_str()
			///			,String(m_tTime).c_str()
		);
		fflush(stdout);
	}
	void PrintHeader() {
		printf(
			"\n%s<<Begin:%s %s %s %s %s %s %s"
			, Aspect::GetTab()
			, m_sObject.c_str()
			, m_sFunction.c_str()
			, m_sMessage0.c_str()
			, m_sMessage1.c_str()
			, m_sMessage2.c_str()
			, m_sMessage3.c_str()
			, m_sMessage4.c_str()
			//			,String(m_tTime).c_str()
		);
		fflush(stdout);
		this->AddTab();
	}
	void PrintFooter() {
		this->RemoveTab();
		printf(
			"\n%sEnd>>%s %s %s %s %s %s %s\n"
			, Aspect::GetTab()
			, m_sObject.c_str()
			, m_sFunction.c_str()
			, m_sMessage0.c_str()
			, m_sMessage1.c_str()
			, m_sMessage2.c_str()
			, m_sMessage3.c_str()
			, m_sMessage4.c_str()
			//			,String(m_tTime).c_str()
		);
		fflush(stdout);
	}
};

#if _DEBUG
	#define LOG_HEADER(...) Log(__VA_ARGS__).PrintHeader()
	#define LOG_FOOTER(...) Log(__VA_ARGS__).PrintFooter()

	#define LOG_HEADER0(...) Log(this->GetClassName(), __func__, ##__VA_ARGS__).PrintHeader()
	#define LOG_FOOTER0(...) Log(this->GetClassName(), __func__, ##__VA_ARGS__).PrintFooter()

	#define LOG_NEWLINE(...) Log(__VA_ARGS__).Println()
	#define LOG(...) Log(__VA_ARGS__).Print()
//	#define LOG_TIME(...) Log(__VA_ARGS__).PrintTime()
#else
	#define LOG_HEADER(CLASSNAME, ...)
	#define LOG_FOOTER(...)
	
	#define LOG_HEADER0()
	#define LOG_FOOTER0()

	#define LOG_NEWLINE(CLASSNAME, ...)
	#define LOG(CLASSNAME, ...)
//	#define LOG_TIME(...) Log(__VA_ARGS__).PrintTime()
#endif

#if _DEBUG_M
	#define MLOG_HEADER(...) Log(__VA_ARGS__).PrintHeader()
	#define MLOG_FOOTER(...) Log(__VA_ARGS__).PrintFooter()

	#define MLOG_HEADER0(...) Log(this->GetClassName(), __func__, ##__VA_ARGS__).PrintHeader()
	#define MLOG_FOOTER0(...) Log(this->GetClassName(), __func__, ##__VA_ARGS__).PrintFooter()

	#define MLOG_NEWLINE(...) Log(__VA_ARGS__).Println()
	#define MLOG_NEWLINE0(...) Log(this->GetClassName(), __func__, ##__VA_ARGS__).Println()
	#define MLOG(...) Log(__VA_ARGS__).Print()
//	#define MLOG_TIME(...) Log(__VA_ARGS__).PrintTime()
 	#define MLOG_SHOW(TITLE, MEMORYNAME) MEMORYNAME->Show(""#TITLE"")
#else
	#define MLOG_HEADER(CLASSNAME, ...)
	#define MLOG_FOOTER(...)
	
	#define MLOG_HEADER0()
	#define MLOG_FOOTER0()

	#define MLOG_NEWLINE(CLASSNAME, ...)
	#define MLOG_NEWLINE0(...)
	#define MLOG(CLASSNAME, ...)
//	#define MLOG_TIME(...) Log(__VA_ARGS__).PrintTime()

	#define MLOG_SHOW(MEMORYNAME, ...) 
#endif

#if _DEBUG_MSTATIC
	#define NEW_STATIC(MEMORYNAME, ADDRESS, ...) Log("-SNEW: ", MEMORYNAME, (size_t)ADDRESS, ##__VA_ARGS__).Println()
	#define DELETE_STATIC(POBJECT, ...) Log("-SDELETE: ", (size_t)POBJECT, ##__VA_ARGS__).Println()
	#define SHOW_STATIC(MESSAGE) BaseObject::s_pMemory->Show(MESSAGE)
#else
	#define NEW_STATIC(MEMORYNAME, ADDRESS, ...)
	#define DELETE_STATIC(POBJECT, ...)
	#define SHOW_STATIC(MESSAGE)
#endif
#if _DEBUG_MDYNAMIC
	#define NEW_DYNAMIC(MEMORYNAME, ADDRESS, ...) Log("*DNEW: ", MEMORYNAME, (size_t)ADDRESS, ##__VA_ARGS__).Println();
//		this->m_mapLogs[(size_t)ADDRESS] = new("MemoryLog") MemoryLog(MEMORYNAME)

	#define DELETE_DYNAMIC(POBJECT, IDXPAGE, ...) Log("*DDELETE: ", (size_t)POBJECT, IDXPAGE, ##__VA_ARGS__).Println();
//		this->m_mapLogs[(size_t)POBJECT]->SetBAllocated(false)

	#define SHOW_DYNAMIC(MESSAGE) ValueObject::s_pMemory->Show(MESSAGE)
#else
	#define NEW_DYNAMIC(MEMORYNAME, ADDRESS, ...) 
	#define DELETE_DYNAMIC(POBJECT, IDXPAGE, ...)
	#define SHOW_DYNAMIC(MESSAGE)
#endif