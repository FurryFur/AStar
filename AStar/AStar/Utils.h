//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : 
// Author       : Lance Chaney
// Mail         : lance.cha7337@mediadesign.school.nz
//

#pragma once

#include <array>

// A simple mulidimensional array
template <typename T, size_t DimFirst, size_t... Dims>
class NDArray : public std::array<NDArray<T, Dims...>, DimFirst> {};

// A simple mulidimensional array
template <typename T, size_t DimLast>
class NDArray<T, DimLast> : public std::array<T, DimLast> {};