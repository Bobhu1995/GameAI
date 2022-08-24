#include <iostream>
#include <time.h>
#include "bsp_tree.h"
#include "fsm.h"
#include "light_switch.h"
#include "behavior_tree.h"
using namespace std;

MY_COLOR get_color(MY_COLOR color)
{
    return color;
}

void test_bsp_tree()
{
    BspTree<double> bsp_tree;

    vector<Flat<double>> flat_vecs;
    srand((int)time(0));
    for (int i = 0; i < 16; ++i)
    {
        Flat<double> flat;
        for (int j = 0; j < FLAT_POINT; j++)
        {
            flat.point[j] = Point<double>(-200.00 + rand() % 400, -200.00 + rand() % 400, -200.00 + rand() % 400);
        }
        flat_vecs.push_back(flat);
    }

    bsp_tree.InitBspTree(flat_vecs, Point<double>(-200, -200, -200), Point<double>(200, 200, 200), 5);
    bsp_tree.TraversBspTree();
    bsp_tree.DeleteBspTree();
}

void test_behavior_tree()
{
    BehaviorNode::ptr root = std::make_shared<SelectorNode>("root");
    BehaviorNode::ptr attack_select_node = std::make_shared<SelectorNode>("attack_select");
    BehaviorNode::ptr defend_select_node = std::make_shared<SelectorNode>("defend_select");

    BehaviorNode::ptr attact_node_1 = std::make_shared<ActionNode>("attack1");
    BehaviorNode::ptr attact_node_2 = std::make_shared<ActionNode>("attack2");

    BehaviorNode::ptr defend_node_1 = std::make_shared<ActionNode>("defend1");
    BehaviorNode::ptr defend_node_2 = std::make_shared<ActionNode>("defend2");

    root->add_children({ attack_select_node, defend_select_node });
    attack_select_node->add_children({ attact_node_1, attact_node_2 });
    defend_select_node->add_children({ defend_node_1 , defend_node_2 });

    root->execute();
}

int main()
{
    test_behavior_tree();

    init();
    while (true)
    {
        loop();
    }
    test_bsp_tree();
    system("pause");
    //std::cout << get_color(MY_COLOR::BLACK) << std::endl;
}