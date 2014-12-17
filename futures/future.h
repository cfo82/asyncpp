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
 
#ifndef FUTURES_FUTURE_H
#define FUTURES_FUTURE_H

#include "futurestate.h"
#include "impl/futurethengenerator.h"

#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800 && _MSC_FULL_VER != 170051025) // 170051025 == ctp nov 2012

namespace futures {

template <typename SchedulerT, typename ReturnT>
class Future
{
public:
	Future(SchedulerT* scheduler = NULL)
	//:	state_(new FutureState<SchedulerT, ReturnT>(scheduler))
	{
	}

	void Set(const ReturnT& v)
	{
		//state_->SetValue(move(v));
	}
};

} // namespace futures

#else

namespace futures {

template <typename SchedulerT, typename ReturnT>
class Future
{
public:
	typedef SchedulerT scheduler_t;
	typedef ReturnT return_t;

	Future(SchedulerT* scheduler = nullptr)
	:	state_(new FutureState<SchedulerT, ReturnT>(scheduler))
	{
	}

	Future(Future<SchedulerT, ReturnT>&& rhs)
	:	state_(nullptr)
	{
		state_ = rhs.state_;
		rhs.state_ = nullptr;
	}

	Future(const Future<SchedulerT, ReturnT>& rhs)
	:	state_(nullptr)
	{
		rhs.state_->AddRef();
		state_ = rhs.state_;
	}

	void Set(ReturnT&& v)
	{
		state_->SetValue(move(v));
	}

	const ReturnT& Get()
	{
		return state_->GetValue();
	}

	void Await()
	{
		state_->Await();
	}

	SchedulerT* GetScheduler()
	{
		return state_->GetScheduler();
	}

	template <typename OtherLambdaT>
	Future<SchedulerT, typename EvaluateLambdaTypes<OtherLambdaT>::type_t> Then(OtherLambdaT&& lambda)
	{
		return impl::FutureThenGenerator<Future<SchedulerT, ReturnT>, OtherLambdaT>::BuildThen(*this, move(lambda));
	}

public:
	FutureState<SchedulerT, ReturnT>* state_;
};

template <typename SchedulerT>
class Future<SchedulerT, void>
{
public:
	typedef SchedulerT scheduler_t;
	typedef void return_t;

	Future(SchedulerT* scheduler = nullptr)
	:	state_(new FutureState<SchedulerT, void>(scheduler))
	{
	}

	Future(Future<SchedulerT, void>&& rhs)
	:	state_(nullptr)
	{
		state_ = rhs.state_;
		rhs.state_ = nullptr;
	}

	Future(const Future<SchedulerT, void>& rhs)
	:	state_(nullptr)
	{
		rhs.state_->AddRef();
		state_ = rhs.state_;
	}

	void SetDone()
	{
		state_->SetDone();
	}

	const void Await()
	{
		state_->Await();
	}

	SchedulerT* GetScheduler()
	{
		return state_->GetScheduler();
	}

	template <typename OtherLambdaT>
	Future<SchedulerT, typename EvaluateLambdaTypes<OtherLambdaT>::type_t> Then(OtherLambdaT&& lambda)
	{
		return impl::FutureThenGenerator<Future<SchedulerT, ReturnT>, OtherLambdaT>::BuildThen(*this, move(lambda));
	}

private:
	FutureState<SchedulerT, void>* state_;
};

} // namespace futures

#endif

#endif // FUTURES_FUTURESTATE_H
