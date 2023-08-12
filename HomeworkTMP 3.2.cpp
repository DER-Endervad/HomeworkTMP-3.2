#include <iostream>
#include <vector>
#include <fstream>

class Observed {
public:
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

class DecoderObserved : public Observed {
protected:
    std::vector<Observed*> obs_;
    Observed* obs2;
    explicit DecoderObserved(Observed* obs) : obs2(obs) { obs_.push_back(obs2); }
    void RemoveObserver(Observed* obs) {
        auto it = std::remove(obs_.begin(), obs_.end(), obs);
        obs_.erase(it, obs_.end());
    }
};

class ForOnWarning : public DecoderObserved {
public:
    explicit ForOnWarning(Observed* obs) : DecoderObserved(obs) {}
    void onWarning(const std::string& message) override {
        std::cout << "Warning: " << message << std::endl;
    }
};

class ForOnError : public DecoderObserved {
public:
    explicit ForOnError(Observed* obs) : DecoderObserved(obs) {}
    void onError(const std::string& message) override {
        std::ofstream out("Error.txt", std::ios::app);
        if (out.is_open()) {
            out << "Error: " << message << std::endl;
        }
        out.close();
    }
};

class ForOnFatalError : public DecoderObserved {
public:
    explicit ForOnFatalError(Observed* obs) : DecoderObserved(obs) {}
    void onFatalError(const std::string& message) override {
        std::cout << "FatalError: " << message << std::endl;
        std::ofstream out("Error.txt", std::ios::app);
        if (out.is_open()) {
            out << "FatalError: " << message << std::endl;
        }
        out.close();
    }
 };

class Realization : public DecoderObserved {
public:
    explicit Realization(Observed* obs) : DecoderObserved(obs) {}
    void onWarning(Observed* obs, const std::string& message) {
        obs->onWarning(message);
    }
    void onError(Observed* obs, const std::string& message) {
        obs->onError(message);
    }
    void onFatalError(Observed* obs, const std::string& message) {
        obs->onFatalError(message);
    }
};

int main()
{
    auto war = new ForOnWarning(new Observed);
    auto err = new ForOnError(new Observed);
    auto fat = new ForOnFatalError(new Observed);

    auto realiz = new Realization(new Observed);

    realiz->onWarning(war, "check 1");
    realiz->onError(err, "check 2");
    realiz->onFatalError(fat, "check 3");
    return 0;
}