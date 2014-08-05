// NXT++ test.cpp : Defines the entry point for the console application.
//
//LEGO 


#include <iostream>
//#include <fstream>
//#include <time.h>
//#include <stdio.h>
#include <windows.h>          // for HANDLE
#include <process.h>    /* CreateRecorderThread, _endthread */

void  silly( void * );   // function prototype

class ThreadX
{
private:
  int loopStart;
  int loopEnd;
  int dispFrequency;

public:
  std::string threadName;
  // Constructor
  ThreadX( int startValue, int endValue, int frequency )
  {
    loopStart = startValue;
    loopEnd = endValue;
    dispFrequency = frequency;
  }

  // In C++ you must employ a free (C) function or a static
  // class member function as the thread entry-point-function.

  static unsigned __stdcall ThreadStaticEntryPoint(void * pThis)
  {
    ThreadX * pthX = (ThreadX*)pThis;   // the tricky cast

    pthX->ThreadEntryPoint();    // now call the true entry-point-function

    // A thread terminates automatically if it completes execution,
    // or it can terminate itself with a call to _endthread().

    return 1;          // the thread exit code
  }

  void ThreadEntryPoint()
  {
    // This is the desired entry-point-function but to get
    // here we have to use a 2 step procedure involving
    // the ThreadStaticEntryPoint() function.
    for (int i = loopStart; i <= loopEnd; ++i)
    {
      if (i % dispFrequency == 0)
      {
        printf( "%s: i = %d\n", threadName.c_str(), i );
      }
    }
    printf( "%s thread terminating\n", threadName.c_str() );
  }
};


int main()
{
	//printf( "Now in the main() function.\n" );
  std::cout << "Now in the main() function." << std::endl;

  // creat thread object
  ThreadX * o1 = new ThreadX( 0, 1000000, 20000 );
  o1->threadName = "t1";

  // start thread (suspended) via static thread entry point function
  HANDLE   hth1;
  unsigned  uiThread1ID;

  hth1 = (HANDLE)_beginthreadex( NULL,         // security
    0,            // stack size
    ThreadX::ThreadStaticEntryPoint,
    o1,           // arg list
    CREATE_SUSPENDED,  // so we can later call ResumeThread()
    &uiThread1ID );

  if ( hth1 == 0 )
    printf("Failed to create thread 1\n");
    
  // get exit code for suspended thread
  DWORD   dwExitCode;

  GetExitCodeThread( hth1, &dwExitCode );  // should be STILL_ACTIVE = 0x00000103 = 259
  printf( "initial thread 1 exit code = %u\n", dwExitCode );

  ThreadX * o2 = new ThreadX( -1000000, 0, 20000 );

  HANDLE   hth2;
  unsigned  uiThread2ID;

  hth2 = (HANDLE)_beginthreadex( NULL,         // security
    0,            // stack size
    ThreadX::ThreadStaticEntryPoint,
    o2,           // arg list
    CREATE_SUSPENDED,  // so we can later call ResumeThread()
    &uiThread2ID );

  if ( hth2 == 0 )
    printf("Failed to create thread 2\n");

  GetExitCodeThread( hth2, &dwExitCode );  // should be STILL_ACTIVE = 0x00000103 = 259
  printf( "initial thread 2 exit code = %u\n", dwExitCode );

  o2->threadName = "t2";


  //start thread since it was created suspended 
  ResumeThread( hth1 );
  ResumeThread( hth2 );

  // wait for thread to finish. If not done main thread will finish and all started threads will be terminated
  WaitForSingleObject( hth1, INFINITE );
  WaitForSingleObject( hth2, INFINITE );

  // get exit code for finished thread
  GetExitCodeThread( hth1, &dwExitCode );
  printf( "thread 1 exited with code %u\n", dwExitCode );

  GetExitCodeThread( hth2, &dwExitCode );
  printf( "thread 2 exited with code %u\n", dwExitCode );
  // clean up
  CloseHandle( hth1 );

  delete o1;
  o1 = NULL;

  CloseHandle( hth2 );

  delete o2;
  o2 = NULL;

  std::cin.ignore();

	return 0;
}

void  silly( void *arg )
{
  //printf( "The silly() function was passed %d\n", (INT_PTR)arg ) ;
  std::cout << "The silly() function was passed " << (int)arg << std::endl;
}