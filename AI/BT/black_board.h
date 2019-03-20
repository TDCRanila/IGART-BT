#pragma once

#include <string>
#include <unordered_map>

/**
* @file black_board.h
* @brief The BlackBoard can store data that the BehaviourTree uses.
*
* @author Channing Eggers
* @date 6 March 2018
* @version $Revision 1.1
*
* Class contains std::unordered_maps for the data types;
* Integer, Float, Bool & String.
* Setting memory will create a copy and will store it inside of the maps.
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

        class BaseNode;

        class BlackBoard final {
        public:
            /** BlackBoard Constructor */
            BlackBoard();

            /** BlackBoard Destructor */
            ~BlackBoard();

#pragma region BlackBoard Memory Setters

            /**
            *   Stores a copy of the integer inside of the std::map of the blackboard with a string ID.
            *   @param a_id is a string that is assigned this value.
            *   @param a_value is the integer value that you want to store inside of the blackboard.
            */
            void SetGlobalIntMemory(std::string a_id, int a_value);

            /**
            *   Stores a copy of the integer inside of the std::map of the blackboard with a string ID
            *   in the scope of the node.
            *   @param a_id is a string that is assigned this value.
            *   @param a_node is a pointer to the node that will have access to this memory value.
            *   @param a_value is the integer value that you want to store inside of the blackboard.
            */
            void SetIntMemory(std::string a_id, BaseNode* a_node, int a_value);

            /**
            *   Stores a copy of the float inside of the std::map of the blackboard with a string ID.
            *   @param a_id is a string that is assigned to this value.
            *   @param a_value is the float value that you want to store inside of the blackboard.
            */
            void SetGlobalFloatMemory(std::string a_id, float a_value);

            /**
            *   Stores a copy of the float inside of the std::map of the blackboard with a string ID.
            *   in the scope of the node.
            *   @param a_id is a string that is assigned to this value.
            *   @param a_node is a pointer to the node that will have access to this memory value.
            *   @param a_value is the float value that you want to store inside of the blackboard.
            */
            void SetFloatMemory(std::string a_id, BaseNode* a_node, float a_value);

            /**
            *   Stores a copy of the bool inside of the std::map of the blackboard with a string ID.
            *   @param a_id is a string for getting the value back from the std::map.
            *   @param a_value is the bool value that you want to store inside of the blackboard.
            */
            void SetGlobalBoolMemory(std::string a_id, bool a_value);

            /**
            *   Stores a copy of the bool inside of the std::map of the blackboard with a string ID.
            *   in the scope of the node.
            *   @param a_id is a string for getting the value back from the std::map.
            *   @param a_node is a pointer to the node that will have access to this memory value.
            *   @param a_value is the bool value that you want to store inside of the blackboard.
            */
            void SetBoolMemory(std::string a_id, BaseNode* a_node, bool a_value);

            /**
            *   Stores a copy of the std::string inside of the std::map of the blackboard with a string ID.
            *   @param a_id is a string that is assigned to this value.
            *   @param a_value is the std::string value that you want to store inside of the blackboard.
            */
            void SetGlobalStringMemory(std::string a_id, std::string a_value);

            /**
            *   Stores a copy of the std::string inside of the std::map of the blackboard with a string ID.
            *   in the scope of the node.
            *   @param a_id is a string that is assigned to this value.
            *   @param a_node is a pointer to the node that will have access to this memory value.
            *   @param a_value is the std::string value that you want to store inside of the blackboard.
            */
            void SetStringMemory(std::string a_id, BaseNode* a_node, std::string a_value);

#pragma endregion End of Blackboard Memory Setters

#pragma region BlackBoard Memory Getters

            /**
            *   Sets the value of the passed through item to the value that was stored 
            *   inside of the blackboard.
            *   @param a_id is the string to access the value inside of the std::map.
            *   @param a_int_ref is the container that you want to store the value in.
            *   @return bool - If True ~ Value with ID has been found.
            */
            bool GetIntMemory(std::string a_id, int& a_int_ref) const;

            /**
            *   Sets the value of the passed through item to the value that was stored
            *   inside of the blackboard.
            *   @param a_id is the string to access the value inside of the std::map.
            *   @param a_node is a pointer to the node that will have access to this memory value.
            *   @param a_int_ref is the container that you want to store the value in.
            *   @return bool - If True ~ Value with ID has been found.
            */
            bool GetIntMemory(std::string a_id, BaseNode* a_node, int& a_int_ref) const;

            /**
            *   Sets the value of the passed through item to the value that was stored
            *   inside of the blackboard.
            *   @param a_id is the string to access the value inside of the std::map.
            *   @param a_float_ref is the container that you want to store the value in.
            *   @return bool - If True ~ Value with ID has been found.
            */
            bool GetFloatMemory(std::string a_id, float& a_float_ref) const;

            /**
            *   Sets the value of the passed through item to the value that was stored
            *   inside of the blackboard.
            *   @param a_id is the string to access the value inside of the std::map.
            *   @param a_node is a pointer to the node that will have access to this memory value.
            *   @param a_float_ref is the container that you want to store the value in.
            *   @return bool - If True ~ Value with ID has been found.
            */
            bool GetFloatMemory(std::string a_id, BaseNode* a_node, float& a_float_ref) const;

            /**
            *   Sets the value of the passed through item to the value that was stored
            *   inside of the blackboard.
            *   @param a_id is the string to access the value inside of the std::map.
            *   @param a_bool_ref is the container that you want to store the value in.
            *   @return bool - If True ~ Value with ID has been found.
            */
            bool GetBoolMemory(std::string a_id, bool& a_bool_ref) const;

            /**
            *   Sets the value of the passed through item to the value that was stored
            *   inside of the blackboard.
            *   @param a_id is the string to access the value inside of the std::map.
            *   @param a_node is a pointer to the node that will have access to this memory value.
            *   @param a_bool_ref is the container that you want to store the value in.
            *   @return bool - If True ~ Value with ID has been found.
            */
            bool GetBoolMemory(std::string a_id, BaseNode* a_node, bool& a_bool_ref) const;

            /**
            *   Sets the value of the passed through item to the value that was stored
            *   inside of the blackboard.
            *   @param a_id is the string to access the value inside of the std::map.
            *   @param a_string_ref is the container that you want to store the value in.
            *   @return bool - If True ~ Value with ID has been found.
            */
            bool GetStringMemory(std::string a_id , std::string& a_string_ref) const;

            /**
            *   Sets the value of the passed through item to the value that was stored
            *   inside of the blackboard.
            *   @param a_id is the string to access the value inside of the std::map.
            *   @param a_node is a pointer to the node that will have access to this memory value.
            *   @param a_string_ref is the container that you want to store the value in.
            *   @return bool - If True ~ Value with ID has been found.
            */
            bool GetStringMemory(std::string a_id, BaseNode* a_node, std::string& a_string_ref) const;

#pragma endregion End of BlackBoard Memory Getters

        private:

            std::unordered_map<std::string, int> int_memory_; /** Unordered std::string~integer map - Stores values for the BehaviourTree. */

            std::unordered_map<std::string, float> float_memory_; /** Unordered std::string~float map - Stores values for the BehaviourTree. */

            std::unordered_map<std::string, bool> bool_memory_; /** Unordered std::string~bool map - Stores values for the BehaviourTree. */

            std::unordered_map<std::string, std::string> string_memory_; /** Unordered std::string~std::string map - Stores values for the BehaviourTree. */

    };

    } // End of namespace ~ bt

} // End of namespace ~ iga
