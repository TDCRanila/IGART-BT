#pragma once

#include <vector>
#include <map>

/**
*   @file bt_editor_finder.h
*   @author Channing Eggers
*   @date 20 March 2018
*   @brief iga::bte::BTEditorFinder Header File
*
*   @section DESCRIPTION
*   ---
*   BTEditorFinder searched for items in the storage
*   containers of the Behaviour Tree Editor.
*
*   BTEditorFinder is befriended with BehaviourTreeEditor.   
*   ---
*
*/

/**
*   IGArt namespace
*/
namespace iga {

    namespace bt {
        class BaseNode;
    }

    /**
    *   Behaviour Tree Editor namspace
    */
    namespace bte {

        struct EditorNode;
        struct Link;
        struct Pin;

        class BehaviourTreeEditor;

        class BTEditorFinder {
        public:

            /*
            *   BTEditorFinder Default Constructor.
            */
            BTEditorFinder();

            /*
            *   BTEditorFinder Constructor.
            *   @param a_bt_editor is a BTEditor pointer.
            */
            BTEditorFinder(BehaviourTreeEditor* a_bt_editor);

            /*
            *   BTEditorFinder Destructor.
            */
            ~BTEditorFinder();

            /*
            *   FindNode in the editor and return a pointer to it.
            *   Returns nullptr if there are no nodes with this ID.
            *   @param a_id is an integer.
            *   @return Node pointer to the node with this ID.
            */
            EditorNode* FindNode(int a_id);

            /*
            *   Find nnode with this pin in the editor and return a pointer to it.
            *   Returns nullptr if there are no nodes with this pin ID.
            *   @param a_pin_id is an integer.
            *   @return Node pointer to the node with this ID.
            */
            EditorNode* FindNodeWithPinID(int a_pin_id);

            /*
            *   FindLink in the editor and return a pointer to it.
            *   Returns nullptr if there are no links with this ID.
            *   @param a_id is an integer.
            *   @return Link pointer to the node with this ID.
            */
            Link* FindLink(int a_id);

            /*
            *   FindLink in the vector and return a pointer to it.
            *   Returns nullptr if there are no links with this ID.
            *   @param a_id is an integer.
            *   @return Link pointer to the node with this ID.
            */
            Link* FindLink(int a_id, std::vector<Link*>* a_search_link_vector);

            /*
            *   FindPin in the editor and return a pointer to it.
            *   Returns nullptr if there are no pins with this ID.
            *   @param a_id is an integer.
            *   @return Pin pointer to the node with this ID.
            */
            Pin* FindPin(int a_id);

            /*
            *   Check if a pin already has a connection.
            *   @param a_id int value.
            *   @return bool value ~ Returns True if a pin has a connection.
            */
            bool IsPinLinked(int a_id);

            /*
            *   Function checks if two pins are compatible with each other.
            *   @param a is a pointer to a Pin.
            *   @param b is a pointer to a Pin.
            *   @return bool value ~ Returns True if the pins can make a connection.
            */
            bool CanCreateLink(Pin* a, Pin* b);

            /**
            *   Function searches for a editor node with this pin id in a_search_node_vector.
            *   @param int a_output_pin_id The pin id that you want to search for.
            *   @param std::vector<iga::bt::EditorNode*> a_search_node_vector Vector of EditorNodes that you want to loop over.
            *   @return EditorNode Pointer of the found EditorNode. If it cannot be found, nullptr gets returned.
            */
            EditorNode* FindEditorNodeWithOutputPin(int a_output_pin_id, std::vector<EditorNode*>* a_search_node_vector);

            /**
            *   Function searches for a editor node with this pin id in a_search_node_vector.
            *   @param int a_output_pin_id The pin id that you want to search for.
            *   @param std::vector<iga::bt::EditorNode*> a_search_node_vector Vector of EditorNodes that you want to loop over.
            *   @return EditorNode Pointer of the found EditorNode. If it cannot be found, nullptr gets returned.
            */
            EditorNode* FindEditorNodeWithInputPin(int a_input_pin_id, std::vector<EditorNode*>* a_search_node_vector);

            /**
            *   Function searches for a link(in a of links) with this output pin iID
            *   The link vector, as the parameter, will contain the link(s) found.
            *   @param int a_output_pin_id The output pin id of that you want to search for.
            *   @param std::vector<iga::bte::Link*> a_search_link_vector Vector of links that you find to loop over.
            *   @param std::vector<iga::bte::Link*> a_returned_link_vector Container vector.
            */
            void FindLinksWithInputPinID(int a_input_pin_id, std::vector<iga::bte::Link*>* a_search_link_vector, std::vector<iga::bte::Link*>* a_returned_link_vector);

            /**
            *   Function searches for a link(in a of links vector) with this input pin ID.
            *   The link vector, as the parameter, will contain the link(s) found.
            *   @param int a_input_pin_id The input pin id of that you want to search for.
            *   @param std::vector<iga::bte::Link*> a_search_link_vector Vector of links that you find to loop over.
            *   @param std::vector<iga::bte::Link*> a_returned_link_vector Container vector.
            */
            void FindLinksWithOutputPinID(int a_output_pin_id, std::vector<iga::bte::Link*>* a_search_link_vector, std::vector<iga::bte::Link*>* a_returned_link_vector);

        private:

            BehaviourTreeEditor* bt_editor; /** Pointer to the editor. */

        };

    } // End of namespace ~ bte

} // End of namespace ~ iga