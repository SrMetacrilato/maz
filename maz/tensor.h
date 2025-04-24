#pragma once
#include <array>

namespace maz
{
	namespace detail
	{
		template<std::size_t N, typename... T>
		concept ExactSize = (sizeof...(T) == N);


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

		protected:
			TElement& Get(size_t i_index)
			{
				return data[i_index];
			}

			const TElement& Get(size_t i_index) const
			{
				return data[i_index];
			}

		private:
			TData data;

		};


		//vec3
		template<typename T>
		class tensor_base<1, std::array<size_t, 1>{ 3 }, T>
		{
		public:
			static const size_t k_dims = 1;
			static constexpr std::array<size_t, 1> k_sizes = std::array<size_t, 1>{ 3 };
			using TElement = tensor_base<0, pop(k_sizes), T>;

			TElement x;
			TElement y;
			TElement z;

		private:


		protected:

			tensor_base() = default;

			template<typename T2>
			requires (std::is_trivially_constructible_v<TElement, T2>)
			tensor_base(T2 i_x, T2 i_y, T2 i_z)
				: x(std::move(i_x))
				, y (std::move(i_y))
				, z (std::move(i_z))
			{

			}

			TElement& Get(size_t i_index)
			{
				switch (i_index)
				{
				case 0:
					return x;
				case 1:
					return y;
				case 2:
					return z;
				default:
					return x;
				}
			}

			const TElement& Get(size_t i_index) const
			{
				switch (i_index)
				{
				case 0:
					return x;
				case 1:
					return y;
				case 2:
					return z;
				default:
					return x;
				}
			}
		
	
		};

		//scalar
		template<typename T>
		class tensor_base < 0, std::array<size_t, 0>{}, T >
		{
		public:
			static const size_t k_dims = 0;
			static constexpr std::array<size_t, 0> k_sizes = std::array<size_t, 0>{};
			using TElement = T;

			tensor_base() = default;
			tensor_base(T i_data)
				: data(std::move(i_data))
			{

			}

			operator T () const
			{
				return data;
			}

			void operator= (T i_data)
			{
				data = std::move(i_data);
			}

			TElement& Get(size_t i_index)
			{
				return data;
			}

			const TElement& Get(size_t i_index) const
			{
				return data;
			}

		private:
			TElement data;
		};
	}

	

	template<typename T, size_t... Dimensions>
	class tensor : public detail::tensor_base < sizeof...(Dimensions), std::array<size_t, sizeof...(Dimensions)>({ Dimensions... }), T >
	{
	public:
		using base = detail::tensor_base < sizeof...(Dimensions), std::array<size_t, sizeof...(Dimensions)>({ Dimensions... }), T > ;
		
		template<typename... T2>
		//requires (detail::ExactSize<base::k_sizes[0], T2...>)
		requires (detail::ExactSize<base::k_sizes[0], T2...> && (std::is_constructible_v<typename base::TElement, T2> || ...))
		tensor(T2... i_values)
		{
		
		}

		template<typename T2>
		requires ((sizeof...(Dimensions) == 0) && (std::is_trivially_constructible_v<typename base::TElement, T2>))
		tensor(T2 i_data)
			: base(std::move(i_data))
		{

		}

		tensor() = default;

		inline auto& operator[] (size_t i_index)
		{
			return base::Get(i_index);
		}

		inline const auto& operator[] (size_t i_index) const
		{
			return base::Get(i_index);
		}
	};

	template<typename T>
	using scalar = tensor<T>;

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
	return maz::tensor(i_left);
	return i_left;
}