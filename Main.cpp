#include <92TestApplication/PTestMain/PTestMain.h>
#include <91TestPlatform/TestAspect/TestException.h>

int main() {
	try {
		PTestMain *pPTestMain = new("PTestMain") PTestMain();
		pPTestMain->BeforeInitialize();
		pPTestMain->InitializeMain();
		pPTestMain->BeforeRun();
		pPTestMain->RunMain();
		pPTestMain->AfterRun();
		pPTestMain->FinalizeMain();
		pPTestMain->AfterFinalize();
		delete pPTestMain;
	}
	catch (TestException& exception) {
		exception.Println();
	}
	return 0;
}