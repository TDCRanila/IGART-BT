#pragma once

#include <AI\BT\Nodes\composite_node.h>

/**
* @file selector.h
* @brief iga::bt::Selector Header File
*
* @author Channing Eggers
* @date 4 March 2018
* @version $Revision 1.2
*
* Selector class is derived from CompositeNode.
*
* The Selector node will keep updating its children
* until one of them doesn't return FAILED.
* The result of that node is then returned.

* If all children FAILED, the selector node will return FAILED.
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

        class Selector final : public CompositeNode {
        public:

            /** Selector Constructor */
            Selector();

            /** Selector Destructor */
            ~Selector();

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
            *   the selector node gets updated.
            *   @see BaseNode 
            *   @return NodeResult SUCCESS/RUNNING/FAILED/ERROR.
            */
            NodeResult OnUpdate();

        };

        SUBSCRIBE_NODE( Selector )

    } // End of namespace ~ bt

} // End of namespace ~ iga
