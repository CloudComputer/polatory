// Copyright (c) 2016, GSI and The Polatory Authors.

#include "polatory/interpolation/rbf_direct_evaluator.hpp"

#include "polatory/numeric/sum_accumulator.hpp"

namespace polatory {
namespace interpolation {

rbf_direct_evaluator::rbf_direct_evaluator(const rbf::rbf_base& rbf, int poly_dimension, int poly_degree,
                                           const std::vector<Eigen::Vector3d>& source_points)
  : rbf_(rbf)
  , n_polynomials_(polynomial::basis_base::basis_size(poly_dimension, poly_degree))
  , src_points_(source_points) {
  if (n_polynomials_ > 0) {
    p_ = std::make_unique<PolynomialEvaluator>(poly_dimension, poly_degree);
  }
}

Eigen::VectorXd rbf_direct_evaluator::evaluate() const {
  auto y_accum = std::vector<numeric::kahan_sum_accumulator<double>>(fld_points_.size());

  for (size_t i = 0; i < src_points_.size(); i++) {
    for (size_t j = 0; j < fld_points_.size(); j++) {
      auto a_ij = rbf_.evaluate(src_points_[i], fld_points_[j]);
      y_accum[j] += weights_(i) * a_ij;
    }
  }

  if (n_polynomials_ > 0) {
    // Add polynomial terms.
    auto poly_val = p_->evaluate();
    for (size_t i = 0; i < fld_points_.size(); i++) {
      y_accum[i] += poly_val(i);
    }
  }

  Eigen::VectorXd y(fld_points_.size());
  for (size_t i = 0; i < fld_points_.size(); i++) {
    y(i) = y_accum[i].get();
  }

  return y;
}

void rbf_direct_evaluator::set_field_points(const std::vector<Eigen::Vector3d>& field_points) {
  this->fld_points_ = field_points;

  if (n_polynomials_ > 0) {
    p_->set_field_points(fld_points_);
  }
}

} // namespace interpolation
} // namespace polatory