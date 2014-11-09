
#ifndef EXECUTIONCONTEXT_H
#define EXECUTIONCONTEXT_H

#include "tokens.h"
#include "taskgraphinstance.h"

namespace tasks {

class ExecutionContext
{
public:
	ExecutionContext(TaskGraphInstance* graph_instance_in);
	void RunStep(int current);

private:
	TaskGraphInstance* graph_instance;
};

} // namespace tasks

#endif // EXECUTIONCONTEXT_H
