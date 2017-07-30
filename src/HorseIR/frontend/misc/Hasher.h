#pragma once

#include <string>
#include <cstring>
#include <type_traits>
#include <stdexcept>

namespace horseIR {
namespace misc {
template <typename T, typename Enable = void> struct isSTLString : std::false_type {} ;
template <typename T>
struct isSTLString<T,
                   typename std::enable_if<std::is_same<T, std::basic_string<char>>::value ||
                                           std::is_same<T, const std::basic_string<char>>::value>::type>
    : std::true_type
{} ;

template <typename T, typename Enable = void> struct isCString : std::false_type {} ;
template <typename T>
struct isCString<T,
                 typename std::enable_if<std::is_same<T, char*>::value ||
                                         std::is_same<T, const char*>::value>::type>
    : std::true_type
{} ;

template <typename InputType, typename HashCodeType>
struct TrivialHasher {
    static HashCodeType hash(const InputType& input) {
        return HashCodeType() ;
    }
} ;
        
template <typename InputType, typename HashCodeType, typename Enable = void>
struct Hasher
{
    static HashCodeType hash(const InputType& input) = delete ;
} ;
        
template <typename InputType, typename HashCodeType>
struct Hasher<InputType, HashCodeType,
              typename std::enable_if<isSTLString<InputType>::value>::type>
{
    typedef InputType input_type ;
    typedef HashCodeType result_type ;
    static HashCodeType hash(const InputType& input) {
        HashCodeType result = 0 ;
        HashCodeType base = 1 ;
        for (auto iter = input.rbegin(); iter != input.rend(); ++iter) {
            result += static_cast<HashCodeType>(*iter) * base ;
            base = -base + (base << 5) ;
        }
        return result ;
    } ;
} ;

template <typename InputType, typename HashCodeType>
struct Hasher<InputType, HashCodeType,
              typename std::enable_if<isCString<InputType>::value>::type>
{
    typedef InputType input_type ;
    typedef HashCodeType result_type ;
    static HashCodeType hash(InputType input) {
        HashCodeType result = 0 ;
        const std::size_t strLength = std::strlen(input) ;
        for (std::size_t iter = 0; iter < strLength; ++iter) {
            HashCodeType base = 1 ;
            for (std::size_t baseCal = 0; baseCal < strLength - iter - 1; ++baseCal) {
                base = -base + (base << 5) ;
            }
            result += static_cast<HashCodeType>(input[iter]) * base ;
        }
        return result ;
    }
} ;

template <typename InputType, typename HashCodeType, typename Enable = void>
struct HasherDJB2
{
    static HashCodeType hash(const InputType& input) = delete ;
} ;
        
template <typename InputType, typename HashCodeType>
struct HasherDJB2<InputType, HashCodeType,
                  typename std::enable_if<isSTLString<InputType>::value>::type>
{
    typedef InputType input_type ;
    typedef HashCodeType result_type ;
    static HashCodeType hash(const InputType& input) {
        HashCodeType result = static_cast<HashCodeType>(5381) ;
        for (auto iter = input.cbegin(); iter != input.cend(); ++iter) {
            result = ((result << 5) + result) + static_cast<HashCodeType>(*iter) ;
        }
        return result ;
    }
} ;

template <typename InputType, typename HashCodeType>
struct HasherDJB2<InputType, HashCodeType,
                  typename std::enable_if<isCString<InputType>::value>::type>
{
    static HashCodeType hash(InputType input) {
        HashCodeType result = static_cast<HashCodeType>(5381) ;
        unsigned char c ;
        while ((c = *input++)) {
            result = ((result << 5) + result) + static_cast<HashCodeType>(c) ;
        }
        return result ;
    }
} ;

template <typename InputType, typename HashCodeType, typename Enable = void>
struct HasherSDBM
{
    static HashCodeType hash(const InputType& input) = delete ;
} ;

template <typename InputType, typename HashCodeType>
struct HasherSDBM<InputType, HashCodeType,
                  typename std::enable_if<isSTLString<InputType>::value>::type>
{
    typedef InputType input_type ;
    typedef HashCodeType result_type ;
    static HashCodeType hash(const InputType& input) {
        HashCodeType result = static_cast<HashCodeType>(0) ;
        for (auto iter = input.cbegin(); iter != input.cend(); ++iter) {
            result = (*iter) + (result << 6) + (result << 16) - result ;
        }
        return result ;
    }
} ;

template <typename InputType, typename HashCodeType>
struct HasherSDBM<InputType, HashCodeType,
                  typename std::enable_if<isCString<InputType>::value>::type>
{
    typedef InputType input_type ;
    typedef HashCodeType result_type ;
    static HashCodeType hash(InputType input) {
        HashCodeType result = static_cast<HashCodeType>(0) ;
        unsigned char c ;
        while ((c = *input++)) {
            result = static_cast<HashCodeType>(c) +
                (result << 6) + (result << 16) - result ;
        }
        return result ;
    }
} ;

template <typename InputType, typename HashCodeType, typename Enable = void>
struct HasherLoseLose {
    static HashCodeType hash(const InputType& input) = delete ;
} ;

template <typename InputType, typename HashCodeType>
struct HasherLoseLose<InputType, HashCodeType,
                      typename std::enable_if<isSTLString<InputType>::value>::type>
{
    typedef InputType input_type ;
    typedef HashCodeType output_type ;
    static HashCodeType hash(const InputType& input) {
        HashCodeType result = static_cast<HashCodeType>(0) ;
        for (auto iter = input.cbegin(); iter != input.cend(); ++iter) {
            result += static_cast<HashCodeType>(*iter) ;
        }
        return result ;
    }
} ;

template <typename InputType, typename HashCodeType>
struct HasherLoseLose<InputType, HashCodeType,
                      typename std::enable_if<isCString<InputType>::value>::type>
{
    typedef InputType input_type ;
    typedef HashCodeType output_type ;
    static HashCodeType hash(InputType input) {
        HashCodeType result = static_cast<HashCodeType>(0) ;
        unsigned char c ;
        while ((c = *input++)) {
            result += static_cast<HashCodeType>(c) ;
        }
        return result ;
    }
} ;
}
}
