// pamendment3.c 
// ������� ����������������� ������� 
// ����� � ������� ������������ ����������
// ������ ��� ��������� �������� ��������.


// � ������ ���� �� ���� ������� FLUID ����� ������� �������
// �������� ����������� � ��������� ��������� �����.
// ������ � �������������� �������� ����, � ����� � �����
// �������������� ���� ������� ������������ � ����������� ������.
// �������� � ����������� ������������� �������. 
// ����� ������� �� �� �������� ��� ���� ����� �������
// ��������� ��� �������� �������� ���� ������������ � 
// ������������ �����������. �������� ��� ����� SPD 
// � ������� ����������� ������ ��������� ������� ���������.
// ������ ������� �������� � ��������� �������� ������� �������� ������.



#ifndef PAMENDMENTv_0_07_C
#define PAMENDMENTv_0_07_C 1


// ������������� ����������� ��������� ���������-��������
// �� ����������� �����
#include "my_elmatr_quad_f3D.c"
// ���������� ���� �� ������������� ��� ��� ������������ �������� ������.
#include "pseudo_time.cpp"
// ��������� �������� �� ����������� �����
// ��� ���������� � ��������� ����������� �������.
#include "correct_velocity.cpp"

#define PRESSUREOUTLET 0 // �������� �������� �� �������� �������
#define BERNULLI 1 // ���� �������� �������� �� ��������� ������� �� �������� ���������� ������ ����, � ���� ������� �� �������� ����� ��������.


// ���������� ��������� �������.
// � ��������� ��� �������� ��������.
// ����� ���������� ���������.
void my_elmatr_quad_PAm_bon(equation3D_bon** &slb, equation3D** sl, 
							integer inumber, integer maxelm, integer maxbound, 
							BOUND* &sosedb, integer** nvtx, bool bPfix,
							doublereal dbeta, TOCHKA* pa, doublereal** potent,
							doublereal** prop, doublereal** prop_b, doublereal* alpha,
							integer ls, integer lw, WALL* w, bool bDirichlet, 
							ALICE_PARTITION** sosedi, doublereal **diag_coef, doublereal RCh,
							bool &breversedflow) {
	
    // inumber - ����� ���������� ����, � ������� ���������.
	bool bFReeStyle=false; // ������ ����� �� �������� �������� ��������, �������� �������� ���� ==true ������ ����� ���������.
	// ���� bFReeStyle==false �� �������� �������� ����������� ���� �� �������� ������� ���� � �����.

    
	

	// bDirichlet==true ������ �������� ������ ������� ������� �������.

	if ( (inumber==(maxbound-1)) && bPfix && (!bFReeStyle)) {
        // ��� ����������� ������� ���� 
		// ������ ��������� ������������� 
		// ����������� �������� � ����� ����� ��������� �������.

		if (bDirichlet) {
           // �������� �������� ������������ ����� ����:
		   slb[PAM][inumber].aw=1.0;
		   slb[PAM][inumber].ai=0.0;
		   slb[PAM][inumber].b=0.0;
		   slb[PAM][inumber].iI=-1; // �� ������������ � �������
		   slb[PAM][inumber].iW=sosedb[inumber].iB; 

		   // ��� ������� �������:
		   // ������ ������������ �������
		   // �� ����� ����.
		   slb[PAM][inumber].iW1=-1;
		   slb[PAM][inumber].iW2=-1;
		   slb[PAM][inumber].iW3=-1;
		   slb[PAM][inumber].iW4=-1;
		   //printf("pressure fix\n"); getchar(); // debug
		}

	}
	 else 
	{

		if ((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && (w[sosedb[inumber].MCB - ls].bpressure)) {
            
			if (!bFReeStyle) {
				doublereal rCe=0.0; // �������� �������� �� �������� �������
			    integer ioutflowcondition=BERNULLI; // PRESSUREOUTLET // BERNULLI �.�.�������
			    doublereal kineticenergy=0.0; // ������������ ������� ������: 0.5*rho*Vel!2.
			    doublereal rsign=1.0; // >0 �������� ���������� �� ��������� ������� ������, ���� <0 �� ������ � ������������ ����������� ��������.
			
			    if (bDirichlet) {

			    	switch (ioutflowcondition) {
				     case PRESSUREOUTLET : // �������� �������� ������������ � ����� ����:
				                      // ���������� ������ - ����������� ������������ �� ������ ��������.
				                      // �� ������ �� ��� ������� ���������������� ��� �� �������.
		                              slb[PAM][inumber].aw=1.0;
		                              slb[PAM][inumber].ai=0.0;
		                              slb[PAM][inumber].b=rCe-potent[PRESS][sosedb[inumber].iB];
		                              slb[PAM][inumber].iI=-1; // �� ������������ � �������
		                              slb[PAM][inumber].iW=sosedb[inumber].iB;
					                  break;
			      	 case BERNULLI : // ��������������� �������� ������� ������ � ������� ��� �������� �������� ��� ��.
					      // ���� �������� �������� ����� �������� ������� �� ������������ ���������� �������� rCe �� �������� �������.
					      // �.�. ��� ������� ��������� �������� �� ��������� ������� ����� �������� ������� ������������ ����������� 
					      // ������� PRESSUREOUTLET. �� ���� ����� ������� �������� ���� ��������� ��������������� ������� � ��������
                          // ������� � ��������� ������� ����� �������� ������� �� ������������ ����������� �������� ��� ����������� 
					      // �������� �� �������� �������. ����������� �������� ������ ������������ ��������� ���������� ������� �
					      // � ����� ��� ��������. ������ ������� ���������� � ������ �.�.�������� ������������� ��������� �������
					      // ��� �������� �� �������� ������� ��� ������� ������� ���������� ������� ����������� �������� �� �����������
					      // ������. 1997 ���. �.48-54. ������� ������� ������. ����� ���������� ���������� � ��������.
					      
					      // ����������� ����� �������� rsign:
					      // ���������� ������� �� �������� ������� ��������� �������.
	                      switch (sosedb[inumber].Norm) {
						  case ESIDE : if (potent[VX][sosedb[inumber].iB]>0.0) {
							          // �������� ������� ������ ��������� ������� ����� �������� �������.
							          rsign=-1.0;
									  breversedflow=true;
								   } else rsign=1.0;
							       break;
						  case WSIDE : if (potent[VX][sosedb[inumber].iB]<0.0) {
							          // �������� ������� ������ ��������� ������� ����� �������� �������.
							          rsign=-1.0;
									  breversedflow=true;
								   } else rsign=1.0;
							       break;
						  case NSIDE : if (potent[VY][sosedb[inumber].iB]>0.0) {
							          // �������� ������� ������ ��������� ������� ����� �������� �������.
							          rsign=-1.0;
									  breversedflow=true;
								   } else rsign=1.0;
							       break;
						  case SSIDE :if (potent[VY][sosedb[inumber].iB]<0.0) {
							          // �������� ������� ������ ��������� ������� ����� �������� �������.
							          rsign=-1.0;
									  breversedflow=true;
								   } else rsign=1.0;
							       break;
						  case TSIDE : if (potent[VZ][sosedb[inumber].iB]>0.0) {
							          // �������� ������� ������ ��������� ������� ����� �������� �������.
							          rsign=-1.0;
									  breversedflow=true;
								   } else rsign=1.0;
							       break;
						  case BSIDE : if (potent[VZ][sosedb[inumber].iB]<0.0) {
							          // �������� ������� ������ ��������� ������� ����� �������� �������.
							          rsign=-1.0;
									  breversedflow=true;
								   } else rsign=1.0;
							       break;
			              } // end switch
					      // ���������� �������� �������� �� ������� ��������� �������:
					      if (rsign>=0.0) {
                             slb[PAM][inumber].aw=1.0;
		                     slb[PAM][inumber].ai=0.0;
		                     slb[PAM][inumber].b=rCe-potent[PRESS][sosedb[inumber].iB];
		                     slb[PAM][inumber].iI=-1; // �� ������������ � �������
		                     slb[PAM][inumber].iW=sosedb[inumber].iB; 
						  }
						  else {
							  kineticenergy=0.5*prop_b[RHO][sosedb[inumber].iB-maxelm];
							  kineticenergy*=(potent[VX][sosedb[inumber].iB]*potent[VX][sosedb[inumber].iB]+
								              potent[VY][sosedb[inumber].iB]*potent[VY][sosedb[inumber].iB]+
											  potent[VZ][sosedb[inumber].iB]*potent[VZ][sosedb[inumber].iB]);
							  slb[PAM][inumber].aw=1.0;
		                      slb[PAM][inumber].ai=0.0;
		                      slb[PAM][inumber].b=rCe-kineticenergy-potent[PRESS][sosedb[inumber].iB]; // ����������� ��������
		                      slb[PAM][inumber].iI=-1; // �� ������������ � �������
		                      slb[PAM][inumber].iW=sosedb[inumber].iB; 
						  }
					      break;
				     default : // �������� �������� ������������ � ����� ����:
				          // ���������� ������ - ����������� ������������ �� ������ ��������.
				          // �� ������ �� ��� ������� ���������������� ��� �� �������.
		                  slb[PAM][inumber].aw=1.0;
		                  slb[PAM][inumber].ai=0.0;
		                  slb[PAM][inumber].b=rCe-potent[PRESS][sosedb[inumber].iB];
		                  slb[PAM][inumber].iI=-1; // �� ������������ � �������
		                  slb[PAM][inumber].iW=sosedb[inumber].iB;
					      break;
				     } // end switch

					// ��� ������� �������:
		            // ������ ������������ �������
		            // �� ����� ����.
		            slb[PAM][inumber].iW1=-1;
		            slb[PAM][inumber].iW2=-1;
		            slb[PAM][inumber].iW3=-1;
		            slb[PAM][inumber].iW4=-1;
			        //printf("pressure outlet\n"); // debug
			    
			     }	
						 

                 
			}
			else { // bFReeStyle==true
				if (!bDirichlet) {
					// ���������� ������� �������
		    doublereal dl, deltal, dS;
	        doublereal rhoi; // ��������� �� ����� ��.
		    doublereal aUPi; // ����������������� ������������ ����������� �� ��������
	        doublereal fiplus; // ���� ��������������� �����
			//doublereal FgRhieChow; // �������� ���-���

			// ���������� �������
	        switch (sosedb[inumber].Norm) {
		        case ESIDE :
			    
				    dl=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x;
					dS=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[1][sosedb[inumber].iI]-1].y; 
					dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����
					slb[PAM][inumber].ai=dbeta*alpha[VX]*prop_b[RHO][sosedb[inumber].iB-maxelm]*dS*dS/slb[VX][sosedb[inumber].iB-maxelm].aw;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].ai/=(1.0-alpha[VX]);
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					deltal=0.5*(pa[nvtx[1][sosedb[inumber].iII]-1].x+pa[nvtx[0][sosedb[inumber].iII]-1].x);
					deltal-=0.5*(pa[nvtx[1][sosedb[inumber].iI]-1].x+pa[nvtx[0][sosedb[inumber].iI]-1].x);
                    fiplus=0.5*dl/deltal;

					rhoi=(prop[RHO][sosedb[inumber].iI]*prop[RHO][sosedb[inumber].iII]);
					rhoi=rhoi/((1.0-fiplus)*prop[RHO][sosedb[inumber].iI] + fiplus*prop[RHO][sosedb[inumber].iII]);// ��������� !
					aUPi=(sl[VX][sosedb[inumber].iI].ap*sl[VX][sosedb[inumber].iII].ap);
					aUPi=aUPi/((1.0-fiplus)*sl[VX][sosedb[inumber].iI].ap + fiplus*sl[VX][sosedb[inumber].iII].ap);// ��������� !
			        
                    // ������ �����:  
					slb[PAM][inumber].b=(dbeta-1.0)*alpha[VX]*rhoi*dS*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/aUPi;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].b/=(1.0-alpha[VX]);

					/*
					*  ���� ��������� �� �������� �������� ����� ����������� ��������.
					*  ����������� �������� ����������� ��������� ���� ���������
					*  � ��������� ����. ����� ����������� �������� ��� ������� 
					*  �������������� ������ (��) ��� ���������� ��������� ����������
					*  �������� � ��������� ����� � ������ ��.
					*  ����� ��� ���� � ��������� ��. �� �������� ���������� (����� ������� �����). 
					*  ����� ���������  �� ��������������� ������� ������������� �.�. � �� �� �����������
					*  ��� ����� �� � �����, �.�. ������������� ��������� ����� ����� ��������� � �������������� 
					*  ���������� ����� ������.
					*  �����: ������� �������������� ������� � ������������ ���� �� ���������!!! � �� ������ ����.
					*  ��������� ����� �������� ����� ������.
					*/

					//slb[PAM][inumber].b+=rhoi*dS*(potent[VX][sosedb[inumber].iB]-potent[VX][sosedb[inumber].iI]);
					//printf("iII=%d, iI=%d, iB=%d, maxelm=%d",sosedb[inumber].iII,sosedb[inumber].iI,sosedb[inumber].iB,maxelm);
					//getchar(); // debug ���������� �������
					  //FgRhieChow=rFgRhieChow_internal_border(sosedb[inumber].iI, WSIDE, prop_b[RHO][sosedb[inumber].iB-maxelm], alpha[VX], nvtx, sosedi, maxelm, potent[PRESS], pa, diag_coef);
					//slb[PAM][inumber].b+=prop_b[RHO][sosedb[inumber].iB-maxelm]*dS*0.5*(potent[VX][sosedb[inumber].iB]+potent[VX][sosedb[inumber].iI]);
					  //slb[PAM][inumber].b+=prop_b[RHO][sosedb[inumber].iB-maxelm]*dS*potent[VX][sosedb[inumber].iB];
					  //slb[PAM][inumber].b+=RCh*FgRhieChow;
					  // ������ ������� ��-�������� �������� � ������������.
					   //printf("RCh=%e, Fe=%e\n",RCh*FgRhieChow,slb[PAM][inumber].b+=prop_b[RHO][sosedb[inumber].iB-maxelm]*dS*potent[VX][sosedb[inumber].iB]);
					   // getchar();
				    break;			
			
		        case NSIDE :

                    dl=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y;
					dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����
					slb[PAM][inumber].ai=dbeta*alpha[VY]*prop_b[RHO][sosedb[inumber].iB-maxelm]*dS*dS/slb[VY][sosedb[inumber].iB-maxelm].aw;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].ai/=(1.0-alpha[VY]);
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					deltal=0.5*(pa[nvtx[2][sosedb[inumber].iII]-1].y+pa[nvtx[0][sosedb[inumber].iII]-1].y);
					deltal-=0.5*(pa[nvtx[2][sosedb[inumber].iI]-1].y+pa[nvtx[0][sosedb[inumber].iI]-1].y);
                    fiplus=0.5*dl/deltal;

					rhoi=(prop[RHO][sosedb[inumber].iI]*prop[RHO][sosedb[inumber].iII]);
					rhoi=rhoi/((1.0-fiplus)*prop[RHO][sosedb[inumber].iI] + fiplus*prop[RHO][sosedb[inumber].iII]);// ��������� !
					aUPi=(sl[VY][sosedb[inumber].iI].ap*sl[VY][sosedb[inumber].iII].ap);
					aUPi=aUPi/((1.0-fiplus)*sl[VY][sosedb[inumber].iI].ap + fiplus*sl[VY][sosedb[inumber].iII].ap);// ��������� !
			        
                    // ������ �����:  
					slb[PAM][inumber].b=(dbeta-1.0)*alpha[VY]*rhoi*dS*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/aUPi;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].b/=(1.0-alpha[VY]);

				    break;

			    case TSIDE : 

                    dl=pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z;
					dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					dS*=(pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y); // ������� �����
					slb[PAM][inumber].ai=dbeta*alpha[VZ]*prop_b[RHO][sosedb[inumber].iB-maxelm]*dS*dS/slb[VZ][sosedb[inumber].iB-maxelm].aw;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].ai/=(1.0-alpha[VZ]);
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					deltal=0.5*(pa[nvtx[4][sosedb[inumber].iII]-1].z+pa[nvtx[0][sosedb[inumber].iII]-1].z);
					deltal-=0.5*(pa[nvtx[4][sosedb[inumber].iI]-1].z+pa[nvtx[0][sosedb[inumber].iI]-1].z);
                    fiplus=0.5*dl/deltal;

					rhoi=(prop[RHO][sosedb[inumber].iI]*prop[RHO][sosedb[inumber].iII]);
					rhoi=rhoi/((1.0-fiplus)*prop[RHO][sosedb[inumber].iI] + fiplus*prop[RHO][sosedb[inumber].iII]);// ��������� !
					aUPi=(sl[VZ][sosedb[inumber].iI].ap*sl[VZ][sosedb[inumber].iII].ap);
					aUPi=aUPi/((1.0-fiplus)*sl[VZ][sosedb[inumber].iI].ap + fiplus*sl[VZ][sosedb[inumber].iII].ap);// ��������� !
			        
                    // ������ �����:  
					slb[PAM][inumber].b=(dbeta-1.0)*alpha[VZ]*rhoi*dS*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/aUPi;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].b/=(1.0-alpha[VZ]);
					
				    break;

			    case WSIDE :

                    dl=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x;
					dS=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y; 
					dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����
					slb[PAM][inumber].ai=dbeta*alpha[VX]*prop_b[RHO][sosedb[inumber].iB-maxelm]*dS*dS/slb[VX][sosedb[inumber].iB-maxelm].aw;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].ai/=(1.0-alpha[VX]);
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					deltal=-0.5*(pa[nvtx[1][sosedb[inumber].iII]-1].x+pa[nvtx[0][sosedb[inumber].iII]-1].x);
					deltal+=0.5*(pa[nvtx[1][sosedb[inumber].iI]-1].x+pa[nvtx[0][sosedb[inumber].iI]-1].x);
                    fiplus=0.5*dl/deltal;

					rhoi=(prop[RHO][sosedb[inumber].iI]*prop[RHO][sosedb[inumber].iII]);
					rhoi=rhoi/((1.0-fiplus)*prop[RHO][sosedb[inumber].iI]+ fiplus*prop[RHO][sosedb[inumber].iII]);// ��������� !
					aUPi=(sl[VX][sosedb[inumber].iI].ap*sl[VX][sosedb[inumber].iII].ap);
					aUPi=aUPi/((1.0-fiplus)*sl[VX][sosedb[inumber].iI].ap + fiplus*sl[VX][sosedb[inumber].iII].ap);// ��������� !
			        
                    // ������ �����:  
					slb[PAM][inumber].b=(dbeta-1.0)*alpha[VX]*rhoi*dS*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/aUPi;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].b/=(1.0-alpha[VX]);
					
					break;

		        case SSIDE :

                    dl=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y;
					dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����
					slb[PAM][inumber].ai=dbeta*alpha[VY]*prop_b[RHO][sosedb[inumber].iB-maxelm]*dS*dS/slb[VY][sosedb[inumber].iB-maxelm].aw;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].ai/=(1.0-alpha[VY]);
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					deltal=-0.5*(pa[nvtx[2][sosedb[inumber].iII]-1].y+pa[nvtx[0][sosedb[inumber].iII]-1].y);
					deltal+=0.5*(pa[nvtx[2][sosedb[inumber].iI]-1].y+pa[nvtx[0][sosedb[inumber].iI]-1].y);
                    fiplus=0.5*dl/deltal;

					rhoi=(prop[RHO][sosedb[inumber].iI]*prop[RHO][sosedb[inumber].iII]);
					rhoi=rhoi/((1.0-fiplus)*prop[RHO][sosedb[inumber].iI] + fiplus*prop[RHO][sosedb[inumber].iII]);// ��������� !
					aUPi=(sl[VY][sosedb[inumber].iI].ap*sl[VY][sosedb[inumber].iII].ap);
					aUPi=aUPi/((1.0-fiplus)*sl[VY][sosedb[inumber].iI].ap + fiplus*sl[VY][sosedb[inumber].iII].ap);// ��������� !
			        
                    // ������ �����:  
					slb[PAM][inumber].b=(dbeta-1.0)*alpha[VY]*rhoi*dS*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/aUPi;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].b/=(1.0-alpha[VY]);
					
				   break;

		        case BSIDE : 

                    dl=pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z;
					dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					dS*=(pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y); // ������� �����
					slb[PAM][inumber].ai=dbeta*alpha[VZ]*prop_b[RHO][sosedb[inumber].iB-maxelm]*dS*dS/slb[VZ][sosedb[inumber].iB-maxelm].aw;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].ai/=(1.0-alpha[VZ]);
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					deltal=-0.5*(pa[nvtx[4][sosedb[inumber].iII]-1].z+pa[nvtx[0][sosedb[inumber].iII]-1].z);
					deltal+=0.5*(pa[nvtx[4][sosedb[inumber].iI]-1].z+pa[nvtx[0][sosedb[inumber].iI]-1].z);
                    fiplus=0.5*dl/deltal;

					rhoi=(prop[RHO][sosedb[inumber].iI]*prop[RHO][sosedb[inumber].iII]);
					rhoi=rhoi/((1.0-fiplus)*prop[RHO][sosedb[inumber].iI]+ fiplus*prop[RHO][sosedb[inumber].iII]); // ��������� !
					aUPi=(sl[VZ][sosedb[inumber].iI].ap*sl[VZ][sosedb[inumber].iII].ap);
					aUPi=aUPi/((1.0-fiplus)*sl[VZ][sosedb[inumber].iI].ap + fiplus*sl[VZ][sosedb[inumber].iII].ap); // ��������� !
			        
                    // ������ �����:  
					slb[PAM][inumber].b=(dbeta-1.0)*alpha[VZ]*rhoi*dS*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/aUPi;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].b/=(1.0-alpha[VZ]);
					
				    break;
	        } // switch

			//*/
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

			if (j<6) { slb[PAM][inumber].iW1=sosedb[inumber].iW[j++]; l++; }
			if (j<6) { slb[PAM][inumber].iW2=sosedb[inumber].iW[j++]; l++; }
			if (j<6) { slb[PAM][inumber].iW3=sosedb[inumber].iW[j++]; l++; }
			if (j<6) { slb[PAM][inumber].iW4=sosedb[inumber].iW[j++]; l++; } 

			switch (l) {
				case 0 : slb[PAM][inumber].iW1=-1;
		                 slb[PAM][inumber].iW2=-1;
		                 slb[PAM][inumber].iW3=-1;
		                 slb[PAM][inumber].iW4=-1;
		                 break;
				case 1 : slb[PAM][inumber].iW2=-1;
		                 slb[PAM][inumber].iW3=-1;
		                 slb[PAM][inumber].iW4=-1;
						 break;
				case 2 : slb[PAM][inumber].iW3=-1;
		                 slb[PAM][inumber].iW4=-1;
						 break;
				case 3 : slb[PAM][inumber].iW4=-1;
						 break;
			}

			
				}
			}

	    }
		 else if ((!bDirichlet) && !((inumber == (maxbound - 1)) && bPfix) && !((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && (w[sosedb[inumber].MCB - ls].bpressure )))
	    {  
			// �� ������� �������, �� �������� �������� � �����, �� �������� �������
			//printf("neiman!\n"); getchar(); // debug

			/*
            // �������� �������� ������������ ����� ����:
		    slb[PAM][inumber].aw=1.0;
		    slb[PAM][inumber].ai=0.0;
		    slb[PAM][inumber].b=0.0;
			slb[PAM][inumber].iI=-1;//sosedb[inumber].iI; //  ������������ � �������
		    slb[PAM][inumber].iW=sosedb[inumber].iB;
			*/

            // ����� ��������������, ��� ���� iI � iII ����������, ����� ���������
		    // ��������� ����� ������������. �� ����� 2-�� ��������� �� � ������ ����� ������.
			//printf("neiman pressure...\n"); // debug
			//getchar();

			///*
		    // ���������� ������� �������
		    doublereal dl, deltal, dS;
	        doublereal rhoi; // ��������� �� ����� ��.
		    doublereal aUPi; // ����������������� ������������ ����������� �� ��������
	        doublereal fiplus; // ���� ��������������� �����
			//doublereal FgRhieChow; // �������� ���-���

			// ���������� �������
	        switch (sosedb[inumber].Norm) {
		        case ESIDE :
			    
				    dl=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x;
					dS=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[1][sosedb[inumber].iI]-1].y; 
					dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����
					slb[PAM][inumber].ai=dbeta*alpha[VX]*prop_b[RHO][sosedb[inumber].iB-maxelm]*dS*dS/slb[VX][sosedb[inumber].iB-maxelm].aw;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].ai/=(1.0-alpha[VX]);
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					deltal=0.5*(pa[nvtx[1][sosedb[inumber].iII]-1].x+pa[nvtx[0][sosedb[inumber].iII]-1].x);
					deltal-=0.5*(pa[nvtx[1][sosedb[inumber].iI]-1].x+pa[nvtx[0][sosedb[inumber].iI]-1].x);
                    fiplus=0.5*dl/deltal;

					rhoi=(prop[RHO][sosedb[inumber].iI]*prop[RHO][sosedb[inumber].iII]);
					rhoi=rhoi/((1.0-fiplus)*prop[RHO][sosedb[inumber].iI]+ fiplus*prop[RHO][sosedb[inumber].iII]); // ��������� !
					aUPi=(sl[VX][sosedb[inumber].iI].ap*sl[VX][sosedb[inumber].iII].ap);
					aUPi=aUPi/((1.0-fiplus)*sl[VX][sosedb[inumber].iI].ap + fiplus*sl[VX][sosedb[inumber].iII].ap); // ��������� !
			        
                    // ������ �����:  
					slb[PAM][inumber].b=(dbeta-1.0)*alpha[VX]*rhoi*dS*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/aUPi;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].b/=(1.0-alpha[VX]);

					/*
					*  ���� ��������� �� �������� �������� ����� ����������� ��������.
					*  ����������� �������� ����������� ��������� ���� ���������
					*  � ��������� ����. ����� ����������� �������� ��� ������� 
					*  �������������� ������ (��) ��� ���������� ��������� ����������
					*  �������� � ��������� ����� � ������ ��.
					*  ����� ��� ���� � ��������� ��. �� �������� ���������� (����� ������� �����). 
					*  ����� ���������  �� ��������������� ������� ������������� �.�. � �� �� �����������
					*  ��� ����� �� � �����, �.�. ������������� ��������� ����� ����� ��������� � �������������� 
					*  ���������� ����� ������.
					*  �����: ������� �������������� ������� � ������������ ���� �� ���������!!! � �� ������ ����.
					*  ��������� ����� �������� ����� ������.
					*/

					//slb[PAM][inumber].b+=rhoi*dS*(potent[VX][sosedb[inumber].iB]-potent[VX][sosedb[inumber].iI]);
					//printf("iII=%d, iI=%d, iB=%d, maxelm=%d",sosedb[inumber].iII,sosedb[inumber].iI,sosedb[inumber].iB,maxelm);
					//getchar(); // debug ���������� �������
					  //FgRhieChow=rFgRhieChow_internal_border(sosedb[inumber].iI, WSIDE, prop_b[RHO][sosedb[inumber].iB-maxelm], alpha[VX], nvtx, sosedi, maxelm, potent[PRESS], pa, diag_coef);
					//slb[PAM][inumber].b+=prop_b[RHO][sosedb[inumber].iB-maxelm]*dS*0.5*(potent[VX][sosedb[inumber].iB]+potent[VX][sosedb[inumber].iI]);
					  //slb[PAM][inumber].b+=prop_b[RHO][sosedb[inumber].iB-maxelm]*dS*potent[VX][sosedb[inumber].iB];
					  //slb[PAM][inumber].b+=RCh*FgRhieChow;
					  // ������ ������� ��-�������� �������� � ������������.
					   //printf("RCh=%e, Fe=%e\n",RCh*FgRhieChow,slb[PAM][inumber].b+=prop_b[RHO][sosedb[inumber].iB-maxelm]*dS*potent[VX][sosedb[inumber].iB]);
					   // getchar();
				    break;			
			
		        case NSIDE :

                    dl=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y;
					dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����
					slb[PAM][inumber].ai=dbeta*alpha[VY]*prop_b[RHO][sosedb[inumber].iB-maxelm]*dS*dS/slb[VY][sosedb[inumber].iB-maxelm].aw;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].ai/=(1.0-alpha[VY]);
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					deltal=0.5*(pa[nvtx[2][sosedb[inumber].iII]-1].y+pa[nvtx[0][sosedb[inumber].iII]-1].y);
					deltal-=0.5*(pa[nvtx[2][sosedb[inumber].iI]-1].y+pa[nvtx[0][sosedb[inumber].iI]-1].y);
                    fiplus=0.5*dl/deltal;

					rhoi=(prop[RHO][sosedb[inumber].iI]*prop[RHO][sosedb[inumber].iII]);
					rhoi=rhoi/((1.0-fiplus)*prop[RHO][sosedb[inumber].iI]+ fiplus*prop[RHO][sosedb[inumber].iII]); // ��������� !
					aUPi=(sl[VY][sosedb[inumber].iI].ap*sl[VY][sosedb[inumber].iII].ap);
					aUPi=aUPi/((1.0-fiplus)*sl[VY][sosedb[inumber].iI].ap + fiplus*sl[VY][sosedb[inumber].iII].ap); // ��������� !
			        
                    // ������ �����:  
					slb[PAM][inumber].b=(dbeta-1.0)*alpha[VY]*rhoi*dS*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/aUPi;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].b/=(1.0-alpha[VY]);

				    break;

			    case TSIDE : 

                    dl=pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z;
					dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					dS*=(pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y); // ������� �����
					slb[PAM][inumber].ai=dbeta*alpha[VZ]*prop_b[RHO][sosedb[inumber].iB-maxelm]*dS*dS/slb[VZ][sosedb[inumber].iB-maxelm].aw;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].ai/=(1.0-alpha[VZ]);
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					deltal=0.5*(pa[nvtx[4][sosedb[inumber].iII]-1].z+pa[nvtx[0][sosedb[inumber].iII]-1].z);
					deltal-=0.5*(pa[nvtx[4][sosedb[inumber].iI]-1].z+pa[nvtx[0][sosedb[inumber].iI]-1].z);
                    fiplus=0.5*dl/deltal;

					rhoi=(prop[RHO][sosedb[inumber].iI]*prop[RHO][sosedb[inumber].iII]);
					rhoi=rhoi/((1.0-fiplus)*prop[RHO][sosedb[inumber].iI]+ fiplus*prop[RHO][sosedb[inumber].iII]); // ��������� !
					aUPi=(sl[VZ][sosedb[inumber].iI].ap*sl[VZ][sosedb[inumber].iII].ap);
					aUPi=aUPi/((1.0-fiplus)*sl[VZ][sosedb[inumber].iI].ap + fiplus*sl[VZ][sosedb[inumber].iII].ap); // ��������� !
			        
                    // ������ �����:  
					slb[PAM][inumber].b=(dbeta-1.0)*alpha[VZ]*rhoi*dS*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/aUPi;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].b/=(1.0-alpha[VZ]);
					
				    break;

			    case WSIDE :

                    dl=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x;
					dS=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y; 
					dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����
					slb[PAM][inumber].ai=dbeta*alpha[VX]*prop_b[RHO][sosedb[inumber].iB-maxelm]*dS*dS/slb[VX][sosedb[inumber].iB-maxelm].aw;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].ai/=(1.0-alpha[VX]);
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					deltal=-0.5*(pa[nvtx[1][sosedb[inumber].iII]-1].x+pa[nvtx[0][sosedb[inumber].iII]-1].x);
					deltal+=0.5*(pa[nvtx[1][sosedb[inumber].iI]-1].x+pa[nvtx[0][sosedb[inumber].iI]-1].x);
                    fiplus=0.5*dl/deltal;

					rhoi=(prop[RHO][sosedb[inumber].iI]*prop[RHO][sosedb[inumber].iII]);
					rhoi=rhoi/((1.0-fiplus)*prop[RHO][sosedb[inumber].iI] + fiplus*prop[RHO][sosedb[inumber].iII]); // ��������� !
					aUPi=(sl[VX][sosedb[inumber].iI].ap*sl[VX][sosedb[inumber].iII].ap);
					aUPi=aUPi/((1.0-fiplus)*sl[VX][sosedb[inumber].iI].ap + fiplus*sl[VX][sosedb[inumber].iII].ap); // ��������� !
			        
                    // ������ �����:  
					slb[PAM][inumber].b=(dbeta-1.0)*alpha[VX]*rhoi*dS*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/aUPi;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].b/=(1.0-alpha[VX]);
					
					break;

		        case SSIDE :

                    dl=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y;
					dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����
					slb[PAM][inumber].ai=dbeta*alpha[VY]*prop_b[RHO][sosedb[inumber].iB-maxelm]*dS*dS/slb[VY][sosedb[inumber].iB-maxelm].aw;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].ai/=(1.0-alpha[VY]);
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					deltal=-0.5*(pa[nvtx[2][sosedb[inumber].iII]-1].y+pa[nvtx[0][sosedb[inumber].iII]-1].y);
					deltal+=0.5*(pa[nvtx[2][sosedb[inumber].iI]-1].y+pa[nvtx[0][sosedb[inumber].iI]-1].y);
                    fiplus=0.5*dl/deltal;

					rhoi=(prop[RHO][sosedb[inumber].iI]*prop[RHO][sosedb[inumber].iII]);
					rhoi=rhoi/((1.0-fiplus)*prop[RHO][sosedb[inumber].iI]+ fiplus*prop[RHO][sosedb[inumber].iII]); // ��������� !
					aUPi=(sl[VY][sosedb[inumber].iI].ap*sl[VY][sosedb[inumber].iII].ap);
					aUPi=aUPi/((1.0-fiplus)*sl[VY][sosedb[inumber].iI].ap + fiplus*sl[VY][sosedb[inumber].iII].ap); // ��������� !
			        
                    // ������ �����:  
					slb[PAM][inumber].b=(dbeta-1.0)*alpha[VY]*rhoi*dS*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/aUPi;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].b/=(1.0-alpha[VY]);
					
				   break;

		        case BSIDE : 

                    dl=pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z;
					dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					dS*=(pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y); // ������� �����
					slb[PAM][inumber].ai=dbeta*alpha[VZ]*prop_b[RHO][sosedb[inumber].iB-maxelm]*dS*dS/slb[VZ][sosedb[inumber].iB-maxelm].aw;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].ai/=(1.0-alpha[VZ]);
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					deltal=-0.5*(pa[nvtx[4][sosedb[inumber].iII]-1].z+pa[nvtx[0][sosedb[inumber].iII]-1].z);
					deltal+=0.5*(pa[nvtx[4][sosedb[inumber].iI]-1].z+pa[nvtx[0][sosedb[inumber].iI]-1].z);
                    fiplus=0.5*dl/deltal;

					rhoi=(prop[RHO][sosedb[inumber].iI]*prop[RHO][sosedb[inumber].iII]);
					rhoi=rhoi/((1.0-fiplus)*prop[RHO][sosedb[inumber].iI]+ fiplus*prop[RHO][sosedb[inumber].iII]); // ��������� !
					aUPi=(sl[VZ][sosedb[inumber].iI].ap*sl[VZ][sosedb[inumber].iII].ap);
					aUPi=aUPi/((1.0-fiplus)*sl[VZ][sosedb[inumber].iI].ap + fiplus*sl[VZ][sosedb[inumber].iII].ap); // ��������� !
			        
                    // ������ �����:  
					slb[PAM][inumber].b=(dbeta-1.0)*alpha[VZ]*rhoi*dS*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/aUPi;
					if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) slb[PAM][inumber].b/=(1.0-alpha[VZ]);
					
				    break;
	        } // switch

			//*/
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

			if (j<6) { slb[PAM][inumber].iW1=sosedb[inumber].iW[j++]; l++; }
			if (j<6) { slb[PAM][inumber].iW2=sosedb[inumber].iW[j++]; l++; }
			if (j<6) { slb[PAM][inumber].iW3=sosedb[inumber].iW[j++]; l++; }
			if (j<6) { slb[PAM][inumber].iW4=sosedb[inumber].iW[j++]; l++; } 

			switch (l) {
				case 0 : slb[PAM][inumber].iW1=-1;
		                 slb[PAM][inumber].iW2=-1;
		                 slb[PAM][inumber].iW3=-1;
		                 slb[PAM][inumber].iW4=-1;
		                 break;
				case 1 : slb[PAM][inumber].iW2=-1;
		                 slb[PAM][inumber].iW3=-1;
		                 slb[PAM][inumber].iW4=-1;
						 break;
				case 2 : slb[PAM][inumber].iW3=-1;
		                 slb[PAM][inumber].iW4=-1;
						 break;
				case 3 : slb[PAM][inumber].iW4=-1;
						 break;
			}

			// ��������� ���������� � ������ ���������� ���� iI � ������������ 
			// ��������� ���� ��� ��������� ����� ������� ������� �� ���������. 
			// �.�. ���� iI ������ ���������� ��� ������� iI < maxelm, � ������������ 
			// ������� � �������� ������� ����� � �������� >= maxelm ������� ��� ��
			// ������������.
	    }
	}
	
	
} // my_elmatr_quad_PAm_bon

// ���������� ��������� �������.
// � ��������� ��� �������� ��������
// �� ������ ����������� �������������.
// ����� ���������� ���������.
// ����������� 22 ���� 2012 ����.
void my_elmatr_quad_PAm_bon2(equation3D_bon** &slb, equation3D** sl, 
							integer inumber, integer maxelm, integer maxbound, 
							BOUND* &sosedb, integer** nvtx, bool bPfix,
							doublereal dbeta, TOCHKA* pa, doublereal** potent,
							doublereal** prop, doublereal** prop_b, doublereal* alpha,
							integer ls, integer lw, WALL* w, bool bDirichlet, 
							ALICE_PARTITION** sosedi, doublereal RCh,
							bool &breversedflow, doublereal* tau) {
	
    // inumber - ����� ���������� ����, � ������� ���������.
	bool bFReeStyle=false; // ������ ����� �� �������� �������� ��������, �������� �������� ���� ==true ������ ����� ���������.
	// ���� bFReeStyle==false �� �������� �������� ����������� ���� �� �������� ������� ���� � �����.

   
	// bDirichlet==true ������ �������� ������ ������� ������� �������.

	if ( (inumber==(maxbound-1)) && bPfix && (!bFReeStyle)) {
        // ��� ����������� ������� ���� 
		// ������ ��������� ������������� 
		// ����������� �������� � ����� ����� ��������� �������.

		if (bDirichlet) {
           // �������� �������� ������������ ����� ����:
		   slb[PAM][inumber].aw=1.0;
		   slb[PAM][inumber].ai=0.0;
		   slb[PAM][inumber].b=0.0;
		   slb[PAM][inumber].iI=-1; // �� ������������ � �������
		   slb[PAM][inumber].iW=sosedb[inumber].iB; 

		   // ��� ������� �������:
		   // ������ ������������ �������
		   // �� ����� ����.
		   slb[PAM][inumber].iW1=-1;
		   slb[PAM][inumber].iW2=-1;
		   slb[PAM][inumber].iW3=-1;
		   slb[PAM][inumber].iW4=-1;
		   //printf("pressure fix\n"); getchar(); // debug
		}

	}
	 else 
	{

		if ((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && (w[sosedb[inumber].MCB - ls].bpressure )) {
            
			if (!bFReeStyle) {
				doublereal rCe=0.0; // �������� �������� �� �������� �������
			    integer ioutflowcondition=PRESSUREOUTLET; // PRESSUREOUTLET // BERNULLI �.�.�������
			    doublereal kineticenergy=0.0; // ������������ ������� ������: 0.5*rho*Vel!2.
			    doublereal rsign=1.0; // >0 �������� ���������� �� ��������� ������� ������, ���� <0 �� ������ � ������������ ����������� ��������.
			
			    if (bDirichlet) {

			    	switch (ioutflowcondition) {
				     case PRESSUREOUTLET : // �������� �������� ������������ � ����� ����:
				                      // ���������� ������ - ����������� ������������ �� ������ ��������.
				                      // �� ������ �� ��� ������� ���������������� ��� �� �������.
		                              slb[PAM][inumber].aw=1.0;
		                              slb[PAM][inumber].ai=0.0;
		                              slb[PAM][inumber].b=rCe-potent[PRESS][sosedb[inumber].iB];
		                              slb[PAM][inumber].iI=-1; // �� ������������ � �������
		                              slb[PAM][inumber].iW=sosedb[inumber].iB;
					                  break;
			      	 case BERNULLI : // ��������������� �������� ������� ������ � ������� ��� �������� �������� ��� ��.
					      // ���� �������� �������� ����� �������� ������� �� ������������ ���������� �������� rCe �� �������� �������.
					      // �.�. ��� ������� ��������� �������� �� ��������� ������� ����� �������� ������� ������������ ����������� 
					      // ������� PRESSUREOUTLET. �� ���� ����� ������� �������� ���� ��������� ��������������� ������� � ��������
                          // ������� � ��������� ������� ����� �������� ������� �� ������������ ����������� �������� ��� ����������� 
					      // �������� �� �������� �������. ����������� �������� ������ ������������ ��������� ���������� ������� �
					      // � ����� ��� ��������. ������ ������� ���������� � ������ �.�.�������� ������������� ��������� �������
					      // ��� �������� �� �������� ������� ��� ������� ������� ���������� ������� ����������� �������� �� �����������
					      // ������. 1997 ���. �.48-54. ������� ������� ������. ����� ���������� ���������� � ��������.
					      
					      // ����������� ����� �������� rsign:
					      // ���������� ������� �� �������� ������� ��������� �������.
	                      switch (sosedb[inumber].Norm) {
						  case ESIDE : if (potent[VX][sosedb[inumber].iB]>0.0) {
							          // �������� ������� ������ ��������� ������� ����� �������� �������.
							          rsign=-1.0;
									  breversedflow=true;
								   } else rsign=1.0;
							       break;
						  case WSIDE : if (potent[VX][sosedb[inumber].iB]<0.0) {
							          // �������� ������� ������ ��������� ������� ����� �������� �������.
							          rsign=-1.0;
									  breversedflow=true;
								   } else rsign=1.0;
							       break;
						  case NSIDE : if (potent[VY][sosedb[inumber].iB]>0.0) {
							          // �������� ������� ������ ��������� ������� ����� �������� �������.
							          rsign=-1.0;
									  breversedflow=true;
								   } else rsign=1.0;
							       break;
						  case SSIDE :if (potent[VY][sosedb[inumber].iB]<0.0) {
							          // �������� ������� ������ ��������� ������� ����� �������� �������.
							          rsign=-1.0;
									  breversedflow=true;
								   } else rsign=1.0;
							       break;
						  case TSIDE : if (potent[VZ][sosedb[inumber].iB]>0.0) {
							          // �������� ������� ������ ��������� ������� ����� �������� �������.
							          rsign=-1.0;
									  breversedflow=true;
								   } else rsign=1.0;
							       break;
						  case BSIDE : if (potent[VZ][sosedb[inumber].iB]<0.0) {
							          // �������� ������� ������ ��������� ������� ����� �������� �������.
							          rsign=-1.0;
									  breversedflow=true;
								   } else rsign=1.0;
							       break;
			              } // end switch
					      // ���������� �������� �������� �� ������� ��������� �������:
					      if (rsign>=0.0) {
                             slb[PAM][inumber].aw=1.0;
		                     slb[PAM][inumber].ai=0.0;
		                     slb[PAM][inumber].b=rCe-potent[PRESS][sosedb[inumber].iB];
		                     slb[PAM][inumber].iI=-1; // �� ������������ � �������
		                     slb[PAM][inumber].iW=sosedb[inumber].iB; 
						  }
						  else {
							  kineticenergy=0.5*prop_b[RHO][sosedb[inumber].iB-maxelm];
							  kineticenergy*=(potent[VX][sosedb[inumber].iB]*potent[VX][sosedb[inumber].iB]+
								              potent[VY][sosedb[inumber].iB]*potent[VY][sosedb[inumber].iB]+
											  potent[VZ][sosedb[inumber].iB]*potent[VZ][sosedb[inumber].iB]);
							  slb[PAM][inumber].aw=1.0;
		                      slb[PAM][inumber].ai=0.0;
		                      slb[PAM][inumber].b=rCe-kineticenergy-potent[PRESS][sosedb[inumber].iB]; // ����������� ��������
		                      slb[PAM][inumber].iI=-1; // �� ������������ � �������
		                      slb[PAM][inumber].iW=sosedb[inumber].iB; 
						  }
					      break;
				     default : // �������� �������� ������������ � ����� ����:
				          // ���������� ������ - ����������� ������������ �� ������ ��������.
				          // �� ������ �� ��� ������� ���������������� ��� �� �������.
		                  slb[PAM][inumber].aw=1.0;
		                  slb[PAM][inumber].ai=0.0;
		                  slb[PAM][inumber].b=rCe-potent[PRESS][sosedb[inumber].iB];
		                  slb[PAM][inumber].iI=-1; // �� ������������ � �������
		                  slb[PAM][inumber].iW=sosedb[inumber].iB;
					      break;
				     } // end switch

					// ��� ������� �������:
		            // ������ ������������ �������
		            // �� ����� ����.
		            slb[PAM][inumber].iW1=-1;
		            slb[PAM][inumber].iW2=-1;
		            slb[PAM][inumber].iW3=-1;
		            slb[PAM][inumber].iW4=-1;
			        //printf("pressure outlet\n"); // debug
			    
			     }	
						 

                 
			}
			else { // bFReeStyle==true
				if (!bDirichlet) {
					// ���������� ������� �������
		            doublereal dl, deltal, dS;
	                doublereal taui; // ����������� �� ����� ��.
	                doublereal fiplus; // ���� ��������������� �����

        			// ���������� �������
	                switch (sosedb[inumber].Norm) {
		              case ESIDE :
			    
				          dl=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x;
					      dS=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[1][sosedb[inumber].iI]-1].y; 
					      dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����

					      slb[PAM][inumber].ai=2.0*dbeta*tau[sosedb[inumber].iB]*dS/dl;
					      slb[PAM][inumber].iI=sosedb[inumber].iI;
					      slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					      slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					      deltal=0.5*(pa[nvtx[1][sosedb[inumber].iII]-1].x+pa[nvtx[0][sosedb[inumber].iII]-1].x);
					      deltal-=0.5*(pa[nvtx[1][sosedb[inumber].iI]-1].x+pa[nvtx[0][sosedb[inumber].iI]-1].x);
                          fiplus=0.5*dl/deltal;

					      taui=(tau[sosedb[inumber].iI]*tau[sosedb[inumber].iII]);
					      taui=taui/((1.0-fiplus)*tau[sosedb[inumber].iI]+fiplus*tau[sosedb[inumber].iII]);
					
			        
                          // ������ �����:  
					      slb[PAM][inumber].b=(dbeta-1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/deltal;

					      /*
					      *  ���� ��������� �� �������� �������� ����� ����������� ��������.
					      *  ����������� �������� ����������� ��������� ���� ���������
					      *  � ��������� ����. ����� ����������� �������� ��� ������� 
					      *  �������������� ������ (��) ��� ���������� ��������� ����������
					      *  �������� � ��������� ����� � ������ ��.
					      *  ����� ��� ���� � ��������� ��. �� �������� ���������� (����� ������� �����). 
					      *  ����� ���������  �� ��������������� ������� ������������� �.�. � �� �� �����������
					      *  ��� ����� �� � �����, �.�. ������������� ��������� ����� ����� ��������� � �������������� 
					      *  ���������� ����� ������.
				 	      *  �����: ������� �������������� ������� � ������������ ���� �� ���������!!! � �� ������ ����.
					      *  ��������� ����� �������� ����� ������.
					      */
				          break;			
			
		              case NSIDE :

                          dl=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y;
					      dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					      dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����

					      slb[PAM][inumber].ai=2.0*dbeta*tau[sosedb[inumber].iB]*dS/dl;
					      slb[PAM][inumber].iI=sosedb[inumber].iI;
					      slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					      slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					      deltal=0.5*(pa[nvtx[2][sosedb[inumber].iII]-1].y+pa[nvtx[0][sosedb[inumber].iII]-1].y);
					      deltal-=0.5*(pa[nvtx[2][sosedb[inumber].iI]-1].y+pa[nvtx[0][sosedb[inumber].iI]-1].y);
                          fiplus=0.5*dl/deltal;

					      taui=(tau[sosedb[inumber].iI]*tau[sosedb[inumber].iII]);
					      taui=taui/((1.0-fiplus)*tau[sosedb[inumber].iI]+ fiplus*tau[sosedb[inumber].iII]);
					
			        
                          // ������ �����:  
					      slb[PAM][inumber].b=(dbeta-1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/deltal;
					

				          break;

			          case TSIDE : 

                          dl=pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z;
					      dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					      dS*=(pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y); // ������� �����

					      slb[PAM][inumber].ai=2.0*dbeta*tau[sosedb[inumber].iB]*dS/dl;
					      slb[PAM][inumber].iI=sosedb[inumber].iI;
					      slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					      slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					      deltal=0.5*(pa[nvtx[4][sosedb[inumber].iII]-1].z+pa[nvtx[0][sosedb[inumber].iII]-1].z);
					      deltal-=0.5*(pa[nvtx[4][sosedb[inumber].iI]-1].z+pa[nvtx[0][sosedb[inumber].iI]-1].z);
                          fiplus=0.5*dl/deltal;

					      taui=(tau[sosedb[inumber].iI]*tau[sosedb[inumber].iII]);
					      taui=taui/((1.0-fiplus)*tau[sosedb[inumber].iI]+fiplus*tau[sosedb[inumber].iII]);
					
			        
                          // ������ �����:  
					      slb[PAM][inumber].b=(dbeta-1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/deltal;
					
				          break;

			          case WSIDE :

                          dl=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x;
					      dS=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y; 
					      dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����

					      slb[PAM][inumber].ai=2.0*dbeta*tau[sosedb[inumber].iB]*dS/dl;
					      slb[PAM][inumber].iI=sosedb[inumber].iI;
					      slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					      slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					      deltal=-0.5*(pa[nvtx[1][sosedb[inumber].iII]-1].x+pa[nvtx[0][sosedb[inumber].iII]-1].x);
					      deltal+=0.5*(pa[nvtx[1][sosedb[inumber].iI]-1].x+pa[nvtx[0][sosedb[inumber].iI]-1].x);
                          fiplus=0.5*dl/deltal;

					      taui=(tau[sosedb[inumber].iI]*tau[sosedb[inumber].iII]);
					      taui=taui/((1.0-fiplus)*tau[sosedb[inumber].iI]+fiplus*tau[sosedb[inumber].iII]);
					
			        
                          // ������ �����:  
					      slb[PAM][inumber].b=(dbeta-1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/deltal;
					
					      break;

		              case SSIDE :

                          dl=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y;
					      dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					      dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����

					      slb[PAM][inumber].ai=2.0*dbeta*tau[sosedb[inumber].iB]*dS/dl;
					      slb[PAM][inumber].iI=sosedb[inumber].iI;
					      slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					      slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					      deltal=-0.5*(pa[nvtx[2][sosedb[inumber].iII]-1].y+pa[nvtx[0][sosedb[inumber].iII]-1].y);
					      deltal+=0.5*(pa[nvtx[2][sosedb[inumber].iI]-1].y+pa[nvtx[0][sosedb[inumber].iI]-1].y);
                          fiplus=0.5*dl/deltal;

					      taui=(tau[sosedb[inumber].iI]*tau[sosedb[inumber].iII]);
					      taui=taui/((1.0-fiplus)*tau[sosedb[inumber].iI]+fiplus*tau[sosedb[inumber].iII]);
					
			        
                          // ������ �����:  
					      slb[PAM][inumber].b=(dbeta-1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/deltal;
					
					
				          break;

		              case BSIDE : 

                          dl=pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z;
					      dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					      dS*=(pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y); // ������� �����

					      slb[PAM][inumber].ai=2.0*dbeta*tau[sosedb[inumber].iB]*dS/dl;
					      slb[PAM][inumber].iI=sosedb[inumber].iI;
					      slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					      slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					      deltal=-0.5*(pa[nvtx[4][sosedb[inumber].iII]-1].z+pa[nvtx[0][sosedb[inumber].iII]-1].z);
					      deltal+=0.5*(pa[nvtx[4][sosedb[inumber].iI]-1].z+pa[nvtx[0][sosedb[inumber].iI]-1].z);
                          fiplus=0.5*dl/deltal;

					      taui=(tau[sosedb[inumber].iI]*tau[sosedb[inumber].iII]);
					      taui=taui/((1.0-fiplus)*tau[sosedb[inumber].iI]+fiplus*tau[sosedb[inumber].iII]);
					      
			        
                          // ������ �����:  
					      slb[PAM][inumber].b=(dbeta-1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/deltal;
					      
					
				          break;
	                } // switch

			        //*/
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

			        if (j<6) { slb[PAM][inumber].iW1=sosedb[inumber].iW[j++]; l++; }
			        if (j<6) { slb[PAM][inumber].iW2=sosedb[inumber].iW[j++]; l++; }
			        if (j<6) { slb[PAM][inumber].iW3=sosedb[inumber].iW[j++]; l++; }
			        if (j<6) { slb[PAM][inumber].iW4=sosedb[inumber].iW[j++]; l++; } 

			        switch (l) {
				       case 0 : slb[PAM][inumber].iW1=-1;
		                        slb[PAM][inumber].iW2=-1;
		                        slb[PAM][inumber].iW3=-1;
		                        slb[PAM][inumber].iW4=-1;
		                        break;
				       case 1 : slb[PAM][inumber].iW2=-1;
		                        slb[PAM][inumber].iW3=-1;
		                        slb[PAM][inumber].iW4=-1;
						        break;
				       case 2 : slb[PAM][inumber].iW3=-1;
		                        slb[PAM][inumber].iW4=-1;
						        break;
				       case 3 : slb[PAM][inumber].iW4=-1;
						        break;
			        }

			
				}
			}

	    }
	     else if ((!bDirichlet) && !((inumber==(maxbound-1)) && bPfix) && !((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure) )
	    {  
			// �� ������� �������, �� �������� �������� � �����, �� �������� �������
			//printf("neiman!\n"); getchar(); // debug


			/*
            // �������� �������� ������������ ����� ����:
		    slb[PAM][inumber].aw=1.0;
		    slb[PAM][inumber].ai=0.0;
		    slb[PAM][inumber].b=0.0;
			slb[PAM][inumber].iI=-1;//sosedb[inumber].iI; //  ������������ � �������
		    slb[PAM][inumber].iW=sosedb[inumber].iB;
			*/

            // ����� ��������������, ��� ���� iI � iII ����������, ����� ���������
		    // ��������� ����� ������������. �� ����� 2-�� ��������� �� � ������ ����� ������.
			//printf("neiman pressure...\n"); // debug
			//getchar();

			///*
		    // ���������� ������� �������
		    doublereal dl, deltal, dS;
	        doublereal taui; // ����������� �� ����� ��.
	        doublereal fiplus; // ���� ��������������� �����
			//doublereal FgRhieChow; // �������� ���-���

			// ���������� �������
	        switch (sosedb[inumber].Norm) {
		        case ESIDE :
			    
				    dl=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x;
					dS=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[1][sosedb[inumber].iI]-1].y; 
					dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����

					slb[PAM][inumber].ai=2.0*dbeta*tau[sosedb[inumber].iB]*dS/dl;
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					deltal=0.5*(pa[nvtx[1][sosedb[inumber].iII]-1].x+pa[nvtx[0][sosedb[inumber].iII]-1].x);
					deltal-=0.5*(pa[nvtx[1][sosedb[inumber].iI]-1].x+pa[nvtx[0][sosedb[inumber].iI]-1].x);
                    fiplus=0.5*dl/deltal;

					taui=(tau[sosedb[inumber].iI]*tau[sosedb[inumber].iII]);
					taui=taui/((1.0-fiplus)*tau[sosedb[inumber].iI]+fiplus*tau[sosedb[inumber].iII]);
					
			        
                    // ������ �����:  
					slb[PAM][inumber].b=(dbeta-1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/deltal;

					/*
					*  ���� ��������� �� �������� �������� ����� ����������� ��������.
					*  ����������� �������� ����������� ��������� ���� ���������
					*  � ��������� ����. ����� ����������� �������� ��� ������� 
					*  �������������� ������ (��) ��� ���������� ��������� ����������
					*  �������� � ��������� ����� � ������ ��.
					*  ����� ��� ���� � ��������� ��. �� �������� ���������� (����� ������� �����). 
					*  ����� ���������  �� ��������������� ������� ������������� �.�. � �� �� �����������
					*  ��� ����� �� � �����, �.�. ������������� ��������� ����� ����� ��������� � �������������� 
					*  ���������� ����� ������.
					*  �����: ������� �������������� ������� � ������������ ���� �� ���������!!! � �� ������ ����.
					*  ��������� ����� �������� ����� ������.
					*/
				    break;			
			
		        case NSIDE :

                    dl=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y;
					dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����

					slb[PAM][inumber].ai=2.0*dbeta*tau[sosedb[inumber].iB]*dS/dl;
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					deltal=0.5*(pa[nvtx[2][sosedb[inumber].iII]-1].y+pa[nvtx[0][sosedb[inumber].iII]-1].y);
					deltal-=0.5*(pa[nvtx[2][sosedb[inumber].iI]-1].y+pa[nvtx[0][sosedb[inumber].iI]-1].y);
                    fiplus=0.5*dl/deltal;

					taui=(tau[sosedb[inumber].iI]*tau[sosedb[inumber].iII]);
					taui=taui/((1.0-fiplus)*tau[sosedb[inumber].iI]+fiplus*tau[sosedb[inumber].iII]);
					
			        
                    // ������ �����:  
					slb[PAM][inumber].b=(dbeta-1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/deltal;

				    break;

			    case TSIDE : 

                    dl=pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z;
					dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					dS*=(pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y); // ������� �����

					slb[PAM][inumber].ai=2.0*dbeta*tau[sosedb[inumber].iB]*dS/dl;
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					deltal=0.5*(pa[nvtx[4][sosedb[inumber].iII]-1].z+pa[nvtx[0][sosedb[inumber].iII]-1].z);
					deltal-=0.5*(pa[nvtx[4][sosedb[inumber].iI]-1].z+pa[nvtx[0][sosedb[inumber].iI]-1].z);
                    fiplus=0.5*dl/deltal;

					taui=(tau[sosedb[inumber].iI]*tau[sosedb[inumber].iII]);
					taui=taui/((1.0-fiplus)*tau[sosedb[inumber].iI]+fiplus*tau[sosedb[inumber].iII]);
								        
                    // ������ �����:  
					slb[PAM][inumber].b=(dbeta-1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/deltal;
					
				    break;

			    case WSIDE :

                    dl=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x;
					dS=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y; 
					dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����

					slb[PAM][inumber].ai=2.0*dbeta*tau[sosedb[inumber].iB]*dS/dl;
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					deltal=-0.5*(pa[nvtx[1][sosedb[inumber].iII]-1].x+pa[nvtx[0][sosedb[inumber].iII]-1].x);
					deltal+=0.5*(pa[nvtx[1][sosedb[inumber].iI]-1].x+pa[nvtx[0][sosedb[inumber].iI]-1].x);
                    fiplus=0.5*dl/deltal;

					taui=(tau[sosedb[inumber].iI]*tau[sosedb[inumber].iII]);
					taui=taui/((1.0-fiplus)*tau[sosedb[inumber].iI]+fiplus*tau[sosedb[inumber].iII]);
					
			        
                    // ������ �����:  
					slb[PAM][inumber].b=(dbeta-1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/deltal;
					
					break;

		        case SSIDE :

                    dl=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y;
					dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����

					slb[PAM][inumber].ai=2.0*dbeta*tau[sosedb[inumber].iB]*dS/dl;
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					deltal=-0.5*(pa[nvtx[2][sosedb[inumber].iII]-1].y+pa[nvtx[0][sosedb[inumber].iII]-1].y);
					deltal+=0.5*(pa[nvtx[2][sosedb[inumber].iI]-1].y+pa[nvtx[0][sosedb[inumber].iI]-1].y);
                    fiplus=0.5*dl/deltal;

					taui=(tau[sosedb[inumber].iI]*tau[sosedb[inumber].iII]);
					taui=taui/((1.0-fiplus)*tau[sosedb[inumber].iI]+fiplus*tau[sosedb[inumber].iII]);
					
			        
                    // ������ �����:  
					slb[PAM][inumber].b=(dbeta-1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/deltal;
					
				   break;

		        case BSIDE : 

                    dl=pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z;
					dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					dS*=(pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y); // ������� �����

					slb[PAM][inumber].ai=2.0*dbeta*tau[sosedb[inumber].iB]*dS/dl;
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					deltal=-0.5*(pa[nvtx[4][sosedb[inumber].iII]-1].z+pa[nvtx[0][sosedb[inumber].iII]-1].z);
					deltal+=0.5*(pa[nvtx[4][sosedb[inumber].iI]-1].z+pa[nvtx[0][sosedb[inumber].iI]-1].z);
                    fiplus=0.5*dl/deltal;

					taui=(tau[sosedb[inumber].iI]*tau[sosedb[inumber].iII]);
					taui=taui/((1.0-fiplus)*tau[sosedb[inumber].iI]+fiplus*tau[sosedb[inumber].iII]);
					
			        
                    // ������ �����:  
					slb[PAM][inumber].b=(dbeta-1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI]-potent[PAM][sosedb[inumber].iII])/deltal;
					
				    break;
	        } // switch

			//*/
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

			if (j<6) { slb[PAM][inumber].iW1=sosedb[inumber].iW[j++]; l++; }
			if (j<6) { slb[PAM][inumber].iW2=sosedb[inumber].iW[j++]; l++; }
			if (j<6) { slb[PAM][inumber].iW3=sosedb[inumber].iW[j++]; l++; }
			if (j<6) { slb[PAM][inumber].iW4=sosedb[inumber].iW[j++]; l++; } 

			switch (l) {
				case 0 : slb[PAM][inumber].iW1=-1;
		                 slb[PAM][inumber].iW2=-1;
		                 slb[PAM][inumber].iW3=-1;
		                 slb[PAM][inumber].iW4=-1;
		                 break;
				case 1 : slb[PAM][inumber].iW2=-1;
		                 slb[PAM][inumber].iW3=-1;
		                 slb[PAM][inumber].iW4=-1;
						 break;
				case 2 : slb[PAM][inumber].iW3=-1;
		                 slb[PAM][inumber].iW4=-1;
						 break;
				case 3 : slb[PAM][inumber].iW4=-1;
						 break;
			}

			// ��������� ���������� � ������ ���������� ���� iI � ������������ 
			// ��������� ���� ��� ��������� ����� ������� ������� �� ���������. 
			// �.�. ���� iI ������ ���������� ��� ������� iI < maxelm, � ������������ 
			// ������� � �������� ������� ����� � �������� >= maxelm ������� ��� ��
			// ������������.
	    }
	}
	
	
} // my_elmatr_quad_PAm_bon2

// ���������� ��������� �������.
// � ��������� ��� �������� ��������
// �� ������ ����������� �������������.
// ����� ���������� ���������.
// ����������� 22 ���� 2012 ����.
// ���������� ������� opening �� ����������� �����������. 27.07.2016.
void my_elmatr_quad_PAm_bon3(equation3D_bon** &slb, equation3D** sl, 
							integer inumber, integer maxelm, integer maxbound, 
							BOUND* &sosedb, integer** nvtx, bool bPfix,
							doublereal dbeta, TOCHKA* pa, doublereal** potent,
							doublereal** prop, doublereal** prop_b, doublereal* alpha,
							integer ls, integer lw, WALL* w, bool bDirichlet, 
							ALICE_PARTITION** sosedi, doublereal RCh,
							bool &breversedflow, doublereal** tau) {
	
	bool bcheck = false;

    // inumber - ����� ���������� ����, � ������� ���������.
	bool bFReeStyle=false; // ������ ����� �� �������� �������� ��������, �������� �������� ���� ==true ������ ����� ���������.
	// ���� bFReeStyle==false �� �������� �������� ����������� ���� �� �������� ������� ���� � �����.
	///bFReeStyle = true; // 5.05.2017 ��� �� �������� ������.

	bool b_prosto = true;
	// bDirichlet==true ������ �������� ������ ������� ������� �������.

	/*
	if ( (inumber==(maxbound-1)) && bPfix && (!bFReeStyle)) {
        // ��� ����������� ������� ���� 
		// ������ ��������� ������������� 
		// ����������� �������� � ����� ����� ��������� �������.
		/*
		printf("pressure fix Ok...");
		getchar();
		*//*

		if (bDirichlet) {
           // �������� �������� ������������ ����� ����:
		   slb[PAM][inumber].aw=1.0;
		   slb[PAM][inumber].ai=0.0;
		   slb[PAM][inumber].b=0.0;
		   slb[PAM][inumber].iI=-1; // �� ������������ � �������
		   slb[PAM][inumber].iW=sosedb[inumber].iB; 

		   // ��� ������� �������:
		   // ������ ������������ �������
		   // �� ����� ����.
		   slb[PAM][inumber].iW1=-1;
		   slb[PAM][inumber].iW2=-1;
		   slb[PAM][inumber].iW3=-1;
		   slb[PAM][inumber].iW4=-1;
		   //printf("pressure fix\n"); getchar(); // debug
		}

	}
	 else 
	{*/

	if ((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB<(ls + lw)) && (w[sosedb[inumber].MCB - ls].bpressure || w[sosedb[inumber].MCB - ls].bopening)) {
            
			if (!bFReeStyle) {
				


				doublereal rCe=w[sosedb[inumber].MCB-ls].P; // �������� �������� �� �������� �������
				//printf("rCe=%e %d \n",rCe,sosedb[inumber].MCB-ls);
				//getchar();

				integer ioutflowcondition =  BERNULLI; // PRESSUREOUTLET // BERNULLI �.�.�������
			    doublereal kineticenergy=0.0; // ������������ ������� ������: 0.5*rho*Vel!2.
			    doublereal rsign=1.0; // >0 �������� ���������� �� ��������� ������� ������, ���� <0 �� ������ � ������������ ����������� ��������.
			
			    if (bDirichlet) {

					bcheck = true;

			    	switch (ioutflowcondition) {
				     case PRESSUREOUTLET : // �������� �������� ������������ � ����� ����:
				                      // ���������� ������ - ����������� ������������ �� ������ ��������.
				                      // �� ������ �� ��� ������� ���������������� ��� �� �������.
		                              slb[PAM][inumber].aw=1.0;
		                              slb[PAM][inumber].ai=0.0;
		                              slb[PAM][inumber].b=rCe-potent[PRESS][sosedb[inumber].iB];
		                              slb[PAM][inumber].iI=-1; // �� ������������ � �������
		                              slb[PAM][inumber].iW=sosedb[inumber].iB;
									 // printf("%d\n",slb[PAM][inumber].iW); getchar();// ������ ���� ����� maxelm
					                  break;
			      	 case BERNULLI : // ��������������� �������� ������� ������ � ������� ��� �������� �������� ��� ��.
					      // ���� �������� �������� ����� �������� ������� �� ������������ ���������� �������� rCe �� �������� �������.
					      // �.�. ��� ������� ��������� �������� �� ��������� ������� ����� �������� ������� ������������ ����������� 
					      // ������� PRESSUREOUTLET. �� ���� ����� ������� �������� ���� ��������� ��������������� ������� � ��������
                          // ������� � ��������� ������� ����� �������� ������� �� ������������ ����������� �������� ��� ����������� 
					      // �������� �� �������� �������. ����������� �������� ������ ������������ ��������� ���������� ������� �
					      // � ����� ��� ��������. ������ ������� ���������� � ������ �.�.�������� ������������� ��������� �������
					      // ��� �������� �� �������� ������� ��� ������� ������� ���������� ������� ����������� �������� �� �����������
					      // ������. 1997 ���. �.48-54. ������� ������� ������. ����� ���������� ���������� � ��������.
					      
					      // ����������� ����� �������� rsign:
					      // ���������� ������� �� �������� ������� ��������� �������.
	                      switch (sosedb[inumber].Norm) {
						  case ESIDE : if (potent[VX][sosedb[inumber].iB]>0.0) {
							          // �������� ������� ������ ��������� ������� ����� �������� �������.
							          rsign=-1.0;
									  breversedflow=true;
								   } else rsign=1.0;
							       break;
						  case WSIDE : if (potent[VX][sosedb[inumber].iB]<0.0) {
							          // �������� ������� ������ ��������� ������� ����� �������� �������.
							          rsign=-1.0;
									  breversedflow=true;
								   } else rsign=1.0;
							       break;
						  case NSIDE : if (potent[VY][sosedb[inumber].iB]>0.0) {
							          // �������� ������� ������ ��������� ������� ����� �������� �������.
							          rsign=-1.0;
									  breversedflow=true;
								   } else rsign=1.0;
							       break;
						  case SSIDE :if (potent[VY][sosedb[inumber].iB]<0.0) {
							          // �������� ������� ������ ��������� ������� ����� �������� �������.
							          rsign=-1.0;
									  breversedflow=true;
								   } else rsign=1.0;
							       break;
						  case TSIDE : if (potent[VZ][sosedb[inumber].iB]>0.0) {
							          // �������� ������� ������ ��������� ������� ����� �������� �������.
							          rsign=-1.0;
									  breversedflow=true;
								   } else rsign=1.0;
							       break;
						  case BSIDE : if (potent[VZ][sosedb[inumber].iB]<0.0) {
							          // �������� ������� ������ ��������� ������� ����� �������� �������.
							          rsign=-1.0;
									  breversedflow=true;
								   } else rsign=1.0;
							       break;
			              } // end switch
					      // ���������� �������� �������� �� ������� ��������� �������:
					      if (rsign>=0.0) {
							  // �������� �������� ��������� �������.

                             slb[PAM][inumber].aw=1.0;
		                     slb[PAM][inumber].ai=0.0;
		                     slb[PAM][inumber].b=rCe-potent[PRESS][sosedb[inumber].iB];
		                     slb[PAM][inumber].iI=-1; // �� ������������ � �������
		                     slb[PAM][inumber].iW=sosedb[inumber].iB; 
						  }
						  else {
							  // �������� ��������� ������ ��������� �������.

							  kineticenergy=0.5*prop_b[RHO][sosedb[inumber].iB-maxelm];
							  // VComponentCOR ������ VComponent �� ��������.
							  kineticenergy*=(potent[VXCOR][sosedb[inumber].iB]*potent[VXCOR][sosedb[inumber].iB]+
								              potent[VYCOR][sosedb[inumber].iB]*potent[VYCOR][sosedb[inumber].iB]+
											  potent[VZCOR][sosedb[inumber].iB]*potent[VZCOR][sosedb[inumber].iB]);
							  slb[PAM][inumber].aw=1.0;
		                      slb[PAM][inumber].ai=0.0;
		                      slb[PAM][inumber].b=rCe-kineticenergy-potent[PRESS][sosedb[inumber].iB]; // ����������� ��������
		                      slb[PAM][inumber].iI=-1; // �� ������������ � �������
		                      slb[PAM][inumber].iW=sosedb[inumber].iB; 
						  }
					      break;
				     default : // �������� �������� ������������ � ����� ����:
				          // ���������� ������ - ����������� ������������ �� ������ ��������.
				          // �� ������ �� ��� ������� ���������������� ��� �� �������.
		                  slb[PAM][inumber].aw=1.0;
		                  slb[PAM][inumber].ai=0.0;
		                  slb[PAM][inumber].b=rCe-potent[PRESS][sosedb[inumber].iB];
		                  slb[PAM][inumber].iI=-1; // �� ������������ � �������
		                  slb[PAM][inumber].iW=sosedb[inumber].iB;
					      break;
				     } // end switch

					// ��� ������� �������:
		            // ������ ������������ �������
		            // �� ����� ����.
		            slb[PAM][inumber].iW1=-1;
		            slb[PAM][inumber].iW2=-1;
		            slb[PAM][inumber].iW3=-1;
		            slb[PAM][inumber].iW4=-1;
			        //printf("pressure outlet\n"); // debug
			    
			     }	
				 else if (1) {
					 // !bDirihlet (not bDirichlet)
					 if (w[sosedb[inumber].MCB - ls].bopening) {
						 // ��������!!! ������ � ������ opening ������� ��������� �������.

						 // 5.05.2017. ����������� �������� ���������� ==1.
						 // ��������� ������� ������� ���� ����������� �������� ���������� �� 
						 // ������� ���������.
						 // ���������� ������� ������� ��� �������� �������� � ������� �������,
						 // ����� ������� ������������� (������) �������� ��������� �������.
						 // �������� ����������� � ������ Opening �������.
						 switch (ioutflowcondition) {
						 case BERNULLI:
							 // ����������� ����� �������� rsign:
							 // ���������� ������� �� �������� ������� ��������� �������.
							 switch (sosedb[inumber].Norm) {
							 case ESIDE:  if (potent[VX][sosedb[inumber].iB] > 0.0) {
								        // �������� ������� ������ ��������� ������� ����� �������� �������.
								        rsign = -1.0;
										// �������� �� ����������� ������� �� � �������������.
								        // breversedflow = true;
							         }
									 else rsign = 1.0;
									 break;
							 case WSIDE: if (potent[VX][sosedb[inumber].iB] < 0.0) {
								          // �������� ������� ������ ��������� ������� ����� �������� �������.
								          rsign = -1.0;
										  // �������� �� ����������� ������� �� � �������������.
								          //breversedflow = true;
							         }
									 else rsign = 1.0;
									 break;
							 case NSIDE: if (potent[VY][sosedb[inumber].iB] > 0.0) {
								             // �������� ������� ������ ��������� ������� ����� �������� �������.
								             rsign = -1.0;
											 // �������� �� ����������� ������� �� � �������������.
								             //breversedflow = true;
							             }
										 else rsign = 1.0;
										 break;
							 case SSIDE:if (potent[VY][sosedb[inumber].iB] < 0.0) {
								            // �������� ������� ������ ��������� ������� ����� �������� �������.
								            rsign = -1.0;
											// �������� �� ����������� ������� �� � �������������.
								            //breversedflow = true;
							            }
										else rsign = 1.0;
										break;
							 case TSIDE: if (potent[VZ][sosedb[inumber].iB] > 0.0) {
								             // �������� ������� ������ ��������� ������� ����� �������� �������.
								             rsign = -1.0;
											 // �������� �� ����������� ������� �� � �������������.				             
											 //breversedflow = true;
							             }
										 else rsign = 1.0;
										 break;
							 case BSIDE: if (potent[VZ][sosedb[inumber].iB] < 0.0) {
								             // �������� ������� ������ ��������� ������� ����� �������� �������.
								             rsign = -1.0;
											 // �������� �� ����������� ������� �� � �������������.
											 //breversedflow = true;
							             }
										 else rsign = 1.0;
										 break;
							 } // end switch
							   // ���������� �������� �������� �� ������� ��������� �������:
							 if (rsign >= 0.0) {
								 // �������� �������� ��������� �������.
								 // ���������� ������� ������� ��� �������� �������� 
								 // ��� ��������� ������� �� ��������� �������.
								 slb[PAM][inumber].aw = 1.0;
								 slb[PAM][inumber].ai = 1.0;
								 slb[PAM][inumber].b = 0.0;
								 slb[PAM][inumber].iI = sosedb[inumber].iI;
								 slb[PAM][inumber].iW = sosedb[inumber].iB;
							 }
							 break;
						 }
					 }
				 }
						 

                 
			}
			else { // bFReeStyle==true
				if (!bDirichlet) {
					// ���������� ������� �������
		            doublereal dl, deltal, dS;
	                doublereal taui; // ����������� �� ����� ��.
	                doublereal fiplus; // ���� ��������������� �����

					bcheck = true;

        			// ���������� �������
	                switch (sosedb[inumber].Norm) {
		              case ESIDE :
			    
				          dl=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x;
					      dS=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[1][sosedb[inumber].iI]-1].y; 
					      dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����

					      slb[PAM][inumber].ai=2.0*dbeta*tau[VX][sosedb[inumber].iB]*dS/dl;
					      slb[PAM][inumber].iI=sosedb[inumber].iI;
					      slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					      slb[PAM][inumber].iW=sosedb[inumber].iB;
						  if (!b_prosto) {


							  deltal = 0.5*(pa[nvtx[1][sosedb[inumber].iII] - 1].x + pa[nvtx[0][sosedb[inumber].iII] - 1].x);
							  deltal -= 0.5*(pa[nvtx[1][sosedb[inumber].iI] - 1].x + pa[nvtx[0][sosedb[inumber].iI] - 1].x);
							  fiplus = 0.5*dl / deltal;

							  taui = (tau[VX][sosedb[inumber].iI] * tau[VX][sosedb[inumber].iII]);
							  taui = taui / ((1.0 - fiplus)*tau[VX][sosedb[inumber].iI] + fiplus*tau[VX][sosedb[inumber].iII]); // ��������� !


							  // ������ �����:  
							  slb[PAM][inumber].b = (dbeta - 1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI] - potent[PAM][sosedb[inumber].iII]) / deltal;
						  }
						  else slb[PAM][inumber].b = 0.0;
					      /*
					      *  ���� ��������� �� �������� �������� ����� ����������� ��������.
					      *  ����������� �������� ����������� ��������� ���� ���������
					      *  � ��������� ����. ����� ����������� �������� ��� ������� 
					      *  �������������� ������ (��) ��� ���������� ��������� ����������
					      *  �������� � ��������� ����� � ������ ��.
					      *  ����� ��� ���� � ��������� ��. �� �������� ���������� (����� ������� �����). 
					      *  ����� ���������  �� ��������������� ������� ������������� �.�. � �� �� �����������
					      *  ��� ����� �� � �����, �.�. ������������� ��������� ����� ����� ��������� � �������������� 
					      *  ���������� ����� ������.
				 	      *  �����: ������� �������������� ������� � ������������ ���� �� ���������!!! � �� ������ ����.
					      *  ��������� ����� �������� ����� ������.
					      */
				          break;			
			
		              case NSIDE :

                          dl=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y;
					      dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					      dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����

					      slb[PAM][inumber].ai=2.0*dbeta*tau[VY][sosedb[inumber].iB]*dS/dl;
					      slb[PAM][inumber].iI=sosedb[inumber].iI;
					      slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					      slb[PAM][inumber].iW=sosedb[inumber].iB;
						  if (!b_prosto) {

							  deltal = 0.5*(pa[nvtx[2][sosedb[inumber].iII] - 1].y + pa[nvtx[0][sosedb[inumber].iII] - 1].y);
							  deltal -= 0.5*(pa[nvtx[2][sosedb[inumber].iI] - 1].y + pa[nvtx[0][sosedb[inumber].iI] - 1].y);
							  fiplus = 0.5*dl / deltal;

							  taui = (tau[VY][sosedb[inumber].iI] * tau[VY][sosedb[inumber].iII]);
							  taui = taui / ((1.0 - fiplus)*tau[VY][sosedb[inumber].iI] + fiplus*tau[VY][sosedb[inumber].iII]); // ��������� !


							  // ������ �����:  
							  slb[PAM][inumber].b = (dbeta - 1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI] - potent[PAM][sosedb[inumber].iII]) / deltal;
						  }
						  else slb[PAM][inumber].b = 0.0;

				          break;

			          case TSIDE : 

                          dl=pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z;
					      dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					      dS*=(pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y); // ������� �����

					      slb[PAM][inumber].ai=2.0*dbeta*tau[VZ][sosedb[inumber].iB]*dS/dl;
					      slb[PAM][inumber].iI=sosedb[inumber].iI;
					      slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					      slb[PAM][inumber].iW=sosedb[inumber].iB;
						  if (!b_prosto) {

							  deltal = 0.5*(pa[nvtx[4][sosedb[inumber].iII] - 1].z + pa[nvtx[0][sosedb[inumber].iII] - 1].z);
							  deltal -= 0.5*(pa[nvtx[4][sosedb[inumber].iI] - 1].z + pa[nvtx[0][sosedb[inumber].iI] - 1].z);
							  fiplus = 0.5*dl / deltal;

							  taui = (tau[VZ][sosedb[inumber].iI] * tau[VZ][sosedb[inumber].iII]);
							  taui = taui / ((1.0 - fiplus)*tau[VZ][sosedb[inumber].iI] + fiplus*tau[VZ][sosedb[inumber].iII]);// ��������� !


							  // ������ �����:  
							  slb[PAM][inumber].b = (dbeta - 1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI] - potent[PAM][sosedb[inumber].iII]) / deltal;
						  }
						  else slb[PAM][inumber].b = 0.0;

				          break;

			          case WSIDE :

                          dl=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x;
					      dS=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y; 
					      dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����

					      slb[PAM][inumber].ai=2.0*dbeta*tau[VX][sosedb[inumber].iB]*dS/dl;
					      slb[PAM][inumber].iI=sosedb[inumber].iI;
					      slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					      slb[PAM][inumber].iW=sosedb[inumber].iB;
					
						  if (!b_prosto) {

							  deltal = -0.5*(pa[nvtx[1][sosedb[inumber].iII] - 1].x + pa[nvtx[0][sosedb[inumber].iII] - 1].x);
							  deltal += 0.5*(pa[nvtx[1][sosedb[inumber].iI] - 1].x + pa[nvtx[0][sosedb[inumber].iI] - 1].x);
							  fiplus = 0.5*dl / deltal;

							  taui = (tau[VX][sosedb[inumber].iI] * tau[VX][sosedb[inumber].iII]);
							  taui = taui / ((1.0 - fiplus)*tau[VX][sosedb[inumber].iI] + fiplus*tau[VX][sosedb[inumber].iII]);// ��������� !


							  // ������ �����:  
							  slb[PAM][inumber].b = (dbeta - 1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI] - potent[PAM][sosedb[inumber].iII]) / deltal;
						  }
						  else slb[PAM][inumber].b = 0.0;

					      break;

		              case SSIDE :

                          dl=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y;
					      dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					      dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����

					      slb[PAM][inumber].ai=2.0*dbeta*tau[VY][sosedb[inumber].iB]*dS/dl;
					      slb[PAM][inumber].iI=sosedb[inumber].iI;
					      slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					      slb[PAM][inumber].iW=sosedb[inumber].iB;
						  if (!b_prosto) {

							  deltal = -0.5*(pa[nvtx[2][sosedb[inumber].iII] - 1].y + pa[nvtx[0][sosedb[inumber].iII] - 1].y);
							  deltal += 0.5*(pa[nvtx[2][sosedb[inumber].iI] - 1].y + pa[nvtx[0][sosedb[inumber].iI] - 1].y);
							  fiplus = 0.5*dl / deltal;

							  taui = (tau[VY][sosedb[inumber].iI] * tau[VY][sosedb[inumber].iII]);
							  taui = taui / ((1.0 - fiplus)*tau[VY][sosedb[inumber].iI] + fiplus*tau[VY][sosedb[inumber].iII]);// ��������� !


							  // ������ �����:  
							  slb[PAM][inumber].b = (dbeta - 1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI] - potent[PAM][sosedb[inumber].iII]) / deltal;
						  }
						  else slb[PAM][inumber].b = 0.0;

				          break;

		              case BSIDE : 

                          dl=pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z;
					      dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					      dS*=(pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y); // ������� �����

					      slb[PAM][inumber].ai=2.0*dbeta*tau[VZ][sosedb[inumber].iB]*dS/dl;
					      slb[PAM][inumber].iI=sosedb[inumber].iI;
					      slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					      slb[PAM][inumber].iW=sosedb[inumber].iB;
					
						  if (!b_prosto) {

							  deltal = -0.5*(pa[nvtx[4][sosedb[inumber].iII] - 1].z + pa[nvtx[0][sosedb[inumber].iII] - 1].z);
							  deltal += 0.5*(pa[nvtx[4][sosedb[inumber].iI] - 1].z + pa[nvtx[0][sosedb[inumber].iI] - 1].z);
							  fiplus = 0.5*dl / deltal;

							  taui = (tau[VZ][sosedb[inumber].iI] * tau[VZ][sosedb[inumber].iII]);
							  taui = taui / ((1.0 - fiplus)*tau[VZ][sosedb[inumber].iI] + fiplus*tau[VZ][sosedb[inumber].iII]);// ��������� !


							  // ������ �����:  
							  slb[PAM][inumber].b = (dbeta - 1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI] - potent[PAM][sosedb[inumber].iII]) / deltal;

						  }
						  else slb[PAM][inumber].b = 0.0;

				          break;
	                } // switch

			        //*/
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

			        if (j<6) { slb[PAM][inumber].iW1=sosedb[inumber].iW[j++]; l++; }
			        if (j<6) { slb[PAM][inumber].iW2=sosedb[inumber].iW[j++]; l++; }
			        if (j<6) { slb[PAM][inumber].iW3=sosedb[inumber].iW[j++]; l++; }
			        if (j<6) { slb[PAM][inumber].iW4=sosedb[inumber].iW[j++]; l++; } 

			        switch (l) {
				       case 0 : slb[PAM][inumber].iW1=-1;
		                        slb[PAM][inumber].iW2=-1;
		                        slb[PAM][inumber].iW3=-1;
		                        slb[PAM][inumber].iW4=-1;
		                        break;
				       case 1 : slb[PAM][inumber].iW2=-1;
		                        slb[PAM][inumber].iW3=-1;
		                        slb[PAM][inumber].iW4=-1;
						        break;
				       case 2 : slb[PAM][inumber].iW3=-1;
		                        slb[PAM][inumber].iW4=-1;
						        break;
				       case 3 : slb[PAM][inumber].iW4=-1;
						        break;
			        }

			
				}
			}

	    }
	     else if ((!bDirichlet) /*&& !((inumber==(maxbound-1)) && bPfix && (!bFReeStyle))*/ && !((sosedb[inumber].MCB>=ls) && (sosedb[inumber].MCB<(ls+lw)) && w[sosedb[inumber].MCB-ls].bpressure) )
	    {  
			// �� ������� �������, �� �������� �������� � �����, �� �������� �������
			//printf("neiman!\n"); getchar(); // debug

		

			/*
            // �������� �������� ������������ ����� ����:
		    slb[PAM][inumber].aw=1.0;
		    slb[PAM][inumber].ai=0.0;
		    slb[PAM][inumber].b=0.0;
			slb[PAM][inumber].iI=-1;//sosedb[inumber].iI; //  ������������ � �������
		    slb[PAM][inumber].iW=sosedb[inumber].iB;
			*/

            // ����� ��������������, ��� ���� iI � iII ����������, ����� ���������
		    // ��������� ����� ������������. �� ����� 2-�� ��������� �� � ������ ����� ������.
			//printf("neiman pressure...\n"); // debug
			//getchar();

			///*
		    // ���������� ������� �������
		    doublereal dl, deltal, dS;
	        doublereal taui; // ����������� �� ����� ��.
	        doublereal fiplus; // ���� ��������������� �����
			//doublereal FgRhieChow; // �������� ���-���

			bcheck = true;

			// ���������� �������
	        switch (sosedb[inumber].Norm) {
		        case ESIDE :
			    
				    dl=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x;
					dS=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[1][sosedb[inumber].iI]-1].y; 
					dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����

					slb[PAM][inumber].ai=2.0*dbeta*tau[VX][sosedb[inumber].iB]*dS/dl;
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;

					if (!b_prosto) {

						deltal = 0.5*(pa[nvtx[1][sosedb[inumber].iII] - 1].x + pa[nvtx[0][sosedb[inumber].iII] - 1].x);
						deltal -= 0.5*(pa[nvtx[1][sosedb[inumber].iI] - 1].x + pa[nvtx[0][sosedb[inumber].iI] - 1].x);
						fiplus = 0.5*dl / deltal;

						taui = (tau[VX][sosedb[inumber].iI] * tau[VX][sosedb[inumber].iII]);
						taui = taui / ((1.0 - fiplus)*tau[VX][sosedb[inumber].iI] + fiplus*tau[VX][sosedb[inumber].iII]);// ��������� !


						// ������ �����:  
						slb[PAM][inumber].b = (dbeta - 1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI] - potent[PAM][sosedb[inumber].iII]) / deltal;

					}
					else slb[PAM][inumber].b = 0.0;
					/*
					*  ���� ��������� �� �������� �������� ����� ����������� ��������.
					*  ����������� �������� ����������� ��������� ���� ���������
					*  � ��������� ����. ����� ����������� �������� ��� ������� 
					*  �������������� ������ (��) ��� ���������� ��������� ����������
					*  �������� � ��������� ����� � ������ ��.
					*  ����� ��� ���� � ��������� ��. �� �������� ���������� (����� ������� �����). 
					*  ����� ���������  �� ��������������� ������� ������������� �.�. � �� �� �����������
					*  ��� ����� �� � �����, �.�. ������������� ��������� ����� ����� ��������� � �������������� 
					*  ���������� ����� ������.
					*  �����: ������� �������������� ������� � ������������ ���� �� ���������!!! � �� ������ ����.
					*  ��������� ����� �������� ����� ������.
					*/
				    break;			
			
		        case NSIDE :

                    dl=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y;
					dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����

					slb[PAM][inumber].ai=2.0*dbeta*tau[VY][sosedb[inumber].iB]*dS/dl;
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					if (!b_prosto) {


						deltal = 0.5*(pa[nvtx[2][sosedb[inumber].iII] - 1].y + pa[nvtx[0][sosedb[inumber].iII] - 1].y);
						deltal -= 0.5*(pa[nvtx[2][sosedb[inumber].iI] - 1].y + pa[nvtx[0][sosedb[inumber].iI] - 1].y);
						fiplus = 0.5*dl / deltal;

						taui = (tau[VY][sosedb[inumber].iI] * tau[VY][sosedb[inumber].iII]);
						taui = taui / ((1.0 - fiplus)*tau[VY][sosedb[inumber].iI] + fiplus*tau[VY][sosedb[inumber].iII]);// ��������� !


						// ������ �����:  
						slb[PAM][inumber].b = (dbeta - 1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI] - potent[PAM][sosedb[inumber].iII]) / deltal;
					}
					else slb[PAM][inumber].b = 0.0;

				    break;

			    case TSIDE : 

                    dl=pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z;
					dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					dS*=(pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y); // ������� �����

					slb[PAM][inumber].ai=2.0*dbeta*tau[VZ][sosedb[inumber].iB]*dS/dl;
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					if (!b_prosto) {


						deltal = 0.5*(pa[nvtx[4][sosedb[inumber].iII] - 1].z + pa[nvtx[0][sosedb[inumber].iII] - 1].z);
						deltal -= 0.5*(pa[nvtx[4][sosedb[inumber].iI] - 1].z + pa[nvtx[0][sosedb[inumber].iI] - 1].z);
						fiplus = 0.5*dl / deltal;

						taui = (tau[VZ][sosedb[inumber].iI] * tau[VZ][sosedb[inumber].iII]);
						taui = taui / ((1.0 - fiplus)*tau[VZ][sosedb[inumber].iI] + fiplus*tau[VZ][sosedb[inumber].iII]);// ��������� !

						// ������ �����:  
						slb[PAM][inumber].b = (dbeta - 1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI] - potent[PAM][sosedb[inumber].iII]) / deltal;
					}
					else slb[PAM][inumber].b = 0.0;

				    break;

			    case WSIDE :

                    dl=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x;
					dS=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y; 
					dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����

					slb[PAM][inumber].ai=2.0*dbeta*tau[VX][sosedb[inumber].iB]*dS/dl;
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;
					
					if (!b_prosto) {

						deltal = -0.5*(pa[nvtx[1][sosedb[inumber].iII] - 1].x + pa[nvtx[0][sosedb[inumber].iII] - 1].x);
						deltal += 0.5*(pa[nvtx[1][sosedb[inumber].iI] - 1].x + pa[nvtx[0][sosedb[inumber].iI] - 1].x);
						fiplus = 0.5*dl / deltal;

						taui = (tau[VX][sosedb[inumber].iI] * tau[VX][sosedb[inumber].iII]);
						taui = taui / ((1.0 - fiplus)*tau[VX][sosedb[inumber].iI] + fiplus*tau[VX][sosedb[inumber].iII]);// ��������� !


						// ������ �����:  
						slb[PAM][inumber].b = (dbeta - 1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI] - potent[PAM][sosedb[inumber].iII]) / deltal;
					}
					else slb[PAM][inumber].b = 0.0;

					break;

		        case SSIDE :

                    dl=pa[nvtx[2][sosedb[inumber].iI]-1].y-pa[nvtx[0][sosedb[inumber].iI]-1].y;
					dS=pa[nvtx[1][sosedb[inumber].iI]-1].x-pa[nvtx[0][sosedb[inumber].iI]-1].x; 
					dS*=(pa[nvtx[4][sosedb[inumber].iI]-1].z-pa[nvtx[0][sosedb[inumber].iI]-1].z); // ������� �����

					slb[PAM][inumber].ai=2.0*dbeta*tau[VY][sosedb[inumber].iB]*dS/dl;
					slb[PAM][inumber].iI=sosedb[inumber].iI;
					slb[PAM][inumber].aw=slb[PAM][inumber].ai;
					slb[PAM][inumber].iW=sosedb[inumber].iB;

                    if (!b_prosto) {


	                    deltal = -0.5*(pa[nvtx[2][sosedb[inumber].iII] - 1].y + pa[nvtx[0][sosedb[inumber].iII] - 1].y);
	                    deltal += 0.5*(pa[nvtx[2][sosedb[inumber].iI] - 1].y + pa[nvtx[0][sosedb[inumber].iI] - 1].y);
	                    fiplus = 0.5*dl / deltal;

	                    taui = (tau[VY][sosedb[inumber].iI] * tau[VY][sosedb[inumber].iII]);
	                    taui = taui / ((1.0 - fiplus)*tau[VY][sosedb[inumber].iI] + fiplus*tau[VY][sosedb[inumber].iII]);// ��������� !


	                    // ������ �����:  
	                    slb[PAM][inumber].b = (dbeta - 1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI] - potent[PAM][sosedb[inumber].iII]) / deltal;
                    }
                    else slb[PAM][inumber].b = 0.0;

                    break;

		        case BSIDE:

					dl = pa[nvtx[4][sosedb[inumber].iI] - 1].z - pa[nvtx[0][sosedb[inumber].iI] - 1].z;
					dS = pa[nvtx[1][sosedb[inumber].iI] - 1].x - pa[nvtx[0][sosedb[inumber].iI] - 1].x;
					dS *= (pa[nvtx[2][sosedb[inumber].iI] - 1].y - pa[nvtx[0][sosedb[inumber].iI] - 1].y); // ������� �����

					slb[PAM][inumber].ai = 2.0*dbeta*tau[VZ][sosedb[inumber].iB] * dS / dl;
					slb[PAM][inumber].iI = sosedb[inumber].iI;
					slb[PAM][inumber].aw = slb[PAM][inumber].ai;
					slb[PAM][inumber].iW = sosedb[inumber].iB;
					if (!b_prosto) {


						deltal = -0.5*(pa[nvtx[4][sosedb[inumber].iII] - 1].z + pa[nvtx[0][sosedb[inumber].iII] - 1].z);
						deltal += 0.5*(pa[nvtx[4][sosedb[inumber].iI] - 1].z + pa[nvtx[0][sosedb[inumber].iI] - 1].z);
						fiplus = 0.5*dl / deltal;

						taui = (tau[VZ][sosedb[inumber].iI] * tau[VZ][sosedb[inumber].iII]);
						taui = taui / ((1.0 - fiplus)*tau[VZ][sosedb[inumber].iI] + fiplus*tau[VZ][sosedb[inumber].iII]);// ��������� !


						// ������ �����:  
						slb[PAM][inumber].b = (dbeta - 1.0)*taui*dS*(potent[PAM][sosedb[inumber].iI] - potent[PAM][sosedb[inumber].iII]) / deltal;
					}
					else slb[PAM][inumber].b = 0.0;

					break;
			} // switch

			//*/
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

			if (j < 6) { slb[PAM][inumber].iW1 = sosedb[inumber].iW[j++]; l++; }
			if (j < 6) { slb[PAM][inumber].iW2 = sosedb[inumber].iW[j++]; l++; }
			if (j < 6) { slb[PAM][inumber].iW3 = sosedb[inumber].iW[j++]; l++; }
			if (j < 6) { slb[PAM][inumber].iW4 = sosedb[inumber].iW[j++]; l++; }

			switch (l) {
			case 0: slb[PAM][inumber].iW1 = -1;
				slb[PAM][inumber].iW2 = -1;
				slb[PAM][inumber].iW3 = -1;
				slb[PAM][inumber].iW4 = -1;
				break;
			case 1: slb[PAM][inumber].iW2 = -1;
				slb[PAM][inumber].iW3 = -1;
				slb[PAM][inumber].iW4 = -1;
				break;
			case 2: slb[PAM][inumber].iW3 = -1;
				slb[PAM][inumber].iW4 = -1;
				break;
			case 3: slb[PAM][inumber].iW4 = -1;
				break;
			}

			// ��������� ���������� � ������ ���������� ���� iI � ������������ 
			// ��������� ���� ��� ��������� ����� ������� ������� �� ���������. 
			// �.�. ���� iI ������ ���������� ��� ������� iI < maxelm, � ������������ 
			// ������� � �������� ������� ����� � �������� >= maxelm ������� ��� ��
			// ������������.
		}
		//}

		if (!bDirichlet) {
			if (!bcheck) {
				if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB < (ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
					//printf("OPENING");
				}
				else {
					/*
					if (fabs(slb[PAM][inumber].ai - slb[PAM][inumber].aw) > 1.0e-9) {
						printf(" ai_ap=%e, b=%e\n", slb[PAM][inumber].ai / slb[PAM][inumber].aw, slb[PAM][inumber].b);
						printf("aw=%e ai=%e b=%e\n", slb[PAM][inumber].aw, slb[PAM][inumber].ai, slb[PAM][inumber].b);

						printf("my_elmatr_quad_PAm_bon3\n");
						system("pause");
					}
					*/
				}
		}
		//if (fabs(slb[PAM][inumber].b) > 1.0e-9) {
			//printf("non zero b: ai_ap=%e, b=%e\n", slb[PAM][inumber].ai / slb[PAM][inumber].aw, slb[PAM][inumber].b);
			//printf("my_elmatr_quad_PAm_bon3\n");
			//system("pause");
		//}

	}

		if (!bcheck) {
			if (!bDirichlet) {
				if (((sosedb[inumber].MCB >= ls) && (sosedb[inumber].MCB < (ls + lw)) && w[sosedb[inumber].MCB - ls].bopening)) {
					//printf("OPENING");
				}
				else {
					/*
					printf("undefined situation for pam found. 10.02.2017\n");
					printf("my_elmatr_quad_PAm_bon3 method.\n");
					//if (bDirichlet) {
						//printf("Dirichlet\n");
					//}
					//else {
						//printf("Neiman\n");
					//}

					system("pause");


					slb[PAM][inumber].ai = 1.0;
					slb[PAM][inumber].iI = sosedb[inumber].iI;
					slb[PAM][inumber].aw = 1.0;
					slb[PAM][inumber].iW = sosedb[inumber].iB;
					slb[PAM][inumber].b = 0.0;
					*/
				}
			}
	}


} // my_elmatr_quad_PAm_bon3


  // ���������� ����������������� �������� �����.
  // ���������������� �������� ����� mf ����������� �� ������ �������������
  // ����������������� �������� ��� ������ �������� �� ������ ���������� �������������.
// ��� ����� ��� ���������� ������� ��������� ���������-�������� ��� ������ ���������������� ��������,
// ������� �������� ���-��� ������ �������������.
// 26.03.2017
void return_calc_correct_mass_flux_only_interpolation(integer iP, doublereal** potent, TOCHKA* pa, doublereal** prop, doublereal** prop_b,
	integer** nvtx, ALICE_PARTITION** sosedi, integer maxelm, 
	doublereal* &mfcurrentretune)
{

	// SpeedCorOld - ���������������� �������� �� ���������� ��������.

	// ���� bsimplelinearinterpol ����� true �� ����������� ������� �������� ������������ �������� �� ����� ������������ ������.

	// ��-�������� ����� ����� �������� �������� ���-��� ������ �� ���������� �����,
	// ��� ��������� ����� �������� ������ (�� ��������� �������) � ��-�������� �� 
	// ��������� ��������� � ��� ���������������� ��������.

	// ��������� ��� �������� �������� ���-��� 1983�.
	//bool bRhieChowi = true, bRhieChowb = false; // i - internal, b - border.

	// iP - ����� ������������ ������������ ������
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


	// ������������ � ����
	bool bslE = false, bslN = false, bslT = false, bslW = false, bslS = false, bslB = false;
	bool bslE2 = false, bslN2 = false, bslT2 = false, bslW2 = false, bslS2 = false, bslB2 = false;
	bool bslE3 = false, bslN3 = false, bslT3 = false, bslW3 = false, bslS3 = false, bslB3 = false;
	bool bslE4 = false, bslN4 = false, bslT4 = false, bslW4 = false, bslS4 = false, bslB4 = false;

	// ���� � ����� �� ������ ������� ��������� ������� 
	// �� ���������� ����� true
	bool bE = false, bN = false, bT = false, bW = false, bS = false, bB = false;
	bool bE2 = false, bN2 = false, bT2 = false, bW2 = false, bS2 = false, bB2 = false;
	bool bE3 = false, bN3 = false, bT3 = false, bW3 = false, bS3 = false, bB3 = false;
	bool bE4 = false, bN4 = false, bT4 = false, bW4 = false, bS4 = false, bB4 = false;

	if (iE >= maxelm) bE = true;
	if (iN >= maxelm) bN = true;
	if (iT >= maxelm) bT = true;
	if (iW >= maxelm) bW = true;
	if (iS >= maxelm) bS = true;
	if (iB >= maxelm) bB = true;

	if (iE > -1) bslE = true;
	if (iW > -1) bslW = true;
	if (iN > -1) bslN = true;
	if (iS > -1) bslS = true;
	if (iT > -1) bslT = true;
	if (iB > -1) bslB = true;

	if (iE2 > -1) bslE2 = true;
	if (iW2 > -1) bslW2 = true;
	if (iN2 > -1) bslN2 = true;
	if (iS2 > -1) bslS2 = true;
	if (iT2 > -1) bslT2 = true;
	if (iB2 > -1) bslB2 = true;

	if (iE3 > -1) bslE3 = true;
	if (iW3 > -1) bslW3 = true;
	if (iN3 > -1) bslN3 = true;
	if (iS3 > -1) bslS3 = true;
	if (iT3 > -1) bslT3 = true;
	if (iB3 > -1) bslB3 = true;

	if (iE4 > -1) bslE4 = true;
	if (iW4 > -1) bslW4 = true;
	if (iN4 > -1) bslN4 = true;
	if (iS4 > -1) bslS4 = true;
	if (iT4 > -1) bslT4 = true;
	if (iB4 > -1) bslB4 = true;

	if (iE2 >= maxelm) bE2 = true;
	if (iW2 >= maxelm) bW2 = true;
	if (iN2 >= maxelm) bN2 = true;
	if (iS2 >= maxelm) bS2 = true;
	if (iT2 >= maxelm) bT2 = true;
	if (iB2 >= maxelm) bB2 = true;

	if (iE3 >= maxelm) bE3 = true;
	if (iW3 >= maxelm) bW3 = true;
	if (iN3 >= maxelm) bN3 = true;
	if (iS3 >= maxelm) bS3 = true;
	if (iT3 >= maxelm) bT3 = true;
	if (iB3 >= maxelm) bB3 = true;

	if (iE4 >= maxelm) bE4 = true;
	if (iW4 >= maxelm) bW4 = true;
	if (iN4 >= maxelm) bN4 = true;
	if (iS4 >= maxelm) bS4 = true;
	if (iT4 >= maxelm) bT4 = true;
	if (iB4 >= maxelm) bB4 = true;

	// ���������� �������� �������� ������������ ������:
	doublereal dx = 0.0, dy = 0.0, dz = 0.0; // ������� ������������ ������
	volume3D(iP, nvtx, pa, dx, dy, dz);

	doublereal dxe = 0.5*dx, dxw = 0.5*dx, dyn = 0.5*dy, dys = 0.5*dy, dzt = 0.5*dz, dzb = 0.5*dz;
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

	// ��������� ���������������� ������� �������������
	doublereal rhoe=0.0, rhow = 0.0, rhon = 0.0, rhos = 0.0, rhot = 0.0, rhob = 0.0;
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


	// ������������ ��������� ������� ���, ����� ����������� 
	// ���������� �����������.
	if (!bE) rhoe = rE*rP / (feplus*rE + (1.0 - feplus)*rP); else rhoe = rE;
	if (!bW) rhow = rW*rP / (fwplus*rW + (1.0 - fwplus)*rP); else rhow = rW;
	if (!bN) rhon = rN*rP / (fnplus*rN + (1.0 - fnplus)*rP); else rhon = rN;
	if (!bS) rhos = rS*rP / (fsplus*rS + (1.0 - fsplus)*rP); else rhos = rS;
	if (!bT) rhot = rT*rP / (ftplus*rT + (1.0 - ftplus)*rP); else rhot = rT;
	if (!bB) rhob = rB*rP / (fbplus*rB + (1.0 - fbplus)*rP); else rhob = rB;


	doublereal rhoe2, rhow2, rhon2, rhos2, rhot2, rhob2;
	doublereal rhoe3, rhow3, rhon3, rhos3, rhot3, rhob3;
	doublereal rhoe4, rhow4, rhon4, rhos4, rhot4, rhob4;

	if (fabs(feplus2*rE2 + (1.0 - feplus2)*rP) < 1.0e-30) {
		rhoe2 = 0.0;
	}
	else {
		rhoe2 = rE2*rP / (feplus2*rE2 + (1.0 - feplus2)*rP);
	}
	if (fabs((fwplus2*rW2 + (1.0 - fwplus2)*rP)) < 1.0e-30) {
		rhow2 = 0.0;
	}
	else {
		rhow2 = rW2*rP / (fwplus2*rW2 + (1.0 - fwplus2)*rP);
	}
	
	if (fabs((fnplus2*rN2 + (1.0 - fnplus2)*rP)) < 1.0e-30) {
		rhon2 = 0.0;
	}
	else {
		rhon2 = rN2*rP / (fnplus2*rN2 + (1.0 - fnplus2)*rP);
	}
	
	if (fabs((fsplus2*rS2 + (1.0 - fsplus2)*rP)) < 1.0e-30) {
		rhos2 = 0.0;
	}
	else {
		rhos2 = rS2*rP / (fsplus2*rS2 + (1.0 - fsplus2)*rP);
	}
	
	if (fabs((ftplus2*rT2 + (1.0 - ftplus2)*rP)) < 1.0e-30) {
		rhot2 = 0.0;
	}
	else {
		rhot2 = rT2*rP / (ftplus2*rT2 + (1.0 - ftplus2)*rP);
	}
	
	if (fabs((fbplus2*rB2 + (1.0 - fbplus2)*rP)) < 1.0e-30) {
		rhob2 = 0.0;
	}
	else {
		rhob2 = rB2*rP / (fbplus2*rB2 + (1.0 - fbplus2)*rP);
	}
	
	if (fabs((feplus3*rE3 + (1.0 - feplus3)*rP)) < 1.0e-30) {
		rhoe3 = 0.0;
	}
	else {
		rhoe3 = rE3*rP / (feplus3*rE3 + (1.0 - feplus3)*rP);
	}
	
	if (fabs((fwplus3*rW3 + (1.0 - fwplus3)*rP)) < 1.0e-30) {
		rhow3 = 0.0;
	}
	else {
		rhow3 = rW3*rP / (fwplus3*rW3 + (1.0 - fwplus3)*rP);
	}
	
	if (fabs((fnplus3*rN3 + (1.0 - fnplus3)*rP)) < 1.0e-30) {
		rhon3 = 0.0;
	}
	else {
		rhon3 = rN3*rP / (fnplus3*rN3 + (1.0 - fnplus3)*rP);
	}
	
	if (fabs((fsplus3*rS3 + (1.0 - fsplus3)*rP)) < 1.0e-30) {
		rhos3 = 0.0;
	}
	else {
		rhos3 = rS3*rP / (fsplus3*rS3 + (1.0 - fsplus3)*rP);
	}
	
	if (fabs((ftplus3*rT3 + (1.0 - ftplus3)*rP)) < 1.0e-30) {
		rhot3 = 0.0;
	}
	else {
		rhot3 = rT3*rP / (ftplus3*rT3 + (1.0 - ftplus3)*rP);
	}
	
	if (fabs((fbplus3*rB3 + (1.0 - fbplus3)*rP)) < 1.0e-30) {
		rhob3 = 0.0;
	}
	else {
		rhob3 = rB3*rP / (fbplus3*rB3 + (1.0 - fbplus3)*rP);
	}
	

	if (fabs((feplus4*rE4 + (1.0 - feplus4)*rP)) < 1.0e-30) {
		rhoe4 = 0.0;
	}
	else {
		rhoe4 = rE4*rP / (feplus4*rE4 + (1.0 - feplus4)*rP);
	}
	
	if (fabs((fwplus4*rW4 + (1.0 - fwplus4)*rP)) < 1.0e-30) {
		rhow4 = 0.0;
	}
	else {
		rhow4 = rW4*rP / (fwplus4*rW4 + (1.0 - fwplus4)*rP);
	}
	
	if (fabs((fnplus4*rN4 + (1.0 - fnplus4)*rP)) < 1.0e-30) {
		rhon4 = 0.0;
	}
	else {
		rhon4 = rN4*rP / (fnplus4*rN4 + (1.0 - fnplus4)*rP);
	}
	
	if (fabs((fsplus4*rS4 + (1.0 - fsplus4)*rP)) < 1.0e-30) {
		rhos4 = 0.0;
	}
	else {
		rhos4 = rS4*rP / (fsplus4*rS4 + (1.0 - fsplus4)*rP);
	}
	
	if (fabs((ftplus4*rT4 + (1.0 - ftplus4)*rP)) < 1.0e-30) {
		rhot4= 0.0;
	}
	else {
		rhot4 = rT4*rP / (ftplus4*rT4 + (1.0 - ftplus4)*rP);
	}
	
	if (fabs((fbplus4*rB4 + (1.0 - fbplus4)*rP)) < 1.0e-30) {
		rhob4 = 0.0;
	}
	else {
		rhob4 = rB4*rP / (fbplus4*rB4 + (1.0 - fbplus4)*rP);
	}
	

	doublereal Fw = 0.0, Fe = 0.0, Fs = 0.0, Fn = 0.0, Ft = 0.0, Fb = 0.0;

	// ��� ���� �����.
	//doublereal Fe1 = 0.0, Fe2 = 0.0, Fe3 = 0.0, Fe4 = 0.0;
	//doublereal Fw1 = 0.0, Fw2 = 0.0, Fw3 = 0.0, Fw4 = 0.0;
	//doublereal Fn1 = 0.0, Fn2 = 0.0, Fn3 = 0.0, Fn4 = 0.0;
	//doublereal Fs1 = 0.0, Fs2 = 0.0, Fs3 = 0.0, Fs4 = 0.0;
	//doublereal Ft1 = 0.0, Ft2 = 0.0, Ft3 = 0.0, Ft4 = 0.0;
	//doublereal Fb1 = 0.0, Fb2 = 0.0, Fb3 = 0.0, Fb4 = 0.0;

	

		doublereal SpeedCorOlde=0.0, SpeedCorOldw = 0.0, SpeedCorOldn = 0.0, SpeedCorOlds = 0.0, SpeedCorOldt = 0.0, SpeedCorOldb = 0.0;
		if (!bE) {
			SpeedCorOlde = feplus*potent[VX][iE] + (1.0 - feplus)*potent[VX][iP];
		}
		else {
			SpeedCorOlde = potent[VX][iE];
		}
		if (!bN) {
			SpeedCorOldn = fnplus*potent[VY][iN] + (1.0 - fnplus)*potent[VY][iP];
		}
		else {
			SpeedCorOldn = potent[VY][iN];
		}
		if (!bT) {
			SpeedCorOldt = ftplus*potent[VZ][iT] + (1.0 - ftplus)*potent[VZ][iP];
		}
		else {
			SpeedCorOldt = potent[VZ][iT];
		}
		if (!bW) {
			SpeedCorOldw = fwplus*potent[VX][iW] + (1.0 - fwplus)*potent[VX][iP];
		}
		else {
			SpeedCorOldw = potent[VX][iW];
		}
		if (!bS) {
			SpeedCorOlds = fsplus*potent[VY][iS] + (1.0 - fsplus)*potent[VY][iP];
		}
		else {
			SpeedCorOlds = potent[VY][iS];
		}
		if (!bB) {
			SpeedCorOldb = fbplus*potent[VZ][iB] + (1.0 - fbplus)*potent[VZ][iP];
		}
		else {
			SpeedCorOldb = potent[VZ][iB];
		}

		doublereal SpeedCorOlde2 = 0.0, SpeedCorOldw2 = 0.0, SpeedCorOldn2 = 0.0, SpeedCorOlds2 = 0.0, SpeedCorOldt2 = 0.0, SpeedCorOldb2 = 0.0;
		doublereal SpeedCorOlde3 = 0.0, SpeedCorOldw3 = 0.0, SpeedCorOldn3 = 0.0, SpeedCorOlds3 = 0.0, SpeedCorOldt3 = 0.0, SpeedCorOldb3 = 0.0;
		doublereal SpeedCorOlde4 = 0.0, SpeedCorOldw4 = 0.0, SpeedCorOldn4 = 0.0, SpeedCorOlds4 = 0.0, SpeedCorOldt4 = 0.0, SpeedCorOldb4 = 0.0;


		if (!bE2) {
			SpeedCorOlde2 = feplus2*potent[VX][iE2] + (1.0 - feplus2)*potent[VX][iP];
		}
		else {
			SpeedCorOlde2 = potent[VX][iE2];
		}
		if (!bN2) {
			SpeedCorOldn2 = fnplus2*potent[VY][iN2] + (1.0 - fnplus2)*potent[VY][iP];
		}
		else {
			SpeedCorOldn2 = potent[VY][iN2];
		}
		if (!bT2) {
			SpeedCorOldt2 = ftplus2*potent[VZ][iT2] + (1.0 - ftplus2)*potent[VZ][iP];
		}
		else {
			SpeedCorOldt2 = potent[VZ][iT2];
		}
		if (!bW2) {
			SpeedCorOldw2 = fwplus2*potent[VX][iW2] + (1.0 - fwplus2)*potent[VX][iP];
		}
		else {
			SpeedCorOldw2 = potent[VX][iW2];
		}
		if (!bS2) {
			SpeedCorOlds2 = fsplus2*potent[VY][iS2] + (1.0 - fsplus2)*potent[VY][iP];
		}
		else {
			SpeedCorOlds2 = potent[VY][iS2];
		}
		if (!bB2) {
			SpeedCorOldb2 = fbplus2*potent[VZ][iB2] + (1.0 - fbplus2)*potent[VZ][iP];
		}
		else {
			SpeedCorOldb2 = potent[VZ][iB2];
		}


		if (!bE3) {
			SpeedCorOlde3 = feplus3*potent[VX][iE3] + (1.0 - feplus3)*potent[VX][iP];
		}
		else {
			SpeedCorOlde3 = potent[VX][iE3];
		}
		if (!bN3) {
			SpeedCorOldn3 = fnplus3*potent[VY][iN3] + (1.0 - fnplus3)*potent[VY][iP];
		}
		else {
			SpeedCorOldn3 = potent[VY][iN3];
		}
		if (!bT3) {
			SpeedCorOldt3 = ftplus3*potent[VZ][iT3] + (1.0 - ftplus3)*potent[VZ][iP];
		}
		else {
			SpeedCorOldt3 = potent[VZ][iT3];
		}
		if (!bW3) {
			SpeedCorOldw3 = fwplus3*potent[VX][iW3] + (1.0 - fwplus3)*potent[VX][iP];
		}
		else {
			SpeedCorOldw3 = potent[VX][iW3];
		}
		if (!bS3) {
			SpeedCorOlds3 = fsplus3*potent[VY][iS3] + (1.0 - fsplus3)*potent[VY][iP];
		}
		else {
			SpeedCorOlds3 = potent[VY][iS3];
		}
		if (!bB3) {
			SpeedCorOldb3 = fbplus3*potent[VZ][iB3] + (1.0 - fbplus3)*potent[VZ][iP];
		}
		else {
			SpeedCorOldb3 = potent[VZ][iB3];
		}


		if (!bE4) {
			SpeedCorOlde4 = feplus4*potent[VX][iE4] + (1.0 - feplus4)*potent[VX][iP];
		}
		else {
			SpeedCorOlde4 = potent[VX][iE4];
		}
		if (!bN4) {
			SpeedCorOldn4 = fnplus4*potent[VY][iN4] + (1.0 - fnplus4)*potent[VY][iP];
		}
		else {
			SpeedCorOldn4 = potent[VY][iN4];
		}
		if (!bT4) {
			SpeedCorOldt4 = ftplus4*potent[VZ][iT4] + (1.0 - ftplus4)*potent[VZ][iP];
		}
		else {
			SpeedCorOldt4 = potent[VZ][iT4];
		}
		if (!bW4) {
			SpeedCorOldw4 = fwplus4*potent[VX][iW4] + (1.0 - fwplus4)*potent[VX][iP];
		}
		else {
			SpeedCorOldw4 = potent[VX][iW4];
		}
		if (!bS4) {
			SpeedCorOlds4 = fsplus4*potent[VY][iS4] + (1.0 - fsplus4)*potent[VY][iP];
		}
		else {
			SpeedCorOlds4 = potent[VY][iS4];
		}
		if (!bB4) {
			SpeedCorOldb4 = fbplus4*potent[VZ][iB4] + (1.0 - fbplus4)*potent[VZ][iP];
		}
		else {
			SpeedCorOldb4 = potent[VZ][iB4];
		}


		// mfold - �������� ��������� ������ � ���������� ��������.
		Fe =  (rhoe*SpeedCorOlde+ rhoe2*SpeedCorOlde2 + rhoe3*SpeedCorOlde3 + rhoe4*SpeedCorOlde4)*dy*dz;
		Fn = (rhon*SpeedCorOldn + rhon2*SpeedCorOldn2 + rhon3*SpeedCorOldn3 + rhon4*SpeedCorOldn4)*dx*dz;
		Ft = (rhot*SpeedCorOldt + rhot2*SpeedCorOldt2 + rhot3*SpeedCorOldt3 + rhot4*SpeedCorOldt4)*dx*dy;
		Fw = (rhow*SpeedCorOldw + rhow2*SpeedCorOldw2 + rhow3*SpeedCorOldw3 + rhow4*SpeedCorOldw4)*dy*dz;
		Fs = (rhos*SpeedCorOlds + rhos2*SpeedCorOlds2 + rhos3*SpeedCorOlds3 + rhos4*SpeedCorOlds4)*dx*dz;
		Fb = (rhob*SpeedCorOldb + rhob2*SpeedCorOldb2 + rhob3*SpeedCorOldb3 + rhob4*SpeedCorOldb4)*dx*dy;
	

	
		mfcurrentretune[ESIDE] = Fe;
		mfcurrentretune[NSIDE] = Fn;
		mfcurrentretune[TSIDE] = Ft;
		mfcurrentretune[WSIDE] = Fw;
		mfcurrentretune[SSIDE] = Fs;
		mfcurrentretune[BSIDE] = Fb;
	

} // return_correct_mass_flux_only_interpolation


// ���������� �������� �������� �� ����� � �������������� ���������������� �������� ���-���.
// �������� : ������ ���������� �������� �� ������ ��� ������������ �����, �� � ��� ��������������.
// ��� �������������� ����� ��������� ������� �������� ����� �� ����� � ����������� ���������� ����.
// �������� ��������� ������ ������� ����������� � � ���������� ������ (��������� ��������� ������� ��� ���������
// ��� ������) ��-�� �������� ���������� ������������ ����������.
// ����������� 31 ����� 2012 ����.
doublereal calcFg(bool bG, doublereal fgplus, doublereal VG, doublereal VP, 
	        integer iP, integer G, doublereal alpha, doublereal rhog, 
			doublereal dS, doublereal dV, bool btimedepend, 
			doublereal** speedoldtimestep, doublereal* mfoldtimestep,
			doublereal dG, doublereal dP, doublereal dtimestep,
			doublereal RCh, integer** nvtx, ALICE_PARTITION** sosedi,
			integer maxelm, doublereal* pressure,  TOCHKA* pa, doublereal **diag_coef,
			bool bRhieChowi, bool bRhieChowb, bool bsimplelinearinterpol) {
	/*
	*  bG - �������� �� ����� ��������� ? 
	*  fgplus - ���������������� ������,
	*  VG, VP - �������� ��������������� �������� � ����� iP � iG. 
	*  RCh (= 1.0 �� ���������) - ������������ ��� ���������� ������ �������� ���-���.
	*  iP - ����� ���������������� ������������ ������,
	*  G - ������������� ����� �� 6 ������,
	* alpha - ����������� ���������� ��� ��������,
	* rhog - ��������� �� ����� ��, 
	* dS - ������� ����� ������������ ������,
	* dV - ����� ������������ ������,
	* btimedepend- ������������ ��� �������������� ������ ?
	* speedoldtimestep[VX,VY,VZ][iP,iG] - ������ ��������� � ����������� ���������� ����,
	* VGoldtimestep - �������� � ���� iG � ����������� ���������� ����,
	* VPoldtimestep - �������� � ���� iP � ����������� ���������� ����,
	* mfoldtimestep - �������� ����� � ����������� ���������� ���� �� ����� G ( ������ �� ����� �������� ��� ������ �������� G=E,W,N,S,T,B).
	* dG - ������������ ����������� � ������� ��� ������ ���������� �������� � ���� iG,
	* dP - ������������ ����������� � ������� ��� ������ ���������� �������� � ���� iP,
	* dtimestep - ������ ���� �� �������,
	* pressure - ���� ��������,
	* diag_coef - ������������ ������������ � �������� ��� ��������� �������� ������� ���� ��������� ����� ����� ������� ��-�� ���������� ��������.
	* bRhieChowi (internal) - ������������� �� ���������������� �������� ���-��� ��� ���������� ����� ��������� �������.
	* bRhieChowb (border) - ������������� �� ���������������� �������� ���-��� ��� ��������� ����� ��������� �������.
	* ���� bsimplelinearinterpol ����� ������ �� ����� ��������� ������ �������� ������������ �������� �� ����� ��� ������������� �������� ���-���.
	* ����� ����������� ��������� �� ������ �������� ������������� ���������.
	*/

	doublereal ug=0.0; // �������� �� ����� ������������ ������.
	doublereal Fg=0.0; // �������� ����� �� ����� ������������ ������.

	if (bsimplelinearinterpol) {

		// ����� ������� �������� ������������ �������� �� �����,
		// ������������ �� ������ �������� ������������� �������.
        if (!bG) { 
	          ug=fgplus*VG+(1.0-fgplus)*VP;
	    }
	    else {
	          ug=VG;
	    }

		// ������������ ����� ����� ����� ��.
	    Fg=rhog*ug*dS;
	}
	else
	{

	// ��� �������� ���� ��� ���� ����� �������� ��������������
	// ������ ���������� ��� ��� ��� �������� � I. Sezai.
	if (!bG) { 
	   ug=fgplus*VG+(1.0-fgplus)*VP;
	   if (bRhieChowi) {
	       ug+=RCh*ugRhieChow_internal(iP, G, alpha, nvtx, sosedi, maxelm, pressure, pa, diag_coef); // ����� �������� ���-���
	   }
	}
	else {
	   ug=VG;
	   if (bRhieChowb) {
	      ug+=RCh*ugRhieChow_internal_border(iP, G, alpha, nvtx, sosedi, maxelm, pressure, pa, diag_coef); // ����� �������� ���-���
	   }
	}

	// ������������ ����� ����� ����� ��.
	Fg=rhog*ug*dS;

	// �������� ����� �������������� ��� ��������� ������ ���������.
	if (btimedepend) {
		// � �������������� ������ � �������� ���-��� ��������� ������� �������.

		doublereal VGoldtimestep=0.0;
		doublereal VPoldtimestep=0.0;
		integer iG=sosedi[G][iP].iNODE1;
		switch (G) {
		 case ESIDE : case WSIDE : VPoldtimestep=speedoldtimestep[VX][iP];
			               VGoldtimestep=speedoldtimestep[VX][iG];
			      break;
		 case NSIDE : case SSIDE :VPoldtimestep=speedoldtimestep[VY][iP];
			               VGoldtimestep=speedoldtimestep[VY][iG];
			      break;
		 case TSIDE :  case BSIDE : VPoldtimestep=speedoldtimestep[VZ][iP];
			                VGoldtimestep=speedoldtimestep[VZ][iG];
			      break;
		 }

		// �������� �� ����� �� � ����������� ���������� ����:
		doublereal ugold=fgplus*VGoldtimestep+(1.0-fgplus)*VPoldtimestep; 


		// ������������ ����������� ������� ������ ���� ���� �� ������� ������������ ������.
		if (!bG) {
			// ���������� ����������� �����
		    doublereal dc_e=dP*dG/(fgplus*dG+(1.0-fgplus)*dP); // ������������ ����������� �� �����
		    doublereal tau_e=(rhog*alpha*dV)/(dc_e); // ����������� (rhog - ��������� �� ����� ������������ ������.
			if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) tau_e/=(1.0-alpha);
		    Fg+=(tau_e/dtimestep)*(mfoldtimestep[G]-rhog*ugold*dS);
		}
		else {
			// ��������� ����������� �����
			doublereal tau_e=(rhog*alpha*dV)/dG;
			if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) tau_e/=(1.0-alpha);
			Fg+=(tau_e/dtimestep)*(mfoldtimestep[G]-rhog*ugold*dS);
		}

	}
	}

	return Fg;

} // calcFg


// ���������� �������� �������� �� ����� � �������������� ���������������� �������� ���-���.
// �������� : ������ ���������� �������� �� ������ ��� ������������ �����, �� � ��� ��������������.
// ��� �������������� ����� ��������� ������� �������� ����� �� ����� � ����������� ���������� ����.
// �������� ��������� ������ ������� ����������� � � ���������� ������ (��������� ��������� ������� ��� ���������
// ��� ������) ��-�� �������� ���������� ������������ ����������.
// ����������� 21 ���� 2012 ����. (����� ������� ������� �� ��������� � calcFg).
// ������ �������� ������ ���� �� ��� (CFD).
doublereal calcFg2(bool bG, doublereal fgplus, 
	        integer iP, integer G, doublereal rhog, 
			doublereal dS, bool btimedepend, 
			doublereal** speedoldtimestep, doublereal* mfoldtimestep,
			doublereal dtimestep, doublereal RCh, ALICE_PARTITION** sosedi,
			bool bRhieChowi, bool bRhieChowb, bool bsimplelinearinterpol,
			doublereal* tau, doublereal drg, doublereal** potent) {
	/*
	*  bG - �������� �� ����� ��������� ? 
	*  fgplus - ���������������� ������,
	*  VG, VP - �������� ��������������� �������� � ����� iP � iG. 
	*  RCh (= 1.0 �� ���������) - ������������ ��� ���������� ������ �������� ���-���.
	*  iP - ����� ���������������� ������������ ������,
	*  G - ������������� ����� �� 6 ������,
	* rhog - ��������� �� ����� ��, 
	* dS - ������� ����� ������������ ������,
	* btimedepend- ������������ ��� �������������� ������ ?
	* speedoldtimestep[VX,VY,VZ][iP,iG] - ������ ��������� � ����������� ���������� ����,
	* VGoldtimestep - �������� � ���� iG � ����������� ���������� ����,
	* VPoldtimestep - �������� � ���� iP � ����������� ���������� ����,
	* mfoldtimestep - �������� ����� � ����������� ���������� ���� �� ����� G ( ������ �� ����� �������� ��� ������ �������� G=E,W,N,S,T,B).
	* dtimestep - ������ ���� �� �������,
	* potent - �������� � �������� ��������,
	* bRhieChowi (internal) - ������������� �� ���������������� �������� ���-��� ��� ���������� ����� ��������� �������.
	* bRhieChowb (border) - ������������� �� ���������������� �������� ���-��� ��� ��������� ����� ��������� �������.
	* ���� bsimplelinearinterpol ����� ������ �� ����� ��������� ������ �������� ������������ �������� �� ����� ��� ������������� �������� ���-���.
	* ����� ����������� ��������� �� ������ �������� ������������� ���������.
	* tau - ���������� �����������.
	* drg - ���������� ����� ������������ �������� iP � iG.
	*/

	integer iG=sosedi[G][iP].iNODE1;
	doublereal VG=0.0, VP=0.0;
	switch (G) {
		case ESIDE : case WSIDE : VG=potent[VX][iG]; VP=potent[VX][iP]; break;
		case NSIDE : case SSIDE :VG=potent[VY][iG]; VP=potent[VY][iP]; break;
		case TSIDE : case BSIDE : VG=potent[VZ][iG]; VP=potent[VZ][iP]; break;
	} // end switch
	

	doublereal ug=0.0; // �������� �� ����� ������������ ������.
	doublereal Fg=0.0; // �������� ����� �� ����� ������������ ������.

	if (bsimplelinearinterpol) {

		// ����� ������� �������� ������������ �������� �� �����,
		// ������������ �� ������ �������� ������������� �������.
        if (!bG) { 
	          ug=fgplus*VG+(1.0-fgplus)*VP;
	    }
	    else {
	          ug=VG;
	    }

		// ������������ ����� ����� ����� ��.
	    Fg=rhog*ug*dS;
	}
	else
	{

		
        doublereal taug=tau[iP]*tau[iG]/(fgplus*tau[iG]+(1.0-fgplus)*tau[iP]); // ����������� �� ����� ��.
	    doublereal gradP=0.0, gradG=0.0;
	    switch(G) {
	       case ESIDE : gradP=potent[GRADXPRESS][iP]; gradG=potent[GRADXPRESS][sosedi[ESIDE][iP].iNODE1]; break;
	       case WSIDE : gradP=potent[GRADXPRESS][iP]; gradG=potent[GRADXPRESS][sosedi[WSIDE][iP].iNODE1]; break;
	       case NSIDE : gradP=potent[GRADYPRESS][iP]; gradG=potent[GRADYPRESS][sosedi[NSIDE][iP].iNODE1]; break;
	       case SSIDE :gradP=potent[GRADYPRESS][iP]; gradG=potent[GRADYPRESS][sosedi[SSIDE][iP].iNODE1]; break;
	       case TSIDE : gradP=potent[GRADZPRESS][iP]; gradG=potent[GRADZPRESS][sosedi[TSIDE][iP].iNODE1]; break;
	       case BSIDE : gradP=potent[GRADZPRESS][iP]; gradG=potent[GRADZPRESS][sosedi[BSIDE][iP].iNODE1]; break;
	    }


	    // ��� �������� ���� ��� ���� ����� �������� ��������������
	    // ������ ���������� ��� ��� ��� �������� � I. Sezai.
	    if (!bG) { 
	       ug=fgplus*VG+(1.0-fgplus)*VP;
	       if (bRhieChowi) {
	           //ug+=RCh*ugRhieChow_internal(iP, G, alpha, nvtx, sosedi, maxelm, pressure, pa, diag_coef); // ����� �������� ���-���
		       ug+=RCh*(taug/rhog)*(-(potent[PRESS][iG]-potent[PRESS][iP])/drg+(1.0-fgplus)*gradP+fgplus*gradG); // ����� �������� ���-���
	       }
	    }
	    else {
	       ug=VG;
	       /*
	       // ������� ��� ���������� ���� ����. �.�. ������� � �������.
	       if (bRhieChowb) {
	          set additional ug+=0.0;
	          //ug+=RCh*ugRhieChow_internal_border(iP, G, alpha, nvtx, sosedi, maxelm, pressure, pa, diag_coef); // ����� �������� ���-���
	       }
	       */
	    }

	    // ������������ ����� ����� ����� ��.
	    Fg=rhog*ug*dS;

 	    // �������� ����� �������������� ��� ��������� ������ ���������.
	    if (btimedepend) {
			// � �������������� ������ � �������� ���-��� ��������� ������� �������.

		    doublereal VGoldtimestep=0.0;
		    doublereal VPoldtimestep=0.0;

		    switch (G) {
		       case ESIDE : case WSIDE : VPoldtimestep=speedoldtimestep[VX][iP];
			                     VGoldtimestep=speedoldtimestep[VX][iG];
			                     break;
		       case NSIDE : case SSIDE :VPoldtimestep=speedoldtimestep[VY][iP];
			                     VGoldtimestep=speedoldtimestep[VY][iG];
			                     break;
		       case TSIDE :  case BSIDE : VPoldtimestep=speedoldtimestep[VZ][iP];
			                      VGoldtimestep=speedoldtimestep[VZ][iG];
			                     break;
		    }

		    // �������� �� ����� �� � ����������� ���������� ����:
		    doublereal ugold=fgplus*VGoldtimestep+(1.0-fgplus)*VPoldtimestep; 


		    // ������������ ����������� ������� ������ ���� ���� �� ������� ������������ ������.
		    if (!bG) {
			    // ���������� ����������� �����
		        Fg+=(taug/dtimestep)*(mfoldtimestep[G]-rhog*ugold*dS);
		    }
		    else {
			    // ��������� ����������� �����
			    Fg+=(taug/dtimestep)*(mfoldtimestep[G]-rhog*ugold*dS);
		    }

    	}
	}

	return Fg;

} // calcFg2

// ���������� �������� �������� �� ����� � �������������� ���������������� �������� ���-���.
// �������� : ������ ���������� �������� �� ������ ��� ������������ �����, �� � ��� ��������������.
// ��� �������������� ����� ��������� ������� �������� ����� �� ����� � ����������� ���������� ����.
// �������� ��������� ������ ������� ����������� � � ���������� ������ (��������� ��������� ������� ��� ���������
// ��� ������) ��-�� �������� ���������� ������������ ����������.
// ����������� 23 ���� 2012 ����. (����� ������� ������� �� ��������� � calcFg).
// ������ �������� ������ ���� �� ��� (CFD).
// ������ ������ �������� �� ��� ��������� ����� �������������.
doublereal calcFg3(bool bG, doublereal fgplus, 
	        integer iP, integer G, doublereal rhog, 
			doublereal dS, bool btimedepend, 
			doublereal** speedoldtimestep, doublereal* mfoldtimestep,
			doublereal dtimestep, doublereal RCh, ALICE_PARTITION** sosedi,
			bool bRhieChowi, bool bRhieChowb, bool bsimplelinearinterpol,
			doublereal** tau, doublereal drg, doublereal** potent) {
	/*
	*  bG - �������� �� ����� ��������� ? 
	*  fgplus - ���������������� ������,
	*  VG, VP - �������� ��������������� �������� � ����� iP � iG. 
	*  RCh (= 1.0 �� ���������) - ������������ ��� ���������� ������ �������� ���-���.
	*  iP - ����� ���������������� ������������ ������,
	*  G - ������������� ����� �� 6 ������,
	* rhog - ��������� �� ����� ��, 
	* dS - ������� ����� ������������ ������,
	* btimedepend- ������������ ��� �������������� ������ ?
	* speedoldtimestep[VX,VY,VZ][iP,iG] - ������ ��������� � ����������� ���������� ����,
	* VGoldtimestep - �������� � ���� iG � ����������� ���������� ����,
	* VPoldtimestep - �������� � ���� iP � ����������� ���������� ����,
	* mfoldtimestep - �������� ����� � ����������� ���������� ���� �� ����� G ( ������ �� ����� �������� ��� ������ �������� G=E,W,N,S,T,B).
	* dtimestep - ������ ���� �� �������,
	* potent - �������� � �������� ��������,
	* bRhieChowi (internal) - ������������� �� ���������������� �������� ���-��� ��� ���������� ����� ��������� �������.
	* bRhieChowb (border) - ������������� �� ���������������� �������� ���-��� ��� ��������� ����� ��������� �������.
	* ���� bsimplelinearinterpol ����� ������ �� ����� ��������� ������ �������� ������������ �������� �� ����� ��� ������������� �������� ���-���.
	* ����� ����������� ��������� �� ������ �������� ������������� ���������.
	* tau - ���������� �����������.
	* drg - ���������� ����� ������������ �������� iP � iG.
	*/

	integer iG=sosedi[G][iP].iNODE1;
	doublereal VG=0.0, VP=0.0;
	integer VGid=-1;
	switch (G) {
		case ESIDE : case WSIDE : VG=potent[VX][iG]; VP=potent[VX][iP]; VGid=VX; break;
		case NSIDE : case SSIDE :VG=potent[VY][iG]; VP=potent[VY][iP]; VGid=VY; break;
		case TSIDE : case BSIDE : VG=potent[VZ][iG]; VP=potent[VZ][iP]; VGid=VZ; break;
	} // end switch
	

	doublereal ug=0.0; // �������� �� ����� ������������ ������.
	doublereal Fg=0.0; // �������� ����� �� ����� ������������ ������.

	if (bsimplelinearinterpol) {

		// ����� ������� �������� ������������ �������� �� �����,
		// ������������ �� ������ �������� ������������� �������.
        if (!bG) { 
	          ug=fgplus*VG+(1.0-fgplus)*VP;
	    }
	    else {
	          ug=VG;
	    }

		// ������������ ����� ����� ����� ��.
	    Fg=rhog*ug*dS;
	}
	else
	{

		// ������� ����������� � ��� ��������� ����� ����.
        doublereal taug=tau[VGid][iP]*tau[VGid][iG]/(fgplus*tau[VGid][iG]+(1.0-fgplus)*tau[VGid][iP]); // ����������� �� ����� ��.
		//28.07.2016
		if (bG) {
			taug = tau[VGid][iG];
		}
	    doublereal gradP=0.0, gradG=0.0;
	    switch(G) {
	       case ESIDE : gradP=potent[GRADXPRESS][iP]; gradG=potent[GRADXPRESS][sosedi[ESIDE][iP].iNODE1]; break;
	       case WSIDE : gradP=potent[GRADXPRESS][iP]; gradG=potent[GRADXPRESS][sosedi[WSIDE][iP].iNODE1]; break;
	       case NSIDE : gradP=potent[GRADYPRESS][iP]; gradG=potent[GRADYPRESS][sosedi[NSIDE][iP].iNODE1]; break;
	       case SSIDE :gradP=potent[GRADYPRESS][iP]; gradG=potent[GRADYPRESS][sosedi[SSIDE][iP].iNODE1]; break;
	       case TSIDE : gradP=potent[GRADZPRESS][iP]; gradG=potent[GRADZPRESS][sosedi[TSIDE][iP].iNODE1]; break;
	       case BSIDE : gradP=potent[GRADZPRESS][iP]; gradG=potent[GRADZPRESS][sosedi[BSIDE][iP].iNODE1]; break;
	    }


	    // ��� �������� ���� ��� ���� ����� �������� ��������������
	    // ������ ���������� ��� ��� ��� �������� � I. Sezai.
	    if (!bG) { 
	       ug=fgplus*VG+(1.0-fgplus)*VP;
	       if (bRhieChowi) {
	           //ug+=RCh*ugRhieChow_internal(iP, G, alpha, nvtx, sosedi, maxelm, pressure, pa, diag_coef); // ����� �������� ���-���

			   
			   // ������ ���� ��� ������ �����������: 
			   //ug+=RCh*(taug/rhog)*(-(potent[PRESS][iG]-potent[PRESS][iP])/drg+(1.0-fgplus)*gradP+fgplus*gradG);
			   // �������������� ������� ������������ ��� ��� ����������� ����������� �������� �������� �� ����� potent[PRESS][iG]-potent[PRESS][iP].
			   // ������ (potent[PRESS][iG]-potent[PRESS][iP]) ����� ������ ��� ����������� E, N , T. �� ������� ��� WSIDE, S, B.

			   // ���������� ������� �������� ���-���.
			   switch (G) {// ����� �������� ���-���
			      case ESIDE : case NSIDE : case TSIDE : ug+=RCh*(taug/rhog)*(-(potent[PRESS][iG]-potent[PRESS][iP])/drg+(1.0-fgplus)*gradP+fgplus*gradG); 
				                           break;
			      case WSIDE : case SSIDE :case BSIDE : ug+=RCh*(taug/rhog)*(-(potent[PRESS][iP]-potent[PRESS][iG])/drg+(1.0-fgplus)*gradP+fgplus*gradG);
				                           break;
			   }
			   
			   
	       }
	    }
	    else {
	       ug=VG;
	       
	       // �������� ��� ���������� ���� ����. �.�. ������� � �������.
		   // �� � ������ ������� ��� ������������ ������������ ���������� ���������� ������ �������� �������.
	       if (bRhieChowb) {
	          //set additional ug+=0.0;
	          //ug+=RCh*ugRhieChow_internal_border(iP, G, alpha, nvtx, sosedi, maxelm, pressure, pa, diag_coef); // ����� �������� ���-���

			   // � ������ ������ drg==0.5*dr; fgplus==0.5;

              // ���������� ������� �������� ���-���.
			   switch (G) {// ����� �������� ���-���
			      case ESIDE : case NSIDE : case TSIDE : ug+=RCh*(taug/rhog)*(-(potent[PRESS][iG]-potent[PRESS][iP])/drg+(1.0-fgplus)*gradP+fgplus*gradG); 
				                           break;
			      case WSIDE : case SSIDE :case BSIDE : ug+=RCh*(taug/rhog)*(-(potent[PRESS][iP]-potent[PRESS][iG])/drg+(1.0-fgplus)*gradP+fgplus*gradG);
				                           break;
			   }

	       }
	       
	    }

	    // ������������ ����� ����� ����� ��.
	    Fg=rhog*ug*dS;

 	    // �������� ����� �������������� ��� ��������� ������ ���������.
	    if (btimedepend) {
			// � �������������� ������ � �������� ���-��� ��������� ������� �������.

		    doublereal VGoldtimestep=0.0;
		    doublereal VPoldtimestep=0.0;

		    switch (G) {
		       case ESIDE : case WSIDE : VPoldtimestep=speedoldtimestep[VX][iP];
			                     VGoldtimestep=speedoldtimestep[VX][iG];
			                     break;
		       case NSIDE : case SSIDE :VPoldtimestep=speedoldtimestep[VY][iP];
			                     VGoldtimestep=speedoldtimestep[VY][iG];
			                     break;
		       case TSIDE :  case BSIDE : VPoldtimestep=speedoldtimestep[VZ][iP];
			                      VGoldtimestep=speedoldtimestep[VZ][iG];
			                     break;
		    }

		    // �������� �� ����� �� � ����������� ���������� ����:
		    doublereal ugold=fgplus*VGoldtimestep+(1.0-fgplus)*VPoldtimestep; 


		    // ������������ ����������� ������� ������ ���� ���� �� ������� ������������ ������.
		    if (!bG) {
				if (bRhieChowi) {
			       // ���������� ����������� �����
		           Fg+=(taug/dtimestep)*(mfoldtimestep[G]-rhog*ugold*dS);
				}
		    }
		    else {
				if (bRhieChowb) {
			       // ��������� ����������� �����
			       Fg+=(taug/dtimestep)*(mfoldtimestep[G]-rhog*ugold*dS);
				}
		    }

    	}
	}

	return Fg;

} // calcFg3


// ���������� ����������������� �������� �����.
// ���������������� �������� ����� mf ����������� �� ������ �������������
// ����������������� �������� � ��������, � ����� ���������� ������������ 
// ������������� ������� ���� ��� ��������� ��������.
void return_calc_correct_mass_flux(integer iP, doublereal** potent, TOCHKA* pa, doublereal** prop, doublereal** prop_b,
	integer** nvtx, ALICE_PARTITION** sosedi, integer maxelm, doublereal **diag_coef,
						doublereal* alpha, doublereal RCh,
						bool btimedepend, doublereal dtimestep, doublereal* mfoldtimestep,
						doublereal* &mfcurrentretune, doublereal** speedoldtimestep, bool bsimplelinearinterpol,
						doublereal** SpeedCorOld, doublereal *mfold)
{

	// SpeedCorOld - ���������������� �������� �� ���������� ��������.

	// ���� bsimplelinearinterpol ����� true �� ����������� ������� �������� ������������ �������� �� ����� ������������ ������.

	// ��-�������� ����� ����� �������� �������� ���-��� ������ �� ���������� �����,
	// ��� ��������� ����� �������� ������ (�� ��������� �������) � ��-�������� �� 
	// ��������� ��������� � ��� ���������������� ��������.

	// ��������� ��� �������� �������� ���-��� 1983�.
	bool bRhieChowi=true, bRhieChowb=false; // i - internal, b - border.

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

	// ��������� ���������������� ������� �������������
	doublereal rhoe, rhow, rhon, rhos, rhot, rhob;
	doublereal rP, rE, rN, rT, rW, rS, rB;

    rP=prop[RHO][iP];
	if (!bE) rE=prop[RHO][iE]; else rE=prop_b[RHO][iE-maxelm];
    if (!bN) rN=prop[RHO][iN]; else rN=prop_b[RHO][iN-maxelm];
    if (!bT) rT=prop[RHO][iT]; else rT=prop_b[RHO][iT-maxelm];
	if (!bW) rW=prop[RHO][iW]; else rW=prop_b[RHO][iW-maxelm];
    if (!bS) rS=prop[RHO][iS]; else rS=prop_b[RHO][iS-maxelm];
    if (!bB) rB=prop[RHO][iB]; else rB=prop_b[RHO][iB-maxelm];

	// ������������ ��������� ������� ���, ����� ����������� 
	// ���������� �����������.
	if (!bE) rhoe=rE*rP/(feplus*rE+(1.0-feplus)*rP); else rhoe=rE;
	if (!bW) rhow=rW*rP/(fwplus*rW+(1.0-fwplus)*rP); else rhow=rW;
	if (!bN) rhon=rN*rP/(fnplus*rN+(1.0-fnplus)*rP); else rhon=rN;
	if (!bS) rhos=rS*rP/(fsplus*rS+(1.0-fsplus)*rP); else rhos=rS;
    if (!bT) rhot=rT*rP/(ftplus*rT+(1.0-ftplus)*rP); else rhot=rT;
	if (!bB) rhob=rB*rP/(fbplus*rB+(1.0-fbplus)*rP); else rhob=rB;


	doublereal Fw=0.0, Fe=0.0, Fs=0.0, Fn=0.0, Ft=0.0, Fb=0.0; 

	Fe=calcFg(bE, feplus, potent[VX][iE], potent[VX][iP],  
	          iP, ESIDE, alpha[VX], rhoe, 
		      dy*dz, dx*dy*dz, btimedepend, 
		      speedoldtimestep, mfoldtimestep,
		      diag_coef[VX][iE], diag_coef[VX][iP], dtimestep,
		      RCh, nvtx, sosedi, maxelm, 
			  potent[PRESS],  pa, diag_coef,
		      bRhieChowi, bRhieChowb, bsimplelinearinterpol);

	Fw=calcFg(bW, fwplus, potent[VX][iW], potent[VX][iP],  
	          iP, WSIDE, alpha[VX], rhow, 
		      dy*dz, dx*dy*dz, btimedepend, 
		      speedoldtimestep, mfoldtimestep,
		      diag_coef[VX][iW], diag_coef[VX][iP], dtimestep,
		      RCh, nvtx, sosedi, maxelm, 
			  potent[PRESS],  pa, diag_coef,
		      bRhieChowi, bRhieChowb, bsimplelinearinterpol);

	Fn=calcFg(bN, fnplus, potent[VY][iN], potent[VY][iP],  
	          iP, NSIDE, alpha[VY], rhon, 
	          dx*dz, dx*dy*dz, btimedepend, 
		      speedoldtimestep, mfoldtimestep,
		      diag_coef[VY][iN], diag_coef[VY][iP], dtimestep,
		      RCh, nvtx, sosedi, maxelm, 
			  potent[PRESS],  pa, diag_coef,
		      bRhieChowi, bRhieChowb, bsimplelinearinterpol);

	Fs=calcFg(bS, fsplus, potent[VY][iS], potent[VY][iP],  
	          iP, SSIDE, alpha[VY], rhos, 
		      dx*dz, dx*dy*dz, btimedepend, 
		      speedoldtimestep, mfoldtimestep,
		      diag_coef[VY][iS], diag_coef[VY][iP], dtimestep,
		      RCh, nvtx, sosedi, maxelm, 
			  potent[PRESS],  pa, diag_coef,
		      bRhieChowi, bRhieChowb, bsimplelinearinterpol);

    Ft=calcFg(bT, ftplus, potent[VZ][iT], potent[VZ][iP],  
	          iP, TSIDE, alpha[VZ], rhot, 
	          dx*dy, dx*dy*dz, btimedepend, 
		      speedoldtimestep, mfoldtimestep,
		      diag_coef[VZ][iT], diag_coef[VZ][iP], dtimestep,
		      RCh, nvtx, sosedi, maxelm, 
			  potent[PRESS],  pa, diag_coef,
		      bRhieChowi, bRhieChowb, bsimplelinearinterpol);

	Fb=calcFg(bB, fbplus, potent[VZ][iB], potent[VZ][iP],  
	          iP, BSIDE, alpha[VZ], rhob, 
		      dx*dy, dx*dy*dz, btimedepend, 
		      speedoldtimestep, mfoldtimestep,
		      diag_coef[VZ][iB], diag_coef[VZ][iP], dtimestep,
		      RCh, nvtx, sosedi, maxelm, 
			  potent[PRESS],  pa, diag_coef,
		      bRhieChowi, bRhieChowb, bsimplelinearinterpol);

	bool ISezai=true;
	if (ISezai) {

	    doublereal SpeedCorOlde, SpeedCorOldw,  SpeedCorOldn,  SpeedCorOlds,  SpeedCorOldt,  SpeedCorOldb; 
	    if (!bE) { 
		   SpeedCorOlde=feplus*SpeedCorOld[VX][iE]+(1.0-feplus)*SpeedCorOld[VX][iP];
	    }
	    else {
	       SpeedCorOlde=SpeedCorOld[VX][iE];
	    }
	    if (!bN) { 
		   SpeedCorOldn=fnplus*SpeedCorOld[VY][iN]+(1.0-fnplus)*SpeedCorOld[VY][iP];
	    }
	    else {
	       SpeedCorOldn=SpeedCorOld[VY][iN];
	    }
	    if (!bT) { 
		   SpeedCorOldt=ftplus*SpeedCorOld[VZ][iT]+(1.0-ftplus)*SpeedCorOld[VZ][iP];
	    }
	    else {
	       SpeedCorOldt=SpeedCorOld[VZ][iT];
	    }
	    if (!bW) { 
		   SpeedCorOldw=fwplus*SpeedCorOld[VX][iW]+(1.0-fwplus)*SpeedCorOld[VX][iP];
     	}
        else {
	       SpeedCorOldw=SpeedCorOld[VX][iW];
	    }
	    if (!bS) { 
		   SpeedCorOlds=fsplus*SpeedCorOld[VY][iS]+(1.0-fsplus)*SpeedCorOld[VY][iP];
	    }
	    else {
	       SpeedCorOlds=SpeedCorOld[VY][iS];
	    }
	    if (!bB) { 
		   SpeedCorOldb=fbplus*SpeedCorOld[VZ][iB]+(1.0-fbplus)*SpeedCorOld[VZ][iP];
	    }
	    else {
	       SpeedCorOldb=SpeedCorOld[VZ][iB];
	    }

	    // ���������� �������� ������ �� ����� ��.
	    // � ���������� ��������� (�������������� ������ ����������) �� ������ I. Sezai. !!!
	    //mfcurrentretune[ESIDE]=Fe+(1.0-alpha[VX])*(mfcurrentretune[ESIDE]-rhoe*SpeedCorOlde*dy*dz);
	    //mfcurrentretune[NSIDE]=Fn+(1.0-alpha[VY])*(mfcurrentretune[NSIDE]-rhon*SpeedCorOldn*dx*dz);
	    //mfcurrentretune[TSIDE]=Ft+(1.0-alpha[VZ])*(mfcurrentretune[TSIDE]-rhot*SpeedCorOldt*dx*dy);
	    //mfcurrentretune[WSIDE]=Fw+(1.0-alpha[VX])*(mfcurrentretune[WSIDE]-rhow*SpeedCorOldw*dy*dz);
	    //mfcurrentretune[SSIDE]=Fs+(1.0-alpha[VY])*(mfcurrentretune[SSIDE]-rhos*SpeedCorOlds*dx*dz);
	    //mfcurrentretune[BSIDE]=Fb+(1.0-alpha[VZ])*(mfcurrentretune[BSIDE]-rhob*SpeedCorOldb*dx*dy);

	    // mfold - �������� ��������� ������ � ���������� ��������.
        mfcurrentretune[ESIDE]=Fe+(1.0-alpha[VX])*(mfold[ESIDE]-rhoe*SpeedCorOlde*dy*dz);
	    mfcurrentretune[NSIDE]=Fn+(1.0-alpha[VY])*(mfold[NSIDE]-rhon*SpeedCorOldn*dx*dz);
	    mfcurrentretune[TSIDE]=Ft+(1.0-alpha[VZ])*(mfold[TSIDE]-rhot*SpeedCorOldt*dx*dy);
	    mfcurrentretune[WSIDE]=Fw+(1.0-alpha[VX])*(mfold[WSIDE]-rhow*SpeedCorOldw*dy*dz);
	    mfcurrentretune[SSIDE]=Fs+(1.0-alpha[VY])*(mfold[SSIDE]-rhos*SpeedCorOlds*dx*dz);
	    mfcurrentretune[BSIDE]=Fb+(1.0-alpha[VZ])*(mfold[BSIDE]-rhob*SpeedCorOldb*dx*dy);
	}

	if (!ISezai) {
	   mfcurrentretune[ESIDE]=Fe;
	   mfcurrentretune[NSIDE]=Fn;
	   mfcurrentretune[TSIDE]=Ft;
	   mfcurrentretune[WSIDE]=Fw;
	   mfcurrentretune[SSIDE]=Fs;
	   mfcurrentretune[BSIDE]=Fb;
	}

} // return_correct_mass_flux

// ���������� ����������������� �������� �����.
// ���������������� �������� ����� mf ����������� �� ������ �������������
// ����������������� �������� � ��������, � ����� ���������� ������������ 
// ������������� ������� ���� ��� ��������� ��������.
// � ������ ���������� ����������� ���������� ����������� tau.
// ����������� 22 ���� 2012 ����. ������������ �� ���������� ������������� tau.
void return_calc_correct_mass_flux2(integer iP, doublereal** potent, TOCHKA* pa, doublereal** prop, doublereal** prop_b,
	integer** nvtx, ALICE_PARTITION** sosedi, integer maxelm, doublereal* alpha, doublereal RCh,
						bool btimedepend, doublereal dtimestep, doublereal* mfoldtimestep,
						doublereal* &mfcurrentretune, doublereal** speedoldtimestep, bool bsimplelinearinterpol,
						doublereal** SpeedCorOld, doublereal *mfold, doublereal* tau)
{

	// SpeedCorOld - ���������������� �������� �� ���������� ��������.
	// tau - ���������� �����������.

	// ���� bsimplelinearinterpol ����� true �� ����������� ������� �������� ������������ �������� �� ����� ������������ ������.

	// ��-�������� ����� ����� �������� �������� ���-��� ������ �� ���������� �����,
	// ��� ��������� ����� �������� ������ (�� ��������� �������) � ��-�������� �� 
	// ��������� ��������� � ��� ���������������� ��������.

	// ��������� ��� �������� �������� ���-��� 1983�.
	bool bRhieChowi=true, bRhieChowb=false; // i - internal, b - border.

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

	// ��������� ���������������� ������� �������������
	doublereal rhoe, rhow, rhon, rhos, rhot, rhob;
	doublereal rP, rE, rN, rT, rW, rS, rB;

    rP=prop[RHO][iP];
	if (!bE) rE=prop[RHO][iE]; else rE=prop_b[RHO][iE-maxelm];
    if (!bN) rN=prop[RHO][iN]; else rN=prop_b[RHO][iN-maxelm];
    if (!bT) rT=prop[RHO][iT]; else rT=prop_b[RHO][iT-maxelm];
	if (!bW) rW=prop[RHO][iW]; else rW=prop_b[RHO][iW-maxelm];
    if (!bS) rS=prop[RHO][iS]; else rS=prop_b[RHO][iS-maxelm];
    if (!bB) rB=prop[RHO][iB]; else rB=prop_b[RHO][iB-maxelm];

	// ������������ ��������� ������� ���, ����� ����������� 
	// ���������� �����������.
	if (!bE) rhoe=rE*rP/(feplus*rE+(1.0-feplus)*rP); else rhoe=rE;
	if (!bW) rhow=rW*rP/(fwplus*rW+(1.0-fwplus)*rP); else rhow=rW;
	if (!bN) rhon=rN*rP/(fnplus*rN+(1.0-fnplus)*rP); else rhon=rN;
	if (!bS) rhos=rS*rP/(fsplus*rS+(1.0-fsplus)*rP); else rhos=rS;
    if (!bT) rhot=rT*rP/(ftplus*rT+(1.0-ftplus)*rP); else rhot=rT;
	if (!bB) rhob=rB*rP/(fbplus*rB+(1.0-fbplus)*rP); else rhob=rB;


	doublereal Fw=0.0, Fe=0.0, Fs=0.0, Fn=0.0, Ft=0.0, Fb=0.0; 

	Fe=calcFg2(bE, feplus, iP, ESIDE,  rhoe, 
			   dy*dz, btimedepend, 
			   speedoldtimestep,  mfoldtimestep,
		       dtimestep,	 RCh, sosedi, 
			   bRhieChowi, bRhieChowb, 
			   bsimplelinearinterpol, tau, dxe, potent);

	Fw=calcFg2(bW, fwplus, iP, WSIDE, rhow, 
			   dy*dz, btimedepend, 
			   speedoldtimestep, mfoldtimestep,
			   dtimestep,	RCh, sosedi, 
			   bRhieChowi, bRhieChowb, bsimplelinearinterpol,
			   tau, dxw, potent);

	Fn=calcFg2(bN, fnplus, iP, NSIDE, rhon, 
			   dx*dz, btimedepend, 
			   speedoldtimestep, mfoldtimestep,
			   dtimestep, RCh, sosedi, 
			   bRhieChowi, bRhieChowb, bsimplelinearinterpol,
			   tau, dyn, potent);

	Fs=calcFg2(bS, fsplus, iP, SSIDE, rhos, 
			   dx*dz, btimedepend, 
			   speedoldtimestep, mfoldtimestep,
			   dtimestep, RCh, sosedi, 
			   bRhieChowi, bRhieChowb, bsimplelinearinterpol,
			   tau, dys, potent);

	Ft=calcFg2(bT, ftplus, iP, TSIDE, rhot, 
		       dx*dy, btimedepend, 
			   speedoldtimestep, mfoldtimestep,
			   dtimestep,	RCh, sosedi, 
			   bRhieChowi, bRhieChowb, bsimplelinearinterpol,
			   tau, dzt, potent);

	Fb=calcFg2(bB, fbplus, iP, BSIDE, rhob, 
			   dx*dy, btimedepend, 
			   speedoldtimestep, mfoldtimestep,
			   dtimestep,	RCh, sosedi, 
			   bRhieChowi, bRhieChowb, bsimplelinearinterpol,
			   tau, dzb, potent);

	bool ISezai=true;
	if (ISezai) {

	    doublereal SpeedCorOlde, SpeedCorOldw,  SpeedCorOldn,  SpeedCorOlds,  SpeedCorOldt,  SpeedCorOldb; 
	    if (!bE) { 
		   SpeedCorOlde=feplus*SpeedCorOld[VX][iE]+(1.0-feplus)*SpeedCorOld[VX][iP];
	    }
	    else {
	       SpeedCorOlde=SpeedCorOld[VX][iE];
	    }
	    if (!bN) { 
		   SpeedCorOldn=fnplus*SpeedCorOld[VY][iN]+(1.0-fnplus)*SpeedCorOld[VY][iP];
	    }
	    else {
	       SpeedCorOldn=SpeedCorOld[VY][iN];
	    }
	    if (!bT) { 
		   SpeedCorOldt=ftplus*SpeedCorOld[VZ][iT]+(1.0-ftplus)*SpeedCorOld[VZ][iP];
	    }
	    else {
	       SpeedCorOldt=SpeedCorOld[VZ][iT];
	    }
	    if (!bW) { 
		   SpeedCorOldw=fwplus*SpeedCorOld[VX][iW]+(1.0-fwplus)*SpeedCorOld[VX][iP];
     	}
        else {
	       SpeedCorOldw=SpeedCorOld[VX][iW];
	    }
	    if (!bS) { 
		   SpeedCorOlds=fsplus*SpeedCorOld[VY][iS]+(1.0-fsplus)*SpeedCorOld[VY][iP];
	    }
	    else {
	       SpeedCorOlds=SpeedCorOld[VY][iS];
	    }
	    if (!bB) { 
		   SpeedCorOldb=fbplus*SpeedCorOld[VZ][iB]+(1.0-fbplus)*SpeedCorOld[VZ][iP];
	    }
	    else {
	       SpeedCorOldb=SpeedCorOld[VZ][iB];
	    }

	    // ���������� �������� ������ �� ����� ��.
	    // � ���������� ��������� (�������������� ������ ����������) �� ������ I. Sezai. !!!
	   // mfold - �������� ��������� ������ � ���������� ��������.
        mfcurrentretune[ESIDE]=Fe+(1.0-alpha[VX])*(mfold[ESIDE]-rhoe*SpeedCorOlde*dy*dz);
	    mfcurrentretune[NSIDE]=Fn+(1.0-alpha[VY])*(mfold[NSIDE]-rhon*SpeedCorOldn*dx*dz);
	    mfcurrentretune[TSIDE]=Ft+(1.0-alpha[VZ])*(mfold[TSIDE]-rhot*SpeedCorOldt*dx*dy);
	    mfcurrentretune[WSIDE]=Fw+(1.0-alpha[VX])*(mfold[WSIDE]-rhow*SpeedCorOldw*dy*dz);
	    mfcurrentretune[SSIDE]=Fs+(1.0-alpha[VY])*(mfold[SSIDE]-rhos*SpeedCorOlds*dx*dz);
	    mfcurrentretune[BSIDE]=Fb+(1.0-alpha[VZ])*(mfold[BSIDE]-rhob*SpeedCorOldb*dx*dy);
	}

	if (!ISezai) {
	   mfcurrentretune[ESIDE]=Fe;
	   mfcurrentretune[NSIDE]=Fn;
	   mfcurrentretune[TSIDE]=Ft;
	   mfcurrentretune[WSIDE]=Fw;
	   mfcurrentretune[SSIDE]=Fs;
	   mfcurrentretune[BSIDE]=Fb;
	}

} // return_correct_mass_flux2

// ���������� ����������������� �������� �����.
// ���������������� �������� ����� mf ����������� �� ������ �������������
// ����������������� �������� � ��������, � ����� ���������� ������������ 
// ������������� ������� ���� ��� ��������� ��������.
// � ������ ���������� ����������� ���������� ����������� tau.
// ����������� 23 ���� 2012 ����. ������������ �� ���������� ������������� tau.
void return_calc_correct_mass_flux3(integer iP, doublereal** potent, TOCHKA* pa, doublereal** prop, doublereal** prop_b,
	integer** nvtx, ALICE_PARTITION** sosedi, integer maxelm, doublereal* alpha, doublereal RCh,
						bool btimedepend, doublereal dtimestep, doublereal* mfoldtimestep,
						doublereal* &mfcurrentretune, doublereal** speedoldtimestep, bool bsimplelinearinterpol,
						doublereal** SpeedCorOld, doublereal *mfold, doublereal** tau)
{

	// SpeedCorOld - ���������������� �������� �� ���������� ��������.
	// tau - ���������� �����������.

	// ���� bsimplelinearinterpol ����� true �� ����������� ������� �������� ������������ �������� �� ����� ������������ ������.

	// ��-�������� ����� ����� �������� �������� ���-��� ������ �� ���������� �����,
	// ��� ��������� ����� �������� ������ (�� ��������� �������) � ��-�������� �� 
	// ��������� ��������� � ��� ���������������� ��������.

	// ��������� ��� �������� �������� ���-��� 1983�.
	bool bRhieChowi=true, bRhieChowb=false; // i - internal, b - border.

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

	// ��������� ���������������� ������� �������������
	doublereal rhoe, rhow, rhon, rhos, rhot, rhob;
	doublereal rP, rE, rN, rT, rW, rS, rB;

    rP=prop[RHO][iP];
	if (!bE) rE=prop[RHO][iE]; else rE=prop_b[RHO][iE-maxelm];
    if (!bN) rN=prop[RHO][iN]; else rN=prop_b[RHO][iN-maxelm];
    if (!bT) rT=prop[RHO][iT]; else rT=prop_b[RHO][iT-maxelm];
	if (!bW) rW=prop[RHO][iW]; else rW=prop_b[RHO][iW-maxelm];
    if (!bS) rS=prop[RHO][iS]; else rS=prop_b[RHO][iS-maxelm];
    if (!bB) rB=prop[RHO][iB]; else rB=prop_b[RHO][iB-maxelm];

	// ������������ ��������� ������� ���, ����� ����������� 
	// ���������� �����������.
	if (!bE) rhoe=rE*rP/(feplus*rE+(1.0-feplus)*rP); else rhoe=rE;
	if (!bW) rhow=rW*rP/(fwplus*rW+(1.0-fwplus)*rP); else rhow=rW;
	if (!bN) rhon=rN*rP/(fnplus*rN+(1.0-fnplus)*rP); else rhon=rN;
	if (!bS) rhos=rS*rP/(fsplus*rS+(1.0-fsplus)*rP); else rhos=rS;
    if (!bT) rhot=rT*rP/(ftplus*rT+(1.0-ftplus)*rP); else rhot=rT;
	if (!bB) rhob=rB*rP/(fbplus*rB+(1.0-fbplus)*rP); else rhob=rB;


	doublereal Fw=0.0, Fe=0.0, Fs=0.0, Fn=0.0, Ft=0.0, Fb=0.0; 

	Fe=calcFg3(bE, feplus, iP, ESIDE,  rhoe, 
			   dy*dz, btimedepend, 
			   speedoldtimestep,  mfoldtimestep,
		       dtimestep,	 RCh, sosedi, 
			   bRhieChowi, bRhieChowb, 
			   bsimplelinearinterpol, tau, dxe, potent);

	Fw=calcFg3(bW, fwplus, iP, WSIDE, rhow, 
			   dy*dz, btimedepend, 
			   speedoldtimestep, mfoldtimestep,
			   dtimestep,	RCh, sosedi, 
			   bRhieChowi, bRhieChowb, bsimplelinearinterpol,
			   tau, dxw, potent);

	Fn=calcFg3(bN, fnplus, iP, NSIDE, rhon, 
			   dx*dz, btimedepend, 
			   speedoldtimestep, mfoldtimestep,
			   dtimestep, RCh, sosedi, 
			   bRhieChowi, bRhieChowb, bsimplelinearinterpol,
			   tau, dyn, potent);

	Fs=calcFg3(bS, fsplus, iP, SSIDE, rhos, 
			   dx*dz, btimedepend, 
			   speedoldtimestep, mfoldtimestep,
			   dtimestep, RCh, sosedi, 
			   bRhieChowi, bRhieChowb, bsimplelinearinterpol,
			   tau, dys, potent);

	Ft=calcFg3(bT, ftplus, iP, TSIDE, rhot, 
		       dx*dy, btimedepend, 
			   speedoldtimestep, mfoldtimestep,
			   dtimestep,	RCh, sosedi, 
			   bRhieChowi, bRhieChowb, bsimplelinearinterpol,
			   tau, dzt, potent);

	Fb=calcFg3(bB, fbplus, iP, BSIDE, rhob, 
			   dx*dy, btimedepend, 
			   speedoldtimestep, mfoldtimestep,
			   dtimestep,	RCh, sosedi, 
			   bRhieChowi, bRhieChowb, bsimplelinearinterpol,
			   tau, dzb, potent);

	bool ISezai=true; // true
	if (ISezai) {

	    doublereal SpeedCorOlde, SpeedCorOldw,  SpeedCorOldn,  SpeedCorOlds,  SpeedCorOldt,  SpeedCorOldb; 
	    if (!bE) { 
		   SpeedCorOlde=feplus*SpeedCorOld[VX][iE]+(1.0-feplus)*SpeedCorOld[VX][iP];
	    }
	    else {
	       SpeedCorOlde=SpeedCorOld[VX][iE];
	    }
	    if (!bN) { 
		   SpeedCorOldn=fnplus*SpeedCorOld[VY][iN]+(1.0-fnplus)*SpeedCorOld[VY][iP];
	    }
	    else {
	       SpeedCorOldn=SpeedCorOld[VY][iN];
	    }
	    if (!bT) { 
		   SpeedCorOldt=ftplus*SpeedCorOld[VZ][iT]+(1.0-ftplus)*SpeedCorOld[VZ][iP];
	    }
	    else {
	       SpeedCorOldt=SpeedCorOld[VZ][iT];
	    }
	    if (!bW) { 
		   SpeedCorOldw=fwplus*SpeedCorOld[VX][iW]+(1.0-fwplus)*SpeedCorOld[VX][iP];
     	}
        else {
	       SpeedCorOldw=SpeedCorOld[VX][iW];
	    }
	    if (!bS) { 
		   SpeedCorOlds=fsplus*SpeedCorOld[VY][iS]+(1.0-fsplus)*SpeedCorOld[VY][iP];
	    }
	    else {
	       SpeedCorOlds=SpeedCorOld[VY][iS];
	    }
	    if (!bB) { 
		   SpeedCorOldb=fbplus*SpeedCorOld[VZ][iB]+(1.0-fbplus)*SpeedCorOld[VZ][iP];
	    }
	    else {
	       SpeedCorOldb=SpeedCorOld[VZ][iB];
	    }

	    // ���������� �������� ������ �� ����� ��.
	    // � ���������� ��������� (�������������� ������ ����������) �� ������ I. Sezai. !!!
	   // mfold - �������� ��������� ������ � ���������� ��������.
        mfcurrentretune[ESIDE]=Fe+(1.0-alpha[VX])*(mfold[ESIDE]-rhoe*SpeedCorOlde*dy*dz);
	    mfcurrentretune[NSIDE]=Fn+(1.0-alpha[VY])*(mfold[NSIDE]-rhon*SpeedCorOldn*dx*dz);
	    mfcurrentretune[TSIDE]=Ft+(1.0-alpha[VZ])*(mfold[TSIDE]-rhot*SpeedCorOldt*dx*dy);
	    mfcurrentretune[WSIDE]=Fw+(1.0-alpha[VX])*(mfold[WSIDE]-rhow*SpeedCorOldw*dy*dz);
	    mfcurrentretune[SSIDE]=Fs+(1.0-alpha[VY])*(mfold[SSIDE]-rhos*SpeedCorOlds*dx*dz);
	    mfcurrentretune[BSIDE]=Fb+(1.0-alpha[VZ])*(mfold[BSIDE]-rhob*SpeedCorOldb*dx*dy);
	}

	if (!ISezai) {
	   mfcurrentretune[ESIDE]=Fe;
	   mfcurrentretune[NSIDE]=Fn;
	   mfcurrentretune[TSIDE]=Ft;
	   mfcurrentretune[WSIDE]=Fw;
	   mfcurrentretune[SSIDE]=Fs;
	   mfcurrentretune[BSIDE]=Fb;
	}

} // return_correct_mass_flux3



// ���������� ������� ��� ��������� 
// �������� ��������
void my_elmatr_quad_PAm(integer iP, equation3D** &sl, equation3D_bon** &slb,  
						doublereal** potent, TOCHKA* pa, doublereal** prop, doublereal** prop_b,
						integer** nvtx, ALICE_PARTITION** sosedi, integer maxelm, doublereal **diag_coef,
						doublereal* alpha, doublereal dbeta, doublereal** &rhie_chow, doublereal RCh,
						bool btimedepend, doublereal dtimestep, doublereal* mfoldtimestep,
						doublereal* &mfcurrentretune, doublereal** speedoldtimestep) {

    // 31 ����� 2012 �� ���� ������� ������� ����� ������������������ ����. ��� ����� ����� � backup`�� AliceFlowv0_07.

    /* btimedepend - ������������ (false), �������������� (true).
	*  dtimestep - �������� �������� ���� �� �������.
	*  
	*
	*  mfoldtimestep - �������� ����� ����� ������ ����� �� � ����������� ���������� ����. (������������ � ������ ������� ��������).
	*  mfcurrentretune - ������������ ������� �������� ����� ����� ������ ����� ��.
	*  �� ����� 6 ������ : E,N,T,W,S,B.
	*
	* speedoldtimestep[VX or VY or VZ][iP : 0..maxelm+maxbound-1] - ���� �������� � ����������� ���������� ����.
	*
	*/

	bool bhighorder=false;

	doublereal eps=admission; // ��� ��������� ������������� ����.
	// ��������� ��� �������� �������� ���-��� 1983�.
	bool bRhieChowi=true, bRhieChowb=false; // i - internal, b - border

	// ���������� ���� � ��� ������:

    // iP - ����� ������������ ������������ ������
	integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	iE=sosedi[ESIDE][iP].iNODE1; iN=sosedi[NSIDE][iP].iNODE1; iT=sosedi[TSIDE][iP].iNODE1;
	iW=sosedi[WSIDE][iP].iNODE1; iS=sosedi[SSIDE][iP].iNODE1; iB=sosedi[BSIDE][iP].iNODE1;
	sl[PAM][iP].iP=iP;
	sl[PAM][iP].iE=iE; sl[PAM][iP].iN=iN; 
	sl[PAM][iP].iS=iS; sl[PAM][iP].iW=iW;
    sl[PAM][iP].iT=iT; sl[PAM][iP].iB=iB;


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
		

	doublereal apue=1.0, apuw=1.0, apvn=1.0, apvs=1.0, apwt=1.0, apwb=1.0;
	if (!bE) apue=sl[VX][iE].ap*sl[VX][iP].ap/(feplus*sl[VX][iE].ap+(1-feplus)*sl[VX][iP].ap); else apue=slb[VX][iE-maxelm].aw;
	if (!bW) apuw=sl[VX][iW].ap*sl[VX][iP].ap/(fwplus*sl[VX][iW].ap+(1-fwplus)*sl[VX][iP].ap); else apuw=slb[VX][iW-maxelm].aw;
	if (!bN) apvn=sl[VY][iN].ap*sl[VY][iP].ap/(fnplus*sl[VY][iN].ap+(1-fnplus)*sl[VY][iP].ap); else apvn=slb[VY][iN-maxelm].aw;
	if (!bS) apvs=sl[VY][iS].ap*sl[VY][iP].ap/(fsplus*sl[VY][iS].ap+(1-fsplus)*sl[VY][iP].ap); else apvs=slb[VY][iS-maxelm].aw;
	if (!bT) apwt=sl[VZ][iT].ap*sl[VZ][iP].ap/(ftplus*sl[VZ][iT].ap+(1-ftplus)*sl[VZ][iP].ap); else apwt=slb[VZ][iT-maxelm].aw;
	if (!bB) apwb=sl[VZ][iB].ap*sl[VZ][iP].ap/(fbplus*sl[VZ][iB].ap+(1-fbplus)*sl[VZ][iP].ap); else apwb=slb[VZ][iB-maxelm].aw;

	doublereal de=dx, dw=dx, dn=dy, ds=dy, dt=dz, db=dz;
	// ��� ����������� ������ �. ��������, ���� ������
	// ������� ��� ��� ������ ���� ����������� � ����-��
	// ��������� ��� ��������� ��������� ��������.
	//de=dy*dz/apue; dw=dy*dz/apuw; 
	//dn=dx*dz/apvn; ds=dx*dz/apvs;
	//dt=dx*dy/apwt; db=dx*dy/apwb;
	// ��� ����������� ������ � ��������� �������:
	//1. �.�.��������, �.�.����������
	// ���������� ���������� ����� ��� ������� ��������� �����-������
	// � �������� � �������������� ���������. 
	// ��� (���) �������������� ������������� 2005�, ��� 17, ����� 8, ��� 15-30
	// 2. SIMPLE METHOD FOR THE SOLUTION OF INCOMPRESSIBLE FLOWS ON NON-STAGGERED GRIDS
	// I.Sezai - Eastern Mediterranean University. Revised January, 2011.
	// 3. �������� ������ sigma-flow.
	// ����� �������� ����� ������ ���� ����������� � ���� ��� ������������ ��� ��������� ���������.
	if (iSIMPLE_alg==SIMPLE_Carretto) {
		// SIMPLE
		de=alpha[VX]*dy*dz/apue; dw=alpha[VX]*dy*dz/apuw; 
	    dn=alpha[VY]*dx*dz/apvn; ds=alpha[VY]*dx*dz/apvs;
	    dt=alpha[VZ]*dx*dy/apwt; db=alpha[VZ]*dx*dy/apwb;
	}
	if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) {
		// SIMPLEC
		de=alpha[VX]*dy*dz/((1.0-alpha[VX])*apue); dw=alpha[VX]*dy*dz/((1.0-alpha[VX])*apuw); 
	    dn=alpha[VY]*dx*dz/((1.0-alpha[VY])*apvn); ds=alpha[VY]*dx*dz/((1.0-alpha[VY])*apvs);
	    dt=alpha[VZ]*dx*dy/((1.0-alpha[VZ])*apwt); db=alpha[VZ]*dx*dy/((1.0-alpha[VZ])*apwb);
	}

    // ��������� ���������������� ������� �������������
	doublereal rhoe, rhow, rhon, rhos, rhot, rhob;
	doublereal rP, rE, rN, rT, rW, rS, rB;

    rP=prop[RHO][iP];
	if (!bE) rE=prop[RHO][iE]; else rE=prop_b[RHO][iE-maxelm];
    if (!bN) rN=prop[RHO][iN]; else rN=prop_b[RHO][iN-maxelm];
    if (!bT) rT=prop[RHO][iT]; else rT=prop_b[RHO][iT-maxelm];
	if (!bW) rW=prop[RHO][iW]; else rW=prop_b[RHO][iW-maxelm];
    if (!bS) rS=prop[RHO][iS]; else rS=prop_b[RHO][iS-maxelm];
    if (!bB) rB=prop[RHO][iB]; else rB=prop_b[RHO][iB-maxelm];

	// ������������ ��������� ������� ���, ����� ����������� 
	// ���������� �����������.
	if (!bE) rhoe=rE*rP/(feplus*rE+(1.0-feplus)*rP); else rhoe=rE;
	if (!bW) rhow=rW*rP/(fwplus*rW+(1.0-fwplus)*rP); else rhow=rW;
	if (!bN) rhon=rN*rP/(fnplus*rN+(1.0-fnplus)*rP); else rhon=rN;
	if (!bS) rhos=rS*rP/(fsplus*rS+(1.0-fsplus)*rP); else rhos=rS;
    if (!bT) rhot=rT*rP/(ftplus*rT+(1.0-ftplus)*rP); else rhot=rT;
	if (!bB) rhob=rB*rP/(fbplus*rB+(1.0-fbplus)*rP); else rhob=rB;


    // 0.5 - �������� ���������� ��� ��������� (�� �������� ������)
	// ��������� �������� ��������.

	doublereal De, Dw, Ds, Dn, Dt, Db; // ������������ ����� ����� ����� ��.
	

	if (!bE) {
		if (bW) De=dbeta*rhoe*de*dy*dz;
		else De=rhoe*de*dy*dz;
	}
	else De=dbeta*rhoe*de*dy*dz;

	if (!bW) {
		if (bE) Dw=dbeta*rhow*dw*dy*dz;
		else Dw=rhow*dw*dy*dz; 
	}
	else Dw=dbeta*rhow*dw*dy*dz;

	if (!bN) {
		if (bS) Dn=dbeta*rhon*dn*dx*dz;
		else Dn=rhon*dn*dx*dz; 
	}
	else Dn=dbeta*rhon*dn*dx*dz;


	if (!bS) {
		if (bN) Ds=dbeta*rhos*ds*dx*dz;
		else Ds=rhos*ds*dx*dz;
	}
	else Ds=dbeta*rhos*ds*dx*dz;


	if (!bT) {
		if (bB) Dt=dbeta*rhot*dt*dx*dy;
		else Dt=rhot*dt*dx*dy;
	} 
	else Dt=dbeta*rhot*dt*dx*dy;


	if (!bB) {
		if (bT) Db=dbeta*rhob*db*dx*dy;
		else Db=rhob*db*dx*dy;
	}
	else Db=dbeta*rhob*db*dx*dy;

	// ����� ����� ����� ����.
	sl[PAM][iP].ae=De*1.0; // ��� Pe==0.0 �������� fD(0.0, EXP2, true, feplus); ����� ������ 1.0;
	sl[PAM][iP].aw=Dw; // *fD(0.0, EXP2, true, fwplus); ����� ������ 1.0;
	sl[PAM][iP].an=Dn; // *fD(0.0, EXP2, true, fnplus); ����� ������ 1.0;
	sl[PAM][iP].as=Ds; // *fD(0.0, EXP2, true, fsplus); ����� ������ 1.0;
	sl[PAM][iP].at=Dt; // *fD(0.0, EXP2, true, ftplus); ����� ������ 1.0;
	sl[PAM][iP].ab=Db; // *fD(0.0, EXP2, true, fbplus); ����� ������ 1.0;

	sl[PAM][iP].ap=sl[PAM][iP].ae+sl[PAM][iP].aw+sl[PAM][iP].an+sl[PAM][iP].as+sl[PAM][iP].at+sl[PAM][iP].ab;

	doublereal baddDFLUX2=0.0;
	if (bhighorder) {
		// ���� bborder == false �� ���� ������ ����������.
		// ���� bborder == true �� �� ��������� ������ ���������� ����.
		bool bborder=false;
		doublereal myflux=0.0;
		myflux=De*(dxe*DFDXiP(potent[PAM], iP, ESIDE, sosedi, maxelm, nvtx, pa, bborder)-(potent[PAM][iE]-potent[PAM][iP]));
		baddDFLUX2+=myflux;
		myflux=Dw*(-dxw*DFDXiP(potent[PAM], iP, WSIDE, sosedi, maxelm, nvtx, pa, bborder)-(potent[PAM][iW]-potent[PAM][iP]));
	    baddDFLUX2+=myflux;
		myflux=Dn*(dyn*DFDXiP(potent[PAM], iP, NSIDE, sosedi, maxelm, nvtx, pa, bborder)-(potent[PAM][iN]-potent[PAM][iP]));
		baddDFLUX2+=myflux;
        myflux=Ds*(-dys*DFDXiP(potent[PAM], iP, SSIDE, sosedi, maxelm, nvtx, pa, bborder)-(potent[PAM][iS]-potent[PAM][iP]));
	    baddDFLUX2+=myflux;
		myflux=Dt*(dzt*DFDXiP(potent[PAM], iP, TSIDE, sosedi, maxelm, nvtx, pa, bborder)-(potent[PAM][iT]-potent[PAM][iP]));
	    baddDFLUX2+=myflux;
		myflux=Db*(-dzb*DFDXiP(potent[PAM], iP, BSIDE, sosedi, maxelm, nvtx, pa, bborder)-(potent[PAM][iB]-potent[PAM][iP]));
	    baddDFLUX2+=myflux;
	}



    doublereal Fw=0.0, Fe=0.0, Fs=0.0, Fn=0.0, Ft=0.0, Fb=0.0; 
	     
	
	Fe=calcFg(bE, feplus, potent[VX][iE], potent[VX][iP],  
	          iP, ESIDE, alpha[VX], rhoe, 
		      dy*dz, dx*dy*dz, btimedepend, 
		      speedoldtimestep, mfoldtimestep,
		      diag_coef[VX][iE], diag_coef[VX][iP], dtimestep,
		      RCh, nvtx, sosedi, maxelm, 
			  potent[PRESS],  pa, diag_coef,
		      bRhieChowi, bRhieChowb,false);

	Fw=calcFg(bW, fwplus, potent[VX][iW], potent[VX][iP],  
	          iP, WSIDE, alpha[VX], rhow, 
		      dy*dz, dx*dy*dz, btimedepend, 
		      speedoldtimestep, mfoldtimestep,
		      diag_coef[VX][iW], diag_coef[VX][iP], dtimestep,
		      RCh, nvtx, sosedi, maxelm, 
			  potent[PRESS],  pa, diag_coef,
		      bRhieChowi, bRhieChowb,false);

	Fn=calcFg(bN, fnplus, potent[VY][iN], potent[VY][iP],  
	          iP, NSIDE, alpha[VY], rhon, 
		      dx*dz, dx*dy*dz, btimedepend, 
		      speedoldtimestep, mfoldtimestep,
		      diag_coef[VY][iN], diag_coef[VY][iP], dtimestep,
		      RCh, nvtx, sosedi, maxelm, 
			  potent[PRESS],  pa, diag_coef,
		      bRhieChowi, bRhieChowb,false);

	Fs=calcFg(bS, fsplus, potent[VY][iS], potent[VY][iP],  
	          iP, SSIDE, alpha[VY], rhos, 
		      dx*dz, dx*dy*dz, btimedepend, 
		      speedoldtimestep, mfoldtimestep,
		      diag_coef[VY][iS], diag_coef[VY][iP], dtimestep,
		      RCh, nvtx, sosedi, maxelm, 
			  potent[PRESS],  pa, diag_coef,
		      bRhieChowi, bRhieChowb,false);

    Ft=calcFg(bT, ftplus, potent[VZ][iT], potent[VZ][iP],  
	          iP, TSIDE, alpha[VZ], rhot, 
		      dx*dy, dx*dy*dz, btimedepend, 
		      speedoldtimestep, mfoldtimestep,
		      diag_coef[VZ][iT], diag_coef[VZ][iP], dtimestep,
		      RCh, nvtx, sosedi, maxelm, 
			  potent[PRESS],  pa, diag_coef,
		      bRhieChowi, bRhieChowb,false);

	Fb=calcFg(bB, fbplus, potent[VZ][iB], potent[VZ][iP],  
	          iP, BSIDE, alpha[VZ], rhob, 
		      dx*dy, dx*dy*dz, btimedepend, 
		      speedoldtimestep, mfoldtimestep,
		      diag_coef[VZ][iB], diag_coef[VZ][iP], dtimestep,
		      RCh, nvtx, sosedi, maxelm, 
			  potent[PRESS],  pa, diag_coef,
		      bRhieChowi, bRhieChowb,false);

	
	bool ISezai=true;
	if (ISezai) {

		doublereal SpeedCorOlde, SpeedCorOldw,  SpeedCorOldn,  SpeedCorOlds,  SpeedCorOldt,  SpeedCorOldb; 
	    if (!bE) { 
		   SpeedCorOlde=feplus*potent[VXCOR][iE]+(1.0-feplus)*potent[VXCOR][iP];
	    }
	    else {
	       SpeedCorOlde=potent[VXCOR][iE];
	    }
	    if (!bN) { 
		   SpeedCorOldn=fnplus*potent[VYCOR][iN]+(1.0-fnplus)*potent[VYCOR][iP];
	    }
	    else {
	       SpeedCorOldn=potent[VYCOR][iN];
	    }
	    if (!bT) { 
		   SpeedCorOldt=ftplus*potent[VZCOR][iT]+(1.0-ftplus)*potent[VZCOR][iP];
	    }
	    else {
	       SpeedCorOldt=potent[VZCOR][iT];
	    }
	    if (!bW) { 
		   SpeedCorOldw=fwplus*potent[VXCOR][iW]+(1.0-fwplus)*potent[VXCOR][iP];
	    }
	    else {
	       SpeedCorOldw=potent[VXCOR][iW];
	    }
	    if (!bS) { 
		   SpeedCorOlds=fsplus*potent[VYCOR][iS]+(1.0-fsplus)*potent[VYCOR][iP];
	    }
	    else {
	       SpeedCorOlds=potent[VYCOR][iS];
    	}
	    if (!bB) { 
		   SpeedCorOldb=fbplus*potent[VZCOR][iB]+(1.0-fbplus)*potent[VZCOR][iP];
	    }
	    else {
	       SpeedCorOldb=potent[VZCOR][iB];
	    }

	    // ���������� �������� ������ �� ����� ��.
	    // � ���������� ��������� (�������������� ������ ����������) �� ������ I. Sezai. !!!
	    // �������������� �������� �������������� �� ������ ���������������� ��������.
	    // ������ ������ ���� ��������� ����� ������ ���� � ���������� ��������.
	    //mfcurrentretune[ESIDE]=Fe+(1.0-alpha[VX])*(mfcurrentretune[ESIDE]-rhoe*SpeedCorOlde*dy*dz);
	    //mfcurrentretune[NSIDE]=Fn+(1.0-alpha[VY])*(mfcurrentretune[NSIDE]-rhon*SpeedCorOldn*dx*dz);
	    //mfcurrentretune[TSIDE]=Ft+(1.0-alpha[VZ])*(mfcurrentretune[TSIDE]-rhot*SpeedCorOldt*dx*dy);
	    //mfcurrentretune[WSIDE]=Fw+(1.0-alpha[VX])*(mfcurrentretune[WSIDE]-rhow*SpeedCorOldw*dy*dz);
	    //mfcurrentretune[SSIDE]=Fs+(1.0-alpha[VY])*(mfcurrentretune[SSIDE]-rhos*SpeedCorOlds*dx*dz);
	    //mfcurrentretune[BSIDE]=Fb+(1.0-alpha[VZ])*(mfcurrentretune[BSIDE]-rhob*SpeedCorOldb*dx*dy);

		// ���������� �������� ������ �� ����� ��.
	    // � ���������� ��������� (�������������� ������ ����������) �� ������ I. Sezai. !!!
	    // �������������� �������� �������������� �� ������ ���������������� ��������.
	    // ������ ������ ���� ��������� ����� ������ ���� � ���������� ��������.
	    Fe+=(1.0-alpha[VX])*(mfcurrentretune[ESIDE]-rhoe*SpeedCorOlde*dy*dz);
	    Fn+=(1.0-alpha[VY])*(mfcurrentretune[NSIDE]-rhon*SpeedCorOldn*dx*dz);
	    Ft+=(1.0-alpha[VZ])*(mfcurrentretune[TSIDE]-rhot*SpeedCorOldt*dx*dy);
	    Fw+=(1.0-alpha[VX])*(mfcurrentretune[WSIDE]-rhow*SpeedCorOldw*dy*dz);
	    Fs+=(1.0-alpha[VY])*(mfcurrentretune[SSIDE]-rhos*SpeedCorOlds*dx*dz);
	    Fb+=(1.0-alpha[VZ])*(mfcurrentretune[BSIDE]-rhob*SpeedCorOldb*dx*dy);

	} 
	

	
	// ���������� �������� ������ �� ����� ��.
	mfcurrentretune[ESIDE]=Fe;
	mfcurrentretune[NSIDE]=Fn;
	mfcurrentretune[TSIDE]=Ft;
	mfcurrentretune[WSIDE]=Fw;
	mfcurrentretune[SSIDE]=Fs;
	mfcurrentretune[BSIDE]=Fb;
	
	

	sl[PAM][iP].b=Fw-Fe+Fs-Fn+Fb-Ft+baddDFLUX2;


	// ������������� ���� :

	// ��� �������� �������� ���������� ������������� ��������� � SPD ��������.
	
	// ������ ������� �������� �������� ��������� �������:
	// -ab ... -as ... -aw ... +ap ... -ae ... -an ... -at == b

		// 1. ���� ������� �������:
		if ((iE>=maxelm) && (fabs(slb[PAM][iE-maxelm].ai)<eps)) {
			sl[PAM][iP].b+=sl[PAM][iP].ae*slb[PAM][iE-maxelm].b/slb[PAM][iE-maxelm].aw;
			sl[PAM][iP].ae=0.0;
			sl[PAM][iP].iE=-1; // �� ������ � ������� ����.
		}
		if ((iW>=maxelm) && (fabs(slb[PAM][iW-maxelm].ai)<eps)) {
			sl[PAM][iP].b+=sl[PAM][iP].aw*slb[PAM][iW-maxelm].b/slb[PAM][iW-maxelm].aw;
			sl[PAM][iP].aw=0.0;
			sl[PAM][iP].iW=-1; // �� ������ � ������� ����.
		}
		if ((iN>=maxelm) && (fabs(slb[PAM][iN-maxelm].ai)<eps)) {
			sl[PAM][iP].b+=sl[PAM][iP].an*slb[PAM][iN-maxelm].b/slb[PAM][iN-maxelm].aw;
			sl[PAM][iP].an=0.0;
			sl[PAM][iP].iN=-1; // �� ������ � ������� ����.
		}
		if ((iS>=maxelm) && (fabs(slb[PAM][iS-maxelm].ai)<eps)) {
			sl[PAM][iP].b+=sl[PAM][iP].as*slb[PAM][iS-maxelm].b/slb[PAM][iS-maxelm].aw;
			sl[PAM][iP].as=0.0;
			sl[PAM][iP].iS=-1; // �� ������ � ������� ����.
		}
		if ((iT>=maxelm) && (fabs(slb[PAM][iT-maxelm].ai)<eps)) {
			sl[PAM][iP].b+=sl[PAM][iP].at*slb[PAM][iT-maxelm].b/slb[PAM][iT-maxelm].aw;
			sl[PAM][iP].at=0.0;
			sl[PAM][iP].iT=-1; // �� ������ � ������� ����.
		}
		if ((iB>=maxelm) && (fabs(slb[PAM][iB-maxelm].ai)<eps)) {
			sl[PAM][iP].b+=sl[PAM][iP].ab*slb[PAM][iB-maxelm].b/slb[PAM][iB-maxelm].aw;
			sl[PAM][iP].ab=0.0;
			sl[PAM][iP].iB=-1; // �� ������ � ������� ����.
		}


} // my_elmatr_quad_PAm

// ���������� ������� ��� ��������� 
// �������� ��������
// ������ ������ ������� ��� �������� �������� �������� �� ���������� �������������,
// ��� ��� ����������� �������� ������. ���� ���� ���������� ����������� �� �������,
// �� ����� ���� ������ ����� � ��������������� �.�. �������� ������ ������� ��� ������ �������� 
// � ������� ����������� ������ tau (�����������).
// ����������� 22 ���� 2012 ����.
void my_elmatr_quad_PAm2(integer iP, equation3D** &sl, equation3D_bon** &slb,  
						doublereal** potent, TOCHKA* pa, doublereal** prop, doublereal** prop_b,
						integer** nvtx, ALICE_PARTITION** sosedi, integer maxelm,
						doublereal* alpha, doublereal dbeta, doublereal** &rhie_chow, doublereal RCh,
						bool btimedepend, doublereal dtimestep, doublereal* mfoldtimestep,
						doublereal* &mfcurrentretune, doublereal** speedoldtimestep, doublereal* tau) {

    // 31 ����� 2012 �� ���� ������� ������� ����� ������������������ ����. ��� ����� ����� � backup`�� AliceFlowv0_07.

	// tau - ������� ����������� �����������, ��� ������ ������������� �������� � ������������� ���������, � ����-�� ���
	// ����� ������ � ���������������� �������� ���-��� (1983 ����).

    /* btimedepend - ������������ (false), �������������� (true).
	*  dtimestep - �������� �������� ���� �� �������.
	*  
	*
	*  mfoldtimestep - �������� ����� ����� ������ ����� �� � ����������� ���������� ����. (������������ � ������ ������� ��������).
	*  mfcurrentretune - ������������ ������� �������� ����� ����� ������ ����� ��.
	*  �� ����� 6 ������ : E,N,T,W,S,B.
	*
	* speedoldtimestep[VX or VY or VZ][iP : 0..maxelm+maxbound-1] - ���� �������� � ����������� ���������� ����.
	*
	*/

	bool bhighorder=false;

	doublereal eps=admission; // ��� ��������� ������������� ����.
	// ��������� ��� �������� �������� ���-��� 1983�.
	bool bRhieChowi=true, bRhieChowb=false; // i - internal, b - border

	// ���������� ���� � ��� ������:

    // iP - ����� ������������ ������������ ������
	integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	iE=sosedi[ESIDE][iP].iNODE1; iN=sosedi[NSIDE][iP].iNODE1; iT=sosedi[TSIDE][iP].iNODE1;
	iW=sosedi[WSIDE][iP].iNODE1; iS=sosedi[SSIDE][iP].iNODE1; iB=sosedi[BSIDE][iP].iNODE1;
	sl[PAM][iP].iP=iP;
	sl[PAM][iP].iE=iE; sl[PAM][iP].iN=iN; 
	sl[PAM][iP].iS=iS; sl[PAM][iP].iW=iW;
    sl[PAM][iP].iT=iT; sl[PAM][iP].iB=iB;


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
	
	// �������� ������������ �������� (�������������) �� ����� ������������ ������.
	doublereal taue=1.0, tauw=1.0, taun=1.0, taus=1.0, taut=1.0, taub=1.0;
	if (!bE) taue=tau[iE]*tau[iP]/(feplus*tau[iE]+(1-feplus)*tau[iP]); else taue=tau[iE];
	if (!bW) tauw=tau[iW]*tau[iP]/(fwplus*tau[iW]+(1-fwplus)*tau[iP]); else tauw=tau[iW];
	if (!bN) taun=tau[iN]*tau[iP]/(fnplus*tau[iN]+(1-fnplus)*tau[iP]); else taun=tau[iN];
	if (!bS) taus=tau[iS]*tau[iP]/(fsplus*tau[iS]+(1-fsplus)*tau[iP]); else taus=tau[iS];
	if (!bT) taut=tau[iT]*tau[iP]/(ftplus*tau[iT]+(1-ftplus)*tau[iP]); else taut=tau[iT];
	if (!bB) taub=tau[iB]*tau[iP]/(fbplus*tau[iB]+(1-fbplus)*tau[iP]); else taub=tau[iB];

	
	/*
	doublereal de, dw, dn, ds, dt, db;
	// ��� ����������� ������ �. ��������, ���� ������
	// ������� ��� ��� ������ ���� ����������� � ����-��
	// ��������� ��� ��������� ��������� ��������.
	//de=dy*dz/apue; dw=dy*dz/apuw; 
	//dn=dx*dz/apvn; ds=dx*dz/apvs;
	//dt=dx*dy/apwt; db=dx*dy/apwb;
	// ��� ����������� ������ � ��������� �������:
	//1. �.�.��������, �.�.����������
	// ���������� ���������� ����� ��� ������� ��������� �����-������
	// � �������� � �������������� ���������. 
	// ��� (���) �������������� ������������� 2005�, ��� 17, ����� 8, ��� 15-30
	// 2. SIMPLE METHOD FOR THE SOLUTION OF INCOMPRESSIBLE FLOWS ON NON-STAGGERED GRIDS
	// I.Sezai - Eastern Mediterranean University. Revised January, 2011.
	// 3. �������� ������ sigma-flow.
	// ����� �������� ����� ������ ���� ����������� � ���� ��� ������������ ��� ��������� ���������.
	if (iSIMPLE_alg==SIMPLE_Carretto) {
		// SIMPLE
		de=alpha[VX]*dy*dz/apue; dw=alpha[VX]*dy*dz/apuw; 
	    dn=alpha[VY]*dx*dz/apvn; ds=alpha[VY]*dx*dz/apvs;
	    dt=alpha[VZ]*dx*dy/apwt; db=alpha[VZ]*dx*dy/apwb;
	}
	if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) {
		// SIMPLEC
		de=alpha[VX]*dy*dz/((1.0-alpha[VX])*apue); dw=alpha[VX]*dy*dz/((1.0-alpha[VX])*apuw); 
	    dn=alpha[VY]*dx*dz/((1.0-alpha[VY])*apvn); ds=alpha[VY]*dx*dz/((1.0-alpha[VY])*apvs);
	    dt=alpha[VZ]*dx*dy/((1.0-alpha[VZ])*apwt); db=alpha[VZ]*dx*dy/((1.0-alpha[VZ])*apwb);
	}
	*/
    // ��������� ���������������� ������� �������������
	doublereal rhoe, rhow, rhon, rhos, rhot, rhob;
	doublereal rP, rE, rN, rT, rW, rS, rB;

    rP=prop[RHO][iP];
	if (!bE) rE=prop[RHO][iE]; else rE=prop_b[RHO][iE-maxelm];
    if (!bN) rN=prop[RHO][iN]; else rN=prop_b[RHO][iN-maxelm];
    if (!bT) rT=prop[RHO][iT]; else rT=prop_b[RHO][iT-maxelm];
	if (!bW) rW=prop[RHO][iW]; else rW=prop_b[RHO][iW-maxelm];
    if (!bS) rS=prop[RHO][iS]; else rS=prop_b[RHO][iS-maxelm];
    if (!bB) rB=prop[RHO][iB]; else rB=prop_b[RHO][iB-maxelm];

	// ������������ ��������� ������� ���, ����� ����������� 
	// ���������� �����������.
	if (!bE) rhoe=rE*rP/(feplus*rE+(1.0-feplus)*rP); else rhoe=rE;
	if (!bW) rhow=rW*rP/(fwplus*rW+(1.0-fwplus)*rP); else rhow=rW;
	if (!bN) rhon=rN*rP/(fnplus*rN+(1.0-fnplus)*rP); else rhon=rN;
	if (!bS) rhos=rS*rP/(fsplus*rS+(1.0-fsplus)*rP); else rhos=rS;
    if (!bT) rhot=rT*rP/(ftplus*rT+(1.0-ftplus)*rP); else rhot=rT;
	if (!bB) rhob=rB*rP/(fbplus*rB+(1.0-fbplus)*rP); else rhob=rB;


    // 0.8 - SIMPLEC �������� ���������� ��� ��������� (�� �������� ������)
	// ��������� �������� ��������.

	doublereal De, Dw, Ds, Dn, Dt, Db; // ������������ ����� ����� ����� ��.
	
	// ����������� tau �������� ������������� �������� 
	// � ������������� ��������� �� �������� ��������.

	if (!bE) {
		if (bW) De=dbeta*taue*dy*dz/dxe;
		else De=taue*dy*dz/dxe;
	}
	else De=dbeta*taue*dy*dz/dxe;

	if (!bW) {
		if (bE) Dw=dbeta*tauw*dy*dz/dxw;
		else Dw=tauw*dy*dz/dxw; 
	}
	else Dw=dbeta*tauw*dy*dz/dxw;

	if (!bN) {
		if (bS) Dn=dbeta*taun*dx*dz/dyn;
		else Dn=taun*dx*dz/dyn; 
	}
	else Dn=dbeta*taun*dx*dz/dyn;
	
	if (!bS) {
		if (bN) Ds=dbeta*taus*dx*dz/dys;
		else Ds=taus*dx*dz/dys;
	}
	else Ds=dbeta*taus*dx*dz/dys;


	if (!bT) {
		if (bB) Dt=dbeta*taut*dx*dy/dzt;
		else Dt=taut*dx*dy/dzt;
	} 
	else Dt=dbeta*taut*dx*dy/dzt;


	if (!bB) {
		if (bT) Db=dbeta*taub*dx*dy/dzb;
		else Db=taub*dx*dy/dzb;
	}
	else Db=dbeta*taub*dx*dy/dzb;

	
	// ����� ����� ����� ����.
	sl[PAM][iP].ae=De*1.0; // ��� Pe==0.0 �������� fD(0.0, EXP2, true, feplus); ����� ������ 1.0;
	sl[PAM][iP].aw=Dw; // *fD(0.0, EXP2, true, fwplus); ����� ������ 1.0;
	sl[PAM][iP].an=Dn; // *fD(0.0, EXP2, true, fnplus); ����� ������ 1.0;
	sl[PAM][iP].as=Ds; // *fD(0.0, EXP2, true, fsplus); ����� ������ 1.0;
	sl[PAM][iP].at=Dt; // *fD(0.0, EXP2, true, ftplus); ����� ������ 1.0;
	sl[PAM][iP].ab=Db; // *fD(0.0, EXP2, true, fbplus); ����� ������ 1.0;

	sl[PAM][iP].ap=sl[PAM][iP].ae+sl[PAM][iP].aw+sl[PAM][iP].an+sl[PAM][iP].as+sl[PAM][iP].at+sl[PAM][iP].ab;

	doublereal baddDFLUX2=0.0;
	if (bhighorder) {
		// ���� bborder == false �� ���� ������ ����������.
		// ���� bborder == true �� �� ��������� ������ ���������� ����.
		bool bborder=false;
		doublereal myflux=0.0;
		myflux=De*(dxe*DFDXiP(potent[PAM], iP, ESIDE, sosedi, maxelm, nvtx, pa, bborder)-(potent[PAM][iE]-potent[PAM][iP]));
		baddDFLUX2+=myflux;
		myflux=Dw*(-dxw*DFDXiP(potent[PAM], iP, WSIDE, sosedi, maxelm, nvtx, pa, bborder)-(potent[PAM][iW]-potent[PAM][iP]));
	    baddDFLUX2+=myflux;
		myflux=Dn*(dyn*DFDXiP(potent[PAM], iP, NSIDE, sosedi, maxelm, nvtx, pa, bborder)-(potent[PAM][iN]-potent[PAM][iP]));
	    baddDFLUX2+=myflux;
		myflux=Ds*(-dys*DFDXiP(potent[PAM], iP, SSIDE, sosedi, maxelm, nvtx, pa, bborder)-(potent[PAM][iS]-potent[PAM][iP]));
	    baddDFLUX2+=myflux;
		myflux=Dt*(dzt*DFDXiP(potent[PAM], iP, TSIDE, sosedi, maxelm, nvtx, pa, bborder)-(potent[PAM][iT]-potent[PAM][iP]));
	    baddDFLUX2+=myflux;
		myflux=Db*(-dzb*DFDXiP(potent[PAM], iP, BSIDE, sosedi, maxelm, nvtx, pa, bborder)-(potent[PAM][iB]-potent[PAM][iP]));
	    baddDFLUX2+=myflux;
	}



    doublereal Fw=0.0, Fe=0.0, Fs=0.0, Fn=0.0, Ft=0.0, Fb=0.0; 
	     
	Fe=calcFg2(bE, feplus, iP, ESIDE, rhoe, 
			dy*dz, btimedepend, 
			speedoldtimestep, mfoldtimestep,
			dtimestep,	RCh, sosedi, 
			bRhieChowi, bRhieChowb, false,
			tau, dxe, potent);

	Fw=calcFg2(bW, fwplus, iP, WSIDE, rhow, 
			dy*dz, btimedepend, 
			speedoldtimestep, mfoldtimestep,
			dtimestep,	RCh, sosedi, 
			bRhieChowi, bRhieChowb, false,
			tau, dxw, potent);

	Fn=calcFg2(bN, fnplus, iP, NSIDE, rhon, 
			dx*dz, btimedepend, 
			speedoldtimestep, mfoldtimestep,
			dtimestep,	RCh, sosedi, 
			bRhieChowi, bRhieChowb, false,
			tau, dyn, potent);

	Fs=calcFg2(bS, fsplus, iP, SSIDE, rhos, 
			dx*dz, btimedepend, 
			speedoldtimestep, mfoldtimestep,
			dtimestep,	RCh, sosedi, 
			bRhieChowi, bRhieChowb, false,
			tau, dys, potent);

	Ft=calcFg2(bT, ftplus, iP, TSIDE, rhot, 
			dx*dy, btimedepend, 
			speedoldtimestep, mfoldtimestep,
			dtimestep,	RCh, sosedi, 
			bRhieChowi, bRhieChowb, false,
			tau, dzt, potent);

	Fb=calcFg2(bB, fbplus, iP, BSIDE, rhob, 
			dx*dy, btimedepend, 
			speedoldtimestep, mfoldtimestep,
			dtimestep,	RCh, sosedi, 
			bRhieChowi, bRhieChowb, false,
			tau, dzb, potent);
	
	bool ISezai=true;
	if (ISezai) {

		doublereal SpeedCorOlde, SpeedCorOldw,  SpeedCorOldn,  SpeedCorOlds,  SpeedCorOldt,  SpeedCorOldb; 
	    if (!bE) { 
		   SpeedCorOlde=feplus*potent[VXCOR][iE]+(1.0-feplus)*potent[VXCOR][iP];
	    }
	    else {
	       SpeedCorOlde=potent[VXCOR][iE];
	    }
	    if (!bN) { 
		   SpeedCorOldn=fnplus*potent[VYCOR][iN]+(1.0-fnplus)*potent[VYCOR][iP];
	    }
	    else {
	       SpeedCorOldn=potent[VYCOR][iN];
	    }
	    if (!bT) { 
		   SpeedCorOldt=ftplus*potent[VZCOR][iT]+(1.0-ftplus)*potent[VZCOR][iP];
	    }
	    else {
	       SpeedCorOldt=potent[VZCOR][iT];
	    }
	    if (!bW) { 
		   SpeedCorOldw=fwplus*potent[VXCOR][iW]+(1.0-fwplus)*potent[VXCOR][iP];
	    }
	    else {
	       SpeedCorOldw=potent[VXCOR][iW];
	    }
	    if (!bS) { 
		   SpeedCorOlds=fsplus*potent[VYCOR][iS]+(1.0-fsplus)*potent[VYCOR][iP];
	    }
	    else {
	       SpeedCorOlds=potent[VYCOR][iS];
    	}
	    if (!bB) { 
		   SpeedCorOldb=fbplus*potent[VZCOR][iB]+(1.0-fbplus)*potent[VZCOR][iP];
	    }
	    else {
	       SpeedCorOldb=potent[VZCOR][iB];
	    }

	    // ���������� �������� ������ �� ����� ��.
	    // � ���������� ��������� (�������������� ������ ����������) �� ������ I. Sezai. !!!
	    // �������������� �������� �������������� �� ������ ���������������� ��������.
	    // ������ ������ ���� ��������� ����� ������ ���� � ���������� ��������.
	    Fe+=(1.0-alpha[VX])*(mfcurrentretune[ESIDE]-rhoe*SpeedCorOlde*dy*dz);
	    Fn+=(1.0-alpha[VY])*(mfcurrentretune[NSIDE]-rhon*SpeedCorOldn*dx*dz);
	    Ft+=(1.0-alpha[VZ])*(mfcurrentretune[TSIDE]-rhot*SpeedCorOldt*dx*dy);
	    Fw+=(1.0-alpha[VX])*(mfcurrentretune[WSIDE]-rhow*SpeedCorOldw*dy*dz);
	    Fs+=(1.0-alpha[VY])*(mfcurrentretune[SSIDE]-rhos*SpeedCorOlds*dx*dz);
	    Fb+=(1.0-alpha[VZ])*(mfcurrentretune[BSIDE]-rhob*SpeedCorOldb*dx*dy);

	} 
	

	
	// ���������� �������� ������ �� ����� ��.
	mfcurrentretune[ESIDE]=Fe;
	mfcurrentretune[NSIDE]=Fn;
	mfcurrentretune[TSIDE]=Ft;
	mfcurrentretune[WSIDE]=Fw;
	mfcurrentretune[SSIDE]=Fs;
	mfcurrentretune[BSIDE]=Fb;
	
	

	sl[PAM][iP].b=Fw-Fe+Fs-Fn+Fb-Ft+baddDFLUX2;



	// ������������� ���� :

	// ��� �������� �������� ���������� ������������� ��������� � SPD ��������.
	
	// ������ ������� �������� �������� ��������� �������:
	// -ab ... -as ... -aw ... +ap ... -ae ... -an ... -at == b

		// 1. ���� ������� �������:
		if ((iE>=maxelm) && (fabs(slb[PAM][iE-maxelm].ai)<eps)) {
			sl[PAM][iP].b+=sl[PAM][iP].ae*slb[PAM][iE-maxelm].b/slb[PAM][iE-maxelm].aw;
			sl[PAM][iP].ae=0.0;
			sl[PAM][iP].iE=-1; // �� ������ � ������� ����.
		}
		if ((iW>=maxelm) && (fabs(slb[PAM][iW-maxelm].ai)<eps)) {
			sl[PAM][iP].b+=sl[PAM][iP].aw*slb[PAM][iW-maxelm].b/slb[PAM][iW-maxelm].aw;
			sl[PAM][iP].aw=0.0;
			sl[PAM][iP].iW=-1; // �� ������ � ������� ����.
		}
		if ((iN>=maxelm) && (fabs(slb[PAM][iN-maxelm].ai)<eps)) {
			sl[PAM][iP].b+=sl[PAM][iP].an*slb[PAM][iN-maxelm].b/slb[PAM][iN-maxelm].aw;
			sl[PAM][iP].an=0.0;
			sl[PAM][iP].iN=-1; // �� ������ � ������� ����.
		}
		if ((iS>=maxelm) && (fabs(slb[PAM][iS-maxelm].ai)<eps)) {
			sl[PAM][iP].b+=sl[PAM][iP].as*slb[PAM][iS-maxelm].b/slb[PAM][iS-maxelm].aw;
			sl[PAM][iP].as=0.0;
			sl[PAM][iP].iS=-1; // �� ������ � ������� ����.
		}
		if ((iT>=maxelm) && (fabs(slb[PAM][iT-maxelm].ai)<eps)) {
			sl[PAM][iP].b+=sl[PAM][iP].at*slb[PAM][iT-maxelm].b/slb[PAM][iT-maxelm].aw;
			sl[PAM][iP].at=0.0;
			sl[PAM][iP].iT=-1; // �� ������ � ������� ����.
		}
		if ((iB>=maxelm) && (fabs(slb[PAM][iB-maxelm].ai)<eps)) {
			sl[PAM][iP].b+=sl[PAM][iP].ab*slb[PAM][iB-maxelm].b/slb[PAM][iB-maxelm].aw;
			sl[PAM][iP].ab=0.0;
			sl[PAM][iP].iB=-1; // �� ������ � ������� ����.
		}

		

} // my_elmatr_quad_PAm2

// ���������� ������� ��� ��������� 
// �������� ��������
// ������ ������ ������� ��� �������� �������� �������� �� ���������� �������������,
// ��� ��� ����������� �������� ������. ���� ���� ���������� ����������� �� �������,
// �� ����� ���� ������ ����� � ��������������� �.�. �������� ������ ������� ��� ������ �������� 
// � ������� ����������� ������ tau (�����������).
// ����������� 23 ���� 2012 ����.
void my_elmatr_quad_PAm3(integer iP, equation3D** &sl, equation3D_bon** &slb,  
						doublereal** potent, TOCHKA* pa, doublereal** prop, doublereal** prop_b,
						integer** nvtx, ALICE_PARTITION** sosedi, integer maxelm,
						doublereal* alpha, doublereal dbeta, doublereal** &rhie_chow, doublereal RCh,
						bool btimedepend, doublereal dtimestep, doublereal* mfoldtimestep,
						doublereal* &mfcurrentretune, doublereal** speedoldtimestep, doublereal** tau,
						bool bmyhighorder, bool bdeltapfinish, bool bRhieChowi, bool bRhieChowb) {

	// �������� ������� ��������� ��� �������� �������� ��������������������, ������� ����� ������ ��������� ���
	// ������ ����������� ��� ���������� ��������. 26 ���� 2012 ����.
	// bdeltapfinish==true. - �������� �������.

    // 31 ����� 2012 �� ���� ������� ������� ����� ������������������ ����. ��� ����� ����� � backup`�� AliceFlowv0_07.

	// tau - ������� ����������� �����������, ��� ������ ������������� �������� � ������������� ���������, � ����-�� ���
	// ����� ������ � ���������������� �������� ���-��� (1983 ����).

    /* btimedepend - ������������ (false), �������������� (true).
	*  dtimestep - �������� �������� ���� �� �������.
	*  
	*
	*  mfoldtimestep - �������� ����� ����� ������ ����� �� � ����������� ���������� ����. (������������ � ������ ������� ��������).
	*  mfcurrentretune - ������������ ������� �������� ����� ����� ������ ����� ��.
	*  �� ����� 6 ������ : E,N,T,W,S,B.
	*
	* speedoldtimestep[VX or VY or VZ][iP : 0..maxelm+maxbound-1] - ���� �������� � ����������� ���������� ����.
	*
	*/

	// �������� ���� bdeltapfinish==true �� ����� ����� �������� bhighorder.
	bool bhighorder=bmyhighorder;

	doublereal eps=admission; // ��� ��������� ������������� ����.
	// ��������� ��� �������� �������� ���-��� 1983�.
	//bool bRhieChowi=true, bRhieChowb=false; // i - internal, b - border ������ ������ ��������� ���������� ������ ������� �����.

	// ���������� ���� � ��� ������:

    // iP - ����� ������������ ������������ ������
	integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	iE=sosedi[ESIDE][iP].iNODE1; iN=sosedi[NSIDE][iP].iNODE1; iT=sosedi[TSIDE][iP].iNODE1;
	iW=sosedi[WSIDE][iP].iNODE1; iS=sosedi[SSIDE][iP].iNODE1; iB=sosedi[BSIDE][iP].iNODE1;
	sl[PAM][iP].iP=iP;
	sl[PAM][iP].iE=iE; sl[PAM][iP].iN=iN; 
	sl[PAM][iP].iS=iS; sl[PAM][iP].iW=iW;
    sl[PAM][iP].iT=iT; sl[PAM][iP].iB=iB;


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
	
	// �������� ������������ �������� (�������������) �� ����� ������������ ������.
	// ����������� ��� ����������� ��� ��� ��������� ����.
	doublereal taue=1.0, tauw=1.0, taun=1.0, taus=1.0, taut=1.0, taub=1.0;
	if (!bE) taue=tau[VX][iE]*tau[VX][iP]/(feplus*tau[VX][iE]+(1.0-feplus)*tau[VX][iP]); else taue=tau[VX][iE]; // ��������� !
	if (!bW) tauw=tau[VX][iW]*tau[VX][iP]/(fwplus*tau[VX][iW]+(1.0-fwplus)*tau[VX][iP]); else tauw=tau[VX][iW];
	if (!bN) taun=tau[VY][iN]*tau[VY][iP]/(fnplus*tau[VY][iN]+(1.0-fnplus)*tau[VY][iP]); else taun=tau[VY][iN];
	if (!bS) taus=tau[VY][iS]*tau[VY][iP]/(fsplus*tau[VY][iS]+(1.0-fsplus)*tau[VY][iP]); else taus=tau[VY][iS];
	if (!bT) taut=tau[VZ][iT]*tau[VZ][iP]/(ftplus*tau[VZ][iT]+(1.0-ftplus)*tau[VZ][iP]); else taut=tau[VZ][iT];
	if (!bB) taub=tau[VZ][iB]*tau[VZ][iP]/(fbplus*tau[VZ][iB]+(1.0-fbplus)*tau[VZ][iP]); else taub=tau[VZ][iB];

	
	/*
	doublereal de, dw, dn, ds, dt, db;
	// ��� ����������� ������ �. ��������, ���� ������
	// ������� ��� ��� ������ ���� ����������� � ����-��
	// ��������� ��� ��������� ��������� ��������.
	//de=dy*dz/apue; dw=dy*dz/apuw; 
	//dn=dx*dz/apvn; ds=dx*dz/apvs;
	//dt=dx*dy/apwt; db=dx*dy/apwb;
	// ��� ����������� ������ � ��������� �������:
	//1. �.�.��������, �.�.����������
	// ���������� ���������� ����� ��� ������� ��������� �����-������
	// � �������� � �������������� ���������. 
	// ��� (���) �������������� ������������� 2005�, ��� 17, ����� 8, ��� 15-30
	// 2. SIMPLE METHOD FOR THE SOLUTION OF INCOMPRESSIBLE FLOWS ON NON-STAGGERED GRIDS
	// I.Sezai - Eastern Mediterranean University. Revised January, 2011.
	// 3. �������� ������ sigma-flow.
	// ����� �������� ����� ������ ���� ����������� � ���� ��� ������������ ��� ��������� ���������.
	if (iSIMPLE_alg==SIMPLE_Carretto) {
		// SIMPLE
		de=alpha[VX]*dy*dz/apue; dw=alpha[VX]*dy*dz/apuw; 
	    dn=alpha[VY]*dx*dz/apvn; ds=alpha[VY]*dx*dz/apvs;
	    dt=alpha[VZ]*dx*dy/apwt; db=alpha[VZ]*dx*dy/apwb;
	}
	if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) {
		// SIMPLEC
		de=alpha[VX]*dy*dz/((1.0-alpha[VX])*apue); dw=alpha[VX]*dy*dz/((1.0-alpha[VX])*apuw); 
	    dn=alpha[VY]*dx*dz/((1.0-alpha[VY])*apvn); ds=alpha[VY]*dx*dz/((1.0-alpha[VY])*apvs);
	    dt=alpha[VZ]*dx*dy/((1.0-alpha[VZ])*apwt); db=alpha[VZ]*dx*dy/((1.0-alpha[VZ])*apwb);
	}
	*/
    // ��������� ���������������� ������� �������������
	doublereal rhoe, rhow, rhon, rhos, rhot, rhob;
	doublereal rP, rE, rN, rT, rW, rS, rB;

    rP=prop[RHO][iP];
	
	if (!bE) rE=prop[RHO][iE]; else rE=prop_b[RHO][iE-maxelm];
    if (!bN) rN=prop[RHO][iN]; else rN=prop_b[RHO][iN-maxelm];
    if (!bT) rT=prop[RHO][iT]; else rT=prop_b[RHO][iT-maxelm];
	if (!bW) rW=prop[RHO][iW]; else rW=prop_b[RHO][iW-maxelm];
    if (!bS) rS=prop[RHO][iS]; else rS=prop_b[RHO][iS-maxelm];
    if (!bB) rB=prop[RHO][iB]; else rB=prop_b[RHO][iB-maxelm];
	// � ��������� ���������������� ������������� ������ ��� ��������� ��� ������������ ������������� 
	// ���������������� � ��������� � ��������� ���������� ����. ������� ����� ����������� �������.
	// ���������������� ���� ����������, ��� ������ ��������� �� ������ �� �� ����������� �� �� continity � ������ ������.
	/* // ���� �� �� ���� ���������� !
	if (!bE) rE=prop[RHO][iE]; else rE=rP;
    if (!bN) rN=prop[RHO][iN]; else rN=rP;
    if (!bT) rT=prop[RHO][iT]; else rT=rP;
	if (!bW) rW=prop[RHO][iW]; else rW=rP;
    if (!bS) rS=prop[RHO][iS]; else rS=rP;
    if (!bB) rB=prop[RHO][iB]; else rB=rP;
	*/

	// ������������ ��������� ������� ���, ����� ����������� 
	// ���������� �����������.
	if (!bE) rhoe=rE*rP/(feplus*rE+(1.0-feplus)*rP); else rhoe=rE; // ��������� !
	if (!bW) rhow=rW*rP/(fwplus*rW+(1.0-fwplus)*rP); else rhow=rW;
	if (!bN) rhon=rN*rP/(fnplus*rN+(1.0-fnplus)*rP); else rhon=rN;
	if (!bS) rhos=rS*rP/(fsplus*rS+(1.0-fsplus)*rP); else rhos=rS;
    if (!bT) rhot=rT*rP/(ftplus*rT+(1.0-ftplus)*rP); else rhot=rT;
	if (!bB) rhob=rB*rP/(fbplus*rB+(1.0-fbplus)*rP); else rhob=rB;


    // 0.8 - SIMPLEC �������� ���������� ��� ��������� (�� �������� ������)
	// ��������� �������� ��������.

	doublereal De, Dw, Ds, Dn, Dt, Db; // ������������ ����� ����� ����� ��.
	
	// ����������� tau �������� ������������� �������� 
	// � ������������� ��������� �� �������� ��������.

	if (!bE) {
		if (bW) De=dbeta*taue*dy*dz/dxe;
		else De=taue*dy*dz/dxe;
	}
	else De=dbeta*taue*dy*dz/dxe;

	if (!bW) {
		if (bE) Dw=dbeta*tauw*dy*dz/dxw;
		else Dw=tauw*dy*dz/dxw; 
	}
	else Dw=dbeta*tauw*dy*dz/dxw;

	if (!bN) {
		if (bS) Dn=dbeta*taun*dx*dz/dyn;
		else Dn=taun*dx*dz/dyn; 
	}
	else Dn=dbeta*taun*dx*dz/dyn;
	
	if (!bS) {
		if (bN) Ds=dbeta*taus*dx*dz/dys;
		else Ds=taus*dx*dz/dys;
	}
	else Ds=dbeta*taus*dx*dz/dys;


	if (!bT) {
		if (bB) Dt=dbeta*taut*dx*dy/dzt;
		else Dt=taut*dx*dy/dzt;
	} 
	else Dt=dbeta*taut*dx*dy/dzt;


	if (!bB) {
		if (bT) Db=dbeta*taub*dx*dy/dzb;
		else Db=taub*dx*dy/dzb;
	}
	else Db=dbeta*taub*dx*dy/dzb;

	
	// ����� ����� ����� ����.
	sl[PAM][iP].ae=De*1.0; // ��� Pe==0.0 �������� fD(0.0, EXP2, true, feplus); ����� ������ 1.0;
	sl[PAM][iP].aw=Dw; // *fD(0.0, EXP2, true, fwplus); ����� ������ 1.0;
	sl[PAM][iP].an=Dn; // *fD(0.0, EXP2, true, fnplus); ����� ������ 1.0;
	sl[PAM][iP].as=Ds; // *fD(0.0, EXP2, true, fsplus); ����� ������ 1.0;
	sl[PAM][iP].at=Dt; // *fD(0.0, EXP2, true, ftplus); ����� ������ 1.0;
	sl[PAM][iP].ab=Db; // *fD(0.0, EXP2, true, fbplus); ����� ������ 1.0;

	sl[PAM][iP].ap=sl[PAM][iP].ae+sl[PAM][iP].aw+sl[PAM][iP].an+sl[PAM][iP].as+sl[PAM][iP].at+sl[PAM][iP].ab;

	doublereal baddDFLUX2=0.0;
	if (0&&bhighorder) {
		// ���� bborder == false �� ���� ������ ����������.
		// ���� bborder == true �� �� ��������� ������ ���������� ����.
		bool bborder=false;
		doublereal myflux=0.0;
		myflux=De*(dxe*DFDXiP(potent[PAM], iP, ESIDE, sosedi, maxelm, nvtx, pa, bborder)-(potent[PAM][iE]-potent[PAM][iP]));
		baddDFLUX2+=myflux;
		myflux=Dw*(-dxw*DFDXiP(potent[PAM], iP, WSIDE, sosedi, maxelm, nvtx, pa, bborder)-(potent[PAM][iW]-potent[PAM][iP]));
	    baddDFLUX2+=myflux;
		myflux=Dn*(dyn*DFDXiP(potent[PAM], iP, NSIDE, sosedi, maxelm, nvtx, pa, bborder)-(potent[PAM][iN]-potent[PAM][iP]));
	    baddDFLUX2+=myflux;
		myflux=Ds*(-dys*DFDXiP(potent[PAM], iP, SSIDE, sosedi, maxelm, nvtx, pa, bborder)-(potent[PAM][iS]-potent[PAM][iP]));
	    baddDFLUX2+=myflux;
		myflux=Dt*(dzt*DFDXiP(potent[PAM], iP, TSIDE, sosedi, maxelm, nvtx, pa, bborder)-(potent[PAM][iT]-potent[PAM][iP]));
	    baddDFLUX2+=myflux;
		myflux=Db*(-dzb*DFDXiP(potent[PAM], iP, BSIDE, sosedi, maxelm, nvtx, pa, bborder)-(potent[PAM][iB]-potent[PAM][iP]));
	    baddDFLUX2+=myflux;
	}



    doublereal Fw=0.0, Fe=0.0, Fs=0.0, Fn=0.0, Ft=0.0, Fb=0.0; 
	     
	Fe=calcFg3(bE, feplus, iP, ESIDE, rhoe, 
			dy*dz, btimedepend, 
			speedoldtimestep, mfoldtimestep,
			dtimestep,	RCh, sosedi, 
			bRhieChowi, bRhieChowb, false,
			tau, dxe, potent);

	Fw=calcFg3(bW, fwplus, iP, WSIDE, rhow, 
			dy*dz, btimedepend, 
			speedoldtimestep, mfoldtimestep,
			dtimestep,	RCh, sosedi, 
			bRhieChowi, bRhieChowb, false,
			tau, dxw, potent);

	Fn=calcFg3(bN, fnplus, iP, NSIDE, rhon, 
			dx*dz, btimedepend, 
			speedoldtimestep, mfoldtimestep,
			dtimestep,	RCh, sosedi, 
			bRhieChowi, bRhieChowb, false,
			tau, dyn, potent);

	Fs=calcFg3(bS, fsplus, iP, SSIDE, rhos, 
			dx*dz, btimedepend, 
			speedoldtimestep, mfoldtimestep,
			dtimestep,	RCh, sosedi, 
			bRhieChowi, bRhieChowb, false,
			tau, dys, potent);

	Ft=calcFg3(bT, ftplus, iP, TSIDE, rhot, 
			dx*dy, btimedepend, 
			speedoldtimestep, mfoldtimestep,
			dtimestep,	RCh, sosedi, 
			bRhieChowi, bRhieChowb, false,
			tau, dzt, potent);

	Fb=calcFg3(bB, fbplus, iP, BSIDE, rhob, 
			dx*dy, btimedepend, 
			speedoldtimestep, mfoldtimestep,
			dtimestep,	RCh, sosedi, 
			bRhieChowi, bRhieChowb, false,
			tau, dzb, potent);
	
	bool ISezai=true;
	if (ISezai) {

		doublereal SpeedCorOlde, SpeedCorOldw,  SpeedCorOldn,  SpeedCorOlds,  SpeedCorOldt,  SpeedCorOldb; 
	    if (!bE) { 
		   SpeedCorOlde=feplus*potent[VXCOR][iE]+(1.0-feplus)*potent[VXCOR][iP];
	    }
	    else {
	       SpeedCorOlde=potent[VXCOR][iE];
	    }
	    if (!bN) { 
		   SpeedCorOldn=fnplus*potent[VYCOR][iN]+(1.0-fnplus)*potent[VYCOR][iP];
	    }
	    else {
	       SpeedCorOldn=potent[VYCOR][iN];
	    }
	    if (!bT) { 
		   SpeedCorOldt=ftplus*potent[VZCOR][iT]+(1.0-ftplus)*potent[VZCOR][iP];
	    }
	    else {
	       SpeedCorOldt=potent[VZCOR][iT];
	    }
	    if (!bW) { 
		   SpeedCorOldw=fwplus*potent[VXCOR][iW]+(1.0-fwplus)*potent[VXCOR][iP];
	    }
	    else {
	       SpeedCorOldw=potent[VXCOR][iW];
	    }
	    if (!bS) { 
		   SpeedCorOlds=fsplus*potent[VYCOR][iS]+(1.0-fsplus)*potent[VYCOR][iP];
	    }
	    else {
	       SpeedCorOlds=potent[VYCOR][iS];
    	}
	    if (!bB) { 
		   SpeedCorOldb=fbplus*potent[VZCOR][iB]+(1.0-fbplus)*potent[VZCOR][iP];
	    }
	    else {
	       SpeedCorOldb=potent[VZCOR][iB];
	    }

	    // ���������� �������� ������ �� ����� ��.
	    // � ���������� ��������� (�������������� ������ ����������) �� ������ I. Sezai. !!!
	    // �������������� �������� �������������� �� ������ ���������������� ��������.
	    // ������ ������ ���� ��������� ����� ������ ���� � ���������� ��������.
	    Fe+=(1.0-alpha[VX])*(mfcurrentretune[ESIDE]-rhoe*SpeedCorOlde*dy*dz);
	    Fn+=(1.0-alpha[VY])*(mfcurrentretune[NSIDE]-rhon*SpeedCorOldn*dx*dz);
	    Ft+=(1.0-alpha[VZ])*(mfcurrentretune[TSIDE]-rhot*SpeedCorOldt*dx*dy);
	    Fw+=(1.0-alpha[VX])*(mfcurrentretune[WSIDE]-rhow*SpeedCorOldw*dy*dz);
	    Fs+=(1.0-alpha[VY])*(mfcurrentretune[SSIDE]-rhos*SpeedCorOlds*dx*dz);
	    Fb+=(1.0-alpha[VZ])*(mfcurrentretune[BSIDE]-rhob*SpeedCorOldb*dx*dy);

	} 
	

	
	// ���������� �������� ������ �� ����� ��.
	if (bdeltapfinish==true) {
	   // ���������� �������� ����� ����� �����.
	   mfcurrentretune[ESIDE]=Fe;
	   mfcurrentretune[NSIDE]=Fn;
	   mfcurrentretune[TSIDE]=Ft;
	   mfcurrentretune[WSIDE]=Fw;
	   mfcurrentretune[SSIDE]=Fs;
	   mfcurrentretune[BSIDE]=Fb;
	}
	
	

	sl[PAM][iP].b=(Fw-Fe+Fs-Fn+Fb-Ft+baddDFLUX2);
	//sl[PAM][iP].b = Fw - Fe + Fs - Fn + Fb - Ft; // 20.07.2016

	
	// ������������� ���� :
	/*
	// ��� �������� �������� ���������� ������������� ��������� � SPD ��������.
	
	// ������ ������� �������� �������� ��������� �������:
	// -ab ... -as ... -aw ... +ap ... -ae ... -an ... -at == b

		// 1. ���� ������� �������:
		if ((iE>=maxelm) && (fabs(slb[PAM][iE-maxelm].ai)<eps)) {
			sl[PAM][iP].b+=sl[PAM][iP].ae*slb[PAM][iE-maxelm].b/slb[PAM][iE-maxelm].aw;
			sl[PAM][iP].ae=0.0;
			sl[PAM][iP].iE=-1; // �� ������ � ������� ����.
		}
		if ((iW>=maxelm) && (fabs(slb[PAM][iW-maxelm].ai)<eps)) {
			sl[PAM][iP].b+=sl[PAM][iP].aw*slb[PAM][iW-maxelm].b/slb[PAM][iW-maxelm].aw;
			sl[PAM][iP].aw=0.0;
			sl[PAM][iP].iW=-1; // �� ������ � ������� ����.
		}
		if ((iN>=maxelm) && (fabs(slb[PAM][iN-maxelm].ai)<eps)) {
			sl[PAM][iP].b+=sl[PAM][iP].an*slb[PAM][iN-maxelm].b/slb[PAM][iN-maxelm].aw;
			sl[PAM][iP].an=0.0;
			sl[PAM][iP].iN=-1; // �� ������ � ������� ����.
		}
		if ((iS>=maxelm) && (fabs(slb[PAM][iS-maxelm].ai)<eps)) {
			sl[PAM][iP].b+=sl[PAM][iP].as*slb[PAM][iS-maxelm].b/slb[PAM][iS-maxelm].aw;
			sl[PAM][iP].as=0.0;
			sl[PAM][iP].iS=-1; // �� ������ � ������� ����.
		}
		if ((iT>=maxelm) && (fabs(slb[PAM][iT-maxelm].ai)<eps)) {
			sl[PAM][iP].b+=sl[PAM][iP].at*slb[PAM][iT-maxelm].b/slb[PAM][iT-maxelm].aw;
			sl[PAM][iP].at=0.0;
			sl[PAM][iP].iT=-1; // �� ������ � ������� ����.
		}
		if ((iB>=maxelm) && (fabs(slb[PAM][iB-maxelm].ai)<eps)) {
			sl[PAM][iP].b+=sl[PAM][iP].ab*slb[PAM][iB-maxelm].b/slb[PAM][iB-maxelm].aw;
			sl[PAM][iP].ab=0.0;
			sl[PAM][iP].iB=-1; // �� ������ � ������� ����.
		}
	
	*/

} // my_elmatr_quad_PAm3

// ���� ������ ������� �������������� �������� �������� �� �������
// ��� ����� ��� �������� ��������� �������� ������ ��������� �������, � 
// ������ ��������������� �������� �� ����������� ����� �� ������� � ������� 
// ������������ ������������.
void free_pressure(FLOW &f) {
	const integer LINEAR_INTERPOL=0;
	const integer QUAD_INTERPOL=1;
	const integer NO_INTERPOL = 2;

	integer imy_interpol = QUAD_INTERPOL;// QUAD_INTERPOL; // LINEAR_INTERPOL QUAD_INTERPOL

		for (integer iP=0; iP<f.maxelm; iP++) {
			// iP - ����� ������������ ������������ ������
	        integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	        iE=f.sosedi[ESIDE][iP].iNODE1; iN=f.sosedi[NSIDE][iP].iNODE1; iT=f.sosedi[TSIDE][iP].iNODE1;
	        iW=f.sosedi[WSIDE][iP].iNODE1; iS=f.sosedi[SSIDE][iP].iNODE1; iB=f.sosedi[BSIDE][iP].iNODE1;
	        

            // ���� � ����� �� ������ ������� ��������� ������� 
	        // �� ���������� ����� true
	        bool bE=false, bN=false, bT=false, bW=false, bS=false, bB=false;

            if (iE>=f.maxelm) bE=true;
	        if (iN>=f.maxelm) bN=true;
	        if (iT>=f.maxelm) bT=true;
            if (iW>=f.maxelm) bW=true;
	        if (iS>=f.maxelm) bS=true;
	        if (iB>=f.maxelm) bB=true;

	        // ���������� �������� �������� ������������ ������:
	        doublereal dx=0.0, dy=0.0, dz=0.0; // ������� ������������ ������
            volume3D(iP, f.nvtx, f.pa, dx, dy, dz);
	    

    	
			// ��������  � ������������ ������������.
            if (bW) {
				// ���� W ���������
				TOCHKA pp,pb,pbb;
				integer iEE=f.sosedi[ESIDE][iE].iNODE1;
				switch (imy_interpol) {
				case LINEAR_INTERPOL : 
		                   center_cord3D(iP, f.nvtx, f.pa, pp,100);
		                   center_cord3D(iE, f.nvtx, f.pa, pb,ESIDE);
		                   f.potent[PAM][iW]=my_linear_interpolation('-', f.potent[PAM][iP], f.potent[PAM][iE], pp.x, pb.x, pp.x-0.5*dx);
						   break;
				case QUAD_INTERPOL :
					        center_cord3D(iP, f.nvtx, f.pa, pp,100);
		                    center_cord3D(iE, f.nvtx, f.pa, pb,ESIDE);
						    center_cord3D(iEE, f.nvtx, f.pa, pbb,EE);
							f.potent[PAM][iW]=my_quadratic_interpolation('-', f.potent[PAM][iEE], f.potent[PAM][iE], f.potent[PAM][iP],
	                            pbb.x , pb.x, pp.x, pp.x-0.5*dx);
					       break;
				case NO_INTERPOL: f.potent[PAM][iW] = f.potent[PAM][iP];
					break;
				default :
					// ���� W ���������
		            center_cord3D(iP, f.nvtx, f.pa, pp,100);
		            center_cord3D(iE, f.nvtx, f.pa, pb,ESIDE);
		            f.potent[PAM][iW]=my_linear_interpolation('-', f.potent[PAM][iP], f.potent[PAM][iE], pp.x, pb.x, pp.x-0.5*dx);
					break;
				}
	        }
	        if (bE) {
		        // ���� E ���������
				TOCHKA pp,pb,pbb;
				integer iWW=f.sosedi[WSIDE][iW].iNODE1;
				switch (imy_interpol) {
				case LINEAR_INTERPOL :
		                  center_cord3D(iP, f.nvtx, f.pa, pp,100);
		                  center_cord3D(iW, f.nvtx, f.pa, pb,WSIDE);
		                  f.potent[PAM][iE]=my_linear_interpolation('+', f.potent[PAM][iP], f.potent[PAM][iW], pp.x, pb.x, pp.x+0.5*dx);
						  break;
				case QUAD_INTERPOL :
					      center_cord3D(iP, f.nvtx, f.pa, pp,100);
		                  center_cord3D(iW, f.nvtx, f.pa, pb,WSIDE);
						  center_cord3D(iWW, f.nvtx, f.pa, pbb,WW);
						  
						  f.potent[PAM][iE]=my_quadratic_interpolation('+', f.potent[PAM][iWW], f.potent[PAM][iW], f.potent[PAM][iP], pbb.x, pb.x, pp.x, pp.x+0.5*dx);	  
					break;
				case NO_INTERPOL: f.potent[PAM][iE] = f.potent[PAM][iP];
					break;
				default : 
					center_cord3D(iP, f.nvtx, f.pa, pp,100);
		            center_cord3D(iW, f.nvtx, f.pa, pb,WSIDE);
		            f.potent[PAM][iE]=my_linear_interpolation('+', f.potent[PAM][iP], f.potent[PAM][iW], pp.x, pb.x, pp.x+0.5*dx);  
					break;
				}
	        }
	        if (bS) {
		        // ���� S ���������
				TOCHKA pp,pb,pbb;
				integer iNN=f.sosedi[NSIDE][iN].iNODE1;
				switch (imy_interpol) {
                case LINEAR_INTERPOL :
		                 center_cord3D(iP, f.nvtx, f.pa, pp,100);
		                 center_cord3D(iN, f.nvtx, f.pa, pb,NSIDE);
		                 f.potent[PAM][iS]=my_linear_interpolation('-', f.potent[PAM][iP], f.potent[PAM][iN], pp.y, pb.y, pp.y-0.5*dy);
				         break;
				case QUAD_INTERPOL :
		                 center_cord3D(iP, f.nvtx, f.pa, pp,100);
		                 center_cord3D(iN, f.nvtx, f.pa, pb,NSIDE);
						 center_cord3D(iNN, f.nvtx, f.pa, pbb,NN);
						 f.potent[PAM][iS]=my_quadratic_interpolation('-', f.potent[PAM][iNN], f.potent[PAM][iN], f.potent[PAM][iP],
	                            pbb.y, pb.y, pp.y, pp.y-0.5*dy);
					break;
				case NO_INTERPOL: f.potent[PAM][iS] = f.potent[PAM][iP];
					break;
				default :
		            center_cord3D(iP, f.nvtx, f.pa, pp,100);
		            center_cord3D(iN, f.nvtx, f.pa, pb,NSIDE);
		            f.potent[PAM][iS]=my_linear_interpolation('-', f.potent[PAM][iP], f.potent[PAM][iN], pp.y, pb.y, pp.y-0.5*dy);
					break;
				}
	        }
	        if (bN) {
		        // ���� N ���������
				TOCHKA pp,pb,pbb;
				integer iSS=f.sosedi[SSIDE][iS].iNODE1;
				switch (imy_interpol) {
					case LINEAR_INTERPOL :
		                 center_cord3D(iP, f.nvtx, f.pa, pp,100);
		                 center_cord3D(iS, f.nvtx, f.pa, pb,SSIDE);
		                 f.potent[PAM][iN]=my_linear_interpolation('+', f.potent[PAM][iP], f.potent[PAM][iS], pp.y, pb.y, pp.y+0.5*dy);
				    break;
					case QUAD_INTERPOL :
		                center_cord3D(iP, f.nvtx, f.pa, pp,100);
		                center_cord3D(iS, f.nvtx, f.pa, pb,SSIDE);
						center_cord3D(iSS, f.nvtx, f.pa, pbb,SS);
						f.potent[PAM][iN]=my_quadratic_interpolation('+', f.potent[PAM][iSS], f.potent[PAM][iS], f.potent[PAM][iP],
	                            pbb.y , pb.y, pp.y, pp.y+0.5*dy);
					break;
					case NO_INTERPOL: f.potent[PAM][iN] = f.potent[PAM][iP];
						break;
					default :
						center_cord3D(iP, f.nvtx, f.pa, pp,100);
		                center_cord3D(iS, f.nvtx, f.pa, pb,SSIDE);
		                f.potent[PAM][iN]=my_linear_interpolation('+', f.potent[PAM][iP], f.potent[PAM][iS], pp.y, pb.y, pp.y+0.5*dy);
					break;
				}
	        }
	       if (bB) {
		        // ���� B ���������
			   TOCHKA pp,pb,pbb;
			   integer iTT=f.sosedi[TSIDE][iT].iNODE1;
				switch (imy_interpol) {
		        case LINEAR_INTERPOL :
		               center_cord3D(iP, f.nvtx, f.pa, pp,100);
		               center_cord3D(iT, f.nvtx, f.pa, pb,TSIDE);
		               f.potent[PAM][iB]=my_linear_interpolation('-', f.potent[PAM][iP], f.potent[PAM][iT], pp.z, pb.z, pp.z-0.5*dz);
				   break;
				case QUAD_INTERPOL :
					center_cord3D(iP, f.nvtx, f.pa, pp,100);
		            center_cord3D(iT, f.nvtx, f.pa, pb,TSIDE);
					center_cord3D(iTT, f.nvtx, f.pa, pbb,TTSIDE);
					f.potent[PAM][iB]=my_quadratic_interpolation('-', f.potent[PAM][iTT], f.potent[PAM][iT], f.potent[PAM][iP],
	                            pbb.z, pb.z, pp.z, pp.z-0.5*dz);

					break;
				case NO_INTERPOL: f.potent[PAM][iB] = f.potent[PAM][iP];
					break;
				default :
					center_cord3D(iP, f.nvtx, f.pa, pp,100);
		            center_cord3D(iT, f.nvtx, f.pa, pb,TSIDE);
		            f.potent[PAM][iB]=my_linear_interpolation('-', f.potent[PAM][iP], f.potent[PAM][iT], pp.z, pb.z, pp.z-0.5*dz);
					break;
				}
	       }
	       if (bT) { 
		        // ���� T ���������
		        TOCHKA pp,pb, pbb;
				integer iBB=f.sosedi[BSIDE][iB].iNODE1;
				switch (imy_interpol) {
				case LINEAR_INTERPOL :
		              center_cord3D(iP, f.nvtx, f.pa, pp,100);
		              center_cord3D(iB, f.nvtx, f.pa, pb,BSIDE);
		              f.potent[PAM][iT]=my_linear_interpolation('+', f.potent[PAM][iP], f.potent[PAM][iB], pp.z, pb.z, pp.z+0.5*dz);
					  break;
                case QUAD_INTERPOL :
					  center_cord3D(iP, f.nvtx, f.pa, pp,100);
		              center_cord3D(iB, f.nvtx, f.pa, pb,BSIDE);
					  center_cord3D(iBB, f.nvtx, f.pa, pbb,BB);
					  f.potent[PAM][iT]=my_quadratic_interpolation('+', f.potent[PAM][iBB], f.potent[PAM][iB], f.potent[PAM][iP],
	                            pbb.z , pb.z, pp.z, pp.z+0.5*dz);
					break;
				case NO_INTERPOL: f.potent[PAM][iT] = f.potent[PAM][iP];
					break;
				default :
					  center_cord3D(iP, f.nvtx, f.pa, pp,100);
		              center_cord3D(iB, f.nvtx, f.pa, pb,BSIDE);
		              f.potent[PAM][iT]=my_linear_interpolation('+', f.potent[PAM][iP], f.potent[PAM][iB], pp.z, pb.z, pp.z+0.5*dz);
					break;
				}
	       }
	
		
	}
} // free_pressure

// �������� �������� ���� ������� �����������.
void correctpressureoutlet(FLOW &f, integer lw, integer ls, WALL* w) {
	// �������������� �������� ������� ��������� �������
	// �� �������.
	free_pressure(f);

	// ����������� �������� �������� ������ ����������� �� �������� ������� ��������� �������.

	doublereal pamout=0.0;
	for (integer iP=0; iP<f.maxelm; iP++) {
			// iP - ����� ������������ ������������ ������
	        integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	        iE=f.sosedi[ESIDE][iP].iNODE1; iN=f.sosedi[NSIDE][iP].iNODE1; iT=f.sosedi[TSIDE][iP].iNODE1;
	        iW=f.sosedi[WSIDE][iP].iNODE1; iS=f.sosedi[SSIDE][iP].iNODE1; iB=f.sosedi[BSIDE][iP].iNODE1;
	        

            // ���� � ����� �� ������ ������� ��������� ������� 
	        // �� ���������� ����� true
	        bool bE=false, bN=false, bT=false, bW=false, bS=false, bB=false;

            if (iE>=f.maxelm) bE=true;
	        if (iN>=f.maxelm) bN=true;
	        if (iT>=f.maxelm) bT=true;
            if (iW>=f.maxelm) bW=true;
	        if (iS>=f.maxelm) bS=true;
	        if (iB>=f.maxelm) bB=true;



	        integer inumber;

			if (bW) {
		       // ���� W ���������
				inumber=iW-f.maxelm;
		       if ((f.sosedb[inumber].MCB<(ls+lw)) && (f.sosedb[inumber].MCB>=ls) && (w[f.sosedb[inumber].MCB-ls].bpressure)) {
				   pamout=fmin(f.potent[PAM][iW],pamout);
			   }
	        }
	        if (bE) {
		        // ���� E ���������
		        inumber=iE-f.maxelm;
				if ((f.sosedb[inumber].MCB<(ls+lw)) && (f.sosedb[inumber].MCB>=ls) && (w[f.sosedb[inumber].MCB-ls].bpressure)) {
				   pamout=fmin(f.potent[PAM][iE],pamout);
			   }
	        }
	        if (bS) {
		        // ���� S ���������
		        inumber=iS-f.maxelm;
                if ((f.sosedb[inumber].MCB<(ls+lw)) && (f.sosedb[inumber].MCB>=ls) && (w[f.sosedb[inumber].MCB-ls].bpressure)) {
				   pamout=fmin(f.potent[PAM][iS],pamout);
			   }
	        }
	        if (bN) {
		        // ���� N ���������
		        inumber=iN-f.maxelm;
				if ((f.sosedb[inumber].MCB<(ls+lw)) && (f.sosedb[inumber].MCB>=ls) && (w[f.sosedb[inumber].MCB-ls].bpressure)) {
				   pamout=fmin(f.potent[PAM][iN],pamout);
			    }
	        }
	       if (bB) {
		        // ���� B ���������
		        inumber=iB-f.maxelm;
				if ((f.sosedb[inumber].MCB<(ls+lw)) && (f.sosedb[inumber].MCB>=ls) && (w[f.sosedb[inumber].MCB-ls].bpressure)) {
				   pamout=fmin(f.potent[PAM][iB],pamout);
			   }
	       }
	       if (bT) { 
		        // ���� T ���������
		        inumber=iT-f.maxelm;
				if ((f.sosedb[inumber].MCB<(ls+lw)) && (f.sosedb[inumber].MCB>=ls) && (w[f.sosedb[inumber].MCB-ls].bpressure)) {
				   pamout=fmin(f.potent[PAM][iT],pamout);
			   }
	       }
	
		
	}

	for (integer i=0; i<(f.maxelm+f.maxbound); i++) f.potent[PAM][i]-=pamout;

} // correctpressureoutlet

// ������������ �������� ������� ��������� ������� �� �������.
// ��� ��������� �� ���������� ������� �������.
void interpolatevel(FLOW &f, integer lw, integer ls, WALL* w, integer iVar) {
	// �������������� �������� ������� ��������� �������
	// �� �������.
	free_pressure(f);

	
	for (integer iP=0; iP<f.maxelm; iP++) {
			// iP - ����� ������������ ������������ ������
	        integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
	        iE=f.sosedi[ESIDE][iP].iNODE1; iN=f.sosedi[NSIDE][iP].iNODE1; iT=f.sosedi[TSIDE][iP].iNODE1;
	        iW=f.sosedi[WSIDE][iP].iNODE1; iS=f.sosedi[SSIDE][iP].iNODE1; iB=f.sosedi[BSIDE][iP].iNODE1;
	        

            // ���� � ����� �� ������ ������� ��������� ������� 
	        // �� ���������� ����� true
	        bool bE=false, bN=false, bT=false, bW=false, bS=false, bB=false;

            if (iE>=f.maxelm) bE=true;
	        if (iN>=f.maxelm) bN=true;
	        if (iT>=f.maxelm) bT=true;
            if (iW>=f.maxelm) bW=true;
	        if (iS>=f.maxelm) bS=true;
	        if (iB>=f.maxelm) bB=true;

			 // ���������� �������� �������� ������������ ������:
	        doublereal dx=0.0, dy=0.0, dz=0.0; // ������� ������������ ������
            volume3D(iP, f.nvtx, f.pa, dx, dy, dz);

	        integer inumber;

			if (bW) {
		       // ���� W ���������
				inumber=iW-f.maxelm;
				if ((f.sosedb[inumber].MCB<(ls + lw)) && (f.sosedb[inumber].MCB >= ls) && ((w[f.sosedb[inumber].MCB - ls].bpressure || w[f.sosedb[inumber].MCB - ls].bopening))) {
				   // �������� ������������
				   TOCHKA pp,pb;
		           center_cord3D(iP, f.nvtx, f.pa, pp,100);
		           center_cord3D(iE, f.nvtx, f.pa, pb,ESIDE);
		           f.potent[iVar][iW]=my_linear_interpolation('-', f.potent[iVar][iP], f.potent[iVar][iE], pp.x, pb.x, pp.x-0.5*dx);
			   }
	        }
	        if (bE) {
		        // ���� E ���������
		        inumber=iE-f.maxelm;
				if ((f.sosedb[inumber].MCB<(ls + lw)) && (f.sosedb[inumber].MCB >= ls) && ((w[f.sosedb[inumber].MCB - ls].bpressure || w[f.sosedb[inumber].MCB - ls].bopening))) {
					// �������� ������������
				    TOCHKA pp,pb;
		            center_cord3D(iP, f.nvtx, f.pa, pp,100);
		            center_cord3D(iW, f.nvtx, f.pa, pb,WSIDE);
		            f.potent[iVar][iE]=my_linear_interpolation('+', f.potent[iVar][iP], f.potent[iVar][iW], pp.x, pb.x, pp.x+0.5*dx);
			   }
	        }
	        if (bS) {
		        // ���� S ���������
		        inumber=iS-f.maxelm;
				if ((f.sosedb[inumber].MCB<(ls + lw)) && (f.sosedb[inumber].MCB >= ls) && ((w[f.sosedb[inumber].MCB - ls].bpressure || w[f.sosedb[inumber].MCB - ls].bopening))) {
				    TOCHKA pp,pb;
		            center_cord3D(iP, f.nvtx, f.pa, pp,100);
		            center_cord3D(iN, f.nvtx, f.pa, pb,NSIDE);
		            f.potent[iVar][iS]=my_linear_interpolation('-', f.potent[iVar][iP], f.potent[iVar][iN], pp.y, pb.y, pp.y-0.5*dy);
			   }
	        }
	        if (bN) {
		        // ���� N ���������
		        inumber=iN-f.maxelm;
				if ((f.sosedb[inumber].MCB<(ls + lw)) && (f.sosedb[inumber].MCB >= ls) && ((w[f.sosedb[inumber].MCB - ls].bpressure || w[f.sosedb[inumber].MCB - ls].bopening))) {
				   TOCHKA pp,pb;
		           center_cord3D(iP, f.nvtx, f.pa, pp,100);
		           center_cord3D(iS, f.nvtx, f.pa, pb,SSIDE);
		           f.potent[iVar][iN]=my_linear_interpolation('+', f.potent[iVar][iP], f.potent[iVar][iS], pp.y, pb.y, pp.y+0.5*dy);
			    }
	        }
	       if (bB) {
		        // ���� B ���������
		        inumber=iB-f.maxelm;
				if ((f.sosedb[inumber].MCB<(ls + lw)) && (f.sosedb[inumber].MCB >= ls) && ((w[f.sosedb[inumber].MCB - ls].bpressure || w[f.sosedb[inumber].MCB - ls].bopening))) {
				   TOCHKA pp,pb;
		           center_cord3D(iP, f.nvtx, f.pa, pp,100);
		           center_cord3D(iT, f.nvtx, f.pa, pb,TSIDE);
		           f.potent[iVar][iB]=my_linear_interpolation('-', f.potent[iVar][iP], f.potent[iVar][iT], pp.z, pb.z, pp.z-0.5*dz);
			   }
	       }
	       if (bT) { 
		        // ���� T ���������
		        inumber=iT-f.maxelm;
				if ((f.sosedb[inumber].MCB<(ls + lw)) && (f.sosedb[inumber].MCB >= ls) && ((w[f.sosedb[inumber].MCB - ls].bpressure ||  w[f.sosedb[inumber].MCB - ls].bopening))) {
				   TOCHKA pp,pb;
		           center_cord3D(iP, f.nvtx, f.pa, pp,100);
		           center_cord3D(iB, f.nvtx, f.pa, pb,BSIDE);
		           f.potent[iVar][iT]=my_linear_interpolation('+', f.potent[iVar][iP], f.potent[iVar][iB], pp.z, pb.z, pp.z+0.5*dz);
			   }
	       }
	
		
	}

} // interpolatevel

#endif