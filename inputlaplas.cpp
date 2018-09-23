// ���� inputlaplas.cpp
// �������� ���� �������� ������  
// ����������� ����������� ���������.


#ifndef _INPUTLAPLAS_CPP_
#define _INPUTLAPLAS_CPP_ 1


#include <math.h> // �������������� �������
//#include <string.h> // ������� ��������� �����
#include "power_temperature_depend.cpp" // ���������� ������������� �������� �������� ������� (����������� �������� �� �����������).

// ������������ �� ����� �������� ���������� �� ����������� ��������� �����.
doublereal etalon_max_size_ratio=2.0;
// �������� ���������� �� �������� ��������� �����.
doublereal etalon_max_size_ratio2 = 30.0; // 30 ������� �� ������������ FlowVision.
bool bsnap_TO_global = true; // snap to grid

// ���������
#define XY 1
#define XZ 2
#define YZ 3

// ��� �����
#define SOLID 1
#define HOLLOW 2
#define FLUID 3

// ����� � ��������� ������������
typedef struct TPOinteger {
	doublereal x, y, z;
} TOCHKA;

// ������ ��� enumerate_volume_improved
// � ��� uniformsimplemeshgen.cpp
// ��. ����� ������������ ���� constr_struct.cpp
typedef struct TBlock_indexes {
	integer iL, iR, jL, jR, kL, kR;
} Block_indexes;

// �������������� ��������
typedef struct TGEOM {
	integer itypegeom; // 0 - Prism, 1 - Cylinder, 2 - Polygon
	// Prism
	doublereal xS, yS, zS; // ���������� ������ �������
	doublereal xE, yE, zE; // ���������� ����� �������
	// Cylinder
	integer iPlane; // ��������� � ������� ����� ������ ��������� ��������.
	doublereal xC, yC, zC, Hcyl, R_out_cyl, R_in_cyl;
	// Polygon
	integer iPlane_obj2; // ��������� � ������� ����� ������ ��������� ��������.
	integer nsizei; // ���������� ������� ����� ���������� �������� �������.
	doublereal *hi, *xi, *yi, *zi;
} GEOM;

// �������� ����������
typedef struct TPROP {
	// ���������� �������� �������� �������������:
	// rho - ���������,
	// cp - �����������,
	// lam - ����������������,
	// mu - ������������ ��������,
	// beta_t - ����������� ��������� �������������� ����������.
	//doublereal rho, cp, lam;
	doublereal rho;
	// 16_11_2016
	// ����������� ��������� � �������� �������.
	// ������������ - ��������� ���������������� � �����������.
	integer n_lam, n_cp; // ����� ����� ��� �������� �������������.
	// arr_lam=f(temp_lam);
	// arr_cp=f(temp_cp);
	doublereal *temp_lam=NULL, *temp_cp=NULL, *arr_lam=NULL, *arr_cp=NULL;
	// ������������� ���������������� :
	// ��������� ������������ �������� ����� � ��������� � ����������� ����������������� :
	// CuMoCu, SiC � ������������� �����.
	// ���������������� � �������� ������������ ����������� ����������� �� ���� ���������.
	doublereal orthotropy_multiplyer_x, orthotropy_multiplyer_y, orthotropy_multiplyer_z;
	doublereal mu, beta_t;
	// ��������� ��� ��������� ��������� � ���������� ���������� 
	// ����������:
	integer blibmat; // 0 - �������� ������������ � ������������ 
	// ���������� ��������� ������������ ����. 1 - ������������ ��������.
	integer ilibident; // ������������� ������������� ���������.
	// �������������: 
	// 0 - ������� ����� ������� ��� � ����������, 
	// 100 - ������ ���� �������� ��� � ����������.
	// � ������ ���� ������������� ��������� �������� 0 � 100 ��
	// ������������ ���������� ��������� �������� �������������, �.�.
	// � ���� ������ blibmat - ����������� ����� 0.
	// 1-99 - ������������ ������� �����: ��������, ���� � ������ �������.
	// 101-<infinity ������������ ������ ����.
	bool bBussineskApproach; // ����� �� ������������ ����������� ��������-����������

	// ����� ������������� ������������� ��������������
	// ����������������� ���� ������������� ��������� ���������� �������:
	/* ����� ������ �������� ������
	*  0            ���������� �������� (������������ ��������) 
	*  1            ��������-�� ���� (power-law fluid)
	*  2            ������
	*  3            ��������
	*  4            Carreau
	*  5            ������-������
	*  6            ���������
	*/
	integer ilawmu; // ����� ������

	doublereal mumin, mumax; // ������������ ������������ ��������
	doublereal Amu, Bmu, Cmu, degreennmu; // ��������� ������� ��� ����������� �������� �� ���������� ������.

	// 8.4.2017 
	// ��������� ��������� ���������.
	doublereal mu_Lame, lambda_Lame; // ������������ ����.

} PROP;

// ��������� view factor ����� ����� ���������� ������������� ����� ����� ������ ����
// � �������������� �� ������ �����.
// 2 ������� 2016.
doublereal view_factor_perpendicular_rectangles_with_a_common_edge(doublereal x34, doublereal y34, doublereal z34)
{
	// ���������� View Factors ����� ����� ����������������� ���������� �������� ����� �����
	// �� ������ ������������ ������������ �������� ���������.
	// Heat and Mass Transfer: Fundamentals & Applications. Fourth Edition.
	// Yunus A.Cengel, Afshin J.Ghajar. McGraw-Hill, 2011.
	doublereal h = z34 / x34;
	doublereal w = y34 / x34;
	if ((h > 120.0) || (w > 120.0)) {
		printf("H=%e W=%e. calculate view factor error...\n",h,w);
		//getchar();
		system("PAUSE");
		exit(1);
	}
	// Isidoro Martinez Radiative View Factors. 1995-2016.
	doublereal MPI = 3.1415926;
	doublereal a = ((1.0 + h*h)*(1.0 + w*w)) / (1.0 + h*h + w*w);
	doublereal b = (w*w*(1.0 + h*h + w*w)) / ((1.0 + w*w)*(h*h + w*w));
	doublereal c = (h*h*(1.0 + h*h + w*w)) / ((1.0 + h*h)*(h*h + w*w));
	doublereal Fi_j = (1.0 / (MPI*w))*(h*atan(1.0 / h) + w*atan(1.0 / w) -
		sqrt(h*h + w*w)*atan(1.0 / (sqrt(h*h + w*w))) + 
		0.25*log(a*pow(b, w*w)*pow(c, h*h)));
	return Fi_j;
} // view_factor_perpendicular_rectangles_with_a_common_edge

// ��������� view factor ����� ����� ������������� ����������� ����������.
doublereal view_factor_aligned_parallel_rectangles(doublereal x34, doublereal y34, doublereal L34)
{
    // ���������� View Factors ����� ����� ������������� ���������� �� ������ ������������ ������������ 
	// �������� ���������.
	// Heat and Mass Transfer: Fundamentals & Applications. Fourth Edition.
	// Yunus A. Cengel, Afshin J.Ghajar. McGraw-Hill, 2011.
	doublereal x_tilda = x34 / L34;
	doublereal y_tilda = y34 / L34;
	if ((x_tilda > 120.0) || (y_tilda > 120.0)) {
		printf("X=%e Y=%e. calculate view factor error...\n", x_tilda, y_tilda);
		//getchar();
		system("pause");
		exit(1);
	}
	// Isidoro Martinez Radiative View Factors. 1995-2016.
	doublereal MPI = 3.1415926;
	doublereal x_tilda1 = sqrt(1.0 + x_tilda*x_tilda);
	doublereal y_tilda1 = sqrt(1.0 + y_tilda*y_tilda);
	doublereal Fi_j = (1.0 / (MPI*x_tilda*y_tilda))*(log((x_tilda1*x_tilda1*y_tilda1*y_tilda1) / (x_tilda1*x_tilda1 + y_tilda1*y_tilda1 - 1.0)) +
		2.0*x_tilda*(y_tilda1*atan(x_tilda / y_tilda1) - atan(x_tilda)) + 2.0*y_tilda*(x_tilda1*atan(y_tilda / x_tilda1) - atan(y_tilda)));
	return Fi_j;
} //view_factor_aligned_parallel_rectangles

// ��� ���������� ���������� ����������� �� ����� �����
// ������ ����� ����� (��� ��������� ������ ��� ���������� �����) � 
// (��������� ���� � ��������� � ���� �������� ���������� ��� ����� �� ������� ��������� �������).
typedef struct TMY_PAIR {
	integer node1, node21, node22, node23, node24; // ���� ����������� �� ����� -1.
	doublereal  dS1, dS2, dS3, dS4; // ������� ������.
	// 20 �������� 2016.
	// dS ������ ���� ����� ��������� �������.
	//doublereal dS;
} MY_PAIR;

// �������� ������ ���������� ��� ���������� ���������� ������������ �������.
const doublereal alpha_Radiation_block = 0.1;
// ���������� �������-��������� ��� ���������� ���������� ������������ �������.
const doublereal sigma_Radiation_block = 5.670367e-8; // Wxm!-2xK!-4.
// ������������� ����� �������� ��� ���������� ���������� ����������� �������.
const integer maxiter_Radiation_block = 1000; // ������ ������� � 200, ����� � �������.


typedef struct TBLOCKRADIATION {
	// emissivity:
	doublereal emissW, emissE, emissS, emissN, emissB, emissT;
	bool binternalRadiation;
	// View Factors for Prism Object.
	doublereal FWE, FWS, FWN, FWB, FWT;
	doublereal FEW, FES, FEN, FEB, FET;
	doublereal FSW, FSE, FSN, FSB, FST;
	doublereal FNW, FNE, FNS, FNB, FNT;
	doublereal FBW, FBE, FBS, FBN, FBT;
	doublereal FTW, FTE, FTS, FTN, FTB;
	// ����������� � ��������� �� ������ Prism Object:
	// ������� �������������� ����������� �� ����� Prism Object.
	doublereal TempW, TempE, TempS, TempN, TempB, TempT;
	// ��������� ������������ �������� ������� �� ������ Prism Object.
	doublereal JW, JE, JS, JN, JB, JT;
	

	// ������ ����� �� ������ �� ������ � �� ����������:
	MY_PAIR* nodelistW=NULL;
	integer nodelistWsize;
	MY_PAIR* nodelistE=NULL;
	integer nodelistEsize;
	MY_PAIR* nodelistS=NULL;
	integer nodelistSsize;
	MY_PAIR* nodelistN=NULL;
	integer nodelistNsize;
	MY_PAIR* nodelistB=NULL;
	integer nodelistBsize;
	MY_PAIR* nodelistT=NULL;
	integer nodelistTsize;
} BLOCKRADIATION;

// ����
typedef struct TBLOCK {
	integer itype; // ��� SOLID, HOLLOW ��� FLUID.
	GEOM g;
	integer imatid; // ������������� ��������� � ����������
	//doublereal Sc; // �������� �������������� �� ������� ������
	// 19 11 2016 ������������ ��������� �������� ��������������.
	integer n_Sc;
	doublereal *arr_Sc=NULL, *temp_Sc=NULL;
    // ����� ����������� �������� �������������� �� �������.
	integer ipower_time_depend; // 0 - �� �������, 1 - square wave �����������.
	// �� ����������� � ����������� ����������:
	// ������������� ����������� ������������, ������ ���������� ����������.
	BLOCKRADIATION radiation;
	bool bvisible; // ����� �� ���� ��� �������� � �������.

	// �������������� ����������� 
	// 0 - �� ����������� (����������� ��������),
	// n > 0 ����������� ����������� � ������� n.
	integer iunion_id;
	// ����������� ������� ������ �������� ?
	bool CylinderFixed;
} BLOCK;



// ��������� 30 view �������� ������ ���������� ����������.
// 3 ������� 2016.
void calculate_view_factors(BLOCK &b)
{
	doublereal x34, y34, z34, s34_W, s34_E, s34_S, s34_N, s34_B, s34_T;
	// min X (W)
	x34 = fabs(b.g.yE - b.g.yS); // x
	y34 = fabs(b.g.zE - b.g.zS); // y
	z34 = fabs(b.g.xE - b.g.xS); // z
	b.radiation.FWE = view_factor_aligned_parallel_rectangles(x34, y34, z34);
	b.radiation.FWS = view_factor_perpendicular_rectangles_with_a_common_edge(y34,x34,z34);
	b.radiation.FWN = view_factor_perpendicular_rectangles_with_a_common_edge(y34,x34,z34);
	b.radiation.FWB = view_factor_perpendicular_rectangles_with_a_common_edge(x34,y34,z34);
	b.radiation.FWT = view_factor_perpendicular_rectangles_with_a_common_edge(x34,y34,z34);
	s34_W = b.radiation.FWE + 2.0*(b.radiation.FWS + b.radiation.FWB); // ����������� ����� ������ ���� ����� 1.0.
	// max X (E)
	x34 = fabs(b.g.yE - b.g.yS);
	y34 = fabs(b.g.zE - b.g.zS);
	z34 = fabs(b.g.xE - b.g.xS);
	b.radiation.FEW = view_factor_aligned_parallel_rectangles(y34,x34,z34);
	b.radiation.FES = view_factor_perpendicular_rectangles_with_a_common_edge(y34,x34,z34);
	b.radiation.FEN = view_factor_perpendicular_rectangles_with_a_common_edge(y34,x34,z34);
	b.radiation.FEB = view_factor_perpendicular_rectangles_with_a_common_edge(x34,y34,z34);
	b.radiation.FET = view_factor_perpendicular_rectangles_with_a_common_edge(x34,y34,z34);
	s34_E = b.radiation.FEW + 2.0*(b.radiation.FES + b.radiation.FEB); // ����������� ����� ������ ���� ����� 1.0.
	// min Y (S)
	z34 = fabs(b.g.yE - b.g.yS);
	x34 = fabs(b.g.zE - b.g.zS);
	y34 = fabs(b.g.xE - b.g.xS);
	b.radiation.FSW = view_factor_perpendicular_rectangles_with_a_common_edge(x34,y34,z34);
	b.radiation.FSE = view_factor_perpendicular_rectangles_with_a_common_edge(x34,y34,z34);
	b.radiation.FSN = view_factor_aligned_parallel_rectangles(x34,y34,z34);
	b.radiation.FSB = view_factor_perpendicular_rectangles_with_a_common_edge(y34,x34,z34);
	b.radiation.FST = view_factor_perpendicular_rectangles_with_a_common_edge(y34,x34,z34);
	s34_S = b.radiation.FSN + 2.0*(b.radiation.FSW + b.radiation.FSB);
    // max Y (N)
	z34 = fabs(b.g.yE - b.g.yS);
	x34 = fabs(b.g.zE - b.g.zS);
	y34 = fabs(b.g.xE - b.g.xS);
	b.radiation.FNW = view_factor_perpendicular_rectangles_with_a_common_edge(x34,y34,z34);
	b.radiation.FNE = view_factor_perpendicular_rectangles_with_a_common_edge(x34,y34,z34);
	b.radiation.FNS = view_factor_aligned_parallel_rectangles(x34,y34,z34);
	b.radiation.FNB = view_factor_perpendicular_rectangles_with_a_common_edge(y34,x34,z34);
	b.radiation.FNT = view_factor_perpendicular_rectangles_with_a_common_edge(y34,x34,z34);
	s34_N = b.radiation.FNS + 2.0*(b.radiation.FNW + b.radiation.FNB);
	// min Z (B)
	x34 = fabs(b.g.yE - b.g.yS);
	z34 = fabs(b.g.zE - b.g.zS);
	y34 = fabs(b.g.xE - b.g.xS);
	b.radiation.FBW = view_factor_perpendicular_rectangles_with_a_common_edge(x34,y34,z34);
	b.radiation.FBE = view_factor_perpendicular_rectangles_with_a_common_edge(x34,y34,z34);
	b.radiation.FBS = view_factor_perpendicular_rectangles_with_a_common_edge(y34,x34,z34);
	b.radiation.FBN = view_factor_perpendicular_rectangles_with_a_common_edge(y34,x34,z34);
	b.radiation.FBT = view_factor_aligned_parallel_rectangles(x34,y34,z34);
	s34_B = b.radiation.FBT + 2.0*(b.radiation.FBW + b.radiation.FBS);
	// max Z (T)
	x34 = fabs(b.g.yE - b.g.yS);
	z34 = fabs(b.g.zE - b.g.zS);
	y34 = fabs(b.g.xE - b.g.xS);
	b.radiation.FTW = view_factor_perpendicular_rectangles_with_a_common_edge(x34,y34,z34);
	b.radiation.FTE = view_factor_perpendicular_rectangles_with_a_common_edge(x34,y34,z34);
	b.radiation.FTS = view_factor_perpendicular_rectangles_with_a_common_edge(y34,x34,z34);
	b.radiation.FTN = view_factor_perpendicular_rectangles_with_a_common_edge(y34,x34,z34);
	b.radiation.FTB = view_factor_aligned_parallel_rectangles(x34,y34,z34);
	s34_T = b.radiation.FTB + 2.0*(b.radiation.FTW + b.radiation.FTS);
	// log:
	printf("view factors:\n");
	printf("FWE=%1.3f FWS=%1.3f FWN=%1.3f FWB=%1.3f FWT=%1.3f sum=%1.3f\n", b.radiation.FWE, b.radiation.FWS, b.radiation.FWN, b.radiation.FWB, b.radiation.FWT, s34_W);
	printf("FEW=%1.3f FES=%1.3f FEN=%1.3f FEB=%1.3f FET=%1.3f sum=%1.3f\n", b.radiation.FEW, b.radiation.FES, b.radiation.FEN, b.radiation.FEB, b.radiation.FET, s34_E);
	printf("FSW=%1.3f FSE=%1.3f FSN=%1.3f FSB=%1.3f FST=%1.3f sum=%1.3f\n", b.radiation.FSW, b.radiation.FSE, b.radiation.FSN, b.radiation.FSB, b.radiation.FST, s34_S);
	printf("FNW=%1.3f FNE=%1.3f FNS=%1.3f FNB=%1.3f FNT=%1.3f sum=%1.3f\n", b.radiation.FNW, b.radiation.FNE, b.radiation.FNS, b.radiation.FNB, b.radiation.FNT, s34_N);
	printf("FBW=%1.3f FBE=%1.3f FBS=%1.3f FBN=%1.3f FBT=%1.3f sum=%1.3f\n", b.radiation.FBW, b.radiation.FBE, b.radiation.FBS, b.radiation.FBN, b.radiation.FBT, s34_B);
	printf("FTW=%1.3f FTE=%1.3f FTS=%1.3f FTN=%1.3f FTB=%1.3f sum=%1.3f\n", b.radiation.FTW, b.radiation.FTE, b.radiation.FTS, b.radiation.FTN, b.radiation.FTB, s34_T);
	//getchar();
	
} // calculate_view_factors


// ���������� �������� ���������������� ��� ����������� t_C � ��������.
doublereal get_lam(integer nsize_lam, doublereal* &temp_lam, doublereal* &arr_lam, doublereal t_C) {
	if (nsize_lam == 1) {
		return arr_lam[0];
	}
	else if (nsize_lam>1) {
		if (t_C < temp_lam[0]) {
			return arr_lam[0];
		}
		else if (t_C > temp_lam[nsize_lam - 1]) {
			return arr_lam[nsize_lam - 1];
		}
		else {
			integer if_3 = -1;
			for (integer i_sc = 0; i_sc < nsize_lam - 1; i_sc++) {
				if ((t_C >= temp_lam[i_sc]) && (t_C <= temp_lam[i_sc + 1])) {
					if_3 = i_sc;
					break;
				}
			}
			if (if_3 > -1) {
				doublereal af = (arr_lam[if_3+1]-arr_lam[if_3]) / (temp_lam[if_3 + 1] - temp_lam[if_3]);
				doublereal bf = (arr_lam[if_3] - af*temp_lam[if_3]);
				return (af*t_C+bf);
			}
			else {
				printf("error in get_lam function if_3==-1\n");
				system("pause");
				exit(1);
			}
		}
	}
	else {
		printf("error in get_lam function\n");
		system("pause");
		exit(1);
	}
} // get_lam

  // ���������� �������� ����������� ��� ����������� t_C � ��������.
doublereal get_cp(integer nsize_cp, doublereal* &temp_cp, doublereal* &arr_cp, doublereal t_C) {
	if (nsize_cp == 1) {
		return arr_cp[0];
	}
	else if (nsize_cp>1) {
		if (t_C < temp_cp[0]) {
			return arr_cp[0];
		}
		else if (t_C > temp_cp[nsize_cp - 1]) {
			return arr_cp[nsize_cp - 1];
		}
		else {
			integer if_3 = -1;
			for (integer i_sc = 0; i_sc < nsize_cp - 1; i_sc++) {
				if ((t_C >= temp_cp[i_sc]) && (t_C <= temp_cp[i_sc + 1])) {
					if_3 = i_sc;
					break;
				}
			}
			if (if_3 > -1) {
				doublereal af = (arr_cp[if_3 + 1] - arr_cp[if_3]) / (temp_cp[if_3 + 1] - temp_cp[if_3]);
				doublereal bf = (arr_cp[if_3] - af*temp_cp[if_3]);
					return (af*t_C + bf);
			}
			else {
				printf("error in get_cp function if_3==-1\n");
				system("pause");
				exit(1);
			}
		}
	}
	else {
		printf("error in get_cp function\n");
		system("pause");
		exit(1);
	}

} // get_cp

// 19 november 2016.
// ���������� �������� �������� ��������� �������� �������������� ��� ����������� t_C � ��������.
doublereal get_power(integer nsize_Sc, doublereal* &temp_Sc, doublereal* &arr_Sc, doublereal t_C) {
	if (nsize_Sc == 1) {
		return arr_Sc[0];
	}
	else if (nsize_Sc>1) {
		if (t_C < temp_Sc[0]) {
			return arr_Sc[0];
		}
		else if (t_C > temp_Sc[nsize_Sc - 1]) {
			return arr_Sc[nsize_Sc - 1];
		}
		else {
			integer if_3 = -1;
			for (integer i_sc = 0; i_sc < nsize_Sc - 1; i_sc++) {
				if ((t_C >= temp_Sc[i_sc]) && (t_C <= temp_Sc[i_sc + 1])) {
					if_3 = i_sc;
					break;
				}
			}
			if (if_3 > -1) {
				doublereal af = (arr_Sc[if_3 + 1] - arr_Sc[if_3]) / (temp_Sc[if_3 + 1] - temp_Sc[if_3]);
				doublereal bf = (arr_Sc[if_3] - af*temp_Sc[if_3]);
				return (af*t_C + bf);
			}
			else {
				printf("error in get_power function if_3==-1\n");
				system("pause");
				exit(1);
			}
		}
	}
	else {
		printf("error in get_power function\n");
		system("pause");
		exit(1);
	}
} // get_power

// ��������� ��������� ��������� ������ ��������� �� ������ ���������� ����������
// �� �������� ��������� :
// 1. ������������� ������������ �� ������ ����������: emissivity,
// 2. ���������� ������������ �� ������ ����������.
// 3. ������� View Factors �� ������ ����������.
void calculation_density_radiation_heat_flux(BLOCK &b)
{
	// ������ ���� ������ Prism Object �������� ���������.
	if (b.radiation.binternalRadiation)
	{
		
		// �������������:
		b.radiation.JW = 0.0;
		b.radiation.JE = 0.0;
		b.radiation.JS = 0.0;
		b.radiation.JN = 0.0;
		b.radiation.JB = 0.0;
		b.radiation.JT = 0.0;
		// ������������ ������������.
		doublereal apW = 1.0 + ((1.0 - b.radiation.emissW) / b.radiation.emissW)*1.0;
		doublereal apE = 1.0 + ((1.0 - b.radiation.emissE) / b.radiation.emissE)*1.0;
		doublereal apS = 1.0 + ((1.0 - b.radiation.emissS) / b.radiation.emissS)*1.0;
		doublereal apN = 1.0 + ((1.0 - b.radiation.emissN) / b.radiation.emissN)*1.0;
		doublereal apB = 1.0 + ((1.0 - b.radiation.emissB) / b.radiation.emissB)*1.0;
		doublereal apT = 1.0 + ((1.0 - b.radiation.emissT) / b.radiation.emissT)*1.0;

		//doublereal residual = 1.0; // �������.

		doublereal JW_new, JE_new, JS_new, JN_new, JB_new, JT_new;
		for (integer k = 0; k <= maxiter_Radiation_block; k++)
		{
			// �������� ������������������� ��� ����� ������ � �� ����� �� ������� ���������� ������������.

			// �������� !!! b.radiation.Temp � ���������.

			JW_new = alpha_Radiation_block*((sigma_Radiation_block*b.radiation.TempW*b.radiation.TempW*b.radiation.TempW*b.radiation.TempW + ((1.0 - b.radiation.emissW) / b.radiation.emissW)*
				(b.radiation.FWE*b.radiation.JE + b.radiation.FWS*b.radiation.JS + b.radiation.FWN*b.radiation.JN + b.radiation.FWB*b.radiation.JB +
				b.radiation.FWT*b.radiation.JT)) / apW) + (1.0 - alpha_Radiation_block)*b.radiation.JW;

			JE_new = alpha_Radiation_block*((sigma_Radiation_block*b.radiation.TempE*b.radiation.TempE*b.radiation.TempE*b.radiation.TempE + ((1.0 - b.radiation.emissE) / b.radiation.emissE)*
				(b.radiation.FEW*b.radiation.JW + b.radiation.FES*b.radiation.JS + b.radiation.FEN*b.radiation.JN + b.radiation.FEB*b.radiation.JB +
				b.radiation.FET*b.radiation.JT)) / apE) + (1.0 - alpha_Radiation_block)*b.radiation.JE;

			JS_new = alpha_Radiation_block*((sigma_Radiation_block*b.radiation.TempS*b.radiation.TempS*b.radiation.TempS*b.radiation.TempS + ((1.0 - b.radiation.emissS) / b.radiation.emissS)*
				(b.radiation.FSW*b.radiation.JW + b.radiation.FSE*b.radiation.JE + b.radiation.FSN*b.radiation.JN + b.radiation.FSB*b.radiation.JB +
				b.radiation.FST*b.radiation.JT)) / apS) + (1.0 - alpha_Radiation_block)*b.radiation.JS;

			JN_new = alpha_Radiation_block*((sigma_Radiation_block*b.radiation.TempN*b.radiation.TempN*b.radiation.TempN*b.radiation.TempN + ((1.0 - b.radiation.emissN) / b.radiation.emissN)*
				(b.radiation.FNW*b.radiation.JW + b.radiation.FNE*b.radiation.JE + b.radiation.FNS*b.radiation.JS + b.radiation.FNB*b.radiation.JB +
				b.radiation.FNT*b.radiation.JT)) / apN) + (1.0 - alpha_Radiation_block)*b.radiation.JN;

			JB_new = alpha_Radiation_block*((sigma_Radiation_block*b.radiation.TempB*b.radiation.TempB*b.radiation.TempB*b.radiation.TempB + ((1.0 - b.radiation.emissB) / b.radiation.emissB)*
				(b.radiation.FBW*b.radiation.JW + b.radiation.FBE*b.radiation.JE + b.radiation.FBS*b.radiation.JS + b.radiation.FBN*b.radiation.JN +
				b.radiation.FBT*b.radiation.JT)) / apB) + (1.0 - alpha_Radiation_block)*b.radiation.JB;

			JT_new = alpha_Radiation_block*((sigma_Radiation_block*b.radiation.TempT*b.radiation.TempT*b.radiation.TempT*b.radiation.TempT + ((1.0 - b.radiation.emissT) / b.radiation.emissT)*
				(b.radiation.FTW*b.radiation.JW + b.radiation.FTE*b.radiation.JE + b.radiation.FTS*b.radiation.JS + b.radiation.FTN*b.radiation.JN +
				b.radiation.FTB*b.radiation.JB)) / apT) + (1.0 - alpha_Radiation_block)*b.radiation.JT;

			// ���������� �������.

			// update:
			b.radiation.JW = JW_new;
			b.radiation.JE = JE_new;
			b.radiation.JS = JS_new;
			b.radiation.JN = JN_new;
			b.radiation.JB = JB_new;
			b.radiation.JT = JT_new;
		}

		printf("TW=%e TE=%e TS=%e TN=%e TB=%e TT=%e\n", b.radiation.TempW, b.radiation.TempE, b.radiation.TempS, b.radiation.TempN, b.radiation.TempB, b.radiation.TempT);
		printf("JW=%e JE=%e JS=%e JN=%e JB=%e JT=%e\n", b.radiation.JW, b.radiation.JE, b.radiation.JS, b.radiation.JN, b.radiation.JB, b.radiation.JT);
	//	getchar();

		
		fprintf_s(fp_radiation_log, "%e %e ", b.radiation.TempB, b.radiation.TempT);
				
		
	}
} // calculation_density_radiation_heat_flux

// �������� ������������ � ����� ������� ��
// ����������� � �������� �����.
// ���������� � ����������� �������� � �������� �������� 
// �������. ������� ����� �� ����������� ������� � ��������
// �.�. �������.
typedef struct TTEMP_DEP_POWER {
	char* sname; // ��� ���������� ����� ����������� ��������� �������.
	
	integer intemp; // ���������� ��������� ���������� �������� �����������.
	integer inoffset_drain; // ���������� ��������� ���������� �������� �������� �����.
	
	// �������: ����� ������� �����������,
	// ������� ������ ��� ������� �������� - �������� �����.
	// ��������� �������� � ������� ������������� ������������ ��������.
	doublereal* rtemp; // �������� �����������
	doublereal* roffset_drain; // �������� �������� �����
	doublereal** rpower_table; // ������� ���������.

} TEMP_DEP_POWER;

// �������� �����
typedef struct TSOURCE {
	// �������� ��������� �����
	// � ������ ���� �������� ������� �� ������������ ����������� � �������� ����� ��
	// ��� �������� ������������� ��������������� ����� ������� �������, 
	// ��� ��� � ���� ������ ���������� ������� ������ ������ �� �����.
	doublereal power; 
	// �������� ����� �������� �� ����������� � �������� �����.
	// ����������� ������� �������� �� ������ �������� � ��������� �.�. �������.
	bool bgarber_depend; // ������ �� ����������� ������������ �������� �� ����������� � �������� �����.
	// � ������ bgarber_depend == true, �������� power �������� ������������� �� ������� ����������� 
	// �������� ��������� �������� ��������.
	integer igarber_depend; // ���������� ����� ��������� �����������. (��������� ���������� � ����).
	doublereal roperation_offset_drain; // ������� �������� �������� �����.
	doublereal power_multiplyer; // �� ��� ��������� ����������� �������� ��������, ��� ������ ��� ���������������
	// ��������� �������� �������� ����� ����� ������� �������� �����������.

	doublereal square; // ������� ��������� �����
	integer iPlane; // ��������� � ������� ����� �������� �����
	// 1 - XY, 2 - XZ, 3 - YZ.
	GEOM g; // ������� �������.

	// �������������� ����������� 
	// 0 - �� ����������� (����������� ��������),
	// n > 0 ����������� ����������� � ������� n.
	integer iunion_id;
} SOURCE;

// ������ (��������� �����������)
typedef struct TWALL {
	integer ifamily; // ��� �������� ������� �� �����������
	doublereal Tamb, hf; // �������� ����������� �� ��������� ����������� � �������� �����.
	doublereal emissivity, film_coefficient;
	bool bsymmetry, bpressure, bopening;
	doublereal Vx, Vy, Vz, P;
	// Thermal-Stress boundary condition
	// 0 - free,
	// 1 - x fixit,
	// 2 - y fixit,
	// 3 - z fixit,
	// 4 - xy fixit,
	// 5 - xz fixit,
	// 6 - yz fixit,
	// 7 - fixit all,
	// 8 - x Force,
	// 9 - y Force,
	// 10 - z Force.
	integer ithermal_Stress_boundary_condition;
	doublereal xForce, yForce, zForce; // ��� ���������� ����, ����������� � ���� � ��������.
    integer iPlane; // ��������� � ������� ����� ������
	// 1 - XY, 2 - XZ, 3 - YZ.
	GEOM g; // ������� �������.
	// ������������� ������� � ������� ��������� ���
	// ������� ��������� �����������.
	//bool bfixboundary; // true �������������, false ���������.

	// �������������� ����������� 
	// 0 - �� ����������� (����������� ��������),
	// n > 0 ����������� ����������� � ������� n.
	integer iunion_id;
} WALL;

// ��������� ������� ������������ ��� 
// ��������� ������ �������������.
typedef struct TSMAGORINSKYINFO {
	doublereal Cs; // ��������� �������������.
	bool bDynamic_Stress; // ��� ����������� ��������� ������������� Cs ������������ ������������ ������ �������.
	bool bLimiters_Cs; // ������������ �� ���������� ������������� ?
	doublereal minCs, maxCs; // ����������� � ������������ �������� ��������� �������������.
	integer itypeFiltrGermano; // ��� ������� ������� ������������ ��� ���������� � ������ �������.
	doublereal roughness; // �������� ������������� �� ������ ����������� ������.
						  // ���������� ������� ��� ����� ������������� �� ������.
	integer ipowerroughness; // ����� ��������� �������� ������ 1 ��� 2.
	bool bfdelta; // ������������ �� �������� ������ ��������� �� ������������� ����� ?
	bool bSmagorinsky_Lilly; // ������������ �� ������ �������������-����� ?
	bool bsurface_roughness; // ������������ �� �������� ����������� ������������� ������ ?
	bool bSelectiveSmagorinsky; // ������������ �� Selective Smagorinsky Model ?
	integer itypeFILTRSelectiveSmagorinsky; // ��� ������� ������� ������������ ��� ���������� � ������ Selective Smagorinsky.
	doublereal SSangle; // ���� ����� ������ � ���������� ������ � ������ Selective Smagorinsky;
	bool bRichardsonCorrect; // ������������ �� �������� ��������� � ������ ���������� ��� ������� � ��������� ����� ����.
	doublereal rRichardsonMultiplyer; // ����������� � ����������� ������� ��������� � ��������� ����� ����.
} SMAGORINSKYINFO;

// ��� ������������� ������:
typedef struct TNODELR {
	integer id; // ������������� ����������� ����
	struct TNODELR *next = NULL; // ������ �� ��������� ���� ��� NULL
} NODELR;

typedef struct TNODELR_BASE {
	integer ilineid; // ������������� �������� �����
	integer iN; // ���������� ������� ����� ������� ���������
	struct TNODELR *root = NULL; // ������ ����� �������� �����
	struct TNODELR_BASE *next = NULL; // ��������� �� ��������� �������� ����� ��� NULL

									  // ������ ������ :
									  // ��������� �������� ���������� 
									  // ������� ��������� �����.
									  // ���������� ����� ������ ���� ����� � ���������� 
									  // �������� (� ������ ���� ������ �������� � ������� ����������).
	bool bNeimanStart;
	bool bNeimanEnd;
} NODELR_BASE;




typedef struct TBOUND {
	// ��������� ���� � ���� ������ ��������� �������
	integer iB, iI, iII;
	integer iI1, iI2;
	integer Norm; // ���������� �������
				  // marker boundary:
				  /*
				  * � ������� �������:
				  * if (MCB < ls) ������ �������� � ������� MCB;
				  * else if (MCB < ls+lw) ������ ������ c ������� MCB-ls;
				  * else �������� �� ���������. MCB==ls+lw.
				  */
	integer MCB; // marker boundary

				 // ������ �� ������� �������,
				 // �� ������� ����� � �������:
				 // �� ���� �������� ����� ������ ����.
				 // ���� ������� �����������, �� ����� -1.
				 // ����� �������� �� ����� 4 �������, �� 
				 // ����� ������� ������ ��-�� 
				 // ���������������� ��������.
				 // ������� ������������� �������� �����.
	integer iW[6];

	// ��� ����������� ��������� ���������
	// �� ��������� ���������� ���� � �������� �������
	// ��������� ��������������� ������� �������, ��� �� -1
	// ���� �������� �� ������� hollow �����.
	// TODO 6 ��� 2016.

	// ��� ����������� ���������� ������ emissivity.
	doublereal emissivity; // ������������� ����������� �������.

						   // ������� �����.
						   // ��� ����� ���� �������� ���� 20 �������� 2016.
	doublereal dS;

} BOUND;


// ���������� � ������ ����
typedef struct TFLOWINFO {
	// ������� �����
	doublereal xc, yc, zc;
	// ����� �� ����������� ���� �������
	integer iflow;
	// ����� ������� : 0 - ����������, 1 - ������������
	integer iflowregime;
	// ������ ��������������
	// 0 - Zero Equation Turbulence Model (RANS).
	// 1 - ������ ������������� (LES). (��� ����� ������, ������ ������� 1991 (LES)).
	// 2 - RNG (LES).
	integer iturbmodel;
	// ��������� ������ �������������:
	doublereal Cs; // ���������� �������������.
	bool bDynamic_Stress; // ���� == true �� �������� ������������ ������ ������� ��� ����������� �������� ���������� �������������.
	bool bLimiters_Cs; // �������� ����������� �� ���������� ������������� : ����������� �� ����������� � ������������ ��������.
	doublereal rminCs, rmaxCs; // ����������� � ������������ ����������� ��� ��������� �������������.
	integer itypeFiltrGermano; // �������� ������ ������� ������������ ��� ���������� � ������ ������� 1991 ����.
	// �������� �� ������������� �����,
	// ������ �������������-�����,
	// ���� ������������� ������.
	integer bfdelta, bSmagorinskyLilly, bsurface_roughness;
	doublereal roughness; // ������������� ������.
	integer ipowerroughness; // ���������� ������� � ������ ����� ������������� ������.
	doublereal rRi_mult; // �������������� ��������� ������������� ������������ ����� ����������.
	doublereal rSelectiveAngle; // ��������� �������� ���� � ������ Selective Smagorinsky.
	integer itypeSelectiveSmagorinsky_filtr; // ��� ������� � ������ �������������.
	// �������� �� ������� � ��������� ����� ����,
	// ������������� ������ �������������.
	bool bSwirlAmendment, bSelectiveSmagorinsky; 
} FLOWINFO;

// ��� ���������� � ������ ������ �������� ���������
typedef struct TEQUATIONINFO {
	// ����� �� ������ ��������� ����������������
	// 0 - �������, 1 - �����.
	integer itemper;
	// ������������ ���������� ������������� �� ��������
	// ������ ��� (FLUID interior)
	integer imaxflD;
	// ���� ������ � ����������� � ������ �����.
	FLOWINFO* fluidinfo=NULL;
} EQUATIONINFO;

EQUATIONINFO eqin; // ���������� � ������ �������� ���������

struct Tdatabase {
	doublereal *x = NULL, *y = NULL, *z = NULL; // ���������� �����.
	integer maxelm;
	integer** nvtxcell = NULL;
	integer ncell;
	// ����� ������������� � ��������������.
	integer **ptr = NULL;// ��� ������������ ��������������� �������������� ������
};

// ���� ������ ������� ���� � 3D ��������
// ��� ����������� ��.
typedef struct Tequation3D {
	doublereal ap, ae, an, aw, as, at, ab, b;
	integer iP, iE, iN, iT, iW, iS, iB;
	// ���������� ��������� ������ ��� ���� �����.
	// �� ���� ����� ������ ����� ���������� ����� ������ � 
	// �� ����� ���������� �� ������������� ������� ��
	// 25 ��������� �����������.
	// �� ���� ����� ���� ����� ������������ ������� �� ����� ������������� ������������ � 
	// ��� ������� ���� � ����� �������� ����� ����������� "��������" ������ ���� BiCGStab + ILU2.
	// ������� ������������� �������������� ������:
	// true ���� ����������� ����������.
	bool bE2, bN2, bT2, bW2, bS2, bB2;
	bool bE3, bN3, bT3, bW3, bS3, bB3;
	bool bE4, bN4, bT4, bW4, bS4, bB4;
	// �������� ���������� ������������:
	doublereal ae2, an2, aw2, as2, at2, ab2;
	doublereal ae3, an3, aw3, as3, at3, ab3;
	doublereal ae4, an4, aw4, as4, at4, ab4;
	// ���������� ��� �������������� ������.
	integer iE2, iN2, iT2, iW2, iS2, iB2;
	integer iE3, iN3, iT3, iW3, iS3, iB3;
	integer iE4, iN4, iT4, iW4, iS4, iB4;
	// ������� ���������� ��� ������������� ������������,
	// ��� ������ ����� ����� �������������� ���������
	// �������������� �����:
	doublereal ae_dop, aw_dop, an_dop, as_dop, at_dop, ab_dop;
	integer iE_dop, iW_dop, iN_dop, iS_dop, iT_dop, iB_dop;
} equation3D;

// ���� ������ ������� ���� � 3D ��������
// ��� ���������� ��.
typedef struct Tequation3D_bon {
	doublereal aw, ai, b; // wall, internal, ������ �����
	integer iW, iI;

	// ������ �� ������� �������,
	// �� ������� ����� � �������:
	// �� ���� �������� ����� ������ ����.
	integer iW1, iW2, iW3, iW4;
} equation3D_bon;

typedef struct TTEMPER {
	// ���� ���������� �� ������������ 
	// ������ ������� ������. ���������:
	// ����� ������ ������� ���� ����� 
	// ���������� ����� ��� ��������� �����������
	// ��� ������ �������. ��� ����� ������� 
	// ������ � ��� ������ ���� ������� ����������
	// ���� ��������, ��� �������� � ������ �������
	// ��� ��������� ������ ����������������.
	bool free_temper_level1;
	// ���� ���������� �� ������������ ������
	// ������� ������. ����� ������� ���� ����������������
	// � ������ SIMPLESPARSE. �������� ������� � ������� 
	// equation3D ����� ������ �� ����������� ������ ����������.
	bool free_temper_level2;

	integer maxnod; // ������������ ����� ���� (����������� �������)
					// pa[0..maxnod-1];
	TOCHKA* pa = NULL; // ���������� ����� ����� ������������� ��������� �������

	integer maxelm; // ����� ��������� ����������� �������
					// nvtx[0..7][0..maxelm-1]
	integer **nvtx = NULL; // ������ ����� ��� ������� �������� (���������� ������������ ������)
						   // sosedi[0..11][0..maxelm-1]
						   //integer **sosedi; // �������� ����������� ������ ��� ������� ����������� ������������ ������
						   // AliceMesh
	ALICE_PARTITION **sosedi = NULL;// �������� ����������� ������ ��� ������� ����������� ������������ ������
	integer maxbound; // ����� ��������� �����
	integer maxp; // maxp == maxelm + maxbound;
				  // sosedb[0..maxbound-1];
	BOUND* sosedb = NULL; // ��������� ���� ��������� ������� 
						  // ��� ���� ��������� ��. ����� ������ ���� ����� ���� �
						  // �������� ������ ������ ��������� ������� ������ �� ��� 
						  // ���������� ������ ������� ���������� ������ �������� � �� 
						  // ���� ��� ������� ����������� �������� � �� ������ ������ ����.
	bool* binternalsource;

	// ������ ����� ����������� ���������� ��
	integer* whot_is_block;

	integer **ptr = NULL; // ����� � ��������������.

						  // ��� ����������� ������������
	integer ncell; // ���������� ������ ��� ����������� �������.
	integer **nvtxcell = NULL; // ����� ��� ����������� �������.

							   // ��� ���� ����� ������ ����� ������ ������, ��� 
							   // ����������� ��� ������ �������.
							   // ilevel_alice[0..maxelm-1].
	integer *ilevel_alice = NULL;


	doublereal *potent = NULL; // ������ ������� ����������� (������� �������)
	doublereal **total_deformation = NULL; // ������ ����������.

										   // �������� ���������� ����������� ���  
										   // ���������� � ��� ��������� ��.
										   // ������� prop[0..2][0..maxelm-1]
	doublereal **prop = NULL; // �������� ���������� ��� ���������� ��.
							  // ������� prop_b[0..2][0..maxbound-1]
	doublereal **prop_b = NULL; // �������� ��� ��������� ��.


	doublereal *Sc = NULL; // �������� �������������� ������������ �� ���� ��������� ����������� �����.
	integer *ipower_time_depend = NULL; // ����� ����������� �������� �������������� �� �������.

	doublereal alpha; // �������� ����������
	equation3D *slau = NULL; // ������������ ������� ���� ��� ���������� ��
	equation3D_bon *slau_bon = NULL; // ������������ ������� ���� ��� ��������� ��

									 // ��� ������������� ������ :
									 // ������������ ����� ������������ ����. ������������ ������������ �. ����������.
									 // ������������ ����� �������� ��������� ������������ - �� ������ ��������� �������� 
									 // ������� ������ ��  ��������� ��������. ��� ������� � ��� ��� ������������ ����� 
									 // ����� ���� ����������� ��� ������������������� � ��������� ���-���-������ - BiCGStab.
	NODELR_BASE *rootWE = NULL;
	NODELR_BASE *rootSN = NULL;
	NODELR_BASE *rootBT = NULL;

	integer iWE, iSN, iBT; // ����� �������� ����� ����� ������� �� �����������.

						   // ��� ������������� ������ LR1:
						   // ������ ����� ���������� � 
						   // ������������ ���� ��� � �� 
						   // ������ ��� � �����. 
						   // ��� ������ �������� ����������.
						   // �.�. ���������� ��������� �������������� �� ������ ����� ����������
						   // � ������������ ����������� ��� ������ �������� - ���� ��������� � ����
						   // ����������� ������. � ������ �������� ����� ������������ ��������� ����� 
						   // ����� ������� ��������� ������ ������� �� ������ ������ ����� ������� �� ���������
						   // �� �������� ����������.

						   // �������� ������� ��� �����������
						   // ������������� � ��������� ������������� ���������.
	doublereal resLR1sk; // O(h!3)

						 // ����� �������� �������� ��� �������������� ������.
	integer inx_copy, iny_copy, inz_copy;
	doublereal operatingtemperature_copy;

	doublereal *xpos_copy = NULL, *ypos_copy = NULL, *zpos_copy = NULL;

	// 9 ������� 2015.
	// ��� ����������������� 
	integer *ifrontregulationgl = NULL;
	integer *ibackregulationgl = NULL; // �������� ��������������.

	doublereal operatingtemperature = 20.0;

	Tdatabase database;

}  TEMPER;

typedef struct TFLOW {
	integer maxnod; // ������������ ����� ���� (����������� �������)
	integer maxelm; // ����� ���������� ����������� �������
					// nvtx[0..7][0..maxelm-1]
	integer **nvtx = NULL; // ������ ����� ��� ������� ����������� �������� (������������ ������)

						   // pa[0..maxnod-1]
	TOCHKA* pa = NULL; // ���������� ����� ����� ������������� ��������� �������

					   // sosedi[0..11][0..maxelm-1]
					   //integer **sosedi; // �������� ����������� ������ ��� ������� ����������� ��
					   // ��� ALICEMESH �����.
	ALICE_PARTITION **sosedi = NULL;// �������� ����������� ������ ��� ������� ����������� ��
	integer maxbound; // ����� ��������� ��
	integer maxp; // ����� ���������
				  // sosedb[0..maxbound-1];
	BOUND* sosedb = NULL; // ��������� ���� ��������� �������

	integer *ptr = NULL; // ����� � �����������������


						 // ������ ����� ����������� ���������� ��
	integer* whot_is_block = NULL;

	// potent[iVar][0..maxp-1]
	doublereal **potent = NULL; // ������ ������� ����������� (������� �������)
								// prop[0..2][0..maxelm-1]
	doublereal **prop = NULL; // �������� ����������
							  // prop_b[0..2][0..maxbound-1]
	doublereal **prop_b = NULL; // �������� ���������� ��� ��������� ��

	doublereal *alpha = NULL; // ��������� ������ ����������
	equation3D **slau = NULL; // ������������ ������� ���� ��� ���������� ��.
	equation3D_bon **slau_bon = NULL; // ������������ ������� ���� ��� ��������� ��
									  // ��� ���������� �������������� ���-��� ��������� ������� ������������ ������������.
	doublereal **diag_coef = NULL;
	doublereal OpTemp; // Operating Temperature


	bool bactive; // �����-�� ������������ ���� �������
	bool bPressureFix; // ����� �� ����������� �������� � ����� �����
	bool bLR1free; // ����� �� ��������� ��������� ������������ ������ (�� ���������� ����� ������� ����������).

				   // ��� ������������� ������ :
				   // ������������ ����� ������������ ����. ������������ ������������ �. ����������.
				   // ������������ ����� �������� ��������� ������������ - �� ������ ��������� �������� 
				   // ������� ������ ��  ��������� ��������. ��� ������� � ��� ��� ������������ ����� 
				   // ����� ���� ����������� ��� ������������������� � ��������� ���-���-������ - BiCGStab.

	integer iWE, iSN, iBT; // ����� �������� ����� ����� ������� �� �����������.
	integer** iN = NULL; //iN[3][max3(iWE,iSN,iBT)];
	integer*** id = NULL; //id[3][max3(iWE,iSN,iBT)][max(iN)]; 

						  // ��� ������������� ������ LR1:
						  // ������ ����� ���������� � 
						  // ������������ ���� ��� � �� 
						  // ������ ��� � �����. 
						  // ��� ������ �������� ����������.
						  // �.�. ���������� ��������� �������������� �� ������ ����� ����������
						  // � ������������ ����������� ��� ������ �������� - ���� ��������� � ����
						  // ����������� ������. � ������ �������� ����� ������������ ��������� ����� 
						  // ����� ������� ��������� ������ ������� �� ������ ������ ����� ������� �� ���������
						  // �� �������� ����������.

						  // � ���������� ����������� ������� �����������,
						  // ����� ���������� ��������� ��������������� ���������� ��� ������� ������������ �������.
						  // ����� ������� ��� ������ ���� FLUID
	integer iflowregime; // default LAMINAR
						 // ���������� ���������� �� ��������� ������ [0..maxelm-1]
	doublereal* rdistWall = NULL; // ���������� �� ��������� ������ ������.
								  // ������� ������������ ���� � ������� ���������
	doublereal rdistWallmax;
	// S ��������� ������� ���������-����������
	doublereal* SInvariantStrainRateTensor = NULL; // [0..maxelm+maxbound-1]; // ���������������� ����.

												   // �������� ����� ����� ����� �� :
	doublereal** mf = NULL;

	SMAGORINSKYINFO smaginfo; // ��������� ������ �������������.

							  // �������� ������� ��� �������� ��������
							  // ������������� � ��������� ������������� ���������.
	doublereal resICCG; // O(h!2)
	doublereal resLR1sk; // O(h!3)

						 // ��� ���������� ������ mass balance ��� ����������� ������������ �����
						 // ����� ������������� ������ �� ��������� ����������������� �������� ���
						 // ���������� ��.
	integer *icolor_different_fluid_domain = NULL; // ������ ����������������� ���������� ����� ������ �����.

												   // 9 ������� 2015.
												   // ��� ����������������� 
	integer *ifrontregulationgl = NULL;
	integer *ibackregulationgl = NULL; // �������� ��������������.

} FLOW;




// �����������
// ��� ������ ����������������� ��������� �����.
// ������ ���������� 25.04.2018.
typedef struct TUNION {
	// id ��������� �� ����������.
	integer id; // ���������� ����� �����������.

				// �� �������� ����� ������� ��� Hollow ����
				// � ���� ����� ������������� ������.
				// ������� ���������� �� ����������.
	doublereal xS, xE, yS, yE, zS, zE;

	// ���������� ����� union.
	// Union �������� ��������� ��� ����� ���������� ������.
	// ��� ����������� �����������.
	doublereal *xpos = NULL, *ypos = NULL, *zpos = NULL;
	doublereal *xposadd = NULL, *yposadd = NULL, *zposadd = NULL;

	// ��� ���������� ����� (�����������).
	// ��������� �� ����������.
	integer inx, iny, inz;
	// ��� ����������� �����������.
	integer inxadd = -1, inyadd = -1, inzadd = -1;

	//  ��� ��������� ����������
	// ��������� �� ����������.
	integer iswitchMeshGenerator = 2; // 2 - CoarseMeshGen

									  // ��������� ����������.
	TEMPER t;
	integer flow_interior; // ��������� ����� FLUID ���
	FLOW* f = NULL;
} UNION;



// ���������� ���������� �� 
// �������� ����� premeshin.txt
void premeshin(const char *fname, integer &lmatmax, integer &lb, integer &ls, integer &lw, TPROP* &matlist, BLOCK* &b, SOURCE* &s, WALL* &w, 
	           doublereal &dgx, doublereal &dgy, doublereal &dgz, integer &inx, integer &iny, integer &inz, doublereal &operatingtemperature, 
			   integer &ltdp, TEMP_DEP_POWER* &gtdps, integer &lu, UNION* &my_union) {


#ifdef MINGW_COMPILLER
	// eqin - ���������� � ������ �������� ���������.

	// dgx, dgy, dgz - ������ ���� �������.
	// inx, iny, inz - ���������� ����� �� ������ �� ����.

	FILE *fp;
	errno_t err1;
	if ((err1 = fopen_s(&fp, fname, "r")) != 0) {
		printf("No input File premeshin.txt \n");
		//getchar();
		system("pause");

	}
	else
	{
		if (fp != NULL) {
			float fin = 0.0;
			integer din = 0;
			doublereal scale = 1.0;
			doublereal dbuf; // ��� �������������� � ������� �����������

			fscanf(fp, "%d", &din);
			ionly_solid_visible=din;
			printf("ionly_solid_visible =%d\n", ionly_solid_visible);
			fscanf(fp, "%f", &fin);
			scale = fin;
			fscanf(fp, "%d", &din);
			lmatmax = din;
			fscanf(fp, "%d", &din);
			lb = din;
			fscanf(fp, "%d", &din);
			ls = din;
			fscanf(fp, "%d", &din);
			lw = din;
			fscanf(fp, "%d", &din);
			ltdp = din; // ���������� ���������� ������ � ���������� ������� �� ����������� ����������� �������� �� �����������.
			
						// ��������� �������� ������� ���� �������:
			fscanf(fp, "%f", &fin);
			dgx = fin;
			fscanf(fp, "%f", &fin);
			dgy = fin;
			fscanf(fp, "%f", &fin);
			dgz = fin;

			// ��������� ���������� ����� �� ������ ������������ ���
			fscanf(fp, "%d", &din);
			inx = din;
			fscanf(fp, "%d", &din);
			iny = din;
			fscanf(fp, "%d", &din);
			inz = din;

			fscanf(fp, "%f", &fin);
			operatingtemperature = fin; // Operating Temperature
			operating_temperature_for_film_coeff=fin;


			// ������������� ��������� �������� ����������.
			// ������ �������� ��� ���� ��������� �������.
			// initialization value.
			fscanf(fp, "%f", &fin);
			starting_speed_Vx = fin;
			fscanf(fp, "%f", &fin);
			starting_speed_Vy = fin;
			fscanf(fp, "%f", &fin);
			starting_speed_Vz = fin;

			// ��������� ���������� ������� ����� ����� ������� �������� ���������������� ����� Variation Plot.
			fscanf(fp, "%f", &fin);
			Tochka_position_X0_for_XY_Plot = scale*fin;
			fscanf(fp, "%f", &fin);
			Tochka_position_Y0_for_XY_Plot = scale*fin;
			fscanf(fp, "%f", &fin);
			Tochka_position_Z0_for_XY_Plot = scale*fin;
			// ����������� ����� ��������� � ������������ 
			// ����� �� ���� ���������� ������������� ������� ���������:
			// 0 - Ox, 1 - Oy, 2 - Oz.
			fscanf(fp, "%d", &din);
			idirectional_for_XY_Plot = din;

			fscanf(fp, "%f", &fin);
			etalon_max_size_ratio = fin; // ����������� ��������� �����.

			fscanf(fp, "%f", &fin);
			etalon_max_size_ratio2 = fin; // �������� �������� ��������� ����� �� ������ FlowVision.

			fscanf(fp, "%d", &din);
			switch (din) {
			case 0: bsnap_TO_global = false;  break;
			default : bsnap_TO_global = true;  break;
			}
			

			fscanf(fp, "%d", &din);
			iswitchsolveramg_vs_BiCGstab_plus_ILU2 = din; // ����� ��������� ���������� : ���� amg1r5 ���� BiCGStab+ILU2.

			fscanf(fp, "%d", &din);
			iswitchsolveramg_vs_BiCGstab_plus_ILU6 = din; // ����� ��������� ���������� : ���� �����0.14 ���� BiCGStab+ILU6.

			fscanf(fp, "%d", &din);
			if (din == 1) {
				// SIMPLEC algorithm.
				iSIMPLE_alg = SIMPLEC_Van_Doormal_and_Raithby;
			}
			else {
				// SIMPLE algorithm 1972.
				iSIMPLE_alg = SIMPLE_Carretto;
			}

			fscanf(fp, "%d", &din);
			// ����� ����� ��� ������ ��������.
			// �������� ��� ����������� ������ ��������� ��������������� 
			// ������������ � ����� my_approx_convective2.c
			switch (din) {
			case 1: iFLOWScheme = UNEVEN_MUSCL; break; // MUSCL 2
			case 2: iFLOWScheme = UNEVEN_SOUCUP; break; // SOUCUP [MINMOD] 2
			case 3: iFLOWScheme = UNEVEN_HLPA; break; // HLPA 2
			case 4: iFLOWScheme = UNEVEN_SMART; break; // SMART 3
			case 5: iFLOWScheme = UNEVEN_WACEB; break; // WACEB 3 TVD
			case 6: iFLOWScheme = UNEVEN_SMARTER; break; // SMARTER 3
			case 7: iFLOWScheme = UNEVEN_LPPA; break; // LPPA 3
			case 8: iFLOWScheme = UNEVEN_VONOS; break; // VONOS 3
			case 9: iFLOWScheme = UNEVEN_STOIC; break; // STOIC
			case 10: iFLOWScheme = UNEVEN_CLAM; break; // CLAM
			case 11: iFLOWScheme = UNEVEN_OSHER; break; // OSHER
			case 12: iFLOWScheme = UNEVEN_EXPONENTIAL; break; // EXPONENTIAL
			case 13: iFLOWScheme = UNEVEN_SUPER_C; break; // SUPER_C
			case 14: iFLOWScheme = UNEVEN_ISNAS; break; // ISNAS
			case 15: iFLOWScheme = UNEVEN_CUBISTA; break; // CUBISTA
			default: iFLOWScheme = 2; break; // UDS ����� ���������� �����.
			}

			fscanf(fp, "%d", &din);
			// ����� ����� ��� ����������� � ������ ��������.
			// �������� ��� ����������� ������ ��������� ��������������� 
			// ������������ � ����� my_approx_convective2.c
			switch (din) {
			case 1: iTEMPScheme = UNEVEN_MUSCL; break; // MUSCL 2
			case 2: iTEMPScheme = UNEVEN_SOUCUP; break; // SOUCUP [MINMOD] 2
			case 3: iTEMPScheme = UNEVEN_HLPA; break; // HLPA 2
			case 4: iTEMPScheme = UNEVEN_SMART; break; // SMART 3
			case 5: iTEMPScheme = UNEVEN_WACEB; break; // WACEB 3 TVD
			case 6: iTEMPScheme = UNEVEN_SMARTER; break; // SMARTER 3
			case 7: iTEMPScheme = UNEVEN_LPPA; break; // LPPA 3
			case 8: iTEMPScheme = UNEVEN_VONOS; break; // VONOS 3
			case 9: iTEMPScheme = UNEVEN_STOIC; break; // STOIC
			case 10: iTEMPScheme = UNEVEN_CLAM; break; // CLAM
			case 11: iTEMPScheme = UNEVEN_OSHER; break; // OSHER
			case 12: iTEMPScheme = UNEVEN_EXPONENTIAL; break; // EXPONENTIAL
			case 13: iTEMPScheme = UNEVEN_SUPER_C; break; // SUPER_C
			case 14: iTEMPScheme = UNEVEN_ISNAS; break; // ISNAS
			case 15: iTEMPScheme = UNEVEN_CUBISTA; break; // CUBISTA
			default: iTEMPScheme = 2; break; // UDS ����� ���������� �����.
			}


			// ����� ��������� ����������.
			fscanf(fp, "%d", &din);
			iswitchMeshGenerator = din;


			fscanf(fp, "%d", &din);
			steady_or_unsteady_global_determinant = 2;
			if ((din == 0) || (din == 1) || (din == 2) || (din == 3) || (din == 5) || (din == 6)) {
				// 2 - mesh generator only.
				// 3 - fluid dynamic.
				// 5 - Static Structural
				// 6 - Thermal Stress
				steady_or_unsteady_global_determinant = din; // steady - 0, or unsteady -1 calculation.
			}
			else {
				printf("error input parametr steady or unsteady calculation\n");
				system("PAUSE");
				exit(1);
			}

			fscanf(fp, "%d", &din);
			if ((din == 0) || (din == 1) || (din == 2) || (din == 3)) {
				glTSL.id_law = din;
			}
			else {
				printf("error input parametr timestep law\n");
				system("PAUSE");
				exit(1);
			}
			fscanf(fp, "%f", &fin);
			glTSL.Factor_a_for_Linear = fin; // Factor_a
			if ((fin<=0.0)||(fin>=1.0)) {
				printf("error input parametr timestep law Factor a\n");
				system("PAUSE");
				exit(1);
            }
			fscanf(fp, "%f", &fin);
			if (fin<=0.0) {
				printf("error input parametr timestep law tau must be strongly positive\n");
				system("PAUSE");
				exit(1);
            }
			glTSL.tau = fin; // ������������ ��������.
			fscanf(fp, "%f", &fin);
			glTSL.Q = fin;  // ����������.
			// ��������� ����������� ������ ��� ���� �������.
			fscanf(fp, "%f", &fin);
			if ((fin<=0.0)||(fin>=1.0)) {
				printf("error input parametr timestep law APPARAT multiplyer\n");
				system("PAUSE");
				exit(1);
			}
			glTSL.m1 = fin;
			fscanf(fp, "%f", &fin);
			if (fin<=0.0) {
				printf("error input parametr timestep law APPARAT tau1 must be strongly positive\n");
				system("PAUSE");
				exit(1);
			}
			glTSL.tau1 = fin;			
			fscanf(fp, "%f", &fin);
			if (fin<=0.0) {
				printf("error input parametr timestep law APPARAT tau2 must be strongly positive\n");
				system("PAUSE");
				exit(1);
			}
			glTSL.tau2 = fin;
			fscanf(fp, "%f", &fin);
			if (fin<=0.0) {
				printf("error input parametr timestep law APPARAT tau_pause must be strongly positive\n");
				system("PAUSE");
				exit(1);
			}
			glTSL.tau_pause = fin;
			fscanf(fp, "%d", &din);
			glTSL.n_cycle = din;
			fscanf(fp, "%f", &fin);
			if (fin<=0.0) {
				printf("error input parametr timestep law APPARAT Period must be strongly positive\n");
				system("PAUSE");
				exit(1);
			}
			glTSL.T_all = fin;
			doublereal t_pause_gl = glTSL.T_all - glTSL.n_cycle*(2 * glTSL.tau1 + glTSL.tau2 + glTSL.tau_pause);
			if (t_pause_gl <= 0.0) {
				printf("error in parameters Square Wave APPARAT time step law.\n");
				getchar();
				exit(1);
			}

			fscanf(fp, "%f", &fin);
			if (fin <= 0.0) {
				printf("error input parametr on_time_double_linear law hot cold reshime must be strongly positive\n");
				system("PAUSE");
				exit(1);
			}
			glTSL.on_time_double_linear = fin;


			// ����� ��������� ��������������� ������������� ��� ������� ������������� � ������ ����.
			fscanf(fp, "%f", &fin);
			globalEndTimeUnsteadyTemperatureCalculation = fin;

			// Newton-Richman condition.
			fscanf(fp, "%d", &din);
			adiabatic_vs_heat_transfer_coeff = din;  // 0 - adiabatic wall, 1 - Newton Richman condition, 2 - Stefan Bolcman condition, 3 - mix condition.
			fscanf(fp, "%f", &fin);
			film_coefficient = fin;
			// A��� �����
			fscanf(fp, "%d", &din);
			if (din==0) {
				// ������� ����������������� �����.
				b_on_adaptive_local_refinement_mesh = false;
			}
			else {
				// ����
				b_on_adaptive_local_refinement_mesh = true;
			}
			fscanf(fp, "%d", &din);
			itype_ALICE_Mesh=din;
			fscanf(fp, "%d", &din);
			my_amg_manager.m_restart = din;
			// classical algebraic multigrid parameters:
			// only for my_agregat_amg.cu.
			fscanf(fp, "%d", &din);
			my_amg_manager.imySortAlgorithm = din;
			fscanf(fp, "%d", &din);
			//my_amg_manager.maximum_levels = din;
			my_amg_manager.maximum_delete_levels_Temperature = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.maximum_delete_levels_Speed = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.maximum_delete_levels_Pressure = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.maximum_delete_levels_Stress = din;

			// type interpolation procedure :
			//fscanf(fp, "%d", &din);
			//my_amg_manager.number_interpolation_procedure = din;

			fscanf(fp, "%d", &din);
			my_amg_manager.number_interpolation_procedure_Temperature = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.number_interpolation_procedure_Speed = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.number_interpolation_procedure_Pressure = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.number_interpolation_procedure_Stress = din;

			fscanf(fp, "%d", &din);
			my_amg_manager.iCFalgorithm_and_data_structure_Temperature = din;// 3-Treap.
			fscanf(fp, "%d", &din);
			my_amg_manager.iCFalgorithm_and_data_structure_Speed = din;// 3-Treap.
			fscanf(fp, "%d", &din);
			my_amg_manager.iCFalgorithm_and_data_structure_Pressure = din;// 3-Treap.
			fscanf(fp, "%d", &din);
			my_amg_manager.iCFalgorithm_and_data_structure_Stress = din;// 3-Treap.

			fscanf(fp, "%d", &din);
			my_amg_manager.itypemodifyinterpol = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.inumberadaptpass = din;

			// 23.02.2018
			// print matrix portrait
			fscanf(fp, "%d", &din);
			my_amg_manager.bTemperatureMatrixPortrait = din; // 0 - NO_PRINT, 1 - PRINT.
			fscanf(fp, "%d", &din);
			my_amg_manager.bSpeedMatrixPortrait = din; // 0 - NO_PRINT, 1 - PRINT.
			fscanf(fp, "%d", &din);
			my_amg_manager.bPressureMatrixPortrait = din; // 0 - NO_PRINT, 1 - PRINT.
			fscanf(fp, "%d", &din);
			my_amg_manager.bStressMatrixPortrait = din; // 0 - NO_PRINT, 1 - PRINT.

			// 01.05.2017
			// truncation of interpolation:
			fscanf(fp, "%d", &din);
			my_amg_manager.itruncation_interpolation_Temperature = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.itruncation_interpolation_Speed = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.itruncation_interpolation_Pressure = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.itruncation_interpolation_Stress = din;
			fscanf(fp, "%f", &fin);
			my_amg_manager.truncation_interpolation_Temperature = fin;
			fscanf(fp, "%f", &fin);
			my_amg_manager.truncation_interpolation_Speed = fin;
			fscanf(fp, "%f", &fin);
			my_amg_manager.truncation_interpolation_Pressure = fin;
			fscanf(fp, "%f", &fin);
			my_amg_manager.truncation_interpolation_Stress = fin;

			// number nFinnest sweeps :
			fscanf(fp, "%d", &din);
			my_amg_manager.nFinnest_Temperature = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.nFinnest_Speed = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.nFinnest_Pressure = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.nFinnest_Stress = din;

			// number presweeps:
			fscanf(fp, "%d", &din);
			my_amg_manager.nu1_Temperature = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.nu1_Speed = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.nu1_Pressure = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.nu1_Stress = din;

			// number postsweeps :
			fscanf(fp, "%d", &din);
			my_amg_manager.nu2_Temperature = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.nu2_Speed = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.nu2_Pressure = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.nu2_Stress = din;

			// memory size :
			fscanf(fp, "%d", &din);
			my_amg_manager.memory_size_Temperature = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.memory_size_Speed = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.memory_size_Pressure = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.memory_size_Stress = din;


			// �������� ������� ���������� � ������������.
			fscanf(fp, "%f", &fin);
			my_amg_manager.gold_const_Temperature = fin;
			fscanf(fp, "%f", &fin);
			my_amg_manager.gold_const_Speed = fin;
			fscanf(fp, "%f", &fin);
			my_amg_manager.gold_const_Pressure = fin;
			fscanf(fp, "%f", &fin);
			my_amg_manager.gold_const_Stress = fin;

			// ������������ �� ilu2 smoother.
			fscanf(fp, "%d", &din);
			if (din == 3) {
				din = 0;
				my_amg_manager.ilu2_smoother_Temperature = din;
				my_amg_manager.b_gmresTemp = true;
			}
			else {
				my_amg_manager.ilu2_smoother_Temperature = din;
			}
			fscanf(fp, "%d", &din);
			if (din == 3) {
				din = 0;
				my_amg_manager.ilu2_smoother_Speed = din;
				my_amg_manager.b_gmresSpeed = true;
			}
			else {
				my_amg_manager.ilu2_smoother_Speed = din;
			}			
			fscanf(fp, "%d", &din);
			if (din == 3) {
				din = 0;
				my_amg_manager.ilu2_smoother_Pressure = din;
				my_amg_manager.b_gmresPressure = true;
			}
			else {
				my_amg_manager.ilu2_smoother_Pressure = din;
			}			
			fscanf(fp, "%d", &din);
			if (din == 3) {
				din = 0;
				my_amg_manager.ilu2_smoother_Stress = din;
				my_amg_manager.b_gmresStress = true;
			}
			else {
				my_amg_manager.ilu2_smoother_Stress = din;
			}
			

			// strength threshold :
			fscanf(fp, "%f", &fin);
			my_amg_manager.theta_Temperature = fin;
			fscanf(fp, "%f", &fin);
			my_amg_manager.theta_Speed = fin;
			fscanf(fp, "%f", &fin);
			my_amg_manager.theta_Pressure = fin;
			fscanf(fp, "%f", &fin);
			my_amg_manager.theta_Stress = fin;

			// magic threshold :
			//fscanf(fp, "%f", &fin);
			//my_amg_manager.magic = fin;
			// magic <=> F_to_F
			fscanf(fp, "%f", &fin);
			my_amg_manager.F_to_F_Temperature = fin;
			fscanf(fp, "%f", &fin);
			my_amg_manager.F_to_F_Speed = fin;
			fscanf(fp, "%f", &fin);
			my_amg_manager.F_to_F_Pressure = fin;
			fscanf(fp, "%f", &fin);
			my_amg_manager.F_to_F_Stress = fin;

			// AMG Splitting (coarsening)
			// ������ ���������� C-F ��������� : 0 - standart, 1 - RS 2.
			// RS 2 ���������� ������ ���������� C-F ��������� ���������� ������ ������.
			fscanf(fp, "%d", &din);
			my_amg_manager.icoarseningTemp = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.icoarseningSpeed = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.icoarseningPressure = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.icoarseningStress = din;

			// ���� din==0 �� ������ �������������� ������������� ����� ��� ����������� ���������� ��������������� �������,
			// ���� din==1, Stabilization BiCGStab.
			// 8.01.2017 ����� ��� ��� ������ BiCGStab 
			// ����������������� ��������������� ������������� �������.
			// 9.01.2018 ���� din==2, FGMRes ����������������� �������������� ������������� �������.
			fscanf(fp, "%d", &din);
			my_amg_manager.istabilizationTemp = din; // 0 - none
			fscanf(fp, "%d", &din);
			my_amg_manager.istabilizationSpeed = din; // 0 - none
			fscanf(fp, "%d", &din);
			my_amg_manager.istabilizationPressure = din; // 0 - none
			fscanf(fp, "%d", &din);
			my_amg_manager.istabilizationStress = din; // 0 - none
			fscanf(fp, "%d", &din);
			my_amg_manager.ipatch_number = din; // 0 - ���� �� �����������.

			// ������ ���� �� �������.
			fscanf(fp, "%d", &din);
			my_amg_manager.iprint_log_Temperature = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.iprint_log_Speed = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.iprint_log_Pressure = din;
			fscanf(fp, "%d", &din);
			my_amg_manager.iprint_log_Stress = din;

			fscanf(fp, "%d", &din);
			my_amg_manager.lfil = din;

			fscanf(fp, "%d", &din);
			ireconstruction_free_construct_alloc = din;

			fscanf(fp, "%d", &din);
			ianimation_write_on = din;

			// ��������� ����������� ������.
			gtdps = new TEMP_DEP_POWER[ltdp];
			matlist = new TPROP[lmatmax];
			b = new BLOCK[lb];
			s = new SOURCE[ls];
			w = new WALL[lw];
			integer i = 0; // ������� ����� for

			for (i = 0; i < ltdp; i++) {
				// ���������� ��� ������.
				gtdps[i].sname = new char[100]; // ��������� ������
				fscanf(fp, "%s", gtdps[i].sname, 100);
				//printf("%s",gtdps[i].sname);
				//getchar();
				// ���������� ������� � ������.
				my_read_power_table(gtdps[i].sname, gtdps[i].intemp, gtdps[i].inoffset_drain, gtdps[i].rtemp, gtdps[i].roffset_drain, gtdps[i].rpower_table);
				printf("printeger table\n"); // debug
				my_print_table(gtdps[i].intemp, gtdps[i].inoffset_drain, gtdps[i].rtemp, gtdps[i].roffset_drain, gtdps[i].rpower_table);
				printf("Please, press any key to continue...\n");
				//getchar();
				system("pause");

			}

		
			// ���������� ���� ����������
			for (i = 0; i < lmatmax; i++) {
				// �������� ����������:
				// ���������
				fscanf(fp, "%f", &fin);
				matlist[i].rho = fin;
				// ����������� ��� ���������� ��������
				//fscanf(fp, "%f", &fin);
				//matlist[i].cp = fin;
				fscanf(fp, "%d", &din);
				matlist[i].n_cp = din;
				matlist[i].arr_cp = NULL;
				matlist[i].temp_cp = NULL;
				matlist[i].arr_cp = new doublereal[matlist[i].n_cp];
				matlist[i].temp_cp= new doublereal[matlist[i].n_cp];
				if (matlist[i].temp_cp == NULL) {
					printf("problem memory allocation for temp_cp\n");
					system("pause");
					exit(1);
				}
				if (matlist[i].arr_cp == NULL) {
					printf("problem memory allocation for arr_cp\n");
					system("pause");
					exit(1);
				}				
				for (integer i_4 = 0; i_4 < matlist[i].n_cp; i_4++) {
					// ����������� � C.
					fscanf(fp, "%f", &fin);
					matlist[i].temp_cp[i_4] = fin;
					fscanf(fp, "%f", &fin);
					matlist[i].arr_cp[i_4] = fin;
				}
				// ����������������
				//fscanf(fp, "%f", &fin);
				//matlist[i].lam = fin;
				fscanf(fp, "%d", &din);
				matlist[i].n_lam = din;
				matlist[i].arr_lam = NULL;
				matlist[i].temp_lam = NULL;
				matlist[i].arr_lam = new doublereal[matlist[i].n_lam];
				matlist[i].temp_lam = new doublereal[matlist[i].n_lam];
				if (matlist[i].temp_lam == NULL) {
					printf("problem memory allocation for temp_lam\n");
					system("pause");
					exit(1);
				}
				if (matlist[i].arr_lam == NULL) {
					printf("problem memory allocation for arr_lam\n");
					system("pause");
					exit(1);
				}
				for (integer i_4 = 0; i_4 < matlist[i].n_lam; i_4++) {
					// ����������� � C.
					fscanf(fp, "%f", &fin);
					matlist[i].temp_lam[i_4] = fin;
					fscanf(fp, "%f", &fin);
					matlist[i].arr_lam[i_4] = fin;
				}
				// ������������� ���������������� :
				fscanf(fp, "%f", &fin);
				matlist[i].orthotropy_multiplyer_x=fin;
				fscanf(fp, "%f", &fin);
				matlist[i].orthotropy_multiplyer_y=fin;
				fscanf(fp, "%f", &fin);
				matlist[i].orthotropy_multiplyer_z=fin;
				// 5.08.2017.
				// ������������ ��� ������ ���������.
				// ������ ���� � ����������� ��������.
				doublereal Poissonratio = 0.154;
				doublereal Youngmodule = 217.5e9;
				fscanf(fp, "%f", &fin);
				Poissonratio = fin;
				fscanf(fp, "%f", &fin);
				Youngmodule = fin*1e9;
				fscanf(fp, "%f", &fin);
				// TODO beta_t*1E-6
				// ������������ ����.
				doublereal E1_koef = Youngmodule / (1.0- Poissonratio*Poissonratio);
				doublereal nu1_koef = Poissonratio / (1.0- Poissonratio);
				matlist[i].mu_Lame = E1_koef/(2.0*(1.0+ nu1_koef));
				matlist[i].lambda_Lame = (E1_koef*nu1_koef)/(1.0- nu1_koef*nu1_koef);
				// ����������� ������������ ��������
				fscanf(fp, "%f", &fin);
				matlist[i].mu = fin;
				// ����������� ��������� �������������� ����������
				fscanf(fp, "%f", &fin);
				matlist[i].beta_t = fin;
				// ������� �������������� ���������
				fscanf(fp, "%d", &din);
				matlist[i].blibmat = din;
				// ����� ��������� � ����������
				fscanf(fp, "%d", &din);
				matlist[i].ilibident = din;
				
				// ��� ������� �� ������� �����,
				// ���� ������ �� �� ������������� ������� �������
				// ����� ���� ������������ ����������� 
				// ��������-���������� � ��������������� ������� ������������ Tref.
				fscanf(fp, "%d", &din);
				switch (din) {
				case 0: matlist[i].bBussineskApproach = false; break;
				case 1: matlist[i].bBussineskApproach = true; break;
				default: matlist[i].bBussineskApproach = false; break;
				}
				// ����� ������ ��� ����������� ������������ �������� �� ���������� ������
				fscanf(fp, "%d", &din);
				matlist[i].ilawmu = din;
				// ����������� �������� ������������ ��������
				fscanf(fp, "%f", &fin);
				matlist[i].mumin = fin;
				// ������������ �������� ������������ ��������
				fscanf(fp, "%f", &fin);
				matlist[i].mumax = fin;
				// ��������� ��������� ������� ��� ����������� �������� �� ���������� ������
				fscanf(fp, "%f", &fin);
				matlist[i].Amu = fin;
				fscanf(fp, "%f", &fin);
				matlist[i].Bmu = fin;
				fscanf(fp, "%f", &fin);
				matlist[i].Cmu = fin;
				// ���������� �������
				fscanf(fp, "%f", &fin);
				matlist[i].degreennmu = fin;

				// ������ ��������� �������� �� �������
				printf("%e %e %e %e %e\n", matlist[i].rho, matlist[i].cp, matlist[i].lam, matlist[i].mu, matlist[i].beta_t);
				printf("%d %d %d\n", matlist[i].blibmat, matlist[i].ilibident, matlist[i].ilawmu); // bBoussinesq �� ����������
				printf("%e %e %e %e %e %e\n", matlist[i].mumin, matlist[i].mumax, matlist[i].Amu, matlist[i].Bmu, matlist[i].Cmu, matlist[i].degreennmu);
			}

			// ���������� ������
			for (i = 0; i<lb; i++) {

				fscanf(fp, "%d", &din);
				b[i].iunion_id = din; // 0==�������, ����� ���������� �������� �����������.

				// ���������


				fscanf(fp, "%d", &din);
				b[i].g.itypegeom = din;
				fscanf(fp, "%d", &din);
				if (din == 1) {
					b[i].bvisible = true;
				}
				else {
					b[i].bvisible = false;
				}


				fscanf(fp, "%f", &fin);
				b[i].g.xS = scale*fin;
				fscanf(fp, "%f", &fin);
				b[i].g.yS = scale*fin;
				fscanf(fp, "%f", &fin);
				b[i].g.zS = scale*fin;
				fscanf(fp, "%f", &fin);
				b[i].g.xE = scale*fin;
				fscanf(fp, "%f", &fin);
				b[i].g.yE = scale*fin;
				fscanf(fp, "%f", &fin);
				b[i].g.zE = scale*fin;
				// swap
				if (b[i].g.xS>b[i].g.xE) {
					dbuf = b[i].g.xS;
					b[i].g.xS = b[i].g.xE;
					b[i].g.xE = dbuf;
				}
				if (b[i].g.yS > b[i].g.yE) {
					dbuf = b[i].g.yS;
					b[i].g.yS = b[i].g.yE;
					b[i].g.yE = dbuf;
				}
				if (b[i].g.zS > b[i].g.zE) {
					dbuf = b[i].g.zS;
					b[i].g.zS = b[i].g.zE;
					b[i].g.zE = dbuf;
				}


				// Cylinder

				fscanf(fp, "%d", &din);
				b[i].g.iPlane = din;

				fscanf(fp, "%f", &fin);
				b[i].g.xC= scale*fin;
				fscanf(fp, "%f", &fin);
				b[i].g.yC = scale*fin;
				fscanf(fp, "%f", &fin);
				b[i].g.zC = scale*fin;
				fscanf(fp, "%f", &fin);
				b[i].g.Hcyl = scale*fin;
				if (b[i].g.Hcyl < 0.0) {
					// ����������� ������������� ������ ��������.
					switch (b[i].g.iPlane) {
					case XY:
						b[i].g.zC += b[i].g.Hcyl;
						break;
					case XZ:
						b[i].g.yC += b[i].g.Hcyl;
						break;
					case YZ:
						b[i].g.xC += b[i].g.Hcyl;
						break;
					}
					b[i].g.Hcyl = fabs(b[i].g.Hcyl);
				}


				fscanf(fp, "%f", &fin);
				b[i].g.R_out_cyl = scale*fin;
				fscanf(fp, "%f", &fin);
				b[i].g.R_in_cyl = scale*fin;

				// Polygon
				fscanf(fp, "%d", &din);
				b[i].g.iPlane_obj2 = din;
				fscanf(fp, "%d", &din);
				b[i].g.nsizei = din;
				b[i].g.hi = new doublereal[b[i].g.nsizei];
				b[i].g.xi = new doublereal[b[i].g.nsizei];
				b[i].g.yi = new doublereal[b[i].g.nsizei];
				b[i].g.zi = new doublereal[b[i].g.nsizei];
				for (integer i73 = 0; i73 < b[i].g.nsizei; i73++) {
					fscanf(fp, "%f", &fin);
					b[i].g.hi[i73] = scale*fin;
					fscanf(fp, "%f", &fin);
					b[i].g.xi[i73] = scale*fin;
					fscanf(fp, "%f", &fin);
					b[i].g.yi[i73] = scale*fin;
					fscanf(fp, "%f", &fin);
					b[i].g.zi[i73] = scale*fin;
					if (b[i].g.hi[i73] < 0.0) {
						// ����������� ������������� ������ ��������.
						switch (b[i].g.iPlane_obj2) {
						case XY:
							b[i].g.zi[i73] += b[i].g.hi[i73];
							break;
						case XZ:
							b[i].g.yi[i73] += b[i].g.hi[i73];
							break;
						case YZ:
							b[i].g.xi[i73] += b[i].g.hi[i73];
							break;
						}
						b[i].g.hi[i73] = fabs(b[i].g.hi[i73]);
					}
				}
				if ((b[i].g.itypegeom == 2) && (b[i].g.nsizei > 0)) {
					// �� ������� � ����������� ������ ������� �������� ����� �������� 
					// ������ � inmodel_temp ������ ������� ������ ��� ������ ��������.
					doublereal xmin53 = 1.0e30;
					doublereal ymin53 = 1.0e30;
					doublereal zmin53 = 1.0e30;
					doublereal xmax53 = -1.0e30;
					doublereal ymax53 = -1.0e30;
					doublereal zmax53 = -1.0e30;
					for (integer i73 = 0; i73 < b[i].g.nsizei; i73++) {
						if (b[i].g.xi[i73] > xmax53) xmax53 = b[i].g.xi[i73];
						if (b[i].g.yi[i73] > ymax53) ymax53 = b[i].g.yi[i73];
						if (b[i].g.zi[i73] > zmax53) zmax53 = b[i].g.zi[i73];
						if (b[i].g.xi[i73] < xmin53) xmin53 = b[i].g.xi[i73];
						if (b[i].g.yi[i73] < ymin53) ymin53 = b[i].g.yi[i73];
						if (b[i].g.zi[i73] < zmin53) zmin53 = b[i].g.zi[i73];
					}
					switch (b[i].g.iPlane_obj2) {
					case XY:
						b[i].g.xS = xmin53;
						b[i].g.xE = xmax53;
						b[i].g.yS = ymin53;
						b[i].g.yE = ymax53;
						b[i].g.zS = zmin53;
						b[i].g.zE = zmin53 + b[i].g.hi[0];
						break;
					case XZ:
						b[i].g.xS = xmin53;
						b[i].g.xE = xmax53;
						b[i].g.zS = zmin53;
						b[i].g.zE = zmax53;
						b[i].g.yS = ymin53;
						b[i].g.yE = ymin53 + b[i].g.hi[0];
						break;
					case YZ:
						b[i].g.yS = ymin53;
						b[i].g.yE = ymax53;
						b[i].g.zS = zmin53;
						b[i].g.zE = zmax53;
						b[i].g.xS = xmin53;
						b[i].g.xE = xmin53 + b[i].g.hi[0];
						break;
					}
				}
				// ���� �������������� ����������.
				// emissivity
				fscanf(fp, "%f", &fin);
				b[i].radiation.emissW = fin;
				fscanf(fp, "%f", &fin);
				b[i].radiation.emissE = fin;
				fscanf(fp, "%f", &fin);
				b[i].radiation.emissS = fin;
				fscanf(fp, "%f", &fin);
				b[i].radiation.emissN = fin;
				fscanf(fp, "%f", &fin);
				b[i].radiation.emissB = fin;
				fscanf(fp, "%f", &fin);
				b[i].radiation.emissT = fin;
				fscanf(fp, "%d", &din);
				if (din==0) {
					// ���� �� �������� ��������� �����������.
					b[i].radiation.binternalRadiation = false;
				}
				else {
					// ���� �������� ��������� �����������.
					b[i].radiation.binternalRadiation = true;
					if (bvacuumPrism) {
						bdouble_vacuum_PRISM = true;
					}
					bvacuumPrism=true;
					// ��������� View Factors.
					calculate_view_factors(b[i]);
				}
				b[i].radiation.nodelistW = NULL;
				b[i].radiation.nodelistE = NULL;
				b[i].radiation.nodelistS = NULL;
				b[i].radiation.nodelistN = NULL;
				b[i].radiation.nodelistB = NULL;
				b[i].radiation.nodelistT = NULL;
				b[i].radiation.nodelistWsize = 0;
				b[i].radiation.nodelistEsize = 0;
				b[i].radiation.nodelistSsize = 0;
				b[i].radiation.nodelistNsize = 0;
				b[i].radiation.nodelistBsize = 0;
				b[i].radiation.nodelistTsize = 0;

				// ������������� ��������� � ���� ����������
				fscanf(fp, "%d", &din);
				b[i].imatid = din;

				fscanf(fp, "%d", &din);

				// bCylinderFixed
				if (din == 1) {
					b[i].CylinderFixed = true;
				}
				else {
					b[i].CylinderFixed = false;
				}

				// �������� ��������������
				//fscanf(fp, "%f", &fin);
				//b[i].Sc = fin;
				// 19 november 2016 ������������ ��������� �������� ��������������.
				fscanf(fp, "%d", &din);
				b[i].n_Sc = din;
				b[i].arr_Sc = NULL;
				b[i].temp_Sc = NULL;
				b[i].arr_Sc = new doublereal[b[i].n_Sc];
				b[i].temp_Sc = new doublereal[b[i].n_Sc];
				if (b[i].temp_Sc == NULL) {
					printf("problem memory allocation for temp_Sc\n");
					system("pause");
					exit(1);
				}
				if (b[i].arr_Sc == NULL) {
					printf("problem memory allocation for arr_Sc\n");
					system("pause");
					exit(1);
				}
				for (integer i_4 = 0; i_4 < b[i].n_Sc; i_4++) {
					// ����������� � C.
					fscanf(fp, "%f", &fin);
					b[i].temp_Sc[i_4] = fin;
					fscanf(fp, "%f", &fin);
					if (fin != fin) {
						b[i].arr_Sc[i_4] = 0.0;
					}
					else {
						b[i].arr_Sc[i_4] = fin;
					}
				}

                // ����� ����������� �������� �������������� � ����� �� �������.
				fscanf(fp, "%d", &din);
				b[i].ipower_time_depend=din;
				// ��� �����
				fscanf(fp, "%d", &din);
				b[i].itype = din;

				// ������ ��������� �������� �� �������
				printf("%e %e %e %e %e %e\n", b[i].g.xS, b[i].g.yS, b[i].g.zS, b[i].g.xE, b[i].g.yE, b[i].g.zE);
				printf("%d %d %d\n", b[i].imatid,  b[i].itype, b[i].ipower_time_depend);
				printf("temperature depend power\n");
				printf("t_C power_W\n");
				for (integer i_54 = 0; i_54 < b[i].n_Sc; i_54++) {
					printf("%e %e\n", b[i].temp_Sc[i_54], b[i].arr_Sc[i_54]);
				}
			}

			// ���������� ���������� �����
			for (i = 0; i < ls; i++) {

				fscanf(fp, "%d", &din);
				s[i].iunion_id = din; // 0==�������, ����� ���������� �������� �����������.

				fscanf(fp, "%f", &fin);
				s[i].power = fin;
				fscanf(fp, "%d", &din);
				if (din == 0) {
					// const
					// ������� ���������� �������� ������������ � ����� ��������.
					s[i].power_multiplyer = 1.0;
					s[i].bgarber_depend = false;
				}
				else if (din == 1) {
					// ������������ � ����� �������� ������� ��������
					// � ����������� �� ������������ ����������� � �������� 
					// �������� �������� �����.
					s[i].bgarber_depend = true;
					s[i].power_multiplyer = s[i].power;
					// �������� ����� ��������� ��� ambient Temperature ����������� ��������� �����.
				}
				fscanf(fp, "%d", &din);
				s[i].igarber_depend = din; // ���������� ����� �������.
				fscanf(fp, "%f", &fin);
				s[i].roperation_offset_drain = fin; // ������� �������� �������� �����.
													//printf("offset drain is %e\n",s[i].roperation_offset_drain);
													//getchar();
				bool bsplinereadOk = true;
				if (s[i].bgarber_depend) {
					s[i].power = my_splain_interpol_power_table(gtdps[s[i].igarber_depend].intemp,
						gtdps[s[i].igarber_depend].inoffset_drain,
						gtdps[s[i].igarber_depend].rtemp,
						gtdps[s[i].igarber_depend].roffset_drain,
						gtdps[s[i].igarber_depend].rpower_table,
						operatingtemperature,
						s[i].roperation_offset_drain);
					if (bsplinereadOk) {
						// ��������� �������� �������� ���������� �������������.
						printf("single test validation spline approximation...\n");
						printf("calculate initial power=%e\n", s[i].power);
						printf("please, press any key to continue...");
						// getchar();
						system("pause");

						bsplinereadOk = false;
					}
					s[i].power *= s[i].power_multiplyer; // ���������� �� �������������� ���������.
				}



				fscanf(fp, "%d", &din);
				s[i].iPlane = din;
				// ���������
				fscanf(fp, "%f", &fin);
				s[i].g.xS = scale*fin;
				fscanf(fp, "%f", &fin);
				s[i].g.yS = scale*fin;
				fscanf(fp, "%f", &fin);
				s[i].g.zS = scale*fin;
				fscanf(fp, "%f", &fin);
				s[i].g.xE = scale*fin;
				fscanf(fp, "%f", &fin);
				s[i].g.yE = scale*fin;
				fscanf(fp, "%f", &fin);
				s[i].g.zE = scale*fin;



				// swap
				if (s[i].g.xS > s[i].g.xE) {
					dbuf = s[i].g.xS;
					s[i].g.xS = s[i].g.xE;
					s[i].g.xE = dbuf;
				}
				if (s[i].g.yS > s[i].g.yE) {
					dbuf = s[i].g.yS;
					s[i].g.yS = s[i].g.yE;
					s[i].g.yE = dbuf;
				}
				if (s[i].g.zS > s[i].g.zE) {
					dbuf = s[i].g.zS;
					s[i].g.zS = s[i].g.zE;
					s[i].g.zE = dbuf;
				}
				switch (s[i].iPlane) {
				case XY: s[i].square = fabs(s[i].g.xE - s[i].g.xS)*fabs(s[i].g.yE - s[i].g.yS); break;
				case XZ: s[i].square = fabs(s[i].g.xE - s[i].g.xS)*fabs(s[i].g.zE - s[i].g.zS); break;
				case YZ: s[i].square = fabs(s[i].g.yE - s[i].g.yS)*fabs(s[i].g.zE - s[i].g.zS); break;
				default: break;
				}
				printf("%e %d %e %e %e %e %e %e %e\n", s[i].power, s[i].iPlane, s[i].g.xS, s[i].g.yS, s[i].g.zS, s[i].g.xE, s[i].g.yE, s[i].g.zE, s[i].square);
			}

			// ���������� ������ ������



			for (i = 0; i < lw; i++) {

				fscanf(fp, "%d", &din);
				w[i].iunion_id = din; // 0==�������, ����� ���������� �������� �����������.

				fscanf(fp, "%d", &din);
				w[i].ifamily = din;
				switch (din) {
				case 1:  fscanf(fp, "%f", &fin);
					w[i].Tamb = fin;
					fscanf(fp, "%f", &fin); // Stefan Bolcman
					// termostability wall
					w[i].emissivity=0.0;
					w[i].film_coefficient=0.0;
					fscanf(fp, "%f", &fin);
					w[i].hf = 0.0;
					break; // ������� ����
				case 2:  fscanf(fp, "%f", &fin);
					w[i].Tamb = 0.0;
					fscanf(fp, "%f", &fin); // Stefan Bolcman
					// adiabatic wall
					w[i].emissivity=0.0;
					w[i].film_coefficient=0.0;
					fscanf(fp, "%f", &fin);
					w[i].hf = 0.0;
					break; // ���������� ������� �������
				case 3:  fscanf(fp, "%f", &fin);
					w[i].Tamb = fin;
					fscanf(fp, "%f", &fin); // Stefan Bolcman
					// Newton-Richman condition, film coefficient.
					w[i].emissivity=0.0;
					w[i].film_coefficient=fin;
					fscanf(fp, "%f", &fin);
					w[i].hf = 0.0;
					break; // ������-������.
				case 4:  fscanf(fp, "%f", &fin);
					w[i].Tamb = fin;
					fscanf(fp, "%f", &fin); // Stefan Bolcman
					// Stefan - Bolcman condition
					w[i].emissivity=fin;
					w[i].film_coefficient=0.0;
					fscanf(fp, "%f", &fin);
					w[i].hf = 0.0;
					break; // ������-��������.
				default: break;
				}
				fscanf(fp, "%d", &din);
				if (din == 1) w[i].bsymmetry = true; else w[i].bsymmetry = false;
				fscanf(fp, "%d", &din);
				if (din == 1) w[i].bpressure = true; else w[i].bpressure = false;
				fscanf(fp, "%d", &din);
				if (din == 1) w[i].bopening = true; else w[i].bopening = false;

				fscanf(fp, "%f", &fin);
				w[i].Vx = fin;
				fscanf(fp, "%f", &fin);
				w[i].Vy = fin;
				fscanf(fp, "%f", &fin);
				w[i].Vz = fin;
				fscanf(fp, "%f", &fin);
				w[i].P = fin;
				fscanf(fp, "%d", &din);
				w[i].ithermal_Stress_boundary_condition = din;
				fscanf(fp, "%f", &fin);
				w[i].xForce = fin;
				fscanf(fp, "%f", &fin);
				w[i].yForce = fin;
				fscanf(fp, "%f", &fin);
				w[i].zForce = fin;
				fscanf(fp, "%d", &din);
				w[i].iPlane = din;
				// ���������
				fscanf(fp, "%f", &fin);
				w[i].g.xS = scale*fin;
				fscanf(fp, "%f", &fin);
				w[i].g.yS = scale*fin;
				fscanf(fp, "%f", &fin);
				w[i].g.zS = scale*fin;
				fscanf(fp, "%f", &fin);
				w[i].g.xE = scale*fin;
				fscanf(fp, "%f", &fin);
				w[i].g.yE = scale*fin;
				fscanf(fp, "%f", &fin);
				w[i].g.zE = scale*fin;
				// swap
				if (w[i].g.xS > w[i].g.xE) {
					dbuf = w[i].g.xS;
					w[i].g.xS = w[i].g.xE;
					w[i].g.xE = dbuf;
				}
				if (w[i].g.yS > w[i].g.yE) {
					dbuf = w[i].g.yS;
					w[i].g.yS = w[i].g.yE;
					w[i].g.yE = dbuf;
				}
				if (w[i].g.zS > w[i].g.zE) {
					dbuf = w[i].g.zS;
					w[i].g.zS = w[i].g.zE;
					w[i].g.zE = dbuf;
				}
				w[i].bfixboundary = false;// ��������� �������.
				printf("%d %e %e %d %e %e %e %e %e %e\n", w[i].ifamily, w[i].Tamb, w[i].hf, w[i].iPlane, w[i].g.xS, w[i].g.yS, w[i].g.zS, w[i].g.xE, w[i].g.yE, w[i].g.zE);
			}


			// ���������.
			fscanf(fp, "%d", &din);
			lu = din;
			if (lu == 0) {
				my_union = NULL;
			}
			else {
				my_union = new UNION[lu];
				// �������������.
				for (i = 0; i < lu; i++) {
					my_union[i].f = NULL;
					my_union[i].xpos = NULL;
					my_union[i].ypos = NULL;
					my_union[i].zpos = NULL;
					my_union[i].xposadd = NULL;
					my_union[i].yposadd = NULL;
					my_union[i].zposadd = NULL;
					my_union[i].iswitchMeshGenerator = 2; // 2 - CoarseMeshGen
					my_union[i].inxadd = -1;
					my_union[i].inyadd = -1;
					my_union[i].inzadd = -1;
					my_union[i].flow_interior = 0;
				}
			}
			for (i = 0; i < lu; i++) {
				fscanf(fp, "%f", &fin);
				my_union[i].xS = scale*fin;
				fscanf(fp, "%f", &fin);
				my_union[i].xE = scale*fin;
				fscanf(fp, "%f", &fin);
				my_union[i].yS = scale*fin;
				fscanf(fp, "%f", &fin);
				my_union[i].yE = scale*fin;
				fscanf(fp, "%f", &fin);
				my_union[i].zS = scale*fin;
				fscanf(fp, "%f", &fin);
				my_union[i].zE = scale*fin;

				fscanf(fp, "%d", &din);
				my_union[i].id = din; // ���������� ������������� ����������.
				fscanf(fp, "%d", &din);
				my_union[i].inx = din;
				fscanf(fp, "%d", &din);
				my_union[i].iny = din;
				fscanf(fp, "%d", &din);
				my_union[i].inz = din;

			}

			// ���������� ���������� � ������ �������� ���������
			fscanf(fp, "%d", &din);
			eqin.itemper = din;
			fscanf(fp, "%d", &din);
			eqin.imaxflD = din;
			if (eqin.imaxflD == 0) {
				eqin.fluidinfo = NULL;
			}
			else
			{
				// ��������� ����������� ������
				if (eqin.fluidinfo != NULL) {
					delete eqin.fluidinfo;
					eqin.fluidinfo = NULL;
				}
				eqin.fluidinfo = new FLOWINFO[eqin.imaxflD];
				for (i = 0; i < eqin.imaxflD; i++) {
					// ���������� ��������� ������� �����
					fscanf(fp, "%f", &fin);
					eqin.fluidinfo[i].xc = scale*fin;
					fscanf(fp, "%f", &fin);
					eqin.fluidinfo[i].yc = scale*fin;
					fscanf(fp, "%f", &fin);
					eqin.fluidinfo[i].zc = scale*fin;
					fscanf(fp, "%d", &din);
					eqin.fluidinfo[i].iflow = din;
					fscanf(fp, "%d", &din);
					eqin.fluidinfo[i].iflowregime = din;
					fscanf(fp, "%d", &din);
					eqin.fluidinfo[i].iturbmodel = din;
					fscanf(fp, "%f", &fin);
					eqin.fluidinfo[i].Cs = fin; // ���������� �������������.
					fscanf(fp, "%d", &din);
					// ���� ������������� ����������� �������� ���������� �������������.
					// �������� Dynamic Subgrid Scale Model ������� 1991 ����.
					if (din == 1) {
						eqin.fluidinfo[i].bDynamic_Stress = true;
					}
					else {
						eqin.fluidinfo[i].bDynamic_Stress = false;
					}
					fscanf(fp, "%d", &din);
					// �������� ����������� ������ � ����� �� ��������� �������� ���������� �������������.
					if (din == 1) {
						eqin.fluidinfo[i].bLimiters_Cs = true;
					}
					else {
						eqin.fluidinfo[i].bLimiters_Cs = false;
					}
					fscanf(fp, "%f", &fin);
					eqin.fluidinfo[i].rminCs = fin; // ����������� ��������� �������� ���������� �������������.
					fscanf(fp, "%f", &fin);
					eqin.fluidinfo[i].rmaxCs = fin; // ������������ ��������� �������� ���������� �������������.
					fscanf(fp, "%d", &din);
					eqin.fluidinfo[i].itypeFiltrGermano = din; // ��� ������� � ������ ������� 1991 ����.
					fscanf(fp, "%f", &fin);
					eqin.fluidinfo[i].roughness = 1.0e-6*fin; // ������������� ������ � �.
					fscanf(fp, "%f", &fin);
					eqin.fluidinfo[i].rRi_mult = fin; // ��������� �������������� ������������ ����� ����������.
					fscanf(fp, "%f", &fin);
					eqin.fluidinfo[i].rSelectiveAngle = fin; // ��������� �������� ���� � ������ Selective Smagorinsky.
					fscanf(fp, "%d", &din);
					eqin.fluidinfo[i].ipowerroughness = din; // ���������� ������� � ������ ����� �������������.
					fscanf(fp, "%d", &din);
					eqin.fluidinfo[i].itypeSelectiveSmagorinsky_filtr = din; // ��� ������� � ������ Selective Smagorinsky.
					fscanf(fp, "%d", &din);
					eqin.fluidinfo[i].bfdelta = din; // ���� ��������������� �����.
					fscanf(fp, "%d", &din);
					eqin.fluidinfo[i].bSmagorinskyLilly = din; // ������ �������������-�����.
					fscanf(fp, "%d", &din);
					eqin.fluidinfo[i].bsurface_roughness = din; // ���� ������������� ������.
					fscanf(fp, "%d", &din);
					// ���� ������� � ��������� ����� ����.
					if (din == 1) {
						eqin.fluidinfo[i].bSwirlAmendment = true;
					}
					else {
						eqin.fluidinfo[i].bSwirlAmendment = false;
					}
					fscanf(fp, "%d", &din);
					// ���� ��������������� � ������ �������������
					if (din == 1) {
						eqin.fluidinfo[i].bSelectiveSmagorinsky = true;
					}
					else {
						eqin.fluidinfo[i].bSelectiveSmagorinsky = false;
					}

				}
			}

			fclose(fp); // �������� �����
		}
	
	}
#endif
#if doubleintprecision == 0

    // eqin - ���������� � ������ �������� ���������.

    // dgx, dgy, dgz - ������ ���� �������.
    // inx, iny, inz - ���������� ����� �� ������ �� ����.

    FILE *fp;
	errno_t err1;
	err1 = fopen_s(&fp, fname, "r");

	if ((err1)!=0) {
		printf("No input File premeshin.txt \n");
		//getchar();
		system("pause");

	}
	else
	{
		if (fp != NULL) {
			float fin = 0.0;
			integer din = 0;
			doublereal scale = 1.0;
			doublereal dbuf; // ��� �������������� � ������� �����������



			fscanf_s(fp, "%d", &din);
			ionly_solid_visible = din;
			printf("ionly_solid_visible =%d\n", ionly_solid_visible);
			fscanf_s(fp, "%f", &fin);
			scale = fin;
			fscanf_s(fp, "%d", &din);
			lmatmax = din;
			fscanf_s(fp, "%d", &din);
			lb = din;
			fscanf_s(fp, "%d", &din);
			ls = din;
			fscanf_s(fp, "%d", &din);
			lw = din;
			fscanf_s(fp, "%d", &din);
			ltdp = din; // ���������� ���������� ������ � ���������� ������� �� ����������� ����������� �������� �� �����������.
			

			// ��������� �������� ������� ���� �������:
			fscanf_s(fp, "%f", &fin);
			dgx = fin;
			fscanf_s(fp, "%f", &fin);
			dgy = fin;
			fscanf_s(fp, "%f", &fin);
			dgz = fin;

			// ��������� ���������� ����� �� ������ ������������ ���
			fscanf_s(fp, "%d", &din);
			inx = din;
			fscanf_s(fp, "%d", &din);
			iny = din;
			fscanf_s(fp, "%d", &din);
			inz = din;

			fscanf_s(fp, "%f", &fin);
			operatingtemperature = fin; // Operating Temperature
			operating_temperature_for_film_coeff = fin;

			// ������������� ��������� �������� ����������.
			// ������ �������� ��� ���� ��������� �������.
			// initialization value.
			fscanf_s(fp, "%f", &fin);
			starting_speed_Vx = fin;
			fscanf_s(fp, "%f", &fin);
			starting_speed_Vy = fin;
			fscanf_s(fp, "%f", &fin);
			starting_speed_Vz = fin;


			// ��������� ���������� ������� ����� ����� ������� �������� ���������������� ����� Variation Plot.
			fscanf_s(fp, "%f", &fin);
			Tochka_position_X0_for_XY_Plot = scale*fin;
			fscanf_s(fp, "%f", &fin);
			Tochka_position_Y0_for_XY_Plot = scale*fin;
			fscanf_s(fp, "%f", &fin);
			Tochka_position_Z0_for_XY_Plot = scale*fin;
			// ����������� ����� ��������� � ������������ 
			// ����� �� ���� ���������� ������������� ������� ���������:
			// 0 - Ox, 1 - Oy, 2 - Oz.
			fscanf_s(fp, "%d", &din);
			idirectional_for_XY_Plot = din;

			fscanf_s(fp, "%f", &fin);
			etalon_max_size_ratio = fin; // ����������� ��������� �����.

			fscanf_s(fp, "%f", &fin);
			etalon_max_size_ratio2 = fin; // �������� �������� ��������� ����� �� ������ FlowVision.

			//printf("etalon_max_size_ratio=%e etalon_max_size_ratio2=%e\n", etalon_max_size_ratio, etalon_max_size_ratio2);
			//getchar();

			fscanf_s(fp, "%d", &din);
			switch (din) {
			case 0: bsnap_TO_global = false;  break;
			default: bsnap_TO_global = true;  break;
			}

			fscanf_s(fp, "%d", &din);
			iswitchsolveramg_vs_BiCGstab_plus_ILU2 = din; // ����� ��������� ���������� : ���� amg1r5 ���� BiCGStab+ILU2.

			fscanf_s(fp, "%d", &din);
			iswitchsolveramg_vs_BiCGstab_plus_ILU6 = din; // ����� ��������� ���������� : ���� �����0.14 ���� BiCGStab+ILU6.

			fscanf_s(fp, "%d", &din);
			if (din == 1) {
				// SIMPLEC algorithm.
				iSIMPLE_alg = SIMPLEC_Van_Doormal_and_Raithby;
			}
			else {
				// SIMPLE algorithm 1972.
				iSIMPLE_alg = SIMPLE_Carretto;
			}

			fscanf_s(fp, "%d", &din);
			// ����� ����� ��� ������ ��������.
			// �������� ��� ����������� ������ ��������� ��������������� 
			// ������������ � ����� my_approx_convective2.c
			switch (din) {
			case 1: iFLOWScheme = UNEVEN_MUSCL; break; // MUSCL 2
			case 2: iFLOWScheme = UNEVEN_SOUCUP; break; // SOUCUP [MINMOD] 2
			case 3: iFLOWScheme = UNEVEN_HLPA; break; // HLPA 2
			case 4: iFLOWScheme = UNEVEN_SMART; break; // SMART 3
			case 5: iFLOWScheme = UNEVEN_WACEB; break; // WACEB 3 TVD
			case 6: iFLOWScheme = UNEVEN_SMARTER; break; // SMARTER 3
			case 7: iFLOWScheme = UNEVEN_LPPA; break; // LPPA 3
			case 8: iFLOWScheme = UNEVEN_VONOS; break; // VONOS 3
			case 9: iFLOWScheme = UNEVEN_STOIC; break; // STOIC
			case 10: iFLOWScheme = UNEVEN_CLAM; break; // CLAM
			case 11: iFLOWScheme = UNEVEN_OSHER; break; // OSHER
			case 12: iFLOWScheme = UNEVEN_EXPONENTIAL; break; // EXPONENTIAL
			case 13: iFLOWScheme = UNEVEN_SUPER_C; break; // SUPER_C
			case 14: iFLOWScheme = UNEVEN_ISNAS; break; // ISNAS
			case 15: iFLOWScheme = UNEVEN_CUBISTA; break; // CUBISTA
			default: iFLOWScheme = 2; break; // UDS ����� ���������� �����.
			}

			fscanf_s(fp, "%d", &din);
			// ����� ����� ��� ����������� � ������ ��������.
			// �������� ��� ����������� ������ ��������� ��������������� 
			// ������������ � ����� my_approx_convective2.c
			switch (din) {
			case 1: iTEMPScheme = UNEVEN_MUSCL; break; // MUSCL 2
			case 2: iTEMPScheme = UNEVEN_SOUCUP; break; // SOUCUP [MINMOD] 2
			case 3: iTEMPScheme = UNEVEN_HLPA; break; // HLPA 2
			case 4: iTEMPScheme = UNEVEN_SMART; break; // SMART 3
			case 5: iTEMPScheme = UNEVEN_WACEB; break; // WACEB 3 TVD
			case 6: iTEMPScheme = UNEVEN_SMARTER; break; // SMARTER 3
			case 7: iTEMPScheme = UNEVEN_LPPA; break; // LPPA 3
			case 8: iTEMPScheme = UNEVEN_VONOS; break; // VONOS 3
			case 9: iTEMPScheme = UNEVEN_STOIC; break; // STOIC
			case 10: iTEMPScheme = UNEVEN_CLAM; break; // CLAM
			case 11: iTEMPScheme = UNEVEN_OSHER; break; // OSHER
			case 12: iTEMPScheme = UNEVEN_EXPONENTIAL; break; // EXPONENTIAL
			case 13: iTEMPScheme = UNEVEN_SUPER_C; break; // SUPER_C
			case 14: iTEMPScheme = UNEVEN_ISNAS; break; // ISNAS
			case 15: iTEMPScheme = UNEVEN_CUBISTA; break; // CUBISTA
			default: iTEMPScheme = 2; break; // UDS ����� ���������� �����.
			}



			// ����� ��������� ����������.
			fscanf_s(fp, "%d", &din);
			iswitchMeshGenerator = din;

			fscanf_s(fp, "%d", &din);
			steady_or_unsteady_global_determinant = 2;
			if ((din == 0) || (din == 1) || (din == 2) || (din == 3) || (din == 5) || (din == 6)) {
				// 2 - mesh generator only.
				// 3 - fluid dynamic.
				// 5 - Static Structural.
				// 6 - Thermal Stress.
				steady_or_unsteady_global_determinant = din; // steady - 0, or unsteady -1 calculation.
			}
			else {
				printf("error input parametr steady or unsteady calculation\n");
				system("PAUSE");
				exit(1);
			}

			fscanf_s(fp, "%d", &din);
			if ((din == 0) || (din == 1) || (din == 2) || (din == 3)) {
				glTSL.id_law = din;
			}
			else {
				printf("error input parametr timestep law\n");
				system("PAUSE");
				exit(1);
			}
			fscanf_s(fp, "%f", &fin);
			if ((fin <= 0.0) || (fin >= 1.0)) {
				printf("error input parametr timestep law Factor a\n");
				system("PAUSE");
				exit(1);
			}
			glTSL.Factor_a_for_Linear = fin; // Factor_a
			fscanf_s(fp, "%f", &fin);
			if (fin <= 0.0) {
				printf("error input parametr timestep law tau must be strongly positive\n");
				system("PAUSE");
				exit(1);
			}
			glTSL.tau = fin; // ������������ ��������.
			fscanf_s(fp, "%f", &fin);
			glTSL.Q = fin;  // ����������.
            // ��������� ����������� ������ ��� ���� �������.
			fscanf_s(fp, "%f", &fin);
			if ((fin <= 0.0) || (fin >= 1.0)) {
				printf("error input parametr timestep law APPARAT multiplyer\n");
				system("PAUSE");
				exit(1);
			}
			glTSL.m1 = fin;
			fscanf_s(fp, "%f", &fin);
			if (fin <= 0.0) {
				printf("error input parametr timestep law APPARAT tau1 must be strongly positive\n");
				system("PAUSE");
				exit(1);
			}
			glTSL.tau1 = fin;
			fscanf_s(fp, "%f", &fin);
			if (fin <= 0.0) {
				printf("error input parametr timestep law APPARAT tau2 must be strongly positive\n");
				system("PAUSE");
				exit(1);
			}
			glTSL.tau2 = fin;
			fscanf_s(fp, "%f", &fin);
			if (fin <= 0.0) {
				printf("error input parametr timestep law APPARAT tau_pause must be strongly positive\n");
				system("PAUSE");
				exit(1);
			}
			glTSL.tau_pause = fin;
			fscanf_s(fp, "%d", &din);
			glTSL.n_cycle = din;
			fscanf_s(fp, "%f", &fin);
			if (fin <= 0.0) {
				printf("error input parametr timestep law APPARAT Period must be strongly positive\n");
				system("PAUSE");
				exit(1);
			}
			glTSL.T_all = fin;
			doublereal t_pause_gl = glTSL.T_all - glTSL.n_cycle*(2 * glTSL.tau1 + glTSL.tau2 + glTSL.tau_pause);
			if (t_pause_gl <= 0.0) {
				printf("error in parameters Square Wave APPARAT time step law.\n");
				//getchar();
				system("pause");
				exit(1);
			}

			fscanf_s(fp, "%f", &fin);
			if (fin <= 0.0) {
				printf("error input parametr on_time_double_linear law hot cold reshime must be strongly positive\n");
				system("PAUSE");
				exit(1);
			}
			glTSL.on_time_double_linear = fin;

			// ����� ��������� ��������������� ������������� ��� ������� ������������� � ������ ����.
			fscanf_s(fp, "%f", &fin);
			globalEndTimeUnsteadyTemperatureCalculation = fin;




			// Newton-Richman condition.
			fscanf_s(fp, "%d", &din);
			adiabatic_vs_heat_transfer_coeff = din; // 0 - adiabatic wall, 1 - Newton Richman condition, 2 - Stefan Bolcman condition, 3 - mix condition.
			fscanf_s(fp, "%f", &fin);
			film_coefficient = fin;
			// A��� �����
			fscanf_s(fp, "%d", &din);
			if (din == 0) {
				// ������� ����������������� �����.
				b_on_adaptive_local_refinement_mesh = false;
			}
			else {
				// ����
				b_on_adaptive_local_refinement_mesh = true;
			}
			fscanf_s(fp, "%d", &din);
			itype_ALICE_Mesh = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.m_restart = din;
			// classical algebraic multigrid parameters:
			// only for my_agregat_amg.cu.
			// only for my_agregat_amg.cu.
			fscanf_s(fp, "%d", &din);
			my_amg_manager.imySortAlgorithm = din;
			fscanf_s(fp, "%d", &din);
			//my_amg_manager.maximum_levels = din;
			my_amg_manager.maximum_delete_levels_Temperature = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.maximum_delete_levels_Speed = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.maximum_delete_levels_Pressure = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.maximum_delete_levels_Stress = din;

			// type interpolation procedure :
			//fscanf_s(fp, "%d", &din);
			//my_amg_manager.number_interpolation_procedure = din;

			fscanf_s(fp, "%d", &din);
			my_amg_manager.number_interpolation_procedure_Temperature = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.number_interpolation_procedure_Speed = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.number_interpolation_procedure_Pressure = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.number_interpolation_procedure_Stress = din;

			fscanf_s(fp, "%d", &din);
			my_amg_manager.iCFalgorithm_and_data_structure_Temperature = din;// 3-Treap.
			fscanf_s(fp, "%d", &din);
			my_amg_manager.iCFalgorithm_and_data_structure_Speed = din;// 3-Treap.
			fscanf_s(fp, "%d", &din);
			my_amg_manager.iCFalgorithm_and_data_structure_Pressure = din;// 3-Treap.
			fscanf_s(fp, "%d", &din);
			my_amg_manager.iCFalgorithm_and_data_structure_Stress = din;// 3-Treap.

			fscanf_s(fp, "%d", &din);
			my_amg_manager.itypemodifyinterpol = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.inumberadaptpass = din;


			// 23.02.2018
			// print matrix portrait
			fscanf(fp, "%d", &din);
			my_amg_manager.bTemperatureMatrixPortrait = din; // 0 - NO_PRINT, 1 - PRINT.
			fscanf(fp, "%d", &din);
			my_amg_manager.bSpeedMatrixPortrait = din; // 0 - NO_PRINT, 1 - PRINT.
			fscanf(fp, "%d", &din);
			my_amg_manager.bPressureMatrixPortrait = din; // 0 - NO_PRINT, 1 - PRINT.
			fscanf(fp, "%d", &din);
			my_amg_manager.bStressMatrixPortrait = din; // 0 - NO_PRINT, 1 - PRINT.

			// 01.05.2017
			// truncation of interpolation:
			fscanf_s(fp, "%d", &din);
			my_amg_manager.itruncation_interpolation_Temperature = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.itruncation_interpolation_Speed = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.itruncation_interpolation_Pressure = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.itruncation_interpolation_Stress = din;
			fscanf_s(fp, "%f", &fin);
			my_amg_manager.truncation_interpolation_Temperature = fin;
			fscanf_s(fp, "%f", &fin);
			my_amg_manager.truncation_interpolation_Speed = fin;
			fscanf_s(fp, "%f", &fin);
			my_amg_manager.truncation_interpolation_Pressure = fin;
			fscanf_s(fp, "%f", &fin);
			my_amg_manager.truncation_interpolation_Stress = fin;

			// number nFinnest sweeps :
			fscanf_s(fp, "%d", &din);
			my_amg_manager.nFinnest_Temperature = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.nFinnest_Speed = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.nFinnest_Pressure = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.nFinnest_Stress = din;

			// number presweeps:
			fscanf_s(fp, "%d", &din);
			my_amg_manager.nu1_Temperature = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.nu1_Speed = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.nu1_Pressure = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.nu1_Stress = din;

			// number postsweeps :
			fscanf_s(fp, "%d", &din);
			my_amg_manager.nu2_Temperature = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.nu2_Speed = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.nu2_Pressure = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.nu2_Stress = din;

			// memory size :
			fscanf_s(fp, "%d", &din);
			my_amg_manager.memory_size_Temperature = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.memory_size_Speed = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.memory_size_Pressure = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.memory_size_Stress = din;


			// �������� ������� ���������� � ������������.
			fscanf_s(fp, "%f", &fin);
			my_amg_manager.gold_const_Temperature = fin;
			fscanf_s(fp, "%f", &fin);
			my_amg_manager.gold_const_Speed = fin;
			fscanf_s(fp, "%f", &fin);
			my_amg_manager.gold_const_Pressure = fin;
			fscanf_s(fp, "%f", &fin);
			my_amg_manager.gold_const_Stress = fin;

			// ������������ �� ilu2 smoother.
			// ������������ �� ilu2 smoother.
			fscanf_s(fp, "%d", &din);
			if (din == 3) {
				din = 0;
				my_amg_manager.ilu2_smoother_Temperature = din;
				my_amg_manager.b_gmresTemp = true;
			}
			else {
				my_amg_manager.ilu2_smoother_Temperature = din;
			}
			fscanf_s(fp, "%d", &din);
			if (din == 3) {
				din = 0;
				my_amg_manager.ilu2_smoother_Speed = din;
				my_amg_manager.b_gmresSpeed = true;
			}
			else {
				my_amg_manager.ilu2_smoother_Speed = din;
			}
			fscanf_s(fp, "%d", &din);
			if (din == 3) {
				din = 0;
				my_amg_manager.ilu2_smoother_Pressure = din;
				my_amg_manager.b_gmresPressure = true;
			}
			else {
				my_amg_manager.ilu2_smoother_Pressure = din;
			}
			fscanf_s(fp, "%d", &din);
			if (din == 3) {
				din = 0;
				my_amg_manager.ilu2_smoother_Stress = din;
				my_amg_manager.b_gmresStress = true;
			}
			else {
				my_amg_manager.ilu2_smoother_Stress = din;
			}


			// strength threshold :
			fscanf_s(fp, "%f", &fin);
			my_amg_manager.theta_Temperature = fin;
			fscanf_s(fp, "%f", &fin);
			my_amg_manager.theta_Speed = fin;
			fscanf_s(fp, "%f", &fin);
			my_amg_manager.theta_Pressure = fin;
			fscanf_s(fp, "%f", &fin);
			my_amg_manager.theta_Stress = fin;

			// magic threshold :
			//fscanf_s(fp, "%f", &fin);
			//my_amg_manager.magic = fin;

			// magic <=> F_to_F
			fscanf_s(fp, "%f", &fin);
			my_amg_manager.F_to_F_Temperature = fin;
			fscanf_s(fp, "%f", &fin);
			my_amg_manager.F_to_F_Speed = fin;
			fscanf_s(fp, "%f", &fin);
			my_amg_manager.F_to_F_Pressure = fin;
			fscanf_s(fp, "%f", &fin);
			my_amg_manager.F_to_F_Stress = fin;

			// AMG Splitting (coarsening)
			// ������ ���������� C-F ��������� : 0 - standart, 1 - RS 2.
			// RS 2 ���������� ������ ���������� C-F ��������� ���������� ������ ������.
			fscanf_s(fp, "%d", &din);
			my_amg_manager.icoarseningTemp = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.icoarseningSpeed = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.icoarseningPressure = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.icoarseningStress = din;

			// ���� din==0 �� ������ �������������� ������������� ����� ��� ����������� ���������� ��������������� �������,
			// ���� din==1, Stabilization BiCGStab.
			// 8.01.2017 ����� ��� ��� ������ BiCGStab 
			// ����������������� ��������������� ������������� �������.
			// 9.01.2018 ���� din==2, FGMRes ����������������� �������������� ������������� �������.
			fscanf_s(fp, "%d", &din);
			my_amg_manager.istabilizationTemp = din; // 0 - none
			fscanf_s(fp, "%d", &din);
			my_amg_manager.istabilizationSpeed = din; // 0 - none
			fscanf_s(fp, "%d", &din);
			my_amg_manager.istabilizationPressure = din; // 0 - none
			fscanf_s(fp, "%d", &din);
			my_amg_manager.istabilizationStress = din; // 0 - none
			fscanf_s(fp, "%d", &din);
			my_amg_manager.ipatch_number = din; // 0 - ���� �� �����������.

			// ������ ���� �� �������.
			fscanf_s(fp, "%d", &din);
			my_amg_manager.iprint_log_Temperature = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.iprint_log_Speed = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.iprint_log_Pressure = din;
			fscanf_s(fp, "%d", &din);
			my_amg_manager.iprint_log_Stress = din;

			fscanf(fp, "%d", &din);
			my_amg_manager.lfil = din;

			fscanf_s(fp, "%d", &din);
			ireconstruction_free_construct_alloc = din;

			fscanf_s(fp, "%d", &din);
			ianimation_write_on = din;

			// ��������� ����������� ������.
			gtdps = new TEMP_DEP_POWER[ltdp];
			matlist = new TPROP[lmatmax];
			b = new BLOCK[lb];
			s = new SOURCE[ls];
			w = new WALL[lw];
			integer i = 0; // ������� ����� for

			for (i = 0; i < ltdp; i++) {
				// ���������� ��� ������.
				gtdps[i].sname = new char[100]; // ��������� ������
				fscanf_s(fp, "%s", gtdps[i].sname, 100);
				//printf("%s",gtdps[i].sname);
				//getchar();
				// ���������� ������� � ������.
				my_read_power_table(gtdps[i].sname, gtdps[i].intemp, gtdps[i].inoffset_drain, gtdps[i].rtemp, gtdps[i].roffset_drain, gtdps[i].rpower_table);
				printf("printeger table\n"); // debug
				my_print_table(gtdps[i].intemp, gtdps[i].inoffset_drain, gtdps[i].rtemp, gtdps[i].roffset_drain, gtdps[i].rpower_table);
				printf("Please, press any key to continue...\n");
				//getchar();
				system("pause");

			}

			

			// ���������� ���� ����������
			for (i = 0; i < lmatmax; i++) {
				// �������� ����������:
				// ���������
				fscanf_s(fp, "%f", &fin);
				matlist[i].rho = fin;
				// ����������� ��� ���������� ��������
				//fscanf(fp, "%f", &fin);
				//matlist[i].cp = fin;
				fscanf_s(fp, "%d", &din);
				matlist[i].n_cp = din;
				matlist[i].arr_cp = NULL;
				matlist[i].temp_cp = NULL;
				matlist[i].arr_cp = new doublereal[matlist[i].n_cp];
				matlist[i].temp_cp = new doublereal[matlist[i].n_cp];
				if (matlist[i].temp_cp == NULL) {
					printf("problem memory allocation for temp_cp\n");
					system("pause");
					exit(1);
				}
				if (matlist[i].arr_cp == NULL) {
					printf("problem memory allocation for arr_cp\n");
					system("pause");
					exit(1);
				}
				for (integer i_4 = 0; i_4 < matlist[i].n_cp; i_4++) {
					// ����������� � C.
					fscanf_s(fp, "%f", &fin);
					matlist[i].temp_cp[i_4] = fin;
					fscanf_s(fp, "%f", &fin);
					matlist[i].arr_cp[i_4] = fin;
				}
				// ����������������
				//fscanf(fp, "%f", &fin);
				//matlist[i].lam = fin;
				fscanf_s(fp, "%d", &din);
				matlist[i].n_lam = din;
				matlist[i].arr_lam = NULL;
				matlist[i].temp_lam = NULL;
				matlist[i].arr_lam = new doublereal[matlist[i].n_lam];
				matlist[i].temp_lam = new doublereal[matlist[i].n_lam];
				if (matlist[i].temp_lam == NULL) {
					printf("problem memory allocation for temp_lam\n");
					system("pause");
					exit(1);
				}
				if (matlist[i].arr_lam == NULL) {
					printf("problem memory allocation for arr_lam\n");
					system("pause");
					exit(1);
				}
				for (integer i_4 = 0; i_4 < matlist[i].n_lam; i_4++) {
					// ����������� � C.
					fscanf_s(fp, "%f", &fin);
					matlist[i].temp_lam[i_4] = fin;
					fscanf_s(fp, "%f", &fin);
					matlist[i].arr_lam[i_4] = fin;
				}
				// ������������� ���������������� :
				fscanf_s(fp, "%f", &fin);
				matlist[i].orthotropy_multiplyer_x = fin;
				fscanf_s(fp, "%f", &fin);
				matlist[i].orthotropy_multiplyer_y = fin;
				fscanf_s(fp, "%f", &fin);
				matlist[i].orthotropy_multiplyer_z = fin;
				// 5.08.2017.
				// ������������ ��� ������ ���������.
				// ������ ���� � ����������� ��������.
				doublereal Poissonratio = 0.154;
				doublereal Youngmodule = 217.5e9;
				fscanf_s(fp, "%f", &fin);
				Poissonratio = fin;
				fscanf_s(fp, "%f", &fin);
				Youngmodule = fin*1e9;
				fscanf_s(fp, "%f", &fin);
				// TODO beta_t*1E-6
				// ������������ ����.
				doublereal E1_koef = Youngmodule / (1.0 - Poissonratio*Poissonratio);
				doublereal nu1_koef = Poissonratio / (1.0 - Poissonratio);
				matlist[i].mu_Lame = E1_koef / (2.0*(1.0 + nu1_koef));
				matlist[i].lambda_Lame = (E1_koef*nu1_koef) / (1.0 - nu1_koef*nu1_koef);
				// ����������� ������������ ��������
				fscanf_s(fp, "%f", &fin);
				matlist[i].mu = fin;
				// ����������� ��������� �������������� ����������
				fscanf_s(fp, "%f", &fin);
				matlist[i].beta_t = fin;
				// ������� �������������� ���������
				fscanf_s(fp, "%d", &din);
				matlist[i].blibmat = din;
				// ����� ��������� � ����������
				fscanf_s(fp, "%d", &din);
				matlist[i].ilibident = din;
				//printf("blibmat=%d ilibident=%d\n", matlist[i].blibmat, matlist[i].ilibident);
				//system("pause");
				
				// ��� ������� �� ������� �����,
				// ���� ������ �� �� ������������� ������� �������
				// ����� ���� ������������ ����������� 
				// ��������-���������� � ��������������� ������� ������������ Tref.
				fscanf_s(fp, "%d", &din);
				switch (din) {
				case 0: matlist[i].bBussineskApproach = false; break;
				case 1: matlist[i].bBussineskApproach = true; break;
				default: matlist[i].bBussineskApproach = false; break;
				}
				// ����� ������ ��� ����������� ������������ �������� �� ���������� ������
				fscanf_s(fp, "%d", &din);
				matlist[i].ilawmu = din;
				// ����������� �������� ������������ ��������
				fscanf_s(fp, "%f", &fin);
				matlist[i].mumin = fin;
				// ������������ �������� ������������ ��������
				fscanf_s(fp, "%f", &fin);
				matlist[i].mumax = fin;
				// ��������� ��������� ������� ��� ����������� �������� �� ���������� ������
				fscanf_s(fp, "%f", &fin);
				matlist[i].Amu = fin;
				fscanf_s(fp, "%f", &fin);
				matlist[i].Bmu = fin;
				fscanf_s(fp, "%f", &fin);
				matlist[i].Cmu = fin;
				// ���������� �������
				fscanf_s(fp, "%f", &fin);
				matlist[i].degreennmu = fin;

				// ������ ��������� �������� �� �������
				//printf("%e %e %e %e %e\n", matlist[i].rho, matlist[i].cp, matlist[i].lam, matlist[i].mu, matlist[i].beta_t);
				printf("cp\n");
				printf("t_C cp\n");
				for (integer i_4 = 0; i_4 < matlist[i].n_cp; i_4++) {
					printf("%e %e\n", matlist[i].temp_cp[i_4], matlist[i].arr_cp[i_4]);
				}
				printf("lam\n");
				printf("t_C lam\n");
				for (integer i_4 = 0; i_4 < matlist[i].n_lam; i_4++) {
					printf("%e %e\n", matlist[i].temp_lam[i_4], matlist[i].arr_lam[i_4]);
				}
				printf("%e %e %e\n", matlist[i].rho,  matlist[i].mu, matlist[i].beta_t);
				printf("%d %d %d\n", matlist[i].blibmat, matlist[i].ilibident, matlist[i].ilawmu); // bBoussinesq �� ����������
				printf("%e %e %e %e %e %e\n", matlist[i].mumin, matlist[i].mumax, matlist[i].Amu, matlist[i].Bmu, matlist[i].Cmu, matlist[i].degreennmu);
			}

			// ���������� ������
			for (i = 0; i<lb; i++) {

				fscanf_s(fp, "%d", &din);
				b[i].iunion_id = din; // 0==�������, ����� ���������� �������� �����������.

				fscanf_s(fp, "%d", &din);
				b[i].g.itypegeom = din; // 0 - Prism, 1 - Cylinder
				fscanf_s(fp, "%d", &din);
				if (din == 1) {
					b[i].bvisible = true;
				}
				else {
					b[i].bvisible = false;
				}


				// ���������
				fscanf_s(fp, "%f", &fin);
				b[i].g.xS = scale*fin;
				fscanf_s(fp, "%f", &fin);
				b[i].g.yS = scale*fin;
				fscanf_s(fp, "%f", &fin);
				b[i].g.zS = scale*fin;
				fscanf_s(fp, "%f", &fin);
				b[i].g.xE = scale*fin;
				fscanf_s(fp, "%f", &fin);
				b[i].g.yE = scale*fin;
				fscanf_s(fp, "%f", &fin);
				b[i].g.zE = scale*fin;
				// swap
				if (b[i].g.xS>b[i].g.xE) {
					dbuf = b[i].g.xS;
					b[i].g.xS = b[i].g.xE;
					b[i].g.xE = dbuf;
				}
				if (b[i].g.yS > b[i].g.yE) {
					dbuf = b[i].g.yS;
					b[i].g.yS = b[i].g.yE;
					b[i].g.yE = dbuf;
				}
				if (b[i].g.zS > b[i].g.zE) {
					dbuf = b[i].g.zS;
					b[i].g.zS = b[i].g.zE;
					b[i].g.zE = dbuf;
				}

				// Cylinder

				fscanf_s(fp, "%d", &din);
				b[i].g.iPlane = din;

				fscanf_s(fp, "%f", &fin);
				b[i].g.xC = scale*fin;
				fscanf_s(fp, "%f", &fin);
				b[i].g.yC = scale*fin;
				fscanf_s(fp, "%f", &fin);
				b[i].g.zC = scale*fin;
				fscanf_s(fp, "%f", &fin);
				b[i].g.Hcyl = scale*fin;
				if (b[i].g.Hcyl < 0.0) {
					// ����������� ������������� ������ ��������.
					switch (b[i].g.iPlane) {
					case XY:
						b[i].g.zC += b[i].g.Hcyl;
						break;
					case XZ:
						b[i].g.yC += b[i].g.Hcyl;
						break;
					case YZ:
						b[i].g.xC += b[i].g.Hcyl;
						break;
					}
					b[i].g.Hcyl = fabs(b[i].g.Hcyl);
				}

				fscanf_s(fp, "%f", &fin);
				b[i].g.R_out_cyl = scale*fin;
				fscanf_s(fp, "%f", &fin);
				b[i].g.R_in_cyl = scale*fin;

				// Polygon
				fscanf_s(fp, "%d", &din);
				b[i].g.iPlane_obj2 = din;
				fscanf_s(fp, "%d", &din);
				b[i].g.nsizei = din;
				b[i].g.hi = new doublereal[b[i].g.nsizei];
				b[i].g.xi = new doublereal[b[i].g.nsizei];
				b[i].g.yi = new doublereal[b[i].g.nsizei];
				b[i].g.zi = new doublereal[b[i].g.nsizei];
				for (integer i73 = 0; i73 < b[i].g.nsizei; i73++) {
					fscanf_s(fp, "%f", &fin);
					b[i].g.hi[i73] = scale*fin;
					fscanf_s(fp, "%f", &fin);
					b[i].g.xi[i73] = scale*fin;
					fscanf_s(fp, "%f", &fin);
					b[i].g.yi[i73] = scale*fin;
					fscanf_s(fp, "%f", &fin);
					b[i].g.zi[i73] = scale*fin;
					if (b[i].g.hi[i73] < 0.0) {
						// ����������� ������������� ������ ��������.
						switch (b[i].g.iPlane_obj2) {
						case XY:
							b[i].g.zi[i73] += b[i].g.hi[i73];
							break;
						case XZ:
							b[i].g.yi[i73] += b[i].g.hi[i73];
							break;
						case YZ:
							b[i].g.xi[i73] += b[i].g.hi[i73];
							break;
						}
						b[i].g.hi[i73] = fabs(b[i].g.hi[i73]);
					}
				}

				if ((b[i].g.itypegeom == 2) && (b[i].g.nsizei > 0)) {
					// �� ������� � ����������� ������ ������� �������� ����� �������� 
					// ������ � inmodel_temp ������ ������� ������ ��� ������ ��������.
					doublereal xmin53 = 1.0e30;
					doublereal ymin53 = 1.0e30;
					doublereal zmin53 = 1.0e30;
					doublereal xmax53 = -1.0e30;
					doublereal ymax53 = -1.0e30;
					doublereal zmax53 = -1.0e30;
					for (integer i73 = 0; i73 < b[i].g.nsizei; i73++) {
						if (b[i].g.xi[i73] > xmax53) xmax53 = b[i].g.xi[i73];
						if (b[i].g.yi[i73] > ymax53) ymax53 = b[i].g.yi[i73];
						if (b[i].g.zi[i73] > zmax53) zmax53 = b[i].g.zi[i73];
						if (b[i].g.xi[i73] < xmin53) xmin53 = b[i].g.xi[i73];
						if (b[i].g.yi[i73] < ymin53) ymin53 = b[i].g.yi[i73];
						if (b[i].g.zi[i73] < zmin53) zmin53 = b[i].g.zi[i73];
					}
					switch (b[i].g.iPlane_obj2) {
					case XY:
						b[i].g.xS = xmin53;
						b[i].g.xE = xmax53;
						b[i].g.yS = ymin53;
						b[i].g.yE = ymax53;
						b[i].g.zS = zmin53;
						b[i].g.zE = zmin53 + b[i].g.hi[0];
						break;
					case XZ:
						b[i].g.xS = xmin53;
						b[i].g.xE = xmax53;
						b[i].g.zS = zmin53;
						b[i].g.zE = zmax53;
						b[i].g.yS = ymin53;
						b[i].g.yE = ymin53 + b[i].g.hi[0];
						break;
					case YZ:
						b[i].g.yS = ymin53;
						b[i].g.yE = ymax53;
						b[i].g.zS = zmin53;
						b[i].g.zE = zmax53;
						b[i].g.xS = xmin53;
						b[i].g.xE = xmin53 + b[i].g.hi[0];
						break;
					}
				}

				if (b[i].g.itypegeom == 1) {
					// Cylinder
					//printf("%e %e %e %e %e %e %e %e %e %e %e %e\n", b[i].g.xS, b[i].g.yS, b[i].g.zS, b[i].g.xE, b[i].g.yE, b[i].g.zE, b[i].g.xC, b[i].g.yC, b[i].g.zC, b[i].g.Hcyl, b[i].g.R_out_cyl, b[i].g.R_in_cyl);
					//getchar();
				}


				// ���� �������������� ����������.
				// emissivity
				fscanf_s(fp, "%f", &fin);
				b[i].radiation.emissW = fin;
				fscanf_s(fp, "%f", &fin);
				b[i].radiation.emissE = fin;
				fscanf_s(fp, "%f", &fin);
				b[i].radiation.emissS = fin;
				fscanf_s(fp, "%f", &fin);
				b[i].radiation.emissN = fin;
				fscanf_s(fp, "%f", &fin);
				b[i].radiation.emissB = fin;
				fscanf_s(fp, "%f", &fin);
				b[i].radiation.emissT = fin;
				fscanf_s(fp, "%d", &din);
				if (din == 0) {
					// ���� �� �������� ��������� �����������.
					b[i].radiation.binternalRadiation = false;
				}
				else {
					// ���� �������� ��������� �����������.
					b[i].radiation.binternalRadiation = true;
					if (bvacuumPrism) {
						bdouble_vacuum_PRISM = true;
					}
					bvacuumPrism = true;
					// ��������� View Factors.
					calculate_view_factors(b[i]);
				}
				b[i].radiation.nodelistW = NULL;
				b[i].radiation.nodelistE = NULL;
				b[i].radiation.nodelistS = NULL;
				b[i].radiation.nodelistN = NULL;
				b[i].radiation.nodelistB = NULL;
				b[i].radiation.nodelistT = NULL;
				b[i].radiation.nodelistWsize = 0;
				b[i].radiation.nodelistEsize = 0;
				b[i].radiation.nodelistSsize = 0;
				b[i].radiation.nodelistNsize = 0;
				b[i].radiation.nodelistBsize = 0;
				b[i].radiation.nodelistTsize = 0;

				// ������������� ��������� � ���� ����������
				fscanf_s(fp, "%d", &din);
				b[i].imatid = din;

				fscanf_s(fp, "%d", &din);

				// bCylinderFixed
				if (din == 1) {
					b[i].CylinderFixed = true;
				}
				else {
					b[i].CylinderFixed = false;
				}

				// �������� ��������������
				//fscanf_s(fp, "%f", &fin);
				//b[i].Sc = fin;
				// 19 november 2016 ������������ ��������� �������� ��������������.
				fscanf_s(fp, "%d", &din);
				b[i].n_Sc = din;
				b[i].arr_Sc = NULL;
				b[i].temp_Sc = NULL;
				b[i].arr_Sc = new doublereal[b[i].n_Sc];
				b[i].temp_Sc = new doublereal[b[i].n_Sc];
				if (b[i].temp_Sc == NULL) {
					printf("problem memory allocation for temp_Sc\n");
					system("pause");
					exit(1);
				}
				if (b[i].arr_Sc == NULL) {
					printf("problem memory allocation for arr_Sc\n");
					system("pause");
					exit(1);
				}
				for (integer i_4 = 0; i_4 < b[i].n_Sc; i_4++) {
					// ����������� � C.
					fscanf_s(fp, "%f", &fin);
					b[i].temp_Sc[i_4] = fin;
					fscanf_s(fp, "%f", &fin);
					if (fin != fin) {
						b[i].arr_Sc[i_4] = 0.0;
					}
					else {
						b[i].arr_Sc[i_4] = fin;
					}
				}

				// debug
				//if (fabs(b[i].Sc)>1.0e-30) {
					//printf("%e\n", b[i].Sc);
					//getchar();
				//}
				// ����� ����������� �������� �������������� � ����� �� �������.
				// 0 - �� ������� �� �������, 1 - square wave �����������, 
				// 2 - square wave apparat �����������.
				fscanf_s(fp, "%d", &din);
				b[i].ipower_time_depend = din;
				// ��� �����
				fscanf_s(fp, "%d", &din);
				b[i].itype = din;

				// ������ ��������� �������� �� �������
				printf("%e %e %e %e %e %e\n", b[i].g.xS, b[i].g.yS, b[i].g.zS, b[i].g.xE, b[i].g.yE, b[i].g.zE);
				printf("%d %d %d\n", b[i].imatid,  b[i].itype, b[i].ipower_time_depend);
				printf("temperature depend power\n");
				printf("t_C power_W\n");
				for (integer i_54 = 0; i_54 < b[i].n_Sc; i_54++) {
					printf("%e %e\n",b[i].temp_Sc[i_54], b[i].arr_Sc[i_54]);
				}
			}

			// ���������� ���������� �����
			for (i = 0; i < ls; i++) {

				fscanf_s(fp, "%d", &din);
				s[i].iunion_id = din; // 0==�������, ����� ���������� �������� �����������.

				fscanf_s(fp, "%f", &fin);
				s[i].power = fin;
				fscanf_s(fp, "%d", &din);
				if (din == 0) {
					// const
					// ������� ���������� �������� ������������ � ����� ��������.
					s[i].power_multiplyer = 1.0;
					s[i].bgarber_depend = false;
				}
				else if (din == 1) {
					// ������������ � ����� �������� ������� ��������
					// � ����������� �� ������������ ����������� � �������� 
					// �������� �������� �����.
					s[i].bgarber_depend = true;
					s[i].power_multiplyer = s[i].power;
					// �������� ����� ��������� ��� ambient Temperature ����������� ��������� �����.
				}
				fscanf_s(fp, "%d", &din);
				s[i].igarber_depend = din; // ���������� ����� �������.
				fscanf_s(fp, "%f", &fin);
				s[i].roperation_offset_drain = fin; // ������� �������� �������� �����.
				//printf("offset drain is %e\n",s[i].roperation_offset_drain);
				//getchar();
				bool bsplinereadOk = true;
				if (s[i].bgarber_depend) {
					s[i].power = my_splain_interpol_power_table(gtdps[s[i].igarber_depend].intemp,
						gtdps[s[i].igarber_depend].inoffset_drain,
						gtdps[s[i].igarber_depend].rtemp,
						gtdps[s[i].igarber_depend].roffset_drain,
						gtdps[s[i].igarber_depend].rpower_table,
						operatingtemperature,
						s[i].roperation_offset_drain);
					if (bsplinereadOk) {
						// ��������� �������� �������� ���������� �������������.
						printf("single test validation spline approximation...\n");
						printf("calculate initial power=%e\n", s[i].power);
						printf("please, press any key to continue...");
						// getchar();
						system("pause");

						bsplinereadOk = false;
					}
					s[i].power *= s[i].power_multiplyer; // ���������� �� �������������� ���������.
				}



				fscanf_s(fp, "%d", &din);
				s[i].iPlane = din;
				// ���������
				fscanf_s(fp, "%f", &fin);
				s[i].g.xS = scale*fin;
				fscanf_s(fp, "%f", &fin);
				s[i].g.yS = scale*fin;
				fscanf_s(fp, "%f", &fin);
				s[i].g.zS = scale*fin;
				fscanf_s(fp, "%f", &fin);
				s[i].g.xE = scale*fin;
				fscanf_s(fp, "%f", &fin);
				s[i].g.yE = scale*fin;
				fscanf_s(fp, "%f", &fin);
				s[i].g.zE = scale*fin;



				// swap
				if (s[i].g.xS > s[i].g.xE) {
					dbuf = s[i].g.xS;
					s[i].g.xS = s[i].g.xE;
					s[i].g.xE = dbuf;
				}
				if (s[i].g.yS > s[i].g.yE) {
					dbuf = s[i].g.yS;
					s[i].g.yS = s[i].g.yE;
					s[i].g.yE = dbuf;
				}
				if (s[i].g.zS > s[i].g.zE) {
					dbuf = s[i].g.zS;
					s[i].g.zS = s[i].g.zE;
					s[i].g.zE = dbuf;
				}
				switch (s[i].iPlane) {
				case XY: s[i].square = fabs(s[i].g.xE - s[i].g.xS)*fabs(s[i].g.yE - s[i].g.yS); break;
				case XZ: s[i].square = fabs(s[i].g.xE - s[i].g.xS)*fabs(s[i].g.zE - s[i].g.zS); break;
				case YZ: s[i].square = fabs(s[i].g.yE - s[i].g.yS)*fabs(s[i].g.zE - s[i].g.zS); break;
				default: break;
				}
				printf("%e %d %e %e %e %e %e %e %e\n", s[i].power, s[i].iPlane, s[i].g.xS, s[i].g.yS, s[i].g.zS, s[i].g.xE, s[i].g.yE, s[i].g.zE, s[i].square);
			}

			// ���������� ������ ������



			for (i = 0; i < lw; i++) {
				
				fscanf_s(fp, "%d", &din);
				w[i].iunion_id = din; // 0==�������, ����� ���������� �������� �����������.

				fscanf_s(fp, "%d", &din);
				w[i].ifamily = din;
				switch (din) {
				case 1:  fscanf_s(fp, "%f", &fin);
					w[i].Tamb = fin;
					fscanf_s(fp, "%f", &fin); // Stefan Bolcman
					// termostability wall
					w[i].emissivity = 0.0;
					w[i].film_coefficient = 0.0;
					fscanf_s(fp, "%f", &fin);
					w[i].hf = 0.0;
					break; // ������� ����
				case 2:  fscanf_s(fp, "%f", &fin);
					w[i].Tamb = 0.0;
					fscanf_s(fp, "%f", &fin);  // Stefan Bolcman
					// adiabatic wall
					w[i].emissivity = 0.0;
					w[i].film_coefficient = 0.0;
					fscanf_s(fp, "%f", &fin);
					w[i].hf = 0.0;
					break; // ���������� ������� �������
				case 3:  fscanf_s(fp, "%f", &fin);
					w[i].Tamb = fin;
					fscanf_s(fp, "%f", &fin); // Stefan Bolcman
					// Newton-Richman condition, film coefficient.
					w[i].emissivity = 0.0;
					w[i].film_coefficient = fin;
					fscanf_s(fp, "%f", &fin);
					w[i].hf = 0.0;
					break; // ������-������.
				case 4:  fscanf_s(fp, "%f", &fin);
					w[i].Tamb = fin;
					fscanf_s(fp, "%f", &fin); // Stefan Bolcman
					// Stefan - Bolcman condition
					w[i].emissivity = fin;
					w[i].film_coefficient = 0.0;
					fscanf_s(fp, "%f", &fin);
					w[i].hf = 0.0;
					break; // ������-��������.
				default: break;
				}
				fscanf_s(fp, "%d", &din);
				if (din == 1) w[i].bsymmetry = true; else w[i].bsymmetry = false;
				fscanf_s(fp, "%d", &din);
				if (din == 1) w[i].bpressure = true; else w[i].bpressure = false;
				fscanf_s(fp, "%d", &din);
				if (din == 1) w[i].bopening = true; else w[i].bopening = false;
				fscanf_s(fp, "%f", &fin);
				w[i].Vx = fin;
				fscanf_s(fp, "%f", &fin);
				w[i].Vy = fin;
				fscanf_s(fp, "%f", &fin);
				w[i].Vz = fin;
				fscanf_s(fp, "%f", &fin);
				w[i].P = fin;
				fscanf(fp, "%d", &din);
				w[i].ithermal_Stress_boundary_condition = din;
				fscanf(fp, "%f", &fin);
				w[i].xForce = fin;
				fscanf(fp, "%f", &fin);
				w[i].yForce = fin;
				fscanf(fp, "%f", &fin);
				w[i].zForce = fin;
				fscanf_s(fp, "%d", &din);
				w[i].iPlane = din;
				// ���������
				fscanf_s(fp, "%f", &fin);
				w[i].g.xS = scale*fin;
				fscanf_s(fp, "%f", &fin);
				w[i].g.yS = scale*fin;
				fscanf_s(fp, "%f", &fin);
				w[i].g.zS = scale*fin;
				fscanf_s(fp, "%f", &fin);
				w[i].g.xE = scale*fin;
				fscanf_s(fp, "%f", &fin);
				w[i].g.yE = scale*fin;
				fscanf_s(fp, "%f", &fin);
				w[i].g.zE = scale*fin;
				// swap
				if (w[i].g.xS > w[i].g.xE) {
					dbuf = w[i].g.xS;
					w[i].g.xS = w[i].g.xE;
					w[i].g.xE = dbuf;
				}
				if (w[i].g.yS > w[i].g.yE) {
					dbuf = w[i].g.yS;
					w[i].g.yS = w[i].g.yE;
					w[i].g.yE = dbuf;
				}
				if (w[i].g.zS > w[i].g.zE) {
					dbuf = w[i].g.zS;
					w[i].g.zS = w[i].g.zE;
					w[i].g.zE = dbuf;
				}
				
				printf("%d %e %e %d %e %e %e %e %e %e\n", w[i].ifamily, w[i].Tamb, w[i].hf, w[i].iPlane, w[i].g.xS, w[i].g.yS, w[i].g.zS, w[i].g.xE, w[i].g.yE, w[i].g.zE);
			}


			// ���������.
			fscanf_s(fp, "%d", &din);
			lu = din;
			if (lu == 0) {
				my_union = NULL;
			}
			else {
				my_union = new UNION[lu];
				// �������������.
				for (i = 0; i < lu; i++) {
					my_union[i].f = NULL;
					my_union[i].xpos = NULL;
					my_union[i].ypos = NULL;
					my_union[i].zpos = NULL;
					my_union[i].xposadd = NULL;
					my_union[i].yposadd = NULL;
					my_union[i].zposadd = NULL;
					my_union[i].iswitchMeshGenerator = 2; // 2 - CoarseMeshGen
					my_union[i].inxadd = -1;
					my_union[i].inyadd = -1;
					my_union[i].inzadd = -1;
					my_union[i].flow_interior = 0;
				}
			}
			for (i = 0; i < lu; i++) {
				fscanf_s(fp, "%f", &fin);
				my_union[i].xS = scale*fin;
				fscanf_s(fp, "%f", &fin);
				my_union[i].xE = scale*fin;
				fscanf_s(fp, "%f", &fin);
				my_union[i].yS = scale*fin;
				fscanf_s(fp, "%f", &fin);
				my_union[i].yE = scale*fin;
				fscanf_s(fp, "%f", &fin);
				my_union[i].zS = scale*fin;
				fscanf_s(fp, "%f", &fin);
				my_union[i].zE = scale*fin;

				fscanf_s(fp, "%d", &din);
				my_union[i].id = din; // ���������� ������������� ����������.
				fscanf_s(fp, "%d", &din);
				my_union[i].inx = din;
				fscanf_s(fp, "%d", &din);
				my_union[i].iny = din;
				fscanf_s(fp, "%d", &din);
				my_union[i].inz = din;

			}


			// ���������� ���������� � ������ �������� ���������
			fscanf_s(fp, "%d", &din);
			eqin.itemper = din;
			fscanf_s(fp, "%d", &din);
			eqin.imaxflD = din;
			if (eqin.imaxflD == 0) {
				eqin.fluidinfo = NULL;
			}
			else
			{
				// ��������� ����������� ������
				if (eqin.fluidinfo != NULL) {
					delete eqin.fluidinfo;
					eqin.fluidinfo = NULL;
				}
				eqin.fluidinfo = new FLOWINFO[eqin.imaxflD];
				for (i = 0; i < eqin.imaxflD; i++) {
					// ���������� ��������� ������� �����
					fscanf_s(fp, "%f", &fin);
					eqin.fluidinfo[i].xc = scale*fin;
					fscanf_s(fp, "%f", &fin);
					eqin.fluidinfo[i].yc = scale*fin;
					fscanf_s(fp, "%f", &fin);
					eqin.fluidinfo[i].zc = scale*fin;
					fscanf_s(fp, "%d", &din);
					eqin.fluidinfo[i].iflow = din;
					fscanf_s(fp, "%d", &din);
					eqin.fluidinfo[i].iflowregime = din;
					fscanf_s(fp, "%d", &din);
					eqin.fluidinfo[i].iturbmodel = din;
					fscanf_s(fp, "%f", &fin);
					eqin.fluidinfo[i].Cs = fin; // ���������� �������������.
					fscanf_s(fp, "%d", &din);
					// ���� ������������� ����������� �������� ���������� �������������.
					// �������� Dynamic Subgrid Scale Model ������� 1991 ����.
					if (din == 1) {
						eqin.fluidinfo[i].bDynamic_Stress = true;
					}
					else {
						eqin.fluidinfo[i].bDynamic_Stress = false;
					}
					fscanf_s(fp, "%d", &din);
					// �������� ����������� ������ � ����� �� ��������� �������� ���������� �������������.
					if (din == 1) {
						eqin.fluidinfo[i].bLimiters_Cs = true;
					}
					else {
						eqin.fluidinfo[i].bLimiters_Cs = false;
					}
					fscanf_s(fp, "%f", &fin);
					eqin.fluidinfo[i].rminCs = fin; // ����������� ��������� �������� ���������� �������������.
					fscanf_s(fp, "%f", &fin);
					eqin.fluidinfo[i].rmaxCs = fin; // ������������ ��������� �������� ���������� �������������.
					fscanf_s(fp, "%d", &din);
					eqin.fluidinfo[i].itypeFiltrGermano = din; // ��� ������� � ������ ������� 1991 ����.
					fscanf_s(fp, "%f", &fin);
					eqin.fluidinfo[i].roughness = 1.0e-6*fin; // ������������� ������ � �.
					fscanf_s(fp, "%f", &fin);
					eqin.fluidinfo[i].rRi_mult = fin; // ��������� �������������� ������������ ����� ����������.
					fscanf_s(fp, "%f", &fin);
					eqin.fluidinfo[i].rSelectiveAngle = fin; // ��������� �������� ���� � ������ Selective Smagorinsky.
					fscanf_s(fp, "%d", &din);
					eqin.fluidinfo[i].ipowerroughness = din; // ���������� ������� � ������ ����� �������������.
					fscanf_s(fp, "%d", &din);
					eqin.fluidinfo[i].itypeSelectiveSmagorinsky_filtr = din; // ��� ������� � ������ Selective Smagorinsky.
					fscanf_s(fp, "%d", &din);
					eqin.fluidinfo[i].bfdelta = din; // ���� ��������������� �����.
					fscanf_s(fp, "%d", &din);
					eqin.fluidinfo[i].bSmagorinskyLilly = din; // ������ �������������-�����.
					fscanf_s(fp, "%d", &din);
					eqin.fluidinfo[i].bsurface_roughness = din; // ���� ������������� ������.
					fscanf_s(fp, "%d", &din);
					// ���� ������� � ��������� ����� ����.
					if (din == 1) {
						eqin.fluidinfo[i].bSwirlAmendment = true;
					}
					else {
						eqin.fluidinfo[i].bSwirlAmendment = false;
					}
					fscanf_s(fp, "%d", &din);
					// ���� ��������������� � ������ �������������
					if (din == 1) {
						eqin.fluidinfo[i].bSelectiveSmagorinsky = true;
					}
					else {
						eqin.fluidinfo[i].bSelectiveSmagorinsky = false;
					}

				}
			}

			fclose(fp); // �������� �����
		}
	}

#else
// eqin - ���������� � ������ �������� ���������.

// dgx, dgy, dgz - ������ ���� �������.
// inx, iny, inz - ���������� ����� �� ������ �� ����.

FILE *fp;
errno_t err1;
if ((err1 = fopen_s(&fp, fname, "r")) != 0) {
	printf("No input File premeshin.txt \n");
	//getchar();
	system("pause");

}
else
{
	if (fp != NULL) {
		float fin = 0.0;
		integer din = 0;
		doublereal scale = 1.0;
		doublereal dbuf; // ��� �������������� � ������� �����������



		fscanf_s(fp, "%lld", &din);
		ionly_solid_visible = din;
		printf("ionly_solid_visible =%lld\n", ionly_solid_visible);
		fscanf_s(fp, "%f", &fin);
		scale = fin;
		fscanf_s(fp, "%lld", &din);
		lmatmax = din;
		fscanf_s(fp, "%lld", &din);
		lb = din;
		fscanf_s(fp, "%lld", &din);
		ls = din;
		fscanf_s(fp, "%lld", &din);
		lw = din;
		fscanf_s(fp, "%lld", &din);
		ltdp = din; // ���������� ���������� ������ � ���������� ������� �� ����������� ����������� �������� �� �����������.
		

					// ��������� �������� ������� ���� �������:
		fscanf_s(fp, "%f", &fin);
		dgx = fin;
		fscanf_s(fp, "%f", &fin);
		dgy = fin;
		fscanf_s(fp, "%f", &fin);
		dgz = fin;

		// ��������� ���������� ����� �� ������ ������������ ���
		fscanf_s(fp, "%lld", &din);
		inx = din;
		fscanf_s(fp, "%lld", &din);
		iny = din;
		fscanf_s(fp, "%lld", &din);
		inz = din;

		fscanf_s(fp, "%f", &fin);
		operatingtemperature = fin; // Operating Temperature
		operating_temperature_for_film_coeff = fin;

		// ������������� ��������� �������� ����������.
		// ������ �������� ��� ���� ��������� �������.
		// initialization value.
		fscanf_s(fp, "%f", &fin);
		starting_speed_Vx = fin;
		fscanf_s(fp, "%f", &fin);
		starting_speed_Vy = fin;
		fscanf_s(fp, "%f", &fin);
		starting_speed_Vz = fin;

		// ��������� ���������� ������� ����� ����� ������� �������� ���������������� ����� Variation Plot.
		fscanf_s(fp, "%f", &fin);
		Tochka_position_X0_for_XY_Plot = scale*fin;
		fscanf_s(fp, "%f", &fin);
		Tochka_position_Y0_for_XY_Plot = scale*fin;
		fscanf_s(fp, "%f", &fin);
		Tochka_position_Z0_for_XY_Plot = scale*fin;
		// ����������� ����� ��������� � ������������ 
		// ����� �� ���� ���������� ������������� ������� ���������:
		// 0 - Ox, 1 - Oy, 2 - Oz.
		fscanf_s(fp, "%d", &din);
		idirectional_for_XY_Plot = din;

		fscanf_s(fp, "%f", &fin);
		etalon_max_size_ratio = fin; // ����������� ��������� �����.

		fscanf_s(fp, "%f", &fin);
		etalon_max_size_ratio2 = fin; // �������� �������� ��������� ����� �� ������ FlowVision.

		fscanf_s(fp, "%lld", &din);
		switch (din) {
		case 0: bsnap_TO_global = false;  break;
		default: bsnap_TO_global = true;  break;
		}

		fscanf_s(fp, "%lld", &din);
		iswitchsolveramg_vs_BiCGstab_plus_ILU2 = din; // ����� ��������� ���������� : ���� amg1r5 ���� BiCGStab+ILU2.

		fscanf_s(fp, "%lld", &din);
		iswitchsolveramg_vs_BiCGstab_plus_ILU6 = din; // ����� ��������� ���������� : ���� �����0.14 ���� BiCGStab+ILU6.

		fscanf_s(fp, "%lld", &din);
		if (din == 1) {
			// SIMPLEC algorithm.
			iSIMPLE_alg = SIMPLEC_Van_Doormal_and_Raithby;
		}
		else {
			// SIMPLE algorithm 1972.
			iSIMPLE_alg = SIMPLE_Carretto;
		}

		fscanf_s(fp, "%lld", &din);
		// ����� ����� ��� ������ ��������.
		// �������� ��� ����������� ������ ��������� ��������������� 
		// ������������ � ����� my_approx_convective2.c
		switch (din) {
		case 1: iFLOWScheme = UNEVEN_MUSCL; break; // MUSCL 2
		case 2: iFLOWScheme = UNEVEN_SOUCUP; break; // SOUCUP [MINMOD] 2
		case 3: iFLOWScheme = UNEVEN_HLPA; break; // HLPA 2
		case 4: iFLOWScheme = UNEVEN_SMART; break; // SMART 3
		case 5: iFLOWScheme = UNEVEN_WACEB; break; // WACEB 3 TVD
		case 6: iFLOWScheme = UNEVEN_SMARTER; break; // SMARTER 3
		case 7: iFLOWScheme = UNEVEN_LPPA; break; // LPPA 3
		case 8: iFLOWScheme = UNEVEN_VONOS; break; // VONOS 3
		case 9: iFLOWScheme = UNEVEN_STOIC; break; // STOIC
		case 10: iFLOWScheme = UNEVEN_CLAM; break; // CLAM
		case 11: iFLOWScheme = UNEVEN_OSHER; break; // OSHER
		case 12: iFLOWScheme = UNEVEN_EXPONENTIAL; break; // EXPONENTIAL
		case 13: iFLOWScheme = UNEVEN_SUPER_C; break; // SUPER_C
		case 14: iFLOWScheme = UNEVEN_ISNAS; break; // ISNAS
		case 15: iFLOWScheme = UNEVEN_CUBISTA; break; // CUBISTA
		default: iFLOWScheme = 2; break; // UDS ����� ���������� �����.
		}

		fscanf_s(fp, "%lld", &din);
		// ����� ����� ��� ����������� � ������ ��������.
		// �������� ��� ����������� ������ ��������� ��������������� 
		// ������������ � ����� my_approx_convective2.c
		switch (din) {
		case 1: iTEMPScheme = UNEVEN_MUSCL; break; // MUSCL 2
		case 2: iTEMPScheme = UNEVEN_SOUCUP; break; // SOUCUP [MINMOD] 2
		case 3: iTEMPScheme = UNEVEN_HLPA; break; // HLPA 2
		case 4: iTEMPScheme = UNEVEN_SMART; break; // SMART 3
		case 5: iTEMPScheme = UNEVEN_WACEB; break; // WACEB 3 TVD
		case 6: iTEMPScheme = UNEVEN_SMARTER; break; // SMARTER 3
		case 7: iTEMPScheme = UNEVEN_LPPA; break; // LPPA 3
		case 8: iTEMPScheme = UNEVEN_VONOS; break; // VONOS 3
		case 9: iTEMPScheme = UNEVEN_STOIC; break; // STOIC
		case 10: iTEMPScheme = UNEVEN_CLAM; break; // CLAM
		case 11: iTEMPScheme = UNEVEN_OSHER; break; // OSHER
		case 12: iTEMPScheme = UNEVEN_EXPONENTIAL; break; // EXPONENTIAL
		case 13: iTEMPScheme = UNEVEN_SUPER_C; break; // SUPER_C
		case 14: iTEMPScheme = UNEVEN_ISNAS; break; // ISNAS
		case 15: iTEMPScheme = UNEVEN_CUBISTA; break; // CUBISTA
		default: iTEMPScheme = 2; break; // UDS ����� ���������� �����.
		}



		// ����� ��������� ����������.
		fscanf_s(fp, "%lld", &din);
		iswitchMeshGenerator = din;

		fscanf_s(fp, "%lld", &din);
		steady_or_unsteady_global_determinant = 2;
		if ((din == 0) || (din == 1) || (din == 2) || (din == 3) || (din == 5) || (din == 6)) {
			// 2 - mesh generator only.
			// 3 - fluid dynamic.
			// 5 - Static Structural.
			// 6 - Thermal Stress.
			steady_or_unsteady_global_determinant = din; // steady - 0, or unsteady -1 calculation.
		}
		else {
			printf("error input parametr steady or unsteady calculation\n");
			system("PAUSE");
			exit(1);
		}

		fscanf_s(fp, "%lld", &din);
		if ((din == 0) || (din == 1) || (din == 2) || (din == 3)) {
			glTSL.id_law = din;
		}
		else {
			printf("error input parametr timestep law\n");
			system("PAUSE");
			exit(1);
		}
		fscanf_s(fp, "%f", &fin);
		if ((fin <= 0.0) || (fin >= 1.0)) {
			printf("error input parametr timestep law Factor a\n");
			system("PAUSE");
			exit(1);
		}
		glTSL.Factor_a_for_Linear = fin; // Factor_a
		fscanf_s(fp, "%f", &fin);
		if (fin <= 0.0) {
			printf("error input parametr timestep law tau must be strongly positive\n");
			system("PAUSE");
			exit(1);
		}
		glTSL.tau = fin; // ������������ ��������.
		fscanf_s(fp, "%f", &fin);
		glTSL.Q = fin;  // ����������.
						// ��������� ����������� ������ ��� ���� �������.
		fscanf_s(fp, "%f", &fin);
		if ((fin <= 0.0) || (fin >= 1.0)) {
			printf("error input parametr timestep law APPARAT multiplyer\n");
			system("PAUSE");
			exit(1);
		}
		glTSL.m1 = fin;
		fscanf_s(fp, "%f", &fin);
		if (fin <= 0.0) {
			printf("error input parametr timestep law APPARAT tau1 must be strongly positive\n");
			system("PAUSE");
			exit(1);
		}
		glTSL.tau1 = fin;
		fscanf_s(fp, "%f", &fin);
		if (fin <= 0.0) {
			printf("error input parametr timestep law APPARAT tau2 must be strongly positive\n");
			system("PAUSE");
			exit(1);
		}
		glTSL.tau2 = fin;
		fscanf_s(fp, "%f", &fin);
		if (fin <= 0.0) {
			printf("error input parametr timestep law APPARAT tau_pause must be strongly positive\n");
			system("PAUSE");
			exit(1);
		}
		glTSL.tau_pause = fin;
		fscanf_s(fp, "%lld", &din);
		glTSL.n_cycle = din;
		fscanf_s(fp, "%f", &fin);
		if (fin <= 0.0) {
			printf("error input parametr timestep law APPARAT Period must be strongly positive\n");
			system("PAUSE");
			exit(1);
		}
		glTSL.T_all = fin;
		doublereal t_pause_gl = glTSL.T_all - glTSL.n_cycle*(2 * glTSL.tau1 + glTSL.tau2 + glTSL.tau_pause);
		if (t_pause_gl <= 0.0) {
			printf("error in parameters Square Wave APPARAT time step law.\n");
			//getchar();
			system("pause");
			exit(1);
		}

		fscanf_s(fp, "%f", &fin);
		if (fin <= 0.0) {
			printf("error input parametr on_time_double_linear law hot cold reshime must be strongly positive\n");
			system("PAUSE");
			exit(1);
		}
		glTSL.on_time_double_linear = fin;

		// ����� ��������� ��������������� ������������� ��� ������� ������������� � ������ ����.
		fscanf_s(fp, "%f", &fin);
		globalEndTimeUnsteadyTemperatureCalculation = fin;


		// Newton-Richman condition.
		fscanf_s(fp, "%lld", &din);
		adiabatic_vs_heat_transfer_coeff = din; // 0 - adiabatic wall, 1 - Newton Richman condition, 2 - Stefan Bolcman condition, 3 - mix condition.
		fscanf_s(fp, "%f", &fin);
		film_coefficient = fin;
		// A��� �����
		fscanf_s(fp, "%lld", &din);
		if (din == 0) {
			// ������� ����������������� �����.
			b_on_adaptive_local_refinement_mesh = false;
		}
		else {
			// ����
			b_on_adaptive_local_refinement_mesh = true;
		}
		fscanf_s(fp, "%lld", &din);
		itype_ALICE_Mesh = din;

		fscanf_s(fp, "%lld", &din);
		my_amg_manager.m_restart = din;
		// classical algebraic multigrid parameters:
		// only for my_agregat_amg.cu.
		// only for my_agregat_amg.cu.
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.imySortAlgorithm = din;
		fscanf_s(fp, "%lld", &din);
		//my_amg_manager.maximum_levels = din;
		my_amg_manager.maximum_delete_levels_Temperature = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.maximum_delete_levels_Speed = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.maximum_delete_levels_Pressure = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.maximum_delete_levels_Stress = din;

		// type interpolation procedure :
		//fscanf_s(fp, "%d", &din);
		//my_amg_manager.number_interpolation_procedure = din;

		fscanf_s(fp, "%lld", &din);
		my_amg_manager.number_interpolation_procedure_Temperature = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.number_interpolation_procedure_Speed = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.number_interpolation_procedure_Pressure = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.number_interpolation_procedure_Stress = din;

		fscanf_s(fp, "%lld", &din);
		my_amg_manager.iCFalgorithm_and_data_structure_Temperature = din;// 3-Treap.
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.iCFalgorithm_and_data_structure_Speed = din;// 3-Treap.
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.iCFalgorithm_and_data_structure_Pressure = din;// 3-Treap.
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.iCFalgorithm_and_data_structure_Stress = din;// 3-Treap.

		fscanf_s(fp, "%lld", &din);
		my_amg_manager.itypemodifyinterpol = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.inumberadaptpass = din;


		// 23.02.2018
		// print matrix portrait
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.bTemperatureMatrixPortrait = din; // 0 - NO_PRINT, 1 - PRINT.
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.bSpeedMatrixPortrait = din; // 0 - NO_PRINT, 1 - PRINT.
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.bPressureMatrixPortrait = din; // 0 - NO_PRINT, 1 - PRINT.
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.bStressMatrixPortrait = din; // 0 - NO_PRINT, 1 - PRINT.

		// truncation of interpolation:
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.itruncation_interpolation_Temperature = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.itruncation_interpolation_Speed = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.itruncation_interpolation_Pressure = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.itruncation_interpolation_Stress = din;
		fscanf_s(fp, "%f", &fin);
		my_amg_manager.truncation_interpolation_Temperature = fin;
		fscanf_s(fp, "%f", &fin);
		my_amg_manager.truncation_interpolation_Speed = fin;
		fscanf_s(fp, "%f", &fin);
		my_amg_manager.truncation_interpolation_Pressure = fin;
		fscanf_s(fp, "%f", &fin);
		my_amg_manager.truncation_interpolation_Stress = fin;

		// number nFinnest sweeps :
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.nFinnest_Temperature = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.nFinnest_Speed = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.nFinnest_Pressure = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.nFinnest_Stress = din;

		// number presweeps:
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.nu1_Temperature = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.nu1_Speed = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.nu1_Pressure = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.nu1_Stress = din;

		// number postsweeps :
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.nu2_Temperature = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.nu2_Speed = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.nu2_Pressure = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.nu2_Stress = din;

		// memory size :
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.memory_size_Temperature = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.memory_size_Speed = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.memory_size_Pressure = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.memory_size_Stress = din;


		// �������� ������� ���������� � ������������.
		fscanf_s(fp, "%f", &fin);
		my_amg_manager.gold_const_Temperature = fin;
		fscanf_s(fp, "%f", &fin);
		my_amg_manager.gold_const_Speed = fin;
		fscanf_s(fp, "%f", &fin);
		my_amg_manager.gold_const_Pressure = fin;
		fscanf_s(fp, "%f", &fin);
		my_amg_manager.gold_const_Stress = fin;

		// ������������ �� ilu2 smoother.
		// ������������ �� ilu2 smoother.
		fscanf_s(fp, "%lld", &din);
		if (din == 3) {
			din = 0;
			my_amg_manager.ilu2_smoother_Temperature = din;
			my_amg_manager.b_gmresTemp = true;
		}
		else {
			my_amg_manager.ilu2_smoother_Temperature = din;
		}
		fscanf_s(fp, "%lld", &din);
		if (din == 3) {
			din = 0;
			my_amg_manager.ilu2_smoother_Speed = din;
			my_amg_manager.b_gmresSpeed = true;
		}
		else {
			my_amg_manager.ilu2_smoother_Speed = din;
		}
		fscanf_s(fp, "%lld", &din);
		if (din == 3) {
			din = 0;
			my_amg_manager.ilu2_smoother_Pressure = din;
			my_amg_manager.b_gmresPressure = true;
		}
		else {
			my_amg_manager.ilu2_smoother_Pressure = din;
		}
		fscanf_s(fp, "%lld", &din);
		if (din == 3) {
			din = 0;
			my_amg_manager.ilu2_smoother_Stress = din;
			my_amg_manager.b_gmresStress = true;
		}
		else {
			my_amg_manager.ilu2_smoother_Stress = din;
		}

		// strength threshold :
		fscanf_s(fp, "%f", &fin);
		my_amg_manager.theta_Temperature = fin;
		fscanf_s(fp, "%f", &fin);
		my_amg_manager.theta_Speed = fin;
		fscanf_s(fp, "%f", &fin);
		my_amg_manager.theta_Pressure = fin;
		fscanf_s(fp, "%f", &fin);
		my_amg_manager.theta_Stress = fin;

		// magic threshold :
		//fscanf_s(fp, "%f", &fin);
		//my_amg_manager.magic = fin;

		// magic <=> F_to_F
		fscanf_s(fp, "%f", &fin);
		my_amg_manager.F_to_F_Temperature = fin;
		fscanf_s(fp, "%f", &fin);
		my_amg_manager.F_to_F_Speed = fin;
		fscanf_s(fp, "%f", &fin);
		my_amg_manager.F_to_F_Pressure = fin;
		fscanf_s(fp, "%f", &fin);
		my_amg_manager.F_to_F_Stress = fin;

		// AMG Splitting (coarsening)
		// ������ ���������� C-F ��������� : 0 - standart, 1 - RS 2.
		// RS 2 ���������� ������ ���������� C-F ��������� ���������� ������ ������.
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.icoarseningTemp = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.icoarseningSpeed = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.icoarseningPressure = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.icoarseningStress = din;

		// ���� din==0 �� ������ �������������� ������������� ����� ��� ����������� ���������� ��������������� �������,
		// ���� din==1, Stabilization BiCGStab.
		// 8.01.2017 ����� ��� ��� ������ BiCGStab 
		// ����������������� ��������������� ������������� �������.
		// 9.01.2018 ���� din==2, FGMRes ����������������� �������������� ������������� �������.
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.istabilizationTemp = din; // 0 - none
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.istabilizationSpeed = din; // 0 - none
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.istabilizationPressure = din; // 0 - none
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.istabilizationStress = din; // 0 - none
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.ipatch_number = din; // 0 - ���� �� �����������.

											// ������ ���� �� �������.
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.iprint_log_Temperature = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.iprint_log_Speed = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.iprint_log_Pressure = din;
		fscanf_s(fp, "%lld", &din);
		my_amg_manager.iprint_log_Stress = din;

		fscanf_s(fp, "%d", &din);
		my_amg_manager.lfil = din;

		fscanf_s(fp, "%lld", &din);
		ireconstruction_free_construct_alloc = din;

		fscanf_s(fp, "%lld", &din);
		ianimation_write_on = din;

		// ��������� ����������� ������.
		gtdps = new TEMP_DEP_POWER[ltdp];
		matlist = new TPROP[lmatmax];
		b = new BLOCK[lb];
		s = new SOURCE[ls];
		w = new WALL[lw];
		integer i = 0; // ������� ����� for

		for (i = 0; i < ltdp; i++) {
			// ���������� ��� ������.
			gtdps[i].sname = new char[100]; // ��������� ������
			fscanf_s(fp, "%s", gtdps[i].sname, 100);
			//printf("%s",gtdps[i].sname);
			//getchar();
			// ���������� ������� � ������.
			my_read_power_table(gtdps[i].sname, gtdps[i].intemp, gtdps[i].inoffset_drain, gtdps[i].rtemp, gtdps[i].roffset_drain, gtdps[i].rpower_table);
			printf("printeger table\n"); // debug
			my_print_table(gtdps[i].intemp, gtdps[i].inoffset_drain, gtdps[i].rtemp, gtdps[i].roffset_drain, gtdps[i].rpower_table);
			printf("Please, press any key to continue...\n");
			//getchar();
			system("pause");

		}



		// ���������� ���� ����������
		for (i = 0; i < lmatmax; i++) {
			// �������� ����������:
			// ���������
			fscanf_s(fp, "%f", &fin);
			matlist[i].rho = fin;
			// ����������� ��� ���������� ��������
			//fscanf(fp, "%f", &fin);
			//matlist[i].cp = fin;
			fscanf_s(fp, "%lld", &din);
			matlist[i].n_cp = din;
			matlist[i].arr_cp = NULL;
			matlist[i].temp_cp = NULL;
			matlist[i].arr_cp = new doublereal[matlist[i].n_cp];
			matlist[i].temp_cp = new doublereal[matlist[i].n_cp];
			if (matlist[i].temp_cp == NULL) {
				printf("problem memory allocation for temp_cp\n");
				system("pause");
				exit(1);
			}
			if (matlist[i].arr_cp == NULL) {
				printf("problem memory allocation for arr_cp\n");
				system("pause");
				exit(1);
			}
			for (integer i_4 = 0; i_4 < matlist[i].n_cp; i_4++) {
				// ����������� � C.
				fscanf_s(fp, "%f", &fin);
				matlist[i].temp_cp[i_4] = fin;
				fscanf_s(fp, "%f", &fin);
				matlist[i].arr_cp[i_4] = fin;
			}
			// ����������������
			//fscanf(fp, "%f", &fin);
			//matlist[i].lam = fin;
			fscanf_s(fp, "%lld", &din);
			matlist[i].n_lam = din;
			matlist[i].arr_lam = NULL;
			matlist[i].temp_lam = NULL;
			matlist[i].arr_lam = new doublereal[matlist[i].n_lam];
			matlist[i].temp_lam = new doublereal[matlist[i].n_lam];
			if (matlist[i].temp_lam == NULL) {
				printf("problem memory allocation for temp_lam\n");
				system("pause");
				exit(1);
			}
			if (matlist[i].arr_lam == NULL) {
				printf("problem memory allocation for arr_lam\n");
				system("pause");
				exit(1);
			}
			for (integer i_4 = 0; i_4 < matlist[i].n_lam; i_4++) {
				// ����������� � C.
				fscanf_s(fp, "%f", &fin);
				matlist[i].temp_lam[i_4] = fin;
				fscanf_s(fp, "%f", &fin);
				matlist[i].arr_lam[i_4] = fin;
			}
			// ������������� ���������������� :
			fscanf_s(fp, "%f", &fin);
			matlist[i].orthotropy_multiplyer_x = fin;
			fscanf_s(fp, "%f", &fin);
			matlist[i].orthotropy_multiplyer_y = fin;
			fscanf_s(fp, "%f", &fin);
			matlist[i].orthotropy_multiplyer_z = fin;
			// 5.08.2017.
			// ������������ ��� ������ ���������.
			// ������ ���� � ����������� ��������.
			// �������� ���. 232 �.�.�������, �.�. �������� ����� �������� ��������� � ������� ����������.
			//doublereal Poissonratio = 0.33;
			//doublereal Youngmodule = 71.7e9;
			// steel
			// Tensile Yield Strength 4.12E+8
			// Compressive Yield Strength 4.12E+8
			// Tensile Ultimate Strength 5.7E+8
			//doublereal Poissonratio = 0.154;
			//doublereal Youngmodule = 217.5E9;
			doublereal Poissonratio = 0.3;
			doublereal Youngmodule = 200.0E9;
			fscanf_s(fp, "%f", &fin);
			Poissonratio = fin;
			fscanf_s(fp, "%f", &fin);
			Youngmodule = fin*1e9;
			fscanf_s(fp, "%f", &fin);
			// TODO beta_t*1E-6
			// ������������ ����.
			//doublereal E1_koef = Youngmodule / (1.0 - Poissonratio*Poissonratio);
			//doublereal nu1_koef = Poissonratio / (1.0 - Poissonratio);
			//matlist[i].mu_Lame = E1_koef / (2.0*(1.0 + nu1_koef));
			//matlist[i].lambda_Lame = (E1_koef*nu1_koef) / (1.0 - nu1_koef*nu1_koef);
			// ���. 25 �.�.�������, �.�. �������� ����� �������� ��������� � ������� ����������.
			matlist[i].mu_Lame = Youngmodule / (2.0*(1.0+ Poissonratio));
		    matlist[i].lambda_Lame = (Poissonratio*Youngmodule) / ((1.0+ Poissonratio)*(1.0-2.0*Poissonratio));
			//printf("E=%e N/m^2 mu=%e lambda=%e\n", Youngmodule, matlist[i].mu_Lame, matlist[i].lambda_Lame);
			//getchar();
			// ����������� ������������ ��������
			fscanf_s(fp, "%f", &fin);
			matlist[i].mu = fin;
			// ����������� ��������� �������������� ����������
			fscanf_s(fp, "%f", &fin);
			matlist[i].beta_t = fin;
			// ������� �������������� ���������
			fscanf_s(fp, "%lld", &din);
			matlist[i].blibmat = din;
			// ����� ��������� � ����������
			fscanf_s(fp, "%lld", &din);
			matlist[i].ilibident = din;
			//printf("blibmat=%d ilibident=%d\n", matlist[i].blibmat, matlist[i].ilibident);
			//system("pause");

			// ��� ������� �� ������� �����,
			// ���� ������ �� �� ������������� ������� �������
			// ����� ���� ������������ ����������� 
			// ��������-���������� � ��������������� ������� ������������ Tref.
			fscanf_s(fp, "%lld", &din);
			switch (din) {
			case 0: matlist[i].bBussineskApproach = false; break;
			case 1: matlist[i].bBussineskApproach = true; break;
			default: matlist[i].bBussineskApproach = false; break;
			}
			// ����� ������ ��� ����������� ������������ �������� �� ���������� ������
			fscanf_s(fp, "%lld", &din);
			matlist[i].ilawmu = din;
			// ����������� �������� ������������ ��������
			fscanf_s(fp, "%f", &fin);
			matlist[i].mumin = fin;
			// ������������ �������� ������������ ��������
			fscanf_s(fp, "%f", &fin);
			matlist[i].mumax = fin;
			// ��������� ��������� ������� ��� ����������� �������� �� ���������� ������
			fscanf_s(fp, "%f", &fin);
			matlist[i].Amu = fin;
			fscanf_s(fp, "%f", &fin);
			matlist[i].Bmu = fin;
			fscanf_s(fp, "%f", &fin);
			matlist[i].Cmu = fin;
			// ���������� �������
			fscanf_s(fp, "%f", &fin);
			matlist[i].degreennmu = fin;

			// ������ ��������� �������� �� �������
			//printf("%e %e %e %e %e\n", matlist[i].rho, matlist[i].cp, matlist[i].lam, matlist[i].mu, matlist[i].beta_t);
			printf("cp\n");
			printf("t_C cp\n");
			for (integer i_4 = 0; i_4 < matlist[i].n_cp; i_4++) {
				printf("%e %e\n", matlist[i].temp_cp[i_4], matlist[i].arr_cp[i_4]);
			}
			printf("lam\n");
			printf("t_C lam\n");
			for (integer i_4 = 0; i_4 < matlist[i].n_lam; i_4++) {
				printf("%e %e\n", matlist[i].temp_lam[i_4], matlist[i].arr_lam[i_4]);
			}
			printf("%e %e %e\n", matlist[i].rho, matlist[i].mu, matlist[i].beta_t);
			printf("%lld %lld %lld\n", matlist[i].blibmat, matlist[i].ilibident, matlist[i].ilawmu); // bBoussinesq �� ����������
			printf("%e %e %e %e %e %e\n", matlist[i].mumin, matlist[i].mumax, matlist[i].Amu, matlist[i].Bmu, matlist[i].Cmu, matlist[i].degreennmu);
		}

		// ���������� ������
		for (i = 0; i<lb; i++) {

			fscanf_s(fp, "%lld", &din);
			b[i].iunion_id = din; // 0==�������, ����� ���������� �������� �����������.

			fscanf_s(fp, "%lld", &din);
			b[i].g.itypegeom = din; // 0 - Prism, 1 - Cylinder, 2 - Polygon
			fscanf_s(fp, "%lld", &din);
			if (din == 1) {
				b[i].bvisible = true;
			}
			else {
				b[i].bvisible = false;
			}


			// ���������
			fscanf_s(fp, "%f", &fin);
			b[i].g.xS = scale*fin;
			fscanf_s(fp, "%f", &fin);
			b[i].g.yS = scale*fin;
			fscanf_s(fp, "%f", &fin);
			b[i].g.zS = scale*fin;
			fscanf_s(fp, "%f", &fin);
			b[i].g.xE = scale*fin;
			fscanf_s(fp, "%f", &fin);
			b[i].g.yE = scale*fin;
			fscanf_s(fp, "%f", &fin);
			b[i].g.zE = scale*fin;
			// swap
			if (b[i].g.xS>b[i].g.xE) {
				dbuf = b[i].g.xS;
				b[i].g.xS = b[i].g.xE;
				b[i].g.xE = dbuf;
			}
			if (b[i].g.yS > b[i].g.yE) {
				dbuf = b[i].g.yS;
				b[i].g.yS = b[i].g.yE;
				b[i].g.yE = dbuf;
			}
			if (b[i].g.zS > b[i].g.zE) {
				dbuf = b[i].g.zS;
				b[i].g.zS = b[i].g.zE;
				b[i].g.zE = dbuf;
			}

			// Cylinder

			fscanf_s(fp, "%lld", &din);
			b[i].g.iPlane = din;

			fscanf_s(fp, "%f", &fin);
			b[i].g.xC = scale*fin;
			fscanf_s(fp, "%f", &fin);
			b[i].g.yC = scale*fin;
			fscanf_s(fp, "%f", &fin);
			b[i].g.zC = scale*fin;
			fscanf_s(fp, "%f", &fin);
			b[i].g.Hcyl = scale*fin;
			if (b[i].g.Hcyl < 0.0) {
				// ����������� ������������� ������ ��������.
				switch (b[i].g.iPlane) {
				case XY:
					b[i].g.zC += b[i].g.Hcyl;
					break;
				case XZ:
					b[i].g.yC += b[i].g.Hcyl;
					break;
				case YZ:
					b[i].g.xC += b[i].g.Hcyl;
					break;
				}
				b[i].g.Hcyl = fabs(b[i].g.Hcyl);
			}

			fscanf_s(fp, "%f", &fin);
			b[i].g.R_out_cyl = scale*fin;
			fscanf_s(fp, "%f", &fin);
			b[i].g.R_in_cyl = scale*fin;

			// Polygon
			printf("Polygon\n");
			fscanf_s(fp, "%lld", &din);
			b[i].g.iPlane_obj2 = din;
#if doubleintprecision == 1
			printf("iPlane_obj2=%lld\n", b[i].g.iPlane_obj2);
#else
			printf("iPlane_obj2=%d\n", b[i].g.iPlane_obj2);
#endif			
			fscanf_s(fp, "%lld", &din);
			b[i].g.nsizei = din;
#if doubleintprecision == 1
			printf("nsizei=%lld\n", b[i].g.nsizei);
#else
			printf("nsizei=%d\n", b[i].g.nsizei);
#endif			
			b[i].g.hi = new doublereal[b[i].g.nsizei];
			b[i].g.xi = new doublereal[b[i].g.nsizei];
			b[i].g.yi = new doublereal[b[i].g.nsizei];
			b[i].g.zi = new doublereal[b[i].g.nsizei];
			for (integer i73 = 0; i73 < b[i].g.nsizei; i73++) {
				fscanf_s(fp, "%f", &fin);
				b[i].g.hi[i73] = scale*fin;
				fscanf_s(fp, "%f", &fin);
				b[i].g.xi[i73] = scale*fin;
				fscanf_s(fp, "%f", &fin);
				b[i].g.yi[i73] = scale*fin;
				fscanf_s(fp, "%f", &fin);
				b[i].g.zi[i73] = scale*fin;
				if (b[i].g.hi[i73] < 0.0) {
					// ����������� ������������� ������ ��������.
					switch (b[i].g.iPlane_obj2) {
					case XY:
						b[i].g.zi[i73] += b[i].g.hi[i73];
						break;
					case XZ:
						b[i].g.yi[i73] += b[i].g.hi[i73];
						break;
					case YZ:
						b[i].g.xi[i73] += b[i].g.hi[i73];
						break;
					}
					b[i].g.hi[i73] = fabs(b[i].g.hi[i73]);
				}

#if doubleintprecision == 1
				printf("%lld h=%e x=%e y=%e z=%e",i73, b[i].g.hi[i73], b[i].g.xi[i73], b[i].g.yi[i73], b[i].g.zi[i73]);
#else
				printf("%d h=%e x=%e y=%e z=%e", i73, b[i].g.hi[i73], b[i].g.xi[i73], b[i].g.yi[i73], b[i].g.zi[i73]);
#endif
				//getchar();
			}

			if ((b[i].g.itypegeom==2)&&(b[i].g.nsizei > 0)) {
				// �� ������� � ����������� ������ ������� �������� ����� �������� 
				// ������ � inmodel_temp ������ ������� ������ ��� ������ ��������.
				doublereal xmin53 = 1.0e30;
				doublereal ymin53 = 1.0e30;
				doublereal zmin53 = 1.0e30;
				doublereal xmax53 = -1.0e30;
				doublereal ymax53 = -1.0e30;
				doublereal zmax53 = -1.0e30;
				for (integer i73 = 0; i73 < b[i].g.nsizei; i73++) {
					if (b[i].g.xi[i73] > xmax53) xmax53 = b[i].g.xi[i73];
					if (b[i].g.yi[i73] > ymax53) ymax53 = b[i].g.yi[i73];
					if (b[i].g.zi[i73] > zmax53) zmax53 = b[i].g.zi[i73];
					if (b[i].g.xi[i73] < xmin53) xmin53 = b[i].g.xi[i73];
					if (b[i].g.yi[i73] < ymin53) ymin53 = b[i].g.yi[i73];
					if (b[i].g.zi[i73] < zmin53) zmin53 = b[i].g.zi[i73];
				}
				switch (b[i].g.iPlane_obj2) {
				case XY:
					b[i].g.xS = xmin53;
					b[i].g.xE = xmax53;
					b[i].g.yS = ymin53;
					b[i].g.yE = ymax53;
					b[i].g.zS = zmin53;
					b[i].g.zE = zmin53 + b[i].g.hi[0];
					break;
				case XZ:
					b[i].g.xS = xmin53;
					b[i].g.xE = xmax53;
					b[i].g.zS = zmin53;
					b[i].g.zE = zmax53;
					b[i].g.yS = ymin53;
					b[i].g.yE = ymin53 + b[i].g.hi[0];
					break;
				case YZ:
					b[i].g.yS = ymin53;
					b[i].g.yE = ymax53;
					b[i].g.zS = zmin53;
					b[i].g.zE = zmax53;
					b[i].g.xS = xmin53;
					b[i].g.xE = xmin53 + b[i].g.hi[0];
					break;
				}
			}

			if (b[i].g.itypegeom == 1) {
				// Celinder
				//printf("%e %e %e %e %e %e %e %e %e %e %e %e\n", b[i].g.xS, b[i].g.yS, b[i].g.zS, b[i].g.xE, b[i].g.yE, b[i].g.zE, b[i].g.xC, b[i].g.yC, b[i].g.zC, b[i].g.Hcyl, b[i].g.R_out_cyl, b[i].g.R_in_cyl);
				//getchar();
			}


			// ���� �������������� ����������.
			// emissivity
			fscanf_s(fp, "%f", &fin);
			b[i].radiation.emissW = fin;
			fscanf_s(fp, "%f", &fin);
			b[i].radiation.emissE = fin;
			fscanf_s(fp, "%f", &fin);
			b[i].radiation.emissS = fin;
			fscanf_s(fp, "%f", &fin);
			b[i].radiation.emissN = fin;
			fscanf_s(fp, "%f", &fin);
			b[i].radiation.emissB = fin;
			fscanf_s(fp, "%f", &fin);
			b[i].radiation.emissT = fin;
#if doubleintprecision == 1
			fscanf_s(fp, "%lld", &din);
#else
			fscanf_s(fp, "%d", &din);
#endif
			if (din == 0) {
				// ���� �� �������� ��������� �����������.
				b[i].radiation.binternalRadiation = false;
			}
			else {
				// ���� �������� ��������� �����������.
				b[i].radiation.binternalRadiation = true;
				if (bvacuumPrism) {
					bdouble_vacuum_PRISM = true;
				}
				bvacuumPrism = true;
				// ��������� View Factors.
				calculate_view_factors(b[i]);
			}
			b[i].radiation.nodelistW = NULL;
			b[i].radiation.nodelistE = NULL;
			b[i].radiation.nodelistS = NULL;
			b[i].radiation.nodelistN = NULL;
			b[i].radiation.nodelistB = NULL;
			b[i].radiation.nodelistT = NULL;
			b[i].radiation.nodelistWsize = 0;
			b[i].radiation.nodelistEsize = 0;
			b[i].radiation.nodelistSsize = 0;
			b[i].radiation.nodelistNsize = 0;
			b[i].radiation.nodelistBsize = 0;
			b[i].radiation.nodelistTsize = 0;

			// ������������� ��������� � ���� ����������
#if doubleintprecision == 1
			fscanf_s(fp, "%lld", &din);
#else
			fscanf_s(fp, "%d", &din);
#endif
			b[i].imatid = din;

#if doubleintprecision == 1
			fscanf_s(fp, "%lld", &din);
#else
			fscanf_s(fp, "%d", &din);
#endif
			// bCylinderFixed
			if (din == 1) {
				b[i].CylinderFixed = true;
			}
			else {
				b[i].CylinderFixed = false;
			}

			// �������� ��������������
			//fscanf_s(fp, "%f", &fin);
			//b[i].Sc = fin;
			// 19 november 2016 ������������ ��������� �������� ��������������.
#if doubleintprecision == 1
			fscanf_s(fp, "%lld", &din);
#else
			fscanf_s(fp, "%d", &din);
#endif
			b[i].n_Sc = din;
			b[i].arr_Sc = NULL;
			b[i].temp_Sc = NULL;
			b[i].arr_Sc = new doublereal[b[i].n_Sc];
			b[i].temp_Sc = new doublereal[b[i].n_Sc];
			if (b[i].temp_Sc == NULL) {
				printf("problem memory allocation for temp_Sc\n");
				system("pause");
				exit(1);
			}
			if (b[i].arr_Sc == NULL) {
				printf("problem memory allocation for arr_Sc\n");
				system("pause");
				exit(1);
			}
			for (integer i_4 = 0; i_4 < b[i].n_Sc; i_4++) {
				// ����������� � C.
				fscanf_s(fp, "%f", &fin);
				b[i].temp_Sc[i_4] = fin;
				fscanf_s(fp, "%f", &fin);
				if (fin != fin) {
					b[i].arr_Sc[i_4] = 0.0;
				}
				else {
					b[i].arr_Sc[i_4] = fin;
				}
			}

			// debug
			//if (fabs(b[i].Sc)>1.0e-30) {
			//printf("%e\n", b[i].Sc);
			//getchar();
			//}
			// ����� ����������� �������� �������������� � ����� �� �������.
			// 0 - �� ������� �� �������, 1 - square wave �����������, 
			// 2 - square wave apparat �����������.
#if doubleintprecision == 1
			fscanf_s(fp, "%lld", &din);
#else
			fscanf_s(fp, "%d", &din);
#endif
			b[i].ipower_time_depend = din;
			// ��� �����
#if doubleintprecision == 1
			fscanf_s(fp, "%lld", &din);
#else
			fscanf_s(fp, "%d", &din);
#endif
			b[i].itype = din;

			// ������ ��������� �������� �� �������
			printf("%e %e %e %e %e %e\n", b[i].g.xS, b[i].g.yS, b[i].g.zS, b[i].g.xE, b[i].g.yE, b[i].g.zE);
			printf("%lld %lld %lld\n", b[i].imatid, b[i].itype, b[i].ipower_time_depend);
			printf("temperature depend power\n");
			printf("t_C power_W\n");
			for (integer i_54 = 0; i_54 < b[i].n_Sc; i_54++) {
				printf("%e %e\n", b[i].temp_Sc[i_54], b[i].arr_Sc[i_54]);
			}
		}

		// ���������� ���������� �����
		for (i = 0; i < ls; i++) {

			fscanf_s(fp, "%lld", &din);
			s[i].iunion_id = din; // 0==�������, ����� ���������� �������� �����������.

			fscanf_s(fp, "%f", &fin);
			s[i].power = fin;
#if doubleintprecision == 1
			fscanf_s(fp, "%lld", &din);
#else
			fscanf_s(fp, "%d", &din);
#endif
			if (din == 0) {
				// const
				// ������� ���������� �������� ������������ � ����� ��������.
				s[i].power_multiplyer = 1.0;
				s[i].bgarber_depend = false;
			}
			else if (din == 1) {
				// ������������ � ����� �������� ������� ��������
				// � ����������� �� ������������ ����������� � �������� 
				// �������� �������� �����.
				s[i].bgarber_depend = true;
				s[i].power_multiplyer = s[i].power;
				// �������� ����� ��������� ��� ambient Temperature ����������� ��������� �����.
			}
#if doubleintprecision == 1
			fscanf_s(fp, "%lld", &din);
#else
			fscanf_s(fp, "%d", &din);
#endif
			s[i].igarber_depend = din; // ���������� ����� �������.
			fscanf_s(fp, "%f", &fin);
			s[i].roperation_offset_drain = fin; // ������� �������� �������� �����.
												//printf("offset drain is %e\n",s[i].roperation_offset_drain);
												//getchar();
			bool bsplinereadOk = true;
			if (s[i].bgarber_depend) {
				s[i].power = my_splain_interpol_power_table(gtdps[s[i].igarber_depend].intemp,
					gtdps[s[i].igarber_depend].inoffset_drain,
					gtdps[s[i].igarber_depend].rtemp,
					gtdps[s[i].igarber_depend].roffset_drain,
					gtdps[s[i].igarber_depend].rpower_table,
					operatingtemperature,
					s[i].roperation_offset_drain);
				if (bsplinereadOk) {
					// ��������� �������� �������� ���������� �������������.
					printf("single test validation spline approximation...\n");
					printf("calculate initial power=%e\n", s[i].power);
					printf("please, press any key to continue...");
					// getchar();
					system("pause");

					bsplinereadOk = false;
				}
				s[i].power *= s[i].power_multiplyer; // ���������� �� �������������� ���������.
			}



#if doubleintprecision == 1
			fscanf_s(fp, "%lld", &din);
#else
			fscanf_s(fp, "%d", &din);
#endif
			s[i].iPlane = din;
			// ���������
			fscanf_s(fp, "%f", &fin);
			s[i].g.xS = scale*fin;
			fscanf_s(fp, "%f", &fin);
			s[i].g.yS = scale*fin;
			fscanf_s(fp, "%f", &fin);
			s[i].g.zS = scale*fin;
			fscanf_s(fp, "%f", &fin);
			s[i].g.xE = scale*fin;
			fscanf_s(fp, "%f", &fin);
			s[i].g.yE = scale*fin;
			fscanf_s(fp, "%f", &fin);
			s[i].g.zE = scale*fin;



			// swap
			if (s[i].g.xS > s[i].g.xE) {
				dbuf = s[i].g.xS;
				s[i].g.xS = s[i].g.xE;
				s[i].g.xE = dbuf;
			}
			if (s[i].g.yS > s[i].g.yE) {
				dbuf = s[i].g.yS;
				s[i].g.yS = s[i].g.yE;
				s[i].g.yE = dbuf;
			}
			if (s[i].g.zS > s[i].g.zE) {
				dbuf = s[i].g.zS;
				s[i].g.zS = s[i].g.zE;
				s[i].g.zE = dbuf;
			}
			switch (s[i].iPlane) {
			case XY: s[i].square = fabs(s[i].g.xE - s[i].g.xS)*fabs(s[i].g.yE - s[i].g.yS); break;
			case XZ: s[i].square = fabs(s[i].g.xE - s[i].g.xS)*fabs(s[i].g.zE - s[i].g.zS); break;
			case YZ: s[i].square = fabs(s[i].g.yE - s[i].g.yS)*fabs(s[i].g.zE - s[i].g.zS); break;
			default: break;
			}
			printf("source %e %lld %e %e %e %e %e %e %e\n", s[i].power, s[i].iPlane, s[i].g.xS, s[i].g.yS, s[i].g.zS, s[i].g.xE, s[i].g.yE, s[i].g.zE, s[i].square);
		}

		// ���������� ������ ������



		for (i = 0; i < lw; i++) {

			fscanf_s(fp, "%lld", &din);
			w[i].iunion_id = din; // 0==�������, ����� ���������� �������� �����������.

#if doubleintprecision == 1
			fscanf_s(fp, "%lld", &din);
#else
			fscanf_s(fp, "%d", &din);
#endif
			w[i].ifamily = din;
			switch (din) {
			case 1:  fscanf_s(fp, "%f", &fin);
				w[i].Tamb = fin;
				fscanf_s(fp, "%f", &fin); // Stefan Bolcman
										  // termostability wall
				w[i].emissivity = 0.0;
				w[i].film_coefficient = 0.0;
				fscanf_s(fp, "%f", &fin);
				w[i].hf = 0.0;
				break; // ������� ����
			case 2:  fscanf_s(fp, "%f", &fin);
				w[i].Tamb = 0.0;
				fscanf_s(fp, "%f", &fin);  // Stefan Bolcman
										   // adiabatic wall
				w[i].emissivity = 0.0;
				w[i].film_coefficient = 0.0;
				fscanf_s(fp, "%f", &fin);
				w[i].hf = 0.0;
				break; // ���������� ������� �������
			case 3:  fscanf_s(fp, "%f", &fin);
				w[i].Tamb = fin;
				fscanf_s(fp, "%f", &fin); // Stefan Bolcman
										  // Newton-Richman condition, film coefficient.
				w[i].emissivity = 0.0;
				w[i].film_coefficient = fin;
				fscanf_s(fp, "%f", &fin);
				w[i].hf = 0.0;
				break; // ������-������.
			case 4:  fscanf_s(fp, "%f", &fin);
				w[i].Tamb = fin;
				fscanf_s(fp, "%f", &fin); // Stefan Bolcman
										  // Stefan - Bolcman condition
				w[i].emissivity = fin;
				w[i].film_coefficient = 0.0;
				fscanf_s(fp, "%f", &fin);
				w[i].hf = 0.0;
				break; // ������-��������.
			default: 
				printf("error: wall unlnown boundary condition type.\n");
				getchar();
				break;
			}
#if doubleintprecision == 1
			fscanf_s(fp, "%lld", &din);
#else
			fscanf_s(fp, "%d", &din);
#endif
			if (din == 1) w[i].bsymmetry = true; else w[i].bsymmetry = false;
#if doubleintprecision == 1
			fscanf_s(fp, "%lld", &din);
#else
			fscanf_s(fp, "%d", &din);
#endif
			if (din == 1) w[i].bpressure = true; else w[i].bpressure = false;
#if doubleintprecision == 1
			fscanf_s(fp, "%lld", &din);
#else
			fscanf_s(fp, "%d", &din);
#endif
			if (din == 1) w[i].bopening = true; else w[i].bopening = false;
			fscanf_s(fp, "%f", &fin);
			w[i].Vx = fin;
			fscanf_s(fp, "%f", &fin);
			w[i].Vy = fin;
			fscanf_s(fp, "%f", &fin);
			w[i].Vz = fin;
			fscanf_s(fp, "%f", &fin);
			w[i].P = fin;
#if doubleintprecision == 1
			fscanf_s(fp, "%lld", &din);
#else
			fscanf_s(fp, "%d", &din);
#endif		
			if (din >= 0 && din < 11) {
				// 0- FREE
				w[i].ithermal_Stress_boundary_condition = din;
			}
			else {
				printf("error: unknown ithermal_Stress_boundary_condition\n");
				printf("ithermal_Stress_boundary_condition=%d\n", din);
				getchar();
				w[i].ithermal_Stress_boundary_condition = 7; // Fixit all
			}
			fscanf_s(fp, "%f", &fin);
			w[i].xForce = fin;
			fscanf_s(fp, "%f", &fin);
			w[i].yForce = fin;
			fscanf_s(fp, "%f", &fin);
			w[i].zForce = fin;
#if doubleintprecision == 1
			fscanf_s(fp, "%lld", &din);
#else
			fscanf_s(fp, "%d", &din);
#endif
			w[i].iPlane = din;
			// ���������
			fscanf_s(fp, "%f", &fin);
			w[i].g.xS = scale*fin;
			fscanf_s(fp, "%f", &fin);
			w[i].g.yS = scale*fin;
			fscanf_s(fp, "%f", &fin);
			w[i].g.zS = scale*fin;
			fscanf_s(fp, "%f", &fin);
			w[i].g.xE = scale*fin;
			fscanf_s(fp, "%f", &fin);
			w[i].g.yE = scale*fin;
			fscanf_s(fp, "%f", &fin);
			w[i].g.zE = scale*fin;
			// swap
			if (w[i].g.xS > w[i].g.xE) {
				dbuf = w[i].g.xS;
				w[i].g.xS = w[i].g.xE;
				w[i].g.xE = dbuf;
			}
			if (w[i].g.yS > w[i].g.yE) {
				dbuf = w[i].g.yS;
				w[i].g.yS = w[i].g.yE;
				w[i].g.yE = dbuf;
			}
			if (w[i].g.zS > w[i].g.zE) {
				dbuf = w[i].g.zS;
				w[i].g.zS = w[i].g.zE;
				w[i].g.zE = dbuf;
			}
			printf("wall %lld %e %e %lld %e %e %e %e %e %e\n", w[i].ifamily, w[i].Tamb, w[i].hf, w[i].iPlane, w[i].g.xS, w[i].g.yS, w[i].g.zS, w[i].g.xE, w[i].g.yE, w[i].g.zE);
		}


		// ���������.
#if doubleintprecision == 1
		fscanf_s(fp, "%lld", &din);
#else
		fscanf_s(fp, "%d", &din);
#endif
		lu = din;
		if (lu == 0) {
			my_union = NULL;
		}
		else {
			my_union = new UNION[lu];
			// �������������.
			for (i = 0; i < lu; i++) {
				my_union[i].f = NULL;
				my_union[i].xpos = NULL;
				my_union[i].ypos = NULL;
				my_union[i].zpos = NULL;
				my_union[i].xposadd = NULL;
				my_union[i].yposadd = NULL;
				my_union[i].zposadd = NULL;
				my_union[i].iswitchMeshGenerator = 2; // 2 - CoarseMeshGen
				my_union[i].inxadd = -1;
				my_union[i].inyadd = -1;
				my_union[i].inzadd = -1;
				my_union[i].flow_interior = 0;
			}
		}
		for (i = 0; i < lu; i++) {
			fscanf_s(fp, "%f", &fin);
			my_union[i].xS = scale*fin;
			fscanf_s(fp, "%f", &fin);
			my_union[i].xE = scale*fin;
			fscanf_s(fp, "%f", &fin);
			my_union[i].yS = scale*fin;
			fscanf_s(fp, "%f", &fin);
			my_union[i].yE = scale*fin;
			fscanf_s(fp, "%f", &fin);
			my_union[i].zS = scale*fin;
			fscanf_s(fp, "%f", &fin);
			my_union[i].zE = scale*fin;

#if doubleintprecision == 1
			fscanf_s(fp, "%lld", &din);
#else
			fscanf_s(fp, "%d", &din);
#endif
			my_union[i].id = din; // ���������� ������������� ����������.
#if doubleintprecision == 1
			fscanf_s(fp, "%lld", &din);
#else
			fscanf_s(fp, "%d", &din);
#endif
			my_union[i].inx = din;
#if doubleintprecision == 1
			fscanf_s(fp, "%lld", &din);
#else
			fscanf_s(fp, "%d", &din);
#endif
			my_union[i].iny = din;
#if doubleintprecision == 1
			fscanf_s(fp, "%lld", &din);
#else
			fscanf_s(fp, "%d", &din);
#endif
			my_union[i].inz = din;

		}

		// ���������� ���������� � ������ �������� ���������
#if doubleintprecision == 1
		fscanf_s(fp, "%lld", &din);
#else
		fscanf_s(fp, "%d", &din);
#endif
		eqin.itemper = din;
#if doubleintprecision == 1
		fscanf_s(fp, "%lld", &din);
#else
		fscanf_s(fp, "%d", &din);
#endif
		eqin.imaxflD = din;
		printf("itemper=%lld eqin.imaxflD=%lld\n", eqin.itemper, eqin.imaxflD);
		//getchar();
		if (eqin.imaxflD == 0) {
			eqin.fluidinfo = NULL;
		}
		else
		{
			// ��������� ����������� ������
			if (eqin.fluidinfo != NULL) {
				delete eqin.fluidinfo;
				eqin.fluidinfo = NULL;
			}
			eqin.fluidinfo = new FLOWINFO[eqin.imaxflD];
			for (i = 0; i < eqin.imaxflD; i++) {
				// ���������� ��������� ������� �����
				fscanf_s(fp, "%f", &fin);
				eqin.fluidinfo[i].xc = scale*fin;
				fscanf_s(fp, "%f", &fin);
				eqin.fluidinfo[i].yc = scale*fin;
				fscanf_s(fp, "%f", &fin);
				eqin.fluidinfo[i].zc = scale*fin;
#if doubleintprecision == 1
				fscanf_s(fp, "%lld", &din);
#else
				fscanf_s(fp, "%d", &din);
#endif
				eqin.fluidinfo[i].iflow = din;
#if doubleintprecision == 1
				fscanf_s(fp, "%lld", &din);
#else
				fscanf_s(fp, "%d", &din);
#endif
				eqin.fluidinfo[i].iflowregime = din;
#if doubleintprecision == 1
				fscanf_s(fp, "%lld", &din);
#else
				fscanf_s(fp, "%d", &din);
#endif
				eqin.fluidinfo[i].iturbmodel = din;
				fscanf_s(fp, "%f", &fin);
				eqin.fluidinfo[i].Cs = fin; // ���������� �������������.
#if doubleintprecision == 1
				fscanf_s(fp, "%lld", &din);
#else
				fscanf_s(fp, "%d", &din);
#endif
				// ���� ������������� ����������� �������� ���������� �������������.
				// �������� Dynamic Subgrid Scale Model ������� 1991 ����.
				if (din == 1) {
					eqin.fluidinfo[i].bDynamic_Stress = true;
				}
				else {
					eqin.fluidinfo[i].bDynamic_Stress = false;
				}
#if doubleintprecision == 1
				fscanf_s(fp, "%lld", &din);
#else
				fscanf_s(fp, "%d", &din);
#endif
				// �������� ����������� ������ � ����� �� ��������� �������� ���������� �������������.
				if (din == 1) {
					eqin.fluidinfo[i].bLimiters_Cs = true;
				}
				else {
					eqin.fluidinfo[i].bLimiters_Cs = false;
				}
				fscanf_s(fp, "%f", &fin);
				eqin.fluidinfo[i].rminCs = fin; // ����������� ��������� �������� ���������� �������������.
				fscanf_s(fp, "%f", &fin);
				eqin.fluidinfo[i].rmaxCs = fin; // ������������ ��������� �������� ���������� �������������.
#if doubleintprecision == 1
				fscanf_s(fp, "%lld", &din);
#else
				fscanf_s(fp, "%d", &din);
#endif
				eqin.fluidinfo[i].itypeFiltrGermano = din; // ��� ������� � ������ ������� 1991 ����.
				fscanf_s(fp, "%f", &fin);
				eqin.fluidinfo[i].roughness = 1.0e-6*fin; // ������������� ������ � �.
				fscanf_s(fp, "%f", &fin);
				eqin.fluidinfo[i].rRi_mult = fin; // ��������� �������������� ������������ ����� ����������.
				fscanf_s(fp, "%f", &fin);
				eqin.fluidinfo[i].rSelectiveAngle = fin; // ��������� �������� ���� � ������ Selective Smagorinsky.
#if doubleintprecision == 1
				fscanf_s(fp, "%lld", &din);
#else
				fscanf_s(fp, "%d", &din);
#endif
				eqin.fluidinfo[i].ipowerroughness = din; // ���������� ������� � ������ ����� �������������.
#if doubleintprecision == 1
				fscanf_s(fp, "%lld", &din);
#else
				fscanf_s(fp, "%d", &din);
#endif
				eqin.fluidinfo[i].itypeSelectiveSmagorinsky_filtr = din; // ��� ������� � ������ Selective Smagorinsky.
#if doubleintprecision == 1
				fscanf_s(fp, "%lld", &din);
#else
				fscanf_s(fp, "%d", &din);
#endif
				eqin.fluidinfo[i].bfdelta = din; // ���� ��������������� �����.
#if doubleintprecision == 1
				fscanf_s(fp, "%lld", &din);
#else
				fscanf_s(fp, "%d", &din);
#endif
				eqin.fluidinfo[i].bSmagorinskyLilly = din; // ������ �������������-�����.
#if doubleintprecision == 1
				fscanf_s(fp, "%lld", &din);
#else
				fscanf_s(fp, "%d", &din);
#endif
				eqin.fluidinfo[i].bsurface_roughness = din; // ���� ������������� ������.
#if doubleintprecision == 1
				fscanf_s(fp, "%lld", &din);
#else
				fscanf_s(fp, "%d", &din);
#endif
				// ���� ������� � ��������� ����� ����.
				if (din == 1) {
					eqin.fluidinfo[i].bSwirlAmendment = true;
				}
				else {
					eqin.fluidinfo[i].bSwirlAmendment = false;
				}
#if doubleintprecision == 1
				fscanf_s(fp, "%lld", &din);
#else
				fscanf_s(fp, "%d", &din);
#endif
				// ���� ��������������� � ������ �������������
				if (din == 1) {
					eqin.fluidinfo[i].bSelectiveSmagorinsky = true;
				}
				else {
					eqin.fluidinfo[i].bSelectiveSmagorinsky = false;
				}

			}
		}

		fclose(fp); // �������� �����
	}
}

#endif

	printf("OK. \n");
} // premeshin

#endif