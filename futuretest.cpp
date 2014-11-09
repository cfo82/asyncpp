#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <typeinfo>

#pragma warning(disable:4503)

#include "concurrency/mutex.h"
#include "concurrency/event.h"
#include "concurrency/thread.h"
#include "concurrency/interlockedinteger.h"

#include "futures/task.h"
#include "futures/taskqueue.h"
#include "futures/worker.h"
#include "futures/future.h"
#include "futures/async.h"
#include "futures/move.h"
#include "futures/join.h"

#include "templatelibrary/templatelistcontainer.h"

using namespace futures;

class MyScheduler
{
public:
	MyScheduler()
	:	task_queue(),
		w1_(NULL),
		w2_(NULL)
	{
		w1_ = new Worker(&task_queue);
		w2_ = new Worker(&task_queue);

		w1_->Create();
		w2_->Create();
	}

	~MyScheduler()
	{
		w1_->Terminate();
		w2_->Terminate();

		delete w1_;
		delete w2_;
	}

	void AddTask(Task* task)
	{
		task_queue.Add(task);
	}

private:
	TaskQueue task_queue;
	Worker* w1_;
	Worker* w2_;
};

struct MYOVERLAPPED2 : public OVERLAPPED
{
	Future<MyScheduler, DWORD> future;
};

VOID CALLBACK completion_routine_2(
	_In_ DWORD dwErrorCode,
	_In_ DWORD dwNumberOfBytesTransfered,
	_Inout_ LPOVERLAPPED lpOverlapped
)
{
	MYOVERLAPPED2* overlapped = reinterpret_cast<MYOVERLAPPED2*>(lpOverlapped);
	overlapped->future.Set(move(dwNumberOfBytesTransfered));

	printf("completion_routine\n");
}

Future<MyScheduler, DWORD> WriteFileAsync(MyScheduler* scheduler, HANDLE file_handle, const char* buffer, const DWORD size)
{
	Future<MyScheduler, DWORD> f = Future<MyScheduler, DWORD>(scheduler);

	MYOVERLAPPED2* overlapped = new MYOVERLAPPED2;
	memset(overlapped, 0, sizeof(MYOVERLAPPED2));
	overlapped->future = f;

	BOOL result = ::WriteFileEx(file_handle, buffer, size, overlapped, completion_routine_2);

	return f;
}

#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800 && _MSC_FULL_VER != 170051025) // 170051025 == ctp nov 2012

void RunSample()
{
}

#else

void RunSample()
{
	MyScheduler scheduler;
	std::wstring path = L"helloworld.txt";

	const char* hello_world = "Hello World!";

	auto f = async(&scheduler, [path]() -> HANDLE {
		return ::CreateFileW(path.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
	}).Then([&scheduler, hello_world](HANDLE file_handle) -> Future<MyScheduler, HANDLE> {
		auto inner_f = WriteFileAsync(&scheduler, file_handle, hello_world, strlen(hello_world) + 1);
		auto handle_f = inner_f.Then([file_handle](DWORD bytes_written) -> HANDLE { return file_handle; });
		return handle_f;
	}).Then([](HANDLE file_handle) {
		::CloseHandle(file_handle);
		printf("close handle\n");
	});

	f.Await();

	Future<MyScheduler, int> f1 = async(&scheduler, [] { printf("A\n"); return 1; });
	Future<MyScheduler, float> f2 = async(&scheduler, [] { printf("B\n"); return 3.5f; });
	Future<MyScheduler, TemplateListContainer<TemplateListDeclaration<int, float>::type_t>> f3 = join(f1, f2);

	/*auto f4 = f3.Then([](int a, float b) {
		printf("%d, %f\n", a, b);
	});*/

	f3.Await();
}

#endif

int main(const int argc, const char* argv[])
{
	RunSample();

	return EXIT_SUCCESS;
}
