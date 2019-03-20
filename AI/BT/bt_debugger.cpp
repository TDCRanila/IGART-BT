#include <AI\BT\bt_debugger.h>

#include <AI\BT\Nodes/base_node.h>
#include <AI\BT\Nodes/node_enums.h>

#include <iostream>

namespace iga {

    namespace bt {

        BTDebugger::BTDebugger() { /*EMPTY*/ }

        BTDebugger::~BTDebugger() { /*EMPTY*/ }

        void BTDebugger::OnNodeEntry(BaseNode* a_node) {

            std::string node_type_string = this->ConvertNodeTypeEnum(a_node->GetNodeType());
            std::cout << "ID:" << a_node->GetID() << " - Node: " << a_node->GetNodeName() << " - " 
                      << node_type_string << " -  Node Entry" << std::endl;

            this->nodes_updated_.push_back(a_node);
            ++this->node_count_;
        }

        void BTDebugger::OnNodeUpdate(BaseNode* a_node) {

            std::string node_type_string = this->ConvertNodeTypeEnum(a_node->GetNodeType());
            std::cout << "ID:" << a_node->GetID() << " - Node: " << a_node->GetNodeName() << " - " 
                      << node_type_string << " -  Node Update" << std::endl;

        }

        void BTDebugger::OnNodeExit(BaseNode* a_node) {

            std::string node_type_string = this->ConvertNodeTypeEnum(a_node->GetNodeType());
            std::cout << "ID:"  << a_node->GetID() << " - Node: " << a_node->GetNodeName() << " - " 
                      << node_type_string << " -  Node Exit" << std::endl;

        }

        std::string BTDebugger::ConvertNodeTypeEnum(NodeType a_node_type) {
            switch (a_node_type) {
            case (NodeType::COMPOSITE)  :   return std::string("Composite");
            case (NodeType::DECORATOR)  :   return std::string("Decorator");
            case (NodeType::LEAF)       :   return std::string("Leaf     ");
            default:                        return std::string("NonSpecifiedNodeType");
            }
        }

        void BTDebugger::ResetDebugger() {
            this->node_count_ = 0;
            if (!this->nodes_updated_.empty()) { this->nodes_updated_.clear(); }
        }

    } // End of namespace ~ bt

} // End of namespace ~ iga
