#pragma once

#include <defines.h>

#include <AI/BT/Nodes/base_node.h>

#include <Editor/BTEditor/bt_editor_enums.h>

#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

#include <vectormath/vectormath.hpp>

#ifdef EDITOR
#include <imgui/include/imgui.h>
#endif // EDITOR

#include <string>
#include <vector>

/**
*   @file bt_editor_structs.h
*   @author Channing Eggers
*   @date 20 March 2018
*   @brief Header File for iga::bte::NodeCombo, iga::bte::Pin, iga::bte::Node, iga::bte::Link
*
*   @section DESCRIPTION
*   ---
*   File contains structs used by the Behaviour Tree Editor & iga::bt::BTBuilder. Structs will also be 
*   used when loading the behaviour tree files into game code.
*   ---
*
*/

/**
*  IGArt namespace
*/
namespace iga {

    namespace bt {
        class BTBuilder;
        class BaseNode;
    }

    /**
    *   Behaviour Tree Editor namspace
    */
    namespace bte {

        struct Pin;
        struct EditorNode;
        struct Link;

        struct EditorSaveStruct {

            /**
            *   Cereal versions of iga::bt::EditorSaveStruct.
            */
            enum class Version {
                DEFAULT = 0,    /**< Default normal version. */
                LATEST          /**< Latest version. */
            };

            /**
            *   Cereal Serialization funtion for EditorSaveStruct struct.
            */
            template <class Archive>
            void serialize(Archive& a_archive, uint32 const a_version);

            int behaviour_tree_id_start;                            /**< The ID where the editor will start at on loading the file. */
            std::string behaviour_tree_name;                        /**< The file name of the behaviour tree. */
            std::vector<EditorNode> behaviour_tree_editor_nodes;    /**< Vector of EditorNodes. */
            std::vector<Link> behaviour_tree_links;                 /**< Vector of Links. */
        };

        struct Pin {

			/**
			*   Cereal versions of iga::Pin.
			*/
			enum class Version {
				DEFAULT = 0,    /**< Default normal version. */
				LATEST          /**< Latest version. */
			};

            /**
            *   Pin Default Constructor
            */
            Pin();

            /**
            *   Pin Constructor
            */
            Pin(int a_id, const char* a_name, PinType a_type);

            /**
            *   Pin Default Destructor
            */
            ~Pin();

            /**
            *   Cereal Serialization funtion for Pin struct.
            */
            template <class Archive>
            void serialize(Archive& a_archive, uint32 const a_version);

            int             id;                 /**< ID of the pin. */
            int             parent_node_id;     /**< ID of the node parent of this pin. */
            EditorNode*     node;               /**< Pointer to parent node. */
            std::string     name;               /**< Name of the Pin used by the editor. */
            PinType         type;               /**< Type of the Pin - Enum iga::bte::PinType. */
            PinKind         kind;               /**< Kind of the Pin - Enum iga::bte::PinKind.*/

        };

        struct EditorNode {
        public:
            friend class BehaviourTreeEditor;
			friend class BTEditorFinder;
            friend class BTEditorFinder;
			friend iga::bt::BTBuilder;
            
			/**
			*   Cereal versions of iga::bte::EditorNode.
			*/
			enum class Version {
				DEFAULT = 0,    /**< Default normal version. */
				LATEST          /**< Latest version. */
			};

            /**
            *   Default Node Constructor
            */
            EditorNode();

            /**
            *   Default Node Destructor
            */
            ~EditorNode();

#ifdef EDITOR
            /**
            *   Custom Node Constructor - Only used by the Editor.
            */
            EditorNode(int a_id, const char* a_name, ImColor a_color = ImColor(255, 255, 255));
#endif

            /**
            *   Custom copy constructor.
            */
            EditorNode(const EditorNode& other);

            /**
            *   Function that adds a pin to the Input Vector.
            *   @param PinType a_pin_type Parameter that sets the type of the pin.
            *   @param std::string a_pin_name Paramater that sets the name of the pin.
            */
            void AddInputPin(PinType a_pin_type, std::string a_pin_name = "");
            
            /**
            *   Function that adds a pin to the Output Vector.
            *   @param PinType a_pin_type Parameter that sets the type of the pin.
            *   @param std::string a_pin_name Paramater that sets the name of the pin.
            */
            void AddOutputPin(PinType a_pin_type, std::string a_pin_name = "");

            /**
            *   Function that sets the type of the editor node.
            *   @param NodeType a_node_type Paramter that sets type of the node.
            */
            void SetType(NodeType a_node_type);

            /**
            *   Function sets the BaseNode for this EditorNode.
            *   @param BaseNode a_base_node Pointer to the base node.
            */
            void SetBaseNode(iga::bt::BaseNode* a_base_node);

            /**
            *   Function that sets the name of the node.
            *   @param std::string a_node_name Paramter to set the name of the node.
            */
            void SetName(std::string a_node_name);

            /**
            *   Function that returns the name of the node.
            *   @return std::string Name of the node.
            */
            std::string GetName() const;

            /**
            *   Cereal Serialization funtion for EditorNode struct.
            */
            template <class Archive>
            void serialize(Archive& a_archive, uint32 const a_version);

        private:
            Vector4             color;              /**< Colour of the EditorNode - used by the editor only. */
            Vector2             size;               /**< Size of the EditorNode - used by the editor only. */

            int                 id;                 /**< ID of the EditorNode. */
            std::string         name;               /**< Name of the EditorNode. */
            std::vector<Pin>    inputs;             /**< Input Pin vector of the EditorNode. */
            std::vector<Pin>    outputs;            /**< Output Pin vector of the EditorNode. */
            std::vector<int>    outgoing_link_IDs;  /**< The IDs of the links owned by this node. */
            std::vector<int>    receiving_link_IDs; /**< The IDs of the links received of this node. */

            NodeType            type;               /**< Type of the EditorNode - ENUM iga::bte::NodeType. */
            std::string         state;              /**< State of the EditorNode - used by the editor only. */
            std::string         saved_state;        /**< The staved state of the EditorNode - used by the editor only. */

            iga::bt::BaseNode*                   base_node;            /**< Pointer to the BaseNode. */
            std::shared_ptr<iga::bt::BaseNode>   saving_base_node;     /**< Pointer to the BaseNode used when saving & loading. */

        };

        struct Link {

			/**
			*   Cereal versions of iga::Link
			*/
			enum class Version {
				DEFAULT = 0,    /**< Default normal version. */
				LATEST          /**< Latest version. */
			};

            /**
            *   Link Default Constructor
            */
            Link();

            /**
            *   Link Constructor
            */
            Link(int a_id, int a_start_pin_id, int a_end_pin_id);

            /**
            *   Link Default Destructor
            */
            ~Link();

            /**
            *   Cereal Serialization funtion for Link struct.
            */
            template <class Archive>
            void serialize(Archive& a_archive, uint32 const a_version);

            Vector4     color;          /** Colour of the Link - used by the editor only. */

            int         id;             /** ID of the Link. */
            int         start_pin_id;   /** ID of the start pin of the Link. */
            int         end_pin_id;     /** ID of the end pin of the Link. */

        };

    } // End of namespace ~ bte

    template <class Archive>
    void bte::EditorSaveStruct::serialize(Archive& a_archive, uint32 const a_version) {
        switch (static_cast<Version>(a_version)) {
        case Version::LATEST:
        case Version::DEFAULT:
            a_archive(  cereal::make_nvp("EditorIDStart", behaviour_tree_id_start),
                        cereal::make_nvp("BTName", behaviour_tree_name),
                        cereal::make_nvp("BTEditorNodes", behaviour_tree_editor_nodes),
                        cereal::make_nvp("BTLinks", behaviour_tree_links)
            );
            break;
        }
    }

    template <class Archive>
    inline void bte::Pin::serialize(Archive& a_archive, uint32 const a_version) {
        switch (static_cast<Version>(a_version)) {
        case Version::LATEST:
        case Version::DEFAULT:
            a_archive(  cereal::make_nvp("PinID", id),
                        cereal::make_nvp("ParentNodeId", parent_node_id),
                        cereal::make_nvp("PinName", name),
                        cereal::make_nvp("PinType", type),
                        cereal::make_nvp("PinKind", kind)
            );
            break;
        }
    }

    template <class Archive>
    void bte::EditorNode::serialize(Archive& a_archive, uint32 const a_version) {
        switch (static_cast<Version>(a_version)) {
        case Version::LATEST:
        case Version::DEFAULT:
            a_archive(  cereal::make_nvp("EditorNodeID", id),
                        cereal::make_nvp("EditorNodeName", name),
                        cereal::make_nvp("EditorNodeInput", inputs),
                        cereal::make_nvp("EditorNodeOutput", outputs),

                        cereal::make_nvp("OutgoingLinkIDs", outgoing_link_IDs),
                        cereal::make_nvp("ReceivingLinkIDs", receiving_link_IDs),

                        cereal::make_nvp("EditorNodeColour", color),
                        cereal::make_nvp("NodeType", type),

                        cereal::make_nvp("EditorNodeSize", size),
                        cereal::make_nvp("EditorNodeState", state),
                        cereal::make_nvp("EditorNodeSavedState", saved_state),

                        cereal::make_nvp("BaseNodeData", saving_base_node)
            );
            break;
        }
    }

    template <class Archive>
    void bte::Link::serialize(Archive& a_archive, uint32 const a_version) {
        switch (static_cast<Version>(a_version)) {
        case Version::LATEST:
        case Version::DEFAULT:
            a_archive(  cereal::make_nvp("LinkID", id),
                        cereal::make_nvp("LinkColour", color),
                        cereal::make_nvp("StartPinID", start_pin_id),
                        cereal::make_nvp("EndPinID", end_pin_id)
            );
            break;
        }
    }


} // End of namespace ~ iga

CEREAL_CLASS_VERSION(iga::bte::Pin, (static_cast<iga::uint32>(iga::bte::Pin::Version::LATEST) - 1));
CEREAL_CLASS_VERSION(iga::bte::EditorSaveStruct, (static_cast<iga::uint32>(iga::bte::EditorSaveStruct::Version::LATEST) - 1));
CEREAL_CLASS_VERSION(iga::bte::EditorNode, (static_cast<iga::uint32>(iga::bte::EditorNode::Version::LATEST) - 1));
CEREAL_CLASS_VERSION(iga::bte::Link, (static_cast<iga::uint32>(iga::bte::Link::Version::LATEST) - 1));
