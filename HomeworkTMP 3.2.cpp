#include <iostream>
#include <vector>
#include <fstream>

class Observer {
private:
    std::vector<Observer*> observers_;
public:
    void AddObserver(Observer* obs) {
        observers_.push_back(obs);
    }

    virtual void onWarning(const std::string& message) {
        for (int i = 0; i < observers_.size(); i++) {
            observers_[i]->onWarning(message);
        }
    }

    virtual void onError(const std::string& message) {
        for (int i = 0; i < observers_.size(); i++) {
            observers_[i]->onError(message);
        }
    }

    virtual void onFatalError(const std::string& message) {
        for (int i = 0; i < observers_.size(); i++) {
            observers_[i]->onFatalError(message);
        }
    }

    void RemoveObserver(Observer* obs) {
        auto it = std::remove(observers_.begin(), observers_.end(), obs);
        observers_.erase(it, observers_.end());
    }
};

class ForOnWarning : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << "Warning: " << message << std::endl;
    }
};

class ForOnError : public Observer {
public:
    void onError(const std::string& message) override {
        std::ofstream out("Error.txt", std::ios::app);
        if (out.is_open()) {
            out << "Error: " << message << std::endl;
        }
        out.close();
    }
};

class ForOnFatalError : public Observer {
public:
    void onFatalError(const std::string& message) override {
        std::cout << "FatalError: " << message << std::endl;
        std::ofstream out("Error.txt", std::ios::app);
        if (out.is_open()) {
            out << "FatalError: " << message << std::endl;
        }
        out.close();
    }
};

int main()
{
    auto observers = new Observer();
    auto war = new ForOnWarning(); observers->AddObserver(war);
    auto err = new ForOnError(); observers->AddObserver(err);
    auto fal = new ForOnFatalError(); observers->AddObserver(fal);

    observers->onWarning("Check 1");
    observers->onError("Check 2");
    observers->onFatalError("Check 3");
}