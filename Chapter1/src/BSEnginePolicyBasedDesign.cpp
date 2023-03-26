#include <cmath>
#include <functional>
#include <iostream>
#include <tuple>
#include <utility>

#include "SystemUnderDiscussion.hpp"


using value_type = double;


int main() {
    Processing<value_type> converter;
    // Calls
    SUD<value_type, InputData, OutputData> callPricer(converter);
    value_type S = 60.0;
    callPricer.run(S);
    // Puts
    ProcessingII<value_type> converter2;
    SUD<value_type, InputData, OutputData> putPricer(converter2);
    value_type S2 = 60.0;
    putPricer.run(S2);
    return 0;
}