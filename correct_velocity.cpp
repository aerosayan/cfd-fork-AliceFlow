// � ����� correct_velocity.cpp ���������� ��� ������� �����������
// ��� ��������� �������� ��� ����� ��� ����� ������������� ��������� 
// �������������.

// ������ ������� �������� � ��������� ���� 31 ����� 2012 ����.

// 
// ��������� �������� ���� ��������� �� ��� ���������.
// ������ ��������� ��� ���������� ��. ��� ���� ������������ ������������� ��� 
// �������� �������� �������� �� ������� ��������� �������. 
// 
// ������ ��������� ��������� ��� ��������� ��. � ������������� �������� ������� ������� �� �������.
// ��������� ��������� �� ����� � ���������, �.�. ��� ����� �� ������������.
// �������� ������ ����� �������� ���� � ������ ���������� ��������� ��������� �����.
// ��� ����� ��������� ��������������� ���� ���������� �������.
// �� ������ ������ ������ �������� ��������� �������� �� ������� ���� ������������� ������� �������.

// ��������� ��������� �� ��������.
// �� ������ �������� ��������� �� ������ mysolver SIMPLE Algorithm.
// � ���� ����� ���������� ��� ���������������� ������� � 
// ������ ����������� �������� � ���������� �������.
// ������������ �������������, ������������� �������� ����������, ������� � ���� ������ 13,14,15 ��� 2012 ����.

#ifndef CORRECT_VELOCITYv_0_07_CPP
#define CORRECT_VELOCITYv_0_07_CPP 1



// ��������� �������� ��� ���������� ��.
// ���������������� �������� ������������� ��������� 
// �������������.
void correct_internal_volume(integer iP, integer iVar, equation3D** sl,   
			 integer** nvtx, doublereal** &potent, integer maxelm, doublereal* alpha,
			 TOCHKA* pa, ALICE_PARTITION** sosedi, integer iternumber) {

    // ������������ ������������� ��������� 14 ��� 2012 ����.
	// ��� ��� ������� ����� ��������� ��� ������������� (��������� ������ �� ��� ������).
	integer interpol=0; // 0 - ��� ������ �������������, 1- �������� �������������, 2- ������������ �������������.

	// �������� ����� ������������ � ������ ������� ����. ������� �� ����� �.�.�������  
	// ��������. ������� �� ����� �.�. ������� ������������ �������� ����������� �� 
	// �������� �������� ������� ������� �� ������������� ����� (��� ������� ��� ����������� 
	// �� �������� ����������� �� ��� ������).
	// ������� �������� ���������� ������ ����������� �� ��������.
	integer iderivative_pressure=1; // 1 - ������ �������, 2 - ������ �������.


    // ���������� ���� � ��� ������:
    // iP ����������� ��������� 0..maxelm-1

    // iP - ����� ������������ ������������ ������
	integer iE=-1, iN=-1, iT=-1, iW=-1, iS=-1, iB=-1; // ������ �������� ����������� �������
	iE=sosedi[ESIDE][iP].iNODE1; iN=sosedi[NSIDE][iP].iNODE1; iT=sosedi[TSIDE][iP].iNODE1;
	iW=sosedi[WSIDE][iP].iNODE1; iS=sosedi[SSIDE][iP].iNODE1; iB=sosedi[BSIDE][iP].iNODE1;
	// ������������� ������� ��� �������� �������� 
	// �������, �.�. ��� ��� ���������������� � 
	// my_elmatr_quad_PAm...
	//sl[PAM][iP].iP=iP;
	//sl[PAM][iP].iE=iE; sl[PAM][iP].iN=iN; 
	//sl[PAM][iP].iS=iS; sl[PAM][iP].iW=iW;
    //sl[PAM][iP].iT=iT; sl[PAM][iP].iB=iB;

    // ���� � ����� �� ������ ������� ��������� ������� 
	// �� ���������� ����� true
	bool bE=false, bN=false, bT=false, bW=false, bS=false, bB=false;

    if (iE>=maxelm) bE=true;
	if (iN>=maxelm) bN=true;
	if (iT>=maxelm) bT=true;
    if (iW>=maxelm) bW=true;
	if (iS>=maxelm) bS=true;
	if (iB>=maxelm) bB=true;

	// ���������� �������� �������� ������������ ������:
	doublereal dx=0.0, dy=0.0, dz=0.0; // ������� ������������ ������
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


	doublereal feplus=0.0, fwplus=0.0, fnplus=0.0, fsplus=0.0, ftplus=0.0, fbplus=0.0;
	// x-direction
	feplus=0.5*dx/dxe; // if(bE) then feplus=1.0;
	fwplus=0.5*dx/dxw;
	// y-direction
	fnplus=0.5*dy/dyn;
	fsplus=0.5*dy/dys;
	// z-direction
	ftplus=0.5*dz/dzt;
	fbplus=0.5*dz/dzb;

	//printf("%e %e %e %e %e %e\n",feplus, fwplus, fnplus, fsplus, ftplus, fbplus);
	//getchar();


	doublereal ds=0.0, dl=0.0, dv=0.0; // ������� �����, ����� ��������� � ����� ������������ ������.
	switch (iVar) {
       case VX : ds=dy*dz; dv=ds*dx; dl=dx; break;
       case VY : ds=dx*dz; dv=ds*dy; dl=dy;  break;
       case VZ : ds=dx*dy; dv=ds*dz; dl=dz;  break;
	}

	
	doublereal PAmP=0.0, PAmW=0.0, PAmE=0.0, PAmS=0.0, PAmN=0.0, PAmB=0.0, PAmT=0.0;
	PAmP=potent[PAM][iP];

	// �������� �������� ��������
	// ��� �� ���������� ��� � � 
	// ��������� �����.
	// �������� �������� � ���� �������� ������ ���� ��������������� �� �������
	// � ������� �������� ��� ������������ ������������ (������������ ������� �����).
	// ���������� ������� ������� �� ������� ��� �������� �������� �� ����������� ����,
	// �.�. � ������������ ���� ��������� ���������� �������� ��������.
	// �������� ����� ������ ��-�������� �� � ����� ��������� ��������, � ����� ����� �������
	// ��������� ��� �������� ��������. ����� �������� ������������ ����� � ����� ����� ��������� �������������.
	/*
	PAmW=potent[PAM][iW];
	PAmE=potent[PAM][iE];
	PAmS=potent[PAM][iS];
	PAmN=potent[PAM][iN];
	PAmB=potent[PAM][iB];
	PAmT=potent[PAM][iT];//*/

	doublereal hxminus=0.0, hxplus=0.0, hyminus=0.0, hyplus=0.0, hzminus=0.0, hzplus=0.0;

	//*
    if (!bW) {
		TOCHKA pp, pb;
		center_cord3D(iP, nvtx, pa, pp,100);
		center_cord3D(iW, nvtx, pa, pb,WSIDE);
		hxminus=fabs(pp.x-pb.x);

		PAmW=potent[PAM][iW];
	} else {
		// ���� W ���������
		TOCHKA pp, pb,pbb;
		center_cord3D(iP, nvtx, pa, pp,100);
		hxminus=0.5*dx;

		switch (interpol) {
		case 0: // �������� ������� �������� ����� ������� ����.
			    PAmW=potent[PAM][iW]; 
			    break;
		case 1 : // �������� �������������
		         center_cord3D(iP, nvtx, pa, pp,100);
		         center_cord3D(iE, nvtx, pa, pb,ESIDE);
		         PAmW=my_linear_interpolation('-', potent[PAM][iP], potent[PAM][iE], pp.x, pb.x, pp.x-0.5*dx); 
			    break;
		case 2 : // ������������ �������������.

		         center_cord3D(iP, nvtx, pa, pp,100);
		         center_cord3D(iE, nvtx, pa, pb,ESIDE);
			     center_cord3D(sosedi[ESIDE][iE].iNODE1, nvtx, pa, pbb,EE);
					
			     PAmW=my_quadratic_interpolation('-', potent[PAM][sosedi[ESIDE][iE].iNODE1], potent[PAM][iE], potent[PAM][iP], pbb.x , pb.x, pp.x, pp.x-0.5*dx); 
			    break;
		default : // �������� ������� �������� ����� ������� ����.
			      PAmW=potent[PAM][iW];
			    break;
		} // end switch
		
	}
	if (!bE) {
		TOCHKA pp, pb;
		center_cord3D(iP, nvtx, pa, pp,100);
		center_cord3D(iE, nvtx, pa, pb,ESIDE);
		hxplus=fabs(pb.x-pp.x);

		PAmE=potent[PAM][iE];
	} else {
        // ���� E ���������
		hxplus=0.5*dx;
		TOCHKA pp,pb,pbb;

		switch (interpol) {
		case 0 : // �������� ������� �������� ����� ������� ����. 
			    PAmE=potent[PAM][iE];
			    break;
		case 1 : // �������� ������������

		         center_cord3D(iP, nvtx, pa, pp,100);
		         center_cord3D(iW, nvtx, pa, pb,WSIDE);
		         PAmE=my_linear_interpolation('+', potent[PAM][iP], potent[PAM][iW], pp.x, pb.x, pp.x+0.5*dx);

			    break;
		case 2 : // ������������ �������������.
						     
		         center_cord3D(iP, nvtx, pa, pp,100);
		         center_cord3D(iW, nvtx, pa, pb,WSIDE);
				 center_cord3D(sosedi[WSIDE][iW].iNODE1, nvtx, pa, pbb,WW);
					
				 PAmE = my_quadratic_interpolation('+', potent[PAM][sosedi[WSIDE][iW].iNODE1], potent[PAM][iW], potent[PAM][iP], pbb.x, pb.x, pp.x, pp.x + 0.5*dx);
			    break;
		default : // �������� ������� �������� ����� ������� ����.
			     PAmE=potent[PAM][iE]; 
			     break;
		}

	}
	if (!bS) {
		TOCHKA pp, pb;
		center_cord3D(iP, nvtx, pa, pp,100);
		center_cord3D(iS, nvtx, pa, pb,SSIDE);
		hyminus=fabs(pp.y-pb.y);

		PAmS=potent[PAM][iS];
	} else {
		// ���� S ���������
		hyminus=0.5*dy;
		TOCHKA pp,pb,pbb;

		switch (interpol) {
		case 0 : // �������� ������� �������� ����� ������� ����.  
			    PAmS=potent[PAM][iS];
			    break;
		case 1 :
			    // �������� ������������

		        center_cord3D(iP, nvtx, pa, pp,100);
		        center_cord3D(iN, nvtx, pa, pb,NSIDE);
		        PAmS=my_linear_interpolation('-', potent[PAM][iP], potent[PAM][iN], pp.y, pb.y, pp.y-0.5*dy);

			    break;
		case 2 :  
			    // ������������ �������������.

		        center_cord3D(iP, nvtx, pa, pp,100);
		        center_cord3D(iN, nvtx, pa, pb,NSIDE);
				center_cord3D(sosedi[NSIDE][iN].iNODE1, nvtx, pa, pbb,NN);
					
				PAmS = my_quadratic_interpolation('-', potent[PAM][sosedi[NSIDE][iN].iNODE1], potent[PAM][iN], potent[PAM][iP], pbb.y, pb.y, pp.y, pp.y - 0.5*dy);

			    break;
		default : // �������� ������� �������� ����� ������� ����. 
			    PAmS=potent[PAM][iS];
			    break;
		}

	}
	if (!bN) {
		TOCHKA pp, pb;
		center_cord3D(iP, nvtx, pa, pp,100);
		center_cord3D(iN, nvtx, pa, pb,NSIDE);
		hyplus=fabs(pb.y-pp.y);

		PAmN=potent[PAM][iN];
	} else {
		// ���� N ���������
		hyplus=0.5*dy;
		TOCHKA pp,pb,pbb;

		switch (interpol) {
		case 0 : // �������� ������� �������� ����� ������� ����. 
			    PAmN=potent[PAM][iN];
			    break;
		case 1 :
			    // �������� ������������

		        center_cord3D(iP, nvtx, pa, pp,100);
		        center_cord3D(iS, nvtx, pa, pb,SSIDE);
		        PAmN=my_linear_interpolation('+', potent[PAM][iP], potent[PAM][iS], pp.y, pb.y, pp.y+0.5*dy);
			    break;
		case 2 :
			    // ������������ �������������.

		        center_cord3D(iP, nvtx, pa, pp,100);
		        center_cord3D(iS, nvtx, pa, pb,SSIDE);
				center_cord3D(sosedi[SSIDE][iS].iNODE1, nvtx, pa, pbb,SS);
					
				PAmN = my_quadratic_interpolation('+', potent[PAM][sosedi[SSIDE][iS].iNODE1], potent[PAM][iS], potent[PAM][iP], pbb.y, pb.y, pp.y, pp.y + 0.5*dy);
			    break;
		default : 
			    // �������� ������� �������� ����� ������� ����. 
			    PAmN=potent[PAM][iN];
			    break;
		}

		 
	} 

	if (!bB) {
		TOCHKA pp, pb;
		center_cord3D(iP, nvtx, pa, pp,100);
		center_cord3D(iB, nvtx, pa, pb,BSIDE);
		hzminus=fabs(pp.z-pb.z);

		PAmB=potent[PAM][iB];
	} else {
		// ���� B ���������
		hzminus=0.5*dz;
		TOCHKA pp,pb,pbb;

		switch (interpol) {
		case 0 :
			    // �������� ������� �������� ����� ������� ����. 
			    PAmB=potent[PAM][iB];
			    break;
		case 1 : 
			    // �������� ������������

		        center_cord3D(iP, nvtx, pa, pp,100);
		        center_cord3D(iT, nvtx, pa, pb,TSIDE);
		        PAmB=my_linear_interpolation('-', potent[PAM][iP], potent[PAM][iT], pp.z, pb.z, pp.z-0.5*dz);
			    break;
		case 2 : // ������������ �������������.

		        center_cord3D(iP, nvtx, pa, pp,100);
		        center_cord3D(iT, nvtx, pa, pb,TSIDE);
				center_cord3D(sosedi[TSIDE][iT].iNODE1, nvtx, pa, pbb,TTSIDE);
					
				PAmB = my_quadratic_interpolation('-', potent[PAM][sosedi[TSIDE][iT].iNODE1], potent[PAM][iT], potent[PAM][iP], pbb.z, pb.z, pp.z, pp.z - 0.5*dz);
			    break;
		default :
			    // �������� ������� �������� ����� ������� ����. 
			    PAmB=potent[PAM][iB];
			    break;
		}

	}
	if (!bT) { 
		TOCHKA pp, pb;
		center_cord3D(iP, nvtx, pa, pp,100);
		center_cord3D(iT, nvtx, pa, pb,TSIDE);
		hzplus=fabs(pb.z-pp.z);

		PAmT=potent[PAM][iT];
	} else {
		// ���� T ���������
		hzplus=0.5*dz;
		TOCHKA pp,pb,pbb;

		switch (interpol) {
		case 0 : // �������� ������� �������� ����� ������� ����. 
			    PAmT=potent[PAM][iT];
			    break;
		case 1 : 
			    // �������� ������������

		        center_cord3D(iP, nvtx, pa, pp,100);
		        center_cord3D(iB, nvtx, pa, pb,BSIDE);
		        PAmT=my_linear_interpolation('+', potent[PAM][iP], potent[PAM][iB], pp.z, pb.z, pp.z+0.5*dz);
			    break;
		case 2 :// ������������ �������������.
                
		        center_cord3D(iP, nvtx, pa, pp,100);
		        center_cord3D(iB, nvtx, pa, pb,BSIDE);
				center_cord3D(sosedi[BSIDE][iB].iNODE1, nvtx, pa, pbb,BB);
					
				PAmT = my_quadratic_interpolation('+', potent[PAM][sosedi[BSIDE][iB].iNODE1], potent[PAM][iB], potent[PAM][iP], pbb.z, pb.z, pp.z, pp.z + 0.5*dz);
			    break;
		default : // �������� ������� �������� ����� ������� ����. 
			     PAmT=potent[PAM][iT];
			    break;
		} 

	}
	//*/

	// ������ ���������� ���� G ����������� ���������,
	// �.�. � ���� ������ fgplus==1.0; 

	// �������� ������������ �������� �� ����� ��.
	doublereal deltaP=0.0, gradP=0.0;
	switch (iVar) {
		case VX : if (iderivative_pressure==1) {
			          // ������������ ������������� ������� �������.
				      deltaP=(fwplus*PAmW+(1-fwplus)*PAmP);
			          deltaP-=(feplus*PAmE+(1-feplus)*PAmP); 
				      gradP=deltaP/dl; // ������ ����������� �� ��������.
				  }
				  else if (iderivative_pressure==2) {
					  // ������������� ������� ������� ��������.
					  // ����������� ����� ���� ����� ����� ��������
					  // ����� ���������� � ������ �������.
					  gradP=-rgradF(PAmW, PAmP, PAmE, hxminus, hxplus);
				  }
				  break;
		case VY : if (iderivative_pressure==1) {
			          // ������������ ������������� ������� �������.
				      deltaP=(fsplus*PAmS+(1-fsplus)*PAmP);
			          deltaP-=(fnplus*PAmN+(1-fnplus)*PAmP);
				      gradP=deltaP/dl; // ������ ����������� �� ��������.
				  }
				  else if (iderivative_pressure==2) {
					  // ������������� ������� ������� ��������.
					  // ����������� ����� ���� ����� ����� ��������
					  // ����� ���������� � ������ �������.
					  gradP=-rgradF(PAmS, PAmP, PAmN, hyminus, hyplus);
				  }
			      break;
        case VZ : if (iderivative_pressure==1) {
			          // ������������ ������������� ������� �������.
			          deltaP=(fbplus*PAmB+(1-fbplus)*PAmP);
			          deltaP-=(ftplus*PAmT+(1-ftplus)*PAmP);
				      gradP=deltaP/dl; // ������ ����������� �� ��������.
				  }
				  else if (iderivative_pressure==2) {
					  // ������������� ������� ������� ��������.
					  // ����������� ����� ���� ����� ����� ��������
					  // ����� ���������� � ������ �������.
				      gradP=-rgradF(PAmB, PAmP, PAmT, hzminus, hzplus);
				      //printf("%e\n",PAmB); getchar();
				  }
				  break;
	}

    // ��������� ���������� �������� �� ���������� ���� iP. 

	// ��������� �������� �� ������ ������������ ������ ����������.
	// ��� ���������� ������ �. ��������. ��� ������ ���� ����������� � ������������ ���������
	// ��� �������� ��������.
	//potent[iVar][iP]+=ds*(deltaP)/sl[iVar][iP].ap;//alpha[iVar]*
	// ��� ���������� ������ �������� ������ � ��������� ��������� Sigma-flow.
	// ��� ������ ���� ����������� � ������������ ��������� ��� �������� ��������.
	// potent[iVar][iP]+=(tau/rho)*deltaP;
	if (iSIMPLE_alg==SIMPLE_Carretto) {
		// SIMPLE ��������: Carretto et al., 1973.
		// tau ~ (alpha[iVar]*rho*ds)/(sl[iVar][iP].ap);
		//potent[iVar][iP]+=alpha[iVar]*ds*(deltaP)/sl[iVar][iP].ap;// ��. ������ ��������� ������.
		potent[iVar][iP]+=alpha[iVar]*dv*(gradP)/sl[iVar][iP].ap;// ��. ������ ��������� ������.
	}
	if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) {
		// SIMPLEC ��������: Van Doormal and Raithby., 1984
		// tau ~ (alpha[iVar]*rho*ds)/((1.0-alpha[iVar])*sl[iVar][iP].ap);
        //potent[iVar][iP]+=alpha[iVar]*ds*(deltaP)/((1.0-alpha[iVar])*sl[iVar][iP].ap);// ��. ������ ��������� ������.
		potent[iVar][iP]+=alpha[iVar]*dv*(gradP)/((1.0-alpha[iVar])*sl[iVar][iP].ap);
	}

	// ��������� �������� � ��������� ����� ������ ���� ������� ���������
	// ���������� ��������� �������. � ������ 1. ���� � ��������� ����
	// �� �������� ����� ������� ������� �� ������� ��������� �������� 
	// � ��������� ���� �� ���������. 2. ���� � ��������� ���� �� ��������
	// ����� ���������� ������� ������� �� ���������� ������ ���������������� 
	// �������� �� ���������� ����������� ���� � ���������.
	
    
} // correct_internal_volume

// ��������� �������� ��� ���������� ��.
// ���������������� �������� ������������� ��������� 
// �������������.
void correct_internal_volume2(integer iP, integer iVar, equation3D** sl,   
			 integer** nvtx, doublereal** &potent, integer maxelm, doublereal* alpha,
			 TOCHKA* pa, ALICE_PARTITION** sosedi, integer iternumber) {

	// ��������� ��������� �������� �� ���������� ����������� ������� �� 
	// ������ ����������� ��������� ��������.
    
    // ���������� ���� � ��� ������:
    // iP ����������� ��������� 0..maxelm-1

	
	// ���������� �������� �������� ������������ ������:
	doublereal dx=0.0, dy=0.0, dz=0.0; // ������� ������������ ������
    volume3D(iP, nvtx, pa, dx, dy, dz);
	    

	doublereal ds=0.0, dv=0.0; // ������� �����, ����� ��������� � ����� ������������ ������.
	switch (iVar) {
       case VX : ds=dy*dz; dv=ds*dx; break;
       case VY : ds=dx*dz; dv=ds*dy; break;
       case VZ : ds=dx*dy; dv=ds*dz; break;
	}
	
	// ������ ���������� ���� G ����������� ���������,
	// �.�. � ���� ������ fgplus==1.0; 

	// �������� ������������ �������� �� ����� ��.
	doublereal deltaP=0.0, gradP=0.0;
	switch (iVar) {
		case VX : gradP=-potent[GRADXPAM][iP];
				  break;
		case VY : gradP=-potent[GRADYPAM][iP];
				  break;
        case VZ : gradP=-potent[GRADZPAM][iP];
				  break;
	}


    // ��������� ���������� �������� �� ���������� ���� iP. 

	// ��������� �������� �� ������ ������������ ������ ����������.
	// ��� ���������� ������ �. ��������. ��� ������ ���� ����������� � ������������ ���������
	// ��� �������� ��������.
	//potent[iVar][iP]+=ds*(deltaP)/sl[iVar][iP].ap;//alpha[iVar]*
	// ��� ���������� ������ �������� ������ � ��������� ��������� Sigma-flow.
	// ��� ������ ���� ����������� � ������������ ��������� ��� �������� ��������.
	// potent[iVar][iP]+=(tau/rho)*deltaP;
	if (iSIMPLE_alg==SIMPLE_Carretto) {
		// SIMPLE ��������: Carretto et al., 1973.
		// tau ~ (alpha[iVar]*rho*ds)/(sl[iVar][iP].ap);
		//potent[iVar][iP]+=alpha[iVar]*ds*(deltaP)/sl[iVar][iP].ap;// ��. ������ ��������� ������.
		potent[iVar][iP]+=alpha[iVar]*dv*(gradP)/sl[iVar][iP].ap;// ��. ������ ��������� ������.
	}
	if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) {
		// SIMPLEC ��������: Van Doormal and Raithby., 1984
		// tau ~ (alpha[iVar]*rho*ds)/((1.0-alpha[iVar])*sl[iVar][iP].ap);
        //potent[iVar][iP]+=alpha[iVar]*ds*(deltaP)/((1.0-alpha[iVar])*sl[iVar][iP].ap);// ��. ������ ��������� ������.
		potent[iVar][iP]+=alpha[iVar]*dv*(gradP)/((1.0-alpha[iVar])*sl[iVar][iP].ap);
	}

	// ��������� �������� � ��������� ����� ������ ���� ������� ���������
	// ���������� ��������� �������. � ������ 1. ���� � ��������� ����
	// �� �������� ����� ������� ������� �� ������� ��������� �������� 
	// � ��������� ���� �� ���������. 2. ���� � ��������� ���� �� ��������
	// ����� ���������� ������� ������� �� ���������� ������ ���������������� 
	// �������� �� ���������� ����������� ���� � ���������.
	
    
} // correct_internal_volume2

// ��������� �������� ��� ���������� ��.
// ���������������� �������� ������������� ��������� 
// ������������� (�������������).
// begin 20 ���� 2012 ����.
void correct_internal_volume3(integer iP, integer iVar, doublereal** prop,   
			  doublereal** &potent,  doublereal* tau) {

	// ��������� ��������� �������� �� ���������� ����������� ������� �� 
	// ������ �������������� ������������ �� ������� �����-������ ��������� �������� ��������.
    
    // ���������� ���� 
    // iP ����������� ��������� 0..maxelm-1

	
	// ����������� ��������� �� �������� ��������
	// � ����������� �� ��������� ���������� �������� ���
	// ������������� �����������.
	doublereal gradP=0.0;
	switch (iVar) {
		case VX : gradP=-potent[GRADXPAM][iP];
				  break;
		case VY : gradP=-potent[GRADYPAM][iP];
				  break;
        case VZ : gradP=-potent[GRADZPAM][iP];
				  break;
	}


    // ��������� ���������� �������� �� ���������� ���� iP. 

	// ��������� �������� �� ������ ������������ ������ ����������.
	// ��� ���������� ������ �. ��������. ��� ������ ���� ����������� � ������������ ���������
	// ��� �������� ��������.
	//potent[iVar][iP]+=ds*(deltaP)/sl[iVar][iP].ap;//alpha[iVar]*
	// ��� ���������� ������ �������� ������ � ��������� ��������� Sigma-flow.
	// ��� ������ ���� ����������� � ������������ ��������� ��� �������� ��������.
	// potent[iVar][iP]+=(tau/rho)*deltaP;
	/*
	if (iSIMPLE_alg==SIMPLE_Carretto) {
		// SIMPLE ��������: Carretto et al., 1973.
		// tau ~ (alpha[iVar]*rho*ds)/(sl[iVar][iP].ap);
		//potent[iVar][iP]+=alpha[iVar]*ds*(deltaP)/sl[iVar][iP].ap;// ��. ������ ��������� ������.
		potent[iVar][iP]+=alpha[iVar]*dv*(gradP)/sl[iVar][iP].ap;// ��. ������ ��������� ������.
	}
	if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) {
		// SIMPLEC ��������: Van Doormal and Raithby., 1984
		// tau ~ (alpha[iVar]*rho*ds)/((1.0-alpha[iVar])*sl[iVar][iP].ap);
        //potent[iVar][iP]+=alpha[iVar]*ds*(deltaP)/((1.0-alpha[iVar])*sl[iVar][iP].ap);// ��. ������ ��������� ������.
		potent[iVar][iP]+=alpha[iVar]*dv*(gradP)/((1.0-alpha[iVar])*sl[iVar][iP].ap);
	}
	*/

	// �������������� ��� ��� �� ������������� tau �������������� �������,
	// ����������� �������������� ������� ������� ��� ��� �� ������� � ������������ ������
	// ������ ���� �������� ��������� � �������� ����� ����������. ��. �������� ������.
	// ���� tau �� ���������� �� �������� � ������ �������� ������������ ���������������
	// �������� ��� ������ ����������.
	// � ������� ��� tau ��� ����������� ��� �������� �������� ��������� : SIMPLE � SIMPLEC.
	potent[iVar][iP]+=tau[iP]*gradP/prop[RHO][iP];

	// ��������� �������� � ��������� ����� ������ ���� ������� ���������
	// ���������� ��������� �������. � ������ 1. ���� � ��������� ����
	// �� �������� ����� ������� ������� �� ������� ��������� �������� 
	// � ��������� ���� �� ���������. 2. ���� � ��������� ���� �� ��������
	// ����� ���������� ������� ������� �� ���������� ������ ���������������� 
	// �������� �� ���������� ����������� ���� � ���������.
	
    
} // correct_internal_volume3

// ��������� �������� ��� ���������� ��.
// ���������������� �������� ������������� ��������� 
// ������������� (�������������).
// ����������� 23 ���� 2012 ����.
// �� ������ ����������� �������������, � ������
// ��� ��������� ����� �������������
void correct_internal_volume4(integer iP, integer iVar, doublereal** prop,   
			  doublereal** &potent,  doublereal** tau) {

	// ��������� ��������� �������� �� ���������� ����������� ������� �� 
	// ������ �������������� ������������ �� ������� �����-������ ��������� �������� ��������.
    
    // ���������� ���� 
    // iP ����������� ��������� 0..maxelm-1

	
	// ����������� ��������� �� �������� ��������
	// � ����������� �� ��������� ���������� �������� ���
	// ������������� �����������.
	doublereal gradPAM=0.0, tauP=0.0;
	switch (iVar) {
		// �� ��������� ����� ���������� ������ ���� �����.
		case VX : gradPAM=-potent[GRADXPAM][iP];
			      tauP=tau[VX][iP];
				  break;
		case VY : gradPAM=-potent[GRADYPAM][iP];
			      tauP=tau[VY][iP];
				  break;
        case VZ : gradPAM=-potent[GRADZPAM][iP];
			      tauP=tau[VZ][iP];
				  break;
	}


    // ��������� ���������� �������� �� ���������� ���� iP. 

	// ��������� �������� �� ������ ������������ ������ ����������.
	// ��� ���������� ������ �. ��������. ��� ������ ���� ����������� � ������������ ���������
	// ��� �������� ��������.
	//potent[iVar][iP]+=ds*(deltaP)/sl[iVar][iP].ap;//alpha[iVar]*
	// ��� ���������� ������ �������� ������ � ��������� ��������� Sigma-flow.
	// ��� ������ ���� ����������� � ������������ ��������� ��� �������� ��������.
	// potent[iVar][iP]+=(tau/rho)*deltaP;
	/*
	if (iSIMPLE_alg==SIMPLE_Carretto) {
		// SIMPLE ��������: Carretto et al., 1973.
		// tau ~ (alpha[iVar]*rho*ds)/(sl[iVar][iP].ap);
		//potent[iVar][iP]+=alpha[iVar]*ds*(deltaP)/sl[iVar][iP].ap;// ��. ������ ��������� ������.
		potent[iVar][iP]+=alpha[iVar]*dv*(gradP)/sl[iVar][iP].ap;// ��. ������ ��������� ������.
	}
	if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) {
		// SIMPLEC ��������: Van Doormal and Raithby., 1984
		// tau ~ (alpha[iVar]*rho*ds)/((1.0-alpha[iVar])*sl[iVar][iP].ap);
        //potent[iVar][iP]+=alpha[iVar]*ds*(deltaP)/((1.0-alpha[iVar])*sl[iVar][iP].ap);// ��. ������ ��������� ������.
		potent[iVar][iP]+=alpha[iVar]*dv*(gradP)/((1.0-alpha[iVar])*sl[iVar][iP].ap);
	}
	*/

	// �������������� ��� ��� �� ������������� tau �������������� �������,
	// ����������� �������������� ������� ������� ��� ��� �� ������� � ������������ ������
	// ������ ���� �������� ��������� � �������� ����� ����������. ��. �������� ������.
	// ���� tau �� ���������� �� �������� � ������ �������� ������������ ���������������
	// �������� ��� ������ ����������.
	// � ������� ��� tau ��� ����������� ��� �������� �������� ��������� : SIMPLE � SIMPLEC.
	potent[iVar][iP]+=tauP*gradPAM/prop[RHO][iP];

	// ��������� �������� � ��������� ����� ������ ���� ������� ���������
	// ���������� ��������� �������. � ������ 1. ���� � ��������� ����
	// �� �������� ����� ������� ������� �� ������� ��������� �������� 
	// � ��������� ���� �� ���������. 2. ���� � ��������� ���� �� ��������
	// ����� ���������� ������� ������� �� ���������� ������ ���������������� 
	// �������� �� ���������� ����������� ���� � ���������.
	
    
} // correct_internal_volume4

// ��������� ��������� ������ �� ����� ��.
// begin 25 ���� 2012 ����.
void correct_mf(doublereal** &mfcurrentretune, doublereal** potent,  doublereal** tau,
	TOCHKA* pa, ALICE_PARTITION** sosedi, integer** nvtx, integer maxelm,
				BOUND* &sosedb, integer ls, integer lw, WALL* w, doublereal** prop_b) {

					

	doublereal** mfloc = NULL;
	mfloc=new doublereal*[maxelm];
	for (integer i=0; i<maxelm; i++) {
		mfloc[i]=new doublereal[6];
	}

	

	for (integer iP=0; iP<maxelm; iP++) {

		// iP - ����� ������������ ������������ ������
	    integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	    iE=sosedi[ESIDE][iP].iNODE1; iN=sosedi[NSIDE][iP].iNODE1; iT=sosedi[TSIDE][iP].iNODE1;
	    iW=sosedi[WSIDE][iP].iNODE1; iS=sosedi[SSIDE][iP].iNODE1; iB=sosedi[BSIDE][iP].iNODE1;
	    

        // ���� � ����� �� ������ ������� ��������� ������� 
	    // �� ���������� ����� true
	    bool bE=false, bN=false, bT=false, bW=false, bS=false, bB=false;

        if (iE>=maxelm) bE=true;
	    if (iN>=maxelm) bN=true;
	    if (iT>=maxelm) bT=true;
        if (iW>=maxelm) bW=true;
	    if (iS>=maxelm) bS=true;
	    if (iB>=maxelm) bB=true;

	    // ���������� �������� �������� ������������ ������:
	    doublereal dx=0.0, dy=0.0, dz=0.0; // ������� ������������ ������
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


	    doublereal feplus, fwplus, fnplus, fsplus, ftplus, fbplus;
	    // x-direction
	    feplus=0.5*dx/dxe; // if(bE) then feplus=1.0;
	    fwplus=0.5*dx/dxw;
	    // y-direction
	    fnplus=0.5*dy/dyn;
	    fsplus=0.5*dy/dys;
	    // z-direction
	    ftplus=0.5*dz/dzt;
	    fbplus=0.5*dz/dzb;

	    //printf("%e %e %e %e %e %e\n",feplus, fwplus, fnplus, fsplus, ftplus, fbplus);
	    //getchar();

		// �������� ������������� �� ����� ������������ ������.
		doublereal taue, tauw, taun, taus, taut, taub;
        // ������������ ������������� ������� ���, ����� ����������� 
	    // ���������� �����������.
	    if (!bE) taue=tau[VX][iE]*tau[VX][iP]/(feplus*tau[VX][iE]+(1.0-feplus)*tau[VX][iP]); else taue=tau[VX][iE];
	    if (!bW) tauw=tau[VX][iW]*tau[VX][iP]/(fwplus*tau[VX][iW]+(1.0-fwplus)*tau[VX][iP]); else tauw=tau[VX][iW];
	    if (!bN) taun=tau[VY][iN]*tau[VY][iP]/(fnplus*tau[VY][iN]+(1.0-fnplus)*tau[VY][iP]); else taun=tau[VY][iN];
	    if (!bS) taus=tau[VY][iS]*tau[VY][iP]/(fsplus*tau[VY][iS]+(1.0-fsplus)*tau[VY][iP]); else taus=tau[VY][iS];
        if (!bT) taut=tau[VZ][iT]*tau[VZ][iP]/(ftplus*tau[VZ][iT]+(1.0-ftplus)*tau[VZ][iP]); else taut=tau[VZ][iT];
	    if (!bB) taub=tau[VZ][iB]*tau[VZ][iP]/(fbplus*tau[VZ][iB]+(1.0-fbplus)*tau[VZ][iP]); else taub=tau[VZ][iB];

		// �������� �������� �������� �� ����� ������������ ������.
		doublereal gradpame, gradpamw, gradpamn, gradpams, gradpamt, gradpamb;
		if (!bE) gradpame=feplus*potent[GRADXPAM][iE]+(1.0-feplus)*potent[GRADXPAM][iP]; else gradpame=potent[GRADXPAM][iE];
        if (!bW) gradpamw=fwplus*potent[GRADXPAM][iW]+(1.0-fwplus)*potent[GRADXPAM][iP]; else gradpamw=potent[GRADXPAM][iW];
	    if (!bN) gradpamn=fnplus*potent[GRADYPAM][iN]+(1.0-fnplus)*potent[GRADYPAM][iP]; else gradpamn=potent[GRADYPAM][iN];
        if (!bS) gradpams=fsplus*potent[GRADYPAM][iS]+(1.0-fsplus)*potent[GRADYPAM][iP]; else gradpams=potent[GRADYPAM][iS];
        if (!bT) gradpamt=ftplus*potent[GRADZPAM][iT]+(1.0-ftplus)*potent[GRADZPAM][iP]; else gradpamt=potent[GRADZPAM][iT];
        if (!bB) gradpamb=fbplus*potent[GRADZPAM][iB]+(1.0-fbplus)*potent[GRADZPAM][iP]; else gradpamb=potent[GRADZPAM][iB];

		// ������� ���������� ������������������ ��������� 
		// ������ �� ����� ��.
		mfloc[iP][ESIDE]=mfcurrentretune[iP][ESIDE]-taue*gradpame*dy*dz;
		mfloc[iP][WSIDE]=mfcurrentretune[iP][WSIDE]-tauw*gradpamw*dy*dz;
		mfloc[iP][NSIDE]=mfcurrentretune[iP][NSIDE]-taun*gradpamn*dx*dz;
		mfloc[iP][SSIDE]=mfcurrentretune[iP][SSIDE]-taus*gradpams*dx*dz;
		mfloc[iP][TSIDE]=mfcurrentretune[iP][TSIDE]-taut*gradpamt*dx*dy;
		mfloc[iP][BSIDE]=mfcurrentretune[iP][BSIDE]-taub*gradpamb*dx*dy;

	}

	// ������ ���� ������� ��� �������� ����� ����� �������������,
	// �������� ��� ���� ���-�� ������.
	for (integer iP=0; iP<maxelm; iP++) {
		// iP - ����� ������������ ������������ ������
	    integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	    iE=sosedi[ESIDE][iP].iNODE1; iN=sosedi[NSIDE][iP].iNODE1; iT=sosedi[TSIDE][iP].iNODE1;
	    iW=sosedi[WSIDE][iP].iNODE1; iS=sosedi[SSIDE][iP].iNODE1; iB=sosedi[BSIDE][iP].iNODE1;
	    

        // ���� � ����� �� ������ ������� ��������� ������� 
	    // �� ���������� ����� true
	    bool bE=false, bN=false, bT=false, bW=false, bS=false, bB=false;

        if (iE>=maxelm) bE=true;
	    if (iN>=maxelm) bN=true;
	    if (iT>=maxelm) bT=true;
        if (iW>=maxelm) bW=true;
	    if (iS>=maxelm) bS=true;
	    if (iB>=maxelm) bB=true;

		// ���������� �������� �������� ������������ ������:
	    doublereal dx=0.0, dy=0.0, dz=0.0; // ������� ������������ ������
        volume3D(iP, nvtx, pa, dx, dy, dz);
		const doublereal relax_bound = 1.0;

		if (bE) {
			integer inumber=iE-maxelm;
			if ((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && (w[sosedb[inumber].MCB - ls].bpressure || w[sosedb[inumber].MCB - ls].bopening)) {
				// �������� ������� ��������� �� ��� ����
				mfloc[iP][ESIDE] = relax_bound*(mfloc[iP][ESIDE]) + (1.0 - relax_bound)*mfcurrentretune[iP][ESIDE];
			}
			else if ((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bsymmetry) {
				mfloc[iP][ESIDE]=0.0;
			} else if ((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw))) {
				// �������� �������� �� ������� �������.
				mfloc[iP][ESIDE]=prop_b[RHO][inumber]*w[sosedb[inumber].MCB-ls].Vx*dy*dz; // �������� �������� �����.
			}
			else {
				// ������ ����������� ������ �� ���������
				mfloc[iP][ESIDE]=0.0;
			}
		}

		if (bW) {
			integer inumber=iW-maxelm;
			if ((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && (w[sosedb[inumber].MCB - ls].bpressure ||  w[sosedb[inumber].MCB - ls].bopening)) {
				// �������� ������� ��������� �� ��� ����
				mfloc[iP][WSIDE] = relax_bound*(mfloc[iP][WSIDE]) + (1.0 - relax_bound)*mfcurrentretune[iP][WSIDE];
			}
			else if ((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bsymmetry) {
				mfloc[iP][WSIDE]=0.0;
			} else if ((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw))) {
				// �������� �������� �� ������� �������.
				mfloc[iP][WSIDE]=prop_b[RHO][inumber]*w[sosedb[inumber].MCB-ls].Vx*dy*dz; // �������� �������� �����.
			}
			else {
				// ������ ����������� ������ �� ���������
				mfloc[iP][WSIDE]=0.0;
			}
		}

		if (bN) {
			integer inumber=iN-maxelm;
			if ((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && (w[sosedb[inumber].MCB - ls].bpressure ||  w[sosedb[inumber].MCB - ls].bopening)) {
				// �������� ������� ��������� �� ��� ����
				mfloc[iP][NSIDE] = relax_bound*(mfloc[iP][NSIDE]) + (1.0 - relax_bound)*mfcurrentretune[iP][NSIDE];
			}
			else if ((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bsymmetry) {
				mfloc[iP][NSIDE]=0.0;
			} else if ((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw))) {
				// �������� �������� �� ������� �������.
				mfloc[iP][NSIDE]=prop_b[RHO][inumber]*w[sosedb[inumber].MCB-ls].Vy*dx*dz; // �������� �������� �����.
			}
			else {
				// ������ ����������� ������ �� ���������
				mfloc[iP][NSIDE]=0.0;
			}
		}

		if (bS) {
			integer inumber=iS-maxelm;
			if ((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && (w[sosedb[inumber].MCB - ls].bpressure ||  w[sosedb[inumber].MCB - ls].bopening)) {
				// �������� ������� ��������� �� ��� ����
				mfloc[iP][SSIDE] = relax_bound*(mfloc[iP][SSIDE]) + (1.0 - relax_bound)*mfcurrentretune[iP][SSIDE];
			}
			else if ((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bsymmetry) {
				mfloc[iP][SSIDE]=0.0;
			} else if ((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw))) {
				// �������� �������� �� ������� �������.
				mfloc[iP][SSIDE]=prop_b[RHO][inumber]*w[sosedb[inumber].MCB-ls].Vy*dx*dz; // �������� �������� �����.
			}
			else {
				// ������ ����������� ������ �� ���������
				mfloc[iP][SSIDE]=0.0;
			}
		}

		if (bT) {
			integer inumber=iT-maxelm;
			if ((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && (w[sosedb[inumber].MCB - ls].bpressure ||  w[sosedb[inumber].MCB - ls].bopening)) {
				// �������� ������� ��������� �� ��� ����
				mfloc[iP][TSIDE] = relax_bound*(mfloc[iP][TSIDE]) + (1.0 - relax_bound)*mfcurrentretune[iP][TSIDE];
			}
			else if ((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bsymmetry) {
				mfloc[iP][TSIDE]=0.0;
			} else if ((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw))) {
				// �������� �������� �� ������� �������.
				mfloc[iP][TSIDE]=prop_b[RHO][inumber]*w[sosedb[inumber].MCB-ls].Vz*dx*dy; // �������� �������� �����.
			}
			else {
				// ������ ����������� ������ �� ���������
				mfloc[iP][TSIDE]=0.0;
			}
		}

		if (bB) {
			integer inumber=iB-maxelm;
			if ((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && (w[sosedb[inumber].MCB - ls].bpressure ||  w[sosedb[inumber].MCB - ls].bopening)) {
				// �������� ������� ��������� �� ��� ����
				mfloc[iP][BSIDE] = relax_bound*(mfloc[iP][BSIDE]) + (1.0 - relax_bound)*mfcurrentretune[iP][BSIDE];
			}
			else if ((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bsymmetry) {
				mfloc[iP][BSIDE]=0.0;
			} else if ((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw))) {
				// �������� �������� �� ������� �������.
				mfloc[iP][BSIDE]=prop_b[RHO][inumber]*w[sosedb[inumber].MCB-ls].Vz*dx*dy; // �������� �������� �����.
			}
			else {
				// ������ ����������� ������ �� ���������
				mfloc[iP][BSIDE]=0.0;
			}
		}

	}

	// �������� �����������.
	for (integer iG=0; iG<6; iG++) {
		for (integer iP=0; iP<maxelm; iP++) {
			mfcurrentretune[iP][iG]=mfloc[iP][iG];
		}
	}

	// ������������ ������.
	
	if (mfloc != NULL) {
		for (integer i = 0; i < maxelm; i++) {
			if (mfloc[i] != NULL) {
				delete[] mfloc[i];
			}
		}
		delete[] mfloc;
	}

} // correct_mf

void iscorrectmf(doublereal** &mf,
							 integer maxelm, 
							 ALICE_PARTITION** sosedi, BOUND* &sosedb,
							 integer ls, integer lw, WALL* w) {
    integer iP=0;
	integer inumber;
    // iP - ����� ������������ ������������ ������
	for (iP=0; iP<maxelm; iP++) {
		integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	    iE=sosedi[ESIDE][iP].iNODE1; iN=sosedi[NSIDE][iP].iNODE1; iT=sosedi[TSIDE][iP].iNODE1;
	    iW=sosedi[WSIDE][iP].iNODE1; iS=sosedi[SSIDE][iP].iNODE1; iB=sosedi[BSIDE][iP].iNODE1;

		if (iE>=maxelm) {
			// ��������� ����
			inumber=iE-maxelm;
			if (sosedb[inumber].MCB==(ls+lw)) {
				if (fabs(mf[iP][ESIDE])>admission) {
#if doubleintprecision == 1
					printf("wall mf flux velocity non zero iE=%lld", iE);
#else
					printf("wall mf flux velocity non zero iE=%d", iE);
#endif

					
					//getchar();
					system("pause");
				}
				
			}
		}
		if (iW>=maxelm) {
			// ��������� ����
			inumber=iW-maxelm;
			if (sosedb[inumber].MCB==(ls+lw)) {
				if (fabs(mf[iP][WSIDE])>admission) {
#if doubleintprecision == 1
					printf("wall mf flux velocity non zero iW=%lld", iW);
#else
					printf("wall mf flux velocity non zero iW=%d", iW);
#endif
					
					//getchar();
					system("pause");
				}
				
			}
		}

		if (iN>=maxelm) {
			// ��������� ����
			inumber=iN-maxelm;
			if (sosedb[inumber].MCB==(ls+lw)) {
				if (fabs(mf[iP][NSIDE])>admission) {
#if doubleintprecision == 1
					printf("wall mf flux velocity non zero iN=%lld", iN);
#else
					printf("wall mf flux velocity non zero iN=%d", iN);
#endif
					
					//getchar();
					system("pause");
				}
				
			}
		}
		if (iS>=maxelm) {
			// ��������� ����
			inumber=iS-maxelm;
			if (sosedb[inumber].MCB==(ls+lw)) {
				if (fabs(mf[iP][SSIDE])>admission) {
#if doubleintprecision == 1
					printf("wall mf flux velocity non zero iS=%lld", iS);
#else
					printf("wall mf flux velocity non zero iS=%d", iS);
#endif
					
					//getchar();
					system("pause");
				}
				
			}
		}
		if (iT>=maxelm) {
			// ��������� ����
			inumber=iT-maxelm;
			if (sosedb[inumber].MCB==(ls+lw)) {
				if (fabs(mf[iP][TSIDE])>admission) {
#if doubleintprecision == 1
					printf("wall mf flux velocity non zero iT=%lld", iT);
#else
					printf("wall mf flux velocity non zero iT=%d", iT);
#endif
					
					//getchar();
					system("pause");
				}
				
			}
		}
		if (iB>=maxelm) {
			// ��������� ����
			inumber=iB-maxelm;
			if (sosedb[inumber].MCB==(ls+lw)) {
				if (fabs(mf[iP][BSIDE])>admission) {
#if doubleintprecision == 1
					printf("wall mf flux velocity non zero iB=%lld", iB);
#else
					printf("wall mf flux velocity non zero iB=%d", iB);
#endif
					
					//getchar();
					system("pause");
				}
				
			}
		}


	}

}

void iscorrectOk(doublereal** &potent,
							 integer maxelm, 
							 ALICE_PARTITION** sosedi, BOUND* &sosedb,
							 integer ls, integer lw, WALL* w)
{
	integer iP=0;
	integer inumber;
    // iP - ����� ������������ ������������ ������
	for (iP=0; iP<maxelm; iP++) {
		integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	    iE=sosedi[ESIDE][iP].iNODE1; iN=sosedi[NSIDE][iP].iNODE1; iT=sosedi[TSIDE][iP].iNODE1;
	    iW=sosedi[WSIDE][iP].iNODE1; iS=sosedi[SSIDE][iP].iNODE1; iB=sosedi[BSIDE][iP].iNODE1;

		if (iE>=maxelm) {
			// ��������� ����
			inumber=iE-maxelm;
			if (sosedb[inumber].MCB==(ls+lw)) {
				if (fabs(potent[VX][iE])>admission) {
#if doubleintprecision == 1
					printf("wall VX velocity non zero iE=%lld", iE);
#else
					printf("wall VX velocity non zero iE=%d", iE);
#endif

					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VY][iE])>admission) {
#if doubleintprecision == 1
					printf("wall VY velocity non zero iE=%lld", iE);
#else
					printf("wall VY velocity non zero iE=%d", iE);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VZ][iE])>admission) {
#if doubleintprecision == 1
					printf("wall VZ velocity non zero iE=%lld", iE);
#else
					printf("wall VZ velocity non zero iE=%d", iE);
#endif
					
					//getchar();
					system("pause");
				}
			}
		}
		if (iW>=maxelm) {
			// ��������� ����
			inumber=iW-maxelm;
			if (sosedb[inumber].MCB==(ls+lw)) {
				if (fabs(potent[VX][iW])>admission) {
#if doubleintprecision == 1
					printf("wall VX velocity non zero iW=%lld", iW);
#else
					printf("wall VX velocity non zero iW=%d", iW);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VY][iW])>admission) {
#if doubleintprecision == 1
					printf("wall VY velocity non zero iW=%lld", iW);
#else
					printf("wall VY velocity non zero iW=%d", iW);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VZ][iW])>admission) {
#if doubleintprecision == 1
					printf("wall VZ velocity non zero iW=%lld", iW);
#else
					printf("wall VZ velocity non zero iW=%d", iW);
#endif
					
					//getchar();
					system("pause");
				}
			}
		}

		if (iN>=maxelm) {
			// ��������� ����
			inumber=iN-maxelm;
			if (sosedb[inumber].MCB==(ls+lw)) {
				if (fabs(potent[VX][iN])>admission) {
#if doubleintprecision == 1
					printf("wall VX velocity non zero iN=%lld", iN);
#else
					printf("wall VX velocity non zero iN=%d", iN);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VY][iN])>admission) {
#if doubleintprecision == 1
					printf("wall VY velocity non zero iN=%lld", iN);
#else
					printf("wall VY velocity non zero iN=%d", iN);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VZ][iN])>admission) {
#if doubleintprecision == 1
					printf("wall VZ velocity non zero iN=%lld", iN);
#else
					printf("wall VZ velocity non zero iN=%d", iN);
#endif
					
					//getchar();
					system("pause");
				}
			}
		}
		if (iS>=maxelm) {
			// ��������� ����
			inumber=iS-maxelm;
			if (sosedb[inumber].MCB==(ls+lw)) {
				if (fabs(potent[VX][iS])>admission) {
#if doubleintprecision == 1
					printf("wall VX velocity non zero iS=%lld", iS);
#else
					printf("wall VX velocity non zero iS=%d", iS);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VY][iS])>admission) {
#if doubleintprecision == 1
					printf("wall VY velocity non zero iS=%lld", iS);
#else
					printf("wall VY velocity non zero iS=%d", iS);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VZ][iS])>admission) {
#if doubleintprecision == 1
					printf("wall VZ velocity non zero iS=%lld", iS);
#else
					printf("wall VZ velocity non zero iS=%d", iS);
#endif
					
					//getchar();
					system("pause");
				}
			}
		}

		if (iT>=maxelm) {
			// ��������� ����
			inumber=iT-maxelm;
			if (sosedb[inumber].MCB==(ls+lw)) {
				if (fabs(potent[VX][iT])>admission) {
#if doubleintprecision == 1
					printf("wall VX velocity non zero iT=%lld", iT);
#else
					printf("wall VX velocity non zero iT=%d", iT);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VY][iT])>admission) {
#if doubleintprecision == 1
					printf("wall VY velocity non zero iT=%lld", iT);
#else
					printf("wall VY velocity non zero iT=%d", iT);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VZ][iT])>admission) {
#if doubleintprecision == 1
					printf("wall VZ velocity non zero iT=%lld", iT);
#else
					printf("wall VZ velocity non zero iT=%d", iT);
#endif
					
					//getchar();
					system("pause");
				}
			}
		}
		if (iB>=maxelm) {
			// ��������� ����
			inumber=iB-maxelm;
			if (sosedb[inumber].MCB==(ls+lw)) {
				if (fabs(potent[VX][iB])>admission) {
#if doubleintprecision == 1
					printf("wall VX velocity non zero iB=%lld", iB);
#else
					printf("wall VX velocity non zero iB=%d", iB);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VY][iB])>admission) {
#if doubleintprecision == 1
					printf("wall VY velocity non zero iB=%lld", iB);
#else
					printf("wall VY velocity non zero iB=%d", iB);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VZ][iB])>admission) {
#if doubleintprecision == 1
					printf("wall VZ velocity non zero iB=%lld", iB);
#else
					printf("wall VZ velocity non zero iB=%d", iB);
#endif
					
					//getchar();
					system("pause");
				}
			}
		}

		if (iE>=maxelm) {
			// ��������� ����
			inumber=iE-maxelm;
			if (sosedb[inumber].MCB==(ls+lw)) {
				if (fabs(potent[VXCOR][iE])>admission) {
#if doubleintprecision == 1
					printf("wall VXCOR velocity non zero iE=%lld", iE);
#else
					printf("wall VXCOR velocity non zero iE=%d", iE);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VYCOR][iE])>admission) {
#if doubleintprecision == 1
					printf("wall VYCOR velocity non zero iE=%lld", iE);
#else
					printf("wall VYCOR velocity non zero iE=%d", iE);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VZCOR][iE])>admission) {
#if doubleintprecision == 1
					printf("wall VZCOR velocity non zero iE=%lld", iE);
#else
					printf("wall VZCOR velocity non zero iE=%d", iE);
#endif
					
					//getchar();
					system("pause");
				}
			}
		}
		if (iW>=maxelm) {
			// ��������� ����
			inumber=iW-maxelm;
			if (sosedb[inumber].MCB==(ls+lw)) {
				if (fabs(potent[VXCOR][iW])>admission) {
#if doubleintprecision == 1
					printf("wall VXCOR velocity non zero iW=%lld", iW);
#else
					printf("wall VXCOR velocity non zero iW=%d", iW);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VYCOR][iW])>admission) {
#if doubleintprecision == 1
					printf("wall VYCOR velocity non zero iW=%lld", iW);
#else
					printf("wall VYCOR velocity non zero iW=%d", iW);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VZCOR][iW])>admission) {
#if doubleintprecision == 1
					printf("wall VZCOR velocity non zero iW=%lld", iW);
#else
					printf("wall VZCOR velocity non zero iW=%d", iW);
#endif
					
					//getchar();
					system("pause");
				}
			}
		}

		if (iN>=maxelm) {
			// ��������� ����
			inumber=iN-maxelm;
			if (sosedb[inumber].MCB==(ls+lw)) {
				if (fabs(potent[VXCOR][iN])>admission) {
#if doubleintprecision == 1
					printf("wall VXCOR velocity non zero iN=%lld", iN);
#else
					printf("wall VXCOR velocity non zero iN=%d", iN);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VYCOR][iN])>admission) {
#if doubleintprecision == 1
					printf("wall VYCOR velocity non zero iN=%lld", iN);
#else
					printf("wall VYCOR velocity non zero iN=%d", iN);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VZCOR][iN])>admission) {
#if doubleintprecision == 1
					printf("wall VZCOR velocity non zero iN=%lld", iN);
#else
					printf("wall VZCOR velocity non zero iN=%d", iN);
#endif
					
					//getchar();
					system("pause");
				}
			}
		}
		if (iS>=maxelm) {
			// ��������� ����
			inumber=iS-maxelm;
			if (sosedb[inumber].MCB==(ls+lw)) {
				if (fabs(potent[VXCOR][iS])>admission) {
#if doubleintprecision == 1
					printf("wall VXCOR velocity non zero iS=%lld", iS);
#else
					printf("wall VXCOR velocity non zero iS=%d", iS);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VYCOR][iS])>admission) {
#if doubleintprecision == 1
					printf("wall VYCOR velocity non zero iS=%lld", iS);
#else
					printf("wall VYCOR velocity non zero iS=%d", iS);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VZCOR][iS])>admission) {
#if doubleintprecision == 1
					printf("wall VZCOR velocity non zero iS=%lld", iS);
#else
					printf("wall VZCOR velocity non zero iS=%d", iS);
#endif
					
					//getchar();
					system("pause");
				}
			}
		}

		if (iT>=maxelm) {
			// ��������� ����
			inumber=iT-maxelm;
			if (sosedb[inumber].MCB==(ls+lw)) {
				if (fabs(potent[VXCOR][iT])>admission) {
#if doubleintprecision == 1
					printf("wall VXCOR velocity non zero iT=%lld", iT);
#else
					printf("wall VXCOR velocity non zero iT=%d", iT);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VYCOR][iT])>admission) {
#if doubleintprecision == 1
					printf("wall VYCOR velocity non zero iT=%lld", iT);
#else
					printf("wall VYCOR velocity non zero iT=%d", iT);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VZCOR][iT])>admission) {
#if doubleintprecision == 1
					printf("wall VZCOR velocity non zero iT=%lld", iT);
#else
					printf("wall VZCOR velocity non zero iT=%d", iT);
#endif
					
					//getchar();
					system("pause");
				}
			}
		}
		if (iB>=maxelm) {
			// ��������� ����
			inumber=iB-maxelm;
			if (sosedb[inumber].MCB==(ls+lw)) {
				if (fabs(potent[VXCOR][iB])>admission) {
#if doubleintprecision == 1
					printf("wall VXCOR velocity non zero iB=%lld", iB);
#else
					printf("wall VXCOR velocity non zero iB=%d", iB);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VYCOR][iB])>admission) {
#if doubleintprecision == 1
					printf("wall VYCOR velocity non zero iB=%lld", iB);
#else
					printf("wall VYCOR velocity non zero iB=%d", iB);
#endif
					
					//getchar();
					system("pause");
				}
				if (fabs(potent[VZCOR][iB])>admission) {
#if doubleintprecision == 1
					printf("wall VZCOR velocity non zero iB=%lld", iB);
#else
					printf("wall VZCOR velocity non zero iB=%d", iB);
#endif
					
					//getchar();
					system("pause");
				}
			}
		}

	}
	
}

// ��������� ��������� ����� ���� �� ��� ����� ������� �������.
void correct_boundary_volume(integer iVar, doublereal** &potent,
							 integer maxelm, integer** nvtx, TOCHKA* pa, 
							 ALICE_PARTITION** sosedi, BOUND* &sosedb,
							 integer ls, integer lw, WALL* w, doublereal* &relax_value) {

   // ��������:
   // ������ �� ���� ���������� ��. 
   // ��� ������� ����������� �� ������ �� ���� ��� �������:
   // ���� �������� ���� �������� ���������, ������������� ������ wall
   // �� ������� ����� ������� ��������� �������� ����, �� ��� ����
   // ��������� �������� �� ���� ������� ����� ���������� ������� �������,
   // � ������� ����� ����� �������� ��������� ������� ��� ����������������� ��������
   // ��������� ���������� �������� �� ������� � ��������� ��������������� ��������� ��������
   // � ��������� ���������� ����.

    // iVar - ������������ (�������������) ������������� ���������� ��������.

	// 14 ��� � ��� ��������� ������������ ������������ �� �������.
	// ��� ������������ ����������� �� ���������� ��������� SIMPLEC (������� ������� �).
	// ������ ������, ������� ���� ��������� �������� ������������� ��������� ��������.
	// � �� ������� ����� ���������� ������� �������, ��� �������� ��������� �������� �������� F
	// � ��������� � ��������� ������������ ����. �������, ������ �� ���� �����������, ������ ���������� ���������
	// ������� �������������� ��������� binterpol==0. ������ �������� 0 ��������� ����������. ���������� �������� 
	// ���������� �������������� ��������� 2 (������������ �������������).
	// 1 ������������ �������� ������������ �� �������. (0 �� ������������).
	integer binterpol=0; // 2 - ������������ ������������ �������������.
	bool brelax_bound = false;
	bool brelax_val2 = true;
	const doublereal relaxboundconstvel = 1.0;

    integer iP=0;
	integer inumber;
    // iP - ����� ������������ ������������ ������
	for (iP=0; iP<maxelm; iP++) {
		integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	    iE=sosedi[ESIDE][iP].iNODE1; iN=sosedi[NSIDE][iP].iNODE1; iT=sosedi[TSIDE][iP].iNODE1;
	    iW=sosedi[WSIDE][iP].iNODE1; iS=sosedi[SSIDE][iP].iNODE1; iB=sosedi[BSIDE][iP].iNODE1;

		// ���������� �������� �������� ������������ ������:
	    doublereal dx=0.0, dy=0.0, dz=0.0; // ������� ������������ ������
        volume3D(iP, nvtx, pa, dx, dy, dz);
		
		if (iE>=maxelm) {
			// ��������� ����
			inumber=iE-maxelm;
			if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && (w[sosedb[inumber].MCB - ls].bpressure || w[sosedb[inumber].MCB - ls].bopening))) {
				// �� ���� ������� ����������� �������� ������ �� ���� ��������� ����� ������� �������.
				// ������ �������� � ��������� ���� ����� �������������� ������� � �� �������� �� ���������� ����������� ����.
				if (binterpol==0) {
					if (brelax_bound) {
						// ����� �������� ���� ������������� � ���������������� �������� ��������������� ��������� �������������.
						if (brelax_val2) {
							potent[iVar][iE] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*relax_value[iE];
						}
						else {
							potent[iVar][iE] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*potent[iVar][iE];
						}
					}
					else {
						potent[iVar][iE] = potent[iVar][iP]; // ������������ ��������.
					}
				}
				else if (binterpol==1) {
					// �������� ������������:
					TOCHKA pp,pb;
		            center_cord3D(iP, nvtx, pa, pp,100);
		            center_cord3D(iW, nvtx, pa, pb,WSIDE);
		            potent[iVar][iE]=my_linear_interpolation('+', potent[iVar][iP], potent[iVar][iW], pp.x, pb.x, pp.x+0.5*dx);
				}
				else if (binterpol==2) {
					// ������������ �������������.

					TOCHKA pp,pb,pbb;
		            center_cord3D(iP, nvtx, pa, pp,100);
		            center_cord3D(iW, nvtx, pa, pb,WSIDE);
					center_cord3D(sosedi[WSIDE][iW].iNODE1, nvtx, pa, pbb,WW);
					
					potent[iVar][iE]=my_quadratic_interpolation('+', potent[iVar][sosedi[WSIDE][iW].iNODE1], potent[iVar][iW], potent[iVar][iP], pbb.x , pb.x, pp.x, pp.x+0.5*dx);
				}
			} // pressure outlet
			else if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bsymmetry)) {
				// ������� ���������: �� VY � VZ ����� ���������� ������� �������, � ��� VX==0.0;
				// ������ �������� VY � VZ � ��������� ���� ����� �������������� ������� � �� �������� �� ���������� ����������� ����,
				// ��� ����� ����������� ��������� ������� ��� ����������������� ��������.
				switch (iVar) {
				   case VX : potent[iVar][iE]=0.0; break; // �� ����������� ������ ��� ���������� �������� ����� ����.
				   case VY : case VZ : if (binterpol==0) {
					   if (brelax_bound) {
						   // ����� �������� ���� ������������� � ���������������� �������� ��������������� ��������� �������������.
						   if (brelax_val2) {
							   potent[iVar][iE] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*relax_value[iE];
						   }
						   else {
							   potent[iVar][iE] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*potent[iVar][iE];
						   }
					   }
					   else {
						   potent[iVar][iE] = potent[iVar][iP];
					   }
							 }
							 else if (binterpol==1) {
								 // �������� ������������:
					             TOCHKA pp,pb;
		                         center_cord3D(iP, nvtx, pa, pp,100);
		                         center_cord3D(iW, nvtx, pa, pb,WSIDE);
		                         potent[iVar][iE]=my_linear_interpolation('+', potent[iVar][iP], potent[iVar][iW], pp.x, pb.x, pp.x+0.5*dx);
							 }
							 else if (binterpol==2) {
								 // ������������ �������������.

					             TOCHKA pp,pb,pbb;
		                         center_cord3D(iP, nvtx, pa, pp,100);
		                         center_cord3D(iW, nvtx, pa, pb,WSIDE);
					             center_cord3D(sosedi[WSIDE][iW].iNODE1, nvtx, pa, pbb,WW);
					
					             potent[iVar][iE]=my_quadratic_interpolation('+', potent[iVar][sosedi[WSIDE][iW].iNODE1], potent[iVar][iW], potent[iVar][iP], pbb.x , pb.x, pp.x, pp.x+0.5*dx);
				             }
							 break; // ������������ ��������.
				}
				 
			} // symmetry
			else if ((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw))) {
				switch (iVar) {
				  case VX : potent[iVar][iE]=w[sosedb[inumber].MCB-ls].Vx; break;
				  case VY : potent[iVar][iE]=w[sosedb[inumber].MCB-ls].Vy; break;
				  case VZ : potent[iVar][iE]=w[sosedb[inumber].MCB-ls].Vz; break;
				}				
			}
			else {
				// ������ ����������� ������ Stacionary WALL
                switch (iVar) {
				  case VX : potent[iVar][iE]=0.0; break;
				  case VY : potent[iVar][iE]=0.0; break;
				  case VZ : potent[iVar][iE]=0.0; break;
				}				
			}


		} // iE

		if (iW>=maxelm) {
			// ��������� ����
			inumber=iW-maxelm;
			if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && (w[sosedb[inumber].MCB - ls].bpressure || w[sosedb[inumber].MCB - ls].bopening))) {
				// �� ���� ������� ����������� �������� ������ �� ���� ��������� ����� ������� �������.
				// ������ �������� � ��������� ���� ����� �������������� ������� � �� �������� �� ���������� ����������� ����.
				if (binterpol==0) {
					if (brelax_bound) {
						// ����� �������� ���� ������������� � ���������������� �������� ��������������� ��������� �������������.
						if (brelax_val2) {
							potent[iVar][iW] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*relax_value[iW];
						}
						else {
							potent[iVar][iW] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*potent[iVar][iW];
						}
					}
					else {
						potent[iVar][iW] = potent[iVar][iP]; // ������������ ��������.
					}
				}
				else if (binterpol==1) {
                    TOCHKA pp,pb;
		            center_cord3D(iP, nvtx, pa, pp,100);
		            center_cord3D(iE, nvtx, pa, pb,ESIDE);
		            potent[iVar][iW]=my_linear_interpolation('-', potent[iVar][iP], potent[iVar][iE], pp.x, pb.x, pp.x-0.5*dx);
				}
				else if (binterpol==2) {
					// ������������ �������������.

					TOCHKA pp,pb,pbb;
		            center_cord3D(iP, nvtx, pa, pp,100);
		            center_cord3D(iE, nvtx, pa, pb,ESIDE);
					center_cord3D(sosedi[ESIDE][iE].iNODE1, nvtx, pa, pbb,EE);
					
					potent[iVar][iW]=my_quadratic_interpolation('-', potent[iVar][sosedi[ESIDE][iE].iNODE1], potent[iVar][iE], potent[iVar][iP], pbb.x , pb.x, pp.x, pp.x-0.5*dx);
				}
			} // pressure outlet
			else if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bsymmetry)) {
				// ������� ���������: �� VY � VZ ����� ���������� ������� �������, � ��� VX==0.0;
				// ������ �������� VY � VZ � ��������� ���� ����� �������������� ������� � �� �������� �� ���������� ����������� ����,
				// ��� ����� ����������� ��������� ������� ��� ����������������� ��������.
				switch (iVar) {
				   case VX : potent[iVar][iW]=0.0; break; // �� ����������� ������ ��� ���������� �������� ����� ����.
				   case VY : case VZ : if (binterpol==0) {
					          if (brelax_bound) {
						          // ����� �������� ���� ������������� � ���������������� �������� ��������������� ��������� �������������.
								  if (brelax_val2) {
									  potent[iVar][iW] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*relax_value[iW];
								  }
								  else {
									  potent[iVar][iW] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*potent[iVar][iW];
								  }
							}
					          else {
						          potent[iVar][iW] = potent[iVar][iP]; // ������������ ��������.
					          }
					      }
				             else if (binterpol==1) {
                                TOCHKA pp,pb;
		                        center_cord3D(iP, nvtx, pa, pp,100);
		                        center_cord3D(iE, nvtx, pa, pb,ESIDE);
		                        potent[iVar][iW]=my_linear_interpolation('-', potent[iVar][iP], potent[iVar][iE], pp.x, pb.x, pp.x-0.5*dx);
				             }
							 else if (binterpol==2) {
								 // ������������ �������������.

					             TOCHKA pp,pb,pbb;
		                         center_cord3D(iP, nvtx, pa, pp,100);
		                         center_cord3D(iE, nvtx, pa, pb,ESIDE);
					             center_cord3D(sosedi[ESIDE][iE].iNODE1, nvtx, pa, pbb,EE);
					
					             potent[iVar][iW]=my_quadratic_interpolation('-', potent[iVar][sosedi[ESIDE][iE].iNODE1], potent[iVar][iE], potent[iVar][iP], pbb.x , pb.x, pp.x, pp.x-0.5*dx);
				             }
					         break; // ������������ ��������.
				}
				 
			} // symmetry
			else if ((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw))) {
				switch (iVar) {
				  case VX : potent[iVar][iW]=w[sosedb[inumber].MCB-ls].Vx; break;
				  case VY : potent[iVar][iW]=w[sosedb[inumber].MCB-ls].Vy; break;
				  case VZ : potent[iVar][iW]=w[sosedb[inumber].MCB-ls].Vz; break;
				}
			}
			else {
				// ������ ����������� ������ Stacionary WALL
                switch (iVar) {
				  case VX : potent[iVar][iW]=0.0; break;
				  case VY : potent[iVar][iW]=0.0; break;
				  case VZ : potent[iVar][iW]=0.0; break;
				}
			}

		} // iW

		if (iN>=maxelm) {
			// ��������� ����
			inumber=iN-maxelm;
			if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && (w[sosedb[inumber].MCB - ls].bpressure || w[sosedb[inumber].MCB - ls].bopening))) {
				// �� ���� ������� ����������� �������� ������ �� ���� ��������� ����� ������� �������.
				// ������ �������� � ��������� ���� ����� �������������� ������� � �� �������� �� ���������� ����������� ����.
				if (binterpol==0) {
					if (brelax_bound) {
						// ����� �������� ���� ������������� � ���������������� �������� ��������������� ��������� �������������.
						if (brelax_val2) {
							potent[iVar][iN] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*relax_value[iN];
						}
						else {
							potent[iVar][iN] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*potent[iVar][iN];
						}
					}
					else {
						potent[iVar][iN] = potent[iVar][iP]; // ������������ ��������.
					}
				}
				else if (binterpol==1) {
					 TOCHKA pp,pb;
		             center_cord3D(iP, nvtx, pa, pp,100);
		             center_cord3D(iS, nvtx, pa, pb,SSIDE);
		             potent[iVar][iN]=my_linear_interpolation('+', potent[iVar][iP], potent[iVar][iS], pp.y, pb.y, pp.y+0.5*dy);
				}
				else if (binterpol==2) {
					// ������������ �������������.

					TOCHKA pp,pb,pbb;
		            center_cord3D(iP, nvtx, pa, pp,100);
		            center_cord3D(iS, nvtx, pa, pb,SSIDE);
					center_cord3D(sosedi[SSIDE][iS].iNODE1, nvtx, pa, pbb,SS);
					
					potent[iVar][iN]=my_quadratic_interpolation('+', potent[iVar][sosedi[SSIDE][iS].iNODE1], potent[iVar][iS], potent[iVar][iP], pbb.y , pb.y, pp.y, pp.y+0.5*dy);
				}
			} // pressure outlet
			else if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bsymmetry)) {
				// ������� ���������: �� VX � VZ ����� ���������� ������� �������, � ��� VY==0.0;
				// ������ �������� VX � VZ � ��������� ���� ����� �������������� ������� � �� �������� �� ���������� ����������� ����,
				// ��� ����� ����������� ��������� ������� ��� ����������������� ��������.
				switch (iVar) {
				   case VX : case VZ : if (binterpol==0) {
					   if (brelax_bound) {
						   // ����� �������� ���� ������������� � ���������������� �������� ��������������� ��������� �������������.
						   if (brelax_val2) {
							   potent[iVar][iN] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*relax_value[iN];
						   }
						   else {
							   potent[iVar][iN] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*potent[iVar][iN];
						   }
					   }
					   else {
						   potent[iVar][iN] = potent[iVar][iP]; // ������������ ��������.
					   }
				             }
				             else if (binterpol==1) {
					             TOCHKA pp,pb;
		                         center_cord3D(iP, nvtx, pa, pp,100);
		                         center_cord3D(iS, nvtx, pa, pb,SSIDE);
		                         potent[iVar][iN]=my_linear_interpolation('+', potent[iVar][iP], potent[iVar][iS], pp.y, pb.y, pp.y+0.5*dy);
				             }
							 else if (binterpol==2) {
								 // ������������ �������������.

					             TOCHKA pp,pb,pbb;
		                         center_cord3D(iP, nvtx, pa, pp,100);
		                         center_cord3D(iS, nvtx, pa, pb,SSIDE);
					             center_cord3D(sosedi[SSIDE][iS].iNODE1, nvtx, pa, pbb,SS);
					
					             potent[iVar][iN]=my_quadratic_interpolation('+', potent[iVar][sosedi[SSIDE][iS].iNODE1], potent[iVar][iS], potent[iVar][iP], pbb.y , pb.y, pp.y, pp.y+0.5*dy);
				             }
					         break; // ������������ ��������.
				   case VY : potent[iVar][iN]=0.0; break; // �� ����������� ������ ��� ���������� �������� ����� ����.
				}
				 
			} // symmetry
			else if ((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw))) {
				switch (iVar) {
				  case VX : potent[iVar][iN]=w[sosedb[inumber].MCB-ls].Vx; break;
				  case VY : potent[iVar][iN]=w[sosedb[inumber].MCB-ls].Vy; break;
				  case VZ : potent[iVar][iN]=w[sosedb[inumber].MCB-ls].Vz; break;
				}
			}
			else {
				// ������ ����������� ������ Stacionary WALL
                switch (iVar) {
				  case VX : potent[iVar][iN]=0.0; break;
				  case VY : potent[iVar][iN]=0.0; break;
				  case VZ : potent[iVar][iN]=0.0; break;
				}
			}

		} // iN

		if (iS>=maxelm) {
			// ��������� ����
			inumber=iS-maxelm;
			if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && (w[sosedb[inumber].MCB - ls].bpressure || w[sosedb[inumber].MCB - ls].bopening))) {
				// �� ���� ������� ����������� �������� ������ �� ���� ��������� ����� ������� �������.
				// ������ �������� � ��������� ���� ����� �������������� ������� � �� �������� �� ���������� ����������� ����.
				if (binterpol==0) {
					if (brelax_bound) {
						// ����� �������� ���� ������������� � ���������������� �������� ��������������� ��������� �������������.
						if (brelax_val2) {
							potent[iVar][iS] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*relax_value[iS];
						}
						else {
							potent[iVar][iS] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*potent[iVar][iS];
						}
					}
					else {
						potent[iVar][iS] = potent[iVar][iP]; // ������������ ��������.
					}
				}
				else if (binterpol==1) {
					TOCHKA pp,pb;
		            center_cord3D(iP, nvtx, pa, pp,100);
		            center_cord3D(iN, nvtx, pa, pb,NSIDE);
		            potent[iVar][iS]=my_linear_interpolation('-', potent[iVar][iP], potent[iVar][iN], pp.y, pb.y, pp.y-0.5*dy);
				}
				else if (binterpol==2) {
					// ������������ �������������.

					TOCHKA pp,pb,pbb;
		            center_cord3D(iP, nvtx, pa, pp,100);
		            center_cord3D(iN, nvtx, pa, pb,NSIDE);
					center_cord3D(sosedi[NSIDE][iN].iNODE1, nvtx, pa, pbb,NN);
					
					potent[iVar][iS]=my_quadratic_interpolation('-', potent[iVar][sosedi[NSIDE][iN].iNODE1], potent[iVar][iN], potent[iVar][iP], pbb.y , pb.y, pp.y, pp.y-0.5*dy);
				}
				//if (iVar==VY) { printf("Vs==%e, Vp==%e\n",potent[iVar][iS],potent[iVar][iP]); getchar(); } // debug
			} // pressure outlet
			else if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bsymmetry)) {
				// ������� ���������: �� VX � VZ ����� ���������� ������� �������, � ��� VY==0.0;
				// ������ �������� VX � VZ � ��������� ���� ����� �������������� ������� � �� �������� �� ���������� ����������� ����,
				// ��� ����� ����������� ��������� ������� ��� ����������������� ��������.
				switch (iVar) {
				   case VX : case VZ : if (binterpol==0) {
					   if (brelax_bound) {
						      // ����� �������� ���� ������������� � ���������������� �������� ��������������� ��������� �������������.
						   if (brelax_val2) {
							   potent[iVar][iS] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*relax_value[iS];
						   }
						   else {
							   potent[iVar][iS] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*potent[iVar][iS];
						   }
					        }
					        else {
						        potent[iVar][iS] = potent[iVar][iP]; // ������������ ��������.
					          }
				             }
				             else if (binterpol==1) {
					             TOCHKA pp,pb;
		                         center_cord3D(iP, nvtx, pa, pp,100);
		                         center_cord3D(iN, nvtx, pa, pb,NSIDE);
		                         potent[iVar][iS]=my_linear_interpolation('-', potent[iVar][iP], potent[iVar][iN], pp.y, pb.y, pp.y-0.5*dy);
				             }
							 else if (binterpol==2) {
								 // ������������ �������������.

					             TOCHKA pp,pb,pbb;
		                         center_cord3D(iP, nvtx, pa, pp,100);
		                         center_cord3D(iN, nvtx, pa, pb,NSIDE);
					             center_cord3D(sosedi[NSIDE][iN].iNODE1, nvtx, pa, pbb,NN);
					
					             potent[iVar][iS]=my_quadratic_interpolation('-', potent[iVar][sosedi[NSIDE][iN].iNODE1], potent[iVar][iN], potent[iVar][iP], pbb.y , pb.y, pp.y, pp.y-0.5*dy);
				             }
					         break; // ������������ ��������.
				   case VY : potent[iVar][iS]=0.0; break; // �� ����������� ������ ��� ���������� �������� ����� ����.
				}
				 
			} // symmetry
			else if ((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw))) {
				switch (iVar) {
				  case VX : potent[iVar][iS]=w[sosedb[inumber].MCB-ls].Vx; break;
				  case VY : potent[iVar][iS]=w[sosedb[inumber].MCB-ls].Vy; break;
				  case VZ : potent[iVar][iS]=w[sosedb[inumber].MCB-ls].Vz; break;
				}
			}
			else {
				// ������ ����������� ������ Stacionary WALL
                switch (iVar) {
				  case VX : potent[iVar][iS]=0.0; break;
				  case VY : potent[iVar][iS]=0.0; break;
				  case VZ : potent[iVar][iS]=0.0; break;
				}
			}

		} // iS

		if (iT>=maxelm) {
			// ��������� ����
			inumber=iT-maxelm;
			if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && (w[sosedb[inumber].MCB - ls].bpressure || w[sosedb[inumber].MCB - ls].bopening))) {
				// �� ���� ������� ����������� �������� ������ �� ���� ��������� ����� ������� �������.
				// ������ �������� � ��������� ���� ����� �������������� ������� � �� �������� �� ���������� ����������� ����.
				if (binterpol==0) {
					if (brelax_bound) {
						// ����� �������� ���� ������������� � ���������������� �������� ��������������� ��������� �������������.
						if (brelax_val2) {
							potent[iVar][iT] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*relax_value[iT];
						}
						else {
							potent[iVar][iT] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*potent[iVar][iT];
						}
					}
					else {
						potent[iVar][iT] = potent[iVar][iP]; // ������������ ��������.
					}
				}
				else if (binterpol==1) {
					TOCHKA pp,pb;
		            center_cord3D(iP, nvtx, pa, pp,100);
		            center_cord3D(iB, nvtx, pa, pb,BSIDE);
		            potent[iVar][iT]=my_linear_interpolation('+', potent[iVar][iP], potent[iVar][iB], pp.z, pb.z, pp.z+0.5*dz);
				}
				else if (binterpol==2) {
					// ������������ �������������.

					TOCHKA pp,pb,pbb;
		            center_cord3D(iP, nvtx, pa, pp,100);
		            center_cord3D(iB, nvtx, pa, pb,BSIDE);
					center_cord3D(sosedi[BSIDE][iB].iNODE1, nvtx, pa, pbb,BB);
					
					potent[iVar][iT]=my_quadratic_interpolation('+', potent[iVar][sosedi[BSIDE][iB].iNODE1], potent[iVar][iB], potent[iVar][iP], pbb.z , pb.z, pp.z, pp.z+0.5*dz);
				}
			} // pressure outlet
			else  if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bsymmetry)) {
				// ������� ���������: �� VX � VY ����� ���������� ������� �������, � ��� VZ==0.0;
				// ������ �������� VX � VY � ��������� ���� ����� �������������� ������� � �� �������� �� ���������� ����������� ����,
				// ��� ����� ����������� ��������� ������� ��� ����������������� ��������.
				switch (iVar) {
				   case VX : case VY : if (binterpol==0) {
					   if (brelax_bound) {
						   // ����� �������� ���� ������������� � ���������������� �������� ��������������� ��������� �������������.
						   if (brelax_val2) {
							   potent[iVar][iT] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*relax_value[iT];
						   }
						   else {
							   potent[iVar][iT] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*potent[iVar][iT];
						   }
					   }
					   else {
						   potent[iVar][iT] = potent[iVar][iP]; // ������������ ��������.
					   }
				             }
				             else if (binterpol==1) {
					            TOCHKA pp,pb;
		                        center_cord3D(iP, nvtx, pa, pp,100);
		                        center_cord3D(iB, nvtx, pa, pb,BSIDE);
		                        potent[iVar][iT]=my_linear_interpolation('+', potent[iVar][iP], potent[iVar][iB], pp.z, pb.z, pp.z+0.5*dz);
				             }
							 else if (binterpol==2) {
								 // ������������ �������������.

								 TOCHKA pp,pb,pbb;
		                         center_cord3D(iP, nvtx, pa, pp,100);
		                         center_cord3D(iB, nvtx, pa, pb,BSIDE);
								 center_cord3D(sosedi[BSIDE][iB].iNODE1, nvtx, pa, pbb,BB);

								 potent[iVar][iT]=my_quadratic_interpolation('+', potent[iVar][sosedi[BSIDE][iB].iNODE1], potent[iVar][iB], potent[iVar][iP], pbb.z , pb.z, pp.z, pp.z+0.5*dz);
							 }
					         break; // ������������ ��������.
				   case VZ : potent[iVar][iT]=0.0; break; // �� ����������� ������ ��� ���������� �������� ����� ����.
				}
				 
			} // symmetry
			else if ((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw))) {
				switch (iVar) {
				  case VX : potent[iVar][iT]=w[sosedb[inumber].MCB-ls].Vx; break;
				  case VY : potent[iVar][iT]=w[sosedb[inumber].MCB-ls].Vy; break;
				  case VZ : potent[iVar][iT]=w[sosedb[inumber].MCB-ls].Vz; break;
				}
			}
			else {
				// ������ ����������� ������ Stacionary WALL
                switch (iVar) {
				  case VX : potent[iVar][iT]=0.0; break;
				  case VY : potent[iVar][iT]=0.0; break;
				  case VZ : potent[iVar][iT]=0.0; break;
				}
			}

		} // iT

		if (iB>=maxelm) {
			// ��������� ����
			inumber=iB-maxelm;
			if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && (w[sosedb[inumber].MCB - ls].bpressure ||  w[sosedb[inumber].MCB - ls].bopening))) {
				// �� ���� ������� ����������� �������� ������ �� ���� ��������� ����� ������� �������.
				// ������ �������� � ��������� ���� ����� �������������� ������� � �� �������� �� ���������� ����������� ����.
				if (binterpol==0) {
					if (brelax_bound) {
						// ����� �������� ���� ������������� � ���������������� �������� ��������������� ��������� �������������.
						if (brelax_val2) {
							potent[iVar][iB] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*relax_value[iB];
						}
						else {
							potent[iVar][iB] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*potent[iVar][iB];
						}
					}
					else {
						potent[iVar][iB] = potent[iVar][iP]; // ������������ ��������.
					}
				}
				else if (binterpol==1) {
					TOCHKA pp,pb;
		            center_cord3D(iP, nvtx, pa, pp,100);
		            center_cord3D(iT, nvtx, pa, pb,TSIDE);
		            potent[iVar][iB]=my_linear_interpolation('-', potent[iVar][iP], potent[iVar][iT], pp.z, pb.z, pp.z-0.5*dz);
				}
				else if (binterpol==2) {
					// ������������ �������������.

					TOCHKA pp,pb,pbb;
		            center_cord3D(iP, nvtx, pa, pp,100);
		            center_cord3D(iT, nvtx, pa, pb,TSIDE);
					center_cord3D(sosedi[TSIDE][iT].iNODE1, nvtx, pa, pbb,TTSIDE);
					
					potent[iVar][iB]=my_quadratic_interpolation('-', potent[iVar][sosedi[TSIDE][iT].iNODE1], potent[iVar][iT], potent[iVar][iP], pbb.z , pb.z, pp.z, pp.z-0.5*dz);
				}
			} // pressure outlet
			else if (((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bsymmetry)) {
				// ������� ���������: �� VX � VY ����� ���������� ������� �������, � ��� VZ==0.0;
				// ������ �������� VX � VY � ��������� ���� ����� �������������� ������� � �� �������� �� ���������� ����������� ����,
				// ��� ����� ����������� ��������� ������� ��� ����������������� ��������.
				switch (iVar) {
				   case VX : case VY : if (binterpol==0) {
					   if (brelax_bound) {
						   // ����� �������� ���� ������������� � ���������������� �������� ��������������� ��������� �������������.
						   if (brelax_val2) {
							   potent[iVar][iB] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*relax_value[iB];
						   }
						   else {
							   potent[iVar][iB] = relaxboundconstvel*potent[iVar][iP] + (1.0 - relaxboundconstvel)*potent[iVar][iB];
						   }
					   }
					   else {
						   potent[iVar][iB] = potent[iVar][iP]; // ������������ ��������.
					   }
				             }
				             else if (binterpol==1) {
					             TOCHKA pp,pb;
		                         center_cord3D(iP, nvtx, pa, pp,100);
		                         center_cord3D(iT, nvtx, pa, pb,TSIDE);
		                         potent[iVar][iB]=my_linear_interpolation('-', potent[iVar][iP], potent[iVar][iT], pp.z, pb.z, pp.z-0.5*dz);
				             }
							 else if (binterpol==2) {
								 // ������������ �������������.

					             TOCHKA pp,pb,pbb;
		                         center_cord3D(iP, nvtx, pa, pp,100);
		                         center_cord3D(iT, nvtx, pa, pb,TSIDE);
					             center_cord3D(sosedi[TSIDE][iT].iNODE1, nvtx, pa, pbb,TTSIDE);
					
					             potent[iVar][iB]=my_quadratic_interpolation('-', potent[iVar][sosedi[TSIDE][iT].iNODE1], potent[iVar][iT], potent[iVar][iP], pbb.z , pb.z, pp.z, pp.z-0.5*dz);
				             }
					         break; // ������������ ��������.
				   case VZ : potent[iVar][iB]=0.0; break; // �� ����������� ������ ��� ���������� �������� ����� ����.
				}
				 
			} // symmetry
			else if ((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw))) {
				switch (iVar) {
				  case VX : potent[iVar][iB]=w[sosedb[inumber].MCB-ls].Vx; break;
				  case VY : potent[iVar][iB]=w[sosedb[inumber].MCB-ls].Vy; break;
				  case VZ : potent[iVar][iB]=w[sosedb[inumber].MCB-ls].Vz; break;
				}
			}
			else {
				// ������ ����������� ������ Stacionary WALL
                switch (iVar) {
				  case VX : potent[iVar][iB]=0.0; break;
				  case VY : potent[iVar][iB]=0.0; break;
				  case VZ : potent[iVar][iB]=0.0; break;
				}
			}

		} // iB

	}

} // correct_boundary_volume

#endif