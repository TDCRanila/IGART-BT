#pragma once

#include <AI\BT\Nodes\decorator_node.h>

/**
* @file repeater.h
* @brief iga::bt::Repeater header File
*
* @author Channing Eggers
* @date 4 March 2018
* @version $Revision 1.2
*
* Repeater class is derived from DecoratorNode.
* The repeater node will repeatetly update the child
* until the child doesn't return success or that the 
* max count has been reached.
*/

/**
*  igart namespace
*/
namespace iga {

    /**
    *  BT namespace
    */
    namespace bt {

        class Repeater final : public DecoratorNode {
        public:

            /** Repeater Constructor */
            Repeater();

            /** Repeater Destructor */
            ~Repeater();

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

            /**
            *   Function sets the max amount of times the repeater repeats itself.
            *   @param a_max_count is an integer that represents the amount of times
            *   the repeater repeats.
            */
            void SetMaxRepeaterCount(int a_max_count);

        private:

#ifdef EDITOR

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
            *   the repeater node gets updated.
            *   @see BaseNode
            *   @return NodeResult SUCCESS/RUNNING/FAILED/ERROR.
            */
            NodeResult OnUpdate();

            int repeater_count_ = 0;        /**< Current Repeater count. */

            int repeater_count_max_ = 0;    /**< Max Repeater Count. */

        };

        SUBSCRIBE_NODE(Repeater)

    } // End of namespace ~ bt

    template<class Archive>
    inline void bt::Repeater::serialize(Archive &a_archive, uint32 const a_version) {
        switch (static_cast<Version>(a_version)) {
        case Version::LATEST:
        case Version::DEFAULT:
            a_archive(  cereal::base_class<bt::BaseNode>(this),
                        cereal::make_nvp("RepeaterMaxCount", repeater_count_max_)
            );
            break;
        }
    }

} // End of namespace ~ iga

CEREAL_CLASS_VERSION(iga::bt::Repeater, (static_cast<iga::uint32>(iga::bt::Repeater::Version::LATEST) - 1))
