// ���� my_elmatr_quad_f.c 
// ������ ������� ��� �����������
// ��������� ���������-�������� �� 
// ����������� �����.

#ifndef MY_ELMATR_QUAD_F3D_C
#define MY_ELMATR_QUAD_F3D_C 1




#include "my_linalg.cu" // ���������� ������� �������� �������
// ��� �������: 
// eqsolve_simple_gauss - ������ ���� ������� ���������� ������
// eqsolv_simple_holesskii - ������ ���� ������� ���������� ����������


#include "my_interpolate_v0_07.cpp" // ������� ��� �������������.

// ������������� ������������� ����� A(|P|)
#include "my_approx_convective.c" // �� ��������� ����� BULG

#include "my_approx_convective2.c" // ������������� ������������� � ������������� �����


#define distsheme 100 // ��������� �������� �� ������ ����� � �����



#include "greengauss.c" // ���������� ��������� ��������
#include "avtosave.c" // �������������� � ������������� �����.

#include "rhie_chow.cpp" // �������� ���-��� 1983 �.

// ���������� ������ ����������� �������� � �������� � ������ ����� ��
doublereal DFDXiP(doublereal* potent, integer iP, integer G, ALICE_PARTITION** sosedi, integer maxelm,
	         integer** nvtx, TOCHKA* pa, bool &bborder) {

	bborder=false; // �� ��������� ������ ���������� ���� ����� �� ������.

	integer iG, iGG, ibackG;
	bool bG=false, bbackG=false, bGG=false;
	TOCHKA pointP;
	doublereal x1, x2, x3, x4, f1, f2, f3, f4, xg;
	doublereal dx=0.0, dy=0.0, dz=0.0;
	volume3D(iP, nvtx, pa, dx, dy, dz);
	doublereal dx1=0.0, dy1=0.0, dz1=0.0;
	doublereal DFDXgran=0.0;

	switch (G) {
	case ESIDE :  iG=sosedi[ESIDE][iP].iNODE1;
		      if (iG>=maxelm) bG=true;
			  if (bG) {
				  bGG=true;
			  }
			  else {
				  iGG=sosedi[ESIDE][iG].iNODE1;
				  if (iGG>=maxelm) bGG=true;
			  }
			  ibackG=sosedi[WSIDE][iP].iNODE1;
			  if (ibackG>=maxelm) bbackG=true;

			  if (!bG && !bGG && !bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x2=pointP.x;
				  center_cord3D(ibackG,nvtx,pa,pointP,WSIDE);
				  x1=pointP.x;
				  center_cord3D(iG,nvtx,pa,pointP,ESIDE);
				  x3=pointP.x;
				  center_cord3D(iGG,nvtx,pa,pointP,EE);
				  x4=pointP.x;
				  xg=x2+0.5*dx;
				  f1=potent[ibackG];
				  f2=potent[iP];
				  f3=potent[iG];
				  f4=potent[iGG];

				  DFDXgran=DFDXg(x1, x2, x3, x4, xg, f1, f2, f3, f4);
				  bborder=false;
			  }

			  if (!bG && bGG && !bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x2=pointP.x;
				  center_cord3D(ibackG,nvtx,pa,pointP,WSIDE);
				  x1=pointP.x;
				  center_cord3D(iG,nvtx,pa,pointP,ESIDE);
				  x3=pointP.x;
				  volume3D(iG, nvtx, pa, dx1, dy1, dz1);
				  x4=pointP.x+0.5*dx1;
				  xg=x2+0.5*dx;
				  f1=potent[ibackG];
				  f2=potent[iP];
				  f3=potent[iG];
				  f4=potent[iGG];

				  DFDXgran=DFDXg(x1, x2, x3, x4, xg, f1, f2, f3, f4);
				  bborder=true;
			  }

			  if (!bG && !bGG && bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x2=pointP.x;
				  volume3D(iP, nvtx, pa, dx1, dy1, dz1);
				  x1=x2-0.5*dx1;
				  center_cord3D(iG,nvtx,pa,pointP,ESIDE);
				  x3=pointP.x;
				  center_cord3D(iGG,nvtx,pa,pointP,EE);
				  x4=pointP.x;
				  xg=x2+0.5*dx;
				  f1=potent[ibackG];
				  f2=potent[iP];
				  f3=potent[iG];
				  f4=potent[iGG];

				  DFDXgran=DFDXg(x1, x2, x3, x4, xg, f1, f2, f3, f4);
				  bborder=true;
			  }

			  if (bG && !bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x2=pointP.x;
				  center_cord3D(ibackG,nvtx,pa,pointP,WSIDE);
				  x1=pointP.x;
				  xg=x3=x2+0.5*dx;
				  f1=potent[ibackG];
				  f2=potent[iP];
				  f3=potent[iG];

				  DFDXgran=DFDXg2(x1, x2, x3, xg, f1, f2, f3);
				  bborder=true;
			  }

		    break;
		case NSIDE :  iG=sosedi[NSIDE][iP].iNODE1;
		      if (iG>=maxelm) bG=true;
			  if (bG) {
				  bGG=true;
			  }
			  else {
				  iGG=sosedi[NSIDE][iG].iNODE1;
				  if (iGG>=maxelm) bGG=true;
			  }
			  ibackG=sosedi[SSIDE][iP].iNODE1;
			  if (ibackG>=maxelm) bbackG=true;

			  if (!bG && !bGG && !bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x2=pointP.y;
				  center_cord3D(ibackG,nvtx,pa,pointP,SSIDE);
				  x1=pointP.y;
				  center_cord3D(iG,nvtx,pa,pointP,NSIDE);
				  x3=pointP.y;
				  center_cord3D(iGG,nvtx,pa,pointP,NN);
				  x4=pointP.y;
				  xg=x2+0.5*dy;
				  f1=potent[ibackG];
				  f2=potent[iP];
				  f3=potent[iG];
				  f4=potent[iGG];

				  DFDXgran=DFDXg(x1, x2, x3, x4, xg, f1, f2, f3, f4);
				   bborder=false;
			  }

			  if (!bG && bGG && !bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x2=pointP.y;
				  center_cord3D(ibackG,nvtx,pa,pointP,SSIDE);
				  x1=pointP.y;
				  center_cord3D(iG,nvtx,pa,pointP,NSIDE);
				  x3=pointP.y;
				  volume3D(iG, nvtx, pa, dx1, dy1, dz1);
				  x4=pointP.y+0.5*dy1;
				  xg=x2+0.5*dy;
				  f1=potent[ibackG];
				  f2=potent[iP];
				  f3=potent[iG];
				  f4=potent[iGG];

				  DFDXgran=DFDXg(x1, x2, x3, x4, xg, f1, f2, f3, f4);
				  bborder=true;
			  }

			  if (!bG && !bGG && bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x2=pointP.y;
				  volume3D(iP, nvtx, pa, dx1, dy1, dz1);
				  x1=x2-0.5*dy1;
				  center_cord3D(iG,nvtx,pa,pointP,NSIDE);
				  x3=pointP.y;
				  center_cord3D(iGG,nvtx,pa,pointP,NN);
				  x4=pointP.y;
				  xg=x2+0.5*dy;
				  f1=potent[ibackG];
				  f2=potent[iP];
				  f3=potent[iG];
				  f4=potent[iGG];

				  DFDXgran=DFDXg(x1, x2, x3, x4, xg, f1, f2, f3, f4);
				  bborder=true;
			  }

			  if (bG && !bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x2=pointP.y;
				  center_cord3D(ibackG,nvtx,pa,pointP,SSIDE);
				  x1=pointP.y;
				  xg=x3=x2+0.5*dy;
				  f1=potent[ibackG];
				  f2=potent[iP];
				  f3=potent[iG];

				  DFDXgran=DFDXg2(x1, x2, x3, xg, f1, f2, f3);
				  bborder=true;
			  }

		    break;
	 	 
		case TSIDE :   iG=sosedi[TSIDE][iP].iNODE1;
		      if (iG>=maxelm) bG=true;
			  if (bG) {
				  bGG=true;
			  }
			  else {
				  iGG=sosedi[TSIDE][iG].iNODE1;
				  if (iGG>=maxelm) bGG=true;
			  }
			  ibackG=sosedi[BSIDE][iP].iNODE1;
			  if (ibackG>=maxelm) bbackG=true;

			  if (!bG && !bGG && !bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x2=pointP.z;
				  center_cord3D(ibackG,nvtx,pa,pointP,BSIDE);
				  x1=pointP.z;
				  center_cord3D(iG,nvtx,pa,pointP,TSIDE);
				  x3=pointP.z;
				  center_cord3D(iGG,nvtx,pa,pointP,TTSIDE);
				  x4=pointP.z;
				  xg=x2+0.5*dz;
				  f1=potent[ibackG];
				  f2=potent[iP];
				  f3=potent[iG];
				  f4=potent[iGG];

				  DFDXgran=DFDXg(x1, x2, x3, x4, xg, f1, f2, f3, f4);
				   bborder=false;
			  }

			  if (!bG && bGG && !bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x2=pointP.z;
				  center_cord3D(ibackG,nvtx,pa,pointP,BSIDE);
				  x1=pointP.z;
				  center_cord3D(iG,nvtx,pa,pointP,TSIDE);
				  x3=pointP.z;
				  volume3D(iG, nvtx, pa, dx1, dy1, dz1);
				  x4=pointP.z+0.5*dz1;
				  xg=x2+0.5*dz;
				  f1=potent[ibackG];
				  f2=potent[iP];
				  f3=potent[iG];
				  f4=potent[iGG];

				  DFDXgran=DFDXg(x1, x2, x3, x4, xg, f1, f2, f3, f4);
				   bborder=true;
			  }

			  if (!bG && !bGG && bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x2=pointP.z;
				  volume3D(iP, nvtx, pa, dx1, dy1, dz1);
				  x1=x2-0.5*dz1;
				  center_cord3D(iG,nvtx,pa,pointP,TSIDE);
				  x3=pointP.z;
				  center_cord3D(iGG,nvtx,pa,pointP,TTSIDE);
				  x4=pointP.z;
				  xg=x2+0.5*dz;
				  f1=potent[ibackG];
				  f2=potent[iP];
				  f3=potent[iG];
				  f4=potent[iGG];

				  DFDXgran=DFDXg(x1, x2, x3, x4, xg, f1, f2, f3, f4);
				   bborder=true;
			  }

			  if (bG && !bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x2=pointP.z;
				  center_cord3D(ibackG,nvtx,pa,pointP,BSIDE);
				  x1=pointP.z;
				  xg=x3=x2+0.5*dz;
				  f1=potent[ibackG];
				  f2=potent[iP];
				  f3=potent[iG];

				  DFDXgran=DFDXg2(x1, x2, x3, xg, f1, f2, f3);
				   bborder=true;
			  }

		    break;

		case WSIDE:  iG=sosedi[WSIDE][iP].iNODE1;
		      if (iG>=maxelm) bG=true;
			  if (bG) {
				  bGG=true;
			  }
			  else {
				  iGG=sosedi[WSIDE][iG].iNODE1;
				  if (iGG>=maxelm) bGG=true;
			  }
			  ibackG=sosedi[ESIDE][iP].iNODE1;
			  if (ibackG>=maxelm) bbackG=true;

			  if (!bG && !bGG && !bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x3=pointP.x;
				  center_cord3D(ibackG,nvtx,pa,pointP,ESIDE);
				  x4=pointP.x;
				  center_cord3D(iG,nvtx,pa,pointP,WSIDE);
				  x2=pointP.x;
				  center_cord3D(iGG,nvtx,pa,pointP,WW);
				  x1=pointP.x;
				  xg=x3-0.5*dx;
				  f4=potent[ibackG];
				  f3=potent[iP];
				  f2=potent[iG];
				  f1=potent[iGG];

				  DFDXgran=DFDXg(x1, x2, x3, x4, xg, f1, f2, f3, f4);
				   bborder=false;
			  }

			  if (!bG && bGG && !bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x3=pointP.x;
				  center_cord3D(ibackG,nvtx,pa,pointP,ESIDE);
				  x4=pointP.x;
				  center_cord3D(iG,nvtx,pa,pointP,WSIDE);
				  x2=pointP.x;
				  volume3D(iG, nvtx, pa, dx1, dy1, dz1);
				  x1=pointP.x-0.5*dx1;
				  xg=x3-0.5*dx;
				  f4=potent[ibackG];
				  f3=potent[iP];
				  f2=potent[iG];
				  f1=potent[iGG];

				  DFDXgran=DFDXg(x1, x2, x3, x4, xg, f1, f2, f3, f4);
				   bborder=true;
			  }

			  if (!bG && !bGG && bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x3=pointP.x;
				  volume3D(iP, nvtx, pa, dx1, dy1, dz1);
				  x4=x3+0.5*dx1;
				  center_cord3D(iG,nvtx,pa,pointP,WSIDE);
				  x2=pointP.x;
				  center_cord3D(iGG,nvtx,pa,pointP,WW);
				  x1=pointP.x;
				  xg=x3-0.5*dx;
				  f4=potent[ibackG];
				  f3=potent[iP];
				  f2=potent[iG];
				  f1=potent[iGG];

				  DFDXgran=DFDXg(x1, x2, x3, x4, xg, f1, f2, f3, f4);
				   bborder=true;
			  }

			  if (bG && !bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x2=pointP.x;
				  center_cord3D(ibackG,nvtx,pa,pointP,ESIDE);
				  x3=pointP.x;
				  xg=x1=x2-0.5*dx;
				  f3=potent[ibackG];
				  f2=potent[iP];
				  f1=potent[iG];

				  DFDXgran=DFDXg2(x1, x2, x3, xg, f1, f2, f3);
				   bborder=true;
			  }

		    break;

		case SSIDE :  iG=sosedi[SSIDE][iP].iNODE1;
		      if (iG>=maxelm) bG=true;
			  if (bG) {
				  bGG=true;
			  }
			  else {
				  iGG=sosedi[SSIDE][iG].iNODE1;
				  if (iGG>=maxelm) bGG=true;
			  }
			  ibackG=sosedi[NSIDE][iP].iNODE1;
			  if (ibackG>=maxelm) bbackG=true;

			  if (!bG && !bGG && !bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x3=pointP.y;
				  center_cord3D(ibackG,nvtx,pa,pointP,NSIDE);
				  x4=pointP.y;
				  center_cord3D(iG,nvtx,pa,pointP,SSIDE);
				  x2=pointP.y;
				  center_cord3D(iGG,nvtx,pa,pointP,SS);
				  x1=pointP.y;
				  xg=x3-0.5*dy;
				  f4=potent[ibackG];
				  f3=potent[iP];
				  f2=potent[iG];
				  f1=potent[iGG];

				  DFDXgran=DFDXg(x1, x2, x3, x4, xg, f1, f2, f3, f4);
				   bborder=false;
			  }

			  if (!bG && bGG && !bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x3=pointP.y;
				  center_cord3D(ibackG,nvtx,pa,pointP,NSIDE);
				  x4=pointP.y;
				  center_cord3D(iG,nvtx,pa,pointP,SSIDE);
				  x2=pointP.y;
				  volume3D(iG, nvtx, pa, dx1, dy1, dz1);
				  x1=pointP.y-0.5*dy1;
				  xg=x3-0.5*dy;
				  f4=potent[ibackG];
				  f3=potent[iP];
				  f2=potent[iG];
				  f1=potent[iGG];

				  DFDXgran=DFDXg(x1, x2, x3, x4, xg, f1, f2, f3, f4);
				   bborder=true;
			  }

			  if (!bG && !bGG && bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x3=pointP.y;
				  volume3D(iP, nvtx, pa, dx1, dy1, dz1);
				  x4=x3+0.5*dy1;
				  center_cord3D(iG,nvtx,pa,pointP,SSIDE);
				  x2=pointP.y;
				  center_cord3D(iGG,nvtx,pa,pointP,SS);
				  x1=pointP.y;
				  xg=x3-0.5*dy;
				  f4=potent[ibackG];
				  f3=potent[iP];
				  f2=potent[iG];
				  f1=potent[iGG];

				  DFDXgran=DFDXg(x1, x2, x3, x4, xg, f1, f2, f3, f4);
				   bborder=true;
			  }

			  if (bG && !bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x2=pointP.y;
				  center_cord3D(ibackG,nvtx,pa,pointP,NSIDE);
				  x3=pointP.y;
				  xg=x1=x2-0.5*dy;
				  f3=potent[ibackG];
				  f2=potent[iP];
				  f1=potent[iG];

				  DFDXgran=DFDXg2(x1, x2, x3, xg, f1, f2, f3);
				   bborder=true;
			  }

		    break;

			case BSIDE :  iG=sosedi[BSIDE][iP].iNODE1;
		      if (iG>=maxelm) bG=true;
			  if (bG) {
				  bGG=true;
			  }
			  else {
				  iGG=sosedi[BSIDE][iG].iNODE1;
				  if (iGG>=maxelm) bGG=true;
			  }
			  ibackG=sosedi[TSIDE][iP].iNODE1;
			  if (ibackG>=maxelm) bbackG=true;

			  if (!bG && !bGG && !bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x3=pointP.z;
				  center_cord3D(ibackG,nvtx,pa,pointP,TSIDE);
				  x4=pointP.z;
				  center_cord3D(iG,nvtx,pa,pointP,BSIDE);
				  x2=pointP.z;
				  center_cord3D(iGG,nvtx,pa,pointP,BB);
				  x1=pointP.z;
				  xg=x3-0.5*dz;
				  f4=potent[ibackG];
				  f3=potent[iP];
				  f2=potent[iG];
				  f1=potent[iGG];

				  DFDXgran=DFDXg(x1, x2, x3, x4, xg, f1, f2, f3, f4);
				   bborder=false;
			  }

			  if (!bG && bGG && !bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x3=pointP.z;
				  center_cord3D(ibackG,nvtx,pa,pointP,TSIDE);
				  x4=pointP.z;
				  center_cord3D(iG,nvtx,pa,pointP,BSIDE);
				  x2=pointP.z;
				  volume3D(iG, nvtx, pa, dx1, dy1, dz1);
				  x1=pointP.z-0.5*dz1;
				  xg=x3-0.5*dz;
				  f4=potent[ibackG];
				  f3=potent[iP];
				  f2=potent[iG];
				  f1=potent[iGG];

				  DFDXgran=DFDXg(x1, x2, x3, x4, xg, f1, f2, f3, f4);
				   bborder=true;
			  }

			  if (!bG && !bGG && bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x3=pointP.z;
				  volume3D(iP, nvtx, pa, dx1, dy1, dz1);
				  x4=x3+0.5*dz1;
				  center_cord3D(iG,nvtx,pa,pointP,BSIDE);
				  x2=pointP.z;
				  center_cord3D(iGG,nvtx,pa,pointP,BB);
				  x1=pointP.z;
				  xg=x3-0.5*dz;
				  f4=potent[ibackG];
				  f3=potent[iP];
				  f2=potent[iG];
				  f1=potent[iGG];

				  DFDXgran=DFDXg(x1, x2, x3, x4, xg, f1, f2, f3, f4);
				   bborder=true;
			  }

			  if (bG && !bbackG) {
				  center_cord3D(iP,nvtx,pa,pointP,100);
				  x2=pointP.z;
				  center_cord3D(ibackG,nvtx,pa,pointP,TSIDE);
				  x3=pointP.z;
				  xg=x1=x2-0.5*dz;
				  f3=potent[ibackG];
				  f2=potent[iP];
				  f1=potent[iG];

				  DFDXgran=DFDXg2(x1, x2, x3, xg, f1, f2, f3);
				   bborder=true;
			  }

		    break;


	} // end switch


	return DFDXgran;

} // DFDXiP

// ���� ��������� ������� ��� ��������
void my_elmatr_quad_F3D_bound(integer inumber, integer maxelm, 
							  bool bDirichlet, BOUND* sosedb, integer ls, integer lw,
							  WALL* w, integer iVar, equation3D_bon* &slb, doublereal dbeta,
							  TOCHKA* pa, integer** nvtx, doublereal** prop_b, doublereal** prop,
							  doublereal** potent, integer iflowregime
							  ) {

     // iVar - ���������� �������� ��� ������� ������� ��������� �������.

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
	/* 1. ������� �������. ��� ����������� ������. �� ������� ��������� � �� �������� ������� bpressure.
       2. ������� �������. ��� �������� ����� ��� ������ ������. 	
	*/


     // ������� ������� ��������� ������� �������
	//if (bDirichlet && (sosedb[inumber].MCB<(ls + lw)) && (sosedb[inumber].MCB >= ls) && (!w[sosedb[inumber].MCB - ls].bopening) && (!w[sosedb[inumber].MCB - ls].bsymmetry) && (!w[sosedb[inumber].MCB - ls].bpressure)) {
	if (bDirichlet && (sosedb[inumber].MCB<(ls + lw)) && (sosedb[inumber].MCB >= ls)  && (!w[sosedb[inumber].MCB - ls].bsymmetry) && (!w[sosedb[inumber].MCB - ls].bpressure)) {

		// ��������� ������� �������
		// ������ �������� �� �������
        // ��� �� ������� ��������� � �� �������� �������.

		slb[inumber].aw=1.0;
		slb[inumber].ai=0.0;

		if (w[sosedb[inumber].MCB - ls].bopening) {
			switch (sosedb[inumber].Norm) {
			case ESIDE:  
				switch (iVar) {
				case VX: slb[inumber].b = potent[VXCOR][maxelm+inumber]; break;
				case VY: slb[inumber].b = 0.0; break;
				case VZ: slb[inumber].b = 0.0; break;
				}
				break;
			case WSIDE: 
				switch (iVar) {
				case VX: slb[inumber].b = potent[VXCOR][maxelm + inumber]; break;
				case VY: slb[inumber].b = 0.0; break;
				case VZ: slb[inumber].b = 0.0; break;
				}
				break;
			case NSIDE:
				switch (iVar) {
				case VX: slb[inumber].b = 0.0; break;
				case VY: slb[inumber].b = potent[VYCOR][maxelm + inumber]; break;
				case VZ: slb[inumber].b = 0.0; break;
				}
				break;
			case SSIDE : 
				switch (iVar) {
				case VX: slb[inumber].b = 0.0; break;
				case VY: slb[inumber].b = potent[VYCOR][maxelm + inumber]; break;
				case VZ: slb[inumber].b = 0.0; break;
				}
				break;
			case TSIDE: 
				switch (iVar) {
				case VX: slb[inumber].b = 0.0; break;
				case VY: slb[inumber].b = 0.0; break;
				case VZ: slb[inumber].b = potent[VZCOR][maxelm + inumber];  break;
				}
				break;
			case BSIDE:
				switch (iVar) {
				case VX: slb[inumber].b =0.0; break;
				case VY: slb[inumber].b = 0.0; break;
				case VZ: slb[inumber].b = potent[VZCOR][maxelm + inumber]; break;
				}
				break;
			}
		}
		else {

			// ���� ������ ������������� ������� ������� ������,
			// ����� �� ������� ������� ������ ���������� ��������.
			// ��� ���� ������������� �������� � ������� ������� ����� ���� �� ����� ����,
			// � ����� �������� ��� ���������� �������� !.
			switch (iVar) {
			case VX: slb[inumber].b = w[sosedb[inumber].MCB - ls].Vx; break;
			case VY: slb[inumber].b = w[sosedb[inumber].MCB - ls].Vy; break;
			case VZ: slb[inumber].b = w[sosedb[inumber].MCB - ls].Vz; break;
			}
		}

		

		slb[inumber].iI=-1; // �� ������������ � �������
		slb[inumber].iW=sosedb[inumber].iB;
#if doubleintprecision == 1
		//printf("%lld, soseddb=%lld\n",inumber, sosedb[inumber].iB); getchar(); // debug
#else
		//printf("%d, soseddb=%d\n",inumber, sosedb[inumber].iB); getchar(); // debug
#endif
		

		// ��� ������� �������:
		// ������ ������������ ������� 
		// �� ����� ����.
		slb[inumber].iW1=-1;
        slb[inumber].iW2=-1;
        slb[inumber].iW3=-1;
        slb[inumber].iW4=-1;
	}
	else if (bDirichlet && ( (sosedb[inumber].MCB==(ls+lw)) ||(sosedb[inumber].MCB<ls)) ) { // 
		// �������� ���� �������� ������ �������.
		// ���� ������ ������. ������ ������ ����������� �� ������� (sosedb[inumber].MCB==(ls+lw)).

        // ��������� ������� �������
		// ������ ������� ���������� �� ������ ������.

		slb[inumber].aw=1.0;
		slb[inumber].ai=0.0;
		slb[inumber].b=0.0; // ������� ��������.
		slb[inumber].iI=-1; // �� ������������ � �������
		slb[inumber].iW=sosedb[inumber].iB;

		// ��� ������� �������:
		// ������ ������������ ������� 
		// �� ����� ����.
		slb[inumber].iW1=-1;
        slb[inumber].iW2=-1;
        slb[inumber].iW3=-1;
        slb[inumber].iW4=-1;
	}
	else if ((sosedb[inumber].MCB<(ls+lw)) && (sosedb[inumber].MCB>=ls) && (w[sosedb[inumber].MCB-ls].bsymmetry)) {
		// ������� ��������� �� �������:
		// ���� �� ������� ����� ��������� ������� � ��������� ���������, ��
		// ���������� �������� � ������� ����� ����, � ���������� �����������
		// � ������� �� ����������� ��������� �������� ���� ����� ����.

		// bNei - ����� �� ��� ������ ���������� �������� 
		// ���������� ������� ������� ?
		bool bNei=false;

		switch (iVar) {
			case VX : // ��������� ���������� ������� � �������
				      switch (sosedb[inumber].Norm) {
			             case ESIDE : bNei=false; break;
						 case WSIDE: bNei=false; break;
						 case NSIDE : bNei=true; break;
						 case SSIDE : bNei=true; break;
						 case TSIDE :  bNei=true; break;
						 case BSIDE : bNei=true; break;
					  }
				      break;
			case VY : switch (sosedb[inumber].Norm) {
			             case ESIDE : bNei=true; break;
						 case WSIDE: bNei=true; break;
						 case NSIDE : bNei=false; break;
						 case SSIDE : bNei=false; break;
						 case TSIDE :  bNei=true; break;
						 case BSIDE : bNei=true; break;
					  }
				      break;
			case VZ : switch (sosedb[inumber].Norm) {
			             case ESIDE : bNei=true; break;
						 case WSIDE: bNei=true; break;
						 case NSIDE : bNei=true; break;
						 case SSIDE : bNei=true; break;
						 case TSIDE :  bNei=false; break;
						 case BSIDE : bNei=false; break;
					  }
				      break;
		}

		if (bDirichlet && (!bNei)) {
			// ��������� ������� �������

		    slb[inumber].aw=1.0;
		    slb[inumber].ai=0.0;
		    slb[inumber].b=0.0;
		    slb[inumber].iI=-1; // �� ������������ � �������
		    slb[inumber].iW=sosedb[inumber].iB;

		    // ��� ������� �������:
		    // ������ ������������ ������� 
		    // �� ����� ����.
		    slb[inumber].iW1=-1;
            slb[inumber].iW2=-1;
            slb[inumber].iW3=-1;
            slb[inumber].iW4=-1;
		}
		else if ((!bDirichlet) && bNei) {
			// ���������� ������� �������:

			// ���������. �.�. ��������� ��� ���������� ��������
			// ��������� � ���������� ���������������� � ���������
			// �� ������� ������� � ������������� � ���������, ��
			// ��������� ������� ��� ��������� ����������������
			// ����������� �� ��������� ��� ���������� ��������, � ��������� 
			// �� ������������� � ���������.
            
			doublereal dl, deltal, dS; // �������������� ���������
	        doublereal mui; // ������������ �������� �� ����� ��
	        doublereal fiplus; // ���� ��������������� �����
			integer iVar_id=iVar;
			switch (iVar) {
				case VX : iVar_id=VXCOR; break;
				case VY : iVar_id=VYCOR; break;
				case VZ : iVar_id=VZCOR; break;
			}
			doublereal muB, muI, muII; // ������������ ��������

			// ���������� �������.
	        switch (sosedb[inumber].Norm) {
		       case ESIDE : 
			        dl = pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x;
                    //dS=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[1][sosedb[inumber].iI]-1].y; 
					//dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����
					dS = sosedb[inumber].dS; // ������� ����� 26.09.2016.

					// ������������ ������������ ��������
					muB=prop_b[MU][sosedb[inumber].iB-maxelm];
					muI=prop[MU][sosedb[inumber].iI];
					muII=prop[MU][sosedb[inumber].iII];
					// ��������� ������������ ������������ �������� ��������
					// �������� ����������.
					if (iflowregime==ZEROEQMOD) {
					   muB+=potent[MUT][sosedb[inumber].iB];
					   muI+=potent[MUT][sosedb[inumber].iI];
					   muII+=potent[MUT][sosedb[inumber].iII];
					}

					slb[inumber].ai=2.0*dbeta*muB*dS/dl;
					slb[inumber].iI=sosedb[inumber].iI;
					slb[inumber].aw=slb[inumber].ai;
					slb[inumber].iW=sosedb[inumber].iB;

                    deltal=0.5*(pa[nvtx[1][sosedb[inumber].iII]-1].x+pa[nvtx[0][sosedb[inumber].iII]-1].x);
					deltal-=0.5*(pa[nvtx[1][sosedb[inumber].iI]-1].x+pa[nvtx[0][sosedb[inumber].iI]-1].x);
                    fiplus=0.5*dl/deltal;

                    mui=(muI*muII)/((1.0-fiplus)*muI+fiplus*muII); // ������������ �������� ���������.

					// ������ �����
					slb[inumber].b=(dbeta-1.0)*mui*dS*(potent[iVar_id][sosedb[inumber].iI]-potent[iVar_id][sosedb[inumber].iII])/deltal;

					break;
				case NSIDE : 
			        dl = pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y;
                    //dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					//dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����
					dS = sosedb[inumber].dS; // ������� ����� 26.09.2016.


					// ������������ ������������ ��������
					muB=prop_b[MU][sosedb[inumber].iB-maxelm];
					muI=prop[MU][sosedb[inumber].iI];
					muII=prop[MU][sosedb[inumber].iII];
					// ��������� ������������ ������������ �������� ��������
					// �������� ����������.
					if (iflowregime==ZEROEQMOD) {
					    muB+=potent[MUT][sosedb[inumber].iB];
					    muI+=potent[MUT][sosedb[inumber].iI];
					    muII+=potent[MUT][sosedb[inumber].iII];
					}

					slb[inumber].ai=2.0*dbeta*muB*dS/dl;
					slb[inumber].iI=sosedb[inumber].iI;
					slb[inumber].aw=slb[inumber].ai;
					slb[inumber].iW=sosedb[inumber].iB;

                    deltal=0.5*(pa[nvtx[2][sosedb[inumber].iII]-1].y+pa[nvtx[0][sosedb[inumber].iII]-1].y);
					deltal-=0.5*(pa[nvtx[2][sosedb[inumber].iI]-1].y+pa[nvtx[0][sosedb[inumber].iI]-1].y);
                    fiplus=0.5*dl/deltal;

                    mui=(muI*muII)/((1.0-fiplus)*muI+fiplus*muII); // ������������ �������� ���������.

					// ������ �����
					slb[inumber].b=(dbeta-1.0)*mui*dS*(potent[iVar_id][sosedb[inumber].iI]-potent[iVar_id][sosedb[inumber].iII])/deltal;

					break;

			   case TSIDE :  
			        dl = pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z;
                    //dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					//dS*=(pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y); // ������� �����
					dS = sosedb[inumber].dS; // ������� ����� 26.09.2016.

					// ������������ ������������ ��������
					muB=prop_b[MU][sosedb[inumber].iB-maxelm];
					muI=prop[MU][sosedb[inumber].iI];
					muII=prop[MU][sosedb[inumber].iII];
					// ��������� ������������ ������������ �������� ��������
					// �������� ����������.
					if (iflowregime==ZEROEQMOD) {
					    muB+=potent[MUT][sosedb[inumber].iB];
					    muI+=potent[MUT][sosedb[inumber].iI];
					    muII+=potent[MUT][sosedb[inumber].iII];
					}

					slb[inumber].ai=2.0*dbeta*muB*dS/dl;
					slb[inumber].iI=sosedb[inumber].iI;
					slb[inumber].aw=slb[inumber].ai;
					slb[inumber].iW=sosedb[inumber].iB;

                    deltal=0.5*(pa[nvtx[4][sosedb[inumber].iII]-1].z+pa[nvtx[0][sosedb[inumber].iII]-1].z);
					deltal-=0.5*(pa[nvtx[4][sosedb[inumber].iI]-1].z+pa[nvtx[0][sosedb[inumber].iI]-1].z);
                    fiplus=0.5*dl/deltal;

                    mui=(muI*muII)/((1.0-fiplus)*muI+fiplus*muII); // ������������ �������� ���������.

					// ������ �����
					slb[inumber].b=(dbeta-1.0)*mui*dS*(potent[iVar_id][sosedb[inumber].iI]-potent[iVar_id][sosedb[inumber].iII])/deltal;


					break;

			case WSIDE: 
			        dl = pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x;
                    //dS=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[1][sosedb[inumber].iI]-1].y; 
					//dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����
					dS = sosedb[inumber].dS; // ������� ����� 26.09.2016.


					// ������������ ������������ ��������
					muB=prop_b[MU][sosedb[inumber].iB-maxelm];
					muI=prop[MU][sosedb[inumber].iI];
					muII=prop[MU][sosedb[inumber].iII];
					// ��������� ������������ ������������ �������� ��������
					// �������� ����������.
					if (iflowregime==ZEROEQMOD) {
					   muB+=potent[MUT][sosedb[inumber].iB];
					   muI+=potent[MUT][sosedb[inumber].iI];
					   muII+=potent[MUT][sosedb[inumber].iII];
					}

					slb[inumber].ai=2.0*dbeta*muB*dS/dl;
					slb[inumber].iI=sosedb[inumber].iI;
					slb[inumber].aw=slb[inumber].ai;
					slb[inumber].iW=sosedb[inumber].iB;

                    deltal=-0.5*(pa[nvtx[1][sosedb[inumber].iII]-1].x+pa[nvtx[0][sosedb[inumber].iII]-1].x);
					deltal+=0.5*(pa[nvtx[1][sosedb[inumber].iI]-1].x+pa[nvtx[0][sosedb[inumber].iI]-1].x);
                    fiplus=0.5*dl/deltal;

                    mui=(muI*muII)/((1.0-fiplus)*muI+fiplus*muII); // ������������ �������� ���������.

					// ������ �����
					// �������� !!! ���� ����� ���������� ������� ��� dbeta > 1 �� �������� ����� �������� ������� I � II!!!!
					slb[inumber].b=(dbeta-1.0)*mui*dS*(potent[iVar_id][sosedb[inumber].iI]-potent[iVar_id][sosedb[inumber].iII])/deltal;

					break;

           case SSIDE : 
			        dl = pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y;
                    //dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					//dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����
					dS = sosedb[inumber].dS; // ������� ����� 26.09.2016.


					// ������������ ������������ ��������
					muB=prop_b[MU][sosedb[inumber].iB-maxelm];
					muI=prop[MU][sosedb[inumber].iI];
					muII=prop[MU][sosedb[inumber].iII];
					// ��������� ������������ ������������ �������� ��������
					// �������� ����������.
					if (iflowregime==ZEROEQMOD) {
					    muB+=potent[MUT][sosedb[inumber].iB];
					    muI+=potent[MUT][sosedb[inumber].iI];
					    muII+=potent[MUT][sosedb[inumber].iII];
					}

					slb[inumber].ai=2.0*dbeta*muB*dS/dl;
					slb[inumber].iI=sosedb[inumber].iI;
					slb[inumber].aw=slb[inumber].ai;
					slb[inumber].iW=sosedb[inumber].iB;

                    deltal=-0.5*(pa[nvtx[2][sosedb[inumber].iII]-1].y+pa[nvtx[0][sosedb[inumber].iII]-1].y);
					deltal+=0.5*(pa[nvtx[2][sosedb[inumber].iI]-1].y+pa[nvtx[0][sosedb[inumber].iI]-1].y);
                    fiplus=0.5*dl/deltal;

                    mui=(muI*muII)/((1.0-fiplus)*muI+fiplus*muII); // ������������ �������� ���������.

					// ������ �����
					slb[inumber].b=(dbeta-1.0)*mui*dS*(potent[iVar_id][sosedb[inumber].iI]-potent[iVar_id][sosedb[inumber].iII])/deltal;

					break;

			case BSIDE : 
			        dl = pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z;
                    //dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					//dS*=(pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y); // ������� �����
					dS = sosedb[inumber].dS; // ������� ����� 26.09.2016.


					// ������������ ������������ ��������
					muB=prop_b[MU][sosedb[inumber].iB-maxelm];
					muI=prop[MU][sosedb[inumber].iI];
					muII=prop[MU][sosedb[inumber].iII];
					// ��������� ������������ ������������ �������� ��������
					// �������� ����������.
					if (iflowregime==ZEROEQMOD) {
					    muB+=potent[MUT][sosedb[inumber].iB];
					    muI+=potent[MUT][sosedb[inumber].iI];
				    	muII+=potent[MUT][sosedb[inumber].iII];
					}

					slb[inumber].ai=2.0*dbeta*muB*dS/dl;
					slb[inumber].iI=sosedb[inumber].iI;
					slb[inumber].aw=slb[inumber].ai;
					slb[inumber].iW=sosedb[inumber].iB;

                    deltal=-0.5*(pa[nvtx[4][sosedb[inumber].iII]-1].z+pa[nvtx[0][sosedb[inumber].iII]-1].z);
					deltal+=0.5*(pa[nvtx[4][sosedb[inumber].iI]-1].z+pa[nvtx[0][sosedb[inumber].iI]-1].z);
                    fiplus=0.5*dl/deltal;

                    mui=(muI*muII)/((1.0-fiplus)*muI+fiplus*muII); // ������������ �������� ���������.

					// ������ �����
					slb[inumber].b=(dbeta-1.0)*mui*dS*(potent[iVar_id][sosedb[inumber].iI]-potent[iVar_id][sosedb[inumber].iII])/deltal;

					break;
	        } // end switch

            integer j,l,xitem,k;
	        // ���������� �� �����������
	        for (j=0; j<5; j++) {
	 		    k=j; xitem=sosedb[inumber].iW[j];
			    for (l=j+1; l<6; l++) {
				    if (sosedb[inumber].iW[l] < xitem) {
					    k=l; xitem=sosedb[inumber].iW[k];
				    }
			    }
                sosedb[inumber].iW[k]=sosedb[inumber].iW[j];
			    sosedb[inumber].iW[j]=xitem;
		    }

            j=0; l=0;
		    while (sosedb[inumber].iW[j]==(-1)) j++;

		    if (j<6) { slb[inumber].iW1=sosedb[inumber].iW[j++]; l++; }
		    if (j<6) { slb[inumber].iW2=sosedb[inumber].iW[j++]; l++; }
		    if (j<6) { slb[inumber].iW3=sosedb[inumber].iW[j++]; l++; }
		    if (j<6) { slb[inumber].iW4=sosedb[inumber].iW[j++]; l++; } 

		    switch (l) {
			   case 0 : slb[inumber].iW1=-1;
		                slb[inumber].iW2=-1;
		                slb[inumber].iW3=-1;
		                slb[inumber].iW4=-1;
		                break;
			   case 1 : slb[inumber].iW2=-1;
		                slb[inumber].iW3=-1;
		                slb[inumber].iW4=-1;
		    		    break;
			   case 2 : slb[inumber].iW3=-1;
		                slb[inumber].iW4=-1;
					    break;
			   case 3 : slb[inumber].iW4=-1;
					    break;
		    }

		    // ��������� ���������� � ������ ���������� ���� iI � ������������ 
		    // ��������� ���� ��� ��������� ����� ������� ������� �� ���������. 
		    // �.�. ���� iI ������ ���������� ��� ������� iI < maxelm, � ������������ 
		    // ������� � �������� ������� ����� � �������� >= maxelm ������� ��� ��
		    // ������������.
            
		}

	}
	else if ((sosedb[inumber].MCB<(ls + lw)) && (sosedb[inumber].MCB >= ls) && ((w[sosedb[inumber].MCB - ls].bpressure) /*|| ((w[sosedb[inumber].MCB - ls].bopening))*/)) {
		    
		    bool bmassbal=false; // ���� true �� ������������ �������� ������ ������������ �� ����� ��������� ������.

		    if ((!bDirichlet)&&(!bmassbal)) {
		    // ��� ����� ���������� �������� ����� ������� �������:
            // ���������. �.�. ��������� ��� ���������� ��������
			// ��������� � ���������� ���������������� � ���������
			// �� ������� ������� � ������������� � ���������, ��
			// ��������� ������� ��� ��������� ����������������
			// ����������� �� ��������� ��� ���������� ��������, � ��������� 
			// �� ������������� � ���������.
            
			doublereal dl, deltal, dS; // �������������� ���������
	        doublereal mui; // ������������ �������� �� ����� ��
	        doublereal fiplus; // ���� ��������������� �����

			integer iVar_id=iVar;
			switch (iVar) {
				case VX : iVar_id=VXCOR; break;
				case VY : iVar_id=VYCOR; break;
				case VZ : iVar_id=VZCOR; break;
			}

			doublereal muB, muI, muII; // ������������ ��������

	        switch (sosedb[inumber].Norm) {
		       case ESIDE : 
			        dl = pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x;
                    //dS=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[1][sosedb[inumber].iI]-1].y; 
					//dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����
					dS = sosedb[inumber].dS; // ������� ����� 26.09.2016.


					// ������������ ������������ ��������
					muB=prop_b[MU][sosedb[inumber].iB-maxelm];
					muI=prop[MU][sosedb[inumber].iI];
					muII=prop[MU][sosedb[inumber].iII];
					// ��������� ������������ ������������ �������� ��������
					// �������� ����������.
					if (iflowregime==ZEROEQMOD) {
					    muB+=potent[MUT][sosedb[inumber].iB];
						muI+=potent[MUT][sosedb[inumber].iI];
						muII+=potent[MUT][sosedb[inumber].iII];
					}

					slb[inumber].ai=2.0*dbeta*muB*dS/dl;
					slb[inumber].iI=sosedb[inumber].iI;
					slb[inumber].aw=slb[inumber].ai;
					slb[inumber].iW=sosedb[inumber].iB;

                    deltal=0.5*(pa[nvtx[1][sosedb[inumber].iII]-1].x+pa[nvtx[0][sosedb[inumber].iII]-1].x);
					deltal-=0.5*(pa[nvtx[1][sosedb[inumber].iI]-1].x+pa[nvtx[0][sosedb[inumber].iI]-1].x);
                    fiplus=0.5*dl/deltal;

                    mui=(muI*muII)/((1.0-fiplus)*muI+fiplus*muII); // ������������ �������� ���������.

					// ������ �����
					slb[inumber].b=(dbeta-1.0)*mui*dS*(potent[iVar_id][sosedb[inumber].iI]-potent[iVar_id][sosedb[inumber].iII])/deltal;

					break;
				case NSIDE : 
			        dl = pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y;
                    //dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					//dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����
					dS = sosedb[inumber].dS; // ������� ����� 26.09.2016.


					// ������������ ������������ ��������
					muB=prop_b[MU][sosedb[inumber].iB-maxelm];
					muI=prop[MU][sosedb[inumber].iI];
					muII=prop[MU][sosedb[inumber].iII];
					// ��������� ������������ ������������ �������� ��������
					// �������� ����������.
					if (iflowregime==ZEROEQMOD) {
						muB+=potent[MUT][sosedb[inumber].iB];
						muI+=potent[MUT][sosedb[inumber].iI];
						muII+=potent[MUT][sosedb[inumber].iII];
					}

					slb[inumber].ai=2.0*dbeta*muB*dS/dl;
					slb[inumber].iI=sosedb[inumber].iI;
					slb[inumber].aw=slb[inumber].ai;
					slb[inumber].iW=sosedb[inumber].iB;

                    deltal=0.5*(pa[nvtx[2][sosedb[inumber].iII]-1].y+pa[nvtx[0][sosedb[inumber].iII]-1].y);
					deltal-=0.5*(pa[nvtx[2][sosedb[inumber].iI]-1].y+pa[nvtx[0][sosedb[inumber].iI]-1].y);
                    fiplus=0.5*dl/deltal;

                   	mui=(muI*muII)/((1.0-fiplus)*muI+fiplus*muII); // ������������ ��������

					// ������ �����
					slb[inumber].b=(dbeta-1.0)*mui*dS*(potent[iVar_id][sosedb[inumber].iI]-potent[iVar_id][sosedb[inumber].iII])/deltal;

					break;

			   case TSIDE :  
			        dl = pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z;
                    //dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					//dS*=(pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y); // ������� �����
					dS = sosedb[inumber].dS; // ������� ����� 26.09.2016.


					// ������������ ������������ ��������
					muB=prop_b[MU][sosedb[inumber].iB-maxelm];
					muI=prop[MU][sosedb[inumber].iI];
					muII=prop[MU][sosedb[inumber].iII];
					// ��������� ������������ ������������ �������� ��������
					// �������� ����������.
					if (iflowregime==ZEROEQMOD) {
						muB+=potent[MUT][sosedb[inumber].iB];
						muI+=potent[MUT][sosedb[inumber].iI];
						muII+=potent[MUT][sosedb[inumber].iII];
					}

					slb[inumber].ai=2.0*dbeta*muB*dS/dl;
					slb[inumber].iI=sosedb[inumber].iI;
					slb[inumber].aw=slb[inumber].ai;
					slb[inumber].iW=sosedb[inumber].iB;

                    deltal=0.5*(pa[nvtx[4][sosedb[inumber].iII]-1].z+pa[nvtx[0][sosedb[inumber].iII]-1].z);
					deltal-=0.5*(pa[nvtx[4][sosedb[inumber].iI]-1].z+pa[nvtx[0][sosedb[inumber].iI]-1].z);
                    fiplus=0.5*dl/deltal;

                    mui=(muI*muII)/((1.0-fiplus)*muI+fiplus*muII); // ������������ �������� ���������.

					// ������ �����
					slb[inumber].b=(dbeta-1.0)*mui*dS*(potent[iVar_id][sosedb[inumber].iI]-potent[iVar_id][sosedb[inumber].iII])/deltal;

					break;

			case WSIDE: 
			        dl = pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x;
                    //dS=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[1][sosedb[inumber].iI]-1].y; 
					//dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����
					dS = sosedb[inumber].dS; // ������� ����� 26.09.2016.

					// ������������ ������������ ��������
					muB=prop_b[MU][sosedb[inumber].iB-maxelm];
					muI=prop[MU][sosedb[inumber].iI];
					muII=prop[MU][sosedb[inumber].iII];
					// ��������� ������������ ������������ �������� ��������
					// �������� ����������.
					if (iflowregime==ZEROEQMOD) {
						muB+=potent[MUT][sosedb[inumber].iB];
						muI+=potent[MUT][sosedb[inumber].iI];
						muII+=potent[MUT][sosedb[inumber].iII];
					}

					slb[inumber].ai=2.0*dbeta*muB*dS/dl;
					slb[inumber].iI=sosedb[inumber].iI;
					slb[inumber].aw=slb[inumber].ai;
					slb[inumber].iW=sosedb[inumber].iB;

                    deltal=-0.5*(pa[nvtx[1][sosedb[inumber].iII]-1].x+pa[nvtx[0][sosedb[inumber].iII]-1].x);
					deltal+=0.5*(pa[nvtx[1][sosedb[inumber].iI]-1].x+pa[nvtx[0][sosedb[inumber].iI]-1].x);
                    fiplus=0.5*dl/deltal;

                    mui=(muI*muII)/((1.0-fiplus)*muI+fiplus*muII); // ������������ �������� ���������.

					// ������ �����
					// �������� !!! ���� ����� ���������� ������� ��� dbeta > 1 �� �������� ����� �������� ������� I � II!!!!
					slb[inumber].b=(dbeta-1.0)*mui*dS*(potent[iVar_id][sosedb[inumber].iI]-potent[iVar_id][sosedb[inumber].iII])/deltal;

					break;

           case SSIDE : 
			        dl = pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y;
                    //dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					//dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����
					dS = sosedb[inumber].dS; // ������� ����� 26.09.2016.


					// ������������ ������������ ��������
					muB=prop_b[MU][sosedb[inumber].iB-maxelm];
					muI=prop[MU][sosedb[inumber].iI];
					muII=prop[MU][sosedb[inumber].iII];
					// ��������� ������������ ������������ �������� ��������
					// �������� ����������.
					if (iflowregime==ZEROEQMOD) {
						muB+=potent[MUT][sosedb[inumber].iB];
						muI+=potent[MUT][sosedb[inumber].iI];
						muII+=potent[MUT][sosedb[inumber].iII];
					}

					slb[inumber].ai=2.0*dbeta*muB*dS/dl;
					slb[inumber].iI=sosedb[inumber].iI;
					slb[inumber].aw=slb[inumber].ai;
					slb[inumber].iW=sosedb[inumber].iB;

                    deltal=-0.5*(pa[nvtx[2][sosedb[inumber].iII]-1].y+pa[nvtx[0][sosedb[inumber].iII]-1].y);
					deltal+=0.5*(pa[nvtx[2][sosedb[inumber].iI]-1].y+pa[nvtx[0][sosedb[inumber].iI]-1].y);
                    fiplus=0.5*dl/deltal;

                   	mui=(muI*muII)/((1.0-fiplus)*muI+fiplus*muII); // ������������ �������� ���������.

					// ������ �����
					slb[inumber].b=(dbeta-1.0)*mui*dS*(potent[iVar_id][sosedb[inumber].iI]-potent[iVar_id][sosedb[inumber].iII])/deltal;

					break;

			case BSIDE : 
			        dl = pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z;
                    //dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					//dS*=(pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y); // ������� �����
					dS = sosedb[inumber].dS; // ������� ����� 26.09.2016.


					// ������������ ������������ ��������
					muB=prop_b[MU][sosedb[inumber].iB-maxelm];
					muI=prop[MU][sosedb[inumber].iI];
					muII=prop[MU][sosedb[inumber].iII];
					// ��������� ������������ ������������ �������� ��������
					// �������� ����������.
					if (iflowregime==ZEROEQMOD) {
						muB+=potent[MUT][sosedb[inumber].iB];
						muI+=potent[MUT][sosedb[inumber].iI];
						muII+=potent[MUT][sosedb[inumber].iII];
					}

					slb[inumber].ai=2.0*dbeta*muB*dS/dl;
					slb[inumber].iI=sosedb[inumber].iI;
					slb[inumber].aw=slb[inumber].ai;
					slb[inumber].iW=sosedb[inumber].iB;

                    deltal=-0.5*(pa[nvtx[4][sosedb[inumber].iII]-1].z+pa[nvtx[0][sosedb[inumber].iII]-1].z);
					deltal+=0.5*(pa[nvtx[4][sosedb[inumber].iI]-1].z+pa[nvtx[0][sosedb[inumber].iI]-1].z);
                    fiplus=0.5*dl/deltal;

                    mui=(muI*muII)/((1.0-fiplus)*muI+fiplus*muII); // ������������ �������� ���������.

					// ������ �����
					slb[inumber].b=(dbeta-1.0)*mui*dS*(potent[iVar_id][sosedb[inumber].iI]-potent[iVar_id][sosedb[inumber].iII])/deltal;

					break;
	        } // end switch

            integer j,l,xitem,k;
	        // ���������� �� �����������
	        for (j=0; j<5; j++) {
	 		    k=j; xitem=sosedb[inumber].iW[j];
			    for (l=j+1; l<6; l++) {
				    if (sosedb[inumber].iW[l] < xitem) {
					    k=l; xitem=sosedb[inumber].iW[k];
				    }
			    }
                sosedb[inumber].iW[k]=sosedb[inumber].iW[j];
			    sosedb[inumber].iW[j]=xitem;
		    }

            j=0; l=0;
		    while (sosedb[inumber].iW[j]==(-1)) j++;

		    if (j<6) { slb[inumber].iW1=sosedb[inumber].iW[j++]; l++; }
		    if (j<6) { slb[inumber].iW2=sosedb[inumber].iW[j++]; l++; }
		    if (j<6) { slb[inumber].iW3=sosedb[inumber].iW[j++]; l++; }
		    if (j<6) { slb[inumber].iW4=sosedb[inumber].iW[j++]; l++; } 

		    switch (l) {
			   case 0 : slb[inumber].iW1=-1;
		                slb[inumber].iW2=-1;
		                slb[inumber].iW3=-1;
		                slb[inumber].iW4=-1;
		                break;
			   case 1 : slb[inumber].iW2=-1;
		                slb[inumber].iW3=-1;
		                slb[inumber].iW4=-1;
		    		    break;
			   case 2 : slb[inumber].iW3=-1;
		                slb[inumber].iW4=-1;
					    break;
			   case 3 : slb[inumber].iW4=-1;
					    break;
		    }

		    // ��������� ���������� � ������ ���������� ���� iI � ������������ 
		    // ��������� ���� ��� ��������� ����� ������� ������� �� ���������. 
		    // �.�. ���� iI ������ ���������� ��� ������� iI < maxelm, � ������������ 
		    // ������� � �������� ������� ����� � �������� >= maxelm ������� ��� ��
		    // ������������.
			}

			if ((bDirichlet)&&(bmassbal)) {
				// �������� ������ ���������� ��������� �� �������� ������� ������� �� �������� �������,
				// ��� ����� �������� ������ �� �������� ������� ��� ����� ���������.

                // ��������� ������� �������
		        // ������ ������� ��������� �������� ��������������� ��������� �������������.

				

		        slb[inumber].aw=1.0;
		        slb[inumber].ai=0.0;

				// ��������� ���������� �������.
				switch (sosedb[inumber].Norm) {
				  case ESIDE : switch (iVar) {
				             case VX :  slb[inumber].b=potent[VXCOR][sosedb[inumber].iB]; break; //<--
							 case VY : slb[inumber].b=potent[VYCOR][sosedb[inumber].iB]; break; // ��������� ����������� �������� �� �������� �������.
							 case VZ : slb[inumber].b=potent[VZCOR][sosedb[inumber].iB]; break;
				             default : slb[inumber].b=0.0; // ��������� ����������� �������� � �������� �������.

								       break;	         
						   }
						   break;
				  case WSIDE: switch (iVar) {
				             case VX :  slb[inumber].b=potent[VXCOR][sosedb[inumber].iB]; break; // <--
                             case VY : slb[inumber].b=potent[VYCOR][sosedb[inumber].iB]; break;
							 case VZ : slb[inumber].b=potent[VZCOR][sosedb[inumber].iB]; break;
				             default : slb[inumber].b=0.0; 
								 break;	         
						   }
					       break;
				  case NSIDE : switch (iVar) {
					         case VX :  slb[inumber].b=potent[VXCOR][sosedb[inumber].iB]; break;
				             case VY :  slb[inumber].b=potent[VYCOR][sosedb[inumber].iB]; break; // <--
                             case VZ : slb[inumber].b=potent[VZCOR][sosedb[inumber].iB]; break;
				             default : slb[inumber].b=0.0; break;	         
						   }
					       break;
				  case SSIDE : switch (iVar) {
					         case VX :  slb[inumber].b=potent[VXCOR][sosedb[inumber].iB]; break;
				             case VY :  slb[inumber].b=potent[VYCOR][sosedb[inumber].iB]; break; //<--
                             case VZ : slb[inumber].b=potent[VZCOR][sosedb[inumber].iB]; break;
				             default : slb[inumber].b=0.0; break;	         
						   }
						   /*
						   printf("mass ball S ok.\n");
						   printf("vell==%e\n",slb[inumber].b);
				                       getchar();
									   */
					       break;
				  case TSIDE :  switch (iVar) {
					         case VX :  slb[inumber].b=potent[VXCOR][sosedb[inumber].iB]; break;
				             case VY :  slb[inumber].b=potent[VYCOR][sosedb[inumber].iB]; break;
				             case VZ :  slb[inumber].b=potent[VZCOR][sosedb[inumber].iB]; break; // <--
				             default : slb[inumber].b=0.0;
								      break;	         
						   }
					       break;
				  case BSIDE : switch (iVar) {
					         case VX :  slb[inumber].b=potent[VXCOR][sosedb[inumber].iB]; break;
				             case VY :  slb[inumber].b=potent[VYCOR][sosedb[inumber].iB]; break;
				             case VZ :  slb[inumber].b=potent[VZCOR][sosedb[inumber].iB]; break; // <--
				             default : slb[inumber].b=0.0;
								      break;	         
						   }
					       break;
				}
		        
		        slb[inumber].iI=-1; // �� ������������ � �������
		        slb[inumber].iW=sosedb[inumber].iB;

		        // ��� ������� �������:
		        // ������ ������������ ������� 
		        // �� ����� ����.
		        slb[inumber].iW1=-1;
                slb[inumber].iW2=-1;
                slb[inumber].iW3=-1;
                slb[inumber].iW4=-1;
			}
	}

} // my_elmatr_quad_F3D_bound

// �������� ���� ��������� ������� ���� ��� ����������� ��������� 
// ��������� - ��������, ��� ������������ ����������� ������������ ������.
// ��� ������������� ��������� ������������ Hex �����.
// ��� ������ ����������� ������ ��� ��������� ��������.
void my_elmatr_quad_F3D(integer iP, BOUND* sosedb, integer lw, integer ls, equation3D** &sl, equation3D_bon** &slb, doublereal** diag_coef,
						integer iVar, bool btimedep, doublereal tauparam, integer* ptr,
					    integer** nvtx, doublereal** potent, doublereal* potent_temper,
						TOCHKA* pa, doublereal** prop, doublereal** prop_b, integer maxelm, 
						ALICE_PARTITION** sosedi, doublereal* alpha, doublereal dgx, doublereal dgy, doublereal dgz,
						doublereal dbeta, integer ishconvection, bool bBussineskApproach, doublereal temp_ref,
						bool bfirst_start, doublereal RCh, integer iflowregime,
						doublereal* speedoldtimestep, doublereal* toldtimestep, 
						BLOCK* b, integer lb, TPROP* matlist, doublereal* mf, bool bVERYStable, doublereal &sumanb) {
	// 31 ����� 2012 ���� �� ������� ����� ������� ����������������� �������� ���� ��������� � ��������� ���-���.
	// ��� ����� ����� � ���������� backup`�� ��������� AliceFlow_v0_07 (� ����� v0_06).

	// speedoldtimestep - ���������� ��������, ��������������� iVar, � ����������� ���� �� �������,
	// toldtimestep - ����������� � ����������� ���� �� �������.
	// mf - ����������� �������� ����� � ���������� �������� ��������������� �������� ���-���.

	// ���� bVERYStable ����� true �� �� �� ���������� ����� ���������� ��������� � ���������� ������ ��������������� ����� ������� �������.
	// ����� ���� bVERYStable==true �� ����������� ����� ����� � ������ ����������� ���������� �������� ���������� ������ �������.

    // btimedep==false - ������������, ����� (true) ��������������
	// tauparam - ��� �� �������.
	// ���������� ��� ����� ���� ap0 �������� ������ ���� � ������ ������ ���������� ������� �������.
    // ������ ���������� ��� ������� ������� ������� !
	// ��������� ������ ���������� ��������� ��������.
	// ���� ����� ���� ������� ������ ���������� ������� ������� (1 �� �������) ��� � ���������, ����������� 
	// ������������ ����������� ������� ap->ap/alpha � ������ ����� b->b+ap*Vn-1*(1-alpha)/alpha, 
	// ���� ���������� �� ������� �������� � �������������� ������ ��������� � ��������� ������ �������
	// tau=rho*dx*dy*dz*alpha/ap/(1.0-alpha), apzero=rho*dx*dy*dz/tau;  ������� ��� apzero==0.0 ��� alpha==1.0 ��� ������������� �������� ������.
	// ���� ���� ������ ������� ���� ������ � ����� �����.
	// ���� ����� ��������� imitation_time=true �� ����� ��������� ������ ���������� ��� ������ ������� � � ������������ ������.
	// ������� �������������� ����� ���������� �������� ������������ ������������� ��������� imitation_time.
	// ������ �������� ������ sigma flow.
	// �������� 1 � 2 ��� ��� ������������� ������������� ����� ������ ������ � ������. ������� ����� ������������ ���� 1 ���� 2, ��
	// �� � ���� ������ �� ��� ������� �����.
    const bool imitation_time=false; // false - ������ ����� �� ������������, true - ������������
	/*
	   ������������ ��� ����������, ������� �� ������� ������ ����������, ����� ����������
	   ��� � ��������� ��� �������� ������������� ���-��� ������� �� �����. �.�. ��������� ���
	   �������� ������ ���������� ������� ����� �� ����� � ������, � �������� ���-��� ��������
	   ������ � ��������� ��� �������� ��������.
	   ������� ��������� ��� ��� ����������� � ������������� �������� � ��� ��� ��������� ���� �� 
	   �������� ������� ��� ������� ���������.

	   ����������� �� ����� ��������� ������ (�������� sigma flow) �������� ���-���
	   ��������� ������� � � ��������� ��� �������� � ����� ��� �����������. ����� 
	   ����� �� ����� ������������� ��������� �������������.

	   ����������� ���������� ������� � ��� ��� �� �������� ��� ��������� 
	   �� ���������������� �������� ���-��� ��������� �� �������. Ÿ ���������� �
	   ��������� �� �������� ������������ �������. ������� ��������� �������� 
	   bRhieChowi=true;
       bRhieChowb=false;

	   �� ������ ���������� ������ �������� ���-���. ���� �� ����������� ��������� ������ � ����������
	   RCh ����� ������ ����� ������ 1.0. �� �������� ��� �������� (1.0) ����� ������� � �����������
	   ������������� ��������� �������������.
	*/

    // iP - ����� ����������� ������������ ������
	// iP ���������� �� 0 �� maxelm-1.
	integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	iE=sosedi[ESIDE][iP].iNODE1; iN=sosedi[NSIDE][iP].iNODE1; iT=sosedi[TSIDE][iP].iNODE1; iW=sosedi[WSIDE][iP].iNODE1; iS=sosedi[SSIDE][iP].iNODE1; iB=sosedi[BSIDE][iP].iNODE1;
	sl[iVar][iP].iE=iE; sl[iVar][iP].iN=iN; sl[iVar][iP].iT=iT; sl[iVar][iP].iP=iP; sl[iVar][iP].iS=iS; sl[iVar][iP].iW=iW; sl[iVar][iP].iB=iB;
	
	
	// ���������� ��.	

	// ���� � ����� �� ������ ����� ������� ��������� �������
	// �� ��������������� ���������� ����� true
	bool bE=false, bN=false, bT=false, bW=false, bS=false, bB=false;
    
	if (iE>=maxelm) bE=true;
	if (iN>=maxelm) bN=true;
	if (iT>=maxelm) bT=true;
    if (iW>=maxelm) bW=true;
	if (iS>=maxelm) bS=true;
	if (iB>=maxelm) bB=true;
		
	// ���������� �������� �������� ������������ ������:
	doublereal dx=0.0, dy=0.0, dz=0.0;// ����� �������� ������������ ������
	volume3D(iP, nvtx, pa, dx, dy, dz);
	   
	//printf("%.2f %.2f\n",dx,dy); // debug GOOD
	//getchar();

	doublereal dxe=0.5*dx, dxw=0.5*dx, dyn=0.5*dy, dys=0.5*dy, dzt=0.5*dz, dzb=0.5*dz;
    // �.�. �������� ��������� ������ ����, �� ����� ��� ������������
	// x - direction
    if (!bE) dxe=0.5*(pa[nvtx[1][iE]-1].x+pa[nvtx[0][iE]-1].x);
	if (!bE) dxe-=0.5*(pa[nvtx[1][iP]-1].x+pa[nvtx[0][iP]-1].x);
	if (!bW) dxw=0.5*(pa[nvtx[1][iP]-1].x+pa[nvtx[0][iP]-1].x);
	if (!bW) dxw-=0.5*(pa[nvtx[1][iW]-1].x+pa[nvtx[0][iW]-1].x);
    // y - direction
	if (!bN) dyn=0.5*(pa[nvtx[2][iN]-1].y+pa[nvtx[0][iN]-1].y);
	if (!bN) dyn-=0.5*(pa[nvtx[2][iP]-1].y+pa[nvtx[0][iP]-1].y);
	if (!bS) dys=0.5*(pa[nvtx[2][iP]-1].y+pa[nvtx[0][iP]-1].y);
	if (!bS) dys-=0.5*(pa[nvtx[2][iS]-1].y+pa[nvtx[0][iS]-1].y);
    // z - direction
	if (!bT) dzt=0.5*(pa[nvtx[4][iT]-1].z+pa[nvtx[0][iT]-1].z);
	if (!bT) dzt-=0.5*(pa[nvtx[4][iP]-1].z+pa[nvtx[0][iP]-1].z);
	if (!bB) dzb=0.5*(pa[nvtx[4][iP]-1].z+pa[nvtx[0][iP]-1].z);
	if (!bB) dzb-=0.5*(pa[nvtx[4][iB]-1].z+pa[nvtx[0][iB]-1].z);


	// ���� ��������������� ��������� �����:
	doublereal feplus, fwplus, fnplus, fsplus, ftplus, fbplus;
	// x-direction
	feplus=0.5*dx/dxe;
	fwplus=0.5*dx/dxw;
	// y-direction
	fnplus=0.5*dy/dyn;
	fsplus=0.5*dy/dys;
	// z-direction
	ftplus=0.5*dz/dzt;
	fbplus=0.5*dz/dzb;

	//printf("%e %e %e %e %e %e\n",feplus, fwplus, fnplus, fsplus, ftplus, fbplus);
	//getchar();

	// ��������� �� ����� �� ���������������� ������� �������������
	doublereal rhoe, rhow, rhon, rhos, rhot, rhob;
	doublereal rP, rE, rN, rT, rW, rS, rB;
    rP=prop[RHO][iP];
	if (!bE) rE=prop[RHO][iE]; else rE=prop_b[RHO][iE-maxelm];
    if (!bN) rN=prop[RHO][iN]; else rN=prop_b[RHO][iN-maxelm];
    if (!bT) rT=prop[RHO][iT]; else rT=prop_b[RHO][iT-maxelm];
	if (!bW) rW=prop[RHO][iW]; else rW=prop_b[RHO][iW-maxelm];
    if (!bS) rS=prop[RHO][iS]; else rS=prop_b[RHO][iS-maxelm];
	if (!bB) rB=prop[RHO][iB]; else rB=prop_b[RHO][iB-maxelm];
	    
	rhoe=rE*rP/(feplus*rE+(1.0-feplus)*rP);  // ���������.
	rhow=rW*rP/(fwplus*rW+(1.0-fwplus)*rP);
	rhon=rN*rP/(fnplus*rN+(1.0-fnplus)*rP);
	rhos=rS*rP/(fsplus*rS+(1.0-fsplus)*rP);
    rhot=rT*rP/(ftplus*rT+(1.0-ftplus)*rP);
	rhob=rB*rP/(fbplus*rB+(1.0-fbplus)*rP);

	
	/*
	   ����������� ����������:
	   �� �������� ������������� ���-��� �������� �� ����� ��
	   ������ ���� ��������� � � ���������� ��� ��������� ��������. 
	   ��� ������������� ������ ���� ��������� ��� ���������� ������� �� ����� ������������ ������
	   � ���������� ���������� ��������, ���������������� � ������������ ������������� �.�. � ���������� 
	   ����������� ��� ������ ����� ������������� ��������� �������������. ���� ��� ���������� ������� �� 
	   ��������� ������������ ���-��� �� ���� ��������� ���������� � �� ��������� (���� ��� � ������������ 
	   ������ � ��������� ���������� �������� � ����������������. ��� �������� �������� ��������� �����������
	   ������� ����� ��������� ��������� ����������) �� ������ ����� �� ����� ������������� ��������� �������������
	   � ������������� ������� ����� ��������.
	   ������������ ���������� ������������� �������� �� ��� ��� ������������, �
	   �� ����������� ������ ���. ��� ����������� ����� ��������� ������������� �������� ���� ��� �� ������
	   ���������������� �������� � ��������. ��� ���������� ��������� ������������ ������������ �
	   ���������� ��� ��������� ��������. ��� ������� � ������� �������� ��������� 
	   SIMPLE (�� ������ ����������������� ���������� ������� ������������ ������� � ������� ��������, ��
	   ���� � ��� ��� ����� �� �� ��������� �������� ���������� ����������� ����� ������ ����� (������� ���� ��������� � ������)
	   � ������� ������� ��� ������������ ������������ �������� � ���������� ��������). 
	   ��������� ������������� ��������... TODO
	   �������� ������ �������������� ������ ��������, �.�. �� ��� ������������ ������������
	   ��� ���� ����� ��� �� ���������. ������� ������������ �������� ������������� ���-��� ������
	   �� ������ �������� ��������� SIMPLE. �� ������ �������� ������������� ������� ������������
	   �������� ����� ���������� ������� �������� ������������� ��������.
	*/

    
	// ������������ ����� ����� ����� ��.
    // � ���������� �������� � ������ ���������������� ��������������� 
    // ���������� ���������������� �������� ���-���.
	doublereal Fe=0.0, Fw=0.0, Fn=0.0, Fs=0.0, Ft=0.0, Fb=0.0;
	// �������� ����� ������������ � �� �����������, 
	// ��� ������ ����������� ��������� �� �������� ������ ������� ����.
	Fe=mf[ESIDE];
	Fn=mf[NSIDE];
	Ft=mf[TSIDE];
	Fw=mf[WSIDE];
	Fs=mf[SSIDE];
	Fb=mf[BSIDE];


	doublereal eps=1e-37; // ��� ��������� ������������� ����.

	
	// �������� � �������� ����� ����������� ��� ���������� ������������� �����:
    doublereal kP, kW, kE, kS, kN, kB, kT;

    kP=potent[PRESS][iP];
    kW=potent[PRESS][iW]; // ������� ��������� ���� ��� �������������
    kE=potent[PRESS][iE];    
    kS=potent[PRESS][iS];
    kN=potent[PRESS][iN];    
	kB=potent[PRESS][iB];	
    kT=potent[PRESS][iT];


	// ������������ ��������:
	doublereal GP, GE, GW, GN, GS, GT, GB;
	doublereal Ge, Gw, Gn, Gs, Gt, Gb;
	// ���������� ������������ ��������:
    GP=prop[MU][iP]; // � ������ ����������� ��.
	if (!bE) GE=prop[MU][iE]; else GE=prop_b[MU][iE-maxelm];
    if (!bN) GN=prop[MU][iN]; else GN=prop_b[MU][iN-maxelm];
	if (!bT) GT=prop[MU][iT]; else GT=prop_b[MU][iT-maxelm];
	if (!bW) GW=prop[MU][iW]; else GW=prop_b[MU][iW-maxelm];
    if (!bS) GS=prop[MU][iS]; else GS=prop_b[MU][iS-maxelm];
	if (!bB) GB=prop[MU][iB]; else GB=prop_b[MU][iB-maxelm];

	// ���������� ������������ �������� :
	if (iflowregime==ZEROEQMOD) {
		// �������������� ��� ����������� 
		// �������� ����������.
		GP+=potent[MUT][iP];
		GE+=potent[MUT][iE];
		GN+=potent[MUT][iN];
		GT+=potent[MUT][iT];
		GW+=potent[MUT][iW];
		GS+=potent[MUT][iS];
		GB+=potent[MUT][iB];
	}

    // �������� ������������ �������� �� ����� ��.
    Ge=GE*GP/(feplus*GE+(1-feplus)*GP); // ���������.
	Gw=GW*GP/(fwplus*GW+(1-fwplus)*GP);
	Gn=GN*GP/(fnplus*GN+(1-fnplus)*GP);
	Gs=GS*GP/(fsplus*GS+(1-fsplus)*GP);
    Gt=GT*GP/(ftplus*GT+(1-ftplus)*GP);
	Gb=GB*GP/(fbplus*GB+(1-fbplus)*GP);


	// ������������ ������������ ������:
	doublereal De=1.0, Dw=1.0, Dn=1.0, Ds=1.0, Dt=1.0, Db=1.0; // �������������
	De=Ge*dy*dz/dxe;
	Dw=Gw*dy*dz/dxw;
	Dn=Gn*dx*dz/dyn;
	Ds=Gs*dx*dz/dys;
    Dt=Gt*dx*dy/dzt;
	Db=Gb*dx*dy/dzb;


	// ����� �����:
	doublereal Pe, Pw, Pn, Ps, Pt, Pb;
	Pe=(Fe)/De; 
	Pw=-(Fw)/Dw; 
	Pn=(Fn)/Dn; 
	Ps=-(Fs)/Ds; 
    Pt=(Ft)/Dt; 
	Pb=-(Fb)/Db; 

	// ���� ������������� �������� ������� �� �������:
    if (!bE) {
		if (bW) De*=dbeta;
	} else De*=dbeta;

	if (!bW) {
		if (bE) Dw*=dbeta;
	} else Dw*=dbeta;

	if (!bN) {
		if (bS) Dn*=dbeta;
	} else Dn*=dbeta;

	if (!bS) {
		if (bN) Ds*=dbeta;
	} else Ds*=dbeta; 

	if (!bT) {
		if (bB) Dt*=dbeta; 
	} else Dt*=dbeta;

	if (!bB) {
		if (bT) Db*=dbeta;
	} else Db*=dbeta;

	

	// ������� � ������ ����� ��� ������������� ����� �������� QUICK
	// � ���� ������������� ������ ���������� ���������.
	// addition to the right side QUICK Leonard.
	doublereal attrs=0.0;

	if (ishconvection < distsheme) {

		if (1) {
			// ������� ��� ����������� ������ � ��������������� � ��������� 7.05.2017. 

			// TODO 25 07 2015
			// ���������� ������������� ����������� �������:
		    sl[iVar][iP].ae=De*ApproxConvective(fabs(Pe),ishconvection)+fmax(-(Fe),0); 
		    sl[iVar][iP].aw=Dw*ApproxConvective(fabs(Pw),ishconvection)+fmax(Fw,0); 
			sl[iVar][iP].an=Dn*ApproxConvective(fabs(Pn),ishconvection)+fmax(-(Fn),0); 
			sl[iVar][iP].as=Ds*ApproxConvective(fabs(Ps),ishconvection)+fmax(Fs,0); 
			sl[iVar][iP].at=Dt*ApproxConvective(fabs(Pt),ishconvection)+fmax(-(Ft),0); 
			sl[iVar][iP].ab=Db*ApproxConvective(fabs(Pb),ishconvection)+fmax(Fb,0); 
			//sl[iVar][iP].ap=sl[iVar][iP].ae+sl[iVar][iP].aw+sl[iVar][iP].an+sl[iVar][iP].as+sl[iVar][iP].at+sl[iVar][iP].ab;
		}
		else
		{
			// �������� �� ������ ���������������������� 25 ���� 2015.
			if (!bE) {
				sl[iVar][iP].ae = De*ApproxConvective(fabs(Pe), ishconvection) + fmax(-(Fe), 0);
			}
			else {
				integer inumber = iE - maxelm;
				if (sosedb[inumber].MCB == (ls + lw)) {
					// ������� �� ��������� : ������ ������.
					// �������� ������� ���� �� �������, ��� �� ����� ������� ������.
					sl[iVar][iP].ae = De*ApproxConvective(fabs(Pe), ishconvection) + fabs(Fe);
				}
				else {
					sl[iVar][iP].ae = De*ApproxConvective(fabs(Pe), ishconvection) + fmax(-(Fe), 0);
				}
			}
			if (!bW) {
				sl[iVar][iP].aw = Dw*ApproxConvective(fabs(Pw), ishconvection) + fmax(Fw, 0);
			}
			else {
				integer inumber = iW - maxelm;
				if (sosedb[inumber].MCB == (ls + lw)) {
					// ������� �� ��������� : ������ ������.
					// �������� ������� ���� �� �������, ��� �� ����� ������� ������.
					sl[iVar][iP].aw = Dw*ApproxConvective(fabs(Pw), ishconvection) + fabs(Fw);
				}
				else {
					sl[iVar][iP].aw = Dw*ApproxConvective(fabs(Pw), ishconvection) + fmax(Fw, 0);
				}
			}
			if (!bN) {
				sl[iVar][iP].an = Dn*ApproxConvective(fabs(Pn), ishconvection) + fmax(-(Fn), 0);
			}
			else {
				integer inumber = iN - maxelm;
				if (sosedb[inumber].MCB == (ls + lw)) {
					// ������� �� ��������� : ������ ������.
					// �������� ������� ���� �� �������, ��� �� ����� ������� ������.
					sl[iVar][iP].an = Dn*ApproxConvective(fabs(Pn), ishconvection) + fabs(Fn);
				}
				else {
					sl[iVar][iP].an = Dn*ApproxConvective(fabs(Pn), ishconvection) + fmax(-(Fn), 0);
				}
			}
			if (!bS) {
				sl[iVar][iP].as = Ds*ApproxConvective(fabs(Ps), ishconvection) + fmax(Fs, 0);
			}
			else {
				integer inumber = iS - maxelm;
				if (sosedb[inumber].MCB == (ls + lw)) {
					// ������� �� ��������� : ������ ������.
					// �������� ������� ���� �� �������, ��� �� ����� ������� ������.
					sl[iVar][iP].as = Ds*ApproxConvective(fabs(Ps), ishconvection) + fabs(Fs);
				}
				else {
					sl[iVar][iP].as = Ds*ApproxConvective(fabs(Ps), ishconvection) + fmax(Fs, 0);
				}
			}
			if (!bT) {
				sl[iVar][iP].at = Dt*ApproxConvective(fabs(Pt), ishconvection) + fmax(-(Ft), 0);
			}
			else {
				integer inumber = iT - maxelm;
				if (sosedb[inumber].MCB == (ls + lw)) {
					// ������� �� ��������� : ������ ������.
					// �������� ������� ���� �� �������, ��� �� ����� ������� ������.
					sl[iVar][iP].at = Dt*ApproxConvective(fabs(Pt), ishconvection) + fabs(Ft);
				}
				else {
					sl[iVar][iP].at = Dt*ApproxConvective(fabs(Pt), ishconvection) + fmax(-(Ft), 0);
				}
			}
			if (!bB) {
				sl[iVar][iP].ab = Db*ApproxConvective(fabs(Pb), ishconvection) + fmax(Fb, 0);
			}
			else
			{
				integer inumber = iB - maxelm;
				if (sosedb[inumber].MCB == (ls + lw)) {
					// ������� �� ��������� : ������ ������.
					// �������� ������� ���� �� �������, ��� �� ����� ������� ������.
					sl[iVar][iP].ab = Db*ApproxConvective(fabs(Pb), ishconvection) + fabs(Fb);
				}
				else {
					sl[iVar][iP].ab = Db*ApproxConvective(fabs(Pb), ishconvection) + fmax(Fb, 0);
				}
			}

		}
        
	    // ������ ��� ����������� ������ � ��������� � ����������. 7.05.2017.
		//sl[iVar][iP].ap=sl[iVar][iP].ae+sl[iVar][iP].aw+sl[iVar][iP].an+sl[iVar][iP].as+sl[iVar][iP].at+sl[iVar][iP].ab;
		// ��� ���������:
		// ����� ������������� !!! (���������� �� ����).
		sl[iVar][iP].ap = De*ApproxConvective(fabs(Pe), ishconvection) + fmax(+(Fe), 0);
		sl[iVar][iP].ap += Dw*ApproxConvective(fabs(Pw), ishconvection) + fmax(-(Fw), 0);
		sl[iVar][iP].ap += Dn*ApproxConvective(fabs(Pn), ishconvection) + fmax(+(Fn), 0);
		sl[iVar][iP].ap += Ds*ApproxConvective(fabs(Ps), ishconvection) + fmax(-(Fs), 0);
		sl[iVar][iP].ap += Dt*ApproxConvective(fabs(Pt), ishconvection) + fmax(+(Ft), 0);
		sl[iVar][iP].ap += Db*ApproxConvective(fabs(Pb), ishconvection) + fmax(-(Fb), 0);

		// 13 ������� 2016
		// ��� ��������. ��� ����� �� �������� � ����������. �� ������� ��� ��������� ������������ ������������, ��
		// ������������� ���������� ���� � �������, �� �� ��������������� ��� ����� ���������� ������������ ����������� ������ ������.
		//sl[iVar][iP].ap = fabs(sl[iVar][iP].ae) + fabs(sl[iVar][iP].aw) + fabs(sl[iVar][iP].an) + fabs(sl[iVar][iP].as) + fabs(sl[iVar][iP].at) + fabs(sl[iVar][iP].ab);
		

		
		if (sl[iVar][iP].ap<1.0e-40) {
			printf("Zero diagonal coefficient in internal volume in my_elmatr_quad_F3D.\n");
#if doubleintprecision == 1
			printf("ap=%e iP=%lld\n", sl[iVar][iP].ap, iP);
#else
			printf("ap=%e iP=%d\n", sl[iVar][iP].ap, iP);
#endif
			
			printf("ae=%e aw=%e an=%e as=%e at=%e ab=%e\n", sl[iVar][iP].ae, sl[iVar][iP].aw, sl[iVar][iP].an, sl[iVar][iP].as, sl[iVar][iP].at, sl[iVar][iP].ab);
			system("pause");

			// ��� ������ ������ �� ������� �� ��������
			// �.�. �� ������ ����� ����� ������� ������������ �������.
			// ������� ����� �������� �������� ����� ������ ����� ��������� �����������, ��� ����� �����������.
			// �� �������� �� ��-���� ��������� �������� ����� ���������.
            sl[iVar][iP].ae=De*ApproxConvective(fabs(Pe),BULG);//+fmax(-(Fe),0); 
	        sl[iVar][iP].aw=Dw*ApproxConvective(fabs(Pw),BULG);//+fmax(Fw,0); 
	        sl[iVar][iP].an=Dn*ApproxConvective(fabs(Pn),BULG);//+fmax(-(Fn),0); 
	        sl[iVar][iP].as=Ds*ApproxConvective(fabs(Ps),BULG);//+fmax(Fs,0); 
            sl[iVar][iP].at=Dt*ApproxConvective(fabs(Pt),BULG);//+fmax(-(Ft),0); 
	        sl[iVar][iP].ab=Db*ApproxConvective(fabs(Pb),BULG);//+fmax(Fb,0); 
		    sl[iVar][iP].ap=sl[iVar][iP].ae+sl[iVar][iP].aw+sl[iVar][iP].an+sl[iVar][iP].as+sl[iVar][iP].at+sl[iVar][iP].ab;
		}
		

		// ������ ��� ����������� ������ � ��������� � ����������. 7.05.2017.
		//sumanb=sl[iVar][iP].ae+sl[iVar][iP].aw+sl[iVar][iP].an+sl[iVar][iP].as+sl[iVar][iP].at+sl[iVar][iP].ab;
		// ��� ���������:
		// ����� ������������� !!! (���������� �� ����).
		sumanb = De*ApproxConvective(fabs(Pe), ishconvection) + fmax(+(Fe), 0);
		sumanb += Dw*ApproxConvective(fabs(Pw), ishconvection) + fmax(-(Fw), 0);
		sumanb += Dn*ApproxConvective(fabs(Pn), ishconvection) + fmax(+(Fn), 0);
		sumanb += Ds*ApproxConvective(fabs(Ps), ishconvection) + fmax(-(Fs), 0);
		sumanb += Dt*ApproxConvective(fabs(Pt), ishconvection) + fmax(+(Ft), 0);
		sumanb += Db*ApproxConvective(fabs(Pb), ishconvection) + fmax(-(Fb), 0);

		//13 ������� 2016.
		//sumanb = fabs(sl[iVar][iP].ae) + fabs(sl[iVar][iP].aw) + fabs(sl[iVar][iP].an) + fabs(sl[iVar][iP].as) + fabs(sl[iVar][iP].at) + fabs(sl[iVar][iP].ab);

	}
	  else if (ishconvection < QUICK)
	{
        // ���������� ������������� ����������� �������:
	    sl[iVar][iP].ae=-(Fe)*fC(Pe, ishconvection, true, feplus)+De*fD(Pe, ishconvection, true, feplus); 
	    sl[iVar][iP].aw=(Fw)*fC(Pw, ishconvection, true, fwplus)+Dw*fD(Pw, ishconvection, true, fwplus); 
	    sl[iVar][iP].an=-(Fn)*fC(Pn, ishconvection, true, fnplus)+Dn*fD(Pn, ishconvection, true, fnplus); 
	    sl[iVar][iP].as=(Fs)*fC(Ps, ishconvection, true, fsplus)+Ds*fD(Ps, ishconvection, true, fsplus); 
        sl[iVar][iP].at=-(Ft)*fC(Pt, ishconvection, true, ftplus)+Dt*fD(Pt, ishconvection, true, ftplus); 
	    sl[iVar][iP].ab=(Fb)*fC(Pb, ishconvection, true, fbplus)+Db*fD(Pb, ishconvection, true, fbplus); 
		//sl[iVar][iP].ap=sl[iVar][iP].ae+sl[iVar][iP].aw+sl[iVar][iP].an+sl[iVar][iP].as+sl[iVar][iP].at+sl[iVar][iP].ab;

		// ������ ��� ����������� ������ � ��������� � ����������. 7.05.2017.
		//sumanb=sl[iVar][iP].ae+sl[iVar][iP].aw+sl[iVar][iP].an+sl[iVar][iP].as+sl[iVar][iP].at+sl[iVar][iP].ab;
		//13 ������� 2016.
		//sumanb = fabs(sl[iVar][iP].ae) + fabs(sl[iVar][iP].aw) + fabs(sl[iVar][iP].an) + fabs(sl[iVar][iP].as) + fabs(sl[iVar][iP].at) + fabs(sl[iVar][iP].ab);

		// 08.05.2017.
		// ��� ���������:
		// ����� ������������� !!! (���������� �� ����).

		sl[iVar][iP].ap = +(Fe)*fC(Pe, ishconvection, true, feplus) + De*fD(Pe, ishconvection, true, feplus);
		sl[iVar][iP].ap += -(Fw)*fC(Pw, ishconvection, true, fwplus) + Dw*fD(Pw, ishconvection, true, fwplus);
		sl[iVar][iP].ap += +(Fn)*fC(Pn, ishconvection, true, fnplus) + Dn*fD(Pn, ishconvection, true, fnplus);
		sl[iVar][iP].ap += -(Fs)*fC(Ps, ishconvection, true, fsplus) + Ds*fD(Ps, ishconvection, true, fsplus);
		sl[iVar][iP].ap += +(Ft)*fC(Pt, ishconvection, true, ftplus) + Dt*fD(Pt, ishconvection, true, ftplus);
		sl[iVar][iP].ap += -(Fb)*fC(Pb, ishconvection, true, fbplus) + Db*fD(Pb, ishconvection, true, fbplus);

		sumanb = +(Fe)*fC(Pe, ishconvection, true, feplus) + De*fD(Pe, ishconvection, true, feplus);
		sumanb += -(Fw)*fC(Pw, ishconvection, true, fwplus) + Dw*fD(Pw, ishconvection, true, fwplus);
		sumanb += +(Fn)*fC(Pn, ishconvection, true, fnplus) + Dn*fD(Pn, ishconvection, true, fnplus);
		sumanb += -(Fs)*fC(Ps, ishconvection, true, fsplus) + Ds*fD(Ps, ishconvection, true, fsplus);
		sumanb += +(Ft)*fC(Pt, ishconvection, true, ftplus) + Dt*fD(Pt, ishconvection, true, ftplus);
		sumanb += -(Fb)*fC(Pb, ishconvection, true, fbplus) + Db*fD(Pb, ishconvection, true, fbplus);

	}
	  else if (ishconvection >= QUICK)
	{
		// � 3D ������������ ������ ����� ������������ �� ��� ���������� �����.
		// ����� �������� ����� ������ ������� � ����������� � ������� ��������� ���������� ���������.
		integer iVarCOR=VXCOR;
		switch (iVar) {
		  case VX : iVarCOR=VXCOR; break; // VXCOR
		  case VY : iVarCOR=VYCOR; break; // VYCOR
		  case VZ : iVarCOR=VZCOR; break; // VZCOR
		  default : 
			  printf("Error ! This feature is not available. \n");
			  printf("exeption in my_elmatr_quad_f3D.c : iVar != Vx || Vy || Vz.\n");
			  printf("Please, press any key to exit...\n");
			  //getchar();
			  system("pause");
			  exit(0);
			  break;
		}

		// X - direction
		doublereal positionxP, positionxE, positionxW, positionxEE, positionxWW, positionxe, positionxw;
		doublereal SpeedP=0.0, SpeedE=0.0, SpeedW=0.0, SpeedEE=0.0, SpeedWW=0.0, SpeedN=0.0, SpeedS=0.0;
		doublereal SpeedNN=0.0, SpeedSS=0.0, SpeedT=0.0, SpeedB=0.0, SpeedTT=0.0, SpeedBB=0.0;
		doublereal Speede=0.0, Speedw=0.0, Speedn=0.0, Speeds=0.0, Speedt=0.0, Speedb=0.0;
		// Y - direction
		doublereal positionyP, positionyN, positionyS, positionyNN, positionySS, positionyn, positionys;
		// Z - direction
		doublereal positionzP, positionzT, positionzB, positionzTT, positionzBB, positionzt, positionzb;

		TOCHKA pointP;
		center_cord3D(iP,nvtx,pa,pointP,100);
		positionxP=pointP.x; positionyP=pointP.y; positionzP=pointP.z;
		SpeedP=potent[iVarCOR][iP];
		// X - direction
		if (!bE) {
			SpeedE=potent[iVarCOR][iE];
			center_cord3D(iE,nvtx,pa,pointP,ESIDE);
			positionxE=pointP.x;
			positionxe=positionxP+0.5*dx;

			integer iEE=sosedi[EE][iP].iNODE1;
			if ((iEE>=0)&&(iEE < maxelm)) {
				// ���������� ����
				SpeedEE=potent[iVarCOR][iEE];
				center_cord3D(iEE,nvtx,pa,pointP,EE);
				positionxEE=pointP.x;
			}
			else
			{
				// ��������� ����
				SpeedEE=potent[iVarCOR][iEE];
				volume3D(iE, nvtx, pa, pointP.x, pointP.y, pointP.z);
				positionxEE=positionxE+0.5*pointP.x;
			}
		}
		else {
			// ��� ��������� ����
			SpeedE=potent[iVarCOR][iE];
			SpeedEE=potent[iVarCOR][iE]; 
			positionxe=positionxP+0.5*dx;
            positionxE=positionxP+0.5*dx;
			positionxEE=positionxP+dx; // ����� ���� �� ���������� !
		}

		if (!bW) {
			center_cord3D(iW,nvtx,pa,pointP,WSIDE);
			positionxW=pointP.x;
			positionxw=positionxP-0.5*dx;
			SpeedW=potent[iVarCOR][iW];

			integer iWW=sosedi[WW][iP].iNODE1;
			if ((iWW >= 0) && (iWW < maxelm)) {
				// ���������� ����
				SpeedWW=potent[iVarCOR][iWW];
				center_cord3D(iWW,nvtx,pa,pointP,WW);
				positionxWW=pointP.x;
			}
			else
			{
				// ��������� ����
				SpeedWW=potent[iVarCOR][iWW];
				volume3D(iW, nvtx, pa, pointP.x, pointP.y, pointP.z);
				positionxWW=positionxW-0.5*pointP.x;
			}
		}
		else {
			// ��� ��������� ����
			SpeedW=potent[iVarCOR][iW]; // Attantion !! Debug TODO
			SpeedWW=potent[iVarCOR][iW]; 
			//printf("SpeedW==%e\n",SpeedW); getchar();
			positionxw=positionxP-0.5*dx;
            positionxW=positionxP-0.5*dx;
			positionxWW=positionxP-dx; // ����� ���� �� ���������� !
		}

		// Y - direction
		if (!bN) {
			SpeedN=potent[iVarCOR][iN];
			center_cord3D(iN,nvtx,pa,pointP,NSIDE);
			positionyN=pointP.y;
			positionyn=positionxP+0.5*dy;

			integer iNN=sosedi[NN][iP].iNODE1;
			if ((iNN >= 0) && (iNN < maxelm)) {
				// ���������� ����
				SpeedNN=potent[iVarCOR][iNN];
				center_cord3D(iNN,nvtx,pa,pointP,NN);
				positionyNN=pointP.y;
			}
			else
			{
				// ��������� ����
				SpeedNN=potent[iVarCOR][iNN];
				volume3D(iN, nvtx, pa, pointP.x, pointP.y, pointP.z);
				positionyNN=positionyN+0.5*pointP.y;
			}
		}
		else {
			// ��� ��������� ����
			SpeedN=potent[iVarCOR][iN];
			SpeedNN=potent[iVarCOR][iN]; 
			positionyn=positionyP+0.5*dy;
            positionyN=positionyP+0.5*dy;
			positionyNN=positionyP+dy; // ����� ���� �� ���������� !
		}

		if (!bS) {
			SpeedS=potent[iVarCOR][iS];
			center_cord3D(iS,nvtx,pa,pointP,SSIDE);
			positionyS=pointP.y;
			positionys=positionyP-0.5*dy;

			integer iSS=sosedi[SS][iP].iNODE1;
			if ((iSS >= 0) && (iSS < maxelm)) {
				// ���������� ����
				SpeedSS=potent[iVarCOR][iSS];
				center_cord3D(iSS,nvtx,pa,pointP,SS);
				positionySS=pointP.y;
			}
			else
			{
				// ��������� ����
				SpeedSS=potent[iVarCOR][iSS];
				volume3D(iS, nvtx, pa, pointP.x, pointP.y, pointP.z);
				positionySS=positionyS-0.5*pointP.y;
			}
		}
		else {
			// ��� ��������� ����
			SpeedS=potent[iVarCOR][iS]; // ATTANTION !!!! TODO 
			SpeedSS=potent[iVarCOR][iS]; // ������� �������� ������ ������� ����.
			positionys=positionyP-0.5*dy;
            positionyS=positionyP-0.5*dy;
			positionySS=positionyP-dy; // ����� ���� �� ���������� !
		}

		// Z - direction
		if (!bT) {
			SpeedT=potent[iVarCOR][iT];
			center_cord3D(iT,nvtx,pa,pointP,TSIDE);
			positionzT=pointP.z;
			positionzt=positionzP+0.5*dz;

			integer iTT=sosedi[TTSIDE][iP].iNODE1;
			if ((iTT >= 0) && (iTT < maxelm)) {
				// ���������� ����
				SpeedTT=potent[iVarCOR][iTT];
				center_cord3D(iTT,nvtx,pa,pointP,TTSIDE);
				positionzTT=pointP.z;
			}
			else
			{
				// ��������� ����
				SpeedTT=potent[iVarCOR][iTT];
				volume3D(iT, nvtx, pa, pointP.x, pointP.y, pointP.z);
				positionzTT=positionzT+0.5*pointP.z;
			}
		}
		else {
			// ��� ��������� ����
			SpeedT=potent[iVarCOR][iT];
			SpeedTT=potent[iVarCOR][iT]; // �������� ������ ������� ����
			positionzt=positionzP+0.5*dz;
            positionzT=positionzP+0.5*dz;
			positionzTT=positionzP+dz; // ����� ���� �� ���������� !
		}

		if (!bB) {
			SpeedB=potent[iVarCOR][iB];
			center_cord3D(iB,nvtx,pa,pointP,BSIDE);
			positionzB=pointP.z;
			positionzb=positionzP-0.5*dz;

			integer iBB=sosedi[BB][iP].iNODE1;
			if ((iBB >= 0) && (iBB < maxelm)) {
				// ���������� ����
				SpeedBB=potent[iVarCOR][iBB];
				center_cord3D(iBB,nvtx,pa,pointP,BB);
				positionzBB=pointP.z;
			}
			else
			{
				// ��������� ����
				SpeedBB=potent[iVarCOR][iBB];
				volume3D(iB, nvtx, pa, pointP.x, pointP.y, pointP.z);
				positionzBB=positionzB-0.5*pointP.z;
			}
		}
		else {
			// ��� ��������� ����
			SpeedB=potent[iVarCOR][iB];
			SpeedBB=potent[iVarCOR][iB]; // �������� ������ ������� ����
			positionzb=positionzP-0.5*dz;
            positionzB=positionzP-0.5*dz;
			positionzBB=positionzP-dz; // ����� ���� �� ���������� !
		}


		if ((ishconvection >= QUICK) && (ishconvection <= FROMM)) {
             // ������ ����� ������������ �� ��������� �. ��������� PHOENICS.
			// ������������� ishconvection ������ ��������� �������� �� ����� QUICK (������������) � ������ �� ����� UNEVENQUICK (�� ������� ���������).

		     // X - direction
		     Speede=cell_face_value_global(ishconvection, (Fe), SpeedW, SpeedP, SpeedE, SpeedEE); 
		     Speedw=cell_face_value_global(ishconvection, (Fw), SpeedWW, SpeedW, SpeedP, SpeedE); 
		     // Y - direction
		     Speedn=cell_face_value_global(ishconvection, (Fn), SpeedS, SpeedP, SpeedN, SpeedNN); 
		     Speeds=cell_face_value_global(ishconvection, (Fs), SpeedSS, SpeedS, SpeedP, SpeedN); 
		     // Z - direction
		     Speedt=cell_face_value_global(ishconvection, (Ft), SpeedB, SpeedP, SpeedT, SpeedTT); 
		     Speedb=cell_face_value_global(ishconvection, (Fb), SpeedBB, SpeedB, SpeedP, SpeedT); 
		     
		}

		if (ishconvection >= UNEVENQUICK) {
		

           /*
		   // ����������������� �������� ��������� � ����� ���������� ���������� ����� QUICK �� ������������� �����.
		   // ���������� ���� ������������ �� ������ : ...
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
		        Speede=workQUICK(dx, 2.0*(positionxE-positionxe), positionxW, positionxP, positionxE, positionxEE, SpeedW, SpeedP, SpeedE, SpeedEE, (Fe)); 
		        Speedw=workQUICK(2.0*(positionxw-positionxW), dx, positionxWW, positionxW, positionxP, positionxE, SpeedWW, SpeedW, SpeedP, SpeedE, (Fw)); 
		        // Y - direction
		        Speedn=workQUICK(dy, 2.0*(positionyN-positionyn), positionyS, positionyP, positionyN, positionyNN, SpeedS, SpeedP, SpeedN, SpeedNN, (Fn)); 
		        Speeds=workQUICK(2.0*(positionys-positionyS), dy, positionySS, positionyS, positionyP, positionyN, SpeedSS, SpeedS, SpeedP, SpeedN, (Fs)); 
		        // Z - direction
		        Speedt=workQUICK(dz, 2.0*(positionzT-positionzt), positionzB, positionzP, positionzT, positionzTT, SpeedB, SpeedP, SpeedT, SpeedTT, (Ft)); 
		        Speedb=workQUICK(2.0*(positionzb-positionzB), dz, positionzBB, positionzB, positionzP, positionzT, SpeedBB, SpeedB, SpeedP, SpeedT, (Fb)); 
			}

			if ((ishconvection > UNEVENQUICK) && (ishconvection <= UNEVEN_CUBISTA )) {
				// ���� �� ������ ������ ������������� ����������� ������������ ������ ������ ������ �����:
				// 1. UNEVEN_MUSCL, 2. UNEVEN_SOUCUP, 3. UNEVEN_HLPA, 4. UNEVEN_SMART.
				// ������������� ����� ������ ��������������� ��������.

				// X - direction
		        Speede=workKN_VOLKOV( positionxW, positionxP, positionxE, positionxEE, SpeedW, SpeedP, SpeedE, SpeedEE, (Fe), ishconvection); 
		        Speedw=workKN_VOLKOV( positionxWW, positionxW, positionxP, positionxE, SpeedWW, SpeedW, SpeedP, SpeedE, (Fw), ishconvection); 
		        // Y - direction
		        Speedn=workKN_VOLKOV( positionyS, positionyP, positionyN, positionyNN, SpeedS, SpeedP, SpeedN, SpeedNN, (Fn), ishconvection); 
		        Speeds=workKN_VOLKOV( positionySS, positionyS, positionyP, positionyN, SpeedSS, SpeedS, SpeedP, SpeedN, (Fs), ishconvection); 
		        // Z - direction
		        Speedt=workKN_VOLKOV( positionzB, positionzP, positionzT, positionzTT, SpeedB, SpeedP, SpeedT, SpeedTT, (Ft), ishconvection); 
		        Speedb=workKN_VOLKOV( positionzBB, positionzB, positionzP, positionzT, SpeedBB, SpeedB, SpeedP, SpeedT, (Fb), ishconvection); 

				// debug ������ �������� ������.
				//printf("%f, %f, %f, %f, %f, %f\n",Speede,Speedw,Speedn,Speeds,Speedt,Speedb);
				//getchar();
			}
		
		} // endif (ishconvection >= UNEVENQUICK)
		
		

		// ������: SIMPLE method for the solution of incompressible flows on non-staggered grids
		// I. Sezai - Eastern Mediterranean University, Mechanical Engineering Department, Mersin 10-Turkey Revised in January, 2011.

		// ���������� ������������� ����������� �������:
		// ����������� ����� ���������� ��������� :
		// ������ ����������� ������ ��������������� �����, 
		// � ���������� ����� ������������ � ������ ����� 
		// �������� ������� ���������.
		if (1) {

           /*
	       sl[iVar][iP].ae=De*fD(Pe, EXP2, true, feplus) + fmax(-(Fe),0.0); 
           sl[iVar][iP].aw=Dw*fD(Pw, EXP2, true, fwplus) + fmax((Fw),0.0); 
	       sl[iVar][iP].an=Dn*fD(Pn, EXP2, true, fnplus) + fmax(-(Fn),0.0); 
		   sl[iVar][iP].as=Ds*fD(Ps, EXP2, true, fsplus) + fmax((Fs),0.0); 
           sl[iVar][iP].at=Dt*fD(Pt, EXP2, true, ftplus) + fmax(-(Ft),0.0); 
		   sl[iVar][iP].ab=Db*fD(Pb, EXP2, true, fbplus) + fmax((Fb),0.0);
		   */

			// ������� ��� ����������� ������ � ������������� � ���������� 7.05.2017.
			// ����� ������ UDS.
			// ��� ����������� � ����������.
		   sl[iVar][iP].ae=De + fmax(-(Fe),0.0); 
           sl[iVar][iP].aw=Dw + fmax((Fw),0.0); 
	       sl[iVar][iP].an=Dn + fmax(-(Fn),0.0); 
		   sl[iVar][iP].as=Ds + fmax((Fs),0.0); 
           sl[iVar][iP].at=Dt + fmax(-(Ft),0.0); 
		   sl[iVar][iP].ab=Db + fmax((Fb),0.0);


		   // 08.05.2017.
		   // ��� ���������:
		   // ����� ������������� !!! (���������� �� ����).
		   sl[iVar][iP].ap = De + fmax(+(Fe), 0.0);
		   sl[iVar][iP].ap += Dw + fmax(-(Fw), 0.0);
		   sl[iVar][iP].ap += Dn + fmax(+(Fn), 0.0);
		   sl[iVar][iP].ap += Ds + fmax(-(Fs), 0.0);
		   sl[iVar][iP].ap += Dt + fmax(+(Ft), 0.0);
		   sl[iVar][iP].ap += Db + fmax(-(Fb), 0.0);


		   sumanb = De + fmax(+(Fe), 0.0);
		   sumanb += Dw + fmax(-(Fw), 0.0);
		   sumanb += Dn + fmax(+(Fn), 0.0);
		   sumanb += Ds + fmax(-(Fs), 0.0);
		   sumanb += Dt + fmax(+(Ft), 0.0);
		   sumanb += Db + fmax(-(Fb), 0.0);

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
			   sl[iVar][iP].ae=De + fmax(-(Fe),0.0); 
	       }
		   else {
			   integer inumber=iE-maxelm;
			   if (sosedb[inumber].MCB==(ls+lw)) {
				   // ������� �� ��������� : ������ ������.
				   // �������� ������� ���� �� �������, ��� �� ����� ������� ������.
                   sl[iVar][iP].ae=De + fabs(Fe); 
			   }
			   else {
				   // �� ���� ��������� ������� ����� ������� ������� �� �������.
				   sl[iVar][iP].ae=De + fabs(Fe); 
			   }
		   }

		   if (!bW) {
			   // ������ ����������.
			   sl[iVar][iP].aw=Dw + fmax((Fw),0.0);
		   }
		   else {
               integer inumber=iW-maxelm;
			   if (sosedb[inumber].MCB==(ls+lw)) {
				   // ������� �� ��������� : ������ ������.
				   // �������� ������� ���� �� �������, ��� �� ����� ������� ������.
				   sl[iVar][iP].aw=Dw + fabs(Fw);
			   }
			    else {
				   // �� ���� ��������� ������� ����� ������� ������� �� �������.
                   sl[iVar][iP].aw=Dw + fabs(Fw);
				}
		   }

		    if (!bN) {
			   // ������ ����������.
			   sl[iVar][iP].an=Dn + fmax(-(Fn),0.0); 
	       }
		   else {
			   integer inumber=iN-maxelm;
			   if (sosedb[inumber].MCB==(ls+lw)) {
				   // ������� �� ��������� : ������ ������.
				   // �������� ������� ���� �� �������, ��� �� ����� ������� ������.
                   sl[iVar][iP].an=Dn + fabs(Fn); 
			   }
			   else {
				   // �� ���� ��������� ������� ����� ������� ������� �� �������.
				   sl[iVar][iP].an=Dn + fabs(Fn); 
			   }
		   }

		   if (!bS) {
			   // ������ ����������.
			   sl[iVar][iP].as=Ds + fmax((Fs),0.0);
		   }
		   else {
               integer inumber=iS-maxelm;
			   if (sosedb[inumber].MCB==(ls+lw)) {
				   // ������� �� ��������� : ������ ������.
				   // �������� ������� ���� �� �������, ��� �� ����� ������� ������.
				   sl[iVar][iP].as=Ds + fabs(Fs);
			   }
			    else {
				   // �� ���� ��������� ������� ����� ������� ������� �� �������.
                   sl[iVar][iP].as=Ds + fabs(Fs);
				}
		   }

		    if (!bT) {
			   // ������ ����������.
			   sl[iVar][iP].at=Dt + fmax(-(Ft),0.0); 
	       }
		   else {
			   integer inumber=iT-maxelm;
			   if (sosedb[inumber].MCB==(ls+lw)) {
				   // ������� �� ��������� : ������ ������.
				   // �������� ������� ���� �� �������, ��� �� ����� ������� ������.
                   sl[iVar][iP].at=Dt + fabs(Ft); 
			   }
			   else {
				   // �� ���� ��������� ������� ����� ������� ������� �� �������.
				   sl[iVar][iP].at=Dt + fabs(Ft); 
			   }
		   }

		   if (!bB) {
			   // ������ ����������.
			   sl[iVar][iP].ab=Db + fmax((Fb),0.0);
		   }
		   else {
               integer inumber=iB-maxelm;
			   if (sosedb[inumber].MCB==(ls+lw)) {
				   // ������� �� ��������� : ������ ������.
				   // �������� ������� ���� �� �������, ��� �� ����� ������� ������.
				   sl[iVar][iP].ab=Db + fabs(Fb);
			   }
			    else {
				   // �� ���� ��������� ������� ����� ������� ������� �� �������.
                   sl[iVar][iP].ab=Db + fabs(Fb);
				}
		   }

		   // 7.05.2017 ������� ��� ����������� ������ � ��������������� � ����������.
		   sl[iVar][iP].ap=sl[iVar][iP].ae+sl[iVar][iP].aw+sl[iVar][iP].an+sl[iVar][iP].as+sl[iVar][iP].at+sl[iVar][iP].ab;

		   sumanb=sl[iVar][iP].ae+sl[iVar][iP].aw+sl[iVar][iP].an+sl[iVar][iP].as+sl[iVar][iP].at+sl[iVar][iP].ab;
		
		}


		// 7.05.2017 ������� ��� ����������� ������ � ��������������� � ����������.
		//sl[iVar][iP].ap=sl[iVar][iP].ae+sl[iVar][iP].aw+sl[iVar][iP].an+sl[iVar][iP].as+sl[iVar][iP].at+sl[iVar][iP].ab;

		//sumanb=sl[iVar][iP].ae+sl[iVar][iP].aw+sl[iVar][iP].an+sl[iVar][iP].as+sl[iVar][iP].at+sl[iVar][iP].ab;

		//13 ������� 2016.
		//sl[iVar][iP].ap = fabs(sl[iVar][iP].ae) + fabs(sl[iVar][iP].aw) + fabs(sl[iVar][iP].an) + fabs(sl[iVar][iP].as) + fabs(sl[iVar][iP].at) + fabs(sl[iVar][iP].ab);
		//sumanb = fabs(sl[iVar][iP].ae) + fabs(sl[iVar][iP].aw) + fabs(sl[iVar][iP].an) + fabs(sl[iVar][iP].as) + fabs(sl[iVar][iP].at) + fabs(sl[iVar][iP].ab);


		if (1) {
		    // ����� � ������ ����� (����� ���������� ���������):
		    // X - direction
		    attrs+=-fmax((Fe),0)*(Speede-SpeedP)+fmax(-(Fe),0)*(Speede-SpeedE); 
		    attrs+=-fmax(-(Fw),0)*(Speedw-SpeedP)+fmax((Fw),0)*(Speedw-SpeedW); 
		    // Y - direction
		    attrs+=-fmax((Fn),0)*(Speedn-SpeedP)+fmax(-(Fn),0)*(Speedn-SpeedN); 
		    attrs+=-fmax(-(Fs),0)*(Speeds-SpeedP)+fmax((Fs),0)*(Speeds-SpeedS); 
		    // Z - direction
            attrs+=-fmax((Ft),0)*(Speedt-SpeedP)+fmax(-(Ft),0)*(Speedt-SpeedT); 
		    attrs+=-fmax(-(Fb),0)*(Speedb-SpeedP)+fmax((Fb),0)*(Speedb-SpeedB); 
		}
		else {
			// �������.
		   // 30 07 2015
		   // TODO
		   // ������ ������ ������� ����� ���������� �� UDS.
			if (!bE) {
				attrs+=-fmax((Fe),0)*(Speede-SpeedP)+fmax(-(Fe),0)*(Speede-SpeedE); 
			}
			if (!bW) {
				attrs+=-fmax(-(Fw),0)*(Speedw-SpeedP)+fmax((Fw),0)*(Speedw-SpeedW);
			}
			if (!bN) {
				attrs+=-fmax((Fn),0)*(Speedn-SpeedP)+fmax(-(Fn),0)*(Speedn-SpeedN);
			}
			if (!bS) {
                attrs+=-fmax(-(Fs),0)*(Speeds-SpeedP)+fmax((Fs),0)*(Speeds-SpeedS);
			}
			if (!bT) {
				attrs+=-fmax((Ft),0)*(Speedt-SpeedP)+fmax(-(Ft),0)*(Speedt-SpeedT);
			}
			if (!bB) {
				attrs+=-fmax(-(Fb),0)*(Speedb-SpeedP)+fmax((Fb),0)*(Speedb-SpeedB); 
			}

		}

		//attrs=0.0; // ����� ����� ������� ����������� ����������� (�������� ����� ��������).

	}


	if (0&&(sl[iVar][iP].ap <= 1.0e-9)) {
		sl[iVar][iP].ap = sl[iVar][iP].ae + sl[iVar][iP].aw + sl[iVar][iP].an + sl[iVar][iP].as + sl[iVar][iP].at + sl[iVar][iP].ab;
		sumanb = sl[iVar][iP].ae + sl[iVar][iP].aw + sl[iVar][iP].an + sl[iVar][iP].as + sl[iVar][iP].at + sl[iVar][iP].ab;
	}

	doublereal Folditer=0.0; // ���������������� �������� � ���������� ��������

    doublereal tau, apzero1, apzero0;
	doublereal Fold=0.0; // �������� ������� � ����������� ���������� ����
	if (btimedep) {
	   // ��������������
	   tau=tauparam;
	   Fold=speedoldtimestep[iP]; // ����� ����� ����� ���������� �������� ������ � ����������� ���� �� �������
	   switch (iVar) {
	    	// ����� ������������� � ���������
			// ��������������� ��������� �������������.
			case VX : Folditer=potent[VXCOR][iP]; break;
			case VY : Folditer=potent[VYCOR][iP]; break;
			case VZ : Folditer=potent[VZCOR][iP]; break;
		}
	}
	else {
	   // ������������

	   // ����� �����������:
		// ������� �������� ��������� ������ ��� SIMPLE ��������� ����������� ��������� �������:
		// ��. �������� ������ sigma flow ����� 5 ������� (�5.1.12).
		// ��. ������ ��������� ������.
		if (fabs(alpha[iVar]-1.0)<admission) tau=1e30; // ���������� ������� ��� �� �������, ��� ������������� ������������� ��������.
		else {
			// ������ ������� ���������� ���������� � �������������� ����� ������, �.�. ������� 2. ������ ����������� �� ��������.
			// ����� �������� ������� 2 �.�. �� ��������� � tau ������� ������������ � �������� ���-���.

			// 1.
			//tau=rP*dx*dy*dz*alpha[iVar]/((sl[iVar][iP].ae+sl[iVar][iP].aw+sl[iVar][iP].an+sl[iVar][iP].as+sl[iVar][iP].at+sl[iVar][iP].ab)*(1.0-alpha[iVar]));
			// 2.
			tau=rP*dx*dy*dz*alpha[iVar]/((sl[iVar][iP].ae+sl[iVar][iP].aw+sl[iVar][iP].an+sl[iVar][iP].as+sl[iVar][iP].at+sl[iVar][iP].ab));
		}
	   switch (iVar) {
	    	// ����� ������������� � ���������
			// ��������������� ��������� �������������.
			case VX : Fold=potent[VXCOR][iP]; break;
			case VY : Fold=potent[VYCOR][iP]; break;
			case VZ : Fold=potent[VZCOR][iP]; break;
		}

	   switch (iVar) {
	    	// ����� ������������� � ���������
			// ��������������� ��������� �������������.
			case VX : Folditer=potent[VXCOR][iP]; break;
			case VY : Folditer=potent[VYCOR][iP]; break;
			case VZ : Folditer=potent[VZCOR][iP]; break;
		}
	    	
	}

	if ((!btimedep) && (!imitation_time)) {
		// ����� ��������� imitation_time=false
		// �.�. ������ ���������� � ���������������� �������� � ��� ��� ������������ ��������������� ����� ������ ������� ����.

		// ������������ � �������� ����� �� ������� �� ������������
		apzero1=0.0; // � ������ ���������� ����
		apzero0=0.0; // � ����������� ���������� ����
	}
	else if (((!btimedep) && (imitation_time) && (fabs(alpha[iVar]-1.0)>admission))) {
		//  ������������ � ������������ �������� ����� �� ������� � ������ ����������


		// ����� ����� ������������ �� ����������� � ����������� ������������� ����.
		// �� ��� ��� ������ ������� ������� �� ������������ ����� ������ ���������� �������� ������� ������ ������ � ������.
		// ���� ������� �� ������������ �.�. ����������� �������������� ����� ����� ����� ������ ������� � �� � ���� ����� ����
		// � �������������� �������������.
		apzero0=apzero1=rP*dx*dy*dz/tau;
		printf("error ispolzuetsq staticionar solver and imitation time step!\n");
		//getchar();
		system("pause");
		exit(0);
	}
	else if (btimedep) {
		// ��������� ��������������.

		apzero1=rP*dx*dy*dz/tau;

		TOCHKA p;
		center_cord3D(iP,nvtx,pa,p,100);
		integer ib; // ����� �������� �����
		in_model_flow(p,ib,b,lb);

		doublereal rho=1.1614;

		if (matlist[b[ib].imatid].blibmat==1) {
			// ������������ ����������������� �������.
			// ������ ����� �������� ������ FLUID ������� �� ��� �������� ������� ���.
			if (b[ib].itype==SOLID) {
				doublereal cp, lam; // �������� �� ������������ �� ���������
				my_solid_properties(toldtimestep[ptr[iP]],rho,cp,lam,matlist[b[ib].imatid].ilibident);
			} // SOLID
			if (b[ib].itype==FLUID) {
				doublereal mu, beta_t, lam, cp; // �������� �� ������������ �� ���������
				doublereal pressure=potent[PRESS][iP]; // �� �� ����� ���� �������� ��������� � ����������� ���������� ����.
				my_fluid_properties(toldtimestep[ptr[iP]], pressure, rho, cp, lam, mu, beta_t, matlist[b[ib].imatid].ilibident);
			}// FLUID
		}
		else if (matlist[b[ib].imatid].blibmat==0) {
			// �������� ����������� ������������� :
			rho=matlist[b[ib].imatid].rho;
		}
		apzero0=rho*dx*dy*dz/tau;

	}
	else {
		apzero1=0.0;
		apzero0=0.0;
	}
  
	// ������������ ����
	doublereal dSc=0.0, dSp=0.0, Bp=0.0;
	// Bp - ��� ����� �������� ��������. ����������� ����� �� �����������
	// �������� ����� ���������.

	// ���������� ����������� �������� ����������� � ��������� �������:
	//integer tavg = 0.0; // ����������� ����������� � ��������� �������.
	//for (integer i_1 = 0; i_1 < maxelm; i_1++) {
		//if (potent_temper[ptr[i_1]] < tmin) {
		//	tavg += potent_temper[ptr[i_1]];
		//}
	//}
	//tavg = tavg / maxelm;
	// ���������� ���� �������� �� ������ tmin ������� �������������� ���������� ���� �������.


    doublereal body_force=0.0; // �������� ���� ��������� � ����� ������� ��� �������� (�����������)
    // ��� Vx, Vy, Vz ��������� ������� �������� ����������� �� ���������� ����������� �����
	//doublereal tp1, tp2; // �������� �� ������� �������� ����������� �� ����������� �����.
	// ����� ����� ������� ��������� ���������� :
	// an explicit part of the tensor strain rate - exptsr
	doublereal exptsr=0.0;
	// ���� ����� ����� ������� ��������� ����������
	doublereal tsrE=0.0, tsrW=0.0, tsrP=0.0, tsrN=0.0, tsrS=0.0, tsrT=0.0, tsrB=0.0;

	//printf("dgx=%e dgy=%e dgz=%e\n",dgx,dgy,dgz);
	//getchar(); // debug; 


	/*
	// ������������ FlowVision.
	������ �������� ���� ������ �������������� ��� ������ ������, ������� ��� 
	����������� ������� ���������� ������������ ��������� ���������� ���
	��������������� ����. �� �������� ���� ����������� ��������� ����������� ��������� ����. 
	� �������� ��������� �������� ��������� � ����������� ��� ����������� �������� ����
	������������� ������������ ��������� �������� ��������� � �����������, ������� � ������
	������ ���� ����������� ������ � ������� ��������� � ������������ �������.

	�� �������� ����������������� ����������� ������� �������� ����, ���������� ���������
	����������� g � ����������� ���� ���������� ������� ��������� �� FlowVision.

	� ������� ���� ��� ����������� ������������ ���������� ���������� �������� ����������.
	���� ��������� ����������� ����� ���� (���������� ������������� �������), �� �������� 
	��� ���������, �.�. Tref � ������������ ��������� ����� ���������. ����� ��������� ���
	�������� ���� ������ ��� beta*(TEMP0-50) . ��� TEMP0 - ��� ���������� ��������� �����������
	������������ 50 �������� ������� (���� ������� ����� 273).
	*/

	switch (iVar) {
		case VX : 
                  // ������� �������� �������������:
			      /*
				  tp1=(1.0-feplus)*kP+feplus*kE;
				  tp2=(1.0-fwplus)*kP+fwplus*kW;

				  Bp+=(tp2-tp1)*dy*dz;
				  */

			      // ���� �������� ����� ����� ������� �� �� ����� 
		          // ������������� �������� ��������� ����� �������.
			      // �.�. �� ����� ������������� �������� �������� ��� 
			      // ������������ �������� �������� �������� ����� �������.
			      // ��� ��������� ��� ���� ����������� ����� �������.
			      // ������������ ���� ������ ���� ���������������, ������� �� ����
			      // ����� ��������. ��� �� �������� ���������� ��� ���� Oy && Oz. ��� 
			      // ��� ���� ����� ����� ����� ��������.
				  Bp-=potent[GRADXPRESS][iP]*dx*dy*dz;

				  //printf("%e",(tp2-tp1)*dy*dz);
				  //getchar();
				  // ����� ����� � �������� ����������!
				  if (bBussineskApproach) {
					  //printf("debug: rho: %e, beta=%e, dgx=%e, temp_ref=%e",prop[RHO][iP],prop[BETA_T][iP],dgx,temp_ref);
					  //getchar();
					  // ���������� ����
					   body_force = -prop[RHO][iP] * prop[BETA_T][iP] * dgx*(potent_temper[ptr[iP]] - temp_ref);
					
				  }
				  else 
				  {

					  //printf("no bussinesk: rho: %e, beta=%e, dgx=%e, temp_ref=%e",prop[RHO][iP],prop[BETA_T][iP],dgx,temp_ref);
					  //getchar();
					  // ���� �������
                      body_force=prop[RHO][iP]*dgx;
				  }
                  dSc+=body_force;

				  // ���� ����� ����� ������� ��������� ����������
				  tsrP=potent[GRADXVX][iP]-(2.0/3.0)*(potent[GRADXVX][iP]+potent[GRADYVY][iP]+potent[GRADZVZ][iP]); // ��������� 10 ������ 2013.
				  tsrE=potent[GRADXVX][iE]-(2.0/3.0)*(potent[GRADXVX][iE]+potent[GRADYVY][iE]+potent[GRADZVZ][iE]);
				  tsrW=potent[GRADXVX][iW]-(2.0/3.0)*(potent[GRADXVX][iW]+potent[GRADYVY][iW]+potent[GRADZVZ][iW]);
				  exptsr+=(Ge*(tsrP*(1.0-feplus)+tsrE*feplus)-Gw*(tsrP*(1.0-fwplus)+tsrW*fwplus))*dy*dz;
				  exptsr+=(Gn*(potent[GRADYVX][iP]*(1.0-fnplus)+potent[GRADYVX][iN]*fnplus)-Gs*(potent[GRADYVX][iP]*(1.0-fsplus)+potent[GRADYVX][iS]*fsplus))*dx*dz;
				  exptsr+=(Gt*(potent[GRADZVX][iP]*(1.0-ftplus)+potent[GRADZVX][iT]*ftplus)-Gb*(potent[GRADZVX][iP]*(1.0-fbplus)+potent[GRADZVX][iB]*fbplus))*dx*dy;
				 
				 

				  break;

        case VY : 
				  // ������� �������� �������������:
			      /*
				  tp1=(1.0-fnplus)*kP+fnplus*kN;
				  tp2=(1.0-fsplus)*kP+fsplus*kS;

				  Bp+=(tp2-tp1)*dx*dz;
				  */

				  Bp-=potent[GRADYPRESS][iP]*dx*dy*dz;

				  //printf("%e",(tp2-tp1)*dx*dz);
				  //getchar();
				  // ����� ����� � �������� ����������!
				  if (bBussineskApproach) {
					  // ���������� ����
					  // ��������� ������� ���������� �� ����� ������ �������.
					  // ��� �������� ����� �����.
                      body_force=-prop[RHO][iP]*prop[BETA_T][iP]*dgy*(potent_temper[ptr[iP]]-temp_ref);
				  }
				  else 
				  {
					  // ���� �������
                      body_force=prop[RHO][iP]*dgy;
				  }
                  dSc+=body_force; 

				  // ���� ����� ����� ������� ��������� ����������
				  tsrP=potent[GRADYVY][iP]-(2.0/3.0)*(potent[GRADXVX][iP]+potent[GRADYVY][iP]+potent[GRADZVZ][iP]); // ��������� 10 ������ 2013.
				  tsrN=potent[GRADYVY][iN]-(2.0/3.0)*(potent[GRADXVX][iN]+potent[GRADYVY][iN]+potent[GRADZVZ][iN]);
				  tsrS=potent[GRADYVY][iS]-(2.0/3.0)*(potent[GRADXVX][iS]+potent[GRADYVY][iS]+potent[GRADZVZ][iS]);
				  exptsr+=(Gn*(tsrP*(1.0-fnplus)+tsrN*fnplus)-Gs*(tsrP*(1.0-fsplus)+tsrS*fsplus))*dx*dz;
				  exptsr+=(Ge*(potent[GRADXVY][iP]*(1.0-feplus)+potent[GRADXVY][iE]*feplus)-Gw*(potent[GRADXVY][iP]*(1.0-fwplus)+potent[GRADXVY][iW]*fwplus))*dy*dz;
				  exptsr+=(Gt*(potent[GRADZVY][iP]*(1.0-ftplus)+potent[GRADZVY][iT]*ftplus)-Gb*(potent[GRADZVY][iP]*(1.0-fbplus)+potent[GRADZVY][iB]*fbplus))*dx*dy;
				  
				 

				  break;

		case VZ : 
                  // ������� �������� �������������:
			      /*
				  tp1=(1.0-ftplus)*kP+ftplus*kT;
				  tp2=(1.0-fbplus)*kP+fbplus*kB;

				  Bp+=(tp2-tp1)*dy*dx;
				  */

				  Bp-=potent[GRADZPRESS][iP]*dx*dy*dz;

				  //printf("%e",(tp2-tp1)*dy*dx);
				  //getchar();
				  // ����� ����� � �������� ����������!
				  if (bBussineskApproach) {
					  // ���������� ����
                      body_force=-prop[RHO][iP]*prop[BETA_T][iP]*dgz*(potent_temper[ptr[iP]]-temp_ref);
				  }
				  else 
				  {
					  // ���� �������
                      body_force=prop[RHO][iP]*dgz;
				  }
                  dSc+=body_force;

				  // ���� ����� ����� ������� ��������� ����������
				  tsrP=potent[GRADZVZ][iP]-(2.0/3.0)*(potent[GRADXVX][iP]+potent[GRADYVY][iP]+potent[GRADZVZ][iP]);
				  tsrT=potent[GRADZVZ][iT]-(2.0/3.0)*(potent[GRADXVX][iT]+potent[GRADYVY][iT]+potent[GRADZVZ][iT]);
				  tsrB=potent[GRADZVZ][iB]-(2.0/3.0)*(potent[GRADXVX][iB]+potent[GRADYVY][iB]+potent[GRADZVZ][iB]);
				  exptsr+=(Gt*(tsrP*(1.0-ftplus)+tsrT*ftplus)-Gb*(tsrP*(1.0-fbplus)+tsrB*fbplus))*dx*dy;
				  exptsr+=(Ge*(potent[GRADZVX][iP]*(1.0-feplus)+potent[GRADZVX][iE]*feplus)-Gw*(potent[GRADZVX][iP]*(1.0-fwplus)+potent[GRADZVX][iW]*fwplus))*dy*dz; // ��������� 10 ������ 2013.
				  exptsr+=(Gn*(potent[GRADZVY][iP]*(1.0-fnplus)+potent[GRADZVY][iN]*fnplus)-Gs*(potent[GRADZVY][iP]*(1.0-fsplus)+potent[GRADZVY][iS]*fsplus))*dx*dz;
				 
				
				 

				  break;
	    	default : dSc=0.0; break;
	    }

		// 10.02.2017 ����� ������� ��������� ���������� �������� �� �����������.
		


	    // ��� ������������� �������� Fe, Fw, Fn, Fs, Ft, Fb - ���������� �������� ������ ����� ���������� �������������� ���-���.
		// 02.05.2017
		// ��� ������� �.�. �������� � ������������� ������������ �������������.
		// ������ ���� ������� : deltaF=(Fe-Fw+Fn-Fs+Ft-Fb);
		// ����������� ����� ����������� � ���������� �� ANSYS Icepak.
		// ��� ����������� �� ���� �������� ����� �� ���������� ���� - ��� ���������� ������ ��������.
		//doublereal deltaF=(Fe-Fw+Fn-Fs+Ft-Fb);
		// ��� ������ ���������� ��������� ������������� ��� ��������� ����� ����.
		// � ������ ���� ����������� ��������� ��� �������� �������� �������� � ������������ ������ (��)
		// ��� ���������� ��������� ��������� ������������ ������������, �� ������ ���������� ������ ���������� ����� 
		// ����� ����� ���������� ��������� ���������.
		// 8.05.2017.
		doublereal deltaF=fabs(Fe-Fw+Fn-Fs+Ft-Fb);
		// 13 08 2016
		// � ��� ������ ������ �������, �.�. ��� ������ ��������� �� ������������ ������������.
		//doublereal deltaF = fabs(Fe - Fw) + fabs(Fn - Fs) + fabs(Ft - Fb);
		// �� ��� ����������� �������� ��� ���������� ������� �������� �������� �� ������� ������ �� ������� ���� ��� �������.
		// � ������ ������ ������ ������ ����� ��� �������� ���� ������ ������������. ��� ������� � ��� ��� ������������ ���� ������ �����������. � ������������ ���� �������� 
		// ������� ����� deltaF ������� ���������. ��� ����������� � ���������� deltaF ����� �� ������ � ������ � ��� ���������� ������� ��� �� ����� ��������� �������.
		// �� ������� �������� deltaF � ������������� ����� ������� � �� ������ ������  ����� ������� � ������ ����� deltaF ���������� deltaF*Folditer.
		// �� ���� � ��� ��� ���������� � ������ ����� �������� deltaF*Folditer �������� � ������������������ ����������� ������ ��������� �������, ������� 
		// ��� ��� ��� ������� � I.Sezai ������ ����� ��������� ����� ������ �� �������. �.�. I.Sezai ����������� ������ ������ �� ���������
		// � ������ �����. � ����� �� ������ ��������������� ��������� ������������ ���� � ��� ���� � ������ ����������. �� ������ ���������� � ��� � ��� ��� ������������ ������� ����� 
		// ������������� ��������� ��������� ����� (��� ���� � ��������� ������� � ���������) � ����� �� ���� ��������� �� ������� deltaF ������ (��� ���������� ��������).
		// ������� �� �������� ������ ����� ���������.
		// ������ ��� ������, ��. ������ �������� � ����� �� ���������� �����.
		sumanb+=deltaF;// 0.0 //fmax(0.0,deltaF); // ������ ���� ������ ���� ������� ������ ����� ���������.
		// deltaF - ������� ���� � ������� ���� ����� b-=Fold*deltaF; ���� ��������� ������������� �� ����������� �� ���� ���� ������������ 
		// ��� ������������ �������������.
		// �� �������� ���� ���� ���� ������ ������������� ��� ����� �������� � ������������.
		// ������ ������������ ��������� ���� �������� �������� �������.
		// ����� � ����� ������ �� ����������� ����������� ������� ��� � ������ ����� ������� ���������� �����
		// ������� �������� ��� �������� � ������������ ��� ������� ����.
		// ����� �� �������� ������ ��� ��� ������� ������, ��. ���������� ����. �������� ���� ������ ��� ���� �������
		// ������������� �����������. �����������: ap+=fmax(0.0,deltaF); ������ deltaF ������� ��� ������. � ����� ��� ������ ������� � ������.
		// ��. ��� ������� ���� ��� � ������ ����.
		// �������� ��� �������� �� ����� ���������� ���� deltaF �.�. ��� ����� ����� ������ ������ � ����������, � deltaF ��� ������ ������� ������������ ��������.
		// deltaF ���� � �������������� ������ ����������, �� ������ ���������� ��� ���������� � ����� � ������������� alpha==0.7 ��� ��������.
		// �������� deltaF ������� � �������������� ������ ���������� � ���������� ������� �������� alpha, � �� alpha ������� ����������� � ���� �������� � �����.
		// ��������� ����� ������������� � ���������� ������� ���� ������ ����� ������� ������� ����� � ������.
		// �� �������� ���� ��������� � ��������� deltaF �� deltaF ����� ��������� � � sumanb ��� ��� ����� ���������� �� �������������.
        sl[iVar][iP].ap+=deltaF;//-->//sl[iVar][iP].ap+=apzero1+deltaF;//+deltaF; // ������������ ������� ������� deltaF ������ ��������������.  ���������� ��������� 
		// ���� � �������� ������ ����������.
		// ��������� ������ ������� � ������ ���� deltaF ������ ����� ������� �� ������. 
	    //sl[iVar][iP].b=Bp+deltaF*Folditer;//Bp+apzero0*Fold+deltaF*Folditer;//+dSc*dx*dy*dz+exptsr+apzero0*Fold;//+attrs;//+deltaF*Folditer; // �������� ! ��� �� �������� �������� ��. I.Sezai. 
		sl[iVar][iP].b=Bp; // ��� ���������� ���������� �������.
		// �� ����� ���������� ���� Fold*(Fe-Fw+Fn-Fs+Ft-Fb) �� ������ �� ���������� ��� �������� ��������� ����������.
		// �������� ������� �������� Bp �� ���������� �� �����!!!
		if (!bVERYStable) {
           // sl[iVar][iP].ap+=-dSp*dx*dy*dz;
			//sl[iVar][iP].b+=dSc*dx*dy*dz+exptsr+attrs;
		}

		sl[iVar][iP].b+=exptsr+attrs; // ������ ����� ������� ��� ������� ��������� ���������� � ����� ����������� ��������� ��� ����� ������� ����������� �����������.
		sl[iVar][iP].b+=dSc*dx*dy*dz; // ���������� ���� ��������. (���� ���� ������, ������ �����-������.)

		if (btimedep) {
			// �������������� ������.
			sl[iVar][iP].ap+=apzero1;
			sumanb+=apzero1;
			sl[iVar][iP].b+=apzero0*Fold;
		}

		
	    // ������������� ������� ����:
// ��������� ���� ����������� ������ ���������� � ������ �������.
// � ������ ������ ���������� ������������� ��������� � SPD ��������.

// ������ ������� �������� �������� ��������� �������:
// -ab ... -as ... -aw ... +ap ... -ae ... -an ... -at == b

/*

// ���� ������� ������� �������:
if ((iE>maxelm) && (slb[iVar][iE-maxelm].iI==(-1))) {
sl[iVar][iP].b+=sl[iVar][iP].ae*slb[iVar][iE-maxelm].b/slb[iVar][iE-maxelm].aw;
sl[iVar][iP].ae=0.0;
sl[iVar][iP].iE=-1;
}

if ((iW>maxelm) && (slb[iVar][iW-maxelm].iI==(-1))) {
sl[iVar][iP].b+=sl[iVar][iP].aw*slb[iVar][iW-maxelm].b/slb[iVar][iW-maxelm].aw;
sl[iVar][iP].aw=0.0;
sl[iVar][iP].iW=-1;
}

if ((iN>maxelm) && (slb[iVar][iN-maxelm].iI==(-1))) {
sl[iVar][iP].b+=sl[iVar][iP].an*slb[iVar][iN-maxelm].b/slb[iVar][iN-maxelm].aw;
sl[iVar][iP].an=0.0;
sl[iVar][iP].iN=-1;
}

if ((iS>maxelm) && (slb[iVar][iS-maxelm].iI==(-1))) {
sl[iVar][iP].b+=sl[iVar][iP].as*slb[iVar][iS-maxelm].b/slb[iVar][iS-maxelm].aw;
sl[iVar][iP].as=0.0;
sl[iVar][iP].iS=-1;
}

if ((iT>maxelm) && (slb[iVar][iT-maxelm].iI==(-1))) {
sl[iVar][iP].b+=sl[iVar][iP].at*slb[iVar][iT-maxelm].b/slb[iVar][iT-maxelm].aw;
sl[iVar][iP].at=0.0;
sl[iVar][iP].iT=-1;
}

if ((iB>maxelm) && (slb[iVar][iB-maxelm].iI==(-1))) {
sl[iVar][iP].b+=sl[iVar][iP].ab*slb[iVar][iB-maxelm].b/slb[iVar][iB-maxelm].aw;
sl[iVar][iP].ab=0.0;
sl[iVar][iP].iB=-1;
}

*/

		if (fabs(sl[iVar][iP].ap) < 1.0e-30) {
			sl[iVar][iP].ap = 1.0;
			switch (iVar) {
			case VX: sl[iVar][iP].b = potent[VXCOR][iP]; break;
			case VY: sl[iVar][iP].b = potent[VYCOR][iP]; break;
			case VZ: sl[iVar][iP].b = potent[VZCOR][iP]; break;
			}
		}

} // my_elmatr_quad_F3D


// ���� ��������� ������� ��� 
// ��������� ����������������
// 25.09.2016 ������ ������������ � �������� � ��� ���� ����� ����.
void my_elmatr_quad_T3D_bound(integer inumber, integer maxbound, integer maxelm,
	BOUND* sosedb, integer ls, integer lw, WALL* w, SOURCE* s,
	equation3D_bon* &slb, bool bDirichlet, doublereal dbeta,
	integer** nvtx, TOCHKA* pa, doublereal** prop, doublereal** prop_b,
	doublereal* potent, doublereal* potent_old, integer** ptr, FLOW* &f,
	doublereal poweron_multiplier_sequence) {

	bool bsc1 = false;

	// ��� opening �������.
	 doublereal tolerance_input = -1.0e-5; // ����������� ������������� ��������� �� ������ ��������.
	 // �� ���� opening ������� ���������� ������� �������.
	//tolerance_input = -1.0e30;


	// � ������� poweron_multiplier_sequence == 0.0 �������� ����� ��������� !.
	// bDirichlet == true �������������� ������ ������ ��������� ������� �������.
	// bDirichlet == false �������������� ������ ������ ������������ ��� ���������� ������� �������.
	
	if (qnbc == NULL) {
		qnbc = new QuickNonlinearBoundaryCondition[maxbound+1];
		iadd_qnbc_maxelm = maxelm;
		// initialization
		for (integer i24 = 0; i24 < maxbound + 1; i24++) {
			qnbc[i24].bactive = false;
			qnbc[i24].bStefanBolcman_q_on = false;
			qnbc[i24].emissivity = 0.0001;
			qnbc[i24].bNewtonRichman_q_on = false;
			qnbc[i24].film_coefficient = 0.0;
			qnbc[i24].Tamb = 20.0;
			qnbc[i24].dS = 0.0;
		}
	}

	if (bvacuumPrism) {
		breakRUMBAcalc_for_nonlinear_boundary_condition = true;
	}

	/*
	if ((inumber==242-maxelm)||(inumber==245-maxelm)||(inumber==223-maxelm)||(inumber==220-maxelm)) {
	getchar();
	}
	*/

	bool bopening_Neiman_on = false;
	if (((sosedb[inumber].MCB < (ls + lw)) && (sosedb[inumber].MCB >= ls) && (w[sosedb[inumber].MCB - ls].ifamily == 1)) && (w[sosedb[inumber].MCB - ls].bopening)) {
		// opening ������� ��������� �������. 
		doublereal rsign = 1.0;

		

		// ����������� ����� �������� rsign:
		// ���������� ������� �� �������� ������� ��������� �������.
		// �������� �������� ������������� ����� ������� �� ��������� ���������� ����� ��������� �������,
		// �.�. ��������� t.ptr ������������ ������ ������ ������� ��������� ������ ��� ���������� ��, � �� ���������.
		// �� ����� �������, ��� �������� �������� ������������� � �������� � ���������� ����������� ���� ���� ������� �����������
		// ��� �������� �� opening �������.
		if ((ptr != NULL) && (ptr[1][sosedb[inumber].iI] != -1)) {
			switch (sosedb[inumber].Norm) {
			case ESIDE: 
				if (f[0].potent[VX][ptr[0][sosedb[inumber].iI]] > tolerance_input) {
					// �������� ������� ������ ��������� ������� ����� �������� �������.
					rsign = -1.0;
				}
				else rsign = 1.0;			
					break;
			case WSIDE:  
				if (f[0].potent[VX][ptr[0][sosedb[inumber].iI]] < fabs(tolerance_input)) {
					// �������� ������� ������ ��������� ������� ����� �������� �������.
					rsign = -1.0;
				}
				else rsign = 1.0;			
					 break;
			case NSIDE:  
				if (f[0].potent[VY][ptr[0][sosedb[inumber].iI]] > tolerance_input) {
					// �������� ������� ������ ��������� ������� ����� �������� �������.
					rsign = -1.0;
				}
				else rsign = 1.0;			
						 break;
			case SSIDE: 
				if (f[0].potent[VY][ptr[0][sosedb[inumber].iI]] < fabs(tolerance_input)) {
					// �������� ������� ������ ��������� ������� ����� �������� �������.
					rsign = -1.0;
				}
				else rsign = 1.0;			
						break;
			case TSIDE:
				if (f[0].potent[VZ][ptr[0][sosedb[inumber].iI]] > tolerance_input) {
					// �������� ������� ������ ��������� ������� ����� �������� �������.
					rsign = -1.0;
				}
				else rsign = 1.0;
				break;
			case BSIDE: if (f[0].potent[VZ][ptr[0][sosedb[inumber].iI]] < fabs(tolerance_input)) {
				     // �������� ������� ������ ��������� ������� ����� �������� �������.
				     rsign = -1.0;
			     }
				 else rsign = 1.0;
				 break;
			} // end switch
		}
		if (rsign >= 0.0) {
			// �������� �������� �� �������� �������.
			bopening_Neiman_on = true;
		}
	}

	/*
	if (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 0) {
		if ((((sosedb[inumber].MCB < (ls + lw)) && (sosedb[inumber].MCB >= ls) && (w[sosedb[inumber].MCB - ls].ifamily == 4)))) {
			breakRUMBAcalc_for_nonlinear_boundary_condition = true;
			//getchar();
		}
	}
	*/

	// inumber - ����� ���������� ��.
	// inumber ���������� �� 0..maxbound-1
	if (bDirichlet && (((sosedb[inumber].MCB < (ls + lw)) && (sosedb[inumber].MCB >= ls) && (w[sosedb[inumber].MCB - ls].ifamily == 1)) || (bBlockStefanBolcman&&(
		(((sosedb[inumber].MCB < (ls + lw)) && (sosedb[inumber].MCB >= ls) && (w[sosedb[inumber].MCB - ls].ifamily == 4))))))) {

		

		if ((bBlockStefanBolcman && (
			(((sosedb[inumber].MCB < (ls + lw)) && (sosedb[inumber].MCB >= ls) && (w[sosedb[inumber].MCB - ls].ifamily == 4)))))) {
			breakRUMBAcalc_for_nonlinear_boundary_condition = true;
			// ��������� ������� �������:
			// �������� ����������� �� �������.

#if doubleintprecision == 1
			//printf("Dirichlet %lld T=%e\n",inumber, potent[inumber + maxelm]); // debug
#else
			//printf("Dirichlet %d  T=%e\n",inumber, potent[inumber + maxelm]); // debug
#endif
			
			//getchar();

			// ����������� �� ������� ������ � ����� Tamb:
			slb[inumber].aw = 1.0;
			slb[inumber].ai = 0.0;
			//slb[inumber].b = w[sosedb[inumber].MCB - ls].Tamb; // ������� ������� ���������� ����������� �� ��� � ������� �������� - ���������.
			slb[inumber].b = potent[inumber + maxelm];
			slb[inumber].iI = -1; // �� ������������ � �������
			slb[inumber].iW = sosedb[inumber].iB;
		}
		else {
			// ��������� ������� �������:
			// �������� ����������� �� �������.

#if doubleintprecision == 1
			//printf("Dirichlet %lld\n",inumber); // debug
#else
			//printf("Dirichlet %d\n",inumber); // debug
#endif
			
			//getchar();

			if (((sosedb[inumber].MCB < (ls + lw)) && (sosedb[inumber].MCB >= ls) && (w[sosedb[inumber].MCB - ls].ifamily == 1))&&(w[sosedb[inumber].MCB - ls].bopening)) {
				// opening ������� ��������� �������. 
				doublereal rsign = 1.0;

				// ����������� ����� �������� rsign:
				// ���������� ������� �� �������� ������� ��������� �������.
				// �������� �������� ������������� ����� ������� �� ��������� ���������� ����� ��������� �������,
				// �.�. ��������� t.ptr ������������ ������ ������ ������� ��������� ������ ��� ���������� ��, � �� ���������.
				// �� ����� �������, ��� �������� �������� ������������� � �������� � ���������� ����������� ���� ���� ������� �����������
				// ��� �������� �� opening �������.
				if ((ptr != NULL) && (ptr[1][sosedb[inumber].iI] != -1)) {
					switch (sosedb[inumber].Norm) {
					case ESIDE: if (f[0].potent[VX][ptr[0][sosedb[inumber].iI]] > tolerance_input) {
						// �������� ������� ������ ��������� ������� ����� �������� �������.
						rsign = -1.0;
					}
							else rsign = 1.0;
							break;
					case WSIDE: if (f[0].potent[VX][ptr[0][sosedb[inumber].iI]] < fabs(tolerance_input)) {
						// �������� ������� ������ ��������� ������� ����� �������� �������.
						rsign = -1.0;
					}
							else rsign = 1.0;
							break;
					case NSIDE: if (f[0].potent[VY][ptr[0][sosedb[inumber].iI]] > tolerance_input) {
						// �������� ������� ������ ��������� ������� ����� �������� �������.
						rsign = -1.0;
					}
								else rsign = 1.0;
								break;
					case SSIDE:if (f[0].potent[VY][ptr[0][sosedb[inumber].iI]] < fabs(tolerance_input)) {
						// �������� ������� ������ ��������� ������� ����� �������� �������.
						rsign = -1.0;
					}
							   else rsign = 1.0;
							   break;
					case TSIDE: if (f[0].potent[VZ][ptr[0][sosedb[inumber].iI]] > tolerance_input) {
						// �������� ������� ������ ��������� ������� ����� �������� �������.
						rsign = -1.0;
					}
								else rsign = 1.0;
								break;
					case BSIDE: if (f[0].potent[VZ][ptr[0][sosedb[inumber].iI]] < fabs(tolerance_input)) {
						// �������� ������� ������ ��������� ������� ����� �������� �������.
						rsign = -1.0;
					}
								else rsign = 1.0;
								break;
					} // end switch
				}
				  // ���������� ����������� �� ������� ��������� �������:
				if (rsign < 0.0) {
					// �������� ������� ������  ��������� ������� � ������� ������������.
					// ������� ������� ��� ���� ���������� 
					// ��� �������� ������� � ��������� �������.
					slb[inumber].aw = 1.0;
					slb[inumber].ai = 0.0;
					slb[inumber].b = w[sosedb[inumber].MCB - ls].Tamb;
					slb[inumber].iI = -1; // �� ������������ � �������
					slb[inumber].iW = sosedb[inumber].iB;
					// ����� ��������.
					//printf("vtekaet Tamb=%e\n", w[sosedb[inumber].MCB - ls].Tamb);
					//getchar();
				}
			}
			else {

				// ����������� �� ������� ������ � ����� Tamb:
				slb[inumber].aw = 1.0;
				slb[inumber].ai = 0.0;
				slb[inumber].b = w[sosedb[inumber].MCB - ls].Tamb;
				slb[inumber].iI = -1; // �� ������������ � �������
				slb[inumber].iW = sosedb[inumber].iB;
			}

		}
		
		// ��� ������� �������:
		// ������ ������������ �������
		// �� ����� ����.
		slb[inumber].iW1 = -1;
		slb[inumber].iW2 = -1;
		slb[inumber].iW3 = -1;
		slb[inumber].iW4 = -1;
	}
	else if (!bDirichlet && ((sosedb[inumber].MCB == (ls + lw)) || (sosedb[inumber].MCB < ls) || bopening_Neiman_on || ((sosedb[inumber].MCB < (ls + lw)) && (sosedb[inumber].MCB >= ls) && (w[sosedb[inumber].MCB - ls].ifamily != 1)))) {
		// ���������� ������� ������� ��� ������������ ������� �������.
		// ���� ������� �� ���������, ���� �������� �����, ���� ������ � ���������� �������� �������

		doublereal qb = 0.0; // �������� ����� �� ������.
		// ��������� �������� ����� ����� ��������������� ������ ��������� �����.
		// �� ������ WALL ����� ���� ����� ������ ������� �������� �����.
		// �������� ����� qb == �������� ��������� / ������� ���������.
		bool bcontinue = true;

		if (!bopening_Neiman_on) {

			// The Stefan - Bolcman condition 23.07.2016.
			if (((sosedb[inumber].MCB < (ls + lw)) && (sosedb[inumber].MCB >= ls) && (w[sosedb[inumber].MCB - ls].ifamily == 4))) {

				breakRUMBAcalc_for_nonlinear_boundary_condition = true;

				if (bBlockStefanBolcman) {
					bcontinue = false;
					//printf("bcontinue==false\n");
					//system("PAUSE");
				}
				else {

					doublereal alpha_relax1 = 0.25;
					if (bvacuumPrism) {
						//alpha_relax1 = 8.0*my_amg_manager.theta_Pressure;
					}
					// blocker_Newton_Richman �� ����� ��� ��� �� �������� ���������� ������� ������� �� ������ �������,
					// �� ����������� �� �������� ��������� ���������� ������� ��� ��� �� ������ ����� 5 V - ������.
					/*
					if (potent[sosedb[inumber].iB] > w[sosedb[inumber].MCB - ls].Tamb) {
						qb = alpha_relax1 *(-w[sosedb[inumber].MCB - ls].emissivity*5.670367e-8*(( potent[sosedb[inumber].iB])*( potent[sosedb[inumber].iB])*( potent[sosedb[inumber].iB])*( potent[sosedb[inumber].iB]) - ( w[sosedb[inumber].MCB - ls].Tamb)*( w[sosedb[inumber].MCB - ls].Tamb)*( w[sosedb[inumber].MCB - ls].Tamb)*( w[sosedb[inumber].MCB - ls].Tamb))) +
							(1.0 - alpha_relax1)*(-w[sosedb[inumber].MCB - ls].emissivity*5.670367e-8*(( potent_old[sosedb[inumber].iB])*(potent_old[sosedb[inumber].iB])*( potent_old[sosedb[inumber].iB])*( potent_old[sosedb[inumber].iB]) - (w[sosedb[inumber].MCB - ls].Tamb)*( w[sosedb[inumber].MCB - ls].Tamb)*( w[sosedb[inumber].MCB - ls].Tamb)*(w[sosedb[inumber].MCB - ls].Tamb)));
					}
					else {
						//qb = 0.0;
						// ������� ������� ���������� � ���� ������.
						qb = -w[sosedb[inumber].MCB - ls].emissivity*5.670367e-8*((1+ w[sosedb[inumber].MCB - ls].Tamb)*(1+ w[sosedb[inumber].MCB - ls].Tamb)*(1 + w[sosedb[inumber].MCB - ls].Tamb)*(1 + w[sosedb[inumber].MCB - ls].Tamb) - (1 + w[sosedb[inumber].MCB - ls].Tamb)*(1 + w[sosedb[inumber].MCB - ls].Tamb)*(1+ w[sosedb[inumber].MCB - ls].Tamb)*(1 + w[sosedb[inumber].MCB - ls].Tamb));
					}
					*/
					if (potent[sosedb[inumber].iB] > w[sosedb[inumber].MCB - ls].Tamb) {

						if (potent[sosedb[inumber].iB] < -272.15) {
							potent[sosedb[inumber].iB] = -272.15;
						}
						qnbc[inumber].bactive = true;
						qnbc[inumber].bStefanBolcman_q_on = true;
						qnbc[inumber].emissivity = w[sosedb[inumber].MCB - ls].emissivity;
						qnbc[inumber].Tamb = w[sosedb[inumber].MCB - ls].Tamb;
						bsc1 = true;
						//alpha_relax1 = 1.0;
						qb = alpha_relax1 *(-w[sosedb[inumber].MCB - ls].emissivity*5.670367e-8*((273.15 + potent[sosedb[inumber].iB])*(273.15 + potent[sosedb[inumber].iB])*(273.15 + potent[sosedb[inumber].iB])*(273.15 + potent[sosedb[inumber].iB]) - (273.15 + w[sosedb[inumber].MCB - ls].Tamb)*(273.15 + w[sosedb[inumber].MCB - ls].Tamb)*(273.15 + w[sosedb[inumber].MCB - ls].Tamb)*(273.15 + w[sosedb[inumber].MCB - ls].Tamb))) +
							(1.0 - alpha_relax1)*(-w[sosedb[inumber].MCB - ls].emissivity*5.670367e-8*((273.15 + potent_old[sosedb[inumber].iB])*(273.15 + potent_old[sosedb[inumber].iB])*(273.15 + potent_old[sosedb[inumber].iB])*(273.15 + potent_old[sosedb[inumber].iB]) - (273.15 + w[sosedb[inumber].MCB - ls].Tamb)*(273.15 + w[sosedb[inumber].MCB - ls].Tamb)*(273.15 + w[sosedb[inumber].MCB - ls].Tamb)*(273.15 + w[sosedb[inumber].MCB - ls].Tamb)));
						//printf("%e qb=%e\n", potent[sosedb[inumber].iB], qb);
						//system("PAUSE");
					}
					else {
						qb = 0.0;
						//printf("qb zero\n");
						//system("PAUSE");
						// ������� ������� ���������� � ���� ������.
						//qb = -w[sosedb[inumber].MCB - ls].emissivity*5.670367e-8*((274.15 + w[sosedb[inumber].MCB - ls].Tamb)*(274.15 + w[sosedb[inumber].MCB - ls].Tamb)*(274.15 + w[sosedb[inumber].MCB - ls].Tamb)*(274.15 + w[sosedb[inumber].MCB - ls].Tamb) - (273.15 + w[sosedb[inumber].MCB - ls].Tamb)*(273.15 + w[sosedb[inumber].MCB - ls].Tamb)*(273.15 + w[sosedb[inumber].MCB - ls].Tamb)*(273.15 + w[sosedb[inumber].MCB - ls].Tamb));
					}
				}
			}

		}
		

		if (bcontinue) {
			
			if (!bopening_Neiman_on) {

				if (((sosedb[inumber].MCB < (ls + lw)) && (sosedb[inumber].MCB >= ls) && (w[sosedb[inumber].MCB - ls].ifamily == 3))) {
					breakRUMBAcalc_for_nonlinear_boundary_condition = true;


					qnbc[inumber].bactive = true;
					qnbc[inumber].bNewtonRichman_q_on = true;
					qnbc[inumber].film_coefficient = w[sosedb[inumber].MCB - ls].film_coefficient;
					qnbc[inumber].Tamb = w[sosedb[inumber].MCB - ls].Tamb;
					bsc1 = true;
					// blocker_Newton_Richman �� ����� ��� ��� �� �������� ���������� ������� ������� �� ������ �������,
					// �� ����������� �� �������� ��������� ���������� ������� ��� ��� �� ������ ����� 5 V - ������. 
					qb = -w[sosedb[inumber].MCB - ls].film_coefficient*(potent[sosedb[inumber].iB] - w[sosedb[inumber].MCB - ls].Tamb);

				}

				// ������� �������-�������. ��� �������� ����������, �.�. �������� ��������� ������
				// � ������ ��������� ������� ������� �� ����������� ����������� � ��������� ����.
				if ((sosedb[inumber].MCB == (ls + lw)) && (adiabatic_vs_heat_transfer_coeff == 1)) {
					if (blocker_Newton_Richman) {
						qnbc[inumber].bactive = true;
						qnbc[inumber].bNewtonRichman_q_on = true;
						qnbc[inumber].film_coefficient = film_coefficient;
						qnbc[inumber].Tamb = operating_temperature_for_film_coeff;
						bsc1 = true;
						// ������ ������� ������������ ��������� ������� ���������� ��� ������ ������� ������� �������-�������,
						// ��������� ������� ������ 20�������� ��� ���� ���������� ���������� ������ � ���������� ������� ��� 
						// ������� ��� ������� ���������� ���� ���������� ����� ���������� ��������.
						qb = -film_coefficient*(potent[sosedb[inumber].iB] - operating_temperature_for_film_coeff);
					}
					else {
						qnbc[inumber].bactive = true;
						qnbc[inumber].bNewtonRichman_q_on = true;
						qnbc[inumber].film_coefficient = film_coefficient;
						qnbc[inumber].Tamb = operating_temperature_for_film_coeff;
						bsc1 = true;
						qb = -film_coefficient*(potent[sosedb[inumber].iB] - operating_temperature_for_film_coeff);
					}
				}

				// ������� �������-���������. ��� �������� ����������, �.�. �������� ��������� ������
				// � ������ ��������� ������� ������� �� ����������� ����������� � ��������� ����.
				if ((sosedb[inumber].MCB == (ls + lw)) && (adiabatic_vs_heat_transfer_coeff == 2)) {
					// sosedb[inumber].emissivity ������ �������� ������������� ����������� �� �������.

					doublereal alpha_relax1 = 0.25; // ����������� ������ ���������� � ������ ��������� �������.
					if (bvacuumPrism) {
						//alpha_relax1 = 2.0*my_amg_manager.theta_Pressure;
					}
					/*
					if (blocker_Newton_Richman) {
						// ������ ������� ������������ ��������� ������� ���������� ��� ������ ������� ������� �������-���������,
						// ��������� ������� ������ 20�������� ��� ���� ���������� ���������� ������ � ���������� ������� ���
						// ������� ��� ������� ���������� ���� ���������� ����� ���������� ��������.
						if (potent[sosedb[inumber].iB] > operating_temperature_for_film_coeff) {
							qb = alpha_relax1 *(-sosedb[inumber].emissivity*5.670367e-8*(( potent[sosedb[inumber].iB])*( potent[sosedb[inumber].iB])*( potent[sosedb[inumber].iB])*( potent[sosedb[inumber].iB]) - ( operating_temperature_for_film_coeff)*( operating_temperature_for_film_coeff)*( operating_temperature_for_film_coeff)*( operating_temperature_for_film_coeff))) +
								(1.0 - alpha_relax1)*(-sosedb[inumber].emissivity*5.670367e-8*(( potent_old[sosedb[inumber].iB])*( potent_old[sosedb[inumber].iB])*( potent_old[sosedb[inumber].iB])*( potent_old[sosedb[inumber].iB]) - (operating_temperature_for_film_coeff)*( operating_temperature_for_film_coeff)*( operating_temperature_for_film_coeff)*( operating_temperature_for_film_coeff)));
						}
						else {
							qb = 0.0;
						}
					}
					else {
						if (potent[sosedb[inumber].iB] > operating_temperature_for_film_coeff) {
							qb = alpha_relax1*(-sosedb[inumber].emissivity*5.670367e-8*(( potent[sosedb[inumber].iB])*( potent[sosedb[inumber].iB])*( potent[sosedb[inumber].iB])*( potent[sosedb[inumber].iB]) - ( operating_temperature_for_film_coeff)*(operating_temperature_for_film_coeff)*( operating_temperature_for_film_coeff)*(operating_temperature_for_film_coeff))) +
								(1.0 - alpha_relax1)*(-sosedb[inumber].emissivity*5.670367e-8*(( potent_old[sosedb[inumber].iB])*( potent_old[sosedb[inumber].iB])*( potent_old[sosedb[inumber].iB])*(potent_old[sosedb[inumber].iB]) - ( operating_temperature_for_film_coeff)*( operating_temperature_for_film_coeff)*( operating_temperature_for_film_coeff)*( operating_temperature_for_film_coeff)));
						}
						else {
							qb = 0.0;
						}
					}*/
					if (blocker_Newton_Richman) {
						// ������ ������� ������������ ��������� ������� ���������� ��� ������ ������� ������� �������-���������,
						// ��������� ������� ������ 20�������� ��� ���� ���������� ���������� ������ � ���������� ������� ��� 
						// ������� ��� ������� ���������� ���� ���������� ����� ���������� ��������.
						if (potent[sosedb[inumber].iB] > operating_temperature_for_film_coeff) {

							if (potent[sosedb[inumber].iB] < -272.15) {
								potent[sosedb[inumber].iB] = -272.15;
							}
							qnbc[inumber].bactive = true;
							qnbc[inumber].bStefanBolcman_q_on = true;
							qnbc[inumber].emissivity = sosedb[inumber].emissivity;
							qnbc[inumber].Tamb = operating_temperature_for_film_coeff;
							bsc1 = true;
							qb = alpha_relax1 *(-sosedb[inumber].emissivity*5.670367e-8*((273.15 + potent[sosedb[inumber].iB])*(273.15 + potent[sosedb[inumber].iB])*(273.15 + potent[sosedb[inumber].iB])*(273.15 + potent[sosedb[inumber].iB]) - (273.15 + operating_temperature_for_film_coeff)*(273.15 + operating_temperature_for_film_coeff)*(273.15 + operating_temperature_for_film_coeff)*(273.15 + operating_temperature_for_film_coeff))) +
								(1.0 - alpha_relax1)*(-sosedb[inumber].emissivity*5.670367e-8*((273.15 + potent_old[sosedb[inumber].iB])*(273.15 + potent_old[sosedb[inumber].iB])*(273.15 + potent_old[sosedb[inumber].iB])*(273.15 + potent_old[sosedb[inumber].iB]) - (273.15 + operating_temperature_for_film_coeff)*(273.15 + operating_temperature_for_film_coeff)*(273.15 + operating_temperature_for_film_coeff)*(273.15 + operating_temperature_for_film_coeff)));
						}
						else {
							qb = 0.0;
						}
					}
					else {
						if (potent[sosedb[inumber].iB] > operating_temperature_for_film_coeff) {

							if (potent[sosedb[inumber].iB] < -272.15) {
								potent[sosedb[inumber].iB] = -272.15;
							}
							qnbc[inumber].bactive = true;
							qnbc[inumber].bStefanBolcman_q_on = true;
							qnbc[inumber].emissivity = sosedb[inumber].emissivity;
							qnbc[inumber].Tamb = operating_temperature_for_film_coeff;
							qb = alpha_relax1*(-sosedb[inumber].emissivity*5.670367e-8*((273.15 + potent[sosedb[inumber].iB])*(273.15 + potent[sosedb[inumber].iB])*(273.15 + potent[sosedb[inumber].iB])*(273.15 + potent[sosedb[inumber].iB]) - (273.15 + operating_temperature_for_film_coeff)*(273.15 + operating_temperature_for_film_coeff)*(273.15 + operating_temperature_for_film_coeff)*(273.15 + operating_temperature_for_film_coeff))) +
								(1.0 - alpha_relax1)*(-sosedb[inumber].emissivity*5.670367e-8*((273.15 + potent_old[sosedb[inumber].iB])*(273.15 + potent_old[sosedb[inumber].iB])*(273.15 + potent_old[sosedb[inumber].iB])*(273.15 + potent_old[sosedb[inumber].iB]) - (273.15 + operating_temperature_for_film_coeff)*(273.15 + operating_temperature_for_film_coeff)*(273.15 + operating_temperature_for_film_coeff)*(273.15 + operating_temperature_for_film_coeff)));
						}
						else {
							qb = 0.0;
						}
					}
				}

				// ������� �������-������� ��������� � �������� �������-���������. ��� �������� ����������, �.�. �������� ��������� ������
				// � ������ ��������� ������� ������� �� ����������� ����������� � ��������� ����.
				if ((sosedb[inumber].MCB == (ls + lw)) && (adiabatic_vs_heat_transfer_coeff == 3)) {
					/*
					if (blocker_Newton_Richman) {
						// ������ ������� ������������ ��������� ������� ���������� ��� ������ ������� ������� �������-�������,
						// ��������� ������� ������ 20�������� ��� ���� ���������� ���������� ������ � ���������� ������� ���
						// ������� ��� ������� ���������� ���� ���������� ����� ���������� ��������.
						qb = -film_coefficient*(potent[sosedb[inumber].iB] - operating_temperature_for_film_coeff);
						qb = qb - sosedb[inumber].emissivity*5.670367e-8*(( potent[sosedb[inumber].iB])*( potent[sosedb[inumber].iB])*( potent[sosedb[inumber].iB])*( potent[sosedb[inumber].iB]) - ( operating_temperature_for_film_coeff)*( operating_temperature_for_film_coeff)*( operating_temperature_for_film_coeff)*(operating_temperature_for_film_coeff));

					}
					else {
						qb = -film_coefficient*(potent[sosedb[inumber].iB] - operating_temperature_for_film_coeff);
						qb = qb - sosedb[inumber].emissivity*5.670367e-8*(( potent[sosedb[inumber].iB])*( potent[sosedb[inumber].iB])*( potent[sosedb[inumber].iB])*( potent[sosedb[inumber].iB]) - ( operating_temperature_for_film_coeff)*( operating_temperature_for_film_coeff)*( operating_temperature_for_film_coeff)*( operating_temperature_for_film_coeff));
					}
					*/
					if (blocker_Newton_Richman) {



						// ������ ������� ������������ ��������� ������� ���������� ��� ������ ������� ������� �������-�������,
						// ��������� ������� ������ 20�������� ��� ���� ���������� ���������� ������ � ���������� ������� ��� 
						// ������� ��� ������� ���������� ���� ���������� ����� ���������� ��������.
						if (potent[sosedb[inumber].iB] < -272.15) {
							potent[sosedb[inumber].iB] = -272.15;
						}
						qnbc[inumber].bactive = true;
						qnbc[inumber].bStefanBolcman_q_on = true;
						qnbc[inumber].bNewtonRichman_q_on = true;
						qnbc[inumber].emissivity = sosedb[inumber].emissivity;
						qnbc[inumber].film_coefficient = film_coefficient;
						qnbc[inumber].Tamb = operating_temperature_for_film_coeff;
						bsc1 = true;
						qb = -film_coefficient*(potent[sosedb[inumber].iB] - operating_temperature_for_film_coeff);
						qb = qb - sosedb[inumber].emissivity*5.670367e-8*((273.15 + potent[sosedb[inumber].iB])*(273.15 + potent[sosedb[inumber].iB])*(273.15 + potent[sosedb[inumber].iB])*(273.15 + potent[sosedb[inumber].iB]) - (273.15 + operating_temperature_for_film_coeff)*(273.15 + operating_temperature_for_film_coeff)*(273.15 + operating_temperature_for_film_coeff)*(273.15 + operating_temperature_for_film_coeff));

					}
					else {
						// ������� ���������� ����.
						if (potent[sosedb[inumber].iB] < -272.15) {
							potent[sosedb[inumber].iB] = -272.15;
						}
						qnbc[inumber].bactive = true;
						qnbc[inumber].bStefanBolcman_q_on = true;
						qnbc[inumber].bNewtonRichman_q_on = true;
						qnbc[inumber].emissivity = sosedb[inumber].emissivity;
						qnbc[inumber].film_coefficient = film_coefficient;
						qnbc[inumber].Tamb = operating_temperature_for_film_coeff;
						bsc1 = true;
						qb = -film_coefficient*(potent[sosedb[inumber].iB] - operating_temperature_for_film_coeff);
						qb = qb - sosedb[inumber].emissivity*5.670367e-8*((273.15 + potent[sosedb[inumber].iB])*(273.15 + potent[sosedb[inumber].iB])*(273.15 + potent[sosedb[inumber].iB])*(273.15 + potent[sosedb[inumber].iB]) - (273.15 + operating_temperature_for_film_coeff)*(273.15 + operating_temperature_for_film_coeff)*(273.15 + operating_temperature_for_film_coeff)*(273.15 + operating_temperature_for_film_coeff));
					}
				}

				if (bsc1) {
					b_sign_on_nonlinear_bc = true;
				}

			}
			else {
				// opening ������� ����� ������� ������ �������� ��������� �������.
				// ��������� ������� �������� ��������, ���������� ���������� ������� ������� �� �����������.
				qnbc[inumber].bactive = false;
				qnbc[inumber].bStefanBolcman_q_on = false;
				qnbc[inumber].bNewtonRichman_q_on = false;
				qnbc[inumber].emissivity = 0.001;
				qnbc[inumber].film_coefficient = 0.001;
				qnbc[inumber].Tamb = f[0].OpTemp;
				qb = 0.0;
			}

			bool b1 = false;
			if (sosedb[inumber].MCB < ls) {
				// ����� ������ ������ ����������� (����� �� �����).
				// qb=poweron_multiplier_sequence*s[sosedb[inumber].MCB].power/s[sosedb[inumber].MCB].square;
				// b1=true;// ���������.
			}

			

			doublereal dl=0.0, dS=0.0; // �������������� ���������
			//doublereal deltal;
			//doublereal lami; // ���������������� �� ����� ��
			//doublereal fiplus; // ���� ��������������� �����

			doublereal lamB= 0.0, lamI=0.0, lamII=0.0; // ����������������

			//printf("qb=%e\n", qb);
			//system("PAUSE");

			// ���������� �������
			switch (sosedb[inumber].Norm) {
			case ESIDE:
				dl = pa[nvtx[1][sosedb[inumber].iI] - 1].x - pa[nvtx[0][sosedb[inumber].iI] - 1].x;
				//dS = pa[nvtx[2][sosedb[inumber].iI] - 1].y - pa[nvtx[1][sosedb[inumber].iI] - 1].y;
				//dS *= (pa[nvtx[4][sosedb[inumber].iI] - 1].z - pa[nvtx[0][sosedb[inumber].iI] - 1].z); // ������� �����
				dS = sosedb[inumber].dS; // ������� ����� 25.09.2016.

				// ����������� ����������������
				lamB = prop_b[LAM][sosedb[inumber].iB - maxelm];
				lamI = prop[LAM][sosedb[inumber].iI];
				if (sosedb[inumber].iII >= maxelm) {
					lamII = prop_b[LAM][sosedb[inumber].iII - maxelm];
				}
				else {
					lamII = prop[LAM][sosedb[inumber].iII];
				}

				// �������������.
				lamB *= prop[MULT_LAM_X][sosedb[inumber].iI];
				lamI *= prop[MULT_LAM_X][sosedb[inumber].iI];
				lamII *= prop[MULT_LAM_X][sosedb[inumber].iI];


				/*
				if ((ptr!=NULL) && (ptr[1][sosedb[inumber].iI]!=-1)) {
				lamB+=prop_b[CP][sosedb[inumber].iB-maxelm]*f[ptr[1][sosedb[inumber].iI]].potent[MUT][f[ptr[1][sosedb[inumber].iI]].sosedi[WSIDE][ptr[0][sosedb[inumber].iI]].iNODE1]/0.85;
				lamI+=prop[CP][sosedb[inumber].iI]*f[ptr[1][sosedb[inumber].iI]].potent[MUT][ptr[0][sosedb[inumber].iI]]/0.85;
				lamII+=prop[CP][sosedb[inumber].iII]*f[ptr[1][sosedb[inumber].iI]].potent[MUT][ptr[0][sosedb[inumber].iII]]/0.85;
				}
				*/

				// ���������������� �����������, ��� ���� ������ ���������������� ������� �� ������� � � ��������� ���������� ����,
				// ����� ����� �������� ���������� �����������. 4 ������ 2013 ����.
				// ������������ ����������� � ���������� ��������� �����������, �������� ����� �������� ����������� ������������� ����� �������� ��������� ��
				// ������� ����� �������� � ������ �����.
				// ������, ������������ ����� ������� ����� ��������� ��������� � �� ����������������� ������������.
				lamB = lamI;


				if (b1) {
					//  ������� ������������� �� �����.
					lamB = conductivity2Dinsource[inumber];
				}

				slb[inumber].ai = 2.0*dbeta*lamB*dS / dl;
				slb[inumber].iI = sosedb[inumber].iI;
				slb[inumber].aw = slb[inumber].ai;
				slb[inumber].iW = sosedb[inumber].iB;

				//deltal=0.5*(pa[nvtx[1][sosedb[inumber].iII]-1].x+pa[nvtx[0][sosedb[inumber].iII]-1].x);
				//deltal-=0.5*(pa[nvtx[1][sosedb[inumber].iI]-1].x+pa[nvtx[0][sosedb[inumber].iI]-1].x);
				//fiplus=0.5*dl/deltal;

				//lami=(lamI*lamII)/((1.0-fiplus)*lamI+fiplus*lamII); // ���������

				// ������ �����
				slb[inumber].b = 0;
				//slb[inumber].b=(dbeta-1.0)*lami*dS*(potent[sosedb[inumber].iI]-potent[sosedb[inumber].iII])/deltal;
				slb[inumber].b += qb*dS;

				break;

			case NSIDE:
				dl = pa[nvtx[2][sosedb[inumber].iI] - 1].y - pa[nvtx[0][sosedb[inumber].iI] - 1].y;
				//dS = pa[nvtx[1][sosedb[inumber].iI] - 1].x - pa[nvtx[0][sosedb[inumber].iI] - 1].x;
				//dS *= (pa[nvtx[4][sosedb[inumber].iI] - 1].z - pa[nvtx[0][sosedb[inumber].iI] - 1].z); // ������� �����
				dS = sosedb[inumber].dS; // ������� ����� 25.09.2016.

				// ����������� ����������������
				lamB = prop_b[LAM][sosedb[inumber].iB - maxelm];
				lamI = prop[LAM][sosedb[inumber].iI];
				if (sosedb[inumber].iII >= maxelm) {
					lamII = prop_b[LAM][sosedb[inumber].iII - maxelm];
				}
				else {
					lamII = prop[LAM][sosedb[inumber].iII];
				}

				// �������������.
				lamB *= prop[MULT_LAM_Y][sosedb[inumber].iI];
				lamI *= prop[MULT_LAM_Y][sosedb[inumber].iI];
				lamII *= prop[MULT_LAM_Y][sosedb[inumber].iI];

				/*
				if ((ptr!=NULL) && (ptr[1][sosedb[inumber].iI]!=-1)) {
				lamB+=prop_b[CP][sosedb[inumber].iB-maxelm]*f[ptr[1][sosedb[inumber].iI]].potent[MUT][f[ptr[1][sosedb[inumber].iI]].sosedi[SSIDE][ptr[0][sosedb[inumber].iI]].iNODE1]/0.85;
				lamI+=prop[CP][sosedb[inumber].iI]*f[ptr[1][sosedb[inumber].iI]].potent[MUT][ptr[0][sosedb[inumber].iI]]/0.85;
				lamII+=prop[CP][sosedb[inumber].iII]*f[ptr[1][sosedb[inumber].iI]].potent[MUT][ptr[0][sosedb[inumber].iII]]/0.85;
				}
				*/

				// ���������������� �����������, ��� ���� ������ ���������������� ������� �� ������� � � ��������� ���������� ����,
				// ����� ����� �������� ���������� �����������. 4 ������ 2013 ����.
				lamB = lamI;

				if (b1) {
					//  ������� ������������� �� �����.
					lamB = conductivity2Dinsource[inumber];
				}

				slb[inumber].ai = 2.0*dbeta*lamB*dS / dl;
				slb[inumber].iI = sosedb[inumber].iI;
				slb[inumber].aw = slb[inumber].ai;
				slb[inumber].iW = sosedb[inumber].iB;

				//deltal=0.5*(pa[nvtx[2][sosedb[inumber].iII]-1].y+pa[nvtx[0][sosedb[inumber].iII]-1].y);
				//deltal-=0.5*(pa[nvtx[2][sosedb[inumber].iI]-1].y+pa[nvtx[0][sosedb[inumber].iI]-1].y);
				//fiplus=0.5*dl/deltal;

				// lami=(lamI*lamII)/((1.0-fiplus)*lamI+fiplus*lamII); // ���������

				// ������ �����
				slb[inumber].b = 0;
				//slb[inumber].b=(dbeta-1.0)*lami*dS*(potent[sosedb[inumber].iI]-potent[sosedb[inumber].iII])/deltal;
				slb[inumber].b += qb*dS;
				//printf("source=%e\n",qb*dS); getchar(); // debug

				break;

			case TSIDE: 
				dl = pa[nvtx[4][sosedb[inumber].iI] - 1].z - pa[nvtx[0][sosedb[inumber].iI] - 1].z;
				//dS = pa[nvtx[1][sosedb[inumber].iI] - 1].x - pa[nvtx[0][sosedb[inumber].iI] - 1].x;
				//dS *= (pa[nvtx[2][sosedb[inumber].iI] - 1].y - pa[nvtx[0][sosedb[inumber].iI] - 1].y); // ������� �����
				dS = sosedb[inumber].dS; // ������� ����� 25.09.2016.

				// ����������� ����������������
				lamB = prop_b[LAM][sosedb[inumber].iB - maxelm];
				lamI = prop[LAM][sosedb[inumber].iI];
				if (sosedb[inumber].iII >= maxelm) {
					lamII = prop_b[LAM][sosedb[inumber].iII - maxelm];
				}
				else {
					lamII = prop[LAM][sosedb[inumber].iII];
				}

				// �������������.
				lamB *= prop[MULT_LAM_Z][sosedb[inumber].iI];
				lamI *= prop[MULT_LAM_Z][sosedb[inumber].iI];
				lamII *= prop[MULT_LAM_Z][sosedb[inumber].iI];

				/*
				if ((ptr!=NULL) && (ptr[1][sosedb[inumber].iI]!=-1)) {
				lamB+=prop_b[CP][sosedb[inumber].iB-maxelm]*f[ptr[1][sosedb[inumber].iI]].potent[MUT][f[ptr[1][sosedb[inumber].iI]].sosedi[BSIDE][ptr[0][sosedb[inumber].iI]].iNODE1]/0.85;
				lamI+=prop[CP][sosedb[inumber].iI]*f[ptr[1][sosedb[inumber].iI]].potent[MUT][ptr[0][sosedb[inumber].iI]]/0.85;
				lamII+=prop[CP][sosedb[inumber].iII]*f[ptr[1][sosedb[inumber].iI]].potent[MUT][ptr[0][sosedb[inumber].iII]]/0.85;
				}
				*/

				// ���������������� �����������, ��� ���� ������ ���������������� ������� �� ������� � � ��������� ���������� ����,
				// ����� ����� �������� ���������� �����������. 4 ������ 2013 ����.
				lamB = lamI;

				if (b1) {
					//  ������� ������������� �� �����.
					lamB = conductivity2Dinsource[inumber];
				}

				slb[inumber].ai = 2.0*dbeta*lamB*dS / dl;
				slb[inumber].iI = sosedb[inumber].iI;
				slb[inumber].aw = slb[inumber].ai;
				slb[inumber].iW = sosedb[inumber].iB;

				//deltal=0.5*(pa[nvtx[4][sosedb[inumber].iII]-1].z+pa[nvtx[0][sosedb[inumber].iII]-1].z);
				//deltal-=0.5*(pa[nvtx[4][sosedb[inumber].iI]-1].z+pa[nvtx[0][sosedb[inumber].iI]-1].z);
				//fiplus=0.5*dl/deltal;

				//lami=(lamI*lamII)/((1.0-fiplus)*lamI+fiplus*lamII); // ���������.

				// ������ �����
				slb[inumber].b = 0.0;
				//slb[inumber].b=(dbeta-1.0)*lami*dS*(potent[sosedb[inumber].iI]-potent[sosedb[inumber].iII])/deltal;
				slb[inumber].b += qb*dS;

				break;

			case WSIDE:
				dl = pa[nvtx[1][sosedb[inumber].iI] - 1].x - pa[nvtx[0][sosedb[inumber].iI] - 1].x;
				//dS = pa[nvtx[2][sosedb[inumber].iI] - 1].y - pa[nvtx[1][sosedb[inumber].iI] - 1].y;
				//dS *= (pa[nvtx[4][sosedb[inumber].iI] - 1].z - pa[nvtx[0][sosedb[inumber].iI] - 1].z); // ������� �����
				dS = sosedb[inumber].dS; // ������� ����� 25.09.2016.


				// ����������� ����������������
				lamB = prop_b[LAM][sosedb[inumber].iB - maxelm];
				lamI = prop[LAM][sosedb[inumber].iI];
				if (sosedb[inumber].iII >= maxelm) {
					lamII = prop_b[LAM][sosedb[inumber].iII - maxelm];
				}
				else {
					lamII = prop[LAM][sosedb[inumber].iII];
				}

				// �������������.
				lamB *= prop[MULT_LAM_X][sosedb[inumber].iI];
				lamI *= prop[MULT_LAM_X][sosedb[inumber].iI];
				lamII *= prop[MULT_LAM_X][sosedb[inumber].iI];

				/*
				if ((ptr!=NULL) && (ptr[1][sosedb[inumber].iI]!=-1)) {
				lamB+=prop_b[CP][sosedb[inumber].iB-maxelm]*f[ptr[1][sosedb[inumber].iI]].potent[MUT][f[ptr[1][sosedb[inumber].iI]].sosedi[ESIDE][ptr[0][sosedb[inumber].iI]].iNODE1]/0.85;
				lamI+=prop[CP][sosedb[inumber].iI]*f[ptr[1][sosedb[inumber].iI]].potent[MUT][ptr[0][sosedb[inumber].iI]]/0.85;
				lamII+=prop[CP][sosedb[inumber].iII]*f[ptr[1][sosedb[inumber].iI]].potent[MUT][ptr[0][sosedb[inumber].iII]]/0.85;
				}
				*/

				// ���������������� �����������, ��� ���� ������ ���������������� ������� �� ������� � � ��������� ���������� ����,
				// ����� ����� �������� ���������� �����������. 4 ������ 2013 ����.
				lamB = lamI;

				if (b1) {
					//  ������� ������������� �� �����.
					lamB = conductivity2Dinsource[inumber];
				}

				slb[inumber].ai = 2.0*dbeta*lamB*dS / dl;
				slb[inumber].iI = sosedb[inumber].iI;
				slb[inumber].aw = slb[inumber].ai;
				slb[inumber].iW = sosedb[inumber].iB;

				//deltal=-0.5*(pa[nvtx[1][sosedb[inumber].iII]-1].x+pa[nvtx[0][sosedb[inumber].iII]-1].x);
				//deltal+=0.5*(pa[nvtx[1][sosedb[inumber].iI]-1].x+pa[nvtx[0][sosedb[inumber].iI]-1].x);
				//fiplus=0.5*dl/deltal;

				//lami=(lamI*lamII)/((1.0-fiplus)*lamI+fiplus*lamII); // ���������.

				// ������ �����
				// �������� !!! ���� ����� ���������� ������� ��� dbeta > 1 �� �������� ����� �������� ������� I � II!!!!
				slb[inumber].b = 0;
				//slb[inumber].b=(dbeta-1.0)*lami*dS*(potent[sosedb[inumber].iI]-potent[sosedb[inumber].iII])/deltal;
				slb[inumber].b += qb*dS;


				break;

			case SSIDE : 
				dl = pa[nvtx[2][sosedb[inumber].iI] - 1].y - pa[nvtx[0][sosedb[inumber].iI] - 1].y;
				//dS = pa[nvtx[1][sosedb[inumber].iI] - 1].x - pa[nvtx[0][sosedb[inumber].iI] - 1].x;
				//dS *= (pa[nvtx[4][sosedb[inumber].iI] - 1].z - pa[nvtx[0][sosedb[inumber].iI] - 1].z); // ������� �����
				dS = sosedb[inumber].dS; // ������� ����� 25.09.2016.


				// ����������� ����������������
				lamB = prop_b[LAM][sosedb[inumber].iB - maxelm];
				lamI = prop[LAM][sosedb[inumber].iI];
				if (sosedb[inumber].iII >= maxelm) {
					lamII = prop_b[LAM][sosedb[inumber].iII - maxelm];
				}
				else {
					lamII = prop[LAM][sosedb[inumber].iII];
				}

				// �������������.
				lamB *= prop[MULT_LAM_Y][sosedb[inumber].iI];
				lamI *= prop[MULT_LAM_Y][sosedb[inumber].iI];
				lamII *= prop[MULT_LAM_Y][sosedb[inumber].iI];

				/*
				if ((ptr!=NULL) && (ptr[1][sosedb[inumber].iI]!=-1)) {
				lamB+=prop_b[CP][sosedb[inumber].iB-maxelm]*f[ptr[1][sosedb[inumber].iI]].potent[MUT][f[ptr[1][sosedb[inumber].iI]].sosedi[NSIDE][ptr[0][sosedb[inumber].iI]].iNODE1]/0.85;
				lamI+=prop[CP][sosedb[inumber].iI]*f[ptr[1][sosedb[inumber].iI]].potent[MUT][ptr[0][sosedb[inumber].iI]]/0.85;
				lamII+=prop[CP][sosedb[inumber].iII]*f[ptr[1][sosedb[inumber].iI]].potent[MUT][ptr[0][sosedb[inumber].iII]]/0.85;
				}
				*/

				// ���������������� �����������, ��� ���� ������ ���������������� ������� �� ������� � � ��������� ���������� ����,
				// ����� ����� �������� ���������� �����������. 4 ������ 2013 ����.
				lamB = lamI;

				if (b1) {
					//  ������� ������������� �� �����.
					lamB = conductivity2Dinsource[inumber];
				}

				slb[inumber].ai = 2.0*dbeta*lamB*dS / dl;
				slb[inumber].iI = sosedb[inumber].iI;
				slb[inumber].aw = slb[inumber].ai;
				slb[inumber].iW = sosedb[inumber].iB;

				//deltal=-0.5*(pa[nvtx[2][sosedb[inumber].iII]-1].y+pa[nvtx[0][sosedb[inumber].iII]-1].y);
				//deltal+=0.5*(pa[nvtx[2][sosedb[inumber].iI]-1].y+pa[nvtx[0][sosedb[inumber].iI]-1].y);
				//fiplus=0.5*dl/deltal;

				//lami=(lamI*lamII)/((1.0-fiplus)*lamI+fiplus*lamII); // ���������.

				// ������ �����
				slb[inumber].b = 0;
				//slb[inumber].b=(dbeta-1.0)*lami*dS*(potent[sosedb[inumber].iI]-potent[sosedb[inumber].iII])/deltal;
				slb[inumber].b += qb*dS;
				//printf("source=%e\n",qb*dS); getchar(); // debug

				break;

			case BSIDE:
				dl = pa[nvtx[4][sosedb[inumber].iI] - 1].z - pa[nvtx[0][sosedb[inumber].iI] - 1].z;
				//dS = pa[nvtx[1][sosedb[inumber].iI] - 1].x - pa[nvtx[0][sosedb[inumber].iI] - 1].x;
				//dS *= (pa[nvtx[2][sosedb[inumber].iI] - 1].y - pa[nvtx[0][sosedb[inumber].iI] - 1].y); // ������� �����
				dS = sosedb[inumber].dS; // ������� ����� 25.09.2016.

				

				// ����������� ����������������
				lamB = prop_b[LAM][sosedb[inumber].iB - maxelm];
				lamI = prop[LAM][sosedb[inumber].iI];
				if (sosedb[inumber].iII >= maxelm) {
					lamII = prop_b[LAM][sosedb[inumber].iII-maxelm];
				}
				else {
					lamII = prop[LAM][sosedb[inumber].iII];
				}

				// �������������.
				lamB *= prop[MULT_LAM_Z][sosedb[inumber].iI];
				lamI *= prop[MULT_LAM_Z][sosedb[inumber].iI];
				lamII *= prop[MULT_LAM_Z][sosedb[inumber].iI];

				/*
				if ((ptr!=NULL) && (ptr[1][sosedb[inumber].iI]!=-1)) {
				//printf("add turbulent conductivity...\n");
				//getchar();
				lamB+=prop_b[CP][sosedb[inumber].iB-maxelm]*f[ptr[1][sosedb[inumber].iI]].potent[MUT][f[ptr[1][sosedb[inumber].iI]].sosedi[TSIDE][ptr[0][sosedb[inumber].iI]].iNODE1]/0.85;
				lamI+=prop[CP][sosedb[inumber].iI]*f[ptr[1][sosedb[inumber].iI]].potent[MUT][ptr[0][sosedb[inumber].iI]]/0.85;
				lamII+=prop[CP][sosedb[inumber].iII]*f[ptr[1][sosedb[inumber].iI]].potent[MUT][ptr[0][sosedb[inumber].iII]]/0.85;
				}
				*/

				// ���������������� �����������, ��� ���� ������ ���������������� ������� �� ������� � � ��������� ���������� ����,
				// ����� ����� �������� ���������� �����������. 4 ������ 2013 ����.
				lamB = lamI;

				if (b1) {
					//  ������� ������������� �� �����.
					lamB = conductivity2Dinsource[inumber];
				}

				slb[inumber].ai = 2.0*dbeta*lamB*dS / dl;
				slb[inumber].iI = sosedb[inumber].iI;
				slb[inumber].aw = slb[inumber].ai;
				slb[inumber].iW = sosedb[inumber].iB;

				//printf("lamB=%e\n",lamB); // debug
				//getchar();

				//deltal=-0.5*(pa[nvtx[4][sosedb[inumber].iII]-1].z+pa[nvtx[0][sosedb[inumber].iII]-1].z);
				//deltal+=0.5*(pa[nvtx[4][sosedb[inumber].iI]-1].z+pa[nvtx[0][sosedb[inumber].iI]-1].z);
				//fiplus=0.5*dl/deltal;

				//lami=(lamI*lamII)/((1.0-fiplus)*lamI+fiplus*lamII); // ���������.
				//printf("fi_plus = %e\n",fiplus); 
				//getchar();

				// ������ �����
				slb[inumber].b = 0.0;
				//slb[inumber].b=(dbeta-1.0)*lami*dS*(potent[sosedb[inumber].iI]-potent[sosedb[inumber].iII])/deltal;
				slb[inumber].b += qb*dS;

				//printf("heat flux = %e\n",qb); 
				//getchar();

				break;

			} // end switch

			if (bsc1) {
				qnbc[inumber].dS = dS;
			}

			integer j, l, xitem, k;
			// ���������� �� �����������
			for (j = 0; j < 5; j++) {
				k = j; xitem = sosedb[inumber].iW[j];
				for (l = j + 1; l < 6; l++) {
					if (sosedb[inumber].iW[l] < xitem) {
						k = l; xitem = sosedb[inumber].iW[k];
					}
				}
				sosedb[inumber].iW[k] = sosedb[inumber].iW[j];
				sosedb[inumber].iW[j] = xitem;
			}

			j = 0; l = 0;
			while (sosedb[inumber].iW[j] == (-1)) j++;

			if (j < 6) { slb[inumber].iW1 = sosedb[inumber].iW[j++]; l++; }
			if (j < 6) { slb[inumber].iW2 = sosedb[inumber].iW[j++]; l++; }
			if (j < 6) { slb[inumber].iW3 = sosedb[inumber].iW[j++]; l++; }
			if (j < 6) { slb[inumber].iW4 = sosedb[inumber].iW[j++]; l++; }

			switch (l) {
			case 0: slb[inumber].iW1 = -1;
				slb[inumber].iW2 = -1;
				slb[inumber].iW3 = -1;
				slb[inumber].iW4 = -1;
				break;
			case 1: slb[inumber].iW2 = -1;
				slb[inumber].iW3 = -1;
				slb[inumber].iW4 = -1;
				break;
			case 2: slb[inumber].iW3 = -1;
				slb[inumber].iW4 = -1;
				break;
			case 3: slb[inumber].iW4 = -1;
				break;
			}

			// ��������� ���������� � ������ ���������� ���� iI � ������������ 
			// ��������� ���� ��� ��������� ����� ������� ������� �� ���������. 
			// �.�. ���� iI ������ ���������� ��� ������� iI < maxelm, � ������������ 
			// ������� � �������� ������� ����� � �������� >= maxelm ������� ��� ��
			// ������������.
		}
	}


} // my_elmatr_quad_T3D_bound

// ��������� ���������������� �� ��������� ��� ������� �������������.
// 26.09.2016 ������ �������� � �� ���� �����.
void conduct2Dsourceconstruct(integer iP, equation3D* &sl, ALICE_PARTITION** sosedi, integer maxelm,
	BOUND* sosedb, integer ls, doublereal** prop)
{
	integer iE1, iN1, iT1, iW1, iS1, iB1; // ������ �������� ����������� �������
	integer iE2, iN2, iT2, iW2, iS2, iB2;
	integer iE3, iN3, iT3, iW3, iS3, iB3;
	integer iE4, iN4, iT4, iW4, iS4, iB4;


	iE1 = sosedi[ESIDE][iP].iNODE1;
	iN1 = sosedi[NSIDE][iP].iNODE1;
	iT1 = sosedi[TSIDE][iP].iNODE1;
	iW1 = sosedi[WSIDE][iP].iNODE1;
	iS1 = sosedi[SSIDE][iP].iNODE1;
	iB1 = sosedi[BSIDE][iP].iNODE1;

	iE2 = sosedi[ESIDE][iP].iNODE2;
	iN2 = sosedi[NSIDE][iP].iNODE2;
	iT2 = sosedi[TSIDE][iP].iNODE2;
	iW2 = sosedi[WSIDE][iP].iNODE2;
	iS2 = sosedi[SSIDE][iP].iNODE2;
	iB2 = sosedi[BSIDE][iP].iNODE2;

	iE3 = sosedi[ESIDE][iP].iNODE3;
	iN3 = sosedi[NSIDE][iP].iNODE3;
	iT3 = sosedi[TSIDE][iP].iNODE3;
	iW3 = sosedi[WSIDE][iP].iNODE3;
	iS3 = sosedi[SSIDE][iP].iNODE3;
	iB3 = sosedi[BSIDE][iP].iNODE3;

	iE4 = sosedi[ESIDE][iP].iNODE4;
	iN4 = sosedi[NSIDE][iP].iNODE4;
	iT4 = sosedi[TSIDE][iP].iNODE4;
	iW4 = sosedi[WSIDE][iP].iNODE4;
	iS4 = sosedi[SSIDE][iP].iNODE4;
	iB4 = sosedi[BSIDE][iP].iNODE4;

	// ���������� ��.	

	// ���� � ����� �� ������ ����� ������� ��������� �������
	// �� ��������������� ���������� ����� true
	bool bE1 = false, bN1 = false, bT1 = false, bW1 = false, bS1 = false, bB1 = false;
	bool bE2 = false, bN2 = false, bT2 = false, bW2 = false, bS2 = false, bB2 = false;
	bool bE3 = false, bN3 = false, bT3 = false, bW3 = false, bS3 = false, bB3 = false;
	bool bE4 = false, bN4 = false, bT4 = false, bW4 = false, bS4 = false, bB4 = false;


	if (iE1 >= maxelm) bE1 = true;
	if (iN1 >= maxelm) bN1 = true;
	if (iT1 >= maxelm) bT1 = true;
	if (iW1 >= maxelm) bW1 = true;
	if (iS1 >= maxelm) bS1 = true;
	if (iB1 >= maxelm) bB1 = true;

	if (iE2 >= maxelm) bE2 = true;
	if (iN2 >= maxelm) bN2 = true;
	if (iT2 >= maxelm) bT2 = true;
	if (iW2 >= maxelm) bW2 = true;
	if (iS2 >= maxelm) bS2 = true;
	if (iB2 >= maxelm) bB2 = true;

	if (iE3 >= maxelm) bE3 = true;
	if (iN3 >= maxelm) bN3 = true;
	if (iT3 >= maxelm) bT3 = true;
	if (iW3 >= maxelm) bW3 = true;
	if (iS3 >= maxelm) bS3 = true;
	if (iB3 >= maxelm) bB3 = true;

	if (iE4 >= maxelm) bE4 = true;
	if (iN4 >= maxelm) bN4 = true;
	if (iT4 >= maxelm) bT4 = true;
	if (iW4 >= maxelm) bW4 = true;
	if (iS4 >= maxelm) bS4 = true;
	if (iB4 >= maxelm) bB4 = true;

	if (bE1) {
		if (sosedb[iE1 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iE1 - maxelm] < 0.0) {
				conductivity2Dinsource[iE1 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iE1 - maxelm] = 2.0*conductivity2Dinsource[iE1 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iE1 - maxelm] + prop[LAM][iP]);
			}
		}
	}

	if (bW1) {
		if (sosedb[iW1 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iW1 - maxelm] < 0.0) {
				conductivity2Dinsource[iW1 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iW1 - maxelm] = 2.0*conductivity2Dinsource[iW1 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iW1 - maxelm] + prop[LAM][iP]);
			}
		}
	}

	if (bN1) {
		if (sosedb[iN1 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iN1 - maxelm] < 0.0) {
				conductivity2Dinsource[iN1 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iN1 - maxelm] = 2.0*conductivity2Dinsource[iN1 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iN1 - maxelm] + prop[LAM][iP]);
			}
		}
	}

	if (bS1) {
		if (sosedb[iS1 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iS1 - maxelm] < 0.0) {
				conductivity2Dinsource[iS1 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iS1 - maxelm] = 2.0*conductivity2Dinsource[iS1 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iS1 - maxelm] + prop[LAM][iP]);
			}
		}
	}


	if (bT1) {
		if (sosedb[iT1 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iT1 - maxelm] < 0.0) {
				conductivity2Dinsource[iT1 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iT1 - maxelm] = 2.0*conductivity2Dinsource[iT1 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iT1 - maxelm] + prop[LAM][iP]);
				//if (prop[LAM][iP] < conductivity2Dinsource[iT1 - maxelm]) {
					//conductivity2Dinsource[iT1 - maxelm] = prop[LAM][iP];
				//}
			}
		}
	}

	if (bB1) {
		if (sosedb[iB1 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iB1 - maxelm] < 0.0) {
				conductivity2Dinsource[iB1 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iB1 - maxelm] = 2.0*conductivity2Dinsource[iB1 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iB1 - maxelm] + prop[LAM][iP]);
				//if (prop[LAM][iP] < conductivity2Dinsource[iB1 - maxelm]) {
					//conductivity2Dinsource[iB1 - maxelm] = prop[LAM][iP];
				//}
			}
		}
	}

	if (bE2) {
		if (sosedb[iE2 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iE2 - maxelm] < 0.0) {
				conductivity2Dinsource[iE2 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iE2 - maxelm] = 2.0*conductivity2Dinsource[iE2 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iE2 - maxelm] + prop[LAM][iP]);
			}
		}
	}

	if (bW2) {
		if (sosedb[iW2 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iW2 - maxelm] < 0.0) {
				conductivity2Dinsource[iW2 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iW2 - maxelm] = 2.0*conductivity2Dinsource[iW2 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iW2 - maxelm] + prop[LAM][iP]);
			}
		}
	}

	if (bN2) {
		if (sosedb[iN2 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iN2 - maxelm] < 0.0) {
				conductivity2Dinsource[iN2 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iN2 - maxelm] = 2.0*conductivity2Dinsource[iN2 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iN2 - maxelm] + prop[LAM][iP]);
			}
		}
	}

	if (bS2) {
		if (sosedb[iS2 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iS2 - maxelm] < 0.0) {
				conductivity2Dinsource[iS2 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iS2 - maxelm] = 2.0*conductivity2Dinsource[iS2 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iS2 - maxelm] + prop[LAM][iP]);
			}
		}
	}


	if (bT2) {
		if (sosedb[iT2 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iT2 - maxelm] < 0.0) {
				conductivity2Dinsource[iT2 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iT2 - maxelm] = 2.0*conductivity2Dinsource[iT2 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iT2 - maxelm] + prop[LAM][iP]);
				//if (prop[LAM][iP] < conductivity2Dinsource[iT2 - maxelm]) {
				//conductivity2Dinsource[iT2 - maxelm] = prop[LAM][iP];
				//}
			}
		}
	}

	if (bB2) {
		if (sosedb[iB2 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iB2 - maxelm] < 0.0) {
				conductivity2Dinsource[iB2 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iB2 - maxelm] = 2.0*conductivity2Dinsource[iB2 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iB2 - maxelm] + prop[LAM][iP]);
				//if (prop[LAM][iP] < conductivity2Dinsource[iB2 - maxelm]) {
				//conductivity2Dinsource[iB2 - maxelm] = prop[LAM][iP];
				//}
			}
		}
	}


	if (bE3) {
		if (sosedb[iE3 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iE3 - maxelm] < 0.0) {
				conductivity2Dinsource[iE3 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iE3 - maxelm] = 2.0*conductivity2Dinsource[iE3 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iE3 - maxelm] + prop[LAM][iP]);
			}
		}
	}

	if (bW3) {
		if (sosedb[iW3 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iW3 - maxelm] < 0.0) {
				conductivity2Dinsource[iW3 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iW3 - maxelm] = 2.0*conductivity2Dinsource[iW3 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iW3 - maxelm] + prop[LAM][iP]);
			}
		}
	}

	if (bN3) {
		if (sosedb[iN3 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iN3 - maxelm] < 0.0) {
				conductivity2Dinsource[iN3 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iN3 - maxelm] = 2.0*conductivity2Dinsource[iN3 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iN3 - maxelm] + prop[LAM][iP]);
			}
		}
	}

	if (bS3) {
		if (sosedb[iS3 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iS3 - maxelm] < 0.0) {
				conductivity2Dinsource[iS3 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iS3 - maxelm] = 2.0*conductivity2Dinsource[iS3 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iS3 - maxelm] + prop[LAM][iP]);
			}
		}
	}


	if (bT3) {
		if (sosedb[iT3 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iT3 - maxelm] < 0.0) {
				conductivity2Dinsource[iT3 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iT3 - maxelm] = 2.0*conductivity2Dinsource[iT3 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iT3 - maxelm] + prop[LAM][iP]);
				//if (prop[LAM][iP] < conductivity2Dinsource[iT3 - maxelm]) {
				//conductivity2Dinsource[iT3 - maxelm] = prop[LAM][iP];
				//}
			}
		}
	}

	if (bB3) {
		if (sosedb[iB3 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iB3 - maxelm] < 0.0) {
				conductivity2Dinsource[iB3 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iB3 - maxelm] = 2.0*conductivity2Dinsource[iB3 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iB3 - maxelm] + prop[LAM][iP]);
				//if (prop[LAM][iP] < conductivity2Dinsource[iB3 - maxelm]) {
				//conductivity2Dinsource[iB3 - maxelm] = prop[LAM][iP];
				//}
			}
		}
	}

	if (bE4) {
		if (sosedb[iE4 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iE4 - maxelm] < 0.0) {
				conductivity2Dinsource[iE4 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iE4 - maxelm] = 2.0*conductivity2Dinsource[iE4 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iE4 - maxelm] + prop[LAM][iP]);
			}
		}
	}

	if (bW4) {
		if (sosedb[iW4 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iW4 - maxelm] < 0.0) {
				conductivity2Dinsource[iW4 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iW4 - maxelm] = 2.0*conductivity2Dinsource[iW4 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iW4 - maxelm] + prop[LAM][iP]);
			}
		}
	}

	if (bN4) {
		if (sosedb[iN4 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iN4 - maxelm] < 0.0) {
				conductivity2Dinsource[iN4 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iN4 - maxelm] = 2.0*conductivity2Dinsource[iN4 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iN4 - maxelm] + prop[LAM][iP]);
			}
		}
	}

	if (bS4) {
		if (sosedb[iS4 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iS4 - maxelm] < 0.0) {
				conductivity2Dinsource[iS4 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iS4 - maxelm] = 2.0*conductivity2Dinsource[iS4 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iS4 - maxelm] + prop[LAM][iP]);
			}
		}
	}


	if (bT4) {
		if (sosedb[iT4 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iT4 - maxelm] < 0.0) {
				conductivity2Dinsource[iT4 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iT4 - maxelm] = 2.0*conductivity2Dinsource[iT4 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iT4 - maxelm] + prop[LAM][iP]);
				//if (prop[LAM][iP] < conductivity2Dinsource[iT4 - maxelm]) {
				//conductivity2Dinsource[iT4 - maxelm] = prop[LAM][iP];
				//}
			}
		}
	}

	if (bB4) {
		if (sosedb[iB4 - maxelm].MCB < ls) {
			if (conductivity2Dinsource[iB4 - maxelm] < 0.0) {
				conductivity2Dinsource[iB4 - maxelm] = prop[LAM][iP];
			}
			else {
				conductivity2Dinsource[iB4 - maxelm] = 2.0*conductivity2Dinsource[iB4 - maxelm] * prop[LAM][iP] / (conductivity2Dinsource[iB4 - maxelm] + prop[LAM][iP]);
				//if (prop[LAM][iP] < conductivity2Dinsource[iB4 - maxelm]) {
				//conductivity2Dinsource[iB4 - maxelm] = prop[LAM][iP];
				//}
			}
		}
	}

} // conduct2Dsourceconstruct


// ���������� �������� ����������� � ����� � ������������ (x,y,z).
// ��� ����� ���������� �������� ������������� ����������� �� ������� ���������� ���������.
// ��������� ����������� ��� ���� �������������� ��������, � ������������ �������� ������� ������� 
// �� ������� ���������� ���������.
doublereal mnk(integer iP, integer maxelm, doublereal* potent, integer**  nvtx, TOCHKA* pa, ALICE_PARTITION** sosedi, doublereal x, doublereal y, doublereal z)
{
	
	integer inum_now = 1; // iP ��� ����.

	integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	iE = sosedi[ESIDE][iP].iNODE1; iN = sosedi[NSIDE][iP].iNODE1; iT = sosedi[TSIDE][iP].iNODE1;
	iW = sosedi[WSIDE][iP].iNODE1; iS = sosedi[SSIDE][iP].iNODE1; iB = sosedi[BSIDE][iP].iNODE1;

	// 26.09.2016 ������� ��� ���� �����.
	integer iE2, iN2, iT2, iW2, iS2, iB2; // ������ �������� ����������� �������
	integer iE3, iN3, iT3, iW3, iS3, iB3; // ������ �������� ����������� �������
	integer iE4, iN4, iT4, iW4, iS4, iB4; // ������ �������� ����������� �������

	// -1 ���� �� ������������ � [0..maxelm+maxbound-1] ���� ������������.

	iE2 = sosedi[ESIDE][iP].iNODE2; iN2 = sosedi[NSIDE][iP].iNODE2; iT2 = sosedi[TSIDE][iP].iNODE2;
	iW2 = sosedi[WSIDE][iP].iNODE2; iS2 = sosedi[SSIDE][iP].iNODE2; iB2 = sosedi[BSIDE][iP].iNODE2;
	iE3 = sosedi[ESIDE][iP].iNODE3; iN3 = sosedi[NSIDE][iP].iNODE3; iT3 = sosedi[TSIDE][iP].iNODE3;
	iW3 = sosedi[WSIDE][iP].iNODE3; iS3 = sosedi[SSIDE][iP].iNODE3; iB3 = sosedi[BSIDE][iP].iNODE3;
	iE4 = sosedi[ESIDE][iP].iNODE4; iN4 = sosedi[NSIDE][iP].iNODE4; iT4 = sosedi[TSIDE][iP].iNODE4;
	iW4 = sosedi[WSIDE][iP].iNODE4; iS4 = sosedi[SSIDE][iP].iNODE4; iB4 = sosedi[BSIDE][iP].iNODE4;

	if ((iE > -1)&&(iE<maxelm)) inum_now++;
	if ((iW > -1)&&(iW<maxelm)) inum_now++;
	if ((iN > -1)&&(iN<maxelm)) inum_now++;
	if ((iS > -1)&&(iS<maxelm)) inum_now++;
	if ((iT > -1)&&(iT<maxelm)) inum_now++;
	if ((iB > -1)&&(iB<maxelm)) inum_now++;

	if ((iE2 > -1)&&(iE2<maxelm)) inum_now++;
	if ((iW2 > -1)&&(iW2<maxelm)) inum_now++;
	if ((iN2 > -1)&&(iN2<maxelm)) inum_now++;
	if ((iS2 > -1)&&(iS2<maxelm)) inum_now++;
	if ((iT2 > -1)&&(iT2<maxelm)) inum_now++;
	if ((iB2 > -1)&&(iB2<maxelm)) inum_now++;

	if ((iE3 > -1)&&(iE3<maxelm)) inum_now++;
	if ((iW3 > -1)&&(iW3<maxelm)) inum_now++;
	if ((iN3 > -1)&&(iN3<maxelm)) inum_now++;
	if ((iS3 > -1)&&(iS3<maxelm)) inum_now++;
	if ((iT3 > -1)&&(iT3<maxelm)) inum_now++;
	if ((iB3 > -1)&&(iB3<maxelm)) inum_now++;

	if ((iE4 > -1)&&(iE4<maxelm)) inum_now++;
	if ((iW4 > -1)&&(iW4<maxelm)) inum_now++;
	if ((iN4 > -1)&&(iN4<maxelm)) inum_now++;
	if ((iS4 > -1)&&(iS4<maxelm)) inum_now++;
	if ((iT4 > -1)&&(iT4<maxelm)) inum_now++;
	if ((iB4 > -1)&&(iB4<maxelm)) inum_now++;

	TOCHKA* pointerlist = NULL;
	pointerlist = new TOCHKA[inum_now];

	doublereal* rthdsd_Gauss = NULL;
	rthdsd_Gauss = new doublereal[inum_now];

	inum_now = 0;
	rthdsd_Gauss[inum_now++] = potent[iP];

	if ((iE > -1)&&(iE<maxelm)) rthdsd_Gauss[inum_now++] = potent[iE];
	if ((iW > -1)&&(iW<maxelm)) rthdsd_Gauss[inum_now++] = potent[iW];
	if ((iN > -1)&&(iN<maxelm)) rthdsd_Gauss[inum_now++] = potent[iN];
	if ((iS > -1)&&(iS<maxelm)) rthdsd_Gauss[inum_now++] = potent[iS];
	if ((iT > -1)&&(iT<maxelm)) rthdsd_Gauss[inum_now++] = potent[iT];
	if ((iB > -1)&&(iB<maxelm)) rthdsd_Gauss[inum_now++] = potent[iB];

	if ((iE2 > -1)&&(iE2<maxelm)) rthdsd_Gauss[inum_now++] = potent[iE2];
	if ((iW2 > -1)&&(iW2<maxelm)) rthdsd_Gauss[inum_now++] = potent[iW2];
	if ((iN2 > -1)&&(iN2<maxelm)) rthdsd_Gauss[inum_now++] = potent[iN2];
	if ((iS2 > -1)&&(iS2<maxelm)) rthdsd_Gauss[inum_now++] = potent[iS2];
	if ((iT2 > -1)&&(iT2<maxelm)) rthdsd_Gauss[inum_now++] = potent[iT2];
	if ((iB2 > -1)&&(iB2<maxelm)) rthdsd_Gauss[inum_now++] = potent[iB2];

	if ((iE3 > -1)&&(iE3<maxelm)) rthdsd_Gauss[inum_now++] = potent[iE3];
	if ((iW3 > -1)&&(iW3<maxelm)) rthdsd_Gauss[inum_now++] = potent[iW3];
	if ((iN3 > -1)&&(iN3<maxelm)) rthdsd_Gauss[inum_now++] = potent[iN3];
	if ((iS3 > -1)&&(iS3<maxelm)) rthdsd_Gauss[inum_now++] = potent[iS3];
	if ((iT3 > -1)&&(iT3<maxelm)) rthdsd_Gauss[inum_now++] = potent[iT3];
	if ((iB3 > -1)&&(iB3<maxelm)) rthdsd_Gauss[inum_now++] = potent[iB3];

	if ((iE4 > -1)&&(iE4<maxelm)) rthdsd_Gauss[inum_now++] = potent[iE4];
	if ((iW4 > -1)&&(iW4<maxelm)) rthdsd_Gauss[inum_now++] = potent[iW4];
	if ((iN4 > -1)&&(iN4<maxelm)) rthdsd_Gauss[inum_now++] = potent[iN4];
	if ((iS4 > -1)&&(iS4<maxelm)) rthdsd_Gauss[inum_now++] = potent[iS4];
	if ((iT4 > -1)&&(iT4<maxelm)) rthdsd_Gauss[inum_now++] = potent[iT4];
	if ((iB4 > -1)&&(iB4<maxelm)) rthdsd_Gauss[inum_now++] = potent[iB4];

	inum_now = 0;
	TOCHKA point0;
	center_cord3D(iP, nvtx, pa, point0, 100);
	pointerlist[inum_now++] = point0;

	if ((iE > -1)&&(iE<maxelm)) {
		center_cord3D(iE, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iW > -1)&&(iW<maxelm)) {
		center_cord3D(iW, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iN > -1)&&(iN<maxelm)) {
		center_cord3D(iN, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iS > -1)&&(iS<maxelm)) {
		center_cord3D(iS, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iT > -1)&&(iT<maxelm)) {
		center_cord3D(iT, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iB > -1)&&(iB<maxelm)) {
		center_cord3D(iB, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}

	if ((iE2 > -1)&&(iE2<maxelm)) {
		center_cord3D(iE2, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iW2 > -1)&&(iW2<maxelm)) {
		center_cord3D(iW2, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iN2 > -1)&&(iN2<maxelm)) {
		center_cord3D(iN2, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iS2 > -1)&&(iS2<maxelm)) {
		center_cord3D(iS2, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iT2 > -1)&&(iT2<maxelm)) {
		center_cord3D(iT2, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iB2 > -1)&&(iB2<maxelm)) {
		center_cord3D(iB2, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}


	if ((iE3 > -1)&&(iE3<maxelm)) {
		center_cord3D(iE3, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iW3 > -1)&&(iW3<maxelm)) {
		center_cord3D(iW3, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iN3 > -1)&&(iN3<maxelm)) {
		center_cord3D(iN3, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iS3 > -1)&&(iS3<maxelm)) {
		center_cord3D(iS3, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iT3 > -1)&&(iT3<maxelm)) {
		center_cord3D(iT3, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iB3 > -1)&&(iB3<maxelm)) {
		center_cord3D(iB3, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}

	if ((iE4 > -1)&&(iE4<maxelm)) {
		center_cord3D(iE4, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iW4 > -1)&&(iW4<maxelm)) {
		center_cord3D(iW4, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iN4 > -1)&&(iN4<maxelm)) {
		center_cord3D(iN4, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iS4 > -1)&&(iS4<maxelm)) {
		center_cord3D(iS4, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iT4 > -1)&&(iT4<maxelm)) {
		center_cord3D(iT4, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}
	if ((iB4 > -1)&&(iB4<maxelm)) {
		center_cord3D(iB4, nvtx, pa, point0, 100);
		pointerlist[inum_now++] = point0;
	}


	// ����� ��������� �������.
	doublereal min_x = 1e60;
	doublereal min_y = 1e60;
	doublereal min_z = 1e60;
	doublereal max_x = -1e60;
	doublereal max_y = -1e60;
	doublereal max_z = -1e60;


	for (integer j = 0; j < inum_now; j++) {
		if (pointerlist[j].x < min_x) min_x = pointerlist[j].x;
		if (pointerlist[j].y < min_y) min_y = pointerlist[j].y;
		if (pointerlist[j].z < min_z) min_z = pointerlist[j].z;

		if (pointerlist[j].x > max_x) max_x = pointerlist[j].x;
		if (pointerlist[j].y > max_y) max_y = pointerlist[j].y;
		if (pointerlist[j].z > max_z) max_z = pointerlist[j].z;
	}

	
	
	
	
	// 05.07.2017

	min_x = 1.05*fabs(max_x - min_x);
	if (min_x < 1.0e-30) {
		min_x = 1.05*fabs(max_x);
	}
	min_y = 1.05*fabs(max_y - min_y);
	if (min_y < 1.0e-30) {
		min_y = 1.05*fabs(max_y);
	}
	min_z = 1.05*fabs(max_z - min_z);
	if (min_z < 1.0e-30) {
		min_z = 1.05*fabs(max_z);
	}


	/*
	if (min_x < 1.0e-30) {
		printf("error!!! negative min_x MNK!\n");
		printf("min_x=%e max_x=%e\n", min_x, max_x);
	}
	if (min_y < 1.0e-30) {
		printf("error!!! negative min_y MNK!\n");
		printf("min_y=%e max_y=%e\n", min_y, max_y);
	}
	if (min_z < 1.0e-30) {
		printf("error!!! negative min_z MNK!\n");
		printf("min_z=%e max_z=%e\n", min_z, max_z);
	}
	*/

	// ��� ��� ��� ������� ���������� ������ ������ ����.
	for (integer j = 0; j < inum_now; j++) {
		pointerlist[j].x += min_x;
		pointerlist[j].y += min_y;
		pointerlist[j].z += min_z;	
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

	for (integer j = 0; j < inum_now; j++) {

		Xmatr[0][0] += 1.0;
		Xmatr[0][1] += pointerlist[j].x;
		Xmatr[0][2] += pointerlist[j].y;
		Xmatr[0][3] += pointerlist[j].z;

		Xmatr[1][0] += pointerlist[j].x;
		Xmatr[1][1] += pointerlist[j].x*pointerlist[j].x;
		Xmatr[1][2] += pointerlist[j].x*pointerlist[j].y;
		Xmatr[1][3] += pointerlist[j].x*pointerlist[j].z;

		Xmatr[2][0] += pointerlist[j].y;
		Xmatr[2][1] += pointerlist[j].y*pointerlist[j].x;
		Xmatr[2][2] += pointerlist[j].y*pointerlist[j].y;
		Xmatr[2][3] += pointerlist[j].y*pointerlist[j].z;

		Xmatr[3][0] += pointerlist[j].z;
		Xmatr[3][1] += pointerlist[j].z*pointerlist[j].x;
		Xmatr[3][2] += pointerlist[j].z*pointerlist[j].y;
		Xmatr[3][3] += pointerlist[j].z*pointerlist[j].z;

		bmatr[0] += rthdsd_Gauss[j];
		bmatr[1] += pointerlist[j].x*rthdsd_Gauss[j];
		bmatr[2] += pointerlist[j].y*rthdsd_Gauss[j];
		bmatr[3] += pointerlist[j].z*rthdsd_Gauss[j];
	}

	if ((fabs(Xmatr[0][0]) < 1.e-30) || (fabs(Xmatr[1][1]) < 1.e-30) || (fabs(Xmatr[2][2]) < 1.e-30) || (fabs(Xmatr[3][3]) < 1.e-30)) {
#if doubleintprecision == 1
		printf("inum_now=%lld\n", inum_now);
#else
		printf("inum_now=%d\n", inum_now);
#endif
		
		getchar();
	}

	if (pointerlist != NULL) {
		delete[] pointerlist;
		pointerlist = NULL;
	}
	if (rthdsd_Gauss != NULL) {
		delete[] rthdsd_Gauss;
		rthdsd_Gauss = NULL;
	}


	for (integer j1 = 0; j1 <= 100; j1++) {
		koefmatr[0] = (bmatr[0] - Xmatr[0][1] * koefmatr[1] - Xmatr[0][2] * koefmatr[2] - Xmatr[0][3] * koefmatr[3]) / Xmatr[0][0];
		koefmatr[1] = (bmatr[1] - Xmatr[1][0] * koefmatr[0] - Xmatr[1][2] * koefmatr[2] - Xmatr[1][3] * koefmatr[3]) / Xmatr[1][1];
		koefmatr[2] = (bmatr[2] - Xmatr[2][0] * koefmatr[0] - Xmatr[2][1] * koefmatr[1] - Xmatr[2][3] * koefmatr[3]) / Xmatr[2][2];
		koefmatr[3] = (bmatr[3] - Xmatr[3][0] * koefmatr[0] - Xmatr[3][1] * koefmatr[1] - Xmatr[3][2] * koefmatr[2]) / Xmatr[3][3];
	}

	doublereal retT = 0.0;
	retT = (koefmatr[0] + koefmatr[1] * (x + min_x) + koefmatr[2] * (y + min_y) + koefmatr[3] * (z + min_z));

	if (Xmatr != NULL) {
		for (integer j = 0; j <= 3; j++) {
			if (Xmatr[j] != NULL) {
				delete[] Xmatr[j];
				Xmatr[j] = NULL;
			}
		}
	}
	if (Xmatr != NULL) {
		delete[] Xmatr;
		Xmatr = NULL;
	}
	if (bmatr != NULL) {
		delete[] bmatr;
		bmatr = NULL;
	}
	if (koefmatr != NULL) {
		delete[] koefmatr;
		koefmatr = NULL;
	}

	return retT;

} // mnk


// �������� ���� ��������� ������� ���� ��� ����������� ��������� 
// ��������� - ��������, ��� ������������ ������������ ������.
// ��� ������������� �����.
// ��� ������ ����������������. ������ ���������� ��.
// �������� �������� ������� �� ���� ����� 26 �������� 2016. � 14_09.
void my_elmatr_quad_T3D(integer iP, equation3D* &sl, equation3D_bon* &slb, 
						bool btimedep, doublereal tauparam, integer ishconvection,
					    integer** nvtx, doublereal* potent, TOCHKA* pa, 
						doublereal** prop, doublereal** prop_b, ALICE_PARTITION** sosedi,
					    doublereal alpha, doublereal dbeta,  bool bconvective,
						doublereal dSc_out, integer ipower_time_depend, integer maxelm, integer flow_interior,
						integer** ptr, BOUND* sosedb, integer ls, FLOW* &f,
						bool* binternalsource, const doublereal* toldtimestep,
						BLOCK* b, integer lb, TPROP* matlist, integer inumglobaliter,
						SOURCE* s, doublereal poweron_multiplier_sequence,
						integer *ilevel_alice, integer* &whot_is_block) {

	

	if (iP < 0) {
		printf("iP=%lld\n",iP);
		getchar();
	}

	

    // btimedep==false - ������������, ����� (true) ��������������
	// tauparam - ��� �� �������.
	// toldtimestep - ����������� � ����������� ���������� ����.
    const bool imitation_time=false; // false - ������ ����� �� ������������, true - ������������

	// inumglobaliter ����� ���������� �������� SIMPLE ���������.


	// �������� ���-��� ������ ���� �������� ����� �������� 
	// �� ����� ������������� ��������� ������������� (��� ����������� � ���������� ����� ������).
	// ���� true �� �������� �������� ���-���.
	//bool bRhieChowi=true, bRhieChowb=false; // i - internal, b - border
	//doublereal RCh=1.0; // ������������� ������ �������� ���-��� ��. ���������.

    // iP - ����� ������������ ������������ ������
	// iP ���������� �� 0..maxelm-1
	integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	iE=sosedi[ESIDE][iP].iNODE1; iN=sosedi[NSIDE][iP].iNODE1; iT=sosedi[TSIDE][iP].iNODE1;
	iW=sosedi[WSIDE][iP].iNODE1; iS=sosedi[SSIDE][iP].iNODE1; iB=sosedi[BSIDE][iP].iNODE1;


	sl[iP].iE=iE; sl[iP].iN=iN; sl[iP].iT=iT; 
	sl[iP].iS=iS; sl[iP].iW=iW; sl[iP].iB=iB;
	sl[iP].iP = iP;
	

	// 26.09.2016 ������� ��� ���� �����.
	integer iE2, iN2, iT2, iW2, iS2, iB2; // ������ �������� ����������� �������
	integer iE3, iN3, iT3, iW3, iS3, iB3; // ������ �������� ����������� �������
	integer iE4, iN4, iT4, iW4, iS4, iB4; // ������ �������� ����������� �������

	// -1 ���� �� ������������ � [0..maxelm+maxbound-1] ���� ������������.

	iE2 = sosedi[ESIDE][iP].iNODE2; iN2 = sosedi[NSIDE][iP].iNODE2; iT2 = sosedi[TSIDE][iP].iNODE2;
	iW2 = sosedi[WSIDE][iP].iNODE2; iS2 = sosedi[SSIDE][iP].iNODE2; iB2 = sosedi[BSIDE][iP].iNODE2;
	iE3 = sosedi[ESIDE][iP].iNODE3; iN3 = sosedi[NSIDE][iP].iNODE3; iT3 = sosedi[TSIDE][iP].iNODE3;
	iW3 = sosedi[WSIDE][iP].iNODE3; iS3 = sosedi[SSIDE][iP].iNODE3; iB3 = sosedi[BSIDE][iP].iNODE3;
	iE4 = sosedi[ESIDE][iP].iNODE4; iN4 = sosedi[NSIDE][iP].iNODE4; iT4 = sosedi[TSIDE][iP].iNODE4;
	iW4 = sosedi[WSIDE][iP].iNODE4; iS4 = sosedi[SSIDE][iP].iNODE4; iB4 = sosedi[BSIDE][iP].iNODE4;

	sl[iP].iE2 = iE2; sl[iP].iN2 = iN2; sl[iP].iT2 = iT2;
	sl[iP].iS2 = iS2; sl[iP].iW2 = iW2; sl[iP].iB2 = iB2;

	sl[iP].iE3 = iE3; sl[iP].iN3 = iN3; sl[iP].iT3 = iT3;
	sl[iP].iS3 = iS3; sl[iP].iW3 = iW3; sl[iP].iB3 = iB3;

	sl[iP].iE4 = iE4; sl[iP].iN4 = iN4; sl[iP].iT4 = iT4;
	sl[iP].iS4 = iS4; sl[iP].iW4 = iW4; sl[iP].iB4 = iB4;

	sl[iP].b = 0.0;

	// ������� ���������� �����.
	// �� ������� ������ ����� ���������� � ����� �������� ������ ���.
	sl[iP].bE2 = false; sl[iP].bW2 = false; sl[iP].bS2 = false;
	sl[iP].bN2 = false; sl[iP].bB2 = false; sl[iP].bT2 = false;

	sl[iP].bE3 = false; sl[iP].bW3 = false; sl[iP].bS3 = false;
	sl[iP].bN3 = false; sl[iP].bB3 = false; sl[iP].bT3 = false;

	sl[iP].bE4 = false; sl[iP].bW4 = false; sl[iP].bS4 = false;
	sl[iP].bN4 = false; sl[iP].bB4 = false; sl[iP].bT4 = false;

	if (iE2 > -1) sl[iP].bE2 = true;
	if (iW2 > -1) sl[iP].bW2 = true;
	if (iN2 > -1) sl[iP].bN2 = true;
	if (iS2 > -1) sl[iP].bS2 = true;
	if (iT2 > -1) sl[iP].bT2 = true;
	if (iB2 > -1) sl[iP].bB2 = true;

	if (iE3 > -1) sl[iP].bE3 = true;
	if (iW3 > -1) sl[iP].bW3 = true;
	if (iN3 > -1) sl[iP].bN3 = true;
	if (iS3 > -1) sl[iP].bS3 = true;
	if (iT3 > -1) sl[iP].bT3 = true;
	if (iB3 > -1) sl[iP].bB3 = true;

	if (iE4 > -1) sl[iP].bE4 = true;
	if (iW4 > -1) sl[iP].bW4 = true;
	if (iN4 > -1) sl[iP].bN4 = true;
	if (iS4 > -1) sl[iP].bS4 = true;
	if (iT4 > -1) sl[iP].bT4 = true;
	if (iB4 > -1) sl[iP].bB4 = true;

	// ���������� ��.	

	// ���� � ����� �� ������ ����� ������� ��������� �������
	// �� ��������������� ���������� ����� true
	bool bE=false, bN=false, bT=false, bW=false, bS=false, bB=false;
        

	if (iE>=maxelm) bE=true;
	if (iN>=maxelm) bN=true;
	if (iT>=maxelm) bT=true;
    if (iW>=maxelm) bW=true;
	if (iS>=maxelm) bS=true;
	if (iB>=maxelm) bB=true;

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


    /*
    if (iP==25) {
	#if doubleintprecision == 1
		printf("bE=%lld bN=%lld bT=%lld bW=%lld bS=%lld bB=%lld\n",bE,bN,bT,bW,bS,bB);
	#else
		printf("bE=%d bN=%d bT=%d bW=%d bS=%d bB=%d\n",bE,bN,bT,bW,bS,bB);
	#endif
        
        getchar();
	}*/

	// ������ ����� � ���������� ����� � �������:
    bool bsE=false, bsN=false, bsT=false, bsW=false, bsS=false, bsB=false;
	bool bsE1 = false, bsN1 = false, bsT1 = false, bsW1 = false, bsS1 = false, bsB1 = false;
	// ���� ���� ������ ���� � ���� 
	// iP ����������� ������ ���� � �������� � ���������� ����� ������ ���������
	// ������� ����� �� � ������, �� ����� ����� ����� ��������� �������.
	// ������ ����� �������� ����� ������ ����� SOLID ������ �� ������������. TODO.
	// ��� �������� ����� ����� ��������� �� ������� ��������� ������� � ��������� � ���������,
	// ����� ����� ����� ��������� �� ������� (TODO).
	// ��� �������� ���������� ������ ���������� 
	//bool binternalfindsituation = false;
	// ����� �������������� ������ ������ ����������� ��������� �����.
	/*
	if ((flow_interior>0)&&(ptr[1][iP]>-1)) {
		// ��� �������� ������ ��� iP fluid �����.
		if (bE) {
		// �������� � ���������� �����
		//if (sosedb[iE-maxelm].MCB<ls) bsE=true;
		// ������ ���������� �������� ����� ����� ���������
		// � ������ �����. ������ � ���� ������ ����������� �����
		// � ���������� �� ������� ��������, � ��������� �������
		// ���� ��������, ��������, ����� �� ������� ��������� �������
		// � ��������� ��������� ����� �� �����������. ��� �����.
		if (binternalsource[iE-maxelm]) {
		bsE=true;
		//binternalfindsituation=true;
		}
		}
		if (bN) {
		//if (sosedb[iN-maxelm].MCB<ls) bsN=true;
		if (binternalsource[iN-maxelm]) {
		bsN=true;
		//binternalfindsituation=true;
		}
		}
		if (bT) {
		//if (sosedb[iT-maxelm].MCB<ls) bsT=true;
		if (binternalsource[iT-maxelm]) {
		bsT=true;
		//binternalfindsituation=true;
		}
		}
		if (bW) {
		// �������� � ���������� �����
		//if (sosedb[iW-maxelm].MCB<ls) bsW=true;
		if (binternalsource[iW-maxelm]) {
		bsW=true;
		//binternalfindsituation=true;
		}
		}
		if (bS) {
		//if (sosedb[iS-maxelm].MCB<ls) bsS=true;
		if (binternalsource[iS-maxelm]) {
		bsS=true;
		//binternalfindsituation=true;
		}
		}
		if (bB) {
		//if (sosedb[iB-maxelm].MCB<ls) bsB=true;
		if (binternalsource[iB-maxelm]) {
		bsB=true;
		//binternalfindsituation=true;
		}
		}
		}
		*/
//else
/*{
	// 3 ��� ����� ������.
	// �� �� ��� ������� ���������� ������ � �������.
	// ���� �� �� ����� ��������� �������� ����� �� �� ���� ��� (��������� source2Dproblem)
	// �������� �������� � ������ � ������ ����������� � ��������� �����.



	// ���� �� ���������������� ���������� �������� ����� �� �� ��� ���� �� ������
	// ��� ����������� ��� solid-solid ��� solid-fluid.
	if (bE) {
	// �������� � ���������� �����
	//if (sosedb[iE-maxelm].MCB<ls) bsE=true;
	// ������ ���������� �������� ����� ����� ���������
	// � ������ �����. ������ � ���� ������ ����������� �����
	// � ���������� �� ������� ��������, � ��������� �������
	// ���� ��������, ��������, ����� �� ������� ��������� �������
	// � ��������� ��������� ����� �� �����������. ��� �����.
	if (binternalsource[iE - maxelm]) {
	if (sourse2Dproblem[iE - maxelm]) {
	//bsE = true;
	//binternalfindsituation = true;
	}
	sourse2Dproblem[iE - maxelm] = !sourse2Dproblem[iE - maxelm];
	}
	}
	if (bN) {
	//if (sosedb[iN-maxelm].MCB<ls) bsN=true;
	if (binternalsource[iN - maxelm]) {
	if (sourse2Dproblem[iN - maxelm]) {
	//bsN = true;
	//binternalfindsituation = true;
	}
	sourse2Dproblem[iN - maxelm] = !sourse2Dproblem[iN - maxelm];
	}
	}
	if (bT) {
	//if (sosedb[iT-maxelm].MCB<ls) bsT=true;
	if (binternalsource[iT - maxelm]) {
	if (sourse2Dproblem[iT - maxelm]) {
	//bsT = true;
	//binternalfindsituation = true;
	}
	}
	sourse2Dproblem[iT - maxelm] = !sourse2Dproblem[iT - maxelm];
	}
	if (bW) {
	// �������� � ���������� �����
	//if (sosedb[iW-maxelm].MCB<ls) bsW=true;
	if (binternalsource[iW - maxelm]) {
	if (sourse2Dproblem[iW - maxelm]) {
	//bsW = true;
	//binternalfindsituation = true;
	}
	}
	sourse2Dproblem[iW - maxelm] = !sourse2Dproblem[iW - maxelm];
	}
	if (bS) {
	//if (sosedb[iS-maxelm].MCB<ls) bsS=true;
	if (binternalsource[iS - maxelm]) {
	if (sourse2Dproblem[iS - maxelm]) {
	//bsS = true;
	//binternalfindsituation = true;
	}
	}
	sourse2Dproblem[iS - maxelm] = !sourse2Dproblem[iS - maxelm];
	}
	if (bB) {
	//if (sosedb[iB-maxelm].MCB<ls) bsB=true;
	if (binternalsource[iB - maxelm]) {
	if (sourse2Dproblem[iB - maxelm]) {
	//bsB = true;
	//binternalfindsituation = true;
	}
	}
	sourse2Dproblem[iB - maxelm] = !sourse2Dproblem[iB - maxelm];
	}
	}
	//*/



	//bsE=false; bsN=false; bsT=false; bsW=false; bsS=false; bsB=false; // ����� ������ ����� !!!

	// debug.
	/*if (binternalfindsituation) {
		printf("find internal source...\n");
		getchar();
	}*/

	/*	
    if ((iP>=25)&&(iP<=29)) {
	#if doubleintprecision == 1
		printf("bsE=%lld bsN=%lld bsT=%lld bsW=%lld bsS=%lld bsB=%lld\n",bsE,bsN,bsT,bsW,bsS,bsB);
	#else
		printf("bsE=%d bsN=%d bsT=%d bsW=%d bsS=%d bsB=%d\n",bsE,bsN,bsT,bsW,bsS,bsB);
	#endif
        
        getchar();
	}*/

	// ���������� �������� �������� ������������ ������:
	doublereal dx=0.0, dy=0.0, dz=0.0;// ����� �������� ������������ ������
	volume3D(iP, nvtx, pa, dx, dy, dz);

	TOCHKA pointP0;
	center_cord3D(iP, nvtx, pa, pointP0, 100);
	

	// ��� ���������� ����������� �������������� ���� : iP ����� ���������, ����� ��������� MCB � ��.

	doublereal Sc2D = 0.0;
	{
		if (bE) {
			if (sosedb[iE - maxelm].MCB < ls) {
				
				// ��������� ����� � ������� ������,
				// ����� ��������� ��� ����� �� ������ ������������ ������������.
				if ((sosedb[iE - maxelm].iI1>-1) && (sosedb[iE - maxelm].iI2>-1)) {

					if (fabs(prop[LAM][sosedb[iE - maxelm].iI1] - prop[LAM][sosedb[iE - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iE - maxelm]) {
							Sc2D = ((dy*dz / s[sosedb[iE - maxelm].MCB].square)*s[sosedb[iE - maxelm].MCB].power) / (dx*dy*dz);

						}
						sourse2Dproblem[iE - maxelm] = !sourse2Dproblem[iE - maxelm];
					}
					else {
						if (iP == sosedb[iE - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iE - maxelm].iI2]) {
								Sc2D = ((dy*dz / s[sosedb[iE - maxelm].MCB].square)*s[sosedb[iE - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iE - maxelm] = !sourse2Dproblem[iE - maxelm];
							}
						}
						if (iP == sosedb[iE - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iE - maxelm].iI1]) {
								Sc2D = ((dy*dz / s[sosedb[iE - maxelm].MCB].square)*s[sosedb[iE - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iE - maxelm] = !sourse2Dproblem[iE - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}

					if (iP == sosedb[iE - maxelm].iI1) {
						iE = sosedb[iE - maxelm].iI2;
					}
					else {
						iE = sosedb[iE - maxelm].iI1;
					}
					sl[iP].iE = iE;
					bE = false;
				}
				else {
					// ����� ������ ������� �� �����, �.�.
					// �������� ��������� �������� ����� �������,
					// ��� �������� ����� ������ ����� ������ 
					// ��������� ������� � �� �� � �������.
					// ��������� 11.07.2016.
					if (sosedb[iE - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iE - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** E \n");
					//getchar();
					system("PAUSE");
				}
				//bsE1 = true;

			}
			
		}
		if (bW) {
			if (sosedb[iW - maxelm].MCB < ls) {
				
				
				if ((sosedb[iW - maxelm].iI1>-1) && (sosedb[iW - maxelm].iI2>-1)) {


					if (fabs(prop[LAM][sosedb[iW - maxelm].iI1] - prop[LAM][sosedb[iW - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iW - maxelm]) {
							Sc2D = ((dy*dz / s[sosedb[iW - maxelm].MCB].square)*s[sosedb[iW - maxelm].MCB].power) / (dx*dy*dz);
						}
						sourse2Dproblem[iW - maxelm] = !sourse2Dproblem[iW - maxelm];
					}
					else {
						if (iP == sosedb[iW - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iW - maxelm].iI2]) {
								Sc2D = ((dy*dz / s[sosedb[iW - maxelm].MCB].square)*s[sosedb[iW - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iW - maxelm] = !sourse2Dproblem[iW - maxelm];
							}
						}
						if (iP == sosedb[iW - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iW - maxelm].iI1]) {
								Sc2D = ((dy*dz / s[sosedb[iW - maxelm].MCB].square)*s[sosedb[iW - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iW - maxelm] = !sourse2Dproblem[iW - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}


					if (iP == sosedb[iW - maxelm].iI1) {
						iW = sosedb[iW - maxelm].iI2;
					}
					else {
						iW = sosedb[iW - maxelm].iI1;
					}
					sl[iP].iW = iW;
					bW = false;
				}
				else {
					if (sosedb[iW - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iW - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** W\n");
					//getchar();
					system("PAUSE");
				}
				//bsW1 = true;
			}
			
		}
		if (bN) {
			if (sosedb[iN - maxelm].MCB < ls) {
				
				if((sosedb[iN - maxelm].iI1>-1) && (sosedb[iN - maxelm].iI2>-1)) {

					if (fabs(prop[LAM][sosedb[iN - maxelm].iI1] - prop[LAM][sosedb[iN - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iN - maxelm]) {
							Sc2D = ((dx*dz / s[sosedb[iN - maxelm].MCB].square)*s[sosedb[iN - maxelm].MCB].power) / (dx*dy*dz);

						}
						sourse2Dproblem[iN - maxelm] = !sourse2Dproblem[iN - maxelm];
					}
					else {
						if (iP == sosedb[iN - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iN - maxelm].iI2]) {
								Sc2D = ((dx*dz / s[sosedb[iN - maxelm].MCB].square)*s[sosedb[iN - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iN - maxelm] = !sourse2Dproblem[iN - maxelm];
							}
						}
						if (iP == sosedb[iN - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iN - maxelm].iI1]) {
								Sc2D = ((dx*dz / s[sosedb[iN - maxelm].MCB].square)*s[sosedb[iN - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iN - maxelm] = !sourse2Dproblem[iN - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}

					if (iP == sosedb[iN - maxelm].iI1) {
						iN = sosedb[iN - maxelm].iI2;
					}
					else {
						iN = sosedb[iN - maxelm].iI1;
					}
					sl[iP].iN = iN;
					bN = false;
				}
				else {
					if (sosedb[iN - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iN - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** N\n");
					//getchar();
					system("PAUSE");
				}
				//bsN1 = true;
			}
			
		}
		if (bS) {
			if (sosedb[iS -maxelm].MCB < ls) {
				


				if ((sosedb[iS - maxelm].iI1>-1) && (sosedb[iS - maxelm].iI2>-1)) {

					if (fabs(prop[LAM][sosedb[iS - maxelm].iI1] - prop[LAM][sosedb[iS - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iS - maxelm]) {
							Sc2D = ((dx*dz / s[sosedb[iS - maxelm].MCB].square)*s[sosedb[iS - maxelm].MCB].power) / (dx*dy*dz);
						}
						sourse2Dproblem[iS - maxelm] = !sourse2Dproblem[iS - maxelm];
					}
					else {
						if (iP == sosedb[iS - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iS - maxelm].iI2]) {
								Sc2D = ((dx*dz / s[sosedb[iS - maxelm].MCB].square)*s[sosedb[iS - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iS - maxelm] = !sourse2Dproblem[iS - maxelm];
							}
						}
						if (iP == sosedb[iS - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iS - maxelm].iI1]) {
								Sc2D = ((dx*dz / s[sosedb[iS - maxelm].MCB].square)*s[sosedb[iS - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iS - maxelm] = !sourse2Dproblem[iS - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}



					if (iP == sosedb[iS - maxelm].iI1) {
						iS = sosedb[iS - maxelm].iI2;
					}
					else {
						iS = sosedb[iS - maxelm].iI1;
					}
					sl[iP].iS = iS;
					bS = false;
				}
				else {
					if (sosedb[iS - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iS - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** S\n");
					//getchar();
					system("PAUSE");
				}
				//bsS1 = true;
			}
			
		}
		if (bT) {
			if (sosedb[iT - maxelm].MCB < ls) {
				



				if ((sosedb[iT - maxelm].iI1>-1) && (sosedb[iT - maxelm].iI2>-1)) {

					if (fabs(prop[LAM][sosedb[iT - maxelm].iI1] - prop[LAM][sosedb[iT - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iT - maxelm]) {
							Sc2D = ((dx*dy / s[sosedb[iT - maxelm].MCB].square)*s[sosedb[iT - maxelm].MCB].power) / (dx*dy*dz);

						}
						sourse2Dproblem[iT - maxelm] = !sourse2Dproblem[iT - maxelm];
					}
					else {
						if (iP == sosedb[iT - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iT - maxelm].iI2]) {
								
								Sc2D = ((dx*dy / s[sosedb[iT - maxelm].MCB].square)*s[sosedb[iT - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iT - maxelm] = !sourse2Dproblem[iT - maxelm];
							}
						}
						if (iP == sosedb[iT - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iT - maxelm].iI1]) {
								Sc2D = ((dx*dy / s[sosedb[iT - maxelm].MCB].square)*s[sosedb[iT - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iT - maxelm] = !sourse2Dproblem[iT - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}


					if (iP == sosedb[iT - maxelm].iI1) {
						iT = sosedb[iT - maxelm].iI2;
					}
					else {
						iT = sosedb[iT - maxelm].iI1;
					}
					sl[iP].iT = iT;
					bT = false;
				}
				else {
					if (sosedb[iT - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iT - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** T\n");
					//getchar();
					system("PAUSE");
				}
				//bsT1 = true;
			}
			
		}
		if (bB) {
			if (sosedb[iB - maxelm].MCB < ls) {
				

				if ((sosedb[iB - maxelm].iI1>-1) && (sosedb[iB - maxelm].iI2>-1)) {
				
					if (fabs(prop[LAM][sosedb[iB - maxelm].iI1] - prop[LAM][sosedb[iB - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iB - maxelm]) {
							Sc2D = ((dx*dy / s[sosedb[iB - maxelm].MCB].square)*s[sosedb[iB - maxelm].MCB].power) / (dx*dy*dz);
						}
						sourse2Dproblem[iB - maxelm] = !sourse2Dproblem[iB - maxelm];
					}
					else {
						if (iP == sosedb[iB - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iB - maxelm].iI2]) {
								Sc2D = ((dx*dy / s[sosedb[iB - maxelm].MCB].square)*s[sosedb[iB - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iB - maxelm] = !sourse2Dproblem[iB - maxelm];
							}
						}
						if (iP == sosedb[iB - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iB - maxelm].iI1]) {
								Sc2D = ((dx*dy / s[sosedb[iB - maxelm].MCB].square)*s[sosedb[iB - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iB - maxelm] = !sourse2Dproblem[iB - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}
					
					if (iP == sosedb[iB - maxelm].iI1) {
						iB = sosedb[iB - maxelm].iI2;
					}
					else {
						iB = sosedb[iB - maxelm].iI1;
					}
					sl[iP].iB = iB;
					bB = false;
				}
				else {
					if (sosedb[iB - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iB - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** B\n");
					//getchar();
					system("PAUSE");
				}
				//bsB1 = true;
			}
			
		}

		if (bE2) {
			if (sosedb[iE2 - maxelm].MCB < ls) {

				// ��������� ����� � ������� ������,
				// ����� ��������� ��� ����� �� ������ ������������ ������������.
				if ((sosedb[iE2 - maxelm].iI1>-1) && (sosedb[iE2 - maxelm].iI2>-1)) {

					if (fabs(prop[LAM][sosedb[iE2 - maxelm].iI1] - prop[LAM][sosedb[iE2 - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iE2 - maxelm]) {
							Sc2D = ((dy*dz / s[sosedb[iE2 - maxelm].MCB].square)*s[sosedb[iE2 - maxelm].MCB].power) / (dx*dy*dz);

						}
						sourse2Dproblem[iE2 - maxelm] = !sourse2Dproblem[iE2 - maxelm];
					}
					else {
						if (iP == sosedb[iE2 - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iE2 - maxelm].iI2]) {
								Sc2D = ((dy*dz / s[sosedb[iE2 - maxelm].MCB].square)*s[sosedb[iE2 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iE2 - maxelm] = !sourse2Dproblem[iE2 - maxelm];
							}
						}
						if (iP == sosedb[iE2 - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iE2 - maxelm].iI1]) {
								Sc2D = ((dy*dz / s[sosedb[iE2 - maxelm].MCB].square)*s[sosedb[iE2 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iE2 - maxelm] = !sourse2Dproblem[iE2 - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}

					if (iP == sosedb[iE2 - maxelm].iI1) {
						iE2 = sosedb[iE2 - maxelm].iI2;
					}
					else {
						iE2 = sosedb[iE2 - maxelm].iI1;
					}
					sl[iP].iE2 = iE2;
					bE2 = false;
				}
				else {
					// ����� ������ ������� �� �����, �.�.
					// �������� ��������� �������� ����� �������,
					// ��� �������� ����� ������ ����� ������ 
					// ��������� ������� � �� �� � �������.
					// ��������� 11.07.2016.
					if (sosedb[iE2 - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iE2 - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** E \n");
					//getchar();
					system("PAUSE");
				}
				//bsE1 = true;

			}

		}
		if (bW2) {
			if (sosedb[iW2 - maxelm].MCB < ls) {


				if ((sosedb[iW2 - maxelm].iI1>-1) && (sosedb[iW2 - maxelm].iI2>-1)) {


					if (fabs(prop[LAM][sosedb[iW2 - maxelm].iI1] - prop[LAM][sosedb[iW2 - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iW2 - maxelm]) {
							Sc2D = ((dy*dz / s[sosedb[iW2 - maxelm].MCB].square)*s[sosedb[iW2 - maxelm].MCB].power) / (dx*dy*dz);
						}
						sourse2Dproblem[iW2 - maxelm] = !sourse2Dproblem[iW2 - maxelm];
					}
					else {
						if (iP == sosedb[iW2 - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iW2 - maxelm].iI2]) {
								Sc2D = ((dy*dz / s[sosedb[iW2 - maxelm].MCB].square)*s[sosedb[iW2 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iW2 - maxelm] = !sourse2Dproblem[iW2 - maxelm];
							}
						}
						if (iP == sosedb[iW2 - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iW2 - maxelm].iI1]) {
								Sc2D = ((dy*dz / s[sosedb[iW2 - maxelm].MCB].square)*s[sosedb[iW2 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iW2 - maxelm] = !sourse2Dproblem[iW2 - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}


					if (iP == sosedb[iW2 - maxelm].iI1) {
						iW2 = sosedb[iW2 - maxelm].iI2;
					}
					else {
						iW2 = sosedb[iW2 - maxelm].iI1;
					}
					sl[iP].iW2 = iW2;
					bW2 = false;
				}
				else {
					if (sosedb[iW2 - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iW2 - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** W\n");
					//getchar();
					system("PAUSE");
				}
				//bsW1 = true;
			}

		}
		if (bN2) {
			if (sosedb[iN2 - maxelm].MCB < ls) {

				if ((sosedb[iN2 - maxelm].iI1>-1) && (sosedb[iN2 - maxelm].iI2>-1)) {

					if (fabs(prop[LAM][sosedb[iN2 - maxelm].iI1] - prop[LAM][sosedb[iN2 - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iN2 - maxelm]) {
							Sc2D = ((dx*dz / s[sosedb[iN2 - maxelm].MCB].square)*s[sosedb[iN2 - maxelm].MCB].power) / (dx*dy*dz);

						}
						sourse2Dproblem[iN2 - maxelm] = !sourse2Dproblem[iN2 - maxelm];
					}
					else {
						if (iP == sosedb[iN2 - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iN2 - maxelm].iI2]) {
								Sc2D = ((dx*dz / s[sosedb[iN2 - maxelm].MCB].square)*s[sosedb[iN2 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iN2 - maxelm] = !sourse2Dproblem[iN2 - maxelm];
							}
						}
						if (iP == sosedb[iN2 - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iN2 - maxelm].iI1]) {
								Sc2D = ((dx*dz / s[sosedb[iN2 - maxelm].MCB].square)*s[sosedb[iN2 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iN2 - maxelm] = !sourse2Dproblem[iN2 - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}

					if (iP == sosedb[iN2 - maxelm].iI1) {
						iN2 = sosedb[iN2 - maxelm].iI2;
					}
					else {
						iN2 = sosedb[iN2 - maxelm].iI1;
					}
					sl[iP].iN2 = iN2;
					bN2 = false;
				}
				else {
					if (sosedb[iN2 - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iN2 - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** N\n");
					//getchar();
					system("PAUSE");
				}
				//bsN1 = true;
			}

		}
		if (bS2) {
			if (sosedb[iS2 - maxelm].MCB < ls) {



				if ((sosedb[iS2 - maxelm].iI1>-1) && (sosedb[iS2 - maxelm].iI2>-1)) {

					if (fabs(prop[LAM][sosedb[iS2 - maxelm].iI1] - prop[LAM][sosedb[iS2 - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iS2 - maxelm]) {
							Sc2D = ((dx*dz / s[sosedb[iS2 - maxelm].MCB].square)*s[sosedb[iS2 - maxelm].MCB].power) / (dx*dy*dz);
						}
						sourse2Dproblem[iS2 - maxelm] = !sourse2Dproblem[iS2 - maxelm];
					}
					else {
						if (iP == sosedb[iS2 - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iS2 - maxelm].iI2]) {
								Sc2D = ((dx*dz / s[sosedb[iS2 - maxelm].MCB].square)*s[sosedb[iS2 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iS2 - maxelm] = !sourse2Dproblem[iS2 - maxelm];
							}
						}
						if (iP == sosedb[iS2 - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iS2 - maxelm].iI1]) {
								Sc2D = ((dx*dz / s[sosedb[iS2 - maxelm].MCB].square)*s[sosedb[iS2 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iS2 - maxelm] = !sourse2Dproblem[iS2 - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}



					if (iP == sosedb[iS2 - maxelm].iI1) {
						iS2 = sosedb[iS2 - maxelm].iI2;
					}
					else {
						iS2 = sosedb[iS2 - maxelm].iI1;
					}
					sl[iP].iS2 = iS2;
					bS2 = false;
				}
				else {
					if (sosedb[iS2 - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iS2 - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** S\n");
					//getchar();
					system("PAUSE");
				}
				//bsS1 = true;
			}

		}
		if (bT2) {
			if (sosedb[iT2 - maxelm].MCB < ls) {




				if ((sosedb[iT2 - maxelm].iI1>-1) && (sosedb[iT2 - maxelm].iI2>-1)) {

					if (fabs(prop[LAM][sosedb[iT2 - maxelm].iI1] - prop[LAM][sosedb[iT2 - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iT2 - maxelm]) {
							Sc2D = ((dx*dy / s[sosedb[iT2 - maxelm].MCB].square)*s[sosedb[iT2 - maxelm].MCB].power) / (dx*dy*dz);

						}
						sourse2Dproblem[iT2 - maxelm] = !sourse2Dproblem[iT2 - maxelm];
					}
					else {
						if (iP == sosedb[iT2 - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iT2 - maxelm].iI2]) {

								Sc2D = ((dx*dy / s[sosedb[iT2 - maxelm].MCB].square)*s[sosedb[iT2 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iT2 - maxelm] = !sourse2Dproblem[iT2 - maxelm];
							}
						}
						if (iP == sosedb[iT2 - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iT2 - maxelm].iI1]) {
								Sc2D = ((dx*dy / s[sosedb[iT2 - maxelm].MCB].square)*s[sosedb[iT2 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iT2 - maxelm] = !sourse2Dproblem[iT2 - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}


					if (iP == sosedb[iT2 - maxelm].iI1) {
						iT2 = sosedb[iT2 - maxelm].iI2;
					}
					else {
						iT2 = sosedb[iT2 - maxelm].iI1;
					}
					sl[iP].iT2 = iT2;
					bT2 = false;
				}
				else {
					if (sosedb[iT2 - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iT2 - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** T\n");
					//getchar();
					system("PAUSE");
				}
				//bsT1 = true;
			}

		}
		if (bB2) {
			if (sosedb[iB2 - maxelm].MCB < ls) {


				if ((sosedb[iB2 - maxelm].iI1>-1) && (sosedb[iB2 - maxelm].iI2>-1)) {

					if (fabs(prop[LAM][sosedb[iB2 - maxelm].iI1] - prop[LAM][sosedb[iB2 - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iB2 - maxelm]) {
							Sc2D = ((dx*dy / s[sosedb[iB2 - maxelm].MCB].square)*s[sosedb[iB2 - maxelm].MCB].power) / (dx*dy*dz);
						}
						sourse2Dproblem[iB2 - maxelm] = !sourse2Dproblem[iB2 - maxelm];
					}
					else {
						if (iP == sosedb[iB2 - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iB2 - maxelm].iI2]) {
								Sc2D = ((dx*dy / s[sosedb[iB2 - maxelm].MCB].square)*s[sosedb[iB2 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iB2 - maxelm] = !sourse2Dproblem[iB2 - maxelm];
							}
						}
						if (iP == sosedb[iB2 - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iB2 - maxelm].iI1]) {
								Sc2D = ((dx*dy / s[sosedb[iB2 - maxelm].MCB].square)*s[sosedb[iB2 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iB2 - maxelm] = !sourse2Dproblem[iB2 - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}

					if (iP == sosedb[iB2 - maxelm].iI1) {
						iB2 = sosedb[iB2 - maxelm].iI2;
					}
					else {
						iB2 = sosedb[iB2 - maxelm].iI1;
					}
					sl[iP].iB2 = iB2;
					bB2 = false;
				}
				else {
					if (sosedb[iB2 - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iB2 - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** B\n");
					//getchar();
					system("PAUSE");
				}
				//bsB1 = true;
			}

		}

		if (bE3) {
			if (sosedb[iE3 - maxelm].MCB < ls) {

				// ��������� ����� � ������� ������,
				// ����� ��������� ��� ����� �� ������ ������������ ������������.
				if ((sosedb[iE3 - maxelm].iI1>-1) && (sosedb[iE3 - maxelm].iI2>-1)) {

					if (fabs(prop[LAM][sosedb[iE3 - maxelm].iI1] - prop[LAM][sosedb[iE3 - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iE3 - maxelm]) {
							Sc2D = ((dy*dz / s[sosedb[iE3 - maxelm].MCB].square)*s[sosedb[iE3 - maxelm].MCB].power) / (dx*dy*dz);

						}
						sourse2Dproblem[iE3 - maxelm] = !sourse2Dproblem[iE3 - maxelm];
					}
					else {
						if (iP == sosedb[iE3 - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iE3 - maxelm].iI2]) {
								Sc2D = ((dy*dz / s[sosedb[iE3 - maxelm].MCB].square)*s[sosedb[iE3 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iE3 - maxelm] = !sourse2Dproblem[iE3 - maxelm];
							}
						}
						if (iP == sosedb[iE3 - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iE3 - maxelm].iI1]) {
								Sc2D = ((dy*dz / s[sosedb[iE3 - maxelm].MCB].square)*s[sosedb[iE3 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iE3 - maxelm] = !sourse2Dproblem[iE3 - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}

					if (iP == sosedb[iE3 - maxelm].iI1) {
						iE3 = sosedb[iE3 - maxelm].iI2;
					}
					else {
						iE3 = sosedb[iE3 - maxelm].iI1;
					}
					sl[iP].iE3 = iE3;
					bE3 = false;
				}
				else {
					// ����� ������ ������� �� �����, �.�.
					// �������� ��������� �������� ����� �������,
					// ��� �������� ����� ������ ����� ������ 
					// ��������� ������� � �� �� � �������.
					// ��������� 11.07.2016.
					if (sosedb[iE3 - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iE3 - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** E \n");
					//getchar();
					system("PAUSE");
				}
				//bsE1 = true;

			}

		}
		if (bW3) {
			if (sosedb[iW3 - maxelm].MCB < ls) {


				if ((sosedb[iW3 - maxelm].iI1>-1) && (sosedb[iW3 - maxelm].iI2>-1)) {


					if (fabs(prop[LAM][sosedb[iW3 - maxelm].iI1] - prop[LAM][sosedb[iW3 - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iW3 - maxelm]) {
							Sc2D = ((dy*dz / s[sosedb[iW3 - maxelm].MCB].square)*s[sosedb[iW3 - maxelm].MCB].power) / (dx*dy*dz);
						}
						sourse2Dproblem[iW3 - maxelm] = !sourse2Dproblem[iW3 - maxelm];
					}
					else {
						if (iP == sosedb[iW3 - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iW3 - maxelm].iI2]) {
								Sc2D = ((dy*dz / s[sosedb[iW3 - maxelm].MCB].square)*s[sosedb[iW3 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iW3 - maxelm] = !sourse2Dproblem[iW3 - maxelm];
							}
						}
						if (iP == sosedb[iW3 - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iW3 - maxelm].iI1]) {
								Sc2D = ((dy*dz / s[sosedb[iW3 - maxelm].MCB].square)*s[sosedb[iW3 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iW3 - maxelm] = !sourse2Dproblem[iW3 - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}


					if (iP == sosedb[iW3 - maxelm].iI1) {
						iW3 = sosedb[iW3 - maxelm].iI2;
					}
					else {
						iW3 = sosedb[iW3 - maxelm].iI1;
					}
					sl[iP].iW3 = iW3;
					bW3 = false;
				}
				else {
					if (sosedb[iW3 - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iW3 - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** W\n");
					//getchar();
					system("PAUSE");
				}
				//bsW1 = true;
			}

		}
		if (bN3) {
			if (sosedb[iN3 - maxelm].MCB < ls) {

				if ((sosedb[iN3 - maxelm].iI1>-1) && (sosedb[iN3 - maxelm].iI2>-1)) {

					if (fabs(prop[LAM][sosedb[iN3 - maxelm].iI1] - prop[LAM][sosedb[iN3 - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iN3 - maxelm]) {
							Sc2D = ((dx*dz / s[sosedb[iN3 - maxelm].MCB].square)*s[sosedb[iN3 - maxelm].MCB].power) / (dx*dy*dz);

						}
						sourse2Dproblem[iN3 - maxelm] = !sourse2Dproblem[iN3 - maxelm];
					}
					else {
						if (iP == sosedb[iN3 - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iN3 - maxelm].iI2]) {
								Sc2D = ((dx*dz / s[sosedb[iN3 - maxelm].MCB].square)*s[sosedb[iN3 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iN3 - maxelm] = !sourse2Dproblem[iN3 - maxelm];
							}
						}
						if (iP == sosedb[iN3 - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iN3 - maxelm].iI1]) {
								Sc2D = ((dx*dz / s[sosedb[iN3 - maxelm].MCB].square)*s[sosedb[iN3 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iN3 - maxelm] = !sourse2Dproblem[iN3 - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}

					if (iP == sosedb[iN3 - maxelm].iI1) {
						iN3 = sosedb[iN3 - maxelm].iI2;
					}
					else {
						iN3 = sosedb[iN3 - maxelm].iI1;
					}
					sl[iP].iN3 = iN3;
					bN3 = false;
				}
				else {
					if (sosedb[iN3 - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iN3 - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** N\n");
					//getchar();
					system("PAUSE");
				}
				//bsN1 = true;
			}

		}
		if (bS3) {
			if (sosedb[iS3 - maxelm].MCB < ls) {



				if ((sosedb[iS3 - maxelm].iI1>-1) && (sosedb[iS3 - maxelm].iI2>-1)) {

					if (fabs(prop[LAM][sosedb[iS3 - maxelm].iI1] - prop[LAM][sosedb[iS3 - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iS3 - maxelm]) {
							Sc2D = ((dx*dz / s[sosedb[iS3 - maxelm].MCB].square)*s[sosedb[iS3 - maxelm].MCB].power) / (dx*dy*dz);
						}
						sourse2Dproblem[iS3 - maxelm] = !sourse2Dproblem[iS3 - maxelm];
					}
					else {
						if (iP == sosedb[iS3 - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iS3 - maxelm].iI2]) {
								Sc2D = ((dx*dz / s[sosedb[iS3 - maxelm].MCB].square)*s[sosedb[iS3 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iS3 - maxelm] = !sourse2Dproblem[iS3 - maxelm];
							}
						}
						if (iP == sosedb[iS3 - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iS3 - maxelm].iI1]) {
								Sc2D = ((dx*dz / s[sosedb[iS3 - maxelm].MCB].square)*s[sosedb[iS3 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iS3 - maxelm] = !sourse2Dproblem[iS3 - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}



					if (iP == sosedb[iS3 - maxelm].iI1) {
						iS3 = sosedb[iS3 - maxelm].iI2;
					}
					else {
						iS3 = sosedb[iS3 - maxelm].iI1;
					}
					sl[iP].iS3 = iS3;
					bS3 = false;
				}
				else {
					if (sosedb[iS3 - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iS3 - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** S\n");
					//getchar();
					system("PAUSE");
				}
				//bsS1 = true;
			}

		}
		if (bT3) {
			if (sosedb[iT3 - maxelm].MCB < ls) {




				if ((sosedb[iT3 - maxelm].iI1>-1) && (sosedb[iT3 - maxelm].iI2>-1)) {

					if (fabs(prop[LAM][sosedb[iT3 - maxelm].iI1] - prop[LAM][sosedb[iT3 - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iT3 - maxelm]) {
							Sc2D = ((dx*dy / s[sosedb[iT3 - maxelm].MCB].square)*s[sosedb[iT3 - maxelm].MCB].power) / (dx*dy*dz);

						}
						sourse2Dproblem[iT3 - maxelm] = !sourse2Dproblem[iT3 - maxelm];
					}
					else {
						if (iP == sosedb[iT3 - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iT3 - maxelm].iI2]) {

								Sc2D = ((dx*dy / s[sosedb[iT3 - maxelm].MCB].square)*s[sosedb[iT3 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iT3 - maxelm] = !sourse2Dproblem[iT3 - maxelm];
							}
						}
						if (iP == sosedb[iT3 - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iT3 - maxelm].iI1]) {
								Sc2D = ((dx*dy / s[sosedb[iT3 - maxelm].MCB].square)*s[sosedb[iT3 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iT3 - maxelm] = !sourse2Dproblem[iT3 - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}


					if (iP == sosedb[iT3 - maxelm].iI1) {
						iT3 = sosedb[iT3 - maxelm].iI2;
					}
					else {
						iT3 = sosedb[iT3 - maxelm].iI1;
					}
					sl[iP].iT3 = iT3;
					bT3 = false;
				}
				else {
					if (sosedb[iT3 - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iT3 - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** T\n");
					//getchar();
					system("PAUSE");
				}
				//bsT1 = true;
			}

		}
		if (bB3) {
			if (sosedb[iB3 - maxelm].MCB < ls) {


				if ((sosedb[iB3 - maxelm].iI1>-1) && (sosedb[iB3 - maxelm].iI2>-1)) {

					if (fabs(prop[LAM][sosedb[iB3 - maxelm].iI1] - prop[LAM][sosedb[iB3 - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iB3 - maxelm]) {
							Sc2D = ((dx*dy / s[sosedb[iB3 - maxelm].MCB].square)*s[sosedb[iB3 - maxelm].MCB].power) / (dx*dy*dz);
						}
						sourse2Dproblem[iB3 - maxelm] = !sourse2Dproblem[iB3 - maxelm];
					}
					else {
						if (iP == sosedb[iB3 - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iB3 - maxelm].iI2]) {
								Sc2D = ((dx*dy / s[sosedb[iB3 - maxelm].MCB].square)*s[sosedb[iB3 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iB3 - maxelm] = !sourse2Dproblem[iB3 - maxelm];
							}
						}
						if (iP == sosedb[iB3 - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iB3 - maxelm].iI1]) {
								Sc2D = ((dx*dy / s[sosedb[iB3 - maxelm].MCB].square)*s[sosedb[iB3 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iB3 - maxelm] = !sourse2Dproblem[iB3 - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}

					if (iP == sosedb[iB3 - maxelm].iI1) {
						iB3 = sosedb[iB3 - maxelm].iI2;
					}
					else {
						iB3 = sosedb[iB3 - maxelm].iI1;
					}
					sl[iP].iB3 = iB3;
					bB3 = false;
				}
				else {
					if (sosedb[iB3 - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iB3 - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** B\n");
					//getchar();
					system("PAUSE");
				}
				//bsB1 = true;
			}

		}

		if (bE4) {
			if (sosedb[iE4 - maxelm].MCB < ls) {

				// ��������� ����� � ������� ������,
				// ����� ��������� ��� ����� �� ������ ������������ ������������.
				if ((sosedb[iE4 - maxelm].iI1>-1) && (sosedb[iE4 - maxelm].iI2>-1)) {

					if (fabs(prop[LAM][sosedb[iE4 - maxelm].iI1] - prop[LAM][sosedb[iE4 - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iE4 - maxelm]) {
							Sc2D = ((dy*dz / s[sosedb[iE4 - maxelm].MCB].square)*s[sosedb[iE4 - maxelm].MCB].power) / (dx*dy*dz);

						}
						sourse2Dproblem[iE4 - maxelm] = !sourse2Dproblem[iE4 - maxelm];
					}
					else {
						if (iP == sosedb[iE4 - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iE4 - maxelm].iI2]) {
								Sc2D = ((dy*dz / s[sosedb[iE4 - maxelm].MCB].square)*s[sosedb[iE4 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iE4 - maxelm] = !sourse2Dproblem[iE4 - maxelm];
							}
						}
						if (iP == sosedb[iE4 - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iE4 - maxelm].iI1]) {
								Sc2D = ((dy*dz / s[sosedb[iE4 - maxelm].MCB].square)*s[sosedb[iE4 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iE4 - maxelm] = !sourse2Dproblem[iE4 - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}

					if (iP == sosedb[iE4 - maxelm].iI1) {
						iE4 = sosedb[iE4 - maxelm].iI2;
					}
					else {
						iE4 = sosedb[iE4 - maxelm].iI1;
					}
					sl[iP].iE4 = iE4;
					bE4 = false;
				}
				else {
					// ����� ������ ������� �� �����, �.�.
					// �������� ��������� �������� ����� �������,
					// ��� �������� ����� ������ ����� ������ 
					// ��������� ������� � �� �� � �������.
					// ��������� 11.07.2016.
					if (sosedb[iE4 - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iE4 - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** E \n");
					//getchar();
					system("PAUSE");
				}
				//bsE1 = true;

			}

		}
		if (bW4) {
			if (sosedb[iW4 - maxelm].MCB < ls) {


				if ((sosedb[iW4 - maxelm].iI1>-1) && (sosedb[iW4 - maxelm].iI2>-1)) {


					if (fabs(prop[LAM][sosedb[iW4 - maxelm].iI1] - prop[LAM][sosedb[iW4 - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iW4 - maxelm]) {
							Sc2D = ((dy*dz / s[sosedb[iW4 - maxelm].MCB].square)*s[sosedb[iW4 - maxelm].MCB].power) / (dx*dy*dz);
						}
						sourse2Dproblem[iW4 - maxelm] = !sourse2Dproblem[iW4 - maxelm];
					}
					else {
						if (iP == sosedb[iW4 - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iW4 - maxelm].iI2]) {
								Sc2D = ((dy*dz / s[sosedb[iW4 - maxelm].MCB].square)*s[sosedb[iW4 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iW4 - maxelm] = !sourse2Dproblem[iW4 - maxelm];
							}
						}
						if (iP == sosedb[iW4 - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iW4 - maxelm].iI1]) {
								Sc2D = ((dy*dz / s[sosedb[iW4 - maxelm].MCB].square)*s[sosedb[iW4 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iW4 - maxelm] = !sourse2Dproblem[iW4 - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}


					if (iP == sosedb[iW4 - maxelm].iI1) {
						iW4 = sosedb[iW4 - maxelm].iI2;
					}
					else {
						iW4 = sosedb[iW4 - maxelm].iI1;
					}
					sl[iP].iW4 = iW4;
					bW4 = false;
				}
				else {
					if (sosedb[iW4 - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iW4 - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** W\n");
					//getchar();
					system("PAUSE");
				}
				//bsW1 = true;
			}

		}
		if (bN4) {
			if (sosedb[iN4 - maxelm].MCB < ls) {

				if ((sosedb[iN4 - maxelm].iI1>-1) && (sosedb[iN4 - maxelm].iI2>-1)) {

					if (fabs(prop[LAM][sosedb[iN4 - maxelm].iI1] - prop[LAM][sosedb[iN4 - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iN4 - maxelm]) {
							Sc2D = ((dx*dz / s[sosedb[iN4 - maxelm].MCB].square)*s[sosedb[iN4 - maxelm].MCB].power) / (dx*dy*dz);

						}
						sourse2Dproblem[iN4 - maxelm] = !sourse2Dproblem[iN4 - maxelm];
					}
					else {
						if (iP == sosedb[iN4 - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iN4 - maxelm].iI2]) {
								Sc2D = ((dx*dz / s[sosedb[iN4 - maxelm].MCB].square)*s[sosedb[iN4 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iN4 - maxelm] = !sourse2Dproblem[iN4 - maxelm];
							}
						}
						if (iP == sosedb[iN4 - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iN4 - maxelm].iI1]) {
								Sc2D = ((dx*dz / s[sosedb[iN4 - maxelm].MCB].square)*s[sosedb[iN4 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iN4 - maxelm] = !sourse2Dproblem[iN4 - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}

					if (iP == sosedb[iN4 - maxelm].iI1) {
						iN4 = sosedb[iN4 - maxelm].iI2;
					}
					else {
						iN4 = sosedb[iN4 - maxelm].iI1;
					}
					sl[iP].iN4 = iN4;
					bN4 = false;
				}
				else {
					if (sosedb[iN4 - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iN4 - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** N\n");
					//getchar();
					system("PAUSE");
				}
				//bsN1 = true;
			}

		}
		if (bS4) {
			if (sosedb[iS4 - maxelm].MCB < ls) {



				if ((sosedb[iS4 - maxelm].iI1>-1) && (sosedb[iS4 - maxelm].iI2>-1)) {

					if (fabs(prop[LAM][sosedb[iS4 - maxelm].iI1] - prop[LAM][sosedb[iS4 - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iS4 - maxelm]) {
							Sc2D = ((dx*dz / s[sosedb[iS4 - maxelm].MCB].square)*s[sosedb[iS4 - maxelm].MCB].power) / (dx*dy*dz);
						}
						sourse2Dproblem[iS4 - maxelm] = !sourse2Dproblem[iS4 - maxelm];
					}
					else {
						if (iP == sosedb[iS4 - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iS4 - maxelm].iI2]) {
								Sc2D = ((dx*dz / s[sosedb[iS4 - maxelm].MCB].square)*s[sosedb[iS4 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iS4 - maxelm] = !sourse2Dproblem[iS4 - maxelm];
							}
						}
						if (iP == sosedb[iS4 - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iS4 - maxelm].iI1]) {
								Sc2D = ((dx*dz / s[sosedb[iS4 - maxelm].MCB].square)*s[sosedb[iS4 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iS4 - maxelm] = !sourse2Dproblem[iS4 - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}



					if (iP == sosedb[iS4 - maxelm].iI1) {
						iS4 = sosedb[iS4 - maxelm].iI2;
					}
					else {
						iS4 = sosedb[iS4 - maxelm].iI1;
					}
					sl[iP].iS4 = iS4;
					bS4 = false;
				}
				else {
					if (sosedb[iS4 - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iS4 - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** S\n");
					//getchar();
					system("PAUSE");
				}
				//bsS1 = true;
			}

		}
		if (bT4) {
			if (sosedb[iT4 - maxelm].MCB < ls) {




				if ((sosedb[iT4 - maxelm].iI1>-1) && (sosedb[iT4 - maxelm].iI2>-1)) {

					if (fabs(prop[LAM][sosedb[iT4 - maxelm].iI1] - prop[LAM][sosedb[iT4 - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iT4 - maxelm]) {
							Sc2D = ((dx*dy / s[sosedb[iT4 - maxelm].MCB].square)*s[sosedb[iT4 - maxelm].MCB].power) / (dx*dy*dz);

						}
						sourse2Dproblem[iT4 - maxelm] = !sourse2Dproblem[iT4 - maxelm];
					}
					else {
						if (iP == sosedb[iT4 - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iT4 - maxelm].iI2]) {

								Sc2D = ((dx*dy / s[sosedb[iT4 - maxelm].MCB].square)*s[sosedb[iT4 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iT4 - maxelm] = !sourse2Dproblem[iT4 - maxelm];
							}
						}
						if (iP == sosedb[iT4 - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iT4 - maxelm].iI1]) {
								Sc2D = ((dx*dy / s[sosedb[iT4 - maxelm].MCB].square)*s[sosedb[iT4 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iT4 - maxelm] = !sourse2Dproblem[iT4 - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}


					if (iP == sosedb[iT4 - maxelm].iI1) {
						iT4 = sosedb[iT4 - maxelm].iI2;
					}
					else {
						iT4 = sosedb[iT4 - maxelm].iI1;
					}
					sl[iP].iT4 = iT4;
					bT4 = false;
				}
				else {
					if (sosedb[iT4 - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iT4 - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** T\n");
					//getchar();
					system("PAUSE");
				}
				//bsT1 = true;
			}

		}
		if (bB4) {
			if (sosedb[iB4 - maxelm].MCB < ls) {


				if ((sosedb[iB4 - maxelm].iI1>-1) && (sosedb[iB4 - maxelm].iI2>-1)) {

					if (fabs(prop[LAM][sosedb[iB4 - maxelm].iI1] - prop[LAM][sosedb[iB4 - maxelm].iI2]) < 1.0e-40) {
						if (sourse2Dproblem[iB4 - maxelm]) {
							Sc2D = ((dx*dy / s[sosedb[iB4 - maxelm].MCB].square)*s[sosedb[iB4 - maxelm].MCB].power) / (dx*dy*dz);
						}
						sourse2Dproblem[iB4 - maxelm] = !sourse2Dproblem[iB4 - maxelm];
					}
					else {
						if (iP == sosedb[iB4 - maxelm].iI1) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iB4 - maxelm].iI2]) {
								Sc2D = ((dx*dy / s[sosedb[iB4 - maxelm].MCB].square)*s[sosedb[iB4 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iB4 - maxelm] = !sourse2Dproblem[iB4 - maxelm];
							}
						}
						if (iP == sosedb[iB4 - maxelm].iI2) {
							if (prop[LAM][iP] < prop[LAM][sosedb[iB4 - maxelm].iI1]) {
								Sc2D = ((dx*dy / s[sosedb[iB4 - maxelm].MCB].square)*s[sosedb[iB4 - maxelm].MCB].power) / (dx*dy*dz);
								sourse2Dproblem[iB4 - maxelm] = !sourse2Dproblem[iB4 - maxelm];
							}
						}
						// ����� �� ����� ������ � ����� ������ ����������������� � ��� ����� ����� ����� iP �����
						// � ���� ������������ � �������� ������ � ������� �����������������.
					}

					if (iP == sosedb[iB4 - maxelm].iI1) {
						iB4 = sosedb[iB4 - maxelm].iI2;
					}
					else {
						iB4 = sosedb[iB4 - maxelm].iI1;
					}
					sl[iP].iB4 = iB4;
					bB4 = false;
				}
				else {
					if (sosedb[iB4 - maxelm].iI1 > -1) {
						printf("iI1\n");
					}
					if (sosedb[iB4 - maxelm].iI2 > -1) {
						printf("iI2\n");
					}
					printf("*** B\n");
					//getchar();
					system("PAUSE");
				}
				//bsB1 = true;
			}

		}


	}
	// 11.05.2017
	if (poweron_multiplier_sequence > 0.0) {
		// Sc2D != 0.0
	}
	else {
		Sc2D *= 0.0;
	}
	//Sc2D *= poweron_multiplier_sequence; // �� ������ ���� �������� ��������� (��� ����� ��� square wave ��������).
	//printf("%e %e %e\n",dx,dy,dz); // debug GOOD
	//getchar();

	doublereal dxe=0.5*dx, dxw=0.5*dx, dyn=0.5*dy, dys=0.5*dy, dzt=0.5*dz, dzb=0.5*dz;
	
    // �.�. �������� ��������� ������ ����, �� ����� ��� ������������
	// x - direction
	if (iE > -1) {
		if (!bE) dxe = 0.5*(pa[nvtx[1][iE] - 1].x + pa[nvtx[0][iE] - 1].x);
		if (!bE) dxe -= 0.5*(pa[nvtx[1][iP] - 1].x + pa[nvtx[0][iP] - 1].x);
	}
	if (iW > -1) {
		if (!bW) dxw = 0.5*(pa[nvtx[1][iP] - 1].x + pa[nvtx[0][iP] - 1].x);
		if (!bW) dxw -= 0.5*(pa[nvtx[1][iW] - 1].x + pa[nvtx[0][iW] - 1].x);
	}
	// y - direction
	if (iN > -1) {
		if (!bN) dyn = 0.5*(pa[nvtx[2][iN] - 1].y + pa[nvtx[0][iN] - 1].y);
		if (!bN) dyn -= 0.5*(pa[nvtx[2][iP] - 1].y + pa[nvtx[0][iP] - 1].y);
	}
	if (iS > -1) {
		if (!bS) dys = 0.5*(pa[nvtx[2][iP] - 1].y + pa[nvtx[0][iP] - 1].y);
		if (!bS) dys -= 0.5*(pa[nvtx[2][iS] - 1].y + pa[nvtx[0][iS] - 1].y);
	}
    // z - direction
	if (iT > -1) {
		if (!bT) dzt = 0.5*(pa[nvtx[4][iT] - 1].z + pa[nvtx[0][iT] - 1].z);
		if (!bT) dzt -= 0.5*(pa[nvtx[4][iP] - 1].z + pa[nvtx[0][iP] - 1].z);
	}
	if (iB > -1) {
		if (!bB) dzb = 0.5*(pa[nvtx[4][iP] - 1].z + pa[nvtx[0][iP] - 1].z);
		if (!bB) dzb -= 0.5*(pa[nvtx[4][iB] - 1].z + pa[nvtx[0][iB] - 1].z);
	}

	doublereal dxe2 = 0.5*dx, dxw2 = 0.5*dx, dyn2 = 0.5*dy, dys2 = 0.5*dy, dzt2 = 0.5*dz, dzb2 = 0.5*dz;
	doublereal dxe3 = 0.5*dx, dxw3 = 0.5*dx, dyn3 = 0.5*dy, dys3 = 0.5*dy, dzt3 = 0.5*dz, dzb3 = 0.5*dz;
	doublereal dxe4 = 0.5*dx, dxw4 = 0.5*dx, dyn4 = 0.5*dy, dys4 = 0.5*dy, dzt4 = 0.5*dz, dzb4 = 0.5*dz;
	


	// �.�. �������� ��������� ������ ����, �� ����� ��� ������������
	// x - direction
	if (iE2 > -1) {
		if (!bE2) dxe2 = 0.5*(pa[nvtx[1][iE2] - 1].x + pa[nvtx[0][iE2] - 1].x);
		if (!bE2) dxe2 -= 0.5*(pa[nvtx[1][iP] - 1].x + pa[nvtx[0][iP] - 1].x);
	}
	if (iW2 > -1) {
		if (!bW2) dxw2 = 0.5*(pa[nvtx[1][iP] - 1].x + pa[nvtx[0][iP] - 1].x);
		if (!bW2) dxw2 -= 0.5*(pa[nvtx[1][iW2] - 1].x + pa[nvtx[0][iW2] - 1].x);
	}
	// y - direction
	if (iN2 > -1) {
		if (!bN2) dyn2 = 0.5*(pa[nvtx[2][iN2] - 1].y + pa[nvtx[0][iN2] - 1].y);
		if (!bN2) dyn2 -= 0.5*(pa[nvtx[2][iP] - 1].y + pa[nvtx[0][iP] - 1].y);
	}
	if (iS2 > -1) {
		if (!bS2) dys2 = 0.5*(pa[nvtx[2][iP] - 1].y + pa[nvtx[0][iP] - 1].y);
		if (!bS2) dys2 -= 0.5*(pa[nvtx[2][iS2] - 1].y + pa[nvtx[0][iS2] - 1].y);
	}
	// z - direction
	if (iT2 > -1) {
		if (!bT2) dzt2 = 0.5*(pa[nvtx[4][iT2] - 1].z + pa[nvtx[0][iT2] - 1].z);
		if (!bT2) dzt2 -= 0.5*(pa[nvtx[4][iP] - 1].z + pa[nvtx[0][iP] - 1].z);
	}
	if (iB2 > -1) {
		if (!bB2) dzb2 = 0.5*(pa[nvtx[4][iP] - 1].z + pa[nvtx[0][iP] - 1].z);
		if (!bB2) dzb2 -= 0.5*(pa[nvtx[4][iB2] - 1].z + pa[nvtx[0][iB2] - 1].z);
	}

	// �.�. �������� ��������� ������ ����, �� ����� ��� ������������
	// x - direction
	if (iE3 > -1) {
		if (!bE3) dxe3 = 0.5*(pa[nvtx[1][iE3] - 1].x + pa[nvtx[0][iE3] - 1].x);
		if (!bE3) dxe3 -= 0.5*(pa[nvtx[1][iP] - 1].x + pa[nvtx[0][iP] - 1].x);
	}
	if (iW3 > -1) {
		if (!bW3) dxw3 = 0.5*(pa[nvtx[1][iP] - 1].x + pa[nvtx[0][iP] - 1].x);
		if (!bW3) dxw3 -= 0.5*(pa[nvtx[1][iW3] - 1].x + pa[nvtx[0][iW3] - 1].x);
	}
	// y - direction
	if (iN3 > -1) {
		if (!bN3) dyn3 = 0.5*(pa[nvtx[2][iN3] - 1].y + pa[nvtx[0][iN3] - 1].y);
		if (!bN3) dyn3 -= 0.5*(pa[nvtx[2][iP] - 1].y + pa[nvtx[0][iP] - 1].y);
	}
	if (iS3 > -1) {
		if (!bS3) dys3 = 0.5*(pa[nvtx[2][iP] - 1].y + pa[nvtx[0][iP] - 1].y);
		if (!bS3) dys3 -= 0.5*(pa[nvtx[2][iS3] - 1].y + pa[nvtx[0][iS3] - 1].y);
	}
	// z - direction
	if (iT3 > -1) {
		if (!bT3) dzt3 = 0.5*(pa[nvtx[4][iT3] - 1].z + pa[nvtx[0][iT3] - 1].z);
		if (!bT3) dzt3 -= 0.5*(pa[nvtx[4][iP] - 1].z + pa[nvtx[0][iP] - 1].z);
	}
	if (iB3 > -1) {
		if (!bB3) dzb3 = 0.5*(pa[nvtx[4][iP] - 1].z + pa[nvtx[0][iP] - 1].z);
		if (!bB3) dzb3 -= 0.5*(pa[nvtx[4][iB3] - 1].z + pa[nvtx[0][iB3] - 1].z);
	}

	// �.�. �������� ��������� ������ ����, �� ����� ��� ������������
	// x - direction
	if (iE4 > -1) {
		if (!bE4) dxe4 = 0.5*(pa[nvtx[1][iE4] - 1].x + pa[nvtx[0][iE4] - 1].x);
		if (!bE4) dxe4 -= 0.5*(pa[nvtx[1][iP] - 1].x + pa[nvtx[0][iP] - 1].x);
	}
	if (iW4 > -1) {
		if (!bW4) dxw4 = 0.5*(pa[nvtx[1][iP] - 1].x + pa[nvtx[0][iP] - 1].x);
		if (!bW4) dxw4 -= 0.5*(pa[nvtx[1][iW4] - 1].x + pa[nvtx[0][iW4] - 1].x);
	}
	// y - direction
	if (iN4 > -1) {
		if (!bN4) dyn4 = 0.5*(pa[nvtx[2][iN4] - 1].y + pa[nvtx[0][iN4] - 1].y);
		if (!bN4) dyn4 -= 0.5*(pa[nvtx[2][iP] - 1].y + pa[nvtx[0][iP] - 1].y);
	}
	if (iS4 > -1) {
		if (!bS4) dys4 = 0.5*(pa[nvtx[2][iP] - 1].y + pa[nvtx[0][iP] - 1].y);
		if (!bS4) dys4 -= 0.5*(pa[nvtx[2][iS4] - 1].y + pa[nvtx[0][iS4] - 1].y);
	}
	// z - direction
	if (iT4 > -1) {
		if (!bT4) dzt4 = 0.5*(pa[nvtx[4][iT4] - 1].z + pa[nvtx[0][iT4] - 1].z);
		if (!bT4) dzt4 -= 0.5*(pa[nvtx[4][iP] - 1].z + pa[nvtx[0][iP] - 1].z);
	}
	if (iB4 > -1) {
		if (!bB4) dzb4 = 0.5*(pa[nvtx[4][iP] - 1].z + pa[nvtx[0][iP] - 1].z);
		if (!bB4) dzb4 -= 0.5*(pa[nvtx[4][iB4] - 1].z + pa[nvtx[0][iB4] - 1].z);
	}


	doublereal feplus, fwplus, fnplus, fsplus, ftplus, fbplus;
	// x-direction
	feplus=0.5*dx/dxe;
	fwplus=0.5*dx/dxw;
	// y-direction
	fnplus=0.5*dy/dyn;
	fsplus=0.5*dy/dys;
	// z-direction
	ftplus=0.5*dz/dzt;
	fbplus=0.5*dz/dzb;

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

    /*
    if ((iP>=25)&&(iP<=29)) {
        printf("dxe=%e dxw=%e dyn=%e dys=%e dzt=%e dzb=%e\n",dxe, dxw, dyn, dys, dzt, dzb);
        printf("feplus=%e fwplus=%e fnplus=%e fsplus=%e ftplus=%e fbplus=%e\n",feplus, fwplus, fnplus, fsplus, ftplus, fbplus);
	    getchar();
	}
    */

	// ��������� ���������������� ������� �������������
	// ����������: ����� ��� ���������� ���������� ������������ ��������� �� ����������� ��� ���������� ��������.
	doublereal rP, rE=0.0, rN=0.0, rT=0.0, rW=0.0, rS=0.0, rB=0.0;
    rP=prop[RHO][iP]*prop[CP][iP];

	if (iE > -1) {
		if (!bE) rE = prop[RHO][iE] * prop[CP][iE]; else rE = prop_b[RHO][iE - maxelm] * prop_b[CP][iE - maxelm];
	}
	if (iN > -1) {
		if (!bN) rN = prop[RHO][iN] * prop[CP][iN]; else rN = prop_b[RHO][iN - maxelm] * prop_b[CP][iN - maxelm];
	}
	if (iT > -1) {
		if (!bT) rT = prop[RHO][iT] * prop[CP][iT]; else rT = prop_b[RHO][iT - maxelm] * prop_b[CP][iT - maxelm];
	}
	if (iW > -1) {
		if (!bW) rW = prop[RHO][iW] * prop[CP][iW]; else rW = prop_b[RHO][iW - maxelm] * prop_b[CP][iW - maxelm];
	}
	if (iS > -1) {
		if (!bS) rS = prop[RHO][iS] * prop[CP][iS]; else rS = prop_b[RHO][iS - maxelm] * prop_b[CP][iS - maxelm];
	}
	if (iB > -1) {
		if (!bB) rB = prop[RHO][iB] * prop[CP][iB]; else rB = prop_b[RHO][iB - maxelm] * prop_b[CP][iB - maxelm];
	}

	doublereal  rE2=0.0, rN2=0.0, rT2=0.0, rW2=0.0, rS2=0.0, rB2=0.0;
	if (iE2 > -1) {
		if (!bE2) rE2 = prop[RHO][iE2] * prop[CP][iE2]; else rE2 = prop_b[RHO][iE2 - maxelm] * prop_b[CP][iE2 - maxelm];
	}
	if (iN2 > -1) {
		if (!bN2) rN2 = prop[RHO][iN2] * prop[CP][iN2]; else rN2 = prop_b[RHO][iN2 - maxelm] * prop_b[CP][iN2 - maxelm];
	}
	if (iT2 > -1) {
		if (!bT2) rT2 = prop[RHO][iT2] * prop[CP][iT2]; else rT2 = prop_b[RHO][iT2 - maxelm] * prop_b[CP][iT2 - maxelm];
	}
	if (iW2 > -1) {
		if (!bW2) rW2 = prop[RHO][iW2] * prop[CP][iW2]; else rW2 = prop_b[RHO][iW2 - maxelm] * prop_b[CP][iW2 - maxelm];
	}
	if (iS2 > -1) {
		if (!bS2) rS2 = prop[RHO][iS2] * prop[CP][iS2]; else rS2 = prop_b[RHO][iS2 - maxelm] * prop_b[CP][iS2 - maxelm];
	}
	if (iB2 > -1) {
		if (!bB2) rB2 = prop[RHO][iB2] * prop[CP][iB2]; else rB2 = prop_b[RHO][iB2 - maxelm] * prop_b[CP][iB2 - maxelm];
	}

	doublereal  rE3 = 0.0, rN3 = 0.0, rT3 = 0.0, rW3 = 0.0, rS3 = 0.0, rB3 = 0.0;
	if (iE3 > -1) {
		if (!bE3) rE3 = prop[RHO][iE3] * prop[CP][iE3]; else rE3 = prop_b[RHO][iE3 - maxelm] * prop_b[CP][iE3 - maxelm];
	}
	if (iN3 > -1) {
		if (!bN3) rN3 = prop[RHO][iN3] * prop[CP][iN3]; else rN3 = prop_b[RHO][iN3 - maxelm] * prop_b[CP][iN3 - maxelm];
	}
	if (iT3 > -1) {
		if (!bT3) rT3 = prop[RHO][iT3] * prop[CP][iT3]; else rT3 = prop_b[RHO][iT3 - maxelm] * prop_b[CP][iT3 - maxelm];
	}
	if (iW3 > -1) {
		if (!bW3) rW3 = prop[RHO][iW3] * prop[CP][iW3]; else rW3 = prop_b[RHO][iW3 - maxelm] * prop_b[CP][iW3 - maxelm];
	}
	if (iS3 > -1) {
		if (!bS3) rS3 = prop[RHO][iS3] * prop[CP][iS3]; else rS3 = prop_b[RHO][iS3 - maxelm] * prop_b[CP][iS3 - maxelm];
	}
	if (iB3 > -1) {
		if (!bB3) rB3 = prop[RHO][iB3] * prop[CP][iB3]; else rB3 = prop_b[RHO][iB3 - maxelm] * prop_b[CP][iB3 - maxelm];
	}

	doublereal  rE4 = 0.0, rN4 = 0.0, rT4 = 0.0, rW4 = 0.0, rS4 = 0.0, rB4 = 0.0;
	if (iE4 > -1) {
		if (!bE4) rE4 = prop[RHO][iE4] * prop[CP][iE4]; else rE4 = prop_b[RHO][iE4 - maxelm] * prop_b[CP][iE4 - maxelm];
	}
	if (iN4 > -1) {
		if (!bN4) rN4 = prop[RHO][iN4] * prop[CP][iN4]; else rN4 = prop_b[RHO][iN4 - maxelm] * prop_b[CP][iN4 - maxelm];
	}
	if (iT4 > -1) {
		if (!bT4) rT4 = prop[RHO][iT4] * prop[CP][iT4]; else rT4 = prop_b[RHO][iT4 - maxelm] * prop_b[CP][iT4 - maxelm];
	}
	if (iW4 > -1) {
		if (!bW4) rW4 = prop[RHO][iW4] * prop[CP][iW4]; else rW4 = prop_b[RHO][iW4 - maxelm] * prop_b[CP][iW4 - maxelm];
	}
	if (iS4 > -1) {
		if (!bS4) rS4 = prop[RHO][iS4] * prop[CP][iS4]; else rS4 = prop_b[RHO][iS4 - maxelm] * prop_b[CP][iS4 - maxelm];
	}
	if (iB4 > -1) {
		if (!bB4) rB4 = prop[RHO][iB4] * prop[CP][iB4]; else rB4 = prop_b[RHO][iB4 - maxelm] * prop_b[CP][iB4 - maxelm];
	}


	//printf("%e %e %e %e %e\n",rP, rE, rN, rT, rW);
	//getchar();
	

	doublereal rhoe, rhow, rhon, rhos, rhot, rhob;
	// �������� ��������� * ����������� �� ����� ��:
	rhoe=rE*rP/(feplus*rE+(1.0-feplus)*rP); // ������ ����� ������. ���������.
	rhow=rW*rP/(fwplus*rW+(1.0-fwplus)*rP);
	rhon=rN*rP/(fnplus*rN+(1.0-fnplus)*rP);
	rhos=rS*rP/(fsplus*rS+(1.0-fsplus)*rP);
    rhot=rT*rP/(ftplus*rT+(1.0-ftplus)*rP);
	rhob=rB*rP/(fbplus*rB+(1.0-fbplus)*rP);

	doublereal rhoe2, rhow2, rhon2, rhos2, rhot2, rhob2;
	doublereal rhoe3, rhow3, rhon3, rhos3, rhot3, rhob3;
	doublereal rhoe4, rhow4, rhon4, rhos4, rhot4, rhob4;

	rhoe2 = rE2*rP / (feplus2*rE2 + (1.0 - feplus2)*rP); 
	rhow2 = rW2*rP / (fwplus2*rW2 + (1.0 - fwplus2)*rP);
	rhon2 = rN2*rP / (fnplus2*rN2 + (1.0 - fnplus2)*rP);
	rhos2 = rS2*rP / (fsplus2*rS2 + (1.0 - fsplus2)*rP);
	rhot2 = rT2*rP / (ftplus2*rT2 + (1.0 - ftplus2)*rP);
	rhob2 = rB2*rP / (fbplus2*rB2 + (1.0 - fbplus2)*rP);

	rhoe3 = rE3*rP / (feplus3*rE3 + (1.0 - feplus3)*rP); 
	rhow3 = rW3*rP / (fwplus3*rW3 + (1.0 - fwplus3)*rP);
	rhon3 = rN3*rP / (fnplus3*rN3 + (1.0 - fnplus3)*rP);
	rhos3 = rS3*rP / (fsplus3*rS3 + (1.0 - fsplus3)*rP);
	rhot3 = rT3*rP / (ftplus3*rT3 + (1.0 - ftplus3)*rP);
	rhob3 = rB3*rP / (fbplus3*rB3 + (1.0 - fbplus3)*rP);

	rhoe4 = rE4*rP / (feplus4*rE4 + (1.0 - feplus4)*rP);
	rhow4 = rW4*rP / (fwplus4*rW4 + (1.0 - fwplus4)*rP);
	rhon4 = rN4*rP / (fnplus4*rN4 + (1.0 - fnplus4)*rP);
	rhos4 = rS4*rP / (fsplus4*rS4 + (1.0 - fsplus4)*rP);
	rhot4 = rT4*rP / (ftplus4*rT4 + (1.0 - ftplus4)*rP);
	rhob4 = rB4*rP / (fbplus4*rB4 + (1.0 - fbplus4)*rP);

	// �������� ���-��� ��������� � ������������ �������� �� ����� ��
	// ��� ��������� ������������� � �������� 12 ������ 2012 ����.
	// �������� �������� �� ����� ��� ������������ ��� ������� ����� �������������� ������������� ����� (��. ���������� ��������� �������). 

	/*
	// �������� ������������ �������� �� ����� ��.
    doublereal ue=0.0, uw=0.0, vn=0.0, vs=0.0, wt=0.0, wb=0.0;
	if (bconvective) {
		doublereal velP, velS;
	    if (!bE) {
			bool bsolidP=false, bsolidS=false;
		    if (ptr[1][iE]==-1) {
				// ����������� ������� ����:
			    velS=0.0;
			    bsolidS=true;
		    }
	        else velS=f[ptr[1][iE]].potent[VX][ptr[0][iE]];

	        if (ptr[1][iP]==-1) {
			    // ����������� ������� ����:
			    velP=0.0;
			    bsolidP=true;
		    }
	        else velP=f[ptr[1][iP]].potent[VX][ptr[0][iP]];

		    if ((bsolidS || bsolidP) && (!(bsolidS && bsolidP))) {
			    if (bsolidS) {
					// E ������� ������ ����
					ue=f[ptr[1][iP]].potent[VX][f[ptr[1][iP]].sosedi[ESIDE][ptr[0][iP]].iNODE1];
					// ���-��� original interpolation method:
					if (bRhieChowb) {
		                ue+=RCh*ugRhieChow_internal_border(ptr[ENUMERATECONTVOL][iP], ESIDE, f[ptr[1][iP]].alpha[VX], f[ptr[1][iP]].nvtx, f[ptr[1][iP]].sosedi, f[ptr[1][iP]].maxelm, f[ptr[1][iP]].potent[PRESS], f[ptr[1][iP]].pa, f[ptr[1][iP]].diag_coef); // ����� �������� ���-���
		            }
				}
			    else {
					// W ������� ������ ����
					ue=f[ptr[1][iE]].potent[VX][f[ptr[1][iE]].sosedi[WSIDE][ptr[0][iE]].iNODE1];
					// ENUMERATECONTVOL == 0
					// ���-��� original interpolation method:
					if (bRhieChowb) {
			            ue+=RCh*ugRhieChow_internal_border(ptr[ENUMERATECONTVOL][iE], WSIDE, f[ptr[1][iE]].alpha[VX], f[ptr[1][iE]].nvtx, f[ptr[1][iE]].sosedi, f[ptr[1][iE]].maxelm, f[ptr[1][iE]].potent[PRESS], f[ptr[1][iE]].pa, f[ptr[1][iE]].diag_coef); // ����� �������� ���-���
		            }
				}
	    	}
		    else if (!(bsolidS && bsolidP)) {
				// E ����� ������ �������� 
				ue=feplus*velS+(1.0-feplus)*velP;
				// ���-��� original interpolation method:
				if (bRhieChowi) {
		            ue+=RCh*ugRhieChow_internal(ptr[ENUMERATECONTVOL][iP], ESIDE, f[ptr[1][iP]].alpha[VX], f[ptr[1][iP]].nvtx, f[ptr[1][iP]].sosedi, f[ptr[1][iP]].maxelm, f[ptr[1][iP]].potent[PRESS], f[ptr[1][iP]].pa, f[ptr[1][iP]].diag_coef); // ����� �������� ���-���
		        }
			} else ue=0; // � ������ ����
	    } 
	    else  {
		    if (ptr[1][iP]==-1) ue=0.0;
		    else {
				// E ������� ������ ����.
				ue=f[ptr[1][iP]].potent[VX][f[ptr[1][iP]].sosedi[ESIDE][ptr[0][iP]].iNODE1];
				// ���-��� original interpolation method:
				if (bRhieChowb) {
		           ue+=RCh*ugRhieChow_internal_border(ptr[ENUMERATECONTVOL][iP], ESIDE, f[ptr[1][iP]].alpha[VX], f[ptr[1][iP]].nvtx, f[ptr[1][iP]].sosedi, f[ptr[1][iP]].maxelm, f[ptr[1][iP]].potent[PRESS], f[ptr[1][iP]].pa, f[ptr[1][iP]].diag_coef); // ����� �������� ���-���
		        }
			}
	    }
    
	    if (!bW) {
		    bool bsolidP=false, bsolidS=false;
		    if (ptr[1][iW]==-1) {
				// ����������� ������� ����:
			    velS=0.0;
			    bsolidS=true;
		    }
	        else velS=f[ptr[1][iW]].potent[VX][ptr[0][iW]];

	        if (ptr[1][iP]==-1) {
			    // ����������� ������� ����:
			    velP=0.0;
			    bsolidP=true;
		    }
	        else velP=f[ptr[1][iP]].potent[VX][ptr[0][iP]];

		    if ((bsolidS || bsolidP) && (!(bsolidS && bsolidP))) {
			    if (bsolidS) {
					// W ������� ������ ����
					uw=f[ptr[1][iP]].potent[VX][f[ptr[1][iP]].sosedi[WSIDE][ptr[0][iP]].iNODE1];
					// Rhie-Chow original interpolation method 
				    if (bRhieChowb) {
			            uw+=RCh*ugRhieChow_internal_border(ptr[0][iP], WSIDE, f[ptr[1][iP]].alpha[VX], f[ptr[1][iP]].nvtx, f[ptr[1][iP]].sosedi, f[ptr[1][iP]].maxelm, f[ptr[1][iP]].potent[PRESS], f[ptr[1][iP]].pa, f[ptr[1][iP]].diag_coef); // ����� �������� ���-���
		            }
				}
			    else {
					// E ������� ������ ����
					uw=f[ptr[1][iW]].potent[VX][f[ptr[1][iW]].sosedi[ESIDE][ptr[0][iW]].iNODE1];
					// ���-��� original interpolation method:
					if (bRhieChowb) {
		                uw+=RCh*ugRhieChow_internal_border(ptr[0][iW], ESIDE, f[ptr[1][iW]].alpha[VX], f[ptr[1][iW]].nvtx, f[ptr[1][iW]].sosedi, f[ptr[1][iW]].maxelm, f[ptr[1][iW]].potent[PRESS], f[ptr[1][iW]].pa, f[ptr[1][iW]].diag_coef); // ����� �������� ���-���
		            }
				}
		    }
		    else if (!(bsolidS && bsolidP)) {
				// W ����� ������ ��������
				uw=fwplus*velS+(1.0-fwplus)*velP;
				//Rhie-Chow original interpolation method
				if (bRhieChowi) {
			        uw+=RCh*ugRhieChow_internal(ptr[0][iP], WSIDE, f[ptr[1][iP]].alpha[VX], f[ptr[1][iP]].nvtx, f[ptr[1][iP]].sosedi, f[ptr[1][iP]].maxelm, f[ptr[1][iP]].potent[PRESS], f[ptr[1][iP]].pa, f[ptr[1][iP]].diag_coef); // ����� �������� ���-���
		        } 
			} else uw=0.0;
	    } 
	    else  {
			if (ptr[1][iP]==-1) uw=0.0;
		    else {
				// W ������� ������ ����
				uw=f[ptr[1][iP]].potent[VX][f[ptr[1][iP]].sosedi[WSIDE][ptr[0][iP]].iNODE1];
				// Rhie-Chow original interpolation method 
				if (bRhieChowb) {
			        uw+=RCh*ugRhieChow_internal_border(ptr[0][iP], WSIDE, f[ptr[1][iP]].alpha[VX], f[ptr[1][iP]].nvtx, f[ptr[1][iP]].sosedi, f[ptr[1][iP]].maxelm, f[ptr[1][iP]].potent[PRESS], f[ptr[1][iP]].pa, f[ptr[1][iP]].diag_coef); // ����� �������� ���-���
		        }
			}
	    }
	
	    if (!bN) {
			bool bsolidP=false, bsolidS=false;
		    if (ptr[1][iN]==-1) {
				velS=0.0;
			    bsolidS=true;
		    }
	        else velS=f[ptr[1][iN]].potent[VY][ptr[0][iN]];

	        if (ptr[1][iP]==-1) {
				velP=0.0;
			    bsolidP=true;
		    }
	        else velP=f[ptr[1][iP]].potent[VY][ptr[0][iP]];

		    if ((bsolidS || bsolidP) && (!(bsolidS && bsolidP))) {
				if (bsolidS) {
					// N ������� ������ ����
					vn=f[ptr[1][iP]].potent[VY][f[ptr[1][iP]].sosedi[NSIDE][ptr[0][iP]].iNODE1];
					// Rhie-Chow original interpolation method
				    if (bRhieChowb) {
			           vn+=RCh*ugRhieChow_internal_border(ptr[0][iP], N,  f[ptr[1][iP]].alpha[VY], f[ptr[1][iP]].nvtx, f[ptr[1][iP]].sosedi, f[ptr[1][iP]].maxelm, f[ptr[1][iP]].potent[PRESS], f[ptr[1][iP]].pa, f[ptr[1][iP]].diag_coef); // ����� �������� ���-���
		            }
				}
			    else {
					// S ������� ������ ����
					vn=f[ptr[1][iN]].potent[VY][f[ptr[1][iN]].sosedi[SSIDE][ptr[0][iN]].iNODE1];
					// Rhie-Chow original interpolation method
					if (bRhieChowb) {
		               vn+=RCh*ugRhieChow_internal_border(ptr[0][iN], S,  f[ptr[1][iN]].alpha[VY], f[ptr[1][iN]].nvtx, f[ptr[1][iN]].sosedi, f[ptr[1][iN]].maxelm, f[ptr[1][iN]].potent[PRESS], f[ptr[1][iN]].pa, f[ptr[1][iN]].diag_coef); // ����� �������� ���-���
		            }
				}
		    }
		    else if (!(bsolidS && bsolidP)) {
				// N ����� ������ ��������
				vn=fnplus*velS+(1.0-fnplus)*velP;
				// Rhie-Chow original interpolation method
				if (bRhieChowi) {
			        vn+=RCh*ugRhieChow_internal(ptr[0][iP], N, f[ptr[1][iP]].alpha[VY], f[ptr[1][iP]].nvtx, f[ptr[1][iP]].sosedi, f[ptr[1][iP]].maxelm, f[ptr[1][iP]].potent[PRESS], f[ptr[1][iP]].pa, f[ptr[1][iP]].diag_coef); // ����� �������� ���-���
		        }
			} else vn=0.0;
	    } 
	    else  {
			if (ptr[1][iP]==-1) vn=0.0;
		    else {
				// N ������� ������ ����
				vn=f[ptr[1][iP]].potent[VY][f[ptr[1][iP]].sosedi[NSIDE][ptr[0][iP]].iNODE1];
				// Rhie-Chow original interpolation method
				if (bRhieChowb) {
			       vn+=RCh*ugRhieChow_internal_border(ptr[0][iP], N,  f[ptr[1][iP]].alpha[VY], f[ptr[1][iP]].nvtx, f[ptr[1][iP]].sosedi, f[ptr[1][iP]].maxelm, f[ptr[1][iP]].potent[PRESS], f[ptr[1][iP]].pa, f[ptr[1][iP]].diag_coef); // ����� �������� ���-���
		        }
			}
	    }
    
	    if (!bS) {
			bool bsolidP=false, bsolidS=false;
		    if (ptr[1][iS]==-1) {
				// ����������� ������� ����:
			    velS=0.0;
			    bsolidS=true;
		    }
	        else velS=f[ptr[1][iS]].potent[VY][ptr[0][iS]];

	        if (ptr[1][iP]==-1) {
			    // ����������� ������� ����:
			    velP=0.0;
			    bsolidP=true;
		    }
	        else velP=f[ptr[1][iP]].potent[VY][ptr[0][iP]];

		    if ((bsolidS || bsolidP) && (!(bsolidS && bsolidP))) {
			    if (bsolidS) {
					// S ������� ������ ����
					vs=f[ptr[1][iP]].potent[VY][f[ptr[1][iP]].sosedi[SSIDE][ptr[0][iP]].iNODE1];
					// Rhie-Chow original interpolation method
					if (bRhieChowb) {
		                vs+=RCh*ugRhieChow_internal_border(ptr[0][iP], S,  f[ptr[1][iP]].alpha[VY], f[ptr[1][iP]].nvtx, f[ptr[1][iP]].sosedi, f[ptr[1][iP]].maxelm, f[ptr[1][iP]].potent[PRESS], f[ptr[1][iP]].pa, f[ptr[1][iP]].diag_coef); // ����� �������� ���-���
		            }
				}
			    else {
					// N ������� ������ ����
					vs=f[ptr[1][iS]].potent[VY][f[ptr[1][iS]].sosedi[NSIDE][ptr[0][iS]].iNODE1];
					// Rhie-Chow original interpolation method
					if (bRhieChowb) {
			             vs+=RCh*ugRhieChow_internal_border(ptr[0][iS], N,  f[ptr[1][iS]].alpha[VY], f[ptr[1][iS]].nvtx, f[ptr[1][iS]].sosedi, f[ptr[1][iS]].maxelm, f[ptr[1][iS]].potent[PRESS], f[ptr[1][iS]].pa, f[ptr[1][iS]].diag_coef); // ����� �������� ���-���
		            }
				}
		    }
		    else {
				// S ����� ������ ��������
				vs=fsplus*velS+(1.0-fsplus)*velP;
				// Rhie-Chow original interpolation method
				if (bRhieChowi) {
			        vs+=RCh*ugRhieChow_internal(ptr[0][iP], S, f[ptr[1][iP]].alpha[VY], f[ptr[1][iP]].nvtx, f[ptr[1][iP]].sosedi, f[ptr[1][iP]].maxelm, f[ptr[1][iP]].potent[PRESS], f[ptr[1][iP]].pa, f[ptr[1][iP]].diag_coef); // ����� �������� ���-���
		        }		   
			}
	    } 
	    else  {
		    if (ptr[1][iP]==-1) vs=0.0;
		    else {
				// S ������� ������ ����
				vs=f[ptr[1][iP]].potent[VY][f[ptr[1][iP]].sosedi[SSIDE][ptr[0][iP]].iNODE1];
				// Rhie-Chow original interpolation method
				if (bRhieChowb) {
		            vs+=RCh*ugRhieChow_internal_border(ptr[0][iP], S,  f[ptr[1][iP]].alpha[VY], f[ptr[1][iP]].nvtx, f[ptr[1][iP]].sosedi, f[ptr[1][iP]].maxelm, f[ptr[1][iP]].potent[PRESS], f[ptr[1][iP]].pa, f[ptr[1][iP]].diag_coef); // ����� �������� ���-���
		        }
			}
	    }
    
        if (!bT) {
		    bool bsolidP=false, bsolidS=false;
    		if (ptr[1][iT]==-1) {
	    		velS=0.0;
		    	bsolidS=true;
		    }
	        else velS=f[ptr[1][iT]].potent[VZ][ptr[0][iT]];

	        if (ptr[1][iP]==-1) {
		    	velP=0.0;
		    	bsolidP=true;
	    	}
	        else velP=f[ptr[1][iP]].potent[VZ][ptr[0][iP]];

		    if ((bsolidS || bsolidP) && (!(bsolidS && bsolidP))) {
			    if (bsolidS) {
					// T ������� ������ ����
					wt=f[ptr[1][iP]].potent[VZ][f[ptr[1][iP]].sosedi[TSIDE][ptr[0][iP]].iNODE1];
					// Rhie-Chow original interpolation method
				    if (bRhieChowb) {
		                wt+=RCh*ugRhieChow_internal_border(ptr[0][iP], TSIDE, f[ptr[1][iP]].alpha[VZ], f[ptr[1][iP]].nvtx, f[ptr[1][iP]].sosedi, f[ptr[1][iP]].maxelm, f[ptr[1][iP]].potent[PRESS], f[ptr[1][iP]].pa, f[ptr[1][iP]].diag_coef); // ����� �������� ���-���
		            }
				}
		    	else {
					// B ������� ������ ����
					wt=f[ptr[1][iT]].potent[VZ][f[ptr[1][iT]].sosedi[BSIDE][ptr[0][iT]].iNODE1];
					// Rhie-Chow original interpolation method
					if (bRhieChowb) {
		                wt+=RCh*ugRhieChow_internal_border(ptr[0][iT], B, f[ptr[1][iT]].alpha[VZ], f[ptr[1][iT]].nvtx, f[ptr[1][iT]].sosedi, f[ptr[1][iT]].maxelm, f[ptr[1][iT]].potent[PRESS], f[ptr[1][iT]].pa, f[ptr[1][iT]].diag_coef); // ����� �������� ���-���
		            }
				}
	    	}
		    else if (!(bsolidS && bsolidP)) {
				// T ����� ������ ��������
				wt=ftplus*velS+(1.0-ftplus)*velP;
				// Rhie-Chow original interpolation method
				if (bRhieChowi) {
			        wt+=RCh*ugRhieChow_internal(ptr[0][iP], TSIDE, f[ptr[1][iP]].alpha[VZ], f[ptr[1][iP]].nvtx, f[ptr[1][iP]].sosedi, f[ptr[1][iP]].maxelm, f[ptr[1][iP]].potent[PRESS], f[ptr[1][iP]].pa, f[ptr[1][iP]].diag_coef); // ����� �������� ���-���
		        }		   
			} else wt=0.0;
	    } 
	    else  {
		    if (ptr[1][iP]==-1) wt=0.0;
		    else {
				// T ������� ������ ����
				wt=f[ptr[1][iP]].potent[VZ][f[ptr[1][iP]].sosedi[TSIDE][ptr[0][iP]].iNODE1];
				// Rhie-Chow original interpolation method
				if (bRhieChowb) {
		            wt+=RCh*ugRhieChow_internal_border(ptr[0][iP], TSIDE, f[ptr[1][iP]].alpha[VZ], f[ptr[1][iP]].nvtx, f[ptr[1][iP]].sosedi, f[ptr[1][iP]].maxelm, f[ptr[1][iP]].potent[PRESS], f[ptr[1][iP]].pa, f[ptr[1][iP]].diag_coef); // ����� �������� ���-���
		        }
			}
	    }

	    if (!bB) {
		    bool bsolidP=false, bsolidS=false;
		    if (ptr[1][iB]==-1) {
				// ����������� ������� ����:
			    velS=0.0;
			    bsolidS=true;
		    }
	        else velS=f[ptr[1][iB]].potent[VZ][ptr[0][iB]];

	        if (ptr[1][iP]==-1) {
				// ����������� ������� ����:
			    velP=0.0;
			    bsolidP=true;
		    }
	        else velP=f[ptr[1][iP]].potent[VZ][ptr[0][iP]];

		    if ((bsolidS || bsolidP) && (!(bsolidS && bsolidP))) {
			    if (bsolidS) {
					// B ������� ������ ����
					wb=f[ptr[1][iP]].potent[VZ][f[ptr[1][iP]].sosedi[BSIDE][ptr[0][iP]].iNODE1];
					// Rhie-Chow original interpolation method
				    if (bRhieChowb) {
		                wb+=RCh*ugRhieChow_internal_border(ptr[0][iP], B, f[ptr[1][iP]].alpha[VZ], f[ptr[1][iP]].nvtx, f[ptr[1][iP]].sosedi, f[ptr[1][iP]].maxelm, f[ptr[1][iP]].potent[PRESS], f[ptr[1][iP]].pa, f[ptr[1][iP]].diag_coef); // ����� �������� ���-���
		            }
				}
			    else {
					// T ������� ������ ����
					wb=f[ptr[1][iB]].potent[VZ][f[ptr[1][iB]].sosedi[TSIDE][ptr[0][iB]].iNODE1];
					// Rhie-Chow original interpolation method
					if (bRhieChowb) {
		                wb+=RCh*ugRhieChow_internal_border(ptr[0][iB], TSIDE, f[ptr[1][iB]].alpha[VZ], f[ptr[1][iB]].nvtx, f[ptr[1][iB]].sosedi, f[ptr[1][iB]].maxelm, f[ptr[1][iB]].potent[PRESS], f[ptr[1][iB]].pa, f[ptr[1][iB]].diag_coef); // ����� �������� ���-���
		            }
				}
		    }
		    else if (!(bsolidS && bsolidP)) {
				// B ����� ������ ��������
				wb=fbplus*velS+(1.0-fbplus)*velP;
				// Rhie-Chow original interpolation method
				if (bRhieChowi) {
			        wb+=RCh*ugRhieChow_internal(ptr[0][iP], B, f[ptr[1][iP]].alpha[VZ], f[ptr[1][iP]].nvtx, f[ptr[1][iP]].sosedi, f[ptr[1][iP]].maxelm, f[ptr[1][iP]].potent[PRESS], f[ptr[1][iP]].pa, f[ptr[1][iP]].diag_coef); // ����� �������� ���-���
		        }		   
			} else wb=0.0;
	    } 
	    else  {
		    if (ptr[1][iP]==-1) wb=0.0;
		    else {
				// B ������� ������ ����
				wb=f[ptr[1][iP]].potent[VZ][f[ptr[1][iP]].sosedi[BSIDE][ptr[0][iP]].iNODE1];
				// Rhie-Chow original interpolation method
				if (bRhieChowb) {
		            wb+=RCh*ugRhieChow_internal_border(ptr[0][iP], B, f[ptr[1][iP]].alpha[VZ], f[ptr[1][iP]].nvtx, f[ptr[1][iP]].sosedi, f[ptr[1][iP]].maxelm, f[ptr[1][iP]].potent[PRESS], f[ptr[1][iP]].pa, f[ptr[1][iP]].diag_coef); // ����� �������� ���-���
		        }
			}
	    }

	} // end if (bconvective)
    
	//printf("%e %e %e %e %e %e\n",ue, uw, vn, vs, wt, wb);
	//getchar();
	*/

    // ����� ������������ �������� ��� ������� �������� ����� �����
    // ����� ��, ����������� �� ������ ����������������� �������� � ��������,
    // ������������ � ������ � ����� ������������. �� ���� � ��� ��� � ��������� ���������������� �������� �����
    // �� ����� ��������� ��������� �� �������� ����������� ��� ���������� �������� �� �����.
    // ������� ����� ���������� ���������� �����������.

    // ����������� (heat) ���������������� ������� �������������
	// ����������: ����� ��� ������������ ���������� ������ ����������� ��� ���������� ��������.
	
	doublereal heatP, heatE=0.0, heatN=0.0, heatT=0.0, heatW=0.0, heatS=0.0, heatB=0.0;
    heatP=prop[CP][iP];
	if (iE > -1) {
		if (!bE) heatE = prop[CP][iE]; else heatE = prop_b[CP][iE - maxelm];
	}
	if (iN > -1) {
		if (!bN) heatN = prop[CP][iN]; else heatN = prop_b[CP][iN - maxelm];
	}
	if (iT > -1) {
		if (!bT) heatT = prop[CP][iT]; else heatT = prop_b[CP][iT - maxelm];
	}
	if (iW > -1) {
		if (!bW) heatW = prop[CP][iW]; else heatW = prop_b[CP][iW - maxelm];
	}
	if (iS > -1) {
		if (!bS) heatS = prop[CP][iS]; else heatS = prop_b[CP][iS - maxelm];
	}
	if (iB > -1) {
		if (!bB) heatB = prop[CP][iB]; else heatB = prop_b[CP][iB - maxelm];
	}

	doublereal  heatE2=0.0, heatN2=0.0, heatT2=0.0, heatW2=0.0, heatS2=0.0, heatB2=0.0;
	doublereal  heatE3=0.0, heatN3=0.0, heatT3=0.0, heatW3=0.0, heatS3=0.0, heatB3=0.0;
	doublereal  heatE4=0.0, heatN4=0.0, heatT4=0.0, heatW4=0.0, heatS4=0.0, heatB4=0.0;

	if (iE2 > -1) {
		if (!bE2) heatE2 = prop[CP][iE2]; else heatE2 = prop_b[CP][iE2 - maxelm];
	}
	if (iN2 > -1) {
		if (!bN2) heatN2 = prop[CP][iN2]; else heatN2 = prop_b[CP][iN2 - maxelm];
	}
	if (iT2 > -1) {
		if (!bT2) heatT2 = prop[CP][iT2]; else heatT2 = prop_b[CP][iT2 - maxelm];
	}
	if (iW2 > -1) {
		if (!bW2) heatW2 = prop[CP][iW2]; else heatW2 = prop_b[CP][iW2 - maxelm];
	}
	if (iS2 > -1) {
		if (!bS2) heatS2 = prop[CP][iS2]; else heatS2 = prop_b[CP][iS2 - maxelm];
	}
	if (iB2 > -1) {
		if (!bB2) heatB2 = prop[CP][iB2]; else heatB2 = prop_b[CP][iB2 - maxelm];
	}


	if (iE3 > -1) {
		if (!bE3) heatE3 = prop[CP][iE3]; else heatE3 = prop_b[CP][iE3 - maxelm];
	}
	if (iN3 > -1) {
		if (!bN3) heatN3 = prop[CP][iN3]; else heatN3 = prop_b[CP][iN3 - maxelm];
	}
	if (iT3 > -1) {
		if (!bT3) heatT3 = prop[CP][iT3]; else heatT3 = prop_b[CP][iT3 - maxelm];
	}
	if (iW3 > -1) {
		if (!bW3) heatW3 = prop[CP][iW3]; else heatW3 = prop_b[CP][iW3 - maxelm];
	}
	if (iS3 > -1) {
		if (!bS3) heatS3 = prop[CP][iS3]; else heatS3 = prop_b[CP][iS3 - maxelm];
	}
	if (iB3 > -1) {
		if (!bB3) heatB3 = prop[CP][iB3]; else heatB3 = prop_b[CP][iB3 - maxelm];
	}

	if (iE4 > -1) {
		if (!bE4) heatE4 = prop[CP][iE4]; else heatE4 = prop_b[CP][iE4 - maxelm];
	}
	if (iN4 > -1) {
		if (!bN4) heatN4 = prop[CP][iN4]; else heatN4 = prop_b[CP][iN4 - maxelm];
	}
	if (iT4 > -1) {
		if (!bT4) heatT4 = prop[CP][iT4]; else heatT4 = prop_b[CP][iT4 - maxelm];
	}
	if (iW4 > -1) {
		if (!bW4) heatW4 = prop[CP][iW4]; else heatW4 = prop_b[CP][iW4 - maxelm];
	}
	if (iS4 > -1) {
		if (!bS4) heatS4 = prop[CP][iS4]; else heatS4 = prop_b[CP][iS4 - maxelm];
	}
	if (iB4 > -1) {
		if (!bB4) heatB4 = prop[CP][iB4]; else heatB4 = prop_b[CP][iB4 - maxelm];
	}


	//printf("%e %e %e %e %e\n",heatP, heatE, heatN, heatT, heatW);
	//getchar();
	
	doublereal heate=0.0, heatw=0.0, heatn=0.0, heats=0.0, heatt=0.0, heatb=0.0;

	// �������� ����������� ��� ���������� �������� �� ����� ��:
	if (iE > -1) {
		heate = heatE*heatP / (feplus*heatE + (1.0 - feplus)*heatP);
	}
	if (iW > -1) {
		heatw = heatW*heatP / (fwplus*heatW + (1.0 - fwplus)*heatP);
	}
	if (iN > -1) {
		heatn = heatN*heatP / (fnplus*heatN + (1.0 - fnplus)*heatP);
	}
	if (iS > -1) {
		heats = heatS*heatP / (fsplus*heatS + (1.0 - fsplus)*heatP);
	}
	if (iT > -1) {
		heatt = heatT*heatP / (ftplus*heatT + (1.0 - ftplus)*heatP);
	}
	if (iB > -1) {
		heatb = heatB*heatP / (fbplus*heatB + (1.0 - fbplus)*heatP);
	}

	doublereal heate2 = 0.0, heatw2 = 0.0, heatn2 = 0.0, heats2 = 0.0, heatt2 = 0.0, heatb2 = 0.0;
	doublereal heate3 = 0.0, heatw3 = 0.0, heatn3 = 0.0, heats3 = 0.0, heatt3 = 0.0, heatb3 = 0.0;
	doublereal heate4 = 0.0, heatw4 = 0.0, heatn4 = 0.0, heats4 = 0.0, heatt4 = 0.0, heatb4 = 0.0;

	// �������� ����������� ��� ���������� �������� �� ����� ��:
	if (iE2 > -1) {
		heate2 = heatE2*heatP / (feplus2*heatE2 + (1.0 - feplus2)*heatP);
	}
	if (iW2 > -1) {
		heatw2 = heatW2*heatP / (fwplus2*heatW2 + (1.0 - fwplus2)*heatP);
	}
	if (iN2 > -1) {
		heatn2 = heatN2*heatP / (fnplus2*heatN2 + (1.0 - fnplus2)*heatP);
	}
	if (iS2 > -1) {
		heats2 = heatS2*heatP / (fsplus2*heatS2 + (1.0 - fsplus2)*heatP);
	}
	if (iT2 > -1) {
		heatt2 = heatT2*heatP / (ftplus2*heatT2 + (1.0 - ftplus2)*heatP);
	}
	if (iB2 > -1) {
		heatb2 = heatB2*heatP / (fbplus2*heatB2 + (1.0 - fbplus2)*heatP);
	}

	// �������� ����������� ��� ���������� �������� �� ����� ��:
	if (iE3 > -1) {
		heate3 = heatE3*heatP / (feplus3*heatE3 + (1.0 - feplus3)*heatP);
	}
	if (iW3 > -1) {
		heatw3 = heatW3*heatP / (fwplus3*heatW3 + (1.0 - fwplus3)*heatP);
	}
	if (iN3 > -1) {
		heatn3 = heatN3*heatP / (fnplus3*heatN3 + (1.0 - fnplus3)*heatP);
	}
	if (iS3 > -1) {
		heats3 = heatS3*heatP / (fsplus3*heatS3 + (1.0 - fsplus3)*heatP);
	}
	if (iT3 > -1) {
		heatt3 = heatT3*heatP / (ftplus3*heatT3 + (1.0 - ftplus3)*heatP);
	}
	if (iB3 > -1) {
		heatb3 = heatB3*heatP / (fbplus3*heatB3 + (1.0 - fbplus3)*heatP);
	}

	// �������� ����������� ��� ���������� �������� �� ����� ��:
	if (iE4 > -1) {
		heate4 = heatE4*heatP / (feplus4*heatE4 + (1.0 - feplus4)*heatP);
	}
	if (iW4 > -1) {
		heatw4 = heatW4*heatP / (fwplus4*heatW4 + (1.0 - fwplus4)*heatP);
	}
	if (iN4 > -1) {
		heatn4 = heatN4*heatP / (fnplus4*heatN4 + (1.0 - fnplus4)*heatP);
	}
	if (iS4 > -1) {
		heats4 = heatS4*heatP / (fsplus4*heatS4 + (1.0 - fsplus4)*heatP);
	}
	if (iT4 > -1) {
		heatt4 = heatT4*heatP / (ftplus4*heatT4 + (1.0 - ftplus4)*heatP);
	}
	if (iB4 > -1) {
		heatb4 = heatB4*heatP / (fbplus4*heatB4 + (1.0 - fbplus4)*heatP);
	}


	// ������
	doublereal Fe=0.0, Fw=0.0, Fn=0.0, Fs=0.0, Ft=0.0, Fb=0.0;
	// ��� ���� �����.
	doublereal Fe1 = 0.0, Fe2 = 0.0, Fe3 = 0.0, Fe4 = 0.0;
	doublereal Fw1 = 0.0, Fw2 = 0.0, Fw3 = 0.0, Fw4 = 0.0;
	doublereal Fn1 = 0.0, Fn2 = 0.0, Fn3 = 0.0, Fn4 = 0.0;
	doublereal Fs1 = 0.0, Fs2 = 0.0, Fs3 = 0.0, Fs4 = 0.0;
	doublereal Ft1 = 0.0, Ft2 = 0.0, Ft3 = 0.0, Ft4 = 0.0;
	doublereal Fb1 = 0.0, Fb2 = 0.0, Fb3 = 0.0, Fb4 = 0.0;


	/*
	if (bconvective) 
	{
		Fe=rhoe*ue*dy*dz;
	    Fw=rhow*uw*dy*dz;
	    Fn=rhon*vn*dx*dz;
	    Fs=rhos*vs*dx*dz;
        Ft=rhot*wt*dx*dy;
	    Fb=rhob*wb*dx*dy;
	}
	*/

	if ((ptr!=NULL) && (bconvective)) 
	{
		// ���� ptr==NULL �� ����������� ����� � ����������������� ������ � ��� ��������
		// ������ �� ��� ����������������� ����� ��� ��� ������� � �� ��������� � ��������
		// ������ ������������� ������ ������� ����.

		if (ptr[1][iP]!=-1) {
		// ����������� ����� ����������� ������ ����.
		
			if (!b_on_adaptive_local_refinement_mesh) {
				

				Fe = heate*f[ptr[1][iP]].mf[ptr[0][iP]][ESIDE];
				Fn = heatn*f[ptr[1][iP]].mf[ptr[0][iP]][NSIDE];
				Ft = heatt*f[ptr[1][iP]].mf[ptr[0][iP]][TSIDE];
				Fw = heatw*f[ptr[1][iP]].mf[ptr[0][iP]][WSIDE];
				Fs = heats*f[ptr[1][iP]].mf[ptr[0][iP]][SSIDE];
				Fb = heatb*f[ptr[1][iP]].mf[ptr[0][iP]][BSIDE];
#if doubleintprecision == 1
				//printf("fluid heatb=%e heatt=%e Fb=%e Ft=%e iflow=%lld iP=%lld\n", heatb, heatt, Fb,Ft, ptr[1][iP], ptr[0][iP]);
#else
				//printf("fluid heatb=%e heatt=%e Fb=%e Ft=%e iflow=%d iP=%d\n", heatb, heatt, Fb,Ft, ptr[1][iP], ptr[0][iP]);
#endif

			   // getchar();
			}
			else {
				// ���� ����� !!! 9 ������� 2017.
				// ������������ ������� �������� �������������, �� ������ ����� ������ ��� ������������� �������� ���-���.

				if (ptr[0][iP] > -1) {

					// �������� !!! Fg ����� ����� ��������� �� ������� ��������������� ����� ������������ ������.

					if ((iE > -1)&&(!bE)&&(ptr[0][iE] > -1)) {
						if (!bE) Fe1 = heate*(feplus*f[ptr[1][iP]].potent[VX][ptr[0][iE]] + (1.0 - feplus)*f[ptr[1][iP]].potent[VX][ptr[0][iP]]);
					}
					else if (bE) {
						Fe1 = heate*f[ptr[1][iP]].potent[VX][f[ptr[1][iP]].sosedi[ESIDE][ptr[0][iP]].iNODE1];
					}
					if ((iW > -1)&&(!bW)&&(ptr[0][iW] > -1)) {
						if (!bW) Fw1 = heatw*(fwplus*f[ptr[1][iP]].potent[VX][ptr[0][iW]] + (1.0 - fwplus)*f[ptr[1][iP]].potent[VX][ptr[0][iP]]);
					}
					else if (bW) {
						Fw1 = heatw*f[ptr[1][iP]].potent[VX][f[ptr[1][iP]].sosedi[WSIDE][ptr[0][iP]].iNODE1];
					}
					if ((iN > -1)&&(!bN)&&(ptr[0][iN] > -1)) {
						Fn1 = heatn*(fnplus*f[ptr[1][iP]].potent[VY][ptr[0][iN]] + (1.0 - fnplus)*f[ptr[1][iP]].potent[VY][ptr[0][iP]]);
					}
					else if (bN) {
						Fn1 = heatn*f[ptr[1][iP]].potent[VY][f[ptr[1][iP]].sosedi[NSIDE][ptr[0][iP]].iNODE1];
					}
					if ((iS > -1)&&(!bS)&&(ptr[0][iS] > -1)) {
						Fs1 = heats*(fsplus*f[ptr[1][iP]].potent[VY][ptr[0][iS]] + (1.0 - fsplus)*f[ptr[1][iP]].potent[VY][ptr[0][iP]]);
					}
					else if (bS) {
						Fs1 = heats*f[ptr[1][iP]].potent[VY][f[ptr[1][iP]].sosedi[SSIDE][ptr[0][iP]].iNODE1];
					}
					if ((iT > -1)&&(!bT)&&(ptr[0][iT] > -1)) {
						Ft1 = heatt*(ftplus*f[ptr[1][iP]].potent[VZ][ptr[0][iT]] + (1.0 - ftplus)*f[ptr[1][iP]].potent[VZ][ptr[0][iP]]);
					}
					else if (bT) {
						Ft1 = heatt*f[ptr[1][iP]].potent[VZ][f[ptr[1][iP]].sosedi[TSIDE][ptr[0][iP]].iNODE1];
					}
					if ((iB > -1)&&(!bB)&&(ptr[0][iB] > -1)) {
						Fb1 = heatb*(fbplus*f[ptr[1][iP]].potent[VZ][ptr[0][iB]] + (1.0 - fbplus)*f[ptr[1][iP]].potent[VZ][ptr[0][iP]]);
					}
					else if (bB) {
						Fb1 = heatb*f[ptr[1][iP]].potent[VZ][f[ptr[1][iP]].sosedi[BSIDE][ptr[0][iP]].iNODE1];
					}
					//***
					if ((iE2 > -1)&&(!bE2)&&(ptr[0][iE2] > -1)) {
						Fe2 = heate2*(feplus2*f[ptr[1][iP]].potent[VX][ptr[0][iE2]] + (1.0 - feplus2)*f[ptr[1][iP]].potent[VX][ptr[0][iP]]);
					}
					else if (bE2) {
						Fe2 = heate2*f[ptr[1][iP]].potent[VX][f[ptr[1][iP]].sosedi[ESIDE][ptr[0][iP]].iNODE2];
					}
					if ((iW2 > -1)&&(!bW2)&&(ptr[0][iW2] > -1)) {
						Fw2 = heatw2*(fwplus2*f[ptr[1][iP]].potent[VX][ptr[0][iW2]] + (1.0 - fwplus2)*f[ptr[1][iP]].potent[VX][ptr[0][iP]]);
					}
					else if (bW2) {
						Fw2 = heatw2*f[ptr[1][iP]].potent[VX][f[ptr[1][iP]].sosedi[WSIDE][ptr[0][iP]].iNODE2];
					}
					if ((iN2 > -1)&&(!bN2)&&(ptr[0][iN2] > -1)) {
						Fn2 = heatn2*(fnplus2*f[ptr[1][iP]].potent[VY][ptr[0][iN2]] + (1.0 - fnplus2)*f[ptr[1][iP]].potent[VY][ptr[0][iP]]);
					}
					else if (bN2) {
						Fn2 = heatn2*f[ptr[1][iP]].potent[VY][f[ptr[1][iP]].sosedi[NSIDE][ptr[0][iP]].iNODE2];
					}
					if ((iS2 > -1)&&(!bS2)&&(ptr[0][iS2] > -1)) {
						Fs2 = heats2*(fsplus2*f[ptr[1][iP]].potent[VY][ptr[0][iS2]] + (1.0 - fsplus2)*f[ptr[1][iP]].potent[VY][ptr[0][iP]]);
					}
					else if (bS2) {
						Fs2 = heats2*f[ptr[1][iP]].potent[VY][f[ptr[1][iP]].sosedi[SSIDE][ptr[0][iP]].iNODE2];
					}
					if ((iT2 > -1)&&(!bT2)&&(ptr[0][iT2] > -1)) {
						Ft2 = heatt2*(ftplus2*f[ptr[1][iP]].potent[VZ][ptr[0][iT2]] + (1.0 - ftplus2)*f[ptr[1][iP]].potent[VZ][ptr[0][iP]]);
					}
					else if (bT2) {
						Ft2 = heatt2*f[ptr[1][iP]].potent[VZ][f[ptr[1][iP]].sosedi[TSIDE][ptr[0][iP]].iNODE2];
					}
					if ((iB2 > -1)&&(!bB2)&&(ptr[0][iB2] > -1)) {
						Fb2 = heatb2*(fbplus2*f[ptr[1][iP]].potent[VZ][ptr[0][iB2]] + (1.0 - fbplus2)*f[ptr[1][iP]].potent[VZ][ptr[0][iP]]);
					}
					else if (bB2) {
						Fb2 = heatb2*f[ptr[1][iP]].potent[VZ][f[ptr[1][iP]].sosedi[BSIDE][ptr[0][iP]].iNODE2];
					}
					//***
					if ((iE3 > -1)&&(!bE3)&&(ptr[0][iE3] > -1)) {
						Fe3 = heate3*(feplus3*f[ptr[1][iP]].potent[VX][ptr[0][iE3]] + (1.0 - feplus3)*f[ptr[1][iP]].potent[VX][ptr[0][iP]]);
					}
					else if (bE3) {
						Fe3 = heate3*f[ptr[1][iP]].potent[VX][f[ptr[1][iP]].sosedi[ESIDE][ptr[0][iP]].iNODE3];
					}
					if ((iW3 > -1)&&(!bW3)&&(ptr[0][iW3] > -1)) {
						Fw3 = heatw3*(fwplus3*f[ptr[1][iP]].potent[VX][ptr[0][iW3]] + (1.0 - fwplus3)*f[ptr[1][iP]].potent[VX][ptr[0][iP]]);
					}
					else if (bW3) {
						Fw3 = heatw3*f[ptr[1][iP]].potent[VX][f[ptr[1][iP]].sosedi[WSIDE][ptr[0][iP]].iNODE3];
					}
					if ((iN3 > -1)&&(!bN3)&&(ptr[0][iN3] > -1)) {
						Fn3 = heatn3*(fnplus3*f[ptr[1][iP]].potent[VY][ptr[0][iN3]] + (1.0 - fnplus3)*f[ptr[1][iP]].potent[VY][ptr[0][iP]]);
					}
					else if (bN3) {
						Fn3 = heatn3*f[ptr[1][iP]].potent[VY][f[ptr[1][iP]].sosedi[NSIDE][ptr[0][iP]].iNODE3];
					}
					if ((iS3 > -1)&&(!bS3)&&(ptr[0][iS3] > -1)) {
						Fs3 = heats3*(fsplus3*f[ptr[1][iP]].potent[VY][ptr[0][iS3]] + (1.0 - fsplus3)*f[ptr[1][iP]].potent[VY][ptr[0][iP]]);
					}
					else if (bS3) {
						Fs3 = heats3*f[ptr[1][iP]].potent[VY][f[ptr[1][iP]].sosedi[SSIDE][ptr[0][iP]].iNODE3];
					}
					if ((iT3 > -1)&&(!bT3)&&(ptr[0][iT3] > -1)) {
						Ft3 = heatt3*(ftplus3*f[ptr[1][iP]].potent[VZ][ptr[0][iT3]] + (1.0 - ftplus3)*f[ptr[1][iP]].potent[VZ][ptr[0][iP]]);
					}
					else if (bT3) {
						Ft3 = heatt3*f[ptr[1][iP]].potent[VZ][f[ptr[1][iP]].sosedi[TSIDE][ptr[0][iP]].iNODE3];
					}
					if ((iB3 > -1)&&(!bB3)&&(ptr[0][iB3] > -1)) {
						Fb3 = heatb3*(fbplus3*f[ptr[1][iP]].potent[VZ][ptr[0][iB3]] + (1.0 - ftplus3)*f[ptr[1][iP]].potent[VZ][ptr[0][iP]]);
					}
					else if (bB3) {
						Fb3 = heatb3*f[ptr[1][iP]].potent[VZ][f[ptr[1][iP]].sosedi[BSIDE][ptr[0][iP]].iNODE3];
					}
					//***
					if ((iE4 > -1)&&(!bE4)&&(ptr[0][iE4] > -1)) {
						Fe4 = heate4*(feplus4*f[ptr[1][iP]].potent[VX][ptr[0][iE4]] + (1.0 - feplus4)*f[ptr[1][iP]].potent[VX][ptr[0][iP]]);
					}
					else if (bE4) {
						Fe4 = heate4*f[ptr[1][iP]].potent[VX][f[ptr[1][iP]].sosedi[ESIDE][ptr[0][iP]].iNODE4];
					}
					if ((iW4 > -1)&&(!bW4)&&(ptr[0][iW4] > -1)) {
						Fw4 = heatw4*(fwplus4*f[ptr[1][iP]].potent[VX][ptr[0][iW4]] + (1.0 - fwplus4)*f[ptr[1][iP]].potent[VX][ptr[0][iP]]);
					}
					else if (bW4) {
						Fw4 = heatw4*f[ptr[1][iP]].potent[VX][f[ptr[1][iP]].sosedi[WSIDE][ptr[0][iP]].iNODE4];
					}
					if ((iN4 > -1)&&(!bN4)&&(ptr[0][iN4] > -1)) {
						Fn4 = heatn4*(fnplus4*f[ptr[1][iP]].potent[VY][ptr[0][iN4]] + (1.0 - fnplus4)*f[ptr[1][iP]].potent[VY][ptr[0][iP]]);
					}
					else if (bN4) {
						Fn4 = heatn4*f[ptr[1][iP]].potent[VY][f[ptr[1][iP]].sosedi[NSIDE][ptr[0][iP]].iNODE4];
					}
					if ((iS4 > -1)&&(!bS4)&&(ptr[0][iS4] > -1)) {
						Fs4 = heats4*(fsplus4*f[ptr[1][iP]].potent[VY][ptr[0][iS4]] + (1.0 - fsplus4)*f[ptr[1][iP]].potent[VY][ptr[0][iP]]);
					}
					else if (bS4) {
						Fs4 = heats4*f[ptr[1][iP]].potent[VY][f[ptr[1][iP]].sosedi[SSIDE][ptr[0][iP]].iNODE4];
					}
					if ((iT4 > -1)&&(!bT4)&&(ptr[0][iT4] > -1)) {
						Ft4 = heatt4*(ftplus4*f[ptr[1][iP]].potent[VZ][ptr[0][iT4]] + (1.0 - ftplus4)*f[ptr[1][iP]].potent[VZ][ptr[0][iP]]);
					}
					else if (bT4) {
						Ft4 = heatt4*f[ptr[1][iP]].potent[VZ][f[ptr[1][iP]].sosedi[TSIDE][ptr[0][iP]].iNODE4];
					}
					if ((iB4 > -1)&&(!bB4)&&(ptr[0][iB4] > -1)) {
						Fb4 = heatb4*(fbplus4*f[ptr[1][iP]].potent[VZ][ptr[0][iB4]] + (1.0 - fbplus4)*f[ptr[1][iP]].potent[VZ][ptr[0][iP]]);
					}
					else if (bB4) {
						Fb4 = heatb4*f[ptr[1][iP]].potent[VZ][f[ptr[1][iP]].sosedi[BSIDE][ptr[0][iP]].iNODE4];
					}

				}
				else {

				}
			}


	    }
	    else {
			
			if (!b_on_adaptive_local_refinement_mesh) {

				// ����������� ����� iP ����������� ������� ����.
				if (!bE) {
					if (ptr[1][iE] != -1) Fe = heate*f[ptr[1][iE]].mf[ptr[0][iE]][WSIDE]; // ����������� ����� iE ����������� ��������, � �� iP ����������� ������� ����.
					else Fe = 0.0; // iP && iE ����������� ������� ����.
				}
				else Fe = 0.0; // ����� �� iP ����������� ������������ �� ������� ������� ���� ������� ��������� �������.

				if (!bW) {
					if (ptr[1][iW] != -1) Fw = heatw*f[ptr[1][iW]].mf[ptr[0][iW]][ESIDE];
					else Fw = 0.0;
				}
				else Fw = 0.0;

				if (!bN) {
					if (ptr[1][iN] != -1) Fn = heatn*f[ptr[1][iN]].mf[ptr[0][iN]][SSIDE];
					else Fn = 0.0;
				}
				else Fn = 0.0;

				if (!bS) {
					if (ptr[1][iS] != -1) Fs = heats*f[ptr[1][iS]].mf[ptr[0][iS]][NSIDE];
					else Fs = 0.0;
				}
				else Fs = 0.0;

				if (!bT) {
					if (ptr[1][iT] != -1) Ft = heatt*f[ptr[1][iT]].mf[ptr[0][iT]][BSIDE];
					else Ft = 0.0;
				}
				else Ft = 0.0;

				if (!bB) {
					if (ptr[1][iB] != -1) Fb = heatb*f[ptr[1][iB]].mf[ptr[0][iB]][TSIDE];
					else Fb = 0.0;
				}
				else Fb = 0.0;
			}
	    }

		/*
		doublereal ts = Fe*Fe + Fw*Fw + Fn*Fn + Fs*Fs + Ft*Ft + Fb*Fb;
		if (ts != ts) {
			if (Fe*Fe + Fw*Fw + Fn*Fn + Fs*Fs + Ft*Ft + Fb*Fb > 1.0e-40) {
				printf("%d Fe=%e Fw=%e Fn=%e Fs=%e Ft=%e Fb=%e\n", iP, Fe, Fw, Fn, Fs, Ft, Fb);
				getchar();
			}
		}
		*/
	} // bconvective


	
	

	// ������������ ��������:
	doublereal GP=0.0, GE=0.0, GW=0.0, GN=0.0, GS=0.0, GT=0.0, GB=0.0;
	
    GP=prop[LAM][iP];
	if (iE > -1) {
		if (!bE) GE = prop[LAM][iE]; else GE = prop_b[LAM][iE - maxelm];
	}
	if (iN > -1) {
		if (!bN) GN = prop[LAM][iN]; else GN = prop_b[LAM][iN - maxelm];
	}
	if (iT > -1) {
		if (!bT) GT = prop[LAM][iT]; else GT = prop_b[LAM][iT - maxelm];
	}
	if (iW > -1) {
		if (!bW) GW = prop[LAM][iW]; else GW = prop_b[LAM][iW - maxelm];
	}
	if (iS > -1) {
		if (!bS) GS = prop[LAM][iS]; else GS = prop_b[LAM][iS - maxelm];
	}
	if (iB > -1) {
		if (!bB) GB = prop[LAM][iB]; else GB = prop_b[LAM][iB - maxelm];
	}

	doublereal GE2=0.0, GW2=0.0, GN2=0.0, GS2=0.0, GT2=0.0, GB2=0.0;
	doublereal GE3=0.0, GW3=0.0, GN3=0.0, GS3=0.0, GT3=0.0, GB3=0.0;
	doublereal GE4=0.0, GW4=0.0, GN4=0.0, GS4=0.0, GT4=0.0, GB4=0.0;

	if (iE2 > -1) {
		if (!bE2) GE2 = prop[LAM][iE2]; else GE2 = prop_b[LAM][iE2 - maxelm];
	}
	if (iN2 > -1) {
		if (!bN2) GN2 = prop[LAM][iN2]; else GN2 = prop_b[LAM][iN2 - maxelm];
	}
	if (iT2 > -1) {
		if (!bT2) GT2 = prop[LAM][iT2]; else GT2 = prop_b[LAM][iT2 - maxelm];
	}
	if (iW2 > -1) {
		if (!bW2) GW2 = prop[LAM][iW2]; else GW2 = prop_b[LAM][iW2 - maxelm];
	}
	if (iS2 > -1) {
		if (!bS2) GS2 = prop[LAM][iS2]; else GS2 = prop_b[LAM][iS2 - maxelm];
	}
	if (iB2 > -1) {
		if (!bB2) GB2 = prop[LAM][iB2]; else GB2 = prop_b[LAM][iB2 - maxelm];
	}

	if (iE3 > -1) {
		if (!bE3) GE3 = prop[LAM][iE3]; else GE3 = prop_b[LAM][iE3 - maxelm];
	}
	if (iN3 > -1) {
		if (!bN3) GN3 = prop[LAM][iN3]; else GN3 = prop_b[LAM][iN3 - maxelm];
	}
	if (iT3 > -1) {
		if (!bT3) GT3 = prop[LAM][iT3]; else GT3 = prop_b[LAM][iT3 - maxelm];
	}
	if (iW3 > -1) {
		if (!bW3) GW3 = prop[LAM][iW3]; else GW3 = prop_b[LAM][iW3 - maxelm];
	}
	if (iS3 > -1) {
		if (!bS3) GS3 = prop[LAM][iS3]; else GS3 = prop_b[LAM][iS3 - maxelm];
	}
	if (iB3 > -1) {
		if (!bB3) GB3 = prop[LAM][iB3]; else GB3 = prop_b[LAM][iB3 - maxelm];
	}

	if (iE4 > -1) {
		if (!bE4) GE4 = prop[LAM][iE4]; else GE4 = prop_b[LAM][iE4 - maxelm];
	}
	if (iN4 > -1) {
		if (!bN4) GN4 = prop[LAM][iN4]; else GN4 = prop_b[LAM][iN4 - maxelm];
	}
	if (iT4 > -1) {
		if (!bT4) GT4 = prop[LAM][iT4]; else GT4 = prop_b[LAM][iT4 - maxelm];
	}
	if (iW4 > -1) {
		if (!bW4) GW4 = prop[LAM][iW4]; else GW4 = prop_b[LAM][iW4 - maxelm];
	}
	if (iS4 > -1) {
		if (!bS4) GS4 = prop[LAM][iS4]; else GS4 = prop_b[LAM][iS4 - maxelm];
	}
	if (iB4 > -1) {
		if (!bB4) GB4 = prop[LAM][iB4]; else GB4 = prop_b[LAM][iB4 - maxelm];
	}


	/* // ����������������� � �� ������� �������.
	if (!bE) GE=prop[LAM][iE]; else GE=GP;
    if (!bN) GN=prop[LAM][iN]; else GN=GP;
	if (!bT) GT=prop[LAM][iT]; else GT=GP;
	if (!bW) GW=prop[LAM][iW]; else GW=GP;
    if (!bS) GS=prop[LAM][iS]; else GS=GP;
	if (!bB) GB=prop[LAM][iB]; else GB=GP;
	*/
	
	doublereal Ge=0.0, Gw=0.0, Gn=0.0, Gs=0.0, Gt=0.0, Gb=0.0;

    // ���������������� �� ������ ����������� ��:
	if (iE > -1) {
		Ge = GE*GP / (feplus*GE + (1 - feplus)*GP);
	}
	if (iW > -1) {
		Gw = GW*GP / (fwplus*GW + (1 - fwplus)*GP);
	}
	if (iN > -1) {
		Gn = GN*GP / (fnplus*GN + (1 - fnplus)*GP);
	}
	if (iS > -1) {
		Gs = GS*GP / (fsplus*GS + (1 - fsplus)*GP);
	}
	if (iT > -1) {
		Gt = GT*GP / (ftplus*GT + (1 - ftplus)*GP);
	}
	if (iB > -1) {
		Gb = GB*GP / (fbplus*GB + (1 - fbplus)*GP);
	}

	doublereal Ge2 = 0.0, Gw2 = 0.0, Gn2 = 0.0, Gs2 = 0.0, Gt2 = 0.0, Gb2 = 0.0;

	// ���������������� �� ������ ����������� ��:
	if (iE2 > -1) {
		Ge2 = GE2*GP / (feplus2*GE2 + (1 - feplus2)*GP);
	}
	if (iW2 > -1) {
		Gw2 = GW2*GP / (fwplus2*GW2 + (1 - fwplus2)*GP);
	}
	if (iN2 > -1) {
		Gn2 = GN2*GP / (fnplus2*GN2 + (1 - fnplus2)*GP);
	}
	if (iS2 > -1) {
		Gs2 = GS2*GP / (fsplus2*GS2 + (1 - fsplus2)*GP);
	}
	if (iT2 > -1) {
		Gt2 = GT2*GP / (ftplus2*GT2 + (1 - ftplus2)*GP);
	}
	if (iB2 > -1) {
		Gb2 = GB2*GP / (fbplus2*GB2 + (1 - fbplus2)*GP);
	}

	doublereal Ge3 = 0.0, Gw3 = 0.0, Gn3 = 0.0, Gs3 = 0.0, Gt3 = 0.0, Gb3 = 0.0;

	// ���������������� �� ������ ����������� ��:
	if (iE3 > -1) {
		Ge3 = GE3*GP / (feplus3*GE3 + (1 - feplus3)*GP);
	}
	if (iW3 > -1) {
		Gw3 = GW3*GP / (fwplus3*GW3 + (1 - fwplus3)*GP);
	}
	if (iN3 > -1) {
		Gn3 = GN3*GP / (fnplus3*GN3 + (1 - fnplus3)*GP);
	}
	if (iS3 > -1) {
		Gs3 = GS3*GP / (fsplus3*GS3 + (1 - fsplus3)*GP);
	}
	if (iT3 > -1) {
		Gt3 = GT3*GP / (ftplus3*GT3 + (1 - ftplus3)*GP);
	}
	if (iB3 > -1) {
		Gb3 = GB3*GP / (fbplus3*GB3 + (1 - fbplus3)*GP);
	}

	doublereal Ge4 = 0.0, Gw4 = 0.0, Gn4 = 0.0, Gs4 = 0.0, Gt4 = 0.0, Gb4 = 0.0;

	// ���������������� �� ������ ����������� ��:
	if (iE4 > -1) {
		Ge4 = GE4*GP / (feplus4*GE4 + (1 - feplus4)*GP);
	}
	if (iW4 > -1) {
		Gw4 = GW4*GP / (fwplus4*GW4 + (1 - fwplus4)*GP);
	}
	if (iN4 > -1) {
		Gn4 = GN4*GP / (fnplus4*GN4 + (1 - fnplus4)*GP);
	}
	if (iS4 > -1) {
		Gs4 = GS4*GP / (fsplus4*GS4 + (1 - fsplus4)*GP);
	}
	if (iT4 > -1) {
		Gt4 = GT4*GP / (ftplus4*GT4 + (1 - ftplus4)*GP);
	}
	if (iB4 > -1) {
		Gb4 = GB4*GP / (fbplus4*GB4 + (1 - fbplus4)*GP);
	}
	
	// ���������������� �� ����� �������� ����������� 
	// ���� ��� ����������� ������ ���������� ����� 
	// ����� ���������� ����������� �������������.

	// ������������� �� ���� ������������� :
	if (iE > -1) {
		if (iE >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_X][iP] - prop_b[MULT_LAM_X][iE - maxelm]) < 1.0e-23) {
				Ge *= prop[MULT_LAM_X][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_X][iP] - prop[MULT_LAM_X][iE]) < 1.0e-23) {
				Ge *= prop[MULT_LAM_X][iP];
			}
		}		
	}
	if (iW > -1) {
		if (iW >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_X][iP] - prop_b[MULT_LAM_X][iW - maxelm]) < 1.0e-23) {
				Gw *= prop[MULT_LAM_X][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_X][iP] - prop[MULT_LAM_X][iW]) < 1.0e-23) {
				Gw *= prop[MULT_LAM_X][iP];
			}
		}
	}
	if (iN > -1) {
		if (iN >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_Y][iP] - prop_b[MULT_LAM_Y][iN - maxelm]) < 1.0e-23) {
				Gn *= prop[MULT_LAM_Y][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_Y][iP] - prop[MULT_LAM_Y][iN]) < 1.0e-23) {
				Gn *= prop[MULT_LAM_Y][iP];
			}
		}
	}
	if (iS > -1) {
		if (iS >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_Y][iP] - prop_b[MULT_LAM_Y][iS - maxelm]) < 1.0e-23) {
				Gs *= prop[MULT_LAM_Y][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_Y][iP] - prop[MULT_LAM_Y][iS]) < 1.0e-23) {
				Gs *= prop[MULT_LAM_Y][iP];
			}
		}
	}
	if (iT > -1) {
		if (iT >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_Z][iP] - prop_b[MULT_LAM_Z][iT - maxelm]) < 1.0e-23) {
				Gt *= prop[MULT_LAM_Z][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_Z][iP] - prop[MULT_LAM_Z][iT]) < 1.0e-23) {
				Gt *= prop[MULT_LAM_Z][iP];
			}
		}
	}
	if (iB > -1) {
		if (iB >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_Z][iP] - prop_b[MULT_LAM_Z][iB - maxelm]) < 1.0e-23) {
				Gb *= prop[MULT_LAM_Z][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_Z][iP] - prop[MULT_LAM_Z][iB]) < 1.0e-23) {
				Gb *= prop[MULT_LAM_Z][iP];
			}
		}
	}
	

	// ������������� �� ���� ������������� :
	if (iE2 > -1) {
		if (iE2 >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_X][iP] - prop_b[MULT_LAM_X][iE2 - maxelm]) < 1.0e-23) {
				Ge2 *= prop[MULT_LAM_X][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_X][iP] - prop[MULT_LAM_X][iE2]) < 1.0e-23) {
				Ge2 *= prop[MULT_LAM_X][iP];
			}
		}
	}
	if (iW2 > -1) {
		if (iW2 >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_X][iP] - prop_b[MULT_LAM_X][iW2 - maxelm]) < 1.0e-23) {
				Gw2 *= prop[MULT_LAM_X][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_X][iP] - prop[MULT_LAM_X][iW2]) < 1.0e-23) {
				Gw2 *= prop[MULT_LAM_X][iP];
			}
		}
	}
	if (iN2 > -1) {
		if (iN2 >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_Y][iP] - prop_b[MULT_LAM_Y][iN2 - maxelm]) < 1.0e-23) {
				Gn2 *= prop[MULT_LAM_Y][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_Y][iP] - prop[MULT_LAM_Y][iN2]) < 1.0e-23) {
				Gn2 *= prop[MULT_LAM_Y][iP];
			}
		}
	}
	if (iS2 > -1) {
		if (iS2 >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_Y][iP] - prop_b[MULT_LAM_Y][iS2 - maxelm]) < 1.0e-23) {
				Gs2 *= prop[MULT_LAM_Y][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_Y][iP] - prop[MULT_LAM_Y][iS2]) < 1.0e-23) {
				Gs2 *= prop[MULT_LAM_Y][iP];
			}
		}
	}
	if (iT2 > -1) {
		if (iT2 >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_Z][iP] - prop_b[MULT_LAM_Z][iT2 - maxelm]) < 1.0e-23) {
				Gt2 *= prop[MULT_LAM_Z][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_Z][iP] - prop[MULT_LAM_Z][iT2]) < 1.0e-23) {
				Gt2 *= prop[MULT_LAM_Z][iP];
			}
		}
	}
	if (iB2 > -1) {
		if (iB2 >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_Z][iP] - prop_b[MULT_LAM_Z][iB2 - maxelm]) < 1.0e-23) {
				Gb2 *= prop[MULT_LAM_Z][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_Z][iP] - prop[MULT_LAM_Z][iB2]) < 1.0e-23) {
				Gb2 *= prop[MULT_LAM_Z][iP];
			}
		}
	}


	// ������������� �� ���� ������������� :
	if (iE3 > -1) {
		if (iE3 >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_X][iP] - prop_b[MULT_LAM_X][iE3 - maxelm]) < 1.0e-23) {
				Ge3 *= prop[MULT_LAM_X][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_X][iP] - prop[MULT_LAM_X][iE3]) < 1.0e-23) {
				Ge3 *= prop[MULT_LAM_X][iP];
			}
		}
	}
	if (iW3 > -1) {
		if (iW3 >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_X][iP] - prop_b[MULT_LAM_X][iW3 - maxelm]) < 1.0e-23) {
				Gw3 *= prop[MULT_LAM_X][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_X][iP] - prop[MULT_LAM_X][iW3]) < 1.0e-23) {
				Gw3 *= prop[MULT_LAM_X][iP];
			}
		}
	}
	if (iN3 > -1) {
		if (iN3 >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_Y][iP] - prop_b[MULT_LAM_Y][iN3 - maxelm]) < 1.0e-23) {
				Gn3 *= prop[MULT_LAM_Y][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_Y][iP] - prop[MULT_LAM_Y][iN3]) < 1.0e-23) {
				Gn3 *= prop[MULT_LAM_Y][iP];
			}
		}
	}
	if (iS3 > -1) {
		if (iS3 >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_Y][iP] - prop_b[MULT_LAM_Y][iS3 - maxelm]) < 1.0e-23) {
				Gs3 *= prop[MULT_LAM_Y][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_Y][iP] - prop[MULT_LAM_Y][iS3]) < 1.0e-23) {
				Gs3 *= prop[MULT_LAM_Y][iP];
			}
		}
	}
	if (iT3 > -1) {
		if (iT3 >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_Z][iP] - prop_b[MULT_LAM_Z][iT3 - maxelm]) < 1.0e-23) {
				Gt3 *= prop[MULT_LAM_Z][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_Z][iP] - prop[MULT_LAM_Z][iT3]) < 1.0e-23) {
				Gt3 *= prop[MULT_LAM_Z][iP];
			}
		}
	}
	if (iB3 > -1) {
		if (iB3 >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_Z][iP] - prop_b[MULT_LAM_Z][iB3 - maxelm]) < 1.0e-23) {
				Gb3 *= prop[MULT_LAM_Z][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_Z][iP] - prop[MULT_LAM_Z][iB3]) < 1.0e-23) {
				Gb3 *= prop[MULT_LAM_Z][iP];
			}
		}
	}

	// ������������� �� ���� ������������� :
	if (iE4 > -1) {
		if (iE4 >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_X][iP] - prop_b[MULT_LAM_X][iE4 - maxelm]) < 1.0e-23) {
				Ge4 *= prop[MULT_LAM_X][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_X][iP] - prop[MULT_LAM_X][iE4]) < 1.0e-23) {
				Ge4 *= prop[MULT_LAM_X][iP];
			}
		}
	}
	if (iW4 > -1) {
		if (iW4 >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_X][iP] - prop_b[MULT_LAM_X][iW4 - maxelm]) < 1.0e-23) {
				Gw4 *= prop[MULT_LAM_X][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_X][iP] - prop[MULT_LAM_X][iW4]) < 1.0e-23) {
				Gw4 *= prop[MULT_LAM_X][iP];
			}
		}
	}
	if (iN4 > -1) {
		if (iN4 >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_Y][iP] - prop_b[MULT_LAM_Y][iN4 - maxelm]) < 1.0e-23) {
				Gn4 *= prop[MULT_LAM_Y][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_Y][iP] - prop[MULT_LAM_Y][iN4]) < 1.0e-23) {
				Gn4 *= prop[MULT_LAM_Y][iP];
			}
		}
	}
	if (iS4 > -1) {
		if (iS4 >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_Y][iP] - prop_b[MULT_LAM_Y][iS4 - maxelm]) < 1.0e-23) {
				Gs4 *= prop[MULT_LAM_Y][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_Y][iP] - prop[MULT_LAM_Y][iS4]) < 1.0e-23) {
				Gs4 *= prop[MULT_LAM_Y][iP];
			}
		}
	}
	if (iT4 > -1) {
		if (iT4 >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_Z][iP] - prop_b[MULT_LAM_Z][iT4 - maxelm]) < 1.0e-23) {
				Gt4 *= prop[MULT_LAM_Z][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_Z][iP] - prop[MULT_LAM_Z][iT4]) < 1.0e-23) {
				Gt4 *= prop[MULT_LAM_Z][iP];
			}
		}
	}
	if (iB4 > -1) {
		if (iB4 >= maxelm) {
			// ��������� ����.
			if (fabs(prop[MULT_LAM_Z][iP] - prop_b[MULT_LAM_Z][iB4 - maxelm]) < 1.0e-23) {
				Gb4 *= prop[MULT_LAM_Z][iP];
			}
		}
		else {
			// ���������� ����.
			if (fabs(prop[MULT_LAM_Z][iP] - prop[MULT_LAM_Z][iB4]) < 1.0e-23) {
				Gb4 *= prop[MULT_LAM_Z][iP];
			}
		}
	}

	

	// ��.
	// �������� ������������ ����������������:
	//printf("Ge=%e, Gw=%e, Gn=%e, Gs=%e, Gt=%e, Gb=%e\n",Ge,Gw,Gn,Gs,Gt,Gb);


	// ��������� ������������ ����������������.
	if (bconvective) {
		if (ptr != NULL) {

			if (!b_on_adaptive_local_refinement_mesh) {

				// ���� �� ����� ����� ������������ ������, �� � ��� ������ ��� �� ���������,
				// ��� ��� ������ � ������������ �������� � ����������������.

				if (!bE) {
					bool bsolidP = false, bsolidS = false;
					doublereal turblamS = 0.0, turblamP = 0.0;

					if (ptr[1][iE] == -1) {
						// ����������� ������� ����
						turblamS = 0.0;
						bsolidS = true;
					}
					else {
						if (f[ptr[1][iE]].iflowregime == ZEROEQMOD) {
							turblamS = prop[CP][iE] * f[ptr[1][iE]].potent[MUT][ptr[0][iE]] / 0.85;
						}
					}

					if (ptr[1][iP] == -1) {
						// ����������� ������� ����
						turblamP = 0.0;
						bsolidP = true;
					}
					else {
						if (f[ptr[1][iP]].iflowregime == ZEROEQMOD) {
							turblamP = prop[CP][iP] * f[ptr[1][iP]].potent[MUT][ptr[0][iP]] / 0.85;
						}
					}

					if ((bsolidS || bsolidP) && (!(bsolidS && bsolidP))) {
						doublereal cpe = prop[CP][iP] * prop[CP][iE] / ((1.0 - feplus)*prop[CP][iP] + feplus*prop[CP][iE]);
						if ((bsolidS) && (f[ptr[1][iP]].iflowregime == ZEROEQMOD)) {
							Ge += cpe*f[ptr[1][iP]].potent[MUT][f[ptr[1][iP]].sosedi[ESIDE][ptr[0][iP]].iNODE1] / 0.85;
						}
						else if ((bsolidP) && (f[ptr[1][iE]].iflowregime == ZEROEQMOD)) {
							Ge += cpe*f[ptr[1][iE]].potent[MUT][f[ptr[1][iE]].sosedi[WSIDE][ptr[0][iE]].iNODE1] / 0.85;
						}

					}
					else if (!(bsolidS && bsolidP)) {
						Ge += feplus*turblamS + (1.0 - feplus)*turblamP;
					}

				}
				else {
					if (ptr[1][iP] != -1) {
						if (f[ptr[1][iP]].iflowregime == ZEROEQMOD) {
							doublereal cpe = prop_b[CP][iE - maxelm];
							Ge += cpe*f[ptr[1][iP]].potent[MUT][f[ptr[1][iP]].sosedi[ESIDE][ptr[0][iP]].iNODE1] / 0.85;
						}
					}
				}

				if (!bN) {
					bool bsolidP = false, bsolidS = false;
					doublereal turblamS = 0.0, turblamP = 0.0;

					if (ptr[1][iN] == -1) {
						// ����������� ������� ����
						turblamS = 0.0;
						bsolidS = true;
					}
					else {
						if (f[ptr[1][iN]].iflowregime == ZEROEQMOD) {
							turblamS = prop[CP][iN] * f[ptr[1][iN]].potent[MUT][ptr[0][iN]] / 0.85;
						}
					}

					if (ptr[1][iP] == -1) {
						// ����������� ������� ����
						turblamP = 0.0;
						bsolidP = true;
					}
					else {
						if (f[ptr[1][iP]].iflowregime == ZEROEQMOD) {
							turblamP = prop[CP][iP] * f[ptr[1][iP]].potent[MUT][ptr[0][iP]] / 0.85;
						}
					}

					if ((bsolidS || bsolidP) && (!(bsolidS && bsolidP))) {
						doublereal cpn = prop[CP][iP] * prop[CP][iN] / ((1.0 - fnplus)*prop[CP][iP] + fnplus*prop[CP][iN]);
						if ((bsolidS) && (f[ptr[1][iP]].iflowregime == ZEROEQMOD)) {
							Gn += cpn*f[ptr[1][iP]].potent[MUT][f[ptr[1][iP]].sosedi[NSIDE][ptr[0][iP]].iNODE1] / 0.85;
						}
						else if ((bsolidP) && (f[ptr[1][iN]].iflowregime == ZEROEQMOD)) {
							Gn += cpn*f[ptr[1][iN]].potent[MUT][f[ptr[1][iN]].sosedi[SSIDE][ptr[0][iN]].iNODE1] / 0.85;
						}

					}
					else if (!(bsolidS && bsolidP)) {
						Gn += fnplus*turblamS + (1.0 - fnplus)*turblamP;
					}

				}
				else {
					if (ptr[1][iP] != -1) {
						if (f[ptr[1][iP]].iflowregime == ZEROEQMOD) {
							doublereal cpn = prop_b[CP][iN - maxelm];
							Gn += cpn*f[ptr[1][iP]].potent[MUT][f[ptr[1][iP]].sosedi[NSIDE][ptr[0][iP]].iNODE1] / 0.85;
						}
					}
				}


				if (!bT) {
					bool bsolidP = false, bsolidS = false;
					doublereal turblamS = 0.0, turblamP = 0.0;

					if (ptr[1][iT] == -1) {
						// ����������� ������� ����
						turblamS = 0.0;
						bsolidS = true;
					}
					else {
						if (f[ptr[1][iT]].iflowregime == ZEROEQMOD) {
							turblamS = prop[CP][iT] * f[ptr[1][iT]].potent[MUT][ptr[0][iT]] / 0.85;
						}
					}

					if (ptr[1][iP] == -1) {
						// ����������� ������� ����
						turblamP = 0.0;
						bsolidP = true;
					}
					else {
						if (f[ptr[1][iP]].iflowregime == ZEROEQMOD) {
							turblamP = prop[CP][iP] * f[ptr[1][iP]].potent[MUT][ptr[0][iP]] / 0.85;
						}
					}

					if ((bsolidS || bsolidP) && (!(bsolidS && bsolidP))) {
						doublereal cpt = prop[CP][iP] * prop[CP][iT] / ((1.0 - ftplus)*prop[CP][iP] + ftplus*prop[CP][iT]);
						if ((bsolidS) && (f[ptr[1][iP]].iflowregime == ZEROEQMOD)) {
							Gt += cpt*f[ptr[1][iP]].potent[MUT][f[ptr[1][iP]].sosedi[TSIDE][ptr[0][iP]].iNODE1] / 0.85;
						}
						else if ((bsolidP) && (f[ptr[1][iT]].iflowregime == ZEROEQMOD)) {
							Gt += cpt*f[ptr[1][iT]].potent[MUT][f[ptr[1][iT]].sosedi[BSIDE][ptr[0][iT]].iNODE1] / 0.85;
						}

					}
					else if (!(bsolidS && bsolidP)) {
						Gt += ftplus*turblamS + (1.0 - ftplus)*turblamP;
					}

				}
				else {
					if (ptr[1][iP] != -1) {
						if (f[ptr[1][iP]].iflowregime == ZEROEQMOD) {
							doublereal cpt = prop_b[CP][iT - maxelm];
							Gt += cpt*f[ptr[1][iP]].potent[MUT][f[ptr[1][iP]].sosedi[TSIDE][ptr[0][iP]].iNODE1] / 0.85;
						}
					}
				}

				if (!bW) {
					bool bsolidP = false, bsolidS = false;
					doublereal turblamS = 0.0, turblamP = 0.0;

					if (ptr[1][iW] == -1) {
						// ����������� ������� ����
						turblamS = 0.0;
						bsolidS = true;
					}
					else {
						if (f[ptr[1][iW]].iflowregime == ZEROEQMOD) {
							turblamS = prop[CP][iW] * f[ptr[1][iW]].potent[MUT][ptr[0][iW]] / 0.85;
						}
					}

					if (ptr[1][iP] == -1) {
						// ����������� ������� ����
						turblamP = 0.0;
						bsolidP = true;
					}
					else {
						if (f[ptr[1][iP]].iflowregime == ZEROEQMOD) {
							turblamP = prop[CP][iP] * f[ptr[1][iP]].potent[MUT][ptr[0][iP]] / 0.85;
						}
					}

					if ((bsolidS || bsolidP) && (!(bsolidS && bsolidP))) {
						doublereal cpw = prop[CP][iP] * prop[CP][iW] / ((1.0 - fwplus)*prop[CP][iP] + fwplus*prop[CP][iW]);
						if ((bsolidS) && (f[ptr[1][iP]].iflowregime == ZEROEQMOD)) {
							Gw += cpw*f[ptr[1][iP]].potent[MUT][f[ptr[1][iP]].sosedi[WSIDE][ptr[0][iP]].iNODE1] / 0.85;
						}
						else if ((bsolidP) && (f[ptr[1][iW]].iflowregime == ZEROEQMOD)) {
							Gw += cpw*f[ptr[1][iW]].potent[MUT][f[ptr[1][iW]].sosedi[ESIDE][ptr[0][iW]].iNODE1] / 0.85;
						}

					}
					else if (!(bsolidS && bsolidP)) {
						Gw += fwplus*turblamS + (1.0 - fwplus)*turblamP;
					}

				}
				else {
					if (ptr[1][iP] != -1) {
						if (f[ptr[1][iP]].iflowregime == ZEROEQMOD) {
							doublereal cpw = prop_b[CP][iW - maxelm];
							Gw += cpw*f[ptr[1][iP]].potent[MUT][f[ptr[1][iP]].sosedi[WSIDE][ptr[0][iP]].iNODE1] / 0.85;
						}
					}
				}

				if (!bS) {
					bool bsolidP = false, bsolidS = false;
					doublereal turblamS = 0.0, turblamP = 0.0;

					if (ptr[1][iS] == -1) {
						// ����������� ������� ����
						turblamS = 0.0;
						bsolidS = true;
					}
					else {
						if (f[ptr[1][iS]].iflowregime == ZEROEQMOD) {
							turblamS = prop[CP][iS] * f[ptr[1][iS]].potent[MUT][ptr[0][iS]] / 0.85;
						}
					}

					if (ptr[1][iP] == -1) {
						// ����������� ������� ����
						turblamP = 0.0;
						bsolidP = true;
					}
					else {
						if (f[ptr[1][iP]].iflowregime == ZEROEQMOD) {
							turblamP = prop[CP][iP] * f[ptr[1][iP]].potent[MUT][ptr[0][iP]] / 0.85;
						}
					}

					if ((bsolidS || bsolidP) && (!(bsolidS && bsolidP))) {
						doublereal cps = prop[CP][iP] * prop[CP][iS] / ((1.0 - fsplus)*prop[CP][iP] + fsplus*prop[CP][iS]);
						if ((bsolidS) && (f[ptr[1][iP]].iflowregime == ZEROEQMOD)) {
							Gs += cps*f[ptr[1][iP]].potent[MUT][f[ptr[1][iP]].sosedi[SSIDE][ptr[0][iP]].iNODE1] / 0.85;
						}
						else if ((bsolidP) && (f[ptr[1][iS]].iflowregime == ZEROEQMOD)) {
							Gs += cps*f[ptr[1][iS]].potent[MUT][f[ptr[1][iS]].sosedi[NSIDE][ptr[0][iS]].iNODE1] / 0.85;
						}

					}
					else if (!(bsolidS && bsolidP)) {
						Gs += fsplus*turblamS + (1.0 - fsplus)*turblamP;
					}

				}
				else {
					if (ptr[1][iP] != -1) {
						if (f[ptr[1][iP]].iflowregime == ZEROEQMOD) {
							doublereal cps = prop_b[CP][iS - maxelm];
							Gs += cps*f[ptr[1][iP]].potent[MUT][f[ptr[1][iP]].sosedi[SSIDE][ptr[0][iP]].iNODE1] / 0.85;
						}
					}
				}

				if (!bB) {
					bool bsolidP = false, bsolidS = false;
					doublereal turblamS = 0.0, turblamP = 0.0;

					if (ptr[1][iB] == -1) {
						// ����������� ������� ����
						turblamS = 0.0;
						bsolidS = true;
					}
					else {
						if (f[ptr[1][iB]].iflowregime == ZEROEQMOD) {
							turblamS = prop[CP][iB] * f[ptr[1][iB]].potent[MUT][ptr[0][iB]] / 0.85;
						}
					}

					if (ptr[1][iP] == -1) {
						// ����������� ������� ����
						turblamP = 0.0;
						bsolidP = true;
					}
					else {
						if (f[ptr[1][iP]].iflowregime == ZEROEQMOD) {
							turblamP = prop[CP][iP] * f[ptr[1][iP]].potent[MUT][ptr[0][iP]] / 0.85;
						}
					}

					if ((bsolidS || bsolidP) && (!(bsolidS && bsolidP))) {
						doublereal cpb = prop[CP][iP] * prop[CP][iB] / ((1.0 - fbplus)*prop[CP][iP] + fbplus*prop[CP][iB]);
						if ((bsolidS) && (f[ptr[1][iP]].iflowregime == ZEROEQMOD)) {
							Gb += cpb*f[ptr[1][iP]].potent[MUT][f[ptr[1][iP]].sosedi[BSIDE][ptr[0][iP]].iNODE1] / 0.85;
						}
						else if ((bsolidP) && (f[ptr[1][iB]].iflowregime == ZEROEQMOD)) {
							Gb += cpb*f[ptr[1][iB]].potent[MUT][f[ptr[1][iB]].sosedi[TSIDE][ptr[0][iB]].iNODE1] / 0.85;
						}

					}
					else if (!(bsolidS && bsolidP)) {
						Gb += fbplus*turblamS + (1.0 - fbplus)*turblamP;
					}

				}
				else {
					if (ptr[1][iP] != -1) {
						if (f[ptr[1][iP]].iflowregime == ZEROEQMOD) {
							doublereal cpb = prop_b[CP][iB - maxelm];
							Gb += cpb*f[ptr[1][iP]].potent[MUT][f[ptr[1][iP]].sosedi[BSIDE][ptr[0][iP]].iNODE1] / 0.85;
						}
					}
				}
			}
		}
	}  // ������������ ����������������.
	
	
	if (!b_on_adaptive_local_refinement_mesh) {
		// ��� �� ������� �� �����.
		// ����� � ������ ����� ����� ������������ ����� ��������� ����� 
		// ���������������� ���� ���������� � ����� ������ � � ��������� ������
		// � ��� ������ �� ������������ ��������� �������.
		if (bsT1) {
			Gt = conductivity2Dinsource[iT - maxelm];
			Gt *= prop[MULT_LAM_Z][iP];
		}
		if (bsB1) {
			Gb = conductivity2Dinsource[iB - maxelm];
			Gb *= prop[MULT_LAM_Z][iP];
		}
		if (bsN1) {
			Gn = conductivity2Dinsource[iN - maxelm];
			Gn *= prop[MULT_LAM_Y][iP];
		}
		if (bsS1) {
			Gs = conductivity2Dinsource[iS - maxelm];
			Gs *= prop[MULT_LAM_Y][iP];
		}
		if (bsE1) {
			Ge = conductivity2Dinsource[iE - maxelm];
			Ge *= prop[MULT_LAM_X][iP];
		}
		if (bsW1) {
			Gw = conductivity2Dinsource[iW - maxelm];
			Gw *= prop[MULT_LAM_X][iP];
		}

	}

	// �����.
	// �������� ������������ ����������������:
	//printf("Ge=%e, Gw=%e, Gn=%e, Gs=%e, Gt=%e, Gb=%e\n",Ge,Gw,Gn,Gs,Gt,Gb);
	//getchar();

	// ������������ ������������ ������:
	// ���� �������� ����� �� ������� ��������� �������, 
	// �� ������� ������������� �� ������� ����� �������� � ������� 
	// ������� ������������ beta ��������� �� �������.
	doublereal De=1.0, Dw=1.0, Dn=1.0, Ds=1.0, Dt=1.0, Db=1.0;
	
	if (!bE) {
		if (bW) De=dbeta*Ge*dy*dz/dxe;
		else De=Ge*dy*dz/dxe;
	} else De=dbeta*Ge*dy*dz/dxe;

	if (!bW) {
		if (bE) Dw=dbeta*Gw*dy*dz/dxw;
		else Dw=Gw*dy*dz/dxw;
	} else Dw=dbeta*Gw*dy*dz/dxw;


	if (!bN) {
		if (bS) Dn=dbeta*Gn*dx*dz/dyn;
		else Dn=Gn*dx*dz/dyn;
	} else Dn=dbeta*Gn*dx*dz/dyn;

	
	if (!bS) {
		if (bN) Ds=dbeta*Gs*dx*dz/dys;
		else Ds=Gs*dx*dz/dys;
	} else Ds=dbeta*Gs*dx*dz/dys; 

	if (!bT) {
		if (bB) Dt=dbeta*Gt*dx*dy/dzt; 
		else Dt=Gt*dx*dy/dzt;
	} else Dt=dbeta*Gt*dx*dy/dzt;

	if (!bB) {
		if (bT) Db=dbeta*Gb*dx*dy/dzb;
		else Db=Gb*dx*dy/dzb;
	} else Db=dbeta*Gb*dx*dy/dzb;

	doublereal baddDFLUX2=0.0;
	// ���� ����� ������� ������� ������������� �������� ���������� � ����� �����.
	bool bhighorder=false; // �������� ��� ��������� ������� ������������� ������� ��������.
	
	
	// ����� �����:
	doublereal Pe=0.0, Pw=0.0, Pn=0.0, Ps=0.0, Pt=0.0, Pb=0.0;
	Pe=Fe/De; 
	Pw=-Fw/Dw;
	Pn=Fn/Dn;
	Ps=-Fs/Ds;
    Pt=Ft/Dt;
	Pb=-Fb/Db;


		

	// ������� � ������ ����� ��� ������������� ����� �������� QUICK
	// � ���� ������������� ������ ���������� ���������.
	// addition to the right side QUICK Leonard.
	doublereal attrs=0.0;

	if (b_on_adaptive_local_refinement_mesh) {

		// ���������������� ���������.
		sl[iP].ae = 0.0;
		sl[iP].aw = 0.0;
		sl[iP].an = 0.0;
		sl[iP].as = 0.0;
		sl[iP].at = 0.0;
		sl[iP].ab = 0.0;

		sl[iP].ae2 = 0.0;
		sl[iP].aw2 = 0.0;
		sl[iP].an2 = 0.0;
		sl[iP].as2 = 0.0;
		sl[iP].at2 = 0.0;
		sl[iP].ab2 = 0.0;

		sl[iP].ae3 = 0.0;
		sl[iP].aw3 = 0.0;
		sl[iP].an3 = 0.0;
		sl[iP].as3 = 0.0;
		sl[iP].at3 = 0.0;
		sl[iP].ab3 = 0.0;

		sl[iP].ae4 = 0.0;
		sl[iP].aw4 = 0.0;
		sl[iP].an4 = 0.0;
		sl[iP].as4 = 0.0;
		sl[iP].at4 = 0.0;
		sl[iP].ab4 = 0.0;

		sl[iP].ae_dop = 0.0;
		sl[iP].aw_dop = 0.0;
		sl[iP].an_dop = 0.0;
		sl[iP].as_dop = 0.0;
		sl[iP].at_dop = 0.0;
		sl[iP].ab_dop = 0.0;

		sl[iP].iE_dop = -1;
		sl[iP].iW_dop = -1;
		sl[iP].iN_dop = -1;
		sl[iP].iS_dop = -1;
		sl[iP].iT_dop = -1;
		sl[iP].iB_dop = -1;

		// ��� ���� ������������� ������ iE2, iE3, iE4.
		// � ����� ���� ����� ��� ������ WSIDE, S, N, B, T.


		// ��������������� ���������. 
		// ������ ���������� 1 ���� 2017.
		// � ���������� ��������������� ���������� �� ���� ����� 
		// ���������� �� fine ����� ��� ������
		// ��������� �������� ����������� ����������� ������������.
		// � ���������� ��������������� ���������� ��  ���� �� ����
		// coarse ����� ����������� ����������, 
		// �� ������� ���� ������� � 7% ������������. 
		const bool bcorrection_ALICE = false;

		// ������ ������� �� ���� �����:
		if (iE > -1) {
			if (bE) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].ae += (Ge*sosedb[iE - maxelm].dS / dxe)*ApproxConvective(fabs((Fe1*sosedb[iE - maxelm].dS) / (Ge*sosedb[iE - maxelm].dS / dxe)), ishconvection) + fmax(-Fe1*sosedb[iE - maxelm].dS, 0); 
						
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh. Fe1.1.\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].ae += Ge*sosedb[iE - maxelm].dS / dxe;
				}

			}
			else {
				// iE ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iE]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ae += (Ge*dy*dz / dxe)*ApproxConvective(fabs((Fe1*dy*dz)/(Ge*dy*dz / dxe)), ishconvection) + fmax(-Fe1*dy*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh. Fe1.2.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ae += Ge*dy*dz / dxe;
					}
				}
				else if (ilevel_alice[iE] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iE, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ae += (Ge*dy_loc*dz_loc / dxe)*ApproxConvective(fabs((Fe1*dy_loc*dz_loc)/(Ge*dy_loc*dz_loc / dxe)), ishconvection) + fmax(-Fe1*dy_loc*dz_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh. Fe1.3.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ae += Ge*dy_loc*dz_loc / dxe;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iE, nvtx, pa, pointP1, 100);

						sl[iP].b += (Ge*dy_loc*dz_loc / dxe)*(potent[iP] - mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP1.y, pointP1.z));
					}


				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iE).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ae += (Ge*dy*dz / dxe)*ApproxConvective(fabs((Fe1*dy*dz)/(Ge*dy*dz / dxe)), ishconvection) + fmax(-Fe1*dy*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh. Fe1.4.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ae += Ge*dy*dz / dxe;
					}
					
					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iE, nvtx, pa, pointP1, 100);

						sl[iP].b += (Ge*dy*dz / dxe)*(potent[iE] - mnk(iE, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP0.y, pointP0.z));
					}
				}
			}
		}

		if (iE2 > -1) {
			if (bE2) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].ae2 += (Ge2*sosedb[iE2 - maxelm].dS / dxe2)*ApproxConvective(fabs((Fe2*sosedb[iE2 - maxelm].dS)/(Ge2*sosedb[iE2 - maxelm].dS / dxe2)), ishconvection) + fmax(-Fe2*sosedb[iE2 - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh. Fe2.1.\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].ae2 += Ge2*sosedb[iE2 - maxelm].dS / dxe2;
				}
			}
			else {
				// iE2 ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iE2]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ae2 += (Ge2*dy*dz / dxe2)*ApproxConvective(fabs((Fe2*dy*dz)/(Ge2*dy*dz / dxe2)), ishconvection) + fmax(-Fe2*dy*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh. Fe2.2.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ae2 += Ge2*dy*dz / dxe2;
					}
				}
				else if (ilevel_alice[iE2] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iE2, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ae2 += (Ge2*dy_loc*dz_loc / dxe2)*ApproxConvective(fabs((Fe2*dy_loc*dz_loc)/(Ge2*dy_loc*dz_loc / dxe2)), ishconvection) + fmax(-Fe2*dy_loc*dz_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh. Fe2.3.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ae2 += Ge2*dy_loc*dz_loc / dxe2;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iE2, nvtx, pa, pointP1, 100);

						sl[iP].b += (Ge2*dy_loc*dz_loc / dxe2)*(potent[iP] - mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP1.y, pointP1.z));
					}


				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iE2).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ae2 += (Ge2*dy*dz / dxe2)*ApproxConvective(fabs((Fe2*dy*dz)/(Ge2*dy*dz / dxe2)), ishconvection) + fmax(-Fe2*dy*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh. Fe2.4.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ae2 += Ge2*dy*dz / dxe2;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iE2, nvtx, pa, pointP1, 100);

						sl[iP].b += (Ge2*dy*dz / dxe2)*(potent[iE2] - mnk(iE2, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP0.y, pointP0.z));
					}
				}
			}
		}

		if (iE3 > -1) {
			if (bE3) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].ae3 += (Ge3*sosedb[iE3 - maxelm].dS / dxe3)*ApproxConvective(fabs((Fe3*sosedb[iE3 - maxelm].dS)/(Ge3*sosedb[iE3 - maxelm].dS / dxe3)), ishconvection) + fmax(-Fe3*sosedb[iE3 - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh. Fe3.1.\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].ae3 += Ge3*sosedb[iE3 - maxelm].dS / dxe3;
				}
			}
			else {
				// iE3 ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iE3]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ae3 += (Ge3*dy*dz / dxe3)*ApproxConvective(fabs((Fe3*dy*dz)/(Ge3*dy*dz / dxe3)), ishconvection) + fmax(-Fe3*dy*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh. Fe3.2.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ae3 += Ge3*dy*dz / dxe3;
					}
				}
				else if (ilevel_alice[iE3] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iE3, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ae3 += (Ge3*dy_loc*dz_loc / dxe3)*ApproxConvective(fabs((Fe3*dy_loc*dz_loc)/(Ge3*dy_loc*dz_loc / dxe3)), ishconvection) + fmax(-Fe3*dy_loc*dz_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh. Fe3.3.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ae3 += Ge3*dy_loc*dz_loc / dxe3;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iE3, nvtx, pa, pointP1, 100);

						sl[iP].b += (Ge3*dy_loc*dz_loc / dxe3)*(potent[iP] - mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP1.y, pointP1.z));
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iE3).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ae3 += (Ge3*dy*dz / dxe3)*ApproxConvective(fabs((Fe3*dy*dz)/(Ge3*dy*dz / dxe3)), ishconvection) + fmax(-Fe3*dy*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh.Fe3.4.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ae3 += Ge3*dy*dz / dxe3;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iE3, nvtx, pa, pointP1, 100);

						sl[iP].b += (Ge3*dy*dz / dxe3)*(potent[iE3] - mnk(iE3, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP0.y, pointP0.z));
					}

				}
			}
		}

		if (iE4 > -1) {
			if (bE4) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].ae4 += (Ge4*sosedb[iE4 - maxelm].dS / dxe4)*ApproxConvective(fabs((Fe4*sosedb[iE4 - maxelm].dS)/(Ge4*sosedb[iE4 - maxelm].dS / dxe4)), ishconvection) + fmax(-Fe4*sosedb[iE4 - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh. Fe4.1\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].ae4 += Ge4*sosedb[iE4 - maxelm].dS / dxe4;
				}
			}
			else {
				// iE4 ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iE4]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ae4 += (Ge4*dy*dz / dxe4)*ApproxConvective(fabs((Fe4*dy*dz)/(Ge4*dy*dz / dxe4)), ishconvection) + fmax(-Fe4*dy*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh. Fe4.2\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ae4 += Ge4*dy*dz / dxe4;
					}
				}
				else if (ilevel_alice[iE4] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iE4, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ae4 += (Ge4*dy_loc*dz_loc / dxe4)*ApproxConvective(fabs((Fe4*dy_loc*dz_loc)/(Ge4*dy_loc*dz_loc / dxe4)), ishconvection) + fmax(-Fe4*dy_loc*dz_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh. Fe4.3\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ae4 += Ge4*dy_loc*dz_loc / dxe4;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iE4, nvtx, pa, pointP1, 100);

						sl[iP].b += (Ge4*dy_loc*dz_loc / dxe4)*(potent[iP] - mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP1.y, pointP1.z));
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iE).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ae4 += (Ge4*dy*dz / dxe4)*ApproxConvective(fabs((Fe4*dy*dz)/(Ge4*dy*dz / dxe4)), ishconvection) + fmax(-Fe4*dy*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fe4.4\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ae4 += Ge4*dy*dz / dxe4;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iE4, nvtx, pa, pointP1, 100);

						sl[iP].b += (Ge4*dy*dz / dxe4)*(potent[iE4] - mnk(iE4, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP0.y, pointP0.z));
					}
				}
			}
		}

		// ����� � ���������.

		if (iW > -1) {
			if (bW) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].aw += (Gw*sosedb[iW - maxelm].dS / dxw)*ApproxConvective(fabs((Fw1*sosedb[iW - maxelm].dS)/(Gw*sosedb[iW - maxelm].dS / dxw)), ishconvection) + fmax(Fw1*sosedb[iW - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Fw1.1\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].aw += Gw*sosedb[iW - maxelm].dS / dxw;
				}
			}
			else {
				// iW ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iW]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].aw += (Gw*dy*dz / dxw)*ApproxConvective(fabs((Fw1*dy*dz)/(Gw*dy*dz / dxw)), ishconvection) + fmax(Fw1*dy*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fw1.2\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].aw += Gw*dy*dz / dxw;
					}
				}
				else if (ilevel_alice[iW] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iW, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].aw += (Gw*dy_loc*dz_loc / dxw)*ApproxConvective(fabs((Fw1*dy_loc*dz_loc)/(Gw*dy_loc*dz_loc / dxw)), ishconvection) + fmax(Fw1*dy_loc*dz_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fw1.3\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].aw += Gw*dy_loc*dz_loc / dxw;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iW, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gw*dy_loc*dz_loc / dxw)*( mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP1.y, pointP1.z) - potent[iP]);
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iW).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].aw += (Gw*dy*dz / dxw)*ApproxConvective(fabs((Fw1*dy*dz)/(Gw*dy*dz / dxw)), ishconvection) + fmax(Fw1*dy*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fw1.4\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].aw += Gw*dy*dz / dxw;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iW, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gw*dy*dz / dxw)*( mnk(iW, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP0.y, pointP0.z)- potent[iW] );
					}

				}
			}
		}

		if (iW2 > -1) {
			if (bW2) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].aw2 += (Gw2*sosedb[iW2 - maxelm].dS / dxw2)*ApproxConvective(fabs((Fw2*sosedb[iW2 - maxelm].dS)/(Gw2*sosedb[iW2 - maxelm].dS / dxw2)), ishconvection) + fmax(Fw2*sosedb[iW2 - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Fw2.1\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].aw2 += Gw2*sosedb[iW2 - maxelm].dS / dxw2;
				}
			}
			else {
				// iW2 ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iW2]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].aw2 += (Gw2*dy*dz / dxw2)*ApproxConvective(fabs((Fw2*dy*dz)/(Gw2*dy*dz / dxw2)), ishconvection) + fmax(Fw2*dy*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fw2.2\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].aw2 += Gw2*dy*dz / dxw2;
					}
				}
				else if (ilevel_alice[iW2] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iW2, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].aw2 += (Gw2*dy_loc*dz_loc / dxw2)*ApproxConvective(fabs((Fw2*dy_loc*dz_loc)/(Gw2*dy_loc*dz_loc / dxw2)), ishconvection) + fmax(Fw2*dy_loc*dz_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fw2.3\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].aw2 += Gw2*dy_loc*dz_loc / dxw2;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iW2, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gw2*dy_loc*dz_loc / dxw2)*(mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP1.y, pointP1.z) - potent[iP]);
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iW2).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].aw2 += (Gw2*dy*dz / dxw2)*ApproxConvective(fabs((Fw2*dy*dz)/(Gw2*dy*dz / dxw2)), ishconvection) + fmax(Fw2*dy*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fw2.4.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].aw2 += Gw2*dy*dz / dxw2;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iW2, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gw2*dy*dz / dxw2)*(mnk(iW2, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP0.y, pointP0.z) - potent[iW2]);
					}
				}
			}
		}

		if (iW3 > -1) {
			if (bW3) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].aw3 += (Gw3*sosedb[iW3 - maxelm].dS / dxw3)*ApproxConvective(fabs((Fw3*sosedb[iW3 - maxelm].dS)/(Gw3*sosedb[iW3 - maxelm].dS / dxw3)), ishconvection) + fmax(Fw3*sosedb[iW3 - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Fw3.1\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].aw3 += Gw3*sosedb[iW3 - maxelm].dS / dxw3;
				}
			}
			else {
				// iW3 ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iW3]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].aw3 += (Gw3*dy*dz / dxw3)*ApproxConvective(fabs((Fw3*dy*dz)/(Gw3*dy*dz / dxw3)), ishconvection) + fmax(Fw3*dy*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fw3.2\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].aw3 += Gw3*dy*dz / dxw3;
					}
				}
				else if (ilevel_alice[iW3] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iW3, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].aw3 += (Gw3*dy_loc*dz_loc / dxw3)*ApproxConvective(fabs((Fw3*dy_loc*dz_loc)/(Gw3*dy_loc*dz_loc / dxw3)), ishconvection) + fmax(Fw3*dy_loc*dz_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fw3.3\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].aw3 += Gw3*dy_loc*dz_loc / dxw3;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iW3, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gw3*dy_loc*dz_loc / dxw3)*(mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP1.y, pointP1.z) - potent[iP]);
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iW).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].aw3 += (Gw3*dy*dz / dxw3)*ApproxConvective(fabs((Fw3*dy*dz)/(Gw3*dy*dz / dxw3)), ishconvection) + fmax(Fw3*dy*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fw3.4\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].aw3 += Gw3*dy*dz / dxw3;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iW3, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gw3*dy*dz / dxw3)*(mnk(iW3, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP0.y, pointP0.z) - potent[iW3]);
					}

				}
			}
		}

		if (iW4 > -1) {
			if (bW4) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].aw4 += (Gw4*sosedb[iW4 - maxelm].dS / dxw4)*ApproxConvective(fabs((Fw4*sosedb[iW4 - maxelm].dS)/(Gw4*sosedb[iW4 - maxelm].dS / dxw4)), ishconvection) + fmax(Fw4*sosedb[iW4 - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Fw4.1\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].aw4 += Gw4*sosedb[iW4 - maxelm].dS / dxw4;
				}
			}
			else {
				// iW4 ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iW4]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].aw4 += (Gw4*dy*dz / dxw4)*ApproxConvective(fabs((Fw4*dy*dz)/(Gw4*dy*dz / dxw4)), ishconvection) + fmax(Fw4*dy*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fw4.2\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].aw4 += Gw4*dy*dz / dxw4;
					}
				}
				else if (ilevel_alice[iW4] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iW4, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].aw4 += (Gw4*dy_loc*dz_loc / dxw4)*ApproxConvective(fabs((Fw4*dy_loc*dz_loc)/(Gw4*dy_loc*dz_loc / dxw4)), ishconvection) + fmax(Fw4*dy_loc*dz_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fw4.3\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].aw4 += Gw4*dy_loc*dz_loc / dxw4;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iW4, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gw4*dy_loc*dz_loc / dxw4)*(mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP1.y, pointP1.z) - potent[iP]);
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iW4).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].aw4 += (Gw4*dy*dz / dxw4)*ApproxConvective(fabs((Fw4*dy*dz)/(Gw4*dy*dz / dxw4)), ishconvection) + fmax(Fw4*dy*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fw4.4\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].aw4 += Gw4*dy*dz / dxw4;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iW4, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gw4*dy*dz / dxw4)*(mnk(iW4, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP0.y, pointP0.z) - potent[iW4]);
					}

				}
			}
		}

		// ����� W ���������.

		if (iN > -1) {
			if (bN) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].an += (Gn*sosedb[iN - maxelm].dS / dyn)*ApproxConvective(fabs((Fn1*sosedb[iN - maxelm].dS)/(Gn*sosedb[iN - maxelm].dS / dyn)), ishconvection) + fmax(-Fn1*sosedb[iN - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Fn1.1\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].an += Gn*sosedb[iN - maxelm].dS / dyn;
				}
			}
			else {
				// iN ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iN]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].an += (Gn*dx*dz / dyn)*ApproxConvective(fabs((Fn1*dx*dz)/(Gn*dx*dz / dyn)), ishconvection) + fmax(-Fn1*dx*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fn1.2.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].an += Gn*dx*dz / dyn;
					}
				}
				else if (ilevel_alice[iN] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iN, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].an += (Gn*dx_loc*dz_loc / dyn)*ApproxConvective(fabs((Fn1*dx_loc*dz_loc)/(Gn*dx_loc*dz_loc / dyn)), ishconvection) + fmax(-Fn1*dx_loc*dz_loc, 0); ;
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fn1.3.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].an += Gn*dx_loc*dz_loc / dyn;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iN, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gn*dx_loc*dz_loc / dyn)*(potent[iP] - mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP0.y, pointP1.z));
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iN).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].an += (Gn*dx*dz / dyn)*ApproxConvective(fabs((Fn1*dx*dz)/(Gn*dx*dz / dyn)), ishconvection) + fmax(-Fn1*dx*dz, 0); 
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fn1.4.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].an += Gn*dx*dz / dyn;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iN, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gn*dx*dz / dyn)*(potent[iN] - mnk(iN, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP1.y, pointP0.z));
					}

				}
			}
		}


		if (iN2 > -1) {
			if (bN2) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].an2 += (Gn2*sosedb[iN2 - maxelm].dS / dyn2)*ApproxConvective(fabs((Fn2*sosedb[iN2 - maxelm].dS)/(Gn2*sosedb[iN2 - maxelm].dS / dyn2)), ishconvection) + fmax(-Fn2*sosedb[iN2 - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Fn2.1\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].an2 += Gn2*sosedb[iN2 - maxelm].dS / dyn2;
				}
			}
			else {
				// iN2 ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iN2]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].an2 += (Gn2*dx*dz / dyn2)*ApproxConvective(fabs((Fn2*dx*dz)/(Gn2*dx*dz / dyn2)), ishconvection) + fmax(-Fn2*dx*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fn2.2\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].an2 += Gn2*dx*dz / dyn2;
					}
				}
				else if (ilevel_alice[iN2] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iN2, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].an2 += (Gn2*dx_loc*dz_loc / dyn2)*ApproxConvective(fabs((Fn2*dx_loc*dz_loc)/(Gn2*dx_loc*dz_loc / dyn2)), ishconvection) + fmax(-Fn2*dx_loc*dz_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fn2.3\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].an2 += Gn2*dx_loc*dz_loc / dyn2;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iN2, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gn2*dx_loc*dz_loc / dyn2)*(potent[iP] - mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP0.y, pointP1.z));
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iN2).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].an2 += (Gn2*dx*dz / dyn2)*ApproxConvective(fabs((Fn2*dx*dz)/(Gn2*dx*dz / dyn2)), ishconvection) + fmax(-Fn2*dx*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fn2.4\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].an2 += Gn2*dx*dz / dyn2;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iN2, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gn2*dx*dz / dyn2)*(potent[iN2] - mnk(iN2, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP1.y, pointP0.z));
					}

				}
			}
		}


		if (iN3 > -1) {
			if (bN3) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].an3 += (Gn3*sosedb[iN3 - maxelm].dS / dyn3)*ApproxConvective(fabs((Fn3*sosedb[iN3 - maxelm].dS)/(Gn3*sosedb[iN3 - maxelm].dS / dyn3)), ishconvection) + fmax(-Fn3*sosedb[iN3 - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Fn3.1\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].an3 += Gn3*sosedb[iN3 - maxelm].dS / dyn3;
				}
			}
			else {
				// iN ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iN3]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].an3 += (Gn3*dx*dz / dyn3)*ApproxConvective(fabs((Fn3*dx*dz)/(Gn3*dx*dz / dyn3)), ishconvection) + fmax(-Fn3*dx*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fn3.2\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].an3 += Gn3*dx*dz / dyn3;
					}
				}
				else if (ilevel_alice[iN3] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iN3, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].an3 += (Gn3*dx_loc*dz_loc / dyn3)*ApproxConvective(fabs((Fn3*dx_loc*dz_loc)/(Gn3*dx_loc*dz_loc / dyn3)), ishconvection) + fmax(-Fn3*dx_loc*dz_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fn3.3\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].an3 += Gn3*dx_loc*dz_loc / dyn3;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iN3, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gn3*dx_loc*dz_loc / dyn3)*(potent[iP] - mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP0.y, pointP1.z));
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iN3).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].an3 += (Gn3*dx*dz / dyn3)*ApproxConvective(fabs((Fn3*dx*dz)/(Gn3*dx*dz / dyn3)), ishconvection) + fmax(-Fn3*dx*dz, 0); ;
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fn3.4\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].an3 += Gn3*dx*dz / dyn3;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iN3, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gn3*dx*dz / dyn3)*(potent[iN3] - mnk(iN3, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP1.y, pointP0.z));
					}

				}
			}
		}

		if (iN4 > -1) {
			if (bN4) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].an4 += (Gn4*sosedb[iN4 - maxelm].dS / dyn4)*ApproxConvective(fabs((Fn4*sosedb[iN4 - maxelm].dS)/(Gn4*sosedb[iN4 - maxelm].dS / dyn4)), ishconvection) + fmax(-Fn4*sosedb[iN4 - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Fn4.1\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].an4 += Gn4*sosedb[iN4 - maxelm].dS / dyn4;
				}
			}
			else {
				// iN4 ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iN4]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].an4 += (Gn4*dx*dz / dyn4)*ApproxConvective(fabs((Fn4*dx*dz)/(Gn4*dx*dz / dyn4)), ishconvection) + fmax(-Fn4*dx*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fn4.2\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].an4 += Gn4*dx*dz / dyn4;
					}
				}
				else if (ilevel_alice[iN4] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iN4, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].an4 += (Gn4*dx_loc*dz_loc / dyn4)*ApproxConvective(fabs((Fn4*dx_loc*dz_loc)/(Gn4*dx_loc*dz_loc / dyn4)), ishconvection) + fmax(-Fn4*dx_loc*dz_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fn4.3\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].an4 += Gn4*dx_loc*dz_loc / dyn4;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iN4, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gn4*dx_loc*dz_loc / dyn4)*(potent[iP] - mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP0.y, pointP1.z));
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iN4).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].an4 += (Gn4*dx*dz / dyn4)*ApproxConvective(fabs((Fn4*dx*dz)/(Gn4*dx*dz / dyn4)), ishconvection) + fmax(-Fn4*dx*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fn4.4\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].an4 += Gn4*dx*dz / dyn4;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iN4, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gn4*dx*dz / dyn4)*(potent[iN4] - mnk(iN4, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP1.y, pointP0.z));
					}

				}
			}
		}

		// ����� N ���������.

		if (iS > -1) {
			if (bS) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].as += (Gs*sosedb[iS - maxelm].dS / dys)*ApproxConvective(fabs((Fs1*sosedb[iS - maxelm].dS)/(Gs*sosedb[iS - maxelm].dS / dys)), ishconvection) + fmax(Fs1*sosedb[iS - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Fs1.1\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].as += Gs*sosedb[iS - maxelm].dS / dys;
				}
			}
			else {
				// iS ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iS]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].as += (Gs*dx*dz / dys)*ApproxConvective(fabs((Fs1*dx*dz)/(Gs*dx*dz / dys)), ishconvection) + fmax(Fs1*dx*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fs1.2\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].as += Gs*dx*dz / dys;
					}
				}
				else if (ilevel_alice[iS] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iS, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].as += (Gs*dx_loc*dz_loc / dys)*ApproxConvective(fabs((Fs1*dx_loc*dz_loc)/(Gs*dx_loc*dz_loc / dys)), ishconvection) + fmax(Fs1*dx_loc*dz_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fs1.3\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].as += Gs*dx_loc*dz_loc / dys;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iS, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gs*dx_loc*dz_loc / dys)*(mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP0.y, pointP1.z) - potent[iP]);
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iS).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].as += (Gs*dx*dz / dys)*ApproxConvective(fabs((Fs1*dx*dz)/(Gs*dx*dz / dys)), ishconvection) + fmax(Fs1*dx*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fs1.4\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].as += Gs*dx*dz / dys;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iS, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gs*dx*dz / dys)*(mnk(iS, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP1.y, pointP0.z) - potent[iS]);
					}

				}
			}
		}

		if (iS2 > -1) {
			if (bS2) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].as2 += (Gs2*sosedb[iS2 - maxelm].dS / dys2)*ApproxConvective(fabs((Fs2*sosedb[iS2 - maxelm].dS)/(Gs2*sosedb[iS2 - maxelm].dS / dys2)), ishconvection) + fmax(Fs2*sosedb[iS2 - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Fs2.1\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].as2 += Gs2*sosedb[iS2 - maxelm].dS / dys2;
				}
			}
			else {
				// iS2 ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iS2]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].as2 += (Gs2*dx*dz / dys2)*ApproxConvective(fabs((Fs2*dx*dz)/(Gs2*dx*dz / dys2)), ishconvection) + fmax(Fs2*dx*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fs2.2\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].as2 += Gs2*dx*dz / dys2;
					}
				}
				else if (ilevel_alice[iS2] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iS2, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].as2 += (Gs2*dx_loc*dz_loc / dys2)*ApproxConvective(fabs((Fs2*dx_loc*dz_loc)/(Gs2*dx_loc*dz_loc / dys2)), ishconvection) + fmax(Fs2*dx_loc*dz_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fs2.3\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].as2 += Gs2*dx_loc*dz_loc / dys2;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iS2, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gs2*dx_loc*dz_loc / dys2)*(mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP0.y, pointP1.z) - potent[iP]);
					}
				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iS2).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].as2 += (Gs2*dx*dz / dys2)*ApproxConvective(fabs((Fs2*dx*dz)/(Gs2*dx*dz / dys2)), ishconvection) + fmax(Fs2*dx*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fs2.4\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].as2 += Gs2*dx*dz / dys2;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iS2, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gs2*dx*dz / dys2)*(mnk(iS2, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP1.y, pointP0.z) - potent[iS2]);
					}
				}
			}
		}

		if (iS3 > -1) {
			if (bS3) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].as3 += (Gs3*sosedb[iS3 - maxelm].dS / dys3)*ApproxConvective(fabs((Fs3*sosedb[iS3 - maxelm].dS)/(Gs3*sosedb[iS3 - maxelm].dS / dys3)), ishconvection) + fmax(Fs3*sosedb[iS3 - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Fs3.1\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].as3 += Gs3*sosedb[iS3 - maxelm].dS / dys3;
				}
			}
			else {
				// iS3 ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iS3]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].as3 += (Gs3*dx*dz / dys3)*ApproxConvective(fabs((Fs3*dx*dz)/(Gs3*dx*dz / dys3)), ishconvection) + fmax(Fs3*dx*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fs3.2.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].as3 += Gs3*dx*dz / dys3;
					}
				}
				else if (ilevel_alice[iS3] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iS3, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].as3 += (Gs3*dx_loc*dz_loc / dys3)*ApproxConvective(fabs((Fs3*dx_loc*dz_loc)/(Gs3*dx_loc*dz_loc / dys3)), ishconvection) + fmax(Fs3*dx_loc*dz_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fs3.3\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].as3 += Gs3*dx_loc*dz_loc / dys3;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iS3, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gs3*dx_loc*dz_loc / dys3)*(mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP0.y, pointP1.z) - potent[iP]);
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iS3).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].as3 += (Gs3*dx*dz / dys3)*ApproxConvective(fabs((Fs3*dx*dz)/(Gs3*dx*dz / dys3)), ishconvection) + fmax(Fs3*dx*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fs3.4\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].as3 += Gs3*dx*dz / dys3;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iS3, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gs3*dx*dz / dys3)*(mnk(iS3, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP1.y, pointP0.z) - potent[iS3]);
					}
				}
			}
		}

		if (iS4 > -1) {
			if (bS4) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].as4 += (Gs4*sosedb[iS4 - maxelm].dS / dys4)*ApproxConvective(fabs((Fs4*sosedb[iS4 - maxelm].dS)/(Gs4*sosedb[iS4 - maxelm].dS / dys4)), ishconvection) + fmax(Fs4*sosedb[iS4 - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Fs4.1\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].as4 += Gs4*sosedb[iS4 - maxelm].dS / dys4;
				}
			}
			else {
				// iS4 ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iS4]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].as4 += (Gs4*dx*dz / dys4)*ApproxConvective(fabs((Fs4*dx*dz)/(Gs4*dx*dz / dys4)), ishconvection) + fmax(Fs4*dx*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fs4.2\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].as4 += Gs4*dx*dz / dys4;
					}
				}
				else if (ilevel_alice[iS4] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iS4, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].as4 += (Gs4*dx_loc*dz_loc / dys4)*ApproxConvective(fabs((Fs4*dx_loc*dz_loc)/(Gs4*dx_loc*dz_loc / dys4)), ishconvection) + fmax(Fs4*dx_loc*dz_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fs4.3\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].as4 += Gs4*dx_loc*dz_loc / dys4;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iS4, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gs4*dx_loc*dz_loc / dys4)*(mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP0.y, pointP1.z) - potent[iP]);
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iS4).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].as4 += (Gs4*dx*dz / dys4)*ApproxConvective(fabs((Fs4*dx*dz)/(Gs4*dx*dz / dys4)), ishconvection) + fmax(Fs4*dx*dz, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fs4.4\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].as4 += Gs4*dx*dz / dys4;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iS4, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gs4*dx*dz / dys4)*(mnk(iS4, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP1.y, pointP0.z) - potent[iS4]);
					}
				}
			}
		}

		// ����� S ���������.

		if (iT > -1) {
			if (bT) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].at += (Gt*sosedb[iT - maxelm].dS / dzt)*ApproxConvective(fabs((Ft1*sosedb[iT - maxelm].dS)/(Gt*sosedb[iT - maxelm].dS / dzt)), ishconvection) + fmax(-Ft1*sosedb[iT - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Ft1.1.\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].at += Gt*sosedb[iT - maxelm].dS / dzt;
				}
			}
			else {
				// iT ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iT]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].at += (Gt*dx*dy / dzt)*ApproxConvective(fabs((Ft1*dx*dy)/(Gt*dx*dy / dzt)), ishconvection) + fmax(-Ft1*dx*dy, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Ft1.2.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].at += Gt*dx*dy / dzt;
					}
				}
				else if (ilevel_alice[iT] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iT, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].at += (Gt*dx_loc*dy_loc / dzt)*ApproxConvective(fabs((Ft1*dx_loc*dy_loc)/(Gt*dx_loc*dy_loc / dzt)), ishconvection) + fmax(-Ft1*dx_loc*dy_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Ft1.3.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].at += Gt*dx_loc*dy_loc / dzt;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iT, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gt*dx_loc*dy_loc / dzt)*(potent[iP] - mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP1.y, pointP0.z));
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iT).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].at += (Gt*dx*dy / dzt)*ApproxConvective(fabs((Ft1*dx*dy)/(Gt*dx*dy / dzt)), ishconvection) + fmax(-Ft1*dx*dy, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Ft1.4.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].at += Gt*dx*dy / dzt;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iT, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gt*dx*dy / dzt)*(potent[iT] - mnk(iT, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP0.y, pointP1.z));
					}
				}
			}
		}


		if (iT2 > -1) {
			if (bT2) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].at2 += (Gt2*sosedb[iT2 - maxelm].dS / dzt2)*ApproxConvective(fabs((Ft2*sosedb[iT2 - maxelm].dS)/(Gt2*sosedb[iT2 - maxelm].dS / dzt2)), ishconvection) + fmax(-Ft2*sosedb[iT2 - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Ft2.1.\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].at2 += Gt2*sosedb[iT2 - maxelm].dS / dzt2;
				}
			}
			else {
				// iT2 ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iT2]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].at2 += (Gt2*dx*dy / dzt2)*ApproxConvective(fabs((Ft2*dx*dy)/(Gt2*dx*dy / dzt2)), ishconvection) + fmax(-Ft2*dx*dy, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Ft2.2.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].at2 += Gt2*dx*dy / dzt2;
					}
				}
				else if (ilevel_alice[iT2] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iT2, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].at2 += (Gt2*dx_loc*dy_loc / dzt2)*ApproxConvective(fabs((Ft2*dx_loc*dy_loc)/(Gt2*dx_loc*dy_loc / dzt2)), ishconvection) + fmax(-Ft2*dx_loc*dy_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Ft2.3.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].at2 += Gt2*dx_loc*dy_loc / dzt2;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iT2, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gt2*dx_loc*dy_loc / dzt2)*(potent[iP] - mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP1.y, pointP0.z));
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iT2).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].at2 += (Gt2*dx*dy / dzt2)*ApproxConvective(fabs((Ft2*dx*dy)/(Gt2*dx*dy / dzt2)), ishconvection) + fmax(-Ft2*dx*dy, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Ft2.4.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].at2 += Gt2*dx*dy / dzt2;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iT2, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gt2*dx*dy / dzt2)*(potent[iT2] - mnk(iT2, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP0.y, pointP1.z));
					}

				}
			}
		}


		if (iT3 > -1) {
			if (bT3) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].at3 += (Gt3*sosedb[iT3 - maxelm].dS / dzt3)*ApproxConvective(fabs((Ft3*sosedb[iT3 - maxelm].dS)/(Gt3*sosedb[iT3 - maxelm].dS / dzt3)), ishconvection) + fmax(-Ft3*sosedb[iT3 - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Ft3.1.\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].at3 += Gt3*sosedb[iT3 - maxelm].dS / dzt3;
				}
			}
			else {
				// iT3 ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iT3]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].at3 += (Gt3*dx*dy / dzt3)*ApproxConvective(fabs((Ft3*dx*dy)/(Gt3*dx*dy / dzt3)), ishconvection) + fmax(-Ft3*dx*dy, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Ft3.2\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].at3 += Gt3*dx*dy / dzt3;
					}
				}
				else if (ilevel_alice[iT3] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iT3, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].at3 += (Gt3*dx_loc*dy_loc / dzt3)*ApproxConvective(fabs((Ft3*dx_loc*dy_loc)/(Gt3*dx_loc*dy_loc / dzt3)), ishconvection) + fmax(-Ft3*dx_loc*dy_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Ft3.3.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].at3 += Gt3*dx_loc*dy_loc / dzt3;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iT3, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gt3*dx_loc*dy_loc / dzt3)*(potent[iP] - mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP1.y, pointP0.z));
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iT3).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].at3 += (Gt3*dx*dy / dzt3)*ApproxConvective(fabs((Ft3*dx*dy)/(Gt3*dx*dy / dzt3)), ishconvection) + fmax(-Ft3*dx*dy, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Ft3.4.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].at3 += Gt3*dx*dy / dzt3;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iT3, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gt3*dx*dy / dzt3)*(potent[iT3] - mnk(iT3, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP0.y, pointP1.z));
					}

				}
			}
		}

		if (iT4 > -1) {
			if (bT4) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].at4 += (Gt4*sosedb[iT4 - maxelm].dS / dzt4)*ApproxConvective(fabs((Ft4*sosedb[iT4 - maxelm].dS)/(Gt4*sosedb[iT4 - maxelm].dS / dzt4)), ishconvection) + fmax(-Ft4*sosedb[iT4 - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Ft4.1\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].at4 += Gt4*sosedb[iT4 - maxelm].dS / dzt4;
				}
			}
			else {
				// iT4 ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iT4]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].at4 += (Gt4*dx*dy / dzt4)*ApproxConvective(fabs((Ft4*dx*dy)/(Gt4*dx*dy)), ishconvection) + fmax(-Ft4*dx*dy, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Ft4.2\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].at4 += Gt4*dx*dy / dzt4;
					}
				}
				else if (ilevel_alice[iT4] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iT4, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].at4 += (Gt4*dx_loc*dy_loc / dzt4)*ApproxConvective(fabs((Ft4*dx_loc*dy_loc)/(Gt4*dx_loc*dy_loc / dzt4)), ishconvection) + fmax(-Ft4*dx_loc*dy_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Ft4.3\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].at4 += Gt4*dx_loc*dy_loc / dzt4;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iT4, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gt4*dx_loc*dy_loc / dzt4)*(potent[iP] - mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP1.y, pointP0.z));
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iT4).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].at4 += (Gt4*dx*dy / dzt4)*ApproxConvective(fabs((Ft4*dx*dy)/(Gt4*dx*dy / dzt4)), ishconvection) + fmax(-Ft4*dx*dy, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Ft4.4\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].at4 += Gt4*dx*dy / dzt4;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iT4, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gt4*dx*dy / dzt4)*(potent[iT4] - mnk(iT4, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP0.y, pointP1.z));
					}

				}
			}
		}

		// ����� T ���������.

		if (iB > -1) {
			if (bB) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].ab += (Gb*sosedb[iB - maxelm].dS / dzb)*ApproxConvective(fabs((Fb1*sosedb[iB - maxelm].dS)/(Gb*sosedb[iB - maxelm].dS / dzb)), ishconvection) + fmax(Fb1*sosedb[iB - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Fb1.1\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].ab += Gb*sosedb[iB - maxelm].dS / dzb;
				}
			}
			else {
				// iB ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iB]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ab += (Gb*dx*dy / dzb)*ApproxConvective(fabs((Fb1*dx*dy)/(Gb*dx*dy / dzb)), ishconvection) + fmax(Fb1*dx*dy, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fb1.2\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ab += Gb*dx*dy / dzb;
					}
				}
				else if (ilevel_alice[iB] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iB, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ab += (Gb*dx_loc*dy_loc / dzb)*ApproxConvective(fabs((Fb1*dx_loc*dy_loc)/(Gb*dx_loc*dy_loc / dzb)), ishconvection) + fmax(Fb1*dx_loc*dy_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fb1.3\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ab += Gb*dx_loc*dy_loc / dzb;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iB, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gb*dx_loc*dy_loc / dzb)*(mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP1.y, pointP0.z) - potent[iP]);
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iB).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ab += (Gb*dx*dy / dzb)*ApproxConvective(fabs((Fb1*dx*dy)/(Gb*dx*dy / dzb)), ishconvection) + fmax(Fb1*dx*dy, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fb1.4\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ab += Gb*dx*dy / dzb;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iB, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gb*dx*dy / dzb)*(mnk(iB, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP0.y, pointP1.z) - potent[iB]);
					}
				}
			}
		}

		if (iB2 > -1) {
			if (bB2) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].ab2 += (Gb2*sosedb[iB2 - maxelm].dS / dzb2)*ApproxConvective(fabs((Fb2*sosedb[iB2 - maxelm].dS)/(Gb2*sosedb[iB2 - maxelm].dS / dzb2)), ishconvection) + fmax(Fb2*sosedb[iB2 - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Fb2.1\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].ab2 += Gb2*sosedb[iB2 - maxelm].dS / dzb2;
				}
			}
			else {
				// iB2 ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iB2]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ab2 += (Gb2*dx*dy / dzb2)*ApproxConvective(fabs((Fb2*dx*dy)/(Gb2*dx*dy / dzb2)), ishconvection) + fmax(Fb2*dx*dy, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fb2.2\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ab2 += Gb2*dx*dy / dzb2;
					}
				}
				else if (ilevel_alice[iB2] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iB2, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ab2 += (Gb2*dx_loc*dy_loc / dzb2)*ApproxConvective(fabs((Fb2*dx_loc*dy_loc)/(Gb2*dx_loc*dy_loc / dzb2)), ishconvection) + fmax(Fb2*dx_loc*dy_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fb2.3\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ab2 += Gb2*dx_loc*dy_loc / dzb2;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iB2, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gb2*dx_loc*dy_loc / dzb2)*(mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP1.y, pointP0.z) - potent[iP]);
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iB2).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ab2 += (Gb2*dx*dy / dzb2)*ApproxConvective(fabs((Fb2*dx*dy)/(Gb2*dx*dy / dzb2)), ishconvection) + fmax(Fb2*dx*dy, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fb2.4\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ab2 += Gb2*dx*dy / dzb2;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iB2, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gb2*dx*dy / dzb2)*(mnk(iB2, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP0.y, pointP1.z) - potent[iB2]);
					}

				}
			}
		}

		if (iB3 > -1) {
			if (bB3) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].ab3 += (Gb3*sosedb[iB3 - maxelm].dS / dzb3)*ApproxConvective(fabs((Fb3*sosedb[iB3 - maxelm].dS)/(Gb3*sosedb[iB3 - maxelm].dS / dzb3)), ishconvection) + fmax(Fb3*sosedb[iB3 - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Fb3.1\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].ab3 += Gb3*sosedb[iB3 - maxelm].dS / dzb3;
				}
			}
			else {
				// iB3 ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iB3]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ab3 += (Gb3*dx*dy / dzb3)*ApproxConvective(fabs((Fb3*dx*dy)/(Gb3*dx*dy / dzb3)), ishconvection) + fmax(Fb3*dx*dy, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fb3.2\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ab3 += Gb3*dx*dy / dzb3;
					}
				}
				else if (ilevel_alice[iB3] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iB3, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ab3 += (Gb3*dx_loc*dy_loc / dzb3)*ApproxConvective(fabs((Fb3*dx_loc*dy_loc)/(Gb3*dx_loc*dy_loc / dzb3)), ishconvection) + fmax(Fb3*dx_loc*dy_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fb3.3.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ab3 += Gb3*dx_loc*dy_loc / dzb3;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iB3, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gb3*dx_loc*dy_loc / dzb3)*(mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP1.y, pointP0.z) - potent[iP]);
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iB3).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ab3 += (Gb3*dx*dy / dzb3)*ApproxConvective(fabs((Fb3*dx*dy)/(Gb3*dx*dy / dzb3)), ishconvection) + fmax(Fb3*dx*dy, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fb3.4\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ab3 += Gb3*dx*dy / dzb3;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iB3, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gb3*dx*dy / dzb3)*(mnk(iB3, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP0.y, pointP1.z) - potent[iB3]);
					}
				}
			}
		}

		if (iB4 > -1) {
			if (bB4) {
				// ��������� ����.
				if (bconvective)
				{
					if (ishconvection < distsheme) {
						sl[iP].ab4 += (Gb4*sosedb[iB4 - maxelm].dS / dzb4)*ApproxConvective(fabs((Fb4*sosedb[iB4 - maxelm].dS)/(Gb4*sosedb[iB4 - maxelm].dS / dzb4)), ishconvection) + fmax(Fb4*sosedb[iB4 - maxelm].dS, 0);
					}
					else {
						printf("convective scheme my be only first order, becouse using  ALICE Mesh Fb4.1\n");
						system("pause");
						exit(1);
					}
				}
				else {
					sl[iP].ab4 += Gb4*sosedb[iB4 - maxelm].dS / dzb4;
				}
			}
			else {
				// iB4 ���������� ����.
				// �������� 3 ������:
				// 1. ������ ���������.
				// 2. ������� ������ ���� (� ����� ��� ��������� �� 2, 3 ��� 4 ������).
				// 3. ������� ������ ���� (����� ������� ������ ��� ����������� ������������ ������������).
				if (ilevel_alice[iP] == ilevel_alice[iB4]) {
					// ������ �����:
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ab4 += (Gb4*dx*dy / dzb4)*ApproxConvective(fabs((Fb4*dx*dy)/(Gb4*dx*dy / dzb4)), ishconvection) + fmax(Fb4*dx*dy, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fb4.2.\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ab4 += Gb4*dx*dy / dzb4;
					}
				}
				else if (ilevel_alice[iB4] > ilevel_alice[iP]) {
					// ��� ������ ���� �� �������.
					// ����� ���������� ������ �� ae2, ae3, ae4. ��� ����� ���������� ���� �� ������.

					// ����� ����� �� ������ ����:
					// ���������.
					// ���������� �������� ��������� ������������ ������:
					doublereal dx_loc = 0.0, dy_loc = 0.0, dz_loc = 0.0;// ����� �������� ������������� ������
					volume3D(iB4, nvtx, pa, dx_loc, dy_loc, dz_loc);
					// ������� ������ �� ����� ���� ������ ��� ������� ����� ���� ������ iP.
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ab4 += (Gb4*dx_loc*dy_loc / dzb4)*ApproxConvective(fabs((Fb4*dx_loc*dy_loc)/(Gb4*dx_loc*dy_loc / dzb4)), ishconvection) + fmax(Fb4*dx_loc*dy_loc, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fb4.3\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ab4 += Gb4*dx_loc*dy_loc / dzb4;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iB4, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gb4*dx_loc*dy_loc / dzb4)*(mnk(iP, maxelm, potent, nvtx, pa, sosedi, pointP1.x, pointP1.y, pointP0.z) - potent[iP]);
					}

				}
				else {
					// ������� iP ���� ������ ������.
					// ���������� ������������� ������������.
					// ����� ������� ������. (24 ������ ��� ����� iB4).
					if (bconvective)
					{
						if (ishconvection < distsheme) {
							sl[iP].ab4 += (Gb4*dx*dy / dzb4)*ApproxConvective(fabs((Fb4*dx*dy)/(Gb4*dx*dy / dzb4)), ishconvection) + fmax(Fb4*dx*dy, 0);
						}
						else {
							printf("convective scheme my be only first order, becouse using  ALICE Mesh Fb4.4\n");
							system("pause");
							exit(1);
						}
					}
					else {
						sl[iP].ab4 += Gb4*dx*dy / dzb4;
					}

					if (bcorrection_ALICE) {
						// ����� ���������� ����� ���������� ���������,
						// �� ����������� �������������� ������� ������� �����
						// ������������ ��� ������ �������. 
						// ��� �������� �������� ������������� �� ���� �����.
						// 01.07.2017.

						TOCHKA pointP1;
						center_cord3D(iB4, nvtx, pa, pointP1, 100);

						sl[iP].b += (Gb4*dx*dy / dzb4)*(mnk(iB4, maxelm, potent, nvtx, pa, sosedi, pointP0.x, pointP0.y, pointP1.z) - potent[iB4]);
					}

				}
			}
		}

		//  ����� B ���������.
		// 28 �������� 2016. 10_51.


	}
	else {

		if (ishconvection < distsheme) {
			// ���������� ������������� ����������� �������:
			if (bconvective)
			{
				if (!bsE) sl[iP].ae = De*ApproxConvective(fabs(Pe), ishconvection) + fmax(-Fe, 0); else sl[iP].ae = 0.0;
				if (!bsW) sl[iP].aw = Dw*ApproxConvective(fabs(Pw), ishconvection) + fmax(Fw, 0);  else sl[iP].aw = 0.0;
				if (!bsN) sl[iP].an = Dn*ApproxConvective(fabs(Pn), ishconvection) + fmax(-Fn, 0); else sl[iP].an = 0.0;
				if (!bsS) sl[iP].as = Ds*ApproxConvective(fabs(Ps), ishconvection) + fmax(Fs, 0); else sl[iP].as = 0.0;
				if (!bsT) sl[iP].at = Dt*ApproxConvective(fabs(Pt), ishconvection) + fmax(-Ft, 0); else sl[iP].at = 0.0;
				if (!bsB) sl[iP].ab = Db*ApproxConvective(fabs(Pb), ishconvection) + fmax(Fb, 0); else sl[iP].ab = 0.0;

			}
			else
			{
				
				// ���������� 
				// �������������
				// ����������� �������:
				if (!bsE) sl[iP].ae = De; else sl[iP].ae = 0.0;
				if (!bsW) sl[iP].aw = Dw; else sl[iP].aw = 0.0;
				if (!bsN) sl[iP].an = Dn; else sl[iP].an = 0.0;
				if (!bsS) sl[iP].as = Ds; else sl[iP].as = 0.0;
				if (!bsT) sl[iP].at = Dt; else sl[iP].at = 0.0;
				if (!bsB) sl[iP].ab = Db; else sl[iP].ab = 0.0;

				baddDFLUX2 = 0.0; // �������������.
				if (bhighorder) { // ��������� !
					// ���� bborder == false �� ���� ������ ����������.
					// ���� bborder == true �� �� ��������� ������ ���������� ����.
					bool bborder = false;
					doublereal myflux = 0.0;/*
					myflux=De*(dxe*DFDXiP(potent, iP, ESIDE, sosedi, maxelm, nvtx, pa, bborder)+(potent[iP]-potent[iE]));
					baddDFLUX2+=myflux;
					myflux=Dw*(-dxw*DFDXiP(potent, iP, WSIDE, sosedi, maxelm, nvtx, pa, bborder)+(potent[iP]-potent[iW]));
					baddDFLUX2+=myflux;
					myflux=Dn*(dyn*DFDXiP(potent, iP, N, sosedi, maxelm, nvtx, pa, bborder)+(potent[iP]-potent[iN]));
					baddDFLUX2+=myflux;
					myflux=Ds*(-dys*DFDXiP(potent, iP, S, sosedi, maxelm, nvtx, pa, bborder)+(potent[iP]-potent[iS]));
					baddDFLUX2+=myflux;
					myflux=Dt*(dzt*DFDXiP(potent, iP, TSIDE, sosedi, maxelm, nvtx, pa, bborder)+(potent[iP]-potent[iT]));
					baddDFLUX2+=myflux;
					myflux=Db*(-dzb*DFDXiP(potent, iP, B, sosedi, maxelm, nvtx, pa, bborder)+(potent[iP]-potent[iB]));
					baddDFLUX2+=myflux;
					//baddDFLUX2*=-1.0;*/
					myflux = De*(dxe*DFDXiP(potent, iP, ESIDE, sosedi, maxelm, nvtx, pa, bborder) + (potent[iP] - potent[iE]));
					if (!bborder) baddDFLUX2 += myflux;
					myflux = Dw*(-dxw*DFDXiP(potent, iP, WSIDE, sosedi, maxelm, nvtx, pa, bborder) + (potent[iP] - potent[iW]));
					if (!bborder) baddDFLUX2 += myflux;
					myflux = Dn*(dyn*DFDXiP(potent, iP, NSIDE, sosedi, maxelm, nvtx, pa, bborder) + (potent[iP] - potent[iN]));
					if (!bborder) baddDFLUX2 += myflux;
					myflux = Ds*(-dys*DFDXiP(potent, iP, SSIDE, sosedi, maxelm, nvtx, pa, bborder) + (potent[iP] - potent[iS]));
					if (!bborder) baddDFLUX2 += myflux;
					myflux = Dt*(dzt*DFDXiP(potent, iP, TSIDE, sosedi, maxelm, nvtx, pa, bborder) + (potent[iP] - potent[iT]));
					if (!bborder) baddDFLUX2 += myflux;
					myflux = Db*(-dzb*DFDXiP(potent, iP, BSIDE, sosedi, maxelm, nvtx, pa, bborder) + (potent[iP] - potent[iB]));
					if (!bborder) baddDFLUX2 += myflux;
					//baddDFLUX2*=-1.0;  // ��������� ����� ������ +1.0.
				}

			}
		}
		else if (ishconvection < QUICK)
		{
			// ���������� ������������� ����������� �������:
			if (bconvective)
			{
				if (!bsE) sl[iP].ae = -Fe*fC(Pe, ishconvection, true, feplus) + De*fD(Pe, ishconvection, true, feplus); else sl[iP].ae = 0.0;
				if (!bsW) sl[iP].aw = Fw*fC(Pw, ishconvection, true, fwplus) + Dw*fD(Pw, ishconvection, true, fwplus); else sl[iP].aw = 0.0;
				if (!bsN) sl[iP].an = -Fn*fC(Pn, ishconvection, true, fnplus) + Dn*fD(Pn, ishconvection, true, fnplus); else sl[iP].an = 0.0;
				if (!bsS) sl[iP].as = Fs*fC(Ps, ishconvection, true, fsplus) + Ds*fD(Ps, ishconvection, true, fsplus); else sl[iP].as = 0.0;
				if (!bsT) sl[iP].at = -Ft*fC(Pt, ishconvection, true, ftplus) + Dt*fD(Pt, ishconvection, true, ftplus); else sl[iP].at = 0.0;
				if (!bsB) sl[iP].ab = Fb*fC(Pb, ishconvection, true, fbplus) + Db*fD(Pb, ishconvection, true, fbplus); else sl[iP].ab = 0.0;
			}
			else
			{
				// ����� ����� ����� 0.
				// fD(Pe) ���������� 1.0.

				// ���������� 
				// �������������
				// ����������� �������:
				if (!bsE) sl[iP].ae = De; else sl[iP].ae = 0.0;
				if (!bsW) sl[iP].aw = Dw; else sl[iP].aw = 0.0;
				if (!bsN) sl[iP].an = Dn; else sl[iP].an = 0.0;
				if (!bsS) sl[iP].as = Ds; else sl[iP].as = 0.0;
				if (!bsT) sl[iP].at = Dt; else sl[iP].at = 0.0;
				if (!bsB) sl[iP].ab = Db; else sl[iP].ab = 0.0;

				baddDFLUX2 = 0.0; // �������������.
				if (bhighorder) { // ��������� !
					// ���� bborder == false �� ���� ������ ����������.
					// ���� bborder == true �� �� ��������� ������ ���������� ����.
					bool bborder = false;
					doublereal myflux = 0.0;/*
					myflux=De*(dxe*DFDXiP(potent, iP, ESIDE, sosedi, maxelm, nvtx, pa, bborder)+(potent[iP]-potent[iE]));
					baddDFLUX2+=myflux;
					myflux=Dw*(-dxw*DFDXiP(potent, iP, WSIDE, sosedi, maxelm, nvtx, pa, bborder)+(potent[iP]-potent[iW]));
					baddDFLUX2+=myflux;
					myflux=Dn*(dyn*DFDXiP(potent, iP, N, sosedi, maxelm, nvtx, pa, bborder)+(potent[iP]-potent[iN]));
					baddDFLUX2+=myflux;
					myflux=Ds*(-dys*DFDXiP(potent, iP, S, sosedi, maxelm, nvtx, pa, bborder)+(potent[iP]-potent[iS]));
					baddDFLUX2+=myflux;
					myflux=Dt*(dzt*DFDXiP(potent, iP, TSIDE, sosedi, maxelm, nvtx, pa, bborder)+(potent[iP]-potent[iT]));
					baddDFLUX2+=myflux;
					myflux=Db*(-dzb*DFDXiP(potent, iP, B, sosedi, maxelm, nvtx, pa, bborder)+(potent[iP]-potent[iB]));
					baddDFLUX2+=myflux;
					//baddDFLUX2*=-1.0;*/
					myflux = De*(dxe*DFDXiP(potent, iP, ESIDE, sosedi, maxelm, nvtx, pa, bborder) + (potent[iP] - potent[iE]));
					if (!bborder) baddDFLUX2 += myflux;
					myflux = Dw*(-dxw*DFDXiP(potent, iP, WSIDE, sosedi, maxelm, nvtx, pa, bborder) + (potent[iP] - potent[iW]));
					if (!bborder) baddDFLUX2 += myflux;
					myflux = Dn*(dyn*DFDXiP(potent, iP, NSIDE, sosedi, maxelm, nvtx, pa, bborder) + (potent[iP] - potent[iN]));
					if (!bborder) baddDFLUX2 += myflux;
					myflux = Ds*(-dys*DFDXiP(potent, iP, SSIDE, sosedi, maxelm, nvtx, pa, bborder) + (potent[iP] - potent[iS]));
					if (!bborder) baddDFLUX2 += myflux;
					myflux = Dt*(dzt*DFDXiP(potent, iP, TSIDE, sosedi, maxelm, nvtx, pa, bborder) + (potent[iP] - potent[iT]));
					if (!bborder) baddDFLUX2 += myflux;
					myflux = Db*(-dzb*DFDXiP(potent, iP, BSIDE, sosedi, maxelm, nvtx, pa, bborder) + (potent[iP] - potent[iB]));
					if (!bborder) baddDFLUX2 += myflux;
					//baddDFLUX2*=-1.0;  // ��������� ����� ������ +1.0.
				}
			}
		}
		else if (ishconvection >= QUICK)
		{

			// � ������ ���������.
			bool berrE = false, berrW = false, berrN = false, berrS = false, berrT = false, berrB = false;
			bool bpatch = true;

			// � 3D ������������ ������ ����� ������������ �� ��� ���������� �����.
			// ����� �������� ����� ������ ������� � ����������� � ������� ��������� ���������� ���������.


			// X - direction
			doublereal positionxP, positionxE, positionxW, positionxEE, positionxWW, positionxe, positionxw;
			doublereal FiP = 0.0, FiE = 0.0, FiW = 0.0, FiEE = 0.0, FiWW = 0.0, FiN = 0.0, FiS = 0.0;
			doublereal FiNN = 0.0, FiSS = 0.0, FiT = 0.0, FiB = 0.0, FiTT = 0.0, FiBB = 0.0;
			doublereal Fie = 0.0, Fiw = 0.0, Fin = 0.0, Fis = 0.0, Fit = 0.0, Fib = 0.0;
			// Y - direction
			doublereal positionyP, positionyN, positionyS, positionyNN, positionySS, positionyn, positionys;
			// Z - direction
			doublereal positionzP, positionzT, positionzB, positionzTT, positionzBB, positionzt, positionzb;


			if (bconvective)
			{

				
				

				TOCHKA pointP;
				center_cord3D(iP, nvtx, pa, pointP,100);
				//printf("iP");
				//system("pause");
				positionxP = pointP.x; positionyP = pointP.y; positionzP = pointP.z;
				FiP = potent[iP];
				// X - direction
				if (!bE) {
					FiE = potent[iE];
					center_cord3D(iE, nvtx, pa, pointP,ESIDE);
					//printf("iE");
					//system("pause");
					positionxE = pointP.x;
					positionxe = positionxP + 0.5*dx;

					integer iEE = sosedi[EE][iP].iNODE1;
					if ((iEE>=0)&&(iEE < maxelm)) {
						// ���������� ����
						FiEE = potent[iEE];
						center_cord3D(iEE, nvtx, pa, pointP,EE);
						//printf("iEE");
						//system("pause");
						positionxEE = pointP.x;
						integer ib_1, ib_2, ib_3, ib_4;
						ib_1 = whot_is_block[iP];
						ib_2 = whot_is_block[iE];
						ib_3 = whot_is_block[iEE];
						if (!bW) ib_4 = whot_is_block[iW];
						if ((!bW)&&(b[ib_1].itype == b[ib_2].itype) && (b[ib_1].itype == b[ib_3].itype) && (b[ib_1].itype == b[ib_4].itype)) {

						}
						else {
							if ((bpatch) || (b[ib_2].itype == SOLID) && (b[ib_3].itype == FLUID)) {
								//printf("06.01.2018 FLUID SOLID FLUID sequence\n");
								//getchar();
								//FiEE = 0.0;
								berrE = true;
							}
						}
					}
					else
					{
						integer ib_1 = whot_is_block[iP];
						integer ib_2 = whot_is_block[iE];
						if ((b[ib_1].itype != b[ib_2].itype)) {
							//printf("problem found\n");
							//getchar();
							berrE = true;
						}

						// ��������� ����
						FiEE = potent[iEE];
						volume3D(iE, nvtx, pa, pointP.x, pointP.y, pointP.z);
						positionxEE = positionxE + 0.5*pointP.x;
					}
				}
				else {
					// ��� ��������� ����
					FiE = potent[iE];
					FiEE = potent[iE];
					positionxe = positionxP + 0.5*dx;
					positionxE = positionxP + 0.5*dx;
					positionxEE = positionxP + dx; // ����� ���� �� ���������� !
				}

				if (!bW) {
					center_cord3D(iW, nvtx, pa, pointP,WSIDE);
					//printf("iW");
					//system("pause");
					positionxW = pointP.x;
					positionxw = positionxP - 0.5*dx;
					FiW = potent[iW];

					integer iWW = sosedi[WW][iP].iNODE1;
					if ((iWW >= 0) && (iWW < maxelm)) {
						// ���������� ����
						FiWW = potent[iWW];
						center_cord3D(iWW, nvtx, pa, pointP,WW);
						//printf("iWW");
						//system("pause");
						positionxWW = pointP.x;

						integer ib_1, ib_2, ib_3, ib_4;
						ib_1 = whot_is_block[iP];
						ib_2 = whot_is_block[iW];
						ib_3 = whot_is_block[iWW];
						if (!bE) ib_4 = whot_is_block[iE];
						if ((!bE) && (b[ib_1].itype == b[ib_2].itype) && (b[ib_1].itype == b[ib_3].itype) && (b[ib_1].itype == b[ib_4].itype)) {

						}
						else {
							if ((bpatch) || (b[ib_2].itype == SOLID) && (b[ib_3].itype == FLUID)) {
								//printf("06.01.2018 FLUID SOLID FLUID sequence\n");
								//getchar();
								//FiWW = 0.0;
								berrW = true;
							}
						}
					}
					else
					{
						integer ib_1 = whot_is_block[iP];
						integer ib_2 = whot_is_block[iW];
						if ((b[ib_1].itype != b[ib_2].itype)) {
							//printf("problem found\n");
							//getchar();
							berrW = true;
						}

						// ��������� ����
						FiWW = potent[iWW];
						volume3D(iW, nvtx, pa, pointP.x, pointP.y, pointP.z);
						positionxWW = positionxW - 0.5*pointP.x;
					}
				}
				else {
					// ��� ��������� ����
					FiW = potent[iW];
					FiWW = potent[iW];
					positionxw = positionxP - 0.5*dx;
					positionxW = positionxP - 0.5*dx;
					positionxWW = positionxP - dx; // ����� ���� �� ���������� !
				}

				// Y - direction
				if (!bN) {
					FiN = potent[iN];
					center_cord3D(iN, nvtx, pa, pointP,NSIDE);
					//printf("iN");
					//system("pause");
					positionyN = pointP.y;
					positionyn = positionxP + 0.5*dy;

					integer iNN = sosedi[NN][iP].iNODE1;
					if ((iNN >= 0) && (iNN < maxelm)) {
						// ���������� ����
						FiNN = potent[iNN];
						center_cord3D(iNN, nvtx, pa, pointP,NN);
						//printf("iNN");
						//system("pause");
						positionyNN = pointP.y;

						integer ib_1, ib_2, ib_3, ib_4;
						ib_1 = whot_is_block[iP];
						ib_2 = whot_is_block[iN];
						ib_3 = whot_is_block[iNN];
						if (!bS) ib_4 = whot_is_block[iS];
						if ((!bS) && (b[ib_1].itype == b[ib_2].itype) && (b[ib_1].itype == b[ib_3].itype) && (b[ib_1].itype == b[ib_4].itype)) {

						}
						else {
							if ((bpatch) || (b[ib_2].itype == SOLID) && (b[ib_3].itype == FLUID)) {
								//printf("06.01.2018 FLUID SOLID FLUID sequence\n");
								//getchar();
								//FiNN = 0.0;
								berrN = true;
							}
						}
					}
					else
					{
						integer ib_1 = whot_is_block[iP];
						integer ib_2 = whot_is_block[iN];
						if ((b[ib_1].itype != b[ib_2].itype)) {
							//printf("problem found\n");
							//getchar();
							berrN = true;
						}

						// ��������� ����
						FiNN = potent[iNN];
						volume3D(iN, nvtx, pa, pointP.x, pointP.y, pointP.z);
						positionyNN = positionyN + 0.5*pointP.y;
					}
				}
				else {
					// ��� ��������� ����
					FiN = potent[iN];
					FiNN = potent[iN];
					positionyn = positionyP + 0.5*dy;
					positionyN = positionyP + 0.5*dy;
					positionyNN = positionyP + dy; // ����� ���� �� ���������� !
				}

				if (!bS) {
					FiS = potent[iS];
					center_cord3D(iS, nvtx, pa, pointP,SSIDE);
					//printf("iS");
					//system("pause");
					positionyS = pointP.y;
					positionys = positionyP - 0.5*dy;

					integer iSS = sosedi[SS][iP].iNODE1;
					if ((iSS >= 0) && (iSS < maxelm)) {
						// ���������� ����
						FiSS = potent[iSS];
						center_cord3D(iSS, nvtx, pa, pointP,SS);
						//printf("iSS");
						//system("pause");
						positionySS = pointP.y;

						integer ib_1, ib_2, ib_3, ib_4;
						ib_1 = whot_is_block[iP];
						ib_2 = whot_is_block[iS];
						ib_3 = whot_is_block[iSS];
						if (!bN) ib_4 = whot_is_block[iN];
						if ((!bN) && (b[ib_1].itype == b[ib_2].itype) && (b[ib_1].itype == b[ib_3].itype) && (b[ib_1].itype == b[ib_4].itype)) {

						}
						else {
							if ((bpatch) || (b[ib_2].itype == SOLID) && (b[ib_3].itype == FLUID)) {
								//printf("06.01.2018 FLUID SOLID FLUID sequence\n");
								//getchar();
								//FiSS = 0.0;
								berrS = true;
							}
						}
					}
					else
					{
						integer ib_1 = whot_is_block[iP];
						integer ib_2 = whot_is_block[iS];
						if ((b[ib_1].itype != b[ib_2].itype)) {
							//printf("problem found\n");
							//getchar();
							berrS = true;
						}

						// ��������� ����
						FiSS = potent[iSS];
						volume3D(iS, nvtx, pa, pointP.x, pointP.y, pointP.z);
						positionySS = positionyS - 0.5*pointP.y;
					}
				}
				else {
					// ��� ��������� ����
					FiS = potent[iS]; // ATTANTION !!!! TODO 
					FiSS = potent[iS]; // ������� �������� ������ ������� ����.
					positionys = positionyP - 0.5*dy;
					positionyS = positionyP - 0.5*dy;
					positionySS = positionyP - dy; // ����� ���� �� ���������� !
				}

				// Z - direction
				if (!bT) {
					FiT = potent[iT];
					center_cord3D(iT, nvtx, pa, pointP,TSIDE);
					//printf("iT");
					//system("pause");
					positionzT = pointP.z;
					positionzt = positionzP + 0.5*dz;

					integer iTT = sosedi[TTSIDE][iP].iNODE1;
					if ((iTT >= 0) && (iTT < maxelm)) {
						// ���������� ����
						FiTT = potent[iTT];
						center_cord3D(iTT, nvtx, pa, pointP,TTSIDE);
						//printf("iTT");
						//system("pause");
						positionzTT = pointP.z;

						integer ib_1, ib_2, ib_3, ib_4;
						ib_1 = whot_is_block[iP];
						ib_2 = whot_is_block[iT];
						ib_3 = whot_is_block[iTT];
						if (!bB) ib_4 = whot_is_block[iB];
						if ((!bB) && (b[ib_1].itype == b[ib_2].itype) && (b[ib_1].itype == b[ib_3].itype) && (b[ib_1].itype == b[ib_4].itype)) {

						}
						else {
							if ((bpatch) || (b[ib_2].itype == SOLID) && (b[ib_3].itype == FLUID)) {
								//printf("06.01.2018 FLUID SOLID FLUID sequence\n");
								//getchar();
								//FiTT = 0.0;
								berrT = true;
							}
						}
					}
					else
					{
						integer ib_1 = whot_is_block[iP];
						integer ib_2 = whot_is_block[iT];
						if ((b[ib_1].itype != b[ib_2].itype)) {
							//printf("problem found\n");
							//getchar();
							berrT = true;
						}
						// ��������� ����
						FiTT = potent[iTT];
						volume3D(iT, nvtx, pa, pointP.x, pointP.y, pointP.z);
						positionzTT = positionzT + 0.5*pointP.z;
					}
				}
				else {
					// ��� ��������� ����
					FiT = potent[iT];
					FiTT = potent[iT]; // �������� ������ ������� ����
					positionzt = positionzP + 0.5*dz;
					positionzT = positionzP + 0.5*dz;
					positionzTT = positionzP + dz; // ����� ���� �� ���������� !
				}

				if (!bB) {
					FiB = potent[iB];
					center_cord3D(iB, nvtx, pa, pointP,BSIDE);
					//printf("iB");
					//system("pause");
					positionzB = pointP.z;
					positionzb = positionzP - 0.5*dz;

					integer iBB = sosedi[BB][iP].iNODE1;
					if ((iBB >= 0) && (iBB < maxelm)) {
						// ���������� ����
						FiBB = potent[iBB];
						center_cord3D(iBB, nvtx, pa, pointP,BB);
						//printf("iBB");
						//system("pause");
						positionzBB = pointP.z;

						integer ib_1, ib_2, ib_3, ib_4;
						ib_1 = whot_is_block[iP];
						ib_2 = whot_is_block[iB];
						ib_3 = whot_is_block[iBB];
						if (!bT) ib_4 = whot_is_block[iT];
						if ((!bT) && (b[ib_1].itype == b[ib_2].itype) && (b[ib_1].itype == b[ib_3].itype) && (b[ib_1].itype == b[ib_4].itype)) {

						}
						else {
							if ((bpatch)||(b[ib_2].itype == SOLID) && (b[ib_3].itype == FLUID)) {
								//printf("06.01.2018 FLUID SOLID FLUID sequence\n");
								//getchar();
								//FiBB = 0.0;
								berrB = true;
							}
						}
					}
					else
					{
						integer ib_1 = whot_is_block[iP];
						integer ib_2 = whot_is_block[iB];
						if ((b[ib_1].itype != b[ib_2].itype)) {
							//printf("problem found\n");
							//getchar();
							berrB = true;
						}
						// ��������� ����
						FiBB = potent[iBB];
						volume3D(iB, nvtx, pa, pointP.x, pointP.y, pointP.z);
						positionzBB = positionzB - 0.5*pointP.z;
					}
				}
				else {
					// ��� ��������� ����
					FiB = potent[iB];
					FiBB = potent[iB]; // �������� ������ ������� ����
					positionzb = positionzP - 0.5*dz;
					positionzB = positionzP - 0.5*dz;
					positionzBB = positionzP - dz; // ����� ���� �� ���������� !
				}

				if ((ishconvection >= QUICK) && (ishconvection <= FROMM)) {
					// X - direction
					Fie = cell_face_value_global(ishconvection, (Fe), FiW, FiP, FiE, FiEE);
					Fiw = cell_face_value_global(ishconvection, (Fw), FiWW, FiW, FiP, FiE);
					// Y - direction
					Fin = cell_face_value_global(ishconvection, (Fn), FiS, FiP, FiN, FiNN);
					Fis = cell_face_value_global(ishconvection, (Fs), FiSS, FiS, FiP, FiN);
					// Z - direction
					Fit = cell_face_value_global(ishconvection, (Ft), FiB, FiP, FiT, FiTT);
					Fib = cell_face_value_global(ishconvection, (Fb), FiBB, FiB, FiP, FiT);					

				}

				

				if (ishconvection >= UNEVENQUICK) {


					/*
					// ����������������� �������� ��������� � ����� ���������� ���������� ����� QUICK �� ������������� �����.
					// ���������� ���� ������������ �� ������ : ...
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

					if (ishconvection == UNEVENQUICK)
					{
						// X - direction
						Fie = workQUICK(dx, 2.0*(positionxE - positionxe), positionxW, positionxP, positionxE, positionxEE, FiW, FiP, FiE, FiEE, (Fe));
						Fiw = workQUICK(2.0*(positionxw - positionxW), dx, positionxWW, positionxW, positionxP, positionxE, FiWW, FiW, FiP, FiE, (Fw));
						// Y - direction
						Fin = workQUICK(dy, 2.0*(positionyN - positionyn), positionyS, positionyP, positionyN, positionyNN, FiS, FiP, FiN, FiNN, (Fn));
						Fis = workQUICK(2.0*(positionys - positionyS), dy, positionySS, positionyS, positionyP, positionyN, FiSS, FiS, FiP, FiN, (Fs));
						// Z - direction
						Fit = workQUICK(dz, 2.0*(positionzT - positionzt), positionzB, positionzP, positionzT, positionzTT, FiB, FiP, FiT, FiTT, (Ft));
						Fib = workQUICK(2.0*(positionzb - positionzB), dz, positionzBB, positionzB, positionzP, positionzT, FiBB, FiB, FiP, FiT, (Fb));
					}

					if ((ishconvection > UNEVENQUICK) && (ishconvection <= UNEVEN_CUBISTA))
					{
						// ���� �� ������ ������ ������������� ����������� ������������ ������ ������ ������ ����� :
						// 1. UNEVEN_MUSCL, 2. UNEVEN_SOUCUP, 3. UNEVEN_HLPA, 4. UNEVEN_SMART.
						// ������������� ����� ������ ��������������� ��������.

						// X - direction
						Fie = workKN_VOLKOV(positionxW, positionxP, positionxE, positionxEE, FiW, FiP, FiE, FiEE, (Fe), ishconvection);
						Fiw = workKN_VOLKOV(positionxWW, positionxW, positionxP, positionxE, FiWW, FiW, FiP, FiE, (Fw), ishconvection);
						// Y - direction
						Fin = workKN_VOLKOV(positionyS, positionyP, positionyN, positionyNN, FiS, FiP, FiN, FiNN, (Fn), ishconvection);
						Fis = workKN_VOLKOV(positionySS, positionyS, positionyP, positionyN, FiSS, FiS, FiP, FiN, (Fs), ishconvection);
						// Z - direction
						Fit = workKN_VOLKOV(positionzB, positionzP, positionzT, positionzTT, FiB, FiP, FiT, FiTT, (Ft), ishconvection);
						Fib = workKN_VOLKOV(positionzBB, positionzB, positionzP, positionzT, FiBB, FiB, FiP, FiT, (Fb), ishconvection);
						
					}

				}
			}



			// ������: SIMPLE method for the solution of incompressible flows on non-staggered grids
			// I. Sezai - Eastern Mediterranean University, Mechanical Engineering Department, Mersin 10-Turkey Revised in 
			// January, 2011.

			// ���������� ������������� ����������� �������:
			// ����������� ����� ���������� ��������� :
			// ������ ����������� ������ ��������������� �����, 
			// � ���������� ����� ������������ � ������ ����� 
			// �������� ������� ���������.
			if (bconvective)
			{

				//if (!bsE) sl[iP].ae = De*fD(Pe, EXP2, true, feplus) + fmax(-(Fe), 0); else sl[iP].ae = 0.0;
				//if (!bsW) sl[iP].aw = Dw*fD(Pw, EXP2, true, fwplus) + fmax((Fw), 0); else sl[iP].aw = 0.0;
				//if (!bsN) sl[iP].an = Dn*fD(Pn, EXP2, true, fnplus) + fmax(-(Fn), 0); else sl[iP].an = 0.0;
				//if (!bsS) sl[iP].as = Ds*fD(Ps, EXP2, true, fsplus) + fmax((Fs), 0); else sl[iP].as = 0.0;
				//if (!bsT) sl[iP].at = Dt*fD(Pt, EXP2, true, ftplus) + fmax(-(Ft), 0); else sl[iP].at = 0.0;
				//if (!bsB) sl[iP].ab = Db*fD(Pb, EXP2, true, fbplus) + fmax((Fb), 0);  else sl[iP].ab = 0.0;

				// ����� ������ UDS.
				// ��� ����������� � ����������.

				if (!bsE) sl[iP].ae = De + fmax(-(Fe), 0); else sl[iP].ae = 0.0;
				if (!bsW) sl[iP].aw = Dw + fmax((Fw), 0); else sl[iP].aw = 0.0;
				if (!bsN) sl[iP].an = Dn + fmax(-(Fn), 0); else sl[iP].an = 0.0;
				if (!bsS) sl[iP].as = Ds + fmax((Fs), 0); else sl[iP].as = 0.0;
				if (!bsT) sl[iP].at = Dt + fmax(-(Ft), 0); else sl[iP].at = 0.0;
				if (!bsB) sl[iP].ab = Db + fmax((Fb), 0);  else sl[iP].ab = 0.0;

				attrs = 0.0;

				// 6.01.2018
				if (!bsE)  if (!berrE) attrs += -fmax((Fe), 0)*(Fie - FiP) + fmax(-(Fe), 0)*(Fie - FiE);
				if (!bsW)  if (!berrW) attrs += -fmax(-(Fw), 0)*(Fiw - FiP) + fmax((Fw), 0)*(Fiw - FiW);
				if (!bsN)  if (!berrN) attrs += -fmax((Fn), 0)*(Fin - FiP) + fmax(-(Fn), 0)*(Fin - FiN);
				if (!bsS)  if (!berrS) attrs += -fmax(-(Fs), 0)*(Fis - FiP) + fmax((Fs), 0)*(Fis - FiS);
				if (!bsT)  if (!berrT) attrs += -fmax((Ft), 0)*(Fit - FiP) + fmax(-(Ft), 0)*(Fit - FiT);
				if (!bsB)  if (!berrB) attrs += -fmax(-(Fb), 0)*(Fib - FiP) + fmax((Fb), 0)*(Fib - FiB);

				//attrs=0.0; // ����� ����� ��������.
			}
			else
			{
				// ���������� 
				// �������������
				// ����������� �������:
				if (!bsE) sl[iP].ae = De; else sl[iP].ae = 0.0;
				if (!bsW) sl[iP].aw = Dw; else sl[iP].aw = 0.0;
				if (!bsN) sl[iP].an = Dn; else sl[iP].an = 0.0;
				if (!bsS) sl[iP].as = Ds; else sl[iP].as = 0.0;
				if (!bsT) sl[iP].at = Dt; else sl[iP].at = 0.0;
				if (!bsB) sl[iP].ab = Db; else sl[iP].ab = 0.0;

				attrs = 0.0; // ������ ��������.

				baddDFLUX2 = 0.0; // �������������.
				if (bhighorder) { // ��������� !
					// ���� bborder == false �� ���� ������ ����������.
					// ���� bborder == true �� �� ��������� ������ ���������� ����.
					bool bborder = false;
					doublereal myflux = 0.0;/*
					myflux=De*(dxe*DFDXiP(potent, iP, ESIDE, sosedi, maxelm, nvtx, pa, bborder)+(potent[iP]-potent[iE]));
					baddDFLUX2+=myflux;
					myflux=Dw*(-dxw*DFDXiP(potent, iP, WSIDE, sosedi, maxelm, nvtx, pa, bborder)+(potent[iP]-potent[iW]));
					baddDFLUX2+=myflux;
					myflux=Dn*(dyn*DFDXiP(potent, iP, N, sosedi, maxelm, nvtx, pa, bborder)+(potent[iP]-potent[iN]));
					baddDFLUX2+=myflux;
					myflux=Ds*(-dys*DFDXiP(potent, iP, S, sosedi, maxelm, nvtx, pa, bborder)+(potent[iP]-potent[iS]));
					baddDFLUX2+=myflux;
					myflux=Dt*(dzt*DFDXiP(potent, iP, TSIDE, sosedi, maxelm, nvtx, pa, bborder)+(potent[iP]-potent[iT]));
					baddDFLUX2+=myflux;
					myflux=Db*(-dzb*DFDXiP(potent, iP, B, sosedi, maxelm, nvtx, pa, bborder)+(potent[iP]-potent[iB]));
					baddDFLUX2+=myflux;
					//baddDFLUX2*=-1.0;*/
					myflux = De*(dxe*DFDXiP(potent, iP, ESIDE, sosedi, maxelm, nvtx, pa, bborder) + (potent[iP] - potent[iE]));
					if (!bborder) baddDFLUX2 += myflux;
					myflux = Dw*(-dxw*DFDXiP(potent, iP, WSIDE, sosedi, maxelm, nvtx, pa, bborder) + (potent[iP] - potent[iW]));
					if (!bborder) baddDFLUX2 += myflux;
					myflux = Dn*(dyn*DFDXiP(potent, iP, NSIDE, sosedi, maxelm, nvtx, pa, bborder) + (potent[iP] - potent[iN]));
					if (!bborder) baddDFLUX2 += myflux;
					myflux = Ds*(-dys*DFDXiP(potent, iP, SSIDE, sosedi, maxelm, nvtx, pa, bborder) + (potent[iP] - potent[iS]));
					if (!bborder) baddDFLUX2 += myflux;
					myflux = Dt*(dzt*DFDXiP(potent, iP, TSIDE, sosedi, maxelm, nvtx, pa, bborder) + (potent[iP] - potent[iT]));
					if (!bborder) baddDFLUX2 += myflux;
					myflux = Db*(-dzb*DFDXiP(potent, iP, BSIDE, sosedi, maxelm, nvtx, pa, bborder) + (potent[iP] - potent[iB]));
					if (!bborder) baddDFLUX2 += myflux;
					//baddDFLUX2*=-1.0; // ��������� ����� ������ +1.0.
				}
			}


		}
	}

    //printf("%e %e %e %e %e %e\n",sl[iP].ae,sl[iP].aw, sl[iP].an, sl[iP].as, sl[iP].at, sl[iP].ab);
	//getchar(); // GOOD


    doublereal tau, apzero1, apzero0;
	doublereal Fold=0.0; // �������� ������� � ����������� ���������� ����
	if (btimedep) {
	   // ��������������
	   tau=tauparam; // ��� �� �������
	   Fold=toldtimestep[iP]; // ����������� � ����������� ���������� ���� �� �������.
	}
	else {
	   // ������������

	   // ����� �����������:
		// �������� ��������� ������ ������ ����� ��� SIMPLE ��������� �����������
		// �� ������� (�5.1.12) ������� ��������� � ����� 5 �������� �� sigmaflow.
		if (b_on_adaptive_local_refinement_mesh) {
			doublereal sum_2 = sl[iP].ae2 + sl[iP].aw2 + sl[iP].an2 + sl[iP].as2 + sl[iP].at2 + sl[iP].ab2;
			doublereal sum_3 = sl[iP].ae3 + sl[iP].aw3 + sl[iP].an3 + sl[iP].as3 + sl[iP].at3 + sl[iP].ab3;
			doublereal sum_4 = sl[iP].ae4 + sl[iP].aw4 + sl[iP].an4 + sl[iP].as4 + sl[iP].at4 + sl[iP].ab4;
			tau = rP*dx*dy*dz*alpha / ((sl[iP].ae + sl[iP].aw + sl[iP].an + sl[iP].as + sl[iP].at + sl[iP].ab + sum_2 + sum_3 + sum_4));
		}
		else {
			tau = rP*dx*dy*dz*alpha / ((sl[iP].ae + sl[iP].aw + sl[iP].an + sl[iP].as + sl[iP].at + sl[iP].ab));
		}
       Fold=potent[iP];	    	
	}


	// ��������! ��� ������� ��������� �����-������ � �������������� ���������� �������� ����� �����
	// ����� � ����������� ���������� ����. TODO
	if ((!btimedep) && (!imitation_time)) {
		// ������������ � �������� ����� �� ������� �� ������������
		apzero1=0.0; // � ������ ���������� ����.
		apzero0=0.0; // � ����������� ���������� ����
	}
	else {
		// ��� ���������, ��������� 7 ������� 2016.
		// 15 �������� 2016 ��� ������� ��� �������������� ������������� � ������ ����.
		//printf("Told comming\n");
		//getchar();

		apzero1=rP*dx*dy*dz/tau; // rho1*cp1*dx*dy*dz/tau. // ap01
		TOCHKA p; // ���������� ������ ��.
		integer ib; // ����� ����� �������� ����������� ��.
		doublereal rho=1.1614, cp=1005; // ������������� default  dry air 300K 1atm properties
		center_cord3D(iP, nvtx, pa, p,100); // ���������� ��������� ������ ��.
		in_model_temp(p,ib,b,lb); // ���������� ����� ����� ib �������� ����������� ����������� ����� � ������� iP.
		 
		if (matlist[b[ib].imatid].blibmat==1) {
			// ������������, ����������������� ��������.
			if (b[ib].itype==SOLID) {
				doublereal lam=0.0; // �� ������������ ����� ��������.
			    my_solid_properties(Fold, rho, cp, lam, matlist[b[ib].imatid].ilibident);
		    } // SOLID
		    if (b[ib].itype==FLUID) {
			   doublereal mu, beta_t, lam; // �������� �� ������������ �� ���������.
		       doublereal pressure;
			   if (ptr != NULL) {
				   if (ptr[1][iP] == -1) {
					   pressure = 0.0; // �������� ������ ������� ���� (����� �� ����� ����, �.�. ����� ����������� ��������).
				   }
				   else pressure = f[ptr[1][iP]].potent[PRESS][ptr[0][iP]];
			   }
			   else {
				   // ptr==NULL
				   pressure = 0.0;
			   }
			   my_fluid_properties(Fold, pressure, rho, cp, lam, mu, beta_t, matlist[b[ib].imatid].ilibident);
		    } // FLUID
		}
		else if (matlist[b[ib].imatid].blibmat==0) {
			// �������� ����������� �������������:
			// ���������� ��������.
			rho=matlist[b[ib].imatid].rho;
			//cp=matlist[b[ib].imatid].cp;
			cp = get_lam(matlist[b[ib].imatid].n_cp, matlist[b[ib].imatid].temp_cp, matlist[b[ib].imatid].arr_cp, Fold);

		}
		apzero0=rho*cp*dx*dy*dz/tau; // ap00

		//printf("apzero0=%e, apzero1=%e\n",apzero0,apzero1);
		//getchar();
	}

	// ������ �.�. ����������� � ��������� ������� �� �����������,
	// ���������� ����� ��������� ����������� ap00 � ����������� ���������� ����:

	/*if (iP==20) {
	printf("tau=%e\n",tau);
	getchar();
	}*/
  
	// ������������ ����
	doublereal dSc = Sc2D;
	doublereal dSp = 0.0;
	if (btimedep) {
		// �������������� ��������.
		if (ipower_time_depend==0) {
			// �������� ������ ����� �� ������� �� ������� � ���������� ���������.
			dSc += dSc_out;
		}
		else if (ipower_time_depend == 1)  {
			// �������� ���������� ������ � ������� ���������� �� ���� multiplyera.
			if (poweron_multiplier_sequence > 0.0) {
				dSc += dSc_out;
			}
		}
	    else if (ipower_time_depend == 2)  {
		    // Square Wave Apparat.
		    // �������� ���������� ������ � ������� ���������� �� ���� multiplyera
		    // � ������ �������� ��������� multiplyer.
		    if (poweron_multiplier_sequence > 0.0) {
			    dSc += dSc_out*poweron_multiplier_sequence;
	     	}
	    }
		else if (ipower_time_depend == 3) {
			// �������� ���������� ������ � ������� ���������� �� ���� multiplyera.
			if (poweron_multiplier_sequence > 0.0) {
				dSc += dSc_out;
			}
		}
	}
	else {
		dSc += dSc_out;
	}

	/*if (iP==23901) {
		printf("ae=%e, aw=%e, an=%e, as=%e, at=%e, ab=%e\n",sl[iP].ae,sl[iP].aw,sl[iP].an,sl[iP].as,sl[iP].at,sl[iP].ab);
		printf("ap01=%e, source=%e\n",apzero1,dSp*dx*dy*dz);
		getchar();
	}
	*/

	// ���� ������� ������������� � ���� ����� ��������� ��������:
	doublereal rbdissipate=0.0;
	if ((ptr!=NULL) && (ptr[1][iP]!=-1)) {
		// ����������� ����� ����������� ������ ����.
		rbdissipate+=f[ptr[1][iP]].prop[MU][ptr[0][iP]]*f[ptr[1][iP]].SInvariantStrainRateTensor[ptr[0][iP]]*f[ptr[1][iP]].SInvariantStrainRateTensor[ptr[0][iP]];
		/*if (inumglobaliter>=66) {
			printf("mu=%e, tensor=%e\n",f[ptr[1][iP]].prop[MU][ptr[0][iP]], f[ptr[1][iP]].SInvariantStrainRateTensor[ptr[0][iP]]);
		}*/
	}

	//printf("rbdissipate=%e\n",rbdissipate);
	//getchar();

	// �������� ������������� �����.
	//printf("dSp=%e, dSc=%e\n",dSp,dSc);
	//getchar();
	if (b_on_adaptive_local_refinement_mesh) {
		doublereal sum_2 = sl[iP].ae2 + sl[iP].aw2 + sl[iP].an2 + sl[iP].as2 + sl[iP].at2 + sl[iP].ab2;
		doublereal sum_3 = sl[iP].ae3 + sl[iP].aw3 + sl[iP].an3 + sl[iP].as3 + sl[iP].at3 + sl[iP].ab3;
		doublereal sum_4 = sl[iP].ae4 + sl[iP].aw4 + sl[iP].an4 + sl[iP].as4 + sl[iP].at4 + sl[iP].ab4;
		sl[iP].ap = sl[iP].ae + sl[iP].aw + sl[iP].an + sl[iP].as + sl[iP].at + sl[iP].ab + sum_2 + sum_3 + sum_4 + apzero1 - dSp*dx*dy*dz;//+apzero1-dSp*dx*dy*dz; // ������������ ������� �������
	}
	else {

		if (bconvective) {
			if (ishconvection < distsheme) {
				// 07.05.2017
				// ��� ���������:
				// ����� ������������� !!! (���������� �� ����).
				if (!bsE) {
					sl[iP].ap = De*ApproxConvective(fabs(Pe), ishconvection) + fmax(+(Fe), 0);
				}
				else {
					sl[iP].ap = 0.0;
				}
				if (!bsW) {
					sl[iP].ap += Dw*ApproxConvective(fabs(Pw), ishconvection) + fmax(-(Fw), 0);
				}
				if (!bsN) {
					sl[iP].ap += Dn*ApproxConvective(fabs(Pn), ishconvection) + fmax(+(Fn), 0);
				}
				if (!bsS) {
					sl[iP].ap += Ds*ApproxConvective(fabs(Ps), ishconvection) + fmax(-(Fs), 0);
				}
				if (!bsT) {
					sl[iP].ap += Dt*ApproxConvective(fabs(Pt), ishconvection) + fmax(+(Ft), 0);
				}
				if (!bsB) {
					sl[iP].ap += Db*ApproxConvective(fabs(Pb), ishconvection) + fmax(-(Fb), 0);
				}
			}
			else if (ishconvection < QUICK)
			{
				//  8.05.2017
				// ��� ���������:
				// ����� ������������� !!! (���������� �� ����).

				if (!bsE) sl[iP].ap = +Fe*fC(Pe, ishconvection, true, feplus) + De*fD(Pe, ishconvection, true, feplus); else sl[iP].ap = 0.0;
				if (!bsW) sl[iP].ap += -Fw*fC(Pw, ishconvection, true, fwplus) + Dw*fD(Pw, ishconvection, true, fwplus); else sl[iP].ap += 0.0;
				if (!bsN) sl[iP].ap += +Fn*fC(Pn, ishconvection, true, fnplus) + Dn*fD(Pn, ishconvection, true, fnplus); else sl[iP].ap += 0.0;
				if (!bsS) sl[iP].ap += -Fs*fC(Ps, ishconvection, true, fsplus) + Ds*fD(Ps, ishconvection, true, fsplus); else sl[iP].ap += 0.0;
				if (!bsT) sl[iP].ap += +Ft*fC(Pt, ishconvection, true, ftplus) + Dt*fD(Pt, ishconvection, true, ftplus); else sl[iP].ap += 0.0;
				if (!bsB) sl[iP].ap += -Fb*fC(Pb, ishconvection, true, fbplus) + Db*fD(Pb, ishconvection, true, fbplus); else sl[iP].ap += 0.0;
				
			}
			else if (ishconvection >= QUICK)
			{
				
				//  8.05.2017
				// ��� ���������:
				// ����� ������������� !!! (���������� �� ����).
				//if (!bsE) sl[iP].ap = De*fD(Pe, EXP2, true, feplus) + fmax(+(Fe), 0); else sl[iP].ap = 0.0;
				//if (!bsW) sl[iP].ap += Dw*fD(Pw, EXP2, true, fwplus) + fmax(-(Fw), 0); else sl[iP].ap += 0.0;
				//if (!bsN) sl[iP].ap += Dn*fD(Pn, EXP2, true, fnplus) + fmax(+(Fn), 0); else sl[iP].ap += 0.0;
				//if (!bsS) sl[iP].ap += Ds*fD(Ps, EXP2, true, fsplus) + fmax(-(Fs), 0); else sl[iP].ap += 0.0;
				//if (!bsT) sl[iP].ap += Dt*fD(Pt, EXP2, true, ftplus) + fmax(+(Ft), 0); else sl[iP].ap += 0.0;
				//if (!bsB) sl[iP].ap += Db*fD(Pb, EXP2, true, fbplus) + fmax(-(Fb), 0);  else sl[iP].ap += 0.0;

				// ����� ������ UDS.
				// ��� ����������� � ����������.
				// ��� ���������:
				// ����� ������������� !!! (���������� �� ����).
				if (!bsE) sl[iP].ap = De + fmax(+(Fe), 0); else sl[iP].ap = 0.0;
				if (!bsW) sl[iP].ap += Dw + fmax(-(Fw), 0); else sl[iP].ap += 0.0;
				if (!bsN) sl[iP].ap += Dn + fmax(+(Fn), 0); else sl[iP].ap += 0.0;
				if (!bsS) sl[iP].ap += Ds + fmax(-(Fs), 0); else sl[iP].ap += 0.0;
				if (!bsT) sl[iP].ap += Dt + fmax(+(Ft), 0); else sl[iP].ap += 0.0;
				if (!bsB) sl[iP].ap += Db + fmax(-(Fb), 0); else sl[iP].ap += 0.0;

			}
			//sl[iP].ap += apzero1 - dSp*dx*dy*dz;
			// ������ ��� � ����� ����� ��������� �� opening �����.
			sl[iP].ap = sl[iP].ae + sl[iP].aw + sl[iP].an + sl[iP].as + sl[iP].at + sl[iP].ab + apzero1 - dSp*dx*dy*dz;//+apzero1-dSp*dx*dy*dz; // ������������ ������� �������
			


		}
		else {
			sl[iP].ap = sl[iP].ae + sl[iP].aw + sl[iP].an + sl[iP].as + sl[iP].at + sl[iP].ab + apzero1 - dSp*dx*dy*dz;//+apzero1-dSp*dx*dy*dz; // ������������ ������� �������
		}

		
		

	}

	
	
	doublereal ts = sl[iP].ae + sl[iP].aw + sl[iP].an + sl[iP].as + sl[iP].at + sl[iP].ab;
	if (ts != ts) {
		//if (Fe*Fe + Fw*Fw + Fn*Fn + Fs*Fs + Ft*Ft + Fb*Fb > 1.0e-40) {
			printf("PEREPOLNENIE %d ae=%e aw=%e an=%e as=%e at=%e ab=%e\n", iP, Fe, Fw, Fn, Fs, Ft, Fb);
			getchar();
		//}
	}
	

	doublereal rpower_diss = dSc;
	//6.10.2018
	if (!bE && !bW) {
		rpower_diss *= dx;
	}
	else {
		rpower_diss *= 0.5*dx;
	}
	if (!bN && !bS) {
		rpower_diss *= dy;
	}
	else {
		rpower_diss *= 0.5*dy;
	}
	if (!bT && !bB) {
		rpower_diss *= dz;
	}
	else {
		rpower_diss *= 0.5*dz;
	}

	// 3.07.2017. 
	// �������� !!! �������� �� ������� ������������ ���� ������� ������������� ���������.
	sl[iP].b += rpower_diss/*dSc*dx*dy*dz*/+apzero0*Fold;//+apzero0*Fold;// ������ ����� //-Fold*(Fe-Fw+Fn-Fs); // ���� ���� �������� ���������� �������
	// ���� �������������� ����� ������� ����������� ����������� �� ���� ���� ����������� ������ ���� �������.
	sl[iP].b += attrs; // ����� ������� ����������� ����������� ��� ���������. ��������, SMARTER 3 �������.
	// �������� ������������� ������������ � ������ ������������ ��������� �������������,
	// �� ���������� ������� (��������������� ��������� �������������) ����� ����� ����� ��������.
	// �� � ���� ������ �� �������� (Fe-Fw+Fn-Fs+Ft-Fb), �.�. �������� � ������ ����������� ������� � opening �����.
	//sl[iP].ap += fabs(Fe-Fw+Fn-Fs+Ft-Fb);
	//sl[iP].ap += Fe - Fw + Fn - Fs + Ft - Fb;
	//sl[iP].b+=rbdissipate*dx*dy*dz; // ����� ��������
	//sl[iP].b+=baddDFLUX2; // ����� ��������.
	   /*if (inumglobaliter>=66) {
			printf("dSc=%e, apzero0=%e, Fold=%e, rbdissipate=%e, vol=%e\n",dSc,apzero0, Fold,rbdissipate, dx*dy*dz);
			getchar();
		}
		*/

        /*
         if (iP==26) {
		 #if doubleintprecision == 1
			 printf("numberCV=%lld ap=%1.4e ae=%1.4e aw=%1.4e an=%1.4e as=%1.4e at=%1.4e ab=%1.4e b=%1.4e\n",
		 #else
			 printf("numberCV=%d ap=%1.4e ae=%1.4e aw=%1.4e an=%1.4e as=%1.4e at=%1.4e ab=%1.4e b=%1.4e\n",
		 #endif
       
		   iP,sl[iP].ap,sl[iP].ae,sl[iP].aw,sl[iP].an,sl[iP].as,sl[iP].at,sl[iP].ab,sl[iP].b);
       getchar();
	}*/

    // ������������� ������� ����:
	// ��������� ���� ����������� ������ ���������� � ������ �������.
	// � ������ ������ ���������� ������������� ��������� � SPD ��������.
	/*
	// ������ ������� �������� �������� ��������� �������:
	// -ab ... -as ... -aw ... +ap ... -ae ... -an ... -at == b

	
	// ���� ������� ������� �������:
	if ((iE>maxelm) && (slb[iE-maxelm].iI==(-1))) {
		sl[iP].b+=sl[iP].ae*slb[iE-maxelm].b/slb[iE-maxelm].aw;
		sl[iP].ae=0.0;
		sl[iP].iE=-1;
	}
    
	if ((iW>maxelm) && (slb[iW-maxelm].iI==(-1))) {
		sl[iP].b+=sl[iP].aw*slb[iW-maxelm].b/slb[iW-maxelm].aw;
		sl[iP].aw=0.0;
		sl[iP].iW=-1;
	}

	if ((iN>maxelm) && (slb[iN-maxelm].iI==(-1))) {
		sl[iP].b+=sl[iP].an*slb[iN-maxelm].b/slb[iN-maxelm].aw;
		sl[iP].an=0.0;
		sl[iP].iN=-1;
	}

	if ((iS>maxelm) && (slb[iS-maxelm].iI==(-1))) {
		sl[iP].b+=sl[iP].as*slb[iS-maxelm].b/slb[iS-maxelm].aw;
		sl[iP].as=0.0;
		sl[iP].iS=-1;
	}

	if ((iT>maxelm) && (slb[iT-maxelm].iI==(-1))) {
		sl[iP].b+=sl[iP].at*slb[iT-maxelm].b/slb[iT-maxelm].aw;
		sl[iP].at=0.0;
		sl[iP].iT=-1;
	}

	if ((iB>maxelm) && (slb[iB-maxelm].iI==(-1))) {
		sl[iP].b+=sl[iP].ab*slb[iB-maxelm].b/slb[iB-maxelm].aw;
		sl[iP].ab=0.0;
		sl[iP].iB=-1;
	}
	
	*/

    /*
    if (iP==26) {
	#if doubleintprecision == 1
		printf("numberCV=%lld ap=%1.4e ae=%1.4e aw=%1.4e an=%1.4e as=%1.4e at=%1.4e ab=%1.4e b=%1.4e\n",
		iP,sl[iP].ap,sl[iP].ae,sl[iP].aw,sl[iP].an,sl[iP].as,sl[iP].at,sl[iP].ab,sl[iP].b);
	#else
		printf("numberCV=%d ap=%1.4e ae=%1.4e aw=%1.4e an=%1.4e as=%1.4e at=%1.4e ab=%1.4e b=%1.4e\n",
		iP,sl[iP].ap,sl[iP].ae,sl[iP].aw,sl[iP].an,sl[iP].as,sl[iP].at,sl[iP].ab,sl[iP].b);
	#endif
       
       getchar();
	}
    */

	if (fabs(sl[iP].ap) < 1.0e-30) {
		sl[iP].ap = 1.0;
		sl[iP].b = Fold; // ����������� � ���������� ��������.
	}

} // my_elmatr_quad_T3D


/*
// ����������� ������� ���� ��� ����� ������� radiosity Prism Object.
void radiosity_patch_for_vacuum_Prism_Object(equation3D* &sl, equation3D_bon* &slb, BLOCK* &b, integer lb, integer maxelm)
{
	for (integer i = 0; i < lb; i++) {
		if (b[i].radiation.binternalRadiation)
		{
			if ((b[i].radiation.nodelistW != NULL) && (b[i].radiation.nodelistE != NULL) && (b[i].radiation.nodelistS != NULL) && (b[i].radiation.nodelistN != NULL) && (b[i].radiation.nodelistB != NULL) && (b[i].radiation.nodelistT != NULL))
			{
				
				for (integer j = 0; j < b[i].radiation.nodelistWsize; j++) {
					//sl[b[i].radiation.nodelistW[j].node1].b += b[i].radiation.JW*b[i].radiation.nodelistW[j].dS;
					if (b[i].radiation.nodelistW[j].node2 >= maxelm) {
						// ���������.
						// �������� ����� !!!
						//slb[b[i].radiation.nodelistW[j].node2 - maxelm].b += -b[i].radiation.JW*b[i].radiation.nodelistW[j].dS;
					}
					else {
						// ����������.
						// �������� ����� !!!
						sl[b[i].radiation.nodelistW[j].node2].b += -(b[i].radiation.JW-b[i].radiation.JE)*b[i].radiation.nodelistW[j].dS;
					}
				}


				for (integer j = 0; j < b[i].radiation.nodelistEsize; j++) {
					//sl[b[i].radiation.nodelistE[j].node1].b += b[i].radiation.JE*b[i].radiation.nodelistE[j].dS;
					if (b[i].radiation.nodelistE[j].node2 >= maxelm) {
						// ���������.
						// �������� ����� !!!
						//slb[b[i].radiation.nodelistE[j].node2 - maxelm].b += -b[i].radiation.JE*b[i].radiation.nodelistE[j].dS;
					}
					else {
						// ����������.
						// �������� ���� �� ������� West, �.�. ����� East ����� �� ��� ����� � �������� ��������� �� ��� ����� West.
						// ����������� ������ ������ �� ���������� - ������� ���������� West ��������� ����������� Estom.
						sl[b[i].radiation.nodelistE[j].node2].b += (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistE[j].dS;
					}
				}

				for (integer j = 0; j < b[i].radiation.nodelistSsize; j++) {
					//sl[b[i].radiation.nodelistS[j].node1].b += b[i].radiation.JS*b[i].radiation.nodelistS[j].dS;
					if (b[i].radiation.nodelistS[j].node2 >= maxelm) {
						// ���������.
						// �������� ����� !!!
						//slb[b[i].radiation.nodelistS[j].node2 - maxelm].b += -b[i].radiation.JS*b[i].radiation.nodelistS[j].dS;
					}
					else {
						// ����������.
						// �������� ����� !!!
						sl[b[i].radiation.nodelistS[j].node2].b += -(b[i].radiation.JS-b[i].radiation.JN)*b[i].radiation.nodelistS[j].dS;
					}
				}


				for (integer j = 0; j < b[i].radiation.nodelistNsize; j++) {
					//sl[b[i].radiation.nodelistN[j].node1].b += b[i].radiation.JN*b[i].radiation.nodelistN[j].dS;
					if (b[i].radiation.nodelistN[j].node2 >= maxelm) {
						// ���������.
						// �������� ����� !!!
						//slb[b[i].radiation.nodelistN[j].node2 - maxelm].b += -b[i].radiation.JN*b[i].radiation.nodelistN[j].dS;
					}
					else {
						// ����������.
						sl[b[i].radiation.nodelistN[j].node2].b += (b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistN[j].dS;
					}
				}
				

				for (integer j = 0; j < b[i].radiation.nodelistBsize; j++) {
					//sl[b[i].radiation.nodelistB[j].node1].b += b[i].radiation.JB*b[i].radiation.nodelistB[j].dS;
					if (b[i].radiation.nodelistB[j].node2 >= maxelm) {
						// ���������.
						// �������� ����� !!!
						//slb[b[i].radiation.nodelistB[j].node2 - maxelm].b += -b[i].radiation.JB*b[i].radiation.nodelistB[j].dS;
					}
					else {
						// ����������.
						// �������� ����� !!!
						// ������� (������ ����� � ����������� �Y), �� �� �� � ������ �� ��� ����� �� ���� TOP.
						sl[b[i].radiation.nodelistB[j].node2].b += -(b[i].radiation.JB-b[i].radiation.JT)*b[i].radiation.nodelistB[j].dS;
					}
				}


				for (integer j = 0; j < b[i].radiation.nodelistTsize; j++) {
					//sl[b[i].radiation.nodelistT[j].node1].b += b[i].radiation.JT*b[i].radiation.nodelistT[j].dS;
					if (b[i].radiation.nodelistT[j].node2 >= maxelm) {
						// ���������.
						// �������� ����� !!!
						//slb[b[i].radiation.nodelistT[j].node2 - maxelm].b += -b[i].radiation.JT*b[i].radiation.nodelistT[j].dS;
					}
					else {
						// ����������.
						// �������� ���� �� ������� Bottom, �.�. ����� top ����� �� ��� ����� � �������� ��������� �� ��� ����� bottom.
						// ����������� ������ ������ �� ���������� - ������� ���������� botomom ��������� ����������� topom.
						sl[b[i].radiation.nodelistT[j].node2].b += (b[i].radiation.JB-b[i].radiation.JT)*b[i].radiation.nodelistT[j].dS;
					}
				}

			}
		}
	}
} // radiosity_patch_for_vacuum_Prism_Object
*/

/*
// ����������� ������� ���� ��� ����� ������� radiosity Prism Object.
void radiosity_patch_for_vacuum_Prism_Object(equation3D* &sl, equation3D_bon* &slb, BLOCK* &b, integer lb, integer maxelm)
{
	doublereal result, alpha = 0.1;
	for (integer i = 0; i < lb; i++) {
		if (b[i].radiation.binternalRadiation)
		{
			if ((b[i].radiation.nodelistW != NULL) && (b[i].radiation.nodelistE != NULL) && (b[i].radiation.nodelistS != NULL) && (b[i].radiation.nodelistN != NULL) && (b[i].radiation.nodelistB != NULL) && (b[i].radiation.nodelistT != NULL))
			{

				for (integer j = 0; j < b[i].radiation.nodelistWsize; j++) {
					//sl[b[i].radiation.nodelistW[j].node1].b += b[i].radiation.JW*b[i].radiation.nodelistW[j].dS;
					if (b[i].radiation.nodelistW[j].node2 >= maxelm) {
						// ���������.
						// �������� ����� !!!
						//slb[b[i].radiation.nodelistW[j].node2 - maxelm].b += -b[i].radiation.JW*b[i].radiation.nodelistW[j].dS;
					}
					else {
						// ����������.
						// �������� ����� !!!
						result = sl[b[i].radiation.nodelistW[j].node2].b - (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistW[j].dS;
						sl[b[i].radiation.nodelistW[j].node2].b = alpha*(result)+(1 - alpha)*(bsource_term_radiation_for_relax[b[i].radiation.nodelistW[j].node2]);
						bsource_term_radiation_for_relax[b[i].radiation.nodelistW[j].node2] = sl[b[i].radiation.nodelistW[j].node2].b;
						//sl[b[i].radiation.nodelistW[j].node2].b += -(b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistW[j].dS;
					}
				}


				for (integer j = 0; j < b[i].radiation.nodelistEsize; j++) {
					//sl[b[i].radiation.nodelistE[j].node1].b += b[i].radiation.JE*b[i].radiation.nodelistE[j].dS;
					if (b[i].radiation.nodelistE[j].node2 >= maxelm) {
						// ���������.
						// �������� ����� !!!
						//slb[b[i].radiation.nodelistE[j].node2 - maxelm].b += -b[i].radiation.JE*b[i].radiation.nodelistE[j].dS;
					}
					else {
						// ����������.
						// �������� ���� �� ������� West, �.�. ����� East ����� �� ��� ����� � �������� ��������� �� ��� ����� West.
						// ����������� ������ ������ �� ���������� - ������� ���������� West ��������� ����������� Estom.
						result = sl[b[i].radiation.nodelistE[j].node2].b + (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistE[j].dS;
						sl[b[i].radiation.nodelistE[j].node2].b = alpha*result + (1.0 - alpha)*(bsource_term_radiation_for_relax[b[i].radiation.nodelistE[j].node2]);
						bsource_term_radiation_for_relax[b[i].radiation.nodelistE[j].node2] = sl[b[i].radiation.nodelistE[j].node2].b;
						//sl[b[i].radiation.nodelistE[j].node2].b += (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistE[j].dS;
					}
				}

				for (integer j = 0; j < b[i].radiation.nodelistSsize; j++) {
					//sl[b[i].radiation.nodelistS[j].node1].b += b[i].radiation.JS*b[i].radiation.nodelistS[j].dS;
					if (b[i].radiation.nodelistS[j].node2 >= maxelm) {
						// ���������.
						// �������� ����� !!!
						//slb[b[i].radiation.nodelistS[j].node2 - maxelm].b += -b[i].radiation.JS*b[i].radiation.nodelistS[j].dS;
					}
					else {
						// ����������.
						// �������� ����� !!!
						result = sl[b[i].radiation.nodelistS[j].node2].b -(b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistS[j].dS;
						sl[b[i].radiation.nodelistS[j].node2].b = alpha*(result)+(1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistS[j].node2];
						bsource_term_radiation_for_relax[b[i].radiation.nodelistS[j].node2] = sl[b[i].radiation.nodelistS[j].node2].b;
						//sl[b[i].radiation.nodelistS[j].node2].b += -(b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistS[j].dS;
					}
				}


				for (integer j = 0; j < b[i].radiation.nodelistNsize; j++) {
					//sl[b[i].radiation.nodelistN[j].node1].b += b[i].radiation.JN*b[i].radiation.nodelistN[j].dS;
					if (b[i].radiation.nodelistN[j].node2 >= maxelm) {
						// ���������.
						// �������� ����� !!!
						//slb[b[i].radiation.nodelistN[j].node2 - maxelm].b += -b[i].radiation.JN*b[i].radiation.nodelistN[j].dS;
					}
					else {
						// ����������.
						result = sl[b[i].radiation.nodelistN[j].node2].b + (b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistN[j].dS;
						sl[b[i].radiation.nodelistN[j].node2].b = alpha*result + (1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistN[j].node2];
						bsource_term_radiation_for_relax[b[i].radiation.nodelistN[j].node2] = sl[b[i].radiation.nodelistN[j].node2].b;
						//sl[b[i].radiation.nodelistN[j].node2].b += (b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistN[j].dS;
					}
				}


				for (integer j = 0; j < b[i].radiation.nodelistBsize; j++) {
					//sl[b[i].radiation.nodelistB[j].node1].b += b[i].radiation.JB*b[i].radiation.nodelistB[j].dS;
					if (b[i].radiation.nodelistB[j].node2 >= maxelm) {
						// ���������.
						// �������� ����� !!!
						//slb[b[i].radiation.nodelistB[j].node2 - maxelm].b += -b[i].radiation.JB*b[i].radiation.nodelistB[j].dS;
					}
					else {
						// ����������.
						// �������� ����� !!!
						// ������� (������ ����� � ����������� �Y), �� �� �� � ������ �� ��� ����� �� ���� TOP.
						result = sl[b[i].radiation.nodelistB[j].node2].b  -(b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistB[j].dS;
						sl[b[i].radiation.nodelistB[j].node2].b = alpha*(result)+(1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistB[j].node2];
						bsource_term_radiation_for_relax[b[i].radiation.nodelistB[j].node2] = sl[b[i].radiation.nodelistB[j].node2].b;
						//sl[b[i].radiation.nodelistB[j].node2].b += -(b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistB[j].dS;
					}
				}


				for (integer j = 0; j < b[i].radiation.nodelistTsize; j++) {
					//sl[b[i].radiation.nodelistT[j].node1].b += b[i].radiation.JT*b[i].radiation.nodelistT[j].dS;
					if (b[i].radiation.nodelistT[j].node2 >= maxelm) {
						// ���������.
						// �������� ����� !!!
						//slb[b[i].radiation.nodelistT[j].node2 - maxelm].b += -b[i].radiation.JT*b[i].radiation.nodelistT[j].dS;
					}
					else {
						// ����������.
						// �������� ���� �� ������� Bottom, �.�. ����� top ����� �� ��� ����� � �������� ��������� �� ��� ����� bottom.
						// ����������� ������ ������ �� ���������� - ������� ���������� botomom ��������� ����������� topom.
						result = sl[b[i].radiation.nodelistT[j].node2].b + (b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistT[j].dS;
						sl[b[i].radiation.nodelistT[j].node2].b = alpha*(result)+(1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistT[j].node2];
						bsource_term_radiation_for_relax[b[i].radiation.nodelistT[j].node2] = sl[b[i].radiation.nodelistT[j].node2].b;
						//sl[b[i].radiation.nodelistT[j].node2].b += (b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistT[j].dS;
					}
				}

			}
		}
	}
} // radiosity_patch_for_vacuum_Prism_Object
*/


// ����������� ������� ���� ��� ����� ������� radiosity Prism Object.
//void radiosity_patch_for_vacuum_Prism_Object(equation3D* &sl, equation3D_bon* &slb, BLOCK* &b, integer lb, integer maxelm)
// 26.09.2016 �������� � ��� ���� ����� ����.
void radiosity_patch_for_vacuum_Prism_Object_(doublereal* &rthdsd, BLOCK* &b, integer lb, integer maxelm)
{

	
	for (integer i = 0; i < maxelm; i++) {
		// �������������.
		b_buffer_correct_source[i] = 0.0;
	}
	
	for (integer i = 0; i < lb; i++) {
		if (b[i].radiation.binternalRadiation)
		{
			if ((b[i].radiation.nodelistW != NULL) && (b[i].radiation.nodelistE != NULL) && (b[i].radiation.nodelistS != NULL) && (b[i].radiation.nodelistN != NULL) && (b[i].radiation.nodelistB != NULL) && (b[i].radiation.nodelistT != NULL))
			{

				for (integer j = 0; j < b[i].radiation.nodelistWsize; j++) {
					//sl[b[i].radiation.nodelistW[j].node1].b += b[i].radiation.JW*b[i].radiation.nodelistW[j].dS;

					if (b[i].radiation.nodelistW[j].node21>-1) {
						if (b[i].radiation.nodelistW[j].node21 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistW[j].node21 - maxelm].b += -b[i].radiation.JW*b[i].radiation.nodelistW[j].dS;
						}
						else {
							// ����������.
							// �������� ����� !!!
							//result = sl[b[i].radiation.nodelistW[j].node21].b - (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistW[j].dS;
							//sl[b[i].radiation.nodelistW[j].node21].b = alpha*(result)+(1 - alpha)*(bsource_term_radiation_for_relax[b[i].radiation.nodelistW[j].node21]);
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistW[j].node21] = sl[b[i].radiation.nodelistW[j].node21].b;
							//sl[b[i].radiation.nodelistW[j].node21].b += -(b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistW[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistW[j].node21] += -(b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistW[j].dS1;
						}
					}

					if (b[i].radiation.nodelistW[j].node22>-1) {
						if (b[i].radiation.nodelistW[j].node22 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistW[j].node21 - maxelm].b += -b[i].radiation.JW*b[i].radiation.nodelistW[j].dS;
						}
						else {
							// ����������.
							// �������� ����� !!!
							//result = sl[b[i].radiation.nodelistW[j].node21].b - (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistW[j].dS;
							//sl[b[i].radiation.nodelistW[j].node21].b = alpha*(result)+(1 - alpha)*(bsource_term_radiation_for_relax[b[i].radiation.nodelistW[j].node21]);
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistW[j].node21] = sl[b[i].radiation.nodelistW[j].node21].b;
							//sl[b[i].radiation.nodelistW[j].node21].b += -(b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistW[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistW[j].node22] += -(b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistW[j].dS2;
						}
					}

					if (b[i].radiation.nodelistW[j].node23>-1) {
						if (b[i].radiation.nodelistW[j].node23 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistW[j].node21 - maxelm].b += -b[i].radiation.JW*b[i].radiation.nodelistW[j].dS;
						}
						else {
							// ����������.
							// �������� ����� !!!
							//result = sl[b[i].radiation.nodelistW[j].node21].b - (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistW[j].dS;
							//sl[b[i].radiation.nodelistW[j].node21].b = alpha*(result)+(1 - alpha)*(bsource_term_radiation_for_relax[b[i].radiation.nodelistW[j].node21]);
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistW[j].node21] = sl[b[i].radiation.nodelistW[j].node21].b;
							//sl[b[i].radiation.nodelistW[j].node21].b += -(b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistW[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistW[j].node23] += -(b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistW[j].dS3;
						}
					}

					if (b[i].radiation.nodelistW[j].node24>-1) {
						if (b[i].radiation.nodelistW[j].node24 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistW[j].node21 - maxelm].b += -b[i].radiation.JW*b[i].radiation.nodelistW[j].dS;
						}
						else {
							// ����������.
							// �������� ����� !!!
							//result = sl[b[i].radiation.nodelistW[j].node21].b - (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistW[j].dS;
							//sl[b[i].radiation.nodelistW[j].node21].b = alpha*(result)+(1 - alpha)*(bsource_term_radiation_for_relax[b[i].radiation.nodelistW[j].node21]);
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistW[j].node21] = sl[b[i].radiation.nodelistW[j].node21].b;
							//sl[b[i].radiation.nodelistW[j].node21].b += -(b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistW[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistW[j].node24] += -(b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistW[j].dS4;
						}
					}

				}


				for (integer j = 0; j < b[i].radiation.nodelistEsize; j++) {
					//sl[b[i].radiation.nodelistE[j].node1].b += b[i].radiation.JE*b[i].radiation.nodelistE[j].dS;
					if (b[i].radiation.nodelistE[j].node21>-1) {
						if (b[i].radiation.nodelistE[j].node21 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistE[j].node21 - maxelm].b += -b[i].radiation.JE*b[i].radiation.nodelistE[j].dS;
						}
						else {
							// ����������.
							// �������� ���� �� ������� West, �.�. ����� East ����� �� ��� ����� � �������� ��������� �� ��� ����� West.
							// ����������� ������ ������ �� ���������� - ������� ���������� West ��������� ����������� Estom.
							//result = sl[b[i].radiation.nodelistE[j].node21].b + (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistE[j].dS;
							//sl[b[i].radiation.nodelistE[j].node21].b = alpha*result + (1.0 - alpha)*(bsource_term_radiation_for_relax[b[i].radiation.nodelistE[j].node21]);
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistE[j].node21] = sl[b[i].radiation.nodelistE[j].node21].b;
							//sl[b[i].radiation.nodelistE[j].node21].b += (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistE[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistE[j].node21] += (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistE[j].dS1;
						}
					}
					if (b[i].radiation.nodelistE[j].node22>-1) {
						if (b[i].radiation.nodelistE[j].node22 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistE[j].node21 - maxelm].b += -b[i].radiation.JE*b[i].radiation.nodelistE[j].dS;
						}
						else {
							// ����������.
							// �������� ���� �� ������� West, �.�. ����� East ����� �� ��� ����� � �������� ��������� �� ��� ����� West.
							// ����������� ������ ������ �� ���������� - ������� ���������� West ��������� ����������� Estom.
							//result = sl[b[i].radiation.nodelistE[j].node21].b + (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistE[j].dS;
							//sl[b[i].radiation.nodelistE[j].node21].b = alpha*result + (1.0 - alpha)*(bsource_term_radiation_for_relax[b[i].radiation.nodelistE[j].node21]);
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistE[j].node21] = sl[b[i].radiation.nodelistE[j].node21].b;
							//sl[b[i].radiation.nodelistE[j].node21].b += (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistE[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistE[j].node22] += (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistE[j].dS2;
						}
					}
					if (b[i].radiation.nodelistE[j].node23>-1) {
						if (b[i].radiation.nodelistE[j].node23 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistE[j].node21 - maxelm].b += -b[i].radiation.JE*b[i].radiation.nodelistE[j].dS;
						}
						else {
							// ����������.
							// �������� ���� �� ������� West, �.�. ����� East ����� �� ��� ����� � �������� ��������� �� ��� ����� West.
							// ����������� ������ ������ �� ���������� - ������� ���������� West ��������� ����������� Estom.
							//result = sl[b[i].radiation.nodelistE[j].node21].b + (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistE[j].dS;
							//sl[b[i].radiation.nodelistE[j].node21].b = alpha*result + (1.0 - alpha)*(bsource_term_radiation_for_relax[b[i].radiation.nodelistE[j].node21]);
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistE[j].node21] = sl[b[i].radiation.nodelistE[j].node21].b;
							//sl[b[i].radiation.nodelistE[j].node21].b += (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistE[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistE[j].node23] += (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistE[j].dS3;
						}
					}
					if (b[i].radiation.nodelistE[j].node24>-1) {
						if (b[i].radiation.nodelistE[j].node24 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistE[j].node21 - maxelm].b += -b[i].radiation.JE*b[i].radiation.nodelistE[j].dS;
						}
						else {
							// ����������.
							// �������� ���� �� ������� West, �.�. ����� East ����� �� ��� ����� � �������� ��������� �� ��� ����� West.
							// ����������� ������ ������ �� ���������� - ������� ���������� West ��������� ����������� Estom.
							//result = sl[b[i].radiation.nodelistE[j].node21].b + (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistE[j].dS;
							//sl[b[i].radiation.nodelistE[j].node21].b = alpha*result + (1.0 - alpha)*(bsource_term_radiation_for_relax[b[i].radiation.nodelistE[j].node21]);
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistE[j].node21] = sl[b[i].radiation.nodelistE[j].node21].b;
							//sl[b[i].radiation.nodelistE[j].node21].b += (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistE[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistE[j].node24] += (b[i].radiation.JW - b[i].radiation.JE)*b[i].radiation.nodelistE[j].dS4;
						}
					}

				}

				for (integer j = 0; j < b[i].radiation.nodelistSsize; j++) {
					//sl[b[i].radiation.nodelistS[j].node1].b += b[i].radiation.JS*b[i].radiation.nodelistS[j].dS;
					if (b[i].radiation.nodelistS[j].node21>-1) {
						if (b[i].radiation.nodelistS[j].node21 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistS[j].node21 - maxelm].b += -b[i].radiation.JS*b[i].radiation.nodelistS[j].dS;
						}
						else {
							// ����������.
							// �������� ����� !!!
							//result = sl[b[i].radiation.nodelistS[j].node21].b - (b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistS[j].dS;
							//sl[b[i].radiation.nodelistS[j].node21].b = alpha*(result)+(1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistS[j].node21];
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistS[j].node21] = sl[b[i].radiation.nodelistS[j].node21].b;
							//sl[b[i].radiation.nodelistS[j].node21].b += -(b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistS[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistS[j].node21] += -(b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistS[j].dS1;
						}
					}
					if (b[i].radiation.nodelistS[j].node22>-1) {
						if (b[i].radiation.nodelistS[j].node22 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistS[j].node21 - maxelm].b += -b[i].radiation.JS*b[i].radiation.nodelistS[j].dS;
						}
						else {
							// ����������.
							// �������� ����� !!!
							//result = sl[b[i].radiation.nodelistS[j].node21].b - (b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistS[j].dS;
							//sl[b[i].radiation.nodelistS[j].node21].b = alpha*(result)+(1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistS[j].node21];
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistS[j].node21] = sl[b[i].radiation.nodelistS[j].node21].b;
							//sl[b[i].radiation.nodelistS[j].node21].b += -(b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistS[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistS[j].node22] += -(b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistS[j].dS2;
						}
					}
					if (b[i].radiation.nodelistS[j].node23>-1) {
						if (b[i].radiation.nodelistS[j].node23 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistS[j].node21 - maxelm].b += -b[i].radiation.JS*b[i].radiation.nodelistS[j].dS;
						}
						else {
							// ����������.
							// �������� ����� !!!
							//result = sl[b[i].radiation.nodelistS[j].node21].b - (b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistS[j].dS;
							//sl[b[i].radiation.nodelistS[j].node21].b = alpha*(result)+(1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistS[j].node21];
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistS[j].node21] = sl[b[i].radiation.nodelistS[j].node21].b;
							//sl[b[i].radiation.nodelistS[j].node21].b += -(b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistS[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistS[j].node23] += -(b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistS[j].dS3;
						}
					}
					if (b[i].radiation.nodelistS[j].node24>-1) {
						if (b[i].radiation.nodelistS[j].node24 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistS[j].node21 - maxelm].b += -b[i].radiation.JS*b[i].radiation.nodelistS[j].dS;
						}
						else {
							// ����������.
							// �������� ����� !!!
							//result = sl[b[i].radiation.nodelistS[j].node21].b - (b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistS[j].dS;
							//sl[b[i].radiation.nodelistS[j].node21].b = alpha*(result)+(1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistS[j].node21];
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistS[j].node21] = sl[b[i].radiation.nodelistS[j].node21].b;
							//sl[b[i].radiation.nodelistS[j].node21].b += -(b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistS[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistS[j].node24] += -(b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistS[j].dS4;
						}
					}
				}


				for (integer j = 0; j < b[i].radiation.nodelistNsize; j++) {
					//sl[b[i].radiation.nodelistN[j].node1].b += b[i].radiation.JN*b[i].radiation.nodelistN[j].dS;

					if (b[i].radiation.nodelistN[j].node21>-1) {
						if (b[i].radiation.nodelistN[j].node21 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistN[j].node21 - maxelm].b += -b[i].radiation.JN*b[i].radiation.nodelistN[j].dS;
						}
						else {
							// ����������.
							//result = sl[b[i].radiation.nodelistN[j].node21].b + (b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistN[j].dS;
							//sl[b[i].radiation.nodelistN[j].node21].b = alpha*result + (1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistN[j].node21];
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistN[j].node21] = sl[b[i].radiation.nodelistN[j].node21].b;
							//sl[b[i].radiation.nodelistN[j].node21].b += (b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistN[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistN[j].node21] += (b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistN[j].dS1;
						}
					}
					if (b[i].radiation.nodelistN[j].node22>-1) {
						if (b[i].radiation.nodelistN[j].node22 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistN[j].node21 - maxelm].b += -b[i].radiation.JN*b[i].radiation.nodelistN[j].dS;
						}
						else {
							// ����������.
							//result = sl[b[i].radiation.nodelistN[j].node21].b + (b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistN[j].dS;
							//sl[b[i].radiation.nodelistN[j].node21].b = alpha*result + (1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistN[j].node21];
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistN[j].node21] = sl[b[i].radiation.nodelistN[j].node21].b;
							//sl[b[i].radiation.nodelistN[j].node21].b += (b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistN[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistN[j].node22] += (b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistN[j].dS2;
						}
					}
					if (b[i].radiation.nodelistN[j].node23>-1) {
						if (b[i].radiation.nodelistN[j].node23 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistN[j].node21 - maxelm].b += -b[i].radiation.JN*b[i].radiation.nodelistN[j].dS;
						}
						else {
							// ����������.
							//result = sl[b[i].radiation.nodelistN[j].node21].b + (b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistN[j].dS;
							//sl[b[i].radiation.nodelistN[j].node21].b = alpha*result + (1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistN[j].node21];
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistN[j].node21] = sl[b[i].radiation.nodelistN[j].node21].b;
							//sl[b[i].radiation.nodelistN[j].node21].b += (b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistN[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistN[j].node23] += (b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistN[j].dS3;
						}
					}
					if (b[i].radiation.nodelistN[j].node24>-1) {
						if (b[i].radiation.nodelistN[j].node24 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistN[j].node21 - maxelm].b += -b[i].radiation.JN*b[i].radiation.nodelistN[j].dS;
						}
						else {
							// ����������.
							//result = sl[b[i].radiation.nodelistN[j].node21].b + (b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistN[j].dS;
							//sl[b[i].radiation.nodelistN[j].node21].b = alpha*result + (1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistN[j].node21];
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistN[j].node21] = sl[b[i].radiation.nodelistN[j].node21].b;
							//sl[b[i].radiation.nodelistN[j].node21].b += (b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistN[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistN[j].node24] += (b[i].radiation.JS - b[i].radiation.JN)*b[i].radiation.nodelistN[j].dS4;
						}
					}

				}


				for (integer j = 0; j < b[i].radiation.nodelistBsize; j++) {
					//sl[b[i].radiation.nodelistB[j].node1].b += b[i].radiation.JB*b[i].radiation.nodelistB[j].dS;
					if (b[i].radiation.nodelistB[j].node21>-1) {
						if (b[i].radiation.nodelistB[j].node21 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistB[j].node21 - maxelm].b += -b[i].radiation.JB*b[i].radiation.nodelistB[j].dS;
						}
						else {
							// ����������.
							// �������� ����� !!!
							// ������� (������ ����� � ����������� �Y), �� �� �� � ������ �� ��� ����� �� ���� TOP.
							//result = sl[b[i].radiation.nodelistB[j].node21].b - (b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistB[j].dS;
							//sl[b[i].radiation.nodelistB[j].node21].b = alpha*(result)+(1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistB[j].node21];
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistB[j].node21] = sl[b[i].radiation.nodelistB[j].node21].b;
							//sl[b[i].radiation.nodelistB[j].node21].b += -(b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistB[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistB[j].node21] += -(b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistB[j].dS1;
						}
					}
					if (b[i].radiation.nodelistB[j].node22>-1) {
						if (b[i].radiation.nodelistB[j].node22 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistB[j].node21 - maxelm].b += -b[i].radiation.JB*b[i].radiation.nodelistB[j].dS;
						}
						else {
							// ����������.
							// �������� ����� !!!
							// ������� (������ ����� � ����������� �Y), �� �� �� � ������ �� ��� ����� �� ���� TOP.
							//result = sl[b[i].radiation.nodelistB[j].node21].b - (b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistB[j].dS;
							//sl[b[i].radiation.nodelistB[j].node21].b = alpha*(result)+(1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistB[j].node21];
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistB[j].node21] = sl[b[i].radiation.nodelistB[j].node21].b;
							//sl[b[i].radiation.nodelistB[j].node21].b += -(b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistB[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistB[j].node22] += -(b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistB[j].dS2;
						}
					}
					if (b[i].radiation.nodelistB[j].node23>-1) {
						if (b[i].radiation.nodelistB[j].node23 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistB[j].node21 - maxelm].b += -b[i].radiation.JB*b[i].radiation.nodelistB[j].dS;
						}
						else {
							// ����������.
							// �������� ����� !!!
							// ������� (������ ����� � ����������� �Y), �� �� �� � ������ �� ��� ����� �� ���� TOP.
							//result = sl[b[i].radiation.nodelistB[j].node21].b - (b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistB[j].dS;
							//sl[b[i].radiation.nodelistB[j].node21].b = alpha*(result)+(1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistB[j].node21];
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistB[j].node21] = sl[b[i].radiation.nodelistB[j].node21].b;
							//sl[b[i].radiation.nodelistB[j].node21].b += -(b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistB[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistB[j].node23] += -(b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistB[j].dS3;
						}
					}
					if (b[i].radiation.nodelistB[j].node24>-1) {
						if (b[i].radiation.nodelistB[j].node24 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistB[j].node21 - maxelm].b += -b[i].radiation.JB*b[i].radiation.nodelistB[j].dS;
						}
						else {
							// ����������.
							// �������� ����� !!!
							// ������� (������ ����� � ����������� �Y), �� �� �� � ������ �� ��� ����� �� ���� TOP.
							//result = sl[b[i].radiation.nodelistB[j].node21].b - (b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistB[j].dS;
							//sl[b[i].radiation.nodelistB[j].node21].b = alpha*(result)+(1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistB[j].node21];
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistB[j].node21] = sl[b[i].radiation.nodelistB[j].node21].b;
							//sl[b[i].radiation.nodelistB[j].node21].b += -(b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistB[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistB[j].node24] += -(b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistB[j].dS4;
						}
					}
				}


				for (integer j = 0; j < b[i].radiation.nodelistTsize; j++) {
					//sl[b[i].radiation.nodelistT[j].node1].b += b[i].radiation.JT*b[i].radiation.nodelistT[j].dS;
					if (b[i].radiation.nodelistT[j].node21>-1) {
						if (b[i].radiation.nodelistT[j].node21 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistT[j].node21 - maxelm].b += -b[i].radiation.JT*b[i].radiation.nodelistT[j].dS;
						}
						else {
							// ����������.
							// �������� ���� �� ������� Bottom, �.�. ����� top ����� �� ��� ����� � �������� ��������� �� ��� ����� bottom.
							// ����������� ������ ������ �� ���������� - ������� ���������� botomom ��������� ����������� topom.
							//result = sl[b[i].radiation.nodelistT[j].node21].b + (b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistT[j].dS;
							//sl[b[i].radiation.nodelistT[j].node21].b = alpha*(result)+(1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistT[j].node21];
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistT[j].node21] = sl[b[i].radiation.nodelistT[j].node21].b;
							//sl[b[i].radiation.nodelistT[j].node21].b += (b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistT[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistT[j].node21] += (b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistT[j].dS1;
						}
					}
					if (b[i].radiation.nodelistT[j].node22>-1) {
						if (b[i].radiation.nodelistT[j].node22 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistT[j].node21 - maxelm].b += -b[i].radiation.JT*b[i].radiation.nodelistT[j].dS;
						}
						else {
							// ����������.
							// �������� ���� �� ������� Bottom, �.�. ����� top ����� �� ��� ����� � �������� ��������� �� ��� ����� bottom.
							// ����������� ������ ������ �� ���������� - ������� ���������� botomom ��������� ����������� topom.
							//result = sl[b[i].radiation.nodelistT[j].node21].b + (b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistT[j].dS;
							//sl[b[i].radiation.nodelistT[j].node21].b = alpha*(result)+(1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistT[j].node21];
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistT[j].node21] = sl[b[i].radiation.nodelistT[j].node21].b;
							//sl[b[i].radiation.nodelistT[j].node21].b += (b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistT[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistT[j].node22] += (b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistT[j].dS2;
						}
					}
					if (b[i].radiation.nodelistT[j].node23>-1) {
						if (b[i].radiation.nodelistT[j].node23 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistT[j].node21 - maxelm].b += -b[i].radiation.JT*b[i].radiation.nodelistT[j].dS;
						}
						else {
							// ����������.
							// �������� ���� �� ������� Bottom, �.�. ����� top ����� �� ��� ����� � �������� ��������� �� ��� ����� bottom.
							// ����������� ������ ������ �� ���������� - ������� ���������� botomom ��������� ����������� topom.
							//result = sl[b[i].radiation.nodelistT[j].node21].b + (b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistT[j].dS;
							//sl[b[i].radiation.nodelistT[j].node21].b = alpha*(result)+(1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistT[j].node21];
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistT[j].node21] = sl[b[i].radiation.nodelistT[j].node21].b;
							//sl[b[i].radiation.nodelistT[j].node21].b += (b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistT[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistT[j].node23] += (b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistT[j].dS3;
						}
					}
					if (b[i].radiation.nodelistT[j].node24>-1) {
						if (b[i].radiation.nodelistT[j].node24 >= maxelm) {
							// ���������.
							// �������� ����� !!!
							//slb[b[i].radiation.nodelistT[j].node21 - maxelm].b += -b[i].radiation.JT*b[i].radiation.nodelistT[j].dS;
						}
						else {
							// ����������.
							// �������� ���� �� ������� Bottom, �.�. ����� top ����� �� ��� ����� � �������� ��������� �� ��� ����� bottom.
							// ����������� ������ ������ �� ���������� - ������� ���������� botomom ��������� ����������� topom.
							//result = sl[b[i].radiation.nodelistT[j].node21].b + (b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistT[j].dS;
							//sl[b[i].radiation.nodelistT[j].node21].b = alpha*(result)+(1.0 - alpha)*bsource_term_radiation_for_relax[b[i].radiation.nodelistT[j].node21];
							//bsource_term_radiation_for_relax[b[i].radiation.nodelistT[j].node21] = sl[b[i].radiation.nodelistT[j].node21].b;
							//sl[b[i].radiation.nodelistT[j].node21].b += (b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistT[j].dS;
							b_buffer_correct_source[b[i].radiation.nodelistT[j].node24] += (b[i].radiation.JB - b[i].radiation.JT)*b[i].radiation.nodelistT[j].dS4;
						}
					}

				}

			}
		}
	}
	
	doublereal result34 = 0.0;  
	doublereal alpha34 = 0.00625; // 0.00625 0.1 0.02
	//alpha34 = my_amg_manager.theta_Speed;

	for (integer i = 0; i < maxelm; i++) {
		if (fabs(b_buffer_correct_source[i]) > 1.0e-20) {
			// ��� ����������:
			//sl[i].b += b_buffer_correct_source[i];
			rthdsd[i] += b_buffer_correct_source[i];
			bsource_term_radiation_for_relax[i] = b_buffer_correct_source[i];

			// ������������ 23.11.2016
			// � ������ �����������: 
			//result34 = b_buffer_correct_source[i];
			//doublereal badditional = alpha34*(result34)+(1.0 - alpha34)*(bsource_term_radiation_for_relax[i]);
			//rthdsd[i]+=badditional;
			//bsource_term_radiation_for_relax[i] = badditional;
			
			/*
			// ��������������� 23_11_2016.
			if (fabs(bsource_term_radiation_for_relax[i]) > 1.0e-20) {
				// � ������ �����������: 
				result34 = b_buffer_correct_source[i];
				if (b_buffer_correct_source[i] * bsource_term_radiation_for_relax[i] > 0.0) {
					//doublereal badditional = alpha34*(result34)+(1.0 - alpha34)*(bsource_term_radiation_for_relax[i]);
					rthdsd[i] += b_buffer_correct_source[i];
					bsource_term_radiation_for_relax[i] = b_buffer_correct_source[i];
				}
				else {
					result34 = 0.0; // ��������������� �����. ���������������� ��������� ������������.
					//doublereal badditional = alpha34*(result34)+(1.0 - alpha34)*(bsource_term_radiation_for_relax[i]);
					rthdsd[i] += 0.0;
					//bsource_term_radiation_for_relax[i] = badditional;
				}
			}
			else {
				// � ������ �����������: 
				result34 = b_buffer_correct_source[i];
				//doublereal badditional = alpha34*(result34)+(1.0 - alpha34)*(bsource_term_radiation_for_relax[i]);
				rthdsd[i] += b_buffer_correct_source[i];
				bsource_term_radiation_for_relax[i] = b_buffer_correct_source[i];
			}
			*/
		}
	}


	
} // radiosity_patch_for_vacuum_Prism_Object

  // �������������� ������ ������� Ƹ������� ��� ������������ ������. 4.08.2017.
  // 16.09.2017.
void Thermal_Structural_assemble(integer iP, integer** nvtx,
	TOCHKA* pa, doublereal** prop, doublereal** &Kmatrix)
{

	//nvtx
	// ---|+--|-+-|++-|--+|+-+|-++|+++
	// 1	2	3	4	5	6	7	8
	// ������� ������������ ������� ����� � ������ ������.
	 


	doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
	volume3D(iP, nvtx, pa, hx, hy, hz);
	//printf("%e %e %e\n",hx,hy,hz);
	for (integer i_1 = 0; i_1 < 8; i_1++) {
		//printf("x=%e y=%e z=%e\n", pa[nvtx[i_1][iP] - 1].x, pa[nvtx[i_1][iP] - 1].y, pa[nvtx[i_1][iP] - 1].z);
	}
	//getchar();

	doublereal mu, lambda; // ������������ ����.

	mu = prop[MU_LAME][iP];
	lambda = prop[LAMBDA_LAME][iP];

	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;

	// ������ ��������� �������.
	// ������ ������� 24*24 ������������� �������� ���������� ���������� maple18 �� �������� �������� ���.
	Kmatrix[0][0] = 0.8888888888 / (hx*hx)*(lambda + 2.0*mu) + 0.8888888891 / (hy*hy)
		*mu + 0.888888889 / (hz*hz)*mu;
	Kmatrix[0][1] = 0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[0][2] = 0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[0][3] = -0.8888888888 / (hx*hx)*(lambda + 2.0*mu) + 0.4444444445 / (hy*hy
		)*mu + 0.4444444444 / (hz*hz)*mu;
	Kmatrix[0][4] = 0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[0][5] = 0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[0][6] = 0.4444444445 / (hx*hx)*(lambda + 2.0*mu) - 0.8888888891 / (hy*hy)
		*mu + 0.4444444446 / (hz*hz)*mu;
	Kmatrix[0][7] = -0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[0][8] = 0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[0][9] = -0.4444444445 / (hx*hx)*(lambda + 2.0*mu) - 0.4444444445 / (hy*hy
		)*mu + 0.2222222222 / (hz*hz)*mu;
	Kmatrix[0][10] = -0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[0][11] = 0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[0][12] = 0.4444444444 / (hx*hx)*(lambda + 2.0*mu) + 0.4444444445 / (hy*hy
		)*mu - 0.888888889 / (hz*hz)*mu;
	Kmatrix[0][13] = 0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[0][14] = -0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[0][15] = -0.4444444444 / (hx*hx)*(lambda + 2.0*mu) + 0.2222222223 / (hy*
		hy)*mu - 0.4444444444 / (hz*hz)*mu;
	Kmatrix[0][16] = 0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[0][17] = -0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[0][18] = 0.2222222223 / (hx*hx)*(lambda + 2.0*mu) - 0.4444444445 / (hy*hy
		)*mu - 0.4444444446 / (hz*hz)*mu;
	Kmatrix[0][19] = -0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[0][20] = -0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[0][21] = -0.2222222223 / (hx*hx)*(lambda + 2.0*mu) - 0.2222222223 / (hy*
		hy)*mu - 0.2222222222 / (hz*hz)*mu;
	Kmatrix[0][22] = -0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[0][23] = -0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[1][0] = 0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[1][1] = 0.8888888888 / (hx*hx)*mu + 0.8888888891 / (hy*hy)*(lambda + 2.0*
		mu) + 0.888888889 / (hz*hz)*mu;
	Kmatrix[1][2] = 0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[1][3] = -0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[1][4] = -0.8888888888 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*(lambda + 2.0
		*mu) + 0.4444444444 / (hz*hz)*mu;
	Kmatrix[1][5] = 0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[1][6] = 0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[1][7] = 0.4444444445 / (hx*hx)*mu - 0.8888888891 / (hy*hy)*(lambda + 2.0*
		mu) + 0.4444444446 / (hz*hz)*mu;
	Kmatrix[1][8] = 0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[1][9] = -0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[1][10] = -0.4444444445 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*(lambda +
		2.0*mu) + 0.2222222222 / (hz*hz)*mu;
	Kmatrix[1][11] = 0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[1][12] = 0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[1][13] = 0.4444444444 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*(lambda + 2.0
		*mu) - 0.888888889 / (hz*hz)*mu;
	Kmatrix[1][14] = -0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[1][15] = -0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[1][16] = -0.4444444444 / (hx*hx)*mu + 0.2222222223 / (hy*hy)*(lambda +
		2.0*mu) - 0.4444444444 / (hz*hz)*mu;
	Kmatrix[1][17] = -0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[1][18] = 0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[1][19] = 0.2222222223 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*(lambda + 2.0
		*mu) - 0.4444444446 / (hz*hz)*mu;
	Kmatrix[1][20] = -0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[1][21] = -0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[1][22] = -0.2222222223 / (hx*hx)*mu - 0.2222222223 / (hy*hy)*(lambda +
		2.0*mu) - 0.2222222222 / (hz*hz)*mu;
	Kmatrix[1][23] = -0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[2][0] = 0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[2][1] = 0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[2][2] = 0.8888888888 / (hx*hx)*mu + 0.8888888891 / (hy*hy)*mu +
		0.888888889 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[2][3] = -0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[2][4] = 0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[2][5] = -0.8888888888 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*mu +
		0.4444444444 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[2][6] = 0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[2][7] = -0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[2][8] = 0.4444444445 / (hx*hx)*mu - 0.8888888891 / (hy*hy)*mu +
		0.4444444446 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[2][9] = -0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[2][10] = -0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[2][11] = -0.4444444445 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*mu +
		0.2222222222 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[2][12] = 0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[2][13] = 0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[2][14] = 0.4444444444 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*mu
		- 0.888888889 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[2][15] = -0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[2][16] = 0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[2][17] = -0.4444444444 / (hx*hx)*mu + 0.2222222223 / (hy*hy)*mu
		- 0.4444444444 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[2][18] = 0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[2][19] = -0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[2][20] = 0.2222222223 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*mu
		- 0.4444444446 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[2][21] = -0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[2][22] = -0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[2][23] = -0.2222222223 / (hx*hx)*mu - 0.2222222223 / (hy*hy)*mu
		- 0.2222222222 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[3][0] = -0.8888888888 / (hx*hx)*(lambda + 2.0*mu) + 0.4444444445 / (hy*hy
		)*mu + 0.4444444444 / (hz*hz)*mu;
	Kmatrix[3][1] = -0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[3][2] = -0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[3][3] = 0.8888888888 / (hx*hx)*(lambda + 2.0*mu) + 0.8888888891 / (hy*hy)
		*mu + 0.888888889 / (hz*hz)*mu;
	Kmatrix[3][4] = -0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[3][5] = -0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[3][6] = -0.4444444445 / (hx*hx)*(lambda + 2.0*mu) - 0.4444444445 / (hy*hy
		)*mu + 0.2222222222 / (hz*hz)*mu;
	Kmatrix[3][7] = 0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[3][8] = -0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[3][9] = 0.4444444445 / (hx*hx)*(lambda + 2.0*mu) - 0.8888888891 / (hy*hy)
		*mu + 0.4444444446 / (hz*hz)*mu;
	Kmatrix[3][10] = 0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[3][11] = -0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[3][12] = -0.4444444444 / (hx*hx)*(lambda + 2.0*mu) + 0.2222222223 / (hy*
		hy)*mu - 0.4444444444 / (hz*hz)*mu;
	Kmatrix[3][13] = -0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[3][14] = 0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[3][15] = 0.4444444444 / (hx*hx)*(lambda + 2.0*mu) + 0.4444444445 / (hy*hy
		)*mu - 0.888888889 / (hz*hz)*mu;
	Kmatrix[3][16] = -0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[3][17] = 0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[3][18] = -0.2222222223 / (hx*hx)*(lambda + 2.0*mu) - 0.2222222223 / (hy*
		hy)*mu - 0.2222222222 / (hz*hz)*mu;
	Kmatrix[3][19] = 0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[3][20] = 0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[3][21] = 0.2222222223 / (hx*hx)*(lambda + 2.0*mu) - 0.4444444445 / (hy*hy
		)*mu - 0.4444444446 / (hz*hz)*mu;
	Kmatrix[3][22] = 0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[3][23] = 0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[4][0] = 0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[4][1] = -0.8888888888 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*(lambda + 2.0
		*mu) + 0.4444444444 / (hz*hz)*mu;
	Kmatrix[4][2] = 0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[4][3] = -0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[4][4] = 0.8888888888 / (hx*hx)*mu + 0.8888888891 / (hy*hy)*(lambda + 2.0*
		mu) + 0.888888889 / (hz*hz)*mu;
	Kmatrix[4][5] = 0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[4][6] = 0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[4][7] = -0.4444444445 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*(lambda + 2.0
		*mu) + 0.2222222222 / (hz*hz)*mu;
	Kmatrix[4][8] = 0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[4][9] = -0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[4][10] = 0.4444444445 / (hx*hx)*mu - 0.8888888891 / (hy*hy)*(lambda + 2.0
		*mu) + 0.4444444446 / (hz*hz)*mu;
	Kmatrix[4][11] = 0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[4][12] = 0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[4][13] = -0.4444444444 / (hx*hx)*mu + 0.2222222223 / (hy*hy)*(lambda +
		2.0*mu) - 0.4444444444 / (hz*hz)*mu;
	Kmatrix[4][14] = -0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[4][15] = -0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[4][16] = 0.4444444444 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*(lambda + 2.0
		*mu) - 0.888888889 / (hz*hz)*mu;
	Kmatrix[4][17] = -0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[4][18] = 0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[4][19] = -0.2222222223 / (hx*hx)*mu - 0.2222222223 / (hy*hy)*(lambda +
		2.0*mu) - 0.2222222222 / (hz*hz)*mu;
	Kmatrix[4][20] = -0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[4][21] = -0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[4][22] = 0.2222222223 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*(lambda + 2.0
		*mu) - 0.4444444446 / (hz*hz)*mu;
	Kmatrix[4][23] = -0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[5][0] = 0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[5][1] = 0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[5][2] = -0.8888888888 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*mu +
		0.4444444444 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[5][3] = -0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[5][4] = 0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[5][5] = 0.8888888888 / (hx*hx)*mu + 0.8888888891 / (hy*hy)*mu +
		0.888888889 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[5][6] = 0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[5][7] = -0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[5][8] = -0.4444444445 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*mu +
		0.2222222222 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[5][9] = -0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[5][10] = -0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[5][11] = 0.4444444445 / (hx*hx)*mu - 0.8888888891 / (hy*hy)*mu +
		0.4444444446 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[5][12] = 0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[5][13] = 0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[5][14] = -0.4444444444 / (hx*hx)*mu + 0.2222222223 / (hy*hy)*mu
		- 0.4444444444 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[5][15] = -0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[5][16] = 0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[5][17] = 0.4444444444 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*mu
		- 0.888888889 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[5][18] = 0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[5][19] = -0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[5][20] = -0.2222222223 / (hx*hx)*mu - 0.2222222223 / (hy*hy)*mu
		- 0.2222222222 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[5][21] = -0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[5][22] = -0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[5][23] = 0.2222222223 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*mu
		- 0.4444444446 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[6][0] = 0.4444444445 / (hx*hx)*(lambda + 2.0*mu) - 0.8888888891 / (hy*hy)
		*mu + 0.4444444446 / (hz*hz)*mu;
	Kmatrix[6][1] = 0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[6][2] = 0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[6][3] = -0.4444444445 / (hx*hx)*(lambda + 2.0*mu) - 0.4444444445 / (hy*hy
		)*mu + 0.2222222222 / (hz*hz)*mu;
	Kmatrix[6][4] = 0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[6][5] = 0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[6][6] = 0.8888888888 / (hx*hx)*(lambda + 2.0*mu) + 0.8888888891 / (hy*hy)
		*mu + 0.888888889 / (hz*hz)*mu;
	Kmatrix[6][7] = -0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[6][8] = 0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[6][9] = -0.8888888888 / (hx*hx)*(lambda + 2.0*mu) + 0.4444444445 / (hy*hy
		)*mu + 0.4444444444 / (hz*hz)*mu;
	Kmatrix[6][10] = -0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[6][11] = 0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[6][12] = 0.2222222223 / (hx*hx)*(lambda + 2.0*mu) - 0.4444444445 / (hy*hy
		)*mu - 0.4444444446 / (hz*hz)*mu;
	Kmatrix[6][13] = 0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[6][14] = -0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[6][15] = -0.2222222223 / (hx*hx)*(lambda + 2.0*mu) - 0.2222222223 / (hy*
		hy)*mu - 0.2222222222 / (hz*hz)*mu;
	Kmatrix[6][16] = 0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[6][17] = -0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[6][18] = 0.4444444444 / (hx*hx)*(lambda + 2.0*mu) + 0.4444444445 / (hy*hy
		)*mu - 0.888888889 / (hz*hz)*mu;
	Kmatrix[6][19] = -0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[6][20] = -0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[6][21] = -0.4444444444 / (hx*hx)*(lambda + 2.0*mu) + 0.2222222223 / (hy*
		hy)*mu - 0.4444444444 / (hz*hz)*mu;
	Kmatrix[6][22] = -0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[6][23] = -0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[7][0] = -0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[7][1] = 0.4444444445 / (hx*hx)*mu - 0.8888888891 / (hy*hy)*(lambda + 2.0*
		mu) + 0.4444444446 / (hz*hz)*mu;
	Kmatrix[7][2] = -0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[7][3] = 0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[7][4] = -0.4444444445 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*(lambda + 2.0
		*mu) + 0.2222222222 / (hz*hz)*mu;
	Kmatrix[7][5] = -0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[7][6] = -0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[7][7] = 0.8888888888 / (hx*hx)*mu + 0.8888888891 / (hy*hy)*(lambda + 2.0*
		mu) + 0.888888889 / (hz*hz)*mu;
	Kmatrix[7][8] = -0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[7][9] = 0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[7][10] = -0.8888888888 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*(lambda +
		2.0*mu) + 0.4444444444 / (hz*hz)*mu;
	Kmatrix[7][11] = -0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[7][12] = -0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[7][13] = 0.2222222223 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*(lambda + 2.0
		*mu) - 0.4444444446 / (hz*hz)*mu;
	Kmatrix[7][14] = 0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[7][15] = 0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[7][16] = -0.2222222223 / (hx*hx)*mu - 0.2222222223 / (hy*hy)*(lambda +
		2.0*mu) - 0.2222222222 / (hz*hz)*mu;
	Kmatrix[7][17] = 0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[7][18] = -0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[7][19] = 0.4444444444 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*(lambda + 2.0
		*mu) - 0.888888889 / (hz*hz)*mu;
	Kmatrix[7][20] = 0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[7][21] = 0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[7][22] = -0.4444444444 / (hx*hx)*mu + 0.2222222223 / (hy*hy)*(lambda +
		2.0*mu) - 0.4444444444 / (hz*hz)*mu;
	Kmatrix[7][23] = 0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[8][0] = 0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[8][1] = 0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[8][2] = 0.4444444445 / (hx*hx)*mu - 0.8888888891 / (hy*hy)*mu +
		0.4444444446 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[8][3] = -0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[8][4] = 0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[8][5] = -0.4444444445 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*mu +
		0.2222222222 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[8][6] = 0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[8][7] = -0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[8][8] = 0.8888888888 / (hx*hx)*mu + 0.8888888891 / (hy*hy)*mu +
		0.888888889 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[8][9] = -0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[8][10] = -0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[8][11] = -0.8888888888 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*mu +
		0.4444444444 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[8][12] = 0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[8][13] = 0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[8][14] = 0.2222222223 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*mu
		- 0.4444444446 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[8][15] = -0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[8][16] = 0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[8][17] = -0.2222222223 / (hx*hx)*mu - 0.2222222223 / (hy*hy)*mu
		- 0.2222222222 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[8][18] = 0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[8][19] = -0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[8][20] = 0.4444444444 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*mu
		- 0.888888889 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[8][21] = -0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[8][22] = -0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[8][23] = -0.4444444444 / (hx*hx)*mu + 0.2222222223 / (hy*hy)*mu
		- 0.4444444444 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[9][0] = -0.4444444445 / (hx*hx)*(lambda + 2.0*mu) - 0.4444444445 / (hy*hy
		)*mu + 0.2222222222 / (hz*hz)*mu;
	Kmatrix[9][1] = -0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[9][2] = -0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[9][3] = 0.4444444445 / (hx*hx)*(lambda + 2.0*mu) - 0.8888888891 / (hy*hy)
		*mu + 0.4444444446 / (hz*hz)*mu;
	Kmatrix[9][4] = -0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[9][5] = -0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[9][6] = -0.8888888888 / (hx*hx)*(lambda + 2.0*mu) + 0.4444444445 / (hy*hy
		)*mu + 0.4444444444 / (hz*hz)*mu;
	Kmatrix[9][7] = 0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[9][8] = -0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[9][9] = 0.8888888888 / (hx*hx)*(lambda + 2.0*mu) + 0.8888888891 / (hy*hy)
		*mu + 0.888888889 / (hz*hz)*mu;
	Kmatrix[9][10] = 0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[9][11] = -0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[9][12] = -0.2222222223 / (hx*hx)*(lambda + 2.0*mu) - 0.2222222223 / (hy*
		hy)*mu - 0.2222222222 / (hz*hz)*mu;
	Kmatrix[9][13] = -0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[9][14] = 0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[9][15] = 0.2222222223 / (hx*hx)*(lambda + 2.0*mu) - 0.4444444445 / (hy*hy
		)*mu - 0.4444444446 / (hz*hz)*mu;
	Kmatrix[9][16] = -0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[9][17] = 0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[9][18] = -0.4444444444 / (hx*hx)*(lambda + 2.0*mu) + 0.2222222223 / (hy*
		hy)*mu - 0.4444444444 / (hz*hz)*mu;
	Kmatrix[9][19] = 0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[9][20] = 0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[9][21] = 0.4444444444 / (hx*hx)*(lambda + 2.0*mu) + 0.4444444445 / (hy*hy
		)*mu - 0.888888889 / (hz*hz)*mu;
	Kmatrix[9][22] = 0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[9][23] = 0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[10][0] = -0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[10][1] = -0.4444444445 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*(lambda +
		2.0*mu) + 0.2222222222 / (hz*hz)*mu;
	Kmatrix[10][2] = -0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[10][3] = 0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[10][4] = 0.4444444445 / (hx*hx)*mu - 0.8888888891 / (hy*hy)*(lambda + 2.0
		*mu) + 0.4444444446 / (hz*hz)*mu;
	Kmatrix[10][5] = -0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[10][6] = -0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[10][7] = -0.8888888888 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*(lambda +
		2.0*mu) + 0.4444444444 / (hz*hz)*mu;
	Kmatrix[10][8] = -0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[10][9] = 0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[10][10] = 0.8888888888 / (hx*hx)*mu + 0.8888888891 / (hy*hy)*(lambda +
		2.0*mu) + 0.888888889 / (hz*hz)*mu;
	Kmatrix[10][11] = -0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[10][12] = -0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[10][13] = -0.2222222223 / (hx*hx)*mu - 0.2222222223 / (hy*hy)*(lambda +
		2.0*mu) - 0.2222222222 / (hz*hz)*mu;
	Kmatrix[10][14] = 0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[10][15] = 0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[10][16] = 0.2222222223 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*(lambda +
		2.0*mu) - 0.4444444446 / (hz*hz)*mu;
	Kmatrix[10][17] = 0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[10][18] = -0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[10][19] = -0.4444444444 / (hx*hx)*mu + 0.2222222223 / (hy*hy)*(lambda +
		2.0*mu) - 0.4444444444 / (hz*hz)*mu;
	Kmatrix[10][20] = 0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[10][21] = 0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[10][22] = 0.4444444444 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*(lambda +
		2.0*mu) - 0.888888889 / (hz*hz)*mu;
	Kmatrix[10][23] = 0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[11][0] = 0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[11][1] = 0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[11][2] = -0.4444444445 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*mu +
		0.2222222222 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[11][3] = -0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[11][4] = 0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[11][5] = 0.4444444445 / (hx*hx)*mu - 0.8888888891 / (hy*hy)*mu +
		0.4444444446 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[11][6] = 0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[11][7] = -0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[11][8] = -0.8888888888 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*mu +
		0.4444444444 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[11][9] = -0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[11][10] = -0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[11][11] = 0.8888888888 / (hx*hx)*mu + 0.8888888891 / (hy*hy)*mu +
		0.888888889 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[11][12] = 0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[11][13] = 0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[11][14] = -0.2222222223 / (hx*hx)*mu - 0.2222222223 / (hy*hy)*mu
		- 0.2222222222 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[11][15] = -0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[11][16] = 0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[11][17] = 0.2222222223 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*mu
		- 0.4444444446 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[11][18] = 0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[11][19] = -0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[11][20] = -0.4444444444 / (hx*hx)*mu + 0.2222222223 / (hy*hy)*mu
		- 0.4444444444 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[11][21] = -0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[11][22] = -0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[11][23] = 0.4444444444 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*mu
		- 0.888888889 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[12][0] = 0.4444444444 / (hx*hx)*(lambda + 2.0*mu) + 0.4444444445 / (hy*hy
		)*mu - 0.888888889 / (hz*hz)*mu;
	Kmatrix[12][1] = 0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[12][2] = 0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[12][3] = -0.4444444444 / (hx*hx)*(lambda + 2.0*mu) + 0.2222222223 / (hy*
		hy)*mu - 0.4444444444 / (hz*hz)*mu;
	Kmatrix[12][4] = 0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[12][5] = 0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[12][6] = 0.2222222223 / (hx*hx)*(lambda + 2.0*mu) - 0.4444444445 / (hy*hy
		)*mu - 0.4444444446 / (hz*hz)*mu;
	Kmatrix[12][7] = -0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[12][8] = 0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[12][9] = -0.2222222223 / (hx*hx)*(lambda + 2.0*mu) - 0.2222222223 / (hy*
		hy)*mu - 0.2222222222 / (hz*hz)*mu;
	Kmatrix[12][10] = -0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[12][11] = 0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[12][12] = 0.8888888888 / (hx*hx)*(lambda + 2.0*mu) + 0.8888888891 / (hy*
		hy)*mu + 0.888888889 / (hz*hz)*mu;
	Kmatrix[12][13] = 0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[12][14] = -0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[12][15] = -0.8888888888 / (hx*hx)*(lambda + 2.0*mu) + 0.4444444445 / (hy*
		hy)*mu + 0.4444444444 / (hz*hz)*mu;
	Kmatrix[12][16] = 0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[12][17] = -0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[12][18] = 0.4444444445 / (hx*hx)*(lambda + 2.0*mu) - 0.8888888891 / (hy*
		hy)*mu + 0.4444444446 / (hz*hz)*mu;
	Kmatrix[12][19] = -0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[12][20] = -0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[12][21] = -0.4444444445 / (hx*hx)*(lambda + 2.0*mu) - 0.4444444445 / (hy*
		hy)*mu + 0.2222222222 / (hz*hz)*mu;
	Kmatrix[12][22] = -0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[12][23] = -0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[13][0] = 0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[13][1] = 0.4444444444 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*(lambda + 2.0
		*mu) - 0.888888889 / (hz*hz)*mu;
	Kmatrix[13][2] = 0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[13][3] = -0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[13][4] = -0.4444444444 / (hx*hx)*mu + 0.2222222223 / (hy*hy)*(lambda +
		2.0*mu) - 0.4444444444 / (hz*hz)*mu;
	Kmatrix[13][5] = 0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[13][6] = 0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[13][7] = 0.2222222223 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*(lambda + 2.0
		*mu) - 0.4444444446 / (hz*hz)*mu;
	Kmatrix[13][8] = 0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[13][9] = -0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[13][10] = -0.2222222223 / (hx*hx)*mu - 0.2222222223 / (hy*hy)*(lambda +
		2.0*mu) - 0.2222222222 / (hz*hz)*mu;
	Kmatrix[13][11] = 0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[13][12] = 0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[13][13] = 0.8888888888 / (hx*hx)*mu + 0.8888888891 / (hy*hy)*(lambda +
		2.0*mu) + 0.888888889 / (hz*hz)*mu;
	Kmatrix[13][14] = -0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[13][15] = -0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[13][16] = -0.8888888888 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*(lambda +
		2.0*mu) + 0.4444444444 / (hz*hz)*mu;
	Kmatrix[13][17] = -0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[13][18] = 0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[13][19] = 0.4444444445 / (hx*hx)*mu - 0.8888888891 / (hy*hy)*(lambda +
		2.0*mu) + 0.4444444446 / (hz*hz)*mu;
	Kmatrix[13][20] = -0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[13][21] = -0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[13][22] = -0.4444444445 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*(lambda +
		2.0*mu) + 0.2222222222 / (hz*hz)*mu;
	Kmatrix[13][23] = -0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[14][0] = -0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[14][1] = -0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[14][2] = 0.4444444444 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*mu
		- 0.888888889 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[14][3] = 0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[14][4] = -0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[14][5] = -0.4444444444 / (hx*hx)*mu + 0.2222222223 / (hy*hy)*mu
		- 0.4444444444 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[14][6] = -0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[14][7] = 0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[14][8] = 0.2222222223 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*mu
		- 0.4444444446 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[14][9] = 0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[14][10] = 0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[14][11] = -0.2222222223 / (hx*hx)*mu - 0.2222222223 / (hy*hy)*mu
		- 0.2222222222 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[14][12] = -0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[14][13] = -0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[14][14] = 0.8888888888 / (hx*hx)*mu + 0.8888888891 / (hy*hy)*mu +
		0.888888889 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[14][15] = 0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[14][16] = -0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[14][17] = -0.8888888888 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*mu +
		0.4444444444 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[14][18] = -0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[14][19] = 0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[14][20] = 0.4444444445 / (hx*hx)*mu - 0.8888888891 / (hy*hy)*mu +
		0.4444444446 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[14][21] = 0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[14][22] = 0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[14][23] = -0.4444444445 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*mu +
		0.2222222222 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[15][0] = -0.4444444444 / (hx*hx)*(lambda + 2.0*mu) + 0.2222222223 / (hy*
		hy)*mu - 0.4444444444 / (hz*hz)*mu;
	Kmatrix[15][1] = -0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[15][2] = -0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[15][3] = 0.4444444444 / (hx*hx)*(lambda + 2.0*mu) + 0.4444444445 / (hy*hy
		)*mu - 0.888888889 / (hz*hz)*mu;
	Kmatrix[15][4] = -0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[15][5] = -0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[15][6] = -0.2222222223 / (hx*hx)*(lambda + 2.0*mu) - 0.2222222223 / (hy*
		hy)*mu - 0.2222222222 / (hz*hz)*mu;
	Kmatrix[15][7] = 0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[15][8] = -0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[15][9] = 0.2222222223 / (hx*hx)*(lambda + 2.0*mu) - 0.4444444445 / (hy*hy
		)*mu - 0.4444444446 / (hz*hz)*mu;
	Kmatrix[15][10] = 0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[15][11] = -0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[15][12] = -0.8888888888 / (hx*hx)*(lambda + 2.0*mu) + 0.4444444445 / (hy*
		hy)*mu + 0.4444444444 / (hz*hz)*mu;
	Kmatrix[15][13] = -0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[15][14] = 0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[15][15] = 0.8888888888 / (hx*hx)*(lambda + 2.0*mu) + 0.8888888891 / (hy*
		hy)*mu + 0.888888889 / (hz*hz)*mu;
	Kmatrix[15][16] = -0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[15][17] = 0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[15][18] = -0.4444444445 / (hx*hx)*(lambda + 2.0*mu) - 0.4444444445 / (hy*
		hy)*mu + 0.2222222222 / (hz*hz)*mu;
	Kmatrix[15][19] = 0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[15][20] = 0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[15][21] = 0.4444444445 / (hx*hx)*(lambda + 2.0*mu) - 0.8888888891 / (hy*
		hy)*mu + 0.4444444446 / (hz*hz)*mu;
	Kmatrix[15][22] = 0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[15][23] = 0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[16][0] = 0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[16][1] = -0.4444444444 / (hx*hx)*mu + 0.2222222223 / (hy*hy)*(lambda +
		2.0*mu) - 0.4444444444 / (hz*hz)*mu;
	Kmatrix[16][2] = 0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[16][3] = -0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[16][4] = 0.4444444444 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*(lambda + 2.0
		*mu) - 0.888888889 / (hz*hz)*mu;
	Kmatrix[16][5] = 0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[16][6] = 0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[16][7] = -0.2222222223 / (hx*hx)*mu - 0.2222222223 / (hy*hy)*(lambda +
		2.0*mu) - 0.2222222222 / (hz*hz)*mu;
	Kmatrix[16][8] = 0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[16][9] = -0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[16][10] = 0.2222222223 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*(lambda +
		2.0*mu) - 0.4444444446 / (hz*hz)*mu;
	Kmatrix[16][11] = 0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[16][12] = 0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[16][13] = -0.8888888888 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*(lambda +
		2.0*mu) + 0.4444444444 / (hz*hz)*mu;
	Kmatrix[16][14] = -0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[16][15] = -0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[16][16] = 0.8888888888 / (hx*hx)*mu + 0.8888888891 / (hy*hy)*(lambda +
		2.0*mu) + 0.888888889 / (hz*hz)*mu;
	Kmatrix[16][17] = -0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[16][18] = 0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[16][19] = -0.4444444445 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*(lambda +
		2.0*mu) + 0.2222222222 / (hz*hz)*mu;
	Kmatrix[16][20] = -0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[16][21] = -0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[16][22] = 0.4444444445 / (hx*hx)*mu - 0.8888888891 / (hy*hy)*(lambda +
		2.0*mu) + 0.4444444446 / (hz*hz)*mu;
	Kmatrix[16][23] = -0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[17][0] = -0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[17][1] = -0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[17][2] = -0.4444444444 / (hx*hx)*mu + 0.2222222223 / (hy*hy)*mu
		- 0.4444444444 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[17][3] = 0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[17][4] = -0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[17][5] = 0.4444444444 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*mu
		- 0.888888889 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[17][6] = -0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[17][7] = 0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[17][8] = -0.2222222223 / (hx*hx)*mu - 0.2222222223 / (hy*hy)*mu
		- 0.2222222222 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[17][9] = 0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[17][10] = 0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[17][11] = 0.2222222223 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*mu
		- 0.4444444446 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[17][12] = -0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[17][13] = -0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[17][14] = -0.8888888888 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*mu +
		0.4444444444 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[17][15] = 0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[17][16] = -0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[17][17] = 0.8888888888 / (hx*hx)*mu + 0.8888888891 / (hy*hy)*mu +
		0.888888889 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[17][18] = -0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[17][19] = 0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[17][20] = -0.4444444445 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*mu +
		0.2222222222 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[17][21] = 0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[17][22] = 0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[17][23] = 0.4444444445 / (hx*hx)*mu - 0.8888888891 / (hy*hy)*mu +
		0.4444444446 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[18][0] = 0.2222222223 / (hx*hx)*(lambda + 2.0*mu) - 0.4444444445 / (hy*hy
		)*mu - 0.4444444446 / (hz*hz)*mu;
	Kmatrix[18][1] = 0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[18][2] = 0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[18][3] = -0.2222222223 / (hx*hx)*(lambda + 2.0*mu) - 0.2222222223 / (hy*
		hy)*mu - 0.2222222222 / (hz*hz)*mu;
	Kmatrix[18][4] = 0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[18][5] = 0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[18][6] = 0.4444444444 / (hx*hx)*(lambda + 2.0*mu) + 0.4444444445 / (hy*hy
		)*mu - 0.888888889 / (hz*hz)*mu;
	Kmatrix[18][7] = -0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[18][8] = 0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[18][9] = -0.4444444444 / (hx*hx)*(lambda + 2.0*mu) + 0.2222222223 / (hy*
		hy)*mu - 0.4444444444 / (hz*hz)*mu;
	Kmatrix[18][10] = -0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[18][11] = 0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[18][12] = 0.4444444445 / (hx*hx)*(lambda + 2.0*mu) - 0.8888888891 / (hy*
		hy)*mu + 0.4444444446 / (hz*hz)*mu;
	Kmatrix[18][13] = 0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[18][14] = -0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[18][15] = -0.4444444445 / (hx*hx)*(lambda + 2.0*mu) - 0.4444444445 / (hy*
		hy)*mu + 0.2222222222 / (hz*hz)*mu;
	Kmatrix[18][16] = 0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[18][17] = -0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[18][18] = 0.8888888888 / (hx*hx)*(lambda + 2.0*mu) + 0.8888888891 / (hy*
		hy)*mu + 0.888888889 / (hz*hz)*mu;
	Kmatrix[18][19] = -0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[18][20] = -0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[18][21] = -0.8888888888 / (hx*hx)*(lambda + 2.0*mu) + 0.4444444445 / (hy*
		hy)*mu + 0.4444444444 / (hz*hz)*mu;
	Kmatrix[18][22] = -0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[18][23] = -0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[19][0] = -0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[19][1] = 0.2222222223 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*(lambda + 2.0
		*mu) - 0.4444444446 / (hz*hz)*mu;
	Kmatrix[19][2] = -0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[19][3] = 0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[19][4] = -0.2222222223 / (hx*hx)*mu - 0.2222222223 / (hy*hy)*(lambda +
		2.0*mu) - 0.2222222222 / (hz*hz)*mu;
	Kmatrix[19][5] = -0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[19][6] = -0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[19][7] = 0.4444444444 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*(lambda + 2.0
		*mu) - 0.888888889 / (hz*hz)*mu;
	Kmatrix[19][8] = -0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[19][9] = 0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[19][10] = -0.4444444444 / (hx*hx)*mu + 0.2222222223 / (hy*hy)*(lambda +
		2.0*mu) - 0.4444444444 / (hz*hz)*mu;
	Kmatrix[19][11] = -0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[19][12] = -0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[19][13] = 0.4444444445 / (hx*hx)*mu - 0.8888888891 / (hy*hy)*(lambda +
		2.0*mu) + 0.4444444446 / (hz*hz)*mu;
	Kmatrix[19][14] = 0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[19][15] = 0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[19][16] = -0.4444444445 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*(lambda +
		2.0*mu) + 0.2222222222 / (hz*hz)*mu;
	Kmatrix[19][17] = 0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[19][18] = -0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[19][19] = 0.8888888888 / (hx*hx)*mu + 0.8888888891 / (hy*hy)*(lambda +
		2.0*mu) + 0.888888889 / (hz*hz)*mu;
	Kmatrix[19][20] = 0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[19][21] = 0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[19][22] = -0.8888888888 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*(lambda +
		2.0*mu) + 0.4444444444 / (hz*hz)*mu;
	Kmatrix[19][23] = 0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[20][0] = -0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[20][1] = -0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[20][2] = 0.2222222223 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*mu
		- 0.4444444446 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[20][3] = 0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[20][4] = -0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[20][5] = -0.2222222223 / (hx*hx)*mu - 0.2222222223 / (hy*hy)*mu
		- 0.2222222222 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[20][6] = -0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[20][7] = 0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[20][8] = 0.4444444444 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*mu
		- 0.888888889 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[20][9] = 0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[20][10] = 0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[20][11] = -0.4444444444 / (hx*hx)*mu + 0.2222222223 / (hy*hy)*mu
		- 0.4444444444 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[20][12] = -0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[20][13] = -0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[20][14] = 0.4444444445 / (hx*hx)*mu - 0.8888888891 / (hy*hy)*mu +
		0.4444444446 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[20][15] = 0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[20][16] = -0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[20][17] = -0.4444444445 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*mu +
		0.2222222222 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[20][18] = -0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[20][19] = 0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[20][20] = 0.8888888888 / (hx*hx)*mu + 0.8888888891 / (hy*hy)*mu +
		0.888888889 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[20][21] = 0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[20][22] = 0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[20][23] = -0.8888888888 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*mu +
		0.4444444444 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[21][0] = -0.2222222223 / (hx*hx)*(lambda + 2.0*mu) - 0.2222222223 / (hy*
		hy)*mu - 0.2222222222 / (hz*hz)*mu;
	Kmatrix[21][1] = -0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[21][2] = -0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[21][3] = 0.2222222223 / (hx*hx)*(lambda + 2.0*mu) - 0.4444444445 / (hy*hy
		)*mu - 0.4444444446 / (hz*hz)*mu;
	Kmatrix[21][4] = -0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[21][5] = -0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[21][6] = -0.4444444444 / (hx*hx)*(lambda + 2.0*mu) + 0.2222222223 / (hy*
		hy)*mu - 0.4444444444 / (hz*hz)*mu;
	Kmatrix[21][7] = 0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[21][8] = -0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[21][9] = 0.4444444444 / (hx*hx)*(lambda + 2.0*mu) + 0.4444444445 / (hy*hy
		)*mu - 0.888888889 / (hz*hz)*mu;
	Kmatrix[21][10] = 0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[21][11] = -0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[21][12] = -0.4444444445 / (hx*hx)*(lambda + 2.0*mu) - 0.4444444445 / (hy*
		hy)*mu + 0.2222222222 / (hz*hz)*mu;
	Kmatrix[21][13] = -0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[21][14] = 0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[21][15] = 0.4444444445 / (hx*hx)*(lambda + 2.0*mu) - 0.8888888891 / (hy*
		hy)*mu + 0.4444444446 / (hz*hz)*mu;
	Kmatrix[21][16] = -0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[21][17] = 0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[21][18] = -0.8888888888 / (hx*hx)*(lambda + 2.0*mu) + 0.4444444445 / (hy*
		hy)*mu + 0.4444444444 / (hz*hz)*mu;
	Kmatrix[21][19] = 0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[21][20] = 0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[21][21] = 0.8888888888 / (hx*hx)*(lambda + 2.0*mu) + 0.8888888891 / (hy*
		hy)*mu + 0.888888889 / (hz*hz)*mu;
	Kmatrix[21][22] = 0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[21][23] = 0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[22][0] = -0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[22][1] = -0.2222222223 / (hx*hx)*mu - 0.2222222223 / (hy*hy)*(lambda +
		2.0*mu) - 0.2222222222 / (hz*hz)*mu;
	Kmatrix[22][2] = -0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[22][3] = 0.3333333333 / hx * lambda / hy - 0.3333333333 / hy * mu / hx;
	Kmatrix[22][4] = 0.2222222223 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*(lambda + 2.0
		*mu) - 0.4444444446 / (hz*hz)*mu;
	Kmatrix[22][5] = -0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[22][6] = -0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[22][7] = -0.4444444444 / (hx*hx)*mu + 0.2222222223 / (hy*hy)*(lambda +
		2.0*mu) - 0.4444444444 / (hz*hz)*mu;
	Kmatrix[22][8] = -0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[22][9] = 0.3333333333 / hx * lambda / hy + 0.3333333333 / hy * mu / hx;
	Kmatrix[22][10] = 0.4444444444 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*(lambda +
		2.0*mu) - 0.888888889 / (hz*hz)*mu;
	Kmatrix[22][11] = -0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[22][12] = -0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[22][13] = -0.4444444445 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*(lambda +
		2.0*mu) + 0.2222222222 / (hz*hz)*mu;
	Kmatrix[22][14] = 0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[22][15] = 0.6666666667 / hx * lambda / hy - 0.6666666667 / hy * mu / hx;
	Kmatrix[22][16] = 0.4444444445 / (hx*hx)*mu - 0.8888888891 / (hy*hy)*(lambda +
		2.0*mu) + 0.4444444446 / (hz*hz)*mu;
	Kmatrix[22][17] = 0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[22][18] = -0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[22][19] = -0.8888888888 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*(lambda +
		2.0*mu) + 0.4444444444 / (hz*hz)*mu;
	Kmatrix[22][20] = 0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[22][21] = 0.6666666667 / hx * lambda / hy + 0.6666666667 / hy * mu / hx;
	Kmatrix[22][22] = 0.8888888888 / (hx*hx)*mu + 0.8888888891 / (hy*hy)*(lambda +
		2.0*mu) + 0.888888889 / (hz*hz)*mu;
	Kmatrix[22][23] = 0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[23][0] = -0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[23][1] = -0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[23][2] = -0.2222222223 / (hx*hx)*mu - 0.2222222223 / (hy*hy)*mu
		- 0.2222222222 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[23][3] = 0.3333333334 / hx * lambda / hz - 0.3333333334 / hz * mu / hx;
	Kmatrix[23][4] = -0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[23][5] = 0.2222222223 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*mu
		- 0.4444444446 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[23][6] = -0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[23][7] = 0.3333333334 / hy * lambda / hz - 0.3333333334 / hz * mu / hy;
	Kmatrix[23][8] = -0.4444444444 / (hx*hx)*mu + 0.2222222223 / (hy*hy)*mu
		- 0.4444444444 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[23][9] = 0.6666666666 / hx * lambda / hz - 0.6666666666 / hz * mu / hx;
	Kmatrix[23][10] = 0.6666666668 / hy * lambda / hz - 0.6666666668 / hz * mu / hy;
	Kmatrix[23][11] = 0.4444444444 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*mu
		- 0.888888889 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[23][12] = -0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[23][13] = -0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[23][14] = -0.4444444445 / (hx*hx)*mu - 0.4444444445 / (hy*hy)*mu +
		0.2222222222 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[23][15] = 0.3333333334 / hx * lambda / hz + 0.3333333334 / hz * mu / hx;
	Kmatrix[23][16] = -0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[23][17] = 0.4444444445 / (hx*hx)*mu - 0.8888888891 / (hy*hy)*mu +
		0.4444444446 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[23][18] = -0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[23][19] = 0.3333333334 / hy * lambda / hz + 0.3333333334 / hz * mu / hy;
	Kmatrix[23][20] = -0.8888888888 / (hx*hx)*mu + 0.4444444445 / (hy*hy)*mu +
		0.4444444444 / (hz*hz)*(lambda + 2.0*mu);
	Kmatrix[23][21] = 0.6666666666 / hx * lambda / hz + 0.6666666666 / hz * mu / hx;
	Kmatrix[23][22] = 0.6666666668 / hy * lambda / hz + 0.6666666668 / hz * mu / hy;
	Kmatrix[23][23] = 0.8888888888 / (hx*hx)*mu + 0.8888888891 / (hy*hy)*mu +
		0.888888889 / (hz*hz)*(lambda + 2.0*mu);



	volume3D(iP, nvtx, pa, hx, hy, hz);
	//printf("%e %e %e\n",hx,hy,hz);
	integer imc = -729;
	// �� �������� �������� �� ����� ������ �������������, ����� ����������� ���������.
	for (integer i_4 = 0; i_4 < 24; i_4++) {
		for (integer j_4 = 0; j_4 < 24; j_4++) {
			//[Kmatrix]==[Pa*m^(-2)]=[Newton*m^(-4)].
			//[u]=[m].
			// [Pa]=[mu]=[lambda]=[Newton/m^2].
			Kmatrix[i_4][j_4] *= 0.5*0.125*(hx*hy*hz);//*m^3
			//0.125 �.�. ������ ������� ��������� ������� ����� ������ ������.
			//[Kmatrix*vol*u] = [Newton];
		}

	}

	errno_t err96;
	FILE* fp96 = NULL;
	if (iP == imc) {
		fopen_s(&fp96, "maple8", "w");
	}

	if (iP == imc) {
		printf("mu=%e lambda=%e hx=%e hy=%e hz=%e\n", mu, lambda, hx, hy, hz);
		printf("multiplyer=%e\n", 0.017448*228.51*(hx*hy*hz)*1e-5);
		fprintf(fp96, "Cmatr:=matrix(24,24,[[");
	}

	for (integer i_4 = 0; i_4 < 24; i_4++) {
		for (integer j_4 = 0; j_4 < 24; j_4++) {
			//228.51 ����������� ����������� ����������� ��� ����� ��������� � ANSYS ������ �� ����������.
			if (iP == imc) {
				if (Kmatrix[i_4][j_4] > 0) {
					printf(" %1.2f ", 1E-5*Kmatrix[i_4][j_4]);
					fprintf(fp96, " %1.2f, ", 1E-5*Kmatrix[i_4][j_4]);
				}
				else {
					printf("%1.2f ", 1E-5*Kmatrix[i_4][j_4]);
					fprintf(fp96, "%1.2f, ", 1E-5*Kmatrix[i_4][j_4]);
				}
				if (1E-5*fabs(Kmatrix[i_4][j_4] - Kmatrix[j_4][i_4])>1.0e-2) {
					printf("\n%e=%e %d %d\n", 1E-5*Kmatrix[i_4][j_4], 1E-5*Kmatrix[j_4][i_4], i_4, j_4);
					getchar();
				}
			}
		}
		if (iP == imc) {
			printf("\n");
			if (i_4 < 23) { fprintf(fp96, "],[\n"); }
			else { fprintf(fp96, "]]);"); }
		}
	}
	if (iP == imc) {
		getchar();
		fclose(fp96);
	}
}


// �������������� ������ ������� Ƹ������� ��� ������������ ������. 4.08.2017.
// 16.09.2017.
void Thermal_Structural_assemble2(integer iP, integer** nvtx, 
	TOCHKA* pa, doublereal** prop, doublereal** &Kmatrix)
{
	
	//nvtx
	// ---|+--|-+-|++-|--+|+-+|-++|+++
	// 1	2	3	4	5	6	7	8
	// ������� ������������ ������� ����� � ������ ������.
	// --+|-++|+++|+-+|---|-+-|++-|+--
	// 5	7	8	6	1	3	4	2
	// ����� ������ ����� ������������� ����� �������. 


	doublereal hx=1.0, hy=1.0, hz=1.0; // ������� ������
	volume3D(iP, nvtx, pa, hx, hy, hz);
	//printf("%e %e %e\n",hx,hy,hz);

	doublereal mu, lambda; // ������������ ����.

	mu = prop[MU_LAME][iP];
	lambda = prop[LAMBDA_LAME][iP];

	const doublereal kc1 = 0.05555557;
	const doublereal kc2 = 0.04166666;
	const doublereal kc3 = 0.02777777;
	const doublereal kc4 = 0.02083333;
	const doublereal kc5 = 0.01388888;

	// ������ ��������� �������.
	Kmatrix[0][0] = (kc1*(lambda + 2 * mu) / (hx*hx))+(kc1*mu/(hy*hy))+((kc1*mu)/(hz*hz));
	Kmatrix[0][1] = ((kc2*lambda) / (hx*hy)) + ((kc2*mu) / (hy*hx));
	Kmatrix[0][2] = -((kc2*lambda) / (hx*hz)) - ((kc2*mu)/(hz*hx));
	Kmatrix[0][3] = (kc3*(lambda + 2 * mu) / (hx*hx)) - ((kc1*mu) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[0][4] = -((kc2*lambda) / (hx*hy)) + ((kc2*mu) / (hy*hx));
	Kmatrix[0][5] = -((kc4*lambda) / (hx*hz)) - ((kc4*mu) / (hz*hx));
	Kmatrix[0][6] = -((kc3*(lambda + 2 * mu)) / (hx*hx)) - ((kc3*mu) / (hy*hy)) + ((kc5*mu) / (hz*hz));
	Kmatrix[0][7] = -((kc2*lambda) / (hx*hy)) - ((kc2*mu) / (hy*hx));
	Kmatrix[0][8] = -((kc4*lambda) / (hx*hz)) + ((kc4*mu)/(hz*hx));
	Kmatrix[0][9] = -((kc1*(lambda + 2 * mu)) / (hx*hx)) + ((kc3*mu) / (hy*hy)) + ((kc3*mu)/(hz*hz));
	Kmatrix[0][10] = ((kc2*lambda) / (hx*hy)) - ((kc2*mu)/(hy*hx));
	Kmatrix[0][11] = -((kc2*lambda) / (hx*hz)) + ((kc2*mu)/(hz*hx));
	Kmatrix[0][12] = ((kc3*(lambda + 2 * mu)) / (hx*hx)) + ((kc3*mu) / (hy*hy)) - ((kc1*mu) / (hz*hz));
	Kmatrix[0][13] = ((kc4*lambda)/(hx*hy))+((kc4*mu)/(hy*hx));
	Kmatrix[0][14] = ((kc2*lambda) / (hx*hz)) - ((kc2*mu)/(hz*hx));
	Kmatrix[0][15] = ((kc5*(lambda+2*mu))/(hx*hx)) - ((kc3*mu)/(hy*hy)) - ((kc3*mu)/(hz*hz));
	Kmatrix[0][16] = -((kc4*lambda) / (hx*hy)) + ((kc4*mu)/(hy*hx));
	Kmatrix[0][17] = ((kc4*lambda)/(hx*hz)) - ((kc4*mu)/(hz*hx));
	Kmatrix[0][18] = -((kc5*(lambda + 2 * mu)) / (hx*hx)) - ((kc5*mu)/(hy*hy)) - ((kc5*mu)/(hz*hz));
	Kmatrix[0][19] = -((kc4*lambda) / (hx*hy)) - ((kc4*mu)/(hy*hx));
	Kmatrix[0][20] = ((kc4*lambda)/(hx*hz)) + ((kc4*mu)/(hz*hx));
	Kmatrix[0][21] = -((kc3*(lambda+2*mu))/(hx*hx)) +((kc5*mu)/(hy*hy)) -((kc3*mu)/(hz*hz));
	Kmatrix[0][22] = ((kc4*lambda)/(hx*hy)) - ((kc4*mu)/(hy*hx));
	Kmatrix[0][23] = ((kc2*lambda)/(hx*hz)) + ((kc2*mu)/(hz*hx));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[1][0] = (kc2*(lambda + mu)) / (hx*hy);
	Kmatrix[1][1] = (kc1*(mu) / (hx*hx)) + ((kc1*(lambda + 2 * mu)) / (hy*hy))+((kc1*mu)/(hz*hz));
	Kmatrix[1][2] = -((kc2*(lambda+mu))/(hy*hz));
	Kmatrix[1][3] = (kc2*(lambda-mu)) / (hy*hx);
	Kmatrix[1][4] = ((kc3*mu)/(hx*hx)) - ((kc1*(lambda+2*mu))/(hy*hy))+((kc3*mu)/(hz*hz));
	Kmatrix[1][5] = -((kc2*(lambda-mu)) / (hy*hz));
	Kmatrix[1][6] = -((kc2*lambda)/(hx*hy)) - ((kc2*mu)/(hy*hx));
	Kmatrix[1][7] = -((kc3*mu)/(hx*hx)) - ((kc3*(lambda+2*mu))/(hy*hy))+((kc5*mu)/(hz*hz));
	Kmatrix[1][8] = -((kc4*(lambda-mu)) / (hy*hz));
	Kmatrix[1][9] = ((kc2*(mu-lambda))/(hx*hy));
	Kmatrix[1][10] = -((kc1*mu)/(hx*hx)) + ((kc3*(lambda+2*mu))/(hy*hy))+((kc3*mu)/(hz*hz));
	Kmatrix[1][11] = -((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[1][12] = kc4*((lambda + mu) / (hx*hy));
	Kmatrix[1][13] = ((kc3*mu)/(hx*hx)) + ((kc3*(lambda+2*mu))/(hy*hy))-((kc1*mu)/(hz*hz));
	Kmatrix[1][14] = ((kc2*(lambda-mu)) / (hy*hz));
	Kmatrix[1][15] = ((kc4*(lambda - mu)) / (hx*hy));
	Kmatrix[1][16] = ((kc5*mu) / (hx*hx)) - ((kc3*(lambda+2*mu))/(hy*hy))-((kc3*mu)/(hz*hz));
	Kmatrix[1][17] = ((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[1][18] = -((kc4*lambda)/(hx*hy)) - ((kc4*mu)/(hx*hy));
	Kmatrix[1][19] = -((kc5*mu) / (hx*hx)) - ((kc5*(lambda+2*mu))/(hy*hy))-((kc5*mu)/(hz*hz));
	Kmatrix[1][20] = ((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[1][21] = ((kc4*(mu-lambda))/(hx*hy));
	Kmatrix[1][22] = -((kc3*mu) / (hx*hx)) + ((kc5*(lambda+2*mu))/(hy*hy))-((kc3*mu)/(hz*hz));
	Kmatrix[1][23] = ((kc4*(lambda-mu)) / (hy*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[2][0] = -((kc2*(lambda+mu))/(hx*hz));
	Kmatrix[2][1] = -((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[2][2] = ((kc1*mu) / (hx*hx)) + ((kc1*mu) / (hy*hy)) + ((kc1*(lambda+2*mu))/(hz*hz));
	Kmatrix[2][3] = -((kc4*(lambda+mu))/(hx*hz));
	Kmatrix[2][4] = ((kc2*(lambda-mu))/(hy*hz));
	Kmatrix[2][5] = ((kc3*mu)/(hx*hx))-((kc1*mu)/(hy*hy))+((kc3*(lambda+2*mu))/(hz*hz));
	Kmatrix[2][6] = ((kc4*(lambda-mu))/(hx*hz));
	Kmatrix[2][7] = (kc4*(lambda - mu)) / (hy*hz);
	Kmatrix[2][8] = -((kc3*mu)/(hx*hx)) - ((kc3*mu) / (hy*hy)) + ((kc5*(lambda+2*mu)) / (hz*hz));
	Kmatrix[2][9] = ((kc2*(lambda-mu))/(hx*hz));
	Kmatrix[2][10] = -((kc4*(lambda+mu))/(hy*hz));
	Kmatrix[2][11] = -((kc1*mu) / (hx*hx)) + ((kc3*mu) / (hy*hy)) + ((kc3*(lambda+2*mu))/(hz*hz));
	Kmatrix[2][12] = ((kc2*(mu-lambda))/(hx*hz));
	Kmatrix[2][13] = ((kc2*(mu-lambda))/(hy*hz));
	Kmatrix[2][14] = ((kc3*mu)/(hx*hx))+ ((kc3*mu) / (hy*hy))- ((kc1*(lambda+2*mu)) / (hz*hz));
	Kmatrix[2][15] = ((kc4*(mu-lambda))/(hx*hz));
	Kmatrix[2][16] = ((kc2*(lambda+mu))/(hy*hz));
	Kmatrix[2][17] = ((kc5*mu) / (hx*hx)) - ((kc3*mu) / (hy*hy)) - ((kc3*(lambda+2*mu))/(hz*hz));
	Kmatrix[2][18] = ((kc4*(lambda+mu))/(hx*hz));
	Kmatrix[2][19] = ((kc4*(lambda+mu))/(hy*hz));
	Kmatrix[2][20] = -((kc5*mu)/(hx*hx))-((kc5*mu)/(hy*hy))-((kc5*(lambda+2*mu))/(hz*hz));
	Kmatrix[2][21] = ((kc2*(lambda + mu)) / (hx*hz));
	Kmatrix[2][22] = ((kc4*(mu-lambda))/(hy*hz));
	Kmatrix[2][23] = -((kc3*mu)/(hx*hx))+((kc5*mu)/(hy*hy))-((kc3*(lambda+2*mu))/(hz*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[3][0] = ((kc3*(lambda+2*mu))/(hx*hx))-((kc1*mu)/(hy*hy))+((kc3*mu)/(hz*hz));
	Kmatrix[3][1] = ((kc2*(lambda-mu))/(hx*hy));
	Kmatrix[3][2] = -((kc4*(lambda+mu))/(hx*hz));
	Kmatrix[3][3] = ((kc1*(lambda+2*mu))/(hx*hx)) + ((kc1*mu)/(hy*hy)) + ((kc1*mu)/(hz*hz));
	Kmatrix[3][4] = -((kc2*(lambda+mu))/(hx*hy));
	Kmatrix[3][5] = -((kc2*(lambda+mu))/(hx*hz));
	Kmatrix[3][6] = -((kc1*(lambda+2*mu))/(hx*hx)) + ((kc3*mu)/(hy*hy)) + ((kc3*mu)/(hz*hz));
	Kmatrix[3][7] = ((kc2*(mu-lambda))/(hx*hy));
	Kmatrix[3][8] = ((kc2*(mu-lambda))/(hx*hz));
	Kmatrix[3][9] = -((kc3*(lambda+2*mu))/(hx*hx)) - ((kc3*mu)/(hy*hy)) + ((kc5*mu)/(hz*hz));
	Kmatrix[3][10] = ((kc2*(lambda+mu))/(hx*hy));
	Kmatrix[3][11] = ((kc4*(mu-lambda))/(hx*hz));
	Kmatrix[3][12] = ((kc5*(lambda+2*mu))/(hx*hx)) - ((kc3*mu)/(hy*hy)) - ((kc3*mu)/(hz*hz));
	Kmatrix[3][13] = ((kc4*(lambda-mu))/(hx*hy));
	Kmatrix[3][14] = ((kc4*(lambda-mu))/(hx*hz));
	Kmatrix[3][15] = ((kc3*(lambda+2*mu))/(hx*hx)) + ((kc3*mu)/(hy*hy)) - ((kc1*mu)/(hz*hz));
	Kmatrix[3][16] = -((kc4*(mu+lambda))/(hx*hy));
	Kmatrix[3][17] = ((kc2*(lambda-mu))/(hx*hz));
	Kmatrix[3][18] = -((kc3*(lambda+2*mu)) / (hx*hx)) + ((kc5*mu) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[3][19] = ((kc4*(mu-lambda)) / (hx*hy));
	Kmatrix[3][20] = ((kc2*(mu+lambda)) / (hx*hz));
	Kmatrix[3][21] = - ((kc5*(lambda+2*mu)) / (hx*hx)) - ((kc5*mu) / (hy*hy)) - ((kc5*mu) / (hz*hz));
	Kmatrix[3][22] = ((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[3][23] = (kc4*(lambda+mu) / (hx*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[4][0] = ((kc2*(mu-lambda)) / (hx*hy));
	Kmatrix[4][1] = ((kc3*mu) / (hx*hx)) - ((kc1*(lambda+2*mu)) / (hy*hy))+((kc3*mu)/(hz*hz));	
	Kmatrix[4][2] = ((kc2*(lambda-mu)) / (hy*hz));
	Kmatrix[4][3] = -((kc2*(mu+lambda)) / (hx*hy));
	Kmatrix[4][4] = ((kc1*mu) / (hx*hx)) + ((kc1*(lambda+2*mu)) / (hy*hy)) + ((kc1*mu) / (hz*hz));
	Kmatrix[4][5] = ((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[4][6] = ((kc2*(lambda-mu)) / (hx*hy));
	Kmatrix[4][7] = -((kc1*mu) / (hx*hx)) + ((kc3*(lambda+2*mu)) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[4][8] = ((kc4*(mu+lambda)) / (hy*hz));
	Kmatrix[4][9] = ((kc2*(mu+lambda)) / (hx*hy));
	Kmatrix[4][10] = -((kc3*mu) / (hx*hx)) - ((kc3*(lambda+2*mu)) / (hy*hy)) + ((kc5*mu) / (hz*hz));
	Kmatrix[4][11] = ((kc4*(lambda-mu)) / (hy*hz));
	Kmatrix[4][12] = ((kc4*(mu-lambda)) / (hx*hy));
	Kmatrix[4][13] = ((kc5*mu) / (hx*hx)) - ((kc3*(lambda+2*mu)) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[4][14] = -((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[4][15] = -((kc4*(mu+lambda)) / (hx*hy));
	Kmatrix[4][16] = ((kc3*mu) / (hx*hx)) + ((kc3*(lambda+2*mu)) / (hy*hy)) - ((kc1*mu) / (hz*hz));
	Kmatrix[4][17] = ((kc2*(mu-lambda)) / (hy*hz));
	Kmatrix[4][18] = ((kc4*(lambda-mu)) / (hx*hy));
	Kmatrix[4][19] = -((kc3*mu) / (hx*hx)) + ((kc5*(lambda+2*mu)) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[4][20] = ((kc4*(mu-lambda)) / (hy*hz));
	Kmatrix[4][21] = ((kc4*(mu+lambda)) / (hx*hy));
	Kmatrix[4][22] = -((kc5*mu) / (hx*hx)) - ((kc5*(lambda+2*mu)) / (hy*hy)) - ((kc5*mu) / (hz*hz));
	Kmatrix[4][23] = -((kc4*(lambda+mu)) / (hy*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[5][0] = -((kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[5][1] = -((kc2*(lambda-mu)) / (hy*hz));
	Kmatrix[5][2] = ((kc3*mu) / (hx*hx)) - ((kc1*mu) / (hy*hy)) + ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[5][3] = -((kc2*(mu+lambda)) / (hx*hz));
	Kmatrix[5][4] = ((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[5][5] = ((kc1*mu) / (hx*hx)) + ((kc1*mu) / (hy*hy)) + ((kc1*(lambda+2*mu)) / (hz*hz));
	Kmatrix[5][6] = ((kc2*(lambda-mu)) / (hx*hz));
	Kmatrix[5][7] = ((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[5][8] = -((kc1*mu) / (hx*hx)) + ((kc3*mu) / (hy*hy)) + ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[5][9] = ((kc4*(lambda-mu)) / (hx*hz));
	Kmatrix[5][10] = ((kc4*(mu-lambda)) / (hy*hz));
	Kmatrix[5][11] = -((kc3*mu) / (hx*hx)) - ((kc3*mu) / (hy*hy)) + ((kc5*(lambda+2*mu)) / (hz*hz));
	Kmatrix[5][12] = ((kc4*(mu-lambda)) / (hx*hz));
	Kmatrix[5][13] = -((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[5][14] = ((kc5*mu) / (hx*hx)) - ((kc3*mu) / (hy*hy)) - ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[5][15] = ((kc2*(mu-lambda)) / (hx*hz));
	Kmatrix[5][16] = ((kc2*(lambda-mu)) / (hy*hz));
	Kmatrix[5][17] = ((kc3*mu) / (hx*hx)) + ((kc3*mu) / (hy*hy)) - ((kc1*(lambda+2*mu)) / (hz*hz));
	Kmatrix[5][18] = ((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[5][19] = ((kc4*(lambda-mu)) / (hy*hz));
	Kmatrix[5][20] = -((kc3*mu) / (hx*hx)) + ((kc5*mu) / (hy*hy)) - ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[5][21] = ((kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[5][22] = -((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[5][23] = -((kc5*mu) / (hx*hx)) - ((kc5*mu) / (hy*hy)) - ((kc5*(lambda+2*mu)) / (hz*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[6][0] = -((kc3*(lambda+2*mu)) / (hx*hx)) - ((kc3*mu) / (hy*hy)) + ((kc5*mu) / (hz*hz));
	Kmatrix[6][1] = -((kc2*(lambda+mu)) / (hx*hy));
	Kmatrix[6][2] = ((kc4*(lambda-mu)) / (hx*hz));
	Kmatrix[6][3] = -((kc1*(lambda+2*mu)) / (hx*hx)) + ((kc3*mu) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[6][4] = ((kc2*(lambda-mu)) / (hx*hy));
	Kmatrix[6][5] = ((kc2*(lambda-mu)) / (hx*hz));
	Kmatrix[6][6] = ((kc1*(lambda+2*mu)) / (hx*hx)) + ((kc1*mu) / (hy*hy)) + ((kc1*mu) / (hz*hz));
	Kmatrix[6][7] = ((kc2*(lambda+mu)) / (hx*hy));
	Kmatrix[6][8] = ((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[6][9] = ((kc3*(lambda+2*mu)) / (hx*hx)) - ((kc1*mu) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[6][10] = ((kc2*(mu-lambda)) / (hx*hy));
	Kmatrix[6][11] = ((kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[6][12] = -((kc5*(lambda+2*mu)) / (hx*hx)) - ((kc5*mu) / (hy*hy)) - ((kc5*mu) / (hz*hz));
	Kmatrix[6][13] = -((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[6][14] = -((kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[6][15] = -((kc3*(lambda+2*mu)) / (hx*hx)) + ((kc5*mu) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[6][16] = ((kc4*(lambda-mu)) / (hx*hy));
	Kmatrix[6][17] = -((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[6][18] = ((kc3*(lambda+2*mu)) / (hx*hx)) + ((kc3*mu) / (hy*hy)) - ((kc1*mu) / (hz*hz));
	Kmatrix[6][19] = ((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[6][20] = ((kc2*(mu-lambda)) / (hx*hz));
	Kmatrix[6][21] = ((kc5*(lambda+2*mu)) / (hx*hx)) - ((kc3*mu) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[6][22] = ((kc4*(mu-lambda)) / (hx*hy));
	Kmatrix[6][23] = ((kc4*(mu-lambda)) / (hx*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[7][0] = -((kc2*(lambda+mu)) / (hx*hy));
	Kmatrix[7][1] = -((kc3*mu) / (hx*hx)) - ((kc3*(lambda+2*mu)) / (hy*hy))+((kc5*mu)/(hz*hz));
	Kmatrix[7][2] = ((kc4*(lambda-mu)) / (hy*hz));
	Kmatrix[7][3] = ((kc2*(mu-lambda)) / (hx*hy));
	Kmatrix[7][4] = -((kc1*mu) / (hx*hx)) + ((kc3*(lambda+2*mu)) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[7][5] = ((kc4*(mu+lambda)) / (hy*hz));
	Kmatrix[7][6] = ((kc2*(lambda+mu)) / (hx*hy));
	Kmatrix[7][7] = ((kc1*mu) / (hx*hx)) + ((kc1*(lambda+2*mu)) / (hy*hy)) + ((kc1*mu) / (hz*hz));
	Kmatrix[7][8] = ((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[7][9] = ((kc2*(lambda-mu)) / (hx*hy));
	Kmatrix[7][10] = ((kc3*mu) / (hx*hx)) - ((kc1*(lambda+2*mu)) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[7][11] = ((kc2*(lambda-mu)) / (hy*hz));
	Kmatrix[7][12] = -((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[7][13] = -((kc5*mu) / (hx*hx)) - ((kc5*(lambda+2*mu)) / (hy*hy)) - ((kc5*mu) / (hz*hz));
	Kmatrix[7][14] = -((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[7][15] = ((kc4*(mu-lambda)) / (hx*hy));
	Kmatrix[7][16] = -((kc3*mu) / (hx*hx)) + ((kc5*(lambda+2*mu)) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[7][17] = ((kc4*(mu-lambda)) / (hy*hz));
	Kmatrix[7][18] = ((kc4*(mu+lambda)) / (hx*hy));
	Kmatrix[7][19] = ((kc3*mu) / (hx*hx)) + ((kc3*(lambda+2*mu)) / (hy*hy)) - ((kc1*mu) / (hz*hz));
	Kmatrix[7][20] = ((kc2*(mu-lambda)) / (hy*hz));
	Kmatrix[7][21] = ((kc4*(lambda-mu)) / (hx*hy));
	Kmatrix[7][22] = ((kc5*mu) / (hx*hx)) - ((kc3*(lambda+2*mu)) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[7][23] = -((kc2*(lambda+mu)) / (hy*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[8][0] = ((kc4*(mu-lambda)) / (hx*hz));
	Kmatrix[8][1] = -((kc4*(lambda-mu)) / (hy*hz));
	Kmatrix[8][2] = -((kc3*mu) / (hx*hx)) - ((kc3*mu) / (hy*hy)) + ((kc5*(lambda+2*mu)) / (hz*hz));
	Kmatrix[8][3] = ((kc2*(mu-lambda)) / (hx*hz));
	Kmatrix[8][4] = ((kc4*(mu+lambda)) / (hy*hz));
	Kmatrix[8][5] = -((kc1*mu) / (hx*hx)) + ((kc3*mu) / (hy*hy)) + ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[8][6] = ((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[8][7] = ((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[8][8] = ((kc1*mu) / (hx*hx)) + ((kc1*mu) / (hy*hy)) + ((kc1*(lambda+2*mu)) / (hz*hz));
	Kmatrix[8][9] = ((kc4*(mu+lambda)) / (hx*hz));
	Kmatrix[8][10] = ((kc2*(mu-lambda)) / (hy*hz));
	Kmatrix[8][11] = ((kc3*mu) / (hx*hx)) - ((kc1*mu) / (hy*hy)) + ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[8][12] = -((kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[8][13] = -((kc4*(mu+lambda)) / (hy*hz));
	Kmatrix[8][14] = -((kc5*mu) / (hx*hx)) - ((kc5*mu) / (hy*hy)) - ((kc5*(lambda+2*mu)) / (hz*hz));
	Kmatrix[8][15] = -((kc2*(mu+lambda)) / (hx*hz));
	Kmatrix[8][16] = ((kc4*(lambda-mu)) / (hy*hz));
	Kmatrix[8][17] = -((kc3*mu) / (hx*hx)) + ((kc5*mu) / (hy*hy)) - ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[8][18] = ((kc2*(lambda-mu)) / (hx*hz));
	Kmatrix[8][19] = ((kc2*(lambda-mu)) / (hy*hz));
	Kmatrix[8][20] = ((kc3*mu) / (hx*hx)) + ((kc3*mu) / (hy*hy)) - ((kc1*(lambda+2*mu)) / (hz*hz));
	Kmatrix[8][21] = ((kc4*(lambda-mu)) / (hx*hz));
	Kmatrix[8][22] = -((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[8][23] = ((kc5*mu) / (hx*hx)) - ((kc3*mu) / (hy*hy)) - ((kc3*(lambda+2*mu)) / (hz*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[9][0] = -((kc1*(lambda+2*mu)) / (hx*hx)) + ((kc3*mu) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[9][1] = ((kc2*(mu-lambda)) / (hx*hy));
	Kmatrix[9][2] = ((kc2*(lambda-mu)) / (hx*hz));
	Kmatrix[9][3] = -((kc3*(lambda+2*mu)) / (hx*hx)) - ((kc3*mu) / (hy*hy)) + ((kc5*mu) / (hz*hz));
	Kmatrix[9][4] = ((kc2*(lambda+mu)) / (hx*hy));
	Kmatrix[9][5] = ((kc4*(lambda-mu)) / (hx*hz));
	Kmatrix[9][6] = ((kc3*(lambda+2*mu)) / (hx*hx)) - ((kc1*mu) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[9][7] = ((kc2*(lambda-mu)) / (hx*hy));
	Kmatrix[9][8] = ((kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[9][9] = ((kc1*(lambda+2*mu)) / (hx*hx)) + ((kc1*mu) / (hy*hy)) + ((kc1*mu) / (hz*hz));
	Kmatrix[9][10] = -((kc2*(lambda+mu)) / (hx*hy));
	Kmatrix[9][11] = ((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[9][12] = -((kc3*(lambda+2*mu)) / (hx*hx)) + ((kc5*mu) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[9][13] = ((kc4*(mu-lambda)) / (hx*hy));
	Kmatrix[9][14] = -((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[9][15] = -((kc5*(lambda+2*mu)) / (hx*hx)) - ((kc5*mu) / (hy*hy)) - ((kc5*mu) / (hz*hz));
	Kmatrix[9][16] = ((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[9][17] = -((kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[9][18] = ((kc5*(lambda+2*mu)) / (hx*hx)) - ((kc3*mu) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[9][19] = ((kc4*(lambda-mu)) / (hx*hy));
	Kmatrix[9][20] = ((kc4*(mu-lambda)) / (hx*hz));
	Kmatrix[9][21] = ((kc3*(lambda+2*mu)) / (hx*hx)) + ((kc3*mu) / (hy*hy)) - ((kc1*mu) / (hz*hz));
	Kmatrix[9][22] = -((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[9][23] = ((kc2*(mu-lambda)) / (hx*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[10][0] = ((kc2*(lambda-mu)) / (hx*hy));
	Kmatrix[10][1] = -((kc1*mu) / (hx*hx)) + ((kc3*(lambda+2*mu)) / (hy*hy))+((kc3*mu)/(hz*hz));
	Kmatrix[10][2] = -((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[10][3] = ((kc2*(lambda+mu)) / (hx*hy));
	Kmatrix[10][4] = -((kc3*mu) / (hx*hx)) - ((kc3*(lambda+2*mu)) / (hy*hy)) + ((kc5*mu) / (hz*hz));
	Kmatrix[10][5] = ((kc4*(mu-lambda)) / (hy*hz));
	Kmatrix[10][6] = ((kc2*(mu-lambda)) / (hx*hy));
	Kmatrix[10][7] = ((kc3*mu) / (hx*hx)) - ((kc1*(lambda+2*mu)) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[10][8] = ((kc2*(mu-lambda)) / (hy*hz));
	Kmatrix[10][9] = -((kc2*(mu+lambda)) / (hx*hy));
	Kmatrix[10][10] = ((kc1*mu) / (hx*hx)) + ((kc1*(lambda+2*mu)) / (hy*hy)) + ((kc1*mu) / (hz*hz));
	Kmatrix[10][11] = -((kc2*(mu+lambda)) / (hy*hz));
	Kmatrix[10][12] = ((kc4*(lambda-mu)) / (hx*hy));
	Kmatrix[10][13] = -((kc3*mu) / (hx*hx)) + ((kc5*(lambda+2*mu)) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[10][14] = ((kc4*(lambda-mu)) / (hy*hz));
	Kmatrix[10][15] = ((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[10][16] = -((kc5*mu) / (hx*hx)) - ((kc5*(lambda+2*mu)) / (hy*hy)) - ((kc5*mu) / (hz*hz));
	Kmatrix[10][17] = ((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[10][18] = ((kc4*(mu-lambda)) / (hx*hy));
	Kmatrix[10][19] = ((kc5*mu) / (hx*hx)) - ((kc3*(lambda+2*mu)) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[10][20] = ((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[10][21] = -((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[10][22] = ((kc3*mu) / (hx*hx)) + ((kc3*(lambda+2*mu)) / (hy*hy)) - ((kc1*mu) / (hz*hz));
	Kmatrix[10][23] = ((kc2*(lambda-mu)) / (hy*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[11][0] = ((kc2*(mu-lambda)) / (hx*hz));
	Kmatrix[11][1] = -((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[11][2] = -((kc1*mu) / (hx*hx)) + ((kc3*mu) / (hy*hy)) + ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[11][3] = ((kc4*(mu-lambda)) / (hx*hz));
	Kmatrix[11][4] = ((kc4*(lambda-mu)) / (hy*hz));
	Kmatrix[11][5] = -((kc3*mu) / (hx*hx)) - ((kc3*mu) / (hy*hy)) + ((kc5*(lambda+2*mu)) / (hz*hz));
	Kmatrix[11][6] = ((kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[11][7] = ((kc2*(lambda-mu)) / (hy*hz));
	Kmatrix[11][8] = ((kc3*mu) / (hx*hx)) - ((kc1*mu) / (hy*hy)) + ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[11][9] = ((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[11][10] = -((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[11][11] = ((kc1*mu) / (hx*hx)) + ((kc1*mu) / (hy*hy)) + ((kc1*(lambda+2*mu)) / (hz*hz));
	Kmatrix[11][12] = -((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[11][13] = ((kc4*(mu-lambda)) / (hy*hz));
	Kmatrix[11][14] = -((kc3*mu) / (hx*hx)) + ((kc5*mu) / (hy*hy)) - ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[11][15] = -((kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[11][16] = ((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[11][17] = -((kc5*mu) / (hx*hx)) - ((kc5*mu) / (hy*hy)) - ((kc5*(lambda+2*mu)) / (hz*hz));
	Kmatrix[11][18] = ((kc4*(lambda-mu)) / (hx*hz));
	Kmatrix[11][19] = ((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[11][20] = ((kc5*mu) / (hx*hx)) - ((kc3*mu) / (hy*hy)) - ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[11][21] = ((kc2*(lambda-mu)) / (hx*hz));
	Kmatrix[11][22] = ((kc2*(mu-lambda)) / (hy*hz));
	Kmatrix[11][23] = ((kc3*mu) / (hx*hx)) + ((kc3*mu) / (hy*hy)) - ((kc1*(lambda+2*mu)) / (hz*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[12][0] = ((kc3*(lambda+2*mu)) / (hx*hx)) + ((kc3*mu) / (hy*hy)) - ((kc1*mu) / (hz*hz));
	Kmatrix[12][1] = ((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[12][2] = ((kc2*(mu-lambda)) / (hx*hz));
	Kmatrix[12][3] = ((kc5*(lambda+2*mu)) / (hx*hx)) - ((kc3*mu) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[12][4] = ((kc4*(mu-lambda)) / (hx*hy));
	Kmatrix[12][5] = ((kc4*(mu-lambda)) / (hx*hz));
	Kmatrix[12][6] = -((kc5*(lambda+2*mu)) / (hx*hx)) - ((kc5*mu) / (hy*hy)) - ((kc5*mu) / (hz*hz));
	Kmatrix[12][7] = -((kc4*(mu+lambda)) / (hx*hy));
	Kmatrix[12][8] = -((kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[12][9] = -((kc3*(lambda+2*mu)) / (hx*hx)) + ((kc5*mu) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[12][10] = ((kc4*(lambda-mu)) / (hx*hy));
	Kmatrix[12][11] = -((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[12][12] = ((kc1*(lambda+2*mu)) / (hx*hx)) + ((kc1*mu) / (hy*hy)) + ((kc1*mu) / (hz*hz));
	Kmatrix[12][13] = ((kc2*(lambda+mu)) / (hx*hy));
	Kmatrix[12][14] = ((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[12][15] = ((kc3*(lambda+2*mu)) / (hx*hx)) - ((kc1*mu) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[12][16] = ((kc2*(mu-lambda)) / (hx*hy));
	Kmatrix[12][17] = ((kc4*(mu+lambda)) / (hx*hz));
	Kmatrix[12][18] = -((kc3*(lambda+2*mu)) / (hx*hx)) - ((kc3*mu) / (hy*hy)) + ((kc5*mu) / (hz*hz));
	Kmatrix[12][19] = -((kc2*(lambda+mu)) / (hx*hy));
	Kmatrix[12][20] = ((kc4*(lambda-mu)) / (hx*hz));
	Kmatrix[12][21] = -((kc1*(lambda+2*mu)) / (hx*hx)) + ((kc3*mu) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[12][22] = ((kc2*(lambda-mu)) / (hx*hy));
	Kmatrix[12][23] = ((kc2*(lambda-mu)) / (hx*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[13][0] = ((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[13][1] = ((kc3*mu) / (hx*hx)) + ((kc3*(lambda+2*mu)) / (hy*hy))-((kc1*mu)/(hz*hz));
	Kmatrix[13][2] = ((kc2*(mu-lambda)) / (hy*hz));
	Kmatrix[13][3] = ((kc4*(lambda-mu)) / (hx*hy));
	Kmatrix[13][4] = ((kc5*mu) / (hx*hx)) - ((kc3*(lambda+2*mu)) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[13][5] = -((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[13][6] = -((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[13][7] = -((kc5*mu) / (hx*hx)) - ((kc5*(lambda+2*mu)) / (hy*hy)) - ((kc5*mu) / (hz*hz));
	Kmatrix[13][8] = -((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[13][9] = ((kc4*(mu-lambda)) / (hx*hy));
	Kmatrix[13][10] = -((kc3*mu) / (hx*hx)) + ((kc5*(lambda+2*mu)) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[13][11] = ((kc4*(mu-lambda)) / (hy*hz));
	Kmatrix[13][12] = ((kc2*(mu+lambda)) / (hx*hy));
	Kmatrix[13][13] = ((kc1*mu) / (hx*hx)) + ((kc1*(lambda+2*mu)) / (hy*hy)) + ((kc1*mu) / (hz*hz));
	Kmatrix[13][14] = ((kc2*(mu+lambda)) / (hy*hz));
	Kmatrix[13][15] = ((kc2*(lambda-mu)) / (hx*hy));
	Kmatrix[13][16] = ((kc3*mu) / (hx*hx)) - ((kc1*(lambda+2*mu)) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[13][17] = ((kc2*(lambda-mu)) / (hy*hz));
	Kmatrix[13][18] = -((kc2*(lambda+mu)) / (hx*hy));
	Kmatrix[13][19] = -((kc3*mu) / (hx*hx)) - ((kc3*(lambda+2*mu)) / (hy*hy)) + ((kc5*mu) / (hz*hz));
	Kmatrix[13][20] = ((kc4*(lambda-mu)) / (hy*hz));
	Kmatrix[13][21] = ((kc2*(mu-lambda)) / (hx*hy));
	Kmatrix[13][22] = -((kc1*mu) / (hx*hx)) + ((kc3*(lambda+2*mu)) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[13][23] = ((kc4*(lambda+mu)) / (hy*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[14][0] = ((kc2*(lambda-mu)) / (hx*hz));
	Kmatrix[14][1] = ((kc2*(lambda-mu)) / (hy*hz));
	Kmatrix[14][2] = ((kc3*mu) / (hx*hx)) + ((kc3*mu) / (hy*hy)) - ((kc1*(lambda+2*mu)) / (hz*hz));
	Kmatrix[14][3] = ((kc4*(lambda-mu)) / (hx*hz));
	Kmatrix[14][4] = -((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[14][5] = ((kc5*mu) / (hx*hx)) - ((kc3*mu) / (hy*hy)) - ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[14][6] = -((kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[14][7] = -((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[14][8] = -((kc5*mu) / (hx*hx)) - ((kc5*mu) / (hy*hy)) - ((kc5*(lambda+2*mu)) / (hz*hz));
	Kmatrix[14][9] = -((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[14][10] = ((kc4*(lambda-mu)) / (hy*hz));
	Kmatrix[14][11] = -((kc3*mu) / (hx*hx)) + ((kc5*mu) / (hy*hy)) - ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[14][12] = ((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[14][13] = ((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[14][14] = ((kc1*mu) / (hx*hx)) + ((kc1*mu) / (hy*hy)) + ((kc1*(lambda+2*mu)) / (hz*hz));
	Kmatrix[14][15] = ((kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[14][16] = ((kc2*(mu-lambda)) / (hy*hz));
	Kmatrix[14][17] = ((kc3*mu) / (hx*hx)) - ((kc1*mu) / (hy*hy)) + ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[14][18] = ((kc4*(mu-lambda)) / (hx*hz));
	Kmatrix[14][19] = ((kc4*(mu-lambda)) / (hy*hz));
	Kmatrix[14][20] = -((kc3*mu) / (hx*hx)) - ((kc3*mu) / (hy*hy)) + ((kc5*(lambda+2*mu)) / (hz*hz));
	Kmatrix[14][21] = ((kc2*(mu-lambda)) / (hx*hz));
	Kmatrix[14][22] = ((kc4*(mu+lambda)) / (hy*hz));
	Kmatrix[14][23] = -((kc1*mu) / (hx*hx)) + ((kc3*mu) / (hy*hy)) + ((kc3*(lambda+2*mu)) / (hz*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[15][0] = ((kc5*(lambda+2*mu)) / (hx*hx)) - ((kc3*mu) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[15][1] = ((kc4*(lambda-mu)) / (hx*hy));
	Kmatrix[15][2] = ((kc4*(mu-lambda)) / (hx*hz));
	Kmatrix[15][3] = ((kc3*(lambda+2*mu)) / (hx*hx)) + ((kc3*mu) / (hy*hy)) - ((kc1*mu) / (hz*hz));
	Kmatrix[15][4] = -((kc4*(mu+lambda)) / (hx*hy));
	Kmatrix[15][5] = ((kc2*(mu-lambda)) / (hx*hz));
	Kmatrix[15][6] = -((kc3*(lambda+2*mu)) / (hx*hx)) + ((kc5*mu) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[15][7] = ((kc4*(mu-lambda)) / (hx*hy));
	Kmatrix[15][8] = -((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[15][9] = -((kc5*(lambda+2*mu)) / (hx*hx)) - ((kc5*mu) / (hy*hy)) - ((kc5*mu) / (hz*hz));
	Kmatrix[15][10] = ((kc4*(mu+lambda)) / (hx*hy));
	Kmatrix[15][11] = -((kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[15][12] = ((kc3*(lambda+2*mu)) / (hx*hx)) - ((kc1*mu) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[15][13] = ((kc2*(lambda-mu)) / (hx*hy));
	Kmatrix[15][14] = ((kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[15][15] = ((kc1*(lambda+2*mu)) / (hx*hx)) + ((kc1*mu) / (hy*hy)) + ((kc1*mu) / (hz*hz));
	Kmatrix[15][16] = -((kc2*(lambda+mu)) / (hx*hy));
	Kmatrix[15][17] = ((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[15][18] = -((kc1*(lambda+2*mu)) / (hx*hx)) + ((kc3*mu) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[15][19] = ((kc2*(mu-lambda)) / (hx*hy));
	Kmatrix[15][20] = ((kc2*(lambda-mu)) / (hx*hz));
	Kmatrix[15][21] = -((kc3*(lambda+2*mu)) / (hx*hx)) - ((kc3*mu) / (hy*hy)) + ((kc5*mu) / (hz*hz));
	Kmatrix[15][22] = ((kc2*(lambda+mu)) / (hx*hy));
	Kmatrix[15][23] = ((kc4*(lambda-mu)) / (hx*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[16][0] = ((kc4*(mu-lambda)) / (hx*hy));
	Kmatrix[16][1] = ((kc5*mu) / (hx*hx)) - ((kc3*(lambda+2*mu)) / (hy*hy))-((kc3*mu)/(hz*hz));
	Kmatrix[16][2] = ((kc2*(mu+lambda)) / (hy*hz));
	Kmatrix[16][3] = -((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[16][4] = ((kc3*mu) / (hx*hx)) + ((kc3*(lambda+2*mu)) / (hy*hy)) - ((kc1*mu) / (hz*hz));
	Kmatrix[16][5] = ((kc2*(lambda-mu)) / (hy*hz));
	Kmatrix[16][6] = ((kc4*(lambda-mu)) / (hx*hy));
	Kmatrix[16][7] = -((kc3*mu) / (hx*hx)) + ((kc5*(lambda+2*mu)) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[16][8] = ((kc4*(lambda-mu)) / (hy*hz));
	Kmatrix[16][9] = ((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[16][10] = -((kc5*mu) / (hx*hx)) - ((kc5*(lambda+2*mu)) / (hy*hy)) - ((kc5*mu) / (hz*hz));
	Kmatrix[16][11] = ((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[16][12] = ((kc2*(mu-lambda)) / (hx*hy));
	Kmatrix[16][13] = ((kc3*mu) / (hx*hx)) - ((kc1*(lambda+2*mu)) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[16][14] = ((kc2*(mu-lambda)) / (hy*hz));
	Kmatrix[16][15] = -((kc2*(mu+lambda)) / (hx*hy));
	Kmatrix[16][16] = ((kc1*mu) / (hx*hx)) + ((kc1*(lambda+2*mu)) / (hy*hy)) + ((kc1*mu) / (hz*hz));
	Kmatrix[16][17] = -((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[16][18] = ((kc2*(lambda-mu)) / (hx*hy));
	Kmatrix[16][19] = -((kc1*mu) / (hx*hx)) + ((kc3*(lambda+2*mu)) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[16][20] = -((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[16][21] = ((kc2*(lambda+mu)) / (hx*hy));
	Kmatrix[16][22] = -((kc3*mu) / (hx*hx)) - ((kc3*(lambda+2*mu)) / (hy*hy)) + ((kc5*mu) / (hz*hz));
	Kmatrix[16][23] = ((kc4*(mu-lambda)) / (hy*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[17][0] = ((kc4*(lambda-mu)) / (hx*hz));
	Kmatrix[17][1] = ((kc2*(lambda+mu)) / (hz*hy));
	Kmatrix[17][2] = ((kc5*mu) / (hx*hx)) - ((kc3*mu) / (hy*hy)) - ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[17][3] = ((kc2*(lambda-mu)) / (hx*hz));
	Kmatrix[17][4] = ((kc2*(mu-lambda)) / (hy*hz));
	Kmatrix[17][5] = ((kc3*mu) / (hx*hx)) + ((kc3*mu) / (hy*hy)) - ((kc1*(lambda+2*mu)) / (hz*hz));
	Kmatrix[17][6] = -((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[17][7] = ((kc4*(mu-lambda)) / (hy*hz));
	Kmatrix[17][8] = -((kc3*mu) / (hx*hx)) + ((kc5*mu) / (hy*hy)) - ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[17][9] = -((kc4*(mu+lambda)) / (hx*hz));
	Kmatrix[17][10] = ((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[17][11] = -((kc5*mu) / (hx*hx)) - ((kc5*mu) / (hy*hy)) - ((kc5*(lambda+2*mu)) / (hz*hz));
	Kmatrix[17][12] = ((kc4*(mu+lambda)) / (hx*hz));
	Kmatrix[17][13] = ((kc2*(lambda-mu)) / (hy*hz));
	Kmatrix[17][14] = ((kc3*mu) / (hx*hx)) - ((kc1*mu) / (hy*hy)) + ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[17][15] = ((kc2*(mu+lambda)) / (hx*hz));
	Kmatrix[17][16] = -((kc2*(mu+lambda)) / (hy*hz));
	Kmatrix[17][17] = ((kc1*mu) / (hx*hx)) + ((kc1*mu) / (hy*hy)) + ((kc1*(lambda+2*mu)) / (hz*hz));
	Kmatrix[17][18] = ((kc2*(mu-lambda)) / (hx*hz));
	Kmatrix[17][19] = -((kc4*(mu+lambda)) / (hy*hz));
	Kmatrix[17][20] = -((kc1*mu) / (hx*hx)) + ((kc3*mu) / (hy*hy)) + ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[17][21] = ((kc4*(mu-lambda)) / (hx*hz));
	Kmatrix[17][22] = ((kc4*(lambda-mu)) / (hy*hz));
	Kmatrix[17][23] = -((kc3*mu) / (hx*hx)) - ((kc3*mu) / (hy*hy)) + ((kc5*(lambda+2*mu)) / (hz*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[18][0] = -((kc5*(lambda+2*mu)) / (hx*hx)) - ((kc5*mu) / (hy*hy)) - ((kc5*mu) / (hz*hz));
	Kmatrix[18][1] = -((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[18][2] = ((kc4*(mu+lambda)) / (hx*hz));
	Kmatrix[18][3] = -((kc3*(lambda+2*mu)) / (hx*hx)) + ((kc5*mu) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[18][4] = ((kc4*(lambda-mu)) / (hx*hy));
	Kmatrix[18][5] = ((kc2*(mu+lambda)) / (hx*hz));
	Kmatrix[18][6] = ((kc3*(lambda+2*mu)) / (hx*hx)) + ((kc3*mu) / (hy*hy)) - ((kc1*mu) / (hz*hz));
	Kmatrix[18][7] = ((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[18][8] = ((kc2*(lambda-mu)) / (hx*hz));
	Kmatrix[18][9] = ((kc5*(lambda+2*mu)) / (hx*hx)) - ((kc3*mu) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[18][10] = ((kc4*(mu-lambda)) / (hx*hy));
	Kmatrix[18][11] = ((kc4*(lambda-mu)) / (hx*hz));
	Kmatrix[18][12] = -((kc3*(lambda+2*mu)) / (hx*hx)) - ((kc3*mu) / (hy*hy)) + ((kc5*mu) / (hz*hz));
	Kmatrix[18][13] = -((kc2*(mu+lambda)) / (hx*hy));
	Kmatrix[18][14] = ((kc4*(mu-lambda)) / (hx*hz));
	Kmatrix[18][15] = -((kc1*(lambda+2*mu)) / (hx*hx)) + ((kc3*mu) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[18][16] = ((kc2*(lambda-mu)) / (hx*hy));
	Kmatrix[18][17] = ((kc2*(mu-lambda)) / (hx*hz));
	Kmatrix[18][18] = ((kc1*(lambda+2*mu)) / (hx*hx)) + ((kc1*mu) / (hy*hy)) + ((kc1*mu) / (hz*hz));
	Kmatrix[18][19] = ((kc2*(mu+lambda)) / (hx*hy));
	Kmatrix[18][20] = -((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[18][21] = ((kc3*(lambda+2*mu)) / (hx*hx)) - ((kc1*mu) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[18][22] = ((kc2*(mu-lambda)) / (hx*hy));
	Kmatrix[18][23] = -((kc4*(lambda+mu)) / (hx*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[19][0] = -((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[19][1] = -((kc5*mu) / (hx*hx)) - ((kc5*(lambda+2*mu)) / (hy*hy))-((kc5*mu)/(hz*hz));
	Kmatrix[19][2] = ((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[19][3] = ((kc4*(mu-lambda)) / (hx*hy));
	Kmatrix[19][4] = -((kc3*mu) / (hx*hx)) + ((kc5*(lambda+2*mu)) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[19][5] = ((kc4*(lambda-mu)) / (hy*hz));
	Kmatrix[19][6] = ((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[19][7] = ((kc3*mu) / (hx*hx)) + ((kc3*(lambda+2*mu)) / (hy*hy)) - ((kc1*mu) / (hz*hz));
	Kmatrix[19][8] = ((kc2*(lambda-mu)) / (hy*hz));
	Kmatrix[19][9] = ((kc4*(lambda-mu)) / (hx*hy));
	Kmatrix[19][10] = ((kc5*mu) / (hx*hx)) - ((kc3*(lambda+2*mu)) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[19][11] = ((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[19][12] = -((kc2*(lambda+mu)) / (hx*hy));
	Kmatrix[19][13] = -((kc3*mu) / (hx*hx)) - ((kc3*(lambda+2*mu)) / (hy*hy)) + ((kc5*mu) / (hz*hz));
	Kmatrix[19][14] = ((kc4*(mu-lambda)) / (hy*hz));
	Kmatrix[19][15] = ((kc2*(mu-lambda)) / (hx*hy));
	Kmatrix[19][16] = -((kc1*mu) / (hx*hx)) + ((kc3*(lambda+2*mu)) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[19][17] = -((kc4*(mu+lambda)) / (hy*hz));
	Kmatrix[19][18] = ((kc2*(mu+lambda)) / (hx*hy));
	Kmatrix[19][19] = ((kc1*mu) / (hx*hx)) + ((kc1*(lambda+2*mu)) / (hy*hy)) + ((kc1*mu) / (hz*hz));
	Kmatrix[19][20] = -((kc2*(mu+lambda)) / (hy*hz));
	Kmatrix[19][21] = ((kc2*(lambda-mu)) / (hx*hy));
	Kmatrix[19][22] = ((kc3*mu) / (hx*hx)) - ((kc1*(lambda+2*mu)) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[19][23] = ((kc2*(mu-lambda)) / (hy*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[20][0] = ((kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[20][1] = ((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[20][2] = -((kc5*mu) / (hx*hx)) - ((kc5*mu) / (hy*hy)) - ((kc5*(lambda+2*mu)) / (hz*hz));
	Kmatrix[20][3] = ((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[20][4] = ((kc4*(mu-lambda)) / (hy*hz));
	Kmatrix[20][5] = -((kc3*mu) / (hx*hx)) + ((kc5*mu) / (hy*hy)) - ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[20][6] = ((kc2*(mu-lambda)) / (hx*hz));
	Kmatrix[20][7] = ((kc2*(mu-lambda)) / (hy*hz));
	Kmatrix[20][8] = ((kc3*mu) / (hx*hx)) + ((kc3*mu) / (hy*hy)) - ((kc1*(lambda+2*mu)) / (hz*hz));
	Kmatrix[20][9] = ((kc4*(mu-lambda)) / (hx*hz));
	Kmatrix[20][10] = ((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[20][11] = ((kc5*mu) / (hx*hx)) - ((kc3*mu) / (hy*hy)) - ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[20][12] = ((kc4*(lambda-mu)) / (hx*hz));
	Kmatrix[20][13] = ((kc4*(lambda-mu)) / (hy*hz));
	Kmatrix[20][14] = -((kc3*mu) / (hx*hx)) - ((kc3*mu) / (hy*hy)) + ((kc5*(lambda+2*mu)) / (hz*hz));
	Kmatrix[20][15] = ((kc2*(lambda-mu)) / (hx*hz));
	Kmatrix[20][16] = -((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[20][17] = -((kc1*mu) / (hx*hx)) + ((kc3*mu) / (hy*hy)) + ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[20][18] = -((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[20][19] = -((kc2*(mu+lambda)) / (hy*hz));
	Kmatrix[20][20] = ((kc1*mu) / (hx*hx)) + ((kc1*mu) / (hy*hy)) + ((kc1*(lambda+2*mu)) / (hz*hz));
	Kmatrix[20][21] = -((kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[20][22] = ((kc2*(lambda-mu)) / (hy*hz));
	Kmatrix[20][23] = ((kc3*mu) / (hx*hx)) - ((kc1*mu) / (hy*hy)) + ((kc3*(lambda+2*mu)) / (hz*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[21][0] = -((kc3*(lambda+2*mu)) / (hx*hx)) + ((kc5*mu) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[21][1] = ((kc4*(mu-lambda)) / (hx*hy));
	Kmatrix[21][2] = ((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[21][3] = -((kc5*(lambda+2*mu)) / (hx*hx)) - ((kc5*mu) / (hy*hy)) - ((kc5*mu) / (hz*hz));
	Kmatrix[21][4] = ((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[21][5] = ((kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[21][6] = ((kc5*(lambda+2*mu)) / (hx*hx)) - ((kc3*mu) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[21][7] = ((kc4*(lambda-mu)) / (hx*hy));
	Kmatrix[21][8] = ((kc4*(lambda-mu)) / (hx*hz));
	Kmatrix[21][9] = ((kc3*(lambda+2*mu)) / (hx*hx)) + ((kc3*mu) / (hy*hy)) - ((kc1*mu) / (hz*hz));
	Kmatrix[21][10] = -((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[21][11] = ((kc2*(lambda-mu)) / (hx*hz));
	Kmatrix[21][12] = -((kc1*(lambda+2*mu)) / (hx*hx)) + ((kc3*mu) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[21][13] = ((kc2*(mu-lambda)) / (hx*hy));
	Kmatrix[21][14] = ((kc2*(mu-lambda)) / (hx*hz));
	Kmatrix[21][15] = -((kc3*(lambda+2*mu)) / (hx*hx)) - ((kc3*mu) / (hy*hy)) + ((kc5*mu) / (hz*hz));
	Kmatrix[21][16] = ((kc2*(lambda+mu)) / (hx*hy));
	Kmatrix[21][17] = ((kc4*(mu-lambda)) / (hx*hz));
	Kmatrix[21][18] = ((kc3*(lambda+2*mu)) / (hx*hx)) - ((kc1*mu) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[21][19] = ((kc2*(lambda-mu)) / (hx*hy));
	Kmatrix[21][20] = -((kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[21][21] = ((kc1*(lambda+2*mu)) / (hx*hx)) + ((kc1*mu) / (hy*hy)) + ((kc1*mu) / (hz*hz));
	Kmatrix[21][22] = -((kc2*(lambda+mu)) / (hx*hy));
	Kmatrix[21][23] = -((kc2*(lambda+mu)) / (hx*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[22][0] = ((kc4*(lambda-mu)) / (hx*hy));
	Kmatrix[22][1] = -((kc3*mu) / (hx*hx)) + ((kc5*(lambda+2*mu)) / (hy*hy))-((kc3*mu)/(hz*hz));
	Kmatrix[22][2] = ((kc4*(mu-lambda)) / (hy*hz));
	Kmatrix[22][3] = ((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[22][4] = -((kc5*mu) / (hx*hx)) - ((kc5*(lambda+2*mu)) / (hy*hy)) - ((kc5*mu) / (hz*hz));
	Kmatrix[22][5] = -((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[22][6] = ((kc4*(mu-lambda)) / (hx*hy));
	Kmatrix[22][7] = ((kc5*mu) / (hx*hx)) - ((kc3*(lambda+2*mu)) / (hy*hy)) - ((kc3*mu) / (hz*hz));
	Kmatrix[22][8] = -((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[22][9] = -((kc4*(lambda+mu)) / (hx*hy));
	Kmatrix[22][10] = ((kc3*mu) / (hx*hx)) + ((kc3*(lambda+2*mu)) / (hy*hy)) - ((kc1*mu) / (hz*hz));
	Kmatrix[22][11] = ((kc2*(mu-lambda)) / (hy*hz));
	Kmatrix[22][12] = ((kc2*(lambda-mu)) / (hx*hy));
	Kmatrix[22][13] = -((kc1*mu) / (hx*hx)) + ((kc3*(lambda+2*mu)) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[22][14] = ((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[22][15] = ((kc2*(lambda+mu)) / (hx*hy));
	Kmatrix[22][16] = -((kc3*mu) / (hx*hx)) - ((kc3*(lambda+2*mu)) / (hy*hy)) + ((kc5*mu) / (hz*hz));
	Kmatrix[22][17] = ((kc4*(lambda-mu)) / (hy*hz));
	Kmatrix[22][18] = ((kc2*(mu-lambda)) / (hx*hy));
	Kmatrix[22][19] = ((kc3*mu) / (hx*hx)) - ((kc1*(lambda+2*mu)) / (hy*hy)) + ((kc3*mu) / (hz*hz));
	Kmatrix[22][20] = ((kc2*(lambda-mu)) / (hy*hz));
	Kmatrix[22][21] = -((kc2*(lambda+mu)) / (hx*hy));
	Kmatrix[22][22] = ((kc1*mu) / (hx*hx)) + ((kc1*(lambda+2*mu)) / (hy*hy)) + ((kc1*mu) / (hz*hz));
	Kmatrix[22][23] = ((kc2*(lambda+mu)) / (hy*hz));
	//const doublereal kc1 = 0.05555557;
	//const doublereal kc2 = 0.04166666;
	//const doublereal kc3 = 0.02777777;
	//const doublereal kc4 = 0.02083333;
	//const doublereal kc5 = 0.01388888;
	Kmatrix[23][0] = ((kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[23][1] = ((kc4*(lambda-mu)) / (hy*hz));
	Kmatrix[23][2] = (-(kc3*mu) / (hx*hx)) + ((kc5*mu) / (hy*hy)) - ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[23][3] = ((kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[23][4] = (-(kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[23][5] = -((kc5*mu) / (hx*hx)) - ((kc5*mu) / (hy*hy)) - ((kc5*(lambda+2*mu)) / (hz*hz));
	Kmatrix[23][6] = ((kc4*(mu-lambda)) / (hx*hz));
	Kmatrix[23][7] = (-(kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[23][8] = ((kc5*mu) / (hx*hx)) - ((kc3*mu) / (hy*hy)) - ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[23][9] = ((kc2*(mu-lambda)) / (hx*hz));
	Kmatrix[23][10] = ((kc2*(lambda-mu)) / (hy*hz));
	Kmatrix[23][11] = ((kc3*mu) / (hx*hx)) + ((kc3*mu) / (hy*hy)) - ((kc1*(lambda+2*mu)) / (hz*hz));
	Kmatrix[23][12] = ((kc2*(lambda-mu)) / (hx*hz));
	Kmatrix[23][13] = ((kc4*(lambda+mu)) / (hy*hz));
	Kmatrix[23][14] = -((kc1*mu) / (hx*hx)) + ((kc3*mu) / (hy*hy)) + ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[23][15] = ((kc4*(lambda-mu)) / (hx*hz));
	Kmatrix[23][16] = ((kc4*(mu-lambda)) / (hy*hz));
	Kmatrix[23][17] = -((kc3*mu) / (hx*hx)) - ((kc3*mu) / (hy*hy)) + ((kc5*(lambda+2*mu)) / (hz*hz));
	Kmatrix[23][18] = (-(kc4*(lambda+mu)) / (hx*hz));
	Kmatrix[23][19] = ((kc2*(mu-lambda)) / (hy*hz));
	Kmatrix[23][20] = ((kc3*mu) / (hx*hx)) - ((kc1*mu) / (hy*hy)) + ((kc3*(lambda+2*mu)) / (hz*hz));
	Kmatrix[23][21] = (-(kc2*(lambda+mu)) / (hx*hz));
	Kmatrix[23][22] = ((kc2*(lambda+mu)) / (hy*hz));
	Kmatrix[23][23] = ((kc1*mu) / (hx*hx)) + ((kc1*mu) / (hy*hy)) + ((kc1*(lambda+2.0*mu)) / (hz*hz));

	volume3D(iP, nvtx, pa, hx, hy, hz);
	//printf("%e %e %e\n",hx,hy,hz);
	integer imc = -729;
	// �� �������� �������� �� ����� ������ �������������, ����� ����������� ���������.
	for (integer i_4 = 0; i_4 < 24; i_4++) {
		for (integer j_4 = 0; j_4 < 24; j_4++) {
			//228.51 ����������� ����������� ����������� ��� ����� ��������� � ANSYS ������ �� ����������.			
			// 0.01744841 - ����� ������� �� ��������������.
			Kmatrix[i_4][j_4] *= 0.017448*228.51*(hx*hy*hz);			
		}
		
	}

	errno_t err96;
	FILE* fp96=NULL;
	if (iP == imc) {
		fopen_s(&fp96, "maple8", "w");
	}

	if (iP == imc) {
		printf("mu=%e lambda=%e hx=%e hy=%e hz=%e\n",mu,lambda,hx,hy,hz);
		printf("multiplyer=%e\n", 0.017448*228.51*(hx*hy*hz)*1e-5);
		fprintf(fp96, "Cmatr:=matrix(24,24,[[");
	}

	for (integer i_4 = 0; i_4 < 24; i_4++) {
		for (integer j_4 = 0; j_4 < 24; j_4++) {
			//228.51 ����������� ����������� ����������� ��� ����� ��������� � ANSYS ������ �� ����������.
			if (iP == imc) {
				if (Kmatrix[i_4][j_4] > 0) {
					printf(" %1.2f ", 1E-5*Kmatrix[i_4][j_4] );
					fprintf(fp96," %1.2f, ", 1E-5*Kmatrix[i_4][j_4]);
				}
				else {
					printf("%1.2f ", 1E-5*Kmatrix[i_4][j_4] );
					fprintf(fp96, "%1.2f, ", 1E-5*Kmatrix[i_4][j_4]);
				}
				if (1E-5*fabs(Kmatrix[i_4][j_4] - Kmatrix[j_4][i_4])>1.0e-2) {
					printf("\n%e=%e %d %d\n", 1E-5*Kmatrix[i_4][j_4], 1E-5*Kmatrix[j_4][i_4], i_4, j_4);
					getchar();
				}
			}
		}
		if (iP == imc) {
			printf("\n");
			if (i_4 < 23) { fprintf(fp96,"],[\n"); }
			else { fprintf(fp96,"]]);"); }
		}
	}
	if (iP == imc) {
		getchar();
		fclose(fp96);
	}
	/*
	for (integer i_4 = 0; i_4 < 24; i_4++) {
		if (i_4 % 3 == 0) {
			// X
			for (integer j_4 = 0; j_4 < 24; j_4++) {
				Kmatrix[i_4][j_4] *= hx;
			}
		}
		if ((i_4+1) % 3 == 0) {
			// Y
			for (integer j_4 = 0; j_4 < 24; j_4++) {
				Kmatrix[i_4][j_4] *= hy;
			}
		}
		if ((i_4+2) % 3 == 0) {
			// X
			for (integer j_4 = 0; j_4 < 24; j_4++) {
				Kmatrix[i_4][j_4] *= hz;
			}
		}
	}
	*/
	
	doublereal** KP = new doublereal*[24];
	for (integer i_1 = 0; i_1 < 24; i_1++) KP[i_1] = new doublereal[24];

	// �������������.
	for (integer i_1 = 0; i_1 < 24; i_1++) {
		for (integer j_1 = 0; j_1 < 24; j_1++) {
			integer i_1new, j_1new;
			
			switch (i_1) {
			case 0:i_1new = 3*4;
				break;
			case 1:i_1new = 3*4+1;
				break;
			case 2:i_1new = 3*4+2;
				break;
			case 3:i_1new = 3*6;
				break;
			case 4:i_1new = 3*6+1;
				break;
			case 5:i_1new = 3*6+2;
				break;
			case 6:i_1new = 3*7;
				break;
			case 7:i_1new = 3*7+1;
				break;
			case 8:i_1new = 3*7+2;
				break;
			case 9:i_1new = 3*5;
				break;
			case 10:i_1new = 3*5+1;
				break;
			case 11:i_1new = 3*5+2;
				break;
			case 12:i_1new = 3*0;
				break;
			case 13:i_1new = 3*0+1;
				break;
			case 14:i_1new = 3*0+2;
				break;
			case 15:i_1new = 3*2;
				break;
			case 16:i_1new = 3*2+1;
				break;
			case 17:i_1new = 3*2+2;
				break;
			case 18:i_1new = 3*3;
				break;
			case 19:i_1new = 3*3+1;
				break;
			case 20:i_1new = 3*3+2;
				break;
			case 21:i_1new = 3*1;
				break;
			case 22:i_1new = 3*1+1;
				break;
			case 23:i_1new = 3*1+2;
				break;
			}
			switch (j_1) {
			case 0:j_1new = 3 * 4;
				break;
			case 1:j_1new = 3 * 4 + 1;
				break;
			case 2:j_1new = 3 * 4 + 2;
				break;
			case 3:j_1new = 3 * 6;
				break;
			case 4:j_1new = 3 * 6 + 1;
				break;
			case 5:j_1new = 3 * 6 + 2;
				break;
			case 6:j_1new = 3 * 7;
				break;
			case 7:j_1new = 3 * 7 + 1;
				break;
			case 8:j_1new = 3 * 7 + 2;
				break;
			case 9:j_1new = 3 * 5;
				break;
			case 10:j_1new = 3 * 5 + 1;
				break;
			case 11:j_1new = 3 * 5 + 2;
				break;
			case 12:j_1new = 3 * 0;
				break;
			case 13:j_1new = 3 * 0 + 1;
				break;
			case 14:j_1new = 3 * 0 + 2;
				break;
			case 15:j_1new = 3 * 2;
				break;
			case 16:j_1new = 3 * 2 + 1;
				break;
			case 17:j_1new = 3 * 2 + 2;
				break;
			case 18:j_1new = 3 * 3;
				break;
			case 19:j_1new = 3 * 3 + 1;
				break;
			case 20:j_1new = 3 * 3 + 2;
				break;
			case 21:j_1new = 3 * 1;
				break;
			case 22:j_1new = 3 * 1 + 1;
				break;
			case 23:j_1new = 3 * 1 + 2;
				break;
			}		
			
/*
// (6,7), (2,3)
switch (i_1) {
case 0:i_1new = 3 * 4;
	break;
case 1:i_1new = 3 * 4 + 1;
	break;
case 2:i_1new = 3 * 4 + 2;
	break;
case 3:i_1new = 3 * 7;
	break;
case 4:i_1new = 3 * 7 + 1;
	break;
case 5:i_1new = 3 * 7 + 2;
	break;
case 6:i_1new = 3 * 6;
	break;
case 7:i_1new = 3 * 6 + 1;
	break;
case 8:i_1new = 3 * 6 + 2;
	break;
case 9:i_1new = 3 * 5;
	break;
case 10:i_1new = 3 * 5 + 1;
	break;
case 11:i_1new = 3 * 5 + 2;
	break;
case 12:i_1new = 3 * 0;
	break;
case 13:i_1new = 3 * 0 + 1;
	break;
case 14:i_1new = 3 * 0 + 2;
	break;
case 15:i_1new = 3 * 3;
	break;
case 16:i_1new = 3 * 3 + 1;
	break;
case 17:i_1new = 3 * 3 + 2;
	break;
case 18:i_1new = 3 * 2;
	break;
case 19:i_1new = 3 * 2 + 1;
	break;
case 20:i_1new = 3 * 2 + 2;
	break;
case 21:i_1new = 3 * 1;
	break;
case 22:i_1new = 3 * 1 + 1;
	break;
case 23:i_1new = 3 * 1 + 2;
	break;
}
switch (j_1) {
case 0:j_1new = 3 * 4;
	break;
case 1:j_1new = 3 * 4 + 1;
	break;
case 2:j_1new = 3 * 4 + 2;
	break;
case 3:j_1new = 3 * 7;
	break;
case 4:j_1new = 3 * 7 + 1;
	break;
case 5:j_1new = 3 * 7 + 2;
	break;
case 6:j_1new = 3 * 6;
	break;
case 7:j_1new = 3 * 6 + 1;
	break;
case 8:j_1new = 3 * 6 + 2;
	break;
case 9:j_1new = 3 * 5;
	break;
case 10:j_1new = 3 * 5 + 1;
	break;
case 11:j_1new = 3 * 5 + 2;
	break;
case 12:j_1new = 3 * 0;
	break;
case 13:j_1new = 3 * 0 + 1;
	break;
case 14:j_1new = 3 * 0 + 2;
	break;
case 15:j_1new = 3 * 3;
	break;
case 16:j_1new = 3 * 3 + 1;
	break;
case 17:j_1new = 3 * 3 + 2;
	break;
case 18:j_1new = 3 * 2;
	break;
case 19:j_1new = 3 * 2 + 1;
	break;
case 20:j_1new = 3 * 2 + 2;
	break;
case 21:j_1new = 3 * 1;
	break;
case 22:j_1new = 3 * 1 + 1;
	break;
case 23:j_1new = 3 * 1 + 2;
	break;
}
*/
/*
switch (i_1) {
case 0:i_1new = 3 * 4;
	break;
case 1:i_1new = 3 * 4 + 1;
	break;
case 2:i_1new = 3 * 4 + 2;
	break;
case 3:i_1new = 3 * 6;
	break;
case 4:i_1new = 3 * 6 + 1;
	break;
case 5:i_1new = 3 * 6 + 2;
	break;
case 6:i_1new = 3 * 5;
	break;
case 7:i_1new = 3 * 5 + 1;
	break;
case 8:i_1new = 3 * 5 + 2;
	break;
case 9:i_1new = 3 * 7;
	break;
case 10:i_1new = 3 * 7 + 1;
	break;
case 11:i_1new = 3 * 7 + 2;
	break;
case 12:i_1new = 3 * 0;
	break;
case 13:i_1new = 3 * 0 + 1;
	break;
case 14:i_1new = 3 * 0 + 2;
	break;
case 15:i_1new = 3 * 2;
	break;
case 16:i_1new = 3 * 2 + 1;
	break;
case 17:i_1new = 3 * 2 + 2;
	break;
case 18:i_1new = 3 * 1;
	break;
case 19:i_1new = 3 * 1 + 1;
	break;
case 20:i_1new = 3 * 1 + 2;
	break;
case 21:i_1new = 3 * 3;
	break;
case 22:i_1new = 3 * 3 + 1;
	break;
case 23:i_1new = 3 * 3 + 2;
	break;
}
switch (j_1) {
case 0:j_1new = 3 * 4;
	break;
case 1:j_1new = 3 * 4 + 1;
	break;
case 2:j_1new = 3 * 4 + 2;
	break;
case 3:j_1new = 3 * 6;
	break;
case 4:j_1new = 3 * 6 + 1;
	break;
case 5:j_1new = 3 * 6 + 2;
	break;
case 6:j_1new = 3 * 5;
	break;
case 7:j_1new = 3 * 5 + 1;
	break;
case 8:j_1new = 3 * 5 + 2;
	break;
case 9:j_1new = 3 * 7;
	break;
case 10:j_1new = 3 * 7 + 1;
	break;
case 11:j_1new = 3 * 7 + 2;
	break;
case 12:j_1new = 3 * 0;
	break;
case 13:j_1new = 3 * 0 + 1;
	break;
case 14:j_1new = 3 * 0 + 2;
	break;
case 15:j_1new = 3 * 2;
	break;
case 16:j_1new = 3 * 2 + 1;
	break;
case 17:j_1new = 3 * 2 + 2;
	break;
case 18:j_1new = 3 * 1;
	break;
case 19:j_1new = 3 * 1 + 1;
	break;
case 20:j_1new = 3 * 1 + 2;
	break;
case 21:j_1new = 3 * 3;
	break;
case 22:j_1new = 3 * 3 + 1;
	break;
case 23:j_1new = 3 * 3 + 2;
	break;
}
*/

			KP[i_1new][j_1new] = Kmatrix[i_1][j_1];
		}
	}

	for (integer i_1 = 0; i_1 < 24; i_1++) {
		for (integer j_1 = 0; j_1 < 24; j_1++) {
			Kmatrix[i_1][j_1] = KP[i_1][j_1];
		}
	}

	for (integer i_1 = 0; i_1 < 24; i_1++) delete[] KP[i_1];
	delete[] KP;
	

} // ������ ��������� ������� Ƹ�������.

  // �������������� �������� ������ ��� �������� ��� ����������.
  // ���������� ������ ������ �� ��������� �����������:
  // (x1-xC)*(x-xC)+(y1-yC)*(y-yC)=R^2. ��� ��������� XY.
typedef struct TCylindricalSupport {
	integer iPlane;
	bool bactive; // ����� �� �������� Cylindrical Support ��������� �������.
	doublereal xC, yC, zC, Radius; // ����� ����������.
	doublereal x1, y1, z1; // ����� � ������� �������� �����������.
} CylindricalSupport;

  // ��������� ��������� ��������� ��������� Kmatrix � Tmatrix (selm, telm)
  // � ���������� ������� Smatrix � (���� bsecond_member_of_equation=true)
  // ������ �����.
  // 8.4.2017
void elembdSparse(integer ie, SIMPLESPARSE &sparseM, integer** nvtx,
	bool* &constr, doublereal* &rthdsd,
	doublereal** &Kmatrix, doublereal* &potent,
	bool bsecond_member_of_equation, CylindricalSupport* &cylsup,
	doublereal epsx, doublereal epsy, doublereal epsz) {

	// ������� ��������� ��������� S � T (selm � telm)
	// � ������������� ���������� ������� S � ������ �����

	const integer nve = 8;
	doublereal distancez = 0.0, distancex = 0.0;

	integer i, j; // �������� ����� for
	integer irow, icol; // ������, �������
	for (i = 0; i<nve; i++) {
		for (integer idirection1 = 0; idirection1 < 3; idirection1++) {
			irow = idirection1 + 3 * (nvtx[i][ie] - 1);

			// ������ ������������� �������������� ���������� ?
			if (constr[irow]) { // ��
				if (cylsup[irow].bactive) {
					switch (cylsup[irow].iPlane) {
					case XY:
						switch (idirection1) {
						case 0:
							addelmsimplesparse_Stress_clean_string(sparseM, irow);
							addelmsimplesparse_Stress(sparseM, (cylsup[irow].x1 - cylsup[irow].xC), irow, irow, false, false);
							addelmsimplesparse_Stress(sparseM, (cylsup[irow].y1 - cylsup[irow].yC), irow, irow+1, false, false);
							if (bsecond_member_of_equation) rthdsd[irow] = cylsup[irow].Radius*cylsup[irow].Radius+cylsup[irow].xC*(cylsup[irow].x1-cylsup[irow].xC)+ cylsup[irow].yC*(cylsup[irow].y1 - cylsup[irow].yC);
							break;
						case 1: 
							addelmsimplesparse_Stress_clean_string(sparseM, irow);
							addelmsimplesparse_Stress(sparseM, (cylsup[irow].x1 - cylsup[irow].xC), irow, irow-1, false, false);
							addelmsimplesparse_Stress(sparseM, (cylsup[irow].y1 - cylsup[irow].yC), irow, irow, false, false);
							if (bsecond_member_of_equation) rthdsd[irow] = cylsup[irow].Radius*cylsup[irow].Radius + cylsup[irow].xC*(cylsup[irow].x1 - cylsup[irow].xC) + cylsup[irow].yC*(cylsup[irow].y1 - cylsup[irow].yC);
							break;
						case 2:
							//setValueIMatrix(&sparseS, irow, irow, 1.0f);
							addelmsimplesparse_Stress(sparseM, 1.0f, irow, irow, true, true);
							if (bsecond_member_of_equation) rthdsd[irow] = potent[irow];
							break;
						}
						break;
					case XZ: 
						switch (idirection1) {
						case 0:
							addelmsimplesparse_Stress_clean_string(sparseM, irow);
							if (irow == 3) {
							//	printf("clean string=%d\n", irow);
								//getchar();
							}
							if (fabs(cylsup[irow].z1 - cylsup[irow].zC) < epsz) {
								distancez = epsz;
							}
							else {
								distancez = cylsup[irow].z1 - cylsup[irow].zC;
							}

							if (cylsup[irow].x1 - cylsup[irow].xC < epsx) {
								if (fabs(cylsup[irow].x1 - cylsup[irow].xC) < epsx) {
									//printf("irow=%d %e \n",irow, cylsup[irow].x1 - cylsup[irow].xC);
									//printf("xC=%e zC=%e x1=%e z1=%e R=%e\n", cylsup[irow].xC, cylsup[irow].zC, cylsup[irow].x1, cylsup[irow].z1, cylsup[irow].Radius);
									//getchar();
									addelmsimplesparse_Stress(sparseM, 1.0, irow, irow, false, false);
									addelmsimplesparse_Stress(sparseM, -(distancez)/(epsx), irow, irow + 2, false, false);
									if (bsecond_member_of_equation) rthdsd[irow] = (cylsup[irow].Radius*cylsup[irow].Radius + cylsup[irow].xC*(epsx) + cylsup[irow].zC*(distancez))/(epsx);
									//printf("ap=%e %e %e\n",1.0, -(distancez) / (epsx), rthdsd[irow]);
									//getchar();
								}
								else {
									addelmsimplesparse_Stress(sparseM, -(cylsup[irow].x1 - cylsup[irow].xC), irow, irow, false, false);
									addelmsimplesparse_Stress(sparseM, (distancez), irow, irow + 2, false, false);
									if (bsecond_member_of_equation) rthdsd[irow] = -cylsup[irow].Radius*cylsup[irow].Radius - cylsup[irow].xC*(cylsup[irow].x1 - cylsup[irow].xC) - cylsup[irow].zC*(distancez);
									//printf("ap=%e %e %e\n", -(cylsup[irow].x1 - cylsup[irow].xC), (distancez), rthdsd[irow]);
									//getchar();
								}
							}
							else {
								addelmsimplesparse_Stress(sparseM, (cylsup[irow].x1 - cylsup[irow].xC), irow, irow, false, false);
								addelmsimplesparse_Stress(sparseM, -(distancez), irow, irow + 2, false, false);
								if (bsecond_member_of_equation) rthdsd[irow] = cylsup[irow].Radius*cylsup[irow].Radius + cylsup[irow].xC*(cylsup[irow].x1 - cylsup[irow].xC) + cylsup[irow].zC*(distancez);
								//printf("ap=%e %e %e\n", (cylsup[irow].x1 - cylsup[irow].xC), -(distancez), rthdsd[irow]);
								//getchar();
							}
							break;
						case 1:
							//setValueIMatrix(&sparseS, irow, irow, 1.0f);
							addelmsimplesparse_Stress(sparseM, 1.0f, irow, irow, true, true);
							if (bsecond_member_of_equation) rthdsd[irow] = potent[irow];
							break;
						case 2:
							addelmsimplesparse_Stress_clean_string(sparseM, irow);
							if (fabs(cylsup[irow].x1 - cylsup[irow].xC) < epsx) {
								distancex = epsx;
							}
							else {
								distancex = cylsup[irow].x1 - cylsup[irow].xC;
							}
							if ((cylsup[irow].z1 - cylsup[irow].zC) < epsz) {
								if (fabs(cylsup[irow].z1 - cylsup[irow].zC) < epsz) {
									//printf("irow=%d %e \n", irow, cylsup[irow].z1 - cylsup[irow].zC);
									//printf("xC=%e zC=%e x1=%e z1=%e R=%e\n", cylsup[irow].xC, cylsup[irow].zC, cylsup[irow].x1, cylsup[irow].z1, cylsup[irow].Radius);

									//getchar();
									addelmsimplesparse_Stress(sparseM, -(distancex)/(epsz), irow, irow - 2, false, false);
									addelmsimplesparse_Stress(sparseM,+1.0 , irow, irow, false, false);
									if (bsecond_member_of_equation) rthdsd[irow] = -(cylsup[irow].Radius*cylsup[irow].Radius + cylsup[irow].xC*(distancex) + cylsup[irow].zC*(epsz))/(epsz);
									//printf("ap=%e %e %e\n", 1.0,-(distancex) / (epsz), rthdsd[irow]);
									//getchar();
								}
								else {
									addelmsimplesparse_Stress(sparseM, (distancex), irow, irow - 2, false, false);
									addelmsimplesparse_Stress(sparseM, -(cylsup[irow].z1 - cylsup[irow].zC), irow, irow, false, false);
									if (bsecond_member_of_equation) rthdsd[irow] = cylsup[irow].Radius*cylsup[irow].Radius + cylsup[irow].xC*(distancex) + cylsup[irow].zC*(cylsup[irow].z1 - cylsup[irow].zC);
									//printf("ap=%e %e %e\n", -(cylsup[irow].z1 - cylsup[irow].zC), (distancex), rthdsd[irow]);
									//getchar();
								}
							}
							else {
								addelmsimplesparse_Stress(sparseM, -(distancex), irow, irow - 2, false, false);
								addelmsimplesparse_Stress(sparseM, +(cylsup[irow].z1 - cylsup[irow].zC), irow, irow, false, false);
								if (bsecond_member_of_equation) rthdsd[irow] = -cylsup[irow].Radius*cylsup[irow].Radius + cylsup[irow].xC*(distancex) + cylsup[irow].zC*(cylsup[irow].z1 - cylsup[irow].zC);
								//printf("ap=%e %e %e\n", (cylsup[irow].z1 - cylsup[irow].zC), -(distancex), rthdsd[irow]);
								//getchar();
							}
							break;
						}
						break;
					case YZ:
						switch (idirection1) {
						case 0:
							//setValueIMatrix(&sparseS, irow, irow, 1.0f);
							addelmsimplesparse_Stress(sparseM, 1.0f, irow, irow, true, true);
							if (bsecond_member_of_equation) rthdsd[irow] = potent[irow];
							break;
						case 1:
							addelmsimplesparse_Stress_clean_string(sparseM, irow);
							addelmsimplesparse_Stress(sparseM, (cylsup[irow].y1 - cylsup[irow].yC), irow, irow, false, false);
							addelmsimplesparse_Stress(sparseM, (cylsup[irow].z1 - cylsup[irow].zC), irow, irow + 1, false, false);
							if (bsecond_member_of_equation) rthdsd[irow] = cylsup[irow].Radius*cylsup[irow].Radius + cylsup[irow].yC*(cylsup[irow].y1 - cylsup[irow].yC) + cylsup[irow].zC*(cylsup[irow].z1 - cylsup[irow].zC);
							break;
						case 2:
							addelmsimplesparse_Stress_clean_string(sparseM, irow);
							addelmsimplesparse_Stress(sparseM, (cylsup[irow].y1 - cylsup[irow].yC), irow, irow - 1, false, false);
							addelmsimplesparse_Stress(sparseM, (cylsup[irow].z1 - cylsup[irow].zC), irow, irow, false, false);
							if (bsecond_member_of_equation) rthdsd[irow] = cylsup[irow].Radius*cylsup[irow].Radius + cylsup[irow].yC*(cylsup[irow].y1 - cylsup[irow].yC) + cylsup[irow].zC*(cylsup[irow].z1 - cylsup[irow].zC);
							break;
						}
						break;
					}
				}
				else {
					//setValueIMatrix(&sparseS, irow, irow, 1.0f);
					addelmsimplesparse_Stress(sparseM, 1.0f, irow, irow, true, true);
					if (bsecond_member_of_equation) rthdsd[irow] = potent[irow];
				}
			}
			else { // ���
				   // ���, ��������� ����������, �������� nve-��������
				for (j = 0; j < nve; j++) {
					for (integer idirection2 = 0; idirection2 < 3; idirection2++) {
						icol = idirection2 + 3 * (nvtx[j][ie] - 1);

						// ������� ������������� �������������� ���������� ?
						if (constr[icol]) { // ��
											// ����� ���������� ������ ������ �����
											//if (bsecond_member_of_equation) rthdsd[irow] += /*Kmatrix[idirection1 + 3 * i][idirection2 + 3 * j] * source[ie]*/ - selm[i][j] * potent[icol];
							
							if (cylsup[icol].bactive) {
								switch (cylsup[icol].iPlane) {
								case XY :
									switch (idirection2) {
									case 0: addelmsimplesparse_Stress(sparseM, Kmatrix[idirection1 + 3 * i][idirection2 + 3 * j], irow, icol, false, false);
										break;
									case 1: addelmsimplesparse_Stress(sparseM, Kmatrix[idirection1 + 3 * i][idirection2 + 3 * j], irow, icol, false, false);
										break;
									case 2:
										break;
									}
									break;
								case XZ :
									switch (idirection2) {
									case 0: addelmsimplesparse_Stress(sparseM, Kmatrix[idirection1 + 3 * i][idirection2 + 3 * j], irow, icol, false, false);
										break;
									case 1:
										break;
									case 2: addelmsimplesparse_Stress(sparseM, Kmatrix[idirection1 + 3 * i][idirection2 + 3 * j], irow, icol, false, false);
										break;
									}
									break;
								case YZ :
									switch (idirection2) {
									case 0:
										break;
									case 1: addelmsimplesparse_Stress(sparseM, Kmatrix[idirection1 + 3 * i][idirection2 + 3 * j], irow, icol, false, false);
										break;
									case 2: addelmsimplesparse_Stress(sparseM, Kmatrix[idirection1 + 3 * i][idirection2 + 3 * j], irow, icol, false, false);
										break;
									}
									break;
								}
						    }
							
							
						}
						else { // ���
							   // ����� ���������� ���������� ������� S � ������ �����
							   //addValueIMatrix(&sparseS, irow, icol, Kmatrix[idirection1 + 3 * i][idirection2 + 3 * j]);
							addelmsimplesparse_Stress(sparseM, Kmatrix[idirection1 + 3 * i][idirection2 + 3 * j], irow, icol, false, false);
							//if (bsecond_member_of_equation) rthdsd[irow] += telm[i][j] * source[ie];
						}
					}
				}
			}
		}
	}
} //elembdSparse(ie)


// ��������� ��������� ��������� ��������� Kmatrix � Tmatrix (selm, telm)
// � ���������� ������� Smatrix � (���� bsecond_member_of_equation=true)
// ������ �����.
// 8.4.2017
void elembdSparse_noCylindricalSupport(integer ie, SIMPLESPARSE &sparseM, integer** nvtx,
	bool* &constr,	doublereal* &rthdsd, 
	doublereal** &Kmatrix, doublereal* &potent,
	bool bsecond_member_of_equation,
	CylindricalSupport* &cylsup,  doublereal epsx, doublereal epsy, doublereal epsz) {
	// ������� ��������� ��������� S � T (selm � telm)
	// � ������������� ���������� ������� S � ������ �����

	const integer nve = 8;

	integer i, j; // �������� ����� for
	integer irow, icol; // ������, �������
	for (i = 0; i<nve; i++) {
		for (integer idirection1 = 0; idirection1 < 3; idirection1++) {
			irow = idirection1 + 3*(nvtx[i][ie] - 1);

			// ������ ������������� �������������� ���������� ?
			if (constr[irow]) { // ��
				//setValueIMatrix(&sparseS, irow, irow, 1.0f);
				addelmsimplesparse_Stress(sparseM, 1.0f, irow, irow, true,true);
				if (bsecond_member_of_equation) rthdsd[irow] = potent[irow];
			}
			else { // ���
				   // ���, ��������� ����������, �������� nve-��������
				for (j = 0; j < nve; j++) {
					for (integer idirection2 = 0; idirection2 < 3; idirection2++) {
						icol = idirection2 + 3*(nvtx[j][ie] - 1);

						// ������� ������������� �������������� ���������� ?
						if (constr[icol]) { // ��
											// ����� ���������� ������ ������ �����
							//if (bsecond_member_of_equation) rthdsd[irow] += /*Kmatrix[idirection1 + 3 * i][idirection2 + 3 * j] * source[ie]*/ - selm[i][j] * potent[icol];
						}
						else { // ���
							   // ����� ���������� ���������� ������� S � ������ �����
							//addValueIMatrix(&sparseS, irow, icol, Kmatrix[idirection1 + 3 * i][idirection2 + 3 * j]);
							addelmsimplesparse_Stress(sparseM, Kmatrix[idirection1 + 3 * i][idirection2 + 3 * j], irow, icol, false,false);
							//if (bsecond_member_of_equation) rthdsd[irow] += telm[i][j] * source[ie];
						}
					}
				}
			}
		}
	}
} //elembdSparse_noCylindricalSupport(ie)


  // ��������� ��������� ��������� ��������� Kmatrix � Tmatrix (selm, telm)
  // � ���������� ������� Smatrix � (���� bsecond_member_of_equation=true)
  // ������ �����.
  // 8.4.2017
void elembdSparse2(integer ie, IMatrix &sparseS, integer** nvtx,
	bool* &constr, doublereal* &rthdsd,
	doublereal** &Kmatrix, doublereal* &potent,
	bool bsecond_member_of_equation) {
	// ������� ��������� ��������� S � T (selm � telm)
	// � ������������� ���������� ������� S � ������ �����

	const integer nve = 8;

	integer i, j; // �������� ����� for
	integer irow, icol; // ������, �������
	for (i = 0; i<nve; i++) {
		for (integer idirection1 = 0; idirection1 < 3; idirection1++) {
			irow = idirection1 + 3 * (nvtx[i][ie] - 1);

			// ������ ������������� �������������� ���������� ?
			if (constr[irow]) { // ��
				setValueIMatrix(&sparseS, irow, irow, 1.0f);
				//addelmsimplesparse_Stress(sparseM, 1.0f, irow, irow, true, true);
				if (bsecond_member_of_equation) rthdsd[irow] = potent[irow];
			}
			else { // ���
				   // ���, ��������� ����������, �������� nve-��������
				for (j = 0; j < nve; j++) {
					for (integer idirection2 = 0; idirection2 < 3; idirection2++) {
						icol = idirection2 + 3 * (nvtx[j][ie] - 1);

						// ������� ������������� �������������� ���������� ?
						if (constr[icol]) { // ��
											// ����� ���������� ������ ������ �����
											//if (bsecond_member_of_equation) rthdsd[irow] += /*Kmatrix[idirection1 + 3 * i][idirection2 + 3 * j] * source[ie]*/ - selm[i][j] * potent[icol];
						}
						else { // ���
							   // ����� ���������� ���������� ������� S � ������ �����
							 addValueIMatrix(&sparseS, irow, icol, Kmatrix[idirection1 + 3 * i][idirection2 + 3 * j]);
							//addelmsimplesparse_Stress(sparseM, Kmatrix[idirection1 + 3 * i][idirection2 + 3 * j], irow, icol, false, false);
							//if (bsecond_member_of_equation) rthdsd[irow] += telm[i][j] * source[ie];
						}
					}
				}
			}
		}
	}
} //elembdSparse(ie)

  

  // ��������� ��������� ��������� ��������� Kmatrix � Tmatrix (selm, telm)
  // � ���������� ������� Smatrix � (���� bsecond_member_of_equation=true)
  // ������ �����. ��� ��������� �������������.
  // 19.05.2018
void elembdSparse3(integer ie, IMatrix &sparseS, integer** nvtx,
	bool* &constr, doublereal* &rthdsd,
	doublereal** &Kmatrix, doublereal* &potent,
	bool bsecond_member_of_equation) {
	// ������� ��������� ��������� S � T (selm � telm)
	// � ������������� ���������� ������� S � ������ �����

	const integer nve = 8;

	integer i, j; // �������� ����� for
	integer irow, icol; // ������, �������
	for (i = 0; i<nve; i++) {
		irow = nvtx[i][ie] - 1;

		// ������ ������������� �������������� ���������� ?
		if (constr[irow]) { // ��
			setValueIMatrix(&sparseS, irow, irow, 1.0f);
			//addelmsimplesparse_Stress(sparseM, 1.0f, irow, irow, true, true);
			if (bsecond_member_of_equation) rthdsd[irow] = potent[irow];
		}
		else { // ���
			   // ���, ��������� ����������, �������� nve-��������
			for (j = 0; j < nve; j++) {

				icol = nvtx[j][ie] - 1;

				// ������� ������������� �������������� ���������� ?
				if (constr[icol]) { // ��
					addValueIMatrix(&sparseS, irow, icol, Kmatrix[i][j]);
									// ����� ���������� ������ ������ �����
									//if (bsecond_member_of_equation) rthdsd[irow] += /*Kmatrix[i][j] * source[ie]*/ - selm[i][j] * potent[icol];
				}
				else { // ���
					   // ����� ���������� ���������� ������� S � ������ �����
					addValueIMatrix(&sparseS, irow, icol, Kmatrix[i][j]);
					//addelmsimplesparse_Stress(sparseM, Kmatrix[i][j], irow, icol, false, false);
					//if (bsecond_member_of_equation) rthdsd[irow] += telm[i][j] * source[ie];
				}
			}
		}

	}
} //elembdSparse(ie)


  // ��������� ��������� ��������� ��������� Kmatrix � Tmatrix (selm, telm)
  // � ���������� ������� Smatrix � (���� bsecond_member_of_equation=true)
  // ������ �����. ��� ��������� �������������.
  // 19.05.2018
void elembdSparse4(integer ie, SIMPLESPARSE &sparseM, integer** nvtx,
	bool* &constr, doublereal* &rthdsd,
	doublereal** &Kmatrix, doublereal* &potent,
	bool bsecond_member_of_equation) {
	// ������� ��������� ��������� S � T (selm � telm)
	// � ������������� ���������� ������� S � ������ �����

	const integer nve = 8;

	integer i, j; // �������� ����� for
	integer irow, icol; // ������, �������
	for (i = 0; i<nve; i++) {
		irow = nvtx[i][ie] - 1;

		// ������ ������������� �������������� ���������� ?
		if (constr[irow]) { // ��
			//setValueIMatrix(sparseM, irow, irow, 1.0f);
			addelmsimplesparse_Stress(sparseM, 1.0f, irow, irow, true, true);
			//addelmsimplesparse_Stress(sparseM, 1.0f, irow, irow, false, false);
			if (bsecond_member_of_equation) rthdsd[irow] = potent[irow];
		}
		else { // ���
			   // ���, ��������� ����������, �������� nve-��������
			for (j = 0; j < nve; j++) {

				icol = nvtx[j][ie] - 1;

				// ������� ������������� �������������� ���������� ?
				if (constr[icol]) { // ��
					addelmsimplesparse_Stress(sparseM, Kmatrix[i][j], irow, icol, false, false);
									// ����� ���������� ������ ������ �����
									//if (bsecond_member_of_equation) rthdsd[irow] += /*Kmatrix[i][j] * source[ie]*/ - selm[i][j] * potent[icol];
					
				}
				else { // ���
					   // ����� ���������� ���������� ������� S � ������ �����
					addelmsimplesparse_Stress(sparseM, Kmatrix[i][j], irow, icol, false, false);
					//addelmsimplesparse_Stress(sparseM, Kmatrix[i][j], irow, icol, false, false);
					//if (bsecond_member_of_equation) rthdsd[irow] += telm[i][j] * source[ie];
				}
			}
		}

	}
} //elembdSparse(ie)


  // �������������� ������ ������� ������������� ��� ������������ ������. 4.08.2017.
  // 16.09.2017.
void Thermal_ALICE_assemble_old(integer iP, integer** nvtx,
	TOCHKA* pa, doublereal** prop, doublereal** &Kmatrix)
{

	//nvtx
	// ---|+--|-+-|++-|--+|+-+|-++|+++
	// 1	2	3	4	5	6	7	8
	// ������� ������������ ������� ����� � ������ ������.
	// --+|-++|+++|+-+|---|-+-|++-|+--
	// 5	7	8	6	1	3	4	2
	// ����� ������ ����� ������������� ����� �������. 


	doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
	volume3D(iP, nvtx, pa, hx, hy, hz);
	//printf("%e %e %e\n",hx,hy,hz);

	doublereal lambda; // ����������� ����������������.

	lambda = prop[LAM][iP];

	// ������������ ������ ����������������.

	// ������ ��������� �������.
	Kmatrix[0][0] = 0.25*lambda*((prop[MULT_LAM_X][iP] * hy*hz / hx)+ (prop[MULT_LAM_Y][iP] * hx * hz / hy)+(prop[MULT_LAM_Z][iP] * hy*hx / hz));
	Kmatrix[0][1] = -0.25*lambda*prop[MULT_LAM_X][iP]*hy*hz/hx;
	Kmatrix[0][2] = 0.0;
	Kmatrix[0][3] = -0.25*lambda*prop[MULT_LAM_Y][iP] *hx*hz / hy;
	Kmatrix[0][4] = -0.25*lambda*prop[MULT_LAM_Z][iP]*hy*hx / hz;
	Kmatrix[0][5] = 0.0;
	Kmatrix[0][6] = 0.0;
	Kmatrix[0][7] = 0.0;
    //
	Kmatrix[1][0] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx;
	Kmatrix[1][1] = 0.25*lambda*((prop[MULT_LAM_X][iP] * hy*hz / hx) + (prop[MULT_LAM_Y][iP] * hx * hz / hy) + (prop[MULT_LAM_Z][iP] * hy*hx / hz));
	Kmatrix[1][2] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy;
	Kmatrix[1][3] = 0.0;
	Kmatrix[1][4] = 0.0;
	Kmatrix[1][5] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz;
	Kmatrix[1][6] = 0.0;
	Kmatrix[1][7] = 0.0;
	//
	
	Kmatrix[2][0] = 0.0;
	Kmatrix[2][1] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy;
	Kmatrix[2][2] = 0.25*lambda*((prop[MULT_LAM_X][iP]*hy*hz / hx) + (prop[MULT_LAM_Y][iP] *hx * hz / hy) + (prop[MULT_LAM_Z][iP] *hy*hx / hz));
	Kmatrix[2][3] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx;
	Kmatrix[2][4] = 0.0;
	Kmatrix[2][5] = 0.0;
	Kmatrix[2][6] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz;
	Kmatrix[2][7] = 0.0;
	// 
	Kmatrix[3][0] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy;
	Kmatrix[3][1] = 0.0;
	Kmatrix[3][2] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx;
	Kmatrix[3][3] = 0.25*lambda*((prop[MULT_LAM_X][iP]*hy*hz / hx) + (prop[MULT_LAM_Y][iP] *hx * hz / hy) + (prop[MULT_LAM_Z][iP] *hy*hx / hz));
	Kmatrix[3][4] = 0.0;
	Kmatrix[3][5] = 0.0;
	Kmatrix[3][6] = 0.0;
	Kmatrix[3][7] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz;
	
	/*
	// 
	Kmatrix[2][0] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy;
	Kmatrix[2][1] = 0.0;
	Kmatrix[2][2] = 0.25*lambda*((prop[MULT_LAM_X][iP] * hy*hz / hx) + (prop[MULT_LAM_Y][iP] * hx * hz / hy) + (prop[MULT_LAM_Z][iP] * hy*hx / hz));
	Kmatrix[2][3] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx;
	Kmatrix[2][4] = 0.0;
	Kmatrix[2][5] = 0.0;
	Kmatrix[2][6] =  -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz;
	Kmatrix[2][7] = 0.0;
	//
	Kmatrix[3][0] = 0.0;
	Kmatrix[3][1] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy;
	Kmatrix[3][2] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx;
	Kmatrix[3][3] = 0.25*lambda*(prop[MULT_LAM_X][iP] * (hy*hz / hx) + (prop[MULT_LAM_Y][iP] * hx * hz / hy) + (prop[MULT_LAM_Z][iP] * hy*hx / hz));
	Kmatrix[3][4] = 0.0;
	Kmatrix[3][5] = 0.0;
	Kmatrix[3][6] = 0.0;
	Kmatrix[3][7] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz;
	*/
    // 
	Kmatrix[4][0] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz;
	Kmatrix[4][1] = 0.0;
	Kmatrix[4][2] = 0.0;
	Kmatrix[4][3] = 0.0;
	Kmatrix[4][4] = 0.25*lambda*((prop[MULT_LAM_X][iP] * hy*hz / hx) + (prop[MULT_LAM_Y][iP] * hx * hz / hy) + (prop[MULT_LAM_Z][iP] * hy*hx / hz));
	Kmatrix[4][5] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx;
	Kmatrix[4][6] = 0.0;
	Kmatrix[4][7] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy;
	// 
	Kmatrix[5][0] = 0.0;
	Kmatrix[5][1] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz;
	Kmatrix[5][2] = 0.0;
	Kmatrix[5][3] = 0.0;
	Kmatrix[5][4] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx;
	Kmatrix[5][5] = 0.25*lambda*((prop[MULT_LAM_X][iP] * hy*hz / hx) + (prop[MULT_LAM_Y][iP] * hx * hz / hy) + (prop[MULT_LAM_Z][iP] * hy*hx / hz));
	Kmatrix[5][6] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy;
	Kmatrix[5][7] = 0.0;
	// 
	
	Kmatrix[6][0] = 0.0;
	Kmatrix[6][1] = 0.0;
	Kmatrix[6][2] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz;
	Kmatrix[6][3] = 0.0;
	Kmatrix[6][4] = 0.0;
	Kmatrix[6][5] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy;
	Kmatrix[6][6] = 0.25*lambda*((prop[MULT_LAM_X][iP]*hy*hz / hx) + (prop[MULT_LAM_Y][iP] *hx * hz / hy) + (prop[MULT_LAM_Z][iP] *hy*hx / hz));
	Kmatrix[6][7] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx;
	// 
	Kmatrix[7][0] = 0.0;
	Kmatrix[7][1] = 0.0;
	Kmatrix[7][2] = 0.0;
	Kmatrix[7][3] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz;
	Kmatrix[7][4] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy;
	Kmatrix[7][5] = 0.0;
	Kmatrix[7][6] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx;
	Kmatrix[7][7] = 0.25*lambda*((prop[MULT_LAM_X][iP]*hy*hz / hx) + (prop[MULT_LAM_Y][iP] *hx * hz / hy) + (prop[MULT_LAM_Z][iP] *hy*hx / hz));
	
	/*
	// 
	Kmatrix[6][0] = 0.0;
	Kmatrix[6][1] = 0.0;
	Kmatrix[6][2] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz;
	Kmatrix[6][3] = 0.0;
	Kmatrix[6][4] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy;
	Kmatrix[6][5] = 0.0;
	Kmatrix[6][6] = 0.25*lambda*((prop[MULT_LAM_X][iP] * hy*hz / hx) + (prop[MULT_LAM_Y][iP] * hx * hz / hy) + (prop[MULT_LAM_Z][iP] * hy*hx / hz));
	Kmatrix[6][7] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx;
	//
	Kmatrix[7][0] = 0.0;
	Kmatrix[7][1] = 0.0;
	Kmatrix[7][2] = 0.0;
	Kmatrix[7][3] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz;
	Kmatrix[7][4] = 0.0;
	Kmatrix[7][5] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy;
	Kmatrix[7][6] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx;
	Kmatrix[7][7] = 0.25*lambda*((prop[MULT_LAM_X][iP] * hy*hz / hx) + (prop[MULT_LAM_Y][iP] * hx * hz / hy) + (prop[MULT_LAM_Z][iP] * hy*hx / hz));
	// 
	*/
	/*
	for (integer i_4 = 0; i_4 < 8; i_4++) {
		doublereal ap = Kmatrix[i_4][i_4];
		for (integer j_4 = 0; j_4 < 8; j_4++) {
			//228.51 ����������� ����������� ����������� ��� ����� ��������� � ANSYS ������ �� ����������.
			// 0.01744841 - ����� ������� �� ��������������.
			Kmatrix[i_4][j_4] /= ap;
		}
	}
	*/
}

// �������������� ������ ������� ������������� ��� ������������ ������. 4.08.2017.
// 16.09.2017. 29.09.2018 (convection UDS).
void Thermal_ALICE_assemble(integer iP, integer** nvtx,
	TOCHKA* pa, doublereal** prop, doublereal** &Kmatrix, 
	integer** &ptr, doublereal* &Ux_arr, doublereal* &Uy_arr, doublereal* &Uz_arr)
{

	//nvtx
	// ---|+--|-+-|++-|--+|+-+|-++|+++
	// 1	2	3	4	5	6	7	8
	// ������� ������������ ������� ����� � ������ ������.
	// --+|-++|+++|+-+|---|-+-|++-|+--
	// 5	7	8	6	1	3	4	2
	// ����� ������ ����� ������������� ����� �������. 

	doublereal Ux = Ux_arr[iP], Uy = Uy_arr[iP], Uz = Uz_arr[iP]; // ���������� �������� � ������ ������ iP.

	/*
	if (ptr[0][iP] > -1) {
		Ux = f[ptr[1][iP]].potent[VX][ptr[0][iP]];
		Uy = f[ptr[1][iP]].potent[VY][ptr[0][iP]];
		Uz = f[ptr[1][iP]].potent[VZ][ptr[0][iP]];
	}
	*/
	doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
	volume3D(iP, nvtx, pa, hx, hy, hz);
	//printf("%e %e %e\n",hx,hy,hz);

	doublereal lambda; // ����������� ����������������.

	lambda = prop[LAM][iP];

	// ������������ ������ ���������������� � ���������� �� ������ ��������������� �����.
	doublereal rho_Cp = prop[RHO][iP] * prop[CP][iP];

	// ������ ��������� �������.
	Kmatrix[0][0] = 0.25*lambda*(prop[MULT_LAM_X][iP] * hy*hz / hx) +fmax(-0.25*(rho_Cp*hz*hy*Ux),0.0)
		           +0.25*lambda*(prop[MULT_LAM_Y][iP] * hx * hz / hy) + fmax(-0.25*(rho_Cp*hz*hx*Uy), 0.0)
		           +0.25*lambda*(prop[MULT_LAM_Z][iP] * hy*hx / hz)+fmax(-0.25*(rho_Cp*hx*hy*Uz), 0.0);
	Kmatrix[0][1] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx - fmax(-0.25*(rho_Cp*hz*hy*Ux), 0.0);
	Kmatrix[0][2] = 0.0;
	Kmatrix[0][3] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy - fmax(-0.25*(rho_Cp*hz*hx*Uy), 0.0);
	Kmatrix[0][4] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz - fmax(-0.25*(rho_Cp*hx*hy*Uz), 0.0);
	Kmatrix[0][5] = 0.0;
	Kmatrix[0][6] = 0.0;
	Kmatrix[0][7] = 0.0;
	//
	Kmatrix[1][0] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx - fmax(0.25*(rho_Cp*hz*hy*Ux), 0.0);
	Kmatrix[1][1] = 0.25*lambda*(prop[MULT_LAM_X][iP] * hy*hz / hx) + fmax(0.25*(rho_Cp*hz*hy*Ux), 0.0)
		          + 0.25*lambda*(prop[MULT_LAM_Y][iP] * hx * hz / hy) + fmax(-0.25*(rho_Cp*hz*hx*Uy), 0.0)
		          + 0.25*lambda*(prop[MULT_LAM_Z][iP] * hy*hx / hz) + fmax(-0.25*(rho_Cp*hx*hy*Uz), 0.0);
	Kmatrix[1][2] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy - fmax(-0.25*(rho_Cp*hz*hx*Uy), 0.0);
	Kmatrix[1][3] = 0.0;
	Kmatrix[1][4] = 0.0;
	Kmatrix[1][5] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz - fmax(-0.25*(rho_Cp*hx*hy*Uz), 0.0);
	Kmatrix[1][6] = 0.0;
	Kmatrix[1][7] = 0.0;
	//

	Kmatrix[2][0] = 0.0;
	Kmatrix[2][1] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy - fmax(0.25*(rho_Cp*hz*hx*Uy), 0.0) ;
	Kmatrix[2][2] = 0.25*lambda*(prop[MULT_LAM_X][iP] * hy*hz / hx) + fmax(0.25*(rho_Cp*hz*hy*Ux), 0.0)
		          + 0.25*lambda*(prop[MULT_LAM_Y][iP] * hx * hz / hy) + fmax(0.25*(rho_Cp*hz*hx*Uy), 0.0)
		          + 0.25*lambda*(prop[MULT_LAM_Z][iP] * hy*hx / hz)+ fmax(-0.25*(rho_Cp*hx*hy*Uz), 0.0);
	Kmatrix[2][3] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx - fmax(0.25*(rho_Cp*hz*hy*Ux), 0.0);
	Kmatrix[2][4] = 0.0;
	Kmatrix[2][5] = 0.0;
	Kmatrix[2][6] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz - fmax(-0.25*(rho_Cp*hx*hy*Uz), 0.0);
	Kmatrix[2][7] = 0.0;
	// 
	Kmatrix[3][0] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy - fmax(0.25*(rho_Cp*hz*hx*Uy), 0.0);
	Kmatrix[3][1] = 0.0;
	Kmatrix[3][2] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx - fmax(-0.25*(rho_Cp*hz*hy*Ux), 0.0);
	Kmatrix[3][3] = 0.25*lambda*(prop[MULT_LAM_X][iP] * hy*hz / hx) + fmax(-0.25*(rho_Cp*hz*hy*Ux), 0.0)
		          + 0.25*lambda*(prop[MULT_LAM_Y][iP] * hx * hz / hy) + fmax(0.25*(rho_Cp*hz*hx*Uy), 0.0)
		          + 0.25*lambda*(prop[MULT_LAM_Z][iP] * hy*hx / hz) + fmax(-0.25*(rho_Cp*hx*hy*Uz), 0.0);
	Kmatrix[3][4] = 0.0;
	Kmatrix[3][5] = 0.0;
	Kmatrix[3][6] = 0.0;
	Kmatrix[3][7] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz - fmax(-0.25*(rho_Cp*hx*hy*Uz), 0.0);

	/*
	//
	Kmatrix[2][0] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy;
	Kmatrix[2][1] = 0.0;
	Kmatrix[2][2] = 0.25*lambda*((prop[MULT_LAM_X][iP] * hy*hz / hx) + (prop[MULT_LAM_Y][iP] * hx * hz / hy) + (prop[MULT_LAM_Z][iP] * hy*hx / hz));
	Kmatrix[2][3] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx;
	Kmatrix[2][4] = 0.0;
	Kmatrix[2][5] = 0.0;
	Kmatrix[2][6] =  -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz;
	Kmatrix[2][7] = 0.0;
	//
	Kmatrix[3][0] = 0.0;
	Kmatrix[3][1] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy;
	Kmatrix[3][2] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx;
	Kmatrix[3][3] = 0.25*lambda*(prop[MULT_LAM_X][iP] * (hy*hz / hx) + (prop[MULT_LAM_Y][iP] * hx * hz / hy) + (prop[MULT_LAM_Z][iP] * hy*hx / hz));
	Kmatrix[3][4] = 0.0;
	Kmatrix[3][5] = 0.0;
	Kmatrix[3][6] = 0.0;
	Kmatrix[3][7] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz;
	*/
	
	//
	// 
	Kmatrix[4][0] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz - fmax(0.25*(rho_Cp*hx*hy*Uz), 0.0);
	Kmatrix[4][1] = 0.0;
	Kmatrix[4][2] = 0.0;
	Kmatrix[4][3] = 0.0;
	Kmatrix[4][4] = 0.25*lambda*(prop[MULT_LAM_X][iP] * hy*hz / hx) + fmax(-0.25*(rho_Cp*hz*hy*Ux), 0.0)
		          + 0.25*lambda*(prop[MULT_LAM_Y][iP] * hx * hz / hy)+ fmax(-0.25*(rho_Cp*hz*hx*Uy), 0.0)
	              + 0.25*lambda*(prop[MULT_LAM_Z][iP] * hy*hx / hz)+ fmax(0.25*(rho_Cp*hx*hy*Uz), 0.0);
	Kmatrix[4][5] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx - fmax(-0.25*(rho_Cp*hz*hy*Ux), 0.0);
	Kmatrix[4][6] = 0.0;
	Kmatrix[4][7] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy - fmax(-0.25*(rho_Cp*hz*hx*Uy), 0.0);
	
	// 
	Kmatrix[5][0] = 0.0;
	Kmatrix[5][1] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz - fmax(0.25*(rho_Cp*hx*hy*Uz), 0.0);
	Kmatrix[5][2] = 0.0;
	Kmatrix[5][3] = 0.0;
	Kmatrix[5][4] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx - fmax(0.25*(rho_Cp*hz*hy*Ux), 0.0);
	Kmatrix[5][5] = 0.25*lambda*(prop[MULT_LAM_X][iP] * hy*hz / hx) + fmax(0.25*(rho_Cp*hz*hy*Ux), 0.0)
		          + 0.25*lambda*(prop[MULT_LAM_Y][iP] * hx * hz / hy) + fmax(-0.25*(rho_Cp*hz*hx*Uy), 0.0)
		          + 0.25*lambda*(prop[MULT_LAM_Z][iP] * hy*hx / hz) + fmax(0.25*(rho_Cp*hx*hy*Uz), 0.0);
	Kmatrix[5][6] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy - fmax(-0.25*(rho_Cp*hz*hx*Uy), 0.0);
	Kmatrix[5][7] = 0.0;
	
	// 

	Kmatrix[6][0] = 0.0;
	Kmatrix[6][1] = 0.0;
	Kmatrix[6][2] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz - fmax(0.25*(rho_Cp*hx*hy*Uz), 0.0);
	Kmatrix[6][3] = 0.0;
	Kmatrix[6][4] = 0.0;
	Kmatrix[6][5] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy - fmax(0.25*(rho_Cp*hz*hx*Uy), 0.0);
	Kmatrix[6][6] = 0.25*lambda*(prop[MULT_LAM_X][iP] * hy*hz / hx) + fmax(0.25*(rho_Cp*hz*hy*Ux), 0.0)
	              + 0.25*lambda*(prop[MULT_LAM_Y][iP] * hx * hz / hy) + fmax(0.25*(rho_Cp*hz*hx*Uy), 0.0)
		          + 0.25*lambda*(prop[MULT_LAM_Z][iP] * hy*hx / hz) + fmax(+0.25*(rho_Cp*hx*hy*Uz), 0.0);
	Kmatrix[6][7] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx - fmax(0.25*(rho_Cp*hz*hy*Ux), 0.0);
	
	// 
	Kmatrix[7][0] = 0.0;
	Kmatrix[7][1] = 0.0;
	Kmatrix[7][2] = 0.0;
	Kmatrix[7][3] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz - fmax(0.25*(rho_Cp*hx*hy*Uz), 0.0);
	Kmatrix[7][4] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy - fmax(0.25*(rho_Cp*hz*hx*Uy), 0.0);
	Kmatrix[7][5] = 0.0;
	Kmatrix[7][6] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx - fmax(-0.25*(rho_Cp*hz*hy*Ux), 0.0);
	Kmatrix[7][7] = 0.25*lambda*(prop[MULT_LAM_X][iP] * hy*hz / hx) + fmax(-0.25*(rho_Cp*hz*hy*Ux), 0.0)
		          + 0.25*lambda*(prop[MULT_LAM_Y][iP] * hx * hz / hy) + fmax(0.25*(rho_Cp*hz*hx*Uy), 0.0)
	              + 0.25*lambda*(prop[MULT_LAM_Z][iP] * hy*hx / hz) + fmax(0.25*(rho_Cp*hx*hy*Uz), 0.0);

	/*
	//
	Kmatrix[6][0] = 0.0;
	Kmatrix[6][1] = 0.0;
	Kmatrix[6][2] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz;
	Kmatrix[6][3] = 0.0;
	Kmatrix[6][4] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy;
	Kmatrix[6][5] = 0.0;
	Kmatrix[6][6] = 0.25*lambda*((prop[MULT_LAM_X][iP] * hy*hz / hx) + (prop[MULT_LAM_Y][iP] * hx * hz / hy) + (prop[MULT_LAM_Z][iP] * hy*hx / hz));
	Kmatrix[6][7] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx;
	//
	Kmatrix[7][0] = 0.0;
	Kmatrix[7][1] = 0.0;
	Kmatrix[7][2] = 0.0;
	Kmatrix[7][3] = -0.25*lambda*prop[MULT_LAM_Z][iP] * hy*hx / hz;
	Kmatrix[7][4] = 0.0;
	Kmatrix[7][5] = -0.25*lambda*prop[MULT_LAM_Y][iP] * hx*hz / hy;
	Kmatrix[7][6] = -0.25*lambda*prop[MULT_LAM_X][iP] * hy*hz / hx;
	Kmatrix[7][7] = 0.25*lambda*((prop[MULT_LAM_X][iP] * hy*hz / hx) + (prop[MULT_LAM_Y][iP] * hx * hz / hy) + (prop[MULT_LAM_Z][iP] * hy*hx / hz));
	//
	*/
	/*
	for (integer i_4 = 0; i_4 < 8; i_4++) {
	doublereal ap = Kmatrix[i_4][i_4];
	for (integer j_4 = 0; j_4 < 8; j_4++) {
	//228.51 ����������� ����������� ����������� ��� ����� ��������� � ANSYS ������ �� ����������.
	// 0.01744841 - ����� ������� �� ��������������.
	Kmatrix[i_4][j_4] /= ap;
	}
	}
	*/
}



#endif