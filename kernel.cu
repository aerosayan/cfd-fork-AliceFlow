// AliceFlow_v0_24.cpp 
// 9 ���� 2017 ������� �� 64 ������ ����� int64_t.
// 15 ������ 2017 ��������������� � visual studio community edition 2017 (open source).
// 1 ������� 2016 �������������� �� nvidia cuda 8.0. 
// ���� ����� ������� � ����� ��� ������������� � ������ ����. 
// 11 ������ 2016 ���� ������� cl_agl_amg_v0_14.
// 15 ������� 2015 ����. ������ � Visual Studio 2015.
// AliceFlow_v0_21.cpp
// 15 ������� 2015. ������ � Visual Studio 2013.
// AliceFlow_v0_20.cpp
// 14 ������� 2015 ������������� ���������� ����������������� lusol � ilu2 decomposition �� 2 ������.
// AliceFlow_v0_07.cpp: ���������� ����� ����� ��� ����������� ����������.
// AliceFlow_v0_07.cpp �� ������  AliceFlow_v0_06.cpp, �� ������ � LES ������� ��������������.
// ��������� �� ������ ������������ � �������� ������.
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
// ��� ������ �������� ��� ����� ������� ������� ������� 2-3 ����� ���������, � ����� 3-5 ���� ���������. 

// ���������������� � ������ ���� ������ ���������� �������������� ������������ gcc �� GNU.
//#define MINGW_COMPILLER 1

#ifdef MINGW_COMPILLER
#include <stdio.h>
#endif

// ��� std::locale::global(std::locale("en_US.UTF-8"));
// �� ��������.
//#include <locale.h>

//#include "stdafx.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <cinttypes> // ��� ���� 64 ������� ������ ����� int64_t


#include <stdlib.h> 
#include <omp.h> // OpenMP
//-Xcompiler "/openmp" 

//using namespace System;

// ������������ ����������.
#define doubleprecision 1
#if doubleprecision == 1
#define doublereal double //double // ������ ������������� ����� ������� ��������
//#define doublereal Decimal // decimal
#else
#define doublereal float //float // ������ ������������� ����� ��������� ��������
#endif

#define doubleintprecision 1



#if doubleintprecision == 1
#define integer int64_t
#else
#define integer int
#endif

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

// ���������� �������������� ������������� ������� �� ����������.
typedef struct TMY_AMG_MANAGER {

	// 0 - �� �������� ������� �������, 
	// 1 - �������� ������� �������.
	integer bTemperatureMatrixPortrait;
	integer bSpeedMatrixPortrait;
	integer bPressureMatrixPortrait;
	integer bStressMatrixPortrait;
	integer bMatrixPortrait;

	// fgmres(m_restart)
	integer m_restart;

	// lfil for BiCGStab+ILU2 and fgmres.
	integer lfil;

	// Temperature
	doublereal theta_Temperature;
	integer maximum_delete_levels_Temperature;
	integer nFinnest_Temperature, nu1_Temperature, nu2_Temperature;
	integer memory_size_Temperature;
	integer ilu2_smoother_Temperature; // 0 - �� ������������, 1 - ������������.
	// 0 - AVL Tree, 1 - SPLAY Tree, 2 - Binary Heap, 3 - Treap.
	// default - 3.
	integer iCFalgorithm_and_data_structure_Temperature;
	// Speed
	doublereal theta_Speed;
	integer maximum_delete_levels_Speed;
	integer nFinnest_Speed, nu1_Speed, nu2_Speed;
	integer memory_size_Speed;
	integer ilu2_smoother_Speed; // 0 - �� ������������, 1 - ������������.
	// 0 - AVL Tree, 1 - SPLAY Tree, 2 - Binary Heap, 3 - Treap.
	// default - 3.
	integer iCFalgorithm_and_data_structure_Speed;
	// Pressure
	doublereal theta_Pressure;
	integer maximum_delete_levels_Pressure;
	integer nFinnest_Pressure, nu1_Pressure, nu2_Pressure;
	integer memory_size_Pressure;
	integer ilu2_smoother_Pressure; // 0 - �� ������������, 1 - ������������.
	// 0 - AVL Tree, 1 - SPLAY Tree, 2 - Binary Heap, 3 - Treap.
	// default - 3.
	integer iCFalgorithm_and_data_structure_Pressure;
	// Stress
	doublereal theta_Stress;
	integer maximum_delete_levels_Stress;
	integer nFinnest_Stress, nu1_Stress, nu2_Stress;
	integer memory_size_Stress;
	integer ilu2_smoother_Stress; // 0 - �� ������������, 1 - ������������.
	// 0 - AVL Tree, 1 - SPLAY Tree, 2 - Binary Heap, 3 - Treap.
	// default - 3.
	integer iCFalgorithm_and_data_structure_Stress;
	// global
	bool bCFJacoby;
	integer iRunge_Kutta_smoother; // 3 - �������� �������, 5 - ������ �������, ����� ������ ����� �� ������������. 
	integer iFinnest_ilu; // 0 �� ������������, 1 - ilu0. ������ �� ����� ��������� �����.
	// ������������� iluk ���������� �� �������� ������� ����������� ��� �������
	// �������� ������ nnz/n ����� ������ ������ ���� ������ 6 (�����).
	bool b_ilu_smoothers_in_nnz_n_LE_6;
	doublereal theta; // strength threshold
	//integer maximum_levels; // ������������ ���������� ������� ����������� (������ ���� ������������).
	integer maximum_delete_levels; // ���������� ������� ���������� ����� � ������� ������ �����.
	integer nFinnest, nu1, nu2; // ���������� �����������.
	// 0 - AVL Tree, 1 - SPLAY Tree, 2 - Binary Heap, 3 - Treap.
	// default - 3.
	integer iCFalgorithm_and_data_structure;
	integer memory_size; // � �������� ������� �.
	// ��� ������ ������� ���������� � ������������.
	integer number_interpolation_procedure; // ������������� ��������� �������������.
	integer number_interpolation_procedure_Temperature;
	integer number_interpolation_procedure_Speed;
	integer number_interpolation_procedure_Pressure;
	integer number_interpolation_procedure_Stress;

	// 6 december 2016.
	integer itypemodifyinterpol; // ����� ����������� �������������.
	integer inumberadaptpass; // ������������ ���������� ������-�������� � �������������.
	doublereal gold_const, gold_const_Temperature, gold_const_Speed, gold_const_Pressure, gold_const_Stress;
	doublereal magic;
	doublereal F_to_F_Temperature, F_to_F_Speed, F_to_F_Pressure, F_to_F_Stress; // magic
	integer ilu2_smoother; // 0 - �� ������������, 1 - ������������.
	// AMG Splitting (coarsening)
	// ������ ���������� C-F ��������� : 0 - standart, 1 - RS 2.
	// RS 2 ���������� ������ ���������� C-F ��������� ���������� ������ ������.
	integer icoarseningTemp, icoarseningSpeed, icoarseningPressure, icoarseningStress;
	integer icoarseningtype;
	// Stabilization BiCGStab.
	// 8.01.2017
	integer istabilizationTemp, istabilizationSpeed, istabilizationPressure, istabilizationStress;
	integer istabilization;
	// ipatch - ����� �����.
	integer ipatch_number;

	integer iprint_log, iprint_log_Temperature, iprint_log_Speed, iprint_log_Pressure, iprint_log_Stress;

	// truncation for interpolation.
	integer itruncation_interpolation, itruncation_interpolation_Temperature, itruncation_interpolation_Speed, itruncation_interpolation_Pressure, itruncation_interpolation_Stress;
	double truncation_interpolation, truncation_interpolation_Temperature, truncation_interpolation_Speed, truncation_interpolation_Pressure, truncation_interpolation_Stress;

	// gmres smoother
	// �.����, ����� [1986].
	bool b_gmresTemp, b_gmresSpeed, b_gmresPressure, b_gmresStress;
	bool b_gmres;

} MY_AMG_MANAGER;

MY_AMG_MANAGER my_amg_manager;

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
	integer id_law; // 0 - Linear, 1 - Square Wave.
	doublereal Factor_a_for_Linear;
	doublereal tau; // ������������ �������� ��� Square Wave
	// 06_03_2017 ���������� ����� ���� � �������.
	doublereal Q; // ���������� ��� Square Wave.
	// ���������� ����� ��� ���� �������.
	doublereal m1, tau1, tau2, tau_pause, T_all;
	integer n_cycle;
	// hot cold reshime (double linear)
	doublereal on_time_double_linear;

} TimeStepLaw;

TimeStepLaw glTSL;

// 24 ������� 2016. 
// ��� ��������� ����� ���������� ����� � ����� 0.14 ��������.
typedef struct TQuickNonlinearBoundaryCondition {
	doublereal emissivity;
	doublereal Tamb, dS;
	doublereal film_coefficient;
	bool bactive;
	bool bStefanBolcman_q_on;
	bool bNewtonRichman_q_on;

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

bool bwait = false; // ���� false �� �� ���������� getchar().
#define admission 1.0e-10 // ��� ����������� ���������� ���� ������������ �����.

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
// BETA 1.0 4/3=1.333333333 6/5=1.2
#define BETA 1.0

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
	integer ileft_start, ileft_finish, iright_start, iright_finish, iseparate_start, iseparate_finish;
	bool active; // ���������� ������������.
} PARBOUND;


// ��������� ������ ������������ ��� �����������������.
typedef struct TPARDATA {
	integer ncore; // 1, 2, 4, 8.
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



#include "adaptive_local_refinement_mesh.cpp" // ����
#include "constr_struct.cpp" // ���������� �������� ������ TEMPER � FLOW

// ���������� ������, ���������� � ������.
integer lb, ls, lw;
BLOCK* b = NULL; // ������
SOURCE* s = NULL; // ����������
WALL* w = NULL; // ������ ������

doublereal *xpos = NULL, *ypos = NULL, *zpos = NULL;
doublereal *xposadd = NULL, *yposadd = NULL, *zposadd = NULL;

#include "my_LR.c" // ������������ �����

#include "my_material_properties.c" // ���������� �������� ������� ����������

// ������������� ����������� ��������� ���������-��������
// �� ����������� �����
#include "pamendment3.c"


#include "shortest_distance.cpp" // ���������� ����������� ���������� �� ������

// 8 ������ 2016.
const bool bvery_big_memory = true; // true ��� ������ � ���� �� ������ � ����������� ������. ��� ����������� ������� �� ��������.

struct Tdatabase {
	doublereal *x=NULL, *y=NULL, *z=NULL; // ���������� �����.
	integer maxelm;
	integer** nvtxcell=NULL;
	integer ncell;
	// ����� ������������� � ��������������.
	integer **ptr=NULL;// ��� ������������ ��������������� �������������� ������
};

Tdatabase database;

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





doublereal rterminate_residual_ICCG_Oh2(FLOW floc) {
	// �������� ������������� O(h!2)
	doublereal* resterm = new doublereal[floc.maxelm + floc.maxbound];
	for (integer i = 0; i<floc.maxelm + floc.maxbound; i++) {
		resterm[i] = 0.0; // �������������.
	}

	for (integer iP = 0; iP<floc.maxelm; iP++) {
		// ���������� �������� �������� ������������ ������:
		doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
		volume3D(iP, floc.nvtx, floc.pa, dx, dy, dz);
		doublereal dl = fmin(dx, fmin(dy, dz));
		resterm[iP] = 0.1*dl*dl; // O(h!2)
		integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
		iE = floc.sosedi[ESIDE][iP].iNODE1; iN = floc.sosedi[NSIDE][iP].iNODE1; iT = floc.sosedi[TSIDE][iP].iNODE1; iW = floc.sosedi[WSIDE][iP].iNODE1; iS = floc.sosedi[SSIDE][iP].iNODE1; iB = floc.sosedi[BSIDE][iP].iNODE1;
		// ���� � ����� �� ������ ����� ������� ��������� �������
		// �� ��������������� ���������� ����� true
		bool bE = false, bN = false, bT = false, bW = false, bS = false, bB = false;

		if (iE >= floc.maxelm) bE = true;
		if (iN >= floc.maxelm) bN = true;
		if (iT >= floc.maxelm) bT = true;
		if (iW >= floc.maxelm) bW = true;
		if (iS >= floc.maxelm) bS = true;
		if (iB >= floc.maxelm) bB = true;

		if ((bE) || (bW)) {
			dl = 0.5*dx;
			if (bE) resterm[iE] = 0.1*dl*dl; // O(h!2)
			if (bW) resterm[iW] = 0.1*dl*dl; // O(h!2)
		}
		if ((bN) || (bS)) {
			dl = 0.5*dy;
			if (bN) resterm[iN] = 0.1*dl*dl; // O(h!2)
			if (bS) resterm[iS] = 0.1*dl*dl; // O(h!2)
		}
		if ((bT) || (bB)) {
			dl = 0.5*dz;
			if (bT) resterm[iT] = 0.1*dl*dl; // O(h!2)
			if (bB) resterm[iB] = 0.1*dl*dl; // O(h!2)
		}
	}
	doublereal ret = Scal(resterm, resterm, floc.maxelm + floc.maxbound);
	delete[] resterm;
	resterm=NULL;
	return ret;
} // rterminate_residual_ICCG_Oh2

doublereal rterminate_residual_LR1sk_Oh3(FLOW floc) {
	// �������� ������������� O(h!2)
	doublereal* resterm = new doublereal[floc.maxelm + floc.maxbound];
	for (integer i = 0; i<floc.maxelm + floc.maxbound; i++) {
		resterm[i] = 0.0; // �������������.
	}

	for (integer iP = 0; iP<floc.maxelm; iP++) {
		// ���������� �������� �������� ������������ ������:
		doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
		volume3D(iP, floc.nvtx, floc.pa, dx, dy, dz);
		doublereal dl = fmin(dx, fmin(dy, dz));
		resterm[iP] = 0.1*dl*dl*dl; // O(h!3)
		integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
		iE = floc.sosedi[ESIDE][iP].iNODE1; iN = floc.sosedi[NSIDE][iP].iNODE1; iT = floc.sosedi[TSIDE][iP].iNODE1; iW = floc.sosedi[WSIDE][iP].iNODE1; iS = floc.sosedi[SSIDE][iP].iNODE1; iB = floc.sosedi[BSIDE][iP].iNODE1;
		// ���� � ����� �� ������ ����� ������� ��������� �������
		// �� ��������������� ���������� ����� true
		bool bE = false, bN = false, bT = false, bW = false, bS = false, bB = false;

		if (iE >= floc.maxelm) bE = true;
		if (iN >= floc.maxelm) bN = true;
		if (iT >= floc.maxelm) bT = true;
		if (iW >= floc.maxelm) bW = true;
		if (iS >= floc.maxelm) bS = true;
		if (iB >= floc.maxelm) bB = true;

		if ((bE) || (bW)) {
			dl = 0.5*dx;
			if (bE) resterm[iE] = 0.1*dl*dl*dl; // O(h!3)
			if (bW) resterm[iW] = 0.1*dl*dl*dl; // O(h!3)
		}
		if ((bN) || (bS)) {
			dl = 0.5*dy;
			if (bN) resterm[iN] = 0.1*dl*dl*dl; // O(h!3)
			if (bS) resterm[iS] = 0.1*dl*dl*dl; // O(h!3)
		}
		if ((bT) || (bB)) {
			dl = 0.5*dz;
			if (bT) resterm[iT] = 0.1*dl*dl*dl; // O(h!3)
			if (bB) resterm[iB] = 0.1*dl*dl*dl; // O(h!3)
		}
	}
	doublereal ret;
	//ret=Scal(resterm,resterm,floc.maxelm+floc.maxbound);
	ret = NormaV(resterm, floc.maxelm + floc.maxbound);
	// ������������ ����������� ������.
	if (resterm != NULL) {
		delete[] resterm;
		resterm = NULL;
	}
	return ret;
} // rterminate_residual_LR1sk_Oh3

doublereal rterminate_residual_LR1sk_temp_Oh3(TEMPER t) {
	// �������� ������������� O(h!2)
	// �������� ������������� ������������� � ������ ���� O(h).
	doublereal* resterm = new doublereal[t.maxelm + t.maxbound];
	for (integer i = 0; i<t.maxelm + t.maxbound; i++) {
		resterm[i] = 0.0; // �������������.
	}

	for (integer iP = 0; iP<t.maxelm; iP++) {
		// ���������� �������� �������� ������������ ������:
		doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
		volume3D(iP, t.nvtx, t.pa, dx, dy, dz);
		doublereal dl = fmin(dx, fmin(dy, dz));
		//resterm[iP]=0.1*dl*dl*dl; // O(h!3)
		resterm[iP] = dl; // O(h)
		integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
		iE = t.sosedi[ESIDE][iP].iNODE1; iN = t.sosedi[NSIDE][iP].iNODE1; iT = t.sosedi[TSIDE][iP].iNODE1; iW = t.sosedi[WSIDE][iP].iNODE1; iS = t.sosedi[SSIDE][iP].iNODE1; iB = t.sosedi[BSIDE][iP].iNODE1;
		// ���� � ����� �� ������ ����� ������� ��������� �������
		// �� ��������������� ���������� ����� true
		bool bE = false, bN = false, bT = false, bW = false, bS = false, bB = false;

		if (iE >= t.maxelm) bE = true;
		if (iN >= t.maxelm) bN = true;
		if (iT >= t.maxelm) bT = true;
		if (iW >= t.maxelm) bW = true;
		if (iS >= t.maxelm) bS = true;
		if (iB >= t.maxelm) bB = true;

		if ((bE) || (bW)) {
			dl = 0.5*dx;
			//if (bE) resterm[iE]=0.1*dl*dl*dl; // O(h!3)
			//if (bW) resterm[iW]=0.1*dl*dl*dl; // O(h!3)
			if (bE) resterm[iE] = dl; // O(h)
			if (bW) resterm[iW] = dl; // O(h)

		}
		if ((bN) || (bS)) {
			dl = 0.5*dy;
			//if (bN) resterm[iN]=0.1*dl*dl*dl; // O(h!3)
			//if (bS) resterm[iS]=0.1*dl*dl*dl; // O(h!3)
			if (bN) resterm[iN] = dl; // O(h)
			if (bS) resterm[iS] = dl; // O(h)

		}
		if ((bT) || (bB)) {
			dl = 0.5*dz;
			//if (bT) resterm[iT]=0.1*dl*dl*dl; // O(h!3)
			//if (bB) resterm[iB]=0.1*dl*dl*dl; // O(h!3)
			if (bT) resterm[iT] = dl; // O(h)
			if (bB) resterm[iB] = dl; // O(h)
		}
	}
	doublereal ret;
	//ret=Scal(resterm,resterm,f.maxelm+f.maxbound);
	ret = NormaV(resterm, t.maxelm + t.maxbound);
	// ������������ ����������� ������.
	if (resterm != NULL) {
		delete[] resterm;
		resterm = NULL;
	}
	return ret;
} // rterminate_residual_LR1sk_temp_Oh3



int main(void)
{
	getchar();



	

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
	
	//fgmres(m_restart)
	my_amg_manager.m_restart = 20; // ���������� �������� ��������� fgmres ����� ������������.

	// amg default settings:
	my_amg_manager.lfil = 2; // default value

	// ��������� ������������ �������������� ������ � ���������.
	// ��������� �������� ���� ������� �� ���� ��������� ������� ������� �� ����:
	// �������������� <-> ��������������, ����������������, ������������ ������ <-> ������������ ������,
	// ������� ������������ ��������� (����� ����������).
	//my_amg_manager.maximum_levels = 20; // ������������ ����� ������� ������� � �������� ���������� ��������.
	my_amg_manager.maximum_delete_levels = 0; // ���������� ������� ���������� � ������ ����� ��� ������ �����.
	my_amg_manager.number_interpolation_procedure = 3; // ����� ���������������� ���������.
	my_amg_manager.number_interpolation_procedure_Temperature = 3;
	my_amg_manager.number_interpolation_procedure_Speed = 3;
	my_amg_manager.number_interpolation_procedure_Pressure = 3;
	my_amg_manager.number_interpolation_procedure_Stress = 3;

	my_amg_manager.iCFalgorithm_and_data_structure=3; // 3-Treap.
	my_amg_manager.iCFalgorithm_and_data_structure_Temperature=3;// 3-Treap.
	my_amg_manager.iCFalgorithm_and_data_structure_Speed=3;// 3-Treap.
	my_amg_manager.iCFalgorithm_and_data_structure_Pressure=3;// 3-Treap.
	my_amg_manager.iCFalgorithm_and_data_structure_Stress=3;// 3-Treap.

	my_amg_manager.bTemperatureMatrixPortrait = 0; // NO_PRINT
	my_amg_manager.bSpeedMatrixPortrait = 0; // NO_PRINT
	my_amg_manager.bPressureMatrixPortrait = 0; // NO_PRINT
	my_amg_manager.bStressMatrixPortrait = 0; // NO_PRINT
	my_amg_manager.bMatrixPortrait = 0; // NO_PRINT


	my_amg_manager.nFinnest = 2; // ����� �������� �� ��������� �����.
	my_amg_manager.nu1 = 1; // ����� ���������������.
	my_amg_manager.nu2 = 2; // ����� ���� ����������.	
	my_amg_manager.memory_size = 9; // ���������� ����������� ������ � ������������ ������� �.
	my_amg_manager.gold_const = 0.2; // �������� ������� ���������� � ������������.
	my_amg_manager.gold_const_Temperature = 0.2;
	my_amg_manager.gold_const_Speed = 0.2;
	my_amg_manager.gold_const_Pressure = 0.2;
	my_amg_manager.gold_const_Stress = 0.2;
	my_amg_manager.bCFJacoby = true; // CF-Jacobi smoothers 12% ���������� ����� V ������. 5.06.2017
	// Runge-Kutt smoother: 3 - �������� �������, 5 - ������ �������, ����� ������ ����� �� ������������.
	my_amg_manager.iRunge_Kutta_smoother = 0;
	my_amg_manager.iFinnest_ilu = 0; // 0 - �� ������������, 1 - ������������, �� ������ �� ����� ��������� �����.
	// ������������� iluk ���������� �� �������� ������� ����������� ��� �������
	// �������� ������ nnz/n ����� ������ ������ ���� ������ 6 (�����).
	my_amg_manager.b_ilu_smoothers_in_nnz_n_LE_6 = false;
	my_amg_manager.theta = 0.24;
	my_amg_manager.magic = 0.4;
	my_amg_manager.F_to_F_Temperature = 0.4;
	my_amg_manager.F_to_F_Speed = 0.4; 
	my_amg_manager.F_to_F_Pressure = 0.4;
	my_amg_manager.F_to_F_Stress = 0.4;
	my_amg_manager.ilu2_smoother = 0; // 0 - �� ������������, 1 - ������������.

	my_amg_manager.itypemodifyinterpol=0; // ����� ����������� �������������.
	my_amg_manager.inumberadaptpass=0; // ������������ ���������� ������-�������� � �������������.

	my_amg_manager.theta_Temperature = 0.24;
	my_amg_manager.maximum_delete_levels_Temperature = 0;
	my_amg_manager.nFinnest_Temperature = 2;
	my_amg_manager.nu1_Temperature = 1;
	my_amg_manager.nu2_Temperature = 2;
	my_amg_manager.memory_size_Temperature = 9;
	my_amg_manager.ilu2_smoother_Temperature = 0; // 0 - �� ������������, 1 - ������������.
	// Speed
	my_amg_manager.theta_Speed = 0.24;
	my_amg_manager.maximum_delete_levels_Speed = 0;
	my_amg_manager.nFinnest_Speed = 2;
	my_amg_manager.nu1_Speed = 1;
	my_amg_manager.nu2_Speed = 2;
	my_amg_manager.memory_size_Speed = 9;
	my_amg_manager.ilu2_smoother_Speed = 0; // 0 - �� ������������, 1 - ������������.
	// Pressure
	my_amg_manager.theta_Pressure = 0.24;
	my_amg_manager.maximum_delete_levels_Pressure = 0;
	my_amg_manager.nFinnest_Pressure = 2;
	my_amg_manager.nu1_Pressure = 1;
	my_amg_manager.nu2_Pressure = 2;
	my_amg_manager.memory_size_Pressure = 9;
	my_amg_manager.ilu2_smoother_Pressure = 0; // 0 - �� ������������, 1 - ������������.
	// Stress
	my_amg_manager.theta_Stress = 0.24;
	my_amg_manager.maximum_delete_levels_Stress = 0;
	my_amg_manager.nFinnest_Stress = 2;
	my_amg_manager.nu1_Stress = 1;
	my_amg_manager.nu2_Stress = 2;
	my_amg_manager.memory_size_Stress = 9;
	my_amg_manager.ilu2_smoother_Stress = 0; // 0 - �� ������������, 1 - ������������.
	// AMG Splitting (coarsening)
	// ������ ���������� C-F ��������� : 0 - standart, 1 - RS 2.
	// RS 2 ���������� ������ ���������� C-F ��������� ���������� ������ ������.
	my_amg_manager.icoarseningTemp = 0; // standart
	my_amg_manager.icoarseningSpeed = 0; // standart
	my_amg_manager.icoarseningPressure=0; // standart
	my_amg_manager.icoarseningStress = 0; // standart
	my_amg_manager.icoarseningtype=0; // standart vs RS 2.
	// Stabilization BiCGStab.
	// 8.01.2017 ����� ��� ��� ������ BiCGStab 
	// ����������������� ��������������� ������������� �������.
	// 0 - ������������ ������ �������������� ������������� ����� ��� ������-���� ����������� ���������� ��������������� �������,
	// 1 - ������������ �������� �. ��� ��� ������ BiCGStab [1992], ����������������� �������������� ������������� �������.
	// 2 - ������������ �������� ����� � ������ FGMRes [1986], ����������������� �������������� ������������� �������.
	my_amg_manager.istabilizationTemp = 0; // none
	my_amg_manager.istabilizationSpeed = 0; // none
	my_amg_manager.istabilizationPressure = 0; // none
	my_amg_manager.istabilizationStress = 0; // none
	my_amg_manager.istabilization = 0; // none

	// ����� ������������ �����.
	my_amg_manager.ipatch_number = 0; // 0 - ���� �� ������������.

	// ������ ���� �� �������.
	my_amg_manager.iprint_log = 1;
	my_amg_manager.iprint_log_Temperature = 1;
	my_amg_manager.iprint_log_Speed = 1;
	my_amg_manager.iprint_log_Pressure=1;
	my_amg_manager.iprint_log_Stress = 1;

	// truncation for interpolation.
	// �� ��������� �������� ������������ �� ������������.
	my_amg_manager.itruncation_interpolation = 0; // 0 - off
	my_amg_manager.itruncation_interpolation_Temperature = 0;
	my_amg_manager.itruncation_interpolation_Speed = 0;
	my_amg_manager.itruncation_interpolation_Pressure=0;
	my_amg_manager.itruncation_interpolation_Stress = 0;
	// 0.2 recomended Stuben.
	my_amg_manager.truncation_interpolation = 0.2; // 0.2 recomended default value.
	my_amg_manager.truncation_interpolation_Temperature = 0.2;
	my_amg_manager.truncation_interpolation_Speed = 0.2;
	my_amg_manager.truncation_interpolation_Pressure=0.2;
	my_amg_manager.truncation_interpolation_Stress = 0.2;

	// GMRES smoother.
	my_amg_manager.b_gmresTemp = false;
	my_amg_manager.b_gmresSpeed = false;
	my_amg_manager.b_gmresPressure=false;
	my_amg_manager.b_gmresStress = false;
	my_amg_manager.b_gmres=false;

	// ����� �������.
	unsigned int calculation_main_start_time = 0; // ������ ����� ��.
	unsigned int calculation_main_end_time = 0; // ��������� ����� ��.
	unsigned int calculation_main_seach_time = 0; // ����� ���������� ������� ���� � ��.

	calculation_main_start_time = clock(); // ������ ������ �����.

	bool bextendedprint = false; // ������ �� ��������� ����� ����������� �����.

	// ��������������� ������ ��� �������� ���������� ����������.
	err_radiation_log = fopen_s(&fp_radiation_log, "log_radiation.txt", "a");
	if (err_radiation_log != 0) {
		printf("Error open file log.txt\n");
		printf("Please, press any key to continue...\n");
		//getchar();
		system("pause");
		exit(0);
	}

	//std::locale::global(std::locale("en_US.UTF-8"));
	system("mode con cols=126 lines=12000");
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
		

	printf("AliceFlow 3D x64 v0_07\n");
#ifdef _OPENMP 
	omp_set_num_threads(inumcore); // ��������� ����� �������
#endif

	errno_t err;
	err = fopen_s(&fp_log, "log.txt", "w");
	if (err != 0) {
		printf("Error open file log.txt\n");
		printf("Please, press any key to continue...\n");
		//getchar();
		system("pause");
		exit(0);
	}

	if (fp_log != NULL) {

		//ilu0_Saadtest();
		//printf("the end Saad ilu0 test\n");
		//getchar();

		// ���������� ����� �� ������ �� ����.
		//integer inx=120, iny=64, inz=64;
		integer inx = 30, iny = 30, inz = 30;
		integer inxadd = -1, inyadd = -1, inzadd = -1;
		doublereal dgx = 0.0, dgy = 0.0, dgz = 0.0; // ���� �������
		doublereal operatingtemperature = 20.0; // Operating Temperature 20.0 ����. �.

		

		premeshin("premeshin.txt", lmatmax, lb, ls, lw, matlist, b, s, w, dgx, dgy, dgz, inx, iny, inz, operatingtemperature,  ltdp, gtdps);
		if (iswitchMeshGenerator == 0) {
			simplemeshgen(xpos, ypos, zpos, inx, iny, inz, lb, ls, lw, b, s, w, matlist, xposadd, yposadd, zposadd, inxadd, inyadd, inzadd);
		}
		else if (iswitchMeshGenerator == 1) {
			unevensimplemeshgen(xpos, ypos, zpos, inx, iny, inz, lb, ls, lw, b, s, w, matlist, dgx, dgy, dgz, xposadd, yposadd, zposadd, inxadd, inyadd, inzadd); // ��������� ������������� ����� � �������������� ������������� ���������������.
		}
		else if (iswitchMeshGenerator == 2) {
			// � ��������� ������� coarse Mesh ��� � Icepak.
			// �������� ������ ����������� ��������������-���������������, � 
			// ������� ������������� ���������� ����������� �����, �.�. cpu �������� � 4��� �
			// ���������� ����������������� ��������� ������� ������� ��������.
			coarsemeshgen(xpos, ypos, zpos, inx, iny, inz, lb, ls, lw, b, s, w, matlist, xposadd, yposadd, zposadd, inxadd, inyadd, inzadd);
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


		if (b_on_adaptive_local_refinement_mesh) {
			printf("starting ALICE\n");
			// ��� ������� ������� �������� �� ������ ������������ �������,
			// � ������ �������� �� ��������� ���� � ��������� new ��� malloc.
			integer maxelm_loc = (inx + 1)*(iny + 1)*(inz + 1);
			bool bOkal=alice_mesh(xpos, ypos, zpos, inx, iny, inz, b, lb, lw, w, s, ls, maxelm_loc, xposadd, yposadd, zposadd, inxadd, inyadd, inzadd);
			//system("PAUSE");
			
			if (itype_ALICE_Mesh == 1) {
				// �������� ��������� ���������.

				while (!bOkal) {
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
					//getchar();
					//system("PAUSE");
					free_octree(oc_global);
					delete[] my_ALICE_STACK;
					top_ALICE_STACK = 0;
					printf("free octree end...\n");
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

					if (iswitchMeshGenerator == 0) {
						simplemeshgen(xpos, ypos, zpos, inx, iny, inz, lb, ls, lw, b, s, w, matlist, xposadd, yposadd, zposadd, inxadd, inyadd, inzadd);
					}
					else if (iswitchMeshGenerator == 1) {
						unevensimplemeshgen(xpos, ypos, zpos, inx, iny, inz, lb, ls, lw, b, s, w, matlist, dgx, dgy, dgz, xposadd, yposadd, zposadd, inxadd, inyadd, inzadd); // ��������� ������������� ����� � �������������� ������������� ���������������.
					}
					else if (iswitchMeshGenerator == 2) {
						// � ��������� ������� coarse Mesh ��� � Icepak.
						// �������� ������ ����������� ��������������-���������������, � 
						// ������� ������������� ���������� ����������� �����, �.�. cpu �������� � 4��� �
						// ���������� ����������������� ��������� ������� ������� ��������.
						coarsemeshgen(xpos, ypos, zpos, inx, iny, inz, lb, ls, lw, b, s, w, matlist, xposadd, yposadd, zposadd, inxadd, inyadd, inzadd);
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

					//getchar();
					//system("PAUSE");
				}
			}
			printf("end ALICE\n");
		}

		load_TEMPER_and_FLOW(t, f, inx, iny, inz, xpos, ypos, zpos, flow_interior, b, lb, lw, w, s, ls, operatingtemperature, matlist, bextendedprint, dgx, dgy, dgz, b_on_adaptive_local_refinement_mesh, false);
		
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

		// ������������ ����������� ������ �� ��� octree ������.
		if (b_on_adaptive_local_refinement_mesh) {
			printf("free octree start...\n");
			//getchar();
			//system("PAUSE");
			free_octree(oc_global);
			delete[] my_ALICE_STACK;
			top_ALICE_STACK = 0;
			printf("free octree end...\n");
			//getchar();
			//system("PAUSE");
		}

		if (0) {
			xyplot(f, flow_interior, t);
			printf("after load temper and flow. OK.\n");
			//getchar(); // debug avtosave
			system("pause");
		}

		// �� ���� ����� ���������� ����� �� �������� ���������������.
		if (!b_on_adaptive_local_refinement_mesh) {
			// ������ ���������� � �������� ������ ��� ������������� ������ LR:
			constr_line(f, flow_interior);  // ��� �������������
			t.rootBT = NULL;
			t.rootSN = NULL;
			t.rootWE = NULL;
			constr_line_temp(t, b, lb); // ��� ����������������
			printf("LR preprocessing finish...\n");
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
		if (0 && (flow_interior == 1)) {
			calc_front(f, f[0], t, flow_interior, ls, lw, w, nd);
			// ���������� ��������� !
			printf("separator compleate...\n");
			//getchar();
		}



		t.free_temper_level1 = false; // ������ ���������������� ������������ ������ ����������� ��� ������ ������� ����� �������� ������.
		t.free_temper_level2 = false; // ������������ ������ ��� �������� ������� ��� ���������� � � SIMPLESPARSE ������.	

		printf("construction of all structures...\n");
		printf("mesh check start...\n");
#if doubleintprecision == 1
		for (integer i = 0; i < inx; i++) if (fabs(xpos[i + 1] - xpos[i]) < 1.0e-23)
			printf("error: zalipanie po X: xpos[%lld]=%e xpos[%lld]=%e inx=%lld\n", i, xpos[i], i + 1, xpos[i + 1], inx);
		for (integer i = 0; i < iny; i++) if (fabs(ypos[i + 1] - ypos[i]) < 1.0e-23)
			printf("error: zalipanie po X: ypos[%lld]=%e ypos[%lld]=%e iny=%lld\n", i, ypos[i], i + 1, ypos[i + 1], iny);
		for (integer i = 0; i < inz; i++) if (fabs(zpos[i + 1] - zpos[i]) < 1.0e-23)
			printf("error: zalipanie po X: zpos[%lld]=%e zpos[%lld]=%e inz=%lld\n", i, zpos[i], i + 1, zpos[i + 1], inz);
		for (integer iP = 0; iP < t.maxelm; iP++) {
			if ((t.nvtx[0][iP] == 0) || (t.nvtx[1][iP] == 0) || (t.nvtx[2][iP] == 0) || (t.nvtx[3][iP] == 0) || (t.nvtx[4][iP] == 0) || (t.nvtx[5][iP] == 0) || (t.nvtx[6][iP] == 0) || (t.nvtx[7][iP] == 0)) {
				printf("nvtx[%lld] : %lld %lld %lld %lld %lld %lld %lld %lld \n", iP, t.nvtx[0][iP] - 1, t.nvtx[1][iP] - 1, t.nvtx[2][iP] - 1, t.nvtx[3][iP] - 1, t.nvtx[4][iP] - 1, t.nvtx[5][iP] - 1, t.nvtx[6][iP] - 1, t.nvtx[7][iP] - 1);
			}
		}
#else
		for (integer i = 0; i < inx; i++) if (fabs(xpos[i + 1] - xpos[i]) < 1.0e-23)
			printf("error: zalipanie po X: xpos[%d]=%e xpos[%d]=%e inx=%d\n", i, xpos[i], i + 1, xpos[i + 1], inx);
		for (integer i = 0; i < iny; i++) if (fabs(ypos[i + 1] - ypos[i]) < 1.0e-23)
			printf("error: zalipanie po X: ypos[%d]=%e ypos[%d]=%e iny=%d\n", i, ypos[i], i + 1, ypos[i + 1], iny);
		for (integer i = 0; i < inz; i++) if (fabs(zpos[i + 1] - zpos[i]) < 1.0e-23)
			printf("error: zalipanie po X: zpos[%d]=%e zpos[%d]=%e inz=%d\n", i, zpos[i], i + 1, zpos[i + 1], inz);
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
			printf("residual O(h!2) is equal=%e\n", f[i].resICCG);
			f[i].resLR1sk = rterminate_residual_LR1sk_Oh3(f[i]); // O(h!3)
			printf("residual O(h!3) is equal=%e\n", f[i].resLR1sk);
		}
		printf("TEMPERATURE\n");
		t.resLR1sk = rterminate_residual_LR1sk_temp_Oh3(t); // O(h!3)		
		printf("temp residual O(h!3) is equal=%e\n", t.resLR1sk);
		printf("mesh check.\n");
		if (bwait) {
			//getchar();
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
		if (inumber_iteration_SIMPLE == NULL) {
			// ������������ ������ �� ������ ������������.
			printf("Problem : not enough memory on your equipment for inumber_iteration_SIMPLE in main...\n");
			printf("Please any key to exit...\n");
			exit(1);
		}
		for (integer i = 0; i < flow_interior; i++) inumber_iteration_SIMPLE[i] = 0; // ��������� �������� ��������� SIMPLE ��� ������ FLUID ����.

		// ���������� ��������� ������� �� ����� ��� ������������� �������
		bool breadOk = false;
		avtoreadvalue(f, t, flow_interior, inumber_iteration_SIMPLE, continity_start, breadOk);
		// ���� ���������� ������ ��������� �� breadOk==false � ��� ������ ��� ���� ������� ������ �� �������� �������� ��� �������������.

		if (b_on_adaptive_local_refinement_mesh) {
			// ��������� ������������ ���� �����.
			printf("the invariant correctness...\n");
			ANES_ALICE_CORRECT(t.maxnod, t.pa, t.maxelm, t.nvtx);
		}

		// ������� ���������� ���������� � ��������� tecplot360:
		// ����� ������������ ��� �������� ����������� �����.
		if (0) {
			exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint,0);
			printf("read values. OK.\n");
			//getchar(); // debug avtosave
			system("pause");
		}



		/*for (integer i=0; i<lw; i++) {
		printf("%e  \n",w[i].Tamb);
		}
		//exporttecplotxy360T_3D(t.maxelm, t.ncell, t.nvtx, t.nvtxcell, t.pa, t.potent);
		exporttecplotxy360T_3D_part2(t.maxelm, t.potent);
		getchar(); // debug
		*/

		// 29.01.2017
		// if (1 && steady_or_unsteady_global_determinant == 2)  
		// �� �� ������ �������� ����� �� ������� �������.
		if (1 && steady_or_unsteady_global_determinant == 2) {
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
					// ������� ���������� ���������� � ��������� tecplot360:
					exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint, 0);
				}
				else {
					// ������� � ��������� ������� �����������.
					//� ���� �����.
					ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t,0);
				}
			}

			
		}

		// steady
		if (1 && steady_or_unsteady_global_determinant == 0) {

			// ����� �������.
			unsigned int calculation_start_time = 0; // ������ ����� ��.
			unsigned int calculation_end_time = 0; // ��������� ����� ��.
			unsigned int calculation_seach_time = 0; // ����� ���������� ������� ���� � ��.

			calculation_start_time = clock(); // ������ ������ �����.

			for (integer i7 = 0; i7<t.maxelm + t.maxbound; i7++) t.potent[i7] = operating_temperature_for_film_coeff; // �������������.

			// ������ ������ ������������� � ������ ����.
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
				printf("solution of pure heat...\n");
				printf("please, press any key to continue...\n");
				if (bwait) {
					//getchar();
					system("pause");
				}

				// ��� ������������ ������������� ����������� ��������.
				bool bprintmessage = true; // �������� �� ��������� �� �������.

				doublereal dbeta = 1.0; // ������ ������� ������������� �� �������.
				bool bmyconvective = false;
				if (starting_speed_Vx*starting_speed_Vx + starting_speed_Vy*starting_speed_Vy + starting_speed_Vz*starting_speed_Vz > 1.0e-30) {
					bmyconvective = true;
				}
				else {
					// �������� ������������� ��������� ��������.
					errno_t err_inicialization_data;
					FILE* fp_inicialization_data;
					err_inicialization_data = fopen_s(&fp_inicialization_data, "load.txt", "r");
					if (err_inicialization_data == 0) {
						// �������� ������ � ���� ������������.
						bmyconvective = true;
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
					NULL); // �������� ����� ����� ������� � ����������� ���������� ����.
				// ��������� �������� ������ 1.0 �������� ��� �������� �������.
				
				// ���������� ����� ������.
				massa_cabinet(t, f, inx, iny, inz,
					xpos, ypos, zpos, flow_interior,
					b, lb, operatingtemperature,
					matlist);

				// 10.10.2017
				// ���������� ���������� ������� ����� ���������.
				xyplot_temp(t, t.potent);
				//printf("graphics writing sucseful\n");
				//getchar();

				if (1) {
					if (!b_on_adaptive_local_refinement_mesh) {
						// ������� ���������� ���������� � ��������� tecplot360:
						exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint,0);
					}
					else {
						// ������� � ��������� ������� �����������.
						//� ���� �����.
						//ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent,t,0);
						
					}
				}
				//printf("marker stop\n");
				//getchar();
			}

			doublereal tmaxfinish = -273.15; // ���������� ����.
			// ���������� �������� ������������ ����������� ������ ��������� ������� � �� � ��������:
			//for (integer i = 0; i < t.maxelm + t.maxbound; i++) tmaxfinish = fmax(tmaxfinish, fabs(t.potent[i]));
			// 23 ������� 2015
			// �� ��������� ������ ���������� ����� �� ����� ��������� ������� �����������, �������
			// �������� �� ������� ����� � ��������� ����������� ������ �� ���������� ��. 
			for (integer i = 0; i < t.maxelm; i++) tmaxfinish = fmax(tmaxfinish, t.potent[i]);
			FILE *fp;
			errno_t err1;
			err1 = fopen_s(&fp, "report.txt", "w");
			// �������� ����� ��� ������.
			if ((err1) != 0) {
				printf("Create File report.txt Error\n");
				//getchar();
				system("pause");
			}
			else {
				// ������ ���������
				fprintf(fp, "Maximum Temperature %.2f\n", tmaxfinish);
				fclose(fp);
			}
			// 1 - solver/solid_static/
			report_temperature(flow_interior, f, t, b, lb, s, ls, w, lw, 0);

			if (1) {

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

					ANES_tecplot360_export_temperature_preobrazovatel(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, x_buf, y_buf, z_buf, t_buf, nvtx_buf, m_sizeT, m_size_nvtx);

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

					if (bvery_big_memory) {
						if (database.x != NULL) {
							free(database.x);
						}
						if (database.y != NULL) {
							free(database.y);
						}
						if (database.z != NULL) {
							free(database.z);
						}
						if (database.nvtxcell != NULL) {
							for (integer i = 0; i <= 7; i++) {
								delete[] database.nvtxcell[i];
							}
							delete[] database.nvtxcell;
						}
						if (database.ptr != NULL) {
							if (database.ptr[0] != NULL) {
								delete[] database.ptr[0];
							}
							if (database.ptr[1] != NULL) {
								delete[] database.ptr[1];
							}
							delete[] database.ptr;
						}
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
					load_TEMPER_and_FLOW(t, f, inx, iny, inz, xpos, ypos, zpos, flow_interior, b, lb, lw, w, s, ls, operatingtemperature, matlist, bextendedprint, dgx, dgy, dgz, b_on_adaptive_local_refinement_mesh, false);

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
					ALICE_2_Structural(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, x_buf, y_buf, z_buf, t_buf, nvtx_buf, m_sizeT, m_size_nvtx);


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
					exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint, 0);
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
				//ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent,t,0);
			}

			

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
					//getchar();
					system("pause");
				}

				// ��� ������������ ������������� ����������� ��������.
				bool bprintmessage = true; // �������� �� ��������� �� �������.

				doublereal dbeta = 1.0; // ������ ������� ������������� �� �������.
				bool bmyconvective = false;
				if (starting_speed_Vx*starting_speed_Vx + starting_speed_Vy*starting_speed_Vy + starting_speed_Vz*starting_speed_Vz > 1.0e-30) {
					bmyconvective = true;
				}
				else {
					// �������� ������������� ��������� ��������.
					errno_t err_inicialization_data;
					FILE* fp_inicialization_data;
					err_inicialization_data = fopen_s(&fp_inicialization_data, "load.txt", "r");
					if (err_inicialization_data == 0) {
						// �������� ������ � ���� ������������.
						bmyconvective = true;
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
				//bPhysics_stop = false;
				// ����� ������� Static Structural.
				solve_Structural(t, w, lw, m, false, operatingtemperature);
				//bPhysics_stop = true;

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
						exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint, 0);
					}
					else {
						// ������� � ��������� ������� �����������.
						//� ���� �����.
						ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t,0);
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

			FILE *fp;
			errno_t err1;
			err1 = fopen_s(&fp, "report.txt", "w");
			// �������� ����� ��� ������.
			if ((err1) != 0) {
				printf("Create File report.txt Error\n");
				//getchar();
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
					//getchar();
					system("pause");
				}

				// ��� ������������ ������������� ����������� ��������.
				bool bprintmessage = true; // �������� �� ��������� �� �������.

				doublereal dbeta = 1.0; // ������ ������� ������������� �� �������.
				bool bmyconvective = false;
				if (starting_speed_Vx*starting_speed_Vx + starting_speed_Vy*starting_speed_Vy + starting_speed_Vz*starting_speed_Vz > 1.0e-30) {
					bmyconvective = true;
				}
				else {
					// �������� ������������� ��������� ��������.
					errno_t err_inicialization_data;
					FILE* fp_inicialization_data;
					err_inicialization_data = fopen_s(&fp_inicialization_data, "load.txt", "r");
					if (err_inicialization_data == 0) {
						// �������� ������ � ���� ������������.
						bmyconvective = true;
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
				NULL); // �������� ����� ����� ������� � ����������� ���������� ����.
				// ��������� �������� ������ 1.0 �������� ��� �������� �������.
				
				//bPhysics_stop = false;
				// ����� ������� Static Structural.
				solve_Structural(t, w, lw, m, true, operatingtemperature);
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
						exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint, 0);
					}
					else {
						// ������� � ��������� ������� �����������.
						//� ���� �����.
						ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t,0);
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

			FILE *fp;
			errno_t err1;
			err1 = fopen_s(&fp, "report.txt", "w");
			// �������� ����� ��� ������.
			if ((err1) != 0) {
				printf("Create File report.txt Error\n");
				//getchar();
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
		getchar();
		exit(1);  // ����� ����������� ������������ ��������� ���������� ���������� �������� ������������ �����.
		}

		if (b_on_adaptive_local_refinement_mesh) {
		printf("Solve temperature is compleate. \n");
		getchar();
		exit(1);  // ����� ����������� ������������ ��������� ���������� ���������� �������� ������������ �����.
		}
		*/

		//system("pause");

		if (1 && steady_or_unsteady_global_determinant == 1) {
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
			unsteady_temperature_calculation(f[0], f, t,
				rhie_chow,
				b, lb, s, ls, w, lw,
				dbeta, flow_interior,
				false, matlist,
				operatingtemperature,
				gtdps, ltdp); // �������������� ������������� ������

			// ���������� ����� ������.
			massa_cabinet(t, f, inx, iny, inz,
				xpos, ypos, zpos, flow_interior,
				b, lb, operatingtemperature,
				matlist);

			if (!b_on_adaptive_local_refinement_mesh) {
				// ������� ���������� ���������� � ��������� tecplot360:
				exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint,0);
			}
			else {
				// ������� � ��������� ������� �����������.
				//� ���� �����.
				ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent,t,0);
			}

			doublereal tmaxfinish = -273.15;
			// ���������� �������� ������������ ����������� ������ ��������� ������� � �� � ��������:
			for (integer i = 0; i < t.maxelm + t.maxbound; i++) tmaxfinish = fmax(tmaxfinish, t.potent[i]);
			FILE *fp;
			errno_t err1;
			err1 = fopen_s(&fp, "report.txt", "w");
			// �������� ����� ��� ������.
			if ((err1) != 0) {
				printf("Create File report.txt Error\n");
				//getchar();
				system("pause");
			}
			else {
				// ������ ���������
				fprintf(fp, "Maximum Temperature %.2f\n", tmaxfinish);
				fclose(fp);
			}
			// 1 - solver/solid_static/
			report_temperature(flow_interior, f, t, b, lb, s, ls, w, lw, 0);

			printf("calculation complete...\n");
			// getchar();
		}

		fclose(fp_radiation_log);

		// ������� ���������� ���������� � ��������� tecplot360:
		// ����� ������������ ��� �������� ����������� �����.
		if (false) {
			exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint,0);
			printf("read values. OK.\n");
			if (bwait) {
				//getchar(); // debug avtosave
				system("pause");
			}
		}

		if ((1 && steady_or_unsteady_global_determinant == 3)) {
			// Fluid dynamic.

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
			//getchar();
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
			//getchar();

			// ������������ ����������������� ��������.
			steady_cfd_calculation(breadOk,
				eqin, dgx, dgy, dgz,
				continity_start,
				inumber_iteration_SIMPLE,
				flow_interior, f, t, b, lb,
				s, ls, w, lw, matlist,
				gtdps, ltdp, bextendedprint);
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
			exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint,0);
			save_velocity_for_init(t.maxelm, t.ncell, f, t, flow_interior);
			// exporttecplotxy360T_3D_part2_rev(t.maxelm, t.ncell, f, t, flow_interior, 0, bextendedprint,b,lb);
			delete[] bPamendment_source_old;
			delete[] told_temperature_global_for_HOrelax;
		}
		if (0) {
			

			told_temperature_global_for_HOrelax = new doublereal[t.maxelm + t.maxbound];
			bSIMPLErun_now_for_temperature = true;
			bHORF = true;
			bPamendment_source_old = new doublereal[f[0].maxelm + f[0].maxbound];
			for (integer i5 = 0; i5 < f[0].maxelm + f[0].maxbound; i5++) bPamendment_source_old[i5] = 0.0;
			// �������������� ����������������� �������� :
			usteady_cfd_calculation(breadOk, eqin,
				dgx, dgy, dgz,
				continity_start,
				inumber_iteration_SIMPLE,
				flow_interior,
				f, t,
				b, lb, s, ls,
				w, lw, matlist, gtdps, ltdp, bextendedprint);
			delete[] bPamendment_source_old;
			delete[] told_temperature_global_for_HOrelax;

			// ���������� ����� ������.
			massa_cabinet(t, f, inx, iny, inz,
				xpos, ypos, zpos, flow_interior,
				b, lb, operatingtemperature,
				matlist);
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
		//getchar();
		system("pause");
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

	// ������������ ������ �� ��� amg1r5.
	if (amgGM.a != NULL) {
		delete amgGM.a;
		amgGM.a = NULL;
	}
	if (amgGM.ia != NULL) {
		delete amgGM.ia;
		amgGM.ia = NULL;
	}
	if (amgGM.ja != NULL) {
		delete amgGM.ja;
		amgGM.ja = NULL;
	}
	if (amgGM.u != NULL) {
		delete amgGM.u;
		amgGM.u = NULL;
	}
	if (amgGM.f != NULL) {
		delete amgGM.f;
		amgGM.f = NULL;
	}
	if (amgGM.ig != NULL) {
		delete amgGM.ig;
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
	delete b; delete s; delete w; // ������������ ������
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
	delete[] gtdps;
	if (eqin.fluidinfo != NULL) {
		delete eqin.fluidinfo;
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
	}

	if (bvery_big_memory) {
		if (database.x != NULL) {
			free(database.x);
		}
		if (database.y != NULL) {
			free(database.y);
		}
		if (database.z != NULL) {
			free(database.z);
		}
		if (database.nvtxcell != NULL) {
			for (integer i = 0; i <= 7; i++) {
				delete[] database.nvtxcell[i];
			}
			delete[] database.nvtxcell;
		}
		if (database.ptr != NULL) {
			if (database.ptr[0] != NULL) {
				delete[] database.ptr[0];
			}
			if (database.ptr[1] != NULL) {
				delete[] database.ptr[1];
			}
			delete[] database.ptr;
		}
	}

	// ������������ ����� ������ � ILU �������.
	if (milu_gl_buffer.alu_copy != NULL) delete[] milu_gl_buffer.alu_copy;
	if (milu_gl_buffer.jlu_copy != NULL) delete[] milu_gl_buffer.jlu_copy;
	if (milu_gl_buffer.ju_copy != NULL) delete[] milu_gl_buffer.ju_copy;
	milu_gl_buffer.alu_copy = NULL;
	milu_gl_buffer.jlu_copy = NULL;
	milu_gl_buffer.ju_copy = NULL;



	flow_interior = 0;
	printf("free memory finish...\n");

	if (1 && steady_or_unsteady_global_determinant == 2) {
		// ��� ������ ����� ��������� ����������.
		printf("Mesh generation procedure is finish.\n");
	}
	else {
		printf("Calculation procedure is finish.\n");
	}
	printf("Please, press any key to exit...\n");
	if (bwait) {
		//getchar();
		system("pause");
	}

	

	calculation_main_end_time = clock();
	calculation_main_seach_time = calculation_main_end_time - calculation_main_start_time;


	/*printf("time=%d statistic vorst=%3.2f %% \n",calculation_main_seach_time,(float)(100.0*calculation_vorst_seach_time/calculation_main_seach_time));
	getchar();
	*/

	
	
	// ����� ����� ����������.
	int im=0, is=0, ims=0;
	im=(int)(calculation_main_seach_time/60000); // ������
	is=(int)((calculation_main_seach_time-60000*im)/1000); // �������
	ims=(int)((calculation_main_seach_time-60000*im-1000*is)/10); // ������������ ������� �� 10
	
	printf("time calculation is:  %d minute %d second %d millisecond\n", im, is, 10*ims);


	system("pause");
	return 0;
}