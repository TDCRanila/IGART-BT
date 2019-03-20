#pragma once

#include <AI\BT\Nodes\decorator_node.h>

/**
* @file successor.h
* @brief iga::bt::Sucessor
*
* @author Channing Eggers
* @date 4 March 2018
* @version $Revision 1.2
*
* Successor class is derived from DecoratorNode.
* The Succesor node will always return SUCCESS,
* no matter the result of the child.
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

        class Successor final : public DecoratorNode {
        public:

            /** Successor Constructor */
            Successor();

            /** Successor Destructor */
            ~Successor();

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
            *   the successor node gets updated.
            *   @see BaseNode
            *   @return NodeResult SUCCESS/RUNNING/FAILED/ERROR.
            */
            NodeResult OnUpdate();

        };

        SUBSCRIBE_NODE( Successor )

    } // End of namespace ~ bt

} // End of namespace ~ iga
