#include <tuple>
#include <thread>
#include <iostream>

// Option data {K, T, r, sig/v} from Input system
template <typename T>
using OptionData = std::tuple<T, T, T, T>;

// Return type of Algorithm system
// We compute V, delta and gamma
template <typename T>
using ComputedData = std::tuple<T, T, T>;

template <typename T>
void ThreadSafePrint(const ComputedData<T>& tup) {  // Function to avoid garbled output on the console
    std::mutex my_mutex;
    std::lock_guard<std::mutex> guard(my_mutex);
    std::cout << "(" << std::get<0>(tup) << "," << std::get<1>(tup)
              << "," << std::get<2>(tup) << ")\n";
}

template <typename T>
class OutputData {
   protected:
    void sendData(const ComputedData<T>& tup) const {
        ThreadSafePrint(tup);
    }
    void end() {
        std::cout << "Sinking ends" << std::endl;
    }

   private:
    ComputedData<T> m_data;
};

template <typename T>
class InputData {
   protected:
    OptionData<T> getData() {  // Function object
        T K = 65.0;
        T expiration = 0.25;
        T r = 0.08;
        T v = 0.3;
        OptionData<T> optData(K, expiration, r, v);
        return optData;
    }
};



