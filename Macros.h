#pragma once
#include<exception>
#include<string>
#include<concepts>
#define _NODISCARD [[nodiscard]]

#define _CONSTEXPR constexpr
#define _PANAGIOTIS_BEGIN namespace panagiotis{
#define _PANAGIOTIS_END }
_PANAGIOTIS_BEGIN

class pop_from_an_empty_list :public std::exception
{
private:
    std::string errorMessage; // To store the error message
public:
    //Constructor to initialize the error message
    explicit pop_from_an_empty_list(const std::string& message)
        : errorMessage(message) {
    }

    // Override the what() method
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};



class tried_to_access_an_empty_list_ :public std::exception
{
private:
    std::string errorMessage; // To store the error message
public:
    //Constructor to initialize the error message
    explicit tried_to_access_an_empty_list_(const std::string& message)
        : errorMessage(message) {
    }

    // Override the what() method
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};
class tried_to_access_an_empty_iterator_ :public std::exception
{
private:
    std::string errorMessage; // To store the error message
public:
    //Constructor to initialize the error message
    explicit tried_to_access_an_empty_iterator_(const std::string& message)
        : errorMessage(message) {
    }

    // Override the what() method
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};


class not_a_valid_position :public std::exception
{
private:
    std::string errorMessage; // To store the error message
public:
    //Constructor to initialize the error message
    explicit not_a_valid_position(const std::string& message)
        : errorMessage(message) {
    }

    // Override the what() method
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};

_PANAGIOTIS_END
