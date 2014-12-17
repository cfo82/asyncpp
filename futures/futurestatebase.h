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
