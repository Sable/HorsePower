#include <cassert>
#include <utility>

#include "../Structure.h"

using namespace horseIR::ast ;

StatementIterator::StatementIterator()
    : ptr{nullptr}
{}

StatementIterator::StatementIterator(Statement *statement)
    : ptr{statement}
{}

StatementIterator::StatementIterator(StatementIterator &iterator)
    : ptr{iterator.ptr}
{}

StatementIterator::StatementIterator(StatementIterator &&iterator)
    : ptr{iterator.ptr}
{}

StatementIterator StatementIterator::nextOnTrue() const {
    return StatementIterator(ptr->getOutwardFlow().first) ;
}

StatementIterator StatementIterator::nextOnFalse() const
{
    return StatementIterator(ptr->getOutwardFlow().second) ;
}

Statement* StatementIterator::operator *() {
    return ptr ;
}

StatementIterator& StatementIterator::operator=(StatementIterator &iterator)
{
    ptr = iterator.ptr ;
    return *this ;
}

StatementIterator& StatementIterator::operator=(StatementIterator &&iterator)
{
    ptr = iterator.ptr ;
    return *this ;
}