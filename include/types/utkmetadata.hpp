//===================================================================================================================================
// @file	utkmetadata.hpp
// @author	Jac Jenkins
// @date	06/02/2025
// 
// @brief	Header file containing type declarations & associated helper funcitons
//			for the Utility Toolkit metadata type
//===================================================================================================================================

#pragma once

#include <type_traits>
#include <iostream>
#include <concepts>
#include <vector>
#include <format>
#include <string>
#include <tuple>

/**
 * @brief A concept to check if the type is a tuple-like type.
 *
 * This concept ensures that 'T' has a 'std::tuple_size' specialization,
 * which implies that 'T' behaves like a tuple(or similar container that
 * allows the use of 'std::tuple_size<T>' to retrieve its size).
*/
template<typename T>
concept TupleType = requires{ typename std::tuple_size<T>::type; };

/**
 * @brief Type helper to sanitize input types for tuple construction
 * 
 * The alias transforms const char* types into std::string to avoid
 * dangling pointers and ensure safe memory ownership in tuples.
 * 
 * All other types are passed through unchanged(after type decay)
 * 
 * @tparam T: The original type to sanitize.
*/
template <typename T>
using CleanedType = 
    std::conditional_t<std::is_same_v<std::decay_t<T>, const char*>, std::string, std::decay_t<T>>;

namespace UTK::Types {
    namespace Metadata {

        using ReflectedValues = std::vector<std::string>;

        template<TupleType T>
        class Metadata {
        
        private:
            T _data;
        
        public:
            explicit Metadata(T&& data) noexcept : _data(std::move(data)) {}
            explicit Metadata(const T& data) : _data(data) {}
            
            /**
             * @brief Accessor function to print tuple contents
            */
            void print() const {

                std::apply([](const auto&... args) {
                    ((std::cout << args << " "), ...);  // Fold expression to print each element
                }, _data);

                std::cout << std::endl;
            }

            /**
             * @brief Getter method to return a specific tuple element.
             * 
             * Method template to access values of the internal tuple compile 
             * time.
             *
             * @tparam Index: The index of the tuple to evaluate.
             * 
             * @return The value at the associated index.
            */
            template<std::size_t Index>
            decltype(auto) getTupleElement() const {
                return std::get<Index>(_data);
            }

            /**
             * @brief Extracts and converts tuple elements to strings
             * 
             * Iterates over all elements in the internal tuple '_data', converting each element
             * to a 'std::string' and collecting them into a 'ReflectedValues' vector.
             * 
             * @return A `ReflectedValues` vector containing the string representation of each element in `_data`.
             * 
             * @note If an element is already a `std::string`, it is used as-is. If it is a C-style string 
             *       (`const char*`), it is converted to `std::string`. If it is an arithmetic type 
             *       (e.g., int, float), it is converted using `std::to_string`. Any unsupported types 
             *       trigger a compile-time static assertion. 
             * 
             * @note This method uses `std::apply` and a fold expression to unpack the tuple and apply
             *       the conversion logic per element.
            */
            ReflectedValues getData() {
                
                ReflectedValues result;

                std::apply([&result](const auto&... args) {
                    (..., result.emplace_back(
                        [](const auto& arg) -> std::string {

                            using T = std::decay_t<decltype(arg)>;

                            if constexpr (std::is_same_v<T, std::string>)
                                return arg;
                            else if constexpr (std::is_same_v<T, const char*>)
                                return std::string(arg);
                            else if constexpr (std::is_convertible_v<T, std::string>)
                                return std::string(arg);
                            else if constexpr (std::is_arithmetic_v<T>)
                                return std::to_string(arg);
                            else
                                static_assert(sizeof(T) == 0, "Unsupported type in _data");
                        }(args)
                    ));
                }, _data);

                return result;
            }
        };

        namespace Helpers {

            /**
             * @brief A utility function that creates a tuple from the given arguments.
             *
             * This function acts as a wrapper around `std::make_tuple` to create a tuple
             * from a variable number of arguments, preserving their types. It forwards the
             * arguments to `std::make_tuple` using perfect forwarding to maintain efficiency
             * and correctness with respect to lvalue and rvalue references.
             *
             * @tparam ...Args: The types of the arguments, passed as a parameter pack.
             * @param args:     The arguments to be included in the resulting tuple, forwarded as-is.
             *
             * @return A tuple containing the passed arguments.
             *
             * @note This function is currently a direct wrapper around `std::make_tuple`,
             *       but it can be extended in the future to add additional behavior, such as
             *       logging, validation, or transformations of the input arguments. The reason
             *       for providing this simple wrapper was to ensure consistency between my API
             *       and not requiring the user to use the STL features as default, whilst
             *       allowing it to also be used.
             * 
             * @note See CleanedTypes for further info
            */
            template<typename ...Args>
            std::tuple<CleanedType<Args>...> makeTuple(Args&&... args) {
                return std::make_tuple(std::forward<Args>(args)...);
            }
        }
    }
}