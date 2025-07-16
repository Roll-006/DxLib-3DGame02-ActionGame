#pragma once
#include "obj_concepts.hpp"

template<obj_concepts::ObjT ObjT>
class IState;

namespace state_concepts
{
    /// @brief ステート型である
    template<typename T, typename ObjT>
    concept StateT = std::is_base_of_v<IState<ObjT>, T> && obj_concepts::ObjT<ObjT>;
}
