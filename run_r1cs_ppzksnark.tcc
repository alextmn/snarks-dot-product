/** @file
 *****************************************************************************

 Implementation of functionality that runs the R1CS ppzkSNARK for
 a given R1CS example.

 See run_r1cs_ppzksnark.hpp .

 *****************************************************************************
 * @author     This file is part of libsnark, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#ifndef RUN_R1CS_PPZKSNARK_TCC_
#define RUN_R1CS_PPZKSNARK_TCC_

#include <sstream>
#include <type_traits>

#include <libff/common/profiling.hpp>

#include <libsnark/zk_proof_systems/ppzksnark/r1cs_ppzksnark/r1cs_ppzksnark.hpp>

#include <iostream>
#include <fstream>

namespace libsnark {

template<typename ppT>
typename std::enable_if<ppT::has_affine_pairing, void>::type
test_affine_verifier(const r1cs_ppzksnark_verification_key<ppT> &vk,
                     const r1cs_ppzksnark_primary_input<ppT> &primary_input,
                     const r1cs_ppzksnark_proof<ppT> &proof,
                     const bool expected_answer)
{
    libff::print_header("R1CS ppzkSNARK Affine Verifier");
    const bool answer = r1cs_ppzksnark_affine_verifier_weak_IC<ppT>(vk, primary_input, proof);
    assert(answer == expected_answer);
}

template<typename ppT>
typename std::enable_if<!ppT::has_affine_pairing, void>::type
test_affine_verifier(const r1cs_ppzksnark_verification_key<ppT> &vk,
                     const r1cs_ppzksnark_primary_input<ppT> &primary_input,
                     const r1cs_ppzksnark_proof<ppT> &proof,
                     const bool expected_answer)
{
    libff::UNUSED(vk, primary_input, proof, expected_answer);
    libff::print_header("R1CS ppzkSNARK Affine Verifier");
    printf("Affine verifier is not supported; not testing anything.\n");
}

/**
 * The code below provides an example of all stages of running a R1CS ppzkSNARK.
 *
 * Of course, in a real-life scenario, we would have three distinct entities,
 * mangled into one in the demonstration below. The three entities are as follows.
 * (1) The "generator", which runs the ppzkSNARK generator on input a given
 *     constraint system CS to create a proving and a verification key for CS.
 * (2) The "prover", which runs the ppzkSNARK prover on input the proving key,
 *     a primary input for CS, and an auxiliary input for CS.
 * (3) The "verifier", which runs the ppzkSNARK verifier on input the verification key,
 *     a primary input for CS, and a proof.
 */
template<typename S>
void write(const std::string fileName, S& s) {
    std::ofstream f;
    f.open (fileName, std::ios::binary);
    f << s;
    f.close();
}
template<typename S>
void read(const std::string fileName, S& s) {
    std::ifstream f;
    f.open (fileName, std::ios::binary);
    f >> s;
    f.close();
}


template<typename ppT>
std::tuple< std::string, std::string  > circuit_key_generation(
    const r1cs_constrain_cointainer<libff::Fr<ppT> > &container) {
    
    r1cs_ppzksnark_keypair<ppT> keypair = 
            r1cs_ppzksnark_generator<ppT>(container.constraint_system);

    std::ostringstream sPk, sVk;
    sPk << keypair.pk;
    sVk << keypair.vk;

    return std::make_tuple(sPk.str(), sVk.str());
}

template<typename ppT>
std::string create_proof(
        const r1cs_constrain_cointainer<libff::Fr<ppT> > &container, 
        const std::string sPk) {
    
    r1cs_ppzksnark_proving_key<ppT> pk;
    std::istringstream (sPk) >> pk;
    r1cs_ppzksnark_proof<ppT> proof = r1cs_ppzksnark_prover<ppT>(pk, container.primary_input, container.auxiliary_input);
    std::ostringstream cProof;
    cProof << proof;  
    return cProof.str();     
}

template<typename ppT>
bool verify_proof( 
        std::string sProof, std::string sVk, long result) {
    
    typedef libff::Fr<libff::default_ec_pp> FieldT;
    std::vector<FieldT> v;
    v.push_back(FieldT (result));
    const r1cs_primary_input<FieldT> r1cs_primary_input(v.begin(), v.end());
    
    r1cs_ppzksnark_verification_key<ppT> vk;
    std::istringstream(sVk) >> vk;
    r1cs_ppzksnark_proof<ppT> proof;
    std::istringstream(sProof) >> proof;
    
    const bool isOk = r1cs_ppzksnark_verifier_strong_IC<ppT>(vk, r1cs_primary_input, proof);
    return isOk;     
}

template<typename ppT>
bool run_r1cs_test(const r1cs_constrain_cointainer<libff::Fr<ppT> > &container,
                        const long result) {
  std::tuple< std::string, std::string > key = circuit_key_generation<ppT>(container);
  std::string proof = create_proof<ppT>(container, std::get<0>(key));
  bool is_ok = verify_proof<ppT>(proof, std::get<1>(key), result );
  assert(is_ok);
  return is_ok;                          
}

} // libsnark

#endif // RUN_R1CS_PPZKSNARK_TCC_
