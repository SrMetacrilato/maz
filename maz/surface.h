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
		n_surface(vector<T, SpaceDims> i_origin, std::array<vector<T, SpaceDims>, SurfaceDims> i_directions)
		{
			std::array< vector<T, SpaceDims>, SurfaceDims + 1> arr;
			arr[0] = -i_origin;
			for (size_t i = 0; i < SurfaceDims; ++i)
			{
				arr[i + 1] = i_directions[i];
			}
			m_definitionMatrix = matrix<T, SurfaceDims + 1, SpaceDims>::fromColumns(arr);
			//std::array<T, SurfaceDims + 1> factors;
			//for (size_t i = 0; i < SurfaceDims+1; ++i)
			//{
			//	factors[i] = maz::determinant(maz::cofactor(m, 0, i));
			//}

		}

	private:
		//vector<T, SpaceDims> m_origin;
		//std::array<vector<T, SpaceDims>, SurfaceDims> m_directions;
		matrix<T, SurfaceDims + 1, SpaceDims> m_definitionMatrix;
	};

	template<typename T, size_t SpaceDims>
	using plane = n_surface<T, SpaceDims, 2>;

	template<typename T, size_t SpaceDims>
	using line = n_surface<T, SpaceDims, 1>;
}