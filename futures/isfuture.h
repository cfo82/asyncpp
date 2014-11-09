
#ifndef FUTURES_ISFUTURE_H
#define FUTURES_ISFUTURE_H

#include "../templatelibrary/removereference.h"

namespace futures {

template <typename SchedulerT, typename ReturnT> class Future;

template <typename TypeT>
struct IsFuture
{
	static const bool value = false;
};
template <typename SchedulerT, typename ReturnT>
struct IsFuture<Future<SchedulerT, ReturnT> >
{
	static const bool value = true;
};

} // namespace futures

#endif // FUTURES_ISFUTURE_H
