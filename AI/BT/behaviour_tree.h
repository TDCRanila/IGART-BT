#pragma once

#include <string>
#include <memory>
#include <vector>

/**
*   @file behaviour_tree.h
*   @brief iga::bt::BehaviourTree Header File
*
*   @author Channing Eggers
*   @date 2 March 2018
*   @version $Revision 1.1
*
*   Class contains a base_node root that connects to other base_nodes.
*   Class has a function; ExecuteTree, which will update the whole tree.
*   Each Behaviour Tree has its own BlackBoard.
*   A Behaviour Tree has access to the Entity it is targetting.
*   Behaviour Tree debugging can be enabled by define #BT_DEBUG in bt_defines.h
*
*/

/**
*  IGArt namespace
*/
namespace iga {

    class Entity;

    /**
    *  BT namespace
    */
    namespace bt {

        class BaseNode;
        class BlackBoard;
        class BTDebugger;

        /**
        *  iga::bt::BehaviourTree; Class functions as the main "brain" of the system.
        */
        class BehaviourTree final {
        public:

            /**
            *   BehaviourTree Constructor
            *   @param a_target is a weak pointer to an iga:Entity. This BehaviourTree will 
            *   target this entity. 
            */
            BehaviourTree(std::weak_ptr<iga::Entity> a_target);

            /// BehaviourTree Destructor.
            ~BehaviourTree();

            /**
            *   Function that executes the tree its behaviour. It
            *   calls the root's update function.
            *   @see iga::BaseNode
            */
            void ExecuteTree();

            /**
            *   Function sets the root node for this behaviour tree.
            *   @param a_node is a pointer to a iga::bt::BaseNode.
            */
            void SetRoot(BaseNode* a_node);

            /**
            *   Returns a pointer of the behaviour trees's targeted Entitiy.
            *   @return Pointer to target iga::Entity.
            */
            std::weak_ptr<Entity> GetTarget() const;

            /**
            *   Adds the BaseNode to a vector. The behaviour tree will keep track of it.
            *   When the behaviour tree calls it destructor, it will delete its tracked nodes.
            *   @param BaseNode a_node_to_be_tracked Pointer to a BaseNode that want gets tracked.
            */
            void TrackNode(BaseNode* a_node_to_be_tracked);

            /**
            *   Function returns tracked nodes stored inside of the vector that got passed through.
            *   @param std::vector<iga::bte::BaseNode*> a_empty_vector The returned vector with nodes.
            */
            void GetTrackedNodes(std::vector<iga::bt::BaseNode*>* a_empty_vector);

            /**
            *   Returns a pointer to the blackboard of the behaviour tree.
            *   @return Pointer to iga::bt::BlackBoard.
            */
            BlackBoard* GetBlackBoard() const;

            /**
            *   Returns a pointer to the debugger of the behaviour tree.
            *   @return Pointer to iga::bt::BTDebugger.
            */
            BTDebugger* GetDebugger() const;

        private:

            std::string name_ = "";                 /**< Name of the Behaviour Tree. */

            BaseNode* root_ = nullptr;              /**< Root Node of the Behaviour Tree. */
            std::vector<BaseNode*> tracked_nodes_;  /**< Vector of nodes that this behaviour tree ownes. Only used to delete nodes when behaviour tree destructor gets called.*/

            std::weak_ptr<iga::Entity> target_;     /**< The target of the Behaviour Tree. */

            BlackBoard* blackboard_ = nullptr;      /**< Blackboard for this behaviour tree. */
            BTDebugger* debugger_ = nullptr;        /**< Behaviour Tree Debugger. */
            
        };

    } // End of namespace ~ bt

} // End of namespace ~ iga
