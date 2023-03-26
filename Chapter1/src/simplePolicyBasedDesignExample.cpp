#include <string>
#include <iostream>

template<typename OutputPolicy , typename LanaguagePolicy>
class HelloWorld : private OutputPolicy, private LanaguagePolicy{
public:
    void Run(){
        Print(Message());
    }
private:
    using OutputPolicy::Print;
    using LanaguagePolicy::Message;
};

class OutputPolicyCout{
protected:
    template<typename Message>
    void Print(Message&& m){
        std::cout << m << std::endl;
    }

};

class LanaguagePolicyEnglish{
protected:
    std::string Message() const{
        return "Hello world!";
    }
};

class LanaguagePolicyFrench{
protected:
    std::string Message() const{
        return "Bonjour monde!";
    }
};

int main(){
    using HelloWorldEnglish = HelloWorld<OutputPolicyCout,LanaguagePolicyEnglish>;
    HelloWorldEnglish eng;
    eng.Run();

    using HelloWorldFrench = HelloWorld<OutputPolicyCout,LanaguagePolicyFrench>;
    HelloWorldFrench  fr;
    fr.Run();
    
    return 0;
}