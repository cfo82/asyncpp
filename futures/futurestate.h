
#ifndef FUTURES_FUTURESTATE_H
#define FUTURES_FUTURESTATE_H

#include <Windows.h>
#include <stdio.h>

#include "futurestatebase.h"
#include "taskqueue.h"

#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800 && _MSC_FULL_VER != 170051025) // 170051025 == ctp nov 2012

#else

namespace futures {

template <typename SchedulerT, typename ValueT>
class FutureState : private FutureStateBase<SchedulerT, FutureState<SchedulerT, ValueT> >
{
public:
	FutureState(SchedulerT* scheduler)
	:	FutureStateBase(scheduler),
		value_()
	{
	}

	void AddRef()
	{
		FutureStateBase::AddRef();
	}

	void RemRef()
	{
		FutureStateBase::RemRef();
	}

	void SetValue(ValueT&& value)
	{
		continuation_guard_.Lock();

		value_ = value;
		done_ = true;
		event_.Signal();

		ContinuationTask* t = continuation_;
		while (t)
		{
			scheduler_->AddTask(continuation_);
			t = t->next_;
		}

		continuation_guard_.Unlock();
	}

	const ValueT& GetValue()
	{
		event_.Wait();
		return value_;
	}

	void Await()
	{
		event_.Wait();
	}

	const bool IsDone() const
	{
		return FutureStateBase::IsDone();
	}

	SchedulerT* GetScheduler()
	{
		return FutureStateBase::GetScheduler();
	}

	void AddContinuation(ContinuationTask* task)
	{
		FutureStateBase::AddContinuation(task);
	}

private:
	ValueT value_;
};
template <typename SchedulerT>
class FutureState<SchedulerT, void> : private FutureStateBase<SchedulerT, FutureState<SchedulerT, void> >
{
public:
	FutureState(SchedulerT* scheduler)
	:	FutureStateBase(scheduler)
	{
	}

	void AddRef()
	{
		FutureStateBase::AddRef();
	}

	void RemRef()
	{
		FutureStateBase::RemRef();
	}

	void SetDone()
	{
		continuation_guard_.Lock();

		done_ = true;
		event_.Signal();

		ContinuationTask* t = continuation_;
		while (t)
		{
			scheduler_->AddTask(continuation_);
			t = t->next_;
		}

		continuation_guard_.Unlock();
	}

	void Await()
	{
		event_.Wait();
	}

	const bool IsDone() const
	{
		return FutureStateBase::IsDone();
	}

	SchedulerT* GetScheduler()
	{
		return FutureStateBase::GetScheduler();
	}

	void AddContinuation(ContinuationTask* task)
	{
		FutureStateBase::AddContinuation(task);
	}
};

} // namespace futures

#endif

#endif // FUTURES_FUTURESTATE_H
