
#ifndef RUNGRAPHTASK_H
#define RUNGRAPHTASK_H

#include "task.h"

namespace tasks {

class RunGraphTask : public Task
{
public:
	RunGraphTask();

	virtual void Run(TaskInstance* instance);
};

} // namespace tasks

#endif // RUNGRAPHTASK_H
