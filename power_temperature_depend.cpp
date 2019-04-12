// � ����� power_temperature_depend.cpp ����������
// ���������� ����������� �������� ��������� ����� ��
// �������� �������� ����������� � �������� �����.
// �������������� ������� � ���� ������� ��� 
// ������ ���������� �������� � �����:
//        0�      5�     10�    15�    20�    25�    30�
//  20    0.0    10��   20��   30��   35��   40��   50�� 
//  30    0.0    11��   22��   31��   37��   ...
//  40    0.0    12��   24��   32��   ...
//  70    ...    ...    ...    ...
//  100
//  120
//  150
// ������ ������� ���� �������� ���� �������� � ������ ���������.
// ��� � ����� ����� ������� ����������� ����������� ����� intemp ����,
// � ������ ������ ����������� ��������� �������� ������ � ���������� inoffsetdrain.
// ������� ������ ���� ��������� ��������� �������� �� �������������� � �������� ���� ������ ����������.
// ��������� ����� (�) �� ����������� ����������������  ������ � ������� ��� ���������������
// ������ ����������� � �������� ����� �������� ������������ � ����� ��������.
// �.�. �������� ����������� � ��������� �������� �������� ����� ��������� ���� �������� ����������,
// �� ��� ����� �������� ���� �������� (�����������, �������� �����) ��������������� �������� ������������
// � ����� �������� ������������  �������� � ������� ���������� �������������.
// � ������ ��� ������ ������ ������� (������� �����������  �������� �����������) ������������ ����������� ���� ������ 
// ���������������� �������� � ������ ������. ����� ���� intemp ����� �������� ����� ����� ������� �������� ���������������
// ������������� �������� �������� �����. ����� �������� ������ ��� ����� ������ ������� ����� ���������� ������� ��������
// � ���� ������� ��������������� ������������� �������� �����������.
// ������ ���� ����������� � ���� ����� ������� � ���������� ������� ������� ������ :
// 1. ��������� ������� �� ���������� �����.
// 2. �������� ������� ������� ��� ���������� �������� �����.
// 3. �������� �������� ������� � ������������ ����� ��� ����������� ���� �������� ��� ��������.
// 4. ���� ������� ����� power_table[i][j] �������� �������� ������������ ��������, ���������������
// �������� ����������� � �������� �����.
// 
// ���������� ����� ������ ����������� ����������� ��������� �����, ��������, �� 0� �� 30�.
// ��� ������� �������� ����� ����� ������� ������������ ��������. ��� ������������� 
// �������� ����� ����� ������������� ������������ � ����� ��������. �������� �����,
// � ������ � ������������ � ����� �������� �������� � �������� ������� - ����� �����
// ������������� ��������� (������������� ������ � ���������� ������ � ��������� ����������� Q).
// �� ����� ���� �������� �� ������������� � ����� ����� �������� : �������� ����� 70��, ������ 100��.
// ������������ �������� ����� ���� : 2���, 20���, 200���, 2000���. �������� ����� �������� ������ �� �������,
// � � ������ �������� ��� �������, � � ������� ������������ �������������� �������� tau ��� ��������� (����� ���������)
// � ����� �������� 30� > 0�. � ����� �������� ����� ���������� � ������������� ������������� �������.
// begin 23 ������ 2012 ����.

// ������� ��������� �������� ������������ �� ����� �.�. ������� 
// ������ ���������������� �� VBA Exel � ��������� �������.

#ifndef MY_POWER_TEMPERATYRE_DEPEND_CPP
#define MY_POWER_TEMPERATYRE_DEPEND_CPP 1

#include <stdlib.h>

// ���������� ������� �� ���������� ����� :
// power_table.txt
void my_read_power_table(char* sname, integer &intemp, integer &inoffset_drain, 
	                     doublereal* &rtemp, doublereal* &roffset_drain,
						 doublereal** &rpower_table) 
{
	// sname - ���������� ��� ����� � ��������� ������������.
	intemp=0; inoffset_drain=0; // ��� �������� ��������� �������� ���� ���� �� ������ ���� ������.
	
	FILE *fpt=NULL; // ���� �� �������� ����� ����������� ������� ���������.
	errno_t err;
	err = fopen_s(&fpt, sname, "r");

	if ((err ) != 0) {
		// ���� ����������� ������ ��� ������,
		// ���� ������ ����� ������ �� ���� �� ����� �������� �� ������ ����������.
	    printf("Open File %s Error\n",sname);
       // system("PAUSE");
		system("pause");
        exit(0);
	}
	else {
		if (fpt != NULL) {
			// ��������� ������������ ����� ���������:
			// ������� ��� intemp ������ inoffset_drain ������� �� ����� ������
			// � ����� ������ ���������� ������� ��������� ��� ������.

			//printf("comming...\n"); // debug

			integer din = 0;
			float fin = 0.0;


#ifdef MINGW_COMPILLER
			fscanf(fpt, "%d", &din);
			intemp = din; // ���������� ��������� ���������� �������� �����������.
			fscanf(fpt, "%d", &din);
			inoffset_drain = din; // ���������� ��������� ���������� �������� �����������.

#elseif sizeof(integer) == 4
			fscanf_s(fpt, "%d", &din);
			intemp = din; // ���������� ��������� ���������� �������� �����������.
			fscanf_s(fpt, "%d", &din);
			inoffset_drain = din; // ���������� ��������� ���������� �������� �����������.
#else
			fscanf_s(fpt, "%lld", &din);
			intemp = din; // ���������� ��������� ���������� �������� �����������.
			fscanf_s(fpt, "%lld", &din);
			inoffset_drain = din; // ���������� ��������� ���������� �������� �����������.
#endif


		//printf("%d %d \n",intemp,din); // debug

		// ��������� ����������� ������ ��� �������:
			rtemp = new doublereal[intemp];
			roffset_drain = new doublereal[inoffset_drain];
			rpower_table = new doublereal*[intemp];
			for (integer i = 0; i < intemp; i++) rpower_table[i] = new doublereal[inoffset_drain];

			// ���������� �������� �� ����� :

			// �������� �������� �����.
			for (integer i = 0; i < inoffset_drain; i++) {

#ifdef MINGW_COMPILLER
				fscanf(fpt, "%f", &fin);
#else

				fscanf_s(fpt, "%f", &fin);
#endif

				roffset_drain[i] = fin;
			}

			// ��������������� �������� ����������� � 
			// ������ ��������� ��� ������ �������� �������� �����.
			for (integer j = 0; j < intemp; j++) {
				// ���������� ������� ���������.
				for (integer i = 0; i <= inoffset_drain; i++) {

#ifdef MINGW_COMPILLER
					fscanf(fpt, "%f", &fin);
#else
					fscanf_s(fpt, "%f", &fin);
#endif
					if (i == 0) {
						rtemp[j] = fin;
					}
					else {
						rpower_table[j][i - 1] = fin;
					}
				}
			}

			fclose(fpt); // ������������ ��������� �����������.
		}
	}
} // my_read_power_table

// �������� ������� �� �������.
void my_print_table(integer intemp, integer inoffset_drain, 
	                     doublereal* rtemp, doublereal* roffset_drain,
						 doublereal** rpower_table) {
	printf("\t   ");
	for (integer i=0; i<inoffset_drain; i++) {
		printf("%.2e ",roffset_drain[i]);
	}
	printf("\n");

	for (integer i=0; i<intemp; i++) {
		printf("%.3e ",rtemp[i]);
		for (integer j=0; j<inoffset_drain; j++) printf("%.3e ",rpower_table[i][j]);
		printf("\n");
	}
}

// ���������� �������� ������� �� ����� �.�.�������.
// ���. 238-246. "������ ���������������� �� VBA Exel � ��������� �������."
void mos(integer n, doublereal* &x, doublereal* &f, doublereal gamma_left, doublereal delta_left,
	     doublereal alpha_right, doublereal delta_right, doublereal* &mom) 
{

	if (n >= 4) {

		// ����� ���������� �������� �������� ������� ������ �������������
		// ���������.
		// n - ���������� ���������� �������� �������,
		// x - �������� ���������� �������,
		// f - ��������������� �������� �������.
		// mom - ��������������� ������� �������.
		// ��� ���������� �������� ����� ���� � ��� ������������ �������� :
		// alpha[i]*mom[i-1]+2.0*mom[i]+gamma[i]*mom[i+1]=delta[i]; // i - �� ��������� 3-� ������������ ����.
		// � ���������� ���������:
		// 2*mom[0]+gamma_left*mom[1]=delta_left; // ����� ��������� �������.
		// alpha_right*mom[n-2]+2.0*mom[n-1]=delta_right; // ������ ��������� �������.

		doublereal ralpha=0.0, rgamma=0.0, rdelta=0.0, rw=0.0; // r - doublereal;
		integer i=0;
		doublereal* rH = NULL;
		rH = new doublereal[n];
		for (i = 1; i < n; i++) rH[i] = x[i] - x[i - 1]; // ���� �����
		doublereal* rP = NULL;
		doublereal* rQ = NULL;

		rP = new doublereal[n];
		rQ = new doublereal[n];

		// ������ ��������:
		rP[1] = -0.5*gamma_left;
		rQ[1] = 0.5*delta_left;
		for (i = 1; i < n - 1; i++) {
			rw = rH[i] + rH[i + 1];
			ralpha = rH[i] / rw;
			rgamma = 1.0 - ralpha;
			rdelta = 6.0*(((f[i + 1] - f[i]) / rH[i + 1]) - ((f[i] - f[i - 1]) / rH[i])) / rw;
			rw = ralpha*rP[i] + 2.0;
			rP[i + 1] = -rgamma / rw;
			rQ[i + 1] = (rdelta - ralpha*rQ[i]) / rw;
		} // end for i

		// �������� ��������:
		mom[n - 1] = (delta_right - alpha_right*rQ[n - 1]) / (alpha_right*rP[n - 1] + 2.0);
		for (i = n - 1; i > 0; i--) {
			mom[i - 1] = rP[i] * mom[i] + rQ[i];
		} // end for i

		// ������������ ����������� ������:
		// �� ����� ������ ����������� ��������� �� NULL ��� ��� ������
		// ���� ����� �������� ���������� new.
		//if (rH != NULL) {
			delete[] rH;
		//}
		//if (rP != NULL) {
			delete[] rP;
		//}
		//if (rQ != NULL) {
			delete[] rQ;
		//}

	}
	else {
		printf("Error dimensional for mos function. \n");
		printf("parametr n<=3. error. \n");
		system("pause");
		exit(1);
	}
} // mos

// ������������ ���������� ������������.
// ��������� �������� ������� � �������� �����.
// ����������� �� ����� �.�.�������.
// ���. 238-246. "������ ���������������� �� VBA Exel � ��������� �������."
void si(integer n, doublereal* x, doublereal* f, doublereal* mom, doublereal chi,
	    doublereal &s, doublereal &s1, doublereal &s2) {
	 // n - ���������� ���������� ����� �� ������� �������� ������,
     // x - ������ �������� ���������� �������,
     // f - ������ �������� �������,
	 // mom - ������ �������� �������,
	 // chi - ���������� ����� �� ������� ����������� ������� � ������� ����� ��������� �������� �������,
	 // s - �������������� �������� �������,
	 // s1, s2 - �������������� �������� ������ � ������ ����������� �������.

	integer i;
	doublereal rh, rhh, rh1, rh1h1, rh2, rh2h2; // r - doublereal
	// ���������� ������������� �������, ����������� chi:
	for (i=1; i<n; i++) {
		if (x[i]>chi) break; // �������������� ����� �� �����
	}
	if (i>n-1) i=n-1;
	// ������ �������� ������� � ����� chi :
	rh=x[i]-x[i-1];
	rhh=rh*rh;
	rh1=chi-x[i-1];
	rh1h1=rh1*rh1;
	rh2=x[i]-chi;
	rh2h2=rh2*rh2;
	s=(mom[i-1]*rh2h2*rh2+mom[i]*rh1h1*rh1)/(6.0*rh)+((f[i-1]-mom[i-1]*rhh/6.0)*rh2+(f[i]-mom[i]*rhh/6.0)*rh1)/rh;
	// ������ ������ ����������� ������� � ����� chi :
	s1=(-mom[i-1]*rh2h2+mom[i]*rh1h1)/(2.0*rh)+(f[i]-f[i-1])/rh-rh*(mom[i]-mom[i-1])/6.0;
	// ������ ������ ����������� ������� � ����� chi :
	s2=(mom[i-1]*rh2+mom[i]*rh1)/rh;
} // si


// �� �������� �� ������ ������� ���������, ���������� �������� ��������
// � ������������ �����(rtemp_current, roffset_drain_current) ���� 
// ���������� �������������.
doublereal my_splain_interpol_power_table(integer intemp, integer inoffset_drain, 
	                     doublereal* rtemp, doublereal* roffset_drain, doublereal** rpower_table, 
						 doublereal rtemp_current, doublereal roffset_drain_current)  
{

	// ��������:
	// ������� �������� ������� ��� ������ ������ �������.
	// ���� ������ ��� ������ ������� ������������ ���������� �������� �������
	// ��� ��������� �������� �������� ����� � ������ ������ ������, �.�. ��� �������� �����������.
	// �������� ��� ��� ���� ����� �� ������� ������� ��������������� ��������� �������� �����.
	// �������� ���������� ������������ ��� ����������� ������� ����� ��� ����� ��������� ��������
	// ������� ������������ �� ����� ������� ��� �������� ����������� - ��� � ����� ������� ��������.

	doublereal rpower_curent;

	doublereal* m2=new doublereal[inoffset_drain];
	doublereal** mm=new doublereal*[intemp];
	for (integer i=0; i<intemp; i++) mm[i]=new doublereal[inoffset_drain];
	doublereal* f2=new doublereal[inoffset_drain];

	// ������ ���������� ������� �������� �� ��������� �������� ����� ��� ���� ����� �������
	// ��������������� ������ ����.
	for (integer i=0; i<intemp; i++) {
		for (integer j=0; j<inoffset_drain; j++) {
			// ���������� ������ ��������� � ������� f2:
			f2[j]=rpower_table[i][j];
		}  // end for j
		mos(inoffset_drain,roffset_drain,f2,0.0,0.0,0.0,0.0,m2);
		for (integer j=0; j<inoffset_drain; j++) {
			mm[i][j]=m2[j];
		} // end for j
	}


	doublereal* f1=new doublereal[intemp];
	for (integer i=0; i<intemp; i++) {
		// ��� ���� �������� ����������� ���������
		// ��������������� ��������� �������� �������� �����:
		for (integer j=0; j<inoffset_drain; j++) {
			f2[j]=rpower_table[i][j];
			m2[j]=mm[i][j];
		}
		// ���������� ������ ���������������� ��������� �������� �������� �����.
		doublereal s1, s2;
		si(inoffset_drain, roffset_drain, f2, m2, roffset_drain_current,  f1[i], s1, s2); 
	}
	doublereal* m1=new doublereal[intemp];
	mos(intemp,rtemp,f1,0.0,0.0,0.0,0.0,m1);
	doublereal s1, s2;
	// ���������� �������� �������� �������� ���� ��������� ���������� ������������ �� 
	// �������, ����������� � ���������� ������������ ������� �������.
    si(intemp, rtemp, f1, m1, rtemp_current,  rpower_curent, s1, s2);

	// ������������ ����������� ������:
	delete[] m2;
	delete[] f2;
	for (integer i=0; i<intemp; i++) delete[] mm[i];
	delete[] mm;
	delete[] f1; 
	delete[] m1;

	//printf("power is %e\n",rpower_curent); // debug

	return rpower_curent;
}

#endif
