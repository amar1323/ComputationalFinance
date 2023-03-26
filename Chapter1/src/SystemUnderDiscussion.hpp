#include "Algorithm.hpp"


template <typename T, template <typename> class Source, template <typename> class Sink>
class SUD : private Source<T>, private Sink<T> {
   private:
    // Define 'provides'/'requires' interfaces of satellite systems
    using Source<T>::getData;
    using Sink<T>::sendData;
    using Sink<T>::end;

    // Conversion
    IAlgorithm<T> convert;

   public:
    SUD(const IAlgorithm<T>& conversion) : convert(conversion) {}
    void run(const T& S) {
        OptionData<T> options = getData();
        ComputedData<T> output = convert(options, S);
        sendData(output);
        end();
    }
};