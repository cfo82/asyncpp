
#ifndef DEPENDENCY_H
#define DEPENDENCY_H

#include "../concurrency/mutex.h"

namespace tasks {

class TaskGraphInstance;

class Dependency
{
public:
	struct WaitContext
	{
		WaitContext* next;
		int next_instruction;
	};

public:
	Dependency();

	bool Wait(WaitContext* wait_context);
	void Satisfy(TaskGraphInstance* instance);

private:
	int satisfied_;
	WaitContext* wait_context_list_;
	Mutex guard_;
};

} // namespace tasks

#endif // DEPENDENCY_H
