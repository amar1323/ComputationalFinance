#include <cmath>
#include <functional>
#include <future>
#include <iostream>
#include <thread>
#include <tuple>
#include <utility>

#include "SystemUnderDiscussion.hpp"

using value_type = double;

int main() {
    Processing<value_type> converter;
    value_type S = 60.0;
    ProcessingII<value_type> converter2;
    value_type S2 = 60.0;
    {  // Sequential
        std::cout << "Sequential run..." << std::endl;
        SUD<value_type, InputData, OutputData> callPricer(converter);
        // Calls
        callPricer.run(S);
        SUD<value_type, InputData, OutputData> putPricer(converter2);
        // Puts
        putPricer.run(S2);
        std::cout << std::endl;
        std::cout << std::endl;
    }
    auto run1 = [&converter](const value_type& S) {
        SUD<value_type, InputData, OutputData> callPricer(converter);
        callPricer.run(S);
    };
    auto run2 = [&converter2](const value_type& S) {
        SUD<value_type, InputData, OutputData> putPricer(converter2);
        putPricer.run(S);
    };

    {  // Multithreading using thread
        std::cout << "Multithreading using thread run..." << std::endl;
        std::thread t1(run1, S);
        std::thread t2(run2, S2);
        t1.join();
        t2.join();
        std::cout << std::endl;
        std::cout << std::endl;
    }

    {  // Multithreading using tasks
        std::cout << "Multithreading using task run..." << std::endl;
        std::future<void> task1(std::async(run1, S));
        std::future<void> task2(std::async(run2, S2));
        task1.wait();
        task2.wait();
        task1.get();
        task2.get();
        std::cout << std::endl;
        std::cout << std::endl;
    }
    return 0;
}