#pragma once
#include "vector.h"
#include <numbers>
#include <concepts>
#include "operations.h"
#include "surface.h"

namespace maz
{

	template<typename T>
	concept Shape = requires(T t)
	{
		t.X();
	};

	namespace detail
	{
		//template<typename T, size_t Dim>
		//auto surface(const T& i_radius)
		//{
		//
		//}

		//https://es.wikipedia.org/wiki/Archivo:N_SpheresVolumeAndSurfaceArea_es.png
		template<size_t Dims>
		consteval double k_volume()
		{
			if constexpr (Dims == 2)
			{
				return std::numbers::pi_v<double>;
			}
			else if constexpr (Dims == 1)
			{
				return 2.0;
			}
			else
			{
				return k_volume(Dims - 2) * (2.0 * std::numbers::pi_v<double>) / ((double)Dims);
			}
		}

		template<typename T, size_t Dim>
		auto volume(const T& i_radius)
		{
			return pow<Dim>(i_radius) * k_volume<Dim>();
		}

		template<size_t Dims>
		consteval double k_surface()
		{
			if constexpr (Dims == 2)
			{
				return std::numbers::pi_v<double> * 2.0;
			}
			else if constexpr (Dims == 1)
			{
				return 2.0;
			}
			else
			{
				return k_surface(Dims - 2) * (2.0 * std::numbers::pi_v<double>) / ((double)Dims);
			}
		}

		template<typename T, size_t Dim>
		auto surface(const T& i_radius)
		{
			return pow<Dim - 1>(i_radius) * k_surface<Dim>();
		}
	}

	template<typename T, size_t Dim>
	class n_sphere
	{
	public:
		n_sphere() = default;
		n_sphere(T i_radius, vector<T, Dim> i_center)
			: radius(std::move(i_radius))
			, center(std::move(i_center))
		{

		}

		n_sphere(T i_radius, auto... i_args)
			: radius(std::move(i_radius))
			, center(std::forward<decltype(i_args)>(i_args)...)
		{

		}

		vector<T, Dim> center;
		T radius;
	};

	template<typename T, size_t Dim>
	auto surface(const n_sphere<T, Dim>& i_sphere)
	{
		return detail::surface<T, Dim>(i_sphere.radius);
	}

	template<typename T, size_t Dim>
	auto volume(const n_sphere<T, Dim>& i_sphere)
	{
		return detail::volume<T, Dim>(i_sphere.radius);
	}

	template<typename T>
	using circle = n_sphere<T, 2>;

	template<typename T>
	using sphere = n_sphere<T, 3>;


	template<typename T, size_t Dim, size_t NumVertices>
	class n_polygon
	{

		std::array<vector<T, Dim>, NumVertices> m_vertices;

	public:
		template<typename... Params>
		requires (sizeof...(Params) == NumVertices)
		n_polygon(Params... i_values)
			: m_vertices( std::forward<Params>(i_values)...)
		{

		}

		std::array<line<T, Dim>, NumVertices> axes() const
		{
			std::array<line<T, Dim>, NumVertices> result;
			for (int i = 0; i < NumVertices; ++i)
			{
				result[i] = line<T, Dim>(m_vertices[i], { m_vertices[i] - m_vertices[(i + 1) % NumVertices] } );
			}
			return result;
		}

		inline const std::array<vector<T, Dim>, NumVertices> vertices() const
		{
			return m_vertices;
		}
	};

	template<typename T, size_t Dim>
	using n_triangle = n_polygon<T, Dim, 3>;

	template<typename T>
	using triangle3 = n_triangle<T, 3>;

	template<typename T, size_t Dim>
	class n_aabb
	{
		vector<T, Dim> tl;
		vector<T, Dim> br;

	public:
		n_aabb() = default;

		n_aabb(vector<T, Dim> i_tl, vector<T, Dim> i_br)
			: tl(std::move(i_tl))
			, br(std::move(i_br))
		{

		}

		std::array<line<T, Dim>, Dim> axes() const
		{
			std::array<line<T, Dim>, Dim> result;
			for (int i = 0; i < Dim; ++i)
			{
				vector<T, Dim> axis;
				axis[i] = (T)1;
				result[i] = line<T, Dim>(tl, { axis } );
			}
			return result;
		}

		inline const std::array<vector<T, Dim>, pow<Dim>(2)> vertices() const
		{
			std::array<vector<T, Dim>, pow<Dim>(2)> result;
			constexpr auto s = pow<Dim>(2);
			auto delta = br - tl;
			for (int i = 0; i < s; ++i)
			{
				for (int j = 0; j < Dim; ++j)
				{
					auto should = (i >> j) % 2;
					result[i][j] = delta[j] * should;
				}
			}
			return result;
		}
	};

	template<typename T>
	using aabb = n_aabb<T, 2>;
}