#pragma once

#include <AI\BT\Nodes\composite_node.h>

/**
* @file sequence.h
* @brief iga::bt::Sequence Header File
*
* @author Channing Eggers
* @date 4 March 2018
* @version $Revision 1.2
*
* Sequence class is derived from CompositeNode.
*
* This node will return SUCCESS when all of its children
* nodes have updated and have returned SUCCESS. If a child doesn't
* return success, the sequence node will stop updating 
* and will return the same state as the child.
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

        enum class NodeResult;

        class Sequence final : public CompositeNode {
        public:
            
            /** Sequence Constructor */
            Sequence();

            /** Sequence Destructor */
            ~Sequence();

            /**
            *   Virtual Function that is used when polymorphically cloning this object when a shared_ptr needs to be returned.
            *   @return std::shared_ptr<BaseNode> Pointer to the newly cloned object.
            */
            virtual std::shared_ptr<BaseNode> CloneToShared() const override;

            /**
            *   Virtual Function that is used when polymorphically cloning this object when a raw pointer needs to be returned.
            *   @return BaseNode* Pointer to the newly cloned object.
            */
            virtual BaseNode* Clone() const override;

        private:

#ifdef EDITOR

            /**
            *   Overriden function that creates and returns an editor node.
            *   @param EditorNode a_editor_node A pointer to a newly constructed editor node.
            */
            void ConstructEditorNode(iga::bte::EditorNode* a_editor_node) override;

#endif // EDITOR

            /** OnUpdate function overriden from BaseNode class. Called when
            *   the sequence node gets updated.
            *   @see BaseNode
            *   @return NodeResult SUCCESS/RUNNING/FAILED/ERROR.
            */
            NodeResult OnUpdate();

        };

        SUBSCRIBE_NODE( Sequence )

    } // End of namespace ~ bt

} // End of namespace ~ iga
