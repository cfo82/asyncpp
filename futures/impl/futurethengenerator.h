
#ifndef FUTURES_IMPL_FUTURETHENGENERATOR_H
#define FUTURES_IMPL_FUTURETHENGENERATOR_H

#include "../isfuture.h"
#include "../../templatelibrary/evaluatelambdatypes.h"
#include "../../templatelibrary/issame.h"
#include "../../templatelibrary/if.h"

namespace futures {

template <typename SchedulerT, typename ReturnT>
class Future;

}

#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800 && _MSC_FULL_VER != 170051025) // 170051025 == ctp nov 2012

#else

namespace futures { namespace impl {

template <typename FutureT, typename ContinuationLambdaT>
struct FutureThenGenerator
{
	template <bool ReduceFutureReturnType>
	struct InternalFutureThenGenerator
	{
		typedef Future<typename FutureT::scheduler_t, typename EvaluateLambdaTypes<ContinuationLambdaT>::type_t> return_t;

		static return_t BuildThen(FutureT& future, ContinuationLambdaT&& lambda)
		{
			PackagedTask<typename FutureT::scheduler_t, ContinuationLambdaT, FutureT::return_t>* task = new PackagedTask<typename FutureT::scheduler_t, ContinuationLambdaT, FutureT::return_t>(
				future.GetScheduler(),
				move(lambda),
				future
				);

			future.state_->AddContinuation(task);

			return task->GetFuture();
		}
	};
	template <>
	struct InternalFutureThenGenerator<true>
	{
		typedef Future<typename FutureT::scheduler_t, typename EvaluateLambdaTypes<ContinuationLambdaT>::type_t> return_t;
		typedef typename FutureT::return_t future_return_t;

		static return_t BuildThen(FutureT& future, ContinuationLambdaT&& lambda)
		{
			auto continuation_lambda = [lambda](future_return_t v) {
				v.Then(move(lambda));
			};

			PackagedTask<typename FutureT::scheduler_t, decltype(continuation_lambda), future_return_t>* task = new PackagedTask<typename FutureT::scheduler_t, decltype(continuation_lambda), future_return_t>(
				future.GetScheduler(),
				move(continuation_lambda),
				future
				);

			future.state_->AddContinuation(task);

			return task->GetFuture();
		}
	};

	// these GetFutureReturnType structs are helpers to get the return type from a given type in case it is actually a future
	template <typename TypeT>
	struct GetFutureReturnType
	{
		template <bool IsFuture, typename ValueT> struct InternalGetFutureReturnType { typedef typename ValueT::return_t return_t; };
		template <typename ValueT> struct InternalGetFutureReturnType<false, ValueT> { typedef void return_t; };
		typedef typename InternalGetFutureReturnType<IsFuture<TypeT>::value, TypeT>::return_t return_t;
	};

	typedef typename EvaluateLambdaTypes<ContinuationLambdaT>::arguments_t continuation_arguments_t;
	typedef typename continuation_arguments_t::current_t first_argument_t;

	static const bool ReduceFutureReturnType = 
		IsFuture<typename FutureT::return_t>::value && 
		continuation_arguments_t::Size > 0 && 
		IsSame<typename GetFutureReturnType<FutureT::return_t>::return_t, first_argument_t>::yes
		;

	typedef InternalFutureThenGenerator<ReduceFutureReturnType> internal_t;

	typedef typename internal_t::return_t return_t;

	static return_t BuildThen(FutureT& future, ContinuationLambdaT&& lambda)
	{
		return internal_t::BuildThen(future, move(lambda));
	}
};

}} // namespace futures::impl

#endif

#endif // FUTURES_IMPL_FUTURETHENGENERATOR_H
