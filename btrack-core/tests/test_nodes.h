#ifndef __TEST_NODES_H__
#define __TEST_NODES_H__


#include "nodes/system/nodes.h"
#include "nodes/utilities/math/UnaryOperation.h"
#include "nodes/system/shortcuts.h"
#include "nodes/runners/NodeRunnerProvider.h"


using namespace btrack::nodes::system;
using namespace btrack::nodes::runners;
using namespace btrack::nodes::utilities::math;

class DummyGraph : public NodeObserver
{
public:
    void update() {}
};

class DummyNode : public _Node
{
public:
    DummyNode() : _Node::_Node("", NodeItemType::UNKNOWN) {}
    size_t inputCount() const { return 1; }
    size_t outputCount() const { return 1; }

    // This observer does not have a parent observer, so we need to override the default update method.
    void update() override {}
};

auto dummy = std::make_shared<DummyNode>();

class ExecutionTracker
{
private:
    boost::mutex mMutex;
    std::vector<std::weak_ptr<_NodeItem>> mExecutionQueue;
public:
    void notifyExecuted(std::shared_ptr<_NodeItem> item)
    {
        boost::lock_guard<boost::mutex> lock(mMutex);
        mExecutionQueue.emplace_back(item);
    }

    std::vector<std::weak_ptr<_NodeItem>> order() { return mExecutionQueue; }
};

class TestNode : public Node
{
private:
    std::weak_ptr<ExecutionTracker> mTracker;
protected:
    TestNode(
        const std::string_view& _name, 
        const std::string_view& _friendlyName = "",
        const std::string_view& _description = ""
        ) : 
            TestNode::Node(_name, _friendlyName, _description) {}

public:
    void setTracker(std::shared_ptr<ExecutionTracker> tracker) { mTracker = tracker; }
    void process() override 
    {
        IF_WEAK_VALID(mTracker)->notifyExecuted(this->shared_from_this());
    }
};

class TestMetaNode : public MetaNode
{
protected:
    std::vector<std::shared_ptr<ExecutionTracker>> mTrackers;

    TestMetaNode(
        const std::string_view& _name, 
        const std::string_view& _friendlyName = "",
        const std::string_view& _description = ""
        ) : 
            TestMetaNode::MetaNode(_name, _friendlyName, _description) 
            {
                
            }

    template <std::derived_from<TestNode> NodeType>
    void testGenerateImpl(int count)
    {
        for (int i = 0; i != count; ++i)
        {
            auto t = std::make_shared<ExecutionTracker>();
            mTrackers.emplace_back(t);
            auto node = NodeType::create(this->asObserver());
            node->setTracker(t);
            IF_WEAK_VALID(mObserver)->addItem(node);
            for (int ii = 0; ii < this->inputCount(); ++ii)
            {
                this->_MetaInputAt(ii)->attach(node->_InputAt(ii));
            }
            for (int ii = 0; ii < this->outputCount(); ++ii)
            {
                this->_MetaOutputAt(ii)->attach(node->_OutputAt(ii));
            }
        }
    }

};

class NodeStart : public TestNode
{
private:
    struct Protected { explicit Protected() = default; };
public:
    std::shared_ptr<OutputValue<int>> valueOutput;
    std::shared_ptr<OutputValue<int>> arrayOutput;

    NodeStart(Protected _) : NodeStart::TestNode("nodeStart") { }

    static std::shared_ptr<NodeStart> create(std::shared_ptr<NodeObserver> _observer)
    {
        auto ret = std::make_shared<NodeStart>(Protected());
        ret->valueOutput = ret->addOutputValue<int>("valueOutput");
        ret->arrayOutput = ret->addOutputValue<int>("arrayOutput");
        ret->mObserver = _observer;
        return ret;
    }

    size_t inputCount() const override { return 0; }
    size_t outputCount() const override { return 2; }

};

class TestNodeStart : public TestMetaNode
{
private:
    struct Protected { explicit Protected() = default; };
public:
    std::shared_ptr<MetaOutputValue<int>> valueOutput;
    std::shared_ptr<MetaOutputArray<int>> arrayOutput;
    TestNodeStart(Protected _,
        const std::string_view& _name, 
        const std::string_view& _friendlyName = "",
        const std::string_view& _description = ""
        ) : 
            TestNodeStart::TestMetaNode(_name, _friendlyName, _description) 
            {
                
            }

    static std::shared_ptr<TestNodeStart> create(std::shared_ptr<NodeObserver> _observer)
    {
        auto ret = std::make_shared<TestNodeStart>(Protected(), "");
        ret->valueOutput = ret->addOutputValue<int>("valueOutput");
        ret->arrayOutput = ret->addOutputArray<int>("arrayOutput");
        ret->mObserver = _observer;
        return ret;
    }

    void generate(int count) override 
    {
        this->testGenerateImpl<NodeStart>(count);
    }

    void start(int val, std::vector<int> arr)
    {
        generate(arr.size())
        for (auto v : arr)
        {
            
        }
    }

    size_t inputCount() const override { return 0; }
    size_t outputCount() const override { return 2; }
};

class NodeStartFloat : public TestNode
{
private:
    struct Protected { explicit Protected() = default; };
public:
    std::shared_ptr<OutputValue<float>> valueOutput;
    std::shared_ptr<OutputValue<float>> arrayOutput;

    NodeStartFloat(Protected _) : NodeStartFloat::TestNode("nodeStartFloat")
    {
        
    }

    static std::shared_ptr<NodeStartFloat> create(std::shared_ptr<NodeObserver> _observer)
    {
        auto ret = std::make_shared<NodeStartFloat>(Protected());
        ret->valueOutput = ret->addOutputValue<float>("valueOutput");
        ret->arrayOutput = ret->addOutputValue<float>("arrayOutput");
        ret->mObserver = _observer;
        return ret;
    }
    size_t inputCount() const override { return 0; }
    size_t outputCount() const override { return 2; }

};

class TestNodeStartFloat : public TestMetaNode
{
private:
    struct Protected { explicit Protected() = default; };
public:
    std::shared_ptr<MetaOutputValue<float>> valueOutput;
    std::shared_ptr<MetaOutputArray<float>> arrayOutput;
    TestNodeStartFloat(
        Protected _,
        const std::string_view& _name, 
        const std::string_view& _friendlyName = "",
        const std::string_view& _description = ""
        ) : 
            TestNodeStartFloat::TestMetaNode(_name, _friendlyName, _description) 
            {
                
            }

    static std::shared_ptr<TestNodeStartFloat> create(std::shared_ptr<NodeObserver> _observer)
    {
        auto ret = std::make_shared<TestNodeStartFloat>(Protected(), "");
        ret->valueOutput = ret->addOutputValue<float>("valueOutput");
        ret->arrayOutput = ret->addOutputArray<float>("arrayOutput");
        ret->mObserver = _observer;
        return ret;
    }

    void generate(int count) override
    {
        this->testGenerateImpl<NodeStartFloat>(count);
    }

    size_t inputCount() const override { return 0; }
    size_t outputCount() const override { return 2; }
};


class NodeMid1 : public TestNode
{
private:
    struct Protected { explicit Protected() = default; };
public:
    std::shared_ptr<InputValue<int>> valueInput;
    std::shared_ptr<InputValue<int>> arrayInput;
    std::shared_ptr<OutputValue<int>> valueOutput;
    std::shared_ptr<OutputValue<int>> arrayOutput;

    NodeMid1(Protected _) : NodeMid1::TestNode("nodeMid1") { }

    static std::shared_ptr<NodeMid1> create(std::shared_ptr<NodeObserver> _observer)
    {
        auto ret = std::make_shared<NodeMid1>(Protected());
        ret->valueInput = ret->addInputValue<int>("valueInput");
        ret->arrayInput = ret->addInputValue<int>("arrayInput");
        ret->valueOutput = ret->addOutputValue<int>("valueOutput");
        ret->arrayOutput = ret->addOutputValue<int>("arrayOutput");
        ret->mObserver = _observer;
        return ret;
    }

    size_t inputCount() const override { return 2; }
    size_t outputCount() const override { return 2; }

};

class TestNodeMid1 : public TestMetaNode
{
private:
    struct Protected { explicit Protected() = default; };
public:
    std::shared_ptr<MetaInputValue<int>> valueInput;
    std::shared_ptr<MetaInputArray<int>> arrayInput;
    
    std::shared_ptr<MetaOutputValue<int>> valueOutput;
    std::shared_ptr<MetaOutputArray<int>> arrayOutput;

    TestNodeMid1(
        Protected _,
        const std::string_view& _name, 
        const std::string_view& _friendlyName = "",
        const std::string_view& _description = ""
        ) : 
            TestNodeMid1::TestMetaNode(_name, _friendlyName, _description) 
            {
                
            }
    
    static std::shared_ptr<TestNodeMid1> create(std::shared_ptr<NodeObserver> _observer)
    {
        auto ret = std::make_shared<TestNodeMid1>(Protected(), "");
        ret->valueInput = ret->addInputValue<int>("valueInput");
        ret->arrayInput = ret->addInputArray<int>("arrayInput");
        ret->valueOutput = ret->addOutputValue<int>("valueOutput");
        ret->arrayOutput = ret->addOutputArray<int>("arrayOutput");
        ret->mObserver = _observer;
        return ret;
    }

    void generate(int count) override
    {
        this->testGenerateImpl<NodeMid1>(count);
    }

    size_t inputCount() const override { return 2; }
    size_t outputCount() const override { return 2; }
};

class NodeEnd : public TestNode
{
private:
    struct Protected { explicit Protected() = default; };
public:
    std::shared_ptr<InputValue<int>> valueInput;
    std::shared_ptr<InputValue<int>> arrayInput;

    NodeEnd(Protected _) : NodeEnd::TestNode("nodeEnd") { }

    static std::shared_ptr<NodeEnd> create(std::shared_ptr<NodeObserver> _observer)
    {
        auto ret = std::make_shared<NodeEnd>(Protected());
        ret->valueInput = ret->addInputValue<int>("valueInput");
        ret->arrayInput = ret->addInputValue<int>("arrayInput");
        ret->mObserver = _observer;
        return ret;
    }

    size_t inputCount() const override { return 0; }
    size_t outputCount() const override { return 2; }

};

class TestNodeEnd : public TestMetaNode
{
private:
    struct Protected { explicit Protected() = default; };
public:
    TestNodeEnd(
        Protected _,
        const std::string_view& _name, 
        const std::string_view& _friendlyName = "",
        const std::string_view& _description = ""
        ) : 
            TestNodeEnd::TestMetaNode(_name, _friendlyName, _description) 
            {
                
            }
    std::shared_ptr<MetaInputValue<int>> valueInput;
    std::shared_ptr<MetaInputArray<int>> arrayInput;

    static std::shared_ptr<TestNodeEnd> create(std::shared_ptr<NodeObserver> _observer)
    {
        auto ret = std::make_shared<TestNodeEnd>(Protected(), "");
        ret->valueInput = ret->addInputValue<int>("valueInput");
        ret->arrayInput = ret->addInputArray<int>("arrayInput");
        ret->mObserver = _observer;
        return ret;
    }

    void generate(int count) override
    {
        this->testGenerateImpl<NodeEnd>(count);
    }

    size_t inputCount() const override { return 2; }
    size_t outputCount() const override { return 0; }
};

#endif // __TEST_NODES_H__