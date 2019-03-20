#include <AI\BT\Nodes/Decorator/successor.h>

#include <AI\BT\Nodes/node_enums.h>

CEREAL_REGISTER_TYPE(iga::bt::Successor)
CEREAL_REGISTER_POLYMORPHIC_RELATION(iga::bt::DecoratorNode, iga::bt::Successor)

namespace iga {

    namespace bt {

        Successor::Successor() { /*EMPTY*/ }
        Successor::~Successor() { /*EMPTY*/ }

        std::shared_ptr<BaseNode> Successor::CloneToShared() const { return std::make_shared<Successor>(*this); };

        BaseNode* Successor::Clone() const { return new Successor(*this); }

        NodeResult Successor::OnUpdate() {

            // Always return SUCCESS
            this->GetChild()->ExecuteNode();

            return NodeResult::SUCCESS;

        }

#ifdef EDITOR

        void Successor::ConstructEditorNode(iga::bte::EditorNode* a_editor_node) {
            a_editor_node->SetName(GET_VARIABLE_NAME(Successor));
            a_editor_node->SetType(iga::bte::NodeType::TREE);

            a_editor_node->AddInputPin(iga::bte::PinType::FLOW);
            a_editor_node->AddOutputPin(iga::bte::PinType::FLOW);
        }

#endif // EDITOR

    } // End of namespace ~ bt

} // End of namespace ~ iga
