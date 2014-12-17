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
 
#ifndef TASKGRAPH_H
#define TASKGRAPH_H

#include "task.h"

namespace tasks {

typedef unsigned char ubyte_t;

class TaskGraph
{
public:
	ubyte_t* token_stream_;
	Task** tasks;
	int num_tasks;
	int num_counters;
	int num_dependencies;
	int num_wait_contexts;

	TaskGraph(ubyte_t* token_stream, int num_tasks_in, Task** tasks_in, int num_counters_in, int num_dependencies_in, int num_wait_contexts_in)
	:	token_stream_(token_stream),
		num_tasks(num_tasks_in),
		tasks(tasks_in),
		num_counters(num_counters_in),
		num_dependencies(num_dependencies_in),
		num_wait_contexts(num_wait_contexts_in)
	{
	}
};

} // namespace tasks

#endif // TASKGRAPH_H
