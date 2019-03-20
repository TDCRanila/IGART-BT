#include <Editor/BTEditor/bt_editor_structs.h>

#include <Editor/BTEditor/behaviour_tree_editor.h>

namespace iga {

    namespace bte {

        EditorNode::EditorNode() :
            size(Vector2(0.0f, 0.0f)), color(Vector4(255.f, 255.f, 255.f, 0.f))
        {
            // DO NOTHING
        }

        EditorNode::~EditorNode() {
            // DO NOTHING
        }

        EditorNode::EditorNode(const EditorNode& other) {
            this->id                    = other.id;
            this->name                  = other.name;
            this->color                 = other.color;
            this->type                  = other.type;
            this->size                  = other.size;

            this->state                 = other.state;
            this->saved_state           = other.saved_state;

            this->base_node             = other.base_node;
            this->saving_base_node      = other.saving_base_node;

            // Vector Copy
            this->outgoing_link_IDs     = other.outgoing_link_IDs;
            this->receiving_link_IDs    = other.receiving_link_IDs;
            this->inputs                = other.inputs;
            this->outputs               = other.outputs;

            for (auto& input_pin : this->inputs) {
                input_pin.node = this;
            }

            for (auto& output_pin : this->outputs) {
                output_pin.node = this;
            }
        }

#ifdef EDITOR

        EditorNode::EditorNode(int a_id, const char* a_name, ImColor a_color) :
            id(a_id), name(a_name), color(Vector4(a_color.Value.x, a_color.Value.y, a_color.Value.z, a_color.Value.w)), type(NodeType::TREE), size(0, 0)
        {
            // DO NOTHING
        }

        void EditorNode::AddInputPin(PinType a_pin_type, std::string a_pin_name) {
            this->inputs.emplace_back(Pin(GetNextID() , a_pin_name.c_str(), a_pin_type));
        }

        void EditorNode::AddOutputPin(PinType a_pin_type, std::string a_pin_name) {
            this->outputs.emplace_back(Pin(GetNextID(), a_pin_name.c_str(), a_pin_type));
        }

        void EditorNode::SetType(NodeType a_node_type) {
            this->type = a_node_type;
        }

        void EditorNode::SetName(std::string a_node_name) {
            this->name = a_node_name;
        }

        void EditorNode::SetBaseNode(iga::bt::BaseNode* a_base_node) {
            this->base_node = a_base_node;
        }

#endif // EDITOR

        std::string EditorNode::GetName() const {
            return this->name;
        }

        Pin::Pin() {
            // DO NOTHING
        }

        Pin::Pin(int a_id, const char* a_name, PinType a_type) : 
            id(a_id), node(nullptr), name(a_name), type(a_type), kind(PinKind::TARGET)
        {
            // DO NOTHING
        }

        Pin::~Pin() {
            // DO NOTHING
        }

        Link::Link() {
            // DO NOTHING
        }

        Link::Link(int a_id, int a_startPinId, int a_endPinId) :
            id(a_id), start_pin_id(a_startPinId), end_pin_id(a_endPinId), color(255.f, 255.f, 255.f, 0.f)
        {
            // DO NOTHING
        }

        Link::~Link() {
            // DO NOTHING
        }

    } // End of namespace ~ bte

} // End of namspace ~ iga