#pragma once

#include <string>
#include <cstring>

namespace horseIR {
    namespace misc {
        template <typename InputType, typename HashCodeType>
        struct Hasher {
            static HashCodeType hash(const InputType& input) ;
        } ;

        template <typename HashCodeType>
        struct Hasher<std::basic_string<char>, HashCodeType> {
            static HashCodeType hash(const std::basic_string<char>& input) {
                HashCodeType result = 0 ;
                HashCodeType base = 1 ;
                for (auto iter = input.rbegin(); iter != input.rend(); ++iter) {
                    result += static_cast<HashCodeType>(*iter) * base ;
                    base = -base + (base << 5) ;
                }
                return result ;
            } ;
        } ;

        template <typename HashCodeType>
        struct Hasher<char*, HashCodeType> {
            static HashCodeType hash(const char* input) {
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

        template <typename InputType, typename HashCodeType>
        struct HasherDJB2 {
            static HashCodeType hash(const InputType& input) ;
        } ;
        
        template <typename HashCodeType>
        struct HasherDJB2<std::basic_string<char>, HashCodeType> {
            static HashCodeType hash(const std::basic_string<char>& input) {
                HashCodeType result = static_cast<HashCodeType>(5281) ;
                for (auto iter = input.cbegin(); iter != input.cend(); ++iter) {
                    result = ((result << 5) + result) + static_cast<HashCodeType>(*iter) ;
                }
                return result ;
            }
        } ;

        template <typename HashCodeType>
        struct HasherDJB2<char*, HashCodeType> {
            static HashCodeType hash(const char* input) {
                HashCodeType result = static_cast<HashCodeType>(5381) ;
                unsigned char c ;
                while ((c = *input++)) {
                    result = ((result << 5) + result) + static_cast<HashCodeType>(c) ;
                }
                return result ;
            }
        } ;

        template <typename InputType, typename HashCodeType>
        struct HasherSDBM {
            static HashCodeType hash(const InputType& input) ;
        } ;

        template <typename HashCodeType>
        struct HasherSDBM<std::basic_string<char>, HashCodeType> {
            static HashCodeType hash(const std::basic_string<char>& input) {
                HashCodeType result = static_cast<HashCodeType>(0) ;
                for (auto iter = input.cbegin(); iter != input.cend(); ++iter) {
                    result = (*iter) + (result << 6) + (result << 16) - result ;
                }
                return result ;
            }
        } ;

        template <typename HashCodeType>
        struct HasherSDBM<char*, HashCodeType> {
            static HashCodeType hash(const char* input) {
                HashCodeType result = static_cast<HashCodeType>(0) ;
                unsigned char c ;
                while ((c = *input++)) {
                    result = static_cast<HashCodeType>(c) +
                        (result << 6) + (result << 16) - result ;
                }
                return result ;
            }
        } ;

        template <typename InputType, typename HashCodeType>
        struct HasherLoseLose {
            static HashCodeType hash(const InputType& input) ;
        } ;

        template <typename HashCodeType>
        struct HasherLoseLose<std::basic_string<char>, HashCodeType> {
            static HashCodeType hash(const std::basic_string<char>& input) {
                HashCodeType result = static_cast<HashCodeType>(0) ;
                for (auto iter = input.cbegin(); iter != input.cend(); ++iter) {
                    result += static_cast<HashCodeType>(*iter) ;
                }
                return result ;
            }
        } ;

        template <typename HashCodeType>
        struct HasherLoseLose<char*, HashCodeType> {
            static HashCodeType hash(const char* input) {
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
