#include <AI\BT\Nodes/Composite/sequence.h>

#include <AI\BT\Nodes/node_enums.h>

CEREAL_REGISTER_TYPE(iga::bt::Sequence)
CEREAL_REGISTER_POLYMORPHIC_RELATION(iga::bt::CompositeNode, iga::bt::Sequence)

namespace iga {

    namespace bt {

        Sequence::Sequence() { /*EMPTY*/ }

        Sequence::~Sequence() { /*EMPTY*/ }

        std::shared_ptr<BaseNode> Sequence::CloneToShared() const { return std::make_shared<Sequence>(*this); };

        BaseNode* Sequence::Clone() const { return new Sequence(*this); }

        NodeResult Sequence::OnUpdate() {

            for (auto child_node : this->GetChildren()) {
                NodeResult result = child_node->ExecuteNode();

                // If node result isn't SUCCESS - Stop updating
                // and return the result.
                if (result != NodeResult::SUCCESS) {
                    return result;
                }
            }

            return NodeResult::SUCCESS;

        }

#ifdef EDITOR

        void Sequence::ConstructEditorNode(iga::bte::EditorNode* a_editor_node) {
            a_editor_node->SetName(GET_VARIABLE_NAME(Sequence));
            a_editor_node->SetType(iga::bte::NodeType::TREE);

            a_editor_node->AddInputPin(iga::bte::PinType::FLOW);
            a_editor_node->AddOutputPin(iga::bte::PinType::FLOW);
        }

#endif // EDITOR

    } // End of namespace ~ bt

} // End of namespace ~ iga
