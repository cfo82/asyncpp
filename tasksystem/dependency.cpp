
#include "dependency.h"
#include "executioncontext.h"

namespace tasks {

Dependency::Dependency()
:	satisfied_(1),
	wait_context_list_(NULL),
	guard_()
{
}

// note wait_context->next_instruction *must* be initialized correctly when passing
// the context to this methode!
bool Dependency::Wait(
	WaitContext* wait_context
)
{
	bool return_value = true;

	guard_.Lock();

	if (satisfied_ > 0)
	{
		// initialize and add it to to this dependency
		wait_context->next = wait_context_list_;
		wait_context_list_ = wait_context;
		return_value = false;
	}

	guard_.Unlock();

	return return_value;
}

void Dependency::Satisfy(
	TaskGraphInstance* instance
)
{
	ExecutionContext execution_context = ExecutionContext(instance);

	guard_.Lock();

	--satisfied_;

	if (satisfied_ <= 0)
	{
		while (wait_context_list_)
		{
			execution_context.RunStep(wait_context_list_->next_instruction);
			wait_context_list_ = wait_context_list_->next;
		}
	}

	guard_.Unlock();
}

} // namespace tasks
