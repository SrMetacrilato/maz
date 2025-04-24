#include "pch.h"
#include "maz/tensor.h"
#include "maz/vector.h"
#include "maz/shapes.h"
#include "maz/measure.h"
#include "maz/collision.h"
#include "maz/surface.h"


//TEST(TestCaseName, TestName) {
//  EXPECT_EQ(1, 1);
//  EXPECT_TRUE(true);
//
//  maz::scalar<float> f (5.0f);
//  maz::scalar<float>::TElement t;
//
//  static_assert(std::is_trivially_constructible_v<float, float>);
//
//  static_assert(maz::vec3<float>::k_sizes[0] == 3);
//  maz::vec3<float> v1(5.f, 6.0, 7.f);
//  maz::vec3<float> v2;
//  auto v3 = v1 + v2;
//
//  float x = v3.x;
//  v3.y = 5;
//}

TEST(ShapeTest, VolumeSurfaceValue) {
  
	maz::circle<float> c1;
	c1.center = maz::vector<float, 2>(0.f, 0.f);
	c1.radius = 1.0f;

	float perimeter = (float) maz::surface(c1);
	float surface = (float) maz::volume(c1);

	EXPECT_FLOAT_EQ(perimeter, 2.0f * std::numbers::pi_v<float>);
	EXPECT_FLOAT_EQ(surface, std::numbers::pi_v<float>);
}

TEST(ShapeTest, VolumeSurfaceUnits) {

	maz::circle<maz::ms::length> c1;
	c1.center = maz::vector<maz::ms::length, 2>(0.0_m, 0.0_m);
	c1.radius = 1.0_m;

	maz::ms::length perimter = maz::surface(c1);
	maz::ms::area surface = maz::volume(c1);

	EXPECT_TRUE(std::abs(perimter - 2.0_m * std::numbers::pi_v<float>) < 0.001_m);
	EXPECT_TRUE(std::abs(surface - 1.0_m2 * std::numbers::pi_v<float>) < 0.001_m2);
}

TEST(ShapeTest, CollisionsCircleVsCircleSuccess) {

	maz::circle<maz::ms::length> c1(1.0_m, 0.0_m, 0.0_m);
	maz::circle<maz::ms::length> c2(1.0_m, 0.5_m, 0.0_m);

	bool collide = maz::collides(c1, c2);

	EXPECT_TRUE(collide);
}

TEST(ShapeTest, CollisionsCircleVsCircleFail) {

	maz::circle<maz::ms::length> c1(1.0_m, 0.0_m, 0.0_m);
	maz::circle<maz::ms::length> c2(1.0_m, 3.0_m, 0.0_m);

	bool collide = maz::collides(c1, c2);

	EXPECT_FALSE(collide);
}

TEST(SurfaceTest, SurfaceCreation) {

	maz::plane<float, 3> plane(maz::vector<float, 3>(0.f, 0.f, 0.f), { maz::vector<float, 3>(1.f, 0.f, 0.f), maz::vector<float, 3>(0.f, 1.f, 0.f) });
	
}

TEST(MatrixTest, Determinant) {

	maz::matrix<float, 3, 3> m = maz::matrix<float, 3, 3>::fromRows(1.f, 0.f, 0.f,
																	0.f, 1.f, 0.f,
																	0.f, 0.f, 1.f);

	auto d = maz::determinant(m);
	EXPECT_FLOAT_EQ(d, 1.f);
}

TEST(MatrixTest, Determinant2) {

	maz::matrix<float, 3, 3> m = maz::matrix<float, 3, 3>::fromRows(1.f, 2.f, 3.f,
																	4.f, 1.f, 5.f,
																	6.f, 7.f, 1.f);

	auto d = maz::determinant(m);
	EXPECT_FLOAT_EQ(d, 84.f);
}