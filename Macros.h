#pragma once
#include<exception>
#include<string>
#include<concepts>
#define _NODISCARD [[nodiscard]]

#define _CONSTEXPR constexpr
#define _PANAGIOTIS_BEGIN namespace panagiotis{
#define _PANAGIOTIS_END }
#if __cplusplus > 202002L
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



//begin
template<typename _Ty>
concept Less_Than_Comparable = requires(_Ty a, _Ty b) {
    { a < b } -> std::convertible_to<bool>;
};
template<typename _Ty>
concept Greater_Than_Comparable = requires(_Ty a, _Ty b) {
    { a > b } -> std::convertible_to<bool>;
};
template<typename _Ty>
concept Comparable = requires(_Ty a, _Ty b) {
    { a == b }-> std::convertible_to<bool>;
};
template<typename _Ty>
concept Less_Or_Equal_Than_Comparable = requires(_Ty a, _Ty b) {
    { a <= b } ->std::convertible_to<bool>;
};
template<typename _Ty>
concept Greater_Or_Equal_Than_Comparable = requires(_Ty a, _Ty b) {
    { a >= b } ->std::convertible_to<bool>;
};

template<typename _Ty>
concept Can_Be_Sorted = requires(_Ty a, _Ty b) {
    requires(Comparable< _Ty>);
    requires(Greater_Than_Comparable< _Ty>);
    requires(Less_Than_Comparable< _Ty>);
    requires(Less_Or_Equal_Than_Comparable<_Ty>);
    requires(Greater_Or_Equal_Than_Comparable<_Ty>);
};
//end

_PANAGIOTIS_END
#endif
