#include <AI/BT/Nodes/Leaf/black_board_set_int.h>

#include <iostream>

CEREAL_REGISTER_TYPE(iga::bt::BBSetInt)
CEREAL_REGISTER_POLYMORPHIC_RELATION(iga::bt::LeafNode, iga::bt::BBSetInt)

namespace iga {

    /**
    *  BT namespace
    */
    namespace bt {

        BBSetInt::BBSetInt() { /*EMPTY*/ }

        BBSetInt::~BBSetInt() {/*EMPTY*/ }

        std::shared_ptr<BaseNode> BBSetInt::CloneToShared() const { return std::make_shared<BBSetInt>(*this); };

        BaseNode* BBSetInt::Clone() const { return new BBSetInt(*this); }

        void BBSetInt::OnBehaviourTreeBuild() {
            //GetBehaviourTree()->GetBlackBoard()->SetGlobalIntMemory(variable_name_, variable_value_);
        }

#ifdef EDITOR

        void BBSetInt::OnEditorLoad() {
            memcpy(variable_name_buffer_, variable_name_.c_str(), variable_name_.size());
        }

        void BBSetInt::ConstructEditorNode(iga::bte::EditorNode* a_editor_node) {
            a_editor_node->SetName(GET_VARIABLE_NAME(BBSetInt));
            a_editor_node->SetType(iga::bte::NodeType::TREE);

            a_editor_node->AddInputPin(iga::bte::PinType::FLOW);
        }

        void BBSetInt::InspectNode() {
            // Name
            ImGui::Text("Variable Name");
            if (ImGui::InputText("##VariableName", variable_name_buffer_, IM_ARRAYSIZE(variable_name_buffer_))) {
                variable_name_ = variable_name_buffer_;
            }

            // Value
            ImGui::InputInt("##Variable Value", &variable_value_);

        }

#endif // EDITOR

        iga::bt::NodeResult BBSetInt::OnUpdate() {
            GetBehaviourTree()->GetBlackBoard()->SetGlobalIntMemory(variable_name_, variable_value_);

            return iga::bt::NodeResult::SUCCESS;
        }

    } // End of namespace ~ bt

} // End of namespace ~ iga