#include <iostream>

using namespace std;

class ErrorHandler {
private:
    static ErrorHandler* instance;
    ErrorHandler() { std::cout << "Singleton created" << std::endl; }
    ErrorHandler(const ErrorHandler&) = delete;             // delete copy constructor
    ErrorHandler& operator=(const ErrorHandler&) = delete;  // delete copy assignment
public:
    static ErrorHandler* getInstance() {
        if (instance == nullptr) {
            instance = new ErrorHandler();
        }
        return instance;
    }
};