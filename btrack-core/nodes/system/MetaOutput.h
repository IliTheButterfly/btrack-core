#ifndef __METAOUTPUT_H__
#define __METAOUTPUT_H__


#include <vector>
#include <iterator>
#include "nodes/system/MetaNodeIO.h"
#include "nodes/system/MetaInput.h"
#include "nodes/system/Output.h"
#include <boost/type_traits.hpp>

namespace btrack::nodes::system {

class _MetaOutput : public MetaNodeIO
{
public:
	using _OutputType = _Output;
    using _OutputPtr = type_traits::ownership::borrowed_ptr_p<_Output>;

	using _MetaInputType = _MetaInput;
    using _MetaInputPtr = type_traits::ownership::borrowed_ptr_p<_MetaInput>;
protected:
	_MetaOutput(
		std::shared_ptr<NodeRunner> runner,
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			_MetaOutput::MetaNodeIO(runner, _name, _nodeType | NodeItemType::OUTPUT, _friendlyName, _description) {}
	

public:
	
	
	virtual _OutputPtr _OutputAt(int index) = 0;
	virtual _OutputPtr _OutputAt(int index) const = 0;
	virtual _MetaInputPtr _MetaInputAt(int index) = 0;
	virtual _MetaInputPtr _MetaInputAt(int index) const = 0;

	bool canConnectTo(const std::shared_ptr<_MetaInput> other) const {
		return convertibleTo(other->dataType()) && 
			(
				(isArray() && other->isArray()) ||
				isValue()
			);
	}
	virtual bool connectTo(std::shared_ptr<_MetaInput> other) = 0;
	virtual bool disconnectFrom(std::shared_ptr<_MetaInput> other) = 0;

	virtual void attach(std::shared_ptr<_Output> input) = 0;
	virtual void detach(std::shared_ptr<_Output> input) = 0;
};

template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class MetaOutput : public _MetaOutput
{
protected:
	MetaOutput(
		std::shared_ptr<NodeRunner> runner,
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			MetaOutput::_MetaOutput(runner, _name, _nodeType, _friendlyName, _description) {}
	
public:
	using _OutputPtr = typename _MetaOutput::_OutputPtr;
	using _MetaInputPtr = typename _MetaOutput::_MetaInputPtr;
	using OutputType = Output<T, I>;
    using OutputPtr = type_traits::ownership::borrowed_ptr_p<Output<T, I>>;
	// using _OutputIterator = NodeIterator<_OutputPtr>;
	// NodeIteratorAccessor(_OutputIterator, _Output, _MetaOutput);

	using MetaInputType = MetaInput<T, I>;
    using MetaInputPtr = type_traits::ownership::borrowed_ptr_p<MetaInput<T, I>>;
	// using _MetaInputIterator = NodeIterator<_MetaInputPtr>;
	// NodeIteratorAccessor(_MetaInputIterator, _MetaInput, _MetaOutput);
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