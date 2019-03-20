#pragma once

#include <AI\BT\Nodes\base_node.h>

#include <AI\BT\Nodes/node_enums.h>

#ifdef EDITOR
#include <Editor/BTEditor/bt_editor_structs.h>
#endif // EDITOR

/**
* @file decorator_node.h
* @brief iga::bt::DecoratorNode Header File
*
* @author Channing Eggers
* @date 2 March 2018
* @version $Revision 1.1
*
* The class DecoratorNode is a base class for Decorator Nodes derived from BaseNode.
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

        class BaseNode;

        class DecoratorNode : public BaseNode {
        public:

            /** DecoratorNode Constructor - Sets the node type of any derived classes from Decorator. */
            DecoratorNode();

            /** DecoratorNode Destructor */
            ~DecoratorNode();

            /**
            *   Sets the child of this node.
            *   @param a_child is a pointer to a BaseNode.
            */
            void SetChild(BaseNode* a_child);

            /** Returns the child of this node which is a pointer to a BaseNode. */
            BaseNode* GetChild() const;

        private:

            BaseNode* child_; /** BaseNode pointer to the child of this node. */

        };

    } // End of namespace ~ bt;

} // End of namespcae ~ iga
