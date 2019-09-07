// AliceFlow_v0_48.cpp
// 4 ������� 2019 �������������� ������������ gcc (g++ 9.1). GNU project.
// ������������ nan, inf � ������ ������������.
// ��������� � ����� ������� � �������� /FP:strict 
// (C/C++ -> Code Generation ->> Floating Pint Model).
// �������� �++ ���������� : ��, � SEH ������������ (/EHa)
// 7-8 ��� 2019 ����������� �������������� ������������� ����� amgcl ������ ��������.
// (���� �����.)
// 6 ������ 2019 ��������������� � visual studio community edition 2019 (open source).
// 25.03.2019 ����� ������������ PVS-Studio 6.0
// 19 ����� 2019 ���������� ������������� �� ���� ������.
// 6.05.2018 LINK : fatal error LNK1102: ������������ ������ 2015 VS community.
// ����� � ���������� � ������ /bigobj
// ��������� ���������� ��� cuda :
// Tools->Options->Text Editor->File Extension ������ cu � ������ ������ add.
// 9 ���� 2017 ������� �� 64 ������ ����� int64_t.
// 15 ������ 2017 ��������������� � vs community edition 2017 (open source).
// 1 ������� 2016 �������������� �� nvidia cuda 8.0. 
// ���� ����� ������� � ����� ��� ������������� � ������ ����. 
// 11 ������ 2016 ���� ������� cl_agl_amg_v0_14.
// 15 ������� 2015 ����. ������ � Visual Studio 2015.
// AliceFlow_v0_21.cpp
// 15 ������� 2015. ������ � Visual Studio 2013.
// AliceFlow_v0_20.cpp
// 14 ������� 2015 ������������� ���������� ����������������� lusol �
// ilu2 decomposition �� 2 ������.
// AliceFlow_v0_07.cpp: ���������� ����� ����� ��� ����������� ����������.
// AliceFlow_v0_07.cpp �� ������  AliceFlow_v0_06.cpp, �� ������ � LES ������� ��������������.
// ��������� �� ������ ������������ � �������� ������ � ������ ������������� Germano.
// 17 ������ 2013 ����. ���������� ����������������� lusol_.
// 1 ������ 2013. ������ � Visual Studio 2012.
//
// AliceFlow_v0_06.cpp :
// 3D ��������� AliceFlow_v0_06.cpp ��������� �������� AliceFlowv0_05.cpp
// �������� �� ������, ��������� ������������ � ����������������.
// 
// ��������� AliceFlowv0_05.cpp, 
// ���������� ������� AliceFlowv0_03.cpp, ����������� 
// ���� �� ������� ����������� ��������������
// � ������ ����������� �� ���� �������.
// � ���� �� ��������� ������������ 
// ������������� ����������������� HEX 
// ��������� �����.
// begin one 17 ��� 2011 ����.
//
// 3D ��������� AliceFlowv0_05.cpp 
// ������������ ��������� ��������:
// 1. ��������� ��������������� �����.
// 2. �������� � ���������� ���� ��������
//    ������ ����������� ��� ������ ������ ����.
// 3. ������ ������� ����.
// 4. �������� ����.
// 5. ������� � ������������ tecplot360.
// begin two 30 ���� 2011 ����.
// begin three 14 ������� 2011 ����. ������ �� Visual Studio 2010.
// begin four 12 ����� 2012 ����. (�� ������� ����� �. ������ - ������� �� ���).
//
// �������� ������� ��������� ����� � ����������� ���� TGF2023_*
// ����� 0.2x120��� �������� 100 �������� (10-20��).
 

// ��������������� � ������ ���� ������ ���������� 
//�������������� ������������ gcc (g++ 9.1) �� GNU.
#include "stdafx.h"
//#include "pch.h"

// ���������������� � ������ ���� ������ ���������� 
//�������������� ������������ gcc (g++ 9.1) �� GNU.
//#define MINGW_COMPILLER 1

//#ifdef MINGW_COMPILLER

// ���� float 32 ���� �� ������� - �� ������� ������� ����������� ������������ ��������������� ��������.
// ��� float (���� ���������� ��������������� �������� ����� ����� ����) ���� � ���� ������� ����� ��������
// �� ��������� � ����� double (12 ������ 7).
// ��� float128 128 ��� ������� � 32 ���� ��������� ��� ��� double 64 ����. �� ���� ���������� �������� 
// 31 ������ 38 � ���� double. ������ �� ���������� � double �� �������� � � ���� float128. ������� ������������ 
// ����������� ������ ���������� �� ���� �������.
// �� ��������� ������������� ������������ ������ ��� double.


/*
// �������� ����������
// boost.org/doc/libs/1_65_1/libs/math/example/float128_example.cpp

// ��������� ��������. ��� __float128
//#include <quadmath.h>
#include <boost/cstdfloat.hpp> // For float_64_t, float128_t. Must be first include!
//#include <boost/config.hpp>
#include <boost/multiprecision/float128.hpp>
//#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/special_functions.hpp> // For gamma function.
#include <boost/math/constants/calculate_constants.hpp> // For constants pi, e ...
#include <typeinfo> //

#include <cmath> // for pow function
*/
/*
C:\AliceFlow_v0_48_GCC>g++ -std=c++11 -fexceptions -std=gnu++17 -g -fext-numeric-literals -II:\modular-boost\libs\math\include -Ii:\modular-boost -c AliceFlow_v0_48.cpp -o obj\Debug\AliceFlow_v0_48.o
C:\AliceFlow_v0_48_GCC>g++ -o bin\Debug\AliceFlow_v0_48.exe obj\Debug\AliceFlow_v0_48.o -lquadmath
*/

//#include <iostream>

//using namespace boost::multiprecision;

//#include <stdlib.h>
//#include <stdio.h>

//#endif



// /fp:except option
//#pragma float_control( except, on )

// ��� std::locale::global(std::locale("en_US.UTF-8"));
// �� ��������.
//#include <locale.h>

//using namespace std;

// ������ ������ ��� ����.

//#include "cuda_runtime.h"
//#include "device_launch_parameters.h"


#include <iostream>

#include <stdio.h>
#include <cinttypes> // ��� ���� 64 ������� ������ ����� int64_t



#include <stdlib.h> 
#include <omp.h> // OpenMP
//-Xcompiler "/openmp" 

//using namespace System;

// 0 - ������ ������ ��� ����������� ���������� ���������� windows.
#define _CUDA_IS_ACTIVE_ 0

// ������������ ����������.
#define doubleprecision 1
#if doubleprecision == 1
#ifdef MINGW_COMPILLER
// ��������� ��������.
//#define doublereal __float128
//#define doublereal  _Quad
//#define doublereal float128
//#define doublereal float // 32 ���
#define doublereal double // 64 ���
#else
//#define doublereal  _Quad
//#define doublereal float128
#define doublereal double // 64 ���
//#define doublereal long double //double // ������ ������������� ����� ������� ��������
//#define doublereal Decimal // decimal
#endif
#else
#define doublereal float //float // ������ ������������� ����� ��������� ��������
#endif

#define doubleintprecision 1


#if doubleintprecision == 1
// �������� !!! ��� ���� int64_t �� �������� ��� ������� ���������� ViennaCL.
// ���������� ������ �������� AMGCL �������� � � ����� int64_t. 
#define integer int64_t
#else
#define integer int
#endif

const integer iGLOBAL_RESTART_LIMIT = 6;
bool bglobal_restart_06_10_2018 = false;
// ��� ��������� �� �� ����� ���������� �� �� ������� ��� ����� ���� �������� ��� ������������.
bool bglobal_restart_06_10_2018_stagnation[iGLOBAL_RESTART_LIMIT+1] = {false,false,false, false,false,false, false};
integer iPnodes_count_shadow_memo = 0;

// ���������� ������ ������������� � ������������� ������, ��� �������� �� ����� ����������. 
doublereal d_GLOBAL_POWER_HEAT_GENERATION_IN_CURRENT_MODEL = 0.0; // ��

// ��������� ��������������� xyplot �������� ��� �������.
// 5.01.2018
typedef struct Tpatcher_for_print_in_report {
	doublereal fminimum = -1.0e+30, fmaximum = 1.0e+30;
	integer idir=1; // 0 - X, 1 - Y, 2 - Z.
} Patcher_for_print_in_report;

Patcher_for_print_in_report pfpir; // xyplot �������.

// 9 september 2017.
// ������ �� ������������ ����������� ������ � ����� ���������� �������� ������.
// �������� �������� �������� ��� ������������ �����, �.�. ����� ������ ��� �������� �� ������, � ���� ���������� 
// ������� ������. ������ ������� ������ �������� ������� �� ������������� ���� ���������� ��� ����� ����� � 
// ����������� ������������������ �������� ���������� �� ����� �����.
integer ireconstruction_free_construct_alloc = 1; // 0 - off, 1 - on.
// ���������� �� ��������� � ��������� ����
// �� ��������� ������� ������ ���� �� �������.
integer ianimation_write_on = 0; // 0 - off, 1 - on.

// ��� ���������� ���������� �� ������ 300 �������� ������� �� ��������� vel*rGradual_changes � ����� 
// ������ ������������������� � ����������� ��� �� ��������� vel.
doublereal rGradual_changes = 0.1; // 1.0 - �� ������������.

integer AMG1R6_LABEL = 0; // 1 - �������� ������ amg1r6.f ������; 0 - �������� amg1r5.f � ����.

// ������������� ��������� �������� ����������.
// ������ �������� ��� ���� ��������� �������.
// initialization value.
doublereal starting_speed_Vx = 0.0;
doublereal starting_speed_Vy = 0.0;
doublereal starting_speed_Vz = 0.0;

// ������� ����� ��� XY-Plot (variation Plot).
// �� ��������� ������� �����, ����� ������� �������� �����, � ���������� 
// ����� ����� ����� �� ���� ���������� ������������� ������� ���������.
doublereal Tochka_position_X0_for_XY_Plot = 0.0;
doublereal Tochka_position_Y0_for_XY_Plot = 0.0;
doublereal Tochka_position_Z0_for_XY_Plot = 0.0;
integer idirectional_for_XY_Plot = 0; // 0 - Ox axis. 

// ��� iVar==TEMP && lw==1 ����� �� ������� ����� ������������ ����� ������������ ����������� ����� V ������� ���������� ����� 0.5K.
bool bPhysics_stop = false;
// ������ ��������� ���������� ����������� ������ ��� ����.
bool bPhysics_PTBSH_memory = false;
// ������ ������ ������������� � ������ ���� :
bool bonly_solid_calculation = false;

// 3 ������� 2015 ����� ����� �������� ����� GUI ������������
// � ����� � ��� ���������� �������������� �������� � ����� ������ ����.
// ������������ �����
#define UNEVEN_MUSCL 1017  // van Leer (1977)
#define UNEVEN_SOUCUP 1018 // MINMOD
#define UNEVEN_HLPA 1019
#define UNEVEN_SMART 1020 // Gaskell and Lau (1988)
#define UNEVEN_WACEB 1021
#define UNEVEN_SMARTER 1022
#define UNEVEN_STOIC 1023 // Darwish (1993)
#define UNEVEN_CLAM 1024
#define UNEVEN_OSHER 1025 // Chakravarthy and Osher (1983)
#define UNEVEN_VONOS 1026
#define UNEVEN_LPPA 1027
#define UNEVEN_EXPONENTIAL 1028
#define UNEVEN_SUPER_C 1029
#define UNEVEN_ISNAS 1030
#define UNEVEN_CUBISTA 1031
#define UNEVEN_GAMMA 1032 // ����� � ���������� beta_m
#define UNEVEN_COPLA 1033 // 1 08 2015
#define UNEVEN_SECBC 1034 // 2 08 2015 Yu et al., (2001b) ��������, �������.
#define UNEVEN_SGSD 1035 // 3 08 2015 Li and Tao (2002)



bool bglobal_first_start_radiation = true;

// ���� �� ������ �������������� ������ ������������� � ������� ����.
bool bglobal_unsteady_temperature_determinant = false;

// ����� ��������� ���������� :
// simplemeshgen == 0 ��� unevensimplemeshgen ==1.
// �� ��������� ������������ simplemeshgen == 0.
integer iswitchMeshGenerator = 0; // ������� �������� ���������.
// �������������� ��� ������������ �������������.
integer steady_or_unsteady_global_determinant = 0; // 0 - steady, 1- unsteady.

// ������������ �� ���������� �������� ������������ ��������� �����.
bool b_on_adaptive_local_refinement_mesh = false;
integer itype_ALICE_Mesh = 1;// ��� ���� �����.

typedef struct TTimeStepLaw
{
	integer id_law=0; // 0 - Linear, 1 - Square Wave, 2 - Square Wave 2, 3 - Hot Cold (���������� �.�.)
	doublereal Factor_a_for_Linear=0.2;
	doublereal tau=60.0E-6; // ������������ �������� ��� Square Wave
	// 06_03_2017 ���������� ����� ���� � �������.
	doublereal Q=10.0; // ���������� ��� Square Wave.
	// ���������� ����� ��� ��������������� Square Wave.
	doublereal m1=1.0, tau1=0.0, tau2=0.0, tau_pause=0.0, T_all=0.0;
	integer n_cycle=20; // 20 ������.
	// hot cold reshime (double linear)
	doublereal on_time_double_linear=3.0; //3c ��������.

} TimeStepLaw;

TimeStepLaw glTSL;

// 24 ������� 2016. 
// ��� ��������� ����� ���������� ����� � ����� 0.14 ��������.
typedef struct TQuickNonlinearBoundaryCondition {
	doublereal emissivity=0.8;
	doublereal Tamb=20.0, dS=0.0;
	doublereal film_coefficient=3.0;
	bool bactive=false;
	bool bStefanBolcman_q_on = false;
	bool bNewtonRichman_q_on=false;

} QuickNonlinearBoundaryCondition;

QuickNonlinearBoundaryCondition* qnbc = NULL;
integer iadd_qnbc_maxelm = 0; // ��� ����������� ������
bool b_sign_on_nonlinear_bc = false;


// ������� �� �� SIMPLE ����������.
// ��� ����� ��� ����� ������ ��������� ������� ��� ��������� �������������.
// ������ BiCGStab_internal3 ��������.
bool bSIMPLErun_now_for_temperature = false;
// ��� ����� ��� ����� ������ ��������� ������� ��� ��������� �������������
// ��� ������� amg1r5 ���������� ����� � ����������� ����������.
bool bSIMPLErun_now_for_natural_convection = false;
// �������������� ������ ���������� ��� �����������.
doublereal* told_temperature_global_for_HOrelax = NULL;

/*
��� ���������� ������� ���������� ����� ������������ ����������� �����.
��� ����� ����� ��� ���� �� ��������������� ��-����� �� ���������.
������� ������������� ������ ��������� � ���� ��� ���������������� ����� ������ ���� �����������
��� ��������� ����� ����������� ������� ����������� � ������ �����. � �������� ����������������
������ ������� ��������������.
*/
bool *sourse2Dproblem=NULL;
doublereal *conductivity2Dinsource=NULL;

// �������������� ������ ����������.
bool bHORF = false;
bool bdontstartsolver = false;
doublereal* bPamendment_source_old = NULL;
doublereal* bsource_term_radiation_for_relax = NULL;
doublereal* b_buffer_correct_source = NULL;
// �� ��������� ������������ �����_0.14 �� ������ �� ��������.
bool bfirst_start_nonlinear_process = true;

// ������� �������-������� �� ������� ��� �����������.
integer adiabatic_vs_heat_transfer_coeff = 0; // 0 - adiabatic wall, 1 - Newton Richman condition, 2 - Stefan Bolcman condition, 3 - mix condition.
// ��� ���������� ��������� ������� �� ������ ���� ������ ���� ��������� ����� V ������. 
bool breakRUMBAcalc_for_nonlinear_boundary_condition = false;
bool bvacuumPrism = false; // ������� ��������� �����������.
bool bdouble_vacuum_PRISM = false; // ������� ��������� ����������. ��� ����� ��� ��������� ���������� ������� - ��������� �� ������ ������.
bool bBlockStefanBolcman = false; // ���� true �� ��������� ������� ���������.
doublereal film_coefficient = 0.0; // ����������� �����������.
doublereal operating_temperature_for_film_coeff = 20.0; // Tamb for Newton-Richman condition.
// ���� ��������� ���� ���������� ����� ���������������� ��������� operating_temperature_for_film_coeff
// �� � ������� �������-������� �� ������� ������� �������� ����� (���������� ������� �������) ��� ���
// ������� �������� �������������� � ���������� ������� ������� ������� � ����������� ������������ � ������������
// ��������������� ��������. ����� ����� �������� ������������ ���������� blocker_Newton_Richman.
bool blocker_Newton_Richman = true;

FILE* fp_radiation_log = NULL;
errno_t err_radiation_log;

// 1 - ������������ ������ ������� ����.
// 0 - ������������ � �������� � �������� ����.
integer ionly_solid_visible = 0;

// ������������ ����� �������������� ������������� ������� � ���������� ��� ��� ������ BiCGStab+ILU2.
// 0 - �������� BiCGStab + ILU2. 1 - �������� ��������������� �������������� ������ amg1r5.
// 2 - BiCGStab + ADI (Lr1sk).
integer iswitchsolveramg_vs_BiCGstab_plus_ILU2 = 0; // BiCGStab + ILU2.
// 0 - BiCGStab+ILU6, 1 - Direct, 2 - ����� 0.14
// for Stress Solver
integer iswitchsolveramg_vs_BiCGstab_plus_ILU6 = 0; // BiCGStab + ILU6.

bool bwait = false; // ���� false �� �� ���������� getchar().
// ���� ������ ����� �������� �� 1e-10 �� ����� ������ ����� ������ ����
#define admission 1.0e-30 //1.0e-10 // ��� ����������� ���������� ���� ������������ �����.

unsigned int calculation_vorst_seach_time = 0;

// ���� ����������� ������ �������� 
// ����������� � 200 �������� ������� 
// �� ������ ������ �� ����� (������).
// � ������ ���������� ����������� ������ TEMPERATURE_FAILURE_DC
// �� ������� ���������� ��������������� ��������� � ����� ����
// ��� ������������ ������ ��������� �������������� ����� �� ���������.
#define TEMPERATURE_FAILURE_DC 5000.2


// �������� ����� ��������� ��������� ������� �������� �������������
// ���������� �������. ��. ��� ����������� � ����� ������ �. ���������.
// BETA_PRECISION 1.0 4/3=1.333333333 6/5=1.2
#define BETA_PRECISION 1.0

// UDS  ��. my_approx_convective.c
unsigned int iFLOWScheme = 2; // ��������������� ������� �������
unsigned int iTEMPScheme = 2; // ��������������� ������� �������

// �������� ����� ������ ���������� �������� SIMPLEC
// SIMPLEC Van Doormal and Raithby, 1984 ���.
// SIMPLEC ���������� �� SIMPLE ������ � ���� �����:
// 1. � SIMPLEC �� ������������ ������ ���������� ��� �������� ��� ��������� ��������, �.�. alphaP=1.0.
// 2. � SIMPLEC ������ ����� ��������������� tau ~ alpha/(1-alpha), � � SIMPLE tau ~ alpha. 
// � ��������� ��������� ��������� ���������.
#define SIMPLE_Carretto 0 // �������� SIMPLE Carretto et al., 1973 ������������ �� ���������.
#define SIMPLEC_Van_Doormal_and_Raithby 1 // �������� SIMPLEC Van Doormal and Raithby, 1984 ���.
unsigned int iSIMPLE_alg = SIMPLE_Carretto;// SIMPLE_Carretto SIMPLEC_Van_Doormal_and_Raithby

// �������� ������� ���� ��� ���� ������� ��� �����
// (residual,residual) ��� () ��������� ������������ � R^n.
// �������� ���� ��������� ������ �������� �� �������� ������������� 
// ����. ��������� � ������ ����������� �� ��������� ����� �� �������
// ������ ������������ ������ (FVM). ������� ��� ��������� ����� ��������������
// ������������� ������� � ������ mysolverv0_03.c. ��� ������������� ���������� ��,
// ��� ���� �� ����� ������ ������ ������ ��� ������������ �������� ����������� �������,
// ����� ������� ��� ����������� �������� �������� ������� ��� ������� ������ �������� �������������
// �� ������ ����� �������� ��������� (����������). ��� ����� ���������.
// ��� ���������� ������ �������� ������� �������������������� ������� 1.0e-4
// �������� ���������� ������ �� CFX �� �������.
// ���� ����� �������� �� ������� ���������� ����� 1.0e-3.
// �������� ����� �� ������ �� CFX.
doublereal dterminatedTResudual = 1e-5; // ��� ��� Congruate Gradients � ����� BiCGStab_internal3.

doublereal globalEndTimeUnsteadyTemperatureCalculation = 1.0; // ���������� ����� ��� �������������� ������������� ������������� � ������ ����. 

// � ���� ���� ����� ������������ ���������� � ������
// �������� ��������� ����.
FILE *fp_statistic_convergence=NULL;
// � ���� ���� ����� �������������� ������ ����.
// ��� ����� ��� ������� ��������� �������� � ���������.
// �� �������� ���������� ��� ���� �������� ���������� � ���������� ��������������� ��������.
FILE *fp_log=NULL;

// ������������ ��� ��������� ������� 
// ������ ������������ �������������.
bool bsolid_static_only = false;

const integer inumcore = 2; // ����� ���� ����������
const bool bparallelizm_old = false;

// ��������� ������ ������� :
typedef struct TPARBOUND {
	integer ileft_start = -1, ileft_finish = -2;
	integer iright_start = -1, iright_finish = -2;
	integer iseparate_start = -1, iseparate_finish = -2;
	bool active=false; // ���������� ������������.
} PARBOUND;


// ��������� ������ ������������ ��� �����������������.
typedef struct TPARDATA {
	integer ncore=2; // 1, 2, 4, 8.
	integer *inumerate=NULL;
	// ��� ��� ncore==2;
	PARBOUND b0;
	// ��� ��� ncore==4;
	PARBOUND b00, b01;
	// ��� ��� ncore==8;
	PARBOUND b000, b001, b010, b011;
} PARDATA;

PARDATA nd;




// ������������ � ����� ��� ��������� �����.
doublereal* rthdsd_no_radiosity_patch = NULL;

// ��� ��������� (���� bdroblenie4=true)
// ������ �� ����� ������ ����� ��������� � �������� ��������� ��������.
typedef struct TALICE_PARTITION {
	bool bdroblenie4;
	integer iNODE1, iNODE2, iNODE3, iNODE4;
} ALICE_PARTITION;



#include "adaptive_local_refinement_mesh.cpp" // ����
#include "constr_struct.cpp" // ���������� �������� ������ TEMPER � FLOW
#include "uniformsimplemeshgen.cpp" // �������� ���������



#include "my_LR.c" // ������������ �����

#include "my_material_properties.c" // ���������� �������� ������� ����������


#include "my_linalg.cpp" // ���������� ������� �������� �������
// ��� �������: 
// eqsolve_simple_gauss - ������ ���� ������� ���������� ������
// eqsolv_simple_holesskii - ������ ���� ������� ���������� ����������

// ������������� ����������� ��������� ���������-��������
// �� ����������� �����
#include "pamendment3.c"



#include "shortest_distance.cpp" // ���������� ����������� ���������� �� ������

// 8 ������ 2016.
const bool bvery_big_memory = true; // true ��� ������ � ���� �� ������ � ����������� ������. ��� ����������� ������� �� ��������.




UNION* my_union = NULL; // ��� �����������.

// ���������� ����������
TEMPER t;
integer flow_interior; // ��������� ����� FLUID ���
FLOW* f=NULL;

// ������� �������� � ��������� tecplot360
#include "my_export_tecplot3.c"

// ���������� � ������� ���������� ������� ������������ �� 
// icepak user guide.
typedef struct TFLUENT_RESIDUAL{
	// ������ ������� ���������� �� ������ ����� ������� ����.
	// ������� ������������� � ���������� FLUENT
	// �.�. ����������� �� ������� fluent.
	doublereal res_vx; // ������� X ��������
	doublereal res_vy; // ������� Y ��������
	doublereal res_vz; // ������� Z ��������
	doublereal res_no_balance; // ������������������ ��������� �����.
	doublereal operating_value_b; // �������� ������������������ ���������� ����� � ���������� ��������.
} FLUENT_RESIDUAL;


// ���������� ��������
#include "mysolverv0_03.c"


// �������������� ������ ��� �����������
// �� ������ ������������� �������,
// � ����� �������������� ������ ��� 
// ������������� �� ������ ������������� �������.
#include "my_unsteady_temperature.c"

// ������������� ��� ������������ ���������.
#include "my_nested_dissection.cpp"

#include <ctime> // ��� ������ ������� ����������.




integer ltdp; // ���������� �������� �������� ��������� �� ����������� � �������� �����.
TEMP_DEP_POWER* gtdps=NULL; // Garber temperature depend power sequence. 

// ���� ������ ����������:
integer lmatmax; // ������������ ����� ����������
TPROP* matlist=NULL; // ��������� ���� ������ ����������







void check_data(TEMPER t) {
	if (t.potent != NULL) {
		for (integer i = 0; i < t.maxelm + t.maxbound; i++) {
			if (t.potent[i] != t.potent[i]) {
				printf("t.potent[%lld] is %e\n",i, t.potent[i]);
				system("pause");
			}
		}
	}
} // check_data



int main(void)
{



	//printLOGO();

	//system("PAUSE");

	// ���������� ������, ���������� � ������, �������.
	integer lb=0, ls=0, lw=0, lu=0;
	BLOCK* b = NULL; // ������ ������
	SOURCE* s = NULL; // ������ ����������
	WALL* w = NULL; // ������ ������ ������

	// ��� ��� � ������ bFULL_AUTOMATIC ������� ������������ �������� �
	// ������� ������������ �������, �� �������� ������� ����������� ���� ���� ���, �
	// ��� ��������� ��������� ���� ��������� � ������ ��� �������.
	// 20mm ���� ��������� � 1��� 9� �� 53� �� ���� ������ bFULL_AUTOMATIC.
	// ��� ������� ��� automatic
	// ��������� ����������� ������ ��� ��� �������.
	shorter_hash_X = new doublereal[isize_shorter_hash];
	shorter_hash_Y = new doublereal[isize_shorter_hash];
	shorter_hash_Z = new doublereal[isize_shorter_hash];
	bshorter_hash_X = new bool[isize_shorter_hash];
	bshorter_hash_Y = new bool[isize_shorter_hash];
	bshorter_hash_Z = new bool[isize_shorter_hash];


	// �������������, ���������� ��.
	pfpir.fmaximum = 1.0e+30;
	pfpir.fminimum = -1.0e+30;
	pfpir.idir = 1;
	

	// 22.01.2017 �������������.
	eqin.fluidinfo = NULL;
	t.rootBT = NULL;
	t.rootSN = NULL;
	t.rootWE = NULL;

	// 29 10 2016.
	// ������������� ����� ������ � ILU �������.
	milu_gl_buffer.alu_copy = NULL;
	milu_gl_buffer.jlu_copy = NULL;
	milu_gl_buffer.ju_copy = NULL;
	
	my_amg_manager_init();

	// ����� �������.
	unsigned int calculation_main_start_time = 0; // ������ ����� ��.
	unsigned int calculation_main_end_time = 0; // ��������� ����� ��.
	unsigned int calculation_main_seach_time = 0; // ����� ���������� ������� ���� � ��.

	calculation_main_start_time = clock(); // ������ ������ �����.

	bool bextendedprint = false; // ������ �� ��������� ����� ����������� �����.

	// ��������������� ������ ��� �������� ���������� ����������.
#ifdef MINGW_COMPILLER
	err_radiation_log = 0;
	fp_radiation_log=fopen64("log_radiation.txt", "a");
	if (fp_radiation_log == NULL) err_radiation_log = 1;
#else
	err_radiation_log = fopen_s(&fp_radiation_log, "log_radiation.txt", "a");
#endif
	
	if (err_radiation_log != 0) {
		printf("Error open file log.txt\n");
		printf("Please, press any key to continue...\n");
		//system("PAUSE");
		system("pause");
		exit(0);
	}

	//std::locale::global(std::locale("en_US.UTF-8"));
	system("mode con cols=166 lines=12000");
	// � ������� ��� ���� ����� �������� ���� ������ � ������� ����� ������� � ������� ���
	//HANDLE hOCol = GetStdHandle(STD_OUTPUT_HANDLE);
	//SetConsoleTextAttribute(hOCol, FOREGROUND_GREEN);
	// ��������� ��������� ������� (����� ���)	
	//SetConsoleTextAttribute(hOCol, BACKGROUND_BLUE |
	//	BACKGROUND_GREEN |
	//	BACKGROUND_RED |
	//	BACKGROUND_INTENSITY);

	//system("cls");
	// ���������� � ���, ��� ����� ��� ���������� ���� ��� ��� ��������� �����.
		
	// ��� ��������.
	// �� ������ ������ �����������. 
	doublereal *xpos = NULL, *ypos = NULL, *zpos = NULL;
	doublereal *xposadd = NULL, *yposadd = NULL, *zposadd = NULL;


	printf("AliceFlow 3D x64 v0.48\n");
#ifdef _OPENMP 
	omp_set_num_threads(inumcore); // ��������� ����� �������
#endif

	errno_t err=0;
#ifdef MINGW_COMPILLER
	fp_log=fopen64("log.txt", "w");
	if (fp_log == NULL) err = 1;
#else
	err = fopen_s(&fp_log, "log.txt", "w");
#endif
	
	if (err != 0) {
		printf("Error open file log.txt\n");
		printf("Please, press any key to continue...\n");
		//system("PAUSE");
		system("pause");
		exit(0);
	}

	if (fp_log != NULL) {

		//ilu0_Saadtest();
		//printf("the end Saad ilu0 test\n");
		//system("PAUSE");

		// ���������� ����� �� ������ �� ����.
		//integer inx=120, iny=64, inz=64;
		integer inx = 30, iny = 30, inz = 30;
		integer inxadd = -1, inyadd = -1, inzadd = -1;
		doublereal dgx = 0.0, dgy = 0.0, dgz = 0.0; // ���� �������
		doublereal operatingtemperature = 20.0; // Operating Temperature 20.0 ����. �.

		lu = 0;
		// lu, my_union
		loadFromFile();
		premeshin("premeshin.txt", lmatmax, lb, ls, lw, matlist, b, s, w,
			dgx, dgy, dgz, inx, iny, inz, operatingtemperature, ltdp, gtdps, lu, my_union);
		freeStringList();

		// ��������� ���� �� ����� �� ������� ��������
		// ����� ������, ������ � ���������� �����. 02.08.2019.
		BODY_CHECK(b, lb, w, lw, s, ls);

		init_QSBid(lb, b, w, lw, s, ls); // ��� ���������� ������ ������� myisblock_id.


		if ((steady_or_unsteady_global_determinant == 3) || (steady_or_unsteady_global_determinant == 9)) {
			// ��� ������� ��������� ������������� �� ������� ������ load.txt ����.
			remove("load.txt");
		}


		if (1 && steady_or_unsteady_global_determinant == 8) {
			// �������������� ����� � ������������ ����������.
			// ��� ��������� �������. 05.01.2018.
			tecplot360patcher_for_print_in_report();
			exit(1);
		}

		integer iCabinetMarker = 0;
		if (0 == iswitchMeshGenerator) {
			simplemeshgen(xpos, ypos, zpos, inx, iny, inz, lb, ls, lw, b, s, w, lu, my_union, matlist,
				xposadd, yposadd, zposadd, inxadd, inyadd, inzadd, iCabinetMarker);
		}
		else if (1 == iswitchMeshGenerator) {
			unevensimplemeshgen(xpos, ypos, zpos, inx, iny, inz, lb, ls, lw, b, s, w, lu, my_union, matlist,
				dgx, dgy, dgz, xposadd, yposadd, zposadd, inxadd, inyadd, inzadd, iCabinetMarker);
			// ��������� ������������� ����� � �������������� ������������� ���������������.
		}
		else if (2 == iswitchMeshGenerator) {
			// � ��������� ������� coarse Mesh ��� � Icepak.
			// �������� ������ ����������� ��������������-���������������, � 
			// ������� ������������� ���������� ����������� �����, �.�. CPU �������� � 4��� �
			// ���������� ����������������� ��������� ������� ������� ��������.
			coarsemeshgen(xpos, ypos, zpos, inx, iny, inz, lb, ls, lw, b, s, w, lu, my_union, matlist,
				xposadd, yposadd, zposadd, inxadd, inyadd, inzadd, iCabinetMarker);
		}
		else {
#if doubleintprecision == 1
			printf("error : yuor mesh generator is undefined %lld\n", iswitchMeshGenerator);
#else
			printf("error : yuor mesh generator is undefined %d\n", iswitchMeshGenerator);
#endif

			system("pause");
			exit(1);
		}


		// ������ ��������� ����� � ������������.
		for (integer iu = 0; iu < lu; iu++) {
			my_union[iu].inxadd = -1;
			my_union[iu].inyadd = -1;
			my_union[iu].inzadd = -1;
			my_union[iu].xposadd = NULL;
			my_union[iu].yposadd = NULL;
			my_union[iu].zposadd = NULL;
			my_union[iu].xpos = NULL;
			my_union[iu].ypos = NULL;
			my_union[iu].zpos = NULL;
			integer iup1 = iu + 1;
			switch (my_union[iu].iswitchMeshGenerator) {
			case 0: simplemeshgen(my_union[iu].xpos, my_union[iu].ypos, my_union[iu].zpos,
				my_union[iu].inx, my_union[iu].iny, my_union[iu].inz, lb, ls, lw, b, s, w, lu, my_union, matlist,
				my_union[iu].xposadd, my_union[iu].yposadd, my_union[iu].zposadd, my_union[iu].inxadd,
				my_union[iu].inyadd, my_union[iu].inzadd, iup1);
				break;
			case 1: unevensimplemeshgen(my_union[iu].xpos, my_union[iu].ypos, my_union[iu].zpos, my_union[iu].inx,
				my_union[iu].iny, my_union[iu].inz, lb, ls, lw, b, s, w, lu, my_union, matlist,
				dgx, dgy, dgz, my_union[iu].xposadd, my_union[iu].yposadd, my_union[iu].zposadd,
				my_union[iu].inxadd, my_union[iu].inyadd, my_union[iu].inzadd, iup1);
				// ��������� ������������� ����� � �������������� ������������� ���������������.
				break;
			case 2: coarsemeshgen(my_union[iu].xpos, my_union[iu].ypos, my_union[iu].zpos,
				my_union[iu].inx, my_union[iu].iny, my_union[iu].inz, lb, ls, lw, b, s, w, lu, my_union, matlist,
				my_union[iu].xposadd, my_union[iu].yposadd, my_union[iu].zposadd, my_union[iu].inxadd,
				my_union[iu].inyadd, my_union[iu].inzadd, iup1);
				break;
			default:
				coarsemeshgen(my_union[iu].xpos, my_union[iu].ypos, my_union[iu].zpos,
					my_union[iu].inx, my_union[iu].iny, my_union[iu].inz, lb, ls, lw, b, s, w, lu, my_union, matlist,
					my_union[iu].xposadd, my_union[iu].yposadd, my_union[iu].zposadd, my_union[iu].inxadd,
					my_union[iu].inyadd, my_union[iu].inzadd, iup1);
				break;
			}
			// ��������� �������� ����� ����������� �������� ��� ��������� �������� �������������.
			for (integer i76 = 0; i76 <= inx; i76++) {
				// ��������� ���������� �������� ����� ��������.
				if ((xpos[i76] >= my_union[iu].xS) && (xpos[i76] <= my_union[iu].xE)) {
					addboundary(my_union[iu].xpos, my_union[iu].inx, xpos[i76],YZ, b, lb, w, lw, s, ls);
				}
			}
			Sort_method(my_union[iu].xpos, my_union[iu].inx);
			for (integer i76 = 0; i76 <= iny; i76++) {
				// ��������� ���������� �������� ����� ��������.
				if ((ypos[i76] >= my_union[iu].yS) && (ypos[i76] <= my_union[iu].yE)) {
					addboundary(my_union[iu].ypos, my_union[iu].iny, ypos[i76],XZ, b, lb, w, lw, s, ls);
				}
			}
			Sort_method(my_union[iu].ypos, my_union[iu].iny);
			for (integer i76 = 0; i76 <= inz; i76++) {
				// ��������� ���������� �������� ����� ��������.
				if ((zpos[i76] >= my_union[iu].zS) && (zpos[i76] <= my_union[iu].zE)) {
					addboundary(my_union[iu].zpos, my_union[iu].inz, zpos[i76],XY, b, lb, w, lw, s, ls);
				}
			}
			Sort_method(my_union[iu].zpos, my_union[iu].inz);
		}



		if (b_on_adaptive_local_refinement_mesh) {
			// ������ 1TT113 22.03.2018:
			// AliceCorse 1266911 ����, � ����������������� ���������� 93592. 
			// 40��� 33� ������ ���������� �������������� �� 10000�.
			// ����� ���������� ����� 2��� 6�. 14 ������� ��������.
			// AliceFine 1092571 ����, � ����������������� ���������� 8050.
			// AliceFine (��������� �����) 1335076 ����, � ����������������� ���������� 93448.
			// ����� ���������� ����� 4��� 25�. 14 ������� ��������.
			// ����� ���������� ����� (��������� �����) 6��� 32�. 14 ������� ��������.



			printf("starting ALICE\n");
			if (0) {
				if (1 == itype_ALICE_Mesh) {
					// ��� ������ �� � ���� ������ ������ �� ������� �������� ����������� ������.
					doublereal *xpos_copy = NULL;
					// 10 ������� ������� �������� ���������.
					const integer jiterM = my_amg_manager.nu1_Temperature;
					// ������������� ��������� ������� ��������� �������.
					for (integer jiter = 0; jiter < jiterM; jiter++) {
						xpos_copy = new doublereal[2 * (inx + 1) - 1];
						for (integer i74 = 0; i74 < inx; i74++) {
							xpos_copy[2 * i74] = xpos[i74];
							xpos_copy[2 * i74 + 1] = 0.5*(xpos[i74] + xpos[i74 + 1]);
						}
						xpos_copy[2 * (inx + 1) - 2] = xpos[inx];
						delete[] xpos;
						xpos = NULL;
						xpos = new doublereal[2 * (inx + 1) - 1];
						for (integer i74 = 0; i74 < 2 * (inx + 1) - 1; i74++) {
							xpos[i74] = xpos_copy[i74];
						}
						delete[] xpos_copy;
						xpos_copy = NULL;
						inx = 2 * (inx + 1) - 2;
					}

					for (integer jiter = 0; jiter < jiterM; jiter++) {
						xpos_copy = new doublereal[2 * (iny + 1) - 1];
						for (integer i74 = 0; i74 < iny; i74++) {
							xpos_copy[2 * i74] = ypos[i74];
							xpos_copy[2 * i74 + 1] = 0.5*(ypos[i74] + ypos[i74 + 1]);
						}
						xpos_copy[2 * (iny + 1) - 2] = ypos[iny];
						delete[] ypos;
						ypos = NULL;
						ypos = new doublereal[2 * (iny + 1) - 1];
						for (integer i74 = 0; i74 < 2 * (iny + 1) - 1; i74++) {
							ypos[i74] = xpos_copy[i74];
						}
						delete[] xpos_copy;
						xpos_copy = NULL;
						iny = 2 * (iny + 1) - 2;
					}

					for (integer jiter = 0; jiter < jiterM; jiter++) {
						xpos_copy = new doublereal[2 * (inz + 1) - 1];
						for (integer i74 = 0; i74 < inz; i74++) {
							xpos_copy[2 * i74] = zpos[i74];
							xpos_copy[2 * i74 + 1] = 0.5*(zpos[i74] + zpos[i74 + 1]);
						}
						xpos_copy[2 * (inz + 1) - 2] = zpos[inz];
						delete[] zpos;
						zpos = NULL;
						zpos = new doublereal[2 * (inz + 1) - 1];
						for (integer i74 = 0; i74 < 2 * (inz + 1) - 1; i74++) {
							zpos[i74] = xpos_copy[i74];
						}
						delete[] xpos_copy;
						xpos_copy = NULL;
						inz = 2 * (inz + 1) - 2;
					}
				}
			}

			// ��� ������� ������� �������� �� ������ ������������ �������,
			// � ������ �������� �� ��������� ���� � ��������� new ��� malloc.
			integer maxelm_loc = (inx + 1)*(iny + 1)*(inz + 1);
			bool bOkal = alice_mesh(xpos, ypos, zpos, inx, iny, inz, b, lb, lw, w, s, ls, maxelm_loc, xposadd, yposadd, zposadd, inxadd, inyadd, inzadd);
			//system("PAUSE");



			if (0 || itype_ALICE_Mesh == 1/*1*/) {
				// �������� ��������� ���������.

				/*
				����� ���� ������� ����������� �� ��������� �������� �����
				��� ������� ������������������� �������� ����� ������������ 
				�� �� ���� ������������� ���� ��������� AliceMedium �����
				� ��� ��������� ���������� � ����� �������� ����� �� ����������.
				��� ���� ����� �������� ����� �� ��������� ���������� ����� 
				� ��� ������ ����� ����� ��������� ��������.
				17.08.2019
				*/
				doublereal dSTOP_flag1 = 1.0e+4; // ����� ���������� �����.
				doublereal dSTOP_flag2 = 1.0e+1; // ����� ���������� �����.

				while (!bOkal) {
					printf("repeat call ALICE...\n");
					//system("PAUSE");

					/* 3.09.2017
					���� ����� ����� ������� ����� ��� ��������� ������ ��������������� �������������� ����� �������
					����� �������. ������ ������� ������� ������������� ������ � ���������� ���� �� ����� ����������� �������������
					����� ���� ������ ���� ����� ������ ���������. � ���������� �������� ������������� ����������� ��������. ������ �����
					��������� ������� ������ �������������� ����� ����� ��������� ����� ����� ��� ������������ ����� (�.�. ����� ����������
					��� ���������� ����� �������� ����� ������� ����� ������ �������� ����� ���������� �� ����� ��� �� 1. ������� ������
					����� ����������� ��� ��������� ������ ������ � ������� if_disbalnce(...) � ��� ������ ����� ������ ����������� �������
					�������� ��������������� ����� �������� �����. �������� ����������� ������ � ����������� �� �������� ������� � ����������
					���� ����� ���������� ������ ������ ������ ������� ����� ��� �������� ����������� �������� �����.
					*/


					// ����� ���������� ������ �� ��� octree ������ � ����������� �����.
					printf("free octree start...\n");
					//system("PAUSE");
					//system("PAUSE");
					free_octree(oc_global, maxelm_loc);
					delete[] my_ALICE_STACK;
					top_ALICE_STACK = 0;
					printf("free octree end...\n");
					doublereal t_1 = NormaV(xpos, inx + 1);
					doublereal t_2 = NormaV(ypos, iny + 1);
					doublereal t_3 = NormaV(zpos, inz + 1);
					dSTOP_flag2 = sqrt(t_1*t_1+ t_2 * t_2 + t_3 * t_3);
					printf("comparison mesh %e \n", fabs(dSTOP_flag2 - dSTOP_flag1));
					//system("pause");
					
					// ����� ���������� ��������� �����.
					delete[] xpos;
					xpos = NULL;
					inx = 0;
					delete[] ypos;
					ypos = NULL;
					iny = 0;
					delete[] zpos;
					zpos = NULL;
					inz = 0;

					printf("free xpos, ypos, zpos\n");
					//system("PAUSE");

					integer iCabinetMarker = 0;
					if (0 == iswitchMeshGenerator) {
						simplemeshgen(xpos, ypos, zpos, inx, iny, inz, lb, ls, lw, b, s, w, lu, my_union,
							matlist, xposadd, yposadd, zposadd, inxadd, inyadd, inzadd, iCabinetMarker);
					}
					else if (1 == iswitchMeshGenerator) {
						unevensimplemeshgen(xpos, ypos, zpos, inx, iny, inz, lb, ls, lw, b, s, w, lu, my_union,
							matlist, dgx, dgy, dgz, xposadd, yposadd, zposadd, inxadd, inyadd, inzadd, iCabinetMarker); // ��������� ������������� ����� � �������������� ������������� ���������������.
					}
					else if (2 == iswitchMeshGenerator) {
						// � ��������� ������� coarse Mesh ��� � Icepak.
						// �������� ������ ����������� ��������������-���������������, � 
						// ������� ������������� ���������� ����������� �����, �.�. CPU �������� � 4��� �
						// ���������� ����������������� ��������� ������� ������� ��������.
						coarsemeshgen(xpos, ypos, zpos, inx, iny, inz, lb, ls, lw, b, s, w, lu, my_union,
							matlist, xposadd, yposadd, zposadd, inxadd, inyadd, inzadd, iCabinetMarker);
					}
					else {
#if doubleintprecision == 1
						printf("error : yuor mesh generator is undefined %lld\n", iswitchMeshGenerator);
#else
						printf("error : yuor mesh generator is undefined %d\n", iswitchMeshGenerator);
#endif

						system("pause");
						exit(1);
					}
					// ����� ������ ���� �����������.

					printf("new construct xpos, ypos, zpos\n");
					//system("PAUSE");

					bOkal = alice_mesh(xpos, ypos, zpos, inx, iny, inz, b, lb, lw, w, s, ls, maxelm_loc, xposadd, yposadd, zposadd, inxadd, inyadd, inzadd);

					if (fabs(dSTOP_flag2 - dSTOP_flag1) < 1.0e-20) {
						bOkal = true;
						break;
					}
					dSTOP_flag1=dSTOP_flag2;

					//system("PAUSE");
					//system("PAUSE");
				}
			}
			printf("end ALICE\n");
		}

		iCabinetMarker = 0;
		load_TEMPER_and_FLOW(t, f, inx, iny, inz, xpos, ypos, zpos, flow_interior,
			b, lb, lw, w, s, ls, lu, my_union, operatingtemperature, matlist, bextendedprint,
			dgx, dgy, dgz, b_on_adaptive_local_refinement_mesh, false, iCabinetMarker);

		t.operatingtemperature = operatingtemperature;


		for (integer iu = 0; iu < lu; iu++) {
			integer iup1 = iu + 1;
			load_TEMPER_and_FLOW(my_union[iu].t, my_union[iu].f,
				my_union[iu].inx, my_union[iu].iny, my_union[iu].inz,
				my_union[iu].xpos, my_union[iu].ypos, my_union[iu].zpos,
				my_union[iu].flow_interior,
				b, lb, lw, w, s, ls, lu, my_union, operatingtemperature, matlist, bextendedprint,
				dgx, dgy, dgz, b_on_adaptive_local_refinement_mesh, false, iup1);

			my_union[iu].t.operatingtemperature = operatingtemperature;
		}




		// ��� ����� ������ ����� ��� ������� �������������� ������.
		t.inx_copy = inx;
		t.iny_copy = iny;
		t.inz_copy = inz;
		t.operatingtemperature_copy = operatingtemperature;
		t.xpos_copy = new doublereal[inx + 1];
		t.ypos_copy = new doublereal[iny + 1];
		t.zpos_copy = new doublereal[inz + 1];
		// ������ ������������� ����� ��� �������� ����������� ������,
		// ��������� ������ ����� ��������� �� ��� � ����� ������������� 
		// ���� ���������.
		for (integer i_7 = 0; i_7 < inx + 1; i_7++) {
			t.xpos_copy[i_7] = xpos[i_7];
		}
		for (integer i_7 = 0; i_7 < iny + 1; i_7++) {
			t.ypos_copy[i_7] = ypos[i_7];
		}
		for (integer i_7 = 0; i_7 < inz + 1; i_7++) {
			t.zpos_copy[i_7] = zpos[i_7];
		}

		for (integer iu = 0; iu < lu; iu++) {
			// ��� ����� ������ ����� ��� ������� �������������� ������.
			my_union[iu].t.inx_copy = my_union[iu].inx;
			my_union[iu].t.iny_copy = my_union[iu].iny;
			my_union[iu].t.inz_copy = my_union[iu].inz;
			my_union[iu].t.operatingtemperature_copy = operatingtemperature;
			my_union[iu].t.xpos_copy = new doublereal[my_union[iu].inx + 1];
			my_union[iu].t.ypos_copy = new doublereal[my_union[iu].iny + 1];
			my_union[iu].t.zpos_copy = new doublereal[my_union[iu].inz + 1];
			// ������ ������������� ����� ��� �������� ����������� ������,
			// ��������� ������ ����� ��������� �� ��� � ����� ������������� 
			// ���� ���������.
			for (integer i_7 = 0; i_7 < my_union[iu].inx + 1; i_7++) {
				my_union[iu].t.xpos_copy[i_7] = my_union[iu].xpos[i_7];
			}
			for (integer i_7 = 0; i_7 < my_union[iu].iny + 1; i_7++) {
				my_union[iu].t.ypos_copy[i_7] = my_union[iu].ypos[i_7];
			}
			for (integer i_7 = 0; i_7 < my_union[iu].inz + 1; i_7++) {
				my_union[iu].t.zpos_copy[i_7] = my_union[iu].zpos[i_7];
			}
		}

		// ������������ ����������� ������ �� ��� octree ������.
		if (b_on_adaptive_local_refinement_mesh) {
			printf("free octree start...\n");
			//system("PAUSE");
			//system("PAUSE");
			integer maxelm_loc = (inx + 1)*(iny + 1)*(inz + 1);
			free_octree(oc_global, maxelm_loc);
			delete[] my_ALICE_STACK;
			top_ALICE_STACK = 0;
			printf("free octree end...\n");
			//system("PAUSE");
			//system("PAUSE");
		}

		if (0) {
			xyplot(f, flow_interior, t);
			printf("after load temper and flow. OK.\n");
			//system("PAUSE"); // debug avtosave
			system("pause");
		}

		// �� ���� ����� ���������� ����� �� �������� ���������������.
		if (!b_on_adaptive_local_refinement_mesh) {
			// ������ ���������� � �������� ������ ��� ������������� ������ LR:
			if (2 == iswitchsolveramg_vs_BiCGstab_plus_ILU2) {
				// Lr1sk algorithm
				constr_line(f, flow_interior);  // ��� �������������
			}
			t.rootBT = NULL;
			t.rootSN = NULL;
			t.rootWE = NULL;
			if (2 == iswitchsolveramg_vs_BiCGstab_plus_ILU2) {
				// Lr1sk algorithm
				constr_line_temp(t, b, lb); // ��� ����������������
				printf("LR preprocessing finish...\n");
			}
		}

		// ���������� ������ ��� ��������������� �������������� ������.

		amgGM.a = NULL;
		amgGM.f = NULL;
		amgGM.ia = NULL;
		amgGM.ig = NULL;
		amgGM.ja = NULL;
		amgGM.u = NULL;
		amgGM.nda = -1;
		amgGM.ndf = -1;
		amgGM.ndia = -1;
		amgGM.ndig = -1;
		amgGM.ndja = -1;
		amgGM.ndu = -1;


		//PARDATA nd;
		nd.ncore = 2; // ��� ����.
		// �� ��������� ��� ��������� ���������.
		nd.b0.active = false;
		nd.b00.active = false;
		nd.b01.active = false;
		nd.b000.active = false;
		nd.b001.active = false;
		nd.b010.active = false;
		nd.b011.active = false;
		if (0 && (1 == flow_interior)) {
			calc_front(f, f[0], t, flow_interior, ls, lw, w, nd,b,lb,s);
			// ���������� ��������� !
			printf("separator compleate...\n");
			//system("PAUSE");
		}



		t.free_temper_level1 = false; // ������ ���������������� ������������ ������ ����������� ��� ������ ������� ����� �������� ������.
		t.free_temper_level2 = false; // ������������ ������ ��� �������� ������� ��� ���������� � � SIMPLESPARSE ������.	

		printf("construction of all structures...\n");
		printf("mesh check start...\n");
		const doublereal d_zalipanie = 1.0e-23;// ������
#if doubleintprecision == 1
		doublereal minimum_gap = 1.0e60;
		for (integer i = 0; i < inx; i++) {
			if ((xpos[i + 1] - xpos[i]) < minimum_gap) minimum_gap = (xpos[i + 1] - xpos[i]);
			if ((xpos[i + 1] - xpos[i]) < d_zalipanie) {
				//printf("error: zalipanie po X: xpos[%lld]=%e xpos[%lld]=%e inx=%lld\n", i, xpos[i], i + 1, xpos[i + 1], inx);
				std::cout << "error: zalipanie po X: xpos[" << i << "]=" << xpos[i] << " xpos[" << i + 1 << "]=" << xpos[i + 1] << " inx=" << inx << std::endl;
				std::cout << "tolerance zalipanie=" << d_zalipanie << std::endl;
				system("pause");
			}
		}
		std::cout << "minimum gap X=" << minimum_gap << std::endl;
		minimum_gap = 1.0e60;
		for (integer i = 0; i < iny; i++) {
			if ((ypos[i + 1] - ypos[i]) < minimum_gap) minimum_gap = (ypos[i + 1] - ypos[i]);
			if ((ypos[i + 1] - ypos[i]) < d_zalipanie) {
				//printf("error: zalipanie po Y: ypos[%lld]=%e ypos[%lld]=%e iny=%lld\n", i, ypos[i], i + 1, ypos[i + 1], iny);
				std::cout << "error: zalipanie po Y: ypos[" << i << "]=" << ypos[i] << " ypos[" << i + 1 << "]=" << ypos[i + 1] << " iny=" << iny << std::endl;
				std::cout << "tolerance zalipanie=" << d_zalipanie << std::endl;
				system("pause");
			}
		}
		std::cout << "minimum gap Y=" << minimum_gap << std::endl;
		minimum_gap = 1.0e60;
		for (integer i = 0; i < inz; i++) {
			if ((zpos[i + 1] - zpos[i]) < minimum_gap) minimum_gap = (zpos[i + 1] - zpos[i]);
			if ((zpos[i + 1] - zpos[i]) < d_zalipanie) {
				//printf("error: zalipanie po Z: zpos[%lld]=%e zpos[%lld]=%e inz=%lld\n", i, zpos[i], i + 1, zpos[i + 1], inz);
				std::cout << "error: zalipanie po Z: zpos[" << i << "]=" << zpos[i] << " zpos[" << i + 1 << "]=" << zpos[i + 1] << " inz=" << inz << std::endl;
				std::cout << "tolerance zalipanie=" << d_zalipanie << std::endl;
				system("pause");
			}
		}
		std::cout << "minimum gap Z=" << minimum_gap << std::endl;
		for (integer iP = 0; iP < t.maxelm; iP++) {
			if ((t.nvtx[0][iP] == 0) || (t.nvtx[1][iP] == 0) || (t.nvtx[2][iP] == 0) || (t.nvtx[3][iP] == 0) || (t.nvtx[4][iP] == 0) || (t.nvtx[5][iP] == 0) || (t.nvtx[6][iP] == 0) || (t.nvtx[7][iP] == 0)) {
				printf("nvtx[%lld] : %lld %lld %lld %lld %lld %lld %lld %lld \n", iP, t.nvtx[0][iP] - 1, t.nvtx[1][iP] - 1, t.nvtx[2][iP] - 1, t.nvtx[3][iP] - 1, t.nvtx[4][iP] - 1, t.nvtx[5][iP] - 1, t.nvtx[6][iP] - 1, t.nvtx[7][iP] - 1);
			}
		}
#else
		doublereal minimum_gap = 1.0e60;
		for (integer i = 0; i < inx; i++) {
			if ((xpos[i + 1] - xpos[i]) < minimum_gap) minimum_gap = (xpos[i + 1] - xpos[i]);
			if ((xpos[i + 1] - xpos[i]) < d_zalipanie) {
				//printf("error: zalipanie po X: xpos[%d]=%e xpos[%d]=%e inx=%d\n", i, xpos[i], i + 1, xpos[i + 1], inx);
				std::cout << "error: zalipanie po X: xpos[" << i << "]=" << xpos[i] << " xpos[" << i + 1 << "]=" << xpos[i + 1] << " inx=" << inx << std::endl;
				std::cout << "tolerance zalipanie=" << d_zalipanie << std::endl;
				system("pause");
			}
		}
		std::cout << "minimum gap X=" << minimum_gap << std::endl;
		minimum_gap = 1.0e60;
		for (integer i = 0; i < iny; i++) {
			if ((ypos[i + 1] - ypos[i]) < minimum_gap) minimum_gap = (ypos[i + 1] - ypos[i]);
			if ((ypos[i + 1] - ypos[i]) < d_zalipanie) {
				//	printf("error: zalipanie po Y: ypos[%d]=%e ypos[%d]=%e iny=%d\n", i, ypos[i], i + 1, ypos[i + 1], iny);
				std::cout << "error: zalipanie po Y: ypos[" << i << "]=" << ypos[i] << " ypos[" << i + 1 << "]=" << ypos[i + 1] << " iny=" << iny << std::endl;
				std::cout << "tolerance zalipanie=" << d_zalipanie << std::endl;
				system("pause");
			}
		}
		std::cout << "minimum gap Y=" << minimum_gap << std::endl;
		minimum_gap = 1.0e60;
		for (integer i = 0; i < inz; i++) {
			if ((zpos[i + 1] - zpos[i]) < minimum_gap) minimum_gap = (zpos[i + 1] - zpos[i]);
			if ((zpos[i + 1] - zpos[i]) < d_zalipanie) {
				//		printf("error: zalipanie po Z: zpos[%d]=%e zpos[%d]=%e inz=%d\n", i, zpos[i], i + 1, zpos[i + 1], inz);
				std::cout << "error: zalipanie po Z: zpos[" << i << "]=" << zpos[i] << " zpos[" << i + 1 << "]=" << zpos[i + 1] << " inz=" << inz << std::endl;
				std::cout << "tolerance zalipanie=" << d_zalipanie << std::endl;
				system("pause");
			}
	    }
		std::cout << "minimum gap Z=" << minimum_gap << std::endl;
		for (integer iP = 0; iP < t.maxelm; iP++) {
			if ((t.nvtx[0][iP] == 0) || (t.nvtx[1][iP] == 0) || (t.nvtx[2][iP] == 0) || (t.nvtx[3][iP] == 0) || (t.nvtx[4][iP] == 0) || (t.nvtx[5][iP] == 0) || (t.nvtx[6][iP] == 0) || (t.nvtx[7][iP] == 0)) {
				printf("nvtx[%d] : %d %d %d %d %d %d %d %d \n", iP, t.nvtx[0][iP] - 1, t.nvtx[1][iP] - 1, t.nvtx[2][iP] - 1, t.nvtx[3][iP] - 1, t.nvtx[4][iP] - 1, t.nvtx[5][iP] - 1, t.nvtx[6][iP] - 1, t.nvtx[7][iP] - 1);
			}
		}
#endif
		
		
		// �� ����� ������ ������ ���� � ��������� ����������� ������� �������������.
		// ������� ������������� O(h!2) ������� ������� ������������ ��������� ������������ ������,
		// �.�. ������� �� ����������� ��������� �����.
		for (integer i = 0; i < flow_interior; i++) {
#if doubleintprecision == 1
			printf("FLUID %lld\n", i);
#else
			printf("FLUID %d\n", i);
#endif
			
			// �������� � ������� ���������������� ��������� ��� �������� ��������.
			f[i].resICCG = rterminate_residual_ICCG_Oh2(f[i]); // O(h!2)
			//printf("residual O(h!2) is equal=%e\n", f[i].resICCG);
			std::cout <<"residual O(h!2) is equal="<< f[i].resICCG << std::endl;
			f[i].resLR1sk = rterminate_residual_LR1sk_Oh3(f[i]); // O(h!3)
			//printf("residual O(h!3) is equal=%e\n", f[i].resLR1sk);
			std::cout << "residual O(h!3) is equal=" << f[i].resLR1sk << std::endl;
		}
		printf("TEMPERATURE\n");
		t.resLR1sk = rterminate_residual_LR1sk_temp_Oh3(t); // O(h!3)		
		//printf("temp residual O(h!3) is equal=%e\n", t.resLR1sk);
		std::cout << "temp residual O(h!3) is equal=" << t.resLR1sk << std::endl;
		printf("mesh check.\n");
		if (bwait) {
			//system("PAUSE");
			system("pause");
		}
		
		sourse2Dproblem = new bool[t.maxbound];
		conductivity2Dinsource = new doublereal[t.maxbound];
		// ������ ���������� ������������� ����� ��� ����������� �������.
		bsource_term_radiation_for_relax = new doublereal[t.maxelm];
		for (integer i_init = 0; i_init < t.maxelm; i_init++) bsource_term_radiation_for_relax[i_init] = 0.0;
		b_buffer_correct_source = new doublereal[t.maxelm];


		// ������� continity ����� ���������� �� ��������� � ������ 1e0.
		doublereal* continity_start = NULL;
		continity_start = new doublereal[flow_interior];
		if (continity_start == NULL) {
			// ������������ ������ �� ������ ������������.
			printf("Problem : not enough memory on your equipment for continity start in main...\n");
			printf("Please any key to exit...\n");
			exit(1);
		}
		for (integer i = 0; i < flow_interior; i++) continity_start[i] = 1.0;

		integer* inumber_iteration_SIMPLE = NULL;
		inumber_iteration_SIMPLE = new integer[flow_interior];
		if (NULL == inumber_iteration_SIMPLE) {
			// ������������ ������ �� ������ ������������.
			printf("Problem : not enough memory on your equipment for inumber_iteration_SIMPLE in main...\n");
			printf("Please any key to exit...\n");
			exit(1);
		}
		for (integer i = 0; i < flow_interior; i++) inumber_iteration_SIMPLE[i] = 0; // ��������� �������� ��������� SIMPLE ��� ������ FLUID ����.

		// ���������� ��������� ������� �� ����� ��� ������������� �������
		bool breadOk = false;
		avtoreadvalue(f, t, flow_interior, inumber_iteration_SIMPLE, continity_start, breadOk,b,lb,s,ls,w,lw);
		// ���� ���������� ������ ��������� �� breadOk==false � ��� ������ ��� ���� ������� ������ �� �������� �������� ��� �������������.

		if (b_on_adaptive_local_refinement_mesh) {
			// ��������� ������������ ���� �����.
			printf("the invariant correctness...\n");
			ANES_ALICE_CORRECT(t.maxnod, t.pa, t.maxelm, t.nvtx);
		}

		// ������� ���������� ���������� � ��������� tecplot360:
		// ����� ������������ ��� �������� ����������� �����.
		if (0) {
			exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint,0,b,lb);
			printf("read values. OK.\n");
			//system("PAUSE"); // debug avtosave
			system("pause");
		}

		

		/*for (integer i=0; i<lw; i++) {
		printf("%e  \n",w[i].Tamb);
		}
		//exporttecplotxy360T_3D(t.maxelm, t.ncell, t.nvtx, t.nvtxcell, t.pa, t.potent);
		exporttecplotxy360T_3D_part2(t.maxelm, t.potent);
		system("PAUSE"); // debug
		*/

		// 29.01.2017
		// if (1 && steady_or_unsteady_global_determinant == 2)  
		// �� �� ������ �������� ����� �� ������� �������.
		if (1 && 2 == steady_or_unsteady_global_determinant) {
			// ����� �������.
			unsigned int calculation_start_time = 0; // ������ ����� ��.
			unsigned int calculation_end_time = 0; // ��������� ����� ��.
			unsigned int calculation_seach_time = 0; // ����� ���������� ������� ���� � ��.

			calculation_start_time = clock(); // ������ ������ �����.

			// ���������� ����� ������.
			massa_cabinet(t, f, inx, iny, inz,
				xpos, ypos, zpos, flow_interior,
				b, lb, operatingtemperature,
				matlist);

			calculation_end_time = clock(); // ������ ��������� �����.
			calculation_seach_time = calculation_end_time - calculation_start_time;
			unsigned int im = 0, is = 0, ims = 0;
			im = (unsigned int)(calculation_seach_time / 60000); // ������
			is = (unsigned int)((calculation_seach_time - 60000 * im) / 1000); // �������
			ims = (unsigned int)((calculation_seach_time - 60000 * im - 1000 * is) / 10); // ������������ ������� �� 10

			printf("time export to tecplot360 is:  %d minute %d second %d millisecond\n", im, is, 10 * ims);

			// ������� ����� � tecplot 360.
			if (1) {
				if (!b_on_adaptive_local_refinement_mesh) {
					if (0 == lu) {
						// ������� ���������� ���������� � ��������� tecplot360:
						exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint, 0, b, lb);
					}
					else {
						//exporttecplotxy360T_3D_part2_assembles(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint, 0, lu, my_union);
						// ����� ��� ��������.
						exporttecplot_assembles_mesh(t, lu, my_union);
					}
				}
				else {
					// ������� � ��������� ������� �����������.
					//� ���� �����.
					//ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t,0,b,lb);
				}
			}

			
		}

		char ch_EXPORT_ALICE_ONLY = 'y';

		// steady Temperature Finite Volume Method
		if (1 && (0 == steady_or_unsteady_global_determinant)&&(eqin.itemper==1)) {

			// ����� �������.
			unsigned int calculation_start_time = 0; // ������ ����� ��.
			unsigned int calculation_end_time = 0; // ��������� ����� ��.
			unsigned int calculation_seach_time = 0; // ����� ���������� ������� ���� � ��.

			calculation_start_time = clock(); // ������ ������ �����.

			for (integer i7 = 0; i7<t.maxelm + t.maxbound; i7++) t.potent[i7] = operating_temperature_for_film_coeff; // �������������.

			// ������ ������ ������������� � ������ ����.
			bonly_solid_calculation = true;

			// �������� ����������� ���������� �� ����������� ������.
			if (1 == lw) {
				bPhysics_stop = true;
				if (lb < 11) {
					// ��� ����������� �������� :
					// MD40, AuSn, Cu, AuSn, SiC, GaN. cabinet and hollow.
					bPhysics_PTBSH_memory = true;
				}
			}

			if (adiabatic_vs_heat_transfer_coeff == 1) {
				// �� �������������� ��������� ��������� ����� ��������� �� ��������������� ��� ������ ������ ����.
				//for (integer i7 = 0; i7<t.maxelm + t.maxbound; i7++) t.potent[i7] = 0.57*operating_temperature_for_film_coeff;
			}

			// ����� �������������� ��� �� ������ ������������ ������ ������ ����������������.
			bsolid_static_only = true;
			bool bcleantemp = false;
			if (1 == eqin.itemper) {
				bcleantemp = true;
				integer i = 0; // ������� �����
				for (i = 0; i < flow_interior; i++) {
					if (1 == eqin.fluidinfo[i].iflow) bcleantemp = false;
				}
				// ���� bcleantemp==true �� �� ������ ������ ������ ������������� ��� ����� ���������.
			}
			
			if (1 || bcleantemp) {
				// ������� ������������ ���������� (��� ��������) ������ ������ ���������������� � ��������� �������. 
				printf("solution of pure heat...\n");
				printf("please, press any key to continue...\n");
				if (bwait) {
					//system("PAUSE");
					system("pause");
				}

				// ��� ������������ ������������� ����������� ��������.
				bool bprintmessage = true; // �������� �� ��������� �� �������.

				doublereal dbeta = 1.0; // ������ ������� ������������� �� �������.
				bool bmyconvective = false;
				if (starting_speed_Vx*starting_speed_Vx + starting_speed_Vy*starting_speed_Vy + starting_speed_Vz*starting_speed_Vz > 1.0e-30) {
					if (f[0].maxelm > 0) {
						bmyconvective = true;
					}
				}
				else {
					// �������� ������������� ��������� ��������.
					errno_t err_inicialization_data = 0;
					FILE* fp_inicialization_data = NULL;
#ifdef MINGW_COMPILLER
					fp_inicialization_data = fopen64("load.txt", "r");
					if (fp_inicialization_data == NULL) err_inicialization_data = 1;
#else
					err_inicialization_data = fopen_s(&fp_inicialization_data, "load.txt", "r");
#endif
					if (0 == err_inicialization_data) {
						// �������� ������ � ���� ������������.
						if (f[0].maxelm > 0) {
							// ������ � ������ ������� ������ ����� �� ������ ������ � ����������.
							bmyconvective = true;
						}
						fclose(fp_inicialization_data);
					}
					//printf("maxelm flow = %lld\n",f[0].maxelm);
					//system("PAUSE");
				}

				// if (flow_interior>0) bmyconvective=true;
				// ������ ���������� ����������,
				// ��������� ��� �������� ������� ���-���
				doublereal **rhie_chow = NULL;
				QuickMemVorst m;
				m.ballocCRSt = false; // �������� ������
				m.bsignalfreeCRSt = true; // � ����� �����������.
				// ������������� ����������.
				m.tval = NULL;
				m.tcol_ind = NULL;
				m.trow_ptr = NULL;
				m.tri = NULL;
				m.troc = NULL;
				m.ts = NULL;
				m.tt = NULL;
				m.tvi = NULL;
				m.tpi = NULL;
				m.tdx = NULL;
				m.tdax = NULL;
				m.ty = NULL;
				m.tz = NULL;
				m.ta = NULL;
				m.tja = NULL;
				m.tia = NULL;
				m.talu = NULL;
				m.tjlu = NULL;
				m.tju = NULL;
				m.tiw = NULL;
				m.tlevs = NULL;
				m.tw = NULL;
				m.tjw = NULL;
				m.icount_vel = 100000; // ����� ������� �����.			

				
				// ���� flow_interior == 0 �� f[0] ������ ���������� ��������  
				solve_nonlinear_temp(f[0], f, t,
					rhie_chow,
					b, lb, s, ls, w, lw,
					dbeta, flow_interior,
					bmyconvective, NULL, 0.001, 0.001,
					false,
					matlist, 0,
					bprintmessage,
					gtdps, ltdp, 1.0, m,
					NULL, // �������� � ����������� ���������� ����. 
					NULL,
					lu, my_union); // �������� ����� ����� ������� � ����������� ���������� ����.
				// ��������� �������� ������ 1.0 �������� ��� �������� �������.

			    
				while ((bglobal_restart_06_10_2018)) {

					// ��������� ����� ���� ����������� ���������. ��������� ������������ ������.
					if (t.potent != NULL) {
						delete[] t.potent;
						t.potent = NULL;
						t.potent = new doublereal[t.maxelm + t.maxbound];
						for (integer i7 = 0; i7<t.maxelm + t.maxbound; i7++) t.potent[i7] = operating_temperature_for_film_coeff; // �������������.
						if (bsource_term_radiation_for_relax != NULL) {
							delete[] bsource_term_radiation_for_relax;
							bsource_term_radiation_for_relax = NULL;
							bsource_term_radiation_for_relax = new doublereal[t.maxelm];
						}
						if (sourse2Dproblem != NULL) {
							delete[] sourse2Dproblem;
							sourse2Dproblem = NULL;
							sourse2Dproblem = new bool[t.maxbound];
						}
						if (conductivity2Dinsource != NULL) {
							delete[] conductivity2Dinsource;
							conductivity2Dinsource = NULL;
							conductivity2Dinsource = new doublereal[t.maxbound];
						}

						if (rthdsd_no_radiosity_patch != NULL) {
							delete[]	rthdsd_no_radiosity_patch;
							rthdsd_no_radiosity_patch = NULL;
						}

						if (b_buffer_correct_source != NULL) {
							delete[] b_buffer_correct_source;
							b_buffer_correct_source = NULL;
							b_buffer_correct_source = new doublereal[t.maxelm];
						}

						if (t.slau != NULL) {
							delete[] t.slau;
							t.slau = NULL;
							t.slau = new equation3D[t.maxelm]; // ������������ ������� ���� ��� ���������� ��.
							if (t.slau == NULL) {
								// ������������ ������ �� ������ ������������.
								printf("Problem : not enough memory on your equipment for slau temperature constr struct...\n");
								printf("Please any key to exit...\n");
								//system("PAUSE");
								system("pause");
								exit(1);
							}
						}

						if (t.slau_bon != NULL) {
							delete[] t.slau_bon;
							t.slau_bon = NULL;
							t.slau_bon = new equation3D_bon[t.maxbound]; // ������������ ������� ���� ��� ��������� ��
							if (t.slau_bon == NULL) {
								// ������������ ������ �� ������ ������������.
								printf("Problem : not enough memory on your equipment for slau boundary temperature constr struct...\n");
								printf("Please any key to exit...\n");
								//system("PAUSE");
								system("pause");
								exit(1);
							}
						}
					}
					//bglobal_restart_06_10_2018 = false;

					// ���� flow_interior == 0 �� f[0] ������ ���������� ��������  
					solve_nonlinear_temp(f[0], f, t,
						rhie_chow,
						b, lb, s, ls, w, lw,
						dbeta, flow_interior,
						bmyconvective, NULL, 0.001, 0.001,
						false,
						matlist, 0,
						bprintmessage,
						gtdps, ltdp, 1.0, m,
						NULL, // �������� � ����������� ���������� ����. 
						NULL,
						lu, my_union); // �������� ����� ����� ������� � ����������� ���������� ����.
									   // ��������� �������� ������ 1.0 �������� ��� �������� �������.

				}
				
				// ���������� ����� ������.
				massa_cabinet(t, f, inx, iny, inz,
					xpos, ypos, zpos, flow_interior,
					b, lb, operatingtemperature,
					matlist);

				// 10.10.2017
				// ���������� ���������� ������� ����� ���������.
				xyplot_temp(t, t.potent);
				//printf("graphics writing sucseful\n");
				//system("PAUSE");

				if (1) {
					if (!b_on_adaptive_local_refinement_mesh) {
						
						// ������� ���������� ���������� � ��������� tecplot360:
						exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint,0, b, lb);
					}
					else {
						if (b_on_adaptive_local_refinement_mesh) {
							printf("Would you like to save the result on the ALICE grid ? y/n\n");
							ch_EXPORT_ALICE_ONLY = getchar(); // ����� ������ getchar();
						}
						if (ch_EXPORT_ALICE_ONLY == 'y') {
							// ������� � ��������� ������� �����������.
							//� ���� �����.
							ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, f, 0, b, lb);
						}
						
					}
				}
				
			}

			doublereal tmaxfinish = -273.15; // ���������� ����.
			// ���������� �������� ������������ ����������� ������ ��������� ������� � �� � ��������:
			//for (integer i = 0; i < t.maxelm + t.maxbound; i++) tmaxfinish = fmax(tmaxfinish, fabs(t.potent[i]));
			// 23 ������� 2015
			// �� ��������� ������ ���������� ����� �� ����� ��������� ������� �����������, �������
			// �������� �� ������� ����� � ��������� ����������� ������ �� ���������� ��. 
			for (integer i = 0; i < t.maxelm; i++) tmaxfinish = fmax(tmaxfinish, t.potent[i]);
			
			FILE *fp = NULL;
			errno_t err1 = 0;
#ifdef MINGW_COMPILLER
			fp = fopen64("report.txt", "w");
			if (fp == NULL) err1 = 1;
#else
			err1 = fopen_s(&fp, "report.txt", "w");
#endif
			// �������� ����� ��� ������.
			if ((err1) != 0) {
				printf("Create File report.txt Error\n");
				//system("PAUSE");
				system("pause");
			}
			else {
				// ������ ���������
				fprintf(fp, "Maximum Temperature %.2f\n", tmaxfinish);
				fclose(fp);
			}
			// 1 - solver/solid_static/
			report_temperature(flow_interior, f, t, b, lb, s, ls, w, lw, 0);

			if (ch_EXPORT_ALICE_ONLY != 'y') {

				calculation_end_time = clock(); // ������ ��������� �����.
				calculation_seach_time = calculation_end_time - calculation_start_time;
				unsigned int im = 0, is = 0, ims = 0;
				im = (unsigned int)(calculation_seach_time / 60000); // ������
				is = (unsigned int)((calculation_seach_time - 60000 * im) / 1000); // �������
				ims = (unsigned int)((calculation_seach_time - 60000 * im - 1000 * is) / 10); // ������������ ������� �� 10

				printf("time calculation is:  %d minute %d second %d millisecond\n", im, is, 10 * ims);

			    // �� �������� �������������� ������ (11.4� ����� �� ���� Coarse )
				// ����������� ����������� �����. 

				// 25.11.2017
				// 1. �������� ������ � ����������� � ����� �� ����.
				// 2. ��������� �� � ����������� ������.
				// 3. ���������� ������.
				// 4. ��������� ������� ���������� ������������� �����.
				// 5. ��������� ������ � ����������� � ���� �� ������� ���������� ������������� �����.
				// 6. ��������������� ����������� � ����������� �� ����������������� ����� �������� ������.
				// 7.1 ������� �������� ������������� �� ��� ���� �� ����������������� ����� � � ������� � � ������. 
				// 7.2 ������������ �� ���� ����� � ������ ���� �������� ����� ���������� ���� �� ������������������ (���� ��������).


				if (b_on_adaptive_local_refinement_mesh) {
					// 1. ��������� x,y,z,T,nvtx, m_sizeT, m_size_nvtx.
					doublereal *x_buf = NULL;
					doublereal *y_buf = NULL;
					doublereal *z_buf = NULL;
					doublereal *t_buf = NULL;
					integer **nvtx_buf = NULL;
					integer m_sizeT = 0, m_size_nvtx = 0;

					ANES_tecplot360_export_temperature_preobrazovatel(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, x_buf, y_buf, z_buf, t_buf, nvtx_buf, m_sizeT, m_size_nvtx, operatingtemperature);

					// 2. ������������ ������.
					// ������������ ����������� ������.
					if (t.xpos_copy != NULL) {
						delete[] t.xpos_copy;
						t.xpos_copy = NULL;
					}
					if (t.ypos_copy != NULL) {
						delete[] t.ypos_copy;
						t.ypos_copy = NULL;
					}
					if (t.zpos_copy != NULL) {
						delete[] t.zpos_copy;
						t.zpos_copy = NULL;
					}


					if (bsource_term_radiation_for_relax != NULL) {
						delete[] bsource_term_radiation_for_relax; // ���������� ������������ ������ ������������ �������.
						bsource_term_radiation_for_relax = NULL;
					}
					if (b_buffer_correct_source != NULL) {
						delete[] b_buffer_correct_source;
						b_buffer_correct_source = NULL;
					}

					if (rthdsd_no_radiosity_patch != NULL) {
						free(rthdsd_no_radiosity_patch);
					}
					rthdsd_no_radiosity_patch = NULL;

					// ������� ��������� ���������� ��������� ������� � ����� 0.14 ��������.
					if (qnbc != NULL) {
						delete[] qnbc;
						qnbc = NULL;
						iadd_qnbc_maxelm = 0;
					}

					// ����� ���������� ����������� ������ �� ��� ���� �������� ������:
					free_level1_temp(t);
					free_level2_temp(t); // ������������ ������ �� ��� ������.
										 // ����������� ������ ��� LR ������.
					if (t.rootWE != NULL) {
						free_root(t.rootWE, t.iWE);
					}
					if (t.rootSN != NULL) {
						free_root(t.rootSN, t.iSN);
					}
					if (t.rootBT != NULL) {
						free_root(t.rootBT, t.iBT);
					}
					if (t.rootWE != NULL) {
						delete[] t.rootWE;
						t.rootWE = NULL;
					}
					if (t.rootSN != NULL) {
						delete[] t.rootSN;
						t.rootSN = NULL;
					}
					if (t.rootBT != NULL) {
						delete[] t.rootBT;
						t.rootBT = NULL;
					}

					if (bvery_big_memory) {
						if (t.database.x != NULL) {
							free(t.database.x);
						}
						if (t.database.y != NULL) {
							free(t.database.y);
						}
						if (t.database.z != NULL) {
							free(t.database.z);
						}
						if (t.database.nvtxcell != NULL) {
							for (integer i = 0; i <= 7; i++) {
								delete[] t.database.nvtxcell[i];
							}
							delete[] t.database.nvtxcell;
						}
						if (t.database.ptr != NULL) {
							if (t.database.ptr[0] != NULL) {
								delete[] t.database.ptr[0];
							}
							if (t.database.ptr[1] != NULL) {
								delete[] t.database.ptr[1];
							}
							delete[] t.database.ptr;
						}
					}

					// ������������ ������ ��� LR �����.
					free_level1_flow(f, flow_interior);
					free_level2_flow(f, flow_interior); // ������������ ������ �� ��� ������.

					if (sourse2Dproblem != NULL) {
						delete[] sourse2Dproblem;
						sourse2Dproblem = NULL;
					}
					if (conductivity2Dinsource != NULL) {
						delete[] conductivity2Dinsource;
						conductivity2Dinsource = NULL;
					}

					if (x_jacoby_buffer != NULL) {
						// 30 ������� 2016. 
						// � seidelsor2 ������ ������������� �� ����� ������ ���������� �.�. �����.
						// ������������ ������ �� ��� jacobi buffer.
						delete[] x_jacoby_buffer;
					}

					
					/*
					// ������������ ����� ������ � ILU �������.
					if (milu_gl_buffer.alu_copy != NULL) delete[] milu_gl_buffer.alu_copy;
					if (milu_gl_buffer.jlu_copy != NULL) delete[] milu_gl_buffer.jlu_copy;
					if (milu_gl_buffer.ju_copy != NULL) delete[] milu_gl_buffer.ju_copy;
					milu_gl_buffer.alu_copy = NULL;
					milu_gl_buffer.jlu_copy = NULL;
					milu_gl_buffer.ju_copy = NULL;
					*/
					flow_interior = 0;

					// 3. ���������� ������� �����.

					b_on_adaptive_local_refinement_mesh = false;
					iCabinetMarker = 0;
					load_TEMPER_and_FLOW(t, f, inx, iny, inz, xpos, ypos, zpos, flow_interior,
						b, lb, lw, w, s, ls, lu, my_union, operatingtemperature, matlist, bextendedprint, 
						dgx, dgy, dgz, b_on_adaptive_local_refinement_mesh, false, iCabinetMarker);

					t.operatingtemperature=operatingtemperature;


					// ��� ����� ������ ����� ��� ������� �������������� ������.
					t.inx_copy = inx;
					t.iny_copy = iny;
					t.inz_copy = inz;
					t.operatingtemperature_copy = operatingtemperature;
					t.xpos_copy = new doublereal[inx + 1];
					t.ypos_copy = new doublereal[iny + 1];
					t.zpos_copy = new doublereal[inz + 1];
					// ������ ������������� ����� ��� �������� ����������� ������,
					// ��������� ������ ����� ��������� �� ��� � ����� ������������� 
					// ���� ���������.
					for (integer i_7 = 0; i_7 < inx + 1; i_7++) {
						t.xpos_copy[i_7] = xpos[i_7];
					}
					for (integer i_7 = 0; i_7 < iny + 1; i_7++) {
						t.ypos_copy[i_7] = ypos[i_7];
					}
					for (integer i_7 = 0; i_7 < inz + 1; i_7++) {
						t.zpos_copy[i_7] = zpos[i_7];
					}

					t.free_temper_level1 = false; // ������ ���������������� ������������ ������ ����������� ��� ������ ������� ����� �������� ������.
					t.free_temper_level2 = false; // ������������ ������ ��� �������� ������� ��� ���������� � � SIMPLESPARSE ������.	


					// 4. ������������� ��� �����������.
					ALICE_2_Structural(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, x_buf, y_buf, z_buf, t_buf, nvtx_buf, m_sizeT, m_size_nvtx, t.operatingtemperature_copy);


					if (x_buf != NULL) {
						delete[] x_buf;
						x_buf = NULL;
					}
					if (y_buf != NULL) {
						delete[] y_buf;
						y_buf = NULL;
					}
					if (z_buf != NULL) {
						delete[] z_buf;
						z_buf = NULL;
					}
					if (t_buf != NULL) {
						delete[] t_buf;
						t_buf = NULL;
					}
					if (nvtx_buf != NULL) {
						for (integer i_1 = 0; i_1 < 8; i_1++) {
							if (nvtx_buf[i_1] != NULL) {
								delete[] nvtx_buf[i_1];
								nvtx_buf[i_1] = NULL;
							}
						}
						delete[] nvtx_buf;
						nvtx_buf = NULL;
					}
					m_sizeT = 0, m_size_nvtx = 0;
					// 5. ������� ������� � �������.
					exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint, 0, b, lb);
				}
			}
			else {
				// �������� ������������ �� ���� ����� ������ ���� � ������ �����, �������
				// ��� ������������ �������� ������������ ����� ������� �� ����������������� �����.
				// � ������� �� ���� ����� ��� ������ �� ��������.
				// �������� �� ���� ����� �����. ����� �������� ������� �������� ��� ���� ����������, ��
				// ��������� �������� ������� ������� �� ����� ������� (������������ �����) ������ � ������
				// ��������������� ������ ���� ������ ����� ����� ���� ����� ��������� (���������������� ������).

				// ������� � ��������� ������� �����������.
				// � ���� �����.
				//ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent,t,0,b,lb);
			}

			

		}

		// steady Temperature Finite Element Method
		if (1 && (0 == steady_or_unsteady_global_determinant) && (eqin.itemper == 2)) {
		   
			// ����� �������.
			unsigned int calculation_start_time = 0; // ������ ����� ��.
			unsigned int calculation_end_time = 0; // ��������� ����� ��.
			unsigned int calculation_seach_time = 0; // ����� ���������� ������� ���� � ��.

			calculation_start_time = clock(); // ������ ������ �����.

			for (integer i7 = 0; i7 < t.maxelm + t.maxbound; i7++) t.potent[i7] = operating_temperature_for_film_coeff; // �������������.

			// ������ ������ Static Structural.
			bonly_solid_calculation = true;

			// �������� ����������� ���������� �� ����������� ������.
			if (lw == 1) {
				bPhysics_stop = true;
				if (lb < 11) {
					// ��� ����������� �������� :
					// MD40, AuSn, Cu, AuSn, SiC, GaN. cabinet and hollow.
					bPhysics_PTBSH_memory = true;
				}
			}

			if (adiabatic_vs_heat_transfer_coeff == 1) {
				// �� �������������� ��������� ��������� ����� ��������� �� ��������������� ��� ������ ������ ����.
				//for (integer i7 = 0; i7<t.maxelm + t.maxbound; i7++) t.potent[i7] = 0.57*operating_temperature_for_film_coeff;
			}

			// ����� �������������� ��� �� ������ ������������ ������ ������ ����������������.
			bsolid_static_only = true;
			bool bcleantemp = false;
			if (eqin.itemper == 1) {
				bcleantemp = true;
				integer i = 0; // ������� �����
				for (i = 0; i < flow_interior; i++) {
					if (eqin.fluidinfo[i].iflow == 1) bcleantemp = false;
				}
				// ���� bcleantemp==true �� �� ������ ������ ������ ������������� ��� ����� ���������.
			}
			if (1 || bcleantemp) {
				// ������� ������������ ���������� (��� ��������) ������ ������ ���������������� � ��������� �������. 
				printf("solution of pure Static Structural...\n");
				printf("please, press any key to continue...\n");
				if (bwait) {
					//system("PAUSE");
					system("pause");
				}

				// ��� ������������ ������������� ����������� ��������.
				bool bprintmessage = true; // �������� �� ��������� �� �������.

				doublereal dbeta = 1.0; // ������ ������� ������������� �� �������.
				bool bmyconvective = false;
				if (starting_speed_Vx*starting_speed_Vx + starting_speed_Vy * starting_speed_Vy + starting_speed_Vz * starting_speed_Vz > 1.0e-30) {
					if (f[0].maxelm > 0) {
						bmyconvective = true;
					}
				}
				else {
					// �������� ������������� ��������� ��������.
					errno_t err_inicialization_data = 0;
					FILE* fp_inicialization_data = NULL;
#ifdef MINGW_COMPILLER
					fp_inicialization_data = fopen64("load.txt", "r");
					if (fp_inicialization_data == NULL) err_inicialization_data = 1;
#else
					err_inicialization_data = fopen_s(&fp_inicialization_data, "load.txt", "r");
#endif

					if (err_inicialization_data == 0) {
						// �������� ������ � ���� ������������.
						if (f[0].maxelm > 0) {
							bmyconvective = true;
						}
						fclose(fp_inicialization_data);
					}
				}

				// if (flow_interior>0) bmyconvective=true;
				// ������ ���������� ����������,
				// ��������� ��� �������� ������� ���-���
				doublereal **rhie_chow = NULL;
				QuickMemVorst m;
				m.ballocCRSt = false; // �������� ������
				m.bsignalfreeCRSt = true; // � ����� �����������.

										  // ������������� ����������.
				m.tval = NULL;
				m.tcol_ind = NULL;
				m.trow_ptr = NULL;
				m.tri = NULL;
				m.troc = NULL;
				m.ts = NULL;
				m.tt = NULL;
				m.tvi = NULL;
				m.tpi = NULL;
				m.tdx = NULL;
				m.tdax = NULL;
				m.ty = NULL;
				m.tz = NULL;
				m.ta = NULL;
				m.tja = NULL;
				m.tia = NULL;
				m.talu = NULL;
				m.tjlu = NULL;
				m.tju = NULL;
				m.tiw = NULL;
				m.tlevs = NULL;
				m.tw = NULL;
				m.tjw = NULL;
				m.icount_vel = 100000; // ����� ������� �����.

				bPhysics_stop = false;
				
				// ����������� 19.05.2018

				doublereal* lstub = NULL;
				integer maxelm_global_ret = 0;
				solve_Thermal(t, f, matlist, w, lw, lu, b, lb, m, false, operatingtemperature, false, 0.0, lstub, lstub, maxelm_global_ret, 1.0);


				/*
				// ���� flow_interior == 0 �� f[0] ������ ���������� ��������
				solve_nonlinear_temp(f[0], f, t,
				rhie_chow,
				b, lb, s, ls, w, lw,
				dbeta, flow_interior,
				bmyconvective, NULL, 0.001, 0.001,
				false,
				matlist, 0,
				bprintmessage,
				gtdps, ltdp, 1.0, m,
				NULL, // �������� � ����������� ���������� ����.
				NULL); // �������� ����� ����� ������� � ����������� ���������� ����.
				// ��������� �������� ������ 1.0 �������� ��� �������� �������.
				*/
				// ���������� ����� ������.
				massa_cabinet(t, f, inx, iny, inz,
					xpos, ypos, zpos, flow_interior,
					b, lb, operatingtemperature,
					matlist);


				calculation_end_time = clock(); // ������ ��������� �����.
				calculation_seach_time = calculation_end_time - calculation_start_time;
				unsigned int im = 0, is = 0, ims = 0;
				im = (unsigned int)(calculation_seach_time / 60000); // ������
				is = (unsigned int)((calculation_seach_time - 60000 * im) / 1000); // �������
				ims = (unsigned int)((calculation_seach_time - 60000 * im - 1000 * is) / 10); // ������������ ������� �� 10

				printf("time calculation is:  %d minute %d second %d millisecond\n", im, is, 10 * ims);

				if (1) {
					if (!b_on_adaptive_local_refinement_mesh) {
						// ������� ���������� ���������� � ��������� tecplot360:
						exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint, 0, b, lb);
					}
					else {
						// ������� � ��������� ������� �����������.
						//� ���� �����.
						ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, f, 0, b, lb);
					}
				}

			}

			doublereal tmaxfinish = -273.15; // ���������� ����.
											 // ���������� �������� ������������ ����������� ������ ��������� ������� � �� � ��������:
											 //for (integer i = 0; i < t.maxelm + t.maxbound; i++) tmaxfinish = fmax(tmaxfinish, fabs(t.potent[i]));
											 // 23 ������� 2015
											 // �� ��������� ������ ���������� ����� �� ����� ��������� ������� �����������, �������
											 // �������� �� ������� ����� � ��������� ����������� ������ �� ���������� ��. 
			for (integer i = 0; i < t.maxelm; i++) tmaxfinish = fmax(tmaxfinish, t.potent[i]);

			doublereal totaldeform_max = -1.0e+30;
			for (integer i = 0; i < t.maxelm; i++) totaldeform_max = fmax(totaldeform_max, t.total_deformation[TOTALDEFORMATION][i]);

			FILE *fp = NULL;
			errno_t err1 = 0;
#ifdef MINGW_COMPILLER
			fp = fopen64("report.txt", "w");
			if (fp == NULL) err1 = 1;
#else
			err1 = fopen_s(&fp, "report.txt", "w");
#endif
			// �������� ����� ��� ������.
			if ((err1) != 0) {
				printf("Create File report.txt Error\n");
				//system("PAUSE");
				system("pause");
			}
			else {
				// ������ ���������
				fprintf(fp, "Maximum Temperature %.2f\n", tmaxfinish);
				fclose(fp);
			}
			// 1 - solver/solid_static/
			report_temperature(flow_interior, f, t, b, lb, s, ls, w, lw, 0);

		
		}

		// steady Static Structural
		if (1 && steady_or_unsteady_global_determinant == 5) {


			// ����� �������.
			unsigned int calculation_start_time = 0; // ������ ����� ��.
			unsigned int calculation_end_time = 0; // ��������� ����� ��.
			unsigned int calculation_seach_time = 0; // ����� ���������� ������� ���� � ��.

			calculation_start_time = clock(); // ������ ������ �����.

			for (integer i7 = 0; i7<t.maxelm + t.maxbound; i7++) t.potent[i7] = operating_temperature_for_film_coeff; // �������������.

		    // ������ ������ Static Structural.
			bonly_solid_calculation = true;

			// �������� ����������� ���������� �� ����������� ������.
			if (lw == 1) {
				bPhysics_stop = true;
				if (lb < 11) {
					// ��� ����������� �������� :
					// MD40, AuSn, Cu, AuSn, SiC, GaN. cabinet and hollow.
					bPhysics_PTBSH_memory = true;
				}
			}

			if (adiabatic_vs_heat_transfer_coeff == 1) {
				// �� �������������� ��������� ��������� ����� ��������� �� ��������������� ��� ������ ������ ����.
				//for (integer i7 = 0; i7<t.maxelm + t.maxbound; i7++) t.potent[i7] = 0.57*operating_temperature_for_film_coeff;
			}

			// ����� �������������� ��� �� ������ ������������ ������ ������ ����������������.
			bsolid_static_only = true;
			bool bcleantemp = false;
			if (eqin.itemper == 1) {
				bcleantemp = true;
				integer i = 0; // ������� �����
				for (i = 0; i < flow_interior; i++) {
					if (eqin.fluidinfo[i].iflow == 1) bcleantemp = false;
				}
				// ���� bcleantemp==true �� �� ������ ������ ������ ������������� ��� ����� ���������.
			}
			if (1 || bcleantemp) {
				// ������� ������������ ���������� (��� ��������) ������ ������ ���������������� � ��������� �������. 
				printf("solution of pure Static Structural...\n");
				printf("please, press any key to continue...\n");
				if (bwait) {
					//system("PAUSE");
					system("pause");
				}

				// ��� ������������ ������������� ����������� ��������.
				bool bprintmessage = true; // �������� �� ��������� �� �������.

				doublereal dbeta = 1.0; // ������ ������� ������������� �� �������.
				bool bmyconvective = false;
				if (starting_speed_Vx*starting_speed_Vx + starting_speed_Vy*starting_speed_Vy + starting_speed_Vz*starting_speed_Vz > 1.0e-30) {
					if (f[0].maxelm > 0) {
						bmyconvective = true;
					}
				}
				else {
					// �������� ������������� ��������� ��������.
					errno_t err_inicialization_data = 0;
					FILE* fp_inicialization_data = NULL;
#ifdef MINGW_COMPILLER
					fp_inicialization_data = fopen64("load.txt", "r");
					if (fp_inicialization_data == NULL) err_inicialization_data = 1;
#else
					err_inicialization_data = fopen_s(&fp_inicialization_data, "load.txt", "r");
#endif

					if (err_inicialization_data == 0) {
						// �������� ������ � ���� ������������.
						if (f[0].maxelm > 0) {
							bmyconvective = true;
						}
						fclose(fp_inicialization_data);
					}
				}

				// if (flow_interior>0) bmyconvective=true;
				// ������ ���������� ����������,
				// ��������� ��� �������� ������� ���-���
				doublereal **rhie_chow = NULL;
				QuickMemVorst m;
				m.ballocCRSt = false; // �������� ������
				m.bsignalfreeCRSt = true; // � ����� �����������.
				
										  // ������������� ����������.
				m.tval = NULL;
				m.tcol_ind = NULL;
				m.trow_ptr = NULL;
				m.tri = NULL;
				m.troc = NULL;
				m.ts = NULL;
				m.tt = NULL;
				m.tvi = NULL;
				m.tpi = NULL;
				m.tdx = NULL;
				m.tdax = NULL;
				m.ty = NULL;
				m.tz = NULL;
				m.ta = NULL;
				m.tja = NULL;
				m.tia = NULL;
				m.talu = NULL;
				m.tjlu = NULL;
				m.tju = NULL;
				m.tiw = NULL;
				m.tlevs = NULL;
				m.tw = NULL;
				m.tjw = NULL;
				m.icount_vel = 100000; // ����� ������� �����.

				bPhysics_stop = false;
				// ����� ������� Static Structural.
				// �������� 19.05.2018
				//solve_Structural(t, w, lw, m, false, operatingtemperature);
				bPhysics_stop = true;
				// ����������� 19.05.2018
				
				//doublereal* lstub = NULL;
				//integer maxelm_global_ret = 0;
				//solve_Thermal(t, f, matlist, w, lw, lu, b, lb, m, false, operatingtemperature,false, 0.0, lstub, lstub, maxelm_global_ret, 1.0);
				

				/*
				// ���� flow_interior == 0 �� f[0] ������ ���������� ��������
				solve_nonlinear_temp(f[0], f, t,
				rhie_chow,
				b, lb, s, ls, w, lw,
				dbeta, flow_interior,
				bmyconvective, NULL, 0.001, 0.001,
				false,
				matlist, 0,
				bprintmessage,
				gtdps, ltdp, 1.0, m,
				NULL, // �������� � ����������� ���������� ����.
				NULL); // �������� ����� ����� ������� � ����������� ���������� ����.
				// ��������� �������� ������ 1.0 �������� ��� �������� �������.
				*/
				// ���������� ����� ������.
				massa_cabinet(t, f, inx, iny, inz,
					xpos, ypos, zpos, flow_interior,
					b, lb, operatingtemperature,
					matlist);


				calculation_end_time = clock(); // ������ ��������� �����.
				calculation_seach_time = calculation_end_time - calculation_start_time;
				unsigned int im = 0, is = 0, ims = 0;
				im = (unsigned int)(calculation_seach_time / 60000); // ������
				is = (unsigned int)((calculation_seach_time - 60000 * im) / 1000); // �������
				ims = (unsigned int)((calculation_seach_time - 60000 * im - 1000 * is) / 10); // ������������ ������� �� 10

				printf("time calculation is:  %d minute %d second %d millisecond\n", im, is, 10 * ims);

				if (1) {
					if (!b_on_adaptive_local_refinement_mesh) {
						// ������� ���������� ���������� � ��������� tecplot360:
						exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint, 0, b, lb);
					}
					else {
						// ������� � ��������� ������� �����������.
						//� ���� �����.
						ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t,f, 0,b,lb);
					}
				}

			}

			doublereal tmaxfinish = -273.15; // ���������� ����.
											 // ���������� �������� ������������ ����������� ������ ��������� ������� � �� � ��������:
											 //for (integer i = 0; i < t.maxelm + t.maxbound; i++) tmaxfinish = fmax(tmaxfinish, fabs(t.potent[i]));
											 // 23 ������� 2015
											 // �� ��������� ������ ���������� ����� �� ����� ��������� ������� �����������, �������
											 // �������� �� ������� ����� � ��������� ����������� ������ �� ���������� ��. 
			for (integer i = 0; i < t.maxelm; i++) tmaxfinish = fmax(tmaxfinish, t.potent[i]);

			doublereal totaldeform_max = -1.0e+30;
			for (integer i = 0; i < t.maxelm; i++) totaldeform_max = fmax(totaldeform_max, t.total_deformation[TOTALDEFORMATION][i]);

			FILE *fp = NULL;
			errno_t err1 = 0;
#ifdef MINGW_COMPILLER
			fp = fopen64("report.txt", "w");
			if (fp == NULL) err1 = 1;
#else
			err1 = fopen_s(&fp, "report.txt", "w");
#endif
			// �������� ����� ��� ������.
			if ((err1) != 0) {
				printf("Create File report.txt Error\n");
				//system("PAUSE");
				system("pause");
			}
			else {
				// ������ ���������
				fprintf(fp, "Maximum Temperature %.2f\n", tmaxfinish);
				fclose(fp);
			}
			// 1 - solver/solid_static/
			report_temperature(flow_interior, f, t, b, lb, s, ls, w, lw, 0);

			

		}

		// steady Static Structural and Temperature (Thermal Stress).
		if (1 && steady_or_unsteady_global_determinant == 6) {

			// ����� �������.
			unsigned int calculation_start_time = 0; // ������ ����� ��.
			unsigned int calculation_end_time = 0; // ��������� ����� ��.
			unsigned int calculation_seach_time = 0; // ����� ���������� ������� ���� � ��.

			calculation_start_time = clock(); // ������ ������ �����.

			for (integer i7 = 0; i7<t.maxelm + t.maxbound; i7++) t.potent[i7] = operating_temperature_for_film_coeff; // �������������.

			// ������ �������������, � ����� Static Structural.
			bonly_solid_calculation = true;

			// �������� ����������� ���������� �� ����������� ������.
			if (lw == 1) {
				bPhysics_stop = true;
				if (lb < 11) {
					// ��� ����������� �������� :
					// MD40, AuSn, Cu, AuSn, SiC, GaN. cabinet and hollow.
					bPhysics_PTBSH_memory = true;
				}
			}

			if (adiabatic_vs_heat_transfer_coeff == 1) {
				// �� �������������� ��������� ��������� ����� ��������� �� ��������������� ��� ������ ������ ����.
				//for (integer i7 = 0; i7<t.maxelm + t.maxbound; i7++) t.potent[i7] = 0.57*operating_temperature_for_film_coeff;
			}

			// ����� �������������� ��� �� ������ ������������ ������ ������ ����������������.
			bsolid_static_only = true;
			bool bcleantemp = false;
			if (eqin.itemper == 1) {
				bcleantemp = true;
				integer i = 0; // ������� �����
				for (i = 0; i < flow_interior; i++) {
					if (eqin.fluidinfo[i].iflow == 1) bcleantemp = false;
				}
				// ���� bcleantemp==true �� �� ������ ������ ������ ������������� ��� ����� ���������.
			}
			if (1 || bcleantemp) {
				// ������� ������������ ���������� (��� ��������) ������ ������ ���������������� � ��������� �������. 
				printf("solution of pure Static Structural...\n");
				printf("please, press any key to continue...\n");
				if (bwait) {
					//system("PAUSE");
					system("pause");
				}

				// ��� ������������ ������������� ����������� ��������.
				bool bprintmessage = true; // �������� �� ��������� �� �������.

				doublereal dbeta = 1.0; // ������ ������� ������������� �� �������.
				bool bmyconvective = false;
				if (starting_speed_Vx*starting_speed_Vx + starting_speed_Vy*starting_speed_Vy + starting_speed_Vz*starting_speed_Vz > 1.0e-30) {
					if (f[0].maxelm > 0) {
						bmyconvective = true;
					}
				}
				else {
					// �������� ������������� ��������� ��������.
					errno_t err_inicialization_data=0;
					FILE* fp_inicialization_data=NULL;
#ifdef MINGW_COMPILLER
					fp_inicialization_data=fopen64("load.txt", "r");
					if (fp_inicialization_data==NULL) err_inicialization_data = 1;
#else
					err_inicialization_data = fopen_s(&fp_inicialization_data, "load.txt", "r");
#endif
					
					if (err_inicialization_data == 0) {
						// �������� ������ � ���� ������������.
						if (f[0].maxelm > 0) {
							bmyconvective = true;
						}
						fclose(fp_inicialization_data);
					}
				}

				// if (flow_interior>0) bmyconvective=true;
				// ������ ���������� ����������,
				// ��������� ��� �������� ������� ���-���
				doublereal **rhie_chow = NULL;
				QuickMemVorst m;
				m.ballocCRSt = false; // �������� ������
				m.bsignalfreeCRSt = true; // � ����� �����������.
				
										  // ������������� ����������.
				m.tval = NULL;
				m.tcol_ind = NULL;
				m.trow_ptr = NULL;
				m.tri = NULL;
				m.troc = NULL;
				m.ts = NULL;
				m.tt = NULL;
				m.tvi = NULL;
				m.tpi = NULL;
				m.tdx = NULL;
				m.tdax = NULL;
				m.ty = NULL;
				m.tz = NULL;
				m.ta = NULL;
				m.tja = NULL;
				m.tia = NULL;
				m.talu = NULL;
				m.tjlu = NULL;
				m.tju = NULL;
				m.tiw = NULL;
				m.tlevs = NULL;
				m.tw = NULL;
				m.tjw = NULL;
				m.icount_vel = 100000; // ����� ������� �����.

				


				
				// ���� flow_interior == 0 �� f[0] ������ ���������� ��������
				solve_nonlinear_temp(f[0], f, t,
				rhie_chow,
				b, lb, s, ls, w, lw,
				dbeta, flow_interior,
				bmyconvective, NULL, 0.001, 0.001,
				false,
				matlist, 0,
				bprintmessage,
				gtdps, ltdp, 1.0, m,
				NULL, // �������� � ����������� ���������� ����.
				NULL,
				lu, my_union); // �������� ����� ����� ������� � ����������� ���������� ����.
				// ��������� �������� ������ 1.0 �������� ��� �������� �������.
				
				//bPhysics_stop = false;
				// ����� ������� Static Structural.
				solve_Structural(t, w, lw, m, true, operatingtemperature,b,lb);
				//bPhysics_stop = true;

				// ���������� ����� ������.
				massa_cabinet(t, f, inx, iny, inz,
					xpos, ypos, zpos, flow_interior,
					b, lb, operatingtemperature,
					matlist);

				calculation_end_time = clock(); // ������ ��������� �����.
				calculation_seach_time = calculation_end_time - calculation_start_time;
				unsigned int im = 0, is = 0, ims = 0;
				im = (unsigned int)(calculation_seach_time / 60000); // ������
				is = (unsigned int)((calculation_seach_time - 60000 * im) / 1000); // �������
				ims = (unsigned int)((calculation_seach_time - 60000 * im - 1000 * is) / 10); // ������������ ������� �� 10

				printf("time calculation is:  %d minute %d second %d millisecond\n", im, is, 10 * ims);

				if (1) {
					if (!b_on_adaptive_local_refinement_mesh) {
						// ������� ���������� ���������� � ��������� tecplot360:
						exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint, 0, b, lb);
					}
					else {
						// ������� � ��������� ������� �����������.
						//� ���� �����.
						ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t,f,0,b,lb);
					}
				}

			}

			doublereal tmaxfinish = -273.15; // ���������� ����.
											 // ���������� �������� ������������ ����������� ������ ��������� ������� � �� � ��������:
											 //for (integer i = 0; i < t.maxelm + t.maxbound; i++) tmaxfinish = fmax(tmaxfinish, fabs(t.potent[i]));
											 // 23 ������� 2015
											 // �� ��������� ������ ���������� ����� �� ����� ��������� ������� �����������, �������
											 // �������� �� ������� ����� � ��������� ����������� ������ �� ���������� ��. 
			for (integer i = 0; i < t.maxelm; i++) tmaxfinish = fmax(tmaxfinish, t.potent[i]);

			doublereal totaldeform_max = -1.0e+30;
			for (integer i = 0; i < t.maxelm; i++) totaldeform_max = fmax(totaldeform_max, t.total_deformation[TOTALDEFORMATION][i]);

			FILE *fp=NULL;
			errno_t err1=0;
#ifdef MINGW_COMPILLER
			fp=fopen64("report.txt", "w");
			if (fp == NULL) err1 = 1;
#else
			err1 = fopen_s(&fp, "report.txt", "w");
#endif
			
			// �������� ����� ��� ������.
			if ((err1) != 0) {
				printf("Create File report.txt Error\n");
				//system("PAUSE");
				system("pause");
			}
			else {
				// ������ ���������
				fprintf(fp, "Maximum Temperature %.2f\n", tmaxfinish);
				fclose(fp);
			}
			// 1 - solver/solid_static/
			report_temperature(flow_interior, f, t, b, lb, s, ls, w, lw, 0);

			

		}



		/*
		if (b_on_adaptive_local_refinement_mesh) {
		printf("t.maxbound=%d\n", t.maxbound);
		printf("v dvuch shagah ot ALICE sborki. \n");
		system("PAUSE");
		exit(1);  // ����� ����������� ������������ ��������� ���������� ���������� �������� ������������ �����.
		}

		if (b_on_adaptive_local_refinement_mesh) {
		printf("Solve temperature is compleate. \n");
		system("PAUSE");
		exit(1);  // ����� ����������� ������������ ��������� ���������� ���������� �������� ������������ �����.
		}
		*/

		//system("pause");

		if (1 && ((steady_or_unsteady_global_determinant == 1)||(steady_or_unsteady_global_determinant == 7))) {
			// �������������� ����������������.
			
			// ������ ������ ������������� � ������ ����.
			bonly_solid_calculation = true;

			// �������� ����������� ���������� �� ����������� ������.
			// ��� ���������� �������� ������ �� ���� �������� �������� ����������.
			if (lw == 1) {
				bPhysics_stop = true;
				if (lb < 11) {
					// ��� ����������� �������� :
					// MD40, AuSn, Cu, AuSn, SiC, GaN. cabinet and hollow.
					bPhysics_PTBSH_memory = true;
				}
			}

			bglobal_unsteady_temperature_determinant = true;
			// ����� �������������� ��� �� ������ ������ ������ ����������������.
			// ��������� 13 �������� bconvective
			// �� ����������� �������� ��������, �������� ������� ����������� ����������,
			// ����������� �� ����������� ��������� �����. ������� ������� ������������� �� 
			// ������� ������� ����������� ������ �����.
			// ��� ������������� ����� � ������������� ��������� 
			// ���������� ��������� ������� ������������� � ������� �� ������
			// ��� ��������� ����������� � ������ ������� �� 5 �������� �� ���� ��������� � 167�������� � �������.
			// ��� ��������� �������� � icepak ����� 120 ��������, ��� ������ � ��������� ����������� Rt 
			// (�������� 6.875��, RT=16K/W) � �����������������
			// ������.
			doublereal dbeta = 1.3333333;//1.0; // ���� 1.0 �� ������ ������� ������������� �� �������.
			dbeta = 1.0; // ����� ���������� ��������.
			// ������ ���������� ����������,
			// ��������� ��� �������� ������� ���-���
			doublereal **rhie_chow = NULL;
			//solve_nonlinear_temp(f[0], f, t, rhie_chow, b, lb, s, ls, w, lw, dbeta, flow_interior, false, NULL, 0.001, false);
			bool bsecond_T_solver = false;
			if (steady_or_unsteady_global_determinant == 7) {
				// ������������� ������ �� ������ ���������� ������ ������� 10.11.2018.
				bsecond_T_solver = true;
			}
			unsteady_temperature_calculation(f[0], f, t,
				rhie_chow,
				b, lb, s, ls, w, lw,
				dbeta, flow_interior,
			    matlist,
				operatingtemperature,
				gtdps, ltdp, lu, my_union, bsecond_T_solver); // �������������� ������������� ������

			// ���������� ����� ������.
			massa_cabinet(t, f, inx, iny, inz,
				xpos, ypos, zpos, flow_interior,
				b, lb, operatingtemperature,
				matlist);

			if (!bsecond_T_solver) {
				if (!b_on_adaptive_local_refinement_mesh) {
					// ������� ���������� ���������� � ��������� tecplot360:
					exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint, 0, b, lb);
				}
				else {
					// ������� � ��������� ������� �����������.
					//� ���� �����.
					ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, f, 0, b, lb);
				}


				doublereal tmaxfinish = -273.15;
				// ���������� �������� ������������ ����������� ������ ��������� ������� � �� � ��������:
				for (integer i = 0; i < t.maxelm + t.maxbound; i++) tmaxfinish = fmax(tmaxfinish, t.potent[i]);
				FILE *fp=NULL;
				errno_t err1=0;
#ifdef MINGW_COMPILLER
				fp=fopen64("report.txt", "w");
				if (fp == NULL) err1 = 1;
#else
				err1 = fopen_s(&fp, "report.txt", "w");
#endif
				// �������� ����� ��� ������.
				if ((err1) != 0) {
					printf("Create File report.txt Error\n");
					//system("PAUSE");
					system("pause");
				}
				else {
					// ������ ���������
					fprintf(fp, "Maximum Temperature %.2f\n", tmaxfinish);
					fclose(fp);
				}
				// 1 - solver/solid_static/
				report_temperature(flow_interior, f, t, b, lb, s, ls, w, lw, 0);
			}
			else {
				printf("THIS IS SECOND UNSTEADY TEMPERATURE SOLVER ON ALL MESHES.\n");
				printf("NO EXPOPRT TECPLOT.\n");
				printf("NO PRINT REPORT.\n");
			}
			printf("calculation complete...\n");
			// system("PAUSE");
		}

		fclose(fp_radiation_log);

		// ������� ���������� ���������� � ��������� tecplot360:
		// ����� ������������ ��� �������� ����������� �����.
		if (false) {
			exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint,0, b, lb);
			printf("read values. OK.\n");
			if (bwait) {
				//system("PAUSE"); // debug avtosave
				system("pause");
			}
		}

		if ((1 && steady_or_unsteady_global_determinant == 3)) {
			// Fluid dynamic.
#ifdef _OPENMP
			// �������������� � ������������� ������� cfd �� openMP 08.05.2019.
			printf("CFD not work in OPENMP ON and bparallelismold is true.\n");
			printf("uskorenie ot OPENMP otsutstvuet. Rabotaen odnopotochnaq versiq.\n");
			printf("variable bparallelismold must be equal false.\n");
			//system("PAUSE");
#endif

			told_temperature_global_for_HOrelax = new doublereal[t.maxelm + t.maxbound];
			bSIMPLErun_now_for_temperature = true;
			
			if (dgx*dgx + dgy*dgy + dgz*dgz > 1.0e-20) {
				// ���� ����� ��������� �������� �� ��� fluid ���������� ����������� ����������.
				bool bbussinesk_7 = false;
#pragma omp parallel for
				for (integer i_8 = 0; i_8 < f[0].maxelm; i_8++) {
					integer ib = t.whot_is_block[f[0].ptr[i_8]];
					if (ib > -1) {
						if (b[ib].itype == FLUID) {
							integer i_7 = b[ib].imatid;
							if (matlist[i_7].bBussineskApproach) bbussinesk_7 = true;
						}
					}
				}
				if (bbussinesk_7) {					
					bSIMPLErun_now_for_natural_convection = true;
				}
			}
			bHORF = true;
			bPamendment_source_old = new doublereal[f[0].maxelm + f[0].maxbound];
			for (integer i5 = 0; i5 < f[0].maxelm + f[0].maxbound; i5++) bPamendment_source_old[i5] = 0.0;
			// exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint);
			//system("PAUSE");
			if (dgx*dgx + dgy*dgy + dgz*dgz > 1.0e-20) {
				// ���� ����� ��������� �������� �� ��� fluid ���������� ����������� ����������.
				bool bbussinesk_7 = false;
#pragma omp parallel for
				for (integer i_8 = 0; i_8 < f[0].maxelm; i_8++) {
					integer ib = t.whot_is_block[f[0].ptr[i_8]];
					if (ib > -1) {
						if (b[ib].itype == FLUID) {
							integer i_7 = b[ib].imatid;
							if (matlist[i_7].bBussineskApproach) {
								bbussinesk_7 = true;
							}
						}
					}
				}
				if (bbussinesk_7) {
					printf("Bussinesk approach Operating Temperature=%e\n", f[0].OpTemp); // Operating Temperature);
				}
			}

			//exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint, 0);
			//system("PAUSE");

			// ������������ ����������������� ��������.
			steady_cfd_calculation(breadOk,
				eqin, dgx, dgy, dgz,
				continity_start,
				inumber_iteration_SIMPLE,
				flow_interior, f, t, b, lb,
				s, ls, w, lw, matlist,
				gtdps, ltdp, bextendedprint, lu, my_union);
			//xyplot( f, 0, t);
			// boundarylayer_info(f, t, flow_interior, w, lw);
			// 2 - solver/conjugate_heat_transfer_static/
			report_temperature(flow_interior, f, t, b, lb, s, ls, w, lw, 0/*2*/);

			// ���������� ����� ������.
			massa_cabinet(t, f, inx, iny, inz,
				xpos, ypos, zpos, flow_interior,
				b, lb, operatingtemperature,
				matlist);

			// ������� ���������� ���������� � ��������� tecplot360:
			if (!b_on_adaptive_local_refinement_mesh) {
				exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint, 0, b, lb);
			}
			else {
				ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, f, 0, b, lb);
			}

			save_velocity_for_init(t.maxelm, t.ncell, f, t, flow_interior);
			// exporttecplotxy360T_3D_part2_rev(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint,b,lb);
			delete[] bPamendment_source_old;
			bPamendment_source_old = NULL;
			delete[] told_temperature_global_for_HOrelax;
			told_temperature_global_for_HOrelax = NULL;
		}
		if ((1 && (steady_or_unsteady_global_determinant == 9))) {
			

#ifdef _OPENMP
			// �������������� � ������������� ������� cfd �� openMP 08.05.2019.
			printf("Unsteady CFD not work in OPENMP ON and bparallelismold is true.\n");
			printf("uskorenie ot OPENMP otsutstvuet. Rabotaen odnopotochnaq versiq.\n");
			printf("variable bparallelismold must be equal false.\n");
			//system("PAUSE");
#endif

			told_temperature_global_for_HOrelax = new doublereal[t.maxelm + t.maxbound];
			bSIMPLErun_now_for_temperature = true;


			if (dgx*dgx + dgy*dgy + dgz*dgz > 1.0e-20) {
				// ���� ����� ��������� �������� �� ��� fluid ���������� ����������� ����������.
				bool bbussinesk_7 = false;
#pragma omp parallel for
				for (integer i_8 = 0; i_8 < f[0].maxelm; i_8++) {
					integer ib = t.whot_is_block[f[0].ptr[i_8]];
					if (ib > -1) {
						if (b[ib].itype == FLUID) {
							integer i_7 = b[ib].imatid;
							if (matlist[i_7].bBussineskApproach) bbussinesk_7 = true;
						}
					}
				}
				if (bbussinesk_7) {
					bSIMPLErun_now_for_natural_convection = true;
				}
			}

			bHORF = true;
			bPamendment_source_old = new doublereal[f[0].maxelm + f[0].maxbound];
			for (integer i5 = 0; i5 < f[0].maxelm + f[0].maxbound; i5++) bPamendment_source_old[i5] = 0.0;

			// exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint);
			//system("PAUSE");
			if (dgx*dgx + dgy*dgy + dgz*dgz > 1.0e-20) {
				// ���� ����� ��������� �������� �� ��� fluid ���������� ����������� ����������.
				bool bbussinesk_7 = false;
#pragma omp parallel for
				for (integer i_8 = 0; i_8 < f[0].maxelm; i_8++) {
					integer ib = t.whot_is_block[f[0].ptr[i_8]];
					if (ib > -1) {
						if (b[ib].itype == FLUID) {
							integer i_7 = b[ib].imatid;
							if (matlist[i_7].bBussineskApproach) {
								bbussinesk_7 = true;
							}
						}
					}
				}
				if (bbussinesk_7) {
					printf("Bussinesk approach Operating Temperature=%e\n", f[0].OpTemp); // Operating Temperature);
				}
			}

			//exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint, 0);
			//system("PAUSE");

			if (1) {
				printf("Sorry unsteady cfd calcuation dont support... 21.07.2019\n");
				printf("Your may send your message to kirill7785@mail.ru.\n");
				system("pause");
			}
			else {
				// �������������� ����������������� �������� :
				usteady_cfd_calculation(breadOk, eqin,
					dgx, dgy, dgz,
					continity_start,
					inumber_iteration_SIMPLE,
					flow_interior,
					f, t,
					b, lb, s, ls,
					w, lw, matlist, gtdps, ltdp, bextendedprint, lu, my_union);
			}

			//xyplot( f, 0, t);
			// boundarylayer_info(f, t, flow_interior, w, lw);
			// 2 - solver/conjugate_heat_transfer_static/
			report_temperature(flow_interior, f, t, b, lb, s, ls, w, lw, 0/*2*/);

			// ���������� ����� ������.
			massa_cabinet(t, f, inx, iny, inz,
				xpos, ypos, zpos, flow_interior,
				b, lb, operatingtemperature,
				matlist);

			// ������� ���������� ���������� � ��������� tecplot360:
			if (!b_on_adaptive_local_refinement_mesh) {
				exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint, 0, b, lb);
			}
			else {
				ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, f, 0, b, lb);
			}

			save_velocity_for_init(t.maxelm, t.ncell, f, t, flow_interior);
			// exporttecplotxy360T_3D_part2_rev(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint,b,lb);

			delete[] bPamendment_source_old;
			bPamendment_source_old = NULL;
			delete[] told_temperature_global_for_HOrelax;
			told_temperature_global_for_HOrelax = NULL;

		}

		fclose(fp_log); // �������� ����� ����.


		if (continity_start != NULL) {
			delete[] continity_start;
			continity_start = NULL;
		}

		if (inumber_iteration_SIMPLE != NULL) {
			delete[] inumber_iteration_SIMPLE;
			inumber_iteration_SIMPLE = NULL;
		}

	}

	// ������������ ����������� ������.
	if (t.xpos_copy != NULL) {
		delete[] t.xpos_copy;
		t.xpos_copy = NULL;
	}
	if (t.ypos_copy != NULL) {
		delete[] t.ypos_copy;
		t.ypos_copy = NULL;
	}
	if (t.zpos_copy != NULL) {
		delete[] t.zpos_copy;
		t.zpos_copy = NULL;
	}
	
	// ������������ ����������� ������.
	if (xposadd != NULL) {
		delete[] xposadd;
		xposadd = NULL;
	}
	if (yposadd != NULL) {
		delete[] yposadd;
		yposadd = NULL;
	}
	if (zposadd != NULL) {
		delete[] zposadd;
		zposadd = NULL;
	}

	
	// ������������ ����������� ������.
	if (xpos != NULL) {
		delete[] xpos;
		xpos = NULL;
	}
	if (ypos != NULL) {
		delete[] ypos;
		ypos = NULL;
	}
	if (zpos != NULL) {
		delete[] zpos;
		zpos = NULL;
	}

	if (bsource_term_radiation_for_relax != NULL) {
		delete[] bsource_term_radiation_for_relax; // ���������� ������������ ������ ������������ �������.
		bsource_term_radiation_for_relax = NULL;
	}
	if (b_buffer_correct_source != NULL) {
		delete[] b_buffer_correct_source;
		b_buffer_correct_source = NULL;
	}

	printf("free memory begin...\n");
	if (bwait) {
		//system("PAUSE");
		system("pause");
	}

	if (rthdsd_no_radiosity_patch != NULL) {
		delete[] rthdsd_no_radiosity_patch;
		rthdsd_no_radiosity_patch = NULL;
	}
	

	// ������� ��������� ���������� ��������� ������� � ����� 0.14 ��������.
	if (qnbc != NULL) {
		delete[] qnbc;
		qnbc = NULL;
		iadd_qnbc_maxelm = 0;
	}

	// ������������ ������ �� ��� amg1r5.
	if (amgGM.a != NULL) {
		delete[] amgGM.a;
		amgGM.a = NULL;
	}
	if (amgGM.ia != NULL) {
		delete[] amgGM.ia;
		amgGM.ia = NULL;
	}
	if (amgGM.ja != NULL) {
		delete[] amgGM.ja;
		amgGM.ja = NULL;
	}
	if (amgGM.u != NULL) {
		delete[] amgGM.u;
		amgGM.u = NULL;
	}
	if (amgGM.f != NULL) {
		delete[] amgGM.f;
		amgGM.f = NULL;
	}
	if (amgGM.ig != NULL) {
		delete[] amgGM.ig;
		amgGM.ig = NULL;
	}

	amgGM.nda = -1;
	amgGM.ndf = -1;
	amgGM.ndia = -1;
	amgGM.ndig = -1;
	amgGM.ndja = -1;
	amgGM.ndu = -1;

	for (integer i_7 = 0; i_7 < lb; i_7++) {
		if (b[i_7].temp_Sc != NULL) {
			delete[] b[i_7].temp_Sc;
			b[i_7].temp_Sc = NULL;
		}
		if (b[i_7].arr_Sc != NULL) {
			delete[] b[i_7].arr_Sc;
			b[i_7].arr_Sc = NULL;
		}
		if (b[i_7].g.hi != NULL) {
			delete[] b[i_7].g.hi;
			b[i_7].g.hi = NULL;
		}
		if (b[i_7].g.xi != NULL) {
			delete[] b[i_7].g.xi;
			b[i_7].g.xi = NULL;
		}
		if (b[i_7].g.yi != NULL) {
			delete[] b[i_7].g.yi;
			b[i_7].g.yi = NULL;
		}
		if (b[i_7].g.zi != NULL) {
			delete[] b[i_7].g.zi;
			b[i_7].g.zi = NULL;
		}
	}
	delete[] b; delete[] s; delete[] w; // ������������ ������
	b = NULL;
	s = NULL;
	w = NULL;
	for (integer i_7 = 0; i_7 < lmatmax; i_7++) {
		if (matlist[i_7].arr_cp != NULL) {
			delete[] matlist[i_7].arr_cp;
			matlist[i_7].arr_cp = NULL;
     	}
		if (matlist[i_7].temp_cp != NULL) {
			delete[] matlist[i_7].temp_cp;
			matlist[i_7].temp_cp = NULL;
		}
		if (matlist[i_7].arr_lam != NULL) {
			delete[] matlist[i_7].arr_lam;
			matlist[i_7].arr_lam = NULL;
		}
		if (matlist[i_7].temp_lam != NULL) {
			delete[] matlist[i_7].temp_lam;
			matlist[i_7].temp_lam = NULL;
		}
	}
	delete[] matlist;
	matlist = NULL;
	delete[] gtdps;
	gtdps = NULL;
	if (eqin.fluidinfo != NULL) {
		delete[] eqin.fluidinfo;
		eqin.fluidinfo = NULL;
	}
	// ����� ���������� ����������� ������ �� ��� ���� �������� ������:
	free_level1_temp(t);
	free_level2_temp(t); // ������������ ������ �� ��� ������.
	// ����������� ������ ��� LR ������.
	free_root(t.rootWE, t.iWE);
	free_root(t.rootSN, t.iSN);
	free_root(t.rootBT, t.iBT);
	if (t.rootWE != NULL) {
		delete[] t.rootWE;
		t.rootWE = NULL;
	}
	if (t.rootSN != NULL) {
		delete[] t.rootSN;
		t.rootSN = NULL;
	}
	if (t.rootBT != NULL) {
		delete[] t.rootBT;
		t.rootBT = NULL;
	}
	// ������������ ������ ��� LR �����.
	free_level1_flow(f, flow_interior);
	free_level2_flow(f, flow_interior); // ������������ ������ �� ��� ������.

	delete[] f;
	f = NULL;

	if (sourse2Dproblem != NULL) {
		delete[] sourse2Dproblem;
		sourse2Dproblem = NULL;
	}
	if (conductivity2Dinsource != NULL) {
		delete[] conductivity2Dinsource;
		conductivity2Dinsource = NULL;
	}

	if (x_jacoby_buffer != NULL) {
		// 30 ������� 2016. 
		// � seidelsor2 ������ ������������� �� ����� ������ ���������� �.�. �����.
		// ������������ ������ �� ��� jacobi buffer.
		delete[] x_jacoby_buffer;
		x_jacoby_buffer = NULL;
	}

	if (bvery_big_memory) {
		if (t.database.x != NULL) {
			free(t.database.x);
			t.database.x = NULL;
		}
		if (t.database.y != NULL) {
			free(t.database.y);
			t.database.y = NULL;
		}
		if (t.database.z != NULL) {
			free(t.database.z);
			t.database.z = NULL;
		}
		if (t.database.nvtxcell != NULL) {
			for (integer i = 0; i <= 7; i++) {
				delete[] t.database.nvtxcell[i];
			}
			delete[] t.database.nvtxcell;
			t.database.nvtxcell = NULL;
		}
		if (t.database.ptr != NULL) {
			if (t.database.ptr[0] != NULL) {
				delete[] t.database.ptr[0];
			}
			if (t.database.ptr[1] != NULL) {
				delete[] t.database.ptr[1];
			}
			delete[] t.database.ptr;
			t.database.ptr = NULL;
		}
	}

	for (integer i63 = 0; i63 < lu; i63++) {
		// ����� ���������� ����������� ������ �� ��� ���� �������� ������:
		free_level1_temp(my_union[i63].t);
		free_level2_temp(my_union[i63].t); // ������������ ������ �� ��� ������.

		// ����������� ������ ��� LR ������.
		free_root(my_union[i63].t.rootWE, my_union[i63].t.iWE);
		free_root(my_union[i63].t.rootSN, my_union[i63].t.iSN);
		free_root(my_union[i63].t.rootBT, my_union[i63].t.iBT);
		if (my_union[i63].t.rootWE != NULL) {
			delete[] my_union[i63].t.rootWE;
			my_union[i63].t.rootWE = NULL;
		}
		if (my_union[i63].t.rootSN != NULL) {
			delete[] my_union[i63].t.rootSN;
			my_union[i63].t.rootSN = NULL;
		}
		if (my_union[i63].t.rootBT != NULL) {
			delete[] my_union[i63].t.rootBT;
			my_union[i63].t.rootBT = NULL;
		}

		// ������������ ������ ��� LR �����.
		free_level1_flow(my_union[i63].f, my_union[i63].flow_interior);
		free_level2_flow(my_union[i63].f, my_union[i63].flow_interior); // ������������ ������ �� ��� ������.

		delete[] my_union[i63].f;
		my_union[i63].f = NULL;

		if (bvery_big_memory) {
			if (my_union[i63].t.database.x != NULL) {
				free(my_union[i63].t.database.x);
				my_union[i63].t.database.x = NULL;
			}
			if (my_union[i63].t.database.y != NULL) {
				free(my_union[i63].t.database.y);
				my_union[i63].t.database.y = NULL;
			}
			if (my_union[i63].t.database.z != NULL) {
				free(my_union[i63].t.database.z);
				my_union[i63].t.database.z = NULL;
			}
			if (my_union[i63].t.database.nvtxcell != NULL) {
				for (integer i = 0; i <= 7; i++) {
					delete[] my_union[i63].t.database.nvtxcell[i];
				}
				delete[] my_union[i63].t.database.nvtxcell;
				my_union[i63].t.database.nvtxcell = NULL;
			}
			if (my_union[i63].t.database.ptr != NULL) {
				if (my_union[i63].t.database.ptr[0] != NULL) {
					delete[] my_union[i63].t.database.ptr[0];
				}
				if (my_union[i63].t.database.ptr[1] != NULL) {
					delete[] my_union[i63].t.database.ptr[1];
				}
				delete[] my_union[i63].t.database.ptr;
				my_union[i63].t.database.ptr = NULL;
			}
		}
	}

	// ������������ ����� ������ � ILU �������.
	if (milu_gl_buffer.alu_copy != NULL) delete[] milu_gl_buffer.alu_copy;
	if (milu_gl_buffer.jlu_copy != NULL) delete[] milu_gl_buffer.jlu_copy;
	if (milu_gl_buffer.ju_copy != NULL) delete[] milu_gl_buffer.ju_copy;
	milu_gl_buffer.alu_copy = NULL;
	milu_gl_buffer.jlu_copy = NULL;
	milu_gl_buffer.ju_copy = NULL;

	free_QSBid(); // ��� ���������� ������ ������� myisblock_id.

	flow_interior = 0;
	printf("free memory finish...\n");

	if (1 && steady_or_unsteady_global_determinant == 2) {
		// ��� ������ ����� ��������� ����������.
		printf("Mesh generation procedure is finish.\n");
	}
	else {
		printf("Calculation procedure is finish.\n");
	}
	
	if (bwait) {
		printf("Please, press any key to exit...\n");
		//system("PAUSE");
		system("pause");
	}


	// ��� ��� � ������ bFULL_AUTOMATIC ������� ������������ �������� �
	// ������� ������������ �������, �� �������� ������� ����������� ���� ���� ���, �
	// ��� ��������� ��������� ���� ��������� � ������ ��� �������.
	// 20mm ���� ��������� � 1��� 9� �� 53� �� ���� ������ bFULL_AUTOMATIC.
	// ��� ������� ��� automatic
	// ������������ ����������� ������ �� ��� ��� �������.
	delete[] shorter_hash_X;
	delete[] shorter_hash_Y;
	delete[] shorter_hash_Z;
	delete[] bshorter_hash_X;
	delete[] bshorter_hash_Y;
	delete[] bshorter_hash_Z;

	// ����������� ������ �� ��� ������� �����.
	delete[] StringList;

	calculation_main_end_time = clock();
	calculation_main_seach_time = calculation_main_end_time - calculation_main_start_time;


	/*printf("time=%d statistic vorst=%3.2f %% \n",calculation_main_seach_time,(float)(100.0*calculation_vorst_seach_time/calculation_main_seach_time));
	system("PAUSE");
	*/

	
	
	// ����� ����� ����������.
	int im=0, is=0, ims=0;
	im=(int)(calculation_main_seach_time/60000); // ������
	is=(int)((calculation_main_seach_time-60000*im)/1000); // �������
	ims=(int)((calculation_main_seach_time-60000*im-1000*is)/10); // ������������ ������� �� 10
	
	printf("time calculation is:  %d minute %d second %d millisecond\n", im, is, 10*ims);

	if (1 && (steady_or_unsteady_global_determinant != 8)) {
		system("pause");
	}
	return 0;
}