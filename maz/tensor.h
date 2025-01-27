#pragma once
#include <array>

namespace maz
{
	namespace detail
	{
		template<size_t Dimensions, int... Index>
		constexpr std::array<size_t, Dimensions - 1> _pop(std::array<size_t, Dimensions> i_in, std::index_sequence<Index...>)
		{
			return { i_in[Index]... };
		}

		template<size_t Dimensions>
		constexpr std::array<size_t, Dimensions-1> pop(std::array<size_t, Dimensions> i_in)
		{
			std::make_index_sequence<Dimensions - 1> indexList;
			return _pop(i_in, indexList);
		}

		template<size_t Dimensions, std::array<size_t, Dimensions> Sizes, typename T>
		class tensor_base
		{
		private:
			

		public:
			static const size_t k_dims = Dimensions;
			static constexpr std::array<size_t, Dimensions> k_sizes = Sizes;
			using TElement = tensor_base<Dimensions - 1, pop(Sizes), T>;
			using TData = std::array<TElement, k_sizes[Dimensions - 1]>;

		private:
			TData data;

		};

		//vec3
		template<typename T>
		class tensor_base<1, std::array<size_t, 1>{ 3 }, T>
		{
		private:


		public:
			static const size_t k_dims = 1;
			static constexpr std::array<size_t, 1> k_sizes = std::array<size_t, 1>{ 3 };
			using TElement = tensor_base<0, pop(k_sizes), T>;
			
			TElement x;
			TElement y;
			TElement z;
	
		};

		template<std::array<size_t, 0> Sizes, typename T>
		class tensor_base<0, Sizes, T>
		{
		public:
			using TElement = T;

		private:
			TElement data;
		};
	}

	

	template<typename T, size_t... Dimensions>
	class tensor : public detail::tensor_base < sizeof...(Dimensions), std::array<size_t, sizeof...(Dimensions)>({ Dimensions... }), T >
	{

	};

	template<size_t Rows, typename T>
	using vec = tensor < T, Rows > ;

	template<typename T>
	using vec3 = vec < 3, T >;


	template<size_t Rows, size_t Cols, typename T>
	using mat = tensor < T, Rows, Cols >;


	

}


template<typename T1, typename T2, size_t... Dimensions>
auto operator+ (const maz::tensor<T1, Dimensions...>& i_left, const maz::tensor<T2, Dimensions...>& i_right)
{
	return i_left;
}


template<typename T1, typename T2, size_t... Dimensions>
auto add (const maz::tensor<T1, Dimensions...>& i_left, const maz::tensor<T2, Dimensions...>& i_right)
{
	return i_left;
}