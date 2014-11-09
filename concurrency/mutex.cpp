#include "Mutex.h"

Mutex::Mutex()
{
	::InitializeCriticalSection(&this->cs_);
}


Mutex::~Mutex()
{
	::DeleteCriticalSection(&this->cs_);
}

void Mutex::Lock() const
{
	::EnterCriticalSection(&this->cs_);
}

void Mutex::Unlock() const
{
	::LeaveCriticalSection(&this->cs_);
}