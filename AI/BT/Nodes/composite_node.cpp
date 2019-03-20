#include <AI\BT\Nodes/composite_node.h>

CEREAL_REGISTER_TYPE(iga::bt::CompositeNode)
CEREAL_REGISTER_POLYMORPHIC_RELATION(iga::bt::BaseNode, iga::bt::CompositeNode)

/**
*  igart namespace
*/
namespace iga {

    namespace bt {

        CompositeNode::CompositeNode() {
            this->node_type_ = NodeType::COMPOSITE;
        }

        CompositeNode::~CompositeNode() { /* EMPTY */ }

        void CompositeNode::AddChild(BaseNode* a_child) {
            this->children_.push_back(a_child);
        }

        std::vector<BaseNode*> CompositeNode::GetChildren() const {
            return this->children_;
        }

    } // End of namespace ~ bt;

} // End of namespcae ~ iga
