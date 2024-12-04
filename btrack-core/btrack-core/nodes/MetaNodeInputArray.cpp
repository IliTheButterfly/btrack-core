#include "nodes/MetaNodeInputArray.h"
#include "nodes/NodeInputArray.h"

namespace btrack { namespace nodes
{

template <typename T> 
MetaNodeInputArray<T>::NodeInputArrayIterator MetaNodeInputArray<T>::NodeInputArrayBegin() { return MetaNodeInputArray<T>::NodeInputArrayIterator(mInputs.begin()); }
template <typename T> 
MetaNodeInputArray<T>::NodeInputArrayIterator MetaNodeInputArray<T>::NodeInputArrayEnd() { return MetaNodeInputArray<T>::NodeInputArrayIterator(mInputs.end()); }
template <typename T> 
MetaNodeInputArray<T>::_NodeInputIterator MetaNodeInputArray<T>::_NodeInputBegin() { return MetaNodeInputArray<T>::_NodeInputIterator(mInputs.begin()); }
template <typename T> 
MetaNodeInputArray<T>::_NodeInputIterator MetaNodeInputArray<T>::_NodeInputEnd() { return MetaNodeInputArray<T>::_NodeInputIterator(mInputs.end()); }
template <typename T> 
MetaNodeInputArray<T>::NodeInputIterator MetaNodeInputArray<T>::NodeInputBegin() { return MetaNodeInputArray<T>::NodeInputIterator(mInputs.begin()); }
template <typename T> 
MetaNodeInputArray<T>::NodeInputIterator MetaNodeInputArray<T>::NodeInputEnd() { return MetaNodeInputArray<T>::NodeInputIterator(mInputs.end()); }
	
}} // btrack::nodes