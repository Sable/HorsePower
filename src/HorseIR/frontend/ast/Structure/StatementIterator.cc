#include <cassert>
#include <utility>

#include "../AST.h"

using namespace horseIR::ast ;

StatementIterator::StatementIterator()
    : ptr{nullptr}
{}

StatementIterator::StatementIterator(Statement *statement)
    : ptr{statement}
{}

StatementIterator::StatementIterator(const StatementIterator &iterator)
    : ptr{iterator.ptr}
{}

StatementIterator::StatementIterator(StatementIterator &&iterator)
    : ptr{iterator.ptr}
{}

StatementIterator::value_type StatementIterator::nextOnTrue()
{
    StatementIterator::value_type current = ptr ;
    ptr = ptr->getOutwardFlow().first ;
    return current ;
}

StatementIterator::value_type StatementIterator::nextOnFalse()
{
    StatementIterator::value_type current = ptr ;
    ptr = ptr->getOutwardFlow().second ;
    return current ;
}

StatementIterator::value_type StatementIterator::operator *() {
    return ptr ;
}

StatementIterator& StatementIterator::operator=(const StatementIterator &iterator)
{
    ptr = iterator.ptr ;
    return *this ;
}

StatementIterator& StatementIterator::operator=(StatementIterator &&iterator)
{
    ptr = iterator.ptr ;
    return *this ;
}

bool StatementIterator::operator==(const horseIR::ast::StatementIterator &iterator)
{
    return ptr == iterator.ptr ;
}

bool StatementIterator::operator!=(const horseIR::ast::StatementIterator &iterator)
{
    return ptr != iterator.ptr ;
}
