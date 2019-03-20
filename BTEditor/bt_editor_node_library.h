#pragma once

#include <map>
#include <string>
#include <utility>

namespace iga {
    namespace bt {
        class BaseNode;
    }
}

namespace nodedetail {
    typedef iga::bt::BaseNode* (*objectConstructLambda)();
}

class StaticNodeLibrary {
public:
    static std::map<std::string, nodedetail::objectConstructLambda>* node_library_map_;
};

#define SUBSCRIBE_NODE(classArg)                                                                                                                            \
namespace classArg##classArg {                                                                                                                              \
    template <typename C> struct NodeItem																											        \
    {																																				        \
        NodeItem();																																            \
    };																																				        \
    template <typename C> NodeItem<C>::NodeItem()																									        \
    {																																				        \
        if (StaticNodeLibrary::node_library_map_ == nullptr)																				                \
        {																																			        \
        StaticNodeLibrary::node_library_map_ = new std::map<std::string, ::nodedetail::objectConstructLambda>();									        \
        }																																			        \
        StaticNodeLibrary::node_library_map_->insert(std::make_pair( std::string( #classArg ), []() { return ( iga::bt::BaseNode* )new classArg(); } ) );   \
    }																																			            \
    static NodeItem<classArg> n;                                                                                                                            \
}