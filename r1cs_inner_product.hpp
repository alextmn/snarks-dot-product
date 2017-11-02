#ifndef INNER_PRODUCT_HPP_
#define INNER_PRODUCT_HPP_

#include <libff/common/default_types/ec_pp.hpp>

#include "r1cs_constrain_cointainer.hpp"

namespace libsnark {

    r1cs_constrain_cointainer<libff::Fr<libff::default_ec_pp> >
         fill_with_constant(const long value, const size_t size);
   
         typedef std::vector<std::tuple<long, long> > TupleVecT;

    r1cs_constrain_cointainer<libff::Fr<libff::default_ec_pp> > 
        circuit(TupleVecT& tupleVector);

} // libsnark

#endif // INNER_PRODUCT_HPP_
