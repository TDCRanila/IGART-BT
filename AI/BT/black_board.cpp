#include <AI\BT\black_board.h>

#include <AI\BT\Nodes\base_node.h>

#include <iostream>

namespace iga {

    namespace bt {

        BlackBoard::BlackBoard() { /*EMPTY*/ }

        BlackBoard::~BlackBoard() { 
            this->int_memory_.clear();
            this->float_memory_.clear();
            this->bool_memory_.clear();
            this->string_memory_.clear();
        }

        void BlackBoard::SetGlobalIntMemory(std::string a_id, int a_value) {
            std::unordered_map<std::string, int>::const_iterator it;
            it = this->int_memory_.find(a_id);

            if (it != this->int_memory_.end()) {
                this->int_memory_.at(a_id) = a_value;
            } else {
                this->int_memory_.emplace(a_id, a_value);
            }
        }

        void BlackBoard::SetIntMemory(std::string a_id, BaseNode* a_node, int a_value) {
            std::string new_id = a_id.append(std::to_string(a_node->GetID()));
            SetGlobalIntMemory(new_id, a_value);
        }

        void BlackBoard::SetGlobalFloatMemory(std::string a_id, float a_value) {
            std::unordered_map<std::string, float>::const_iterator it;
            it = this->float_memory_.find(a_id);

            if (it != this->float_memory_.end()) {
                this->float_memory_.at(a_id) = a_value;
            } else {
                this->float_memory_.emplace(a_id, a_value);
            }
        }

        void BlackBoard::SetFloatMemory(std::string a_id, BaseNode* a_node, float a_value) {
            std::string new_id = a_id.append(std::to_string(a_node->GetID()));
            SetGlobalFloatMemory(new_id, a_value);
        }

        void BlackBoard::SetGlobalBoolMemory(std::string a_id, bool a_value) {
            std::unordered_map<std::string, bool>::const_iterator it;
            it = this->bool_memory_.find(a_id);

            if (it != this->bool_memory_.end()) {
                this->bool_memory_.at(a_id) = a_value;
            } else {
                this->bool_memory_.emplace(a_id, a_value);
            }
        }

        void BlackBoard::SetBoolMemory(std::string a_id, BaseNode* a_node, bool a_value) {
            std::string new_id = a_id.append(std::to_string(a_node->GetID()));
            SetGlobalBoolMemory(new_id, a_value);
        }
        
        void BlackBoard::SetGlobalStringMemory(std::string a_id, std::string a_value) {
            std::unordered_map < std::string, std::string > ::const_iterator it;
            it = this->string_memory_.find(a_id);

            if (it != this->string_memory_.end()) {
                this->string_memory_.at(a_id) = a_value;
            } else {
                this->string_memory_.emplace(a_id, a_value);
            }
        }

        void BlackBoard::SetStringMemory(std::string a_id, BaseNode* a_node, std::string a_value) {
            std::string new_id = a_id.append(std::to_string(a_node->GetID()));
            SetGlobalStringMemory(new_id, a_value);
        }

        bool BlackBoard::GetIntMemory(std::string a_id, int& a_int_ref) const {
            std::unordered_map<std::string, int>::const_iterator it;
            it = this->int_memory_.find(a_id);

            if (it != this->int_memory_.end()) {
                a_int_ref = it->second;
                return true;
            } else {
                return false; 
            }
        }

        bool BlackBoard::GetIntMemory(std::string a_id, BaseNode* a_node, int& a_int_ref) const {
            std::string new_id = a_id.append(std::to_string(a_node->GetID()));
            return GetIntMemory(new_id, a_int_ref);
        }

        bool BlackBoard::GetFloatMemory(std::string a_id, float& a_float_ref) const {
            std::unordered_map<std::string, float>::const_iterator it;
            it = this->float_memory_.find(a_id);

            if (it != this->float_memory_.end()) {
                a_float_ref = it->second;
                return true;
            } else {
                return false;
            }
        }

        bool BlackBoard::GetFloatMemory(std::string a_id, BaseNode* a_node, float& a_float_ref) const {
            std::string new_id = a_id.append(std::to_string(a_node->GetID()));
            return GetFloatMemory(new_id, a_float_ref);
        }

        bool BlackBoard::GetBoolMemory(std::string a_id, bool& a_bool_ref) const {
            std::unordered_map<std::string, bool>::const_iterator it;
            it = this->bool_memory_.find(a_id);

            if (it != this->bool_memory_.end()) {
                a_bool_ref = it->second;
                return true;
            } else {
                return false;
            }
        }

        bool BlackBoard::GetBoolMemory(std::string a_id, BaseNode* a_node, bool& a_bool_ref) const {
            std::string new_id = a_id.append(std::to_string(a_node->GetID()));
            return GetBoolMemory(new_id, a_bool_ref);
        }

        bool BlackBoard::GetStringMemory(std::string a_id, std::string& a_string_ref) const {
            std::unordered_map<std::string, std::string>::const_iterator it;
            it = this->string_memory_.find(a_id);

            if (it != this->string_memory_.end()) {
                a_string_ref = it->second;
                return true;
            } else {
                return false;
            }
        }

        bool BlackBoard::GetStringMemory(std::string a_id, BaseNode* a_node, std::string& a_string_ref) const {
            std::string new_id = a_id.append(std::to_string(a_node->GetID()));
            return GetStringMemory(new_id, a_string_ref);
        }


    } // End of namespace ~ bt

} // End of namespace ~ iga
