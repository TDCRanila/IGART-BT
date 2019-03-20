#include <AI\BT\Nodes/Decorator/root.h>

#include <AI\BT\Nodes/node_enums.h>

CEREAL_REGISTER_TYPE(iga::bt::Root)
CEREAL_REGISTER_POLYMORPHIC_RELATION(iga::bt::DecoratorNode, iga::bt::Root)

namespace iga {

    namespace bt {

        Root::Root() { /*EMPTY*/ }
        Root::~Root() { /*EMPTY*/ }

        std::shared_ptr<BaseNode> Root::CloneToShared() const { return std::make_shared<Root>(*this); };

        BaseNode* Root::Clone() const { return new Root(*this); }

        NodeResult Root::OnUpdate() {

            NodeResult result = this->GetChild()->ExecuteNode();

            return result;

        }

#ifdef EDITOR

        void Root::ConstructEditorNode(iga::bte::EditorNode* a_editor_node) {
            a_editor_node->SetName(GET_VARIABLE_NAME(Root));
            a_editor_node->SetType(iga::bte::NodeType::TREE);

            a_editor_node->AddOutputPin(iga::bte::PinType::FLOW);
        }

#endif // EDITOR

    } // End of namespace ~ bt

} // End of namespace ~ iga
