#pragma once
#include <vector>
#include <iterator>
#include "nodes/system/MetaNodeIO.h"
#include "nodes/system/MetaInput.h"
#include "nodes/system/Output.h"

namespace btrack { namespace nodes { namespace system {

class _MetaOutput : public MetaNodeIO
{
protected:
	_MetaOutput(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			_MetaOutput::MetaNodeIO(_name, _nodeType | NodeItemType::OUTPUT, _friendlyName, _description) {}
public:
	using _OutputType = _Output;
    using _OutputPtr = std::shared_ptr<_Output>;
	using _OutputIterator = NodeIterator<_OutputPtr>;
	NodeIteratorAccessor(_OutputIterator, _Output, _MetaOutput);

	using _MetaInputType = _MetaInput;
    using _MetaInputPtr = std::shared_ptr<_MetaInput>;
	using _MetaInputIterator = NodeIterator<_MetaInputPtr>;
	NodeIteratorAccessor(_MetaInputIterator, _MetaInput, _MetaOutput);
};

template <typename T>
class MetaOutput : public _MetaOutput
{
protected:
	MetaOutput(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			MetaOutput::_MetaOutput(_name, _nodeType, _friendlyName, _description) {}
public:
	using _OutputIterator = _MetaOutput::_OutputIterator;
	using _MetaInputIterator = _MetaOutput::_MetaInputIterator;

    using OutputType = Output<T>;
    using OutputPtr = std::shared_ptr<Output<T>>;
	using OutputIterator = NodeIterator<OutputPtr>;
	NodeIteratorAccessor(OutputIterator, Output, MetaOutput<T>);

	using MetaInputType = MetaInput<T>;
    using MetaInputPtr = std::shared_ptr<MetaInput<T>>;
	using MetaInputIterator = NodeIterator<MetaInputPtr>;
	NodeIteratorAccessor(MetaInputIterator, MetaInput, MetaOutput<T>);

	constexpr const std::type_info& dataType() const override { return typeid(T); }
	virtual void addSender(std::shared_ptr<Sender<T>> sender) = 0;
	virtual void removeSender(std::shared_ptr<Sender<T>> sender) = 0;
	virtual void broadcast(SendParam_t<T> value) = 0;
};

}}} // namespace btrack::nodes::system