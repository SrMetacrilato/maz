#pragma once

namespace maz
{
	template<size_t Exp, typename T>
	constexpr auto pow(const T& i_base)
	{
		if constexpr (Exp == 0)
		{
			return 1;
		}
		else if constexpr (Exp == 1)
		{
			return i_base;
		}
		else
		{
			return pow<Exp - 1>(i_base) * i_base;
		}
	}
	
}