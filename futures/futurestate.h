/*
 * This file is part of the asyncpp Library release under the MIT license.
 *
 * Copyright (c) 2014 Christian Oberholzer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
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
