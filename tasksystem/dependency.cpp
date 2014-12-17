/*
 * This file is part of the asyncpp Library release under the MIT license.
 *
 * Copyright (c) 2014 Christian Oberholzer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
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
