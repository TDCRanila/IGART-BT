#include <AI/BT/Nodes/Leaf/black_board_isvalid.h>

#include <iostream>

CEREAL_REGISTER_TYPE(iga::bt::BBIsValid)
CEREAL_REGISTER_POLYMORPHIC_RELATION(iga::bt::LeafNode, iga::bt::BBIsValid)

namespace iga {

    /**
    *  BT namespace
    */
    namespace bt {

        BBIsValid::BBIsValid() { 
            type_           = BlackBoardTypes::INT;
            variable_name_  = "";
        }

        BBIsValid::~BBIsValid() {/*EMPTY*/ }

        std::shared_ptr<BaseNode> BBIsValid::CloneToShared() const { return std::make_shared<BBIsValid>(*this); };

        BaseNode* BBIsValid::Clone() const { return new BBIsValid(*this); }

#ifdef EDITOR

        void BBIsValid::OnEditorLoad() { 
            memcpy(variable_name_buffer_, variable_name_.c_str(), variable_name_.size());
        }

        void BBIsValid::ConstructEditorNode(iga::bte::EditorNode* a_editor_node) {
            a_editor_node->SetName(GET_VARIABLE_NAME(BBIsValid));
            a_editor_node->SetType(iga::bte::NodeType::TREE);

            a_editor_node->AddInputPin(iga::bte::PinType::FLOW);
        }

        void BBIsValid::InspectNode() {

            // Variable Type
            ImGui::Text("Variable Type");
            static const char* list_items[] = { "Int", "Float", "Bool", "String" };
            static int current_setting;
            if      (type_ == BlackBoardTypes::INT)     { current_setting = 0; }
            else if (type_ == BlackBoardTypes::FLOAT)   { current_setting = 1; }
            else if (type_ == BlackBoardTypes::BOOL)    { current_setting = 2; }
            else if (type_ == BlackBoardTypes::STRING)  { current_setting = 2; }
            if (ImGui::ListBox("##VariableType", &current_setting, list_items, IM_ARRAYSIZE(list_items), 4)) {
                switch (current_setting) {
                case (0): { type_ = BlackBoardTypes::INT;    break; }
                case (1): { type_ = BlackBoardTypes::FLOAT;  break; }
                case (2): { type_ = BlackBoardTypes::BOOL;   break; }
                case (3): { type_ = BlackBoardTypes::STRING; break; }
                }
            }

            // Name
            ImGui::Text("Variable Name");
            if (ImGui::InputText("##VariableName", variable_name_buffer_, IM_ARRAYSIZE(variable_name_buffer_))) {
                variable_name_ = variable_name_buffer_;
            }

        }

#endif // EDITOR

        iga::bt::NodeResult BBIsValid::OnUpdate() {

            bool is_valid = false;

            switch (type_) {
                case(BlackBoardTypes::INT): {
                    int temp;
                    is_valid = GetBehaviourTree()->GetBlackBoard()->GetIntMemory(variable_name_, temp);
                    break;
                }
                case (BlackBoardTypes::FLOAT): {
                    float temp;
                    is_valid = GetBehaviourTree()->GetBlackBoard()->GetFloatMemory(variable_name_, temp);
                    break;
                }
                case (BlackBoardTypes::BOOL): {
                    bool temp;
                    is_valid = GetBehaviourTree()->GetBlackBoard()->GetBoolMemory(variable_name_, temp);
                    break;
                }
                case (BlackBoardTypes::STRING): {
                    std::string temp;
                    is_valid = GetBehaviourTree()->GetBlackBoard()->GetStringMemory(variable_name_, temp);
                    break;
                }
            }

            if (is_valid) {
                return iga::bt::NodeResult::SUCCESS;
            } else {
                return iga::bt::NodeResult::FAILED;
            }
        }

    } // End of namespace ~ bt

} // End of namespace ~ iga