
#ifndef FUTURES_CONTINUATIONTASK_H
#define FUTURES_CONTINUATIONTASK_H

#include <Windows.h>
#include <stdio.h>

#include "task.h"

namespace futures {

class ContinuationTask : public Task
{
public:
	ContinuationTask();
	virtual ~ContinuationTask();

	virtual void Run() = 0;

	ContinuationTask* next_;
};

} // namespace futures

#endif // FUTURES_CONTINUATIONTASK_H
