#pragma once

namespace horseIR
{
namespace misc
{

struct Collections {
  template<class InputIt, class Stream, class Delimiter>
  static void
  toStream (Stream &stream, InputIt begin,
            InputIt end, const Delimiter &delimiter)
  {
    while (begin != end)
      {
        stream << *begin;
        if ((++begin) != end) stream << delimiter;
      }
  }

  template<class InpuIt, class Stream, class Delimiter, class Finisher>
  static void
  toStream (Stream &stream, InpuIt begin, InpuIt end,
            const Delimiter &delimiter, const Finisher &finisher)
  {
    while (begin != end)
      {
        stream << *begin;
        if ((++begin) != end) stream << delimiter;
      }
    stream << finisher;
  };
};

}
}