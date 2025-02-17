#ifndef __NODETREE_H__
#define __NODETREE_H__

#include "nodes/system/Node.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

namespace btrack::nodes::system {

using namespace boost;

template <VariantTemplate VariantType>
class NodeTree : public Clonable<NodeTree<VariantType>>
{
protected:
    boost::container::vector<NodeBase<VariantType>*> mNodes;
    boost::container::vector<int> mOrder;
    adjacency_list<vecS, vecS, directedS> mGraph;
    
    void compile0();
    void compile1();
    void compile2();

public:
    template <size_t OPTIMIZATION = 2>
    void compile();
    void run();

    template <std::derived_from<NodeBase<VariantType>> NodeType, typename ...TArgs>
    NodeType* addNode(TArgs&&... args);

    void clone(NodeTree<VariantType>* to) const override;

    virtual ~NodeTree();
};
template <VariantTemplate VariantType>
template <size_t OPTIMIZATION>
inline void NodeTree<VariantType>::compile()
{
    if constexpr (OPTIMIZATION == 0) { compile0(); }
    if constexpr (OPTIMIZATION == 1) { compile1(); }
    if constexpr (OPTIMIZATION == 2) { compile2(); }
}
template <VariantTemplate VariantType>
inline void NodeTree<VariantType>::compile0()
{
    mGraph = adjacency_list<vecS, vecS, directedS>(mNodes.size() + 1);
    mOrder.clear();
    int i = 0;
    for (NodeBase<VariantType>* n : mNodes)
    {
        n->compile();
        auto it = ((const NodeBase<VariantType>*)n)->recursiveIterator(0);
        while (it.hasNext())
        {
            const Output<VariantType>* out = dynamic_cast<const Output<VariantType>*>(it.next());
            if (!out) continue;
            for (const PortBase<VariantType>* p : out->connections())
            {
                auto parent = std::find(mNodes.begin(), mNodes.end(), p->parent());
                if (parent == mNodes.end()) continue;
                add_edge(i + 1, parent - mNodes.begin() + 1, mGraph);
            }
        }
        if (n->isForced()) add_edge(i + 1, 0, mGraph);
        ++i;
    }
    topological_sort(mGraph, std::back_inserter(mOrder));
}
template <VariantTemplate VariantType>
inline void NodeTree<VariantType>::compile1()
{
    mGraph = adjacency_list<vecS, vecS, directedS>(mNodes.size() + 1);
    mOrder.clear();
    mOrder.reserve(mNodes.size() + 1);
    int i = 0;
    for (NodeBase<VariantType>* n : mNodes)
    {
        n->compile();
        for (auto it = ((const NodeBase<VariantType>*)n)->pbegin(); it != n->pend(); ++it)
        {
            const Output<VariantType>* out = dynamic_cast<const Output<VariantType>*>(*it);
            if (!out) continue;
            for (const PortBase<VariantType>* p : out->connections())
            {
                auto parent = std::find(mNodes.begin(), mNodes.end(), p->parent());
                if (parent == mNodes.end()) continue;
                add_edge(i + 1, parent - mNodes.begin() + 1, mGraph);
            }
        }
        if (n->isForced()) add_edge(i + 1, 0, mGraph);
        ++i;
    }
    topological_sort(mGraph, std::back_inserter(mOrder));
}
template <VariantTemplate VariantType>
inline void NodeTree<VariantType>::compile2()
{
    mGraph = adjacency_list<vecS, vecS, directedS>(mNodes.size() + 1);
    mOrder.clear();
    mOrder.reserve(mNodes.size() + 1);
    int i = 0;
    for (NodeBase<VariantType>* n : mNodes)
    {
        n->compile();
        for (auto it = ((const NodeBase<VariantType>*)n)->pbegin(); it != n->pend(); ++it)
        {
            const Output<VariantType>* out = dynamic_cast<const Output<VariantType>*>(*it);
            if (!out) continue;
            for (const PortBase<VariantType>* p : out->connections())
            {
                add_edge(i + 1, p->parent()->id() + 1, mGraph);
            }
        }
        if (n->isForced()) add_edge(i + 1, 0, mGraph);
        ++i;
    }
    topological_sort(mGraph, std::back_inserter(mOrder));
}
template <VariantTemplate VariantType>
inline void NodeTree<VariantType>::run()
{
    for (auto i = mOrder.rbegin(); i != mOrder.rend(); ++i)
    {
        if (*i == 0) continue;
        mNodes.at(*i - 1)->run();
    }
}

template <VariantTemplate VariantType>
inline void NodeTree<VariantType>::clone(NodeTree<VariantType> *to) const
{
}

template <VariantTemplate VariantType>
template <std::derived_from<NodeBase<VariantType>> NodeType, typename... TArgs>
inline NodeType *NodeTree<VariantType>::addNode(TArgs &&...args)
{
    typedef typename std::remove_cv<NodeType>::type _Tp_nc;
    auto res = dynamic_cast<NodeType*>(mNodes.emplace_back(new _Tp_nc(forward<TArgs>(args)...)));
    NodeRegister::id(*res) = mNodes.size() - 1;
    return res;
}
template <VariantTemplate VariantType>
inline NodeTree<VariantType>::~NodeTree()
{
    for (NodeBase<VariantType>* n : mNodes)
    {
        delete n;
    }
}
}

#endif // __NODETREE_H__