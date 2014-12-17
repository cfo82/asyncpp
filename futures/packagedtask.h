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
 
#ifndef FUTURES_PACKAGEDTASK_H
#define FUTURES_PACKAGEDTASK_H

#include "future.h"

#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800 && _MSC_FULL_VER != 170051025) // 170051025 == ctp nov 2012

#else

namespace futures {

template <typename SchedulerT, typename LambdaT, typename ReturnT, typename ArgumentT>
struct PackagedTaskState
{
	typedef Future<SchedulerT, ReturnT> future_t;

	PackagedTaskState(SchedulerT* scheduler, LambdaT&& lambda, Future<SchedulerT, ArgumentT> predecessor)
	:	lambda_(lambda),
		predecessor_(predecessor),
		future_(scheduler)
	{
	}

	virtual void Run()
	{
		future_.Set(lambda_(predecessor_.Get()));
	}

	future_t GetFuture()
	{
		return future_;
	}

	LambdaT lambda_;
	Future<SchedulerT, ArgumentT> predecessor_;
	future_t future_;
};
template <typename SchedulerT, typename LambdaT, typename ArgumentT>
struct PackagedTaskState<SchedulerT, LambdaT, void, ArgumentT>
{
	typedef Future<SchedulerT, void> future_t;

	PackagedTaskState(SchedulerT* scheduler, LambdaT&& lambda, Future<SchedulerT, ArgumentT> predecessor)
	:	lambda_(lambda),
		predecessor_(predecessor),
		future_(scheduler)
	{
	}

	virtual void Run()
	{
		lambda_(predecessor_.Get());
		future_.SetDone();
	}

	future_t GetFuture()
	{
		return future_;
	}

	LambdaT lambda_;
	Future<SchedulerT, ArgumentT> predecessor_;
	future_t future_;
};
template <typename SchedulerT, typename LambdaT, typename ReturnT>
struct PackagedTaskState<SchedulerT, LambdaT, ReturnT, void>
{
	typedef Future<SchedulerT, ReturnT> future_t;

	PackagedTaskState(SchedulerT* scheduler, LambdaT&& lambda, Future<SchedulerT, void> predecessor)
	:	lambda_(lambda),
		future_(scheduler)
	{
	}

	virtual void Run()
	{
		future_.Set(lambda_());
	}

	future_t GetFuture()
	{
		return future_;
	}

	LambdaT lambda_;
	future_t future_;
};
template <typename SchedulerT, typename LambdaT>
struct PackagedTaskState<SchedulerT, LambdaT, void, void>
{
	typedef Future<SchedulerT, void> future_t;

	PackagedTaskState(SchedulerT* scheduler, LambdaT&& lambda, Future<SchedulerT, void> predecessor)
	:	lambda_(lambda),
		future_(scheduler)
	{
	}

	virtual void Run()
	{
		lambda_();
		future_.SetDone();
	}

	future_t GetFuture()
	{
		return future_;
	}

	LambdaT lambda_;
	future_t future_;
};

template <typename SchedulerT, typename LambdaT, typename ArgumentT = void>
struct PackagedTask : public ContinuationTask
{
	typedef typename EvaluateLambdaTypes<LambdaT>::type_t return_type_t;

	PackagedTask(SchedulerT* scheduler, LambdaT&& lambda, Future<SchedulerT, ArgumentT> predecessor = Future<SchedulerT, ArgumentT>(nullptr))
	:	state_(scheduler, move(lambda), predecessor)
	{
	}

	virtual void Run()
	{
		state_.Run();
	}

	typename PackagedTaskState<SchedulerT, LambdaT, return_type_t, ArgumentT>::future_t GetFuture()
	{
		return state_.GetFuture();
	}

	PackagedTaskState<SchedulerT, LambdaT, return_type_t, ArgumentT> state_;
};

} // namespace futures

#endif

#endif // FUTURES_PACKAGEDTASK_H
