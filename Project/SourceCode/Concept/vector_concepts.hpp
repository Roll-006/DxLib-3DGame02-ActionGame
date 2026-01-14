#pragma once
#include <concepts>
#include <DxLib.h>

namespace v3d_concepts
{
    template<typename T>
    concept ValidT = std::same_as<T, VECTOR>
                  || std::same_as<T, VECTOR_D>;
}
