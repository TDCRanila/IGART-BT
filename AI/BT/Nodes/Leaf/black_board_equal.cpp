#include <AI/BT/Nodes/Leaf/black_board_equal.h>

#include <iostream>

CEREAL_REGISTER_TYPE(iga::bt::BBCompare)
CEREAL_REGISTER_POLYMORPHIC_RELATION(iga::bt::LeafNode, iga::bt::BBCompare)

namespace iga {

    /**
    *  BT namespace
    */
    namespace bt {

        BBCompare::BBCompare() {
            type_ = BlackBoardTypes::INT;
            variable_name_ = "";
        }

        BBCompare::~BBCompare() {/*EMPTY*/ }

        std::shared_ptr<BaseNode> BBCompare::CloneToShared() const { return std::make_shared<BBCompare>(*this); };

        BaseNode* BBCompare::Clone() const { return new BBCompare(*this); }

#ifdef EDITOR

        void BBCompare::OnEditorLoad() {
            memcpy(variable_name_buffer_, variable_name_.c_str(), variable_name_.size());
            memcpy(variable_value_buffer_, variable_value_string_.c_str(), variable_value_string_.size());
        }

        void BBCompare::ConstructEditorNode(iga::bte::EditorNode* a_editor_node) {
            a_editor_node->SetName(GET_VARIABLE_NAME(BBCompare));
            a_editor_node->SetType(iga::bte::NodeType::TREE);

            a_editor_node->AddInputPin(iga::bte::PinType::FLOW);
        }

        void BBCompare::InspectNode() {

            // Variable Type
            ImGui::Text("Variable Type");
            static const char* list_items[] = { "Int", "Float", "Bool", "String" };
            static int current_setting;
            if (type_ == BlackBoardTypes::INT) { current_setting = 0; }
            else if (type_ == BlackBoardTypes::FLOAT) { current_setting = 1; }
            else if (type_ == BlackBoardTypes::BOOL) { current_setting = 2; }
            else if (type_ == BlackBoardTypes::STRING) { current_setting = 2; }
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

            ImGui::Text("Variable Value");
            // Value
            switch (type_) {
            case(BlackBoardTypes::INT): {
                ImGui::InputInt("##VariableNameInt", &variable_value_int_);
                break;
            }
            case (BlackBoardTypes::FLOAT): {
                ImGui::InputFloat("##VariableNameFloat", &variable_value_float_);
                break;
            }
            case (BlackBoardTypes::BOOL): {
                ImGui::Checkbox("##VariableNameBool", &variable_value_bool_);
                break;
            }
            case (BlackBoardTypes::STRING): {
                if (ImGui::InputText("##VariableNameString", variable_value_buffer_, IM_ARRAYSIZE(variable_value_buffer_))) {
                    variable_value_string_ = variable_value_buffer_;
                }
                break;
            }
            }

        }

#endif // EDITOR

        iga::bt::NodeResult BBCompare::OnUpdate() {

            bool is_valid = false;

            switch (type_) {
                case(BlackBoardTypes::INT): {
                    is_valid = EqualCompare(variable_name_,variable_value_int_);
                    break;
                }
                case (BlackBoardTypes::FLOAT): {
                    is_valid = EqualCompare(variable_name_, variable_value_float_);
                    break;
                }
                case (BlackBoardTypes::BOOL): {
                    is_valid = EqualCompare(variable_name_, variable_value_bool_);
                    break;
                }
                case (BlackBoardTypes::STRING): {
                    is_valid = EqualCompare(variable_name_, variable_value_string_);
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