#include <iostream>
#include <memory>

using namespace std;

template <typename T>
using SP = std::shared_ptr<T>;

template <typename T>
class Strategy {
  public:
    double payoff(double S) const {
        return static_cast<const T*>(this)->payoff(S);
    }
};

class TCallStrategy : public Strategy<TCallStrategy> {
   private:
    double K;
   public:
    TCallStrategy(double k) : K(k){}
    double payoff(double S) const {
        return std::max<double>(S - K, 0);
    }
};

class TBullSpreadPayoff : public Strategy<TBullSpreadPayoff> {
   private:
    double K1;
    double K2;

   public:
    TBullSpreadPayoff(double strike1, double strike2) : K1(strike1), K2(strike2) {

    }
    double payoff(double S) const {
        if (S >= K2)
            return K2 - K1;
        if (S <= K1)
            return 0.0;
        return S - K1;
    }
};

template<typename T>
double ClientPayoff(Strategy<T>& fun, double S){
    return fun.payoff(S);
}

int main() {
    SP<TBullSpreadPayoff> tbull = make_shared<TBullSpreadPayoff>(5.,10.);
    SP<TCallStrategy> tcall = make_shared<TCallStrategy>(5.);

    auto res1 = ClientPayoff(*tbull,100.);
    auto res2 = ClientPayoff(*tcall,100.);

    return 0;
}