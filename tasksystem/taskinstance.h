
#ifndef TASKINSTANCE_H
#define TASKINSTANCE_H

#include "task.h"

namespace tasks {

class TaskGraphInstance;

struct TaskInstance
{
	int next_token;
	Task* task;
	TaskGraphInstance* graph_instance;
};

} // namespace tasks

#endif // TASKINSTANCE_H
