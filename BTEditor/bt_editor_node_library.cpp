#include <Editor/BTEditor/bt_editor_node_library.h>

std::map<std::string, nodedetail::objectConstructLambda>* StaticNodeLibrary::node_library_map_ = node_library_map_ == nullptr ? new std::map<std::string, nodedetail::objectConstructLambda>() : node_library_map_;
