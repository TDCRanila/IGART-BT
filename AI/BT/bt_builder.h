#pragma once

#include <Core/entity.h>

#include <Editor/BTEditor/bt_editor_structs.h>

#include <map>
#include <vector>
#include <string>
#include <memory>

/**
* @file bt_builder.h
* @author Channing Eggers
* @date 03 April 2018
* @brief iga::bt::BTBuilder
*
* iga::bt::BTBuilder loads in a BT File and converts it into an in-game behaviour tree.
*
*/

namespace iga {
    namespace bte {
        struct Link;
        struct EditorSaveStruct;
        struct EditorNode;
    } // End of namspace ~ bte
} // End of namespace ~ iga

/**
*  IGArt namespace
*/
namespace iga {

    /**
    *   BT namspace
    */
    namespace bt {

        class BehaviourTree;
        class BaseNode;

        /**
        *
        */
        class BTBuilder {
        public:

            /**
            *   BTBuilder Default Constructor.
            */
            BTBuilder();
            
            /**
            *   BTBuilder Default Destructor.
            */
            ~BTBuilder();

            /**
            *   Function builds a behaviour tree from a BT File.
            *   @param std::string a_bt_file File Path of the BT File.
            *   @param iga::bt::BehaviourTree a_bt Pointer to a behaviour tree.
            *   @param std::weak_tr<Entity> a_entity A weak point to the owner of this behaviour tree.
            *   @aram bool a_overwrite_tree Overwrites the behaviour tree if it wasn't nullptr.
            */
            bool BuildTree(std::string a_bt_file, BehaviourTree*& a_bt, std::weak_ptr<Entity> a_entity, bool a_overwrite_tree);

        };

    } // End of namespace ~  iga

} // End of namespace ~ bt