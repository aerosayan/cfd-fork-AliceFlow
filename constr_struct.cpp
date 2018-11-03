// ���� constr_struct.cpp �������� ���������
// ��� ������� ������ ����������� �����������.
// ������ ������ �������� �� ���������� ����������������� ����� � ������������ ��������.
// ���������� ������� ������ �������������, �� ����� ������������ ����� ��� ������
// ��������������� ������� �� ������ constr_struct_alice.cpp �� ���������� �������� ������������ ����� ����.
// 26.09.2016 ��������� ������� ������ � �������� ���������.


#ifndef _CONSTR_STRUCT_CPP_
#define _CONSTR_STRUCT_CPP_ 1


#define TEMPERATURE 0
#define HYDRODINAMIC 1


#define RHO 0 // ���������
#define CP 1 // ����������� ��� ���������� ��������
#define LAM 2 // ����������������
#define MULT_LAM_X 3
#define MULT_LAM_Y 4
#define MULT_LAM_Z 5
#define MU_LAME 6  // ����������� ����
#define LAMBDA_LAME 7 // ����������� ����.


#define MU 1 // ������������ ��������
#define BETA_T 2 // ����������� ��������� �������������� ����������

#define VX 0 // �������������� �������� (����������� ������ ���������� � ����)
#define VY 1 // ������������ ��������
#define VZ 2
#define PRESS 3 // ��������
#define PAM  4 // �������� ��������
#define VXCOR 5 // �������� � ����������  
#define VYCOR 6 // �������� ���������������
#define VZCOR 7 // ��������� �������������
// ��������� ��������� � ������� ��:
#define GRADXVX 8
#define GRADYVX 9
#define GRADZVX 10
#define GRADXVY 11
#define GRADYVY 12
#define GRADZVY 13
#define GRADXVZ 14
#define GRADYVZ 15
#define GRADZVZ 16
// ����������� ������������ ������������ ��������
#define MUT 17
#define CURL 18 // ������ ������ �������� (������������).
#define FBUF 19 // ����� � ������� ����� ��������� ������ ������������ ������ ��������� �������: ������������� ��������� ������������� � ������������ � �.�.
// ��������� �������� ��������.
#define GRADXPAM 20
#define GRADYPAM 21
#define GRADZPAM 22
// ��������� �������� ������������ ��� ���������� �������� ���-���.
#define GRADXPRESS 23
#define GRADYPRESS 24
#define GRADZPRESS 25
#define PAMOLDITER 26
#define TOTALDEFORMATIONVAR 27
#define SPEED 28 // ������ �������� �������������.
// Static Structural
// ����������:
#define TOTALDEFORMATION 0 // ������
#define XDEFORMATION 1 // � ����������
#define YDEFORMATION 2 // y ����������
#define ZDEFORMATION 3 // z ����������
// von-Mises


#define SPEED 99 // velocity magnitude (������ ��������) ��������� ������������ ��� �������� ��������.
#define TEMP 100 // ��������� ����������������


#define ENUMERATECONTVOL 0 // ���� ��������� ��
#define MASKDOMAINFLUID 1 // ���� ����� ���������� ���� FLUID
#define MASKDOMAINFLUIDCOLOR 2 // � evt_f2 ���� ����������������� ����������.

#define LAMINAR 0 // ���������� �������
#define ZEROEQMOD 1 // ������������ ������� - �������������� ������ �������������� Zero Equation Model
#define SMAGORINSKY 2 // ������������ ������� - LES ������������� ���� �� �������������� ������ �������������.
#define RNG_LES 3 // Based on Renormalization Group Theory. (������ ������������� �������� CFD-Wiki).
// ������������ ������ ������� 1991 ����. (������������ �� ������ ������������� � ����������� � ���� � ����� - bDynamic_Stress).

typedef struct TTOCKA_INT {
	integer i, j, k;
} TOCKA_INT;

// ���������� ������� ��� ������� ����� ���������� ����.
// �������� ���������� �����
// ������� ���������� ������� � ��������� tecplot360
// ����� 1 � 3.
void exporttecplotxy360T_3D_part1and3(TEMPER &t, integer maxelm, integer maxbound, bool bextendedprint, integer ncell, integer** nvtx, integer** nvtxcell, TOCHKA* pa, BOUND* sosedb, integer ivarexport, integer** ptr_out);
// ���������� ������� ��� ������� ����� ���������� �����
void xyplot( FLOW* &fglobal, integer flow_interior, TEMPER &t);

// ���������� ������� ��� ������� ����� ���������� ������� �����
// �� ������ ��������� � ������ shortest_distance.cpp.
// ���������� ���������� �� ��������� ������ ���� ������� ����
// � ������������ ������������ ����������� ��������.
// ���� �������� ���������� � ANSYS CFX.
void calcdistwallCFX(FLOW &f, integer ls, integer lw, WALL* w);

// ����������� ������ ������ 1
// ��� ��������� ����������������.
void free_level1_temp(TEMPER &t) {

	integer i = 0;

	printf("delete temperature sosedi\n");
	if (t.sosedi != NULL) {
		for (i = 0; i<12; i++) {
			if (t.sosedi[i] != NULL) {
				delete[] t.sosedi[i]; // -12N
				t.sosedi[i] = NULL;
			}
		}
	}
	if (t.sosedi != NULL) {
		delete[] t.sosedi;
		t.sosedi = NULL;
	}

	if (t.whot_is_block != NULL) {
		delete[] t.whot_is_block;
		t.whot_is_block = NULL;
	}

	printf("delete temperature Sc\n");
	if (t.Sc != NULL) {
		delete[] t.Sc; // -N
		t.Sc = NULL;
	}

	printf("delete temperature ipower_time_depend\n");
	if (t.ipower_time_depend != NULL) {
		delete[] t.ipower_time_depend; // -N
		t.ipower_time_depend = NULL;
	}

	printf("delete temperature pa\n");
	if (t.pa != NULL) {
		delete[] t.pa; // -3N
		t.pa = NULL;
	}

	printf("delete temperature sosedb\n");
	if (t.sosedb != NULL) {
		delete[] t.sosedb;
		t.sosedb = NULL;
	}

	

	printf("delete temperature binternal source\n");
	if (t.binternalsource != NULL) {
		delete[] t.binternalsource;
		t.binternalsource = NULL;
	}

	printf("delete temperature prop\n");
	if (t.prop != NULL) {
		for (i = 0; i<8; i++) {
			if (t.prop[i] != NULL) {
				delete[] t.prop[i]; // -3N
			}
		}
	}
	if (t.prop != NULL) {
		delete[] t.prop;
		t.prop = NULL;
	}

	printf("delete temperature ptr\n");
	if (t.ptr != NULL) {
		for (i = 0; i<2; i++) {
			if (t.ptr[i] != NULL) {
				delete[] t.ptr[i];
				t.ptr[i] = NULL;
			}
		}
		delete[] t.ptr;
		t.ptr = NULL;
	}

	printf("delete temperature prop_b\n");
	if (t.prop_b != NULL) {
		for (i = 0; i<6; i++) {
			if (t.prop_b[i] != NULL) {
				delete[] t.prop_b[i]; // -3N
				t.prop_b[i] = NULL;
			}
		}
	}
	if (t.prop_b != NULL) {
		delete[] t.prop_b;
		t.prop_b = NULL;
	}

	printf("delete temperature nvtx and nvtxcell\n");
	for (i = 0; i<8; i++) { // -8N
		if (t.nvtx != NULL) {
			if (t.nvtx[i] != NULL) {
				delete[] t.nvtx[i];
				t.nvtx[i] = NULL;
			}
		}
		if (t.nvtxcell != NULL) {
			if (t.nvtxcell[i] != NULL) {
				delete[] t.nvtxcell[i]; // ����� ���� ��� �������
				t.nvtxcell[i] = NULL;
			}
		}
	}
	if (t.nvtx != NULL) {
		delete[] t.nvtx;
		t.nvtx = NULL;
	}
	if (t.nvtxcell != NULL) {
		delete[] t.nvtxcell;
		t.nvtxcell = NULL;
	}

	// 26_09_2016.
	printf("delete ilevel_alice\n");
	if (t.ilevel_alice != NULL) {
		delete[] t.ilevel_alice;
		t.ilevel_alice = NULL;
	}

	// ����� -31N

} // free_level1_temp



// ����������� ������ ������ 2
// ��� ��������� ����������������.
void free_level2_temp(TEMPER &t) {

	// ������������ ����������� ������ ���������� ��� ������� ����.
	// -15N
	printf("delete temperature slau\n");
	if (t.slau != NULL) {
		delete t.slau;
		t.slau = NULL;
	}
	printf("delete temperature slau_bon\n");
	if (t.slau_bon != NULL) {
		delete t.slau_bon;
		t.slau_bon = NULL;
	}

	printf("delete temperature potent\n");
	if (t.potent != NULL) {
		delete t.potent;
		t.potent = NULL;
	}

	printf("delete total deformation \n");
	if (t.total_deformation != NULL) {
		for (integer i_1 = 0; i_1 < 4; i_1++) {
			if (t.total_deformation[i_1] != NULL) {
				delete[] t.total_deformation[i_1];
				t.total_deformation[i_1] = NULL;
			}
		}

		delete[] t.total_deformation;
		t.total_deformation = NULL;
	}
} // free_level2_temp

// ���������� ����������� ����� ������������ ������������ ������
void center_cord3D(integer iP, integer** nvtx, TOCHKA* pa, TOCHKA &p, integer id) {
	// ���������� ����������� ����� ������������ ������������ ������
	if (iP < 0) {
		p.x = 0.0;
		p.y = 0.0;
		p.z = 0.0;
		printf("fatall error in center_cord3D !!! ");
		switch (id) {
		case WSIDE: printf("W"); break;
		case ESIDE: printf("E"); break;
		case NSIDE: printf("N"); break;
		case SSIDE: printf("S"); break;
		case BSIDE: printf("B"); break;
		case TSIDE: printf("T"); break;
		case WW: printf("WW"); break;
		case EE: printf("EE"); break;
		case NN: printf("NN"); break;
		case SS: printf("SS"); break;
		case BB: printf("BB"); break;
		case TTSIDE: printf("TT"); break;
		default: printf("P"); break;
		}
		system("PAUSE");
	}
	else {
		// ���������� ������ ������������ ������
		p.x = 0.5*(pa[nvtx[1][iP] - 1].x + pa[nvtx[0][iP] - 1].x);
		p.y = 0.5*(pa[nvtx[2][iP] - 1].y + pa[nvtx[0][iP] - 1].y);
		p.z = 0.5*(pa[nvtx[4][iP] - 1].z + pa[nvtx[0][iP] - 1].z);

		doublereal 	dx = 0.0, dy = 0.0, dz = 0.0;// ������� ������������ ������
		dx = pa[nvtx[1][iP] - 1].x - pa[nvtx[0][iP] - 1].x;
		dy = pa[nvtx[2][iP] - 1].y - pa[nvtx[0][iP] - 1].y;
		dz = pa[nvtx[4][iP] - 1].z - pa[nvtx[0][iP] - 1].z;

		if (fabs(pa[nvtx[4][iP] - 1].z - pa[nvtx[0][iP] - 1].z) < 1.0e-40) {
			printf("error : center cord 3D slipanie po Z. delta < 1.0E-40\n");
#if doubleintprecision == 1
			printf("iP=%lld dx=%e dy=%e dz=%e\n", iP, dx, dy, dz);
			printf("nvtx[%lld] : %lld %lld %lld %lld %lld %lld %lld %lld \n", iP, nvtx[0][iP] - 1, nvtx[1][iP] - 1, nvtx[2][iP] - 1, nvtx[3][iP] - 1, nvtx[4][iP] - 1, nvtx[5][iP] - 1, nvtx[6][iP] - 1, nvtx[7][iP] - 1);
#else
			printf("iP=%d dx=%e dy=%e dz=%e\n", iP, dx, dy, dz);
			printf("nvtx[%d] : %d %d %d %d %d %d %d %d \n", iP, nvtx[0][iP] - 1, nvtx[1][iP] - 1, nvtx[2][iP] - 1, nvtx[3][iP] - 1, nvtx[4][iP] - 1, nvtx[5][iP] - 1, nvtx[6][iP] - 1, nvtx[7][iP] - 1);
#endif

				//getchar();
			system("PAUSE");
		}
		if (fabs(pa[nvtx[1][iP] - 1].x - pa[nvtx[0][iP] - 1].x) < 1.0e-40) {
			printf("error : center cord 3D slipanie po X. delta < 1.0E-40\n");
#if doubleintprecision == 1
			printf("iP=%lld dx=%e dy=%e dz=%e\n", iP, dx, dy, dz);
			printf("nvtx[%lld] : %lld %lld %lld %lld %lld %lld %lld %lld \n", iP, nvtx[0][iP] - 1, nvtx[1][iP] - 1, nvtx[2][iP] - 1, nvtx[3][iP] - 1, nvtx[4][iP] - 1, nvtx[5][iP] - 1, nvtx[6][iP] - 1, nvtx[7][iP] - 1);
#else
			printf("iP=%d dx=%e dy=%e dz=%e\n", iP, dx, dy, dz);
			printf("nvtx[%d] : %d %d %d %d %d %d %d %d \n", iP, nvtx[0][iP] - 1, nvtx[1][iP] - 1, nvtx[2][iP] - 1, nvtx[3][iP] - 1, nvtx[4][iP] - 1, nvtx[5][iP] - 1, nvtx[6][iP] - 1, nvtx[7][iP] - 1);
#endif
				//getchar();
			system("PAUSE");
		}
		if (fabs(pa[nvtx[3][iP] - 1].y - pa[nvtx[0][iP] - 1].y) < 1.0e-40) {
			printf("error : center cord 3D slipanie po Y. delta < 1.0E-40\n");
#if doubleintprecision == 1
			printf("iP=%lld dx=%e dy=%e dz=%e\n", iP, dx, dy, dz);
			printf("nvtx[%lld] : %lld %lld %lld %lld %lld %lld %lld %lld \n", iP, nvtx[0][iP] - 1, nvtx[1][iP] - 1, nvtx[2][iP] - 1, nvtx[3][iP] - 1, nvtx[4][iP] - 1, nvtx[5][iP] - 1, nvtx[6][iP] - 1, nvtx[7][iP] - 1);
#else
			printf("iP=%d dx=%e dy=%e dz=%e\n", iP, dx, dy, dz);
			printf("nvtx[%d] : %d %d %d %d %d %d %d %d \n", iP, nvtx[0][iP] - 1, nvtx[1][iP] - 1, nvtx[2][iP] - 1, nvtx[3][iP] - 1, nvtx[4][iP] - 1, nvtx[5][iP] - 1, nvtx[6][iP] - 1, nvtx[7][iP] - 1);
#endif
				//getchar();
			system("PAUSE");
		}
	}

	//printf("%e %e %e\n",dx,dy,dz); // debug GOOD
	//getchar();	
} // center_cord3D

// ��������� ������� ������������ ������
void volume3D(integer iP, integer** nvtx, TOCHKA* pa, doublereal &dx, doublereal &dy, doublereal &dz) {
	// ���������� �������� �������� ������������ ������:
	
	dx = 0.0; dy = 0.0; dz = 0.0;// ������� ������������ ������
	dx=pa[nvtx[1][iP]-1].x-pa[nvtx[0][iP]-1].x;
	dy=pa[nvtx[2][iP]-1].y-pa[nvtx[0][iP]-1].y;
    dz=pa[nvtx[4][iP]-1].z-pa[nvtx[0][iP]-1].z;
	if (fabs(pa[nvtx[4][iP] - 1].z - pa[nvtx[0][iP] - 1].z) < 1.0e-40) {
		printf("error : volume 3D slipanie po Z. delta < 1.0E-40\n");
#if doubleintprecision == 1
		printf("iP=%lld dx=%e dy=%e dz=%e\n", iP, dx, dy, dz);
		printf("nvtx[%lld] : %lld %lld %lld %lld %lld %lld %lld %lld \n", iP, nvtx[0][iP] - 1, nvtx[1][iP] - 1, nvtx[2][iP] - 1, nvtx[3][iP] - 1, nvtx[4][iP] - 1, nvtx[5][iP] - 1, nvtx[6][iP] - 1, nvtx[7][iP] - 1);
#else
		printf("iP=%d dx=%e dy=%e dz=%e\n", iP, dx, dy, dz);
		printf("nvtx[%d] : %d %d %d %d %d %d %d %d \n", iP, nvtx[0][iP] - 1, nvtx[1][iP] - 1, nvtx[2][iP] - 1, nvtx[3][iP] - 1, nvtx[4][iP] - 1, nvtx[5][iP] - 1, nvtx[6][iP] - 1, nvtx[7][iP] - 1);
#endif
			//getchar();
		system("PAUSE");
	}
	if (fabs(pa[nvtx[1][iP] - 1].x - pa[nvtx[0][iP] - 1].x) < 1.0e-40) {
		printf("error : volume 3D slipanie po X. delta < 1.0E-40\n");
#if doubleintprecision == 1
		printf("iP=%lld dx=%e dy=%e dz=%e\n", iP, dx, dy, dz);
		printf("nvtx[%lld] : %lld %lld %lld %lld %lld %lld %lld %lld \n", iP, nvtx[0][iP] - 1, nvtx[1][iP] - 1, nvtx[2][iP] - 1, nvtx[3][iP] - 1, nvtx[4][iP] - 1, nvtx[5][iP] - 1, nvtx[6][iP] - 1, nvtx[7][iP] - 1);
#else
		printf("iP=%d dx=%e dy=%e dz=%e\n", iP, dx, dy, dz);
		printf("nvtx[%d] : %d %d %d %d %d %d %d %d \n", iP, nvtx[0][iP] - 1, nvtx[1][iP] - 1, nvtx[2][iP] - 1, nvtx[3][iP] - 1, nvtx[4][iP] - 1, nvtx[5][iP] - 1, nvtx[6][iP] - 1, nvtx[7][iP] - 1);
#endif
		//getchar();
		system("PAUSE");
	}
	if (fabs(pa[nvtx[3][iP] - 1].y - pa[nvtx[0][iP] - 1].y) < 1.0e-40) {
		printf("error : volume 3D slipanie po Y. delta < 1.0E-40\n");
#if doubleintprecision == 1
		printf("iP=%lld dx=%e dy=%e dz=%e\n", iP, dx, dy, dz);
		printf("nvtx[%lld] : %lld %lld %lld %lld %lld %lld %lld %lld \n", iP, nvtx[0][iP] - 1, nvtx[1][iP] - 1, nvtx[2][iP] - 1, nvtx[3][iP] - 1, nvtx[4][iP] - 1, nvtx[5][iP] - 1, nvtx[6][iP] - 1, nvtx[7][iP] - 1);
#else
		printf("iP=%d dx=%e dy=%e dz=%e\n", iP, dx, dy, dz);
		printf("nvtx[%d] : %d %d %d %d %d %d %d %d \n", iP, nvtx[0][iP] - 1, nvtx[1][iP] - 1, nvtx[2][iP] - 1, nvtx[3][iP] - 1, nvtx[4][iP] - 1, nvtx[5][iP] - 1, nvtx[6][iP] - 1, nvtx[7][iP] - 1);
#endif
		//getchar();
		system("PAUSE");
	}
	//printf("%e %e %e\n",dx,dy,dz); // debug GOOD
	//getchar();	
} // volume3D 

// 1 october 2016
// ������� �� ���� ����� �����
/*
integer min(integer i1, integer i2) {
	integer ir=i1;
	if (i2<i1) ir=i2;
	return ir;
} // min
*/

// ��������� ����������� �� ����������� �����
// �������� ������.
// ���������� �������� ib ������ ������ �����
// �������� ����������� ����������� �����.
bool in_model_temp(TOCHKA p, integer &ib, BLOCK* b, integer lb) {
	integer i=0, k=0;
	bool ret=true;// �� ��������� ����������� ������
	integer lb4 = (integer)(0.25*lb); // ��� ����� ������ ������� ��������� ������ � ������,
	// ���� ��� ����� ����������� : �������� �������� ���������� ����� �����.

	
#ifdef _OPENMP
	integer k1 = 0, k2 = 0, k3 = 0, k4 = 0;
#pragma omp parallel shared(k1,k2,k3,k4) num_threads(4)
	{
		integer tid = omp_get_thread_num();
		if (tid == 0) {
			// ���� �� ���� ������
			//for (integer i1 = 0; i1<lb4; i1++) {
			for (integer i1 = 0; i1<lb; i1+=4) {
				if (b[i1].g.itypegeom == 0) {
					// Prism
					if ((p.x > b[i1].g.xS) && (p.x < b[i1].g.xE) && (p.y > b[i1].g.yS) && (p.y < b[i1].g.yE) && (p.z > b[i1].g.zS) && (p.z < b[i1].g.zE)) {
						k1 = i1;
					}
				}
				if (b[i1].g.itypegeom == 2) {

					if ((p.x > b[i1].g.xS) && (p.x < b[i1].g.xE) && (p.y > b[i1].g.yS) && (p.y < b[i1].g.yE) && (p.z > b[i1].g.zS) && (p.z < b[i1].g.zE)) {
						// ���������� �������������� ����� ��������.
						in_polygon(p, b[i1].g.nsizei, b[i1].g.xi, b[i1].g.yi, b[i1].g.zi, b[i1].g.hi, b[i1].g.iPlane_obj2, k1, i1);
					}

				}

				if (b[i1].g.itypegeom == 1) {
					// Cylinder
					switch (b[i1].g.iPlane) {
					case XY:
						if (fabs(b[i1].g.R_in_cyl) < 1.0e-40) {
							if ((p.z > b[i1].g.zC) && (p.z < b[i1].g.zC + b[i1].g.Hcyl)) {
								if (sqrt((b[i1].g.xC - p.x)*(b[i1].g.xC - p.x) + (b[i1].g.yC - p.y)*(b[i1].g.yC - p.y)) < b[i1].g.R_out_cyl) {
									k1 = i1;
								}
							}
						}
						else {
							if ((p.z > b[i1].g.zC) && (p.z < b[i1].g.zC + b[i1].g.Hcyl)) {
								if (sqrt((b[i1].g.xC - p.x)*(b[i1].g.xC - p.x) + (b[i1].g.yC - p.y)*(b[i1].g.yC - p.y)) < b[i1].g.R_out_cyl) {
									if (sqrt((b[i1].g.xC - p.x)*(b[i1].g.xC - p.x) + (b[i1].g.yC - p.y)*(b[i1].g.yC - p.y)) > b[i1].g.R_in_cyl) {
										k1 = i1;
									}
								}
							}
						}
						break;
					case XZ:
						if (fabs(b[i1].g.R_in_cyl) < 1.0e-40) {
							if ((p.y > b[i1].g.yC) && (p.y < b[i1].g.yC + b[i1].g.Hcyl)) {
								if (sqrt((b[i1].g.xC - p.x)*(b[i1].g.xC - p.x) + (b[i1].g.zC - p.z)*(b[i1].g.zC - p.z)) < b[i1].g.R_out_cyl) {
									k1 = i1;
								}
							}
						}
						else {
							if ((p.y > b[i1].g.yC) && (p.y < b[i1].g.yC + b[i1].g.Hcyl)) {
								if (sqrt((b[i1].g.xC - p.x)*(b[i1].g.xC - p.x) + (b[i1].g.zC - p.z)*(b[i1].g.zC - p.z)) < b[i1].g.R_out_cyl) {
									if (sqrt((b[i1].g.xC - p.x)*(b[i1].g.xC - p.x) + (b[i1].g.zC - p.z)*(b[i1].g.zC - p.z)) > b[i1].g.R_in_cyl) {
										k1 = i1;
									}
								}
							}
						}
						break;
					case YZ:
						if (fabs(b[i1].g.R_in_cyl) < 1.0e-40) {
							if ((p.x > b[i1].g.xC) && (p.x < b[i1].g.xC + b[i1].g.Hcyl)) {
								if (sqrt((b[i1].g.yC - p.y)*(b[i1].g.yC - p.y) + (b[i1].g.zC - p.z)*(b[i1].g.zC - p.z)) < b[i1].g.R_out_cyl) {
									k1 = i1;
								}
							}
						}
						else {
							if ((p.x > b[i1].g.xC) && (p.x < b[i1].g.xC + b[i1].g.Hcyl)) {
								if (sqrt((b[i1].g.yC - p.y)*(b[i1].g.yC - p.y) + (b[i1].g.zC - p.z)*(b[i1].g.zC - p.z)) < b[i1].g.R_out_cyl) {
									if (sqrt((b[i1].g.yC - p.y)*(b[i1].g.yC - p.y) + (b[i1].g.zC - p.z)*(b[i1].g.zC - p.z)) > b[i1].g.R_in_cyl) {
										k1 = i1;
									}
								}
							}
						}
						break;
					}
				}
			}
		}
		if (tid == 1) {
			// ���� �� ���� ������
			//for (integer i2 = lb4; i2<2 * lb4; i2++) {
			for (integer i2 = 1; i2<lb; i2+=4) {
				if (b[i2].g.itypegeom == 0) {
					// Prism
					if ((p.x > b[i2].g.xS) && (p.x < b[i2].g.xE) && (p.y > b[i2].g.yS) && (p.y < b[i2].g.yE) && (p.z > b[i2].g.zS) && (p.z < b[i2].g.zE)) {
						k2 = i2;
					}
				}
				if (b[i2].g.itypegeom == 2) {

					if ((p.x > b[i2].g.xS) && (p.x < b[i2].g.xE) && (p.y > b[i2].g.yS) && (p.y < b[i2].g.yE) && (p.z > b[i2].g.zS) && (p.z < b[i2].g.zE)) {
						// ���������� �������������� ����� ��������.
						in_polygon(p, b[i2].g.nsizei, b[i2].g.xi, b[i2].g.yi, b[i2].g.zi, b[i2].g.hi, b[i2].g.iPlane_obj2, k2, i2);
					}

				}

				if (b[i2].g.itypegeom == 1) {
					// Cylinder
					switch (b[i2].g.iPlane) {
					case XY:
						if (fabs(b[i2].g.R_in_cyl) < 1.0e-40) {
							if ((p.z > b[i2].g.zC) && (p.z < b[i2].g.zC + b[i2].g.Hcyl)) {
								if (sqrt((b[i2].g.xC - p.x)*(b[i2].g.xC - p.x) + (b[i2].g.yC - p.y)*(b[i2].g.yC - p.y)) < b[i2].g.R_out_cyl) {
									k2 = i2;
								}
							}
						}
						else {
							if ((p.z > b[i2].g.zC) && (p.z < b[i2].g.zC + b[i2].g.Hcyl)) {
								if (sqrt((b[i2].g.xC - p.x)*(b[i2].g.xC - p.x) + (b[i2].g.yC - p.y)*(b[i2].g.yC - p.y)) < b[i2].g.R_out_cyl) {
									if (sqrt((b[i2].g.xC - p.x)*(b[i2].g.xC - p.x) + (b[i2].g.yC - p.y)*(b[i2].g.yC - p.y)) > b[i2].g.R_in_cyl) {
										k2 = i2;
									}
								}
							}
						}
						break;
					case XZ:
						if (fabs(b[i2].g.R_in_cyl) < 1.0e-40) {
							if ((p.y > b[i2].g.yC) && (p.y < b[i2].g.yC + b[i2].g.Hcyl)) {
								if (sqrt((b[i2].g.xC - p.x)*(b[i2].g.xC - p.x) + (b[i2].g.zC - p.z)*(b[i2].g.zC - p.z)) < b[i2].g.R_out_cyl) {
									k2 = i2;
								}
							}
						}
						else {
							if ((p.y > b[i2].g.yC) && (p.y < b[i2].g.yC + b[i2].g.Hcyl)) {
								if (sqrt((b[i2].g.xC - p.x)*(b[i2].g.xC - p.x) + (b[i2].g.zC - p.z)*(b[i2].g.zC - p.z)) < b[i2].g.R_out_cyl) {
									if (sqrt((b[i2].g.xC - p.x)*(b[i2].g.xC - p.x) + (b[i2].g.zC - p.z)*(b[i2].g.zC - p.z)) > b[i2].g.R_in_cyl) {
										k2 = i2;
									}
								}
							}
						}
						break;
					case YZ:
						if (fabs(b[i2].g.R_in_cyl) < 1.0e-40) {
							if ((p.x > b[i2].g.xC) && (p.x < b[i2].g.xC + b[i2].g.Hcyl)) {
								if (sqrt((b[i2].g.yC - p.y)*(b[i2].g.yC - p.y) + (b[i2].g.zC - p.z)*(b[i2].g.zC - p.z)) < b[i2].g.R_out_cyl) {
									k2 = i2;
								}
							}
						}
						else {
							if ((p.x > b[i2].g.xC) && (p.x < b[i2].g.xC + b[i2].g.Hcyl)) {
								if (sqrt((b[i2].g.yC - p.y)*(b[i2].g.yC - p.y) + (b[i2].g.zC - p.z)*(b[i2].g.zC - p.z)) < b[i2].g.R_out_cyl) {
									if (sqrt((b[i2].g.yC - p.y)*(b[i2].g.yC - p.y) + (b[i2].g.zC - p.z)*(b[i2].g.zC - p.z)) > b[i2].g.R_in_cyl) {
										k2 = i2;
									}
								}
							}
						}
						break;
					}
				}
			}
		}
		if (tid == 2) {
			// ���� �� ���� ������
			//for (integer i3 = 2 * lb4; i3<3 * lb4; i3++) {
			for (integer i3 = 2; i3<lb; i3 += 4) {
				if (b[i3].g.itypegeom == 0) {
					// Prism
					if ((p.x > b[i3].g.xS) && (p.x < b[i3].g.xE) && (p.y > b[i3].g.yS) && (p.y < b[i3].g.yE) && (p.z > b[i3].g.zS) && (p.z < b[i3].g.zE)) {
						k3 = i3;
					}
				}
				if (b[i3].g.itypegeom == 2) {

					if ((p.x > b[i3].g.xS) && (p.x < b[i3].g.xE) && (p.y > b[i3].g.yS) && (p.y < b[i3].g.yE) && (p.z > b[i3].g.zS) && (p.z < b[i3].g.zE)) {
						// ���������� �������������� ����� ��������.
						in_polygon(p, b[i3].g.nsizei, b[i3].g.xi, b[i3].g.yi, b[i3].g.zi, b[i3].g.hi, b[i3].g.iPlane_obj2, k3, i3);
					}

				}

				if (b[i3].g.itypegeom == 1) {
					// Cylinder
					switch (b[i3].g.iPlane) {
					case XY:
						if (fabs(b[i3].g.R_in_cyl) < 1.0e-40) {
							if ((p.z > b[i3].g.zC) && (p.z < b[i3].g.zC + b[i3].g.Hcyl)) {
								if (sqrt((b[i3].g.xC - p.x)*(b[i3].g.xC - p.x) + (b[i3].g.yC - p.y)*(b[i3].g.yC - p.y)) < b[i3].g.R_out_cyl) {
									k3 = i3;
								}
							}
						}
						else {
							if ((p.z > b[i3].g.zC) && (p.z < b[i3].g.zC + b[i3].g.Hcyl)) {
								if (sqrt((b[i3].g.xC - p.x)*(b[i3].g.xC - p.x) + (b[i3].g.yC - p.y)*(b[i3].g.yC - p.y)) < b[i3].g.R_out_cyl) {
									if (sqrt((b[i3].g.xC - p.x)*(b[i3].g.xC - p.x) + (b[i3].g.yC - p.y)*(b[i3].g.yC - p.y)) > b[i3].g.R_in_cyl) {
										k3 = i3;
									}
								}
							}
						}
						break;
					case XZ:
						if (fabs(b[i3].g.R_in_cyl) < 1.0e-40) {
							if ((p.y > b[i3].g.yC) && (p.y < b[i3].g.yC + b[i3].g.Hcyl)) {
								if (sqrt((b[i3].g.xC - p.x)*(b[i3].g.xC - p.x) + (b[i3].g.zC - p.z)*(b[i3].g.zC - p.z)) < b[i3].g.R_out_cyl) {
									k3 = i3;
								}
							}
						}
						else {
							if ((p.y > b[i3].g.yC) && (p.y < b[i3].g.yC + b[i3].g.Hcyl)) {
								if (sqrt((b[i3].g.xC - p.x)*(b[i3].g.xC - p.x) + (b[i3].g.zC - p.z)*(b[i3].g.zC - p.z)) < b[i3].g.R_out_cyl) {
									if (sqrt((b[i3].g.xC - p.x)*(b[i3].g.xC - p.x) + (b[i3].g.zC - p.z)*(b[i3].g.zC - p.z)) > b[i3].g.R_in_cyl) {
										k3 = i3;
									}
								}
							}
						}
						break;
					case YZ:
						if (fabs(b[i3].g.R_in_cyl) < 1.0e-40) {
							if ((p.x > b[i3].g.xC) && (p.x < b[i3].g.xC + b[i3].g.Hcyl)) {
								if (sqrt((b[i3].g.yC - p.y)*(b[i3].g.yC - p.y) + (b[i3].g.zC - p.z)*(b[i3].g.zC - p.z)) < b[i3].g.R_out_cyl) {
									k3 = i3;
								}
							}
						}
						else {
							if ((p.x > b[i3].g.xC) && (p.x < b[i3].g.xC + b[i3].g.Hcyl)) {
								if (sqrt((b[i3].g.yC - p.y)*(b[i3].g.yC - p.y) + (b[i3].g.zC - p.z)*(b[i3].g.zC - p.z)) < b[i3].g.R_out_cyl) {
									if (sqrt((b[i3].g.yC - p.y)*(b[i3].g.yC - p.y) + (b[i3].g.zC - p.z)*(b[i3].g.zC - p.z)) > b[i3].g.R_in_cyl) {
										k3 = i3;
									}
								}
							}
						}
						break;
					}
				}
			}
		}
		if (tid == 3) {
			// ���� �� ���� ������
			//for (integer i4 = 3 * lb4; i4<lb; i4++) {
			for (integer i4 = 3; i4<lb; i4 += 4) {
				if (b[i4].g.itypegeom == 0) {
					// Prism
					if ((p.x > b[i4].g.xS) && (p.x < b[i4].g.xE) && (p.y > b[i4].g.yS) && (p.y < b[i4].g.yE) && (p.z > b[i4].g.zS) && (p.z < b[i4].g.zE)) {
						k4 = i4;
					}
				}
				if (b[i4].g.itypegeom == 2) {

					if ((p.x > b[i4].g.xS) && (p.x < b[i4].g.xE) && (p.y > b[i4].g.yS) && (p.y < b[i4].g.yE) && (p.z > b[i4].g.zS) && (p.z < b[i4].g.zE)) {
						// ���������� �������������� ����� ��������.
						in_polygon(p, b[i4].g.nsizei, b[i4].g.xi, b[i4].g.yi, b[i4].g.zi, b[i4].g.hi, b[i4].g.iPlane_obj2, k4, i4);
					}

				}

				if (b[i4].g.itypegeom == 1) {
					// Cylinder
					switch (b[i4].g.iPlane) {
					case XY:
						if (fabs(b[i4].g.R_in_cyl) < 1.0e-40) {
							if ((p.z > b[i4].g.zC) && (p.z < b[i4].g.zC + b[i4].g.Hcyl)) {
								if (sqrt((b[i4].g.xC - p.x)*(b[i4].g.xC - p.x) + (b[i4].g.yC - p.y)*(b[i4].g.yC - p.y)) < b[i4].g.R_out_cyl) {
									k4 = i4;
								}
							}
						}
						else {
							if ((p.z > b[i4].g.zC) && (p.z < b[i4].g.zC + b[i4].g.Hcyl)) {
								if (sqrt((b[i4].g.xC - p.x)*(b[i4].g.xC - p.x) + (b[i4].g.yC - p.y)*(b[i4].g.yC - p.y)) < b[i4].g.R_out_cyl) {
									if (sqrt((b[i4].g.xC - p.x)*(b[i4].g.xC - p.x) + (b[i4].g.yC - p.y)*(b[i4].g.yC - p.y)) > b[i4].g.R_in_cyl) {
										k4 = i4;
									}
								}
							}
						}
						break;
					case XZ:
						if (fabs(b[i4].g.R_in_cyl) < 1.0e-40) {
							if ((p.y > b[i4].g.yC) && (p.y < b[i4].g.yC + b[i4].g.Hcyl)) {
								if (sqrt((b[i4].g.xC - p.x)*(b[i4].g.xC - p.x) + (b[i4].g.zC - p.z)*(b[i4].g.zC - p.z)) < b[i4].g.R_out_cyl) {
									k4 = i4;
								}
							}
						}
						else {
							if ((p.y > b[i4].g.yC) && (p.y < b[i4].g.yC + b[i4].g.Hcyl)) {
								if (sqrt((b[i4].g.xC - p.x)*(b[i4].g.xC - p.x) + (b[i4].g.zC - p.z)*(b[i4].g.zC - p.z)) < b[i4].g.R_out_cyl) {
									if (sqrt((b[i4].g.xC - p.x)*(b[i4].g.xC - p.x) + (b[i4].g.zC - p.z)*(b[i4].g.zC - p.z)) > b[i4].g.R_in_cyl) {
										k4 = i4;
									}
								}
							}
						}
						break;
					case YZ:
						if (fabs(b[i4].g.R_in_cyl) < 1.0e-40) {
							if ((p.x > b[i4].g.xC) && (p.x < b[i4].g.xC + b[i4].g.Hcyl)) {
								if (sqrt((b[i4].g.yC - p.y)*(b[i4].g.yC - p.y) + (b[i4].g.zC - p.z)*(b[i4].g.zC - p.z)) < b[i4].g.R_out_cyl) {
									k4 = i4;
								}
							}
						}
						else {
							if ((p.x > b[i4].g.xC) && (p.x < b[i4].g.xC + b[i4].g.Hcyl)) {
								if (sqrt((b[i4].g.yC - p.y)*(b[i4].g.yC - p.y) + (b[i4].g.zC - p.z)*(b[i4].g.zC - p.z)) < b[i4].g.R_out_cyl) {
									if (sqrt((b[i4].g.yC - p.y)*(b[i4].g.yC - p.y) + (b[i4].g.zC - p.z)*(b[i4].g.zC - p.z)) > b[i4].g.R_in_cyl) {
										k4 = i4;
									}
								}
							}
						}
						break;
					}
				}
			}
		}

	}
	k = k1;
	if (k2 > k) k = k2;
	if (k3 > k) k = k3;
	if (k4 > k) k = k4;

#else

   // 27_12_2017.
   // ����� ������������� �������� �����������.
   // ������� ���� ����� � ������ ����������� ������� � ����.
   // ����  ������� ������� �������������� �������� ����� � ������� �������.
   // ������� ���� ����������� ����� � ����� (������� � �������� �������� ������ lb � �����
   // � ������� ���������� �������, �� ������ ��������� ���� ��� ��� � ����� ������ � �����
   // �������� �������� ���� for � ������� break � ���������� ������������ ���� �������.


	// ���� �� ���� ������
	for (i = lb-1; i>=0; i--) {
		if (b[i].g.itypegeom == 0) {
			// Prism
			if ((p.x > b[i].g.xS) && (p.x < b[i].g.xE) && (p.y > b[i].g.yS) && (p.y < b[i].g.yE) && (p.z > b[i].g.zS) && (p.z < b[i].g.zE)) {
				k = i;
				 // ����� � ����� ��������� �������� � ������ ������.
				goto OUTOF_IN_MODEL_TEMP1;
			}
		}
		if (b[i].g.itypegeom == 2) {

			if ((p.x > b[i].g.xS) && (p.x < b[i].g.xE) && (p.y > b[i].g.yS) && (p.y < b[i].g.yE) && (p.z > b[i].g.zS) && (p.z < b[i].g.zE)) {

				bool bfound = false;
				// ���������� �������������� ����� ��������.
				bfound=in_polygon(p, b[i].g.nsizei, b[i].g.xi, b[i].g.yi, b[i].g.zi, b[i].g.hi, b[i].g.iPlane_obj2, k, i);
				if (bfound) {
					// ����� � ����� ��������� �������� � ������ ������.
					goto OUTOF_IN_MODEL_TEMP1;
				}
			}

		}
		if (b[i].g.itypegeom == 1) {
			// Cylinder
			switch (b[i].g.iPlane) {
			case XY:
				if (fabs(b[i].g.R_in_cyl) < 1.0e-40) {
					if ((p.z > b[i].g.zC) && (p.z < b[i].g.zC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.yC - p.y)*(b[i].g.yC - p.y)) < b[i].g.R_out_cyl) {
							k = i;
							// ����� � ����� ��������� �������� � ������ ������.
							goto OUTOF_IN_MODEL_TEMP1;
						}
					}
				}
				else {
					if ((p.z > b[i].g.zC) && (p.z < b[i].g.zC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.yC - p.y)*(b[i].g.yC - p.y)) < b[i].g.R_out_cyl) {
							if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.yC - p.y)*(b[i].g.yC - p.y)) > b[i].g.R_in_cyl) {
								k = i;
								// ����� � ����� ��������� �������� � ������ ������.
								goto OUTOF_IN_MODEL_TEMP1;
							}
						}
					}
				}
				break;
			case XZ:
				if (fabs(b[i].g.R_in_cyl) < 1.0e-40) {
					if ((p.y > b[i].g.yC) && (p.y < b[i].g.yC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) < b[i].g.R_out_cyl) {
							k = i;
							// ����� � ����� ��������� �������� � ������ ������.
							goto OUTOF_IN_MODEL_TEMP1;
						}
					}
				}
				else {
					if ((p.y > b[i].g.yC) && (p.y < b[i].g.yC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) < b[i].g.R_out_cyl) {
							if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) > b[i].g.R_in_cyl) {
								k = i;
								// ����� � ����� ��������� �������� � ������ ������.
								goto OUTOF_IN_MODEL_TEMP1;
							}
						}
					}
				}
				break;
			case YZ:
				if (fabs(b[i].g.R_in_cyl) < 1.0e-40) {
					if ((p.x > b[i].g.xC) && (p.x < b[i].g.xC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.yC - p.y)*(b[i].g.yC - p.y) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) < b[i].g.R_out_cyl) {
							k = i;
							// ����� � ����� ��������� �������� � ������ ������.
							goto OUTOF_IN_MODEL_TEMP1;
						}
					}
				}
				else {
					if ((p.x > b[i].g.xC) && (p.x < b[i].g.xC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.yC - p.y)*(b[i].g.yC - p.y) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) < b[i].g.R_out_cyl) {
							if (sqrt((b[i].g.yC - p.y)*(b[i].g.yC - p.y) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) > b[i].g.R_in_cyl) {
								k = i;
								// ����� � ����� ��������� �������� � ������ ������.
								goto OUTOF_IN_MODEL_TEMP1;
							}
						}
					}
				}
				break;
			}
		}
	}

OUTOF_IN_MODEL_TEMP1 :

#endif

	
	if (b[k].itype==HOLLOW) ret=false;
	ib=k;

	return ret;

} // in_model_temp

// ��������� ����������� �� ����������� �����
// ����������������� ������.
// ���������� �������� ib ������ ������ �����
// �������� ����������� ����������� �����.
bool in_model_flow(TOCHKA p, integer &ib, BLOCK* b, integer lb) {
	integer i=0, k=0;
	bool ret=true;// �� ��������� ����������� ������

	// 27_12_2017.
	// ����� ������������� �������� �����������.
	// ������� ���� ����� � ������ ����������� ������� � ����.
	// ����  ������� ������� �������������� �������� ����� � ������� �������.
	// ������� ���� ����������� ����� � ����� (������� � �������� �������� ������ lb � �����
	// � ������� ���������� �������, �� ������ ��������� ���� ��� ��� � ����� ������ � �����
	// �������� �������� ���� for � ������� break � ���������� ������������ ���� �������.

	// ���� �� ���� ������
	for (i=lb-1; i>=0; i--) {

		if (b[i].g.itypegeom == 0) {
			// Prism
			if ((p.x > b[i].g.xS) && (p.x < b[i].g.xE) && (p.y > b[i].g.yS) && (p.y < b[i].g.yE) && (p.z > b[i].g.zS) && (p.z < b[i].g.zE)) {
				k = i;
				// ������ ����� � ����� ��������� ��������.
				goto OUTOF_IN_MODEL_FLOW;
			}
		}
		if (b[i].g.itypegeom == 2) {

			if ((p.x > b[i].g.xS) && (p.x < b[i].g.xE) && (p.y > b[i].g.yS) && (p.y < b[i].g.yE) && (p.z > b[i].g.zS) && (p.z < b[i].g.zE)) {
				
				bool found = false;
				// ���������� �������������� ����� ��������.
				found=in_polygon(p, b[i].g.nsizei, b[i].g.xi, b[i].g.yi, b[i].g.zi, b[i].g.hi, b[i].g.iPlane_obj2, k, i);
				if (found) {
					// ������ ����� � ����� ��������� ��������.
					goto OUTOF_IN_MODEL_FLOW;
				}
			
			}

		}

		if (b[i].g.itypegeom == 1) {
			// Cylinder
			switch (b[i].g.iPlane) {
			case XY:
				if (fabs(b[i].g.R_in_cyl) < 1.0e-40) {
					if ((p.z > b[i].g.zC) && (p.z < b[i].g.zC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.yC - p.y)*(b[i].g.yC - p.y)) < b[i].g.R_out_cyl) {
							k = i;
							// ������ ����� � ����� ��������� ��������.
							goto OUTOF_IN_MODEL_FLOW;
						}
					}
				}
				else {
					if ((p.z > b[i].g.zC) && (p.z < b[i].g.zC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.yC - p.y)*(b[i].g.yC - p.y)) < b[i].g.R_out_cyl) {
							if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.yC - p.y)*(b[i].g.yC - p.y)) > b[i].g.R_in_cyl) {
								k = i;
								// ������ ����� � ����� ��������� ��������.
								goto OUTOF_IN_MODEL_FLOW;
							}
						}
					}
				}
				break;
			case XZ:
				if (fabs(b[i].g.R_in_cyl) < 1.0e-40) {
					if ((p.y > b[i].g.yC) && (p.y < b[i].g.yC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) < b[i].g.R_out_cyl) {
							k = i;
							// ������ ����� � ����� ��������� ��������.
							goto OUTOF_IN_MODEL_FLOW;
						}
					}
				}
				else {
					if ((p.y > b[i].g.yC) && (p.y < b[i].g.yC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) < b[i].g.R_out_cyl) {
							if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) > b[i].g.R_in_cyl) {
								k = i;
								// ������ ����� � ����� ��������� ��������.
								goto OUTOF_IN_MODEL_FLOW;
							}
						}
					}
				}
				break;
			case YZ:
				if (fabs(b[i].g.R_in_cyl) < 1.0e-40) {
					if ((p.x > b[i].g.xC) && (p.x < b[i].g.xC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.yC - p.y)*(b[i].g.yC - p.y) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) < b[i].g.R_out_cyl) {
							k = i;
							// ������ ����� � ����� ��������� ��������.
							goto OUTOF_IN_MODEL_FLOW;
						}
					}
				}
				else {
					if ((p.x > b[i].g.xC) && (p.x < b[i].g.xC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.yC - p.y)*(b[i].g.yC - p.y) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) < b[i].g.R_out_cyl) {
							if (sqrt((b[i].g.yC - p.y)*(b[i].g.yC - p.y) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) > b[i].g.R_in_cyl) {
								k = i;
								// ������ ����� � ����� ��������� ��������.
								goto OUTOF_IN_MODEL_FLOW;
							}
						}
					}
				}
				break;
			}
		}
	}

	OUTOF_IN_MODEL_FLOW :

	if ((b[k].itype==SOLID) || (b[k].itype==HOLLOW)) ret=false;
	ib=k;

	return ret;

} // in_model_flow

#include "constr_struct_alice.cpp" // �������� ���������� ���������� � loadTemper_and_Flow ��� ���� �����.



// ������ ��� enumerate_volume_improved
// ��. ������������ ���� adaptive_local_refinement_mesh.cpp
//typedef struct TBlock_indexes {
	//integer iL, iR, jL, jR, kL, kR;
//} Block_indexes;

  // ���������� ��������� ����������� �������
  // ��� ����� ����������������.
// 25.03.2017 improved ������ ����� ������� �� �������� ����������.
// ������ ����� ��������� �������������� �������� ������ ��� ������������� �����.
// 2.04.2017 ���������������� ������.
void enumerate_volume_improved(integer* &evt, integer &maxelm, integer iflag, doublereal* xpos, doublereal* ypos, doublereal* zpos, integer* &whot_is_block,
	integer inx, integer iny, integer inz, BLOCK* b, integer lb) {

	int i_my_num_core_parallelesation = omp_get_num_threads();
	omp_set_num_threads(8); // ���������� 8 �������, 10 ������� ��� �������� �� �������.

	

	evt = NULL;
	evt = new integer[inx*iny*inz];
	if (evt == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for evt constr struct...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}
	whot_is_block = NULL;
	whot_is_block = new integer[inx*iny*inz];
	if (whot_is_block == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for whot_is_block constr struct...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}
	

	printf("enumerate_volume_improved start.\n");
	Block_indexes* block_indexes = new Block_indexes[lb];
	if (block_indexes==NULL) {
		printf("error in allocation memory for block_indexes in enumerate_volume_improved.\n");
		system("pause");
		exit(1);
	}
	integer i, j, k;

	// 08.04.2018
	for (i = 0; i < lb; i++) {
		// �������������, �� ������ ���� ����� �� ����� ����������.
		block_indexes[i].iL = -1;
		block_indexes[i].iR = -2;
		block_indexes[i].jL = -1;
		block_indexes[i].jR = -2;
		block_indexes[i].kL = -1;
		block_indexes[i].kR = -2;
	}

	for (i = 0; i < lb; i++) {
		doublereal x4 = b[i].g.xS;
		for (j = 0; j <= inx; j++) {
			if (fabs(xpos[j] - x4) < 1.0e-40) {
				block_indexes[i].iL = j;
				break;
			}
		}
		x4 = b[i].g.xE;
		for (j = 0; j <= inx; j++) {
			if (fabs(xpos[j] - x4) < 1.0e-40) {
				block_indexes[i].iR = j;
				break;
			}
		}
		x4 = b[i].g.yS;
		for (j = 0; j <= iny; j++) {
			if (fabs(ypos[j] - x4) < 1.0e-40) {
				block_indexes[i].jL = j;
				break;
			}
		}
		x4 = b[i].g.yE;
		for (j = 0; j <= iny; j++) {
			if (fabs(ypos[j] - x4) < 1.0e-40) {
				block_indexes[i].jR = j;
				break;
			}
		}
		x4 = b[i].g.zS;
		for (j = 0; j <= inz; j++) {
			if (fabs(zpos[j] - x4) < 1.0e-40) {
				block_indexes[i].kL = j;
				break;
			}
		}
		x4 = b[i].g.zE;
		for (j = 0; j <= inz; j++) {
			if (fabs(zpos[j] - x4) < 1.0e-40) {
				block_indexes[i].kR = j;
				break;
			}
		}
	}

	// ���������� �������� ����������� ����������� � � ����� ��� �������� � �������������
	// ���������� ��������������.
	integer m7;

	/*
	integer ib_stub = -1;
	// �� ������ ����� ������� �� ������� Hollow block, ����� ����� ������ �������.
	ib_stub = 0;
	doublereal vol_stub = -1.0;
	for (i = 0; i < lb; i++) {
		if (b[i].itype == HOLLOW) {
			if (fabs(b[i].g.xE - b[i].g.xS)*fabs(b[i].g.yE - b[i].g.yS)*fabs(b[i].g.zE - b[i].g.zS) > vol_stub) {
				ib_stub = i;
				vol_stub = fabs(b[i].g.xE - b[i].g.xS)*fabs(b[i].g.yE - b[i].g.yS)*fabs(b[i].g.zE - b[i].g.zS);
			}
		}
	}
	*/

#pragma omp parallel for
	for (integer i1 = 0; i1 < inx; i1++) for (integer j1 = 0; j1 < iny; j1++) for (integer k1 = 0; k1 < inz; k1++) {
		evt[i1 + j1*inx + k1*inx*iny] = -1;// -1
	}
	for (m7 = 0; m7 < lb; m7++) {

#pragma omp parallel for
		for (integer i1 = block_indexes[m7].iL; i1 < block_indexes[m7].iR; i1++) for (integer j1 = block_indexes[m7].jL; j1 < block_indexes[m7].jR; j1++) for (integer k1 = block_indexes[m7].kL; k1 < block_indexes[m7].kR; k1++) {
			switch (iflag) {
			case TEMPERATURE:
				if (b[m7].itype == HOLLOW) {
					evt[i1 + j1*inx + k1*inx*iny] = -1;
				}
				else {
					evt[i1 + j1*inx + k1*inx*iny] = m7;
				}
					break;
			case HYDRODINAMIC:
				if ((b[m7].itype == SOLID) || (b[m7].itype == HOLLOW)) {
					evt[i1 + j1*inx + k1*inx*iny] = -1;
				}
				else {
					evt[i1 + j1*inx + k1*inx*iny] = m7;
				}
				break;
			}
		
		}
	}
	printf("enumerate_volume_improved 80 procent.\n");
	// ��������� � evt ������� � �������.
	// ���� �� ����������� ��������� ������� �� ����� 0.
	integer l = 1, ib;
	for (i = 0; i < inx; i++) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
		if (evt[i + j*inx + k*inx*iny] > -1) {
			ib = evt[i + j*inx + k*inx*iny]; // ����� ����� ��� ������� �����.
			// ��� ����� ����� ��� ������ �������.
			whot_is_block[l - 1] = ib; // ����� ����� �������� ����������� ����� (p.x,p.y,p.z).
			evt[i + j*inx + k*inx*iny] = l;
			l++;
		}
		else
		{   // �� ����������� ��������� �������
			evt[i + j*inx + k*inx*iny] = 0;
		}
	}
	maxelm = l - 1;
	
	delete[] block_indexes;

	printf("enumerate_volume_improved end.\n");

	/*
	bool inDomain = false;
	// ��������� ����������� ������� ���������� � �������
	// ���� ����������� ����� �� ����������� ��������� ������� �� �������� 0.
	for (i = 0; i<inx; i++) for (j = 0; j<iny; j++) for (k = 0; k<inz; k++) {
		p.x = 0.5*(xpos[i] + xpos[i + 1]);
		p.y = 0.5*(ypos[j] + ypos[j + 1]);
		p.z = 0.5*(zpos[k] + zpos[k + 1]);
		switch (iflag) {
		case TEMPERATURE: inDomain = in_model_temp(p, ib, b, lb); break;
		case HYDRODINAMIC: inDomain = in_model_flow(p, ib, b, lb); break;
		}
		if (inDomain) {
			// ����������� ��������� �������
			evt[i + j*inx + k*inx*iny] = l;
			// ��� ����� ����� ��� ������ �������.
			whot_is_block[l - 1] = ib; // ����� ����� �������� ����������� ����� (p.x,p.y,p.z).
			l++;
		}
		else
		{   // �� ����������� ��������� �������
			evt[i + j*inx + k*inx*iny] = 0;
		}
	}

	// maxelm - ����� ����������� ������� ������������� ��������� �������
	maxelm = l - 1;
	*/

	omp_set_num_threads(i_my_num_core_parallelesation);

} // enumerate_volume_improved

// ���������� ��������� ����������� �������
// ��� ����� ����������������.
// 25.03.2017 improved ������ ����� ������� �� �������� ����������.
// ������ ����� ��������� �������������� �������� ������ ��� ������������� �����.
// �������� � �������� �������������� ������� �������.
// 2.04.2017 ���������������� ������.
void enumerate_volume_improved_obobshenie(integer* &evt, integer &maxelm, integer iflag, doublereal* xpos, doublereal* ypos, doublereal* zpos, integer* &whot_is_block,
	integer inx, integer iny, integer inz, BLOCK* b, integer lb, TOCKA_INT* &tck_int_list) {

	int i_my_num_core_parallelesation = omp_get_num_threads();
	omp_set_num_threads(8); // ���������� 8 �������, 10 ������� ��� �������� �� �������.


	tck_int_list = new TOCKA_INT[inx*iny*inz];
	if (tck_int_list == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for evt tck_int_list...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}

	bool* bvisit = NULL;
	bvisit = new bool[inx*iny*inz];
	if (bvisit == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for bvisit constr struct...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}

	evt = NULL;
	evt = new integer[inx*iny*inz];
	if (evt == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for evt constr struct...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}
	whot_is_block = NULL;
	whot_is_block = new integer[inx*iny*inz];
	if (whot_is_block == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for whot_is_block constr struct...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}


	printf("enumerate_volume_improved_obobshenie start.\n");
	Block_indexes* block_indexes = new Block_indexes[lb];
	if (block_indexes == NULL) {
		printf("error in allocation memory for block_indexes in enumerate_volume_improved.\n");
		system("pause");
		exit(1);
	}
	integer i, j, k, i_1=lb-1;//lb-1

	// 08.04.2018
	for (i = 0; i < lb; i++) {
		// �������������, �� ������ ���� ����� �� ����� ����������.
		block_indexes[i].iL = -1;
		block_indexes[i].iR = -2;
		block_indexes[i].jL = -1;
		block_indexes[i].jR = -2;
		block_indexes[i].kL = -1;
		block_indexes[i].kR = -2;
	}

	// ����������� ������ ���������� � �������������.
	// ������������ ����� � ��� �������������� � �������� ���������.
	// ����� ������������ ������������� ����������� � 0.15%.
	const doublereal otnositelnaq_tolerance_eps = 0.0035; // 0.15% 0.0015; 0.35% 27.10.2018

	//for (i = 0; i < lb; i++) {
	for (i = lb-1; i >=0 ; i--) {
		// � ������ �����������, �� �� ������������ ������ ���� � ����, � �������� �����.

		//if (b[i].g.itypegeom == 0) {

		// polygon (b[i].g.itypegeom == 2)
		// �������� ������������� ������ xS, xE, yS, yE, zS, zE - ������ �����������
		// ������� ������������� ������, ��� �������� ��������� �������������� ����� ��������
		// ������ ��� ����� ����� ����������� ������ ������ ������������� ������, ��� ������ 
		// �������� ���������.
		//if ((b[i].g.itypegeom == 0)||(b[i].g.itypegeom == 2) || (b[i].g.itypegeom == 1)) 
		{
			//  (b[i].g.itypegeom == 1) cylinder

			doublereal x4 = b[i].g.xS;
			if ((b[i].g.itypegeom == 1) && ((b[i].g.iPlane == XY) || (b[i].g.iPlane == XZ))) {
				x4 = b[i].g.xC - b[i].g.R_out_cyl;
			}
			if ((b[i].g.itypegeom == 1) && ((b[i].g.iPlane == YZ))) {
				if (b[i].g.Hcyl > 0.0) {
					x4 = b[i].g.xC;
				}
				else {
					x4 = b[i].g.xC+ b[i].g.Hcyl;
				}
			}
			for (j = 0; j <= inx; j++) {
				if (fabs(x4) > 0.0) {
					// ������������� ����������� ����� 0.15%.
					if (fabs(100 * (xpos[j] - x4) / fabs(x4)) < otnositelnaq_tolerance_eps) {
						block_indexes[i_1].iL = j;
						break;
					}
				}
				else {
					// ���������� �����������.
					if (fabs(xpos[j] - x4) < 1.0e-40) {
						block_indexes[i_1].iL = j;
						break;
					}
				}
			}
			x4 = b[i].g.xE;
			if ((b[i].g.itypegeom == 1) && ((b[i].g.iPlane == XY) || (b[i].g.iPlane == XZ))) {
				x4 = b[i].g.xC + b[i].g.R_out_cyl;
			}
			if ((b[i].g.itypegeom == 1) && ((b[i].g.iPlane == YZ))) {
				if (b[i].g.Hcyl > 0.0) {
					x4 = b[i].g.xC + b[i].g.Hcyl;
				}
				else {
					x4 = b[i].g.xC;
				}
			}
			for (j = 0; j <= inx; j++) {
				if (fabs(x4) > 0.0) {
					// ������������� ����������� ����� 0.15%.
					if (fabs(100 * (xpos[j] - x4) / fabs(x4)) < otnositelnaq_tolerance_eps) {
						block_indexes[i_1].iR = j;
						break;
					}
				}
				else {
					// ���������� �����������.
					if (fabs(xpos[j] - x4) < 1.0e-40) {
						block_indexes[i_1].iR = j;
						break;
					}
				}
			}
			x4 = b[i].g.yS;
			if ((b[i].g.itypegeom == 1) && ((b[i].g.iPlane == XY) || (b[i].g.iPlane == YZ))) {
				x4 = b[i].g.yC - b[i].g.R_out_cyl;
			}
			if ((b[i].g.itypegeom == 1) && ((b[i].g.iPlane == XZ))) {
				if (b[i].g.Hcyl > 0.0) {
					x4 = b[i].g.yC;
				}
				else {
					x4 = b[i].g.yC + b[i].g.Hcyl;
				}
			}			
			for (j = 0; j <= iny; j++) {
				if (fabs(x4) > 0.0) {
					// ������������� ����������� ����� 0.15%.
					if (fabs(100 * (ypos[j] - x4) / fabs(x4)) < otnositelnaq_tolerance_eps) {
						block_indexes[i_1].jL = j;
						break;
					}
				}
				else {
					// ���������� �����������.
					if (fabs(ypos[j] - x4) < 1.0e-40) {
						block_indexes[i_1].jL = j;
						break;
					}
				}
			}
			x4 = b[i].g.yE;
			if ((b[i].g.itypegeom == 1) && ((b[i].g.iPlane == XY) || (b[i].g.iPlane == YZ))) {
				x4 = b[i].g.yC + b[i].g.R_out_cyl;
			}
			if ((b[i].g.itypegeom == 1) && ((b[i].g.iPlane == XZ))) {
				if (b[i].g.Hcyl > 0.0) {
					x4 = b[i].g.yC + b[i].g.Hcyl;
				}
				else {
					x4 = b[i].g.yC ;
				}
			}

			for (j = 0; j <= iny; j++) {
				
				if (fabs(x4) > 0.0) {
					// ������������� ����������� ����� 0.15%.
					if (fabs(100*(ypos[j] - x4)/fabs(x4)) < otnositelnaq_tolerance_eps) {
						block_indexes[i_1].jR = j;
						break;
					}
				}
				else {
					// ���������� �����������.
					if (fabs(ypos[j] - x4) < 1.0e-40) {
						block_indexes[i_1].jR = j;
						break;
					}
				}
			}
			x4 = b[i].g.zS;
			if ((b[i].g.itypegeom == 1) && ((b[i].g.iPlane == XZ) || (b[i].g.iPlane == YZ))) {
				x4 = b[i].g.zC - b[i].g.R_out_cyl;
			}
			if ((b[i].g.itypegeom == 1) && ((b[i].g.iPlane == XY))) {
				if (b[i].g.Hcyl > 0.0) {
					x4 = b[i].g.zC;
				}
				else {
					x4 = b[i].g.zC + b[i].g.Hcyl;
				}
			}
			for (j = 0; j <= inz; j++) {
				if (fabs(x4) > 0.0) {
					// ������������� ����������� ����� 0.15%.
					if (fabs(100 * (zpos[j] - x4) / fabs(x4)) < otnositelnaq_tolerance_eps) {
						block_indexes[i_1].kL = j;
						break;
					}
				}
				else {
					// ���������� �����������.
					if (fabs(zpos[j] - x4) < 1.0e-40) {
						block_indexes[i_1].kL = j;
						break;
					}
				}
			}
			x4 = b[i].g.zE;
			if ((b[i].g.itypegeom == 1) && ((b[i].g.iPlane == XZ) || (b[i].g.iPlane == YZ))) {
				x4 = b[i].g.zC + b[i].g.R_out_cyl;
			}
			if ((b[i].g.itypegeom == 1) && ((b[i].g.iPlane == XY))) {
				if (b[i].g.Hcyl > 0.0) {
					x4 = b[i].g.zC + b[i].g.Hcyl;
				}
				else {
					x4 = b[i].g.zC;
				}
			}
			for (j = 0; j <= inz; j++) {
				if (fabs(x4) > 0.0) {
					// ������������� ����������� ����� 0.15%.
					if (fabs(100 * (zpos[j] - x4) / fabs(x4)) < otnositelnaq_tolerance_eps) {
						block_indexes[i_1].kR = j;
						break;
					}
				}
				else {
					// ���������� �����������.
					if (fabs(zpos[j] - x4) < 1.0e-40) {
						block_indexes[i_1].kR = j;
						break;
					}
				}
			}

			//i_1++;
			i_1--;
		}

	}

	// ���������� �������� ����������� ����������� � � ����� ��� �������� � �������������
	// ���������� ��������������.
	integer m7= lb-1, m8;//  0 m7= lb - 1,
//#pragma omp parallel for
	//for (integer i1 = 0; i1 < inx; i1++) for (integer j1 = 0; j1 < iny; j1++) for (integer k1 = 0; k1 < inz; k1++) {
		//integer iP = i1 + j1 * inx + k1 * inx*iny;
#pragma omp parallel for
	for (integer iP=0; iP<inx*iny*inz; iP++) {
		evt[iP] = -1;
		bvisit[iP] = false;
	}
   // for (m8 = 0; m8 < lb; m8++) {
	for (m8 = lb-1; m8 >= 0; m8--) {
		// ������� ���� ���� � ��������� �����������, �� �� ������������� ������ ������.
		if (b[m8].g.itypegeom == 0) {
#pragma omp parallel for
			for (integer i1 = block_indexes[m7].iL; i1 < block_indexes[m7].iR; i1++) for (integer j1 = block_indexes[m7].jL; j1 < block_indexes[m7].jR; j1++) for (integer k1 = block_indexes[m7].kL; k1 < block_indexes[m7].kR; k1++) {
				integer iP = i1 + j1 * inx + k1 * inx*iny;
				if (bvisit[iP] == false)
				{

					bvisit[iP] = true;

					switch (iflag) {
					case TEMPERATURE:
						if (b[m8].itype == HOLLOW) {
							evt[iP] = -1;
						}
						else {
							evt[iP] = m8;
						}
						break;
					case HYDRODINAMIC:
						if ((b[m8].itype == SOLID) || (b[m8].itype == HOLLOW)) {
							evt[iP] = -1;
						}
						else {
							evt[iP] = m8;
						}
						break;
					}
				}
			}
			m7--;
			
		}
		else if (b[m8].g.itypegeom == 1) {

			// TODO ��� ��� ����������� �������������� ������ ��� �������� ? 
			// ���� ����� ��������� ����� ����������� �����.
			// Cylinder
			//for (integer i1 = 0; i1 < inx; i1++) for (integer j1 = 0; j1 < iny; j1++) for (integer k1 = 0; k1 < inz; k1++) {
//#pragma omp parallel for
			for (integer i1 = block_indexes[m7].iL; i1 < block_indexes[m7].iR; i1++) for (integer j1 = block_indexes[m7].jL; j1 < block_indexes[m7].jR; j1++) for (integer k1 = block_indexes[m7].kL; k1 < block_indexes[m7].kR; k1++) {
				integer iP = i1 + j1 * inx + k1 * inx*iny;
				
				if (bvisit[iP] == false)
				{

					TOCHKA p;
					p.x = 0.5*(xpos[i1] + xpos[i1 + 1]);
					p.y = 0.5*(ypos[j1] + ypos[j1 + 1]);
					p.z = 0.5*(zpos[k1] + zpos[k1 + 1]);

					switch (b[m8].g.iPlane) {
					case XY:
						if (fabs(b[m8].g.R_in_cyl) < 1.0e-40) {
							if ((p.z > b[m8].g.zC) && (p.z < b[m8].g.zC + b[m8].g.Hcyl)) {
								if (sqrt((b[m8].g.xC - p.x)*(b[m8].g.xC - p.x) + (b[m8].g.yC - p.y)*(b[m8].g.yC - p.y)) < b[m8].g.R_out_cyl) {

									bvisit[iP] = true;

									switch (iflag) {
									case TEMPERATURE:
										if (b[m8].itype == HOLLOW) {
											evt[iP] = -1;
										}
										else {
											evt[iP] = m8;
										}
										break;
									case HYDRODINAMIC:
										if ((b[m8].itype == SOLID) || (b[m8].itype == HOLLOW)) {
											evt[iP] = -1;
										}
										else {
											evt[iP] = m8;
										}
										break;
									}
								}
							}
						}
						else {
							if ((p.z > b[m8].g.zC) && (p.z < b[m8].g.zC + b[m8].g.Hcyl)) {
								if (sqrt((b[m8].g.xC - p.x)*(b[m8].g.xC - p.x) + (b[m8].g.yC - p.y)*(b[m8].g.yC - p.y)) < b[m8].g.R_out_cyl) {
									if (sqrt((b[m8].g.xC - p.x)*(b[m8].g.xC - p.x) + (b[m8].g.yC - p.y)*(b[m8].g.yC - p.y)) > b[m8].g.R_in_cyl) {

										bvisit[iP] = true;

										switch (iflag) {
										case TEMPERATURE:
											if (b[m8].itype == HOLLOW) {
												evt[iP] = -1;
											}
											else {
												evt[iP] = m8;
											}
											break;
										case HYDRODINAMIC:
											if ((b[m8].itype == SOLID) || (b[m8].itype == HOLLOW)) {
												evt[iP] = -1;
											}
											else {
												evt[iP] = m8;
											}
											break;
										}
									}
								}
							}
						}
						break;
					case XZ:
						if (fabs(b[m8].g.R_in_cyl) < 1.0e-40) {
							if ((p.y > b[m8].g.yC) && (p.y < b[m8].g.yC + b[m8].g.Hcyl)) {
								if (sqrt((b[m8].g.xC - p.x)*(b[m8].g.xC - p.x) + (b[m8].g.zC - p.z)*(b[m8].g.zC - p.z)) < b[m8].g.R_out_cyl) {

									bvisit[iP] = true;

									switch (iflag) {
									case TEMPERATURE:
										if (b[m8].itype == HOLLOW) {
											evt[iP] = -1;
										}
										else {
											evt[iP] = m8;
										}
										break;
									case HYDRODINAMIC:
										if ((b[m8].itype == SOLID) || (b[m8].itype == HOLLOW)) {
											evt[iP] = -1;
										}
										else {
											evt[iP] = m8;
										}
										break;
									}
								}
							}
						}
						else {
							if ((p.y > b[m8].g.yC) && (p.y < b[m8].g.yC + b[m8].g.Hcyl)) {
								if (sqrt((b[m8].g.xC - p.x)*(b[m8].g.xC - p.x) + (b[m8].g.zC - p.z)*(b[m8].g.zC - p.z)) < b[m8].g.R_out_cyl) {
									if (sqrt((b[m8].g.xC - p.x)*(b[m8].g.xC - p.x) + (b[m8].g.zC - p.z)*(b[m8].g.zC - p.z)) > b[m8].g.R_in_cyl) {

										bvisit[iP] = true;

										switch (iflag) {
										case TEMPERATURE:
											if (b[m8].itype == HOLLOW) {
												evt[iP] = -1;
											}
											else {
												evt[iP] = m8;
											}
											break;
										case HYDRODINAMIC:
											if ((b[m8].itype == SOLID) || (b[m8].itype == HOLLOW)) {
												evt[iP] = -1;
											}
											else {
												evt[iP] = m8;
											}
											break;
										}
									}
								}
							}
						}
						break;
					case YZ:
						if (fabs(b[m8].g.R_in_cyl) < 1.0e-40) {
							if ((p.x > b[m8].g.xC) && (p.x < b[m8].g.xC + b[m8].g.Hcyl)) {
								if (sqrt((b[m8].g.yC - p.y)*(b[m8].g.yC - p.y) + (b[m8].g.zC - p.z)*(b[m8].g.zC - p.z)) < b[m8].g.R_out_cyl) {

									bvisit[iP] = true;

									switch (iflag) {
									case TEMPERATURE:
										if (b[m8].itype == HOLLOW) {
											evt[iP] = -1;
										}
										else {
											evt[iP] = m8;
										}
										break;
									case HYDRODINAMIC:
										if ((b[m8].itype == SOLID) || (b[m8].itype == HOLLOW)) {
											evt[iP] = -1;
										}
										else {
											evt[iP] = m8;
										}
										break;
									}
								}
							}
						}
						else {
							if ((p.x > b[m8].g.xC) && (p.x < b[m8].g.xC + b[m8].g.Hcyl)) {
								if (sqrt((b[m8].g.yC - p.y)*(b[m8].g.yC - p.y) + (b[m8].g.zC - p.z)*(b[m8].g.zC - p.z)) < b[m8].g.R_out_cyl) {
									if (sqrt((b[m8].g.yC - p.y)*(b[m8].g.yC - p.y) + (b[m8].g.zC - p.z)*(b[m8].g.zC - p.z)) > b[m8].g.R_in_cyl) {
										bvisit[iP] = true;

										switch (iflag) {
										case TEMPERATURE:
											if (b[m8].itype == HOLLOW) {
												evt[iP] = -1;
											}
											else {
												evt[iP] = m8;
											}
											break;
										case HYDRODINAMIC:
											if ((b[m8].itype == SOLID) || (b[m8].itype == HOLLOW)) {
												evt[iP] = -1;
											}
											else {
												evt[iP] = m8;
											}
											break;
										}
									}
								}
							}
						}
						break;
					}
				}
			}
			m7--;
		}
		else if (b[m8].g.itypegeom == 2) {
			
			// polygon
			// �� ��������� ����� ����������� ����� 
			// ������������ ������ ����� ������ ����������� ������������� ������.

			

            #pragma omp parallel for
			for (integer i1 = block_indexes[m7].iL; i1 < block_indexes[m7].iR; i1++) for (integer j1 = block_indexes[m7].jL; j1 < block_indexes[m7].jR; j1++) for (integer k1 = block_indexes[m7].kL; k1 < block_indexes[m7].kR; k1++) {

				integer iP = i1 + j1 * inx + k1 * inx*iny;

				if (bvisit[iP] == false)
				{

					//for (integer i1 = 0; i1 < inx; i1++) for (integer j1 = 0; j1 < iny; j1++) for (integer k1 = 0; k1 < inz; k1++) {
					TOCHKA p;
					p.x = 0.5*(xpos[i1] + xpos[i1 + 1]);
					p.y = 0.5*(ypos[j1] + ypos[j1 + 1]);
					p.z = 0.5*(zpos[k1] + zpos[k1 + 1]);

					integer k74 = -1;
					if (in_polygon(p, b[m8].g.nsizei, b[m8].g.xi, b[m8].g.yi, b[m8].g.zi, b[m8].g.hi, b[m8].g.iPlane_obj2, k74, m8)) {
						//printf("i1=%d j1=%d k1=%d inx*iny*inz=%d\n",i1,j1,k1, inx*iny*inz);
						//printf("iL=%d iR=%d jL=%d jR=%d kL=%d kR=%d\n", block_indexes[m7].iL, block_indexes[m7].iR, block_indexes[m7].jL, block_indexes[m7].jR, block_indexes[m7].kL, block_indexes[m7].kR);

						bvisit[iP] = true;

						switch (iflag) {
						case TEMPERATURE:
							if (b[m8].itype == HOLLOW) {
								evt[iP] = -1;
							}
							else {
								evt[iP] = m8;
							}
							break;
						case HYDRODINAMIC:
							if ((b[m8].itype == SOLID) || (b[m8].itype == HOLLOW)) {
								evt[iP] = -1;
							}
							else {
								evt[iP] = m8;
							}
							break;
						}
					}

				}
			}
			m7--;
		}
		
	}

	if (bvisit != NULL) {
		delete[] bvisit;
		bvisit = NULL;
	}
	

	printf("enumerate_volume_improved 80 procent.\n");
	// ��������� � evt ������� � �������.
	// ���� �� ����������� ��������� ������� �� ����� 0.
	integer l = 1, ib;
	for (i = 0; i < inx; i++) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
		integer iP = i + j * inx + k * inx*iny;
		if (evt[iP] > -1) {
			ib = evt[iP]; // ����� ����� ��� ������� �����.
											 // ��� ����� ����� ��� ������ �������.
			whot_is_block[l - 1] = ib; // ����� ����� �������� ����������� ����� (p.x,p.y,p.z).
			tck_int_list[l - 1].i = i;
			tck_int_list[l - 1].j = j;
			tck_int_list[l - 1].k = k;
			evt[iP] = l;
			l++;
		}
		else
		{   // �� ����������� ��������� �������
			evt[iP] = 0;
		}
	}
	maxelm = l - 1;

	if (block_indexes != NULL) {
		delete[] block_indexes;
		block_indexes = NULL;
	}

	printf("enumerate_volume_improved_obobshenie end.\n");

	/*
	bool inDomain = false;
	// ��������� ����������� ������� ���������� � �������
	// ���� ����������� ����� �� ����������� ��������� ������� �� �������� 0.
	for (i = 0; i<inx; i++) for (j = 0; j<iny; j++) for (k = 0; k<inz; k++) {
	p.x = 0.5*(xpos[i] + xpos[i + 1]);
	p.y = 0.5*(ypos[j] + ypos[j + 1]);
	p.z = 0.5*(zpos[k] + zpos[k + 1]);
	switch (iflag) {
	case TEMPERATURE: inDomain = in_model_temp(p, ib, b, lb); break;
	case HYDRODINAMIC: inDomain = in_model_flow(p, ib, b, lb); break;
	}
	if (inDomain) {
	// ����������� ��������� �������
	evt[i + j*inx + k*inx*iny] = l;
	// ��� ����� ����� ��� ������ �������.
	whot_is_block[l - 1] = ib; // ����� ����� �������� ����������� ����� (p.x,p.y,p.z).
	l++;
	}
	else
	{   // �� ����������� ��������� �������
	evt[i + j*inx + k*inx*iny] = 0;
	}
	}

	// maxelm - ����� ����������� ������� ������������� ��������� �������
	maxelm = l - 1;
	*/

	omp_set_num_threads(i_my_num_core_parallelesation);

} // enumerate_volume_improved_obobshenie



  // ���������� ��������� ����������� �������
  // ��� ����� ����������������
void enumerate_volume(integer* &evt, integer &maxelm, integer iflag, doublereal* xpos, doublereal* ypos, doublereal* zpos, integer* &whot_is_block,
	integer inx, integer iny, integer inz, BLOCK* b, integer lb, integer lu, UNION* &my_union, integer &iunion_id_p1, TOCKA_INT* &tck_int_list) {

	

	if (lu==0) {
		// 29.12.2017
		// �������� ����� � ������ ��������� � ���������.
		enumerate_volume_improved_obobshenie(evt, maxelm, iflag, xpos, ypos, zpos, whot_is_block, inx, iny, inz, b, lb, tck_int_list);
	}
	else {

		if (lu > 0) {

			// 25.04.2018
			// �������� � ��� ���������.

			// ������������ ����� � ��������.

			tck_int_list=new TOCKA_INT[inx*iny*inz];
			if (tck_int_list == NULL) {
				// ������������ ������ �� ������ ������������.
				printf("Problem : not enough memory on your equipment for evt tck_int_list...\n");
				printf("Please any key to exit...\n");
				exit(1);
			}

			evt = NULL;
			evt = new integer[inx*iny*inz];
			if (evt == NULL) {
				// ������������ ������ �� ������ ������������.
				printf("Problem : not enough memory on your equipment for evt constr struct...\n");
				printf("Please any key to exit...\n");
				exit(1);
			}
			whot_is_block = NULL;
			whot_is_block = new integer[inx*iny*inz];
			if (whot_is_block == NULL) {
				// ������������ ������ �� ������ ������������.
				printf("Problem : not enough memory on your equipment for whot_is_block constr struct...\n");
				printf("Please any key to exit...\n");
				exit(1);
			}
			TOCHKA p;
			// ��������� � evt ������� � �������.
			// ���� �� ����������� ��������� ������� �� ����� 0.
			integer l = 1, ib;
			integer i, j, k;
			bool inDomain = false;
			// ��������� ����������� ������� ���������� � �������
			// ���� ����������� ����� �� ����������� ��������� ������� �� �������� 0.
			for (i = 0; i < inx; i++) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				p.x = 0.5*(xpos[i] + xpos[i + 1]);
				p.y = 0.5*(ypos[j] + ypos[j + 1]);
				p.z = 0.5*(zpos[k] + zpos[k + 1]);
				switch (iflag) {
				case TEMPERATURE: inDomain = in_model_temp(p, ib, b, lb); break;
				case HYDRODINAMIC: inDomain = in_model_flow(p, ib, b, lb); break;
				}
				if (iunion_id_p1 == 0) {
					for (integer iu = 0; iu < lu; iu++) {
						if ((p.x > my_union[iu].xS) && (p.x < my_union[iu].xE) &&
							(p.y > my_union[iu].yS) && (p.y < my_union[iu].yE) &&
							(p.z > my_union[iu].zS) && (p.z < my_union[iu].zE)) {
							// ���� ������������� ������� � �����������, �� ������� ������
							// ���������� ���� Hollow ����.
							inDomain = false;
						}
					}
				}
				else {
					if ((p.x > my_union[iunion_id_p1-1].xS) && (p.x < my_union[iunion_id_p1-1].xE) &&
						(p.y > my_union[iunion_id_p1-1].yS) && (p.y < my_union[iunion_id_p1-1].yE) &&
						(p.z > my_union[iunion_id_p1-1].zS) && (p.z < my_union[iunion_id_p1-1].zE)) {
						// ���� ������������� ������� � �����������, �� ������� ������
						// ���������� ���� Hollow ����.
						//inDomain = false;
						// ��������� ��� �� �����.
						if (inDomain) {
						//	printf("in our assembles\n");
						//	getchar();
						}
					}
				}


				if (inDomain) {
					// ����������� ��������� �������
					evt[i + j*inx + k*inx*iny] = l;
					// ��� ����� ����� ��� ������ �������.
					whot_is_block[l - 1] = ib; // ����� ����� �������� ����������� ����� (p.x,p.y,p.z).
					tck_int_list[l - 1].i = i;
					tck_int_list[l - 1].j = j;
					tck_int_list[l - 1].k = k;
					l++;
				}
				else
				{   // �� ����������� ��������� �������
					evt[i + j*inx + k*inx*iny] = 0;
				}
			}

			// maxelm - ����� ����������� ������� ������������� ��������� �������
			maxelm = l - 1;
		}

		
		else {
			bool b_only_Prism_object = true;
			for (integer m7 = 0; m7 < lb; m7++) {
				if (b[m7].g.itypegeom != 0) {
					b_only_Prism_object = false;
					break;
				}
			}

			if (b_only_Prism_object) {
				// ��� ������ ��������� ������ �� ������������� �������.
				enumerate_volume_improved(evt, maxelm, iflag, xpos, ypos, zpos, whot_is_block, inx, iny, inz, b, lb);
			}
			else {
				// ������������ ����� � ��������.

				evt = NULL;
				evt = new integer[inx*iny*inz];
				if (evt == NULL) {
					// ������������ ������ �� ������ ������������.
					printf("Problem : not enough memory on your equipment for evt constr struct...\n");
					printf("Please any key to exit...\n");
					exit(1);
				}
				whot_is_block = NULL;
				whot_is_block = new integer[inx*iny*inz];
				if (whot_is_block == NULL) {
					// ������������ ������ �� ������ ������������.
					printf("Problem : not enough memory on your equipment for whot_is_block constr struct...\n");
					printf("Please any key to exit...\n");
					exit(1);
				}
				TOCHKA p;
				// ��������� � evt ������� � �������.
				// ���� �� ����������� ��������� ������� �� ����� 0.
				integer l = 1, ib;
				integer i, j, k;
				bool inDomain = false;
				// ��������� ����������� ������� ���������� � �������
				// ���� ����������� ����� �� ����������� ��������� ������� �� �������� 0.
				for (i = 0; i < inx; i++) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
					p.x = 0.5*(xpos[i] + xpos[i + 1]);
					p.y = 0.5*(ypos[j] + ypos[j + 1]);
					p.z = 0.5*(zpos[k] + zpos[k + 1]);
					switch (iflag) {
					case TEMPERATURE: inDomain = in_model_temp(p, ib, b, lb); break;
					case HYDRODINAMIC: inDomain = in_model_flow(p, ib, b, lb); break;
					}
					if (inDomain) {
						// ����������� ��������� �������
						evt[i + j*inx + k*inx*iny] = l;
						// ��� ����� ����� ��� ������ �������.
						whot_is_block[l - 1] = ib; // ����� ����� �������� ����������� ����� (p.x,p.y,p.z).
						tck_int_list[l - 1].i = i;
						tck_int_list[l - 1].j = j;
						tck_int_list[l - 1].k = k;
						l++;
					}
					else
					{   // �� ����������� ��������� �������
						evt[i + j*inx + k*inx*iny] = 0;
					}
				}

				// maxelm - ����� ����������� ������� ������������� ��������� �������
				maxelm = l - 1;
			}
		}

		
	}
} // enumerate_volume

  // ������������� evt_f. �� ����� ��� �������, ����� ������� ������������ ������� �����.
  // � ������� ����� ����� ������������ ������� ��� ������������� ��������� ������� ��� ����������
  // FLUID �������� ��������� ���� ���������� �������������, ����� �� ����� ����������.
void init_evt_f_alice_improved(integer* &evt, integer iflag, doublereal* xpos, doublereal* ypos, doublereal* zpos,
	integer inx, integer iny, integer inz, BLOCK* b, integer lb, TOCKA_INT* &tck_int_list) {

	tck_int_list = new TOCKA_INT[inx*iny*inz];
	if (tck_int_list == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for evt tck_int_list...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}

	evt = NULL;
	evt = new integer[inx*iny*inz];
	if (evt == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for evt constr struct...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}

	//TOCHKA p;

	printf("enumerate_volume_improved start.\n");
	Block_indexes* block_indexes = new Block_indexes[lb];
	if (block_indexes == NULL) {
		printf("error in allocation memory for block_indexes in init_evt_f_alice_improved.\n");
		system("pause");
		exit(1);
	}

	integer i, j, k;

	for (i = 0; i < lb; i++) {
		doublereal x4 = b[i].g.xS;
		for (j = 0; j <= inx; j++) {
			if (fabs(xpos[j] - x4) < 1.0e-40) {
				block_indexes[i].iL = j;
				break;
			}
		}
		x4 = b[i].g.xE;
		for (j = 0; j <= inx; j++) {
			if (fabs(xpos[j] - x4) < 1.0e-40) {
				block_indexes[i].iR = j;
				break;
			}
		}
		x4 = b[i].g.yS;
		for (j = 0; j <= iny; j++) {
			if (fabs(ypos[j] - x4) < 1.0e-40) {
				block_indexes[i].jL = j;
				break;
			}
		}
		x4 = b[i].g.yE;
		for (j = 0; j <= iny; j++) {
			if (fabs(ypos[j] - x4) < 1.0e-40) {
				block_indexes[i].jR = j;
				break;
			}
		}
		x4 = b[i].g.zS;
		for (j = 0; j <= inz; j++) {
			if (fabs(zpos[j] - x4) < 1.0e-40) {
				block_indexes[i].kL = j;
				break;
			}
		}
		x4 = b[i].g.zE;
		for (j = 0; j <= inz; j++) {
			if (fabs(zpos[j] - x4) < 1.0e-40) {
				block_indexes[i].kR = j;
				break;
			}
		}
	}

	// ���������� �������� ����������� ����������� � � ����� ��� �������� � �������������
	// ���������� ��������������.
	integer m7;
	for (i = 0; i < inx; i++) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
		evt[i + j*inx + k*inx*iny] = -1;
	}
	for (m7 = 0; m7 < lb; m7++) {
		for (i = block_indexes[m7].iL; i < block_indexes[m7].iR; i++) for (j = block_indexes[m7].jL; j < block_indexes[m7].jR; j++) for (k = block_indexes[m7].kL; k < block_indexes[m7].kR; k++) {
			switch (iflag) {
			case TEMPERATURE:
				if (b[m7].itype == HOLLOW) {
					evt[i + j*inx + k*inx*iny] = -1;
				}
				else {
					evt[i + j*inx + k*inx*iny] = m7;
				}
				break;
			case HYDRODINAMIC:
				if ((b[m7].itype == SOLID) || (b[m7].itype == HOLLOW)) {
					evt[i + j*inx + k*inx*iny] = -1;
				}
				else {
					evt[i + j*inx + k*inx*iny] = m7;
				}
				break;
			}

		}
	}
	printf("enumerate_volume_improved 80 procent.\n");
	// ��������� � evt ������� � �������.
	// ���� �� ����������� ��������� ������� �� ����� 0.
	integer l = 1, ib;
	for (i = 0; i < inx; i++) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
		if (evt[i + j*inx + k*inx*iny] > -1) {
			ib = evt[i + j*inx + k*inx*iny]; // ����� ����� ��� ������� �����.
											 // ��� ����� ����� ��� ������ �������.
											 //whot_is_block[l - 1] = ib; // ����� ����� �������� ����������� ����� (p.x,p.y,p.z).
			evt[i + j*inx + k*inx*iny] = l;

			tck_int_list[l - 1].i = i;
			tck_int_list[l - 1].j = j;
			tck_int_list[l - 1].k = k;
			l++;
		}
		else {
			// �� ����������� ��������� �������
			evt[i + j*inx + k*inx*iny] = 0;
		}
	}


	delete[] block_indexes;

	printf("enumerate_volume_improved end.\n");
	/*
	// ��������� � evt ������� � �������.
	// ���� �� ����������� ��������� ������� �� ����� 0.
	integer l = 1, ib;
	integer i, j, k;
	bool inDomain = false;
	// ��������� ����������� ������� ���������� � �������
	// ���� ����������� ����� �� ����������� ��������� ������� �� �������� 0.
	for (i = 0; i<inx; i++) for (j = 0; j<iny; j++) for (k = 0; k<inz; k++) {
	p.x = 0.5*(xpos[i] + xpos[i + 1]);
	p.y = 0.5*(ypos[j] + ypos[j + 1]);
	p.z = 0.5*(zpos[k] + zpos[k + 1]);
	switch (iflag) {
	case TEMPERATURE: inDomain = in_model_temp(p, ib, b, lb); break;
	case HYDRODINAMIC: inDomain = in_model_flow(p, ib, b, lb); break;
	}
	if (inDomain) {
	// ����������� ��������� �������
	evt[i + j*inx + k*inx*iny] = l;
	l++;
	}
	else
	{   // �� ����������� ��������� �������
	evt[i + j*inx + k*inx*iny] = 0;
	}
	}
	*/

} // init_evt_f_alice_improved.


  // ������������� evt_f. �� ����� ��� �������, ����� ������� ������������ ������� �����.
  // � ������� ����� ����� ������������ ������� ��� ������������� ��������� ������� ��� ����������
  // FLUID �������� ��������� ���� ���������� �������������, ����� �� ����� ����������.
void init_evt_f_alice_improved_obobshenie(integer* &evt, integer iflag, doublereal* xpos, doublereal* ypos, doublereal* zpos,
	integer inx, integer iny, integer inz, BLOCK* b, integer lb, TOCKA_INT* &tck_int_list) {


	tck_int_list = new TOCKA_INT[inx*iny*inz];
	if (tck_int_list == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for evt tck_int_list...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}

	evt = NULL;
	evt = new integer[inx*iny*inz];
	if (evt == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for evt constr struct...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}

	//TOCHKA p;

	printf("enumerate_volume_improved start.\n");
	Block_indexes* block_indexes = new Block_indexes[lb];
	if (block_indexes == NULL) {
		printf("error in allocation memory for block_indexes in init_evt_f_alice_improved.\n");
		system("pause");
		exit(1);
	}

	integer i, j, k, i_1 = 0;

	// ����������� ������ ���������� � �������������.
	// ������������ ����� � ��� �������������� � �������� ���������.
	// ����� ������������ ������������� ����������� � 0.15%.
	const doublereal otnositelnaq_tolerance_eps = 0.0015; // 0.15%

	for (i = 0; i < lb; i++) {
		//if (b[i].g.itypegeom == 0) {

		// polygon (b[i].g.itypegeom == 2)
		// �������� ������������� ������ xS, xE, yS, yE, zS, zE - ������ �����������
		// ������� ������������� ������, ��� �������� ��������� �������������� ����� ��������
		// ������ ��� ����� ����� ����������� ������ ������ ������������� ������, ��� ������ 
		// �������� ���������.
		if ((b[i].g.itypegeom == 0) || (b[i].g.itypegeom == 2)) {

			doublereal x4 = b[i].g.xS;
			for (j = 0; j <= inx; j++) {
				if (fabs(x4) > 0.0) {
					// ������������� ����������� ����� 0.15%.
					if (fabs(100 * (xpos[j] - x4) / fabs(x4)) < otnositelnaq_tolerance_eps) {
						block_indexes[i_1].iL = j;
						break;
					}
				}
				else {
					// ���������� �����������.
					if (fabs(xpos[j] - x4) < 1.0e-40) {
						block_indexes[i_1].iL = j;
						break;
					}
				}
			}
			x4 = b[i].g.xE;
			for (j = 0; j <= inx; j++) {
				if (fabs(x4) > 0.0) {
					// ������������� ����������� ����� 0.15%.
					if (fabs(100 * (xpos[j] - x4) / fabs(x4)) < otnositelnaq_tolerance_eps) {
						block_indexes[i_1].iR = j;
						break;
					}
				}
				else {
					// ���������� �����������.
					if (fabs(xpos[j] - x4) < 1.0e-40) {
						block_indexes[i_1].iR = j;
						break;
					}
				}
			}
			x4 = b[i].g.yS;
			for (j = 0; j <= iny; j++) {
				if (fabs(x4) > 0.0) {
					// ������������� ����������� ����� 0.15%.
					if (fabs(100 * (ypos[j] - x4) / fabs(x4)) < otnositelnaq_tolerance_eps) {
						block_indexes[i_1].jL = j;
						break;
					}
				}
				else {
					// ���������� �����������.
					if (fabs(ypos[j] - x4) < 1.0e-40) {
						block_indexes[i_1].jL = j;
						break;
					}
				}
			}
			x4 = b[i].g.yE;
			for (j = 0; j <= iny; j++) {

				if (fabs(x4) > 0.0) {
					// ������������� ����������� ����� 0.15%.
					if (fabs(100 * (ypos[j] - x4) / fabs(x4)) < otnositelnaq_tolerance_eps) {
						block_indexes[i_1].jR = j;
						break;
					}
				}
				else {
					// ���������� �����������.
					if (fabs(ypos[j] - x4) < 1.0e-40) {
						block_indexes[i_1].jR = j;
						break;
					}
				}
			}
			x4 = b[i].g.zS;
			for (j = 0; j <= inz; j++) {
				if (fabs(x4) > 0.0) {
					// ������������� ����������� ����� 0.15%.
					if (fabs(100 * (zpos[j] - x4) / fabs(x4)) < otnositelnaq_tolerance_eps) {
						block_indexes[i_1].kL = j;
						break;
					}
				}
				else {
					// ���������� �����������.
					if (fabs(zpos[j] - x4) < 1.0e-40) {
						block_indexes[i_1].kL = j;
						break;
					}
				}
			}
			x4 = b[i].g.zE;
			for (j = 0; j <= inz; j++) {
				if (fabs(x4) > 0.0) {
					// ������������� ����������� ����� 0.15%.
					if (fabs(100 * (zpos[j] - x4) / fabs(x4)) < otnositelnaq_tolerance_eps) {
						block_indexes[i_1].kR = j;
						break;
					}
				}
				else {
					// ���������� �����������.
					if (fabs(zpos[j] - x4) < 1.0e-40) {
						block_indexes[i_1].kR = j;
						break;
					}
				}
			}

			i_1++;
		}

	}

	// ���������� �������� ����������� ����������� � � ����� ��� �������� � �������������
	// ���������� ��������������.
	
	for (i = 0; i < inx; i++) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
		evt[i + j*inx + k*inx*iny] = -1;
	}
	integer m7 = 0, m8;

	for (m8 = 0; m8 < lb; m8++) {
		if (b[m8].g.itypegeom == 0) {
#pragma omp parallel for
			for (integer i1 = block_indexes[m7].iL; i1 < block_indexes[m7].iR; i1++) for (integer j1 = block_indexes[m7].jL; j1 < block_indexes[m7].jR; j1++) for (integer k1 = block_indexes[m7].kL; k1 < block_indexes[m7].kR; k1++) {
				switch (iflag) {
				case TEMPERATURE:
					if (b[m8].itype == HOLLOW) {
						evt[i1 + j1*inx + k1*inx*iny] = -1;
					}
					else {
						evt[i1 + j1*inx + k1*inx*iny] = m8;
					}
					break;
				case HYDRODINAMIC:
					if ((b[m8].itype == SOLID) || (b[m8].itype == HOLLOW)) {
						evt[i1 + j1*inx + k1*inx*iny] = -1;
					}
					else {
						evt[i1 + j1*inx + k1*inx*iny] = m8;
					}
					break;
				}

			}
			m7++;
		}
		else if (b[m8].g.itypegeom == 1) {

			// TODO ��� ��� ����������� �������������� ������ ��� �������� ? 
			// ���� ����� ��������� ����� ����������� �����.
			// Cylinder
			for (integer i1 = 0; i1 < inx; i1++) for (integer j1 = 0; j1 < iny; j1++) for (integer k1 = 0; k1 < inz; k1++) {
				TOCHKA p;
				p.x = 0.5*(xpos[i1] + xpos[i1 + 1]);
				p.y = 0.5*(ypos[j1] + ypos[j1 + 1]);
				p.z = 0.5*(zpos[k1] + zpos[k1 + 1]);

				switch (b[m8].g.iPlane) {
				case XY:
					if (fabs(b[m8].g.R_in_cyl) < 1.0e-40) {
						if ((p.z > b[m8].g.zC) && (p.z < b[m8].g.zC + b[m8].g.Hcyl)) {
							if (sqrt((b[m8].g.xC - p.x)*(b[m8].g.xC - p.x) + (b[m8].g.yC - p.y)*(b[m8].g.yC - p.y)) < b[m8].g.R_out_cyl) {
								switch (iflag) {
								case TEMPERATURE:
									if (b[m8].itype == HOLLOW) {
										evt[i1 + j1*inx + k1*inx*iny] = -1;
									}
									else {
										evt[i1 + j1*inx + k1*inx*iny] = m8;
									}
									break;
								case HYDRODINAMIC:
									if ((b[m8].itype == SOLID) || (b[m8].itype == HOLLOW)) {
										evt[i1 + j1*inx + k1*inx*iny] = -1;
									}
									else {
										evt[i1 + j1*inx + k1*inx*iny] = m8;
									}
									break;
								}
							}
						}
					}
					else {
						if ((p.z > b[m8].g.zC) && (p.z < b[m8].g.zC + b[m8].g.Hcyl)) {
							if (sqrt((b[m8].g.xC - p.x)*(b[m8].g.xC - p.x) + (b[m8].g.yC - p.y)*(b[m8].g.yC - p.y)) < b[m8].g.R_out_cyl) {
								if (sqrt((b[m8].g.xC - p.x)*(b[m8].g.xC - p.x) + (b[m8].g.yC - p.y)*(b[m8].g.yC - p.y)) > b[m8].g.R_in_cyl) {
									switch (iflag) {
									case TEMPERATURE:
										if (b[m8].itype == HOLLOW) {
											evt[i1 + j1*inx + k1*inx*iny] = -1;
										}
										else {
											evt[i1 + j1*inx + k1*inx*iny] = m8;
										}
										break;
									case HYDRODINAMIC:
										if ((b[m8].itype == SOLID) || (b[m8].itype == HOLLOW)) {
											evt[i1 + j1*inx + k1*inx*iny] = -1;
										}
										else {
											evt[i1 + j1*inx + k1*inx*iny] = m8;
										}
										break;
									}
								}
							}
						}
					}
					break;
				case XZ:
					if (fabs(b[m8].g.R_in_cyl) < 1.0e-40) {
						if ((p.y > b[m8].g.yC) && (p.y < b[m8].g.yC + b[m8].g.Hcyl)) {
							if (sqrt((b[m8].g.xC - p.x)*(b[m8].g.xC - p.x) + (b[m8].g.zC - p.z)*(b[m8].g.zC - p.z)) < b[m8].g.R_out_cyl) {
								switch (iflag) {
								case TEMPERATURE:
									if (b[m8].itype == HOLLOW) {
										evt[i1 + j1*inx + k1*inx*iny] = -1;
									}
									else {
										evt[i1 + j1*inx + k1*inx*iny] = m8;
									}
									break;
								case HYDRODINAMIC:
									if ((b[m8].itype == SOLID) || (b[m8].itype == HOLLOW)) {
										evt[i1 + j1*inx + k1*inx*iny] = -1;
									}
									else {
										evt[i1 + j1*inx + k1*inx*iny] = m8;
									}
									break;
								}
							}
						}
					}
					else {
						if ((p.y > b[m8].g.yC) && (p.y < b[m8].g.yC + b[m8].g.Hcyl)) {
							if (sqrt((b[m8].g.xC - p.x)*(b[m8].g.xC - p.x) + (b[m8].g.zC - p.z)*(b[m8].g.zC - p.z)) < b[m8].g.R_out_cyl) {
								if (sqrt((b[m8].g.xC - p.x)*(b[m8].g.xC - p.x) + (b[m8].g.zC - p.z)*(b[m8].g.zC - p.z)) > b[m8].g.R_in_cyl) {
									switch (iflag) {
									case TEMPERATURE:
										if (b[m8].itype == HOLLOW) {
											evt[i1 + j1*inx + k1*inx*iny] = -1;
										}
										else {
											evt[i1 + j1*inx + k1*inx*iny] = m8;
										}
										break;
									case HYDRODINAMIC:
										if ((b[m8].itype == SOLID) || (b[m8].itype == HOLLOW)) {
											evt[i1 + j1*inx + k1*inx*iny] = -1;
										}
										else {
											evt[i1 + j1*inx + k1*inx*iny] = m8;
										}
										break;
									}
								}
							}
						}
					}
					break;
				case YZ:
					if (fabs(b[m8].g.R_in_cyl) < 1.0e-40) {
						if ((p.x > b[m8].g.xC) && (p.x < b[m8].g.xC + b[m8].g.Hcyl)) {
							if (sqrt((b[m8].g.yC - p.y)*(b[m8].g.yC - p.y) + (b[m8].g.zC - p.z)*(b[m8].g.zC - p.z)) < b[m8].g.R_out_cyl) {
								switch (iflag) {
								case TEMPERATURE:
									if (b[m8].itype == HOLLOW) {
										evt[i1 + j1*inx + k1*inx*iny] = -1;
									}
									else {
										evt[i1 + j1*inx + k1*inx*iny] = m8;
									}
									break;
								case HYDRODINAMIC:
									if ((b[m8].itype == SOLID) || (b[m8].itype == HOLLOW)) {
										evt[i1 + j1*inx + k1*inx*iny] = -1;
									}
									else {
										evt[i1 + j1*inx + k1*inx*iny] = m8;
									}
									break;
								}
							}
						}
					}
					else {
						if ((p.x > b[m8].g.xC) && (p.x < b[m8].g.xC + b[m8].g.Hcyl)) {
							if (sqrt((b[m8].g.yC - p.y)*(b[m8].g.yC - p.y) + (b[m8].g.zC - p.z)*(b[m8].g.zC - p.z)) < b[m8].g.R_out_cyl) {
								if (sqrt((b[m8].g.yC - p.y)*(b[m8].g.yC - p.y) + (b[m8].g.zC - p.z)*(b[m8].g.zC - p.z)) > b[m8].g.R_in_cyl) {
									switch (iflag) {
									case TEMPERATURE:
										if (b[m8].itype == HOLLOW) {
											evt[i1 + j1*inx + k1*inx*iny] = -1;
										}
										else {
											evt[i1 + j1*inx + k1*inx*iny] = m8;
										}
										break;
									case HYDRODINAMIC:
										if ((b[m8].itype == SOLID) || (b[m8].itype == HOLLOW)) {
											evt[i1 + j1*inx + k1*inx*iny] = -1;
										}
										else {
											evt[i1 + j1*inx + k1*inx*iny] = m8;
										}
										break;
									}
								}
							}
						}
					}
					break;
				}
			}
		}
		else if (b[m8].g.itypegeom == 2) {

			// polygon
			// �� ��������� ����� ����������� ����� 
			// ������������ ������ ����� ������ ����������� ������������� ������.



#pragma omp parallel for
			for (integer i1 = block_indexes[m7].iL; i1 < block_indexes[m7].iR; i1++) for (integer j1 = block_indexes[m7].jL; j1 < block_indexes[m7].jR; j1++) for (integer k1 = block_indexes[m7].kL; k1 < block_indexes[m7].kR; k1++) {

				//for (integer i1 = 0; i1 < inx; i1++) for (integer j1 = 0; j1 < iny; j1++) for (integer k1 = 0; k1 < inz; k1++) {
				TOCHKA p;
				p.x = 0.5*(xpos[i1] + xpos[i1 + 1]);
				p.y = 0.5*(ypos[j1] + ypos[j1 + 1]);
				p.z = 0.5*(zpos[k1] + zpos[k1 + 1]);

				integer k74 = -1;
				if (in_polygon(p, b[m8].g.nsizei, b[m8].g.xi, b[m8].g.yi, b[m8].g.zi, b[m8].g.hi, b[m8].g.iPlane_obj2, k74, m8)) {
					//printf("i1=%d j1=%d k1=%d inx*iny*inz=%d\n",i1,j1,k1, inx*iny*inz);
					//printf("iL=%d iR=%d jL=%d jR=%d kL=%d kR=%d\n", block_indexes[m7].iL, block_indexes[m7].iR, block_indexes[m7].jL, block_indexes[m7].jR, block_indexes[m7].kL, block_indexes[m7].kR);


					switch (iflag) {
					case TEMPERATURE:
						if (b[m8].itype == HOLLOW) {
							evt[i1 + j1*inx + k1*inx*iny] = -1;
						}
						else {
							evt[i1 + j1*inx + k1*inx*iny] = m8;
						}
						break;
					case HYDRODINAMIC:
						if ((b[m8].itype == SOLID) || (b[m8].itype == HOLLOW)) {
							evt[i1 + j1*inx + k1*inx*iny] = -1;
						}
						else {
							evt[i1 + j1*inx + k1*inx*iny] = m8;
						}
						break;
					}
				}

			}
			m7++;
		}
	}



	printf("enumerate_volume_improved 80 procent.\n");
	// ��������� � evt ������� � �������.
	// ���� �� ����������� ��������� ������� �� ����� 0.
	integer l = 1, ib;
	for (i = 0; i < inx; i++) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
		if (evt[i + j*inx + k*inx*iny] > -1) {
			ib = evt[i + j*inx + k*inx*iny]; // ����� ����� ��� ������� �����.
											 // ��� ����� ����� ��� ������ �������.
			//whot_is_block[l - 1] = ib; // ����� ����� �������� ����������� ����� (p.x,p.y,p.z).
			evt[i + j*inx + k*inx*iny] = l;
			tck_int_list[l - 1].i = i;
			tck_int_list[l - 1].j = j;
			tck_int_list[l - 1].k = k;
			l++;
		}
		else {
			// �� ����������� ��������� �������
			evt[i + j*inx + k*inx*iny] = 0;
		}
	}


	delete[] block_indexes;

	printf("enumerate_volume_improved end.\n");
	/*
	// ��������� � evt ������� � �������.
	// ���� �� ����������� ��������� ������� �� ����� 0.
	integer l = 1, ib;
	integer i, j, k;
	bool inDomain = false;
	// ��������� ����������� ������� ���������� � �������
	// ���� ����������� ����� �� ����������� ��������� ������� �� �������� 0.
	for (i = 0; i<inx; i++) for (j = 0; j<iny; j++) for (k = 0; k<inz; k++) {
	p.x = 0.5*(xpos[i] + xpos[i + 1]);
	p.y = 0.5*(ypos[j] + ypos[j + 1]);
	p.z = 0.5*(zpos[k] + zpos[k + 1]);
	switch (iflag) {
	case TEMPERATURE: inDomain = in_model_temp(p, ib, b, lb); break;
	case HYDRODINAMIC: inDomain = in_model_flow(p, ib, b, lb); break;
	}
	if (inDomain) {
	// ����������� ��������� �������
	evt[i + j*inx + k*inx*iny] = l;
	l++;
	}
	else
	{   // �� ����������� ��������� �������
	evt[i + j*inx + k*inx*iny] = 0;
	}
	}
	*/

} // init_evt_f_alice_improved_obobshenie.

// ������������� evt_f. �� ����� ��� �������, ����� ������� ������������ ������� �����.
// � ������� ����� ����� ������������ ������� ��� ������������� ��������� ������� ��� ����������
// FLUID �������� ��������� ���� ���������� �������������, ����� �� ����� ����������.
void init_evt_f_alice(integer* &evt,  integer iflag, doublereal* xpos, doublereal* ypos, doublereal* zpos, 
	integer inx, integer iny, integer inz, BLOCK* b, integer lb, TOCKA_INT* &tck_int_list) {


	bool b_only_Prism_object = true;
	for (integer m7 = 0; m7 < lb; m7++) {
		if (b[m7].g.itypegeom != 0) {
			b_only_Prism_object = false;
			break;
		}
	}


	if (1) {
		// 2.01.2018
		// ���������, ���������� �� ������ �� ������������� �������.
		// �������� �� ���������, ������������� ������� � ���������.
		init_evt_f_alice_improved_obobshenie(evt, iflag, xpos, ypos, zpos, inx, iny, inz, b, lb, tck_int_list);
	}
	else {
		if (b_only_Prism_object) {
			// ������ ��������� ������� �� ������������� �����.

			init_evt_f_alice_improved(evt, iflag, xpos, ypos, zpos, inx, iny, inz, b, lb, tck_int_list);
		}
		else {
			// � ������ ������������ �� ������ ������������� ������.

			tck_int_list = new TOCKA_INT[inx*iny*inz];
			if (tck_int_list == NULL) {
				// ������������ ������ �� ������ ������������.
				printf("Problem : not enough memory on your equipment for evt tck_int_list...\n");
				printf("Please any key to exit...\n");
				exit(1);
			}

			evt = NULL;
			evt = new integer[inx*iny*inz];
			if (evt == NULL) {
				// ������������ ������ �� ������ ������������.
				printf("Problem : not enough memory on your equipment for evt constr struct...\n");
				printf("Please any key to exit...\n");
				exit(1);
			}

			TOCHKA p;
			// ��������� � evt ������� � �������.
			// ���� �� ����������� ��������� ������� �� ����� 0.
			integer l = 1, ib;
			integer i, j, k;
			bool inDomain = false;
			// ��������� ����������� ������� ���������� � �������
			// ���� ����������� ����� �� ����������� ��������� ������� �� �������� 0.
			for (i = 0; i < inx; i++) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				p.x = 0.5*(xpos[i] + xpos[i + 1]);
				p.y = 0.5*(ypos[j] + ypos[j + 1]);
				p.z = 0.5*(zpos[k] + zpos[k + 1]);
				switch (iflag) {
				case TEMPERATURE: inDomain = in_model_temp(p, ib, b, lb); break;
				case HYDRODINAMIC: inDomain = in_model_flow(p, ib, b, lb); break;
				}
				if (inDomain) {
					// ����������� ��������� �������
					evt[i + j*inx + k*inx*iny] = l;
					tck_int_list[l - 1].i = i;
					tck_int_list[l - 1].j = j;
					tck_int_list[l - 1].k = k;
					l++;
				}
				else
				{   // �� ����������� ��������� �������
					evt[i + j*inx + k*inx*iny] = 0;
				}
			}
		}
	}

} // init_evt_f_alice.



// ������� ������� ��� ������� ����������� ������������
// ������ ��� 0 ���� ������ ���.
void constr_sosed(integer* evt, integer* ent, integer** &sosed, integer maxelm,
				  integer inx, integer iny, integer inz, TOCKA_INT* &tck_int_list) {
    integer i,j,k;
	sosed=NULL;
	sosed = new integer*[12];
	if (sosed==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for sosed constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	for (i=0; i<12; i++) {
		sosed[i]=NULL;
	}
	for (i=0; i<12; i++) {
		sosed[i]=new integer[maxelm];
		if (sosed[i]==NULL) {
	       // ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
			printf("Problem : not enough memory on your equipment for sosed[%lld] constr struct...\n", i);
#else
			printf("Problem : not enough memory on your equipment for sosed[%d] constr struct...\n", i);
#endif
		   
		   printf("Please any key to exit...\n");
		   //getchar();
		   system("pause");
		   exit(1);
	    }
	}

	integer ic,l=0;
	integer iE, iN, iT, iW, iS, iB;
    integer iEE, iNN, iTT, iWW, iSS, iBB;
	// ������ �� ���� ����������� ������� �������������
	// ��������� �������.
   // for (i=0; i<(inx); i++) for (j=0; j<(iny); j++) for (k=0; k<(inz); k++) {
	//	ic=i+j*inx+k*inx*iny;
		//if (evt[ic]>0) {

	for (integer iscan = 0; iscan<maxelm; iscan++) {
		{
			integer i = tck_int_list[iscan].i;
			integer j = tck_int_list[iscan].j;
			integer k = tck_int_list[iscan].k;
			ic = i + j * inx + k * inx*iny;
			


				if (l < maxelm) {

					iE = (i + 1) + j*inx + k*inx*iny + 1;
					if (((i + 1) < inx) && (iE > 0) && (iE <= inx*iny*inz) && (evt[iE - 1] > 0)) sosed[ESIDE][l] = evt[iE - 1];
					else {
						sosed[ESIDE][l] = 0; // ����� �� ����������� ��������� �������
					}
					iW = (i - 1) + j*inx + k*inx*iny + 1;
					if (((i - 1) >= 0) && (iW > 0) && (iW <= inx*iny*inz) && (evt[iW - 1] > 0)) sosed[WSIDE][l] = evt[iW - 1];
					else {
						sosed[WSIDE][l] = 0; // ����� �� ����������� ��������� �������
					}
					iN = i + (j + 1)*inx + k*inx*iny + 1;
					if (((j + 1) < iny) && (iN > 0) && (iN <= inx*iny*inz) && (evt[iN - 1] > 0)) sosed[NSIDE][l] = evt[iN - 1];
					else {
						sosed[NSIDE][l] = 0; // ����� �� ����������� ��������� �������
					}
					iS = i + (j - 1)*inx + k*inx*iny + 1;
					if (((j - 1) >= 0) && (iS > 0) && (iS <= inx*iny*inz) && (evt[iS - 1] > 0)) sosed[SSIDE][l] = evt[iS - 1];
					else {
						sosed[SSIDE][l] = 0; // ����� �� ����������� ��������� �������
					}
					iT = i + j*inx + (k + 1)*inx*iny + 1;
					if (((k + 1) < inz) && (iT > 0) && (iT <= inx*iny*inz) && (evt[iT - 1] > 0)) sosed[TSIDE][l] = evt[iT - 1];
					else {
						sosed[TSIDE][l] = 0; // ����� �� ����������� ��������� �������
					}
					iB = i + j*inx + (k - 1)*inx*iny + 1;
					if (((k - 1) >= 0) && (iB > 0) && (iB <= inx*iny*inz) && (evt[iB - 1] > 0)) sosed[BSIDE][l] = evt[iB - 1];
					else {
						sosed[BSIDE][l] = 0; // ����� �� ����������� ��������� �������
					}

					iEE = (i + 2) + j*inx + k*inx*iny + 1;
					if (((i + 2) < inx) && (iEE > 0) && (iEE <= inx*iny*inz) && (evt[iEE - 1] > 0)) sosed[EE][l] = evt[iEE - 1];
					else sosed[EE][l] = 0; // ����� �� ����������� ��������� �������
					iWW = (i - 2) + j*inx + k*inx*iny + 1;
					if (((i - 2) >= 0) && (iWW > 0) && (iWW <= inx*iny*inz) && (evt[iWW - 1] > 0)) sosed[WW][l] = evt[iWW - 1];
					else sosed[WW][l] = 0; // ����� �� ����������� ��������� �������
					iNN = i + (j + 2)*inx + k*inx*iny + 1;
					if (((j + 2) < iny) && (iNN > 0) && (iNN <= inx*iny*inz) && (evt[iNN - 1] > 0)) sosed[NN][l] = evt[iNN - 1];
					else sosed[NN][l] = 0; // ����� �� ����������� ��������� �������
					iSS = i + (j - 2)*inx + k*inx*iny + 1;
					if (((j - 2) >= 0) && (iSS > 0) && (iSS <= inx*iny*inz) && (evt[iSS - 1] > 0)) sosed[SS][l] = evt[iSS - 1];
					else sosed[SS][l] = 0; // ����� �� ����������� ��������� �������
					iTT = i + j*inx + (k + 2)*inx*iny + 1;
					if (((k + 2) < inz) && (iTT > 0) && (iTT <= inx*iny*inz) && (evt[iTT - 1] > 0)) sosed[TTSIDE][l] = evt[iTT - 1];
					else sosed[TTSIDE][l] = 0; // ����� �� ����������� ��������� �������
					iBB = i + j*inx + (k - 2)*inx*iny + 1;
					if (((k - 2) >= 0) && (iBB > 0) && (iBB <= inx*iny*inz) && (evt[iBB - 1] > 0)) sosed[BB][l] = evt[iBB - 1];
					else sosed[BB][l] = 0; // ����� �� ����������� ��������� �������

					l++;
				}
				else {
					printf("model error: l>maxelm\n");
					system("pause");
					exit(1);
				}
		}
	}

} // constr_sosed

// ������� ������� ��� ������� ����������� ������������
// ������ ��� 0 ���� ������ ���.
void constr_sosed_flow(integer **evt_f2, integer iDom, 
					   integer** &sosed, integer maxelm,
				       integer inx, integer iny, integer inz) {
    integer i,j,k;	
	sosed=NULL;
	sosed = new integer*[12];
	if (sosed==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for sosed constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	for (i=0; i<12; i++) sosed[i]=NULL;
	for (i=0; i<12; i++) {
		sosed[i]=new integer[maxelm];
		if (sosed[i]==NULL) {
	        // ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
			printf("Problem : not enough memory on your equipment for sosed[%lld] constr struct...\n", i);
#else
			printf("Problem : not enough memory on your equipment for sosed[%d] constr struct...\n", i);
#endif
		    
		    printf("Please any key to exit...\n");
			//getchar();
			system("pause");
		    exit(1);
	    }
	}

	integer ic,l=0;
	integer iE, iN, iT, iW, iS, iB;
    integer iEE, iNN, iTT, iWW, iSS, iBB;
	// ������ �� ���� ����������� ������� �������������
	// ��������� �������.
    for (i=0; i<(inx); i++) for (j=0; j<(iny); j++) for (k=0; k<(inz); k++) {
		ic=i+j*inx+k*inx*iny;
			if ((evt_f2[ENUMERATECONTVOL][ic]>0) && (evt_f2[MASKDOMAINFLUID][ic] == (iDom + 1))) {


				
					if (l < maxelm) {
							iE = (i + 1) + j*inx + k*inx*iny + 1;
							if (((i + 1) < inx) && (iE > 0) && (iE <= inx*iny*inz) && (evt_f2[0][iE - 1] > 0) && (evt_f2[1][iE - 1] == (iDom + 1))) sosed[ESIDE][l] = evt_f2[0][iE - 1];
							else sosed[ESIDE][l] = 0; // ����� �� ����������� ��������� �������
							iW = (i - 1) + j*inx + k*inx*iny + 1;
							if (((i - 1) >= 0) && (iW > 0) && (iW <= inx*iny*inz) && (evt_f2[0][iW - 1] > 0) && (evt_f2[1][iW - 1] == (iDom + 1))) sosed[WSIDE][l] = evt_f2[0][iW - 1];
							else sosed[WSIDE][l] = 0; // ����� �� ����������� ��������� �������
							iN = i + (j + 1)*inx + k*inx*iny + 1;
							if (((j + 1) < iny) && (iN > 0) && (iN <= inx*iny*inz) && (evt_f2[0][iN - 1] > 0) && (evt_f2[1][iN - 1] == (iDom + 1))) sosed[NSIDE][l] = evt_f2[0][iN - 1];
							else sosed[NSIDE][l] = 0; // ����� �� ����������� ��������� �������
							iS = i + (j - 1)*inx + k*inx*iny + 1;
							if (((j - 1) >= 0) && (iS > 0) && (iS <= inx*iny*inz) && (evt_f2[0][iS - 1] > 0) && (evt_f2[1][iS - 1] == (iDom + 1))) sosed[SSIDE][l] = evt_f2[0][iS - 1];
							else sosed[SSIDE][l] = 0; // ����� �� ����������� ��������� �������
							iT = i + j*inx + (k + 1)*inx*iny + 1;
							if (((k + 1) < inz) && (iT > 0) && (iT <= inx*iny*inz) && (evt_f2[0][iT - 1] > 0) && (evt_f2[1][iT - 1] == (iDom + 1))) sosed[TSIDE][l] = evt_f2[0][iT - 1];
							else sosed[TSIDE][l] = 0; // ����� �� ����������� ��������� �������
							iB = i + j*inx + (k - 1)*inx*iny + 1;
							if (((k - 1) >= 0) && (iB > 0) && (iB <= inx*iny*inz) && (evt_f2[0][iB - 1] > 0) && (evt_f2[1][iB - 1] == (iDom + 1))) sosed[BSIDE][l] = evt_f2[0][iB - 1];
							else sosed[BSIDE][l] = 0; // ����� �� ����������� ��������� �������

							iEE = (i + 2) + j*inx + k*inx*iny + 1;
							if (((i + 2) < inx) && (iEE > 0) && (iEE <= inx*iny*inz) && (evt_f2[0][iEE - 1] > 0) && (evt_f2[1][iEE - 1] == (iDom + 1))) sosed[EE][l] = evt_f2[0][iEE - 1];
							else sosed[EE][l] = 0; // ����� �� ����������� ��������� �������
							iWW = (i - 2) + j*inx + k*inx*iny + 1;
							if (((i - 2) >= 0) && (iWW > 0) && (iWW <= inx*iny*inz) && (evt_f2[0][iWW - 1] > 0) && (evt_f2[1][iWW - 1] == (iDom + 1))) sosed[WW][l] = evt_f2[0][iWW - 1];
							else sosed[WW][l] = 0; // ����� �� ����������� ��������� �������
							iNN = i + (j + 2)*inx + k*inx*iny + 1;
							if (((j + 2) < iny) && (iNN > 0) && (iNN <= inx*iny*inz) && (evt_f2[0][iNN - 1] > 0) && (evt_f2[1][iNN - 1] == (iDom + 1))) sosed[NN][l] = evt_f2[0][iNN - 1];
							else sosed[NN][l] = 0; // ����� �� ����������� ��������� �������
							iSS = i + (j - 2)*inx + k*inx*iny + 1;
							if (((j - 2) >= 0) && (iSS > 0) && (iSS <= inx*iny*inz) && (evt_f2[0][iSS - 1] > 0) && (evt_f2[1][iSS - 1] == (iDom + 1))) sosed[SS][l] = evt_f2[0][iSS - 1];
							else sosed[SS][l] = 0; // ����� �� ����������� ��������� �������
							iTT = i + j*inx + (k + 2)*inx*iny + 1;
							if (((k + 2) < inz) && (iTT > 0) && (iTT <= inx*iny*inz) && (evt_f2[0][iTT - 1] > 0) && (evt_f2[1][iTT - 1] == (iDom + 1))) sosed[TTSIDE][l] = evt_f2[0][iTT - 1];
							else sosed[TTSIDE][l] = 0; // ����� �� ����������� ��������� �������
							iBB = i + j*inx + (k - 2)*inx*iny + 1;
							if (((k - 2) >= 0) && (iBB > 0) && (iBB <= inx*iny*inz) && (evt_f2[0][iBB - 1] > 0) && (evt_f2[1][iBB - 1] == (iDom + 1))) sosed[BB][l] = evt_f2[0][iBB - 1];
							else sosed[BB][l] = 0; // ����� �� ����������� ��������� �������

							l++;
						}
						else {
							printf("model is incorrect : constr struct sosed index >= maxelm\n");
							system("pause");
							exit(1);
						}
		}
	}

} // constr_sosed_flow

// ���� ������� ������������� ������������� ������������ ���� 
void my_fill_Domain(integer* &evt_f, integer i, integer j, integer k, 
					integer inx, integer iny, integer inz) {
        integer iP, iE, iW, iN, iS, iT, iB;
		integer isize = inx*iny*inz;
		iP=i+j*inx+k*inx*iny;
        //iE=(i+1)+j*inx+k*inx*iny+1;
		//iW=(i-1)+j*inx+k*inx*iny+1;
        //iN=i+(j+1)*inx+k*inx*iny+1;
		//iS=i+(j-1)*inx+k*inx*iny+1;
		//iT=i+j*inx+(k+1)*inx*iny+1;
		//iB=i+j*inx+(k-1)*inx*iny+1;

		if (((i)<inx) && (iP>=0) && (iP < isize) && evt_f[iP]>0) {
			
			iE = iP + 2;
			iW = iP;
			iN = iP + 1 + inx;
			iS = iP + 1 - inx;
			integer isize_loc = inx*iny;
			iT = iP + 1 + isize_loc;
			iB = iP + 1 - isize_loc;
            // ���� ����������� ���� FLUID.
			// ���� ������������ ������� ��������� ���� �����. 
            if (((i+1)<inx) && (iE>0) && (iE<= isize) && (evt_f[iE-1]>0) && (evt_f[iE - 1]!= evt_f[iP])) evt_f[iE-1]=evt_f[iP];
            if (((i-1)>=0) && (iW>0) && (iW<= isize) && (evt_f[iW-1]>0) && (evt_f[iW - 1] != evt_f[iP])) evt_f[iW-1]=evt_f[iP];
            if (((j+1)<iny) && (iN>0) && (iN<= isize) && (evt_f[iN-1]>0) && (evt_f[iN - 1] != evt_f[iP])) evt_f[iN-1]=evt_f[iP];
            if (((j-1)>=0) && (iS>0) && (iS<= isize) && (evt_f[iS-1]>0) && (evt_f[iS - 1] != evt_f[iP])) evt_f[iS-1]=evt_f[iP];
            if (((k+1)<inz) && (iT>0) && (iT<= isize) && (evt_f[iT-1]>0) && (evt_f[iT - 1] != evt_f[iP])) evt_f[iT-1]=evt_f[iP];
            if (((k-1)>=0) && (iB>0) && (iB<= isize) && (evt_f[iB-1]>0) && (evt_f[iB - 1] != evt_f[iP])) evt_f[iB-1]=evt_f[iP];
		}
} // my_fill_Domain


// ���� ������� ������������� ������������� ������������ ���� 
void my_fill_Domain_recursive(integer* &evt_f, integer i, integer j, integer k, 
					integer inx, integer iny, integer inz) {
        
		integer iP, iE, iW, iN, iS, iT, iB;


		/* ����������� ��������� ������� ��������� ������� ������������ ���������,
		� ����� �� ���� ������������ ����� ����������� ������� �������� ��������� ��������
		�� �������� � ����� � ������� �����.
		*/

		// ��� ����� ����.
		typedef struct TSTEK {
	        integer i;
	        integer j;
	        integer k;
        } STEK;
		
		STEK *stek=NULL;
		stek=new STEK[inx*iny*inz];
		if (stek==NULL) {
			printf("no allocate memory for my_fill_Domain_recursive : stek==NULL\n");
			//getchar();
			system("pause");
			exit(1);
		}
		integer itop=0; // ������ ������� �����.

		stek[itop].i=i;
		stek[itop].j=j;
		stek[itop].k=k;
		itop++;

		// ������� ���������� ����������� ������� �� �������������� ������������,
		// ���� �������� �������� �� ��������.
		

		while (itop>0) {
			if (itop>0) {
				// �������� ������� �����.
               i=stek[itop-1].i;
		       j=stek[itop-1].j;
		       k=stek[itop-1].k;
			   itop--;

	           iP=i+j*inx+k*inx*iny;
               iE=(i+1)+j*inx+k*inx*iny+1;
		       iW=(i-1)+j*inx+k*inx*iny+1;
               iN=i+(j+1)*inx+k*inx*iny+1;
		       iS=i+(j-1)*inx+k*inx*iny+1;
		       iT=i+j*inx+(k+1)*inx*iny+1;
		       iB=i+j*inx+(k-1)*inx*iny+1;



		if (evt_f[iP]>0) {

			 // ���� ����������� ���� FLUID.
			// ���� ������������ ������� ��������� ���� �����. 
           

            // ���� ����������� ���� FLUID.
			// ���� ������������ ������� ��������� ���� �����. 
            if (((i+1)<inx) && (iE>0) && (iE<=inx*iny*inz) && (evt_f[iE-1]>0)&&(evt_f[iE-1]!=evt_f[iP])) {
				evt_f[iE-1]=evt_f[iP];
				//my_fill_Domain_recursive(evt_f, i+1, j, k, inx, iny, inz);
				// PUSH �������� � ����.
	            stek[itop].i=i+1;
		        stek[itop].j=j;
		        stek[itop].k=k;
		        itop++;
			}
            if (((i-1)>=0) && (iW>0) && (iW<=inx*iny*inz) && (evt_f[iW-1]>0)&&(evt_f[iW-1]!=evt_f[iP])) {
				evt_f[iW-1]=evt_f[iP];
                //my_fill_Domain_recursive(evt_f, i-1, j, k, inx, iny, inz);
				// PUSH �������� � ����.
                stek[itop].i=i-1;
		        stek[itop].j=j;
		        stek[itop].k=k;
		        itop++;			
			}
            if (((j+1)<iny) && (iN>0) && (iN<=inx*iny*inz) && (evt_f[iN-1]>0)&&(evt_f[iN-1]!=evt_f[iP])) {
				evt_f[iN-1]=evt_f[iP];
				//my_fill_Domain_recursive(evt_f, i, j+1, k, inx, iny, inz);
				// PUSH �������� � ����.
			    stek[itop].i=i;
	        	stek[itop].j=j+1;
		        stek[itop].k=k;
		        itop++;
			}
            if (((j-1)>=0) && (iS>0) && (iS<=inx*iny*inz) && (evt_f[iS-1]>0)&&(evt_f[iS-1]!=evt_f[iP])) {
				evt_f[iS-1]=evt_f[iP];
				//my_fill_Domain_recursive(evt_f, i, j-1, k, inx, iny, inz);
				// PUSH �������� � ����.
                stek[itop].i=i;
		        stek[itop].j=j-1;
		        stek[itop].k=k;
		        itop++;			
			}
            if (((k+1)<inz) && (iT>0) && (iT<=inx*iny*inz) && (evt_f[iT-1]>0)&&(evt_f[iT-1]!=evt_f[iP])) {
				evt_f[iT-1]=evt_f[iP];
                //my_fill_Domain_recursive(evt_f, i, j, k+1, inx, iny, inz);
				// PUSH �������� � ����.
			    stek[itop].i=i;
		        stek[itop].j=j;
		        stek[itop].k=k+1;
		        itop++;
			}
            if (((k-1)>=0) && (iB>0) && (iB<=inx*iny*inz) && (evt_f[iB-1]>0)&&(evt_f[iB-1]!=evt_f[iP])) {
				evt_f[iB-1]=evt_f[iP];
                //my_fill_Domain_recursive(evt_f, i, j, k-1, inx, iny, inz);
				// PUSH �������� � ����.
				stek[itop].i=i;
		        stek[itop].j=j;
	         	stek[itop].k=k-1;
		        itop++;
			}
		}
			}

		}

		if (stek != NULL) {
			delete[] stek;
		}
} // my_fill_Domain_recursive


// �������� ������ ������������� � �����������������.
// ������ ����� �������� 22 �������� 2016 � ����� � ���������� ���� �����.
// ��� ����� ������������ � �������� � ��� ���� ����� ����.
void constr_ptr_temp_part1(integer &flow_interior, 
	integer * &evt_f, integer** &evt_f2, integer* &domain_id, 
	integer inx, integer iny, integer inz) {


	int i_my_num_core_parallelesation = omp_get_num_threads();
	omp_set_num_threads(8); // ���������� 8 �������, 10 ������� ��� �������� �� �������.

	integer i = 0, j = 0, k = 0;

	// 9 ��� 2013 ������ �������� ����� �� ��������� �� ����� ������� ��������� � ����� 
	// ������ ����������� �������� �������.
	// �������� (��� ����������������� �����) ���������� �������� � STECK OVERFLOW,
	// �.�. ������� ����������� ������� ���������� � � ��������� �� ��������������� �������.



	// 26 ���� 2015 ����������� �������� ������� ����������� ������� 
	// ����� ���������, ���� �� �� �������� ����������� �� ������.
	// �� ����������� �������� ��������� �� ������, �� �� ����������� � ����� �������� 
	// �������������� ��������� �������. 
	// 22 �������� 2016. ������ ����������� �.�. ��� ������ ���� � 1000 �������� �������
	// ����� ������ ������� ��� ����� ��������� � ��������� ����������� ���� ������� ���
	// ������� ���������. ����������� ����� ������ ��������� ����� ���������� �������.

	bool brecursiv_algorithm = false;

	if (brecursiv_algorithm) {
		// ����������� ��������.
		bool bfirst_visit = true;

		for (i = 0; i<inx; i++) for (j = 0; j<iny; j++) for (k = 0; k<inz; k++) {
			if (bfirst_visit) {
				integer iP, iE, iW, iN, iS, iT, iB;
				iP = i + j*inx + k*inx*iny;
				iE = (i + 1) + j*inx + k*inx*iny + 1;
				iW = (i - 1) + j*inx + k*inx*iny + 1;
				iN = i + (j + 1)*inx + k*inx*iny + 1;
				iS = i + (j - 1)*inx + k*inx*iny + 1;
				iT = i + j*inx + (k + 1)*inx*iny + 1;
				iB = i + j*inx + (k - 1)*inx*iny + 1;
				if ((iP >= 0) && (iP<inx*iny*inz) && (evt_f[iP]>0)) {
					if (((i + 1)<inx) && (iE>0) && (iE <= inx*iny*inz) && (evt_f[iE - 1]>0)) {
						if (((i - 1) >= 0) && (iW>0) && (iW <= inx*iny*inz) && (evt_f[iW - 1]>0)) {
							if (((j + 1)<iny) && (iN>0) && (iN <= inx*iny*inz) && (evt_f[iN - 1]>0)) {
								if (((j - 1) >= 0) && (iS>0) && (iS <= inx*iny*inz) && (evt_f[iS - 1]>0)) {
									if (((k + 1)<inz) && (iT>0) && (iT <= inx*iny*inz) && (evt_f[iT - 1]>0)) {
										if (((k - 1) >= 0) && (iB>0) && (iB <= inx*iny*inz) && (evt_f[iB - 1]>0)) {
#if doubleintprecision == 1
											// printf("in comming %lld\n",evt_f[iP]); // debug.
#else
											// printf("in comming %d\n",evt_f[iP]); // debug.
#endif
											
											//getchar();
											bfirst_visit = false;// ��������� ����� �� ����� for.
											my_fill_Domain_recursive(evt_f, i, j, k, inx, iny, inz);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else {

		bool *evt_etalon = NULL;
		evt_etalon = new bool[inx*iny*inz+2];
		if (evt_etalon == NULL) {
			// ������������ ������ �� ������ ������������.
			printf("Problem : not enough memory on your equipment for evt_etalon constr struct...\n");
			printf("Please any key to exit...\n");
			exit(1);
		}

		bool *evt_etalon1 = NULL;
		evt_etalon1 = new bool[inx*iny*inz+2];
		if (evt_etalon1 == NULL) {
			// ������������ ������ �� ������ ������������.
			printf("Problem : not enough memory on your equipment for evt_etalon1 constr struct...\n");
			printf("Please any key to exit...\n");
			exit(1);
		}

#pragma omp parallel for
		for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
			evt_etalon[i_1] = false; // initialisation.
			evt_etalon1[i_1] = false; // initialisation.
		}

		evt_etalon[0] = true;
#pragma omp parallel for
		for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
			if (evt_f[i_1] > 0) {
				evt_etalon[evt_f[i_1]] = true;
			}
		}
		

		printf("\n                                          ");
		printf("progress bar :\n	");
		printf("\\");
		omp_set_num_threads(1); // ���� ������������ ��������� 29_07_2017
		// �������� - ���������� ����� ������ ����� � ��� �� ����� ������ ����� ������ ��������� �������.
		// 5 ������� 2016. (����������� �������� �������� ��-�� ������������ ����� �������).
		// � ������������ ����� ���� ��������� ��������.
		for (integer ilk = 0; ilk < 1000; ilk++) {
			bool bisequal = true;

			//if (ilk % 100 == 0) printf("progress bar %e procent\n", ilk / 10.0);
			printf("\b");
			printf("|");
			// ������ ������: �������.
			// 6*4 == 24 ������
			// ������ ������������ ����������� ������ ��������� �������,
			// ����� ��������� �� �������� "������������ �� ����".
			// ����� ����� ����������� �.�. 1000 �������� ��� ����� ����� ��� tgf20.

#pragma omp parallel for
			for (i = 0; i < inx; i+=2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i+=2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b");
			printf("/");
			if (inx*iny*inz > 9000000) {
				// ����� ������� ������, ������������� ���� ��������� ��������� ����������.
				// ��� ���������� �����������, ���� �� ������.
				// ����� ������� ������������ ����� ������ ������� ����� �������.
				// ���������� � ��� ��� �� ������� 2*inx*iny*inz ����� ����� ������.
				evt_etalon1[0] = true;
#pragma omp parallel for
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					if (evt_f[i_1] > 0) {
						evt_etalon1[evt_f[i_1]] = true;
					}
				}

				bisequal = true;
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					if (evt_etalon[i_1] != evt_etalon1[i_1]) {
						bisequal = false;
						break;
					}
				}
				if (bisequal) {
					break;
				}
#pragma omp parallel for
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					evt_etalon[i_1] = evt_etalon1[i_1];
					evt_etalon1[i_1] = false;// initialization.
				}
			}

#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b");
			printf("--");
			if (inx*iny*inz > 9000000) {
				// ����� ������� ������, ������������� ���� ��������� ��������� ����������.
				// ��� ���������� �����������, ���� �� ������.
				// ����� ������� ������������ ����� ������ ������� ����� �������.
				// ���������� � ��� ��� �� ������� 2*inx*iny*inz ����� ����� ������.
				evt_etalon1[0] = true;
#pragma omp parallel for
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					if (evt_f[i_1] > 0) {
						evt_etalon1[evt_f[i_1]] = true;
					}
				}

				bisequal = true;
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					if (evt_etalon[i_1] != evt_etalon1[i_1]) {
						bisequal = false;
						break;
					}
				}
				if (bisequal) {
					break;
				}
#pragma omp parallel for
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					evt_etalon[i_1] = evt_etalon1[i_1];
					evt_etalon1[i_1] = false;// initialization.
				}
			}
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b\b");
			printf("\\");
			if (inx*iny*inz > 9000000) {
				// ����� ������� ������, ������������� ���� ��������� ��������� ����������.
				// ��� ���������� �����������, ���� �� ������.
				// ����� ������� ������������ ����� ������ ������� ����� �������.
				// ���������� � ��� ��� �� ������� 2*inx*iny*inz ����� ����� ������.
				evt_etalon1[0] = true;
#pragma omp parallel for
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					if (evt_f[i_1] > 0) {
						evt_etalon1[evt_f[i_1]] = true;
					}
				}

				bisequal = true;
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					if (evt_etalon[i_1] != evt_etalon1[i_1]) {
						bisequal = false;
						break;
					}
				}
				if (bisequal) {
					break;
				}
#pragma omp parallel for
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					evt_etalon[i_1] = evt_etalon1[i_1];
					evt_etalon1[i_1] = false;// initialization.
				}
			}
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b");
			printf("|");
			if (inx*iny*inz > 9000000) {
				// ����� ������� ������, ������������� ���� ��������� ��������� ����������.
				// ��� ���������� �����������, ���� �� ������.
				// ����� ������� ������������ ����� ������ ������� ����� �������.
				// ���������� � ��� ��� �� ������� 2*inx*iny*inz ����� ����� ������.
				evt_etalon1[0] = true;
#pragma omp parallel for
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					if (evt_f[i_1] > 0) {
						evt_etalon1[evt_f[i_1]] = true;
					}
				}

				bisequal = true;
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					if (evt_etalon[i_1] != evt_etalon1[i_1]) {
						bisequal = false;
						break;
					}
				}
				if (bisequal) {
					break;
				}
#pragma omp parallel for
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					evt_etalon[i_1] = evt_etalon1[i_1];
					evt_etalon1[i_1] = false;// initialization.
				}
			}
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b");
			printf("/");
			if (inx*iny*inz > 9000000) {
				// ����� ������� ������, ������������� ���� ��������� ��������� ����������.
				// ��� ���������� �����������, ���� �� ������.
				// ����� ������� ������������ ����� ������ ������� ����� �������.
				// ���������� � ��� ��� �� ������� 2*inx*iny*inz ����� ����� ������.
				evt_etalon1[0] = true;
#pragma omp parallel for
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					if (evt_f[i_1] > 0) {
						evt_etalon1[evt_f[i_1]] = true;
					}
				}

				bisequal = true;
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					if (evt_etalon[i_1] != evt_etalon1[i_1]) {
						bisequal = false;
						break;
					}
				}
				if (bisequal) {
					break;
				}
#pragma omp parallel for
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					evt_etalon[i_1] = evt_etalon1[i_1];
					evt_etalon1[i_1] = false;// initialization.
				}
			}

#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b");
			printf("--");
			if (inx*iny*inz > 9000000) {
				// ����� ������� ������, ������������� ���� ��������� ��������� ����������.
				// ��� ���������� �����������, ���� �� ������.
				// ����� ������� ������������ ����� ������ ������� ����� �������.
				// ���������� � ��� ��� �� ������� 2*inx*iny*inz ����� ����� ������.
				evt_etalon1[0] = true;
#pragma omp parallel for
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					if (evt_f[i_1] > 0) {
						evt_etalon1[evt_f[i_1]] = true;
					}
				}

				bisequal = true;
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					if (evt_etalon[i_1] != evt_etalon1[i_1]) {
						bisequal = false;
						break;
					}
				}
				if (bisequal) {
					break;
				}
#pragma omp parallel for
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					evt_etalon[i_1] = evt_etalon1[i_1];
					evt_etalon1[i_1] = false;// initialization.
				}
			}
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b\b");
			printf("\\");
			if (inx*iny*inz > 9000000) {
				// ����� ������� ������, ������������� ���� ��������� ��������� ����������.
				// ��� ���������� �����������, ���� �� ������.
				// ����� ������� ������������ ����� ������ ������� ����� �������.
				// ���������� � ��� ��� �� ������� 2*inx*iny*inz ����� ����� ������.
				evt_etalon1[0] = true;
#pragma omp parallel for
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					if (evt_f[i_1] > 0) {
						evt_etalon1[evt_f[i_1]] = true;
					}
				}

				bisequal = true;
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					if (evt_etalon[i_1] != evt_etalon1[i_1]) {
						bisequal = false;
						break;
					}
				}
				if (bisequal) {
					break;
				}
#pragma omp parallel for
				for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
					evt_etalon[i_1] = evt_etalon1[i_1];
					evt_etalon1[i_1] = false;// initialization.
				}
			}
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b");
			printf("|");
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b");
			printf("/");
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b");
			printf("--");
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b\b");
			printf("\\");
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b");
			printf("|");
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b");
			printf("/");
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b");
			printf("--");
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b\b");
			printf("\\");
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b");
			printf("|");
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b");
			printf("/");
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b");
			printf("--");
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b\b");
			printf("\\");
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b");
			printf("|");
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b");
			printf("/");
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b");
			printf("--");
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			printf("\b\b");
			printf("\\");
#pragma omp parallel for
			for (i = 0; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
#pragma omp parallel for
			for (i = 1; i < inx; i += 2) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				my_fill_Domain(evt_f, i, j, k, inx, iny, inz);
			}
			//printf("\b");
			//printf("|");

			// ��� ���������� �����������, ���� �� ������.
			// ����� ������� ������������ ����� ������ ������� ����� �������.
			// ���������� � ��� ��� �� ������� 2*inx*iny*inz ����� ����� ������.
			for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
				evt_etalon1[evt_f[i_1]]=true;
			}

			bisequal = true;
			for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
				if (evt_etalon[i_1] != evt_etalon1[i_1]) {
					bisequal = false;
				}
			}
			if (bisequal) {
				break;
			}
#pragma omp parallel for
			for (integer i_1 = 0; i_1 < inx*iny*inz; i_1++) {
				evt_etalon[i_1] = evt_etalon1[i_1];
				evt_etalon1[i_1] = false;// initialization.
			}

			//printf("ilk==%d\n",ilk);
		}
		omp_set_num_threads(8);// i_my_num_core_parallelesation
		printf("\n");
		//system("PAUSE");

		if (evt_etalon != NULL) {
			delete[] evt_etalon;
		}
		evt_etalon = NULL;

		if (evt_etalon1 != NULL) {
			delete[] evt_etalon1;
		}
		evt_etalon1 = NULL;
	}

	printf("part 11\n");

	integer iP;


	/*
	// debug GOOD
	// �������� ������������ ������������� �� �����.
	for (k=0; k<inz; k++) {
	    for (i=0; i<inx; i++) {
	        for (j=0; j<iny; j++) {
	             iP=i+j*inx+k*inx*iny;
				 #if doubleintprecision == 1
						printf("%lld ",evt_f[iP]);
				 #else
						printf("%d ",evt_f[iP]);
				 #endif
	             
	        }
	        printf("\n");
	    }
	    getchar();
	    printf("\n\n");
	}
	*/
#if doubleintprecision == 1
	//printf("evt_f[86]=%lld, evt_f[124]=%lld\n",evt_f[86],evt_f[124]); // debug
#else
	//printf("evt_f[86]=%d, evt_f[124]=%d\n",evt_f[86],evt_f[124]); // debug
#endif
	
	//getchar();

	// ��� ���������� ����� ���������� � ��������� �����������
	// ��� ��������� mass_balance.
	// �.�. ��������� ������������ ���� ���� ������ ��� ������ 
	// ��������� ��������� �����������������
	// ���������� ����� �� ����� ����������.
	// ����� ���������� ���� �������� ���������� �� ����� �������� � evt_f2[2][iP] �������. 
	integer* evt_f_shadow = NULL;
	evt_f_shadow = new integer[inx*iny*inz];
	if (evt_f_shadow == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for evt_f_shadow constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	// �����������.
	for (i = 0; i < inx; i++) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
		iP = i + j*inx + k*inx*iny;
		evt_f_shadow[iP] = evt_f[iP];
	}

	// ������ ������:
	// ���� ��������� ������, �� �������� max_domain ����� ���������.
	integer max_domain = 256; // ������������ ���������� ��� FLUID
	//integer *domain_id = NULL;
	domain_id = NULL;
	domain_id = new integer[max_domain];
	if (domain_id == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for domain_id constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	for (i = 0; i<max_domain; i++) domain_id[i] = 0; // �������������

	integer *domain_id_shadow = NULL;
	domain_id_shadow = new integer[max_domain];
	if (domain_id_shadow == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for domain_id_shadow constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	for (i = 0; i<max_domain; i++) domain_id_shadow[i] = 0; // �������������


	integer id;
	bool bfind;
	bool bfind_shadow;
	integer l;
	integer ic = 0; // ������� �� domain_id
	integer ic_shadow = 0; // ������� �� domain_id_shadow
	flow_interior = 0;
	for (i = 0; i<inx; i++) for (j = 0; j<iny; j++) for (k = 0; k<inz; k++) {
		iP = i + j*inx + k*inx*iny;
		if (evt_f[iP]>0) {
			id = evt_f[iP]; // ������������� ��������� FLUID ����.
			bfind = false;
			// ��������!!! �������� ��� ��������� ������� ����!.
			for (l = 0; l<max_domain; l++) if (domain_id[l] == id) bfind = true;
			if (!bfind) {
				if (ic >= max_domain - 1) {
					printf("error! nado uvelichit max_domain count...\n");
					//getchar();
					system("PAUSE");
					exit(1);
				}
				// ���������� ������������� ��������� FLUID ����.
				domain_id[ic++] = id;
				// ���������� ������������� FLUID ���.
				flow_interior = ic;
			}
			bfind_shadow = false;
			// ��������!!! �������� ��� ��������� ������� ����!.
			for (l = 0; l<max_domain; l++) if (domain_id_shadow[l] == id) bfind_shadow = true;
			if (!bfind_shadow) {
				if (ic_shadow >= max_domain - 1) {
					printf("error! nado uvelichit max_domain count...\n");
					//getchar();
					system("PAUSE");
					exit(1);
				}
				domain_id_shadow[ic_shadow++] = id;
				//flow_interior = ic;
			}
		}
	}
	printf("part 12\n");

	if (1) {
		// � ���������� domain_id �������� ������������ FLUID ���� � � ������������� 2.


		printf("5_08_2016: several fluid domain combine into one fluid domain.\n");
		// ����� � ��� ��� ���� ���� � ��� ��������� ���������������� ����������� �� �� ������ ���� ������������.
		ic = 0;
		/*
		// ������ ���, ������ ������. ���������� 22 �������� 2016.
		for (i = 0; i < inx; i++) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
		    iP = i + j*inx + k*inx*iny;
		    if (evt_f[iP] > 0) {
		       evt_f[iP] = 2;
		       id = evt_f[iP];
		       bfind = false;
		       // ��������!!! �������� ��� ��������� ������� ����!.
		       for (l = 0; l < max_domain; l++) if (domain_id[l] == id) bfind = true;
		       if (!bfind) {
		          if (ic >= max_domain - 1) {
		             printf("error! nado uvelichit max_domain count...\n");
		             getchar();
		             exit(1);
		          }
		          domain_id[ic++] = id;
		          flow_interior = ic;
		       }
		    }
		}
		*/
		// � ���������� domain_id �������� ������������ FLUID ���� � � ������������� 2.
		for (i = 0; i < inx; i++) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
			iP = i + j*inx + k*inx*iny;
			if (evt_f[iP] > 0) {
				evt_f[iP] = 2;
			}
		}


		for (l = 0; l < max_domain; l++) domain_id[l] = 0;
		ic = 0;
		domain_id[ic++] = 2;
		flow_interior = ic;

		printf("part 12.5 05_08_2016; 22_09_2016.\n");
	}



	if (domain_id != NULL) {
		delete[] domain_id;
		domain_id = NULL;
	}

	if (domain_id_shadow != NULL) {
		delete[] domain_id_shadow;
		domain_id_shadow = NULL;
	}

	// ������������ ������.
	if (evt_f_shadow != NULL) {
		delete[] evt_f_shadow;
	}
	evt_f_shadow = NULL;

	omp_set_num_threads(i_my_num_core_parallelesation);

} // constr_ptr_temp_part1

//  ����������� ���������� evt_f2 ��� ����� ������ �.�. ��� �� �������� 
// �� ������ �� ���������������  �������.
// ����� ���������� ��� ����������� ������ evt_f2.
void constr_ptr_temp_part2(integer &flow_interior,
	integer * &evt_f, integer** &evt_f2, integer* &domain_id,
	integer inx, integer iny, integer inz, TOCKA_INT* &tck_int_list, integer &maxelm) {

	integer i = 0, j = 0, k = 0;

	// ��� ���������� ����� ���������� � ��������� �����������
	// ��� ��������� mass_balance.
	// �.�. ��������� ������������ ���� ���� ������ ��� ������ 
	// ��������� ��������� �����������������
	// ���������� ����� �� ����� ����������.
	// ����� ���������� ���� �������� ���������� �� ����� �������� � evt_f2[2][iP] �������. 
	integer* evt_f_shadow = NULL;
	evt_f_shadow = new integer[inx*iny*inz];
	if (evt_f_shadow == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for evt_f_shadow constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	// �����������.
	//for (i = 0; i < inx; i++) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
		//integer iP = i + j*inx + k*inx*iny;
		//evt_f_shadow[iP] = evt_f[iP];
	//}
	// ����� ������� �����������.
	const integer isize = inx * iny*inz;
	for (integer iP = 0; iP < isize; iP++) {
		evt_f_shadow[iP] = evt_f[iP];
	}

	// ������ ������:
	// ���� ��������� ������, �� �������� max_domain ����� ���������.
	integer max_domain = 256; // ������������ ���������� ��� FLUID
						  //integer *domain_id = NULL;
	domain_id = NULL;
	domain_id = new integer[max_domain];
	if (domain_id == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for domain_id constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	for (i = 0; i<max_domain; i++) domain_id[i] = 0; // �������������

	integer *domain_id_shadow = NULL;
	domain_id_shadow = new integer[max_domain];
	if (domain_id_shadow == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for domain_id_shadow constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	for (i = 0; i<max_domain; i++) domain_id_shadow[i] = 0; // �������������


	integer id;
	bool bfind;
	bool bfind_shadow;
	integer l;
	integer ic = 0; // ������� �� domain_id
	integer ic_shadow = 0; // ������� �� domain_id_shadow
	flow_interior = 0;
	//for (i = 0; i<inx; i++) for (j = 0; j<iny; j++) for (k = 0; k<inz; k++) {
		//integer iP = i + j*inx + k*inx*iny;
		//if (evt_f[iP]>0) {
	for (integer iscan = 0; iscan<maxelm; iscan++) 
		{
			integer i = tck_int_list[iscan].i;
			integer j = tck_int_list[iscan].j;
			integer k = tck_int_list[iscan].k;
			integer iP = i + j * inx + k * inx*iny;
			{

			id = evt_f[iP]; // ������������� ��������� FLUID ����.
			bfind = false;
			// ��������!!! �������� ��� ��������� ������� ����!.
			for (l = 0; l<max_domain; l++) if (domain_id[l] == id) bfind = true;
			if (!bfind) {
				if (ic >= max_domain - 1) {
					printf("error! nado uvelichit max_domain count...\n");
					//getchar();
					system("PAUSE");
					exit(1);
				}
				// ���������� ������������� ��������� FLUID ����.
				domain_id[ic++] = id;
				// ���������� ������������� FLUID ���.
				flow_interior = ic;
			}
			bfind_shadow = false;
			// ��������!!! �������� ��� ��������� ������� ����!.
			for (l = 0; l<max_domain; l++) if (domain_id_shadow[l] == id) bfind_shadow = true;
			if (!bfind_shadow) {
				if (ic_shadow >= max_domain - 1) {
					printf("error! nado uvelichit max_domain count...\n");
					//getchar();
					system("PAUSE");
					exit(1);
				}
				domain_id_shadow[ic_shadow++] = id;
				//flow_interior = ic;
			}
		}
	}
	printf("part 12.6\n");

#if doubleintprecision == 1
	// printf("flow interior=%lld\n",flow_interior); //debug.
#else
	// printf("flow interior=%d\n",flow_interior); //debug.
#endif
	
	// getchar();

	integer i1; // �������

			// ��������� ����������� ������
	evt_f2 = NULL;
	evt_f2 = new integer*[3];
	if (evt_f2 == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for evt_f2 constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	for (i = 0; i<3; i++) evt_f2[i] = NULL;
	for (i = 0; i<3; i++) {
		evt_f2[i] = new integer[inx*iny*inz];
		if (evt_f2[i] == NULL) {
			// ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
			printf("Problem : not enough memory on your equipment for evt_f2[%lld] constr struct...\n", i);
#else
			printf("Problem : not enough memory on your equipment for evt_f2[%d] constr struct...\n", i);
#endif
				printf("Please any key to exit...\n");
			// getchar();
			system("pause");
			exit(1);
		}
	}
	// ������ �� ���� ����������� �������
	integer ipos = -1;
	integer ipos_shadow = -1;

	for (integer iP = 0; iP < isize; iP++) {
		evt_f2[MASKDOMAINFLUID][iP] = 0; // �� ����������� ��������� �������.
	}
	//for (i = 0; i<inx; i++) for (j = 0; j<iny; j++) for (k = 0; k<inz; k++) {
		//integer iP = i + j*inx + k*inx*iny;

	for (integer iscan = 0; iscan<maxelm; iscan++) 
		{
			integer i = tck_int_list[iscan].i;
			integer j = tck_int_list[iscan].j;
			integer k = tck_int_list[iscan].k;
			integer iP = i + j * inx + k * inx*iny;
			
		//if (evt_f[iP] == 0) {
			//evt_f2[MASKDOMAINFLUID][iP] = 0; // �� ����������� ��������� �������.
		//}
		//else
			{
			// FLUID Domain
			// 22.09.2016 �� ����� ���� ������ ����� 2.
			integer id = evt_f[iP];
			// �������� !!! ��������� �������� ��������� ������� ����.
			for (i1 = 0; i1<max_domain; i1++) if (domain_id[i1] == id) {
				// 22.09.2016 �� ������������ ������� � ��� ����� ����������� ����� ipos==i1==0.
				ipos = i1;
				break;
			}
			// �� ������� ������ ��� ���������� ������������� FLUID
			// ����� ������ ������������� ���� �������.
			evt_f2[MASKDOMAINFLUID][iP] = ipos + 1; // ��������� FLUID Domain ���������� � 1.

													// FLUID Domain
			integer id_shadow = evt_f_shadow[iP];
			// �������� !!! ��������� �������� ��������� ������� ����.
			for (i1 = 0; i1<max_domain; i1++) if (domain_id_shadow[i1] == id_shadow) {
				// ������������� ����� ��������� FLUID ���� ������� � ����.
				ipos_shadow = i1;
				break;
			}
			// �� ������� ������ ��� ���������� ������������� FLUID
			evt_f2[MASKDOMAINFLUIDCOLOR][iP] = ipos_shadow + 1; // ��������� FLUID Domain ���������� � 1.
		}

	}

	printf("part 13\n");

	/*
	// debug GOOD
	// �������� ������������ ������������� �� �����.
	for (k=0; k<inz; k++) {
	for (i=0; i<inx; i++) {
	for (j=0; j<iny; j++) {
	iP=i+j*inx+k*inx*iny;
	#if doubleintprecision == 1
		printf("%lld ",evt_f2[MASKDOMAINFLUID][iP]);
	#else
		printf("%d ",evt_f2[MASKDOMAINFLUID][iP]);
	#endif
	
	}
	printf("\n");
	}
	getchar();
	printf("\n\n");
	}
	//*/

	if (domain_id_shadow != NULL) {
		delete[] domain_id_shadow;
		domain_id_shadow = NULL;
	}

	// ������������ ������.
	if (evt_f_shadow != NULL) {
		delete[] evt_f_shadow;
	}
	evt_f_shadow = NULL;

} // constr_ptr_temp_part2

// ��������� ������ ��� FLOW.
// ���� �������� 22 �������� 2016.
// ������ ������� ���� ���� �������� ��������� ����������� ������, � ��� �� �� �� ��������� ���
// ���� �����.
void constr_ptr_temp_allocation_memory_alice(integer &flow_interior, FLOW* &f) {
	f = new FLOW[flow_interior];
	// ������ �� ���� ������ �����.
	for (integer l = 0; l < flow_interior; l++) {
		// ����� ��������� ��.
		// �������� ������� �������� ����� ����� ������� � ����������.
		// ����� ������� ��� �� ������������ ���������� whot_is_block ��� ����������������� �����������.
		f[l].whot_is_block = NULL;
		// ���������� ��������� � ������ ����� ���������� � ������ ����� � ����������� �������.
		//f[l].maxelm = domain_counter[l];
#if doubleintprecision == 1
		//printf("FLUID%lld maxelm=%lld\n", l, f[l].maxelm);
#else
		//printf("FLUID%d maxelm=%d\n", l, f[l].maxelm);
#endif
		
		// ��������� ����������� ������.
		f[l].ptr = NULL;
		// f[l].ptr[fluid_elm_id]=temper_elm_id; // ����� ptr.
		// ��� ����������������� ����������� � ������ ����� !!!.
		/*
		f[l].ptr = new integer[f[l].maxelm];
		#if doubleintprecision == 1
			printf("delete flow %lld ptr\n", l);
		#else
			printf("delete flow %d ptr\n", l);
		#endif
		
		if (f[l].ptr == NULL) { // -2N
			// ������������ ������ �� ������ ������������.
			#if doubleintprecision == 1
				printf("Problem : not enough memory on your equipment for ptr flow%lld constr struct...\n", l);
			#else
				printf("Problem : not enough memory on your equipment for ptr flow%d constr struct...\n", l);
			#endif
			
			printf("Please any key to exit...\n");
			//getchar();
			system("pause");
			exit(1);
		}
		for (j = 0; j < maxelm_t; j++) {
			if (ptr[MASKDOMAINFLUID][j] == l) {
				f[l].ptr[ptr[ENUMERATECONTVOL][j]] = j;
			}
		}
		*/
	}
	printf("part 14\n");
	printf("part 15\n");
	printf("part 16\n");
} // constr_ptr_temp_allocation_memory_alice


// �������� ������ ������������� � �����������������.
void constr_ptr_temp(integer &flow_interior, FLOW* &f, integer maxelm_t,
					 integer** &ptr, integer *evt_t, integer * &evt_f, integer** &evt_f2,
					 integer* &domain_id, integer inx, integer iny, integer inz, bool breconstruct) {
	
    integer i=0,j=0,k=0;

	
	integer iP;

	// ���� ��������� ������, �� �������� max_domain ����� ���������.
	integer max_domain = 256; // ������������ ���������� ��� FLUID

	integer id;
	//bool bfind;
	//bool bfind_shadow;
	integer l;
	integer ic = 0; // ������� �� domain_id
	integer ic_shadow = 0; // ������� �� domain_id_shadow
	//flow_interior = 0; // ����������� � constr_ptr_temp_part1.
	
	

	 integer i1; // �������
	 	
	 integer ipos = -1;
	 integer ipos_shadow = -1;

	// 22 �������� 2016.
	// �� ����� 13 ����� ������������ � ��� ���� �����.

	// ��������� ������ :

	 //printf("delete temperature ptr\n");
	 if (ptr != NULL) {
		 for (i = 0; i<2; i++) {
			 if (ptr[i] != NULL) {
				 delete[] ptr[i];
				 ptr[i] = NULL;
			 }
		 }
		 delete[] ptr;
		 ptr = NULL;
	 }

	 i = 0;

	ptr=NULL;
    ptr = new integer*[2];
	if (ptr==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for ptr constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	for (i=0; i<2; i++) ptr[i]=NULL;

	for (i=0; i<2; i++) {
		ptr[i]=new integer[maxelm_t];
		if (ptr[i]==NULL) {
	        // ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
			printf("Problem : not enough memory on your equipment for ptr[%lld] constr struct...\n", i);
#else
			printf("Problem : not enough memory on your equipment for ptr[%d] constr struct...\n", i);
#endif
		    
		    printf("Please any key to exit...\n");
			//getchar();
			system("pause");
		    exit(1);
	    }   
	}

	for (i = 0; i < 2; i++) {
		for (integer i87 = 0; i87 < maxelm_t; i87++) {
			ptr[i][i87] = -1; // ������������� ������� �����.
		}
	}


    //#define ENUMERATECONTVOL 0 // ���� ��������� ��
    //#define MASKDOMAINFLUID 1 // ���� ����� ���������� ���� FLUID
    //#define MASKDOMAINFLUIDCOLOR 2 // � evt_f2 ���� ����������������� ����������.

	if ((ptr != NULL) && (ptr[0] != NULL) && (ptr[1] != NULL)) {

		// ������ ������. ���������� ��������� ��� FLUID:
		l = 0;
		ipos = -1;
		for (i = 0; i < inx; i++) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
			iP = i + j*inx + k*inx*iny;
			if (evt_t[iP] > 0) {
				// ���� �� ����������� ������ ����������������.
				if (evt_f[iP] > 0) {
					// FLUID Domain
					id = evt_f[iP];
					// �������� !!! ��������� �������� ��������� ������� ����.
					for (i1 = 0; i1 < max_domain; i1++) if (domain_id[i1] == id) {
						ipos = i1; // ��������� ���������� � ����.
						break;
					}
					if (l < maxelm_t) {
						// ��� ���������������� -1, ������� ��������� ���������� � 0.
						ptr[MASKDOMAINFLUID][l] = ipos;// ��������� ���������� � ����.
					}
					else {
						printf("perepolnenie ptr[MASKDOMAINFLUID][l] l>=maxelm_t\n");
						system("pause");
						exit(1);
					}
				}
				else {
					// �� fluid �� ����� (� �� ������)
					if (ptr != NULL) {
						if (ptr[MASKDOMAINFLUID] != NULL) {
							if (l < maxelm_t) {
								ptr[MASKDOMAINFLUID][l] = -1;
							}
							else {
								printf("perepolnenie 2 ptr[MASKDOMAINFLUID][l] l>=maxelm_t\n");
								system("pause");
								exit(1);
							}
						}
						else {
							printf("error : null ptr[MASKDOMAINFLUID] in constr_ptr_temp.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						//ptr == NULL;
						printf("error : null ptr in constr_ptr_temp.\n");
						system("pause");
						exit(1);
					}
				}

				l++;
			}
		}

		printf("part 14\n");

		// ������� ���������� �� � ������ �� domain FLUID.
		integer *domain_counter = NULL;
		domain_counter = new integer[max_domain];
		if (domain_counter == NULL) {
			// ������������ ������ �� ������ ������������.
			printf("Problem : not enough memory on your equipment for domain_counter constr struct...\n");
			printf("Please any key to exit...\n");
			//getchar();
			system("pause");
			exit(1);
		}
		else {

			for (i = 0; i < max_domain; i++) domain_counter[i] = 0; // �������������

			if (ptr != NULL) {
				if ((ptr[MASKDOMAINFLUID] != NULL)&&(ptr[ENUMERATECONTVOL]!=NULL)) {
					for (i = 0; i < maxelm_t; i++) {
						if (ptr[MASKDOMAINFLUID][i] == -1) {
							ptr[ENUMERATECONTVOL][i] = -1;
						}
						else {
							ptr[ENUMERATECONTVOL][i] = domain_counter[ptr[MASKDOMAINFLUID][i]];
							domain_counter[ptr[MASKDOMAINFLUID][i]]++;
						}
					}
				}
				else {
					printf("memory for ptr[MASKDOMAINFLUID] no allocate. error.\n");
					printf("see function constr_ptr_temp in module constr struct\n");
					system("pause");
					exit(1);
				}
			}
			else {
				printf("memory for ptr no allocate. error.\n");
				printf("see function constr_ptr_temp in module constr struct\n");
				system("pause");
				exit(1);
			}


			if (!breconstruct) {
				if (flow_interior > 0) {
					f = new FLOW[flow_interior];
					for (l = 0; l < flow_interior; l++) {
						f[l].ptr = NULL;
						f[l].nvtx = NULL;
						f[l].pa = NULL;
						f[l].sosedi = NULL;
						f[l].sosedb = NULL;
						f[l].whot_is_block = NULL;
						f[l].potent = NULL;
						f[l].prop = NULL;
						f[l].prop_b = NULL;
						f[l].alpha = NULL;
						f[l].slau = NULL;
						f[l].slau_bon = NULL;
						f[l].diag_coef = NULL;
						f[l].iN = NULL;
						f[l].id = NULL;
						f[l].rdistWall = NULL;
						f[l].SInvariantStrainRateTensor = NULL;
						f[l].mf = NULL;
						f[l].icolor_different_fluid_domain = NULL;
						f[l].ifrontregulationgl = NULL;
						f[l].ibackregulationgl = NULL;
					}
				}
				else {
					f = NULL;
				}
			}
			if (f != NULL) {
				// ������ �� ���� ������ �����.
				for (l = 0; l < flow_interior; l++) {
					// ����� ��������� ��.
					// �������� ������� �������� ����� ����� ������� � ����������.
					// ����� ������� ��� �� ������������ ���������� whot_is_block ��� ����������������� �����������.
					f[l].whot_is_block = NULL;
					f[l].maxelm = domain_counter[l];
#if doubleintprecision == 1
					printf("FLUID%lld maxelm=%lld\n", l, f[l].maxelm);
#else
					printf("FLUID%d maxelm=%d\n", l, f[l].maxelm);
#endif
					
					// ��������� ����������� ������.
					if (f[l].ptr != NULL) {
						delete[] f[l].ptr;
					}
					f[l].ptr = NULL;
					f[l].ptr = new integer[f[l].maxelm];
#if doubleintprecision == 1
					printf("delete flow %lld ptr\n", l);
#else
					printf("delete flow %d ptr\n", l);
#endif
					
					if (f[l].ptr == NULL) { // -2N
						// ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
						printf("Problem : not enough memory on your equipment for ptr flow%lld constr struct...\n", l);
#else
						printf("Problem : not enough memory on your equipment for ptr flow%d constr struct...\n", l);
#endif
						printf("Please any key to exit...\n");
						//getchar();
						system("pause");
						exit(1);
					}
					for (j = 0; j < maxelm_t; j++) {
						if (ptr[MASKDOMAINFLUID][j] == l) {
							f[l].ptr[ptr[ENUMERATECONTVOL][j]] = j;
						}
					}
				}
			}

			printf("part 15\n");

			// ������� ���������� �� �� �������������� ���������.
			for (i = 0; i < max_domain; i++) domain_counter[i] = 0; // �������������

			for (i = 0; i < inx; i++) for (j = 0; j < iny; j++) for (k = 0; k < inz; k++) {
				iP = i + j*inx + k*inx*iny;
				if (evt_f2[MASKDOMAINFLUID][iP] == 0) {
					evt_f2[ENUMERATECONTVOL][iP] = 0; // �� ����������� ��������� �������
				}
				else {
					// ���������� ��������� ����� ����������������� ����������.
					// ��������� ���������� � �������.
					evt_f2[ENUMERATECONTVOL][iP] = domain_counter[evt_f2[MASKDOMAINFLUID][iP] - 1] + 1;
					domain_counter[evt_f2[MASKDOMAINFLUID][iP] - 1] = domain_counter[evt_f2[MASKDOMAINFLUID][iP] - 1] + 1;
				}
			}


			/*
			// debug GOOD
			// �������� ������������ ������������� �� �����.
			for (k=0; k<inz; k++) {
				for (i=0; i<inx; i++) {
					for (j=0; j<iny; j++) {
						 iP=i+j*inx+k*inx*iny;
						 #if doubleintprecision == 1
								printf("%lld ",evt_f2[ENUMERATECONTVOL][iP]);
						 #else
								printf("%d ",evt_f2[ENUMERATECONTVOL][iP]);
						 #endif
						 
					}
					printf("\n");
				}
				getchar();
				printf("\n\n");
			}
			//*/


			if (domain_counter != NULL) {
				delete[] domain_counter;
				domain_counter = NULL;
			}


		}

		if (domain_id != NULL) {
			delete[] domain_id;
			domain_id = NULL;
		}
		

		printf("part 16\n");

	}

	

} // constr_ptr_temp

// ���������� ������� SetLength �� Delphi 
// ��� ��������� �������� ������������� ������� ra. 
// ���������� ���������������� ������ ra ������������ 
//  �������. 
// �� ����� ������ ������� ������ ������ �� ����������.
void SetLength_point(TOCHKA* &ra, integer isizeold, integer isize) 
{
	// isize - ����� ����� ������������� �������.
	TOCHKA *temp=NULL;
	temp = new TOCHKA[isize];
	if (temp==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for SetLength_pointeger constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
    integer i;
	for (i=0; i<isize; i++) {
		temp[i].x=0.0; // �������������
		temp[i].y=0.0;
		temp[i].z=0.0;
	}
    /*
	integer isizeold;
	if (ra != NULL) {
       isizeold = sizeof(ra)/sizeof(ra[0]); // ����� ������� �������
	   #if doubleintprecision == 1
			printf("%lld  ",isizeold); // �� ����� ��������� ���������� ������ �������
	   #else
			printf("%d  ",isizeold); // �� ����� ��������� ���������� ������ �������
	   #endif
       
	} 
	  else
	{
       isizeold=0;
	}
	*/
	if (isize < isizeold) isizeold=isize;
	for (i=0; i<isizeold; i++) temp[i]=ra[i];
	
	if (ra != NULL) {
		delete[]  ra; // ����������� �������
		ra = NULL;
	}
	ra = new TOCHKA[isize]; // ��������� ������
	for (i=0; i<isize; i++) ra[i]=temp[i]; // �����������
	if (temp != NULL) {
		delete[] temp; // ������������ ������
		temp = NULL;
	}
	
} // SetLength_point

// ���������� �������� �����. �������� ��������� ������.
// ��������� ���������� ����� � ������� pa
// ����� il
void addpoint_old(TOCHKA* &pa,integer &maxnode,TOCHKA pnew, integer* &ent, integer node, bool* &bvisit) {
	//integer i=0;
	bool bfind=false;
	/* // ��������� ������� ����
	for (i=0; i<maxnode; i++) { 
		if ((fabs(pa[i].x-pnew.x)<admission) && (fabs(pa[i].y-pnew.y)<admission) && (fabs(pa[i].z-pnew.z)<admission)) {
			bfind=true;
		}
	}*/
	bfind=bvisit[node];

	if (!bfind) {
		// ����������, ����� ��� �� �����������
		//SetLength_point(pa, maxnode, maxnode+1);
		pa[maxnode]=pnew;
		ent[node]=maxnode+1; // ��������� ���������� � �������
		bvisit[node]=true;
		maxnode++;
#if doubleintprecision == 1
		//printf("maxnode=%lld\n",maxnode);
#else
		//printf("maxnode=%d\n",maxnode);
#endif
		
	}
	else {
		// ������� 16.04.2017.
		/*
		// ��������� ������� ����
		integer ifound = -1;
		for (integer i = 0; i<maxnode; i++) {
			if ((fabs(pa[i].x - pnew.x)<admission) && (fabs(pa[i].y - pnew.y)<admission) && (fabs(pa[i].z - pnew.z)<admission)) {
				bfind = true;
				ifound = i;
				break;
			}
		}
		ent[node] = ifound+1;
		*/
	}
} // addpoint_old


// ���������� ������ 4 ���� 2017.
  // ��������� ���������� ����� � ������� pa
void addpoint(TOCHKA* &pa, integer &maxnode, TOCHKA pnew, integer* &ent, integer node, bool* &bvisit) {

	if (!bvisit[node]) {
		// ����������, ����� ��� �� �����������
		
		pa[maxnode++] = pnew;
		ent[node] = maxnode; // ��������� ���������� � �������
		bvisit[node] = true; // ���������� ����.
		
	}
	
} // addpoint


// ������ ������ ����� ������������� ��������� �������
void constr_nodes(TOCHKA* &pa, integer &maxnode, integer* &ent, integer iflag, integer* &whot_is_block, integer* &evt,
	integer inx, integer iny, integer inz,
	doublereal *xpos, doublereal *ypos, doublereal *zpos, BLOCK* b, integer lb, TOCKA_INT* &tck_int_list, integer &maxelm) {
	// iflag - ��������� ��� �������� : TEMPERATURE ��� HYDRODINAMIC � 
	// ��������� � ����� ��������� ����� �������� � � ����� ��������� �������.

	// ent - ���������� ��������� �����.
	// ��������� ����� ���������� � �������, ���� ���� �� �����������
	// ��������� ������� �� �������� �������� 0.
	ent = NULL;
	ent = new integer[(inx + 1)*(iny + 1)*(inz + 1)];
	if (ent == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for ent constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	// ��� �� �������� ���� � ������� node
	bool *bvisit = NULL;
	bvisit = new bool[(inx + 1)*(iny + 1)*(inz + 1)];
	if (bvisit == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for bvisit constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	// ��������� ������. ����� ����� ��� ������������� ����������:
	pa = NULL;
	pa = new TOCHKA[(inx + 1)*(iny + 1)*(inz + 1)];
	if (pa == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for pa constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}


	TOCHKA p, pnew;
	integer i, j, k;
#pragma omp parallel for
	for (i = 0; i<(inx + 1)*(iny + 1)*(inz + 1); i++) {
		ent[i] = 0;
		bvisit[i] = false;
	}

	bool inDomain = false;
	integer ib, node;
	maxnode = 0;// maxnode - ������� ����� ������� pa
	// ���� �� ���� ����������� �������
	//for (i = 0; i<inx; i++) for (j = 0; j<iny; j++) for (k = 0; k<inz; k++) {
	for (integer iscan = 0; iscan<maxelm; iscan++) {

			integer i = tck_int_list[iscan].i;
			integer j = tck_int_list[iscan].j;
			integer k = tck_int_list[iscan].k;

		p.x = 0.5*(xpos[i] + xpos[i + 1]);
		p.y = 0.5*(ypos[j] + ypos[j + 1]);
		p.z = 0.5*(zpos[k] + zpos[k + 1]);
		integer iP = 0;
		switch (iflag) {
		case TEMPERATURE:
			// �������������� ��������� ������� ����.
			//inDomain = in_model_temp(p, ib, b, lb);
		    iP = evt[i + j*inx + k*inx*iny];
			if (iP > 0) {
				ib = whot_is_block[iP-1];
				if (b[ib].itype == HOLLOW) {
					inDomain = false;
				}
				else inDomain = true;
			}
			else inDomain = false;			
			break;
		case HYDRODINAMIC: 
			// �������������� ��������� ������� ����.
			//inDomain = in_model_flow(p, ib, b, lb);
			iP = evt[i + j*inx + k*inx*iny];
			if (iP > 0) {
				ib = whot_is_block[iP - 1];
				if (b[ib].itype != FLUID) {
					inDomain = false;
				}
				else inDomain = true;
			}
			else inDomain = false;
			break;
		}

		if (inDomain) {
			// ����������� ����� ����������� ������
			pnew.x = xpos[i];
			pnew.y = ypos[j];
			pnew.z = zpos[k];
			node = i + j*(inx + 1) + k*(inx + 1)*(iny + 1);
			addpoint(pa, maxnode, pnew, ent, node, bvisit);
			pnew.x = xpos[i + 1];
			pnew.y = ypos[j];
			pnew.z = zpos[k];
			node = (i + 1) + j*(inx + 1) + k*(inx + 1)*(iny + 1);
			addpoint(pa, maxnode, pnew, ent, node, bvisit);
			pnew.x = xpos[i];
			pnew.y = ypos[j + 1];
			pnew.z = zpos[k];
			node = i + (j + 1)*(inx + 1) + k*(inx + 1)*(iny + 1);
			addpoint(pa, maxnode, pnew, ent, node, bvisit);
			pnew.x = xpos[i + 1];
			pnew.y = ypos[j + 1];
			pnew.z = zpos[k];
			node = (i + 1) + (j + 1)*(inx + 1) + k*(inx + 1)*(iny + 1);
			addpoint(pa, maxnode, pnew, ent, node, bvisit);
			pnew.x = xpos[i];
			pnew.y = ypos[j];
			pnew.z = zpos[k + 1];
			node = i + j*(inx + 1) + (k + 1)*(inx + 1)*(iny + 1);
			addpoint(pa, maxnode, pnew, ent, node, bvisit);
			pnew.x = xpos[i + 1];
			pnew.y = ypos[j];
			pnew.z = zpos[k + 1];
			node = (i + 1) + j*(inx + 1) + (k + 1)*(inx + 1)*(iny + 1);
			addpoint(pa, maxnode, pnew, ent, node, bvisit);
			pnew.x = xpos[i];
			pnew.y = ypos[j + 1];
			pnew.z = zpos[k + 1];
			node = i + (j + 1)*(inx + 1) + (k + 1)*(inx + 1)*(iny + 1);
			addpoint(pa, maxnode, pnew, ent, node, bvisit);
			pnew.x = xpos[i + 1];
			pnew.y = ypos[j + 1];
			pnew.z = zpos[k + 1];
			node = (i + 1) + (j + 1)*(inx + 1) + (k + 1)*(inx + 1)*(iny + 1);
			addpoint(pa, maxnode, pnew, ent, node, bvisit);
		}
	}

	// ������������ ������ 
	// ���������� ������� �������
	SetLength_point(pa, (inx + 1)*(iny + 1)*(inz + 1), maxnode);
	if (bvisit != NULL) {
		delete[] bvisit;
		bvisit = NULL;
	}
} // constr_nodes





// ������ ������ ����� ������������� ��������� �������
void constr_nodes_flow(TOCHKA* &pa, integer &maxnode, integer* &ent,
	integer **evt_f2, integer iDom,
	integer inx, integer iny, integer inz,
	doublereal *xpos, doublereal *ypos, doublereal *zpos) {

	// ent - ���������� ��������� �����.
	// ��������� ����� ���������� � �������, ���� ���� �� �����������
	// ��������� ������� �� �������� �������� 0.
	ent = NULL;
	ent = new integer[(inx + 1)*(iny + 1)*(inz + 1)];
	if (ent == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for ent flow constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	// ��� �� �������� ���� � ������� node
	bool *bvisit = NULL;
	bvisit = new bool[(inx + 1)*(iny + 1)*(inz + 1)];
	if (bvisit == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for bvisit flow constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	// ��������� ������. ����� ����� ��� ������������� ����������:
	pa = NULL;
	pa = new TOCHKA[(inx + 1)*(iny + 1)*(inz + 1)];
	if (pa == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for pa flow constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}


	// �������������:
	integer i, j, k;
	for (i = 0; i<(inx + 1)*(iny + 1)*(inz + 1); i++) {
		ent[i] = 0;
		bvisit[i] = false;
	}

	bool inDomain = false;
	integer node, ivol;
	TOCHKA pnew;
	maxnode = 0;// maxnode - ������� ����� ������� pa
	// ���� �� ���� ����������� �������
	for (i = 0; i<inx; i++) for (j = 0; j<iny; j++) for (k = 0; k<inz; k++) {

		ivol = i + j*inx + k*inx*iny; // ����� ������������ ������
		if (evt_f2[1][ivol] == (iDom + 1)) inDomain = true;

		if (inDomain) {
			// ����������� ����� ����������� ������
			pnew.x = xpos[i];
			pnew.y = ypos[j];
			pnew.z = zpos[k];
			node = i + j*(inx + 1) + k*(inx + 1)*(iny + 1);
			addpoint(pa, maxnode, pnew, ent, node, bvisit);
			pnew.x = xpos[i + 1];
			pnew.y = ypos[j];
			pnew.z = zpos[k];
			node = (i + 1) + j*(inx + 1) + k*(inx + 1)*(iny + 1);
			addpoint(pa, maxnode, pnew, ent, node, bvisit);
			pnew.x = xpos[i];
			pnew.y = ypos[j + 1];
			pnew.z = zpos[k];
			node = i + (j + 1)*(inx + 1) + k*(inx + 1)*(iny + 1);
			addpoint(pa, maxnode, pnew, ent, node, bvisit);
			pnew.x = xpos[i + 1];
			pnew.y = ypos[j + 1];
			pnew.z = zpos[k];
			node = (i + 1) + (j + 1)*(inx + 1) + k*(inx + 1)*(iny + 1);
			addpoint(pa, maxnode, pnew, ent, node, bvisit);
			pnew.x = xpos[i];
			pnew.y = ypos[j];
			pnew.z = zpos[k + 1];
			node = i + j*(inx + 1) + (k + 1)*(inx + 1)*(iny + 1);
			addpoint(pa, maxnode, pnew, ent, node, bvisit);
			pnew.x = xpos[i + 1];
			pnew.y = ypos[j];
			pnew.z = zpos[k + 1];
			node = (i + 1) + j*(inx + 1) + (k + 1)*(inx + 1)*(iny + 1);
			addpoint(pa, maxnode, pnew, ent, node, bvisit);
			pnew.x = xpos[i];
			pnew.y = ypos[j + 1];
			pnew.z = zpos[k + 1];
			node = i + (j + 1)*(inx + 1) + (k + 1)*(inx + 1)*(iny + 1);
			addpoint(pa, maxnode, pnew, ent, node, bvisit);
			pnew.x = xpos[i + 1];
			pnew.y = ypos[j + 1];
			pnew.z = zpos[k + 1];
			node = (i + 1) + (j + 1)*(inx + 1) + (k + 1)*(inx + 1)*(iny + 1);
			addpoint(pa, maxnode, pnew, ent, node, bvisit);
		}
	}

	// ������������ ������ 
	// ���������� ������� �������
	SetLength_point(pa, (inx + 1)*(iny + 1)*(inz + 1), maxnode);
	if (bvisit != NULL) {
		delete[] bvisit;
		bvisit = NULL;
	}
} // constr_nodes_flow



// ��� ������� ������������ ������ ��������������
// ��������� ������� ���������� ������ ��� ������.
void constr_nvtx(integer* evt, integer* ent, integer** &nvtx, integer &maxelm,
	integer inx, integer iny, integer inz, TOCKA_INT* &tck_int_list) {

	// ��������� ent ���������� � �������.

	// ������ �� ���� ����������� ������� �������������
	// ��������� �������
	// maxelm - ����� ����������� ������� ������������� ��������� �������
	//maxelm = 0;
	integer i, j, k;
	integer ic, l = 0;
	integer i1, i2, i3, i4, i5, i6, i7, i8;
	// ������� ���������� ����������� ������� 
	// ������������� ��������� �������.
	/*
	for (i = 0; i<(inx); i++) for (j = 0; j<(iny); j++) for (k = 0; k<(inz); k++) {
		ic = i + j*inx + k*inx*iny;
		if (evt[ic] > 0) {
			maxelm++;
		}
	}
	*/
	nvtx = NULL;
	nvtx = new integer*[8];
	if (nvtx == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for nvtx constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	for (i = 0; i<8; i++) nvtx[i] = NULL;
	for (i = 0; i<8; i++) {
		nvtx[i] = new integer[maxelm];
		if (nvtx[i] == NULL) {
			// ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
			printf("Problem : not enough memory on your equipment for nvtx[%lld] constr struct...\n", i);
#else
			printf("Problem : not enough memory on your equipment for nvtx[%d] constr struct...\n", i);
#endif
			printf("Please any key to exit...\n");
			//getchar();
			system("pause");
			exit(1);
		}
	}
	// ����� �� ���� ����������� �������.
	//for (i = 0; i<(inx); i++) for (j = 0; j<(iny); j++) for (k = 0; k<(inz); k++) {
		//ic = i + j*inx + k*inx*iny;
		//if (evt[ic]>0) {
	for (integer iscan=0; iscan<maxelm; iscan++) {
		{
			integer i = tck_int_list[iscan].i;
			integer j = tck_int_list[iscan].j;
			integer k = tck_int_list[iscan].k;
			ic = i + j * inx + k * inx*iny;

			// ����������� ����� ����������� ��������� �������.
			i1 = i + j*(inx + 1) + k*(inx + 1)*(iny + 1);
			i2 = (i + 1) + j*(inx + 1) + k*(inx + 1)*(iny + 1);
			i3 = i + (j + 1)*(inx + 1) + k*(inx + 1)*(iny + 1);
			i4 = (i + 1) + (j + 1)*(inx + 1) + k*(inx + 1)*(iny + 1);
			i5 = i + j*(inx + 1) + (k + 1)*(inx + 1)*(iny + 1);
			i6 = (i + 1) + j*(inx + 1) + (k + 1)*(inx + 1)*(iny + 1);
			i7 = i + (j + 1)*(inx + 1) + (k + 1)*(inx + 1)*(iny + 1);
			i8 = (i + 1) + (j + 1)*(inx + 1) + (k + 1)*(inx + 1)*(iny + 1);
			// ������� 15 ������ 2017.
			// ent - ��������� ���������� � �������.
			if ((ent[i1] > 0) && (ent[i2] > 0) && (ent[i3] > 0) && (ent[i4] > 0) && (ent[i5] > 0) && (ent[i6] > 0) && (ent[i7] > 0) && (ent[i8] > 0)) {
				nvtx[0][l] = ent[i1];
				nvtx[1][l] = ent[i2];
				nvtx[2][l] = ent[i3];
				nvtx[3][l] = ent[i4];
				nvtx[4][l] = ent[i5];
				nvtx[5][l] = ent[i6];
				nvtx[6][l] = ent[i7];
				nvtx[7][l] = ent[i8];

				l++;

				if ((ent[i1] == 0) || (ent[i2] == 0) || (ent[i3] == 0) || (ent[i4] == 0) || (ent[i5] == 0) || (ent[i6] == 0) || (ent[i7] == 0) || (ent[i8] == 0)) {
#if doubleintprecision == 1
					printf("error : ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld \n", i1, ent[i1], i2, ent[i2], i3, ent[i3], i4, ent[i4], i5, ent[i5], i6, ent[i6], i7, ent[i7], i8, ent[i8]);
#else
					printf("error : ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d \n", i1, ent[i1], i2, ent[i2], i3, ent[i3], i4, ent[i4], i5, ent[i5], i6, ent[i6], i7, ent[i7], i8, ent[i8]);
#endif
					//getchar();
					system("pause");
				}
			}
			else {
#if doubleintprecision == 1
				printf("error TEMP ent <= 0: ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld \n", i1, ent[i1], i2, ent[i2], i3, ent[i3], i4, ent[i4], i5, ent[i5], i6, ent[i6], i7, ent[i7], i8, ent[i8]);
#else
				printf("error TEMP ent <= 0: ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d \n", i1, ent[i1], i2, ent[i2], i3, ent[i3], i4, ent[i4], i5, ent[i5], i6, ent[i6], i7, ent[i7], i8, ent[i8]);
#endif
				//getchar();
				system("pause");
			}
			
			
		}
	}
} // constr_nvtx

// ������� � ������� �������� ����������.
// 9.08.2017.
void walk_in_octree_icolor_different_fluid_domain(octTree* &oc, integer inx, integer iny, integer inz, integer* &icolor_different_fluid_domain, integer** evt_f2) {


	top_ALICE_STACK = 0;
	if (oc->link0 != NULL) {
		my_ALICE_STACK[top_ALICE_STACK].link = (oc->link0);
		my_ALICE_STACK[top_ALICE_STACK].maxx = oc->link0->maxx;
		my_ALICE_STACK[top_ALICE_STACK].minx = oc->link0->minx;
		my_ALICE_STACK[top_ALICE_STACK].maxy = oc->link0->maxy;
		my_ALICE_STACK[top_ALICE_STACK].miny = oc->link0->miny;
		my_ALICE_STACK[top_ALICE_STACK].maxz = oc->link0->maxz;
		my_ALICE_STACK[top_ALICE_STACK].minz = oc->link0->minz;
		top_ALICE_STACK++;
	}
	if (oc->link1 != NULL) {
		my_ALICE_STACK[top_ALICE_STACK].link = (oc->link1);
		my_ALICE_STACK[top_ALICE_STACK].maxx = oc->link1->maxx;
		my_ALICE_STACK[top_ALICE_STACK].minx = oc->link1->minx;
		my_ALICE_STACK[top_ALICE_STACK].maxy = oc->link1->maxy;
		my_ALICE_STACK[top_ALICE_STACK].miny = oc->link1->miny;
		my_ALICE_STACK[top_ALICE_STACK].maxz = oc->link1->maxz;
		my_ALICE_STACK[top_ALICE_STACK].minz = oc->link1->minz;
		top_ALICE_STACK++;
	}
	if (oc->link2 != NULL) {
		my_ALICE_STACK[top_ALICE_STACK].link = (oc->link2);
		my_ALICE_STACK[top_ALICE_STACK].maxx = oc->link2->maxx;
		my_ALICE_STACK[top_ALICE_STACK].minx = oc->link2->minx;
		my_ALICE_STACK[top_ALICE_STACK].maxy = oc->link2->maxy;
		my_ALICE_STACK[top_ALICE_STACK].miny = oc->link2->miny;
		my_ALICE_STACK[top_ALICE_STACK].maxz = oc->link2->maxz;
		my_ALICE_STACK[top_ALICE_STACK].minz = oc->link2->minz;
		top_ALICE_STACK++;
	}
	if (oc->link3 != NULL) {
		my_ALICE_STACK[top_ALICE_STACK].link = (oc->link3);
		my_ALICE_STACK[top_ALICE_STACK].maxx = oc->link3->maxx;
		my_ALICE_STACK[top_ALICE_STACK].minx = oc->link3->minx;
		my_ALICE_STACK[top_ALICE_STACK].maxy = oc->link3->maxy;
		my_ALICE_STACK[top_ALICE_STACK].miny = oc->link3->miny;
		my_ALICE_STACK[top_ALICE_STACK].maxz = oc->link3->maxz;
		my_ALICE_STACK[top_ALICE_STACK].minz = oc->link3->minz;
		top_ALICE_STACK++;
	}
	if (oc->link4 != NULL) {
		my_ALICE_STACK[top_ALICE_STACK].link = (oc->link4);
		my_ALICE_STACK[top_ALICE_STACK].maxx = oc->link4->maxx;
		my_ALICE_STACK[top_ALICE_STACK].minx = oc->link4->minx;
		my_ALICE_STACK[top_ALICE_STACK].maxy = oc->link4->maxy;
		my_ALICE_STACK[top_ALICE_STACK].miny = oc->link4->miny;
		my_ALICE_STACK[top_ALICE_STACK].maxz = oc->link4->maxz;
		my_ALICE_STACK[top_ALICE_STACK].minz = oc->link4->minz;
		top_ALICE_STACK++;
	}
	if (oc->link5 != NULL) {
		my_ALICE_STACK[top_ALICE_STACK].link = (oc->link5);
		my_ALICE_STACK[top_ALICE_STACK].maxx = oc->link5->maxx;
		my_ALICE_STACK[top_ALICE_STACK].minx = oc->link5->minx;
		my_ALICE_STACK[top_ALICE_STACK].maxy = oc->link5->maxy;
		my_ALICE_STACK[top_ALICE_STACK].miny = oc->link5->miny;
		my_ALICE_STACK[top_ALICE_STACK].maxz = oc->link5->maxz;
		my_ALICE_STACK[top_ALICE_STACK].minz = oc->link5->minz;
		top_ALICE_STACK++;
	}
	if (oc->link6 != NULL) {
		my_ALICE_STACK[top_ALICE_STACK].link = (oc->link6);
		my_ALICE_STACK[top_ALICE_STACK].maxx = oc->link6->maxx;
		my_ALICE_STACK[top_ALICE_STACK].minx = oc->link6->minx;
		my_ALICE_STACK[top_ALICE_STACK].maxy = oc->link6->maxy;
		my_ALICE_STACK[top_ALICE_STACK].miny = oc->link6->miny;
		my_ALICE_STACK[top_ALICE_STACK].maxz = oc->link6->maxz;
		my_ALICE_STACK[top_ALICE_STACK].minz = oc->link6->minz;
		top_ALICE_STACK++;
	}
	if (oc->link7 != NULL) {
		my_ALICE_STACK[top_ALICE_STACK].link = (oc->link7);
		my_ALICE_STACK[top_ALICE_STACK].maxx = oc->link7->maxx;
		my_ALICE_STACK[top_ALICE_STACK].minx = oc->link7->minx;
		my_ALICE_STACK[top_ALICE_STACK].maxy = oc->link7->maxy;
		my_ALICE_STACK[top_ALICE_STACK].miny = oc->link7->miny;
		my_ALICE_STACK[top_ALICE_STACK].maxz = oc->link7->maxz;
		my_ALICE_STACK[top_ALICE_STACK].minz = oc->link7->minz;
		top_ALICE_STACK++;
	}
	while (top_ALICE_STACK > 0) {
		if (my_ALICE_STACK[top_ALICE_STACK - 1].link != NULL) {
			if (my_ALICE_STACK[top_ALICE_STACK - 1].link->dlist == true) {



				octTree* octree1 = my_ALICE_STACK[top_ALICE_STACK - 1].link;

				// ��������� �� 8.
				integer minx = my_ALICE_STACK[top_ALICE_STACK - 1].minx;
				integer maxx = my_ALICE_STACK[top_ALICE_STACK - 1].maxx;
				integer miny = my_ALICE_STACK[top_ALICE_STACK - 1].miny;
				integer maxy = my_ALICE_STACK[top_ALICE_STACK - 1].maxy;
				integer minz = my_ALICE_STACK[top_ALICE_STACK - 1].minz;
				integer maxz = my_ALICE_STACK[top_ALICE_STACK - 1].maxz;

				// ���������  ����������.
				my_ALICE_STACK[top_ALICE_STACK - 1].link = NULL;
				top_ALICE_STACK--;

				// ��������� ����.
				if (octree1->inum_FD > 0) {
					integer ielm = octree1->inum_FD - 1; // ����� nvtx.
					integer ic = 0;
					ic = minx + miny*inx + minz*inx*iny;
					integer id_found = ielm;

					icolor_different_fluid_domain[id_found] = evt_f2[MASKDOMAINFLUIDCOLOR][ic]; // ���������� ����� ������� �������� � ������� ���� evt_f2.
				}

			}
			else {
				// ���������� ���������� �� �������.
				STACK_ALICE buf1 = my_ALICE_STACK[top_ALICE_STACK - 1];
				STACK_ALICE* buf = &buf1;
				top_ALICE_STACK--;
				if (buf->link->link0 != NULL) {
					my_ALICE_STACK[top_ALICE_STACK].link = (buf->link->link0);
					my_ALICE_STACK[top_ALICE_STACK].maxx = buf->link->link0->maxx;
					my_ALICE_STACK[top_ALICE_STACK].minx = buf->link->link0->minx;
					my_ALICE_STACK[top_ALICE_STACK].maxy = buf->link->link0->maxy;
					my_ALICE_STACK[top_ALICE_STACK].miny = buf->link->link0->miny;
					my_ALICE_STACK[top_ALICE_STACK].maxz = buf->link->link0->maxz;
					my_ALICE_STACK[top_ALICE_STACK].minz = buf->link->link0->minz;
					top_ALICE_STACK++;
				}
				if (buf->link->link1 != NULL) {
					my_ALICE_STACK[top_ALICE_STACK].link = (buf->link->link1);
					my_ALICE_STACK[top_ALICE_STACK].maxx = buf->link->link1->maxx;
					my_ALICE_STACK[top_ALICE_STACK].minx = buf->link->link1->minx;
					my_ALICE_STACK[top_ALICE_STACK].maxy = buf->link->link1->maxy;
					my_ALICE_STACK[top_ALICE_STACK].miny = buf->link->link1->miny;
					my_ALICE_STACK[top_ALICE_STACK].maxz = buf->link->link1->maxz;
					my_ALICE_STACK[top_ALICE_STACK].minz = buf->link->link1->minz;
					top_ALICE_STACK++;
				}
				if (buf->link->link2 != NULL) {
					my_ALICE_STACK[top_ALICE_STACK].link = (buf->link->link2);
					my_ALICE_STACK[top_ALICE_STACK].maxx = buf->link->link2->maxx;
					my_ALICE_STACK[top_ALICE_STACK].minx = buf->link->link2->minx;
					my_ALICE_STACK[top_ALICE_STACK].maxy = buf->link->link2->maxy;
					my_ALICE_STACK[top_ALICE_STACK].miny = buf->link->link2->miny;
					my_ALICE_STACK[top_ALICE_STACK].maxz = buf->link->link2->maxz;
					my_ALICE_STACK[top_ALICE_STACK].minz = buf->link->link2->minz;
					top_ALICE_STACK++;
				}
				if (buf->link->link3 != NULL) {
					my_ALICE_STACK[top_ALICE_STACK].link = (buf->link->link3);
					my_ALICE_STACK[top_ALICE_STACK].maxx = buf->link->link3->maxx;
					my_ALICE_STACK[top_ALICE_STACK].minx = buf->link->link3->minx;
					my_ALICE_STACK[top_ALICE_STACK].maxy = buf->link->link3->maxy;
					my_ALICE_STACK[top_ALICE_STACK].miny = buf->link->link3->miny;
					my_ALICE_STACK[top_ALICE_STACK].maxz = buf->link->link3->maxz;
					my_ALICE_STACK[top_ALICE_STACK].minz = buf->link->link3->minz;
					top_ALICE_STACK++;
				}
				if (buf->link->link4 != NULL) {
					my_ALICE_STACK[top_ALICE_STACK].link = (buf->link->link4);
					my_ALICE_STACK[top_ALICE_STACK].maxx = buf->link->link4->maxx;
					my_ALICE_STACK[top_ALICE_STACK].minx = buf->link->link4->minx;
					my_ALICE_STACK[top_ALICE_STACK].maxy = buf->link->link4->maxy;
					my_ALICE_STACK[top_ALICE_STACK].miny = buf->link->link4->miny;
					my_ALICE_STACK[top_ALICE_STACK].maxz = buf->link->link4->maxz;
					my_ALICE_STACK[top_ALICE_STACK].minz = buf->link->link4->minz;
					top_ALICE_STACK++;
				}
				if (buf->link->link5 != NULL) {
					my_ALICE_STACK[top_ALICE_STACK].link = (buf->link->link5);
					my_ALICE_STACK[top_ALICE_STACK].maxx = buf->link->link5->maxx;
					my_ALICE_STACK[top_ALICE_STACK].minx = buf->link->link5->minx;
					my_ALICE_STACK[top_ALICE_STACK].maxy = buf->link->link5->maxy;
					my_ALICE_STACK[top_ALICE_STACK].miny = buf->link->link5->miny;
					my_ALICE_STACK[top_ALICE_STACK].maxz = buf->link->link5->maxz;
					my_ALICE_STACK[top_ALICE_STACK].minz = buf->link->link5->minz;
					top_ALICE_STACK++;
				}
				if (buf->link->link6 != NULL) {
					my_ALICE_STACK[top_ALICE_STACK].link = (buf->link->link6);
					my_ALICE_STACK[top_ALICE_STACK].maxx = buf->link->link6->maxx;
					my_ALICE_STACK[top_ALICE_STACK].minx = buf->link->link6->minx;
					my_ALICE_STACK[top_ALICE_STACK].maxy = buf->link->link6->maxy;
					my_ALICE_STACK[top_ALICE_STACK].miny = buf->link->link6->miny;
					my_ALICE_STACK[top_ALICE_STACK].maxz = buf->link->link6->maxz;
					my_ALICE_STACK[top_ALICE_STACK].minz = buf->link->link6->minz;
					top_ALICE_STACK++;
				}
				if (buf->link->link7 != NULL) {
					my_ALICE_STACK[top_ALICE_STACK].link = (buf->link->link7);
					my_ALICE_STACK[top_ALICE_STACK].maxx = buf->link->link7->maxx;
					my_ALICE_STACK[top_ALICE_STACK].minx = buf->link->link7->minx;
					my_ALICE_STACK[top_ALICE_STACK].maxy = buf->link->link7->maxy;
					my_ALICE_STACK[top_ALICE_STACK].miny = buf->link->link7->miny;
					my_ALICE_STACK[top_ALICE_STACK].maxz = buf->link->link7->maxz;
					my_ALICE_STACK[top_ALICE_STACK].minz = buf->link->link7->minz;
					top_ALICE_STACK++;
				}
			}
		}
		//}
		//getchar();
	}
} // walk_in_octree_icolor_different_fluid_domain

// 22 �������� 2016 ���������� ������� ����� ��� ���� �����.
// ������ ������� ���������� ����  evt_f2[MASKDOMAINFLUIDCOLOR] � ������.
void constr_icolor_different_fluid_domain_alice(integer maxelm_flow, integer* &icolor_different_fluid_domain,
	integer inx, integer iny, integer inz, integer** evt_f2,  doublereal* &x_pos, doublereal* &y_pos, doublereal* &z_pos, TOCHKA* pa,
	integer** nvtx, octTree* &oc)
{
	integer i = 0, j = 0, k = 0; // �������� ����� for.

	// ��������� ������.
	icolor_different_fluid_domain = NULL;
	icolor_different_fluid_domain = new integer[maxelm_flow];
	if (icolor_different_fluid_domain == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for icolor_different_fluid_domain constr_nvtx_flow in constr_struct...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}

#if doubleintprecision == 1
	printf("maxelm_flow=%lld inx=%lld, iny=%lld inz=%lld\n", maxelm_flow, inx, iny, inz);
#else
	printf("maxelm_flow=%d inx=%d, iny=%d inz=%d\n", maxelm_flow, inx, iny, inz);
#endif
	
	/*
	// ����������� ��������� ����� ����.
	for (i = 0; i<(inx); i++) for (j = 0; j<(iny); j++) for (k = 0; k<(inz); k++) {
		integer ic = 0;
		ic = i + j*inx + k*inx*iny;
		// ���������� ������ ��������� ������.
		doublereal x_c = 0.5*(x_pos[i]+x_pos[i+1]);
		doublereal y_c = 0.5*(y_pos[j] + y_pos[j + 1]);
		doublereal z_c = 0.5*(z_pos[k] + z_pos[k + 1]);

		
		// ����� ������ ic64.
		bool bfound = false;
		integer id_found = -1;
		
		for (integer i_1 = 0; i_1 < maxelm_flow; i_1++) {
			if (pa[nvtx[0][i_1] - 1].x>=pa[nvtx[1][i_1] - 1].x) {
				printf("fatall error! ");
				printf("maxelm_flow=%d\n", maxelm_flow);
				printf("pa[nvtx[0][i_1] - 1].x=%e pa[nvtx[1][i_1] - 1].x=%e\n", pa[nvtx[0][i_1] - 1].x, pa[nvtx[1][i_1] - 1].x);
				printf("%d %d %d %d %d %d %d %d\n", nvtx[0][i_1], nvtx[1][i_1], nvtx[2][i_1], nvtx[3][i_1], nvtx[4][i_1], nvtx[5][i_1], nvtx[6][i_1], nvtx[7][i_1]);
				system("pause");
			}
			if (pa[nvtx[0][i_1] - 1].y >= pa[nvtx[3][i_1] - 1].y) {
				printf("fatall error! ");
				printf("maxelm_flow=%d\n", maxelm_flow);
				printf("pa[nvtx[0][i_1] - 1].y=%e pa[nvtx[3][i_1] - 1].y=%e\n", pa[nvtx[0][i_1] - 1].y, pa[nvtx[3][i_1] - 1].y);
				printf("%d %d %d %d %d %d %d %d\n", nvtx[0][i_1], nvtx[1][i_1], nvtx[2][i_1], nvtx[3][i_1], nvtx[4][i_1], nvtx[5][i_1], nvtx[6][i_1], nvtx[7][i_1]);
				system("pause");
			}
			if (pa[nvtx[0][i_1] - 1].z >= pa[nvtx[4][i_1] - 1].z) {
				printf("fatall error! ");
				printf("maxelm_flow=%d\n", maxelm_flow);
				printf("pa[nvtx[0][i_1] - 1].z=%e pa[nvtx[4][i_1] - 1].z=%e\n", pa[nvtx[0][i_1] - 1].z, pa[nvtx[4][i_1] - 1].z);
				printf("%d %d %d %d %d %d %d %d\n", nvtx[0][i_1], nvtx[1][i_1], nvtx[2][i_1], nvtx[3][i_1], nvtx[4][i_1], nvtx[5][i_1], nvtx[6][i_1], nvtx[7][i_1]);
				system("pause");
			}

			if ((x_c>pa[nvtx[0][i_1] - 1].x) && (x_c<pa[nvtx[1][i_1] - 1].x) && (y_c>pa[nvtx[0][i_1] - 1].y) && (y_c<pa[nvtx[3][i_1] - 1].y) && (z_c>pa[nvtx[0][i_1] - 1].z) && (z_c<pa[nvtx[4][i_1] - 1].z)) {
				bfound = true;
				id_found = i_1;
				// � ������ ���������� ��������� ����� �� ����� for.
				break;
			}
		}
		if (bfound) {
			icolor_different_fluid_domain[id_found] = evt_f2[MASKDOMAINFLUIDCOLOR][ic]; // ���������� ����� ������� �������� � ������� ���� evt_f2.
		}
	}
	*/
	// 9.08.2017 �� ������ �� ����� ������� ���������� �� ���� ������.
	walk_in_octree_icolor_different_fluid_domain(oc,  inx, iny, inz, icolor_different_fluid_domain,  evt_f2);
} //  constr_icolor_different_fluid_domain_alice





// ��� ������� ������������ ������ ��������������
// ��������� ������� ���������� ������ ��� ������.
void constr_nvtx_flow(integer** evt_f2, integer* &icolor_different_fluid_domain, integer iDom, integer* ent,
	integer** &nvtx, integer &maxelm,
	integer inx, integer iny, integer inz) {
	// ������ �� ���� ����������� ������� �������������
	// ��������� �������
	// maxelm - ����� ����������� ������� ������������� ��������� �������
	maxelm = 0;
	integer i, j, k;
	integer ic, l = 0;
	integer i1, i2, i3, i4, i5, i6, i7, i8;
	// ������� ���������� ����������� ������� 
	// ������������� ��������� �������.
	for (i = 0; i<(inx); i++) for (j = 0; j<(iny); j++) for (k = 0; k<(inz); k++) {
		ic = i + j*inx + k*inx*iny;
		if ((evt_f2[ENUMERATECONTVOL][ic] > 0) && (evt_f2[MASKDOMAINFLUID][ic] == (iDom + 1))) {   
			maxelm++;
		}
	}
	// ��������� ������.
	icolor_different_fluid_domain = NULL;
	icolor_different_fluid_domain = new integer[maxelm];
	if (icolor_different_fluid_domain == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for icolor_different_fluid_domain constr_nvtx_flow in constr_struct...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}

	integer ic64 = 0;
	for (i = 0; i<(inx); i++) for (j = 0; j<(iny); j++) for (k = 0; k<(inz); k++) {
		ic = i + j*inx + k*inx*iny;
		if ((evt_f2[ENUMERATECONTVOL][ic] > 0) && (evt_f2[MASKDOMAINFLUID][ic] == (iDom + 1))) {

			icolor_different_fluid_domain[ic64] = evt_f2[MASKDOMAINFLUIDCOLOR][ic]; // ���������� ����� ������� �������� � ������� ���� evt_f2.
			ic64++;
		}
	}
	nvtx = NULL;
	nvtx = new integer*[8];
	if (nvtx == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for nvtx flow constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	for (i = 0; i<8; i++) nvtx[i] = NULL;
	for (i = 0; i<8; i++) {
		nvtx[i] = new integer[maxelm];
		if (nvtx[i] == NULL) {
			// ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
			printf("Problem : not enough memory on your equipment for nvtx[%lld] constr struct...\n", i);
#else
			printf("Problem : not enough memory on your equipment for nvtx[%d] constr struct...\n", i);
#endif
			
			printf("Please any key to exit...\n");
			//getchar();
			system("pause");
			exit(1);
		}
	}
	// ����� �� ���� ����������� �������.
	for (i = 0; i<(inx); i++) for (j = 0; j<(iny); j++) for (k = 0; k<(inz); k++) {
		ic = i + j*inx + k*inx*iny;
		if ((evt_f2[ENUMERATECONTVOL][ic]>0) && (evt_f2[MASKDOMAINFLUID][ic] == (iDom + 1))) {
			// ����������� ����� ����������� ��������� �������.
			i1 = i + j*(inx + 1) + k*(inx + 1)*(iny + 1);
			i2 = (i + 1) + j*(inx + 1) + k*(inx + 1)*(iny + 1);
			i3 = i + (j + 1)*(inx + 1) + k*(inx + 1)*(iny + 1);
			i4 = (i + 1) + (j + 1)*(inx + 1) + k*(inx + 1)*(iny + 1);
			i5 = i + j*(inx + 1) + (k + 1)*(inx + 1)*(iny + 1);
			i6 = (i + 1) + j*(inx + 1) + (k + 1)*(inx + 1)*(iny + 1);
			i7 = i + (j + 1)*(inx + 1) + (k + 1)*(inx + 1)*(iny + 1);
			i8 = (i + 1) + (j + 1)*(inx + 1) + (k + 1)*(inx + 1)*(iny + 1);

			

			// ������� 15 ������ 2017.
			// ent - ��������� ���������� � �������.
			if ((ent[i1] > 0) && (ent[i2] > 0) && (ent[i3] > 0) && (ent[i4] > 0) && (ent[i5] > 0) && (ent[i6] > 0) && (ent[i7] > 0) && (ent[i8] > 0)) {


					nvtx[0][l] = ent[i1];
					nvtx[1][l] = ent[i2];
					nvtx[2][l] = ent[i3];
					nvtx[3][l] = ent[i4];
					nvtx[4][l] = ent[i5];
					nvtx[5][l] = ent[i6];
					nvtx[6][l] = ent[i7];
					nvtx[7][l] = ent[i8];
					l++;

					if ((ent[i1] == 0) || (ent[i2] == 0) || (ent[i3] == 0) || (ent[i4] == 0) || (ent[i5] == 0) || (ent[i6] == 0) || (ent[i7] == 0) || (ent[i8] == 0)) {
#if doubleintprecision == 1
						printf("error : ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld \n", i1, ent[i1], i2, ent[i2], i3, ent[i3], i4, ent[i4], i5, ent[i5], i6, ent[i6], i7, ent[i7], i8, ent[i8]);
#else
						printf("error : ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d \n", i1, ent[i1], i2, ent[i2], i3, ent[i3], i4, ent[i4], i5, ent[i5], i6, ent[i6], i7, ent[i7], i8, ent[i8]);
#endif
						//getchar();
						system("pause");
					}
			}
			else {
#if doubleintprecision == 1
				printf("error FLOW ent <= 0: ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld ent[%lld]=%lld \n", i1, ent[i1], i2, ent[i2], i3, ent[i3], i4, ent[i4], i5, ent[i5], i6, ent[i6], i7, ent[i7], i8, ent[i8]);
#else
				printf("error FLOW ent <= 0: ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d ent[%d]=%d \n", i1, ent[i1], i2, ent[i2], i3, ent[i3], i4, ent[i4], i5, ent[i5], i6, ent[i6], i7, ent[i7], i8, ent[i8]);
#endif
				//getchar();
				system("pause");
			}

		}
	}
} // constr_nvtx_flow

// ������� �������� ���������� � ���������
void constr_prop(integer* evt, integer* &whot_is_block, integer* ent, doublereal** &prop, integer maxelm, integer iflag, BLOCK* b,
	integer lb, integer inx, integer iny, integer inz, doublereal* &Sc, integer* &ipower_time_depend,
				 doublereal *xpos, doublereal *ypos, doublereal *zpos, TPROP* matlist, TOCKA_INT* &tck_int_list) {
	integer i;
	prop=NULL;
	prop=new doublereal*[8];
	if (prop==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for prop constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	for (i=0; i<8; i++) prop[i]=NULL;
	for (i=0; i<8; i++) {
		prop[i]=new doublereal[maxelm];
		if (prop[i]==NULL) {
	        // ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
			printf("Problem : not enough memory on your equipment for prop[%lld] constr struct...\n", i);
#else
			printf("Problem : not enough memory on your equipment for prop[%d] constr struct...\n", i);
#endif
		    
		    printf("Please any key to exit...\n");
			//getchar();
			system("pause");
			exit(1);
	    }
	}
	Sc=NULL;
	Sc=new doublereal[maxelm];
	if (Sc==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for Sc constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	ipower_time_depend = NULL;
	ipower_time_depend = new integer[maxelm];
	if (ipower_time_depend == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for ipower_time_depend constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}

	integer j,k;
	TOCHKA p;
    bool inDomain=false;
	integer ib,l=0; 
	
	// ������ �� ���� ����������� ������� � ������� ������:
	//for (i=0; i<inx; i++) for (j=0; j<iny; j++) for (k=0; k<inz; k++) {

		//integer ic = i + j*inx + k*inx*iny;
		//if (evt[ic] > 0) {
	for (integer iscan = 0; iscan<maxelm; iscan++) {
		{
			integer i = tck_int_list[iscan].i;
			integer j = tck_int_list[iscan].j;
			integer k = tck_int_list[iscan].k;
			integer ic = i + j * inx + k * inx*iny;

				p.x = 0.5*(xpos[i] + xpos[i + 1]);
				p.y = 0.5*(ypos[j] + ypos[j + 1]);
				p.z = 0.5*(zpos[k] + zpos[k + 1]);
				/*
				switch (iflag) {
				  case TEMPERATURE: inDomain = in_model_temp(p, ib, b, lb); break;
				  case HYDRODINAMIC: inDomain = in_model_flow(p, ib, b, lb); break;
				}
				*/
				integer iP = 0;
				switch (iflag) {
				case TEMPERATURE:
					// �������������� ��������� ������� ����.
					//inDomain = in_model_temp(p, ib, b, lb);
					iP = evt[i + j*inx + k*inx*iny];
					if (iP > 0) {
						ib = whot_is_block[iP - 1];
						if (b[ib].itype == HOLLOW) {
							inDomain = false;
						}
						else inDomain = true;
					}
					else inDomain = false;
					break;
				case HYDRODINAMIC:
					// �������������� ��������� ������� ����.
					//inDomain = in_model_flow(p, ib, b, lb);
					iP = evt[i + j*inx + k*inx*iny];
					if (iP > 0) {
						ib = whot_is_block[iP - 1];
						if (b[ib].itype != FLUID) {
							inDomain = false;
						}
						else inDomain = true;
					}
					else inDomain = false;
					break;
				}

				if (inDomain) {
					if (l < maxelm)
					{
						switch (iflag) {
						case TEMPERATURE:  prop[RHO][l] = matlist[b[ib].imatid].rho;
							//prop[CP][l] = matlist[b[ib].imatid].cp;
							//prop[LAM][l] = matlist[b[ib].imatid].lam;
							prop[CP][l] = get_lam(matlist[b[ib].imatid].n_cp, matlist[b[ib].imatid].temp_cp, matlist[b[ib].imatid].arr_cp, 25.0);
							prop[LAM][l] = get_lam(matlist[b[ib].imatid].n_lam, matlist[b[ib].imatid].temp_lam, matlist[b[ib].imatid].arr_lam, 25.0);

							prop[MULT_LAM_X][l] = matlist[b[ib].imatid].orthotropy_multiplyer_x;
							prop[MULT_LAM_Y][l] = matlist[b[ib].imatid].orthotropy_multiplyer_y;
							prop[MULT_LAM_Z][l] = matlist[b[ib].imatid].orthotropy_multiplyer_z;
							// ������������ ����.
							prop[MU_LAME][l] = matlist[b[ib].imatid].mu_Lame;
							prop[LAMBDA_LAME][l] = matlist[b[ib].imatid].lambda_Lame;

							//Sc[evt[i + j*inx + k*inx*iny] - 1] = b[ib].Sc;
							Sc[evt[i + j*inx + k*inx*iny] - 1] = get_power(b[ib].n_Sc, b[ib].temp_Sc, b[ib].arr_Sc, 20.0);
							ipower_time_depend[evt[i + j*inx + k*inx*iny] - 1] = b[ib].ipower_time_depend;

							//Sc[l]= get_power(b[ib].n_Sc, b[ib].temp_Sc, b[ib].arr_Sc, 20.0);
							//ipower_time_depend[l] = b[ib].ipower_time_depend;
							break;
						case HYDRODINAMIC: prop[RHO][l] = matlist[b[ib].imatid].rho;
							prop[MU][l] = matlist[b[ib].imatid].mu;
							prop[BETA_T][l] = matlist[b[ib].imatid].beta_t;
							break;
						}

						l++;
					}
					else {
						printf("model is incorrect : prop indx >=maxelm\n");
						printf("see constr struct file.\n");
						system("pause");
						exit(1);
					}
				}
		}
	}
} // constr_prop

// ������� �������� ���������� � ���������
// ��� ������ ���� � ������� iDom.
void constr_prop_flow(integer* evt, integer* &whot_is_block, 
	integer** evt_f2, integer iDom, doublereal** &prop,
					  integer maxelm, BLOCK* b, integer lb, 
					  integer inx, integer iny, integer inz, 
				      doublereal *xpos, doublereal *ypos, doublereal *zpos, TPROP* matlist) {
	integer i;
	prop=NULL;
	prop=new doublereal*[3];
	if (prop==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for prop flow constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	for (i=0; i<3; i++) prop[i]=NULL;
	for (i=0; i<3; i++) {
		prop[i]=new doublereal[maxelm];
		if (prop[i]==NULL) {
	        // ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
			printf("Problem : not enough memory on your equipment for prop[%lld] constr struct...\n", i);
#else
			printf("Problem : not enough memory on your equipment for prop[%d] constr struct...\n", i);
#endif
		    
		    printf("Please any key to exit...\n");
			//getchar();
			system("pause");
		    exit(1);
	    }
	}

	integer j,k,ic;
	//TOCHKA p;
    bool inDomain=false;
	integer ib,l=0; 
	
	// ������ �� ���� ����������� ������� � ������� ������:
	for (i=0; i<inx; i++) for (j=0; j<iny; j++) for (k=0; k<inz; k++) {
		ic=i+j*inx+k*inx*iny; // ����� ��
        
		if ((evt_f2[ENUMERATECONTVOL][ic]>0) && (evt_f2[MASKDOMAINFLUID][ic]==(iDom+1))) {

					//p.x = 0.5*(xpos[i] + xpos[i + 1]);
					//p.y = 0.5*(ypos[j] + ypos[j + 1]);
					//p.z = 0.5*(zpos[k] + zpos[k + 1]);

					//inDomain = in_model_flow(p, ib, b, lb); // ���������� ����� ����� ������ ����

					integer iP = evt[i + j*inx + k*inx*iny];
					if (iP > 0) {
						ib = whot_is_block[iP - 1];
						if (b[ib].itype != FLUID) {
							inDomain = false;
						}
						else inDomain = true;
					}
					else inDomain = false;


					if (inDomain) {
						if (l < maxelm) {
							prop[RHO][l] = matlist[b[ib].imatid].rho;
							prop[MU][l] = matlist[b[ib].imatid].mu;
							prop[BETA_T][l] = matlist[b[ib].imatid].beta_t;

							l++;
						}
						else {
							printf("model is incorrect: prop 2 indx>=maxelm\n");
							printf("see file constr struct in function constr_prop_flow\n");
							system("pause");
							exit(1);
						}
					}
				}
	}
} // constr_prop_flow

// �������� ���������� ��������� ������.
// � ���������� ������ ��������� �����,
// ������������� ��������� ������� �����
// ���������� �����.
// �������� !!! ����� ���������� ���������� ����� ����� �������������.
void enumerate_gran_temp(integer** &gran, integer maxelm, integer** nvtx,
						 integer &maxbound, integer **sosed, TOCHKA* pa,
						 SOURCE* s, integer ls) {
	// ��������� ����������� ������.
	if (gran != NULL) {
		for (integer i = 0; i < 6; i++) {
			if (gran[i] != NULL) {
				delete[] gran[i];
				gran[i] = NULL;
			}
		}
		delete[] gran;
	}
	gran = NULL;

	gran=NULL;
	gran=new integer*[6];
	if (gran==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for gran constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	integer i=0, j=0;
	for (i=0; i<6; i++) gran[i]=NULL;
	for (i=0; i<6; i++) {
		gran[i]=new integer[maxelm];
		if (gran[i]==NULL) {
	        // ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
			printf("Problem : not enough memory on your equipment for gran[%lld] constr struct...\n", i);
#else
			printf("Problem : not enough memory on your equipment for gran[%d] constr struct...\n", i);
#endif
		    
		    printf("Please any key to exit...\n");
			//getchar();
			system("pause");
		    exit(1);
	    }
	}
	// �������������:
    for (i=0; i<6; i++) for (j=0; j<maxelm; j++) gran[i][j]=-1;

	bool *bvisit=NULL;
	bvisit=new bool[maxelm];
	if (bvisit==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for bvisit constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
    for (j=0; j<maxelm; j++) bvisit[j]=false; // ������� ��������� ����.

	maxbound=0; // ����� ����� ������
	integer G; // ������� �����
	doublereal x_c, y_c, z_c;
	integer iplane;
	bool bfind=false;
	integer GR; // G Reverse
	for (i=0; i<maxelm; i++) {
		// ���� �� ���� �������
		for (G=0; G<6; G++) {
			if (sosed[G][i]==0) {
				// ����� �� ����������
				gran[G][i]=maxbound++;
				// ���������� ������������� ����� �
				// ����������� ������� ������.
			}
			else {
				// ����� ����������, ����� ����������
				if (bvisit[sosed[G][i]-1]) {
					// ���� ��� ��� �������
					switch (G) {
						case ESIDE : GR=WSIDE; break;
						case WSIDE: GR=ESIDE; break;
						case NSIDE : GR=SSIDE; break;
						case SSIDE : GR=NSIDE; break;
						case TSIDE : GR=BSIDE; break;
						case BSIDE : GR=TSIDE; break;
					}
					gran[G][i]=gran[GR][sosed[G][i]-1];
				}
				else {
					// ������ ���������� ������ ����� � ���������� � ������������
					switch (G) {
						case ESIDE : x_c=pa[nvtx[1][i]-1].x;
							     y_c=0.5*(pa[nvtx[1][i]-1].y+pa[nvtx[3][i]-1].y);
								 z_c=0.5*(pa[nvtx[0][i]-1].z+pa[nvtx[4][i]-1].z);
								 iplane=YZ;
							     break;
						case WSIDE: x_c=pa[nvtx[0][i]-1].x;
							     y_c=0.5*(pa[nvtx[1][i]-1].y+pa[nvtx[3][i]-1].y);
								 z_c=0.5*(pa[nvtx[0][i]-1].z+pa[nvtx[4][i]-1].z);
								 iplane=YZ;
							     break;
						case NSIDE : x_c=0.5*(pa[nvtx[0][i]-1].x+pa[nvtx[1][i]-1].x);
							     y_c=pa[nvtx[2][i]-1].y;
								 z_c=0.5*(pa[nvtx[0][i]-1].z+pa[nvtx[4][i]-1].z);
								 iplane=XZ;
							     break;
						case SSIDE : x_c=0.5*(pa[nvtx[0][i]-1].x+pa[nvtx[1][i]-1].x);
							     y_c=pa[nvtx[0][i]-1].y;
								 z_c=0.5*(pa[nvtx[0][i]-1].z+pa[nvtx[4][i]-1].z);
								 iplane=XZ;
							     break;
						case TSIDE : x_c=0.5*(pa[nvtx[0][i]-1].x+pa[nvtx[1][i]-1].x);
                                 y_c=0.5*(pa[nvtx[1][i]-1].y+pa[nvtx[3][i]-1].y);
								 z_c=pa[nvtx[4][i]-1].z;
                                 iplane=XY;
							     break;
						case BSIDE : x_c=0.5*(pa[nvtx[0][i]-1].x+pa[nvtx[1][i]-1].x);
                                 y_c=0.5*(pa[nvtx[1][i]-1].y+pa[nvtx[3][i]-1].y);
								 z_c=pa[nvtx[0][i]-1].z;
                                 iplane=XY;
							     break;
					} // end case
					bfind=false;
					for (j=0; j<ls; j++) {
						if (s[j].iPlane==iplane) {
							switch (iplane) {
							case XY: s[j].g.zE = s[j].g.zS;
									      if ((x_c>s[j].g.xS) && (x_c<s[j].g.xE) && (y_c>s[j].g.yS) && (y_c<s[j].g.yE) && (fabs(z_c-s[j].g.zE)<admission)) bfind=true;
									      break;
							case XZ:  s[j].g.yE = s[j].g.yS;
									if ((x_c>s[j].g.xS) && (x_c<s[j].g.xE) && (z_c>s[j].g.zS) && (z_c<s[j].g.zE) && (fabs(y_c-s[j].g.yE)<admission)) bfind=true;
									      break;
							case YZ: s[j].g.xE = s[j].g.xS;
									if ((z_c>s[j].g.zS) && (z_c<s[j].g.zE) && (y_c>s[j].g.yS) && (y_c<s[j].g.yE) && (fabs(x_c-s[j].g.xE)<admission)) bfind=true;
									      break;
							}	
						}
						if (bfind) break; // ��������� ����� �� ����� for.
					}
					if (bfind) {
						// ����� ��������� ����� ��������������� ����� maxbound.
                        gran[G][i]=maxbound++;
					}
				}
			}
		}
		bvisit[i]=true; // ���� �������
	}

	if (bvisit != NULL) {
		delete[] bvisit;
		bvisit = NULL;
	}
	// ����� ����� ������ ����� ���������� ������ maxbound.
	// ��� ������ ����� ����� � ������� � �����.

} // enumerate_gran_temp

// �������� ���������� ��������� ������.
// � ���������� ������ ��������� �����,
// ������������� ��������� ������� �����
// ���������� �����.
void enumerate_gran_flow(integer** &gran, integer maxelm, integer** nvtx,
						 integer &maxbound, integer **sosed, TOCHKA* pa) {
	// ��������� ����������� ������.
	gran=NULL;
	gran=new integer*[6];
	if (gran==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for gran flow constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	integer i=0, j=0;
	for (i=0; i<6; i++) gran[i]=NULL;
	for (i=0; i<6; i++) {
		gran[i]=new integer[maxelm];
		if (gran[i]==NULL) {
	        // ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
			printf("Problem : not enough memory on your equipment for gran[%lld] constr struct...\n", i);
#else
			printf("Problem : not enough memory on your equipment for gran[%d] constr struct...\n", i);
#endif
		   
		    printf("Please any key to exit...\n");
			//getchar();
			system("pause");
			exit(1);
	    }
	}
	// �������������:
    for (i=0; i<6; i++) for (j=0; j<maxelm; j++) gran[i][j]=-1;

	maxbound=0; // ����� ����� ������
	integer G; // ������� �����

	for (i=0; i<maxelm; i++) {
		// ���� �� ���� �������
		for (G=0; G<6; G++) {
			if (sosed[G][i]==0) {
				// ����� �� ����������
				gran[G][i]=maxbound++;
				// ���������� ������������� ����� �
				// ����������� ������� ������.
			}
		}	
	}

   
	// ����� ����� ������ ����� ���������� ������ maxbound.
	// ��� ������ ����� ����� � ������� � �����.

} // enumerate_gran_flow

// ���������� ������� ��� ������� ����������� ����. 
// ������ ��������� ������� ���������� � ����� ���������� �� � 
// ����� ��������� ��.
// ���������������: �������� � ��� TEMPER � ��� FLOW
void constr_sosedi(ALICE_PARTITION** &sosedi, integer maxelm, integer** gran,
						integer** sosed) {
	integer i=0;
	// ��������� ����������� ������.
	sosedi=NULL;
	sosedi = new ALICE_PARTITION*[12];
	if (sosedi==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for sosedi constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	for (i=0; i<12; i++) sosedi[i]=NULL;
	for (i=0; i<12; i++) {
		sosedi[i] = new ALICE_PARTITION[maxelm];
		if (sosedi[i]==NULL) {
	        // ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
			printf("Problem : not enough memory on your equipment for sosedi[%lld] constr struct...\n", i);
#else
			printf("Problem : not enough memory on your equipment for sosedi[%d] constr struct...\n", i);
#endif
		   
		    printf("Please any key to exit...\n");
			//getchar();
			system("pause");
		    exit(1);
	    }
	}

	integer G, GG; // ������� �����
	// ����� � ����� �� ���� ���������� �� ������������� ��������� �������
	for (i=0; i<maxelm; i++) {
		// ������ �� ���� ������ ������������� �������� �� � ����������� �������.
		for (G=0; G<6; G++) {
			if (gran[G][i]>-1) {
				// ��������� �����:
				sosedi[G][i].iNODE1=maxelm+gran[G][i]; // ��������� �� ���������� � ��������� �������,
				sosedi[G][i].bdroblenie4 = false;
				sosedi[G][i].iNODE2 = -1;
				sosedi[G][i].iNODE3 = -1;
				sosedi[G][i].iNODE4 = -1;
				// ����� maxelm ���������� ��.

				// ���������� �������� ������
				switch (G) {
					case ESIDE : GG=EE; break; // E
					case NSIDE : GG=NN; break; // N
					case TSIDE : GG=TTSIDE; break; // T
					case WSIDE: GG=WW; break; // W
					case SSIDE : GG=SS; break; // S
					case BSIDE : GG=BB; break; // B
				}
				sosedi[GG][i].iNODE1=-1; // ������ ���.
				sosedi[GG][i].bdroblenie4 = false;
				sosedi[GG][i].iNODE2 = -1;
				sosedi[GG][i].iNODE3 = -1;
				sosedi[GG][i].iNODE4 = -1;

			}
			else {
				// ������� �������� ���������� ��.
				sosedi[G][i].iNODE1=sosed[G][i]-1;
				sosedi[G][i].bdroblenie4 = false;
				sosedi[G][i].iNODE2 = -1;
				sosedi[G][i].iNODE3 = -1;
				sosedi[G][i].iNODE4 = -1;

				// ���������� �������� ������
				switch (G) {
					case ESIDE : GG=EE; break; // E
					case NSIDE : GG=NN; break; // N
					case TSIDE : GG=TTSIDE; break; // T
					case WSIDE: GG=WW; break; // W
					case SSIDE : GG=SS; break; // S
					case BSIDE : GG=BB; break; // B
				}

				if (gran[G][sosed[G][i]-1]>-1) {
					// ��������� ��
					sosedi[GG][i].iNODE1=maxelm+gran[G][sosed[G][i]-1];
					sosedi[GG][i].bdroblenie4 = false;
					sosedi[GG][i].iNODE2 = -1;
					sosedi[GG][i].iNODE3 = -1;
					sosedi[GG][i].iNODE4 = -1;
				}
				else {
					sosedi[GG][i].iNODE1 = sosed[GG][i] - 1; // ���������� ��.
					sosedi[GG][i].bdroblenie4 = false;
					sosedi[GG][i].iNODE2 = -1;
					sosedi[GG][i].iNODE3 = -1;
					sosedi[GG][i].iNODE4 = -1;
				}

			}
		}
	}

	// sosedi: maxelm -> maxp == maxelm + maxbound;

	// ������� ���� ��� ���� ���������: ����� �� >= maxelm.
	// ������ boundary (������������ ��������� ����) ���������� �������.

} // constr_sosedi

// ������ � ��������� ������� ��� ���������
// �����. ��� ��������� ��� ������������� 
// �������� ������� ����.
void constr_boundary_sosed(integer G, integer i, BOUND* &sosedb, integer** gran, integer** sosed, ALICE_PARTITION **sosedi, integer maxelm) {
	integer j=0; // ������� ����� for

    for (j=0; j<6; j++) sosedb[gran[G][i]].iW[j]=-1; // ������������� 

	// 13 10 2016 �������� ������ ��� �.�. �� ��������� ��������.
	/*
	// ��� �������� ��������� ��� ������������� �������� ������� ����:
	switch (G) {
	case ESIDE: if ((sosed[NSIDE][i]>0) && (gran[G][sosedi[NSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[NSIDE] = maxelm + gran[G][sosedi[NSIDE][i].iNODE1];
		if ((sosed[SSIDE][i]>0) && (gran[G][sosedi[SSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[SSIDE] = maxelm + gran[G][sosedi[SSIDE][i].iNODE1];
		if ((sosed[TSIDE][i]>0) && (gran[G][sosedi[TSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[TSIDE] = maxelm + gran[G][sosedi[TSIDE][i].iNODE1];
		if ((sosed[BSIDE][i]>0) && (gran[G][sosedi[BSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[BSIDE] = maxelm + gran[G][sosedi[BSIDE][i].iNODE1];
			  break;
	case WSIDE: if ((sosed[NSIDE][i]>0) && (gran[G][sosedi[NSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[NSIDE] = maxelm + gran[G][sosedi[NSIDE][i].iNODE1];
		if ((sosed[SSIDE][i]>0) && (gran[G][sosedi[SSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[SSIDE] = maxelm + gran[G][sosedi[SSIDE][i].iNODE1];
		if ((sosed[TSIDE][i]>0) && (gran[G][sosedi[TSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[TSIDE] = maxelm + gran[G][sosedi[TSIDE][i].iNODE1];
		if ((sosed[BSIDE][i]>0) && (gran[G][sosedi[BSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[BSIDE] = maxelm + gran[G][sosedi[BSIDE][i].iNODE1];
			  break;
	case NSIDE: if ((sosed[ESIDE][i]>0) && (gran[G][sosedi[ESIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[ESIDE] = maxelm + gran[G][sosedi[ESIDE][i].iNODE1];
		if ((sosed[WSIDE][i]>0) && (gran[G][sosedi[WSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[WSIDE] = maxelm + gran[G][sosedi[WSIDE][i].iNODE1];
		if ((sosed[TSIDE][i]>0) && (gran[G][sosedi[TSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[TSIDE] = maxelm + gran[G][sosedi[TSIDE][i].iNODE1];
		if ((sosed[BSIDE][i]>0) && (gran[G][sosedi[BSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[BSIDE] = maxelm + gran[G][sosedi[BSIDE][i].iNODE1];
			  break;
	case SSIDE: if ((sosed[ESIDE][i]>0) && (gran[G][sosedi[ESIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[ESIDE] = maxelm + gran[G][sosedi[ESIDE][i].iNODE1];
		if ((sosed[WSIDE][i]>0) && (gran[G][sosedi[WSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[WSIDE] = maxelm + gran[G][sosedi[WSIDE][i].iNODE1];
		if ((sosed[TSIDE][i]>0) && (gran[G][sosedi[TSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[TSIDE] = maxelm + gran[G][sosedi[TSIDE][i].iNODE1];
		if ((sosed[BSIDE][i]>0) && (gran[G][sosedi[BSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[BSIDE] = maxelm + gran[G][sosedi[BSIDE][i].iNODE1];
			  break;
	case TSIDE: if ((sosed[ESIDE][i]>0) && (gran[G][sosedi[ESIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[ESIDE] = maxelm + gran[G][sosedi[ESIDE][i].iNODE1];
		if ((sosed[WSIDE][i]>0) && (gran[G][sosedi[WSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[WSIDE] = maxelm + gran[G][sosedi[WSIDE][i].iNODE1];
		if ((sosed[NSIDE][i]>0) && (gran[G][sosedi[NSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[NSIDE] = maxelm + gran[G][sosedi[NSIDE][i].iNODE1];
		if ((sosed[SSIDE][i]>0) && (gran[G][sosedi[SSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[SSIDE] = maxelm + gran[G][sosedi[SSIDE][i].iNODE1];
		      break;
	case BSIDE : if ((sosed[ESIDE][i]>0) && (gran[G][sosedi[ESIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[ESIDE] = maxelm + gran[G][sosedi[ESIDE][i].iNODE1];
		if ((sosed[WSIDE][i]>0) && (gran[G][sosedi[WSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[WSIDE] = maxelm + gran[G][sosedi[WSIDE][i].iNODE1];
		if ((sosed[NSIDE][i]>0) && (gran[G][sosedi[NSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[NSIDE] = maxelm + gran[G][sosedi[NSIDE][i].iNODE1];
		if ((sosed[SSIDE][i]>0) && (gran[G][sosedi[SSIDE][i].iNODE1]>-1)) sosedb[gran[G][i]].iW[SSIDE] = maxelm + gran[G][sosedi[SSIDE][i].iNODE1];
		      break;
	} // end ���� ���������� ��� ������������� ����.

	*/

} // constr_boundary_sosed

// ���������� ���������� � ��������� �����:
// ����������� sosedb � binternalsource.
void constr_sosedb_temp(BOUND* &sosedb, integer* &whot_is_block, bool* &binternalsource, integer maxelm, integer maxbound, 
	integer** gran, integer** sosed, ALICE_PARTITION **sosedi, integer **nvtx, TOCHKA* pa,
						BLOCK* b, integer lb, integer lw, WALL* w, SOURCE* s, integer ls) {

	// ��������:
    // ��������� ����������� ������:
    // gran[0..5][0..maxbound-1]
	sosedb=NULL;
	sosedb= new BOUND[maxbound];
	if (sosedb==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for sosedb constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	binternalsource=NULL;
    binternalsource = new bool[maxbound];
	if (binternalsource==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for binternalsource constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	//bool bfluidsolid=false;

    // ������ �� ���� ���������� �� ������������� ��������� �������.
	integer i=0,j,jpos; // ������� �����.
	for (i=0; i<maxbound; i++) binternalsource[i]=false; // �������������.
	bool bfind; 
	integer G=-1; // ������� �����.
    doublereal x_c=0.0, y_c=0.0, z_c=0.0; // ���������� ������ �����
	integer iplane; // ��������� � ������� ����� �����.
	TOCHKA p_c;
	integer ib; // ����� �����
	bool bi_fluid; // �� i ����������� ������ ����.

	bool *bvisit=NULL;
	bvisit=new bool[maxbound];
	if (bvisit==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for bvisit constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	for (i=0; i<maxbound; i++) bvisit[i]=false;

	//integer il=0; // debug
    //for (i=0; i<maxelm; i++) for (G=0; G<6; G++) if (gran[G][i]>-1) il++;
    //for (i=0; i<maxelm; i++) for (G=0; G<6; G++) if (sosed[G][i]==0) il++;
#if doubleintprecision == 1
	//printf("il=%lld\n",il); getchar();
#else
	//printf("il=%d\n",il); getchar();
#endif
   

	for (i=0; i<maxelm; i++) {
		// ������ �� ���� ������ ����������� �� � ������ �������.
		for (G=0; G<6; G++) {
			if (gran[G][i]>-1) {
				// ��������� �����
				doublereal dS = 0.0;
                // ������ ���������� ������ ����� � ���������� � ������������
				switch (G) {
					case ESIDE : x_c=pa[nvtx[1][i]-1].x;
						     y_c=0.5*(pa[nvtx[1][i]-1].y+pa[nvtx[3][i]-1].y);
							 z_c=0.5*(pa[nvtx[0][i]-1].z+pa[nvtx[4][i]-1].z);
							 dS = fabs(pa[nvtx[3][i] - 1].y - pa[nvtx[0][i] - 1].y)*fabs(pa[nvtx[4][i] - 1].z - pa[nvtx[0][i] - 1].z);
							 iplane=YZ;
						     break;
					case WSIDE: x_c=pa[nvtx[0][i]-1].x;
						     y_c=0.5*(pa[nvtx[1][i]-1].y+pa[nvtx[3][i]-1].y);
							 z_c=0.5*(pa[nvtx[0][i]-1].z+pa[nvtx[4][i]-1].z);
							 dS = fabs(pa[nvtx[3][i] - 1].y - pa[nvtx[0][i] - 1].y)*fabs(pa[nvtx[4][i] - 1].z - pa[nvtx[0][i] - 1].z);
							 iplane=YZ;
						     break;
					case NSIDE : x_c=0.5*(pa[nvtx[0][i]-1].x+pa[nvtx[1][i]-1].x);
						     y_c=pa[nvtx[2][i]-1].y;
							 z_c=0.5*(pa[nvtx[0][i]-1].z+pa[nvtx[4][i]-1].z);
							 dS = fabs(pa[nvtx[1][i] - 1].x - pa[nvtx[0][i] - 1].x)*fabs(pa[nvtx[4][i] - 1].z - pa[nvtx[0][i] - 1].z);
							 iplane=XZ;
						     break;
					case SSIDE : x_c=0.5*(pa[nvtx[0][i]-1].x+pa[nvtx[1][i]-1].x);
						     y_c=pa[nvtx[0][i]-1].y;
							 z_c=0.5*(pa[nvtx[0][i]-1].z+pa[nvtx[4][i]-1].z);
							 dS = fabs(pa[nvtx[1][i] - 1].x - pa[nvtx[0][i] - 1].x)*fabs(pa[nvtx[4][i] - 1].z - pa[nvtx[0][i] - 1].z);
							 iplane=XZ;
						     break;
					case TSIDE : x_c=0.5*(pa[nvtx[0][i]-1].x+pa[nvtx[1][i]-1].x);
                             y_c=0.5*(pa[nvtx[1][i]-1].y+pa[nvtx[3][i]-1].y);
							 z_c=pa[nvtx[4][i]-1].z;
                             iplane=XY;
							 dS = fabs(pa[nvtx[1][i] - 1].x - pa[nvtx[0][i] - 1].x)*fabs(pa[nvtx[3][i] - 1].y - pa[nvtx[0][i] - 1].y);
						     break;
				    case BSIDE : x_c=0.5*(pa[nvtx[0][i]-1].x+pa[nvtx[1][i]-1].x);
                             y_c=0.5*(pa[nvtx[1][i]-1].y+pa[nvtx[3][i]-1].y);
					    	 z_c=pa[nvtx[0][i]-1].z;
							 dS = fabs(pa[nvtx[1][i] - 1].x - pa[nvtx[0][i] - 1].x)*fabs(pa[nvtx[3][i] - 1].y - pa[nvtx[0][i] - 1].y);
                             iplane=XY;
						     break;
				} // end case

				if (sosed[G][i]==0) { // ����� ����� �� ������� ��������� �������.

					// ������ � ��������� ������� ��� ���������
                    // �����. ��� ��������� ��� ������������� 
                    // �������� ������� ����.
                    constr_boundary_sosed(G,i,sosedb, gran, sosed, sosedi, maxelm);

					

					// ����� ����� �� ������� ��������� �������.
					switch (G) {
						case ESIDE : sosedb[gran[G][i]].Norm=WSIDE;
							     sosedb[gran[G][i]].iII=sosedi[WSIDE][i].iNODE1;
							     break;
						case WSIDE: sosedb[gran[G][i]].Norm=ESIDE;
							sosedb[gran[G][i]].iII = sosedi[ESIDE][i].iNODE1;
							     break;
						case NSIDE : sosedb[gran[G][i]].Norm=SSIDE;
							sosedb[gran[G][i]].iII = sosedi[SSIDE][i].iNODE1;
							     break;
						case SSIDE: sosedb[gran[G][i]].Norm = NSIDE;
							sosedb[gran[G][i]].iII = sosedi[NSIDE][i].iNODE1;
							     break;
						case TSIDE : sosedb[gran[G][i]].Norm=BSIDE; 
							sosedb[gran[G][i]].iII = sosedi[BSIDE][i].iNODE1;
							     break;
						case BSIDE : sosedb[gran[G][i]].Norm=TSIDE; 
							sosedb[gran[G][i]].iII = sosedi[TSIDE][i].iNODE1;
							     break;
					} // end ����������� ���������� �������
                    sosedb[gran[G][i]].iB=maxelm+gran[G][i];
					sosedb[gran[G][i]].iI=i;
					sosedb[gran[G][i]].iI1 = -1;
					sosedb[gran[G][i]].iI2 = -1;
					bvisit[gran[G][i]]=true; // ����� ���� ��������
					sosedb[gran[G][i]].dS = dS;
					// ��������� emissivity:
					integer ibfound = -1;
					ibfound = whot_is_block[i];
					if (ibfound >= 0) {
						// ���� ������.
						// ���������� ���������� �������:
						switch (sosedb[gran[G][i]].Norm) {
						case WSIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissE;
							break;
						case ESIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissW;
							break;
						case SSIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissN;
							break;
						case NSIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissS;
							break;
						case BSIDE : sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissT;
							break;
						case TSIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissB;
							break;
						}
					}
					else {
						printf("error: emissivity calculation block unfound. in constr_sosedb_temp.\n");
						system("PAUSE");
						exit(1);
					}

					bfind=false;
					for (j=0; j<ls; j++) {
						if (s[j].iPlane==iplane) {
							// ����� �� ������ ��������� � ����� ������� �� � ���������� � �������� �� ����� ������:
							switch (iplane) {
								case XY : if ((x_c>s[j].g.xS) && (x_c<s[j].g.xE) && (y_c>s[j].g.yS) && (y_c<s[j].g.yE) && (fabs(z_c-s[j].g.zE)<admission)) { bfind=true; jpos=j; } break;
								case YZ : if ((z_c>s[j].g.zS) && (z_c<s[j].g.zE) && (y_c>s[j].g.yS) && (y_c<s[j].g.yE) && (fabs(x_c-s[j].g.xE)<admission)) { bfind=true; jpos=j; } break;
								case XZ : if ((x_c>s[j].g.xS) && (x_c<s[j].g.xE) && (z_c>s[j].g.zS) && (z_c<s[j].g.zE) && (fabs(y_c-s[j].g.yE)<admission)) { bfind=true; jpos=j; } break;
							}
						}
					}
					if (bfind) {
						// ������� �������� �����
						//printf("source out found...\n"); // debug
						//getchar();
						sosedb[gran[G][i]].MCB=jpos;
					}
					else {
                       for (j=0; j<lw; j++) {
						  if (w[j].iPlane==iplane) {
							  // ����� �� ������ ��������� � ����� ������� �� � ���������� � �������� �� ����� ������:
						      switch (iplane) {
								case XY : if ((x_c>w[j].g.xS) && (x_c<w[j].g.xE) && (y_c>w[j].g.yS) && (y_c<w[j].g.yE) && (fabs(z_c-w[j].g.zE)<admission)) { bfind=true; jpos=j; } break;
								case YZ : if ((z_c>w[j].g.zS) && (z_c<w[j].g.zE) && (y_c>w[j].g.yS) && (y_c<w[j].g.yE) && (fabs(x_c-w[j].g.xE)<admission)) { bfind=true; jpos=j; } break;
								case XZ : if ((x_c>w[j].g.xS) && (x_c<w[j].g.xE) && (z_c>w[j].g.zS) && (z_c<w[j].g.zE) && (fabs(y_c-w[j].g.yE)<admission)) { bfind=true; jpos=j; } break;
							  }
						  }
					   }
					   if (bfind) {
						   // ������� �����
						   //printf("ambient wall found...\n"); // debug
						   //getchar();
                           sosedb[gran[G][i]].MCB=ls+jpos;
					   }
					   else {
						   // ��������� ������� �� ���������
						   sosedb[gran[G][i]].MCB=ls+lw;
					   }

					}

				}
				else 
				{
					// ���������� ����� - ��� ���������� �������� �����.
					if (!bvisit[gran[G][i]]) {
						// ���� ���������� ����� ��� �� ���� ��������.

						// ����� ��������� ��� ��������� ������ ����:
						// ������� ����� ���������� � ������� ������� ����.
						// ���������� ������ ������������ ������
						
						p_c.x=0.5*(pa[nvtx[0][i]-1].x+pa[nvtx[1][i]-1].x);
						p_c.y=0.5*(pa[nvtx[1][i]-1].y+pa[nvtx[3][i]-1].y);
						p_c.z=0.5*(pa[nvtx[0][i]-1].z+pa[nvtx[4][i]-1].z);

						bi_fluid=in_model_flow(p_c,ib,b,lb);

						if (bi_fluid) {
							// �� i ����������� ������ ����

							// ���������� � ������ ��� �� ������� ����������� ������� ����
							// � ��������������� �� ������� �� ������� ��������������� �����.
                            sosedb[gran[G][i]].Norm=G; // ���������� �������
							sosedb[gran[G][i]].iB=maxelm+gran[G][i];
							sosedb[gran[G][i]].iI=sosed[G][i]-1;
							sosedb[gran[G][i]].iI1 = sosed[G][i] - 1;
							sosedb[gran[G][i]].iII = sosedi[G][sosed[G][i] - 1].iNODE1;
							sosedb[gran[G][i]].dS = dS;
							//bfluidsolid=true;							

							// ��������� emissivity:
							integer ibfound = -1;
							ibfound = whot_is_block[sosed[G][i] - 1];
							if (ibfound >= 0) {
								// ���� ������.
								// ���������� ���������� �������:
								switch (sosedb[gran[G][i]].Norm) {
								case WSIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissE;
									break;
								case ESIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissW;
									break;
								case SSIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissN;
									break;
								case NSIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissS;
									break;
								case BSIDE : sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissT;
									break;
								case TSIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissB;
									break;
								}
							}
							else {
								printf("error: emissivity calculation block unfound. in constr_sosedb_temp.\n");
								system("PAUSE");
								exit(1);
							}


						}
						else {
							// � ������� ����� ����� ����� ������������� ��������
							// ����� ������� �������� �������� ��� ������ ����.
							// ���� ��� �� �������. ��� ����� ����� ������� �����
							// �� ���� ������ ��� ���� � ���������� �����������������.

							// �� i ����������� SOLID

							switch (G) {
						       case ESIDE : sosedb[gran[G][i]].Norm=WSIDE;
								   sosedb[gran[G][i]].iII = sosedi[WSIDE][i].iNODE1;
							            break;
						       case WSIDE: sosedb[gran[G][i]].Norm=ESIDE;
								   sosedb[gran[G][i]].iII = sosedi[ESIDE][i].iNODE1;
							            break;
						       case NSIDE : sosedb[gran[G][i]].Norm=SSIDE;
								   sosedb[gran[G][i]].iII = sosedi[SSIDE][i].iNODE1;
							            break;
						       case SSIDE : sosedb[gran[G][i]].Norm=NSIDE;
								   sosedb[gran[G][i]].iII = sosedi[NSIDE][i].iNODE1;
							            break;
						       case TSIDE : sosedb[gran[G][i]].Norm=BSIDE; 
								   sosedb[gran[G][i]].iII = sosedi[BSIDE][i].iNODE1;
							            break;
						       case BSIDE : sosedb[gran[G][i]].Norm=TSIDE; 
								   sosedb[gran[G][i]].iII = sosedi[TSIDE][i].iNODE1;
							            break;
					         } // end ����������� ���������� �������
                            sosedb[gran[G][i]].iB=maxelm+gran[G][i];
					        sosedb[gran[G][i]].iI=i;
							sosedb[gran[G][i]].iI1 = i;
							sosedb[gran[G][i]].dS = dS;
							//bfluidsolid=true;
							// ����� ��������������� ��� ��� ���� ���������� ������
							// ����� �������� ����� �������� ���������� ����� SOLID � FLUID.

							// ��������� emissivity:
							integer ibfound = -1;
							ibfound = whot_is_block[i];
							if (ibfound >= 0) {
								// ���� ������.
								// ���������� ���������� �������:
								switch (sosedb[gran[G][i]].Norm) {
								case WSIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissE;
									break;
								case ESIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissW;
									break;
								case SSIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissN;
									break;
								case NSIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissS;
									break;
								case BSIDE : sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissT;
									break;
								case TSIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissB;
									break;
								}
							}
							else {
								printf("error: emissivity calculation block unfound. in constr_sosedb_temp.\n");
								system("PAUSE");
								exit(1);
							}

						}

						// ������ � ��������� ������� ��� ���������
                        // �����. ��� ��������� ��� ������������� 
                        // �������� ������� ����.
                        constr_boundary_sosed(G,i,sosedb, gran, sosed, sosedi, maxelm);

						// ����� ���������� MCB:
                        bfind=false;
					    for (j=0; j<ls; j++) {
						   if (s[j].iPlane==iplane) {
							  switch (iplane) {
									case XY : if ((x_c>s[j].g.xS) && (x_c<s[j].g.xE) && (y_c>s[j].g.yS) && (y_c<s[j].g.yE) && (fabs(z_c-s[j].g.zE)<admission)) { bfind=true; jpos=j; } break;
								    case YZ : if ((z_c>s[j].g.zS) && (z_c<s[j].g.zE) && (y_c>s[j].g.yS) && (y_c<s[j].g.yE) && (fabs(x_c-s[j].g.xE)<admission)) { bfind=true; jpos=j; } break;
								    case XZ : if ((x_c>s[j].g.xS) && (x_c<s[j].g.xE) && (z_c>s[j].g.zS) && (z_c<s[j].g.zE) && (fabs(y_c-s[j].g.yE)<admission)) { bfind=true; jpos=j; } break;
							  }
						   }
					    }
						
					    if (bfind) {
						    // ��������� �������� �����
							//printf("internal source found...\n"); // debug
							//getchar();
						    sosedb[gran[G][i]].MCB=jpos;
							binternalsource[gran[G][i]]=true; // ���������� �������� ����� �� ������� �������� � ������� ����
						}
						else {
							// ������ ���������� �������� ����� �� ���������!
							printf("Error !!! internal source not found...\n");
							//getchar();
							system("pause");
							exit(0);
							sosedb[gran[G][i]].MCB=ls+lw;
						}

					}
					else
					{
						p_c.x = 0.5*(pa[nvtx[0][i] - 1].x + pa[nvtx[1][i] - 1].x);
						p_c.y = 0.5*(pa[nvtx[1][i] - 1].y + pa[nvtx[3][i] - 1].y);
						p_c.z = 0.5*(pa[nvtx[0][i] - 1].z + pa[nvtx[4][i] - 1].z);

						bi_fluid = in_model_flow(p_c, ib, b, lb);

						if (bi_fluid) {
							// �� i ����������� ������ ����

							// ���������� � ������ ��� �� ������� ����������� ������� ����
							// � ��������������� �� ������� �� ������� ��������������� �����.
							
							sosedb[gran[G][i]].iI2 = sosed[G][i] - 1;
							

						}
						else {
							sosedb[gran[G][i]].iI2 = i;
						}
					}

					bvisit[gran[G][i]]=true; // ����� ���� ��������
				}
			}
		}
	}

	// ������������ ����������� ������
	delete[] bvisit; 
	bvisit=NULL;

	// sosedb : maxbound -> maxp == maxelm + maxbound;

} // constr_sosedb_temp 

// ���������� ���������� � ��������� �����:
void constr_sosedb_flow(BOUND* &sosedb,  integer* &whot_is_block, integer* ptr, integer maxelm, integer maxbound,
	                     integer** gran, integer** sosed, ALICE_PARTITION **sosedi, integer **nvtx, TOCHKA* pa,
						integer lw, WALL* w, integer ls, BLOCK* b) {

	// ��������:
    // ��������� ����������� ������:
    // gran[0..5][0..maxbound-1]
	sosedb=NULL;
	sosedb= new BOUND[maxbound];
	if (sosedb==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for sosedb constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}

    // ������ �� ���� ���������� �� ������������� ��������� �������.
	integer i=0,j=0,jpos=0; // ������� �����.
	bool bfind; 
	integer G=-1; // ������� �����.
    doublereal x_c=0.0, y_c=0.0, z_c=0.0; // ���������� ������ �����
	integer iplane; // ��������� � ������� ����� �����.


	for (i=0; i<maxelm; i++) {
		// ������ �� ���� ������ ����������� �� � ������ �������.
		for (G=0; G<6; G++) {
			if (gran[G][i]>-1) {
				// ��������� �����
				
				doublereal dS = 0.0;
                // ������ ���������� ������ ����� � ���������� � ������������
				switch (G) {
					case ESIDE : x_c=pa[nvtx[1][i]-1].x;
						     y_c=0.5*(pa[nvtx[1][i]-1].y+pa[nvtx[3][i]-1].y);
							 z_c=0.5*(pa[nvtx[0][i]-1].z+pa[nvtx[4][i]-1].z);
							 dS = fabs(pa[nvtx[3][i] - 1].y - pa[nvtx[0][i] - 1].y)*fabs(pa[nvtx[4][i] - 1].z - pa[nvtx[0][i] - 1].z);
							 iplane=YZ;
						     break;
					case WSIDE: x_c=pa[nvtx[0][i]-1].x;
						     y_c=0.5*(pa[nvtx[1][i]-1].y+pa[nvtx[3][i]-1].y);
							 z_c=0.5*(pa[nvtx[0][i]-1].z+pa[nvtx[4][i]-1].z);
							 dS = fabs(pa[nvtx[3][i] - 1].y - pa[nvtx[0][i] - 1].y)*fabs(pa[nvtx[4][i] - 1].z - pa[nvtx[0][i] - 1].z);
							 iplane=YZ;
						     break;
					case NSIDE : x_c=0.5*(pa[nvtx[0][i]-1].x+pa[nvtx[1][i]-1].x);
						     y_c=pa[nvtx[2][i]-1].y;
							 z_c=0.5*(pa[nvtx[0][i]-1].z+pa[nvtx[4][i]-1].z);
							 dS = fabs(pa[nvtx[1][i] - 1].x - pa[nvtx[0][i] - 1].x)*fabs(pa[nvtx[4][i] - 1].z - pa[nvtx[0][i] - 1].z);
							 iplane=XZ;
						     break;
					case SSIDE : x_c=0.5*(pa[nvtx[0][i]-1].x+pa[nvtx[1][i]-1].x);
						     y_c=pa[nvtx[0][i]-1].y;
							 z_c=0.5*(pa[nvtx[0][i]-1].z+pa[nvtx[4][i]-1].z);
							 dS = fabs(pa[nvtx[1][i] - 1].x - pa[nvtx[0][i] - 1].x)*fabs(pa[nvtx[4][i] - 1].z - pa[nvtx[0][i] - 1].z);
							 iplane=XZ;
						     break;
					case TSIDE : x_c=0.5*(pa[nvtx[0][i]-1].x+pa[nvtx[1][i]-1].x);
                             y_c=0.5*(pa[nvtx[1][i]-1].y+pa[nvtx[3][i]-1].y);
							 z_c=pa[nvtx[4][i]-1].z;
							 dS = fabs(pa[nvtx[1][i] - 1].x - pa[nvtx[0][i] - 1].x)*fabs(pa[nvtx[3][i] - 1].y - pa[nvtx[0][i] - 1].y);
                             iplane=XY;
						     break;
				    case BSIDE : x_c=0.5*(pa[nvtx[0][i]-1].x+pa[nvtx[1][i]-1].x);
                             y_c=0.5*(pa[nvtx[1][i]-1].y+pa[nvtx[3][i]-1].y);
					    	 z_c=pa[nvtx[0][i]-1].z;
							 dS = fabs(pa[nvtx[1][i] - 1].x - pa[nvtx[0][i] - 1].x)*fabs(pa[nvtx[3][i] - 1].y - pa[nvtx[0][i] - 1].y);
                             iplane=XY;
						     break;
				} // end case

				if (sosed[G][i]==0) {

                    // ������ � ��������� ������� ��� ���������
                    // �����. ��� ��������� ��� ������������� 
                    // �������� ������� ����.
                    constr_boundary_sosed(G,i,sosedb, gran, sosed, sosedi, maxelm);


					// ����� ����� �� ������� ��������� �������.
					switch (G) {
						case ESIDE : sosedb[gran[G][i]].Norm=WSIDE;
							     sosedb[gran[G][i]].iII=sosedi[WSIDE][i].iNODE1;
							     break;
						case WSIDE: sosedb[gran[G][i]].Norm=ESIDE;
							sosedb[gran[G][i]].iII = sosedi[ESIDE][i].iNODE1;
							     break;
						case NSIDE : sosedb[gran[G][i]].Norm=SSIDE;
							sosedb[gran[G][i]].iII = sosedi[SSIDE][i].iNODE1;
							     break;
						case SSIDE : sosedb[gran[G][i]].Norm=NSIDE;
							sosedb[gran[G][i]].iII = sosedi[NSIDE][i].iNODE1;
							     break;
						case TSIDE : sosedb[gran[G][i]].Norm=BSIDE; 
							sosedb[gran[G][i]].iII = sosedi[BSIDE][i].iNODE1;
							     break;
						case BSIDE : sosedb[gran[G][i]].Norm=TSIDE; 
							sosedb[gran[G][i]].iII = sosedi[TSIDE][i].iNODE1;
							     break;
					} // end ����������� ���������� �������
                    sosedb[gran[G][i]].iB=maxelm+gran[G][i];
					sosedb[gran[G][i]].iI=i;
					sosedb[gran[G][i]].dS = dS; // ������� �����.

					// ��������� emissivity:
					integer ibfound = -1;
					ibfound = whot_is_block[ptr[i]];
					if (ibfound >= 0) {
						// ���� ������.
						// ���������� ���������� �������:
						switch (sosedb[gran[G][i]].Norm) {
						case WSIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissE;
							break;
						case ESIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissW;
							break;
						case SSIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissN;
							break;
						case NSIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissS;
							break;
						case BSIDE : sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissT;
							break;
						case TSIDE: sosedb[gran[G][i]].emissivity = b[ibfound].radiation.emissB;
							break;
						}
					}
					else {
						printf("error: emissivity calculation block unfound. in constr_sosedb_temp.\n");
						system("PAUSE");
						exit(1);
					}
					

					bfind=false;
					for (j=0; j<lw; j++) {
						if (w[j].iPlane==iplane) {
							switch (iplane) {
								case XY : if ((x_c>w[j].g.xS) && (x_c<w[j].g.xE) && (y_c>w[j].g.yS) && (y_c<w[j].g.yE) && (fabs(z_c-w[j].g.zE)<admission)) 
										  {
											  //printf("z_c=%f, zE=%f\n",z_c, w[j].g.zE);
											  //getchar(); // debug
											  bfind=true; jpos=j;
										  }
										  break;
								case YZ : if ((z_c>w[j].g.zS) && (z_c<w[j].g.zE) && (y_c>w[j].g.yS) && (y_c<w[j].g.yE) && (fabs(x_c-w[j].g.xE)<admission)) { bfind=true; jpos=j; } break;
								case XZ : if ((x_c>w[j].g.xS) && (x_c<w[j].g.xE) && (z_c>w[j].g.zS) && (z_c<w[j].g.zE) && (fabs(y_c-w[j].g.yE)<admission)) { bfind=true; jpos=j; } break;
							}
						}
					}
					if (bfind) {
						// ������� �������� �����
						sosedb[gran[G][i]].MCB=ls+jpos;
#if doubleintprecision == 1
						//printf("MCB=%lld\n",sosedb[gran[G][i]].MCB);
#else
						//printf("MCB=%d\n",sosedb[gran[G][i]].MCB);
#endif
						
						// getchar(); // debug
					}
					else {
                         // ��������� ������� �� ���������
					     sosedb[gran[G][i]].MCB=ls+lw;
						 //printf("default\n");
						 //getchar(); // debug
					}
				} 
			}
		}
	}

	
	// sosedb : maxbound -> maxp == maxelm + maxbound;

} // constr_sosedb_flow

// �������� ��������� �� ������� ������������� �������.
void constr_prop_bound(doublereal **prop, doublereal** &prop_b, integer maxelm, integer maxbound,
					   integer **gran, integer **sosed, integer **nvtx, TOCHKA* pa,
					   BLOCK* b, integer lb) {
	// ��������� ����������� ������:
	prop_b=NULL;
	prop_b=new doublereal*[6];
	if (prop_b==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for prop_b constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	integer i=0; // �������
	for (i=0; i<6; i++) prop_b[i]=NULL;
	for (i=0; i<6; i++) {
		prop_b[i]=new doublereal[maxbound];
		if (prop_b[i]==NULL) {
	        // ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
			printf("Problem : not enough memory on your equipment for prop_b[%lld] constr struct...\n", i);
#else
			printf("Problem : not enough memory on your equipment for prop_b[%d] constr struct...\n", i);
#endif
		   
		    printf("Please any key to exit...\n");
			//getchar();
			system("pause");
		    exit(1);
	    }
	}
	integer G; // ������� �����
	bool *bvisit=NULL;
	bvisit=new bool[maxbound];
	if (bvisit==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for bvisit constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	for (i=0; i<maxbound; i++) bvisit[i]=false; // ������� ������������ �����
	TOCHKA p_c;
	integer ib;
	bool bi_fluid;


    // � ����� �� ���� ���������� �� ������������� ��������� �������:
	for (i=0; i<maxelm; i++) {
		// ������ �� ���� ������ ����������� �� � ������ �������.
		for (G=0; G<6; G++) {
			if (gran[G][i]>-1) {
				// ��������� �����
				if (sosed[G][i]==0) {
					// ����� ����� �� ������� ��������� �������

					// � ��������� ���� �������� �������� ������������ ��:
					prop_b[RHO][gran[G][i]]=prop[RHO][i];
					prop_b[CP][gran[G][i]]=prop[CP][i];
					prop_b[LAM][gran[G][i]]=prop[LAM][i];
					prop_b[MULT_LAM_X][gran[G][i]] = prop[MULT_LAM_X][i];
					prop_b[MULT_LAM_Y][gran[G][i]] = prop[MULT_LAM_Y][i];
					prop_b[MULT_LAM_Z][gran[G][i]] = prop[MULT_LAM_Z][i];
					bvisit[gran[G][i]]=true;
				}
				else
				{
					// ���������� �����
					// ��� ������ ������������ �������� ���������� ������� ��������� �����.
					// ���� ��� ����. ������ ��������� ����� �������� ������� ����. ������ 
					// ��������� ������������������� ������������� � ��������.
					// ������� ������ ����.

					if (!bvisit[gran[G][i]]) {

						// ����� ��������� ��� ��������� ������ ����:
						// ������� ����� ���������� � ������� ������� ����.
						p_c.x=0.5*(pa[nvtx[0][i]-1].x+pa[nvtx[1][i]-1].x);
						p_c.y=0.5*(pa[nvtx[1][i]-1].y+pa[nvtx[3][i]-1].y);
						p_c.z=0.5*(pa[nvtx[0][i]-1].z+pa[nvtx[4][i]-1].z);

						bi_fluid=in_model_flow(p_c,ib,b,lb);

						if (bi_fluid) {
                            prop_b[RHO][gran[G][i]]=prop[RHO][sosed[G][i]-1];
					        prop_b[CP][gran[G][i]]=prop[CP][sosed[G][i]-1];
					        prop_b[LAM][gran[G][i]]=prop[LAM][sosed[G][i]-1];
							prop_b[MULT_LAM_X][gran[G][i]] = prop[MULT_LAM_X][sosed[G][i] - 1];
							prop_b[MULT_LAM_Y][gran[G][i]] = prop[MULT_LAM_Y][sosed[G][i] - 1];
							prop_b[MULT_LAM_Z][gran[G][i]] = prop[MULT_LAM_Z][sosed[G][i] - 1];
						}
						else {
                            prop_b[RHO][gran[G][i]]=prop[RHO][i];
					        prop_b[CP][gran[G][i]]=prop[CP][i];
					        prop_b[LAM][gran[G][i]]=prop[LAM][i];
							prop_b[MULT_LAM_X][gran[G][i]] = prop[MULT_LAM_X][i];
							prop_b[MULT_LAM_Y][gran[G][i]] = prop[MULT_LAM_Y][i];
							prop_b[MULT_LAM_Z][gran[G][i]] = prop[MULT_LAM_Z][i];
						}

					}

                    bvisit[gran[G][i]]=true;
				}
			}
		}
	}

	delete[] bvisit;
	bvisit=NULL;

} // constr_prop_bound 

// �������� ��������� �� ������� ������ �������.
void constr_prop_bound_flow(doublereal **prop, doublereal** &prop_b, integer maxelm, integer maxbound,
					   integer **gran, integer **sosed) {
	// ��������� ����������� ������:
	prop_b=NULL;
	prop_b=new doublereal*[3];
	if (prop_b==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for prop_b constr struct...\n");
		printf("Please any key to exit...\n");
	//	getchar();
		system("pause");
		exit(1);
	}
	integer i=0; // �������
	for (i=0; i<3; i++) prop_b[i]=NULL;
	for (i=0; i<3; i++) {
		prop_b[i]=new doublereal[maxbound];
		if (prop_b[i]==NULL) {
	        // ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
			printf("Problem : not enough memory on your equipment for prop_b[%lld] constr struct...\n", i);
#else
			printf("Problem : not enough memory on your equipment for prop_b[%d] constr struct...\n", i);
#endif
		   
		    printf("Please any key to exit...\n");
			//getchar();
			system("pause");
		    exit(1);
	    }
	}
	integer G; // ������� �����
	


    // � ����� �� ���� ���������� �� ������������� ��������� �������:
	for (i=0; i<maxelm; i++) {
		// ������ �� ���� ������ ����������� �� � ������ �������.
		for (G=0; G<6; G++) {
			if (gran[G][i]>-1) {
				// ��������� �����
				if (sosed[G][i]==0) {
					// ����� ����� �� ������� ��������� �������

					// � ��������� ���� �������� �������� ������������ ��:
					prop_b[RHO][gran[G][i]]=prop[RHO][i];
					prop_b[MU][gran[G][i]]=prop[MU][i];
					prop_b[BETA_T][gran[G][i]]=prop[BETA_T][i];

				}
				
			}
		}
	}

} // constr_prop_bound_flow

// ����� �� ����������� �������� � ����� �����.
// �� ��������� ������������ ��������� ��������� �����.
// ������ ������� ������������ � �������� ����� � �� ���� �����.
// 24 �������� 2016.
void determination_of_activity_flow(BOUND* sosedb, integer maxbound, 
									integer ls, integer lw, WALL* w, 
									bool &bactiv, bool &bpressure_fix, bool &bLR1free) {
	integer i=0; // ������� ����� for
	bool bpfix=true; // �� ��������� �������� ����� ����������� � ����� �����

	// 10.02.2017
	//bpfix = false; ��� ������ ������ �.�. ������� ����������.
	
	for (i=0; i<maxbound; i++) {
		if ((sosedb[i].MCB>=ls) && (sosedb[i].MCB<(ls+lw))) {
			if (w[sosedb[i].MCB - ls].bpressure || w[sosedb[i].MCB - ls].bopening) {
				// �������� ����������� �������
				bpfix=false;
				break; // ��������� ����� �� ����� for
			}
		}
	}

	bLR1free=bpfix; // ��������� ��������� ������������ ������ ��� �������� ��������, �.�. �� ���������� ����� ������� ����������.
	//bLR1free=false; // ->
	
	//->//bpfix=false; // ���� �������� �� ����������� � ����� ����� �� �������� SIMPLE ������� ����� ������� ����������.
    //->//bactiv=!bpfix; // �� ��� ������ � ��� ������ ���� �� ���� ������� ��������� ������� ������ ������� ��������.
	bactiv=true;
	/*
	bool bnonzeromag=false;

	// ���� �� ������� ������� �������� ��������� �� ����� ��������� ���������� ��������� �������:
	// ��� ��� ������� � ������� ��� ���� ���� ������.
    for (i=0; i<maxbound; i++) {
		if ((sosedb[i].MCB>=ls) && (sosedb[i].MCB<(ls+lw))) {
            if (!w[sosedb[i].MCB-ls].bpressure) {
				if ((fabs(w[sosedb[i].MCB-ls].Vx)>admission) || (fabs(w[sosedb[i].MCB-ls].Vy)>admission) || (fabs(w[sosedb[i].MCB-ls].Vz)>admission)) bnonzeromag=true;
			}
		}
	}
	if (!bactiv) bactiv=bnonzeromag;
	*/

    bpressure_fix=bpfix;

} // determination_of_activity_flow

// ��������� ����������� ������ ��� ������ ����������������.
void allocation_memory_temp(doublereal* &potent, doublereal** &total_deformation, equation3D* &sl, equation3D_bon* &slb, BOUND* sosedb, integer maxelm, integer maxbound, integer ls, integer lw, WALL* w) {
    // ��������� ������ ��� ������� ������� ��������.
	potent=NULL;
    potent=new doublereal[maxelm+maxbound];
	if (potent==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for potent constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}

	total_deformation = NULL;
	total_deformation = new doublereal*[4];	
	for (integer i_1 = 0; i_1 < 4; i_1++) {
		total_deformation[i_1] = NULL;
	}
	if (total_deformation == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for total_deformation constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	for (integer i_1 = 0; i_1 < 4; i_1++) {
		total_deformation[i_1] = new doublereal[maxelm + maxbound];
		if (total_deformation[i_1] == NULL) {
			// ������������ ������ �� ������ ������������.
			printf("Problem : not enough memory on your equipment for total_deformation[%d] constr struct...\n",i_1);
			printf("Please any key to exit...\n");
			//getchar();
			system("pause");
			exit(1);
		}
	}

    doublereal Tamb=0.0;
    bool bTamb=false;
       // �������� ������ ���� ���������������� � ������ ��������� �������.
       if (lw>0) {
          Tamb=+1.0e+20;
	      for (integer i=0; i<maxbound; i++) {
		      if (((sosedb[i].MCB<(ls+lw))&&(sosedb[i].MCB>=ls)) && (w[sosedb[i].MCB-ls].ifamily==1)) {
			      // �� ������� ������ �����������
			      Tamb=fmin(Tamb,w[sosedb[i].MCB-ls].Tamb);
                  bTamb=true;
		      }
	      }
	   }

	   // �������� ������������� ���� !!!.
       if (!bTamb) Tamb=0.0;

	// ���������
	integer j=0;
	for (j = 0; j < (maxelm + maxbound); j++) {
		potent[j] = Tamb; // ���� �������� ������� ��������� �����������.
		for (integer i_1 = 0; i_1 < 4; i_1++) {
			total_deformation[i_1][j] = 0.0; // ������� ������ ����������
		}
	}

	sl=NULL;
	sl = new equation3D[maxelm]; // ������������ ������� ���� ��� ���������� ��.
	if (sl==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for slau temperature constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}

	slb=NULL;
	slb = new equation3D_bon[maxbound]; // ������������ ������� ���� ��� ��������� ��
	if (slb==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for slau boundary temperature constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
} // allocation_memory_temp

  // ���������� ����������������� �������� �����.
  // ���������������� �������� ����� mf ����������� �� ������ �������������
  // ����������������� �������� ��� ������ �������� �� ������ ���������� �������������.
  // ��� ����� ��� ���������� ������� ��������� ���������-�������� ��� ������ ���������������� ��������,
  // ������� �������� ���-��� ������ �������������.
  // 26.03.2017 15.09.2018 ���������� ��� ������������� ��� �����. ���������� � ����� pamendment3.c.
void return_calc_correct_mass_flux_only_interpolation(integer iP, doublereal** potent, TOCHKA* pa, doublereal** prop, doublereal** prop_b,
	integer** nvtx, ALICE_PARTITION** sosedi, integer maxelm,
	doublereal* &mfcurrentretune);

// ��������� ����������� ������ ��� ������ �������������.
void allocation_memory_flow( doublereal** &potent, equation3D** &sl, equation3D_bon** &slb,
							 BOUND* sosedb, integer maxelm, integer maxbound,
							 doublereal* &alpha, integer ls, integer lw, WALL* w, 
							 doublereal dgx, doublereal dgy, doublereal dgz, integer** nvtx, TOCHKA* pa, doublereal** prop, ALICE_PARTITION** sosedi) {

	// ��������� ������ ��� ������� ������� ��������.
	potent=NULL;
    potent=new doublereal*[27];
	if (potent==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for potent constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	for (integer i=0; i<27; i++) potent[i]=NULL;
	for (integer i=0; i<27; i++) {
		potent[i]=new doublereal[maxelm+maxbound];
		if (potent[i]==NULL) {
	        // ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
			printf("Problem : not enough memory on your equipment for potent[%lld] constr struct...\n", i);
#else
			printf("Problem : not enough memory on your equipment for potent[%d] constr struct...\n", i);
#endif
		    
		    printf("Please any key to exit...\n");
			//getchar();
			system("pause");
		    exit(1);
	    }
	}

	// ���������
    for (integer i=0; i<27; i++) for (integer j=0; j<(maxelm+maxbound); j++) potent[i][j]=0.0;

	// ������������ ������������ �������� ��� ������������ ��������� 
	// �� ����������� �� ��������� �����-������ � ������ ����������� ��� 
	// �������������, ���� ��� ������ �� �������� �������� � �����-������.
	TOCHKA p;
	doublereal minX1 = 1.0e20;
	doublereal minY1 = 1.0e20;
	doublereal minZ1 = 1.0e20;
	minX1 = 0.0;
	minY1 = 0.0;
	minZ1 = 0.0;
	for(integer iP = 0; iP < maxelm; iP++) {
		center_cord3D(iP, nvtx, pa, p,100);
		// ���������� �������� �������� ������������ ������:
		doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
		volume3D(iP, nvtx, pa, dx, dy, dz);
		//if (p.x < minX1) minX1 = p.x-0.5*dx;
		//if (p.y < minY1) minY1 = p.y-0.5*dy;
		//if (p.z < minZ1) minZ1 = p.z-0.5*dz;
		minX1 += p.x;
		minY1 += p.y;
		minZ1 += p.z;
	}
	minX1 /= maxelm;
	minY1 /= maxelm;
	minZ1 /= maxelm;
	for (integer iP = 0; iP < maxelm; iP++) {
		center_cord3D(iP, nvtx, pa, p,100);
		// ���������� �������� �������� ������������ ������:
		doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
		volume3D(iP, nvtx, pa, dx, dy, dz);
		potent[PRESS][iP] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y - minY1) + dgz*(p.z - minZ1));
		integer iE1, iN1, iT1, iW1, iS1, iB1; // ������ �������� ����������� �������
		integer iE2, iN2, iT2, iW2, iS2, iB2;
		integer iE3, iN3, iT3, iW3, iS3, iB3;
		integer iE4, iN4, iT4, iW4, iS4, iB4;

		iE1 = sosedi[ESIDE][iP].iNODE1; iE2 = sosedi[ESIDE][iP].iNODE2; iE3 = sosedi[ESIDE][iP].iNODE3; iE4 = sosedi[ESIDE][iP].iNODE4;
		iN1 = sosedi[NSIDE][iP].iNODE1; iN2 = sosedi[NSIDE][iP].iNODE2; iN3 = sosedi[NSIDE][iP].iNODE3; iN4 = sosedi[NSIDE][iP].iNODE4;
		iT1 = sosedi[TSIDE][iP].iNODE1; iT2 = sosedi[TSIDE][iP].iNODE2; iT3 = sosedi[TSIDE][iP].iNODE3; iT4 = sosedi[TSIDE][iP].iNODE4;
		iW1 = sosedi[WSIDE][iP].iNODE1; iW2 = sosedi[WSIDE][iP].iNODE2; iW3 = sosedi[WSIDE][iP].iNODE3; iW4 = sosedi[WSIDE][iP].iNODE4;
		iS1 = sosedi[SSIDE][iP].iNODE1; iS2 = sosedi[SSIDE][iP].iNODE2; iS3 = sosedi[SSIDE][iP].iNODE3; iS4 = sosedi[SSIDE][iP].iNODE4;
		iB1 = sosedi[BSIDE][iP].iNODE1; iB2 = sosedi[BSIDE][iP].iNODE2; iB3 = sosedi[BSIDE][iP].iNODE3; iB4 = sosedi[BSIDE][iP].iNODE4;

		/*
		// ��� ������ �� � ���� ������ ������ �.�. ��������� ������������ �������� �� ������� ��������� �������.
		if (0) {
			if (iE >= maxelm) {
				potent[PRESS][iE] += prop[RHO][iP] * (dgx*(p.x + 0.5*dx - minX1) + dgy*(p.y - minY1) + dgz*(p.z - minZ1));
			}
			if (iW >= maxelm) {
				potent[PRESS][iW] += prop[RHO][iP] * (dgx*(p.x - 0.5*dx - minX1) + dgy*(p.y - minY1) + dgz*(p.z - minZ1));
			}
			if (iN >= maxelm) {
				potent[PRESS][iN] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y + 0.5*dy - minY1) + dgz*(p.z - minZ1));
			}
			if (iS >= maxelm) {
				potent[PRESS][iS] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y - 0.5*dy - minY1) + dgz*(p.z - minZ1));
			}
			if (iT >= maxelm) {
				potent[PRESS][iT] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y - minY1) + dgz*(p.z + 0.5*dz - minZ1));
			}
			if (iB >= maxelm) {
				potent[PRESS][iB] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y - minY1) + dgz*(p.z - 0.5*dz - minZ1));
			}
		}
		else */ 
		{
			if (iE1 >= maxelm) {
				potent[PRESS][iE1] += prop[RHO][iP] * (dgx*(p.x + 0.5*dx - minX1) + dgy*(p.y - minY1) + dgz*(p.z - minZ1));
			}
			if (iW1 >= maxelm) {
				potent[PRESS][iW1] += prop[RHO][iP] * (dgx*(p.x - 0.5*dx - minX1) + dgy*(p.y - minY1) + dgz*(p.z - minZ1));
			}
			if (iN1 >= maxelm) {
				potent[PRESS][iN1] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y + 0.5*dy - minY1) + dgz*(p.z - minZ1));
			}
			if (iS1 >= maxelm) {
				potent[PRESS][iS1] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y - 0.5*dy - minY1) + dgz*(p.z - minZ1));
			}
			if (iT1 >= maxelm) {
				potent[PRESS][iT1] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y - minY1) + dgz*(p.z + 0.5*dz - minZ1));
			}
			if (iB1 >= maxelm) {
				potent[PRESS][iB1] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y - minY1) + dgz*(p.z - 0.5*dz - minZ1));
			}

			if (iE2 >= maxelm) {
				potent[PRESS][iE2] += prop[RHO][iP] * (dgx*(p.x + 0.5*dx - minX1) + dgy*(p.y - minY1) + dgz*(p.z - minZ1));
			}
			if (iW2 >= maxelm) {
				potent[PRESS][iW2] += prop[RHO][iP] * (dgx*(p.x - 0.5*dx - minX1) + dgy*(p.y - minY1) + dgz*(p.z - minZ1));
			}
			if (iN2 >= maxelm) {
				potent[PRESS][iN2] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y + 0.5*dy - minY1) + dgz*(p.z - minZ1));
			}
			if (iS2 >= maxelm) {
				potent[PRESS][iS2] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y - 0.5*dy - minY1) + dgz*(p.z - minZ1));
			}
			if (iT2 >= maxelm) {
				potent[PRESS][iT2] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y - minY1) + dgz*(p.z + 0.5*dz - minZ1));
			}
			if (iB2 >= maxelm) {
				potent[PRESS][iB2] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y - minY1) + dgz*(p.z - 0.5*dz - minZ1));
			}

			if (iE3 >= maxelm) {
				potent[PRESS][iE3] += prop[RHO][iP] * (dgx*(p.x + 0.5*dx - minX1) + dgy*(p.y - minY1) + dgz*(p.z - minZ1));
			}
			if (iW3 >= maxelm) {
				potent[PRESS][iW3] += prop[RHO][iP] * (dgx*(p.x - 0.5*dx - minX1) + dgy*(p.y - minY1) + dgz*(p.z - minZ1));
			}
			if (iN3 >= maxelm) {
				potent[PRESS][iN3] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y + 0.5*dy - minY1) + dgz*(p.z - minZ1));
			}
			if (iS3 >= maxelm) {
				potent[PRESS][iS3] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y - 0.5*dy - minY1) + dgz*(p.z - minZ1));
			}
			if (iT3 >= maxelm) {
				potent[PRESS][iT3] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y - minY1) + dgz*(p.z + 0.5*dz - minZ1));
			}
			if (iB3 >= maxelm) {
				potent[PRESS][iB3] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y - minY1) + dgz*(p.z - 0.5*dz - minZ1));
			}

			if (iE4 >= maxelm) {
				potent[PRESS][iE4] += prop[RHO][iP] * (dgx*(p.x + 0.5*dx  - minX1) + dgy*(p.y - minY1) + dgz*(p.z - minZ1));
			}
			if (iW4 >= maxelm) {
				potent[PRESS][iW4] += prop[RHO][iP] * (dgx*(p.x - 0.5*dx - minX1) + dgy*(p.y - minY1) + dgz*(p.z - minZ1));
			}
			if (iN4 >= maxelm) {
				potent[PRESS][iN4] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y + 0.5*dy - minY1) + dgz*(p.z - minZ1));
			}
			if (iS4 >= maxelm) {
				potent[PRESS][iS4] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y - 0.5*dy - minY1) + dgz*(p.z - minZ1));
			}
			if (iT4 >= maxelm) {
				potent[PRESS][iT4] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y - minY1) + dgz*(p.z + 0.5*dz - minZ1));
			}
			if (iB4 >= maxelm) {
				potent[PRESS][iB4] += prop[RHO][iP] * (dgx*(p.x - minX1) + dgy*(p.y - minY1) + dgz*(p.z - 0.5*dz - minZ1));
			}
		}
	}
	
	// 10.02.2017
	for (integer iP = 0; iP < maxelm+maxbound; iP++) {
		//potent[PRESS][iP] *= -1.0;
		// ANSYS Icepak �� ��������� ���������������� ������� ��������.
		// ��� �������� ���� �������� ����� ����������� ���������.
		potent[PRESS][iP] = 0.0;
	}


	/*
	// �������� ������ ���� ���������������� � ������ ��������� �������.
	for (i=0; i<maxbound; i++) {
		if (((sosedb[i].MCB<(ls+lw))&&(sosedb[i].MCB>=ls)) && (w[sosedb[i].MCB-ls].bpressure)) {
			// �� ������� ������ ��������
			potent[PRESS][sosedb[i].iB]=w[sosedb[i].MCB-ls].P;
		}
	}
	*/


	// �������� ������������� ��������� ��������.
	errno_t err_inicialization_data;
	FILE* fp_inicialization_data;
	err_inicialization_data = fopen_s(&fp_inicialization_data, "load.txt", "r");
	if (err_inicialization_data != 0) {
		// �������� �������� ��� ���� �����������.

	

		// ������������� ��������� �������� �� ������������ ��������� �������.
		// 26 ����� 2017.
		for (integer i = 0; i < maxelm; i++) {
			potent[VX][i] = starting_speed_Vx;
			potent[VY][i] = starting_speed_Vy;
			potent[VZ][i] = starting_speed_Vz;
			// ����������������� ���� �������� ������ ������������� ��������� �������������.
			potent[VXCOR][i] = starting_speed_Vx;
			potent[VYCOR][i] = starting_speed_Vy;
			potent[VZCOR][i] = starting_speed_Vz;						
		}

	}
	else {
		// 23 ���� 2017.
		// ����������������� ������������� ������� ��������� cfd �������� � 
		// ��������� � ��������� ���� load.txt.
		// ����� �� �������� ����� �������� ������ � ����� �������� ��������� ��
		// �������������� ��������������� ����� load.txt. 
		// ��� ���� �.�. � �������� ������ ����� ���������� �� �������������� ����������������� � 
		// ����� �� ����� ����������������� ������������� � ������� ������ ���������� ��������� �.�.������. 

		// ������� �������� 
		printf("load start...\n");

		// ��������� �����:
		// maxnode
		// maxelm
		// x
		// y
		// z
		// Vx
		// Vy
		// Vz
		// nvtx

		integer maxnode47 = 0;
		integer maxelm47 = 0;
		integer din47 = 0;
#if doubleintprecision == 1
		fscanf_s(fp_inicialization_data, "%lld", &din47);
		maxnode47 = din47;
		fscanf_s(fp_inicialization_data, "%lld", &din47);
#else
		fscanf_s(fp_inicialization_data, "%d", &din47);
		maxnode47 = din47;
		fscanf_s(fp_inicialization_data, "%d", &din47);
#endif		
		maxelm47 = din47;

		float fin47 = 0.0;
		doublereal *x47=NULL, *y47=NULL, *z47=NULL, *Vx47=NULL, *Vy47=NULL, *Vz47=NULL;
		integer** nvtx47 = NULL;
		// ����� ��������� � ����.
		x47 = new doublereal[maxnode47];
		y47 = new doublereal[maxnode47];
		z47 = new doublereal[maxnode47];
		Vx47 = new doublereal[maxnode47];
		Vy47 = new doublereal[maxnode47];
		Vz47 = new doublereal[maxnode47];
		// ���������� ������.
		for (integer i_47 = 0; i_47 < maxnode47; i_47++) {
			fscanf_s(fp_inicialization_data, "%f", &fin47);
			x47[i_47] = fin47;
		}
		for (integer i_47 = 0; i_47 < maxnode47; i_47++) {
			fscanf_s(fp_inicialization_data, "%f", &fin47);
			y47[i_47] = fin47;
		}
		for (integer i_47 = 0; i_47 < maxnode47; i_47++) {
			fscanf_s(fp_inicialization_data, "%f", &fin47);
			z47[i_47] = fin47;
		}

		for (integer i_47 = 0; i_47 < maxnode47; i_47++) {
			fscanf_s(fp_inicialization_data, "%f", &fin47);
			Vx47[i_47] = fin47;
		}
		for (integer i_47 = 0; i_47 < maxnode47; i_47++) {
			fscanf_s(fp_inicialization_data, "%f", &fin47);
			Vy47[i_47] = fin47;
		}
		for (integer i_47 = 0; i_47 < maxnode47; i_47++) {
			fscanf_s(fp_inicialization_data, "%f", &fin47);
			Vz47[i_47] = fin47;
		}
		nvtx47 = new integer*[8];
		for (integer i_47 = 0; i_47 < 8; i_47++) {
			nvtx47[i_47] = new integer[maxelm47];
		}

		for (integer j_47 = 0; j_47 < maxelm47; j_47++) {
			for (integer i_47 = 0; i_47 < 8; i_47++) {
#if doubleintprecision == 1
				fscanf_s(fp_inicialization_data, "%lld", &din47);
#else
				fscanf_s(fp_inicialization_data, "%d", &din47);
#endif
				
				nvtx47[i_47][j_47] = din47;
			}
		}
		fclose(fp_inicialization_data);

		printf("load done...\n");
		printf("interpolation start...\n");

		// ������������� ��������� �������� �� ������������ ��������� �������.
		// 26 ����� 2017.

		// ����� ��������� �������.
		doublereal min_x = 1e60;
		doublereal min_y = 1e60;
		doublereal min_z = 1e60;
		doublereal max_x = -1e60;
		doublereal max_y = -1e60;
		doublereal max_z = -1e60;

		for (integer i_48 = 0; i_48 < maxelm47; i_48++) {
			for (integer i_47 = 0; i_47 < 8; i_47++) {
				if (x47[nvtx47[i_47][i_48]] < min_x) {
					min_x = x47[nvtx47[i_47][i_48]];
				}
				if (y47[nvtx47[i_47][i_48]] < min_y) {
					min_y = y47[nvtx47[i_47][i_48]];
				}
				if (z47[nvtx47[i_47][i_48]] < min_z) {
					min_z = z47[nvtx47[i_47][i_48]];
				}
				if (x47[nvtx47[i_47][i_48]] > max_x) {
					max_x = x47[nvtx47[i_47][i_48]];
				}
				if (y47[nvtx47[i_47][i_48]] > max_y) {
					max_y = y47[nvtx47[i_47][i_48]];
				}
				if (z47[nvtx47[i_47][i_48]] > max_z) {
					max_z = z47[nvtx47[i_47][i_48]];
				}
			}
		}

		min_x = 1.05*fabs(max_x - min_x);
		if (min_x < 1.0e-40) {
			min_x = 1.05*fabs(max_x);
		}
		min_y = 1.05*fabs(max_y - min_y);
		if (min_y < 1.0e-40) {
			min_y = 1.05*fabs(max_y);
		}
		min_z = 1.05*fabs(max_z - min_z);
		if (min_z < 1.0e-40) {
			min_z = 1.05*fabs(max_z);
		}


		TOCHKA** pointerlist = new TOCHKA*[maxelm];
		doublereal** rthdsd_Gauss = new doublereal*[maxelm];
		for (integer i_47 = 0; i_47 < maxelm; i_47++) {
			pointerlist[i_47] = new TOCHKA[8];
			rthdsd_Gauss[i_47] = new doublereal[8];
		}

		for (integer i = 0; i < maxelm; i++) {
			doublereal xc47, yc47, zc47;

			TOCHKA p;
			center_cord3D(i, nvtx, pa, p, 100);
			xc47 = p.x;
			yc47 = p.y;
			zc47 = p.z;

			// ������ ����� ��������� ������������������� �������� 
			// � ������� ���������� ����� p.
			bool bfound = false;
			integer ifound = -1;
			for (integer i_47 = 0; i_47 < maxelm47; i_47++) {
				if ((xc47 >= x47[nvtx47[0][i_47]]) && (xc47 <= x47[nvtx47[1][i_47]]) && (yc47 >= y47[nvtx47[0][i_47]]) && (yc47 <= y47[nvtx47[3][i_47]]) &&
					(zc47 >= z47[nvtx47[0][i_47]]) && (zc47 <= z47[nvtx47[4][i_47]]))
				{
					ifound = i_47;
					bfound = true;
					break;
				}
			}

			// ���
			// ��� ����, ��� ������ ���������� ��������.
			doublereal starting_speed_Vx_47 = 0.0;
			doublereal starting_speed_Vy_47 = 0.0;
			doublereal starting_speed_Vz_47 = 0.0;

			if (bfound) {

				//TOCHKA p;
				//center_cord3D(i, nvtx, pa, p, 100);
				p.x = p.x + min_x;
				p.y = p.y + min_y;
				p.z = p.z + min_z;

				// VX

				for (integer j = 0; j <= 7; j++) {
					TOCHKA p1;
					p1.x = x47[nvtx47[j][ifound]];
					p1.y = y47[nvtx47[j][ifound]];
					p1.z = z47[nvtx47[j][ifound]];
					p1.x = p1.x + min_x;
					p1.y = p1.y + min_y;
					p1.z = p1.z + min_z;

					pointerlist[i][j] = p1;
					if (fabs(p1.x) < 1.0e-40) {
						printf("problem x=%e\n", p1.x);
						getchar();
					}
					if (fabs(p1.y) < 1.0e-40) {
						printf("problem y=%e\n", p1.y);
						getchar();
					}
					if (fabs(p1.z) < 1.0e-40) {
						printf("problem z=%e\n", p1.z);
						getchar();
					}
					rthdsd_Gauss[i][j] = Vx47[nvtx47[j][ifound]];
				}

				doublereal** Xmatr = new doublereal*[4];
				for (integer j = 0; j <= 3; j++) {
					Xmatr[j] = new doublereal[4];
				}


				doublereal* bmatr = new doublereal[4];
				doublereal* koefmatr = new doublereal[4];

				for (integer j1 = 0; j1 <= 3; j1++) {
					for (integer j2 = 0; j2 <= 3; j2++) {
						Xmatr[j1][j2] = 0.0;
					}
					bmatr[j1] = 0.0;
					koefmatr[j1] = 0.0;
				}




				for (integer j = 0; j < 8; j++) {

					Xmatr[0][0] += 1.0;
					Xmatr[0][1] += pointerlist[i][j].x;
					Xmatr[0][2] += pointerlist[i][j].y;
					Xmatr[0][3] += pointerlist[i][j].z;

					Xmatr[1][0] += pointerlist[i][j].x;
					Xmatr[1][1] += pointerlist[i][j].x*pointerlist[i][j].x;
					Xmatr[1][2] += pointerlist[i][j].x*pointerlist[i][j].y;
					Xmatr[1][3] += pointerlist[i][j].x*pointerlist[i][j].z;

					Xmatr[2][0] += pointerlist[i][j].y;
					Xmatr[2][1] += pointerlist[i][j].y*pointerlist[i][j].x;
					Xmatr[2][2] += pointerlist[i][j].y*pointerlist[i][j].y;
					Xmatr[2][3] += pointerlist[i][j].y*pointerlist[i][j].z;

					Xmatr[3][0] += pointerlist[i][j].z;
					Xmatr[3][1] += pointerlist[i][j].z*pointerlist[i][j].x;
					Xmatr[3][2] += pointerlist[i][j].z*pointerlist[i][j].y;
					Xmatr[3][3] += pointerlist[i][j].z*pointerlist[i][j].z;

					bmatr[0] += rthdsd_Gauss[i][j];
					bmatr[1] += pointerlist[i][j].x*rthdsd_Gauss[i][j];
					bmatr[2] += pointerlist[i][j].y*rthdsd_Gauss[i][j];
					bmatr[3] += pointerlist[i][j].z*rthdsd_Gauss[i][j];
				}


				for (integer j1 = 0; j1 <= 100; j1++) {
					koefmatr[0] = (bmatr[0] - Xmatr[0][1] * koefmatr[1] - Xmatr[0][2] * koefmatr[2] - Xmatr[0][3] * koefmatr[3]) / Xmatr[0][0];
					koefmatr[1] = (bmatr[1] - Xmatr[1][0] * koefmatr[0] - Xmatr[1][2] * koefmatr[2] - Xmatr[1][3] * koefmatr[3]) / Xmatr[1][1];
					koefmatr[2] = (bmatr[2] - Xmatr[2][0] * koefmatr[0] - Xmatr[2][1] * koefmatr[1] - Xmatr[2][3] * koefmatr[3]) / Xmatr[2][2];
					koefmatr[3] = (bmatr[3] - Xmatr[3][0] * koefmatr[0] - Xmatr[3][1] * koefmatr[1] - Xmatr[3][2] * koefmatr[2]) / Xmatr[3][3];
				}
				// ����������� 2.0 �������� 20.10.2018
				starting_speed_Vx_47 = 2.0*(koefmatr[0] + koefmatr[1] * (p.x) + koefmatr[2] * (p.y) + koefmatr[3] * (p.z));

				// VY

				for (integer j = 0; j <= 7; j++) {
					rthdsd_Gauss[i][j] = Vy47[nvtx47[j][ifound]];
				}

				for (integer j1 = 0; j1 <= 3; j1++) {
					for (integer j2 = 0; j2 <= 3; j2++) {
						Xmatr[j1][j2] = 0.0;
					}
					bmatr[j1] = 0.0;
					koefmatr[j1] = 0.0;
				}




				for (integer j = 0; j < 8; j++) {

					Xmatr[0][0] += 1.0;
					Xmatr[0][1] += pointerlist[i][j].x;
					Xmatr[0][2] += pointerlist[i][j].y;
					Xmatr[0][3] += pointerlist[i][j].z;

					Xmatr[1][0] += pointerlist[i][j].x;
					Xmatr[1][1] += pointerlist[i][j].x*pointerlist[i][j].x;
					Xmatr[1][2] += pointerlist[i][j].x*pointerlist[i][j].y;
					Xmatr[1][3] += pointerlist[i][j].x*pointerlist[i][j].z;

					Xmatr[2][0] += pointerlist[i][j].y;
					Xmatr[2][1] += pointerlist[i][j].y*pointerlist[i][j].x;
					Xmatr[2][2] += pointerlist[i][j].y*pointerlist[i][j].y;
					Xmatr[2][3] += pointerlist[i][j].y*pointerlist[i][j].z;

					Xmatr[3][0] += pointerlist[i][j].z;
					Xmatr[3][1] += pointerlist[i][j].z*pointerlist[i][j].x;
					Xmatr[3][2] += pointerlist[i][j].z*pointerlist[i][j].y;
					Xmatr[3][3] += pointerlist[i][j].z*pointerlist[i][j].z;

					bmatr[0] += rthdsd_Gauss[i][j];
					bmatr[1] += pointerlist[i][j].x*rthdsd_Gauss[i][j];
					bmatr[2] += pointerlist[i][j].y*rthdsd_Gauss[i][j];
					bmatr[3] += pointerlist[i][j].z*rthdsd_Gauss[i][j];
				}


				for (integer j1 = 0; j1 <= 100; j1++) {
					koefmatr[0] = (bmatr[0] - Xmatr[0][1] * koefmatr[1] - Xmatr[0][2] * koefmatr[2] - Xmatr[0][3] * koefmatr[3]) / Xmatr[0][0];
					koefmatr[1] = (bmatr[1] - Xmatr[1][0] * koefmatr[0] - Xmatr[1][2] * koefmatr[2] - Xmatr[1][3] * koefmatr[3]) / Xmatr[1][1];
					koefmatr[2] = (bmatr[2] - Xmatr[2][0] * koefmatr[0] - Xmatr[2][1] * koefmatr[1] - Xmatr[2][3] * koefmatr[3]) / Xmatr[2][2];
					koefmatr[3] = (bmatr[3] - Xmatr[3][0] * koefmatr[0] - Xmatr[3][1] * koefmatr[1] - Xmatr[3][2] * koefmatr[2]) / Xmatr[3][3];
				}
				// ����������� 2.0 �������� 20.10.2018
				starting_speed_Vy_47 = 2.0*(koefmatr[0] + koefmatr[1] * (p.x) + koefmatr[2] * (p.y) + koefmatr[3] * (p.z));


				// VZ

				for (integer j = 0; j <= 7; j++) {
					rthdsd_Gauss[i][j] = Vz47[nvtx47[j][ifound]];
				}

				for (integer j1 = 0; j1 <= 3; j1++) {
					for (integer j2 = 0; j2 <= 3; j2++) {
						Xmatr[j1][j2] = 0.0;
					}
					bmatr[j1] = 0.0;
					koefmatr[j1] = 0.0;
				}




				for (integer j = 0; j < 8; j++) {

					Xmatr[0][0] += 1.0;
					Xmatr[0][1] += pointerlist[i][j].x;
					Xmatr[0][2] += pointerlist[i][j].y;
					Xmatr[0][3] += pointerlist[i][j].z;

					Xmatr[1][0] += pointerlist[i][j].x;
					Xmatr[1][1] += pointerlist[i][j].x*pointerlist[i][j].x;
					Xmatr[1][2] += pointerlist[i][j].x*pointerlist[i][j].y;
					Xmatr[1][3] += pointerlist[i][j].x*pointerlist[i][j].z;

					Xmatr[2][0] += pointerlist[i][j].y;
					Xmatr[2][1] += pointerlist[i][j].y*pointerlist[i][j].x;
					Xmatr[2][2] += pointerlist[i][j].y*pointerlist[i][j].y;
					Xmatr[2][3] += pointerlist[i][j].y*pointerlist[i][j].z;

					Xmatr[3][0] += pointerlist[i][j].z;
					Xmatr[3][1] += pointerlist[i][j].z*pointerlist[i][j].x;
					Xmatr[3][2] += pointerlist[i][j].z*pointerlist[i][j].y;
					Xmatr[3][3] += pointerlist[i][j].z*pointerlist[i][j].z;

					bmatr[0] += rthdsd_Gauss[i][j];
					bmatr[1] += pointerlist[i][j].x*rthdsd_Gauss[i][j];
					bmatr[2] += pointerlist[i][j].y*rthdsd_Gauss[i][j];
					bmatr[3] += pointerlist[i][j].z*rthdsd_Gauss[i][j];
				}


				for (integer j1 = 0; j1 <= 100; j1++) {
					koefmatr[0] = (bmatr[0] - Xmatr[0][1] * koefmatr[1] - Xmatr[0][2] * koefmatr[2] - Xmatr[0][3] * koefmatr[3]) / Xmatr[0][0];
					koefmatr[1] = (bmatr[1] - Xmatr[1][0] * koefmatr[0] - Xmatr[1][2] * koefmatr[2] - Xmatr[1][3] * koefmatr[3]) / Xmatr[1][1];
					koefmatr[2] = (bmatr[2] - Xmatr[2][0] * koefmatr[0] - Xmatr[2][1] * koefmatr[1] - Xmatr[2][3] * koefmatr[3]) / Xmatr[2][2];
					koefmatr[3] = (bmatr[3] - Xmatr[3][0] * koefmatr[0] - Xmatr[3][1] * koefmatr[1] - Xmatr[3][2] * koefmatr[2]) / Xmatr[3][3];
				}
				// ����������� 2.0 �������� 20.10.2018
				starting_speed_Vz_47 = 2.0*(koefmatr[0] + koefmatr[1] * (p.x) + koefmatr[2] * (p.y) + koefmatr[3] * (p.z));

				for (integer j = 0; j <= 3; j++) {
					delete[] Xmatr[j];
				}
				delete[] Xmatr;
				delete[] bmatr;
				delete[] koefmatr;
			}

			if (starting_speed_Vx_47 != starting_speed_Vx_47) {
				printf("NAN pri interpolation Vx %d\n",i);
			}
			if (starting_speed_Vy_47 != starting_speed_Vy_47) {
				printf("NAN pri interpolation Vy %d\n",i);
			}
			if (starting_speed_Vz_47 != starting_speed_Vz_47) {
				printf("NAN pri interpolation Vz %d\n",i);
			}

			potent[VX][i] = starting_speed_Vx_47;
			potent[VY][i] = starting_speed_Vy_47;
			potent[VZ][i] = starting_speed_Vz_47;
			// ����������������� ���� �������� ������ ������������� ��������� �������������.
			potent[VXCOR][i] = starting_speed_Vx_47;
			potent[VYCOR][i] = starting_speed_Vy_47;
			potent[VZCOR][i] = starting_speed_Vz_47;
		}

		for (integer i = 0; i < maxelm; i++) {
			delete[] pointerlist[i];
			delete[] rthdsd_Gauss[i];
		}
		delete[] pointerlist;
		delete[] rthdsd_Gauss;

		// ������������ ����������� ������.
		delete[] x47;
		delete[] y47;
		delete[] z47;
		delete[] Vx47;
		delete[] Vy47;
		delete[] Vz47;
		for (integer i_47 = 0; i_47 < 8; i_47++) {
			delete[] nvtx47[i_47];
		}
		delete[] nvtx47;

		printf("done.\n");
		//system("PAUSE");
	}


	///* ����� �������� � �������� ���� ��������.
	// �� ������� ������ ���� ��������� ��������� ������� 8 ��� 2013�.
	// �������� ���� ������ ���� ���������������� � ������ ��������� ������� �������.
	for (integer i = 0; i < maxbound; i++) {

		
		
		if ((sosedb[i].MCB < (ls + lw)) && (sosedb[i].MCB >= ls)) {
			if ((w[sosedb[i].MCB - ls].bpressure) || (w[sosedb[i].MCB - ls].bopening)) {
				// �������� ������� ������ �� ������� ����� ������� �������.
				// ����������� �������� �� ��������� ���������� ����� ��������� �������.
				potent[VX][sosedb[i].iB] = potent[VX][sosedb[i].iI];
				potent[VY][sosedb[i].iB] = potent[VY][sosedb[i].iI];
				potent[VZ][sosedb[i].iB] = potent[VZ][sosedb[i].iI];
				// ����������������� ���� �������� ������ ������������� ��������� �������������.
				potent[VXCOR][sosedb[i].iB] = potent[VX][sosedb[i].iI];
				potent[VYCOR][sosedb[i].iB] = potent[VY][sosedb[i].iI];
				potent[VZCOR][sosedb[i].iB] = potent[VZ][sosedb[i].iI];
				//printf("Vx=%e, Vy=%e, Vz=%e\n",w[sosedb[i].MCB-ls].Vx, w[sosedb[i].MCB-ls].Vy, w[sosedb[i].MCB-ls].Vz);
				//getchar(); // debug
			}
			else {
				// �� ������� ������ ��������
				potent[VX][sosedb[i].iB] = w[sosedb[i].MCB - ls].Vx;
				potent[VY][sosedb[i].iB] = w[sosedb[i].MCB - ls].Vy;
				potent[VZ][sosedb[i].iB] = w[sosedb[i].MCB - ls].Vz;
				// ����������������� ���� �������� ������ ������������� ��������� �������������.
				potent[VXCOR][sosedb[i].iB] = w[sosedb[i].MCB - ls].Vx;
				potent[VYCOR][sosedb[i].iB] = w[sosedb[i].MCB - ls].Vy;
				potent[VZCOR][sosedb[i].iB] = w[sosedb[i].MCB - ls].Vz;
				//printf("Vx=%e, Vy=%e, Vz=%e\n",w[sosedb[i].MCB-ls].Vx, w[sosedb[i].MCB-ls].Vy, w[sosedb[i].MCB-ls].Vz);
				//getchar(); // debug
			}
		}
		
	}
	//*/
	 
	alpha=NULL;
	alpha = new doublereal[4];
	if (alpha==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for alpha constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	// �������� Fluent �������� ������
    alpha[VX]=0.7; // 0.5 0.7 0.8
    alpha[VY]=0.7;
    alpha[VZ]=0.7;
    alpha[PRESS]=0.3; //0.8 0.3 (0.2, �� ������ ������ �� ���������� SIMPLEC �� ���������).

   // ������������ ������� ���� ��� ���������� ��.
   sl=NULL;
   sl=new equation3D*[5];
   if (sl==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for slau flow constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
   
   for (integer i=0; i<5; i++) {
	   sl[i]=NULL;
   }

   for (integer i=0; i<5; i++) {
	   switch (i) {
		   case VX : sl[VX]=new equation3D[maxelm]; 
			         if (sl[VX]==NULL) {
	                     // ������������ ������ �� ������ ������������.
		                 printf("Problem : not enough memory on your equipment for slau[VX] constr struct...\n");
		                 printf("Please any key to exit...\n");
						// getchar();
						 system("pause");
		                 exit(1);
	                 }
			         break;
		   case VY : sl[VY]=new equation3D[maxelm];
			         if (sl[VY]==NULL) {
	                     // ������������ ������ �� ������ ������������.
		                 printf("Problem : not enough memory on your equipment for slau[VY] constr struct...\n");
		                 printf("Please any key to exit...\n");
						 //getchar();
						 system("pause");
		                 exit(1);
	                 } 
			         break;
		   case VZ : sl[VZ]=new equation3D[maxelm]; 
			         if (sl[VZ]==NULL) {
	                     // ������������ ������ �� ������ ������������.
		                 printf("Problem : not enough memory on your equipment for slau[VZ] constr struct...\n");
		                 printf("Please any key to exit...\n");
						 //getchar();
						 system("pause");
		                 exit(1);
	                 }
			         break;
		   case PRESS : sl[PRESS]=NULL; break;
		   case PAM : sl[PAM]=new equation3D[maxelm];
			          if (sl[PAM]==NULL) {
	                     // ������������ ������ �� ������ ������������.
		                 printf("Problem : not enough memory on your equipment for slau[PAM] constr struct...\n");
		                 printf("Please any key to exit...\n");
						 //getchar();
						 system("pause");
		                 exit(1);
	                  }
			          break;
	   }
   }

   // ������������ ������� ���� ��� ��������� ��
   slb=NULL;
   slb = new equation3D_bon*[5];
   if (slb==NULL) {
	    // ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for slau boundary constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
   }

   for (integer i=0; i<5; i++) {
	   slb[i]=NULL;
   }

   for (integer i=0; i<5; i++) {
	   switch (i) {
		   case VX : slb[VX]=new equation3D_bon[maxbound];
			         if (slb[VX]==NULL) {
	                     // ������������ ������ �� ������ ������������.
		                 printf("Problem : not enough memory on your equipment for slau_bon[VX] constr struct...\n");
		                 printf("Please any key to exit...\n");
						 //getchar();
						 system("pause");
		                 exit(1);
	                 } 
			         break;
		   case VY : slb[VY]=new equation3D_bon[maxbound]; 
			         if (slb[VY]==NULL) {
	                     // ������������ ������ �� ������ ������������.
		                 printf("Problem : not enough memory on your equipment for slau_bon[VY] constr struct...\n");
		                 printf("Please any key to exit...\n");
						 //getchar();
						 system("pause");
		                 exit(1);
	                 }
			         break;
		   case VZ : slb[VZ]=new equation3D_bon[maxbound]; 
			         if (slb[VZ]==NULL) {
	                     // ������������ ������ �� ������ ������������.
		                 printf("Problem : not enough memory on your equipment for slau_bon[VZ] constr struct...\n");
		                 printf("Please any key to exit...\n");
						 //getchar();
						 system("pause");
		                 exit(1);
	                 }  
			         break;
		   case PRESS : slb[PRESS]=NULL; break;
		   case PAM : slb[PAM]=new equation3D_bon[maxbound]; 
			         if (slb[PAM]==NULL) {
	                     // ������������ ������ �� ������ ������������.
		                 printf("Problem : not enough memory on your equipment for slau_bon[PAM] constr struct...\n");
		                 printf("Please any key to exit...\n");
						 //getchar();
						 system("pause");
		                 exit(1);
	                 }
			         break;
	   }
   }

   
				
  
} // allocation_memory_flow


  // ��������� ����������� ������ ��� ������ �������������.
void allocation_memory_flow_2(
	doublereal** &diag_coef,  integer maxelm, integer maxbound,
	 doublereal* &SInvariantStrainRateTensor,
	doublereal** &mf) {

	// ������������ ������������ ��������� �������� 
	// ���������� ������� ��� ���������� �������� ���-���.
	diag_coef = NULL;
	diag_coef = new doublereal*[3];
	if (diag_coef == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for diag_coef constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}

	for (integer i = 0; i<3; i++) {
		diag_coef[i] = NULL;
		diag_coef[i] = new doublereal[maxelm + maxbound];
		if (diag_coef[i] == NULL) {
			// ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
			printf("Problem : not enough memory on your equipment for diag_coef[%lld] constr struct...\n", i);
#else
			printf("Problem : not enough memory on your equipment for diag_coef[%d] constr struct...\n", i);
#endif
			
			printf("Please any key to exit...\n");
			//getchar();
			system("pause");
			exit(1);
		}
	}
	// �������������
	for (integer i = 0; i<3; i++) for (integer j = 0; j<(maxelm + maxbound); j++) diag_coef[i][j] = 1.0;

	// ��������� ������ ��� S ��������� ������� ���������-����������.
	SInvariantStrainRateTensor = NULL;
	SInvariantStrainRateTensor = new doublereal[maxelm + maxbound];
	if (SInvariantStrainRateTensor == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for SInvariantStrainRateTensor constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	// ������������� S ���������� strain-rate �������:
	for (integer i = 0; i<(maxelm + maxbound); i++) {
		SInvariantStrainRateTensor[i] = 0.0; // �������������
	}

	mf = NULL;
	mf = new doublereal*[maxelm];
	if (mf == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for mf constr struct...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}

	for (integer i = 0; i<maxelm; i++) {
		mf[i] = NULL;
		mf[i] = new doublereal[6];
		if (mf[i] == NULL) {
			// ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
			printf("Problem : not enough memory on your equipment for mf[%lld] constr struct...\n", i);
#else
			printf("Problem : not enough memory on your equipment for mf[%d] constr struct...\n", i);
#endif
			
			printf("Please any key to exit...\n");
			//getchar();
			system("pause");
			exit(1);
		}
	}
	// ������������� :
	for (integer i = 0; i<maxelm; i++) {
		for (integer j = 0; j<6; j++) {
			mf[i][j] = 0.0;
		}
	}

}

// ������ ����� ����� ������������ �������� ��� ����������� 
// ������������.
// ������������: �������� � ��� ����������� � ��� �������
void constr_nvtxcell(integer* evt, BOUND* sosedb, integer maxbound, integer maxelm, bool bextendedprint, integer** &nvtxcell, integer &ncell,
	integer inx, integer iny, integer inz, TOCKA_INT* &tck_int_list) {

	// ���� bextendedprinteger = true �� �� ����� ���� � ����������� ������� ������� ��������� �������� �����.

	// ��� ����������� ������������
	// nvtxcell - ����� ��� ����������� �������.
	// ncell - ���������� ������ ��� ����������� �������.
	ncell = 0;
	// ncell > maxelm.
	integer i, j, k;
	integer i1, i2, i3, i4, i5, i6, i7, i8;
	//for (i = 0; i<(inx - 1); i++) for (j = 0; j<(iny - 1); j++) for (k = 0; k<(inz - 1); k++) {
	// ��������� ����� ���������� �� ������� �������.
	for (integer iscan = 0; iscan < maxelm; iscan++) {

		integer i = tck_int_list[iscan].i;
		integer j = tck_int_list[iscan].j;
		integer k = tck_int_list[iscan].k;

		if ((i < (inx - 1)) && (j < (iny - 1)) && (k < (inz - 1)))
		{

			// ������� ���������� ������ ����� ����������� �� � �����
			i1 = i + j * inx + k * inx*iny;
		i2 = (i + 1) + j * inx + k * inx*iny;
		i3 = (i + 1) + (j + 1)*inx + k * inx*iny;
		i4 = i + (j + 1)*inx + k * inx*iny;
		i5 = i + j * inx + (k + 1)*inx*iny;
		i6 = (i + 1) + j * inx + (k + 1)*inx*iny;
		i7 = (i + 1) + (j + 1)*inx + (k + 1)*inx*iny;
		i8 = i + (j + 1)*inx + (k + 1)*inx*iny;
		/*i1 = i + j*inx + (k+1)*inx*iny;
		i2 = (i + 1) + j*inx + (k+1)*inx*iny;
		i3 = (i + 1) + (j + 1)*inx + (k+1)*inx*iny;
		i4 = i + (j + 1)*inx + (k+1)*inx*iny;
		i5 = i + j*inx + (k + 2)*inx*iny;
		i6 = (i + 1) + j*inx + (k + 2)*inx*iny;
		i7 = (i + 1) + (j + 1)*inx + (k + 2)*inx*iny;
		i8 = i + (j + 1)*inx + (k + 2)*inx*iny;
		*/
		if ((evt[i1] > 0) && (evt[i2] > 0) && (evt[i3] > 0) && (evt[i4] > 0) && (evt[i5] > 0) && (evt[i6] > 0) && (evt[i7] > 0) && (evt[i8] > 0)) {
			ncell++;

			if (bextendedprint) {

				// ��������� ������ ���������� ������� � ������� ��������� 
				// ������� ����������-������ ������� �� ������� ��������� ��������� �������.
				integer inorm = ESIDE;
				bool bfound1 = false, bfound2 = false, bfound3 = false, bfound4 = false;

				// �����.
				for (integer isel = 0; isel < maxbound; isel++) {
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i1] - 1)) {
						bfound1 = true;
					}
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i4] - 1)) {
						bfound2 = true;
					}
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i5] - 1)) {
						bfound3 = true;
					}
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i8] - 1)) {
						bfound4 = true;
					}
				}

				if (bfound1&&bfound2&&bfound3&&bfound4) {
					ncell++;
				}
				bfound1 = false;
				bfound2 = false;
				bfound3 = false;
				bfound4 = false;

				inorm = WSIDE;
				// �����.
				for (integer isel = 0; isel < maxbound; isel++) {
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i2] - 1)) {
						bfound1 = true;
					}
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i3] - 1)) {
						bfound2 = true;
					}
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i6] - 1)) {
						bfound3 = true;
					}
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i7] - 1)) {
						bfound4 = true;
					}
				}

				if (bfound1&&bfound2&&bfound3&&bfound4) {
					ncell++;
				}

				bfound1 = false;
				bfound2 = false;
				bfound3 = false;
				bfound4 = false;

				inorm = NSIDE;
				// �����.
				for (integer isel = 0; isel < maxbound; isel++) {
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i1] - 1)) {
						bfound1 = true;
					}
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i2] - 1)) {
						bfound2 = true;
					}
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i5] - 1)) {
						bfound3 = true;
					}
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i6] - 1)) {
						bfound4 = true;
					}
				}

				if (bfound1&&bfound2&&bfound3&&bfound4) {
					ncell++;
				}

				bfound1 = false;
				bfound2 = false;
				bfound3 = false;
				bfound4 = false;
				inorm = SSIDE;
				// �����.
				for (integer isel = 0; isel < maxbound; isel++) {
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i3] - 1)) {
						bfound1 = true;
					}
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i4] - 1)) {
						bfound2 = true;
					}
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i7] - 1)) {
						bfound3 = true;
					}
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i8] - 1)) {
						bfound4 = true;
					}
				}

				if (bfound1&&bfound2&&bfound3&&bfound4) {
					ncell++;
				}

				bfound1 = false;
				bfound2 = false;
				bfound3 = false;
				bfound4 = false;
				inorm = TSIDE;
				// �����.
				for (integer isel = 0; isel < maxbound; isel++) {
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i1] - 1)) {
						bfound1 = true;
					}
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i2] - 1)) {
						bfound2 = true;
					}
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i3] - 1)) {
						bfound3 = true;
					}
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i4] - 1)) {
						bfound4 = true;
					}
				}

				if (bfound1&&bfound2&&bfound3&&bfound4) {
					ncell++;
				}

				bfound1 = false;
				bfound2 = false;
				bfound3 = false;
				bfound4 = false;
				inorm = BSIDE;
				// �����.
				for (integer isel = 0; isel < maxbound; isel++) {
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i5] - 1)) {
						bfound1 = true;
					}
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i6] - 1)) {
						bfound2 = true;
					}
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i7] - 1)) {
						bfound3 = true;
					}
					if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i8] - 1)) {
						bfound4 = true;
					}
				}

				if (bfound1&&bfound2&&bfound3&&bfound4) {
					ncell++;
				}

			} // bextendedprint

		}
	}
	}
	// ��������� �� 
	nvtxcell = NULL;
	nvtxcell = new integer*[8];
	if (nvtxcell == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for nvtxcell constr struct...\n");
		printf("Please any key to exit...\n");
		// getchar();
		system("pause");
		exit(1);
	}
	integer l = 0;
	for (i = 0; i<8; i++) {
		nvtxcell[i] = NULL;
		nvtxcell[i] = new integer[ncell];
		if (nvtxcell[i] == NULL) {
			// ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
			printf("Problem : not enough memory on your equipment for nvtxcell[%lld] constr struct...\n", i);
#else
			printf("Problem : not enough memory on your equipment for nvtxcell[%d] constr struct...\n", i);
#endif
			
			printf("Please any key to exit...\n");
			//getchar();
			system("pause");
			exit(1);
		}
	}
	//for (i = 0; i<(inx - 1); i++) for (j = 0; j<(iny - 1); j++) for (k = 0; k<(inz - 1); k++) {
	// ��������� ����� ���������� �� ������� �������.
	for (integer iscan = 0; iscan < maxelm; iscan++) {

		integer i = tck_int_list[iscan].i;
		integer j = tck_int_list[iscan].j;
		integer k = tck_int_list[iscan].k;

		if ((i < (inx - 1)) && (j < (iny - 1)) && (k < (inz - 1)))
		{


			// ��������� �������������� ������ ��� ���������� ��.
			i1 = i + j * inx + k * inx*iny;
			i2 = (i + 1) + j * inx + k * inx*iny;
			i3 = (i + 1) + (j + 1)*inx + k * inx*iny;
			i4 = i + (j + 1)*inx + k * inx*iny;
			i5 = i + j * inx + (k + 1)*inx*iny;
			i6 = (i + 1) + j * inx + (k + 1)*inx*iny;
			i7 = (i + 1) + (j + 1)*inx + (k + 1)*inx*iny;
			i8 = i + (j + 1)*inx + (k + 1)*inx*iny;

			/*i1 = i + j*inx + (k + 1)*inx*iny;
			i2 = (i + 1) + j*inx + (k + 1)*inx*iny;
			i3 = (i + 1) + (j + 1)*inx + (k + 1)*inx*iny;
			i4 = i + (j + 1)*inx + (k + 1)*inx*iny;
			i5 = i + j*inx + (k + 2)*inx*iny;
			i6 = (i + 1) + j*inx + (k + 2)*inx*iny;
			i7 = (i + 1) + (j + 1)*inx + (k + 2)*inx*iny;
			i8 = i + (j + 1)*inx + (k + 2)*inx*iny;
			*/
			if ((evt[i1] > 0) && (evt[i2] > 0) && (evt[i3] > 0) && (evt[i4] > 0) && (evt[i5] > 0) && (evt[i6] > 0) && (evt[i7] > 0) && (evt[i8] > 0)) {
				nvtxcell[0][l] = evt[i1]; nvtxcell[1][l] = evt[i2]; nvtxcell[2][l] = evt[i3]; nvtxcell[3][l] = evt[i4];
				nvtxcell[4][l] = evt[i5]; nvtxcell[5][l] = evt[i6]; nvtxcell[6][l] = evt[i7]; nvtxcell[7][l] = evt[i8];
				l++;

				if (bextendedprint) {

					// ��������� ������ ���������� ������� � ������� ��������� 
					// ������� ����������-������ ������� �� ������� ��������� ��������� �������.
					integer inorm = ESIDE;
					bool bfound1 = false, bfound2 = false, bfound3 = false, bfound4 = false;
					integer im1 = -1, im2 = -1, im3 = -1, im4 = -1;


					// �����.
					for (integer isel = 0; isel < maxbound; isel++) {
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i1] - 1)) {
							bfound1 = true;
							im1 = isel + 1 + maxelm;
							//im1=sosedb[isel].iB+1;
						}
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i4] - 1)) {
							bfound2 = true;
							im2 = isel + 1 + maxelm;
							//im2=sosedb[isel].iB+1;
						}
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i5] - 1)) {
							bfound3 = true;
							im3 = isel + 1 + maxelm;
							//im3=sosedb[isel].iB+1;
						}
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i8] - 1)) {
							bfound4 = true;
							im4 = isel + 1 + maxelm;
							//im4=sosedb[isel].iB+1;
						}
					}

					if (bfound1&&bfound2&&bfound3&&bfound4) {
						nvtxcell[0][l] = im1; nvtxcell[1][l] = evt[i2]; nvtxcell[2][l] = evt[i3]; nvtxcell[3][l] = im2;
						nvtxcell[4][l] = im3; nvtxcell[5][l] = evt[i6]; nvtxcell[6][l] = evt[i7]; nvtxcell[7][l] = im4;
						l++;

					}
					bfound1 = false;
					bfound2 = false;
					bfound3 = false;
					bfound4 = false;

					inorm = WSIDE;
					// �����.
					for (integer isel = 0; isel < maxbound; isel++) {
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i2] - 1)) {
							bfound1 = true;
							im1 = isel + 1 + maxelm;
							//im1=sosedb[isel].iB+1;
						}
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i3] - 1)) {
							bfound2 = true;
							im2 = isel + 1 + maxelm;
							//im2=sosedb[isel].iB+1;
						}
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i6] - 1)) {
							bfound3 = true;
							im3 = isel + 1 + maxelm;
						}
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i7] - 1)) {
							bfound4 = true;
							im4 = isel + 1 + maxelm;
						}
					}

					if (bfound1&&bfound2&&bfound3&&bfound4) {
						nvtxcell[0][l] = evt[i1]; nvtxcell[1][l] = im1; nvtxcell[2][l] = im2; nvtxcell[3][l] = evt[i4];
						nvtxcell[4][l] = evt[i5]; nvtxcell[5][l] = im3; nvtxcell[6][l] = im4; nvtxcell[7][l] = evt[i8];
						l++;
					}

					bfound1 = false;
					bfound2 = false;
					bfound3 = false;
					bfound4 = false;

					inorm = NSIDE;
					// �����.
					for (integer isel = 0; isel < maxbound; isel++) {
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i1] - 1)) {
							bfound1 = true;
							im1 = isel + 1 + maxelm;
						}
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i2] - 1)) {
							bfound2 = true;
							im2 = isel + 1 + maxelm;
						}
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i5] - 1)) {
							bfound3 = true;
							im3 = isel + 1 + maxelm;
						}
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i6] - 1)) {
							bfound4 = true;
							im4 = isel + 1 + maxelm;
						}
					}

					if (bfound1&&bfound2&&bfound3&&bfound4) {
						nvtxcell[0][l] = im1; nvtxcell[1][l] = im2; nvtxcell[2][l] = evt[i3]; nvtxcell[3][l] = evt[i4];
						nvtxcell[4][l] = im3; nvtxcell[5][l] = im4; nvtxcell[6][l] = evt[i7]; nvtxcell[7][l] = evt[i8];
						l++;
					}

					bfound1 = false;
					bfound2 = false;
					bfound3 = false;
					bfound4 = false;
					inorm = SSIDE;
					// �����.
					for (integer isel = 0; isel < maxbound; isel++) {
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i3] - 1)) {
							bfound1 = true;
							im1 = isel + 1 + maxelm;
						}
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i4] - 1)) {
							bfound2 = true;
							im2 = isel + 1 + maxelm;
						}
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i7] - 1)) {
							bfound3 = true;
							im3 = isel + 1 + maxelm;
						}
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i8] - 1)) {
							bfound4 = true;
							im4 = isel + 1 + maxelm;
						}
					}

					if (bfound1&&bfound2&&bfound3&&bfound4) {
						nvtxcell[0][l] = evt[i1]; nvtxcell[1][l] = evt[i2]; nvtxcell[2][l] = im1; nvtxcell[3][l] = im2;
						nvtxcell[4][l] = evt[i5]; nvtxcell[5][l] = evt[i6]; nvtxcell[6][l] = im3; nvtxcell[7][l] = im4;
						l++;
					}

					bfound1 = false;
					bfound2 = false;
					bfound3 = false;
					bfound4 = false;
					inorm = TSIDE;
					// �����.
					for (integer isel = 0; isel < maxbound; isel++) {
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i1] - 1)) {
							bfound1 = true;
							im1 = isel + 1 + maxelm;
						}
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i2] - 1)) {
							bfound2 = true;
							im2 = isel + 1 + maxelm;
						}
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i3] - 1)) {
							bfound3 = true;
							im3 = isel + 1 + maxelm;
						}
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i4] - 1)) {
							bfound4 = true;
							im4 = isel + 1 + maxelm;
						}
					}

					if (bfound1&&bfound2&&bfound3&&bfound4) {
						nvtxcell[0][l] = im1; nvtxcell[1][l] = im2; nvtxcell[2][l] = im3; nvtxcell[3][l] = im4;
						nvtxcell[4][l] = evt[i5]; nvtxcell[5][l] = evt[i6]; nvtxcell[6][l] = evt[i7]; nvtxcell[7][l] = evt[i8];
						l++;
					}

					bfound1 = false;
					bfound2 = false;
					bfound3 = false;
					bfound4 = false;
					inorm = BSIDE;
					// �����.
					for (integer isel = 0; isel < maxbound; isel++) {
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i5] - 1)) {
							bfound1 = true;
							im1 = isel + 1 + maxelm;
						}
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i6] - 1)) {
							bfound2 = true;
							im2 = isel + 1 + maxelm;
						}
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i7] - 1)) {
							bfound3 = true;
							im3 = isel + 1 + maxelm;
						}
						if ((sosedb[isel].Norm == inorm) && (sosedb[isel].iI == evt[i8] - 1)) {
							bfound4 = true;
							im4 = isel + 1 + maxelm;
						}
					}

					if (bfound1&&bfound2&&bfound3&&bfound4) {
						nvtxcell[0][l] = evt[i1]; nvtxcell[1][l] = evt[i2]; nvtxcell[2][l] = evt[i3]; nvtxcell[3][l] = evt[i4];
						nvtxcell[4][l] = im1; nvtxcell[5][l] = im2; nvtxcell[6][l] = im3; nvtxcell[7][l] = im4;
						l++;
					}

				} // bextendedprint

			}
		}
	}
} // constr_nvtxcell

// ���������� ���������� �� ��������� ������
void calcdistwall(FLOW &f, integer ls, integer lw, WALL* w) {
	// ���������� ���������� �� ��������� ������ ��������� �� ������ ������� ��������������.
    f.rdistWall=NULL;
	f.rdistWall=new doublereal[f.maxelm+f.maxbound];
	if (f.rdistWall==NULL) {
	      // ������������ ������ �� ������ ������������.
		  printf("Problem : not enough memory on your equipment for rdistWall constr struct...\n");
		  printf("Please any key to exit...\n");
		  //getchar();
		  system("pause");
		  exit(1);
	}
	for (integer i=0; i<(f.maxelm+f.maxbound); i++) f.rdistWall[i]=1e30; // ����������� ����� ������� ������������ �����.

	bool *bwall=NULL;
	bwall=new bool[f.maxbound]; 
	if (bwall==NULL) {
	      // ������������ ������ �� ������ ������������.
		  printf("Problem : not enough memory on your equipment for bwall constr struct...\n");
		  printf("Please any key to exit...\n");
		  //getchar();
		  system("pause");
		  exit(1);
	}
	for (integer i=0; i<f.maxbound; i++) bwall[i]=false; // ������� �� ����������� ������ ������.
	doublereal **rposition=NULL;
	rposition=new doublereal*[3];
	if (rposition==NULL) {
	      // ������������ ������ �� ������ ������������.
		  printf("Problem : not enough memory on your equipment for rposition constr struct...\n");
		  printf("Please any key to exit...\n");
		  //getchar();
		  system("pause");
		  exit(1);
	}
	for (integer i=0; i<3; i++) {
		rposition[i]=NULL;
		rposition[i]=new doublereal[f.maxbound];
		if (rposition[i]==NULL) {
	      // ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
			printf("Problem : not enough memory on your equipment for rposition[%lld] constr struct...\n", i);
#else
			printf("Problem : not enough memory on your equipment for rposition[%d] constr struct...\n", i);
#endif
		  
		  printf("Please any key to exit...\n");
		  //getchar();
		  system("pause");
		  exit(1);
	    }
	}
	const integer X=0;
	const integer Y=1;
	const integer Z=2;
	for (integer i=0; i<3; i++) {
		for (integer j=0; j<(f.maxbound); j++) {
			rposition[i][j]=0.0; // �������������.
		}
	}

	for (integer i=0; i<f.maxelm; i++) {
		// ���� �� ���� ���������� ����������� �������
		for (integer G=0; G<6; G++) {
			if (f.sosedi[G][i].iNODE1>=f.maxelm) {
				// ��� ��������� ����
				integer inumber=f.sosedi[G][i].iNODE1-f.maxelm; // ����� ���������� ����
				TOCHKA p; // ���������� ������ ��.
				center_cord3D(i, f.nvtx, f.pa, p,100); // ���������� ��������� ������ ��.
				// ���������� �������� �������� ������������ ������:
	            doublereal dx=0.0, dy=0.0, dz=0.0;// ����� �������� ������������� ������
	            volume3D(i, f.nvtx, f.pa, dx, dy, dz);

				if ((f.sosedb[inumber].MCB==(ls+lw))||(f.sosedb[inumber].MCB<ls)) {
					bwall[inumber]=true;					
				}

				if ((f.sosedb[inumber].MCB<(ls + lw)) && (f.sosedb[inumber].MCB >= ls) && (!w[f.sosedb[inumber].MCB - ls].bsymmetry) && (!w[f.sosedb[inumber].MCB - ls].bopening) && (!w[f.sosedb[inumber].MCB - ls].bpressure)) {
					doublereal Magnitude=fabs(w[f.sosedb[inumber].MCB-ls].Vx)+fabs(w[f.sosedb[inumber].MCB-ls].Vy)+fabs(w[f.sosedb[inumber].MCB-ls].Vz);
					if (Magnitude<admission) {
						// ������ �� ������� ������ ������� ��������
						// ������������� ��� ������ ������
						bwall[inumber]=true;
					} // Magnitude
				} // WALL

				switch (G) {
					case ESIDE : rposition[X][inumber]=p.x+0.5*dx;
						     rposition[Y][inumber]=p.y;
							 rposition[Z][inumber]=p.z;
						     break;
					case WSIDE: rposition[X][inumber]=p.x-0.5*dx;
						     rposition[Y][inumber]=p.y;
							 rposition[Z][inumber]=p.z;
						     break;
					case NSIDE : rposition[X][inumber]=p.x;
						     rposition[Y][inumber]=p.y+0.5*dy;
							 rposition[Z][inumber]=p.z;
						     break;
					case SSIDE : rposition[X][inumber]=p.x;
						     rposition[Y][inumber]=p.y-0.5*dy;
							 rposition[Z][inumber]=p.z;
						     break;
					case TSIDE : rposition[X][inumber]=p.x;
						     rposition[Y][inumber]=p.y;
							 rposition[Z][inumber]=p.z+0.5*dz;
						     break;
					case BSIDE : rposition[X][inumber]=p.x;
						     rposition[Y][inumber]=p.y;
							 rposition[Z][inumber]=p.z-0.5*dz;
						     break;
					}

			}
		}
	}

	// ���������������� ���������� ����������:
	// ������ ��� ���������� ��.
	for (integer i=0; i<f.maxelm; i++) {
		TOCHKA p; // ���������� ������ ��.
		center_cord3D(i, f.nvtx, f.pa, p,100); // ���������� ��������� ������ ��.
		for (integer j=0; j<f.maxbound; j++) {
			if (bwall[j]) {
				doublereal dist=sqrt((p.x-rposition[X][j])*(p.x-rposition[X][j])+(p.y-rposition[Y][j])*(p.y-rposition[Y][j])+(p.z-rposition[Z][j])*(p.z-rposition[Z][j]));
				if (dist<f.rdistWall[i]) f.rdistWall[i]=dist;
			}
		}
	}

	// ���������������� ���������� ���������� ��� ��������� ��:
	// ������������� ������� �������, �������� ������� ��� ������� � ��������� ���������.
	for (integer i=f.maxelm; i<f.maxelm+f.maxbound; i++) {
		integer inumber=i-f.maxelm; // ����� ���������� ����
		if (bwall[inumber]) {
			// ���������� ����� ����
			f.rdistWall[i]=0.0;
		}
		else {
            for (integer j=0; j<f.maxbound; j++) {
		 	    if (bwall[j]) {
                    doublereal dist=sqrt((rposition[X][inumber]-rposition[X][j])*(rposition[X][inumber]-rposition[X][j])+(rposition[Y][inumber]-rposition[Y][j])*(rposition[Y][inumber]-rposition[Y][j])+(rposition[Z][inumber]-rposition[Z][j])*(rposition[Z][inumber]-rposition[Z][j]));
				    if (dist<f.rdistWall[i]) f.rdistWall[i]=dist;
			    }
			}
		}
	}

	// ���������� ������� ������������ ���� � ������� ���������
	f.rdistWallmax=-1e20; // ����� ��������� ��������
	for (integer i=0; i<(f.maxelm+f.maxbound); i++) {
		if (f.rdistWall[i]>f.rdistWallmax) f.rdistWallmax=f.rdistWall[i];
	}


	// ������������ ����������� ������.
	delete[] bwall;
	bwall=NULL;
	for (integer i=0; i<3; i++) {
		delete[] rposition[i];
		rposition[i]=NULL;
	}
	delete[] rposition;
	rposition=NULL;
} // calcdistwall

void constr_fluid_equation(FLOW* &f,  integer flow_interior,
	                       integer ls, integer lw, WALL* w) {

	// ���� bCFXcalcdistancetotheWALL==true �� ����� �����������
	// ������ ���������� ���������� �� ��������� ������ ������������ � ANSYS CFX.
	// � ���� bCFXcalcdistancetotheWALL==false �� ����� ����������� ������� ������� ����
    // ���������� ����������� ���������� �� ��������� ������.
	bool bCFXcalcdistancetotheWALL=true;//true;

	if (flow_interior==0) {
		if (eqin.imaxflD>0) {
			printf("Error: liquid zones should not be...\n");
			printf("Will be carried out of the program.\n");
			printf("please, press any key to exit...\n");
			//getchar();
			system("pause");
			exit(0); // ����� �� ����������
		}
	}
	else {
		if (flow_interior!=eqin.imaxflD) {
			if ((eqin.itemper==1)&&(eqin.imaxflD==0)) {
				for (integer i=0; i<flow_interior; i++) {
				    f[i].rdistWall=NULL; // �������������
					f[i].iflowregime=LAMINAR; // LAMINAR
				}
				printf("pure heat calculation.\n");
			}
			else {

			     printf("number of recognized program of liquid\n");
			     printf("isolated areas do not correspond to the\n");
			     printf("number of isolated areas of a user-defined liquid.\n");
			     printf("Will be carried out of the program.\n");
                 printf("please, press any key to exit...\n");
#if doubleintprecision == 1
				 printf("debug! flow_interior=%lld eqin.imaxflD=%lld\n", flow_interior, eqin.imaxflD);
#else
				 printf("debug! flow_interior=%d eqin.imaxflD=%d\n", flow_interior, eqin.imaxflD);
#endif
			     
			     //getchar();
				 system("pause");
				 exit(0); // ����� �� ����������
			}
		}
		else {

			integer i=0; // ������� ����� for

			// �� � ������� ���������� ������������� ������ ��� ������������ ����������
			// ����� ���������� ������������� ������ ��� �������� ������������� � ����������.

			// ����� ����������� ������ ���� �������� ������������� � ��� ������� � ������� ��� ���� 
			// ���������� ����������. ��� ����� ������� � ������� ������� ���������� �� ������� ����� �������� �������������
			// �� ������ ���������� ������ ���� (����������� ����������).
			if (flow_interior==1) {
				// �� �������� �.�. ����� ������ ���� ������ ����.

				if (eqin.fluidinfo[0].iflowregime==0) {
					// ���������� �����
					f[0].iflowregime=LAMINAR; // LAMINAR
				}
				else {
					if (eqin.fluidinfo[0].iturbmodel==0) {
                       f[0].iflowregime=ZEROEQMOD; // ZEROEQMOD
					}
					if (eqin.fluidinfo[0].iturbmodel==1) {
                       f[0].iflowregime=SMAGORINSKY; // SMAGORINSKY
					}
					if (eqin.fluidinfo[0].iturbmodel==2) {
                       f[0].iflowregime=RNG_LES; // RNG_LES
					}
					// ���������� ���������� ������ �������������.
                       /*
					   // debug.
					   // �������� ������ �������� ���� ����� �����. ���������� �������.
					   f[0].smaginfo.Cs=0.151; 
					   f[0].smaginfo.bfdelta=true; // ���� ��������������� �����
					   f[0].smaginfo.bSmagorinsky_Lilly=true; // ������ �������������-�����.
					   f[0].smaginfo.bsurface_roughness=false; // �� ��������� ������������� ������.
					   f[0].smaginfo.ipowerroughness=2; // �������� ������������� �������.
					   f[0].smaginfo.roughness=10e-6; // ������������ ������������� ����������� 10 ���.
					   */
					   f[0].smaginfo.Cs=eqin.fluidinfo[0].Cs; // ���������� �������������.
					   // ������������ �� ������������ ������ ������� ��� �����������
					   // �������� ���������� �������������.
					   f[0].smaginfo.bDynamic_Stress=eqin.fluidinfo[0].bDynamic_Stress;
					   // ������������ �� ����������� ����������� �� ���������� �������������.
					   f[0].smaginfo.bLimiters_Cs=eqin.fluidinfo[0].bLimiters_Cs;
					   f[0].smaginfo.minCs=eqin.fluidinfo[0].rminCs; // ����������� �������� ���������� �������������
					   f[0].smaginfo.maxCs=eqin.fluidinfo[0].rmaxCs; // ������������ �������� ���������� �������������
					   // ��� ������� ������� ������������ � ������ �������.
					   f[0].smaginfo.itypeFiltrGermano=eqin.fluidinfo[0].itypeFiltrGermano;
					   // ���� ��������������� �����
					   if (eqin.fluidinfo[0].bfdelta==1) {
						   f[0].smaginfo.bfdelta=true; // �������. 
					   }
					   else {
						   f[0].smaginfo.bfdelta=false; // ��������.
					   }
					   // ������ �������������-�����.
					   if (eqin.fluidinfo[0].bSmagorinskyLilly==1) {
						   f[0].smaginfo.bSmagorinsky_Lilly=true; // �������.
					   }
					   else {
						   f[0].smaginfo.bSmagorinsky_Lilly=false; // ��������.
					   }
					   // ���� ������������� ������.
					   if (eqin.fluidinfo[0].bsurface_roughness==1) {
						   f[0].smaginfo.bsurface_roughness=true; // ��������� ������������� ������.
					   }
					   else {
						   f[0].smaginfo.bsurface_roughness=false; // �� ��������� ������������� ������.
					   }

					   f[0].smaginfo.bRichardsonCorrect=eqin.fluidinfo[0].bSwirlAmendment; // �������� �� ������� � ��������� ����� ����.
					   f[0].smaginfo.bSelectiveSmagorinsky=eqin.fluidinfo[0].bSelectiveSmagorinsky; // ��������������� � ������ �������������.
					   
					   f[0].smaginfo.ipowerroughness=eqin.fluidinfo[0].ipowerroughness; // �������� ������������� ������� ipowerroughness==2.
					   f[0].smaginfo.roughness=eqin.fluidinfo[0].roughness; // ������������ ������������� ����������� 10 ���.

					   f[0].smaginfo.rRichardsonMultiplyer=eqin.fluidinfo[0].rRi_mult; // ��������� �������������� ������������ ����� ����������.
					   f[0].smaginfo.SSangle=eqin.fluidinfo[0].rSelectiveAngle; // ��������� �������� ���� � ������ Selective Smagorinsky.
					   f[0].smaginfo.itypeFILTRSelectiveSmagorinsky=eqin.fluidinfo[0].itypeSelectiveSmagorinsky_filtr; // ��� ������� � ������ Selective Smagorinsky.
				}

				// ���������� ��������������� ������� ��� ������� �������������� :
			    bool bdist=false; // ��������� �� ���������� �� ������.
			    switch (f[0].iflowregime) {
			      case LAMINAR : bdist=false; break;  // ���������� �������.
			      case ZEROEQMOD : bdist=true; break; // Zero Equation Turbulence model (RANS)
				  case SMAGORINSKY : bdist=true; break; // Smagorinsky model (LES) (�������� ��� ����� ������ ������� 1991 ����).
				  case RNG_LES : bdist=false; break; // RNG_LES Renormalization Group Theory (LES)
			    }
				f[0].rdistWall=NULL; // �������������
			    if (bdist) {
				   // ���������� ���������� �� ��������� ������
				   printf("please wait! program calculate distance to the wall...\n");
				   if (!bCFXcalcdistancetotheWALL) {
					   // ������� ���������� ��������.
					   calcdistwall(f[0], ls, lw, w);
				   }
				   else {
					   // ������ ������������ � ANSYS CFX.
					   calcdistwallCFX(f[0], ls, lw, w);
				   }
			    }
			}
			else {
				// ����� ��� ������ ���� ��� ������.
			    doublereal** distzone=NULL;
				// distzone[i][j] - ���������� ���������� �� ������� ����� i-��� ������� ������ ������������ �� j-�� ����������� ���������� ������ ����.
				// ��������� ������ ����������� ������
				distzone=new doublereal*[flow_interior];
				if (distzone==NULL) {
	                // ������������ ������ �� ������ ������������.
		            printf("Problem : not enough memory on your equipment for distzone constr struct...\n");
		            printf("Please any key to exit...\n");
					//getchar();
					system("pause");
		            exit(1);
	            }
				for (i=0; i<flow_interior; i++) {
					distzone[i]=NULL;
					distzone[i]=new doublereal[flow_interior];
					if (distzone[i]==NULL) {
	                     // ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
						printf("Problem : not enough memory on your equipment for distzone[%lld] constr struct...\n", i);
#else
						printf("Problem : not enough memory on your equipment for distzone[%d] constr struct...\n", i);
#endif
		                
		                 printf("Please any key to exit...\n");
						 //getchar();
						 system("pause");
		                 exit(1);
	                }
				}
				// �������������:
				for (i=0; i<flow_interior; i++) for (integer j=0; j<flow_interior; j++) distzone[i][j]=1e30; // ����������� ����������� ������� �����.
				// ���������� ����������:
				for (i=0; i<flow_interior; i++) {
					for (integer j=0; j<flow_interior; j++) {
						// ������ �� ���������� ����������� �������
						TOCHKA p; // ���������� ������ ��.
						for (integer iP=0; iP<f[j].maxelm; iP++) {
							center_cord3D(iP, f[j].nvtx, f[j].pa, p,100); // ���������� ��������� ������ ��.
							distzone[i][j]=fmin(distzone[i][j],sqrt((eqin.fluidinfo[i].xc-p.x)*(eqin.fluidinfo[i].xc-p.x)+(eqin.fluidinfo[i].yc-p.y)*(eqin.fluidinfo[i].yc-p.y)+(eqin.fluidinfo[i].zc-p.z)*(eqin.fluidinfo[i].zc-p.z)));
						}
					}
				}
				integer *key=NULL;
				key=new integer[flow_interior]; // ���� ��� ����������
				if (key==NULL) {
	                // ������������ ������ �� ������ ������������.
		            printf("Problem : not enough memory on your equipment for key constr struct...\n");
		            printf("Please any key to exit...\n");
					//getchar();
					system("pause");
		            exit(1);
	            }
				// �������������.
				for (i=0; i<flow_interior; i++) key[i]=flow_interior; // ����������� ������������ ��������.
				for (i=0; i<flow_interior; i++) {
					// ����� �������� � i-�� ������ distzone
					doublereal rmin=1e30;
					integer imin=flow_interior;
					for (integer j=0; j<flow_interior; j++) {
						if (distzone[i][j]<rmin) {
							rmin=distzone[i][j];
							imin=j;
						}
					}
					// ����������� ���������� � i-�� ������ ��������� � ������ imin
					key[i]=imin;
				}

				bool berror=false;
				for (i=0; i<flow_interior; i++) if (key[i]>=flow_interior) {
					// ������ ������������� ������ ��� �������� ��������������.
					printf("Error detection of liquid zones defined by the user!\n");
					printf("Will be carried out of the program.\n");
					printf("Please, press any key to exit...\n");
					//getchar();
					system("pause");
					exit(0);
				}

				// ���������� ������� eqin.fluidinfo �� ����� key.
                // ����������� ���������� BubbleSort
				integer numberOfPairs=flow_interior;
				bool swappedElements=true;
				while (swappedElements) {
                    numberOfPairs--;
					swappedElements=false;
					for (i=0; i<numberOfPairs; i++) {
						if (key[i]>key[i+1]) {
							// Swap i <-> i+1
							TFLOWINFO termflow;
							termflow=eqin.fluidinfo[i];
							eqin.fluidinfo[i]=eqin.fluidinfo[i+1];
							eqin.fluidinfo[i+1]=termflow;
							// swap key i <-> i+1
							integer ikeyterm;
							ikeyterm=key[i];
							key[i]=key[i+1];
							key[i+1]=ikeyterm;
							swappedElements=true;
						}
					}
				}

				// �������� ������������, �.�. � ������� key[] �� ������ ���� ������������� �������� ������.
				berror=false; // ������ �� ����������.
				for (i=0; i<(flow_interior-1); i++) {
					// ������� ������� ��� ����� key[] ������ ������������� � ������� ����������� 
					if (key[i]==key[i+1]) berror=true;
				}

				if (berror) {
					// ������! ��� ��� ����� ���������������� �� ��������  ������ ����
					// ��������� �� ���� � �� �� ������������ ������ ���� ������������ ����������. 
                    printf("Error! two or more non-overlapping zones \n");
					printf("of fluid liquid refer to the same\n");
					printf("isolated area of the liquid the recognized program.\n");
					printf("Will be carried out of the program.\n");
					printf("Please, press any key to exit...\n");
					//getchar();
					system("pause");
					exit(0);
				}

				// ������������ ����������� ������
				for (i=0; i<flow_interior; i++) {
					delete[] distzone[i];
					distzone[i]=NULL;
				}
				delete[] distzone;
				distzone=NULL;
				delete[] key;
				key=NULL;

				for (i=0; i<flow_interior; i++) {
					if (eqin.fluidinfo[i].iflowregime==0) {
					    // ���������� �����
					    f[i].iflowregime=LAMINAR; // LAMINAR
				    }
				    else {
					    if (eqin.fluidinfo[i].iturbmodel==0) {
                            f[i].iflowregime=ZEROEQMOD; // ZEROEQMOD
					    }
						if (eqin.fluidinfo[i].iturbmodel==1) {
                            f[i].iflowregime=SMAGORINSKY; // SMAGORINSKY	
					    }
                        if (eqin.fluidinfo[i].iturbmodel==2) {
                            f[i].iflowregime=RNG_LES; // RNG_LES	
					    }

						f[i].smaginfo.Cs=eqin.fluidinfo[i].Cs; // ���������� �������������.
						// ������������ �� ������������ ������ ������� ��� �����������
					    // �������� ���������� �������������.
					    f[i].smaginfo.bDynamic_Stress=eqin.fluidinfo[i].bDynamic_Stress;
						// ������������ �� ����������� ����������� �� ���������� �������������.
					    f[i].smaginfo.bLimiters_Cs=eqin.fluidinfo[i].bLimiters_Cs;
					    f[i].smaginfo.minCs=eqin.fluidinfo[i].rminCs; // ����������� �������� ���������� �������������
					    f[i].smaginfo.maxCs=eqin.fluidinfo[i].rmaxCs; // ������������ �������� ���������� �������������
					    // ��� ������� ������� ������������ � ������ �������.
					    f[i].smaginfo.itypeFiltrGermano=eqin.fluidinfo[i].itypeFiltrGermano;
					    // ���� ��������������� �����
					    if (eqin.fluidinfo[i].bfdelta==1) {
						    f[i].smaginfo.bfdelta=true; // �������. 
					    }
					    else {
						    f[i].smaginfo.bfdelta=false; // ��������.
					    }
					    // ������ �������������-�����.
					    if (eqin.fluidinfo[i].bSmagorinskyLilly==1) {
						    f[i].smaginfo.bSmagorinsky_Lilly=true; // �������.
					    }
					    else {
						    f[i].smaginfo.bSmagorinsky_Lilly=false; // ��������.
					    }
					    // ���� ������������� ������.
					    if (eqin.fluidinfo[i].bsurface_roughness==1) {
						    f[i].smaginfo.bsurface_roughness=true; // ��������� ������������� ������.
					    }
					    else {
						    f[i].smaginfo.bsurface_roughness=false; // �� ��������� ������������� ������.
					    }
					   
						f[i].smaginfo.bRichardsonCorrect=eqin.fluidinfo[i].bSwirlAmendment; // �������� �� ������� � ��������� ����� ����.
					    f[i].smaginfo.bSelectiveSmagorinsky=eqin.fluidinfo[i].bSelectiveSmagorinsky; // ��������������� � ������ �������������.

						f[i].smaginfo.ipowerroughness=eqin.fluidinfo[i].ipowerroughness; // �������� ������������� ������� ipowerroughness==2.
					    f[i].smaginfo.roughness=eqin.fluidinfo[i].roughness; // ������������ ������������� ����������� 10 ���.

						f[i].smaginfo.rRichardsonMultiplyer=eqin.fluidinfo[i].rRi_mult; // ��������� �������������� ������������ ����� ����������.
					    f[i].smaginfo.SSangle=eqin.fluidinfo[i].rSelectiveAngle; // ��������� �������� ���� � ������ Selective Smagorinsky.
					    f[i].smaginfo.itypeFILTRSelectiveSmagorinsky=eqin.fluidinfo[i].itypeSelectiveSmagorinsky_filtr; // ��� ������� � ������ Selective Smagorinsky.

				    }

				    // ���������� ��������������� ������� ��� ������� �������������� :
			        bool bdist=false; // ��������� �� ���������� �� ������.
			        switch (f[i].iflowregime) {
			           case LAMINAR : bdist=false; break;
			           case ZEROEQMOD : bdist=true; break; // Zero Equation Model (RANS)
					   case SMAGORINSKY : bdist=true; break; // Smagorinsky model (LES) (�������� ��� ����� ������ ������� 1991 ����).
					   case RNG_LES : bdist=false; break; // RNG_LES Renormalization Group Theory (LES)
			        }
					f[i].rdistWall=NULL; // �������������
			        if (bdist) {
				       // ���������� ���������� �� ��������� ������
				       printf("please wait! program calculate distance to the wall...\n");
					   if (!bCFXcalcdistancetotheWALL) {
					        // ������� ���������� ��������.
					        calcdistwall(f[i], ls, lw, w);
				       }
				       else {
					        // ������ ������������ � ANSYS CFX.
					        calcdistwallCFX(f[i], ls, lw, w);
				       }
			        }
				}
				
			}
		}
	}
} // constr_fluid_equation

// ������ ����������� ����� ��� ������ ��������� ������ �����.
void constr_link_on_surface_for_radiation_model(integer maxelm, integer* &whot_is_block, ALICE_PARTITION** sosedi,
	                                            integer** nvtx, TOCHKA* pa,  BLOCK* &b, integer lb)
{
	for (integer i = 0; i < lb; i++) {
		if (b[i].radiation.binternalRadiation) {
			integer itek = i;

			MY_PAIR* list = new MY_PAIR[maxelm];
			integer ic = 0;

			

			for (integer iP = 0; iP < maxelm; iP++) {
				integer ib = whot_is_block[iP];
				if (ib == itek) 
				{
					integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
					iE = sosedi[ESIDE][iP].iNODE1; iN = sosedi[NSIDE][iP].iNODE1; iT = sosedi[TSIDE][iP].iNODE1; iW = sosedi[WSIDE][iP].iNODE1; iS = sosedi[SSIDE][iP].iNODE1; iB = sosedi[BSIDE][iP].iNODE1;

					// ���������� �������� �������� ������������ ������:
					doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
					volume3D(iP, nvtx, pa, dx, dy, dz);

					if (iW >= maxelm) {
						// ��������� ����.
						list[ic].node1 = iP;
						list[ic].node21 = iW;
						list[ic].node22 = -1;
						list[ic].node23 = -1;
						list[ic].node24 = -1;
						//list[ic].dS = dy*dz; // ������� ������.
						list[ic].dS1 = dy*dz;
						list[ic].dS2 = 0.0;
						list[ic].dS3 = 0.0;
						list[ic].dS4 = 0.0;
						ic++;
					}
					else
					{
						// ���������� �����, �� ������ ����������� ������ ������.
						if (itek != whot_is_block[iW]) {
							list[ic].node1 = iP;
							list[ic].node21 = iW;
							list[ic].node22 = -1;
							list[ic].node23 = -1;
							list[ic].node24 = -1;
							//list[ic].dS = dy*dz; // ������� ������.
							list[ic].dS1 = dy*dz;
							list[ic].dS2 = 0.0;
							list[ic].dS3 = 0.0;
							list[ic].dS4 = 0.0;
							ic++;
						}
					}

				}
			}

			b[itek].radiation.nodelistW = new MY_PAIR[ic];
			b[itek].radiation.nodelistWsize = ic;
			for (integer j = 0; j < ic; j++) b[itek].radiation.nodelistW[j] = list[j];

			delete[] list;
			list = NULL;

			list = new MY_PAIR[maxelm];
			ic = 0;

			for (integer iP = 0; iP < maxelm; iP++) {
				integer ib = whot_is_block[iP];
				if (ib == itek)
				{
					integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
					iE = sosedi[ESIDE][iP].iNODE1; iN = sosedi[NSIDE][iP].iNODE1; iT = sosedi[TSIDE][iP].iNODE1; iW = sosedi[WSIDE][iP].iNODE1; iS = sosedi[SSIDE][iP].iNODE1; iB = sosedi[BSIDE][iP].iNODE1;

					// ���������� �������� �������� ������������ ������:
					doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
					volume3D(iP, nvtx, pa, dx, dy, dz);

					if (iE >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node21 = iE;
						list[ic].node22 = -1;
						list[ic].node23 = -1;
						list[ic].node24 = -1;
						//list[ic].dS = dy*dz; // ������� ������.
						list[ic].dS1 = dy*dz;
						list[ic].dS2 = 0.0;
						list[ic].dS3 = 0.0;
						list[ic].dS4 = 0.0;
						ic++;
					}
					else
					{
						if (itek != whot_is_block[iE]) {
							list[ic].node1 = iP;
							list[ic].node21 = iE;
							list[ic].node22 = -1;
							list[ic].node23 = -1;
							list[ic].node24 = -1;
							//list[ic].dS = dy*dz; // ������� ������.
							list[ic].dS1 = dy*dz;
							list[ic].dS2 = 0.0;
							list[ic].dS3 = 0.0;
							list[ic].dS4 = 0.0;
							ic++;
						}
					}

				}
			}

			b[itek].radiation.nodelistE = new MY_PAIR[ic];
			b[itek].radiation.nodelistEsize = ic;
			for (integer j = 0; j < ic; j++) b[itek].radiation.nodelistE[j] = list[j];

			delete[] list;
			list = NULL;

			list = new MY_PAIR[maxelm];
			ic = 0;

			for (integer iP = 0; iP < maxelm; iP++) {
				integer ib = whot_is_block[iP];
				if (ib == itek)
				{
					integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
					iE = sosedi[ESIDE][iP].iNODE1; iN = sosedi[NSIDE][iP].iNODE1; iT = sosedi[TSIDE][iP].iNODE1; iW = sosedi[WSIDE][iP].iNODE1; iS = sosedi[SSIDE][iP].iNODE1; iB = sosedi[BSIDE][iP].iNODE1;

					// ���������� �������� �������� ������������ ������:
					doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
					volume3D(iP, nvtx, pa, dx, dy, dz);

					if (iS >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node21 = iS;
						list[ic].node22 = -1;
						list[ic].node23 = -1;
						list[ic].node24 = -1;
						//list[ic].dS = dx*dz; // ������� ������.
						list[ic].dS1 = dx*dz;
						list[ic].dS2 = 0.0;
						list[ic].dS3 = 0.0;
						list[ic].dS4 = 0.0;
						ic++;
					}
					else
					{
						if (itek != whot_is_block[iS]) {
							list[ic].node1 = iP;
							list[ic].node21 = iS;
							list[ic].node22 = -1;
							list[ic].node23 = -1;
							list[ic].node24 = -1;
							//list[ic].dS = dx*dz; // ������� ������.
							list[ic].dS1 = dx*dz;
							list[ic].dS2 = 0.0;
							list[ic].dS3 = 0.0;
							list[ic].dS4 = 0.0;
							ic++;
						}
					}

				}
			}

			b[itek].radiation.nodelistS = new MY_PAIR[ic];
			b[itek].radiation.nodelistSsize = ic;
			for (integer j = 0; j < ic; j++) b[itek].radiation.nodelistS[j] = list[j];

			delete[] list;
			list = NULL;

			list = new MY_PAIR[maxelm];
			ic = 0;

			for (integer iP = 0; iP < maxelm; iP++) {
				integer ib = whot_is_block[iP];
				if (ib == itek)
				{
					integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
					iE = sosedi[ESIDE][iP].iNODE1; iN = sosedi[NSIDE][iP].iNODE1; iT = sosedi[TSIDE][iP].iNODE1; iW = sosedi[WSIDE][iP].iNODE1; iS = sosedi[SSIDE][iP].iNODE1; iB = sosedi[BSIDE][iP].iNODE1;

					// ���������� �������� �������� ������������ ������:
					doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
					volume3D(iP, nvtx, pa, dx, dy, dz);

					if (iN >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node21 = iN;
						list[ic].node22 = -1;
						list[ic].node23 = -1;
						list[ic].node24 = -1;
						//list[ic].dS = dx*dz; // ������� ������.
						list[ic].dS1 = dx*dz;
						list[ic].dS2 = 0.0;
						list[ic].dS3 = 0.0;
						list[ic].dS4 = 0.0;
						ic++;
					}
					else
					{
						if (itek != whot_is_block[iN]) {
							list[ic].node1 = iP;
							list[ic].node21 = iN;
							list[ic].node22 = -1;
							list[ic].node23 = -1;
							list[ic].node24 = -1;
							//list[ic].dS = dx*dz; // ������� ������.
							list[ic].dS1 = dx*dz;
							list[ic].dS2 = 0.0;
							list[ic].dS3 = 0.0;
							list[ic].dS4 = 0.0;
							ic++;
						}
					}

				}
			}

			b[itek].radiation.nodelistN = new MY_PAIR[ic];
			b[itek].radiation.nodelistNsize = ic;
			for (integer j = 0; j < ic; j++) b[itek].radiation.nodelistN[j] = list[j];

			delete[] list;
			list = NULL;


			list = new MY_PAIR[maxelm];
			ic = 0;

			for (integer iP = 0; iP < maxelm; iP++) {
				integer ib = whot_is_block[iP];
				if (ib == itek)
				{
					integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
					iE = sosedi[ESIDE][iP].iNODE1; iN = sosedi[NSIDE][iP].iNODE1; iT = sosedi[TSIDE][iP].iNODE1; iW = sosedi[WSIDE][iP].iNODE1; iS = sosedi[SSIDE][iP].iNODE1; iB = sosedi[BSIDE][iP].iNODE1;

					// ���������� �������� �������� ������������ ������:
					doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
					volume3D(iP, nvtx, pa, dx, dy, dz);

					if (iB >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node21 = iB;
						list[ic].node22 = -1;
						list[ic].node23 = -1;
						list[ic].node24 = -1;
						//list[ic].dS = dx*dy; // ������� ������.
						list[ic].dS1 = dx*dy;
						list[ic].dS2 = 0.0;
						list[ic].dS3 = 0.0;
						list[ic].dS4 = 0.0;
						ic++;
					}
					else
					{
						if (itek != whot_is_block[iB]) {
							list[ic].node1 = iP;
							list[ic].node21 = iB;
							list[ic].node22 = -1;
							list[ic].node23 = -1;
							list[ic].node24 = -1;
							//list[ic].dS = dx*dy; // ������� ������.
							list[ic].dS1 = dx*dy;
							list[ic].dS2 = 0.0;
							list[ic].dS3 = 0.0;
							list[ic].dS4 = 0.0;
							ic++;
						}
					}

				}
			}

			b[itek].radiation.nodelistB = new MY_PAIR[ic];
			b[itek].radiation.nodelistBsize = ic;
			for (integer j = 0; j < ic; j++) b[itek].radiation.nodelistB[j] = list[j];

			delete[] list;
			list = NULL;

			list = new MY_PAIR[maxelm];
			ic = 0;

			for (integer iP = 0; iP < maxelm; iP++) {
				integer ib = whot_is_block[iP];
				if (ib == itek)
				{
					integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
					iE = sosedi[ESIDE][iP].iNODE1; iN = sosedi[NSIDE][iP].iNODE1; iT = sosedi[TSIDE][iP].iNODE1; iW = sosedi[WSIDE][iP].iNODE1; iS = sosedi[SSIDE][iP].iNODE1; iB = sosedi[BSIDE][iP].iNODE1;


					// ���������� �������� �������� ������������ ������:
					doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
					volume3D(iP, nvtx, pa, dx, dy, dz);

					if (iT >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node21 = iT;
						list[ic].node22 = -1;
						list[ic].node23 = -1;
						list[ic].node24 = -1;
						//list[ic].dS = dx*dy; // ������� ������.
						list[ic].dS1 = dx*dy;
						list[ic].dS2 = 0.0;
						list[ic].dS3 = 0.0;
						list[ic].dS4 = 0.0;
						ic++;
					}
					else
					{
						if (itek != whot_is_block[iT]) {
							list[ic].node1 = iP;
							list[ic].node21 = iT;
							list[ic].node22 = -1;
							list[ic].node23 = -1;
							list[ic].node24 = -1;
							//list[ic].dS = dx*dy; // ������� ������.
							list[ic].dS1 = dx*dy;
							list[ic].dS2 = 0.0;
							list[ic].dS3 = 0.0;
							list[ic].dS4 = 0.0;
							ic++;
						}
					}

				}
			}

			b[itek].radiation.nodelistT = new MY_PAIR[ic];
			b[itek].radiation.nodelistTsize = ic;
			for (integer j = 0; j < ic; j++) b[itek].radiation.nodelistT[j] = list[j];

			delete[] list;
			list = NULL;


		}
	}
} // constr_link_on_surface_for_radiation_model


// ������ ��������� 14 �������� 2016. 15_06.
// ����������� ��������� 20 �������� 2016. 14_45.
// ����� ���������� ������ ��� � ������ my_solver_v0_03.cpp,
// � � ������ my_elmatr_quad.cpp
// ������ ����������� ����� ��� ������ ��������� ������ �����.
void constr_link_on_surface_for_radiation_model_alice(integer maxelm, BOUND* &sosedb, integer* &whot_is_block, ALICE_PARTITION** sosedi,
	integer** nvtx, TOCHKA* pa, BLOCK* &b, integer lb)
{
	for (integer i = 0; i < lb; i++) {
		if (b[i].radiation.binternalRadiation) {
			integer itek = i;

			MY_PAIR* list = new MY_PAIR[maxelm];
			integer ic = 0;



			for (integer iP = 0; iP < maxelm; iP++) {
				integer ib = whot_is_block[iP];
				if (ib == itek)
				{
					integer iE1, iN1, iT1, iW1, iS1, iB1; // ������ �������� ����������� �������
					integer iE2, iN2, iT2, iW2, iS2, iB2;
					integer iE3, iN3, iT3, iW3, iS3, iB3;
					integer iE4, iN4, iT4, iW4, iS4, iB4;

					// -1 ���� ���� �� ����������.
					// 0 .. maxelm-1 - ������ ���������� ����.
					// maxelm .. maxelm + maxbound-1 - ��������� ����.

					iE1 = sosedi[ESIDE][iP].iNODE1; iN1 = sosedi[NSIDE][iP].iNODE1; iT1 = sosedi[TSIDE][iP].iNODE1; 
					iW1 = sosedi[WSIDE][iP].iNODE1; iS1 = sosedi[SSIDE][iP].iNODE1; iB1 = sosedi[BSIDE][iP].iNODE1;

					iE2 = sosedi[ESIDE][iP].iNODE2; iN2 = sosedi[NSIDE][iP].iNODE2; iT2 = sosedi[TSIDE][iP].iNODE2;
					iW2 = sosedi[WSIDE][iP].iNODE2; iS2 = sosedi[SSIDE][iP].iNODE2; iB2 = sosedi[BSIDE][iP].iNODE2;

					iE3 = sosedi[ESIDE][iP].iNODE3; iN3 = sosedi[NSIDE][iP].iNODE3; iT3 = sosedi[TSIDE][iP].iNODE3;
					iW3 = sosedi[WSIDE][iP].iNODE3; iS3 = sosedi[SSIDE][iP].iNODE3; iB3 = sosedi[BSIDE][iP].iNODE3;

					iE4 = sosedi[ESIDE][iP].iNODE4; iN4 = sosedi[NSIDE][iP].iNODE4; iT4 = sosedi[TSIDE][iP].iNODE4;
					iW4 = sosedi[WSIDE][iP].iNODE4; iS4 = sosedi[SSIDE][iP].iNODE4; iB4 = sosedi[BSIDE][iP].iNODE4;

					// ���������� �������� �������� ������������ ������:
					doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
					volume3D(iP, nvtx, pa, dx, dy, dz);

					bool binc = false;
					list[ic].node21 = -1;
					list[ic].node22 = -1;
					list[ic].node23 = -1;
					list[ic].node24 = -1;
					list[ic].dS1 = 0.0;
					list[ic].dS2 = 0.0;
					list[ic].dS3 = 0.0;
					list[ic].dS4 = 0.0;

					if (iW1 == -1) {
						printf("iW1 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iW1 >= maxelm) {
						// ��������� ����.
						list[ic].node1 = iP;
						list[ic].node21 = iW1;
						//list[ic].dS = dy*dz; // ������� ������.
						//list[ic].dS1 = dy*dz; // ������� ������.
						list[ic].dS1 = sosedb[iW1 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						// ���������� �����, �� ������ ����������� ������ ������.
						if (itek != whot_is_block[iW1]) {
							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iW1, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node21 = iW1;
							//list[ic].dS = dy*dz; // ������� ������.
							list[ic].dS1 = dy_loc*dz_loc; // ������� ������.
							binc = true;
						}
					}

					if (iW2 == -1) {
						printf("iW2 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iW2 >= maxelm) {
						// ��������� ����.
						list[ic].node1 = iP;
						list[ic].node22 = iW2;
						//list[ic].dS = dy*dz; // ������� ������.
						//list[ic].dS2 = dy*dz; // ������� ������.
						list[ic].dS2 = sosedb[iW2 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						// ���������� �����, �� ������ ����������� ������ ������.
						if (itek != whot_is_block[iW2]) {
							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iW2, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node22 = iW2;
							//list[ic].dS = dy*dz; // ������� ������.
							list[ic].dS2 = dy_loc*dz_loc; // ������� ������.
							binc = true;
						}
					}

					if (iW3 == -1) {
						printf("iW3 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iW3 >= maxelm) {
						// ��������� ����.
						list[ic].node1 = iP;
						list[ic].node23 = iW3;
						//list[ic].dS = dy*dz; // ������� ������.
						//list[ic].dS3 = dy*dz; // ������� ������.
						list[ic].dS3 = sosedb[iW3 - maxelm].dS; // ������� ������.

						binc = true;
					}
					else
					{
						// ���������� �����, �� ������ ����������� ������ ������.
						if (itek != whot_is_block[iW3]) {
							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iW3, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node23 = iW3;
							//list[ic].dS = dy*dz; // ������� ������.
							list[ic].dS3 = dy_loc*dz_loc; // ������� ������.
							binc = true;
						}
					}

					if (iW4 == -1) {
						printf("iW4 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iW4 >= maxelm) {
						// ��������� ����.
						list[ic].node1 = iP;
						list[ic].node24 = iW4;
						//list[ic].dS = dy*dz; // ������� ������.
						//list[ic].dS4 = dy*dz; // ������� ������.
						list[ic].dS4 = sosedb[iW4 - maxelm].dS; // ������� ������.

						binc = true;
					}
					else
					{
						// ���������� �����, �� ������ ����������� ������ ������.
						if (itek != whot_is_block[iW4]) {
							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iW4, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node24 = iW4;
							//list[ic].dS = dy*dz; // ������� ������.
							list[ic].dS4 = dy_loc*dz_loc; // ������� ������.
							binc = true;
						}
					}

					if (binc) {
						ic++;
					}
				}
			}

			b[itek].radiation.nodelistW = new MY_PAIR[ic];
			b[itek].radiation.nodelistWsize = ic;
			for (integer j = 0; j < ic; j++) b[itek].radiation.nodelistW[j] = list[j];

			delete[] list;
			list = NULL;

			list = new MY_PAIR[maxelm];
			ic = 0;

			for (integer iP = 0; iP < maxelm; iP++) {
				integer ib = whot_is_block[iP];
				if (ib == itek)
				{
				

					integer iE1, iN1, iT1, iW1, iS1, iB1; // ������ �������� ����������� �������
					integer iE2, iN2, iT2, iW2, iS2, iB2;
					integer iE3, iN3, iT3, iW3, iS3, iB3;
					integer iE4, iN4, iT4, iW4, iS4, iB4;

					// -1 ���� ���� �� ����������.
					// 0 .. maxelm-1 - ������ ���������� ����.
					// maxelm .. maxelm + maxbound-1 - ��������� ����.

					iE1 = sosedi[ESIDE][iP].iNODE1; iN1 = sosedi[NSIDE][iP].iNODE1; iT1 = sosedi[TSIDE][iP].iNODE1;
					iW1 = sosedi[WSIDE][iP].iNODE1; iS1 = sosedi[SSIDE][iP].iNODE1; iB1 = sosedi[BSIDE][iP].iNODE1;

					iE2 = sosedi[ESIDE][iP].iNODE2; iN2 = sosedi[NSIDE][iP].iNODE2; iT2 = sosedi[TSIDE][iP].iNODE2;
					iW2 = sosedi[WSIDE][iP].iNODE2; iS2 = sosedi[SSIDE][iP].iNODE2; iB2 = sosedi[BSIDE][iP].iNODE2;

					iE3 = sosedi[ESIDE][iP].iNODE3; iN3 = sosedi[NSIDE][iP].iNODE3; iT3 = sosedi[TSIDE][iP].iNODE3;
					iW3 = sosedi[WSIDE][iP].iNODE3; iS3 = sosedi[SSIDE][iP].iNODE3; iB3 = sosedi[BSIDE][iP].iNODE3;

					iE4 = sosedi[ESIDE][iP].iNODE4; iN4 = sosedi[NSIDE][iP].iNODE4; iT4 = sosedi[TSIDE][iP].iNODE4;
					iW4 = sosedi[WSIDE][iP].iNODE4; iS4 = sosedi[SSIDE][iP].iNODE4; iB4 = sosedi[BSIDE][iP].iNODE4;


					bool binc = false;
					list[ic].node21 = -1;
					list[ic].node22 = -1;
					list[ic].node23 = -1;
					list[ic].node24 = -1;
					list[ic].dS1 = 0.0;
					list[ic].dS2 = 0.0;
					list[ic].dS3 = 0.0;
					list[ic].dS4 = 0.0;

					// ���������� �������� �������� ������������ ������:
					doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
					volume3D(iP, nvtx, pa, dx, dy, dz);

					if (iE1 == -1) {
						printf("iE1 == -1\n");
						//getchar();
						system("PAUSE");
					}


					if (iE1 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node21 = iE1;
						//list[ic].dS = dy*dz; // ������� ������.
						//list[ic].dS1 = dy*dz; // ������� ������.
						list[ic].dS1 = sosedb[iE1 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iE1]) {

							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iE1, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node21 = iE1;
							//list[ic].dS = dy*dz; // ������� ������.
							list[ic].dS1 = dy_loc*dz_loc; // ������� ������.
							binc = true;
						}
					}

					if (iE2 == -1) {
						printf("iE2 == -1\n");
						//getchar();
						system("PAUSE");
					}


					if (iE2 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node22 = iE2;
						//list[ic].dS = dy*dz; // ������� ������.
						//list[ic].dS2 = dy*dz; // ������� ������.
						list[ic].dS2 = sosedb[iE2 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iE2]) {

							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iE2, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node22 = iE2;
							//list[ic].dS = dy*dz; // ������� ������.
							list[ic].dS2 = dy_loc*dz_loc; // ������� ������.
							binc = true;
						}
					}

					if (iE3 == -1) {
						printf("iE3 == -1\n");
						//getchar();
						system("PAUSE");
					}


					if (iE3 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node23 = iE3;
						//list[ic].dS = dy*dz; // ������� ������.
						//list[ic].dS3 = dy*dz; // ������� ������.
						list[ic].dS3 = sosedb[iE3 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iE3]) {

							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iE3, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node23 = iE3;
							//list[ic].dS = dy*dz; // ������� ������.
							list[ic].dS3 = dy_loc*dz_loc; // ������� ������.
							binc = true;
						}
					}

					if (iE4 == -1) {
						printf("iE4 == -1\n");
						//getchar();
						system("PAUSE");
					}


					if (iE4 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node24 = iE4;
						//list[ic].dS = dy*dz; // ������� ������.
						//list[ic].dS4 = dy*dz; // ������� ������.
						list[ic].dS4 = sosedb[iE4 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iE4]) {

							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iE4, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node24 = iE4;
							//list[ic].dS = dy*dz; // ������� ������.
							list[ic].dS4 = dy_loc*dz_loc; // ������� ������
							binc = true;
						}
					}

					if (binc) {
						ic++;
					}

				}
			}

			b[itek].radiation.nodelistE = new MY_PAIR[ic];
			b[itek].radiation.nodelistEsize = ic;
			for (integer j = 0; j < ic; j++) b[itek].radiation.nodelistE[j] = list[j];

			delete[] list;
			list = NULL;

			list = new MY_PAIR[maxelm];
			ic = 0;

			for (integer iP = 0; iP < maxelm; iP++) {
				integer ib = whot_is_block[iP];
				if (ib == itek)
				{
					
					integer iE1, iN1, iT1, iW1, iS1, iB1; // ������ �������� ����������� �������
					integer iE2, iN2, iT2, iW2, iS2, iB2;
					integer iE3, iN3, iT3, iW3, iS3, iB3;
					integer iE4, iN4, iT4, iW4, iS4, iB4;

					// -1 ���� ���� �� ����������.
					// 0 .. maxelm-1 - ������ ���������� ����.
					// maxelm .. maxelm + maxbound-1 - ��������� ����.

					iE1 = sosedi[ESIDE][iP].iNODE1; iN1 = sosedi[NSIDE][iP].iNODE1; iT1 = sosedi[TSIDE][iP].iNODE1;
					iW1 = sosedi[WSIDE][iP].iNODE1; iS1 = sosedi[SSIDE][iP].iNODE1; iB1 = sosedi[BSIDE][iP].iNODE1;

					iE2 = sosedi[ESIDE][iP].iNODE2; iN2 = sosedi[NSIDE][iP].iNODE2; iT2 = sosedi[TSIDE][iP].iNODE2;
					iW2 = sosedi[WSIDE][iP].iNODE2; iS2 = sosedi[SSIDE][iP].iNODE2; iB2 = sosedi[BSIDE][iP].iNODE2;

					iE3 = sosedi[ESIDE][iP].iNODE3; iN3 = sosedi[NSIDE][iP].iNODE3; iT3 = sosedi[TSIDE][iP].iNODE3;
					iW3 = sosedi[WSIDE][iP].iNODE3; iS3 = sosedi[SSIDE][iP].iNODE3; iB3 = sosedi[BSIDE][iP].iNODE3;

					iE4 = sosedi[ESIDE][iP].iNODE4; iN4 = sosedi[NSIDE][iP].iNODE4; iT4 = sosedi[TSIDE][iP].iNODE4;
					iW4 = sosedi[WSIDE][iP].iNODE4; iS4 = sosedi[SSIDE][iP].iNODE4; iB4 = sosedi[BSIDE][iP].iNODE4;


					bool binc = false;
					list[ic].node21 = -1;
					list[ic].node22 = -1;
					list[ic].node23 = -1;
					list[ic].node24 = -1;
					list[ic].dS1 = 0.0;
					list[ic].dS2 = 0.0;
					list[ic].dS3 = 0.0;
					list[ic].dS4 = 0.0;

					// ���������� �������� �������� ������������ ������:
					doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
					volume3D(iP, nvtx, pa, dx, dy, dz);

					if (iS1 == -1) {
						printf("iS1 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iS1 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node21 = iS1;
						//list[ic].dS = dx*dz; // ������� ������.
						//list[ic].dS1 = dx*dz; // ������� ������.
						list[ic].dS1 = sosedb[iS1 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iS1]) {

							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iS1, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node21 = iS1;
							//list[ic].dS = dx*dz; // ������� ������.
							list[ic].dS1 = dx_loc*dz_loc; // ������� ������.
							binc = true;
						}
					}

					if (iS2 == -1) {
						printf("iS2 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iS2 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node22 = iS2;
						//list[ic].dS = dx*dz; // ������� ������.
						//list[ic].dS2 = dx*dz; // ������� ������.
						list[ic].dS2 = sosedb[iS2 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iS2]) {
							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iS2, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node22 = iS2;
							//list[ic].dS = dx*dz; // ������� ������.
							list[ic].dS2 = dx_loc*dz_loc; // ������� ������.
							binc = true;
						}
					}

					if (iS3 == -1) {
						printf("iS3 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iS3 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node23 = iS3;
						//list[ic].dS = dx*dz; // ������� ������.
						//list[ic].dS3 = dx*dz; // ������� ������.
						list[ic].dS3 = sosedb[iS3 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iS3]) {
							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iS3, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node23 = iS3;
							//list[ic].dS = dx*dz; // ������� ������.
							list[ic].dS3 = dx_loc*dz_loc; // ������� ������.
							binc = true;
						}
					}

					if (iS4 == -1) {
						printf("iS4 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iS4 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node24 = iS4;
						//list[ic].dS = dx*dz; // ������� ������.
						//list[ic].dS4 = dx*dz; // ������� ������.
						list[ic].dS4 = sosedb[iS4 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iS4]) {
							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iS4, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node24 = iS4;
							//list[ic].dS = dx*dz; // ������� ������.
							list[ic].dS4 = dx_loc*dz_loc; // ������� ������.
							binc = true;
						}
					}

					if (binc) {
						ic++;
					}

				}
			}

			b[itek].radiation.nodelistS = new MY_PAIR[ic];
			b[itek].radiation.nodelistSsize = ic;
			for (integer j = 0; j < ic; j++) b[itek].radiation.nodelistS[j] = list[j];

			delete[] list;
			list = NULL;

			list = new MY_PAIR[maxelm];
			ic = 0;

			for (integer iP = 0; iP < maxelm; iP++) {
				integer ib = whot_is_block[iP];
				if (ib == itek)
				{
					
					integer iE1, iN1, iT1, iW1, iS1, iB1; // ������ �������� ����������� �������
					integer iE2, iN2, iT2, iW2, iS2, iB2;
					integer iE3, iN3, iT3, iW3, iS3, iB3;
					integer iE4, iN4, iT4, iW4, iS4, iB4;

					// -1 ���� ���� �� ����������.
					// 0 .. maxelm-1 - ������ ���������� ����.
					// maxelm .. maxelm + maxbound-1 - ��������� ����.

					iE1 = sosedi[ESIDE][iP].iNODE1; iN1 = sosedi[NSIDE][iP].iNODE1; iT1 = sosedi[TSIDE][iP].iNODE1;
					iW1 = sosedi[WSIDE][iP].iNODE1; iS1 = sosedi[SSIDE][iP].iNODE1; iB1 = sosedi[BSIDE][iP].iNODE1;

					iE2 = sosedi[ESIDE][iP].iNODE2; iN2 = sosedi[NSIDE][iP].iNODE2; iT2 = sosedi[TSIDE][iP].iNODE2;
					iW2 = sosedi[WSIDE][iP].iNODE2; iS2 = sosedi[SSIDE][iP].iNODE2; iB2 = sosedi[BSIDE][iP].iNODE2;

					iE3 = sosedi[ESIDE][iP].iNODE3; iN3 = sosedi[NSIDE][iP].iNODE3; iT3 = sosedi[TSIDE][iP].iNODE3;
					iW3 = sosedi[WSIDE][iP].iNODE3; iS3 = sosedi[SSIDE][iP].iNODE3; iB3 = sosedi[BSIDE][iP].iNODE3;

					iE4 = sosedi[ESIDE][iP].iNODE4; iN4 = sosedi[NSIDE][iP].iNODE4; iT4 = sosedi[TSIDE][iP].iNODE4;
					iW4 = sosedi[WSIDE][iP].iNODE4; iS4 = sosedi[SSIDE][iP].iNODE4; iB4 = sosedi[BSIDE][iP].iNODE4;


					bool binc = false;
					list[ic].node21 = -1;
					list[ic].node22 = -1;
					list[ic].node23 = -1;
					list[ic].node24 = -1;
					list[ic].dS1 = 0.0;
					list[ic].dS2 = 0.0;
					list[ic].dS3 = 0.0;
					list[ic].dS4 = 0.0;


					// ���������� �������� �������� ������������ ������:
					doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
					volume3D(iP, nvtx, pa, dx, dy, dz);


					if (iN1 == -1) {
						printf("iN1 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iN1 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node21 = iN1;
						//list[ic].dS = dx*dz; // ������� ������.
						//list[ic].dS1 = dx*dz; // ������� ������.
						list[ic].dS1 = sosedb[iN1 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iN1]) {
							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iN1, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node21 = iN1;
							//list[ic].dS = dx*dz; // ������� ������.
							list[ic].dS1 = dx_loc*dz_loc; // ������� ������.
							binc = true;
						}
					}

					if (iN2 == -1) {
						printf("iN2 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iN2 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node22 = iN2;
						//list[ic].dS = dx*dz; // ������� ������.
						//list[ic].dS2 = dx*dz; // ������� ������.
						list[ic].dS2 = sosedb[iN2 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iN2]) {
							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iN2, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node22 = iN2;
							//list[ic].dS = dx*dz; // ������� ������.
							list[ic].dS2 = dx_loc*dz_loc; // ������� ������.
							binc = true;
						}
					}

					if (iN3 == -1) {
						printf("iN3 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iN3 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node23 = iN3;
						//list[ic].dS = dx*dz; // ������� ������.
						//list[ic].dS3 = dx*dz; // ������� ������.
						list[ic].dS3 = sosedb[iN3 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iN3]) {
							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iN3, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node23 = iN3;
							//list[ic].dS = dx*dz; // ������� ������.
							list[ic].dS3 = dx_loc*dz_loc; // ������� ������.
							binc = true;
						}
					}

					if (iN4 == -1) {
						printf("iN4 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iN4 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node24 = iN4;
						//list[ic].dS = dx*dz; // ������� ������.
						//list[ic].dS4 = dx*dz; // ������� ������.
						list[ic].dS4 = sosedb[iN4 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iN4]) {

							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iN4, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node24 = iN4;
							//list[ic].dS = dx*dz; // ������� ������.
							list[ic].dS4 = dx_loc*dz_loc; // ������� ������.
							binc = true;
						}
					}

					if (binc) {
						ic++;
					}

				}
			}

			b[itek].radiation.nodelistN = new MY_PAIR[ic];
			b[itek].radiation.nodelistNsize = ic;
			for (integer j = 0; j < ic; j++) b[itek].radiation.nodelistN[j] = list[j];

			delete[] list;
			list = NULL;


			list = new MY_PAIR[maxelm];
			ic = 0;

			for (integer iP = 0; iP < maxelm; iP++) {
				integer ib = whot_is_block[iP];
				if (ib == itek)
				{
				
					integer iE1, iN1, iT1, iW1, iS1, iB1; // ������ �������� ����������� �������
					integer iE2, iN2, iT2, iW2, iS2, iB2;
					integer iE3, iN3, iT3, iW3, iS3, iB3;
					integer iE4, iN4, iT4, iW4, iS4, iB4;

					// -1 ���� ���� �� ����������.
					// 0 .. maxelm-1 - ������ ���������� ����.
					// maxelm .. maxelm + maxbound-1 - ��������� ����.

					iE1 = sosedi[ESIDE][iP].iNODE1; iN1 = sosedi[NSIDE][iP].iNODE1; iT1 = sosedi[TSIDE][iP].iNODE1;
					iW1 = sosedi[WSIDE][iP].iNODE1; iS1 = sosedi[SSIDE][iP].iNODE1; iB1 = sosedi[BSIDE][iP].iNODE1;

					iE2 = sosedi[ESIDE][iP].iNODE2; iN2 = sosedi[NSIDE][iP].iNODE2; iT2 = sosedi[TSIDE][iP].iNODE2;
					iW2 = sosedi[WSIDE][iP].iNODE2; iS2 = sosedi[SSIDE][iP].iNODE2; iB2 = sosedi[BSIDE][iP].iNODE2;

					iE3 = sosedi[ESIDE][iP].iNODE3; iN3 = sosedi[NSIDE][iP].iNODE3; iT3 = sosedi[TSIDE][iP].iNODE3;
					iW3 = sosedi[WSIDE][iP].iNODE3; iS3 = sosedi[SSIDE][iP].iNODE3; iB3 = sosedi[BSIDE][iP].iNODE3;

					iE4 = sosedi[ESIDE][iP].iNODE4; iN4 = sosedi[NSIDE][iP].iNODE4; iT4 = sosedi[TSIDE][iP].iNODE4;
					iW4 = sosedi[WSIDE][iP].iNODE4; iS4 = sosedi[SSIDE][iP].iNODE4; iB4 = sosedi[BSIDE][iP].iNODE4;


					bool binc = false;
					list[ic].node21 = -1;
					list[ic].node22 = -1;
					list[ic].node23 = -1;
					list[ic].node24 = -1;
					list[ic].dS1 = 0.0;
					list[ic].dS2 = 0.0;
					list[ic].dS3 = 0.0;
					list[ic].dS4 = 0.0;


					// ���������� �������� �������� ������������ ������:
					doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
					volume3D(iP, nvtx, pa, dx, dy, dz);

					if (iB1 == -1) {
						printf("iB1 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iB1 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node21 = iB1;
						//list[ic].dS1 = dx*dy; // ������� ������.
						list[ic].dS1 = sosedb[iB1 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iB1]) {
							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iB1, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node21 = iB1;
							list[ic].dS1 = dx_loc*dy_loc; // ������� ������.
							binc = true;
						}
					}

					if (iB2 == -1) {
						printf("iB2 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iB2 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node22 = iB2;
						//list[ic].dS2 = dx*dy; // ������� ������.
						list[ic].dS2 = sosedb[iB2 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iB2]) {
							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iB2, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node22 = iB2;
							list[ic].dS2 = dx_loc*dy_loc; // ������� ������.
							binc = true;
						}
					}

					if (iB3 == -1) {
						printf("iB3 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iB3 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node23 = iB3;
						//list[ic].dS3 = dx*dy; // ������� ������.
						list[ic].dS3 = sosedb[iB3 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iB3]) {
							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iB3, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node23 = iB3;
							list[ic].dS3 = dx_loc*dy_loc; // ������� ������.
							binc = true;
						}
					}

					if (iB4 == -1) {
						printf("iB4 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iB4 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node24 = iB4;
						//list[ic].dS4 = dx*dy; // ������� ������.
						list[ic].dS4 = sosedb[iB4 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iB4]) {
							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iB4, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node24 = iB4;
							list[ic].dS4 = dx_loc*dy_loc; // ������� ������.
							binc = true;
						}
					}

					if (binc) {
						ic++;
					}

				}
			}

			b[itek].radiation.nodelistB = new MY_PAIR[ic];
			b[itek].radiation.nodelistBsize = ic;
			for (integer j = 0; j < ic; j++) b[itek].radiation.nodelistB[j] = list[j];

			delete[] list;
			list = NULL;

			list = new MY_PAIR[maxelm];
			ic = 0;

			for (integer iP = 0; iP < maxelm; iP++) {
				integer ib = whot_is_block[iP];
				if (ib == itek)
				{
				
					integer iE1, iN1, iT1, iW1, iS1, iB1; // ������ �������� ����������� �������
					integer iE2, iN2, iT2, iW2, iS2, iB2;
					integer iE3, iN3, iT3, iW3, iS3, iB3;
					integer iE4, iN4, iT4, iW4, iS4, iB4;

					// -1 ���� ���� �� ����������.
					// 0 .. maxelm-1 - ������ ���������� ����.
					// maxelm .. maxelm + maxbound-1 - ��������� ����.

					iE1 = sosedi[ESIDE][iP].iNODE1; iN1 = sosedi[NSIDE][iP].iNODE1; iT1 = sosedi[TSIDE][iP].iNODE1;
					iW1 = sosedi[WSIDE][iP].iNODE1; iS1 = sosedi[SSIDE][iP].iNODE1; iB1 = sosedi[BSIDE][iP].iNODE1;

					iE2 = sosedi[ESIDE][iP].iNODE2; iN2 = sosedi[NSIDE][iP].iNODE2; iT2 = sosedi[TSIDE][iP].iNODE2;
					iW2 = sosedi[WSIDE][iP].iNODE2; iS2 = sosedi[SSIDE][iP].iNODE2; iB2 = sosedi[BSIDE][iP].iNODE2;

					iE3 = sosedi[ESIDE][iP].iNODE3; iN3 = sosedi[NSIDE][iP].iNODE3; iT3 = sosedi[TSIDE][iP].iNODE3;
					iW3 = sosedi[WSIDE][iP].iNODE3; iS3 = sosedi[SSIDE][iP].iNODE3; iB3 = sosedi[BSIDE][iP].iNODE3;

					iE4 = sosedi[ESIDE][iP].iNODE4; iN4 = sosedi[NSIDE][iP].iNODE4; iT4 = sosedi[TSIDE][iP].iNODE4;
					iW4 = sosedi[WSIDE][iP].iNODE4; iS4 = sosedi[SSIDE][iP].iNODE4; iB4 = sosedi[BSIDE][iP].iNODE4;


					bool binc = false;
					list[ic].node21 = -1;
					list[ic].node22 = -1;
					list[ic].node23 = -1;
					list[ic].node24 = -1;
					list[ic].dS1 = 0.0;
					list[ic].dS2 = 0.0;
					list[ic].dS3 = 0.0;
					list[ic].dS4 = 0.0;

					// ���������� �������� �������� ������������ ������:
					doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
					volume3D(iP, nvtx, pa, dx, dy, dz);

					if (iT1 == -1) {
						printf("iT1 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iT1 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node21 = iT1;
						//list[ic].dS1 = dx*dy; // ������� ������.
						list[ic].dS1 = sosedb[iT1 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iT1]) {
							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iT1, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node21 = iT1;
							list[ic].dS1 = dx_loc*dy_loc; // ������� ������.
							binc = true;
						}
					}

					if (iT2 == -1) {
						printf("iT2 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iT2 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node22 = iT2;
						//list[ic].dS2 = dx*dy; // ������� ������.
						list[ic].dS2 = sosedb[iT2 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iT2]) {
							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iT2, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node22 = iT2;
							list[ic].dS2 = dx_loc*dy_loc; // ������� ������.
							binc = true;
						}
					}

					if (iT3 == -1) {
						printf("iT3 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iT3 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node23 = iT3;
						//list[ic].dS3 = dx*dy; // ������� ������.
						list[ic].dS3 = sosedb[iT3 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iT3]) {

							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iT3, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node23 = iT3;
							list[ic].dS3 = dx_loc*dy_loc; // ������� ������.
							binc = true;
						}
					}

					if (iT4 == -1) {
						printf("iT4 == -1\n");
						//getchar();
						system("PAUSE");
					}

					if (iT4 >= maxelm) {
						list[ic].node1 = iP;
						list[ic].node24 = iT4;
						//list[ic].dS4 = dx*dy; // ������� ������.
						list[ic].dS4 = sosedb[iT4 - maxelm].dS; // ������� ������.
						binc = true;
					}
					else
					{
						if (itek != whot_is_block[iT4]) {
							// ���������� �������� �������� ������������ ������:
							doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
							volume3D(iT4, nvtx, pa, dx_loc, dy_loc, dz_loc);

							list[ic].node1 = iP;
							list[ic].node24 = iT4;
							list[ic].dS4 = dx_loc*dy_loc; // ������� ������.
							binc = true;
						}
					}

					if (binc) {
						ic++;
					}

				}
			}

			b[itek].radiation.nodelistT = new MY_PAIR[ic];
			b[itek].radiation.nodelistTsize = ic;
			for (integer j = 0; j < ic; j++) b[itek].radiation.nodelistT[j] = list[j];

			delete[] list;
			list = NULL;


		}
	}
} // constr_link_on_surface_for_radiation_model_alice


  // ����������� ������ ������ 1
  // ��� ��������� �������������.
void free_level1_flow(FLOW* &fglobal, integer &flow_interior) {
	integer iflow = 0;
	for (integer iflow = 0; iflow<flow_interior; iflow++) {
		integer j = 0;
		if (fglobal != NULL) {

#if doubleintprecision == 1
			printf("delete flow %lld icolor_diferent_fluid_domain\n", iflow);
#else
			printf("delete flow %d icolor_diferent_fluid_domain\n", iflow);
#endif
			
			if (fglobal[iflow].icolor_different_fluid_domain != NULL) {
				delete fglobal[iflow].icolor_different_fluid_domain;
				fglobal[iflow].icolor_different_fluid_domain = NULL;
			}

#if doubleintprecision == 1
			printf("delete flow %lld sosedb\n", iflow);
#else
			printf("delete flow %d sosedb\n", iflow);
#endif
			
			if (fglobal[iflow].sosedb != NULL) {
				delete fglobal[iflow].sosedb;
				fglobal[iflow].sosedb = NULL;
			}

#if doubleintprecision == 1
			printf("delete flow %lld rdistWall\n", iflow);
#else
			printf("delete flow %d rdistWall\n", iflow);
#endif
			
			if (fglobal[iflow].rdistWall != NULL) { // -1N
				delete fglobal[iflow].rdistWall;
				fglobal[iflow].rdistWall = NULL;
			}


#if doubleintprecision == 1
			printf("delete flow %lld sosedi\n", iflow);
#else
			printf("delete flow %d sosedi\n", iflow);
#endif
			
			if (fglobal[iflow].sosedi != NULL) {
				for (integer i = 0; i<12; i++) {
					if (fglobal[iflow].sosedi[i] != NULL) {
						delete fglobal[iflow].sosedi[i]; // -12N
					}
				}
			}
			if (fglobal[iflow].sosedi != NULL) {
				delete fglobal[iflow].sosedi;
				fglobal[iflow].sosedi = NULL;
			}

#if doubleintprecision == 1
			printf("delete flow %lld nvtx\n", iflow);
#else
			printf("delete flow %d nvtx\n", iflow);
#endif
			
			for (integer i = 0; i<8; i++) { // -8N
#if doubleintprecision == 1
											//printf("%lld\n",i); getchar(); // debug
#else
											//printf("%d\n",i); getchar(); // debug
#endif
										
				if (fglobal[iflow].nvtx != NULL) {
					if (fglobal[iflow].nvtx[i] != NULL) {
						delete fglobal[iflow].nvtx[i];
					}
				}

			}
			if (fglobal[iflow].nvtx != NULL) {
				delete fglobal[iflow].nvtx;
				fglobal[iflow].nvtx = NULL;
			}

#if doubleintprecision == 1
			printf("delete flow %lld prop_b\n", iflow);
#else
			printf("delete flow %d prop_b\n", iflow);
#endif
			
			if (fglobal[iflow].prop_b != NULL) {
				for (integer i = 0; i<3; i++) {
					if (fglobal[iflow].prop_b[i] != NULL) {
						delete fglobal[iflow].prop_b[i]; // -3N
					}
				}
			}
			if (fglobal[iflow].prop_b != NULL) {
				delete fglobal[iflow].prop_b;
				fglobal[iflow].prop_b = NULL;
			}

#if doubleintprecision == 1
			printf("delete flow %lld prop\n", iflow);
#else
			printf("delete flow %d prop\n", iflow);
#endif
			
			if (fglobal[iflow].prop != NULL) {
				for (integer i = 0; i<3; i++) {
					if (fglobal[iflow].prop[i] != NULL) {
						delete fglobal[iflow].prop[i]; // -3N
					}
				}
			}
			if (fglobal[iflow].prop != NULL) {
				delete fglobal[iflow].prop;
				fglobal[iflow].prop = NULL;
			}

#if doubleintprecision == 1
			printf("delete flow %lld mf\n", iflow);
#else
			printf("delete flow %d mf\n", iflow);
#endif
			
			if (fglobal[iflow].mf != NULL) {
				for (integer i = 0; i<fglobal[iflow].maxelm; i++) {
					if (fglobal[iflow].mf[i] != NULL) {
						delete fglobal[iflow].mf[i]; // -6N
						fglobal[iflow].mf[i] = NULL;
					}
				}
			}
			if (fglobal[iflow].mf != NULL) {
				delete fglobal[iflow].mf;
				fglobal[iflow].mf = NULL;
			}

#if doubleintprecision == 1
			printf("delete flow %lld SInvariantStrainRateTensor\n", iflow);
#else
			printf("delete flow %d SInvariantStrainRateTensor\n", iflow);
#endif
			
			if (fglobal[iflow].SInvariantStrainRateTensor != NULL) { // -1N
				delete fglobal[iflow].SInvariantStrainRateTensor;
				fglobal[iflow].SInvariantStrainRateTensor = NULL;
			}

#if doubleintprecision == 1
			printf("delete flow %lld diag_coef\n", iflow);
#else
			printf("delete flow %d diag_coef\n", iflow);
#endif
			
			if (fglobal[iflow].diag_coef != NULL) {
				for (integer i = 0; i<3; i++) {
					if (fglobal[iflow].diag_coef[i] != NULL) {
						delete fglobal[iflow].diag_coef[i]; // -3N
						fglobal[iflow].diag_coef[i] = NULL;
					}
				}
			}
			if (fglobal[iflow].diag_coef != NULL) {
				delete fglobal[iflow].diag_coef;
				fglobal[iflow].diag_coef = NULL;
			}


#if doubleintprecision == 1
			printf("delete flow %lld pa\n", iflow);
#else
			printf("delete flow %d pa\n", iflow);
#endif
			
			if (fglobal[iflow].pa != NULL) { // -3N
				delete fglobal[iflow].pa;
				fglobal[iflow].pa = NULL;
			}

#if doubleintprecision == 1
			printf("delete flow %lld ptr\n", iflow);
#else
			printf("delete flow %d ptr\n", iflow);
#endif
			
			if (fglobal[iflow].ptr != NULL) { // -N
				delete[] fglobal[iflow].ptr;
				fglobal[iflow].ptr = NULL;
			}

#if doubleintprecision == 1
			printf("delete flow %lld iN\n", iflow);
#else
			printf("delete flow %d iN\n", iflow);
#endif
			
			if (fglobal[iflow].iN != NULL) {
				for (integer i = 0; i<3; i++) {
					if (fglobal[iflow].iN[i] != NULL) {
						delete fglobal[iflow].iN[i]; // -max3(iWE, iSN, iBT)
						fglobal[iflow].iN[i] = NULL;
					}
				}
			}
			if (fglobal[iflow].iN != NULL) {
				delete fglobal[iflow].iN;
				fglobal[iflow].iN = NULL;
			}

#if doubleintprecision == 1
			printf("delete flow %lld id\n", iflow);
#else
			printf("delete flow %d id\n", iflow);
#endif
			
			if (fglobal[iflow].id != NULL) {
				for (integer i = 0; i<3; i++) {
					if (fglobal[iflow].id[i] != NULL) {
						integer n1 = 0;
						switch (i) {
						case 0: n1 = fglobal[iflow].iWE;  break;
						case 1: n1 = fglobal[iflow].iSN;  break;
						case 2: n1 = fglobal[iflow].iBT;  break;
						}
						for (integer j = 0; j<n1; j++) {
							if (fglobal[iflow].id[i][j] != NULL) {
								delete fglobal[iflow].id[i][j];
								fglobal[iflow].id[i][j] = NULL;
							}
						}
					}
				}
			}
			if (fglobal[iflow].id != NULL) {
				for (integer i = 0; i<3; i++) {
					if (fglobal[iflow].id[i] != NULL) {
						delete fglobal[iflow].id[i]; // 
						fglobal[iflow].id[i] = NULL;
					}
				}
			}
			if (fglobal[iflow].id != NULL) {
				delete fglobal[iflow].id;
				fglobal[iflow].id = NULL;
			}

			printf("please, press any key to continue...\n");
		}
	}
} // free_level1_flow


  // ����������� ������ ������ 2
  // ��� ��������� �������������.
void free_level2_flow(FLOW* &fglobal, integer &flow_interior) {
	integer iflow = 0;
	for (integer iflow = 0; iflow<flow_interior; iflow++) {
		integer j = 0;
		if (fglobal != NULL) {

#if doubleintprecision == 1
			printf("delete flow%lld slau\n", iflow);
#else
			printf("delete flow%d slau\n", iflow);
#endif
			
			if (fglobal[iflow].slau != NULL) {
				for (j = 0; j<5; j++) {
					if (fglobal[iflow].slau[j] != NULL) {
						delete fglobal[iflow].slau[j];
						fglobal[iflow].slau[j] = NULL;
					}
				}
				delete fglobal[iflow].slau;
				fglobal[iflow].slau = NULL;
			}

#if doubleintprecision == 1
			printf("delete flow%lld slau_bon\n", iflow);
#else
			printf("delete flow%d slau_bon\n", iflow);
#endif
			
			if (fglobal[iflow].slau_bon != NULL) {
				for (j = 0; j<5; j++) {
					if (fglobal[iflow].slau_bon[j] != NULL) {
						delete fglobal[iflow].slau_bon[j];
						fglobal[iflow].slau_bon[j] = NULL;
					}
				}
				delete fglobal[iflow].slau_bon;
				fglobal[iflow].slau_bon = NULL;
			}

#if doubleintprecision == 1
			printf("delete fluid %lld potent\n", iflow);
#else
			printf("delete fluid %d potent\n", iflow);
#endif
		
			if (fglobal[iflow].potent != NULL) { // -26N
				for (integer i = 0; i<27; i++) {
					if (fglobal[iflow].potent[i] != NULL) {
						delete fglobal[iflow].potent[i];
						fglobal[iflow].potent[i] = NULL;
					}
				}
				delete fglobal[iflow].potent;
				fglobal[iflow].potent = NULL;
			}

#if doubleintprecision == 1
			printf("delete flow %lld alpha\n", iflow);
#else
			printf("delete flow %d alpha\n", iflow);
#endif
			
			if (fglobal[iflow].alpha != NULL) { // -4
				delete fglobal[iflow].alpha;
				fglobal[iflow].alpha = NULL;
			}

		}

	}
	//flow_interior=0;
} // free_level2_flow


/* ��������� ��������� ������ ��� ������ ���������������� TEMPER
* � ��������� ������ ��� ������ ������������� i*FLOW. 
* �������� ��������: 
* 1. ������� ������������� ������ ���, �.�.
*    ����� ������ SOLID.
* 2. ����� ������������� ���� �� �� ��������, �.�. �������
*    ����������������� ���������� ���� �������. � ���� ������
*    ����� ����������� ������� ��� ������ �� �����. ���� 
*    ������ ��������� ����� ��� ����������� ��������� � ����� 
*    ��� ������������ ���������.
* 3. ���� ����� � ����������� ���������� �� ������ �������� 
*    � ������ �������.  
* 4. ���� ����� � ������������ ����������. ����� ��������� 
*    ������������������� ������.
*/
void load_TEMPER_and_FLOW(TEMPER &t, FLOW* &f, integer &inx, integer &iny, integer &inz,
				 doublereal* &xpos, doublereal* &ypos, doublereal* &zpos, integer &flow_interior,
				 BLOCK* b, integer lb, integer lw, WALL* w, SOURCE* s, integer ls, 
	             integer lu, UNION* &my_union, doublereal temp_ref,
				 TPROP* matlist,  bool bextendedprint,
				 doublereal dgx, doublereal dgy, doublereal dgz, bool bALICEflag,
	             bool breconstruct, integer &iunion_id_p1) {

	// eqin - ���������� � �������� ������ ���������.

	integer ipolygon_count_limit = 0;

	integer iPnodes_count_shadow = 0, inx_shadow = 0, iny_shadow = 0, inz_shadow = 0;

	//�������������.
	inx_shadow = inx;
	iny_shadow = iny;
	inz_shadow = inz;
	iPnodes_count_shadow = iPnodes_count_shadow_memo;

	RESTART_CONSTRUCT : // ������������ � ������������ ���� �������� ������:

	ipolygon_count_limit++;

#if doubleintprecision == 1
	printf("inx=%lld, iny=%lld, inz=%lld. Nodes=%lld\n", inx + 1, iny + 1, inz + 1, (inx + 1)*(iny + 1)*(inz + 1));
#else
	printf("inx=%d, iny=%d, inz=%d. Nodes=%d\n", inx + 1, iny + 1, inz + 1, (inx + 1)*(iny + 1)*(inz + 1));
#endif
    
	inx_shadow = inx;
	iny_shadow = iny;
	inz_shadow = inz;


	//for (integer i_53 = 0; i_53 <= inz; i_53++) {
#if doubleintprecision == 1
	//printf("i=%lld zpos[%lld]=%e ", i_53, i_53, zpos[i_53]);
#else
	//printf("i=%d zpos[%d]=%e ", i_53, i_53, zpos[i_53]);
#endif
		
		//if (i_53 < inz) printf("h=%e",zpos[i_53+1]-zpos[i_53]);
		//printf("\n");
	//}
	//system("PAUSE");

	// ���������������� :
	t.ilevel_alice = NULL;

    integer *evt_t=NULL; // ���������� ��������� ����������� �������
	TOCKA_INT* tck_int_list = NULL;

	if (!bALICEflag) {
		enumerate_volume(evt_t, t.maxelm, TEMPERATURE, xpos, ypos, zpos, t.whot_is_block, inx, iny, inz, b, lb, lu, my_union, iunion_id_p1, tck_int_list);
		t.ilevel_alice = new integer[t.maxelm];
		// ��� ���������� ����� ����� ���� � ���-�� ������� - ������.
		for (integer i_3 = 0; i_3 < t.maxelm; i_3++) {
			t.ilevel_alice[i_3] = 1; 
		}
	}
	else {
		// evt_t �����������.
		// ��������� t.maxelm, t.whot_is_block. ������ enumerate_volume.
		// ent_t �����������.
		// pa[0..maxnod-1]
		// ��������� t.pa, t.maxnod. ������ constr_nodes.
		// nvtx[0..7][0..maxelm-1]
		// ��������� t.nvtx. ������ constr_nvtx.
		// ��������� t.prop, t.Sc, t.ipower_time_depend. ������ constr_prop.
		// ��� ������������ 8 �������� 2016.
		// ������������ ���������� ��������� ������� ������������� ������. ������ constr_sosed.
		// ��������� ������ sosed=NULL.
		// ��� ������� �� ��� ��������� ����� 1-6. part 1..6.
		constr_nodes_nvtx_prop_alice(oc_global, inx, iny, inz, t.maxelm, xpos, ypos, zpos, TEMPERATURE, b, lb, t.whot_is_block, t.pa, t.maxnod, t.nvtx,
			t.prop, t.Sc, t.ipower_time_depend, matlist, t.ilevel_alice);

		if (b_on_adaptive_local_refinement_mesh) {
			// ��������� ������������ ���� �����.
			printf("the invariant correctness 8 november 2016...\n");
			printf("ANES_ALICE_CORRECT in constr_struct_alice.cpp file\n");
			ANES_ALICE_CORRECT(t.maxnod, t.pa, t.maxelm, t.nvtx);
		}
	}
	printf("part 1\n");

	 
	integer *ent_t = NULL; // ���������� ��������� �����.
	if (!bALICEflag) {
		// pa[0..maxnod-1]
		constr_nodes(t.pa, t.maxnod, ent_t, TEMPERATURE, t.whot_is_block, evt_t, inx, iny, inz, xpos, ypos, zpos, b, lb, tck_int_list, t.maxelm);
	}
   
	printf("part 2\n");    

	// ������� ������� ������ ����� ���������� �� 
	// ��� ������� ����������� ������������
	// ������ ��� 0 ���� ������ ���.
	// sosed[0..11][0..maxelm-1]
	integer **sosed = NULL;
	if (!bALICEflag) {
		constr_sosed(evt_t, ent_t, sosed, t.maxelm, inx, iny, inz, tck_int_list);
	}
	printf("part 3\n");
    
    // ��� ������� ������������ ������ ��������������
    // ��������� ������� ���������� ������ ��� ������.
	// nvtx[0..7][0..maxelm-1]
	// ��������� ���������� � �������.
	if (!bALICEflag) {
		// maxelm ���������� ��� ����������� �������, ��� �� ���� ��������� ������.
		constr_nvtx(evt_t, ent_t, t.nvtx, t.maxelm, inx, iny, inz, tck_int_list);
	}
	printf("part 4\n");

	

    // ������ ������������� ����� ���������������� ���� ���� ���������� ����� ���������.
	// ���� ���������� ����� ��������� ���� ��� ���������� �����, � ������� ������ ���� ����������� Tamb.
	// ����� ���������� ����� � ���� ������ ���� ��������� ����������� �������. ��� ���������� ���� ���� �������
	// �������������� ����� � ��������� ���������� ������ 2. ���������� ���� ������� � ������������� ������� � 
	// ������������� �� ����� �������������� � ��������� ���������� ������ 3. � ����� ���������� ������� ����������� 
	// ������ ����� �������������� ����� 1 � 3 ����� ����� � ������, �.�. ����� ������ ��� ����������� ��� ��������
	// ��������� ������ ����� ����������. �� �� ������ ������ ������ ���������� � ������ � ������� ������, �.�. ��������� 
	// � ��������� ���������� 2 � 3. ������� ������� ������� �� ����� �������� ��������� (������� ����� ��������� �����) 
	// ����� �������� ������ ������ ������� �����, ���������� �� �������� �������������� ����������.
	printf("part 5\n");

	// ������� �������� ���������� � ��������� prop ��� ���������� ��.
	// prop[0..2][0..maxelm-1]
	if (!bALICEflag) {
		constr_prop(evt_t, t.whot_is_block, ent_t, t.prop, t.maxelm, TEMPERATURE, b, lb, inx, iny, inz, t.Sc, t.ipower_time_depend, xpos, ypos, zpos, matlist, tck_int_list);
	}
	printf("part 6\n");

	

	/*
	*  sosedi, sosedb ( i - internal, b - boundary).
	*/

	// �������� ���������� ��������� ������.
    // � ���������� ������ ��������� �����,
    // ������������� ��������� ������� �����
    // ���������� �����.
	integer **gran_t=NULL;
	if (!bALICEflag) {
		enumerate_gran_temp(gran_t, t.maxelm, t.nvtx, t.maxbound, sosed, t.pa, s, ls);
	}
	else {
		// calculate_max_bound_temp ������������ 10 �������� 2016. � 10_34.
		// gran_t==NULL �� ������������.
		calculate_max_bound_temp(oc_global, t.maxbound, (inx + 1)*(iny + 1)*(inz + 1), b, lb, s, ls);
#if doubleintprecision == 1
		printf("maxbound=%lld\n", t.maxbound);
#else
		printf("maxbound=%d\n", t.maxbound);
#endif
		
		// constr_sosedi_alice ������������ 14 �������� 2016. � 12_36.
		// ���������� prop_b. ������ constr_prop_bound. ������������ 14 �������� 2016. � 14_24.
		// nvtxcell ������� ������������� NULL. ������������ � ������� �� ���������� ��������-������������ �����
		// ����� ������������ ��� ������� � ������������ ANES ��������.
		// ���������� t.sosedb, t.binternalsource. ������ constr_sosedb_temp ������������ 20 �������� 2016. � 11_24.
		constr_sosedi_prop_b_alice(oc_global, t.sosedb, t.binternalsource, t.sosedi,
			t.prop, t.prop_b, t.maxbound, t.maxelm, (inx + 1)*(iny + 1)*(inz + 1), b, lb, s, ls, w, lw, t.whot_is_block, t.nvtx, t.pa);
	}
	t.maxp = t.maxelm + t.maxbound;
#if doubleintprecision == 1
	printf("maxelm=%lld maxbound=%lld maxp=%lld\n", t.maxelm, t.maxbound, t.maxp);
#else
	printf("maxelm=%d maxbound=%d maxp=%d\n", t.maxelm, t.maxbound, t.maxp);
#endif
	
	printf("part 7\n");

	

    // ���������� ������� ��� ������� ����������� ����. 
    // ������ ��������� ������� ���������� � �����  
    // ���������� �� � ����� ��������� ��.
	if (!bALICEflag) {
		constr_sosedi(t.sosedi, t.maxelm, gran_t, sosed);
	}
	printf("part 8\n");

	

	// ���������� ���������� � ��������� �����:
	if (!bALICEflag) {
        // �������� !!! �� ��������� ���� dS.
		constr_sosedb_temp(t.sosedb, t.whot_is_block, t.binternalsource, t.maxelm, t.maxbound, gran_t, sosed, t.sosedi, t.nvtx, t.pa, b, lb, lw, w, s, ls);
	}
	printf("part 9\n");


	// �������� ��������� �� ������� ������������� �������.
	if (!bALICEflag) {
		constr_prop_bound(t.prop, t.prop_b, t.maxelm, t.maxbound, gran_t, sosed, t.nvtx, t.pa, b, lb);
	}
	printf("part 10\n");

	// ����� �� ������ �������� ������ ��� potent.
	//t.alpha=0.9; // �������� ������ ���������� ��� ��������� ����������������
	// �� ������� ������ 12-16 �������� 2016, ���� �������� ��� ��� ������������ ���������� ���������
	// ������ ������ �������� ������ 1.0, � ������ ���������� �������������� � ������ ���������� 
	// bHORF == 0.25 ��� ������������� � Theory Guide ANSYS Fluent. 
	t.alpha = 1.0; // �������� !!! ������ �������� t.alpha = 1.0 ���������. 

	
	// ��������� ����������� ������ ��� ������ ����������������.
    // ������������� ����� ������������. ������������ �������������� ����������� ����������� ����������� �� ������������ ������� WALL.
	// ������ ������� ������������  �������� ����� � ��� A��� �����.
	if (!breconstruct) {
		allocation_memory_temp(t.potent, t.total_deformation, t.slau, t.slau_bon, t.sosedb, t.maxelm, t.maxbound, ls, lw, w);
		printf("part 11\n");
	}


	

	// ������ ����������� ����� ��� ������ ��������� ������ �����.
	if (!breconstruct) {
		if (!bALICEflag) {
			constr_link_on_surface_for_radiation_model(t.maxelm, t.whot_is_block, t.sosedi, t.nvtx, t.pa, b, lb);
		}
		else {
			// sosedb - ������� ��� �������� ������� ����� ���������� ���� �� ���� �����.
			constr_link_on_surface_for_radiation_model_alice(t.maxelm, t.sosedb, t.whot_is_block, t.sosedi, t.nvtx, t.pa, b, lb);
		}
	}
	// ������ ����� ����� ������������ �������� ��� ����������� 
    // ������������.

	printf("part 12.0\n");

	
	if (!bALICEflag) {
		if (bextendedprint) {
			printf("extended print. please wait...\n");
		}
		constr_nvtxcell(evt_t, t.sosedb, t.maxbound, t.maxelm, bextendedprint, t.nvtxcell, t.ncell, inx, iny, inz, tck_int_list);
	}

	if (tck_int_list != NULL) {
		delete[] tck_int_list;
		tck_int_list = NULL;
	}

#if doubleintprecision == 1
	/*
	for (integer i = 0; i <= inx; i++) {
		printf("xpos[%lld]=%e\n", i, xpos[i]);
	}
	// ���������� ���������� � ���� � ������������� ��������� inx
	for (integer i = 0; i <= iny; i++) {
		printf("ypos[%lld]=%e\n", i, ypos[i]);
	}
	// ���������� ���������� � ���� � ������������� ��������� inx
	for (integer i = 0; i <= inz; i++) {
		printf("zpos[%lld]=%e\n", i, zpos[i]);
	}
	getchar();
	*/
#else
	/*
	for (integer i = 0; i <= inx; i++) {
		printf("xpos[%d]=%e\n", i, xpos[i]);
	}
	// ���������� ���������� � ���� � ������������� ��������� inx
	for (integer i = 0; i <= iny; i++) {
		printf("ypos[%d]=%e\n", i, ypos[i]);
	}
	// ���������� ���������� � ���� � ������������� ��������� inx
	for (integer i = 0; i <= inz; i++) {
		printf("zpos[%d]=%e\n", i, zpos[i]);
	}
	getchar();
	*/
#endif
	

	printf("part 12.1\n");

	// ��� ������������� ���� ����� ������������ ������������ � ������������ � ������������� 
	// ANES �������, ������� ���������� �������� ����� ������ �� ���������.
	if( (!bALICEflag)) {

		// 15 �������. � ������ �������� ����� ��� ������������ ������������ �� ������ 
		// Coarce Mesh ����� ��������� �������������� ���������� ����������� ����� � �������.
		// �������� nvtxcell
		integer iPnodes_count = 0;
		bool* found_hash_table = new bool[t.maxelm + 1];
		for (integer i = 1; i <= t.maxelm; i++) {
			found_hash_table[i] = false; // �������������.
			integer ib = t.whot_is_block[i-1];
			if (b[ib].g.itypegeom == 2) {
				// 30.08.2017. �� ��������� �� �� ���������� ���� �����.
				found_hash_table[i] = true; // �������������.
			}
		}

		for (integer i_1 = 0; i_1 < t.ncell; i_1++) {
			found_hash_table[t.nvtxcell[0][i_1]] = true;
			found_hash_table[t.nvtxcell[1][i_1]] = true;
			found_hash_table[t.nvtxcell[2][i_1]] = true;
			found_hash_table[t.nvtxcell[3][i_1]] = true;
			found_hash_table[t.nvtxcell[4][i_1]] = true;
			found_hash_table[t.nvtxcell[5][i_1]] = true;
			found_hash_table[t.nvtxcell[6][i_1]] = true;
			found_hash_table[t.nvtxcell[7][i_1]] = true;
		}


		iPnodes_count = 0;
		for (integer i = 1; i <= t.maxelm; i++) {
			if (!found_hash_table[i]) {
				// ���� i �� ������ � nvtxcell
				iPnodes_count++;
			}
		}

#if doubleintprecision == 1
		printf("problem visualisation nodes count iPnodes_count=%lld\n", iPnodes_count);
#else
		printf("problem visualisation nodes count iPnodes_count=%d\n", iPnodes_count);
#endif
		
		bool bpolygon = false;
		for (integer i83 = 1; i83 < lb; i83++) {
			if (b[i83].g.itypegeom == 2) {
				bpolygon = true;
			}
		}

		//getchar();
		if ((iPnodes_count > 0)&&(ipolygon_count_limit<iGLOBAL_RESTART_LIMIT)/*&&(!bpolygon)*/) {

			for (integer i = 1; i <= t.maxelm; i++) {
				if (!found_hash_table[i]) {
					integer iP = i - 1;
					// iP - ����� ������������ ������������ ������
					// iP ���������� �� 0..maxelm-1
					integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
					iE = t.sosedi[ESIDE][iP].iNODE1; iN = t.sosedi[NSIDE][iP].iNODE1; iT = t.sosedi[TSIDE][iP].iNODE1; iW = t.sosedi[WSIDE][iP].iNODE1; iS = t.sosedi[SSIDE][iP].iNODE1; iB = t.sosedi[BSIDE][iP].iNODE1;

					// ���������� �������� �������� ������������ ������:
					//doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
					//volume3D(iP, t.nvtx, t.pa, dx, dy, dz);
					TOCHKA pointP;
					center_cord3D(iP, t.nvtx, t.pa, pointP,100);


					if ((iE >= t.maxelm) && (iW >= t.maxelm)) {
						// ������ � xpos
						addboundary(xpos, inx, pointP.x);
					}
					if ((iN >= t.maxelm) && (iS >= t.maxelm)) {
						// ������ � ypos
						addboundary(ypos, iny, pointP.y);
					}
					if ((iT >= t.maxelm) && (iB >= t.maxelm)) {
						// ������ � zpos
						addboundary(zpos, inz, pointP.z);
					}

				}
			}

			// �������������� �� �����������
			BubbleEnhSort(xpos, inx);
			BubbleEnhSort(ypos, iny);
			BubbleEnhSort(zpos, inz);

			// ����� ���������� ����������� ������ �� ��� ���� �������� ������:
			free_level1_temp(t);
			if (!breconstruct) {
				free_level2_temp(t); // ������������ ������ �� ��� ������.
			}

			delete[] found_hash_table;

			// 22.01.2017
			if (sosed != NULL) {
				for (integer i_97 = 0; i_97 < 12; i_97++) {
					if (sosed[i_97] != NULL) {
						delete sosed[i_97];
						sosed[i_97] = NULL;
					}
				}
				delete sosed;
			}
			sosed = NULL;

			if (gran_t != NULL) {
				for (integer i_97 = 0; i_97 < 6; i_97++) {
					if (gran_t[i_97] != NULL) {
						delete gran_t[i_97];
						gran_t[i_97] = NULL;
					}
				}
				delete gran_t;
			}
			gran_t = NULL;

			if (ent_t != NULL) {
				delete[] ent_t;
			}
			ent_t = NULL;

			if (evt_t != NULL) {
				delete[] evt_t;
				evt_t = NULL;
			}

			iPnodes_count_shadow_memo = iPnodes_count;

			printf("progon number is ipolygon_count_limit=%d\n", ipolygon_count_limit);
			printf("iPnodes_count = %d=%d, x= %d=%d, y= %d=%d, z= %d=%d\n", iPnodes_count_shadow, iPnodes_count, inx_shadow,inx, iny_shadow,iny, inz_shadow, inz);
			if (abs(iPnodes_count_shadow - iPnodes_count) + abs(inx_shadow - inx) + abs(iny_shadow - iny) + abs(inz_shadow - inz) == 0) {
				bglobal_restart_06_10_2018_stagnation[ipolygon_count_limit] = true;
			}
			else {
				bglobal_restart_06_10_2018_stagnation[ipolygon_count_limit] = false;
			}
			if (ipolygon_count_limit == iGLOBAL_RESTART_LIMIT) {
				bool bcheck = true;
				for (integer i_4 = 1; i_4 <= iGLOBAL_RESTART_LIMIT; i_4++) {
					bcheck = bcheck && bglobal_restart_06_10_2018_stagnation[i_4];
				}
				if (bcheck) {
					bglobal_restart_06_10_2018 = false;
					printf("bglobal_restart_06_10_2018 = false\n");
				}
				else {
					bglobal_restart_06_10_2018 = true;
					printf("bglobal_restart_06_10_2018 = true;\n");
					//bglobal_restart_06_10_2018 = false;
				}
			}
			
			iPnodes_count_shadow = iPnodes_count;
			// ������� ������� �������.
			goto RESTART_CONSTRUCT;

		}
		delete[] found_hash_table;
	}


	
   
    // �������������:
	integer i=0;

	integer maxelm_global_flow; // ������������ ��������� ���������� ���������� �� ������������� ��������� �������.
	integer *evt_f=NULL; // ���������� ��������� ����������� �������
	integer *whot_is_block_fl=NULL; // �������� ���������.

	printf("part 12.2\n");

	TOCKA_INT* tck_int_list_flow = NULL;

	if (!bALICEflag) {
		enumerate_volume(evt_f, maxelm_global_flow, HYDRODINAMIC, xpos, ypos, zpos, whot_is_block_fl, inx, iny, inz, b, lb, lu, my_union, iunion_id_p1, tck_int_list_flow);
	}
	else {
		calculate_max_elm(oc_global, maxelm_global_flow, HYDRODINAMIC, b, lb, false);
		printf("maxelm calculate succseful.\n");
		// ��� ����������� ��� ��������� �������.
		init_evt_f_alice(evt_f, HYDRODINAMIC, xpos, ypos, zpos, inx, iny, inz, b, lb, tck_int_list_flow);
	}
	
	if (whot_is_block_fl != NULL) {
		// ���������� ������� �������������� � �� � ���������.
		delete[] whot_is_block_fl;
	}

#if doubleintprecision == 1
	printf("maxelm_global_flow=%lld\n", maxelm_global_flow);
#else
	printf("maxelm_global_flow=%d\n", maxelm_global_flow);
#endif
	
	printf("part 12.3\n");
	t.rootWE=NULL; t.rootSN=NULL; t.rootBT=NULL;

	// 22 �������� 2016 ���� �������� ������ �����, ��� ��������
	// ������������� � �������� ��� ���� ����� ����.
	// ����� 11, 12, 12.5, 13.
	// ���������� evt_f, domain_id, evt_f2.
	integer **evt_f2 = NULL;
	integer* domain_id = NULL;
	constr_ptr_temp_part1(flow_interior, evt_f, evt_f2, domain_id, inx, iny, inz);


	

	   // ��������� ������ �����
	   // �������������� � ������������.
       // �����   14, 15, 16, 17, 18
	   // ����� 22 �������� 2016 ��� ������� ���� �������� ������ �����,
	   // ������ ����� ������������ � �������� � ��� ���� �����.
	   if (!bALICEflag) {
		   constr_ptr_temp_part2(flow_interior, evt_f, evt_f2, domain_id, inx, iny, inz, tck_int_list_flow, maxelm_global_flow);

		   constr_ptr_temp(flow_interior, f, t.maxelm, t.ptr, evt_t, evt_f, evt_f2, domain_id, inx, iny, inz, breconstruct);

		   if (evt_f != NULL) {
			   delete evt_f;
		   }
		   evt_f = NULL;
	   }
	   else {
		   // ���������� ��������� ptr �� ���� �����.
		   // ��������� ������ ��� FLOW.
		   // ���� �������� 22 �������� 2016.
		   // ������ ������� ���� ���� �������� ��������� ����������� ������, � ��� �� �� �� ��������� ���
		   // ���� �����.
		   constr_ptr_temp_allocation_memory_alice(flow_interior, f);
	   }
				

		
		// ������� � ����������� ������������.
		if (!bALICEflag) {
			printf("please wait...\n");
			// ���������� ������ � ������ ����� ��������������� � tecplot360
			// ����� �� ����. ��� �������� ��� ���� ����� ��������� � ���������� 
			// 8*t.maxelm*sizeof(doublereal) �� ����������� ������ ����������.
			// ��� �� ����� ����� ����� ������������ � ��� ������ �������������.
			// �.�. ������ ����������� ����������� ������ ��� ���������� ��.
			if (iunion_id_p1 == 0) {
				exporttecplotxy360T_3D_part1and3(t, t.maxelm, t.maxbound, bextendedprint, t.ncell, t.nvtx, t.nvtxcell, t.pa, t.sosedb, 1, t.ptr);
			}
			else {
				exporttecplotxy360T_3D_part1and3(my_union[iunion_id_p1-1].t, my_union[iunion_id_p1 - 1].t.maxelm, 
					my_union[iunion_id_p1 - 1].t.maxbound, bextendedprint, my_union[iunion_id_p1 - 1].t.ncell,
					my_union[iunion_id_p1 - 1].t.nvtx, my_union[iunion_id_p1 - 1].t.nvtxcell,
					my_union[iunion_id_p1 - 1].t.pa, my_union[iunion_id_p1 - 1].t.sosedb, 1, my_union[iunion_id_p1 - 1].t.ptr);
			}
		}
		if (t.nvtxcell != NULL) {
			for (integer i74 = 0; i74<8; i74++) { // -8N
				if (t.nvtxcell[i74] != NULL) {
					delete t.nvtxcell[i74];
					t.nvtxcell[i74] = NULL;
				}
			}
			delete t.nvtxcell;
		}
		t.nvtxcell = NULL; // ���� ��������� ����� NULL �� ��� ������������ �� ���������� �������� ������.


		if (sosed != NULL) {
			for (i = 0; i < 12; i++) {
				if (sosed[i] != NULL) {
					delete sosed[i];
					sosed[i] = NULL;
				}
			}
			delete sosed;
		}
		sosed = NULL;

		// ������������ ����������� ������ TEMPER
		if (ent_t != NULL) {
			delete ent_t;
		}
		ent_t = NULL;
		//if (evt_t != NULL) {
			//delete evt_t;
		//}
		//evt_t = NULL;
		if (sosed != NULL) {
			for (i = 0; i < 12; i++) {
				if (sosed[i] != NULL) {
					delete sosed[i];
					sosed[i] = NULL;
				}
			}
			delete sosed;
		}
		sosed = NULL;

		if (gran_t != NULL) {
			for (i = 0; i < 6; i++) {
				if (gran_t[i] != NULL) {
					delete gran_t[i];
					gran_t[i] = NULL;
				}
			}
			delete gran_t;
		}
		gran_t = NULL;

	if (maxelm_global_flow > 0) {
			// ���� ���� FLUID

		// ������������ ����������� ������ TEMPER
		if (ent_t != NULL) {
			delete ent_t;
		}
		ent_t=NULL;
		
		if (sosed != NULL) {
			for (i = 0; i < 12; i++) {
				if (sosed[i] != NULL) {
					delete sosed[i];
					sosed[i] = NULL;
				}
			}
			delete sosed;
		}
		sosed=NULL;

		if (gran_t != NULL) {
			for (i = 0; i < 6; i++) {
				if (gran_t[i] != NULL) {
					delete gran_t[i];
					gran_t[i] = NULL;
				}
			}
			delete gran_t;
		}
		gran_t=NULL;

		integer j=0; // ��������
        integer *ent_f=NULL; // ���������� ��������� �����.
		integer **sosed_f=NULL; // ������ ��� ���������� �� ������ ����� ���������� ��.
		integer **gran_f=NULL; // ���������� ��������� ������

		integer icount_part=19; // ������� ���� ����������

        // ������ �� ���� ������ �����:
		for (i=0; i<flow_interior; i++) {
			// ��� ���������� ��� ������ ���� FLUID.

			if (bALICEflag) {
				// ������ ������ 19, 20, 21 � 22.
				// ���������� f[i].pa, f[i].maxnod. ������ constr_nodes_flow.
				// �������� !!! icolor_different_fluid_domain �� ���������.
				// ���������� f[i].nvtx, f[i].maxelm. ������ constr_nvtx_flow.
				// ���������� f[i].prop. ������ constr_prop_flow.
				// 21.09.2016 ������ ���������. ��� ������� ����� icolor_different_fluid_domain.
				// ���������� ������� inum_FD � octree ������.
				// 22.09.2016 ������� ���������� f[0].ptr[fluid_elm_id]=��������������� temper_elm_id.
				// 25.09.2016 ���������� t.ptr. 
				constr_nodes_nvtx_prop_flow_alice(oc_global, inx, iny, inz, f[i].maxelm, xpos, ypos, zpos,
					HYDRODINAMIC, b, lb, f[i].pa, f[i].maxnod, f[i].nvtx, f[i].prop, matlist, f[i].ptr, 
					f[i].whot_is_block, t.ptr, t.maxelm);

				// 22 �������� 2016 ���������� ������� ����� ��� ���� �����.
				// ������ ������� ���������� ����  evt_f2[MASKDOMAINFLUIDCOLOR] � ������.
				// ���������� icolor_different_fluid_domain.
				constr_ptr_temp_part2(flow_interior, evt_f, evt_f2, domain_id, inx, iny, inz, tck_int_list_flow, maxelm_global_flow);
				if (evt_f != NULL) {
					delete evt_f;
				}
				evt_f = NULL;
				constr_icolor_different_fluid_domain_alice(f[i].maxelm, f[i].icolor_different_fluid_domain,
					inx, iny, inz, evt_f2, xpos, ypos, zpos, f[i].pa, f[i].nvtx, oc_global);
			}

			// integer *ent_f; - ���������� ��������� �����.
			if (!bALICEflag) {
				constr_nodes_flow(f[i].pa, f[i].maxnod, ent_f, evt_f2, i, inx, iny, inz, xpos, ypos, zpos);
			}

#if doubleintprecision == 1
			printf("part %lld\n", icount_part++); //19
#else
			printf("part %d\n", icount_part++); //19
#endif
	       

            // ��� ������� ������������ ������ ��������������
            // ��������� ������� ���������� ������ ��� ������.
			if (!bALICEflag) {
				constr_nvtx_flow(evt_f2, f[i].icolor_different_fluid_domain, i, ent_f, f[i].nvtx, f[i].maxelm, inx, iny, inz);
			}
#if doubleintprecision == 1
			printf("part %lld\n", icount_part++); //20
#else
			printf("part %d\n", icount_part++); //20
#endif
	        

	        // ������� ������� ��� ������� ����������� ������������
            // ������ ��� 0 ���� ������ ���.
			if (!bALICEflag) {
				// ��� ������ ���������� �.�. ���������� � sosed_f ��� ������ ���� �� ���������, ��� �������� � octree ������.
				constr_sosed_flow(evt_f2, i, sosed_f, f[i].maxelm, inx, iny, inz);
			}
#if doubleintprecision == 1
			printf("part %lld\n", icount_part++); //21
#else
			printf("part %d\n", icount_part++); //21
#endif
	        

	        // ������ ����� ����� ������������ �������� ��� ����������� 
            // ������������. ����� ����������� ������������ � FLOW ���, ��� 
			// ���������� ������������ ������������ � ����� TEMPER. �����������
			// ������������ �������� ������ ��� ���������� �� �� ���������� 
			// ��������� ����.
			/*
			* �������� ! �������� ��������� ������� ������.
	        * constr_nvtxcell(evt_f, f.nvtxcell, f.ncell, inx, iny, inz);
	        * printf("part not_number\n");
			*/


            // ������� �������� ���������� ���������� �� � ��������� prop.
#if doubleintprecision == 1
			printf("%lld\n", f[i].maxelm);
#else
			printf("%d\n", f[i].maxelm);
#endif
			 
			if (bwait) {
				//getchar();
				system("pause");
			}
			if (!bALICEflag) {
				constr_prop_flow(evt_t, t.whot_is_block, evt_f2, i, f[i].prop, f[i].maxelm, b, lb, inx, iny, inz, xpos, ypos, zpos, matlist);
			}
			if (evt_t != NULL) {
				delete evt_t;
			}
			evt_t = NULL;
#if doubleintprecision == 1
			printf("part %lld\n", icount_part++); //22
#else
			printf("part %d\n", icount_part++); //22
#endif
	        

			
	
			/*
	         *  sosedi, sosedb ( i - internal, b - boundary).
	         */

            // �������� ���������� ��������� ������.
            // � ���������� ������ ��������� �����,
            // ������������� ��������� ������� �����
            // ���������� �����.
			if (!bALICEflag) {
				enumerate_gran_flow(gran_f, f[i].maxelm, f[i].nvtx, f[i].maxbound, sosed_f, f[i].pa);
			}
			else {
				calculate_max_bound_flow(oc_global, f[i].maxbound, (inx + 1)*(iny + 1)*(inz + 1), b, lb, s, ls);
#if doubleintprecision == 1
				printf("maxbound=%lld\n", f[i].maxbound);
#else
				printf("maxbound=%d\n", f[i].maxbound);
#endif
				
			}
	        f[i].maxp = f[i].maxelm + f[i].maxbound;
#if doubleintprecision == 1
			printf("maxelm=%lld maxbound=%lld maxp=%lld\n", f[i].maxelm, f[i].maxbound, f[i].maxp);
			printf("part %lld\n", icount_part++);
#else
			printf("maxelm=%d maxbound=%d maxp=%d\n", f[i].maxelm, f[i].maxbound, f[i].maxp);
			printf("part %d\n", icount_part++);
#endif
			 

			


			if (bALICEflag) {
				// constr_sosedi_prop_b_flow_alice ������������  22 �������� 2016 �� ������
				// constr_sosedi_prop_b_alice ������� ���� ������������ 14 �������� 2016. � 12_36.
				// ���������� prop_b. ������ constr_prop_bound_flow. ������������ 14 �������� 2016. � 14_24.
				// nvtxcell ������� ������������� NULL. ������������ � ������� �� ���������� ��������-������������ �����
				// ����� ������������ ��� ������� � ������������ ANES ��������.
				// ���������� f[i].sosedb ������ constr_sosedb_flow ������������ 20 �������� 2016. � 11_24.
				// �������������� 24 �������� 2016.
				constr_sosedi_prop_b_flow_alice(oc_global, f[i].sosedb,
					f[i].sosedi, f[i].prop, f[i].prop_b,
					f[i].maxbound, f[i].maxelm, (inx + 1)*(iny + 1)*(inz + 1),
					b, lb, s, ls, w, lw, f[i].whot_is_block, f[i].nvtx, f[i].pa);
			}

            // ���������� ������� ��� ������� ����������� ����. 
            // ������ ��������� ������� ���������� � ����� ���������� �� � 
            // ����� ��������� ��.
			if (!bALICEflag) {
				constr_sosedi(f[i].sosedi, f[i].maxelm, gran_f, sosed_f);
			}
#if doubleintprecision == 1
			printf("part %lld\n", icount_part++); //24
#else
			printf("part %d\n", icount_part++); //24
#endif
	       

	        // ���������� ���������� � ��������� �����:
			if (!bALICEflag) {
				constr_sosedb_flow(f[i].sosedb, t.whot_is_block, f[i].ptr, f[i].maxelm, f[i].maxbound, gran_f, sosed_f, f[i].sosedi, f[i].nvtx, f[i].pa, lw, w, ls, b);
			}
#if doubleintprecision == 1
			printf("part %lld\n", icount_part++); //25
#else
			printf("part %d\n", icount_part++); //25
#endif
			


	        // �������� ������� ��������� �� ������� ������ �������.
			if (!bALICEflag) {
				constr_prop_bound_flow(f[i].prop, f[i].prop_b, f[i].maxelm, f[i].maxbound, gran_f, sosed_f);
			}
#if doubleintprecision == 1
			printf("part %lld\n", icount_part++); //26
#else
			printf("part %d\n", icount_part++); //26
#endif
			

			
			// 24 �������� 2016 (������ ������� ������������ � �������� ����� � ��� ���� �����).
			// ����� �� ����������� �������� � ����� �����.
            // �� ��������� ������������ ��������� ��������� �����.
			determination_of_activity_flow(f[i].sosedb, f[i].maxbound, ls, lw, w, f[i].bactive, f[i].bPressureFix, f[i].bLR1free);
#if doubleintprecision == 1
			printf("part %lld\n", icount_part++); //27
#else
			printf("part %d\n", icount_part++); //27
#endif
            

			
			


	        // ����� �� ������ �������� ������ ��� potent.
	        // ����� ���������� ���������� ��������� ����������.

	        // ��������� ����������� ������ ��� ������ �������������.
			// �������������� ���������� ������� ������ potent.
			// 24 �������� 2016 (������ ���� ����� �������� � �� ���� �����).
			if (!breconstruct) {
				allocation_memory_flow( f[i].potent, f[i].slau, f[i].slau_bon,  f[i].sosedb, f[i].maxelm, f[i].maxbound, f[i].alpha, ls, lw, w,  dgx, dgy, dgz, f[i].nvtx, f[i].pa, f[i].prop, f[i].sosedi);
			}
			allocation_memory_flow_2(
				f[i].diag_coef, f[i].maxelm, f[i].maxbound,
				f[i].SInvariantStrainRateTensor,
				f[i].mf);


			// �������� ������������� ��������� ��������.
			//errno_t err_inicialization_data;
			//FILE* fp_inicialization_data;
			//err_inicialization_data = fopen_s(&fp_inicialization_data, "load.txt", "r");
			//if (err_inicialization_data != 0) {
				// �������� �������� ��� ���� �����������.



				// ������������� ��������� �������� �� ������������ ��������� �������.
				// 26 ����� 2017.
				for (integer i32 = 0; i32 < f[i].maxelm; i32++) {
					// 15.09.2018	


					// ��������� ����������������� �������� ����� ����� ����� ��.
					// �������� ����� ����������� �� ������� �������� �� � ������
					// ������ ��� ���������������� �������� ���-���. ��� ��� ���������� ������������
					// ������� �������� ������������ �������� �� ����� ��.

					bool bsimplelinearinterpol = true; // ����������� ������� �������� ������������ �������� �� �����.

					integer iflow = 0;
					return_calc_correct_mass_flux_only_interpolation(i32,
						f[i].potent,
						f[i].pa,
						f[i].prop,
						f[i].prop_b,
						f[i].nvtx,
						f[i].sosedi,
						f[i].maxelm,
						f[i].mf[i32]);

					//printf("%e\n", f[i].mf[i32][TSIDE]);
					//doublereal ts = f[i].mf[i32][TSIDE] + f[i].mf[i32][BSIDE] + f[i].mf[i32][ESIDE] + f[i].mf[i32][WSIDE] + f[i].mf[i32][NSIDE] + f[i].mf[i32][SSIDE];
					//if (ts != ts) {
						//printf("%d %e %e %e %e %e %e\n",i32, f[i].mf[i32][TSIDE], f[i].mf[i32][BSIDE], f[i].mf[i32][ESIDE], f[i].mf[i32][WSIDE], f[i].mf[i32][NSIDE], f[i].mf[i32][SSIDE]);
					//}

				}
				//getchar();
			//}

#if doubleintprecision == 1
			printf("part %lld allocation_memory_flow_2.\n", icount_part++); //28
#else
			printf("part %d allocation_memory_flow_2\n", icount_part++); //28
#endif
			//getchar();
			 if (0) {
		        xyplot( f, flow_interior, t);
		        printf("after allocate memory. OK.\n");
				system("pause");
	            //getchar(); // debug 
	         }


			// ������������ ����������� ������ ��� ������ ���� FLUID.
			 if (ent_f != NULL) {
				 delete ent_f;
			 }
			ent_f=NULL;
			if (sosed_f != NULL) {
				for (j = 0; j < 12; j++) {
					if (sosed_f[j] != NULL) {
						delete sosed_f[j];
						sosed_f[j] = NULL;
					}
				}
				delete sosed_f;
			}
			sosed_f=NULL;

			if (gran_f != NULL) {
				for (j = 0; j < 6; j++) {
					if (gran_f[j] != NULL) {
						delete gran_f[j];
						gran_f[j] = NULL;
					}
				}
				delete gran_f;
			}			
			gran_f=NULL;

			// ������������� ��� ������������� ������:
			// TODO

			f[i].OpTemp=temp_ref; // Operating Temperature

#if doubleintprecision == 1
			printf("end FLUID %lld\n", i);
#else
			printf("end FLUID %d\n", i);
#endif
			
			//getchar();

		} // ����� ����� �� ���� FLUID �����.


		// ���������� ���������� �� ������ �� ������������� ��� 
		// ���� ����� �.�. ��� ���������� ������ ������� �� �����������  
		// ���������� ������� ������ �������.
		if (!bALICEflag) {
			printf("share information about the turbulence model\n");
			constr_fluid_equation(f, flow_interior, ls, lw, w);
			printf("end fluid\n");
		}

		if (evt_f2 != NULL) {
			for (i = 0; i < 3; i++) {
				if (evt_f2[i] != NULL) {
					delete evt_f2[i];
					evt_f2[i] = NULL;
				}
			}
			delete evt_f2;
		}
		evt_f2=NULL;

	} else {
       
	   if (!breconstruct) {
		   if (f != NULL) {
			   // ������������ ������ ��� ��������� flow.
			   free_level1_flow(f, flow_interior);
			   free_level2_flow(f, flow_interior); // ������������ ������ �� ��� ������.
			   delete[] f;
			   f = NULL;
		   }
		   f = new FLOW[1];
		   f[0].ptr = NULL;
	   }
	   flow_interior = 0; // ��� ��� FLUID
	   // ����� ������������� � ������������ ���, �.�. 
	   // ��� ����������������� ������������.
	   if (!breconstruct) {
		   // ���� ������ ������ �� �� � ���������.
		   if (t.ptr != NULL) {
			   for (i = 0; i<2; i++) {
				   if (t.ptr[i] != NULL) {
					   delete[] t.ptr[i];
					   t.ptr[i] = NULL;
				   }
			   }
			   delete[] t.ptr;
			   t.ptr = NULL;
		   }
		   t.ptr = NULL;
	   }
	   // ���������� ������������� ������.
	   f[0].nvtx=NULL;
	   f[0].maxnod=0;
	   f[0].maxelm=0;
	   f[0].pa=NULL;
	   f[0].sosedi=NULL;
	   f[0].maxbound=0;
	   f[0].maxp=0;
	   f[0].sosedb=NULL;
	   f[0].icolor_different_fluid_domain = NULL;
	   if (f[0].ptr != NULL) {
		   delete[] f[0].ptr;
	   }
	   f[0].ptr=NULL;
	   f[0].potent=NULL;
	   f[0].prop=NULL;
	   f[0].prop_b=NULL;
	   f[0].alpha=NULL;
	   if (!breconstruct) {
		   f[0].slau = NULL;
		   f[0].slau_bon = NULL;
	   }
	   f[0].mf=NULL;
	   f[0].rdistWall=NULL;
	   f[0].SInvariantStrainRateTensor=NULL;
	   f[0].diag_coef=NULL;
	   f[0].bactive=false;
	   f[0].OpTemp=temp_ref;
	   f[0].bLR1free=true;
	   f[0].bPressureFix=true;
	   if (!breconstruct) {
		   f[0].iN = NULL;
		   f[0].id = NULL;
	   }
	   f[0].iflowregime=LAMINAR;

	   // ��� ���������� ������ ������ ���� �����������.
	   if (evt_f2 != NULL) {
		   for (i = 0; i < 3; i++) {
			   if (evt_f2[i] != NULL) {
				   delete evt_f2[i];
				   evt_f2[i] = NULL;
			   }
		   }
		   delete evt_f2;
	   }
	   evt_f2 = NULL;
	}

	if (tck_int_list_flow != NULL) {
		delete[] tck_int_list_flow;
		tck_int_list_flow = NULL;
	}

	if (evt_t != NULL) {
		delete evt_t;
	}
	evt_t = NULL;
	
	if (maxelm_global_flow == 0) {
        // ������������ ����������� ������ TEMPER
		if (ent_t != NULL) {
			delete ent_t;
		}
		ent_t=NULL;
		if (evt_t != NULL) {
			delete evt_t;
		}
		evt_t=NULL;
		if (sosed != NULL) {
			for (i = 0; i < 6; i++) {
				if (sosed[i] != NULL) {
					delete sosed[i];
					sosed[i] = NULL;
				}
			}
			delete sosed;
		}
		sosed=NULL;
		if (gran_t != NULL) {
			for (i = 0; i < 6; i++) {
				if (gran_t[i] != NULL) {
					delete gran_t[i];
					gran_t[i] = NULL;
				}
			}
			delete gran_t;
		}
		gran_t=NULL;

		// ������������ ����������� ������ FLOW
		if (evt_f != NULL) {
			delete evt_f;
		}
		evt_f=NULL;
	}

	if (0) {
		// ������ ������ xyplot �� ���� ����� ������������ ���������� � ����� ��������� �������������.
		xyplot( f, flow_interior, t);
		printf("load temper and flow. OK.\n");
		system("pause");
	    //getchar(); // debug avtosave
	}
    	
} // load_TEMPER_and_FLOW


/*
// ���������� ������ ����� ����� meshin.txt,
// � ������ ���������� �����.
// ��� ����� ����������������.
void meshin_nodes_temp(char* fname, TEMPER t) {
   
    FILE *fp;
	errno_t err;
	// �������� ����� ��� ������.
	if ((err = fopen_s( &fp, fname, "w")) != 0) {
		printf("Create File Error\n");
	}
	else {
		integer i=0;
		// ������ ������ - ��������� �����.
		for (i=0; i<t.maxnod; i++) {
		#if doubleintprecision == 1
			fprintf(fp,"%lld %e %e %e\n",i+1,t.pa[i].x,t.pa[i].y,t.pa[i].z);
		#else
			fprintf(fp,"%d %e %e %e\n",i+1,t.pa[i].x,t.pa[i].y,t.pa[i].z);
		#endif
			
		}
        fprintf(fp,"\n");
		// ������ ������ - ����������� ������.
		for (i=0; i<t.maxelm; i++) {
		#if doubleintprecision == 1
			fprintf(fp,"%lld %lld %lld %lld %lld",i+1,t.nvtx[0][i],t.nvtx[1][i],t.nvtx[2][i],t.nvtx[3][i]);
			fprintf(fp," %lld %lld %lld %lld", t.nvtx[4][i],t.nvtx[5][i],t.nvtx[6][i],t.nvtx[7][i]);
			fprintf(fp," %lld %lld %lld", t.sosedi[ESIDE][i].iNODE1, t.sosedi[NSIDE][i].iNODE1, t.sosedi[TSIDE][i].iNODE1);
			fprintf(fp," %lld %lld %lld", t.sosedi[WSIDE][i].iNODE1, t.sosedi[SSIDE][i].iNODE1, t.sosedi[BSIDE][i].iNODE1);
			fprintf(fp," %lld %lld %lld", t.sosedi[EE][i].iNODE1, t.sosedi[NN][i].iNODE1, t.sosedi[TTSIDE][i].iNODE1);
			fprintf(fp," %lld %lld %lld", t.sosedi[WW][i].iNODE1, t.sosedi[SS][i].iNODE1, t.sosedi[BB][i].iNODE1);
		#else
			fprintf(fp,"%d %d %d %d %d",i+1,t.nvtx[0][i],t.nvtx[1][i],t.nvtx[2][i],t.nvtx[3][i]);
			fprintf(fp," %d %d %d %d", t.nvtx[4][i],t.nvtx[5][i],t.nvtx[6][i],t.nvtx[7][i]);
			fprintf(fp," %d %d %d", t.sosedi[ESIDE][i].iNODE1, t.sosedi[NSIDE][i].iNODE1, t.sosedi[TSIDE][i].iNODE1);
			fprintf(fp," %d %d %d", t.sosedi[WSIDE][i].iNODE1, t.sosedi[SSIDE][i].iNODE1, t.sosedi[BSIDE][i].iNODE1);
			fprintf(fp," %d %d %d", t.sosedi[EE][i].iNODE1, t.sosedi[NN][i].iNODE1, t.sosedi[TTSIDE][i].iNODE1);
			fprintf(fp," %d %d %d", t.sosedi[WW][i].iNODE1, t.sosedi[SS][i].iNODE1, t.sosedi[BB][i].iNODE1);
		#endif
			
			fprintf(fp," %e %e %e", t.prop[RHO][i], t.prop[CP][i], t.prop[LAM][i]);
			fprintf(fp," %e %e %e", t.prop[MULT_LAM_X][i], t.prop[MULT_LAM_Y][i], t.prop[MULT_LAM_Z][i]);
		}
        fprintf(fp,"\n");
        // �������� ������ - ������ ����������� ������� ��� ����������� ������������.
		for (i=0; i<t.ncell; i++) {
		#if doubleintprecision == 1
			fprintf(fp,"%lld %lld %lld %lld ",t.nvtxcell[0][i], t.nvtxcell[1][i], t.nvtxcell[2][i], t.nvtxcell[3][i]);
			fprintf(fp,"%lld %lld %lld %lld\n",t.nvtxcell[4][i], t.nvtxcell[5][i], t.nvtxcell[6][i], t.nvtxcell[7][i]);
		#else
			fprintf(fp,"%d %d %d %d ",t.nvtxcell[0][i], t.nvtxcell[1][i], t.nvtxcell[2][i], t.nvtxcell[3][i]);
			fprintf(fp,"%d %d %d %d\n",t.nvtxcell[4][i], t.nvtxcell[5][i], t.nvtxcell[6][i], t.nvtxcell[7][i]);
		#endif
			
		}
		fprintf(fp,"\n");
       fclose(fp); // �������� �����
	}
} // meshin_nodes_temp
*/

/*
// ���������� ������ ����� ����� meshin.txt,
// � ������ ���������� �����.
// ��� ����� �������������.
void meshin_nodes_flow(char* fname, FLOW f) {
   
    FILE *fp;
	errno_t err;
	// �������� ����� ��� ������.
	if ((err = fopen_s( &fp, fname, "w")) != 0) {
		printf("Create File Error\n");
	}
	else {
		integer i=0;
        // ������ ������ - ��������� �����.
		for (i=0; i<f.maxnod; i++) {
		#if doubleintprecision == 1
			fprintf(fp,"%lld %e %e %e\n",i+1,f.pa[i].x,f.pa[i].y,f.pa[i].z);
		#else
			fprintf(fp,"%d %e %e %e\n",i+1,f.pa[i].x,f.pa[i].y,f.pa[i].z);
		#endif
			
		}
		fprintf(fp,"\n");
		// ������ ������ - ����������� ������.
		for (i=0; i<f.maxelm; i++) {
		#if doubleintprecision == 1
			fprintf(fp,"%lld %lld %lld %lld %lld",i+1,f.nvtx[0][i],f.nvtx[1][i],f.nvtx[2][i],f.nvtx[3][i]);
			fprintf(fp," %lld %lld %lld %lld",f.nvtx[4][i],f.nvtx[5][i],f.nvtx[6][i],f.nvtx[7][i]);
			fprintf(fp," %lld %lld %lld", f.sosedi[ESIDE][i].iNODE1, f.sosedi[NSIDE][i].iNODE1, f.sosedi[TSIDE][i].iNODE1);
			fprintf(fp," %lld %lld %lld", f.sosedi[WSIDE][i].iNODE1, f.sosedi[SSIDE][i].iNODE1, f.sosedi[BSIDE][i].iNODE1);
			fprintf(fp," %lld %lld %lld", f.sosedi[EE][i].iNODE1, f.sosedi[NN][i].iNODE1, f.sosedi[TTSIDE][i].iNODE1);
			fprintf(fp," %lld %lld %lld", f.sosedi[WW][i].iNODE1, f.sosedi[SS][i].iNODE1, f.sosedi[BB][i].iNODE1);
		#else
			fprintf(fp,"%d %d %d %d %d",i+1,f.nvtx[0][i],f.nvtx[1][i],f.nvtx[2][i],f.nvtx[3][i]);
			fprintf(fp," %d %d %d %d",f.nvtx[4][i],f.nvtx[5][i],f.nvtx[6][i],f.nvtx[7][i]);
			fprintf(fp," %d %d %d", f.sosedi[ESIDE][i].iNODE1, f.sosedi[NSIDE][i].iNODE1, f.sosedi[TSIDE][i].iNODE1);
			fprintf(fp," %d %d %d", f.sosedi[WSIDE][i].iNODE1, f.sosedi[SSIDE][i].iNODE1, f.sosedi[BSIDE][i].iNODE1);
			fprintf(fp," %d %d %d", f.sosedi[EE][i].iNODE1, f.sosedi[NN][i].iNODE1, f.sosedi[TTSIDE][i].iNODE1);
			fprintf(fp," %d %d %d", f.sosedi[WW][i].iNODE1, f.sosedi[SS][i].iNODE1, f.sosedi[BB][i].iNODE1);
		#endif
			
			fprintf(fp," %e %e %e\n", f.prop[RHO][i], f.prop[MU][i], f.prop[BETA_T][i]);
		}
        fprintf(fp,"\n");
		
        fclose(fp); // �������� �����
	}
} // meshin_nodes_flow
*/


#endif