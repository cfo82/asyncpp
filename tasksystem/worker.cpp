
#include "worker.h"
#include "executioncontext.h"

namespace tasks {

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

	TaskInstance* t = queue_->GetNext();
	if (!t)
	{
		//printf("%08x: no task\n", ::GetCurrentThreadId());
		::SleepEx(1, TRUE);
		return;
	}

	t->task->Run(t);

	ExecutionContext context = ExecutionContext(t->graph_instance);
	context.RunStep(t->next_token);

	SleepEx(0, TRUE); // handle completion routines
}

} // namespace tasks
