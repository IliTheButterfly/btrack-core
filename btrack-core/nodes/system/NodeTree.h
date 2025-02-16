#ifndef __NODETREE_H__
#define __NODETREE_H__

#include "nodes/system/Node.h"

namespace btrack::nodes::system {

template <VariantTemplate VariantType>
class NodeTree : public Clonable<NodeTree<VariantType>>
{
protected:
    boost::container::vector<Node<VariantType>> mNodes;

public:
    void compile();
    void run();
};

template <VariantTemplate VariantType>
inline void NodeTree<VariantType>::compile()
{
}

}

#endif // __NODETREE_H__