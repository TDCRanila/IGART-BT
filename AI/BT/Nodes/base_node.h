#pragma once

#include <AI/BT/behaviour_tree.h>
#include <AI/BT/Nodes/node_enums.h> 

#include <Editor/BTEditor/bt_editor_node_library.h>

#include <defines.h>
#include <iga_util.h>

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>  

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

#ifdef EDITOR
#include <Editor/BTEditor/bt_editor_enums.h>
#endif // EDITOR

#include <string>
#include <map>
#include <string>
#include <utility>

/**
* @file base_node.h
* @brief iga::bt::BaseNode Header File
*
* @author Channing Eggers
* @date 5 March 2018
* @version $Revision 1.2
*
* The class BaseNode is a base class. Derived classes must
* implement the pure virtual function; OnUpdate, and implement 
* their functionality in there.
*
* Child Nodes that need to show up in the editor, need to have their
* ConstructEditorNode overriden and implemented.
*
*/

/**
*  igart namespace
*/
namespace iga {

    namespace bte {
        struct EditorNode;
    }

    /**
    *  BT namespace
    */
    namespace bt {

        static unsigned int node_id_counter_ = 0; /** Static integer that keeps track of the IDs that have been given out. */

        class BehaviourTree;
        class BTDebugger;
        class BTBuilder;

        enum class NodeResult;
        enum class NodeType;

        class BaseNode {
        public:
            friend class BTBuilder;

            /**
            *   Cereal versions of iga::bt::BaseNode.
            */
            enum class Version {
                DEFAULT = 0, /**< Default normal version. */
                LATEST /**< Latest version. */
            };

            /** BaseNode Constructor */
            BaseNode();

            /** BaseNode Virtual Destructor */
            virtual ~BaseNode();

            /** 
            *   Virtual Function that is used when polymorphically cloning this object when a shared_ptr needs to be returned. 
            *   @return std::shared_ptr<BaseNode> Pointer to the newly cloned object.
            */
            virtual std::shared_ptr<BaseNode> CloneToShared() const;

            /** 
            *   Virtual Function that is used when polymorphically cloning this object when a raw pointer needs to be returned.
            *   @return BaseNode* Pointer to the newly cloned object.
            */
            virtual BaseNode* Clone() const;

            /** Virtual Function that gets called whenever the behaviour tree is getting build. */
            virtual void OnBehaviourTreeBuild();

            /**
            *   Executes the node and runs its functionality.
            *   @param a_bt is a pointer to the BehaviourTree.
            */
            NodeResult ExecuteNode();

            /**
            *   Returns the ID of the node.
            *   @return int value.
            */
            int GetID() const;

            /**
            *   Returns the type of the node.
            *   @return NodeType enum value.
            */
            NodeType GetNodeType() const;

            /**
            *   Sets the name of the node.
            *   @param std::string a_node_name The name of the node.
            */
            void SetNodeName(std::string a_node_name);

            /**
            *   Returns the name of the node.
            *   @return std::string value.
            */
            std::string GetNodeName() const;

            /**
            *  Serializes this node.
            */
            template<class Archive>
            void serialize(Archive &/*a_archive*/, uint32 const /*a_version*/) { /*EMPTY*/ }

#ifdef EDITOR
            /**
            *   Virutal function that creates and returns an editor node.
            *   @param EditorNode a_editor_node Pointer to the editor node.
            */
            virtual void ConstructEditorNode(iga::bte::EditorNode* a_editor_node);

            /**
            *   Virtual function that gets called whenever a base node gets loaded by the editor.
            *   (Gets called after the constructor and the loading of serialized data.)
            */
            virtual void OnEditorLoad();

            /**
            *   Virtual function for inspecting the noed in the behaviour tree editor.
            */
            virtual void InspectNode();

#endif // EDITOR

        protected:

            /**
            *   Returns a pointer to the behaviour tree that
            *   this node is part of.
            *   @return BehaviourTree pointer.
            */
            BehaviourTree* GetBehaviourTree() const;

            NodeType node_type_; /** The type of the node. */

            std::string node_type_name_; /** A string of the node type. */

        private:

            /// Virtual function that gets called when before the update function of the node.
            virtual void OnEntry()  { /* Empty */ }
            
            /// Virutal function that gets called to update the node.
            virtual NodeResult OnUpdate() { return NodeResult::SUCCESS; }
            
            /// Virtual function thats gets called after the update function of the ndoe.
            virtual void OnExit() { /* Empty */ }

            /**
            *   Returns a new unsigned integer value.
            *   @return unsigned int.
            */
            unsigned int GetNextID();

            unsigned int node_id_ = 0; /** The ID of the node. */

            BehaviourTree* behaviour_tree_; /** Pointer to the behaviour tree. */

        };

    } // End of namespace ~ bt

} // End of namespace ~ iga

CEREAL_CLASS_VERSION(iga::bt::BaseNode, (static_cast<iga::uint32>(iga::bt::BaseNode::Version::LATEST) - 1));