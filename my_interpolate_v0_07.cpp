// ���� my_interpolate_v0_07.cpp ��������
// ��������� ����������� ��� ������������� �������.
// �������� ������� ���������:
// 1. ������� ���������� ������ ����������� �� ������������� ����� � ��������� O(h^2).
// ������ ������� ������������ �� ����� �.�. ������� � ������������ ������ ������� �� ������������� �����.
// ������� �������� ����������� � ������ �� ���������� �������� �� ��� ������ � ������ � ����������� � ����������� �����.

// ������ ������� ���������� � ������ 15 ��� 2012 ����.

#ifndef MY_INTERPOLATE_V0_07_CPP
#define MY_INTERPOLATE_V0_07_CPP 1

// ����������� �� ������� �������� � �������� �� ����� ��.
// ������� ����� xg.
doublereal DFDXg(doublereal x1, doublereal x2, doublereal x3, doublereal x4, doublereal xg,
	      doublereal f1, doublereal f2, doublereal f3, doublereal f4) {

	doublereal DFDXgran=0.0;

	const integer parabola_MNK=0; // ����������� �� �������� ����������� �� ������� ���������� ���������.
	// �������� : line_MNK ����� �� ������������, �.�. ���������������� �����������, �� ������� ������
	// ������� tgf2023_01 ��� ���������� ��� ����� ������� ����������� �� ���������, ����������� ����� ������������� �� ���������
	// ����� ��� 40 ���������� �������� � �����. �������� ���� ������������.
	const integer line_MNK=1; // ������� ���� ������� ������, ������ ��������� �� ������� ��� �� ������ ������.
	// ������ ����� ����� �� ������������� ������������ ! ����� ���������� ����������.
	const integer cubic_parabola=2; // ���������� �������� �� 4 ������ � �� �� �����������.

	integer imetod=parabola_MNK;

	
	if (imetod==parabola_MNK) 
	{

	     // ���������.

	     // ����� ���������� ������������ �������� ��������� ������������ ��������.
	     // �������� �� 4 ������ �������� ��������� ���.

		doublereal* a = NULL;
		a=new doublereal[3];
		doublereal** AM = NULL;
		AM=new doublereal*[3];
	     for (integer i=0; i<3; i++) {
		     AM[i]=new doublereal[3]; // ������� ����
	     }
		 doublereal* b = NULL;
		 b=new doublereal[3]; // ������ �����

	     for (integer i=0; i<3; i++) {
		     for (integer j=0; j<3; j++) {
		    	AM[i][j]=0.0; // �������������
		     }
		     b[i]=0.0;
		     a[i]=0.0;
	     }

	     AM[0][0]=4.0;
	     AM[0][1]=x1+x2+x3+x4;
	     AM[0][2]=x1*x1+x2*x2+x3*x3+x4*x4;
	     b[0]=f1+f2+f3+f4;
	     AM[1][0]=x1+x2+x3+x4;
	     AM[1][1]=x1*x1+x2*x2+x3*x3+x4*x4;
	     AM[1][2]=x1*x1*x1+x2*x2*x2+x3*x3*x3+x4*x4*x4;
	     b[1]=f1*x1+f2*x2+f3*x3+f4*x4;
	     AM[2][0]=x1*x1+x2*x2+x3*x3+x4*x4;
	     AM[2][1]=x1*x1*x1+x2*x2*x2+x3*x3*x3+x4*x4*x4;
	     AM[2][2]=x1*x1*x1*x1+x2*x2*x2*x2+x3*x3*x3*x3+x4*x4*x4*x4;
	     b[2]=f1*x1*x1+f2*x2*x2+f3*x3*x3+f4*x4*x4;

	     integer nodes=3;
	     eqsolve_simple_gauss(AM, nodes, b, a); // ������� ���� ������� ������.

	     // ������������ ������
		 if (AM != NULL) {
			 for (integer i = 0; i < 3; i++) {
				 if (AM[i] != NULL) {
					 delete[] AM[i];
				 }
			 }
			 delete[] AM;
		 }
		 if (b != NULL) {
			 delete[] b;
		 }

	     //f(x)=a[0]+a[1]*x+a[2]*x^2;

	     //������ ����������� � ����� xg ����� 2.0*a[2]*xg+a[1];
	     DFDXgran=2.0*a[2]*xg+a[1];

		 if (a != NULL) {
			 delete[] a;
		 }

	}

	if (imetod==line_MNK) {  
		// �� 4 ������ �� ������� ���������� ��������� ������ ����� � ������� � ����������� ������������.


		doublereal* a = NULL;
		a=new doublereal[2];
		doublereal** AM = NULL;
		AM=new doublereal*[2];
	    for (integer i=0; i<2; i++) {
		    AM[i]=new doublereal[3]; // ������� ����
	    }
		doublereal* b = NULL;
		b=new doublereal[2]; // ������ �����

	    for (integer i=0; i<2; i++) {
		    for (integer j=0; j<2; j++) {
			    AM[i][j]=0.0; // �������������
		    }
		    b[i]=0.0;
		    a[i]=0.0;
	    }

	    AM[0][0]=4.0;
	    AM[0][1]=x1+x2+x3+x4;
	    b[0]=f1+f2+f3+f4;
	    AM[1][0]=x1+x2+x3+x4;
	    AM[1][1]=x1*x1+x2*x2+x3*x3+x4*x4;
	    b[1]=f1*x1+f2*x2+f3*x3+f4*x4;
	

	    integer nodes=2;
	    eqsolve_simple_gauss(AM, nodes, b, a); // ������� ���� ������� ������.

	    // ������������ ������
		if (AM != NULL) {
			for (integer i = 0; i < 2; i++) {
				if (AM[i] != NULL) {
					delete[] AM[i];
				}
			}
			delete[] AM;
		}
		if (b != NULL) {
			delete[] b;
		}

	    //f(x)=a[0]+a[1]*x;

	    //������ ����������� � ����� xg ����� a[1];
	    DFDXgran=a[1];

		if (a != NULL) {
			delete[] a;
		}

	}

	if (imetod==cubic_parabola) {
		// �� ������ ������ ������ ���������� �������� � ������������ ������� � ����������� � ������ ����� xg.

		doublereal* a = NULL;
		a=new doublereal[4];
		doublereal** AM = NULL;
		AM=new doublereal*[4];
	    for (integer i=0; i<4; i++) {
		    AM[i]=new doublereal[4]; // ������� ����
	    }
		doublereal* b = NULL;
		b=new doublereal[4]; // ������ �����

	    for (integer i=0; i<4; i++) {
		    for (integer j=0; j<4; j++) {
			    AM[i][j]=0.0; // �������������
		    }
		    b[i]=0.0;
		    a[i]=0.0;
	    }

	     AM[0][0]=1.0;
	     AM[0][1]=x1;
	     AM[0][2]=x1*x1;
	     AM[0][3]=x1*x1*x1;
	     b[0]=f1;
	     AM[1][0]=1.0;
	     AM[1][1]=x2;
	     AM[1][2]=x2*x2;
		 AM[1][3]=x2*x2*x2;
	     b[1]=f2;
		 AM[2][0]=1.0;
	     AM[2][1]=x3;
	     AM[2][2]=x3*x3;
		 AM[2][3]=x3*x3*x3;
	     b[2]=f3;
		 AM[3][0]=1.0;
	     AM[3][1]=x4;
	     AM[3][2]=x4*x4;
		 AM[3][3]=x4*x4*x4;
	     b[3]=f4;
	

	     integer nodes=4;
	     eqsolve_simple_gauss(AM, nodes, b, a); // ������� ���� ������� ������.

	     // ������������ ������
		 if (AM != NULL) {
			 for (integer i = 0; i < 3; i++) {
				 if (AM[i] != NULL) {
					 delete AM[i];
				 }
			 }
			 delete[] AM;
		 }
		 if (b != NULL) {
			 delete[] b;
		 }

	     //f(x)=a[0]+a[1]*x+a[2]*x^2+a[3]*x^3;

	     //������ ����������� � ����� xg ����� 2.0*a[2]*xg+a[1];
	     DFDXgran=3.0*a[3]*xg*xg+2.0*a[2]*xg+a[1];

		 if (a != NULL) {
			 delete[] a;
		 }

	}
	

	return DFDXgran; // ����������� � �������� �� ����� ��.

} // DFDXg

// ����������� �� ������� �������� � �������� �� ����� ��.
// ������� ����� xg. �� ��� ������ ��������� � �������.
doublereal DFDXg2(doublereal x1, doublereal x2, doublereal x3, doublereal xg,
	      doublereal f1, doublereal f2, doublereal f3) 
{
	doublereal h1, h2;
	h1=x2-x1; h2=x3-x2;
	doublereal D1, D2, D;
	D=h1*h2*(h1+h2);
	D1=(f3-f2)*h1+(f1-f2)*h2;
	D2=(f3-f2)*h1*h1-(f1-f2)*h2*h2;

	doublereal alpha, beta;
	alpha=D1/D;
	beta=D2/D;

	return 2.0*alpha*xg+beta;

} // DFDXg2

// � ����� �.�. ������� �� ��� 208-210 ���������� �������
// ��� ���������� ������ ����������� �� ������������� ����� � ��������� O(h^2).
// ��-�������� ���� ������ ����� ������ ��� ������ ������������ ������������� � 
// ��� ����� ����� ������������ ��� ��������� �������� �� ���������� ����������� �������.
// ������������� ������ ������� � ���������� �������� �� ��� ������ � ����� ���������� � 
// ������ �����������.
doublereal rgradF(doublereal Fback, doublereal Fcenter, doublereal Fforvard, doublereal hback, doublereal hforvard) {
	doublereal r=0.0;

	// ������������� ��� �������� ��������� �������:
	//  Fback Fcenter Fforvard
	// -hback    0    hforvard

	// �������� ������ ������� O(h^2)
	r=((Fforvard-Fcenter)*hback*hback-(Fback-Fcenter)*hforvard*hforvard)/(hback*hforvard*(hback+hforvard));

	return r;
} // rgradF

// �������� ������������ �� ���� �����
doublereal my_linear_interpolation(char chdirect, doublereal PP, doublereal PbG, doublereal posP,
	                         doublereal posbG, doublereal posvirtual) {
	doublereal a=0.0,b=0.0;
	doublereal r=0.0;

	switch (chdirect) {
    	case '+' : a=(PP-PbG)/(posP-posbG);
	    	       b=(PbG*posP-PP*posbG)/(posP-posbG);
		           break;
	    case '-' : a=(PbG-PP)/(posbG-posP);
		    	   b=(PP*posbG-PbG*posP)/(posbG-posP);
		           break;
	}

	r=a*posvirtual+b; // linear interpolation
	return r;

} // my_linear_interpolation

doublereal my_quadratic_interpolation(char chdirect, doublereal PbGG, doublereal PbG, doublereal PP,
	                            doublereal posbGG, doublereal posbG, doublereal posP, doublereal posvirtual) {

    // ��� ������������� �������� �� ��������� ��������� �������
    doublereal **B3x3=NULL;
	B3x3=new doublereal*[3];
	integer l=0; // ������� ����� for
	for (l=0; l<3; l++) B3x3[l]=new doublereal[3];

	doublereal posbGG2=0.0, posbG2=0.0, posP2=0.0; // �������� ��������� �������
	posbGG2=posbGG*posbGG; posbG2=posbG*posbG; posP2=posP*posP;

	doublereal r=0.0; // ����� ����� ������������� ������������ ��������

	switch (chdirect) {
	case '+' : B3x3[0][0]=1.0; B3x3[0][1]=posbGG; B3x3[0][2]=posbGG2;
	           B3x3[1][0]=1.0; B3x3[1][1]=posbG; B3x3[1][2]=posbG2;
	           B3x3[2][0]=1.0; B3x3[2][1]=posP; B3x3[2][2]=posP2;

	           inverse_matrix_simple(B3x3, 3, false); // �������� �������

			   r=1.0*(B3x3[0][0]*PbGG+B3x3[0][1]*PbG+B3x3[0][2]*PP);
	           r+=posvirtual*(B3x3[1][0]*PbGG+B3x3[1][1]*PbG+B3x3[1][2]*PP);
	           r+=posvirtual*posvirtual*(B3x3[2][0]*PbGG+B3x3[2][1]*PbG+B3x3[2][2]*PP);
		       break;
	case '-' : B3x3[0][0]=1.0; B3x3[0][1]=posP; B3x3[0][2]=posP2;
	           B3x3[1][0]=1.0; B3x3[1][1]=posbG; B3x3[1][2]=posbG2;
	           B3x3[2][0]=1.0; B3x3[2][1]=posbGG; B3x3[2][2]=posbGG2;

	           inverse_matrix_simple(B3x3, 3, false); // �������� �������

               r=1.0*(B3x3[0][0]*PP+B3x3[0][1]*PbG+B3x3[0][2]*PbGG);
	           r+=posvirtual*(B3x3[1][0]*PP+B3x3[1][1]*PbG+B3x3[1][2]*PbGG);
	           r+=posvirtual*posvirtual*(B3x3[2][0]*PP+B3x3[2][1]*PbG+B3x3[2][2]*PbGG);
		       break;
	}
		
	if (B3x3 != NULL) {
		for (l = 0; l < 3; l++) {
			if (B3x3[l] != NULL) {
				delete[] B3x3[l];
				B3x3[l] = NULL;
			}
		}
		delete[] B3x3;
		B3x3 = NULL;
	}

	return r;

} // my_quadratic_interpolation

#endif