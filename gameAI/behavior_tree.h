#pragma once
#include <memory>
#include "common.h"

const std::string ATTACK = "attack2";
const std::string DEFEND = "defend2";

class BehaviorNode
{
public:
    typedef std::shared_ptr<BehaviorNode> ptr;

    explicit BehaviorNode(std::string name) { m_name = std::move(name); }

    virtual ~BehaviorNode() = default;

    virtual bool execute() = 0;

    virtual void add_children(const BehaviorNode::ptr& child_ptr)
    {
        m_children.push_back(child_ptr);
    }

    virtual void add_children(std::initializer_list<BehaviorNode::ptr> child_nodes)
    {
        for (const auto& child : child_nodes)
        {
            m_children.push_back(child);
        }
    }

    virtual bool can_execute() { return true; }

    std::string get_name() { return m_name; }

private:
    std::string m_name;

protected:
    std::vector<BehaviorNode::ptr> m_children;
};

class ActionNode : public BehaviorNode
{
public:
    explicit ActionNode(const std::string name) : BehaviorNode::BehaviorNode(name) { }

    ~ActionNode() override = default;

    bool execute() override
    {
        std::cout << BehaviorNode::get_name() << " is_running" << std::endl;
        return true;
    }

    bool can_execute() override
    {
        std::string name = BehaviorNode::get_name();
        if (name == ATTACK or name == DEFEND)
            return true;
        return false;
    }
};

class SequenceNode : public BehaviorNode
{
public:
    explicit SequenceNode(const std::string name) : BehaviorNode(name) {}
    ~SequenceNode() override = default;

    bool execute() override
    {
        for (auto& child : m_children)
        {
            if (!child->can_execute() || !child->execute())
                return false;
        }
        return true;
    }
};

class SelectorNode : public BehaviorNode
{
public:
    explicit SelectorNode(const std::string name) : BehaviorNode(name) {}

    ~SelectorNode() override = default;

    bool execute() override
    {
        for (auto& child : m_children)
        {
            if (child->can_execute() && child->execute())
            {
                return true;
            }
        }
        return false;
    }
};

class ParallelNode : public BehaviorNode
{
public:
    explicit ParallelNode(const std::string name) : BehaviorNode(name) {}

    ~ParallelNode() override = default;

    bool execute() override
    {
        for (auto& child : m_children)
            child->execute();
        return true;
    }
};
