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
