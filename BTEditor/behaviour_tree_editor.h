#pragma once

#include <defines.h>

#ifdef EDITOR

#include <Editor/editor_window.h> // Includes IMGUI.h

#include <Editor/BTEditor/bt_editor_finder.h>
#include <Editor/BTEditor/bt_editor_enums.h>

#include <vector>
#include <map>

/**
* @file behaviour_tree_editor.h
* @author Ben Landor
* @author Channing Eggers
* @date 08 mar 2018
* @brief iga::bte::BehaviourTreeEditor
*
* iga::bte::BehaviourTreeEditor extends EditorWindow and contains the logic for showing the BehaviourTreeEditor Window.
*/

namespace ax {
    namespace NodeEditor {
        struct EditorContext;
    }
}

/**
*  IGArt namespace
*/
namespace iga {

    /**
    *   Behaviour Tree Editor namspace
    */
    namespace bte {

        /**
        *   Returns a new unsigned integer value for a node.
        *   @return unsigned int.
        */
        unsigned int GetNextID();

        static unsigned int node_id_counter_; /** Static integer that keeps track of the IDs that have been given out. */
        
        struct EditorNode;
        struct Link;

        /**
        * iga::BehaviourTreeEditor extends EditorWindow and contains the logic for showing the BehaviourTreeEditor Window.
        */
        class BehaviourTreeEditor final : public EditorWindow {
        public:
            friend class BTEditorFinder;
            friend class BaseNode;
            friend struct EditorNode;

            /**
            *   BehaviourTreeEditor Custom Constructor.
            *   Constructs; BTEditorFinder.
            */
            BehaviourTreeEditor::BehaviourTreeEditor();

            /**
            *   BehaviourTreeEditor Destructor.
            */
            BehaviourTreeEditor::~BehaviourTreeEditor();

            /**
            *  Initializes iga::BehaviourTreeEditor.
            */
            void OnCreate() override;
            /**
            *  Updates the iga::BehaviourTreeEditor and shows the Window.
            */
            void Update() override;

            /**
            *  Overriden from EditorWindow function called when the window is about to open.
            */
            void OnOpen() override;
            /**
            *  Overriden from EditorWindow function called when the window is about to close.
            */
            void OnClose() override;

        private:

            /**
            *   Displays the left pane of the behaviour tree editor window.
            */
            void DisplayLeftPane(float a_pane_width);

            /**
            *   Displays the menubar of the behaviour tree editor window.
            */
            void DisplayMenuBar();

            /**
            *   Returns an integer value representing the order that the node gets called by its parent.
            *   @param EditorNode a_editor_node A Pointer to the editor node.
            *   @return int Call order value.
            */
            int GetNodeOrderFromParent(EditorNode* a_editor_node);

#pragma region User Editor Functionality 

            /**
            *   Clears the current nodes in the editor and sets the editor into a blank state.
            */
            void NewBTFile();

            /**
            *   Opens a BT File and loads it in the editor.
            */
            void OpenBTFile();

            /**
            *   Saves the current nodes that are active to a BT File.
            */
            void SaveBTFile();

            /**
            *   Closes the BehaviourTree Editor window.
            */
            void ExitBTEditor();

            /**
            *   Sets the zoom of the editor to tis default state.
            */
            void SetEditorDefaultZoom();

            /**
            *   Completly clears the editor.
            */
            void ClearEditor();

            /**
            *   Swaps the order for composite nodes.
            */
            void SwapCallOrder(std::vector<int>* a_call_order_vector, int a_current_index, int a_new_index);

            /**
            *   Inspect Sequence Node
            */
            void InspectSelectedNode();

#pragma endregion End of User Editor Functionality

#pragma region Touch Functions

            /** 
            *   Sets the touch time of the node to the touch_time.
            *   @param int a_id The ID of the node.
            */
            void TouchNode(int a_id);

            /**
            *   Tries to find and returns the touch time of the node with this ID.
            *   @return int float The touch progress of the node.
            */
            float GetTouchProgress(int a_id);

            /**
            *   Loops over the nodes in the map and updated the nodes' touch time.
            */
            void UpdateTouch();

#pragma endregion End of Touch Functions Region

#pragma region Node Builder

            /**
            *   Function builds the node that is passed through.
            */
            void BuildNode(EditorNode* a_node);

            /**
            *   Loops over the all of the nodes that the editor owns
            *   and builds them all.
            */
            void BuildNodes();

#pragma endregion End of Node Builder Region

#pragma region Behaviour Tree Draw Functions

            /**
            *   Splits a size/window into two parts.
            *   @param int a_split_vertically
            *   @param float a_thickness The space between the two parts.
            *   @param float* a_size0 The maximum size of the first part.
            *   @param float* a_size0 The maximum size of the second part.
            *   @param float* a_min_size0 The mininum size of the first part.
            *   @param float* a_min_size1 The mininum size of the second part.
            */
            void DrawSplitter(int a_split_vertically, float a_thickness, float* a_size0, float* a_size1, float a_min_size0, float a_min_size1);

            /**
            *   Returns the colour values for a pin.
            *   @param PinType a_type The type of the pin.
            *   @return ImColor Colour of the PinType.
            */
            ImColor GetIconColor(PinType a_type);

            /**
            *   Draws the Pin's of the nodes in the editor.
            *   @param a_pin References to the pin.
            *   @param bool a_connected specifies if a pin is connected with another pin.
            *   @param int  a_alpha The alpha for the pin its colour.
            */
            void DrawPinIcon(const Pin& a_pin, bool a_connected, int a_alpha);

#pragma endregion Behaviour Tree Draw Functions Region

            ax::NodeEditor::EditorContext* editor_context_;     /**< Editor Context. */
            BTEditorFinder editor_finder_;                      /**< Editor Finder. */

            std::vector<EditorNode*> editor_nodes_;             /**< Vector of nodes. */
            std::vector<Link*> editor_links_;                   /**< Vector of Links. */
            EditorNode* inspector_node_             = nullptr;  /**< EditorNode selected in the editor. */

            const int pin_icon_size_                = 24;       /**< Size of the icons. */
            //ImTextureID header_background_        = nullptr;  /**< IMTextureID pointer to the header background. */
            //ImTextureID save_icon_                = nullptr;  /**< IMTextureID pointer to the save icon. */
            //ImTextureID restore_icon_             = nullptr;  /**< IMTextureID pointer to the restore icon. */
            
            char bt_path_buffer_[256]               = "*name*"; /**< A char buffer for the name of the behaviour tree. */
            std::string bt_directory_name_          = "btfiles";/**< Name of the folder */
            std::string bt_file_extensions_         = ".bt";    /**< Name of the folder */

            int change_count_                       = 0;        /**< Counter for the amount of changes happening in the BT Editor. */
            float left_pane_width_                  = 250.0f;   /**< The width of the Left Pane window of the BT Editor. */
            float right_pane_width_                 = 750.0f;   /**< The width of the Right/Main window of the BT Editor. */
            bool window_collapsed_                  = false;    /**< Boolean for the ImGui Window */

            const float rounding_                   = 2.5f;     /**< The roundness factor for the editor nodes. */
            const float padding_                    = 12.0f;    /**< The amount of padding it adds to the editor nodes. */

            const float touch_time_                 = 1.0f;     /**< The touch time of nodes. */
            std::map<int, float> node_touch_time_;              /**< Map containing touch times of nodes.  */
            
        };

    } // End of namespace ~ bte

} // End of namespace ~ iga

#endif // EDITOR