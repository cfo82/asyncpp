
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
