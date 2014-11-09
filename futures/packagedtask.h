
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
