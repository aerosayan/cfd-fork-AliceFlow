// �������� ���-��� 1983 ����

#ifndef _RHIE_CHOW_CPP_
#define _RHIE_CHOW_CPP_ 1

// ���������� ����� �������� ���-��� �� ����� ������������ ������.
doublereal rFgRhieChow_internal(integer iP, integer G, doublereal rhog, doublereal alpha, 
	integer** nvtx, ALICE_PARTITION** sosedi, integer maxelm,
				 doublereal* pressure, TOCHKA* pa, doublereal **diag_coef) {

	// ���� ��������� ������ ������� : 30 ������� 2011 ����.

	/*
	*  iP - ����� ������������ ������ ��� ����� �������� ��������� �������� ���-���,
	*  G - ����� (Gran) ��� ������ ��������� ������� ���-���,
	*  rhog - �������� ��������� �� ����� G �� iP,
	*  alpha - �������� ���������� ��� ��������,
	*  diag_coef - ������������ ������������ ������� ��� ��������� ��������,
	*  pressure - ���� ��������.
	*  ����� ��������� ���������� ������ ���� ����, ���� ��� ��. ������ constr_struct.c
	*/

	// ������ ������� �������� ����������� ��������� ���� ��������� � �������� ��������
	// ���-��� ������ � ����� �����, � ������ ������ ������ �������. ������ ����������
	// �������� ��� ����������� ������������ ������ ������� ������� �� ������� �� ����������� ����� G
	// ��� ������� �� ���� ����������� ����� ���������� ������. �� ������� ��������������� ����������� �� ����� G,
	// ����� ���� ��� ����������� ������ ���������� ������ ��� � ��������� ����������� ������. ���� ������ ��������� 
    // ��������. 
	// �������, ��� ������ �������������
	// � ��� ������ ����� ������������. ������ ������������ ���� ����� ��������� (�� ������ �� ������ ����� ��)
	// �������� �� ����� ��������� �������������� ���������� ���� - ������� ������� ������������ ������������ 
	// ��� 6 ��������� �����. ����� ���������� ���� ����� ������ ��������, �� ������� ��� �� ��������, �.�. 
	// ����� �� ������ ������� ����������� ������ ��� ����� ��������� ��� ������� ����.

	// ����������������� ������ ��� ������ ����������:
	// [1] SIMPLE METHOD FOR THE SOLUTION OF INCOMPRESSIBLE FLOWS ON  NON-STAGGERED GRIDS.
	// I. Sezai - Eastern Mediterranean University, Mechanical Engineering Departament, Mersin 10-Turkey
	// Revised in January, 2011.

    

	doublereal dx=0.0, dy=0.0, dz=0.0;
    volume3D(iP, nvtx, pa, dx, dy, dz);

	doublereal koef=0.0;
	integer GG=0; // ��������� ����� �� ������ G.
	integer backG=0; 
	switch (G) {
	    case ESIDE : GG=EE; backG=WSIDE; koef=rhog*dy*dz*dy*dz*alpha; break;
		case WSIDE: GG=WW; backG=ESIDE; koef=rhog*dy*dz*dy*dz*alpha; break;
		case NSIDE : GG=NN; backG=SSIDE; koef=rhog*dx*dz*dx*dz*alpha; break;
		case SSIDE : GG=SS; backG=NSIDE; koef=rhog*dx*dz*dx*dz*alpha; break;
		case TSIDE : GG=TTSIDE; backG=BSIDE; koef=rhog*dx*dy*dx*dy*alpha; break;
		case BSIDE :GG=BB; backG=TSIDE; koef=rhog*dx*dy*dx*dy*alpha; break;
	}

	// SIMPLEC ��������.
	if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) koef/=(1.0-alpha);

	integer iG=sosedi[G][iP].iNODE1;
	integer iGG=sosedi[GG][iP].iNODE1;
	integer ibackG=sosedi[backG][iP].iNODE1;

	doublereal dlg=0.0, fgplus=1.0, dblg=0.0, fbgplus=1.0;
	switch (G) {
	   case ESIDE : dlg=0.5*dx;
		        dlg=0.5*(pa[nvtx[1][iG]-1].x+pa[nvtx[0][iG]-1].x);
	            dlg-=0.5*(pa[nvtx[1][iP]-1].x+pa[nvtx[0][iP]-1].x);
				fgplus=0.5*dx/dlg;

				if (ibackG < maxelm) {
				    dblg=0.5*(pa[nvtx[1][iP]-1].x+pa[nvtx[0][iP]-1].x);
	                dblg-=0.5*(pa[nvtx[1][ibackG]-1].x+pa[nvtx[0][ibackG]-1].x);
				} else dblg=0.5*dx;
				fbgplus=0.5*dx/dblg;
		        break;
	   case WSIDE: dlg=0.5*dx;
		        dlg=0.5*(pa[nvtx[1][iP]-1].x+pa[nvtx[0][iP]-1].x);
	            dlg-=0.5*(pa[nvtx[1][iG]-1].x+pa[nvtx[0][iG]-1].x);
				fgplus=0.5*dx/dlg;

				if (ibackG < maxelm) {
				   dblg=0.5*(pa[nvtx[1][ibackG]-1].x+pa[nvtx[0][ibackG]-1].x);
	               dblg-=0.5*(pa[nvtx[1][iP]-1].x+pa[nvtx[0][iP]-1].x);
				}
				else dblg=0.5*dx;
				fbgplus=0.5*dx/dblg;
		        break;
	   case NSIDE : dlg=0.5*dy;
		        dlg=0.5*(pa[nvtx[2][iG]-1].y+pa[nvtx[0][iG]-1].y);
	            dlg-=0.5*(pa[nvtx[2][iP]-1].y+pa[nvtx[0][iP]-1].y);
				fgplus=0.5*dy/dlg;

				if (ibackG < maxelm) {
				    dblg=0.5*(pa[nvtx[2][iP]-1].y+pa[nvtx[0][iP]-1].y);
	                dblg-=0.5*(pa[nvtx[2][ibackG]-1].y+pa[nvtx[0][ibackG]-1].y);
				} else dblg=0.5*dy;
				fbgplus=0.5*dy/dblg;
		        break;
	   case SSIDE : dlg=0.5*dy;
		        dlg=0.5*(pa[nvtx[2][iP]-1].y+pa[nvtx[0][iP]-1].y);
	            dlg-=0.5*(pa[nvtx[2][iG]-1].y+pa[nvtx[0][iG]-1].y);
				fgplus=0.5*dy/dlg;

				if (ibackG < maxelm) {
				   dblg=0.5*(pa[nvtx[2][ibackG]-1].y+pa[nvtx[0][ibackG]-1].y);
	               dblg-=0.5*(pa[nvtx[2][iP]-1].y+pa[nvtx[0][iP]-1].y);
				}
				else dblg=0.5*dy;
				fbgplus=0.5*dy/dblg;
		        break;
	   case TSIDE : dlg=0.5*dz;
		        dlg=0.5*(pa[nvtx[4][iG]-1].z+pa[nvtx[0][iG]-1].z);
	            dlg-=0.5*(pa[nvtx[4][iP]-1].z+pa[nvtx[0][iP]-1].z);
				fgplus=0.5*dz/dlg;

				if (ibackG < maxelm) {
				    dblg=0.5*(pa[nvtx[4][iP]-1].z+pa[nvtx[0][iP]-1].z);
	                dblg-=0.5*(pa[nvtx[4][ibackG]-1].z+pa[nvtx[0][ibackG]-1].z);
				} else dblg=0.5*dz;
				fbgplus=0.5*dz/dblg;
		        break;
	   case BSIDE :dlg=0.5*dz;
		        dlg=0.5*(pa[nvtx[4][iP]-1].z+pa[nvtx[0][iP]-1].z);
	            dlg-=0.5*(pa[nvtx[4][iG]-1].z+pa[nvtx[0][iG]-1].z);
				fgplus=0.5*dz/dlg;

				if (ibackG < maxelm) {
				   dblg=0.5*(pa[nvtx[4][ibackG]-1].z+pa[nvtx[0][ibackG]-1].z);
	               dblg-=0.5*(pa[nvtx[4][iP]-1].z+pa[nvtx[0][iP]-1].z);
				}
				else dblg=0.5*dz;
				fbgplus=0.5*dz/dblg;
		        break;
	}

	
	doublereal dlgg=dlg;
	doublereal PGG=pressure[iGG]; // �������� � ���� iGG
	if (iGG<maxelm) {
	    // ���� ���� ����������.
	    switch (G) {
	       case ESIDE : dlgg=0.5*(pa[nvtx[1][iGG]-1].x+pa[nvtx[0][iGG]-1].x);
	                dlgg-=0.5*(pa[nvtx[1][iG]-1].x+pa[nvtx[0][iG]-1].x);
		            break;
	       case WSIDE: dlgg=0.5*(pa[nvtx[1][iG]-1].x+pa[nvtx[0][iG]-1].x);
	                dlgg-=0.5*(pa[nvtx[1][iGG]-1].x+pa[nvtx[0][iGG]-1].x);
		            break;
	       case NSIDE : dlgg=0.5*(pa[nvtx[2][iGG]-1].y+pa[nvtx[0][iGG]-1].y);
	                dlgg-=0.5*(pa[nvtx[2][iG]-1].y+pa[nvtx[0][iG]-1].y);
		            break;
	       case SSIDE : dlgg=0.5*(pa[nvtx[2][iG]-1].y+pa[nvtx[0][iG]-1].y);
	                dlgg-=0.5*(pa[nvtx[2][iGG]-1].y+pa[nvtx[0][iGG]-1].y);
		            break;
	       case TSIDE : dlgg=0.5*(pa[nvtx[4][iGG]-1].z+pa[nvtx[0][iGG]-1].z);
	                dlgg-=0.5*(pa[nvtx[4][iG]-1].z+pa[nvtx[0][iG]-1].z);
		            break;
	       case BSIDE :dlgg=0.5*(pa[nvtx[4][iG]-1].z+pa[nvtx[0][iG]-1].z);
	                dlgg-=0.5*(pa[nvtx[4][iGG]-1].z+pa[nvtx[0][iGG]-1].z);
		            break;
	    } // end switch
	}
	else {
		 // ���� iWW ���������
		switch (G) {
		   case ESIDE : case WSIDE: dlgg=dlg-0.5*dx; break; // ���� iGG ���������
		   case NSIDE : case SSIDE : dlgg=dlg-0.5*dy; break;
		   case TSIDE : case BSIDE :dlgg=dlg-0.5*dz; break;
		} // end switch
	   
	}
			

	doublereal dxG=0.0, dyG=0.0, dzG=0.0;
    volume3D(iG, nvtx, pa, dxG, dyG, dzG);
	doublereal fggplus=0.0, fpplus=0.0;
	doublereal diagap_P=1.0, diagap_G=1.0; // ������������ ����������� �� �������� � ���� iP 
	 switch (G) {
	     case ESIDE : case WSIDE: fggplus=0.5*dxG/dlgg; fpplus=0.5*dxG/dlg; diagap_P=diag_coef[VX][iP]; diagap_G=diag_coef[VX][iG]; break; 
		 case NSIDE : case SSIDE : fggplus=0.5*dyG/dlgg; fpplus=0.5*dyG/dlg; diagap_P=diag_coef[VY][iP]; diagap_G=diag_coef[VY][iG]; break;
		 case TSIDE : case BSIDE :fggplus=0.5*dzG/dlgg; fpplus=0.5*dzG/dlg; diagap_P=diag_coef[VZ][iP]; diagap_G=diag_coef[VZ][iG]; break;
	 } // end switch


	doublereal apvelg=1.0;
	switch (G) {
	    case ESIDE : case WSIDE: apvelg=diag_coef[VX][iG]*diag_coef[VX][iP]/(fgplus*diag_coef[VX][iG]+(1-fgplus)*diag_coef[VX][iP]); break;
        case NSIDE : case SSIDE : apvelg=diag_coef[VY][iG]*diag_coef[VY][iP]/(fgplus*diag_coef[VY][iG]+(1-fgplus)*diag_coef[VY][iP]); break;
		case TSIDE : case BSIDE :apvelg=diag_coef[VZ][iG]*diag_coef[VZ][iP]/(fgplus*diag_coef[VZ][iG]+(1-fgplus)*diag_coef[VZ][iP]); break;
	} // end switch
	
    doublereal FgRhie_Chow=0.0; // ������������ ��������

	
	switch (G) {
	    case ESIDE : case NSIDE :  case TSIDE : FgRhie_Chow+=koef*(fgplus)*(fggplus*PGG+(1.0-fggplus)*pressure[iG]-fpplus*pressure[iP]-(1.0-fpplus)*pressure[iG])/(diagap_G);
                 FgRhie_Chow+=koef*(1.0-fgplus)*(fgplus*pressure[iG]+(1.0-fgplus)*pressure[iP]-fbgplus*pressure[ibackG]-(1.0-fbgplus)*pressure[iP])/(diagap_P);
	             FgRhie_Chow-=koef*(pressure[iG]-pressure[iP])/apvelg;
			     break;
		case WSIDE: case SSIDE : case BSIDE :FgRhie_Chow+=koef*(1.0-fgplus)*(fbgplus*pressure[ibackG]+(1.0-fbgplus)*pressure[iP]-fgplus*pressure[iG]-(1.0-fgplus)*pressure[iP])/(diagap_P);
                 FgRhie_Chow+=koef*(fgplus)*(fpplus*pressure[iP]+(1.0-fpplus)*pressure[iG]-fggplus*PGG-(1.0-fggplus)*pressure[iG])/(diagap_G);
			     FgRhie_Chow-=koef*(pressure[iP]-pressure[iG])/apvelg;
			     break;
	} // end final switch

	return FgRhie_Chow;

} // rFbRhieChow_internal

// ���������� ����� �������� ���-��� � ���������� �������� 
// �� ����� �����������  ������������ ������.
doublereal ugRhieChow_internal(integer iP, integer G, doublereal alpha, 
	integer** nvtx, ALICE_PARTITION** sosedi, integer maxelm,
				 doublereal* pressure, TOCHKA* pa, doublereal **diag_coef) {

	// ���� ��������� ������ ������� : 20 ������� 2011 ����.

	/*
	*  iP - ����� ������������ ������ ��� ����� �������� ��������� �������� ���-���,
	*  G - ����� (Gran) ��� ������ ��������� ������� ���-���,
	*  rhog - �������� ��������� �� ����� G �� iP,
	*  alpha - �������� ���������� ��� ��������,
	*  diag_coef - ������������ ������������ ������� ��� ��������� ��������,
	*  pressure - ���� ��������.
	*  ����� ��������� ���������� ������ ���� ����, ���� ��� ��. ������ constr_struct.c
	*/

	// ������ ������� �������� ����������� ��������� ���� ��������� � �������� ��������
	// ���-��� ������ � ����� �����, � ������ ������ ������ �������. ������ ����������
	// �������� ��� ����������� ������������ ������ ������� ������� �� ������� �� ����������� ����� G
	// ��� ������� �� ���� ����������� ����� ���������� ������. �� ������� ��������������� ����������� �� ����� G,
	// ����� ���� ��� ����������� ������ ���������� ������ ��� � ��������� ����������� ������. ���� ������ ��������� 
    // ��������. 
	// �������, ��� ������ �������������
	// � ��� ������ ����� ������������. ������ ������������ ���� ����� ��������� (�� ������ �� ������ ����� ��)
	// �������� �� ����� ��������� �������������� ���������� ���� - ������� ������� ������������ ������������ 
	// ��� 6 ��������� �����. ����� ���������� ���� ����� ������ ��������, �� ������� ��� �� ��������, �.�. 
	// ����� �� ������ ������� ����������� ������ ��� ����� ��������� ��� ������� ����.

	// ����������������� ������ ��� ������ ����������:
	// [1] SIMPLE METHOD FOR THE SOLUTION OF INCOMPRESSIBLE FLOWS ON  NON-STAGGERED GRIDS.
	// I. Sezai - Eastern Mediterranean University, Mechanical Engineering Departament, Mersin 10-Turkey
	// Revised in January, 2011.

    

	doublereal dx=0.0, dy=0.0, dz=0.0;
    volume3D(iP, nvtx, pa, dx, dy, dz);

	doublereal koef=0.0;
	integer GG=0; // ��������� ����� �� ������ G.
	integer backG=0; 
	switch (G) {
	    case ESIDE : GG=EE; backG=WSIDE; koef=dy*dz*alpha; break; // rhog*dy*dz*
		case WSIDE: GG=WW; backG=ESIDE; koef=dy*dz*alpha; break; // rhog*dy*dz*
		case NSIDE : GG=NN; backG=SSIDE; koef=dx*dz*alpha; break; // rhog*dx*dz*
		case SSIDE : GG=SS; backG=NSIDE; koef=dx*dz*alpha; break; // rhog*dx*dz* 
		case TSIDE : GG=TTSIDE; backG=BSIDE; koef=dx*dy*alpha; break; // rhog*dx*dy*
		case BSIDE :GG=BB; backG=TSIDE; koef=dx*dy*alpha; break; // rhog*dx*dy*
	}

	// SIMPLEC ��������.
	if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) koef/=(1.0-alpha);

	integer iG=sosedi[G][iP].iNODE1;
	integer iGG=sosedi[GG][iP].iNODE1;
	integer ibackG=sosedi[backG][iP].iNODE1;

	doublereal dlg=0.0, fgplus=1.0, dblg=0.0, fbgplus=1.0;
	switch (G) {
	   case ESIDE : dlg=0.5*dx;
		        dlg=0.5*(pa[nvtx[1][iG]-1].x+pa[nvtx[0][iG]-1].x);
	            dlg-=0.5*(pa[nvtx[1][iP]-1].x+pa[nvtx[0][iP]-1].x);
				fgplus=0.5*dx/dlg;

				if (ibackG < maxelm) {
				    dblg=0.5*(pa[nvtx[1][iP]-1].x+pa[nvtx[0][iP]-1].x);
	                dblg-=0.5*(pa[nvtx[1][ibackG]-1].x+pa[nvtx[0][ibackG]-1].x);
				} else dblg=0.5*dx;
				fbgplus=0.5*dx/dblg;
		        break;
	   case WSIDE: dlg=0.5*dx;
		        dlg=0.5*(pa[nvtx[1][iP]-1].x+pa[nvtx[0][iP]-1].x);
	            dlg-=0.5*(pa[nvtx[1][iG]-1].x+pa[nvtx[0][iG]-1].x);
				fgplus=0.5*dx/dlg;

				if (ibackG < maxelm) {
				   dblg=0.5*(pa[nvtx[1][ibackG]-1].x+pa[nvtx[0][ibackG]-1].x);
	               dblg-=0.5*(pa[nvtx[1][iP]-1].x+pa[nvtx[0][iP]-1].x);
				}
				else dblg=0.5*dx;
				fbgplus=0.5*dx/dblg;
		        break;
	   case NSIDE : dlg=0.5*dy;
		        dlg=0.5*(pa[nvtx[2][iG]-1].y+pa[nvtx[0][iG]-1].y);
	            dlg-=0.5*(pa[nvtx[2][iP]-1].y+pa[nvtx[0][iP]-1].y);
				fgplus=0.5*dy/dlg;

				if (ibackG < maxelm) {
				    dblg=0.5*(pa[nvtx[2][iP]-1].y+pa[nvtx[0][iP]-1].y);
	                dblg-=0.5*(pa[nvtx[2][ibackG]-1].y+pa[nvtx[0][ibackG]-1].y);
				} else dblg=0.5*dy;
				fbgplus=0.5*dy/dblg;
		        break;
	   case SSIDE : dlg=0.5*dy;
		        dlg=0.5*(pa[nvtx[2][iP]-1].y+pa[nvtx[0][iP]-1].y);
	            dlg-=0.5*(pa[nvtx[2][iG]-1].y+pa[nvtx[0][iG]-1].y);
				fgplus=0.5*dy/dlg;

				if (ibackG < maxelm) {
				   dblg=0.5*(pa[nvtx[2][ibackG]-1].y+pa[nvtx[0][ibackG]-1].y);
	               dblg-=0.5*(pa[nvtx[2][iP]-1].y+pa[nvtx[0][iP]-1].y);
				}
				else dblg=0.5*dy;
				fbgplus=0.5*dy/dblg;
		        break;
	   case TSIDE : dlg=0.5*dz;
		        dlg=0.5*(pa[nvtx[4][iG]-1].z+pa[nvtx[0][iG]-1].z);
	            dlg-=0.5*(pa[nvtx[4][iP]-1].z+pa[nvtx[0][iP]-1].z);
				fgplus=0.5*dz/dlg;

				if (ibackG < maxelm) {
				    dblg=0.5*(pa[nvtx[4][iP]-1].z+pa[nvtx[0][iP]-1].z);
	                dblg-=0.5*(pa[nvtx[4][ibackG]-1].z+pa[nvtx[0][ibackG]-1].z);
				} else dblg=0.5*dz;
				fbgplus=0.5*dz/dblg;
		        break;
	   case BSIDE :dlg=0.5*dz;
		        dlg=0.5*(pa[nvtx[4][iP]-1].z+pa[nvtx[0][iP]-1].z);
	            dlg-=0.5*(pa[nvtx[4][iG]-1].z+pa[nvtx[0][iG]-1].z);
				fgplus=0.5*dz/dlg;

				if (ibackG < maxelm) {
				   dblg=0.5*(pa[nvtx[4][ibackG]-1].z+pa[nvtx[0][ibackG]-1].z);
	               dblg-=0.5*(pa[nvtx[4][iP]-1].z+pa[nvtx[0][iP]-1].z);
				}
				else dblg=0.5*dz;
				fbgplus=0.5*dz/dblg;
		        break;
	}

	
	doublereal dlgg=dlg;
	doublereal PGG=pressure[iGG]; // �������� � ���� iGG
	if (iGG<maxelm) {
	    // ���� ���� ����������.
	    switch (G) {
	       case ESIDE : dlgg=0.5*(pa[nvtx[1][iGG]-1].x+pa[nvtx[0][iGG]-1].x);
	                dlgg-=0.5*(pa[nvtx[1][iG]-1].x+pa[nvtx[0][iG]-1].x);
		            break;
	       case WSIDE: dlgg=0.5*(pa[nvtx[1][iG]-1].x+pa[nvtx[0][iG]-1].x);
	                dlgg-=0.5*(pa[nvtx[1][iGG]-1].x+pa[nvtx[0][iGG]-1].x);
		            break;
	       case NSIDE : dlgg=0.5*(pa[nvtx[2][iGG]-1].y+pa[nvtx[0][iGG]-1].y);
	                dlgg-=0.5*(pa[nvtx[2][iG]-1].y+pa[nvtx[0][iG]-1].y);
		            break;
	       case SSIDE : dlgg=0.5*(pa[nvtx[2][iG]-1].y+pa[nvtx[0][iG]-1].y);
	                dlgg-=0.5*(pa[nvtx[2][iGG]-1].y+pa[nvtx[0][iGG]-1].y);
		            break;
	       case TSIDE : dlgg=0.5*(pa[nvtx[4][iGG]-1].z+pa[nvtx[0][iGG]-1].z);
	                dlgg-=0.5*(pa[nvtx[4][iG]-1].z+pa[nvtx[0][iG]-1].z);
		            break;
	       case BSIDE :dlgg=0.5*(pa[nvtx[4][iG]-1].z+pa[nvtx[0][iG]-1].z);
	                dlgg-=0.5*(pa[nvtx[4][iGG]-1].z+pa[nvtx[0][iGG]-1].z);
		            break;
	    } // end switch
	}
	else {
		 // ���� iWW ���������
		switch (G) {
		   case ESIDE : case WSIDE: dlgg=dlg-0.5*dx; break; // ���� iGG ���������
		   case NSIDE : case SSIDE : dlgg=dlg-0.5*dy; break;
		   case TSIDE : case BSIDE :dlgg=dlg-0.5*dz; break;
		} // end switch
	   
	}
			

	doublereal dxG=0.0, dyG=0.0, dzG=0.0;
    volume3D(iG, nvtx, pa, dxG, dyG, dzG);
	doublereal fggplus=0.0, fpplus=0.0;
	doublereal diagap_P=1.0, diagap_G=1.0; // ������������ ����������� �� �������� � ���� iP 
	 switch (G) {
	     case ESIDE : case WSIDE: fggplus=0.5*dxG/dlgg; fpplus=0.5*dxG/dlg; diagap_P=diag_coef[VX][iP]; diagap_G=diag_coef[VX][iG]; break; 
		 case NSIDE : case SSIDE : fggplus=0.5*dyG/dlgg; fpplus=0.5*dyG/dlg; diagap_P=diag_coef[VY][iP]; diagap_G=diag_coef[VY][iG]; break;
		 case TSIDE : case BSIDE :fggplus=0.5*dzG/dlgg; fpplus=0.5*dzG/dlg; diagap_P=diag_coef[VZ][iP]; diagap_G=diag_coef[VZ][iG]; break;
	 } // end switch


	doublereal apvelg=1.0;
	switch (G) {
	    case ESIDE : case WSIDE: apvelg=diag_coef[VX][iG]*diag_coef[VX][iP]/(fgplus*diag_coef[VX][iG]+(1-fgplus)*diag_coef[VX][iP]); break;
        case NSIDE : case SSIDE : apvelg=diag_coef[VY][iG]*diag_coef[VY][iP]/(fgplus*diag_coef[VY][iG]+(1-fgplus)*diag_coef[VY][iP]); break;
		case TSIDE : case BSIDE :apvelg=diag_coef[VZ][iG]*diag_coef[VZ][iP]/(fgplus*diag_coef[VZ][iG]+(1-fgplus)*diag_coef[VZ][iP]); break;
	} // end switch
	
    doublereal FgRhie_Chow=0.0; // ������������ ��������

	
	switch (G) {
	    case ESIDE : case NSIDE :  case TSIDE : FgRhie_Chow+=koef*(fgplus)*(fggplus*PGG+(1.0-fggplus)*pressure[iG]-fpplus*pressure[iP]-(1.0-fpplus)*pressure[iG])/(diagap_G);
                 FgRhie_Chow+=koef*(1.0-fgplus)*(fgplus*pressure[iG]+(1.0-fgplus)*pressure[iP]-fbgplus*pressure[ibackG]-(1.0-fbgplus)*pressure[iP])/(diagap_P);
	             FgRhie_Chow-=koef*(pressure[iG]-pressure[iP])/apvelg;
			     break;
		case WSIDE: case SSIDE : case BSIDE :FgRhie_Chow+=koef*(1.0-fgplus)*(fbgplus*pressure[ibackG]+(1.0-fbgplus)*pressure[iP]-fgplus*pressure[iG]-(1.0-fgplus)*pressure[iP])/(diagap_P);
                 FgRhie_Chow+=koef*(fgplus)*(fpplus*pressure[iP]+(1.0-fpplus)*pressure[iG]-fggplus*PGG-(1.0-fggplus)*pressure[iG])/(diagap_G);
			     FgRhie_Chow-=koef*(pressure[iP]-pressure[iG])/apvelg;
			     break;
	} // end final switch

	return FgRhie_Chow;

} // ugRhieChow_internal


//*
// �� ������� �� � ���� ������, �.�. ������ ������� ����� �����.
// ���������� ����� �������� ���-��� �� 
// �� �������� ����� ������ ������� �������� �������� ������� � �������
// ���������� � ������� ������������ ������. �.�. ��� ��� ���� ���� ����������������
// ������ ����������� �� ������� �������.
doublereal rFgRhieChow_internal_border1(integer iP, integer G, doublereal rhog, doublereal alpha, 
	integer** nvtx, ALICE_PARTITION** sosedi, integer maxelm,
				 doublereal* pressure, TOCHKA* pa, doublereal **diag_coef) {

    // �������� �������������� �������� � ����:
    doublereal PGG=0.0; // �������� � ���� iGG

	integer backG=0, backGG=0; 
	switch (G) {
	    case ESIDE : backG=WSIDE; backGG=WW; break;
		case WSIDE: backG=ESIDE; backGG=EE; break;
		case NSIDE : backG=SSIDE; backGG=SS; break;
		case SSIDE : backG=NSIDE; backGG=NN; break;
		case TSIDE : backG=BSIDE; backGG=BB; break;
		case BSIDE :backG=TSIDE; backGG=TTSIDE; break;
	}

	integer iG=sosedi[G][iP].iNODE1;
	integer ibackG=sosedi[backG][iP].iNODE1;

	//printf("iP=%d, iG=%d, ibackG=%d, maxelm=%d",iP,iG,ibackG,maxelm);
	//getchar(); // debug ��������� ������� ��� ��������

	doublereal PbackG, PP, PG;
	PbackG=pressure[ibackG]; PP=pressure[iP]; PG=pressure[iG];

	doublereal PbackGG;
	integer ibackGG=sosedi[backGG][iP].iNODE1; // ����� ���� ������ ������������ ��� �������� �������������
    PbackGG=pressure[ibackGG]; // �������� �������� � ���� ����.
	doublereal dx=0.0, dy=0.0, dz=0.0;
    volume3D(iP, nvtx, pa, dx, dy, dz);
	// �������� �������������:
	doublereal posbackG=0.0, posbackGG=0.0, posP=0.0, posGG=0.0;
	switch (G) {
	   case ESIDE : case WSIDE: posbackG=0.5*(pa[nvtx[1][ibackG]-1].x+pa[nvtx[0][ibackG]-1].x);
		                 posbackGG=0.5*(pa[nvtx[1][ibackGG]-1].x+pa[nvtx[0][ibackGG]-1].x); // xWW
		                 posP=0.5*(pa[nvtx[1][iP]-1].x+pa[nvtx[0][iP]-1].x); // xP
				         break;
	   case NSIDE : case SSIDE : posbackG=0.5*(pa[nvtx[2][ibackG]-1].y+pa[nvtx[0][ibackG]-1].y);
		                 posbackGG=0.5*(pa[nvtx[2][ibackGG]-1].y+pa[nvtx[0][ibackGG]-1].y); // ySS
		                 posP=0.5*(pa[nvtx[2][iP]-1].y+pa[nvtx[0][iP]-1].y); // yP
				         break;
	   case TSIDE : case BSIDE :posbackG=0.5*(pa[nvtx[4][ibackG]-1].z+pa[nvtx[0][ibackG]-1].z);
		                 posbackGG=0.5*(pa[nvtx[4][ibackGG]-1].z+pa[nvtx[0][ibackGG]-1].z); // zBB
		                 posP=0.5*(pa[nvtx[4][iP]-1].z+pa[nvtx[0][iP]-1].z); // zP
		                 break;
	} // end switch G


	switch (G) {
	   case ESIDE : posGG=posP+dx; // +0.5*dx+0.5*dx; 
		        break;
	   case WSIDE: posGG=posP-dx; // -0.5*dx-0.5*dx;
		        break;
	   case NSIDE : posGG=posP+dy; break;
	   case SSIDE : posGG=posP-dy; break;
	   case TSIDE : posGG=posP+dz; break;
	   case BSIDE :posGG=posP-dz; break;
	}
	 
		
    switch (G) {
	   case ESIDE : case NSIDE : case TSIDE : 
	               PGG=my_linear_interpolation('+', PP, PbackGG, posP, posbackGG, posGG); // �������� �������������.
				   break;
	   case WSIDE: case SSIDE : case BSIDE :
		           PGG=my_linear_interpolation('-', PP, PbackGG, posP, posbackGG, posGG); // �������� �������������.
		           break;
	} // end switch G


    doublereal koef=0.0;
	switch (G) {
	    case ESIDE : case WSIDE: koef=rhog*dy*dz*dy*dz*alpha; break;
		case NSIDE : case SSIDE : koef=rhog*dx*dz*dx*dz*alpha; break;
		case TSIDE : case BSIDE :koef=rhog*dx*dy*dx*dy*alpha; break;
	} // end switch G

	// SIMPLEC ��������.
	if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) koef/=(1.0-alpha);

    doublereal fggplus=0.5;
	doublereal positionleftnode=0.0, fdeltaplus=0.0, fbplus=0.0, fdelta2plus=0.0, apvelback=0.0, apvelg=0.0, apvelG=0.0;
	doublereal positionrightnode=0.0;

	switch (G) {
	case ESIDE : positionleftnode=posP-dx/8.0;// 0.5*((posP-0.5*dx)+(posP+0.25*dx));
	         fdeltaplus=((posP+0.25*dx-positionleftnode)/(posP+0.5*dx-positionleftnode));
	         fbplus=0.5*dx/(posP-posbackG);
	         fdelta2plus=dx/(8.0*(posP-posbackG));
	         apvelback=fdelta2plus*diag_coef[VX][ibackG]+(1.0-fdelta2plus)*diag_coef[VX][iP];
	         apvelg=0.5*(diag_coef[VX][iP]+diag_coef[VX][iG]);
			 apvelG=diag_coef[VX][iG];
		     break;
	case WSIDE: positionrightnode=posP+dx/8.0;
		     fdeltaplus=((posP-0.25*dx-positionrightnode)/(posP-0.5*dx-positionrightnode)); // (-1)/(-1)
		     fbplus=0.5*dx/(posbackG-posP);
			 fdelta2plus=dx/(8.0*(posbackG-posP));
			 apvelback=fdelta2plus*diag_coef[VX][ibackG]+(1.0-fdelta2plus)*diag_coef[VX][iP];
		     apvelg=0.5*(diag_coef[VX][iP]+diag_coef[VX][iG]);
			 apvelG=diag_coef[VX][iG];
		     break;
	case NSIDE : positionleftnode=posP-dy/8.0;// 0.5*((posP-0.5*dy)+(posP+0.25*dy));
	         fdeltaplus=((posP+0.25*dy-positionleftnode)/(posP+0.5*dy-positionleftnode));
	         fbplus=0.5*dy/(posP-posbackG);
	         fdelta2plus=dy/(8.0*(posP-posbackG));
	         apvelback=fdelta2plus*diag_coef[VY][ibackG]+(1.0-fdelta2plus)*diag_coef[VY][iP];
	         apvelg=0.5*(diag_coef[VY][iP]+diag_coef[VY][iG]);
			 apvelG=diag_coef[VY][iG];
		     break;
    case SSIDE : positionrightnode=posP+dy/8.0;
		     fdeltaplus=((posP-0.25*dy-positionrightnode)/(posP-0.5*dy-positionrightnode)); // (-1)/(-1)
		     fbplus=0.5*dy/(posbackG-posP);
			 fdelta2plus=dy/(8.0*(posbackG-posP));
			 apvelback=fdelta2plus*diag_coef[VY][ibackG]+(1.0-fdelta2plus)*diag_coef[VY][iP];
		     apvelg=0.5*(diag_coef[VY][iP]+diag_coef[VY][iG]);
			 apvelG=diag_coef[VY][iG];
		     break;
	case TSIDE : positionleftnode=posP-dz/8.0;// 0.5*((posP-0.5*dz)+(posP+0.25*dz));
	         fdeltaplus=((posP+0.25*dz-positionleftnode)/(posP+0.5*dz-positionleftnode));
	         fbplus=0.5*dz/(posP-posbackG);
	         fdelta2plus=dz/(8.0*(posP-posbackG));
	         apvelback=fdelta2plus*diag_coef[VZ][ibackG]+(1.0-fdelta2plus)*diag_coef[VZ][iP];
	         apvelg=0.5*(diag_coef[VZ][iP]+diag_coef[VZ][iG]);
			 apvelG=diag_coef[VZ][iG];
		     break;
	case BSIDE :positionrightnode=posP+dz/8.0;
		     fdeltaplus=((posP-0.25*dz-positionrightnode)/(posP-0.5*dz-positionrightnode)); // (-1)/(-1)
		     fbplus=0.5*dz/(posbackG-posP);
			 fdelta2plus=dz/(8.0*(posbackG-posP));
			 apvelback=fdelta2plus*diag_coef[VZ][ibackG]+(1.0-fdelta2plus)*diag_coef[VZ][iP];
		     apvelg=0.5*(diag_coef[VZ][iP]+diag_coef[VZ][iG]);
			 apvelG=diag_coef[VZ][iG];
		     break;
	}
	 

	doublereal FgRhie_Chow=0.0; // ������������ ��������

	switch (G) {
	case ESIDE : case NSIDE : case TSIDE :  // ������������� �����
	                            FgRhie_Chow+=koef*(fdeltaplus)*(0.5*(PGG+PG)-0.5*(PP+PG))/(apvelG);
                                FgRhie_Chow+=koef*(1.0-fdeltaplus)*(0.5*(PG+PP)-fbplus*PbackG-(1.0-fbplus)*PP)/(apvelback);
	                            FgRhie_Chow-=koef*(PG-PP)/apvelg;
								break;
	case WSIDE: case SSIDE : case BSIDE :// ������������� �����
			                    FgRhie_Chow+=koef*(1.0-fdeltaplus)*(fbplus*PbackG+(1.0-fbplus)*PP-0.5*(PG+PP))/(apvelback);
                                FgRhie_Chow+=koef*(fdeltaplus)*(0.5*(PP+PG)-0.5*(PGG+PG))/(apvelG);
			                    FgRhie_Chow-=koef*(PP-PG)/apvelg;
		                        break;
	}

	return FgRhie_Chow;

} // rFgRhieChow_internal_border
//*/


// ���������� ����� �������� ���-��� �� ������� ������������ ������
// � ������ ���� ��������������� ������� � ������� ����������� �����.
// �������� �� ��� ������ �� ���������� ������ ������� ��� ���������� ������
// ���������� � ��������� ��-�������� ��������� � ��������������� ��������.
// ������ ������� ������������� ����� ����� ����� ������.
doublereal rFgRhieChow_internal_border2(integer iP, integer G, doublereal rhog, doublereal alpha, 
	integer** nvtx, ALICE_PARTITION** sosedi, integer maxelm,
				 doublereal* pressure, TOCHKA* pa, doublereal **diag_coef) {

	// ����� ������ ����������:
				
	// �������� �������������� �������� � ����:
	doublereal PGG=0.0, PG=0.0; // �������� � �������������� ����� (��� ��������� �������).

	integer backG=0, backGG=0; 
	switch (G) {
	    case ESIDE : backG=WSIDE; backGG=WW; break;
		case WSIDE: backG=ESIDE; backGG=EE; break;
		case NSIDE : backG=SSIDE; backGG=SS; break;
		case SSIDE : backG=NSIDE; backGG=NN; break;
		case TSIDE : backG=BSIDE; backGG=BB; break;
		case BSIDE :backG=TSIDE; backGG=TTSIDE; break;
	}

	integer ibackGG=sosedi[backGG][iP].iNODE1; // ����� ���� ������ ������������ ��� �������� �������������
	integer ibackG=sosedi[backG][iP].iNODE1;
	integer iG=sosedi[G][iP].iNODE1;
	doublereal Pg=0.0, PP=0.0, PbackG=0.0, PbackGG=0.0;
	Pg=pressure[iG]; PP=pressure[iP]; PbackG=pressure[ibackG]; PbackGG=pressure[ibackGG];

	doublereal posbackG=0.0, posbackGG=0.0, posP=0.0;
	switch (G) {
	   case ESIDE : case WSIDE: posbackG=0.5*(pa[nvtx[1][ibackG]-1].x+pa[nvtx[0][ibackG]-1].x);
		                 posbackGG=0.5*(pa[nvtx[1][ibackGG]-1].x+pa[nvtx[0][ibackGG]-1].x); // xWW
		                 posP=0.5*(pa[nvtx[1][iP]-1].x+pa[nvtx[0][iP]-1].x); // xP
				         break;
	   case NSIDE : case SSIDE : posbackG=0.5*(pa[nvtx[2][ibackG]-1].y+pa[nvtx[0][ibackG]-1].y);
		                 posbackGG=0.5*(pa[nvtx[2][ibackGG]-1].y+pa[nvtx[0][ibackGG]-1].y); // ySS
		                 posP=0.5*(pa[nvtx[2][iP]-1].y+pa[nvtx[0][iP]-1].y); // yP
				         break;
	   case TSIDE : case BSIDE :posbackG=0.5*(pa[nvtx[4][ibackG]-1].z+pa[nvtx[0][ibackG]-1].z);
		                 posbackGG=0.5*(pa[nvtx[4][ibackGG]-1].z+pa[nvtx[0][ibackGG]-1].z); // zBB
		                 posP=0.5*(pa[nvtx[4][iP]-1].z+pa[nvtx[0][iP]-1].z); // zP
		                 break;
	} // end switch G

	doublereal dx=0.0, dy=0.0, dz=0.0;
    volume3D(iP, nvtx, pa, dx, dy, dz);

	doublereal posg=0.0, posG=0.0, posGG=0.0, dlbackg=0.0;
	switch (G) {
	    case ESIDE : dlbackg=posP-posbackG;
			     posg=posP+0.5*dx;
				 posG=posP+dx;
				 posGG=posG+dlbackg; 
			     break;
		case WSIDE: dlbackg=posbackG-posP;
			     posg=posP-0.5*dx;
				 posG=posP-dx;
				 posGG=posG-dlbackg; 
			     break;
		case NSIDE : dlbackg=posP-posbackG;
			     posg=posP+0.5*dy;
				 posG=posP+dy;
				 posGG=posG+dlbackg;
			     break;
		case SSIDE : dlbackg=posbackG-posP;
			     posg=posP-0.5*dy;
				 posG=posP-dy;
				 posGG=posG-dlbackg; 
			     break;
		case TSIDE : dlbackg=posP-posbackG;
			     posg=posP+0.5*dz;
				 posG=posP+dz;
				 posGG=posG+dlbackg;
			     break;
		case BSIDE :dlbackg=posbackG-posP;
			     posg=posP-0.5*dz;
				 posG=posP-dz;
				 posGG=posG-dlbackg; 
			     break;
	} // end switch G

	integer i1=1;
	// ���������� ��� ��������� ��������:
	/* 
	* i1 +  - + - - +
	*  0 WW W P e E EE
	*
	* i1    1 2 3
	*  1 WW W P e E EE // ������ ���� ������� ���� �������� �� ������� ������
	*
	* i1 1  2 3 
	*  2 WW W P e E EE
	*
	* i1 +  - + - + -
	*  3 WW W P e E EE
	*/
	// �������� �������������:
	switch (G) {
	case ESIDE : case NSIDE : case TSIDE :
		     switch (i1) {
			 case 0 : 
		              PGG=my_linear_interpolation('+', PP, PbackGG, posP, posbackGG, posGG); // �������� �������������.
			          PG=my_linear_interpolation('+', Pg, PbackG, posg, posbackG, posG); // �������� �������������.
					  break;
			 case 1 : 
				      PGG=my_quadratic_interpolation('+', PbackG, PP, Pg, posbackG, posP, posg, posGG); // ������������ ������������
					  PG=my_quadratic_interpolation('+', PbackG, PP, Pg, posbackG, posP, posg, posG); // ������������ ������������
				      break;
			 case 2 :
				      PGG=my_quadratic_interpolation('+', PbackGG, PbackG, PP, posbackGG, posbackG, posP, posGG); // ������������ ������������
					  PG=my_quadratic_interpolation('+', PbackGG, PbackG, PP, posbackGG, posbackG, posP, posG); // ������������ ������������
				      break;
			 case 3 : 
				      PG=my_linear_interpolation('+', PP, PbackGG, posP, posbackGG, posG); // �������� �������������.
			          PGG=my_linear_interpolation('+', Pg, PbackG, posg, posbackG, posGG); // �������� �������������.
				      break;
			 }
		     break;
	case WSIDE: case SSIDE : case BSIDE :
		     switch (i1) {
			 case 0 :
			          PGG=my_linear_interpolation('-', PP, PbackGG, posP, posbackGG, posGG); // �������� �������������.
			          PG=my_linear_interpolation('-', Pg, PbackG, posg, posbackG, posG); // �������� �������������.
			          break;
			 case 1 : 
				      PGG=my_quadratic_interpolation('-', PbackG, PP, Pg, posbackG, posP, posg, posGG); // ������������ ������������
					  PG=my_quadratic_interpolation('-', PbackG, PP, Pg, posbackG, posP, posg, posG); // ������������ ������������
				      break;
			 case 2 :
				      PGG=my_quadratic_interpolation('-', PbackGG, PbackG, PP, posbackGG, posbackG, posP, posGG); // ������������ ������������
					  PG=my_quadratic_interpolation('-', PbackGG, PbackG, PP, posbackGG, posbackG, posP, posG); // ������������ ������������
				      break;
			 case 3 : 
				      PG=my_linear_interpolation('-', PP, PbackGG, posP, posbackGG, posG); // �������� �������������.
			          PGG=my_linear_interpolation('-', Pg, PbackG, posg, posbackG, posGG); // �������� �������������.
				      break;
			 }
		     break;
	} // end switch G
					
	doublereal koef = 0.0;
	switch (G) {
	    case ESIDE : case WSIDE: koef=rhog*dy*dz*dy*dz*alpha; break;
		case NSIDE : case SSIDE : koef=rhog*dx*dz*dx*dz*alpha; break;
		case TSIDE : case BSIDE :koef=rhog*dx*dy*dx*dy*alpha; break;
	} // end switch G	

	// SIMPLEC ��������.
	if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) koef/=(1.0-alpha);
					
					
   doublereal fggplus=0.5*dx/dlbackg, fpplus=0.5*dx/dx, fgplusloc=0.5;
   doublereal apvelP=1.0, apvelG=1.0, apvelg=1.0;
   switch (G) {
       case ESIDE : case WSIDE: apvelP=diag_coef[VX][iP];
		                 apvelg=diag_coef[VX][iG];
		                 break;
	   case NSIDE : case SSIDE : apvelP=diag_coef[VY][iP];
		                 apvelg=diag_coef[VY][iG];
		                 break;
	   case TSIDE : case BSIDE :apvelP=diag_coef[VZ][iP];
		                 apvelg=diag_coef[VZ][iG];
		                 break;
   }
   //apvelG=apvelP; // ��� �������������� apvelg=
   apvelG=apvelP=apvelg; // ������������ ����������� ��� ���������� ���� 
   // ��������� ������� ���������� �� ���� ��� � ������� ��������
   /* // ����� ������ ��������� ������������ ������������� ������������ ��� ������������ :
   switch (G) {
	case ESIDE : case NSIDE : case TSIDE : apvelG=my_linear_interpolation('+', apvelg, apvelP, posg, posP, posG);
		                       break;
    case WSIDE: case SSIDE : case BSIDE :apvelG=my_linear_interpolation('-', apvelg, apvelP, posg, posP, posG);
		                       break;
   }
   */
   
                    
   doublereal FgRhie_Chow=0.0;
					
	switch (G) {
	   case ESIDE : case NSIDE : case TSIDE :
		            // ������������� �����
	                FgRhie_Chow+=koef*(fgplusloc)*(fggplus*PGG+(1.0-fggplus)*PG-fpplus*PP-(1.0-fpplus)*PG)/(apvelG);
                    FgRhie_Chow+=koef*(1.0-fgplusloc)*(fgplusloc*PG+(1.0-fgplusloc)*PP-fggplus*PbackG-(1.0-fggplus)*PP)/(apvelP);
	                FgRhie_Chow-=koef*(PG-PP)/apvelg;
		            break;
	   case WSIDE: case SSIDE : case BSIDE :
		             // �������� ������������� ������������ ���� � ����������� ���� W ����������� ��� ��������� ������� ����������
					// ������ �� ���������� ���������� ����. 
                    FgRhie_Chow+=koef*(1.0-fgplusloc)*(fggplus*PbackG+(1.0-fggplus)*PP-fgplusloc*PG-(1.0-fgplusloc)*PP)/(apvelP);
                    FgRhie_Chow+=koef*(fgplusloc)*(fpplus*PP+(1.0-fpplus)*PG-fggplus*PGG-(1.0-fggplus)*PG)/(apvelG);
			        FgRhie_Chow-=koef*(PP-PG)/apvelg;
		            break;
	}

	return FgRhie_Chow;
					
} // rFgRhieChow_internal_border

// ���� �� ������ ��������� ���� ��� �����������-���������� ������� ����������
// �������� ���-��� � ������������ ����������� ������.
doublereal rFgRhieChow_internal_border(integer iP, integer G, doublereal rhog, doublereal alpha, 
	integer** nvtx, ALICE_PARTITION** sosedi, integer maxelm,
				 doublereal* pressure, TOCHKA* pa, doublereal **diag_coef) {

	integer backG=0, backGG=0; 
	switch (G) {
	    case ESIDE : backG=WSIDE; backGG=WW; break;
		case WSIDE: backG=ESIDE; backGG=EE; break;
		case NSIDE : backG=SSIDE; backGG=SS; break;
		case SSIDE : backG=NSIDE; backGG=NN; break;
		case TSIDE : backG=BSIDE; backGG=BB; break;
		case BSIDE :backG=TSIDE; backGG=TTSIDE; break;
	}

	integer ibackGG=sosedi[backGG][iP].iNODE1; // ����� ���� ������ ������������ ��� �������� �������������
	integer ibackG=sosedi[backG][iP].iNODE1;
	integer iG=sosedi[G][iP].iNODE1;

	// �������� �������������� �������� � ����:
    doublereal PGG=0.0; // �������� � ���� iEE
	doublereal PbackG=0.0, PP=0.0, PG=0.0;
	PbackG=pressure[ibackG]; PP=pressure[iP]; PG=pressure[iG];

	doublereal PbackGG=0.0;
	PbackGG=pressure[ibackGG]; // �������� �������� � ���� ����.

	doublereal posbackGG=0.0, posP=0.0;
	switch (G) {
	   case ESIDE : case WSIDE: 
		                 posbackGG=0.5*(pa[nvtx[1][ibackGG]-1].x+pa[nvtx[0][ibackGG]-1].x); // xWW
		                 posP=0.5*(pa[nvtx[1][iP]-1].x+pa[nvtx[0][iP]-1].x); // xP
				         break;
	   case NSIDE : case SSIDE : 
		                 posbackGG=0.5*(pa[nvtx[2][ibackGG]-1].y+pa[nvtx[0][ibackGG]-1].y); // ySS
		                 posP=0.5*(pa[nvtx[2][iP]-1].y+pa[nvtx[0][iP]-1].y); // yP
				         break;
	   case TSIDE : case BSIDE :
		                 posbackGG=0.5*(pa[nvtx[4][ibackGG]-1].z+pa[nvtx[0][ibackGG]-1].z); // zBB
		                 posP=0.5*(pa[nvtx[4][iP]-1].z+pa[nvtx[0][iP]-1].z); // zP
		                 break;
	} // end switch G

	doublereal dx=0.0, dy=0.0, dz=0.0;
    volume3D(iP, nvtx, pa, dx, dy, dz);

	//doublereal posg, posG, , dlbackg;
	doublereal posGG=0.0;
	switch (G) {
	    case ESIDE : //dlbackg=posP-posbackG;
			     //posg=posP+0.5*dx;
				 //posG=posP+dx;
				 posGG=posP+dx; // +0.5*dx+0.5*dx;
			     break;
		case WSIDE: //dlbackg=posbackG-posP;
			     //posg=posP-0.5*dx;
				 //posG=posP-dx;
				 //posGG=posG-dlbackg; 
			     posGG=posP-dx;
			     break;
		case NSIDE : //dlbackg=posP-posbackG;
			     //posg=posP+0.5*dy;
				 //posG=posP+dy;
				 //posGG=posG+dlbackg;
			     posGG=posP+dy;
			     break;
		case SSIDE : //dlbackg=posbackG-posP;
			     //posg=posP-0.5*dy;
				 //posG=posP-dy;
				 //posGG=posG-dlbackg; 
			     posGG=posP-dy;
			     break;
		case TSIDE : //dlbackg=posP-posbackG;
			     //posg=posP+0.5*dz;
				 //posG=posP+dz;
				 //posGG=posG+dlbackg;
			     posGG=posP+dz;
			     break;
		case BSIDE ://dlbackg=posbackG-posP;
			     //posg=posP-0.5*dz;
				 //posG=posP-dz;
				 //posGG=posG-dlbackg; 
			     posGG=posP-dz;
			     break;
	} // end switch G

	integer i1=0; // ����� ������� ������������

	// �������� �������������:
	switch (G) {
	case ESIDE : case NSIDE : case TSIDE :
		     switch (i1) {
			 case 0 : 
		              PGG=my_linear_interpolation('+', PP, PbackGG, posP, posbackGG, posGG); // �������� �������������.
			          break;
			 }
    case WSIDE: case SSIDE : case BSIDE :
		     switch (i1) {
			 case 0 :
			          PGG=my_linear_interpolation('-', PP, PbackGG, posP, posbackGG, posGG); // �������� �������������.
			          break;
			 }
	}
	
	
	doublereal koef=0.0;
	switch (G) {
	    case ESIDE : case WSIDE: koef=rhog*dy*dz*dy*dz*alpha; break;
		case NSIDE : case SSIDE : koef=rhog*dx*dz*dx*dz*alpha; break;
		case TSIDE : case BSIDE :koef=rhog*dx*dy*dx*dy*alpha; break;
	} // end switch G

	// SIMPLEC ��������.
	if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) koef/=(1.0-alpha);

   doublereal apvelg=1.0;
   switch (G) {
       case ESIDE : case WSIDE: 
		                 apvelg=diag_coef[VX][iG];
		                 break;
	   case NSIDE : case SSIDE : 
		                 apvelg=diag_coef[VY][iG];
		                 break;
	   case TSIDE : case BSIDE :
		                 apvelg=diag_coef[VZ][iG];
		                 break;
   }

    doublereal FgRhie_Chow=0.0;

	switch (G) {
	   case ESIDE : case NSIDE : case TSIDE :
		   // ������������� �����
	       FgRhie_Chow+=koef*(0.5*(PGG+PG)-0.5*(PP+PG))/apvelg;
           FgRhie_Chow-=koef*(PG-PP)/apvelg;
		   break;
	   case WSIDE: case SSIDE : case BSIDE :
		   // ������������� �����
	       FgRhie_Chow+=koef*(0.5*(PP+PG)-0.5*(PGG+PG))/apvelg;
           FgRhie_Chow-=koef*(PP-PG)/apvelg;
		   break;
	}
    

	return FgRhie_Chow;

} // rFgRhieChow_internal_border

// ���� �� ������ ��������� ���� ��� �����������-���������� ������� ����������
// �������� ���-��� � ������������ ����������� ������.
// ����� ��������������� ������  ����� �������� ���-��� � ���������� ��������, � �� � �����
// ������ ��������� ����������� koef.
doublereal ugRhieChow_internal_border(integer iP, integer G, doublereal alpha, 
	integer** nvtx, ALICE_PARTITION** sosedi, integer maxelm,
				 doublereal* pressure, TOCHKA* pa, doublereal **diag_coef) {

	integer backG=0, backGG=0; 
	switch (G) {
	    case ESIDE : backG=WSIDE; backGG=WW; break;
		case WSIDE: backG=ESIDE; backGG=EE; break;
		case NSIDE : backG=SSIDE; backGG=SS; break;
		case SSIDE : backG=NSIDE; backGG=NN; break;
		case TSIDE : backG=BSIDE; backGG=BB; break;
		case BSIDE :backG=TSIDE; backGG=TTSIDE; break;
	}

	integer ibackGG=sosedi[backGG][iP].iNODE1; // ����� ���� ������ ������������ ��� �������� �������������
	integer ibackG=sosedi[backG][iP].iNODE1;
	integer iG=sosedi[G][iP].iNODE1;

	// �������� �������������� �������� � ����:
    doublereal PGG=0.0; // �������� � ���� iEE
	doublereal PbackG=0.0, PP=0.0, PG=0.0;
	PbackG=pressure[ibackG]; PP=pressure[iP]; PG=pressure[iG];

	doublereal PbackGG=0.0;
	PbackGG=pressure[ibackGG]; // �������� �������� � ���� ����.

	doublereal posbackGG=0.0, posP=0.0;
	switch (G) {
	   case ESIDE : case WSIDE: 
		                 posbackGG=0.5*(pa[nvtx[1][ibackGG]-1].x+pa[nvtx[0][ibackGG]-1].x); // xWW
		                 posP=0.5*(pa[nvtx[1][iP]-1].x+pa[nvtx[0][iP]-1].x); // xP
				         break;
	   case NSIDE : case SSIDE : 
		                 posbackGG=0.5*(pa[nvtx[2][ibackGG]-1].y+pa[nvtx[0][ibackGG]-1].y); // ySS
		                 posP=0.5*(pa[nvtx[2][iP]-1].y+pa[nvtx[0][iP]-1].y); // yP
				         break;
	   case TSIDE : case BSIDE :
		                 posbackGG=0.5*(pa[nvtx[4][ibackGG]-1].z+pa[nvtx[0][ibackGG]-1].z); // zBB
		                 posP=0.5*(pa[nvtx[4][iP]-1].z+pa[nvtx[0][iP]-1].z); // zP
		                 break;
	} // end switch G

	doublereal dx=0.0, dy=0.0, dz=0.0;
    volume3D(iP, nvtx, pa, dx, dy, dz);

	//doublereal posg, posG, , dlbackg;
	doublereal posGG=0.0;
	switch (G) {
	    case ESIDE : //dlbackg=posP-posbackG;
			     //posg=posP+0.5*dx;
				 //posG=posP+dx;
				 posGG=posP+dx; // +0.5*dx+0.5*dx;
			     break;
		case WSIDE: //dlbackg=posbackG-posP;
			     //posg=posP-0.5*dx;
				 //posG=posP-dx;
				 //posGG=posG-dlbackg; 
			     posGG=posP-dx;
			     break;
		case NSIDE : //dlbackg=posP-posbackG;
			     //posg=posP+0.5*dy;
				 //posG=posP+dy;
				 //posGG=posG+dlbackg;
			     posGG=posP+dy;
			     break;
		case SSIDE : //dlbackg=posbackG-posP;
			     //posg=posP-0.5*dy;
				 //posG=posP-dy;
				 //posGG=posG-dlbackg; 
			     posGG=posP-dy;
			     break;
		case TSIDE : //dlbackg=posP-posbackG;
			     //posg=posP+0.5*dz;
				 //posG=posP+dz;
				 //posGG=posG+dlbackg;
			     posGG=posP+dz;
			     break;
		case BSIDE ://dlbackg=posbackG-posP;
			     //posg=posP-0.5*dz;
				 //posG=posP-dz;
				 //posGG=posG-dlbackg; 
			     posGG=posP-dz;
			     break;
	} // end switch G

	integer i1=0; // ����� ������� ������������

	// �������� �������������:
	switch (G) {
	case ESIDE : case NSIDE : case TSIDE :
		     switch (i1) {
			 case 0 : 
		              PGG=my_linear_interpolation('+', PP, PbackGG, posP, posbackGG, posGG); // �������� �������������.
			          break;
			 }
    case WSIDE: case SSIDE : case BSIDE :
		     switch (i1) {
			 case 0 :
			          PGG=my_linear_interpolation('-', PP, PbackGG, posP, posbackGG, posGG); // �������� �������������.
			          break;
			 }
	}
	
	
	doublereal koef=0.0;
	switch (G) {
	    case ESIDE : case WSIDE: koef=dy*dz*alpha; break; // rhog*dy*dz*
		case NSIDE : case SSIDE : koef=dx*dz*alpha; break; // rhog*dx*dz*
		case TSIDE : case BSIDE :koef=dx*dy*alpha; break; // rhog*dx*dy*
	} // end switch G

	// SIMPLEC ��������.
	if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) koef/=(1.0-alpha);

   doublereal apvelg=1.0;
   switch (G) {
       case ESIDE : case WSIDE: 
		                 apvelg=diag_coef[VX][iG];
		                 break;
	   case NSIDE : case SSIDE : 
		                 apvelg=diag_coef[VY][iG];
		                 break;
	   case TSIDE : case BSIDE :
		                 apvelg=diag_coef[VZ][iG];
		                 break;
   }

    doublereal FgRhie_Chow=0.0;

	switch (G) {
	   case ESIDE : case NSIDE : case TSIDE :
		   // ������������� �����
	       FgRhie_Chow+=koef*(0.5*(PGG+PG)-0.5*(PP+PG))/apvelg;
           FgRhie_Chow-=koef*(PG-PP)/apvelg;
		   break;
	   case WSIDE: case SSIDE : case BSIDE :
		   // ������������� �����
	       FgRhie_Chow+=koef*(0.5*(PP+PG)-0.5*(PGG+PG))/apvelg;
           FgRhie_Chow-=koef*(PP-PG)/apvelg;
		   break;
	}
    

	return FgRhie_Chow;

} // ugRhieChow_internal_border

#endif