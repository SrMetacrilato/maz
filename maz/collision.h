#pragma once
#include "shapes.h"
#include "surface.h"

namespace maz
{
	template<typename T, typename R>
	bool collides(const T& i_t, const R& i_r)
	{
		return false;
	}
	
	template<typename T, size_t Dim>
	bool collides(const n_sphere<T, Dim>& i_s1, const n_sphere<T, Dim>& i_s2)
	{
		return (i_s1.center - i_s2.center).lengthSq() <= (i_s1.radius + i_s2.radius) * (i_s1.radius + i_s2.radius);
	}

	//SAT
	template<typename T, size_t Dim, size_t PolyVertices>
	bool collides(const n_polygon<T, Dim, PolyVertices>& i_left, const n_aabb<T, Dim> i_right)
	{
		auto axesL = i_left.axes();
		for (const auto& axis : axesL)
		{
			auto planeToProject = complementary(axis);
		}

		auto axesR = i_right.axes();
		return true;
	}
}