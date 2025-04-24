#pragma once
#include "shapes.h"

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
}