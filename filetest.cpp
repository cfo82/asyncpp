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
 
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <typeinfo>

#pragma warning(disable:4503)

typedef unsigned char ubyte_t;

#include "tasksystem/task.h"
#include "tasksystem/taskinstance.h"
#include "tasksystem/taskgraph.h"
#include "tasksystem/taskgraphinstance.h"
#include "tasksystem/executioncontext.h"
#include "tasksystem/rungraphtask.h"
#include "tasksystem/taskqueue.h"
#include "tasksystem/worker.h"
#include "tasksystem/tokenstreamgenerator.h"
#include "tasksystem/graphnode.h"
#include "tasksystem/statictaskgraph.h"
#include "templatelibrary/inttemplatelistdeclaration.h"

using namespace tasks;

HANDLE event_handle;
static const int WriteCompletedDependency = 0;

struct MYOVERLAPPED : public OVERLAPPED
{
	TaskInstance* task_instance;
	TaskQueue* task_queue;
};

VOID CALLBACK completion_routine(
  _In_     DWORD dwErrorCode,
  _In_     DWORD dwNumberOfBytesTransfered,
  _Inout_  LPOVERLAPPED lpOverlapped
)
{
	TaskInstance* task_instance = reinterpret_cast<MYOVERLAPPED*>(lpOverlapped)->task_instance;
	printf("overlapped parameter: %p, %p\n", lpOverlapped, task_instance);
	task_instance->graph_instance->SatisfyDependency(WriteCompletedDependency);
}

MYOVERLAPPED overlapped;

class File
{
public:
	File() : file_handle(0)
	{
	}

	void Open(const wchar_t* path)
	{
		file_handle = ::CreateFileW(L"helloworld.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
	}

	void Write(TaskInstance* task_instance, const char* data, const DWORD length)
	{
		DWORD num_bytes_written = 0;
		//MYOVERLAPPED overlapped;
		memset(&overlapped, 0, sizeof(overlapped));
		overlapped.task_instance = task_instance;
		printf("create overlapped on stack: %p, %p\n", &overlapped, overlapped.task_instance);
		::WriteFileEx(file_handle, data, length, &overlapped, completion_routine);
	}

	void Close()
	{
		::CloseHandle(file_handle);
		file_handle = NULL;
	}

	HANDLE file_handle;
};

struct OpenFileTask
{
	typedef TemplateListContainer<TemplateListDeclaration<File>::type_t> context_t;

	static void Run(TaskInstance* instance, context_t& context)
	{
		printf("%08x: open file\n", ::GetCurrentThreadId());
		context.GetData<0>().Open(L"helloworld.txt");
	}
};

class WriteHelloWorldTask
{
public:
	typedef TemplateListContainer<TemplateListDeclaration<File>::type_t> context_t;

	static void Run(TaskInstance* instance, context_t& context)
	{
		printf("%08x: writing\n", ::GetCurrentThreadId());
		const char* hello_world = "Hello World!";
		context.GetData<0>().Write(instance, hello_world, strlen(hello_world) + 1);
	}
};

class CloseFileTask
{
public:
	typedef TemplateListContainer<TemplateListDeclaration<File>::type_t> context_t;

	static void Run(TaskInstance* instance, context_t& context)
	{
		::SleepEx(2, TRUE);
		printf("%08x: close file\n", ::GetCurrentThreadId());
		context.GetData<0>().Close();
		::SetEvent(event_handle);
	}
};


typedef GraphNode<OpenFileTask, TemplateListDeclaration<WriteHelloWorldTask>::type_t> node_0_t;
typedef GraphNode<WriteHelloWorldTask, TemplateListDeclaration<CloseFileTask>::type_t> node_1_t;
typedef GraphNode<CloseFileTask, EmptyTemplateList, IntTemplateListDeclaration<WriteCompletedDependency>::type_t> node_2_t;
typedef TemplateListDeclaration<node_0_t, node_1_t, node_2_t>::type_t GraphNodes;

typedef TemplateListContainer<TemplateListDeclaration<File>::type_t> context_data_t;

typedef StaticTaskGraph<GraphNodes, context_data_t> graph_t;

TaskQueue file_queue;
TaskGraphInstanceTemplate<context_data_t>* file_graph_instance = graph_t::CreateInstance(&file_queue);
RunGraphTask file_initial_task;

/*main(const int argc, const char* argv[])
{
	event_handle = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	Worker w1 = Worker(&file_queue);
	Worker w2 = Worker(&file_queue);

	TaskInstance initial_instance;
	initial_instance.graph_instance = file_graph_instance;
	initial_instance.task = &file_initial_task;
	initial_instance.next_token = 0;

	w1.Create();
	w2.Create();

	file_queue.Add(&initial_instance);

	::WaitForSingleObjectEx(event_handle, INFINITE, TRUE);
	::CloseHandle(event_handle);

	w1.Terminate();
	w2.Terminate();

	return EXIT_SUCCESS;
}*/
