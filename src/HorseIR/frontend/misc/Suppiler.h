#pragma once

#include <type_traits>

namespace horseIR {
namespace misc {

template <class T>
struct TrivialSuppiler {
    static T supply() {
        return T() ;
    }
} ;

}
}
