#include <functional>
#include <utility>
#include <iostream>
#include <tuple>


// Option data {K, T, r, sig/v} from Input system
template <typename T>
using OptionData = std::tuple<T, T, T, T>;

// Return type of Algorithm system
// We compute V, delta and gamma
template <typename T>
using ComputedData = std::tuple<T, T, T>;

// The abstract interface to compute V, delta and gamma

template <typename T>
using IAlgorithm = std::function<ComputedData<T>(const OptionData<T>& options, const T& S)>;

template <typename T, template <typename> class Source,template <typename> class Sink>
class SUD : private Source<T>, private Sink<T> {
   private:
    // Define 'provides'/'requires' interfaces of satellite systems
    using Source<T>::getData;
    using Sink<T>::sendData;
    using Sink<T>::end;

    // Conversion
    IAlgorithm<T> convert;

   public:
    SUD(const IAlgorithm<T>& conversion): convert(conversion) {}
    void run(const T& S) {
        OptionData<T> options = getData();
        ComputedData<T> output = convert(options, S);
        sendData(output);
        end();
    }
};

template <typename T>
class InputData {
   protected:
    OptionData<T> getData() const {
        return m_data;
    }

   private:
    OptionData<T> m_data;
};

template <typename T>
class OutputData {
   protected:
    void sendData(const ComputedData<T>& sentData) {
        m_data = sentData;
    }
    void end() {
        std::cout << "Sinking ends" << std::endl;
    }

   private:
    ComputedData<T> m_data;
};

int main() {

    using SUDdouble = SUD<double, InputData, OutputData>;
    SUDdouble firstSUD([](const OptionData<double>& options, const double& S){
        return ComputedData<double>(std::get<0>(options), std::get<1>(options), S*std::get<2>(options)/std::get<3>(options));
    });
    return 0;
}