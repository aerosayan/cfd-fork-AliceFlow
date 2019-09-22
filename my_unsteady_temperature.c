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

// �������� ������ ����� ���������� � ��������� ���� 
// report_temperature.txt
// ������������ ����������� ������� �����,
// ������������ ����������� ������� ���������,
// ������������ ����������� ������ ������.
void report_temperature(integer flow_interior,
	FLOW* &fglobal, TEMPER &t,
	BLOCK* b, integer lb, SOURCE* s, integer ls,
	WALL* w, integer lw, integer ipref) {

	doublereal* tmaxreportblock = NULL;
		tmaxreportblock = new doublereal[lb];
		if (tmaxreportblock == NULL) {
			// ������������ ������ �� ������ ������������.
			printf("Problem : not enough memory on your equipment for tmaxreportblock report_temperature...\n");
			//printf("Please any key to exit...\n");
			system("pause");
			exit(1);
		}
	doublereal* tmaxreportsource = NULL;
		tmaxreportsource = new doublereal[ls];
		if (tmaxreportsource == NULL) {
			// ������������ ������ �� ������ ������������.
			printf("Problem : not enough memory on your equipment for tmaxreportsource report_temperature...\n");
			//printf("Please any key to exit...\n");
			system("pause");
			exit(1);
		}
	doublereal* tmaxreportwall = NULL;
		tmaxreportwall = new doublereal[lw];
		if (tmaxreportwall == NULL) {
			// ������������ ������ �� ������ ������������.
			printf("Problem : not enough memory on your equipment for tmaxreportwall report_temperature...\n");
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
	if (t.sosedb[inumber].MCB<(ls+lw)) {
	if (t.sosedb[inumber].MCB<ls) {
	if (tmaxreportsource[t.sosedb[inumber].MCB]<t.potent[i]) {
	tmaxreportsource[t.sosedb[inumber].MCB]=t.potent[i];
	}
	}
	else {
	if (tmaxreportwall[t.sosedb[inumber].MCB-ls]<t.potent[i]) {
	tmaxreportwall[t.sosedb[inumber].MCB-ls]=t.potent[i];
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
			if (t.sosedb[inumber].MCB<(ls + lw)) {
				if (t.sosedb[inumber].MCB<ls) {
					if (tmaxreportsource[t.sosedb[inumber].MCB]<t.potent[i]) {
						tmaxreportsource[t.sosedb[inumber].MCB] = t.potent[i];
					}
				}
				else {
					if (tmaxreportwall[t.sosedb[inumber].MCB - ls]<t.potent[i]) {
						tmaxreportwall[t.sosedb[inumber].MCB - ls] = t.potent[i];
					}
				}
			}
		}
	}

	// ������ ����� ��������� ��� ��������� ����� �������� �������, �������
	// ����� ������������� ������������ ����������������� �����������.
	// ��� ��� ������ ����� ������� �� ������ ����� ��������� ��������� ��,
	// ����� ����������� � �� ����������� ��� ������� ��������������.
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
		FILE *fp; // ���� � ������� ����� ������������ �������
		errno_t err;


		char name1[] = "report_temperature.txt";
		char name2[] = "solver/solid_static/report_temperature.txt";
		char name3[] = "solver/conjugate_heat_transfer_static/report_temperature.txt";

		char *name = NULL;

		switch (ipref) {
		case 0: name = name1; break;
		case 1: name = name2; break;
		case 2: name = name3; break;
		default:
			printf("error in my_unsteady_temperature.c : report_temperature : name==NULL\n");
			system("pause");
			exit(1);
			break;
		}

		/*
		char *name="report_temperature.txt";
		switch(ipref) {
		case 0 : name="report_temperature.txt"; break;
		case 1 : name="solver/solid_static/report_temperature.txt"; break;
		case 2 : name="solver/conjugate_heat_transfer_static/report_temperature.txt"; break;
		}
		*/

#ifdef MINGW_COMPILLER
		err = 0;
		fp=fopen64(name, "w");
		if (fp == NULL) err = 1;
#else
		err = fopen_s(&fp, name, "w");
#endif

		

		if ((err) != 0) {
			printf("Create File report_temperature.txt Error\n");
			// getchar();
			name = NULL;
			system("pause");
			exit(0);
		}
		else {

			if (fp != NULL) {

				name = NULL;

				fprintf(fp, "temperature, �C   power, W\n");
				for (integer i = 0; i < lb; i++) {
					doublereal Vol = fabs((b[i].g.xE - b[i].g.xS)*(b[i].g.yE - b[i].g.yS)*(b[i].g.zE - b[i].g.zS));
					//fprintf(fp, "%e %e\n", tmaxreportblock[i], b[i].Sc*(Vol));
					fprintf(fp, "%e %e\n", tmaxreportblock[i], get_power(b[i].n_Sc, b[i].temp_Sc, b[i].arr_Sc, tmaxreportblock[i])*(Vol));
				}
				for (integer i = 0; i < ls; i++) {
					fprintf(fp, "%e %e\n", tmaxreportsource[i], s[i].power);
				}
				for (integer i = 0; i < lw; i++) {
					fprintf(fp, "%e %e\n", tmaxreportwall[i], 0.0);
				}

				fclose(fp);
			}
		}

		if (tmaxreportblock != NULL) {
			delete[] tmaxreportblock;
			tmaxreportblock = NULL;
		}

		if (tmaxreportsource != NULL) {
			delete[] tmaxreportsource;
			tmaxreportsource = NULL;
		}

		if (tmaxreportwall != NULL) {
			delete[] tmaxreportwall;
			tmaxreportwall = NULL;
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
// �������� : ������������������ ��� ������ �� ������� ������� ����������� 
// ������ ������������ ������ ���������� AliceMesh* ������� ��� ����������� ������������ 
// ������ �������������� � ����������� ������ ����������.
// �������� ������ ����� ���������� � ��������� ���� 
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
	// ��������� ������ :
	// report_temperature_for_unsteady_modeling(flow_interior, f, t, b, lb, s, ls, w, lw, 0, time_solution_now, poweron_multiplier_sequence);
	// ���������� ������ �� ������� : unsteady_temperature_calculation.

	doublereal* tmaxreportblock = NULL;
	tmaxreportblock = new doublereal[lb];
	if (tmaxreportblock == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for tmaxreportblock report_temperature_for_unsteady_modeling...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	doublereal* tmaxreportsource = NULL;
	tmaxreportsource = new doublereal[ls];
	if (tmaxreportsource == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for tmaxreportsource report_temperature_for_unsteady_modeling...\n");
		printf("Please any key to exit...\n");
		//getchar();
		system("pause");
		exit(1);
	}
	doublereal* tmaxreportwall = NULL;
	tmaxreportwall = new doublereal[lw];
	if (tmaxreportwall == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for tmaxreportwall report_temperature_for_unsteady_modeling...\n");
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
	if (t.sosedb[inumber].MCB<(ls+lw)) {
	if (t.sosedb[inumber].MCB<ls) {
	if (tmaxreportsource[t.sosedb[inumber].MCB]<t.potent[i]) {
	tmaxreportsource[t.sosedb[inumber].MCB]=t.potent[i];
	}
	}
	else {
	if (tmaxreportwall[t.sosedb[inumber].MCB-ls]<t.potent[i]) {
	tmaxreportwall[t.sosedb[inumber].MCB-ls]=t.potent[i];
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
			if (t.sosedb[inumber].MCB<(ls + lw)) {
				if (t.sosedb[inumber].MCB<ls) {
					if (tmaxreportsource[t.sosedb[inumber].MCB]<t.potent[i]) {
						tmaxreportsource[t.sosedb[inumber].MCB] = t.potent[i];
					}
				}
				else {
					if (tmaxreportwall[t.sosedb[inumber].MCB - ls]<t.potent[i]) {
						tmaxreportwall[t.sosedb[inumber].MCB - ls] = t.potent[i];
					}
				}
			}
		}
	}

	// ������ ����� ��������� ��� ��������� ����� �������� �������, �������
	// ����� ������������� ������������ ����������������� �����������.
	// ��� ��� ������ ����� ������� �� ������ ����� ��������� ��������� ��,
	// ����� ����������� � �� ����������� ��� ������� ��������������.
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
		FILE *fp; // ���� � ������� ����� ������������ �������
		errno_t err;


		/*
		char name1[] = "report_temperature.txt";
		char name2[] = "solver/solid_static/report_temperature.txt";
		char name3[] = "solver/conjugate_heat_transfer_static/report_temperature.txt";

		char *name = NULL;

		switch (ipref) {
		case 0: name = name1; break;
		case 1: name = name2; break;
		case 2: name = name3; break;
		default:
		printf("error in my_unsteady_temperature.c : report_temperature : name==NULL\n");
		system("pause");
		exit(1);
		break;
		}
		*/


		/*
		char *name="report_temperature.txt";
		switch(ipref) {
		case 0 : name="report_temperature.txt"; break;
		case 1 : name="solver/solid_static/report_temperature.txt"; break;
		case 2 : name="solver/conjugate_heat_transfer_static/report_temperature.txt"; break;
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
			//name = NULL;
			// 3.09.2019 ������ ������. �� ������ �� ��������� ��� ������� ��� ��������� �������� �����.
			//system("pause");
			//exit(0);
		}
		else {

			if (fp != NULL) {

				//name = NULL;

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

				if (tmaxreportblock != NULL) {
					delete[] tmaxreportblock;
					tmaxreportblock = NULL;
				}

				if (tmaxreportsource != NULL) {
					delete[] tmaxreportsource;
					tmaxreportsource = NULL;
				}

				if (tmaxreportwall != NULL) {
					delete[] tmaxreportwall;
					tmaxreportwall = NULL;
				}

			}
		}



	}
	else {

		// ���� ������ ����� ��������� ��� ������� ��� ������� ��� ��������
		// ������� ������ �������� � �������� �������� � ���� ������.
		// ��� ������, ����� ��������� �� ����� ��������� �������� �� �������� ������� � ���������� ����� �������.
		// ���������� ������������� ���� ������ ����� �������� ���������.

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
// � �������� : Dt=Dt0+a*time;
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
// � �������� : Dt=Dt0+a*time;
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
// � �������� : Dt=Dt0+a*time;
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
		 // ��������� �������� phase_delay_time :
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

// ����������������� ��� SquareWave2 ����� 24.07.2016
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
		while (time < EndTime) {
				i++;
				j++;
				if (!bost) {
					integer kmod = (j - 1) % 40;
					if (kmod <= 9) {
						time += glTSL.tau1 / 10.0;
					}
					else if ((kmod >= 10) && (kmod <= 19)) {
						time += glTSL.tau2 / 10.0;
					}
					else if ((kmod >= 20) && (kmod <= 29)) {
						time += glTSL.tau1 / 10.0;
					}
					else if ((kmod >= 30) && (kmod <= 39)) {
						time += glTSL.tau_pause / 10.0;
					}
				}
					integer kmod2 = (i-1) % (glTSL.n_cycle * 40 + 30);
					if (kmod2>=glTSL.n_cycle * 40)
                     {
						 if (!bost) {
							 time -= glTSL.tau1 / 10.0;
						 }
							// ������� �� �����
							time += t_pause_gl / 30.0;
							bost = true;


					}
				
					//if (time > ig*glTSL.T_all) {
					if ((i != 1) && ((i - 1) % (glTSL.n_cycle * 40 + 30) == 0)) {
                		ig++;
						bost = false;
						j = 1;
						time += glTSL.tau1 / 10.0;
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
				integer kmod = (j - 1) % 40;
				if (kmod <= 9) {
					time += glTSL.tau1 / 10.0;
					timestep_sequence[i - 1] = glTSL.tau1 / 10.0;
					poweron_multiplier_sequence[i - 1] = glTSL.m1; // �������� �������� ��������.
				}
				else if ((kmod >= 10) && (kmod <= 19)) {
					time += glTSL.tau2 / 10.0;
					timestep_sequence[i - 1] = glTSL.tau2 / 10.0;
					poweron_multiplier_sequence[i - 1] = 1.0; // �������� �������� �� ������. 
				}
				else if ((kmod >= 20) && (kmod <= 29)) {
					time += glTSL.tau1 / 10.0;
					timestep_sequence[i - 1] = glTSL.tau1 / 10.0;
					poweron_multiplier_sequence[i - 1] = glTSL.m1; // �������� �������� ��������.
				}
				else if ((kmod >= 30) && (kmod <= 39)) {
					time += glTSL.tau_pause / 10.0;
					timestep_sequence[i - 1] = glTSL.tau_pause / 10.0;
					poweron_multiplier_sequence[i - 1] = 0.0; // �������� ���������.
				}
			}
			integer kmod2 = (i-1) % (glTSL.n_cycle * 40 + 30);
			if (kmod2 >= glTSL.n_cycle * 40)
			{

				if (!bost) {
					time -= glTSL.tau1 / 10.0;
				}
				// ������� �� �����
				time += t_pause_gl / 30.0;
				bost = true;
				timestep_sequence[i - 1] = t_pause_gl / 30.0;
				poweron_multiplier_sequence[i - 1] = 0.0; // �������� ���������.

			}
			

           // if (time > ig*glTSL.T_all) {
			if ((i != 1) && ((i - 1) % (glTSL.n_cycle * 40 + 30) == 0)) {
				//printf("incomming\n");
				 ig++;
			     bost = false;
				 j = 1;
				 time += glTSL.tau1 / 10.0;
				 timestep_sequence[i - 1] = glTSL.tau1 / 10.0;
				 poweron_multiplier_sequence[i - 1] = glTSL.m1; // �������� �������� ��������.
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

// �������������� ������������� ������
void unsteady_temperature_calculation(FLOW &f, FLOW* &fglobal, TEMPER &t, doublereal** &rhie_chow,
	                      BLOCK* b, integer lb, SOURCE* s, integer ls, WALL* w, integer lw, 
						  doublereal dbeta, integer flow_interior,  TPROP* matlist, 
						  doublereal operatingtemperature, TEMP_DEP_POWER* gtdps,
	                      integer ltdp, integer lu, UNION* &my_union, bool bsecond_T_solver)
{

	// ����� �������.
	unsigned int calculation_start_time = 0; // ������ ����� ��.
	unsigned int calculation_end_time = 0; // ��������� ����� ��.
	unsigned int calculation_seach_time = 0; // ����� ���������� ������� ���� � ��.

	calculation_start_time = clock(); // ������ ������ �����.

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
	doublereal* toldtimestep = NULL;
	doublereal* tnewtimestep = NULL;
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
	doublereal* timestep_sequence=NULL; // ������������������ ����� �� �������.
	// ���������� � ������ �������� �� ������ ��������� ����
	doublereal* poweron_multiplier_sequence=NULL; // (��������� ������� �������� ������� �� ����������).
    doublereal StartTime=0.0, EndTime=globalEndTimeUnsteadyTemperatureCalculation; // ������������ 
	doublereal TimeStepIncrement=1.0e-7; // ��������� ��� �� ������� 1���. (������������ � ���������� ���� �� �������.)
	doublereal Initial_Time_Step=1e-7; // �.�. ������� �� ����������.
	doublereal Factor_a=0.4; // ������ ���������� ���� �� �������
	Factor_a = glTSL.Factor_a_for_Linear;
	doublereal** evdokimova_report = NULL;
	if (glTSL.id_law == 0) {
		// ������� ����� �� ������� � ���������� � ���������� ��������.
		// ���������� ��� �� �������:
		//--->//uniform_timestep_seq(StartTime, EndTime, TimeStepIncrement, iN, timestep_sequence, poweron_multiplier_sequence);
		// ���������� �������� ��� �� ������� (� ������������ � �������������� �����������):
		linear_timestep_seq(StartTime, EndTime, Initial_Time_Step, Factor_a, iN, timestep_sequence, poweron_multiplier_sequence);
		// �� ������ ����������� ������������ ����� � ������� ���������.
		//linear_timestep_seq2(StartTime, EndTime, Initial_Time_Step, Factor_a, iN, timestep_sequence, poweron_multiplier_sequence);

		// ������ �� ������ : �������� ������ ����������������� ��������. ���������� �.�., ���� �.�., ����� �.�.
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

	FILE *fpcurvedata; // ���� � ������� ����� ������������ ���������� ��������������� �������������.
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
				fprintf(fpKras_max, "0 \n");
				fprintf(fpKras_max, "0 \n");
				fprintf(fpKras_min, "0 \n");
				fprintf(fpKras_min, "0 \n");
				fprintf(fpKras_max, "Evalution maximum temperature in default interior \n");
				fprintf(fpKras_max, "time[s] maximum_temperature[C] \n");
				fprintf(fpKras_min, "Evalution minimum temperature in default interior \n");
				fprintf(fpKras_min, "time[s] maximum_temperature[C] \n");
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
			my_memory_bicgstab.tval = NULL;
			my_memory_bicgstab.tcol_ind = NULL;
			my_memory_bicgstab.trow_ptr = NULL;
			my_memory_bicgstab.tri = NULL;
			my_memory_bicgstab.troc = NULL;
			my_memory_bicgstab.ts = NULL;
			my_memory_bicgstab.tt = NULL;
			my_memory_bicgstab.tvi = NULL;
			my_memory_bicgstab.tpi = NULL;
			my_memory_bicgstab.tdx = NULL;
			my_memory_bicgstab.tdax = NULL;
			my_memory_bicgstab.ty = NULL;
			my_memory_bicgstab.tz = NULL;
			my_memory_bicgstab.ta = NULL;
			my_memory_bicgstab.tja = NULL;
			my_memory_bicgstab.tia = NULL;
			my_memory_bicgstab.talu = NULL;
			my_memory_bicgstab.tjlu = NULL;
			my_memory_bicgstab.tju = NULL;
			my_memory_bicgstab.tiw = NULL;
			my_memory_bicgstab.tlevs = NULL;
			my_memory_bicgstab.tw = NULL;
			my_memory_bicgstab.tjw = NULL;
			my_memory_bicgstab.icount_vel = 100000; // ����� ������� �����.

			doublereal phisicaltime = StartTime;

			// ��������� ����� � ����������� ������� ������� �� ������� ������� ������ :
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
						w, lw, lu, b, lb,
						my_memory_bicgstab,
						false, operatingtemperature,
						// ��� ��������������� �������������� ������������� 10.11.2018
						btimedep, timestep_sequence[j],
						toldtimestep, tnewtimestep, maxelm_global_ret,
						poweron_multiplier_sequence[j]);
				}
				else {
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
						NULL, // �������� � ����������� ���������� ����.
						NULL, lu, my_union); // �������� ����� ����� ������� � ����������� ���������� ����.
				}
				
				if (bsecond_T_solver) {
					// ����� ������������� ������.
					for (integer i = 0; i < maxelm_global_ret; i++) {
						if (tnewtimestep[i] < -273.15) {
							tnewtimestep[i] = -273.15; // �������������� ���������� ����.
						}
					}
				}
				else {
					for (integer i = 0; i < t.maxelm + t.maxbound; i++) {
						if (t.potent[i] < -273.15) {
							t.potent[i] = -273.15; // �������������� ���������� ����.
						}
					}
				}

				if (!bsecond_T_solver) {
					if ((glTSL.id_law == 2) && (j == 1039)) {
						// 29_11_2017
						// ��������� ������ ����� 6 ��������� �� �������� �����.
						if (!b_on_adaptive_local_refinement_mesh) {
							bool bextendedprint_1 = false;
							exporttecplotxy360T_3D_part2_apparat_hot(t.maxelm, t.ncell, fglobal, t, flow_interior, 0, bextendedprint_1, 1, b);
						}
						else {
							// ������� � ����
							// ������� � ��������� ������� �����������.
							//� ���� �����.
							ANES_tecplot360_export_temperature(t.maxnod, t.pa, t.maxelm, t.nvtx, t.potent, t, fglobal, 1, b, lb);
						}
					}
				}
				

				if (!bsecond_T_solver) {
					for (integer i = 0; i < t.maxelm + t.maxbound; i++) toldtimestep[i] = t.potent[i]; // copy
				}
				else {
					for (integer i = 0; i < maxelm_global_ret; i++) toldtimestep[i] = tnewtimestep[i]; // copy
				}
				doublereal tmaxi = -1.0e10; // ������������ ����������� ��� ���������� ��.


				if (!bsecond_T_solver) {
					if (!b_on_adaptive_local_refinement_mesh) {
						if (bfirst_export && (phisicaltime > 287990)) {
							bfirst_export = false;
							// ��������� ������ ����� 6 ��������� �� �������� �����.
							bool bextendedprint_1 = false;
							exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, 0, bextendedprint_1, 1,b,lb);
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

					// ��������� ����� � ����������� ������� ������� �� ������� ������� ������ :
					report_temperature_for_unsteady_modeling(0, fglobal, t, b, lb, s, ls, w, lw, 0, phisicaltime, poweron_multiplier_sequence[j], operatingtemperature);
				}
				

				doublereal tmaxavg = -273.15;
				doublereal *nullpointer = NULL;
				if (!bsecond_T_solver) {
					if (!b_on_adaptive_local_refinement_mesh) {

						// ���������� �������� ����, � ����������� �� �� 9.01.2017.
						// ���������� �������� ������ �� ������� ������������� 
						// �����������������  �����.
						/*
						doublereal* tempfiltr = new doublereal[t.maxelm + t.maxbound];
						double_average_potent(t.potent, tempfiltr,
							t.maxelm, t.maxbound, t.sosedi,
							t.nvtx, t.pa, nullpointer,
							SIMPSON_FILTR, t.sosedb, 0); // VOLUME_AVERAGE_FILTR

						for (integer i = 0; i < t.maxelm; i++) tmaxavg = fmax(tmaxavg, tempfiltr[i]);
						if (!b_on_adaptive_local_refinement_mesh) {
							xyplot_temp(t, tempfiltr);
						}
						if (tempfiltr != NULL) {
							delete[] tempfiltr; // ������������ ������.
							tempfiltr = NULL;
						}
						*/
						for (integer i = 0; i < t.maxelm; i++) tmaxavg = fmax(tmaxavg, t.potent[i]);
					}
					else {
						for (integer i = 0; i < t.maxelm; i++) tmaxavg = fmax(tmaxavg, t.potent[i]);
					}
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
						doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
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
						doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
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
					if (evdokimova_report != NULL) {
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
				printf("complete is : %3.0f %% \n", (doublereal)(100.0*(j + 1) / iN)); // ���������� ������� ��������� ���������.
			}

			fclose(fpcurvedata); // �������� ����� ��� ������ ������ ��������.
		}
		fclose(fpKras); // �������� ����� ��� ������ ������ �������� � ������� ��� ������������ ����.
	}

	
	if (toldtimestep != NULL) {
		delete[] toldtimestep;
	}	
	if (tnewtimestep != NULL) {
		delete[] tnewtimestep;
	}
	if (timestep_sequence!=NULL) {
		delete[] timestep_sequence;
	}
	if (poweron_multiplier_sequence!=NULL) {
		delete[] poweron_multiplier_sequence;
	}

	if (glTSL.id_law == 0) {
        // Linear.
		// ������������ report ����������.
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

		// ������ ������� � ��������� ����:
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

		for (integer i = 0; i < iN + 1; i++) {
			delete[] evdokimova_report[i];
		}
		delete[] evdokimova_report;

	}

	// ��������� � ��� 23 ������ 2016 ����.
	calculation_end_time = clock();
	calculation_seach_time = calculation_end_time - calculation_start_time;
	unsigned int im = 0, is = 0, ims = 0;
	im = (unsigned int)(calculation_seach_time / 60000); // ������
	is = (unsigned int)((calculation_seach_time - 60000 * im) / 1000); // �������
	ims = (unsigned int)((calculation_seach_time - 60000 * im - 1000 * is) / 10); // ������������ ������� �� 10

	printf("time calculation is:  %d minute %d second %d 10*millisecond\n", im, is, ims);


} // unsteady_temperature_calculation



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
	integer lu, UNION* &my_union)
{


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
    doublereal **rhie_chow=NULL;

	///* 
    FILE *fpcont; // ���� � ������� ����� ������������ �������
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
					  // ���� ������ ����������������� ���������� ��������� ���������:
					  if (eqin.fluidinfo[iflow].iflow==1) {
						  // ������������� ����������������� ���������� � ������� iflow.

						  if (fglobal[iflow].bLR1free) {
							  // ��� ������ ����������������� ���������� �� ��� ��������� ����� ���������� ������� ������� ���
							  // �������� ��������. �� ���� ������� ������������ ������������.
						      printf("WARNING! bLR1free is true. All neiman condition for PAmendment.\n");
			                 // getchar();
						  }

						  bool btimedep=false;

				          // ��������� ����������� ������ ��� �������� Rhie-Chow
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
                          
						  if (eqin.itemper == 1) {
							  fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM     energy      \n");
						  }
						  else {
							  fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM     \n");
						  }

						  fclose(fp_statistic_convergence);


		                  doublereal continity=1.0; // �������������
					   
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
						  calculation_start_time=clock(); // ������ ������ �����.
					      bool bfirst=true;
						  doublereal* smagconstolditer=NULL;
						  if (fglobal[iflow].smaginfo.bDynamic_Stress) {
							  smagconstolditer=new doublereal[fglobal[iflow].maxelm];
							  if (smagconstolditer==NULL) {
	                              // ������������ ������ �� ������ ������������.
		                          printf("Problem : not enough memory on your equipment for smagconstolditer steady cfd calculation...\n");
		                          printf("Please any key to exit...\n");
		                          exit(1);
	                          }
						  }

						  // ���������� ���������������� �������� � ���������� ��������.
						  doublereal **SpeedCorOld=NULL;
	                      SpeedCorOld=new doublereal*[3];
						  if (SpeedCorOld==NULL) {
	                          // ������������ ������ �� ������ ������������.
		                      printf("Problem : not enough memory on your equipment for SpeedCorOld steady cfd calculation...\n");
		                      printf("Please any key to exit...\n");
		                      exit(1);
	                      }
 	                      for (integer i=0; i<3; i++) {
							  SpeedCorOld[i]=NULL;
		                      SpeedCorOld[i]=new doublereal[fglobal[iflow].maxelm+fglobal[iflow].maxbound];
							  if (SpeedCorOld[i]==NULL) {
	                               // ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
								  printf("Problem : not enough memory on your equipment for SpeedCorOld[%lld] steady cfd calculation...\n", i);
#else
								  printf("Problem : not enough memory on your equipment for SpeedCorOld[%d] steady cfd calculation...\n", i);
#endif
		                           printf("Please any key to exit...\n");
		                           exit(1);
	                          }
	                      }
	                      for (integer i=0; i<3; i++) {
		                      for (integer j=0; j<fglobal[iflow].maxelm+fglobal[iflow].maxbound; j++) {
			                      switch (i) {
			                         case VX : SpeedCorOld[VX][j]=fglobal[iflow].potent[VXCOR][j];
				                               break;
			                         case VY : SpeedCorOld[VY][j]=fglobal[iflow].potent[VYCOR][j];
				                               break;
			                         case VZ : SpeedCorOld[VZ][j]=fglobal[iflow].potent[VZCOR][j];
				                               break;
			                      }
		                      }
	                      }

						  doublereal **mfold=new doublereal*[fglobal[iflow].maxelm];
						  for (integer i=0; i<fglobal[iflow].maxelm; i++) {
							  mfold[i]=new doublereal[6];
						  }
						  
						  for (integer i = 0; i < fglobal[iflow].maxelm; i++) {
							  for (integer j = 0; j < 6; j++) {
								  mfold[i][j] = fglobal[iflow].mf[i][j]; // ��������� �����.
							  }
						  }

                          for (integer iP=0; iP<fglobal[iflow].maxelm; iP++) {

			                   // ��������� ����������������� �������� ����� ����� ����� ��.
			                   // �������� ����� ����������� �� ������� �������� �� � ������
			                   // ������ ��� ���������������� �������� ���-���. ��� ��� ���������� ������������
			                   // ������� �������� ������������ �������� �� ����� ��.

							   bool bsimplelinearinterpol=true; // ����������� ������� �������� ������������ �������� �� �����.
			
							   // 25.03.2019 ������ �������� �� ���� �����.
                               return_calc_correct_mass_flux(iP, 
								                        	 fglobal[iflow].potent,
									                         fglobal[iflow].pa,
									                         fglobal[iflow].prop,
									                         fglobal[iflow].prop_b,
					                                         fglobal[iflow].nvtx,
									                         fglobal[iflow].sosedi,
									                         fglobal[iflow].maxelm,
									                         fglobal[iflow].diag_coef,
									                         fglobal[iflow].alpha,
						                                     RCh,
									                         false,
						                                     0.01,
									                         NULL,
						                                     fglobal[iflow].mf[iP], // ������������ �������� ��������� ������
									                         NULL,bsimplelinearinterpol,
															 SpeedCorOld, mfold[iP],
								                             fglobal[iflow].sosedb,
								                             t.ilevel_alice,
								                             fglobal[iflow].ptr);

							   if (fglobal[iflow].smaginfo.bDynamic_Stress) {
							       smagconstolditer[iP]=0.0; // ��������� ��������
						       }
			               }

						  // �������������.
						  for (integer i = 0; i < fglobal[iflow].maxelm; i++) {
							  for (integer j = 0; j < 6; j++) {
								  mfold[i][j] = fglobal[iflow].mf[i][j]; // ��������� �����.
							  }
						  }
						       	

						  // ������������ ����������� ������ �� ����.
						  if (SpeedCorOld != NULL) {
							  for (integer i = 0; i < 3; i++) {
								  if (SpeedCorOld[i] != NULL) {
									  delete[] SpeedCorOld[i];
									  SpeedCorOld[i] = NULL;
								  }
							  }
							  delete[] SpeedCorOld;
						  }
                          SpeedCorOld=NULL;
						  
                          QuickMemVorst my_memory_bicgstab;
						  my_memory_bicgstab.ballocCRScfd=false; // �������� ������.
						  my_memory_bicgstab.bsignalfreeCRScfd=false; // �� ���������� ������. (��� ����).
						  // ������������� ���������� !
						  my_memory_bicgstab.val=NULL;
			              my_memory_bicgstab.col_ind=NULL;
			              my_memory_bicgstab.row_ptr=NULL;
			              my_memory_bicgstab.ri=NULL;
			              my_memory_bicgstab.roc=NULL;
			              my_memory_bicgstab.s=NULL;
			              my_memory_bicgstab.t=NULL;
			              my_memory_bicgstab.vi=NULL;
			              my_memory_bicgstab.pi=NULL;
			              my_memory_bicgstab.dx=NULL;
			              my_memory_bicgstab.dax=NULL;
			              my_memory_bicgstab.y=NULL;
			              my_memory_bicgstab.z=NULL;
			              my_memory_bicgstab.a=NULL;
			              my_memory_bicgstab.ja=NULL;
			              my_memory_bicgstab.ia=NULL;
			              my_memory_bicgstab.alu=NULL;
			              my_memory_bicgstab.jlu=NULL;
			              my_memory_bicgstab.ju=NULL;
						   my_memory_bicgstab.alu1=NULL;
			              my_memory_bicgstab.jlu1=NULL;
			              my_memory_bicgstab.ju1=NULL;
						  my_memory_bicgstab.x1=NULL;
			              my_memory_bicgstab.iw=NULL;
			              my_memory_bicgstab.levs=NULL;
			              my_memory_bicgstab.w=NULL;
			              my_memory_bicgstab.jw=NULL;
                          my_memory_bicgstab.w_dubl=NULL;
			              my_memory_bicgstab.jw_dubl=NULL;
						  // ������ ��������� � ����������� ������������� �������� � ��������� �������������.
			              my_memory_bicgstab.ballocCRSt=false; // �������� ������
			              my_memory_bicgstab.bsignalfreeCRSt=false; // � ����� �� �����������.
			              // ������������� ����������.
                          my_memory_bicgstab.tval=NULL;
			              my_memory_bicgstab.tcol_ind=NULL;
			              my_memory_bicgstab.trow_ptr=NULL;
			              my_memory_bicgstab.tri=NULL;
			              my_memory_bicgstab.troc=NULL;
			              my_memory_bicgstab.ts=NULL;
			              my_memory_bicgstab.tt=NULL;
			              my_memory_bicgstab.tvi=NULL;
			              my_memory_bicgstab.tpi=NULL;
			              my_memory_bicgstab.tdx=NULL;
			              my_memory_bicgstab.tdax=NULL;
			              my_memory_bicgstab.ty=NULL;
			              my_memory_bicgstab.tz=NULL;
			              my_memory_bicgstab.ta=NULL;
			              my_memory_bicgstab.tja=NULL;
			              my_memory_bicgstab.tia=NULL;
			              my_memory_bicgstab.talu=NULL;
			              my_memory_bicgstab.tjlu=NULL;
			              my_memory_bicgstab.tju=NULL;
			              my_memory_bicgstab.tiw=NULL;
			              my_memory_bicgstab.tlevs=NULL;
			              my_memory_bicgstab.tw=NULL;
			              my_memory_bicgstab.tjw=NULL;
						  my_memory_bicgstab.icount_vel=100000; // ����� ������� �����.

						  // ���������� ���������������� �������� � ���������� ��������.
	                      doublereal **SpeedCorOldinternal=new doublereal*[3];
	                      for (integer i=0; i<3; i++) {
	                         	SpeedCorOldinternal[i]=new doublereal[fglobal[iflow].maxelm+fglobal[iflow].maxbound];
	                      }

                          doublereal* xb=new doublereal[fglobal[iflow].maxelm+fglobal[iflow].maxbound];
                          doublereal* rthdsd=NULL; // ������ ����� ������� ���������.
                          doublereal* rthdsdt=NULL;
                          rthdsd=new doublereal[fglobal[iflow].maxelm+fglobal[iflow].maxbound]; 
						  rthdsdt=new doublereal[t.maxelm+t.maxbound];


						  integer iend=1000; // 300 ����� ��������.
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
								  // ������� ������� � ������� ������������� �� �������� �������� �������� � ��� 700 ��������.
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
						  integer iseparate_SIMPLE=10000;
						  bool bseparate_SIMPLE = true;// ������ ������ ���� ���.

						  doublereal start_average_continity = 0.0;
						  
						  if (number_iteration_SIMPLE_algorithm > 0) {
							  // 22.09.2019
							  // ���������� �������� SIMPLE ��������� �������� 
							  // ������������� ����� ����������� ���������.
							  iend = number_iteration_SIMPLE_algorithm;
						  }

	                      for (integer i=inumber_iteration_SIMPLE[iflow]+1; i<iend; i++) {
							  if (i==iend-1) {
								  my_memory_bicgstab.bsignalfreeCRScfd=true;
								  my_memory_bicgstab.bsignalfreeCRSt=true; // ����������� �� ��������� ��������.
							  }

							  if ((bSIMPLErun_now_for_temperature)&&((fabs(dgx) > 1.0e-20) || (fabs(dgy) > 1.0e-20) || (fabs(dgz) > 1.0e-20))) {
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
									  // ������� ������� � ������� ������������� �� �������� �������� �������� � ��� 700 ��������.
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

							   if (0&&(i>=67)) { // debug
								   bprintmessage=true;
								   // 25.03.2019
								   // ������� ���������� ���������� � ��������� tecplot360:
								   if (!b_on_adaptive_local_refinement_mesh) {
									   exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, i, bextendedprint, 0,b,lb);
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
											   exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, i, bextendedprint, 0,b,lb);
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


					           if (i==(inumber_iteration_SIMPLE[iflow]+1)) {
							       // ��������� ������ ���������� ������ ������
							       // ���� ����������������, ��������, �. ����������. 0.5; 0.8;
								   // � ����� Ferczinger and Peric �������������� ���������� ���������� ���������� ������ : 0.7; 0.3; 
								   // ��� �������� 0.7, � ��� �������� 0.3. ��� ���� ���������� ����� ������ ��� 0.7+0.3 == 1.0;
								   if (!b_on_adaptive_local_refinement_mesh) {
									   fglobal[iflow].alpha[VX] = 0.7; // 0.8 0.5
									   fglobal[iflow].alpha[VY] = 0.7; // 0.8 0.5
									   fglobal[iflow].alpha[VZ] = 0.7; // 0.8 0.5
									   fglobal[iflow].alpha[PRESS] = 0.3; // 0.2 0.8
								   }
								   else {
									   fglobal[iflow].alpha[VX] = 0.8; // 0.8 0.5
									   fglobal[iflow].alpha[VY] = 0.8; // 0.8 0.5
									   fglobal[iflow].alpha[VZ] = 0.8; // 0.8 0.5
									   fglobal[iflow].alpha[PRESS] = 0.2;// 0.05; // 0.2 0.8
								   }
					           }
					           else {
						           // ����� �� ������������ ��������� ���������� ������������ 
						           // � ����� �. ���������.
								   // � ����� Ferczinger and Peric �������������� ���������� ���������� ���������� ������ : 0.7; 0.3; 
								   // ��� �������� 0.7, � ��� �������� 0.3. ��� ���� ���������� ����� ������ ��� 0.7+0.3 == 1.0;
								   if (!b_on_adaptive_local_refinement_mesh) {
									   fglobal[iflow].alpha[VX] = 0.7; // 0.8 0.5
									   fglobal[iflow].alpha[VY] = 0.7; // 0.8 0.5
									   fglobal[iflow].alpha[VZ] = 0.7; // 0.8 0.5
									   fglobal[iflow].alpha[PRESS] = 0.3; // 0.2 0.8
								   }
								   else {
									   fglobal[iflow].alpha[VX] = 0.8; // 0.8 0.5
									   fglobal[iflow].alpha[VY] = 0.8; // 0.8 0.5
									   fglobal[iflow].alpha[VZ] = 0.8; // 0.8 0.5
									   fglobal[iflow].alpha[PRESS] = 0.2;// 0.05; // 0.2 0.8
								   }
					           }
							  
						       bool bfirst_start=false;
						       if ((i==(inumber_iteration_SIMPLE[iflow]+1))&&bfirst) {
							      bfirst_start=true;
							      bfirst=false; // ������ ������ ���� ������ ���� ���.
						       }


							   // ����� �������.
	                           unsigned int calculation_simple_start_time; // ������ ����� ��.
	                           unsigned int calculation_simple_end_time; // ��������� ����� ��.
	                           unsigned int calculation_simple_seach_time; // ����� ���������� ������� ���� � ��.

							   calculation_simple_start_time=clock(); // ������ ������ �����.

							   

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
															 NULL, NULL, NULL,
															 bprintmessage,
															 gtdps, ltdp,
															 rfluentres, rfluentrestemp, 
															 smagconstolditer,
															 mfold, eqin.itemper, 
															 my_memory_bicgstab,
															 bextendedprint,
															 SpeedCorOldinternal,xb,
															 rthdsd,rthdsdt, lu, my_union);


							   calculation_simple_end_time=clock();
				               calculation_simple_seach_time=calculation_simple_end_time-calculation_simple_start_time;
							   int im=0, is=0, ims=0;
							   im=(int)(calculation_simple_seach_time/60000); // ������
							   is=(int)((calculation_simple_seach_time-60000*im)/1000); // �������
							   ims=(int)((calculation_simple_seach_time-60000*im-1000*is)/10); // ������������ ������� �� 10
							   
							   const integer ianimstart=1800;
							   if (1&&(i>ianimstart)) {
								   char* buffer=new char[10];
							       buffer[0]='\0';
							       KRitoa(i-ianimstart+1,buffer);
							       //printf("%s\n",buffer);
							       char* mymessage=new char[30];
							       mymessage[0]='\0';
							       KRstrcat(mymessage,"iter=");
							       //printf("%s\n",mymessage);
							       KRstrcat(mymessage,buffer);
							       //printf("%s\n",mymessage);
							       //getchar();
								   bool btitle=false;
								   if (i==ianimstart+1) btitle=true;
							       animationtecplot360T_3D_part2all(t.maxelm,t.ncell, fglobal, t, flow_interior, mymessage, btitle);  
							       delete[] buffer; delete[] mymessage;
							   }

							   

#if doubleintprecision == 1
							   //if (i==5) continity_start[iflow]=continity;
							   if (i <= 5) {


								   fprintf(fpcont, "%lld 1.0\n", i + 1);
								   if (!bprintmessage) {
									   if (eqin.itemper == 0) {
										   // ������� ������ ������������� ��� ��������� ����������������.
										   //printf("%lld 1.0\n",i+1);
										   printf(" %lld %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
											   i, rfluentres.res_no_balance, rfluentres.res_vx,
											   rfluentres.res_vy, rfluentres.res_vz, im, is, ims, iend - i);
										   if (i % 10 == 0) {
											   printf("  iter continity x-velocity y-velocity z-velocity \t time/iter\n");
										   }
#ifdef MINGW_COMPILLER
										   err_stat = 0;
										   fp_statistic_convergence=fopen64("statistic_convergence.txt", "a");
										   if (fp_statistic_convergence == NULL) err_stat = 1;
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
									   else if (eqin.itemper == 1) {
										   doublereal tmax = 0.0;
										   // ���������� �������� ������������ ����������� ������ ��������� ������� � �� � ��������:
										   for (integer i1 = 0; i1<t.maxelm + t.maxbound; i1++) tmax = fmax(tmax, fabs(t.potent[i1]));
										   // ������� ������������� ��������� � ���������� ����������������.
										   printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
											   i, rfluentres.res_no_balance, rfluentres.res_vx,
											   rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, tmax, im, is, ims, iend - i);
										   if (i % 10 == 0) {
											   printf("  iter continity x-velocity y-velocity z-velocity temperature Tmax\t time/iter\n");
										   }
#ifdef MINGW_COMPILLER
										   err_stat = 0;
										   fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
										   if (fp_statistic_convergence == NULL) err_stat = 1;
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
								   continity_start[iflow] = continity;
								   rfluentres.operating_value_b = rfluentres.res_no_balance;
							   }
							   else {
								   fprintf(fpcont, "%lld %e\n", i + 1, continity / continity_start[iflow]); // ���������� � ����������
								   if (!bprintmessage) {
									   if (eqin.itemper == 0) {
										   // ������� ������ ������������� ��� ��������� ����������������.
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
										   if (fp_statistic_convergence == NULL) err_stat = 1;
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
									   else if (eqin.itemper == 1) {
										   doublereal tmax = 0.0;
										   // ���������� �������� ������������ ����������� ������ ��������� ������� � �� � ��������:
										   for (integer i1 = 0; i1<t.maxelm + t.maxbound; i1++) tmax = fmax(tmax, fabs(t.potent[i1]));
										   // ������� ������������� ��������� � ���������� ����������������.
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
										   if (fp_statistic_convergence == NULL) err_stat = 1;
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
#else
							   //if (i==5) continity_start[iflow]=continity;
							   if (i <= 5) {


								   fprintf(fpcont, "%d 1.0\n", i + 1);
								   if (!bprintmessage) {
									   if (eqin.itemper == 0) {
										   // ������� ������ ������������� ��� ��������� ����������������.
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
										   if (fp_statistic_convergence == NULL) err_stat = 1;
#else
										   err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a")
#endif
										   if ((err_stat) == 0) {
											   // 29 ������� 2015.
											   fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
												   rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance);
											   fclose(fp_statistic_convergence);
										   }
									   }
									   else if (eqin.itemper == 1) {
										   doublereal tmax = 0.0;
										   // ���������� �������� ������������ ����������� ������ ��������� ������� � �� � ��������:
										   for (integer i1 = 0; i1<t.maxelm + t.maxbound; i1++) tmax = fmax(tmax, fabs(t.potent[i1]));
										   // ������� ������������� ��������� � ���������� ����������������.
										   printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %d\n",
											   i, rfluentres.res_no_balance, rfluentres.res_vx,
											   rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, tmax, im, is, ims, iend - i);
										   if (i % 10 == 0) {
											   printf("  iter continity x-velocity y-velocity z-velocity temperature Tmax\t time/iter\n");
										   }
#ifdef MINGW_COMPILLER
										   err_stat = 0;
										   fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
										   if (fp_statistic_convergence == NULL) err_stat = 1;
#else
										   err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a")
#endif
										   if ((err_stat) == 0) {
											   // 29 ������� 2015.
											   fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
												   rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance, rfluentrestemp);
											   fclose(fp_statistic_convergence);
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
										   if (fp_statistic_convergence == NULL) err_stat = 1;
#else
										   err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a")
#endif
										   if ((err_stat) == 0) {
											   // 29 ������� 2015.
											   fprintf(fp_statistic_convergence, " %d %1.4e %1.4e %1.4e %1.4e\n", i, rfluentres.res_vx,
												   rfluentres.res_vy, rfluentres.res_vz, rfluentres.res_no_balance);
											   fclose(fp_statistic_convergence);
										   }
									   }
									   else if (eqin.itemper == 1) {
										   doublereal tmax = 0.0;
										   // ���������� �������� ������������ ����������� ������ ��������� ������� � �� � ��������:
										   for (integer i1 = 0; i1<t.maxelm + t.maxbound; i1++) tmax = fmax(tmax, fabs(t.potent[i1]));
										   // ������� ������������� ��������� � ���������� ����������������.
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
										   if (fp_statistic_convergence == NULL) err_stat = 1;
#else
										   err_stat = fopen_s(&fp_statistic_convergence, "statistic_convergence.txt", "a")
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
                               rhie_chow[i]=NULL;
						   }
				           delete[] rhie_chow;
						   rhie_chow=NULL;

						   // ������������ ����������� ������ �� ��� ��������� ������ �� �������.
						    for (integer i=0; i<fglobal[iflow].maxelm; i++) {
							  delete[]  mfold[i];
							  mfold[i]=NULL;
							}
							delete[] mfold;
							mfold=NULL;

							if (fglobal[iflow].smaginfo.bDynamic_Stress) {
							     delete[] smagconstolditer;
							     smagconstolditer=NULL;
							}
					  }
				  }

				  calculation_end_time=clock();
				  calculation_seach_time=calculation_end_time-calculation_start_time;

				 unsigned int im=0, is=0, ims=0;
				 im=(unsigned int)(calculation_seach_time/60000); // ������
				 is=(unsigned int)((calculation_seach_time-60000*im)/1000); // �������
				 ims=(unsigned int)((calculation_seach_time-60000*im-1000*is)/10); // ������������ ������� �� 10

				  printf("time calculation is:  %d minute %d second %d 10*millisecond\n",im,is,ims);
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

		

		// �������� ��� ��� ����������� ������ ����������� :
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
            my_memory_bicgstab.tval=NULL;
			my_memory_bicgstab.tcol_ind=NULL;
			my_memory_bicgstab.trow_ptr=NULL;
			my_memory_bicgstab.tri=NULL;
			my_memory_bicgstab.troc=NULL;
			my_memory_bicgstab.ts=NULL;
			my_memory_bicgstab.tt=NULL;
			my_memory_bicgstab.tvi=NULL;
			my_memory_bicgstab.tpi=NULL;
			my_memory_bicgstab.tdx=NULL;
			my_memory_bicgstab.tdax=NULL;
			my_memory_bicgstab.ty=NULL;
			my_memory_bicgstab.tz=NULL;
			my_memory_bicgstab.ta=NULL;
			my_memory_bicgstab.tja=NULL;
			my_memory_bicgstab.tia=NULL;
			my_memory_bicgstab.talu=NULL;
			my_memory_bicgstab.tjlu=NULL;
			my_memory_bicgstab.tju=NULL;
			my_memory_bicgstab.tiw=NULL;
			my_memory_bicgstab.tlevs=NULL;
			my_memory_bicgstab.tw=NULL;
			my_memory_bicgstab.tjw=NULL;
			my_memory_bicgstab.icount_vel=100000; // ����� ������� �����.
            doublereal* rthdsdt=NULL;
			rthdsdt=new doublereal[t.maxelm+t.maxbound];


			// ���������� �������� �������������� �� ���� ���������� �����.
			for (integer i47 = 0; i47 < t.maxelm; i47++) {
				// �������� � ��� ��� �������� �� ����������� ��� ������ � ������ ��������.
				integer ib = t.whot_is_block[i47];
				t.Sc[i47] = get_power(b[ib].n_Sc, b[ib].temp_Sc, b[ib].arr_Sc, t.potent[i47]);
			}

			doublereal** rsumanbstuff=NULL; // NULL ������� ��� ����� ������������ �������������
			doublereal rfluent_res_temp = 0.0;
		    solve(TEMP, res, fglobal[0], fglobal,
				  t, rhie_chow, s, w, b, ls, lw, lb, 
				  BETA_PRECISION, flow_interior, bconvective,
				  bfirst_start, NULL, NULL, NULL, NULL, tauparam,
				  btimedep, dgx, dgy, dgz, matlist,
				  inumiter, consolemessage, RCh,bVeryStable,
				  NULL,rsumanbstuff,bhighorder,bdeltapfinish, 1.0, 
				  my_memory_bicgstab, rthdsdt, rfluent_res_temp, lu, my_union);
			// ��������� �������� ������ ������� �������� ��� �������� ������� !
			doublereal tmax = 0.0;
			for (integer i1 = 0; i1<t.maxelm + t.maxbound; i1++) tmax = fmax(tmax, fabs(t.potent[i1]));
			printf("maximum temperature in default interior is %1.4e\n",tmax);

            delete[] rthdsdt;

		    for (integer i=0; i<3; i++) {
				delete[] rhie_chow[i];
				rhie_chow[i]=NULL;
			}
		    delete[] rhie_chow;
			rhie_chow=NULL;

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

	//*/
	//getchar();

	if (0) {
		// �������������� � ������ �������� 1908�.:
		boundarylayer_info(fglobal, t, flow_interior, w, lw);
	}

} // steady_cfd_calculation


// �������������� cfd �������� :
void usteady_cfd_calculation(bool breadOk, EQUATIONINFO &eqin, 
	                        doublereal dgx, doublereal dgy, doublereal dgz,
							doublereal* continity_start, 
							integer* inumber_iteration_SIMPLE,
							integer flow_interior, 
							FLOW* &fglobal, TEMPER &t,
							BLOCK* b, integer lb, SOURCE* s, integer ls,
							WALL* w, integer lw, TPROP* matlist,
							TEMP_DEP_POWER* gtdps, integer ltdp, bool bextendedprint,
	integer lu, UNION* &my_union)
{


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
    doublereal **rhie_chow=NULL;

	///* 
    FILE *fpcont; // ���� � ������� ����� ������������ �������
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

						// ������ ���� ������ ��� :
						// � ����������� ���������� ���� ��������� ������� :
						// a. ���� ����������; b. ���� ���������; c. ���������������� �������� ���-��� (���� �������� ��� ������� ��, ��� ��������� ��).

						// ����������� :
						doublereal* toldtimestep = new doublereal[t.maxelm + t.maxbound]; // ���� ���������� �� ���������� ��������� ����
						for (integer i1 = 0; i1 < t.maxelm + t.maxbound; i1++) {
							toldtimestep[i1] = t.potent[i1]; // copy �������������
						}

						// ���� �������� :
						// ��������� ������ :
						doublereal** speedoldtimestep = new doublereal*[3];							
						for (integer i2 = 0; i2 < 3; i2++) {
							speedoldtimestep[i2] = new doublereal[fglobal[iflow].maxelm + fglobal[iflow].maxbound];
						}
						
						// ������������� :
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
																	 fglobal[iflow].sosedi,
																	 fglobal[iflow].maxelm,
																	 fglobal[iflow].diag_coef,
																	 fglobal[iflow].alpha,
																	 RCh,
																	 false,
																	 0.01,
																	 NULL,
																	 fglobal[iflow].mf[iP], // ������������ �������� ��������� ������
																	 NULL,bsimplelinearinterpol,
																	 SpeedCorOld, mfold[iP]);

									   if (fglobal[iflow].smaginfo.bDynamic_Stress) {
										   smagconstolditer[iP]=0.0; // ��������� ��������
									   }
								   }
								   */
								   // �������� ����� ����� ����� �� � ���������� ��������.
								   // ��� ���������� �� ����� avtosave.txt ��� �������� ����� ������ �����������.
								   // ���� ����� ����������� ���������� ���������� � �������� �������� (���� �������� ��������� ����������).
								   // ��������� ������ :
						doublereal** mfoldtimestep = new doublereal*[fglobal[iflow].maxelm];							
						for (integer i2 = 0; i2 < fglobal[iflow].maxelm; i2++) {
							mfoldtimestep[i2] = new doublereal[6];
						}
						
						// ������������� :
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

						if (eqin.itemper == 1) {
							fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM     energy      \n");
						}
						else {
							fprintf(fp_statistic_convergence, "iter    VX      VY       VZ      PAM     \n");
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

						doublereal* smagconstolditer = NULL;
						if (fglobal[iflow].smaginfo.bDynamic_Stress) {
							smagconstolditer = new doublereal[fglobal[iflow].maxelm];
							if (smagconstolditer == NULL) {
								// ������������ ������ �� ������ ������������.
								printf("Problem : not enough memory on your equipment for smagconstolditer steady cfd calculation...\n");
								printf("Please any key to exit...\n");
								exit(1);
							}
						}

						// ���������� ���������������� �������� � ���������� ��������.
						doublereal **SpeedCorOld = NULL;
						SpeedCorOld = new doublereal*[3];
						if (SpeedCorOld == NULL) {
							// ������������ ������ �� ������ ������������.
							printf("Problem : not enough memory on your equipment for SpeedCorOld steady cfd calculation...\n");
							printf("Please any key to exit...\n");
							exit(1);
						}
						for (integer i = 0; i<3; i++) {
							SpeedCorOld[i] = NULL;
							SpeedCorOld[i] = new doublereal[fglobal[iflow].maxelm + fglobal[iflow].maxbound];
							if (SpeedCorOld[i] == NULL) {
								// ������������ ������ �� ������ ������������.
#if doubleintprecision == 1
								printf("Problem : not enough memory on your equipment for SpeedCorOld[%lld] steady cfd calculation...\n", i);
#else
								printf("Problem : not enough memory on your equipment for SpeedCorOld[%d] steady cfd calculation...\n", i);
#endif
								printf("Please any key to exit...\n");
								exit(1);
							}
						}
						for (integer i = 0; i<3; i++) {
							for (integer j = 0; j<fglobal[iflow].maxelm + fglobal[iflow].maxbound; j++) {
								switch (i) {
								case VX: SpeedCorOld[VX][j] = fglobal[iflow].potent[VXCOR][j];
									break;
								case VY: SpeedCorOld[VY][j] = fglobal[iflow].potent[VYCOR][j];
									break;
								case VZ: SpeedCorOld[VZ][j] = fglobal[iflow].potent[VZCOR][j];
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
								fglobal[iflow].sosedi,
								fglobal[iflow].maxelm,
								fglobal[iflow].diag_coef,
								fglobal[iflow].alpha,
								RCh,
								false,
								0.01,
								NULL,
								fglobal[iflow].mf[iP], // ������������ �������� ��������� ������
								NULL, bsimplelinearinterpol,
								SpeedCorOld, mfold[iP],
								fglobal[iflow].sosedb,
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
						if (SpeedCorOld != NULL) {
							for (integer i = 0; i < 3; i++) {
								if (SpeedCorOld[i] != NULL) {
									delete[] SpeedCorOld[i];
									SpeedCorOld[i] = NULL;
								}
							}
							delete[] SpeedCorOld;
						}
						SpeedCorOld = NULL;


						// ������� ����� �� ������� � ���������� � ���������� ��������.
						integer iN = 0; // ���������� ����� �� �������
						doublereal* timestep_sequence = NULL; // ������������������ ����� �� �������.
						// ���������� � ������ �������� �� ������ ��������� ����
						doublereal* poweron_multiplier_sequence = NULL; // (��������� ������� �������� ������� �� ����������).
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
						// �������������� ������:
						for (integer j = 0; j < iN; j++) {

							rfluentres.operating_value_b = 1.0; // ������������� ��������� ��������.

							// ��������� ������� ������������� �� �������, ���� ������ ������� ��� ��������
							phisicaltime += timestep_sequence[j];

							
									// ��������� ����������� ������ ��� �������� Rhie-Chow
									my_malloc2(rhie_chow, fglobal[iflow].maxelm);



									doublereal continity = 1.0; // �������������

									
									printf("phisical time = %e\n", phisicaltime);

									// ������������ �������� �� ������ ���� �� ������� :
									bool bfirst = true;
									integer iend = 40; // ����� ��������.
									QuickMemVorst my_memory_bicgstab;
									my_memory_bicgstab.ballocCRScfd = false; // �������� ������.
									my_memory_bicgstab.bsignalfreeCRScfd = false; // �� ���������� ������ ��� ����.
									// ������������� ���������� !
									my_memory_bicgstab.val = NULL;
									my_memory_bicgstab.col_ind = NULL;
									my_memory_bicgstab.row_ptr = NULL;
									my_memory_bicgstab.ri = NULL;
									my_memory_bicgstab.roc = NULL;
									my_memory_bicgstab.s = NULL;
									my_memory_bicgstab.t = NULL;
									my_memory_bicgstab.vi = NULL;
									my_memory_bicgstab.pi = NULL;
									my_memory_bicgstab.dx = NULL;
									my_memory_bicgstab.dax = NULL;
									my_memory_bicgstab.y = NULL;
									my_memory_bicgstab.z = NULL;
									my_memory_bicgstab.a = NULL;
									my_memory_bicgstab.ja = NULL;
									my_memory_bicgstab.ia = NULL;
									my_memory_bicgstab.alu = NULL;
									my_memory_bicgstab.jlu = NULL;
									my_memory_bicgstab.ju = NULL;
									my_memory_bicgstab.alu1 = NULL;
									my_memory_bicgstab.jlu1 = NULL;
									my_memory_bicgstab.ju1 = NULL;
									my_memory_bicgstab.x1 = NULL;
									my_memory_bicgstab.iw = NULL;
									my_memory_bicgstab.levs = NULL;
									my_memory_bicgstab.w = NULL;
									my_memory_bicgstab.jw = NULL;
									my_memory_bicgstab.w_dubl = NULL;
									my_memory_bicgstab.jw_dubl = NULL;
									// ������ ��������� � ����������� ������������� �������� � ��������� �������������.
									my_memory_bicgstab.ballocCRSt = false; // �������� ������
									my_memory_bicgstab.bsignalfreeCRSt = false; // � ����� �� �����������.
									// ������������� ����������.
									my_memory_bicgstab.tval = NULL;
									my_memory_bicgstab.tcol_ind = NULL;
									my_memory_bicgstab.trow_ptr = NULL;
									my_memory_bicgstab.tri = NULL;
									my_memory_bicgstab.troc = NULL;
									my_memory_bicgstab.ts = NULL;
									my_memory_bicgstab.tt = NULL;
									my_memory_bicgstab.tvi = NULL;
									my_memory_bicgstab.tpi = NULL;
									my_memory_bicgstab.tdx = NULL;
									my_memory_bicgstab.tdax = NULL;
									my_memory_bicgstab.ty = NULL;
									my_memory_bicgstab.tz = NULL;
									my_memory_bicgstab.ta = NULL;
									my_memory_bicgstab.tja = NULL;
									my_memory_bicgstab.tia = NULL;
									my_memory_bicgstab.talu = NULL;
									my_memory_bicgstab.tjlu = NULL;
									my_memory_bicgstab.tju = NULL;
									my_memory_bicgstab.tiw = NULL;
									my_memory_bicgstab.tlevs = NULL;
									my_memory_bicgstab.tw = NULL;
									my_memory_bicgstab.tjw = NULL;
									my_memory_bicgstab.icount_vel = 100000; // ����� ������� �����.

									// ���������� ���������������� �������� � ���������� ��������.
									doublereal **SpeedCorOldinternal = new doublereal*[3];
									for (integer i = 0; i < 3; i++) {
										SpeedCorOldinternal[i] = new doublereal[fglobal[iflow].maxelm + fglobal[iflow].maxbound];
									}
									doublereal* xb = new doublereal[fglobal[iflow].maxelm + fglobal[iflow].maxbound];
									doublereal* rthdsd = NULL; // ������ ����� ������� ���������.
									doublereal* rthdsdt = NULL;
									rthdsd = new doublereal[fglobal[iflow].maxelm + fglobal[iflow].maxbound];
									rthdsdt = new doublereal[t.maxelm + t.maxbound];

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

										

										if (i == (inumber_iteration_SIMPLE[iflow] + 1)) {
											// ��������� ������ ���������� ������ ������
											// ���� ����������������, ��������, �. ����������. 0.5; 0.8;
											// � ����� Ferczinger and Peric �������������� ���������� ���������� ���������� ������ : 0.7; 0.3; 
											// ��� �������� 0.7, � ��� �������� 0.3. ��� ���� ���������� ����� ������ ��� 0.7+0.3 == 1.0;
											if (!b_on_adaptive_local_refinement_mesh) {
												fglobal[iflow].alpha[VX] = 0.7; // 0.8 0.5
												fglobal[iflow].alpha[VY] = 0.7; // 0.8 0.5
												fglobal[iflow].alpha[VZ] = 0.7; // 0.8 0.5
												fglobal[iflow].alpha[PRESS] = 0.3; // 0.2 0.8
											}
											else {
												fglobal[iflow].alpha[VX] = 0.8; // 0.8 0.5
												fglobal[iflow].alpha[VY] = 0.8; // 0.8 0.5
												fglobal[iflow].alpha[VZ] = 0.8; // 0.8 0.5
												fglobal[iflow].alpha[PRESS] = 0.2;// 0.05; // 0.2 0.8
											}
										}
										else {
											// ����� ������������ ��������� ���������� ������������ 
											// � ����� �. ���������.
											if (!b_on_adaptive_local_refinement_mesh) {
												fglobal[iflow].alpha[VX] = 0.7; // 0.8 0.5
												fglobal[iflow].alpha[VY] = 0.7; // 0.8 0.5
												fglobal[iflow].alpha[VZ] = 0.7; // 0.8 0.5
												fglobal[iflow].alpha[PRESS] = 0.3; // 0.2 0.8
											}
											else {
												fglobal[iflow].alpha[VX] = 0.8; // 0.8 0.5
												fglobal[iflow].alpha[VY] = 0.8; // 0.8 0.5
												fglobal[iflow].alpha[VZ] = 0.8; // 0.8 0.5
												fglobal[iflow].alpha[PRESS] = 0.2;// 0.05; // 0.2 0.8
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

										doublereal dtimestepold = timestep_sequence[j];
										if (j > 0) {
											dtimestepold = timestep_sequence[j - 1];
										}

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
											rthdsd, rthdsdt, lu, my_union);

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
													//printf("%lld 1.0\n",i+1);
													printf(" %lld %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d %lld\n",
														i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
														rfluentres.res_vy, rfluentres.res_vz, im, is, ims, iend - i);
													if (i % 10 == 0) {
														printf("  iter continity x-velocity y-velocity z-velocity \t time/iter\n");
													}
#ifdef MINGW_COMPILLER
													err_stat = 0;
													fp_statistic_convergence=fopen64("statistic_convergence.txt", "a");
													if (fp_statistic_convergence == NULL) err_stat = 1;
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
												else if (eqin.itemper == 1) {
													// ������� ������������� ��������� � ���������� ����������������.
													printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d %lld\n",
														i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
														rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, im, is, ims, iend - i);
													if (i % 10 == 0) {
														printf("  iter continity x-velocity y-velocity z-velocity temperature \t time/iter\n");
													}
#ifdef MINGW_COMPILLER
													err_stat = 0;
													fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
													if (fp_statistic_convergence == NULL) err_stat = 1;
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
											continity_start[iflow] = continity;
											rfluentres.operating_value_b = rfluentres.res_no_balance;
										}
										else {
											fprintf(fpcont, "%lld %e\n", i + 1, continity / continity_start[iflow]); // ���������� � ����������
											if (!bprintmessage) {
												if (eqin.itemper == 0) {
													// ������� ������ ������������� ��� ��������� ����������������
													//printf("%lld %e\n", i+1, continity/continity_start[iflow]); // ���������� � ����������
													printf(" %lld %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
														i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
														rfluentres.res_vy, rfluentres.res_vz, im, is, ims, iend - i);
													if (i % 10 == 0) {
														printf("  iter continity x-velocity y-velocity z-velocity \t time/iter\n");
													}
#ifdef MINGW_COMPILLER
													err_stat = 0;
													fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
													if (fp_statistic_convergence == NULL) err_stat = 1;
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
												else if (eqin.itemper == 1) {
													// ������� ������������� ��������� � ���������� ����������������.
													printf(" %lld %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %lld\n",
														i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
														rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, im, is, ims, iend - i);
													if (i % 10 == 0) {
														printf("  iter continity x-velocity y-velocity z-velocity temperature \t time/iter\n");
													}
#ifdef MINGW_COMPILLER
													err_stat = 0;
													fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
													if (fp_statistic_convergence == NULL) err_stat = 1;
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
#else
								//if (i==5) continity_start[iflow]=continity;
										if (i <= 5) {
											fprintf(fpcont, "%d 1.0\n", i + 1);
											if (!bprintmessage) {
												if (eqin.itemper == 0) {
													// ������� ������ ������������� ��� ��������� ����������������
													//printf("%d 1.0\n",i+1);
													printf(" %d %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d %d\n",
														i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
														rfluentres.res_vy, rfluentres.res_vz, im, is, ims, iend - i);
													if (i % 10 == 0) {
														printf("  iter continity x-velocity y-velocity z-velocity \t time/iter\n");
													}
#ifdef MINGW_COMPILLER
													err_stat = 0;
													fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
													if (fp_statistic_convergence == NULL) err_stat = 1;
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
												else if (eqin.itemper == 1) {
													// ������� ������������� ��������� � ���������� ����������������.
													printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d %d\n",
														i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
														rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, im, is, ims, iend - i);
													if (i % 10 == 0) {
														printf("  iter continity x-velocity y-velocity z-velocity temperature \t time/iter\n");
													}
#ifdef MINGW_COMPILLER
													err_stat = 0;
													fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
													if (fp_statistic_convergence == NULL) err_stat = 1;
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
											continity_start[iflow] = continity;
											rfluentres.operating_value_b = rfluentres.res_no_balance;
										}
										else {
											fprintf(fpcont, "%d %e\n", i + 1, continity / continity_start[iflow]); // ���������� � ����������
											if (!bprintmessage) {
												if (eqin.itemper == 0) {
													// ������� ������ ������������� ��� ��������� ����������������
													//printf("%d %e\n", i+1, continity/continity_start[iflow]); // ���������� � ����������
													printf(" %d %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %d\n",
														i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
														rfluentres.res_vy, rfluentres.res_vz, im, is, ims, iend - i);
													if (i % 10 == 0) {
														printf("  iter continity x-velocity y-velocity z-velocity \t time/iter\n");
													}
#ifdef MINGW_COMPILLER
													err_stat = 0;
													fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
													if (fp_statistic_convergence == NULL) err_stat = 1;
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
												else if (eqin.itemper == 1) {
													// ������� ������������� ��������� � ���������� ����������������.
													printf(" %d %1.4e %1.4e %1.4e %1.4e %1.4e %1d:%2d:%2d  %d\n",
														i_gl, rfluentres.res_no_balance, rfluentres.res_vx,
														rfluentres.res_vy, rfluentres.res_vz, rfluentrestemp, im, is, ims, iend - i);
													if (i % 10 == 0) {
														printf("  iter continity x-velocity y-velocity z-velocity temperature \t time/iter\n");
													}
#ifdef MINGW_COMPILLER
													err_stat = 0;
													fp_statistic_convergence = fopen64("statistic_convergence.txt", "a");
													if (fp_statistic_convergence == NULL) err_stat = 1;
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

									for (integer i = 0; i < 3; i++) {
										delete[] SpeedCorOldinternal[i];
									}
									delete[] SpeedCorOldinternal;

									delete[] xb; // �� �������� ����������� ������.
									delete[] rthdsd;
									delete[] rthdsdt;

									for (integer i = 0; i < 3; i++) delete[] rhie_chow[i];
									delete[] rhie_chow;
								
							


							// ���������� ��� �� ������� :

							/* //��������������� 14.05.2019
							//������ ��������.
							char* buffer = NULL;
							buffer = new char[10];
							buffer[0] = '\0';
							KRitoa(j, buffer);
							//printf("%s\n",buffer);
							char* mymessage = NULL;
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
							if (buffer != NULL) {
								delete[] buffer;
							}
							if (mymessage != NULL) {
								delete[] mymessage;
							}
							*/

							// ���������� ���� ���������� :
							for (integer i1 = 0; i1 < t.maxelm + t.maxbound; i1++) {
								toldtimestep[i1] = t.potent[i1]; // copy end time step
							}

							// ���������� ���� �������� :
							
								for (integer i3 = 0; i3 < (fglobal[iflow].maxelm + fglobal[iflow].maxbound); i3++) {
									// i1 - ����� FLUID INTERIOR,
									// i2 - VX, VY, VZ - ���� �� ��� ��������� ��������,
									// i3 - ��������������� ����� ������������ ������ 
									speedoldtimestep[VX][i3] = fglobal[iflow].potent[VX][i3]; // copy end time step
									speedoldtimestep[VY][i3] = fglobal[iflow].potent[VY][i3]; // copy end time step
									speedoldtimestep[VZ][i3] = fglobal[iflow].potent[VZ][i3]; // copy end time step
								}
							

							// ���������� ������������ ����� ����� ������� �� :
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

							// �� ��������� ���� �� ������� �� ������� ������, 
							// ������� � ���������� �������� �������.
							for (integer i = 0; i < flow_interior; i++) continity_start[i] = 1.0;
							for (integer i = 0; i < flow_interior; i++) inumber_iteration_SIMPLE[i] = 0; // ��������� �������� ��������� SIMPLE ��� ������ FLUID ����.

						}  // ����� ������ ���� �� �������.

						// ������������ ����������� ������.
						for (integer i = 0; i < fglobal[iflow].maxelm; i++) {
							delete[]  mfold[i];
							delete[] mfoldtimestep[i];
						}
						
											
						delete[] mfold;
						delete[] mfoldtimestep;
						mfold = NULL;
						mfoldtimestep = NULL;

						
						for (integer i2 = 0; i2 < 3; i2++) {
							// i1 - ����� FLUID INTERIOR,
							// i2 - VX, VY, VZ - ���� �� ��� ��������� ��������,
							// i3 - ��������������� ����� ������������ ������ (����������
							delete speedoldtimestep[i2];
						}						
						
						if (speedoldtimestep != NULL) {
							delete[] speedoldtimestep;
						}
						speedoldtimestep = NULL;

						if (toldtimestep != NULL) {
							delete[] toldtimestep;
						}
						toldtimestep = NULL;

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

} // unsteady_cfd_calculation




#endif