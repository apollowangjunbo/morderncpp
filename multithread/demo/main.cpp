#include <iostream>
#include <thread>

void threadFn1()
{
    std::cout << "in threadFn1" << std::endl;
    std::cout << "=============" << std::endl;

}
void threadFn2(int value)
{
    std::cout << "in threadFn2" << std::endl;
    std::cout << "value:"<<value << std::endl;
    std::cout << "=============" << std::endl;
}
void threadFn3(int& value)
{
    std::cout << "in threadFn3" << std::endl;
    std::cout << "thread_begin_value:" << value << std::endl;
    value++;
    std::cout << "thread_end_value:" << value << std::endl;
    std::cout << "=============" << std::endl;

}
void join()
{
    //无参
    std::thread t1{ threadFn1 };
    t1.join();//阻塞
    //有参
    std::thread t2{ threadFn2,100 };
    t2.join();
    //引用传参
    int value = 300;
    std::thread t3{ threadFn3,std::ref(value) };
    t3.join();
    std::cout << "main_value:" << value << std::endl;
    //lambda表达式
    std::thread t4{ [&]() {
        std::cout << "in thread t4" << std::endl;
        std::cout << "thread_begin_value:" << value << std::endl;
        value++;
        std::cout << "thread_end_value:" << value << std::endl;

    } };
    t4.join();
    std::cout << "=============" << std::endl;
    //t4.join();执行第二次运行时报错
}
void detach()
{
    std::cout << "thread_detach:"  << std::endl;
    std::thread t1{ [&]() {
        for(int i=0;i<10;i++)
        {
            std::cout << "A";
        }
    } };
    std::thread t2{ [&]() {
    for (int i = 0; i < 10; i++)
    {
        std::cout << "B";
    }
    } };
    t1.detach();
    t2.detach();
    std::cout << "main_end" << std::endl;
    std::cout << "=============" << std::endl;
}
#include <chrono> 
#include <ctime> 
void thread_task(int n) {
    auto time1 = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "hello thread "
        << std::this_thread::get_id()
        << "begin time:" << time1 << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(n));
    auto time2 = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout<< " end_time: " << time2
        << " nead_pause " << n << " seconds;   "
        << " real_paused " << time2 - time1 << " seconds;"
        << std::endl;
}

void sleep()
{
    std::thread threads[5];
    std::cout << "Spawning 5 threads...\n";
    for (int i = 0; i < 5; i++) {
        threads[i] = std::thread(thread_task, i*5 + 1);
    }
    std::cout << "Done spawning threads! Now wait for them to join\n";
    for (auto& t : threads) {
        std::cout <<"begin"<< t.get_id();
        t.join();
    }
    //均匀的每一秒返回一个
    std::cout << "All threads joined.\n";
}

void  joinable()
{
    std::thread t;
    std::cout << "before starting, joinable: " << t.joinable() << '\n';

    t = std::thread(threadFn1);
    std::cout << "after starting, joinable: " << t.joinable() << '\n';

    t.join();
    std::cout << "after join, joinable: " << t.joinable() << '\n';
}
int main()
{
    join();
    detach();
    sleep();
    joinable();
    std::cin.get();
    return 0;
}