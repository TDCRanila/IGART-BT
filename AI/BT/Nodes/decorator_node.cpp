#include <AI\BT\Nodes/decorator_node.h>

CEREAL_REGISTER_TYPE(iga::bt::DecoratorNode)
CEREAL_REGISTER_POLYMORPHIC_RELATION(iga::bt::BaseNode, iga::bt::DecoratorNode)

namespace iga {

    namespace bt {

        DecoratorNode::DecoratorNode() {
            this->node_type_ = NodeType::DECORATOR;
        }

        DecoratorNode::~DecoratorNode() { /*EMPTY*/ }

        void DecoratorNode::SetChild(BaseNode* a_child) {
            this->child_ = a_child;
        }

        BaseNode* DecoratorNode::GetChild() const {
            return this->child_;
        }

    } // End of namespace ~ bt;

} // End of namespcae ~ iga
