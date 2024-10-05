#pragma once
#include <exception>
#include <string>

class NotImplementedException : public std::exception
{

  public:
    NotImplementedException(const char *error = "Functionality not yet implemented!")
    {
        error_message = error;
    }

    const char *what() const noexcept
    {
        return error_message.c_str();
    }

  private:
    std::string error_message;
};
