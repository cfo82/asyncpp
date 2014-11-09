
#ifndef FUTURES_FUTURESTATEBASE_H
#define FUTURES_FUTURESTATEBASE_H

#include "continuationtask.h"
#include "../concurrency/interlockedinteger.h"
#include "../concurrency/event.h"
#include "../concurrency/mutex.h"

namespace futures {

template <
	typename SchedulerT,
	typename DerivedT
>
class FutureStateBase
{
public:
	FutureStateBase(SchedulerT* scheduler)
	:	scheduler_(scheduler),
		refcount_(1),
		event_(),
		done_(false),
		continuation_guard_(),
		continuation_(nullptr)
	{
	}

	~FutureStateBase()
	{
	}

	void AddRef()
	{
		refcount_.Increment();
	}

	void RemRef()
	{
		if (refcount_.Decrement() <= 0)
		{
			delete static_cast<DerivedT*>(this);
		}
	}

	const bool IsDone() const
	{
		return done_;
	}

	void AddContinuation(ContinuationTask* task)
	{

		continuation_guard_.Lock();

		if (IsDone())
		{
			scheduler_->AddTask(task);
		}
		else
		{
			if (continuation_)
			{
				task->next_ = continuation_;
			}

			continuation_ = task;
		}

		continuation_guard_.Unlock();
	}

	SchedulerT* GetScheduler()
	{
		return scheduler_;
	}

protected:
	SchedulerT* scheduler_;
	InterlockedInteger refcount_;
	Event event_;
	bool done_;
	Mutex continuation_guard_;
	ContinuationTask* continuation_;
};

} // namespace futures

#endif // FUTURES_FUTURESTATEBASE_H
