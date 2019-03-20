#include <AI\BT\Nodes/base_node.h>

#include <AI\BT\bt_debugger.h>
#include <AI\BT\bt_defines.h>
#include <AI\BT\behaviour_tree.h>
#include <AI\BT\Nodes/node_enums.h>

CEREAL_REGISTER_TYPE(iga::bt::BaseNode)

namespace iga {
    
    namespace bt {

        BaseNode::BaseNode() {
            this->node_id_ = GetNextID();
        }

        BaseNode::~BaseNode() { /* EMPTY */ }

        std::shared_ptr<BaseNode> BaseNode::CloneToShared() const { return nullptr; }

        BaseNode* BaseNode::Clone() const { return nullptr; }

        void BaseNode::OnBehaviourTreeBuild() { /*DO NOTHING*/ }

        NodeResult BaseNode::ExecuteNode() {

            if (this->behaviour_tree_ == nullptr) {
                return NodeResult::FATAL_ERROR;
            }

#ifndef BT_DEBUG
            this->OnEntry();

            NodeResult result = this->OnUpdate();

            this->OnExit();

            return result;
#else
            this->behaviour_tree_->GetDebugger()->OnNodeEntry(this);
            this->OnEntry();

            this->behaviour_tree_->GetDebugger()->OnNodeUpdate(this);
            NodeResult result = this->OnUpdate();

            this->behaviour_tree_->GetDebugger()->OnNodeExit(this);
            this->OnExit();

            return result;
#endif // BT_DEBUG

        }

#ifdef EDITOR
        void BaseNode::ConstructEditorNode(iga::bte::EditorNode* /*a_editor_node*/) {
            /* DO NOTHING - FUNCTION SHOULD GET OVERRIDEN */
        }

        void BaseNode::OnEditorLoad() {
            /* DO NOTHING - FUNCTION SHOULD GET OVERRIDEN */
        }

        void BaseNode::InspectNode() {
            /* DO NOTHING - FUNCTION SHOULD GET OVERRIDEN */
        }
#endif

        int BaseNode::GetID() const {
            return this->node_id_;
        }

        NodeType BaseNode::GetNodeType() const {
            return this->node_type_;
        }

        void BaseNode::SetNodeName(std::string a_node_name) {
            this->node_type_name_ = a_node_name;
        }

        std::string BaseNode::GetNodeName() const {
            return this->node_type_name_;
        }

        BehaviourTree* BaseNode::GetBehaviourTree() const {
            return this->behaviour_tree_;
        }

        unsigned int BaseNode::GetNextID() {
            return ++node_id_counter_;
        }

    } // End of namespace ~ bt

} // End of namespace ~ iga
