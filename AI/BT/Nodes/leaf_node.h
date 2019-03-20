#pragma once

#include <AI\BT\Nodes\base_node.h>

#include <AI\BT\Nodes/node_enums.h>

#ifdef EDITOR
#include <Editor/BTEditor/bt_editor_structs.h>
#endif // EDITOR

/**
* @file leaf_node.h
* @brief iga::bt::LeafNode Header File
*
* @author Channing Eggers
* @date 2 March 2018
* @version $Revision 1.1
*
* The class LeafNode is a base class for Leaf Nodes derived from BaseNode.
* @see BaseNode
*
*/

/**
*  igart namespace
*/
namespace iga {

    /**
    *  BT namespace
    */
    namespace bt {

        class LeafNode : public BaseNode {
        public:

            /** LeafNode Constructor - Sets the node type for any derived classes from LeafNode. */
            LeafNode();

            /** LeafNode Destructor */
            ~LeafNode();

        };

    } // End of namespace ~ bt;

} // End of namespcae ~ iga