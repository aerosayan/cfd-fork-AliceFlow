// ���� greengauss.c ��������� ��������� ������� 
// � ������� ����������� �������.
// begin 20 ������� 2011 �. 
// ������: �������� ������ ���� ���������� cfd.
// �������� �� ������� �����-������.
// begin 15 ��� 2012 ����. ���������� ������ ������� ��������.

#ifndef GREEN_GAUSS_C
#define GREEN_GAUSS_C 1


// ���������� ���������� ��������� � ������� ���������� ��
// � �� �������� � ������� �������� �������������.
// ��������� ������������� �������� �� �������� ������ ������� O(h). 
// �� ������ �������� O(h) ������, ����� ���� � O(h^2). � ���� �� ���� ��������
// ��� ������ ������ ���������� ����������, ��� ������� ������������� ������������� �����
// ��������� �� ���������� ������� ���������� ���������.
void green_gaussO1(integer iP, doublereal** &potent, integer** &nvtx, TOCHKA* &pa,
	ALICE_PARTITION** &sosedi, integer maxelm, bool bbond,
								 doublereal* &mf, doublereal* &prop, doublereal* &prop_b) {


	// ������������ �� �������� �� ����� � ������� �������� ���-���.
	bool bRCh=false;

	// maxelm - ����� ���������� ��.
	// ��������� ��������� ��������� ��� ���������� ��.
	// ���� bbond == true �� ����� ��������� �������� � ��������� ��, ����� ������ �� ����������.
    // ��������� �� ���������� �� �������� ���������� ������ ���� ��������� � ������ �������. �.�.
	// ���������� ��������� ��� ���������������� ������� ������ �������.

	// iP - ����� ����������� ������������ ������
	// iP ���������� �� 0 �� maxelm-1.
	integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	iE = sosedi[ESIDE][iP].iNODE1; iN = sosedi[NSIDE][iP].iNODE1; iT = sosedi[TSIDE][iP].iNODE1; iW = sosedi[WSIDE][iP].iNODE1; iS = sosedi[SSIDE][iP].iNODE1; iB = sosedi[BSIDE][iP].iNODE1;

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
	doublereal dx=0.0, dy=0.0, dz=0.0;// ����� �������� ������������� ������
	volume3D(iP, nvtx, pa, dx, dy, dz);

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

	// ��������� �� ����� �� ���������������� ������� �������������
	doublereal rhoe=1.0, rhow=1.0, rhon=1.0, rhos=1.0, rhot=1.0, rhob=1.0;
	doublereal rP=1.0, rE=1.0, rN=1.0, rT=1.0, rW=1.0, rS=1.0, rB=1.0;
	if (bRCh) {
		rP=prop[iP];
	    if (!bE) rE=prop[iE]; else rE=prop_b[iE-maxelm];
        if (!bN) rN=prop[iN]; else rN=prop_b[iN-maxelm];
        if (!bT) rT=prop[iT]; else rT=prop_b[iT-maxelm];
	    if (!bW) rW=prop[iW]; else rW=prop_b[iW-maxelm];
        if (!bS) rS=prop[iS]; else rS=prop_b[iS-maxelm];
	    if (!bB) rB=prop[iB]; else rB=prop_b[iB-maxelm];
	    
	    rhoe=rE*rP/(feplus*rE+(1.0-feplus)*rP); // ���������.
	    rhow=rW*rP/(fwplus*rW+(1.0-fwplus)*rP);
	    rhon=rN*rP/(fnplus*rN+(1.0-fnplus)*rP);
	    rhos=rS*rP/(fsplus*rS+(1.0-fsplus)*rP);
        rhot=rT*rP/(ftplus*rT+(1.0-ftplus)*rP);
	    rhob=rB*rP/(fbplus*rB+(1.0-fbplus)*rP);
	}

	// �������� ������������ �������� VX �� ����� ��.
    doublereal fe, fw, fn, fs, ft, fb;
	if (!bbond) {
		// ���������� ��.

		// ������� �������������� �������� �� ����� ������������ ������,
		// � ����� ��������� ����������� � ������ ������������ ������ �� ������� ������� ���������� �������. 

		// VX
	    if (!bE) {
			if (bRCh) {
				fe=mf[ESIDE]/(rhoe*dy*dz); // �������� �� ����� � ������ �������� ���-���.
			}
			else {
				fe=feplus*potent[VXCOR][iE]+(1.0-feplus)*potent[VXCOR][iP]; 
			}
		}
		else fe=potent[VXCOR][iE];
        if (!bW) {
			if (bRCh) {
				fw=mf[WSIDE]/(rhow*dy*dz); // �������� �� ����� � ������ ���������������� �������� ���-���.
			}
			else {
				fw=fwplus*potent[VXCOR][iW]+(1.0-fwplus)*potent[VXCOR][iP];
			}
		}
		else fw=potent[VXCOR][iW];
		// ��� ���������� �������� ���� �� ���� ����� ��������� � ������� ���������������� ��������.
		// ������ � ������������� ���� ������� ��������. ��������� ���������� �������� � ����������� ���������� ��� VX.
	    if (!bN) fn=fnplus*potent[VXCOR][iN]+(1.0-fnplus)*potent[VXCOR][iP]; else fn=potent[VXCOR][iN];
        if (!bS) fs=fsplus*potent[VXCOR][iS]+(1.0-fsplus)*potent[VXCOR][iP]; else fs=potent[VXCOR][iS];
        if (!bT) ft=ftplus*potent[VXCOR][iT]+(1.0-ftplus)*potent[VXCOR][iP]; else ft=potent[VXCOR][iT];
        if (!bB) fb=fbplus*potent[VXCOR][iB]+(1.0-fbplus)*potent[VXCOR][iP]; else fb=potent[VXCOR][iB];
        // �������� VX
	    potent[GRADXVX][iP]=(fe-fw)/dx;
	    potent[GRADYVX][iP]=(fn-fs)/dy;
	    potent[GRADZVX][iP]=(ft-fb)/dz;

		// �������� ������������ �������� VY �� ����� ��.
	    
	    if (!bE) fe=feplus*potent[VYCOR][iE]+(1.0-feplus)*potent[VYCOR][iP]; else fe=potent[VYCOR][iE];
        if (!bW) fw=fwplus*potent[VYCOR][iW]+(1.0-fwplus)*potent[VYCOR][iP]; else fw=potent[VYCOR][iW];
	    if (!bN) {
			if (bRCh) {
				fn=mf[NSIDE]/(rhon*dx*dz);
			}
			else {
				fn=fnplus*potent[VYCOR][iN]+(1.0-fnplus)*potent[VYCOR][iP]; 
			}
		} 
		else fn=potent[VYCOR][iN];
        if (!bS) {
			if (bRCh) {
				fs=mf[SSIDE]/(rhos*dx*dz);
			}
			else {
				fs=fsplus*potent[VYCOR][iS]+(1.0-fsplus)*potent[VYCOR][iP]; 
			}
		}
		else fs=potent[VYCOR][iS];
        if (!bT) ft=ftplus*potent[VYCOR][iT]+(1.0-ftplus)*potent[VYCOR][iP]; else ft=potent[VYCOR][iT];
        if (!bB) fb=fbplus*potent[VYCOR][iB]+(1.0-fbplus)*potent[VYCOR][iP]; else fb=potent[VYCOR][iB];
		
		// �������� VY

	    potent[GRADXVY][iP]=(fe-fw)/dx;
	    potent[GRADYVY][iP]=(fn-fs)/dy;
	    potent[GRADZVY][iP]=(ft-fb)/dz;

	    // �������� ������������ �������� VZ �� ����� ��.
    
	    if (!bE) fe=feplus*potent[VZCOR][iE]+(1.0-feplus)*potent[VZCOR][iP]; else fe=potent[VZCOR][iE];
        if (!bW) fw=fwplus*potent[VZCOR][iW]+(1.0-fwplus)*potent[VZCOR][iP]; else fw=potent[VZCOR][iW];
	    if (!bN) fn=fnplus*potent[VZCOR][iN]+(1.0-fnplus)*potent[VZCOR][iP]; else fn=potent[VZCOR][iN];
        if (!bS) fs=fsplus*potent[VZCOR][iS]+(1.0-fsplus)*potent[VZCOR][iP]; else fs=potent[VZCOR][iS];
        if (!bT) {
			if (bRCh) {
				ft=mf[TSIDE]/(rhot*dx*dy); 
			}
			else {
               ft=ftplus*potent[VZCOR][iT]+(1.0-ftplus)*potent[VZCOR][iP]; 
			}
		} else ft=potent[VZCOR][iT];
        if (!bB) {
			if (bRCh) {
				fb=mf[BSIDE]/(rhob*dx*dy);
			}
			else {
				fb=fbplus*potent[VZCOR][iB]+(1.0-fbplus)*potent[VZCOR][iP];
			}
		} else fb=potent[VZCOR][iB];
		// �������� VZ
	    potent[GRADXVZ][iP]=(fe-fw)/dx;
	    potent[GRADYVZ][iP]=(fn-fs)/dy;
	    potent[GRADZVZ][iP]=(ft-fb)/dz;
	}
	else {

		if (1) {

			

			// �� �������� : �������� �� ������� ��������� �� ���������� ����������� ����.

			if (bE) {

				// 10.02.2017
				// ���� �� ������ ���������� ������� ���������� �� �������� �������� �� ������ ����� ������������ ����� ����.

				doublereal dspeed = sqrt((potent[VXCOR][iE])*(potent[VXCOR][iE]) + (potent[VYCOR][iE])*(potent[VYCOR][iE]) + (potent[VZCOR][iE])*(potent[VZCOR][iE]));

				if (dspeed < 1.0e-10) {
					potent[GRADXVX][iE] = 0.0;
					potent[GRADYVX][iE] = 0.0;
					potent[GRADZVX][iE] = 0.0;

					potent[GRADXVY][iE] = 0.0;
					potent[GRADYVY][iE] = 0.0;
					potent[GRADZVY][iE] = 0.0;

					potent[GRADXVZ][iE] = 0.0;
					potent[GRADYVZ][iE] = 0.0;
					potent[GRADZVZ][iE] = 0.0;

					/*
					potent[GRADXVX][iP] = 0.0;
					potent[GRADYVX][iP] = 0.0;
					potent[GRADZVX][iP] = 0.0;

					potent[GRADXVY][iP] = 0.0;
					potent[GRADYVY][iP] = 0.0;
					potent[GRADZVY][iP] = 0.0;

					potent[GRADXVZ][iP] = 0.0;
					potent[GRADYVZ][iP] = 0.0;
					potent[GRADZVZ][iP] = 0.0;
					*/
				}
				else {

					potent[GRADXVX][iE] = potent[GRADXVX][iP];
					potent[GRADYVX][iE] = potent[GRADYVX][iP];
					potent[GRADZVX][iE] = potent[GRADZVX][iP];

					potent[GRADXVY][iE] = potent[GRADXVY][iP];
					potent[GRADYVY][iE] = potent[GRADYVY][iP];
					potent[GRADZVY][iE] = potent[GRADZVY][iP];

					potent[GRADXVZ][iE] = potent[GRADXVZ][iP];
					potent[GRADYVZ][iE] = potent[GRADYVZ][iP];
					potent[GRADZVZ][iE] = potent[GRADZVZ][iP];
				}
		}

		if (bW) {

			doublereal dspeed = sqrt((potent[VXCOR][iW])*(potent[VXCOR][iW]) + (potent[VYCOR][iW])*(potent[VYCOR][iW]) + (potent[VZCOR][iW])*(potent[VZCOR][iW]));

			if (dspeed < 1.0e-10) {
				potent[GRADXVX][iW] = 0.0;
				potent[GRADYVX][iW] = 0.0;
				potent[GRADZVX][iW] = 0.0;

				potent[GRADXVY][iW] = 0.0;
				potent[GRADYVY][iW] = 0.0;
				potent[GRADZVY][iW] = 0.0;

				potent[GRADXVZ][iW] = 0.0;
				potent[GRADYVZ][iW] = 0.0;
				potent[GRADZVZ][iW] = 0.0;
			}
			else {

				potent[GRADXVX][iW] = potent[GRADXVX][iP];
				potent[GRADYVX][iW] = potent[GRADYVX][iP];
				potent[GRADZVX][iW] = potent[GRADZVX][iP];

				potent[GRADXVY][iW] = potent[GRADXVY][iP];
				potent[GRADYVY][iW] = potent[GRADYVY][iP];
				potent[GRADZVY][iW] = potent[GRADZVY][iP];

				potent[GRADXVZ][iW] = potent[GRADXVZ][iP];
				potent[GRADYVZ][iW] = potent[GRADYVZ][iP];
				potent[GRADZVZ][iW] = potent[GRADZVZ][iP];
			}
		}

		if (bN) {

			doublereal dspeed = sqrt((potent[VXCOR][iN])*(potent[VXCOR][iN]) + (potent[VYCOR][iN])*(potent[VYCOR][iN]) + (potent[VZCOR][iN])*(potent[VZCOR][iN]));

			if (dspeed < 1.0e-10) {
				potent[GRADXVX][iN] = 0.0;
				potent[GRADYVX][iN] = 0.0;
				potent[GRADZVX][iN] = 0.0;

				potent[GRADXVY][iN] = 0.0;
				potent[GRADYVY][iN] = 0.0;
				potent[GRADZVY][iN] = 0.0;

				potent[GRADXVZ][iN] = 0.0;
				potent[GRADYVZ][iN] = 0.0;
				potent[GRADZVZ][iN] = 0.0;
			}
			else {

				potent[GRADXVX][iN] = potent[GRADXVX][iP];
				potent[GRADYVX][iN] = potent[GRADYVX][iP];
				potent[GRADZVX][iN] = potent[GRADZVX][iP];

				potent[GRADXVY][iN] = potent[GRADXVY][iP];
				potent[GRADYVY][iN] = potent[GRADYVY][iP];
				potent[GRADZVY][iN] = potent[GRADZVY][iP];

				potent[GRADXVZ][iN] = potent[GRADXVZ][iP];
				potent[GRADYVZ][iN] = potent[GRADYVZ][iP];
				potent[GRADZVZ][iN] = potent[GRADZVZ][iP];
			}
		}

		if (bS) {

			doublereal dspeed = sqrt((potent[VXCOR][iS])*(potent[VXCOR][iS]) + (potent[VYCOR][iS])*(potent[VYCOR][iS]) + (potent[VZCOR][iS])*(potent[VZCOR][iS]));

			if (dspeed < 1.0e-10) {
				potent[GRADXVX][iS] = 0.0;
				potent[GRADYVX][iS] = 0.0;
				potent[GRADZVX][iS] = 0.0;

				potent[GRADXVY][iS] = 0.0;
				potent[GRADYVY][iS] = 0.0;
				potent[GRADZVY][iS] = 0.0;

				potent[GRADXVZ][iS] = 0.0;
				potent[GRADYVZ][iS] = 0.0;
				potent[GRADZVZ][iS] = 0.0;
			}
			else {

				potent[GRADXVX][iS] = potent[GRADXVX][iP];
				potent[GRADYVX][iS] = potent[GRADYVX][iP];
				potent[GRADZVX][iS] = potent[GRADZVX][iP];

				potent[GRADXVY][iS] = potent[GRADXVY][iP];
				potent[GRADYVY][iS] = potent[GRADYVY][iP];
				potent[GRADZVY][iS] = potent[GRADZVY][iP];

				potent[GRADXVZ][iS] = potent[GRADXVZ][iP];
				potent[GRADYVZ][iS] = potent[GRADYVZ][iP];
				potent[GRADZVZ][iS] = potent[GRADZVZ][iP];
			}
		}

		if (bT) {

			doublereal dspeed = sqrt((potent[VXCOR][iT])*(potent[VXCOR][iT]) + (potent[VYCOR][iT])*(potent[VYCOR][iT]) + (potent[VZCOR][iT])*(potent[VZCOR][iT]));

			if (dspeed < 1.0e-10) {
				potent[GRADXVX][iT] = 0.0;
				potent[GRADYVX][iT] = 0.0;
				potent[GRADZVX][iT] = 0.0;

				potent[GRADXVY][iT] = 0.0;
				potent[GRADYVY][iT] = 0.0;
				potent[GRADZVY][iT] = 0.0;

				potent[GRADXVZ][iT] = 0.0;
				potent[GRADYVZ][iT] = 0.0;
				potent[GRADZVZ][iT] = 0.0;
			}
			else {

				potent[GRADXVX][iT] = potent[GRADXVX][iP];
				potent[GRADYVX][iT] = potent[GRADYVX][iP];
				potent[GRADZVX][iT] = potent[GRADZVX][iP];

				potent[GRADXVY][iT] = potent[GRADXVY][iP];
				potent[GRADYVY][iT] = potent[GRADYVY][iP];
				potent[GRADZVY][iT] = potent[GRADZVY][iP];

				potent[GRADXVZ][iT] = potent[GRADXVZ][iP];
				potent[GRADYVZ][iT] = potent[GRADYVZ][iP];
				potent[GRADZVZ][iT] = potent[GRADZVZ][iP];
			}
		}

		if (bB) {
			doublereal dspeed = sqrt((potent[VXCOR][iB])*(potent[VXCOR][iB]) + (potent[VYCOR][iB])*(potent[VYCOR][iB]) + (potent[VZCOR][iB])*(potent[VZCOR][iB]));

			if (dspeed < 1.0e-10) {
				potent[GRADXVX][iB] = 0.0;
				potent[GRADYVX][iB] = 0.0;
				potent[GRADZVX][iB] = 0.0;

				potent[GRADXVY][iB] = 0.0;
				potent[GRADYVY][iB] = 0.0;
				potent[GRADZVY][iB] = 0.0;

				potent[GRADXVZ][iB] = 0.0;
				potent[GRADYVZ][iB] = 0.0;
				potent[GRADZVZ][iB] = 0.0;
			}
			else {

				potent[GRADXVX][iB] = potent[GRADXVX][iP];
				potent[GRADYVX][iB] = potent[GRADYVX][iP];
				potent[GRADZVX][iB] = potent[GRADZVX][iP];

				potent[GRADXVY][iB] = potent[GRADXVY][iP];
				potent[GRADYVY][iB] = potent[GRADYVY][iP];
				potent[GRADZVY][iB] = potent[GRADZVY][iP];

				potent[GRADXVZ][iB] = potent[GRADXVZ][iP];
				potent[GRADYVZ][iB] = potent[GRADYVZ][iP];
				potent[GRADZVZ][iB] = potent[GRADZVZ][iP];
			}
		}


		}
		else
		{

		// ��������� ����.
		// ��������� � ��������� ����� ����������������� � ������� �������� �������������.

		if (bE) {
			potent[GRADXVX][iE]=potent[GRADXVX][iP]+(dxe/dxw)*(potent[GRADXVX][iP]-potent[GRADXVX][iW]);
			potent[GRADYVX][iE]=potent[GRADYVX][iP]+(dxe/dxw)*(potent[GRADYVX][iP]-potent[GRADYVX][iW]);
			potent[GRADZVX][iE]=potent[GRADZVX][iP]+(dxe/dxw)*(potent[GRADZVX][iP]-potent[GRADZVX][iW]);

			potent[GRADXVY][iE]=potent[GRADXVY][iP]+(dxe/dxw)*(potent[GRADXVY][iP]-potent[GRADXVY][iW]);
			potent[GRADYVY][iE]=potent[GRADYVY][iP]+(dxe/dxw)*(potent[GRADYVY][iP]-potent[GRADYVY][iW]);
			potent[GRADZVY][iE]=potent[GRADZVY][iP]+(dxe/dxw)*(potent[GRADZVY][iP]-potent[GRADZVY][iW]);

			potent[GRADXVZ][iE]=potent[GRADXVZ][iP]+(dxe/dxw)*(potent[GRADXVZ][iP]-potent[GRADXVZ][iW]);
			potent[GRADYVZ][iE]=potent[GRADYVZ][iP]+(dxe/dxw)*(potent[GRADYVZ][iP]-potent[GRADYVZ][iW]);
			potent[GRADZVZ][iE]=potent[GRADZVZ][iP]+(dxe/dxw)*(potent[GRADZVZ][iP]-potent[GRADZVZ][iW]);
		}

		if (bW) {
			potent[GRADXVX][iW]=potent[GRADXVX][iP]+(dxw/dxe)*(potent[GRADXVX][iP]-potent[GRADXVX][iE]);
			potent[GRADYVX][iW]=potent[GRADYVX][iP]+(dxw/dxe)*(potent[GRADYVX][iP]-potent[GRADYVX][iE]);
			potent[GRADZVX][iW]=potent[GRADZVX][iP]+(dxw/dxe)*(potent[GRADZVX][iP]-potent[GRADZVX][iE]);

			potent[GRADXVY][iW]=potent[GRADXVY][iP]+(dxw/dxe)*(potent[GRADXVY][iP]-potent[GRADXVY][iE]);
			potent[GRADYVY][iW]=potent[GRADYVY][iP]+(dxw/dxe)*(potent[GRADYVY][iP]-potent[GRADYVY][iE]);
			potent[GRADZVY][iW]=potent[GRADZVY][iP]+(dxw/dxe)*(potent[GRADZVY][iP]-potent[GRADZVY][iE]);

			potent[GRADXVZ][iW]=potent[GRADXVZ][iP]+(dxw/dxe)*(potent[GRADXVZ][iP]-potent[GRADXVZ][iE]);
			potent[GRADYVZ][iW]=potent[GRADYVZ][iP]+(dxw/dxe)*(potent[GRADYVZ][iP]-potent[GRADYVZ][iE]);
			potent[GRADZVZ][iW]=potent[GRADZVZ][iP]+(dxw/dxe)*(potent[GRADZVZ][iP]-potent[GRADZVZ][iE]);
		}

		if (bN) {
			potent[GRADXVX][iN]=potent[GRADXVX][iP]+(dyn/dys)*(potent[GRADXVX][iP]-potent[GRADXVX][iS]);
			potent[GRADYVX][iN]=potent[GRADYVX][iP]+(dyn/dys)*(potent[GRADYVX][iP]-potent[GRADYVX][iS]);
			potent[GRADZVX][iN]=potent[GRADZVX][iP]+(dyn/dys)*(potent[GRADZVX][iP]-potent[GRADZVX][iS]);

			potent[GRADXVY][iN]=potent[GRADXVY][iP]+(dyn/dys)*(potent[GRADXVY][iP]-potent[GRADXVY][iS]);
			potent[GRADYVY][iN]=potent[GRADYVY][iP]+(dyn/dys)*(potent[GRADYVY][iP]-potent[GRADYVY][iS]);
			potent[GRADZVY][iN]=potent[GRADZVY][iP]+(dyn/dys)*(potent[GRADZVY][iP]-potent[GRADZVY][iS]);

			potent[GRADXVZ][iN]=potent[GRADXVZ][iP]+(dyn/dys)*(potent[GRADXVZ][iP]-potent[GRADXVZ][iS]);
			potent[GRADYVZ][iN]=potent[GRADYVZ][iP]+(dyn/dys)*(potent[GRADYVZ][iP]-potent[GRADYVZ][iS]);
			potent[GRADZVZ][iN]=potent[GRADZVZ][iP]+(dyn/dys)*(potent[GRADZVZ][iP]-potent[GRADZVZ][iS]);
		}

		if (bS) {
			potent[GRADXVX][iS]=potent[GRADXVX][iP]+(dys/dyn)*(potent[GRADXVX][iP]-potent[GRADXVX][iN]);
			potent[GRADYVX][iS]=potent[GRADYVX][iP]+(dys/dyn)*(potent[GRADYVX][iP]-potent[GRADYVX][iN]);
			potent[GRADZVX][iS]=potent[GRADZVX][iP]+(dys/dyn)*(potent[GRADZVX][iP]-potent[GRADZVX][iN]);

			potent[GRADXVY][iS]=potent[GRADXVY][iP]+(dys/dyn)*(potent[GRADXVY][iP]-potent[GRADXVY][iN]);
			potent[GRADYVY][iS]=potent[GRADYVY][iP]+(dys/dyn)*(potent[GRADYVY][iP]-potent[GRADYVY][iN]);
			potent[GRADZVY][iS]=potent[GRADZVY][iP]+(dys/dyn)*(potent[GRADZVY][iP]-potent[GRADZVY][iN]);

			potent[GRADXVZ][iS]=potent[GRADXVZ][iP]+(dys/dyn)*(potent[GRADXVZ][iP]-potent[GRADXVZ][iN]);
			potent[GRADYVZ][iS]=potent[GRADYVZ][iP]+(dys/dyn)*(potent[GRADYVZ][iP]-potent[GRADYVZ][iN]);
			potent[GRADZVZ][iS]=potent[GRADZVZ][iP]+(dys/dyn)*(potent[GRADZVZ][iP]-potent[GRADZVZ][iN]);
		}

		if (bT) {
			potent[GRADXVX][iT]=potent[GRADXVX][iP]+(dzt/dzb)*(potent[GRADXVX][iP]-potent[GRADXVX][iB]);
			potent[GRADYVX][iT]=potent[GRADYVX][iP]+(dzt/dzb)*(potent[GRADYVX][iP]-potent[GRADYVX][iB]);
			potent[GRADZVX][iT]=potent[GRADZVX][iP]+(dzt/dzb)*(potent[GRADZVX][iP]-potent[GRADZVX][iB]);

			potent[GRADXVY][iT]=potent[GRADXVY][iP]+(dzt/dzb)*(potent[GRADXVY][iP]-potent[GRADXVY][iB]);
			potent[GRADYVY][iT]=potent[GRADYVY][iP]+(dzt/dzb)*(potent[GRADYVY][iP]-potent[GRADYVY][iB]);
			potent[GRADZVY][iT]=potent[GRADZVY][iP]+(dzt/dzb)*(potent[GRADZVY][iP]-potent[GRADZVY][iB]);

			potent[GRADXVZ][iT]=potent[GRADXVZ][iP]+(dzt/dzb)*(potent[GRADXVZ][iP]-potent[GRADXVZ][iB]);
			potent[GRADYVZ][iT]=potent[GRADYVZ][iP]+(dzt/dzb)*(potent[GRADYVZ][iP]-potent[GRADYVZ][iB]);
			potent[GRADZVZ][iT]=potent[GRADZVZ][iP]+(dzt/dzb)*(potent[GRADZVZ][iP]-potent[GRADZVZ][iB]);
		}

		if (bB) {
			potent[GRADXVX][iB]=potent[GRADXVX][iP]+(dzb/dzt)*(potent[GRADXVX][iP]-potent[GRADXVX][iT]);
			potent[GRADYVX][iB]=potent[GRADYVX][iP]+(dzb/dzt)*(potent[GRADYVX][iP]-potent[GRADYVX][iT]);
			potent[GRADZVX][iB]=potent[GRADZVX][iP]+(dzb/dzt)*(potent[GRADZVX][iP]-potent[GRADZVX][iT]);

			potent[GRADXVY][iB]=potent[GRADXVY][iP]+(dzb/dzt)*(potent[GRADXVY][iP]-potent[GRADXVY][iT]);
			potent[GRADYVY][iB]=potent[GRADYVY][iP]+(dzb/dzt)*(potent[GRADYVY][iP]-potent[GRADYVY][iT]);
			potent[GRADZVY][iB]=potent[GRADZVY][iP]+(dzb/dzt)*(potent[GRADZVY][iP]-potent[GRADZVY][iT]);

			potent[GRADXVZ][iB]=potent[GRADXVZ][iP]+(dzb/dzt)*(potent[GRADXVZ][iP]-potent[GRADXVZ][iT]);
			potent[GRADYVZ][iB]=potent[GRADYVZ][iP]+(dzb/dzt)*(potent[GRADYVZ][iP]-potent[GRADYVZ][iT]);
			potent[GRADZVZ][iB]=potent[GRADZVZ][iP]+(dzb/dzt)*(potent[GRADZVZ][iP]-potent[GRADZVZ][iT]);
		}

		}
	}

} // green_gaussO1

// ���������� ���������� �������� �������� � ������� ������� �����-������. 
void green_gaussPAM(integer iP, doublereal** &potent, integer** &nvtx, TOCHKA* &pa,
	ALICE_PARTITION** &sosedi, integer maxelm, bool bbond,
					BOUND* &sosedb, integer ls, integer lw, WALL* &w, bool bLRfree) {

	// maxelm - ����� ���������� ��.
	// ��������� ��������� �������� �������� ��� ���������� ��.
	// ���� bbond == true �� ����� ��������� �������� � ��������� ��, ����� ������ �� ����������.
    // ��������� �� ���������� �� �������� ���������� ������ ���� ��������� � ������ �������. �.�.
	// ���������� ��������� ��� ���������������� ������� ������ �������.

	// iP - ����� ����������� ������������ ������
	// iP ���������� �� 0 �� maxelm-1.
	integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	iE = sosedi[ESIDE][iP].iNODE1; iN = sosedi[NSIDE][iP].iNODE1; iT = sosedi[TSIDE][iP].iNODE1; iW = sosedi[WSIDE][iP].iNODE1; iS = sosedi[SSIDE][iP].iNODE1; iB = sosedi[BSIDE][iP].iNODE1;

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
	doublereal dx=0.0, dy=0.0, dz=0.0;// ����� �������� ������������� ������
	volume3D(iP, nvtx, pa, dx, dy, dz);

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

	doublereal PAMe, PAMw, PAMn, PAMs, PAMt, PAMb;

    if (!bbond) {

		if (bLRfree) {

			// ��� ������� ���� �� opening �����.

			// � ������ bLRFree �� ����� ������� ��������������� �������� � ��������� ���� ����� ��������� �������� ���������.
			// �������� �������� ����� ����� ��� ����������� ������������ ������, �.�. ��� ��������� ��� ��������� ����� ���������
			// � ��� ��� ��� �������� ����� ������ ������ ������.

			if (!bE) PAMe=feplus*potent[PAM][iE]+(1.0-feplus)*potent[PAM][iP]; else {
			    // �������� ������������ �������� �� ��������� ���� !!!
				PAMe=potent[PAM][iP]+(dxe/dxw)*(potent[PAM][iP]-potent[PAM][iW]);
			}
             if (!bW) PAMw=fwplus*potent[PAM][iW]+(1.0-fwplus)*potent[PAM][iP]; else {
				 // �������� ������������ �������� �� ��������� ���� !!!
				 PAMw=potent[PAM][iP]+(dxw/dxe)*(potent[PAM][iP]-potent[PAM][iE]);
			 }
	         if (!bN) PAMn=fnplus*potent[PAM][iN]+(1.0-fnplus)*potent[PAM][iP]; else {
				 // �������� ������������ �������� �� ��������� ���� !!!
				 PAMn=potent[PAM][iP]+(dyn/dys)*(potent[PAM][iP]-potent[PAM][iS]);
			 }
             if (!bS) PAMs=fsplus*potent[PAM][iS]+(1.0-fsplus)*potent[PAM][iP]; else {
				 // �������� ������������ �������� �� ��������� ���� !!!
				 PAMs=potent[PAM][iP]+(dys/dyn)*(potent[PAM][iP]-potent[PAM][iN]);
			 }
             if (!bT) PAMt=ftplus*potent[PAM][iT]+(1.0-ftplus)*potent[PAM][iP]; else {
				 // �������� ������������ �������� �� ��������� ���� !!!
				 PAMt=potent[PAM][iP]+(dzt/dzb)*(potent[PAM][iP]-potent[PAM][iB]);
			 }
             if (!bB) PAMb=fbplus*potent[PAM][iB]+(1.0-fbplus)*potent[PAM][iP]; else {
				 // �������� ������������ �������� �� ��������� ���� !!!
				 PAMb=potent[PAM][iP]+(dzb/dzt)*(potent[PAM][iP]-potent[PAM][iT]);
			 }
             // �������� ��������.
	         potent[GRADXPAM][iP]=(PAMe-PAMw)/dx;
	         potent[GRADYPAM][iP]=(PAMn-PAMs)/dy;
	         potent[GRADZPAM][iP]=(PAMt-PAMb)/dz;

		}
		else {


		    // ���������� ��.

		    // ������� �������������� �������� �������� �� ����� ������������ ������,
		    // � ����� ��������� ����������� � ������ ������������ ������ �� ������� ������� ���������� �������. 

		    if (!bE) PAMe=feplus*potent[PAM][iE]+(1.0-feplus)*potent[PAM][iP]; else PAMe=potent[PAM][iE];
            if (!bW) PAMw=fwplus*potent[PAM][iW]+(1.0-fwplus)*potent[PAM][iP]; else PAMw=potent[PAM][iW];
	        if (!bN) PAMn=fnplus*potent[PAM][iN]+(1.0-fnplus)*potent[PAM][iP]; else PAMn=potent[PAM][iN];
            if (!bS) PAMs=fsplus*potent[PAM][iS]+(1.0-fsplus)*potent[PAM][iP]; else PAMs=potent[PAM][iS];
            if (!bT) PAMt=ftplus*potent[PAM][iT]+(1.0-ftplus)*potent[PAM][iP]; else PAMt=potent[PAM][iT];
            if (!bB) PAMb=fbplus*potent[PAM][iB]+(1.0-fbplus)*potent[PAM][iP]; else PAMb=potent[PAM][iB];
            // �������� �������� ��������.
	        potent[GRADXPAM][iP]=(PAMe-PAMw)/dx;
	        potent[GRADYPAM][iP]=(PAMn-PAMs)/dy;
	        potent[GRADZPAM][iP]=(PAMt-PAMb)/dz;
		}

	}
   else {
	    integer interpol=0; // 0 ��� 1 ��� �������� ������������.

		if (interpol==0) {

			if (1) {

				// ������������ �� ��������� � ��� �������� ���������� �� ������� ������ ��������� �����.
				// ������ ����� 6.05.2017.

				if (bE) {
					potent[GRADXPAM][iE]=potent[GRADXPAM][iP];
			        potent[GRADYPAM][iE]=potent[GRADYPAM][iP];
			        potent[GRADZPAM][iE]=potent[GRADZPAM][iP];
		        }

		        if (bW) {
					potent[GRADXPAM][iW]=potent[GRADXPAM][iP];
			        potent[GRADYPAM][iW]=potent[GRADYPAM][iP];
		        	potent[GRADZPAM][iW]=potent[GRADZPAM][iP];
		        }

		        if (bN) {
					potent[GRADYPAM][iN]=potent[GRADYPAM][iP];
			        potent[GRADXPAM][iN]=potent[GRADXPAM][iP];
			        potent[GRADZPAM][iN]=potent[GRADZPAM][iP];
		        }

		        if (bS) {
			       // �� 10.02.2017 ��� �����
			       potent[GRADYPAM][iS]=potent[GRADYPAM][iP];
			       potent[GRADXPAM][iS]=potent[GRADXPAM][iP];
			       potent[GRADZPAM][iS]=potent[GRADZPAM][iP];
		        }

		        if (bT) {
					potent[GRADZPAM][iT]=potent[GRADZPAM][iP];
			        potent[GRADXPAM][iT]=potent[GRADXPAM][iP];
			        potent[GRADYPAM][iT]=potent[GRADYPAM][iP];
		        }

		        if (bB) {
					potent[GRADZPAM][iB]=potent[GRADZPAM][iP];
			        potent[GRADXPAM][iB]=potent[GRADXPAM][iP];
			        potent[GRADYPAM][iB]=potent[GRADYPAM][iP];
		        }

			}
			else if (0) {
				// ��� ����!!!.

				// ���������� ������� ��������� �������.
				// ������ �� ���� ���������� � ������������� ����������.
				// �������� �������� ����� ������������� (�����������) ����������� � ������� ����� �������������� ��� ���������.
				// ������� �� ������� �� ������ �� ���������� ����������� ����.
				// � ���������� � ������� ����������� ���� �� ������� ������ ���������� ���������� ��������, ��
				// � ���������� � ������� ����������� �� ����� ������� �������� �������� ����� ���� � ������ opening �������
				// ��������� �������.
				// ��� ����������������� ����������� ��������� ������� ����������������� ������������� 6.05.2017.				
				

				if (bE) {
					integer inumber = iE - maxelm;
					if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB < (ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
						potent[GRADXPAM][iE] = 0.0;
					}
					else {
						potent[GRADXPAM][iE] = potent[GRADXPAM][iP];
					}

					// �������������� ����������.
					potent[GRADYPAM][iE] = potent[GRADYPAM][iP];
					potent[GRADZPAM][iE] = potent[GRADZPAM][iP];
				}

				if (bW) {
					integer inumber = iW - maxelm;
					if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB < (ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
						potent[GRADXPAM][iW] = 0.0;
					}
					else {						
						potent[GRADXPAM][iW] = potent[GRADXPAM][iP];
					}

					// �������������� ����������.					
					potent[GRADYPAM][iW] = potent[GRADYPAM][iP];
					potent[GRADZPAM][iW] = potent[GRADZPAM][iP];
				}

				if (bN) {
					integer inumber = iN - maxelm;
					if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB < (ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
						potent[GRADYPAM][iN] = 0.0;
					}
					else {
						potent[GRADYPAM][iN] = potent[GRADYPAM][iP];
					}

					// �������������� ����������.					
					potent[GRADXPAM][iN] = potent[GRADXPAM][iP];
					potent[GRADZPAM][iN] = potent[GRADZPAM][iP];
				}

				if (bS) {
					integer inumber = iS - maxelm;

					if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB < (ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
						potent[GRADYPAM][iS] = 0.0;
					}
					else {
						potent[GRADYPAM][iS] = potent[GRADYPAM][iP];
					}

					// �������������� ����������.
					// �� 10.02.2017 ��� �����
					potent[GRADXPAM][iS] = potent[GRADXPAM][iP];
					potent[GRADZPAM][iS] = potent[GRADZPAM][iP];
				}

				if (bT) {
					integer inumber = iT - maxelm;
					if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB < (ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
						potent[GRADZPAM][iT] = 0.0;
					}
					else {
						potent[GRADZPAM][iT] = potent[GRADZPAM][iP];
					}

					// �������������� ����������.						
					potent[GRADXPAM][iT] = potent[GRADXPAM][iP];
					potent[GRADYPAM][iT] = potent[GRADYPAM][iP];
				}

				if (bB) {
					integer inumber = iB - maxelm;
					if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB < (ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
						potent[GRADZPAM][iB] = 0.0;
					}
					else {
						potent[GRADZPAM][iB] = potent[GRADZPAM][iP];
					}

					// �������������� ����������.					
					potent[GRADXPAM][iB] = potent[GRADXPAM][iP];
					potent[GRADYPAM][iB] = potent[GRADYPAM][iP];
				}
			}
			else {

			if (bE) {
				integer inumber=iE-maxelm;
				if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
					potent[GRADXPAM][iE]=potent[GRADXPAM][iP];
				}
				else if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
					potent[GRADXPAM][iE] = potent[GRADXPAM][iP];
				}
				else {
					potent[GRADXPAM][iE]=0.0;
				}
			
				//potent[GRADXPAM][iE] = potent[GRADXPAM][iP];

			    potent[GRADYPAM][iE]=potent[GRADYPAM][iP];
			    potent[GRADZPAM][iE]=potent[GRADZPAM][iP];
		    }

		if (bW) {
			integer inumber=iW-maxelm;
				if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
					potent[GRADXPAM][iW]=potent[GRADXPAM][iP];
				}
				else if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
					potent[GRADXPAM][iW] = potent[GRADXPAM][iP];
				}
				else {
					// ����������� �� �������� �� ������� ����� ����, ������ ��������� �������.
					potent[GRADXPAM][iW]=0.0;
				}

				//potent[GRADXPAM][iW] = potent[GRADXPAM][iP];

			potent[GRADYPAM][iW]=potent[GRADYPAM][iP];
			potent[GRADZPAM][iW]=potent[GRADZPAM][iP];
		}

		if (bN) {

			integer inumber=iN-maxelm;
				if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
					potent[GRADYPAM][iN]=potent[GRADYPAM][iP];
				}
				else if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
					potent[GRADYPAM][iN] = potent[GRADYPAM][iP];
				}
				else {
					// ����������� �� �������� �� ������� ����� ����, ������ ��������� �������.
					potent[GRADYPAM][iN]=0.0;
				}

				//potent[GRADYPAM][iN] = potent[GRADYPAM][iP];

			potent[GRADXPAM][iN]=potent[GRADXPAM][iP];
			potent[GRADZPAM][iN]=potent[GRADZPAM][iP];
		}

		if (bS) {

                integer inumber=iS-maxelm;
				if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
					potent[GRADYPAM][iS]=potent[GRADYPAM][iS];
				}
				else if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
					potent[GRADYPAM][iS] = potent[GRADYPAM][iS];
				}
				else {
					// ����������� �� �������� �� ������� ����� ����, ������ ��������� �������.
					potent[GRADYPAM][iS]=0.0;
				}

				//potent[GRADYPAM][iS] = potent[GRADYPAM][iS];

			potent[GRADXPAM][iS]=potent[GRADXPAM][iP];
			potent[GRADZPAM][iS]=potent[GRADZPAM][iP];
		}

		if (bT) {

			integer inumber=iT-maxelm;
			if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
				potent[GRADZPAM][iT]=potent[GRADZPAM][iT];
			}
			else if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
				potent[GRADZPAM][iT] = potent[GRADZPAM][iT];
			}
			else {
				// ����������� �� �������� �� ������� ����� ����, ������ ��������� �������.
				potent[GRADZPAM][iT]=0.0;
			}

			//potent[GRADZPAM][iT] = potent[GRADZPAM][iT];

			potent[GRADXPAM][iT]=potent[GRADXPAM][iP];
			potent[GRADYPAM][iT]=potent[GRADYPAM][iP];
		}

		if (bB) {

			integer inumber=iB-maxelm;
			if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
				potent[GRADZPAM][iB]=potent[GRADZPAM][iB];
			}
			else if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
				potent[GRADZPAM][iB] = potent[GRADZPAM][iB];
			}
			else {
				// ����������� �� �������� �� ������� ����� ����, ������ ��������� �������.
				potent[GRADZPAM][iB]=0.0;
			}

			//potent[GRADZPAM][iB] = potent[GRADZPAM][iB];

			potent[GRADXPAM][iB]=potent[GRADXPAM][iP];
			potent[GRADYPAM][iB]=potent[GRADYPAM][iP];
		}

			}

		}
		else if (interpol==1) {

		// ��������� ����.
		// ��������� � ��������� ����� ����������������� � ������� �������� �������������.

		// ���� ������ � �������� <-- ���������������� �� �������� �������� �������� ������� ���������������� �� ������� 
		// ��������� ������� ������� ��������� �������.

		if (bE) {

			integer inumber=iE-maxelm;
			if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
				//potent[GRADXPAM][iE]=potent[GRADXPAM][iP]+(dxe/dxw)*(potent[GRADXPAM][iP]-potent[GRADXPAM][iW]);
				potent[GRADXPAM][iE]=0.0;
			}
			else if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
				//potent[GRADXPAM][iE]=potent[GRADXPAM][iP]+(dxe/dxw)*(potent[GRADXPAM][iP]-potent[GRADXPAM][iW]);
				potent[GRADXPAM][iE] = 0.0;
			}
			else {
				// ����������� �� �������� �� ������� ����� ����, ������ ��������� �������.
				potent[GRADXPAM][iE]=0.0;
			}

			potent[GRADYPAM][iE]=potent[GRADYPAM][iP]+(dxe/dxw)*(potent[GRADYPAM][iP]-potent[GRADYPAM][iW]);
			potent[GRADZPAM][iE]=potent[GRADZPAM][iP]+(dxe/dxw)*(potent[GRADZPAM][iP]-potent[GRADZPAM][iW]);
			potent[GRADXPAM][iE]=potent[GRADXPAM][iP]+(dxe/dxw)*(potent[GRADXPAM][iP]-potent[GRADXPAM][iW]); // <--

		}

		if (bW) {

			integer inumber=iW-maxelm;
			if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
				//potent[GRADXPAM][iW]=potent[GRADXPAM][iP]+(dxw/dxe)*(potent[GRADXPAM][iP]-potent[GRADXPAM][iE]);
				potent[GRADXPAM][iW]=0.0;
			}
			else if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
				//potent[GRADXPAM][iW]=potent[GRADXPAM][iP]+(dxw/dxe)*(potent[GRADXPAM][iP]-potent[GRADXPAM][iE]);
				potent[GRADXPAM][iW] = 0.0;
			}
			else {
				// ����������� �� �������� �� ������� ����� ����, ������ ��������� �������.
				potent[GRADXPAM][iW]=0.0;
			}

			potent[GRADYPAM][iW]=potent[GRADYPAM][iP]+(dxw/dxe)*(potent[GRADYPAM][iP]-potent[GRADYPAM][iE]);
			potent[GRADZPAM][iW]=potent[GRADZPAM][iP]+(dxw/dxe)*(potent[GRADZPAM][iP]-potent[GRADZPAM][iE]);
			potent[GRADXPAM][iW]=potent[GRADXPAM][iP]+(dxw/dxe)*(potent[GRADXPAM][iP]-potent[GRADXPAM][iE]); // <--

		}

		if (bN) {

			integer inumber=iN-maxelm;
				if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
					//potent[GRADYPAM][iN]=potent[GRADYPAM][iP]+(dyn/dys)*(potent[GRADYPAM][iP]-potent[GRADYPAM][iS]);
					potent[GRADYPAM][iN]=0.0;
				}
				else if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
					//potent[GRADYPAM][iN]=potent[GRADYPAM][iP]+(dyn/dys)*(potent[GRADYPAM][iP]-potent[GRADYPAM][iS]);
					potent[GRADYPAM][iN] = 0.0;
				}
				else {
					// ����������� �� �������� �� ������� ����� ����, ������ ��������� �������.
					potent[GRADYPAM][iN]=0.0;
				}

			potent[GRADXPAM][iN]=potent[GRADXPAM][iP]+(dyn/dys)*(potent[GRADXPAM][iP]-potent[GRADXPAM][iS]);
			potent[GRADZPAM][iN]=potent[GRADZPAM][iP]+(dyn/dys)*(potent[GRADZPAM][iP]-potent[GRADZPAM][iS]);
			potent[GRADYPAM][iN]=potent[GRADYPAM][iP]+(dyn/dys)*(potent[GRADYPAM][iP]-potent[GRADYPAM][iS]); // <--
		}

		if (bS) {

			integer inumber=iS-maxelm;
			if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
				//potent[GRADYPAM][iS]=potent[GRADYPAM][iP]+(dys/dyn)*(potent[GRADYPAM][iP]-potent[GRADYPAM][iN]);
				potent[GRADYPAM][iS]=0.0;
			}
			else if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
				//potent[GRADYPAM][iS]=potent[GRADYPAM][iP]+(dys/dyn)*(potent[GRADYPAM][iP]-potent[GRADYPAM][iN]);
				potent[GRADYPAM][iS] = 0.0;
			}
			else {
			    // ����������� �� �������� �� ������� ����� ����, ������ ��������� �������.
				potent[GRADYPAM][iS]=0.0;
			}


			potent[GRADXPAM][iS]=potent[GRADXPAM][iP]+(dys/dyn)*(potent[GRADXPAM][iP]-potent[GRADXPAM][iN]);
			potent[GRADZPAM][iS]=potent[GRADZPAM][iP]+(dys/dyn)*(potent[GRADZPAM][iP]-potent[GRADZPAM][iN]);
			potent[GRADYPAM][iS]=potent[GRADYPAM][iP]+(dys/dyn)*(potent[GRADYPAM][iP]-potent[GRADYPAM][iN]); //<--

		}

		if (bT) {

            integer inumber=iT-maxelm;
			if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
				//potent[GRADZPAM][iT]=potent[GRADZPAM][iP]+(dzt/dzb)*(potent[GRADZPAM][iP]-potent[GRADZPAM][iB]);
				potent[GRADZPAM][iT]=0.0;
			}
			else if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
				//potent[GRADZPAM][iT]=potent[GRADZPAM][iP]+(dzt/dzb)*(potent[GRADZPAM][iP]-potent[GRADZPAM][iB]);
				potent[GRADZPAM][iT] = 0.0;
			}
			else {
				// ����������� �� �������� �� ������� ����� ����, ������ ��������� �������.
				potent[GRADZPAM][iT]=0.0;
			}

			potent[GRADXPAM][iT]=potent[GRADXPAM][iP]+(dzt/dzb)*(potent[GRADXPAM][iP]-potent[GRADXPAM][iB]);
			potent[GRADYPAM][iT]=potent[GRADYPAM][iP]+(dzt/dzb)*(potent[GRADYPAM][iP]-potent[GRADYPAM][iB]);
			potent[GRADZPAM][iT]=potent[GRADZPAM][iP]+(dzt/dzb)*(potent[GRADZPAM][iP]-potent[GRADZPAM][iB]); // <--
		}

		if (bB) {

			integer inumber=iB-maxelm;
			if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
				//potent[GRADZPAM][iB]=potent[GRADZPAM][iP]+(dzb/dzt)*(potent[GRADZPAM][iP]-potent[GRADZPAM][iT]);
				potent[GRADZPAM][iB]=0.0;
			}
			else if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
				//potent[GRADZPAM][iB]=potent[GRADZPAM][iP]+(dzb/dzt)*(potent[GRADZPAM][iP]-potent[GRADZPAM][iT]);
				potent[GRADZPAM][iB] = 0.0;
			}
			else {
				// ����������� �� �������� �� ������� ����� ����, ������ ��������� �������.
				potent[GRADZPAM][iB]=0.0;
			}

			potent[GRADXPAM][iB]=potent[GRADXPAM][iP]+(dzb/dzt)*(potent[GRADXPAM][iP]-potent[GRADXPAM][iT]);
			potent[GRADYPAM][iB]=potent[GRADYPAM][iP]+(dzb/dzt)*(potent[GRADYPAM][iP]-potent[GRADYPAM][iT]);
			potent[GRADZPAM][iB]=potent[GRADZPAM][iP]+(dzb/dzt)*(potent[GRADZPAM][iP]-potent[GRADZPAM][iT]); // <--
		}
		}
	}


} // green_gaussPAM

// 13 ������ 2015 ����.
// ���������� ���������� ����������� � ������� ������� �����-������. 
void green_gaussTemperature(integer iP, doublereal* &potent, integer** &nvtx, TOCHKA* &pa,
	ALICE_PARTITION** &sosedi, integer maxelm, bool bbond,
					BOUND* &sosedb, doublereal* &Tx, doublereal* &Ty, doublereal* &Tz) {

	// maxelm - ����� ���������� ��.
	// ��������� ��������� �������� �������� ��� ���������� ��.
	// ���� bbond == true �� ����� ��������� �������� ������ � ��������� ��, ����� ������ �� ����������.
    // ��������� �� ���������� �� �������� ���������� ������ ���� ��������� � ������ �������. �.�.
	// ���������� ��������� ��� ���������������� ������� ������ �������.

	// �������� ! ������ ��� Tx, Ty, Tz �������������� ���������� �������.

	// iP - ����� ����������� ������������ ������
	// iP ���������� �� 0 �� maxelm-1.
	integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	iE = sosedi[ESIDE][iP].iNODE1; iN = sosedi[NSIDE][iP].iNODE1; iT = sosedi[TSIDE][iP].iNODE1; iW = sosedi[WSIDE][iP].iNODE1; iS = sosedi[SSIDE][iP].iNODE1; iB = sosedi[BSIDE][iP].iNODE1;

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
	doublereal dx=0.0, dy=0.0, dz=0.0;// ����� �������� ������������� ������
	volume3D(iP, nvtx, pa, dx, dy, dz);

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

	doublereal Te, Tw, Tn, Ts, Tt, Tb;

    if (!bbond) {
		// ���������� ��.

		// ������� �������������� �������� �������� �� ����� ������������ ������,
		// � ����� ��������� ����������� � ������ ������������ ������ �� ������� ������� ���������� �������. 

		if (!bE) Te=feplus*potent[iE]+(1.0-feplus)*potent[iP]; else Te=potent[iE];
        if (!bW) Tw=fwplus*potent[iW]+(1.0-fwplus)*potent[iP]; else Tw=potent[iW];
	    if (!bN) Tn=fnplus*potent[iN]+(1.0-fnplus)*potent[iP]; else Tn=potent[iN];
        if (!bS) Ts=fsplus*potent[iS]+(1.0-fsplus)*potent[iP]; else Ts=potent[iS];
        if (!bT) Tt=ftplus*potent[iT]+(1.0-ftplus)*potent[iP]; else Tt=potent[iT];
        if (!bB) Tb=fbplus*potent[iB]+(1.0-fbplus)*potent[iP]; else Tb=potent[iB];
        // �������� VX
	    Tx[iP]=(Te-Tw)/dx;
	    Ty[iP]=(Tn-Ts)/dy;
	    Tz[iP]=(Tt-Tb)/dz;

	}
   else {
	    integer interpol=0; // 0 ��� 1 ��� �������� ������������.

		if (interpol==0) {


				if (bE) {
				
			
				Tx[iE]=Tx[iP];
			    Ty[iE]=Ty[iP];
			    Tz[iE]=Tz[iP];
		    }

		if (bW) {
			

			Tx[iW]=Tx[iP];
			Ty[iW]=Ty[iP];
			Tz[iW]=Tz[iP];
		}

		if (bN) {

			Tx[iN]=Tx[iP];
			Ty[iN]=Ty[iP];
			Tz[iN]=Tz[iP];
		}

		if (bS) {
			Tx[iS]=Tx[iP];
			Ty[iS]=Ty[iP];
			Tz[iS]=Tz[iP];
		}

		if (bT) {

			

			Tx[iT]=Tx[iP];
			Ty[iT]=Ty[iP];
			Tz[iT]=Tz[iP];
		}

		if (bB) {

			

			Tx[iB]=Tx[iP];
			Ty[iB]=Ty[iP];
			Tz[iB]=Tz[iP];
		}


			

		}
		else if (interpol==1) {

		// ��������� ����.
		// ��������� � ��������� ����� ����������������� � ������� �������� �������������.

		// ���� ������ � �������� <-- ���������������� �� �������� �������� �������� ������� ���������������� �� ������� 
		// ��������� ������� ������� ��������� �������.

		if (bE) {
			

			Ty[iE]=Ty[iP]+(dxe/dxw)*(Ty[iP]-Ty[iW]);
			Tz[iE]=Tz[iP]+(dxe/dxw)*(Tz[iP]-Tz[iW]);
			Tx[iE]=Tx[iP]+(dxe/dxw)*(Tx[iP]-Tx[iW]); // <--

		}

		if (bW) {
			

			Ty[iW]=Ty[iP]+(dxw/dxe)*(Ty[iP]-Ty[iE]);
			Tz[iW]=Tz[iP]+(dxw/dxe)*(Tz[iP]-Tz[iE]);
			Tx[iW]=Tx[iP]+(dxw/dxe)*(Tx[iP]-Tx[iE]); // <--

		}

		if (bN) {

			

			Tx[iN]=Tx[iP]+(dyn/dys)*(Tx[iP]-Tx[iS]);
			Tz[iN]=Tz[iP]+(dyn/dys)*(Tz[iP]-Tz[iS]);
			Ty[iN]=Ty[iP]+(dyn/dys)*(Ty[iP]-Ty[iS]); // <--
		}

		if (bS) {

			

			Tx[iS]=Tx[iP]+(dys/dyn)*(Tx[iP]-Tx[iN]);
			Tz[iS]=Tz[iP]+(dys/dyn)*(Tz[iP]-Tz[iN]);
			Ty[iS]=Ty[iP]+(dys/dyn)*(Ty[iP]-Ty[iN]); //<--

		}

		if (bT) {

			

			Tx[iT]=Tx[iP]+(dzt/dzb)*(Tx[iP]-Tx[iB]);
			Ty[iT]=Ty[iP]+(dzt/dzb)*(Ty[iP]-Ty[iB]);
			Tz[iT]=Tz[iP]+(dzt/dzb)*(Tz[iP]-Tz[iB]); // <--
		}

		if (bB) {

			

			Tx[iB]=Tx[iP]+(dzb/dzt)*(Tx[iP]-Tx[iT]);
			Ty[iB]=Ty[iP]+(dzb/dzt)*(Ty[iP]-Ty[iT]);
			Tz[iB]=Tz[iP]+(dzb/dzt)*(Tz[iP]-Tz[iT]); // <--
		}
		}
	}


} // green_gaussTemperature



// ������������ ����������� ��������� �������� ��������.
// ������� ��� ������ ��������� ����.
void green_gaussPAMminmax(doublereal** &potent, ALICE_PARTITION** &sosedi, integer maxelm, integer maxbound) {
	

	doublereal** minmaxlimitergrad=new doublereal*[3];
	for (integer i=0; i<3; i++) {
		minmaxlimitergrad[i]=new doublereal[maxelm+maxbound];
	}

	for (integer iP=0; iP<maxelm; iP++) {
	   // iP - ����� ����������� ������������ ������
	   // iP ���������� �� 0 �� maxelm-1.
	   integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	   iE = sosedi[ESIDE][iP].iNODE1; iN = sosedi[NSIDE][iP].iNODE1; iT = sosedi[TSIDE][iP].iNODE1; iW = sosedi[WSIDE][iP].iNODE1; iS = sosedi[SSIDE][iP].iNODE1; iB = sosedi[BSIDE][iP].iNODE1;

	   // ����������� ����������� ������ ��������� ������ ����������.
	   minmaxlimitergrad[VX][iP]=fmax(fmin(fmax(potent[GRADXPAM][iE],potent[GRADXPAM][iW]),potent[GRADXPAM][iP]),fmin(potent[GRADXPAM][iE],potent[GRADXPAM][iW]));
	   minmaxlimitergrad[VY][iP]=fmax(fmin(fmax(potent[GRADYPAM][iN],potent[GRADYPAM][iS]),potent[GRADYPAM][iP]),fmin(potent[GRADYPAM][iN],potent[GRADYPAM][iS]));
	   minmaxlimitergrad[VZ][iP]=fmax(fmin(fmax(potent[GRADZPAM][iT],potent[GRADZPAM][iB]),potent[GRADZPAM][iP]),fmin(potent[GRADZPAM][iT],potent[GRADZPAM][iB]));
	}

	for (integer iP=0; iP<maxelm; iP++) {

		// iP - ����� ����������� ������������ ������
	    // iP ���������� �� 0 �� maxelm-1.
	    integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	    iE=sosedi[ESIDE][iP].iNODE1; iN=sosedi[NSIDE][iP].iNODE1; iT=sosedi[TSIDE][iP].iNODE1; iW=sosedi[WSIDE][iP].iNODE1; iS=sosedi[SSIDE][iP].iNODE1; iB=sosedi[BSIDE][iP].iNODE1;

		// ���� � ����� �� ������ ����� ������� ��������� �������
	    // �� ��������������� ���������� ����� true
	    bool bE=false, bN=false, bT=false, bW=false, bS=false, bB=false;
    
	    if (iE>=maxelm) bE=true;
	    if (iN>=maxelm) bN=true;
	    if (iT>=maxelm) bT=true;
        if (iW>=maxelm) bW=true;
	    if (iS>=maxelm) bS=true;
	    if (iB>=maxelm) bB=true;

		// ��������� ����.
		// ��������� � ��������� ����� ����������������� ������� ��������� �� ���������� ����������� ����.

		if (bE) {
			minmaxlimitergrad[VX][iE]=minmaxlimitergrad[VX][iP];
			minmaxlimitergrad[VY][iE]=minmaxlimitergrad[VY][iP];
			minmaxlimitergrad[VZ][iE]=minmaxlimitergrad[VZ][iP];
		}

		if (bW) {
			minmaxlimitergrad[VX][iW]=minmaxlimitergrad[VX][iP];
			minmaxlimitergrad[VY][iW]=minmaxlimitergrad[VY][iP];
			minmaxlimitergrad[VZ][iW]=minmaxlimitergrad[VZ][iP];
		}

		if (bN) {
			minmaxlimitergrad[VX][iN]=minmaxlimitergrad[VX][iP];
			minmaxlimitergrad[VY][iN]=minmaxlimitergrad[VY][iP];
			minmaxlimitergrad[VZ][iN]=minmaxlimitergrad[VZ][iP];
		}

		if (bS) {
			minmaxlimitergrad[VX][iS]=minmaxlimitergrad[VX][iP];
			minmaxlimitergrad[VY][iS]=minmaxlimitergrad[VY][iP];
			minmaxlimitergrad[VZ][iS]=minmaxlimitergrad[VZ][iP];
		}

		if (bT) {
			minmaxlimitergrad[VX][iT]=minmaxlimitergrad[VX][iP];
			minmaxlimitergrad[VY][iT]=minmaxlimitergrad[VY][iP];
			minmaxlimitergrad[VZ][iT]=minmaxlimitergrad[VZ][iP];
		}

		if (bB) {
			minmaxlimitergrad[VX][iB]=minmaxlimitergrad[VX][iP];
			minmaxlimitergrad[VY][iB]=minmaxlimitergrad[VY][iP];
			minmaxlimitergrad[VZ][iB]=minmaxlimitergrad[VZ][iP];
		}
	}

	// �������� �����������.
	for (integer iP=0; iP<maxelm+maxbound; iP++) {
		potent[GRADXPAM][iP]=minmaxlimitergrad[VX][iP];
		potent[GRADYPAM][iP]=minmaxlimitergrad[VY][iP];
		potent[GRADZPAM][iP]=minmaxlimitergrad[VZ][iP];
	}

	// ������������ ������.
	if (minmaxlimitergrad != NULL) {
		for (integer i = 0; i < 3; i++) {
			if (minmaxlimitergrad[i] != NULL) {
				delete[] minmaxlimitergrad[i];
				minmaxlimitergrad[i] = NULL;
			}
		}
		delete[] minmaxlimitergrad;
		minmaxlimitergrad = NULL;
	}

} // gaussPAMminmax

// ���������� ���������� �������� � ������� ������� �����-������. 
// begin 21 ���� 2012 ����.
void green_gaussPRESS(integer iP, doublereal** &potent, integer** &nvtx, TOCHKA* &pa,
	ALICE_PARTITION** &sosedi, integer maxelm, bool bbond,
					BOUND* &sosedb, integer ls, integer lw, WALL* &w, bool bLRfree) {

	// maxelm - ����� ���������� ��.
	// ��������� ��������� �������� ��� ���������� ��.
	// ���� bbond == true �� ����� ��������� �������� � ��������� ��, ����� ������ �� ����������.
    // ��������� �� ���������� �� �������� ���������� ������ ���� ��������� � ������ �������. �.�.
	// ���������� ��������� ��� ���������������� ������� ������ �������.

	// iP - ����� ����������� ������������ ������
	// iP ���������� �� 0 �� maxelm-1.
	integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	iE = sosedi[ESIDE][iP].iNODE1; iN = sosedi[NSIDE][iP].iNODE1; iT = sosedi[TSIDE][iP].iNODE1; iW = sosedi[WSIDE][iP].iNODE1; iS = sosedi[SSIDE][iP].iNODE1; iB = sosedi[BSIDE][iP].iNODE1;

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
	doublereal dx=0.0, dy=0.0, dz=0.0;// ����� �������� ������������� ������
	volume3D(iP, nvtx, pa, dx, dy, dz);

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

	doublereal PRESSe, PRESSw, PRESSn, PRESSs, PRESSt, PRESSb;

    if (!bbond) {
		// ���������� ��.

		// ������� �������������� �������� �� ����� ������������ ������,
		// � ����� ��������� ����������� � ������ ������������ ������ �� ������� ������� ���������� �������. 

		if (bLRfree) {

			// � ������ bLRFree �� ����� ������� ��������������� �������� � ��������� ���� ����� ��������� �������� ���������.
			// �������� �������� ����� ����� ��� ����������� ������������ ������, �.�. ��� ��������� ��� ��������� ����� ���������
			// � ��� ��� ��� �������� ����� ������ ������ ������.

			if (!bE) PRESSe=feplus*potent[PRESS][iE]+(1.0-feplus)*potent[PRESS][iP]; else {
				//PRESSe=potent[PRESS][iE]; 
				//potent[GRADXPRESS][iE]=potent[GRADXPRESS][iP]+(dxe/dxw)*(potent[GRADXPRESS][iP]-potent[GRADXPRESS][iW]);
			    // �������� ������������ �������� �� ��������� ���� !!!
				PRESSe=potent[PRESS][iP]+(dxe/dxw)*(potent[PRESS][iP]-potent[PRESS][iW]);
			}
             if (!bW) PRESSw=fwplus*potent[PRESS][iW]+(1.0-fwplus)*potent[PRESS][iP]; else {
				 //PRESSw=potent[PRESS][iW];
				 //potent[GRADXPRESS][iW]=potent[GRADXPRESS][iP]+(dxw/dxe)*(potent[GRADXPRESS][iP]-potent[GRADXPRESS][iE]);
				 // �������� ������������ �������� �� ��������� ���� !!!
				 PRESSw=potent[PRESS][iP]+(dxw/dxe)*(potent[PRESS][iP]-potent[PRESS][iE]);
			 }
	         if (!bN) PRESSn=fnplus*potent[PRESS][iN]+(1.0-fnplus)*potent[PRESS][iP]; else {
				 //PRESSn=potent[PRESS][iN];
				 //potent[GRADYPRESS][iN]=potent[GRADYPRESS][iP]+(dyn/dys)*(potent[GRADYPRESS][iP]-potent[GRADYPRESS][iS]);
				 // �������� ������������ �������� �� ��������� ���� !!!
				 PRESSn=potent[PRESS][iP]+(dyn/dys)*(potent[PRESS][iP]-potent[PRESS][iS]);
			 }
             if (!bS) PRESSs=fsplus*potent[PRESS][iS]+(1.0-fsplus)*potent[PRESS][iP]; else {
				 //PRESSs=potent[PRESS][iS];
				 //potent[GRADYPRESS][iS]=potent[GRADYPRESS][iP]+(dys/dyn)*(potent[GRADYPRESS][iP]-potent[GRADYPRESS][iN]);
				 // �������� ������������ �������� �� ��������� ���� !!!
				 PRESSs=potent[PRESS][iP]+(dys/dyn)*(potent[PRESS][iP]-potent[PRESS][iN]);
			 }
             if (!bT) PRESSt=ftplus*potent[PRESS][iT]+(1.0-ftplus)*potent[PRESS][iP]; else {
				 PRESSt=potent[PRESS][iT];
				 //potent[GRADZPRESS][iT]=potent[GRADZPRESS][iP]+(dzt/dzb)*(potent[GRADZPRESS][iP]-potent[GRADZPRESS][iB]);
				 // �������� ������������ �������� �� ��������� ���� !!!
				 PRESSt=potent[PRESS][iP]+(dzt/dzb)*(potent[PRESS][iP]-potent[PRESS][iB]);
			 }
             if (!bB) PRESSb=fbplus*potent[PRESS][iB]+(1.0-fbplus)*potent[PRESS][iP]; else {
				 PRESSb=potent[PRESS][iB];
				 //potent[GRADZPRESS][iB]=potent[GRADZPRESS][iP]+(dzb/dzt)*(potent[GRADZPRESS][iP]-potent[GRADZPRESS][iT]);
				 // �������� ������������ �������� �� ��������� ���� !!!
				 PRESSb=potent[PRESS][iP]+(dzb/dzt)*(potent[PRESS][iP]-potent[PRESS][iT]);
			 }
             // �������� ��������.
	         potent[GRADXPRESS][iP]=(PRESSe-PRESSw)/dx;
	         potent[GRADYPRESS][iP]=(PRESSn-PRESSs)/dy;
	         potent[GRADZPRESS][iP]=(PRESSt-PRESSb)/dz;

		}
		else {

		     if (!bE) PRESSe=feplus*potent[PRESS][iE]+(1.0-feplus)*potent[PRESS][iP]; else PRESSe=potent[PRESS][iE]; // ��������� !
             if (!bW) PRESSw=fwplus*potent[PRESS][iW]+(1.0-fwplus)*potent[PRESS][iP]; else PRESSw=potent[PRESS][iW];
	         if (!bN) PRESSn=fnplus*potent[PRESS][iN]+(1.0-fnplus)*potent[PRESS][iP]; else PRESSn=potent[PRESS][iN];
             if (!bS) PRESSs=fsplus*potent[PRESS][iS]+(1.0-fsplus)*potent[PRESS][iP]; else PRESSs=potent[PRESS][iS];
             if (!bT) PRESSt=ftplus*potent[PRESS][iT]+(1.0-ftplus)*potent[PRESS][iP]; else PRESSt=potent[PRESS][iT];
             if (!bB) PRESSb=fbplus*potent[PRESS][iB]+(1.0-fbplus)*potent[PRESS][iP]; else PRESSb=potent[PRESS][iB];
             // �������� ��������.
	         potent[GRADXPRESS][iP]=(PRESSe-PRESSw)/dx;
	         potent[GRADYPRESS][iP]=(PRESSn-PRESSs)/dy;
	         potent[GRADZPRESS][iP]=(PRESSt-PRESSb)/dz;

		}

	}
   else {

	   integer interpol=0; // 0 ��� 1 ��� �������� ������������.

	   if (interpol==0) {

		   if (1) {

			   if (bE) {
				   potent[GRADXPRESS][iE]=potent[GRADXPRESS][iP];
			       potent[GRADYPRESS][iE]=potent[GRADYPRESS][iP];
			       potent[GRADZPRESS][iE]=potent[GRADZPRESS][iP];
		       }

		       if (bW) {
    				potent[GRADXPRESS][iW]=potent[GRADXPRESS][iP];
			        potent[GRADYPRESS][iW]=potent[GRADYPRESS][iP];
			        potent[GRADZPRESS][iW]=potent[GRADZPRESS][iP];
		       }

		       if (bN) {
			       potent[GRADYPRESS][iN]=potent[GRADYPRESS][iP];
			       potent[GRADXPRESS][iN]=potent[GRADXPRESS][iP];
			       potent[GRADZPRESS][iN]=potent[GRADZPRESS][iP];
		       }

		       if (bS) {
			      potent[GRADYPRESS][iS]=potent[GRADYPRESS][iP];
			      potent[GRADXPRESS][iS]=potent[GRADXPRESS][iP];
			      potent[GRADZPRESS][iS]=potent[GRADZPRESS][iP];
		       }

		       if (bT) {
			      potent[GRADZPRESS][iT]=potent[GRADZPRESS][iP];
			      potent[GRADXPRESS][iT]=potent[GRADXPRESS][iP];
			      potent[GRADYPRESS][iT]=potent[GRADYPRESS][iP];
		      }

		      if (bB) {
                 potent[GRADZPRESS][iB]=potent[GRADZPRESS][iP];
			     potent[GRADXPRESS][iB]=potent[GRADXPRESS][iP];
			     potent[GRADYPRESS][iB]=potent[GRADYPRESS][iP];
		      }


		   }
		   else {

		   if (bE) {

			   integer inumber=iE-maxelm;
				if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
					potent[GRADXPRESS][iE]=potent[GRADXPRESS][iP];
				}
				else if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
					potent[GRADXPRESS][iE] = potent[GRADXPRESS][iP];
				}
				else {
					potent[GRADXPRESS][iE]=0.0;
				}

			potent[GRADYPRESS][iE]=potent[GRADYPRESS][iP];
			potent[GRADZPRESS][iE]=potent[GRADZPRESS][iP];
		}

		if (bW) {

			integer inumber=iW-maxelm;
				if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
					potent[GRADXPRESS][iW]=potent[GRADXPRESS][iP];
				}
				else if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
					potent[GRADXPRESS][iW] = potent[GRADXPRESS][iP];
				}
				else {
					potent[GRADXPRESS][iW]=0.0;
				}

			potent[GRADYPRESS][iW]=potent[GRADYPRESS][iP];
			potent[GRADZPRESS][iW]=potent[GRADZPRESS][iP];
		}

		if (bN) {

			integer inumber=iN-maxelm;
				if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
					potent[GRADYPRESS][iN]=potent[GRADYPRESS][iP];
				}
				else if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
					potent[GRADYPRESS][iN] = potent[GRADYPRESS][iP];
				}
				else {
					potent[GRADYPRESS][iN]=0.0;
				}

			potent[GRADXPRESS][iN]=potent[GRADXPRESS][iP];
			potent[GRADZPRESS][iN]=potent[GRADZPRESS][iP];
		}

		if (bS) {

			integer inumber=iS-maxelm;
				if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
					potent[GRADYPRESS][iS]=potent[GRADYPRESS][iP];
				}
				else if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
					potent[GRADYPRESS][iS] = potent[GRADYPRESS][iP];
				}
				else {
					potent[GRADYPRESS][iS]=0.0;
				}

			potent[GRADXPRESS][iS]=potent[GRADXPRESS][iP];
			potent[GRADZPRESS][iS]=potent[GRADZPRESS][iP];
		}

		if (bT) {

			integer inumber=iT-maxelm;
				if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
					potent[GRADZPRESS][iT]=potent[GRADZPRESS][iP];
				}
				else if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
					potent[GRADZPRESS][iT] = potent[GRADZPRESS][iP];
				}
				else {
					potent[GRADZPRESS][iT]=0.0;
				}

			potent[GRADXPRESS][iT]=potent[GRADXPRESS][iP];
			potent[GRADYPRESS][iT]=potent[GRADYPRESS][iP];
		}

		if (bB) {

                integer inumber=iB-maxelm;
				if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
					potent[GRADZPRESS][iB]=potent[GRADZPRESS][iP];
				}
				else if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
					potent[GRADZPRESS][iB] = potent[GRADZPRESS][iP];
				}
				else {
					potent[GRADZPRESS][iB]=0.0;
				}

			potent[GRADXPRESS][iB]=potent[GRADXPRESS][iP];
			potent[GRADYPRESS][iB]=potent[GRADYPRESS][iP];
		}

		   }
	   }
	   else if (interpol==1) {

        
		// ��������� ����.
		// ��������� � ��������� ����� ����������������� � ������� �������� �������������.

		if (bE) {

			 integer inumber=iE-maxelm;
				if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
					//potent[GRADXPRESS][iE]=potent[GRADXPRESS][iP]+(dxe/dxw)*(potent[GRADXPRESS][iP]-potent[GRADXPRESS][iW]);
					potent[GRADXPRESS][iE]=0.0;
				}
				else {
					potent[GRADXPRESS][iE]=0.0;
				}

			
			potent[GRADYPRESS][iE]=potent[GRADYPRESS][iP]+(dxe/dxw)*(potent[GRADYPRESS][iP]-potent[GRADYPRESS][iW]);
			potent[GRADZPRESS][iE]=potent[GRADZPRESS][iP]+(dxe/dxw)*(potent[GRADZPRESS][iP]-potent[GRADZPRESS][iW]);
			//potent[GRADXPRESS][iE]=potent[GRADXPRESS][iP]+(dxe/dxw)*(potent[GRADXPRESS][iP]-potent[GRADXPRESS][iW]); // <--
		}

		if (bW) {

                integer inumber=iW-maxelm;
				if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
					//potent[GRADXPRESS][iW]=potent[GRADXPRESS][iP]+(dxw/dxe)*(potent[GRADXPRESS][iP]-potent[GRADXPRESS][iE]);
					potent[GRADXPRESS][iW]=0.0;
				}
				else {
					potent[GRADXPRESS][iW]=0.0;
				}

			
			potent[GRADYPRESS][iW]=potent[GRADYPRESS][iP]+(dxw/dxe)*(potent[GRADYPRESS][iP]-potent[GRADYPRESS][iE]);
			potent[GRADZPRESS][iW]=potent[GRADZPRESS][iP]+(dxw/dxe)*(potent[GRADZPRESS][iP]-potent[GRADZPRESS][iE]);
            //potent[GRADXPRESS][iW]=potent[GRADXPRESS][iP]+(dxw/dxe)*(potent[GRADXPRESS][iP]-potent[GRADXPRESS][iE]); // <--
		}

		if (bN) {

                integer inumber=iN-maxelm;
				if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
					//potent[GRADYPRESS][iN]=potent[GRADYPRESS][iP]+(dyn/dys)*(potent[GRADYPRESS][iP]-potent[GRADYPRESS][iS]);
					potent[GRADYPRESS][iN]=0.0;
				}
				else {
					potent[GRADYPRESS][iN]=0.0;
				}

			potent[GRADXPRESS][iN]=potent[GRADXPRESS][iP]+(dyn/dys)*(potent[GRADXPRESS][iP]-potent[GRADXPRESS][iS]);
			potent[GRADZPRESS][iN]=potent[GRADZPRESS][iP]+(dyn/dys)*(potent[GRADZPRESS][iP]-potent[GRADZPRESS][iS]);
			//potent[GRADYPRESS][iN]=potent[GRADYPRESS][iP]+(dyn/dys)*(potent[GRADYPRESS][iP]-potent[GRADYPRESS][iS]); // <--
		}

		if (bS) {

			integer inumber=iS-maxelm;
				if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
					//potent[GRADYPRESS][iS]=potent[GRADYPRESS][iP]+(dys/dyn)*(potent[GRADYPRESS][iP]-potent[GRADYPRESS][iN]);
					potent[GRADYPRESS][iS]=0.0;
				}
				else {
					potent[GRADYPRESS][iS]=0.0;
				}

			potent[GRADXPRESS][iS]=potent[GRADXPRESS][iP]+(dys/dyn)*(potent[GRADXPRESS][iP]-potent[GRADXPRESS][iN]);
			potent[GRADZPRESS][iS]=potent[GRADZPRESS][iP]+(dys/dyn)*(potent[GRADZPRESS][iP]-potent[GRADZPRESS][iN]);
			//potent[GRADYPRESS][iS]=potent[GRADYPRESS][iP]+(dys/dyn)*(potent[GRADYPRESS][iP]-potent[GRADYPRESS][iN]); // <--
		}

		if (bT) {

			integer inumber=iT-maxelm;
				if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
					//potent[GRADZPRESS][iT]=potent[GRADZPRESS][iP]+(dzt/dzb)*(potent[GRADZPRESS][iP]-potent[GRADZPRESS][iB]);
					potent[GRADZPRESS][iT]=0.0;
				}
				else {
					potent[GRADZPRESS][iT]=0.0;
				}

			potent[GRADXPRESS][iT]=potent[GRADXPRESS][iP]+(dzt/dzb)*(potent[GRADXPRESS][iP]-potent[GRADXPRESS][iB]);
			potent[GRADYPRESS][iT]=potent[GRADYPRESS][iP]+(dzt/dzb)*(potent[GRADYPRESS][iP]-potent[GRADYPRESS][iB]);
			//potent[GRADZPRESS][iT]=potent[GRADZPRESS][iP]+(dzt/dzb)*(potent[GRADZPRESS][iP]-potent[GRADZPRESS][iB]);//<--
			
		}

		if (bB) {

                integer inumber=iB-maxelm;
				if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure)) {
					//potent[GRADZPRESS][iB]=potent[GRADZPRESS][iP]+(dzb/dzt)*(potent[GRADZPRESS][iP]-potent[GRADZPRESS][iT]);
					potent[GRADZPRESS][iB]=0.0;
				}
				else {
					potent[GRADZPRESS][iB]=0.0;
				}

			potent[GRADXPRESS][iB]=potent[GRADXPRESS][iP]+(dzb/dzt)*(potent[GRADXPRESS][iP]-potent[GRADXPRESS][iT]);
			potent[GRADYPRESS][iB]=potent[GRADYPRESS][iP]+(dzb/dzt)*(potent[GRADYPRESS][iP]-potent[GRADYPRESS][iT]);
			//potent[GRADZPRESS][iB]=potent[GRADZPRESS][iP]+(dzb/dzt)*(potent[GRADZPRESS][iP]-potent[GRADZPRESS][iT]);//<--
			
		}

	   }
	}


} // green_gaussPRESS

// ���������� ���������� ��������� � ������� ���������� ��
// � �� �������� � ������� ������������ �������������.
// ��������� ������������� ������������ �� �������� ������ ������� O(h^2).
// ������ ������� ����������� 15 ��� 2012 ����.
void green_gaussO2(integer iP, doublereal** &potent, integer** &nvtx, TOCHKA* &pa,
	ALICE_PARTITION** &sosedi, integer maxelm, bool bbond) {
	// maxelm - ����� ���������� ��.
	// ��������� ��������� ��������� ��� ���������� ��.
	// ���� bbond == true �� ����� ��������� �������� � ��������� ��, ����� ������ �� ����������.
    // ��������� �� ���������� �� �������� ���������� ������ ���� ��������� � ������ �������. �.�.
	// ���������� ��������� ��� ���������������� ������� ������ �������.

	// iP - ����� ����������� ������������ ������
	// iP ���������� �� 0 �� maxelm-1.
	integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	iE = sosedi[ESIDE][iP].iNODE1; iN = sosedi[NSIDE][iP].iNODE1; iT = sosedi[TSIDE][iP].iNODE1; iW = sosedi[WSIDE][iP].iNODE1; iS = sosedi[SSIDE][iP].iNODE1; iB = sosedi[BSIDE][iP].iNODE1;

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
	doublereal dx=0.0, dy=0.0, dz=0.0;// ����� �������� ������������� ������
	volume3D(iP, nvtx, pa, dx, dy, dz);


	if (!bbond) {
		// ���������� ��.

		// ������������� �������� �� ����� �.�. ������� �������������� ������ ������� ��������.


		doublereal hxminus, hxplus, hyminus, hyplus, hzminus, hzplus;
		// ���������� �������.
		doublereal VXP, VXW, VXE, VXN, VXS, VXT, VXB;
		doublereal VYP, VYW, VYE, VYN, VYS, VYT, VYB;
		doublereal VZP, VZW, VZE, VZN, VZS, VZT, VZB;

		VXP=potent[VX][iP]; VYP=potent[VY][iP]; VZP=potent[VZ][iP]; // �������� ��������� � ����������� ����������� ������.
		VXW=potent[VX][iW]; VYW=potent[VY][iW]; VZW=potent[VZ][iW];
		VXE=potent[VX][iE]; VYE=potent[VY][iE]; VZE=potent[VZ][iE];
		VXS=potent[VX][iS]; VYS=potent[VY][iS]; VZS=potent[VZ][iS]; 
		VXN=potent[VX][iN]; VYN=potent[VY][iN]; VZN=potent[VZ][iN]; 
		VXB=potent[VX][iB]; VYB=potent[VY][iB]; VZB=potent[VZ][iB];  
		VXT=potent[VX][iT]; VYT=potent[VY][iT]; VZT=potent[VZ][iT];

		if (!bW) {
			TOCHKA pp, pb;
		    center_cord3D(iP, nvtx, pa, pp,100);
		    center_cord3D(iW, nvtx, pa, pb,WSIDE);
		    hxminus=fabs(pp.x-pb.x);		    
	    } else {
		    // ���� W ���������
			hxminus=0.5*dx;
	    }
	    if (!bE) {
		    TOCHKA pp, pb;
		    center_cord3D(iP, nvtx, pa, pp,100);
		    center_cord3D(iE, nvtx, pa, pb,ESIDE);
		    hxplus=fabs(pb.x-pp.x);		
	    } else {
            // ���� E ���������
		    hxplus=0.5*dx;
       	}
	    if (!bS) {
		    TOCHKA pp, pb;
		    center_cord3D(iP, nvtx, pa, pp,100);
		    center_cord3D(iS, nvtx, pa, pb,SSIDE);
		    hyminus=fabs(pp.y-pb.y);
	    } else {
		    // ���� S ���������
		    hyminus=0.5*dy;
		}
	    if (!bN) {
		    TOCHKA pp, pb;
		    center_cord3D(iP, nvtx, pa, pp,100);
		    center_cord3D(iN, nvtx, pa, pb,NSIDE);
		    hyplus=fabs(pb.y-pp.y);		
	    } else {
		    // ���� N ���������
		    hyplus=0.5*dy;
	    } 
		if (!bB) {
		    TOCHKA pp, pb;
		    center_cord3D(iP, nvtx, pa, pp,100);
		    center_cord3D(iB, nvtx, pa, pb,BSIDE);
		    hzminus=fabs(pp.z-pb.z);
		} else {
		    // ���� B ���������
		    hzminus=0.5*dz;
	    }
	    if (!bT) { 
		    TOCHKA pp, pb;
		    center_cord3D(iP, nvtx, pa, pp,100);
		    center_cord3D(iT, nvtx, pa, pb,TSIDE);
		    hzplus=fabs(pb.z-pp.z);
	    } else {
		    // ���� T ���������
		    hzplus=0.5*dz;
	    }

		// VX
		potent[GRADXVX][iP]=rgradF(VXW, VXP, VXE, hxminus, hxplus); // ������ ������� ��������.
		potent[GRADYVX][iP]=rgradF(VXS, VXP, VXN, hyminus, hyplus);
	    potent[GRADZVX][iP]=rgradF(VXB, VXP, VXT, hzminus, hzplus);
		// VY
		potent[GRADXVY][iP]=rgradF(VYW, VYP, VYE, hxminus, hxplus);
	    potent[GRADYVY][iP]=rgradF(VYS, VYP, VYN, hyminus, hyplus);
	    potent[GRADZVY][iP]=rgradF(VYB, VYP, VYT, hzminus, hzplus);
		// VZ
		potent[GRADXVZ][iP]=rgradF(VZW, VZP, VZE, hxminus, hxplus);
	    potent[GRADYVZ][iP]=rgradF(VZS, VZP, VZN, hyminus, hyplus);
	    potent[GRADZVZ][iP]=rgradF(VZB, VZP, VZT, hzminus, hzplus);

	}
	else {
		// ��������� ����.
		// ��������� � ��������� ����� ����������������� � ������� ������������ �������������.

		if (bE) {

			// ������������ �������������.

			TOCHKA pp,pb,pbb;
		    center_cord3D(iP, nvtx, pa, pp,100);
		    center_cord3D(iW, nvtx, pa, pb,WSIDE);
			center_cord3D(sosedi[WSIDE][iW].iNODE1, nvtx, pa, pbb,WW);
					
			potent[GRADXVX][iE] = my_quadratic_interpolation('+', potent[GRADXVX][sosedi[WSIDE][iW].iNODE1], potent[GRADXVX][iW], potent[GRADXVX][iP], pbb.x, pb.x, pp.x, pp.x + 0.5*dx);
			potent[GRADYVX][iE] = my_quadratic_interpolation('+', potent[GRADYVX][sosedi[WSIDE][iW].iNODE1], potent[GRADYVX][iW], potent[GRADYVX][iP], pbb.x, pb.x, pp.x, pp.x + 0.5*dx);
			potent[GRADZVX][iE] = my_quadratic_interpolation('+', potent[GRADZVX][sosedi[WSIDE][iW].iNODE1], potent[GRADZVX][iW], potent[GRADZVX][iP], pbb.x, pb.x, pp.x, pp.x + 0.5*dx);

			potent[GRADXVY][iE] = my_quadratic_interpolation('+', potent[GRADXVY][sosedi[WSIDE][iW].iNODE1], potent[GRADXVY][iW], potent[GRADXVY][iP], pbb.x, pb.x, pp.x, pp.x + 0.5*dx);
			potent[GRADYVY][iE] = my_quadratic_interpolation('+', potent[GRADYVY][sosedi[WSIDE][iW].iNODE1], potent[GRADYVY][iW], potent[GRADYVY][iP], pbb.x, pb.x, pp.x, pp.x + 0.5*dx);
			potent[GRADZVY][iE] = my_quadratic_interpolation('+', potent[GRADZVY][sosedi[WSIDE][iW].iNODE1], potent[GRADZVY][iW], potent[GRADZVY][iP], pbb.x, pb.x, pp.x, pp.x + 0.5*dx);

			potent[GRADXVZ][iE] = my_quadratic_interpolation('+', potent[GRADXVZ][sosedi[WSIDE][iW].iNODE1], potent[GRADXVZ][iW], potent[GRADXVZ][iP], pbb.x, pb.x, pp.x, pp.x + 0.5*dx);
			potent[GRADYVZ][iE] = my_quadratic_interpolation('+', potent[GRADYVZ][sosedi[WSIDE][iW].iNODE1], potent[GRADYVZ][iW], potent[GRADYVZ][iP], pbb.x, pb.x, pp.x, pp.x + 0.5*dx);
			potent[GRADZVZ][iE] = my_quadratic_interpolation('+', potent[GRADZVZ][sosedi[WSIDE][iW].iNODE1], potent[GRADZVZ][iW], potent[GRADZVZ][iP], pbb.x, pb.x, pp.x, pp.x + 0.5*dx);
		}

		if (bW) {

			// ������������ �������������.

			TOCHKA pp, pb,pbb;
		    center_cord3D(iP, nvtx, pa, pp,100);
		    center_cord3D(iE, nvtx, pa, pb,ESIDE);
			center_cord3D(sosedi[ESIDE][iE].iNODE1, nvtx, pa, pbb,EE);

			potent[GRADXVX][iW] = my_quadratic_interpolation('-', potent[GRADXVX][sosedi[ESIDE][iE].iNODE1], potent[GRADXVX][iE], potent[GRADXVX][iP], pbb.x, pb.x, pp.x, pp.x - 0.5*dx);
			potent[GRADYVX][iW] = my_quadratic_interpolation('-', potent[GRADYVX][sosedi[ESIDE][iE].iNODE1], potent[GRADYVX][iE], potent[GRADYVX][iP], pbb.x, pb.x, pp.x, pp.x - 0.5*dx);
			potent[GRADZVX][iW] = my_quadratic_interpolation('-', potent[GRADZVX][sosedi[ESIDE][iE].iNODE1], potent[GRADZVX][iE], potent[GRADZVX][iP], pbb.x, pb.x, pp.x, pp.x - 0.5*dx);

			potent[GRADXVY][iW] = my_quadratic_interpolation('-', potent[GRADXVY][sosedi[ESIDE][iE].iNODE1], potent[GRADXVY][iE], potent[GRADXVY][iP], pbb.x, pb.x, pp.x, pp.x - 0.5*dx);
			potent[GRADYVY][iW] = my_quadratic_interpolation('-', potent[GRADYVY][sosedi[ESIDE][iE].iNODE1], potent[GRADYVY][iE], potent[GRADYVY][iP], pbb.x, pb.x, pp.x, pp.x - 0.5*dx);
			potent[GRADZVY][iW] = my_quadratic_interpolation('-', potent[GRADZVY][sosedi[ESIDE][iE].iNODE1], potent[GRADZVY][iE], potent[GRADZVY][iP], pbb.x, pb.x, pp.x, pp.x - 0.5*dx);

			potent[GRADXVZ][iW] = my_quadratic_interpolation('-', potent[GRADXVZ][sosedi[ESIDE][iE].iNODE1], potent[GRADXVZ][iE], potent[GRADXVZ][iP], pbb.x, pb.x, pp.x, pp.x - 0.5*dx);
			potent[GRADYVZ][iW] = my_quadratic_interpolation('-', potent[GRADYVZ][sosedi[ESIDE][iE].iNODE1], potent[GRADYVZ][iE], potent[GRADYVZ][iP], pbb.x, pb.x, pp.x, pp.x - 0.5*dx);
			potent[GRADZVZ][iW] = my_quadratic_interpolation('-', potent[GRADZVZ][sosedi[ESIDE][iE].iNODE1], potent[GRADZVZ][iE], potent[GRADZVZ][iP], pbb.x, pb.x, pp.x, pp.x - 0.5*dx);
		}

		if (bN) {

			// ������������ �������������.

            TOCHKA pp,pb,pbb;
		    center_cord3D(iP, nvtx, pa, pp,100);
		    center_cord3D(iS, nvtx, pa, pb,SSIDE);
			center_cord3D(sosedi[SSIDE][iS].iNODE1, nvtx, pa, pbb,SS);

			potent[GRADXVX][iN] = my_quadratic_interpolation('+', potent[GRADXVX][sosedi[SSIDE][iS].iNODE1], potent[GRADXVX][iS], potent[GRADXVX][iP], pbb.y, pb.y, pp.y, pp.y + 0.5*dy);
			potent[GRADYVX][iN] = my_quadratic_interpolation('+', potent[GRADYVX][sosedi[SSIDE][iS].iNODE1], potent[GRADYVX][iS], potent[GRADYVX][iP], pbb.y, pb.y, pp.y, pp.y + 0.5*dy);
			potent[GRADZVX][iN] = my_quadratic_interpolation('+', potent[GRADZVX][sosedi[SSIDE][iS].iNODE1], potent[GRADZVX][iS], potent[GRADZVX][iP], pbb.y, pb.y, pp.y, pp.y + 0.5*dy);

			potent[GRADXVY][iN] = my_quadratic_interpolation('+', potent[GRADXVY][sosedi[SSIDE][iS].iNODE1], potent[GRADXVY][iS], potent[GRADXVY][iP], pbb.y, pb.y, pp.y, pp.y + 0.5*dy);
			potent[GRADYVY][iN] = my_quadratic_interpolation('+', potent[GRADYVY][sosedi[SSIDE][iS].iNODE1], potent[GRADYVY][iS], potent[GRADYVY][iP], pbb.y, pb.y, pp.y, pp.y + 0.5*dy);
			potent[GRADZVY][iN] = my_quadratic_interpolation('+', potent[GRADZVY][sosedi[SSIDE][iS].iNODE1], potent[GRADZVY][iS], potent[GRADZVY][iP], pbb.y, pb.y, pp.y, pp.y + 0.5*dy);

			potent[GRADXVZ][iN] = my_quadratic_interpolation('+', potent[GRADXVZ][sosedi[SSIDE][iS].iNODE1], potent[GRADXVZ][iS], potent[GRADXVZ][iP], pbb.y, pb.y, pp.y, pp.y + 0.5*dy);
			potent[GRADYVZ][iN] = my_quadratic_interpolation('+', potent[GRADYVZ][sosedi[SSIDE][iS].iNODE1], potent[GRADYVZ][iS], potent[GRADYVZ][iP], pbb.y, pb.y, pp.y, pp.y + 0.5*dy);
			potent[GRADZVZ][iN] = my_quadratic_interpolation('+', potent[GRADZVZ][sosedi[SSIDE][iS].iNODE1], potent[GRADZVZ][iS], potent[GRADZVZ][iP], pbb.y, pb.y, pp.y, pp.y + 0.5*dy);
		}

		if (bS) {

            // ������������ �������������.

            TOCHKA pp,pb,pbb;
		    center_cord3D(iP, nvtx, pa, pp, 100);
		    center_cord3D(iN, nvtx, pa, pb, NSIDE);
			center_cord3D(sosedi[NSIDE][iN].iNODE1, nvtx, pa, pbb, NN);

			potent[GRADXVX][iS] = my_quadratic_interpolation('-', potent[GRADXVX][sosedi[NSIDE][iN].iNODE1], potent[GRADXVX][iN], potent[GRADXVX][iP], pbb.y, pb.y, pp.y, pp.y - 0.5*dy);
			potent[GRADYVX][iS] = my_quadratic_interpolation('-', potent[GRADYVX][sosedi[NSIDE][iN].iNODE1], potent[GRADYVX][iN], potent[GRADYVX][iP], pbb.y, pb.y, pp.y, pp.y - 0.5*dy);
			potent[GRADZVX][iS] = my_quadratic_interpolation('-', potent[GRADZVX][sosedi[NSIDE][iN].iNODE1], potent[GRADZVX][iN], potent[GRADZVX][iP], pbb.y, pb.y, pp.y, pp.y - 0.5*dy);

			potent[GRADXVY][iS] = my_quadratic_interpolation('-', potent[GRADXVY][sosedi[NSIDE][iN].iNODE1], potent[GRADXVY][iN], potent[GRADXVY][iP], pbb.y, pb.y, pp.y, pp.y - 0.5*dy);
			potent[GRADYVY][iS] = my_quadratic_interpolation('-', potent[GRADYVY][sosedi[NSIDE][iN].iNODE1], potent[GRADYVY][iN], potent[GRADYVY][iP], pbb.y, pb.y, pp.y, pp.y - 0.5*dy);
			potent[GRADZVY][iS] = my_quadratic_interpolation('-', potent[GRADZVY][sosedi[NSIDE][iN].iNODE1], potent[GRADZVY][iN], potent[GRADZVY][iP], pbb.y, pb.y, pp.y, pp.y - 0.5*dy);

			potent[GRADXVZ][iS] = my_quadratic_interpolation('-', potent[GRADXVZ][sosedi[NSIDE][iN].iNODE1], potent[GRADXVZ][iN], potent[GRADXVZ][iP], pbb.y, pb.y, pp.y, pp.y - 0.5*dy);
			potent[GRADYVZ][iS] = my_quadratic_interpolation('-', potent[GRADYVZ][sosedi[NSIDE][iN].iNODE1], potent[GRADYVZ][iN], potent[GRADYVZ][iP], pbb.y, pb.y, pp.y, pp.y - 0.5*dy);
			potent[GRADZVZ][iS] = my_quadratic_interpolation('-', potent[GRADZVZ][sosedi[NSIDE][iN].iNODE1], potent[GRADZVZ][iN], potent[GRADZVZ][iP], pbb.y, pb.y, pp.y, pp.y - 0.5*dy);
		}

		if (bT) {

			// ������������ �������������.

            TOCHKA pp,pb,pbb;
		    center_cord3D(iP, nvtx, pa, pp,100);
		    center_cord3D(iB, nvtx, pa, pb,BSIDE);
			center_cord3D(sosedi[BSIDE][iB].iNODE1, nvtx, pa, pbb,BB);
					
			potent[GRADXVX][iT] = my_quadratic_interpolation('+', potent[GRADXVX][sosedi[BSIDE][iB].iNODE1], potent[GRADXVX][iB], potent[GRADXVX][iP], pbb.z, pb.z, pp.z, pp.z + 0.5*dz);
			potent[GRADYVX][iT] = my_quadratic_interpolation('+', potent[GRADYVX][sosedi[BSIDE][iB].iNODE1], potent[GRADYVX][iB], potent[GRADYVX][iP], pbb.z, pb.z, pp.z, pp.z + 0.5*dz);
			potent[GRADZVX][iT] = my_quadratic_interpolation('+', potent[GRADZVX][sosedi[BSIDE][iB].iNODE1], potent[GRADZVX][iB], potent[GRADZVX][iP], pbb.z, pb.z, pp.z, pp.z + 0.5*dz);

			potent[GRADXVY][iT] = my_quadratic_interpolation('+', potent[GRADXVY][sosedi[BSIDE][iB].iNODE1], potent[GRADXVY][iB], potent[GRADXVY][iP], pbb.z, pb.z, pp.z, pp.z + 0.5*dz);
			potent[GRADYVY][iT] = my_quadratic_interpolation('+', potent[GRADYVY][sosedi[BSIDE][iB].iNODE1], potent[GRADYVY][iB], potent[GRADYVY][iP], pbb.z, pb.z, pp.z, pp.z + 0.5*dz);
			potent[GRADZVY][iT] = my_quadratic_interpolation('+', potent[GRADZVY][sosedi[BSIDE][iB].iNODE1], potent[GRADZVY][iB], potent[GRADZVY][iP], pbb.z, pb.z, pp.z, pp.z + 0.5*dz);

			potent[GRADXVZ][iT] = my_quadratic_interpolation('+', potent[GRADXVZ][sosedi[BSIDE][iB].iNODE1], potent[GRADXVZ][iB], potent[GRADXVZ][iP], pbb.z, pb.z, pp.z, pp.z + 0.5*dz);
			potent[GRADYVZ][iT] = my_quadratic_interpolation('+', potent[GRADYVZ][sosedi[BSIDE][iB].iNODE1], potent[GRADYVZ][iB], potent[GRADYVZ][iP], pbb.z, pb.z, pp.z, pp.z + 0.5*dz);
			potent[GRADZVZ][iT] = my_quadratic_interpolation('+', potent[GRADZVZ][sosedi[BSIDE][iB].iNODE1], potent[GRADZVZ][iB], potent[GRADZVZ][iP], pbb.z, pb.z, pp.z, pp.z + 0.5*dz);
		}

		if (bB) {

			// ������������ �������������.

            TOCHKA pp,pb,pbb;
		    center_cord3D(iP, nvtx, pa, pp,100);
		    center_cord3D(iT, nvtx, pa, pb,TSIDE);
			center_cord3D(sosedi[TSIDE][iT].iNODE1, nvtx, pa, pbb, TTSIDE);


			potent[GRADXVX][iB] = my_quadratic_interpolation('-', potent[GRADXVX][sosedi[TSIDE][iT].iNODE1], potent[GRADXVX][iT], potent[GRADXVX][iP], pbb.z, pb.z, pp.z, pp.z - 0.5*dz);
			potent[GRADYVX][iB] = my_quadratic_interpolation('-', potent[GRADYVX][sosedi[TSIDE][iT].iNODE1], potent[GRADYVX][iT], potent[GRADYVX][iP], pbb.z, pb.z, pp.z, pp.z - 0.5*dz);
			potent[GRADZVX][iB] = my_quadratic_interpolation('-', potent[GRADZVX][sosedi[TSIDE][iT].iNODE1], potent[GRADZVX][iT], potent[GRADZVX][iP], pbb.z, pb.z, pp.z, pp.z - 0.5*dz);

			potent[GRADXVY][iB] = my_quadratic_interpolation('-', potent[GRADXVY][sosedi[TSIDE][iT].iNODE1], potent[GRADXVY][iT], potent[GRADXVY][iP], pbb.z, pb.z, pp.z, pp.z - 0.5*dz);
			potent[GRADYVY][iB] = my_quadratic_interpolation('-', potent[GRADYVY][sosedi[TSIDE][iT].iNODE1], potent[GRADYVY][iT], potent[GRADYVY][iP], pbb.z, pb.z, pp.z, pp.z - 0.5*dz);
			potent[GRADZVY][iB] = my_quadratic_interpolation('-', potent[GRADZVY][sosedi[TSIDE][iT].iNODE1], potent[GRADZVY][iT], potent[GRADZVY][iP], pbb.z, pb.z, pp.z, pp.z - 0.5*dz);

			potent[GRADXVZ][iB] = my_quadratic_interpolation('-', potent[GRADXVZ][sosedi[TSIDE][iT].iNODE1], potent[GRADXVZ][iT], potent[GRADXVZ][iP], pbb.z, pb.z, pp.z, pp.z - 0.5*dz);
			potent[GRADYVZ][iB] = my_quadratic_interpolation('-', potent[GRADYVZ][sosedi[TSIDE][iT].iNODE1], potent[GRADYVZ][iT], potent[GRADYVZ][iP], pbb.z, pb.z, pp.z, pp.z - 0.5*dz);
			potent[GRADZVZ][iB] = my_quadratic_interpolation('-', potent[GRADZVZ][sosedi[TSIDE][iT].iNODE1], potent[GRADZVZ][iT], potent[GRADZVZ][iP], pbb.z, pb.z, pp.z, pp.z - 0.5*dz);
		}
	}

} // green_gaussO2

// ���������� ����������� �� �������� ������� ��� ������� ������� ��������.
void green_gauss(integer iP, doublereal** &potent, integer** &nvtx, TOCHKA* pa,
	ALICE_PARTITION** &sosedi, integer maxelm, bool bbond, FLOW &f) {

	// ���� bsecondorder==true �� ����������� ����� ����������� �� ������ �������� ��������.
	bool bsecondorder=false; // ���� false �� ������ ����� ����������� ���������������� �������� ���-���.

	if (bsecondorder) {
		// ������ ������� ��������.
		green_gaussO2(iP, potent, nvtx, pa, sosedi, maxelm, bbond);
	}
	else {
		// ������ ������� ��������.
		green_gaussO1(iP, potent, nvtx, pa, sosedi, maxelm, bbond, f.mf[iP], f.prop[RHO], f.prop_b[RHO]);
	}

} // green_gauss

#endif