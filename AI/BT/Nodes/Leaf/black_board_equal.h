#pragma once

#include <AI/BT/Nodes/leaf_node.h>

#include <AI/BT/black_board.h>

/**
* @file selector.h
* @brief iga::bt::BBCompare Header File
*
* @author Channing Eggers
* @date 9 April 2018
* @version $Revision 1.2
*
* DebugMessage class is derived from LeafNode.
*
* Outputs a message to the screen/debugger.

* Always returns SUCCESS.
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

        class BBCompare : public LeafNode {
        public:

            /**
            *   Cereal versions of iga::bt::DebugMessage.
            */
            enum class Version {
                DEFAULT = 0,    /**< Default normal version. */
                LATEST          /**< Latest version. */
            };

            /**
            *   Default Constructor
            */
            BBCompare();

            /**
            *   Default Destructor
            */
            ~BBCompare();

            /**
            *   Virtual Function that is used when polymorphically cloning this object when a shared_ptr needs to be returned.
            *   @return std::shared_ptr<BaseNode> Pointer to the newly cloned object.
            */
            virtual std::shared_ptr<BaseNode> CloneToShared() const override;

            /**
            *   Virtual Function that is used when polymorphically cloning this object when a raw pointer needs to be returned.
            *   @return BaseNode* Pointer to the newly cloned object.
            */
            virtual BaseNode* Clone() const override;

            /**
            *  Serializes this node.
            */
            template<class Archive>
            void serialize(Archive &a_archive, uint32 const a_version);

        private:

            template <class T>
            inline const bool EqualCompare(std::string& a_value_name, T& compare_value) const {
                UNUSED(a_value_name);
                UNUSED(compare_value);
                DEBUGASSERT(true);
                return false;
            }

            template <>
            inline const bool EqualCompare(std::string& a_value_name, int& compare_value) const {
                int temp;
                if (!GetBehaviourTree()->GetBlackBoard()->GetIntMemory(a_value_name, temp)) { return false; }
                return temp == compare_value ? true : false;
            }

            template <>
            inline const bool EqualCompare(std::string& a_value_name, float& compare_value) const {
                float temp;
                // TODO: if (!GetBehaviourTree()->GetBlackBoard()->GetFloatMemory(a_value_name, temp)) { return false; }
                return temp == compare_value ? true : false;
            }

            template <>
            inline const bool EqualCompare(std::string& a_value_name, bool& compare_value) const {
                bool temp;
                //T ODO: if (!GetBehaviourTree()->GetBlackBoard()->GetBoolMemory(a_value_name, temp)) { return false; }
                return temp == compare_value ? true : false;
            }

            template <>
            inline const bool EqualCompare(std::string& a_value_name, std::string& compare_value) const {
                std::string temp;
                // TODO: if (!GetBehaviourTree()->GetBlackBoard()->GetStringMemory(a_value_name, temp)) { return false; }
                return temp == compare_value ? true : false;
            }

#ifdef EDITOR

            /**
            *   Virtual function that gets called whenever a base node gets loaded by the editor.
            *   (Gets called after the constructor and the loading of serialized data.)
            */
            void OnEditorLoad() override;

            /**
            *   Overriden function that creates and returns an editor node.
            *   @param EditorNode a_editor_node A pointer to a newly constructed editor node.
            */
            void ConstructEditorNode(iga::bte::EditorNode* a_editor_node) override;

            /**
            *   Overriden function that inspects the node.
            */
            void InspectNode() override;

#endif // EDITOR

            /** OnUpdate function overriden from BaseNode class. Called when
            *   this node gets updated.
            *   @see BaseNode
            *   @return NodeResult SUCCESS/RUNNING/FAILED/ERROR.
            */
            iga::bt::NodeResult OnUpdate();

            BlackBoardTypes type_;

            std::string variable_name_;

            int variable_value_int_;
            float variable_value_float_;
            bool variable_value_bool_;
            std::string variable_value_string_;

#ifdef EDITOR
            char variable_name_buffer_[256] = "";
            char variable_value_buffer_[256] = "";
#endif // EDITOR

        };

        SUBSCRIBE_NODE(BBCompare)

    } // End of namespace ~ bt

    template<class Archive>
    inline void bt::BBCompare::serialize(Archive& a_archive, uint32 const a_version) {
        switch (static_cast<Version>(a_version)) {
        case Version::LATEST:
        case Version::DEFAULT:
            a_archive(
                cereal::base_class<bt::BaseNode>(this),
                cereal::make_nvp("VariableType", type_),
                cereal::make_nvp("VariableName", variable_name_),
                cereal::make_nvp("variable_value_int_", variable_value_int_),
                cereal::make_nvp("variable_value_float_", variable_value_float_),
                cereal::make_nvp("variable_value_bool_", variable_value_bool_),
                cereal::make_nvp("variable_value_string_", variable_value_string_)
            );
            break;
        }
    }

} // End of namespace ~ iga

CEREAL_CLASS_VERSION(iga::bt::BBCompare, (static_cast<iga::uint32>(iga::bt::BBCompare::Version::LATEST) - 1))