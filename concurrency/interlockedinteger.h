#pragma once

#include <windows.h>

class InterlockedInteger
{
public:
	InterlockedInteger(const LONG value = 0) : value_(value) { }
	~InterlockedInteger() { }

	LONG Increment();
	LONG Decrement();
	LONG Add(const LONG value);
	LONG Subtract(const LONG value);
	void SetValue(const LONG value);
	LONG GetValue() const;

private:
	LONG value_;
};

