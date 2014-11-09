
#ifndef FUTURES_TASKQUEUE_H
#define FUTURES_TASKQUEUE_H

#include "task.h"
#include "../concurrency/mutex.h"
#include <vector>

namespace futures {

class TaskQueue
{
public:
	void Add(Task* task);

	Task* GetNext();

private:
	std::vector<Task*> tasks_;
	Mutex guard_;
};

} // namespace futures

#endif // FUTURES_TASKQUEUE_H
