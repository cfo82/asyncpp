#include "Thread.h"
#include <cassert>
#define TRUE 1

//-----------------------------------------------------------------------------------------------------------
unsigned __stdcall windows_thread_create_callback(
	void* arguments
)
{
	return thread_create_callback(static_cast<Thread*>(arguments));
}




//-----------------------------------------------------------------------------------------------------------
/*!	
	\brief Thread entry point
	This function is called after the thread has been started.
*/
unsigned int thread_create_callback(
	Thread* threadptr
)
{
	threadptr->Startup();
	threadptr->Run();
	unsigned int return_code = threadptr->Shutdown();
	threadptr->SetReturnCode(return_code);

	// windows calls the matching exit function after this creation routine returns. so just
	// return the actual return code

	return return_code;
}




//-----------------------------------------------------------------------------------------------------------
Thread::Thread()
:	return_code_(TRUE),
	terminate_requested_(0),
	status_(ThreadStatusDeleted)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!	
	\attention a thread must not destroy itself. destruction must always happen from another codepath
	If the thread has not yet terminated the destructor will termiante it
*/
Thread::~Thread()
{
	if (status_ == ThreadStatusRunning ||
		status_ == ThreadStatusSuspended)
	{
		Terminate();
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!	
	\param stack_size
		defines the size of the stack to be used to execute the thread on. If it is 0
		it will use a default value.
	\param suspended
		if this flag is set the thread will be created in suspended state and must be
		resumed before it runs.
	\return 
		success flag

	The function creates a new thread and executes it with the given paramters.
*/
bool Thread::Create(
	bool suspended /* = false */
)
{
	terminate_requested_ = 0;
	status_ = suspended ? ThreadStatusSuspended : ThreadStatusRunning;

	// ThreadBase starten
	// for a discussion of the advantages/disadvantages of using CreateThread versus _beginthreadex
	// read this link: http://www.codeguru.com/forum/showthread.php?t=371305
	// basically the difference seems to be the c runtime which initializes some thread local storage
	// for errno, etc. which seems to be done in a safer way by using _beginthreadex. _beginthreadex
	// is only available in the multithreaded c runtime library. so we have to link against those.
	unsigned thread_addr = 0;
	thread_handle_ = (HANDLE)_beginthreadex(
		NULL,
		0,
		windows_thread_create_callback,
		this,
		suspended ? CREATE_SUSPENDED : 0,
		&thread_addr
		);
	if (!thread_handle_)
	{
		return false;
	}

	::SetThreadPriorityBoost(thread_handle_, TRUE); // FALSE == Enable Priority Boost

	return true;
}




//-----------------------------------------------------------------------------------------------------------
/*!	
	\brief 
		Thread von aussen beenden
	\return 
		Status

	Wenn der Thread von ausser zerstoert werden soll, so muss diese Methode aufgerufen werden.
	Der Thread wird dann aufgefordert seine Aufgaben zu beenden und zurueckzukuehren.
*/
bool Thread::Terminate()
{
	if (status_ == ThreadStatusSuspended)
	{
		Resume();
	}

	if (status_ == ThreadStatusRunning)
	{
		// Set Terminate Requested Flag and Call Wakeup for eventually sleeping threads...
		long last_value = InterlockedCompareExchange(&terminate_requested_, 1, 0);
		assert(last_value == 0);

		Wakeup();

		// Then Wait until Thread has been Terminated
		// Then Wait until ThreadBase has been Terminated
		::WaitForSingleObject(thread_handle_, INFINITE);
		::CloseHandle(thread_handle_);
		thread_handle_ = NULL;

		status_ = ThreadStatusDeleted;
	}

	return true;
}




//-----------------------------------------------------------------------------------------------------------
/*!	
	\brief Ausfuehrung des Threads stoppen
	
	Unterbricht den Thread.
*/
bool Thread::Suspend()
{
	assert(status_ == ThreadStatusRunning);
	if (::SuspendThread(thread_handle_) == ((DWORD)-1))
		{ return false; }
	status_ = ThreadStatusSuspended;
	return true;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Thread weiter ausfuehren
	
	Wenn der Thread gestoppt wurde kann er mit dieser Methode wieder gestartet werden
*/
bool Thread::Resume()
{
	assert(status_ == ThreadStatusSuspended);
	if (::ResumeThread(thread_handle_) == ((DWORD)-1))
		{ return false; }
	status_ = ThreadStatusRunning;
	return true;
}




//-----------------------------------------------------------------------------------------------------------
/*!	
	\brief Schlafen

	Pausiert den Thread fuer die uebergebene Zeit
*/
void Thread::Sleep(
	unsigned int ms
)
{
	assert((ms != INFINITE));
	::Sleep(ms);
}




//-----------------------------------------------------------------------------------------------------------
/*!	
	\brief Return Code setzen
	\param return_code Neuer Return Code

	Setzt den Return Code des Threads
*/
void Thread::SetReturnCode(
	const unsigned int return_code
)
{
	return_code_ = return_code;
}




//-----------------------------------------------------------------------------------------------------------
/*!	
	\brief Return Code zurueckgeben
	\return Return Code

	Erst gueltig nach Beendigung des Threads!
*/
const unsigned int Thread::GetReturnCode()
{
	assert(status_ == ThreadStatusDeleted);
	DWORD return_code = 0;
	::GetExitCodeThread(thread_handle_, (DWORD*)&return_code);
	return_code_ = return_code;
	return return_code_;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Initialisieren

	Diese Methode wird einmal aufgerufen nachdem der Thread erstellt wurde. Jede abgeleitete
	Klasse kann diese Methode überschreiben und den Initialisierungscode darin abhandeln.
*/
void Thread::Startup()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!	
	This function is called once to perform the threads main piece of work. The thread has to 
	take care to check the terminate_requested_ flag in order to be noticed when it has to 
	exit this function.
	
	If the thread does work in a loop it might be a better idea to overwrite
	the "Loop" methode as this implementation by default calls this methode in an infinite loop
	until the thread is asked to exit. so this default implementation does the looping
	and checking of the termination request.
*/
void Thread::Run()
{
	// Call Loop until a request to terminate occurs
	// this is why terminate_requested_ is declared volatile since it is used by
	// this thread, but changed inside another thread (when stopping)
	while (::InterlockedCompareExchange(&terminate_requested_, 0, 0) == 0)
	{
		Loop();
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	This methode is called in an infinite loop by the default implementation of the thread class.
*/
void Thread::Loop()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	called once after the main thread function has been executed. should perform just short cleanup
	code since it may block other threads. sets the return code.
*/
unsigned int Thread::Shutdown()
{
	return return_code_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	if the thread performs blocking operations or wait for events then it has to overwrite this
	function in order to be woken up if another thread requests its termination.
*/
void Thread::Wakeup()
{
}
