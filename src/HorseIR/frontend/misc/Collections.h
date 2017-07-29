#pragma once

#include <type_traits>
#include <sstream>
#include <vector>

namespace horseIR {
    namespace misc {
        struct Collections {
            template <class Container, class Function>
            static std::vector<typename std::result_of<Function(const typename Container::value_type&)>::type>
            applyAndCollect(const Container& c, const Function& f) {
                using RType = typename std::result_of<Function(const typename Container::value_type&)>::type ;
                std::vector<RType> ret ;
                ret.reserve(c.size()) ;
                for (auto iter = c.cbegin(); iter != c.cend(); ++iter) {
                    ret.push_back(f(*iter)) ;
                }
                return ret ;
            }

            template <class T, class Function>
            static std::vector<typename std::result_of<Function(const T&)>::type>
            applyAndCollect(const T arr[], std::size_t size, const Function& f) {
                using RType = typename std::result_of<Function(const T&)>::type ;
                std::vector<RType> ret ;
                ret.reserve(size) ;
                for (std::size_t iter = 0; iter < size; ++iter) {
                    ret.push_back(f(arr[iter])) ;
                }
                return ret ;
            }

            template <class Container, class UnaryOperation>
            static void apply(const Container& container, const UnaryOperation& f) {
                for (auto iter = container.begin(); iter != container.end(); ++iter) {
                    (void) f(*iter) ;
                }
                return ;
            }

            template <class T, class UnaryOperation>
            static void apply(const T arr[], std::size_t size, const UnaryOperation& f) {
                for (std::size_t iter = 0; iter < size; ++iter) {
                    (void) f(arr[iter]) ;
                }
                return ;
            }

            template <class Container, class UnaryPredicate>
            static std::vector<typename Container::value_type>
            filter(const Container& container, const UnaryPredicate& predicate) {
                std::vector<typename Container::value_type> retVector ;
                retVector.reserve(container.size()) ;
                for (auto iter = container.cbegin(); iter != container.cend(); ++iter) {
                    if (predicate(*iter)) retVector.push_back(*iter) ;
                }
                retVector.shrink_to_fit() ;
                return retVector ;
            }

            template <class T, class UnaryPredicate>
            static std::vector<T> filter(const T arr[], std::size_t size, const UnaryPredicate& predicate) {
                std::vector<T> retVector ;
                retVector.reserve(size) ;
                for (std::size_t iter = 0; iter < size; ++iter) {
                    if (predicate(arr[iter])) retVector.push_back(arr[iter]) ;
                }
                retVector.shrink_to_fit() ;
                return retVector ;
            }

            template <class Container, class Stream, class Delimiter>
            static Stream& writeToStream(Stream& s, const Container& c, const Delimiter& delimiter) {
                for (auto iter = c.cbegin(); iter != c.cend(); ++iter) {
                    auto iter_p = iter ;
                    if ((++iter_p) != c.cend()) {
                        s << *iter << delimiter ;
                    } else {
                        s << *iter ;
                    }
                }
                return s ;
            }

            template <class Container, class Stream, class Delimiter, class Finisher>
            static Stream& writeToStream(Stream& s, const Container& c,
                                         const Delimiter& delimiter, const Finisher& finisher) {
                for (auto iter = c.cbegin(); iter != c.cend(); ++iter) {
                    auto iter_p = iter ;
                    if ((++iter_p) != c.cend()) {
                        s << *iter << delimiter ;
                    } else {
                        s << *iter ;
                    }
                }
                return s << finisher;
            }

            template <class T, class Stream, class Delimiter>
            static Stream& writeToStream(Stream& s, const T c[], std::size_t size, const Delimiter& delimiter) {
                for (size_t iter = 0; iter < size; ++iter) {
                    if (iter + 1 < size) {
                        s << c[iter] << delimiter ;
                    } else  {
                        s << c[iter] ;
                    }
                }
                return s ;
            }

            template <class T, class Stream, class Delimiter, class Finisher>
            static Stream& writeToStream(Stream& s, const T c[], std::size_t size,
                                         const Delimiter& delimiter, const Finisher& finisher) {
                for (size_t iter = 0; iter < size; ++iter) {
                    if (iter + 1 < size) {
                        s << c[iter] << delimiter ;
                    } else  {
                        s << c[iter] ;
                    }
                }
                return s << finisher;
            }
        } ;
    }
}
