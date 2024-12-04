#pragma once

#include "nodes/NodeIO.h"

namespace btrack { namespace nodes
{

class Node : public _Node
{
public:
	virtual void process() = 0;
};



}} // btrack::nodes