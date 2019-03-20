#pragma once

#include <AI/BT/Nodes/leaf_node.h>

#include <AI/BT/black_board.h>

/**
* @file selector.h
* @brief iga::bt::BBIsValid Header File
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

        class BBSetInt : public LeafNode {
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
            BBSetInt();

            /**
            *   Default Destructor
            */
            ~BBSetInt();

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

            /** Virtual Function that gets called whenever the behaviour tree is getting build. */
            virtual void OnBehaviourTreeBuild() override;

            /**
            *  Serializes this node.
            */
            template<class Archive>
            void serialize(Archive &a_archive, uint32 const a_version);

        private:

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

            std::string variable_name_;

            int variable_value_;

#ifdef EDITOR
            char variable_name_buffer_[256] = "";
#endif // EDITOR

        };

        SUBSCRIBE_NODE(BBSetInt)

    } // End of namespace ~ bt

    template<class Archive>
    inline void bt::BBSetInt::serialize(Archive& a_archive, uint32 const a_version) {
        switch (static_cast<Version>(a_version)) {
        case Version::LATEST:
        case Version::DEFAULT:
            a_archive(
                cereal::base_class<bt::BaseNode>(this),
                cereal::make_nvp("VariableName", variable_name_),
                cereal::make_nvp("VariableValue", variable_value_)
            );
            break;
        }
    }

} // End of namespace ~ iga

CEREAL_CLASS_VERSION(iga::bt::BBSetInt, (static_cast<iga::uint32>(iga::bt::BBSetInt::Version::LATEST) - 1))