#pragma once

#include <type_traits>

namespace horseIR {
    namespace misc {
        template <class T, bool FlagPtr = std::is_pointer<T>::value>
        struct TrivialSuppiler {} ;

        template <class T>
        struct TrivialSuppiler<T, true> {
            static T supply() { return nullptr ; }
        } ;

        template <class T>
        struct TrivialSuppiler<T, false> {
            static T supply() { return T() ; }
        } ;
    }
}
