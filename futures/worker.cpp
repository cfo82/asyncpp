
#include "worker.h"

namespace futures {

Worker::Worker(
	TaskQueue* queue
)
:	queue_(queue)
{
}

void Worker::Loop()
{
	if (terminate_requested_)
	{
		return;
	}

	Task* t = queue_->GetNext();
	if (!t)
	{
		//printf("%08x: no task\n", ::GetCurrentThreadId());
		::SleepEx(1, TRUE);
		return;
	}

	t->Run();

	SleepEx(0, TRUE); // handle completion routines
}

} // namespace futures
