#include <Editor/BTEditor/bt_editor_finder.h>

#include <defines.h>

#include <Editor/BTEditor/behaviour_tree_editor.h>
#include <Editor/BTEditor/bt_editor_structs.h>

#include <AI/BT/Nodes/base_node.h>

/**
*   IGArt namespace
*/
namespace iga {

    /**
    *   Behaviour Tree Editor namspace
    */
    namespace bte {

        BTEditorFinder::BTEditorFinder() {
            /* EMPTY */
        }

        BTEditorFinder::BTEditorFinder(BehaviourTreeEditor* a_bt_editor) {
#ifndef EDITOR
            UNUSED(a_bt_editor); // Unused variable for RELEASE Build.
#else
            // Save a pointer to the BTEditor.
            DEBUGASSERT(a_bt_editor);
            this->bt_editor = a_bt_editor;
#endif //!Editor
        }

        BTEditorFinder::~BTEditorFinder() {
            /* EMPTY */
        }

        EditorNode* BTEditorFinder::FindNode(int a_id) {
#ifndef EDITOR
            UNUSED(a_id); // Unused variable for RELEASE Build.
#else
            for (auto& node : bt_editor->editor_nodes_) {
                if (node->id == a_id) {
                    return node;
                }
            }
#endif //!Editor
            return nullptr;
        }

        EditorNode* BTEditorFinder::FindNodeWithPinID(int a_pin_id) {
#ifndef EDITOR
            UNUSED(a_pin_id); // Unused variable for RELEASE Build.
#else
            for (auto& node : bt_editor->editor_nodes_) {
                for (auto& input : node->inputs) {
                    if (input.id == a_pin_id) { return node; }
                }
                for (auto& output : node->outputs) {
                    if (output.id == a_pin_id) { return node; }
                }
            }
#endif //!Editor
            return nullptr;
        }

        Link* BTEditorFinder::FindLink(int a_id) {
#ifndef EDITOR
            UNUSED(a_id); // Unused variable for RELEASE Build.
#else
            for (auto& link : bt_editor->editor_links_) {
                if (link->id == a_id) {
                    return link;
                }
            }
#endif //!Editor
            return nullptr;
        }

        Link* BTEditorFinder::FindLink(int a_id, std::vector<Link*>* a_search_link_vector) {
            for (auto& link : *a_search_link_vector) {
                if (link->id == a_id) {
                    return link;
                }
            }

            return nullptr;
        }

        Pin* BTEditorFinder::FindPin(int a_id) {
#ifndef EDITOR
            UNUSED(a_id); // Unused variable for RELEASE Build.
#else
            if (a_id <= 0) { return nullptr; }

            for (auto& node : bt_editor->editor_nodes_) {
                for (auto& pin : node->inputs) {
                    if (pin.id == a_id) { return &pin; }
                }

                for (auto& pin : node->outputs) {
                    if (pin.id == a_id) { return &pin; }
                }
            }
#endif //!Editor
            return nullptr;
        }

        bool BTEditorFinder::IsPinLinked(int a_id) {
#ifndef EDITOR
            UNUSED(a_id); // Unused variable for RELEASE Build.
#else
            if (a_id <= 0) {
                return false;
            }

            for (auto& link : bt_editor->editor_links_) {
                if (link->start_pin_id == a_id || link->end_pin_id == a_id) {
                    return true;
                }
            }
#endif //!Editor
            return false;
        }

        bool BTEditorFinder::CanCreateLink(Pin* a, Pin* b)
        {
            if (!a || !b || a == b || a->kind == b->kind || a->type != b->type || a->node == b->node) {
                return false;
            }

            return true;
        }

        EditorNode* BTEditorFinder::FindEditorNodeWithOutputPin(int a_output_pin_id, std::vector<EditorNode*>* a_search_node_vector) {
            for (auto*& node : *a_search_node_vector) {
                if (node->outputs.empty()) { continue; }
                if (node->outputs[0].id == a_output_pin_id) {
                    return node;
                }
            }
            return nullptr;
        }

        EditorNode* BTEditorFinder::FindEditorNodeWithInputPin(int a_input_pin_id, std::vector<EditorNode*>* a_search_node_vector) {
            for (auto*& node : *a_search_node_vector) {
                if (node->inputs.empty()) { continue; }
                if (node->inputs[0].id == a_input_pin_id) {
                    return node;
                }
            }
            return nullptr;
        }

        void BTEditorFinder::FindLinksWithInputPinID(int a_input_pin_id, std::vector<iga::bte::Link*>* a_search_link_vector, std::vector<iga::bte::Link*>* a_returned_link_vector) {
            for (iga::bte::Link*& link : *a_search_link_vector) {
                if (link->end_pin_id == a_input_pin_id) {
                    a_returned_link_vector->push_back(link);
                }
            }
        }

        void BTEditorFinder::FindLinksWithOutputPinID(int a_output_pin_id, std::vector<iga::bte::Link*>* a_search_link_vector, std::vector<iga::bte::Link*>* a_returned_link_vector) {
            for (iga::bte::Link*& link : *a_search_link_vector) {
                if (link->start_pin_id == a_output_pin_id) {
                    a_returned_link_vector->push_back(link);
                }
            }
        }

    } // End of namespace ~ bte

} // End of namespace ~ iga