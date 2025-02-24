#ifndef __NODETREE_H__
#define __NODETREE_H__

#include "nodes/system/Node.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/iterator/zip_iterator.hpp>

namespace btrack::nodes::system {

using namespace boost;

template <VariantTemplate VariantType>
class NodeTree : public Node<VariantType>
{
protected:
    boost::container::vector<NodeBase<VariantType>*> mNodes;
    boost::container::vector<int> mOrder;
    adjacency_list<vecS, vecS, directedS> mGraph;
    
    void compile0();
    void compile1();
    void compile2();

public:
    NodeTree() = default;
    NodeTree(const std::string& _name, const std::string& _category, const std::string& _description = "")
        : NodeTree::Node(_name, _category, _description) {}
    template <size_t OPTIMIZATION>
    void compile();
    void compile() override { compile<2>(); }
    void run() override;
    bool isNodeTree() const override { return true; }

    NodeBase<VariantType>* addNode(NodeBase<VariantType>* node);
    template <std::derived_from<NodeBase<VariantType>> NodeType, typename ...TArgs>
    NodeType* addNode(TArgs&&... args);
    bool removeNode(NodeBase<VariantType>* node);
    Item *at(const ID_e &_id, const bool& port = false) override;
    const Item *at(const ID_e &_id, const bool& port = false) const override;

    void clone(Item* to) const override;
    Item* createClone() const override;
    void reserve(size_t count) { mNodes.reserve(count); }

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
    }
    for (NodeBase<VariantType>* n : mNodes)
    {
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
    }
    for (NodeBase<VariantType>* n : mNodes)
    {
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
    }
    for (NodeBase<VariantType>* n : mNodes)
    {
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
inline NodeBase<VariantType>* NodeTree<VariantType>::addNode(NodeBase<VariantType> *node)
{
    mNodes.emplace_back(node);
    return node;
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
inline bool NodeTree<VariantType>::removeNode(NodeBase<VariantType> *node)
{
    if (!node) return false;
    if (auto it = std::find(mNodes.begin(), mNodes.end(), node); it != mNodes.end())
    {
        delete *it;
        mNodes.erase(it);
        return true;
    }
    return false;
}

template <VariantTemplate VariantType>
inline Item *NodeTree<VariantType>::at(const ID_e &_id, const bool &port)
{
    if (port) return Node<VariantType>::at(_id, port);
    if (_id >= mNodes.size()) return nullptr;
    return mNodes.at(_id);
}

template <VariantTemplate VariantType>
inline const Item *NodeTree<VariantType>::at(const ID_e &_id, const bool &port) const
{
    if (port) return Node<VariantType>::at(_id, port);
    if (_id >= mNodes.size()) return nullptr;
    return mNodes.at(_id);
}

template <VariantTemplate VariantType>
inline void NodeTree<VariantType>::clone(Item *to) const
{
    // typedef boost::tuple<typename NodeBase<VariantType>::const_port_iterator, typename NodeBase<VariantType>::port_iterator> _tuple_t;
    if (!to) return;
    Node<VariantType>::clone(to);
    if (NodeTree<VariantType>* res = dynamic_cast<NodeTree<VariantType>*>(to); res)
    {
        // Clone local nodes
        for (const NodeBase<VariantType>* localNode : mNodes)
        {
            NodeBase<VariantType>* clonedNode = dynamic_cast<NodeBase<VariantType>*>(localNode->createClone());
            res->addNode(clonedNode);
        }

        for (size_t i = 0; i < mNodes.size(); ++i)
        {
            const NodeBase<VariantType>* localNode = mNodes.at(i);
            NodeBase<VariantType>* clonedNode = res->mNodes.at(i);
            // TODO Implement passthrough ports

            // Connect new ports
            auto b = boost::make_zip_iterator(boost::make_tuple(localNode->pbegin(), clonedNode->_pbegin()));
            auto e = boost::make_zip_iterator(boost::make_tuple(localNode->pend(), clonedNode->_pend()));
            for (auto pair = b; pair != e; ++pair)
            {
                const PortBase<VariantType>* localPort = boost::get<0>(*pair);
                PortBase<VariantType>* clonedPort = boost::get<1>(*pair);

                for (ID_e ii = 0; ii < localPort->connectionCount(); ++ii)
                {
                    const PortBase<VariantType>* localConnection = localPort->connectionAt(ii);
                    if (!localConnection) continue;
                    // Find equivalent connection for the cloned port
                    PortBase<VariantType>* clonedConnection = (dynamic_cast<PortBase<VariantType>*>((dynamic_cast<NodeBase<VariantType>*>(res->at(localConnection->parent()->id(), false)))->at(localConnection->id(), true)));

                    clonedPort->connect(clonedConnection);
                }
            }
        }
    }
    
}

template <VariantTemplate VariantType>
inline Item* NodeTree<VariantType>::createClone() const
{
    auto res = new NodeTree<VariantType>();
    this->clone(res);
    return res;
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