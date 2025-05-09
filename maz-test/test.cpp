#include "pch.h"
#include <unordered_set>
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


TEST(CollisionTest, SAT) {

	maz::n_triangle<float, 2> tri(
		maz::vector<float, 2>(0.0f, 0.0f),
		maz::vector<float, 2>(0.0f, 1.0f),
		maz::vector<float, 2>(1.0f, 0.0f)
	);
	maz::n_aabb<float, 2> box;
	bool c = maz::collides(tri, box);
}

TEST(AABBTest, Vertices2D) {
	maz::n_aabb<int, 2> box(maz::vector<int, 2>(0, 0), maz::vector<int, 2>(1, 2));
	auto points = box.vertices();
	//std::unordered_set<maz::vector<int, 2>> pointsSet(points.begin(), points.end());

	auto contains = [&points](int x, int y)
		{
			for (const auto& v : points)
			{
				if (v.get(0) == x && v.get(1) == y)
				{
					return true;
				}
			}
			return false;
		};

	EXPECT_TRUE(points.size() == 4);
	EXPECT_TRUE(contains(0, 0));
	EXPECT_TRUE(contains(0, 2));
	EXPECT_TRUE(contains(1, 0));
	EXPECT_TRUE(contains(1, 2));
}

TEST(AABBTest, Vertices3D) {
	maz::n_aabb<int, 3> box(maz::vector<int, 3>(0, 0, 0), maz::vector<int, 3>(3, 1, 2));
	auto points = box.vertices();
	//std::unordered_set<maz::vector<int, 2>> pointsSet(points.begin(), points.end());

	auto contains = [&points](int x, int y, int z)
		{
			for (const auto& v : points)
			{
				if (v.get(0) == x && v.get(1) == y && v.get(2) == z)
				{
					return true;
				}
			}
			return false;
		};

	EXPECT_TRUE(points.size() == 8);
	EXPECT_TRUE(contains(0, 0, 0));
	EXPECT_TRUE(contains(0, 0, 2));
	EXPECT_TRUE(contains(0, 1, 0));
	EXPECT_TRUE(contains(0, 1, 2));
	EXPECT_TRUE(contains(3, 0, 0));
	EXPECT_TRUE(contains(3, 0, 2));
	EXPECT_TRUE(contains(3, 1, 0));
	EXPECT_TRUE(contains(3, 1, 2));
}