#include "nodes/MetaNodeOutputValue.h"
#include "nodes/MetaNodeInputArray.h"
#include "nodes/MetaNodeInputValue.h"
#include "nodes/MetaNodeOutputArray.h"
#include "MetaNodeOutputValue.h"

namespace btrack { namespace nodes
{

template <typename T>
inline MetaNodeOutputValue<T>::MetaNodeInputIterator MetaNodeOutputValue<T>::MetaNodeInputBegin() { return MetaNodeOutputValue<T>::MetaNodeInputIterator(mChildren.begin()); }
template <typename T>
inline MetaNodeOutputValue<T>::MetaNodeInputIterator MetaNodeOutputValue<T>::MetaNodeInputEnd() { return MetaNodeOutputValue<T>::MetaNodeInputIterator(mChildren.end()); }
template <typename T>
inline MetaNodeOutputValue<T>::_MetaNodeInputIterator MetaNodeOutputValue<T>::_MetaNodeInputBegin() { return MetaNodeOutputValue<T>::_MetaNodeInputIterator(mChildren.begin()); }
template <typename T>
inline MetaNodeOutputValue<T>::_MetaNodeInputIterator MetaNodeOutputValue<T>::_MetaNodeInputEnd() { return MetaNodeOutputValue<T>::_MetaNodeInputIterator(mChildren.end()); }
template <typename T>
inline MetaNodeOutputValue<T>::NodeOutputValueIterator MetaNodeOutputValue<T>::NodeOutputValueBegin() { return MetaNodeOutputValue<T>::NodeOutputValueIterator(mOutputs.begin()); }
template <typename T>
inline MetaNodeOutputValue<T>::NodeOutputValueIterator MetaNodeOutputValue<T>::NodeOutputValueEnd() { return MetaNodeOutputValue<T>::NodeOutputValueIterator(mOutputs.end()); }
template <typename T>
inline MetaNodeOutputValue<T>::_NodeOutputIterator MetaNodeOutputValue<T>::_NodeOutputBegin() { return MetaNodeOutputValue<T>::_NodeOutputIterator(mOutputs.begin()); }
template <typename T>
inline MetaNodeOutputValue<T>::_NodeOutputIterator MetaNodeOutputValue<T>::_NodeOutputEnd() { return MetaNodeOutputValue<T>::_NodeOutputIterator(mOutputs.end()); }
template <typename T>
inline MetaNodeOutputValue<T>::NodeOutputIterator MetaNodeOutputValue<T>::NodeOutputBegin() { return MetaNodeOutputValue<T>::NodeOutputIterator(mOutputs.begin()); }
template <typename T>
inline MetaNodeOutputValue<T>::NodeOutputIterator MetaNodeOutputValue<T>::NodeOutputEnd() { return MetaNodeOutputValue<T>::NodeOutputIterator(mOutputs.end()); }

template <typename T>
MetaNodeOutputValue<T> &MetaNodeOutputValue<T>::operator>>(MetaNodeInputValue<T> &input)
{
	return *this;
}

template <typename T>
MetaNodeOutputValue<T> &MetaNodeOutputValue<T>::operator>>(MetaNodeInputArray<T> &input)
{
	return *this;
}


}} // btrack::nodes