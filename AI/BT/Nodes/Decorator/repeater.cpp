#include <AI\BT\Nodes/Decorator/repeater.h>

#include <AI\BT\Nodes/node_enums.h>

CEREAL_REGISTER_TYPE(iga::bt::Repeater)
CEREAL_REGISTER_POLYMORPHIC_RELATION(iga::bt::DecoratorNode, iga::bt::Repeater)

namespace iga {

    namespace bt {

        Repeater::Repeater() { /*EMPTY*/ }
        Repeater::~Repeater() { /*EMPTY*/ }

        std::shared_ptr<BaseNode> Repeater::CloneToShared() const { return std::make_shared<Repeater>(*this); };

        BaseNode* Repeater::Clone() const { return new Repeater(*this); }

        void Repeater::SetMaxRepeaterCount(int a_max_count) {
            this->repeater_count_max_ = a_max_count;
        }

        NodeResult Repeater::OnUpdate() {

            int& count = this->repeater_count_;
            for (count; count < this->repeater_count_max_; ++count) {

                // Keep executing child node until repeater max count has been reached
                // or if the child node doesn't return success.
                NodeResult result = this->GetChild()->ExecuteNode();

                if (result != NodeResult::SUCCESS) {
                    count = 0;
                    return result;
                }

            }

            // Reset repeater count and return success.
            count = 0;
            return NodeResult::SUCCESS;

        }

#ifdef EDITOR

        void Repeater::ConstructEditorNode(iga::bte::EditorNode* a_editor_node) {
            a_editor_node->SetName(GET_VARIABLE_NAME(Repeater));
            a_editor_node->SetType(iga::bte::NodeType::TREE);

            a_editor_node->AddInputPin(iga::bte::PinType::FLOW);
            a_editor_node->AddOutputPin(iga::bte::PinType::FLOW);
        }

        void Repeater::InspectNode() {
            ImGui::InputInt("Repeater Count:", &this->repeater_count_max_);
        }

#endif // EDITOR

    } // End of namespace ~ bt

} // End of namespace ~ iga
