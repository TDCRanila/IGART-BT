#pragma once

/**
* @file node_enums.h
* @brief File that contains enums used by the Behaviour Tree System
*
* @author Channing Eggers
* @date 1 March 2018
* @version $Revision 1.1
*
* The NodeType enum, represents the types that a node can be.
* The NodeResult enum, represents the states that a node can be in, while or after it is doing its actions.
*
*/

/**
*  igart namespace
*/
namespace iga {
    
    /**
    *  BT namespace
    */
    namespace bt {

        /** The NodeType enum, represents the type of a node. */
        enum class NodeType {
            LEAF        = 0,    /**< Specifies that the node is a "LEAF Node". */
            COMPOSITE   = 1,    /**< Specifies that the node is a "COMPOSITE Node". */
            DECORATOR   = 2     /**< Specifies that the node is a "DECORATOR Node". */
        };

        /** The NodeResult enum, represenst the states that a node can be in while or after updating. */
        enum class NodeResult {
            SUCCESS         = 0,    /**< "SUCCESS" means that the node has succesfully executed its action(s). */
            RUNNING         = 1,    /**< "RUNNING" means that the node is still in a state that it performs an actions(s). */
            FAILED          = 2,    /**< "FAILED" means that the node has failed while executing its action(s). */
            FATAL_ERROR     = 3     /**< "NODE_ERROR" means that the node has encountered unexpected behaviour and that it cannot recover from it. */
        };

    } // End of namespcae ~ bt

} // End of namespace ~ iga
