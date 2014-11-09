
#include "taskqueue.h"

namespace tasks {

void TaskQueue::Add(
	TaskInstance* task
)
{
	guard_.Lock();

	//printf("%08x: adding %p\n", ::GetCurrentThreadId(), task);

	tasks_.push_back(task);

	guard_.Unlock();
}

TaskInstance* TaskQueue::GetNext()
{
	guard_.Lock();

	if (tasks_.empty())
	{
		guard_.Unlock();

		//printf("%08x: returning %p\n", ::GetCurrentThreadId(), nullptr);

		return NULL;
	}

	TaskInstance* t = tasks_.back();
	tasks_.pop_back();

	//printf("%08x: returning %p\n", ::GetCurrentThreadId(), t);

	guard_.Unlock();

	return t;
}

} // namespace tasks
