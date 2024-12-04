#include "nodes/MetaNodeInputValue.h"


namespace btrack { namespace nodes
{

template <typename T>
MetaNodeInputValue<T>::NodeInputValueIterator MetaNodeInputValue<T>::NodeInputValueBegin() { return MetaNodeInputValue<T>::NodeInputValueIterator(mInputs.begin()); }
template <typename T>
MetaNodeInputValue<T>::NodeInputValueIterator MetaNodeInputValue<T>::NodeInputValueEnd() { return MetaNodeInputValue<T>::NodeInputValueIterator(mInputs.end()); }
template <typename T>
MetaNodeInputValue<T>::_NodeInputIterator MetaNodeInputValue<T>::_NodeInputBegin() { return MetaNodeInputValue<T>::_NodeInputIterator(mInputs.begin()); }
template <typename T>
MetaNodeInputValue<T>::_NodeInputIterator MetaNodeInputValue<T>::_NodeInputEnd() { return MetaNodeInputValue<T>::_NodeInputIterator(mInputs.end()); }
template <typename T>
MetaNodeInputValue<T>::NodeInputIterator MetaNodeInputValue<T>::NodeInputBegin() { return MetaNodeInputValue<T>::NodeInputIterator(mInputs.begin()); }
template <typename T>
MetaNodeInputValue<T>::NodeInputIterator MetaNodeInputValue<T>::NodeInputEnd() { return MetaNodeInputValue<T>::NodeInputIterator(mInputs.end()); }


}} // btrack::nodes