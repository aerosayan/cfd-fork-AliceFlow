// ���� my_unsteady_temperature.c 
//
// �������������� ������������� ������.
// ����������� ����������� ��������� ������� �����
// ������������� �� �������. 
// ������ [45] ����������� � ����� �. ���������
// "��������� ������ ����������� � �������� ��������".
// � ���� ������ ������������ ����������� ��������� �������� ���
// ���������� ����� �� �������.
// begin 2 ������� 2011 ����.

#pragma once
#ifndef  MY_UNSTEADY_TEMPERATURE_C
#define  MY_UNSTEADY_TEMPERATURE_C 1

// �������������� ��� ������ ��������:
#include "Blasius.c"
#include <ctime> // ��� ������ ������� ����������.

// �������� ����� ����� ���������� � ��������� ���� 
// report_temperature.txt
// ������������ ����������� ������� �����,
// ������������ ����������� ������� ���������,
// ������������ ����������� ������ ������.
void report_temperature(integer flow_interior,
	FLOW* &fglobal, TEMPER &t,
	BLOCK* b, integer lb, SOURCE* s, integer ls,
	WALL* w, integer lw, integer ipref, TPROP* matlist) {

	doublereal pdiss = 0.0; // ��������� �������� �������� � ��.
	doublereal tmin1 = 1.0e30, tmax1 = -1.0e30; // ����������� � ������������ ����������� � ��������� �������.

	doublereal* tmaxreportblock = nullptr;
		tmaxreportblock = new doublereal[lb];
		if (tmaxreportblock == nullptr) {
			// ������������ ������ �� ������ ������������.
			printf("Problem: not enough memory on your equipment for tmaxreportblock report_temperature...\n");
			//printf("Please any key to exit...\n");
			system("pause");
			exit(1);
		}
	doublereal* tmaxreportsource = nullptr;
		tmaxreportsource = new doublereal[ls];
		if (tmaxreportsource == nullptr) {
			// ������������ ������ �� ������ ������������.
			printf("Problem: not enough memory on your equipment for tmaxreportsource report_temperature...\n");
			//printf("Please any key to exit...\n");
			system("pause");
			exit(1);
		}
	doublereal* tmaxreportwall = nullptr;
		tmaxreportwall = new doublereal[lw];
		if (tmaxreportwall == nullptr) {
			// ������������ ������ �� ������ ������������.
			printf("Problem: not enough memory on your equipment for tmaxreportwall report_temperature...\n");
			//printf("Please any key to exit...\n");
			system("pause");
			exit(1);
		}

	const doublereal tmin = -1.0e27;

	// �������������.
	for (integer i = 0; i<lb; i++) {
		tmaxreportblock[i] = tmin;
	}
	for (integer i = 0; i<ls; i++) {
		tmaxreportsource[i] = tmin;
	}
	for (integer i = 0; i<lw; i++) {
		tmaxreportwall[i] = tmin;
	}

	/*
	// �������� �� ���� �� ������� ���������.
	for (integer i=0; i<t.maxelm+t.maxbound; i++) {
	if (i<t.maxelm) {
	TOCHKA p; // ����� - ����� ���������������� ��.
	integer ib; // ����� ����� �������� ����������� ����������� �����.
	integer iP=i;
	// ������ �� ���� ���������� ����������� ������� ��������� �������.
	center_cord3D(iP, t.nvtx, t.pa, p); // ���������� ��������� ������ ��.
	in_model_temp(p,ib,b,lb);
	if (tmaxreportblock[ib]<t.potent[iP]) {
	tmaxreportblock[ib]=t.potent[iP];
	}
	}
	else {
	// ��������� ����.
	integer inumber=i-t.maxelm;
	if (t.border_neighbor[inumber].MCB<(ls+lw)) {
	if (t.border_neighbor[inumber].MCB<ls) {
	if (tmaxreportsource[t.border_neighbor[inumber].MCB]<t.potent[i]) {
	tmaxreportsource[t.border_neighbor[inumber].MCB]=t.potent[i];
	}
	}
	else {
	if (tmaxreportwall[t.border_neighbor[inumber].MCB-ls]<t.potent[i]) {
	tmaxreportwall[t.border_neighbor[inumber].MCB-ls]=t.potent[i];
	}
	}
	}
	}
	}
	*/
	// 8 ������ 2016 ������� ����� ������� ������� �� ��������������.
	// �������� �� ���� �� ������� ���������.
	for (integer i = 0; i<t.maxelm + t.maxbound; i++) {
		if (i<t.maxelm) {
			// �������� � ��� ��� �������� �� ����������� ��� ������ � ������ ��������.
			integer ib = t.whot_is_block[i]; // ����� ����� �������� ����������� ����������� �����.

			//TOCHKA p; // ����� - ����� ���������������� ��.
			//integer ib; // ����� ����� �������� ����������� ����������� �����.
			integer iP = i;
			// ������ �� ���� ���������� ����������� ������� ��������� �������.
			//center_cord3D(iP, t.nvtx, t.pa, p); // ���������� ��������� ������ ��.
			//in_model_temp(p, ib, b, lb);
			if (tmaxreportblock[ib]<t.potent[iP]) {
				tmaxreportblock[ib] = t.potent[iP];
			}
		}
		else {
			// ��������� ����.
			integer inumber = i - t.maxelm;
			if (t.border_neighbor[inumber].MCB<(ls + lw)) {
				if (t.border_neighbor[inumber].MCB<ls) {
					if (tmaxreportsource[t.border_neighbor[inumber].MCB]<t.potent[i]) {
						tmaxreportsource[t.border_neighbor[inumber].MCB] = t.potent[i];
					}
				}
				else {
					if (tmaxreportwall[t.border_neighbor[inumber].MCB - ls]<t.potent[i]) {
						tmaxreportwall[t.border_neighbor[inumber].MCB - ls] = t.potent[i];
					}
				}
			}
		}
	}

	// ������ ����� ��������� ��� ��������� ����� �������� �������, �������
	// ����� ������������� ������������ ����������������� �����������.
	// ��� ��� ������ ����� ������� �� ������ ����� ��������� ��������� ��,
	// ����� ����������� � ��� ����������� ��� ������� ��������������.
	// ���� ������ ������ ��� ������� wall.

	bool bOksource = true, bOkwall = true;
	for (integer i = 0; i<ls; i++) {
		if (tmaxreportsource[i]<tmin + 1.0) {
			bOksource = false;
			break;
		}
	}
	for (integer i = 0; i<lw; i++) {
		if (tmaxreportwall[i]<tmin + 1.0) {
			bOkwall = false;
			break;
		}
	}

	if (bOksource&&bOkwall) {
		// ����������� ������ �������������, 
		// ����� �������� �����.

		// ���������� ������ ���������� � ���� 
		// ������� �����, ����� ���������, ����� ������.
		FILE *fp=NULL; // ���� � ������� ����� ������������ �������
		errno_t err;


		char name1[] = "report_temperature.txt";
		char name2[] = "solver/solid_static/report_temperature.txt";
		char name3[] = "solver/conjugate_heat_transfer_static/report_temperature.txt";

		char *name = nullptr;

		switch (ipref) {
		case 0: name = name1; break;
		case 1: name = name2; break;
		case 2: name = name3; break;
		default:
			printf("error in my_unsteady_temperature.c: report_temperature: name==nullptr\n");
			system("pause");
			exit(1);
			break;
		}

		/*
		char *name="report_temperature.txt";
		switch(ipref) {
		case 0: name="report_temperature.txt"; break;
		case 1: name="solver/solid_static/report_temperature.txt"; break;
		case 2: name="solver/conjugate_heat_transfer_static/report_temperature.txt"; break;
		}
		*/

#ifdef MINGW_COMPILLER
		err = 0;
		fp=fopen64(name, "a");
		if (fp == NULL) err = 1;
#else
		err = fopen_s(&fp, name, "a");
#endif

		

		if ((err) != 0) {
			printf("Create File report_temperature.txt Error\n");
			// getchar();
			name = nullptr;
			system("pause");
			exit(0);
		}
		else {

			if (fp != NULL) {

				name = nullptr;

				fprintf(fp, "object_name temperature, �C   power, W\n");
				for (integer i = 0; i < lb; i++) {
					doublereal Vol = fabs((b[i].g.xE - b[i].g.xS)*(b[i].g.yE - b[i].g.yS)*(b[i].g.zE - b[i].g.zS));
					if (i == 0) {
						// cabinet (�������).
						if (tmaxreportblock[0] < -1.0e26) {
							fprintf(fp, "%-30s \n", b[i].name);
						}
						else {
							pdiss += get_power(b[i].n_Sc, b[i].temp_Sc, b[i].arr_Sc, tmaxreportblock[i]) * (Vol);
							if (tmaxreportblock[i] < tmin1) tmin1 = tmaxreportblock[i];
							if (tmaxreportblock[i] > tmax1) tmax1 = tmaxreportblock[i];
							fprintf(fp, "%-30s %e %e\n", b[i].name, tmaxreportblock[i], get_power(b[i].n_Sc, b[i].temp_Sc, b[i].arr_Sc, tmaxreportblock[i])* (Vol));
						}
					}
					else {
						if (b[i].itype == HOLLOW) {
							fprintf(fp, "%-30s HOLLOW Type\n", b[i].name);
						}
						else {
							pdiss += get_power(b[i].n_Sc, b[i].temp_Sc, b[i].arr_Sc, tmaxreportblock[i]) * (Vol);
							if (tmaxreportblock[i] < tmin1) tmin1 = tmaxreportblock[i];
							if (tmaxreportblock[i] > tmax1) tmax1 = tmaxreportblock[i];
							//fprintf(fp, "%-30s %e %e\n", b[i].name, tmaxreportblock[i], b[i].Sc*(Vol));
							fprintf(fp, "%-30s %e %e\n", b[i].name, tmaxreportblock[i], get_power(b[i].n_Sc, b[i].temp_Sc, b[i].arr_Sc, tmaxreportblock[i]) * (Vol));
						}
					}
				}
				for (integer i = 0; i < ls; i++) {
					pdiss += s[i].power;
					if (tmaxreportsource[i] < tmin1) tmin1 = tmaxreportsource[i];
					if (tmaxreportsource[i] > tmax1) tmax1 = tmaxreportsource[i];
					fprintf(fp, "%-30s %e %e\n", s[i].name, tmaxreportsource[i], s[i].power);
				}
				for (integer i = 0; i < lw; i++) {
					if (tmaxreportwall[i] < tmin1) tmin1 = tmaxreportwall[i];
					if (tmaxreportwall[i] > tmax1) tmax1 = tmaxreportwall[i];
					fprintf(fp, "%-30s %e %e\n", w[i].name, tmaxreportwall[i], 0.0);
				}

				fprintf(fp, "\n\n Power dissipation=%e W\n", pdiss);
				fprintf(fp, "Minimum temperature in default\n interior is equal = %e �C\n", tmin1);
				fprintf(fp, "Maximum temperature in default\n interior is equal = %e �C\n", tmax1);

				doublereal massa=massa_cabinet(t, f,  flow_interior,
					b, lb, t.operatingtemperature,
					matlist);

				fprintf(fp, "\n\n massa = %e kg\n", massa);


				{

					fprintf(fp, "\n\n");

					integer* number_control_volume_on_wall = new integer[lw];
					doublereal* wall_power = new doublereal[lw];
					for (int iwall_scan = 0; iwall_scan < lw; iwall_scan++) {
						wall_power[iwall_scan] = 0.0;
						number_control_volume_on_wall[iwall_scan] = 0;
						for (integer j = 0; j < t.maxbound; j++) {

							if (t.border_neighbor[j].MCB == (ls + iwall_scan)) {

								number_control_volume_on_wall[iwall_scan]++;

								integer iP = t.border_neighbor[j].iI;// fglobal[0].maxelm + j;

								TOCHKA p;
								center_cord3D(iP, t.nvtx, t.pa, p, 100);
								doublereal dx = 0.0, dy = 0.0, dz = 0.0;
								volume3D(iP, t.nvtx, t.pa, dx, dy, dz);
								dx = fabs(dx);
								dy = fabs(dy);
								dz = fabs(dz);
								doublereal dx1 = 0.0, dy1 = 0.0, dz1 = 0.0;
								volume3D(t.border_neighbor[j].iII, t.nvtx, t.pa, dx1, dy1, dz1);
								dx1 = fabs(dx1);
								dy1 = fabs(dy1);
								dz1 = fabs(dz1);
								integer ib; // ����� �������� �����
								in_model_temp(p, ib, b, lb);

								doublereal lam = t.prop[LAM][iP]; // �������� �� ������������ �� ���������
								doublereal temperature_i = t.potent[iP]; // �� �� ����� ���� �������� ��������� � ����������� ���������� ����.
								doublereal temperature_ii = t.potent[t.border_neighbor[j].iII];
								doublereal temperature_w = t.potent[t.border_neighbor[j].iB];

								switch (w[iwall_scan].iPlane) {
								case XY_PLANE: if (t.border_neighbor[j].Norm == T_SIDE) {// ���, ���������� ������.
									//+ �������
									if (fabs((lam * (temperature_ii - temperature_i) * dx * dy) / (0.5 * (dz + dz1))) >
										fabs((lam * (temperature_i - temperature_w) * dx * dy) / (0.5 * (dz)))) {
										wall_power[iwall_scan] += (lam * (temperature_ii - temperature_i) * dx * dy) / (0.5 * (dz + dz1));
									}
									else {
										wall_power[iwall_scan] += (lam * (temperature_i - temperature_w) * dx * dy) / (0.5 * (dz));
									}
								}
											 if (t.border_neighbor[j].Norm == B_SIDE) {// ����, ���������� ������.
												 //+ �������
												 if (fabs((lam * (temperature_ii - temperature_i) * dx * dy) / (0.5 * (dz + dz1))) >
													 fabs((lam * (temperature_i - temperature_w) * dx * dy) / (0.5 * (dz)))) {
													 wall_power[iwall_scan] += (lam * (temperature_ii - temperature_i) * dx * dy) / (0.5 * (dz + dz1));
												 }
												 else {
													 wall_power[iwall_scan] += (lam * (temperature_i - temperature_w) * dx * dy) / (0.5 * (dz));
												 }
											 }
											 break;
								case XZ_PLANE:
									if (t.border_neighbor[j].Norm == N_SIDE) {// ��, ���������� ������.
										//+ �������
										if (fabs((lam * (temperature_ii - temperature_i) * dx * dz) / (0.5 * (dy + dy1))) >
											fabs((lam * (temperature_i - temperature_w) * dx * dz) / (0.5 * (dy)))) {
											wall_power[iwall_scan] += (lam * (temperature_ii - temperature_i) * dx * dz) / (0.5 * (dy + dy1));
										}
										else {
											wall_power[iwall_scan] += (lam * (temperature_i - temperature_w) * dx * dz) / (0.5 * (dy));
										}
									}
									if (t.border_neighbor[j].Norm == S_SIDE) {// �����, ���������� ������.
										//+ �������
										if (fabs((lam * (temperature_ii - temperature_i) * dx * dz) / (0.5 * (dy + dy1))) >
											fabs((lam * (temperature_i - temperature_w) * dx * dz) / (0.5 * (dy)))) {
											wall_power[iwall_scan] += (lam * (temperature_ii - temperature_i) * dx * dz) / (0.5 * (dy + dy1));
										}
										else {
											wall_power[iwall_scan] += (lam * (temperature_i - temperature_w) * dx * dz) / (0.5 * (dy));
										}
									}
									break;
								case YZ_PLANE:  if (t.border_neighbor[j].Norm == E_SIDE) {// �����, ���������� ������.
									//+ �������
									if (fabs((lam * (temperature_ii - temperature_i) * dy * dz) / (0.5 * (dx + dx1))) >
										fabs((lam * (temperature_i - temperature_w) * dy * dz) / (0.5 * (dx)))) {
										wall_power[iwall_scan] += (lam * (temperature_ii - temperature_i) * dy * dz) / (0.5 * (dx + dx1));
									}
									else {
										wall_power[iwall_scan] += (lam * (temperature_i - temperature_w) * dy * dz) / (0.5 * (dx));
									}
								}
											 if (t.border_neighbor[j].Norm == W_SIDE) {// ������, ���������� ������.
												 //+ �������
												 if (fabs((lam * (temperature_ii - temperature_i) * dy * dz) / (0.5 * (dx + dx1))) >
													 fabs((lam * (temperature_i - temperature_w) * dy * dz) / (0.5 * (dx)))) {
													 wall_power[iwall_scan] += (lam * (temperature_ii - temperature_i) * dy * dz) / (0.5 * (dx + dx1));
												 }
												 else {
													 wall_power[iwall_scan] += (lam * (temperature_i - temperature_w) * dy * dz) / (0.5 * (dx));
												 }
											 }
											 break;
								}
							}
						}
					}

					printf("\n");
					for (int iwall_scan = 0; iwall_scan < lw; iwall_scan++) {
						fprintf(fp, "wall[%d].name = %s power is %e W. Number control volume in wall=%lld\n", iwall_scan, w[iwall_scan].name, wall_power[iwall_scan], number_control_volume_on_wall[iwall_scan]);
					}

					delete[] number_control_volume_on_wall;
					delete[] wall_power;

					int idlw = 0;
					if (lw > 0) {
						for (int iwall_scan = 0; iwall_scan < lw; iwall_scan++) {
							if (w[iwall_scan].bpressure) idlw = 1;
							if (w[iwall_scan].bopening) idlw = 1;
						}
					}

					if (idlw > 0) {
						fprintf(fp, "\n");
						fprintf(fp, "\n        rashod m!3/s;  rashod kg/s;  Power out, W;  type wall out; delta Tavg_wall, �C; Tmax_wall, �C.\n");
					}

					doublereal Tamb0 = 1.0e30;// Tamb; // Tamb
					for (int iwall_scan = 0; iwall_scan < lw; iwall_scan++) {
						// ���������� ����������� �������� �����������.
						if ((!w[iwall_scan].bpressure) && (!w[iwall_scan].bsymmetry)) {
							if (w[iwall_scan].ifamily == DIRICHLET_FAMILY) {
								Tamb0 = fmin(Tamb0, w[iwall_scan].Tamb);
							}
						}
					}
					if (Tamb0 > 1.0e10) Tamb0 = fglobal[0].OpTemp;

					doublereal cp = 0.0;

					for (int iwall_scan = 0; iwall_scan < lw; iwall_scan++) {

						doublereal Tmax_wall = -1.0e30;

						//if (iwall_scan == 0) printf("\n");

						if (w[iwall_scan].bpressure) {
							doublereal rashod = 0.0; // m!3/s
							doublereal rashod2 = 0.0; // kg/s
													  // �������� � �� ������� �������� ����� �������� ������� ������.
													  // ������� ����� ����� �.�. ��������� ����� �������� ����������
													  // ���� ������ ������� ������ ������ ������� ������� �� �����������.
							doublereal Qout = 0.0; // ��
							for (integer j = 0; j < fglobal[0].maxbound; j++) {

								if (fglobal[0].border_neighbor[j].MCB == (ls + iwall_scan)) {

									integer iP = fglobal[0].border_neighbor[j].iI;// fglobal[0].maxelm + j;

									TOCHKA p;
									center_cord3D(iP, fglobal[0].nvtx, fglobal[0].pa, p, 100);
									integer ib; // ����� �������� �����
									in_model_flow(p, ib, b, lb);

									doublereal rho, mu, beta_t, lam; // �������� �� ������������ �� ���������
									doublereal pressure = fglobal[0].potent[PRESS][iP]; // �� �� ����� ���� �������� ��������� � ����������� ���������� ����.
									my_fluid_properties(t.potent[fglobal[0].ptr[iP]], pressure, rho, cp, lam, mu, beta_t, matlist[b[ib].imatid].ilibident);
									Tmax_wall = fmax(Tmax_wall, t.potent[fglobal[0].ptr[iP]]);

									// ��������!!! fglobal[0].prop_b[HEAT_CAPACITY][j] ������������ ������, �.�. ��� fluid ��� �� ����������.


									switch (w[iwall_scan].iPlane) {
									case XY_PLANE:
										// ������� ����������
										// ������: �� ��� �������� �� � ������.
										if (fglobal[0].border_neighbor[j].Norm == T_SIDE) {
											rashod += -fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VZCOR][fglobal[0].maxelm + j];
											rashod2 += -fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VZCOR][fglobal[0].maxelm + j];
											Qout += fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VZCOR][fglobal[0].maxelm + j] *
												cp * (t.potent[fglobal[0].ptr[fglobal[0].border_neighbor[j].iI]] - Tamb0);

										}
										else {
											rashod += fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VZCOR][fglobal[0].maxelm + j];
											rashod2 += fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VZCOR][fglobal[0].maxelm + j];
											Qout += -fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VZCOR][fglobal[0].maxelm + j] *
												cp * (t.potent[fglobal[0].ptr[fglobal[0].border_neighbor[j].iI]] - Tamb0);
										}
										break;
									case XZ_PLANE:
										// ������� ����������
										// ������: �� ��� �������� �� � ������.
										if (fglobal[0].border_neighbor[j].Norm == N_SIDE) {
											rashod += -fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VYCOR][fglobal[0].maxelm + j];
											rashod2 += -fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VYCOR][fglobal[0].maxelm + j];
											Qout += fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VYCOR][fglobal[0].maxelm + j] *
												cp * (t.potent[fglobal[0].ptr[fglobal[0].border_neighbor[j].iI]] - Tamb0);
										}
										else {
											rashod += fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VYCOR][fglobal[0].maxelm + j];
											rashod2 += fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VYCOR][fglobal[0].maxelm + j];
											Qout += -fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VYCOR][fglobal[0].maxelm + j] *
												cp * (t.potent[fglobal[0].ptr[fglobal[0].border_neighbor[j].iI]] - Tamb0);
										}
										break;
									case YZ_PLANE:
										// ������� ����������
										// ������: �� ��� �������� �� � ������.
										if (fglobal[0].border_neighbor[j].Norm == E_SIDE) {
											rashod += -fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VXCOR][fglobal[0].maxelm + j];
											rashod2 += -fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VXCOR][fglobal[0].maxelm + j];
											Qout += fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VXCOR][fglobal[0].maxelm + j] *
												cp * (t.potent[fglobal[0].ptr[fglobal[0].border_neighbor[j].iI]] - Tamb0);

										}
										else {
											rashod += fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VXCOR][fglobal[0].maxelm + j];
											rashod2 += fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VXCOR][fglobal[0].maxelm + j];
											Qout += -fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VXCOR][fglobal[0].maxelm + j] *
												cp * (t.potent[fglobal[0].ptr[fglobal[0].border_neighbor[j].iI]] - Tamb0);
										}
										break;
									}
								}
							}

							//printf("wall[%lld] out of boundary (bpressure==true).\n", iwall_scan);
							//printf("rashod = %e m!3/s; rashod = %e kg/s; Power out = %e W, Tavg_wall= %e, Tmax_wall= %e. \n", rashod, rashod2, Qout, Qout / (cp*rashod2), Tmax_wall);
							if (rashod > 0) {
								fprintf(fp, "wall[%d]  %e  %e %e bpressure %e %e\n", iwall_scan, rashod, rashod2, Qout, fabs(Qout / (cp * rashod2)), Tmax_wall);
							}
							else {
								fprintf(fp, "wall[%d] %e %e  %e bpressure %e %e\n", iwall_scan, rashod, rashod2, Qout, fabs(Qout / (cp * rashod2)), Tmax_wall);
							}
						}
						if (w[iwall_scan].bopening) {
							doublereal rashod = 0.0; // m!3/s
							doublereal rashod2 = 0.0; // kg/s
													  // �������� � �� ������� �������� ����� �������� ������� ������.
													  // ������� ����� ����� �.�. ��������� ����� �������� ����������
													  // ���� ������ ������� ������ ������ ������� ������� �� �����������.
							doublereal Qout = 0.0; // ��
							for (integer j = 0; j < fglobal[0].maxbound; j++) {

								if (fglobal[0].border_neighbor[j].MCB == (ls + iwall_scan)) {

									integer iP = fglobal[0].border_neighbor[j].iI;// fglobal[0].maxelm + j;



									TOCHKA p;
									center_cord3D(iP, fglobal[0].nvtx, fglobal[0].pa, p, 100);
									integer ib; // ����� �������� �����
									in_model_flow(p, ib, b, lb);

									doublereal rho, mu, beta_t, lam; // �������� �� ������������ �� ���������
									doublereal pressure = fglobal[0].potent[PRESS][iP]; // �� �� ����� ���� �������� ��������� � ����������� ���������� ����.
									my_fluid_properties(t.potent[fglobal[0].ptr[iP]], pressure, rho, cp, lam, mu, beta_t, matlist[b[ib].imatid].ilibident);
									Tmax_wall = fmax(Tmax_wall, t.potent[fglobal[0].ptr[iP]]);

									// 1 - minx, 2-maxx; 3 - miny, 4-maxy; 5 - minz, 6-maxz;
									//if ((iwall_scan == 2)||(iwall_scan == 5)) {
										//printf("iP=%ld rho=%e cp=%e ptr=%d T=%e iPlane=%d Norm=%d\n", iP,rho,cp, fglobal[0].ptr[iP], t.potent[fglobal[0].ptr[iP]], w[iwall_scan].iPlane, fglobal[0].border_neighbor[j].Norm);
										//printf("xE=%e xS=%e\n", w[iwall_scan].g.xE, w[iwall_scan].g.xS);
										//getchar();
									//}

									// ��������!!! fglobal[0].prop_b[HEAT_CAPACITY][j] ������������ ������, �.�. ��� fluid ��� �� ����������.

									//printf("HEAT_CAPACITY=%e\n", cp); // debug
									//system("pause"); // debug

									switch (w[iwall_scan].iPlane) {
									case XY_PLANE:
										// ������� ����������
										// ������: �� ��� �������� �� � ������.
										if (fglobal[0].border_neighbor[j].Norm == T_SIDE) {
											rashod += -fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VZCOR][fglobal[0].maxelm + j];
											rashod2 += -fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VZCOR][fglobal[0].maxelm + j];
											Qout += fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VZCOR][fglobal[0].maxelm + j] *
												cp * (t.potent[fglobal[0].ptr[fglobal[0].border_neighbor[j].iI]] - Tamb0);

										}
										else {
											//printf("VZ=%e dS=%e BSIDE internal normal TOP boundary\n", fglobal[0].potent[VZCOR][fglobal[0].maxelm + j], fglobal[0].border_neighbor[j].dS);
											//if (j % 10 == 0) system("pause");
											rashod += fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VZCOR][fglobal[0].maxelm + j];
											rashod2 += fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VZCOR][fglobal[0].maxelm + j];
											Qout += -fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VZCOR][fglobal[0].maxelm + j] *
												cp * (t.potent[fglobal[0].ptr[fglobal[0].border_neighbor[j].iI]] - Tamb0);
										}
										break;
									case XZ_PLANE:
										// ������� ����������
										// ������: �� ��� �������� �� � ������.
										if (fglobal[0].border_neighbor[j].Norm == N_SIDE) {
											rashod += -fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VYCOR][fglobal[0].maxelm + j];
											rashod2 += -fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VYCOR][fglobal[0].maxelm + j];
											Qout += fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VYCOR][fglobal[0].maxelm + j] *
												cp * (t.potent[fglobal[0].ptr[fglobal[0].border_neighbor[j].iI]] - Tamb0);
										}
										else {
											rashod += fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VYCOR][fglobal[0].maxelm + j];
											rashod2 += fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VYCOR][fglobal[0].maxelm + j];
											Qout += -fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VYCOR][fglobal[0].maxelm + j] *
												cp * (t.potent[fglobal[0].ptr[fglobal[0].border_neighbor[j].iI]] - Tamb0);
										}
										break;
									case YZ_PLANE:
										// ������� ����������
										// ������: �� ��� �������� �� � ������.
										if (fglobal[0].border_neighbor[j].Norm == E_SIDE) {
											rashod += -fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VXCOR][fglobal[0].maxelm + j];
											rashod2 += -fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VXCOR][fglobal[0].maxelm + j];
											Qout += fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VXCOR][fglobal[0].maxelm + j] *
												cp * (t.potent[fglobal[0].ptr[fglobal[0].border_neighbor[j].iI]] - Tamb0);

										}
										else {
											rashod += fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VXCOR][fglobal[0].maxelm + j];
											rashod2 += fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VXCOR][fglobal[0].maxelm + j];
											Qout += -fglobal[0].prop_b[RHO][j] * fglobal[0].border_neighbor[j].dS * fglobal[0].potent[VXCOR][fglobal[0].maxelm + j] *
												cp * (t.potent[fglobal[0].ptr[fglobal[0].border_neighbor[j].iI]] - Tamb0);
										}
										break;
									}
								}
							}

							//printf("wall[%lld] out of boundary (bopening==true).\n", iwall_scan);
							//printf("rashod = %e m!3/s; rashod = %e kg/s; Power out = %e W, Tavg_wall= %e, Tmax_wall= %e. \n", rashod, rashod2, Qout, Qout / (cp*rashod2), Tmax_wall);
							if (rashod > 0) {
								fprintf(fp, "wall[%d]  %e  %e %e bopening  %e %e\n", iwall_scan, rashod, rashod2, Qout, fabs(Qout / (cp * rashod2)), Tmax_wall);
							}
							else {
								fprintf(fp, "wall[%d] %e %e  %e bopening  %e %e\n", iwall_scan, rashod, rashod2, Qout, fabs(Qout / (cp * rashod2)), Tmax_wall);
							}
						}
					}
					if (idlw > 0) {
						printf("\n");
					}

				}

				fclose(fp);
			}
		}

		if (tmaxreportblock != nullptr) {
			delete[] tmaxreportblock;
			tmaxreportblock = nullptr;
		}

		if (tmaxreportsource != nullptr) {
			delete[] tmaxreportsource;
			tmaxreportsource = nullptr;
		}

		if (tmaxreportwall != nullptr) {
			delete[] tmaxreportwall;
			tmaxreportwall = nullptr;
		}

	}
	else {

		// ���� ������ ����� ��������� ��� ������� ��� ������� ��� ��������
		// ������� ������ �������� � �������� �������� � ���� ������.
		// ��� ������, ����� ��������� �� ����� ��������� �������� �� �������� ������� � ���������� ����� �������.
		// ���������� ������������� ���� ������ ����� �������� ���������.
		printf("Indetify problem in report_temperature in my_unsteady_temperature.c module...\n");
		printf("Error! Geometry is failed. Please, press any key to exit...");
		//getchar();
		system("pause");
		exit(0); // ����� �� ���������.
	}

} // report_temperature

// ������������� � ������ ��������� ������ (����������������).
// �����: ������������� �������� �������� ������ � ����� �����������,
// ����� �������� ����������������.
void debug_signal(TEMPER& t, doublereal operating_temperature) {
	bool debug_reshime = true;

	for (integer i = 0; i < t.maxelm + t.maxbound; i++) {
		if (i < t.maxelm) {
			// �������� � ��� ��� �������� �� ����������� ��� ������ � ������ ��������.
			integer ib = t.whot_is_block[i]; // ����� ����� �������� ����������� ����������� �����.

			//TOCHKA p; // ����� - ����� ���������������� ��.
			//integer ib; // ����� ����� �������� ����������� ����������� �����.
			integer iP = i;
			// ������ �� ���� ���������� ����������� ������� ��������� �������.
			//center_cord3D(iP, t.nvtx, t.pa, p); // ���������� ��������� ������ ��.
			//in_model_temp(p, ib, b, lb);

			if (debug_reshime) {
				if (t.potent[iP] < 0.9 * operating_temperature) {
					printf("Error block number %lld temperature = %e < Tamb=%e\n", ib, t.potent[iP], operating_temperature);
					TOCHKA pbug;
					center_cord3D(iP, t.nvtx, t.pa, pbug, 100); // ���������� ��������� ������ ��.
					printf("geometry location x=%e y=%e z=%e\n", pbug.x, pbug.y, pbug.z);
					printf("control volume = %lld\n", iP);
					printf("t.Sc[%lld]=%e\n", iP, t.Sc[iP]);
					printf("t.slau[%lld].b=%e\n", iP, t.slau[iP].b);
					system("pause");
				}
			}
		}
	}
}


// 2 ������ 2016 �������� ������������� ��� �������������� ������� �����
// ��������� ������� ���� �� ������� ���������� ���� � �������.
// ��������: ������������������ ��� ������ �� ������� ������� ����������� 
// ������ ������������ ������ ���������� AliceMesh* ������� ��� ����������� ������������ 
// ������ �������������� � ����������� ������ ����������.
// �������� ����� ����� ���������� � ��������� ���� 
// report_temperature.txt
// ������������ ����������� ������� �����, �������� �������������� � ��� � ������ ������ �������,
// ������������ ����������� ������� ���������, �������� �������������� � ��� � ������ ������ �������,
// ������������ ����������� ������ ������.
void report_temperature_for_unsteady_modeling(integer flow_interior,
	FLOW* &fglobal, TEMPER &t,
	BLOCK* b, integer lb, SOURCE* s, integer ls,
	WALL* w, integer lw, integer ipref, doublereal time_solution_now, 
	doublereal  poweron_multiplier_sequence_out,
	doublereal operating_temperature) {

	bool debug_reshime = false; // ������ false �.�. � ����� ������� ������ �� ��� t.Sc  �����������.

	// ��� �������������� ������� ���������� time_solution_now 
	// ���������� ����� (���������) �� ������� ��� �� �������.
	// ��������� ������:
	// report_temperature_for_unsteady_modeling(flow_interior, f, t, b, lb, s, ls, w, lw, 0, time_solution_now, poweron_multiplier_sequence);
	// ���������� ������ �� �������: unsteady_temperature_calculation.

	doublereal* tmaxreportblock = nullptr;
	tmaxreportblock = new doublereal[lb];
	if (tmaxreportblock == nullptr) {
		// ������������ ������ �� ������ ������������.
		printf("Problem: not enough memory on your equipment for tmaxreportblock report_temperature_for_unsteady_modeling...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	doublereal* tmaxreportsource = nullptr;
	tmaxreportsource = new doublereal[ls];
	if (tmaxreportsource == nullptr) {
		// ������������ ������ �� ������ ������������.
		printf("Problem: not enough memory on your equipment for tmaxreportsource report_temperature_for_unsteady_modeling...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	doublereal* tmaxreportwall = nullptr;
	tmaxreportwall = new doublereal[lw];
	if (tmaxreportwall == nullptr) {
		// ������������ ������ �� ������ ������������.
		printf("Problem: not enough memory on your equipment for tmaxreportwall report_temperature_for_unsteady_modeling...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}

	const doublereal tmin = -1.0e27;

	// �������������.
	for (integer i = 0; i<lb; i++) {
		tmaxreportblock[i] = tmin;
	}
	for (integer i = 0; i<ls; i++) {
		tmaxreportsource[i] = tmin;
	}
	for (integer i = 0; i<lw; i++) {
		tmaxreportwall[i] = tmin;
	}

	/*
	// �������� �� ���� �� ������� ���������.
	for (integer i=0; i<t.maxelm+t.maxbound; i++) {
	if (i<t.maxelm) {
	TOCHKA p; // ����� - ����� ���������������� ��.
	integer ib; // ����� ����� �������� ����������� ����������� �����.
	integer iP=i;
	// ������ �� ���� ���������� ����������� ������� ��������� �������.
	center_cord3D(iP, t.nvtx, t.pa, p); // ���������� ��������� ������ ��.
	in_model_temp(p,ib,b,lb);
	if (tmaxreportblock[ib]<t.potent[iP]) {
	tmaxreportblock[ib]=t.potent[iP];
	}
	}
	else {
	// ��������� ����.
	integer inumber=i-t.maxelm;
	if (t.border_neighbor[inumber].MCB<(ls+lw)) {
	if (t.border_neighbor[inumber].MCB<ls) {
	if (tmaxreportsource[t.border_neighbor[inumber].MCB]<t.potent[i]) {
	tmaxreportsource[t.border_neighbor[inumber].MCB]=t.potent[i];
	}
	}
	else {
	if (tmaxreportwall[t.border_neighbor[inumber].MCB-ls]<t.potent[i]) {
	tmaxreportwall[t.border_neighbor[inumber].MCB-ls]=t.potent[i];
	}
	}
	}
	}
	}
	*/
	// 8 ������ 2016 ������� ����� ������� ������� �� ��������������.
	// �������� �� ���� �� ������� ���������.
	for (integer i = 0; i<t.maxelm + t.maxbound; i++) {
		if (i<t.maxelm) {
			// �������� � ��� ��� �������� �� ����������� ��� ������ � ������ ��������.
			integer ib = t.whot_is_block[i]; // ����� ����� �������� ����������� ����������� �����.

			//TOCHKA p; // ����� - ����� ���������������� ��.
			//integer ib; // ����� ����� �������� ����������� ����������� �����.
			integer iP = i;
			// ������ �� ���� ���������� ����������� ������� ��������� �������.
			//center_cord3D(iP, t.nvtx, t.pa, p); // ���������� ��������� ������ ��.
			//in_model_temp(p, ib, b, lb);

			if (debug_reshime) {
				if (t.potent[iP] < 0.9*operating_temperature) {
					printf("Error block number %lld temperature = %e < Tamb=%e\n",ib, t.potent[iP], operating_temperature);
					TOCHKA pbug;
					center_cord3D(iP, t.nvtx, t.pa, pbug,100); // ���������� ��������� ������ ��.
					printf("geometry location x=%e y=%e z=%e\n",pbug.x,pbug.y,pbug.z);
					printf("control volume = %lld\n",iP);
					printf("t.Sc[%lld]=%e\n", iP, t.Sc[iP]);
					printf("t.slau[%lld].b=%e\n", iP, t.slau[iP].b);
					system("pause");
				}
			}

			if (tmaxreportblock[ib]<t.potent[iP]) {
				tmaxreportblock[ib] = t.potent[iP];
			}
		}
		else {
			// ��������� ����.
			integer inumber = i - t.maxelm;
			if (t.border_neighbor[inumber].MCB<(ls + lw)) {
				if (t.border_neighbor[inumber].MCB<ls) {
					if (tmaxreportsource[t.border_neighbor[inumber].MCB]<t.potent[i]) {
						tmaxreportsource[t.border_neighbor[inumber].MCB] = t.potent[i];
					}
				}
				else {
					if (tmaxreportwall[t.border_neighbor[inumber].MCB - ls]<t.potent[i]) {
						tmaxreportwall[t.border_neighbor[inumber].MCB - ls] = t.potent[i];
					}
				}
			}
		}
	}

	// ������ ����� ��������� ��� ��������� ����� �������� �������, �������
	// ����� ������������� ������������ ����������������� �����������.
	// ��� ��� ������ ����� ������� �� ������ ����� ��������� ��������� ��,
	// ����� ����������� � ��� ����������� ��� ������� ��������������.
	// ���� ������ ������ ��� ������� wall.

	bool bOksource = true, bOkwall = true;
	for (integer i = 0; i<ls; i++) {
		if (tmaxreportsource[i]<tmin + 1.0) {
			bOksource = false;
			break;
		}
	}
	for (integer i = 0; i<lw; i++) {
		if (tmaxreportwall[i]<tmin + 1.0) {
			bOkwall = false;
			break;
		}
	}

	if (bOksource&&bOkwall) {
		// ����������� ������ �������������, 
		// ����� �������� �����.

		// ���������� ������ ���������� � ���� 
		// ������� �����, ����� ���������, ����� ������.
		FILE *fp=NULL; // ���� � ������� ����� ������������ �������
		errno_t err;


		/*
		char name1[] = "report_temperature.txt";
		char name2[] = "solver/solid_static/report_temperature.txt";
		char name3[] = "solver/conjugate_heat_transfer_static/report_temperature.txt";

		char *name = nullptr;

		switch (ipref) {
		case 0: name = name1; break;
		case 1: name = name2; break;
		case 2: name = name3; break;
		default:
		printf("error in my_unsteady_temperature.c: report_temperature: name==nullptr\n");
		system("pause");
		exit(1);
		break;
		}
		*/


		/*
		char *name="report_temperature.txt";
		switch(ipref) {
		case 0: name="report_temperature.txt"; break;
		case 1: name="solver/solid_static/report_temperature.txt"; break;
		case 2: name="solver/conjugate_heat_transfer_static/report_temperature.txt"; break;
		}
		*/

		// � ����� report_temperature_unsteady.txt ����������� � ������������� ������ ���������� 
		// � ������������ ���� �������� �� ������� ������� ������ � �������� ��������������� �������������.

		// �������������� ��������� ������ ����� � �������:
		// time    block1_name block1_name  block2_name block2_name block3_name block3_name ....
		// time1_s block1_tC block1_Power block2_tC block2_Power block3_tC block3_Power  ....
		// time2_s block1_tC block1_Power block2_tC block2_Power block3_tC block3_Power  ....
		// time3_s ....

		// ��� ���� ������ ������ ��������� ����������� ������ ������ ���������� AliceMesh_v0_39.
#ifdef MINGW_COMPILLER
		err = 0;
		fp=fopen64("report_temperature_unsteady.txt", "a");
		if (fp == NULL) err = 1;
#else
		err = fopen_s(&fp, "report_temperature_unsteady.txt", "a");
#endif
		

		if ((err) != 0) {
			printf("Create File report_temperature_unsteady.txt Error\n");
			// getchar();
			//name = nullptr;
			// 3.09.2019 ������ ������. �� ������ �� ��������� ��� ������� ��� ��������� �������� �����.
			//system("pause");
			//exit(0);
		}
		else {

			if (fp != NULL) {

				//name = nullptr;

				// ������ �������� ���������� ������� �� ������ ��� �� �������.
				fprintf(fp, "%e ", time_solution_now);

				// ������ ���� ����������� �����.
				//fprintf(fp, "temperature, �C   power, W\n");
				for (integer i = 0; i < lb; i++) {
					doublereal Vol = fabs((b[i].g.xE - b[i].g.xS)*(b[i].g.yE - b[i].g.yS)*(b[i].g.zE - b[i].g.zS));
					//fprintf(fp, "%e %e ", tmaxreportblock[i], b[i].Sc*(Vol));
					doublereal  poweron_multiplier_sequence = poweron_multiplier_sequence_out;
					if (b[i].ipower_time_depend == 0) {
						// �������� �������������� �� ������� �� �������.
						poweron_multiplier_sequence = 1.0;
					}
					if (b[i].ipower_time_depend == 1) {
						// square wave
						if (poweron_multiplier_sequence > 0.0) {
							poweron_multiplier_sequence = 1.0;
						}
					}
					fprintf(fp, "%e %e ", tmaxreportblock[i], poweron_multiplier_sequence*get_power(b[i].n_Sc, b[i].temp_Sc, b[i].arr_Sc, tmaxreportblock[i])*(Vol));
					

				}
				for (integer i = 0; i < ls; i++) {
					doublereal  poweron_multiplier_sequence = poweron_multiplier_sequence_out;
					fprintf(fp, "%e %e ", tmaxreportsource[i], poweron_multiplier_sequence*s[i].power);
				}
				for (integer i = 0; i < lw; i++) {
					fprintf(fp, "%e %e ", tmaxreportwall[i], 0.0);
				}
				fprintf(fp, "\n");

				fclose(fp);

				

			}
		}

		if (tmaxreportblock != nullptr) {
			delete[] tmaxreportblock;
			tmaxreportblock = nullptr;
		}

		if (tmaxreportsource != nullptr) {
			delete[] tmaxreportsource;
			tmaxreportsource = nullptr;
		}

		if (tmaxreportwall != nullptr) {
			delete[] tmaxreportwall;
			tmaxreportwall = nullptr;
		}

	}
	else {

		// ���� ������ ����� ��������� ��� ������� ��� ������� ��� ��������
		// ������� ������ �������� � �������� �������� � ���� ������.
		// ��� ������, ����� ��������� �� ����� ��������� �������� �� �������� ������� � ���������� ����� �������.
		// ���������� ������������� ���� ������ ����� �������� ���������.

		if (tmaxreportblock != nullptr) {
			delete[] tmaxreportblock;
			tmaxreportblock = nullptr;
		}

		if (tmaxreportsource != nullptr) {
			delete[] tmaxreportsource;
			tmaxreportsource = nullptr;
		}

		if (tmaxreportwall != nullptr) {
			delete[] tmaxreportwall;
			tmaxreportwall = nullptr;
		}

		printf("Indetify problem in report_temperature_for_unsteady_modeling in my_unsteady_temperature.c module...\n");
		printf("Error! Geometry is failed. Please, press any key to exit...\n");
		//getchar();
		system("pause");
		exit(0); // ����� �� ���������.
	}

} // report_temperature_for_unsteady_modeling


// ��� ��������� ������� ����� ������������� �� �������
// ����� �������� ���������� ��� �� �������.
// ���� ������������ ��������� �������� �������
// ������������������ ����� �� �������.

// ���������� ��� �� �������
void uniform_timestep_seq(doublereal StartTime, doublereal EndTime, doublereal time_step_increment,
	                      integer &iN, doublereal* &timestep_sequence, doublereal* &poweron_multiplier_sequence) {
		doublereal continuance=EndTime-StartTime;
		if ((continuance>0.0)&&(time_step_increment>0.0)) {
			integer i=0;
			doublereal time=StartTime+time_step_increment;
			while (time < EndTime) {
				i++;
				time+=time_step_increment;
			}
			iN=i+1; // �������� �� ���� ��� ����� �� ������� EndTime
			timestep_sequence=new doublereal[iN];
			poweron_multiplier_sequence=new doublereal[iN];
			for (i=0; i<iN; i++) {
               timestep_sequence[i]=time_step_increment;
			   poweron_multiplier_sequence[i]=1.0; // �������� ��������� ��������.
			}
		}
		else {
			// ��������� ������ ������� � ������� �� �����
			iN=0;
		}
} // uniform_timestep_seq

// �������� ��������� ���� �� ������� � ������������
// � ��������: Dt=Dt0+a*time;
// ���� ����� ���������� � ANSYS icepak � ����� ��������
// ��� ������� ������ ��������. �� ����� ������ ����� ��������
// �� ������� ��������� ������ �������� ��� ������ �����������.
void linear_timestep_seq(doublereal StartTime, doublereal EndTime, doublereal initial_time_step, doublereal Factor_a,
	                      integer &iN, doublereal* &timestep_sequence, doublereal* &poweron_multiplier_sequence) 
{
	doublereal continuance=EndTime-StartTime;
	if ((continuance>0.0)&&(initial_time_step>0.0)) {
         integer i=0;
		 doublereal time=StartTime; // ����� ������ ���������� ����
		 while (time < EndTime) {
			i++;
			time+=initial_time_step+Factor_a*time; // ����� ������ ���������� ����
		 }
		 iN=i; // �������� �� ���� ��� ����� �� ������� EndTime
		 timestep_sequence=new doublereal[iN];
		 poweron_multiplier_sequence=new doublereal[iN];
		 time=StartTime;
		 for (i=0; i<iN; i++) {
			 timestep_sequence[i]=initial_time_step+Factor_a*time;
             time+=initial_time_step+Factor_a*time; // ����� ������ ���������� ����
			 poweron_multiplier_sequence[i]=1.0; // �������� ��������� ��������.
		 }
	}
	else {
		// ��������� ������ ������� � ������� �� �����
		iN=0;
	}

} // linear_timestep_seq

// �������� ��������� ���� �� ������� � ������������
// � ��������: Dt=Dt0+a*time;
// ���� ����� ���������� � ANSYS icepak � ����� ��������
// ��� ������� ������ ��������. �� ����� ������ ����� ��������
// �� ������� ��������� ������ �������� ��� ������ �����������.
// ������ �������� ������ �����������, � ������� ����� ��������� ������� ��������� �����-�� �����, ��� � ������� �������.
void linear_timestep_seq2(doublereal StartTime, doublereal EndTime, doublereal initial_time_step, doublereal Factor_a,
	                      integer &iN, doublereal* &timestep_sequence, doublereal* &poweron_multiplier_sequence) 
{
	doublereal continuance=EndTime-StartTime;
	if ((continuance>0.0)&&(initial_time_step>0.0)) {
         integer i=0;
		 doublereal time=StartTime; // ����� ������ ���������� ����
		 while (time < EndTime) {
			i++;
			time+=initial_time_step+Factor_a*time; // ����� ������ ���������� ����
		 }
		 integer iN2=i;
		 iN=2*i; // �������� �� ���� ��� ����� �� ������� EndTime
		 timestep_sequence=new doublereal[iN];
		 poweron_multiplier_sequence=new doublereal[iN];
		 time=StartTime;
		 for (i=0; i<iN2; i++) {
			 timestep_sequence[i]=initial_time_step+Factor_a*time;
			 timestep_sequence[i+iN2]=initial_time_step+Factor_a*time;
             time+=initial_time_step+Factor_a*time; // ����� ������ ���������� ����
			 poweron_multiplier_sequence[i]=1.0; // �������� ��������� ��������.
			 poweron_multiplier_sequence[i+iN2]=0.0; // �������� �� �������
		 }
	}
	else {
		// ��������� ������ ������� � ������� �� �����
		iN=0;
	}

} // linear_timestep_seq2


// �������� ��������� ���� �� ������� � ������������
// � ��������: Dt=Dt0+a*time;
// ���� ����� ���������� � ANSYS icepak � ����� ��������
// ��� ������� ������ ��������. �� ����� ������ ����� ��������
// �� ������� ��������� ������ �������� ��� ������ �����������.
// hot cold �����. ������� ������ �� ������� ������� onTimeWidth
// ��� ���� ��� �� ������� �������� � ������������ � ��������������� �������,
// ����� ��������� � ������� ������� EndTime-StartTime- onTimeWidth � ���� 
// �� ������� ����� �������� � ������������ � ��������������� �������.
void linear_timestep_seq_hot_cold(doublereal StartTime, doublereal EndTime, doublereal initial_time_step, doublereal Factor_a,
	integer &iN, doublereal* &timestep_sequence, doublereal* &poweron_multiplier_sequence, doublereal onTimeWidth)
{
	doublereal continuance = EndTime - StartTime;
	if ((continuance>0.0) && (initial_time_step>0.0)) {
		integer i = 0;
		doublereal time = StartTime; // ����� ������ ���������� ����
		while (time < StartTime+ onTimeWidth) {
			i++;
			time += initial_time_step + Factor_a*time; // ����� ������ ���������� ����
		}
		time = StartTime + onTimeWidth;
		while (time < EndTime) {
			i++;
			time += initial_time_step + Factor_a*(time- onTimeWidth); // ����� ������ ���������� ����
		}
		iN = i; // �������� �� ���� ��� ����� �� ������� EndTime
		timestep_sequence = new doublereal[iN];
		poweron_multiplier_sequence = new doublereal[iN];
		time = StartTime;
		bool b1 = true;
		doublereal oldTime = 0.0;
		for (i = 0; i<=iN; i++) {
			if (time < StartTime + onTimeWidth) {
				timestep_sequence[i] = initial_time_step + Factor_a*time;
				oldTime = time;
				time += initial_time_step + Factor_a*time; // ����� ������ ���������� ����
				poweron_multiplier_sequence[i] = 1.0; // �������� ��������� ��������.
			}
			else {
				if (b1 && (time < EndTime))
				{
					b1 = false;
					timestep_sequence[i-1] = StartTime + onTimeWidth-oldTime;
					time = StartTime + onTimeWidth; // ����� ������ ���������� ����
					poweron_multiplier_sequence[i-1] = 1.0; // �������� ��������� ��������.
				}
				else {
					timestep_sequence[i-1] = initial_time_step + Factor_a*(time- onTimeWidth);
					time += initial_time_step + Factor_a*(time- onTimeWidth); // ����� ������ ���������� ����
					poweron_multiplier_sequence[i-1] = 0.0; // �������� ��������� ��������.
				}
			}
		}
	}
	else {
		// ��������� ������ ������� � ������� �� �����
		iN = 0;
	}

} // linear_timestep_seq_hot_cold



// Square Wave Time-Step Parameters
void square_wave_seq(doublereal StartTime, doublereal EndTime, doublereal phase_delay_time, doublereal tmax_value_of_time_step, 
	                 doublereal tmin_value_of_time_step, doublereal duration_of_tmax, doublereal duration_of_tmin,
	                 integer &iN, doublereal* &timestep_sequence, doublereal* &poweron_multiplier_sequence)
{
	// ������������ ���������:
	// StartTime - ����� ������ �����, EndTime - ����� ����� �������,
	// ���� ����� ��������� ����� �� ������� �������� ���������� ��� ����������� �������������� ������.
	// phase_delay_time - �������� ����� ������� ������ ��������, � �����
	// � ������� phase_delay_time - ����� ���������� ������� �������� P����/Q, Q - ����������.
	// tmax_value_of_time_step - �������� ����������� ���� �� ������� � ������ ������ ��������, 
	// tmin_value_of_time_step - �������� ����������� ���� �� ������� � ������ �������� � �������� ����� ������ ������� ��������,
	// duration_of_tmax - ����� � ������� �������� ������� �������� �� ����� �������,
	// duration_of_tmin - ����� � ������� �������� �������� �� ������� � ������ �������� �� ����� �������.

	duration_of_tmax=fabs(duration_of_tmax);
    duration_of_tmin=fabs(duration_of_tmin);

    doublereal continuance=EndTime-StartTime;
	if (continuance>0.0) {
         integer i=0;
		 doublereal time=StartTime; // ��������� �����
		 // ��������� �������� phase_delay_time:
		 if (phase_delay_time>continuance) phase_delay_time=continuance; // ����� �������� ��������� �� ����� �����.
		 if (phase_delay_time>tmin_value_of_time_step) {
			 while (time <= StartTime+phase_delay_time) {
				 time+=tmin_value_of_time_step;
				 i++;
			 }
			 time-=tmin_value_of_time_step;
			 doublereal Dt=fabs((StartTime+phase_delay_time)-time); // �������� ���������� ����
			 if (Dt<1e-37) i--; // � phase_delay_time ��������� ����� ����� ����� ������ tmin_value_of_time_step
		 }
		 else {
			 if (fabs(phase_delay_time)>1e-37) {
				 i=1;
				 // Dt==fabs(phase_delay_time);
			 }
		 }
		 time=StartTime+phase_delay_time; // ��������� ����� ���������� �����

		 bool bweshouldbecontinue=true; // ���������� ��� ����� ���������.
		 integer inumber_period=1; // ����� �������� �������.
		 while (bweshouldbecontinue) {
			 
		     if (duration_of_tmax>continuance-phase_delay_time-(inumber_period-1)*(duration_of_tmax+duration_of_tmin)) {
			    duration_of_tmax=continuance-phase_delay_time-(inumber_period-1)*(duration_of_tmax+duration_of_tmin);
				bweshouldbecontinue=false;
		     }

			 if (duration_of_tmax>tmax_value_of_time_step) {
			     while (time <= StartTime+phase_delay_time+(inumber_period-1)*(duration_of_tmax+duration_of_tmin)+duration_of_tmax) {
				     time+=tmax_value_of_time_step;
				     i++;
			     }
			     time-=tmax_value_of_time_step;
			     doublereal Dt=fabs((StartTime+phase_delay_time+(inumber_period-1)*(duration_of_tmax+duration_of_tmin)+duration_of_tmax)-time); // �������� ���������� ����
			     if (Dt<1e-37) i--; // � duration_of_tmax ��������� ����� ����� ����� ������ tmax_value_of_time_step
		    }
		    else {
			     if (fabs(duration_of_tmax)>1e-37) {
				    i++;
				 // Dt==fabs(duration_of_tmax);
			     }
		    }
		    time=StartTime+phase_delay_time+(inumber_period-1)*(duration_of_tmax+duration_of_tmin)+duration_of_tmax; // ��������� ����� ���������� �����


            inumber_period++; // ��������� � ���������� �������.
		 }

		 iN=i; // �������� �� ���� ��� ����� �� ������� EndTime


		 timestep_sequence=new doublereal[iN];
		 poweron_multiplier_sequence=new doublereal[iN];
		 time=StartTime;
		 
	}
	else {
		// ��������� ������ ������� � ������� �� �����
		iN=0;
	}
} // square_wave_seq

// ����������������� 23.07.2016
void square_wave_timestep(doublereal EndTime, integer &iN, doublereal* &timestep_sequence, doublereal* &poweron_multiplier_sequence)
{
	if (EndTime > 0.0) {
		doublereal time = 0.0;
		integer i = 0;
		while (time < EndTime) {			
			if (i % 20 < 10) {
				time += glTSL.tau / 10.0;
			}
			else {
				time += ((glTSL.Q - 1.0)*glTSL.tau) / 10.0;
			}
			i++;
		}
		iN = i; // �������� �� ���� ��� ����� �� ������� EndTime.
		timestep_sequence = new doublereal[iN];
		poweron_multiplier_sequence = new doublereal[iN];
		time = 0.0;
		i = 0;
		while (time < EndTime) {
			
			if (i % 20 < 10) {
				time += glTSL.tau / 10.0;
				timestep_sequence[i] = glTSL.tau / 10.0;
				poweron_multiplier_sequence[i] = 1.0; // �������� ��������.
			}
			else {
				time += ((glTSL.Q - 1)*glTSL.tau) / 10.0;
				timestep_sequence[i] = ((glTSL.Q - 1)*glTSL.tau) / 10.0;
				poweron_multiplier_sequence[i] = 0.0; // �������� ���������.
			}
			i++;
		}
	}
	else {
        // ��������� ������ ������� � ������� �� �����
		iN=0;
	}
} // square_wave_timestep


// �������� �������� ����� ��������� ����� �� ������� piecewise constant
void piecewise_const_timestep_law(doublereal &EndTime, integer &iN, doublereal* &timestep_sequence, doublereal* &poweron_multiplier_sequence) {
	iN = 0;
	doublereal time_now = 0.0;
	for (integer i_35 = 0; i_35 < glTSL.n_string_PiecewiseConst; i_35++) {
		iN += (integer)((glTSL.table_law_piecewise_constant[i_35].time - time_now) / glTSL.table_law_piecewise_constant[i_35].timestep);
			time_now = glTSL.table_law_piecewise_constant[i_35].time;
	}
	EndTime = time_now;
	timestep_sequence = new doublereal[iN];
	poweron_multiplier_sequence = new doublereal[iN];
	integer iscan = 1;
	 
	time_now = 0.0;
	for (integer i_35 = 0; i_35 < glTSL.n_string_PiecewiseConst; i_35++) {
		
		integer iLen = (integer)((glTSL.table_law_piecewise_constant[i_35].time - time_now) / glTSL.table_law_piecewise_constant[i_35].timestep);
		time_now = glTSL.table_law_piecewise_constant[i_35].time;
		for (integer i_36 = iscan; i_36 <= iscan + iLen; i_36++) {
			timestep_sequence[i_36 - 1] = glTSL.table_law_piecewise_constant[i_35].timestep;
			poweron_multiplier_sequence[i_36 - 1] = glTSL.table_law_piecewise_constant[i_35].m;
			//printf("m=%e\n", poweron_multiplier_sequence[i_36 - 1]);
			//system("pause");
		}
		iscan += iLen;
	}

}//  piecewise constant

// ����������������� ��� SquareWave2 ����� 24.07.2016
// tau1 ����� ���� ������� ������ ����. � ���� ������
// multiplyer==m1 ������������. 11.01.2020 
void square_wave_timestep_APPARAT(doublereal EndTime, integer &iN, doublereal* &timestep_sequence, doublereal* &poweron_multiplier_sequence)
{
	if (EndTime > 0.0) {
		doublereal time = 0.0;
		integer ig = 1;
		integer i = 0, j = 0;
		bool bost = false;
		doublereal t_pause_gl = glTSL.T_all - glTSL.n_cycle*(2*glTSL.tau1+glTSL.tau2+glTSL.tau_pause);
		if (t_pause_gl <= 0.0) {
			printf("error in parameters Square Wave 2 time step law.\n");
			//getchar();
			system("PAUSE");
			exit(1);
		}

		integer inumber_step_size = 40;
		while (time < EndTime) {
				i++;
				j++;
				if (!bost) {
					integer kmod = (j - 1) % inumber_step_size;
					if (glTSL.tau1 > 1.0e-30) {
						if (kmod <= 9) {
							time += glTSL.tau1 / 10.0;
						}
						else if (/*(kmod >= 10) &&*/ (kmod <= 19)) {
							time += glTSL.tau2 / 10.0;
						}
						else if (/*(kmod >= 20) &&*/ (kmod <= 29)) {
							time += glTSL.tau1 / 10.0;
						}
						else //if ((kmod >= 30)/* && (kmod <= 39)*/) {
						{
							time += glTSL.tau_pause / 10.0;
						}
					}
					else {
						inumber_step_size = 20;
						kmod = (j - 1) % inumber_step_size;
						// tau1 ����� ����. ������������� �������.
						if (kmod <= 9) {
							time += glTSL.tau2 / 10.0;
						}
						else {
							time += glTSL.tau_pause / 10.0;
						}
					}
				}
					integer kmod2 = (i-1) % (glTSL.n_cycle * inumber_step_size + 30);
					if (kmod2>=glTSL.n_cycle * inumber_step_size)
                     {
						 if (!bost) {
							 if (glTSL.tau1 > 1.0e-30) {
								 time -= glTSL.tau1 / 10.0;
							 }
							 else {
								 // tau1 ����� ����. ������������� �������.
								 time -= glTSL.tau2 / 10.0;
							 }
						 }
							// ������� �� �����
							time += t_pause_gl / 30.0;
							bost = true;


					}
				
					//if (time > ig*glTSL.T_all) {
					if ((i != 1) && ((i - 1) % (glTSL.n_cycle * inumber_step_size + 30) == 0)) {
                		ig++;
						bost = false;
						j = 1;
						if (glTSL.tau1 > 1.0e-30) {
							time += glTSL.tau1 / 10.0;
						}
						else {
							// tau1 ����� ����. ������������� �������.
							time += glTSL.tau2 / 10.0;
						}
					}
				
		}
		iN = i; // �������� �� ���� ��� ����� �� ������� EndTime.
		timestep_sequence = new doublereal[iN];
		poweron_multiplier_sequence = new doublereal[iN];
		time = 0.0;
		ig = 1;
		i = 0;
		j = 0;
		bost = false;
		while (time < EndTime) {
			i++;
			j++;
			if (!bost) {
				integer kmod = (j - 1) % inumber_step_size;
				if (glTSL.tau1 > 1.0e-30) {
					if (kmod <= 9) {
						time += glTSL.tau1 / 10.0;
						timestep_sequence[i - 1] = glTSL.tau1 / 10.0;
						poweron_multiplier_sequence[i - 1] = glTSL.m1; // �������� �������� ��������.
					}
					else if (/*(kmod >= 10) && */(kmod <= 19)) {
						time += glTSL.tau2 / 10.0;
						timestep_sequence[i - 1] = glTSL.tau2 / 10.0;
						poweron_multiplier_sequence[i - 1] = 1.0; // �������� �������� �� ������. 
					}
					else if (/*(kmod >= 20) &&*/ (kmod <= 29)) {
						time += glTSL.tau1 / 10.0;
						timestep_sequence[i - 1] = glTSL.tau1 / 10.0;
						poweron_multiplier_sequence[i - 1] = glTSL.m1; // �������� �������� ��������.
					}
					else //if ((kmod >= 30) && (kmod <= 39)) {
					{
						time += glTSL.tau_pause / 10.0;
						timestep_sequence[i - 1] = glTSL.tau_pause / 10.0;
						poweron_multiplier_sequence[i - 1] = 0.0; // �������� ���������.
					}
				}
				else {
					// tau1 ����� ����. ������������� �������.
					inumber_step_size = 20;
					kmod = (j - 1) % inumber_step_size;
					if (kmod <= 9) {
						time += glTSL.tau2 / 10.0;
						timestep_sequence[i - 1] = glTSL.tau2 / 10.0;
						poweron_multiplier_sequence[i - 1] = 1.0; // �������� �������� ���������.
					}
					else {
						time += glTSL.tau_pause / 10.0;
						timestep_sequence[i - 1] = glTSL.tau_pause / 10.0;
						poweron_multiplier_sequence[i - 1] = 0.0; // �������� ���������.
					}
				}
			}
			integer kmod2 = (i-1) % (glTSL.n_cycle * inumber_step_size + 30);
			if (kmod2 >= glTSL.n_cycle * inumber_step_size)
			{

				if (!bost) {
					if (glTSL.tau1 > 1.0e-30) {
						time -= glTSL.tau1 / 10.0;
					}
					else {
						// tau1 ����� ����. ������������� �������.
						time -= glTSL.tau2 / 10.0;
					}
				}
				// ������� �� �����
				time += t_pause_gl / 30.0;
				bost = true;
				timestep_sequence[i - 1] = t_pause_gl / 30.0;
				poweron_multiplier_sequence[i - 1] = 0.0; // �������� ���������.

			}
			

           // if (time > ig*glTSL.T_all) {
			if ((i != 1) && ((i - 1) % (glTSL.n_cycle * inumber_step_size + 30) == 0)) {
				//printf("incomming\n");
				 ig++;
			     bost = false;
				 j = 1;
				 if (glTSL.tau1 > 1.0e-30) {
					 time += glTSL.tau1 / 10.0;
					 timestep_sequence[i - 1] = glTSL.tau1 / 10.0;
					 poweron_multiplier_sequence[i - 1] = glTSL.m1; // �������� �������� ��������.
				 }
				 else {
					 // tau1 ����� ����. ������������� �������.
					 time += glTSL.tau2 / 10.0;
					 timestep_sequence[i - 1] = glTSL.tau2 / 10.0;
					 poweron_multiplier_sequence[i - 1] = 1.0; // �������� �������� ���������.
				 }
			 }			
		}


		//for (integer i = 0; i < iN; i++) {
#if doubleintprecision == 1
		//printf("%lld dt=%e m=%e\n",i, timestep_sequence[i], poweron_multiplier_sequence[i]);
#else
		//printf("%d dt=%e m=%e\n",i, timestep_sequence[i], poweron_multiplier_sequence[i]);
#endif
			
			//if (i == 150) printf("Period end\n");
			//getchar();
		//}
	}
	else {
		// ��������� ������ ������� � ������� �� �����
		iN = 0;
	}
} // square_wave_timestep_APPARAT


void calculate_color_for_temperature_new(integer* &color, TEMPER t, integer inx, doublereal* &xpos) {

	if ((!b_on_adaptive_local_refinement_mesh) && (number_cores() == 2) && (my_amg_manager.lfil < 3)) {
		// �������� ������ ��� ����������������� �����.
		integer isize = 0;


		integer n = t.maxelm + t.maxbound;
		color = new integer[n];
		for (integer i_1 = 0; i_1 < n; i_1++) color[i_1] = 0; // initialization
															  // ����� �� ����.
		doublereal max = -1.0e60;
		//doublereal min = 1.0e60;
		/*
		for (integer i = 0; i < t.maxelm; i++) {
			TOCHKA point0;
			center_cord3D(i, t.nvtx, t.pa, point0, 100);
			if (point0.x > max) max = point0.x;
			if (point0.x < min) min = point0.x;
		}
		doublereal avg = 0.5 * (min + max);
		*/
		doublereal avg = xpos[(integer)(0.5*inx)];

		
		doublereal dx = 0.0, dy = 0.0, dz = 0.0; // ����� �������� ������������ ������
		integer iP = -1;
		max = 1.0e60;
		for (integer i = 0; i < t.maxelm; i++) {
			TOCHKA point0;
			center_cord3D(i, t.nvtx, t.pa, point0, 100);
			if (fabs(avg - point0.x) < max) {
				max = fabs(avg - point0.x);
				//min = point0.x;
				iP = i;

			}
		}

		integer il = 0, ic = 0, ir = n;
		bool bcontinue = true;
		while ((bcontinue) && (abs(ir - il) > 1.4 * ic)) {

			isize = 0;
			il = 0; ir = 0; ic = 0;// �������������.

			TOCHKA point1;
			center_cord3D(iP, t.nvtx, t.pa, point1, 100);
			avg = point1.x;
			volume3D(iP, t.nvtx, t.pa, dx, dy, dz);
			dx = fabs(dx);
			dy = fabs(dy);
			dz = fabs(dz);

			for (integer i = 0; i < t.maxelm; i++) {
				TOCHKA point0;
				center_cord3D(i, t.nvtx, t.pa, point0, 100);
				if (point0.x < avg - 0.4 * dx) {
					color[i] = 1;
					il++;
				}
				else if (point0.x > avg + 0.4 * dx) {
					color[i] = 3;
					ir++;
				}
				else {
					color[i] = 2;
					isize++;
					ic++;
				}
			}
			for (integer iB = 0; iB < t.maxbound; iB++) {
				integer i = t.border_neighbor[iB].iI;
				TOCHKA point0;
				center_cord3D(i, t.nvtx, t.pa, point0, 100);
				if (point0.x < avg - 0.4 * dx) {
					color[t.maxelm + iB] = 1;
					il++;
				}
				else if (point0.x > avg + 0.4 * dx) {
					color[t.maxelm + iB] = 3;
					ir++;
				}
				else {
					color[t.maxelm + iB] = 2;
					isize++;
					ic++;
				}
			}

			printf("ileft=%lld center=%lld right=%lld\n", il, ic, ir);
			if (ir > il) {
				// ���� ���� t.neighbors_for_the_internal_node[ESIDE][iP].iNODE1; ����������.
				integer icP = t.neighbors_for_the_internal_node[E_SIDE][iP].iNODE1;
				if ((icP >= 0) && (icP < t.maxelm)) {
					iP = icP;
				}
				else {
					bcontinue = false;
				}
			}
			else if (ir < il) {
				// ���� ���� t.neighbors_for_the_internal_node[WSIDE][iP].iNODE1; ����������.
				integer icP = t.neighbors_for_the_internal_node[W_SIDE][iP].iNODE1;
				if ((icP >= 0) && (icP < t.maxelm)) {
					iP = icP;
				}
				else {
					bcontinue = false;
				}
			}
		}


		printf("separator size=%lld\n", isize);
		//getchar();
	}
} // calculate_color_for_temperature_new

void calculate_color_for_temperature_old(integer* &color, TEMPER t) {

	if ((!b_on_adaptive_local_refinement_mesh) && (number_cores() == 2) && (my_amg_manager.lfil < 3)) {
		// �������� ������ ��� ����������������� �����.
		integer isize = 0;


		integer n = t.maxelm + t.maxbound;
		color = new integer[n];
		for (integer i_1 = 0; i_1 < n; i_1++) color[i_1] = 0; // initialization
															  // ����� �� ����.
		doublereal max = -1.0e60;
		doublereal min = 1.0e60;
		for (integer i = 0; i < t.maxelm; i++) {
			TOCHKA point0;
			center_cord3D(i, t.nvtx, t.pa, point0, 100);
			if (point0.x > max) max = point0.x;
			if (point0.x < min) min = point0.x;
		}
		doublereal avg = 0.5 * (min + max);

		
		doublereal dx = 0.0, dy = 0.0, dz = 0.0; // ����� �������� ������������ ������
		integer iP = -1;
		max = 1.0e60;
		for (integer i = 0; i < t.maxelm; i++) {
			TOCHKA point0;
			center_cord3D(i, t.nvtx, t.pa, point0, 100);
			if (fabs(avg - point0.x) < max) {
				max = fabs(avg - point0.x);
				min = point0.x;
				iP = i;

			}
		}

		integer il = 0, ic = 0, ir = n;
		bool bcontinue = true;
		while ((bcontinue) && (abs(ir - il) > 1.4 * ic)) {

			isize = 0;
			il = 0; ir = 0; ic = 0;// �������������.

			TOCHKA point1;
			center_cord3D(iP, t.nvtx, t.pa, point1, 100);
			avg = point1.x;
			volume3D(iP, t.nvtx, t.pa, dx, dy, dz);
			dx = fabs(dx);
			dy = fabs(dy);
			dz = fabs(dz);

			for (integer i = 0; i < t.maxelm; i++) {
				TOCHKA point0;
				center_cord3D(i, t.nvtx, t.pa, point0, 100);
				if (point0.x < avg - 0.4 * dx) {
					color[i] = 1;
					il++;
				}
				else if (point0.x > avg + 0.4 * dx) {
					color[i] = 3;
					ir++;
				}
				else {
					color[i] = 2;
					isize++;
					ic++;
				}
			}
			for (integer iB = 0; iB < t.maxbound; iB++) {
				integer i = t.border_neighbor[iB].iI;
				TOCHKA point0;
				center_cord3D(i, t.nvtx, t.pa, point0, 100);
				if (point0.x < avg - 0.4 * dx) {
					color[t.maxelm + iB] = 1;
					il++;
				}
				else if (point0.x > avg + 0.4 * dx) {
					color[t.maxelm + iB] = 3;
					ir++;
				}
				else {
					color[t.maxelm + iB] = 2;
					isize++;
					ic++;
				}
			}

			printf("ileft=%lld center=%lld right=%lld\n", il, ic, ir);
			if (ir > il) {
				// ���� ���� t.neighbors_for_the_internal_node[ESIDE][iP].iNODE1; ����������.
				integer icP = t.neighbors_for_the_internal_node[E_SIDE][iP].iNODE1;
				if ((icP >= 0) && (icP < t.maxelm)) {
					iP = icP;
				}
				else {
					bcontinue = false;
				}
			}
			else if (ir < il) {
				// ���� ���� t.neighbors_for_the_internal_node[WSIDE][iP].iNODE1; ����������.
				integer icP = t.neighbors_for_the_internal_node[W_SIDE][iP].iNODE1;
				if ((icP >= 0) && (icP < t.maxelm)) {
					iP = icP;
				}
				else {
					bcontinue = false;
				}
			}
		}


		printf("separator size=%lld\n", isize);
		//getchar();
	}
} // calculate_color_for_temperature_old

void calculate_color_for_temperature(integer* &color, TEMPER t, integer inx, doublereal* &xpos) {
	printf("calculate color for solid domain on structured mesh\n");
	if (1) {
		calculate_color_for_temperature_new(color, t, inx, xpos);
	}
	else {
		calculate_color_for_temperature_old(color, t);
	}
}// calculate_color_for_temperature

// �������������� ������������� ������
void unsteady_temperature_calculation(FLOW &f, FLOW* &fglobal, TEMPER &t, doublereal** &rhie_chow,
	                      BLOCK* b, integer lb, SOURCE* s, integer ls, WALL* w, integer lw, 
						  doublereal dbeta, integer flow_interior,  TPROP* matlist, 
						  doublereal operatingtemperature, TEMP_DEP_POWER* gtdps,
	                      integer ltdp, integer lu, UNION* &my_union, bool bsecond_T_solver, integer inx, doublereal* &xpos)
{

	// ����� �������.
	unsigned int calculation_start_time = 0; // ������ ����� ��.
	unsigned int calculation_end_time = 0; // ��������� ����� ��.
	unsigned int calculation_seach_time = 0; // ����� ���������� ������� ���� � ��.

	calculation_start_time = clock(); // ������ ������ �����.


	integer* color = nullptr;	
	calculate_color_for_temperature(color, t, inx, xpos);



	// ������������� ��������� �������� ��� �������������� �������������.
	bool bmyconvective = false;
	if (starting_speed_Vx*starting_speed_Vx + starting_speed_Vy*starting_speed_Vy + starting_speed_Vz*starting_speed_Vz > 1.0e-30) {
		if (fglobal[0].maxelm > 0) {
			bmyconvective = true;
		}
	}
	else {
		// �������� ������������� ��������� ��������.
		errno_t err_inicialization_data=0;
		FILE* fp_inicialization_data=NULL;
#ifdef MINGW_COMPILLER
		err_inicialization_data = 0;
		fp_inicialization_data=fopen64("load.txt", "r");
		if (fp_inicialization_data==NULL) err_inicialization_data = 1;
#else
		err_inicialization_data = fopen_s(&fp_inicialization_data, "load.txt", "r");
#endif
		
		if (err_inicialization_data == 0) {
			// �������� ������ � ���� ������������.
			if (fglobal[0].maxelm > 0) {
				bmyconvective = true;
			}
			fclose(fp_inicialization_data);
		}
	}

	// ��� ������������ ������������� ����������� ��������.
	bool bprintmessage=true; // �������� �� ��������� �� �������.

	doublereal Tamb=operatingtemperature; // ��������� �����������
	//printf("Tamb==%e\n",Tamb);
	//getchar(); // debug;
	doublereal* toldtimestep = nullptr;
	doublereal* tnewtimestep = nullptr;
	integer maxelm_global_ret = 0;
	if (!bsecond_T_solver) {
		toldtimestep = new doublereal[t.maxelm + t.maxbound]; // ���� ���������� �� ���������� ��������� ����
		//integer i=0; // ������� ����� for
		for (integer i = 0; i < t.maxelm + t.maxbound; i++) {
			t.potent[i] = Tamb; // �������������
			toldtimestep[i] = t.potent[i]; // copy
		}
	}
	else {
		// ����� ������������� ������ ���������� �� ���� ������.
		integer maxelm_global = t.maxnod;
		integer ncell_shadow_gl = t.maxelm;
		for (integer iu_74 = 0; iu_74 < lu; iu_74++) {
			maxelm_global += my_union[iu_74].t.maxnod;
			ncell_shadow_gl += my_union[iu_74].t.maxelm;
		}
		maxelm_global_ret = maxelm_global;

		toldtimestep = new doublereal[maxelm_global_ret]; // ���� ���������� �� ���������� ��������� ����
															  //integer i=0; // ������� ����� for
		
		tnewtimestep = new doublereal[maxelm_global_ret];
		for (integer i = 0; i < maxelm_global_ret; i++) {
			tnewtimestep[i] = Tamb; // �������������
			toldtimestep[i] = tnewtimestep[i]; // copy
		}
	}

	integer iN=0; // ���������� ����� �� �������
	doublereal* timestep_sequence=nullptr; // ������������������ ����� �� �������.
	// ���������� � ������ �������� �� ������ ��������� ����
	doublereal* poweron_multiplier_sequence=nullptr; // (��������� ������� �������� ������� �� ����������).
    doublereal StartTime=0.0, EndTime=globalEndTimeUnsteadyTemperatureCalculation; // ������������ 
	doublereal TimeStepIncrement=1.0e-7; // ��������� ��� �� ������� 1���. (������������ � ���������� ���� �� �������.)
	doublereal Initial_Time_Step=1e-7; // �.�. ������� �� ����������.
	doublereal Factor_a=0.4; // ������ ���������� ���� �� �������
	Factor_a = glTSL.Factor_a_for_Linear;
	doublereal** evdokimova_report = nullptr;
	if (glTSL.id_law == 0) {
		// ������� ����� �� ������� � ���������� � ���������� ��������.
		// ���������� ��� �� �������:
		//--->//uniform_timestep_seq(StartTime, EndTime, TimeStepIncrement, iN, timestep_sequence, poweron_multiplier_sequence);
		// ���������� �������� ��� �� ������� (� ������������ � �������������� �����������):
		linear_timestep_seq(StartTime, EndTime, Initial_Time_Step, Factor_a, iN, timestep_sequence, poweron_multiplier_sequence);
		// �� ������ ����������� ������������ ����� � ������� ���������.
		//linear_timestep_seq2(StartTime, EndTime, Initial_Time_Step, Factor_a, iN, timestep_sequence, poweron_multiplier_sequence);

		// ������ �� ������: �������� ������ ����������������� ��������. ���������� �.�., ���� �.�., ����� �.�.
		evdokimova_report = new doublereal*[iN + 1];
		for (integer i = 0; i < iN + 1; i++) {
			// �����, ����������� ������, �������� ������������� ������, �����������, ��������� dC/dRt � C/Rt.
			// time, Tch, Rtch, C=Tch/Rt, dC/dRt, C/Rt (� ��� ��� ������ �� ��� ���������� ������ Tch);
			evdokimova_report[i] = new doublereal[18];
		}
	}
	if (glTSL.id_law == 1) {
		Initial_Time_Step = glTSL.tau / 10.0;
		square_wave_timestep(EndTime, iN, timestep_sequence, poweron_multiplier_sequence);
	}
    // ����������������� ��� �������.
	if (glTSL.id_law == 2) {
		Initial_Time_Step = glTSL.tau1 / 10.0;
		square_wave_timestep_APPARAT(EndTime, iN, timestep_sequence, poweron_multiplier_sequence);
	}
	// ������� ��������������� ��� �� �������: ������-���������.
	if (glTSL.id_law == 3) {
        // 18.11.2017
		linear_timestep_seq_hot_cold(StartTime, EndTime, Initial_Time_Step, Factor_a, iN, timestep_sequence, poweron_multiplier_sequence, glTSL.on_time_double_linear);
	}
	if (glTSL.id_law == 4) {
		// �������� �������� ����� ��������� ����� �� ������� piecewise constant
		// 20.12.2019
		Initial_Time_Step = glTSL.table_law_piecewise_constant[0].timestep;
		piecewise_const_timestep_law(EndTime, iN, timestep_sequence, poweron_multiplier_sequence);
	}

	FILE *fpcurvedata=NULL; // ���� � ������� ����� ������������ ���������� ��������������� �������������.
	errno_t err;

	FILE *fpKras=NULL; // ���� � ������� ����� ������������ ���������� ��������������� �������������.
	errno_t err23=0;
#ifdef MINGW_COMPILLER
	fpKras = fopen64("inputKras.txt", "w");
	if (fpKras == NULL) err23 = 1;
#else
	err23 = fopen_s(&fpKras, "inputKras.txt", "w");
#endif
	

	FILE *fpKras_max = NULL; // ���� � ������� ����� ������������ ���������� ��������������� �������������.
	errno_t err23_max = 0;
#ifdef MINGW_COMPILLER
	fpKras_max = fopen64("inputKras_max.txt", "w");
	if (fpKras_max == NULL) err23_max = 1;
#else
	err23_max = fopen_s(&fpKras_max, "inputKras_max.txt", "w");
#endif

	FILE *fpKras_min = NULL; // ���� � ������� ����� ������������ ���������� ��������������� �������������.
	errno_t err23_min = 0;
#ifdef MINGW_COMPILLER
	fpKras_min = fopen64("inputKras_min.txt", "w");
	if (fpKras_min == NULL) err23_min = 1;
#else
	err23_min = fopen_s(&fpKras_min, "inputKras_min.txt", "w");
#endif

	if ((err23) != 0) {
		printf("Create File heating_curves.txt Error\n");
		//getchar();
		system("pause");
		exit(0);
	}
	else {
		if (fpKras != NULL) {
			if (glTSL.id_law == 0) {
				// Linear
				fprintf(fpKras, "1 \n");
				fprintf(fpKras, "0 \n");
			}
			else {
				// Square Wave and Square Wave 2.
				fprintf(fpKras, "0 \n");
				fprintf(fpKras, "0 \n");
			}
			fprintf(fpKras, "Evalution maximum temperature in default interior \n");
			fprintf(fpKras, "time[s] maximum_temperature[C] \n");
			if (glTSL.id_law == 1) {
				// ������ ���� square wave.
				if (fpKras_max != NULL) {
					fprintf(fpKras_max, "0 \n");
					fprintf(fpKras_max, "0 \n");
					fprintf(fpKras_max, "Evalution maximum temperature in default interior \n");
					fprintf(fpKras_max, "time[s] maximum_temperature[C] \n");
				}
				if (fpKras_min != NULL) {
					fprintf(fpKras_min, "0 \n");
					fprintf(fpKras_min, "0 \n");
					fprintf(fpKras_min, "Evalution minimum temperature in default interior \n");
					fprintf(fpKras_min, "time[s] maximum_temperature[C] \n");
				}
			}
			if (fpKras_max != NULL) {
				fclose(fpKras_max);
			}
			if (fpKras_min != NULL) {
				fclose(fpKras_min);
			}
		}
#ifdef MINGW_COMPILLER
		err = 0;
		fpcurvedata=fopen64("heating_curves.txt", "w");
		if (fpcurvedata == NULL) err = 1;
#else
		err = fopen_s(&fpcurvedata, "heating_curves.txt", "w");
#endif
		
		if ((err) != 0) {
			printf("Create File heating_curves.txt Error\n");
			//getchar();
			system("pause");
			exit(0);
		}
		else {
			if (iN <= 0) {
				printf("error in setting the time steps...\n");
				printf("please press any key to exit...\n");
				if (fpcurvedata != NULL) {
					fprintf(fpcurvedata, "Error in setting the time steps...");
				}
				//getchar();
				system("pause");
				if (fpcurvedata != NULL) {
					fclose(fpcurvedata);
				}
				if (fpKras != NULL) {
					fclose(fpKras);
				}
				
				exit(0);
			}
			fprintf(fpcurvedata, " Heating Curves data\n");
			// ����� � ��������, ������������ ����������� �� ���� ��������� ������� (���������� + ��������� ����), 
			// ������������ ����������� ����������� ������ �� ������ ���������� ��.
			fprintf(fpcurvedata, "time [s], temperature all interior [�C], RT all interior [�C/W], temperature only internal nodes [�C], RT internal nodes [�C/W], filtr temperature [�C], RT filtr [�C/W]\n");
			fprintf(fpcurvedata, "%+.16f %+.16f %+.16f %+.16f  %+.16f %+.16f %+.16f\n", StartTime, Tamb, 0.0, Tamb, 0.0, Tamb, 0.0); // ��������� ��������� �� �������� �������� ��������.
			if (glTSL.id_law == 0) {
                // Linear.
				evdokimova_report[0][0] = StartTime; evdokimova_report[0][1] = Tamb; evdokimova_report[0][2] = 0.0;
				evdokimova_report[0][6] = StartTime; evdokimova_report[0][7] = Tamb;  evdokimova_report[0][8] = 0.0;
				evdokimova_report[0][12] = StartTime; evdokimova_report[0][13] = Tamb; evdokimova_report[0][14] = 0.0;
			}
            fprintf(fpKras, "%+.16f %+.16f\n", 0.9e-7, Tamb);

#ifdef MINGW_COMPILLER
			fpKras_max = fopen64("inputKras_max.txt", "a");
#else
			err23_max = fopen_s(&fpKras_max, "inputKras_max.txt", "a");
#endif
			if ((err23_max == 0) && (fpKras_max != NULL)) {
				fprintf(fpKras_max, "%+.16f %+.16f\n", 0.9e-7, Tamb);
				fclose(fpKras_max);
			}

#ifdef MINGW_COMPILLER
			fpKras_min = fopen64("inputKras_min.txt", "a");
#else
			err23_min = fopen_s(&fpKras_min, "inputKras_min.txt", "a");
#endif
			if ((err23_min == 0) && (fpKras_min != NULL)) {
				fprintf(fpKras_min, "%+.16f %+.16f\n", 0.9e-7, Tamb);
				fclose(fpKras_min);
			}

			QuickMemVorst my_memory_bicgstab;
			my_memory_bicgstab.ballocCRSt = false; // �������� ������.
			my_memory_bicgstab.bsignalfreeCRSt = false; // �� ����� �� ����������� !.
			// ������������� ����������.
			my_memory_bicgstab.tval = nullptr;
			my_memory_bicgstab.tcol_ind = nullptr;
			my_memory_bicgstab.trow_ptr = nullptr;
			my_memory_bicgstab.tri = nullptr;
			my_memory_bicgstab.troc = nullptr;
			my_memory_bicgstab.ts = nullptr;
			my_memory_bicgstab.tt = nullptr;
			my_memory_bicgstab.tvi = nullptr;
			my_memory_bicgstab.tpi = nullptr;
			my_memory_bicgstab.tdx = nullptr;
			my_memory_bicgstab.tdax = nullptr;
			my_memory_bicgstab.ty = nullptr;
			my_memory_bicgstab.tz = nullptr;
			my_memory_bicgstab.ta = nullptr;
			my_memory_bicgstab.tja = nullptr;
			my_memory_bicgstab.tia = nullptr;
			my_memory_bicgstab.talu = nullptr;
			my_memory_bicgstab.tjlu = nullptr;
			my_memory_bicgstab.tju = nullptr;
			my_memory_bicgstab.tiw = nullptr;
			my_memory_bicgstab.tlevs = nullptr;
			my_memory_bicgstab.tw = nullptr;
			my_memory_bicgstab.tjw = nullptr;
			my_memory_bicgstab.icount_vel = 100000; // ����� ������� �����.

			doublereal phisicaltime = StartTime;

			// ��������� ����� � ����������� ������� ������� �� ������� ������� ������:
			// ��������� ������������� ���� ����������.
			if (!bsecond_T_solver) {
				report_temperature_for_unsteady_modeling(0, fglobal, t, b, lb, s, ls, w, lw, 0, phisicaltime, 1.0, operatingtemperature);
			}

			/*
			FILE *fp_for_icepak;
			errno_t err1_for_icepak;
			// �������� ����� ��� ������.
			if ((err1_for_icepak = fopen_s(&fp_for_icepak, "report_timestep_piecewice_const.txt", "w")) != 0) {
				printf("Create File report_timestep_piecewice_const.txt Error\n");
				//getchar();
				system("pause");
			}
			else {
				// ������ ���������
				doublereal fcurent_time_val = 0.0;
				for (integer i_332 = 0; i_332 < iN; i_332++) {
					fprintf(fp_for_icepak, "%.2f %.2f\n", fcurent_time_val, timestep_sequence[i_332]);
					fcurent_time_val += timestep_sequence[i_332];
				}
				fclose(fp_for_icepak);
			}
			// �������� ����� ��� ������.
			if ((err1_for_icepak = fopen_s(&fp_for_icepak, "report_powermultiplyer_piecewice_linear.txt", "w")) != 0) {
				printf("Create File report_powermultiplyer_piecewice_linear.txt Error\n");
				//getchar();
				system("pause");
			}
			else {
				// ������ ���������
				doublereal fcurent_time_val = 0.0;
				for (integer i_332 = 0; i_332 < iN; i_332++) {
					fprintf(fp_for_icepak, "%.2f %.2f\n", fcurent_time_val, poweron_multiplier_sequence[i_332]);
					fcurent_time_val += timestep_sequence[i_332];
				}
				fclose(fp_for_icepak);
			}
			printf("icepak aprioritydata is construct...\n");
			system("pause");
			*/

			bool bfirst_export = true;

			// �������������� ������:
			for (integer j = 0; j < iN; j++) {
				
				
				if (j == iN - 1) {
					// ����������� ������ !
					my_memory_bicgstab.bsignalfreeCRSt = true;
				}

				phisicaltime += timestep_sequence[j]; // ��������� ������� ������������� �� �������, ���� ������ ������� ��� ��������

				doublereal tauparamold = timestep_sequence[j];
				if (j > 0) {
					// �������� ���� �� ������� � ����������� ���� �� �������.
					tauparamold = timestep_sequence[j - 1];
				}

				bool btimedep = true; // �������������� ������
				if (bsecond_T_solver) {
					solve_Thermal(t, fglobal, matlist,
						w, lw, lu, b, lb, ls, 
						my_memory_bicgstab,
						false, operatingtemperature,
						// ��� ��������������� �������������� ������������� 10.11.2018
						btimedep, timestep_sequence[j],
						toldtimestep, tnewtimestep, maxelm_global_ret,
						poweron_multiplier_sequence[j], bAVLrealesation);
				}
				else {
					integer dist_max = 3;

					solve_nonlinear_temp(f, fglobal,
						t, rhie_chow,
						b, lb, s, ls, w, lw,
						dbeta, flow_interior,
						bmyconvective,
						toldtimestep,
						timestep_sequence[j],
						tauparamold,
						btimedep, matlist,
						j, bprintmessage,
						gtdps, ltdp,
						poweron_multiplier_sequence[j],
						my_memory_bicgstab,
						nullptr, // �������� � ����������� ���������� ����.
						nullptr, lu, my_union, color, dist_max); // �������� ����� ����� ������� � ����������� ���������� ����.
				}
				
				

				if (!bsecond_T_solver) {

					for (integer i = 0; i < t.maxelm + t.maxbound; i++) {
						if (t.potent[i] < -273.15) {
							t.potent[i] = -273.15; // �������������� ���������� ����.
						}
					}

					if ((glTSL.id_law == 2) && ((j == 220) || (j == 490) || (j == 760) || (j == 1039))) {
						// 29_11_2017
						// ��������� ������ ����� 6 ��������� �� 1, 2, 3, 4 �����.
						if (!b_on_adaptive_local_refinement_mesh) {
							bool bextendedprint_1 = false;
							exporttecplotxy360T_3D_part2_apparat_hot(t.maxelm, t.ncell, fglobal, t, flow_interior, 0, bextendedprint_1, 1, b);
						}
						else {
							// ������� � ����
							// ������� � ��������� tecplot �����������.
							//� ���� �����.
							ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, fglobal, 1, b, lb);
						}
					}
					if ((glTSL.id_law == 2) && ((j == 1300) || (j == 1570) || (j == 1840) || (j == 2110))) {
						// 29_11_2017
						// ��������� ������ ����� 6 ��������� �� 5, 6, 7, 8 �����.
						if (!b_on_adaptive_local_refinement_mesh) {
							bool bextendedprint_1 = false;
							exporttecplotxy360T_3D_part2_apparat_hot(t.maxelm, t.ncell, fglobal, t, flow_interior, 0, bextendedprint_1, 1, b);
						}
						else {
							// ������� � ����
							// ������� � ��������� tecplot �����������.
							//� ���� �����.
							ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, fglobal, 1, b, lb);
						}
					}
					if ((glTSL.id_law == 2) && ((j == 2380) || (j == 2650) || (j==2920)||(j == 3190))) {
						// 30_07_2020
						// ��������� ������ ����� 6 ��������� �� 9, 10, 11, 12 �����.
						if (!b_on_adaptive_local_refinement_mesh) {
							bool bextendedprint_1 = false;
							exporttecplotxy360T_3D_part2_apparat_hot(t.maxelm, t.ncell, fglobal, t, flow_interior, 0, bextendedprint_1, 1, b);
						}
						else {
							// ������� � ����
							// ������� � ��������� tecplot �����������.
							//� ���� �����.
							ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, fglobal, 1, b, lb);
						}
					}
					for (integer i = 0; i < t.maxelm + t.maxbound; i++) toldtimestep[i] = t.potent[i]; // copy

					if (!b_on_adaptive_local_refinement_mesh) {
						if (bfirst_export && (phisicaltime > 287990)) {
							bfirst_export = false;
							// ��������� ������ ����� 6 ��������� �� �������� �����.
							bool bextendedprint_1 = false;
							exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, 0, bextendedprint_1, 1, b, lb);
						}
					}

					if (ianimation_write_on == 1) {
						if (!b_on_adaptive_local_refinement_mesh) {
							// ������ ������������ ������.
							bool bextendedprint_1 = false;
							integer inumbercadr = j;
							exporttecplotxy360T_3D_part2_ianimation_series(t.maxelm, t.ncell, fglobal, t, flow_interior, 0, bextendedprint_1, 1, inumbercadr, phisicaltime, b);
						}
					}

					// ��������� ����� � ����������� ������� ������� �� ������� ������� ������:
					report_temperature_for_unsteady_modeling(0, fglobal, t, b, lb, s, ls, w, lw, 0, phisicaltime, poweron_multiplier_sequence[j], operatingtemperature);

				}
				else {
					// ����� ������������� ������.
					for (integer i = 0; i < maxelm_global_ret; i++) {
						if (tnewtimestep[i] < -273.15) {
							tnewtimestep[i] = -273.15; // �������������� ���������� ����.
						}
					}

					for (integer i = 0; i < maxelm_global_ret; i++) toldtimestep[i] = tnewtimestep[i]; // copy
				}
				doublereal tmaxi = -1.0e10; // ������������ ����������� ��� ���������� ��.


				
				

				doublereal tmaxavg = -273.15;
				doublereal *nullpointer = nullptr;
				if (!bsecond_T_solver) {
					//if (!b_on_adaptive_local_refinement_mesh) {

						// ���������� �������� ����, � ����������� �� �� 9.01.2017.
						// ���������� �������� ������ �� ������� ������������� 
						// �����������������  �����.
						/*
						doublereal* tempfiltr = new doublereal[t.maxelm + t.maxbound];
						double_average_potent(t.potent, tempfiltr,
							t.maxelm, t.maxbound, t.neighbors_for_the_internal_node,
							t.nvtx, t.pa, nullpointer,
							SIMPSON_FILTR, t.border_neighbor, 0); // VOLUME_AVERAGE_FILTR

						for (integer i = 0; i < t.maxelm; i++) tmaxavg = fmax(tmaxavg, tempfiltr[i]);
						if (!b_on_adaptive_local_refinement_mesh) {
							xyplot_temp(t, tempfiltr);
						}
						if (tempfiltr != nullptr) {
							delete[] tempfiltr; // ������������ ������.
							tempfiltr = nullptr;
						}
						*/
						//for (integer i = 0; i < t.maxelm; i++) tmaxavg = fmax(tmaxavg, t.potent[i]);
					//}
					//else {
						for (integer i = 0; i < t.maxelm; i++) tmaxavg = fmax(tmaxavg, t.potent[i]);
					//}
				}
				else {
					// ����� ������������� ������.
					for (integer i = 0; i < maxelm_global_ret; i++) tmaxavg = fmax(tmaxavg, tnewtimestep[i]);
					tmaxi = tmaxavg;// ������ ��� ���������� �� ���������� � ��������� ����������� ������.
				}

				doublereal Pdiss = 0.0; // �������� ������������ � �����.
				doublereal tmaxall = tmaxi; // ������������ ����������� ��� ���� �� ���������� � ���������.

				if (!bsecond_T_solver) {
					integer ifindloc = 0; // ������� �� ����� ��� ������� ������������ �����������.
					for (integer i = 0; i < t.maxelm; i++) {
						//tmaxi=fmax(tmaxi,t.potent[i]);
						if (t.potent[i] > tmaxi) {
							tmaxi = t.potent[i];
							ifindloc = i; // ���������� ������� ���������.
						}
					}
					
					for (integer i = t.maxelm; i < t.maxelm + t.maxbound; i++) tmaxall = fmax(tmaxall, t.potent[i]);

					
					for (integer isource = 0; isource < ls; isource++) {
						Pdiss += s[isource].power;
					}
					//for (integer iblock = 0; iblock < lb; iblock++) {
						//Pdiss += b[iblock].Sc*fabs(b[iblock].g.xE - b[iblock].g.xS)*fabs(b[iblock].g.yE - b[iblock].g.yS)*fabs(b[iblock].g.zE - b[iblock].g.zS);
					//}
				}
				// 19 november 2016.
				// ���������� �������� �������������� �� ���� ���������� �����.
				if (!bsecond_T_solver) {
					for (integer i47 = 0; i47 < t.maxelm; i47++) {
						// �������� � ��� ��� �������� �� ����������� ��� ������ � ������ ��������.
						integer ib = t.whot_is_block[i47];
						t.Sc[i47] = get_power(b[ib].n_Sc, b[ib].temp_Sc, b[ib].arr_Sc, t.potent[i47]);
						// ���������� �������� �������� ������������ ������:
						doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������ ������
						volume3D(i47, t.nvtx, t.pa, dx, dy, dz);
						Pdiss += t.Sc[i47] * dx*dy*dz;
					}
				}
				else {
					// ����� ������������� ������.
					// �������� �������� ����������� ��� ����������� operatingtemperature
					// �.�. ����� ��������� �������� �������� ��� �������� ����������� ����� ����������� �� 
					// �������������� �����. 10.11.2018

					for (integer i47 = 0; i47 < t.maxelm; i47++) {
						// �������� � ��� ��� �������� �� ����������� ��� ������ � ������ ��������.
						integer ib = t.whot_is_block[i47];
						t.Sc[i47] = get_power(b[ib].n_Sc, b[ib].temp_Sc, b[ib].arr_Sc, operatingtemperature);
						// ���������� �������� �������� ������������ ������:
						doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������ ������
						volume3D(i47, t.nvtx, t.pa, dx, dy, dz);
						Pdiss += t.Sc[i47] * dx*dy*dz;
					}
				}
				printf("Pdiss=%e\n", Pdiss); // �������� ������������ � ����� � ������������ ���� �� ������� ����������.
				if (fabs(Pdiss) < 1.0e-30) {
					Pdiss = 1.0; // ����� �������� ������ RT ��������.
					printf("Warning !!! Pdissipation Energy is equal zero (calculation source object).\n");
					printf("Pdiss_virtual:=1.0; RT==DeltaT==(Tmax-Tamb)/1.0;\n");
					printf("Please, press any key to continue...\n");
					//getchar();
					system("pause");
				}
				//getchar();

				fprintf(fpcurvedata, "%+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f\n", phisicaltime, tmaxall, (tmaxall - Tamb) / Pdiss, tmaxi, (tmaxi - Tamb) / Pdiss, tmaxavg, (tmaxavg - Tamb) / Pdiss);
				if (glTSL.id_law == 0) {
                    // Linear.
					if (evdokimova_report != nullptr) {
						evdokimova_report[j + 1][0] = phisicaltime; evdokimova_report[j + 1][1] = tmaxall; evdokimova_report[j + 1][2] = (tmaxall - Tamb) / Pdiss;
						evdokimova_report[j + 1][6] = phisicaltime; evdokimova_report[j + 1][7] = tmaxi;  evdokimova_report[j + 1][8] = (tmaxi - Tamb) / Pdiss;
						evdokimova_report[j + 1][12] = phisicaltime; evdokimova_report[j + 1][13] = tmaxavg; evdokimova_report[j + 1][14] = (tmaxavg - Tamb) / Pdiss;
					}
				}
                fprintf(fpKras, "%+.16f %+.16f\n", phisicaltime, tmaxi); // tmaxall
				if (glTSL.id_law == 1) {
					// ������ ���� square wave.
					if ((j +1 - 10) % 20 == 0) {
#ifdef MINGW_COMPILLER
						fpKras_max = fopen64("inputKras_max.txt", "a");
#else
						err23_max = fopen_s(&fpKras_max, "inputKras_max.txt", "a");
#endif
						if ((err23_max == 0) && (fpKras_max != NULL)) {
							fprintf(fpKras_max, "%+.16f %+.16f\n", phisicaltime, tmaxi);
							fclose(fpKras_max);
						}
					}
					if ((j+1) % 20 == 0) {
#ifdef MINGW_COMPILLER
						fpKras_min = fopen64("inputKras_min.txt", "a");
#else
						err23_min = fopen_s(&fpKras_min, "inputKras_min.txt", "a");
#endif
						if ((err23_min == 0) && (fpKras_min != NULL)) {
							fprintf(fpKras_min, "%+.16f %+.16f\n", phisicaltime, tmaxi);
							fclose(fpKras_min);
						}
					}
				}
				printf("complete is: %3.0f %% \n", (doublereal)(100.0*(j + 1) / iN)); // ���������� ������� ��������� ���������.
			}

			fclose(fpcurvedata); // �������� ����� ��� ������ ������ ��������.
		}
		fclose(fpKras); // �������� ����� ��� ������ ������ �������� � ������� ��� ������������ ����.
	}

	
	if (toldtimestep != nullptr) {
		delete[] toldtimestep;
	}	
	if (tnewtimestep != nullptr) {
		delete[] tnewtimestep;
	}
	if (timestep_sequence!=nullptr) {
		delete[] timestep_sequence;
	}
	if (poweron_multiplier_sequence!=nullptr) {
		delete[] poweron_multiplier_sequence;
	}

	if (glTSL.id_law == 0) {
        // Linear.
		// ������������ ������ �.�. ����������.
		// C dC/dRt C/Rt
		evdokimova_report[0][3] = 0.0;  evdokimova_report[0][5] = 0.0;
		evdokimova_report[0][9] = 0.0;  evdokimova_report[0][11] = 0.0;
		evdokimova_report[0][15] = 0.0; evdokimova_report[0][17] = 0.0;
		for (integer i = 1; i < iN; i++) {
			// ��� 1.
			evdokimova_report[i][3] = evdokimova_report[i][0] / evdokimova_report[i][2];  evdokimova_report[i][5] = evdokimova_report[i][3] / evdokimova_report[i][2];
			evdokimova_report[i][9] = evdokimova_report[i][6] / evdokimova_report[i][8];  evdokimova_report[i][11] = evdokimova_report[i][9] / evdokimova_report[i][8];
			evdokimova_report[i][15] = evdokimova_report[i][12] / evdokimova_report[i][14];  evdokimova_report[i][17] = evdokimova_report[i][15] / evdokimova_report[i][14];
		}
		for (integer i = 0; i < iN; i++) {
			// ��� 2.
			// ������ ��� ������ ���������� ����� ���� 1, �.�. �� ������� �� ����������� ����1.
			if (fabs(evdokimova_report[i + 1][2] - evdokimova_report[i][2]) < 1e-30) {
				// ���� ����������� ����� ����, �� � ��������� ����� ���� � ������ �� ����� ��������������� 0 �� 0 ������� ��������� ������� ���������.
				evdokimova_report[i][4] = 0.0;
			}
			else {
				evdokimova_report[i][4] = (evdokimova_report[i + 1][3] - evdokimova_report[i][3]) / (evdokimova_report[i + 1][2] - evdokimova_report[i][2]);
			}
			if (fabs(evdokimova_report[i + 1][8] - evdokimova_report[i][8]) < 1.0e-30) {
				evdokimova_report[i][10] = 0.0;
			}
			else {
				evdokimova_report[i][10] = (evdokimova_report[i + 1][9] - evdokimova_report[i][9]) / (evdokimova_report[i + 1][8] - evdokimova_report[i][8]);
			}
			if (fabs(evdokimova_report[i + 1][14] - evdokimova_report[i][14]) < 1.0e-30) {
				evdokimova_report[i][16] = 0.0;
			}
			else {
				evdokimova_report[i][16] = (evdokimova_report[i + 1][15] - evdokimova_report[i][15]) / (evdokimova_report[i + 1][14] - evdokimova_report[i][14]);
			}
		}
		evdokimova_report[iN][3] = evdokimova_report[iN][0] / evdokimova_report[iN][2];
		evdokimova_report[iN][5] = evdokimova_report[iN][3] / evdokimova_report[iN][2];
		if (fabs(evdokimova_report[iN][2] - evdokimova_report[iN - 1][2]) < 1.0e-30) {
			// ��������������� ���� �� ����.
			evdokimova_report[iN][4] = 0.0;
		}
		else {
			evdokimova_report[iN][4] = (evdokimova_report[iN][3] - evdokimova_report[iN - 1][3]) / (evdokimova_report[iN][2] - evdokimova_report[iN - 1][2]);
		}

		evdokimova_report[iN][9] = evdokimova_report[iN][6] / evdokimova_report[iN][8];
		evdokimova_report[iN][11] = evdokimova_report[iN][9] / evdokimova_report[iN][8];
		if (fabs(evdokimova_report[iN][8] - evdokimova_report[iN - 1][8]) < 1.0e-30) {
			// ���������� ��������������� ���� �� ����.
			evdokimova_report[iN][10] = 0.0;
		}
		else {
			evdokimova_report[iN][10] = (evdokimova_report[iN][9] - evdokimova_report[iN - 1][9]) / (evdokimova_report[iN][8] - evdokimova_report[iN - 1][8]);
		}

		evdokimova_report[iN][15] = evdokimova_report[iN][12] / evdokimova_report[iN][14]; // C==time/Rt;
		evdokimova_report[iN][17] = evdokimova_report[iN][15] / evdokimova_report[iN][14];
		if (fabs(evdokimova_report[iN][14] - evdokimova_report[iN - 1][14]) < 1.0e-30) {
			// ��������� ��������������� ���� �� ����.
			evdokimova_report[iN][16] = 0.0;
		}
		else {
			evdokimova_report[iN][16] = (evdokimova_report[iN][15] - evdokimova_report[iN - 1][15]) / (evdokimova_report[iN][14] - evdokimova_report[iN - 1][14]);
		}

		// ������ ������ � ��������� ����:
		FILE *fpevdokimova = NULL;
#ifdef MINGW_COMPILLER
		err = 0;
		fpevdokimova=fopen64("Evdokimova.txt", "w");
		if (fpevdokimova == NULL) err = 1;
#else
		err = fopen_s(&fpevdokimova, "Evdokimova.txt", "w");
#endif
		if ((err) != 0) {
			printf("Create File Evdokimova.txt Error\n");
			// getchar();
			system("pause");
			exit(0);
		}
		else {
			if (fpevdokimova != NULL) {
				fprintf(fpevdokimova, "time Tch_all RTch_all Cchall dCchall/dRt_chall Cchall/Rtchall time Tch_in RTch_in Cchin dCchin/dRt_chin Cchin/Rtchin time Tch_avg RTch_avg Cchavg dCchavg/dRt_chavg Cchavg/Rtchavg \n");
				for (integer i = 0; i <= iN; i++) {
					fprintf(fpevdokimova, "%+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f\n", evdokimova_report[i][0], evdokimova_report[i][1], evdokimova_report[i][2], evdokimova_report[i][3], evdokimova_report[i][4], evdokimova_report[i][5], evdokimova_report[i][6], evdokimova_report[i][7], evdokimova_report[i][8], evdokimova_report[i][9], evdokimova_report[i][10], evdokimova_report[i][11], evdokimova_report[i][12], evdokimova_report[i][13], evdokimova_report[i][14], evdokimova_report[i][15], evdokimova_report[i][16], evdokimova_report[i][17]);
				}
				fclose(fpevdokimova); // ��������� ����.
			}
		}

		if (evdokimova_report != nullptr) {
			for (integer i = 0; i < iN + 1; i++) {
				delete[] evdokimova_report[i];
			}
			delete[] evdokimova_report;
			evdokimova_report = nullptr;
		}

	}

	if (evdokimova_report != nullptr) {
		for (integer i = 0; i < iN + 1; i++) {
			delete[] evdokimova_report[i];
		}
		delete[] evdokimova_report;
		evdokimova_report = nullptr;
	}

	delete[] color;

	// ��������� � ��� 23 ������ 2016 ����.
	calculation_end_time = clock();
	calculation_seach_time = calculation_end_time - calculation_start_time;
	unsigned int im = 0, is = 0, ims = 0;
	im = (unsigned int)(calculation_seach_time / 60000); // ������
	is = (unsigned int)((calculation_seach_time - 60000 * im) / 1000); // �������
	ims = (unsigned int)((calculation_seach_time - 60000 * im - 1000 * is) / 10); // ������������ ������� �� 10

	printf("time calculation is:  %u minute %u second %u 10*millisecond\n", im, is, ims);


} // unsteady_temperature_calculation


  // 24.06.2020 - ������ ����������.
  // �������� ����� ������� ������������� ��������� �������������.
  // 27.06.2020 �������������� ����.
  // 29.06.2020 -12.07.2020 �������������� ������ ���������.
void calculate_Network_T_unsteady(TEMPER& t, FLOW* &fglobal,
	BLOCK*& b, integer& lb,
	WALL*& w, integer& lw,
	SOURCE* &s, integer& ls, TPROP* &matlist)
{

	bool b_first_start_matrix_print = true;

	// ������ source ������������ ������ !!!

	// ����� �������.
	unsigned int calculation_start_time = 0; // ������ ����� ��.
	unsigned int calculation_end_time = 0; // ��������� ����� ��.
	unsigned int calculation_seach_time = 0; // ����� ���������� ������� ���� � ��.

	calculation_start_time = clock(); // ������ ������ �����.

	doublereal* Vol_block = new doublereal[lb]; // ����� ����� � ������ ��� ������� ����� � �������.
	bool* block_is_active = new bool[lb]; // ������ �������� ����� (true). ���� ���� ���� ��������� �������� ������ �� �� (false). cabinet ������ false.
	for (integer i_4 = 0; i_4 < lb; i_4++) {
		block_is_active[i_4] = false;
		Vol_block[i_4] = 0.0;
	}
	for (integer i_4 = 0; i_4 < t.maxelm; i_4++) {
		block_is_active[t.whot_is_block[i_4]] = true;
		doublereal dx = 0.0, dy = 0.0, dz = 0.0; // ����� �������� ������������ ������
		volume3D(i_4, t.nvtx, t.pa, dx, dy, dz);
		Vol_block[t.whot_is_block[i_4]] += dx*dy*dz;
	}
	block_is_active[0] = false;

	integer maxelm = 0;
	for (integer i = 1; i < lb; i++) {
		if ((b[i].itype != HOLLOW) && (block_is_active[i])) maxelm++;
	}
	integer* id = new integer[maxelm + lw];
	integer* id_reverse = new integer[lb + lw];
	integer* wall2block_link = new integer[lw];
	integer ic = 0;
	for (integer i = 1; i < lb; i++) {
		if ((b[i].itype != HOLLOW) && (block_is_active[i])) {
			id[ic] = i; // ������������� �����.
			id_reverse[i] = ic;
			ic++;
		}
	}
	for (integer i = 0; i < lw; i++) {
		id[ic] = i; // ������������� ������.
		id_reverse[lb + i] = ic;
		ic++;
		wall2block_link[i] = -1; // �������������.
	}

	integer* icount_internal_nodes = new integer[lb];
	for (integer i_1 = 0; i_1 < lb; i_1++) {
		icount_internal_nodes[i_1] = 0;
	}

	for (integer j_1 = 0; j_1 < t.maxelm; j_1++) {
		// ���������� ����� � ������ �����.
		icount_internal_nodes[t.whot_is_block[j_1]]++;
	}

	integer** number_control_volume_list = new integer*[lb];
	for (integer i_1 = 0; i_1 < lb; i_1++) {
		number_control_volume_list[i_1] = new integer[icount_internal_nodes[i_1]];
	}

	for (integer i_1 = 0; i_1 < lb; i_1++) {
		icount_internal_nodes[i_1] = 0;
	}

	for (integer j_1 = 0; j_1 < t.maxelm; j_1++) {
		// ���������� ����� � ������ �����.
		number_control_volume_list[t.whot_is_block[j_1]][icount_internal_nodes[t.whot_is_block[j_1]]] = j_1;
		icount_internal_nodes[t.whot_is_block[j_1]]++;
	}

	bool* hash = new bool[lb];
	bool* hash_wall = new bool[lw];
	integer** ilink = new integer *[maxelm + lw];
	integer** ilink_reverse = new integer *[maxelm + lw];
	doublereal** dS = new doublereal *[maxelm + lw];
	integer* inumber_neighbour = new integer[maxelm + lw];
	integer* inumber_neighbour_only_body = new integer[maxelm + lw];
	for (integer i = 0; i < maxelm + lw; i++) {
		inumber_neighbour[i] = 0;// ��� �������.
		inumber_neighbour_only_body[i] = 0;// ��� ������� ����� �������� ������.
		ilink[i] = nullptr;
		ilink_reverse[i] = nullptr;
		dS[i] = nullptr;
	}
	ic = 0;
	for (integer i = 1; i < lb; i++) {
		if ((b[i].itype != HOLLOW) && (block_is_active[i])) {
			for (integer j_1 = 0; j_1 < lb; j_1++) {
				hash[j_1] = false;
			}
			for (integer j_1 = 0; j_1 < lw; j_1++) {
				hash_wall[j_1] = false;
			}
			hash[0] = true;
			hash[i] = true;
			integer ic1 = 0;
			// ������� ���������� ������� ����� i, � ������� id �� ����� ����� ic.
			//for (integer j_1 = 0; j_1 < t.maxelm; j_1++) 
			for (integer j_7=0; j_7< icount_internal_nodes[i]; j_7++)
			{
				integer j_1 = number_control_volume_list[i][j_7];
				if (t.whot_is_block[j_1] == i)
				{
					if ((t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE1 > -1) &&
						(t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE1 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE1;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE2 > -1) &&
						(t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE2 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE2;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE3 > -1) &&
						(t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE3 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE3;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE4 > -1) &&
						(t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE4 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE4;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE1 > -1) &&
						(t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE1 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE1;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE2 > -1) &&
						(t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE2 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE2;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE3 > -1) &&
						(t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE3 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE3;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE4 > -1) &&
						(t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE4 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE4;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE1 > -1) &&
						(t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE1 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE1;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE2 > -1) &&
						(t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE2 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE2;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE3 > -1) &&
						(t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE3 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE3;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE4 > -1) &&
						(t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE4 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE4;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE1 > -1) &&
						(t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE1 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE1;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE2 > -1) &&
						(t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE2 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE2;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE3 > -1) &&
						(t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE3 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE3;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE4 > -1) &&
						(t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE4 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE4;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE1 > -1) &&
						(t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE1 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE1;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE2 > -1) &&
						(t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE2 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE2;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE3 > -1) &&
						(t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE3 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE3;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE4 > -1) &&
						(t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE4 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE4;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE1 > -1) &&
						(t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE1 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE1;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE2 > -1) &&
						(t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE2 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE2;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE3 > -1) &&
						(t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE3 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE3;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE4 > -1) &&
						(t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE4 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE4;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ic1++;
							}
						}
					}


				}
			}
			integer ic_block = ic1;
			//printf("Number of count block 2 block neighbours %lld.\n",ic1);
			//system("PAUSE");
			inumber_neighbour_only_body[ic] = ic_block;
			// ������ � �������� �������, �������-������� ��� �������-���������.
			//for (integer j_1 = 0; j_1 < t.maxelm; j_1++)
			for (integer j_7 = 0; j_7 < icount_internal_nodes[i]; j_7++)
			{
				integer j_1 = number_control_volume_list[i][j_7];
				if (t.whot_is_block[j_1] == i) {
					//������ 
					if (t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE1 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE1;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE2 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE2;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE3 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE3;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE4 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE4;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE1 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE1;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE2 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE2;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE3 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE3;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE4 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE4;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE1 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE1;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE2 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE2;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE3 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE3;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE4 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE4;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE1 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE1;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE2 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE2;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE3 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE3;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE4 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE4;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE1 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE1;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE2 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE2;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE3 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE3;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE4 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE4;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE1 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE1;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE2 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE2;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE3 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE3;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE4 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE4;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ic1++;
							}
						}
					}

				}
			}

			//printf("Number of count block 2 block and wall neighbours %lld.\n", ic1);
			//system("PAUSE");
			ilink[ic] = new integer[ic1];
			for (integer j_1 = 0; j_1 < ic1; j_1++) {
				ilink[ic][j_1] = -1; // ������������� �������������� ��������.
			}
			// �������� �������������� ������� ���������������� ��� ����� � ��� ������.
			ilink_reverse[ic] = new integer[lb + lw + 1];
			for (integer j_1 = 0; j_1 < lb + lw + 1; j_1++) {
				ilink_reverse[ic][j_1] = -1;// ������������� �������������� ��������.
			}

			inumber_neighbour[ic] = ic1; // ���������� ������ �������.
			for (integer j_1 = 0; j_1 < lb; j_1++) {
				hash[j_1] = false;
			}

			hash[0] = true;
			hash[i] = true;
			ic1 = 0;
			// ���������� ���� ������� ����� i, � ������� id �� ����� ����� ic.
			//for (integer j_1 = 0; j_1 < t.maxelm; j_1++) {
			for (integer j_7 = 0; j_7 < icount_internal_nodes[i]; j_7++)
			{
				integer j_1 = number_control_volume_list[i][j_7];
				if (t.whot_is_block[j_1] == i) {

					if ((t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE1 > -1) &&
						(t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE1 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE1;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE2 > -1) &&
						(t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE2 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE2;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE3 > -1) &&
						(t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE3 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE3;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE4 > -1) &&
						(t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE4 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE4;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE1 > -1) &&
						(t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE1 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE1;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE2 > -1) &&
						(t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE2 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE2;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE3 > -1) &&
						(t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE3 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE3;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE4 > -1) &&
						(t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE4 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE4;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE1 > -1) &&
						(t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE1 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE1;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE2 > -1) &&
						(t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE2 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE2;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE3 > -1) &&
						(t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE3 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE3;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE4 > -1) &&
						(t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE4 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE4;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE1 > -1) &&
						(t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE1 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE1;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE2 > -1) &&
						(t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE2 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE2;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE3 > -1) &&
						(t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE3 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE3;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE4 > -1) &&
						(t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE4 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE4;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE1 > -1) &&
						(t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE1 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE1;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE2 > -1) &&
						(t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE2 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE2;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE3 > -1) &&
						(t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE3 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE3;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE4 > -1) &&
						(t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE4 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE4;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE1 > -1) &&
						(t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE1 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE1;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE2 > -1) &&
						(t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE2 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE2;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE3 > -1) &&
						(t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE3 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE3;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
					if ((t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE4 > -1) &&
						(t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE4 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE4;
						if (hash[t.whot_is_block[i_1]] == false) {
							if ((b[t.whot_is_block[i_1]].itype != HOLLOW) && (block_is_active[t.whot_is_block[i_1]])) {
								hash[t.whot_is_block[i_1]] = true;
								ilink[ic][ic1] = t.whot_is_block[i_1];// ������ ����� id[ic].
								ilink_reverse[ic][t.whot_is_block[i_1]] = ic1;
								ic1++;
							}
						}
					}
				}
			}

			//printf("Additional internal neighbourhuuds on block %lld.\n", ic1);
			//system("PAUSE");

			for (integer j_1 = 0; j_1 < lw; j_1++) {
				hash_wall[j_1] = false;
			}

			ic_block = ic1;
			// ������ � �������� �������, �������-������� ��� �������-���������.
			//for (integer j_1 = 0; j_1 < t.maxelm; j_1++) {
			for (integer j_7 = 0; j_7 < icount_internal_nodes[i]; j_7++)
			{
				integer j_1 = number_control_volume_list[i][j_7];
				if (t.whot_is_block[j_1] == i) {
					//������ 
					if (t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE1 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE1;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE2 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE2;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE3 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE3;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE4 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE4;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE1 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE1;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE2 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE2;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE3 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE3;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE4 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE4;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE1 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE1;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE2 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE2;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE3 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE3;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE4 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE4;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE1 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE1;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE2 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE2;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE3 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE3;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE4 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE4;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE1 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE1;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE2 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE2;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE3 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE3;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE4 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE4;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							// ������ � �������� �������.
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� ������� ������� ��� �� �����������.
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE1 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE1;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE2 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE2;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE3 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE3;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}
					if (t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE4 >= t.maxelm) {
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE4;
						integer inumber = i_1 - t.maxelm;
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY))) {
							if (hash_wall[t.border_neighbor[inumber].MCB - ls] == false) {
								// ��� ������ � �������� �������
								hash_wall[t.border_neighbor[inumber].MCB - ls] = true;
								ilink[ic][ic1] = t.border_neighbor[inumber].MCB - ls;
								ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls] = ic1;
								wall2block_link[t.border_neighbor[inumber].MCB - ls] = i;// ������������� ����� � ������� ������� ������.
								ic1++;
							}
						}
					}

				}
			}

			//printf("Additional internal and boundary neighbourhuuds on block %lld.\n", ic1);
			//system("PAUSE");

			dS[ic] = new doublereal[ic1];
			for (integer j_1 = 0; j_1 < ic1; j_1++) {
				dS[ic][j_1] = 0.0; // �������������
			}

			// ���������� ����� ������� �������� � ������.
			//for (integer j_1 = 0; j_1 < t.maxelm; j_1++) {
			for (integer j_7 = 0; j_7 < icount_internal_nodes[i]; j_7++)
			{
				integer j_1 = number_control_volume_list[i][j_7];

				if (t.whot_is_block[j_1] == i) {
					if ((t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE1 > -1) &&
						(t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE1 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE1;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "E_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "E_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hy * hz;
						}
					}
					if ((t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE2 > -1) &&
						(t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE2 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE2;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "E_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "E_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hy * hz;
						}
					}
					if ((t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE3 > -1) &&
						(t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE3 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE3;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "E_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "E_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hy * hz;
						}
					}
					if ((t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE4 > -1) &&
						(t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE4 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE4;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "E_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "E_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hy * hz;
						}
					}
					if ((t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE1 > -1) &&
						(t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE1 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE1;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "W_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "W_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hy * hz;
						}
					}
					if ((t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE2 > -1) &&
						(t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE2 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE2;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "W_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "W_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hy * hz;
						}
					}
					if ((t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE3 > -1) &&
						(t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE3 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE3;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "W_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "W_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hy * hz;
						}
					}
					if ((t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE4 > -1) &&
						(t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE4 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE4;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "W_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "W_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hy * hz;
						}
					}
					if ((t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE1 > -1) &&
						(t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE1 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE1;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "N_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "N_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hx * hz;
						}
					}
					if ((t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE2 > -1) &&
						(t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE2 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE2;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "N_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "N_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hx * hz;
						}
					}
					if ((t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE3 > -1) &&
						(t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE3 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE3;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "N_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "N_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hx * hz;
						}
					}
					if ((t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE4 > -1) &&
						(t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE4 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE4;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "N_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "N_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hx * hz;
						}
					}
					if ((t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE1 > -1) &&
						(t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE1 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE1;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "S_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "S_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hx * hz;
						}
					}
					if ((t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE2 > -1) &&
						(t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE2 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE2;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "S_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "S_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hx * hz;
						}
					}
					if ((t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE3 > -1) &&
						(t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE3 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE3;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "S_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "S_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hx * hz;
						}
					}
					if ((t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE4 > -1) &&
						(t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE4 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE4;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "S_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "S_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hx * hz;
						}
					}
					if ((t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE1 > -1) &&
						(t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE1 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE1;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "T_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "T_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hx * hy;
						}
					}
					if ((t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE2 > -1) &&
						(t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE2 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE2;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "T_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "T_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hx * hy;
						}
					}
					if ((t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE3 > -1) &&
						(t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE3 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE3;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "T_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "T_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hx * hy;
						}
					}
					if ((t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE4 > -1) &&
						(t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE4 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE4;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "T_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "T_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hx * hy;
						}
					}
					if ((t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE1 > -1) &&
						(t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE1 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE1;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "B_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "B_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hx * hy;
						}
					}
					if ((t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE2 > -1) &&
						(t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE2 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE2;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "B_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "B_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hx * hy;
						}
					}
					if ((t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE3 > -1) &&
						(t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE3 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE3;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "B_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "B_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hx * hy;
						}
					}
					if ((t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE4 > -1) &&
						(t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE4 < t.maxelm))
					{
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE4;
						if ((t.whot_is_block[i_1] != i) && (t.whot_is_block[i_1] != 0) && (block_is_active[t.whot_is_block[i_1]])) {
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							if (ilink_reverse[ic][t.whot_is_block[i_1]] == -1)
							{
								std::cout << "B_SIDE problem ilink_reverse block2block" << std::endl;
								system("PAUSE");
							}
							if (ilink_reverse[ic][t.whot_is_block[i_1]] > ic1) {
								std::cout << "B_SIDE problem ilink_reverse block2block ic1 limit" << std::endl;
								system("PAUSE");
							}
							dS[ic][ilink_reverse[ic][t.whot_is_block[i_1]]] += hx * hy;
						}
					}

					// ������.

					if (t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE1 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE1;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hy * hz;
						}

					}
					if (t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE2 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE2;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hy * hz;
						}

					}
					if (t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE3 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE3;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hy * hz;
						}

					}
					if (t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE4 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[E_SIDE][j_1].iNODE4;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hy * hz;
						}

					}
					if (t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE1 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE1;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hy * hz;
						}

					}
					if (t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE2 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE2;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hy * hz;
						}

					}
					if (t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE3 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE3;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hy * hz;
						}

					}
					if (t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE4 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[W_SIDE][j_1].iNODE4;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hy * hz;
						}

					}
					if (t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE1 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE1;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hx * hz;
						}

					}
					if (t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE2 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE2;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hx * hz;
						}

					}
					if (t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE3 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE3;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hx * hz;
						}

					}
					if (t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE4 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[N_SIDE][j_1].iNODE4;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hx * hz;
						}

					}
					if (t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE1 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE1;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							//std::cout << "ic_block = " << ic_block << "; t.border_neighbor[inumber].MCB=" << t.border_neighbor[inumber].MCB << "; ls=" << ls << std::endl;
							//std::cout << "inumber_neighbour[ic]=" << inumber_neighbour[ic]<<std::endl;
							//getchar();
							//dS[ic][ilink_reverse[ic][ic_block + t.border_neighbor[inumber].MCB - ls]] += hx * hz;
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hx * hz;
						}

					}
					if (t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE2 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE2;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							//std::cout << "ic_block = " << ic_block << "; t.border_neighbor[inumber].MCB=" << t.border_neighbor[inumber].MCB << "; ls=" << ls << std::endl;
							//std::cout << "inumber_neighbour[ic]=" << inumber_neighbour[ic]<<std::endl;
							//getchar();
							//dS[ic][ilink_reverse[ic][ic_block + t.border_neighbor[inumber].MCB - ls]] += hx * hz;
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hx * hz;
						}

					}
					if (t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE3 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE3;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							//std::cout << "ic_block = " << ic_block << "; t.border_neighbor[inumber].MCB=" << t.border_neighbor[inumber].MCB << "; ls=" << ls << std::endl;
							//std::cout << "inumber_neighbour[ic]=" << inumber_neighbour[ic]<<std::endl;
							//getchar();
							//dS[ic][ilink_reverse[ic][ic_block + t.border_neighbor[inumber].MCB - ls]] += hx * hz;
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hx * hz;
						}

					}
					if (t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE4 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[S_SIDE][j_1].iNODE4;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							//std::cout << "ic_block = " << ic_block << "; t.border_neighbor[inumber].MCB=" << t.border_neighbor[inumber].MCB << "; ls=" << ls << std::endl;
							//std::cout << "inumber_neighbour[ic]=" << inumber_neighbour[ic]<<std::endl;
							//getchar();
							//dS[ic][ilink_reverse[ic][ic_block + t.border_neighbor[inumber].MCB - ls]] += hx * hz;
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hx * hz;
						}

					}
					if (t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE1 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE1;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hx * hy;
						}

					}
					if (t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE2 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE2;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hx * hy;
						}

					}
					if (t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE3 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE3;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hx * hy;
						}

					}
					if (t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE4 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[T_SIDE][j_1].iNODE4;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hx * hy;
						}

					}
					if (t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE1 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE1;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hx * hy;
						}

					}
					if (t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE2 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE2;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hx * hy;
						}

					}
					if (t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE3 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE3;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hx * hy;
						}

					}
					if (t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE4 >= t.maxelm) {
						// ��������� ����
						integer i_1 = t.neighbors_for_the_internal_node[B_SIDE][j_1].iNODE4;
						integer inumber = i_1 - t.maxelm;
						// ������������� ���������� ����.
						if ((t.border_neighbor[inumber].MCB < (ls + lw)) &&
							(t.border_neighbor[inumber].MCB >= ls) &&
							((w[t.border_neighbor[inumber].MCB - ls].ifamily == DIRICHLET_FAMILY) ||
							(w[t.border_neighbor[inumber].MCB - ls].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[t.border_neighbor[inumber].MCB - ls].ifamily == STEFAN_BOLCMAN_FAMILY)
								|| (w[t.border_neighbor[inumber].MCB - ls].ifamily == NEIMAN_FAMILY)))
						{
							doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
							volume3D(j_1, t.nvtx, t.pa, hx, hy, hz);
							dS[ic][ilink_reverse[ic][lb + t.border_neighbor[inumber].MCB - ls]] += hx * hy;
						}

					}

				}
			}

			//printf("Square calculate Ok.");
			//system("PAUSE");

			ic++;
		}
	}


	delete[] icount_internal_nodes;

	for (integer i_1 = 0; i_1 < lb; i_1++) {
		delete[] number_control_volume_list[i_1];
	}
	delete[] number_control_volume_list;

	doublereal* potent = new doublereal[maxelm + lw]; // ������ �������.
	doublereal* rthdsd = new doublereal[maxelm + lw]; // ������ �����.


	bool bmyconvective = false;


	// ��� ������������ ������������� ����������� ��������.
	bool bprintmessage = true; // �������� �� ��������� �� �������.

	doublereal Tamb = t.operatingtemperature; // ��������� �����������
											//printf("Tamb==%e\n",Tamb);
											//getchar(); // debug;
	doublereal* toldtimestep = nullptr;

	toldtimestep = new doublereal[maxelm + lw]; // ���� ���������� �� ���������� ��������� ����
												//integer i=0; // ������� ����� for
	for (integer i = 0; i < maxelm + lw; i++) {
		potent[i] = Tamb; // �������������
		toldtimestep[i] = potent[i]; // copy
	}

	integer iN = 0; // ���������� ����� �� �������
	doublereal* timestep_sequence = nullptr; // ������������������ ����� �� �������.
											 // ���������� � ������ �������� �� ������ ��������� ����
	doublereal* poweron_multiplier_sequence = nullptr; // (��������� ������� �������� ������� �� ����������).
	doublereal StartTime = 0.0, EndTime = globalEndTimeUnsteadyTemperatureCalculation; // ������������ 
	doublereal TimeStepIncrement = 1.0e-7; // ��������� ��� �� ������� 1���. (������������ � ���������� ���� �� �������.)
	doublereal Initial_Time_Step = 1e-7; // �.�. ������� �� ����������.
	doublereal Factor_a = 0.4; // ������ ���������� ���� �� �������
	Factor_a = glTSL.Factor_a_for_Linear;
	doublereal** evdokimova_report = nullptr;
	if (glTSL.id_law == 0) {
		// ������� ����� �� ������� � ���������� � ���������� ��������.
		// ���������� ��� �� �������:
		//--->//uniform_timestep_seq(StartTime, EndTime, TimeStepIncrement, iN, timestep_sequence, poweron_multiplier_sequence);
		// ���������� �������� ��� �� ������� (� ������������ � �������������� �����������):
		linear_timestep_seq(StartTime, EndTime, Initial_Time_Step, Factor_a, iN, timestep_sequence, poweron_multiplier_sequence);
		// �� ������ ����������� ������������ ����� � ������� ���������.
		//linear_timestep_seq2(StartTime, EndTime, Initial_Time_Step, Factor_a, iN, timestep_sequence, poweron_multiplier_sequence);

		// ������ �� ������: �������� ������ ����������������� ��������. ���������� �.�., ���� �.�., ����� �.�.
		evdokimova_report = new doublereal*[iN + 1];
		for (integer i = 0; i < iN + 1; i++) {
			// �����, ����������� ������, �������� ������������� ������, �����������, ��������� dC/dRt � C/Rt.
			// time, Tch, Rtch, C=Tch/Rt, dC/dRt, C/Rt (� ��� ��� ������ �� ��� ���������� ������ Tch);
			evdokimova_report[i] = new doublereal[18];
		}
	}
	if (glTSL.id_law == 1) {
		Initial_Time_Step = glTSL.tau / 10.0;
		square_wave_timestep(EndTime, iN, timestep_sequence, poweron_multiplier_sequence);
	}
	// ����������������� ��� �������.
	if (glTSL.id_law == 2) {
		Initial_Time_Step = glTSL.tau1 / 10.0;
		square_wave_timestep_APPARAT(EndTime, iN, timestep_sequence, poweron_multiplier_sequence);
	}
	// ������� ��������������� ��� �� �������: ������-���������.
	if (glTSL.id_law == 3) {
		// 18.11.2017
		linear_timestep_seq_hot_cold(StartTime, EndTime, Initial_Time_Step, Factor_a, iN, timestep_sequence, poweron_multiplier_sequence, glTSL.on_time_double_linear);
	}
	if (glTSL.id_law == 4) {
		// �������� �������� ����� ��������� ����� �� ������� piecewise constant
		// 20.12.2019
		Initial_Time_Step = glTSL.table_law_piecewise_constant[0].timestep;
		piecewise_const_timestep_law(EndTime, iN, timestep_sequence, poweron_multiplier_sequence);
	}

	FILE *fpcurvedata = NULL; // ���� � ������� ����� ������������ ���������� ��������������� �������������.
	errno_t err;

	FILE *fpKras = NULL; // ���� � ������� ����� ������������ ���������� ��������������� �������������.
	errno_t err23 = 0;
#ifdef MINGW_COMPILLER
	fpKras = fopen64("inputKras.txt", "w");
	if (fpKras == NULL) err23 = 1;
#else
	err23 = fopen_s(&fpKras, "inputKras.txt", "w");
#endif


	FILE *fpKras_max = NULL; // ���� � ������� ����� ������������ ���������� ��������������� �������������.
	errno_t err23_max = 0;
#ifdef MINGW_COMPILLER
	fpKras_max = fopen64("inputKras_max.txt", "w");
	if (fpKras_max == NULL) err23_max = 1;
#else
	err23_max = fopen_s(&fpKras_max, "inputKras_max.txt", "w");
#endif

	FILE *fpKras_min = NULL; // ���� � ������� ����� ������������ ���������� ��������������� �������������.
	errno_t err23_min = 0;
#ifdef MINGW_COMPILLER
	fpKras_min = fopen64("inputKras_min.txt", "w");
	if (fpKras_min == NULL) err23_min = 1;
#else
	err23_min = fopen_s(&fpKras_min, "inputKras_min.txt", "w");
#endif

	if ((err23) != 0) {
		printf("Create File heating_curves.txt Error\n");
		//getchar();
		system("pause");
		exit(0);
	}
	else {
		if (fpKras != NULL) {
			if (glTSL.id_law == 0) {
				// Linear
				fprintf(fpKras, "1 \n");
				fprintf(fpKras, "0 \n");
			}
			else {
				// Square Wave and Square Wave 2.
				fprintf(fpKras, "0 \n");
				fprintf(fpKras, "0 \n");
			}
			fprintf(fpKras, "Evalution maximum temperature in default interior \n");
			fprintf(fpKras, "time[s] maximum_temperature[C] \n");
			if (glTSL.id_law == 1) {
				// ������ ���� square wave.
				if (fpKras_max != NULL) {
					fprintf(fpKras_max, "0 \n");
					fprintf(fpKras_max, "0 \n");
					fprintf(fpKras_max, "Evalution maximum temperature in default interior \n");
					fprintf(fpKras_max, "time[s] maximum_temperature[C] \n");
				}
				if (fpKras_min != NULL) {
					fprintf(fpKras_min, "0 \n");
					fprintf(fpKras_min, "0 \n");
					fprintf(fpKras_min, "Evalution minimum temperature in default interior \n");
					fprintf(fpKras_min, "time[s] maximum_temperature[C] \n");
				}
			}
			if (fpKras_max != NULL) {
				fclose(fpKras_max);
			}
			if (fpKras_min != NULL) {
				fclose(fpKras_min);
			}
		}
#ifdef MINGW_COMPILLER
		err = 0;
		fpcurvedata = fopen64("heating_curves.txt", "w");
		if (fpcurvedata == NULL) err = 1;
#else
		err = fopen_s(&fpcurvedata, "heating_curves.txt", "w");
#endif

		if ((err) != 0) {
			printf("Create File heating_curves.txt Error\n");
			//getchar();
			system("pause");
			exit(0);
		}
		else {
			if (iN <= 0) {
				printf("error in setting the time steps...\n");
				printf("please press any key to exit...\n");
				if (fpcurvedata != NULL) {
					fprintf(fpcurvedata, "Error in setting the time steps...");
				}
				//getchar();
				system("pause");
				if (fpcurvedata != NULL) {
					fclose(fpcurvedata);
				}
				if (fpKras != NULL) {
					fclose(fpKras);
				}

				exit(0);
			}
			fprintf(fpcurvedata, " Heating Curves data\n");
			// ����� � ��������, ������������ ����������� �� ���� ��������� ������� (���������� + ��������� ����), 
			// ������������ ����������� ����������� ������ �� ������ ���������� ��.
			fprintf(fpcurvedata, "time [s], temperature all interior [�C], RT all interior [�C/W], temperature only internal nodes [�C], RT internal nodes [�C/W], filtr temperature [�C], RT filtr [�C/W]\n");
			fprintf(fpcurvedata, "%+.16f %+.16f %+.16f %+.16f  %+.16f %+.16f %+.16f\n", StartTime, Tamb, 0.0, Tamb, 0.0, Tamb, 0.0); // ��������� ��������� �� �������� �������� ��������.
			if (glTSL.id_law == 0) {
				// Linear.
				evdokimova_report[0][0] = StartTime; evdokimova_report[0][1] = Tamb; evdokimova_report[0][2] = 0.0;
				evdokimova_report[0][6] = StartTime; evdokimova_report[0][7] = Tamb;  evdokimova_report[0][8] = 0.0;
				evdokimova_report[0][12] = StartTime; evdokimova_report[0][13] = Tamb; evdokimova_report[0][14] = 0.0;
			}
			fprintf(fpKras, "%+.16f %+.16f\n", 0.9e-7, Tamb);

#ifdef MINGW_COMPILLER
			fpKras_max = fopen64("inputKras_max.txt", "a");
#else
			err23_max = fopen_s(&fpKras_max, "inputKras_max.txt", "a");
#endif
			if ((err23_max == 0) && (fpKras_max != NULL)) {
				fprintf(fpKras_max, "%+.16f %+.16f\n", 0.9e-7, Tamb);
				fclose(fpKras_max);
			}

#ifdef MINGW_COMPILLER
			fpKras_min = fopen64("inputKras_min.txt", "a");
#else
			err23_min = fopen_s(&fpKras_min, "inputKras_min.txt", "a");
#endif
			if ((err23_min == 0) && (fpKras_min != NULL)) {
				fprintf(fpKras_min, "%+.16f %+.16f\n", 0.9e-7, Tamb);
				fclose(fpKras_min);
			}

			doublereal phisicaltime = StartTime;

			// ���������� ����������� �� �����.
			for (integer i = 0; i < t.maxelm; i++) {
				if (block_is_active[t.whot_is_block[i]]) {
					t.potent[i] = potent[id_reverse[t.whot_is_block[i]]];
				}
			}
			for (integer i = 0; i < t.maxbound; i++) {
				// �������� ����������� �� ���������� ����������� ����.
				t.potent[t.maxelm + i] = t.potent[t.border_neighbor[i].iI];
			}

			// ��������� ����� � ����������� ������� ������� �� ������� ������� ������:
			// ��������� ������������� ���� ����������.
			report_temperature_for_unsteady_modeling(0, fglobal, t, b, lb, s, ls, w, lw, 0, phisicaltime, 1.0, t.operatingtemperature);

			bool bfirst_export = true;

			// �������������� ������:
			for (integer j = 0; j < iN; j++) {


				// ������ ����������� � ����� ��������������� ����������.

				phisicaltime += timestep_sequence[j]; // ��������� ������� ������������� �� �������, ���� ������ ������� ��� ��������

				doublereal tauparamold = timestep_sequence[j];
				if (j > 0) {
					// �������� ���� �� ������� � ����������� ���� �� �������.
					tauparamold = timestep_sequence[j - 1];
				}

				bool btimedep = true; // �������������� ������

									  /*
									  ������� ���� �� ��������� ����:
									  */
				for (integer i = 0; i < maxelm + lw; i++) {
					
					if (i < maxelm) {
						// ���������� ����.
						//doublereal Vol = (b[id[i]].g.xE - b[id[i]].g.xS) *
							//(b[id[i]].g.yE - b[id[i]].g.yS) *
							//(b[id[i]].g.zE - b[id[i]].g.zS);
						doublereal Vol = Vol_block[id[i]];

						doublereal rho, cp, lam;
						rho = 1.1614; cp = 1005; lam = 0.025; // ������������� default  dry air 300K 1atm properties
						if (matlist[b[id[i]].imatid].blibmat == 1) {
							// ������������, ����������� ������ ��������� AliceFlow ��������.
							if (b[id[i]].itype == SOLID) {
								my_solid_properties(potent[i], rho, cp, lam, matlist[b[id[i]].imatid].ilibident);
								// �������� �� ������������ ����������.
								diagnostic_critical_temperature(potent[i], f, t, b, lb);
							} // SOLID
							if (b[id[i]].itype == FLUID) {
								doublereal mu, beta_t; // �������� �� ������������ �� ���������.
								doublereal pressure = 0.0; // �������� ������ ������� ���� (����� �� ����� ����, �.�. ����� ����������� ��������).

								my_fluid_properties(potent[i], pressure, rho, cp, lam, mu, beta_t, matlist[b[id[i]].imatid].ilibident);
							} // FLUID
						}
						else if (matlist[b[id[i]].imatid].blibmat == 0) {
							// �������� ����������� �������������:
							// ���������� ��������.
							rho = matlist[b[id[i]].imatid].rho;
							//cp=matlist[b[id[i]].imatid].cp;
							//lam=matlist[b[id[i]].imatid].lam;
							cp = get_cp(matlist[b[id[i]].imatid].n_cp, matlist[b[id[i]].imatid].temp_cp, matlist[b[id[i]].imatid].arr_cp, potent[i]);
							lam = get_lam(matlist[b[id[i]].imatid].n_lam, matlist[b[id[i]].imatid].temp_lam, matlist[b[id[i]].imatid].arr_lam, potent[i]);

						}



						rthdsd[i] = Vol * poweron_multiplier_sequence[j] *
							get_power(b[id[i]].n_Sc, b[id[i]].temp_Sc, b[id[i]].arr_Sc, potent[i]) +
							(rho*cp*Vol*toldtimestep[i]) / timestep_sequence[j];
					}
					else {
						// ������.
						if (w[id[i]].ifamily == DIRICHLET_FAMILY) {
							rthdsd[i] = w[id[i]].Tamb; // ������ ������� �������.
						}
						else {
							// ���������� ��������� �������.
							rthdsd[i] = 0.0;
						}
					}
				}
				// declarate matrix.

				// ������ ����� ��������� �������� � ������� ����.
				integer nnz = maxelm + lw;
				for (integer i = 0; i < maxelm + lw; i++) {
					nnz += inumber_neighbour[i]; // ���������� ������ � ��������.
					if (i >= maxelm) {
						if ((w[id[i]].ifamily == NEWTON_RICHMAN_FAMILY) ||
							(w[id[i]].ifamily == STEFAN_BOLCMAN_FAMILY)||
							(w[id[i]].ifamily == NEIMAN_FAMILY)) {
							nnz++; // ������������ ���������� ��������� �������.
						}
					}
				}
				doublereal* val = new doublereal[nnz];
				integer* col_ind = new integer[nnz];
				integer* row_ptr = new integer[maxelm + lw + 1];

				// ��������� ����������� �����.
				bool b_nonlinear_network = false;
				bool b_Newton_Richman = false;
				bool b_Stefan_Bolcman = false;
				for (integer i = maxelm; i < maxelm + lw; i++) {
					if (w[id[i]].ifamily == NEWTON_RICHMAN_FAMILY) {
						b_Newton_Richman = true; // ������������ �������-�������.
						b_nonlinear_network = true; // ������ ���������. ����� ��������� ������ ����������.
					}
				}

				for (integer i = maxelm; i < maxelm + lw; i++) {
					if (w[id[i]].ifamily == STEFAN_BOLCMAN_FAMILY) {
						b_Stefan_Bolcman = true; // ������������ ������� - ���������, ����� ����� ������� ����������.
						b_Newton_Richman = false;
						b_nonlinear_network = true; // ������ ���������. ����� ��������� ������ ����������.
					}
				}

				//printf("Assemble matrix Ok.");
				//system("PAUSE");

				doublereal tmax_old = -1.0e30;

				doublereal* potent_old = new doublereal[maxelm + lw];
				for (integer i25 = 0; i25 < maxelm + lw; i25++) potent_old[i25] = potent[i25];

				doublereal r1 = 1.0e-30;
				doublereal r2 = 1.0e30;

				integer iter = -1;
				// solve SLAU
				// Seidel method.
				for ( ; ; ) { // ����������� ����.

					iter++;

					r1 = 1.0e-30;

					doublereal tmax = -1.0e30;
					doublereal tmin = 1.0e30;
					for (integer i = 0; i < maxelm + lw; i++) {
						if (potent[i] > tmax) tmax = potent[i];
						if (potent[i] < tmin) tmin = potent[i];
					}

					if (fabs(tmax - tmin) < 1.0e-30) {
						////printf("%4lld %e\n", iter, 10000.0);
					}
					else {
						//0.00002
						//if (fabs(tmax_old - tmax) < 0.00002 * fabs(tmax - tmin)) {
						//	break; // ��������� ����� �� ����� for.
						//}
						//printf("%4lld %e %e\n", iter, fabs(tmax_old - tmax) / fabs(tmax - tmin),r2);
					}
					//if (r2 / r1 < 0.998) break;// ������
					//1.005 14s 22.2C
					//if ((iter>10)&&(r2 > 1.005*r1)) {
						//printf("Stagnation. break;\n");
						//break;
					//}
					if ((fabs(r2) < 1.0e-2) && (fabs(tmax_old - tmax) < 0.0005)) {
						//std::cout << "break bPhysics_stop, dres<1e-2 && (fabs(maxnew - maxold) < 0.0005)" << std::endl;
						break;
					}
					if ((fabs(r2) < 1.4e-2) && (fabs(tmax_old - tmax) < 0.0005)) {
						//std::cout << "break bPhysics_stop, dres<1e-2 && (fabs(maxnew - maxold) < 0.0005)" << std::endl;
						break;
					}
					r1 = r2;
					tmax_old = tmax;

					// Update matrix:
					// initializate matrix.
					for (integer i = 0; i < nnz; i++) {
						val[i] = 0.0;
						col_ind[i] = -1;
					}
					for (integer i = 0; i < maxelm + lw + 1; i++) {
						row_ptr[i] = nnz;
					}
					row_ptr[0] = 0;

					for (integer i = 0; i < maxelm + lw; i++) {
						if (i < maxelm) {
							// ���������� ����.

							//doublereal Vol = (b[id[i]].g.xE - b[id[i]].g.xS) *
								//(b[id[i]].g.yE - b[id[i]].g.yS) *
								//(b[id[i]].g.zE - b[id[i]].g.zS);
							doublereal Vol = Vol_block[id[i]];

							doublereal rho, cp, lam;
							rho = 1.1614; cp = 1005; lam = 0.025; // ������������� default  dry air 300K 1atm properties
							if (matlist[b[id[i]].imatid].blibmat == 1) {
								// ������������, ����������� ������ ��������� AliceFlow ��������.
								if (b[id[i]].itype == SOLID) {
									my_solid_properties(potent[i], rho, cp, lam, matlist[b[id[i]].imatid].ilibident);
									// �������� �� ������������ ����������.
									diagnostic_critical_temperature(potent[i], f, t, b, lb);
								} // SOLID
								if (b[id[i]].itype == FLUID) {
									doublereal mu, beta_t; // �������� �� ������������ �� ���������.
									doublereal pressure = 0.0; // �������� ������ ������� ���� (����� �� ����� ����, �.�. ����� ����������� ��������).

									my_fluid_properties(potent[i], pressure, rho, cp, lam, mu, beta_t, matlist[b[id[i]].imatid].ilibident);
								} // FLUID
							}
							else if (matlist[b[id[i]].imatid].blibmat == 0) {
								// �������� ����������� �������������:
								// ���������� ��������.
								rho = matlist[b[id[i]].imatid].rho;
								//cp=matlist[b[id[i]].imatid].cp;
								//lam=matlist[b[id[i]].imatid].lam;
								cp = get_cp(matlist[b[id[i]].imatid].n_cp, matlist[b[id[i]].imatid].temp_cp, matlist[b[id[i]].imatid].arr_cp, potent[i]);
								lam = get_lam(matlist[b[id[i]].imatid].n_lam, matlist[b[id[i]].imatid].temp_lam, matlist[b[id[i]].imatid].arr_lam, potent[i]);

							}

							rthdsd[i] = Vol *poweron_multiplier_sequence[j] *
								get_power(b[id[i]].n_Sc, b[id[i]].temp_Sc, b[id[i]].arr_Sc, potent[i]) +
								(rho*cp*Vol*toldtimestep[i]) / timestep_sequence[j];
						}
						else {
							// ������.
							if (w[id[i]].ifamily == DIRICHLET_FAMILY) {
								rthdsd[i] = w[id[i]].Tamb; // ������ ������� �������.
							}
							else {
								// ���������� ��������� �������.
								rthdsd[i] = 0.0;
							}
						}
					}

					// assemble heat transfer martrix
					integer im = 1;
					integer idiag = 0;
					for (integer i = 0; i < maxelm + lw; i++) {
						if (i < maxelm) {
							// ���������� ����.
							doublereal sum = 0.0;
							//printf("i==%lld inumber_neighbour[%lld]=%lld inumber_neighbour_only_body[%lld]=%lld\n", i, i, inumber_neighbour[i], i, inumber_neighbour_only_body[i]);
							//getchar();
							for (integer j_1 = 0; j_1 < inumber_neighbour[i]; j_1++) {

								bool ortho_k1 = false;
								bool ortho_k2 = false;
								doublereal ortho_m1 = 1.0;
								doublereal ortho_m2 = 1.0;

								// ���� id[i] �������� � ������ ilink[i][j_1].
								doublereal distance = 0.0;
								if (j_1 < inumber_neighbour_only_body[i]) {
									// ���� id[i] � ����� ilink[i][j_1].
									TOCHKA bp0, bp1;
									if (b[id[i]].g.itypegeom == PRISM) {
										bp0.x = 0.5 * (b[id[i]].g.xS + b[id[i]].g.xE);
										bp0.y = 0.5 * (b[id[i]].g.yS + b[id[i]].g.yE);
										bp0.z = 0.5 * (b[id[i]].g.zS + b[id[i]].g.zE);
									}
									else if (b[id[i]].g.itypegeom == CYLINDER) {
										switch (b[id[i]].g.iPlane) {
										case XY_PLANE:
											bp0.x = b[id[i]].g.xC;
											bp0.y = b[id[i]].g.yC;
											bp0.z = b[id[i]].g.zC + 0.5* b[id[i]].g.Hcyl;
											break;
										case XZ_PLANE:
											bp0.x = b[id[i]].g.xC;
											bp0.z = b[id[i]].g.zC;
											bp0.y = b[id[i]].g.yC + 0.5 * b[id[i]].g.Hcyl;
											break;
										case YZ_PLANE:
											bp0.y = b[id[i]].g.yC;
											bp0.z = b[id[i]].g.zC;
											bp0.x = b[id[i]].g.xC + 0.5 * b[id[i]].g.Hcyl;
											break;
										}
									}
									else if (b[id[i]].g.itypegeom == POLYGON) {
										// ��������� �������������� ����� ��������.
										integer iscan = 0;
										switch (b[id[i]].g.iPlane_obj2) {
										case XY_PLANE:
											bp0.x = 0.0; bp0.y = 0.0;
											for (iscan = 0; iscan < b[id[i]].g.nsizei; iscan++) {
												bp0.x += b[id[i]].g.xi[iscan];
												bp0.y += b[id[i]].g.yi[iscan];
											}
											bp0.x /= 1.0 * b[id[i]].g.nsizei;
											bp0.y /= 1.0 * b[id[i]].g.nsizei;
											bp0.z = b[id[i]].g.zi[0] + 0.5 * b[id[i]].g.hi[0];
											break;
										case XZ_PLANE:
											bp0.x = 0.0; bp0.z = 0.0;
											for (iscan = 0; iscan < b[id[i]].g.nsizei; iscan++) {
												bp0.x += b[id[i]].g.xi[iscan];
												bp0.z += b[id[i]].g.zi[iscan];
											}
											bp0.x /= 1.0 * b[id[i]].g.nsizei;
											bp0.z /= 1.0 * b[id[i]].g.nsizei;
											bp0.y = b[id[i]].g.yi[0] + 0.5 * b[id[i]].g.hi[0];
											break;
										case YZ_PLANE:
											bp0.y = 0.0; bp0.z = 0.0;
											for (iscan = 0; iscan < b[id[i]].g.nsizei; iscan++) {
												bp0.y += b[id[i]].g.yi[iscan];
												bp0.z += b[id[i]].g.zi[iscan];
											}
											bp0.y /= 1.0 * b[id[i]].g.nsizei;
											bp0.z /= 1.0 * b[id[i]].g.nsizei;
											bp0.x = b[id[i]].g.xi[0] + 0.5 * b[id[i]].g.hi[0];
											break;
										}
									}
									else {
										bp0.x = 0.5 * (b[id[i]].g.xS + b[id[i]].g.xE);
										bp0.y = 0.5 * (b[id[i]].g.yS + b[id[i]].g.yE);
										bp0.z = 0.5 * (b[id[i]].g.zS + b[id[i]].g.zE);
									}
									if (b[ilink[i][j_1]].g.itypegeom == PRISM) {
										bp1.x = 0.5 * (b[ilink[i][j_1]].g.xS + b[ilink[i][j_1]].g.xE);
										bp1.y = 0.5 * (b[ilink[i][j_1]].g.yS + b[ilink[i][j_1]].g.yE);
										bp1.z = 0.5 * (b[ilink[i][j_1]].g.zS + b[ilink[i][j_1]].g.zE);
									}
									else if (b[ilink[i][j_1]].g.itypegeom == CYLINDER) {
										switch (b[ilink[i][j_1]].g.iPlane) {
										case XY_PLANE:
											bp1.x = b[ilink[i][j_1]].g.xC;
											bp1.y = b[ilink[i][j_1]].g.yC;
											bp1.z = b[ilink[i][j_1]].g.zC + 0.5 * b[ilink[i][j_1]].g.Hcyl;
											break;
										case XZ_PLANE:
											bp1.x = b[ilink[i][j_1]].g.xC;
											bp1.z = b[ilink[i][j_1]].g.zC;
											bp1.y = b[ilink[i][j_1]].g.yC + 0.5 * b[ilink[i][j_1]].g.Hcyl;
											break;
										case YZ_PLANE:
											bp1.y = b[ilink[i][j_1]].g.yC;
											bp1.z = b[ilink[i][j_1]].g.zC;
											bp1.x = b[ilink[i][j_1]].g.xC + 0.5 * b[ilink[i][j_1]].g.Hcyl;
											break;
										}
									}
									else if (b[ilink[i][j_1]].g.itypegeom == POLYGON) {
										// ��������� �������������� ����� ��������.
										integer iscan = 0;
										switch (b[ilink[i][j_1]].g.iPlane_obj2) {
										case XY_PLANE:
											bp1.x = 0.0; bp1.y = 0.0;
											for (iscan = 0; iscan < b[ilink[i][j_1]].g.nsizei; iscan++) {
												bp1.x += b[ilink[i][j_1]].g.xi[iscan];
												bp1.y += b[ilink[i][j_1]].g.yi[iscan];
											}
											bp1.x /= 1.0 * b[ilink[i][j_1]].g.nsizei;
											bp1.y /= 1.0 * b[ilink[i][j_1]].g.nsizei;
											bp1.z = b[ilink[i][j_1]].g.zi[0] + 0.5 * b[ilink[i][j_1]].g.hi[0];
											break;
										case XZ_PLANE:
											bp1.x = 0.0; bp1.z = 0.0;
											for (iscan = 0; iscan < b[ilink[i][j_1]].g.nsizei; iscan++) {
												bp1.x += b[ilink[i][j_1]].g.xi[iscan];
												bp1.z += b[ilink[i][j_1]].g.zi[iscan];
											}
											bp1.x /= 1.0 * b[ilink[i][j_1]].g.nsizei;
											bp1.z /= 1.0 * b[ilink[i][j_1]].g.nsizei;
											bp1.y = b[ilink[i][j_1]].g.yi[0] + 0.5 * b[ilink[i][j_1]].g.hi[0];
											break;
										case YZ_PLANE:
											bp1.y = 0.0; bp1.z = 0.0;
											for (iscan = 0; iscan < b[ilink[i][j_1]].g.nsizei; iscan++) {
												bp1.y += b[ilink[i][j_1]].g.yi[iscan];
												bp1.z += b[ilink[i][j_1]].g.zi[iscan];
											}
											bp1.y /= 1.0 * b[ilink[i][j_1]].g.nsizei;
											bp1.z /= 1.0 * b[ilink[i][j_1]].g.nsizei;
											bp1.x = b[ilink[i][j_1]].g.xi[0] + 0.5 * b[ilink[i][j_1]].g.hi[0];
											break;
										}
									}
									else {
										bp1.x = 0.5 * (b[ilink[i][j_1]].g.xS + b[ilink[i][j_1]].g.xE);
										bp1.y = 0.5 * (b[ilink[i][j_1]].g.yS + b[ilink[i][j_1]].g.yE);
										bp1.z = 0.5 * (b[ilink[i][j_1]].g.zS + b[ilink[i][j_1]].g.zE);
									}

									distance = sqrt((bp0.x - bp1.x)*(bp0.x - bp1.x) +
										(bp0.y - bp1.y)*(bp0.y - bp1.y) +
										(bp0.z - bp1.z)*(bp0.z - bp1.z));
									if (distance < 1.0e-12) {
										// ������ �� ������� �� ����.
										// ������� ����� ������������ ����� �� ���� ������.
										// ������ ������ ��������� ������� �������
										distance = fmin(fmin(fabs((b[id[i]].g.xS - b[id[i]].g.xE)),
											fmin(fabs((b[id[i]].g.yS - b[id[i]].g.yE)),
												fabs((b[id[i]].g.zS - b[id[i]].g.zE)))),
											fmin(fabs(b[ilink[i][j_1]].g.xS - b[ilink[i][j_1]].g.xE),
												fmin((fabs(b[ilink[i][j_1]].g.yS - b[ilink[i][j_1]].g.yE)),
													(fabs(b[ilink[i][j_1]].g.zS - b[ilink[i][j_1]].g.zE)))));
									}
								}
								else {
									// ���� id[i] � ������ MCB-ls==ilink[i][j_1].
									//printf("ilink[i][j_1] = %lld, i==%lld j_1==%lld\n", ilink[i][j_1],i,j_1);
									//getchar();

									TOCHKA bp0;
									if (b[id[i]].g.itypegeom == PRISM) {
										bp0.x = 0.5 * (b[id[i]].g.xS + b[id[i]].g.xE);
										bp0.y = 0.5 * (b[id[i]].g.yS + b[id[i]].g.yE);
										bp0.z = 0.5 * (b[id[i]].g.zS + b[id[i]].g.zE);
									}
									else if (b[id[i]].g.itypegeom == CYLINDER) {
										switch (b[id[i]].g.iPlane) {
										case XY_PLANE:
											bp0.x = b[id[i]].g.xC;
											bp0.y = b[id[i]].g.yC;
											bp0.z = b[id[i]].g.zC + 0.5 * b[id[i]].g.Hcyl;
											break;
										case XZ_PLANE:
											bp0.x = b[id[i]].g.xC;
											bp0.z = b[id[i]].g.zC;
											bp0.y = b[id[i]].g.yC + 0.5 * b[id[i]].g.Hcyl;
											break;
										case YZ_PLANE:
											bp0.y = b[id[i]].g.yC;
											bp0.z = b[id[i]].g.zC;
											bp0.x = b[id[i]].g.xC + 0.5 * b[id[i]].g.Hcyl;
											break;
										}
									}
									else if (b[id[i]].g.itypegeom == POLYGON) {
										// ��������� �������������� ����� ��������.
										integer iscan = 0;
										switch (b[id[i]].g.iPlane_obj2) {
										case XY_PLANE:
											bp0.x = 0.0; bp0.y = 0.0;
											for (iscan = 0; iscan < b[id[i]].g.nsizei; iscan++) {
												bp0.x += b[id[i]].g.xi[iscan];
												bp0.y += b[id[i]].g.yi[iscan];
											}
											bp0.x /= 1.0 * b[id[i]].g.nsizei;
											bp0.y /= 1.0 * b[id[i]].g.nsizei;
											bp0.z = b[id[i]].g.zi[0] + 0.5 * b[id[i]].g.hi[0];
											break;
										case XZ_PLANE:
											bp0.x = 0.0; bp0.z = 0.0;
											for (iscan = 0; iscan < b[id[i]].g.nsizei; iscan++) {
												bp0.x += b[id[i]].g.xi[iscan];
												bp0.z += b[id[i]].g.zi[iscan];
											}
											bp0.x /= 1.0 * b[id[i]].g.nsizei;
											bp0.z /= 1.0 * b[id[i]].g.nsizei;
											bp0.y = b[id[i]].g.yi[0] + 0.5 * b[id[i]].g.hi[0];
											break;
										case YZ_PLANE:
											bp0.y = 0.0; bp0.z = 0.0;
											for (iscan = 0; iscan < b[id[i]].g.nsizei; iscan++) {
												bp0.y += b[id[i]].g.yi[iscan];
												bp0.z += b[id[i]].g.zi[iscan];
											}
											bp0.y /= 1.0 * b[id[i]].g.nsizei;
											bp0.z /= 1.0 * b[id[i]].g.nsizei;
											bp0.x = b[id[i]].g.xi[0] + 0.5 * b[id[i]].g.hi[0];
											break;
										}
									}
									else {
										bp0.x = 0.5 * (b[id[i]].g.xS + b[id[i]].g.xE);
										bp0.y = 0.5 * (b[id[i]].g.yS + b[id[i]].g.yE);
										bp0.z = 0.5 * (b[id[i]].g.zS + b[id[i]].g.zE);
									}

									distance = sqrt((bp0.x - 0.5 * (w[ilink[i][j_1]].g.xS + w[ilink[i][j_1]].g.xE)) *
										(bp0.x - 0.5 * (w[ilink[i][j_1]].g.xS + w[ilink[i][j_1]].g.xE)) +
										(bp0.y - 0.5 * (w[ilink[i][j_1]].g.yS + w[ilink[i][j_1]].g.yE)) *
										(bp0.y - 0.5 * (w[ilink[i][j_1]].g.yS + w[ilink[i][j_1]].g.yE)) +
										(bp0.z - 0.5 * (w[ilink[i][j_1]].g.zS + w[ilink[i][j_1]].g.zE)) *
										(bp0.z - 0.5 * (w[ilink[i][j_1]].g.zS + w[ilink[i][j_1]].g.zE)));
								}
								doublereal rho, cp, lam;
								rho = 1.1614; cp = 1005; lam = 0.025; // ������������� default  dry air 300K 1atm properties
								if (matlist[b[id[i]].imatid].blibmat == 1) {
									// ������������, ����������� ������ ��������� AliceFlow ��������.
									if (b[id[i]].itype == SOLID) {
										my_solid_properties(potent[i], rho, cp, lam, matlist[b[id[i]].imatid].ilibident);
										// �������� �� ������������ ����������.
										diagnostic_critical_temperature(potent[i], f, t, b, lb);
									} // SOLID
									if (b[id[i]].itype == FLUID) {
										doublereal mu, beta_t; // �������� �� ������������ �� ���������.
										doublereal pressure = 0.0; // �������� ������ ������� ���� (����� �� ����� ����, �.�. ����� ����������� ��������).

										my_fluid_properties(potent[i], pressure, rho, cp, lam, mu, beta_t, matlist[b[id[i]].imatid].ilibident);
									} // FLUID
								}
								else if (matlist[b[id[i]].imatid].blibmat == 0) {
									// �������� ����������� �������������:
									// ���������� ��������.
									if (!((fabs(matlist[b[id[i]].imatid].orthotropy_multiplyer_x - 1.0) < 0.0001) &&
										(fabs(matlist[b[id[i]].imatid].orthotropy_multiplyer_y - 1.0) < 0.0001) &&
										(fabs(matlist[b[id[i]].imatid].orthotropy_multiplyer_z - 1.0) < 0.0001))) {
										ortho_k1 = true;
										ortho_m1 = fmax(matlist[b[id[i]].imatid].orthotropy_multiplyer_x, fmax(
											matlist[b[id[i]].imatid].orthotropy_multiplyer_y,
											matlist[b[id[i]].imatid].orthotropy_multiplyer_z));
									}
									rho = matlist[b[id[i]].imatid].rho;
									//cp=matlist[b[ib].imatid].cp;
									//lam=matlist[b[ib].imatid].lam;
									cp = get_cp(matlist[b[id[i]].imatid].n_cp, matlist[b[id[i]].imatid].temp_cp, matlist[b[id[i]].imatid].arr_cp, potent[i]);
									lam = get_lam(matlist[b[id[i]].imatid].n_lam, matlist[b[id[i]].imatid].temp_lam, matlist[b[id[i]].imatid].arr_lam, potent[i]);

								}
								doublereal lambda_G = lam;
								if (j_1 < inumber_neighbour_only_body[i]) {
									// ���� �������� � ������.
									if (matlist[b[ilink[i][j_1]].imatid].blibmat == 1) {
										// ������������, ����������� ������ ��������� AliceFlow ��������.
										if (b[ilink[i][j_1]].itype == SOLID) {
											my_solid_properties(potent[i], rho, cp, lam, matlist[b[ilink[i][j_1]].imatid].ilibident);
											// �������� �� ������������ ����������.
											diagnostic_critical_temperature(potent[i], f, t, b, lb);
										} // SOLID
										if (b[ilink[i][j_1]].itype == FLUID) {
											doublereal mu, beta_t; // �������� �� ������������ �� ���������.
											doublereal pressure = 0.0; // �������� ������ ������� ���� (����� �� ����� ����, �.�. ����� ����������� ��������).

											my_fluid_properties(potent[i], pressure, rho, cp, lam, mu, beta_t, matlist[b[ilink[i][j_1]].imatid].ilibident);
										} // FLUID
									}
									else if (matlist[b[ilink[i][j_1]].imatid].blibmat == 0) {
										// �������� ����������� �������������:
										// ���������� ��������.
										if (!((fabs(matlist[b[ilink[i][j_1]].imatid].orthotropy_multiplyer_x - 1.0) < 0.0001) &&
											(fabs(matlist[b[ilink[i][j_1]].imatid].orthotropy_multiplyer_y - 1.0) < 0.0001) &&
											(fabs(matlist[b[ilink[i][j_1]].imatid].orthotropy_multiplyer_z - 1.0) < 0.0001))) {
											ortho_k2 = true;
											ortho_m2 = fmax(matlist[b[ilink[i][j_1]].imatid].orthotropy_multiplyer_x, fmax(
												matlist[b[ilink[i][j_1]].imatid].orthotropy_multiplyer_y,
												matlist[b[ilink[i][j_1]].imatid].orthotropy_multiplyer_z));
										}
										rho = matlist[b[ilink[i][j_1]].imatid].rho;
										//cp=matlist[b[ilink[i][j_1]].imatid].cp;
										//lam=matlist[b[ilink[i][j_1]].imatid].lam;
										cp = get_cp(matlist[b[ilink[i][j_1]].imatid].n_cp, matlist[b[ilink[i][j_1]].imatid].temp_cp, matlist[b[ilink[i][j_1]].imatid].arr_cp, potent[i]);
										lam = get_lam(matlist[b[ilink[i][j_1]].imatid].n_lam, matlist[b[ilink[i][j_1]].imatid].temp_lam, matlist[b[ilink[i][j_1]].imatid].arr_lam, potent[i]);

									}
									lambda_G = 2.0 * lambda_G * lam / (lambda_G + lam);
									if (ortho_k1 && ortho_k2) {// ����� � �����.
															   // ���� ������������� ������������ ����������������.
										lambda_G *= 2.0 * ortho_m1 * ortho_m2 / (ortho_m1 + ortho_m2);
									}
								}
								val[im] = -((lambda_G * dS[i][j_1]) / distance);
								if (j_1 < inumber_neighbour_only_body[i]) {
									// ���� id[i] � ����� ilink[i][j_1].
									col_ind[im] = id_reverse[ilink[i][j_1]]; // ����� �������.
								}
								else {
									col_ind[im] = id_reverse[lb + ilink[i][j_1]];
								}
								sum += ((lambda_G * dS[i][j_1]) / distance);
								im++;
							}


							doublereal rho, cp, lam;
							rho = 1.1614; cp = 1005; lam = 0.025; // ������������� default  dry air 300K 1atm properties
							if (matlist[b[id[i]].imatid].blibmat == 1) {
								// ������������, ����������� ������ ��������� AliceFlow ��������.
								if (b[id[i]].itype == SOLID) {
									my_solid_properties(potent[i], rho, cp, lam, matlist[b[id[i]].imatid].ilibident);
									// �������� �� ������������ ����������.
									diagnostic_critical_temperature(potent[i], f, t, b, lb);
								} // SOLID
								if (b[id[i]].itype == FLUID) {
									doublereal mu, beta_t; // �������� �� ������������ �� ���������.
									doublereal pressure = 0.0; // �������� ������ ������� ���� (����� �� ����� ����, �.�. ����� ����������� ��������).

									my_fluid_properties(potent[i], pressure, rho, cp, lam, mu, beta_t, matlist[b[id[i]].imatid].ilibident);
								} // FLUID
							}
							else if (matlist[b[id[i]].imatid].blibmat == 0) {
								// �������� ����������� �������������:
								// ���������� ��������.
								rho = matlist[b[id[i]].imatid].rho;
								//cp=matlist[b[id[i]].imatid].cp;
								//lam=matlist[b[id[i]].imatid].lam;
								cp = get_cp(matlist[b[id[i]].imatid].n_cp, matlist[b[id[i]].imatid].temp_cp, matlist[b[id[i]].imatid].arr_cp, potent[i]);
								lam = get_lam(matlist[b[id[i]].imatid].n_lam, matlist[b[id[i]].imatid].temp_lam, matlist[b[id[i]].imatid].arr_lam, potent[i]);

							}

							//doublereal Vol = (b[id[i]].g.xE - b[id[i]].g.xS) *
								//(b[id[i]].g.yE - b[id[i]].g.yS) *
								//(b[id[i]].g.zE - b[id[i]].g.zS);
							doublereal Vol = Vol_block[id[i]];

							val[idiag] = sum + (rho*cp*Vol) / timestep_sequence[j]; // ������������ ������������.
							col_ind[idiag] = i;
							idiag = im;
							row_ptr[i + 1] = im;
							im++;
						}
						else {
							// ������
							//((w[id[i]].ifamily == DIRICHLET_FAMILY) ||
							//(w[id[i]].ifamily == NEWTON_RICHMAN_FAMILY) ||
							//(w[id[i]].ifamily == STEFAN_BOLCMAN_FAMILY)||
							//(w[id[i]].ifamily == NEIMAN_FAMILY)))

							if (w[id[i]].ifamily == DIRICHLET_FAMILY) {
								val[idiag] = 1.0; // ������ ������� �������.
								col_ind[idiag] = i;
								row_ptr[i + 1] = idiag + 1;
								idiag++;
							}
							else if ((w[id[i]].ifamily == NEWTON_RICHMAN_FAMILY) ||
								(w[id[i]].ifamily == STEFAN_BOLCMAN_FAMILY)||
								(w[id[i]].ifamily == NEIMAN_FAMILY)) {
								// ������� ����� ����� ib � ������� ������������ ������.
								integer ib = wall2block_link[id[i]];
								integer ic = id_reverse[ib];

								TOCHKA bp0;
								if (b[ib].g.itypegeom == PRISM) {
									bp0.x = 0.5 * (b[ib].g.xS + b[ib].g.xE);
									bp0.y = 0.5 * (b[ib].g.yS + b[ib].g.yE);
									bp0.z = 0.5 * (b[ib].g.zS + b[ib].g.zE);
								}
								else if (b[ib].g.itypegeom == CYLINDER) {
									switch (b[ib].g.iPlane) {
									case XY_PLANE:
										bp0.x = b[ib].g.xC;
										bp0.y = b[ib].g.yC;
										bp0.z = b[ib].g.zC + 0.5 * b[ib].g.Hcyl;
										break;
									case XZ_PLANE:
										bp0.x = b[ib].g.xC;
										bp0.z = b[ib].g.zC;
										bp0.y = b[ib].g.yC + 0.5 * b[ib].g.Hcyl;
										break;
									case YZ_PLANE:
										bp0.y = b[ib].g.yC;
										bp0.z = b[ib].g.zC;
										bp0.x = b[ib].g.xC + 0.5 * b[ib].g.Hcyl;
										break;
									}
								}
								else if (b[ib].g.itypegeom == POLYGON) {
									// ��������� �������������� ����� ��������.
									integer iscan = 0;
									switch (b[ib].g.iPlane_obj2) {
									case XY_PLANE:
										bp0.x = 0.0; bp0.y = 0.0;
										for (iscan = 0; iscan < b[ib].g.nsizei; iscan++) {
											bp0.x += b[ib].g.xi[iscan];
											bp0.y += b[ib].g.yi[iscan];
										}
										bp0.x /= 1.0 * b[ib].g.nsizei;
										bp0.y /= 1.0 * b[ib].g.nsizei;
										bp0.z = b[ib].g.zi[0] + 0.5 * b[ib].g.hi[0];
										break;
									case XZ_PLANE:
										bp0.x = 0.0; bp0.z = 0.0;
										for (iscan = 0; iscan < b[ib].g.nsizei; iscan++) {
											bp0.x += b[ib].g.xi[iscan];
											bp0.z += b[ib].g.zi[iscan];
										}
										bp0.x /= 1.0 * b[ib].g.nsizei;
										bp0.z /= 1.0 * b[ib].g.nsizei;
										bp0.y = b[ib].g.yi[0] + 0.5 * b[ib].g.hi[0];
										break;
									case YZ_PLANE:
										bp0.y = 0.0; bp0.z = 0.0;
										for (iscan = 0; iscan < b[ib].g.nsizei; iscan++) {
											bp0.y += b[ib].g.yi[iscan];
											bp0.z += b[ib].g.zi[iscan];
										}
										bp0.y /= 1.0 * b[ib].g.nsizei;
										bp0.z /= 1.0 * b[ib].g.nsizei;
										bp0.x = b[ib].g.xi[0] + 0.5 * b[ib].g.hi[0];
										break;
									}
								}
								else {
									bp0.x = 0.5 * (b[ib].g.xS + b[ib].g.xE);
									bp0.y = 0.5 * (b[ib].g.yS + b[ib].g.yE);
									bp0.z = 0.5 * (b[ib].g.zS + b[ib].g.zE);
								}

								doublereal distance = sqrt((bp0.x - 0.5 * (w[id[i]].g.xS + w[id[i]].g.xE)) *
									(bp0.x - 0.5 * (w[id[i]].g.xS + w[id[i]].g.xE)) +
									(bp0.y - 0.5 * (w[id[i]].g.yS + w[id[i]].g.yE)) *
									(bp0.y - 0.5 * (w[id[i]].g.yS + w[id[i]].g.yE)) +
									(bp0.z - 0.5 * (w[id[i]].g.zS + w[id[i]].g.zE)) *
									(bp0.z - 0.5 * (w[id[i]].g.zS + w[id[i]].g.zE)));
								doublereal rho, cp, lam;
								rho = 1.1614; cp = 1005; lam = 0.025; // ������������� default  dry air 300K 1atm properties
								if (matlist[b[ib].imatid].blibmat == 1) {
									// ������������, ����������� ������ ��������� AliceFlow ��������.
									if (b[ib].itype == SOLID) {
										my_solid_properties(potent[ic], rho, cp, lam, matlist[b[ib].imatid].ilibident);
										// �������� �� ������������ ����������.
										diagnostic_critical_temperature(potent[ic], f, t, b, lb);
									} // SOLID
									if (b[ib].itype == FLUID) {
										doublereal mu, beta_t; // �������� �� ������������ �� ���������.
										doublereal pressure = 0.0; // �������� ������ ������� ���� (����� �� ����� ����, �.�. ����� ����������� ��������).

										my_fluid_properties(potent[ic], pressure, rho, cp, lam, mu, beta_t, matlist[b[ib].imatid].ilibident);
									} // FLUID
								}
								else if (matlist[b[ib].imatid].blibmat == 0) {
									// �������� ����������� �������������:
									// ���������� ��������.
									rho = matlist[b[ib].imatid].rho;
									//cp=matlist[b[ib].imatid].cp;
									//lam=matlist[b[ib].imatid].lam;
									cp = get_cp(matlist[b[ib].imatid].n_cp, matlist[b[ib].imatid].temp_cp, matlist[b[ib].imatid].arr_cp, potent[ic]);
									lam = get_lam(matlist[b[ib].imatid].n_lam, matlist[b[ib].imatid].temp_lam, matlist[b[ib].imatid].arr_lam, potent[ic]);

								}
								doublereal lambda_G = lam;
								// ��������!!! ������ ���� ������ � ��� �������� � ����� ������.
								//((lambda_G*dS[ic][inumber_neighbour[ic]-1]) / distance);
								val[idiag] = ((lambda_G) / distance);
								col_ind[idiag] = i;//������
								idiag++;
								val[idiag] = -((lambda_G) / distance);
								col_ind[idiag] = ic;//����
								idiag++;
								row_ptr[i + 1] = idiag;
								im = idiag + 1;
								if (w[id[i]].ifamily == NEWTON_RICHMAN_FAMILY) {

									//printf("nonlinear:  potent[ic]=%e  potent[i]=%e Tamb=%e\n", potent[ic], potent[i], w[id[i]].Tamb);
									//printf("lambda_G=%e distance=%e w[id[i]].film_coefficient=%e\n", lambda_G, distance, w[id[i]].film_coefficient);
									//getchar();
									if (potent[i] > w[id[i]].Tamb) {
										rthdsd[i] = -w[id[i]].film_coefficient * (potent[i] - w[id[i]].Tamb); // ������� �������-�������.
									}
									else {
										rthdsd[i] = 0.0;
										//rthdsd[i] = -w[id[i]].film_coefficient * (2.0); // ������� �������-�������.
									}
								}
								if (w[id[i]].ifamily == STEFAN_BOLCMAN_FAMILY) {
									// ������� ������� - ���������.
									//printf("nonlinear:  potent[ic]=%e  potent[i]=%e Tamb=%e\n", potent[ic], potent[i], w[id[i]].Tamb);
									//printf("lambda_G=%e distance=%e w[id[i]].emissivity=%e\n", lambda_G, distance, w[id[i]].emissivity);
									//getchar();
									//if (potent[i] > w[id[i]].Tamb) ���� �����, ����������.
									//if (potent[i] < w[id[i]].Tamb) ���� ����, ��������.
									{
										rthdsd[i] = -w[id[i]].emissivity * w[id[i]].ViewFactor *STEFAN_BOLCMAN_CONST * (((273.15 + potent[i]) *
											(273.15 + potent[i]) * (273.15 + potent[i]) * (273.15 + potent[i]) -
											(273.15 + w[id[i]].Tamb) * (273.15 + w[id[i]].Tamb) * (273.15 + w[id[i]].Tamb) *
											(273.15 + w[id[i]].Tamb)));
									}
									
								}

								if (w[id[i]].ifamily == NEIMAN_FAMILY) {
									// ���������� ������� �������
									rthdsd[i] = 0.0;
								}
							}
						}
					}

					integer n = maxelm + lw;

					// One iteration
					r2=residual_network(n, maxelm, rthdsd, potent, val, col_ind, row_ptr, b_nonlinear_network, id, w);
					//Seidel_network(n, maxelm, rthdsd, potent, val, col_ind, row_ptr, b_nonlinear_network, id, w);

					//printf("nnz=%lld row_ptr[n]=%lld\n",nnz, row_ptr[n]);

					/*
					// ����������
					for (integer i_1 = 0; i_1 < n; i_1++) {
					for (integer j_2 = row_ptr[i_1] + 1; j_2 < row_ptr[i_1 + 1]; j_2++) {
					val[j_2] /= val[row_ptr[i_1]];
					}
					rthdsd[i_1] /= val[row_ptr[i_1]];
					val[row_ptr[i_1]] = 1.0;
					}
					*/

					check_CRS_matrix(n, nnz, val, col_ind, row_ptr, rthdsd, potent, b, lb, id, w, lw, maxelm);
					//print_CRS_matrix(n, nnz, val, col_ind, row_ptr, rthdsd, potent);
					if (b_nonlinear_network) {
						 
						 

						doublereal alpha = 0.98; // ������ ����������.
						if (b_Newton_Richman) {
							// ��������� ������� ������� - �������
							// Ok ���������� ����������.
						}

						if (b_Stefan_Bolcman) {
							// ������������ ������� - ���������, ����� ����� ������� ����������.
							// ���� (0.2 ���������) 0.1
							// 0.7 ��������� �������� ��� �������������� ��������.
							alpha = 0.78;// 0.7;//0.01; 0.1-BSK-Dmitrii ��������; 0.8; 0.98;
						}

						// ������ ���������� �������� � ������� ����.
						// 0.1
						// 0.4 ��������.
						// 0.8 ������.
						// 0.95 BSK-Dmitrii ��������.
						// ���� ������ ���������.
						if ((b_Newton_Richman) || (b_Stefan_Bolcman)) {
							// ���� 0.4
							doublereal alphaA = 0.9; // 0.01; ������ ����������.
							for (integer i_1 = 0; i_1 < maxelm; i_1++) {
								// ��� ����� ����� ������� ������ ������� ����.
								rthdsd[i_1] += (1 - alphaA) * (val[row_ptr[i_1]] / alphaA) * potent_old[i_1];
								val[row_ptr[i_1]] = val[row_ptr[i_1]] / alphaA;
							}
						}
						

						for (integer i25 = 0; i25 < n; i25++) potent_old[i25] = potent[i25];

						//bool worked_successfully;
						//amg_loc_memory_networkT(val, col_ind, row_ptr, n, nnz,
							//rthdsd, potent, 1.0, true, 0, worked_successfully,
							//b, lb, ls, maxelm,false); // false - ������ �� ����������.
						//Bi_CGStabCRS(n, val, col_ind, row_ptr, rthdsd, potent, 2000);

						// �������� ������ ��������.
						amgcl_networkT_solver(val, col_ind, row_ptr, n, rthdsd, potent, false);

						for (integer i25 = 0; i25 < n; i25++) {
							potent[i25] = potent_old[i25] + alpha * (potent[i25] - potent_old[i25]);
							//if (potent[i25] < t.operatingtemperature) potent[i25] = t.operatingtemperature;
						}
						//getchar();
					}
					else {
						//Bi_CGStabCRS(n, val, col_ind, row_ptr, rthdsd, potent, 2000);
						bool worked_successfully;
						amg_loc_memory_networkT(val, col_ind, row_ptr, n, nnz,
							rthdsd, potent, 1.0, true, 0, worked_successfully,
							b, lb, ls, maxelm,false);
					}
					//getchar();

				}

				delete[] val;
				delete[] col_ind;
				delete[] row_ptr;


				delete[] potent_old;

				/*
				���������� ������� ���� �� ��������� ����:
				*/

				for (integer i = 0; i < maxelm + lw; i++) {
					if (potent[i] < -273.15) {
						potent[i] = -273.15; // �������������� ���������� ����.
					}
				}

				// ���������� ����������� �� �����.
				for (integer i = 0; i < t.maxelm; i++) {
					if (block_is_active[t.whot_is_block[i]]) {
						t.potent[i] = potent[id_reverse[t.whot_is_block[i]]];
					}
				}
				for (integer i = 0; i < t.maxbound; i++) {
					// �������� ����������� �� ���������� ����������� ����.
					t.potent[t.maxelm + i] = t.potent[t.border_neighbor[i].iI];
				}

				if ((glTSL.id_law == 2) && (j == 1039)) {
					// 29_11_2017

				

					// ��������� ������ ����� 6 ��������� �� �������� �����.
					// ������� � �������.
					if (!b_on_adaptive_local_refinement_mesh) {
						bool bextendedprint_1 = false;
						exporttecplotxy360T_3D_part2_apparat_hot(t.maxelm, t.ncell, fglobal, t, flow_interior, 0, bextendedprint_1, 1, b);
					}
					else {
						// ������� � ����
						// ������� � ��������� tecplot �����������.
						//� ���� �����.
						ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, fglobal, 1, b, lb);
					}

				}
				for (integer i = 0; i < maxelm + lw; i++) toldtimestep[i] = potent[i]; // copy

				if (!b_on_adaptive_local_refinement_mesh) {
					if (bfirst_export && (phisicaltime > 287990)) {
						bfirst_export = false;

						// ���������� ����������� �� �����.
						for (integer i = 0; i < t.maxelm; i++) {
							if (block_is_active[t.whot_is_block[i]]) {
								t.potent[i] = potent[id_reverse[t.whot_is_block[i]]];
							}
						}
						for (integer i = 0; i < t.maxbound; i++) {
							// �������� ����������� �� ���������� ����������� ����.
							t.potent[t.maxelm + i] = t.potent[t.border_neighbor[i].iI];
						}

						// ��������� ������ ����� 6 ��������� �� �������� �����.
						// ������� � �������.
						bool bextendedprint_1 = false;
						exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, 0, bextendedprint_1, 1, b, lb);

					}
				}

				if (ianimation_write_on == 1) {

					// ������ ���������� ������������� �����.
					if (!b_on_adaptive_local_refinement_mesh) {

						// ���������� ����������� �� �����.
						for (integer i = 0; i < t.maxelm; i++) {
							if (block_is_active[t.whot_is_block[i]]) {
								t.potent[i] = potent[id_reverse[t.whot_is_block[i]]];
							}
						}
						for (integer i = 0; i < t.maxbound; i++) {
							// �������� ����������� �� ���������� ����������� ����.
							t.potent[t.maxelm + i] = t.potent[t.border_neighbor[i].iI];
						}

						// ������ ������������ ������.
						bool bextendedprint_1 = false;
						integer inumbercadr = j;
						exporttecplotxy360T_3D_part2_ianimation_series(t.maxelm, t.ncell, fglobal, t, flow_interior, 0, bextendedprint_1, 1, inumbercadr, phisicaltime, b);
					}

				}

				// ���������� ����������� �� �����.
				for (integer i = 0; i < t.maxelm; i++) {
					if (block_is_active[t.whot_is_block[i]]) {
						t.potent[i] = potent[id_reverse[t.whot_is_block[i]]];
					}
				}
				for (integer i = 0; i < t.maxbound; i++) {
					// �������� ����������� �� ���������� ����������� ����.
					t.potent[t.maxelm + i] = t.potent[t.border_neighbor[i].iI];
				}

				// ��������� ����� � ����������� ������� ������� �� ������� ������� ������:
				report_temperature_for_unsteady_modeling(0, fglobal, t, b, lb, s, ls, w, lw, 0, phisicaltime, poweron_multiplier_sequence[j], t.operatingtemperature);

				doublereal tmaxi = -1.0e10; // ������������ ����������� ��� ���������� ��.

				doublereal tmaxavg = -273.15;
				doublereal *nullpointer = nullptr;

				for (integer i = 0; i < maxelm; i++) tmaxavg = fmax(tmaxavg, potent[i]);

				doublereal Pdiss = 0.0; // �������� ������������ � �����.
				doublereal tmaxall = tmaxi; // ������������ ����������� ��� ���� �� ���������� � ���������.

				integer ifindloc = 0; // ������� �� ����� ��� ������� ������������ �����������.
				for (integer i = 0; i < maxelm; i++) {
					//tmaxi=fmax(tmaxi,t.potent[i]);
					if (potent[i] > tmaxi) {
						tmaxi = potent[i];
						ifindloc = i; // ���������� ������� ���������.
					}
				}

				for (integer i = maxelm; i < maxelm + lw; i++) tmaxall = fmax(tmaxall, potent[i]);


				for (integer isource = 0; isource < ls; isource++) {
					Pdiss += s[isource].power;
				}

				// 19 november 2016.
				// ���������� �������� �������������� �� ���� ���������� �����.

				for (integer i47 = 0; i47 < t.maxelm; i47++) {
					// �������� � ��� ��� �������� �� ����������� ��� ������ � ������ ��������.
					integer ib = t.whot_is_block[i47];
					t.Sc[i47] = get_power(b[ib].n_Sc, b[ib].temp_Sc, b[ib].arr_Sc, t.potent[i47]);
					// ���������� �������� �������� ������������ ������:
					doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������ ������
					volume3D(i47, t.nvtx, t.pa, dx, dy, dz);
					Pdiss += t.Sc[i47] * dx*dy*dz;
				}

				printf("Pdiss=%e\n", Pdiss); // �������� ������������ � ����� � ������������ ���� �� ������� ����������.
				if (fabs(Pdiss) < 1.0e-30) {
					Pdiss = 1.0; // ����� �������� ������ RT ��������.
					printf("Warning !!! Pdissipation Energy is equal zero (calculation source object).\n");
					printf("Pdiss_virtual:=1.0; RT==DeltaT==(Tmax-Tamb)/1.0;\n");
					printf("Please, press any key to continue...\n");
					//getchar();
					system("pause");
				}
				//getchar();

				fprintf(fpcurvedata, "%+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f\n", phisicaltime, tmaxall, (tmaxall - Tamb) / Pdiss, tmaxi, (tmaxi - Tamb) / Pdiss, tmaxavg, (tmaxavg - Tamb) / Pdiss);
				if (glTSL.id_law == 0) {
					// Linear.
					if (evdokimova_report != nullptr) {
						evdokimova_report[j + 1][0] = phisicaltime; evdokimova_report[j + 1][1] = tmaxall; evdokimova_report[j + 1][2] = (tmaxall - Tamb) / Pdiss;
						evdokimova_report[j + 1][6] = phisicaltime; evdokimova_report[j + 1][7] = tmaxi;  evdokimova_report[j + 1][8] = (tmaxi - Tamb) / Pdiss;
						evdokimova_report[j + 1][12] = phisicaltime; evdokimova_report[j + 1][13] = tmaxavg; evdokimova_report[j + 1][14] = (tmaxavg - Tamb) / Pdiss;
					}
				}
				fprintf(fpKras, "%+.16f %+.16f\n", phisicaltime, tmaxi); // tmaxall
				if (glTSL.id_law == 1) {
					// ������ ���� square wave.
					if ((j + 1 - 10) % 20 == 0) {
#ifdef MINGW_COMPILLER
						fpKras_max = fopen64("inputKras_max.txt", "a");
#else
						err23_max = fopen_s(&fpKras_max, "inputKras_max.txt", "a");
#endif
						if ((err23_max == 0) && (fpKras_max != NULL)) {
							fprintf(fpKras_max, "%+.16f %+.16f\n", phisicaltime, tmaxi);
							fclose(fpKras_max);
						}
					}
					if ((j + 1) % 20 == 0) {
#ifdef MINGW_COMPILLER
						fpKras_min = fopen64("inputKras_min.txt", "a");
#else
						err23_min = fopen_s(&fpKras_min, "inputKras_min.txt", "a");
#endif
						if ((err23_min == 0) && (fpKras_min != NULL)) {
							fprintf(fpKras_min, "%+.16f %+.16f\n", phisicaltime, tmaxi);
							fclose(fpKras_min);
						}
					}
				}
				printf("complete is: %3.0f %% \n", (doublereal)(100.0*(j + 1) / iN)); // ���������� ������� ��������� ���������.
			}

			fclose(fpcurvedata); // �������� ����� ��� ������ ������ ��������.
		}
		fclose(fpKras); // �������� ����� ��� ������ ������ �������� � ������� ��� ������������ ����.
	}


	if (toldtimestep != nullptr) {
		delete[] toldtimestep;
	}
	if (timestep_sequence != nullptr) {
		delete[] timestep_sequence;
	}
	if (poweron_multiplier_sequence != nullptr) {
		delete[] poweron_multiplier_sequence;
	}

	if (glTSL.id_law == 0) {
		// Linear.
		// ������������ ������ �.�. ����������.
		// C dC/dRt C/Rt
		evdokimova_report[0][3] = 0.0;  evdokimova_report[0][5] = 0.0;
		evdokimova_report[0][9] = 0.0;  evdokimova_report[0][11] = 0.0;
		evdokimova_report[0][15] = 0.0; evdokimova_report[0][17] = 0.0;
		for (integer i = 1; i < iN; i++) {
			// ��� 1.
			evdokimova_report[i][3] = evdokimova_report[i][0] / evdokimova_report[i][2];  evdokimova_report[i][5] = evdokimova_report[i][3] / evdokimova_report[i][2];
			evdokimova_report[i][9] = evdokimova_report[i][6] / evdokimova_report[i][8];  evdokimova_report[i][11] = evdokimova_report[i][9] / evdokimova_report[i][8];
			evdokimova_report[i][15] = evdokimova_report[i][12] / evdokimova_report[i][14];  evdokimova_report[i][17] = evdokimova_report[i][15] / evdokimova_report[i][14];
		}
		for (integer i = 0; i < iN; i++) {
			// ��� 2.
			// ������ ��� ������ ���������� ����� ���� 1, �.�. �� ������� �� ����������� ����1.
			if (fabs(evdokimova_report[i + 1][2] - evdokimova_report[i][2]) < 1e-30) {
				// ���� ����������� ����� ����, �� � ��������� ����� ���� � ������ �� ����� ��������������� 0 �� 0 ������� ��������� ������� ���������.
				evdokimova_report[i][4] = 0.0;
			}
			else {
				evdokimova_report[i][4] = (evdokimova_report[i + 1][3] - evdokimova_report[i][3]) / (evdokimova_report[i + 1][2] - evdokimova_report[i][2]);
			}
			if (fabs(evdokimova_report[i + 1][8] - evdokimova_report[i][8]) < 1.0e-30) {
				evdokimova_report[i][10] = 0.0;
			}
			else {
				evdokimova_report[i][10] = (evdokimova_report[i + 1][9] - evdokimova_report[i][9]) / (evdokimova_report[i + 1][8] - evdokimova_report[i][8]);
			}
			if (fabs(evdokimova_report[i + 1][14] - evdokimova_report[i][14]) < 1.0e-30) {
				evdokimova_report[i][16] = 0.0;
			}
			else {
				evdokimova_report[i][16] = (evdokimova_report[i + 1][15] - evdokimova_report[i][15]) / (evdokimova_report[i + 1][14] - evdokimova_report[i][14]);
			}
		}
		evdokimova_report[iN][3] = evdokimova_report[iN][0] / evdokimova_report[iN][2];
		evdokimova_report[iN][5] = evdokimova_report[iN][3] / evdokimova_report[iN][2];
		if (fabs(evdokimova_report[iN][2] - evdokimova_report[iN - 1][2]) < 1.0e-30) {
			// ��������������� ���� �� ����.
			evdokimova_report[iN][4] = 0.0;
		}
		else {
			evdokimova_report[iN][4] = (evdokimova_report[iN][3] - evdokimova_report[iN - 1][3]) / (evdokimova_report[iN][2] - evdokimova_report[iN - 1][2]);
		}

		evdokimova_report[iN][9] = evdokimova_report[iN][6] / evdokimova_report[iN][8];
		evdokimova_report[iN][11] = evdokimova_report[iN][9] / evdokimova_report[iN][8];
		if (fabs(evdokimova_report[iN][8] - evdokimova_report[iN - 1][8]) < 1.0e-30) {
			// ���������� ��������������� ���� �� ����.
			evdokimova_report[iN][10] = 0.0;
		}
		else {
			evdokimova_report[iN][10] = (evdokimova_report[iN][9] - evdokimova_report[iN - 1][9]) / (evdokimova_report[iN][8] - evdokimova_report[iN - 1][8]);
		}

		evdokimova_report[iN][15] = evdokimova_report[iN][12] / evdokimova_report[iN][14]; // C==time/Rt;
		evdokimova_report[iN][17] = evdokimova_report[iN][15] / evdokimova_report[iN][14];
		if (fabs(evdokimova_report[iN][14] - evdokimova_report[iN - 1][14]) < 1.0e-30) {
			// ��������� ��������������� ���� �� ����.
			evdokimova_report[iN][16] = 0.0;
		}
		else {
			evdokimova_report[iN][16] = (evdokimova_report[iN][15] - evdokimova_report[iN - 1][15]) / (evdokimova_report[iN][14] - evdokimova_report[iN - 1][14]);
		}

		// ������ ������ � ��������� ����:
		FILE *fpevdokimova = NULL;
#ifdef MINGW_COMPILLER
		err = 0;
		fpevdokimova = fopen64("Evdokimova.txt", "w");
		if (fpevdokimova == NULL) err = 1;
#else
		err = fopen_s(&fpevdokimova, "Evdokimova.txt", "w");
#endif
		if ((err) != 0) {
			printf("Create File Evdokimova.txt Error\n");
			// getchar();
			system("pause");
			exit(0);
		}
		else {
			if (fpevdokimova != NULL) {
				fprintf(fpevdokimova, "time Tch_all RTch_all Cchall dCchall/dRt_chall Cchall/Rtchall time Tch_in RTch_in Cchin dCchin/dRt_chin Cchin/Rtchin time Tch_avg RTch_avg Cchavg dCchavg/dRt_chavg Cchavg/Rtchavg \n");
				for (integer i = 0; i <= iN; i++) {
					fprintf(fpevdokimova, "%+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f %+.16f\n", evdokimova_report[i][0], evdokimova_report[i][1], evdokimova_report[i][2], evdokimova_report[i][3], evdokimova_report[i][4], evdokimova_report[i][5], evdokimova_report[i][6], evdokimova_report[i][7], evdokimova_report[i][8], evdokimova_report[i][9], evdokimova_report[i][10], evdokimova_report[i][11], evdokimova_report[i][12], evdokimova_report[i][13], evdokimova_report[i][14], evdokimova_report[i][15], evdokimova_report[i][16], evdokimova_report[i][17]);
				}
				fclose(fpevdokimova); // ��������� ����.
			}
		}

		if (evdokimova_report != nullptr) {
			for (integer i = 0; i < iN + 1; i++) {
				delete[] evdokimova_report[i];
			}
			delete[] evdokimova_report;
			evdokimova_report = nullptr;
		}

	}

	if (evdokimova_report != nullptr) {
		for (integer i = 0; i < iN + 1; i++) {
			delete[] evdokimova_report[i];
		}
		delete[] evdokimova_report;
		evdokimova_report = nullptr;
	}

	// ���������� ����������� �� �����.
	for (integer i = 0; i < t.maxelm; i++) {
		if (block_is_active[t.whot_is_block[i]]) {
			t.potent[i] = potent[id_reverse[t.whot_is_block[i]]];
		}
	}
	for (integer i = 0; i < t.maxbound; i++) {
		// �������� ����������� �� ���������� ����������� ����.
		t.potent[t.maxelm + i] = t.potent[t.border_neighbor[i].iI];
	}

	delete[] id;
	delete[] id_reverse;
	delete[] hash;
	delete[] hash_wall;
	delete[] wall2block_link;
	delete[] block_is_active;
	delete[] Vol_block; 
	delete[] inumber_neighbour;
	delete[] inumber_neighbour_only_body;
	for (integer i = 0; i < maxelm + lw; i++) {
		delete[] dS[i];
		delete[] ilink[i];
		delete[] ilink_reverse[i];
	}
	delete[] dS;
	delete[] ilink;
	delete[] ilink_reverse;

	delete[] potent; // ������ �������.
	delete[] rthdsd; // ������ �����.

					 // ��������� � ��� 23 ������ 2016 ����.
	calculation_end_time = clock();
	calculation_seach_time = calculation_end_time - calculation_start_time;
	unsigned int im = 0, is = 0, ims = 0;
	im = (unsigned int)(calculation_seach_time / 60000); // ������
	is = (unsigned int)((calculation_seach_time - 60000 * im) / 1000); // �������
	ims = (unsigned int)((calculation_seach_time - 60000 * im - 1000 * is) / 10); // ������������ ������� �� 10

	printf("time calculation is:  %u minute %u second %u 10*millisecond\n", im, is, ims);

} // calculate_Network_T_unsteady


void my_malloc2(doublereal** &rhie_chow, integer maxelm) {
	rhie_chow=new doublereal*[3];
	for (integer i=0; i<3; i++) rhie_chow[i]=new doublereal[maxelm];
} // my_malloc2

// ����� ������
integer KRstrlen( const char *s)
{
	integer n = 0;
	while (*s++)
		n++;
	return(n);
}

/* reverse:  �������������� ������ s �� ����� */
 void KRreverse(char s[])
 {
     integer i, j;
     char c;
 
     for (i = 0, j = KRstrlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }


/* itoa:  ������������ n � ������� � s */
 void KRitoa(integer n, char* &s)
 {
     integer i, sign;
 
     if ((sign = n) < 0)  /* ���������� ���� */
         n = -n;          /* ������ n ������������� ������ */
     i = 0;
     do {       /* ���������� ����� � �������� ������� */
         s[i++] = n % 10 + '0';   /* ����� ��������� ����� */
     } while ((n /= 10) > 0);     /* ������� */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     KRreverse(s);
 }

 char * KRstrcat ( char * &destination, const char * source )
{
     char *d = destination;
     while (*d) ++d;
     while ((*d++ = *source++) != '\0') ;
     return (destination);
}

 


void steady_cfd_calculation(bool breadOk, EQUATIONINFO &eqin, 
	                        doublereal dgx, doublereal dgy, doublereal dgz,
							doublereal* continity_start, 
							integer* inumber_iteration_SIMPLE,
							integer flow_interior, 
							FLOW* &fglobal, TEMPER &t,
							BLOCK* b, integer lb, SOURCE* s, integer ls,
							WALL* w, integer lw, TPROP* matlist,
							TEMP_DEP_POWER* gtdps, integer ltdp, bool bextendedprint,
	                        integer lu, UNION* &my_union,integer inx, doublereal* &xpos)
{


	integer* color = nullptr;
	
	if ((!b_on_adaptive_local_refinement_mesh) && (number_cores() == 2) && (my_amg_manager.lfil < 3)) {
		// �������� ������ ��� ����������������� �����.
		integer isize = 0;
		//if ((iVar == VX) || (iVar == VY) || (iVar == VZ) || (iVar == PAM) || (iVar == NUSHA) ||
		//(iVar == TURBULENT_KINETIK_ENERGY) || (iVar == TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA) ||
		//(iVar == TURBULENT_KINETIK_ENERGY_STD_K_EPS) || (iVar == TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS))
		{
			integer n = fglobal[0].maxelm + fglobal[0].maxbound;
			color = new integer[n];
			for (integer i_1 = 0; i_1 < n; i_1++) color[i_1] = 0; // initialization
																  // ����� �� ����.
			doublereal max = -1.0e60;
			//doublereal min = 1.0e60;
			/*
			for (integer i = 0; i < fglobal[0].maxelm; i++) {
				TOCHKA point0;
				center_cord3D(i, fglobal[0].nvtx, fglobal[0].pa, point0, 100);
				if (point0.x > max) max = point0.x;
				if (point0.x < min) min = point0.x;
			}
			doublereal avg = 0.5 * (min + max);
			*/
			doublereal avg = xpos[(integer)(0.5*inx)];

			doublereal dx = 0.0, dy = 0.0, dz = 0.0; // ����� �������� ������������ ������
			integer iP = -1;
			max = 1.0e60;
			for (integer i = 0; i < fglobal[0].maxelm; i++) {
				TOCHKA point0;
				center_cord3D(i, fglobal[0].nvtx, fglobal[0].pa, point0, 100);
				if (fabs(avg - point0.x) < max) {
					max = fabs(avg - point0.x);
					//min = point0.x;
					iP = i;
				}
			}

			integer il = 0, ic = 0, ir = n;
			bool bcontinue = true;
			while ((bcontinue) && (abs(ir - il) > 1.4 * ic)) {

				isize = 0;
				il = 0; ir = 0; ic = 0;// �������������.

				TOCHKA point1;
				center_cord3D(iP, fglobal[0].nvtx, fglobal[0].pa, point1, 100);
				avg = point1.x;
				volume3D(iP, fglobal[0].nvtx, fglobal[0].pa, dx, dy, dz);
				dx = fabs(dx);
				dy = fabs(dy);
				dz = fabs(dz);

				for (integer i = 0; i < fglobal[0].maxelm; i++) {
					TOCHKA point0;
					center_cord3D(i, fglobal[0].nvtx, fglobal[0].pa, point0, 100);
					if (point0.x < avg - 0.4 * dx) {
						color[i] = 1;
						il++;
					}
					else if (point0.x > avg + 0.4 * dx) {
						color[i] = 3;
						ir++;
					}
					else {
						color[i] = 2;
						isize++;
						ic++;
					}
				}
				for (integer iB = 0; iB < fglobal[0].maxbound; iB++) {
					integer i = fglobal[0].border_neighbor[iB].iI;
					if ((i >= 0) && (i < fglobal[0].maxelm)) {
						TOCHKA point0;
						center_cord3D(i, fglobal[0].nvtx, fglobal[0].pa, point0, 100);
						if (point0.x < avg - 0.4 * dx) {
							color[fglobal[0].maxelm + iB] = 1;
							il++;
						}
						else if (point0.x > avg + 0.4 * dx) {
							color[fglobal[0].maxelm + iB] = 3;
							ir++;
						}
						else {
							color[fglobal[0].maxelm + iB] = 2;
							isize++;
							ic++;
						}
					}
					else {
						printf("error iI =%lld\n",i);
						system("pause");
					}
				}

				printf("ileft=%lld center=%lld right=%lld\n", il, ic, ir);
				if (ir > il) {
					// ���� ���� fglobal[0].neighbors_for_the_internal_node[ESIDE][iP].iNODE1; ����������.
					integer icP = fglobal[0].neighbors_for_the_internal_node[E_SIDE][iP].iNODE1;
					if ((icP >= 0) && (icP < fglobal[0].maxelm)) {
						iP = icP;
					}
					else {
						bcontinue = false;
					}
				}
				else if (ir < il) {
					// ���� ���� fglobal[0].neighbors_for_the_internal_node[WSIDE][iP].iNODE1; ����������.
					integer icP = fglobal[0].neighbors_for_the_internal_node[W_SIDE][iP].iNODE1;
					if ((icP >= 0) && (icP < fglobal[0].maxelm)) {
						iP = icP;
					}
					else {
						bcontinue = false;
					}
				}
			}
		}

		printf("separator size=%lld\n", isize);
		//getchar();
	}

	integer* color_solid = nullptr;
	integer dist_max_solid = 3;
	calculate_color_for_temperature(color_solid, t,inx,xpos);

	if ((bSIMPLErun_now_for_temperature) && ((fabs(dgx) > 1.0e-20) || (fabs(dgy) > 1.0e-20) || (fabs(dgz) > 1.0e-20))) {
		// ����������� ���������.
		// ��� ������������� ����������� ��������� �� �� ���������� �������������� rGradual_changes
		rGradual_changes = 1.0;
	}

	// ��������� RCh ��� �������� ���-��� ����������� ������ ���� ����� 1.0 ����� ��������� ��������� ����������.
	// �� ��� � ��������� ������������ ���������� ������������� ���������� ��������� ��� �������� ���-��� ������ 0.1
	// (��� ���������� ��������� ����� �������� ���-��� � 10 ���) �� ���������� ������������:
	// ��. ��������� ��������� � �������� ������.
	doublereal RCh=1.0; // 1.0; 0.1;
	//RCh = my_amg_manager.F_to_F_Stress;//debug

	if (0) {
		xyplot( fglobal, flow_interior, t);
		printf("steady cfd calc presolve. OK.\n");
	    //getchar(); // debug
		system("pause");
	}

	FLUENT_RESIDUAL rfluentres;
	rfluentres.operating_value_b=1.0; // ������������� ��������� ��������.
	doublereal rfluentrestemp=1.0; // ������� � ����� fluent ��� �����������.

	// ����� �������.
	unsigned int calculation_start_time=0; // ������ ����� ��.
	unsigned int calculation_end_time=0; // ��������� ����� ��.
	unsigned int calculation_seach_time=0; // ����� ���������� ������� ���� � ��.

	// ��� ������������ ������������� ����������� ��������.
	bool bprintmessage=false; //true; // �������� �� ��������� �� �������.

	// ������ ���������� ����������,
    // ��������� ��� �������� ������� ���-���
    doublereal **rhie_chow=nullptr;

	///* 
    FILE *fpcont=NULL; // ���� � ������� ����� ������������ �������
	errno_t err;
	
	
	// �������� ����� ��� ������ �������� ������� continity
	// continity - ������������������ ��������� ����� ������� 
	// ������ ���� ���������������.
	// continity - ���������� ���������� ���� ������� ����������������� ���������.
	bool bcontinuecontinity=false;
#ifdef MINGW_COMPILLER
	err = 0;
	if (!breadOk) {
		// ���������� �� ����� avtosave.txt �� �������
		fpcont = fopen64("continity.txt", "w");
	}
	else {
		// �������� ���� ������� �� ����� avtosave.txt
		fpcont = fopen64("continity.txt", "a");
	}
	if (fpcont == NULL) err = 1;
#else
	if (!breadOk) {
		// ���������� �� ����� avtosave.txt �� �������
		err = fopen_s(&fpcont, "continity.txt", "w");
	}
	else {
		// �������� ���� ������� �� ����� avtosave.txt
		err = fopen_s(&fpcont, "continity.txt", "a");
	}
#endif
	
	if (err == 0)  bcontinuecontinity=true;
	else {
         printf("Create File continity.txt Error\n");
         //getchar();
		 system("pause");
         exit(0);
	}
	
	

	
	if (bcontinuecontinity) {

		if (flow_interior>0) {

			// � ���� statistic_convergence.txt ����� ������������ ���������� � ������
            // �������� ��������� ����.
            FILE *fp_statistic_convergence=NULL;
						
			errno_t err_stat=0;
#ifdef MINGW_COMPILLER
			fp_statistic_convergence=fopen64("statistic_convergence.txt", "a");
			if (fp_statistic_convergence == NULL) err_stat = 1;
#else
			err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
			

	        // �������� ����� ��� ������ �������� ������� � 
			// � �������� �������� �������� ����.
			// �������� ��������� ������� ��������� ������ � �������� ���������� ��� ������������ ���� ������� ����������������� ���������.
			// (�.�. ��� �������� ������� � �������� ���� ����������� �������� ����� dterminatedTResudual)
	        if ((err_stat) !=0) {
	            printf("Create File continity.txt Error\n");
                //getchar();
				system("pause");
                exit(0);
 	        }
	        else {

				

			      for (integer iflow=0; iflow<flow_interior; iflow++) {
					  // ���� ������ ����������������� ���������� ��������� ����������:
					  if (eqin.fluidinfo[iflow].iflow == 1) {
						  // �������������� ����������������� ���������� � ������� iflow.

						  if (fglobal[iflow].bLR1free) {
							  // ��� ������ ����������������� ���������� �� ��� ��������� ����� ���������� ������� ������� ���
							  // �������� ��������. �� ���� ������� ������������ ������������.
							  printf("WARNING! bLR1free is true. All neiman condition for PAmendment.\n");
							  // getchar();
						  }

						  bool btimedep = false;

						  // ��������� ����������� ������ ��� �������� Rhie-Chow 1983.
						  my_malloc2(rhie_chow, fglobal[iflow].maxelm);

#if doubleintprecision == 1
						  fprintf(fpcont, " Evalution residual for flow interior=%lld\n", iflow);
						  fprintf(fpcont, " iter \t\t continity\n");
						  fprintf(fp_statistic_convergence, " Statistic convergence for flow interior=%lld\n", iflow);
#else
						  fprintf(fpcont, " Evalution residual for flow interior=%d\n", iflow);
						  fprintf(fpcont, " iter \t\t continity\n");
						  fprintf(fp_statistic_convergence, " Statistic convergence for flow interior=%d\n", iflow);
#endif
						  if (fglobal[0].iflowregime == RANS_STANDART_K_EPS) {
							  if (eqin.itemper == 1) {
								  fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM     energy      k		epsilon	\n");
							  }
							  else {
								  fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM     k		epsilon \n");
							  }
						  }
						  else
						  if (fglobal[0].iflowregime == RANS_MENTER_SST) {
							  if (eqin.itemper == 1) {
								  fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM     energy      k		omega	\n");
							  }
							  else {
								  fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM     k		omega \n");
							  }
						  }
						  else  if (fglobal[0].iflowregime == RANS_SPALART_ALLMARES) {
							  if (eqin.itemper == 1) {
								  fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM     energy      nut	\n");
							  }
							  else {
								  fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM     nut	\n");
							  }
						  }
						  else {
							  if (eqin.itemper == 1) {
								  fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM     energy      \n");
							  }
							  else {
								  fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM     \n");
							  }
						  }
						  fclose(fp_statistic_convergence);


						  doublereal continity = 1.0; // �������������

#if doubleintprecision == 1
						  printf("fluid interior number %lld: maxelm=%lld, maxbound=%lld\n", iflow, fglobal[iflow].maxelm, fglobal[iflow].maxbound);
#else
						  printf("fluid interior number %d: maxelm=%d, maxbound=%d\n", iflow, fglobal[iflow].maxelm, fglobal[iflow].maxbound);
#endif
						  printf("please, press any key to start calculation...\n");
						  if (bwait) {
							  //getchar();
							  system("pause");
						  }
						  calculation_start_time = clock(); // ������ ������ �����.
						  bool bfirst = true;
						  doublereal* smagconstolditer = nullptr;
						  if (fglobal[iflow].smaginfo.bDynamic_Stress) {
							  smagconstolditer = new doublereal[fglobal[iflow].maxelm];
							  if (smagconstolditer == nullptr) {
								  // ������������ ������ �� ������ ������������.
								  printf("Problem: not enough memory on your equipment for smagconstolditer steady cfd calculation...\n");
								  printf("Please any key to exit...\n");
								  exit(1);
							  }
						  }

						  // ���������� ����������������� �������� � ���������� ��������.
						  doublereal **SpeedCorOld = nullptr;
						  SpeedCorOld = new doublereal*[3];
						  if (SpeedCorOld == nullptr) {
							  // ������������ ������ �� ������ ������������.
							  printf("Problem: not enough memory on your equipment for SpeedCorOld steady cfd calculation...\n");
							  printf("Please any key to exit...\n");
							  exit(1);
						  }
						  for (integer i = 0; i < 3; i++) {
							  SpeedCorOld[i] = nullptr;
							  SpeedCorOld[i] = new doublereal[fglobal[iflow].maxelm + fglobal[iflow].maxbound];
							  if (SpeedCorOld[i] == nullptr) {
								  // ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
								  printf("Problem: not enough memory on your equipment for SpeedCorOld[%lld] steady cfd calculation...\n", i);
#else
								  printf("Problem: not enough memory on your equipment for SpeedCorOld[%d] steady cfd calculation...\n", i);
#endif
								  printf("Please any key to exit...\n");
								  exit(1);
							  }
						  }
						  for (integer i = 0; i < 3; i++) {
							  for (integer j = 0; j < fglobal[iflow].maxelm + fglobal[iflow].maxbound; j++) {
								  switch (i) {
								  case VELOCITY_X_COMPONENT: SpeedCorOld[VELOCITY_X_COMPONENT][j] = fglobal[iflow].potent[VXCOR][j];
									  break;
								  case VELOCITY_Y_COMPONENT: SpeedCorOld[VELOCITY_Y_COMPONENT][j] = fglobal[iflow].potent[VYCOR][j];
									  break;
								  case VELOCITY_Z_COMPONENT: SpeedCorOld[VELOCITY_Z_COMPONENT][j] = fglobal[iflow].potent[VZCOR][j];
									  break;
								  }
							  }
						  }

						  doublereal **mfold = new doublereal*[fglobal[iflow].maxelm];
						  for (integer i = 0; i < fglobal[iflow].maxelm; i++) {
							  mfold[i] = new doublereal[6];
						  }

						  for (integer i = 0; i < fglobal[iflow].maxelm; i++) {
							  for (integer j = 0; j < 6; j++) {
								  mfold[i][j] = fglobal[iflow].mf[i][j]; // ��������� �����.
							  }
						  }

						  for (integer iP = 0; iP < fglobal[iflow].maxelm; iP++) {

							  // ��������� ����������������� �������� ����� ����� ����� ��.
							  // �������� ����� ����������� �� ������� �������� �� � ������
							  // ������ ��� ���������������� �������� ���-���. ��� ��� ���������� ������������
							  // ������� �������� ������������ �������� �� ����� ��.

							  bool bsimplelinearinterpol = true; // ����������� ������� �������� ������������ �������� �� �����.

							  // 25.03.2019 ������ �������� �� ���� �����.
							  return_calc_correct_mass_flux(iP,
								  fglobal[iflow].potent,
								  fglobal[iflow].pa,
								  fglobal[iflow].prop,
								  fglobal[iflow].prop_b,
								  fglobal[iflow].nvtx,
								  fglobal[iflow].neighbors_for_the_internal_node,
								  fglobal[iflow].maxelm,
								  fglobal[iflow].diag_coef,
								  fglobal[iflow].alpha,
								  RCh,
								  false,
								  0.01,
								  nullptr,
								  fglobal[iflow].mf[iP], // ������������ �������� ��������� ������
								  nullptr, bsimplelinearinterpol,
								  SpeedCorOld, mfold[iP],
								  fglobal[iflow].border_neighbor,
								  t.ilevel_alice,
								  fglobal[iflow].ptr);

							  if (fglobal[iflow].smaginfo.bDynamic_Stress) {
								  smagconstolditer[iP] = 0.0; // ��������� ��������
							  }
						  }

						  // �������������.
						  for (integer i = 0; i < fglobal[iflow].maxelm; i++) {
							  for (integer j = 0; j < 6; j++) {
								  mfold[i][j] = fglobal[iflow].mf[i][j]; // ��������� �����.
							  }
						  }


						  // ������������ ����������� ������ �� ����.
						  if (SpeedCorOld != nullptr) {
							  for (integer i = 0; i < 3; i++) {
								  if (SpeedCorOld[i] != nullptr) {
									  delete[] SpeedCorOld[i];
									  SpeedCorOld[i] = nullptr;
								  }
							  }
							  delete[] SpeedCorOld;
						  }
						  SpeedCorOld = nullptr;

						  QuickMemVorst my_memory_bicgstab;
						  my_memory_bicgstab.ballocCRScfd = false; // �������� ������.
						  my_memory_bicgstab.bsignalfreeCRScfd = false; // �� ���������� ������. (��� ����).
						  // ������������� ���������� !
						  my_memory_bicgstab.val = nullptr;
						  my_memory_bicgstab.col_ind = nullptr;
						  my_memory_bicgstab.row_ptr = nullptr;
						  my_memory_bicgstab.ri = nullptr;
						  my_memory_bicgstab.roc = nullptr;
						  my_memory_bicgstab.s = nullptr;
						  my_memory_bicgstab.t = nullptr;
						  my_memory_bicgstab.vi = nullptr;
						  my_memory_bicgstab.pi = nullptr;
						  my_memory_bicgstab.dx = nullptr;
						  my_memory_bicgstab.dax = nullptr;
						  my_memory_bicgstab.y = nullptr;
						  my_memory_bicgstab.z = nullptr;
						  my_memory_bicgstab.a = nullptr;
						  my_memory_bicgstab.ja = nullptr;
						  my_memory_bicgstab.ia = nullptr;
						  my_memory_bicgstab.alu = nullptr;
						  my_memory_bicgstab.jlu = nullptr;
						  my_memory_bicgstab.ju = nullptr;
						  my_memory_bicgstab.alu1 = nullptr;
						  my_memory_bicgstab.jlu1 = nullptr;
						  my_memory_bicgstab.ju1 = nullptr;
						  my_memory_bicgstab.x1 = nullptr;
						  my_memory_bicgstab.iw = nullptr;
						  my_memory_bicgstab.levs = nullptr;
						  my_memory_bicgstab.w = nullptr;
						  my_memory_bicgstab.jw = nullptr;
						  my_memory_bicgstab.w_dubl = nullptr;
						  my_memory_bicgstab.jw_dubl = nullptr;
						  // ������ ��������� � ����������� ������������� �������� � ��������� �������������.
						  my_memory_bicgstab.ballocCRSt = false; // �������� ������
						  my_memory_bicgstab.bsignalfreeCRSt = false; // � ����� �� �����������.
						  // ������������� ����������.
						  my_memory_bicgstab.tval = nullptr;
						  my_memory_bicgstab.tcol_ind = nullptr;
						  my_memory_bicgstab.trow_ptr = nullptr;
						  my_memory_bicgstab.tri = nullptr;
						  my_memory_bicgstab.troc = nullptr;
						  my_memory_bicgstab.ts = nullptr;
						  my_memory_bicgstab.tt = nullptr;
						  my_memory_bicgstab.tvi = nullptr;
						  my_memory_bicgstab.tpi = nullptr;
						  my_memory_bicgstab.tdx = nullptr;
						  my_memory_bicgstab.tdax = nullptr;
						  my_memory_bicgstab.ty = nullptr;
						  my_memory_bicgstab.tz = nullptr;
						  my_memory_bicgstab.ta = nullptr;
						  my_memory_bicgstab.tja = nullptr;
						  my_memory_bicgstab.tia = nullptr;
						  my_memory_bicgstab.talu = nullptr;
						  my_memory_bicgstab.tjlu = nullptr;
						  my_memory_bicgstab.tju = nullptr;
						  my_memory_bicgstab.tiw = nullptr;
						  my_memory_bicgstab.tlevs = nullptr;
						  my_memory_bicgstab.tw = nullptr;
						  my_memory_bicgstab.tjw = nullptr;
						  my_memory_bicgstab.icount_vel = 100000; // ����� ������� �����.

						  // ���������� ����������������� �������� � ���������� ��������.
						  doublereal **SpeedCorOldinternal = new doublereal*[3];
						  for (integer i = 0; i < 3; i++) {
							  SpeedCorOldinternal[i] = new doublereal[fglobal[iflow].maxelm + fglobal[iflow].maxbound];
						  }

						  doublereal* xb = new doublereal[fglobal[iflow].maxelm + fglobal[iflow].maxbound];
						  doublereal* rthdsd = nullptr; // ������ ����� ������� ���������.
						  doublereal* rthdsdt = nullptr;
						  rthdsd = new doublereal[fglobal[iflow].maxelm + fglobal[iflow].maxbound];
						  rthdsdt = new doublereal[t.maxelm + t.maxbound];


						  integer iend = 1000; // 300 ����� ��������.
						  for (integer i75 = 0; i75 < lw; i75++) if (w[i75].bopening == true) iend = 450;
						  if ((iFLOWScheme > distsheme) || (iTEMPScheme > distsheme)) {
							  // �� ��������� ���������� �������� ��������� ��� ���������� ��� ������� ���������� �� ����� ������� �����������
							  // �����������.
							  //iend *= 2;
							  iend += 200; // ���������� �������.
						  }

						  if ((bSIMPLErun_now_for_temperature) && ((fabs(dgx) > 1.0e-20) || (fabs(dgy) > 1.0e-20) || (fabs(dgz) > 1.0e-20))) {
							  // ����������� ���������.
							  // ��� ������������� ����������� ��������� �� �� ���������� �������������� rGradual_changes
							  rGradual_changes = 1.0;
							  iend = 1000;
						  }
						  else {

							  if (false || (!b_on_adaptive_local_refinement_mesh)) {
								  // �� ����������������� ����� �� �������� ���������
								  // ����� ������ ����������� ������� �������� �������� ����� ������ ������,
								  // ��� �������� � ���������� ��������� ���������� ������ �� ������������.
								  // ������� ���� ��� ������� ������� ������ ����� �� ���� ������� ����� �� ��������� 
								  // � 10 ��� ������� � ������� 310 �������� � ������� ��� ���� ���������. ����� �������������
								  // ������� ������� � ������� ������������ �� �������� �������� �������� � ��� 700 ��������.
								  // ������ ������� � ������ ���� �� ����, ������� �� ���� �� ������ ������ 300 �������� � ��.
								  if (fabs(rGradual_changes - 1.0) > 1.0e-30) {
									  for (integer i_96 = 0; i_96 < lw; i_96++) {
										  w[i_96].Vx *= rGradual_changes;
										  w[i_96].Vy *= rGradual_changes;
										  w[i_96].Vz *= rGradual_changes;
										  // �� ������ ���� �������� ����� ���� ������ ����� �������� ��������.
										  w[i_96].P *= rGradual_changes * rGradual_changes;
									  }
								  }
							  }
							  else {
								  // � ��������� ���������� �������� ������ �� ������� continity:
								  // ���� ��� ���������� ������� 1.0E-3 ������� ��������� ����������.
								  iend = 1000; // ��� ���� ������ �������.
							  }

						  }
						  // ������� �� ������������� ���������� � ��������� �������.
						  integer iseparate_SIMPLE = 10000;
						  bool bseparate_SIMPLE = true;// ������ ������ ���� ���.

						  doublereal start_average_continity = 0.0;

						  if (number_iteration_SIMPLE_algorithm > 0) {
							  // 22.09.2019
							  // ���������� �������� SIMPLE ��������� �������� 
							  // ������������� ����� ����������� ���������.
							  iend = number_iteration_SIMPLE_algorithm;
						  }

						  for (integer i = inumber_iteration_SIMPLE[iflow] + 1; i < iend; i++) {
							  if (i == iend - 1) {
								  my_memory_bicgstab.bsignalfreeCRScfd = true;
								  my_memory_bicgstab.bsignalfreeCRSt = true; // ����������� �� ��������� ��������.
							  }

							  if ((bSIMPLErun_now_for_temperature) && ((fabs(dgx) > 1.0e-20) || (fabs(dgy) > 1.0e-20) || (fabs(dgz) > 1.0e-20))) {
								  // ����������� ���������.
								   // ��� ������������� ����������� ��������� �� �� ���������� �������������� rGradual_changes
								  rGradual_changes = 1.0;
							  }
							  else {


								  if (false || (!b_on_adaptive_local_refinement_mesh)) {
									  // �� ����������������� ����� �� �������� ���������
									  // ����� ������ ����������� ������� �������� �������� ����� ������ ������,
									  // ��� �������� � ���������� ��������� ���������� ������ �� ������������.
									  // ������� ���� ��� ������� ������� ������ ����� �� ���� ������� ����� �� ��������� 
									  // � 10 ��� ������� � ������� 310 �������� � ������� ��� ���� ���������. ����� �������������
									  // ������� ������� � ������� ������������ �� �������� �������� �������� � ��� 700 ��������.
									  // ������ ������� � ������ ���� �� ����, ������� �� ���� �� ������ ������ 300 �������� � ��.
									  if (bseparate_SIMPLE && (i == iseparate_SIMPLE) && (fabs(rGradual_changes - 1.0) > 1.0e-30)) {
										  bseparate_SIMPLE = false;
										  // ����� �� ��������������� �������.
										  bool b_modify_cor = false;
										  for (integer i_96 = 0; i_96 < lw; i_96++) {
											  if ((fabs(w[i_96].Vx) > 1.0e-30) || (fabs(w[i_96].Vy) > 1.0e-30) || (fabs(w[i_96].Vz) > 1.0e-30) || (fabs(w[i_96].P) > 1.0e-30)) {
												  // � ������ ������������ ��������� ��� �� ���� ������ ��������������, �.�. 
												  // �������� �� ������� ������� � ��� �������� ����� ���������� ������� �������.

												  // ����� ��� �� ��� � �� ��������� �����������.
												  b_modify_cor = true;
											  }
										  }

										  if (b_modify_cor) {
											  for (integer i_96 = 0; i_96 < lw; i_96++) {
												  w[i_96].Vx /= rGradual_changes;
												  w[i_96].Vy /= rGradual_changes;
												  w[i_96].Vz /= rGradual_changes;
												  // �� ������ ��������� �������� ����� ���� ������ ����� ��������.
												  w[i_96].P /= (rGradual_changes * rGradual_changes);
											  }
											  for (integer i_96 = 0; i_96 < fglobal[0].maxelm + fglobal[0].maxbound; i_96++) {
												  for (integer i_97 = 0; i_97 <= 26; i_97++) {
													  if ((i_97 != TOTALDEFORMATIONVAR) && (i_97 != MUT) && (i_97 != FBUF)) {
														  if ((i_97 == PRESS) || (i_97 == PAM) || ((i_97 >= GRADXPAM) && (i_97 <= PAMOLDITER))) {
															  // �������� ������������� � ������� ���.
															  fglobal[0].potent[i_97][i_96] /= (rGradual_changes * rGradual_changes);
														  }
														  else {
															  fglobal[0].potent[i_97][i_96] /= rGradual_changes;
														  }
													  }
												  }
											  }
										  }
									  }
								  }
							  }

							  if (0 && (i >= 67)) { // debug
								  bprintmessage = true;
								  // 25.03.2019
								  // ������� ���������� ���������� � ��������� tecplot360:
								  if (!b_on_adaptive_local_refinement_mesh) {
									  exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, i, bextendedprint, 0, b, lb);
								  }
								  else {
									  ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, fglobal, 0, b, lb);
								  }
								  printf("start iter == 68...\n");
							  }

							  if (lb > 150) {
								  // ������ ����� 150 ������ ���������������.
								  // ��� ��������������� ������� ������� � tecplot ����, ����� �������� ���������.
								  if (!b_on_adaptive_local_refinement_mesh) {
									  if (i % 10 == 0) {
										  // 25.03.2019
										  // ������� ���������� ���������� � ��������� tecplot360:
										  if (!b_on_adaptive_local_refinement_mesh) {
											  exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, i, bextendedprint, 0, b, lb);
										  }
										  else {
											  ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, fglobal, 0, b, lb);
										  }
										  printf("export to tecplot 360... \n");
									  }
								  }
								  else {
									  // ����. ������� ������ �� �������. ����� ������ 50 ��������. 30.03.2019
									  // �.�. �� ������ ��� ����� � 2 ���� ����.
									  if (i % 50 == 0) {
										  //ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, fglobal, 0, b, lb);
										  printf("export to tecplot 360... \n");
									  }
								  }
							  }


							  if (i == (inumber_iteration_SIMPLE[iflow] + 1)) {
								  // ��������� ������ ���������� ������ ������
								  // ���� ����������������, ��������, �. ����������. 0.5; 0.8;
								  // � ����� Ferczinger and Peric �������������� ���������� ���������� ���������� ������: 0.7; 0.3; 
								  // ��� �������� 0.7, � ��� �������� 0.3. ��� ���� ���������� ����� ������ ��� 0.7+0.3 == 1.0;
								  if (!b_on_adaptive_local_refinement_mesh) {
									  fglobal[iflow].alpha[VELOCITY_X_COMPONENT] = 0.7; // 0.8 0.5
									  fglobal[iflow].alpha[VELOCITY_Y_COMPONENT] = 0.7; // 0.8 0.5
									  fglobal[iflow].alpha[VELOCITY_Z_COMPONENT] = 0.7; // 0.8 0.5
									  fglobal[iflow].alpha[PRESS] = 0.3; // 0.2 0.8
									  fglobal[iflow].alpha[NUSHA_SL] = 0.8;// 1.0;// 0.7;
									  fglobal[iflow].alpha[TURBULENT_KINETIK_ENERGY_SL] = 0.8;
									  fglobal[iflow].alpha[TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA_SL] = 0.8;
									  fglobal[iflow].alpha[TURBULENT_KINETIK_ENERGY_STD_K_EPS_SL] = 0.8;
									  fglobal[iflow].alpha[TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS_SL] = 0.8;
								  }
								  else {
									  fglobal[iflow].alpha[VELOCITY_X_COMPONENT] = 0.8; // 0.8 0.5
									  fglobal[iflow].alpha[VELOCITY_Y_COMPONENT] = 0.8; // 0.8 0.5
									  fglobal[iflow].alpha[VELOCITY_Z_COMPONENT] = 0.8; // 0.8 0.5
									  fglobal[iflow].alpha[PRESS] = 0.2;// 0.05; // 0.2 0.8
									  fglobal[iflow].alpha[NUSHA_SL] = 0.8;// 1.0;// 0.8;
									  fglobal[iflow].alpha[TURBULENT_KINETIK_ENERGY_SL] = 0.8;
									  fglobal[iflow].alpha[TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA_SL] = 0.8;
									  fglobal[iflow].alpha[TURBULENT_KINETIK_ENERGY_STD_K_EPS_SL] = 0.8;
									  fglobal[iflow].alpha[TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS_SL] = 0.8;
								  }
							  }
							  else {
								  // ����� �� ������������ ��������� ���������� ������������ 
								  // � ����� �. ���������.
								  // � ����� Ferczinger and Peric �������������� ���������� ���������� ���������� ������: 0.7; 0.3; 
								  // ��� �������� 0.7, � ��� �������� 0.3. ��� ���� ���������� ����� ������ ��� 0.7+0.3 == 1.0;
								  if (!b_on_adaptive_local_refinement_mesh) {
									  fglobal[iflow].alpha[VELOCITY_X_COMPONENT] = 0.7; // 0.8 0.5
									  fglobal[iflow].alpha[VELOCITY_Y_COMPONENT] = 0.7; // 0.8 0.5
									  fglobal[iflow].alpha[VELOCITY_Z_COMPONENT] = 0.7; // 0.8 0.5
									  fglobal[iflow].alpha[PRESS] = 0.3; // 0.2 0.8
									  fglobal[iflow].alpha[NUSHA_SL] = 0.8;// 1.0;// 0.7; 
									  fglobal[iflow].alpha[TURBULENT_KINETIK_ENERGY_SL] = 0.8;
									  fglobal[iflow].alpha[TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA_SL] = 0.8;
									  fglobal[iflow].alpha[TURBULENT_KINETIK_ENERGY_STD_K_EPS_SL] = 0.8;
									  fglobal[iflow].alpha[TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS_SL] = 0.8;
								  }
								  else {
									  fglobal[iflow].alpha[VELOCITY_X_COMPONENT] = 0.8; // 0.8 0.5
									  fglobal[iflow].alpha[VELOCITY_Y_COMPONENT] = 0.8; // 0.8 0.5
									  fglobal[iflow].alpha[VELOCITY_Z_COMPONENT] = 0.8; // 0.8 0.5
									  fglobal[iflow].alpha[PRESS] = 0.2;// 0.05; // 0.2 0.8
									  fglobal[iflow].alpha[NUSHA_SL] = 0.8;// 1.0; // 0.8
									  fglobal[iflow].alpha[TURBULENT_KINETIK_ENERGY_SL] = 0.8;
									  fglobal[iflow].alpha[TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA_SL] = 0.8;
									  fglobal[iflow].alpha[TURBULENT_KINETIK_ENERGY_STD_K_EPS_SL] = 0.8;
									  fglobal[iflow].alpha[TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS_SL] = 0.8;
								  }
							  }

							  bool bfirst_start = false;
							  if ((i == (inumber_iteration_SIMPLE[iflow] + 1)) && bfirst) {
								  bfirst_start = true;
								  bfirst = false; // ������ ������ ���� ������ ���� ���.
							  }


							  // ����� �������.
							  unsigned int calculation_simple_start_time; // ������ ����� ��.
							  unsigned int calculation_simple_end_time; // ��������� ����� ��.
							  unsigned int calculation_simple_seach_time; // ����� ���������� ������� ���� � ��.

							  calculation_simple_start_time = clock(); // ������ ������ �����.

							  integer dist_max_fluid = 3;

							  // ������������ ������.
							  my_version_SIMPLE_Algorithm3D(continity, i,
								  fglobal[iflow],
								  fglobal,
								  t, rhie_chow,
								  b, lb, s, ls, w, lw,
								  BETA_PRECISION,
								  flow_interior,
								  iflow,
								  bfirst_start,
								  dgx, dgy, dgz,
								  matlist,
								  btimedep,
								  0.0, 0.0, 0.0,
								  nullptr, nullptr, nullptr,
								  bprintmessage,
								  gtdps, ltdp,
								  rfluentres, rfluentrestemp,
								  smagconstolditer,
								  mfold, eqin.itemper,
								  my_memory_bicgstab,
								  bextendedprint,
								  SpeedCorOldinternal, xb,
								  rthdsd, rthdsdt, lu, my_union, 
								  color, dist_max_fluid, color_solid, dist_max_solid);


							  calculation_simple_end_time = clock();
							  calculation_simple_seach_time = calculation_simple_end_time - calculation_simple_start_time;
							  int im = 0, is = 0, ims = 0;
							  im = (int)(calculation_simple_seach_time / 60000); // ������
							  is = (int)((calculation_simple_seach_time - 60000 * im) / 1000); // �������
							  ims = (int)((calculation_simple_seach_time - 60000 * im - 1000 * is) / 10); // ������������ ������� �� 10

							  const integer ianimstart = 0;// 1800;
							  if (ianimation_write_on == 1) {
								  if (1 && (i > ianimstart)) {
									  char* buffer = new char[10];
									  buffer[0] = '\0';
									  KRitoa(i - ianimstart + 1, buffer);
									  //printf("%s\n",buffer);
									  char* mymessage = new char[30];
									  mymessage[0] = '\0';
									  KRstrcat(mymessage, "iter=");
									  //printf("%s\n",mymessage);
									  KRstrcat(mymessage, buffer);
									  //printf("%s\n",mymessage);
									  //getchar();
									  bool btitle = false;
									  if (i == ianimstart + 1) btitle = true;
									  animationtecplot360T_3D_part2all(t.maxelm, t.ncell, fglobal, t, flow_interior, mymessage, btitle,b,lb);
									  delete[] buffer; delete[] mymessage;
								  }
							  }


#if doubleintprecision == 1
							  //if (i==5) continity_start[iflow]=continity;
							  if (i <= 5) {


								  fprintf(fpcont, "%lld 1.0\n", i + 1);
								  if (!bprintmessage) {
									  if (eqin.itemper == 0) {
										  // ������� ������ ������������� ��� ��������� ����������������.

										  if (fglobal[0].iflowregime == RANS_STANDART_K_EPS) {
											  //printf("%lld 1.0\n",i+1);
											  printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_turb_kinetik_energy_std_ke,
												  rfluentres.res_turb_epsilon, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity k	epsilon\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
											  if ((err_stat) == 0) {
												  // 31 ������� 2019.
												  fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
													  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentres.res_turb_kinetik_energy_std_ke,
													  rfluentres.res_turb_epsilon);
												  fclose(fp_statistic_convergence);
											  }
										  }
										  else
										  if (fglobal[0].iflowregime == RANS_MENTER_SST) {
											  //printf("%lld 1.0\n",i+1);
											  printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_turb_kinetik_energy,
												  rfluentres.res_turb_omega, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity k	omega\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
											  if ((err_stat) == 0) {
												  // 29 ������� 2015.
												  fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
													  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentres.res_turb_kinetik_energy,
													  rfluentres.res_turb_omega);
												  fclose(fp_statistic_convergence);
											  }
										  }
										  else if (fglobal[0].iflowregime == RANS_SPALART_ALLMARES) {
											  //printf("%lld 1.0\n",i+1);
											  printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_nusha, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity nut		\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
											  if ((err_stat) == 0) {
												  // 29 ������� 2015.
												  fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
													  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentres.res_nusha);
												  fclose(fp_statistic_convergence);
											  }
										  }
										  else {
											  //printf("%lld 1.0\n",i+1);
											  printf(" %lld %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity \t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
											  if ((err_stat) == 0) {
												  // 29 ������� 2015.
												  fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
													  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance);
												  fclose(fp_statistic_convergence);
											  }
										  }
									  }
									  else if (eqin.itemper == 1) {
										  doublereal tmax = -1.0e30;
										  // ���������� �������� ������������ ����������� ������ ��������� ������� � �� � ��������:
										  for (integer i1 = 0; i1 < t.maxelm + t.maxbound; i1++) tmax = fmax(tmax, t.potent[i1]);
										  // ������� ������������� ��������� � ���������� ����������������.
										  if (fglobal[0].iflowregime == RANS_STANDART_K_EPS) {
											  printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp,
												  rfluentres.res_turb_kinetik_energy_std_ke,
												  rfluentres.res_turb_epsilon, tmax, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity temperature k	epsilon	Tmax\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
											  if ((err_stat) == 0) {
												  // 29 ������� 2015. 30 september 2019
												  fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i,
													  rfluentres.res_vx, rfluentres.res_vy, rfluentres.res_vz,
													  rfluentres.res_no_balance, rfluentrestemp, rfluentres.res_turb_kinetik_energy_std_ke,
													  rfluentres.res_turb_epsilon);
												  fclose(fp_statistic_convergence);
											  }
										  }
										  else
										  if (fglobal[0].iflowregime == RANS_MENTER_SST) {
											  printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp,
												  rfluentres.res_turb_kinetik_energy,
												  rfluentres.res_turb_omega, tmax, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity temperature k	omega	Tmax\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
											  if ((err_stat) == 0) {
												  // 29 ������� 2015. 30 september 2019
												  fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i,
													  rfluentres.res_vx, rfluentres.res_vy, rfluentres.res_vz, 
													  rfluentres.res_no_balance, rfluentrestemp, rfluentres.res_turb_kinetik_energy,
													  rfluentres.res_turb_omega);
												  fclose(fp_statistic_convergence);
											  }
										  }
										  else  if (fglobal[0].iflowregime == RANS_SPALART_ALLMARES) {
											  printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, rfluentres.res_nusha, tmax, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity temperature nut		Tmax\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
											  if ((err_stat) == 0) {
												  // 29 ������� 2015. 30 september 2019
												  fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i,
													  rfluentres.res_vx, rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp, rfluentres.res_nusha);
												  fclose(fp_statistic_convergence);
											  }
										  }
										  else {
											  printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, tmax, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity temperature Tmax\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
											  if ((err_stat) == 0) {
												  // 29 ������� 2015.
												  fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e\n", i,
													  rfluentres.res_vx, rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp);
												  fclose(fp_statistic_convergence);
											  }
										  }
									  }
								  }
								  continity_start[iflow] = continity;
								  rfluentres.operating_value_b = rfluentres.res_no_balance;
							  }
							  else {
								  fprintf(fpcont, "%lld %e\n", i + 1, continity / continity_start[iflow]); // ���������� � ����������
								  if (!bprintmessage) {
									  if (eqin.itemper == 0) {
										  // ������� ������ ������������� ��� ��������� ����������������.
										  if (fglobal[0].iflowregime == RANS_STANDART_K_EPS) {
											  //printf("%lld %e\n", i+1, continity/continity_start[iflow]); // ���������� � ����������
											  printf(" %5lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_turb_kinetik_energy_std_ke,
												  rfluentres.res_turb_epsilon, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity k	epsilon \t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
											  if ((err_stat) == 0) {
												  // 29 ������� 2015.
												  fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
													  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance,
													  rfluentres.res_turb_kinetik_energy_std_ke, rfluentres.res_turb_epsilon);
												  fclose(fp_statistic_convergence);
											  }
										  }
										  else
										  if (fglobal[0].iflowregime == RANS_MENTER_SST) {
											  //printf("%lld %e\n", i+1, continity/continity_start[iflow]); // ���������� � ����������
											  printf(" %5lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_turb_kinetik_energy,
												  rfluentres.res_turb_omega, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity k	omega \t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
											  if ((err_stat) == 0) {
												  // 29 ������� 2015.
												  fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
													  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentres.res_turb_kinetik_energy,
													  rfluentres.res_turb_omega);
												  fclose(fp_statistic_convergence);
											  }
										  }
										  else if (fglobal[0].iflowregime == RANS_SPALART_ALLMARES) {
											  //printf("%lld %e\n", i+1, continity/continity_start[iflow]); // ���������� � ����������
											  printf(" %5lld %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_nusha, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity nut		\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
											  if ((err_stat) == 0) {
												  // 29 ������� 2015.
												  fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
													  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentres.res_nusha);
												  fclose(fp_statistic_convergence);
											  }
										  }
										  else {

											  //printf("%lld %e\n", i+1, continity/continity_start[iflow]); // ���������� � ����������
											  printf(" %5lld %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity \t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
											  if ((err_stat) == 0) {
												  // 29 ������� 2015.
												  fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
													  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance);
												  fclose(fp_statistic_convergence);
											  }
										  }
									  }
									  else if (eqin.itemper == 1) {
										  doublereal tmax = -1.0e30;
										  // ���������� �������� ������������ ����������� ������ ��������� ������� � �� � ��������:
										  for (integer i1 = 0; i1 < t.maxelm + t.maxbound; i1++) tmax = fmax(tmax, t.potent[i1]);
										  // ������� ������������� ��������� � ���������� ����������������.
										  if (fglobal[0].iflowregime == RANS_STANDART_K_EPS) {
											  printf(" %5lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp,
												  rfluentres.res_turb_kinetik_energy_std_ke, rfluentres.res_turb_epsilon,
												  tmax, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity temperature k	epsilon    Tmax\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
											  if ((err_stat) == 0) {
												  // 29 ������� 2015.
												  fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
													  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp,
													  rfluentres.res_turb_kinetik_energy_std_ke, rfluentres.res_turb_epsilon);
												  fclose(fp_statistic_convergence);
											  }
										  }
										  else
										  if (fglobal[0].iflowregime == RANS_MENTER_SST) {
											  printf(" %5lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp,
												  rfluentres.res_turb_kinetik_energy, rfluentres.res_turb_omega,
												  tmax, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity temperature k	omega    Tmax\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
											  if ((err_stat) == 0) {
												  // 29 ������� 2015.
												  fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
													  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp, 
													  rfluentres.res_turb_kinetik_energy, rfluentres.res_turb_omega);
												  fclose(fp_statistic_convergence);
											  }
										  }
										  else if (fglobal[0].iflowregime == RANS_SPALART_ALLMARES) {
											  printf(" %5lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, rfluentres.res_nusha, tmax,
												  im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity temperature nut		Tmax\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
											  if ((err_stat) == 0) {
												  // 29 ������� 2015.
												  fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
													  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp, rfluentres.res_nusha);
												  fclose(fp_statistic_convergence);
											  }
										  }
										  else {
											  printf(" %5lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, tmax,
												  im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity temperature Tmax\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
											  if ((err_stat) == 0) {
												  // 29 ������� 2015.
												  fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
													  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp);
												  fclose(fp_statistic_convergence);
											  }
										  }
									  }
								  }
							  }
#else
							  //if (i==5) continity_start[iflow]=continity;
							  if (i <= 5) {


								  fprintf(fpcont, "%d 1.0\n", i + 1);
								  if (!bprintmessage) {
									  if (eqin.itemper == 0) {
										  // ������� ������ ������������� ��� ��������� ����������������.
										  if (fglobal[0].iflowregime == RANS_STANDART_K_EPS) {
											  //printf("%d 1.0\n",i+1);
											  printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %d\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_turb_kinetik_energy_std_ke, 
												  rfluentres.res_turb_epsilon, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity k	  epsilon	\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
												  if ((err_stat) == 0) {
													  // 29 ������� 2015.
													  fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n",
														  i, rfluentres.res_vx,
														  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance,
														  rfluentres.res_turb_kinetik_energy_std_ke, rfluentres.res_turb_epsilon);
													  fclose(fp_statistic_convergence);
												  }
										  }
										  else
										  if (fglobal[0].iflowregime == RANS_MENTER_SST) {
											  //printf("%d 1.0\n",i+1);
											  printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %d\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_turb_kinetik_energy, rfluentres.res_turb_omega, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity k	  omega	\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
												  if ((err_stat) == 0) {
													  // 29 ������� 2015.
													  fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n",
														  i, rfluentres.res_vx,
														  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, 
														  rfluentres.res_turb_kinetik_energy, rfluentres.res_turb_omega);
													  fclose(fp_statistic_convergence);
												  }
										  }
										  else  if (fglobal[0].iflowregime == RANS_SPALART_ALLMARES) {
											  //printf("%d 1.0\n",i+1);
											  printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %d\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_nusha, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity nut		\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
												  if ((err_stat) == 0) {
													  // 29 ������� 2015.
													  fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
														  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentres.res_nusha);
													  fclose(fp_statistic_convergence);
												  }
										  }
										  else {
											  //printf("%d 1.0\n",i+1);
											  printf(" %d %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %d\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity \t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
												  if ((err_stat) == 0) {
													  // 29 ������� 2015.
													  fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
														  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance);
													  fclose(fp_statistic_convergence);
												  }
										  }
									  }
									  else if (eqin.itemper == 1) {
										  doublereal tmax = 0.0;
										  // ���������� �������� ������������ ����������� ������ ��������� ������� � �� � ��������:
										  for (integer i1 = 0; i1 < t.maxelm + t.maxbound; i1++) tmax = fmax(tmax, fabs(t.potent[i1]));
										  // ������� ������������� ��������� � ���������� ����������������.
										  if (fglobal[0].iflowregime == RANS_STANDART_K_EPS) {
											  printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %d\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp,
												  rfluentres.res_turb_kinetik_energy_std_ke, rfluentres.res_turb_epsilon,
												  tmax, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity temperature k    epsilon  Tmax\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
												  if ((err_stat) == 0) {
													  // 29 ������� 2015.
													  fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
														  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp,
														  rfluentres.res_turb_kinetik_energy_std_ke, rfluentres.res_turb_epsilon);
													  fclose(fp_statistic_convergence);
												  }
										  }
										  else
										  if (fglobal[0].iflowregime == RANS_MENTER_SST) {
											  printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %d\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp,
												  rfluentres.res_turb_kinetik_energy, rfluentres.res_turb_omega,
												  tmax, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity temperature k    omega  Tmax\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
												  if ((err_stat) == 0) {
													  // 29 ������� 2015.
													  fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
														  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp,
														  rfluentres.res_turb_kinetik_energy, rfluentres.res_turb_omega);
													  fclose(fp_statistic_convergence);
												  }
										  }
										  else if (fglobal[0].iflowregime == RANS_SPALART_ALLMARES) {
											  printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %d\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, rfluentres.res_nusha, tmax, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity temperature nut		Tmax\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
												  if ((err_stat) == 0) {
													  // 29 ������� 2015.
													  fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
														  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp, rfluentres.res_nusha);
													  fclose(fp_statistic_convergence);
												  }
										  }
										  else {
											  printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %d\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, tmax, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity temperature Tmax\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
												  if ((err_stat) == 0) {
													  // 29 ������� 2015.
													  fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
														  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp);
													  fclose(fp_statistic_convergence);
												  }
										  }
									  }
								  }
								  continity_start[iflow] = continity;
								  rfluentres.operating_value_b = rfluentres.res_no_balance;
							  }
							  else {
								  fprintf(fpcont, "%d %e\n", i + 1, continity / continity_start[iflow]); // ���������� � ����������
								  if (!bprintmessage) {
									  if (eqin.itemper == 0) {
										  // ������� ������ ������������� ��� ��������� ����������������.
										  if (fglobal[0].iflowregime == RANS_STANDART_K_EPS) {
											  //printf("%d %e\n", i+1, continity/continity_start[iflow]); // ���������� � ����������
											  printf(" %5d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %d\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz,
												  rfluentres.res_turb_kinetik_energy_std_ke, rfluentres.res_turb_epsilon, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity k	epsilon\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
												  if ((err_stat) == 0) {
													  // 29 ������� 2015.
													  fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
														  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance,
														  rfluentres.res_turb_kinetik_energy_std_ke, rfluentres.res_turb_epsilon);
													  fclose(fp_statistic_convergence);
												  }
										  }
										  else
										  if (fglobal[0].iflowregime == RANS_MENTER_SST) {
											  //printf("%d %e\n", i+1, continity/continity_start[iflow]); // ���������� � ����������
											  printf(" %5d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %d\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz,
												  rfluentres.res_turb_kinetik_energy, rfluentres.res_turb_omega, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity k	omega\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
												  if ((err_stat) == 0) {
													  // 29 ������� 2015.
													  fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
														  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance,
														  rfluentres.res_turb_kinetik_energy, rfluentres.res_turb_omega);
													  fclose(fp_statistic_convergence);
												  }
										  }
										  else  if (fglobal[0].iflowregime == RANS_SPALART_ALLMARES) {
											  //printf("%d %e\n", i+1, continity/continity_start[iflow]); // ���������� � ����������
											  printf(" %5d %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %d\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_nusha, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity nut		\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
												  if ((err_stat) == 0) {
													  // 29 ������� 2015.
													  fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
														  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentres.res_nusha);
													  fclose(fp_statistic_convergence);
												  }
										  }
										  else {
											  //printf("%d %e\n", i+1, continity/continity_start[iflow]); // ���������� � ����������
											  printf(" %5d %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %d\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity \t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
												  if ((err_stat) == 0) {
													  // 29 ������� 2015.
													  fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
														  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance);
													  fclose(fp_statistic_convergence);
												  }
										  }
									  }
									  else if (eqin.itemper == 1) {
										  doublereal tmax = 0.0;
										  // ���������� �������� ������������ ����������� ������ ��������� ������� � �� � ��������:
										  for (integer i1 = 0; i1 < t.maxelm + t.maxbound; i1++) tmax = fmax(tmax, fabs(t.potent[i1]));
										  // ������� ������������� ��������� � ���������� ����������������.
										  if (fglobal[0].iflowregime == RANS_STANDART_K_EPS) {
											  printf(" %5d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %d\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, rfluentres.res_turb_kinetik_energy_std_ke, rfluentres.res_turb_epsilon, tmax,
												  im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity temperature k      epsilon	Tmax\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
												  if ((err_stat) == 0) {
													  // 29 ������� 2015.
													  fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
														  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp,
														  rfluentres.res_turb_kinetik_energy_std_ke, rfluentres.res_turb_epsilon);
													  fclose(fp_statistic_convergence);
												  }
										  }
										  else
										  if (fglobal[0].iflowregime == RANS_MENTER_SST) {
											  printf(" %5d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %d\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, rfluentres.res_turb_kinetik_energy, rfluentres.res_turb_omega, tmax,
												  im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity temperature k      omega	Tmax\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
												  if ((err_stat) == 0) {
													  // 29 ������� 2015.
													  fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
														  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp, rfluentres.res_turb_kinetik_energy, rfluentres.res_turb_omega);
													  fclose(fp_statistic_convergence);
												  }
										  }
										  else  if (fglobal[0].iflowregime == RANS_SPALART_ALLMARES) {
											  printf(" %5d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %d\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, rfluentres.res_nusha, tmax,
												  im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity temperature nut		Tmax\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
												  if ((err_stat) == 0) {
													  // 29 ������� 2015.
													  fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
														  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp, rfluentres.res_nusha);
													  fclose(fp_statistic_convergence);
												  }
										  }
										  else {
											  printf(" %5d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %d\n",
												  i, rfluentres.res_no_balance, rfluentres.res_vx,
												  rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, tmax,
												  im, is, ims, iend - i);
											  if (i % 10 == 0) {
												  printf("  iter continity x-velocity y-velocity z-velocity temperature Tmax\t time/iter\n");
											  }
#ifdef MINGW_COMPILLER
											  err_stat = 0;
											  fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
											  if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
											  err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
												  if ((err_stat) == 0) {
													  // 29 ������� 2015.
													  fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
														  rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp);
													  fclose(fp_statistic_convergence);
						  }
					  }
									   }
								   }
							   }
#endif

							  
					      
							   bool breturn=false;
		                       //exporttecplotxy360( nve, maxelm, ncell, nvtx, nvtxcell, x, y, potent, rhie_chow);
						       // ������� ���������� ���������� � ��������� tecplot360:
	                           if ((i+1)%100==0) {

								   // 25.03.2019
								   // ������� ���������� ���������� � ��������� tecplot360:
								   if (!b_on_adaptive_local_refinement_mesh) {
									   exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, i, bextendedprint, 0,b,lb);
								   }
								   else {
									   ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, fglobal, 0, b, lb);
								   }

	                              //printf("write values. OK.\n");
	                              //getchar(); // debug avtosave
								  breturn=true;
	                           }
						       if ((i+1)%100==0) {
							      // ��������������
#if doubleintprecision == 1
								   printf("avtosave... iter=%lld", i + 1);
#else
								   printf("avtosave... iter=%d", i + 1);
#endif
							      
							      inumber_iteration_SIMPLE[iflow]=i;
							      avtosave(fglobal, t, flow_interior, inumber_iteration_SIMPLE, continity_start);
                                  breturn=true;
						       }

							   if (breturn) printf("\n");

							   if (0) {
								   // �������� ���������� ������ ����
								   if (i==84) {
									   inumber_iteration_SIMPLE[iflow]=i;
							           avtosave(fglobal, t, flow_interior, inumber_iteration_SIMPLE, continity_start);
									   printf("diagnosic pause...\n");
									  // getchar();
									   system("pause");
								   }
							   }

							   if ((i == 6)||(i== iseparate_SIMPLE)) {
								   start_average_continity = rfluentres.res_no_balance;
							   }

							   if (0&&(i>20) && (rfluentres.res_no_balance/ start_average_continity < 1.0e-6)) {
								   // �� Fluent ����� ������� �� �������� ������� 1.0�-3 � ��� �������
								   // ��� ������� ����� �������� �� ������� ���� ��� ���������� ������� ������� 
								   // (����� 150 �������). � ���������� ������ ������ ���������� 
								   // �������� ������� continity 1.0E-6 �� � ���� �� ����� ��������
								   // ������ �� ������� � ������ ��������� ���� �� ������� ������� (����� 150 �������).
								   // ��������� ������ ������� ���� ���������� ������� �� �������� 1.0E-6.
								   if ((b_on_adaptive_local_refinement_mesh)) {
									   // ��������� �����. ���������� ����������. ���������� ��������.
									   if (!bseparate_SIMPLE) {
										   printf("\ncontinity < 1.0e-6. Dosrochnji vjhod. STOP.\n");
										   i = iend;
									   }
									   else {
										   //iseparate_SIMPLE = i + 1;
										   printf("\ncontinity < 1.0e-6. Dosrochnji vjhod. STOP.\n");
										   i = iend;
									   }
								   }
								   else {								   
										   
										// ��������� �����. ���������� ����������. ���������� ��������.
										if (!bseparate_SIMPLE) {
										    printf("\ncontinity < 1.0e-6. Dosrochnji vjhod. STOP.\n");
											i = iend;
										}
										else {
										   iseparate_SIMPLE = i + 1;
										}
									   
								   }
							   }

							   // 28.07.2016
							  // exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, i, bextendedprint);
							  // getchar(); // debug
						   
	                       } // ����� ����� �������� ��������� SIMPLE

						   for (integer i=0; i<3; i++) {
		                       delete[] SpeedCorOldinternal[i];
	                       }
	                       delete[] SpeedCorOldinternal;

                           delete[] xb;
                           delete[] rthdsd;
                           delete[] rthdsdt;

                           for (integer i=0; i<3; i++) {
							   delete[] rhie_chow[i];
                               rhie_chow[i]=nullptr;
						   }
				           delete[] rhie_chow;
						   rhie_chow=nullptr;

						   // ������������ ����������� ������ �� ��� ��������� ������ �� �������.
						    for (integer i=0; i<fglobal[iflow].maxelm; i++) {
							  delete[]  mfold[i];
							  mfold[i]=nullptr;
							}
							delete[] mfold;
							mfold=nullptr;

							if (fglobal[iflow].smaginfo.bDynamic_Stress) {
							     delete[] smagconstolditer;
							     smagconstolditer=nullptr;
							}
					  }
				  }

				  calculation_end_time=clock();
				  calculation_seach_time=calculation_end_time-calculation_start_time;

				 unsigned int im=0, is=0, ims=0;
				 im=(unsigned int)(calculation_seach_time/60000); // ������
				 is=(unsigned int)((calculation_seach_time-60000*im)/1000); // �������
				 ims=(unsigned int)((calculation_seach_time-60000*im-1000*is)/10); // ������������ ������� �� 10

				  printf("time calculation is:  %u minute %u second %u 10*millisecond\n",im,is,ims);
				  if (bwait) {
				    // getchar();
					  system("pause");
				  }
		          fclose(fpcont); // �������� ����� ��� ������ �������.
				  // fclose(fp_statistic_convergence); // �������� ����� ��� ����� ���������� �� ����� �����.
		          // ������� ���������� ������� � ��������� tecplot360
	              // exporttecplotxy360_3D( f.maxelm, f.ncell, f.nvtx, f.nvtxcell, f.pa, f.potent, rhie_chow);
			}
             
            // 25.03.2019
			// ������� ���������� ���������� � ��������� tecplot360:
			if (!b_on_adaptive_local_refinement_mesh) {
				exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, 0, bextendedprint, 0,b,lb);
			}
			else {
				ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, fglobal, 0, b, lb);
			}			

		}

		{
		// ��������� �������� ������ ������� �������� ��� �������� ������� !
		doublereal tmax = -1.0e30;
		doublereal tmin = 1.0e30;
		doublereal tmax_FLUID = -1.0e30;
		for (integer i1 = 0; i1 < t.maxelm + t.maxbound; i1++) {
			tmax = fmax(tmax, t.potent[i1]);
			tmin = fmin(tmin, t.potent[i1]);
			if (i1 < t.maxelm) {
				if ((t.ptr != nullptr) && (t.ptr[1][i1] > -1)) {
					// ��� FLUID ������
					tmax_FLUID = fmax(tmax_FLUID, t.potent[i1]);
				}
			}
		}
		printf("\n");
		printf("1. minimum temperature in default interior is %1.4e\n", tmin);
		printf("1. maximum temperature in default interior is %1.4e\n", tmax);
		if (tmax_FLUID >= -273.15) {//30.10.2019
			printf("1. maximum temperature in FLUID interior is %1.4e\n", tmax_FLUID);
		}
		printf("\n");
		}

		// �������� ��� ��� ����������� ������ �����������:
		bool bposttempsolve=false;
		if (eqin.itemper==1) {
			// ������ �� ����������������� ������.
			//if (b_on_adaptive_local_refinement_mesh == false)
			//{
				
			    bposttempsolve = true;
			//}
		}
		else if (eqin.itemper==0) {
			 bposttempsolve=false;
		}
		if (bposttempsolve) {

			// ������� ��������� �������������:
			printf("temperature equation solve...\n");
			printf("please, press any key to start calculation...\n");
			//getchar();

			doublereal res=1.0; // ��������� �������� �������.
		    bool bconvective=false;
		    if (flow_interior>0) bconvective=true;
		    // ��������� ����������� ������ ��� �������� Rhie-Chow
	        my_malloc2(rhie_chow, 10); 
		    // printf("%e\n",t.alpha); // debug
		    t.alpha=1.0;
		    //getchar();
			// ���������: 17 - toldtimestep ����������� � ����������� ���������� ����.
			// 18 - tauparam ��� �� �������, 19 btimedep - ������������ false, �������������� true.

			// ����� �� ������ ����������� ����� ������������ ����������, 
			// ��� ���� ����� ���� ����� ��������������� � ����.
			bool bfirst_start=false;
			doublereal tauparam=0.0; // �������
			bool btimedep=false; // ������������ ������.
			integer inumiter=0; // ����� ������� �������� SIMPLE ���������.
			bool bVeryStable=false; // �������� ������������ ��� ������ �������� SIMPLE ���������.
			bool bhighorder=false;
			bool bdeltapfinish=false;
			bool consolemessage=true; // ������ ��������� �� �������.


			QuickMemVorst my_memory_bicgstab;
			my_memory_bicgstab.ballocCRSt=false; // �������� ������
			my_memory_bicgstab.bsignalfreeCRSt=true; // � ����� �����������.
			// ������������� ����������.
            my_memory_bicgstab.tval=nullptr;
			my_memory_bicgstab.tcol_ind=nullptr;
			my_memory_bicgstab.trow_ptr=nullptr;
			my_memory_bicgstab.tri=nullptr;
			my_memory_bicgstab.troc=nullptr;
			my_memory_bicgstab.ts=nullptr;
			my_memory_bicgstab.tt=nullptr;
			my_memory_bicgstab.tvi=nullptr;
			my_memory_bicgstab.tpi=nullptr;
			my_memory_bicgstab.tdx=nullptr;
			my_memory_bicgstab.tdax=nullptr;
			my_memory_bicgstab.ty=nullptr;
			my_memory_bicgstab.tz=nullptr;
			my_memory_bicgstab.ta=nullptr;
			my_memory_bicgstab.tja=nullptr;
			my_memory_bicgstab.tia=nullptr;
			my_memory_bicgstab.talu=nullptr;
			my_memory_bicgstab.tjlu=nullptr;
			my_memory_bicgstab.tju=nullptr;
			my_memory_bicgstab.tiw=nullptr;
			my_memory_bicgstab.tlevs=nullptr;
			my_memory_bicgstab.tw=nullptr;
			my_memory_bicgstab.tjw=nullptr;
			my_memory_bicgstab.icount_vel=100000; // ����� ������� �����.
            doublereal* rthdsdt=nullptr;
			rthdsdt=new doublereal[t.maxelm+t.maxbound];


			// ���������� �������� �������������� �� ���� ���������� �����.
			for (integer i47 = 0; i47 < t.maxelm; i47++) {
				// �������� � ��� ��� �������� �� ����������� ��� ������ � ������ ��������.
				integer ib = t.whot_is_block[i47];
				t.Sc[i47] = get_power(b[ib].n_Sc, b[ib].temp_Sc, b[ib].arr_Sc, t.potent[i47]);
			}

			

			doublereal** rsumanbstuff=nullptr; // nullptr ������� ��� ����� ������������ �������������
			doublereal rfluent_res_temp = 0.0;
		    solve(TEMP, res, fglobal[0], fglobal,
				  t, rhie_chow, s, w, b, ls, lw, lb, 
				  BETA_PRECISION, flow_interior, bconvective,
				  bfirst_start, nullptr, nullptr, nullptr, nullptr, tauparam,
				  btimedep, dgx, dgy, dgz, matlist,
				  inumiter, consolemessage, RCh,bVeryStable,
				  nullptr,rsumanbstuff,bhighorder,bdeltapfinish, 1.0, 
				  my_memory_bicgstab, rthdsdt, rfluent_res_temp, lu, my_union, color_solid, dist_max_solid);
			
			// ��������� �������� ������ ������� �������� ��� �������� ������� !
			doublereal tmax = -1.0e30;
			doublereal tmin = 1.0e30;
			doublereal tmax_FLUID = -1.0e30;
			for (integer i1 = 0; i1 < t.maxelm + t.maxbound; i1++) {
				tmax = fmax(tmax, t.potent[i1]);
				tmin = fmin(tmin, t.potent[i1]);
				if (i1 < t.maxelm) {
					if ((t.ptr != nullptr) && (t.ptr[1][i1] > -1)) {
					    // ��� FLUID ������
						tmax_FLUID = fmax(tmax_FLUID, t.potent[i1]);
					}
				}
			}
			printf("\n");
			printf("minimum temperature in default interior is %1.4e\n", tmin);
			printf("maximum temperature in default interior is %1.4e\n",tmax);
			if (tmax_FLUID >= -273.15) {//30.10.2019
				printf("maximum temperature in FLUID interior is %1.4e\n", tmax_FLUID);
			}
			printf("\n");

			// ������������ ����������� ������.
            delete[] rthdsdt;

		    for (integer i=0; i<3; i++) {
				delete[] rhie_chow[i];
				rhie_chow[i]=nullptr;
			}
		    delete[] rhie_chow;
			rhie_chow=nullptr;

		    // 25.03.2019
			// ������� ���������� ���������� � ��������� tecplot360:
			if (!b_on_adaptive_local_refinement_mesh) {
				exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, 0, bextendedprint, 0,b,lb);
			}
			else {
				ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, fglobal, 0, b, lb);
			}
		}
		
		
	}


	delete[] color;
	delete[] color_solid;

	//*/
	//getchar();

	if (0) {
		// �������������� � ������ �������� 1908�.:
		boundarylayer_info(fglobal, t, flow_interior, w, lw);
	}

} // steady_cfd_calculation


// �������������� cfd ��������:
void usteady_cfd_calculation(bool breadOk, EQUATIONINFO &eqin, 
	                        doublereal dgx, doublereal dgy, doublereal dgz,
							doublereal* continity_start, 
							integer* inumber_iteration_SIMPLE,
							integer flow_interior, 
							FLOW* &fglobal, TEMPER &t,
							BLOCK* b, integer lb, SOURCE* s, integer ls,
							WALL* w, integer lw, TPROP* matlist,
							TEMP_DEP_POWER* gtdps, integer ltdp, bool bextendedprint,
	                        integer lu, UNION* &my_union, integer inx, doublereal* &xpos)
{


	integer* color = nullptr;
	
	if ((!b_on_adaptive_local_refinement_mesh) && (number_cores() == 2) && (my_amg_manager.lfil < 3)) {
		// �������� ������ ��� ����������������� �����.
		integer isize = 0;
		//if ((iVar == VX) || (iVar == VY) || (iVar == VZ) || (iVar == PAM) || (iVar == NUSHA) ||
		//(iVar == TURBULENT_KINETIK_ENERGY) || (iVar == TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA) ||
		//(iVar == TURBULENT_KINETIK_ENERGY_STD_K_EPS) || (iVar == TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS))
		{
			integer n = fglobal[0].maxelm + fglobal[0].maxbound;
			color = new integer[n];
			for (integer i_1 = 0; i_1 < n; i_1++) color[i_1] = 0; // initialization
																  // ����� �� ����.
			doublereal max = -1.0e60;
			doublereal min = 1.0e60;
			/*
			for (integer i = 0; i < fglobal[0].maxelm; i++) {
			TOCHKA point0;
			center_cord3D(i, fglobal[0].nvtx, fglobal[0].pa, point0, 100);
			if (point0.x > max) max = point0.x;
			if (point0.x < min) min = point0.x;
			}
			doublereal avg = 0.5 * (min + max);
			*/
			doublereal avg = xpos[(integer)(0.5*inx)];

			
			doublereal dx = 0.0, dy = 0.0, dz = 0.0; // ����� �������� ������������ ������
			integer iP = -1;
			max = 1.0e60;
			for (integer i = 0; i < fglobal[0].maxelm; i++) {
				TOCHKA point0;
				center_cord3D(i, fglobal[0].nvtx, fglobal[0].pa, point0, 100);
				if (fabs(avg - point0.x) < max) {
					max = fabs(avg - point0.x);
					min = point0.x;
					iP = i;
				}
			}

			integer il = 0, ic = 0, ir = n;
			bool bcontinue = true;
			while ((bcontinue) && (abs(ir - il) > 1.4 * ic)) {

				isize = 0;
				il = 0; ir = 0; ic = 0;// �������������.

				TOCHKA point1;
				center_cord3D(iP, fglobal[0].nvtx, fglobal[0].pa, point1, 100);
				avg = point1.x;
				volume3D(iP, fglobal[0].nvtx, fglobal[0].pa, dx, dy, dz);
				dx = fabs(dx);
				dy = fabs(dy);
				dz = fabs(dz);

				for (integer i = 0; i < fglobal[0].maxelm; i++) {
					TOCHKA point0;
					center_cord3D(i, fglobal[0].nvtx, fglobal[0].pa, point0, 100);
					if (point0.x < avg - 0.4 * dx) {
						color[i] = 1;
						il++;
					}
					else if (point0.x > avg + 0.4 * dx) {
						color[i] = 3;
						ir++;
					}
					else {
						color[i] = 2;
						isize++;
						ic++;
					}
				}
				for (integer iB = 0; iB < fglobal[0].maxbound; iB++) {
					integer i = fglobal[0].border_neighbor[iB].iI;
					if ((i >= 0) && (i < fglobal[0].maxelm)) {
						TOCHKA point0;
						center_cord3D(i, fglobal[0].nvtx, fglobal[0].pa, point0, 100);
						if (point0.x < avg - 0.4 * dx) {
							color[fglobal[0].maxelm + iB] = 1;
							il++;
						}
						else if (point0.x > avg + 0.4 * dx) {
							color[fglobal[0].maxelm + iB] = 3;
							ir++;
						}
						else {
							color[fglobal[0].maxelm + iB] = 2;
							isize++;
							ic++;
						}
					}
					else {
						printf("error iI =%lld\n", i);
						system("pause");
					}
				}

				printf("ileft=%lld center=%lld right=%lld\n", il, ic, ir);
				if (ir > il) {
					// ���� ���� fglobal[0].neighbors_for_the_internal_node[ESIDE][iP].iNODE1; ����������.
					integer icP = fglobal[0].neighbors_for_the_internal_node[E_SIDE][iP].iNODE1;
					if ((icP >= 0) && (icP < fglobal[0].maxelm)) {
						iP = icP;
					}
					else {
						bcontinue = false;
					}
				}
				else if (ir < il) {
					// ���� ���� fglobal[0].neighbors_for_the_internal_node[WSIDE][iP].iNODE1; ����������.
					integer icP = fglobal[0].neighbors_for_the_internal_node[W_SIDE][iP].iNODE1;
					if ((icP >= 0) && (icP < fglobal[0].maxelm)) {
						iP = icP;
					}
					else {
						bcontinue = false;
					}
				}
			}
		}

		printf("separator size=%lld\n", isize);
		//getchar();
	}


	integer* color_solid = nullptr;
	integer dist_max_solid = 3;
	calculate_color_for_temperature(color_solid, t,inx,xpos);


	// ��������� RCh ��� �������� ���-��� ����������� ������ ���� ����� 1.0 ����� ��������� ��������� ����������.
	// �� ��� � ��������� ������������ ���������� ������������� ���������� ��������� ��� �������� ���-��� ������ 0.1
	// (��� ���������� ��������� ����� �������� ���-��� � 10 ���) �� ���������� ������������:
	// ��. ��������� ��������� � �������� ������.
	doublereal RCh = 1.0; // 1.0; 0.1;
						  //RCh = my_amg_manager.F_to_F_Stress;//debug

	if (0) {
		xyplot(fglobal, flow_interior, t);
		printf("steady cfd calc presolve. OK.\n");
		//getchar(); // debug
		system("pause");
	}


	// ������� � ����� Fluent.
	FLUENT_RESIDUAL rfluentres;
	rfluentres.operating_value_b=1.0; // ������������� ��������� ��������.
	doublereal rfluentrestemp=1.0; // ������� � ����� fluent ��� �����������.

								   // ����� �������.
	unsigned int calculation_start_time = 0; // ������ ����� ��.
	unsigned int calculation_end_time = 0; // ��������� ����� ��.
	unsigned int calculation_seach_time = 0; // ����� ���������� ������� ���� � ��.

	// ��� ������������ ������������� ����������� ��������.
	bool bprintmessage=false; // true; // �������� �� ��������� �� �������.

	// ������ ���������� ����������,
    // ��������� ��� �������� ������� ���-���
    doublereal **rhie_chow=nullptr;

	///* 
    FILE *fpcont=NULL; // ���� � ������� ����� ������������ �������
	errno_t err;
	// �������� ����� ��� ������ �������� ������� continity
	// continity - ������������������ ��������� ����� ������� 
	// ������ ���� ���������������.
	// continity - ���������� ���������� ���� ������� ����������������� ���������.
	bool bcontinuecontinity=false;
#ifdef MINGW_COMPILLER
	err = 0;
	if (!breadOk) {
		// ���������� �� ����� avtosave.txt �� �������
		fpcont = fopen64("continity.txt", "w");
	}
	else {
		// �������� ���� ������� �� ����� avtosave.txt
		fpcont = fopen64("continity.txt", "a");
	}
	if (fpcont == NULL) err = 1;
#else
	if (!breadOk) {
		// ���������� �� ����� avtosave.txt �� �������
		err = fopen_s(&fpcont, "continity.txt", "w");
	}
	else {
		// �������� ���� ������� �� ����� avtosave.txt
		err = fopen_s(&fpcont, "continity.txt", "a");
	}
#endif
	
	if (err == 0)  bcontinuecontinity=true;
	else {
         printf("Create File continity.txt Error\n");
		 system("pause");
         exit(0);
	}

	
	if (bcontinuecontinity) {

		// ���������� �� ����� avtosave ������ �������.

		if (flow_interior > 0) {

			// � ������ ������������ ����������������� ����������.

			// �  ���� statistic_convergence.txt ����� ������������ ���������� � ������
            // �������� ��������� ����.
            FILE *fp_statistic_convergence=NULL;

			errno_t err_stat=0;
#ifdef MINGW_COMPILLER
			err_stat = 0;
			fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
			if (fp_statistic_convergence == NULL) err_stat = 1;
#else
			err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
			
			// �������� ����� ��� ������ �������� ������� � 
			// � �������� �������� �������� ����.
			// �������� ��������� ������� ��������� ������ � �������� ���������� ��� ������������ ���� ������� ����������������� ���������.
			// (�.�. ��� �������� ������� � �������� ���� ����������� �������� ����� dterminatedTResudual)
			if ((err_stat) != 0) {
				printf("Create File continity.txt Error\n");
				//getchar();
				system("pause");
				exit(0);
			}
			else {


				for (integer iflow = 0; iflow < flow_interior; iflow++) {
					// ���� ������ ����������������� ���������� ��������� ���������:
					if (eqin.fluidinfo[iflow].iflow == 1) {
						// ������������� ����������������� ���������� � ������� iflow.

						if (fglobal[iflow].bLR1free) {
							// ��� ������ ����������������� ���������� �� ��� ��������� ����� ���������� ������� ������� ���
							// �������� ��������. �� ���� ������� ������������ ������������.
							printf("WARNING! bLR1free is true. All neiman condition for PAmendment.\n");
							// getchar();
						}

						// ���� ����� ���������� � ���������� ������� ������ ��� ���������� � ���� ����������.

						// ������ ���� ������ ���:
						// � ����������� ���������� ���� ��������� �������:
						// a. ���� ����������; b. ���� ���������; c. ���������������� �������� ���-��� (���� �������� ��� ������� ��, ��� ��������� ��).

						// �����������:
						doublereal* toldtimestep = new doublereal[t.maxelm + t.maxbound]; // ���� ���������� �� ���������� ��������� ����
						for (integer i1 = 0; i1 < t.maxelm + t.maxbound; i1++) {
							toldtimestep[i1] = t.potent[i1]; // copy �������������
						}

						// ���� ��������:
						// ��������� ������:
						doublereal** speedoldtimestep = new doublereal*[3];							
						for (integer i2 = 0; i2 < 3; i2++) {
							speedoldtimestep[i2] = new doublereal[fglobal[iflow].maxelm + fglobal[iflow].maxbound];
						}
						
						// �������������:
						for (integer i2 = 0; i2 < 3; i2++) {
							for (integer i3 = 0; i3 < (fglobal[iflow].maxelm + fglobal[iflow].maxbound); i3++) {
								// iflow - ����� FLUID INTERIOR,
								// i2 - VX, VY, VZ - ���� �� ��� ��������� ��������,
								// i3 - ��������������� ����� ������������ ������ (����������
								speedoldtimestep[i2][i3] = fglobal[iflow].potent[i2][i3]; // copy �������������
								//printf("%e %e %e\n",fglobal[iflow].potent[VX][i3],fglobal[iflow].potent[VY][i3],fglobal[iflow].potent[VZ][i3]);
								//printf("%e %e %e\n",fglobal[iflow].potent[VXCOR][i3],fglobal[iflow].potent[VYCOR][i3],fglobal[iflow].potent[VZCOR][i3]);
								//getchar(); // debug
							}
						}
						

						/*
						for (integer
						for (integer iP=0; iP<fglobal[iflow].maxelm; iP++) {

									   // ��������� ����������������� �������� ����� ����� ����� ��.
									   // �������� ����� ����������� �� ������� �������� �� � ������
									   // ������ ��� ���������������� �������� ���-���. ��� ��� ���������� ������������
									   // ������� �������� ������������ �������� �� ����� ��.

									   bool bsimplelinearinterpol=true; // ����������� ������� �������� ������������ �������� �� �����.

									   return_calc_correct_mass_flux(iP,
																	 fglobal[iflow].potent,
																	 fglobal[iflow].pa,
																	 fglobal[iflow].prop,
																	 fglobal[iflow].prop_b,
																	 fglobal[iflow].nvtx,
																	 fglobal[iflow].neighbors_for_the_internal_node,
																	 fglobal[iflow].maxelm,
																	 fglobal[iflow].diag_coef,
																	 fglobal[iflow].alpha,
																	 RCh,
																	 false,
																	 0.01,
																	 nullptr,
																	 fglobal[iflow].mf[iP], // ������������ �������� ��������� ������
																	 nullptr,bsimplelinearinterpol,
																	 SpeedCorOld, mfold[iP]);

									   if (fglobal[iflow].smaginfo.bDynamic_Stress) {
										   smagconstolditer[iP]=0.0; // ��������� ��������
									   }
								   }
								   */
								   // �������� ����� ����� ����� �� � ���������� ��������.
								   // ��� ���������� �� ����� avtosave.txt ��� �������� ����� ������ �����������.
								   // ���� ����� ����������� ���������� ���������� � �������� �������� (���� �������� ��������� ����������).
								   // ��������� ������:
						doublereal** mfoldtimestep = new doublereal*[fglobal[iflow].maxelm];							
						for (integer i2 = 0; i2 < fglobal[iflow].maxelm; i2++) {
							mfoldtimestep[i2] = new doublereal[6];
						}
						
						// �������������:
						for (integer i2 = 0; i2 < fglobal[iflow].maxelm; i2++) {
							for (integer i3 = 0; i3 < 6; i3++) {
								mfoldtimestep[i2][i3] = fglobal[iflow].mf[i2][i3]; // copy �������������
							}
							//printf("%e %e %e %e %e %e\n",fglobal[iflow].mf[i2][0],fglobal[iflow].mf[i2][1],fglobal[iflow].mf[i2][2],fglobal[iflow].mf[i2][3],fglobal[iflow].mf[i2][4],fglobal[iflow].mf[i2][5]);
							//getchar(); // debug
						}
					

											

						

#if doubleintprecision == 1
						fprintf(fpcont, " Evalution residual for flow interior=%lld\n", iflow);
						fprintf(fpcont, " iter \t\t continity\n");
						fprintf(fp_statistic_convergence, " Statistic convergence for flow interior=%lld\n", iflow);
#else
						fprintf(fpcont, " Evalution residual for flow interior=%d\n", iflow);
						fprintf(fpcont, " iter \t\t continity\n");
						fprintf(fp_statistic_convergence, " Statistic convergence for flow interior=%d\n", iflow);
#endif
						if (fglobal[0].iflowregime == RANS_STANDART_K_EPS) {
							if (eqin.itemper == 1) {
								fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM     energy     k	epsilon \n");
							}
							else {
								fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM		k	   epsilon\n");
							}
						}
						else
						if (fglobal[0].iflowregime == RANS_MENTER_SST) {
							if (eqin.itemper == 1) {
								fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM     energy     k	omega \n");
							}
							else {
								fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM		k	   omega\n");
							}
						}
						else if (fglobal[0].iflowregime == RANS_SPALART_ALLMARES) {
							if (eqin.itemper == 1) {
								fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM     energy     nut	 \n");
							}
							else {
								fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM		nut	\n");
							}
						}
						else {
							if (eqin.itemper == 1) {
								fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM     energy      \n");
							}
							else {
								fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM     \n");
							}
						}
						fclose(fp_statistic_convergence);


						

#if doubleintprecision == 1
						printf("fluid interior number %lld: maxelm=%lld, maxbound=%lld\n", iflow, fglobal[iflow].maxelm, fglobal[iflow].maxbound);
#else
						printf("fluid interior number %d: maxelm=%d, maxbound=%d\n", iflow, fglobal[iflow].maxelm, fglobal[iflow].maxbound);
#endif
						printf("please, press any key to start calculation...\n");
						if (bwait) {
							//getchar();
							system("pause");
						}
						calculation_start_time = clock(); // ������ ������ �����.
						bool bfirst = true;


						// ���� ������ ���������� ��� ����������� ���� ���������.
						// ���� ���� ��������� �������� � �������� ������� �� ��� ������� ����������.

						// ������� ������ (� ���� ������ ���������� �� ��������� �������������).
						// � ������������� �� ������ ������ ���� ������ ��� �������� ��������� �������������
						// � ������ ������� ���� �� ������� �� ����� (���� ����� ���� ��� � ������ � ����������� �����).
						// � ����� ��������� ������������� ������ �������� �������� �� ��������� ����� ��������������� ��������,
						// ����� ����� �������������� ������ ����������� ���������� 0.001 � ����� ��������� ��������� ��������� �������
						// ��� �������������� ������� (�.�. ��� �� ������� ����� ���������� ��� �������������� �������� ����������).

						doublereal* smagconstolditer = nullptr;
						if (fglobal[iflow].smaginfo.bDynamic_Stress) {
							smagconstolditer = new doublereal[fglobal[iflow].maxelm];
							if (smagconstolditer == nullptr) {
								// ������������ ������ �� ������ ������������.
								printf("Problem: not enough memory on your equipment for smagconstolditer steady cfd calculation...\n");
								printf("Please any key to exit...\n");
								exit(1);
							}
						}

						// ���������� ����������������� �������� � ���������� ��������.
						doublereal **SpeedCorOld = nullptr;
						SpeedCorOld = new doublereal*[3];
						if (SpeedCorOld == nullptr) {
							// ������������ ������ �� ������ ������������.
							printf("Problem: not enough memory on your equipment for SpeedCorOld steady cfd calculation...\n");
							printf("Please any key to exit...\n");
							exit(1);
						}
						for (integer i = 0; i<3; i++) {
							SpeedCorOld[i] = nullptr;
							SpeedCorOld[i] = new doublereal[fglobal[iflow].maxelm + fglobal[iflow].maxbound];
							if (SpeedCorOld[i] == nullptr) {
								// ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
								printf("Problem: not enough memory on your equipment for SpeedCorOld[%lld] steady cfd calculation...\n", i);
#else
								printf("Problem: not enough memory on your equipment for SpeedCorOld[%d] steady cfd calculation...\n", i);
#endif
								printf("Please any key to exit...\n");
								exit(1);
							}
						}
						for (integer i = 0; i<3; i++) {
							for (integer j = 0; j<fglobal[iflow].maxelm + fglobal[iflow].maxbound; j++) {
								switch (i) {
								case VELOCITY_X_COMPONENT: SpeedCorOld[VELOCITY_X_COMPONENT][j] = fglobal[iflow].potent[VXCOR][j];
									break;
								case VELOCITY_Y_COMPONENT: SpeedCorOld[VELOCITY_Y_COMPONENT][j] = fglobal[iflow].potent[VYCOR][j];
									break;
								case VELOCITY_Z_COMPONENT: SpeedCorOld[VELOCITY_Z_COMPONENT][j] = fglobal[iflow].potent[VZCOR][j];
									break;
								}
							}
						}

						doublereal **mfold = new doublereal*[fglobal[iflow].maxelm];
						for (integer i = 0; i<fglobal[iflow].maxelm; i++) {
							mfold[i] = new doublereal[6];
						}

						for (integer i = 0; i < fglobal[iflow].maxelm; i++) {
							for (integer j = 0; j < 6; j++) {
								mfold[i][j] = fglobal[iflow].mf[i][j]; // ��������� �����.
							}
						}

						for (integer iP = 0; iP<fglobal[iflow].maxelm; iP++) {

							// ��������� ����������������� �������� ����� ����� ����� ��.
							// �������� ����� ����������� �� ������� �������� �� � ������
							// ������ ��� ���������������� �������� ���-���. ��� ��� ���������� ������������
							// ������� �������� ������������ �������� �� ����� ��.

							bool bsimplelinearinterpol = true; // ����������� ������� �������� ������������ �������� �� �����.

															   // 25.03.2019 ������ �������� �� ���� �����.
							return_calc_correct_mass_flux(iP,
								fglobal[iflow].potent,
								fglobal[iflow].pa,
								fglobal[iflow].prop,
								fglobal[iflow].prop_b,
								fglobal[iflow].nvtx,
								fglobal[iflow].neighbors_for_the_internal_node,
								fglobal[iflow].maxelm,
								fglobal[iflow].diag_coef,
								fglobal[iflow].alpha,
								RCh,
								false,
								0.01,
								nullptr,
								fglobal[iflow].mf[iP], // ������������ �������� ��������� ������
								nullptr, bsimplelinearinterpol,
								SpeedCorOld, mfold[iP],
								fglobal[iflow].border_neighbor,
								t.ilevel_alice,
								fglobal[iflow].ptr);

							if (fglobal[iflow].smaginfo.bDynamic_Stress) {
								smagconstolditer[iP] = 0.0; // ��������� ��������
							}
						}

						// �������������.
						for (integer i = 0; i < fglobal[iflow].maxelm; i++) {
							for (integer j = 0; j < 6; j++) {
								mfold[i][j] = fglobal[iflow].mf[i][j]; // ��������� �����.
								mfoldtimestep[i][j]= fglobal[iflow].mf[i][j]; // ��������� �����.
							}
						}


						// ������������ ����������� ������ �� ����.
						if (SpeedCorOld != nullptr) {
							for (integer i = 0; i < 3; i++) {
								if (SpeedCorOld[i] != nullptr) {
									delete[] SpeedCorOld[i];
									SpeedCorOld[i] = nullptr;
								}
							}
							delete[] SpeedCorOld;
						}
						SpeedCorOld = nullptr;


						// ������� ����� �� ������� � ���������� � ���������� ��������.
						integer iN = 0; // ���������� ����� �� �������
						doublereal* timestep_sequence = nullptr; // ������������������ ����� �� �������.
						// ���������� � ������ �������� �� ������ ��������� ����
						doublereal* poweron_multiplier_sequence = nullptr; // (��������� ������� �������� ������� �� ����������).
						doublereal StartTime = 0.0, EndTime = 17.0; // ������������ (� �).
						doublereal TimeStepIncrement = 0.5; // ��������� ��� �� ������� 1�. (������������ � ���������� ���� �� �������.)
						// ���������� ��� �� �������:
						uniform_timestep_seq(StartTime, EndTime, TimeStepIncrement, iN, timestep_sequence, poweron_multiplier_sequence);
						// ���������� �������� ��� �� �������:
						//linear_timestep_seq(StartTime, EndTime, 1e-4, 1.5, iN, timestep_sequence, poweron_multiplier_sequence);

#if doubleintprecision == 1
						printf("number of time step iN=%lld\n", iN);
#else
						printf("number of time step iN=%d\n", iN);
#endif


						//getchar();

						if (iN <= 0) {
							// ������ � ������� ����� �� �������.
							printf("error in setting the time steps...\n");
							printf("please press any key to exit...\n");
							//getchar();
							system("pause");
							exit(0);
						}

						doublereal phisicaltime = StartTime;
						bool btimedep = true; // �������������� ������
						integer i_gl = 0;


						// ���������� ����������������� �������� � ���������� ��������.
						doublereal** SpeedCorOldinternal = new doublereal * [3];
						for (integer i = 0; i < 3; i++) {
							SpeedCorOldinternal[i] = new doublereal[fglobal[iflow].maxelm + fglobal[iflow].maxbound];
						}
						doublereal* xb = new doublereal[fglobal[iflow].maxelm + fglobal[iflow].maxbound];
						doublereal* rthdsd = nullptr; // ������ ����� ������� ���������.
						doublereal* rthdsdt = nullptr;
						rthdsd = new doublereal[fglobal[iflow].maxelm + fglobal[iflow].maxbound];
						rthdsdt = new doublereal[t.maxelm + t.maxbound];


						// �������������� ������:
						for (integer j = 0; j < iN; j++) {

							rfluentres.operating_value_b = 1.0; // ������������� ��������� ��������.

							// ��������� ������� ������������� �� �������, ���� ������ ������� ��� ��������
							phisicaltime += timestep_sequence[j];

							
									// ��������� ����������� ������ ��� �������� Rhie-Chow
									my_malloc2(rhie_chow, fglobal[iflow].maxelm);



									doublereal continity = 1.0; // �������������

									
									printf("phisical time = %e\n", phisicaltime);

									// ������������ �������� �� ������ ���� �� �������:
									bool bfirst = true;
									integer iend = 40; // ����� ��������.
									QuickMemVorst my_memory_bicgstab;
									my_memory_bicgstab.ballocCRScfd = false; // �������� ������.
									my_memory_bicgstab.bsignalfreeCRScfd = false; // �� ���������� ������ ��� ����.
									// ������������� ���������� !
									my_memory_bicgstab.val = nullptr;
									my_memory_bicgstab.col_ind = nullptr;
									my_memory_bicgstab.row_ptr = nullptr;
									my_memory_bicgstab.ri = nullptr;
									my_memory_bicgstab.roc = nullptr;
									my_memory_bicgstab.s = nullptr;
									my_memory_bicgstab.t = nullptr;
									my_memory_bicgstab.vi = nullptr;
									my_memory_bicgstab.pi = nullptr;
									my_memory_bicgstab.dx = nullptr;
									my_memory_bicgstab.dax = nullptr;
									my_memory_bicgstab.y = nullptr;
									my_memory_bicgstab.z = nullptr;
									my_memory_bicgstab.a = nullptr;
									my_memory_bicgstab.ja = nullptr;
									my_memory_bicgstab.ia = nullptr;
									my_memory_bicgstab.alu = nullptr;
									my_memory_bicgstab.jlu = nullptr;
									my_memory_bicgstab.ju = nullptr;
									my_memory_bicgstab.alu1 = nullptr;
									my_memory_bicgstab.jlu1 = nullptr;
									my_memory_bicgstab.ju1 = nullptr;
									my_memory_bicgstab.x1 = nullptr;
									my_memory_bicgstab.iw = nullptr;
									my_memory_bicgstab.levs = nullptr;
									my_memory_bicgstab.w = nullptr;
									my_memory_bicgstab.jw = nullptr;
									my_memory_bicgstab.w_dubl = nullptr;
									my_memory_bicgstab.jw_dubl = nullptr;
									// ������ ��������� � ����������� ������������� �������� � ��������� �������������.
									my_memory_bicgstab.ballocCRSt = false; // �������� ������
									my_memory_bicgstab.bsignalfreeCRSt = false; // � ����� �� �����������.
									// ������������� ����������.
									my_memory_bicgstab.tval = nullptr;
									my_memory_bicgstab.tcol_ind = nullptr;
									my_memory_bicgstab.trow_ptr = nullptr;
									my_memory_bicgstab.tri = nullptr;
									my_memory_bicgstab.troc = nullptr;
									my_memory_bicgstab.ts = nullptr;
									my_memory_bicgstab.tt = nullptr;
									my_memory_bicgstab.tvi = nullptr;
									my_memory_bicgstab.tpi = nullptr;
									my_memory_bicgstab.tdx = nullptr;
									my_memory_bicgstab.tdax = nullptr;
									my_memory_bicgstab.ty = nullptr;
									my_memory_bicgstab.tz = nullptr;
									my_memory_bicgstab.ta = nullptr;
									my_memory_bicgstab.tja = nullptr;
									my_memory_bicgstab.tia = nullptr;
									my_memory_bicgstab.talu = nullptr;
									my_memory_bicgstab.tjlu = nullptr;
									my_memory_bicgstab.tju = nullptr;
									my_memory_bicgstab.tiw = nullptr;
									my_memory_bicgstab.tlevs = nullptr;
									my_memory_bicgstab.tw = nullptr;
									my_memory_bicgstab.tjw = nullptr;
									my_memory_bicgstab.icount_vel = 100000; // ����� ������� �����.

									

									/*
									for (integer i3 = 0; i3 < (fglobal[iflow].maxelm + fglobal[iflow].maxbound); i3++) {
										// ����� ������ ����� ����� �� ������� �� �������� ���������� ��������.
										// �������� � ��� ��� �������� ��������� ���������� �������� � �������� ��� �� ����������,
										// �� ������ ��������� ���� ��������� ������. 15.05.2019
										fglobal[iflow].potent[PRESS][i3] = 0.0;
										fglobal[iflow].potent[GRADXPRESS][i3] = 0.0;
										fglobal[iflow].potent[GRADYPRESS][i3] = 0.0;
										fglobal[iflow].potent[GRADZPRESS][i3] = 0.0;
									}
									*/

									for (integer i = inumber_iteration_SIMPLE[iflow] + 1; i < iend; i++) {

										// ������� �� ������������� ���������� � ��������� �������.
										integer iseparate_SIMPLE = 10000;
										bool bseparate_SIMPLE = true;// ������ ������ ���� ���.

										doublereal start_average_continity = 0.0;

										if (i == iend - 1) {
											my_memory_bicgstab.bsignalfreeCRScfd = true;
											my_memory_bicgstab.bsignalfreeCRSt = true; // ����������� ������ �� ��������� ��������.
										}



										
										
										// ��������� ������ ���������� ������ ������
										// ���� ����������������, ��������, �. ����������. 0.5; 0.8;
										// � ����� Ferczinger and Peric �������������� ���������� ���������� ���������� ������: 0.7; 0.3; 
										// ��� �������� 0.7, � ��� �������� 0.3. ��� ���� ���������� ����� ������ ��� 0.7+0.3 == 1.0;
										if (!b_on_adaptive_local_refinement_mesh) {
											fglobal[iflow].alpha[VELOCITY_X_COMPONENT] = 0.7; // 0.8 0.5
											fglobal[iflow].alpha[VELOCITY_Y_COMPONENT] = 0.7; // 0.8 0.5
											fglobal[iflow].alpha[VELOCITY_Z_COMPONENT] = 0.7; // 0.8 0.5
											fglobal[iflow].alpha[PRESS] = 0.3; // 0.2 0.8
											fglobal[iflow].alpha[NUSHA_SL] = 0.8;// 1.0;// 0.7; 
											fglobal[iflow].alpha[TURBULENT_KINETIK_ENERGY_SL] = 0.8;
											fglobal[iflow].alpha[TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA_SL] = 0.8;
										}
										else {
											fglobal[iflow].alpha[VELOCITY_X_COMPONENT] = 0.8; // 0.8 0.5
											fglobal[iflow].alpha[VELOCITY_Y_COMPONENT] = 0.8; // 0.8 0.5
											fglobal[iflow].alpha[VELOCITY_Z_COMPONENT] = 0.8; // 0.8 0.5
											fglobal[iflow].alpha[PRESS] = 0.2;// 0.05; // 0.2 0.8
											fglobal[iflow].alpha[NUSHA_SL] = 0.8;// 1.0; // 0.8
											fglobal[iflow].alpha[TURBULENT_KINETIK_ENERGY_SL] = 0.8;
											fglobal[iflow].alpha[TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA_SL] = 0.8;
										}
										
										
										bool bfirst_start = false;
										if ((i == (inumber_iteration_SIMPLE[iflow] + 1)) && bfirst) {
											bfirst_start = true;
											bfirst = false; // ������ ������ ���� ������ ���� ���.
										}

										// ����� �������.
										unsigned int calculation_simple_start_time; // ������ ����� ��.
										unsigned int calculation_simple_end_time; // ��������� ����� ��.
										unsigned int calculation_simple_seach_time; // ����� ���������� ������� ���� � ��.

										calculation_simple_start_time = clock(); // ������ ������ �����.

										doublereal dtimestepold = timestep_sequence[j];
										if (j > 0) {
											dtimestepold = timestep_sequence[j - 1];
										}

										integer dist_max_fluid = 3;

										// �������������� �������� SIMPLE �������� � �������� 1972 ���.
										my_version_SIMPLE_Algorithm3D(continity, i,
											fglobal[iflow],
											fglobal,
											t, rhie_chow,
											b, lb, s, ls, w, lw,
											BETA_PRECISION,
											flow_interior,
											iflow,
											bfirst_start,
											dgx, dgy, dgz,
											matlist,
											btimedep,
											timestep_sequence[j],//!!!
											dtimestepold,//!!!
											phisicaltime,//!!!
											toldtimestep,//!!!
											speedoldtimestep,//!!!
											mfoldtimestep,//!!!
											bprintmessage,
											gtdps, ltdp,
											rfluentres, rfluentrestemp,
											smagconstolditer,
											mfold, eqin.itemper, my_memory_bicgstab,
											bextendedprint, SpeedCorOldinternal, xb,
											rthdsd, rthdsdt, lu, my_union, color, dist_max_fluid, color_solid, dist_max_solid);

										calculation_simple_end_time = clock();
										calculation_simple_seach_time = calculation_simple_end_time - calculation_simple_start_time;
										unsigned int im = 0, is = 0, ims = 0;
										im = (unsigned int)(calculation_simple_seach_time / 60000); // ������
										is = (unsigned int)((calculation_simple_seach_time - 60000 * im) / 1000); // �������
										ims = (unsigned int)((calculation_simple_seach_time - 60000 * im - 1000 * is) / 10); // ������������ ������� �� 10

#if doubleintprecision == 1
								//if (i==5) continity_start[iflow]=continity;
										if (i <= 5) {
											fprintf(fpcont, "%lld 1.0\n", i + 1);
											if (!bprintmessage) {
												if (eqin.itemper == 0) {
													// ������� ������ ������������� ��� ��������� ����������������
													if (fglobal[0].iflowregime == RANS_STANDART_K_EPS) {
														//printf("%lld 1.0\n",i+1);
														printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u %lld\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_turb_kinetik_energy_std_ke,
															rfluentres.res_turb_epsilon, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity k     epsilon\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentres.res_turb_kinetik_energy_std_ke,
																rfluentres.res_turb_epsilon);
															fclose(fp_statistic_convergence);
														}
													}
													else
													if (fglobal[0].iflowregime == RANS_MENTER_SST) {
														//printf("%lld 1.0\n",i+1);
														printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u %lld\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_turb_kinetik_energy,
															rfluentres.res_turb_omega, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity k     omega\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentres.res_turb_kinetik_energy,
																rfluentres.res_turb_omega);
															fclose(fp_statistic_convergence);
														}
													}
													else if (fglobal[0].iflowregime == RANS_SPALART_ALLMARES) {
														//printf("%lld 1.0\n",i+1);
														printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u %lld\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_nusha, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity nut	\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentres.res_nusha);
															fclose(fp_statistic_convergence);
														}
													}
													else {
														//printf("%lld 1.0\n",i+1);
														printf(" %lld %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u %lld\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity \t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance);
															fclose(fp_statistic_convergence);
														}
													}
												}
												else if (eqin.itemper == 1) {
													// ������� ������������� ��������� � ���������� ����������������.
													if (fglobal[0].iflowregime == RANS_STANDART_K_EPS) {
														printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u %lld\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp,
															rfluentres.res_turb_kinetik_energy_std_ke, rfluentres.res_turb_epsilon,
															im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity temperature k		 epsilon\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp,
																rfluentres.res_turb_kinetik_energy_std_ke,
																rfluentres.res_turb_epsilon);
															fclose(fp_statistic_convergence);
														}
													}
													else
													if (fglobal[0].iflowregime == RANS_MENTER_SST) {
														printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u %lld\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp,
															rfluentres.res_turb_kinetik_energy, rfluentres.res_turb_omega,
															im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity temperature k		omega\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp, rfluentres.res_turb_kinetik_energy,
																rfluentres.res_turb_omega);
															fclose(fp_statistic_convergence);
														}
													}
													else if (fglobal[0].iflowregime == RANS_SPALART_ALLMARES) {
														printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u %lld\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, rfluentres.res_nusha, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity temperature nut	\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp, rfluentres.res_nusha);
															fclose(fp_statistic_convergence);
														}
													}
													else {
														printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u %lld\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity temperature \t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp);
															fclose(fp_statistic_convergence);
														}
													}
												}
											}
											continity_start[iflow] = continity;
											rfluentres.operating_value_b = rfluentres.res_no_balance;
										}
										else {
											fprintf(fpcont, "%lld %e\n", i + 1, continity / continity_start[iflow]); // ���������� � ����������
											if (!bprintmessage) {
												if (eqin.itemper == 0) {
													// ������� ������ ������������� ��� ��������� ����������������
													if (fglobal[0].iflowregime == RANS_STANDART_K_EPS) {
														//printf("%lld %e\n", i+1, continity/continity_start[iflow]); // ���������� � ����������
														printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u  %lld\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_turb_kinetik_energy_std_ke,
															rfluentres.res_turb_epsilon, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity k		epsilon\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentres.res_turb_kinetik_energy_std_ke,
																rfluentres.res_turb_epsilon);
															fclose(fp_statistic_convergence);
														}
													}
													else
													if (fglobal[0].iflowregime == RANS_MENTER_SST) {
														//printf("%lld %e\n", i+1, continity/continity_start[iflow]); // ���������� � ����������
														printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u  %lld\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_turb_kinetik_energy,
															rfluentres.res_turb_omega, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity k		omega\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentres.res_turb_kinetik_energy,
																rfluentres.res_turb_omega);
															fclose(fp_statistic_convergence);
														}
													}
													else if (fglobal[0].iflowregime == RANS_SPALART_ALLMARES) {
														//printf("%lld %e\n", i+1, continity/continity_start[iflow]); // ���������� � ����������
														printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u  %lld\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_nusha, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity nut	\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentres.res_nusha);
															fclose(fp_statistic_convergence);
														}
													}
													else {
														//printf("%lld %e\n", i+1, continity/continity_start[iflow]); // ���������� � ����������
														printf(" %lld %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u  %lld\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity \t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance);
															fclose(fp_statistic_convergence);
														}
													}
												}
												else if (eqin.itemper == 1) {
													// ������� ������������� ��������� � ���������� ����������������.
													if (fglobal[0].iflowregime == RANS_STANDART_K_EPS) {
														printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u  %lld\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, rfluentres.res_turb_kinetik_energy_std_ke,
															rfluentres.res_turb_epsilon, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity temperature k		epsilon\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 31 ������� 2019.
															fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp, rfluentres.res_turb_kinetik_energy_std_ke,
																rfluentres.res_turb_epsilon);
															fclose(fp_statistic_convergence);
														}
													}
													else
													if (fglobal[0].iflowregime == RANS_MENTER_SST) {
														printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u  %lld\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, rfluentres.res_turb_kinetik_energy,
															rfluentres.res_turb_omega, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity temperature k		omega\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp, rfluentres.res_turb_kinetik_energy,
																rfluentres.res_turb_omega);
															fclose(fp_statistic_convergence);
														}
													}
													else if (fglobal[0].iflowregime == RANS_SPALART_ALLMARES) {
														printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u  %lld\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, rfluentres.res_nusha, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity temperature nut	\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp, rfluentres.res_nusha);
															fclose(fp_statistic_convergence);
														}
													}
													else {
														printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u  %lld\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity temperature \t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %lld %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp);
															fclose(fp_statistic_convergence);
														}
													}
												}
											}
										}
#else
								//if (i==5) continity_start[iflow]=continity;
										if (i <= 5) {
											fprintf(fpcont, "%d 1.0\n", i + 1);
											if (!bprintmessage) {
												if (eqin.itemper == 0) {
													// ������� ������ ������������� ��� ��������� ����������������
													if (fglobal[0].iflowregime == RANS_STANDART_K_EPS) {
														//printf("%d 1.0\n",i+1);
														printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u %d\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_turb_kinetik_energy_std_ke,
															rfluentres.res_turb_epsilon, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity k     epsilon\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentres.res_turb_kinetik_energy_std_ke,
																rfluentres.res_turb_epsilon);
															fclose(fp_statistic_convergence);
														}
													}
													else
													if (fglobal[0].iflowregime == RANS_MENTER_SST) {
														//printf("%d 1.0\n",i+1);
														printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u %d\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_turb_kinetik_energy,
															rfluentres.res_turb_omega, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity k     omega\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentres.res_turb_kinetik_energy,
																rfluentres.res_turb_omega);
															fclose(fp_statistic_convergence);
														}
													}
													else if (fglobal[0].iflowregime == RANS_SPALART_ALLMARES) {
														//printf("%d 1.0\n",i+1);
														printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u %d\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_nusha, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity nut	\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentres.res_nusha);
															fclose(fp_statistic_convergence);
														}
													}
													else {
														//printf("%d 1.0\n",i+1);
														printf(" %d %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u %d\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity \t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance);
															fclose(fp_statistic_convergence);
														}
													}
												}
												else if (eqin.itemper == 1) {
													// ������� ������������� ��������� � ���������� ����������������.
													if (fglobal[0].iflowregime == RANS_STANDART_K_EPS) {
														printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u %d\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, rfluentres.res_turb_kinetik_energy_std_ke,
															rfluentres.res_turb_epsilon, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity temperature k     epsilon\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp,
																rfluentres.res_turb_kinetik_energy_std_ke,
																rfluentres.res_turb_epsilon);
															fclose(fp_statistic_convergence);
														}
													}
													else
													if (fglobal[0].iflowregime == RANS_MENTER_SST) {
														printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u %d\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, rfluentres.res_turb_kinetik_energy,
															rfluentres.res_turb_omega, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity temperature k     omega\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp, rfluentres.res_turb_kinetik_energy,
																rfluentres.res_turb_omega);
															fclose(fp_statistic_convergence);
														}
													}
													else if (fglobal[0].iflowregime == RANS_SPALART_ALLMARES) {
														printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u %d\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, rfluentres.res_nusha, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity temperature nut	\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp, rfluentres.res_nusha);
															fclose(fp_statistic_convergence);
														}
													}
													else {
														printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u %d\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity temperature \t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp);
															fclose(fp_statistic_convergence);
														}
													}
												}
											}
											continity_start[iflow] = continity;
											rfluentres.operating_value_b = rfluentres.res_no_balance;
										}
										else {
											fprintf(fpcont, "%d %e\n", i + 1, continity / continity_start[iflow]); // ���������� � ����������
											if (!bprintmessage) {
												if (eqin.itemper == 0) {
													// ������� ������ ������������� ��� ��������� ����������������
													if (fglobal[0].iflowregime == RANS_STANDART_K_EPS) {
														//printf("%d %e\n", i+1, continity/continity_start[iflow]); // ���������� � ����������
														printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u  %d\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_turb_kinetik_energy_std_ke,
															rfluentres.res_turb_epsilon, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity k     epsilon\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentres.res_turb_kinetik_energy_std_ke,
																rfluentres.res_turb_epsilon);
															fclose(fp_statistic_convergence);
														}
													}
													else
													if (fglobal[0].iflowregime == RANS_MENTER_SST) {
														//printf("%d %e\n", i+1, continity/continity_start[iflow]); // ���������� � ����������
														printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u  %d\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_turb_kinetik_energy,
															rfluentres.res_turb_omega, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity k     omega\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentres.res_turb_kinetik_energy,
																rfluentres.res_turb_omega);
															fclose(fp_statistic_convergence);
														}
													}
													else if (fglobal[0].iflowregime == RANS_SPALART_ALLMARES) {
														//printf("%d %e\n", i+1, continity/continity_start[iflow]); // ���������� � ����������
														printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u  %d\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_nusha, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity nut	\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentres.res_nusha);
															fclose(fp_statistic_convergence);
														}
													}
													else {
														//printf("%d %e\n", i+1, continity/continity_start[iflow]); // ���������� � ����������
														printf(" %d %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u  %d\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity \t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance);
															fclose(fp_statistic_convergence);
														}
													}
												}
												else if (eqin.itemper == 1) {
													// ������� ������������� ��������� � ���������� ����������������.
													if (fglobal[0].iflowregime == RANS_STANDART_K_EPS) {
														printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u  %d\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, rfluentres.res_turb_kinetik_energy_std_ke,
															rfluentres.res_turb_epsilon, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity temperature k    epsilon\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp, rfluentres.res_turb_kinetik_energy_std_ke,
																rfluentres.res_turb_epsilon);
															fclose(fp_statistic_convergence);
														}
													}
													else
													if (fglobal[0].iflowregime == RANS_MENTER_SST) {
														printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u  %d\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, rfluentres.res_turb_kinetik_energy,
															rfluentres.res_turb_omega, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity temperature k    omega\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp, rfluentres.res_turb_kinetik_energy,
																rfluentres.res_turb_omega);
															fclose(fp_statistic_convergence);
														}
													}
													else if (fglobal[0].iflowregime == RANS_SPALART_ALLMARES) {
														printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u  %d\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, rfluentres.res_nusha, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity temperature nut	\t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp, rfluentres.res_nusha);
															fclose(fp_statistic_convergence);
														}
													}
													else {
														printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1u:%2u:%2u  %d\n",
															i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
															rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, im, is, ims, iend - i);
														if (i % 10 == 0) {
															printf("  iter continity x-velocity y-velocity z-velocity temperature \t time/iter\n");
														}
#ifdef MINGW_COMPILLER
														err_stat = 0;
														fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
														if (fp_statistic_convergence == nullptr) err_stat = 1;
#else
														err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a");
#endif
														if ((err_stat) == 0) {
															// 29 ������� 2015.
															fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e\n", i_gl, rfluentres.res_vx,
																rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp);
															fclose(fp_statistic_convergence);
														}
									                }
												}
											}
										}
#endif

										i_gl++;

										bool breturn = false;
										//exporttecplotxy360( nve, maxelm, ncell, nvtx, nvtxcell, x, y, potent, rhie_chow);
										// ������� ���������� ���������� � ��������� tecplot360:
										if (1 && ((i + 1) % 10 == 0)) {
											// 25.03.2019
											// ������� ���������� ���������� � ��������� tecplot360:
											if (!b_on_adaptive_local_refinement_mesh) {
												exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, i, bextendedprint, 0,b,lb);
											}
											else {
												ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, fglobal, 0, b, lb);
											}
											printf("write values. OK.\n");
											//getchar(); // debug avtosave
											breturn = true;
										}
										if ((i + 1) % 20 == 0) {
											// ��������������
#if doubleintprecision == 1
											printf("avtosave...iter=%lld \n", i + 1);
#else
											printf("avtosave...iter=%d \n", i + 1);
#endif

											inumber_iteration_SIMPLE[iflow] = i;
											avtosave(fglobal, t, flow_interior, inumber_iteration_SIMPLE, continity_start);
											breturn = true;
										}

										if (breturn) printf("\n");

										if (0) {
											// �������� ���������� ������ ����
											if (i > 500) {
												printf("diagnosic pause...\n");
												//getchar();
												system("pause");
											}
										}

										if ((i == 6) || (i == iseparate_SIMPLE)) {
											start_average_continity = rfluentres.res_no_balance;
										}

										if ((i>20) && (rfluentres.res_no_balance / start_average_continity < 1.0e-12)) {
											// �� Fluent ����� ������� �� �������� ������� 1.0�-3 � ��� �������
											// ��� ������� ����� �������� �� ������� ���� ��� ���������� ������� ������� 
											// (����� 150 �������). � ���������� ������ ������ ���������� 
											// �������� ������� continity 1.0E-6 �� � ���� �� ����� ��������
											// ������ �� ������� � ������ ��������� ���� �� ������� ������� (����� 150 �������).
											// ��������� ������ ������� ���� ���������� ������� �� �������� 1.0E-6.
											if ((b_on_adaptive_local_refinement_mesh)) {
												// ��������� �����. ���������� ����������. ���������� ��������.
												//if (!bseparate_SIMPLE) {
													printf("\ncontinity < 1.0e-6. Dosrochnji vjhod. STOP.\n");
													i = iend;
												//}
												//else {
													//iseparate_SIMPLE = i + 1;
													//printf("\ncontinity < 1.0e-6. Dosrochnji vjhod. STOP.\n");
													//i = iend;
												//}
											}
											else {

												// ��������� �����. ���������� ����������. ���������� ��������.
												if (!bseparate_SIMPLE) {
													printf("\ncontinity < 1.0e-6. Dosrochnji vjhod. STOP.\n");
													i = iend;
												}
												else {
													iseparate_SIMPLE = i + 1;
												}

											}
										}

										// 28.07.2016
										// exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, i, bextendedprint);
										// getchar(); // debug

									} // ����� ����� �������� ��������� SIMPLE

									
									for (integer i = 0; i < 3; i++) {
										if (rhie_chow[i] != nullptr) {
											delete[] rhie_chow[i];
											rhie_chow[i] = nullptr;
										}
									}
									delete[] rhie_chow;
									rhie_chow = nullptr;
							


							// ���������� ��� �� �������:

							/* //���������������� 14.05.2019
							//������ ��������.
							char* buffer = nullptr;
							buffer = new char[10];
							buffer[0] = '\0';
							KRitoa(j, buffer);
							//printf("%s\n",buffer);
							char* mymessage = nullptr;
							mymessage = new char[30];
							mymessage[0] = '\0';
							KRstrcat(mymessage, "time_number=");
							//printf("%s\n",mymessage);
							KRstrcat(mymessage, buffer);
							//printf("%s\n",mymessage);
							//getchar();
							bool btitle = (j == 0); // �������� �� ���������.
							// �������� ��������.
							animationtecplot360T_3D_part2all(t.maxelm, t.ncell, fglobal, t, flow_interior, mymessage, btitle);
							if (buffer != nullptr) {
								delete[] buffer;
							}
							if (mymessage != nullptr) {
								delete[] mymessage;
							}
							*/

							// ���������� ���� ����������:
							for (integer i1 = 0; i1 < t.maxelm + t.maxbound; i1++) {
								toldtimestep[i1] = t.potent[i1]; // copy end time step
							}

							// ���������� ���� ��������:
							
								for (integer i3 = 0; i3 < (fglobal[iflow].maxelm + fglobal[iflow].maxbound); i3++) {
									// i1 - ����� FLUID INTERIOR,
									// i2 - VX, VY, VZ - ���� �� ��� ��������� ��������,
									// i3 - ��������������� ����� ������������ ������ 
									speedoldtimestep[VELOCITY_X_COMPONENT][i3] = fglobal[iflow].potent[VELOCITY_X_COMPONENT][i3]; // copy end time step
									speedoldtimestep[VELOCITY_Y_COMPONENT][i3] = fglobal[iflow].potent[VELOCITY_Y_COMPONENT][i3]; // copy end time step
									speedoldtimestep[VELOCITY_Z_COMPONENT][i3] = fglobal[iflow].potent[VELOCITY_Z_COMPONENT][i3]; // copy end time step
								}
							

							// ���������� ������������ ����� ����� ������� ��:
							for (integer i2 = 0; i2 < fglobal[iflow].maxelm; i2++) {
									for (integer i3 = 0; i3 < 6; i3++) {
										mfoldtimestep[i2][i3] = fglobal[iflow].mf[i2][i3]; // copy end time step
									}
								}
							

							if (1) {
								printf("phisicaltime ==%f\n", phisicaltime);
								// ������� ���������� ���������� � ��������� tecplot360:
								//exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, 0, bextendedprint, 0);
								// 25.03.2019
								// ������� ���������� ���������� � ��������� tecplot360:
								if (!b_on_adaptive_local_refinement_mesh) {
									exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, 0, bextendedprint, 0,b,lb);
								}
								else {
									ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, fglobal, 0, b, lb);
								}
								//getchar();

							}

							// �� ��������� ���� �� ������� �� �������� ������, 
							// ������� � ���������� �������� �������.
							for (integer i = 0; i < flow_interior; i++) continity_start[i] = 1.0;
							for (integer i = 0; i < flow_interior; i++) inumber_iteration_SIMPLE[i] = 0; // ��������� �������� ��������� SIMPLE ��� ������ FLUID ����.

						}  // ����� ������ ���� �� �������.


						for (integer i = 0; i < 3; i++) {
							if (SpeedCorOldinternal[i] != nullptr) {
								delete[] SpeedCorOldinternal[i];
								SpeedCorOldinternal[i] = nullptr;
							}
						}
						delete[] SpeedCorOldinternal;
						SpeedCorOldinternal = nullptr;

						if (xb != nullptr) {
							delete[] xb; // �� �������� ����������� ������.
							xb = nullptr;
						}
						if (rthdsd != nullptr) {
							delete[] rthdsd;
							rthdsd = nullptr;
						}
						if (rthdsdt != nullptr) {
							delete[] rthdsdt;
							rthdsdt = nullptr;
						}



						// ������������ ����������� ������.
						for (integer i = 0; i < fglobal[iflow].maxelm; i++) {
							delete[]  mfold[i];
							delete[] mfoldtimestep[i];
						}
						
											
						delete[] mfold;
						delete[] mfoldtimestep;
						mfold = nullptr;
						mfoldtimestep = nullptr;

						
						for (integer i2 = 0; i2 < 3; i2++) {
							// i1 - ����� FLUID INTERIOR,
							// i2 - VX, VY, VZ - ���� �� ��� ��������� ��������,
							// i3 - ��������������� ����� ������������ ������ (����������
							delete speedoldtimestep[i2];
						}						
						
						if (speedoldtimestep != nullptr) {
							delete[] speedoldtimestep;
						}
						speedoldtimestep = nullptr;

						if (toldtimestep != nullptr) {
							delete[] toldtimestep;
						}
						toldtimestep = nullptr;

						fclose(fpcont); // �������� ����� ��� ������ �������.
						// ������� ���������� ������� � ��������� tecplot360
						//exporttecplotxy360_3D( f.maxelm, f.ncell, f.nvtx, f.nvtxcell, f.pa, f.potent, rhie_chow);
					}

					// ������� ���������� ���������� � ��������� tecplot360:
					// 25.03.2019
					// ������� ���������� ���������� � ��������� tecplot360:
					if (!b_on_adaptive_local_refinement_mesh) {
						exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, 0, bextendedprint, 0,b,lb);
					}
					else {
						ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, fglobal, 0, b, lb);
					}
					//getchar();
				}
			}
		}
	}

	delete[] color;
	delete[] color_solid;

} // unsteady_cfd_calculation




#endif