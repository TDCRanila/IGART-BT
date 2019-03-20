#include <Editor/BTEditor/behaviour_tree_editor.h>

#ifdef EDITOR
#include <iga_util.h>

#include <Utility/win_util.h>

#include <AI/bt_include.h>

#include <Editor/editor.h>
#include <Editor/BTEditor/bt_editor_structs.h>

#include <NodeEditor/BaseEditor.h>
#include <NodeEditor/Shared/Math2D.h>
#include <NodeEditor/Shared/Interop.h>
#include <NodeEditor/Shared/Builders.h>
#include <NodeEditor/Shared/Widgets.h>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

#include <string>
#include <algorithm>
#include <utility>

namespace ed = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

namespace iga {

    namespace bte {

        unsigned int GetNextID() {
            return node_id_counter_++;
        }

        iga::bte::BehaviourTreeEditor::BehaviourTreeEditor() :
            editor_finder_(this)
        {
            // Set ID counter to 1.
            node_id_counter_ = 1;
        }

        BehaviourTreeEditor::~BehaviourTreeEditor() {
            ClearEditor();
            ed::DestroyEditor(editor_context_);
        }

        void iga::bte::BehaviourTreeEditor::OnCreate() {
            ed::Config config;
            config.SettingsFile = "Blueprints.json";

            config.LoadNodeSettings = [this](int nodeId, char* data, void* /*userPointer*/) -> size_t
            {
                auto node = this->editor_finder_.FindNode(nodeId);
                if (!node)
                    return 0;

                if (data != nullptr)
                    memcpy(data, node->state.data(), node->state.size());
                return node->state.size();
            };

            config.SaveNodeSettings = [this](int nodeId, const char* data, size_t size, ed::SaveReasonFlags /*reason*/, void* /*userPointer*/) -> bool
            {
                auto node = this->editor_finder_.FindNode(nodeId);
                if (!node)
                    return false;

                node->state.assign(data, size);

                this->TouchNode(nodeId);

                return true;
            };

            editor_context_ = ed::CreateEditor(&config);
            ed::SetCurrentEditor(editor_context_);

            SetEditorDefaultZoom();
        }

        void iga::bte::BehaviourTreeEditor::Update() {

            if (!ImGui::Begin("Behaviour Tree Editor", &this->is_open_, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar | 
                                                                        ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar)) 
            {
                ImGui::End(); // End the Update earlier when window is collapsed.
                return;
            }

            // Make sure that the systems knows that this window is focussed.
            editor_->SubWindowFocused();

            // Set the Size & Pos when it appears for the first time for the user.
            ImGui::SetWindowSize(ImVec2(1200, 700), ImGuiSetCond_::ImGuiSetCond_Appearing);
            ImGui::SetWindowPos(ImVec2(150, 50), ImGuiSetCond_::ImGuiSetCond_Appearing);

            UpdateTouch();

            ImGui::Text("Current Zoom Level: %s", std::to_string(ed::GetCurrentZoom()).c_str());

            static int  contextId       = 0;
            static bool createNewNode   = false;
            static Pin* newNodeLinkPin  = nullptr;
            static Pin* newLinkPin      = nullptr;

            ImGui::Spacing();

            ImGui::PushID("menu_bar");
            DisplayMenuBar();
            ImGui::PopID();

            DrawSplitter(0, 4.0f, &left_pane_width_, &right_pane_width_, 50.0f, 50.0f);

            ImGui::PushID("left_pane_width");
            DisplayLeftPane(left_pane_width_);
            ImGui::PopID();

            ImGui::SameLine();

            ed::Begin("Behaviour Tree Editor");
            {
                auto cursorTopLeft = ImGui::GetCursorScreenPos();

                //util::BlueprintNodeBuilder builder(header_background_, Application_GetTextureWidth(s_HeaderBackground), Application_GetTextureHeight(header_background_));
                util::BlueprintNodeBuilder builder;

                for (auto& node : editor_nodes_)
                {
                    if (node->type != NodeType::SIMPLE) {
                        continue;
                    }

                    const auto isSimple = node->type == NodeType::SIMPLE;

                    builder.Begin(node->id);
                    if (!isSimple)
                    {
                        Vector4 n = node->color;
                        builder.Header(ImColor(n.getX(), n.getY(), n.getZ()));
                        ImGui::Spring(0);
                        ImGui::TextUnformatted(node->name.c_str());
                        ImGui::Spring(1);
                        ImGui::Dummy(ImVec2(0, 28));
                        ImGui::Spring(0);
                        builder.EndHeader();
                    }

                    for (auto& input : node->inputs)
                    {
                        auto alpha = ImGui::GetStyle().Alpha;
                        if (newLinkPin && !editor_finder_.CanCreateLink(newLinkPin, &input) && &input != newLinkPin)
                        {
                            alpha = alpha * (48.0f / 255.0f);
                        }

                        builder.Input(input.id);
                        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
                        DrawPinIcon(input, editor_finder_.IsPinLinked(input.id), static_cast<int>((alpha * 255)));
                        ImGui::Spring(0);
                        if (!input.name.empty())
                        {
                            ImGui::TextUnformatted(input.name.c_str());
                            ImGui::Spring(0);
                        }
                        ImGui::PopStyleVar();
                        builder.EndInput();
                    }

                    if (isSimple)
                    {
                        builder.Middle();

                        ImGui::Spring(1, 0);
                        ImGui::TextUnformatted(node->name.c_str());
                        ImGui::Spring(1, 0);
                    }

                    for (auto& output : node->outputs)
                    {
                        auto alpha = ImGui::GetStyle().Alpha;
                        if (newLinkPin && !editor_finder_.CanCreateLink(newLinkPin, &output) && &output != newLinkPin)
                        {
                            alpha = alpha * (48.0f / 255.0f);
                        }

                        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
                        builder.Output(output.id);
                        if (!output.name.empty())
                        {
                            ImGui::Spring(0);
                            ImGui::TextUnformatted(output.name.c_str());
                        }
                        ImGui::Spring(0);
                        DrawPinIcon(output, editor_finder_.IsPinLinked(output.id), static_cast<int>((alpha * 255)));
                        ImGui::PopStyleVar();
                        builder.EndOutput();
                    }

                    builder.End();
                }

                for (auto& node : editor_nodes_)
                {
                    if (node->type != NodeType::TREE) {
                        continue;
                    }

                    const auto pinBackground = ed::GetStyle().Colors[ed::StyleColor_NodeBg];

                    ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(128, 128, 128, 200));
                    ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(32, 32, 32, 200));
                    ed::PushStyleColor(ed::StyleColor_PinRect, ImColor(60, 180, 255, 150));
                    ed::PushStyleColor(ed::StyleColor_PinRectBorder, ImColor(60, 180, 255, 150));

                    ed::PushStyleVar(ed::StyleVar_NodePadding, ImVec4(0, 0, 0, 0));
                    ed::PushStyleVar(ed::StyleVar_NodeRounding, rounding_);
                    ed::PushStyleVar(ed::StyleVar_SourceDirection, ImVec2(0.0f, 1.0f));
                    ed::PushStyleVar(ed::StyleVar_TargetDirection, ImVec2(0.0f, -1.0f));
                    ed::PushStyleVar(ed::StyleVar_LinkStrength, 0.0f);
                    ed::PushStyleVar(ed::StyleVar_PinBorderWidth, 1.0f);
                    ed::PushStyleVar(ed::StyleVar_PinRadius, 5.0f);
                    ed::BeginNode(node->id);

                    ImGui::BeginVertical(node->id);
                    ImGui::BeginHorizontal("inputs");
                    ImGui::Spring(0, padding_ * 2);

                    ax::rect inputsRect;
                    int inputAlpha = 200;
                    if (!node->inputs.empty())
                    {
                        auto& pin = node->inputs[0];
                        ImGui::Dummy(ImVec2(0, padding_));
                        ImGui::Spring(1, 0);
                        inputsRect = ImGui_GetItemRect();

                        ed::PushStyleVar(ed::StyleVar_PinArrowSize, 10.0f);
                        ed::PushStyleVar(ed::StyleVar_PinArrowWidth, 10.0f);
                        ed::PushStyleVar(ed::StyleVar_PinCorners, 12);
                        ed::BeginPin(pin.id, ed::PinKind::Target);
                        ed::PinPivotRect(to_imvec(inputsRect.top_left()), to_imvec(inputsRect.bottom_right()));
                        ed::PinRect(to_imvec(inputsRect.top_left()), to_imvec(inputsRect.bottom_right()));
                        ed::EndPin();
                        ed::PopStyleVar(3);

                        if (newLinkPin && !editor_finder_.CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
                            inputAlpha = static_cast<int>((255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f)));

                    }
                    else 
                    {
                        ImGui::Dummy(ImVec2(0, padding_));
                    }
                    ImGui::Spring(0, padding_ * 2);
                    ImGui::EndHorizontal();

                    ImGui::BeginHorizontal("content_frame");
                    ImGui::Spring(1, padding_);

                    ImGui::BeginVertical("content", ImVec2(0.0f, 0.0f));
                    ImGui::Dummy(ImVec2(160, 0));
                    ImGui::Spring(1);

                    std::string name = node->name;
                    int order_id = GetNodeOrderFromParent(node);
                    if (order_id != -1) {
                        name.append(" - ");
                        name.append(std::to_string(order_id));
                    }

                    ImGui::TextUnformatted(name.c_str());
                    ImGui::Spring(1);
                    ImGui::EndVertical();
                    auto contentRect = ImGui_GetItemRect();

                    ImGui::Spring(1, padding_);
                    ImGui::EndHorizontal();

                    ImGui::BeginHorizontal("outputs");
                    ImGui::Spring(0, padding_ * 2);

                    ax::rect outputsRect;
                    int outputAlpha = 200;
                    if (!node->outputs.empty())
                    {
                        auto& pin = node->outputs[0];
                        ImGui::Dummy(ImVec2(0, padding_));
                        ImGui::Spring(1, 0);
                        outputsRect = ImGui_GetItemRect();

                        ed::PushStyleVar(ed::StyleVar_PinCorners, 3);
                        ed::BeginPin(pin.id, ed::PinKind::Source);
                        ed::PinPivotRect(to_imvec(outputsRect.top_left()), to_imvec(outputsRect.bottom_right()));
                        ed::PinRect(to_imvec(outputsRect.top_left()), to_imvec(outputsRect.bottom_right()));
                        ed::EndPin();
                        ed::PopStyleVar();

                        if (newLinkPin && !editor_finder_.CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
                            outputAlpha = static_cast<int>((255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f)));
                    }
                    else 
                    {
                        ImGui::Dummy(ImVec2(0, padding_));
                    }

                    ImGui::Spring(0, padding_ * 2);
                    ImGui::EndHorizontal();

                    ImGui::EndVertical();

                    ed::EndNode();
                    ed::PopStyleVar(7);
                    ed::PopStyleColor(4);

                    auto drawList = ed::GetNodeBackgroundDrawList(node->id);

                    const auto fringeScale = ImGui::GetStyle().AntiAliasFringeScale;
                    const auto unitSize    = 1.0f / fringeScale;

                    const auto ImDrawList_AddRect = [](ImDrawList* drawList, const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners, float thickness)
                    {
                        if ((col >> 24) == 0) {
                            return;
                        }

                        drawList->PathRect(a, b, rounding, rounding_corners);
                        drawList->PathStroke(col, true, thickness);
                    };

                    drawList->AddRectFilled(to_imvec(inputsRect.top_left()) + ImVec2(0, 1), to_imvec(inputsRect.bottom_right()),
                        IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, 12);
                    ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
                    drawList->AddRect(to_imvec(inputsRect.top_left()) + ImVec2(0, 1), to_imvec(inputsRect.bottom_right()),
                        IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, 12);
                    ImGui::PopStyleVar();
                    drawList->AddRectFilled(to_imvec(outputsRect.top_left()), to_imvec(outputsRect.bottom_right()) - ImVec2(0, 1),
                        IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, 3);
                    ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
                    drawList->AddRect(to_imvec(outputsRect.top_left()), to_imvec(outputsRect.bottom_right()) - ImVec2(0, 1),
                        IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, 3);
                    ImGui::PopStyleVar();
                    drawList->AddRectFilled(to_imvec(contentRect.top_left()), to_imvec(contentRect.bottom_right()), IM_COL32(24, 64, 128, 200), 0.0f);
                    ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
                    drawList->AddRect(
                        to_imvec(contentRect.top_left()),
                        to_imvec(contentRect.bottom_right()),
                        IM_COL32(48, 128, 255, 100), 0.0f);
                    ImGui::PopStyleVar();
                }

                for (auto& node : editor_nodes_)
                {
                    if (node->type != NodeType::COMMENT)
                    {
                        continue;
                    }

                    const float commentAlpha = 0.75f;

                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, commentAlpha);
                    ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(255, 255, 255, 64));
                    ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(255, 255, 255, 64));
                    ed::BeginNode(node->id);
                    ImGui::BeginVertical("content");
                    ImGui::BeginHorizontal("horizontal");
                    ImGui::Spring(1);
                    ImGui::TextUnformatted(node->name.c_str());
                    ImGui::Spring(1);
                    ImGui::EndHorizontal();
                    ed::Group(ImVec2(node->size.getX(), node->size.getY()));
                    ImGui::EndVertical();
                    ed::EndNode();
                    ed::PopStyleColor(2);
                    ImGui::PopStyleVar();

                    if (ed::BeginGroupHint(node->id))
                    {
                        auto alpha = static_cast<int>(commentAlpha * ImGui::GetStyle().Alpha * 255);

                        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, commentAlpha * ImGui::GetStyle().Alpha);

                        auto min = ed::GetGroupMin();

                        ImGui::SetCursorScreenPos(min - ImVec2(-8, ImGui::GetTextLineHeightWithSpacing() + 4));
                        ImGui::BeginGroup();
                        ImGui::TextUnformatted(node->name.c_str());
                        ImGui::EndGroup();

                        auto drawList = ed::GetHintBackgroundDrawList();

                        auto hintBounds = ImGui_GetItemRect();
                        auto hintFrameBounds = hintBounds.expanded(8, 4);

                        drawList->AddRectFilled(
                            to_imvec(hintFrameBounds.top_left()),
                            to_imvec(hintFrameBounds.bottom_right()),
                            IM_COL32(255, 255, 255, 64 * alpha / 255), 4.0f);

                        drawList->AddRect(
                            to_imvec(hintFrameBounds.top_left()),
                            to_imvec(hintFrameBounds.bottom_right()),
                            IM_COL32(255, 255, 255, 128 * alpha / 255), 4.0f);

                        ImGui::PopStyleVar();
                    }
                    ed::EndGroupHint();
                }

                for (auto& link : editor_links_) {
                    Vector4 l = link->color;
                    ed::Link(link->id, link->start_pin_id, link->end_pin_id, ImColor(l.getX(), l.getY(), l.getZ()), 2.0f);
                }

                if (!createNewNode)
                {
                    if (ed::BeginCreate(ImColor(255, 255, 255), 2.0f))
                    {
                        auto showLabel = [](const char* label, ImColor color)
                        {
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
                            auto size = ImGui::CalcTextSize(label);

                            auto padding = ImGui::GetStyle().FramePadding;
                            auto spacing = ImGui::GetStyle().ItemSpacing;

                            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

                            auto rectMin = ImGui::GetCursorScreenPos() - padding;
                            auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

                            auto drawList = ImGui::GetWindowDrawList();
                            drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
                            ImGui::TextUnformatted(label);
                        };

                        int startPinId = 0, endPinId = 0;
                        if (ed::QueryNewLink(&startPinId, &endPinId))
                        {
                            auto startPin = editor_finder_.FindPin(startPinId);
                            auto endPin = editor_finder_.FindPin(endPinId);

                            newLinkPin = startPin ? startPin : endPin;

                            if (startPin->kind == PinKind::TARGET)
                            {
                                std::swap(startPin, endPin);
                                std::swap(startPinId, endPinId);
                            }

                            if (startPin && endPin)
                            {
                                if (endPin == startPin)
                                {
                                    ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
                                }
                                else if (endPin->kind == startPin->kind)
                                {
                                    showLabel("x Incompatible Pin Kind", ImColor(45, 32, 32, 180));
                                    ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
                                }
                                else if (endPin->node == startPin->node)
                                {
                                    showLabel("x Cannot connect to self", ImColor(45, 32, 32, 180));
                                    ed::RejectNewItem(ImColor(255, 0, 0), 1.0f);
                                }
                                else if (endPin->type != startPin->type)
                                {
                                    showLabel("x Incompatible Pin Type", ImColor(45, 32, 32, 180));
                                    ed::RejectNewItem(ImColor(255, 128, 128), 1.0f);
                                }
                                else
                                {
                                    showLabel("+ Create Link", ImColor(32, 45, 32, 180));
                                    if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
                                    {
                                        // Create the link
                                        Link* new_link = new Link(GetNextID(), startPin->id, endPin->id);
                                        ImColor col = GetIconColor(startPin->type);
                                        new_link->color = Vector4(col.Value.x, col.Value.y, col.Value.z, col.Value.w);
                                        editor_links_.emplace_back(new_link);

                                        // Get Node with this pin.
                                        editor_finder_.FindNodeWithPinID(startPin->id)->outgoing_link_IDs.push_back(new_link->id);
                                        editor_finder_.FindNodeWithPinID(endPin->id)->receiving_link_IDs.push_back(new_link->id);
                                    }
                                }
                            }
                        }

                        int pinId = 0;
                        if (ed::QueryNewNode(&pinId))
                        {
                            newLinkPin = editor_finder_.FindPin(pinId);
                            if (newLinkPin) 
                            {
                                showLabel("+ Create Node", ImColor(32, 45, 32, 180));
                            }
                              
                            if (ed::AcceptNewItem())
                            {
                                createNewNode   = true;
                                newNodeLinkPin  = editor_finder_.FindPin(pinId);
                                newLinkPin      = nullptr;
                                ImGui::OpenPopup("Create New Node");
                            }
                        }
                    }
                    else
                    {
                        newLinkPin = nullptr;
                    }

                    ed::EndCreate();

                    if (ed::BeginDelete())
                    {
                        int linkId = 0;
                        while (ed::QueryDeletedLink(&linkId))
                        {
                            if (ed::AcceptDeletedItem())
                            {
                                auto id = std::find_if(editor_links_.begin(), editor_links_.end(), [linkId](auto& link) { return link->id == linkId; });
                                if (id != editor_links_.end()) 
                                {
                                    // Go over the nodes and search which nodes have this link id. Delete them if present.
                                    for (auto& node : editor_nodes_) {
                                        for (int i = 0; i < node->outgoing_link_IDs.size(); ++i) {
                                            if (node->outgoing_link_IDs[i] == linkId) {
                                                node->outgoing_link_IDs.erase((node->outgoing_link_IDs.begin() + i));
                                                break;
                                            }
                                        }
                                        for (int i = 0; i < node->receiving_link_IDs.size(); ++i) {
                                            if (node->receiving_link_IDs[i] == linkId) {
                                                node->receiving_link_IDs.erase((node->receiving_link_IDs.begin() + i));
                                            }
                                        }
                                    }

                                    // Delete Link
                                    Link* deleted_link = editor_finder_.FindLink(linkId);
                                    delete deleted_link;
                                    editor_links_.erase(id);
                                }
                            }
                        }

                        int nodeId = 0;
                        while (ed::QueryDeletedNode(&nodeId))
                        {
                            if (ed::AcceptDeletedItem())
                            {
                                auto id = std::find_if(editor_nodes_.begin(), editor_nodes_.end(), [nodeId](auto& node) { return node->id == nodeId; });
                                if (id != editor_nodes_.end()) 
                                {
                                    EditorNode* deleted_node = editor_finder_.FindNode(nodeId);
                                    if (this->inspector_node_ == deleted_node) { 
                                        this->inspector_node_ = nullptr;
                                    }
                                    delete deleted_node;
                                    editor_nodes_.erase(id);
                                }
                            }
                        }
                    }
                    ed::EndDelete();
                }

                ImGui::SetCursorScreenPos(cursorTopLeft);
            }

            if (ed::ShowNodeContextMenu(&contextId)) 
            {
                ImGui::OpenPopup("Node Context Menu");
            }
            else if (ed::ShowPinContextMenu(&contextId))
            {
                ImGui::OpenPopup("Pin Context Menu");
            }
            else if (ed::ShowLinkContextMenu(&contextId))
            {
                ImGui::OpenPopup("Link Context Menu");
            }
            else if (ed::ShowBackgroundContextMenu())
            {
                ImGui::OpenPopup("Create New Node");
                newNodeLinkPin = nullptr;
            }

            ed::Suspend();
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
            if (ImGui::BeginPopup("Node Context Menu"))
            {
                auto node = editor_finder_.FindNode(contextId);

                ImGui::TextUnformatted("Node Context Menu");
                ImGui::Separator();
                if (ImGui::MenuItem("Inspect Node")) { inspector_node_ = node; }
                ImGui::Separator();
                if (node)
                {
                    ImGui::Text("ID: %d", node->id);
                    ImGui::Text("Type: %s", "Tree");
                    ImGui::Text("Inputs: %d", static_cast<int>(node->inputs.size()));
                    ImGui::Text("Outputs: %d", static_cast<int>(node->outputs.size()));
                }
                else 
                {
                    ImGui::Text("Unknown node: %d", contextId);
                }

                ImGui::Separator();
                if (ImGui::MenuItem("Delete")) { ed::DeleteNode(contextId); }
                ImGui::EndPopup();
            }

            if (ImGui::BeginPopup("Pin Context Menu"))
            {
                auto pin = editor_finder_.FindPin(contextId);

                ImGui::TextUnformatted("Pin Context Menu");
                ImGui::Separator();
                if (pin)
                {
                    ImGui::Text("ID: %d", pin->id);
                    if (pin->node) 
                    {
                        ImGui::Text("Node: %d", pin->node->id);
                    }
                    else 
                    {
                        ImGui::Text("Node: %s", "<none>");
                    }
                }
                else 
                {
                    ImGui::Text("Unknown pin: %d", contextId);
                }

                ImGui::EndPopup();
            }

            if (ImGui::BeginPopup("Link Context Menu"))
            {
                auto link = editor_finder_.FindLink(contextId);

                ImGui::TextUnformatted("Link Context Menu");
                ImGui::Separator();
                if (link)
                {
                    ImGui::Text("ID: %d", link->id);
                    ImGui::Text("From: %d", link->start_pin_id);
                    ImGui::Text("To: %d", link->end_pin_id);
                }
                else 
                {
                    ImGui::Text("Unknown link: %d", contextId);
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Delete")) { 
                    ed::DeleteLink(contextId); 
                }
                ImGui::EndPopup();
            }

            if (ImGui::BeginPopup("Create New Node"))
            {
                auto newNodePostion = ImGui::GetMousePosOnOpeningCurrentPopup();

                EditorNode* node = nullptr;

                ImGui::Separator();
                if (!StaticNodeLibrary::node_library_map_->empty()) {
                    std::map<std::string, nodedetail::objectConstructLambda>::iterator iter = StaticNodeLibrary::node_library_map_->begin();
                    for (iter; iter != StaticNodeLibrary::node_library_map_->end(); ++iter) {
                        if (ImGui::MenuItem(iter->first.c_str())) {

                            // Allocate EditorNode & BaseNode.
                            node = new EditorNode();
                            iga::bt::BaseNode* new_base_node(iter->second());
                            DEBUGASSERT(new_base_node); // Check if new_node is invalid.

                            new_base_node->ConstructEditorNode(node);
                            new_base_node->SetNodeName(iter->first);
                            node->SetBaseNode(new_base_node);

                            BuildNode(node);

                            break; // Stop the loop.
                        }
                    }
                }
                else 
                {
                    ImGui::Text("NO NODES AVAIABLE.");
                }
                ImGui::Separator();

                if (node)
                {
                    createNewNode = false;

                    ed::SetNodePosition(node->id, newNodePostion);

                    if (auto startPin = newNodeLinkPin)
                    {
                        auto& pins = startPin->kind == PinKind::TARGET ? node->outputs : node->inputs;

                        for (auto& pin : pins)
                        {
                            if (editor_finder_.CanCreateLink(startPin, &pin))
                            {
                                auto endPin = &pin;
                                if (startPin->kind == PinKind::TARGET) {
                                    std::swap(startPin, endPin);
                                }
                                Link* new_link = new Link(GetNextID(), startPin->id, endPin->id);
                                editor_links_.emplace_back(new_link);
                                ImColor col = GetIconColor(startPin->type);
                                editor_links_.back()->color = Vector4(col.Value.x, col.Value.y, col.Value.z, col.Value.w);

                                // Get Node with this pin.
                                editor_finder_.FindNodeWithPinID(startPin->id)->outgoing_link_IDs.push_back(new_link->id);
                                editor_finder_.FindNodeWithPinID(endPin->id)->receiving_link_IDs.push_back(new_link->id);
                                break;
                            }
                        }
                    }
                }

                ImGui::EndPopup();
            }
            else 
            {
                createNewNode = false;
            }

            ImGui::PopStyleVar();
            ed::Resume();

            ed::End();

            ImGui::End();
        }

        void iga::bte::BehaviourTreeEditor::OnOpen() {
            // Clear the editor
            ClearEditor();
        }

        void iga::bte::BehaviourTreeEditor::OnClose() {
            // Clear the editor
            ClearEditor();
        }

        void iga::bte::BehaviourTreeEditor::DisplayLeftPane(float a_pane_width) {
            auto& io = ImGui::GetIO();

            ImGui::BeginChild("EditorInspector", ImVec2(a_pane_width, 0));

            a_pane_width = ImGui::GetContentRegionAvailWidth();

            // Name
            ImGui::InputText("BT Name", bt_path_buffer_, IM_ARRAYSIZE(bt_path_buffer_));

            std::vector<int> selectedNodes, selectedLinks;
            selectedNodes.resize(ed::GetSelectedObjectCount());
            selectedLinks.resize(ed::GetSelectedObjectCount());

            int nodeCount = ed::GetSelectedNodes(selectedNodes.data(), static_cast<int>(selectedNodes.size()));
            int linkCount = ed::GetSelectedLinks(selectedLinks.data(), static_cast<int>(selectedLinks.size()));

            selectedNodes.resize(nodeCount);
            selectedLinks.resize(linkCount);

            //int saveIconWidth       = Application_GetTextureWidth(save_icon_);
            //int saveIconHeight      = Application_GetTextureWidth(save_icon_);
            //int restoreIconWidth    = Application_GetTextureWidth(restore_icon_);
            //int restoreIconHeight   = Application_GetTextureWidth(restore_icon_);

            int saveIconWidth       = 16;
            int saveIconHeight      = 16;
            int restoreIconWidth    = 16;
            int restoreIconHeight   = 16;

#pragma region Node List

            ImGui::GetWindowDrawList()->AddRectFilled(
                ImGui::GetCursorScreenPos(),
                ImGui::GetCursorScreenPos() + ImVec2(a_pane_width, ImGui::GetTextLineHeight()),
                ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]), ImGui::GetTextLineHeight() * 0.225f);
            ImGui::Spacing(); ImGui::SameLine();
            ImGui::TextUnformatted("Nodes");
            ImGui::BeginChild("Nodes", ImVec2(a_pane_width, 100.f), true, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysVerticalScrollbar | 
                                                                          ImGuiWindowFlags_::ImGuiWindowFlags_ShowBorders);
            for (auto node : editor_nodes_)
            {
                ImGui::PushID(node->id);
                auto start = ImGui::GetCursorScreenPos();

                if (const auto progress = GetTouchProgress(node->id))
                {
                    ImGui::GetWindowDrawList()->AddLine(
                        start + ImVec2(-8, 0),
                        start + ImVec2(-8, ImGui::GetTextLineHeight()),
                        IM_COL32( 255, 0, 0, 255 - static_cast<int>((255 * progress))),
                        4.0f);
                }

                bool isSelected = std::find(selectedNodes.begin(), selectedNodes.end(), node->id) != selectedNodes.end();
                if (ImGui::Selectable((node->name + "##" + std::to_string(node->id)).c_str(), &isSelected))
                {
                    if (io.KeyCtrl)
                    {
                        if (isSelected) {
                            ed::SelectNode(node->id, true);
                        }
                        else
                        {
                            ed::DeselectNode(node->id);
                        }
                    }
                    else 
                    {
                        ed::SelectNode(node->id, false);
                    }

                    ed::NavigateToSelection();
                }
                if (ImGui::IsItemHovered() && !node->state.empty())
                    ImGui::SetTooltip("State: %s", node->state.c_str());

                auto id = std::string("(") + std::to_string(node->id) + ")";
                auto textSize = ImGui::CalcTextSize(id.c_str(), nullptr);
                auto iconPanelPos = start + ImVec2(
                    a_pane_width - ImGui::GetStyle().FramePadding.x - ImGui::GetStyle().IndentSpacing - saveIconWidth - restoreIconWidth - ImGui::GetStyle().ItemInnerSpacing.x * 1,
                    (ImGui::GetTextLineHeight() - saveIconHeight) / 2);
                ImGui::GetWindowDrawList()->AddText(
                    ImVec2(iconPanelPos.x - textSize.x - ImGui::GetStyle().ItemInnerSpacing.x, start.y),
                    IM_COL32(255, 255, 255, 255), id.c_str(), nullptr);

                ImGui::SetCursorScreenPos(iconPanelPos);
                ImGui::SetItemAllowOverlap();
                if (node->saved_state.empty())
                {
                    if (ImGui::InvisibleButton("Save", ImVec2(static_cast<float>(saveIconWidth), static_cast<float>(saveIconHeight)))) {
                        node->saved_state = node->state;
                    }

                    //if (ImGui::IsItemActive()) 
                    //{
                    //    drawList->AddImage(save_icon_, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 96));
                    //}
                    //else if (ImGui::IsItemHovered())
                    //{
                    //    drawList->AddImage(save_icon_, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
                    //}
                    //else
                    //{
                    //    drawList->AddImage(save_icon_, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 160));
                    //}
                }
                else
                {
                    ImGui::Dummy(ImVec2(static_cast<float>(saveIconWidth), static_cast<float>(saveIconHeight)));
                    //drawList->AddImage(save_icon_, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 32));
                }

                ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
                ImGui::SetItemAllowOverlap();
                if (!node->saved_state.empty())
                {
                    if (ImGui::InvisibleButton("Restore", ImVec2(static_cast<float>(restoreIconWidth), static_cast<float>(restoreIconHeight))))
                    {
                        node->state = node->saved_state;
                        ed::RestoreNodeState(node->id);
                        node->saved_state.clear();
                    }

                    //if (ImGui::IsItemActive())
                    //{
                    //    drawList->AddImage(restore_icon_, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 96));
                    //}
                    //else if (ImGui::IsItemHovered())
                    //{
                    //    drawList->AddImage(restore_icon_, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
                    //}
                    //else
                    //{
                    //    drawList->AddImage(restore_icon_, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 160));
                    //}
                }
                else
                {
                    ImGui::Dummy(ImVec2(static_cast<float>(restoreIconWidth), static_cast<float>(restoreIconHeight)));
                    //drawList->AddImage(restore_icon_, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 32));
                }

                ImGui::SameLine(0, 0);
                ImGui::SetItemAllowOverlap();
                ImGui::Dummy(ImVec2(0, static_cast<float>(restoreIconHeight)));

                ImGui::PopID();
            }
            ImGui::EndChild();

#pragma endregion 

#pragma region Selection & Util

ImGui::GetWindowDrawList()->AddRectFilled(
    ImGui::GetCursorScreenPos(),
    ImGui::GetCursorScreenPos() + ImVec2(a_pane_width, ImGui::GetTextLineHeight()),
    ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]), ImGui::GetTextLineHeight() * 0.225f);
ImGui::Spacing(); ImGui::SameLine();
ImGui::TextUnformatted("Utility");
float split_button_width = (a_pane_width / 2.0f) - 5.0f;
if (ImGui::Button("Show Tree Flow", ImVec2(split_button_width, 20.0f)) || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Z))) {
    for (auto& link : editor_links_) {
        ed::Flow(link->id);
    }
}
ImGui::SameLine();
if (ImGui::Button("Deselect All", ImVec2(split_button_width, 20.0f))) {
    ed::ClearSelection();
}

ImGui::GetWindowDrawList()->AddRectFilled(
    ImGui::GetCursorScreenPos(),
    ImGui::GetCursorScreenPos() + ImVec2(a_pane_width, ImGui::GetTextLineHeight()),
    ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]), ImGui::GetTextLineHeight() * 0.225f);
ImGui::Spacing(); ImGui::SameLine();
ImGui::TextUnformatted("Selection");
ImGui::BeginHorizontal("Selection", ImVec2(a_pane_width, 0));

ImGui::Text("Changed %d time%s", change_count_, change_count_ > 1 ? "s" : "");

//for (int i = 0; i < nodeCount; ++i) ImGui::Text("Node (%d)", selectedNodes[i]);
//for (int i = 0; i < linkCount; ++i) ImGui::Text("Link (%d)", selectedLinks[i]);

//if (ed::HasSelectionChanged()) { ++change_count_; }

ImGui::EndHorizontal(); // End of Selection Horizontal

#pragma endregion

#pragma region Node Inspector

            ImGui::GetWindowDrawList()->AddRectFilled(
                ImGui::GetCursorScreenPos(),
                ImGui::GetCursorScreenPos() + ImVec2(a_pane_width, ImGui::GetTextLineHeight()),
                ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]), ImGui::GetTextLineHeight() * 0.225f);
            ImGui::Spacing(); ImGui::SameLine();
            ImGui::TextUnformatted("Node Inspector");
            ImGui::BeginChild(1, ImVec2(a_pane_width - 5.0f, 0.0f), true, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysVerticalScrollbar |
                ImGuiWindowFlags_::ImGuiWindowFlags_ShowBorders |
                ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::BeginVertical("NodeInspector", ImVec2(a_pane_width, 0));

            InspectSelectedNode();

            ImGui::EndVertical(); // End of NodeInSpector Horizontal
            ImGui::EndChild();

#pragma endregion

ImGui::EndChild();
        }

        void iga::bte::BehaviourTreeEditor::DisplayMenuBar() {
            ImGui::BeginMenuBar();

            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New BehaviourTree File")) { NewBTFile(); }
                if (ImGui::MenuItem("Open BehaviourTree File")) { OpenBTFile(); }
                if (ImGui::MenuItem("Save BehaviourTree File")) { SaveBTFile(); }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit BehaviourTree Editor")) { ExitBTEditor(); }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Debug")) {
                if (ImGui::MenuItem("Debug BehaviourTree")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View")) {
                if (ImGui::MenuItem("Default Zoom")) { SetEditorDefaultZoom(); }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        int iga::bte::BehaviourTreeEditor::GetNodeOrderFromParent(EditorNode* a_editor_node) {
            // Early exits
            if (a_editor_node->receiving_link_IDs.empty()) { return -1; }

            Link* link          = editor_finder_.FindLink(a_editor_node->receiving_link_IDs[0]);
            EditorNode* parent  = editor_finder_.FindNodeWithPinID(link->start_pin_id);
            if (link == nullptr || parent == nullptr) { return -1; }
            
            // Loop over parent childeren and return the order that a_editor_node is in.
            for (int i = 0; i < parent->outgoing_link_IDs.size(); ++i) {
                if (parent->outgoing_link_IDs[i] == a_editor_node->receiving_link_IDs[0]) {
                    return i;
                }
            }

            return -1;
        }

#pragma region User Editor Functionality 

        void iga::bte::BehaviourTreeEditor::NewBTFile() {

            // Ask if you want to create a new file.

            // Clear the editor
            ClearEditor();
        }

        void iga::bte::BehaviourTreeEditor::OpenBTFile() {

            // Check if losing data by not saving current.

            // Get path from windows explorer.
            std::string file_path;
            HRESULT r = winutil::BasicOpenFile(file_path);
            if (r == HRESULT_FROM_WIN32(ERROR_CANCELLED)) { return; }
            // TODO: Resolve E_UNEXPECTED when it happends. It is now exluded for DEBUG_ASSERT.
            if (FAILED(r != E_UNEXPECTED)) {
                DEBUGASSERT(false);
            }

            // File extension check.
            std::string extension_check = iga::GetFileExtension(file_path);
            if (extension_check != bt_file_extensions_) {
                // ImGui Popup - invalid extension
                return;
            }

            // Start the loading of the file.
            if (fs::exists(file_path)) { 

                // Clear the editor.
                ClearEditor();

                // Read the data from the archive.
                std::ifstream is(file_path, std::ios::binary);
                cereal::BinaryInputArchive iarchive(is);
                EditorSaveStruct loaded_save_struct;
                iarchive(loaded_save_struct);

                // Copy over data to the editor.
                strcpy_s(bt_path_buffer_, loaded_save_struct.behaviour_tree_name.c_str());
                node_id_counter_ = loaded_save_struct.behaviour_tree_id_start;

                for (auto& archive_node : loaded_save_struct.behaviour_tree_editor_nodes) {
                    iga::bte::EditorNode* new_node = new iga::bte::EditorNode(archive_node);
                    new_node->SetBaseNode(archive_node.saving_base_node->Clone());
                    new_node->saving_base_node.reset();
                    new_node->base_node->OnEditorLoad();

                    editor_nodes_.push_back(new_node);
                }

                for (auto& link : loaded_save_struct.behaviour_tree_links) {
                    editor_links_.push_back(new Link(link));
                }

            }

        }

        void iga::bte::BehaviourTreeEditor::SaveBTFile() {

            // Early Exit ~ Check if editor vectors empty.
            if ((this->editor_nodes_.empty()) ||
                (this->editor_links_.empty())
                ) 
            {
                // ImGui Popup - couldn't save.
                return;
            }

            // Get file path and name from windows explorer.
            std::string file_path;
            HRESULT r = winutil::BasicSaveFile(file_path);
            if (r == HRESULT_FROM_WIN32(ERROR_CANCELLED)) { return; }
            if (FAILED(r)) { DEBUGASSERT(false); }
            std::string bt_file_name = iga::GetFileStem(file_path);

            // Check for characters in the file name that are illegal.
            if ((bt_file_name == "")                   ||
                (bt_file_name == "*name*")             ||
                (iga::ContainsChar(bt_file_name, '.')) ||
                (iga::ContainsChar(bt_file_name, ':')) ||
                (iga::ContainsChar(bt_file_name, '*')) ||
                (iga::ContainsChar(bt_file_name, '?')) ||
                (iga::ContainsChar(bt_file_name, '"')) ||
                (iga::ContainsChar(bt_file_name, '<')) ||
                (iga::ContainsChar(bt_file_name, '>')) ||
                (iga::ContainsChar(bt_file_name, '|')) ||
                (iga::ContainsChar(bt_file_name, '/')) ||
                (iga::ContainsChar(bt_file_name, '\\'))
                ) 
            {
                // ImGui Popup - invalid name
                return;
            }

            // File extension check.
            std::string extension_check = iga::GetFileExtension(file_path);
            if (extension_check == ""){
                file_path.append(bt_file_extensions_);
            } else if (extension_check != bt_file_extensions_){
                // ImGui Popup - invalid extension
                return;
            }

            // Check if file/folder exists
            if (!fs::exists(FILEPATHPREFIX) || !fs::exists(FILEPATHPREFIX + bt_directory_name_)) {
                if (!iga::CreateNewDirectory(FILEPATHPREFIX)) { return;  }
                if (!iga::CreateNewDirectory(FILEPATHPREFIX + bt_directory_name_)) { return; }
            }

            // TODO: Check if valid behaviour tree

            // Load the name into the editor.
            strcpy_s(bt_path_buffer_, bt_file_name.c_str());

            // Output Stream Setup
            std::ofstream os(file_path, std::ios::binary);
            cereal::BinaryOutputArchive archive(os);

            // Setup data structs
            EditorSaveStruct my_data;
            my_data.behaviour_tree_id_start  = node_id_counter_;
            my_data.behaviour_tree_name      = bt_file_name;

            for (auto& link : editor_links_) {
                my_data.behaviour_tree_links.push_back(*link);
            }

            for (auto node : editor_nodes_) {
                my_data.behaviour_tree_editor_nodes.emplace_back(*node);
                my_data.behaviour_tree_editor_nodes.back().saving_base_node = node->base_node->CloneToShared();
            }

            // Serialize    
            archive(my_data);

            // Post
            for (auto& node : my_data.behaviour_tree_editor_nodes) {
                node.saving_base_node.reset();
            }

        }

        void iga::bte::BehaviourTreeEditor::ExitBTEditor() {
            // TODO Ask if you want to discard all changes and exit the editor.

            ClearEditor();

            this->is_open_ = false;
        }

        void iga::bte::BehaviourTreeEditor::SetEditorDefaultZoom() {
        }

        void iga::bte::BehaviourTreeEditor::ClearEditor() {
            // Make sure to delete all the nodes and links that were used by the editor.
            for (auto& node : this->editor_nodes_) {
                if (node != nullptr) {
                    delete node;
                    node = nullptr;
                }
            }

            for (auto& link : this->editor_links_) {
                if (link == nullptr) { delete link; }
            }

            // Selected Node
            this->inspector_node_ = nullptr;

            // Now clear the vectors
            this->editor_nodes_.clear();
            this->editor_links_.clear();

            // Clear the NodeEditor stuff
            ed::ClearEditorContext(editor_context_);
            this->inspector_node_ = nullptr;

            // Reset ID Counter
            node_id_counter_ = 1;

            // Reset Name
            strcpy_s(bt_path_buffer_, "");
        }

        void iga::bte::BehaviourTreeEditor::SwapCallOrder(std::vector<int>* a_call_order_vector, int a_current_index, int a_new_index) {
            if ((a_new_index > static_cast<int>((*a_call_order_vector).size() - 1)) || (a_new_index < 0)) { return; }

            std::swap((*a_call_order_vector)[a_current_index], (*a_call_order_vector)[a_new_index]);
        }

        void iga::bte::BehaviourTreeEditor::InspectSelectedNode() {
            
            // Early exit.
            if (this->inspector_node_ == nullptr) { return; }

            ImGui::Separator();

            ImGui::Text(this->inspector_node_->name.c_str());

            ImGui::Separator();

            // Node Re-order
            if (inspector_node_->base_node->GetNodeType() == iga::bt::NodeType::COMPOSITE) {
                for (int node_index = 0; node_index < this->inspector_node_->outgoing_link_IDs.size(); ++node_index) {
                    ImGui::PushID(node_index);
                    Link* l = editor_finder_.FindLink(this->inspector_node_->outgoing_link_IDs[node_index]);
                    if (ImGui::Button("^", ImVec2(17.5f, 17.5f))) {
                        SwapCallOrder(&this->inspector_node_->outgoing_link_IDs, node_index, node_index - 1);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("v", ImVec2(17.5f, 17.5f))) {
                        SwapCallOrder(&this->inspector_node_->outgoing_link_IDs, node_index, node_index + 1);
                    }
                    ImGui::SameLine();
                    ImGui::Text(editor_finder_.FindEditorNodeWithInputPin(l->end_pin_id, &this->editor_nodes_)->name.c_str());
                    ImGui::PopID();
                }
            }

            ImGui::Separator();

            this->inspector_node_->base_node->InspectNode();

            ImGui::Separator();
        }

#pragma endregion End of User Editor Functionality

#pragma region Touch Functions Implementation

        void iga::bte::BehaviourTreeEditor::TouchNode(int a_id) {
            node_touch_time_[a_id] = touch_time_;
        }

        float iga::bte::BehaviourTreeEditor::GetTouchProgress(int a_id) {
            auto it = node_touch_time_.find(a_id);
            if (it != node_touch_time_.end() && it->second > 0.0f) {
                return (touch_time_ - it->second) / touch_time_;
            } else {
                return 0.0f;
            }
        }

        void iga::bte::BehaviourTreeEditor::UpdateTouch() {
            const auto deltaTime = ImGui::GetIO().DeltaTime;
            for (auto& entry : node_touch_time_)
            {
                if (entry.second > 0.0f)
                    entry.second -= deltaTime;
            }
        }

#pragma endregion Touch Node

#pragma region Node Builder Implementation

        void iga::bte::BehaviourTreeEditor::BuildNode(EditorNode* a_node) {

            // ID Retrieval
            a_node->id = GetNextID();

            // Node Colour Setup

            // Setup Pins of the node
            for (auto& input : a_node->inputs)
            {
                input.node              = a_node;
                input.parent_node_id    = a_node->id;
                input.kind              = PinKind::TARGET;
            }

            for (auto& output : a_node->outputs)
            {
                output.node             = a_node;
                output.parent_node_id   = a_node->id;
                output.kind             = PinKind::SOURCE;
            }

            editor_nodes_.push_back(a_node);
        }

        void iga::bte::BehaviourTreeEditor::BuildNodes() {
            for (auto& node : this->editor_nodes_) {
                BuildNode(node);
            }
        }

#pragma endregion

#pragma region Behaviour Tree Draw

        void iga::bte::BehaviourTreeEditor::DrawSplitter(int a_split_vertically, float a_thickness, float* a_size0, float* a_size1, float a_min_size0, float a_min_size1) {
            ImVec2 backup_pos = ImGui::GetCursorPos();
            if (a_split_vertically) {
                ImGui::SetCursorPosY(backup_pos.y + *a_size0);
            } else {
                ImGui::SetCursorPosX(backup_pos.x + *a_size0);
            }

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));          // We don't draw while active/pressed because as we move the panes the splitter button will be 1 frame late
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.6f, 0.6f, 0.10f));
            ImGui::Button("##Splitter", ImVec2(!a_split_vertically ? a_thickness : -1.0f, a_split_vertically ? a_thickness : -1.0f));
            ImGui::PopStyleColor(3);

            ImGui::SetItemAllowOverlap(); // This is to allow having other buttons OVER our splitter.

            if (ImGui::IsItemActive())
            {
                float mouse_delta = a_split_vertically ? ImGui::GetIO().MouseDelta.y : ImGui::GetIO().MouseDelta.x;

                // Minimum pane size
                if (mouse_delta < a_min_size0 - *a_size0) {
                    mouse_delta = a_min_size1 - *a_size0;
                }
                if (mouse_delta > *a_size1 - a_min_size1) {
                    mouse_delta = *a_size1 - a_min_size1;
                }

                // Apply resize
                *a_size0 += mouse_delta;
                *a_size0 -= mouse_delta;
            }
            ImGui::SetCursorPos(backup_pos);
        }

        ImColor iga::bte::BehaviourTreeEditor::GetIconColor(PinType a_type)
        {
            switch (a_type) {
                default:
                case PinType::FLOW:     return ImColor(255, 255, 255);
                case PinType::OBJECT:   return ImColor(51, 150, 215);
                case PinType::FUNCTION: return ImColor(218, 0, 183);
            }
        }

        void iga::bte::BehaviourTreeEditor::DrawPinIcon(const Pin& a_pin, bool a_connected, int a_alpha) {
            ax::Widgets::IconType iconType;
            ImColor  color = GetIconColor(a_pin.type);
            color.Value.w = a_alpha / 255.0f;
            switch (a_pin.type)
            {
                    case PinType::FLOW:     iconType = ax::Widgets::IconType::Flow;   break;
                    case PinType::OBJECT:   iconType = ax::Widgets::IconType::Circle; break;
                    case PinType::FUNCTION: iconType = ax::Widgets::IconType::Circle; break;
                    default:
                return;
            }

            ax::Widgets::Icon(to_imvec(ax::size(pin_icon_size_, pin_icon_size_)), iconType, a_connected, color, ImColor(32, 32, 32, a_alpha));
        }

#pragma endregion

    } // End of namespace ~ bte

} // End of namespace ~ iga

#endif // EDITOR