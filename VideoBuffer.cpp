#include <iostream>            // c++ I/O

#include "opencv2/opencv.hpp"

#include <windows.h>          // for HANDLE
#include <process.h>          // for _beginthread()

#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/circular_buffer.hpp>

// Thread safe circular buffer 
template <typename T>
class circ_buffer : private boost::noncopyable
{
public:
  typedef boost::mutex::scoped_lock lock;
  circ_buffer() {}
  circ_buffer(int n) {cb.set_capacity(n);}
  void send (T imdata) {
    lock lk(monitor);
    cb.push_back(imdata);
    buffer_not_empty.notify_one();
  }
  T receive() {
    lock lk(monitor);
    while (cb.empty())
      buffer_not_empty.wait(lk);
    T imdata = cb.front();
    cb.pop_front();
    return imdata;
  }
  void clear() {
    lock lk(monitor);
    cb.clear();
  }
  int size() {
    lock lk(monitor);
    return cb.size();
  }
  void set_capacity(int capacity) {
    lock lk(monitor);
    cb.set_capacity(capacity);
  }
private:
  boost::condition buffer_not_empty;
  boost::mutex monitor;
  boost::circular_buffer<T> cb;
};

class captureVideo        // this is the producer
{
  circ_buffer<cv::Mat>* m_ringBuffer;
  cv::VideoCapture m_cap;
public:
  captureVideo( circ_buffer<cv::Mat>* buff )
  {
    m_ringBuffer = buff;
  }

  void acquirerVideoEntryPoint()
  {
    m_cap.open(0);
    if ( !m_cap.isOpened() )
    {
      std::cout << "Video capture could not be opened. ABORT!" << std::endl;
    }
    else
    {
      cv::Mat frame;
      int itr = 0;

      for (;;)
      {
        m_cap >> frame;
        cv::Size sizeFrame = frame.size();
        std::cout << "Frame Size:" << frame.size() << std::endl;
        if ( sizeFrame.height > 0 && sizeFrame.width > 0 )
        {
          std::cout << frame.size() << std::endl;
          cv::cvtColor( frame, frame, cv::COLOR_BGR2GRAY );
          double minVal, maxVal;
          int minInd, maxInd;
          cv::minMaxIdx( frame, &minVal, &maxVal, &minInd, &maxInd, cv::Mat() );
          cv::Scalar meanVal = cv::mean( frame, cv::Mat() );
          m_ringBuffer->send( frame );
          std::cout << "SET frame at iteration " << itr << ". Buffer size: " << m_ringBuffer->size() << ". Mean intensity in frame: " << meanVal[0] << std::endl;
          ++itr;
        }
      }
    }   
  }

  static unsigned __stdcall acquirerVideoStaticEntryPoint(void * pThis)
  {
    captureVideo* pCV = (captureVideo*)pThis;
    pCV->acquirerVideoEntryPoint();
    return 2;
  }

};

class showVideo
{
  circ_buffer<cv::Mat>* m_ringBuffer;
  
public:
  showVideo( circ_buffer<cv::Mat>* buff )
  {
    m_ringBuffer = buff;
    
  }

  void showVideoEntryPoint()
  {
    cv::Mat frame;
    int itr = 0;
    //cv::namedWindow( "threadedVideoShow",  cv::WINDOW_AUTOSIZE );
    for (;;)
    {
      if ( m_ringBuffer->size() > 2 )
	    {
	      frame = m_ringBuffer->receive();
	      cv::imshow( "threadedVideoShow", frame );
        cv::waitKey(10);
        cv::Scalar meanVal = cv::mean( frame, cv::Mat() );
        std::cout << "GET frame at iteration " << itr << ". Buffer size: " << m_ringBuffer->size() << ". Mean intensity in frame: " << meanVal[0] << std::endl;
        ++itr;
	    }
    }
    
  }

   static unsigned __stdcall showVideoStaticEntryPoint( void* pThis )
   {
     showVideo* pSV = (showVideo*)pThis;
     pSV->showVideoEntryPoint();
     return 2;
   }

};

int main()
{
  DWORD   dwExitCode;
  // circular buffer
  circ_buffer<cv::Mat> *ringBuffer = new circ_buffer<cv::Mat>(100);
  std::cout << "Initialized ring buffer. Buffer size: " << ringBuffer->size() << std::endl;

  // threaded video show
  showVideo* showThreaded = new showVideo( ringBuffer );

  HANDLE hthshowThreaded;
  unsigned uishowThreadedThreadID;

  hthshowThreaded = (HANDLE)_beginthreadex( NULL, 0, showVideo::showVideoStaticEntryPoint, showThreaded, CREATE_SUSPENDED, &uishowThreadedThreadID );
  if ( hthshowThreaded == 0 )
  {
    std::cout << "Failed to created threaded video show." << std::endl;
  }
  GetExitCodeThread( hthshowThreaded, &dwExitCode );
  std::cout << "initial Show video thread exit code = " << dwExitCode << std::endl;

  ResumeThread( hthshowThreaded );
  WaitForSingleObject(hthshowThreaded, 10 );
  
  // threaded video capture
  captureVideo* capThreaded = new captureVideo( ringBuffer );
  
  HANDLE  hthcapThreaded;
  unsigned  uicapThreadedThreadID;

  hthcapThreaded = (HANDLE)_beginthreadex( NULL, 0, captureVideo::acquirerVideoStaticEntryPoint, capThreaded, CREATE_SUSPENDED,  &uicapThreadedThreadID );
  
  if ( hthcapThreaded == 0 )
  {
    std::cout << "Failed to created threaded video capture." << std::endl;
  }
   GetExitCodeThread( hthcapThreaded, &dwExitCode );
   std::cout << "initial capture Video thread exit code = " << dwExitCode << std::endl;

   ResumeThread( hthcapThreaded );
   WaitForSingleObject( hthshowThreaded, 1000000 );

  // Video capture
  cv::VideoCapture cap(0);
  if ( !cap.isOpened() )
  {
    return -1;
  }

  cv::Mat frame;
  cv::Mat blur;
  cv::Mat edges;
  
  cv::namedWindow("frame",1);
  cv::namedWindow("blur",2);
  cv::namedWindow("edges",3);

  int itr = 0;
  for (;;)
  {
    cap >> frame;
    
    ringBuffer->send( frame );
    std::cout << "Set frame at iteration " << itr << ". Buffer size: " << ringBuffer->size() << std::endl;
    ++itr;

    cv::cvtColor( frame, blur, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur( blur, blur, cv::Size(7,7), 3.5, 3.5);
    cv::Canny(blur, edges, 0, 30, 3);
    
    cv::imshow("frame", frame);
    cv::imshow("blur", blur);
    cv::imshow("edges", edges);
    if(cv::waitKey(30) >= 0) 
    {
      break;
    }
  }

  

  std::cin.ignore();

  
}