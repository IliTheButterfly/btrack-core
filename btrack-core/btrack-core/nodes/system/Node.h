#pragma once

#include "nodes/system/NodeIO.h"

namespace btrack { namespace nodes { namespace system {

class Node : public _Node
{
public:
	virtual void process() = 0;
};



}}} // namespace btrack::nodes::system