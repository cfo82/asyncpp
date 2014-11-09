
#ifndef WORKER_H
#define WORKER_H

#include "../concurrency/thread.h"
#include "taskqueue.h"

namespace tasks {

class Worker : public Thread
{
public:
	Worker(TaskQueue* queue);

	virtual void Loop();

private:
	TaskQueue* queue_;
};

} // namespace tasks

#endif // WORKER_H
