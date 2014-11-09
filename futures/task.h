
#ifndef FUTURES_TASK_H
#define FUTURES_TASK_H

#include <Windows.h>
#include <stdio.h>

namespace futures {

class Task
{
public:
	Task();
	virtual ~Task();

	virtual void Run() = 0;
};

} // namespace futures

#endif // FUTURES_TASK_H
