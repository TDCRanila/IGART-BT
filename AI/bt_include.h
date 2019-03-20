#pragma once

/**
*   @file bt_include.h
*   @author Channing Eggers
*   @date 6 March 2018
*   @brief Global Include File for the Behaviour Tree System
*
*   @section DESCRIPTION
*   File contains all includes to the behaviour tree system. Including this 
*   file will give you access the system. There is no need to indivually 
*   include them.
*
*/

#include <AI\BT\behaviour_tree.h>
#include <AI\BT\black_board.h>
#include <AI\BT\behaviour_component.h>

#include <AI\BT\Nodes\base_node.h>
#include <AI\BT\Nodes\node_enums.h>

#include <AI\BT\Nodes\composite_node.h>
#include <AI\BT\Nodes\decorator_node.h>
#include <AI\BT\Nodes\leaf_node.h>

#include <AI\BT\Nodes\Decorator\root.h>

#include <AI\BT\Nodes\Leaf\debug_message.h>

#include <AI\BT\Nodes\Composite\sequence.h>
#include <AI\BT\Nodes\Composite\selector.h>

#include <AI\BT\Nodes\Decorator\inverter.h>
#include <AI\BT\Nodes\Decorator\successor.h>
#include <AI\BT\Nodes\Decorator\repeater.h>
