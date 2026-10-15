/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_TestBasis_init = false;
#include "/home/mondukpe/Cours/INPS/prog_scientifique_jmm/tests/test_basis.h"

static TestBasis suite_TestBasis;

static CxxTest::List Tests_TestBasis = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestBasis( "tests/test_basis.h", 23, "TestBasis", suite_TestBasis, Tests_TestBasis );

static class TestDescription_suite_TestBasis_test_rPart_en_etat_fondamental_a_l_origine : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestBasis_test_rPart_en_etat_fondamental_a_l_origine() : CxxTest::RealTestDescription( Tests_TestBasis, suiteDescription_TestBasis, 55, "test_rPart_en_etat_fondamental_a_l_origine" ) {}
 void runTest() { suite_TestBasis.test_rPart_en_etat_fondamental_a_l_origine(); }
} testDescription_suite_TestBasis_test_rPart_en_etat_fondamental_a_l_origine;

static class TestDescription_suite_TestBasis_test_rPart_en_etat_fondamental_partout : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestBasis_test_rPart_en_etat_fondamental_partout() : CxxTest::RealTestDescription( Tests_TestBasis, suiteDescription_TestBasis, 78, "test_rPart_en_etat_fondamental_partout" ) {}
 void runTest() { suite_TestBasis.test_rPart_en_etat_fondamental_partout(); }
} testDescription_suite_TestBasis_test_rPart_en_etat_fondamental_partout;

static class TestDescription_suite_TestBasis_test_rPart_en_partout_pour_n_et_m_different_de_0 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestBasis_test_rPart_en_partout_pour_n_et_m_different_de_0() : CxxTest::RealTestDescription( Tests_TestBasis, suiteDescription_TestBasis, 101, "test_rPart_en_partout_pour_n_et_m_different_de_0" ) {}
 void runTest() { suite_TestBasis.test_rPart_en_partout_pour_n_et_m_different_de_0(); }
} testDescription_suite_TestBasis_test_rPart_en_partout_pour_n_et_m_different_de_0;

static class TestDescription_suite_TestBasis_test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_br : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestBasis_test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_br() : CxxTest::RealTestDescription( Tests_TestBasis, suiteDescription_TestBasis, 126, "test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_br" ) {}
 void runTest() { suite_TestBasis.test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_br(); }
} testDescription_suite_TestBasis_test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_br;

static class TestDescription_suite_TestBasis_test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_bz : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestBasis_test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_bz() : CxxTest::RealTestDescription( Tests_TestBasis, suiteDescription_TestBasis, 138, "test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_bz" ) {}
 void runTest() { suite_TestBasis.test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_bz(); }
} testDescription_suite_TestBasis_test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_bz;

static class TestDescription_suite_TestBasis_test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_N : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestBasis_test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_N() : CxxTest::RealTestDescription( Tests_TestBasis, suiteDescription_TestBasis, 150, "test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_N" ) {}
 void runTest() { suite_TestBasis.test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_N(); }
} testDescription_suite_TestBasis_test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_N;

static class TestDescription_suite_TestBasis_test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_Q : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestBasis_test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_Q() : CxxTest::RealTestDescription( Tests_TestBasis, suiteDescription_TestBasis, 162, "test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_Q" ) {}
 void runTest() { suite_TestBasis.test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_Q(); }
} testDescription_suite_TestBasis_test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_Q;

static class TestDescription_suite_TestBasis_test_zPart_en_etat_fondamental_a_l_origine : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestBasis_test_zPart_en_etat_fondamental_a_l_origine() : CxxTest::RealTestDescription( Tests_TestBasis, suiteDescription_TestBasis, 174, "test_zPart_en_etat_fondamental_a_l_origine" ) {}
 void runTest() { suite_TestBasis.test_zPart_en_etat_fondamental_a_l_origine(); }
} testDescription_suite_TestBasis_test_zPart_en_etat_fondamental_a_l_origine;

static class TestDescription_suite_TestBasis_test_zPart_en_etat_fondamental_partout : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestBasis_test_zPart_en_etat_fondamental_partout() : CxxTest::RealTestDescription( Tests_TestBasis, suiteDescription_TestBasis, 192, "test_zPart_en_etat_fondamental_partout" ) {}
 void runTest() { suite_TestBasis.test_zPart_en_etat_fondamental_partout(); }
} testDescription_suite_TestBasis_test_zPart_en_etat_fondamental_partout;

static class TestDescription_suite_TestBasis_test_zPart_en_partout_pour_nz_different_de_0 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestBasis_test_zPart_en_partout_pour_nz_different_de_0() : CxxTest::RealTestDescription( Tests_TestBasis, suiteDescription_TestBasis, 214, "test_zPart_en_partout_pour_nz_different_de_0" ) {}
 void runTest() { suite_TestBasis.test_zPart_en_partout_pour_nz_different_de_0(); }
} testDescription_suite_TestBasis_test_zPart_en_partout_pour_nz_different_de_0;

static class TestDescription_suite_TestBasis_test_sur_la_valeur_calculee_de_mMax : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestBasis_test_sur_la_valeur_calculee_de_mMax() : CxxTest::RealTestDescription( Tests_TestBasis, suiteDescription_TestBasis, 238, "test_sur_la_valeur_calculee_de_mMax" ) {}
 void runTest() { suite_TestBasis.test_sur_la_valeur_calculee_de_mMax(); }
} testDescription_suite_TestBasis_test_sur_la_valeur_calculee_de_mMax;

static class TestDescription_suite_TestBasis_test_sur_le_vecteur_nMax_calculee : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestBasis_test_sur_le_vecteur_nMax_calculee() : CxxTest::RealTestDescription( Tests_TestBasis, suiteDescription_TestBasis, 253, "test_sur_le_vecteur_nMax_calculee" ) {}
 void runTest() { suite_TestBasis.test_sur_le_vecteur_nMax_calculee(); }
} testDescription_suite_TestBasis_test_sur_le_vecteur_nMax_calculee;

static class TestDescription_suite_TestBasis_test_sur_le_matrice_n_zMax_calculee : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestBasis_test_sur_le_matrice_n_zMax_calculee() : CxxTest::RealTestDescription( Tests_TestBasis, suiteDescription_TestBasis, 266, "test_sur_le_matrice_n_zMax_calculee" ) {}
 void runTest() { suite_TestBasis.test_sur_le_matrice_n_zMax_calculee(); }
} testDescription_suite_TestBasis_test_sur_le_matrice_n_zMax_calculee;

static class TestDescription_suite_TestBasis_test_basisfunc : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestBasis_test_basisfunc() : CxxTest::RealTestDescription( Tests_TestBasis, suiteDescription_TestBasis, 294, "test_basisfunc" ) {}
 void runTest() { suite_TestBasis.test_basisfunc(); }
} testDescription_suite_TestBasis_test_basisfunc;

#include "/home/mondukpe/Cours/INPS/prog_scientifique_jmm/tests/test_mandatory.h"

static TestMandatory suite_TestMandatory;

static CxxTest::List Tests_TestMandatory = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestMandatory( "tests/test_mandatory.h", 19, "TestMandatory", suite_TestMandatory, Tests_TestMandatory );

static class TestDescription_suite_TestMandatory_testMandatory0 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestMandatory_testMandatory0() : CxxTest::RealTestDescription( Tests_TestMandatory, suiteDescription_TestMandatory, 22, "testMandatory0" ) {}
 void runTest() { suite_TestMandatory.testMandatory0(); }
} testDescription_suite_TestMandatory_testMandatory0;

static class TestDescription_suite_TestMandatory_testMandatory1 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestMandatory_testMandatory1() : CxxTest::RealTestDescription( Tests_TestMandatory, suiteDescription_TestMandatory, 48, "testMandatory1" ) {}
 void runTest() { suite_TestMandatory.testMandatory1(); }
} testDescription_suite_TestMandatory_testMandatory1;

static class TestDescription_suite_TestMandatory_testMandatory2 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestMandatory_testMandatory2() : CxxTest::RealTestDescription( Tests_TestMandatory, suiteDescription_TestMandatory, 76, "testMandatory2" ) {}
 void runTest() { suite_TestMandatory.testMandatory2(); }
} testDescription_suite_TestMandatory_testMandatory2;

static class TestDescription_suite_TestMandatory_testMandatory3 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestMandatory_testMandatory3() : CxxTest::RealTestDescription( Tests_TestMandatory, suiteDescription_TestMandatory, 106, "testMandatory3" ) {}
 void runTest() { suite_TestMandatory.testMandatory3(); }
} testDescription_suite_TestMandatory_testMandatory3;

#include "/home/mondukpe/Cours/INPS/prog_scientifique_jmm/tests/test_poly.h"

static TestPoly suite_TestPoly;

static CxxTest::List Tests_TestPoly = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestPoly( "tests/test_poly.h", 23, "TestPoly", suite_TestPoly, Tests_TestPoly );

static class TestDescription_suite_TestPoly_testHermite : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestPoly_testHermite() : CxxTest::RealTestDescription( Tests_TestPoly, suiteDescription_TestPoly, 31, "testHermite" ) {}
 void runTest() { suite_TestPoly.testHermite(); }
} testDescription_suite_TestPoly_testHermite;

static class TestDescription_suite_TestPoly_testLaguerre : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestPoly_testLaguerre() : CxxTest::RealTestDescription( Tests_TestPoly, suiteDescription_TestPoly, 74, "testLaguerre" ) {}
 void runTest() { suite_TestPoly.testLaguerre(); }
} testDescription_suite_TestPoly_testLaguerre;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
