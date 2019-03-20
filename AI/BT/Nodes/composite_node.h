#pragma once

#include <AI\BT\Nodes\base_node.h>

#include <AI\BT\Nodes/node_enums.h>

#include <vector>

#ifdef EDITOR
#include <Editor/BTEditor/bt_editor_structs.h>
#endif // EDITOR

/**
* @file composite_node.h
* @brief iga::bt::CompositeNode Header File
*
* @author Channing Eggers
* @date 2 March 2018
* @version $Revision 1.1
*
* The class CompositeNode is a base class for Composite Nodes derived from BaseNode.
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
    
        class CompositeNode : public BaseNode {
        public:

            /** CompositeNode Constructor - Sets the node type for any derived classes from CompositeNode. */
            CompositeNode();
            
            /** CompositeNode Destructor */
            ~CompositeNode();

            /**
            *   Function adds a child to the vector of children for this composite node.
            *   !When adding childs, the order matters!
            *   @param a_child is pointer to an iga::bt::BaseNode.
            *   @see BaseNode
            */
            void AddChild(BaseNode* a_child);
            
            /** Returns a copy of the BaseNode* vector of this composite node. */
            std::vector<BaseNode*> GetChildren() const;

        private:

            std::vector<BaseNode*> children_;   /** Vector of BaseNode pointers which are the children of this node. */

        };

    } // End of namespace ~ bt;

} // End of namespcae ~ iga
