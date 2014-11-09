
#ifndef FUTURES_JOIN_H
#define FUTURES_JOIN_H

#include "future.h"
#include "../templatelibrary/templatelist.h"
#include "../templatelibrary/templatelistdeclaration.h"
#include "../templatelibrary/templatelistcontainer.h"

#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800 && _MSC_FULL_VER != 170051025) // 170051025 == ctp nov 2012

#else

namespace futures {

namespace impl {

template <typename SchedulerT, typename ValueContainer>
class JoinState
{
public:
	InterlockedInteger counter;
	ValueContainer values;
	Future<SchedulerT, ValueContainer> future;
};
template <typename CurrentT, typename RestT>
struct InternalCreateReturnTypeList
{
	typedef TemplateList<
		typename CurrentT::return_t,
		typename InternalCreateReturnTypeList<typename RestT::current_t, typename RestT::rest_t>::type_t
	> type_t;
};
template <typename CurrentT>
struct InternalCreateReturnTypeList<CurrentT, EmptyTemplateList>
{
	typedef TemplateList<
		typename CurrentT::return_t,
		EmptyTemplateList
	> type_t;
};
template <typename... FuturesT>
struct CreateReturnTypeList
{
	typedef typename TemplateListDeclaration<FuturesT...>::type_t futures_list_t;
	typedef TemplateListContainer<
		typename InternalCreateReturnTypeList<typename futures_list_t::current_t, typename futures_list_t::rest_t>::type_t
	> type_t;
};

template <typename JoinStateT, int index, typename CurrentFutureT, typename... RestFuturesT>
struct CreateFutureContinuations
{
	static void Create(
		JoinStateT* join_state,
		CurrentFutureT future,
		RestFuturesT... rest
		)
	{
		future.Then([join_state](CurrentFutureT::return_t value) {
			//printf("X\n");
			join_state->values.SetData<index>(value);
			if (join_state->counter.Decrement() <= 0)
				{ join_state->future.Set(std::move(join_state->values)); }
		});

		CreateFutureContinuations<JoinStateT, index+1, RestFuturesT...>::Create(join_state, rest...);
	}
};
template <typename JoinStateT, int index, typename CurrentFutureT>
struct CreateFutureContinuations<JoinStateT, index, CurrentFutureT>
{
	static void Create(
		JoinStateT* join_state,
		CurrentFutureT future
		)
	{
		future.Then([join_state](CurrentFutureT::return_t value) {
			//printf("Y\n");
			join_state->values.SetData<index>(value);
			if (join_state->counter.Decrement() <= 0)
				{ join_state->future.Set(std::move(join_state->values)); }
		});
	}
};
template <typename CurrentFutureT, typename... RestFuturesT>
struct GetSchedulerTypeFromFuturePack
{
	typedef typename CurrentFutureT::scheduler_t scheduler_t;
	static scheduler_t* GetScheduler(CurrentFutureT future, RestFuturesT... rest)
	{
		return future.GetScheduler();
	}
};

} // namespace impl

template <typename... FuturesT>
Future<
	typename impl::GetSchedulerTypeFromFuturePack<FuturesT...>::scheduler_t,
	typename impl::CreateReturnTypeList<FuturesT...>::type_t
>
join(
	FuturesT... futures
)
{
	typedef typename impl::GetSchedulerTypeFromFuturePack<FuturesT...>::scheduler_t scheduler_t;
	typedef typename TemplateListDeclaration<FuturesT...>::type_t futures_list_t;
	typedef typename impl::CreateReturnTypeList<FuturesT...>::type_t return_parameter_list_t;
	typedef impl::JoinState<scheduler_t, return_parameter_list_t> join_state_t;

	auto result_future = Future<scheduler_t, return_parameter_list_t>(impl::GetSchedulerTypeFromFuturePack<FuturesT...>::GetScheduler(futures...));

	auto state = new join_state_t();
	state->counter.SetValue(futures_list_t::Size);
	state->future = result_future;

	impl::CreateFutureContinuations<join_state_t, 0, FuturesT...>::Create(state, futures...);

	return result_future;
}

} // namespace futures

#endif

#endif // FUTURES_JOIN_H
