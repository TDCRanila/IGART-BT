#include <AI\BT\Nodes/Decorator/inverter.h>

#include <AI\BT\Nodes/node_enums.h>

CEREAL_REGISTER_TYPE(iga::bt::Inverter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(iga::bt::DecoratorNode, iga::bt::Inverter)

namespace iga {

    namespace bt {

        Inverter::Inverter() { /*EMPTY*/ }
        Inverter::~Inverter() { /*EMPTY*/ }

        std::shared_ptr<BaseNode> Inverter::CloneToShared() const { return std::make_shared<Inverter>(*this); };

        BaseNode* Inverter::Clone() const { return new Inverter(*this); }

        NodeResult Inverter::OnUpdate() {

            NodeResult result = this->GetChild()->ExecuteNode();

            // Invert the result.
            if (result == NodeResult::SUCCESS) {
                return NodeResult::FAILED;
            }

            if (result == NodeResult::FAILED) {
                return NodeResult::SUCCESS;
            }

            // If the result isn't succesful nor failed, return the result.
            return result;

        }

#ifdef EDITOR

        void Inverter::ConstructEditorNode(iga::bte::EditorNode* a_editor_node) {
            a_editor_node->SetName(GET_VARIABLE_NAME(Inverter));
            a_editor_node->SetType(iga::bte::NodeType::TREE);

            a_editor_node->AddInputPin(iga::bte::PinType::FLOW);
            a_editor_node->AddOutputPin(iga::bte::PinType::FLOW);
        }

#endif // EDITOR

    } // End of namespace ~ bt

} // End of namespace ~ iga
