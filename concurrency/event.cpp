#include "Event.h"

Event::Event()
{
	event_ = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}


Event::~Event()
{
	::CloseHandle(event_);
}

void Event::Signal() const
{
	::SetEvent(event_);
}

void Event::Wait() const
{
	::WaitForSingleObjectEx(event_, INFINITE, TRUE);
}