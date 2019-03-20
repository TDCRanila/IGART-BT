#include <AI/BT/bt_builder.h>

#include <AI/bt_include.h>

#include <Editor/BTEditor/bt_editor_finder.h>

#include <Editor/BTEditor/bt_editor_node_library.h>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

#include <fstream>

namespace iga {

    namespace bt {
    
        BTBuilder::BTBuilder()
        {
            // DO NOTHING
        }

        BTBuilder::~BTBuilder() {
            // DO NOTHING
        }

        bool BTBuilder::BuildTree(std::string a_bt_file, BehaviourTree*& a_bt, std::weak_ptr<Entity> a_entity, bool a_overwrite_tree) {

            // Add asset folder prefix
            a_bt_file = (FILEPATHPREFIX + a_bt_file);

//TODO : ADD OTHER SUPPORT
#ifdef SN_TARGET_PSP2
            std::string win_dir("\\");
            std::size_t found = a_bt_file.find_first_of(win_dir);
            while (found != std::string::npos) {
                a_bt_file[found] = '/';
                found = a_bt_file.find_first_of(win_dir, found + 1);
            }
#endif

            // Check if we have an empty behaviour tree
            if (a_overwrite_tree) {
                if ( a_bt != nullptr) { delete a_bt; }
                a_bt = new BehaviourTree(a_entity);
            } else {
                if ( a_bt != nullptr) { 
                    DEBUGASSERT(false); // Overwritting existing behaviour tree. 
                } else {
                    a_bt = new BehaviourTree(a_entity);
                }
            }

            // Helper
            iga::bte::BTEditorFinder editor_finder;

            // Load in the file.
            std::ifstream is(a_bt_file, std::ios::binary);
            cereal::BinaryInputArchive iarchive(is);

            iga::bte::EditorSaveStruct loaded_save_struct;
            iarchive(loaded_save_struct); // Read the data from the archive

            // Temp Vectors
            std::vector<iga::bte::Link*> link_pointer_vector;
            for (auto& link : loaded_save_struct.behaviour_tree_links) {
                link_pointer_vector.emplace_back(&link);
            }
            std::vector<iga::bte::EditorNode*> node_pointer_vector;
            for (auto& node : loaded_save_struct.behaviour_tree_editor_nodes) {
                node_pointer_vector.emplace_back(&node);
            }

            // Create all the nodes
            if (StaticNodeLibrary::node_library_map_->empty()) { return false; }
                
            std::map<std::string, nodedetail::objectConstructLambda>::const_iterator library_it;

            for (auto& archived_node : node_pointer_vector) {
                // Try to find item with this id.
                library_it = StaticNodeLibrary::node_library_map_->find(archived_node->GetName());

                if (library_it != StaticNodeLibrary::node_library_map_->end()) {
                    BaseNode* new_base_node = archived_node->saving_base_node->Clone();
                    DEBUGASSERT(new_base_node); // Invalid Clone ~  Most likely no implemented Clone function in derived class.

                    new_base_node->SetNodeName(archived_node->GetName());
                    new_base_node->behaviour_tree_ = a_bt;
                    new_base_node->OnBehaviourTreeBuild();
                    a_bt->TrackNode(new_base_node);

                    // Give temporary pointer to this base node, so we can construct the behaviour tree.
                    archived_node->base_node = new_base_node;

                } else {
                    delete a_bt;
                    DEBUGASSERT(false); // ERROR - Node not found in StaticNodeLibrary.
                }
            }

            // Now Link all the nodes together.
            for (auto& editor_node : node_pointer_vector) {

                // Cast for Leaf Nodes.
                if (LeafNode* leaf_node = dynamic_cast<LeafNode*>(editor_node->base_node)) {
                    continue;
                }

                // Cast for Composite Nodes.
                if (CompositeNode* comp_node = dynamic_cast<CompositeNode*>(editor_node->base_node)) {

                    for (auto& linkid : editor_node->outgoing_link_IDs) {
                        iga::bte::Link* l = editor_finder.FindLink(linkid, &link_pointer_vector);
                        iga::bte::EditorNode* e = editor_finder.FindEditorNodeWithInputPin(l->end_pin_id, &node_pointer_vector);

                        for (auto& node : node_pointer_vector) {
                            if (node == e) {
                                comp_node->AddChild(node->base_node);
                            }
                        }
                    }

                    continue;
                }

                // Cast for Root Node.
                if (Root* root_node = dynamic_cast<Root*>(editor_node->base_node)) {
                    // Root found now set it.
                    a_bt->SetRoot(editor_node->base_node);

                    // Find the child of the root node and set it.
                    std::vector<iga::bte::Link*> links;
                    editor_finder.FindLinksWithOutputPinID(editor_node->outputs[0].id, &link_pointer_vector, &links);

                    iga::bte::EditorNode* e = editor_finder.FindEditorNodeWithInputPin(links[0]->end_pin_id, &node_pointer_vector);

                    for (auto& node : node_pointer_vector) {
                        if (node == e) {
                            root_node->SetChild(node->base_node);
                        }
                    }

                    continue;
                }

                // Cast for Decorator Nodes.
                if (DecoratorNode* deco_node = dynamic_cast<DecoratorNode*>(editor_node->base_node)) {
                    // Find the child of the root node and set it as the root of the behaviour tree.
                    std::vector<iga::bte::Link*> links;
                    editor_finder.FindLinksWithOutputPinID(editor_node->outputs[0].id, &link_pointer_vector, &links);

                    iga::bte::EditorNode* e = editor_finder.FindEditorNodeWithInputPin(links[0]->end_pin_id, &node_pointer_vector);

                    for (auto& node : node_pointer_vector) {
                        if (node == e) {
                            deco_node->SetChild(node->base_node);
                        }
                    }
                }
            }

            // If creation of the behaviour tree was succesful. Return True.
            return true;
        }

    } // End of namespace ~ bt

} // End of namespace ~ iga