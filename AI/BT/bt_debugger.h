#pragma once

#include <vector>

/**
* @file bt_debugger.h
* @brief iga::bt::BTDebugger Header File.
*
* @author Channing Eggers
* @date 5 March 2018
* @version $Revision 1.1
*
* BTDebugger class helps with debugging the behaviour tree.
*
*/

/**
*  igart namespace
*/
namespace iga {

    namespace bt {

        class BaseNode;
        enum class NodeType;
        
        class BTDebugger final {
        public:
            
            /** BTDebugger Constructor */
            BTDebugger();

            /** BTDebugger Destructor */
            ~BTDebugger();

            /** Debugger function for Node Entry. Outputs debug lines to the console
            *   regarding the passed through node.
            *   @param a_node is a pointer to a iga::bt::BaseNode.
            */
            void OnNodeEntry(BaseNode* a_node);
            
            /** Debugger function for Node Update. Outputs debug lines to the console
            *   regarding the passed through node.
            *   @param a_node is a pointer to a iga::bt::BaseNode.
            */
            void OnNodeUpdate(BaseNode* a_node);
            
            /** Debugger function for Node Exit. Outputs debug lines to the console
            *   regarding the passed through node.
            *   @param a_node is a pointer to a iga::bt::BaseNode.
            */
            void OnNodeExit(BaseNode* a_node);

            /** Reset the debugger and its values */
            void ResetDebugger();

        private:

            std::string ConvertNodeTypeEnum(NodeType a_node_type); /** Convert NodeType Enum to a string that can be used for debugging purposes. */

            int node_count_ = 0; /** Integer value that keeps track of how many nodes have been updated. */

            std::vector<BaseNode*> nodes_updated_; /** List of updated nodes. */

        };

    } // End of namespace ~ bt

} // End of namespace ~ iga
