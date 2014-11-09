#pragma once

#include <Windows.h>
#include <process.h>

enum ThreadStatus
{
	ThreadStatusDeleted,
	ThreadStatusSuspended,
	ThreadStatusRunning
};

class Thread
{
public:
	friend unsigned int thread_create_callback(Thread* threadptr);

protected:
	Thread();

public:
	virtual ~Thread();

	//! starts the thread
	bool Create(bool suspended = false);
	//! terminates the thread
	bool Terminate();
	//! suspends the threads execution
	bool Suspend();
	//! resumes the thread from suspended state
	bool Resume();
	//! yield for a certain amount of time
	static void Sleep(unsigned int microseconds);

	/// sets the threads return code
	void SetReturnCode(const unsigned int return_code);
	/// gets the thread return code once the thread has been terminated
	const unsigned int GetReturnCode();

protected:
	//! when the thread is started it first calls this function once
	virtual void Startup();
	//! called once to do the threads work
	virtual void Run();
	//! perform the threads work in a loop. TODO: move to another thread class "LoopThread" or something like that
	virtual void Loop();
	//! thread shutdown method. called once afther the thread run methode has returned
	virtual unsigned int Shutdown();
	//! wakeup in order to terminate execution
	virtual void Wakeup();

protected:
	HANDLE thread_handle_;
	//! the threads return code
	unsigned int return_code_;
	//! flag indicating if the thread has to be terminated
	long terminate_requested_;
	//! the threads current status
	ThreadStatus status_;
};
