#include "nodes/MetaNodeOutputArray.h"
#include "MetaNodeOutputArray.h"



namespace btrack { namespace nodes
{

template <typename T>
inline MetaNodeOutputArray<T>::MetaNodeInputArrayIterator MetaNodeOutputArray<T>::MetaNodeInputArrayBegin() { return MetaNodeOutputArray<T>::MetaNodeInputArrayIterator(mChildren.begin()); }
template <typename T>
inline MetaNodeOutputArray<T>::MetaNodeInputArrayIterator MetaNodeOutputArray<T>::MetaNodeInputArrayEnd() { return MetaNodeOutputArray<T>::MetaNodeInputArrayIterator(mChildren.end()); }
template <typename T>
inline MetaNodeOutputArray<T>::MetaNodeInputIterator MetaNodeOutputArray<T>::MetaNodeInputBegin() { return MetaNodeOutputArray<T>::MetaNodeInputIterator(mChildren.begin()); }
template <typename T>
inline MetaNodeOutputArray<T>::MetaNodeInputIterator MetaNodeOutputArray<T>::MetaNodeInputEnd() { return MetaNodeOutputArray<T>::MetaNodeInputIterator(mChildren.end()); }
template <typename T>
inline MetaNodeOutputArray<T>::_MetaNodeInputIterator MetaNodeOutputArray<T>::_MetaNodeInputBegin() { return MetaNodeOutputArray<T>::_MetaNodeInputIterator(mChildren.begin()); }
template <typename T>
inline MetaNodeOutputArray<T>::_MetaNodeInputIterator MetaNodeOutputArray<T>::_MetaNodeInputEnd() { return MetaNodeOutputArray<T>::_MetaNodeInputIterator(mChildren.end()); }
template <typename T>
inline MetaNodeOutputArray<T>::NodeOutputArrayIterator MetaNodeOutputArray<T>::NodeOutputArrayBegin() { return MetaNodeOutputArray<T>::NodeOutputArrayIterator(mOutputs.begin()); }
template <typename T>
inline MetaNodeOutputArray<T>::NodeOutputArrayIterator MetaNodeOutputArray<T>::NodeOutputArrayEnd() { return MetaNodeOutputArray<T>::NodeOutputArrayIterator(mOutputs.end()); }
template <typename T>
inline MetaNodeOutputArray<T>::_NodeOutputIterator MetaNodeOutputArray<T>::_NodeOutputBegin() { return MetaNodeOutputArray<T>::_NodeOutputIterator(mOutputs.begin()); }
template <typename T>
inline MetaNodeOutputArray<T>::_NodeOutputIterator MetaNodeOutputArray<T>::_NodeOutputEnd() { return MetaNodeOutputArray<T>::_NodeOutputIterator(mOutputs.end()); }
template <typename T>
inline MetaNodeOutputArray<T>::NodeOutputIterator MetaNodeOutputArray<T>::NodeOutputBegin() { return MetaNodeOutputArray<T>::NodeOutputIterator(mOutputs.begin()); }
template <typename T>
inline MetaNodeOutputArray<T>::NodeOutputIterator MetaNodeOutputArray<T>::NodeOutputEnd() { return MetaNodeOutputArray<T>::NodeOutputIterator(mOutputs.end()); }


template <typename T>
inline MetaNodeOutputArray<T> &btrack::nodes::MetaNodeOutputArray<T>::operator>>(MetaNodeInputArray<T> &input)
{
	// mChildren.push_back(input);
	return *this;
}



}} // btrack::nodes