#include "continuationtask.h"

namespace futures {

ContinuationTask::ContinuationTask()
:	Task(),
	next_(NULL)
{
}

ContinuationTask::~ContinuationTask()
{
}

} // namespace futures
