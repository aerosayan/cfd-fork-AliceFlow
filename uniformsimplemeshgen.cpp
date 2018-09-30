// ���� uniformsimplemeshgen.cpp
// �������� ��������� ����������� 3D ��������� �����.
// 19 ������� 2011 ��������� ����������� ������������ ������������� �����
// � ������������ � ������� �������������� ���������� (�������� � ���� ��������).

#ifndef _UNIFORMSIMPLEMESHGEN_CPP_
#define _UNIFORMSIMPLEMESHGEN_CPP_ 1



//#include "my_material_properties.c"



void my_solid_properties(doublereal TiP, doublereal &rho, doublereal &cp, doublereal &lam, integer ilibident);

// ����������� ���������� ������ �� 
// ������� �� ����������� �� �����������
// �������� ���������
integer min_elem_in_x_element=4; // ����������� �� ������ 4!
integer min_elem_in_y_element=4; // ����������� �� ������ 4!
integer min_elem_in_z_element=4; // ����������� �� ������ 4!
// ������ ��������� �����
integer adapt_x=0;
integer adapt_y=0;
integer adapt_z=0;
// ��������� �������� ��������.
const doublereal dcount_diametr_cylinder = 8.0;
const bool bcylinder_meshing = true;

/*const*/ /*doublereal etalon_max_size_ratio=2.0;*/ // �� inputlaplas.cpp



// ���������� ������� SetLength �� Delphi 
// ��� ��������� �������� ������������� ������� ra. 
// ���������� ���������������� ������ ra ������������ 
//  �������. 
// �� ����� ������ ������� ������ ������ �� ����������.
void SetLength(doublereal* &ra, integer isizeold, integer isize) 
{

	// isize - ����� ����� ������������� �������.
	doublereal *temp=NULL;
	temp = new doublereal[isize];
    integer i;
	for (i=0; i<isize; i++) temp[i]=0.0; // �������������
    /*
	integer isizeold;
	if (ra != NULL) {
       isizeold = sizeof(ra)/sizeof(ra[0]); // ����� ������� �������
	   #if doubleintprecision == 1
			printf("%lld  ",isizeold); // �� ����� ��������� ���������� ������ �������
	   #else
			printf("%d  ",isizeold); // �� ����� ��������� ���������� ������ �������
	   #endif

       
	} 
	  else
	{
       isizeold=0;
	}
	*/
	if (isize < isizeold) isizeold=isize;
	for (i=0; i<isizeold; i++) temp[i]=ra[i];
	
	if (ra != NULL) {
		delete[]  ra; // ����������� �������
		ra = NULL;
	}
	ra = new doublereal[isize]; // ��������� ������
	for (i=0; i<isize; i++) ra[i]=temp[i]; // �����������
	delete[] temp; // ������������ ������
	temp = NULL;
	
} // SetLength

// ��������� �������������� ������� � �������
void addboundary(doublereal* &rb, integer &in, doublereal g) {
	// rb - �������������� ������ ������,
	// in - ����� ��������� ������� � �������, ��������� ���������� � ����.
	// g - �������, �������� �� ����������.
	integer i=0;
    //const doublereal eps=1e-30; // ��� ��������� ������������� ����.
	bool bfind=false;
	for (i=0; i<=in; i++) if (fabs(rb[i]-g)<admission) bfind=true;
	if (!bfind) {
        SetLength(rb, in+1, in+2);
		in++;
		rb[in]=g; // ������ ����������� ������� � ����� ������������� �������.
	}
} // addboundary



// ���������� �� ����������� ������ ������� -
// ����������� ����������.
// �������� �������: ���� �������� ��� �����������, 
// ���������� ������������.
// in - �������������� ���������� ����� ������ 500,
// ������ ������� ��������� ����������� ����������.
void BubbleEnhSort(doublereal* &rb, integer in) {
	integer i,j,k;
	doublereal x;
	bool swapped; // ���� ������

	for (i=1; i<=in; i++) {
		k=0; // ���������� ������� ������� �������
		swapped=false; // ������� �� ����
		// i ��������� ��� �������, �� ������������� �������
		for (j=in; j>=i; j--) {
			if (rb[j-1]>rb[j]) {
				// SWAP
				x=rb[j-1];
				rb[j-1]=rb[j];
				rb[j]=x;
				k++;
				swapped=true;
			}
		}
		if (!swapped) break; // ����� �� �����
	}
} // BubbleEnhSort

// ���������� �������� ����� �� �������� 1959���.
void ShellSort(doublereal* &rb, integer in) {
	integer i, j;
	doublereal x;
	integer h;

	for (h = 1; h <= in / 9; h = 3 * h + 1);
	for (; h > 0; h /= 3) {
		for (i = h; i <= in; i++) {
			j = i; 
			x = rb[i];

			while (j >= h && x < rb[j-h]) {
				rb[j] = rb[j - h]; j -= h;
			}
			rb[j] = x;
		}
	}
} // ShellSort


// ���������� ���� (��� ��������).
const integer RUN = 32;

// this function sorts array from left index to
// to right index which is of size atmost RUN
void insertionSortTim(integer arr[], integer left, integer right)
{
	for (integer i = left + 1; i <= right; i++)
	{
		integer temp = arr[i];
		integer j = i - 1;
		while (arr[j] > temp && j >= left)
		{
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = temp;
	}
}

// merge function merges the sorted runs
void mergeTim(integer arr[], integer l, integer m, integer r)
{
	// original array is broken in two parts
	// left and right array
	integer len1 = m - l + 1, len2 = r - m;

	//integer left[len1], right[len2];
	integer *left = new integer[len1+1];
	integer *right = new integer[len2+1];

	for (integer i = 0; i < len1; i++)
		left[i] = arr[l + i];
	for (integer i = 0; i < len2; i++)
		right[i] = arr[m + 1 + i];

	integer i = 0;
	integer j = 0;
	integer k = l;

	// after comparing, we merge those two array
	// in larger sub array
	while (i < len1 && j < len2)
	{
		if (left[i] <= right[j])
		{
			arr[k] = left[i];
			i++;
		}
		else
		{
			arr[k] = right[j];
			j++;
		}
		k++;
	}

	// copy remaining elements of left, if any
	while (i < len1)
	{
		arr[k] = left[i];
		k++;
		i++;
	}

	// copy remaining element of right, if any
	while (j < len2)
	{
		arr[k] = right[j];
		k++;
		j++;
	}

	delete[] left;
	delete[] right;
}

// ���������� ������� �� ���� ����� �����.
// 8.05.2018
/*
integer min(integer a, integer b) {
	if (a <= b) return a;
	else return b;
}
*/

// ������� �������������� ������ O(n).
// iterative Timsort function to sort the
// array[0...n-1] (similar to merge sort)
void timSort(integer arr[], integer n)
{
	// Sort individual subarrays of size RUN
	for (integer i = 0; i < n; i += RUN)
		insertionSortTim(arr, i, min((i + 31), (n - 1)));

	// start merging from size RUN (or 32). It will merge
	// to form size 64, then 128, 256 and so on ....
	for (integer size = RUN; size < n; size = 2 * size)
	{
		// pick starting point of left sub array. We
		// are going to merge arr[left..left+size-1]
		// and arr[left+size, left+2*size-1]
		// After every merge, we increase left by 2*size
		for (integer left = 0; left < n; left += 2 * size)
		{
			// find ending point of left sub array
			// mid+1 is starting point of right sub array
			integer mid = left + size - 1;
			integer right = min((left + 2 * size - 1), (n - 1));

			// merge sub array arr[left.....mid] &
			// arr[mid+1....right]
			mergeTim(arr, left, mid, right);
		}
	}
}

// utility function to print the Array
void printArray(integer arr[], integer n)
{
	for (integer i = 0; i < n; i++)
		printf("%d  ", arr[i]);
	printf("\n");
}

// Driver program to test above function
integer test_Tim_Sort()
{
	integer arr[] = { 5, 21, 7, 23, 19 };
	integer n = sizeof(arr) / sizeof(arr[0]);
	printf("Given Array is\n");
	printArray(arr, n);

	timSort(arr, n);

	printf("After Sorting Array is\n");
	printArray(arr, n);
	return 0;
}

// ������������� ���������� �� ����������� ������ ������� -
// ����������� ����������.
// �������� �������: ���� �������� ��� �����������, 
// ���������� ������������.
// in - �������������� ���������� ����� ������ 200,
// ������ ������� ��������� ����������� ����������.
void BubbleEnhSorti(integer* &rb, integer in) {
	integer i,j,k;
	integer x;
	bool swapped; // ���� ������

	for (i=1; i<=in; i++) {
		k=0; // ���������� ������� ������� �������
		swapped=false; // ������� �� ����
		// i ��������� ��� �������, �� ������������� �������
		for (j=in; j>=i; j--) {
			if (rb[j-1]>rb[j]) {
				// SWAP
				x=rb[j-1];
				rb[j-1]=rb[j];
				rb[j]=x;
				k++;
				swapped=true;
			}
		}
		if (!swapped) break; // ����� �� �����
	}
} // BubbleEnhSorti

// ����� ��� ���������� (����� ������ ����������).
// �� ���������� �������������� ����������� ������.
void Sort_method(doublereal* &rb, integer in) {
	//BubbleEnhSort(rb, in);
	// 24 03 2017
	ShellSort(rb, in);
}


// ������ ����� ������ ���������� �����
// ���������� ������� ����� ���������.
// ������ ��������� �������� ��� ���������� ������������ 
// ���������� �����: � ������ ��� ��������� � ����������� ����� �������� dx1,
// ����������� ������� �� ���������� ����� � ����������� ����� �������� dx2.
// ������� ��������� ����������� �������� � ������ ������, ��� ����� 
// � ���� ����������� ����������� ��� ������ ��������� dx2/n, ��� n - ����������
// ������ � ������ ����� ��������.
// ���� � ����� ��� ������� ��������� ������ ������� ������ ����������� �������.
// ����������� ��� ����� ���������: ��������� ������ ���� ����������� ������ ����� 
// ������ ����������� � ������ � ��������� XY.
void simplecorrect_meshgen_x(doublereal* &xpos,  integer &inx, 
				   integer lb, integer ls, integer lw, BLOCK* b, SOURCE* s, WALL* w) {
	// ���������� ����� ����� simplemeshgen()
	integer inxcopy=inx;
	doublereal *xposcopy = new doublereal[inx+1]; // ����� xpos
	integer i=0;
	for (i=0; i<=inx; i++) xposcopy[i]=xpos[i]; // ����������� ������� ���������

    integer ipol=0;
	integer j,i1,i2; // �������� ����� for

    // ���������
	for (i=0; i<ls; i++) {
		if (s[i].iPlane==XY) {
		   // � ��������� XY
           ipol++;
		}
	}

	integer *ileft=new integer[ipol+1]; ileft[ipol]=-1; // ��������
	integer *iright=new integer[ipol+1]; iright[ipol]=-1; // ��������

	i1=0; i2=0;
    // ���������
	for (i=0; i<ls; i++) {
		if (s[i].iPlane==XY) {
		   // � ��������� XY
           for (j=0; j<=inx; j++) {
			   // ����� ������� ����������
			   if (fabs(xpos[j]-s[i].g.xS)<admission) { 
				   ileft[i1]=j;
				   i1++;
			   }
               // ������ ������� ����������
			   if (fabs(xpos[j]-s[i].g.xE)<admission) { 
				   iright[i2]=j;
				   i2++;
			   }
		   }
		}
	}

	// ������� ileft, iright ������ ���� �������������
    BubbleEnhSorti(ileft, ipol-1);
    BubbleEnhSorti(iright, ipol-1);

	// distmin - ������ ����������� ������
	// � ������ ����� �������� ����������. 
	// � ������ 2 ��������� � ����������� ����� ���� ��� ������� 26 ���. 
	doublereal distmin=1e20;
    integer k=0;
    for (j=0; j<=inxcopy; j++) {
		if (j==ileft[k]) {
			// ������� ����� �������
			if ((ileft[k]!=0)&&(distmin>(xposcopy[j]-xposcopy[j-1]))) distmin=(xposcopy[j]-xposcopy[j-1]);
		}
        if (j==iright[k]) {
			// ������� ����� �������
            if ((iright[k]!=inx)&&(distmin>(xposcopy[j+1]-xposcopy[j]))) distmin=(xposcopy[j+1]-xposcopy[j]);
			k++;
		}
	}

    // ������� ��������������� ��������� ��� ����� 
	// ������� ��������� ��������� � ������������ ���������
	// �� �������� ����� ��� � ����������� ���������.
	bool bflag=false;
    k=0;
    for (j=0; j<=inxcopy; j++) {
		if ((j==ileft[k])&&(ileft[k]!=0)) {
			// ������� ����� �������
			if (3.0*distmin<(xposcopy[j]-xposcopy[j-1])) {
               // ������� ��� ����������
               SetLength(xpos,inx+1,inx+4);
			   xpos[inx+1]=xposcopy[j]-3.0*distmin;
               xpos[inx+2]=xposcopy[j]-2.0*distmin;
               xpos[inx+3]=xposcopy[j]-distmin;
			   inx=inx+3;
               bflag=true;
			}
		}
		// 2 november 2016.
		//if ((j == iright[k]) && (iright[k]!=inxcopy)) {
        if ((j==iright[k])&&(iright[k]<inxcopy)) {
			// ������� ����� �������
			if (3.0*distmin<(xposcopy[j+1]-xposcopy[j])) {
				// ������� ��� ����������
                SetLength(xpos,inx+1,inx+4);
				xpos[inx+3]=xposcopy[j]+distmin;
                xpos[inx+2]=xposcopy[j]+2.0*distmin;
                xpos[inx+1]=xposcopy[j]+3.0*distmin;
			    inx=inx+3;
                bflag=true;
			}
			k++;
		}
	}

	if (bflag) {
		// ���������� �� �����������
        //BubbleEnhSort(xpos,inx);
		Sort_method(xpos,inx);
	    SetLength(xposcopy,inxcopy+1,inx+1);
	    inxcopy=inx;
	}
	
    for (i=0; i<=inx; i++) xposcopy[i]=xpos[i]; // ����������� ������� ���������
	inx=inx+2*ipol;
	delete[] xpos;
	xpos = new doublereal[inx+1];
	i=0; k=0; 
	for (j=0; j<=inxcopy; j++) {
		
			if ((j == ileft[k]) && (ileft[k] != 0)) {
				// ������� ����� �������
				if (i <= inx) {
					xpos[i++] = 0.5*(xposcopy[j - 1] + xposcopy[j]);
				}
			}
			if (i <= inx) {
				xpos[i++] = xposcopy[j];
			}
			if ((j == iright[k]) && (iright[k] != inxcopy)) {
				// ������� ����� �������
				if (i <= inx) {
					xpos[i++] = 0.5*(xposcopy[j] + xposcopy[j + 1]);
				}
				k++;
			}
		
	}

} // simplecorrect_meshgen_x

/*
doublereal fmin(doublereal ra, doublereal rb) {
	doublereal r=ra;
	if (rb<ra) r=rb;
	return rb;
} // fmin
*/

// ��������� ����������� ������ � ��������� XY � ������ �� ����� ������ �� Z.
// ����� ��������� ������ ������ � �����.
void simplecorrect_meshgen_z(doublereal* &zpos,  integer &inz, 
				   integer lb, integer ls, integer lw, BLOCK* b, SOURCE* s, WALL* w) {

	// ���������� ����� ����� simplemeshgen()
	integer i=0;

	bool bfindsourseXY=false;
	// ���������
	for (i=0; i<ls; i++) {
		if (s[i].iPlane==XY) {
			bfindsourseXY=true;
		}
	}

	if (bfindsourseXY) {
	
		// ������ ���� ���������� ��������� ����� ������� � ��������� XOY.

	    doublereal rzlevel;
        // ���������
	    for (i=0; i<ls; i++) {
		    if (s[i].iPlane==XY) {
		       // � ��������� XY
			   rzlevel=s[i].g.zS;
			   break; // ����� �� ����� for
		    }
	    }

	    integer izlevel=0;
	    for (i=0; i<=inz; i++) if (fabs(zpos[i]-rzlevel)<admission) { 
		    izlevel=i;
	    	break; // ��������� ����� �� ����� for
	    }

#if doubleintprecision == 1
		//printf("izlevel==%lld inz==%lld\n",izlevel, inz);
#else
		//printf("izlevel==%d inz==%d\n",izlevel, inz);
#endif
       
        //getchar(); // debug

	    doublereal dzminus, dzplus;
	    dzminus=zpos[izlevel]-zpos[izlevel-1];
        if (izlevel==inz) {
            // ����� ����������� ������ ��� ���� ��������� �������.

            // ����������� ����������� ����� �� ��� ������.
            SetLength(zpos,inz+1, inz+2);
            zpos[inz+1]=zpos[inz];
            zpos[inz]=zpos[inz-1]+0.5*dzminus;
            inz++;
	    }
	    else {
            // ������� ����� ������ ��� ���������� � �������.
	        dzplus=zpos[izlevel+1]-zpos[izlevel];
	        if (dzplus>3*dzminus) {
		       SetLength(zpos,inz+1, inz+4);
		       zpos[inz+1]=rzlevel+dzminus;
		       zpos[inz+2]=rzlevel+2.0*dzminus;
		       zpos[inz+3]=rzlevel+3.0*dzminus;
		       inz=inz+3;
               //BubbleEnhSort(zpos, inz);
			   Sort_method(zpos, inz);
	        }
	    }
	    /*else if (dzminus>3*dzplus) {
          SetLength(zpos,inz+1, inz+4);
		  zpos[inz+1]=rzlevel-dzplus;
		  zpos[inz+2]=rzlevel-2.0*dzplus;
		  zpos[inz+3]=rzlevel-3.0*dzplus;
		  inz=inz+3;
          //BubbleEnhSort(zpos, inz);
		  Sort_method(zpos, inz);
	    }*/

	    for (i=0; i<=inz; i++) if (fabs(zpos[i]-rzlevel)<admission) { 
	    	izlevel=i;
		    break; // ��������� ����� �� ����� for
	    }

        if (izlevel!=inz) {
           // ���� ������ ����� ��������� ���� ��������� ������� !
	       dzminus=fmin(0.5*(zpos[izlevel]-zpos[izlevel-1]),0.5*(zpos[izlevel+1]-zpos[izlevel]));
           SetLength(zpos,inz+1, inz+2);
	       zpos[inz+1]=rzlevel+dzminus;
	       //zpos[inz+2]=rzlevel-dzminus;
           inz=inz+1;
	    }
        //BubbleEnhSort(zpos, inz);
		Sort_method(zpos, inz);


		// � ��������� XOY ���� ���������� ��������� �����.
	}

} // simplecorrect_meshgen_z

// �������������� ��� ������� ��������� ����� ��������������
// �������� ����� ����������� ����������� ����������� � ��������� XY.
// ��������� ����� �������� � ����� ���.
void simplecorrect_meshgen_y(doublereal* &ypos,  integer &iny, 
				   integer lb, integer ls, integer lw, BLOCK* b, SOURCE* s, WALL* w) {

    // ���������  ����� ������ ��� ������� ��������� � ������ � ����� �� ��� OY 
    // �������� ��� ���� ����� � ��������� OXY � ��� ������� �� ��� Oz.
    // ������ �������� ������ ���� ������ ���������� ����� ������ ��� ������ �� ������� ��������� �������.

   // ���������� ����� ����� simplemeshgen()
	integer i=0;

	doublereal rymin=1.0e30, rymax=1.0e-40;
    // ���������
	for (i=0; i<ls; i++) {
		if (s[i].iPlane==XY) {
		   // � ��������� XY
			rymin=s[i].g.yS;
			rymax=s[i].g.yE;
			break; // ����� �� ����� for
		}
	}

    doublereal dyminus, dyplus;

	integer iup=0,idouwn=0;
	for (i=0; i<=iny; i++) if (fabs(ypos[i]-rymin)<admission) { 
		idouwn=i;
		break; // ��������� ����� �� ����� for
	}
    
    if (idouwn!=0) {
	   
	   dyminus=ypos[idouwn]-ypos[idouwn-1];
	   dyplus=ypos[idouwn+1]-ypos[idouwn];
       if (dyminus>3*dyplus) {
          SetLength(ypos,iny+1, iny+4);
		  ypos[iny+1]=rymin-dyplus;
		  ypos[iny+2]=rymin-2.0*dyplus;
		  ypos[iny+3]=rymin-3.0*dyplus;
		  iny=iny+3;
          //BubbleEnhSort(ypos, iny);
		  Sort_method(ypos, iny);
	   }
	}

	for (i=0; i<=iny; i++) if (fabs(ypos[i]-rymax)<admission) { 
		iup=i;
		break; // ��������� ����� �� ����� for
	}

    if (iup!=iny) {

       dyminus=ypos[iup]-ypos[iup-1];
	   dyplus=ypos[iup+1]-ypos[iup];
       if (dyplus>3*dyminus) {
          SetLength(ypos,iny+1, iny+4);
		  ypos[iny+1]=rymax+dyminus;
		  ypos[iny+2]=rymax+2.0*dyminus;
		  ypos[iny+3]=rymax+3.0*dyminus;
		  iny=iny+3;
          //BubbleEnhSort(ypos, iny);
		  Sort_method(ypos, iny);
	   }
	}

    for (i=0; i<=iny; i++) if (fabs(ypos[i]-rymin)<admission) { 
		idouwn=i;
		break; // ��������� ����� �� ����� for
	}

	for (i=0; i<=iny; i++) if (fabs(ypos[i]-rymax)<admission) { 
		iup=i;
		break; // ��������� ����� �� ����� for
	}

    if ((idouwn!=0)&&(iup!=iny)) {
       SetLength(ypos, iny+1, iny+5);
	   ypos[iny+1]=0.5*(ypos[idouwn-1]+ypos[idouwn]);
	   ypos[iny+2]=0.5*(ypos[idouwn]+ypos[idouwn+1]);
       ypos[iny+3]=0.5*(ypos[iup-1]+ypos[iup]);
	   ypos[iny+4]=0.5*(ypos[iup]+ypos[iup+1]);
	   iny=iny+4;
	}
    //BubbleEnhSort(ypos, iny);
	Sort_method(ypos, iny);

} // simplecorrect_meshgen_y

// ����� true ���� ���������������� ����� ib1 ������ 
// ���������������� ����� ib2.
bool comparison_lam(TPROP* matlist, BLOCK* b, integer ib1, integer ib2, doublereal t_C) {
	if ((matlist[b[ib1].imatid].blibmat == 0) && (matlist[b[ib2].imatid].blibmat == 0)) {
		//doublereal lam1 = matlist[b[ib1].imatid].lam;
		//doublereal lam2 = matlist[b[ib2].imatid].lam;
		doublereal lam1 = get_lam(matlist[b[ib1].imatid].n_lam, matlist[b[ib1].imatid].temp_lam, matlist[b[ib1].imatid].arr_lam, t_C);
		doublereal lam2 = get_lam(matlist[b[ib2].imatid].n_lam, matlist[b[ib2].imatid].temp_lam, matlist[b[ib2].imatid].arr_lam, t_C);
		if (lam1 > lam2) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if ((matlist[b[ib1].imatid].blibmat == 1) && (matlist[b[ib2].imatid].blibmat == 0)) {
		doublereal lam1=0.025,rho=1.1614,cp=1005, Tnode=20.0;
		// ������������, ����������������� ��������.
		my_solid_properties(Tnode, rho, cp, lam1, matlist[b[ib1].imatid].ilibident);
		
		//doublereal lam2 = matlist[b[ib2].imatid].lam;
		doublereal lam2= get_lam(matlist[b[ib2].imatid].n_lam, matlist[b[ib2].imatid].temp_lam, matlist[b[ib2].imatid].arr_lam, t_C);
		if (lam1 > lam2) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if ((matlist[b[ib1].imatid].blibmat == 0) && (matlist[b[ib2].imatid].blibmat == 1)) {
		doublereal lam2=0.025,rho=1.1614,cp=1005,Tnode=20.0;
		// ������������, ����������������� ��������.
		my_solid_properties(Tnode, rho, cp, lam2, matlist[b[ib2].imatid].ilibident);

		//doublereal lam1 = matlist[b[ib1].imatid].lam;
		doublereal lam1 = get_lam(matlist[b[ib1].imatid].n_lam, matlist[b[ib1].imatid].temp_lam, matlist[b[ib1].imatid].arr_lam, t_C);
		if (lam1 > lam2) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		doublereal lam1=0.025, rho1=1.1614, cp1=1005,Tnode=20.0;
		// ������������, ����������������� ��������.
		my_solid_properties(Tnode, rho1, cp1, lam1, matlist[b[ib1].imatid].ilibident);
		doublereal lam2=0.025, rho2=1.1614, cp2=1005;
		// ������������, ����������������� ��������.
		my_solid_properties(Tnode, rho2, cp2, lam2, matlist[b[ib2].imatid].ilibident);
		if (lam1 > lam2) {
			return 1;
		}
		else {
			return 0;
		}
	}
} // comparison_lam


  // 11.02.2017
void quolite_refinement(integer &inx, integer &iny, integer &inz, doublereal* &xpos, doublereal* &ypos, doublereal* &zpos) {

#if doubleintprecision == 1
	printf("apriory mesh size : inx=%lld iny=%lld inz=%lld\n", inx, iny, inz);
#else
	printf("apriory mesh size : inx=%d iny=%d inz=%d\n", inx, iny, inz);
#endif

	// check_mesh
	doublereal ratio_start_check = 0.0;
	for (integer i_1 = 0; i_1 < inx; i_1++) {
		for (integer j_1 = 0; j_1 < iny; j_1++) {
			for (integer k_1 = 0; k_1 < inz; k_1++) {
				doublereal dx = 1, dy = 1, dz = 1;
				dx = xpos[i_1 + 1] - xpos[i_1];
				dy = ypos[j_1 + 1] - ypos[j_1];
				dz = zpos[k_1 + 1] - zpos[k_1];
				if ((dx >= dy) && (dy >= dz)) {
					if (dx / dz > ratio_start_check) {
						ratio_start_check = dx / dz;
					}
				}
				if ((dx >= dz) && (dz >= dy)) {
					if (dx / dy > ratio_start_check) {
						ratio_start_check = dx / dy;
					}
				}
				if ((dy >= dx) && (dx >= dz)) {
					if (dy / dz > ratio_start_check) {
						ratio_start_check = dy / dz;
					}
				}
				if ((dy >= dz) && (dz >= dx)) {
					if (dy / dx > ratio_start_check) {
						ratio_start_check = dy / dx;
					}
				}
				if ((dz >= dx) && (dx >= dy)) {
					if (dz / dy > ratio_start_check) {
						ratio_start_check = dz / dy;
					}
				}
				if ((dz >= dy) && (dy >= dx)) {
					if (dz / dx > ratio_start_check) {
						ratio_start_check = dz / dx;
					}
				}
			}
		}
	}
	printf("mesh apriority quolite=%e\n", ratio_start_check);


	// �������������� ��������� �������� �����.
	// 10_02_2017
	// (etalon_max_size_ratio2==30 optimum) ��� � ������� �� flowvision
	const doublereal ratio_quality = etalon_max_size_ratio2;
	integer icorrect_stat = 0;
	bool bcont = true;

	integer ic4 = 0;

	if (0) {
		while (bcont) {
			integer i_progress = 0;
		START_LAB1:
			bcont = false;
			for (integer i_1 = 0; i_1 < inx; i_1++) {
				for (integer j_1 = 0; j_1 < iny; j_1++) {
					for (integer k_1 = 0; k_1 < inz; k_1++) {
						// ��������� ����� ���������� �����.
						if (i_1*iny*inz + j_1*inz + k_1 >= i_progress) {
							i_progress = i_1*iny*inz + j_1*inz + k_1;
							doublereal dx = 1, dy = 1, dz = 1;
							dx = xpos[i_1 + 1] - xpos[i_1];
							dy = ypos[j_1 + 1] - ypos[j_1];
							dz = zpos[k_1 + 1] - zpos[k_1];
							if ((dx >= dy) && (dy >= dz)) {
								if (dx / dz > ratio_quality) {
									bcont = true;
									SetLength(xpos, inx + 1, inx + 2);
									for (integer l = inx; l >= i_1 + 1; l--) xpos[l + 1] = xpos[l];
									//xpos[inx + 1] = xpos[i_1] + 0.5*dx;
									xpos[i_1 + 1] = xpos[i_1] + 0.5*dx;
									inx = inx + 1;
									//BubbleEnhSort(xpos, inx);
									//Sort_method(xpos, inx);
									icorrect_stat++;
									goto START_LAB1;
								}
							}
							if ((dx >= dz) && (dz >= dy)) {
								if (dx / dy > ratio_quality) {
									bcont = true;
									SetLength(xpos, inx + 1, inx + 2);
									for (integer l = inx; l >= i_1 + 1; l--) xpos[l + 1] = xpos[l];
									//xpos[inx + 1] = xpos[i_1] + 0.5*dx;
									xpos[i_1 + 1] = xpos[i_1] + 0.5*dx;
									inx = inx + 1;
									//BubbleEnhSort(xpos, inx);
									//Sort_method(xpos,inx);
									icorrect_stat++;
									goto START_LAB1;
								}
							}
							if ((dy >= dx) && (dx >= dz)) {
								if (dy / dz > ratio_quality) {
									bcont = true;
									SetLength(ypos, iny + 1, iny + 2);
									for (integer l = iny; l >= j_1 + 1; l--) ypos[l + 1] = ypos[l];
									//ypos[iny + 1] = ypos[j_1] + 0.5*dy;
									ypos[j_1 + 1] = ypos[j_1] + 0.5*dy;
									iny = iny + 1;
									//BubbleEnhSort(ypos, iny);
									//Sort_method(ypos,iny);
									icorrect_stat++;
									goto START_LAB1;
								}
							}
							if ((dy >= dz) && (dz >= dx)) {
								if (dy / dx > ratio_quality) {
									bcont = true;
									SetLength(ypos, iny + 1, iny + 2);
									for (integer l = iny; l >= j_1 + 1; l--) ypos[l + 1] = ypos[l];
									//ypos[iny + 1] = ypos[j_1] + 0.5*dy;
									ypos[j_1 + 1] = ypos[j_1] + 0.5*dy;
									iny = iny + 1;
									//BubbleEnhSort(ypos, iny);
									icorrect_stat++;
									goto START_LAB1;
								}
							}
							if ((dz >= dy) && (dy >= dx)) {
								if (dz / dx > ratio_quality) {
									bcont = true;
									SetLength(zpos, inz + 1, inz + 2);
									for (integer l = inz; l >= k_1 + 1; l--) zpos[l + 1] = zpos[l];
									//zpos[inz + 1] = zpos[k_1] + 0.5*dz;
									zpos[k_1 + 1] = zpos[k_1] + 0.5*dz;
									inz = inz + 1;
									//BubbleEnhSort(zpos, inz);
									//Sort_method(zpos,inz);
									icorrect_stat++;
									goto START_LAB1;
								}
							}
							if ((dz >= dx) && (dx >= dy)) {
								if (dz / dy > ratio_quality) {
									bcont = true;
									SetLength(zpos, inz + 1, inz + 2);
									for (integer l = inz; l >= k_1 + 1; l--) zpos[l + 1] = zpos[l];
									//zpos[inz + 1] = zpos[k_1] + 0.5*dz;
									zpos[k_1 + 1] = zpos[k_1] + 0.5*dz;
									inz = inz + 1;
									//BubbleEnhSort(zpos, inz);
									//Sort_method(zpos, inz);
									icorrect_stat++;
									goto START_LAB1;
								}
							}
						}
					}
				}
			}
		}
	}
	else if (1) {
		// �������� ���� ��������� �� ������� ����� ��������� ������, � ����� ����������
		// �������� �������� ����� � ��������� � �� ����� � ����� ������.
		// ������������������ ��� ������� � ������ ������� ������� ��������� ������.
		// ��� ���� ����� ���������.

		// ������ ������ �� ������ 18 ����� 2017 �������� ��������� ���������������, 
		// ��� ���������� ������������ ������. � �� ����� ��� ������ ��� ������� �� ������ ����������� ���� ���� 
		// ��������� ����� ����� �������� ����� ������ ���������������, ����� � ��� �������� ����������� �����.


		//while (bcont) {

	START_LAB:

#if doubleintprecision == 1
		//printf("%lld ",0);
#else
		//printf("%d ",0);
#endif
			
			ic4++;
			ratio_start_check = 0.0;
			bcont = false;

			//integer i_1 = 0, j_1 = 0, k_1 = 0;
			integer ic9 = 0;
			integer i_1[100], j_1[100], k_1[100];
			bool *i_b=NULL, *j_b=NULL, *k_b=NULL;
			i_b = new bool[inx];
			j_b = new bool[iny];
			k_b = new bool[inz];

			for (integer i_1l = 0; i_1l < inx; i_1l++) i_b[i_1l] = false;
			for (integer j_1l = 0; j_1l < iny; j_1l++) j_b[j_1l] = false;
			for (integer k_1l = 0; k_1l < inz; k_1l++) k_b[k_1l] = false;

			


			for (integer i_1l = 0; i_1l < inx; i_1l++) {
				for (integer j_1l = 0; j_1l < iny; j_1l++) {
					for (integer k_1l = 0; k_1l < inz; k_1l++) {
						// ��������� ����� ���������� �����.

						doublereal dx = 1, dy = 1, dz = 1;
						dx = xpos[i_1l + 1] - xpos[i_1l];
						dy = ypos[j_1l + 1] - ypos[j_1l];
						dz = zpos[k_1l + 1] - zpos[k_1l];
#if doubleintprecision == 1
					if (fabs(dx) < 1.0e-40) {
							printf("x[%lld]=%e x[%lld]=%e x[%lld]=%e\n", i_1l - 1, xpos[i_1l - 1], i_1l, xpos[i_1l], i_1l + 1, xpos[i_1l + 1]);
							system("pause");
					}
					if (fabs(dy) < 1.0e-40) {
							printf("y[%lld]=%e y[%lld]=%e y[%lld]=%e\n", j_1l - 1, ypos[j_1l - 1], j_1l, ypos[j_1l], j_1l + 1, ypos[j_1l + 1]);
							system("pause");
					}
					if (fabs(dz) < 1.0e-40) {
							printf("z[%lld]=%e z[%lld]=%e z[%lld]=%e\n", k_1l - 1, zpos[k_1l - 1], k_1l, zpos[k_1l], k_1l + 1, zpos[k_1l + 1]);
							system("pause");
					}
#else
						if (fabs(dx) < 1.0e-40) {
							printf("x[%d]=%e x[%d]=%e x[%d]=%e\n", i_1l - 1, xpos[i_1l - 1], i_1l, xpos[i_1l], i_1l + 1, xpos[i_1l + 1]);
							system("pause");
					}
						if (fabs(dy) < 1.0e-40) {
							printf("y[%d]=%e y[%d]=%e y[%d]=%e\n", j_1l - 1, ypos[j_1l - 1], j_1l, ypos[j_1l], j_1l + 1, ypos[j_1l + 1]);
							system("pause");
						}
						if (fabs(dz) < 1.0e-40) {
							printf("z[%d]=%e z[%d]=%e z[%d]=%e\n", k_1l - 1, zpos[k_1l - 1], k_1l, zpos[k_1l], k_1l + 1, zpos[k_1l + 1]);
							system("pause");
						}
#endif
						
						if ((dx >= dy) && (dy >= dz)) {
							if (dx / dz > ratio_quality) {
								if (i_b[i_1l] == false) {
									if (fabs(dz) > 1.0e-40) {
										ratio_start_check = dx / dz;
										i_1[ic9] = i_1l;
										j_1[ic9] = j_1l;
										k_1[ic9] = k_1l;
										i_b[i_1l] = true;
										ic9++;
									}
									else {
#if doubleintprecision == 1
										printf("i=%lld, j=%lld, k=%lld, inx=%lld, iny=%lld, inz=%lld, dx=%e, dy=%e, dz=%e\n", i_1l, j_1l, k_1l, inx, iny, inz, dx, dy, dz);
#else
										printf("i=%d, j=%d, k=%d, inx=%d, iny=%d, inz=%d, dx=%e, dy=%e, dz=%e\n", i_1l, j_1l, k_1l, inx, iny, inz, dx, dy, dz);
#endif
										system("pause");
									}
								}
							}
						}
						if ((dx >= dz) && (dz >= dy)) {
							if (dx / dy > ratio_quality) {
								if (i_b[i_1l] == false) {
									if (fabs(dy) > 1.0e-40) {
										ratio_start_check = dx / dy;
										i_1[ic9] = i_1l;
										j_1[ic9] = j_1l;
										k_1[ic9] = k_1l;
										i_b[i_1l] = true;
										ic9++;
									}
									else {
#if doubleintprecision == 1
										printf("i=%lld, j=%lld, k=%lld, inx=%lld, iny=%lld, inz=%lld, dx=%e, dy=%e, dz=%e\n", i_1l, j_1l, k_1l, inx, iny, inz, dx, dy, dz);
#else
										printf("i=%d, j=%d, k=%d, inx=%d, iny=%d, inz=%d, dx=%e, dy=%e, dz=%e\n", i_1l, j_1l, k_1l, inx, iny, inz, dx, dy, dz);
#endif
										system("pause");
									}
								}
							}
						}
						if ((dy >= dx) && (dx >= dz)) {
							if (dy / dz > ratio_quality) {
								if (j_b[j_1l] == false) {
									if (fabs(dz) > 1.0e-40) {
										ratio_start_check = dy / dz;
										i_1[ic9] = i_1l;
										j_1[ic9] = j_1l;
										k_1[ic9] = k_1l;
										j_b[j_1l] = true;
										ic9++;
									}
									else {
#if doubleintprecision == 1
										printf("i=%lld, j=%lld, k=%lld, inx=%lld, iny=%lld, inz=%lld, dx=%e, dy=%e, dz=%e\n", i_1l, j_1l, k_1l, inx, iny, inz, dx, dy, dz);
#else
										printf("i=%d, j=%d, k=%d, inx=%d, iny=%d, inz=%d, dx=%e, dy=%e, dz=%e\n", i_1l, j_1l, k_1l, inx, iny, inz, dx, dy, dz);
#endif
										system("pause");
									}
								}
							}
						}
						if ((dy >= dz) && (dz >= dx)) {
							if (dy / dx > ratio_quality) {
								if (j_b[j_1l] == false) {
									if (fabs(dx) > 1.0e-40) {
										ratio_start_check = dy / dx;
										i_1[ic9] = i_1l;
										j_1[ic9] = j_1l;
										k_1[ic9] = k_1l;
										j_b[j_1l] = true;
										ic9++;
									}
									else {
#if doubleintprecision == 1
										printf("i=%lld, j=%lld, k=%lld, inx=%lld, iny=%lld, inz=%lld, dx=%e, dy=%e, dz=%e\n", i_1l, j_1l, k_1l, inx, iny, inz, dx, dy, dz);
#else
										printf("i=%d, j=%d, k=%d, inx=%d, iny=%d, inz=%d, dx=%e, dy=%e, dz=%e\n", i_1l, j_1l, k_1l, inx, iny, inz, dx, dy, dz);
#endif
										system("pause");
									}
								}
							}
						}
						if ((dz >= dx) && (dx >= dy)) {
							if (dz / dy > ratio_quality) {
								if (k_b[k_1l] == false) {
									if (fabs(dy) > 1.0e-40) {
										ratio_start_check = dz / dy;
										i_1[ic9] = i_1l;
										j_1[ic9] = j_1l;
										k_1[ic9] = k_1l;
										k_b[k_1l] = true;
										ic9++;
									}
									else {
#if doubleintprecision == 1
										printf("i=%lld, j=%lld, k=%lld, inx=%lld, iny=%lld, inz=%lld, dx=%e, dy=%e, dz=%e\n", i_1l, j_1l, k_1l, inx, iny, inz, dx, dy, dz);
#else
										printf("i=%d, j=%d, k=%d, inx=%d, iny=%d, inz=%d, dx=%e, dy=%e, dz=%e\n", i_1l, j_1l, k_1l, inx, iny, inz, dx, dy, dz);
#endif
										system("pause");
									}
								}
							}
						}
						if ((dz >= dy) && (dy >= dx)) {
							if (dz / dx > ratio_quality) {
								if (k_b[k_1l] == false) {
									if (fabs(dx) > 1.0e-40) {
										ratio_start_check = dz / dx;
										i_1[ic9] = i_1l;
										j_1[ic9] = j_1l;
										k_1[ic9] = k_1l;
										k_b[k_1l] = true;
										ic9++;
									}
									else {
#if doubleintprecision == 1
										printf("i=%lld, j=%lld, k=%lld, inx=%lld, iny=%lld, inz=%lld, dx=%e, dy=%e, dz=%e\n", i_1l, j_1l, k_1l, inx, iny, inz, dx, dy, dz);
#else
										printf("i=%d, j=%d, k=%d, inx=%d, iny=%d, inz=%d, dx=%e, dy=%e, dz=%e\n", i_1l, j_1l, k_1l, inx, iny, inz, dx, dy, dz);

#endif
										system("pause");
									}
								}
							}
						}
						if (ic9 > 93) {
							// STOP process.
							i_1l = inx;
							j_1l = iny;
							k_1l = inz;
						}

					}
				}
			}

			delete[] i_b;
			delete[] j_b;
			delete[] k_b;

#if doubleintprecision == 1
			printf("ic9=%lld %e ", ic9, ratio_start_check);
#else
			printf("ic9=%d %e ", ic9, ratio_start_check);
#endif
			

			for (integer ic7=0; ic7<ic9; ic7++)
			{
			
				doublereal dx = 1, dy = 1, dz = 1;
				dx = xpos[i_1[ic7] + 1] - xpos[i_1[ic7]];
				dy = ypos[j_1[ic7] + 1] - ypos[j_1[ic7]];
				dz = zpos[k_1[ic7] + 1] - zpos[k_1[ic7]];
				if ((dx >= dy) && (dy >= dz)) {
					if (dx / dz > ratio_quality) {

						bool badd = true;
						for (integer i97 = 0; i97 <= inx; i97++) if (fabs(xpos[i97] - (xpos[i_1[ic7]] + 0.5*dx)) < 1.0e-40) badd = false;

						if (badd) {
							bcont = true;
							SetLength(xpos, inx + 1, inx + 2);
							//for (integer l = inx; l >= i_1[ic7] + 1; l--) xpos[l + 1] = xpos[l];
							//xpos[inx + 1] = xpos[i_1[ic7]] + 0.5*dx;
							//xpos[i_1[ic7] + 1] = xpos[i_1[ic7]] + 0.5*dx;
							// ��������� � �����
							xpos[inx + 1] = xpos[i_1[ic7]] + 0.5*dx;
							inx = inx + 1;
							//BubbleEnhSort(xpos, inx);
							//Sort_method(xpos, inx);
							icorrect_stat++;
							//goto START_LAB;
							goto END_LAB8;
						}
					}
				}
				else if ((dx >= dz) && (dz >= dy)) {
					if (dx / dy > ratio_quality) {


						bool badd = true;
						for (integer i97 = 0; i97 <= inx; i97++) if (fabs(xpos[i97] - (xpos[i_1[ic7]] + 0.5*dx)) < 1.0e-40) badd = false;

						if (badd) {

							bcont = true;
							SetLength(xpos, inx + 1, inx + 2);
							//for (integer l = inx; l >= i_1[ic7] + 1; l--) xpos[l + 1] = xpos[l];
							//xpos[inx + 1] = xpos[i_1[ic7]] + 0.5*dx;
							//xpos[i_1[ic7] + 1] = xpos[i_1[ic7]] + 0.5*dx;
							// ��������� � �����
							xpos[inx + 1] = xpos[i_1[ic7]] + 0.5*dx;
							inx = inx + 1;
							//BubbleEnhSort(xpos, inx);
							//Sort_method(xpos, inx);
							icorrect_stat++;
							//goto START_LAB;
							goto END_LAB8;
						}
					}
				}
				if ((dy >= dx) && (dx >= dz)) {
					if (dy / dz > ratio_quality) {


						bool badd = true;
						for (integer i97 = 0; i97 <= iny; i97++) if (fabs(ypos[i97] - (ypos[j_1[ic7]] + 0.5*dy)) < 1.0e-40) badd = false;

						if (badd) {

							bcont = true;
							SetLength(ypos, iny + 1, iny + 2);
							//for (integer l = iny; l >= j_1[ic7] + 1; l--) ypos[l + 1] = ypos[l];
							//ypos[iny + 1] = ypos[j_1[ic7]] + 0.5*dy;
							//ypos[j_1[ic7] + 1] = ypos[j_1[ic7]] + 0.5*dy;
							// ��������� � �����
							ypos[iny + 1] = ypos[j_1[ic7]] + 0.5*dy;
							iny = iny + 1;
							//BubbleEnhSort(ypos, iny);
							//Sort_method(ypos, iny);
							icorrect_stat++;
							//goto START_LAB;
							goto END_LAB8;
						}
					}
				}
				else if ((dy >= dz) && (dz >= dx)) {
					if (dy / dx > ratio_quality) {

						bool badd = true;
						for (integer i97 = 0; i97 <= iny; i97++) if (fabs(ypos[i97] - (ypos[j_1[ic7]] + 0.5*dy)) < 1.0e-40) badd = false;

						if (badd) {

							bcont = true;
							SetLength(ypos, iny + 1, iny + 2);
							//for (integer l = iny; l >= j_1[ic7] + 1; l--) ypos[l + 1] = ypos[l];
							//ypos[iny + 1] = ypos[j_1[ic7]] + 0.5*dy;
							//ypos[j_1[ic7] + 1] = ypos[j_1[ic7]] + 0.5*dy;
							// ��������� � �����
							ypos[iny + 1] = ypos[j_1[ic7]] + 0.5*dy;
							iny = iny + 1;
							//BubbleEnhSort(ypos, iny);
							//Sort_method(ypos, iny);
							icorrect_stat++;
							//goto START_LAB;
							goto END_LAB8;
						}
					}
				}
				if ((dz >= dy) && (dy >= dx)) {
					if (dz / dx > ratio_quality) {

						bool badd = true;
						for (integer i97 = 0; i97 <= inz; i97++) if (fabs(zpos[i97] - (zpos[k_1[ic7]] + 0.5*dz)) < 1.0e-40) badd = false;

						if (badd) {

							bcont = true;
							SetLength(zpos, inz + 1, inz + 2);
							//for (integer l = inz; l >= k_1[ic7] + 1; l--) zpos[l + 1] = zpos[l];
							//zpos[inz + 1] = zpos[k_1[ic7]] + 0.5*dz;
							//zpos[k_1[ic7] + 1] = zpos[k_1[ic7]] + 0.5*dz;
							// ��������� � �����
							zpos[inz + 1] = zpos[k_1[ic7]] + 0.5*dz;
							inz = inz + 1;
							//BubbleEnhSort(zpos, inz);
							//Sort_method(zpos, inz);
							icorrect_stat++;
							//goto START_LAB;
							goto END_LAB8;
						}
					}
				}
				else if ((dz >= dx) && (dx >= dy)) {
					if (dz / dy > ratio_quality) {

						bool badd = true;
						for (integer i97 = 0; i97 <= inz; i97++) if (fabs(zpos[i97] - (zpos[k_1[ic7]] + 0.5*dz)) < 1.0e-40) badd = false;

						if (badd) {

							bcont = true;
							SetLength(zpos, inz + 1, inz + 2);
							//for (integer l = inz; l >= k_1[ic7] + 1; l--) zpos[l + 1] = zpos[l];
							//zpos[inz + 1] = zpos[k_1[ic7]] + 0.5*dz;
							//zpos[k_1[ic7] + 1] = zpos[k_1[ic7]] + 0.5*dz;
							// ��������� � �����
							zpos[inz + 1] = zpos[k_1[ic7]] + 0.5*dz;
							inz = inz + 1;
							//BubbleEnhSort(zpos, inz);
							//Sort_method(zpos, inz);
							icorrect_stat++;
							//goto START_LAB;
							goto END_LAB8;
						}
					}
				}
				
			END_LAB8 :
				// ���� ������ ��� ������ �������� �� ��������� ������ ����������� �2059.
				ic7++;
				ic7--;

			}
			//BubbleEnhSort(xpos, inx);
			//BubbleEnhSort(ypos, iny);
			//BubbleEnhSort(zpos, inz);
			Sort_method(xpos,inx);
			Sort_method(ypos,iny);
			Sort_method(zpos,inz);
			if (bcont == true) {
				goto START_LAB;
			}
		//}
	}
	else  {
		// �������� ���� ��������� �� ������� ����� ��������� ������, � ����� ����������
		// �������� �������� ����� � ��������� � �� ����� � ����� ������.
		// ������������������ ��� ������� � ������ ������� ������� ��������� ������.
		// ��� ���� ����� ���������.
		

		while (bcont) {

		START_LAB2:
#if doubleintprecision == 1
			printf("%d ", 0);
#else
			printf("%d ", 0);
#endif
			
			ic4++;
			ratio_start_check = 0.0;
			bcont = false;

			//integer i_1 = 0, j_1 = 0, k_1 = 0;
			integer ic9 = 0;
			integer i_1[100], j_1[100], k_1[100];

			for (integer i_1l = 0; i_1l < inx; i_1l++) {
				for (integer j_1l = 0; j_1l < iny; j_1l++) {
					for (integer k_1l = 0; k_1l < inz; k_1l++) {
						// ��������� ����� ���������� �����.

						doublereal dx = 1, dy = 1, dz = 1;
						dx = xpos[i_1l + 1] - xpos[i_1l];
						dy = ypos[j_1l + 1] - ypos[j_1l];
						dz = zpos[k_1l + 1] - zpos[k_1l];
						if ((dx >= dy) && (dy >= dz)) {
							if (dx / dz > ratio_start_check) {
								ratio_start_check = dx / dz;
								i_1[ic9] = i_1l;
								j_1[ic9] = j_1l;
								k_1[ic9] = k_1l;
								//ic9++;
							}
						}
						if ((dx >= dz) && (dz >= dy)) {
							if (dx / dy > ratio_start_check) {
								ratio_start_check = dx / dy;
								i_1[ic9] = i_1l;
								j_1[ic9] = j_1l;
								k_1[ic9] = k_1l;
								//ic9++;
							}
						}
						if ((dy >= dx) && (dx >= dz)) {
							if (dy / dz > ratio_start_check) {
								ratio_start_check = dy / dz;
								i_1[ic9] = i_1l;
								j_1[ic9] = j_1l;
								k_1[ic9] = k_1l;
								//ic9++;
							}
						}
						if ((dy >= dz) && (dz >= dx)) {
							if (dy / dx > ratio_start_check) {
								ratio_start_check = dy / dx;
								i_1[ic9] = i_1l;
								j_1[ic9] = j_1l;
								k_1[ic9] = k_1l;
								//ic9++;
							}
						}
						if ((dz >= dx) && (dx >= dy)) {
							if (dz / dy > ratio_start_check) {
								ratio_start_check = dz / dy;
								i_1[ic9] = i_1l;
								j_1[ic9] = j_1l;
								k_1[ic9] = k_1l;
								//ic9++;
							}
						}
						if ((dz >= dy) && (dy >= dx)) {
							if (dz / dx > ratio_start_check) {
								ratio_start_check = dz / dx;
								i_1[ic9] = i_1l;
								j_1[ic9] = j_1l;
								k_1[ic9] = k_1l;
								//ic9++;
							}
						}
						if (ic9 > 93) {
							// STOP process.
							i_1l = inx;
							j_1l = iny;
							k_1l = inz;
						}

					}
				}
			}

			integer ic7 = 0;
			{
				doublereal dx = 1, dy = 1, dz = 1;
				dx = xpos[i_1[ic7] + 1] - xpos[i_1[ic7]];
				dy = ypos[j_1[ic7] + 1] - ypos[j_1[ic7]];
				dz = zpos[k_1[ic7] + 1] - zpos[k_1[ic7]];
				if ((dx >= dy) && (dy >= dz)) {
					if (dx / dz > ratio_quality) {
						bcont = true;
						SetLength(xpos, inx + 1, inx + 2);
						for (integer l = inx; l >= i_1[ic7] + 1; l--) xpos[l + 1] = xpos[l];
						//xpos[inx + 1] = xpos[i_1[ic7]] + 0.5*dx;
						xpos[i_1[ic7] + 1] = xpos[i_1[ic7]] + 0.5*dx;
						
						inx = inx + 1;
						//BubbleEnhSort(xpos, inx);
						//Sort_method(xpos,inx);
						icorrect_stat++;
						goto START_LAB;
					}
				}
				if ((dx >= dz) && (dz >= dy)) {
					if (dx / dy > ratio_quality) {
						bcont = true;
						SetLength(xpos, inx + 1, inx + 2);
						for (integer l = inx; l >= i_1[ic7] + 1; l--) xpos[l + 1] = xpos[l];
						//xpos[inx + 1] = xpos[i_1[ic7]] + 0.5*dx;
						xpos[i_1[ic7] + 1] = xpos[i_1[ic7]] + 0.5*dx;
						
						inx = inx + 1;
						//BubbleEnhSort(xpos, inx);
						//Sort_method(xpos, inx);
						icorrect_stat++;
						goto START_LAB2;
					}
				}
				if ((dy >= dx) && (dx >= dz)) {
					if (dy / dz > ratio_quality) {
						bcont = true;
						SetLength(ypos, iny + 1, iny + 2);
						for (integer l = iny; l >= j_1[ic7] + 1; l--) ypos[l + 1] = ypos[l];
						//ypos[iny + 1] = ypos[j_1[ic7]] + 0.5*dy;
						ypos[j_1[ic7] + 1] = ypos[j_1[ic7]] + 0.5*dy;
						
						iny = iny + 1;
						//BubbleEnhSort(ypos, iny);
						//Sort_method(ypos,iny);
						icorrect_stat++;
						goto START_LAB2;
					}
				}
				if ((dy >= dz) && (dz >= dx)) {
					if (dy / dx > ratio_quality) {
						bcont = true;
						SetLength(ypos, iny + 1, iny + 2);
						for (integer l = iny; l >= j_1[ic7] + 1; l--) ypos[l + 1] = ypos[l];
						//ypos[iny + 1] = ypos[j_1[ic7]] + 0.5*dy;
						ypos[j_1[ic7] + 1] = ypos[j_1[ic7]] + 0.5*dy;
						
						iny = iny + 1;
						//BubbleEnhSort(ypos, iny);
						//Sort_method(ypos, iny);
						icorrect_stat++;
						goto START_LAB2;
					}
				}
				if ((dz >= dy) && (dy >= dx)) {
					if (dz / dx > ratio_quality) {
						bcont = true;
						SetLength(zpos, inz + 1, inz + 2);
						for (integer l = inz; l >= k_1[ic7] + 1; l--) zpos[l + 1] = zpos[l];
						//zpos[inz + 1] = zpos[k_1[ic7]] + 0.5*dz;
						zpos[k_1[ic7] + 1] = zpos[k_1[ic7]] + 0.5*dz;
						
						inz = inz + 1;
						//BubbleEnhSort(zpos, inz);
						//Sort_method(zpos,inz);
						icorrect_stat++;
						goto START_LAB2;
					}
				}
				if ((dz >= dx) && (dx >= dy)) {
					if (dz / dy > ratio_quality) {
						bcont = true;
						SetLength(zpos, inz + 1, inz + 2);
						for (integer l = inz; l >= k_1[ic7] + 1; l--) zpos[l + 1] = zpos[l];
						//zpos[inz + 1] = zpos[k_1[ic7]] + 0.5*dz;
						zpos[k_1[ic7] + 1] = zpos[k_1[ic7]] + 0.5*dz;
						
						inz = inz + 1;
						//BubbleEnhSort(zpos, inz);
						//Sort_method(zpos, inz);
						icorrect_stat++;
						goto START_LAB2;
					}
				}
			}
			//BubbleEnhSort(xpos, inx);
			//BubbleEnhSort(ypos, iny);
			//BubbleEnhSort(zpos, inz);
			//Sort_method(xpos,inx);
			//Sort_method(ypos,iny);
			//Sort_method(zpos,inz);
			//goto START_LAB;
		}

#if doubleintprecision == 1
		printf("%lld \n", ic4);
#else
		printf("%d \n", ic4);
#endif
		
	}
#if doubleintprecision == 1
	printf("automatic correct mesh ratio_quality=%e ipass=%lld\n", ratio_quality, icorrect_stat);
#else
	printf("automatic correct mesh ratio_quality=%e ipass=%d\n", ratio_quality, icorrect_stat);
#endif
	

	ratio_start_check = 0.0;
	for (integer i_1 = 0; i_1 < inx; i_1++) {
		for (integer j_1 = 0; j_1 < iny; j_1++) {
			for (integer k_1 = 0; k_1 < inz; k_1++) {
				doublereal dx = 1, dy = 1, dz = 1;
				dx = xpos[i_1 + 1] - xpos[i_1];
				dy = ypos[j_1 + 1] - ypos[j_1];
				dz = zpos[k_1 + 1] - zpos[k_1];
				if ((dx >= dy) && (dy >= dz)) {
					if (dx / dz > ratio_start_check) {
						ratio_start_check = dx / dz;
					}
				}
				if ((dx >= dz) && (dz >= dy)) {
					if (dx / dy > ratio_start_check) {
						ratio_start_check = dx / dy;
					}
				}
				if ((dy >= dx) && (dx >= dz)) {
					if (dy / dz > ratio_start_check) {
						ratio_start_check = dy / dz;
					}
				}
				if ((dy >= dz) && (dz >= dx)) {
					if (dy / dx > ratio_start_check) {
						ratio_start_check = dy / dx;
					}
				}
				if ((dz >= dx) && (dx >= dy)) {
					if (dz / dy > ratio_start_check) {
						ratio_start_check = dz / dy;
					}
				}
				if ((dz >= dy) && (dy >= dx)) {
					if (dz / dx > ratio_start_check) {
						ratio_start_check = dz / dx;
					}
				}
			}
		}
	}
	printf("mesh post refinement quolite=%e\n", ratio_start_check);
}

// ���������� ���������� ����� �������� ����������� �������� �����.
integer myisblock_id(integer lb, BLOCK* &b, doublereal x11, doublereal y11, doublereal z11) {
	integer ib = 0;
	for (integer i_1 = lb-1; i_1 >= 0; i_1--) {
		if ((b[i_1].g.xS < x11) && (b[i_1].g.xE > x11) && (b[i_1].g.yS < y11) && (b[i_1].g.yE > y11) && (b[i_1].g.zS < z11) && (b[i_1].g.zE > z11)) {
			ib = i_1;
			// ���� ���� ������ �� ������������ ����� ������������.
			break;
		}
	}
	return ib;
}

// 1.09.2017.
doublereal my_sign(doublereal a) {
	if (a >= 0.0) {
		return 1.0;
	}
	else {
		return -1.0;
	}
} // my_sign

// ����� ������������ �����.
// 1.09.2017.
bool in_polygon_check(TOCHKA p, integer nsizei, doublereal* &xi, doublereal* &yi, doublereal* &zi, doublereal* &hi, integer iPlane_obj2, integer &k, integer ib) {

	bool bfound = false;

	doublereal sumphi = 0.0;
	integer i_73 = 0;

	switch (iPlane_obj2) {
	case XY:
		for (i_73 = 0; i_73 < nsizei - 1; i_73++) {
			sumphi += acos(((xi[i_73]-p.x)*(xi[i_73+1]-p.x)+ (yi[i_73] - p.y)*(yi[i_73 + 1] - p.y))/(sqrt((xi[i_73] - p.x)*(xi[i_73] - p.x)+(yi[i_73] - p.y)*(yi[i_73] - p.y))*sqrt((xi[i_73+1] - p.x)*(xi[i_73+1] - p.x) + (yi[i_73+1] - p.y)*(yi[i_73+1] - p.y))))*my_sign((xi[i_73] - p.x)*(yi[i_73+1] - p.y)- (xi[i_73+1] - p.x)*(yi[i_73] - p.y));
		}
		i_73 = nsizei - 1;
		sumphi += acos(((xi[i_73] - p.x)*(xi[0] - p.x) + (yi[i_73] - p.y)*(yi[0] - p.y)) / (sqrt((xi[i_73] - p.x)*(xi[i_73] - p.x) + (yi[i_73] - p.y)*(yi[i_73] - p.y))*sqrt((xi[0] - p.x)*(xi[0] - p.x) + (yi[0] - p.y)*(yi[0] - p.y))))*my_sign((xi[i_73] - p.x)*(yi[0] - p.y) - (xi[0] - p.x)*(yi[i_73] - p.y));
		break;
	case XZ:
		for (i_73 = 0; i_73 < nsizei - 1; i_73++) {
			sumphi += acos(((xi[i_73] - p.x)*(xi[i_73 + 1] - p.x) + (zi[i_73] - p.z)*(zi[i_73 + 1] - p.z)) / (sqrt((xi[i_73] - p.x)*(xi[i_73] - p.x) + (zi[i_73] - p.z)*(zi[i_73] - p.z))*sqrt((xi[i_73 + 1] - p.x)*(xi[i_73 + 1] - p.x) + (zi[i_73 + 1] - p.z)*(zi[i_73 + 1] - p.z))))*my_sign((xi[i_73] - p.x)*(zi[i_73 + 1] - p.z) - (xi[i_73 + 1] - p.x)*(zi[i_73] - p.z));
		}
		i_73 = nsizei - 1;
		sumphi += acos(((xi[i_73] - p.x)*(xi[0] - p.x) + (zi[i_73] - p.z)*(zi[0] - p.z)) / (sqrt((xi[i_73] - p.x)*(xi[i_73] - p.x) + (zi[i_73] - p.z)*(zi[i_73] - p.z))*sqrt((xi[0] - p.x)*(xi[0] - p.x) + (zi[0] - p.z)*(zi[0] - p.z))))*my_sign((xi[i_73] - p.x)*(zi[0] - p.z) - (xi[0] - p.x)*(zi[i_73] - p.z));
		break;
	case YZ:
		for (i_73 = 0; i_73 < nsizei - 1; i_73++) {
			sumphi += acos(((yi[i_73] - p.y)*(yi[i_73 + 1] - p.y) + (zi[i_73] - p.z)*(zi[i_73 + 1] - p.z)) / (sqrt((yi[i_73] - p.y)*(yi[i_73] - p.y) + (zi[i_73] - p.z)*(zi[i_73] - p.z))*sqrt((yi[i_73 + 1] - p.y)*(yi[i_73 + 1] - p.y) + (zi[i_73 + 1] - p.z)*(zi[i_73 + 1] - p.z))))*my_sign((yi[i_73] - p.y)*(zi[i_73 + 1] - p.z) - (yi[i_73 + 1] - p.y)*(zi[i_73] - p.z));
		}
		i_73 = nsizei - 1;
		sumphi += acos(((yi[i_73] - p.y)*(yi[0] - p.y) + (zi[i_73] - p.z)*(zi[0] - p.z)) / (sqrt((yi[i_73] - p.y)*(yi[i_73] - p.y) + (zi[i_73] - p.z)*(zi[i_73] - p.z))*sqrt((yi[0] - p.y)*(yi[0] - p.y) + (zi[0] - p.z)*(zi[0] - p.z))))*my_sign((yi[i_73] - p.y)*(zi[0] - p.z) - (yi[0] - p.y)*(zi[i_73] - p.z));
		break;
	}

	if (fabs(sumphi) > 1.0e-7) {
		// ����� ����� ������ ��������.
		switch (iPlane_obj2) {
		case XY:
			if ((p.z >= zi[0]) && (p.z <= zi[0] + hi[0])) {
				k = ib;
				bfound = true;
			}
			break;
		case XZ:
			if ((p.y >= yi[0]) && (p.y <= yi[0] + hi[0])) {
				k = ib;
				bfound = true;
			}
			break;
		case YZ:
			if ((p.x >= xi[0]) && (p.x <= xi[0] + hi[0])) {
				k = ib;
				bfound = true;
			}
			break;
		}
	}

	return bfound;

} // in_polygon_check

bool in_polygon_check_first(TOCHKA p, integer nsizei, doublereal* &xi, doublereal* &yi, doublereal* &zi, doublereal* &hi, integer iPlane_obj2, integer &k, integer ib) {
	
	bool bfound = false;

	doublereal dpolygon_tolerance = 1e-3;

	// Polygon
	// ����� ����������� �������� ���� ��� ����������� ������ �� ������������� ��� ����������.
	// ������ ������������ ��������� �������� ������� �� ���������� ��� ������ ���� � ����������� �������������
	// ������ �� ������� ����� ������� � ���� ������ ���� � ����� �� ����� �� ������ ��������� �������������.
	// ����� ����������� ������������ ������ ����� ������� ������������ � �������� ����� ����� ��� �������� �������������,
	// ���������� ���� ������ � ����� �� ����� ��������������� ������������.

	// �������� ������ ��� ��������� �������������� � ������ �����
	// ��� ��������� ������ � ��� ������ ���������.
	doublereal xavg = 0.0, yavg = 0.0, zavg = 0.0;
	for (integer i_73 = 0; i_73 < nsizei; i_73++) {
		xavg += xi[i_73] / nsizei;
		yavg += yi[i_73] / nsizei;
		zavg += zi[i_73] / nsizei;
	}
	doublereal Sgl = 0.0;
	doublereal Sloc = 0.0;
	doublereal minx84, maxx84, miny84, maxy84, minz84, maxz84;
	doublereal epsilon_tri = 1.0e-8;

	switch (iPlane_obj2) {
	case XY:
		minx84 = 1.0e40;
		maxx84 = -1.0e40;
		miny84 = 1.0e40;
		maxy84 = -1.0e40;
		minz84 = 1.0e40;
		maxz84 = -1.0e40;
		for (integer i_73 = 0; i_73 < nsizei; i_73++) {
			// minimum
			if (xi[i_73] < minx84) minx84 = xi[i_73];
			if (yi[i_73] < miny84) miny84 = yi[i_73];
			if (zi[i_73] < minz84) minz84 = zi[i_73];
			// maximum
			if (xi[i_73] > maxx84) maxx84 = xi[i_73];
			if (yi[i_73] > maxy84) maxy84 = yi[i_73];
			if (zi[i_73] > maxz84) maxz84 = zi[i_73];
		}
		//printf("epsilon_tri=%e length=%e\n", epsilon_tri, fabs((maxx84 - minx84)*(maxy84 - miny84)));
		//system("pause");
		//epsilon_tri = dpolygon_tolerance*sqrt((maxx84 - minx84)*(maxx84 - minx84) + (maxy84 - miny84)*(maxy84 - miny84));
		epsilon_tri = dpolygon_tolerance*fabs((maxx84 - minx84)*(maxy84 - miny84));

		for (integer i_73 = 0; i_73 < nsizei - 1; i_73++) {
			Sgl = 0.0;
			Sgl = 0.5*((xi[i_73 + 1] - xi[i_73])*(yavg - yi[i_73]) - (yi[i_73 + 1] - yi[i_73])*(xavg - xi[i_73]));
			Sgl = fabs(Sgl);

			Sloc = 0.5*((p.x - xi[i_73])*(yavg - yi[i_73]) - (p.y - yi[i_73])*(xavg - xi[i_73]));
			Sgl -= fabs(Sloc);
			Sloc = 0.5*((xi[i_73 + 1] - p.x)*(yavg - p.y) - (yi[i_73 + 1] - p.y)*(xavg - p.x));
			Sgl -= fabs(Sloc);
			Sloc = 0.5*((xi[i_73 + 1] - xi[i_73])*(p.y - yi[i_73]) - (yi[i_73 + 1] - yi[i_73])*(p.x - xi[i_73]));
			Sgl -= fabs(Sloc);

			if (fabs(Sgl) < epsilon_tri) {
				if ((p.z >= zi[0]) && (p.z <= zi[0] + hi[0])) {
					k = ib;
					bfound = true;
				}
			}
		}
		Sgl = 0.0;
		Sgl = 0.5*((xi[0] - xi[nsizei - 1])*(yavg - yi[nsizei - 1]) - (yi[0] - yi[nsizei - 1])*(xavg - xi[nsizei - 1]));
		Sgl = fabs(Sgl);

		Sloc = 0.5*((p.x - xi[nsizei - 1])*(yavg - yi[nsizei - 1]) - (p.y - yi[nsizei - 1])*(xavg - xi[nsizei - 1]));
		Sgl -= fabs(Sloc);
		Sloc = 0.5*((xi[0] - p.x)*(yavg - p.y) - (yi[0] - p.y)*(xavg - p.x));
		Sgl -= fabs(Sloc);
		Sloc = 0.5*((xi[0] - xi[nsizei - 1])*(p.y - yi[nsizei - 1]) - (yi[0] - yi[nsizei - 1])*(p.x - xi[nsizei - 1]));
		Sgl -= fabs(Sloc);

		if (fabs(Sgl) < epsilon_tri) {
			if ((p.z >= zi[0]) && (p.z <= zi[0] + hi[0])) {
				k = ib;
				bfound = true;
			}
		}
		break;
	case XZ:
		minx84 = 1.0e40;
		maxx84 = -1.0e40;
		miny84 = 1.0e40;
		maxy84 = -1.0e40;
		minz84 = 1.0e40;
		maxz84 = -1.0e40;
		for (integer i_73 = 0; i_73 < nsizei; i_73++) {
			// minimum
			if (xi[i_73] < minx84) minx84 = xi[i_73];
			if (yi[i_73] < miny84) miny84 = yi[i_73];
			if (zi[i_73] < minz84) minz84 = zi[i_73];
			// maximum
			if (xi[i_73] > maxx84) maxx84 = xi[i_73];
			if (yi[i_73] > maxy84) maxy84 = yi[i_73];
			if (zi[i_73] > maxz84) maxz84 = zi[i_73];
		}
		//printf("epsilon_tri=%e length=%e\n", epsilon_tri, fabs((maxx84 - minx84)*(maxz84 - minz84)));
		//system("pause");
		//epsilon_tri = dpolygon_tolerance*sqrt((maxx84 - minx84)*(maxx84 - minx84) + (maxz84 - minz84)*(maxz84 - minz84));
		epsilon_tri = dpolygon_tolerance*fabs((maxx84 - minx84)*(maxz84 - minz84));

		for (integer i_73 = 0; i_73 < nsizei - 1; i_73++) {
			Sgl = 0.0;
			Sgl = 0.5*((xi[i_73 + 1] - xi[i_73])*(zavg - zi[i_73]) - (zi[i_73 + 1] - zi[i_73])*(xavg - xi[i_73]));
			Sgl = fabs(Sgl);

			Sloc = 0.5*((p.x - xi[i_73])*(zavg - zi[i_73]) - (p.z - zi[i_73])*(xavg - xi[i_73]));
			Sgl -= fabs(Sloc);
			Sloc = 0.5*((xi[i_73 + 1] - p.x)*(zavg - p.z) - (zi[i_73 + 1] - p.z)*(xavg - p.x));
			Sgl -= fabs(Sloc);
			Sloc = 0.5*((xi[i_73 + 1] - xi[i_73])*(p.z - zi[i_73]) - (zi[i_73 + 1] - zi[i_73])*(p.x - xi[i_73]));
			Sgl -= fabs(Sloc);

			if (fabs(Sgl) < epsilon_tri) {
				if ((p.y >= yi[0]) && (p.y <= yi[0] + hi[0])) {
					k = ib;
					bfound = true;
				}
			}
		}
		Sgl = 0.0;
		Sgl = 0.5*((xi[0] - xi[nsizei - 1])*(zavg - zi[nsizei - 1]) - (zi[0] - zi[nsizei - 1])*(xavg - xi[nsizei - 1]));
		Sgl = fabs(Sgl);

		Sloc = 0.5*((p.x - xi[nsizei - 1])*(zavg - zi[nsizei - 1]) - (p.z - zi[nsizei - 1])*(xavg - xi[nsizei - 1]));
		Sgl -= fabs(Sloc);
		Sloc = 0.5*((xi[0] - p.x)*(zavg - p.z) - (zi[0] - p.z)*(xavg - p.x));
		Sgl -= fabs(Sloc);
		Sloc = 0.5*((xi[0] - xi[nsizei - 1])*(p.z - zi[nsizei - 1]) - (zi[0] - zi[nsizei - 1])*(p.x - xi[nsizei - 1]));
		Sgl -= fabs(Sloc);

		if (fabs(Sgl) < epsilon_tri) {
			if ((p.y >= yi[0]) && (p.y <= yi[0] + hi[0])) {
				k = ib;
				bfound = true;
			}
		}
		break;
	case YZ:
		minx84 = 1.0e40;
		maxx84 = -1.0e40;
		miny84 = 1.0e40;
		maxy84 = -1.0e40;
		minz84 = 1.0e40;
		maxz84 = -1.0e40;
		for (integer i_73 = 0; i_73 < nsizei; i_73++) {
			// minimum
			if (xi[i_73] < minx84) minx84 = xi[i_73];
			if (yi[i_73] < miny84) miny84 = yi[i_73];
			if (zi[i_73] < minz84) minz84 = zi[i_73];
			// maximum
			if (xi[i_73] > maxx84) maxx84 = xi[i_73];
			if (yi[i_73] > maxy84) maxy84 = yi[i_73];
			if (zi[i_73] > maxz84) maxz84 = zi[i_73];
		}
		//printf("epsilon_tri=%e length=%e\n", epsilon_tri, fabs((maxz84 - minz84)*(maxy84 - miny84)));
		//system("pause");
		//epsilon_tri = dpolygon_tolerance*sqrt((maxz84 - minz84)*(maxz84 - minz84) + (maxy84 - miny84)*(maxy84 - miny84));
		epsilon_tri = dpolygon_tolerance*fabs((maxz84 - minz84)*(maxy84 - miny84));

		for (integer i_73 = 0; i_73 < nsizei - 1; i_73++) {
			Sgl = 0.0;
			Sgl = 0.5*((zi[i_73 + 1] - zi[i_73])*(yavg - yi[i_73]) - (yi[i_73 + 1] - yi[i_73])*(zavg - zi[i_73]));
			Sgl = fabs(Sgl);

			Sloc = 0.5*((p.z - zi[i_73])*(yavg - yi[i_73]) - (p.y - yi[i_73])*(zavg - zi[i_73]));
			Sgl -= fabs(Sloc);
			Sloc = 0.5*((zi[i_73 + 1] - p.z)*(yavg - p.y) - (yi[i_73 + 1] - p.y)*(zavg - p.z));
			Sgl -= fabs(Sloc);
			Sloc = 0.5*((zi[i_73 + 1] - zi[i_73])*(p.y - yi[i_73]) - (yi[i_73 + 1] - yi[i_73])*(p.z - zi[i_73]));
			Sgl -= fabs(Sloc);

			if (fabs(Sgl) < epsilon_tri) {
				if ((p.x >= xi[0]) && (p.x <= xi[0] + hi[0])) {
					k = ib;
					bfound = true;
				}
			}
		}
		Sgl = 0.0;
		Sgl = 0.5*((zi[0] - zi[nsizei - 1])*(yavg - yi[nsizei - 1]) - (yi[0] - yi[nsizei - 1])*(zavg - zi[nsizei - 1]));
		Sgl = fabs(Sgl);

		Sloc = 0.5*((p.z - zi[nsizei - 1])*(yavg - yi[nsizei - 1]) - (p.y - yi[nsizei - 1])*(zavg - zi[nsizei - 1]));
		Sgl -= fabs(Sloc);
		Sloc = 0.5*((zi[0] - p.z)*(yavg - p.y) - (yi[0] - p.y)*(zavg - p.z));
		Sgl -= fabs(Sloc);
		Sloc = 0.5*((zi[0] - zi[nsizei - 1])*(p.y - yi[nsizei - 1]) - (yi[0] - yi[nsizei - 1])*(p.z - zi[nsizei - 1]));
		Sgl -= fabs(Sloc);

		if (fabs(Sgl) < epsilon_tri) {
			if ((p.x >= xi[0]) && (p.x <= xi[0] + hi[0])) {
				k = ib;
				bfound = true;
			}
		}
		break;
	}

	return bfound;

} // in_polygon_check_first


bool in_polygon(TOCHKA p, integer nsizei, doublereal* &xi, doublereal* &yi, doublereal* &zi, doublereal* &hi, integer iPlane_obj2, integer &k, integer ib) {
	
	bool bfound = false;

	// �������������� ���������� ���������� ���������� �� ���� �������� ��������.
	// ������� ����������� ����������� �� ������������ (���������������), � �����
	// ����������� ��� ������������ �� ������� � ���� ����� ����������� ������������ �� ������� 
	// ������������ ����� ����� �������� ��� ������������� ������������ ����������� ������ � �������� ��������������� ������������. 
	//bfound = in_polygon_check_first(p, nsizei, xi, yi, zi, hi, iPlane_obj2, k, ib);

	// ������������������������ ������ �������� �������� ������� ������ � ���������.
	bfound = in_polygon_check(p, nsizei, xi, yi, zi, hi, iPlane_obj2, k, ib);

	return bfound;

}

// ��������� ����������� �� ����������� �����
// ����������������� ������ || hollow block.
// ���������� �������� ib ������ ������ �����
// �������� ����������� ����������� �����.
bool in_model_fluid_gap_1(TOCHKA p, integer &ib, BLOCK* b, integer lb) {
	integer i = 0, k = 0;
	bool ret = true;// �� ��������� ����������� ������
					// ���� �� ���� ������
	for (i = 0; i<lb; i++) {

		if (b[i].g.itypegeom == 0) {
			// Prism
			if ((p.x > b[i].g.xS) && (p.x < b[i].g.xE) && (p.y > b[i].g.yS) && (p.y < b[i].g.yE) && (p.z > b[i].g.zS) && (p.z < b[i].g.zE)) {
				k = i;
			}
		}
		else if (b[i].g.itypegeom == 2) {
			// �� ��������� �������������� �������� ������ � ������ ���� ����� p ���������
			// ������ ������ ������������� ������ ����������� �������.
			if ((p.x > b[i].g.xS) && (p.x < b[i].g.xE) && (p.y > b[i].g.yS) && (p.y < b[i].g.yE) && (p.z > b[i].g.zS) && (p.z < b[i].g.zE)) {
				// ���������� �������������� ����� ��������.
				in_polygon(p, b[i].g.nsizei, b[i].g.xi, b[i].g.yi, b[i].g.zi, b[i].g.hi, b[i].g.iPlane_obj2, k, i);
			}

		}
		else {
		/*if (b[i].g.itypegeom == 1) {*/

			// Cylinder
			switch (b[i].g.iPlane) {
			case XY:
				if (fabs(b[i].g.R_in_cyl) < 1.0e-40) {
					if ((p.z > b[i].g.zC) && (p.z < b[i].g.zC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.yC - p.y)*(b[i].g.yC - p.y)) < b[i].g.R_out_cyl) {
							k = i;
						}
					}
				}
				else {
					if ((p.z > b[i].g.zC) && (p.z < b[i].g.zC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.yC - p.y)*(b[i].g.yC - p.y)) < b[i].g.R_out_cyl) {
							if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.yC - p.y)*(b[i].g.yC - p.y)) > b[i].g.R_in_cyl) {
								k = i;
							}
						}
					}
				}
				break;
			case XZ:
				if (fabs(b[i].g.R_in_cyl) < 1.0e-40) {
					if ((p.y > b[i].g.yC) && (p.y < b[i].g.yC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) < b[i].g.R_out_cyl) {
							k = i;
						}
					}
				}
				else {
					if ((p.y > b[i].g.yC) && (p.y < b[i].g.yC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) < b[i].g.R_out_cyl) {
							if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) > b[i].g.R_in_cyl) {
								k = i;
							}
						}
					}
				}
				break;
			case YZ:
				if (fabs(b[i].g.R_in_cyl) < 1.0e-40) {
					if ((p.x > b[i].g.xC) && (p.x < b[i].g.xC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.yC - p.y)*(b[i].g.yC - p.y) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) < b[i].g.R_out_cyl) {
							k = i;
						}
					}
				}
				else {
					if ((p.x > b[i].g.xC) && (p.x < b[i].g.xC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.yC - p.y)*(b[i].g.yC - p.y) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) < b[i].g.R_out_cyl) {
							if (sqrt((b[i].g.yC - p.y)*(b[i].g.yC - p.y) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) > b[i].g.R_in_cyl) {
								k = i;
							}
						}
					}
				}
				break;
			}
		}
	}
	if ((b[k].itype == SOLID)) ret = false;
	ib = k;

	return ret;

} // in_model_fluid_gap_1

  // ��������� ����������� �� ����������� �����
  // ����������������� ������.
  // ���������� �������� ib ������ ������ �����
  // �������� ����������� ����������� �����.
bool in_model_fluid_gap(TOCHKA p, integer &ib, BLOCK* b, integer lb) {
	integer i = 0, k = 0;
	bool ret = true;// �� ��������� ����������� ������

	// 27_12_2017.
	// ����� ������������� �������� �����������.
	// ������� ���� ����� � ������ ����������� ������� � ����.
	// ����  ������� ������� �������������� �������� ����� � ������� �������.
	// ������� ���� ����������� ����� � ����� (������� � �������� �������� ������ lb � �����
	// � ������� ���������� �������, �� ������ ��������� ���� ��� ��� � ����� ������ � �����
	// �������� �������� ���� for � ������� break � ���������� ������������ ���� �������.
	
	
	// ���� �� ���� ������
	for (i = lb - 1; i >= 0; i--) {

		if (b[i].g.itypegeom == 0) {
			// Prism
			if ((p.x > b[i].g.xS) && (p.x < b[i].g.xE) && (p.y > b[i].g.yS) && (p.y < b[i].g.yE) && (p.z > b[i].g.zS) && (p.z < b[i].g.zE)) {
				k = i;
				// ������ ����� � ����� ��������� ��������.
				goto OUTOF_IN_MODEL_FLOW_1;
			}
		}
		if (b[i].g.itypegeom == 2) {

			if ((p.x > b[i].g.xS) && (p.x < b[i].g.xE) && (p.y > b[i].g.yS) && (p.y < b[i].g.yE) && (p.z > b[i].g.zS) && (p.z < b[i].g.zE)) {

				bool found = false;
				// ���������� �������������� ����� ��������.
				found = in_polygon(p, b[i].g.nsizei, b[i].g.xi, b[i].g.yi, b[i].g.zi, b[i].g.hi, b[i].g.iPlane_obj2, k, i);
				if (found) {
					// ������ ����� � ����� ��������� ��������.
					goto OUTOF_IN_MODEL_FLOW_1;
				}

			}

		}

		if (b[i].g.itypegeom == 1) {
			// Cylinder
			switch (b[i].g.iPlane) {
			case XY:
				if (fabs(b[i].g.R_in_cyl) < 1.0e-40) {
					if ((p.z > b[i].g.zC) && (p.z < b[i].g.zC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.yC - p.y)*(b[i].g.yC - p.y)) < b[i].g.R_out_cyl) {
							k = i;
							// ������ ����� � ����� ��������� ��������.
							goto OUTOF_IN_MODEL_FLOW_1;
						}
					}
				}
				else {
					if ((p.z > b[i].g.zC) && (p.z < b[i].g.zC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.yC - p.y)*(b[i].g.yC - p.y)) < b[i].g.R_out_cyl) {
							if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.yC - p.y)*(b[i].g.yC - p.y)) > b[i].g.R_in_cyl) {
								k = i;
								// ������ ����� � ����� ��������� ��������.
								goto OUTOF_IN_MODEL_FLOW_1;
							}
						}
					}
				}
				break;
			case XZ:
				if (fabs(b[i].g.R_in_cyl) < 1.0e-40) {
					if ((p.y > b[i].g.yC) && (p.y < b[i].g.yC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) < b[i].g.R_out_cyl) {
							k = i;
							// ������ ����� � ����� ��������� ��������.
							goto OUTOF_IN_MODEL_FLOW_1;
						}
					}
				}
				else {
					if ((p.y > b[i].g.yC) && (p.y < b[i].g.yC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) < b[i].g.R_out_cyl) {
							if (sqrt((b[i].g.xC - p.x)*(b[i].g.xC - p.x) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) > b[i].g.R_in_cyl) {
								k = i;
								// ������ ����� � ����� ��������� ��������.
								goto OUTOF_IN_MODEL_FLOW_1;
							}
						}
					}
				}
				break;
			case YZ:
				if (fabs(b[i].g.R_in_cyl) < 1.0e-40) {
					if ((p.x > b[i].g.xC) && (p.x < b[i].g.xC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.yC - p.y)*(b[i].g.yC - p.y) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) < b[i].g.R_out_cyl) {
							k = i;
							// ������ ����� � ����� ��������� ��������.
							goto OUTOF_IN_MODEL_FLOW_1;
						}
					}
				}
				else {
					if ((p.x > b[i].g.xC) && (p.x < b[i].g.xC + b[i].g.Hcyl)) {
						if (sqrt((b[i].g.yC - p.y)*(b[i].g.yC - p.y) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) < b[i].g.R_out_cyl) {
							if (sqrt((b[i].g.yC - p.y)*(b[i].g.yC - p.y) + (b[i].g.zC - p.z)*(b[i].g.zC - p.z)) > b[i].g.R_in_cyl) {
								k = i;
								// ������ ����� � ����� ��������� ��������.
								goto OUTOF_IN_MODEL_FLOW_1;
							}
						}
					}
				}
				break;
			}
		}
	}

OUTOF_IN_MODEL_FLOW_1:

	if ((b[k].itype == SOLID)) ret = false;
	ib = k;

	return ret;

} // in_model_fluid_gap

// �������� �������� ������������� ������ ��������� ����� ������������� ���
// ������� �������� ������ �������� ����� �� ���������� ���� ����� �� ��������
// ������� ��������� ��������. ��� ��������� ��� ������ ������������� �����.
// 25.04.2018 ���������.
void calc_minimum_fluid_gap3(integer &inumboundaryx, doublereal* &rxboundary,
	integer &inumboundaryy, doublereal* &ryboundary,
	integer &inumboundaryz, doublereal* &rzboundary,
	integer lb, integer ls, integer lw, BLOCK* b, SOURCE* &s, WALL* w,
	integer lu, UNION* &my_union, integer &iunion_id_p1)
{
	doublereal dm_start = 1.0 / dcount_diametr_cylinder;
	integer i;

	for (i = 1; i < lb; i++) {
		if (b[i].iunion_id == iunion_id_p1) {
			if (b[i].g.itypegeom != 2) {

				if (bcylinder_meshing && (b[i].g.itypegeom == 1)) {
					// Cylinder

					switch (b[i].g.iPlane) {
					case XY: case XZ:

						// 24.01.2018
						// ��������� ����� � ���� ��� ���� ����� ����� ���������� �������� �� ����������.
						addboundary(rxboundary, inumboundaryx, b[i].g.xC - b[i].g.R_out_cyl - dm_start*2.0*b[i].g.R_out_cyl);
						addboundary(rxboundary, inumboundaryx, b[i].g.xC + b[i].g.R_out_cyl + dm_start*2.0*b[i].g.R_out_cyl);
						break;
					}

				}
			}
		}
	}


	for (i = 1; i < lb; i++) {
		if (b[i].iunion_id == iunion_id_p1) {
			if (b[i].g.itypegeom != 2) {

				if (bcylinder_meshing && (b[i].g.itypegeom == 1)) {
					// Cylinder
					switch (b[i].g.iPlane) {
					case XY: case YZ:

						// 24.01.2018
						// ��������� ����� � ���� ��� ���� ����� ����� ���������� �������� �� ����������.
						addboundary(ryboundary, inumboundaryy, b[i].g.yC - b[i].g.R_out_cyl - dm_start*2.0*b[i].g.R_out_cyl);
						addboundary(ryboundary, inumboundaryy, b[i].g.yC + b[i].g.R_out_cyl + dm_start*2.0*b[i].g.R_out_cyl);
						break;
					}

				}
			}
		}
	}

	for (i = 1; i < lb; i++) {
		if (b[i].iunion_id == iunion_id_p1) {
			if (b[i].g.itypegeom != 2) {


				if (bcylinder_meshing && (b[i].g.itypegeom == 1)) {
					// Cylinder
					switch (b[i].g.iPlane) {
					case XZ: case YZ:

						// 24.01.2018
						// ��������� ����� � ���� ��� ���� ����� ����� ���������� �������� �� ����������.
						addboundary(rzboundary, inumboundaryz, b[i].g.zC - b[i].g.R_out_cyl - dm_start*2.0*b[i].g.R_out_cyl);
						addboundary(rzboundary, inumboundaryz, b[i].g.zC + b[i].g.R_out_cyl + dm_start*2.0*b[i].g.R_out_cyl);
						break;
					}

				}
			}
		}
	}

	if (iunion_id_p1 == 0) {
		// ��������� ������� �������.
		for (i = 0; i < lu; i++) {
			if ((fabs(my_union[i].xE - my_union[i].xS) >= fabs(my_union[i].zE - my_union[i].zS)) && (fabs(my_union[i].xE - my_union[i].xS) >= fabs(my_union[i].yE - my_union[i].yS))) {
				// ������ ����
				for (integer i_65 = 0; i_65 < 17; i_65++) {
					addboundary(rxboundary, inumboundaryx, my_union[i].xS + i_65*0.0625*fabs(my_union[i].xE - my_union[i].xS));
				}
			}
			else if ((fabs(my_union[i].xE - my_union[i].xS) <= fabs(my_union[i].zE - my_union[i].zS)) && (fabs(my_union[i].xE - my_union[i].xS) <= fabs(my_union[i].yE - my_union[i].yS))) {
				// ������ ����
				for (integer i_65 = 0; i_65 < 6; i_65++) {
					addboundary(rxboundary, inumboundaryx, my_union[i].xS + i_65*0.2*fabs(my_union[i].xE - my_union[i].xS));
				}
			}
			else {
				// ���������.
				for (integer i_65 = 0; i_65 < 11; i_65++) {
					addboundary(rxboundary, inumboundaryx, my_union[i].xS + i_65*0.1*fabs(my_union[i].xE - my_union[i].xS));
				}
			}
			addboundary(rxboundary, inumboundaryx, my_union[i].xS);
			addboundary(rxboundary, inumboundaryx, my_union[i].xE);
		}
	}

	if (iunion_id_p1 == 0) {
		// ��������� ������� �������.
		for (i = 0; i < lu; i++) {
			if ((fabs(my_union[i].yE - my_union[i].yS) >= fabs(my_union[i].zE - my_union[i].zS)) && (fabs(my_union[i].yE - my_union[i].yS) >= fabs(my_union[i].xE - my_union[i].xS))) {
				// ������ ����
				for (integer i_65 = 0; i_65 < 17; i_65++) {
					addboundary(ryboundary, inumboundaryy, my_union[i].yS + i_65*0.0625*fabs(my_union[i].yE - my_union[i].yS));
				}
			}
			else if ((fabs(my_union[i].yE - my_union[i].yS) <= fabs(my_union[i].zE - my_union[i].zS)) && (fabs(my_union[i].yE - my_union[i].yS) <= fabs(my_union[i].xE - my_union[i].xS))) {
				// ������ ����
				for (integer i_65 = 0; i_65 < 6; i_65++) {
					addboundary(ryboundary, inumboundaryy, my_union[i].yS + i_65*0.2*fabs(my_union[i].yE - my_union[i].yS));
				}
			}
			else {
				// ���������.
				for (integer i_65 = 0; i_65 < 11; i_65++) {
					addboundary(ryboundary, inumboundaryy, my_union[i].yS + i_65*0.1*fabs(my_union[i].yE - my_union[i].yS));
				}
			}
			addboundary(ryboundary, inumboundaryy, my_union[i].yS);
			addboundary(ryboundary, inumboundaryy, my_union[i].yE);
		}
	}

	if (iunion_id_p1 == 0) {
		// ��������� ������� �������.
		for (i = 0; i < lu; i++) {
			if ((fabs(my_union[i].zE - my_union[i].zS) >= fabs(my_union[i].yE - my_union[i].yS)) && (fabs(my_union[i].zE - my_union[i].zS) >= fabs(my_union[i].xE - my_union[i].xS))) {
			// ������ ����
				for (integer i_65 = 0; i_65 < 17; i_65++) {
					addboundary(rzboundary, inumboundaryz, my_union[i].zS + i_65*0.0625*fabs(my_union[i].zE - my_union[i].zS));
				}
			}
			else if ((fabs(my_union[i].zE - my_union[i].zS) <= fabs(my_union[i].yE - my_union[i].yS)) && (fabs(my_union[i].zE - my_union[i].zS) <= fabs(my_union[i].xE - my_union[i].xS))) {
				// ������ ����
				for (integer i_65 = 0; i_65 < 6; i_65++) {
					addboundary(rzboundary, inumboundaryz, my_union[i].zS + i_65*0.2*fabs(my_union[i].zE - my_union[i].zS));
				}
			}
			else {
				// ���������.
				for (integer i_65 = 0; i_65 < 11; i_65++) {
					addboundary(rzboundary, inumboundaryz, my_union[i].zS + i_65*0.1*fabs(my_union[i].zE - my_union[i].zS));
				}
			}
			addboundary(rzboundary, inumboundaryz, my_union[i].zS);
			addboundary(rzboundary, inumboundaryz, my_union[i].zE);
		}
	}

}

  // ��������� minimum fluid gap. 
  // ����� 1.
  // 12.03.2017
// 16.08.2017 Polygon
// 25.04.2018 ���������.
void calc_minimum_fluid_gap1(integer &inumboundaryx, doublereal* &rxboundary,
	integer &inumboundaryy, doublereal* &ryboundary,
	integer &inumboundaryz, doublereal* &rzboundary,
	integer lb, integer ls, integer lw, BLOCK* b, SOURCE* &s, WALL* w, 
	integer lu, UNION* &my_union, integer &iunion_id_p1)
{

	// ������� ��� ����������� ����� � ������.
	bool bdiagnostic_analysys = false;

	doublereal dm_start = 1.0/ dcount_diametr_cylinder;
	integer i;

	//***************** x **************************

	inumboundaryx = 1;
	rxboundary = new doublereal[inumboundaryx + 1]; // ����� ������ �� ��� x

	if (iunion_id_p1 == 0) {
		// �������� ������� ��������
		rxboundary[0] = b[0].g.xS; // ������ �������
		rxboundary[inumboundaryx] = b[0].g.xE; // ����� �������
	}
	else {
		// �������� ������� ������.
		rxboundary[0] = my_union[iunion_id_p1-1].xS; // ������ �������
		rxboundary[inumboundaryx] = my_union[iunion_id_p1 - 1].xE; // ����� �������
	}

										   // �����
	for (i = 1; i<lb; i++) {
		if (b[i].iunion_id == iunion_id_p1) {
			if (b[i].g.itypegeom != 2) {
				addboundary(rxboundary, inumboundaryx, b[i].g.xS);
				addboundary(rxboundary, inumboundaryx, b[i].g.xE);
				if (bcylinder_meshing && (b[i].g.itypegeom == 1)) {
					// Cylinder

					switch (b[i].g.iPlane) {
					case XY: case XZ:
						for (doublereal dm = dm_start; dm < 0.98; dm = dm + dm_start) {
							addboundary(rxboundary, inumboundaryx, b[i].g.xC - b[i].g.R_out_cyl + dm*2.0*b[i].g.R_out_cyl);
						}
						// 24.01.2018
						// ��������� ����� � ���� ��� ���� ����� ����� ���������� �������� �� ����������.
						addboundary(rxboundary, inumboundaryx, b[i].g.xC - b[i].g.R_out_cyl - 0.5*dm_start*2.0*b[i].g.R_out_cyl);
						addboundary(rxboundary, inumboundaryx, b[i].g.xC + b[i].g.R_out_cyl + 0.5*dm_start*2.0*b[i].g.R_out_cyl);
						break;
					}

				}
			}
			else {
				//Polygon
				switch (b[i].g.iPlane_obj2) {
				case XY:
					for (integer i_65 = 0; i_65 < b[i].g.nsizei; i_65++) {
						addboundary(rxboundary, inumboundaryx, b[i].g.xi[i_65]);
					}
					break;
				case XZ:
					for (integer i_65 = 0; i_65 < b[i].g.nsizei; i_65++) {
						addboundary(rxboundary, inumboundaryx, b[i].g.xi[i_65]);
					}
					break;
				case YZ:
					addboundary(rxboundary, inumboundaryx, b[i].g.xi[0]);
					addboundary(rxboundary, inumboundaryx, b[i].g.xi[0] + b[i].g.hi[0]);
					break;
				}
			}
		}
	}

	if (iunion_id_p1 == 0) {
		// ��������� ������� �������.
		for (i = 0; i < lu; i++) {
			addboundary(rxboundary, inumboundaryx, my_union[i].xS);
			addboundary(rxboundary, inumboundaryx, my_union[i].xE);
		}
	}

	// ���������
	for (i = 0; i<ls; i++) {
		if (s[i].iunion_id == iunion_id_p1) {
			addboundary(rxboundary, inumboundaryx, s[i].g.xS);
			addboundary(rxboundary, inumboundaryx, s[i].g.xE);
		}
	}

	// ������
	for (i = 0; i<lw; i++) {
		if (w[i].iunion_id == iunion_id_p1) {
			addboundary(rxboundary, inumboundaryx, w[i].g.xS);
			addboundary(rxboundary, inumboundaryx, w[i].g.xE);
		}
	}

	// �������������� �� �����������
	//BubbleEnhSort(rxboundary, inumboundaryx);
	Sort_method(rxboundary, inumboundaryx);

	if (bdiagnostic_analysys) {
		printf("diagnostic X:");
		for (int i73 = 0; i73 < inumboundaryx; i73++) {
			printf("%d %e %e\n", i73, rxboundary[i73], rxboundary[i73 + 1] - rxboundary[i73]);
		}
		printf("%d %e\n", inumboundaryx, rxboundary[inumboundaryx]);
		getchar();
	}

	//*********** y  **************************

	ryboundary = new doublereal[inumboundaryy + 1]; // ����� ������ �� ��� y
	

	if (iunion_id_p1 == 0) {
		// �������� ������� ��������
		ryboundary[0] = b[0].g.yS; // ������ ������� 
		ryboundary[inumboundaryy] = b[0].g.yE; // ����� ������� 
	}
	else {
		// �������� ������� ������.
		ryboundary[0] = my_union[iunion_id_p1 - 1].yS; // ������ �������
		ryboundary[inumboundaryy] = my_union[iunion_id_p1 - 1].yE; // ����� �������
	}

	// �����
	for (i = 1; i<lb; i++) {
		if (b[i].iunion_id == iunion_id_p1) {
			if (b[i].g.itypegeom != 2) {
				addboundary(ryboundary, inumboundaryy, b[i].g.yS);
				addboundary(ryboundary, inumboundaryy, b[i].g.yE);
				if (bcylinder_meshing && (b[i].g.itypegeom == 1)) {
					// Cylinder
					switch (b[i].g.iPlane) {
					case XY: case YZ:
						for (doublereal dm = dm_start; dm < 0.98; dm = dm + dm_start) {
							addboundary(ryboundary, inumboundaryy, b[i].g.yC - b[i].g.R_out_cyl + dm*2.0*b[i].g.R_out_cyl);
						}
						// 24.01.2018
						// ��������� ����� � ���� ��� ���� ����� ����� ���������� �������� �� ����������.
						addboundary(ryboundary, inumboundaryy, b[i].g.yC - b[i].g.R_out_cyl - 0.5*dm_start*2.0*b[i].g.R_out_cyl);
						addboundary(ryboundary, inumboundaryy, b[i].g.yC + b[i].g.R_out_cyl + 0.5*dm_start*2.0*b[i].g.R_out_cyl);
						break;
					}

				}
			}
			else {
				//Polygon
				switch (b[i].g.iPlane_obj2) {
				case XY:
					for (integer i_65 = 0; i_65 < b[i].g.nsizei; i_65++) {
						addboundary(ryboundary, inumboundaryy, b[i].g.yi[i_65]);
					}
					break;
				case XZ:
					addboundary(ryboundary, inumboundaryy, b[i].g.yi[0]);
					addboundary(ryboundary, inumboundaryy, b[i].g.yi[0] + b[i].g.hi[0]);
					break;
				case YZ:
					for (integer i_65 = 0; i_65 < b[i].g.nsizei; i_65++) {
						addboundary(ryboundary, inumboundaryy, b[i].g.yi[i_65]);
					}
					break;
				}
			}
		}
	}

	if (iunion_id_p1 == 0) {
		// ��������� ������� �������.
		for (i = 0; i < lu; i++) {
			addboundary(ryboundary, inumboundaryy, my_union[i].yS);
			addboundary(ryboundary, inumboundaryy, my_union[i].yE);
		}
	}

	// ���������
	for (i = 0; i<ls; i++) {
		if (s[i].iunion_id == iunion_id_p1) {
			addboundary(ryboundary, inumboundaryy, s[i].g.yS);
			addboundary(ryboundary, inumboundaryy, s[i].g.yE);
		}
	}

	// ������
	for (i = 0; i<lw; i++) {
		if (w[i].iunion_id == iunion_id_p1) {
			addboundary(ryboundary, inumboundaryy, w[i].g.yS);
			addboundary(ryboundary, inumboundaryy, w[i].g.yE);
		}
	}

	// �������������� �� �����������
	//BubbleEnhSort(ryboundary, inumboundaryy);
	Sort_method(ryboundary, inumboundaryy);

	if (bdiagnostic_analysys) {
		printf("diagnostic Y:");
		for (int i73 = 0; i73 < inumboundaryy; i73++) {
			printf("%d %e %e\n", i73, ryboundary[i73], ryboundary[i73 + 1] - ryboundary[i73]);
		}
		printf("%d %e\n", inumboundaryy, ryboundary[inumboundaryy]);
		getchar();
	}

	//*******************  z **************************************

	rzboundary = new doublereal[1 + inumboundaryz]; // ����� ������ �� ��� y

	
	if (iunion_id_p1 == 0) {
		// �������� ������� ��������
		rzboundary[0] = b[0].g.zS; // ������ ������� 
		rzboundary[inumboundaryz] = b[0].g.zE; // ����� ������� 
	}
	else {
		// �������� ������� ������.
		rzboundary[0] = my_union[iunion_id_p1 - 1].zS; // ������ �������
		rzboundary[inumboundaryz] = my_union[iunion_id_p1 - 1].zE; // ����� �������
	}


	// �����
	for (i = 1; i<lb; i++) {
		if (b[i].iunion_id == iunion_id_p1) {
			if (b[i].g.itypegeom != 2) {
				addboundary(rzboundary, inumboundaryz, b[i].g.zS);
				addboundary(rzboundary, inumboundaryz, b[i].g.zE);

				if (bcylinder_meshing && (b[i].g.itypegeom == 1)) {
					// Cylinder
					switch (b[i].g.iPlane) {
					case XZ: case YZ:
						for (doublereal dm = dm_start; dm < 0.98; dm = dm + dm_start) {
							addboundary(rzboundary, inumboundaryz, b[i].g.zC - b[i].g.R_out_cyl + dm*2.0*b[i].g.R_out_cyl);
						}
						// 24.01.2018
						// ��������� ����� � ���� ��� ���� ����� ����� ���������� �������� �� ����������.
						addboundary(rzboundary, inumboundaryz, b[i].g.zC - b[i].g.R_out_cyl - 0.5*dm_start*2.0*b[i].g.R_out_cyl);
						addboundary(rzboundary, inumboundaryz, b[i].g.zC + b[i].g.R_out_cyl + 0.5*dm_start*2.0*b[i].g.R_out_cyl);
						break;
					}

				}
			}
			else {
				//Polygon
				switch (b[i].g.iPlane_obj2) {
				case XY:
					addboundary(rzboundary, inumboundaryz, b[i].g.zi[0]);
					addboundary(rzboundary, inumboundaryz, b[i].g.zi[0] + b[i].g.hi[0]);
					break;
				case XZ:
					for (integer i_65 = 0; i_65 < b[i].g.nsizei; i_65++) {
						addboundary(rzboundary, inumboundaryz, b[i].g.zi[i_65]);
					}
					break;
				case YZ:
					for (integer i_65 = 0; i_65 < b[i].g.nsizei; i_65++) {
						addboundary(rzboundary, inumboundaryz, b[i].g.zi[i_65]);
					}
					break;
				}
			}
		}
	}

	if (iunion_id_p1 == 0) {
		// ��������� ������� �������.
		for (i = 0; i < lu; i++) {
			addboundary(rzboundary, inumboundaryz, my_union[i].zS);
			addboundary(rzboundary, inumboundaryz, my_union[i].zE);
		}
	}

	// ���������
	for (i = 0; i<ls; i++) {
		if (s[i].iunion_id == iunion_id_p1) {
			addboundary(rzboundary, inumboundaryz, s[i].g.zS);
			addboundary(rzboundary, inumboundaryz, s[i].g.zE);
		}
	}

	// ������
	for (i = 0; i<lw; i++) {
		if (w[i].iunion_id == iunion_id_p1) {
			addboundary(rzboundary, inumboundaryz, w[i].g.zS);
			addboundary(rzboundary, inumboundaryz, w[i].g.zE);
		}
	}

	// �������������� �� �����������
	//BubbleEnhSort(rzboundary, inumboundaryz);
	Sort_method(rzboundary, inumboundaryz);

	if (bdiagnostic_analysys) {
		printf("diagnostic Z:");
		for (int i73 = 0; i73 < inumboundaryz; i73++) {
			printf("%d %e %e\n", i73, rzboundary[i73], rzboundary[i73 + 1] - rzboundary[i73]);
		}
		printf("%d %e\n", inumboundaryz, rzboundary[inumboundaryz]);
		getchar();
	}

}


// ��������� minimum fluid gap. 
// ����� 2.
// 12.03.2017
void calc_minimum_fluid_gap2_1(integer &inumboundaryx, doublereal* &rxboundary,
	integer &inumboundaryy, doublereal* &ryboundary,
	integer &inumboundaryz, doublereal* &rzboundary,
	doublereal &minimum_fluid_gap_x,
	doublereal &minimum_fluid_gap_y,
	doublereal &minimum_fluid_gap_z,
	integer lb, integer ls, integer lw, BLOCK* b, SOURCE* &s, WALL* w)
{
	// minimum fluid gap in Ox direction

	bool bincomming = false; // true ������� ���������� ����.
	doublereal startpos = -1.0e40;
	for (integer i7 = 0; i7 < inumboundaryy; i7++) {
		for (integer i8 = 0; i8 < inumboundaryz; i8++) {
			doublereal yc = 0.5*(ryboundary[i7] + ryboundary[i7 + 1]);
			doublereal zc = 0.5*(rzboundary[i8] + rzboundary[i8 + 1]);
			for (integer i9 = 0; i9 < inumboundaryx; i9++) {
				doublereal xc = 0.5*(rxboundary[i9] + rxboundary[i9 + 1]);
				integer ib;
				TOCHKA p;
				p.x = xc;
				p.y = yc;
				p.z = zc;
				if ((bincomming) && (!in_model_fluid_gap(p, ib, b, lb))) {
					bincomming = false;
					if (fabs(rxboundary[i9] - startpos) < minimum_fluid_gap_x) {
						minimum_fluid_gap_x = fabs(rxboundary[i9] - startpos);
					}
				}
				if ((!bincomming) && (in_model_fluid_gap(p, ib, b, lb))) {
					startpos = rxboundary[i9];
					bincomming = true;
				}
			}
		}
	}


	// minimum fluid gap in Oy direction

	bincomming = false; // true ������� ���������� ����.
	startpos = -1.0e40;
	for (integer i7 = 0; i7 < inumboundaryx; i7++) {
		for (integer i8 = 0; i8 < inumboundaryz; i8++) {
			doublereal xc = 0.5*(rxboundary[i7] + rxboundary[i7 + 1]);
			doublereal zc = 0.5*(rzboundary[i8] + rzboundary[i8 + 1]);
			for (integer i9 = 0; i9 < inumboundaryy; i9++) {
				doublereal yc = 0.5*(ryboundary[i9] + ryboundary[i9 + 1]);
				integer ib;
				TOCHKA p;
				p.x = xc;
				p.y = yc;
				p.z = zc;
				if ((bincomming) && (!in_model_fluid_gap(p, ib, b, lb))) {
					bincomming = false;
					if (fabs(ryboundary[i9] - startpos) < minimum_fluid_gap_y) {
						minimum_fluid_gap_y = fabs(ryboundary[i9] - startpos);
					}
				}
				if ((!bincomming) && (in_model_fluid_gap(p, ib, b, lb))) {
					startpos = ryboundary[i9];
					bincomming = true;
				}
			}
		}
	}

	// minimum fluid gap in Oz direction

	bincomming = false; // true ������� ���������� ����.
	startpos = -1.0e40;
	for (integer i7 = 0; i7 < inumboundaryx; i7++) {
		for (integer i8 = 0; i8 < inumboundaryy; i8++) {
			doublereal xc = 0.5*(rxboundary[i7] + rxboundary[i7 + 1]);
			doublereal yc = 0.5*(ryboundary[i8] + ryboundary[i8 + 1]);
			for (integer i9 = 0; i9 < inumboundaryz; i9++) {
				doublereal zc = 0.5*(rzboundary[i9] + rzboundary[i9 + 1]);
				integer ib;
				TOCHKA p;
				p.x = xc;
				p.y = yc;
				p.z = zc;
				if ((bincomming) && (!in_model_fluid_gap(p, ib, b, lb))) {
					bincomming = false;
					if (fabs(rzboundary[i9] - startpos) < minimum_fluid_gap_z) {
						minimum_fluid_gap_z = fabs(rzboundary[i9] - startpos);
					}
				}
				if ((!bincomming) && (in_model_fluid_gap(p, ib, b, lb))) {
					startpos = rzboundary[i9];
					bincomming = true;
				}
			}
		}
	}
} //calc_minimum_fluid_gap2_1





// ��������� minimum fluid gap. 
// ����� 2.
// 12.03.2017
// 25.04.2018 ����� ��� ������ � �����������
void calc_minimum_fluid_gap2(integer &inumboundaryx, doublereal* &rxboundary,
	integer &inumboundaryy, doublereal* &ryboundary,
	integer &inumboundaryz, doublereal* &rzboundary,
	doublereal &minimum_fluid_gap_x,
	doublereal &minimum_fluid_gap_y,
	doublereal &minimum_fluid_gap_z,
	integer lb, integer ls, integer lw, BLOCK* b, SOURCE* &s, WALL* w, 
	integer lu, UNION* &my_union, integer &iunion_id_p1)
{
	
	
	// preprocessing
	integer* ib_marker = new integer[inumboundaryx*inumboundaryy*inumboundaryz];
	/*
	for (integer i9 = 0; i9 < inumboundaryx; i9++) {
		for (integer i7 = 0; i7 < inumboundaryy; i7++) {
			for (integer i8 = 0; i8 < inumboundaryz; i8++) {
				doublereal yc = 0.5*(ryboundary[i7] + ryboundary[i7 + 1]);
				doublereal zc = 0.5*(rzboundary[i8] + rzboundary[i8 + 1]);
				doublereal xc = 0.5*(rxboundary[i9] + rxboundary[i9 + 1]);
				integer ib;
				TOCHKA p;
				p.x = xc;
				p.y = yc;
				p.z = zc;
				in_model_fluid_gap(p, ib, b, lb);
				// x + y*dimx+z*dimx*dimy
				ib_marker[i9+ inumboundaryx*i7+ inumboundaryx*inumboundaryy*i8] = ib;
			}
		}
	}
	*/

	// ������� ������������� �� �������� �����.
	Block_indexes* block_indexes = new Block_indexes[lb];
	if (block_indexes == NULL) {
		printf("error in allocation memory for block_indexes in enumerate_volume_improved.\n");
		system("pause");
		exit(1);
	}
	integer i, j, k;

	// 08.04.2018
	for (i = 0; i < lb; i++) {
		// �������������, �� ������ ���� ����� �� ����� ����������.
		block_indexes[i].iL = -1;
		block_indexes[i].iR = -2;
		block_indexes[i].jL = -1;
		block_indexes[i].jR = -2;
		block_indexes[i].kL = -1;
		block_indexes[i].kR = -2;
	}

	/*
	for (i = 0; i < lb; i++) {
		doublereal x4 = b[i].g.xS;
		doublereal distmax;
		distmax = 1.0e40;
		for (j = 0; j <= inumboundaryx; j++) {
			if (fabs(rxboundary[j] - x4) < distmax) {
				block_indexes[i].iL = j;
				distmax = fabs(rxboundary[j] - x4);
			}
		}
		x4 = b[i].g.xE;
		distmax = 1.0e40;
		for (j = 0; j <= inumboundaryx; j++) {
			if (fabs(rxboundary[j] - x4) < distmax) {
				block_indexes[i].iR = j;
				distmax = fabs(rxboundary[j] - x4);
			}
		}
		x4 = b[i].g.yS;
		distmax = 1.0e40;
		for (j = 0; j <= inumboundaryy; j++) {
			if (fabs(ryboundary[j] - x4) < distmax) {
				block_indexes[i].jL = j;
				distmax = fabs(ryboundary[j] - x4);
			}
		}
		x4 = b[i].g.yE;
		distmax = 1.0e40;
		for (j = 0; j <= inumboundaryy; j++) {
			if (fabs(ryboundary[j] - x4) < distmax) {
				block_indexes[i].jR = j;
				distmax = fabs(ryboundary[j] - x4);
			}
		}
		x4 = b[i].g.zS;
		distmax = 1.0e40;
		for (j = 0; j <= inumboundaryz; j++) {
			if (fabs(rzboundary[j] - x4) < distmax) {
				block_indexes[i].kL = j;
				distmax = fabs(rzboundary[j] - x4);
			}
		}
		x4 = b[i].g.zE;
		distmax = 1.0e40;
		for (j = 0; j <= inumboundaryz; j++) {
			if (fabs(rzboundary[j] - x4) < distmax) {
				block_indexes[i].kR = j;
				distmax = fabs(rzboundary[j] - x4);
			}
		}
	}
	*/

	for (i = 0; i < lb; i++) {
		//if (b[i].iunion_id == iunion_id_p1) {
		{	
		    doublereal x4 = b[i].g.xS;
			for (j = 0; j <= inumboundaryx; j++) {
				if (fabs(rxboundary[j] - x4) < 1.0e-40) {
					block_indexes[i].iL = j;
					break;
				}
			}
			x4 = b[i].g.xE;
			for (j = 0; j <= inumboundaryx; j++) {
				if (fabs(rxboundary[j] - x4) < 1.0e-40) {
					block_indexes[i].iR = j;
					break;
				}
			}
			x4 = b[i].g.yS;
			for (j = 0; j <= inumboundaryy; j++) {
				if (fabs(ryboundary[j] - x4) < 1.0e-40) {
					block_indexes[i].jL = j;
					break;
				}
			}
			x4 = b[i].g.yE;
			for (j = 0; j <= inumboundaryy; j++) {
				if (fabs(ryboundary[j] - x4) < 1.0e-40) {
					block_indexes[i].jR = j;
					break;
				}
			}
			x4 = b[i].g.zS;
			for (j = 0; j <= inumboundaryz; j++) {
				if (fabs(rzboundary[j] - x4) < 1.0e-40) {
					block_indexes[i].kL = j;
					break;
				}
			}
			x4 = b[i].g.zE;
			for (j = 0; j <= inumboundaryz; j++) {
				if (fabs(rzboundary[j] - x4) < 1.0e-40) {
					block_indexes[i].kR = j;
					break;
				}
			}
		}
	}

	
	integer ismarker = 0;
	if (iunion_id_p1 > 0) {
		for (integer m7 = 0; m7 < lb; m7++) {
			if (block_indexes[m7].iL < 0) {
				block_indexes[m7].iL = 0;
				ismarker++;
			}
			if (block_indexes[m7].jL < 0) {
				block_indexes[m7].jL = 0;
				ismarker++;
			}
			if (block_indexes[m7].kL < 0) {
				block_indexes[m7].kL = 0;
				ismarker++;
			}
			if (block_indexes[m7].iR < 0) {
				block_indexes[m7].iR = inumboundaryx;
				ismarker++;
			}
			if (block_indexes[m7].jR < 0) {
				block_indexes[m7].jR = inumboundaryy;
				ismarker++;
			}
			if (block_indexes[m7].kR < 0) {
				block_indexes[m7].kR = inumboundaryz;
				ismarker++;
			}
		}
	}
	//printf("ismarker =%d\n",ismarker);
	//getchar();
	

	// ���������� �������� ����������� ����������� � � ����� ��� �������� � �������������
	// ���������� ��������������.
	integer m7;
	integer ib_stub = -1;
	// �� ������ ����� ������� �� ������� Hollow block, ����� ����� ������ �������.
	ib_stub = 0;
	doublereal vol_stub = -1.0;
	for (i = 0; i < lb; i++) {
		//if (b[i].iunion_id == iunion_id_p1) {
		{
			if (b[i].itype == HOLLOW) {
				if (fabs(b[i].g.xE - b[i].g.xS)*fabs(b[i].g.yE - b[i].g.yS)*fabs(b[i].g.zE - b[i].g.zS) > vol_stub) {
					ib_stub = i;
					vol_stub = fabs(b[i].g.xE - b[i].g.xS)*fabs(b[i].g.yE - b[i].g.yS)*fabs(b[i].g.zE - b[i].g.zS);
				}
			}
		}
	}


#pragma omp parallel for
	for (integer i1 = 0; i1 < inumboundaryx; i1++) for (integer j1 = 0; j1 < inumboundaryy; j1++) for (integer k1 = 0; k1 < inumboundaryz; k1++) {
		ib_marker[i1 + inumboundaryx*j1 + inumboundaryx*inumboundaryy*k1] = ib_stub; //-1
	}
	for (m7 = 0; m7 < lb; m7++) {
		if (b[m7].iunion_id == iunion_id_p1) {

#pragma omp parallel for
			for (integer i1 = block_indexes[m7].iL; i1 < block_indexes[m7].iR; i1++) for (integer j1 = block_indexes[m7].jL; j1 < block_indexes[m7].jR; j1++) for (integer k1 = block_indexes[m7].kL; k1 < block_indexes[m7].kR; k1++) {
				ib_marker[i1 + inumboundaryx*j1 + inumboundaryx*inumboundaryy*k1] = m7;
			}
		}
	}

	delete[] block_indexes;
	//printf("identifire blocks number 80 procent.\n");


	// minimum fluid gap in Ox direction

	bool bincomming = false; // true ������� ���������� ����.
	doublereal startpos = -1.0e40;
	for (integer i7 = 0; i7 < inumboundaryy; i7++) {
		for (integer i8 = 0; i8 < inumboundaryz; i8++) {
			//doublereal yc = 0.5*(ryboundary[i7] + ryboundary[i7 + 1]);
			//doublereal zc = 0.5*(rzboundary[i8] + rzboundary[i8 + 1]);
			for (integer i9 = 0; i9 < inumboundaryx; i9++) {
				//doublereal xc = 0.5*(rxboundary[i9] + rxboundary[i9 + 1]);
				//integer ib;
				//TOCHKA p;
				//p.x = xc;
				//p.y = yc;
				//p.z = zc;
				if ((bincomming) && ((b[ib_marker[i9 + inumboundaryx*i7 + inumboundaryx*inumboundaryy*i8]].itype == SOLID))) {
					bincomming = false;
					if (fabs(rxboundary[i9] - startpos) < minimum_fluid_gap_x) {
						minimum_fluid_gap_x = fabs(rxboundary[i9] - startpos);
					}
				}
				if ((!bincomming) && (!(b[ib_marker[i9 + inumboundaryx*i7 + inumboundaryx*inumboundaryy*i8]].itype == SOLID))) {
					startpos = rxboundary[i9];
					bincomming = true;
				}
			}
		}
	}


	// minimum fluid gap in Oy direction

	bincomming = false; // true ������� ���������� ����.
	startpos = -1.0e40;
	for (integer i7 = 0; i7 < inumboundaryx; i7++) {
		for (integer i8 = 0; i8 < inumboundaryz; i8++) {
			//doublereal xc = 0.5*(rxboundary[i7] + rxboundary[i7 + 1]);
			//doublereal zc = 0.5*(rzboundary[i8] + rzboundary[i8 + 1]);
			for (integer i9 = 0; i9 < inumboundaryy; i9++) {
				//doublereal yc = 0.5*(ryboundary[i9] + ryboundary[i9 + 1]);
				//integer ib;
				//TOCHKA p;
				//p.x = xc;
				//p.y = yc;
				//p.z = zc;
				if ((bincomming) && ((b[ib_marker[i7 + inumboundaryx*i9 + inumboundaryx*inumboundaryy*i8]].itype == SOLID))) {
					bincomming = false;
					if (fabs(ryboundary[i9] - startpos) < minimum_fluid_gap_y) {
						minimum_fluid_gap_y = fabs(ryboundary[i9] - startpos);
					}
				}
				if ((!bincomming) && (!(b[ib_marker[i7 + inumboundaryx*i9 + inumboundaryx*inumboundaryy*i8]].itype == SOLID))) {
					startpos = ryboundary[i9];
					bincomming = true;
				}
			}
		}
	}

	// minimum fluid gap in Oz direction

	bincomming = false; // true ������� ���������� ����.
	startpos = -1.0e40;
	for (integer i7 = 0; i7 < inumboundaryx; i7++) {
		for (integer i8 = 0; i8 < inumboundaryy; i8++) {
			//doublereal xc = 0.5*(rxboundary[i7] + rxboundary[i7 + 1]);
			//doublereal yc = 0.5*(ryboundary[i8] + ryboundary[i8 + 1]);
			for (integer i9 = 0; i9 < inumboundaryz; i9++) {
				//doublereal zc = 0.5*(rzboundary[i9] + rzboundary[i9 + 1]);
				//integer ib;
				//TOCHKA p;
				//p.x = xc;
				//p.y = yc;
				//p.z = zc;
				if ((bincomming) && (b[ib_marker[i7 + inumboundaryx*i8 + inumboundaryx*inumboundaryy*i9]].itype == SOLID)) {
					bincomming = false;
					if (fabs(rzboundary[i9] - startpos) < minimum_fluid_gap_z) {
						minimum_fluid_gap_z = fabs(rzboundary[i9] - startpos);
					}
				}
				if ((!bincomming) && (!(b[ib_marker[i7 + inumboundaryx*i8 + inumboundaryx*inumboundaryy*i9]].itype == SOLID))) {
					startpos = rzboundary[i9];
					bincomming = true;
				}
			}
		}
	}

	delete[] ib_marker;
}



// 12.03.2017
// ���������� snap to
// ������������ ����������� �������� ������ �� 33%.
// 25.04.2018 ������ � �����������
void snap_to_moving(bool* &source_indexpopadaniqnagranYZ,
	bool* &source_indexpopadaniqnagranXY,
	bool* &source_indexpopadaniqnagranXZ,
	doublereal* &rxboundary, doublereal* &ryboundary, doublereal* &rzboundary,
	integer &inumboundaryx, integer &inumboundaryy, integer &inumboundaryz,
	doublereal &minimum_fluid_gap_x, doublereal &minimum_fluid_gap_y, doublereal &minimum_fluid_gap_z,
	integer lb, integer ls, integer lw, BLOCK* &b, SOURCE* &s, WALL* &w,
	integer lu, UNION* &my_union, integer &iunion_id_p1)
{

	doublereal dm_start = 1.0/ dcount_diametr_cylinder;

	bool bsnap_TO = bsnap_TO_global; // snap to grid (��������� �� ����� � ������� �������).
	doublereal snap_to_multiplyer = 0.3;// ��������� ����� � ��������� (0..1) � ���������� ����� ����� ������������.

	bool brepeat = true;

	integer i;

RESTARTX_SNAPTO:

	if (source_indexpopadaniqnagranYZ != NULL) {
		delete[] source_indexpopadaniqnagranYZ;
		source_indexpopadaniqnagranYZ = NULL;
	}
	if (rxboundary != NULL) {
		delete[] rxboundary;
		rxboundary = NULL;
	}

	inumboundaryx = 1;
	rxboundary = new doublereal[inumboundaryx + 1]; // ����� ������ �� ��� x

	

	if (iunion_id_p1 == 0) {
		// �������� ������� ��������
		rxboundary[0] = b[0].g.xS; // ������ �������
		rxboundary[inumboundaryx] = b[0].g.xE; // ����� �������
	}
	else {
		// �������� ������� ������.
		rxboundary[0] = my_union[iunion_id_p1 - 1].xS; // ������ �������
		rxboundary[inumboundaryx] = my_union[iunion_id_p1 - 1].xE; // ����� �������
	}

										   // �����
	for (i = 1; i<lb; i++) {
		if (b[i].iunion_id == iunion_id_p1) {
			addboundary(rxboundary, inumboundaryx, b[i].g.xS);
			addboundary(rxboundary, inumboundaryx, b[i].g.xE);

			if (bcylinder_meshing && (b[i].g.itypegeom == 1)) {
				// Cylinder

				switch (b[i].g.iPlane) {
				case XY: case XZ:
					for (doublereal dm = dm_start; dm < 0.98; dm = dm + dm_start) {
						//addboundary(rxboundary, inumboundaryx, dm*(b[i].g.xS+ b[i].g.xE));
						addboundary(rxboundary, inumboundaryx, (b[i].g.xC - b[i].g.R_out_cyl + dm*2.0*b[i].g.R_out_cyl));
					}
					break;
				}
			}
		}
	}

	// ���� �������� ����� �� �������� �� ������� ���� ������,
	// ���� ����� �� ��� ����� � ��� ������� ��������.
	source_indexpopadaniqnagranYZ = new bool[ls];
	for (i = 0; i < ls; i++) {
		source_indexpopadaniqnagranYZ[i] = false;
	}
	for (i = 0; i < ls; i++) {
		if (s[i].iunion_id == iunion_id_p1) {
			if (s[i].iPlane == YZ) {
				for (integer i1 = 0; i1 <= inumboundaryx; i1++) {
					s[i].g.xS = s[i].g.xE;
					if (fabs(s[i].g.xS - rxboundary[i1]) < 1.0e-40) {
						source_indexpopadaniqnagranYZ[i] = true;
					}
				}
			}
		}
	}

	if (iunion_id_p1 == 0) {
		// ��������� ������� �������.
		for (i = 0; i < lu; i++) {
			addboundary(rxboundary, inumboundaryx, my_union[i].xS);
			addboundary(rxboundary, inumboundaryx, my_union[i].xE);
		}
	}

	// ���������
	for (i = 0; i<ls; i++) {
		if (s[i].iunion_id == iunion_id_p1) {
			addboundary(rxboundary, inumboundaryx, s[i].g.xS);
			addboundary(rxboundary, inumboundaryx, s[i].g.xE);
		}
	}

	// ������
	for (i = 0; i<lw; i++) {
		if (w[i].iunion_id == iunion_id_p1) {
			addboundary(rxboundary, inumboundaryx, w[i].g.xS);
			addboundary(rxboundary, inumboundaryx, w[i].g.xE);
		}
	}

	// �������������� �� �����������
	//BubbleEnhSort(rxboundary, inumboundaryx);
	Sort_method(rxboundary, inumboundaryx);

	// snap to grid
	if (bsnap_TO) {
		doublereal rmindisteps = 1.0e30;

		for (i = 0; i <inumboundaryx; i++) {
			if (fabs(rxboundary[i + 1] - rxboundary[i]) < rmindisteps) {
				rmindisteps = fabs(rxboundary[i + 1] - rxboundary[i]);
			}
		}


		rmindisteps *= 0.25;

		doublereal rmindist = 1.0e30;

		for (i = 0; i < lb; i++) {
			if (b[i].iunion_id == iunion_id_p1) {
				if (bcylinder_meshing && (b[i].g.itypegeom == 1)) {
					// Cylinder

					switch (b[i].g.iPlane) {
					case XY: case XZ:
						if (fabs(2.0*b[i].g.R_out_cyl) / dcount_diametr_cylinder < rmindist) {
							rmindist = fabs(2.0*b[i].g.R_out_cyl) / dcount_diametr_cylinder;
						}
						break;
					}
				}
				else if (fabs(b[i].g.xE - b[i].g.xS) < rmindist) {
					rmindist = fabs(b[i].g.xE - b[i].g.xS);
				}
			}
		}
		for (i = 0; i < ls; i++) {
			if (s[i].iunion_id == iunion_id_p1) {
				if ((s[i].iPlane == XY) || (s[i].iPlane == XZ)) {
					if (fabs(s[i].g.xE - s[i].g.xS) < rmindist) {
						rmindist = fabs(s[i].g.xE - s[i].g.xS);
					}
				}
			}
		}
		for (i = 0; i < lw; i++) {
			if (w[i].iunion_id == iunion_id_p1) {
				if ((w[i].iPlane == XY) || (w[i].iPlane == XZ)) {
					if (fabs(w[i].g.xE - w[i].g.xS) < rmindist) {
						rmindist = fabs(w[i].g.xE - w[i].g.xS);
					}
				}
			}
		}
		if (iunion_id_p1 == 0) {
			// ��������� ������� �������.
			for (i = 0; i < lu; i++) {
				if (fabs(my_union[i].xE - my_union[i].xS) < rmindist) {
					rmindist = fabs(my_union[i].xE - my_union[i].xS);
				}
			}
		}

		// 28.02.2017.
		// ����� ����� ��������� minimum fluid gap
		// ����� � ��� ��� ������ ������� ����� ���� ����� ���� ������ ���� ����� minimum fluid gap*snap_to_multiplyer.
		// ������ minimum fluid gap ������������ �� ������ �� FLUID �������. ���� �� ����� ���� � ������� ������ �������������
		// �� minimum fluid gap ������ ���� �������� ����� �� hollow ������.
		// ������������� �������� � ���� ��� Snap to �� �������� ��������� �� ���� �������.

		if (minimum_fluid_gap_x < rmindist) rmindist = minimum_fluid_gap_x;


		rmindist *= snap_to_multiplyer;
		doublereal movetopos;
		doublereal changepos;
		bool bmove = false;
		bool bmove2 = false;
		for (i = 0; i < inumboundaryx; i++) {
			if (bmove) bmove2 = true;
			bmove = false;
			if (fabs(rxboundary[i + 1] - rxboundary[i]) < rmindist) {
				if (i > 0) {
					if (i + 2 <= inumboundaryx) {

						if (fabs(rxboundary[i - 1] - rxboundary[i]) <= fabs(rxboundary[i + 2] - rxboundary[i + 1])) {
							if (fabs(rxboundary[i - 1] - rxboundary[i]) < fabs(rxboundary[i + 1] - rxboundary[i])) {
								// ������ ���� ������ ����� �� �����.
								movetopos = rxboundary[i - 1];
								changepos = rxboundary[i];
							}
							else {
								// �������� ��������.
								movetopos = rxboundary[i + 1];
								changepos = rxboundary[i];
							}
							bmove = true;
						}
						else {
							if (fabs(rxboundary[i + 2] - rxboundary[i + 1]) < fabs(rxboundary[i + 1] - rxboundary[i])) {
								// ������ ���� ������ ����� �� �����.
								movetopos = rxboundary[i + 2];
								changepos = rxboundary[i + 1];
							}
							else {
								// �������� ��������.
								movetopos = rxboundary[i];
								changepos = rxboundary[i + 1];
							}

							bmove = true;
						}

					}
					else {
						movetopos = rxboundary[inumboundaryx];
						changepos = rxboundary[inumboundaryx - 1];
						bmove = true;
					}
				}
				else {
					movetopos = rxboundary[0];
					changepos = rxboundary[1];
					bmove = true;
				}
			}
			if (bmove) {
				printf(" X changepos=%e to movetopos=%e \n", changepos, movetopos);
				for (integer i_2 = 0; i_2 < lb; i_2++) {
					if (b[i_2].iunion_id == iunion_id_p1) {
						if (fabs(b[i_2].g.xE - changepos) < rmindisteps) {
							b[i_2].g.xE = movetopos;
						}
						if (fabs(b[i_2].g.xS - changepos) < rmindisteps) {
							b[i_2].g.xS = movetopos;
						}
					}
				}
				for (integer i_2 = 0; i_2 < lw; i_2++) {
					if (w[i_2].iunion_id == iunion_id_p1) {
						if (fabs(w[i_2].g.xE - changepos) < rmindisteps) {
							w[i_2].g.xE = movetopos;
						}
						if (fabs(w[i_2].g.xS - changepos) < rmindisteps) {
							w[i_2].g.xS = movetopos;
						}
					}
				}
				for (integer i_2 = 0; i_2 < ls; i_2++) {
					if (s[i_2].iunion_id == iunion_id_p1) {
						if (fabs(s[i_2].g.xE - changepos) < rmindisteps) {
							s[i_2].g.xE = movetopos;
						}
						if (fabs(s[i_2].g.xS - changepos) < rmindisteps) {
							s[i_2].g.xS = movetopos;
						}
					}
				}
			}
		}

		if (bmove2) {
			if (brepeat) {
				brepeat = false;
				goto RESTARTX_SNAPTO;
			}

		}

	}


	brepeat = true;

	// �� ��� Oy
	inumboundaryy = 1;


RESTARTY_SNAPTO:

	if (source_indexpopadaniqnagranXZ != NULL) {
		delete[] source_indexpopadaniqnagranXZ;
		source_indexpopadaniqnagranXZ = NULL;
	}
	if (ryboundary != NULL) {
		delete[] ryboundary;
		ryboundary = NULL;
	}


	inumboundaryy = 1;
	ryboundary = new doublereal[inumboundaryy + 1]; // ����� ������ �� ��� y
	

	if (iunion_id_p1 == 0) {
		// �������� ������� ��������
		ryboundary[0] = b[0].g.yS; // ������ ������� 
		ryboundary[inumboundaryy] = b[0].g.yE; // ����� ������� 
	}
	else {
		// �������� ������� ������.
		ryboundary[0] = my_union[iunion_id_p1 - 1].yS; // ������ �������
		ryboundary[inumboundaryy] = my_union[iunion_id_p1 - 1].yE; // ����� �������
	}

										   // �����
	for (i = 1; i<lb; i++) {
		if (b[i].iunion_id == iunion_id_p1) {
			addboundary(ryboundary, inumboundaryy, b[i].g.yS);
			addboundary(ryboundary, inumboundaryy, b[i].g.yE);
			if (bcylinder_meshing && (b[i].g.itypegeom == 1)) {
				// Cylinder
				switch (b[i].g.iPlane) {
				case XY: case YZ:
					for (doublereal dm = dm_start; dm < 0.98; dm = dm + dm_start) {
						addboundary(ryboundary, inumboundaryy, (b[i].g.yC - b[i].g.R_out_cyl + dm*2.0*b[i].g.R_out_cyl));
					}
					break;
				}
			}
		}
	}

	// ���� �������� ����� �� �������� �� ������� ���� ������,
	// ���� ����� �� ��� ����� � ��� ������� ��������.
	source_indexpopadaniqnagranXZ = new bool[ls];
	for (i = 0; i < ls; i++) {
		source_indexpopadaniqnagranXZ[i] = false;
	}
	for (i = 0; i < ls; i++) {
		if (s[i].iunion_id == iunion_id_p1) {
			if (s[i].iPlane == XZ) {
				for (integer i1 = 0; i1 <= inumboundaryy; i1++) {
					s[i].g.yS = s[i].g.yE;
					if (fabs(s[i].g.yS - ryboundary[i1]) < 1.0e-40) {
						source_indexpopadaniqnagranXZ[i] = true;
					}
				}
			}
		}
	}

	if (iunion_id_p1 == 0) {
		// ��������� ������� �������.
		for (i = 0; i < lu; i++) {
			addboundary(ryboundary, inumboundaryy, my_union[i].yS);
			addboundary(ryboundary, inumboundaryy, my_union[i].yE);
		}
	}

	// ���������
	for (i = 0; i<ls; i++) {
		if (s[i].iunion_id == iunion_id_p1) {
			addboundary(ryboundary, inumboundaryy, s[i].g.yS);
			addboundary(ryboundary, inumboundaryy, s[i].g.yE);
		}
	}

	// ������
	for (i = 0; i<lw; i++) {
		if (w[i].iunion_id == iunion_id_p1) {
			addboundary(ryboundary, inumboundaryy, w[i].g.yS);
			addboundary(ryboundary, inumboundaryy, w[i].g.yE);
		}
	}

	// �������������� �� �����������
	//BubbleEnhSort(ryboundary, inumboundaryy);
	Sort_method(ryboundary, inumboundaryy);

	// snap to grid
	if (bsnap_TO) {
		doublereal rmindist = 1.0e30;

		for (i = 0; i < lb; i++) {
			if (b[i].iunion_id == iunion_id_p1) {
				if (bcylinder_meshing && (b[i].g.itypegeom == 1)) {
					// Cylinder
					switch (b[i].g.iPlane) {
					case XY: case YZ:
						if (fabs(2.0*b[i].g.R_out_cyl) / dcount_diametr_cylinder < rmindist) {
							rmindist = fabs(2.0*b[i].g.R_out_cyl) / dcount_diametr_cylinder;
						}
						break;
					}
				}
				else if (fabs(b[i].g.yE - b[i].g.yS) < rmindist) {
					rmindist = fabs(b[i].g.yE - b[i].g.yS);
				}
			}
		}

		for (i = 0; i < ls; i++) {
			if (s[i].iunion_id == iunion_id_p1) {
				if ((s[i].iPlane == XY) || (s[i].iPlane == YZ)) {
					if (fabs(s[i].g.yE - s[i].g.yS) < rmindist) {
						rmindist = fabs(s[i].g.yE - s[i].g.yS);
					}
				}
			}
		}
		for (i = 0; i < lw; i++) {
			if (w[i].iunion_id == iunion_id_p1) {
				if ((w[i].iPlane == XY) || (w[i].iPlane == YZ)) {
					if (fabs(w[i].g.yE - w[i].g.yS) < rmindist) {
						rmindist = fabs(w[i].g.yE - w[i].g.yS);
					}
				}
			}
		}
		if (iunion_id_p1 == 0) {
			// ��������� ������� �������.
			for (i = 0; i < lu; i++) {
				if (fabs(my_union[i].yE - my_union[i].yS) < rmindist) {
					rmindist = fabs(my_union[i].yE - my_union[i].yS);
				}
			}
		}

		if (minimum_fluid_gap_y < rmindist) rmindist = minimum_fluid_gap_y;

		rmindist *= snap_to_multiplyer;
		doublereal movetopos;
		doublereal changepos;
		bool bmove = false;
		bool bmove2 = false;
		for (i = 0; i < inumboundaryy; i++) {
			if (bmove) bmove2 = true;
			bmove = false;
			if (fabs(ryboundary[i + 1] - ryboundary[i]) < rmindist) {
				if (i > 0) {
					if (i + 2 <= inumboundaryy) {
						if (fabs(ryboundary[i - 1] - ryboundary[i]) <= fabs(ryboundary[i + 2] - ryboundary[i + 1])) {
							if (fabs(ryboundary[i - 1] - ryboundary[i]) < fabs(ryboundary[i + 1] - ryboundary[i])) {
								// ������ ���� ������ ����� �� �����.
								movetopos = ryboundary[i - 1];
								changepos = ryboundary[i];
							}
							else {
								// �������� ��������.
								movetopos = ryboundary[i + 1];
								changepos = ryboundary[i];
							}
							bmove = true;
						}
						else {
							if (fabs(ryboundary[i + 2] - ryboundary[i + 1]) < fabs(ryboundary[i + 1] - ryboundary[i])) {
								// ������ ���� ������ ����� �� �����.
								movetopos = ryboundary[i + 2];
								changepos = ryboundary[i + 1];
							}
							else {
								// �������� ��������.
								movetopos = ryboundary[i];
								changepos = ryboundary[i + 1];
							}

							bmove = true;
						}



					}
					else {
						movetopos = ryboundary[inumboundaryy];
						changepos = ryboundary[inumboundaryy - 1];
						bmove = true;
					}
				}
				else {
					movetopos = ryboundary[0];
					changepos = ryboundary[1];
					bmove = true;
				}
			}
			if (bmove) {
				printf("Y changepos=%e to movetopos=%e \n", changepos, movetopos);
				for (integer i_2 = 0; i_2 < lb; i_2++) {
					if (b[i_2].iunion_id == iunion_id_p1) {
						if (fabs(b[i_2].g.yE - changepos) < 1.0e-33) {
							b[i_2].g.yE = movetopos;
						}
						if (fabs(b[i_2].g.yS - changepos) < 1.0e-33) {
							b[i_2].g.yS = movetopos;
						}
					}
				}
				for (integer i_2 = 0; i_2 < lw; i_2++) {
					if (w[i_2].iunion_id == iunion_id_p1) {
						if (fabs(w[i_2].g.yE - changepos) < 1.0e-33) {
							w[i_2].g.yE = movetopos;
						}
						if (fabs(w[i_2].g.yS - changepos) < 1.0e-33) {
							w[i_2].g.yS = movetopos;
						}
					}
				}
				for (integer i_2 = 0; i_2 < ls; i_2++) {
					if (s[i_2].iunion_id == iunion_id_p1) {
						if (fabs(s[i_2].g.yE - changepos) < 1.0e-33) {
							s[i_2].g.yE = movetopos;
						}
						if (fabs(s[i_2].g.yS - changepos) < 1.0e-33) {
							s[i_2].g.yS = movetopos;
						}
					}
				}
			}
		}

		if (bmove2) {
			if (brepeat) {
				brepeat = false;
				goto RESTARTY_SNAPTO;
			}
		}

	}


	// �� ��� Oz

	brepeat = true;

	inumboundaryz = 1;


RESTARTZ_SNAPTO:

	if (source_indexpopadaniqnagranXY != NULL) {
		delete[] source_indexpopadaniqnagranXY;
		source_indexpopadaniqnagranXY = NULL;
	}
	if (rzboundary != NULL) {
		delete[] rzboundary;
		rzboundary = NULL;
	}


	inumboundaryz = 1;
	rzboundary = new doublereal[1 + inumboundaryz]; // ����� ������ �� ��� y

	
	if (iunion_id_p1 == 0) {
		// �������� ������� ��������
		rzboundary[0] = b[0].g.zS; // ������ ������� 
		rzboundary[inumboundaryz] = b[0].g.zE; // ����� ������� 
	}
	else {
		// �������� ������� ������.
		rzboundary[0] = my_union[iunion_id_p1 - 1].zS; // ������ �������
		rzboundary[inumboundaryz] = my_union[iunion_id_p1 - 1].zE; // ����� �������
	}


										   // �����
	for (i = 1; i<lb; i++) {
		if (b[i].iunion_id == iunion_id_p1) {
			addboundary(rzboundary, inumboundaryz, b[i].g.zS);
			addboundary(rzboundary, inumboundaryz, b[i].g.zE);
			if (bcylinder_meshing && (b[i].g.itypegeom == 1)) {
				// Cylinder
				switch (b[i].g.iPlane) {
				case XZ: case YZ:
					for (doublereal dm = dm_start; dm < 0.98; dm = dm + dm_start) {
						addboundary(rzboundary, inumboundaryz, (b[i].g.zC - b[i].g.R_out_cyl + dm*2.0*b[i].g.R_out_cyl));
					}
					break;
				}
			}
		}
	}

	// ���� �������� ����� �� �������� �� ������� ���� ������,
	// ���� ����� �� ��� ����� � ��� ������� ��������.
	source_indexpopadaniqnagranXY = new bool[ls];
	for (i = 0; i < ls; i++) {
		source_indexpopadaniqnagranXY[i] = false;
	}
	for (i = 0; i < ls; i++) {
		if (s[i].iunion_id == iunion_id_p1) {
			if (s[i].iPlane == XY) {
				for (integer i1 = 0; i1 <= inumboundaryz; i1++) {
					s[i].g.zS = s[i].g.zE;
					if (fabs(s[i].g.zS - rzboundary[i1]) < 1.0e-40) {
						source_indexpopadaniqnagranXY[i] = true;
					}
				}
			}
		}
	}

	if (iunion_id_p1 == 0) {
		// ��������� ������� �������.
		for (i = 0; i < lu; i++) {
			addboundary(rzboundary, inumboundaryz, my_union[i].zS);
			addboundary(rzboundary, inumboundaryz, my_union[i].zE);
		}
	}

	// ���������
	for (i = 0; i<ls; i++) {
		if (s[i].iunion_id == iunion_id_p1) {
			addboundary(rzboundary, inumboundaryz, s[i].g.zS);
			addboundary(rzboundary, inumboundaryz, s[i].g.zE);
		}
	}

	// ������
	for (i = 0; i<lw; i++) {
		if (w[i].iunion_id == iunion_id_p1) {
			addboundary(rzboundary, inumboundaryz, w[i].g.zS);
			addboundary(rzboundary, inumboundaryz, w[i].g.zE);
		}
	}

	// �������������� �� �����������
	//BubbleEnhSort(rzboundary, inumboundaryz);
	Sort_method(rzboundary, inumboundaryz);


	// snap to grid
	if (bsnap_TO) {
		doublereal rmindist = 1.0e30;

		for (i = 0; i < lb; i++) {
			if (b[i].iunion_id == iunion_id_p1) {
				if (bcylinder_meshing && (b[i].g.itypegeom == 1)) {
					// Cylinder
					switch (b[i].g.iPlane) {
					case XZ: case YZ:
						if (fabs(2.0*b[i].g.R_out_cyl) / dcount_diametr_cylinder < rmindist) {
							rmindist = fabs(2.0*b[i].g.R_out_cyl) / dcount_diametr_cylinder;
						}
						break;
					}
				}
				else if (fabs(b[i].g.zE - b[i].g.zS) < rmindist) {
					rmindist = fabs(b[i].g.zE - b[i].g.zS);
				}
			}
		}
		for (i = 0; i < ls; i++) {
			if (s[i].iunion_id == iunion_id_p1) {
				if ((s[i].iPlane == XZ) || ((s[i].iPlane == YZ))) {
					if (fabs(s[i].g.zE - s[i].g.zS) < rmindist) {
						rmindist = fabs(s[i].g.zE - s[i].g.zS);
					}
				}
			}
		}
		for (i = 0; i < lw; i++) {
			if (w[i].iunion_id == iunion_id_p1) {
				if ((w[i].iPlane == XZ) || (w[i].iPlane == YZ)) {
					if (fabs(w[i].g.zE - w[i].g.zS) < rmindist) {
						rmindist = fabs(w[i].g.zE - w[i].g.zS);
					}
				}
			}
		}

		if (iunion_id_p1 == 0) {
			// ��������� ������� �������.
			for (i = 0; i < lu; i++) {
				if (fabs(my_union[i].zE - my_union[i].zS) < rmindist) {
					rmindist = fabs(my_union[i].zE - my_union[i].zS);
				}
			}
		}

		if (minimum_fluid_gap_z < rmindist) rmindist = minimum_fluid_gap_z;

		rmindist *= snap_to_multiplyer;
		doublereal movetopos;
		doublereal changepos;
		bool bmove = false;
		bool bmove2 = false;
		for (i = 0; i < inumboundaryz; i++) {
			if (bmove) bmove2 = true;
			bmove = false;
			if (fabs(rzboundary[i + 1] - rzboundary[i]) < rmindist) {
				if (i > 0) {
					if (i + 2 <= inumboundaryz) {
						if (fabs(rzboundary[i - 1] - rzboundary[i]) <= fabs(rzboundary[i + 2] - rzboundary[i + 1])) {
							if (fabs(rzboundary[i - 1] - rzboundary[i]) < fabs(rzboundary[i + 1] - rzboundary[i])) {
								// ������ ���� ������ ����� �� �����.
								movetopos = rzboundary[i - 1];
								changepos = rzboundary[i];
							}
							else {
								// �������� ��������.
								movetopos = rzboundary[i + 1];
								changepos = rzboundary[i];
							}
							bmove = true;
						}
						else {
							if (fabs(rzboundary[i + 2] - rzboundary[i + 1]) < fabs(rzboundary[i + 1] - rzboundary[i])) {
								// ������ ���� ������ ����� �� �����.
								movetopos = rzboundary[i + 2];
								changepos = rzboundary[i + 1];
							}
							else {
								// �������� ��������.
								movetopos = rzboundary[i];
								changepos = rzboundary[i + 1];
							}

							bmove = true;
						}
					}
					else {
						movetopos = rzboundary[inumboundaryz];
						changepos = rzboundary[inumboundaryz - 1];
						bmove = true;
					}
				}
				else {
					movetopos = rzboundary[0];
					changepos = rzboundary[1];
					bmove = true;
				}
			}
			if (bmove) {
				printf(" Z changepos=%e to movetopos=%e \n", changepos, movetopos);
				for (integer i_2 = 0; i_2 < lb; i_2++) {
					if (b[i_2].iunion_id == iunion_id_p1) {
						if (fabs(b[i_2].g.zE - changepos) < 1.0e-33) {
							printf("block zE=%e zS=%e movetopos%e\n", b[i_2].g.zE, b[i_2].g.zS, movetopos);
							b[i_2].g.zE = movetopos;
						}
						if (fabs(b[i_2].g.zS - changepos) < 1.0e-33) {
							printf("block zS=%e zE=%e movetopos=%e\n", b[i_2].g.zS, b[i_2].g.zE, movetopos);
							b[i_2].g.zS = movetopos;
						}
					}
				}
				for (integer i_2 = 0; i_2 < lw; i_2++) {
					if (w[i_2].iunion_id == iunion_id_p1) {
						if (fabs(w[i_2].g.zE - changepos) < 1.0e-33) {
							printf("wall zE\n");
							w[i_2].g.zE = movetopos;
						}
						if (fabs(w[i_2].g.zS - changepos) < 1.0e-33) {
							printf("wall zS\n");
							w[i_2].g.zS = movetopos;
						}
					}
				}
				for (integer i_2 = 0; i_2 < ls; i_2++) {
					if (s[i_2].iunion_id == iunion_id_p1) {
						if (fabs(s[i_2].g.zE - changepos) < 1.0e-33) {
							printf("source zE\n");
							s[i_2].g.zE = movetopos;
						}
						if (fabs(s[i_2].g.zS - changepos) < 1.0e-33) {
							printf("source zS\n");
							s[i_2].g.zS = movetopos;
						}
					}
				}
			}
		}

		if (bmove2) {
			if (brepeat) {
				brepeat = false;
				goto RESTARTZ_SNAPTO;
			}
		}

	}

	brepeat = false;

}


/* ���������� ���������� ��������� ����� hex cartesian - 
 * ����������������� ������������� ����� � ��������� ��������� 
 * ������� � ���� �������. 
*/
void simplemeshgen(doublereal* &xpos, doublereal* &ypos, doublereal* &zpos, integer &inx, integer &iny, integer &inz,
				   integer lb, integer ls, integer lw,   BLOCK* b, SOURCE* &s, WALL* w, integer lu, UNION* &my_union, TPROP* matlist,
	doublereal* &xposadd, doublereal* &yposadd, doublereal* &zposadd,
	integer &inxadd, integer &inyadd, integer &inzadd, integer &iunion_id_p1)
{
	// �������� 0.1 ��������� � ��� ����� �� �������.
	doublereal deltavolkov = 0.1; // ������������� ����� � ���� �� �������.

	//bool bsnap_TO = bsnap_TO_global; // snap to grid (��������� �� ����� � ������� �������).
	//doublereal snap_to_multiplyer = 0.3;

	bool bgeom=true; // ���� true, �� ������������ ������������� ����� �� ������ �������������� ����������.
	if (lb <= 2) {
		bgeom = false;
	}
	doublereal q=1.02; // 1.05 - ����� ����� �������������, 1.1, 1.2 - �������� �������������, 1.25, 1.5, 2 - ������ �������������. 
	// ��� ������������������ ������� �������������� ����������� �������������� ���������� ������ ���� �� ������ 1.3.

	//bool brepeat = true;

	// �� ��� Ox
	doublereal *rxboundary = NULL; // ������ ������������ ������
	integer inumboundaryx = 1;
	
	doublereal *ryboundary = NULL; // ������ ������������ ������
	integer inumboundaryy = 1;

	doublereal *rzboundary = NULL; // ������ ������������ ������
	integer inumboundaryz = 1;

	// ���������� � ���������� ������� minimum fluid gap.
	calc_minimum_fluid_gap1(inumboundaryx, rxboundary, inumboundaryy, ryboundary, inumboundaryz, rzboundary,
		lb, ls, lw, b, s, w, lu, my_union, iunion_id_p1);

	doublereal minimum_fluid_gap_x = 1.0e40;
	doublereal minimum_fluid_gap_y = 1.0e40;
	doublereal minimum_fluid_gap_z = 1.0e40;

	// ��������������� ���������� ������� minimum fluid gap.
	calc_minimum_fluid_gap2(inumboundaryx, rxboundary, inumboundaryy, ryboundary,
		inumboundaryz, rzboundary, minimum_fluid_gap_x, minimum_fluid_gap_y, minimum_fluid_gap_z, 
		lb,  ls,  lw,  b, s, w, lu, my_union, iunion_id_p1);


	bool *source_indexpopadaniqnagranYZ = NULL;
	bool *source_indexpopadaniqnagranXY = NULL;
	bool *source_indexpopadaniqnagranXZ = NULL;

	// 12.03.2017
	// ���������� snap to
	// ������������ ����������� �������� ������ �� 33%.
	snap_to_moving(source_indexpopadaniqnagranYZ,
		source_indexpopadaniqnagranXY,
		source_indexpopadaniqnagranXZ,
		rxboundary, ryboundary, rzboundary,
		inumboundaryx, inumboundaryy, inumboundaryz,
		minimum_fluid_gap_x, minimum_fluid_gap_y, minimum_fluid_gap_z,
		lb, ls, lw, b, s, w, lu, my_union, iunion_id_p1);

	integer i;


    integer *ixintervalcount; // ����� ����������
	ixintervalcount = new integer [inumboundaryx]; // �� ���� ������ ��� ����� ������.
	doublereal alphascal=1.0;
	integer inowintervalcount;
	for (i=0; i<(inumboundaryx); i++) {
         alphascal=(rxboundary[i+1]-rxboundary[i])/(rxboundary[inumboundaryx]-rxboundary[0]);
         inowintervalcount=(int)(alphascal*inx);
		 if (inowintervalcount < min_elem_in_x_element) inowintervalcount=min_elem_in_x_element;


		 // FLUID
		 bool b2div = false;
		 for (integer i_3 = 0; i_3 < (inumboundaryy); i_3++) {
			 for (integer i_4 = 0; i_4 < (inumboundaryz); i_4++) {
				 doublereal yp_3 = 0.5*(ryboundary[i_3 + 1] + ryboundary[i_3]);
				 doublereal zp_3 = 0.5*(rzboundary[i_4 + 1] + rzboundary[i_4]);
				 doublereal xp_1 = 0.5*(rxboundary[i + 1] + rxboundary[i]);
				 doublereal xp_2 = xp_1;
				 doublereal xp_3 = xp_1;
				 if (i < inumboundaryx - 1) {
					 xp_2 = 0.5*(rxboundary[i + 2] + rxboundary[i + 1]);
				 }
				 if (i > 0) {
					 xp_3 = 0.5*(rxboundary[i - 1] + rxboundary[i]);
				 }
				 // ���������� ����� ����� �� ���������� �����.
				 //myisblock_id(integer lb, BLOCK* &b, doublereal x11, doublereal y11, doublereal z11)
				 if (!((((b[myisblock_id(lb, b, xp_1, yp_3, zp_3)].itype == HOLLOW)||(b[myisblock_id(lb, b, xp_1, yp_3, zp_3)].itype == SOLID)) && ((b[myisblock_id(lb, b, xp_2, yp_3, zp_3)].itype == HOLLOW)||(b[myisblock_id(lb, b, xp_2, yp_3, zp_3)].itype == SOLID)) && ((b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == HOLLOW)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == SOLID))) || ((b[myisblock_id(lb, b, xp_1, yp_3, zp_3)].itype == SOLID) && (b[myisblock_id(lb, b, xp_2, yp_3, zp_3)].itype == SOLID) && (b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == SOLID)) || (((b[myisblock_id(lb, b, xp_1, yp_3, zp_3)].itype == FLUID)||(b[myisblock_id(lb, b, xp_1, yp_3, zp_3)].itype == HOLLOW)) && ((b[myisblock_id(lb, b, xp_2, yp_3, zp_3)].itype == FLUID)||(b[myisblock_id(lb, b, xp_2, yp_3, zp_3)].itype == HOLLOW)) && ((b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == FLUID)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == HOLLOW)))))
				 {
					 // ���� ������ ���� ������� ����� ����� �� ������������� ������������ solid ��� fluid.
					 b2div = true;
				 }
			 }
		 }
		 if (b2div) {
			// ixintervalcount[i] = 2;
			ixintervalcount[i] = inowintervalcount;
		 }
		 else {
			 printf("inc X");
			 //getchar();
			 ixintervalcount[i] = 1;
		 }
		 // 20 02 2017
		 ixintervalcount[i] = inowintervalcount;

         
	}
    /* // debug
	for (i=0; i<(inumboundaryx); i++) {
	#if doubleintprecision == 1
		printf("%lld  ",ixintervalcount[i]);
	#else
		printf("%d  ",ixintervalcount[i]);
	#endif
         
	} //*/
    //getchar();

	// ���������� ������ ��������� �����.
	integer iposmark = 1;
	doublereal dx;
	integer k;
	integer ixoldsize=0;
	SetLength(xpos,ixoldsize,1);
    ixoldsize=1;
	for (i=0; i<(inumboundaryx); i++) 
	{
		if ((ixintervalcount[i]-2)%2==0) ixintervalcount[i]++; // ����� ����� ����� ���� ������ ������.
		integer n2=(int)((ixintervalcount[i]-1)/2); // ���������� � ������� �������
		doublereal qn2=q;
		for (integer i1=1; i1<n2; i1++) qn2*=q; // ���������� � �������.
		doublereal b1=(rxboundary[i+1]-rxboundary[i])*(q-1.0)/(2.0*(qn2-1.0));
		//printf("length=%e\n",(rxboundary[i+1]-rxboundary[i]));
		//getchar(); // OK

        dx=(rxboundary[i+1]-rxboundary[i])/(ixintervalcount[i]-1);
		SetLength(xpos,ixoldsize,(iposmark+ixintervalcount[i]));
        ixoldsize=(iposmark+ixintervalcount[i]);
#if doubleintprecision == 1
		//printf("%lld  ",ixoldsize);// debug
#else
		//printf("%d  ",ixoldsize);// debug
#endif
       
        for (k=iposmark; k<=iposmark+ixintervalcount[i]-2; k++)
		{
			if (!bgeom) {
				if (lb <= 2 ) {
					
					doublereal betavolkov = 1.0 / (sqrt(1.0 - deltavolkov));
					doublereal nvolkow = ((doublereal)(k - iposmark)) / ((doublereal)(ixintervalcount[i] - 1.0));
					xpos[k] = (rxboundary[i+1]- rxboundary[i]) * fmax(0.0,(((betavolkov + 1.0)*(pow(((betavolkov + 1.0) / (betavolkov - 1.0)), 2.0*nvolkow - 1.0)) - (betavolkov - 1.0))));
					xpos[k] /= 2.0*(1.0+pow((betavolkov + 1.0) / (betavolkov - 1.0), 2.0*nvolkow - 1.0));
					xpos[k] += rxboundary[i];					
				}
				else {
					// ����������� �����
					xpos[k] = rxboundary[i] + (k - iposmark)*dx;
				}
			}
			else
			{
				// ������������� �����
				integer ic1=k-iposmark;
				doublereal madd=b1;
				if (ic1<=n2) {
					// ������� ����� �������.
					for (integer i1=1; i1<ic1; i1++) madd*=q; 
				} else 
				{
					// ����� ������ �������.
					for (integer i1=2*n2-ic1; i1>0; i1--) madd*=q;
				}
				if (k==iposmark) xpos[k]=rxboundary[i];
				else xpos[k]=xpos[k-1]+madd;
				
			}
		}
		iposmark=iposmark+ixintervalcount[i]-1;
	}
	SetLength(xpos,ixoldsize,iposmark+1);
	xpos[iposmark]=rxboundary[inumboundaryx];
	inx=iposmark;
    for (i=0; i<inx; i++) xpos[i]=xpos[i+1]; // ����� ����� �� 1
    SetLength(xpos,inx+1,inx); 
	inx--; // ���������� ���������� � ���� � ������������� ��������� inx
	
   // 3 �������� 2017.
   // ���������� ����� �������� �����.
   // ������������� �� ���������� ����������� ����������� ���� �����.
	for (int i_28 = 0; i_28 <= inxadd; i_28++) {
		SetLength(xpos, inx + 1, inx + 2);
		xpos[inx + 1] = xposadd[i_28];
		inx++;
	}
	Sort_method(xpos, inx);

	for (i=0; i<adapt_x; i++) simplecorrect_meshgen_x(xpos, inx, lb, ls, lw, b, s, w);

	//const doublereal etalon_max_size_ratio=2.0;
	integer inum_iter_ratio_good=0;
	while (1) {

	// ���������� max size ratio x axis.
	doublereal max_size_ratio_x=1.0;
	for (i=0; i<inx-1; i++) {
		
		doublereal dmax=0.0;
		doublereal dmin=0.0;
		if (fabs(xpos[i+1]-xpos[i])>fabs(xpos[i+2]-xpos[i+1])) {
			dmax=fabs(xpos[i+1]-xpos[i]);
			dmin=fabs(xpos[i+2]-xpos[i+1]);
		}
		else {
			dmax=fabs(xpos[i+2]-xpos[i+1]);
			dmin=fabs(xpos[i+1]-xpos[i]);
		}
		if (dmax/dmin>max_size_ratio_x) {
			max_size_ratio_x=dmax/dmin;
		}
	}
	printf("x axis max size ratio is equal = %1.4f\n",max_size_ratio_x);
	if (max_size_ratio_x != max_size_ratio_x) {
		system("PAUSE");
	}
	//getchar();

	if (max_size_ratio_x < (etalon_max_size_ratio*1.1)) {
		break;
	}

	// ������������� max size ratio x axis.
	// ������� 9 ������ 2013 ����.
	
	bool bplus=false;
	max_size_ratio_x=1.0;
	for (i=0; i<inx-1; i++) {
		
		bplus=false;
		doublereal dmax=0.0;
		doublereal dmin=0.0;
		if (fabs(xpos[i+1]-xpos[i])>fabs(xpos[i+2]-xpos[i+1])) {
			dmax=fabs(xpos[i+1]-xpos[i]);
			dmin=fabs(xpos[i+2]-xpos[i+1]);
		}
		else {
			bplus=true;
			dmax=fabs(xpos[i+2]-xpos[i+1]);
			dmin=fabs(xpos[i+1]-xpos[i]);
		}
		if (dmax/dmin>etalon_max_size_ratio) {
			doublereal pos_candidate;
			if (bplus) {
				pos_candidate=xpos[i+1]+0.5*dmax;
			}
			else {
				pos_candidate=xpos[i]+0.5*dmax;
			}
			SetLength(xpos,inx+1,inx+2);
			xpos[inx+1]=pos_candidate;
            inx=inx+1;
			//BubbleEnhSort(xpos, inx);
			Sort_method(xpos, inx);
			break;
		}
	}

	inum_iter_ratio_good++;

	//getchar();

	}
	
#if doubleintprecision == 1
	printf("inum_iter_ratio_good is %lld\n", inum_iter_ratio_good);
#else
	printf("inum_iter_ratio_good is %d\n", inum_iter_ratio_good);
#endif
	
	if (bwait) {
	   //getchar();
		system("pause");

	}

	/* // debug
	for (i=0; i<=inx; i++) {
        printf("%f  ",xpos[i]);   
	}
	getchar(); //*/ 


   


    integer *iyintervalcount; // ����� ����������
    iyintervalcount = new integer [inumboundaryy]; // �� ���� ������ ��� ����� ������.
    for (i=0; i<(inumboundaryy); i++) {
         alphascal=(ryboundary[i+1]-ryboundary[i])/(ryboundary[inumboundaryy]-rxboundary[0]);
         inowintervalcount=(int)(alphascal*iny);
		 if (inowintervalcount < min_elem_in_y_element) inowintervalcount=min_elem_in_y_element; 

		 // FLUID
		 bool b2div = false;
		 for (integer i_3 = 0; i_3 < (inumboundaryx); i_3++) {
			 for (integer i_4 = 0; i_4 < (inumboundaryz); i_4++) {
				 doublereal xp_3 = 0.5*(rxboundary[i_3 + 1] + rxboundary[i_3]);
				 doublereal zp_3 = 0.5*(rzboundary[i_4 + 1] + rzboundary[i_4]);
				 doublereal yp_1 = 0.5*(ryboundary[i + 1] + ryboundary[i]);
				 doublereal yp_2 = yp_1;
				 doublereal yp_3 = yp_1;
				 if (i < inumboundaryy - 1) {
					 yp_2 = 0.5*(ryboundary[i + 2] + ryboundary[i + 1]);
				 }
				 if (i > 0) {
					 yp_3 = 0.5*(ryboundary[i - 1] + ryboundary[i]);
				 }
				 // ���������� ����� ����� �� ���������� �����.
				 //myisblock_id(integer lb, BLOCK* &b, doublereal x11, doublereal y11, doublereal z11)
				 if (!((((b[myisblock_id(lb, b, xp_3, yp_1, zp_3)].itype == HOLLOW)||(b[myisblock_id(lb, b, xp_3, yp_1, zp_3)].itype == SOLID)) && ((b[myisblock_id(lb, b, xp_3, yp_2, zp_3)].itype == HOLLOW)||(b[myisblock_id(lb, b, xp_3, yp_2, zp_3)].itype == SOLID)) && ((b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == HOLLOW)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == SOLID))) || ((b[myisblock_id(lb, b, xp_3, yp_1, zp_3)].itype == SOLID) && (b[myisblock_id(lb, b, xp_3, yp_2, zp_3)].itype == SOLID) && (b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == SOLID)) || (((b[myisblock_id(lb, b, xp_3, yp_1, zp_3)].itype == FLUID)||(b[myisblock_id(lb, b, xp_3, yp_1, zp_3)].itype == HOLLOW)) && ((b[myisblock_id(lb, b, xp_3, yp_2, zp_3)].itype == FLUID)||(b[myisblock_id(lb, b, xp_3, yp_2, zp_3)].itype == HOLLOW)) && ((b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == FLUID)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == HOLLOW)))))
				 {
					 // ���� ������ ���� ������� ����� ����� �� ������������� ������������ solid ��� fluid.
					 b2div = true;
				 }
			 }
		 }
		 if (b2div) {
			// iyintervalcount[i] = 2;
			iyintervalcount[i] = inowintervalcount;
		 }
		 else {
			 printf("inc Y");
			 //getchar();
			 iyintervalcount[i] = 1;
		 }
		 // 20 02 2017
		 iyintervalcount[i] = inowintervalcount;
         
	}
    // ���������� ������� �����
    iposmark = 1;
	doublereal dy;
    integer iyoldsize=0;
    SetLength(ypos,iyoldsize,1);
    iyoldsize=1;
	for (i=0; i<inumboundaryy; i++) 
	{

		// ����� ����� ����� ���� ������ ������
		if ((iyintervalcount[i]-2)%2==0) iyintervalcount[i]++;
		integer n2=(int)((iyintervalcount[i]-1)/2); // ���������� � ������� �������
		doublereal qn2=q;
		for (integer i1=1; i1<n2; i1++) qn2*=q; // ���������� � �������.
		doublereal b1=(ryboundary[i+1]-ryboundary[i])*(q-1.0)/(2.0*(qn2-1.0));

        dy=(ryboundary[i+1]-ryboundary[i])/(iyintervalcount[i]-1);
		SetLength(ypos,iyoldsize,iposmark+iyintervalcount[i]);
        iyoldsize=iposmark+iyintervalcount[i];
        for (k=iposmark; k<=iposmark+iyintervalcount[i]-2; k++) 
		{
			if (!bgeom) {
				if (lb <= 2) {

					doublereal betavolkov = 1.0 / (sqrt(1.0 - deltavolkov));
					doublereal nvolkow = ((doublereal)(k - iposmark)) / ((doublereal)(iyintervalcount[i] - 1.0));
					ypos[k] = (ryboundary[i+1] - ryboundary[i]) * fmax(0.0, (((betavolkov + 1.0)*(pow(((betavolkov + 1.0) / (betavolkov - 1.0)), 2.0*nvolkow - 1.0)) - (betavolkov - 1.0))));
					ypos[k] /= 2.0*(1.0+pow( (betavolkov + 1.0) / (betavolkov - 1.0), 2.0*nvolkow - 1.0));
					ypos[k] += ryboundary[i];

				}
				else {
					// ����������� �����
					ypos[k] = ryboundary[i] + (k - iposmark)*dy;
				}
			}
			else 
			{
				// ������������� �����
				// �� ������ �������������� ���������� � ����� ����������.
				integer ic1=k-iposmark;
				doublereal madd=b1;
				if (ic1<=n2) {
					for (integer i1=1; i1<ic1; i1++) madd*=q;
				} else 
				{
					for (integer i1=2*n2-ic1; i1>0; i1--) madd*=q;
				}
				if (k==iposmark) ypos[k]=ryboundary[i];
				else ypos[k]=ypos[k-1]+madd;
			}
		}
		iposmark=iposmark+iyintervalcount[i]-1;
	}
	SetLength(ypos,iyoldsize,iposmark+1);
	ypos[iposmark]=ryboundary[inumboundaryy];
	iny=iposmark;
    for (i=0; i<iny; i++) ypos[i]=ypos[i+1]; // ����� ����� �� 1
    SetLength(ypos,iny+1,iny); 
	iny--; // ���������� ���������� � ���� � ������������� ��������� iny
    

	// 3 �������� 2017.
	// ���������� ����� �������� �����.
	// ������������� �� ���������� ����������� ����������� ���� �����.
	for (int i_28 = 0; i_28 <= inyadd; i_28++) {
		SetLength(ypos, iny + 1, iny + 2);
		ypos[iny + 1] = yposadd[i_28];
		iny++;
	}
	Sort_method(ypos, iny);

    for (i=0; i<adapt_y; i++) simplecorrect_meshgen_y(ypos, iny, lb, ls, lw, b, s, w);

	inum_iter_ratio_good=0;
	while (1) {

	doublereal max_size_ratio_y=1.0;
	for (i=0; i<iny-1; i++) {
		
		doublereal dmax=0.0;
		doublereal dmin=0.0;
		if (fabs(ypos[i+1]-ypos[i])>fabs(ypos[i+2]-ypos[i+1])) {
			dmax=fabs(ypos[i+1]-ypos[i]);
			dmin=fabs(ypos[i+2]-ypos[i+1]);
		}
		else {
			dmax=fabs(ypos[i+2]-ypos[i+1]);
			dmin=fabs(ypos[i+1]-ypos[i]);
		}
		if (dmax/dmin>max_size_ratio_y) {
			max_size_ratio_y=dmax/dmin;
		}
	}
	printf("y axis max size ratio is equal = %1.4f\n",max_size_ratio_y);
	if (max_size_ratio_y != max_size_ratio_y) {
		system("PAUSE");
	}
	//getchar();
	
	if (max_size_ratio_y < (etalon_max_size_ratio*1.1)) {
		break;
	}

	// ������������� max size ratio y axis.
	// ������� 9 ������ 2013 ����.
	
	bool bplus=false;
	max_size_ratio_y=1.0;
	for (i=0; i<iny-1; i++) {
		
		bplus=false;
		doublereal dmax=0.0;
		doublereal dmin=0.0;
		if (fabs(ypos[i+1]-ypos[i])>fabs(ypos[i+2]-ypos[i+1])) {
			dmax=fabs(ypos[i+1]-ypos[i]);
			dmin=fabs(ypos[i+2]-ypos[i+1]);
		}
		else {
			bplus=true;
			dmax=fabs(ypos[i+2]-ypos[i+1]);
			dmin=fabs(ypos[i+1]-ypos[i]);
		}
		if (dmax/dmin>etalon_max_size_ratio) {
			doublereal pos_candidate;
			if (bplus) {
				pos_candidate=ypos[i+1]+0.5*dmax;
			}
			else {
				pos_candidate=ypos[i]+0.5*dmax;
			}
			SetLength(ypos,iny+1,iny+2);
			ypos[iny+1]=pos_candidate;
            iny=iny+1;
			//BubbleEnhSort(ypos, iny);
			Sort_method(ypos, iny);
			break;
		}
	}

	inum_iter_ratio_good++;

	//getchar();

	}
	
#if doubleintprecision == 1
	printf("inum_iter_ratio_good is %lld\n", inum_iter_ratio_good);
#else
	printf("inum_iter_ratio_good is %d\n", inum_iter_ratio_good);
#endif
	
	if (bwait) {
	   //getchar();
		system("pause");
	}

	/* // debug
	for (i=0; i<=iny; i++) {
        printf("%f  ",ypos[i]);   
	}//*/

    // �� ��� Oz
     
   
	//brepeat = false;


    integer *izintervalcount; // ����� ����������
    izintervalcount = new integer [inumboundaryz]; // �� ���� ������ ��� ����� ������.
    for (i=0; i<(inumboundaryz); i++) {
         alphascal=(rzboundary[i+1]-rzboundary[i])/(rzboundary[inumboundaryz]-rzboundary[0]);
         inowintervalcount=(int)(alphascal*inz);
		 if (inowintervalcount < min_elem_in_z_element) inowintervalcount=min_elem_in_z_element;

		 // FLUID
		 bool b2div = false;
		 for (integer i_3 = 0; i_3 < (inumboundaryx); i_3++) {
			 for (integer i_4 = 0; i_4 < (inumboundaryy); i_4++) {
				 doublereal xp_3 = 0.5*(rxboundary[i_3 + 1] + rxboundary[i_3]);
				 doublereal yp_3 = 0.5*(ryboundary[i_4 + 1] + ryboundary[i_4]);
				 doublereal zp_1 = 0.5*(rzboundary[i + 1] + rzboundary[i]);
				 doublereal zp_2 = zp_1;
				 doublereal zp_3 = zp_1;
				 if (i < inumboundaryz - 1) {
					 zp_2 = 0.5*(rzboundary[i + 2] + rzboundary[i + 1]);
				 }
				 if (i > 0) {
					 zp_3 = 0.5*(rzboundary[i - 1] + rzboundary[i]);
				 }
				 // ���������� ����� ����� �� ���������� �����.
				 //myisblock_id(integer lb, BLOCK* &b, doublereal x11, doublereal y11, doublereal z11)
				 if (!((((b[myisblock_id(lb, b, xp_3, yp_3, zp_1)].itype == HOLLOW)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_1)].itype == SOLID)) && ((b[myisblock_id(lb, b, xp_3, yp_3, zp_2)].itype == HOLLOW)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_2)].itype == SOLID)) && ((b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == HOLLOW)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == SOLID))) || ((b[myisblock_id(lb, b, xp_3, yp_3, zp_1)].itype == SOLID) && (b[myisblock_id(lb, b, xp_3, yp_3, zp_2)].itype == SOLID) && (b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == SOLID)) || (((b[myisblock_id(lb, b, xp_3, yp_3, zp_1)].itype == FLUID)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_1)].itype == HOLLOW)) && ((b[myisblock_id(lb, b, xp_3, yp_3, zp_2)].itype == FLUID)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_2)].itype == HOLLOW)) && ((b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == FLUID)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == HOLLOW)))))
				 {
					 // ���� ������ ���� ������� ����� ����� �� ������������� ������������ solid ��� fluid.
					 b2div = true;
				 }
			 }
		 }
		 if (b2div) {
			// izintervalcount[i] = 2;
			 izintervalcount[i] = inowintervalcount;
		 }
		 else {
			 printf("inc Z");
			// getchar();
			 izintervalcount[i] = 1;
		 }
		 // 20 02 2017
		 izintervalcount[i] = inowintervalcount;
         
	}
    // ���������� ������� �����
    iposmark = 1;
	doublereal dz;
    integer izoldsize=0;
    SetLength(zpos,izoldsize,1);
    izoldsize=1;
	for (i=0; i<(inumboundaryz); i++) 
	{
		// ����� ����� ����� ���� ������ ������
		if ((izintervalcount[i]-2)%2==0) izintervalcount[i]++;
		integer n2=(int)((izintervalcount[i]-1)/2); // ���������� � ������� �������
		doublereal qn2=q;
		for (integer i1=1; i1<n2; i1++) qn2*=q; // ���������� � �������.
		doublereal b1=(rzboundary[i+1]-rzboundary[i])*(q-1.0)/(2.0*(qn2-1.0));

        dz=(rzboundary[i+1]-rzboundary[i])/(izintervalcount[i]-1);
		SetLength(zpos,izoldsize,iposmark+izintervalcount[i]);
        izoldsize=iposmark+izintervalcount[i];
        for (k=iposmark; k<=iposmark+izintervalcount[i]-2; k++)
		{
			if (!bgeom) {
				if (lb <= 2) {

					doublereal betavolkov = 1.0 / (sqrt(1.0 - deltavolkov));
					doublereal nvolkow = ((doublereal)(k - iposmark)) / ((doublereal)(izintervalcount[i] - 1.0));
					zpos[k] = (rzboundary[i+1] - rzboundary[i]) * fmax(0.0,(((betavolkov + 1.0)*(pow(((betavolkov + 1.0) / (betavolkov - 1.0)), 2.0*nvolkow - 1.0)) - (betavolkov - 1.0))));
					zpos[k] /= 2.0*(1.0+pow((betavolkov + 1.0) / (betavolkov - 1.0), 2.0*nvolkow - 1.0));
					zpos[k] += rzboundary[i];
					//printf("%e\n", zpos[k]);
					//getchar();

				}
				else {
					// ����������� �����
					zpos[k] = rzboundary[i] + (k - iposmark)*dz;
				}
			}
			else 
			{
				// ������������� �����
				// �� ������ �������������� ���������� � ����� ����������.
				integer ic1=k-iposmark;
				doublereal madd=b1;
				if (ic1<=n2) {
					for (integer i1=1; i1<ic1; i1++) madd*=q;
				} else 
				{
					for (integer i1=2*n2-ic1; i1>0; i1--) madd*=q;
				}
				if (k==iposmark) zpos[k]=rzboundary[i];
				else zpos[k]=zpos[k-1]+madd;
			}
		}
		iposmark=iposmark+izintervalcount[i]-1;
	}
	SetLength(zpos,izoldsize,iposmark+1);
	zpos[iposmark]=rzboundary[inumboundaryz];
	inz=iposmark;
	for (i=0; i<inz; i++) zpos[i]=zpos[i+1]; // ����� ����� �� 1
    SetLength(zpos,inz+1,inz); 
	inz--; // ���������� ���������� � ���� � ������������� ��������� inz

	// 3 �������� 2017.
	// ���������� ����� �������� �����.
	// ������������� �� ���������� ����������� ����������� ���� �����.
	for (int i_28 = 0; i_28 <= inzadd; i_28++) {
		SetLength(zpos, inz + 1, inz + 2);
		zpos[inz + 1] = zposadd[i_28];
		inz++;
	}
	Sort_method(zpos, inz);

	for (i=0; i<adapt_z; i++) simplecorrect_meshgen_z(zpos, inz, lb, ls, lw, b, s, w);
	
	inum_iter_ratio_good=0;
	while (1) {

	doublereal max_size_ratio_z=1.0;
	for (i=0; i<inz-1; i++) {
		
		doublereal dmax=0.0;
		doublereal dmin=0.0;
		if (fabs(zpos[i+1]-zpos[i])>fabs(zpos[i+2]-zpos[i+1])) {
			dmax=fabs(zpos[i+1]-zpos[i]);
			dmin=fabs(zpos[i+2]-zpos[i+1]);
		}
		else {
			dmax=fabs(zpos[i+2]-zpos[i+1]);
			dmin=fabs(zpos[i+1]-zpos[i]);
		}
		if (dmax/dmin>max_size_ratio_z) {
			max_size_ratio_z=dmax/dmin;
		}
	}
	printf("z axis max size ratio is equal = %1.4f\n",max_size_ratio_z);
	if (max_size_ratio_z != max_size_ratio_z) {
		system("PAUSE");
	}
	//getchar();

	if (max_size_ratio_z < (etalon_max_size_ratio*1.1)) {
		break;
	}

	// ������������� max size ratio z axis.
	// ������� 9 ������ 2013 ����.
	
	bool bplus=false;
	max_size_ratio_z=1.0;
	for (i=0; i<inz-1; i++) {
		
		bplus=false;
		doublereal dmax=0.0;
		doublereal dmin=0.0;
		if (fabs(zpos[i+1]-zpos[i])>fabs(zpos[i+2]-zpos[i+1])) {
			dmax=fabs(zpos[i+1]-zpos[i]);
			dmin=fabs(zpos[i+2]-zpos[i+1]);
		}
		else {
			bplus=true;
			dmax=fabs(zpos[i+2]-zpos[i+1]);
			dmin=fabs(zpos[i+1]-zpos[i]);
		}
		if (dmax/dmin>etalon_max_size_ratio) {
			doublereal pos_candidate;
			if (bplus) {
				pos_candidate=zpos[i+1]+0.5*dmax;
			}
			else {
				pos_candidate=zpos[i]+0.5*dmax;
			}
			SetLength(zpos,inz+1,inz+2);
			zpos[inz+1]=pos_candidate;
            inz=inz+1;
			//BubbleEnhSort(zpos, inz);
			Sort_method(zpos, inz);
			break;
		}
	}

	inum_iter_ratio_good++;

	//getchar();

	}
	
#if doubleintprecision == 1
	printf("inum_iter_ratio_good is %lld\n", inum_iter_ratio_good);
#else
	printf("inum_iter_ratio_good is %d\n", inum_iter_ratio_good);
#endif
	
	if (bwait) {
	  // getchar();
	  system("pause");

	}

	//for (i = 0; i <= inz; i++) {
	//printf("%e  ", zpos[i]);
	//}
	//getchar();

	integer inz_fix = inz;
	// ������������� ��������� XY
	for (i = 0; i < ls; i++) {
		if (source_indexpopadaniqnagranXY[i]) {
			doublereal xc = 0.5*(s[i].g.xS + s[i].g.xE);
			doublereal yc = 0.5*(s[i].g.yS + s[i].g.yE);
			doublereal zg = s[i].g.zS;
			// ����� ������� +Z �� ����� zpos
			// ����� ����� ��.
			// ���� �� ����������� Solid block �� �������� ������� � ���� ���� ������.
			// ��������� maxsize ratio 2 �� ������.
			// ���� ������� ��������� �� �� -Z � ������� ����.
			integer i55_found = -2;
			for (integer i55 = 0; i55 <= inz_fix; i55++) {
				if (fabs(zpos[i55] - zg) < 1.0e-40) {
					i55_found = i55;
					break;
				}
			}
			if (i55_found >= 0) {
				if (i55_found < inz_fix) {
					doublereal zg1 = 0.5*(zg + zpos[i55_found + 1]);
					printf("zg1=%e\n", zg1);
					integer i56_found = -2;
					for (integer ib55 = 0; ib55 < lb; ib55++) {
						if ((xc>b[ib55].g.xS) && (xc<b[ib55].g.xE) && (yc>b[ib55].g.yS) && (yc<b[ib55].g.yE) && (zg1>b[ib55].g.zS) && (zg1 < b[ib55].g.zE))
						{
							i56_found = ib55;
						}
					}
					
					bool bzero_pos = true;
					doublereal zg2 = zpos[0] - 0.5*fabs(zpos[1] - zpos[0]);
					if (i55_found>0) {
						zg2 = 0.5*(zg + zpos[i55_found - 1]);
						bzero_pos = false;
					}
					printf("zg2=%e\n", zg2);
					integer i57_found = -2;
					for (integer ib57 = 0; ib57 < lb; ib57++) {
						if ((xc>b[ib57].g.xS) && (xc<b[ib57].g.xE) && (yc>b[ib57].g.yS) && (yc<b[ib57].g.yE) && (zg2>b[ib57].g.zS) && (zg2 < b[ib57].g.zE))
						{
							if (zg2 > zpos[0]) {
								i57_found = ib57;
							}
						}
					}

					if (i56_found >= 0) {
						if (b[i56_found].itype == SOLID) {
							if ((i57_found >= 0) && (b[i57_found].itype == SOLID)) {
								// �� �������� �������� ����� � ���� � ������� �����������������.
								// comparison_lam ����� ������ ���� ���������������� ����� i56 ������.
								if (comparison_lam(matlist, b, i56_found, i57_found, 25.0)) {
									// � ����� i56 ���������������� ����.
									s[i].g.zS = zg1;
									s[i].g.zE = zg1;
									addboundary(zpos, inz, zg1);
								}
								else {
									// � ����� i57 ���������������� ����.
									s[i].g.zS = zg2;
									s[i].g.zE = zg2;
									addboundary(zpos, inz, zg2);
								}
							}
							else {
								
								// ������ ����� ����.
								printf("zg1==%e\n", zg1);
								doublereal zgolg = zpos[1];
								if (inz == 1) {
									// ������� ������������� ��������� �����.
									zgolg = 0.5*(zpos[0] + zg1);
									addboundary(zpos, inz, zgolg);
									zgolg = 0.5*(zpos[1] + zg1);
									addboundary(zpos, inz, zgolg);
									zgolg = 0.5*(zpos[0] + zg1);
									// ���������� �� ���� ������ ��������� ������� ��� ������.
									s[i].g.zS = zgolg;
									s[i].g.zE = zgolg;
								}
								else {
									if (bzero_pos) {
										// �������� � ������� 0.0.
										// ��������� �����������.
										zgolg = 0.5*(zpos[0] + zg1);
										addboundary(zpos, inz, zgolg);
										zgolg = 0.5*(zpos[1] + zg1);
										addboundary(zpos, inz, zgolg);
										zgolg = 0.5*(zpos[0] + zg1);
										// ���������� �� ���� ������ ��������� ������� ��� ������.
										s[i].g.zS = zgolg;
										s[i].g.zE = zgolg;
									}
									else {
										s[i].g.zS = zg1;
										s[i].g.zE = zg1;
									}
								}
								addboundary(zpos, inz, zg1);
								

								
							}
						}
						else {

							if (i57_found >= 0) {
								if (b[i57_found].itype == SOLID) {
									// ������ ����� ����.
									s[i].g.zS = zg2;
									s[i].g.zE = zg2;
									addboundary(zpos, inz, zg2);
								}
							}
							else {
								printf("ERROR : sourse na granice dvus hollow or fluid blockov.");
								system("PAUSE");
								exit(1);
							}
						}
					}
				}
				else {
					doublereal zg2 = 0.5*(zg + zpos[i55_found - 1]);
					printf("zg2=%e\n", zg2);
					integer i57_found = -2;
					for (integer ib57 = 0; ib57 < lb; ib57++) {
						if ((xc>b[ib57].g.xS) && (xc<b[ib57].g.xE) && (yc>b[ib57].g.yS) && (yc<b[ib57].g.yE) && (zg2>b[ib57].g.zS) && (zg2 < b[ib57].g.zE))
						{
							i57_found = ib57;
						}
					}
					if (i57_found >= 0) {
						if (b[i57_found].itype == SOLID) {
							// ������ ����� ����.
							s[i].g.zS = zg2;
							s[i].g.zE = zg2;
							addboundary(zpos, inz, zg2);
						}
					}
					else {
						printf("ERROR : sourse na granice dvus hollow or fluid blockov.");
						system("PAUSE");
						exit(1);
					}
				}
			}
		}
	}
	//BubbleEnhSort(zpos, inz);
	Sort_method(zpos, inz);
	//SetLength(zpos, inz + 2, inz);
	//inz++;

	integer inx_fix = inx;
	// ������������� ��������� YZ
	for (i = 0; i < ls; i++) {
		if (source_indexpopadaniqnagranYZ[i]) {
			doublereal zc = 0.5*(s[i].g.zS + s[i].g.zE);
			doublereal yc = 0.5*(s[i].g.yS + s[i].g.yE);
			doublereal xg = s[i].g.xS;
			// ����� ������� +Z �� ����� zpos
			// ����� ����� ��.
			// ���� �� ����������� Solid block �� �������� ������� � ���� ���� ������.
			// ��������� maxsize ratio 2 �� ������.
			// ���� ������� ��������� �� �� -Z � ������� ����.
			integer i55_found = -2;
			for (integer i55 = 0; i55 <= inx_fix; i55++) {
				if (fabs(xpos[i55] - xg) < 1.0e-40) {
					i55_found = i55;
					break;
				}
			}
			if (i55_found >= 0) {
				if (i55_found < inx_fix) {
					doublereal xg1 = 0.5*(xg + xpos[i55_found + 1]);
					printf("xg1=%e\n", xg1);
					integer i56_found = -2;
					for (integer ib55 = 0; ib55 < lb; ib55++) {
						if ((zc>b[ib55].g.zS) && (zc<b[ib55].g.zE) && (yc>b[ib55].g.yS) && (yc<b[ib55].g.yE) && (xg1>b[ib55].g.xS) && (xg1 < b[ib55].g.xE))
						{
							i56_found = ib55;
						}
					}
					
					bool bzero_pos = true;
					doublereal xg2 = xpos[0] - 0.5*fabs(xpos[1] - xpos[0]);
					if (i55_found>0) {
						xg2 = 0.5*(xg + xpos[i55_found - 1]);
						bzero_pos = false;
					}
					
					printf("xg2=%e\n", xg2);
					integer i57_found = -2;
					for (integer ib57 = 0; ib57 < lb; ib57++) {
						if ((zc>b[ib57].g.zS) && (zc<b[ib57].g.zE) && (yc>b[ib57].g.yS) && (yc<b[ib57].g.yE) && (xg2>b[ib57].g.xS) && (xg2 < b[ib57].g.xE))
						{
							if (xg2 > xpos[0]) {
								i57_found = ib57;
							}
						}
					}


					if (i56_found >= 0) {
						if (b[i56_found].itype == SOLID) {
							// TODO 11.07.2016
							if ((i57_found >= 0) && (b[i57_found].itype == SOLID)) {
								// �� �������� �������� ����� � ���� � ������� �����������������.
								// comparison_lam ����� ������ ���� ���������������� ����� i56 ������.
								if (comparison_lam(matlist, b, i56_found, i57_found, 25.0)) {
									// � ����� i56 ���������������� ����.
									s[i].g.xS = xg1;
									s[i].g.xE = xg1;
									addboundary(xpos, inz, xg1);
								}
								else {
									// � ����� i57 ���������������� ����.
									s[i].g.xS = xg2;
									s[i].g.xE = xg2;
									addboundary(xpos, inz, xg2);
								}
							}
							else {
								// ������ ����� ����.
								printf("xg1==%e\n", xg1);
								doublereal xgolg = xpos[1];
								if (inx == 1) {
									// ������� ������������� ��������� �����.
									xgolg = 0.5*(xpos[0] + xg1);
									addboundary(xpos, inx, xgolg);
									xgolg = 0.5*(xpos[1] + xg1);
									addboundary(xpos, inx, xgolg);
									xgolg = 0.5*(xpos[0] + xg1);
									// ���������� �� ���� ������ ��������� ������� ��� ������.
									s[i].g.xS = xgolg;
									s[i].g.xE = xgolg;
								}
								else {
									if (bzero_pos) {
										// ������� ������������� ��������� �����.
										xgolg = 0.5*(xpos[0] + xg1);
										addboundary(xpos, inx, xgolg);
										xgolg = 0.5*(xpos[1] + xg1);
										addboundary(xpos, inx, xgolg);
										xgolg = 0.5*(xpos[0] + xg1);
										// ���������� �� ���� ������ ��������� ������� ��� ������.
										s[i].g.xS = xgolg;
										s[i].g.xE = xgolg;
									}
									else {
										s[i].g.xS = xg1;
										s[i].g.xE = xg1;
									}
								}
								addboundary(xpos, inx, xg1);
								
							}
						}
						else {
							
							if (i57_found >= 0) {
								if (b[i57_found].itype == SOLID) {
									// ������ ����� ����.
									s[i].g.xS = xg2;
									s[i].g.xE = xg2;
									addboundary(xpos, inx, xg2);
								}
							}
							else {
								printf("ERROR : sourse na granice dvus hollow or fluid blockov.");
								system("PAUSE");
								exit(1);
							}
						}
					}
				}
				else {
					doublereal xg2 = 0.5*(xg + xpos[i55_found - 1]);
					printf("xg2=%e\n", xg2);
					integer i57_found = -2;
					for (integer ib57 = 0; ib57 < lb; ib57++) {
						if ((zc>b[ib57].g.zS) && (zc<b[ib57].g.zE) && (yc>b[ib57].g.yS) && (yc<b[ib57].g.yE) && (xg2>b[ib57].g.xS) && (xg2 < b[ib57].g.xE))
						{
							i57_found = ib57;
						}
					}
					if (i57_found >= 0) {
						if (b[i57_found].itype == SOLID) {
							// ������ ����� ����.
							s[i].g.xS = xg2;
							s[i].g.xE = xg2;
							addboundary(xpos, inx, xg2);
						}
					}
					else {
						printf("ERROR : sourse na granice dvus hollow or fluid blockov.");
						system("PAUSE");
						exit(1);
					}
				}
			}
		}
	}
	//BubbleEnhSort(xpos, inx);
	Sort_method(xpos, inx);

	integer iny_fix = iny;
	// ������������� ��������� XZ
	for (i = 0; i < ls; i++) {
		if (source_indexpopadaniqnagranXZ[i]) {
			doublereal xc = 0.5*(s[i].g.xS + s[i].g.xE);
			doublereal zc = 0.5*(s[i].g.zS + s[i].g.zE);
			doublereal yg = s[i].g.yS;
			// ����� ������� +Z �� ����� zpos
			// ����� ����� ��.
			// ���� �� ����������� Solid block �� �������� ������� � ���� ���� ������.
			// ��������� maxsize ratio 2 �� ������.
			// ���� ������� ��������� �� �� -Z � ������� ����.
			integer i55_found = -2;
			for (integer i55 = 0; i55 <= iny_fix; i55++) {
				if (fabs(ypos[i55] - yg) < 1.0e-40) {
					i55_found = i55;
					break;
				}
			}
			if (i55_found >= 0) {
				if (i55_found < iny_fix) {
					doublereal yg1 = 0.5*(yg + ypos[i55_found + 1]);
					printf("yg1=%e\n", yg1);
					integer i56_found = -2;
					for (integer ib55 = 0; ib55 < lb; ib55++) {
						if ((xc>b[ib55].g.xS) && (xc<b[ib55].g.xE) && (zc>b[ib55].g.zS) && (zc<b[ib55].g.zE) && (yg1>b[ib55].g.yS) && (yg1 < b[ib55].g.yE))
						{
							i56_found = ib55;
						}
					}
					doublereal yg2 = 0.5*(yg + ypos[i55_found - 1]);
					printf("yg2=%e\n", yg2);
					integer i57_found = -2;
					for (integer ib57 = 0; ib57 < lb; ib57++) {
						if ((xc>b[ib57].g.xS) && (xc<b[ib57].g.xE) && (zc>b[ib57].g.zS) && (zc<b[ib57].g.zE) && (yg2>b[ib57].g.yS) && (yg2 < b[ib57].g.yE))
						{
							if (yg2 > ypos[0]) {
								i57_found = ib57;
							}
						}
					}


					if (i56_found >= 0) {
						if (b[i56_found].itype == SOLID) {
							
							if ((i57_found >= 0) && (b[i57_found].itype == SOLID)) {
								// �� �������� �������� ����� � ���� � ������� �����������������.
								// comparison_lam ����� ������ ���� ���������������� ����� i56 ������.
								if (comparison_lam(matlist, b, i56_found, i57_found, 25.0)) {
									// � ����� i56 ���������������� ����.
									s[i].g.yS = yg1;
									s[i].g.yE = yg1;
									addboundary(ypos, iny, yg1);
								}
								else {
									// � ����� i57 ���������������� ����.
									s[i].g.yS = yg2;
									s[i].g.yE = yg2;
									addboundary(ypos, iny, yg2);
								}
							}
							else {
								// ������ ����� ����.
								s[i].g.yS = yg1;
								s[i].g.yE = yg1;
								addboundary(ypos, iny, yg1);
							}
						}
						else {
							
							if (i57_found >= 0) {
								if (b[i57_found].itype == SOLID) {
									// ������ ����� ����.
									s[i].g.yS = yg2;
									s[i].g.yE = yg2;
									addboundary(ypos, iny, yg2);
								}
							}
							else {
								printf("ERROR : sourse na granice dvus hollow or fluid blockov.");
								system("PAUSE");
								exit(1);
							}
						}
					}
				}
				else {
					doublereal yg2 = 0.5*(yg + ypos[i55_found - 1]);
					printf("yg2=%e\n", yg2);
					integer i57_found = -2;
					for (integer ib57 = 0; ib57 < lb; ib57++) {
						if ((xc>b[ib57].g.xS) && (xc<b[ib57].g.xE) && (zc>b[ib57].g.zS) && (zc<b[ib57].g.zE) && (yg2>b[ib57].g.yS) && (yg2 < b[ib57].g.yE))
						{
							i57_found = ib57;
						}
					}
					if (i57_found >= 0) {
						if (b[i57_found].itype == SOLID) {
							// ������ ����� ����.
							s[i].g.yS = yg2;
							s[i].g.yE = yg2;
							addboundary(ypos, iny, yg2);
						}
					}
					else {
						printf("ERROR : sourse na granice dvus hollow or fluid blockov.");
						system("PAUSE");
						exit(1);
					}
				}
			}
		}
	}
	//BubbleEnhSort(ypos, iny);
	Sort_method(ypos, iny);


	// ��������� �������� ����� �� ������� �������� � ��������� 30.0
	// ��� �� flowvision.
	quolite_refinement(inx, iny, inz, xpos, ypos, zpos);

	// debug

	//for (i=0; i<=inz; i++) {
	//printf("%e  ",zpos[i]);
	//}
	//getchar();//

  
	/*// debug
	for (i=0; i<=inz; i++) {
        printf("%f  ",zpos[i]);   
	}
	getchar();//*/

    // ������������ ����������� ������.
	delete[] rxboundary;
	delete[] ryboundary;
	delete[] rzboundary;
	delete[] ixintervalcount;
	delete[] iyintervalcount;
    delete[] izintervalcount;
	delete[] source_indexpopadaniqnagranXY;
	delete[] source_indexpopadaniqnagranYZ;
	delete[] source_indexpopadaniqnagranXZ;

} // simplemeshgen

/*
// ���������� ���������� �� ���� �����
doublereal fmax(doublereal fA, doublereal fB) {
	doublereal r=fA;
	if (fB>r) r=fB;
	return r;
} // fmax
*/

// ������������ �������� n2 ��� ����� ������ 
// ����������� � �������� ��������� ����� �������� ���������� �������.
integer ibalancen2(integer n2param, doublereal qL, doublereal qR, doublereal *rboundary,
	           integer intervalcount, integer i, integer iposmark) {

		// i - ����� ��������� ������� �������� ���������,

        bool bcontinue=true;
		integer n2=n2param;
		doublereal qgold=fmax(qL,qR);
		// ������ �� ������������ :
		integer ibound=0, imax=1000; // ����������� �� ������������ ����� ��������.

		// ������������ ������������ :
		while ((ibound<imax) && (bcontinue)) 
		{	    

		     doublereal qn2L=qL;
		     for (integer i1=1; i1<n2; i1++) qn2L*=qL; // ���������� � �������.
		     doublereal b1L=(rboundary[i+1]-rboundary[i])*(qL-1.0)/(2.0*(qn2L-1.0));
		     doublereal qn2R=qR;
		     for (integer i1=1; i1<(intervalcount-n2); i1++) qn2R*=qR; // ���������� � �������.
             doublereal b1R=(rboundary[i+1]-rboundary[i])*(qR-1.0)/(2.0*(qn2R-1.0));


		     doublereal rpos1, rpos2, rpos3;
        
             for (integer k=iposmark; k<=iposmark+intervalcount-2; k++)
		     {
			      // ������������� �����
			      integer ic1=k-iposmark;
			      doublereal madd;
			      if (ic1<n2) {
				      // ������� ����� �������.
				      madd=b1L;
				      for (integer i1=0; i1<ic1; i1++) madd*=qL;
					  if (ic1==n2-1) madd=0.5*(rboundary[i+1]+rboundary[i])-rpos3; // ����� �������� ������ ����������
			      } else 
			      {
				      // ����� ������ �������.
				      madd=b1R;
				      for (integer i1=intervalcount-1-ic1; i1>0; i1--) madd*=qR;
			      }
			      if (k==iposmark) {
				     rpos1=rboundary[i];
				     rpos2=rpos1;
				     rpos3=rpos1;
			      }
			      else {
				     rpos1=rpos2;
				     rpos2=rpos3;
				     rpos3=rpos3+madd;
			      }
			      if (ic1==n2) {
				      if (((rpos3-rpos2)/(rpos2-rpos1)<=qgold) && ((rpos3-rpos2)/(rpos2-rpos1)>=1.0/qgold)) {
					      bcontinue=false;
				      } else if ((rpos3-rpos2)/(rpos2-rpos1)<1.0/qgold) {
					      // ������� ����������� ������ �������
					      // ���� ��������� n2;
					      n2+=2;
						  if (n2>intervalcount-4) bcontinue=false;
				      }
					  else if ((rpos3-rpos2)/(rpos2-rpos1)>qgold) {
						  // ������� ����������� ������ �������
						  // ���� ��������� n2;
						  n2-=2;
						  if (n2<5) bcontinue=false;
					  }
				      break; // ����� �� ����� for
			      }
		     }
			 ibound++; // ������������ �� ������������ ����� ��������.
		} // end while

    return n2;  
} // ibalancen2

/* ���������� ���������� ��������� ����� hex cartesian - 
 * ����������������� ������������� ����� � ��������� ��������� 
 * ������� � ���� �������. ����������� ������� ���������� � ���
 * ��� �� ����� ������������ ��� ����������� ����� ��� � �������������.
 * ������ �������������� �������� ��������� �������� ������ ����������� ���������� ���
 * �������������� �������� ��������� simplemeshgen. 
 * ���� ���������� 8 ������� 2012.
*/
void unevensimplemeshgen(doublereal* &xpos, doublereal* &ypos, doublereal* &zpos, integer &inx, integer &iny, integer &inz,
	integer lb, integer ls, integer lw, BLOCK* b, SOURCE* s, WALL* w, integer lu, UNION* &my_union, 
	TPROP* matlist, doublereal dgx, doublereal dgy, doublereal dgz,
	doublereal* &xposadd, doublereal* &yposadd, doublereal* &zposadd,
	integer &inxadd, integer &inyadd, integer &inzadd, integer &iunion_id_p1)
{

	//*****************************************************************************************************************
	// ���������� ��������� ��������������� ��������� ����������
	doublereal deltavolkov = 0.1; // ������������� ����� � ���� �� �������.
	//printf("incomming\n");
	//system("pause");

	// ���� bgeom == true, �� ������������ ������������� ����� �� ������ �������������� ����������.
	bool bgeomx=false; // �� ��� Ox
	bool bgeomy=false; // �� ��� Oy
	bool bgeomz=true; // �� ��� Oz
    // 1.05 - ����� ����� �������������, 1.1, 1.2 - �������� �������������, 1.25, 1.5, 2 - ������ �������������.
	// ��� ������������������ ������� �������������� ����������� �������������� ���������� ������ ���� �� ������ 1.3.
	doublereal qxW=1.0005; // �������� �������� � ����� West �������. 
	doublereal qxE=1.0005; // �������� �������� � ������ East �������
	doublereal qyS=1.0005; // South
	doublereal qyN=1.0005; // North
	doublereal qzB=1.35; // Bottom
	doublereal qzT=1.00005; // Top
	//doublereal qzB=1.00005; // Bottom
	//doublereal qzT=1.35; // Top
	//*****************************************************************************************************************
	
	// ��������� ���� ����-������ ��� ������ ����� ������.
	if (lb == 1) {
		if (dgx*dgx + dgy*dgy + dgz*dgz > 1.0e-20) {

			// �� �������� 18.02.2017.
			//printf("uneven Davis mesh\n");
			bgeomx = true; // �� ��� Ox
			bgeomy = true; // �� ��� Oy
			bgeomz = true; // �� ��� Oz
								// 1.05 - ����� ����� �������������, 1.1, 1.2 - �������� �������������, 1.25, 1.5, 2 - ������ �������������.
								// ��� ������������������ ������� �������������� ����������� �������������� ���������� ������ ���� �� ������ 1.3.
			// 1.35
			doublereal qxW = 1.2; // �������� �������� � ����� West �������. 
			doublereal qxE = 1.2; // �������� �������� � ������ East �������
			doublereal qyS = 1.2; // South
			doublereal qyN = 1.2; // North
			doublereal qzB = 1.2; // Bottom
			doublereal qzT = 1.2; // Top
		}
	}

	if (lb == 1) {
		// 20.05.2017 �.�. ������
		bgeomx = false; // �� ��� Ox
		bgeomy = false; // �� ��� Oy
		bgeomz = false; // �� ��� Oz
	}

	//bool brepeat = true;

	// �� ��� Ox
	doublereal *rxboundary = NULL; // ������ ������������ ������
	integer inumboundaryx = 1;

	doublereal *ryboundary = NULL; // ������ ������������ ������
	integer inumboundaryy = 1;

	doublereal *rzboundary = NULL; // ������ ������������ ������
	integer inumboundaryz = 1;

	// ���������� � ���������� ������� minimum fluid gap.
	calc_minimum_fluid_gap1(inumboundaryx, rxboundary, inumboundaryy, ryboundary, inumboundaryz, rzboundary,
		lb, ls, lw, b, s, w, lu, my_union, iunion_id_p1);

	doublereal minimum_fluid_gap_x = 1.0e40;
	doublereal minimum_fluid_gap_y = 1.0e40;
	doublereal minimum_fluid_gap_z = 1.0e40;

	// ��������������� ���������� ������� minimum fluid gap.
	calc_minimum_fluid_gap2(inumboundaryx, rxboundary, inumboundaryy, ryboundary,
		inumboundaryz, rzboundary, minimum_fluid_gap_x, minimum_fluid_gap_y, minimum_fluid_gap_z,
		lb, ls, lw, b, s, w, lu, my_union, iunion_id_p1);


	bool *source_indexpopadaniqnagranYZ = NULL;
	bool *source_indexpopadaniqnagranXY = NULL;
	bool *source_indexpopadaniqnagranXZ = NULL;

	// 12.03.2017
	// ���������� snap to
	// ������������ ����������� �������� ������ �� 33%.
	snap_to_moving(source_indexpopadaniqnagranYZ,
		source_indexpopadaniqnagranXY,
		source_indexpopadaniqnagranXZ,
		rxboundary, ryboundary, rzboundary,
		inumboundaryx, inumboundaryy, inumboundaryz,
		minimum_fluid_gap_x, minimum_fluid_gap_y, minimum_fluid_gap_z,
		lb, ls, lw, b, s, w, lu, my_union, iunion_id_p1);

	integer i;

	

    integer *ixintervalcount; // ����� ����������
	ixintervalcount = new integer [inumboundaryx]; // �� ���� ������ ��� ����� ������.
	doublereal alphascal=1.0;
	integer inowintervalcount;
	for (i=0; i<(inumboundaryx); i++) {
         alphascal=(rxboundary[i+1]-rxboundary[i])/(rxboundary[inumboundaryx]-rxboundary[0]);
         inowintervalcount=(int)(alphascal*inx);
		 if (inowintervalcount < min_elem_in_x_element) inowintervalcount=min_elem_in_x_element;
         ixintervalcount[i]=inowintervalcount;
	}
    /* // debug
	for (i=0; i<(inumboundaryx); i++) {
	#if doubleintprecision == 1
		printf("%lld  ",ixintervalcount[i]);
	#else
		printf("%d  ",ixintervalcount[i]);
	#endif
        
	} //*/
    //getchar();

	// ���������� ������ ��������� �����.
	integer iposmark = 1;
	doublereal dx;
	integer k;
	integer ixoldsize=0;
	SetLength(xpos,ixoldsize,1);
    ixoldsize=1;
	for (i=0; i<(inumboundaryx); i++) 
	{
		if ((ixintervalcount[i]-2)%2==0) ixintervalcount[i]++; // ����� ����� ����� ���� ������ ������.
		integer n2=(int)((ixintervalcount[i]-1)/2); // ���������� � ������� �������
		if (bgeomx) n2=ibalancen2(n2, qxW, qxE, rxboundary, ixintervalcount[i], i, iposmark); // ������������ 
		doublereal qn2W=qxW;
		for (integer i1=1; i1<n2; i1++) qn2W*=qxW; // ���������� � �������.
		doublereal b1W=(rxboundary[i+1]-rxboundary[i])*(qxW-1.0)/(2.0*(qn2W-1.0));
		doublereal qn2E=qxE;
		for (integer i1=1; i1<(ixintervalcount[i]-n2); i1++) qn2E*=qxE; // ���������� � �������.
		doublereal b1E=(rxboundary[i+1]-rxboundary[i])*(qxE-1.0)/(2.0*(qn2E-1.0));
		//printf("length=%e\n",(rxboundary[i+1]-rxboundary[i]));
		//getchar(); // OK

        dx=(rxboundary[i+1]-rxboundary[i])/(ixintervalcount[i]-1);
		SetLength(xpos,ixoldsize,(iposmark+ixintervalcount[i]));
        ixoldsize=(iposmark+ixintervalcount[i]);
#if doubleintprecision == 1
		//printf("%lld  ",ixoldsize);// debug
#else
		//printf("%d  ",ixoldsize);// debug
#endif
        
        for (k=iposmark; k<=iposmark+ixintervalcount[i]-2; k++)
		{
			if (!bgeomx) {
				// ����������� �����
				if (lb == 1) {

					doublereal betavolkov = 1.0 / (sqrt(1.0 - deltavolkov));
					doublereal nvolkow = ((doublereal)(k - iposmark)) / ((doublereal)(ixintervalcount[i] - 1.0));
					xpos[k] = (rxboundary[1] - rxboundary[0]) * fmax(0.0, (((betavolkov + 1.0)*(pow(((betavolkov + 1.0) / (betavolkov - 1.0)), 2.0*nvolkow - 1.0)) - (betavolkov - 1.0))));
					xpos[k] /= 2.0*(1.0 + pow((betavolkov + 1.0) / (betavolkov - 1.0), 2.0*nvolkow - 1.0));
					xpos[k] += rxboundary[0];
				}
				else {
					xpos[k] = rxboundary[i] + (k - iposmark)*dx;
				}
			}
			else
			{
				// ������������� �����
				integer ic1=k-iposmark;
				doublereal madd;
				if (ic1<n2) {
					// ������� ����� �������.
					madd=b1W;
					for (integer i1=1; i1<ic1; i1++) madd*=qxW; 
					if (ic1==n2-1) madd=0.5*(rxboundary[i+1]+rxboundary[i])-xpos[k-1]; // ����� �������� ������ ����������
				} else 
				{
					// ����� ������ �������.
					madd=b1E;
					for (integer i1=ixintervalcount[i]-1-ic1; i1>0; i1--) madd*=qxE;
				}
				if (k==iposmark) xpos[k]=rxboundary[i];
				else xpos[k]=xpos[k-1]+madd;
				
			}
		}
		iposmark=iposmark+ixintervalcount[i]-1;
	}
	SetLength(xpos,ixoldsize,iposmark+1);
	xpos[iposmark]=rxboundary[inumboundaryx];
	inx=iposmark;
    for (i=0; i<inx; i++) xpos[i]=xpos[i+1]; // ����� ����� �� 1
    SetLength(xpos,inx+1,inx); 
	inx--; // ���������� ���������� � ���� � ������������� ��������� inx
	
	// 3 �������� 2017.
	// ���������� ����� �������� �����.
	// ������������� �� ���������� ����������� ����������� ���� �����.
	for (int i_28 = 0; i_28 <= inxadd; i_28++) {
		SetLength(xpos, inx + 1, inx + 2);
		xpos[inx + 1] = xposadd[i_28];
		inx++;
	}
	Sort_method(xpos, inx);

	for (i=0; i<adapt_x; i++) simplecorrect_meshgen_x(xpos, inx, lb, ls, lw, b, s, w);


	//*****************************************************************************
	// �������������, ����� ����� maxsize_ratio �� �������� 2.0.
	// ������������ ���������� ��� ���������� ����������������� �����.
	// 11.07.2016
	//const doublereal etalon_max_size_ratio=2.0;
	integer inum_iter_ratio_good = 0;
	while (1) {

		// ���������� max size ratio x axis.
		doublereal max_size_ratio_x = 1.0;
		for (i = 0; i<inx - 1; i++) {

			doublereal dmax = 0.0;
			doublereal dmin = 0.0;
			if (fabs(xpos[i + 1] - xpos[i])>fabs(xpos[i + 2] - xpos[i + 1])) {
				dmax = fabs(xpos[i + 1] - xpos[i]);
				dmin = fabs(xpos[i + 2] - xpos[i + 1]);
			}
			else {
				dmax = fabs(xpos[i + 2] - xpos[i + 1]);
				dmin = fabs(xpos[i + 1] - xpos[i]);
			}
			if (dmax / dmin>max_size_ratio_x) {
				max_size_ratio_x = dmax / dmin;
			}
		}
		printf("x axis max size ratio is equal = %1.4f\n", max_size_ratio_x);
		if (max_size_ratio_x != max_size_ratio_x) {
			system("PAUSE");
		}
		//getchar();

		if (max_size_ratio_x < (etalon_max_size_ratio*1.1)) {
			break;
		}

		// ������������� max size ratio x axis.
		// ������� 9 ������ 2013 ����.

		bool bplus = false;
		max_size_ratio_x = 1.0;
		for (i = 0; i<inx - 1; i++) {

			bplus = false;
			doublereal dmax = 0.0;
			doublereal dmin = 0.0;
			if (fabs(xpos[i + 1] - xpos[i])>fabs(xpos[i + 2] - xpos[i + 1])) {
				dmax = fabs(xpos[i + 1] - xpos[i]);
				dmin = fabs(xpos[i + 2] - xpos[i + 1]);
			}
			else {
				bplus = true;
				dmax = fabs(xpos[i + 2] - xpos[i + 1]);
				dmin = fabs(xpos[i + 1] - xpos[i]);
			}
			if (dmax / dmin>etalon_max_size_ratio) {
				doublereal pos_candidate;
				if (bplus) {
					pos_candidate = xpos[i + 1] + 0.5*dmax;
				}
				else {
					pos_candidate = xpos[i] + 0.5*dmax;
				}
				SetLength(xpos, inx + 1, inx + 2);
				xpos[inx + 1] = pos_candidate;
				inx = inx + 1;
				//BubbleEnhSort(xpos, inx);
				Sort_method(xpos, inx);
				break;
			}
		}

		inum_iter_ratio_good++;

		//getchar();

	}

#if doubleintprecision == 1
	printf("inum_iter_ratio_good is %lld\n", inum_iter_ratio_good);
#else
	printf("inum_iter_ratio_good is %d\n", inum_iter_ratio_good);
#endif
	
	if (bwait) {
		//getchar();
		system("pause");

	}

	// �������������, ����� ����� max size_ratio �� �������� 2.0.
	//*****************************************************************************
	

	/* // debug
	for (i=0; i<=inx; i++) {
        printf("%f  ",xpos[i]);   
	}
	getchar(); //*/ 


    // �� ��� Oy
    


    integer *iyintervalcount; // ����� ����������
    iyintervalcount = new integer [inumboundaryy]; // �� ���� ������ ��� ����� ������.
    for (i=0; i<(inumboundaryy); i++) {
         alphascal=(ryboundary[i+1]-ryboundary[i])/(ryboundary[inumboundaryy]-rxboundary[0]);
         inowintervalcount=(int)(alphascal*iny);
		 if (inowintervalcount < min_elem_in_y_element) inowintervalcount=min_elem_in_y_element; 
         iyintervalcount[i]=inowintervalcount;
	}
    // ���������� ������� �����
    iposmark = 1;
	doublereal dy;
    integer iyoldsize=0;
    SetLength(ypos,iyoldsize,1);
    iyoldsize=1;
	for (i=0; i<inumboundaryy; i++) 
	{

		// ����� ����� ����� ���� ������ ������
		if ((iyintervalcount[i]-2)%2==0) iyintervalcount[i]++;
		integer n2=(int)((iyintervalcount[i]-1)/2); // ���������� � ������� �������
		if (bgeomy) n2=ibalancen2(n2, qyS, qyN, ryboundary, iyintervalcount[i], i, iposmark); // ������������ 
		doublereal qn2S=qyS;
		for (integer i1=1; i1<n2; i1++) qn2S*=qyS; // ���������� � �������.
		doublereal b1S=(ryboundary[i+1]-ryboundary[i])*(qyS-1.0)/(2.0*(qn2S-1.0));
		doublereal qn2N=qyN;
		for (integer i1=1; i1<(iyintervalcount[i]-n2); i1++) qn2N*=qyN; // ���������� � �������.
		doublereal b1N=(ryboundary[i+1]-ryboundary[i])*(qyN-1.0)/(2.0*(qn2N-1.0));

        dy=(ryboundary[i+1]-ryboundary[i])/(iyintervalcount[i]-1);
		SetLength(ypos,iyoldsize,iposmark+iyintervalcount[i]);
        iyoldsize=iposmark+iyintervalcount[i];
        for (k=iposmark; k<=iposmark+iyintervalcount[i]-2; k++) 
		{
			if (!bgeomy) {
				if (lb == 1) {

					doublereal betavolkov = 1.0 / (sqrt(1.0 - deltavolkov));
					doublereal nvolkow = ((doublereal)(k - iposmark)) / ((doublereal)(iyintervalcount[i] - 1.0));
					ypos[k] = (ryboundary[1] - ryboundary[0]) * fmax(0.0, (((betavolkov + 1.0)*(pow(((betavolkov + 1.0) / (betavolkov - 1.0)), 2.0*nvolkow - 1.0)) - (betavolkov - 1.0))));
					ypos[k] /= 2.0*(1.0 + pow((betavolkov + 1.0) / (betavolkov - 1.0), 2.0*nvolkow - 1.0));
					ypos[k] += ryboundary[0];

				}
				else {
					// ����������� �����
					ypos[k] = ryboundary[i] + (k - iposmark)*dy;
				}
			}
			else 
			{
				// ������������� �����
				// �� ������ �������������� ���������� � ����� ����������.
				integer ic1=k-iposmark;
				doublereal madd;
				if (ic1<n2) {
					madd=b1S;
					for (integer i1=1; i1<ic1; i1++) madd*=qyS;
					if (ic1==n2-1) madd=0.5*(ryboundary[i+1]+ryboundary[i])-ypos[k-1];
				} else 
				{
					madd=b1N;
					for (integer i1=iyintervalcount[i]-1-ic1; i1>0; i1--) madd*=qyN;
				}
				if (k==iposmark) ypos[k]=ryboundary[i];
				else ypos[k]=ypos[k-1]+madd;
			}
		}
		iposmark=iposmark+iyintervalcount[i]-1;
	}
	SetLength(ypos,iyoldsize,iposmark+1);
	ypos[iposmark]=ryboundary[inumboundaryy];
	iny=iposmark;
    for (i=0; i<iny; i++) ypos[i]=ypos[i+1]; // ����� ����� �� 1
    SetLength(ypos,iny+1,iny); 
	iny--; // ���������� ���������� � ���� � ������������� ��������� iny
    
    // 3 �������� 2017.
	// ���������� ����� �������� �����.
	// ������������� �� ���������� ����������� ����������� ���� �����.
	for (int i_28 = 0; i_28 <= inyadd; i_28++) {
		SetLength(ypos, iny + 1, iny + 2);
		ypos[iny + 1] = yposadd[i_28];
		iny++;
	}
	Sort_method(ypos, iny);

    for (i=0; i<adapt_y; i++) simplecorrect_meshgen_y(ypos, iny, lb, ls, lw, b, s, w);


	inum_iter_ratio_good = 0;
	while (1) {

		doublereal max_size_ratio_y = 1.0;
		for (i = 0; i<iny - 1; i++) {

			doublereal dmax = 0.0;
			doublereal dmin = 0.0;
			if (fabs(ypos[i + 1] - ypos[i])>fabs(ypos[i + 2] - ypos[i + 1])) {
				dmax = fabs(ypos[i + 1] - ypos[i]);
				dmin = fabs(ypos[i + 2] - ypos[i + 1]);
			}
			else {
				dmax = fabs(ypos[i + 2] - ypos[i + 1]);
				dmin = fabs(ypos[i + 1] - ypos[i]);
			}
			if (dmax / dmin>max_size_ratio_y) {
				max_size_ratio_y = dmax / dmin;
			}
		}
		printf("y axis max size ratio is equal = %1.4f\n", max_size_ratio_y);
		if (max_size_ratio_y != max_size_ratio_y) {
			system("PAUSE");
		}
		//getchar();

		if (max_size_ratio_y < (etalon_max_size_ratio*1.1)) {
			break;
		}

		// ������������� max size ratio y axis.
		// ������� 9 ������ 2013 ����.

		bool bplus = false;
		max_size_ratio_y = 1.0;
		for (i = 0; i<iny - 1; i++) {

			bplus = false;
			doublereal dmax = 0.0;
			doublereal dmin = 0.0;
			if (fabs(ypos[i + 1] - ypos[i])>fabs(ypos[i + 2] - ypos[i + 1])) {
				dmax = fabs(ypos[i + 1] - ypos[i]);
				dmin = fabs(ypos[i + 2] - ypos[i + 1]);
			}
			else {
				bplus = true;
				dmax = fabs(ypos[i + 2] - ypos[i + 1]);
				dmin = fabs(ypos[i + 1] - ypos[i]);
			}
			if (dmax / dmin>etalon_max_size_ratio) {
				doublereal pos_candidate;
				if (bplus) {
					pos_candidate = ypos[i + 1] + 0.5*dmax;
				}
				else {
					pos_candidate = ypos[i] + 0.5*dmax;
				}
				SetLength(ypos, iny + 1, iny + 2);
				ypos[iny + 1] = pos_candidate;
				iny = iny + 1;
				//BubbleEnhSort(ypos, iny);
				Sort_method(ypos, iny);
				break;
			}
		}

		inum_iter_ratio_good++;

		//getchar();

	}

#if doubleintprecision == 1
	printf("inum_iter_ratio_good is %lld\n", inum_iter_ratio_good);
#else
	printf("inum_iter_ratio_good is %d\n", inum_iter_ratio_good);
#endif
	
	if (bwait) {
		//getchar();
		system("pause");
	}

	/* // debug
	for (i=0; i<=iny; i++) {
        printf("%f  ",ypos[i]);   
	}//*/

    // �� ��� Oz
     
   
      

    integer *izintervalcount; // ����� ����������
    izintervalcount = new integer [inumboundaryz]; // �� ���� ������ ��� ����� ������.
    for (i=0; i<(inumboundaryz); i++) {
         alphascal=(rzboundary[i+1]-rzboundary[i])/(rzboundary[inumboundaryz]-rzboundary[0]);
         inowintervalcount=(int)(alphascal*inz);
		 if (inowintervalcount < min_elem_in_z_element) inowintervalcount=min_elem_in_z_element;
         izintervalcount[i]=inowintervalcount;
	}
	
    // ���������� ������� �����
    iposmark = 1;
	doublereal dz;
    integer izoldsize=0;
    SetLength(zpos,izoldsize,1);
    izoldsize=1;
	for (i=0; i<(inumboundaryz); i++) 
	{
		// ����� ����� ����� ���� ������ ������
		if ((izintervalcount[i]-2)%2==0) izintervalcount[i]++;
		integer n2=(int)((izintervalcount[i]-1)/2); // ���������� � ������� ������� 
		if (bgeomz) n2=ibalancen2(n2, qzB, qzT, rzboundary, izintervalcount[i], i, iposmark); // ������������ 
#if doubleintprecision == 1
		//printf("n2=%lld\n",n2); // debug
#else
		//printf("n2=%d\n",n2); // debug
#endif
		
		doublereal qn2B=qzB; // ������ �������
		// ������� �������� � 0.25 �� ����� ���������� �� ��������� ��-�� ����������� ������ ����������.
		for (integer i1=1; i1<n2; i1++) qn2B*=qzB; // ���������� � �������.
		doublereal b1B=(rzboundary[i+1]-rzboundary[i])*(qzB-1.0)/(2.0*(qn2B-1.0));
		doublereal qn2T=qzT; // ������� �������
		for (integer i1=1; i1<(izintervalcount[i]-n2); i1++) qn2T*=qzT; // ���������� � �������.
		doublereal b1T=(rzboundary[i+1]-rzboundary[i])*(qzT-1.0)/(2.0*(qn2T-1.0)); 

        dz=(rzboundary[i+1]-rzboundary[i])/(izintervalcount[i]-1);
		SetLength(zpos,izoldsize,iposmark+izintervalcount[i]);
        izoldsize=iposmark+izintervalcount[i];
        for (k=iposmark; k<=iposmark+izintervalcount[i]-2; k++)
		{
			if (!bgeomz) {

				if (lb == 1) {

					doublereal betavolkov = 1.0 / (sqrt(1.0 - deltavolkov));
					doublereal nvolkow = ((doublereal)(k - iposmark)) / ((doublereal)(izintervalcount[i] - 1.0));
					zpos[k] = (rzboundary[1] - rzboundary[0]) * fmax(0.0, (((betavolkov + 1.0)*(pow(((betavolkov + 1.0) / (betavolkov - 1.0)), 2.0*nvolkow - 1.0)) - (betavolkov - 1.0))));
					zpos[k] /= 2.0*(1.0 + pow((betavolkov + 1.0) / (betavolkov - 1.0), 2.0*nvolkow - 1.0));
					zpos[k] += rzboundary[0];
					//printf("%e\n", zpos[k]);
					//getchar();

				}
				else {
					// ����������� �����
					zpos[k] = rzboundary[i] + (k - iposmark)*dz;
				}
			}
			else 
			{
				// ������������� �����
				// �� ������ �������������� ���������� � ����� ����������.
				integer ic1=k-iposmark;
				doublereal madd;
				if (ic1<n2) {
					madd=b1B;
					for (integer i1=0; i1<ic1; i1++) madd*=qzB; 
					if (ic1==n2-1) madd=0.5*(rzboundary[i+1]+rzboundary[i])-zpos[k-1]; // ����� �������� ������ ����������.
					//printf("first.:");
				} else 
				{
					madd=b1T;
					for (integer i1=izintervalcount[i]-1-ic1; i1>0; i1--) madd*=qzT;
					// ��� ���� �������������.
					//if (k==iposmark+izintervalcount[i]-2) madd*=rzboundary[i+1]-zpos[k-1]; // ����� �������� ������ ����������.
					//printf("second.:");
				}
				if (k==iposmark) zpos[k]=rzboundary[i]; // ����� �������� ������ ����������.
				else {
					// ����������� ��� ��� ������� �� ������� �� ��������������
					zpos[k]=zpos[k-1]+madd;
				}
#if doubleintprecision == 1
		//printf("zpos[%lld]=%f, madd=%f\n",k,zpos[k],madd);
#else
		//printf("zpos[%d]=%f, madd=%f\n",k,zpos[k],madd);
#endif
				
				//getchar();
			}
		}
		iposmark=iposmark+izintervalcount[i]-1;
	}
	SetLength(zpos,izoldsize,iposmark+1);
	zpos[iposmark]=rzboundary[inumboundaryz];
#if doubleintprecision == 1
	//printf("finish zpos[%lld]=%f\n",iposmark,zpos[iposmark]);
#else
	//printf("finish zpos[%d]=%f\n",iposmark,zpos[iposmark]);
#endif
	
	//getchar();
	inz=iposmark;
	for (i=0; i<inz; i++) zpos[i]=zpos[i+1]; // ����� ����� �� 1
    SetLength(zpos,inz+1,inz); 
	inz--; // ���������� ���������� � ���� � ������������� ��������� inz

	// 3 �������� 2017.
	// ���������� ����� �������� �����.
	// ������������� �� ���������� ����������� ����������� ���� �����.
	for (int i_28 = 0; i_28 <= inzadd; i_28++) {
		SetLength(zpos, inz + 1, inz + 2);
		zpos[inz + 1] = zposadd[i_28];
		inz++;
	}
	Sort_method(zpos, inz);

	for (i=0; i<adapt_z; i++) simplecorrect_meshgen_z(zpos, inz, lb, ls, lw, b, s, w);
	
	inum_iter_ratio_good = 0;
	while (1) {

		doublereal max_size_ratio_z = 1.0;
		for (i = 0; i<inz - 1; i++) {

			doublereal dmax = 0.0;
			doublereal dmin = 0.0;
			if (fabs(zpos[i + 1] - zpos[i])>fabs(zpos[i + 2] - zpos[i + 1])) {
				dmax = fabs(zpos[i + 1] - zpos[i]);
				dmin = fabs(zpos[i + 2] - zpos[i + 1]);
			}
			else {
				dmax = fabs(zpos[i + 2] - zpos[i + 1]);
				dmin = fabs(zpos[i + 1] - zpos[i]);
			}
			if (dmax / dmin>max_size_ratio_z) {
				max_size_ratio_z = dmax / dmin;
			}
		}
		printf("z axis max size ratio is equal = %1.4f\n", max_size_ratio_z);
		if (max_size_ratio_z != max_size_ratio_z) {
			system("PAUSE");
		}
		//getchar();

		if (max_size_ratio_z < (etalon_max_size_ratio*1.1)) {
			break;
		}

		// ������������� max size ratio z axis.
		// ������� 9 ������ 2013 ����.

		bool bplus = false;
		max_size_ratio_z = 1.0;
		for (i = 0; i<inz - 1; i++) {

			bplus = false;
			doublereal dmax = 0.0;
			doublereal dmin = 0.0;
			if (fabs(zpos[i + 1] - zpos[i])>fabs(zpos[i + 2] - zpos[i + 1])) {
				dmax = fabs(zpos[i + 1] - zpos[i]);
				dmin = fabs(zpos[i + 2] - zpos[i + 1]);
			}
			else {
				bplus = true;
				dmax = fabs(zpos[i + 2] - zpos[i + 1]);
				dmin = fabs(zpos[i + 1] - zpos[i]);
			}
			if (dmax / dmin>etalon_max_size_ratio) {
				doublereal pos_candidate;
				if (bplus) {
					pos_candidate = zpos[i + 1] + 0.5*dmax;
				}
				else {
					pos_candidate = zpos[i] + 0.5*dmax;
				}
				SetLength(zpos, inz + 1, inz + 2);
				zpos[inz + 1] = pos_candidate;
				inz = inz + 1;
				//BubbleEnhSort(zpos, inz);
				Sort_method(zpos, inz);
				break;
			}
		}

		inum_iter_ratio_good++;

		//getchar();

	}
#if doubleintprecision == 1
	printf("inum_iter_ratio_good is %lld\n", inum_iter_ratio_good);
#else
	printf("inum_iter_ratio_good is %d\n", inum_iter_ratio_good);
#endif
	
	if (bwait) {
		// getchar();
		system("pause");

	}

	//for (i = 0; i <= inz; i++) {
	//printf("%e  ", zpos[i]);
	//}
	//getchar();

	integer inz_fix = inz;
	// ������������� ��������� XY
	for (i = 0; i < ls; i++) {
		if (source_indexpopadaniqnagranXY[i]) {
			doublereal xc = 0.5*(s[i].g.xS + s[i].g.xE);
			doublereal yc = 0.5*(s[i].g.yS + s[i].g.yE);
			doublereal zg = s[i].g.zS;
			// ����� ������� +Z �� ����� zpos
			// ����� ����� ��.
			// ���� �� ����������� Solid block �� �������� ������� � ���� ���� ������.
			// ��������� maxsize ratio 2 �� ������.
			// ���� ������� ��������� �� �� -Z � ������� ����.
			integer i55_found = -2;
			for (integer i55 = 0; i55 <= inz_fix; i55++) {
				if (fabs(zpos[i55] - zg) < 1.0e-40) {
					i55_found = i55;
					break;
				}
			}
			if (i55_found >= 0) {
				if (i55_found < inz_fix) {
					doublereal zg1 = 0.5*(zg + zpos[i55_found + 1]);
					printf("zg1=%e\n", zg1);
					integer i56_found = -2;
					for (integer ib55 = 0; ib55 < lb; ib55++) {
						if ((xc>b[ib55].g.xS) && (xc<b[ib55].g.xE) && (yc>b[ib55].g.yS) && (yc<b[ib55].g.yE) && (zg1>b[ib55].g.zS) && (zg1 < b[ib55].g.zE))
						{
							i56_found = ib55;
						}
					}
					
					bool bzero_pos = true;
					doublereal zg2 = zpos[0] - 0.5*fabs(zpos[1] - zpos[0]);
					if (i55_found>0) {
						zg2 = 0.5*(zg + zpos[i55_found - 1]);
						bzero_pos = false;
					}

					printf("zg2=%e\n", zg2);
					integer i57_found = -2;
					for (integer ib57 = 0; ib57 < lb; ib57++) {
						if ((xc>b[ib57].g.xS) && (xc<b[ib57].g.xE) && (yc>b[ib57].g.yS) && (yc<b[ib57].g.yE) && (zg2>b[ib57].g.zS) && (zg2 < b[ib57].g.zE))
						{
							i57_found = ib57;
						}
					}

					if (i56_found >= 0) {
						if (b[i56_found].itype == SOLID) {
							if ((i57_found >= 0) && (b[i57_found].itype == SOLID)) {
								// �� �������� �������� ����� � ���� � ������� �����������������.
								// comparison_lam ����� ������ ���� ���������������� ����� i56 ������.
								if (comparison_lam(matlist, b, i56_found, i57_found, 25.0)) {
									// � ����� i56 ���������������� ����.
									s[i].g.zS = zg1;
									s[i].g.zE = zg1;
									addboundary(zpos, inz, zg1);
								}
								else {
									// � ����� i57 ���������������� ����.
									s[i].g.zS = zg2;
									s[i].g.zE = zg2;
									addboundary(zpos, inz, zg2);
								}
							}
							else {
								// ������ ����� ����.
								printf("zg1==%e\n", zg1);
								doublereal zgolg = zpos[1];
								if (inz == 1) {
									// ������� ������������� ��������� �����.
									zgolg = 0.5*(zpos[0] + zg1);
									addboundary(zpos, inz, zgolg);
									zgolg = 0.5*(zpos[1] + zg1);
									addboundary(zpos, inz, zgolg);
									zgolg = 0.5*(zpos[0] + zg1);
									// ���������� �� ���� ������ ��������� ������� ��� ������.
									s[i].g.zS = zgolg;
									s[i].g.zE = zgolg;
								}
								else {
									if (bzero_pos) {
										// ������� ������������� ��������� �����.
										zgolg = 0.5*(zpos[0] + zg1);
										addboundary(zpos, inz, zgolg);
										zgolg = 0.5*(zpos[1] + zg1);
										addboundary(zpos, inz, zgolg);
										zgolg = 0.5*(zpos[0] + zg1);
										// ���������� �� ���� ������ ��������� ������� ��� ������.
										s[i].g.zS = zgolg;
										s[i].g.zE = zgolg;
									}
									else {
										s[i].g.zS = zg1;
										s[i].g.zE = zg1;
									}
								}
								addboundary(zpos, inz, zg1);
								
							}
						}
						else {

							if (i57_found >= 0) {
								if (b[i57_found].itype == SOLID) {
									// ������ ����� ����.
									s[i].g.zS = zg2;
									s[i].g.zE = zg2;
									addboundary(zpos, inz, zg2);
								}
							}
							else {
								printf("ERROR : sourse na granice dvus hollow or fluid blockov.");
								system("PAUSE");
								exit(1);
							}
						}
					}
				}
				else {
					doublereal zg2 = 0.5*(zg + zpos[i55_found - 1]);
					printf("zg2=%e\n", zg2);
					integer i57_found = -2;
					for (integer ib57 = 0; ib57 < lb; ib57++) {
						if ((xc>b[ib57].g.xS) && (xc<b[ib57].g.xE) && (yc>b[ib57].g.yS) && (yc<b[ib57].g.yE) && (zg2>b[ib57].g.zS) && (zg2 < b[ib57].g.zE))
						{
							i57_found = ib57;
						}
					}
					if (i57_found >= 0) {
						if (b[i57_found].itype == SOLID) {
							// ������ ����� ����.
							s[i].g.zS = zg2;
							s[i].g.zE = zg2;
							addboundary(zpos, inz, zg2);
						}
					}
					else {
						printf("ERROR : sourse na granice dvus hollow or fluid blockov.");
						system("PAUSE");
						exit(1);
					}
				}
			}
		}
	}
	//BubbleEnhSort(zpos, inz);
	Sort_method(zpos, inz);
	//SetLength(zpos, inz + 2, inz);
	//inz++;

	integer inx_fix = inx;
	// ������������� ��������� YZ
	for (i = 0; i < ls; i++) {
		if (source_indexpopadaniqnagranYZ[i]) {
			doublereal zc = 0.5*(s[i].g.zS + s[i].g.zE);
			doublereal yc = 0.5*(s[i].g.yS + s[i].g.yE);
			doublereal xg = s[i].g.xS;
			// ����� ������� +Z �� ����� zpos
			// ����� ����� ��.
			// ���� �� ����������� Solid block �� �������� ������� � ���� ���� ������.
			// ��������� maxsize ratio 2 �� ������.
			// ���� ������� ��������� �� �� -Z � ������� ����.
			integer i55_found = -2;
			for (integer i55 = 0; i55 <= inx_fix; i55++) {
				if (fabs(xpos[i55] - xg) < 1.0e-40) {
					i55_found = i55;
					break;
				}
			}
			if (i55_found >= 0) {
				if (i55_found < inx_fix) {
					doublereal xg1 = 0.5*(xg + xpos[i55_found + 1]);
					printf("xg1=%e\n", xg1);
					integer i56_found = -2;
					for (integer ib55 = 0; ib55 < lb; ib55++) {
						if ((zc>b[ib55].g.zS) && (zc<b[ib55].g.zE) && (yc>b[ib55].g.yS) && (yc<b[ib55].g.yE) && (xg1>b[ib55].g.xS) && (xg1 < b[ib55].g.xE))
						{
							i56_found = ib55;
						}
					}
					
					bool bzero_pos = true;
					doublereal xg2 = xpos[0] - 0.5*fabs(xpos[1] - xpos[0]);
					if (i55_found>0) {
						xg2 = 0.5*(xg + xpos[i55_found - 1]);
						bzero_pos = false;
					}


					printf("xg2=%e\n", xg2);
					integer i57_found = -2;
					for (integer ib57 = 0; ib57 < lb; ib57++) {
						if ((zc>b[ib57].g.zS) && (zc<b[ib57].g.zE) && (yc>b[ib57].g.yS) && (yc<b[ib57].g.yE) && (xg2>b[ib57].g.xS) && (xg2 < b[ib57].g.xE))
						{
							i57_found = ib57;
						}
					}


					if (i56_found >= 0) {
						if (b[i56_found].itype == SOLID) {
							// TODO 11.07.2016
							if ((i57_found >= 0) && (b[i57_found].itype == SOLID)) {
								// �� �������� �������� ����� � ���� � ������� �����������������.
								// comparison_lam ����� ������ ���� ���������������� ����� i56 ������.
								if (comparison_lam(matlist, b, i56_found, i57_found, 25.0)) {
									// � ����� i56 ���������������� ����.
									s[i].g.xS = xg1;
									s[i].g.xE = xg1;
									addboundary(xpos, inz, xg1);
								}
								else {
									// � ����� i57 ���������������� ����.
									s[i].g.xS = xg2;
									s[i].g.xE = xg2;
									addboundary(xpos, inz, xg2);
								}
							}
							else {
								// ������ ����� ����.
								printf("xg1==%e\n", xg1);
								doublereal xgolg = xpos[1];
								if (inx == 1) {
									// ������� ������������� ��������� �����.
									xgolg = 0.5*(xpos[0] + xg1);
									addboundary(xpos, inx, xgolg);
									xgolg = 0.5*(xpos[1] + xg1);
									addboundary(xpos, inx, xgolg);
									xgolg = 0.5*(xpos[0] + xg1);
									// ���������� �� ���� ������ ��������� ������� ��� ������.
									s[i].g.xS = xgolg;
									s[i].g.xE = xgolg;
								}
								else {
									if (bzero_pos) {
										// ������� ������������� ��������� �����.
										xgolg = 0.5*(xpos[0] + xg1);
										addboundary(xpos, inx, xgolg);
										xgolg = 0.5*(xpos[1] + xg1);
										addboundary(xpos, inx, xgolg);
										xgolg = 0.5*(xpos[0] + xg1);
										// ���������� �� ���� ������ ��������� ������� ��� ������.
										s[i].g.xS = xgolg;
										s[i].g.xE = xgolg;
									}
									else {
										s[i].g.xS = xg1;
										s[i].g.xE = xg1;
									}
								}
								addboundary(xpos, inx, xg1);
								

							}
						}
						else {

							if (i57_found >= 0) {
								if (b[i57_found].itype == SOLID) {
									// ������ ����� ����.
									s[i].g.xS = xg2;
									s[i].g.xE = xg2;
									addboundary(xpos, inx, xg2);
								}
							}
							else {
								printf("ERROR : sourse na granice dvus hollow or fluid blockov.");
								system("PAUSE");
								exit(1);
							}
						}
					}
				}
				else {
					doublereal xg2 = 0.5*(xg + xpos[i55_found - 1]);
					printf("xg2=%e\n", xg2);
					integer i57_found = -2;
					for (integer ib57 = 0; ib57 < lb; ib57++) {
						if ((zc>b[ib57].g.zS) && (zc<b[ib57].g.zE) && (yc>b[ib57].g.yS) && (yc<b[ib57].g.yE) && (xg2>b[ib57].g.xS) && (xg2 < b[ib57].g.xE))
						{
							i57_found = ib57;
						}
					}
					if (i57_found >= 0) {
						if (b[i57_found].itype == SOLID) {
							// ������ ����� ����.
							s[i].g.xS = xg2;
							s[i].g.xE = xg2;
							addboundary(xpos, inx, xg2);
						}
					}
					else {
						printf("ERROR : sourse na granice dvus hollow or fluid blockov.");
						system("PAUSE");
						exit(1);
					}
				}
			}
		}
	}
	//BubbleEnhSort(xpos, inx);
	Sort_method(xpos, inx);

	integer iny_fix = iny;
	// ������������� ��������� XZ
	for (i = 0; i < ls; i++) {
		if (source_indexpopadaniqnagranXZ[i]) {
			doublereal xc = 0.5*(s[i].g.xS + s[i].g.xE);
			doublereal zc = 0.5*(s[i].g.zS + s[i].g.zE);
			doublereal yg = s[i].g.yS;
			// ����� ������� +Z �� ����� zpos
			// ����� ����� ��.
			// ���� �� ����������� Solid block �� �������� ������� � ���� ���� ������.
			// ��������� maxsize ratio 2 �� ������.
			// ���� ������� ��������� �� �� -Z � ������� ����.
			integer i55_found = -2;
			for (integer i55 = 0; i55 <= iny_fix; i55++) {
				if (fabs(ypos[i55] - yg) < 1.0e-40) {
					i55_found = i55;
					break;
				}
			}
			if (i55_found >= 0) {
				if (i55_found < iny_fix) {
					doublereal yg1 = 0.5*(yg + ypos[i55_found + 1]);
					printf("yg1=%e\n", yg1);
					integer i56_found = -2;
					for (integer ib55 = 0; ib55 < lb; ib55++) {
						if ((xc>b[ib55].g.xS) && (xc<b[ib55].g.xE) && (zc>b[ib55].g.zS) && (zc<b[ib55].g.zE) && (yg1>b[ib55].g.yS) && (yg1 < b[ib55].g.yE))
						{
							i56_found = ib55;
						}
					}
					doublereal yg2 = 0.5*(yg + ypos[i55_found - 1]);
					printf("yg2=%e\n", yg2);
					integer i57_found = -2;
					for (integer ib57 = 0; ib57 < lb; ib57++) {
						if ((xc>b[ib57].g.xS) && (xc<b[ib57].g.xE) && (zc>b[ib57].g.zS) && (zc<b[ib57].g.zE) && (yg2>b[ib57].g.yS) && (yg2 < b[ib57].g.yE))
						{
							i57_found = ib57;
						}
					}


					if (i56_found >= 0) {
						if (b[i56_found].itype == SOLID) {

							if ((i57_found >= 0) && (b[i57_found].itype == SOLID)) {
								// �� �������� �������� ����� � ���� � ������� �����������������.
								// comparison_lam ����� ������ ���� ���������������� ����� i56 ������.
								if (comparison_lam(matlist, b, i56_found, i57_found, 25.0)) {
									// � ����� i56 ���������������� ����.
									s[i].g.yS = yg1;
									s[i].g.yE = yg1;
									addboundary(ypos, iny, yg1);
								}
								else {
									// � ����� i57 ���������������� ����.
									s[i].g.yS = yg2;
									s[i].g.yE = yg2;
									addboundary(ypos, iny, yg2);
								}
							}
							else {
								// ������ ����� ����.
								s[i].g.yS = yg1;
								s[i].g.yE = yg1;
								addboundary(ypos, iny, yg1);
							}
						}
						else {

							if (i57_found >= 0) {
								if (b[i57_found].itype == SOLID) {
									// ������ ����� ����.
									s[i].g.yS = yg2;
									s[i].g.yE = yg2;
									addboundary(ypos, iny, yg2);
								}
							}
							else {
								printf("ERROR : sourse na granice dvus hollow or fluid blockov.");
								system("PAUSE");
								exit(1);
							}
						}
					}
				}
				else {
					doublereal yg2 = 0.5*(yg + ypos[i55_found - 1]);
					printf("yg2=%e\n", yg2);
					integer i57_found = -2;
					for (integer ib57 = 0; ib57 < lb; ib57++) {
						if ((xc>b[ib57].g.xS) && (xc<b[ib57].g.xE) && (zc>b[ib57].g.zS) && (zc<b[ib57].g.zE) && (yg2>b[ib57].g.yS) && (yg2 < b[ib57].g.yE))
						{
							i57_found = ib57;
						}
					}
					if (i57_found >= 0) {
						if (b[i57_found].itype == SOLID) {
							// ������ ����� ����.
							s[i].g.yS = yg2;
							s[i].g.yE = yg2;
							addboundary(ypos, iny, yg2);
						}
					}
					else {
						printf("ERROR : sourse na granice dvus hollow or fluid blockov.");
						system("PAUSE");
						exit(1);
					}
				}
			}
		}
	}
	//BubbleEnhSort(ypos, iny);
	Sort_method(ypos, iny);

	
	// ��������� �������� ����� �� ������� �������� � ��������� 30.0
	// ��� �� flowvision.
	//quolite_refinement(inx, iny, inz, xpos, ypos, zpos);

	/*// debug
	for (i=0; i<=inz; i++) {
	printf("%f  ",zpos[i]);
	}
	getchar();//*/

	// debug
    /*
	for (i=0; i<=inz; i++) {
        printf("%e  ",zpos[i]);   
	}
	getchar();//
    */

    // ������������ ����������� ������.
	delete[] rxboundary;
	delete[] ryboundary;
	delete[] rzboundary;
	delete[] ixintervalcount;
	delete[] iyintervalcount;
    delete[] izintervalcount;
	delete[] source_indexpopadaniqnagranXY;
	delete[] source_indexpopadaniqnagranYZ;
	delete[] source_indexpopadaniqnagranXZ;

} // unevensimplemeshgen



/* ���������� ���������� ��������� ����� hex cartesian -
* ����������������� ������������� ����� � ��������� ���������
* ������� � ���� �������. ����������� ������� ���������� � ���
* ��� �� ����� ������������ ��� ����������� ����� ��� � �������������.
* ������ �������������� �������� ��������� �������� ������ ����������� ���������� ���
* �������������� �������� ��������� simplemeshgen.
* ���� ���������� 8 ������� 2012.
* �������� ��������� ���������� ANSYS Icepak � ����� ���������� ����������� ��������������� ������� 
* ��������� ���������� ����������� ������ �� ����� ������ �� ���� ��������� ����� �� ������� ��������������.
* ����� ������ ����� ���� ����������� ��������������� ������ �� ��������� ����� (���� ������ ��������� �����)
* � ��������� ������ ������� (�������� ���� ���) ������������ ��������� ��������������� ������.
* 3 ��� 2016.
*/
// 25.04.2018 ����� � �����������.
void coarsemeshgen(doublereal* &xpos, doublereal* &ypos, doublereal* &zpos, integer &inx, integer &iny, integer &inz,
	integer lb, integer ls, integer lw, BLOCK* &b, SOURCE* &s, WALL* &w, integer lu, UNION* &my_union, TPROP* matlist,
	doublereal* &xposadd, doublereal* &yposadd, doublereal* &zposadd,
	integer &inxadd, integer &inyadd, integer &inzadd, integer &iunion_id_p1)
{

	//*****************************************************************************************************************
	// ���������� ��������� ��������������� ��������� ����������

	// ���� bgeom == true, �� ������������ ������������� ����� �� ������ �������������� ����������.
	bool bgeomx = false; // �� ��� Ox
	bool bgeomy = false; // �� ��� Oy
	bool bgeomz = false; // �� ��� Oz
	// 1.05 - ����� ����� �������������, 1.1, 1.2 - �������� �������������, 1.25, 1.5, 2 - ������ �������������.
	// ��� ������������������ ������� �������������� ����������� �������������� ���������� ������ ���� �� ������ 1.3.
	//doublereal qxW = 10.0; // �������� �������� � ����� West �������. 
	//doublereal qxE = 10.0; // �������� �������� � ������ East �������
	//doublereal qyS = 10.0; // South
	//doublereal qyN = 10.0; // North
	//doublereal qzB = 10.0; // Bottom
	//doublereal qzT = 10.0; // Top
	//doublereal qzB=1.00005; // Bottom
	//doublereal qzT=1.35; // Top

	//bool bsnap_TO = bsnap_TO_global; // snap to grid (��������� �� ����� � ������� �������).
	//doublereal snap_to_multiplyer = 0.3;// ��������� ����� � ��������� (0..1) � ���������� ����� ����� ������������.
	//*****************************************************************************************************************

	//bool brepeat = true;

	
	// �� ��� Ox
	doublereal *rxboundary=NULL; // ������ ������������ ������
	integer inumboundaryx = 1;
	
	doublereal *ryboundary = NULL; // ������ ������������ ������
	integer inumboundaryy = 1;

	doublereal *rzboundary = NULL; // ������ ������������ ������
	integer inumboundaryz = 1;

	// ���������� � ���������� ������� minimum fluid gap.
	calc_minimum_fluid_gap1(inumboundaryx, rxboundary, inumboundaryy, ryboundary, inumboundaryz, rzboundary,
		lb, ls, lw, b, s, w, lu, my_union, iunion_id_p1);
	//printf("%d %d %d", inumboundaryx, inumboundaryy, inumboundaryz);

	doublereal minimum_fluid_gap_x = 1.0e40;
	doublereal minimum_fluid_gap_y = 1.0e40;
	doublereal minimum_fluid_gap_z = 1.0e40;

	// ��������������� ���������� ������� minimum fluid gap.
	calc_minimum_fluid_gap2(inumboundaryx, rxboundary, inumboundaryy, ryboundary,
		inumboundaryz, rzboundary, minimum_fluid_gap_x, minimum_fluid_gap_y, minimum_fluid_gap_z,
		lb, ls, lw, b, s, w, lu, my_union, iunion_id_p1);
	

	bool *source_indexpopadaniqnagranYZ = NULL;
	bool *source_indexpopadaniqnagranXY = NULL;
	bool *source_indexpopadaniqnagranXZ = NULL;

	// 12.03.2017
	// ���������� snap to
	// ��������� ����������� �������� ������ �� 33%.
	
	snap_to_moving(source_indexpopadaniqnagranYZ,
		source_indexpopadaniqnagranXY,
		source_indexpopadaniqnagranXZ,
		rxboundary, ryboundary, rzboundary,
		inumboundaryx, inumboundaryy, inumboundaryz,
		minimum_fluid_gap_x, minimum_fluid_gap_y, minimum_fluid_gap_z,
		lb,  ls, lw, b, s, w, lu, my_union, iunion_id_p1);
	//printf("%d %d %d", inumboundaryx, inumboundaryy, inumboundaryz);
	//getchar();

	// �������� �������� ������������� ������ ��������� ����� ������������� ���
	// ������� �������� ������ �������� ����� �� ���������� ���� ����� �� ��������
	// ������� ��������� ��������. ��� ��������� ��� ������ ������������� �����.
	calc_minimum_fluid_gap3(inumboundaryx, rxboundary, inumboundaryy, ryboundary, inumboundaryz, rzboundary,
		lb, ls, lw, b, s, w, lu, my_union, iunion_id_p1);

	//integer i;


	// preprocessing
	integer* ib_marker = new integer[inumboundaryx*inumboundaryy*inumboundaryz];
	/*
	for (integer i9 = 0; i9 < inumboundaryx; i9++) {
	for (integer i7 = 0; i7 < inumboundaryy; i7++) {
	for (integer i8 = 0; i8 < inumboundaryz; i8++) {
	doublereal yc = 0.5*(ryboundary[i7] + ryboundary[i7 + 1]);
	doublereal zc = 0.5*(rzboundary[i8] + rzboundary[i8 + 1]);
	doublereal xc = 0.5*(rxboundary[i9] + rxboundary[i9 + 1]);
	integer ib;
	TOCHKA p;
	p.x = xc;
	p.y = yc;
	p.z = zc;
	in_model_fluid_gap(p, ib, b, lb);
	// x + y*dimx+z*dimx*dimy
	ib_marker[i9+ inumboundaryx*i7+ inumboundaryx*inumboundaryy*i8] = ib;
	}
	}
	}
	*/

	// ������� ������������� �� �������� �����.
	Block_indexes* block_indexes = new Block_indexes[lb];
	if (block_indexes == NULL) {
		printf("error in allocation memory for block_indexes in enumerate_volume_improved.\n");
		system("pause");
		exit(1);
	}
	integer i, j, k;


	// 08.04.2018
	for (i = 0; i < lb; i++) {
		// �������������, �� ������ ���� ����� �� ����� ����������.
		block_indexes[i].iL = -1;
		block_indexes[i].iR = -2;
		block_indexes[i].jL = -1;
		block_indexes[i].jR = -2;
		block_indexes[i].kL = -1;
		block_indexes[i].kR = -2;
	}

/*
	for (i = 0; i < lb; i++) {
		doublereal x4 = b[i].g.xS;
		doublereal distmax;
		distmax = 1.0e40;
		for (j = 0; j <= inumboundaryx; j++) {
			if (fabs(rxboundary[j] - x4) < distmax) {
				block_indexes[i].iL = j;
				distmax = fabs(rxboundary[j] - x4);
			}
		}
		x4 = b[i].g.xE;
		distmax = 1.0e40;
		for (j = 0; j <= inumboundaryx; j++) {
			if (fabs(rxboundary[j] - x4) < distmax) {
				block_indexes[i].iR = j;
				distmax = fabs(rxboundary[j] - x4);
			}
		}
		x4 = b[i].g.yS;
		distmax = 1.0e40;
		for (j = 0; j <= inumboundaryy; j++) {
			if (fabs(ryboundary[j] - x4) < distmax) {
				block_indexes[i].jL = j;
				distmax = fabs(ryboundary[j] - x4);
			}
		}
		x4 = b[i].g.yE;
		distmax = 1.0e40;
		for (j = 0; j <= inumboundaryy; j++) {
			if (fabs(ryboundary[j] - x4) < distmax) {
				block_indexes[i].jR = j;
				distmax = fabs(ryboundary[j] - x4);
			}
		}
		x4 = b[i].g.zS;
		distmax = 1.0e40;
		for (j = 0; j <= inumboundaryz; j++) {
			if (fabs(rzboundary[j] - x4) < distmax) {
				block_indexes[i].kL = j;
				distmax = fabs(rzboundary[j] - x4);
			}
		}
		x4 = b[i].g.zE;
		distmax = 1.0e40;
		for (j = 0; j <= inumboundaryz; j++) {
			if (fabs(rzboundary[j] - x4) < distmax) {
				block_indexes[i].kR = j;
				distmax = fabs(rzboundary[j] - x4);
			}
		}
	}
	*/
	for (i = 0; i < lb; i++) {
		//if (b[i].iunion_id == iunion_id_p1) {
		{
			doublereal x4 = b[i].g.xS;
			for (j = 0; j <= inumboundaryx; j++) {
				if (fabs(rxboundary[j] - x4) < 1.0e-40) {
					block_indexes[i].iL = j;
					break;
				}
			}
			x4 = b[i].g.xE;
			for (j = 0; j <= inumboundaryx; j++) {
				if (fabs(rxboundary[j] - x4) < 1.0e-40) {
					block_indexes[i].iR = j;
					break;
				}
			}
			x4 = b[i].g.yS;
			for (j = 0; j <= inumboundaryy; j++) {
				if (fabs(ryboundary[j] - x4) < 1.0e-40) {
					block_indexes[i].jL = j;
					break;
				}
			}
			x4 = b[i].g.yE;
			for (j = 0; j <= inumboundaryy; j++) {
				if (fabs(ryboundary[j] - x4) < 1.0e-40) {
					block_indexes[i].jR = j;
					break;
				}
			}
			x4 = b[i].g.zS;
			for (j = 0; j <= inumboundaryz; j++) {
				if (fabs(rzboundary[j] - x4) < 1.0e-40) {
					block_indexes[i].kL = j;
					break;
				}
			}
			x4 = b[i].g.zE;
			for (j = 0; j <= inumboundaryz; j++) {
				if (fabs(rzboundary[j] - x4) < 1.0e-40) {
					block_indexes[i].kR = j;
					break;
				}
			}
		}
	}

	

	integer ismarker = 0;
	if (iunion_id_p1 > 0) {
		for (integer m7 = 0; m7 < lb; m7++) {
			if (block_indexes[m7].iL < 0) {
				block_indexes[m7].iL = 0;
				ismarker++;
			}
			if (block_indexes[m7].jL < 0) {
				block_indexes[m7].jL = 0;
				ismarker++;
			}
			if (block_indexes[m7].kL < 0) {
				block_indexes[m7].kL = 0;
				ismarker++;
			}
			if (block_indexes[m7].iR < 0) {
				block_indexes[m7].iR = inumboundaryx;
				ismarker++;
			}
			if (block_indexes[m7].jR < 0) {
				block_indexes[m7].jR = inumboundaryy;
				ismarker++;
			}
			if (block_indexes[m7].kR < 0) {
				block_indexes[m7].kR = inumboundaryz;
				ismarker++;
			}
		}
	}
	//printf("ismarker =%d\n", ismarker);
	//getchar();
	

	// ���������� �������� ����������� ����������� � � ����� ��� �������� � �������������
	// ���������� ��������������.
	integer m7;
	integer ib_stub = -1;
	// �� ������ ����� ������� �� ������� Hollow block, ����� ����� ������ �������.
	ib_stub = 0;
	doublereal vol_stub = -1.0;
	for (i = 0; i < lb; i++) {
		//if (b[i].iunion_id == iunion_id_p1) {
		{
			if (b[i].itype == HOLLOW) {
				if (fabs(b[i].g.xE - b[i].g.xS)*fabs(b[i].g.yE - b[i].g.yS)*fabs(b[i].g.zE - b[i].g.zS) > vol_stub) {
					ib_stub = i;
					vol_stub = fabs(b[i].g.xE - b[i].g.xS)*fabs(b[i].g.yE - b[i].g.yS)*fabs(b[i].g.zE - b[i].g.zS);
				}
			}
		}
	}

#pragma omp parallel for
	for (integer i1 = 0; i1 < inumboundaryx; i1++) for (integer j1 = 0; j1 < inumboundaryy; j1++) for (integer k1 = 0; k1 < inumboundaryz; k1++) {
		ib_marker[i1 + inumboundaryx*j1 + inumboundaryx*inumboundaryy*k1] = ib_stub;//-1
	}
	for (m7 = 0; m7 < lb; m7++) {

#pragma omp parallel for
		for (integer i1 = block_indexes[m7].iL; i1 < block_indexes[m7].iR; i1++) for (integer j1 = block_indexes[m7].jL; j1 < block_indexes[m7].jR; j1++) for (integer k1 = block_indexes[m7].kL; k1 < block_indexes[m7].kR; k1++) {
			ib_marker[i1 + inumboundaryx*j1 + inumboundaryx*inumboundaryy*k1] = m7;
		}
	}

	delete[] block_indexes;
	//printf("identifire blocks number 80 procent.\n");


	integer *ixintervalcount; // ����� ����������
	ixintervalcount = new integer[inumboundaryx]; // �� ���� ������ ��� ����� ������.
	//doublereal alphascal;
	//integer inowintervalcount;
//	for (i = 0; i<(inumboundaryx); i++) {
	//	alphascal = (rxboundary[i + 1] - rxboundary[i]) / (rxboundary[inumboundaryx] - rxboundary[0]);
		//inowintervalcount = (int)(alphascal*inx);
	//	if (inowintervalcount < min_elem_in_x_element) inowintervalcount = min_elem_in_x_element;
		//ixintervalcount[i] = inowintervalcount;
	//}


	for (i = 0; i < (inumboundaryx); i++) {
		// ���� �� � solide �� ���� ������.
		// ���� �� Fluide �� ��� ������.
		doublereal cpos = 0.5*(rxboundary[i + 1] + rxboundary[i]);
		// � ��������� ������ ��������� fluid ����������� ����� ������ 
		// �� E � W �������� ������ Solid � hollow �������.
		bool bfound_onex_fluid_cv = false;
		// �������� ���������� �������������� ������ ����.
		integer ibcur = 0; // ����� �������� ����� (������� �� ���������).
		// ��������� ���������.
		for (integer iy = 0; iy < (inumboundaryy); iy++) {
			for (integer iz = 0; iz < (inumboundaryz); iz++) {
				doublereal qgeom = 10.0;
				// ��������� ���������� �������� ��.
				doublereal cposy = 0.5*(ryboundary[iy + 1] + ryboundary[iy]);
				doublereal cposz = 0.5*(rzboundary[iz + 1] + rzboundary[iz]);
				// ���������� ����� ����� � �������� ����������� ������ ��.
				//for (integer i99 = 0; i99 < lb; i99++) {
					//if ((cpos>b[i99].g.xS) && (cpos < b[i99].g.xE)&&
						//(cposy>b[i99].g.yS) && (cposy < b[i99].g.yE)&&
						//(cposz>b[i99].g.zS) && (cposz < b[i99].g.zE)) {
						//ibcur = i99;
					//}
				//}

				ibcur = ib_marker[i + inumboundaryx*iy + inumboundaryx*inumboundaryy*iz];
				
				if (b[ibcur].itype == FLUID) {
					// ������ ���������� : ���� ���� ���� ����� (E,W) ���� FLUID � ������ �������������� ���������� 10.0 ?
					// �.�. ���� �������� ��� FLUID ������ �� � ��� ��������� ������ ������ 10=qgeom �� ��������� �� ����� ���� �������� �������.
					if (i < inumboundaryx - 1) {
						doublereal cpos_plus = 0.5*(rxboundary[i + 2] + rxboundary[i+1]);
						integer ibcur_plus = 0; 
						// ���������� ����� ����� � �������� ����������� ������ ��.
						//for (integer i99 = 0; i99 < lb; i99++) {
							//if ((cpos_plus>b[i99].g.xS) && (cpos_plus < b[i99].g.xE) &&
								//(cposy>b[i99].g.yS) && (cposy < b[i99].g.yE) &&
								//(cposz>b[i99].g.zS) && (cposz < b[i99].g.zE)) {
								//ibcur_plus = i99;
							//}
						//}
						ibcur_plus = ib_marker[i+1 + inumboundaryx*iy + inumboundaryx*inumboundaryy*iz];
						if (b[ibcur_plus].itype != FLUID) {
							if (i > 0) {
								doublereal cpos_minus = 0.5*(rxboundary[i] + rxboundary[i - 1]);
								integer ibcur_minus = 0;
								// ���������� ����� ����� � �������� ����������� ������ ��.
								//for (integer i99 = 0; i99 < lb; i99++) {
									//if ((cpos_minus>b[i99].g.xS) && (cpos_minus < b[i99].g.xE) &&
										//(cposy>b[i99].g.yS) && (cposy < b[i99].g.yE) &&
										//(cposz>b[i99].g.zS) && (cposz < b[i99].g.zE)) {
										//ibcur_minus = i99;
									//}
								//}
								ibcur_minus = ib_marker[i - 1 + inumboundaryx*iy + inumboundaryx*inumboundaryy*iz];
								if (b[ibcur_minus].itype != FLUID) {
									// ��������� FLUID ����
									bfound_onex_fluid_cv = true;
									break;
								}
								else {
									// ������ ����� ���� � �� ���� FLUID
									// �������� �� qgeom
									doublereal q1=fabs((rxboundary[i] - rxboundary[i - 1]) / (rxboundary[i + 1] - rxboundary[i]));
									doublereal qgeom_inv = 1.0 / qgeom;
									if (q1 < qgeom_inv) {
										bfound_onex_fluid_cv = true;
										break;
									}
									if (q1 > qgeom) {
										// W ������� ���� ���� �������, �� ���� ������ �������������� ��� ��������� W �����. 
									}
								}
							}
							else {
								// � ������� ������ ������ ���� !!!
								// ��������� FLUID ����
								bfound_onex_fluid_cv = true;
								break;
							}
						}
						else {
							// ������� ������ ����� FLUID
							// ���������� ������������ ����� ����� ���� ������� ������� ������ �� ������������� qgeom ��������
							// �.�. ��� ������� �����
							doublereal q2 = fabs((rxboundary[i+2] - rxboundary[i+1]) / (rxboundary[i + 1] - rxboundary[i]));
							doublereal qgeom_inv = 1.0 / qgeom;
							if (q2 < qgeom_inv) {
							     // ������� ������ �������������������,
								// ��������� � ������������ ������.
								if (i > 0) {
									doublereal cpos_minus = 0.5*(rxboundary[i] + rxboundary[i - 1]);
									integer ibcur_minus = 0;
									// ���������� ����� ����� � �������� ����������� ������ ��.
									//for (integer i99 = 0; i99 < lb; i99++) {
										//if ((cpos_minus>b[i99].g.xS) && (cpos_minus < b[i99].g.xE) &&
											//(cposy>b[i99].g.yS) && (cposy < b[i99].g.yE) &&
											//(cposz>b[i99].g.zS) && (cposz < b[i99].g.zE)) {
											//ibcur_minus = i99;
									//	}
									//}
									ibcur_minus = ib_marker[i-1 + inumboundaryx*iy + inumboundaryx*inumboundaryy*iz];
									if (b[ibcur_minus].itype != FLUID) {
										// ��������� FLUID ����
										bfound_onex_fluid_cv = true;
										break;
									}
									else {
										// ������ ����� ���� � �� ���� FLUID
										// �������� �� qgeom
										doublereal q1 = fabs((rxboundary[i] - rxboundary[i - 1]) / (rxboundary[i + 1] - rxboundary[i]));
										doublereal qgeom_inv = 1.0 / qgeom;
										if (q1 < qgeom_inv) {
											bfound_onex_fluid_cv = true;
											break;
										}
										if (q1 > qgeom) {
											// W ������� ���� ���� �������, �� ���� ������ �������������� ��� ��������� W �����. 
										}
									}
								}
								else {
									// � ������� ������ ������ ���� !!!
									// ��������� FLUID ����
									bfound_onex_fluid_cv = true;
									break;
								}
							}
						}
					}
					else {
						// ��������� ������� ������
						// ������� ����� ������ �����������
						if (i > 0) {
							doublereal cpos_minus = 0.5*(rxboundary[i] + rxboundary[i - 1]);
							integer ibcur_minus = 0;
							// ���������� ����� ����� � �������� ����������� ������ ��.
							//for (integer i99 = 0; i99 < lb; i99++) {
								//if ((cpos_minus>b[i99].g.xS) && (cpos_minus < b[i99].g.xE) &&
									//(cposy>b[i99].g.yS) && (cposy < b[i99].g.yE) &&
									//(cposz>b[i99].g.zS) && (cposz < b[i99].g.zE)) {
									//ibcur_minus = i99;
								//}
							//}

							ibcur_minus = ib_marker[i - 1 + inumboundaryx*iy + inumboundaryx*inumboundaryy*iz];
							if (b[ibcur_minus].itype != FLUID) {
								// ��������� FLUID ����
								bfound_onex_fluid_cv = true;
								break;
							}
							else {
								// ������ ����� ���� � �� ���� FLUID
								// �������� �� qgeom
								doublereal q1 = fabs((rxboundary[i] - rxboundary[i - 1]) / (rxboundary[i + 1] - rxboundary[i]));
								doublereal qgeom_inv = 1.0 / qgeom;
								if (q1 < qgeom_inv) {
									bfound_onex_fluid_cv = true;
									break;
								}
								if (q1 > qgeom) {
									// W ������� ���� ���� �������, �� ���� ������ �������������� ��� ��������� W �����. 
								}
							}
						}
						else {
							// ��� �������� ��� � ��� ����� ������ ���� ������.
							// ������ ���� � �������.
							bfound_onex_fluid_cv = true;
							break;
						}
					}
				}
			}
		}
		
		if (bfound_onex_fluid_cv==false) {
			// SOLID
			ixintervalcount[i] = 1;
		}
		else {
			// FLUID
			bool b2div = false;
			for (integer i_3 = 0; i_3 < (inumboundaryy); i_3++) {
				for (integer i_4 = 0; i_4 < (inumboundaryz); i_4++) {
					doublereal yp_3= 0.5*(ryboundary[i_3 + 1] + ryboundary[i_3]);
					doublereal zp_3 = 0.5*(rzboundary[i_4 + 1] + rzboundary[i_4]);
					doublereal xp_1= 0.5*(rxboundary[i + 1] + rxboundary[i]);
					doublereal xp_2 = xp_1;
					doublereal xp_3 = xp_1;
					if (i < inumboundaryx - 1) {
					    xp_2 = 0.5*(rxboundary[i + 2] + rxboundary[i+1]);
					}
					if (i > 0) {
						xp_3 = 0.5*(rxboundary[i - 1] + rxboundary[i]);
					}
					// ���������� ����� ����� �� ���������� �����.
					//myisblock_id(integer lb, BLOCK* &b, doublereal x11, doublereal y11, doublereal z11)
					if (!((((b[myisblock_id(lb, b, xp_1, yp_3, zp_3)].itype == HOLLOW)||(b[myisblock_id(lb, b, xp_1, yp_3, zp_3)].itype == SOLID)) && ((b[myisblock_id(lb, b, xp_2, yp_3, zp_3)].itype == HOLLOW)||(b[myisblock_id(lb, b, xp_2, yp_3, zp_3)].itype == SOLID)) && ((b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == HOLLOW)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == SOLID))) || ((b[myisblock_id(lb,b,xp_1,yp_3,zp_3)].itype == SOLID) && (b[myisblock_id(lb, b, xp_2, yp_3, zp_3)].itype == SOLID) && (b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == SOLID)) || (((b[myisblock_id(lb, b, xp_1, yp_3, zp_3)].itype == FLUID)||(b[myisblock_id(lb, b, xp_1, yp_3, zp_3)].itype == HOLLOW)) && ((b[myisblock_id(lb, b, xp_2, yp_3, zp_3)].itype == FLUID)||(b[myisblock_id(lb, b, xp_2, yp_3, zp_3)].itype == HOLLOW)) && ((b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == FLUID)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == HOLLOW)))))
					{
						// ���� ������ ���� ������� ����� ����� �� ������������� ������������ solid ��� fluid.
						b2div = true;
					}
				}
			}
			if (b2div) {
				ixintervalcount[i] = 2;
			}
			else {
				printf("inc X");
				//getchar();
				ixintervalcount[i] = 1;
			}
		}
	}

	 // debug
	/*
	for (i=0; i<(inumboundaryx); i++) {
	#if doubleintprecision == 1
		printf("%lld  ",ixintervalcount[i]);
	#else
		printf("%d  ",ixintervalcount[i]);
	#endif
		getchar();
	} //*/
	//getchar();

	// ���������� ������ ��������� �����.
	integer iposmark = 1;
	doublereal dx;
	//integer k;
	integer ixoldsize = 0;
	SetLength(xpos, ixoldsize, 1);
	ixoldsize = 1;
	for (i = 0; i<(inumboundaryx); i++)
	{
		//if ((ixintervalcount[i] - 2) % 2 == 0) ixintervalcount[i]++; // ����� ����� ����� ���� ������ ������.
		//integer n2 = (int)((ixintervalcount[i] - 1) / 2); // ���������� � ������� �������
		//if (bgeomx) n2 = ibalancen2(n2, qxW, qxE, rxboundary, ixintervalcount[i], i, iposmark); // ������������ 
		//doublereal qn2W = qxW;
		//for (integer i1 = 1; i1<n2; i1++) qn2W *= qxW; // ���������� � �������.
		//doublereal b1W = (rxboundary[i + 1] - rxboundary[i])*(qxW - 1.0) / (2.0*(qn2W - 1.0));
		//doublereal qn2E = qxE;
		//for (integer i1 = 1; i1<(ixintervalcount[i] - n2); i1++) qn2E *= qxE; // ���������� � �������.
		//doublereal b1E = (rxboundary[i + 1] - rxboundary[i])*(qxE - 1.0) / (2.0*(qn2E - 1.0));
		//printf("length=%e\n",(rxboundary[i+1]-rxboundary[i]));
		//getchar(); // OK

		dx = (rxboundary[i + 1] - rxboundary[i]) / (ixintervalcount[i]);
		SetLength(xpos, ixoldsize, (iposmark + ixintervalcount[i]+1));//+1
		ixoldsize = (iposmark + ixintervalcount[i]+1);//+1
#if doubleintprecision == 1
		//printf("%lld  ",ixoldsize);// debug
#else
		//printf("%d  ",ixoldsize);// debug
#endif
		
		for (k = iposmark; k <= iposmark + ixintervalcount[i] - 2+1; k++)
		{
			if (!bgeomx) {
				// ����������� �����
				xpos[k] = rxboundary[i] + (k - iposmark)*dx;
			}
			/*else
			{
				// ������������� �����
				integer ic1 = k - iposmark;
				doublereal madd;
				if (ic1<n2) {
					// ������� ����� �������.
					madd = b1W;
					for (integer i1 = 1; i1<ic1; i1++) madd *= qxW;
					if (ic1 == n2 - 1) madd = 0.5*(rxboundary[i + 1] + rxboundary[i]) - xpos[k - 1]; // ����� �������� ������ ����������
				}
				else
				{
					// ����� ������ �������.
					madd = b1E;
					for (integer i1 = ixintervalcount[i] - 1 - ic1; i1>0; i1--) madd *= qxE;
				}
				if (k == iposmark) xpos[k] = rxboundary[i];
				else xpos[k] = xpos[k - 1] + madd;

			}
			*/
		}
		iposmark = iposmark + ixintervalcount[i]-1+1; //+1
	}
	SetLength(xpos, ixoldsize, iposmark + 1);
	xpos[iposmark] = rxboundary[inumboundaryx];
	inx = iposmark;
	for (i = 0; i<inx; i++) xpos[i] = xpos[i + 1]; // ����� ����� �� 1
	SetLength(xpos, inx + 1, inx);
	inx--; // ���������� ���������� � ���� � ������������� ��������� inx

	// ��� ����� �����  ������ ����� ������������ �����. 
	//for (i = 0; i<adapt_x; i++) simplecorrect_meshgen_x(xpos, inx, lb, ls, lw, b, s, w);

	/*
	// debug
	for (i=0; i<=inx; i++) {
	printf("%f  ",xpos[i]);
	}
	getchar(); //
	*/

	// 3 �������� 2017.
	// ���������� ����� �������� �����.
	// ������������� �� ���������� ����������� ����������� ���� �����.
	for (int i_28 = 0; i_28 <= inxadd; i_28++) {
		SetLength(xpos, inx+1, inx+2);
		xpos[inx+1] = xposadd[i_28];
		inx++;
	}
	Sort_method(xpos, inx);

	if (1) {
		//const doublereal etalon_max_size_ratio=2.0;
		integer inum_iter_ratio_good = 0;
		while (1) {

			// ���������� max size ratio x axis.
			doublereal max_size_ratio_x = 1.0;
			for (i = 0; i<inx - 1; i++) {

				doublereal dmax = 0.0;
				doublereal dmin = 0.0;
				if (fabs(xpos[i + 1] - xpos[i])>fabs(xpos[i + 2] - xpos[i + 1])) {
					dmax = fabs(xpos[i + 1] - xpos[i]);
					dmin = fabs(xpos[i + 2] - xpos[i + 1]);
				}
				else {
					dmax = fabs(xpos[i + 2] - xpos[i + 1]);
					dmin = fabs(xpos[i + 1] - xpos[i]);
				}
				if (dmax / dmin>max_size_ratio_x) {
					max_size_ratio_x = dmax / dmin;
				}
			}
			printf("x axis max size ratio is equal = %1.4f\n", max_size_ratio_x);
			if (max_size_ratio_x != max_size_ratio_x) {
				system("PAUSE");
			}
			//getchar();

			if (max_size_ratio_x < (etalon_max_size_ratio*1.1)) {
				break;
			}

			// ������������� max size ratio x axis.
			// ������� 9 ������ 2013 ����.

			bool bplus = false;
			max_size_ratio_x = 1.0;
			for (i = 0; i<inx - 1; i++) {

				bplus = false;
				doublereal dmax = 0.0;
				doublereal dmin = 0.0;
				if (fabs(xpos[i + 1] - xpos[i])>fabs(xpos[i + 2] - xpos[i + 1])) {
					dmax = fabs(xpos[i + 1] - xpos[i]);
					dmin = fabs(xpos[i + 2] - xpos[i + 1]);
				}
				else {
					bplus = true;
					dmax = fabs(xpos[i + 2] - xpos[i + 1]);
					dmin = fabs(xpos[i + 1] - xpos[i]);
				}
				if (dmax / dmin>etalon_max_size_ratio) {
					doublereal pos_candidate;
					if (bplus) {
						pos_candidate = xpos[i + 1] + 0.5*dmax;
					}
					else {
						pos_candidate = xpos[i] + 0.5*dmax;
					}
					SetLength(xpos, inx + 1, inx + 2);
					xpos[inx + 1] = pos_candidate;
					inx = inx + 1;
					//BubbleEnhSort(xpos, inx);
					Sort_method(xpos, inx);
					break;
				}
			}

			inum_iter_ratio_good++;

			//getchar();

		}

#if doubleintprecision == 1
		printf("inum_iter_ratio_good is %lld\n", inum_iter_ratio_good);
#else
		printf("inum_iter_ratio_good is %d\n", inum_iter_ratio_good);
#endif
		
	}


	integer *iyintervalcount; // ����� ����������
	iyintervalcount = new integer[inumboundaryy]; // �� ���� ������ ��� ����� ������.
	//for (i = 0; i<(inumboundaryy); i++) {
		//alphascal = (ryboundary[i + 1] - ryboundary[i]) / (ryboundary[inumboundaryy] - rxboundary[0]);
		//inowintervalcount = (int)(alphascal*iny);
		//if (inowintervalcount < min_elem_in_y_element) inowintervalcount = min_elem_in_y_element;
		//iyintervalcount[i] = inowintervalcount;
	//}

	

	for (i = 0; i < (inumboundaryy); i++) {
		// ���� �� � solide �� ���� ������.
		// ���� �� Fluide �� ��� ������.
		doublereal cpos = 0.5*(ryboundary[i + 1] + ryboundary[i]);
		// � ��������� ������ ��������� fluid ����������� ����� ������ 
		// �� E � W �������� ������ Solid � hollow �������.
		bool bfound_onex_fluid_cv = false;
		// �������� ���������� �������������� ������ ����.
		integer ibcur = 0; // ����� �������� ����� (������� �� ���������).
		// ��������� ���������.
		for (integer ix = 0; ix < (inumboundaryx); ix++) {
			for (integer iz = 0; iz < (inumboundaryz); iz++) {
				doublereal qgeom = 10.0;
				// ��������� ���������� �������� ��.
				doublereal cposx = 0.5*(rxboundary[ix + 1] + rxboundary[ix]);
				doublereal cposz = 0.5*(rzboundary[iz + 1] + rzboundary[iz]);
				// ���������� ����� ����� � �������� ����������� ������ ��.
				//for (integer i99 = 0; i99 < lb; i99++) {
					//if ((cpos>b[i99].g.yS) && (cpos < b[i99].g.yE) &&
						//(cposx>b[i99].g.xS) && (cposx < b[i99].g.xE) &&
						//(cposz>b[i99].g.zS) && (cposz < b[i99].g.zE)) {
						//ibcur = i99;
					//}
				//}
				ibcur = ib_marker[ix + inumboundaryx*i + inumboundaryx*inumboundaryy*iz];

				if (b[ibcur].itype == FLUID) {
					// ������ ���������� : ���� ���� ���� ����� (N,S) ���� FLUID � ������ �������������� ���������� 10.0 ?
					// �.�. ���� �������� ��� FLUID ������ �� � ��� ��������� ������ ������ 10=qgeom �� ��������� �� ����� ���� �������� �������.
					if (i < inumboundaryy - 1) {
						doublereal cpos_plus = 0.5*(ryboundary[i + 2] + ryboundary[i + 1]);
						integer ibcur_plus = 0;
						// ���������� ����� ����� � �������� ����������� ������ ��.
						//for (integer i99 = 0; i99 < lb; i99++) {
							//if ((cpos_plus>b[i99].g.yS) && (cpos_plus < b[i99].g.yE) &&
								//(cposx>b[i99].g.xS) && (cposx < b[i99].g.xE) &&
								//(cposz>b[i99].g.zS) && (cposz < b[i99].g.zE)) {
								//ibcur_plus = i99;
							//}
						//}
						ibcur_plus = ib_marker[ix + inumboundaryx*(i+1) + inumboundaryx*inumboundaryy*iz];
						if (b[ibcur_plus].itype != FLUID) {
							if (i > 0) {
								doublereal cpos_minus = 0.5*(ryboundary[i] + ryboundary[i - 1]);
								integer ibcur_minus = 0;
								// ���������� ����� ����� � �������� ����������� ������ ��.
								//for (integer i99 = 0; i99 < lb; i99++) {
									//if ((cpos_minus>b[i99].g.yS) && (cpos_minus < b[i99].g.yE) &&
										//(cposx>b[i99].g.xS) && (cposx < b[i99].g.xE) &&
										//(cposz>b[i99].g.zS) && (cposz < b[i99].g.zE)) {
										//ibcur_minus = i99;
									//}
								//}
								ibcur_minus = ib_marker[ix + inumboundaryx*(i - 1) + inumboundaryx*inumboundaryy*iz];
								if (b[ibcur_minus].itype != FLUID) {
									// ��������� FLUID ����
									bfound_onex_fluid_cv = true;
									break;
								}
								else {
									// ������ ����� ���� � �� ���� FLUID
									// �������� �� qgeom
									doublereal q1 = fabs((ryboundary[i] - ryboundary[i - 1]) / (ryboundary[i + 1] - ryboundary[i]));
									doublereal qgeom_inv = 1.0 / qgeom;
									if (q1 < qgeom_inv) {
										bfound_onex_fluid_cv = true;
										break;
									}
									if (q1 > qgeom) {
										// W ������� ���� ���� �������, �� ���� ������ �������������� ��� ��������� W �����. 
									}
								}
							}
							else {
								// � ������� ������ ������ ���� !!!
								// ��������� FLUID ����
								bfound_onex_fluid_cv = true;
								break;
							}
						}
						else {
							// ������� ������ ����� FLUID
							// ���������� ������������ ����� ����� ���� ������� ������� ������ �� ������������� qgeom ��������
							// �.�. ��� ������� �����
							doublereal q2 = fabs((ryboundary[i + 2] - ryboundary[i + 1]) / (ryboundary[i + 1] - ryboundary[i]));
							doublereal qgeom_inv = 1.0 / qgeom;
							if (q2 < qgeom_inv) {
								// ������� ������ �������������������,
								// ��������� � ������������ ������.
								if (i > 0) {
									doublereal cpos_minus = 0.5*(ryboundary[i] + ryboundary[i - 1]);
									integer ibcur_minus = 0;
									// ���������� ����� ����� � �������� ����������� ������ ��.
									//for (integer i99 = 0; i99 < lb; i99++) {
										//if ((cpos_minus>b[i99].g.yS) && (cpos_minus < b[i99].g.yE) &&
											//(cposx>b[i99].g.xS) && (cposx < b[i99].g.xE) &&
											//(cposz>b[i99].g.zS) && (cposz < b[i99].g.zE)) {
											//ibcur_minus = i99;
										//}
									//}
									ibcur_minus = ib_marker[ix + inumboundaryx*(i - 1) + inumboundaryx*inumboundaryy*iz];
									if (b[ibcur_minus].itype != FLUID) {
										// ��������� FLUID ����
										bfound_onex_fluid_cv = true;
										break;
									}
									else {
										// ������ ����� ���� � �� ���� FLUID
										// �������� �� qgeom
										doublereal q1 = fabs((ryboundary[i] - ryboundary[i - 1]) / (ryboundary[i + 1] - ryboundary[i]));
										doublereal qgeom_inv = 1.0 / qgeom;
										if (q1 < qgeom_inv) {
											bfound_onex_fluid_cv = true;
											break;
										}
										if (q1 > qgeom) {
											// W ������� ���� ���� �������, �� ���� ������ �������������� ��� ��������� W �����. 
										}
									}
								}
								else {
									// � ������� ������ ������ ���� !!!
									// ��������� FLUID ����
									bfound_onex_fluid_cv = true;
									break;
								}
							}
						}
					}
					else {
						// ��������� ������� ������
						// ������� ����� ������ �����������
						if (i > 0) {
							doublereal cpos_minus = 0.5*(ryboundary[i] + ryboundary[i - 1]);
							integer ibcur_minus = 0;
							// ���������� ����� ����� � �������� ����������� ������ ��.
							//for (integer i99 = 0; i99 < lb; i99++) {
								//if ((cpos_minus>b[i99].g.yS) && (cpos_minus < b[i99].g.yE) &&
									//(cposx>b[i99].g.xS) && (cposx < b[i99].g.xE) &&
									//(cposz>b[i99].g.zS) && (cposz < b[i99].g.zE)) {
									//ibcur_minus = i99;
								//}
							//}
							ibcur_minus = ib_marker[ix + inumboundaryx*(i - 1) + inumboundaryx*inumboundaryy*iz];
							if (b[ibcur_minus].itype != FLUID) {
								// ��������� FLUID ����
								bfound_onex_fluid_cv = true;
								break;
							}
							else {
								// ������ ����� ���� � �� ���� FLUID
								// �������� �� qgeom
								doublereal q1 = fabs((ryboundary[i] - ryboundary[i - 1]) / (ryboundary[i + 1] - ryboundary[i]));
								doublereal qgeom_inv = 1.0 / qgeom;
								if (q1 < qgeom_inv) {
									bfound_onex_fluid_cv = true;
									break;
								}
								if (q1 > qgeom) {
									// W ������� ���� ���� �������, �� ���� ������ �������������� ��� ��������� W �����. 
								}
							}
						}
						else {
							// ��� �������� ��� � ��� ����� ������ ���� ������.
							// ������ ���� � �������.
							bfound_onex_fluid_cv = true;
							break;
						}
					}
				}
			}
		}

		if (bfound_onex_fluid_cv == false) {
			iyintervalcount[i] = 1;
		}
		else {
			//iyintervalcount[i] = 2;


			// FLUID
			bool b2div = false;
			for (integer i_3 = 0; i_3 < (inumboundaryx); i_3++) {
				for (integer i_4 = 0; i_4 < (inumboundaryz); i_4++) {
					doublereal xp_3 = 0.5*(rxboundary[i_3 + 1] + rxboundary[i_3]);
					doublereal zp_3 = 0.5*(rzboundary[i_4 + 1] + rzboundary[i_4]);
					doublereal yp_1 = 0.5*(ryboundary[i + 1] + ryboundary[i]);
					doublereal yp_2 = yp_1;
					doublereal yp_3 = yp_1;
					if (i < inumboundaryy - 1) {
						yp_2 = 0.5*(ryboundary[i + 2] + ryboundary[i + 1]);
					}
					if (i > 0) {
						yp_3 = 0.5*(ryboundary[i - 1] + ryboundary[i]);
					}
					// ���������� ����� ����� �� ���������� �����.
					//myisblock_id(integer lb, BLOCK* &b, doublereal x11, doublereal y11, doublereal z11)
					if (!((((b[myisblock_id(lb, b, xp_3, yp_1, zp_3)].itype == HOLLOW)||(b[myisblock_id(lb, b, xp_3, yp_1, zp_3)].itype == SOLID)) && ((b[myisblock_id(lb, b, xp_3, yp_2, zp_3)].itype == HOLLOW)||(b[myisblock_id(lb, b, xp_3, yp_2, zp_3)].itype == SOLID)) && ((b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == HOLLOW)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == SOLID))) || ((b[myisblock_id(lb, b, xp_3, yp_1, zp_3)].itype == SOLID) && (b[myisblock_id(lb, b, xp_3, yp_2, zp_3)].itype == SOLID) && (b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == SOLID)) || (((b[myisblock_id(lb, b, xp_3, yp_1, zp_3)].itype == FLUID)||(b[myisblock_id(lb, b, xp_3, yp_1, zp_3)].itype == HOLLOW)) && ((b[myisblock_id(lb, b, xp_3, yp_2, zp_3)].itype == FLUID)||(b[myisblock_id(lb, b, xp_3, yp_2, zp_3)].itype == HOLLOW)) && ((b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == FLUID)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == HOLLOW)))))
					{
						// ���� ������ ���� ������� ����� ����� �� ������������� ������������ solid ��� fluid.
						b2div = true;
					}
				}
			}
			if (b2div) {
				iyintervalcount[i] = 2;
			}
			else {
				printf("inc Y");
				//getchar();
				iyintervalcount[i] = 1;
			}

		}
		

	}


	// ���������� ������� �����
	iposmark = 1;
	doublereal dy;
	integer iyoldsize = 0;
	SetLength(ypos, iyoldsize, 1);
	iyoldsize = 1;
	for (i = 0; i<inumboundaryy; i++)
	{

		// ����� ����� ����� ���� ������ ������
	//	if ((iyintervalcount[i] - 2) % 2 == 0) iyintervalcount[i]++;
		//integer n2 = (int)((iyintervalcount[i] - 1) / 2); // ���������� � ������� �������
		//if (bgeomy) n2 = ibalancen2(n2, qyS, qyN, ryboundary, iyintervalcount[i], i, iposmark); // ������������ 
		//doublereal qn2S = qyS;
		//for (integer i1 = 1; i1<n2; i1++) qn2S *= qyS; // ���������� � �������.
		//doublereal b1S = (ryboundary[i + 1] - ryboundary[i])*(qyS - 1.0) / (2.0*(qn2S - 1.0));
		//doublereal qn2N = qyN;
		//for (integer i1 = 1; i1<(iyintervalcount[i] - n2); i1++) qn2N *= qyN; // ���������� � �������.
		//doublereal b1N = (ryboundary[i + 1] - ryboundary[i])*(qyN - 1.0) / (2.0*(qn2N - 1.0));

		dy = (ryboundary[i + 1] - ryboundary[i]) / (iyintervalcount[i]);
		SetLength(ypos, iyoldsize, iposmark + iyintervalcount[i]+1);
		iyoldsize = iposmark + iyintervalcount[i]+1;
		for (k = iposmark; k <= iposmark + iyintervalcount[i] - 2+1; k++)
		{
			if (!bgeomy) {
				// ����������� �����
				ypos[k] = ryboundary[i] + (k - iposmark)*dy;
			}
			/*else
			{
				// ������������� �����
				// �� ������ �������������� ���������� � ����� ����������.
				integer ic1 = k - iposmark;
				doublereal madd;
				if (ic1<n2) {
					madd = b1S;
					for (integer i1 = 1; i1<ic1; i1++) madd *= qyS;
					if (ic1 == n2 - 1) madd = 0.5*(ryboundary[i + 1] + ryboundary[i]) - ypos[k - 1];
				}
				else
				{
					madd = b1N;
					for (integer i1 = iyintervalcount[i] - 1 - ic1; i1>0; i1--) madd *= qyN;
				}
				if (k == iposmark) ypos[k] = ryboundary[i];
				else ypos[k] = ypos[k - 1] + madd;
			}*/
		}
		iposmark = iposmark + iyintervalcount[i] - 1+1;
	}
	SetLength(ypos, iyoldsize, iposmark + 1);
	ypos[iposmark] = ryboundary[inumboundaryy];
	iny = iposmark;
	for (i = 0; i<iny; i++) ypos[i] = ypos[i + 1]; // ����� ����� �� 1
	SetLength(ypos, iny + 1, iny);
	iny--; // ���������� ���������� � ���� � ������������� ��������� iny

	//for (i = 0; i<adapt_y; i++) simplecorrect_meshgen_y(ypos, iny, lb, ls, lw, b, s, w);


	/*
	// debug
	for (i=0; i<=iny; i++) {
	printf("%f  ",ypos[i]);
	}//
	*/

	// 3 �������� 2017.
	// ���������� ����� �������� �����.
	// ������������� �� ���������� ����������� ����������� ���� �����.
	for (int i_28 = 0; i_28 <= inyadd; i_28++) {
		SetLength(ypos, iny+1, iny + 2);
		ypos[iny+1] = yposadd[i_28];
		iny++;
	}
	Sort_method(ypos, iny);
	
	if (1) {
		integer inum_iter_ratio_good = 0;
		while (1) {

			doublereal max_size_ratio_y = 1.0;
			for (i = 0; i<iny - 1; i++) {

				doublereal dmax = 0.0;
				doublereal dmin = 0.0;
				if (fabs(ypos[i + 1] - ypos[i])>fabs(ypos[i + 2] - ypos[i + 1])) {
					dmax = fabs(ypos[i + 1] - ypos[i]);
					dmin = fabs(ypos[i + 2] - ypos[i + 1]);
				}
				else {
					dmax = fabs(ypos[i + 2] - ypos[i + 1]);
					dmin = fabs(ypos[i + 1] - ypos[i]);
				}
				if (dmax / dmin>max_size_ratio_y) {
					max_size_ratio_y = dmax / dmin;
				}
			}
			printf("y axis max size ratio is equal = %1.4f\n", max_size_ratio_y);
			if (max_size_ratio_y != max_size_ratio_y) {
				system("PAUSE");
			}
			//getchar();

			if (max_size_ratio_y < (etalon_max_size_ratio*1.1)) {
				break;
			}

			// ������������� max size ratio y axis.
			// ������� 9 ������ 2013 ����.

			bool bplus = false;
			max_size_ratio_y = 1.0;
			for (i = 0; i<iny - 1; i++) {

				bplus = false;
				doublereal dmax = 0.0;
				doublereal dmin = 0.0;
				if (fabs(ypos[i + 1] - ypos[i])>fabs(ypos[i + 2] - ypos[i + 1])) {
					dmax = fabs(ypos[i + 1] - ypos[i]);
					dmin = fabs(ypos[i + 2] - ypos[i + 1]);
				}
				else {
					bplus = true;
					dmax = fabs(ypos[i + 2] - ypos[i + 1]);
					dmin = fabs(ypos[i + 1] - ypos[i]);
				}
				if (dmax / dmin>etalon_max_size_ratio) {
					doublereal pos_candidate;
					if (bplus) {
						pos_candidate = ypos[i + 1] + 0.5*dmax;
					}
					else {
						pos_candidate = ypos[i] + 0.5*dmax;
					}
					SetLength(ypos, iny + 1, iny + 2);
					ypos[iny + 1] = pos_candidate;
					iny = iny + 1;
					//BubbleEnhSort(ypos, iny);
					Sort_method(ypos, iny);
					break;
				}
			}

			inum_iter_ratio_good++;

			//getchar();

		}

#if doubleintprecision == 1
		printf("inum_iter_ratio_good is %lld\n", inum_iter_ratio_good);
#else
		printf("inum_iter_ratio_good is %d\n", inum_iter_ratio_good);
#endif
		
	}

	integer *izintervalcount; // ����� ����������
	izintervalcount = new integer[inumboundaryz]; // �� ���� ������ ��� ����� ������.
	//for (i = 0; i<(inumboundaryz); i++) {
		//alphascal = (rzboundary[i + 1] - rzboundary[i]) / (rzboundary[inumboundaryz] - rzboundary[0]);
		//inowintervalcount = (int)(alphascal*inz);
		//if (inowintervalcount < min_elem_in_z_element) inowintervalcount = min_elem_in_z_element;
	//	izintervalcount[i] = inowintervalcount;
	//}

	/*����������� �������� ��� ���������� ������ �����.
	for (i = 0; i < (inumboundaryz); i++) {
		// ���� �� � solide �� ���� ������.
		// ���� �� Fluide �� ��� ������.
		doublereal cpos = 0.5*(rzboundary[i + 1] + rzboundary[i]);
		integer ibcur = 0; // ����� �������� ����� (������� �� ���������).
		for (integer i99 = 0; i99 < lb; i99++) {
			if ((cpos>b[i99].g.zS) && (cpos < b[i99].g.zE)) {
				ibcur = i99;
			}
		}
		if (b[ibcur].itype == SOLID) {
			izintervalcount[i] = 1;
		}
		if (b[ibcur].itype == FLUID) {
			izintervalcount[i] = 2;
		}
		if (b[ibcur].itype == HOLLOW) {
			izintervalcount[i] = 1;
		}
	}
	*/

	for (i = 0; i < (inumboundaryz); i++) {
		// ���� �� � solide �� ���� ������.
		// ���� �� Fluide �� ��� ������.
		doublereal cpos = 0.5*(rzboundary[i + 1] + rzboundary[i]);
		// � ��������� ������ ��������� fluid ����������� ����� ������ 
		// �� E � W �������� ������ Solid � hollow �������.
		bool bfound_onex_fluid_cv = false;
		// �������� ���������� �������������� ������ ����.
		integer ibcur = 0; // ����� �������� ����� (������� �� ���������).
		// ��������� ���������.
		for (integer ix = 0; ix < (inumboundaryx); ix++) {
			for (integer iy = 0; iy < (inumboundaryy); iy++) {
				doublereal qgeom = 10.0;
				// ��������� ���������� �������� ��.
				doublereal cposx = 0.5*(rxboundary[ix + 1] + rxboundary[ix]);
				doublereal cposy = 0.5*(ryboundary[iy + 1] + ryboundary[iy]);
				// ���������� ����� ����� � �������� ����������� ������ ��.
				//for (integer i99 = 0; i99 < lb; i99++) {
					//if ((cpos>b[i99].g.zS) && (cpos < b[i99].g.zE) &&
						//(cposx>b[i99].g.xS) && (cposx < b[i99].g.xE) &&
						//(cposy>b[i99].g.yS) && (cposy < b[i99].g.yE)) {
						//ibcur = i99;
					//}
				//}
				ibcur = ib_marker[ix + inumboundaryx*iy + inumboundaryx*inumboundaryy*i];

				if (b[ibcur].itype == FLUID) {
					// ������ ���������� : ���� ���� ���� ����� (E,W) ���� FLUID � ������ �������������� ���������� 10.0 ?
					// �.�. ���� �������� ��� FLUID ������ �� � ��� ��������� ������ ������ 10=qgeom �� ��������� �� ����� ���� �������� �������.
					if (i < inumboundaryz - 1) {
						doublereal cpos_plus = 0.5*(rzboundary[i + 2] + rzboundary[i + 1]);
						integer ibcur_plus = 0;
						// ���������� ����� ����� � �������� ����������� ������ ��.
						//for (integer i99 = 0; i99 < lb; i99++) {
							//if ((cpos_plus>b[i99].g.zS) && (cpos_plus < b[i99].g.zE) &&
								//(cposy>b[i99].g.yS) && (cposy < b[i99].g.yE) &&
								//(cposx>b[i99].g.xS) && (cposx < b[i99].g.xE)) {
								//ibcur_plus = i99;
							//}
						//}
						ibcur_plus = ib_marker[ix + inumboundaryx*iy + inumboundaryx*inumboundaryy*(i+1)];
						if (b[ibcur_plus].itype != FLUID) {
							if (i > 0) {
								doublereal cpos_minus = 0.5*(rzboundary[i] + rzboundary[i - 1]);
								integer ibcur_minus = 0;
								// ���������� ����� ����� � �������� ����������� ������ ��.
								//for (integer i99 = 0; i99 < lb; i99++) {
									//if ((cpos_minus>b[i99].g.zS) && (cpos_minus < b[i99].g.zE) &&
										//(cposy>b[i99].g.yS) && (cposy < b[i99].g.yE) &&
										//(cposx>b[i99].g.xS) && (cposx < b[i99].g.xE)) {
										//ibcur_minus = i99;
									//}
								//}
								ibcur_minus = ib_marker[ix + inumboundaryx*iy + inumboundaryx*inumboundaryy*(i-1)];
								if (b[ibcur_minus].itype != FLUID) {
									// ��������� FLUID ����
									bfound_onex_fluid_cv = true;
									break;
								}
								else {
									// ������ ����� ���� � �� ���� FLUID
									// �������� �� qgeom
									doublereal q1 = fabs((rzboundary[i] - rzboundary[i - 1]) / (rzboundary[i + 1] - rzboundary[i]));
									doublereal qgeom_inv = 1.0 / qgeom;
									if (q1 < qgeom_inv) {
										bfound_onex_fluid_cv = true;
										break;
									}
									if (q1 > qgeom) {
										// W ������� ���� ���� �������, �� ���� ������ �������������� ��� ��������� W �����. 
									}
								}
							}
							else {
								// � ������� ������ ������ ���� !!!
								// ��������� FLUID ����
								bfound_onex_fluid_cv = true;
								break;
							}
						}
						else {
							// ������� ������ ����� FLUID
							// ���������� ������������ ����� ����� ���� ������� ������� ������ �� ������������� qgeom ��������
							// �.�. ��� ������� �����
							doublereal q2 = fabs((rzboundary[i + 2] - rzboundary[i + 1]) / (rzboundary[i + 1] - rzboundary[i]));
							doublereal qgeom_inv = 1.0 / qgeom;
							if (q2 < qgeom_inv) {
								// ������� ������ �������������������,
								// ��������� � ������������ ������.
								if (i > 0) {
									doublereal cpos_minus = 0.5*(rzboundary[i] + rzboundary[i - 1]);
									integer ibcur_minus = 0;
									// ���������� ����� ����� � �������� ����������� ������ ��.
									//for (integer i99 = 0; i99 < lb; i99++) {
										//if ((cpos_minus>b[i99].g.zS) && (cpos_minus < b[i99].g.zE) &&
											//(cposy>b[i99].g.yS) && (cposy < b[i99].g.yE) &&
											//(cposx>b[i99].g.xS) && (cposx < b[i99].g.xE)) {
											//ibcur_minus = i99;
										//}
									//}
									ibcur_minus = ib_marker[ix + inumboundaryx*iy + inumboundaryx*inumboundaryy*(i - 1)];
									if (b[ibcur_minus].itype != FLUID) {
										// ��������� FLUID ����
										bfound_onex_fluid_cv = true;
										break;
									}
									else {
										// ������ ����� ���� � �� ���� FLUID
										// �������� �� qgeom
										doublereal q1 = fabs((rzboundary[i] - rzboundary[i - 1]) / (rzboundary[i + 1] - rzboundary[i]));
										doublereal qgeom_inv = 1.0 / qgeom;
										if (q1 < qgeom_inv) {
											bfound_onex_fluid_cv = true;
											break;
										}
										if (q1 > qgeom) {
											// W ������� ���� ���� �������, �� ���� ������ �������������� ��� ��������� W �����. 
										}
									}
								}
								else {
									// � ������� ������ ������ ���� !!!
									// ��������� FLUID ����
									bfound_onex_fluid_cv = true;
									break;
								}
							}
						}
					}
					else {
						// ��������� ������� ������
						// ������� ����� ������ �����������
						if (i > 0) {
							doublereal cpos_minus = 0.5*(rzboundary[i] + rzboundary[i - 1]);
							integer ibcur_minus = 0;
							// ���������� ����� ����� � �������� ����������� ������ ��.
							//for (integer i99 = 0; i99 < lb; i99++) {
								//if ((cpos_minus>b[i99].g.zS) && (cpos_minus < b[i99].g.zE) &&
									//(cposy>b[i99].g.yS) && (cposy < b[i99].g.yE) &&
									//(cposx > b[i99].g.xS) && (cposx < b[i99].g.xE)) {
									//ibcur_minus = i99;
								//}
							//}
							ibcur_minus = ib_marker[ix + inumboundaryx*iy + inumboundaryx*inumboundaryy*(i - 1)];
							if (b[ibcur_minus].itype != FLUID) {
								// ��������� FLUID ����
								bfound_onex_fluid_cv = true;
								break;
							}
							else {
								// ������ ����� ���� � �� ���� FLUID
								// �������� �� qgeom
								doublereal q1 = fabs((rzboundary[i] - rzboundary[i - 1]) / (rzboundary[i + 1] - rzboundary[i]));
								doublereal qgeom_inv = 1.0 / qgeom;
								if (q1 < qgeom_inv) {
									bfound_onex_fluid_cv = true;
									break;
								}
								if (q1 > qgeom) {
									// W ������� ���� ���� �������, �� ���� ������ �������������� ��� ��������� W �����. 
								}
							}
						}
						else {
							// ��� �������� ��� � ��� ����� ������ ���� ������.
							// ������ ���� � �������.
							bfound_onex_fluid_cv = true;
							break;
						}
					}
				}
			}
		}

		if (bfound_onex_fluid_cv == false) {
			izintervalcount[i] = 1;
		}
		else {
			//izintervalcount[i] = 2;

			// FLUID
			bool b2div = false;
			for (integer i_3 = 0; i_3 < (inumboundaryx); i_3++) {
				for (integer i_4 = 0; i_4 < (inumboundaryy); i_4++) {
					doublereal xp_3 = 0.5*(rxboundary[i_3 + 1] + rxboundary[i_3]);
					doublereal yp_3 = 0.5*(ryboundary[i_4 + 1] + ryboundary[i_4]);
					doublereal zp_1 = 0.5*(rzboundary[i + 1] + rzboundary[i]);
					doublereal zp_2 = zp_1;
					doublereal zp_3 = zp_1;
					if (i < inumboundaryz - 1) {
						zp_2 = 0.5*(rzboundary[i + 2] + rzboundary[i + 1]);
					}
					if (i > 0) {
						zp_3 = 0.5*(rzboundary[i - 1] + rzboundary[i]);
					}
					// ���������� ����� ����� �� ���������� �����.
					//myisblock_id(integer lb, BLOCK* &b, doublereal x11, doublereal y11, doublereal z11)
					if (!((((b[myisblock_id(lb, b, xp_3, yp_3, zp_1)].itype == HOLLOW)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_1)].itype == SOLID)) && ((b[myisblock_id(lb, b, xp_3, yp_3, zp_2)].itype == HOLLOW)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_2)].itype == SOLID)) && ((b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == HOLLOW)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == SOLID))) || ((b[myisblock_id(lb, b, xp_3, yp_3, zp_1)].itype == SOLID) && (b[myisblock_id(lb, b, xp_3, yp_3, zp_2)].itype == SOLID) && (b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == SOLID)) || (((b[myisblock_id(lb, b, xp_3, yp_3, zp_1)].itype == FLUID)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_1)].itype == HOLLOW)) && ((b[myisblock_id(lb, b, xp_3, yp_3, zp_2)].itype == FLUID)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_2)].itype == HOLLOW)) && ((b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == FLUID)||(b[myisblock_id(lb, b, xp_3, yp_3, zp_3)].itype == HOLLOW)))))
					{
						// ���� ������ ���� ������� ����� ����� �� ������������� ������������ solid ��� fluid.
						b2div = true;
					}
				}
			}
			if (b2div) {
				izintervalcount[i] = 2;
			}
			else {
				printf("inc Z");
				//getchar();
				izintervalcount[i] = 1;
			}

		}
	}

	// ���������� ������� �����
	iposmark = 1;
	doublereal dz;
	integer izoldsize = 0;
	SetLength(zpos, izoldsize, 1);
	izoldsize = 1;
	for (i = 0; i<(inumboundaryz); i++)
	{
		// ����� ����� ����� ���� ������ ������
		//if ((izintervalcount[i] - 2) % 2 == 0) izintervalcount[i]++;
		//integer n2 = (int)((izintervalcount[i] - 1) / 2); // ���������� � ������� ������� 
		//if (bgeomz) n2 = ibalancen2(n2, qzB, qzT, rzboundary, izintervalcount[i], i, iposmark); // ������������ 
#if doubleintprecision == 1
		//printf("n2=%lld\n",n2); // debug
#else
		//printf("n2=%d\n",n2); // debug
#endif
		
		//doublereal qn2B = qzB; // ������ �������
		// ������� �������� � 0.25 �� ����� ���������� �� ��������� ��-�� ����������� ������ ����������.
		//for (integer i1 = 1; i1<n2; i1++) qn2B *= qzB; // ���������� � �������.
		//doublereal b1B = (rzboundary[i + 1] - rzboundary[i])*(qzB - 1.0) / (2.0*(qn2B - 1.0));
		//doublereal qn2T = qzT; // ������� �������
		//for (integer i1 = 1; i1<(izintervalcount[i] - n2); i1++) qn2T *= qzT; // ���������� � �������.
		//doublereal b1T = (rzboundary[i + 1] - rzboundary[i])*(qzT - 1.0) / (2.0*(qn2T - 1.0));

		dz = (rzboundary[i + 1] - rzboundary[i]) / (izintervalcount[i]);
		SetLength(zpos, izoldsize, iposmark + izintervalcount[i]+1);
		izoldsize = iposmark + izintervalcount[i]+1;
		for (k = iposmark; k <= iposmark + izintervalcount[i] - 2+1; k++)
		{
			if (!bgeomz) {
				// ����������� �����
				zpos[k] = rzboundary[i] + (k - iposmark)*dz;
			}
			/*else
			{
				// ������������� �����
				// �� ������ �������������� ���������� � ����� ����������.
				integer ic1 = k - iposmark;
				doublereal madd;
				if (ic1<n2) {
					madd = b1B;
					for (integer i1 = 0; i1<ic1; i1++) madd *= qzB;
					if (ic1 == n2 - 1) madd = 0.5*(rzboundary[i + 1] + rzboundary[i]) - zpos[k - 1]; // ����� �������� ������ ����������.
					//printf("first.:");
				}
				else
				{
					madd = b1T;
					for (integer i1 = izintervalcount[i] - 1 - ic1; i1>0; i1--) madd *= qzT;
					// ��� ���� �������������.
					//if (k==iposmark+izintervalcount[i]-2) madd*=rzboundary[i+1]-zpos[k-1]; // ����� �������� ������ ����������.
					//printf("second.:");
				}
				if (k == iposmark) zpos[k] = rzboundary[i]; // ����� �������� ������ ����������.
				else {
					// ����������� ��� ��� ������� �� ������� �� ��������������
					zpos[k] = zpos[k - 1] + madd;
				}
				#if doubleintprecision == 1
					//printf("zpos[%lld]=%f, madd=%f\n",k,zpos[k],madd);
				#else
					//printf("zpos[%d]=%f, madd=%f\n",k,zpos[k],madd);
				#endif
				
				//getchar();
			}*/
		}
		iposmark = iposmark + izintervalcount[i] - 1+1;
	}
	SetLength(zpos, izoldsize, iposmark + 1);
	zpos[iposmark] = rzboundary[inumboundaryz];
#if doubleintprecision == 1
	//printf("finish zpos[%lld]=%f\n",iposmark,zpos[iposmark]);
#else
	//printf("finish zpos[%d]=%f\n",iposmark,zpos[iposmark]);
#endif

	
	//getchar();
	inz = iposmark;
	for (i = 0; i<inz; i++) zpos[i] = zpos[i + 1]; // ����� ����� �� 1
	SetLength(zpos, inz + 1, inz);
	inz--; // ���������� ���������� � ���� � ������������� ��������� inz

//	for (i = 0; i<adapt_z; i++) simplecorrect_meshgen_z(zpos, inz, lb, ls, lw, b, s, w);

	//for (i = 0; i <= inz; i++) {
		//printf("%e  ", zpos[i]);
	//}
	//getchar();

	integer inz_fix = inz;
	// ������������� ��������� XY
	for (i = 0; i < ls; i++) {
		if (source_indexpopadaniqnagranXY[i]) {
			doublereal xc = 0.5*(s[i].g.xS + s[i].g.xE);
			doublereal yc = 0.5*(s[i].g.yS + s[i].g.yE);
			doublereal zg = s[i].g.zS;
			// ����� ������� +Z �� ����� zpos
			// ����� ����� ��.
			// ���� �� ����������� Solid block �� �������� ������� � ���� ���� ������.
			// ��������� maxsize ratio 2 �� ������.
			// ���� ������� ��������� �� �� -Z � ������� ����.
			integer i55_found = -2;
			for (integer i55 = 0; i55 <= inz_fix; i55++) {
				if (fabs(zpos[i55] - zg) < 1.0e-40) {
					i55_found = i55;
					break;
				}
			}
			if (i55_found >= 0) {
				if (i55_found < inz_fix) {
					doublereal zg1 = 0.5*(zg + zpos[i55_found + 1]);
					printf("zg1=%e\n", zg1);
					integer i56_found = -2;
					for (integer ib55 = 0; ib55 < lb; ib55++) {
						
							if ((xc > b[ib55].g.xS) && (xc < b[ib55].g.xE) && (yc > b[ib55].g.yS) && (yc < b[ib55].g.yE) && (zg1 > b[ib55].g.zS) && (zg1 < b[ib55].g.zE))
							{
								i56_found = ib55;
							}
					}

					bool bzero_pos = true;
					doublereal zg2 = zpos[0] - 0.5*fabs(zpos[1] - zpos[0]);
					if (i55_found>0) {
						zg2 = 0.5*(zg + zpos[i55_found - 1]);
						bzero_pos = false;
					}
					printf("zg2=%e\n", zg2);
					integer i57_found = -2;
					for (integer ib57 = 0; ib57 < lb; ib57++) {
						
							if ((xc > b[ib57].g.xS) && (xc < b[ib57].g.xE) && (yc > b[ib57].g.yS) && (yc < b[ib57].g.yE) && (zg2 > b[ib57].g.zS) && (zg2 < b[ib57].g.zE))
							{
								if (zg2 > zpos[0]) {
									i57_found = ib57;
								}
							}
					}

					if (i56_found >= 0) {
						if (b[i56_found].itype == SOLID) {
							if ((i57_found >= 0) && (b[i57_found].itype == SOLID)) {
								// �� �������� �������� ����� � ���� � ������� �����������������.
								// comparison_lam ����� ������ ���� ���������������� ����� i56 ������.
								if (comparison_lam(matlist, b, i56_found, i57_found, 25.0)) {
									// � ����� i56 ���������������� ����.
									s[i].g.zS = zg1;
									s[i].g.zE = zg1;
									addboundary(zpos, inz, zg1);
								}
								else {
									// � ����� i57 ���������������� ����.
									s[i].g.zS = zg2;
									s[i].g.zE = zg2;
									addboundary(zpos, inz, zg2);
								}
							}
							else {
								// ������ ����� ����.
								s[i].g.zS = zg1;
								s[i].g.zE = zg1;
								addboundary(zpos, inz, zg1);
							}
						}
						else {

							if (i57_found >= 0) {
								if (b[i57_found].itype == SOLID) {

									// ������ ����� ����.
									printf("zg1==%e\n", zg1);
									doublereal zgolg = zpos[1];
									if (inz == 1) {
										// ������� ������������� ��������� �����.
										zgolg = 0.5*(zpos[0] + zg1);
										addboundary(zpos, inz, zgolg);
										zgolg = 0.5*(zpos[1] + zg1);
										addboundary(zpos, inz, zgolg);
										zgolg = 0.5*(zpos[0] + zg1);
										// ���������� �� ���� ������ ��������� ������� ��� ������.
										s[i].g.zS = zgolg;
										s[i].g.zE = zgolg;
									}
									else {
										if (bzero_pos) {
											// �������� � ������� 0.0 ��������� �����������.
											zgolg = 0.5*(zpos[0] + zg1);
											addboundary(zpos, inz, zgolg);
											zgolg = 0.5*(zpos[1] + zg1);
											addboundary(zpos, inz, zgolg);
											zgolg = 0.5*(zpos[0] + zg1);
											// ���������� �� ���� ������ ��������� ������� ��� ������.
											s[i].g.zS = zgolg;
											s[i].g.zE = zgolg;
										}
										else {
											s[i].g.zS = zg1;
											s[i].g.zE = zg1;
										}
									}
									addboundary(zpos, inz, zg1);
									

								}
							}
							else {
								printf("ERROR : sourse na granice dvus hollow or fluid blockov.");
								system("PAUSE");
								exit(1);
							}
						}
					}
				}
				else {
					doublereal zg2 = 0.5*(zg + zpos[i55_found - 1]);
					printf("zg2=%e\n", zg2);
					integer i57_found = -2;
					for (integer ib57 = 0; ib57 < lb; ib57++) {						
							if ((xc > b[ib57].g.xS) && (xc < b[ib57].g.xE) && (yc > b[ib57].g.yS) && (yc < b[ib57].g.yE) && (zg2 > b[ib57].g.zS) && (zg2 < b[ib57].g.zE))
							{
								i57_found = ib57;
							}
					}
					if (i57_found >= 0) {
						if (b[i57_found].itype == SOLID) {
							// ������ ����� ����.
							s[i].g.zS = zg2;
							s[i].g.zE = zg2;
							addboundary(zpos, inz, zg2);
						}
					}
					else {
						printf("ERROR : sourse na granice dvus hollow or fluid blockov.");
						system("PAUSE");
						exit(1);
					}
				}
			}
		}
	}
	//BubbleEnhSort(zpos, inz);
	Sort_method(zpos, inz);
	//SetLength(zpos, inz + 2, inz);
	//inz++;

	/*
	for (i = 0; i < ls; i++) {
		if (source_indexpopadaniqnagranYZ[i] == false) {
			printf("error\n");
			getchar();
		}
	}
	*/

	integer inx_fix = inx;
	// ������������� ��������� YZ
	for (i = 0; i < ls; i++) {
		if (source_indexpopadaniqnagranYZ[i]) {
			doublereal zc = 0.5*(s[i].g.zS + s[i].g.zE);
			doublereal yc = 0.5*(s[i].g.yS + s[i].g.yE);
			doublereal xg = s[i].g.xS;
			// ����� ������� +Z �� ����� zpos
			// ����� ����� ��.
			// ���� �� ����������� Solid block �� �������� ������� � ���� ���� ������.
			// ��������� maxsize ratio 2 �� ������.
			// ���� ������� ��������� �� �� -Z � ������� ����.
			integer i55_found = -2;
			for (integer i55 = 0; i55 <= inx_fix; i55++) {
				if (fabs(xpos[i55] - xg) < 1.0e-40) {
					i55_found = i55;
					break;
				}
			}
			if (i55_found >= 0) {
				if (i55_found < inx_fix) {
					doublereal xg1 = 0.5*(xg + xpos[i55_found + 1]);
					printf("xg1=%e\n", xg1);
					integer i56_found = -2;
					for (integer ib55 = 0; ib55 < lb; ib55++) {
						
							if ((zc > b[ib55].g.zS) && (zc < b[ib55].g.zE) && (yc > b[ib55].g.yS) && (yc < b[ib55].g.yE) && (xg1 > b[ib55].g.xS) && (xg1 < b[ib55].g.xE))
							{
								i56_found = ib55;
							}
					}
					bool bzero_pos = true;
					doublereal xg2 = xpos[0]-0.5*fabs(xpos[1]-xpos[0]);
					if (i55_found>0) {
						xg2=0.5*(xg + xpos[i55_found - 1]);
						bzero_pos = false;
					}
					printf("xg2=%e\n", xg2);
					integer i57_found = -2;
					for (integer ib57 = 0; ib57 < lb; ib57++) {
							if ((zc > b[ib57].g.zS) && (zc < b[ib57].g.zE) && (yc > b[ib57].g.yS) && (yc < b[ib57].g.yE) && (xg2 > b[ib57].g.xS) && (xg2 < b[ib57].g.xE))
							{
								if (xg2 > xpos[0]) {
									i57_found = ib57;
								}
							}
						
					}

					

					if (i56_found >= 0) {
						if (b[i56_found].itype == SOLID) {
							// TODO 11.07.2016
							if ((i57_found >= 0) && (b[i57_found].itype == SOLID)) {
								// �� �������� �������� ����� � ���� � ������� �����������������.
								// comparison_lam ����� ������ ���� ���������������� ����� i56 ������.
								if (comparison_lam(matlist, b, i56_found, i57_found, 25.0)) {
									// � ����� i56 ���������������� ����.
									s[i].g.xS = xg1;
									s[i].g.xE = xg1;
									addboundary(xpos, inz, xg1);
								}
								else {
									// � ����� i57 ���������������� ����.
									s[i].g.xS = xg2;
									s[i].g.xE = xg2;
									addboundary(xpos, inz, xg2);
								}
							}
							else {
								// ������ ����� ����.
								printf("xg1==%e\n",xg1);
								doublereal xgolg = xpos[1];
								if (inx == 1) {
									// ������� ������������� ��������� �����.
									xgolg = 0.5*(xpos[0] + xg1);
									addboundary(xpos, inx, xgolg);
									xgolg = 0.5*(xpos[1] + xg1);
									addboundary(xpos, inx, xgolg);
									xgolg = 0.5*(xpos[0] + xg1);
									// ���������� �� ���� ������ ��������� ������� ��� ������.
									s[i].g.xS = xgolg;
									s[i].g.xE = xgolg;
								}
								else {
									if (bzero_pos) {
										// �������� � ������� 0.0.
										// ��������� �����������.
										xgolg = 0.5*(xpos[0] + xg1);
										addboundary(xpos, inx, xgolg);
										xgolg = 0.5*(xpos[1] + xg1);
										addboundary(xpos, inx, xgolg);
										xgolg = 0.5*(xpos[0] + xg1);
										// ���������� �� ���� ������ ��������� ������� ��� ������.
										s[i].g.xS = xgolg;
										s[i].g.xE = xgolg;
									}
									else {
										s[i].g.xS = xg1;
										s[i].g.xE = xg1;
									}
								}
								addboundary(xpos, inx, xg1);
								
								
								//printf("ok");
							}
						}
						else {

							if (i57_found >= 0) {
								if (b[i57_found].itype == SOLID) {
									// ������ ����� ����.
									s[i].g.xS = xg2;
									s[i].g.xE = xg2;
									addboundary(xpos, inx, xg2);
								}
							}
							else {
								printf("ERROR : sourse na granice dvus hollow or fluid blockov.");
								system("PAUSE");
								exit(1);
							}
						}
					}
				}
				else {
					doublereal xg2 = 0.5*(xg + xpos[i55_found - 1]);
					printf("xg2=%e\n", xg2);
					integer i57_found = -2;
					for (integer ib57 = 0; ib57 < lb; ib57++) {
						if ((zc>b[ib57].g.zS) && (zc<b[ib57].g.zE) && (yc>b[ib57].g.yS) && (yc<b[ib57].g.yE) && (xg2>b[ib57].g.xS) && (xg2 < b[ib57].g.xE))
						{
							i57_found = ib57;
						}
					}
					if (i57_found >= 0) {
						if (b[i57_found].itype == SOLID) {
							// ������ ����� ����.
							s[i].g.xS = xg2;
							s[i].g.xE = xg2;
							addboundary(xpos, inx, xg2);
						}
					}
					else {
						printf("ERROR : sourse na granice dvus hollow or fluid blockov.");
						system("PAUSE");
						exit(1);
					}
				}
			}
		}
	}
	//getchar();
	//BubbleEnhSort(xpos, inx);
	Sort_method(xpos, inx);

	/*
	for (i = 0; i <= inx; i++) {
		printf("%e\n", xpos[i]);
	}
	printf("%e %e %e\n", s[0].g.xS, s[1].g.xS, s[2].g.xS);
	getchar();
	*/

	integer iny_fix = iny;
	// ������������� ��������� XZ
	for (i = 0; i < ls; i++) {
		if (source_indexpopadaniqnagranXZ[i]) {
			doublereal xc = 0.5*(s[i].g.xS + s[i].g.xE);
			doublereal zc = 0.5*(s[i].g.zS + s[i].g.zE);
			doublereal yg = s[i].g.yS;
			// ����� ������� +Z �� ����� zpos
			// ����� ����� ��.
			// ���� �� ����������� Solid block �� �������� ������� � ���� ���� ������.
			// ��������� maxsize ratio 2 �� ������.
			// ���� ������� ��������� �� �� -Z � ������� ����.
			integer i55_found = -2;
			for (integer i55 = 0; i55 <= iny_fix; i55++) {
				if (fabs(ypos[i55] - yg) < 1.0e-40) {
					i55_found = i55;
					break;
				}
			}
			if (i55_found >= 0) {
				if (i55_found < iny_fix) {
					doublereal yg1 = 0.5*(yg + ypos[i55_found + 1]);
					printf("yg1=%e\n", yg1);
					integer i56_found = -2;
					for (integer ib55 = 0; ib55 < lb; ib55++) {
						if ((xc>b[ib55].g.xS) && (xc<b[ib55].g.xE) && (zc>b[ib55].g.zS) && (zc<b[ib55].g.zE) && (yg1>b[ib55].g.yS) && (yg1 < b[ib55].g.yE))
						{
							i56_found = ib55;
						}
					}
					
					bool bzero_pos = true;
					doublereal yg2 = ypos[0] - 0.5*fabs(ypos[1] - ypos[0]);
					if (i55_found>0) {
						yg2 = 0.5*(yg + ypos[i55_found - 1]);
						bzero_pos = false;
					}

					printf("yg2=%e\n", yg2);
					integer i57_found = -2;
					for (integer ib57 = 0; ib57 < lb; ib57++) {
						if ((xc>b[ib57].g.xS) && (xc<b[ib57].g.xE) && (zc>b[ib57].g.zS) && (zc<b[ib57].g.zE) && (yg2>b[ib57].g.yS) && (yg2 < b[ib57].g.yE))
						{
							if (yg2 > ypos[0]) {
								i57_found = ib57;
							}
						}
					}


					if (i56_found >= 0) {
						if (b[i56_found].itype == SOLID) {

							if ((i57_found >= 0) && (b[i57_found].itype == SOLID)) {
								// �� �������� �������� ����� � ���� � ������� �����������������.
								// comparison_lam ����� ������ ���� ���������������� ����� i56 ������.
								if (comparison_lam(matlist, b, i56_found, i57_found, 25.0)) {
									// � ����� i56 ���������������� ����.
									s[i].g.yS = yg1;
									s[i].g.yE = yg1;
									addboundary(ypos, iny, yg1);
								}
								else {
									// � ����� i57 ���������������� ����.
									s[i].g.yS = yg2;
									s[i].g.yE = yg2;
									addboundary(ypos, iny, yg2);
								}
							}
							else {

								// ������ ����� ����.
								printf("yg1==%e\n", yg1);
								doublereal ygolg = ypos[1];
								if (iny == 1) {
									// ������� ������������� ��������� �����.
									ygolg = 0.5*(ypos[0] + yg1);
									addboundary(ypos, iny, ygolg);
									ygolg = 0.5*(ypos[1] + yg1);
									addboundary(ypos, iny, ygolg);
									ygolg = 0.5*(ypos[0] + yg1);
									// ���������� �� ���� ������ ��������� ������� ��� ������.
									s[i].g.yS = ygolg;
									s[i].g.yE = ygolg;
								}
								else {
									if (bzero_pos) {
										// �������� � ������� 0.0.
										// ��������� �����������.
										ygolg = 0.5*(ypos[0] + yg1);
										addboundary(ypos, iny, ygolg);
										ygolg = 0.5*(ypos[1] + yg1);
										addboundary(ypos, iny, ygolg);
										ygolg = 0.5*(ypos[0] + yg1);
										s[i].g.yS = ygolg;
										s[i].g.yE = ygolg;
									}
									else {
										s[i].g.yS = yg1;
										s[i].g.yE = yg1;
									}
								}
								addboundary(ypos, iny, yg1);
								

							}
						}
						else {

							if (i57_found >= 0) {
								if (b[i57_found].itype == SOLID) {
									// ������ ����� ����.
									s[i].g.yS = yg2;
									s[i].g.yE = yg2;
									addboundary(ypos, iny, yg2);
								}
							}
							else {
								printf("ERROR : sourse na granice dvus hollow or fluid blockov.");
								system("PAUSE");
								exit(1);
							}
						}
					}
				}
				else {
					doublereal yg2 = 0.5*(yg + ypos[i55_found - 1]);
					printf("yg2=%e\n", yg2);
					integer i57_found = -2;
					for (integer ib57 = 0; ib57 < lb; ib57++) {
						if ((xc>b[ib57].g.xS) && (xc<b[ib57].g.xE) && (zc>b[ib57].g.zS) && (zc<b[ib57].g.zE) && (yg2>b[ib57].g.yS) && (yg2 < b[ib57].g.yE))
						{
							i57_found = ib57;
						}
					}
					if (i57_found >= 0) {
						if (b[i57_found].itype == SOLID) {
							// ������ ����� ����.
							s[i].g.yS = yg2;
							s[i].g.yE = yg2;
							addboundary(ypos, iny, yg2);
						}
					}
					else {
						printf("ERROR : sourse na granice dvus hollow or fluid blockov.");
						system("PAUSE");
						exit(1);
					}
				}
			}
		}
	}
	//BubbleEnhSort(ypos, iny);
	Sort_method(ypos, iny);
	// debug
	
	//for (i=0; i<=inz; i++) {
	  // printf("%e  ",zpos[i]);
	//}
	//getchar();//
	
	// 3 �������� 2017.
	// ���������� ����� �������� �����.
	// ������������� �� ���������� ����������� ����������� ���� �����.
	for (int i_28 = 0; i_28 <= inzadd; i_28++) {
		SetLength(zpos, inz+1, inz + 2);
		zpos[inz+1] = zposadd[i_28];
		inz++;
	}
	Sort_method(zpos, inz);


	if (1) {
		integer inum_iter_ratio_good = 0;
		while (1) {

			doublereal max_size_ratio_z = 1.0;
			for (i = 0; i<inz - 1; i++) {

				doublereal dmax = 0.0;
				doublereal dmin = 0.0;
				if (fabs(zpos[i + 1] - zpos[i])>fabs(zpos[i + 2] - zpos[i + 1])) {
					dmax = fabs(zpos[i + 1] - zpos[i]);
					dmin = fabs(zpos[i + 2] - zpos[i + 1]);
				}
				else {
					dmax = fabs(zpos[i + 2] - zpos[i + 1]);
					dmin = fabs(zpos[i + 1] - zpos[i]);
				}
				if (dmax / dmin>max_size_ratio_z) {
					max_size_ratio_z = dmax / dmin;
				}
			}
			printf("z axis max size ratio is equal = %1.4f\n", max_size_ratio_z);
			if (max_size_ratio_z != max_size_ratio_z) {
				system("PAUSE");
			}
			//getchar();

			if (max_size_ratio_z < (etalon_max_size_ratio*1.1)) {
				break;
			}

			// ������������� max size ratio z axis.
			// ������� 9 ������ 2013 ����.

			bool bplus = false;
			max_size_ratio_z = 1.0;
			for (i = 0; i<inz - 1; i++) {

				bplus = false;
				doublereal dmax = 0.0;
				doublereal dmin = 0.0;
				if (fabs(zpos[i + 1] - zpos[i])>fabs(zpos[i + 2] - zpos[i + 1])) {
					dmax = fabs(zpos[i + 1] - zpos[i]);
					dmin = fabs(zpos[i + 2] - zpos[i + 1]);
				}
				else {
					bplus = true;
					dmax = fabs(zpos[i + 2] - zpos[i + 1]);
					dmin = fabs(zpos[i + 1] - zpos[i]);
				}
				if (dmax / dmin>etalon_max_size_ratio) {
					doublereal pos_candidate;
					if (bplus) {
						pos_candidate = zpos[i + 1] + 0.5*dmax;
					}
					else {
						pos_candidate = zpos[i] + 0.5*dmax;
					}
					SetLength(zpos, inz + 1, inz + 2);
					zpos[inz + 1] = pos_candidate;
					inz = inz + 1;
					//BubbleEnhSort(zpos, inz);
					Sort_method(zpos, inz);
					break;
				}
			}

			inum_iter_ratio_good++;

			//getchar();

		}
#if doubleintprecision == 1
		printf("inum_iter_ratio_good is %lld\n", inum_iter_ratio_good);
#else
		printf("inum_iter_ratio_good is %d\n", inum_iter_ratio_good);
#endif

		
	}

	// ��������� �������� ����� �� ������� �������� � ��������� 30.0
	// ��� �� flowvision.
	quolite_refinement(inx, iny, inz, xpos, ypos, zpos);


	// ������������ ����������� ������.
	delete[] ib_marker;
	delete[] rxboundary;
	delete[] ryboundary;
	delete[] rzboundary;
	delete[] ixintervalcount;
	delete[] iyintervalcount;
	delete[] izintervalcount;
	delete[] source_indexpopadaniqnagranXY;
	delete[] source_indexpopadaniqnagranYZ;
	delete[] source_indexpopadaniqnagranXZ;


} // coarsemeshgen

#endif