// ���� SpallartAllmareres.cpp �������� 
// ������ ������� ��� ��������� �������������� RANS
// �������� ��������� 1992 ����.
// ������ 13.00 28.09.2019. ��������� 22:15 02.10.2019.
// ��� ��������������� �����.
// my_elmatr_quad_SpallartAllmares3D: ������ 13:00 28.09.2019 -> ��������� 17:33 28.09.2019
// my_elmatr_quad_SpallartAllmares3D_bound �������� 20:12  28.09.2019.
// �������������� ����� � ��� ���� �����.
// �������� ������ ��� ������������ ����������, �������������� ���� �����������.
// green_gauss_SpallartAllmares: ������ 17:00 29.09.2019 -> ��������� 18:17 29.09.2019
// ������������� my_elmatr_quad_T3D ��� ������ �������������� �������� ���������. ��������� 22:14 29.09.2019
// ������ ������������ - 16:04 30.09.2019.

#pragma once
#ifndef MY_SPALART_ALLMARES_CPP
#define MY_SPALART_ALLMARES_CPP 1

// �������� ���� ��������� ������� ���� ��� ����������� ��������� 
// ��������� - ��������, ��� ������������ ����������� ������������ ������.
// ��� ������������� ��������� ������������ Hex �����.
// ��� ������ ����������� ������ ��� ���������������� �������������� 
// ������������ ��������.
void my_elmatr_quad_SpallartAllmares3D(
	integer iP, 
	BOUND* border_neighbor,
	integer lw, 
	integer ls, 
	equation3D** &sl, 
	equation3D_bon** &slb,
	//doublereal** diag_coef,
	//integer iVar,
	//bool btimedep,
	//doublereal tauparam,
	integer* ptr,
	integer** nvtx, 
	doublereal** potent,
	//doublereal* potent_temper,
	TOCHKA* pa,
	doublereal** prop,
	doublereal** prop_b,
	integer maxelm,
	ALICE_PARTITION** neighbors_for_the_internal_node,
	//doublereal* alpha,
	//doublereal dgx,
	//doublereal dgy,
	//doublereal dgz,
	//doublereal dbeta, 
	integer ishconvection,
	//bool bBussineskApproach,
	//doublereal temp_ref,
	//bool bfirst_start,
	//doublereal RCh, 
	//integer iflowregime,
	//doublereal* speedoldtimestep, 
	//doublereal* toldtimestep,
	BLOCK* b, 
	integer lb, 
	TPROP* matlist,
	doublereal** &mf,
	//bool bVERYStable,
	//doublereal &sumanb,
	integer *ilevel_alice,
	doublereal* &distance_to_wall,
	doublereal* &SInvariantStrainRateTensor
) {

	// iP - ����� ����������� ������������ ������
	// iP ���������� �� 0 �� maxelm-1.
	integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	iE = neighbors_for_the_internal_node[E_SIDE][iP].iNODE1; iN = neighbors_for_the_internal_node[N_SIDE][iP].iNODE1; iT = neighbors_for_the_internal_node[T_SIDE][iP].iNODE1;
	iW = neighbors_for_the_internal_node[W_SIDE][iP].iNODE1; iS = neighbors_for_the_internal_node[S_SIDE][iP].iNODE1; iB = neighbors_for_the_internal_node[B_SIDE][iP].iNODE1;
	sl[NUSHA_SL][iP].iE = iE; sl[NUSHA_SL][iP].iN = iN; sl[NUSHA_SL][iP].iT = iT;
	sl[NUSHA_SL][iP].iS = iS; sl[NUSHA_SL][iP].iW = iW; sl[NUSHA_SL][iP].iB = iB;
	sl[NUSHA_SL][iP].iP = iP;

	// 26.09.2016 ������� ��� ���� �����.
	integer iE2, iN2, iT2, iW2, iS2, iB2; // ������ �������� ����������� �������
	integer iE3, iN3, iT3, iW3, iS3, iB3; // ������ �������� ����������� �������
	integer iE4, iN4, iT4, iW4, iS4, iB4; // ������ �������� ����������� �������


	 // NON_EXISTENT_NODE ���� �� ������������ � [0..maxelm+maxbound-1] ���� ������������.

	iE2 = neighbors_for_the_internal_node[E_SIDE][iP].iNODE2; iN2 = neighbors_for_the_internal_node[N_SIDE][iP].iNODE2; iT2 = neighbors_for_the_internal_node[T_SIDE][iP].iNODE2;
	iW2 = neighbors_for_the_internal_node[W_SIDE][iP].iNODE2; iS2 = neighbors_for_the_internal_node[S_SIDE][iP].iNODE2; iB2 = neighbors_for_the_internal_node[B_SIDE][iP].iNODE2;
	iE3 = neighbors_for_the_internal_node[E_SIDE][iP].iNODE3; iN3 = neighbors_for_the_internal_node[N_SIDE][iP].iNODE3; iT3 = neighbors_for_the_internal_node[T_SIDE][iP].iNODE3;
	iW3 = neighbors_for_the_internal_node[W_SIDE][iP].iNODE3; iS3 = neighbors_for_the_internal_node[S_SIDE][iP].iNODE3; iB3 = neighbors_for_the_internal_node[B_SIDE][iP].iNODE3;
	iE4 = neighbors_for_the_internal_node[E_SIDE][iP].iNODE4; iN4 = neighbors_for_the_internal_node[N_SIDE][iP].iNODE4; iT4 = neighbors_for_the_internal_node[T_SIDE][iP].iNODE4;
	iW4 = neighbors_for_the_internal_node[W_SIDE][iP].iNODE4; iS4 = neighbors_for_the_internal_node[S_SIDE][iP].iNODE4; iB4 = neighbors_for_the_internal_node[B_SIDE][iP].iNODE4;

	sl[NUSHA_SL][iP].iE2 = iE2; sl[NUSHA_SL][iP].iN2 = iN2; sl[NUSHA_SL][iP].iT2 = iT2;
	sl[NUSHA_SL][iP].iS2 = iS2; sl[NUSHA_SL][iP].iW2 = iW2; sl[NUSHA_SL][iP].iB2 = iB2;

	sl[NUSHA_SL][iP].iE3 = iE3; sl[NUSHA_SL][iP].iN3 = iN3; sl[NUSHA_SL][iP].iT3 = iT3;
	sl[NUSHA_SL][iP].iS3 = iS3; sl[NUSHA_SL][iP].iW3 = iW3; sl[NUSHA_SL][iP].iB3 = iB3;

	sl[NUSHA_SL][iP].iE4 = iE4; sl[NUSHA_SL][iP].iN4 = iN4; sl[NUSHA_SL][iP].iT4 = iT4;
	sl[NUSHA_SL][iP].iS4 = iS4; sl[NUSHA_SL][iP].iW4 = iW4; sl[NUSHA_SL][iP].iB4 = iB4;



	// ���������������� ���������.
	sl[NUSHA_SL][iP].ae = 0.0;
	sl[NUSHA_SL][iP].aw = 0.0;
	sl[NUSHA_SL][iP].an = 0.0;
	sl[NUSHA_SL][iP].as = 0.0;
	sl[NUSHA_SL][iP].at = 0.0;
	sl[NUSHA_SL][iP].ab = 0.0;

	sl[NUSHA_SL][iP].ae2 = 0.0;
	sl[NUSHA_SL][iP].aw2 = 0.0;
	sl[NUSHA_SL][iP].an2 = 0.0;
	sl[NUSHA_SL][iP].as2 = 0.0;
	sl[NUSHA_SL][iP].at2 = 0.0;
	sl[NUSHA_SL][iP].ab2 = 0.0;

	sl[NUSHA_SL][iP].ae3 = 0.0;
	sl[NUSHA_SL][iP].aw3 = 0.0;
	sl[NUSHA_SL][iP].an3 = 0.0;
	sl[NUSHA_SL][iP].as3 = 0.0;
	sl[NUSHA_SL][iP].at3 = 0.0;
	sl[NUSHA_SL][iP].ab3 = 0.0;

	sl[NUSHA_SL][iP].ae4 = 0.0;
	sl[NUSHA_SL][iP].aw4 = 0.0;
	sl[NUSHA_SL][iP].an4 = 0.0;
	sl[NUSHA_SL][iP].as4 = 0.0;
	sl[NUSHA_SL][iP].at4 = 0.0;
	sl[NUSHA_SL][iP].ab4 = 0.0;


	// ������� ����������� �����.
	// �� ������� ������ ����� ���������� � ����� �������� ������ ���.
	sl[NUSHA_SL][iP].bE2 = false; sl[NUSHA_SL][iP].bW2 = false; sl[NUSHA_SL][iP].bS2 = false;
	sl[NUSHA_SL][iP].bN2 = false; sl[NUSHA_SL][iP].bB2 = false; sl[NUSHA_SL][iP].bT2 = false;

	sl[NUSHA_SL][iP].bE3 = false; sl[NUSHA_SL][iP].bW3 = false; sl[NUSHA_SL][iP].bS3 = false;
	sl[NUSHA_SL][iP].bN3 = false; sl[NUSHA_SL][iP].bB3 = false; sl[NUSHA_SL][iP].bT3 = false;

	sl[NUSHA_SL][iP].bE4 = false; sl[NUSHA_SL][iP].bW4 = false; sl[NUSHA_SL][iP].bS4 = false;
	sl[NUSHA_SL][iP].bN4 = false; sl[NUSHA_SL][iP].bB4 = false; sl[NUSHA_SL][iP].bT4 = false;

	if (CHECK_NODE_FOR_EXISTENCE(iE2)) sl[NUSHA_SL][iP].bE2 = true;
	if (CHECK_NODE_FOR_EXISTENCE(iW2)) sl[NUSHA_SL][iP].bW2 = true;
	if (CHECK_NODE_FOR_EXISTENCE(iN2)) sl[NUSHA_SL][iP].bN2 = true;
	if (CHECK_NODE_FOR_EXISTENCE(iS2)) sl[NUSHA_SL][iP].bS2 = true;
	if (CHECK_NODE_FOR_EXISTENCE(iT2)) sl[NUSHA_SL][iP].bT2 = true;
	if (CHECK_NODE_FOR_EXISTENCE(iB2)) sl[NUSHA_SL][iP].bB2 = true;

	if (CHECK_NODE_FOR_EXISTENCE(iE3)) sl[NUSHA_SL][iP].bE3 = true;
	if (CHECK_NODE_FOR_EXISTENCE(iW3)) sl[NUSHA_SL][iP].bW3 = true;
	if (CHECK_NODE_FOR_EXISTENCE(iN3)) sl[NUSHA_SL][iP].bN3 = true;
	if (CHECK_NODE_FOR_EXISTENCE(iS3)) sl[NUSHA_SL][iP].bS3 = true;
	if (CHECK_NODE_FOR_EXISTENCE(iT3)) sl[NUSHA_SL][iP].bT3 = true;
	if (CHECK_NODE_FOR_EXISTENCE(iB3)) sl[NUSHA_SL][iP].bB3 = true;

	if (CHECK_NODE_FOR_EXISTENCE(iE4)) sl[NUSHA_SL][iP].bE4 = true;
	if (CHECK_NODE_FOR_EXISTENCE(iW4)) sl[NUSHA_SL][iP].bW4 = true;
	if (CHECK_NODE_FOR_EXISTENCE(iN4)) sl[NUSHA_SL][iP].bN4 = true;
	if (CHECK_NODE_FOR_EXISTENCE(iS4)) sl[NUSHA_SL][iP].bS4 = true;
	if (CHECK_NODE_FOR_EXISTENCE(iT4)) sl[NUSHA_SL][iP].bT4 = true;
	if (CHECK_NODE_FOR_EXISTENCE(iB4)) sl[NUSHA_SL][iP].bB4 = true;

	// ���������� ��.	

	// ���� � ����� �� ������ ����� ������� ��������� �������
	// �� ��������������� ���������� ����� true
	bool bE = false, bN = false, bT = false, bW = false, bS = false, bB = false;

	if (iE >= maxelm) bE = true;
	if (iN >= maxelm) bN = true;
	if (iT >= maxelm) bT = true;
	if (iW >= maxelm) bW = true;
	if (iS >= maxelm) bS = true;
	if (iB >= maxelm) bB = true;

	bool bE2 = false, bN2 = false, bT2 = false, bW2 = false, bS2 = false, bB2 = false;

	if (iE2 >= maxelm) bE2 = true;
	if (iN2 >= maxelm) bN2 = true;
	if (iT2 >= maxelm) bT2 = true;
	if (iW2 >= maxelm) bW2 = true;
	if (iS2 >= maxelm) bS2 = true;
	if (iB2 >= maxelm) bB2 = true;

	bool bE3 = false, bN3 = false, bT3 = false, bW3 = false, bS3 = false, bB3 = false;

	if (iE3 >= maxelm) bE3 = true;
	if (iN3 >= maxelm) bN3 = true;
	if (iT3 >= maxelm) bT3 = true;
	if (iW3 >= maxelm) bW3 = true;
	if (iS3 >= maxelm) bS3 = true;
	if (iB3 >= maxelm) bB3 = true;

	bool bE4 = false, bN4 = false, bT4 = false, bW4 = false, bS4 = false, bB4 = false;

	if (iE4 >= maxelm) bE4 = true;
	if (iN4 >= maxelm) bN4 = true;
	if (iT4 >= maxelm) bT4 = true;
	if (iW4 >= maxelm) bW4 = true;
	if (iS4 >= maxelm) bS4 = true;
	if (iB4 >= maxelm) bB4 = true;

	// ���������� �������� �������� ������������ ������:
	doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������ ������
	volume3D(iP, nvtx, pa, dx, dy, dz);

	//printf("%.2f %.2f\n",dx,dy); // debug GOOD
	//getchar();

	doublereal dxe = 0.5*dx, dxw = 0.5*dx, dyn = 0.5*dy, dys = 0.5*dy, dzt = 0.5*dz, dzb = 0.5*dz;
	// �.�. �������� ��������� ������ ����, �� ����� ��� ������������
	// x - direction
	if (CHECK_NODE_FOR_EXISTENCE(iE)) {
		if (!bE) dxe = 0.5*(pa[nvtx[1][iE] - 1].x + pa[nvtx[0][iE] - 1].x);
		if (!bE) dxe -= 0.5*(pa[nvtx[1][iP] - 1].x + pa[nvtx[0][iP] - 1].x);
	}
	if (CHECK_NODE_FOR_EXISTENCE(iW)) {
		if (!bW) dxw = 0.5*(pa[nvtx[1][iP] - 1].x + pa[nvtx[0][iP] - 1].x);
		if (!bW) dxw -= 0.5*(pa[nvtx[1][iW] - 1].x + pa[nvtx[0][iW] - 1].x);
	}
	// y - direction
	if (CHECK_NODE_FOR_EXISTENCE(iN)) {
		if (!bN) dyn = 0.5*(pa[nvtx[2][iN] - 1].y + pa[nvtx[0][iN] - 1].y);
		if (!bN) dyn -= 0.5*(pa[nvtx[2][iP] - 1].y + pa[nvtx[0][iP] - 1].y);
	}
	if (CHECK_NODE_FOR_EXISTENCE(iS)) {
		if (!bS) dys = 0.5*(pa[nvtx[2][iP] - 1].y + pa[nvtx[0][iP] - 1].y);
		if (!bS) dys -= 0.5*(pa[nvtx[2][iS] - 1].y + pa[nvtx[0][iS] - 1].y);
	}
	// z - direction
	if (CHECK_NODE_FOR_EXISTENCE(iT)) {
		if (!bT) dzt = 0.5*(pa[nvtx[4][iT] - 1].z + pa[nvtx[0][iT] - 1].z);
		if (!bT) dzt -= 0.5*(pa[nvtx[4][iP] - 1].z + pa[nvtx[0][iP] - 1].z);
	}
	if (CHECK_NODE_FOR_EXISTENCE(iB)) {
		if (!bB) dzb = 0.5*(pa[nvtx[4][iP] - 1].z + pa[nvtx[0][iP] - 1].z);
		if (!bB) dzb -= 0.5*(pa[nvtx[4][iB] - 1].z + pa[nvtx[0][iB] - 1].z);
	}


	doublereal dxe2 = 0.5*dx, dxw2 = 0.5*dx, dyn2 = 0.5*dy, dys2 = 0.5*dy, dzt2 = 0.5*dz, dzb2 = 0.5*dz;
	doublereal dxe3 = 0.5*dx, dxw3 = 0.5*dx, dyn3 = 0.5*dy, dys3 = 0.5*dy, dzt3 = 0.5*dz, dzb3 = 0.5*dz;
	doublereal dxe4 = 0.5*dx, dxw4 = 0.5*dx, dyn4 = 0.5*dy, dys4 = 0.5*dy, dzt4 = 0.5*dz, dzb4 = 0.5*dz;

	// �.�. �������� ��������� ������ ����, �� ����� ��� ������������
	// x - direction
	if (CHECK_NODE_FOR_EXISTENCE(iE2)) {
		if (!bE2) dxe2 = 0.5*(pa[nvtx[1][iE2] - 1].x + pa[nvtx[0][iE2] - 1].x);
		if (!bE2) dxe2 -= 0.5*(pa[nvtx[1][iP] - 1].x + pa[nvtx[0][iP] - 1].x);
	}
	if (CHECK_NODE_FOR_EXISTENCE(iW2)) {
		if (!bW2) dxw2 = 0.5*(pa[nvtx[1][iP] - 1].x + pa[nvtx[0][iP] - 1].x);
		if (!bW2) dxw2 -= 0.5*(pa[nvtx[1][iW2] - 1].x + pa[nvtx[0][iW2] - 1].x);
	}
	// y - direction
	if (CHECK_NODE_FOR_EXISTENCE(iN2)) {
		if (!bN2) dyn2 = 0.5*(pa[nvtx[2][iN2] - 1].y + pa[nvtx[0][iN2] - 1].y);
		if (!bN2) dyn2 -= 0.5*(pa[nvtx[2][iP] - 1].y + pa[nvtx[0][iP] - 1].y);
	}
	if (CHECK_NODE_FOR_EXISTENCE(iS2)) {
		if (!bS2) dys2 = 0.5*(pa[nvtx[2][iP] - 1].y + pa[nvtx[0][iP] - 1].y);
		if (!bS2) dys2 -= 0.5*(pa[nvtx[2][iS2] - 1].y + pa[nvtx[0][iS2] - 1].y);
	}
	// z - direction
	if (CHECK_NODE_FOR_EXISTENCE(iT2)) {
		if (!bT2) dzt2 = 0.5*(pa[nvtx[4][iT2] - 1].z + pa[nvtx[0][iT2] - 1].z);
		if (!bT2) dzt2 -= 0.5*(pa[nvtx[4][iP] - 1].z + pa[nvtx[0][iP] - 1].z);
	}
	if (CHECK_NODE_FOR_EXISTENCE(iB2)) {
		if (!bB2) dzb2 = 0.5*(pa[nvtx[4][iP] - 1].z + pa[nvtx[0][iP] - 1].z);
		if (!bB2) dzb2 -= 0.5*(pa[nvtx[4][iB2] - 1].z + pa[nvtx[0][iB2] - 1].z);
	}

	// �.�. �������� ��������� ������ ����, �� ����� ��� ������������
	// x - direction
	if (CHECK_NODE_FOR_EXISTENCE(iE3)) {
		if (!bE3) dxe3 = 0.5*(pa[nvtx[1][iE3] - 1].x + pa[nvtx[0][iE3] - 1].x);
		if (!bE3) dxe3 -= 0.5*(pa[nvtx[1][iP] - 1].x + pa[nvtx[0][iP] - 1].x);
	}
	if (CHECK_NODE_FOR_EXISTENCE(iW3)) {
		if (!bW3) dxw3 = 0.5*(pa[nvtx[1][iP] - 1].x + pa[nvtx[0][iP] - 1].x);
		if (!bW3) dxw3 -= 0.5*(pa[nvtx[1][iW3] - 1].x + pa[nvtx[0][iW3] - 1].x);
	}
	// y - direction
	if (CHECK_NODE_FOR_EXISTENCE(iN3)) {
		if (!bN3) dyn3 = 0.5*(pa[nvtx[2][iN3] - 1].y + pa[nvtx[0][iN3] - 1].y);
		if (!bN3) dyn3 -= 0.5*(pa[nvtx[2][iP] - 1].y + pa[nvtx[0][iP] - 1].y);
	}
	if (CHECK_NODE_FOR_EXISTENCE(iS3)) {
		if (!bS3) dys3 = 0.5*(pa[nvtx[2][iP] - 1].y + pa[nvtx[0][iP] - 1].y);
		if (!bS3) dys3 -= 0.5*(pa[nvtx[2][iS3] - 1].y + pa[nvtx[0][iS3] - 1].y);
	}
	// z - direction
	if (CHECK_NODE_FOR_EXISTENCE(iT3)) {
		if (!bT3) dzt3 = 0.5*(pa[nvtx[4][iT3] - 1].z + pa[nvtx[0][iT3] - 1].z);
		if (!bT3) dzt3 -= 0.5*(pa[nvtx[4][iP] - 1].z + pa[nvtx[0][iP] - 1].z);
	}
	if (CHECK_NODE_FOR_EXISTENCE(iB3)) {
		if (!bB3) dzb3 = 0.5*(pa[nvtx[4][iP] - 1].z + pa[nvtx[0][iP] - 1].z);
		if (!bB3) dzb3 -= 0.5*(pa[nvtx[4][iB3] - 1].z + pa[nvtx[0][iB3] - 1].z);
	}

	// �.�. �������� ��������� ������ ����, �� ����� ��� ������������
	// x - direction
	if (CHECK_NODE_FOR_EXISTENCE(iE4)) {
		if (!bE4) dxe4 = 0.5*(pa[nvtx[1][iE4] - 1].x + pa[nvtx[0][iE4] - 1].x);
		if (!bE4) dxe4 -= 0.5*(pa[nvtx[1][iP] - 1].x + pa[nvtx[0][iP] - 1].x);
	}
	if (CHECK_NODE_FOR_EXISTENCE(iW4)) {
		if (!bW4) dxw4 = 0.5*(pa[nvtx[1][iP] - 1].x + pa[nvtx[0][iP] - 1].x);
		if (!bW4) dxw4 -= 0.5*(pa[nvtx[1][iW4] - 1].x + pa[nvtx[0][iW4] - 1].x);
	}
	// y - direction
	if (CHECK_NODE_FOR_EXISTENCE(iN4)) {
		if (!bN4) dyn4 = 0.5*(pa[nvtx[2][iN4] - 1].y + pa[nvtx[0][iN4] - 1].y);
		if (!bN4) dyn4 -= 0.5*(pa[nvtx[2][iP] - 1].y + pa[nvtx[0][iP] - 1].y);
	}
	if (CHECK_NODE_FOR_EXISTENCE(iS4)) {
		if (!bS4) dys4 = 0.5*(pa[nvtx[2][iP] - 1].y + pa[nvtx[0][iP] - 1].y);
		if (!bS4) dys4 -= 0.5*(pa[nvtx[2][iS4] - 1].y + pa[nvtx[0][iS4] - 1].y);
	}
	// z - direction
	if (CHECK_NODE_FOR_EXISTENCE(iT4)) {
		if (!bT4) dzt4 = 0.5*(pa[nvtx[4][iT4] - 1].z + pa[nvtx[0][iT4] - 1].z);
		if (!bT4) dzt4 -= 0.5*(pa[nvtx[4][iP] - 1].z + pa[nvtx[0][iP] - 1].z);
	}
	if (CHECK_NODE_FOR_EXISTENCE(iB4)) {
		if (!bB4) dzb4 = 0.5*(pa[nvtx[4][iP] - 1].z + pa[nvtx[0][iP] - 1].z);
		if (!bB4) dzb4 -= 0.5*(pa[nvtx[4][iB4] - 1].z + pa[nvtx[0][iB4] - 1].z);
	}


	// ���� ��������������� ��������� �����:
	doublereal feplus, fwplus, fnplus, fsplus, ftplus, fbplus;
	// x-direction
	feplus = 0.5*dx / dxe;
	fwplus = 0.5*dx / dxw;
	// y-direction
	fnplus = 0.5*dy / dyn;
	fsplus = 0.5*dy / dys;
	// z-direction
	ftplus = 0.5*dz / dzt;
	fbplus = 0.5*dz / dzb;

	doublereal feplus2, fwplus2, fnplus2, fsplus2, ftplus2, fbplus2;
	// x-direction
	feplus2 = 0.5*dx / dxe2;
	fwplus2 = 0.5*dx / dxw2;
	// y-direction
	fnplus2 = 0.5*dy / dyn2;
	fsplus2 = 0.5*dy / dys2;
	// z-direction
	ftplus2 = 0.5*dz / dzt2;
	fbplus2 = 0.5*dz / dzb2;

	doublereal feplus3, fwplus3, fnplus3, fsplus3, ftplus3, fbplus3;
	// x-direction
	feplus3 = 0.5*dx / dxe3;
	fwplus3 = 0.5*dx / dxw3;
	// y-direction
	fnplus3 = 0.5*dy / dyn3;
	fsplus3 = 0.5*dy / dys3;
	// z-direction
	ftplus3 = 0.5*dz / dzt3;
	fbplus3 = 0.5*dz / dzb3;

	doublereal feplus4, fwplus4, fnplus4, fsplus4, ftplus4, fbplus4;
	// x-direction
	feplus4 = 0.5*dx / dxe4;
	fwplus4 = 0.5*dx / dxw4;
	// y-direction
	fnplus4 = 0.5*dy / dyn4;
	fsplus4 = 0.5*dy / dys4;
	// z-direction
	ftplus4 = 0.5*dz / dzt4;
	fbplus4 = 0.5*dz / dzb4;

	//printf("%e %e %e %e %e %e\n",feplus, fwplus, fnplus, fsplus, ftplus, fbplus);
	//getchar();

	// ��������� �� ����� �� ���������������� ������� �������������	
	doublereal rP, rE = 0.0, rN = 0.0, rT = 0.0, rW = 0.0, rS = 0.0, rB = 0.0;
	rP = prop[RHO][iP];
	if (iE > -1) {
		if (!bE) rE = prop[RHO][iE]; else rE = prop_b[RHO][iE - maxelm];
	}
	if (iN > -1) {
		if (!bN) rN = prop[RHO][iN]; else rN = prop_b[RHO][iN - maxelm];
	}
	if (iT > -1) {
		if (!bT) rT = prop[RHO][iT]; else rT = prop_b[RHO][iT - maxelm];
	}
	if (iW > -1) {
		if (!bW) rW = prop[RHO][iW]; else rW = prop_b[RHO][iW - maxelm];
	}
	if (iS > -1) {
		if (!bS) rS = prop[RHO][iS]; else rS = prop_b[RHO][iS - maxelm];
	}
	if (iB > -1) {
		if (!bB) rB = prop[RHO][iB]; else rB = prop_b[RHO][iB - maxelm];
	}

	doublereal  rE2 = 0.0, rN2 = 0.0, rT2 = 0.0, rW2 = 0.0, rS2 = 0.0, rB2 = 0.0;
	if (iE2 > -1) {
		if (!bE2) rE2 = prop[RHO][iE2]; else rE2 = prop_b[RHO][iE2 - maxelm];
	}
	if (iN2 > -1) {
		if (!bN2) rN2 = prop[RHO][iN2]; else rN2 = prop_b[RHO][iN2 - maxelm];
	}
	if (iT2 > -1) {
		if (!bT2) rT2 = prop[RHO][iT2]; else rT2 = prop_b[RHO][iT2 - maxelm];
	}
	if (iW2 > -1) {
		if (!bW2) rW2 = prop[RHO][iW2]; else rW2 = prop_b[RHO][iW2 - maxelm];
	}
	if (iS2 > -1) {
		if (!bS2) rS2 = prop[RHO][iS2]; else rS2 = prop_b[RHO][iS2 - maxelm];
	}
	if (iB2 > -1) {
		if (!bB2) rB2 = prop[RHO][iB2]; else rB2 = prop_b[RHO][iB2 - maxelm];
	}

	doublereal  rE3 = 0.0, rN3 = 0.0, rT3 = 0.0, rW3 = 0.0, rS3 = 0.0, rB3 = 0.0;
	if (iE3 > -1) {
		if (!bE3) rE3 = prop[RHO][iE3]; else rE3 = prop_b[RHO][iE3 - maxelm];
	}
	if (iN3 > -1) {
		if (!bN3) rN3 = prop[RHO][iN3]; else rN3 = prop_b[RHO][iN3 - maxelm];
	}
	if (iT3 > -1) {
		if (!bT3) rT3 = prop[RHO][iT3]; else rT3 = prop_b[RHO][iT3 - maxelm];
	}
	if (iW3 > -1) {
		if (!bW3) rW3 = prop[RHO][iW3]; else rW3 = prop_b[RHO][iW3 - maxelm];
	}
	if (iS3 > -1) {
		if (!bS3) rS3 = prop[RHO][iS3]; else rS3 = prop_b[RHO][iS3 - maxelm];
	}
	if (iB3 > -1) {
		if (!bB3) rB3 = prop[RHO][iB3]; else rB3 = prop_b[RHO][iB3 - maxelm];
	}

	doublereal  rE4 = 0.0, rN4 = 0.0, rT4 = 0.0, rW4 = 0.0, rS4 = 0.0, rB4 = 0.0;
	if (iE4 > -1) {
		if (!bE4) rE4 = prop[RHO][iE4]; else rE4 = prop_b[RHO][iE4 - maxelm];
	}
	if (iN4 > -1) {
		if (!bN4) rN4 = prop[RHO][iN4]; else rN4 = prop_b[RHO][iN4 - maxelm];
	}
	if (iT4 > -1) {
		if (!bT4) rT4 = prop[RHO][iT4]; else rT4 = prop_b[RHO][iT4 - maxelm];
	}
	if (iW4 > -1) {
		if (!bW4) rW4 = prop[RHO][iW4]; else rW4 = prop_b[RHO][iW4 - maxelm];
	}
	if (iS4 > -1) {
		if (!bS4) rS4 = prop[RHO][iS4]; else rS4 = prop_b[RHO][iS4 - maxelm];
	}
	if (iB4 > -1) {
		if (!bB4) rB4 = prop[RHO][iB4]; else rB4 = prop_b[RHO][iB4 - maxelm];
	}

	/*
	doublereal rhoe, rhow, rhon, rhos, rhot, rhob;
	// �������� ���������  �� ����� ��:
	rhoe=rE*rP/(feplus*rE+(1.0-feplus)*rP);  // ���������.
	rhow=rW*rP/(fwplus*rW+(1.0-fwplus)*rP);
	rhon=rN*rP/(fnplus*rN+(1.0-fnplus)*rP);
	rhos=rS*rP/(fsplus*rS+(1.0-fsplus)*rP);
	rhot=rT*rP/(ftplus*rT+(1.0-ftplus)*rP);
	rhob=rB*rP/(fbplus*rB+(1.0-fbplus)*rP);


	doublereal rhoe2, rhow2, rhon2, rhos2, rhot2, rhob2;
	doublereal rhoe3, rhow3, rhon3, rhos3, rhot3, rhob3;
	doublereal rhoe4, rhow4, rhon4, rhos4, rhot4, rhob4;

	rhoe2 = rE2 * rP / (feplus2*rE2 + (1.0 - feplus2)*rP);
	rhow2 = rW2 * rP / (fwplus2*rW2 + (1.0 - fwplus2)*rP);
	rhon2 = rN2 * rP / (fnplus2*rN2 + (1.0 - fnplus2)*rP);
	rhos2 = rS2 * rP / (fsplus2*rS2 + (1.0 - fsplus2)*rP);
	rhot2 = rT2 * rP / (ftplus2*rT2 + (1.0 - ftplus2)*rP);
	rhob2 = rB2 * rP / (fbplus2*rB2 + (1.0 - fbplus2)*rP);

	rhoe3 = rE3 * rP / (feplus3*rE3 + (1.0 - feplus3)*rP);
	rhow3 = rW3 * rP / (fwplus3*rW3 + (1.0 - fwplus3)*rP);
	rhon3 = rN3 * rP / (fnplus3*rN3 + (1.0 - fnplus3)*rP);
	rhos3 = rS3 * rP / (fsplus3*rS3 + (1.0 - fsplus3)*rP);
	rhot3 = rT3 * rP / (ftplus3*rT3 + (1.0 - ftplus3)*rP);
	rhob3 = rB3 * rP / (fbplus3*rB3 + (1.0 - fbplus3)*rP);

	rhoe4 = rE4 * rP / (feplus4*rE4 + (1.0 - feplus4)*rP);
	rhow4 = rW4 * rP / (fwplus4*rW4 + (1.0 - fwplus4)*rP);
	rhon4 = rN4 * rP / (fnplus4*rN4 + (1.0 - fnplus4)*rP);
	rhos4 = rS4 * rP / (fsplus4*rS4 + (1.0 - fsplus4)*rP);
	rhot4 = rT4 * rP / (ftplus4*rT4 + (1.0 - ftplus4)*rP);
	rhob4 = rB4 * rP / (fbplus4*rB4 + (1.0 - fbplus4)*rP);
	*/
	doublereal rhoe = 0.0, rhow = 0.0, rhon = 0.0, rhos = 0.0, rhot = 0.0, rhob = 0.0;
	// ������������ ��������� ������� ���, ����� ����������� 
	// ���������� �����������.
	if (iE > -1) {
		if (!bE) rhoe = rE * rP / (feplus*rE + (1.0 - feplus)*rP); else rhoe = rE; // ��������� !
	}
	if (iW > -1) {
		if (!bW) rhow = rW * rP / (fwplus*rW + (1.0 - fwplus)*rP); else rhow = rW;
	}
	if (iN > -1) {
		if (!bN) rhon = rN * rP / (fnplus*rN + (1.0 - fnplus)*rP); else rhon = rN;
	}
	if (iS > -1) {
		if (!bS) rhos = rS * rP / (fsplus*rS + (1.0 - fsplus)*rP); else rhos = rS;
	}
	if (iT > -1) {
		if (!bT) rhot = rT * rP / (ftplus*rT + (1.0 - ftplus)*rP); else rhot = rT;
	}
	if (iB > -1) {
		if (!bB) rhob = rB * rP / (fbplus*rB + (1.0 - fbplus)*rP); else rhob = rB;
	}

	doublereal rhoe2 = 0.0, rhow2 = 0.0, rhon2 = 0.0, rhos2 = 0.0, rhot2 = 0.0, rhob2 = 0.0;
	doublereal rhoe3 = 0.0, rhow3 = 0.0, rhon3 = 0.0, rhos3 = 0.0, rhot3 = 0.0, rhob3 = 0.0;
	doublereal rhoe4 = 0.0, rhow4 = 0.0, rhon4 = 0.0, rhos4 = 0.0, rhot4 = 0.0, rhob4 = 0.0;

	if (iE2 > -1) {
		if (!bE2)  rhoe2 = rE2 * rP / (feplus2*rE2 + (1.0 - feplus2)*rP); else rhoe2 = rE2; // ��������� !
	}
	if (iW2 > -1) {
		if (!bW2)  rhow2 = rW2 * rP / (fwplus2*rW2 + (1.0 - fwplus2)*rP); else rhow2 = rW2;
	}
	if (iN2 > -1) {
		if (!bN2) rhon2 = rN2 * rP / (fnplus2*rN2 + (1.0 - fnplus2)*rP); else rhon2 = rN2;
	}
	if (iS2 > -1) {
		if (!bS2)  rhos2 = rS2 * rP / (fsplus2*rS2 + (1.0 - fsplus2)*rP); else rhos2 = rS2;
	}
	if (iT2 > -1) {
		if (!bT2)  rhot2 = rT2 * rP / (ftplus2*rT2 + (1.0 - ftplus2)*rP); else rhot2 = rT2;
	}
	if (iB2 > -1) {
		if (!bB2) rhob2 = rB2 * rP / (fbplus2*rB2 + (1.0 - fbplus2)*rP); else rhob2 = rB2;
	}

	if (iE3 > -1) {
		if (!bE3) rhoe3 = rE3 * rP / (feplus3*rE3 + (1.0 - feplus3)*rP); else rhoe3 = rE3;
	}
	if (iW3 > -1) {
		if (!bW3) rhow3 = rW3 * rP / (fwplus3*rW3 + (1.0 - fwplus3)*rP); else rhow3 = rW3;
	}
	if (iN3 > -1) {
		if (!bN3) rhon3 = rN3 * rP / (fnplus3*rN3 + (1.0 - fnplus3)*rP); else rhon3 = rN3;
	}
	if (iS3 > -1) {
		if (!bS3) rhos3 = rS3 * rP / (fsplus3*rS3 + (1.0 - fsplus3)*rP); else rhos3 = rS3;
	}
	if (iT3 > -1) {
		if (!bT3) rhot3 = rT3 * rP / (ftplus3*rT3 + (1.0 - ftplus3)*rP); else rhot3 = rT3;
	}
	if (iB3 > -1) {
		if (!bB3) rhob3 = rB3 * rP / (fbplus3*rB3 + (1.0 - fbplus3)*rP); else rhob3 = rB3;
	}

	if (iE4 > -1) {
		if (!bE4) rhoe4 = rE4 * rP / (feplus4*rE4 + (1.0 - feplus4)*rP); else rhoe4 = rE4;
	}
	if (iW4 > -1) {
		if (!bW4) rhow4 = rW4 * rP / (fwplus4*rW4 + (1.0 - fwplus4)*rP); else rhow4 = rW4;
	}
	if (iN4 > -1) {
		if (!bN4) rhon4 = rN4 * rP / (fnplus4*rN4 + (1.0 - fnplus4)*rP); else rhon4 = rN4;
	}
	if (iS4 > -1) {
		if (!bS4) rhos4 = rS4 * rP / (fsplus4*rS4 + (1.0 - fsplus4)*rP); else rhos4 = rS4;
	}
	if (iT4 > -1) {
		if (!bT4) rhot4 = rT4 * rP / (ftplus4*rT4 + (1.0 - ftplus4)*rP); else rhot4 = rT4;
	}
	if (iB4 > -1) {
		if (!bB4) rhob4 = rB4 * rP / (fbplus4*rB4 + (1.0 - fbplus4)*rP); else rhob4 = rB4;
	}



	/*
	   ����������� ����������:
	   �� �������� ������������ ���-��� �������� �� ����� ��
	   ������ ���� ��������� � � ���������� ��� ��������� ��������.
	   ��� ������������� ������ ���� ��������� ��� ���������� ������� �� ����� ������������ ������
	   � ���������� ���������� ��������, ���������������� � ������������ ������������� �.�. � ����������
	   ����������� ��� ������ ����� ������������� ��������� �������������. ���� ��� ���������� ������� ��
	   ��������� ������������ ���-��� �� ���� ��������� ���������� � �� ��������� (���� ��� � ������������
	   ������ � ��������� ���������� �������� � ����������������. ��� �������� �������� ��������� �����������
	   ������� ����� ��������� ��������� ����������) �� ������ ����� �� ����� ������������� ��������� �������������
	   � ������������� ������� ����� ��������.
	   ������������ ���������� ������������ �������� �� ��� ��� ������������, �
	   �� ����������� ������ ���. ��� ����������� ����� ��������� ������������� �������� ���� ��� �� ������
	   ����������������� �������� � ��������. ��� ���������� ��������� ������������ ������������ �
	   ���������� ��� ��������� ��������. ��� ������� � ������� �������� ���������
	   SIMPLE (�� ������ ����������������� ���������� ������� ������������ ������� � ������� ��������, ��
	   ���� � ��� ��� ����� �� �� ��������� �������� ���������� ����������� ����� ������ ����� (������� ���� ��������� � ������)
	   � ������� ������� ��� ������������ ������������ �������� � ���������� ��������).
	   ��������� ������������� ��������... TODO
	   �������� ������ �������������� ������ ��������, �.�. �� ��� ������������ ������������
	   ��� ���� ����� ��� �� ���������. ������� ������������ �������� ������������ ���-��� ������
	   �� ������ �������� ��������� SIMPLE. �� ������ �������� ������������� ������� ������������
	   �������� ����� ���������� ������� �������� ������������� ��������.
	*/


	// ������������ ����� ����� ����� ��.
	// � ���������� �������� � ������ ���������������� ��������������� 
	// ���������� ���������������� �������� ���-���.
	doublereal Fe = 0.0, Fw = 0.0, Fn = 0.0, Fs = 0.0, Ft = 0.0, Fb = 0.0;


	// ��� ���� �����.
	doublereal Fe1 = 0.0, Fe2 = 0.0, Fe3 = 0.0, Fe4 = 0.0;
	doublereal Fw1 = 0.0, Fw2 = 0.0, Fw3 = 0.0, Fw4 = 0.0;
	doublereal Fn1 = 0.0, Fn2 = 0.0, Fn3 = 0.0, Fn4 = 0.0;
	doublereal Fs1 = 0.0, Fs2 = 0.0, Fs3 = 0.0, Fs4 = 0.0;
	doublereal Ft1 = 0.0, Ft2 = 0.0, Ft3 = 0.0, Ft4 = 0.0;
	doublereal Fb1 = 0.0, Fb2 = 0.0, Fb3 = 0.0, Fb4 = 0.0;

	// �������� ����� ������������ � �� �����������, 
	// ��� ������ ����������� ��������� �� �������� ������ ������� ����.
	if (!b_on_adaptive_local_refinement_mesh) {
		if (Fe != Fe) {
			printf("Fe=%e\n", Fe);
		}
		Fe = mf[iP][E_SIDE];
		if (Fe != Fe) {
			printf("Fe=%e\n", Fe);
			system("pause");
		}
		Fn = mf[iP][N_SIDE];
		Ft = mf[iP][T_SIDE];
		Fw = mf[iP][W_SIDE];
		Fs = mf[iP][S_SIDE];
		Fb = mf[iP][B_SIDE];
	}
	else {
		// TODO ����� �� ����. 24.11.2018

		if (iE > -1) {
			if (bE) {
				// ��������� ����.
				Fe = mf[iP][E_SIDE] * (border_neighbor[iE - maxelm].dS / (dy*dz));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iE]]) {
					Fe = mf[iP][E_SIDE];
				}
				else {

					Fe = mf[iE][W_SIDE];

				}
			}
		}

		if (iW > -1) {
			if (bW) {
				// ��������� ����.
				Fw = mf[iP][W_SIDE] * (border_neighbor[iW - maxelm].dS / (dy*dz));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iW]]) {
					Fw = mf[iP][W_SIDE];
				}
				else {

					Fw = mf[iW][E_SIDE];

				}
			}
		}

		if (iN > -1) {
			if (bN) {
				// ��������� ����.
				Fn = mf[iP][N_SIDE] * (border_neighbor[iN - maxelm].dS / (dx*dz));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iN]]) {
					Fn = mf[iP][N_SIDE];
				}
				else {

					Fn = mf[iN][S_SIDE];

				}
			}
		}

		if (iS > -1) {
			if (bS) {
				// ��������� ����.
				Fs = mf[iP][S_SIDE] * (border_neighbor[iS - maxelm].dS / (dx*dz));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iS]]) {
					Fs = mf[iP][S_SIDE];
				}
				else {

					Fs = mf[iS][N_SIDE];

				}
			}
		}

		if (iT > -1) {
			if (bT) {
				// ��������� ����.
				Ft = mf[iP][T_SIDE] * (border_neighbor[iT - maxelm].dS / (dx*dy));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iT]]) {
					Ft = mf[iP][T_SIDE];
				}
				else {

					Ft = mf[iT][B_SIDE];

				}
			}
		}

		if (iB > -1) {
			if (bB) {
				// ��������� ����.
				Fb = mf[iP][B_SIDE] * (border_neighbor[iB - maxelm].dS / (dx*dy));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iB]]) {
					Fb = mf[iP][B_SIDE];
				}
				else {

					Fb = mf[iB][T_SIDE];

				}
			}
		}

		if (iE2 > -1) {
			if (bE2) {
				// ��������� ����.
				Fe2 = mf[iP][E_SIDE] * (border_neighbor[iE2 - maxelm].dS / (dy*dz));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iE2]]) {
					Fe2 = mf[iP][E_SIDE];
				}
				else {

					Fe2 = mf[iE2][W_SIDE];

				}
			}
		}

		if (iW2 > -1) {
			if (bW2) {
				// ��������� ����.
				Fw2 = mf[iP][W_SIDE] * (border_neighbor[iW2 - maxelm].dS / (dy*dz));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iW2]]) {
					Fw = mf[iP][W_SIDE];
				}
				else {

					Fw = mf[iW2][E_SIDE];

				}
			}
		}

		if (iN2 > -1) {
			if (bN2) {
				// ��������� ����.
				Fn2 = mf[iP][N_SIDE] * (border_neighbor[iN2 - maxelm].dS / (dx*dz));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iN2]]) {
					Fn2 = mf[iP][N_SIDE];
				}
				else {

					Fn2 = mf[iN2][S_SIDE];

				}
			}
		}

		if (iS2 > -1) {
			if (bS2) {
				// ��������� ����.
				Fs2 = mf[iP][S_SIDE] * (border_neighbor[iS2 - maxelm].dS / (dx*dz));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iS2]]) {
					Fs2 = mf[iP][S_SIDE];
				}
				else {

					Fs2 = mf[iS2][N_SIDE];

				}
			}
		}

		if (iT2 > -1) {
			if (bT2) {
				// ��������� ����.
				Ft2 = mf[iP][T_SIDE] * (border_neighbor[iT2 - maxelm].dS / (dx*dy));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iT2]]) {
					Ft2 = mf[iP][T_SIDE];
				}
				else {

					Ft2 = mf[iT2][B_SIDE];

				}
			}
		}

		if (iB2 > -1) {
			if (bB2) {
				// ��������� ����.
				Fb2 = mf[iP][B_SIDE] * (border_neighbor[iB2 - maxelm].dS / (dx*dy));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iB2]]) {
					Fb2 = mf[iP][B_SIDE];
				}
				else {

					Fb2 = mf[iB2][T_SIDE];

				}
			}
		}


		if (iE3 > -1) {
			if (bE3) {
				// ��������� ����.
				Fe3 = mf[iP][E_SIDE] * (border_neighbor[iE3 - maxelm].dS / (dy*dz));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iE3]]) {
					Fe3 = mf[iP][E_SIDE];
				}
				else {

					Fe3 = mf[iE3][W_SIDE];

				}
			}
		}

		if (iW3 > -1) {
			if (bW3) {
				// ��������� ����.
				Fw3 = mf[iP][W_SIDE] * (border_neighbor[iW3 - maxelm].dS / (dy*dz));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iW3]]) {
					Fw3 = mf[iP][W_SIDE];
				}
				else {

					Fw3 = mf[iW3][E_SIDE];

				}
			}
		}

		if (iN3 > -1) {
			if (bN3) {
				// ��������� ����.
				Fn3 = mf[iP][N_SIDE] * (border_neighbor[iN3 - maxelm].dS / (dx*dz));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iN3]]) {
					Fn3 = mf[iP][N_SIDE];
				}
				else {

					Fn3 = mf[iN3][S_SIDE];

				}
			}
		}

		if (iS3 > -1) {
			if (bS3) {
				// ��������� ����.
				Fs3 = mf[iP][S_SIDE] * (border_neighbor[iS3 - maxelm].dS / (dx*dz));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iS3]]) {
					Fs3 = mf[iP][S_SIDE];
				}
				else {

					Fs3 = mf[iS3][N_SIDE];

				}
			}
		}

		if (iT3 > -1) {
			if (bT3) {
				// ��������� ����.
				Ft3 = mf[iP][T_SIDE] * (border_neighbor[iT3 - maxelm].dS / (dx*dy));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iT3]]) {
					Ft3 = mf[iP][T_SIDE];
				}
				else {

					Ft3 = mf[iT3][B_SIDE];

				}
			}
		}

		if (iB3 > -1) {
			if (bB3) {
				// ��������� ����.
				Fb3 = mf[iP][B_SIDE] * (border_neighbor[iB3 - maxelm].dS / (dx*dy));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iB3]]) {
					Fb3 = mf[iP][B_SIDE];
				}
				else {

					Fb3 = mf[iB3][T_SIDE];

				}
			}
		}

		if (iE4 > -1) {
			if (bE4) {
				// ��������� ����.
				Fe4 = mf[iP][E_SIDE] * (border_neighbor[iE4 - maxelm].dS / (dy*dz));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iE4]]) {
					Fe4 = mf[iP][E_SIDE];
				}
				else {

					Fe4 = mf[iE4][W_SIDE];

				}
			}
		}

		if (iW4 > -1) {
			if (bW4) {
				// ��������� ����.
				Fw4 = mf[iP][W_SIDE] * (border_neighbor[iW4 - maxelm].dS / (dy*dz));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iW4]]) {
					Fw4 = mf[iP][W_SIDE];
				}
				else {

					Fw4 = mf[iW4][E_SIDE];

				}
			}
		}

		if (iN4 > -1) {
			if (bN4) {
				// ��������� ����.
				Fn4 = mf[iP][N_SIDE] * (border_neighbor[iN4 - maxelm].dS / (dx*dz));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iN4]]) {
					Fn4 = mf[iP][N_SIDE];
				}
				else {

					Fn4 = mf[iN4][S_SIDE];

				}
			}
		}

		if (iS4 > -1) {
			if (bS4) {
				// ��������� ����.
				Fs4 = mf[iP][S_SIDE] * (border_neighbor[iS4 - maxelm].dS / (dx*dz));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iS4]]) {
					Fs4 = mf[iP][S_SIDE];
				}
				else {

					Fs4 = mf[iS4][N_SIDE];

				}
			}
		}

		if (iT4 > -1) {
			if (bT4) {
				// ��������� ����.
				Ft4 = mf[iP][T_SIDE] * (border_neighbor[iT4 - maxelm].dS / (dx*dy));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iT4]]) {
					Ft4 = mf[iP][T_SIDE];
				}
				else {

					Ft4 = mf[iT4][B_SIDE];

				}
			}
		}

		if (iB4 > -1) {
			if (bB4) {
				// ��������� ����.
				Fb4 = mf[iP][B_SIDE] * (border_neighbor[iB4 - maxelm].dS / (dx*dy));
			}
			else {
				if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iB4]]) {
					Fb4 = mf[iP][B_SIDE];
				}
				else {

					Fb4 = mf[iB4][T_SIDE];

				}
			}
		}



	}



	doublereal eps = 1e-37; // ��� ��������� ������������� ����.

	//eqin.fluidinfo[0].sigma_nu
	// ������������ ��������:
	doublereal  GP, GE, GW, GN, GS, GT, GB;
	doublereal  GE2, GW2, GN2, GS2, GT2, GB2;
	doublereal  GE3, GW3, GN3, GS3, GT3, GB3;
	doublereal  GE4, GW4, GN4, GS4, GT4, GB4;

	// ���������� ������������ ��������:
	GP = ((prop[MU_DYNAMIC_VISCOSITY][iP])+ fmax(0.0, rP*potent[NUSHA][iP]))/ eqin.fluidinfo[0].sigma_nu; // � ������ ����������� ��.
	if (iE > -1) {
		if (!bE) GE = ((prop[MU_DYNAMIC_VISCOSITY][iE]) + fmax(0.0, rE*potent[NUSHA][iE])) / eqin.fluidinfo[0].sigma_nu; else GE = ((prop_b[MU_DYNAMIC_VISCOSITY][iE - maxelm]) + rE*potent[NUSHA][iE]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iN > -1) {
		if (!bN) GN = ((prop[MU_DYNAMIC_VISCOSITY][iN]) + fmax(0.0, rN*potent[NUSHA][iN])) / eqin.fluidinfo[0].sigma_nu; else GN = ((prop_b[MU_DYNAMIC_VISCOSITY][iN - maxelm]) + rN*potent[NUSHA][iN]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iT > -1) {
		if (!bT) GT = ((prop[MU_DYNAMIC_VISCOSITY][iT]) + fmax(0.0, rT*potent[NUSHA][iT])) / eqin.fluidinfo[0].sigma_nu; else GT = ((prop_b[MU_DYNAMIC_VISCOSITY][iT - maxelm]) + rT*potent[NUSHA][iT]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iW > -1) {
		if (!bW) GW = ((prop[MU_DYNAMIC_VISCOSITY][iW]) + fmax(0.0, rW*potent[NUSHA][iW])) / eqin.fluidinfo[0].sigma_nu; else GW = ((prop_b[MU_DYNAMIC_VISCOSITY][iW - maxelm]) + rW*potent[NUSHA][iW]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iS > -1) {
		if (!bS) GS = ((prop[MU_DYNAMIC_VISCOSITY][iS]) + fmax(0.0, rS*potent[NUSHA][iS])) / eqin.fluidinfo[0].sigma_nu; else GS = ((prop_b[MU_DYNAMIC_VISCOSITY][iS - maxelm]) + rS*potent[NUSHA][iS]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iB > -1) {
		if (!bB) GB = ((prop[MU_DYNAMIC_VISCOSITY][iB]) + fmax(0.0, rB*potent[NUSHA][iB])) / eqin.fluidinfo[0].sigma_nu; else GB = ((prop_b[MU_DYNAMIC_VISCOSITY][iB - maxelm]) + rB*potent[NUSHA][iB]) / eqin.fluidinfo[0].sigma_nu;
	}

	if (iE2 > -1) {
		if (!bE2) GE2 = ((prop[MU_DYNAMIC_VISCOSITY][iE2]) + fmax(0.0, rE2*potent[NUSHA][iE2])) / eqin.fluidinfo[0].sigma_nu; else GE2 = ((prop_b[MU_DYNAMIC_VISCOSITY][iE2 - maxelm]) + rE2*potent[NUSHA][iE2]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iN2 > -1) {
		if (!bN2) GN2 = ((prop[MU_DYNAMIC_VISCOSITY][iN2]) + fmax(0.0, rN2*potent[NUSHA][iN2])) / eqin.fluidinfo[0].sigma_nu; else GN2 = ((prop_b[MU_DYNAMIC_VISCOSITY][iN2 - maxelm]) + rN2*potent[NUSHA][iN2]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iT2 > -1) {
		if (!bT2) GT2 = ((prop[MU_DYNAMIC_VISCOSITY][iT2]) + fmax(0.0, rT2*potent[NUSHA][iT2])) / eqin.fluidinfo[0].sigma_nu; else GT2 = ((prop_b[MU_DYNAMIC_VISCOSITY][iT2 - maxelm]) + rT2*potent[NUSHA][iT2]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iW2 > -1) {
		if (!bW2) GW2 = ((prop[MU_DYNAMIC_VISCOSITY][iW2]) + fmax(0.0, rW2*potent[NUSHA][iW2])) / eqin.fluidinfo[0].sigma_nu; else GW2 = ((prop_b[MU_DYNAMIC_VISCOSITY][iW2 - maxelm]) + rW2*potent[NUSHA][iW2]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iS2 > -1) {
		if (!bS2) GS2 = ((prop[MU_DYNAMIC_VISCOSITY][iS2]) + fmax(0.0, rS2*potent[NUSHA][iS2])) / eqin.fluidinfo[0].sigma_nu; else GS2 = ((prop_b[MU_DYNAMIC_VISCOSITY][iS2 - maxelm]) + rS2*potent[NUSHA][iS2]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iB2 > -1) {
		if (!bB2) GB2 = ((prop[MU_DYNAMIC_VISCOSITY][iB2]) + fmax(0.0, rB2*potent[NUSHA][iB2])) / eqin.fluidinfo[0].sigma_nu; else GB2 = ((prop_b[MU_DYNAMIC_VISCOSITY][iB2 - maxelm]) + rB2*potent[NUSHA][iB2]) / eqin.fluidinfo[0].sigma_nu;
	}

	if (iE3 > -1) {
		if (!bE3) GE3 = ((prop[MU_DYNAMIC_VISCOSITY][iE3]) + fmax(0.0, rE3*potent[NUSHA][iE3])) / eqin.fluidinfo[0].sigma_nu; else GE3 = ((prop_b[MU_DYNAMIC_VISCOSITY][iE3 - maxelm]) + rE3*potent[NUSHA][iE3]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iN3 > -1) {
		if (!bN3) GN3 = ((prop[MU_DYNAMIC_VISCOSITY][iN3]) + fmax(0.0, rN3*potent[NUSHA][iN3])) / eqin.fluidinfo[0].sigma_nu; else GN3 = ((prop_b[MU_DYNAMIC_VISCOSITY][iN3 - maxelm]) + rN3*potent[NUSHA][iN3]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iT3 > -1) {
		if (!bT3) GT3 = ((prop[MU_DYNAMIC_VISCOSITY][iT3]) + fmax(0.0, rT3*potent[NUSHA][iT3])) / eqin.fluidinfo[0].sigma_nu; else GT3 = ((prop_b[MU_DYNAMIC_VISCOSITY][iT3 - maxelm]) + rT3*potent[NUSHA][iT3]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iW3 > -1) {
		if (!bW3) GW3 = ((prop[MU_DYNAMIC_VISCOSITY][iW3]) + fmax(0.0, rW3*potent[NUSHA][iW3])) / eqin.fluidinfo[0].sigma_nu; else GW3 = ((prop_b[MU_DYNAMIC_VISCOSITY][iW3 - maxelm]) + rW3*potent[NUSHA][iW3]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iS3 > -1) {
		if (!bS3) GS3 = ((prop[MU_DYNAMIC_VISCOSITY][iS3]) + fmax(0.0, rS3*potent[NUSHA][iS3])) / eqin.fluidinfo[0].sigma_nu; else GS3 = ((prop_b[MU_DYNAMIC_VISCOSITY][iS3 - maxelm]) + rS3*potent[NUSHA][iS3]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iB3 > -1) {
		if (!bB3) GB3 = ((prop[MU_DYNAMIC_VISCOSITY][iB3]) + fmax(0.0, rB3*potent[NUSHA][iB3])) / eqin.fluidinfo[0].sigma_nu; else GB3 = ((prop_b[MU_DYNAMIC_VISCOSITY][iB3 - maxelm]) + rB3*potent[NUSHA][iB3]) / eqin.fluidinfo[0].sigma_nu;
	}

	if (iE4 > -1) {
		if (!bE4) GE4 = ((prop[MU_DYNAMIC_VISCOSITY][iE4]) + fmax(0.0, rE4*potent[NUSHA][iE4])) / eqin.fluidinfo[0].sigma_nu; else GE4 = ((prop_b[MU_DYNAMIC_VISCOSITY][iE4 - maxelm]) + rE4*potent[NUSHA][iE4]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iN4 > -1) {
		if (!bN4) GN4 = ((prop[MU_DYNAMIC_VISCOSITY][iN4]) + fmax(0.0, rN4*potent[NUSHA][iN4])) / eqin.fluidinfo[0].sigma_nu; else GN4 = ((prop_b[MU_DYNAMIC_VISCOSITY][iN4 - maxelm]) + rN4*potent[NUSHA][iN4]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iT4 > -1) {
		if (!bT4) GT4 = ((prop[MU_DYNAMIC_VISCOSITY][iT4]) + fmax(0.0, rT4*potent[NUSHA][iT4])) / eqin.fluidinfo[0].sigma_nu; else GT4 = ((prop_b[MU_DYNAMIC_VISCOSITY][iT4 - maxelm]) + rT4*potent[NUSHA][iT4]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iW4 > -1) {
		if (!bW4) GW4 = ((prop[MU_DYNAMIC_VISCOSITY][iW4]) + fmax(0.0, rW4*potent[NUSHA][iW4])) / eqin.fluidinfo[0].sigma_nu; else GW4 = ((prop_b[MU_DYNAMIC_VISCOSITY][iW4 - maxelm]) + rW4*potent[NUSHA][iW4]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iS4 > -1) {
		if (!bS4) GS4 = ((prop[MU_DYNAMIC_VISCOSITY][iS4]) + fmax(0.0, rS4*potent[NUSHA][iS4])) / eqin.fluidinfo[0].sigma_nu; else GS4 = ((prop_b[MU_DYNAMIC_VISCOSITY][iS4 - maxelm]) + rS4*potent[NUSHA][iS4]) / eqin.fluidinfo[0].sigma_nu;
	}
	if (iB4 > -1) {
		if (!bB4) GB4 = ((prop[MU_DYNAMIC_VISCOSITY][iB4]) + fmax(0.0, rB4*potent[NUSHA][iB4])) / eqin.fluidinfo[0].sigma_nu; else GB4 = ((prop_b[MU_DYNAMIC_VISCOSITY][iB4 - maxelm]) + rB4*potent[NUSHA][iB4]) / eqin.fluidinfo[0].sigma_nu;
	}

	doublereal Ge = GP, Gw = GP, Gn = GP, Gs = GP, Gt = GP, Gb = GP;
	// �������� ������������ �������� �� ����� ��.
	if (iE > -1) {
		Ge = GE * GP / (feplus*GE + (1 - feplus)*GP); // ���������.
	}
	if (iW > -1) {
		Gw = GW * GP / (fwplus*GW + (1 - fwplus)*GP);
	}
	if (iN > -1) {
		Gn = GN * GP / (fnplus*GN + (1 - fnplus)*GP);
	}
	if (iS > -1) {
		Gs = GS * GP / (fsplus*GS + (1 - fsplus)*GP);
	}
	if (iT > -1) {
		Gt = GT * GP / (ftplus*GT + (1 - ftplus)*GP);
	}
	if (iB > -1) {
		Gb = GB * GP / (fbplus*GB + (1 - fbplus)*GP);
	}

	doublereal Ge2 = GP, Gw2 = GP, Gn2 = GP, Gs2 = GP, Gt2 = GP, Gb2 = GP;
	// �������� ������������ �������� �� ����� ��.
	if (iE2 > -1) {
		Ge2 = GE2 * GP / (feplus2*GE2 + (1 - feplus2)*GP); // ���������.
	}
	if (iW2 > -1) {
		Gw2 = GW2 * GP / (fwplus2*GW2 + (1 - fwplus2)*GP);
	}
	if (iN2 > -1) {
		Gn2 = GN2 * GP / (fnplus2*GN2 + (1 - fnplus2)*GP);
	}
	if (iS2 > -1) {
		Gs2 = GS2 * GP / (fsplus2*GS2 + (1 - fsplus2)*GP);
	}
	if (iT2 > -1) {
		Gt2 = GT2 * GP / (ftplus2*GT2 + (1 - ftplus2)*GP);
	}
	if (iB2 > -1) {
		Gb2 = GB2 * GP / (fbplus2*GB2 + (1 - fbplus2)*GP);
	}


	doublereal Ge3 = GP, Gw3 = GP, Gn3 = GP, Gs3 = GP, Gt3 = GP, Gb3 = GP;
	// �������� ������������ �������� �� ����� ��.
	if (iE3 > -1) {
		Ge3 = GE3 * GP / (feplus3*GE3 + (1 - feplus3)*GP); // ���������.
	}
	if (iW3 > -1) {
		Gw3 = GW3 * GP / (fwplus3*GW3 + (1 - fwplus3)*GP);
	}
	if (iN3 > -1) {
		Gn3 = GN3 * GP / (fnplus3*GN3 + (1 - fnplus3)*GP);
	}
	if (iS3 > -1) {
		Gs3 = GS3 * GP / (fsplus3*GS3 + (1 - fsplus3)*GP);
	}
	if (iT3 > -1) {
		Gt3 = GT3 * GP / (ftplus3*GT3 + (1 - ftplus3)*GP);
	}
	if (iB3 > -1) {
		Gb3 = GB3 * GP / (fbplus3*GB3 + (1 - fbplus3)*GP);
	}

	doublereal Ge4 = GP, Gw4 = GP, Gn4 = GP, Gs4 = GP, Gt4 = GP, Gb4 = GP;
	// �������� ������������ �������� �� ����� ��.
	if (iE4 > -1) {
		Ge4 = GE4 * GP / (feplus4*GE4 + (1 - feplus4)*GP); // ���������.
	}
	if (iW4 > -1) {
		Gw4 = GW4 * GP / (fwplus4*GW4 + (1 - fwplus4)*GP);
	}
	if (iN4 > -1) {
		Gn4 = GN4 * GP / (fnplus4*GN4 + (1 - fnplus4)*GP);
	}
	if (iS4 > -1) {
		Gs4 = GS4 * GP / (fsplus4*GS4 + (1 - fsplus4)*GP);
	}
	if (iT4 > -1) {
		Gt4 = GT4 * GP / (ftplus4*GT4 + (1 - ftplus4)*GP);
	}
	if (iB4 > -1) {
		Gb4 = GB4 * GP / (fbplus4*GB4 + (1 - fbplus4)*GP);
	}




	const doublereal ZeroDiffusion = 0.0;// 1.0e-30;
	// ������������ ������������ ������:
	doublereal De = ZeroDiffusion, Dw = ZeroDiffusion, Dn = ZeroDiffusion, Ds = ZeroDiffusion, Dt = ZeroDiffusion, Db = ZeroDiffusion; // �������������
	if (iE > -1) {
		if (bE) {
			// ��������� ����.
			De = Ge * border_neighbor[iE - maxelm].dS / dxe;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iE]]) {
				De = Ge * dy*dz / dxe;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iE, nvtx, pa, dx_loc, dy_loc, dz_loc);

				De = Ge * dy_loc*dz_loc / dxe;
			}
		}

	}
	if (iW > -1) {
		if (bW) {
			// ��������� ����
			Dw = Gw * border_neighbor[iW - maxelm].dS / dxw;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iW]]) {
				Dw = Gw * dy*dz / dxw;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iW, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Dw = Gw * dy_loc*dz_loc / dxw;
			}
		}

	}
	if (iN > -1) {
		if (bN) {
			// ��������� ����.
			Dn = Gn * border_neighbor[iN - maxelm].dS / dyn;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iN]]) {
				Dn = Gn * dx*dz / dyn;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iN, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Dn = Gn * dx_loc*dz_loc / dyn;
			}
		}
	}
	if (iS > -1) {
		if (bS) {
			// ��������� ����
			Ds = Gs * border_neighbor[iS - maxelm].dS / dys;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iS]]) {
				Ds = Gs * dx*dz / dys;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iS, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Ds = Gs * dx_loc*dz_loc / dys;
			}
		}
	}
	if (iT > -1) {
		if (bT) {
			// ��������� ����.
			Dt = Gt * border_neighbor[iT - maxelm].dS / dzt;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iT]]) {
				Dt = Gt * dx*dy / dzt;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iT, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Dt = Gt * dx_loc*dy_loc / dzt;
			}
		}


	}
	if (iB > -1) {
		if (bB) {
			// ��������� ����
			Db = Gb * border_neighbor[iB - maxelm].dS / dzb;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iB]]) {
				Db = Gb * dx*dy / dzb;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iB, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Db = Gb * dx_loc*dy_loc / dzb;
			}
		}


	}

	// ������������ ������������ ������:
	doublereal De2 = ZeroDiffusion, Dw2 = ZeroDiffusion, Dn2 = ZeroDiffusion, Ds2 = ZeroDiffusion, Dt2 = ZeroDiffusion, Db2 = ZeroDiffusion; // �������������
	if (iE2 > -1) {
		if (bE2) {
			// ��������� ����.
			De2 = Ge2 * border_neighbor[iE2 - maxelm].dS / dxe2;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iE2]]) {
				De2 = Ge2 * dy*dz / dxe2;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iE2, nvtx, pa, dx_loc, dy_loc, dz_loc);

				De2 = Ge2 * dy_loc*dz_loc / dxe2;
			}
		}

	}
	if (iW2 > -1) {
		if (bW2) {
			// ��������� ����
			Dw2 = Gw2 * border_neighbor[iW2 - maxelm].dS / dxw2;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iW2]]) {
				Dw2 = Gw2 * dy*dz / dxw2;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iW2, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Dw2 = Gw2 * dy_loc*dz_loc / dxw2;
			}
		}

	}
	if (iN2 > -1) {
		if (bN2) {
			// ��������� ����.
			Dn2 = Gn2 * border_neighbor[iN2 - maxelm].dS / dyn2;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iN2]]) {
				Dn2 = Gn2 * dx*dz / dyn2;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iN2, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Dn2 = Gn2 * dx_loc*dz_loc / dyn2;
			}
		}
	}
	if (iS2 > -1) {
		if (bS2) {
			// ��������� ����
			Ds2 = Gs2 * border_neighbor[iS2 - maxelm].dS / dys2;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iS2]]) {
				Ds2 = Gs2 * dx*dz / dys2;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iS2, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Ds2 = Gs2 * dx_loc*dz_loc / dys2;
			}
		}
	}
	if (iT2 > -1) {
		if (bT2) {
			// ��������� ����.
			Dt2 = Gt2 * border_neighbor[iT2 - maxelm].dS / dzt2;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iT2]]) {
				Dt2 = Gt2 * dx*dy / dzt2;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iT2, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Dt2 = Gt2 * dx_loc*dy_loc / dzt2;
			}
		}


	}
	if (iB2 > -1) {
		if (bB2) {
			// ��������� ����
			Db2 = Gb2 * border_neighbor[iB2 - maxelm].dS / dzb2;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iB2]]) {
				Db2 = Gb2 * dx*dy / dzb2;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iB2, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Db2 = Gb2 * dx_loc*dy_loc / dzb2;
			}
		}


	}

	// ������������ ������������ ������:
	doublereal De3 = ZeroDiffusion, Dw3 = ZeroDiffusion, Dn3 = ZeroDiffusion, Ds3 = ZeroDiffusion, Dt3 = ZeroDiffusion, Db3 = ZeroDiffusion; // �������������
	if (iE3 > -1) {
		if (bE3) {
			// ��������� ����.
			De3 = Ge3 * border_neighbor[iE3 - maxelm].dS / dxe3;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iE3]]) {
				De3 = Ge3 * dy*dz / dxe3;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iE3, nvtx, pa, dx_loc, dy_loc, dz_loc);

				De3 = Ge3 * dy_loc*dz_loc / dxe3;
			}
		}

	}
	if (iW3 > -1) {
		if (bW3) {
			// ��������� ����
			Dw3 = Gw3 * border_neighbor[iW3 - maxelm].dS / dxw3;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iW3]]) {
				Dw3 = Gw3 * dy*dz / dxw3;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iW3, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Dw3 = Gw3 * dy_loc*dz_loc / dxw3;
			}
		}

	}
	if (iN3 > -1) {
		if (bN3) {
			// ��������� ����.
			Dn3 = Gn3 * border_neighbor[iN3 - maxelm].dS / dyn3;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iN3]]) {
				Dn3 = Gn3 * dx*dz / dyn3;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iN3, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Dn3 = Gn3 * dx_loc*dz_loc / dyn3;
			}
		}
	}
	if (iS3 > -1) {
		if (bS3) {
			// ��������� ����
			Ds3 = Gs3 * border_neighbor[iS3 - maxelm].dS / dys3;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iS3]]) {
				Ds3 = Gs3 * dx*dz / dys3;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iS3, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Ds3 = Gs3 * dx_loc*dz_loc / dys3;
			}
		}
	}
	if (iT3 > -1) {
		if (bT3) {
			// ��������� ����.
			Dt3 = Gt3 * border_neighbor[iT3 - maxelm].dS / dzt3;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iT3]]) {
				Dt3 = Gt3 * dx*dy / dzt3;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iT3, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Dt3 = Gt3 * dx_loc*dy_loc / dzt3;
			}
		}


	}
	if (iB3 > -1) {
		if (bB3) {
			// ��������� ����
			Db3 = Gb3 * border_neighbor[iB3 - maxelm].dS / dzb3;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iB3]]) {
				Db3 = Gb3 * dx*dy / dzb3;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iB3, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Db3 = Gb3 * dx_loc*dy_loc / dzb3;
			}
		}


	}

	// ������������ ������������ ������:
	doublereal De4 = ZeroDiffusion, Dw4 = ZeroDiffusion, Dn4 = ZeroDiffusion, Ds4 = ZeroDiffusion, Dt4 = ZeroDiffusion, Db4 = ZeroDiffusion; // �������������
	if (iE4 > -1) {
		if (bE4) {
			// ��������� ����.
			De4 = Ge4 * border_neighbor[iE4 - maxelm].dS / dxe4;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iE4]]) {
				De4 = Ge4 * dy*dz / dxe4;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iE4, nvtx, pa, dx_loc, dy_loc, dz_loc);

				De4 = Ge4 * dy_loc*dz_loc / dxe4;
			}
		}

	}
	if (iW4 > -1) {
		if (bW4) {
			// ��������� ����
			Dw4 = Gw4 * border_neighbor[iW4 - maxelm].dS / dxw4;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iW4]]) {
				Dw4 = Gw4 * dy*dz / dxw4;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iW4, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Dw4 = Gw4 * dy_loc*dz_loc / dxw4;
			}
		}

	}
	if (iN4 > -1) {
		if (bN4) {
			// ��������� ����.
			Dn4 = Gn4 * border_neighbor[iN4 - maxelm].dS / dyn4;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iN4]]) {
				Dn4 = Gn4 * dx*dz / dyn4;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iN4, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Dn4 = Gn4 * dx_loc*dz_loc / dyn4;
			}
		}
	}
	if (iS4 > -1) {
		if (bS4) {
			// ��������� ����
			Ds4 = Gs4 * border_neighbor[iS4 - maxelm].dS / dys4;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iS4]]) {
				Ds4 = Gs4 * dx*dz / dys4;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iS4, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Ds4 = Gs4 * dx_loc*dz_loc / dys4;
			}
		}
	}
	if (iT4 > -1) {
		if (bT4) {
			// ��������� ����.
			Dt4 = Gt4 * border_neighbor[iT4 - maxelm].dS / dzt4;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iT4]]) {
				Dt4 = Gt4 * dx*dy / dzt4;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iT4, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Dt4 = Gt4 * dx_loc*dy_loc / dzt4;
			}
		}


	}
	if (iB4 > -1) {
		if (bB4) {
			// ��������� ����
			Db4 = Gb4 * border_neighbor[iB4 - maxelm].dS / dzb4;
		}
		else {
			if (ilevel_alice[ptr[iP]] >= ilevel_alice[ptr[iB4]]) {
				Db4 = Gb4 * dx*dy / dzb4;
			}
			else {
				// ���������� �������� ��������� ������������ ������:
				doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������ ������
				volume3D(iB4, nvtx, pa, dx_loc, dy_loc, dz_loc);

				Db4 = Gb4 * dx_loc*dy_loc / dzb4;
			}
		}
	}

	// ������������� ��������� ������� ���������.
	De = fmax(De, 0.0);
	Dw = fmax(Dw, 0.0);
	Dn = fmax(Dn, 0.0);
	Ds = fmax(Ds, 0.0);
	Dt = fmax(Dt, 0.0);
	Db = fmax(Db, 0.0);

	De2 = fmax(De2, 0.0);
	Dw2 = fmax(Dw2, 0.0);
	Dn2 = fmax(Dn2, 0.0);
	Ds2 = fmax(Ds2, 0.0);
	Dt2 = fmax(Dt2, 0.0);
	Db2 = fmax(Db2, 0.0);

	De3 = fmax(De3, 0.0);
	Dw3 = fmax(Dw3, 0.0);
	Dn3 = fmax(Dn3, 0.0);
	Ds3 = fmax(Ds3, 0.0);
	Dt3 = fmax(Dt3, 0.0);
	Db3 = fmax(Db3, 0.0);

	De4 = fmax(De4, 0.0);
	Dw4 = fmax(Dw4, 0.0);
	Dn4 = fmax(Dn4, 0.0);
	Ds4 = fmax(Ds4, 0.0);
	Dt4 = fmax(Dt4, 0.0);
	Db4 = fmax(Db4, 0.0);

	// ����� �����:
	doublereal Pe = 0.0, Pw = 0.0, Pn = 0.0, Ps = 0.0, Pt = 0.0, Pb = 0.0;
	if (iE > -1) {
		Pe = (Fe) / De;
	}
	if (iW > -1) {
		Pw = -(Fw) / Dw;
	}
	if (iN > -1) {
		Pn = (Fn) / Dn;
	}
	if (iS > -1) {
		Ps = -(Fs) / Ds;
	}
	if (iT > -1) {
		Pt = (Ft) / Dt;
	}
	if (iB > -1) {
		Pb = -(Fb) / Db;
	}

	// ����� �����:
	doublereal Pe2 = 0.0, Pw2 = 0.0, Pn2 = 0.0, Ps2 = 0.0, Pt2 = 0.0, Pb2 = 0.0;
	if (iE2 > -1) {
		Pe2 = (Fe2) / De2;
	}
	if (iW2 > -1) {
		Pw2 = -(Fw2) / Dw2;
	}
	if (iN2 > -1) {
		Pn2 = (Fn2) / Dn2;
	}
	if (iS2 > -1) {
		Ps2 = -(Fs2) / Ds2;
	}
	if (iT2 > -1) {
		Pt2 = (Ft2) / Dt2;
	}
	if (iB2 > -1) {
		Pb2 = -(Fb2) / Db2;
	}

	// ����� �����:
	doublereal Pe3 = 0.0, Pw3 = 0.0, Pn3 = 0.0, Ps3 = 0.0, Pt3 = 0.0, Pb3 = 0.0;
	if (iE3 > -1) {
		Pe3 = (Fe3) / De3;
	}
	if (iW3 > -1) {
		Pw3 = -(Fw3) / Dw3;
	}
	if (iN3 > -1) {
		Pn3 = (Fn3) / Dn3;
	}
	if (iS3 > -1) {
		Ps3 = -(Fs3) / Ds3;
	}
	if (iT3 > -1) {
		Pt3 = (Ft3) / Dt3;
	}
	if (iB3 > -1) {
		Pb3 = -(Fb3) / Db3;
	}

	// ����� �����:
	doublereal Pe4 = 0.0, Pw4 = 0.0, Pn4 = 0.0, Ps4 = 0.0, Pt4 = 0.0, Pb4 = 0.0;
	if (iE4 > -1) {
		Pe4 = (Fe4) / De4;
	}
	if (iW4 > -1) {
		Pw4 = -(Fw4) / Dw4;
	}
	if (iN4 > -1) {
		Pn4 = (Fn4) / Dn4;
	}
	if (iS4 > -1) {
		Ps4 = -(Fs4) / Ds4;
	}
	if (iT4 > -1) {
		Pt4 = (Ft4) / Dt4;
	}
	if (iB4 > -1) {
		Pb4 = -(Fb4) / Db4;
	}


	// ������� � ������ ����� ��� ������������� ����� �������� QUICK
	// � ���� ������������� ������ ���������� ���������.
	// addition to the right side QUICK Leonard.
	doublereal attrs = 0.0;

	if (b_on_adaptive_local_refinement_mesh) {

		// ���������������� ���������.
		sl[NUSHA_SL][iP].ae = 0.0;
		sl[NUSHA_SL][iP].aw = 0.0;
		sl[NUSHA_SL][iP].an = 0.0;
		sl[NUSHA_SL][iP].as = 0.0;
		sl[NUSHA_SL][iP].at = 0.0;
		sl[NUSHA_SL][iP].ab = 0.0;

		sl[NUSHA_SL][iP].ae2 = 0.0;
		sl[NUSHA_SL][iP].aw2 = 0.0;
		sl[NUSHA_SL][iP].an2 = 0.0;
		sl[NUSHA_SL][iP].as2 = 0.0;
		sl[NUSHA_SL][iP].at2 = 0.0;
		sl[NUSHA_SL][iP].ab2 = 0.0;

		sl[NUSHA_SL][iP].ae3 = 0.0;
		sl[NUSHA_SL][iP].aw3 = 0.0;
		sl[NUSHA_SL][iP].an3 = 0.0;
		sl[NUSHA_SL][iP].as3 = 0.0;
		sl[NUSHA_SL][iP].at3 = 0.0;
		sl[NUSHA_SL][iP].ab3 = 0.0;

		sl[NUSHA_SL][iP].ae4 = 0.0;
		sl[NUSHA_SL][iP].aw4 = 0.0;
		sl[NUSHA_SL][iP].an4 = 0.0;
		sl[NUSHA_SL][iP].as4 = 0.0;
		sl[NUSHA_SL][iP].at4 = 0.0;
		sl[NUSHA_SL][iP].ab4 = 0.0;

	}

	if (ishconvection < distsheme) {

		if (1) {
			// ������� ��� ����������� ������ � ��������������� � ���������� 7.05.2017. 
			if (b_on_adaptive_local_refinement_mesh) {
				// ���������� ������������� ����������� �������:
				sl[NUSHA_SL][iP].ae = De * ApproxConvective(fabs(Pe), ishconvection) + fmax(-(Fe), 0);
				sl[NUSHA_SL][iP].aw = Dw * ApproxConvective(fabs(Pw), ishconvection) + fmax(Fw, 0);
				sl[NUSHA_SL][iP].an = Dn * ApproxConvective(fabs(Pn), ishconvection) + fmax(-(Fn), 0);
				sl[NUSHA_SL][iP].as = Ds * ApproxConvective(fabs(Ps), ishconvection) + fmax(Fs, 0);
				sl[NUSHA_SL][iP].at = Dt * ApproxConvective(fabs(Pt), ishconvection) + fmax(-(Ft), 0);
				sl[NUSHA_SL][iP].ab = Db * ApproxConvective(fabs(Pb), ishconvection) + fmax(Fb, 0);

				// ���������� ������������� ����������� �������:
				sl[NUSHA_SL][iP].ae2 = De2 * ApproxConvective(fabs(Pe2), ishconvection) + fmax(-(Fe2), 0);
				sl[NUSHA_SL][iP].aw2 = Dw2 * ApproxConvective(fabs(Pw2), ishconvection) + fmax(Fw2, 0);
				sl[NUSHA_SL][iP].an2 = Dn2 * ApproxConvective(fabs(Pn2), ishconvection) + fmax(-(Fn2), 0);
				sl[NUSHA_SL][iP].as2 = Ds2 * ApproxConvective(fabs(Ps2), ishconvection) + fmax(Fs2, 0);
				sl[NUSHA_SL][iP].at2 = Dt2 * ApproxConvective(fabs(Pt2), ishconvection) + fmax(-(Ft2), 0);
				sl[NUSHA_SL][iP].ab2 = Db2 * ApproxConvective(fabs(Pb2), ishconvection) + fmax(Fb2, 0);

				// ���������� ������������� ����������� �������:
				sl[NUSHA_SL][iP].ae3 = De3 * ApproxConvective(fabs(Pe3), ishconvection) + fmax(-(Fe3), 0);
				sl[NUSHA_SL][iP].aw3 = Dw3 * ApproxConvective(fabs(Pw3), ishconvection) + fmax(Fw3, 0);
				sl[NUSHA_SL][iP].an3 = Dn3 * ApproxConvective(fabs(Pn3), ishconvection) + fmax(-(Fn3), 0);
				sl[NUSHA_SL][iP].as3 = Ds3 * ApproxConvective(fabs(Ps3), ishconvection) + fmax(Fs3, 0);
				sl[NUSHA_SL][iP].at3 = Dt3 * ApproxConvective(fabs(Pt3), ishconvection) + fmax(-(Ft3), 0);
				sl[NUSHA_SL][iP].ab3 = Db3 * ApproxConvective(fabs(Pb3), ishconvection) + fmax(Fb3, 0);

				// ���������� ������������� ����������� �������:
				sl[NUSHA_SL][iP].ae4 = De4 * ApproxConvective(fabs(Pe4), ishconvection) + fmax(-(Fe4), 0);
				sl[NUSHA_SL][iP].aw4 = Dw4 * ApproxConvective(fabs(Pw4), ishconvection) + fmax(Fw4, 0);
				sl[NUSHA_SL][iP].an4 = Dn4 * ApproxConvective(fabs(Pn4), ishconvection) + fmax(-(Fn4), 0);
				sl[NUSHA_SL][iP].as4 = Ds4 * ApproxConvective(fabs(Ps4), ishconvection) + fmax(Fs4, 0);
				sl[NUSHA_SL][iP].at4 = Dt4 * ApproxConvective(fabs(Pt4), ishconvection) + fmax(-(Ft4), 0);
				sl[NUSHA_SL][iP].ab4 = Db4 * ApproxConvective(fabs(Pb4), ishconvection) + fmax(Fb4, 0);

			}
			else {
				// TODO 25 07 2015
				// ���������� ������������� ����������� �������:
				sl[NUSHA_SL][iP].ae = De * ApproxConvective(fabs(Pe), ishconvection) + fmax(-(Fe), 0);
				sl[NUSHA_SL][iP].aw = Dw * ApproxConvective(fabs(Pw), ishconvection) + fmax(Fw, 0);
				sl[NUSHA_SL][iP].an = Dn * ApproxConvective(fabs(Pn), ishconvection) + fmax(-(Fn), 0);
				sl[NUSHA_SL][iP].as = Ds * ApproxConvective(fabs(Ps), ishconvection) + fmax(Fs, 0);
				sl[NUSHA_SL][iP].at = Dt * ApproxConvective(fabs(Pt), ishconvection) + fmax(-(Ft), 0);
				sl[NUSHA_SL][iP].ab = Db * ApproxConvective(fabs(Pb), ishconvection) + fmax(Fb, 0);
				//sl[NUSHA_SL][iP].ap=sl[NUSHA_SL][iP].ae+sl[NUSHA_SL][iP].aw+sl[NUSHA_SL][iP].an+sl[NUSHA_SL][iP].as+sl[NUSHA_SL][iP].at+sl[NUSHA_SL][iP].ab;
			}
		}
		else
		{
			// �������� �� ������ ����������������������� 25 ���� 2015.
			if (!bE) {
				sl[NUSHA_SL][iP].ae = De * ApproxConvective(fabs(Pe), ishconvection) + fmax(-(Fe), 0);
			}
			else {
				integer inumber = iE - maxelm;
				if (border_neighbor[inumber].MCB == (ls + lw)) {
					// ������� �� ���������: ������ ������.
					// �������� ������� ���� �� �������, ��� �� ����� ������� ������.
					sl[NUSHA_SL][iP].ae = De * ApproxConvective(fabs(Pe), ishconvection) + fabs(Fe);
				}
				else {
					sl[NUSHA_SL][iP].ae = De * ApproxConvective(fabs(Pe), ishconvection) + fmax(-(Fe), 0);
				}
			}
			if (!bW) {
				sl[NUSHA_SL][iP].aw = Dw * ApproxConvective(fabs(Pw), ishconvection) + fmax(Fw, 0);
			}
			else {
				integer inumber = iW - maxelm;
				if (border_neighbor[inumber].MCB == (ls + lw)) {
					// ������� �� ���������: ������ ������.
					// �������� ������� ���� �� �������, ��� �� ����� ������� ������.
					sl[NUSHA_SL][iP].aw = Dw * ApproxConvective(fabs(Pw), ishconvection) + fabs(Fw);
				}
				else {
					sl[NUSHA_SL][iP].aw = Dw * ApproxConvective(fabs(Pw), ishconvection) + fmax(Fw, 0);
				}
			}
			if (!bN) {
				sl[NUSHA_SL][iP].an = Dn * ApproxConvective(fabs(Pn), ishconvection) + fmax(-(Fn), 0);
			}
			else {
				integer inumber = iN - maxelm;
				if (border_neighbor[inumber].MCB == (ls + lw)) {
					// ������� �� ���������: ������ ������.
					// �������� ������� ���� �� �������, ��� �� ����� ������� ������.
					sl[NUSHA_SL][iP].an = Dn * ApproxConvective(fabs(Pn), ishconvection) + fabs(Fn);
				}
				else {
					sl[NUSHA_SL][iP].an = Dn * ApproxConvective(fabs(Pn), ishconvection) + fmax(-(Fn), 0);
				}
			}
			if (!bS) {
				sl[NUSHA_SL][iP].as = Ds * ApproxConvective(fabs(Ps), ishconvection) + fmax(Fs, 0);
			}
			else {
				integer inumber = iS - maxelm;
				if (border_neighbor[inumber].MCB == (ls + lw)) {
					// ������� �� ���������: ������ ������.
					// �������� ������� ���� �� �������, ��� �� ����� ������� ������.
					sl[NUSHA_SL][iP].as = Ds * ApproxConvective(fabs(Ps), ishconvection) + fabs(Fs);
				}
				else {
					sl[NUSHA_SL][iP].as = Ds * ApproxConvective(fabs(Ps), ishconvection) + fmax(Fs, 0);
				}
			}
			if (!bT) {
				sl[NUSHA_SL][iP].at = Dt * ApproxConvective(fabs(Pt), ishconvection) + fmax(-(Ft), 0);
			}
			else {
				integer inumber = iT - maxelm;
				if (border_neighbor[inumber].MCB == (ls + lw)) {
					// ������� �� ���������: ������ ������.
					// �������� ������� ���� �� �������, ��� �� ����� ������� ������.
					sl[NUSHA_SL][iP].at = Dt * ApproxConvective(fabs(Pt), ishconvection) + fabs(Ft);
				}
				else {
					sl[NUSHA_SL][iP].at = Dt * ApproxConvective(fabs(Pt), ishconvection) + fmax(-(Ft), 0);
				}
			}
			if (!bB) {
				sl[NUSHA_SL][iP].ab = Db * ApproxConvective(fabs(Pb), ishconvection) + fmax(Fb, 0);
			}
			else
			{
				integer inumber = iB - maxelm;
				if (border_neighbor[inumber].MCB == (ls + lw)) {
					// ������� �� ���������: ������ ������.
					// �������� ������� ���� �� �������, ��� �� ����� ������� ������.
					sl[NUSHA_SL][iP].ab = Db * ApproxConvective(fabs(Pb), ishconvection) + fabs(Fb);
				}
				else {
					sl[NUSHA_SL][iP].ab = Db * ApproxConvective(fabs(Pb), ishconvection) + fmax(Fb, 0);
				}
			}

		}

		// ������ ��� ����������� ������ � ��������� � ����������. 7.05.2017.
		//sl[NUSHA_SL][iP].ap=sl[NUSHA_SL][iP].ae+sl[NUSHA_SL][iP].aw+sl[NUSHA_SL][iP].an+sl[NUSHA_SL][iP].as+sl[NUSHA_SL][iP].at+sl[NUSHA_SL][iP].ab;
		// ��� ���������:
		// ����� ������������� !!! (���������� �� ����).
		if (b_on_adaptive_local_refinement_mesh) {
			// ���� 

			sl[NUSHA_SL][iP].ap = De * ApproxConvective(fabs(Pe), ishconvection) + fmax(+(Fe), 0);
			sl[NUSHA_SL][iP].ap += Dw * ApproxConvective(fabs(Pw), ishconvection) + fmax(-(Fw), 0);
			sl[NUSHA_SL][iP].ap += Dn * ApproxConvective(fabs(Pn), ishconvection) + fmax(+(Fn), 0);
			sl[NUSHA_SL][iP].ap += Ds * ApproxConvective(fabs(Ps), ishconvection) + fmax(-(Fs), 0);
			sl[NUSHA_SL][iP].ap += Dt * ApproxConvective(fabs(Pt), ishconvection) + fmax(+(Ft), 0);
			sl[NUSHA_SL][iP].ap += Db * ApproxConvective(fabs(Pb), ishconvection) + fmax(-(Fb), 0);

			sl[NUSHA_SL][iP].ap += De2 * ApproxConvective(fabs(Pe2), ishconvection) + fmax(+(Fe2), 0);
			sl[NUSHA_SL][iP].ap += Dw2 * ApproxConvective(fabs(Pw2), ishconvection) + fmax(-(Fw2), 0);
			sl[NUSHA_SL][iP].ap += Dn2 * ApproxConvective(fabs(Pn2), ishconvection) + fmax(+(Fn2), 0);
			sl[NUSHA_SL][iP].ap += Ds2 * ApproxConvective(fabs(Ps2), ishconvection) + fmax(-(Fs2), 0);
			sl[NUSHA_SL][iP].ap += Dt2 * ApproxConvective(fabs(Pt2), ishconvection) + fmax(+(Ft2), 0);
			sl[NUSHA_SL][iP].ap += Db2 * ApproxConvective(fabs(Pb2), ishconvection) + fmax(-(Fb2), 0);

			sl[NUSHA_SL][iP].ap += De3 * ApproxConvective(fabs(Pe3), ishconvection) + fmax(+(Fe3), 0);
			sl[NUSHA_SL][iP].ap += Dw3 * ApproxConvective(fabs(Pw3), ishconvection) + fmax(-(Fw3), 0);
			sl[NUSHA_SL][iP].ap += Dn3 * ApproxConvective(fabs(Pn3), ishconvection) + fmax(+(Fn3), 0);
			sl[NUSHA_SL][iP].ap += Ds3 * ApproxConvective(fabs(Ps3), ishconvection) + fmax(-(Fs3), 0);
			sl[NUSHA_SL][iP].ap += Dt3 * ApproxConvective(fabs(Pt3), ishconvection) + fmax(+(Ft3), 0);
			sl[NUSHA_SL][iP].ap += Db3 * ApproxConvective(fabs(Pb3), ishconvection) + fmax(-(Fb3), 0);

			sl[NUSHA_SL][iP].ap += De4 * ApproxConvective(fabs(Pe4), ishconvection) + fmax(+(Fe4), 0);
			sl[NUSHA_SL][iP].ap += Dw4 * ApproxConvective(fabs(Pw4), ishconvection) + fmax(-(Fw4), 0);
			sl[NUSHA_SL][iP].ap += Dn4 * ApproxConvective(fabs(Pn4), ishconvection) + fmax(+(Fn4), 0);
			sl[NUSHA_SL][iP].ap += Ds4 * ApproxConvective(fabs(Ps4), ishconvection) + fmax(-(Fs4), 0);
			sl[NUSHA_SL][iP].ap += Dt4 * ApproxConvective(fabs(Pt4), ishconvection) + fmax(+(Ft4), 0);
			sl[NUSHA_SL][iP].ap += Db4 * ApproxConvective(fabs(Pb4), ishconvection) + fmax(-(Fb4), 0);
			/*
			sl[NUSHA_SL][iP].ap = sl[NUSHA_SL][iP].ae +  sl[NUSHA_SL][iP].aw + sl[NUSHA_SL][iP].an + sl[NUSHA_SL][iP].as + sl[NUSHA_SL][iP].at + sl[NUSHA_SL][iP].ab;
			if (b_on_adaptive_local_refinement_mesh) {
				sl[NUSHA_SL][iP].ap += sl[NUSHA_SL][iP].ae2 + sl[NUSHA_SL][iP].aw2 + sl[NUSHA_SL][iP].an2 + sl[NUSHA_SL][iP].as2 + sl[NUSHA_SL][iP].at2 + sl[NUSHA_SL][iP].ab2;
				sl[NUSHA_SL][iP].ap += sl[NUSHA_SL][iP].ae3 + sl[NUSHA_SL][iP].aw3 + sl[NUSHA_SL][iP].an3 + sl[NUSHA_SL][iP].as3 + sl[NUSHA_SL][iP].at3 + sl[NUSHA_SL][iP].ab3;
				sl[NUSHA_SL][iP].ap += sl[NUSHA_SL][iP].ae4 + sl[NUSHA_SL][iP].aw4 + sl[NUSHA_SL][iP].an4 + sl[NUSHA_SL][iP].as4 + sl[NUSHA_SL][iP].at4 + sl[NUSHA_SL][iP].ab4;
			}
			*/
		}
		else {
			sl[NUSHA_SL][iP].ap = De * ApproxConvective(fabs(Pe), ishconvection) + fmax(+(Fe), 0);
			sl[NUSHA_SL][iP].ap += Dw * ApproxConvective(fabs(Pw), ishconvection) + fmax(-(Fw), 0);
			sl[NUSHA_SL][iP].ap += Dn * ApproxConvective(fabs(Pn), ishconvection) + fmax(+(Fn), 0);
			sl[NUSHA_SL][iP].ap += Ds * ApproxConvective(fabs(Ps), ishconvection) + fmax(-(Fs), 0);
			sl[NUSHA_SL][iP].ap += Dt * ApproxConvective(fabs(Pt), ishconvection) + fmax(+(Ft), 0);
			sl[NUSHA_SL][iP].ap += Db * ApproxConvective(fabs(Pb), ishconvection) + fmax(-(Fb), 0);
		}

		// 13 ������� 2016
		// ��� ��������. ��� ����� �� �������� � ����������. �� ������� ��� ��������� ������������ ������������, ��
		// ������������� ���������� ���� � �������, �� �� ��������������� ��� ����� ���������� ������������ ����������� ������ ������.
		//sl[NUSHA_SL][iP].ap = fabs(sl[NUSHA_SL][iP].ae) + fabs(sl[NUSHA_SL][iP].aw) + fabs(sl[NUSHA_SL][iP].an) + fabs(sl[NUSHA_SL][iP].as) + fabs(sl[NUSHA_SL][iP].at) + fabs(sl[NUSHA_SL][iP].ab);



		if (sl[NUSHA_SL][iP].ap < 1.0e-40) {
			printf("Zero diagonal coefficient in internal volume in my_elmatr_quad_SpallartAllmares3D.\n");
#if doubleintprecision == 1
			printf("ap=%e iP=%lld\n", sl[NUSHA_SL][iP].ap, iP);
#else
			printf("ap=%e iP=%d\n", sl[NUSHA_SL][iP].ap, iP);
#endif
			if (b_on_adaptive_local_refinement_mesh) {
				printf("ae=%e aw=%e an=%e as=%e at=%e ab=%e\n", sl[NUSHA_SL][iP].ae, sl[NUSHA_SL][iP].aw, sl[NUSHA_SL][iP].an, sl[NUSHA_SL][iP].as, sl[NUSHA_SL][iP].at, sl[NUSHA_SL][iP].ab);
				printf("ae2=%e aw2=%e an2=%e as2=%e at2=%e ab2=%e\n", sl[NUSHA_SL][iP].ae2, sl[NUSHA_SL][iP].aw2, sl[NUSHA_SL][iP].an2, sl[NUSHA_SL][iP].as2, sl[NUSHA_SL][iP].at2, sl[NUSHA_SL][iP].ab2);
				printf("ae3=%e aw3=%e an3=%e as3=%e at3=%e ab3=%e\n", sl[NUSHA_SL][iP].ae3, sl[NUSHA_SL][iP].aw3, sl[NUSHA_SL][iP].an3, sl[NUSHA_SL][iP].as3, sl[NUSHA_SL][iP].at3, sl[NUSHA_SL][iP].ab3);
				printf("ae4=%e aw4=%e an4=%e as4=%e at4=%e ab4=%e\n", sl[NUSHA_SL][iP].ae4, sl[NUSHA_SL][iP].aw4, sl[NUSHA_SL][iP].an4, sl[NUSHA_SL][iP].as4, sl[NUSHA_SL][iP].at4, sl[NUSHA_SL][iP].ab4);
			}
			else {
				printf("ae=%e aw=%e an=%e as=%e at=%e ab=%e\n", sl[NUSHA_SL][iP].ae, sl[NUSHA_SL][iP].aw, sl[NUSHA_SL][iP].an, sl[NUSHA_SL][iP].as, sl[NUSHA_SL][iP].at, sl[NUSHA_SL][iP].ab);
			}
			system("pause");

			// ��� ������ ������ �� ������� �� ��������
			// �.�. �� ������ ����� ����� ������� ������������ �������.
			// ������� ����� �������� �������� ����� ������ ����� ��������� �����������, ��� ����� �����������.
			// �� �������� �� ��-���� ��������� �������� ����� ���������.
			sl[NUSHA_SL][iP].ae = De * ApproxConvective(fabs(Pe), BULG);//+fmax(-(Fe),0); 
			sl[NUSHA_SL][iP].aw = Dw * ApproxConvective(fabs(Pw), BULG);//+fmax(Fw,0); 
			sl[NUSHA_SL][iP].an = Dn * ApproxConvective(fabs(Pn), BULG);//+fmax(-(Fn),0); 
			sl[NUSHA_SL][iP].as = Ds * ApproxConvective(fabs(Ps), BULG);//+fmax(Fs,0); 
			sl[NUSHA_SL][iP].at = Dt * ApproxConvective(fabs(Pt), BULG);//+fmax(-(Ft),0); 
			sl[NUSHA_SL][iP].ab = Db * ApproxConvective(fabs(Pb), BULG);//+fmax(Fb,0); 
			if (b_on_adaptive_local_refinement_mesh) {
				sl[NUSHA_SL][iP].ae2 = De2 * ApproxConvective(fabs(Pe2), BULG);//+fmax(-(Fe2),0); 
				sl[NUSHA_SL][iP].aw2 = Dw2 * ApproxConvective(fabs(Pw2), BULG);//+fmax(Fw2,0); 
				sl[NUSHA_SL][iP].an2 = Dn2 * ApproxConvective(fabs(Pn2), BULG);//+fmax(-(Fn2),0); 
				sl[NUSHA_SL][iP].as2 = Ds2 * ApproxConvective(fabs(Ps2), BULG);//+fmax(Fs2,0); 
				sl[NUSHA_SL][iP].at2 = Dt2 * ApproxConvective(fabs(Pt2), BULG);//+fmax(-(Ft2),0); 
				sl[NUSHA_SL][iP].ab2 = Db2 * ApproxConvective(fabs(Pb2), BULG);//+fmax(Fb2,0); 

				sl[NUSHA_SL][iP].ae3 = De3 * ApproxConvective(fabs(Pe3), BULG);//+fmax(-(Fe3),0); 
				sl[NUSHA_SL][iP].aw3 = Dw3 * ApproxConvective(fabs(Pw3), BULG);//+fmax(Fw3,0); 
				sl[NUSHA_SL][iP].an3 = Dn3 * ApproxConvective(fabs(Pn3), BULG);//+fmax(-(Fn3),0); 
				sl[NUSHA_SL][iP].as3 = Ds3 * ApproxConvective(fabs(Ps3), BULG);//+fmax(Fs3,0); 
				sl[NUSHA_SL][iP].at3 = Dt3 * ApproxConvective(fabs(Pt3), BULG);//+fmax(-(Ft3),0); 
				sl[NUSHA_SL][iP].ab3 = Db3 * ApproxConvective(fabs(Pb3), BULG);//+fmax(Fb3,0); 

				sl[NUSHA_SL][iP].ae4 = De4 * ApproxConvective(fabs(Pe4), BULG);//+fmax(-(Fe4),0); 
				sl[NUSHA_SL][iP].aw4 = Dw4 * ApproxConvective(fabs(Pw4), BULG);//+fmax(Fw4,0); 
				sl[NUSHA_SL][iP].an4 = Dn4 * ApproxConvective(fabs(Pn4), BULG);//+fmax(-(Fn4),0); 
				sl[NUSHA_SL][iP].as4 = Ds4 * ApproxConvective(fabs(Ps4), BULG);//+fmax(Fs4,0); 
				sl[NUSHA_SL][iP].at4 = Dt4 * ApproxConvective(fabs(Pt4), BULG);//+fmax(-(Ft4),0); 
				sl[NUSHA_SL][iP].ab4 = Db4 * ApproxConvective(fabs(Pb4), BULG);//+fmax(Fb4,0); 
			}
			sl[NUSHA_SL][iP].ap = sl[NUSHA_SL][iP].ae + sl[NUSHA_SL][iP].aw + sl[NUSHA_SL][iP].an + sl[NUSHA_SL][iP].as + sl[NUSHA_SL][iP].at + sl[NUSHA_SL][iP].ab;
			if (b_on_adaptive_local_refinement_mesh) {
				sl[NUSHA_SL][iP].ap += sl[NUSHA_SL][iP].ae2 + sl[NUSHA_SL][iP].aw2 + sl[NUSHA_SL][iP].an2 + sl[NUSHA_SL][iP].as2 + sl[NUSHA_SL][iP].at2 + sl[NUSHA_SL][iP].ab2;
				sl[NUSHA_SL][iP].ap += sl[NUSHA_SL][iP].ae3 + sl[NUSHA_SL][iP].aw3 + sl[NUSHA_SL][iP].an3 + sl[NUSHA_SL][iP].as3 + sl[NUSHA_SL][iP].at3 + sl[NUSHA_SL][iP].ab3;
				sl[NUSHA_SL][iP].ap += sl[NUSHA_SL][iP].ae4 + sl[NUSHA_SL][iP].aw4 + sl[NUSHA_SL][iP].an4 + sl[NUSHA_SL][iP].as4 + sl[NUSHA_SL][iP].at4 + sl[NUSHA_SL][iP].ab4;
			}
		}


		// ������ ��� ����������� ������ � ��������� � ����������. 7.05.2017.
		//sumanb=sl[NUSHA_SL][iP].ae+sl[NUSHA_SL][iP].aw+sl[NUSHA_SL][iP].an+sl[NUSHA_SL][iP].as+sl[NUSHA_SL][iP].at+sl[NUSHA_SL][iP].ab;
		// ��� ���������:
		// ����� ������������� !!! (���������� �� ����).
		/*
		sumanb = De * ApproxConvective(fabs(Pe), ishconvection) + fmax(+(Fe), 0);
		sumanb += Dw * ApproxConvective(fabs(Pw), ishconvection) + fmax(-(Fw), 0);
		sumanb += Dn * ApproxConvective(fabs(Pn), ishconvection) + fmax(+(Fn), 0);
		sumanb += Ds * ApproxConvective(fabs(Ps), ishconvection) + fmax(-(Fs), 0);
		sumanb += Dt * ApproxConvective(fabs(Pt), ishconvection) + fmax(+(Ft), 0);
		sumanb += Db * ApproxConvective(fabs(Pb), ishconvection) + fmax(-(Fb), 0);
		if (b_on_adaptive_local_refinement_mesh) {
			sumanb += De2 * ApproxConvective(fabs(Pe2), ishconvection) + fmax(+(Fe2), 0);
			sumanb += Dw2 * ApproxConvective(fabs(Pw2), ishconvection) + fmax(-(Fw2), 0);
			sumanb += Dn2 * ApproxConvective(fabs(Pn2), ishconvection) + fmax(+(Fn2), 0);
			sumanb += Ds2 * ApproxConvective(fabs(Ps2), ishconvection) + fmax(-(Fs2), 0);
			sumanb += Dt2 * ApproxConvective(fabs(Pt2), ishconvection) + fmax(+(Ft2), 0);
			sumanb += Db2 * ApproxConvective(fabs(Pb2), ishconvection) + fmax(-(Fb2), 0);

			sumanb += De3 * ApproxConvective(fabs(Pe3), ishconvection) + fmax(+(Fe3), 0);
			sumanb += Dw3 * ApproxConvective(fabs(Pw3), ishconvection) + fmax(-(Fw3), 0);
			sumanb += Dn3 * ApproxConvective(fabs(Pn3), ishconvection) + fmax(+(Fn3), 0);
			sumanb += Ds3 * ApproxConvective(fabs(Ps3), ishconvection) + fmax(-(Fs3), 0);
			sumanb += Dt3 * ApproxConvective(fabs(Pt3), ishconvection) + fmax(+(Ft3), 0);
			sumanb += Db3 * ApproxConvective(fabs(Pb3), ishconvection) + fmax(-(Fb3), 0);

			sumanb += De4 * ApproxConvective(fabs(Pe4), ishconvection) + fmax(+(Fe4), 0);
			sumanb += Dw4 * ApproxConvective(fabs(Pw4), ishconvection) + fmax(-(Fw4), 0);
			sumanb += Dn4 * ApproxConvective(fabs(Pn4), ishconvection) + fmax(+(Fn4), 0);
			sumanb += Ds4 * ApproxConvective(fabs(Ps4), ishconvection) + fmax(-(Fs4), 0);
			sumanb += Dt4 * ApproxConvective(fabs(Pt4), ishconvection) + fmax(+(Ft4), 0);
			sumanb += Db4 * ApproxConvective(fabs(Pb4), ishconvection) + fmax(-(Fb4), 0);
		}
		*/
		//13 ������� 2016.
		//sumanb = fabs(sl[NUSHA_SL][iP].ae) + fabs(sl[NUSHA_SL][iP].aw) + fabs(sl[NUSHA_SL][iP].an) + fabs(sl[NUSHA_SL][iP].as) + fabs(sl[NUSHA_SL][iP].at) + fabs(sl[NUSHA_SL][iP].ab);
		/*sumanb = sl[NUSHA_SL][iP].ae + sl[NUSHA_SL][iP].aw + sl[NUSHA_SL][iP].an + sl[NUSHA_SL][iP].as + sl[NUSHA_SL][iP].at + sl[NUSHA_SL][iP].ab;
		if (b_on_adaptive_local_refinement_mesh) {
			sumanb += sl[NUSHA_SL][iP].ae2 + sl[NUSHA_SL][iP].aw2 + sl[NUSHA_SL][iP].an2 + sl[NUSHA_SL][iP].as2 + sl[NUSHA_SL][iP].at2 + sl[NUSHA_SL][iP].ab2;
			sumanb += sl[NUSHA_SL][iP].ae3 + sl[NUSHA_SL][iP].aw3 + sl[NUSHA_SL][iP].an3 + sl[NUSHA_SL][iP].as3 + sl[NUSHA_SL][iP].at3 + sl[NUSHA_SL][iP].ab3;
			sumanb += sl[NUSHA_SL][iP].ae4 + sl[NUSHA_SL][iP].aw4 + sl[NUSHA_SL][iP].an4 + sl[NUSHA_SL][iP].as4 + sl[NUSHA_SL][iP].at4 + sl[NUSHA_SL][iP].ab4;
		}*/

	}
	else if (ishconvection < QUICK)
	{
		// ���������� ������������� ����������� �������:
		sl[NUSHA_SL][iP].ae = -(Fe)*fC(Pe, ishconvection, true, feplus) + De * fD(Pe, ishconvection, true, feplus);
		sl[NUSHA_SL][iP].aw = (Fw)*fC(Pw, ishconvection, true, fwplus) + Dw * fD(Pw, ishconvection, true, fwplus);
		sl[NUSHA_SL][iP].an = -(Fn)*fC(Pn, ishconvection, true, fnplus) + Dn * fD(Pn, ishconvection, true, fnplus);
		sl[NUSHA_SL][iP].as = (Fs)*fC(Ps, ishconvection, true, fsplus) + Ds * fD(Ps, ishconvection, true, fsplus);
		sl[NUSHA_SL][iP].at = -(Ft)*fC(Pt, ishconvection, true, ftplus) + Dt * fD(Pt, ishconvection, true, ftplus);
		sl[NUSHA_SL][iP].ab = (Fb)*fC(Pb, ishconvection, true, fbplus) + Db * fD(Pb, ishconvection, true, fbplus);
		//sl[NUSHA_SL][iP].ap=sl[NUSHA_SL][iP].ae+sl[NUSHA_SL][iP].aw+sl[NUSHA_SL][iP].an+sl[NUSHA_SL][iP].as+sl[NUSHA_SL][iP].at+sl[NUSHA_SL][iP].ab;

		// ������ ��� ����������� ������ � ��������� � ����������. 7.05.2017.
		//sumanb=sl[NUSHA_SL][iP].ae+sl[NUSHA_SL][iP].aw+sl[NUSHA_SL][iP].an+sl[NUSHA_SL][iP].as+sl[NUSHA_SL][iP].at+sl[NUSHA_SL][iP].ab;
		//13 ������� 2016.
		//sumanb = fabs(sl[NUSHA_SL][iP].ae) + fabs(sl[NUSHA_SL][iP].aw) + fabs(sl[NUSHA_SL][iP].an) + fabs(sl[NUSHA_SL][iP].as) + fabs(sl[NUSHA_SL][iP].at) + fabs(sl[NUSHA_SL][iP].ab);

		// 08.05.2017.
		// ��� ���������:
		// ����� ������������� !!! (���������� �� ����).

		sl[NUSHA_SL][iP].ap = +(Fe)*fC(Pe, ishconvection, true, feplus) + De * fD(Pe, ishconvection, true, feplus);
		sl[NUSHA_SL][iP].ap += -(Fw)*fC(Pw, ishconvection, true, fwplus) + Dw * fD(Pw, ishconvection, true, fwplus);
		sl[NUSHA_SL][iP].ap += +(Fn)*fC(Pn, ishconvection, true, fnplus) + Dn * fD(Pn, ishconvection, true, fnplus);
		sl[NUSHA_SL][iP].ap += -(Fs)*fC(Ps, ishconvection, true, fsplus) + Ds * fD(Ps, ishconvection, true, fsplus);
		sl[NUSHA_SL][iP].ap += +(Ft)*fC(Pt, ishconvection, true, ftplus) + Dt * fD(Pt, ishconvection, true, ftplus);
		sl[NUSHA_SL][iP].ap += -(Fb)*fC(Pb, ishconvection, true, fbplus) + Db * fD(Pb, ishconvection, true, fbplus);

		/*
		sumanb = +(Fe)*fC(Pe, ishconvection, true, feplus) + De * fD(Pe, ishconvection, true, feplus);
		sumanb += -(Fw)*fC(Pw, ishconvection, true, fwplus) + Dw * fD(Pw, ishconvection, true, fwplus);
		sumanb += +(Fn)*fC(Pn, ishconvection, true, fnplus) + Dn * fD(Pn, ishconvection, true, fnplus);
		sumanb += -(Fs)*fC(Ps, ishconvection, true, fsplus) + Ds * fD(Ps, ishconvection, true, fsplus);
		sumanb += +(Ft)*fC(Pt, ishconvection, true, ftplus) + Dt * fD(Pt, ishconvection, true, ftplus);
		sumanb += -(Fb)*fC(Pb, ishconvection, true, fbplus) + Db * fD(Pb, ishconvection, true, fbplus);
		*/
	}
	else if (ishconvection >= QUICK)
	{
		// � 3D ������������ ������ ����� ������������ �� ��� ���������� �����.
		// ����� �������� ����� ������ ������� � ����������� � ������� ��������� ���������� ���������.
		
		// X - direction
		doublereal positionxP, positionxE, positionxW, positionxEE, positionxWW, positionxe, positionxw;
		doublereal SpeedP = 0.0, SpeedE = 0.0, SpeedW = 0.0, SpeedEE = 0.0, SpeedWW = 0.0, SpeedN = 0.0, SpeedS = 0.0;
		doublereal SpeedNN = 0.0, SpeedSS = 0.0, SpeedT = 0.0, SpeedB = 0.0, SpeedTT = 0.0, SpeedBB = 0.0;
		doublereal Speede = 0.0, Speedw = 0.0, Speedn = 0.0, Speeds = 0.0, Speedt = 0.0, Speedb = 0.0;
		// Y - direction
		doublereal positionyP, positionyN, positionyS, positionyNN, positionySS, positionyn, positionys;
		// Z - direction
		doublereal positionzP, positionzT, positionzB, positionzTT, positionzBB, positionzt, positionzb;

		TOCHKA pointP;
		center_cord3D(iP, nvtx, pa, pointP, 100);
		positionxP = pointP.x; positionyP = pointP.y; positionzP = pointP.z;
		SpeedP = potent[NUSHA][iP];
		// X - direction
		if (!bE) {
			SpeedE = potent[NUSHA][iE];
			center_cord3D(iE, nvtx, pa, pointP, E_SIDE);
			positionxE = pointP.x;
			positionxe = positionxP + 0.5*dx;

			integer iEE = neighbors_for_the_internal_node[EE_SIDE][iP].iNODE1;
			if ((iEE >= 0) && (iEE < maxelm)) {
				// ���������� ����
				SpeedEE = potent[NUSHA][iEE];
				center_cord3D(iEE, nvtx, pa, pointP, EE_SIDE);
				positionxEE = pointP.x;
			}
			else
			{
				// ��������� ����
				SpeedEE = potent[NUSHA][iEE];
				volume3D(iE, nvtx, pa, pointP.x, pointP.y, pointP.z);
				positionxEE = positionxE + 0.5*pointP.x;
			}
		}
		else {
			// ��� ��������� ����
			SpeedE = potent[NUSHA][iE];
			SpeedEE = potent[NUSHA][iE];
			positionxe = positionxP + 0.5*dx;
			positionxE = positionxP + 0.5*dx;
			positionxEE = positionxP + dx; // ����� ���� �� ���������� !
		}

		if (!bW) {
			center_cord3D(iW, nvtx, pa, pointP, W_SIDE);
			positionxW = pointP.x;
			positionxw = positionxP - 0.5*dx;
			SpeedW = potent[NUSHA][iW];

			integer iWW = neighbors_for_the_internal_node[WW_SIDE][iP].iNODE1;
			if ((iWW >= 0) && (iWW < maxelm)) {
				// ���������� ����
				SpeedWW = potent[NUSHA][iWW];
				center_cord3D(iWW, nvtx, pa, pointP, WW_SIDE);
				positionxWW = pointP.x;
			}
			else
			{
				// ��������� ����
				SpeedWW = potent[NUSHA][iWW];
				volume3D(iW, nvtx, pa, pointP.x, pointP.y, pointP.z);
				positionxWW = positionxW - 0.5*pointP.x;
			}
		}
		else {
			// ��� ��������� ����
			SpeedW = potent[NUSHA][iW]; // Attantion !! Debug TODO
			SpeedWW = potent[NUSHA][iW];
			//printf("SpeedW==%e\n",SpeedW); getchar();
			positionxw = positionxP - 0.5*dx;
			positionxW = positionxP - 0.5*dx;
			positionxWW = positionxP - dx; // ����� ���� �� ���������� !
		}

		// Y - direction
		if (!bN) {
			SpeedN = potent[NUSHA][iN];
			center_cord3D(iN, nvtx, pa, pointP, N_SIDE);
			positionyN = pointP.y;
			positionyn = positionxP + 0.5*dy;

			integer iNN = neighbors_for_the_internal_node[NN_SIDE][iP].iNODE1;
			if ((iNN >= 0) && (iNN < maxelm)) {
				// ���������� ����
				SpeedNN = potent[NUSHA][iNN];
				center_cord3D(iNN, nvtx, pa, pointP, NN_SIDE);
				positionyNN = pointP.y;
			}
			else
			{
				// ��������� ����
				SpeedNN = potent[NUSHA][iNN];
				volume3D(iN, nvtx, pa, pointP.x, pointP.y, pointP.z);
				positionyNN = positionyN + 0.5*pointP.y;
			}
		}
		else {
			// ��� ��������� ����
			SpeedN = potent[NUSHA][iN];
			SpeedNN = potent[NUSHA][iN];
			positionyn = positionyP + 0.5*dy;
			positionyN = positionyP + 0.5*dy;
			positionyNN = positionyP + dy; // ����� ���� �� ���������� !
		}

		if (!bS) {
			SpeedS = potent[NUSHA][iS];
			center_cord3D(iS, nvtx, pa, pointP, S_SIDE);
			positionyS = pointP.y;
			positionys = positionyP - 0.5*dy;

			integer iSS = neighbors_for_the_internal_node[SS_SIDE][iP].iNODE1;
			if ((iSS >= 0) && (iSS < maxelm)) {
				// ���������� ����
				SpeedSS = potent[NUSHA][iSS];
				center_cord3D(iSS, nvtx, pa, pointP, SS_SIDE);
				positionySS = pointP.y;
			}
			else
			{
				// ��������� ����
				SpeedSS = potent[NUSHA][iSS];
				volume3D(iS, nvtx, pa, pointP.x, pointP.y, pointP.z);
				positionySS = positionyS - 0.5*pointP.y;
			}
		}
		else {
			// ��� ��������� ����
			SpeedS = potent[NUSHA][iS]; // ATTANTION !!!! TODO 
			SpeedSS = potent[NUSHA][iS]; // ������� �������� ������ ������� ����.
			positionys = positionyP - 0.5*dy;
			positionyS = positionyP - 0.5*dy;
			positionySS = positionyP - dy; // ����� ���� �� ���������� !
		}

		// Z - direction
		if (!bT) {
			SpeedT = potent[NUSHA][iT];
			center_cord3D(iT, nvtx, pa, pointP, T_SIDE);
			positionzT = pointP.z;
			positionzt = positionzP + 0.5*dz;

			integer iTT = neighbors_for_the_internal_node[TT_SIDE][iP].iNODE1;
			if ((iTT >= 0) && (iTT < maxelm)) {
				// ���������� ����
				SpeedTT = potent[NUSHA][iTT];
				center_cord3D(iTT, nvtx, pa, pointP, TT_SIDE);
				positionzTT = pointP.z;
			}
			else
			{
				// ��������� ����
				SpeedTT = potent[NUSHA][iTT];
				volume3D(iT, nvtx, pa, pointP.x, pointP.y, pointP.z);
				positionzTT = positionzT + 0.5*pointP.z;
			}
		}
		else {
			// ��� ��������� ����
			SpeedT = potent[NUSHA][iT];
			SpeedTT = potent[NUSHA][iT]; // �������� ������ ������� ����
			positionzt = positionzP + 0.5*dz;
			positionzT = positionzP + 0.5*dz;
			positionzTT = positionzP + dz; // ����� ���� �� ���������� !
		}

		if (!bB) {
			SpeedB = potent[NUSHA][iB];
			center_cord3D(iB, nvtx, pa, pointP, B_SIDE);
			positionzB = pointP.z;
			positionzb = positionzP - 0.5*dz;

			integer iBB = neighbors_for_the_internal_node[BB_SIDE][iP].iNODE1;
			if ((iBB >= 0) && (iBB < maxelm)) {
				// ���������� ����
				SpeedBB = potent[NUSHA][iBB];
				center_cord3D(iBB, nvtx, pa, pointP, BB_SIDE);
				positionzBB = pointP.z;
			}
			else
			{
				// ��������� ����
				SpeedBB = potent[NUSHA][iBB];
				volume3D(iB, nvtx, pa, pointP.x, pointP.y, pointP.z);
				positionzBB = positionzB - 0.5*pointP.z;
			}
		}
		else {
			// ��� ��������� ����
			SpeedB = potent[NUSHA][iB];
			SpeedBB = potent[NUSHA][iB]; // �������� ������ ������� ����
			positionzb = positionzP - 0.5*dz;
			positionzB = positionzP - 0.5*dz;
			positionzBB = positionzP - dz; // ����� ���� �� ���������� !
		}


		if ((ishconvection >= QUICK) && (ishconvection <= FROMM)) {
			// ������ ����� ������������ �� ��������� �. ��������� PHOENICS.
		   // ������������� ishconvection ������ ��������� �������� �� ����� QUICK (������������) � ������ �� ����� UNEVENQUICK (�� ������� ���������).

			// X - direction
			Speede = cell_face_value_global(ishconvection, (Fe), SpeedW, SpeedP, SpeedE, SpeedEE);
			Speedw = cell_face_value_global(ishconvection, (Fw), SpeedWW, SpeedW, SpeedP, SpeedE);
			// Y - direction
			Speedn = cell_face_value_global(ishconvection, (Fn), SpeedS, SpeedP, SpeedN, SpeedNN);
			Speeds = cell_face_value_global(ishconvection, (Fs), SpeedSS, SpeedS, SpeedP, SpeedN);
			// Z - direction
			Speedt = cell_face_value_global(ishconvection, (Ft), SpeedB, SpeedP, SpeedT, SpeedTT);
			Speedb = cell_face_value_global(ishconvection, (Fb), SpeedBB, SpeedB, SpeedP, SpeedT);

		}

		if (ishconvection >= UNEVENQUICK) {


			/*
			// ������������������ �������� ��������� � ����� ���������� ���������� ����� QUICK �� ������������� �����.
			// ���������� ���� ������������ �� ������: ...
			// � ������ ������ ������ ���������� �� ������������.
			//doublereal gamma1E, gamma2E, gamma1W, gamma2W, delta1E, delta2E, delta1W, delta2W;
			//doublereal gamma1N, gamma2N, gamma1S, gamma2S, delta1N, delta2N, delta1S, delta2S;
			//doublereal gamma1T, gamma2T, gamma1B, gamma2B, delta1T, delta2T, delta1B, delta2B;
			// X - direction
			// gamma
			//gamma1E=((positionxe-positionxE)*(positionxe-positionxP))/((positionxW-positionxE)*(positionxW-positionxP));
			//gamma2E=((positionxe-positionxP)*(positionxe-positionxW))/((positionxE-positionxP)*(positionxE-positionxW));
			//gamma1W=((positionxw-positionxP)*(positionxw-positionxW))/((positionxWW-positionxP)*(positionxWW-positionxW));
			//gamma2W=((positionxw-positionxW)*(positionxw-positionxWW))/((positionxP-positionxW)*(positionxP-positionxWW));
			// delta
			//delta1E=((positionxe-positionxEE)*(positionxe-positionxE))/((positionxP-positionxEE)*(positionxP-positionxE));
			//delta2E=((positionxe-positionxE)*(positionxe-positionxP))/((positionxEE-positionxE)*(positionxEE-positionxP));
			//delta1W=((positionxw-positionxE)*(positionxw-positionxP))/((positionxW-positionxE)*(positionxW-positionxP));
			//delta2W=((positionxw-positionxP)*(positionxw-positionxW))/((positionxE-positionxP)*(positionxE-positionxW));
			// Y - direction
			// gamma
			//gamma1N=((positionyn-positionyN)*(positionyn-positionyP))/((positionyS-positionyN)*(positionyS-positionyP));
			//gamma2N=((positionyn-positionyP)*(positionyn-positionyS))/((positionyN-positionyP)*(positionyN-positionyS));
			//gamma1S=((positionys-positionyP)*(positionys-positionyS))/((positionySS-positionyP)*(positionySS-positionyS));
			//gamma2S=((positionys-positionyS)*(positionys-positionySS))/((positionyP-positionyS)*(positionyP-positionySS));
			// delta
			//delta1N=((positionyn-positionyNN)*(positionyn-positionyN))/((positionyP-positionyNN)*(positionyP-positionyN));
			//delta2N=((positionyn-positionyN)*(positionyn-positionyP))/((positionyNN-positionyN)*(positionyNN-positionyP));
			//delta1S=((positionys-positionyN)*(positionys-positionyP))/((positionyS-positionyN)*(positionyS-positionyP));
			//delta2S=((positionys-positionyP)*(positionys-positionyS))/((positionyN-positionyP)*(positionyN-positionyS));
			// Z - direction
			// gamma
			//gamma1T=((positionzt-positionzT)*(positionzt-positionzP))/((positionzB-positionzT)*(positionzB-positionzP));
			//gamma2T=((positionzt-positionzP)*(positionzt-positionzB))/((positionzT-positionzP)*(positionzT-positionzB));
			//gamma1B=((positionzb-positionzP)*(positionzb-positionzB))/((positionzBB-positionzP)*(positionzBB-positionzB));
			//gamma2B=((positionzb-positionzB)*(positionzb-positionzBB))/((positionzP-positionzB)*(positionzP-positionzBB));
			// delta
			//delta1T=((positionzt-positionzTT)*(positionzt-positionzT))/((positionzP-positionzTT)*(positionzP-positionzT));
			//delta2T=((positionzt-positionzT)*(positionzt-positionzP))/((positionzTT-positionzT)*(positionzTT-positionzP));
			//delta1B=((positionzb-positionzT)*(positionzb-positionzP))/((positionzB-positionzT)*(positionzB-positionzP));
			//delta2B=((positionzb-positionzP)*(positionzb-positionzB))/((positionzT-positionzP)*(positionzT-positionzB));
			*/




			// ���������� ������� �������� �� ����� ��
			// ������������ ����� �������� QUICK.
			/* ������� ������������:
			 *  A	B	C	D	e	+/-
			 *  W	P	E	-	e	+
			 *  -	P	E	EE  e   -
			 *  WW   W	P	-	w	+
			 *  -	W	P	E	w	-
			 *  S	P	N	-	n	+
			 *  -	P	N	NN  n	-
			 *  SS   S	P	-	s	+
			 *  -	S	P	N	s	-
			 *  B	P	T	-	t	+
			 *  -	P	T	TT  t	-
			 *  BB   B	P	-	b	+
			 *  -	B	P	T	b	-
			 */

			if (ishconvection == UNEVENQUICK) {
				// X - direction
				Speede = workQUICK(dx, 2.0*(positionxE - positionxe), positionxW, positionxP, positionxE, positionxEE, SpeedW, SpeedP, SpeedE, SpeedEE, (Fe));
				Speedw = workQUICK(2.0*(positionxw - positionxW), dx, positionxWW, positionxW, positionxP, positionxE, SpeedWW, SpeedW, SpeedP, SpeedE, (Fw));
				// Y - direction
				Speedn = workQUICK(dy, 2.0*(positionyN - positionyn), positionyS, positionyP, positionyN, positionyNN, SpeedS, SpeedP, SpeedN, SpeedNN, (Fn));
				Speeds = workQUICK(2.0*(positionys - positionyS), dy, positionySS, positionyS, positionyP, positionyN, SpeedSS, SpeedS, SpeedP, SpeedN, (Fs));
				// Z - direction
				Speedt = workQUICK(dz, 2.0*(positionzT - positionzt), positionzB, positionzP, positionzT, positionzTT, SpeedB, SpeedP, SpeedT, SpeedTT, (Ft));
				Speedb = workQUICK(2.0*(positionzb - positionzB), dz, positionzBB, positionzB, positionzP, positionzT, SpeedBB, SpeedB, SpeedP, SpeedT, (Fb));
			}

			if ((ishconvection > UNEVENQUICK) && (ishconvection <= UNEVEN_CUBISTA)) {
				// ���� �� ������ ������ ������������� ����������� ������������ ������ ������ ������ �����:
				// 1. UNEVEN_MUSCL, 2. UNEVEN_SOUCUP, 3. UNEVEN_HLPA, 4. UNEVEN_SMART.
				// ������������� ����� ������ ��������������� ��������.

				// X - direction
				Speede = workKN_VOLKOV(positionxW, positionxP, positionxE, positionxEE, SpeedW, SpeedP, SpeedE, SpeedEE, (Fe), ishconvection);
				Speedw = workKN_VOLKOV(positionxWW, positionxW, positionxP, positionxE, SpeedWW, SpeedW, SpeedP, SpeedE, (Fw), ishconvection);
				// Y - direction
				Speedn = workKN_VOLKOV(positionyS, positionyP, positionyN, positionyNN, SpeedS, SpeedP, SpeedN, SpeedNN, (Fn), ishconvection);
				Speeds = workKN_VOLKOV(positionySS, positionyS, positionyP, positionyN, SpeedSS, SpeedS, SpeedP, SpeedN, (Fs), ishconvection);
				// Z - direction
				Speedt = workKN_VOLKOV(positionzB, positionzP, positionzT, positionzTT, SpeedB, SpeedP, SpeedT, SpeedTT, (Ft), ishconvection);
				Speedb = workKN_VOLKOV(positionzBB, positionzB, positionzP, positionzT, SpeedBB, SpeedB, SpeedP, SpeedT, (Fb), ishconvection);

				// debug ������ �������� ������.
				//printf("%f, %f, %f, %f, %f, %f\n",Speede,Speedw,Speedn,Speeds,Speedt,Speedb);
				//getchar();
			}

		} // endif (ishconvection >= UNEVENQUICK)



		// ������: SIMPLE method for the solution of incompressible flows on non-staggered grids
		// I. Sezai - Eastern Mediterranean University, Mechanical Engineering Department, Mersin 10-Turkey Revised in January, 2011.

		// ���������� ������������� ����������� �������:
		// ����������� ����� ���������� ���������:
		// ������ ����������� ������ ��������������� �����, 
		// � ���������� ����� ������������ � ������ ����� 
		// �������� ������� ���������.
		if (1) {

			/*
			sl[NUSHA_SL][iP].ae=De*fD(Pe, EXP2, true, feplus) + fmax(-(Fe),0.0);
			sl[NUSHA_SL][iP].aw=Dw*fD(Pw, EXP2, true, fwplus) + fmax((Fw),0.0);
			sl[NUSHA_SL][iP].an=Dn*fD(Pn, EXP2, true, fnplus) + fmax(-(Fn),0.0);
			sl[NUSHA_SL][iP].as=Ds*fD(Ps, EXP2, true, fsplus) + fmax((Fs),0.0);
			sl[NUSHA_SL][iP].at=Dt*fD(Pt, EXP2, true, ftplus) + fmax(-(Ft),0.0);
			sl[NUSHA_SL][iP].ab=Db*fD(Pb, EXP2, true, fbplus) + fmax((Fb),0.0);
			*/

			// ������� ��� ����������� ������ � ������������� � ���������� 7.05.2017.
			// ����� ������ UDS.
			// ��� ����������� � ����������.
			sl[NUSHA_SL][iP].ae = De + fmax(-(Fe), 0.0);
			sl[NUSHA_SL][iP].aw = Dw + fmax((Fw), 0.0);
			sl[NUSHA_SL][iP].an = Dn + fmax(-(Fn), 0.0);
			sl[NUSHA_SL][iP].as = Ds + fmax((Fs), 0.0);
			sl[NUSHA_SL][iP].at = Dt + fmax(-(Ft), 0.0);
			sl[NUSHA_SL][iP].ab = Db + fmax((Fb), 0.0);


			// 08.05.2017.
			// ��� ���������:
			// ����� ������������� !!! (���������� �� ����).
			sl[NUSHA_SL][iP].ap = De + fmax(+(Fe), 0.0);
			sl[NUSHA_SL][iP].ap += Dw + fmax(-(Fw), 0.0);
			sl[NUSHA_SL][iP].ap += Dn + fmax(+(Fn), 0.0);
			sl[NUSHA_SL][iP].ap += Ds + fmax(-(Fs), 0.0);
			sl[NUSHA_SL][iP].ap += Dt + fmax(+(Ft), 0.0);
			sl[NUSHA_SL][iP].ap += Db + fmax(-(Fb), 0.0);

			/*
			sumanb = De + fmax(+(Fe), 0.0);
			sumanb += Dw + fmax(-(Fw), 0.0);
			sumanb += Dn + fmax(+(Fn), 0.0);
			sumanb += Ds + fmax(-(Fs), 0.0);
			sumanb += Dt + fmax(+(Ft), 0.0);
			sumanb += Db + fmax(-(Fb), 0.0);
			*/
		}
		else {

			// 8.05.2017
			// �� � ���� ������ �� �������� ��� ����� ����.


			// ��� ������ ������ �� ��������, ������� ���� � ���������� � ���� ������� ����������,
			// �� ��� ����� ���������� ������������ ��� �� ������������� ����������� ������.
		   // 30 07 2015
		   // TODO
		   // ������ ������ ������� ����� ���������� �� UDS.
			if (!bE) {
				// ������ ����������.
				sl[NUSHA_SL][iP].ae = De + fmax(-(Fe), 0.0);
			}
			else {
				integer inumber = iE - maxelm;
				if (border_neighbor[inumber].MCB == (ls + lw)) {
					// ������� �� ���������: ������ ������.
					// �������� ������� ���� �� �������, ��� �� ����� ������� ������.
					sl[NUSHA_SL][iP].ae = De + fabs(Fe);
				}
				else {
					// �� ���� ��������� ������� ����� ������� ������� �� �������.
					sl[NUSHA_SL][iP].ae = De + fabs(Fe);
				}
			}

			if (!bW) {
				// ������ ����������.
				sl[NUSHA_SL][iP].aw = Dw + fmax((Fw), 0.0);
			}
			else {
				integer inumber = iW - maxelm;
				if (border_neighbor[inumber].MCB == (ls + lw)) {
					// ������� �� ���������: ������ ������.
					// �������� ������� ���� �� �������, ��� �� ����� ������� ������.
					sl[NUSHA_SL][iP].aw = Dw + fabs(Fw);
				}
				else {
					// �� ���� ��������� ������� ����� ������� ������� �� �������.
					sl[NUSHA_SL][iP].aw = Dw + fabs(Fw);
				}
			}

			if (!bN) {
				// ������ ����������.
				sl[NUSHA_SL][iP].an = Dn + fmax(-(Fn), 0.0);
			}
			else {
				integer inumber = iN - maxelm;
				if (border_neighbor[inumber].MCB == (ls + lw)) {
					// ������� �� ���������: ������ ������.
					// �������� ������� ���� �� �������, ��� �� ����� ������� ������.
					sl[NUSHA_SL][iP].an = Dn + fabs(Fn);
				}
				else {
					// �� ���� ��������� ������� ����� ������� ������� �� �������.
					sl[NUSHA_SL][iP].an = Dn + fabs(Fn);
				}
			}

			if (!bS) {
				// ������ ����������.
				sl[NUSHA_SL][iP].as = Ds + fmax((Fs), 0.0);
			}
			else {
				integer inumber = iS - maxelm;
				if (border_neighbor[inumber].MCB == (ls + lw)) {
					// ������� �� ���������: ������ ������.
					// �������� ������� ���� �� �������, ��� �� ����� ������� ������.
					sl[NUSHA_SL][iP].as = Ds + fabs(Fs);
				}
				else {
					// �� ���� ��������� ������� ����� ������� ������� �� �������.
					sl[NUSHA_SL][iP].as = Ds + fabs(Fs);
				}
			}

			if (!bT) {
				// ������ ����������.
				sl[NUSHA_SL][iP].at = Dt + fmax(-(Ft), 0.0);
			}
			else {
				integer inumber = iT - maxelm;
				if (border_neighbor[inumber].MCB == (ls + lw)) {
					// ������� �� ���������: ������ ������.
					// �������� ������� ���� �� �������, ��� �� ����� ������� ������.
					sl[NUSHA_SL][iP].at = Dt + fabs(Ft);
				}
				else {
					// �� ���� ��������� ������� ����� ������� ������� �� �������.
					sl[NUSHA_SL][iP].at = Dt + fabs(Ft);
				}
			}

			if (!bB) {
				// ������ ����������.
				sl[NUSHA_SL][iP].ab = Db + fmax((Fb), 0.0);
			}
			else {
				integer inumber = iB - maxelm;
				if (border_neighbor[inumber].MCB == (ls + lw)) {
					// ������� �� ���������: ������ ������.
					// �������� ������� ���� �� �������, ��� �� ����� ������� ������.
					sl[NUSHA_SL][iP].ab = Db + fabs(Fb);
				}
				else {
					// �� ���� ��������� ������� ����� ������� ������� �� �������.
					sl[NUSHA_SL][iP].ab = Db + fabs(Fb);
				}
			}

			// 7.05.2017 ������� ��� ����������� ������ � ��������������� � ����������.
			sl[NUSHA_SL][iP].ap = sl[NUSHA_SL][iP].ae + sl[NUSHA_SL][iP].aw + sl[NUSHA_SL][iP].an + sl[NUSHA_SL][iP].as + sl[NUSHA_SL][iP].at + sl[NUSHA_SL][iP].ab;

			//sumanb = sl[NUSHA_SL][iP].ae + sl[NUSHA_SL][iP].aw + sl[NUSHA_SL][iP].an + sl[NUSHA_SL][iP].as + sl[NUSHA_SL][iP].at + sl[NUSHA_SL][iP].ab;

		}


		// 7.05.2017 ������� ��� ����������� ������ � ��������������� � ����������.
		//sl[NUSHA_SL][iP].ap=sl[NUSHA_SL][iP].ae+sl[NUSHA_SL][iP].aw+sl[NUSHA_SL][iP].an+sl[NUSHA_SL][iP].as+sl[NUSHA_SL][iP].at+sl[NUSHA_SL][iP].ab;

		//sumanb=sl[NUSHA_SL][iP].ae+sl[NUSHA_SL][iP].aw+sl[NUSHA_SL][iP].an+sl[NUSHA_SL][iP].as+sl[NUSHA_SL][iP].at+sl[NUSHA_SL][iP].ab;

		//13 ������� 2016.
		//sl[NUSHA_SL][iP].ap = fabs(sl[NUSHA_SL][iP].ae) + fabs(sl[NUSHA_SL][iP].aw) + fabs(sl[NUSHA_SL][iP].an) + fabs(sl[NUSHA_SL][iP].as) + fabs(sl[NUSHA_SL][iP].at) + fabs(sl[NUSHA_SL][iP].ab);
		//sumanb = fabs(sl[NUSHA_SL][iP].ae) + fabs(sl[NUSHA_SL][iP].aw) + fabs(sl[NUSHA_SL][iP].an) + fabs(sl[NUSHA_SL][iP].as) + fabs(sl[NUSHA_SL][iP].at) + fabs(sl[NUSHA_SL][iP].ab);


		if (1) {
			// ����� � ������ ����� (����� ���������� ���������):
			// X - direction
			attrs += -fmax((Fe), 0)*(Speede - SpeedP) + fmax(-(Fe), 0)*(Speede - SpeedE);
			attrs += -fmax(-(Fw), 0)*(Speedw - SpeedP) + fmax((Fw), 0)*(Speedw - SpeedW);
			// Y - direction
			attrs += -fmax((Fn), 0)*(Speedn - SpeedP) + fmax(-(Fn), 0)*(Speedn - SpeedN);
			attrs += -fmax(-(Fs), 0)*(Speeds - SpeedP) + fmax((Fs), 0)*(Speeds - SpeedS);
			// Z - direction
			attrs += -fmax((Ft), 0)*(Speedt - SpeedP) + fmax(-(Ft), 0)*(Speedt - SpeedT);
			attrs += -fmax(-(Fb), 0)*(Speedb - SpeedP) + fmax((Fb), 0)*(Speedb - SpeedB);
		}
		else {
			// �������.
		   // 30 07 2015
		   // TODO
		   // ������ ������ ������� ����� ���������� �� UDS.
			if (!bE) {
				attrs += -fmax((Fe), 0)*(Speede - SpeedP) + fmax(-(Fe), 0)*(Speede - SpeedE);
			}
			if (!bW) {
				attrs += -fmax(-(Fw), 0)*(Speedw - SpeedP) + fmax((Fw), 0)*(Speedw - SpeedW);
			}
			if (!bN) {
				attrs += -fmax((Fn), 0)*(Speedn - SpeedP) + fmax(-(Fn), 0)*(Speedn - SpeedN);
			}
			if (!bS) {
				attrs += -fmax(-(Fs), 0)*(Speeds - SpeedP) + fmax((Fs), 0)*(Speeds - SpeedS);
			}
			if (!bT) {
				attrs += -fmax((Ft), 0)*(Speedt - SpeedP) + fmax(-(Ft), 0)*(Speedt - SpeedT);
			}
			if (!bB) {
				attrs += -fmax(-(Fb), 0)*(Speedb - SpeedP) + fmax((Fb), 0)*(Speedb - SpeedB);
			}

		}

		//attrs=0.0; // ����� ����� ������� ����������� ����������� (�������� ����� ��������).

	}

	// ��������� ����������� ��������� ���������������.
	// ������ �������. �������������� ���������� �� ������������.
	// TODO Future.

	// ������������ ����
	doublereal dSc = 0.0, dSp = 0.0;

	// ��� ������������� �������� Fe, Fw, Fn, Fs, Ft, Fb - ���������� �������� ������ ����� ���������� �������������� ���-���.
		// 02.05.2017
		// ��� ������� �.�. �������� � ������������� ������������ �������������.
		// ������ ���� �������: deltaF=(Fe-Fw+Fn-Fs+Ft-Fb);
		// ����������� ����� ����������� � ���������� �� ANSYS Icepak.
		// ��� ����������� �� ���� �������� ����� �� ���������� ���� - ��� ���������� ������ ��������.
		//doublereal deltaF=(Fe-Fw+Fn-Fs+Ft-Fb);
		// ��� ������ ���������� ��������� ������������� ��� ��������� ����� ����.
		// � ������ ���� ����������� ��������� ��� �������� �������� �������� � ������������ ������ (��)
		// ��� ���������� ��������� ��������� ������������ ������������, �� ������ ���������� ������ ���������� ����� 
		// ����� ����� ���������� ��������� ���������.
		// 8.05.2017.
	doublereal deltaF = fabs(Fe - Fw + Fn - Fs + Ft - Fb);
	if (b_on_adaptive_local_refinement_mesh) {
		deltaF = fabs(Fe - Fw + Fn - Fs + Ft - Fb + Fe2 - Fw2 + Fn2 - Fs2 + Ft2 - Fb2 + Fe3 - Fw3 + Fn3 - Fs3 + Ft3 - Fb3 + Fe4 - Fw4 + Fn4 - Fs4 + Ft4 - Fb4);
	}
	if (deltaF != deltaF) {
		printf("Fe=%e Fw=%e Fn=%e Fs=%e Ft=%e Fb=%e\n", Fe, Fw, Fn, Fs, Ft, Fb);
		printf("Fe2=%e Fw2=%e Fn2=%e Fs2=%e Ft2=%e Fb2=%e\n", Fe2, Fw2, Fn2, Fs2, Ft2, Fb2);
		printf("Fe3=%e Fw3=%e Fn3=%e Fs3=%e Ft3=%e Fb3=%e\n", Fe3, Fw3, Fn3, Fs3, Ft3, Fb3);
		printf("Fe4=%e Fw4=%e Fn4=%e Fs4=%e Ft4=%e Fb4=%e\n", Fe4, Fw4, Fn4, Fs4, Ft4, Fb4);
		printf("ERROR deltaF=%e\n", deltaF);
	}

	if (sl[NUSHA_SL][iP].ap != sl[NUSHA_SL][iP].ap) {
		printf("ap!=ap assemble bug. Apriory deltaF. iP=%lld ap=%e\n", iP, sl[NUSHA_SL][iP].ap);
		system("pause");
	}
	sl[NUSHA_SL][iP].ap += deltaF;//-->//sl[NUSHA_SL][iP].ap+=apzero1+deltaF;//+deltaF; // ������������ ������� ������� deltaF ������ ��������������.  ���������� ��������� 
	if (sl[NUSHA_SL][iP].ap != sl[NUSHA_SL][iP].ap) {
		printf("ap!=ap assemble bug. Apost deltaF. iP=%lld ap=%e\n", iP, sl[NUSHA_SL][iP].ap);
		system("pause");
	}

	
	sl[NUSHA_SL][iP].b =  attrs; // ����� ���������� ��������� ��� ����� ������� ����������� �����������.
	if (sl[NUSHA_SL][iP].b != sl[NUSHA_SL][iP].b) {
		printf("exptsr+attrs error NAN or INF in control volume %lld NUSHA\n", iP);
		system("pause");
	}

	dSc = eqin.fluidinfo[0].c_b2*(potent[GRADXNUSHA][iP]* potent[GRADXNUSHA][iP]+ potent[GRADYNUSHA][iP] * potent[GRADYNUSHA][iP]+potent[GRADZNUSHA][iP] * potent[GRADZNUSHA][iP]);
	// ���������
	doublereal kappa = (rP*potent[NUSHA][iP]) / prop[MU_DYNAMIC_VISCOSITY][iP];
	if (kappa < 1.0e-4) kappa = 1.0e-4; // ������, ���������, �������.
	doublereal fnu1 = ((kappa)*(kappa)*(kappa)) / ((kappa)*(kappa)*(kappa) +(eqin.fluidinfo[0].c_nu1)*(eqin.fluidinfo[0].c_nu1)*(eqin.fluidinfo[0].c_nu1));
	doublereal fnu2 = 1.0-(kappa)/(1.0+ fnu1*kappa);
	//fnu2 = pow(1.0+kappa/5.0,-3.0);
	//doublereal fnu3 = (1.0+kappa*fnu1)*(1.0-kappa*fnu2) / kappa;
	doublereal Strain_loc = potent[CURL][iP];
	Strain_loc = potent[CURL][iP] + 2.0*fmin(0.0, SInvariantStrainRateTensor[iP] - potent[CURL][iP]); // 01.10.2019
	//doublereal Strain = Strain_loc*fnu3 + fnu2*potent[NUSHA][iP]/((eqin.fluidinfo[0].karman*distance_to_wall[iP])*(eqin.fluidinfo[0].karman*distance_to_wall[iP]));
	doublereal Strain = Strain_loc + fnu2*potent[NUSHA][iP] / ((eqin.fluidinfo[0].karman*distance_to_wall[iP])*(eqin.fluidinfo[0].karman*distance_to_wall[iP]));
	doublereal ft2 = eqin.fluidinfo[0].C_t3*exp(-eqin.fluidinfo[0].C_t4*(kappa)*(kappa));
	dSc += eqin.fluidinfo[0].c_b1*(1.0 - ft2)*rP*Strain*potent[NUSHA][iP];
	//dSc += eqin.fluidinfo[0].c_b1*rP*Strain*potent[NUSHA][iP];
	// ����������
	doublereal r = potent[NUSHA][iP] / (Strain*(eqin.fluidinfo[0].karman*distance_to_wall[iP])*(eqin.fluidinfo[0].karman*distance_to_wall[iP]));
	if ((r != r) || (fabs(r) > 5.0)) {
		doublereal fw = pow((1.0 + eqin.fluidinfo[0].c_w3*eqin.fluidinfo[0].c_w3*eqin.fluidinfo[0].c_w3*eqin.fluidinfo[0].c_w3*eqin.fluidinfo[0].c_w3*eqin.fluidinfo[0].c_w3), 1.0 / 6.0);
		dSc -= (eqin.fluidinfo[0].c_w1*fw - (eqin.fluidinfo[0].c_b1*ft2) / (eqin.fluidinfo[0].karman*eqin.fluidinfo[0].karman))*(potent[NUSHA][iP] / distance_to_wall[iP])*(potent[NUSHA][iP] / distance_to_wall[iP]);
	}
	else {
		doublereal g = r + eqin.fluidinfo[0].c_w2*(r*r*r*r*r*r - r);
		doublereal fw = g * pow((1.0 + eqin.fluidinfo[0].c_w3*eqin.fluidinfo[0].c_w3*eqin.fluidinfo[0].c_w3*eqin.fluidinfo[0].c_w3*eqin.fluidinfo[0].c_w3*eqin.fluidinfo[0].c_w3) / (g*g*g*g*g*g + eqin.fluidinfo[0].c_w3*eqin.fluidinfo[0].c_w3*eqin.fluidinfo[0].c_w3*eqin.fluidinfo[0].c_w3*eqin.fluidinfo[0].c_w3*eqin.fluidinfo[0].c_w3), 1.0 / 6.0);
		dSc -= (eqin.fluidinfo[0].c_w1*fw - (eqin.fluidinfo[0].c_b1*ft2) / (eqin.fluidinfo[0].karman*eqin.fluidinfo[0].karman))*(potent[NUSHA][iP] / distance_to_wall[iP])*(potent[NUSHA][iP] / distance_to_wall[iP]);
	}
	sl[NUSHA_SL][iP].b += dSc * dx*dy*dz; // ���������� ���� ��������. (���� ���� ������, ������ �����-������.)
	if (sl[NUSHA_SL][iP].b != sl[NUSHA_SL][iP].b) {
		printf("dSc*dx*dy*dz error NAN or INF in control volume %lld\n", iP);
		system("pause");
	}

	if (fabs(sl[NUSHA_SL][iP].ap) < 1.0e-30) {
		sl[NUSHA_SL][iP].ap = 1.0;
		sl[NUSHA_SL][iP].b = potent[NUSHA][iP];
		
		if (sl[NUSHA_SL][iP].b != sl[NUSHA_SL][iP].b) {
			printf("Zero ap in NUSHA component.\n");
		}
	}



	if (sl[NUSHA_SL][iP].ap != sl[NUSHA_SL][iP].ap) {
		printf("ap!=ap assemble bug. iP=%lld ap=%e\n", iP, sl[NUSHA_SL][iP].ap);
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].ae != sl[NUSHA_SL][iP].ae) {
		printf("ae!=ae assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].aw != sl[NUSHA_SL][iP].aw) {
		printf("aw!=aw assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].an != sl[NUSHA_SL][iP].an) {
		printf("an!=an assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].as != sl[NUSHA_SL][iP].as) {
		printf("as!=as assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].at != sl[NUSHA_SL][iP].at) {
		printf("at!=at assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].ab != sl[NUSHA_SL][iP].ab) {
		printf("ab!=ab assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].ae2 != sl[NUSHA_SL][iP].ae2) {
		printf("ae2!=ae2 assemble bug %e %e\n", sl[NUSHA_SL][iP].ae2, sl[NUSHA_SL][iP].ae2);
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].aw2 != sl[NUSHA_SL][iP].aw2) {
		printf("aw2!=aw2 assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].an2 != sl[NUSHA_SL][iP].an2) {
		printf("an2!=an2 assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].as2 != sl[NUSHA_SL][iP].as2) {
		printf("as2!=as2 assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].at2 != sl[NUSHA_SL][iP].at2) {
		printf("at2!=at2 assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].ab2 != sl[NUSHA_SL][iP].ab2) {
		printf("ab2!=ab2 assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].ae3 != sl[NUSHA_SL][iP].ae3) {
		printf("ae3!=ae3 assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].aw3 != sl[NUSHA_SL][iP].aw3) {
		printf("aw3!=aw3 assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].an3 != sl[NUSHA_SL][iP].an3) {
		printf("an3!=an3 assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].as3 != sl[NUSHA_SL][iP].as3) {
		printf("as3!=as3 assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].at3 != sl[NUSHA_SL][iP].at3) {
		printf("at3!=at3 assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].ab3 != sl[NUSHA_SL][iP].ab3) {
		printf("ab3!=ab3 assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].ae4 != sl[NUSHA_SL][iP].ae4) {
		printf("ae4!=ae4 assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].aw4 != sl[NUSHA_SL][iP].aw4) {
		printf("aw4!=aw4 assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].an4 != sl[NUSHA_SL][iP].an4) {
		printf("an4!=an4 assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].as4 != sl[NUSHA_SL][iP].as4) {
		printf("as4!=as4 assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].at4 != sl[NUSHA_SL][iP].at4) {
		printf("at4!=at4 assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}
	if (sl[NUSHA_SL][iP].ab4 != sl[NUSHA_SL][iP].ab4) {
		printf("ab4!=ab4 assemble bug\n");
		printf("NUSHA \n");
		system("pause");
	}

} // my_elmatr_quad_SpallartAllmares3D

// Neiman_Zero_in_Wall_STUB(inumber, slb, border_neighbor);
// ����� ���������� ������� ������� ��� ���������������� 
// ������������ ������������ �������� �� �������� ������� ������,
// ��� �������� �������� ��������� �������.
void Neiman_Zero_in_Wall_STUB(
	integer inumber,
	equation3D_bon* &slb,
	BOUND* border_neighbor
)
{
	// ���������� ������� �������.

	
	slb[inumber].ai = 1.0;
	slb[inumber].iI = border_neighbor[inumber].iI;
	slb[inumber].aw = 1.0;
	slb[inumber].iW = border_neighbor[inumber].iB;
	//printf("iI=%lld iW=%lld\n", border_neighbor[inumber].iI, border_neighbor[inumber].iB);
	//getchar();
	// ������ �����
	slb[inumber].b = 0.0;
	

	integer j, l, xitem, k;
	// ���������� �� �����������
	for (j = 0; j < 5; j++) {
		k = j; xitem = border_neighbor[inumber].iW[j];
		for (l = j + 1; l < 6; l++) {
			if (border_neighbor[inumber].iW[l] < xitem) {
				k = l; xitem = border_neighbor[inumber].iW[k];
			}
		}
		border_neighbor[inumber].iW[k] = border_neighbor[inumber].iW[j];
		border_neighbor[inumber].iW[j] = xitem;
	}

	j = 0; l = 0;
	while (border_neighbor[inumber].iW[j] == (NON_EXISTENT_NODE)) j++;

	if (j < 6) { slb[inumber].iW1 = border_neighbor[inumber].iW[j++]; l++; }
	if (j < 6) { slb[inumber].iW2 = border_neighbor[inumber].iW[j++]; l++; }
	if (j < 6) { slb[inumber].iW3 = border_neighbor[inumber].iW[j++]; l++; }
	if (j < 6) { slb[inumber].iW4 = border_neighbor[inumber].iW[j++]; l++; }

	switch (l) {
	case 0: slb[inumber].iW1 = NON_EXISTENT_NODE;
		slb[inumber].iW2 = NON_EXISTENT_NODE;
		slb[inumber].iW3 = NON_EXISTENT_NODE;
		slb[inumber].iW4 = NON_EXISTENT_NODE;
		break;
	case 1: slb[inumber].iW2 = NON_EXISTENT_NODE;
		slb[inumber].iW3 = NON_EXISTENT_NODE;
		slb[inumber].iW4 = NON_EXISTENT_NODE;
		break;
	case 2: slb[inumber].iW3 = NON_EXISTENT_NODE;
		slb[inumber].iW4 = NON_EXISTENT_NODE;
		break;
	case 3: slb[inumber].iW4 = NON_EXISTENT_NODE;
		break;
	}

	// ��������� ���������� � ������ ���������� ���� iI � ������������ 
	// ��������� ���� ��� ��������� ����� ������� ������� �� ���������. 
	// �.�. ���� iI ������ ���������� ��� ������� iI < maxelm, � ������������ 
	// ������� � �������� ������� ����� � �������� >= maxelm ������� ��� ��
	// ������������.
}

// ���� ��������� ������� ��� ���������������� ������������ ������������ ��������.
void my_elmatr_quad_SpallartAllmares3D_bound(integer inumber, integer maxelm,
	bool bDirichlet, BOUND* border_neighbor, integer ls, integer lw,
	WALL* w,
	//integer iVar,
	equation3D_bon* &slb,
	TOCHKA* pa, integer** nvtx, doublereal** prop_b, doublereal** prop,
	doublereal** potent
	//, integer iflowregime
) {



	// bDirichlet == true �������������� ������ ������ ��������� ������� �������.
	// bDirichlet == false �������������� ������ ������ ���������� ������� �������.

	// inumber - ����� ���������� ��.
	// inumber ���������� �� 0..maxbound-1

   /*
   for (integer i=0; i<lw; i++) {
	   if (w[i].bsymmetry) {
		 printf("symmetry \n");
	   }
	   if (w[i].bpressure) {
		 printf("bpressure \n");
	   }
   }
   getchar(); // debug;
   */

   // ��������.
   /* 1. ������� �������.
		 ��� ����������� ������.
		 ������� ��������� � �� �������� ������� bpressure.
		 � �� opening � ������ ��������� ������.
	  2. ������� �������. ��� �������� ����� ��� ������ ������.
   */

   // �������� ��� ��������� ��� �������� �� ������� ������� ������.
	const doublereal multiplyer_coeff_turbulent_nu_input = 2.0; // �� ������ �� ����.

   // ������� ������� ��������� ������� �������
  //if (bDirichlet && (border_neighbor[inumber].MCB<(ls + lw)) && (border_neighbor[inumber].MCB >= ls) && (!w[border_neighbor[inumber].MCB - ls].bopening) && (!w[border_neighbor[inumber].MCB - ls].bsymmetry) && (!w[border_neighbor[inumber].MCB - ls].bpressure)) {
	if (bDirichlet && (border_neighbor[inumber].MCB < (ls + lw)) && (border_neighbor[inumber].MCB >= ls) && ((!w[border_neighbor[inumber].MCB - ls].bpressure) && (!w[border_neighbor[inumber].MCB - ls].bsymmetry))) {

		//system("PAUSE");

		// ��������� ������� �������
		// ������ �������� �� �������
		// ��� �� ������� ��������� � �� �������� �������.

		slb[inumber].aw = 1.0;
		slb[inumber].ai = 0.0;

		//border_neighbor[inumber].Norm - ���������� �������.
		if (w[border_neighbor[inumber].MCB - ls].bopening) {
			if (((border_neighbor[inumber].Norm == E_SIDE || border_neighbor[inumber].Norm == W_SIDE) && (fabs(potent[VXCOR][maxelm + inumber]) > 1.0e-20)) ||
				(((border_neighbor[inumber].Norm == N_SIDE || border_neighbor[inumber].Norm == S_SIDE) && (fabs(potent[VYCOR][maxelm + inumber]) > 1.0e-20))) ||
				(((border_neighbor[inumber].Norm == T_SIDE || border_neighbor[inumber].Norm == B_SIDE) && (fabs(potent[VZCOR][maxelm + inumber]) > 1.0e-20))))
			{

				if ((border_neighbor[inumber].Norm == E_SIDE) && (potent[VXCOR][maxelm + inumber] > 0.0)) {
					// ������� ������� ������
					slb[inumber].b = multiplyer_coeff_turbulent_nu_input * prop_b[MU_DYNAMIC_VISCOSITY][inumber] / prop_b[RHO][inumber];
				}
				if ((border_neighbor[inumber].Norm == W_SIDE) && (potent[VXCOR][maxelm + inumber] < 0.0)) {
					// ������� ������� ������
					slb[inumber].b = multiplyer_coeff_turbulent_nu_input * prop_b[MU_DYNAMIC_VISCOSITY][inumber] / prop_b[RHO][inumber];
				}
				if ((border_neighbor[inumber].Norm == N_SIDE) && (potent[VYCOR][maxelm + inumber] > 0.0)) {
					// ������� ������� ������
					slb[inumber].b = multiplyer_coeff_turbulent_nu_input * prop_b[MU_DYNAMIC_VISCOSITY][inumber] / prop_b[RHO][inumber];
				}
				if ((border_neighbor[inumber].Norm == S_SIDE) && (potent[VYCOR][maxelm + inumber] < 0.0)) {
					// ������� ������� ������
					slb[inumber].b = multiplyer_coeff_turbulent_nu_input * prop_b[MU_DYNAMIC_VISCOSITY][inumber] / prop_b[RHO][inumber];
				}
				if ((border_neighbor[inumber].Norm == T_SIDE) && (potent[VZCOR][maxelm + inumber] > 0.0)) {
					// ������� ������� ������
					slb[inumber].b = multiplyer_coeff_turbulent_nu_input * prop_b[MU_DYNAMIC_VISCOSITY][inumber] / prop_b[RHO][inumber];
				}
				if ((border_neighbor[inumber].Norm == B_SIDE) && (potent[VZCOR][maxelm + inumber] < 0.0)) {
					// ������� ������� ������
					slb[inumber].b = multiplyer_coeff_turbulent_nu_input * prop_b[MU_DYNAMIC_VISCOSITY][inumber] / prop_b[RHO][inumber];
				}

			}
		}
		else if (((border_neighbor[inumber].Norm == E_SIDE || border_neighbor[inumber].Norm == W_SIDE) && (fabs(w[border_neighbor[inumber].MCB - ls].Vx) > 1.0e-20)) ||
			((border_neighbor[inumber].Norm == N_SIDE || border_neighbor[inumber].Norm == S_SIDE) && fabs(w[border_neighbor[inumber].MCB - ls].Vy) > 1.0e-20) ||
			((border_neighbor[inumber].Norm == T_SIDE || border_neighbor[inumber].Norm == B_SIDE) && fabs(w[border_neighbor[inumber].MCB - ls].Vz) > 1.0e-20))
		{

			if ((border_neighbor[inumber].Norm == E_SIDE) && (w[border_neighbor[inumber].MCB - ls].Vx > 0.0)) {
				// ������� ������� ������
				slb[inumber].b = multiplyer_coeff_turbulent_nu_input * prop_b[MU_DYNAMIC_VISCOSITY][inumber] / prop_b[RHO][inumber];
			}
			if ((border_neighbor[inumber].Norm == W_SIDE) && (w[border_neighbor[inumber].MCB - ls].Vx < 0.0)) {
				// ������� ������� ������
				slb[inumber].b = multiplyer_coeff_turbulent_nu_input * prop_b[MU_DYNAMIC_VISCOSITY][inumber] / prop_b[RHO][inumber];
			}
			if ((border_neighbor[inumber].Norm == N_SIDE) && (w[border_neighbor[inumber].MCB - ls].Vy > 0.0)) {
				// ������� ������� ������
				slb[inumber].b = multiplyer_coeff_turbulent_nu_input * prop_b[MU_DYNAMIC_VISCOSITY][inumber] / prop_b[RHO][inumber];
			}
			if ((border_neighbor[inumber].Norm == S_SIDE) && (w[border_neighbor[inumber].MCB - ls].Vy < 0.0)) {
				// ������� ������� ������
				slb[inumber].b = multiplyer_coeff_turbulent_nu_input * prop_b[MU_DYNAMIC_VISCOSITY][inumber] / prop_b[RHO][inumber];
			}
			if ((border_neighbor[inumber].Norm == T_SIDE) && (w[border_neighbor[inumber].MCB - ls].Vz > 0.0)) {
				// ������� ������� ������
				slb[inumber].b = multiplyer_coeff_turbulent_nu_input * prop_b[MU_DYNAMIC_VISCOSITY][inumber] / prop_b[RHO][inumber];
			}
			if ((border_neighbor[inumber].Norm == B_SIDE) && (w[border_neighbor[inumber].MCB - ls].Vz < 0.0)) {
				// ������� ������� ������
				slb[inumber].b = multiplyer_coeff_turbulent_nu_input * prop_b[MU_DYNAMIC_VISCOSITY][inumber] / prop_b[RHO][inumber];
			}

		}
		else {
			// ����������� ������.
			slb[inumber].b = 0.0;
		}

		slb[inumber].iI = NON_EXISTENT_NODE; // �� ������������ � �������
		slb[inumber].iW = border_neighbor[inumber].iB;
#if doubleintprecision == 1
		//printf("%lld, soseddb=%lld\n",inumber, border_neighbor[inumber].iB); getchar(); // debug
#else
		//printf("%d, soseddb=%d\n",inumber, border_neighbor[inumber].iB); getchar(); // debug
#endif


		// ��� ������� �������:
		// ������ ������������ ������� 
		// �� ����� ����.
		slb[inumber].iW1 = NON_EXISTENT_NODE;
		slb[inumber].iW2 = NON_EXISTENT_NODE;
		slb[inumber].iW3 = NON_EXISTENT_NODE;
		slb[inumber].iW4 = NON_EXISTENT_NODE;
	}
	else if (bDirichlet && ((border_neighbor[inumber].MCB == (ls + lw)) || (border_neighbor[inumber].MCB < ls))) { // 
		// �������� ���� �������� ������ �������.
		// ���� ������ ������. ������ ������ ����������� �� ������� (border_neighbor[inumber].MCB==(ls+lw)).

		// ��������� ������� �������
		// ������ ������� ���������� �� ������ ������.

		slb[inumber].aw = 1.0;
		slb[inumber].ai = 0.0;
		slb[inumber].b = 0.0; // ������� ���������������� ������������ ������������ ��������.
		slb[inumber].iI = NON_EXISTENT_NODE; // �� ������������ � �������
		slb[inumber].iW = border_neighbor[inumber].iB;

		// ��� ������� �������:
		// ������ ������������ ������� 
		// �� ����� ����.
		slb[inumber].iW1 = NON_EXISTENT_NODE;
		slb[inumber].iW2 = NON_EXISTENT_NODE;
		slb[inumber].iW3 = NON_EXISTENT_NODE;
		slb[inumber].iW4 = NON_EXISTENT_NODE;
	}
	else if ((border_neighbor[inumber].MCB < (ls + lw)) && (border_neighbor[inumber].MCB >= ls) && ((w[border_neighbor[inumber].MCB - ls].bpressure)||(w[border_neighbor[inumber].MCB - ls].bsymmetry)/*|| ((w[border_neighbor[inumber].MCB - ls].bopening))*/)) {

        // �������� ������� ������.


		if (!bDirichlet) {
			// ��� ����� ���������� �������� ����� ������� �������:
			// ���������. �.�. ��������� ��� ���������� ��������
			// ��������� � ���������� ���������������� � ���������
			// �� ������� ������� � ������������� � ���������, ��
			// ��������� ������� ��� ��������� ����������������
			// ����������� �� ��������� ��� ���������� ��������, � ��������� 
			// �� ������������� � ���������.

			Neiman_Zero_in_Wall_STUB(inumber, slb, border_neighbor);
			
		}		
	}
	else if (!bDirichlet && (border_neighbor[inumber].MCB < (ls + lw)) && (border_neighbor[inumber].MCB >= ls) && ((!w[border_neighbor[inumber].MCB - ls].bpressure) && (!w[border_neighbor[inumber].MCB - ls].bsymmetry))) {
	// ������� �������� �������. ���������� ������� �������.
	
	if (w[border_neighbor[inumber].MCB - ls].bopening) {
		if (((border_neighbor[inumber].Norm == E_SIDE || border_neighbor[inumber].Norm == W_SIDE) && (fabs(potent[VXCOR][maxelm + inumber]) > 1.0e-20)) ||
			((border_neighbor[inumber].Norm == N_SIDE || border_neighbor[inumber].Norm == S_SIDE) && (fabs(potent[VYCOR][maxelm + inumber]) > 1.0e-20)) ||
			((border_neighbor[inumber].Norm == T_SIDE || border_neighbor[inumber].Norm == B_SIDE) && (fabs(potent[VZCOR][maxelm + inumber]) > 1.0e-20)))
		{

			if ((border_neighbor[inumber].Norm == E_SIDE) && (potent[VXCOR][maxelm + inumber] < 0.0)) {
				// �������� ������� ������
				Neiman_Zero_in_Wall_STUB(inumber, slb, border_neighbor);
			}
			if ((border_neighbor[inumber].Norm == W_SIDE) && (potent[VXCOR][maxelm + inumber] > 0.0)) {
				// �������� ������� ������
				Neiman_Zero_in_Wall_STUB(inumber, slb, border_neighbor);
			}
			if ((border_neighbor[inumber].Norm == N_SIDE) && (potent[VYCOR][maxelm + inumber] < 0.0)) {
				// �������� ������� ������
				Neiman_Zero_in_Wall_STUB(inumber, slb, border_neighbor);
			}
			if ((border_neighbor[inumber].Norm == S_SIDE) && (potent[VYCOR][maxelm + inumber] > 0.0)) {
				// �������� ������� ������
				Neiman_Zero_in_Wall_STUB(inumber, slb, border_neighbor);
			}
			if ((border_neighbor[inumber].Norm == T_SIDE) && (potent[VZCOR][maxelm + inumber] < 0.0)) {
				// �������� ������� ������
				Neiman_Zero_in_Wall_STUB(inumber, slb, border_neighbor);
			}
			if ((border_neighbor[inumber].Norm == B_SIDE) && (potent[VZCOR][maxelm + inumber] > 0.0)) {
				// �������� ������� ������
				Neiman_Zero_in_Wall_STUB(inumber, slb, border_neighbor);
			}

		}
	}
	else if (((border_neighbor[inumber].Norm == E_SIDE || border_neighbor[inumber].Norm == W_SIDE) && (fabs(w[border_neighbor[inumber].MCB - ls].Vx) > 1.0e-20)) ||
		((border_neighbor[inumber].Norm == N_SIDE || border_neighbor[inumber].Norm == S_SIDE) && fabs(w[border_neighbor[inumber].MCB - ls].Vy) > 1.0e-20) ||
		((border_neighbor[inumber].Norm == T_SIDE || border_neighbor[inumber].Norm == B_SIDE) && fabs(w[border_neighbor[inumber].MCB - ls].Vz) > 1.0e-20)) 
		{

		if ((border_neighbor[inumber].Norm == E_SIDE) && (w[border_neighbor[inumber].MCB - ls].Vx < 0.0)) {
			// �������� ������� ������
			Neiman_Zero_in_Wall_STUB(inumber, slb, border_neighbor);
		}
		if ((border_neighbor[inumber].Norm == W_SIDE) && (w[border_neighbor[inumber].MCB - ls].Vx > 0.0)) {
			// �������� ������� ������
			Neiman_Zero_in_Wall_STUB(inumber, slb, border_neighbor);
		}
		if ((border_neighbor[inumber].Norm == N_SIDE) && (w[border_neighbor[inumber].MCB - ls].Vy < 0.0)) {
			// �������� ������� ������
			Neiman_Zero_in_Wall_STUB(inumber, slb, border_neighbor);
		}
		if ((border_neighbor[inumber].Norm == S_SIDE) && (w[border_neighbor[inumber].MCB - ls].Vy > 0.0)) {
			// �������� ������� ������
			Neiman_Zero_in_Wall_STUB(inumber, slb, border_neighbor);
		}
		if ((border_neighbor[inumber].Norm == T_SIDE) && (w[border_neighbor[inumber].MCB - ls].Vz < 0.0)) {
			// �������� ������� ������
			Neiman_Zero_in_Wall_STUB(inumber, slb, border_neighbor);
		}
		if ((border_neighbor[inumber].Norm == B_SIDE) && (w[border_neighbor[inumber].MCB - ls].Vz > 0.0)) {
			// �������� ������� ������
			Neiman_Zero_in_Wall_STUB(inumber, slb, border_neighbor);
		}

		}
	}

} // my_elmatr_quad_SpallartAllmares3D_bound





#endif