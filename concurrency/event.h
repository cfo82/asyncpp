#pragma once

#include <Windows.h>

class Event
{
public:
	Event();
	~Event();

	//! locks the mutex
	void Signal() const;
	//! unlocks the mutex
	void Wait() const;

private:
	/// windows critical section structure
	mutable HANDLE event_;
};
