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
 
#include "executioncontext.h"

namespace tasks {

ExecutionContext::ExecutionContext(TaskGraphInstance* graph_instance_in)
:	graph_instance(graph_instance_in)
{
}

void ExecutionContext::RunStep(
	int current
)
{
	while (true)
	{
		TokenHeader* t = reinterpret_cast<TokenHeader*>(&graph_instance->graph->token_stream_[current]);

		switch (t->type)
		{
		case TokenHeader::Start: {
			//printf("> Executing Start Token\n");
			current += t->size;
			break;
		}
		case TokenHeader::QueueTask: {
			QueueTaskToken* queue_task_t = static_cast<QueueTaskToken*>(t);
			//printf("> Queueing Task %d\n", queue_task_t->index);
			graph_instance->task_queue->Add(&graph_instance->task_instances[queue_task_t->index]);
			graph_instance->task_instances[queue_task_t->index].next_token = current + t->size;
			return;
		}
		case TokenHeader::InitCounter: {
			InitCounterToken* init_counter_t = static_cast<InitCounterToken*>(t);
			//printf("> Initializing Counter %d: with value %d\n", init_counter_t->index, init_counter_t->value);
			graph_instance->counters[init_counter_t->index].SetValue(init_counter_t->value);
			current += t->size;
			break;
		}
		case TokenHeader::Branch: {
			BranchTokenHeader* branch_t = static_cast<BranchTokenHeader*>(t);
			//printf("> Branch token encountered: %d branches\n", branch_t->dimension);

			int* arr = reinterpret_cast<int*>(branch_t + 1);
			for (int i = 0; i < branch_t->dimension; ++i)
			{
				//printf("> Branching to %d\n", arr[i]);
				RunStep(arr[i]);
			}

			return;
		}
		case TokenHeader::Merge: {
			MergeToken* merge_t = static_cast<MergeToken*>(t);
			//printf("> Decrementing Merge Counter %d\n", merge_t->index);
			LONG new_value = graph_instance->counters[merge_t->index].Decrement();
			if (new_value > 0)
				{ return; }
			current += t->size;
			break;
		}
		case TokenHeader::Jump: {
			JumpToken* jump_t = static_cast<JumpToken*>(t);
			//printf("> Jump to %d\n", jump_t->jump_target);
			current = jump_t->jump_target;
			break;
		}
		case TokenHeader::WaitDependency: {
			WaitDependencyToken* wait_t = static_cast<WaitDependencyToken*>(t);
			Dependency* depencency = &graph_instance->dependencies[wait_t->dependency_index];
			Dependency::WaitContext* wait_context = &graph_instance->wait_contexts[wait_t->wait_context_index];
			wait_context->next_instruction = current + t->size;
			if (!depencency->Wait(wait_context))
				{ return; }

			current += t->size;
			break;
		}
		case TokenHeader::End: {
			//printf("> Encountered End token\n");
			graph_instance->done = true;
			return;
		}
		}
	}
}

} // namespace tasks
