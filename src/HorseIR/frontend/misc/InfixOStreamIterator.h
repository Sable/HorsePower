#pragma once

#include <iterator>

namespace horseIR
{
namespace misc
{

template<class T, class CharT = char, class Traits = std::char_traits<CharT>>
class InfixOStreamIterator : public std::iterator<std::output_iterator_tag,
                                                  void, void, void, void> {
 public:
  typedef CharT char_type;
  typedef Traits traits_type;
  typedef std::basic_ostream<CharT, Traits> ostream_type;

  InfixOStreamIterator (ostream_type &stream, const char *infix)
      : outputStream (stream), streamInfix (infix)
  {}

  explicit InfixOStreamIterator (ostream_type &stream)
      : outputStream (stream)
  {}

  InfixOStreamIterator &operator= (const T &elementToWrite)
  {
    if (!isFirstOutputElement && (streamInfix != nullptr))
      outputStream << streamInfix;
    outputStream << elementToWrite;
    isFirstOutputElement = false;
    return *this;
  }

  InfixOStreamIterator &operator* ()
  {
    return *this;
  }

  InfixOStreamIterator &operator++ ()
  {
    return *this;
  }

  InfixOStreamIterator &operator++ (int offset)
  {
    (void) offset;
    return *this;
  }

 protected:
  ostream_type &outputStream;
  bool isFirstOutputElement = true;
  const char *streamInfix = nullptr;
};

}
}