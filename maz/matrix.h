#pragma once
#include <array>
#include "operation_defs.h"
#include "vector.h"
#include <span>

namespace maz
{
	template<typename T, size_t Width, size_t Height>
	class matrix
	{
	public:
		//template<typename... R>
		//requires (sizeof...(R) == Height)
		//static matrix<T, Width, Height> fromRows(R... i_rows)
		//{
		//
		//}

		template<typename R>
		requires (std::is_convertible_v<R, std::span<const T, Width>>)
		static matrix<T, Width, Height> fromRows(std::array<R, Height> i_rows)
		{
			matrix<T, Width, Height> result;
			size_t i = 0;
			for (const R& row : i_rows)
			{
				std::span<const T, Width> rowView = row;
				size_t j = 0;
				for (j = 0; j < Width; ++j)
				{
					result.m_data[i][j] = rowView[j];
				}
				++i;
			}
			return result;
		}

		template<typename... R>
		requires (sizeof...(R) == Width * Height)
		static matrix<T, Width, Height> fromRows(R... i_valuesInRows)
		{
			std::array<T, Width* Height> arr = std::array<T, Width* Height> { std::forward<R>(i_valuesInRows)...};
			matrix<T, Width, Height> result;
			size_t i = 0;
			for (i = 0; i < Height; ++i)
			{
				size_t j = 0;
				for (j = 0; j < Width; ++j)
				{
					result.m_data[i][j] = arr[i * Width + j];
				}
			}
			return result;
		}

		template<typename... R>
			requires (sizeof...(R) == Width * Height)
		static matrix<T, Width, Height> fromColumns(R... i_valuesInColumns)
		{
			return transpose(fromRows(std::forward<R>(i_valuesInColumns)...));
		}

		template<typename R>
		requires (std::is_convertible_v<R, std::span<const T, Height>>)
		static matrix<T, Width, Height> fromColumns(std::array<R, Width> i_rows)
		{
			return transpose(fromRows(std::move(i_rows)));
		}

		
		inline const T& get(size_t i_row, size_t i_col) const
		{
			return m_data[i_row][i_col];
		}

		inline void set(size_t i_row, size_t i_col, T i_value)
		{
			m_data[i_row][i_col] = std::move(i_value);
		}
		

	private:

		std::array<std::array<T, Width>, Height> m_data;
	};

	template<typename T, size_t Width, size_t Height>
	matrix<T, Width - 1, Height - 1> cofactor(const matrix<T, Width, Height>& i_matrix, size_t i_row, size_t i_column)
	{
		matrix<T, Width - 1, Height - 1> result;
		size_t effectiveRow = 0;
		for (size_t row = 0; row < Height; ++row)
		{
			if (row != i_row)
			{
				size_t effectiveCol = 0;
				for (size_t col = 0; col < Width; ++col)
				{
					if (col != i_column)
					{
						result.set(effectiveRow, effectiveCol, i_matrix.get(row, col));
						effectiveCol++;
					}
				}
				effectiveRow++;
			}
		}

		return result;
	}


	

	namespace detail
	{
		template<typename T, size_t... Index>
		inline auto determinant(const matrix<T, 2, 2>& i_matrix, std::index_sequence<Index...>)
		{
			return i_matrix.get(0, 0) * i_matrix.get(1, 1) - i_matrix.get(0, 1) * i_matrix.get(1, 0);
		}

		template<typename T, size_t... Index>
		inline auto determinant(const matrix<T, 3, 3>& i_matrix, std::index_sequence<Index...>)
		{
			return	(i_matrix.get(0, 0) * i_matrix.get(1, 1) * i_matrix.get(2, 2)
				+ i_matrix.get(0, 1) * i_matrix.get(1, 2) * i_matrix.get(2, 0)
				+ i_matrix.get(0, 2) * i_matrix.get(1, 0) * i_matrix.get(2, 1))
				-
				(i_matrix.get(0, 2) * i_matrix.get(1, 1) * i_matrix.get(2, 0)
				+ i_matrix.get(0, 1) * i_matrix.get(1, 0) * i_matrix.get(2, 2)
				+ i_matrix.get(0, 0) * i_matrix.get(1, 2) * i_matrix.get(2, 1));
		}

		template<typename T, size_t Width, size_t... Index>
		inline auto determinant(const matrix<T, Width, Width>& i_matrix, std::index_sequence<Index...>)
		{
			return
				((i_matrix.get(0, Index) * detail::determinant(cofactor(i_matrix, 0, Index), std::make_index_sequence<Width - 1>()) * (std::pow(- 1.0, Index)))
				+
				...);
		}
	}

	template<typename T, size_t Width>
	auto determinant(const matrix<T, Width, Width>& i_matrix)
	{
		return detail::determinant(i_matrix, std::make_index_sequence<Width>());
	}

	template<typename T, size_t Width, size_t Height>
	matrix<T, Height, Width> transpose(const matrix<T, Width, Height>& i_matrix)
	{
		matrix<T, Height, Width> result;
		for (size_t i = 0; i < Height; ++i)
		{
			for (size_t j = 0; j < Width; ++j)
			{
				result.set(i, j, i_matrix.get(j, i));
			}
		}
		return result;
	}
	
}