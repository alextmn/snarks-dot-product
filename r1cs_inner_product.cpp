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


r1cs_example<libff::Fr<libff::default_ec_pp> > gen_inner_product_protoboard(const size_t size)
{
    typedef libff::Fr<libff::default_ec_pp> FieldT;
    
        gadgetlib2::initPublicParamsFromDefaultPp();
        // necessary in case a protoboard was built before,  libsnark assumes variable indices always
        // begin with 0 so we must reset the index before creating constraints which will be used by
        // libsnark
        gadgetlib2::GadgetLibAdapter::resetVariableIndex();
    
        // create a gadgetlib2 gadget. This part is done by both generator and prover.
        auto pb = gadgetlib2::Protoboard::create(gadgetlib2::R1P);
        gadgetlib2::Variable result("result");
        gadgetlib2::VariableArray A(size, "A");
        gadgetlib2::VariableArray B(size, "B");
       
        auto g = gadgetlib2::InnerProduct_Gadget::create(pb, A, B, result);
        // create constraints. This part is done by generator.
        g->generateConstraints();
        // create assignment (witness). This part is done by prover.
        for (size_t k = 0; k < size; ++k)
        {
            pb->val(A[k]) = 2;
            pb->val(B[k]) = 5;
        }
        g->generateWitness();
        // translate constraint system to libsnark format.
        r1cs_constraint_system<FieldT> cs = get_constraint_system_from_gadgetlib2(*pb);
        // translate full variable assignment to libsnark format
        const r1cs_variable_assignment<FieldT> full_assignment = get_variable_assignment_from_gadgetlib2(*pb);
        std::cout << "OOO" << *(full_assignment.begin()) << std::flush;
        // extract primary and auxiliary input
        const r1cs_primary_input<FieldT> primary_input(full_assignment.begin(), full_assignment.begin() + 1);
        const r1cs_auxiliary_input<FieldT> auxiliary_input(full_assignment.begin() + 1, full_assignment.end());
        cs.primary_input_size +=1;
        cs.auxiliary_input_size -=1;
        assert(cs.is_valid());
        assert(cs.is_satisfied(primary_input, auxiliary_input));
    
        return r1cs_example<FieldT>(cs, primary_input, auxiliary_input);
}

} // libsnark

