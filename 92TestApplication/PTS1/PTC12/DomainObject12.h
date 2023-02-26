#pragma once

#include <01Base/Object/ValueObject.h>
#include <01Base/Aspect/Log.h>
#include <01Base/StdLib/Vector.h>
#include <03Technical/MemoryManager/SlotInfo.h>
class DomainObject12 : public ValueObject {
private:
	Vector<int>* m_pVector;  // 248 Byte

public:
	DomainObject12() 
	: ValueObject()
	, m_pVector(0)
	{
		this->m_pVector = new("DomainObject12::m_pVector") Vector<int>();
	}
	virtual ~DomainObject12() {
		delete this->m_pVector;
	}

	void Run() {
		for (int i = 0; i < m_pVector->Max_size(); i++) {
			m_pVector->Add(i);
		}
	}
};
