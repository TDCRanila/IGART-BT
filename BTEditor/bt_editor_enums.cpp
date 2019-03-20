#include <Editor/BTEditor/bt_editor_enums.h>

#include <map>

namespace iga {

    namespace bte {

        std::map<PinType, std::string> PinTypeFMap      = { { PinType::FLOW, "FLOW" }, { PinType::FUNCTION, "FUNCTION" },{ PinType::OBJECT, "OBJECT" } };
        std::map<std::string, PinType> PinTypeRMap      = { { "FLOW", PinType::FLOW }, { "FUNCTION", PinType::FUNCTION}, { "OBJECT", PinType::OBJECT } };

        std::map<PinKind, std::string> PinKindFMap      = { { PinKind::SOURCE, "SOURCE" },{ PinKind::TARGET, "TARGET" } };
        std::map<std::string, PinKind> PinKindRMap      = { { "SOURCE", PinKind::SOURCE },{ "TARGET", PinKind::TARGET } };

        std::map<NodeType, std::string> NodeTypeFMap    = { { NodeType::SIMPLE, "SIMPLE" },{ NodeType::TREE, "TREE" },{ NodeType::COMMENT, "COMMENT" } };
        std::map<std::string, NodeType> NodeTypeRMap    = { { "SIMPLE", NodeType::SIMPLE },{ "TREE", NodeType::TREE },{ "COMMENT", NodeType::COMMENT } };

        std::string PinTypeToString(PinType a_pin_type) {
            return PinTypeFMap[a_pin_type];
        }

        PinType PinTypeFromString(const std::string& a_pin_string) {
            return PinTypeRMap[a_pin_string];
        }

        std::string PinKindToString(PinKind a_pin_kind) {
            return PinKindFMap[a_pin_kind];
        }

        PinKind PinKindFromString(const std::string& a_pin_string) {
            return PinKindRMap[a_pin_string];
        }

        std::string NodeTypeToString(NodeType a_node_type) {
            return NodeTypeFMap[a_node_type];
        }

        NodeType NodeTypeFromString(const std::string& a_node_string) {
            return NodeTypeRMap[a_node_string];
        }
    
    } // End of namespace ~ bte

} // End of namespace ~ iga