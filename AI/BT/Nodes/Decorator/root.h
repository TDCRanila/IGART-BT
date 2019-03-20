#pragma once

#include <AI\BT\Nodes\decorator_node.h>

/**
* @file inverter.h
* @brief iga::bt::Root Header File
*
* @author Channing Eggers
* @date 4 March 2018
* @version $Revision 1.2
*
* Inverter class is derived from DecoratorNode.
* The Inverter Node inverts SUCCESS to FAILED and
* vice versa.
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

        class Root final : public DecoratorNode {
        public:

            /** Root Constructor */
            Root();

            /** Root Destructor */
            ~Root();

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
            *   the inverter node gets updated.
            *   @see BaseNode
            *   @return NodeResult SUCCESS/RUNNING/FAILED/ERROR.
            */
            NodeResult OnUpdate();

        };

        SUBSCRIBE_NODE(Root)

    } // End of namespace ~ bt

} // End of namespace ~ iga
