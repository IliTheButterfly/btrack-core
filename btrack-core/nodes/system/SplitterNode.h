#ifndef __SPLITTERNODE_H__
#define __SPLITTERNODE_H__

#include "nodes/system/MetaNode.h"

namespace btrack::nodes::system {

class SplitterMetaNode : public MetaNode
{
private:
	
public:

	void addPassthrough(std::shared_ptr<_MetaInput> input);
	void removePassthrough(std::shared_ptr<_MetaInput> input);
};

}

#endif // __SPLITTERNODE_H__