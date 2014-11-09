
#ifndef FUTURES_WORKER_H
#define FUTURES_WORKER_H

#include "../concurrency/thread.h"
#include "taskqueue.h"

namespace futures {

class Worker : public Thread
{
public:
	Worker(TaskQueue* queue);

	virtual void Loop();

private:
	TaskQueue* queue_;
};

} // namespace futures

#endif // FUTURES_WORKER_H
