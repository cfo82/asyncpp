
#include "taskqueue.h"

namespace futures {

void TaskQueue::Add(
	Task* task
)
{
	guard_.Lock();

	//printf("%08x: adding %p\n", ::GetCurrentThreadId(), task);

	tasks_.push_back(task);

	guard_.Unlock();
}

Task* TaskQueue::GetNext()
{
	guard_.Lock();

	if (tasks_.empty())
	{
		guard_.Unlock();

		//printf("%08x: returning %p\n", ::GetCurrentThreadId(), NULL);

		return NULL;
	}

	Task* t = tasks_.back();
	tasks_.pop_back();

	//printf("%08x: returning %p\n", ::GetCurrentThreadId(), t);

	guard_.Unlock();

	return t;
}

} // namespace futures
