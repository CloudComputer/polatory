// Copyright (c) 2016, GSI and The Polatory Authors.

#include <cmath>

#include <Eigen/Core>
#include <gtest/gtest.h>

#include <polatory/point_cloud/random_points.hpp>
#include <polatory/polynomial/orthonormal_basis.hpp>

using polatory::geometry::cuboid3d;
using polatory::point_cloud::random_points;
using polatory::polynomial::orthonormal_basis;

namespace {

void test_degree(int dimension, int degree) {
  int n_points = 100;

  auto points = random_points(cuboid3d(), n_points);

  orthonormal_basis basis(dimension, degree, points);
  auto pt = basis.evaluate_points(points);
  auto size = basis.basis_size();

  EXPECT_EQ(basis.basis_size(), pt.rows());
  EXPECT_EQ(n_points, pt.cols());

  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size; j++) {
      auto dot = std::abs(pt.row(i).dot(pt.row(j)));

      if (i == j) {
        EXPECT_LT(std::abs(dot - 1.0), 1e-12);
      } else {
        EXPECT_LT(std::abs(dot), 1e-12);
      }
    }
  }
}

}  // namespace

TEST(orthonormal_basis, trivial) {
  for (int dim = 1; dim <= 3; dim++) {
    for (int deg = 0; deg <= 2; deg++) {
      test_degree(dim, deg);
    }
  }
}
