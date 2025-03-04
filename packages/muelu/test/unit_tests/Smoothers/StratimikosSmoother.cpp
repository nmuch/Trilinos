// @HEADER
//
// ***********************************************************************
//
//        MueLu: A package for multigrid based preconditioning
//                  Copyright 2012 Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact
//                    Jonathan Hu       (jhu@sandia.gov)
//                    Andrey Prokopenko (aprokop@sandia.gov)
//                    Ray Tuminaro      (rstumin@sandia.gov)
//
// ***********************************************************************
//
// @HEADER
#include <Teuchos_UnitTestHarness.hpp>
#include <MueLu_TestHelpers.hpp>
#include "MueLu_TestHelpersSmoothers.hpp"

#include <MueLu_StratimikosSmoother.hpp>
#include <MueLu_Utilities.hpp>

namespace MueLuTests {

// this namespace already has:  #include "MueLu_UseShortNames.hpp"
using namespace TestHelpers::Smoothers;

TEUCHOS_UNIT_TEST_TEMPLATE_4_DECL(StratimikosSmoother, NotSetup, Scalar, LocalOrdinal, GlobalOrdinal, Node) {
#include <MueLu_UseShortNames.hpp>
  MUELU_TESTING_SET_OSTREAM;
  MUELU_TESTING_LIMIT_SCOPE(Scalar, GlobalOrdinal, Node);
  if (!TYPE_EQUAL(Scalar, double)) {
    out << "Skipping for SC != double" << std::endl;
    return;
  }

  StratimikosSmoother smoother("STRATIMIKOS", Teuchos::ParameterList());
  testApplyNoSetup(smoother, out, success);
}

// Tests interface to Stratimikos's Gauss-Seidel preconditioner.
TEUCHOS_UNIT_TEST_TEMPLATE_4_DECL(StratimikosSmoother, HardCodedResult_BlockCG, Scalar, LocalOrdinal, GlobalOrdinal, Node) {
#include <MueLu_UseShortNames.hpp>
  MUELU_TESTING_SET_OSTREAM;
  MUELU_TESTING_LIMIT_SCOPE(Scalar, GlobalOrdinal, Node);
  if (!TYPE_EQUAL(Scalar, double)) {
    out << "Skipping for SC != double" << std::endl;
    return;
  }

  Teuchos::ParameterList paramList;
  paramList.set("Linear Solver Type", "Belos");
  paramList.sublist("Linear Solver Types").sublist("Belos").set("Solver Type", "Block CG");
  paramList.sublist("Linear Solver Types").sublist("Belos").sublist("Solver Types").sublist("Block CG").set("Convergence Tolerance", 1e-2);
  paramList.sublist("Linear Solver Types").sublist("Belos").sublist("Solver Types").sublist("Block CG").set("Maximum Iterations", 100);
  paramList.set("Preconditioner Type", "None");
  StratimikosSmoother smoother("STRATIMIKOS", paramList);

  typename Teuchos::ScalarTraits<SC>::magnitudeType residualNorms = testApply_A125_X0_RandomRHS(smoother, out, success);

  RCP<const Teuchos::Comm<int> > comm                                  = TestHelpers::Parameters::getDefaultComm();
  const typename Teuchos::ScalarTraits<SC>::magnitudeType expectedNorm = 0.02118661536508828144;
  TEST_FLOATING_EQUALITY(residualNorms, expectedNorm, 0.1);

}  // Block CG

#define MUELU_ETI_GROUP(SC, LO, GO, NO)                                               \
  TEUCHOS_UNIT_TEST_TEMPLATE_4_INSTANT(StratimikosSmoother, NotSetup, SC, LO, GO, NO) \
  TEUCHOS_UNIT_TEST_TEMPLATE_4_INSTANT(StratimikosSmoother, HardCodedResult_BlockCG, SC, LO, GO, NO)

#include <MueLu_ETI_4arg.hpp>

}  // namespace MueLuTests
