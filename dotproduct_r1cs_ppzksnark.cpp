#include <cassert>
#include <cstdio>

#include <libff/common/profiling.hpp>
#include <libff/common/utils.hpp>

#include <libsnark/common/default_types/r1cs_ppzksnark_pp.hpp>
#include "r1cs_examples.hpp"
#include "run_r1cs_ppzksnark.hpp"

using namespace libsnark;

int main(int argc, const char * argv[])
{
    default_r1cs_ppzksnark_pp::init_public_params();
    libff::start_profiling();

    if (argc == 2 && strcmp(argv[1], "-v") == 0)
    {
        libff::print_compilation_info();
        return 0;
    }

    if (argc != 3 && argc != 4)
    {
        printf("usage: %s num_constraints input_size [Fr|bytes]\n", argv[0]);
        return 1;
    }
    const int num_constraints = atoi(argv[1]);
    int input_size = atoi(argv[2]);
    if (argc == 4)
    {
        assert(strcmp(argv[3], "Fr") == 0 || strcmp(argv[3], "bytes") == 0);
        if (strcmp(argv[3], "bytes") == 0)
        {
            input_size = libff::div_ceil(8 * input_size, libff::Fr<libff::default_ec_pp>::capacity());
        }
    }

    libff::enter_block("Generate R1CS example");
    r1cs_example<libff::Fr<default_r1cs_ppzksnark_pp> > example = generate_r1cs_example_with_field_input<libff::Fr<default_r1cs_ppzksnark_pp> >(num_constraints, input_size);
    libff::leave_block("Generate R1CS example");

    libff::print_header("(enter) Profile R1CS ppzkSNARK");
    const bool test_serialization = true;
    run_r1cs_ppzksnark<default_r1cs_ppzksnark_pp>(example, test_serialization);
    libff::print_header("(leave) Profile R1CS ppzkSNARK");
}
