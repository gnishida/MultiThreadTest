#include <iostream>  
#include <boost/thread.hpp>   
#include <boost/date_time.hpp>       

static boost::mutex mtx;
int count = 0;

void threadFunc(const char* name)
{  
	for (int i = 0; i < 10; ++i) {
		boost::mutex::scoped_lock lock(mtx);
		{
			int c = count;
			boost::this_thread::sleep(boost::posix_time::milliseconds(100));
			count = c + 1;
			printf("%s: %d\n", name, count);
		}
	}      
}
      
int main(int argc, char* argv[])  
{
	boost::thread thread1(threadFunc, "A");
	boost::thread thread2(threadFunc, "B");
      
    thread1.join();    
	thread2.join();
      
    return 0;  
}