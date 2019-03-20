#include <AI/BT/behaviour_tree.h>

#include <Core/entity.h>

#include <AI/BT/Nodes/base_node.h>
#include <AI/BT/black_board.h>
#include <AI/BT/bt_debugger.h>

#include <AI/BT/bt_defines.h>

#include <iostream>

namespace iga {

    namespace bt {

        BehaviourTree::BehaviourTree(std::weak_ptr<iga::Entity> a_target) :
            target_(a_target),
            blackboard_(),
            debugger_()
        { 
            std::shared_ptr<iga::Entity> target = a_target.lock();
            DEBUGASSERT(target);
            this->name_ = "BT-ID-";
            this->name_.append(std::to_string(target->GetID()));

            this->blackboard_ = new BlackBoard();
            this->debugger_ = new BTDebugger();

        }

        BehaviourTree::~BehaviourTree() { 
            // Delete the blackboard.
            if (this->blackboard_ != nullptr) { delete this->blackboard_; }

            // Delete tracked nodes.
            for (auto& tracked_node : this->tracked_nodes_) {
                if (tracked_node != nullptr) {
                    delete tracked_node;
                    tracked_node = nullptr;
                }
            }
            tracked_nodes_.clear();
        }

        void BehaviourTree::ExecuteTree() {

#ifndef BT_DEBUG
            this->root_->ExecuteNode();
#else
            std::cout << "------------" << std::endl;

            std::cout << "Executing: " << this->name_ << std::endl;

            this->root_->ExecuteNode(this);

            this->debugger_->ResetDebugger();

            std::cout << "------------" << std::endl;
#endif
        }

        void BehaviourTree::SetRoot(BaseNode* a_node) {
            this->root_ = a_node;
        }

        std::weak_ptr<Entity> BehaviourTree::GetTarget() const {
            return this->target_;
        }

        void BehaviourTree::TrackNode(BaseNode* a_node_to_be_tracked) {
            this->tracked_nodes_.push_back(a_node_to_be_tracked);
        }

        void BehaviourTree::GetTrackedNodes(std::vector<iga::bt::BaseNode*>* a_empty_vector) {
            (*a_empty_vector) = this->tracked_nodes_;
        }


        BlackBoard* BehaviourTree::GetBlackBoard() const {
            return this->blackboard_;
        }

        BTDebugger* BehaviourTree::GetDebugger() const {
            return this->debugger_;
        }

    } // End of namespace ~ bt

} // End of namespace ~ iga
