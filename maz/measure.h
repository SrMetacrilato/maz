#pragma once
#include <ratio>

namespace maz
{
	//Magnitud -> distancia 
	//Unidad -> metros
	//Medida 4 metros
	//dist -> m^1 * t^0
	//tiempo -> m^0 * t^1
	//velocidad -> m^1 * t^-1

	static const int k_numUnits = 17;

	template<std::array<int, k_numUnits> Exp>
	class magnitude
	{
	public:
		inline static constexpr std::array<int, k_numUnits> Exponents = Exp;
		
	};


	template<int... Index>
	constexpr std::array<int, k_numUnits> _sub(std::array<int, k_numUnits> i_left, std::array<int, k_numUnits> i_right, std::index_sequence<Index...>)
	{
		return {i_left[Index] - i_right[Index]...};
	}

	constexpr std::array<int, k_numUnits> operator-(std::array<int, k_numUnits> i_left, std::array<int, k_numUnits> i_right)
	{
		std::make_index_sequence<k_numUnits> indexList;
		return _sub(i_left, i_right, indexList);
	}

	template<int... Index>
	constexpr std::array<int, k_numUnits> _add(std::array<int, k_numUnits> i_left, std::array<int, k_numUnits> i_right, std::index_sequence<Index...>)
	{
		return { i_left[Index] + i_right[Index]... };
	}

	constexpr std::array<int, k_numUnits> operator+(std::array<int, k_numUnits> i_left, std::array<int, k_numUnits> i_right)
	{
		std::make_index_sequence<k_numUnits> indexList;
		return _add(i_left, i_right, indexList);
	}

	template<typename T, int... Index>
	constexpr std::array<int, k_numUnits> _div(std::array<int, k_numUnits> i_left, T i_right, std::index_sequence<Index...>)
	{
		return { i_left[Index] / i_right ... };
	}

	template<typename T>
	constexpr std::array<int, k_numUnits> operator/(std::array<int, k_numUnits> i_left, T i_right)
	{
		std::make_index_sequence<k_numUnits> indexList;
		return _div(i_left, i_right, indexList);
	}

	template<int... Index>
	constexpr bool _is_zero(std::array<int, k_numUnits> i_array, std::index_sequence<Index...>)
	{
		return (std::abs(i_array[Index]) + ...) == 0;
	}

	constexpr bool is_zero(std::array<int, k_numUnits> i_array)
	{
		std::make_index_sequence<k_numUnits> indexList;
		return _is_zero(i_array, indexList);
	}

	template<typename Exponents>
	concept is_literal = requires(Exponents i_left)
	{
		is_zero(i_left);
	};
	

	template<typename M, typename Ratio>
	class unit;

	template<typename Measure, typename Unit, typename T>
	struct measure_view;

	
	template<typename M, typename T>
	class measure
	{
	private:
		T m_valueInSI;

	public:
		using value_type = T;

		measure() = default;

		template<typename M2, typename T2>
		requires div::constraint<T, T2>
		inline auto operator/ (measure<M2, T2> i_other) const
		{
			using T3 = maz::div::type<T, T2>;
			measure<magnitude<M::Exponents - M2::Exponents>, T3> result = m_valueInSI / i_other.internalValueInSI();
			return result;
		}

		template<typename M2, typename T2>
		requires mult::constraint<T, T2>
		inline auto operator* (measure<M2, T2> i_other) const
		{
			using T3 = maz::mult::type<T, T2>;
			measure<magnitude<M::Exponents + M2::Exponents>, T3> result = m_valueInSI * i_other.internalValueInSI();
			return result;
		}

		template<typename T2>
		requires arithmetic<T2> && div::constraint<T, T2>
		inline auto operator/ (const T2& i_literal) const
		{
			using T3 = maz::div::type<T, T2>;
			measure<M, T3> result = m_valueInSI / i_literal;
			return result;
		}

		template<typename T2>
		requires arithmetic<T2>&& mult::constraint<T, T2>
		inline auto operator* (const T2& i_literal) const
		{
			using T3 = maz::mult::type<T, T2>;
			measure<M, T3> result = m_valueInSI * i_literal;
			return result;
		}

		template<typename T2>
		requires sum::constraint<T, T2>
		inline auto operator+ (const measure<M, T2>& i_other) const
		{
			using T3 = maz::sum::type<T, T2>;
			measure<magnitude<M::Exponents>, T3> result = m_valueInSI + i_other.internalValueInSI();
			return result;
		}

		template<typename T2>
		requires std::is_same<sum::type<T, T2>, T>::value
		inline auto operator+= (const measure<M, T2>& i_other)
		{
			m_valueInSI += i_other.internalValueInSI();
			return *this;
		}

		template<typename T2>
		requires diff::constraint<T, T2>
		inline auto operator- (const measure<M, T2>& i_other) const
		{
			using T3 = maz::sum::type<T, T2>;
			measure<magnitude<M::Exponents>, T3> result = m_valueInSI - i_other.internalValueInSI();
			return result;
		}

		inline auto operator- () const
		{
			auto result = -m_valueInSI;
			using T3 = decltype(result);
			return measure<M, T3>(result);
		}

		template<typename T2>
		requires std::is_same<sum::type<T, T2>, T>::value
		inline auto operator-= (const measure<M, T2>& i_other)
		{
			m_valueInSI -= i_other.internalValueInSI();
			return *this;
		}

		template<typename T2>
		inline auto operator< (const measure<M, T2>& i_other) const
		{
			return m_valueInSI < i_other.internalValueInSI();
		}

		template<typename T2>
		inline auto operator> (const measure<M, T2>& i_other) const
		{
			return m_valueInSI > i_other.internalValueInSI();
		}

		template<typename T2>
		inline auto operator<= (const measure<M, T2>& i_other) const
		{
			return m_valueInSI <= i_other.internalValueInSI();
		}

		template<typename T2>
		inline auto operator>= (const measure<M, T2>& i_other) const
		{
			return m_valueInSI >= i_other.internalValueInSI();
		}

		template<typename T2>
		inline auto operator== (const measure<M, T2>& i_other) const
		{
			return m_valueInSI == i_other.internalValueInSI();
		}

		template<typename T2>
		inline auto operator!= (const measure<M, T2>& i_other) const
		{
			return m_valueInSI != i_other.internalValueInSI();
		}

		inline void operator=(measure<M, T> i_other)
		{
			m_valueInSI = std::move(i_other.m_valueInSI);
		}

		inline auto sqrt() const
		{
			using T3 = maz::div::type<T, T>;
			measure<magnitude<M::Exponents / 2>, T3> result = std::sqrt(m_valueInSI);
			return result;
		}

		//requires is_literal<M::Exponents>
		//operator T () const
		//{
		//	return m_valueInSI;
		//}

		inline T internalValueInSI() const
		{
			return m_valueInSI;
		}

		//TODO -> make protected
		inline measure(T i_valueInSI)
			: m_valueInSI(std::move(i_valueInSI))
		{

		}

		inline measure(measure<M, T>&& i_measure)
			: m_valueInSI(std::move(i_measure.m_valueInSI))
		{

		}

		inline measure(const measure<M, T>& i_measure)
			: m_valueInSI(i_measure.m_valueInSI)
		{

		}

		template<typename U, typename T2>
		inline void operator= (const measure_view<measure<M, T>, U, T2>& i_other);

	private:

		
	};

	template<typename M, typename Ratio>
	class unit
	{
	public:
		using ratio_t = Ratio;
		template<typename T>
		static inline measure<M, T> create(const T& i_value)
		{
			return measure<M, T>(i_value * Ratio::num / Ratio::den);
		}

		template<typename T>
		static inline T get(const measure<M, T>& i_measure)
		{
			return i_measure.internalValueInSI() * Ratio::den / Ratio::num;
		}

	};


	

	//===================================
	// Magnitudes
	//===================================
	//Primary ones
	using ratio_mag = magnitude < std::array<int, k_numUnits>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0} > ;
	using rotation_mag = ratio_mag;
	using length_mag = magnitude < std::array<int, k_numUnits>{1} > ;
	using time_mag = magnitude < std::array < int, k_numUnits>{0, 1} > ;
	using temperature_mag = magnitude < std::array < int, k_numUnits>{0, 0, 1} > ;
	using current_mag = magnitude < std::array < int, k_numUnits>{0, 0, 0, 1} > ;
	using mass_mag = magnitude < std::array < int, k_numUnits>{0, 0, 0, 0, 1} > ;
	using substance_mag = magnitude < std::array < int, k_numUnits>{0, 0, 0, 0, 0, 1} > ;
	using luminosity_mag = magnitude < std::array < int, k_numUnits>{0, 0, 0, 0, 0, 0, 1} > ;
	
	
	//Compound ones
	using velocity_mag = magnitude < std::array < int, k_numUnits>{1, -1, 0} > ;
	using angular_velocity_mag = magnitude < std::array < int, k_numUnits>{0, -1, 0} > ;
	using frequency_mag = magnitude < std::array < int, k_numUnits>{0, -1, 0} > ;
	using acceleration_mag = magnitude < std::array < int, k_numUnits>{1, -2, 0} > ;
	using angular_acceleration_mag = magnitude < std::array < int, k_numUnits>{0, -2, 0} > ;
	using frequency_acceleration_mag = magnitude < std::array < int, k_numUnits>{0, -2, 0} > ;
	using length2_mag = magnitude < std::array<int, k_numUnits>{2} > ;
	using area_mag = length2_mag;
	using length3_mag = magnitude < std::array<int, k_numUnits>{3} > ;
	using volume_mag = length3_mag;

	//Custom basic ones
	//template<int e1, int e2 = 0, int e3 = 0, int e4 = 0, int e5 = 0, int e6 = 0, int e7 = 0, int e8 = 0, int e9 = 0, int e10 = 0>
	//struct custom_mag
	//{
	//	using type = magnitude < std::array < int, k_numUnits>{0, 0, 0, 0, 0, 0, 0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10} > ;
	//};
	template<int... exponents>
	struct custom_mag
	{
		using type = magnitude < std::array<int, k_numUnits>{0, 0, 0, 0, 0, 0, 0, exponents...} > ;
	};
	//===================================
	// Base Units
	//===================================
	
	//Ratio
	using perone = unit<ratio_mag, std::ratio<1>>;
	using ppo = unit<ratio_mag, std::ratio<1>>;
	using percent = unit<ratio_mag, std::ratio<1, 100>>;
	using ppc = unit<ratio_mag, std::ratio<1, 100>>;
	using perthousand = unit<ratio_mag, std::ratio<1, 1000>>;
	using ppt = unit<ratio_mag, std::ratio<1, 1000>>;
	using pertau = unit<ratio_mag, std::ratio<1000000000, 6283185307>>;
	using per2pi = pertau;
	using percircle = pertau;

	//Distance
	using meters = unit<length_mag, std::ratio<1>>;
	using centimeters = unit<length_mag, std::centi>;
	using kilometers = unit<length_mag, std::kilo>;
	using miles = unit<length_mag, std::ratio<160934, 100>>;

	//Time
	using seconds = unit<time_mag, std::ratio<1>>;
	using milliseconds = unit<time_mag, std::milli>;
	using hours = unit<time_mag, std::ratio<3600, 1>>;

	//Temperature
	using kelvin = unit<temperature_mag, std::ratio<1>>;

	//Rotation
	using revolution = unit<rotation_mag, std::ratio<1>>;
	using radian = unit<rotation_mag, std::ratio<10000000, 62831853>>;
	using degree = unit<rotation_mag, std::ratio<1, 360>>;

	//===================================
	// Compound Units
	//===================================

	//Distance only
	using meters2 = unit<length2_mag, std::ratio<1>>;
	using meters3 = unit<length3_mag, std::ratio<1>>;


	//velocity
	using metersxsecond = unit<velocity_mag, std::ratio_divide<meters::ratio_t, seconds::ratio_t>>;
	using kilometersxhour = unit<velocity_mag, std::ratio_divide<kilometers::ratio_t, hours::ratio_t>>;


	//acceleration
	using metersxsecond2 = unit<acceleration_mag, std::ratio_divide<metersxsecond::ratio_t, seconds::ratio_t>>;

	//angular velocity
	using hertz = unit<frequency_mag, std::ratio_divide<revolution::ratio_t, seconds::ratio_t>>;
	using radianxsecond = unit<angular_velocity_mag, std::ratio_divide<radian::ratio_t, seconds::ratio_t>>;
	using degreesxsecond = unit<angular_velocity_mag, std::ratio_divide<degree::ratio_t, seconds::ratio_t>>;

	//angular acceleration
	//using hertzxsecond = unit< frequency_acceleration_mag, std::ratio<1>>;
	using radianxsecond2 = unit<angular_acceleration_mag, std::ratio_divide<radianxsecond::ratio_t, seconds::ratio_t>>;
	using degreesxsecond2 = unit<angular_acceleration_mag, std::ratio_divide<degreesxsecond::ratio_t, seconds::ratio_t>>;

	namespace ms
	{
		//===================================
		// Measures
		//===================================
		using ratio = measure<ratio_mag, double>;
		using length = measure<length_mag, double>;
		using area = measure<area_mag, double>;
		using volume = measure<volume_mag, double>;
		using time = measure<time_mag, double>;
		using velocity = measure<velocity_mag, double>;
		using acceleration = measure<acceleration_mag, double>;
		using temperature = measure<temperature_mag, double>;
		using angle = measure<rotation_mag, double>;
		using angular_velocity = measure<angular_velocity_mag, double>;
		using angular_acceleration = measure<angular_acceleration_mag, double>;
		using frequency = measure<frequency_mag, double>;
	}

	//===================================
	// Typed numbers
	//===================================
	
	template<typename T, int... exponents>
	struct typed_number : public measure<typename custom_mag<exponents...>::type, T>
	{
	public:
		using measure_type = measure<typename custom_mag<exponents...>::type, T>;

		struct typed_number_unit
		{
			static typed_number<T, exponents...> create(const T& i_value)
			{
				return typed_number<T, exponents...>(i_value);
			}

			static T get(const measure_type& i_value)
			{
				return i_value.internalValueInSI();
			}
		};
		using unit_type = typed_number_unit;

		explicit inline typed_number(T i_value)
			: measure_type(i_value)
		{

		}

		//inline typed_number(measure_type i_value)
		//	: measure_type(std::move(i_value))
		//{
		//
		//}

		inline typed_number(measure_type&& i_value)
			: measure_type(std::move(i_value))
		{

		}

		inline T value() const
		{
			return this->internalValueInSI();
		}

		typed_number() = default;
	};



	template<typename Measure, typename Unit = Measure::unit_type, typename T = Measure::value_type>
	struct measure_view
	{
		static inline T s_dummyForUnreal;

		measure_view()
			: m_raw(s_dummyForUnreal)
		{

		}

		explicit measure_view(const measure_view<Measure, Unit, T>& i_other)
			: m_raw(i_other.m_raw)
		{

		}

		measure_view(T& i_raw)
			: m_raw(i_raw)
		{

		}

		inline operator Measure() const
		{
			return get();
		}

		inline Measure get() const
		{
			return Unit::create((typename Measure::value_type)m_raw);
		}

		template<typename T2>
		inline auto& operator+= (const T2& i_other)
		{
			set(get() + i_other);
			return *this;
		}

		template<typename T2>
		inline auto operator* (const T2& i_other) const
		{
			return get() * i_other;
		}

		inline void operator= (const measure_view<Measure, Unit, T>& i_other)
		{
			m_raw = i_other.m_raw;
		}

		inline void operator= (const Measure& i_other)
		{
			m_raw = (T)Unit::get(i_other);
		}

		template<typename T2>
		inline void operator= (const T2& i_other)
		{
			set(i_other);
		}

	private:
		template<typename T2>
		inline void set(const T2& i_measure)
		{
			m_raw = (T)Unit::get(i_measure);
		}


		T& m_raw;
	};

}

inline maz::ms::length operator"" _cm(long double i_value)
{
	return maz::centimeters::create<double>(i_value);
}

inline maz::ms::length operator"" _m(long double i_value)
{
	return maz::meters::create<double>(i_value);
}

inline maz::ms::area operator"" _m2(long double i_value)
{
	return maz::meters2::create<double>(i_value);
}

inline maz::ms::volume operator"" _m3(long double i_value)
{
	return maz::meters3::create<double>(i_value);
}

inline maz::ms::length operator"" _km(long double i_value)
{
	return maz::kilometers::create<double>(i_value);
}

inline maz::ms::time operator"" _s(long double i_value)
{
	return maz::seconds::create<double>(i_value);
}

inline maz::ms::time operator"" _ms(long double i_value)
{
	return maz::milliseconds::create<double>(i_value);
}

inline maz::ms::ratio operator"" _pct(long double i_value)
{
	return maz::percent::create<double>(i_value);
}

inline maz::ms::ratio operator"" _percent(long double i_value)
{
	return maz::percent::create<double>(i_value);
}

inline maz::ms::ratio operator"" _ppo(long double i_value)
{
	return maz::perone::create<double>(i_value);
}

inline maz::ms::velocity operator"" _m_s(long double i_value)
{
	return maz::metersxsecond::create<double>(i_value);
}

inline maz::ms::acceleration operator"" _m_s2(long double i_value)
{
	return maz::metersxsecond2::create<double>(i_value);
}

inline maz::ms::velocity operator"" _km_h(long double i_value)
{
	return maz::kilometersxhour::create<double>(i_value);
}

inline maz::ms::angle operator"" _rad(long double i_value)
{
	return maz::radian::create<double>(i_value);
}

inline maz::ms::angle operator"" _deg(long double i_value)
{
	return maz::degree::create<double>(i_value);
}

inline maz::ms::angular_velocity operator"" _rad_s(long double i_value)
{
	return maz::radianxsecond::create<double>(i_value);
}

inline maz::ms::angular_velocity operator"" _deg_s(long double i_value)
{
	return maz::degreesxsecond::create<double>(i_value);
}

inline maz::ms::angular_acceleration operator"" _rad_s2(long double i_value)
{
	return maz::radianxsecond2::create<double>(i_value);
}

inline maz::ms::angular_acceleration operator"" _deg_s2(long double i_value)
{
	return maz::degreesxsecond2::create<double>(i_value);
}

inline maz::ms::frequency operator"" _hz(long double i_value)
{
	return maz::hertz::create<double>(i_value);
}


inline maz::ms::ratio cos(const maz::ms::angle& i_angle)
{
	return maz::perone::create(cos(maz::radian::get(i_angle)));
}

inline maz::ms::ratio sin(const maz::ms::angle& i_angle)
{
	return maz::perone::create(sin(maz::radian::get(i_angle)));
}

inline maz::ms::ratio tan(const maz::ms::angle& i_angle)
{
	return maz::perone::create(tan(maz::radian::get(i_angle)));
}

inline maz::ms::angle atan(const maz::ms::ratio& i_ratio)
{
	return maz::radian::create(atan(maz::perone::get(i_ratio)));
}

template<typename M, typename T, typename M2, typename T2>
inline maz::ms::angle atan2(const maz::measure<M, T>& _ArgA, const maz::measure<M2, T2>& _ArgB)
{
	return maz::radian::create(atan2(_ArgA.internalValueInSI(), _ArgB.internalValueInSI()));
}

#if false
//unreal
template<typename M, typename T>
inline bool IsNearlyEqual(const maz::measure<M, T>& _ArgA, const maz::measure<M, T>& _ArgB)
{
	return FMath::IsNearlyEqual(_ArgA.internalValueInSI(), _ArgB.internalValueInSI());
}
#endif

template<typename M, typename T>
inline maz::measure<M, T> abs(const maz::measure<M, T>& _ArgA)
{
	T v = abs(_ArgA.internalValueInSI());
	return maz::measure<M, T>(v);
}

namespace std
{
	template<typename M, typename T>
	inline maz::measure<M, T> abs(const maz::measure<M, T>& _ArgA)
	{
		T v = abs(_ArgA.internalValueInSI());
		return maz::measure<M, T>(v);
	}

}

template<typename M, typename T>
template<typename U, typename T2>
inline void maz::measure<M, T>::operator= (const maz::measure_view<maz::measure<M, T>, U, T2>& i_other)
{
	*this = i_other.get();
}


namespace std
{
	template<typename M, typename T>
	auto sqrt(const maz::measure<M, T>& i_value)
	{
		return i_value.sqrt();
	}

	template<typename M>
	constexpr inline maz::measure<M, float> lerp(const maz::measure<M, float>& _ArgA, const maz::measure<M, float>& _ArgB, const float _ArgT) noexcept {
		return maz::measure<M, float>(std::lerp(_ArgA.internalValueInSI(), _ArgB.internalValueInSI(), _ArgT));
	}

	template<typename M>
	constexpr inline maz::measure<M, double> lerp(const maz::measure<M, double>& _ArgA, const maz::measure<M, double>& _ArgB, const double _ArgT) noexcept {
		return maz::measure<M, double>(std::lerp(_ArgA.internalValueInSI(), _ArgB.internalValueInSI(), _ArgT));
	}

	template<typename M, typename T>
	auto distance(const maz::measure<M, T>& i_left, const maz::measure<M, T>& i_right)
	{
		auto delta = i_left - i_right;
		T v = abs(delta.internalValueInSI());
		return maz::measure<M, T>(v);
	}
}