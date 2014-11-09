#pragma once

#include <Windows.h>

class Mutex
{
public:
	Mutex();
	~Mutex();

	//! locks the mutex
	void Lock() const;
	//! unlocks the mutex
	void Unlock() const;

private:
	/// windows critical section structure
	mutable CRITICAL_SECTION cs_;
};
