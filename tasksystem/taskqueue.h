
#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include "taskinstance.h"
#include "../concurrency/mutex.h"
#include <vector>

namespace tasks {

class TaskQueue
{
public:
	void Add(TaskInstance* task);

	TaskInstance* GetNext();

private:
	std::vector<TaskInstance*> tasks_;
	Mutex guard_;
};

} // namespace tasks

#endif // TASKQUEUE_H
