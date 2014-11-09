
#ifndef TASK_H
#define TASK_H

#include <Windows.h>
#include <stdio.h>

namespace tasks {

struct TaskInstance;

class Task
{
public:
	Task();
	virtual ~Task();

	virtual void Run(TaskInstance* instance) = 0;
};

} // namespace tasks

#endif // TASK_H
