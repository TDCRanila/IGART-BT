#include <AI\BT\Nodes/leaf_node.h>

CEREAL_REGISTER_TYPE(iga::bt::LeafNode)
CEREAL_REGISTER_POLYMORPHIC_RELATION(iga::bt::BaseNode, iga::bt::LeafNode)

/**
*  igart namespace
*/
namespace iga {

    namespace bt {

        LeafNode::LeafNode() {
            this->node_type_ = NodeType::LEAF;
        }

        LeafNode::~LeafNode() { /*EMPTY*/ }


    } // End of namespace ~ bt;

} // End of namespcae ~ iga
