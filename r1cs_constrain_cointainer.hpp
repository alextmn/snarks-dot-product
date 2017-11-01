#ifndef R1CS_CONSTRAIN_CONTAINER_HPP_
#define R1CS_CONSTRAIN_CONTAINER_HPP_

#include <libsnark/relations/constraint_satisfaction_problems/r1cs/r1cs.hpp>

namespace libsnark {

template<typename FieldT>
struct r1cs_constrain_cointainer {
    r1cs_constraint_system<FieldT> constraint_system;
    r1cs_primary_input<FieldT> primary_input;
    r1cs_auxiliary_input<FieldT> auxiliary_input;

    r1cs_constrain_cointainer<FieldT>() = default;
    r1cs_constrain_cointainer<FieldT>(const r1cs_constrain_cointainer<FieldT> &other) = default;
    r1cs_constrain_cointainer<FieldT>(const r1cs_constraint_system<FieldT> &constraint_system,
                         const r1cs_primary_input<FieldT> &primary_input,
                         const r1cs_auxiliary_input<FieldT> &auxiliary_input) :
        constraint_system(constraint_system),
        primary_input(primary_input),
        auxiliary_input(auxiliary_input)
    {};
    r1cs_constrain_cointainer<FieldT>(r1cs_constraint_system<FieldT> &&constraint_system,
                         r1cs_primary_input<FieldT> &&primary_input,
                         r1cs_auxiliary_input<FieldT> &&auxiliary_input) :
        constraint_system(std::move(constraint_system)),
        primary_input(std::move(primary_input)),
        auxiliary_input(std::move(auxiliary_input))
    {};
};

} // libsnark


#endif // R1CS_CONSTRAIN_CONTAINER_HPP_
