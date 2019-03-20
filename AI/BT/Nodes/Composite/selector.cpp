#include <AI\BT\Nodes/Composite/selector.h>

#include <AI\BT\Nodes/node_enums.h>

CEREAL_REGISTER_TYPE(iga::bt::Selector)
CEREAL_REGISTER_POLYMORPHIC_RELATION(iga::bt::CompositeNode, iga::bt::Selector)

namespace iga {

    namespace bt {

        Selector::Selector() { /*EMPTY*/ }
        Selector::~Selector() { /*EMPTY*/ }

        std::shared_ptr<BaseNode> Selector::CloneToShared() const { return std::make_shared<Selector>(*this); };

        BaseNode* Selector::Clone() const { return new Selector(*this); }

        NodeResult Selector::OnUpdate() {

            for (auto child_node : this->GetChildren()) {
                NodeResult result = child_node->ExecuteNode();

                // If result isn't failed return the result, otherwise keep 
                // updating children until a child doesn't return failed.
                if (result != NodeResult::FAILED) {
                    return result;
                }
            }

        // Selector returns failed when all child nodes failed.
        return NodeResult::FAILED;

        }

#ifdef EDITOR

        void Selector::ConstructEditorNode(iga::bte::EditorNode* a_editor_node) {
            a_editor_node->SetName(GET_VARIABLE_NAME(Selector));
            a_editor_node->SetType(iga::bte::NodeType::TREE);

            a_editor_node->AddInputPin(iga::bte::PinType::FLOW);
            a_editor_node->AddOutputPin(iga::bte::PinType::FLOW);
        }

#endif // EDITOR

    } // End of namespace ~ bt

} // End of namespace ~ iga
