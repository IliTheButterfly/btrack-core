#ifndef __METAOUTPUT_H__
#define __METAOUTPUT_H__


#include <vector>
#include <iterator>
#include "nodes/system/MetaNodeIO.h"
#include "nodes/system/MetaInput.h"
#include "nodes/system/Output.h"

namespace btrack::nodes::system {

class _MetaOutput : public MetaNodeIO
{
protected:
	_MetaOutput(
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			_MetaOutput::MetaNodeIO(_name, _nodeType | NodeItemType::OUTPUT, _friendlyName, _description) {}
public:
	using _OutputType = _Output;
    using _OutputPtr = type_traits::ownership::borrowed_ptr_p<_Output>;
	// using _OutputIterator = NodeIterator<_OutputPtr>;
	// NodeIteratorAccessor(_OutputIterator, _Output, _MetaOutput);

	using _MetaInputType = _MetaInput;
    using _MetaInputPtr = type_traits::ownership::borrowed_ptr_p<_MetaInput>;
	// using _MetaInputIterator = NodeIterator<_MetaInputPtr>;
	// NodeIteratorAccessor(_MetaInputIterator, _MetaInput, _MetaOutput);
	virtual _OutputPtr _OutputAt(int index) = 0;
	virtual _OutputPtr _OutputAt(int index) const = 0;
	virtual _MetaInputPtr _MetaInputAt(int index) = 0;
	virtual _MetaInputPtr _MetaInputAt(int index) const = 0;

};

template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class MetaOutput : public _MetaOutput
{
protected:
	MetaOutput(
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			MetaOutput::_MetaOutput(_name, _nodeType, _friendlyName, _description) {}
public:
	using _OutputPtr = _MetaOutput::_OutputPtr;
	using _OutputType = _MetaOutput::_OutputType;
	using _MetaInputPtr = _MetaOutput::_MetaInputPtr;
	using _MetaInputType = _MetaOutput::_MetaInputType;

    using OutputType = Output<T, I>;
    using OutputPtr = type_traits::ownership::borrowed_ptr_p<Output<T, I>>;
	// using OutputIterator = NodeIterator<OutputPtr>;
	// NodeIteratorAccessor(OutputIterator, Output, MetaOutput);

	using MetaInputType = MetaInput<T, I>;
    using MetaInputPtr = type_traits::ownership::borrowed_ptr_p<MetaInput<T, I>>;
	// using MetaInputIterator = NodeIterator<MetaInputPtr>;
	// NodeIteratorAccessor(MetaInputIterator, MetaInput, MetaOutput);

	constexpr const std::type_info& dataType() const override { return typeid(T); }
	virtual void addSender(std::weak_ptr<Sender<T, I>> sender) = 0;
	virtual void removeSender(std::weak_ptr<Sender<T, I>> sender) = 0;
	virtual void broadcast(typename I::readonlyRef value) = 0;

	NodeAtVirtual(Output);
	NodeAtVirtual(MetaInput);
};
} // namespace btrack::nodes::system
#endif // __METAOUTPUT_H__