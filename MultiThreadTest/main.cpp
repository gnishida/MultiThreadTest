﻿#include <iostream>  
#include <boost/thread.hpp>   
#include <boost/date_time.hpp>       
      
void workerFunc()  
{  
    boost::posix_time::seconds workTime(3);          
    std::cout << "Worker: running" << std::endl;    
      
    // Pretend to do something useful... 
    boost::this_thread::sleep(workTime);          
    std::cout << "Worker: finished" << std::endl;  
}    
      
int main(int argc, char* argv[])  
{  
    std::cout << "main: startup" << std::endl;          
    boost::thread workerThread(workerFunc);    
      
    std::cout << "main: waiting for thread" << std::endl;          
    workerThread.join();    
      
    std::cout << "main: done" << std::endl;          
    return 0;  
}