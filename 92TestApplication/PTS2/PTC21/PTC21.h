#pragma once

#include <92TestApplication/PTS2/typedef.h>
#define _PTC21_Id _GET_TESTCLASS_UID(_ETestComponent_PS2::_ePTC21)
#define _PTC21_Name "PTC21"

#include <91TestPlatform/TestCase/TestCase.h>

class PTC21 : public TestCase {
public:
	PTC21(
		unsigned nClassId = _PTC21_Id, 
		const char* pcClassName = _PTC21_Name);
	~PTC21() override;

	void Initialize() override;
	void Finalize() override;
	void Run() override;
};

