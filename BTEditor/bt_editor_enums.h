#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>

#include <string>

/**
*   @file bt_editor_enums.h
*   @author Channing Eggers
*   @date 20 March 2018
*   @brief Header File for iga::bte::PinType, iga::bte::PinKind, iga::bte::NodeType.
*
*   @section DESCRIPTION
*   ---
*   File contains enums used by the Behaviour Tree Editor. Will also be used when loading
*   the behaviour tree files into game code.
*   ---
*
*/

/**
*  IGArt namespace
*/
namespace iga {

    /**
    *   Behaviour Tree Editor namspace
    */
    namespace bte {

#pragma region PinType Enum Class

        /** PinType enum, contains the types for the pins. */
        enum class PinType {
            FLOW,           /**< PinType Flow       */
            //BOOL,         /**< PinType Bool.      */
            //INT,          /**< PinType Int        */
            //FLOAT,        /**< PinType Float      */
            OBJECT,         /**< PinType Object     */
            FUNCTION,       /**< PinType Function   */
            //DELEGATE,     /**< PinType Delegate   */
        };

        /**
        *   Converts a PinType enum to a string.
        *   @param PinType a_pind_type The pin typethat you want to convert.
        *   @return std::string The converted pind type to string.
        */
        std::string PinTypeToString(PinType a_pin_type);

        /**
        *   Tries to convert a string to a PinType.
        *   @param NodeType a_pin_string The node type that you want to convert.
        *   @return std::string The converted pind type to string.
        */
        PinType PinTypeFromString(const std::string& a_pin_string);

        /**
        *   Cereal Save Serialization funtion for PinType enum.
        */
        template <class Archive>
        std::string save_minimal(const Archive&, const PinType& a_pin_type)
        {
            return PinTypeToString(a_pin_type);
        }

        /**
        *   Cereal Load Serialization funtion for PinType enum.
        */
        template <class Archive>
        void load_minimal(const Archive&, PinType& a_pin_type, const std::string& a_string)
        {
            a_pin_type = PinTypeFromString(a_string);
        }

#pragma endregion

#pragma region PinKind Enum Class

        /** PinKind enum, contains the different kinds of nodes. */
        enum class PinKind {
            SOURCE,         /**< PinKind Source     */
            TARGET          /**< PinKind Target     */
        };

        /**
        *   Converts a PinKind enum to a string.
        *   @param PinKind a_pind_kind The pin kindthat you want to convert.
        *   @return std::string The converted pind kind to string.
        */
        std::string PinKindToString(PinKind a_pin_kind);

        /**
        *   Tries to convert a string to a PinKind.
        *   @param NodeType a_pin_string The node type that you want to convert.
        *   @return std::string The converted pind kind to string.
        */
        PinKind PinKindFromString(const std::string& a_pin_string);

        /**
        *   Cereal Save Serialization funtion for PinKind enum.
        */
        template <class Archive>
        std::string save_minimal(const Archive&, const PinKind& a_pin_kind)
        {
            return PinKindToString(a_pin_kind);
        }

        /**
        *   Cereal Load Serialization funtion for PinKind enum.
        */
        template <class Archive>
        void load_minimal(const Archive&, PinKind& a_pin_kind, const std::string& a_string)
        {
            a_pin_kind = PinKindFromString(a_string);
        }

#pragma endregion

#pragma region NodeType Enum Class

        /** NodeType enum, specifies the different types a node can be. */
        enum class NodeType {
            //BLUEPRINT,    /**< NodeType Blueprint */
            SIMPLE,         /**< NodeType Simple    */
            TREE,           /**< NodeType Tree      */
            COMMENT         /**< NodeType Comment   */
        };

        /**
        *   Converts a NodeType enum to a string.
        *   @param NodeType a_node_type The node type that you want to convert.
        *   @return std::string The converted node type to string.
        */
        std::string NodeTypeToString(NodeType a_node_type);

        /**
        *   Tries to convert a string to a NodeType.
        *   @param NodeType a_node_string The node type that you want to convert.
        *   @return std::string The converted node type to string.
        */
        NodeType NodeTypeFromString(const std::string& a_node_string);

        /**
        *   Cereal Save Serialization funtion for NodeType enum.
        */
        template <class Archive>
        std::string save_minimal(const Archive&, const NodeType& a_node_type)
        {
            return NodeTypeToString(a_node_type);
        }

        /**
        *   Cereal Load Serialization funtion for NodeType enum.
        */
        template <class Archive>
        void load_minimal(const Archive&, NodeType& a_node_type, const std::string& a_string)
        {
            a_node_type = NodeTypeFromString(a_string);
        }

#pragma endregion

    } // End of namespace ~ bte

} // End of namespace ~ iga