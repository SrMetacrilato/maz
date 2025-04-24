#pragma once
#include "vector.h"
#include <numbers>
#include "operations.h"
namespace maz
{
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
}