// begin 20 �������� 2011. (��� ����� �������������)
// begin 22 ������� 2011 ���� ��� ������ ����������������.
// begin2 21 ������ 2011 ���� ��� ������ ����������������
// � ����� � ��������� �������� ����������-������� ��������� �����.
// begin 3 end 29, 30 ������� ���������������� ������ �������� �� ��������� OpenMP.
// my_LR.c - ���������� ������������� ������.
// 12 ������ 2013 ����. � ����� � ��������� �� ����� ��������� ����� �� 10��� ����� ����
// ���������� ��� ILU(2) ������������������ �� ����� ����� ��� ���������� ����� �������.
// �� Lr1sk ��������� ����� ������������ � ������ ilu(2) (FlowVision �����������).

// ������������� HEX ��������� ��������� ������������ ������������ ����� ������� ����.
// �� ������������� ������������� ������ ����� �. ��������. ������ ��� �� ����������� ������������.
// � ������ �.�. ������� �� ������������ ���������������� ������������, ��������
// ������� ������������ ��� �� ����������� ������ ������������. �.�. �������� ������������� LR1 ���
// �������� ������� ������� �� 4-5 � ����� ����� �������. ��������� ���������� BiCGStab �������� � ������������
// �� �������� ���������� �� ������ ���� ��� LR1. ����� ����� �.�. ������� �������� � ���� �� ������ ��� � LR1
// �� ���������� � ���� ����� ������ �������� ����������.
// ������������ ��������� ������� ���������� �� ���������� ������� ��������� LR1 � BiCGStab.
// � ������ ����� ����������� LR1.

// 5 ��� 2012 ����.
// �������� ������� � ������:
// ������ ��������� ��� ������������� ���������� ��������� �������:
// A) ��� ��� ����� ����� ���������� � ����� � ��� �� ������ ������.
// B) �� ������� ����, ���� �� ���� �������� � ������ �������� �������.
// C) ���� �� �������������� ���� ������ � ������ ������ ������.
// ��� ������������� ���������� ���� ��� ������� ������� ������� ���������� �������������.
// ����� ��������� ����� ����� ������������ ������ �� ����� � ���-�� ������ ������ ���� � ������ ���� ���.

#ifndef  MY_LR_C
#define  MY_LR_C 1

//#include <math.h>
//#include <omp.h>

// ��� ���������� ������ �������� ������� �������������������� ������� 1.0e-4
// �������� ���������� ������ �� CFX �� �������.
// ���� ����� �������� �� ������� ���������� ����� 1.0e-3.
// �������� ����� �� ������ �� CFX.

// ��������� ����� �������
doublereal NormaV(doublereal *V, integer n){
	doublereal norma;
	doublereal s=0.0;
    doublereal dsize=(doublereal)(1.0*n);

#pragma omp parallel for shared (V,dsize) /*schedule (guided)*/ reduction (+:s)
	for (integer i = 0; i < n; i++) {
		s += V[i] * V[i] / dsize;
	}

	norma=sqrt(s);
	return norma;
} // NormaV

// ������ ���������� � �������� ������ ��� ����������������� �����:
void constr_line(FLOW* &f, integer flow_interior) {

	printf("LR preprocessing FLUID start...\n");

	bool *bmask; // ��� �� ������� ����� ���������� ��.

	// ���� �� ���� flow_interior
	integer i,j; // �������� �����
	for (i=0; i<flow_interior; i++) {

#if doubleintprecision == 1
		printf("LR preprocessing FLUID%lld...\n", i);
#else
		printf("LR preprocessing FLUID%d...\n", i);
#endif

		
		NODELR_BASE *rootWE=NULL;
	    NODELR_BASE *rootSN=NULL;
	    NODELR_BASE *rootBT=NULL;

		// ��� ��������� ������.
	    // ������ ����������� ������� 12 ������ 2013 ���� ��� ��������� ����������,
	    // ��� ��� ���� ���������� �������� ������������������ �� ����� �� 10��� �����.
	    NODELR_BASE *rootWE_end=NULL;
	    NODELR_BASE *rootSN_end=NULL;
	    NODELR_BASE *rootBT_end=NULL;

		bmask=new bool[f[i].maxelm];
		for (j=0; j<f[i].maxelm; j++) bmask[j]=false; // �������������
		integer iline=0; // ����� �������� ����� ������� � ����.
		// ���� �� ���� ���������� ����������� �������:
		for (j=0; j<f[i].maxelm; j++) {
			integer iP=j, idubl=j;
			if (!bmask[iP]) {
				// ���� ���� ��� �� ��� �������
				while (iP < f[i].maxelm) {
					iP=f[i].sosedi[WSIDE][iP].iNODE1;
					if (iP < f[i].maxelm) idubl=iP;
				}
				iP=idubl; // ����� W-�� ���������� �������� ���� � �������� ����� WE
				NODELR_BASE *r1=rootWE;
				integer iN=2; // ��� ��������� ����

				if (r1 != NULL) {
					//while (r1->next !=NULL) {
						//r1=r1->next;
					//}
					r1=rootWE_end;
					r1->next=new NODELR_BASE;
					r1=r1->next;
					r1->ilineid=iline++;
					r1->next=NULL;
					r1->root=NULL;
					rootWE_end=r1;
				}
				else {
					rootWE=new NODELR_BASE;
					rootWE->ilineid=iline++;
					rootWE->next=NULL; // ��������� �������� �����
					rootWE->root=NULL; // ������� �������� �����
					r1=rootWE;
					rootWE_end=r1;
				}
				// r1->root - ��������� �� ����������� ������.
				r1->root=new NODELR;
				NODELR* r2=r1->root;
				while (iP < f[i].maxelm) {
					bmask[iP]=true; // ���� ��� �������.
					iN++;
					r2->id=iP;
					iP = f[i].sosedi[ESIDE][iP].iNODE1;
					if (iP < f[i].maxelm) {
						idubl=iP;
						r2->next=new NODELR;
						r2=r2->next;
					}
					else r2->next=NULL;
				} // while

				r1->iN=iN;

				r1=NULL; r2=NULL; // ����� ���������

			}
		}
		
		rootWE_end=NULL;
		if (bmask != NULL) {
			delete[] bmask;
			bmask = NULL;
		}

		bmask=new bool[f[i].maxelm];
		for (j=0; j<f[i].maxelm; j++) bmask[j]=false; // �������������
	    iline=0; // ����� �������� ����� ������� � ����.
		// ���� �� ���� ���������� ����������� �������:
		for (j=0; j<f[i].maxelm; j++) {
			integer iP=j, idubl=j;
			if (!bmask[iP]) {
				// ���� ���� ��� �� ��� �������
				while (iP < f[i].maxelm) {
					iP = f[i].sosedi[SSIDE][iP].iNODE1;
					if (iP < f[i].maxelm) idubl=iP;
				}
				iP=idubl; // ����� S-�� ���������� �������� ���� � �������� ����� SN
				NODELR_BASE *r1=rootSN;
				integer iN=2; // ��� ��������� ����

				if (r1 != NULL) {
					//while (r1->next !=NULL) {
						//r1=r1->next;
					//}
					r1=rootSN_end;
					r1->next=new NODELR_BASE;
					r1=r1->next;
					r1->ilineid=iline++;
					r1->next=NULL;
					r1->root=NULL;
					rootSN_end=r1;
				}
				else {
					rootSN=new NODELR_BASE;
					rootSN->ilineid=iline++;
					rootSN->next=NULL; // ��������� �������� �����
					rootSN->root=NULL; // ������� �������� �����
					r1=rootSN;
					rootSN_end=r1;
				}
				// r1->root - ��������� �� ����������� ������.
				r1->root=new NODELR;
				NODELR* r2=r1->root;
				while (iP < f[i].maxelm) {
					bmask[iP]=true; // ���� ��� �������.
					iN++;
					r2->id=iP;
					iP = f[i].sosedi[NSIDE][iP].iNODE1;
					if (iP < f[i].maxelm) {
						idubl=iP;
						r2->next=new NODELR;
						r2=r2->next;
					}
					else r2->next=NULL;
				} // while

                r1->iN=iN;   

				r1=NULL; r2=NULL; // ����� ���������

			}
		}
		rootSN_end=NULL;
		delete[] bmask;

		bmask=new bool[f[i].maxelm];
		for (j=0; j<f[i].maxelm; j++) bmask[j]=false; // �������������
		iline=0; // ����� �������� ����� ������� � ����.
		// ���� �� ���� ���������� ����������� �������:
		for (j=0; j<f[i].maxelm; j++) {
			integer iP=j, idubl=j;
			if (!bmask[iP]) {
				// ���� ���� ��� �� ��� �������
				while (iP < f[i].maxelm) {
					iP = f[i].sosedi[BSIDE][iP].iNODE1;
					if (iP < f[i].maxelm) idubl=iP;
				}
				iP=idubl; // ����� S-�� ���������� �������� ���� � �������� ����� SN
				NODELR_BASE *r1=rootBT;
				integer iN=2; // ��� ��������� ����

				if (r1 != NULL) {
					//while (r1->next !=NULL) {
						//r1=r1->next;
					//}
					r1=rootBT_end;
					r1->next=new NODELR_BASE;
					r1=r1->next;
					r1->ilineid=iline++;
					r1->next=NULL;
					r1->root=NULL;
					rootBT_end=r1;
				}
				else {
					rootBT=new NODELR_BASE;
					rootBT->ilineid=iline++;
					rootBT->next=NULL; // ��������� �������� �����
					rootBT->root=NULL; // ������� �������� �����
					r1=rootBT;
					rootBT_end=r1;
				}
				// r1->root - ��������� �� ����������� ������.
				r1->root=new NODELR;
				NODELR* r2=r1->root;
				while (iP < f[i].maxelm) {
					bmask[iP]=true; // ���� ��� �������.
					iN++;
					r2->id=iP;
					iP = f[i].sosedi[TSIDE][iP].iNODE1;
					if (iP < f[i].maxelm) {
						idubl=iP;
						r2->next=new NODELR;
						r2=r2->next;
					}
					else r2->next=NULL;
				} // while

				r1->iN=iN;

				r1=NULL; r2=NULL; // ����� ���������

			}
		}
		rootBT_end=NULL;
		delete[] bmask;


		integer imax=-1;
		NODELR_BASE *r1;
		r1=rootWE;
		while (r1!=NULL) {
			if (r1->iN>imax) imax=r1->iN;
			r1=r1->next;
		}
		r1=rootSN;
		while (r1!=NULL) {
			if (r1->iN>imax) imax=r1->iN;
			r1=r1->next;
		}
		r1=rootBT;
		while (r1!=NULL) {
			if (r1->iN>imax) imax=r1->iN;
			r1=r1->next;
		}

		NODELR_BASE *rootscan=rootWE;
		integer il=0;
		while (rootscan!=NULL) {
			il++;
			rootscan=rootscan->next;
		}
		f[i].iWE=il;

		
		rootscan=rootSN;
	    il=0;
		while (rootscan!=NULL) {
			il++;
			rootscan=rootscan->next;
		}
		f[i].iSN=il;

		
		rootscan=rootBT;
		il=0;
		while (rootscan!=NULL) {
			il++;
			rootscan=rootscan->next;
		}
		f[i].iBT=il;

		f[i].iN=new integer*[3];
		f[i].id=new integer**[3];
		for (integer i1=0; i1<3; i1++) {
			switch (i1) {
			  case 0 : f[i].iN[0]=new integer[f[i].iWE];
				       f[i].id[0]=new integer*[f[i].iWE]; 
					   break;
			  case 1 : f[i].iN[1]=new integer[f[i].iSN]; 
				       f[i].id[1]=new integer*[f[i].iSN];
					   break;
			  case 2 : f[i].iN[2]=new integer[f[i].iBT];
				       f[i].id[2]=new integer*[f[i].iBT]; 
					   break;
			}
		}

		rootscan=rootWE;
		for (integer ic=0; ic<f[i].iWE; ic++) {
			f[i].iN[0][ic]=rootscan->iN;
			f[i].id[0][ic]=new integer[rootscan->iN-2]; // ������ ���������� ���� 
			rootscan=rootscan->next;
		}
		rootscan=rootSN;
		for (integer ic=0; ic<f[i].iSN; ic++) {
			f[i].iN[1][ic]=rootscan->iN;
			f[i].id[1][ic]=new integer[rootscan->iN-2]; // ������ ���������� ����
			rootscan=rootscan->next;
		}
		rootscan=rootBT;
		for (integer ic=0; ic<f[i].iBT; ic++) {
			f[i].iN[2][ic]=rootscan->iN;
			f[i].id[2][ic]=new integer[rootscan->iN-2]; // ������ ���������� ����
			rootscan=rootscan->next;
		}

		
		
		for (integer ic=0; ic<f[i].iWE; ic++) {
			rootscan=rootWE;
			rootWE=rootWE->next;
	
			NODELR* rootlocscan;
			NODELR* rootclean;
			rootlocscan=rootscan->root;
			rootscan->root=NULL; // ����� �����
			for (integer i1=0; i1<rootscan->iN-2; i1++) { // �.�. ��������� ���� ���� ��������� � ����� ���������� ���� ����������
				rootclean=rootlocscan;
#if doubleintprecision == 1
				//printf("%lld %lld \n",rootclean->id, rootclean->next->id); // debug
#else
				//printf("%d %d \n",rootclean->id, rootclean->next->id); // debug
#endif

				
				//getchar();
				f[i].id[0][ic][i1]=rootlocscan->id;
				rootlocscan=rootlocscan->next;
				rootclean->next=NULL;
				delete rootclean;
			}

			rootscan->next=NULL;
			delete rootscan; // ������������ ����������� ������.
		}

		for (integer ic=0; ic<f[i].iSN; ic++) {
			rootscan=rootSN;
			rootSN=rootSN->next;
			
			NODELR* rootlocscan;
			NODELR* rootclean;
			rootlocscan=rootscan->root;
			rootscan->root=NULL; // ����� �����
			for (integer i1=0; i1<rootscan->iN-2; i1++) {
				rootclean=rootlocscan;
				f[i].id[1][ic][i1]=rootlocscan->id;
				rootlocscan=rootlocscan->next;
				rootclean->next=NULL;
				delete rootclean;
			}

			rootscan->next=NULL;
			delete rootscan; // ������������ ����������� ������.
		}

		for (integer ic=0; ic<f[i].iBT; ic++) {
			rootscan=rootBT;
			rootBT=rootBT->next;
			
			NODELR* rootlocscan;
			NODELR* rootclean;
			rootlocscan=rootscan->root;
			rootscan->root=NULL; // ����� �����
			for (integer i1=0; i1<rootscan->iN-2; i1++) {
				rootclean=rootlocscan;
				f[i].id[2][ic][i1]=rootlocscan->id;
				rootlocscan=rootlocscan->next;
				rootclean->next=NULL;
				delete rootclean;
			}

			rootscan->next=NULL;
			delete rootscan; // ������������ ����������� ������.
		}


	} // ����� ����� �� ���� ������ �����

} // constr_line

// ������ ���������� � �������� ������ ��� �������� �����:
void constr_line_temp(TEMPER &t, BLOCK* b, integer lb) {

	printf("LR preprocessing TEMPER start...\n");

	bool *bmask; // ��� �� ������� ����� ���������� ��.

	
	integer j; // ������� �����
	

	bmask=new bool[t.maxelm];
	for (j=0; j<t.maxelm; j++) bmask[j]=false; // �������������
	integer iline=0; // ����� �������� ����� ������� � ����.

	NODELR_BASE *rootWE=NULL;
	NODELR_BASE *rootSN=NULL;
	NODELR_BASE *rootBT=NULL;

	// ��� ��������� ������.
	// ������ ����������� ������� 12 ������ 2013 ���� ��� ��������� ����������,
	// ��� ��� ���� ���������� �������� ������������������ �� ����� �� 10��� �����.
	NODELR_BASE *rootWE_end=NULL;
	NODELR_BASE *rootSN_end=NULL;
	NODELR_BASE *rootBT_end=NULL;

	// � ������ �������� ����� ����������� ���� �������������� ����������
	// ����������� �������.
	// � ��������� ����������� ��� �������� ����� �������� ����� �� ������ �������� �����.
	// �.�. ������ �������� ����� ��������� ����� �� ��� �������� ������������ �������.

	// ���� �� ���� ���������� ����������� �������:
	for (j=0; j<t.maxelm; j++) {
		integer iP=j, idubl=j;
		if (!bmask[iP]) {
			// ���� ���� ��� �� ��� �������
			while (iP < t.maxelm) {
				iP = t.sosedi[WSIDE][iP].iNODE1;
				if (iP < t.maxelm) idubl=iP;
				// ������ ��������� ����� ����� ������������ �.�. ��� ������������� ������������ ������ ������ t.maxelm.
			}
			iP=idubl; // ����� W-�� ���������� �������� ���� � �������� ����� WE
			NODELR_BASE *r1=rootWE;
			integer iN=2; // ��� ��������� ����

			if (r1 != NULL) {
				// ����� ��������� �������� �����.
				//while (r1->next !=NULL) {
					//r1=r1->next;
				//}
				r1=rootWE_end;
				r1->next=new NODELR_BASE;
				r1=r1->next;
				r1->ilineid=iline++;
				r1->next=NULL;
				r1->root=NULL;
				rootWE_end=r1; // ���������� ��������� �������� �����.
			}
			else {
				rootWE=new NODELR_BASE;
				rootWE->ilineid=iline++;
				rootWE->next=NULL; // ��������� �������� �����
				rootWE->root=NULL; // ������� �������� �����
				r1=rootWE;
				rootWE_end=rootWE; // ��������� �� ��������� �������� �����.
			}
			// r1->root - ��������� �� ����������� ������.
			r1->root=new NODELR;
			NODELR* r2=r1->root;
			r2->next = NULL;
			while (iP < t.maxelm) {
				bmask[iP]=true; // ���� ��� �������.
				iN++;
				r2->id=iP;
				iP = t.sosedi[ESIDE][iP].iNODE1;
				if (iP < t.maxelm) {
					//idubl=iP; // ��� ������ TODO
					if (r2->next == NULL) {
						r2->next = new NODELR;
						r2->next->next = NULL; // �������������.
					}
					r2=r2->next;
				}
				else r2->next=NULL;
			} // while

			r1->iN=iN;

			r1=NULL; r2=NULL; // ����� ���������

		}
	}
	rootWE_end=NULL; // ��������������� ��������� ��� ��������� ������.
		
	if (bmask != NULL) {
		delete[] bmask;
		bmask = NULL;
	}

	bmask=new bool[t.maxelm];
	for (j=0; j<t.maxelm; j++) bmask[j]=false; // �������������
	iline=0; // ����� �������� ����� ������� � ����.
	// ���� �� ���� ���������� ����������� �������:
	for (j=0; j<t.maxelm; j++) {
		integer iP=j, idubl=j;
		if (!bmask[iP]) {
			// ���� ���� ��� �� ��� �������
			while (iP < t.maxelm) {
				iP = t.sosedi[SSIDE][iP].iNODE1;
				if (iP < t.maxelm) idubl=iP;
			}
			iP=idubl; // ����� S-�� ���������� �������� ���� � �������� ����� SN
			NODELR_BASE *r1=rootSN;
			integer iN=2; // ��� ��������� ����

			if (r1 != NULL) {
				//while (r1->next !=NULL) {
					//r1=r1->next;
				//}
				r1=rootSN_end;
				r1->next=new NODELR_BASE;
				r1=r1->next;
				r1->ilineid=iline++;
				r1->next=NULL;
				r1->root=NULL;
				rootSN_end=r1;
			}
			else {
				rootSN=new NODELR_BASE;
				rootSN->ilineid=iline++;
				rootSN->next=NULL; // ��������� �������� �����
				rootSN->root=NULL; // ������� �������� �����
				r1=rootSN;
				rootSN_end=rootSN; // ��������� �� ��������� �������� �����.
			}
			// r1->root - ��������� �� ����������� ������.
			r1->root=new NODELR;
			NODELR* r2=r1->root;
			r2->next = NULL;
			while (iP < t.maxelm) {
				bmask[iP]=true; // ���� ��� �������.
				iN++;
				r2->id=iP;
				iP = t.sosedi[NSIDE][iP].iNODE1;
				if (iP < t.maxelm) {
					idubl=iP;
					if (r2->next == NULL) {
						r2->next = new NODELR;
						r2->next->next = NULL;
					}
					r2=r2->next;
				}
				else r2->next=NULL;
			} // while

            r1->iN=iN;   

			r1=NULL; r2=NULL; // ����� ���������

		}
	}

	rootSN_end=NULL;
	if (bmask != NULL) {
		delete[] bmask;
		bmask = NULL;
	}
	

	bmask=new bool[t.maxelm];
	for (j=0; j<t.maxelm; j++) bmask[j]=false; // �������������
	iline=0; // ����� �������� ����� ������� � ����.
	// ���� �� ���� ���������� ����������� �������:
	for (j=0; j<t.maxelm; j++) {
		integer iP=j, idubl=j;
		if (!bmask[iP]) {
			// ���� ���� ��� �� ��� �������
			while (iP < t.maxelm) {
				iP = t.sosedi[BSIDE][iP].iNODE1;
				if (iP < t.maxelm) idubl=iP;
			}
			iP=idubl; // ����� S-�� ���������� �������� ���� � �������� ����� SN
			NODELR_BASE *r1=rootBT;
			integer iN=2; // ��� ��������� ����

			if (r1 != NULL) {
				//while (r1->next !=NULL) {
					//r1=r1->next;
				//}
				r1=rootBT_end; // ���������� ������������.
				r1->next=new NODELR_BASE;
				r1=r1->next;
				r1->ilineid=iline++;
				r1->next=NULL;
				r1->root=NULL;
				rootBT_end=r1;
			}
			else {
				rootBT=new NODELR_BASE;
				rootBT->ilineid=iline++;
				rootBT->next=NULL; // ��������� �������� �����
				rootBT->root=NULL; // ������� �������� �����
				r1=rootBT;
				rootBT_end=rootBT;
			}
			// r1->root - ��������� �� ����������� ������.
			r1->root=new NODELR;
			NODELR* r2=r1->root;
			r2->next = NULL;
			while (iP < t.maxelm) {
				bmask[iP]=true; // ���� ��� �������.
				iN++;
				r2->id=iP;
				iP = t.sosedi[TSIDE][iP].iNODE1;
				if (iP < t.maxelm) {
					idubl=iP;
					if (r2->next == NULL) {
						r2->next = new NODELR;
						r2->next->next = NULL; // �������������.
					}					
					r2=r2->next;
				}
				else r2->next=NULL;
			} // while

			r1->iN=iN;

			r1=NULL; r2=NULL; // ����� ���������

		}
	}
	rootBT_end=NULL;

	delete[] bmask;


	integer imax=-1;
	NODELR_BASE *r1;
	r1=rootWE;
	while (r1!=NULL) {
		if (r1->iN>imax) imax=r1->iN;
		r1=r1->next;
	}
	r1=rootSN;
	while (r1!=NULL) {
		if (r1->iN>imax) imax=r1->iN;
		r1=r1->next;
	}
	r1=rootBT;
	while (r1!=NULL) {
		if (r1->iN>imax) imax=r1->iN;
		r1=r1->next;
	}

	//t.imaxsl=imax;

	/*
	// ���������������� �������
	t.a=new doublereal[imax];
	t.b=new doublereal[imax];
	t.c=new doublereal[imax];
	t.d=new doublereal[imax];
	// ����������� ������������
	t.P=new doublereal[imax];
	t.Q=new doublereal[imax];
	// ���������� ����� ��������� (�����)
	t.ind= new integer[imax];
	*/

	// ���������� � ����� � ������ ����������
	// �������� ��������� �����.

	
    TOCHKA p_c;
	bool bi_fluid;
	integer ib; // ����� �����

	//��� �x
	r1=rootWE;
	while (r1 != NULL) {
		
		// �������������
		r1->bNeimanStart=false;
		r1->bNeimanEnd=false; 
		// ���� ����� ������ true �� ����� � ���������� � ���� ������� ����� ���������.
		// ��� ����� ��� ���������� ���������� �����, ����� � ����� ������� ��������� ��������
		// � � ������ ������ ����.

	    NODELR* r2=r1->root;

		p_c.x=0.5*(t.pa[t.nvtx[0][r2->id]-1].x+t.pa[t.nvtx[1][r2->id]-1].x);
	    p_c.y=0.5*(t.pa[t.nvtx[1][r2->id]-1].y+t.pa[t.nvtx[3][r2->id]-1].y);
	    p_c.z=0.5*(t.pa[t.nvtx[0][r2->id]-1].z+t.pa[t.nvtx[4][r2->id]-1].z);

		// � ib ������������ ����� ����� �������� ����������� ����������� ����� ��.
	    bi_fluid=in_model_flow(p_c,ib,b,lb); // ����������� �� �� ������ ����.

		if ((bi_fluid) && (t.binternalsource[t.sosedi[WSIDE][r2->id].iNODE1 - t.maxelm])) r1->bNeimanStart = true;
		
		while (r2->next!=NULL) r2=r2->next; // ��������� � ����� ������

		p_c.x=0.5*(t.pa[t.nvtx[0][r2->id]-1].x+t.pa[t.nvtx[1][r2->id]-1].x);
	    p_c.y=0.5*(t.pa[t.nvtx[1][r2->id]-1].y+t.pa[t.nvtx[3][r2->id]-1].y);
	    p_c.z=0.5*(t.pa[t.nvtx[0][r2->id]-1].z+t.pa[t.nvtx[4][r2->id]-1].z);

	    bi_fluid=in_model_flow(p_c,ib,b,lb); // ����������� �� �� ������ ����.

		if ((bi_fluid) && (t.binternalsource[t.sosedi[ESIDE][r2->id].iNODE1 - t.maxelm])) r1->bNeimanEnd = true;

		r1=r1->next; // ������� � ��������� ������
	}

	//��� �y
	r1=rootSN;
	while (r1 != NULL) {
		
		// �������������
		r1->bNeimanStart=false;
		r1->bNeimanEnd=false; 

	    NODELR* r2=r1->root;

		p_c.x=0.5*(t.pa[t.nvtx[0][r2->id]-1].x+t.pa[t.nvtx[1][r2->id]-1].x);
	    p_c.y=0.5*(t.pa[t.nvtx[1][r2->id]-1].y+t.pa[t.nvtx[3][r2->id]-1].y);
	    p_c.z=0.5*(t.pa[t.nvtx[0][r2->id]-1].z+t.pa[t.nvtx[4][r2->id]-1].z);

		// � ib ������������ ����� ����� �������� ����������� ����������� ����� ��.
	    bi_fluid=in_model_flow(p_c,ib,b,lb); // ����������� �� �� ������ ����.

		if ((bi_fluid) && (t.binternalsource[t.sosedi[SSIDE][r2->id].iNODE1 - t.maxelm])) r1->bNeimanStart = true;
		
		while (r2->next!=NULL) r2=r2->next; // ��������� � ����� ������

		p_c.x=0.5*(t.pa[t.nvtx[0][r2->id]-1].x+t.pa[t.nvtx[1][r2->id]-1].x);
	    p_c.y=0.5*(t.pa[t.nvtx[1][r2->id]-1].y+t.pa[t.nvtx[3][r2->id]-1].y);
	    p_c.z=0.5*(t.pa[t.nvtx[0][r2->id]-1].z+t.pa[t.nvtx[4][r2->id]-1].z);

	    bi_fluid=in_model_flow(p_c,ib,b,lb); // ����������� �� �� ������ ����.

		if ((bi_fluid) && (t.binternalsource[t.sosedi[NSIDE][r2->id].iNODE1 - t.maxelm])) r1->bNeimanEnd = true;

		r1=r1->next; // ������� � ��������� ������
	}

	// ��� �z
	r1=rootBT;
	while (r1 != NULL) {
		
		// �������������
		r1->bNeimanStart=false;
		r1->bNeimanEnd=false; 

	    NODELR* r2=r1->root;

		p_c.x=0.5*(t.pa[t.nvtx[0][r2->id]-1].x+t.pa[t.nvtx[1][r2->id]-1].x);
	    p_c.y=0.5*(t.pa[t.nvtx[1][r2->id]-1].y+t.pa[t.nvtx[3][r2->id]-1].y);
	    p_c.z=0.5*(t.pa[t.nvtx[0][r2->id]-1].z+t.pa[t.nvtx[4][r2->id]-1].z);

		// � ib ������������ ����� ����� �������� ����������� ����������� ����� ��.
	    bi_fluid=in_model_flow(p_c,ib,b,lb); // ����������� �� �� ������ ����.

		if ((bi_fluid) && (t.binternalsource[t.sosedi[BSIDE][r2->id].iNODE1 - t.maxelm])) r1->bNeimanStart = true;
		
		while (r2->next!=NULL) r2=r2->next; // ��������� � ����� ������

		p_c.x=0.5*(t.pa[t.nvtx[0][r2->id]-1].x+t.pa[t.nvtx[1][r2->id]-1].x);
	    p_c.y=0.5*(t.pa[t.nvtx[1][r2->id]-1].y+t.pa[t.nvtx[3][r2->id]-1].y);
	    p_c.z=0.5*(t.pa[t.nvtx[0][r2->id]-1].z+t.pa[t.nvtx[4][r2->id]-1].z);

	    bi_fluid=in_model_flow(p_c,ib,b,lb); // ����������� �� �� ������ ����.

		if ((bi_fluid) && (t.binternalsource[t.sosedi[TSIDE][r2->id].iNODE1 - t.maxelm])) r1->bNeimanEnd = true;

		r1=r1->next; // ������� � ��������� ������
	}

	NODELR_BASE *rootscan=rootWE;

	integer il=0;
	while (rootscan!=NULL) {
		il++;
		rootscan=rootscan->next;
	}
	t.iWE=il;

	il=0;
	rootscan=rootSN;
	while (rootscan!=NULL) {
		il++;
		rootscan=rootscan->next;
	}
	t.iSN=il;

	il=0;
	rootscan=rootBT;
	while (rootscan!=NULL) {
		il++;
		rootscan=rootscan->next;
	}
	t.iBT=il;

	// ����������� ����������� �������� ������ � ������� ���������.
	// ������ �� ����� next �������������� � ���������� ������.
	// ������� ��������� ��� ���������� ������ �������� �������.
	if (t.rootWE != NULL) {
		delete t.rootWE;
		t.rootWE = NULL;
	}
	t.rootWE=new NODELR_BASE[t.iWE];
	for (integer i1=0; i1<t.iWE; i1++) {
		rootscan=rootWE;
		t.rootWE[i1].bNeimanStart=rootscan->bNeimanStart;
		t.rootWE[i1].bNeimanEnd=rootscan->bNeimanEnd;
		t.rootWE[i1].ilineid=rootscan->ilineid;
		t.rootWE[i1].iN=rootscan->iN;
		t.rootWE[i1].root=rootscan->root;
		t.rootWE[i1].next=NULL;
		rootWE=rootWE->next;
		rootscan->next=NULL;
		rootscan->root=NULL;
		delete rootscan;
	}

	if (t.rootSN != NULL) {
		delete t.rootSN;
		t.rootSN = NULL;
	}
	t.rootSN=new NODELR_BASE[t.iSN];
	for (integer i1=0; i1<t.iSN; i1++) {
		rootscan=rootSN;
		t.rootSN[i1].bNeimanStart=rootscan->bNeimanStart;
		t.rootSN[i1].bNeimanEnd=rootscan->bNeimanEnd;
		t.rootSN[i1].ilineid=rootscan->ilineid;
		t.rootSN[i1].iN=rootscan->iN;
		t.rootSN[i1].root=rootscan->root;
		t.rootSN[i1].next=NULL;
		rootSN=rootSN->next;
		rootscan->next=NULL;
		rootscan->root=NULL;
		delete rootscan;
	}

	if (t.rootBT != NULL) {
		delete t.rootBT;
		t.rootBT = NULL;
	}
	t.rootBT=new NODELR_BASE[t.iBT];
	for (integer i1=0; i1<t.iBT; i1++) {
		rootscan=rootBT;
		t.rootBT[i1].bNeimanStart=rootscan->bNeimanStart;
		t.rootBT[i1].bNeimanEnd=rootscan->bNeimanEnd;
		t.rootBT[i1].ilineid=rootscan->ilineid;
		t.rootBT[i1].iN=rootscan->iN;
		t.rootBT[i1].root=rootscan->root;
		t.rootBT[i1].next=NULL;
		rootBT=rootBT->next;
		rootscan->next=NULL;
		rootscan->root=NULL;
		delete rootscan;
	}

} // constr_line_temp

// ����������� ������
void free_root(NODELR_BASE* &root, integer &isize) {
	if (root!=NULL) {
		NODELR  *r1, *r2;
		for (integer i=0; i<isize; i++) {
			r1=root[i].root;
			root[i].root=NULL;
			// ����������� ����
			while (r1->next!=NULL) {
				r2=r1;
				r1=r1->next;
				r2->next=NULL;
				delete r2;
				r2=NULL;
			}
			delete r1;
			r1=NULL;
		}
		delete root;
		root=NULL;
		isize=0;
	}
} 

// ������������ ������ LR1:
// �.�. ������ ����������� ��������������� �����������, ��������
// � ��������� ������������� ������������� ������������ ������ ������� 
// ���������� ������������� ���������.
void solveLR1(doublereal* &x, doublereal* &rthdsd, integer ns, integer iVar, bool bnorelax,
	ALICE_PARTITION** sosedi, integer maxelm, equation3D** slau, equation3D_bon** slau_bon,
			   integer** iN, integer***id, integer iWE, integer iSN, integer iBT, doublereal *alpha, integer maxbound) {

	// ���� bnorelax==true �� ������ ���������� �� �����������.

    doublereal alphaP=1.0;
	if (!bnorelax) {
		switch (iVar) {
		    case VX : alphaP=alpha[iVar]; break;
		    case VY : alphaP=alpha[iVar]; break;
		    case VZ : alphaP=alpha[iVar]; break;
	    }
	}
	else alphaP=1.0;
	doublereal alphaPb=alphaP; // 1.0; norelax boundary! // ���������� ��������� �����

	// ns - ����������� ������� x � ������ ����� rthdsd.
	// f.slau - ��������� ������ ����.
	// x - ������ �������.
	// rthdsd - ������ ������ �����.

	//f.a, f.b, f.c, f.d - ��������������� ������� ���� � ������ �����
	// f.P, f.Q -  ����������� ������������
	// f.ind - ���������� ����� ����������

    // ���� ����� ������� ������� �� ������ ���������� ���, � ���� ����� ������� ������� �� ������ ���������� ����.

	// WE direct

	// ������ �� ���� ������:

	// ����� ������: ���� � ���� ���������� �������� � ������������ ������������.
	// �� ��������� ����� ������ ����� ����� ������:
	doublereal* xc=new doublereal[maxelm+maxbound];
	#pragma omp parallel for shared (x, xc) schedule (guided) // 
	for (integer j=0; j<maxelm+maxbound; j++) {
		xc[j]=x[j]; // xc - x copy
	}

    #pragma omp parallel for shared (sosedi, maxelm, slau_bon, iVar, slau, rthdsd, alphaP, alphaPb, x, iN, id) schedule (guided) // 
	for (integer j=0; j<iWE; j++) {
		/* ���������� ����������:
		*  f.sosedi, f.maxelm, f.slau_bon, iVar,
		*  f.slau, rthdsd, alphaP, alphaPb, x, 
		*  f.rootWE
		*/
		
		integer n=iN[0][j];

		// ������� ������ ��� ����������� ���������:
		doublereal *a, *b, *c, *d; // ��������������� ������� � ������ �����
	    doublereal *P, *Q; // ����������� ������������
	    integer *ind; // ����� � ���������� ���������� �����.
		// ���������������� �������
	    a=new doublereal[n];
	    b=new doublereal[n];
	    c=new doublereal[n];
	    d=new doublereal[n];
	    // ����������� ������������
	    P=new doublereal[n];
	    Q=new doublereal[n];
	    // ���������� ����� ��������� (�����)
	    ind= new integer[n];

		
		// �.�. ��������� �������:
		c[0]=0.0;
		b[n-1]=0.0;

		integer iP=sosedi[WSIDE][id[0][j][0]].iNODE1; // ����� ���������� ����
		a[0]=slau_bon[iVar][iP-maxelm].aw;///alphaP; // ��������� ���� �� ������������� ���� �� ��� ����� ������� �������
		if (slau_bon[iVar][iP-maxelm].iI>-1) {
			b[0]=slau_bon[iVar][iP-maxelm].ai;
			a[0]=a[0]/alphaPb;
		} else b[0]=0.0;
		d[0]=rthdsd[iP]; //slau_bon[iVar][iP-maxelm].b;
		ind[0]=iP;

		for (integer i=1; i<n-1; i++) {
			iP=id[0][j][i-1];
			ind[i]=iP;
			a[i]=slau[iVar][iP].ap/alphaP;
			b[i]=slau[iVar][iP].ae;
			c[i]=slau[iVar][iP].aw;
			d[i] = slau[iVar][iP].an*xc[sosedi[NSIDE][iP].iNODE1];
			d[i] += slau[iVar][iP].as*xc[sosedi[SSIDE][iP].iNODE1];
			d[i] += slau[iVar][iP].at*xc[sosedi[TSIDE][iP].iNODE1];
			d[i] += slau[iVar][iP].ab*xc[sosedi[BSIDE][iP].iNODE1];
			d[i]+=rthdsd[iP]; //f.slau[iVar][iP].b;
		}

		iP = sosedi[ESIDE][id[0][j][n - 3]].iNODE1; // ����� ���������� ����
		a[n-1]=slau_bon[iVar][iP-maxelm].aw;///alphaP; // ��������� ���� �� �������������
        if (slau_bon[iVar][iP-maxelm].iI>-1) {
		    c[n-1]=slau_bon[iVar][iP-maxelm].ai;
			a[n-1]=a[n-1]/alphaPb;
		} else c[n-1]=0.0;
		d[n-1]=rthdsd[iP]; //f.slau_bon[iVar][iP-f.maxelm].b;
        ind[n-1]=iP;

		// ���������� ���� �������� ��� �������� ������:
        P[0]=b[0]/a[0];
		Q[0]=d[0]/a[0];
		doublereal rdiv;
		for (integer i=1; i<n; i++) {
            rdiv=1.0/(a[i]-c[i]*P[i-1]);
			P[i]=b[i]*rdiv;
			Q[i]=(d[i]+c[i]*Q[i-1])*rdiv;
		}
		x[ind[n-1]]=Q[n-1];
		for (integer i=n-2; i>=0; i--) {
			x[ind[i]]=P[i]*x[ind[i+1]]+Q[i];
		}       

		// ������������ ����������� ������
		delete[] a;
	    delete[] b;
	    delete[] c;
	    delete[] d;
	    // ����������� ������������
	    delete[] P;
	    delete[] Q;
	    // ���������� ����� ��������� (�����)
	    delete[] ind;

	} // for

//#pragma omp barrier


	// SN direct
	
	// ������ �� ���� ������:
	#pragma omp parallel for shared (x, xc) schedule (guided) // 
	for (integer j=0; j<maxelm+maxbound; j++) {
		xc[j]=x[j]; // xc - x copy
	}

    #pragma omp parallel for shared (sosedi, maxelm, slau_bon, iVar, slau, rthdsd, alphaP, alphaPb, x, iN, id) schedule (guided) // 
	for (integer j=0; j<iSN; j++) {

		// printf("Ok\n"); // debug

		integer n=iN[1][j];

		// ������� ������ ��� ����������� ���������:
		doublereal *a, *b, *c, *d; // ��������������� ������� � ������ �����
	    doublereal *P, *Q; // ����������� ������������
	    integer *ind; // ����� � ���������� ���������� �����.
		// ���������������� �������
	    a=new doublereal[n];
	    b=new doublereal[n];
	    c=new doublereal[n];
	    d=new doublereal[n];
	    // ����������� ������������
	    P=new doublereal[n];
	    Q=new doublereal[n];
	    // ���������� ����� ��������� (�����)
	    ind= new integer[n];
		
		// �.�. ��������� �������:
		c[0]=0.0;
		b[n-1]=0.0;

		integer iP = sosedi[SSIDE][id[1][j][0]].iNODE1; // ����� ���������� ����
		a[0]=slau_bon[iVar][iP-maxelm].aw;///alphaP;
		if (slau_bon[iVar][iP-maxelm].iI>-1) {
			b[0]=slau_bon[iVar][iP-maxelm].ai;
			a[0]=a[0]/alphaPb;
		} else b[0]=0.0;
		d[0]=rthdsd[iP]; //slau_bon[iVar][iP-maxelm].b;
		ind[0]=iP;

		for (integer i=1; i<n-1; i++) {
			iP=id[1][j][i-1];
			ind[i]=iP;
			a[i]=slau[iVar][iP].ap/alphaP;
			b[i]=slau[iVar][iP].an;
			c[i]=slau[iVar][iP].as;
			d[i] = slau[iVar][iP].ae*xc[sosedi[ESIDE][iP].iNODE1];
			d[i] += slau[iVar][iP].aw*xc[sosedi[WSIDE][iP].iNODE1];
			d[i] += slau[iVar][iP].at*xc[sosedi[TSIDE][iP].iNODE1];
			d[i] += slau[iVar][iP].ab*xc[sosedi[BSIDE][iP].iNODE1];
			d[i]+=rthdsd[iP]; //f.slau[iVar][iP].b
		}


		iP = sosedi[NSIDE][id[1][j][n - 3]].iNODE1; // ����� ���������� ����
		a[n-1]=slau_bon[iVar][iP-maxelm].aw;///alphaP;
        if (slau_bon[iVar][iP-maxelm].iI>-1) {
		    c[n-1]=slau_bon[iVar][iP-maxelm].ai;
			a[n-1]=a[n-1]/alphaPb;
		} else c[n-1]=0.0;
		d[n-1]=rthdsd[iP]; //f.slau_bon[iVar][iP-f.maxelm].b;
        ind[n-1]=iP;
		

		// ���������� ���� �������� ��� �������� ������:
        P[0]=b[0]/a[0];
		Q[0]=d[0]/a[0];
		doublereal rdiv;
		for (integer i=1; i<n; i++) {
            rdiv=1.0/(a[i]-c[i]*P[i-1]);
			P[i]=b[i]*rdiv;
			Q[i]=(d[i]+c[i]*Q[i-1])*rdiv;
		}
		// printf("Ok2\n"); // debug
		x[ind[n-1]]=Q[n-1];
		for (integer i=n-2; i>=0; i--) {
			x[ind[i]]=P[i]*x[ind[i+1]]+Q[i];
		}       

		// ������������ ����������� ������
		delete[] a;
	    delete[] b;
	    delete[] c;
	    delete[] d;
	    // ����������� ������������
	    delete[] P;
	    delete[] Q;
	    // ���������� ����� ��������� (�����)
	    delete[] ind;

	} // for
	


	// BT direct

	// ������ �� ���� ������:
	#pragma omp parallel for shared (x, xc) schedule (guided) // 
	for (integer j=0; j<maxelm+maxbound; j++) {
		xc[j]=x[j]; // xc - x copy
	}

    #pragma omp parallel for shared (sosedi, maxelm, slau_bon, iVar, slau, rthdsd, alphaP, alphaPb, x, iN, id) schedule (guided) // 
	for (integer j=0; j<iBT; j++) {

		integer n=iN[2][j];

		// ������� ������ ��� ����������� ���������:
		doublereal *a=NULL, *b=NULL, *c=NULL, *d=NULL; // ��������������� ������� � ������ �����
	    doublereal *P=NULL, *Q=NULL; // ����������� ������������
	    integer *ind=NULL; // ����� � ���������� ���������� �����.
		// ���������������� �������
	    a=new doublereal[n];
	    b=new doublereal[n];
	    c=new doublereal[n];
	    d=new doublereal[n];
	    // ����������� ������������
	    P=new doublereal[n];
	    Q=new doublereal[n];
	    // ���������� ����� ��������� (�����)
	    ind= new integer[n];
		
		// �.�. ��������� �������:
		c[0]=0.0;
		b[n-1]=0.0;
 
		integer iP = sosedi[BSIDE][id[2][j][0]].iNODE1; // ����� ���������� ����
		a[0]=slau_bon[iVar][iP-maxelm].aw;///alphaP;
		if (slau_bon[iVar][iP-maxelm].iI>-1) {
			b[0]=slau_bon[iVar][iP-maxelm].ai;
			a[0]=a[0]/alphaPb;
		} else b[0]=0.0;
		d[0]=rthdsd[iP]; //slau_bon[iVar][iP-maxelm].b;
		ind[0]=iP;

		for (integer i=1; i<n-1; i++) {
			iP=id[2][j][i-1];
			ind[i]=iP;
			a[i]=slau[iVar][iP].ap/alphaP;
			b[i]=slau[iVar][iP].at;
			c[i]=slau[iVar][iP].ab;
			d[i] = slau[iVar][iP].ae*xc[sosedi[ESIDE][iP].iNODE1];
			d[i] += slau[iVar][iP].aw*xc[sosedi[WSIDE][iP].iNODE1];
			d[i] += slau[iVar][iP].an*xc[sosedi[NSIDE][iP].iNODE1];
			d[i] += slau[iVar][iP].as*xc[sosedi[SSIDE][iP].iNODE1];
			d[i]+=rthdsd[iP]; //f.slau[iVar][iP].b;
		}
		
		iP = sosedi[TSIDE][id[2][j][n - 3]].iNODE1; // ����� ���������� ����
		a[n-1]=slau_bon[iVar][iP-maxelm].aw;//alphaP;
        if (slau_bon[iVar][iP-maxelm].iI>-1) {
		    c[n-1]=slau_bon[iVar][iP-maxelm].ai;
			a[n-1]=a[n-1]/alphaPb;
		} else c[n-1]=0.0;
		d[n-1]=rthdsd[iP]; //slau_bon[iVar][iP-maxelm].b;
        ind[n-1]=iP;

		// ���������� ���� �������� ��� �������� ������:
        P[0]=b[0]/a[0];
		Q[0]=d[0]/a[0];
		doublereal rdiv;
		for (integer i=1; i<n; i++) {
            rdiv=1.0/(a[i]-c[i]*P[i-1]);
			P[i]=b[i]*rdiv;
			Q[i]=(d[i]+c[i]*Q[i-1])*rdiv;
		}
		x[ind[n-1]]=Q[n-1];
		for (integer i=n-2; i>=0; i--) {
			x[ind[i]]=P[i]*x[ind[i+1]]+Q[i];
		}       

		// ������������ ����������� ������
		//if (a != NULL) {
			// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
			delete[] a;
			a = NULL;
		//}
		//if (b != NULL) {
		    // �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
			delete[] b;
			b = NULL;
		//}
		//if (c != NULL) {
			// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
			delete[] c;
			c = NULL;
		//}
		//if (d != NULL) {
			// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
			delete[] d;
			d = NULL;
		//}
	    // ����������� ������������
		//if (P != NULL) {
		// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
			delete[] P;
			P = NULL;
		//}
		//if (Q != NULL) {
		// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
			delete[] Q;
			Q = NULL;
		//}
	    // ���������� ����� ��������� (�����)
		//if (ind != NULL) {
		// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
			delete[] ind;
			ind = NULL;
		//}

	} // for
 
	if (xc != NULL) {
		delete[] xc; // ������������ ����������� ������
	}
    
} // solveLR1

// ������������ ������.
// ������������ ������ LR1:
// �.�. ������ ����������� ��������������� �����������, ��������
// � ��������� ������������� ������������� ������������ ������ ������� 
// ���������� ������������� ���������.
void solveLR1_serial_sor(doublereal* &x, doublereal* &rthdsd, integer ns, integer iVar, bool bnorelax,
	ALICE_PARTITION** sosedi, integer maxelm, equation3D** slau, equation3D_bon** slau_bon,
	integer** iN, integer***id, integer iWE, integer iSN, integer iBT, doublereal *alpha, integer maxbound, doublereal omega) {

	// ���� bnorelax==true �� ������ ���������� �� �����������.
	//doublereal omega = 1.855;//1.855 ��� ���� ������ �� ��������� ��� ��������� ��������.

	doublereal alphaP = 1.0;
	if (!bnorelax) {
		switch (iVar) {
		case VX: alphaP = alpha[iVar]; break;
		case VY: alphaP = alpha[iVar]; break;
		case VZ: alphaP = alpha[iVar]; break;
		}
	}
	else alphaP = 1.0;
	doublereal alphaPb = alphaP; // 1.0; norelax boundary! // ���������� ��������� �����

	// ns - ����������� ������� x � ������ ����� rthdsd.
	// f.slau - ��������� ������ ����.
	// x - ������ �������.
	// rthdsd - ������ ������ �����.

	//f.a, f.b, f.c, f.d - ��������������� ������� ���� � ������ �����
	// f.P, f.Q -  ����������� ������������
	// f.ind - ���������� ����� ����������

	// ���� ����� ������� ������� �� ������ ���������� ���, � ���� ����� ������� ������� �� ������ ���������� ����.

	// WE direct

	// ������ �� ���� ������:

	

	// ����� ������: ���� � ���� ���������� �������� � ������������ ������������.
	// �� ��������� ����� ������ ����� ����� ������:
	doublereal* xc = new doublereal[maxelm + maxbound];
//#pragma omp parallel for shared (x, xc) schedule (guided) // 
	for (integer j = 0; j<maxelm + maxbound; j++) {
		xc[j] = x[j]; // xc - x copy
	}

//#pragma omp parallel for shared (sosedi, maxelm, slau_bon, iVar, slau, rthdsd, alphaP, alphaPb, x, iN, id) schedule (guided) // 
	for (integer j = 0; j<iWE; j++) {
		

		/* ���������� ����������:
		*  f.sosedi, f.maxelm, f.slau_bon, iVar,
		*  f.slau, rthdsd, alphaP, alphaPb, x,
		*  f.rootWE
		*/

		integer n = iN[0][j];

		// ������� ������ ��� ����������� ���������:
		doublereal *a, *b, *c, *d; // ��������������� ������� � ������ �����
		doublereal *P, *Q; // ����������� ������������
		integer *ind; // ����� � ���������� ���������� �����.
		// ���������������� �������
		a = new doublereal[n];
		b = new doublereal[n];
		c = new doublereal[n];
		d = new doublereal[n];
		// ����������� ������������
		P = new doublereal[n];
		Q = new doublereal[n];
		// ���������� ����� ��������� (�����)
		ind = new integer[n];


		// �.�. ��������� �������:
		c[0] = 0.0;
		b[n - 1] = 0.0;

		

		integer iP = sosedi[WSIDE][id[0][j][0]].iNODE1; // ����� ���������� ����
		a[0] = slau_bon[iVar][iP - maxelm].aw;///alphaP; // ��������� ���� �� ������������� ���� �� ��� ����� ������� �������
		if (slau_bon[iVar][iP - maxelm].iI>-1) {
			b[0] = slau_bon[iVar][iP - maxelm].ai;
			a[0] = a[0] / alphaPb;
		}
		else b[0] = 0.0;
		d[0] = rthdsd[iP]; //slau_bon[iVar][iP-maxelm].b;
		ind[0] = iP;

		

		for (integer i = 1; i<n - 1; i++) {
			// n-2-1==n-3
			iP = id[0][j][i - 1];
			ind[i] = iP;
			a[i] = slau[iVar][iP].ap / alphaP;
			b[i] = slau[iVar][iP].ae;
			c[i] = slau[iVar][iP].aw;
			//d[i] = slau[iVar][iP].an*xc[sosedi[NSIDE][iP].iNODE1];
			//d[i] += slau[iVar][iP].as*xc[sosedi[SSIDE][iP].iNODE1];
			//d[i] += slau[iVar][iP].at*xc[sosedi[TSIDE][iP].iNODE1];
			//d[i] += slau[iVar][iP].ab*xc[sosedi[BSIDE][iP].iNODE1];
			// serial
			
			
			d[i] = slau[iVar][iP].an*x[sosedi[NSIDE][iP].iNODE1];
			d[i] += slau[iVar][iP].as*x[sosedi[SSIDE][iP].iNODE1];
			d[i] += slau[iVar][iP].at*x[sosedi[TSIDE][iP].iNODE1];
			d[i] += slau[iVar][iP].ab*x[sosedi[BSIDE][iP].iNODE1];
			d[i] += rthdsd[iP]; //f.slau[iVar][iP].b;

			
		}

		

		iP = sosedi[ESIDE][id[0][j][n - 3]].iNODE1; // ����� ���������� ����
		
		
		a[n - 1] = slau_bon[iVar][iP - maxelm].aw;///alphaP; // ��������� ���� �� �������������
		if (slau_bon[iVar][iP - maxelm].iI>-1) {
			c[n - 1] = slau_bon[iVar][iP - maxelm].ai;
			a[n - 1] = a[n - 1] / alphaPb;
		}
		else c[n - 1] = 0.0;
			
		d[n - 1] = rthdsd[iP]; //f.slau_bon[iVar][iP-f.maxelm].b;
		ind[n - 1] = iP;

		

		// ���������� ���� �������� ��� �������� ������:
		P[0] = b[0] / a[0];
		Q[0] = d[0] / a[0];
		doublereal rdiv;
		for (integer i = 1; i<n; i++) {
			rdiv = 1.0 / (a[i] - c[i] * P[i - 1]);
			P[i] = b[i] * rdiv;
			Q[i] = (d[i] + c[i] * Q[i - 1])*rdiv;
		}

		
		//x[ind[n - 1]] = Q[n - 1];
		//for (integer i = n - 2; i >= 0; i--) {
			//x[ind[i]] = P[i] * x[ind[i + 1]] + Q[i];
		//}
		xc[ind[n - 1]] = Q[n - 1];
		for (integer i = n - 2; i >= 0; i--) {
			xc[ind[i]] = P[i] * xc[ind[i + 1]] + Q[i];
		}
		for (integer i = n - 2; i >= 1; i--) {
			// ������� ����� ��������.
			x[ind[i]] = omega*xc[ind[i]] + (1.0 - omega)*x[ind[i]];
		}
		// ��������� ���� �� ���������� �������� ����������.
		x[ind[0]] = xc[ind[0]];
		x[ind[n - 1]] = xc[ind[n - 1]];

		// ������������ ����������� ������
		delete[] a;
		delete[] b;
		delete[] c;
		delete[] d;
		// ����������� ������������
		delete[] P;
		delete[] Q;
		// ���������� ����� ��������� (�����)
		delete[] ind;

	} // for

	

	//#pragma omp barrier

	
	// SN direct

	// ������ �� ���� ������:
//#pragma omp parallel for shared (x, xc) schedule (guided) // 
	for (integer j = 0; j<maxelm + maxbound; j++) {
		xc[j] = x[j]; // xc - x copy
	}

//#pragma omp parallel for shared (sosedi, maxelm, slau_bon, iVar, slau, rthdsd, alphaP, alphaPb, x, iN, id) schedule (guided) // 
	for (integer j = 0; j<iSN; j++) {

		// printf("Ok\n"); // debug

		integer n = iN[1][j];

		// ������� ������ ��� ����������� ���������:
		doublereal *a, *b, *c, *d; // ��������������� ������� � ������ �����
		doublereal *P, *Q; // ����������� ������������
		integer *ind; // ����� � ���������� ���������� �����.
		// ���������������� �������
		a = new doublereal[n];
		b = new doublereal[n];
		c = new doublereal[n];
		d = new doublereal[n];
		// ����������� ������������
		P = new doublereal[n];
		Q = new doublereal[n];
		// ���������� ����� ��������� (�����)
		ind = new integer[n];

		// �.�. ��������� �������:
		c[0] = 0.0;
		b[n - 1] = 0.0;

		integer iP = sosedi[SSIDE][id[1][j][0]].iNODE1; // ����� ���������� ����
		a[0] = slau_bon[iVar][iP - maxelm].aw;///alphaP;
		if (slau_bon[iVar][iP - maxelm].iI>-1) {
			b[0] = slau_bon[iVar][iP - maxelm].ai;
			a[0] = a[0] / alphaPb;
		}
		else b[0] = 0.0;
		d[0] = rthdsd[iP]; //slau_bon[iVar][iP-maxelm].b;
		ind[0] = iP;

		for (integer i = 1; i<n - 1; i++) {
			iP = id[1][j][i - 1];
			ind[i] = iP;
			a[i] = slau[iVar][iP].ap / alphaP;
			b[i] = slau[iVar][iP].an;
			c[i] = slau[iVar][iP].as;
			//d[i] = slau[iVar][iP].ae*xc[sosedi[ESIDE][iP].iNODE1];
			//d[i] += slau[iVar][iP].aw*xc[sosedi[WSIDE][iP].iNODE1];
			//d[i] += slau[iVar][iP].at*xc[sosedi[TSIDE][iP].iNODE1];
			//d[i] += slau[iVar][iP].ab*xc[sosedi[BSIDE][iP].iNODE1];
			d[i] = slau[iVar][iP].ae*x[sosedi[ESIDE][iP].iNODE1];
			d[i] += slau[iVar][iP].aw*x[sosedi[WSIDE][iP].iNODE1];
			d[i] += slau[iVar][iP].at*x[sosedi[TSIDE][iP].iNODE1];
			d[i] += slau[iVar][iP].ab*x[sosedi[BSIDE][iP].iNODE1];
			d[i] += rthdsd[iP]; //f.slau[iVar][iP].b
		}


		iP = sosedi[NSIDE][id[1][j][n - 3]].iNODE1; // ����� ���������� ����
		a[n - 1] = slau_bon[iVar][iP - maxelm].aw;///alphaP;
		if (slau_bon[iVar][iP - maxelm].iI>-1) {
			c[n - 1] = slau_bon[iVar][iP - maxelm].ai;
			a[n - 1] = a[n - 1] / alphaPb;
		}
		else c[n - 1] = 0.0;
		d[n - 1] = rthdsd[iP]; //f.slau_bon[iVar][iP-f.maxelm].b;
		ind[n - 1] = iP;


		// ���������� ���� �������� ��� �������� ������:
		P[0] = b[0] / a[0];
		Q[0] = d[0] / a[0];
		doublereal rdiv;
		for (integer i = 1; i<n; i++) {
			rdiv = 1.0 / (a[i] - c[i] * P[i - 1]);
			P[i] = b[i] * rdiv;
			Q[i] = (d[i] + c[i] * Q[i - 1])*rdiv;
		}
		// printf("Ok2\n"); // debug
		//x[ind[n - 1]] = Q[n - 1];
		//for (integer i = n - 2; i >= 0; i--) {
			//x[ind[i]] = P[i] * x[ind[i + 1]] + Q[i];
		//}
		xc[ind[n - 1]] = Q[n - 1];
		for (integer i = n - 2; i >= 0; i--) {
			xc[ind[i]] = P[i] * xc[ind[i + 1]] + Q[i];
		}
		for (integer i = n - 2; i >= 1; i--) {
			// ������� ����� ��������.
			x[ind[i]] = omega*xc[ind[i]] + (1.0 - omega)*x[ind[i]];
		}
		// ��������� ���� �� ���������� �������� ����������.
		x[ind[0]] = xc[ind[0]];
		x[ind[n - 1]] = xc[ind[n - 1]];

		// ������������ ����������� ������
		delete[] a;
		delete[] b;
		delete[] c;
		delete[] d;
		// ����������� ������������
		delete[] P;
		delete[] Q;
		// ���������� ����� ��������� (�����)
		delete[] ind;

	} // for



	// BT direct

	// ������ �� ���� ������:
//#pragma omp parallel for shared (x, xc) schedule (guided) // 
	for (integer j = 0; j<maxelm + maxbound; j++) {
		xc[j] = x[j]; // xc - x copy
	}

//#pragma omp parallel for shared (sosedi, maxelm, slau_bon, iVar, slau, rthdsd, alphaP, alphaPb, x, iN, id) schedule (guided) // 
	for (integer j = 0; j<iBT; j++) {

		integer n = iN[2][j];

		// ������� ������ ��� ����������� ���������:
		doublereal *a = NULL, *b = NULL, *c = NULL, *d = NULL; // ��������������� ������� � ������ �����
		doublereal *P = NULL, *Q = NULL; // ����������� ������������
		integer *ind = NULL; // ����� � ���������� ���������� �����.
		// ���������������� �������
		a = new doublereal[n];
		b = new doublereal[n];
		c = new doublereal[n];
		d = new doublereal[n];
		// ����������� ������������
		P = new doublereal[n];
		Q = new doublereal[n];
		// ���������� ����� ��������� (�����)
		ind = new integer[n];

		// �.�. ��������� �������:
		c[0] = 0.0;
		b[n - 1] = 0.0;

		integer iP = sosedi[BSIDE][id[2][j][0]].iNODE1; // ����� ���������� ����
		a[0] = slau_bon[iVar][iP - maxelm].aw;///alphaP;
		if (slau_bon[iVar][iP - maxelm].iI>-1) {
			b[0] = slau_bon[iVar][iP - maxelm].ai;
			a[0] = a[0] / alphaPb;
		}
		else b[0] = 0.0;
		d[0] = rthdsd[iP]; //slau_bon[iVar][iP-maxelm].b;
		ind[0] = iP;

		for (integer i = 1; i<n - 1; i++) {
			iP = id[2][j][i - 1];
			ind[i] = iP;
			a[i] = slau[iVar][iP].ap / alphaP;
			b[i] = slau[iVar][iP].at;
			c[i] = slau[iVar][iP].ab;
			//d[i] = slau[iVar][iP].ae*xc[sosedi[ESIDE][iP].iNODE1];
			//d[i] += slau[iVar][iP].aw*xc[sosedi[WSIDE][iP].iNODE1];
			//d[i] += slau[iVar][iP].an*xc[sosedi[NSIDE][iP].iNODE1];
			//d[i] += slau[iVar][iP].as*xc[sosedi[SSIDE][iP].iNODE1];

			d[i] = slau[iVar][iP].ae*x[sosedi[ESIDE][iP].iNODE1];
			d[i] += slau[iVar][iP].aw*x[sosedi[WSIDE][iP].iNODE1];
			d[i] += slau[iVar][iP].an*x[sosedi[NSIDE][iP].iNODE1];
			d[i] += slau[iVar][iP].as*x[sosedi[SSIDE][iP].iNODE1];
			d[i] += rthdsd[iP]; //f.slau[iVar][iP].b;
		}

		iP = sosedi[TSIDE][id[2][j][n - 3]].iNODE1; // ����� ���������� ����
		a[n - 1] = slau_bon[iVar][iP - maxelm].aw;//alphaP;
		if (slau_bon[iVar][iP - maxelm].iI>-1) {
			c[n - 1] = slau_bon[iVar][iP - maxelm].ai;
			a[n - 1] = a[n - 1] / alphaPb;
		}
		else c[n - 1] = 0.0;
		d[n - 1] = rthdsd[iP]; //slau_bon[iVar][iP-maxelm].b;
		ind[n - 1] = iP;

		// ���������� ���� �������� ��� �������� ������:
		P[0] = b[0] / a[0];
		Q[0] = d[0] / a[0];
		doublereal rdiv;
		for (integer i = 1; i<n; i++) {
			rdiv = 1.0 / (a[i] - c[i] * P[i - 1]);
			P[i] = b[i] * rdiv;
			Q[i] = (d[i] + c[i] * Q[i - 1])*rdiv;
		}
		//x[ind[n - 1]] = Q[n - 1];
		//for (integer i = n - 2; i >= 0; i--) {
			//x[ind[i]] = P[i] * x[ind[i + 1]] + Q[i];
		//}
		xc[ind[n - 1]] = Q[n - 1];
		for (integer i = n - 2; i >= 0; i--) {
			xc[ind[i]] = P[i] * xc[ind[i + 1]] + Q[i];
		}
		for (integer i = n - 2; i >= 1; i--) {
			// ������� ����� ��������.
			x[ind[i]] = omega*xc[ind[i]] + (1.0 - omega)*x[ind[i]];
		}
		// ��������� ���� �� ���������� �������� ����������.
		x[ind[0]] = xc[ind[0]];
		x[ind[n - 1]] = xc[ind[n - 1]];

		// ������������ ����������� ������
		//if (a != NULL) {
			// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
			delete[] a;
			a = NULL;
		//}
		//if (b != NULL) {
			// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
			delete[] b;
			b = NULL;
		//}
		//if (c != NULL) {
			// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
			delete[] c;
			c = NULL;
		//}
		//if (d != NULL) {
			// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
			delete[] d;
			d = NULL;
		//}
		// ����������� ������������
		//if (P != NULL) {
			// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
			delete[] P;
			P = NULL;
		//}
		//if (Q != NULL) {
			// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
			delete[] Q;
			Q = NULL;
		//}
		// ���������� ����� ��������� (�����)
		//if (ind != NULL) {
			// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
			delete[] ind;
			ind = NULL;
		//}

	} // for

	//if (xc != NULL) {
		// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
		delete[] xc; // ������������ ����������� ������
		xc = NULL;
	//}

} // solveLR1_serial


// ������������ ������ LR1:
// �.�. ������ ����������� ��������������� �����������, ��������
// � ��������� ������������� ������������� ������������ ������ ������� 
// ���������� ������������� ���������.
void solveLR1_temp(TEMPER &t, doublereal* &x, doublereal* &rthdsd, integer ns) {

	// ������ �������� ����� ������������ �������� ����� ���������������� ���������
	// ��������� �������� ����� �� ��� ��������. ��� ����� ������ � ���������. TODO.

    doublereal alphaP=1.0;
	
	// ns - ����������� ������� x � ������ ����� rthdsd.
	// f.slau - ��������� ������ ����.
	// x - ������ �������.
	// rthdsd - ������ ������ �����.

	//f.a, f.b, f.c, f.d - ��������������� ������� ���� � ������ �����
	// f.P, f.Q -  ����������� ������������
	// f.ind - ���������� ����� ����������

    // ���� ����� ������� ������� �� ������ ���������� ���, � ���� ����� ������� ������� �� ������ ���������� ����.

	// WE direct
	
	doublereal* xc=new doublereal[t.maxelm+t.maxbound];

	#pragma omp parallel for shared (x, xc) schedule (guided) // 
	for (integer i1=0; i1<t.maxelm+t.maxbound; i1++) {
		xc[i1]=x[i1]; // copy x
	}

	// ������ �� ���� ������:
	#pragma omp parallel for shared (t, rthdsd, alphaP, x, xc) schedule (guided) // 
	for (integer j=0; j<t.iWE; j++) {		

		integer n=t.rootWE[j].iN; // �������������, � ������ ������� ������ ��� �������� ����������������.

		// ������� ������ ��� ����������� ���������:
		doublereal *a=NULL, *b=NULL, *c=NULL, *d=NULL; // ��������������� ������� � ������ �����
	    doublereal *P=NULL, *Q=NULL; // ����������� ������������
	    integer *ind=NULL; // ����� � ���������� ���������� �����.
		// ���������������� �������
	    a=new doublereal[n];
	    b=new doublereal[n];
	    c=new doublereal[n];
	    d=new doublereal[n];
	    // ����������� ������������
	    P=new doublereal[n];
	    Q=new doublereal[n];
	    // ���������� ����� ��������� (�����)
	    ind= new integer[n];

		integer iP=-1, i=-1;

		if ((a != NULL) && (b != NULL) && (c != NULL) && (d != NULL) && (P != NULL) && (Q != NULL) && (ind != NULL)) {

			

			NODELR* r2 = t.rootWE[j].root;


			if (!t.rootWE[j].bNeimanStart) {
				iP = t.sosedi[WSIDE][r2->id].iNODE1; // ����� ���������� ����
				a[0] = t.slau_bon[iP - t.maxelm].aw;///alphaP; // ��������� ���� �� ������������� ���� �� ��� ����� ������� �������
				if (t.slau_bon[iP - t.maxelm].iI > -1) {
					b[0] = t.slau_bon[iP - t.maxelm].ai;
					a[0] = a[0] / alphaP;
				}
				else b[0] = 0.0;
				d[0] = rthdsd[iP]; //f.slau_bon[iVar][iP-f.maxelm].b;
				ind[0] = iP;
				c[0] = 0.0;
			}
			else {
				//printf("bug\n"); getchar(); // debug
				// ��������� ����� � ����������
				iP = r2->id;
				ind[0] = iP;
				a[0] = t.slau[iP].ap / alphaP;
				b[0] = t.slau[iP].ae;
				c[0] = 0.0; //t.slau[iP].aw; // ����� ���� ���������
				d[0] = t.slau[iP].an*xc[t.sosedi[NSIDE][iP].iNODE1];
				d[0] += t.slau[iP].as*xc[t.sosedi[SSIDE][iP].iNODE1];
				d[0] += t.slau[iP].at*xc[t.sosedi[TSIDE][iP].iNODE1];
				d[0] += t.slau[iP].ab*xc[t.sosedi[BSIDE][iP].iNODE1];
				d[0] += rthdsd[iP]; //f.slau[iVar][iP].b;

				r2 = r2->next;
			}

			i = 1;

			while (r2->next != NULL) {
				iP = r2->id;

				if (i < t.rootWE[j].iN) {
					ind[i] = iP;
					a[i] = t.slau[iP].ap / alphaP;
					b[i] = t.slau[iP].ae;
					c[i] = t.slau[iP].aw;
					d[i] = t.slau[iP].an*xc[t.sosedi[NSIDE][iP].iNODE1];
					d[i] += t.slau[iP].as*xc[t.sosedi[SSIDE][iP].iNODE1];
					d[i] += t.slau[iP].at*xc[t.sosedi[TSIDE][iP].iNODE1];
					d[i] += t.slau[iP].ab*xc[t.sosedi[BSIDE][iP].iNODE1];
					d[i] += rthdsd[iP]; //f.slau[iVar][iP].b;
				}
				else {
					printf("error 1: i >= t.rootWE[j].iN in my_LR.c file in solveLR1_temp.\n");
					system("pause");
					exit(1);
				}

				r2 = r2->next;


				i++;
			}



			if (!t.rootWE[j].bNeimanEnd) {
				if (r2 != NULL) {
					iP = r2->id;
					if (i < t.rootWE[j].iN) {
						ind[i] = iP;
						a[i] = t.slau[iP].ap / alphaP;
						b[i] = t.slau[iP].ae;
						c[i] = t.slau[iP].aw;
						d[i] = t.slau[iP].an*xc[t.sosedi[NSIDE][iP].iNODE1];
						d[i] += t.slau[iP].as*xc[t.sosedi[SSIDE][iP].iNODE1];
						d[i] += t.slau[iP].at*xc[t.sosedi[TSIDE][iP].iNODE1];
						d[i] += t.slau[iP].ab*xc[t.sosedi[BSIDE][iP].iNODE1];
						d[i] += rthdsd[iP]; //f.slau[iVar][iP].b;
					}
					else {
						printf("error 2: i >= t.rootWE[j].iN in my_LR.c file in solveLR1_temp.\n");
						system("pause");
						exit(1);
					}


					i++;
					iP = t.sosedi[ESIDE][r2->id].iNODE1; // ����� ���������� ����
					a[i] = t.slau_bon[iP - t.maxelm].aw;///alphaP; // ��������� ���� �� �������������
					if (t.slau_bon[iP - t.maxelm].iI > -1) {
						c[i] = t.slau_bon[iP - t.maxelm].ai;
						a[i] = a[i] / alphaP;
					}
					else c[i] = 0.0;
					d[i] = rthdsd[iP]; //t.slau_bon[iP-t.maxelm].b;
					ind[i] = iP;
					b[i] = 0.0;
				}
				else {
					printf("error 3: i >= t.rootWE[j].iN in my_LR.c file in solveLR1_temp.\n");
					system("pause");
					exit(1);
				}
			}
			else
			{
				//printf("bug\n"); getchar(); // debug
				// ���� ������ ������������� 
				// ����������� � ���������� ����� �� ��������.
				if (r2 != NULL) {
					iP = r2->id;
					if (i < t.rootWE[j].iN) {
						ind[i] = iP;
						a[i] = t.slau[iP].ap / alphaP;
						b[i] = 0.0; // t.slau[iP].ae; ����� ���������
						c[i] = t.slau[iP].aw;
						d[i] = t.slau[iP].an*xc[t.sosedi[NSIDE][iP].iNODE1];
						d[i] += t.slau[iP].as*xc[t.sosedi[SSIDE][iP].iNODE1];
						d[i] += t.slau[iP].at*xc[t.sosedi[TSIDE][iP].iNODE1];
						d[i] += t.slau[iP].ab*xc[t.sosedi[BSIDE][iP].iNODE1];
						d[i] += rthdsd[iP]; //f.slau[iVar][iP].b;
					}
					else {
						printf("error : i >= t.rootWE[j].iN in my_LR.c file in solveLR1_temp.\n");
						system("pause");
						exit(1);
					}
				}
				else {
					printf("error 2: i >= t.rootWE[j].iN in my_LR.c file in solveLR1_temp.\n");
					system("pause");
					exit(1);
				}
			}
			n = i + 1;

			r2 = NULL; // ���� ��������� ������ �� �����

			// ���������� ���� �������� ��� �������� ������:
			P[0] = b[0] / a[0];
			Q[0] = d[0] / a[0];
			doublereal rdiv;

			if (n <= t.rootWE[j].iN) {

			    for (i = 1; i < n; i++) {
				    rdiv = 1.0 / (a[i] - c[i] * P[i - 1]);
				    P[i] = b[i] * rdiv;
				    Q[i] = (d[i] + c[i] * Q[i - 1])*rdiv;
			    }			

				x[ind[n - 1]] = Q[n - 1];
				for (i = n - 2; i >= 0; i--) {
					x[ind[i]] = P[i] * x[ind[i + 1]] + Q[i];
				}
			}
			else {
				printf("error : n>t.rootWE[j].iN\n");
				printf("see function solveLR1_temp in my_LR.c file.\n");
				system("pause");
				exit(1);
			}

			// ������������ ����������� ������
			//if (a != NULL) {
				// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
				delete[] a;
				a = NULL;
			//}
			//if (b != NULL) {
			// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
				delete[] b;
				b = NULL;
			//}
			//if (c != NULL) {
				// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
				delete[] c;
				c = NULL;
			//}
			//if (d != NULL) {
				// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
				delete[] d;
				d = NULL;
			//}
			// ����������� ������������
			//if (P != NULL) {
				// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
				delete[] P;
				P = NULL;
			//}
			//if (Q != NULL) {
			// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
				delete[] Q;
				Q = NULL;
			//}
			// ���������� ����� ��������� (�����)
			//if (ind != NULL) {
				// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
				delete[] ind;
				ind = NULL;
			//}
		}

	} // while


	// SN direct

	#pragma omp parallel for shared (x, xc) schedule (guided) // 
	for (integer i1=0; i1<t.maxelm+t.maxbound; i1++) {
		xc[i1]=x[i1]; // copy x
	}
	
	// ������ �� ���� ������:
	#pragma omp parallel for shared (t, rthdsd, alphaP, x, xc) schedule (guided) // 
	for (integer j=0; j<t.iSN; j++) {
		// printf("Ok\n"); // debug

		integer n=t.rootSN[j].iN; // ������������� � ������ ������� ������ ��� �������� ����������������.

		// ������� ������ ��� ����������� ���������:
		doublereal *a=NULL, *b=NULL, *c=NULL, *d=NULL; // ��������������� ������� � ������ �����
	    doublereal *P=NULL, *Q=NULL; // ����������� ������������
	    integer *ind=NULL; // ����� � ���������� ���������� �����.
		// ���������������� �������
	    a=new doublereal[n];
	    b=new doublereal[n];
	    c=new doublereal[n];
	    d=new doublereal[n];
	    // ����������� ������������
	    P=new doublereal[n];
	    Q=new doublereal[n];
	    // ���������� ����� ��������� (�����)
	    ind= new integer[n];

		integer iP=-1, i=-1;

		if ((a != NULL) && (b != NULL) && (c != NULL) && (d != NULL) && (P != NULL) && (Q != NULL) && (ind != NULL)) {

			

			NODELR* r2 = t.rootSN[j].root;

			if (!t.rootSN[j].bNeimanStart) {
				iP = t.sosedi[SSIDE][r2->id].iNODE1; // ����� ���������� ����
				a[0] = t.slau_bon[iP - t.maxelm].aw;///alphaP;
				if (t.slau_bon[iP - t.maxelm].iI > -1) {
					b[0] = t.slau_bon[iP - t.maxelm].ai;
					a[0] = a[0] / alphaP;
				}
				else b[0] = 0.0;
				d[0] = rthdsd[iP]; //f.slau_bon[iVar][iP-f.maxelm].b;
				ind[0] = iP;
				c[0] = 0.0;// �.�. ��������� �������:
			}
			else {
				//printf("bug\n"); getchar(); // debug
				// ��������� ����� � ����������
				iP = r2->id;
				ind[0] = iP;
				a[0] = t.slau[iP].ap / alphaP;
				b[0] = t.slau[iP].an;
				c[0] = 0.0; //t.slau[iP].as; // ��������� ����� � ����������
				d[0] = t.slau[iP].ae*xc[t.sosedi[ESIDE][iP].iNODE1];
				d[0] += t.slau[iP].aw*xc[t.sosedi[WSIDE][iP].iNODE1];
				d[0] += t.slau[iP].at*xc[t.sosedi[TSIDE][iP].iNODE1];
				d[0] += t.slau[iP].ab*xc[t.sosedi[BSIDE][iP].iNODE1];
				d[0] += rthdsd[iP]; //f.slau[iVar][iP].b;

				r2 = r2->next;
			}

			i = 1;

			while (r2->next != NULL) {
				iP = r2->id;
				if (i < t.rootSN[j].iN) {
					ind[i] = iP;
					a[i] = t.slau[iP].ap / alphaP;
					b[i] = t.slau[iP].an;
					c[i] = t.slau[iP].as;
					d[i] = t.slau[iP].ae*xc[t.sosedi[ESIDE][iP].iNODE1];
					d[i] += t.slau[iP].aw*xc[t.sosedi[WSIDE][iP].iNODE1];
					d[i] += t.slau[iP].at*xc[t.sosedi[TSIDE][iP].iNODE1];
					d[i] += t.slau[iP].ab*xc[t.sosedi[BSIDE][iP].iNODE1];
					d[i] += rthdsd[iP]; //f.slau[iVar][iP].b;
				}
				else {
					printf("i>=t.rootSN[j].iN in solveLR1_temp in my_LR.c\n");
					printf("model incorrect\n");
					system("pause");
					exit(1);
				}

				r2 = r2->next;


				i++;
			}


			if (!t.rootSN[j].bNeimanEnd) {
				if (r2 != NULL) {
					iP = r2->id;
					if (i<t.rootSN[j].iN) {
						ind[i] = iP;
						a[i] = t.slau[iP].ap / alphaP;
						b[i] = t.slau[iP].an;
						c[i] = t.slau[iP].as;
						d[i] = t.slau[iP].ae*xc[t.sosedi[ESIDE][iP].iNODE1];
						d[i] += t.slau[iP].aw*xc[t.sosedi[WSIDE][iP].iNODE1];
						d[i] += t.slau[iP].at*xc[t.sosedi[TSIDE][iP].iNODE1];
						d[i] += t.slau[iP].ab*xc[t.sosedi[BSIDE][iP].iNODE1];
						d[i] += rthdsd[iP]; //f.slau[iVar][iP].b;
						i++;
						iP = t.sosedi[NSIDE][r2->id].iNODE1; // ����� ���������� ����
						a[i] = t.slau_bon[iP - t.maxelm].aw;///alphaP;
						if (t.slau_bon[iP - t.maxelm].iI > -1) {
							c[i] = t.slau_bon[iP - t.maxelm].ai;
							a[i] = a[i] / alphaP;
						}
						else c[i] = 0.0;
						d[i] = rthdsd[iP]; //f.slau_bon[iVar][iP-f.maxelm].b;
						//printf("source=%e\n",t.d[i]); getchar(); // debug
						ind[i] = iP;
						b[i] = 0.0; // �.�. ��������� �������
					}
				}
				else {
					printf("error 2 : r2==NULL : in solveLR1_temp in my_LR.c file\n");
					system("pause");
					exit(1);
				}
			}
			else {
				//printf("bug\n"); getchar(); // debug
				if (r2 != NULL) {
					iP = r2->id;
					if (i < t.rootSN[j].iN) {
						ind[i] = iP;
						a[i] = t.slau[iP].ap / alphaP;
						b[i] = 0.0; //t.slau[iP].an; ������ ����� � ����������
						c[i] = t.slau[iP].as;
						d[i] = t.slau[iP].ae*xc[t.sosedi[ESIDE][iP].iNODE1];
						d[i] += t.slau[iP].aw*xc[t.sosedi[WSIDE][iP].iNODE1];
						d[i] += t.slau[iP].at*xc[t.sosedi[TSIDE][iP].iNODE1];
						d[i] += t.slau[iP].ab*xc[t.sosedi[BSIDE][iP].iNODE1];
						d[i] += rthdsd[iP]; //f.slau[iVar][iP].b;
					}
				}
				else {
					printf("error r2==NULL in solveLR1_temp in my_LR.c file\n");
					system("pause");
					exit(1);
				}
			}
			n = i + 1;

			r2 = NULL; // ���� ��������� ������ �� �����


			// ���������� ���� �������� ��� �������� ������:
			P[0] = b[0] / a[0];
			Q[0] = d[0] / a[0];
			doublereal rdiv=0.0;
			if (n <= t.rootSN[j].iN) {

			    for (i = 1; i < n; i++) {
				    rdiv = 1.0 / (a[i] - c[i] * P[i - 1]);
				    P[i] = b[i] * rdiv;
				    Q[i] = (d[i] + c[i] * Q[i - 1])*rdiv;
			    }
			    // printf("Ok2\n"); // debug
			
				if (ind[n - 1] < t.maxelm + t.maxbound) {
					x[ind[n - 1]] = Q[n - 1];
				}
				else {
					printf("my_LR.c solveLR1_temp ind[n-1]>=t.maxelm+t.maxbound\n");
					system("pause");
					exit(1);
				}
			
			    //x[t.ind[n-1]]=(t.d[n-1]+t.c[n-1]*t.Q[n-2])/(t.a[n-1]-t.c[n-1]*t.P[n-2]); // ����������� �������� t.Q[n-1].
			    //printf("T==%e, %e, d==%e\n",x[t.ind[n-1]],t.Q[n-1],t.d[n-1]); getchar(); // debug
			    for (i = n - 2; i >= 0; i--) {
			    	x[ind[i]] = P[i] * x[ind[i + 1]] + Q[i];
			    }
			}
			else {
				printf("error : n>t.rootSN[j].iN  in solveLR1_temp in my_LR.c file\n");
				system("pause");
				exit(1);
			}

			// ������������ ����������� ������
			//if (a != NULL) {
				// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
				delete[] a;
				a = NULL;
			//}
			//if (b != NULL) {
			// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
				delete[] b;
				b = NULL;
			//}
			//if (c != NULL) {
			// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
				delete[] c;
				c = NULL;
			//}
			//if (d != NULL) {
				// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
				delete[] d;
				d = NULL;
			//}
			// ����������� ������������
			//if (P != NULL) {
				// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
				delete[] P;
				P = NULL;
			//}
			//if (Q != NULL) {
				// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
				delete[] Q;
				Q = NULL;
			//}
			// ���������� ����� ��������� (�����)
			//if (ind != NULL) {
				// �������� delete ����� ���� �������� �������� � ��� ����� � � �������� ���������. �������� �� null �������.
				delete[] ind;
				ind = NULL;
			//}
		}

	} // for


	// BT direct

	#pragma omp parallel for shared (x, xc) schedule (guided) // 
	for (integer i1=0; i1<t.maxelm+t.maxbound; i1++) {
		xc[i1]=x[i1]; // copy x
	}

	// ������ �� ���� ������:
	#pragma omp parallel for shared (t, rthdsd, alphaP, x, xc) schedule (guided) // 
	for (integer j=0; j<t.iBT; j++) {

		integer n=t.rootBT[j].iN; // �������������. � ������ ������� ������ ��� ��������� ����� ��������������.

		// ������� ������ ��� ����������� ���������:
		doublereal *a=NULL, *b=NULL, *c=NULL, *d=NULL; // ��������������� ������� � ������ �����
	    doublereal *P=NULL, *Q=NULL; // ����������� ������������
	    integer *ind=NULL; // ����� � ���������� ���������� �����.
		// ���������������� �������
	    a=new doublereal[n];
	    b=new doublereal[n];
	    c=new doublereal[n];
	    d=new doublereal[n];
	    // ����������� ������������
	    P=new doublereal[n];
	    Q=new doublereal[n];
	    // ���������� ����� ��������� (�����)
	    ind= new integer[n];

		integer iP = -1, i = -1;

		// ����� ���������� ��������� new �� ��������� ������ �������� �� null.
		//if ((a != NULL) && (b != NULL) && (c != NULL) && (d != NULL) && (P != NULL) && (Q != NULL) && (ind != NULL))
		{

			

			NODELR* r2 = t.rootBT[j].root;

			if (!t.rootBT[j].bNeimanStart) {
				iP = t.sosedi[BSIDE][r2->id].iNODE1; // ����� ���������� ����
				a[0] = t.slau_bon[iP - t.maxelm].aw;///alphaP;
				if (t.slau_bon[iP - t.maxelm].iI > -1) {
					b[0] = t.slau_bon[iP - t.maxelm].ai;
					a[0] = a[0] / alphaP;
				}
				else b[0] = 0.0;
				d[0] = rthdsd[iP]; //f.slau_bon[iVar][iP-f.maxelm].b;
				ind[0] = iP;
				c[0] = 0.0; // �.�. ��������� �������:

			}
			else {
				//printf("bug\n"); getchar(); // debug
				// ������ ����� � ���������� �����.
				iP = r2->id;
				ind[0] = iP;
				a[0] = t.slau[iP].ap / alphaP;
				b[0] = t.slau[iP].at;
				c[0] = 0.0; // t.slau[iP].ab; // ������ ����� � ���������� �����
				d[0] = t.slau[iP].ae*xc[t.sosedi[ESIDE][iP].iNODE1];
				d[0] += t.slau[iP].aw*xc[t.sosedi[WSIDE][iP].iNODE1];
				d[0] += t.slau[iP].an*xc[t.sosedi[NSIDE][iP].iNODE1];
				d[0] += t.slau[iP].as*xc[t.sosedi[SSIDE][iP].iNODE1];
				d[0] += rthdsd[iP]; //f.slau[iVar][iP].b;

				r2 = r2->next;
			}

			i = 1;

			while (r2->next != NULL) {
				iP = r2->id;
				if (i < n) {
					ind[i] = iP;
					a[i] = t.slau[iP].ap / alphaP;
					b[i] = t.slau[iP].at;
					c[i] = t.slau[iP].ab;
					d[i] = t.slau[iP].ae*xc[t.sosedi[ESIDE][iP].iNODE1];
					d[i] += t.slau[iP].aw*xc[t.sosedi[WSIDE][iP].iNODE1];
					d[i] += t.slau[iP].an*xc[t.sosedi[NSIDE][iP].iNODE1];
					d[i] += t.slau[iP].as*xc[t.sosedi[SSIDE][iP].iNODE1];
					d[i] += rthdsd[iP]; //f.slau[iVar][iP].b;
				}
				else {
					printf("error i>=n in my_LR.c in solveLR1_temp\n");
					system("pause");
					exit(1);
				}

				r2 = r2->next;


				i++;
			}

			if (i<n) {

				if (!t.rootBT[j].bNeimanEnd) {
					iP = r2->id;
					ind[i] = iP;
					a[i] = t.slau[iP].ap / alphaP;
					b[i] = t.slau[iP].at;
					c[i] = t.slau[iP].ab;
					d[i] = t.slau[iP].ae*xc[t.sosedi[ESIDE][iP].iNODE1];
					d[i] += t.slau[iP].aw*xc[t.sosedi[WSIDE][iP].iNODE1];
					d[i] += t.slau[iP].an*xc[t.sosedi[NSIDE][iP].iNODE1];
					d[i] += t.slau[iP].as*xc[t.sosedi[SSIDE][iP].iNODE1];
					d[i] += rthdsd[iP]; //f.slau[iVar][iP].b;
					i++;
					iP = t.sosedi[TSIDE][r2->id].iNODE1; // ����� ���������� ����
					a[i] = t.slau_bon[iP - t.maxelm].aw;//alphaP;
					if (t.slau_bon[iP - t.maxelm].iI > -1) {
						c[i] = t.slau_bon[iP - t.maxelm].ai;
						a[i] = a[i] / alphaP;
					}
					else c[n - 1] = 0.0;
					d[i] = rthdsd[iP]; //f.slau_bon[iVar][iP-f.maxelm].b;
					ind[i] = iP;
					b[i] = 0.0; // �.�. ��������� �������:
				}
				else
				{
					//printf("bug\n"); getchar(); // debug
					iP = r2->id;
					ind[i] = iP;
					a[i] = t.slau[iP].ap / alphaP;
					b[i] = 0.0; //t.slau[iP].at; // ������ ����� � ����������
					c[i] = t.slau[iP].ab;
					d[i] = t.slau[iP].ae*xc[t.sosedi[ESIDE][iP].iNODE1];
					d[i] += t.slau[iP].aw*xc[t.sosedi[WSIDE][iP].iNODE1];
					d[i] += t.slau[iP].an*xc[t.sosedi[NSIDE][iP].iNODE1];
					d[i] += t.slau[iP].as*xc[t.sosedi[SSIDE][iP].iNODE1];
					d[i] += rthdsd[iP]; //f.slau[iVar][iP].b;
				}
				n = i + 1;
			}
			else {
				printf("i index >=n error\n");
				printf("my_LR.c function solveLR1_temp\n");
				system("pause");
				exit(1);
			}

			r2 = NULL; // ���� ��������� ������ �� �����

			// ���������� ���� �������� ��� �������� ������:
			P[0] = b[0] / a[0];
			Q[0] = d[0] / a[0];
			doublereal rdiv;

			if (n <= t.rootBT[j].iN) {

			    for (i = 1; i < n; i++) {
				    rdiv = 1.0 / (a[i] - c[i] * P[i - 1]);
			    	P[i] = b[i] * rdiv;
			    	Q[i] = (d[i] + c[i] * Q[i - 1])*rdiv;
			    }


			
				if ((ind[n - 1] < t.maxelm + t.maxbound) && (ind[n - 1] >= 0)) {
					x[ind[n - 1]] = Q[n - 1];
				}
				else {
					printf("ind[n-1] ne leshit v dopustimjh predelax\n");
					printf("see function solveLR1_temp in my_LR.c file in source code\n");
					system("pause");
					exit(1);
				}
				for (i = n - 2; i >= 0; i--) {
					x[ind[i]] = P[i] * x[ind[i + 1]] + Q[i];
				}
			}
			else {
				printf("error : n>t.rootBT[j].iN in my_LR.c file in solveLR1_temp function\n");
				system("pause");
				exit(1);
			}


			// ������������ ����������� ������
			if (a != NULL) {
				delete[] a;
			}
			if (b != NULL) {
				delete[] b;
			}
			if (c != NULL) {
				delete[] c;
			}
			if (d != NULL) {
				delete[] d;
			}
			// ����������� ������������
			if (P != NULL) {
				delete[] P;
			}
			if (Q != NULL) {
				delete[] Q;
			}
			// ���������� ����� ��������� (�����)
			if (ind != NULL) {
				delete[] ind;
			}

		}
		//else {
			//printf("memory no allocate for : a,b,c,d,P,Q,ind in my_LR.c file in function solveLR1_temp\n");
			//system("pause");
			//exit(1);
		//}

	} // for

	if (xc != NULL) {
		delete[] xc; // ������������ ����������� ������
	}
    
} // solveLR1_temp

doublereal fmaxloc(doublereal dA, doublereal dB) {
	doublereal dr=dA;
	if (dB>dA) dr=dB;
	return(dr);
}

// ������ ��������� �������� ������������� ������:
void solveLRn(doublereal* &x, doublereal* &rthdsd, integer ns, integer iVar, integer maxit, bool bprintf, bool bnorelax,
	ALICE_PARTITION** sosedi, integer maxelm, equation3D** slau, equation3D_bon** slau_bon,
			   integer** iN, integer***id, integer iWE, integer iSN, integer iBT, doublereal *alpha, integer maxbound) {
    // bprintf - �������� ���������� � ���� ��� ���.
	// ���� ������������ ����� ������������ �������������� �� ��������, �
	// ���� �� ������������ ��� ������������������� �� �� ��������.

	

	bool only_serial = true;

	// ���������� bdopfinsh �������� ���������������� ���� ��������������
	// ������� ������ �� ����� ������������������.
	// ���� bdopfinish==false �� �������������� ������� ���������.
	// ���� bdopfinish==true �� �������������� ������� ��������.
	bool bdopfinish=true;

	integer i=0; // ������� ���������� ��������
	if (bprintf) {
		printf("LR1 start solution...\n");
		fprintf(fp_log,"LR1 start solution...\n");
	}
	doublereal e=dterminatedTResudual;
	doublereal dresgl=1.0;
	doublereal divres=1.0; //  ������� � �������� ����� ����� ���������� ����������
	// ������ �� ����� ����� ��������
	while (((i<maxit) && (!bdopfinish || ((fabs(dresgl)>e) && (divres>(e*1e-25))))) || (i<1)) {
		// ��� ������� CFD �����, �������� �������� ������ ���� ���������� ��� 
		// sor � ���� ������� ���� ��� ���������� ��� ����� �����.
		if (only_serial) {
			doublereal omega = 1.0;
			switch (iVar) {
			case VX:  omega = 1.0; break;
			case VY:  omega = 1.0;  break;
			case VZ:  omega = 1.0; break;
			case PAM: omega = 1.7; break; // 1.855
			default:  omega = 1.0;  break;
			}
			omega = 0.7; // ��� ����� ������ (����� ������� ���������� � ���������� ������).
			// ������ ���������� 0.7 ��������� ����������� �� ���� ��������� �� �������� ���������� cfd ���������.
			// ���� �������� �� ��� ������������ �� ����� ������� ��������� �������� ������ ���������� omega.
			solveLR1_serial_sor(x, rthdsd, ns, iVar, bnorelax, sosedi, maxelm, slau, slau_bon, iN, id, iWE, iSN, iBT, alpha, maxbound, omega);
		}
		else {
			// ����� ����� �� ��� � ���������������� ����������� ���� ����� ��� ����������.
			solveLR1(x, rthdsd, ns, iVar, bnorelax, sosedi, maxelm, slau, slau_bon, iN, id, iWE, iSN, iBT, alpha, maxbound);
		}

		integer j=0;
		//doublereal rmax=-1.0; // �������� �������
		doublereal dsum=0.0;

		// ������� ������������� � LR1sk ��������.
		doublereal* residual = NULL;
		residual = new doublereal[maxelm + maxbound];
		for (j=0; j<maxelm+maxbound; j++) residual[j]=0.0; // �������������.

		// ��������� �������:
		// 1.
		// ���������� ����������� ������:
        #pragma omp parallel for shared (maxelm, slau, sosedi, rthdsd, x, residual) private (j) schedule (guided) //reduction (+:dsum)  
		for (j=0; j<maxelm; j++) {
			doublereal dbuf=0.0;
			//rmax=fmaxloc(rmax,fabs(f.slau[iVar][j].ap*x[j]-f.slau[iVar][j].ab*x[f.sosedi[BSIDE][j].iNODE1]-f.slau[iVar][j].at*x[f.sosedi[TSIDE][j].iNODE1]-f.slau[iVar][j].an*x[f.sosedi[NSIDE][j].iNODE1]-f.slau[iVar][j].as*x[f.sosedi[SSIDE][j].iNODE1]-f.slau[iVar][j].ae*x[f.sosedi[ESIDE][j].iNODE1]-f.slau[iVar][j].aw*x[f.sosedi[WSIDE][j].iNODE1]-f.slau[iVar][j].b));
			dbuf = fabs(slau[iVar][j].ap*x[j] - slau[iVar][j].ab*x[sosedi[BSIDE][j].iNODE1] - slau[iVar][j].at*x[sosedi[TSIDE][j].iNODE1] - slau[iVar][j].an*x[sosedi[NSIDE][j].iNODE1] - slau[iVar][j].as*x[sosedi[SSIDE][j].iNODE1] - slau[iVar][j].ae*x[sosedi[ESIDE][j].iNODE1] - slau[iVar][j].aw*x[sosedi[WSIDE][j].iNODE1] - rthdsd[j]); //-f.slau[iVar][j].b
			//dbuf=dbuf*dbuf;
			//dsum+=dbuf;
			residual[j]=dbuf;
		}
		// 2.
		// ��������� ����������� ������:
		doublereal dsumbuf=dsum;
		dsum=0.0;
		#pragma omp parallel for shared (maxelm, maxbound, slau_bon, rthdsd, x, residual) private (j) schedule (guided) //reduction (+:dsum) 
		for (j=maxelm; j<maxelm+maxbound; j++) {
			doublereal dbuf=0.0;
			if (slau_bon[iVar][j-maxelm].iI>-1) {
				dbuf=fabs(slau_bon[iVar][j-maxelm].aw*x[j]-slau_bon[iVar][j-maxelm].ai*x[slau_bon[iVar][j-maxelm].iI]-rthdsd[j]); // -f.slau_bon[iVar][j-f.maxelm].b
			}
			else {
				dbuf=fabs(slau_bon[iVar][j-maxelm].aw*x[j]-rthdsd[j]); // -f.slau_bon[iVar][j-f.maxelm].b
			}
			//dbuf=dbuf*dbuf;
			/*// ���� ����������� ������ ���������� � ��������� ����� ��  ��� � �������� ����� ������������ ���� ���
			  if (dbuf >1e+6) { // debug

			  #if doubleintprecision == 1
					printf("Error node =%lld, maxelm=%lld\n",j,f.maxelm);
					printf("iI=%lld, aw=%e, ai=%e, b=%e\n",f.slau_bon[iVar][j-f.maxelm].iI,f.slau_bon[iVar][j-f.maxelm].aw,f.slau_bon[iVar][j-f.maxelm].ai,f.slau_bon[iVar][j-f.maxelm].b);
			  #else
					printf("Error node =%d, maxelm=%d\n",j,f.maxelm);
					printf("iI=%d, aw=%e, ai=%e, b=%e\n",f.slau_bon[iVar][j-f.maxelm].iI,f.slau_bon[iVar][j-f.maxelm].aw,f.slau_bon[iVar][j-f.maxelm].ai,f.slau_bon[iVar][j-f.maxelm].b);
			  #endif

				printf("rthdsd=%e, x=%e\n",rthdsd[j],x[j]);
				getchar();
			}*/
			//dsum+=dbuf;
            residual[j]=dbuf;
		}
		//dsum+=dsumbuf;

		// ����� ������� �� �������� ������ ���� ����������� � �������� ����������.
		dsum=NormaV(residual, maxelm+maxbound); // ���������� ����� �������.
		if (residual != NULL) {
			delete[] residual; // ������������ ������ �� ��� ������� �������.
		}

		if (bprintf) {
		    if (i%10==0) {
				printf("iter residual\n");
				fprintf(fp_log,"iter residual\n");
			}
#if doubleintprecision == 1
			printf(" %lld  %e\n", i + 1, dsum);
			fprintf(fp_log, " %lld  %e\n", i + 1, dsum);
#else
			printf(" %d  %e\n", i + 1, dsum);
			fprintf(fp_log, " %d  %e\n", i + 1, dsum);
#endif

		    
		}
		divres=fabs(dresgl-dsum); // ������� � �������� ����� ����� ���������� ����������
		dresgl=dsum;

		i++;
	}

    // � ���� ���� ������� ���������� �� ���������� ������� ����:
	if (bprintf) {
		//fprintf(fp_statistic_convergence, " LR1 method finish residual=%e \n",dresgl);
	    fprintf(fp_statistic_convergence,"%e ",dresgl);
	}

} // solveLRn

// ������ ��������� �������� ������������� ������:
void solveLRn_temp(TEMPER &t, doublereal* &x, doublereal* &rthdsd, integer ns, integer maxit, bool bprintf) {
    // bprintf - �������� ���������� � ���� ��� ���.
	// ���� ������������ ����� ������������ �������������� �� ��������, �
	// ���� �� ������������ ��� ������������������� �� �� ��������.

	// ���������� bdopfinsh �������� ���������������� ���� ��������������
	// ������� ������ �� ����� ������������������.
	// ���� bdopfinish==false �� �������������� ������� ���������.
	// ���� bdopfinish==true �� �������������� ������� ��������.
	bool bdopfinish=true;

	// �� �������� ��������� ��� ���� �������. ����� �����
	// ������� � ��� ����� �� �� ����� �� ������� ������������ ����������, �.�.
	// ����� �� ���� ������������. �� ���� ���������� ������� ������������������� 
	// ����� �������� ������� � ��������� LR1sk. ����� ������� ����� ��� �������� 
	// �� ���� ������ �����.


	integer i=0; // ������� ���������� ��������
	if (bprintf) {
		printf("LR1 start solution...\n");
		fprintf(fp_log,"LR1 start solution...\n");
	}
	doublereal e=dterminatedTResudual; // ����� ������ ���� ����������� � LR1sk ��������.
	doublereal dresgl=1.0;
	doublereal divres=1.0; //  ������� � �������� ����� ����� ���������� ����������
	// ������ �� ����� ����� ��������
	while (((i<maxit) && (!bdopfinish || ((fabs(dresgl)>e) && (divres>(e*1e-25))))) || (i<1)) {
		solveLR1_temp(t, x, rthdsd, ns);
		integer j=0;
		//doublereal rmax=-1.0; // �������� �������
		doublereal dsum=0.0;
		
		// ������� ������������� � LR1sk ��������.
		doublereal* residual=new doublereal[t.maxelm+t.maxbound];
		for (j=0; j<t.maxelm+t.maxbound; j++) residual[j]=0.0; // �������������.

		// ��������� �������:
		// 1.
		// ���������� ����������� ������:
		#pragma omp parallel for shared (t,rthdsd,x,residual) private(j) schedule (guided) //reduction (+:dsum)
		for (j=0; j<t.maxelm; j++) {
			doublereal dbuf=0.0;

			//rmax=fmaxloc(rmax,fabs(f.slau[iVar][j].ap*x[j]-
			// f.slau[iVar][j].ab*x[f.sosedi[BSIDE][j].iNODE1]-
			// f.slau[iVar][j].at*x[f.sosedi[TSIDE][j].iNODE1]-
			// f.slau[iVar][j].an*x[f.sosedi[NSIDE][j].iNODE1]-
			// f.slau[iVar][j].as*x[f.sosedi[SSIDE][j].iNODE1]-
			// f.slau[iVar][j].ae*x[f.sosedi[ESIDE][j].iNODE1]-
			// f.slau[iVar][j].aw*x[f.sosedi[WSIDE][j].iNODE1]-
			// f.slau[iVar][j].b)); // ��� rthdsd[j]);
			dbuf=fabs(t.slau[j].ap*x[j]-
				t.slau[j].ab*x[t.sosedi[BSIDE][j].iNODE1] -
				t.slau[j].at*x[t.sosedi[TSIDE][j].iNODE1] -
				t.slau[j].an*x[t.sosedi[NSIDE][j].iNODE1] -
				t.slau[j].as*x[t.sosedi[SSIDE][j].iNODE1] -
				t.slau[j].ae*x[t.sosedi[ESIDE][j].iNODE1] -
				t.slau[j].aw*x[t.sosedi[WSIDE][j].iNODE1] -
					  rthdsd[j]);

			//dbuf=dbuf*dbuf;
			//dsum+=dbuf;
			residual[j]=dbuf;
		}
		//printf("sum=%e\n",dsum); getchar();
		// 2.
		//doublereal dbufsum=dsum;
		//dsum=0.0;
		// ��������� ����������� ������:
		#pragma omp parallel for shared (t,rthdsd,x,residual) private(j) schedule (guided) //reduction (+:dsum)
		for (j=t.maxelm; j<t.maxelm+t.maxbound; j++) {
			
			doublereal dbuf=0.0;
			if (t.slau_bon[j-t.maxelm].iI>-1) {
				dbuf=fabs(t.slau_bon[j-t.maxelm].aw*x[j]-
					      t.slau_bon[j-t.maxelm].ai*x[t.slau_bon[j-t.maxelm].iI]-
						  rthdsd[j]);
			}
			else {
				dbuf=fabs(t.slau_bon[j-t.maxelm].aw*x[j]-rthdsd[j]);
			}
			//dbuf=dbuf*dbuf;
			/*// ���� ����������� ������ ���������� � ��������� ����� ��  ��� � �������� ����� ������������ ���� ���
			  if (dbuf >1e+6) { // debug
			  #if doubleintprecision == 1
					 printf("Error node =%lld, maxelm=%lld\n",j,f.maxelm);
					 printf("iI=%lld, aw=%e, ai=%e, b=%e\n",f.slau_bon[iVar][j-f.maxelm].iI,f.slau_bon[iVar][j-f.maxelm].aw,f.slau_bon[iVar][j-f.maxelm].ai,f.slau_bon[iVar][j-f.maxelm].b);
			  #else
					 printf("Error node =%d, maxelm=%d\n",j,f.maxelm);
				     printf("iI=%d, aw=%e, ai=%e, b=%e\n",f.slau_bon[iVar][j-f.maxelm].iI,f.slau_bon[iVar][j-f.maxelm].aw,f.slau_bon[iVar][j-f.maxelm].ai,f.slau_bon[iVar][j-f.maxelm].b);
			  #endif

				printf("rthdsd=%e, x=%e\n",rthdsd[j],x[j]);
				getchar();
			}*/
			//dsum+=dbuf;
			residual[j]=dbuf;
		}
		//dsum+=dbufsum;

		// ����� ������� �� �������� ������ ���� ����������� � �������� ����������.
		dsum=NormaV(residual, t.maxelm+t.maxbound); // ���������� ����� �������.
		if (residual != NULL) {
			delete[] residual; // ������������ ������ �� ��� ������� �������.
			residual = NULL;
		}


		if (bprintf) {
#if doubleintprecision == 1
			if (i % 10 == 0) {
				printf("iter residual\n");
				fprintf(fp_log, " %lld  %e\n", i + 1, dsum);
		    }
			printf(" %lld  %e\n", i + 1, dsum); //getchar();
			fprintf(fp_log, " %lld  %e\n", i + 1, dsum);
#else
			if (i % 10 == 0) {
				printf("iter residual\n");
				fprintf(fp_log, " %d  %e\n", i + 1, dsum);
			}
			printf(" %d  %e\n", i + 1, dsum); //getchar();
			fprintf(fp_log, " %d  %e\n", i + 1, dsum);
#endif

		  
		}
		divres=fabs(dresgl-dsum); // ������� � �������� ����� ����� ���������� ����������
		dresgl=dsum;

		i++;
	}

    // � ���� ���� ������� ���������� �� ���������� ������� ����:
	//if (bprintf) {
		//fprintf(fp_statistic_convergence, " LR1 method finish residual=%e \n",dresgl);
	  //  fprintf(fp_statistic_convergence,"%e ",dresgl);
	//}

} // solveLRn_temp

#endif