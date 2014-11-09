
#ifndef FUTURES_ASYNC_H
#define FUTURES_ASYNC_H

#include "future.h"
#include "packagedtask.h"

#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800 && _MSC_FULL_VER != 170051025) // 170051025 == ctp nov 2012

#else

namespace futures {

template <typename SchedulerT, typename LambdaT>
Future<SchedulerT, typename EvaluateLambdaTypes<LambdaT>::type_t> async(SchedulerT* scheduler, LambdaT&& lambda)
{
	PackagedTask<SchedulerT, LambdaT>* task = new PackagedTask<SchedulerT, LambdaT>(scheduler, move(lambda));

	scheduler->AddTask(task);

	return task->GetFuture();
}

} // namespace futures

#endif

#endif // FUTURES_ISFUTURE_H
