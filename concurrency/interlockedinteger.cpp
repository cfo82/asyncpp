#include "InterlockedInteger.h"

LONG InterlockedInteger::Increment()
{
	return ::InterlockedIncrementAcquire(&value_);
}

LONG InterlockedInteger::Decrement()
{
	return ::InterlockedDecrementRelease(&value_);
}

LONG InterlockedInteger::Add(
	const LONG value
)
{
	return ::InterlockedExchangeAdd(&value_, value) + value;
}

LONG InterlockedInteger::Subtract(
	const LONG value
)
{
	return ::InterlockedExchangeAdd(&value_, - value) - value;
}

void InterlockedInteger::SetValue(
	const LONG value
)
{
	::InterlockedCompareExchange(&value_, value, value_);
}

LONG InterlockedInteger::GetValue() const
{
	return value_;
}
