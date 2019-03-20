#include <AI/BT/Nodes/Leaf/debug_message.h>

#include <iostream>

CEREAL_REGISTER_TYPE(iga::bt::DebugMessage)
CEREAL_REGISTER_POLYMORPHIC_RELATION(iga::bt::LeafNode, iga::bt::DebugMessage)

namespace iga {

    /**
    *  BT namespace
    */
    namespace bt {

        DebugMessage::DebugMessage() { /*EMPTY*/ }

        DebugMessage::~DebugMessage() {/*EMPTY*/ }

        std::shared_ptr<BaseNode> DebugMessage::CloneToShared() const { return std::make_shared<DebugMessage>(*this); };

        BaseNode* DebugMessage::Clone() const { return new DebugMessage(*this); }

#ifdef EDITOR

        void DebugMessage::OnEditorLoad() {
            // Copy over serialized string.
            strcpy_s(this->debug_message_buffer_, this->debug_message_.c_str());
        }

        void DebugMessage::ConstructEditorNode(iga::bte::EditorNode* a_editor_node) {
            a_editor_node->SetName(GET_VARIABLE_NAME(DebugMessage));
            a_editor_node->SetType(iga::bte::NodeType::TREE);

            a_editor_node->AddInputPin(iga::bte::PinType::FLOW);
        }

        void DebugMessage::InspectNode() {
            if (ImGui::InputText("DebugMessage:", debug_message_buffer_, IM_ARRAYSIZE(debug_message_buffer_))) {
                this->debug_message_ = debug_message_buffer_;
            }
        }

#endif // EDITOR

        iga::bt::NodeResult DebugMessage::OnUpdate() {
            std::cout << debug_message_ << std::endl;
            return iga::bt::NodeResult::SUCCESS;
        }

    } // End of namespace ~ bt

} // End of namespace ~ iga