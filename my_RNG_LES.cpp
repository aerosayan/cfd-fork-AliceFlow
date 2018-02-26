// ���� my_RNG_LES.cpp �������� �� ����������� ��� 
// ���������� ������ �������������� RNG LES.
// begin 18 ������ 2012 ����.

#ifndef MY_RNG_LES_CPP
#define MY_RNG_LES_CPP 1

//#include <math.h>

//#define doublereal double

/*
// ������� �� ���� ������������ �����.
doublereal fmin(doublereal ra, doublereal rb) {
	if (ra<rb) return ra;
	else return rb;
}

// �������� �� ���� ������������ �����.
doublereal fmax(doublereal ra, doublereal rb) {
	if (ra<rb) return rb;
	else return ra;
}
*/

// ������� ���������
doublereal rHeviaside(doublereal x) {
	doublereal r=0.0;
	if (x>0.0) {
		r=x;
	}
	return r;
} 
 
// ������� ��� ������� ����� ���������� � ����������� � ����.
doublereal my_RNG_LES_function(doublereal mu_lam, doublereal mu_sgs, doublereal mu_eff) {
	// ���������� ������������ �������� mu_lam,
	// ������������ �������� ���������� �� ������ ������ ������������� mu_sgs,
	// ������� �������� ����������� �������� ���������� �� ������ ������ RNG_LES.

	const doublereal C_RNG_LES=100.0;
	doublereal r;
	r=mu_lam*exp((1.0/3.0)*log(1.0+rHeviaside(((mu_sgs*mu_eff)/(mu_lam*mu_lam*mu_lam))-C_RNG_LES)))-mu_eff;

	return r;
}

// ����� ����� ������� ������� ����������� �������.
doublereal my_dixtomiq_RNG_LES(doublereal mu_lam, doublereal mu_sgs) {

	doublereal eps0=1e-10; // �������� ����������� ����������� ��������

	//doublereal ra=1e-7*fmin(mu_lam,mu_sgs);
	doublereal ra=mu_lam; // ����������� �������� �� ����� ���� ������ ����������.
	doublereal rb=1e+7*fmax(mu_sgs,mu_lam);

	doublereal r=0.5*(ra+rb);
	doublereal fa, fb, fc;

	while (fabs(rb-ra)>eps0) {
		fa=my_RNG_LES_function(mu_lam, mu_sgs, ra);
		fb=my_RNG_LES_function(mu_lam, mu_sgs, rb);
		fc=my_RNG_LES_function(mu_lam, mu_sgs, r);
		if (fa*fc<0.0) {
			rb=r;
            r=0.5*(ra+rb);
		}
		else if (fb*fc<0.0) {
			ra=r;
			r=0.5*(ra+rb);
		} else {
			//printf("error RNG LES calculation\n");
			//printf("press any key to continue...\n");
			//getchar();
			//exit(0);
			// ��������� ���� �������� ��� mu_sgs
			// ����� ������ ��� 100.0*mu_lam^2
			// � ������ ����������� �������� ������ ����� ������������ ��������.

			ra=rb=mu_lam;
			break; // ����� �� ����� while
		}
	}
	r=0.5*(ra+rb);
	return r;
}

#endif
