#pragma once

#include <type_traits>
#include <sstream>
#include <vector>

namespace horseIR {
namespace misc {

struct Collections {
    template <class Container, class MappingFunction>
    static std::vector<typename std::result_of<MappingFunction(const typename Container::value_type&)>::type>
    map(const Container& container, const MappingFunction& mappingFunction) ;

    template <class T, class MappingFunction>
    static std::vector<typename std::result_of<MappingFunction(const T&)>::type>
    map(const T array[], std::size_t arraySize, const MappingFunction& mappingFunction) ;

    template <class Container, class UnaryOperation>
    static void
    apply(const Container& container, const UnaryOperation& unaryOperation) ;

    template <class T, class UnaryOperation>
    static void
    apply(const T array[], std::size_t arraySize, const UnaryOperation& unaryOperation) ;

    template <class Container, class UnaryPredicate>
    static std::vector<typename Container::value_type>
    filter(const Container& container, const UnaryPredicate& unaryPredicate);

    template <class T, class UnaryPredicate>
    static std::vector<T>
    filter(const T array[], std::size_t arraySize, const UnaryPredicate& unaryPredicate) ;

    template <class Container, class UnaryPredicate>
    static bool
    check(const Container& container, const UnaryPredicate& unaryPredicate) ;

    template <class T, class UnaryPredicate>
    static bool
    check(const T array[], std::size_t arraySize, const UnaryPredicate& unaryPredicate) ;

    template <class Container, class BinaryPredicate>
    static bool
    pairwiseCheck(const Container& container, const BinaryPredicate& binaryPredicate) ;

    template <class T, class BinaryPredicate>
    static bool
    pairwiseCheck(const T array[], std::size_t arraySize, const BinaryPredicate& binaryPredicate) ;
    
    template <class Stream, class Container, class Delimiter>
    static void
    writeToStream(Stream& stream, const Container& container, const Delimiter& delimiter) ;

    template <class Stream, class Container, class Delimiter, class Finisher>
    static void
    writeToStream(Stream& stream, const Container& container, const Delimiter& delimiter, const Finisher& finisher) ;

    template <class Stream, class T, class Delimiter>
    static void
    writeToStream(Stream& stream, const T array[], std::size_t arraySize, const Delimiter& delimiter) ;

    template <class Stream, class T, class Delimiter, class Finisher>
    static void
    writeToStream(Stream& stream, const T array[], std::size_t arraySize, const Delimiter& delimiter, const Finisher& finisher) ;
} ;

template <typename T1, typename T2>
inline std::vector<typename std::result_of<T2(const typename T1::value_type&)>::type>
Collections::map(const T1& container, const T2& mappingFunction)
{
    using RType = std::vector<typename std::result_of<T2(const typename T1::value_type&)>::type> ;
    RType retVector {} ;
    retVector.reserve(container.size()) ;
    for (auto iter = container.cbegin(); iter != container.cend(); ++iter) {
        retVector.emplace_back(mappingFunction(*iter)) ;
    }
    return retVector ;
}

template <typename T1, typename T2>
inline std::vector<typename std::result_of<T2(const T1&)>::type>
Collections::map(const T1 array[], std::size_t arraySize, const T2& mappingFunction)
{
    using RType = std::vector<typename std::result_of<T2(const T1&)>::type> ;
    RType retVector {} ;
    retVector.reserve(arraySize) ;
    for (decltype(arraySize) iter = 0; iter < arraySize; ++iter) {
        retVector.emplace_back(mappingFunction(array[iter])) ;
    }
    return retVector ;
}

template <typename T1, typename T2>
inline void
Collections::apply(const T1 &container, const T2 &unaryOperation)
{
    for (auto iter = container.begin(); iter != container.end(); ++iter) {
        (void) unaryOperation(*iter) ;
    }
    return ;
}

template <typename T1, typename T2>
inline void
Collections::apply(const T1 array[], std::size_t arraySize, const T2 &unaryOperation)
{
    for (decltype(arraySize) iter = 0; iter < arraySize; ++iter) {
        (void) unaryOperation(array[iter]) ;
    }
    return ;
}

template <typename T1, typename T2>
inline std::vector<typename T1::value_type>
Collections::filter(const T1 &container, const T2 &unaryPredicate)
{
    using RType = std::vector<typename T1::value_type> ;
    RType returnVector {} ;
    returnVector.reserve(container.size()) ;
    for (auto iter = container.cbegin(); iter != container.cend(); ++iter) {
        if (unaryPredicate(*iter)) returnVector.emplace_back(*iter) ;
    }
    returnVector.shrink_to_fit() ;
    return returnVector ;
}

template <typename T1, typename T2>
inline std::vector<T1>
Collections::filter(const T1 array[], std::size_t arraySize, const T2 &unaryPredicate)
{
    using RType = std::vector<T1> ;
    RType returnVector {} ;
    returnVector.reserve(arraySize) ;
    for (decltype(arraySize) iter = 0; iter < arraySize; ++iter) {
        if (unaryPredicate(array[iter])) returnVector.emplace_back(array[iter]) ;
    }
    returnVector.shrink_to_fit() ;
    return returnVector ;
}

template <typename T1, typename T2>
inline bool
Collections::check(const T1 &container, const T2 &unaryPredicate)
{
    for (auto iter = container.cbegin(); iter != container.cend(); ++iter) {
        if (!unaryPredicate(*iter)) return false ;
    }
    return true ;
}

template <typename T1, typename T2>
inline bool
Collections::check(const T1 array[], std::size_t arraySize, const T2& unaryPredicate)
{
    for (decltype(arraySize) iter = 0; iter < arraySize; ++iter) {
        if (!unaryPredicate(array[iter])) return false ;        
    }
    return true ;
}

template <typename T1, typename T2>
inline bool
Collections::pairwiseCheck(const T1 &container, const T2 &binaryPredicate)
{
    for (auto iterA = container.cbegin(); iterA != container.cend(); ++iterA) {
        auto iterB = iterA ;
        for (++iterB; iterB != container.cend(); ++iterB) {
            if (!binaryPredicate(*iterA, *iterB)) return false ;
        }
    }
    return true ;
}

template <typename T1, typename T2>
inline bool
Collections::pairwiseCheck(const T1 array[], std::size_t arraySize, const T2& binaryPredicate)
{
    for (decltype(arraySize) iterA = 0; iterA < arraySize; ++iterA) {
        for (decltype(arraySize) iterB = iterA + 1; iterB < arraySize; ++iterB) {
            if (!binaryPredicate(array[iterA], array[iterB])) return false ;
        }
    }
    return true ;
}

template <typename T1, typename T2, typename T3>
inline void
Collections::writeToStream(T1 &stream, const T2 &container, const T3 &delimiter)
{
    for (auto iter = container.cbegin(); iter != container.cend(); ++iter) {
        stream << *iter ;
        auto iterDuplicate = iter ;
        if ((++iterDuplicate) != container.cend()) stream << delimiter ;
    }
    return ;
}

template <typename T1, typename T2, typename T3, typename T4>
inline void
Collections::writeToStream(T1 &stream, const T2 &container, const T3 &delimiter, const T4 &finisher)
{
    for (auto iter = container.cbegin(); iter != container.cend(); ++iter) {
        stream << *iter ;
        auto iterDuplicate = iter ;
        if ((++iterDuplicate) != container.cend()) stream << delimiter ;
    }
    stream << finisher ;
    return ;
}

template <typename T1, typename T2, typename T3>
inline void
Collections::writeToStream(T1 &stream, const T2 array[], std::size_t arraySize, const T3 &delimiter)
{
    for (decltype(arraySize) iter = 0; iter < arraySize; ++iter) {
        stream << array[iter] ;
        if ((iter + 1) < arraySize) stream << delimiter ;
    }
    return ;
}

template <typename T1, typename T2, typename T3, typename T4>
inline void
Collections::writeToStream(T1 &stream, const T2 array[], std::size_t arraySize, const T3 &delimiter, const T4 &finisher)
{
    for (decltype(arraySize) iter = 0; iter < arraySize; ++iter) {
        stream << array[iter] ;
        if ((iter + 1) < arraySize) stream << delimiter ;
    }
    stream << finisher ;
    return ;
}

}
}
