/** @file
 *****************************************************************************
 * @author     This file is part of libsnark, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#include <libsnark/gadgetlib2/adapters.hpp>
#include "r1cs_inner_product.hpp"
#include <libsnark/gadgetlib2/gadget.hpp>
#include <libsnark/gadgetlib2/integration.hpp>

namespace libsnark {
 typedef std::vector<std::tuple<long, long> > TupleVecT;

r1cs_constrain_cointainer<libff::Fr<libff::default_ec_pp> > 
    circuit(TupleVecT& tupleVector){
    
    typedef libff::Fr<libff::default_ec_pp> FieldT;
    
    gadgetlib2::initPublicParamsFromDefaultPp();
     // necessary in case a protoboard was built before,  libsnark assumes variable indices always
     // begin with 0 so we must reset the index before creating constraints which will be used by
     // libsnark
     gadgetlib2::GadgetLibAdapter::resetVariableIndex();
     auto pb = gadgetlib2::Protoboard::create(gadgetlib2::R1P);

     size_t size = tupleVector.size();
     // result index will be 0, so we can split 
     gadgetlib2::Variable result("result");
     gadgetlib2::VariableArray A(size, "A");
     gadgetlib2::VariableArray B(size, "B");

     auto g = gadgetlib2::InnerProduct_Gadget::create(pb, A, B, result);
     g->generateConstraints();

     TupleVecT::iterator it;
     size_t i = 0;
     for(it = tupleVector.begin(); it != tupleVector.end(); it++, i++ )    {
         pb->val(A[i]) = std::get<0>(*it);
         pb->val(B[i]) = std::get<1>(*it);
     }
     g->generateWitness();
     r1cs_constraint_system<FieldT> cs = get_constraint_system_from_gadgetlib2(*pb);
     
     const r1cs_variable_assignment<FieldT> full_assignment = get_variable_assignment_from_gadgetlib2(*pb);
     
     // extract primary and auxiliary input
     const r1cs_primary_input<FieldT> primary_input(full_assignment.begin(), full_assignment.begin() + 1);
     const r1cs_auxiliary_input<FieldT> auxiliary_input(full_assignment.begin() + 1, full_assignment.end());
     // "result" is a primary field and it should be given during verification
     cs.primary_input_size +=1;
     // this is private inputs and they are given when generating proof only
     cs.auxiliary_input_size -=1;
     assert(cs.is_valid());
     assert(cs.is_satisfied(primary_input, auxiliary_input));
 
     return r1cs_constrain_cointainer<FieldT>(cs, primary_input, auxiliary_input);
}

r1cs_constrain_cointainer<libff::Fr<libff::default_ec_pp> > 
    fill_with_constant(const long value, const size_t size) {
        TupleVecT in;
        for (size_t i =0; i < size; i++) {
            in.push_back( std::make_tuple(value, value));
        }
        return circuit(in);
    }

} // libsnark

