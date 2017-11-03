#include <cassert>
#include <cstdio>

#include <libff/common/profiling.hpp>
#include <libff/common/utils.hpp>

#include <libsnark/common/default_types/r1cs_ppzksnark_pp.hpp>
#include "r1cs_constrain_cointainer.hpp"
#include "r1cs_inner_product.hpp"
#include "run_r1cs_ppzksnark.hpp"

using namespace libsnark;

int main(int argc, const char * argv[])
{
    default_r1cs_ppzksnark_pp::init_public_params();
    //libff::start_profiling();

 for (size_t i = 0; i< 1000; i++) {
    libff::enter_block("Generate R1CS example");
    r1cs_constrain_cointainer<libff::Fr<default_r1cs_ppzksnark_pp> > container = 
         fill_with_constant(1, 10);
    libff::leave_block("Generate R1CS example");

    libff::print_header("(enter) Profile R1CS ppzkSNARK");
    const bool test_serialization = true;
    run_r1cs_test<default_r1cs_ppzksnark_pp>(container, 10);
    libff::print_header("(leave) Profile R1CS ppzkSNARK");
 }
}
