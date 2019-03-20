#include <AI\BT\behaviour_component.h>

#include <ig_art_engine.h>

#include <AI\BT\behaviour_tree.h>
#include <AI/BT/bt_builder.h>

#ifdef EDITOR
#include <imgui/include/imgui.h>
#include <Utility/win_util.h>
#include <iga_util.h>
#endif


/**
*  igart namespace
*/
namespace iga {

    /**
    *  BT namespace
    */
    namespace bt {

            BehaviourComponent::BehaviourComponent() : 
                Component(std::weak_ptr<iga::Entity>())
            {
                this->component_type_       = "BehaviourComponent";
                this->execute_behaviour_    = true;
            }

            BehaviourComponent::BehaviourComponent(std::weak_ptr<iga::Entity> a_entity) :
                Component(a_entity)
            {
                this->component_type_       = "BehaviourComponent";
                this->execute_behaviour_    = true;
            }

            BehaviourComponent::~BehaviourComponent() {
                if (behaviour_tree_ != nullptr) {
                    delete this->behaviour_tree_;
                }
            }

            void BehaviourComponent::OnCreate(bool /*a_on_load*/) {

            }

            void BehaviourComponent::Update() {
                if (GetGame()->IsPlaying()) {
                    if (this->behaviour_tree_ == nullptr && this->bt_file_path_ != "") {
                        BuildBehaviourTree(false);
                    }

                    if (this->execute_behaviour_ && behaviour_tree_ != nullptr) {
                        this->behaviour_tree_->ExecuteTree();
                    }
                }
            }

            void BehaviourComponent::PostUpdate() {
                /* EMPTY */
            }

            void BehaviourComponent::Inspect() {
#ifdef EDITOR
                // Get path from windows explorer
                if (ImGui::Button("Clear")) {
                    bt_file_path_ = "";
                    if (this->behaviour_tree_ != nullptr) { delete this->behaviour_tree_; }
                }

                ImGui::SameLine();

                // Get path from windows explorer
                if (ImGui::Button("Open File")) {
                    HRESULT r = winutil::BasicOpenFile(bt_file_path_);
                    if (r == HRESULT_FROM_WIN32(ERROR_CANCELLED)) { return; }
                    // TODO: Resolve E_UNEXPECTED when it happends. It is now exluded for DEBUG_ASSERT.
                    if (FAILED(r != E_UNEXPECTED)) { 
                        DEBUGASSERT(false); 
                    }

                    if (bt_file_path_ != "") {
                        // Make full path a relative one.
                        std::string wd;
                        iga::GetWorkDirectory(wd);
                        wd.append(DIR_SLASH);
                        wd.append(FILEPATHPREFIX);
                        if (!iga::SubtractString(wd, bt_file_path_)) {
                            std::cout << "ERROR - Behaviour Tree System - Failed to obtain the file path of this BT file. " << std::endl;
                            bt_file_path_.clear();
                        }
                    }
                }

                ImGui::SameLine();

                ImGui::Text(iga::GetFileStem(bt_file_path_).c_str());

                ImGui::Checkbox("Enable/Disable BT", &this->execute_behaviour_);

                // TODO: ImGui Button - Debug "Compile" Tree

                // TODO: ImGui Toggle - Active Debug Tree.
#endif
            }

            void BehaviourComponent::BuildBehaviourTree(bool a_overwrite_bt) {
                BTBuilder builder;
#ifdef DEBUG
                DEBUGASSERT(builder.BuildTree(bt_file_path_, this->behaviour_tree_, this->GetOwner(), a_overwrite_bt));
#else
                builder.BuildTree(bt_file_path_, this->behaviour_tree_, this->GetOwner(), a_overwrite_bt);
#endif
            }

            void BehaviourComponent::RunTree() {
                this->execute_behaviour_ = true;
            }

            void BehaviourComponent::StopTree() {
                this->execute_behaviour_ = false;
            }

    } // End of namespace ~ bt

} // End of namespace ~ iga