#pragma once

#include <Core/component.h>
#include <Core/entity.h>

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>

#include <memory>

/**
*   @file behaviour_tree.h
*   @brief iga::bt::BehaviourComponent Header File
*
*   @author Channing Eggers
*   @date 6 March 2018
*   @version $Revision 1.2
*
*   Behaviour Component inherits from Component.
*   This component will give the entity behaviour.
*
*/

/**
*  igart namespace
*/
namespace iga {

    class Entity;

    /**
    *  BT namespace
    */
    namespace bt {

        class BehaviourTree;

        class BehaviourComponent : public iga::Component {
        public:
			/**
			*   Cereal versions of iga::BehaviourComponent
			*/
			enum class Version {
				DEFAULT = 0, /**< Default normal version. */
				LATEST /**< Latest version. */
			};

            /** BehaviourComponent Constructor */
            BehaviourComponent();

            /** 
            *   BehaviourComponent Constructor 
            *   @param a_entity is a weak pointer to an iga::Entity that is to be the owner of this component.
            */
            BehaviourComponent(std::weak_ptr<iga::Entity> a_entity);

            /** BehaviourComponent Destructor */
            ~BehaviourComponent();

            /** Overriden OnCreate from base class Component. Called when this component
            *   gets created.
			*  @param a_on_load Wether the component is loaded by a file or created in run-time. True is loaded by a file. False (default value) is created in run-time.
            */
            void OnCreate(bool a_on_load = false) override;

            /** Overriden Update from base class Component. Called when this component
            *   gets updated.
            */
            void Update() override;

            /** Overriden PostUpdate from base class Component. Called after this component
            *   has been updated.
            */
            void PostUpdate() override;

            /** Overriden Inspect from base class Component. Called when inspecting
            *   this component.
            */
            void Inspect() override;

            /**
            *   Tells the behaviour tree to run its behaviour.
            */
            void RunTree();

            /**
            *   Stops the tree from executing its behaviour.
            */
            void StopTree();

            BehaviourTree* behaviour_tree_ = nullptr; /**< The Behaviour Tree of the component */

            /**
            *   Cereal serialization function.
            */
            template<class Archive>
            void serialize(Archive & a_archive, uint32 const a_version);

        private:

            /**
            *   Function will attempt to build the behaviour tree.
            *   @param bool a_overwrite_bt If True ~ function will overwrite an existing behaviour tree.
            */
            void BuildBehaviourTree(bool a_overwrite_bt);

            bool execute_behaviour_;    /**< Boolean whether the behaviour tree will get excuted or not each frame. */
            std::string bt_file_path_;  /**< The file path of the behaviour tree file. */

        };

        SUBSCRIBECOMPONENT(BehaviourComponent)

    } // Enf of namespace ~ bt

    template<class Archive>
    inline void bt::BehaviourComponent::serialize(Archive& a_archive, uint32 const a_version) {
		switch(static_cast<Version>(a_version)) {
			case Version::LATEST:
			case Version::DEFAULT:
                a_archive(  cereal::base_class<Component>(this),
                            cereal::make_nvp("ExecuteBehaviour", execute_behaviour_),
                            cereal::make_nvp("BTFilePath", bt_file_path_)
                         );
				break;
		}
    }

} // End of namespace ~ iga

CEREAL_CLASS_VERSION(iga::bt::BehaviourComponent, (static_cast<iga::uint32>(iga::bt::BehaviourComponent::Version::LATEST) - 1));
