#pragma once
#include "vector.h"
#include "matrix.h"

namespace maz
{
	template<typename T, size_t SpaceDims, size_t SurfaceDims>
	requires (SurfaceDims < SpaceDims)
	class n_surface //subspace
	{
	private:
		//using CofactorT = decltype(maz::determinant(std::declval<matrix<T, SurfaceDims, SurfaceDims>()))
	public:
		n_surface() = default;

		n_surface(vector<T, SpaceDims> i_origin, std::array<vector<T, SpaceDims>, SurfaceDims> i_directions)
			: m_origin(i_origin)
			, m_directions(m_directions)
		{
			//std::array< vector<T, SpaceDims>, SurfaceDims + 1> arr;
			//arr[0] = -i_origin;
			//for (size_t i = 0; i < SurfaceDims; ++i)
			//{
			//	arr[i + 1] = i_directions[i];
			//}
			//m_definitionMatrix = matrix<T, SurfaceDims + 1, SpaceDims>::fromColumns(arr);
			

		}

		const auto& origin() const
		{
			return m_origin;
		}

		const auto& directions() const
		{
			return m_directions;
		}
		

	private:
		vector<T, SpaceDims> m_origin;
		std::array<vector<T, SpaceDims>, SurfaceDims> m_directions;
		//matrix<T, SurfaceDims + 1, SpaceDims> m_definitionMatrix;
	};

	template<typename T, size_t SpaceDims>
	using plane = n_surface<T, SpaceDims, 2>;

	template<typename T, size_t SpaceDims>
	using line = n_surface<T, SpaceDims, 1>;

	template<typename T, size_t SpaceDims, size_t SurfaceDims>
	n_surface<T, SpaceDims, SpaceDims - SurfaceDims> complementary(const n_surface<T, SpaceDims, SurfaceDims>& i_surf)
	{
		//???
	}

	template<typename T>
	plane<T, 3> complementary(const line<T,3>& i_surf)
	{
		//???
	}

	template<typename T>
	line<T, 2> complementary(const line<T, 2>& i_surf)
	{
		const auto& origin = i_surf.origin();
		const auto& dir = i_surf.directions()[0];
		return line<T, 2>(origin, { cross(dir) });
	}

	//Project point into surface
	template<typename T>
	auto project(const vector<T, 2>& i_point, const line<T, 2>& i_surf)
	{
		const auto& e1 = i_surf.directions()[0];
		auto e2 = i_point - i_surf.origin();
		auto ratio = dot(e1, e2);
		return i_surf.origin() + e1 * ratio;
	}
}