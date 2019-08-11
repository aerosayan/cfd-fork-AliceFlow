// ���� ilut.c
// ILUT preconditioner (�������������������)
// ������ 3 ���� 2011.
// 31 ����� 2013 ����. ��� �������� ������ ��� ����� �.����� �� �������.
// ����� ������������ ������� ���������� ��� ���������� SPARSKIT2 � ��� AliceFlowv0_07
// � ������� ������������� ��������� f2c.exe ��������� � ���������.

#include "iluk.c" // ILUK decomposition

// quick-sort split
// 31 ����� 2013 ����.
void qsplit(doublereal* a, integer* ind, integer n, integer ncut) {

    /* ��������� ���������� */
	doublereal tmp, abskey;
	integer itmp, first, last;
	integer mid, j;


    /* ----------------------------------------------------------------------- */
    /*     does a quick-sort split of a real array. */
    /*     on input a(1:n). is a real array */
    /*     on output a(1:n) is permuted such that its elements satisfy: */

    /*     abs(a(i)) .ge. abs(a(ncut)) for i .lt. ncut and */
    /*     abs(a(i)) .le. abs(a(ncut)) for i .gt. ncut */

    /*     ind(1:n) is an integer array which permuted in the same way as a(*). */
    /* ----------------------------------------------------------------------- */
    /* ----- */
    /*
    ��������� ������� ���������������:
    ������ ������� �������������� ������������� ������� a.
    �� ����� ������������ ������ a ���������� ���������� � �������� 1:n ������������.
    �� ������ ����������������� ������ a �������� �������� ������������� ���������� �������:

    fabs(a[i])>=fabs(a[ncut]) ��� ���� i < ncut � �����
    fabs(a[i])<=fabs(a[ncut]) ��� ���� i > ncut.

    ind ��� ������������� ������ ������� ������ ������� ������������ ��������� ������� a.
    ind ���� ����� �������� ���������� � ������� 1:n ������������.

    ������ �������� ������� ��������, ��� ��� ������� ���������� � 1 � ������������� ��������� n !!!. 
    */


    // ��������� ��� ����� �� ���� � ��� ����, ��� ��������� ����� ���������� f2c.exe.
    /* � ����� �� ��������� ������� ��������� �� ������ ������� ���������� �������� ������������ �������� ����.
    ����� ������� ���� �� ������� ��������� ������ �� ���� ������� ����� �� ������ �������, ������� ������ ���� ������ ����
    ������ ����� ����� ������ 1 � ������������� ��������� ������� ����� ����� ����� n. ������ ����� ���� � ���� ��������� ��� �. �����.
    � ����� ���� ���� ������� �� �� ���� �����, �.�. ����� ����� ���������� ���������� � ����. ��� ����� ���� �������� �������� ++ind; ++a;
    */
    /* Parameter adjustments */
    --ind; // �������� �����, � ���� ������� ����� ����������� ������� �������������������.
    --a; // ������� ���� � ���� ��� �������, ��������� �������� � ������������� �����������.

	first=1;
	last=n;
	if (((ncut >= first) && (ncut <= last))) {
		do { // ������� ���� ���� mid �� ����� ncut ������.
			mid=first;
			abskey=fabs(a[mid]);
			for (j=first+1; j<=last; j++) {
				if (fabs(a[j])>abskey) {
					mid++;
                    // ������ ������� �������� � �������� mid � j.
				    tmp=a[mid];
				    itmp=ind[mid];
				    a[mid]=a[j];
				    ind[mid]=ind[j];
				    a[j]=tmp;
				    ind[j]=itmp;
			    }
			}

            // ������ ������� ������� mid � first:

			tmp=a[mid];
			a[mid]=a[first];
			a[first]=tmp;

			itmp=ind[mid];
			ind[mid]=ind[first];
			ind[first]=itmp;

            // ��������� ������� ��������� �����.


			if (mid > ncut) {
				last=mid-1;
			}
			else if (mid < ncut) {
				first=mid+1;
			}

		} while (mid != ncut);
	}

    // ����������� �� ���������� � ����� ��.
    // ������ ������ ������� ����� ����� ������ 0, � ��������� ������ n-1; !!!
    // � ������ �������������� � ������� ind ���� !.
    ++ind;
    ++a; 
	
} // qsplit


// ��� ������ ilut ������� ���������� � �������� �� ���� ��.
// �������� � ���, ��� � �� ������ � ������������ ��������.
// ������� ���������� � ���, ��� � ����� �� ���������� � ������� � �� ����� ��� � �� ��� ������� ���������� � ����.
// 31 ����� 2013 ���� ���� ������� ����������������� �������� ��������� � ������� ������� f2c.exe.
void ilut(integer n, doublereal* &a, integer* &ja, integer* &ia, 
		  integer lfil, doublereal droptol, integer &iwk, 
		  doublereal* &alu, integer* &jlu, integer* &ju,
		  doublereal* &w, integer* &jw, integer &ierr)
{
	/*
	* ������� ������:
	* n - ������ ������� ���� ��� ����� ���������.
	* a,ja,ia - ������� ���� � CRS �������. � 
	*          ������������ ���������.
	* lfil - ������������ ����� ���������������
	*        ��������� � ������ ������ L � U.
	*        lfil ������ ���� >= 0.
	* droptol - ��������� �������� ��� ������.
	* iwk - ������������ ������ alu � jlu.
	*/

	/*
	* ������������ ��������: 
	* alu, jlu - ������� � ���������������� Sparse Row ������� (MSR).
	* ju - ������ ����� n, ����������� ������� ������ ������ ������ U
	*      � �������� alu,jlu.
	* ierr - ������������� ��� ������. ���������� 0 ��� �������� ����������.
	*
	* ���������������� �������� ����� (Modified Sparse Row, MSR) ���������� 
	* ������ ��� ������� : ������ alu - �������� ��������� ������� LU (�� 
	* ��������� L ����� �������) � ������������� ������ jlu. � ������ n 
	* �������� alu �������� ������������ �������� ������� U �� �������. 
	* ������� alu[n+1] �� ����������� ��� ���� �������������� ���������� � �������.
	* ������� � ������� n+2 ������������ ��������� �������� �������� ������� �� �������,
	* �������� ������������. ��� ������� �������� alu[k] ������� jlu[k] ���������� 
	* ���������� ������ � �������� �������.�� n+1 �������� ������� jlu ����������� ��������� 
	* ����� ��� ������ ������ ������� alu � jlu.
	*/

	/*
	*  ������� �������:
	*  jw - �������������� ������� ����� 2*n.
	*  w - ������������ ������� ����� n+1.
	*      1..ii-1 L part, ii..n U part.
	*/

	/*
	* ��������� �������� ������������ ������������� ���������������� �����. 
	* ������� ��������� ���������. ���� � ����������.
	*
	* 1) ����� ������� L � U �������� �������� ������ ���������� 
	*    ���������� �������� (���������� � ������ ��������������� ������) �������������. 
	* 2) ���������� ������ ���������� lfil ��������� �
	*    i-�� ������ L � U (�� ����������� ������������ ���������).
	* 
	* �������� ���������:
	* ����� ������������ droptol==0.0 �����
	* �������� ��������� ���������� �� ��������
	* ���������� ��������� � ������ ������ L && U.
	* �������� �� droptol!=0.0 �� lfil==n �������� �������
	* ��������� ��������� ��� ������ ���������.
	*/

	// ��������� ����������:
	integer ju0, k, j1, j2, j, ii, i, lenl, lenu, jj, jrow, jpos, len;
	doublereal tnorm, t,  s, fact; // absolute,

    /* Parameter adjustments */
    --jw;
    --w;
    --ju;
    --ia;
    --a;
    --ja;
    --alu;
    --jlu;


	if (lfil < 0) {
		// �������� lfil ����� �����������.
		printf("lfil incorrect detected.../n");
        //getchar();
		system("pause");
	}
	else {
       ju0 = n+2;
	   jlu[1] = ju0;

	   for (j=1; j<=n; j++) jw[n+j]=0; // ������������� ������� ��������� ��������.

	   // ������ main �����
	   for (ii=1; ii<=n; ii++) {
		   j1 = ia[ii]; // ������ ������ ii
		   j2 = ia[ii+1]-1; // ����� ������ ii
           tnorm=0.0; 
		   for (k=j1; k<=j2; k++) tnorm+=fabs(a[k]);
		   if (tnorm < 1e-30) {
			   // � ������ ��� �������� ����� ����
#if doubleintprecision == 1
			   printf("%lld is zero string in matrix.../n", ii - 1);
#else
			   printf("%d is zero string in matrix.../n", ii - 1);
#endif
			  
               //getchar();
			   system("pause");
			   break; // ��������� ����� �� ����� for
		   }
		   else {
			   tnorm/=(doublereal)(j2-j1+1);
               
			   // ���������� ������ ������ L � U 
			   // ��������������� ������ ������� A  
			   // � ������������ ������ w.
			   lenu=1;
			   lenl=0;
			   jw[ii]=ii;
			   w[ii]=0.0;
			   jw[n+ii]=ii;

			   for (j=j1; j<=j2; j++) {
				   k=ja[j];
				   t=a[j];
				   if (k<ii) {
					   lenl++;
					   jw[lenl]=k;
					   w[lenl]=t;
					   jw[n+k]=lenl;
				   }
				   else if (k==ii) w[ii]=t;
				   else {
					   lenu++;
					   jpos=ii+lenu-1;
					   jw[jpos]=k;
					   w[jpos]=t;
					   jw[n+k]=jpos;
				   }
			   }
			   jj=0;
			   len=0;

			   // �������� ���������� �����
			   bool bweShouldbeContinue=true;
			   while (bweShouldbeContinue) {
				   jj++; //FORTRAN 150

			       if (jj<=lenl) {

				       jrow=jw[jj];
				       k=jj;

				       for (j=jj+1; j<=lenl; j++) {
					       if (jw[j] < jrow) {
						       jrow=jw[j];
						       k=j;
					       }
				       }

				       if (k!=jj) {
					       j=jw[jj];
					       jw[jj]=jw[k];
					       jw[k]=j;

					       jw[n+jrow]=jj;
					       jw[n+j]=k;
                           // swap(w[jj],w[k]);
					       s=w[jj];
					       w[jj]=w[k];
					       w[k]=s;
				       }

				       jw[n+jrow]=0;

					   // ����� �������� ����� �� �������� � ������
                       fact=w[jj]*alu[jrow];
				   
				       if (fabs(fact) > droptol) { 

					      for (k=ju[jrow]; k<=jlu[jrow+1]-1; k++) {
						      s=fact*alu[k];
						      j=jlu[k];
						      jpos=jw[n+j];
						      if (j >= ii) {
							     if (jpos == 0) {
								     lenu++;
								     if (lenu > n) {
									      // ������� ������ �����������.
										  // �������� ����������.
										  printf(" Error!!! Matrix caused by incorrect.../n");
										  //getchar();
										  system("pause");
									      exit(0);
								     }
								     i=ii+lenu-1;
								     jw[i]=j;
								     jw[n+j]=i;
								     w[i]=-s;
							     }
							     else
							     {
								    w[jpos]-= s;
							     }
						       }
						       else 
						       {
							       if (jpos == 0) {
								       lenl++;
								       if (lenl > n) {
									       // ������� ������ �����������.
										  // �������� ����������.
										  printf(" Error!!! Matrix caused by incorrect.../n");
										  //getchar();
										  system("pause");
									      exit(0);
								       }
								       jw[lenl]=j;
								       jw[n+j]=lenl;
								       w[lenl]= -s;
							       }
							        else
							       {
								       w[jpos]-=s;
							       }
						       }
					      } // end for k

					      len++;
					      w[len]=fact;
					      jw[len]=jrow;
				       }

			       }
			       else {
				   
				      bweShouldbeContinue=false;

				      // ���������� ������� ��������� �� 0.
				      for (k=1; k<=lenu; k++) jw[n+jw[ii+k-1]]=0;

				      // �������� ������� L
				      lenl=len;
				      if (lenl < lfil) len=lenl; 
				      else len=lfil;

				      // sort by quick-split
				      //qsplit(w,jw,lenl,len);
                      qsplit(&w[1],&jw[1],lenl,len);

				      for (k=1; k<=len; k++) {
					      if (ju0 > iwk) {
						       // ������. ��������� ����� ��� ������� LU ����������.
						       printf("error! there is no plase for the matrix LU.../n");
						      // getchar();
							   system("pause");
					    	   exit(0);
					      }
					      else {
						      alu[ju0]=w[k];
						      jlu[ju0]=jw[k];
						      ju0++;
					      }
				      }

				      // �������� ��������� �� ������ ������ ii � ������� U
				      ju[ii]=ju0;

				      // ���������� ������� U - ������ ���������� ��������� ���������.
				      len=0;
				      for (k=1; k<=lenu-1; k++) {
					      if (fabs(w[ii+k])>droptol*tnorm) {
						      len++;
						      w[ii+len]=w[ii+k];
						      jw[ii+len]=jw[ii+k];
					      }
				      }
				      lenu=len+1;
                      // len=min(lenu,lfil);
				      if (lenu<lfil) len=lenu;
				      else len=lfil;

				      // � ��������� ���������� ����� �������
				      //qsplit(w[ii+1], jw[ii+1], lenu-1, len);
				      // ����� �� ������ �� 
				      //qsplit(w,jw,lenu-1,len);
                      // ������������� ������ �������� ���� �������������� ���������� � �������� �� �� � ������� f2c.exe.
                      qsplit(&w[ii+1],&jw[ii+1],lenu-1, len);

				      t=fabs(w[ii]);
				      if ((len+ju0)> iwk) {
                           // ������. ��������� ����� ��� ������� LU ����������.
					    	printf("error! there is no plase for the matrix LU.../n");
						   // getchar();
							system("pause");
						    exit(0);
				      }
				      for (k=ii+1; k<=ii+len-1; k++) {
					       jlu[ju0]=jw[k];
					       alu[ju0]=w[k];
					       t+=fabs(w[k]);
					       ju0++;
				      }

                      // ���� �� ��������� ����, �� �� ������ ����� ��� �� ����.
                      // �������� 1e-30 ������������ � � �������� ������� ��������.
                      if (fabs(w[ii])<1e-30) {
                          w[ii]=(droptol + 1.0e-4)*tnorm;
					  }

				      // � ������������ ������ ������������� 
				      // ������� �������� �������������.
				      alu[ii]=1.0/w[ii]; 

				      // ������� ��������� �� ������ ������ � ������� U
				      jlu[ii+1]=ju0;
			       }
			   } // while 
		   }

	   }  // ����� main �����

	}

    /* Parameter adjustments */
    ++jw;
    ++w;
    ++ju;
    ++ia;
    ++a;
    ++ja;
    ++alu;
    ++jlu;

} // ilut

// msrcsr  : converts modified sparse row format to compressed sparse   
//           row format.   
// ilut ����� ������� � msr �������, � ������� ������ crs.
/* ----------------------------------------------------------------------- */
/* Subroutine */ void msrcsr_(integer n, doublereal *a, integer *ja, 
	doublereal *ao, integer *jao, integer *iao, doublereal *wk, integer *iwk)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    integer i__, j, k, ii, iptr;
    bool added;
    integer idiag;

/* ----------------------------------------------------------------------- */
/*       Modified - Sparse Row  to   Compressed Sparse Row */

/* ----------------------------------------------------------------------- */
/* converts a compressed matrix using a separated diagonal */
/* (modified sparse row format) in the Compressed Sparse Row */
/* format. */
/* does not check for zero elements in the diagonal. */


/* on entry : */
/* --------- */
/* n          = row dimension of matrix */
/* a, ja      = sparse matrix in msr sparse storage format */
/*              see routine csrmsr for details on data structure */

/* on return : */
/* ----------- */

/* ao,jao,iao = output matrix in csr format. */

/* work arrays: */
/* ------------ */
/* wk       = real work array of length n */
/* iwk      = integer work array of length n+1 */

/* notes: */
/*   The original version of this was NOT in place, but has */
/*   been modified by adding the vector iwk to be in place. */
/*   The original version had ja instead of iwk everywhere in */
/*   loop 500.  Modified  Sun 29 May 1994 by R. Bramley (Indiana). */

/* ----------------------------------------------------------------------- */
    /* Parameter adjustments */
    --iwk;
    --wk;
    --iao;
    --a;
    --ja;
    --ao;
    --jao;

    /* Function Body */
    i__1 = n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	wk[i__] = a[i__];
	iwk[i__] = ja[i__];
/* L1: */
    }
    iwk[n + 1] = ja[n + 1];
    iao[1] = 1;
    iptr = 1;
/* --------- */
    i__1 = n;
    for (ii = 1; ii <= i__1; ++ii) {
	added = false;
	idiag = iptr + (iwk[ii + 1] - iwk[ii]);
	i__2 = iwk[ii + 1] - 1;
	for (k = iwk[ii]; k <= i__2; ++k) {
	    j = ja[k];
	    if (j < ii) {
		ao[iptr] = a[k];
		jao[iptr] = j;
		++iptr;
	    } else if (added) {
		ao[iptr] = a[k];
		jao[iptr] = j;
		++iptr;
	    } else {
/* add diag element - only reserve a position for it. */
		idiag = iptr;
		++iptr;
		added = true;
/*     then other element */
		ao[iptr] = a[k];
		jao[iptr] = j;
		++iptr;
	    }
/* L100: */
	}
	ao[idiag] = wk[ii];
	jao[idiag] = ii;
	if (! added) {
	    ++iptr;
	}
	iao[ii + 1] = iptr;
/* L500: */
    }
    
    ++iwk;
    ++wk;
    ++iao;
    ++a;
    ++ja;
    ++ao;
    ++jao;


/* ------------ end of subroutine msrcsr --------------------------------- */
/* ----------------------------------------------------------------------- */
} /* msrcsr_ */

// �������� LU ����������. ���������� ��� ������ ��� ������� ��������� ����������.
// �� ����� ������� � CRS ������� : a, ja, ia. 
// �� ������ ������� ILU0 ���������� � MSR ������� alu, jlu.
// ju - ��������� �� ������������ �������� � MSR.
// ierr - ������������ ��� ������. 
// iw - ������� ������ ����� n;
/* ---------------------------------------------------------------------- */
/* Subroutine */ integer ilu0_(integer n, doublereal* &a, integer* &ja, integer* &ia,
						   doublereal* &alu, integer* &jlu, integer* &ju, integer* &iw, integer &ierr)
{
    

    /* Local variables */
    integer i__, j, jf, ii, jj, jm, js;
    doublereal tl;
    integer jw, ju0, jcol, jrow;

/* ------------------ right preconditioner ------------------------------* */
/*                    ***   ilu(0) preconditioner.   ***                * */
/* ----------------------------------------------------------------------* */
/* Note that this has been coded in such a way that it can be used */
/* with pgmres. Normally, since the data structure of the L+U matrix is */
/* the same as that the A matrix, savings can be made. In fact with */
/* some definitions (not correct for general sparse matrices) all we */
/* need in addition to a, ja, ia is an additional diagonal. */
/* ILU0 is not recommended for serious problems. It is only provided */
/* here for comparison purposes. */
/* ----------------------------------------------------------------------- */

/* on entry: */
/* --------- */
/* n       = dimension of matrix */
/* a, ja, */
/* ia      = original matrix in compressed sparse row storage. */

/* on return: */
/* ----------- */
/* alu,jlu = matrix stored in Modified Sparse Row (MSR) format containing */
/*           the L and U factors together. The diagonal (stored in */
/*           alu(1:n) ) is inverted. Each i-th row of the alu,jlu matrix */
/*           contains the i-th row of L (excluding the diagonal entry=1) */
/*           followed by the i-th row of U. */

/* ju	  = pointer to the diagonal elements in alu, jlu. */

/* ierr	  = integer indicating error code on return */
/* 	     ierr = 0 --> normal return */
/* 	     ierr = k --> code encountered a zero pivot at step k. */
/* work arrays: */
/* ------------- */
/* iw	    = integer work array of length n. */
/* ------------ */
/* IMPORTANT */
/* ----------- */
/* it is assumed that the the elements in the input matrix are stored */
/*    in such a way that in each row the lower part comes first and */
/*    then the upper part. To get the correct ILU factorization, it is */
/*    also necessary to have the elements of L sorted by increasing */
/*    column number. It may therefore be necessary to sort the */
/*    elements of a, ja, ia prior to calling ilu0. This can be */
/*    achieved by transposing the matrix twice using csrcsc. */

/* ----------------------------------------------------------------------- */
    /* Parameter adjustments */
    --iw;
    --ju;
    --jlu;
    --alu;
    --ia;
    --ja;
    --a;

    /* Function Body */
    ju0 = n + 2;
    jlu[1] = ju0;

/* initialize work vector to zero's */


    for (i__ = 1; i__ <= n; ++i__) {
	    iw[i__] = 0;
/* L31: */
    }

/* main loop */

    for (ii = 1; ii <= n; ++ii) 
	{
	   js = ju0;

       /* generating row number ii of L and U. */

       for (j = ia[ii]; j <= ia[ii + 1] - 1; ++j) 
	   {

/*     copy row ii of a, ja, ia into row ii of alu, jlu (L/U) matrix. */

	       jcol = ja[j];
	       if (jcol == ii) {
		      alu[ii] = a[j];
		      iw[jcol] = ii;
		      ju[ii] = ju0;
	       } else {
		      alu[ju0] = a[j];
		      jlu[ju0] = ja[j];
		      iw[jcol] = ju0;
		      ++ju0;
	       }
/* L100: */
	   }
	   jlu[ii + 1] = ju0;
	   jf = ju0 - 1;
	   jm = ju[ii] - 1;

/*     exit if diagonal element is reached. */


	   for (j = js; j <= jm; ++j) {
	       jrow = jlu[j];
	       tl = alu[j] * alu[jrow];
	       alu[j] = tl;

/*     perform  linear combination */

	       for (jj = ju[jrow]; jj <= jlu[jrow + 1] - 1; ++jj) {
		       jw = iw[jlu[jj]];
		       if (jw != 0) {
		           alu[jw] -= tl * alu[jj];
		       }
/* L140: */
	       }
/* L150: */
	    }

/*     invert  and store diagonal element. */

	if (fabs(alu[ii]) < 1.0e-30) {
	    /*     zero pivot : */
        // ������� ������������ ������� � ������ ii-1
        ierr = ii;
#if doubleintprecision == 1
		printf("Error !!! Zero diagonal element in string %lld \n", ii);
#else
		printf("Error !!! Zero diagonal element in string %d \n", ii);
#endif
        
        printf("Please, press any key to exit calculation...\n");
       // getchar();
		system("pause");
        /* Parameter adjustments */
        ++iw;
        ++ju;
        ++jlu;
        ++alu;
        ++ia;
        ++ja;
        ++a;
        exit(0);
        return 0;
	}
	else {
	  
		alu[ii] = 1.0 / alu[ii];

         /*     reset pointer iw to zero */

	     iw[ii] = 0;
	    
	     for (i__ = js; i__ <= jf; ++i__) {
             /* L201: */
	         iw[jlu[i__]] = 0;
	     }
	}
/* L500: */
    } // ����������� ������ main �����.
    
	ierr = 0;

    /* Parameter adjustments */
    ++iw;
    ++ju;
    ++jlu;
    ++alu;
    ++ia;
    ++ja;
    ++a;

    return 0;  

    
/* ------- end-of-ilu0 --------------------------------------------------- */
/* ----------------------------------------------------------------------- */
} /* ilu0_ */

// ������ ILU �������. (LU)x=y; 
// ������� ILU ������������ ������� � MSR ������� :
// alu, jlu, ju; n - ��� ����������� ������� � ������ ���������� ������� ����.
/* ----------------------------------------------------------------------- */
/* Subroutine */ integer lusol_1(integer n, doublereal* &y, doublereal* &x, 
	doublereal* &alu, integer* &jlu, integer* &ju, integer maxelm)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    integer i__, k;

/* ----------------------------------------------------------------------- */

/* This routine solves the system (LU) x = y, */
/* given an LU decomposition of a matrix stored in (alu, jlu, ju) */
/* modified sparse row format */

/* ----------------------------------------------------------------------- */
/* on entry: */
/* n   = dimension of system */
/* y   = the right-hand-side vector */
/* alu, jlu, ju */
/*     = the LU matrix as provided from the ILU routines. */

/* on return */
/* x   = solution of LU x = y. */
/* ----------------------------------------------------------------------- */

/* Note: routine is in place: call lusol (n, x, x, alu, jlu, ju) */
/*       will solve the system with rhs x and overwrite the result on x . */

/* ----------------------------------------------------------------------- */
/* local variables */


/* forward solve */

    /* Parameter adjustments */
    --x;
    --y;
    --alu;
    --jlu;
    --ju;

	// ���������� �.�.�����, �.�.��������.
	// ������������� ������������ ���������� ������������� ���������
	// ������� ������ �������� ��������� � ������������ �������������� 
	// �������� �� ����� ����������.
	/* 9 ������� 2015.
	LUx=y;
	L^(-1)*LUx=L^(-1)*y;
	z=L^(-1)*y;
	Ux=z;
	x=U^(-1)*z;
	*/

	/*
#ifdef _OPENMP

	// ��� ������� ����������� ������� � L ����� ����� ����������� �� ������� ������ � ������.
	// ������� � ������������ ������� ����� ��������� �����������, ��������������� ������� ��������
	// ������ ��������� ������ ������������. ����� ����� ��������� ����������� ������ ���� ������ 
	// ������������ ��� ������ ��������� ���������� � ����� ������, �� ������� ������� ���� ����.

			if (inumcore==2) {
				if (nd.b0.active) {

					// ������ �����
					for (integer iscan_par=nd.b0.ileft_start; iscan_par<=nd.b0.ileft_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];

						// ������� ��������� ������������ ���� ��������� ��.
						// � ��� �� ������ ������� ���������� ��.
						if (iPloc>=maxelm) {
						   // i__1 = n;
                           //for (i__ = 1; i__ <= i__1; ++i__) {
						   i__=iPloc+1;
	                       x[i__] = y[i__];
	                       i__2 = ju[i__] - 1;
	                       for (k = jlu[i__]; k <= i__2; ++k) {
	                           x[i__] -= alu[k] * x[jlu[k]];
                               
	                       }
                           //}
						}

					}
					for (integer iscan_par=nd.b0.ileft_start; iscan_par<=nd.b0.ileft_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];

						// ������� ��������� ������������ ���� ��������� ��.
						// � ��� �� ������ ������� ���������� ��.
						if (iPloc<maxelm) {
						   // i__1 = n;
                           //for (i__ = 1; i__ <= i__1; ++i__) {
						   i__=iPloc+1;
	                       x[i__] = y[i__];
	                       i__2 = ju[i__] - 1;
	                       for (k = jlu[i__]; k <= i__2; ++k) {
	                           x[i__] -= alu[k] * x[jlu[k]];
                               
	                       }
                          
                           //}
						}

					}
					// ������ �����
					for (integer iscan_par=nd.b0.iright_start; iscan_par<=nd.b0.iright_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];
						// ������� ��������� ������������ ���� ��������� ��.
						// � ��� �� ������ ������� ���������� ��.
						if (iPloc>=maxelm) {
						// i__1 = n;
                           //for (i__ = 1; i__ <= i__1; ++i__) {
						   i__=iPloc+1;
	                       x[i__] = y[i__];
	                       i__2 = ju[i__] - 1;
	                       for (k = jlu[i__]; k <= i__2; ++k) {
	                           x[i__] -= alu[k] * x[jlu[k]];
                              
	                       }
                           
                           //}
						}
					}
					for (integer iscan_par=nd.b0.iright_start; iscan_par<=nd.b0.iright_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];
						// ������� ��������� ������������ ���� ��������� ��.
						// � ��� �� ������ ������� ���������� ��.
						if (iPloc<maxelm) {
						// i__1 = n;
                           //for (i__ = 1; i__ <= i__1; ++i__) {
						   i__=iPloc+1;
	                       x[i__] = y[i__];
	                       i__2 = ju[i__] - 1;
	                       for (k = jlu[i__]; k <= i__2; ++k) {
	                           x[i__] -= alu[k] * x[jlu[k]];
                             
	                       }
                          
                           //}
						}
					}
					// �������� ��������� �����
					for (integer iscan_par=nd.b0.iseparate_start; iscan_par<=nd.b0.iseparate_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];
						// ������� ��������� ������������ ���� ��������� ��.
						// � ��� �� ������ ������� ���������� ��.
						if (iPloc>=maxelm) {
						// i__1 = n;
                           //for (i__ = 1; i__ <= i__1; ++i__) {
						   i__=iPloc+1;
	                       x[i__] = y[i__];
	                       i__2 = ju[i__] - 1;
	                       for (k = jlu[i__]; k <= i__2; ++k) {
	                           x[i__] -= alu[k] * x[jlu[k]];
                              
	                       }
                          
                           //}
						}
					}
					for (integer iscan_par=nd.b0.iseparate_start; iscan_par<=nd.b0.iseparate_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];
						// ������� ��������� ������������ ���� ��������� ��.
						// � ��� �� ������ ������� ���������� ��.
						if (iPloc<maxelm) {
						// i__1 = n;
                           //for (i__ = 1; i__ <= i__1; ++i__) {
						   i__=iPloc+1;
	                       x[i__] = y[i__];
	                       i__2 = ju[i__] - 1;
	                       for (k = jlu[i__]; k <= i__2; ++k) {
	                           x[i__] -= alu[k] * x[jlu[k]];
                              
	                       }
                           
                           //}
						}
					}


				}
			}

#else*/

    /* Function Body */
	// L : z=L^(-1)*y;
    i__1 = n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	    x[i__] = y[i__];
	    i__2 = ju[i__] - 1;
		//doublereal dsum = 0.0;
//#pragma loop(hint_parallel(8))
//#pragma omp parallel for private(k) reduction(+:dsum)
	    for (k = jlu[i__]; k <= i__2; ++k) {
	        x[i__] -= alu[k] * x[jlu[k]];
			//dsum -= alu[k] * x[jlu[k]];
            /* L41: */
	    }
		//x[i__] += dsum;
        /* L40: */
    }

//#endif

	/*
#ifdef _OPENMP

			if (inumcore==2) {
				if (nd.b0.active) {

					// �������� ��������� �����
					for (integer iscan_par=nd.b0.iseparate_start; iscan_par<=nd.b0.iseparate_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];
						// ������� ��������� ������������ ���� ��������� ��.
						// � ��� �� ������ ������� ���������� ��.
						if (iPloc>=maxelm) {
							i__=iPloc+1;

						    i__1 = jlu[i__ + 1] - 1;
	                        for (k = ju[i__]; k <= i__1; ++k) {
	                            x[i__] -= alu[k] * x[jlu[k]];
                                
	                        }
	                        x[i__] = alu[i__] * x[i__];
						}
					}
					for (integer iscan_par=nd.b0.iseparate_start; iscan_par<=nd.b0.iseparate_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];
						// ������� ��������� ������������ ���� ��������� ��.
						// � ��� �� ������ ������� ���������� ��.
						if (iPloc<maxelm) {
							i__=iPloc+1;

						    i__1 = jlu[i__ + 1] - 1;
	                        for (k = ju[i__]; k <= i__1; ++k) {
	                        x[i__] -= alu[k] * x[jlu[k]];
                           
	                    }
	                    x[i__] = alu[i__] * x[i__];
						}
					}
					// ������ �����
					for (integer iscan_par=nd.b0.ileft_start; iscan_par<=nd.b0.ileft_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];
						// ������� ��������� ������������ ���� ��������� ��.
						// � ��� �� ������ ������� ���������� ��.
						if (iPloc>=maxelm) {
							i__=iPloc+1;

						    i__1 = jlu[i__ + 1] - 1;
	                        for (k = ju[i__]; k <= i__1; ++k) {
	                            x[i__] -= alu[k] * x[jlu[k]];
                               
	                        }
	                        x[i__] = alu[i__] * x[i__];
						}
					}
					for (integer iscan_par=nd.b0.ileft_start; iscan_par<=nd.b0.ileft_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];
						// ������� ��������� ������������ ���� ��������� ��.
						// � ��� �� ������ ������� ���������� ��.
						if (iPloc<maxelm) {
							i__=iPloc+1;

						    i__1 = jlu[i__ + 1] - 1;
	                        for (k = ju[i__]; k <= i__1; ++k) {
	                        x[i__] -= alu[k] * x[jlu[k]];
                           
	                    }
	                    x[i__] = alu[i__] * x[i__];
						}
					}
					// ������ �����
					for (integer iscan_par=nd.b0.iright_start; iscan_par<=nd.b0.iright_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];
						// ������� ��������� ������������ ���� ��������� ��.
						// � ��� �� ������ ������� ���������� ��.
						if (iPloc>=maxelm) {
							i__=iPloc+1;

						    i__1 = jlu[i__ + 1] - 1;
	                        for (k = ju[i__]; k <= i__1; ++k) {
	                            x[i__] -= alu[k] * x[jlu[k]];
                               
	                        }
	                        x[i__] = alu[i__] * x[i__];
						}
					}
					for (integer iscan_par=nd.b0.iright_start; iscan_par<=nd.b0.iright_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];
						// ������� ��������� ������������ ���� ��������� ��.
						// � ��� �� ������ ������� ���������� ��.
						if (iPloc<maxelm) {
							i__=iPloc+1;

						    i__1 = jlu[i__ + 1] - 1;
	                        for (k = ju[i__]; k <= i__1; ++k) {
	                        x[i__] -= alu[k] * x[jlu[k]];
                           
	                    }
	                    x[i__] = alu[i__] * x[i__];
						}
					}


				}
			}

#else*/

/*     backward solve. */
	// U : x=U^(-1)*z;
    for (i__ = n; i__ >= 1; --i__) {
	     i__1 = jlu[i__ + 1] - 1;
//#pragma loop(hint_parallel(8))
	     for (k = ju[i__]; k <= i__1; ++k) {
	          x[i__] -= alu[k] * x[jlu[k]];
              /* L91: */
	     }
	     x[i__] = alu[i__] * x[i__];
         /* L90: */
    }

//#endif

    /* Parameter adjustments */
    ++x;
    ++y;
    ++alu;
    ++jlu;
    ++ju;


    return 0;
/* ----------------end of lusol ------------------------------------------ */
/* ----------------------------------------------------------------------- */
} /* lusol_ */

// 9 ������ 2016 ����.
// �������� ����������� ������ ��� ILU.
// ���������������� ���������.
typedef struct TLEVEL_ADDITIONAL_DATA_BUFER {
	// lusol_ ������ ������� � ������ ������ �����
	// ������� ����� �������� ������ �� ������ ��������
	doublereal* alu_copy;
	integer* jlu_copy;
	integer* ju_copy;
} LEVEL_ADDITIONAL_DATA_BUFER;

// ���������������� ��������� ��������������� �����,
// ��� ���� ������������ �� ���� ������� ��� ��������� ���������.
// ������ �������� ������������� � ������ ������ �������������, 
// ��� ���� ����������� �� ������ �� ������� �������.
LEVEL_ADDITIONAL_DATA_BUFER milu_gl_buffer;

// 10 ������� 2016 ����.
// �������������� ���������� �� ������ �� �������,
// ��� ����� �������������� � ��� ������������� 
// ������������� �� ������ ������� ��������������� �������.
typedef struct TLEVEL_ADDITIONAL_DATA {
	// ��� �������� ������� �� ������ ������.
	doublereal* val;
	integer* col_ind;
	integer* row_ptr;
	// ��� �������� ilu2 ������������.
	static const integer lfil = 2;
	integer maxelm_plus_maxbound;
	integer iwk;
	doublereal* alu;
	integer* jlu;
	integer* ju;
	integer* levs;
	doublereal* w;
	integer* jw;
	// lusol_ ������ ������� � ������ ������ �����
	// ������� ����� �������� ������ �� ������ ��������
	//doublereal* alu_copy;
	//integer* jlu_copy;
	//integer* ju_copy;
	doublereal* b_copy;
	doublereal* x_copy;
	doublereal* zbuf;
	doublereal* zbuf2;
} LEVEL_ADDITIONAL_DATA;

// 4 ������ 2016 ����.
// �������������� ���������� �� ������ �� �������,
// ��� ����� �������������� � ��� ������������� 
// ������������� �� ������ ������� ��������������� �������.
typedef struct TLEVEL_ADDITIONAL_DATA0 {
	// ��� �������� ������� �� ������ ������.
	doublereal* val;
	integer* col_ind;
	integer* row_ptr;
	// ��� �������� ilu0 ������������.

	// ������� ����������.
	doublereal* alu;
	integer* jlu;
	integer* ju;
	// ��������������� ������.
	integer* iw;
	
	// for lusol_1patchforRUMBA
	integer iwk;
	
	// lusol_ ������ ������� � ������ ������ �����
	// ������� ����� �������� ������ �� ������ ��������
	doublereal* alu_copy;
	integer* jlu_copy;
	integer* ju_copy;
	doublereal* b_copy;
	doublereal* x_copy;
	doublereal* zbuf;
	doublereal* zbuf2;
} LEVEL_ADDITIONAL_DATA0;

// 10.08.2016
// ���� ��� ������������� � ����� ����������.
// 9 ������ 2016 ���������� ��� ����������������� �������� ������ � �������. 
integer lusol_1patchforRUMBA(integer n, doublereal* &y, doublereal* &x,
	LEVEL_ADDITIONAL_DATA &milu2)
{
	// x - argument
	// y - rthdsd

	integer iret = 0;

	
	// move
	for (integer i_1 = 0; i_1 < n; i_1++) {
		milu2.x_copy[i_1] = x[i_1 + 1];
		milu2.b_copy[i_1] = y[i_1 + 1];
	}

	for (integer i7 = 0; i7<milu2.iwk + 2; i7++) {
		//milu2.alu_copy[i7] = milu2.alu[i7];
		//milu2.jlu_copy[i7] = milu2.jlu[i7];
		// ���������� ��� �������������.
		// ��� ���� ��������� ������ ��� ������������ ��� ������ ������ �������.
		milu_gl_buffer.alu_copy[i7] = milu2.alu[i7];
		milu_gl_buffer.jlu_copy[i7] = milu2.jlu[i7];
	}
	//for (integer i7 = 0; i7<n + 2; i7++) milu2.ju_copy[i7] = milu2.ju[i7];
	for (integer i7 = 0; i7<n + 2; i7++) milu_gl_buffer.ju_copy[i7] = milu2.ju[i7];

	//iret = lusol_1(n, milu2.b_copy, milu2.x_copy,
		//milu2.alu_copy, milu2.jlu_copy, milu2.ju_copy, 0);

	iret = lusol_1(n, milu2.b_copy, milu2.x_copy,
		milu_gl_buffer.alu_copy, milu_gl_buffer.jlu_copy, milu_gl_buffer.ju_copy, 0);

	// move
	for (integer i_1 = 0; i_1 < n; i_1++) {
		x[i_1 + 1] = milu2.x_copy[i_1];
		milu2.b_copy[i_1] = y[i_1 + 1];
	}

	// ������������� ��������������.
	for (integer i7 = 0; i7<milu2.iwk + 2; i7++) {
		//milu2.alu_copy[i7] = milu2.alu[i7];
		//milu2.jlu_copy[i7] = milu2.jlu[i7];
		milu_gl_buffer.alu_copy[i7] = milu2.alu[i7];
		milu_gl_buffer.jlu_copy[i7] = milu2.jlu[i7];
	}
	//for (integer i7 = 0; i7<n + 2; i7++) milu2.ju_copy[i7] = milu2.ju[i7];
	for (integer i7 = 0; i7<n + 2; i7++) milu_gl_buffer.ju_copy[i7] = milu2.ju[i7];

	return iret;

} // lusol_1patchforRUMBA

  // 10.08.2016 04.november.2016
  // ���� ��� ������������� � ����� ����������.
integer lusol_1patchforRUMBA(integer n, doublereal* &y, doublereal* &x,
	LEVEL_ADDITIONAL_DATA0 &milu0)
{
	// x - argument
	// y - rthdsd

	integer iret = 0;


	// move
	for (integer i_1 = 0; i_1 < n; i_1++) {
		milu0.x_copy[i_1] = x[i_1 + 1];
		milu0.b_copy[i_1] = y[i_1 + 1];
	}

	for (integer i7 = 0; i7<milu0.iwk + 2; i7++) {
		milu0.alu_copy[i7] = milu0.alu[i7];
		milu0.jlu_copy[i7] = milu0.jlu[i7];
	}
	for (integer i7 = 0; i7<n + 2; i7++) milu0.ju_copy[i7] = milu0.ju[i7];


	iret = lusol_1(n, milu0.b_copy, milu0.x_copy,
		milu0.alu_copy, milu0.jlu_copy, milu0.ju_copy, 0);

	// move
	for (integer i_1 = 0; i_1 < n; i_1++) {
		x[i_1 + 1] = milu0.x_copy[i_1];
		milu0.b_copy[i_1] = y[i_1 + 1];
	}

	// ������������� ��������������.
	for (integer i7 = 0; i7<milu0.iwk + 2; i7++) {
		milu0.alu_copy[i7] = milu0.alu[i7];
		milu0.jlu_copy[i7] = milu0.jlu[i7];
	}
	for (integer i7 = 0; i7<n + 2; i7++) milu0.ju_copy[i7] = milu0.ju[i7];

	return iret;

} // lusol_1patchforRUMBA

// ������ ILU �������. (LU)x=y; 
// ������� ILU ������������ ������� � MSR ������� :
// alu, jlu, ju; n - ��� ����������� ������� � ������ ���������� ������� ����.
/* ----------------------------------------------------------------------- */
/* Subroutine */ integer lusol_2(integer n, doublereal* &y, doublereal* &x, 
	doublereal* &alu, integer* &jlu, integer* &ju,  doublereal* &x_copy, integer maxelm)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    integer i__, k;

/* ----------------------------------------------------------------------- */

/* This routine solves the system (LU) x = y, */
/* given an LU decomposition of a matrix stored in (alu, jlu, ju) */
/* modified sparse row format */

/* ----------------------------------------------------------------------- */
/* on entry: */
/* n   = dimension of system */
/* y   = the right-hand-side vector */
/* alu, jlu, ju */
/*     = the LU matrix as provided from the ILU routines. */

/* on return */
/* x   = solution of LU x = y. */
/* ----------------------------------------------------------------------- */

/* Note: routine is in place: call lusol (n, x, x, alu, jlu, ju) */
/*       will solve the system with rhs x and overwrite the result on x . */

/* ----------------------------------------------------------------------- */
/* local variables */


/* forward solve */

    /* Parameter adjustments */
    --x;
    --y;
    --alu;
    --jlu;
    --ju;

	// ���������� �.�.�����, �.�.��������.
	// ������������� ������������ ���������� ������������� ���������
	// ������� ������ �������� ��������� � ������������ �������������� 
	// �������� �� ����� ����������.
	/* 9 ������� 2015.
	LUx=y;
	L^(-1)*LUx=L^(-1)*y;
	z=L^(-1)*y;
	Ux=z;
	x=U^(-1)*z;
	*/

	
#ifdef _OPENMP

	// ��� ������� ����������� ������� � L ����� ����� ����������� �� ������� ������ � ������.
	// ������� � ������������ ������� ����� ��������� �����������, ��������������� ������� ��������
	// ������ ��������� ������ ������������. ����� ����� ��������� ����������� ������ ���� ������ 
	// ������������ ��� ������ ��������� ���������� � ����� ������, �� ������� ������� ���� ����.


			if (inumcore==2) {
				if (nd.b0.active) {

#pragma omp parallel private(i__, i__2,k)
	{
#pragma omp sections
		{
#pragma omp section
			{

					// ������ �����
					for (integer iscan_par=nd.b0.ileft_start; iscan_par<=nd.b0.ileft_finish; iscan_par++) {
						
						integer iPloc=iscan_par;

						
						   i__=iPloc+1;
	                       x[i__] = y[i__];
	                       i__2 = ju[i__] - 1;
	                       for (k = jlu[i__]; k <= i__2; ++k) {
	                           x[i__] -= alu[k] * x[jlu[k]];
                               
	                       }
                      

					}
					
			}
#pragma omp section
			{

					// ������ �����
					for (integer iscan_par=nd.b0.iright_start; iscan_par<=nd.b0.iright_finish; iscan_par++) {
						
						integer iPloc=iscan_par;

						
						   i__=iPloc+1;
	                       x[i__] = y[i__];
	                       i__2 = ju[i__] - 1;
	                       for (k = jlu[i__]; k <= i__2; ++k) {
	                           x[i__] -= alu[k] * x[jlu[k]];
                              
	                       }
                           
                         
					}
			} // pragma omp section
		} // pragma omp sections
	} // pragma omp parallel
					
					// �������� ��������� �����
					for (integer iscan_par=nd.b0.iseparate_start; iscan_par<=nd.b0.iseparate_finish; iscan_par++) {
						
						integer iPloc=iscan_par;
						
						
						   i__=iPloc+1;
	                       x[i__] = y[i__];
	                       i__2 = ju[i__] - 1;
	                       for (k = jlu[i__]; k <= i__2; ++k) {
	                           x[i__] -= alu[k] * x[jlu[k]];
                              
	                       }
                          
                         
					}
					


				}
			}

#else

    /* Function Body */
	// L : z=L^(-1)*y;
    i__1 = n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	    x[i__] = y[i__];
	    i__2 = ju[i__] - 1;
	    for (k = jlu[i__]; k <= i__2; ++k) {
	        x[i__] -= alu[k] * x[jlu[k]];
            /* L41: */
	    }
        /* L40: */
    }

#endif

	
#ifdef _OPENMP

			if (inumcore==2) {
				if (nd.b0.active) {

					// �������� ��������� �����
					for (integer iscan_par=nd.b0.iseparate_finish; iscan_par>=nd.b0.iseparate_start; iscan_par--) {
						
						integer iPloc=iscan_par;
						
							i__=iPloc+1;

						    i__1 = jlu[i__ + 1] - 1;
	                        for (k = ju[i__]; k <= i__1; ++k) {
	                            x[i__] -= alu[k] * x[jlu[k]];
                                
	                        }
	                        x[i__] = alu[i__] * x[i__];
						
					}
					//doublereal *x_copy=new doublereal[n];
					for (integer icop=1; icop<=n; icop++) x_copy[icop-1]=x[icop];
					
#pragma omp parallel private(i__, i__1,k)
					{
#pragma omp sections
						{
#pragma omp section
							{

					// ������ �����
					for (integer iscan_par=nd.b0.ileft_finish; iscan_par>=nd.b0.ileft_start; iscan_par--) {
						
						integer iPloc=iscan_par;
						
						
							i__=iPloc+1;

						    i__1 = jlu[i__ + 1] - 1;
	                        for (k = ju[i__]; k <= i__1; ++k) {
	                            x_copy[i__-1] -= alu[k] * x_copy[jlu[k]-1];
                               
	                        }
	                        x_copy[i__-1] = alu[i__] * x_copy[i__-1];
						
					}
							}
#pragma omp section
							{
					
					// ������ �����
					for (integer iscan_par=nd.b0.iright_finish; iscan_par>=nd.b0.iright_start; iscan_par--) {
						
						integer iPloc=iscan_par;
						
						
							i__=iPloc+1;

						    i__1 = jlu[i__ + 1] - 1;
	                        for (k = ju[i__]; k <= i__1; ++k) {
	                            x[i__] -= alu[k] * x[jlu[k]];
                               
	                        }
	                        x[i__] = alu[i__] * x[i__];
						
					}
							} // omp section
						} // omp sections
					} // omp parallel
				
					for (integer iscan_par=nd.b0.ileft_finish; iscan_par>=nd.b0.ileft_start; iscan_par--) {
						x[iscan_par+1]=x_copy[iscan_par];
					}
                   // delete x_copy;

				}
			}

#else

/*     backward solve. */
	// U : x=U^(-1)*z;
    for (i__ = n; i__ >= 1; --i__) {
	     i__1 = jlu[i__ + 1] - 1;
	     for (k = ju[i__]; k <= i__1; ++k) {
	          x[i__] -= alu[k] * x[jlu[k]];
              /* L91: */
	     }
	     x[i__] = alu[i__] * x[i__];
         /* L90: */
    }

#endif

    /* Parameter adjustments */
    ++x;
    ++y;
    ++alu;
    ++jlu;
    ++ju;


    return 0;
/* ----------------end of lusol ------------------------------------------ */
/* ----------------------------------------------------------------------- */
} /* lusol_2 */


// ������ ILU �������. (LU)x=y; 
// ������� ILU ������������ ������� � MSR ������� :
// alu, jlu, ju; n - ��� ����������� ������� � ������ ���������� ������� ����.
/* ----------------------------------------------------------------------- */
/* Subroutine */ integer lusol_3(integer n, doublereal* &y, doublereal* &x, 
	doublereal* &alu, integer* &jlu, integer* &ju, integer maxelm)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    integer i__, k;

/* ----------------------------------------------------------------------- */

/* This routine solves the system (LU) x = y, */
/* given an LU decomposition of a matrix stored in (alu, jlu, ju) */
/* modified sparse row format */

/* ----------------------------------------------------------------------- */
/* on entry: */
/* n   = dimension of system */
/* y   = the right-hand-side vector */
/* alu, jlu, ju */
/*     = the LU matrix as provided from the ILU routines. */

/* on return */
/* x   = solution of LU x = y. */
/* ----------------------------------------------------------------------- */

/* Note: routine is in place: call lusol (n, x, x, alu, jlu, ju) */
/*       will solve the system with rhs x and overwrite the result on x . */

/* ----------------------------------------------------------------------- */
/* local variables */


/* forward solve */

    /* Parameter adjustments */
    --x;
    --y;
    --alu;
    --jlu;
    --ju;

	// ���������� �.�.�����, �.�.��������.
	// ������������� ������������ ���������� ������������� ���������
	// ������� ������ �������� ��������� � ������������ �������������� 
	// �������� �� ����� ����������.
	/* 9 ������� 2015.
	LUx=y;
	L^(-1)*LUx=L^(-1)*y;
	z=L^(-1)*y;
	Ux=z;
	x=U^(-1)*z;
	*/

	
#ifdef _OPENMP

	// ��� ������� ����������� ������� � L ����� ����� ����������� �� ������� ������ � ������.
	// ������� � ������������ ������� ����� ��������� �����������, ��������������� ������� ��������
	// ������ ��������� ������ ������������. ����� ����� ��������� ����������� ������ ���� ������ 
	// ������������ ��� ������ ��������� ���������� � ����� ������, �� ������� ������� ���� ����.


			if (inumcore==2) {
				if (nd.b0.active) {

#pragma omp parallel private(i__, i__2,k)
	{
#pragma omp sections
		{
#pragma omp section
			{

					// ������ �����
					for (integer iscan_par=nd.b0.ileft_start; iscan_par<=nd.b0.ileft_finish; iscan_par++) {
						
						integer iPloc=iscan_par;

						
						   i__=iPloc+1;
	                       x[i__] = y[i__];
	                       i__2 = ju[i__] - 1;
	                       for (k = jlu[i__]; k <= i__2; ++k) {
	                           x[i__] -= alu[k] * x[jlu[k]];
                               
	                       }
                      

					}
					
			}
#pragma omp section
			{

					// ������ �����
					for (integer iscan_par=nd.b0.iright_start; iscan_par<=nd.b0.iright_finish; iscan_par++) {
						
						integer iPloc=iscan_par;

						
						   i__=iPloc+1;
	                       x[i__] = y[i__];
	                       i__2 = ju[i__] - 1;
	                       for (k = jlu[i__]; k <= i__2; ++k) {
	                           x[i__] -= alu[k] * x[jlu[k]];
                              
	                       }
                           
                         
					}
			} // pragma omp section
		} // pragma omp sections
	} // pragma omp parallel
					
					// �������� ��������� �����
					for (integer iscan_par=nd.b0.iseparate_start; iscan_par<=nd.b0.iseparate_finish; iscan_par++) {
						
						integer iPloc=iscan_par;
						
						
						   i__=iPloc+1;
	                       x[i__] = y[i__];
	                       i__2 = ju[i__] - 1;
	                       for (k = jlu[i__]; k <= i__2; ++k) {
	                           x[i__] -= alu[k] * x[jlu[k]];
                              
	                       }
                          
                         
					}
					


				}
			}

#else

    /* Function Body */
	// L : z=L^(-1)*y;
    i__1 = n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	    x[i__] = y[i__];
	    i__2 = ju[i__] - 1;
	    for (k = jlu[i__]; k <= i__2; ++k) {
	        x[i__] -= alu[k] * x[jlu[k]];
            /* L41: */
	    }
        /* L40: */
    }

#endif

	
#ifdef _OPENMP

			if (inumcore==2) {
				if (nd.b0.active) {

					// �������� ��������� �����
					for (integer iscan_par=nd.b0.iseparate_finish; iscan_par>=nd.b0.iseparate_start; iscan_par--) {
						
						integer iPloc=iscan_par;
						
							i__=iPloc+1;

						    i__1 = jlu[i__ + 1] - 1;
	                        for (k = ju[i__]; k <= i__1; ++k) {
	                            x[i__] -= alu[k] * x[jlu[k]];
                                
	                        }
	                        x[i__] = alu[i__] * x[i__];
						
					}
					
					
#pragma omp parallel private(i__, i__1,k)
					{
#pragma omp sections
						{
#pragma omp section
							{

					// ������ �����
					for (integer iscan_par=nd.b0.ileft_finish; iscan_par>=nd.b0.ileft_start; iscan_par--) {
						
						integer iPloc=iscan_par;
						
						
							i__=iPloc+1;

						    i__1 = jlu[i__ + 1] - 1;
	                        for (k = ju[i__]; k <= i__1; ++k) {
	                            x[i__] -= alu[k] * x[jlu[k]];
                               
	                        }
	                        x[i__] = alu[i__] * x[i__];
						
					}
							}
#pragma omp section
							{
					
					// ������ �����
					for (integer iscan_par=nd.b0.iright_finish; iscan_par>=nd.b0.iright_start; iscan_par--) {
						
						integer iPloc=iscan_par;
						
						
							i__=iPloc+1;

						    i__1 = jlu[i__ + 1] - 1;
	                        for (k = ju[i__]; k <= i__1; ++k) {
	                            x[i__] -= alu[k] * x[jlu[k]];
                               
	                        }
	                        x[i__] = alu[i__] * x[i__];
						
					}
							} // omp section
						} // omp sections
					} // omp parallel
				
					

				}
			}

#else

/*     backward solve. */
	// U : x=U^(-1)*z;
    for (i__ = n; i__ >= 1; --i__) {
	     i__1 = jlu[i__ + 1] - 1;
	     for (k = ju[i__]; k <= i__1; ++k) {
	          x[i__] -= alu[k] * x[jlu[k]];
              /* L91: */
	     }
	     x[i__] = alu[i__] * x[i__];
         /* L90: */
    }

#endif

    /* Parameter adjustments */
    ++x;
    ++y;
    ++alu;
    ++jlu;
    ++ju;


    return 0;
/* ----------------end of lusol ------------------------------------------ */
/* ----------------------------------------------------------------------- */
} /* lusol_3 */

integer lusol_(integer n, doublereal* &y, doublereal* &x, 
	doublereal* &alu, integer* &jlu, integer* &ju, integer maxelm)
{
	// Serial
	if (1) {
		// �������������� ������ ��  Sparskit2. 
	    lusol_1(n, y, x, alu, jlu, ju, maxelm);
	}
	else {
		// ���������������� ������ 9 ������� 2015 ����.
		//lusol_2(n, y, x, alu, jlu, ju, maxelm);

		printf("lusol_ -> lusol_2 ustarevshii vjzov. 10 08 2015\n");
	//	getchar();
		system("pause");
		exit(1);
	}

	return 0;
}