#include <cmath>

#include "InputOutput.hpp"

// The abstract interface to compute V, delta and gamma

template <typename T>
using IAlgorithm = std::function<ComputedData<T>(const OptionData<T>& options, const T& S)>;

// Normal variates etc.
double n(double x) {
    const double A = 1.0 / std::sqrt(2.0 * M_PI);
    return A * std::exp(-x * x * 0.5);
}
// C++11 supports the error function
auto cndN = [](double x) { return 0.5 * (1.0 - std::erf(-x / std::sqrt(2.0))); };
double N(double x) {  // The approximation to the cumulative normal distribution
    return cndN(x);
}

// Option Pricing; give price+delta+gamma
template <typename V>
ComputedData<V> CallValues(const OptionData<V>& optData,
                           const V& S) {
    // Extract data
    V K = std::get<0>(optData);
    V T = std::get<1>(optData);
    V r = std::get<2>(optData);
    V v = std::get<3>(optData);
    V b = r;  // Stock option
    // Common functionality
    V tmp = v * std::sqrt(T);
    V d1 = (std::log(S / K) + (b + (v * v) * 0.5) * T) / tmp;
    V d2 = d1 - tmp;
    V t1 = std::exp((b - r) * T);
    V t2 = std::exp(-r * T);
    V Nd1 = N(d1);
    V Nd2 = N(d2);
    V price = (S * t1 * Nd1) - (K * t2 * Nd2);
    V delta = t1 * Nd1;
    V gamma = (n(d1) * t1) / (S * tmp);
    return std::make_tuple(price, delta, gamma);
}

// Option Pricing; give price+delta+gamma
template <typename V>
ComputedData<V> PutValues(const OptionData<V>& optData,
                          const V& S) {
    // Extract data
    V K = std::get<0>(optData);
    V T = std::get<1>(optData);
    V r = std::get<2>(optData);
    V v = std::get<3>(optData);
    V b = r;  // Stock option
    // Common functionality
    V tmp = v * std::sqrt(T);
    V d1 = (std::log(S / K) + (b + (v * v) * 0.5) * T) / tmp;
    V d2 = d1 - tmp;
    V t1 = std::exp((b - r) * T);
    V t2 = std::exp(-r * T);
    V Nmd2 = N(-d2);
    V Nmd1 = N(-d1);
    V price = (K * t2 * Nmd2) - (S * t1 * Nmd1);
    V delta = t1 * (Nmd1 - 1.0);
    V gamma = (n(d1) * t1) / (S * tmp);
    return std::make_tuple(price, delta, gamma);
}

template <typename T>
class Processing {
   public:
    ComputedData<T> convert(const OptionData<T>& optData,
                            const T& S) const {
        return CallValues(optData, S);
    }
    ComputedData<T> operator()(const OptionData<T>& optData,
                               const T& S) const {
        return CallValues(optData, S);
    }
};

template <typename T>
class ProcessingII {
   public:
    ComputedData<T> convert(const OptionData<T>& optData,
                            const T& S) const {
        return PutValues(optData, S);
    }
    ComputedData<T> operator()(const OptionData<T>& optData,
                               const T& S) const {
        return PutValues(optData, S);
    }
};