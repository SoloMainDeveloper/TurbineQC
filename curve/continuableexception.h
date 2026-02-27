#pragma once
#include <stdexcept>
#include <string>

class ContinuableException : public std::runtime_error {
public:
    explicit ContinuableException(const std::string& message)
        : std::runtime_error(message) {
    }

    ContinuableException(const std::string& message, int errorCode)
        : std::runtime_error(message), errorCode_(errorCode) {
    }

    int errorCode() const noexcept {
        return errorCode_;
    }

private:
    int errorCode_ = 0;
};

#define ARGUMENT_ASSERT(argToCheck, msg) do { \
        if(!(argToCheck)) { \
            QMessageBox::warning(nullptr, "Error", msg, "Ok"); \
            throw ContinuableException(msg); \
        } \
} while(0)