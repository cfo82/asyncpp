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
 
#include "taskqueue.h"

namespace futures {

void TaskQueue::Add(
	Task* task
)
{
	guard_.Lock();

	//printf("%08x: adding %p\n", ::GetCurrentThreadId(), task);

	tasks_.push_back(task);

	guard_.Unlock();
}

Task* TaskQueue::GetNext()
{
	guard_.Lock();

	if (tasks_.empty())
	{
		guard_.Unlock();

		//printf("%08x: returning %p\n", ::GetCurrentThreadId(), NULL);

		return NULL;
	}

	Task* t = tasks_.back();
	tasks_.pop_back();

	//printf("%08x: returning %p\n", ::GetCurrentThreadId(), t);

	guard_.Unlock();

	return t;
}

} // namespace futures
