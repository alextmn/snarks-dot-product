#ifndef INNER_PRODUCT_HPP_
#define INNER_PRODUCT_HPP_

#include <libff/common/default_types/ec_pp.hpp>

#include "r1cs_examples.hpp"

namespace libsnark {

r1cs_example<libff::Fr<libff::default_ec_pp> > gen_inner_product_protoboard(const size_t size);

} // libsnark

#endif // INNER_PRODUCT_HPP_
