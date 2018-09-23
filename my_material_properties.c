// 24 ������ 2011 ���� � ������
// 25 ������ 2011 ���� 15 ������ ���.
// 26-27 ������ 2011 ���� ��������� ���������� AliceMeshv0_08.
// 28 ������ 2011 ���� ��������� update_temp_properties ������
// ��� ���������� ����������� �������.
// � ������ my_material_properties.c ����������
// ����������������� ���������� ���������� ������� ����������.

#ifndef  MY_MATERIAL_PROPERTIES_C
#define  MY_MATERIAL_PROPERTIES_C 1

// ���������� ������ ������������ ���������� ���������� � ����� ������ ��������� � ������ ������ 
// AliceFlow_v0_27
//#define doublereal double
#include <math.h>

// ������������ ����� �� ��������� � ������ ���������� 
// ����������� ����������� ����������� � ���������� ������
// �� ������ 110��-�� ���������� TGF2023-20: TEMPERATURE_FAILURE_DC.
void diagnostic_critical_temperature(doublereal TiP) {
	 if (TiP > TEMPERATURE_FAILURE_DC) {
		   printf("Attantion! unit burned...\n");
		   printf("temperature exceeds a critical TEMPERATURE_FAILURE_DC==%3.2f",TEMPERATURE_FAILURE_DC);
		 //  getchar();
		   system("pause");
		   char ch;
		   printf("please enter c)ontinue h)alt.\n");
		   ch=getchar();
		   if ((ch=='h')||(ch=='H')) exit(0);
	   }
} // diagnostic_critical_temperature

// �������� 0 ��� ��������� ��������� �������������.
#define MY_AIR 1
#define MY_WATER 2

// � �������� � �������� ������ ������� ������� ������������ ���� ��� ������.
// �� � ������ ����� (����� � ����� �����) ���-���� ������.
// ������� ����� ��������� ����������� ������������ � ���������� �������� ���� � �������.

// ������ ������������ �� ��������� �� ���� ��������,
// ���� ������������ �� ������� �����.
void my_air_properties(doublereal TiP, doublereal PiP, 
	                   doublereal &rho, doublereal &cp,
					   doublereal &lam, doublereal &mu,
					   doublereal &beta) {
	// ���������� �������� �������� ������� !
	// ������� ���������: 
    // TiP - ����������� � �������� ������� � ������ ������������ ������.
	// PiP - �������� � ���������� (������������ �� ����, 0 - ������� ��������). 

	// ������ ����������� ������������� ����������� � ���������
	// ���������� 273-473K.
	// ������: HighExpert.Ru ���������� �������� �������
	// www.engineeringtoolbox.com.

    doublereal TK=TiP+273.15; // K
	doublereal Pair=101325+PiP; // ��

	// �������� ����� � �������:
	/* temp grad C  Vel m/s  Vel km/h
	*  -150			216.7	  780.1
	*  -100			263.7	  949.2
	*  -50			299.3	  1077.6
	*  -20			318.8	  1147.8
	*  -10			325.1	  1170.3
	*  0			331.5	  1193.4
	*  10			337.3	  1214.1
	*  20			343.1	  1235.2
	*  30			348.9	  1256.2
	*  50			360.3	  1296.9
	*  100			387.1     1393.7
	*  200			436.0     1569.5
	*  300			479.8     1727.4
	*  400			520.0     1872.1
	*  500			557.3     2006.4
	*  1000			715.2     2574.8
	*/
	// ��������� ����������� ��� ������ ���� < 0.3.
	// ��������� �������� 102 �/�.
	//rho=Pair/(287.4*TK); // kg/m^3
	// 27. 07. 2016.
	// � ����������� ���������� ������� �������� ��������� ���������,
	// ����������� ��������� �� ����������� ����������� ����� ����������� ��������� �������������� ����������.
	rho = 1.1614;
	cp=1000.0*(1.0005+1.1904e-4*(TiP)); // J/(kg*GC)
	lam=2.44e-2*exp(0.82*log(TK/273.15)); // W/(m*K)
	mu=1.717e-5*exp(0.683*log(TK/273.15)); // Pa*s
	//printf("mu=%e, TK=%e\n",mu,TK); // debug Ok
	//getchar();
	// � maple �� ������� ���������� ��������� �������� ��������� �����������:
    // ���������� 27.07.2016
	beta = 0.001*(-0.06810259 + 0.00013411*TiP - 8.287412214e-8*TiP*TiP + 1022.18 / (273.15 + TiP));

	// ���������� ���������� ��������
	//rho=1.1614; // ������������� !!!
	//mu=1.84e-5; 
	//beta=0.003331;
	//cp=1005;
	//lam=0.025;
	
} // my_air_properties

// ���� - ��� ������ ����� ������� ������ ������� ����,
// ��� ��������. ��� �������� ����� �����.
void my_water_properties(doublereal TiP,  
	                   doublereal &rho, doublereal &cp,
					   doublereal &lam, doublereal &mu,
					   doublereal &beta) {
	// ���������� �������� �������� ���� !
	// ������� ���������: 
    // TiP - ����������� � �������� ������� � ������ ������������ ������.

	// ������ ����������� ������������� ����������� � ���������
	// ���������� 283-373K.
	// ������: HighExpert.Ru ���������� �������� ����
	// www.engineeringtoolbox.com.

    doublereal TK=TiP+273.15;

	// �������� ����� � ���� :
	/* temp grad C  Vel Mag m/s
	*		 0		1.403
	*		 5		1.427
	*		10		1.447
	*		20		1.481
	*		30		1.507
	*		40		1.526
	*		50		1.541
	*		60		1.552
	*		70		1.555
	*		80		1.555
	*		90		1.550
	*	   100		1.543
	*/
	// ��������� ����������� ��� ������ ���� < 0.3.

	//rho=995.7/(0.984+0.483e-3*TiP); // kg/m^3
	// 27. 07. 2016.
	// � ����������� ���������� ������� �������� ��������� ���������,
	// ����������� ��������� �� ����������� ����������� ����� ����������� ��������� �������������� ����������.
	rho = 995.7 / (0.984 + 0.483e-3*(20.0)); // kg/m!3
	cp=(4194-1.15*TiP+1.5e-2*TiP*TiP); // J/(kg*GC)
	lam=0.553*(1.0+0.003*TiP); // W/(m*K)
	mu=(1.78e-6/(1.0+0.0337*TiP+0.000221*TiP*TiP))*rho; // Pa*s
	// � maple �� ������� ���������� ��������� �������� ��������� �����������:
	beta=0.02289718770+0.142992772827544502e-6*TiP*TiP-0.67623235e-4*TiP-(6.27314791869)/(TK);

	// ���������� ���������� �������� ��� 20 ���� �.
	//rho=998.2; // ������������� !!!
	//mu=1e-3;
	//cp=4177;
	//lam=0.58618;
	//beta=0.192166e-3;
} // my_water_properties

// SOLID ������ ����:
// ������� ������� : ��������� ������ ����������� ������.

// �������� 100 ��� ��������� ��������� �������������.
//#define MY_AUSN 101    // ������ ������ 80% ������ 20%
#define MY_ALUMINA 101   // ������� Alumina, Al2O3 -ref. MatWeb[2] 99.9% Al2O3, polycrystalline aluminum oxide.
#define MY_SI 102        // �������
#define MY_GAAS 103      // ������� �����
#define MY_GAN 104       // ������ �����
#define MY_SIC4H 105     // ������ ������� SiC4H
#define MY_SAPPHIRE 106  // ������
#define MY_DIAMOND 107   // �����
#define MY_MD40 108      // ����������� ��40
#define MY_AU 109        // ������
#define MY_SIO2 110      // ����� �������
#define MY_COPPER 111    // ����
#define MY_KOVAR 112     // �����
#define MY_BRASS 113     // ������ ��-59-1-�
#define MY_DURALUMIN 114 // ����������� �16
#define MY_ALUMINIUM_NITRIDE 115 // ������ ��������� (���������������).
#define MY_GLUE_ECHES 116 // ���� ����

// ������ ������ ������ (�����) 
// 21 ����� 2016 ���� ������� ������������� ����������� �� ������
// ��������� ������� ����������� ����������������� ����������.
void my_ausn_properties(doublereal TiP,
	                    doublereal &rho, doublereal &cp,
						doublereal &lam) {
       // AuSn 80 ������/20 �����
       doublereal TK=TiP+273.15;
	   rho=14510; // kg/m^3
	   // cp=150 �� ������ ��������� SYMMIC
	   cp=143; // J/(kg*K) �� ������ TDIM - Master
	  
	   // ���������������� � ���������� �������� ������ 
	   // ��� ���� ����� : 68 ��� 300� � 57 ��� 85 ����. �.
	   lam=57; // �.�. ����� �� ����� ����������� �������� ���������� �������.
	   // ������� �� ������ ���������� ������� ����� ��-��������� � �������� ��� 
	   // ��������������� �.�. ��� �� ������������. ���� ������� � ������ ����������������
	   // ������� �������� �� ������ �������� � ��������� (���������������) ���� ���� �������� � 
	   // ������� ���������������� ����� � ������ �����������.
	   //lam=68*exp(-0.99678*log(TK/300)); // 57 W/(m*K) �������� ������, ����� ����

	   // ���������������� � ����������� ������-���������� ������ ������ 
	   // ��������� �� ������ ������� ��� ����������� � ����������� ���� 
	   // ���������� ������ ������, �.�. ��������� ���������� ������ �������������
	   // ������� ������� ��������� ���������� ����� � ������,  ������� ���������������� �
	   // ����������� ������� ���������� ���� ��� ������ ��  ��������� ������ � �����������.
	   // ������ �������� �������� ������������������ �������, ������������ ���������� ������� ����������.

} // AuSn

// ������� Si Silicon 
void my_si_properties(doublereal TiP,
	                    doublereal &rho, doublereal &cp,
						doublereal &lam) {
       // Silicon
       doublereal TK=TiP+273.15;
	   rho=2330; // kg/m^3
	   //cp=711; // J/(kg*K)
	   // ������� ��� ����������� ����������� � ��������� ���������� : 77K-773K.
	   // TK  77 173 273 373 573 773
	   // cp 180 490 680 770 850 880
	   if (TK < 800.0) {
		   cp=83.23167276-11801.68987/TK+0.3123808545e-5*TK*TK*TK+3.672005743*TK-0.5805489801e-2*TK*TK;
	   }
	   else {
		   // ������������ ������ �� �� ������� ��� ������������� �� ������� ���������� ��������� �� ��������.
		   cp=880;
	   }
	   // ������� ��������� ���������������� �� ����������� ����� ��
	   // ����� �. ���� ����������� ������� �����.
	   //lam=120; // ������ �.�. �������������
	   lam=148*exp(-1.35*log(TK/300)); // 120 W/(m*K)
} // Si

// 18 october 2016.
// ������� ����������� ��� �������� ��������� �� ����������� ��������.
// Alumina, Al2O3-ref. Mat Web[2] 99.9% Al2O3, polycrystalline aluminum oxide.
void my_alumina_properties(doublereal TiP,
	doublereal &rho, doublereal &cp,
	doublereal &lam) {

	doublereal TK = TiP + 273.15;
	rho = 2330; // kg/m^3

	//cp=753; // J/(kg*K)
	// ������� ��� ����������� ����������� � ��������� ���������� : 200K-800K.
	// TK  200 298 400 500 600 800
	// cp 502 753 920 1046 1088 1172
	if (TK < 800.0) {
		cp = 0.00000505903*TK*TK*TK - 0.01042874335*TK*TK + 7.612830818*TK + (50982.822 / TK) - 898.2525;
	}
	else {
		// ������������ ������ �� �� ������� ��� ������������� �� ������� ���������� ��������� �� ��������.
		cp = 1172;
	}

	// ���������������� �� ������ ��������� SYMMIC.
	// TK lam
	// 200 82
	// 298 46
	// 400 32.3
	// 500 24.2
	// 600 18.9
	// 800 13.0
	lam = 47.84832382*exp(-1.328556144*log(TK / 300.0)); // 46

} // alumina

// ������� ����� GaAs 
void my_gaas_properties(doublereal TiP,
	                    doublereal &rho, doublereal &cp,
						doublereal &lam) {
       // GaAs
       doublereal TK=TiP+273.15;
	   rho=5300; // kg/m^3
	   cp=322; // J/(kg*K)
	   if ((TK > 173.0) && (TK < 1514.0))
	   {
		   cp = 4.059e-8*TK*TK*TK - 1.296e-4*TK*TK + 1.854e-1*TK + 279.0;
	   }
	   else if (TK <= 173.0) cp = 307.4;
	   else cp = 402.9;
	   // ������� ��������� ���������������� �� ����������� ����� ��
	   // ����� �. ���� ����������� ������� �����.
	   //lam=47; // ������ �.�. �������������
	   lam=54*exp(-1.25*log(TK/300)); // 54 W/(m*K)

	   // �������� �� ������������ ����������.
	   diagnostic_critical_temperature(TiP);
} // GaAs

// ������ ����� GaN
void my_gan_properties(doublereal TiP,
	                    doublereal &rho, doublereal &cp,
						doublereal &lam) {
       // GaN
       doublereal TK=TiP+273.15;
	   rho=6150; // kg/m^3
	   cp=491; // J/(kg*K)
	   // ������� ��� ����������� ����������� � ��������� ���������� : 200K-1300K.
	   // TK  200 300 400 500 600 700 800 900 1000 1100 1200 1300
	   // cp 322.3 431.3 501.2 543.8 572.17 592.8 608.9 622.2 633.7 643.99 653.4 662.22
	   if ((TK > 200) && (TK < 1300)) {
		   cp = 591.807 + 0.064971*TK - 2.6e7 / (TK*TK) + 2.94e9 / (TK*TK*TK);
	   }
	   // ������� ��������� ���������������� �� ����������� ����� ��
	   // ����� �. ���� ����������� ������� �����.
	   //lam=130; // ������ �.�. �������������
	   lam=130*exp(-0.43*log(TK/300)); // 130 W/(m*K)

	   // �������� �� ������������ ����������.
	   diagnostic_critical_temperature(TiP);
} // GaN

// ������ ������� SiC4H
void my_sic4h_properties(doublereal TiP,
	                    doublereal &rho, doublereal &cp,
						doublereal &lam) {
       // SiC4H
       doublereal TK=TiP+273.15;
	   rho=3210; // kg/m^3
	   //cp=690; // J/(kg*K)
	   // ����������� ����������� ������� �� �����������
	   // ��������� �� ��������� ������� ������:
	   // TK 77 300 373 573 773
	   // Cp 50 690 820 1010 1120
	   if ((TK>=77.0)&&(TK<=773.2)) {
		   cp=-600.9595790625+6.41266663224237*TK+16052.9931969468/TK-0.00900588312450910*TK*TK+0.459922567674634e-5*TK*TK*TK;
	   } else if (TK<77.0) cp=50;
	   else cp=1120;

	   // ������� ��������� ���������������� �� ����������� 
	   // � ����� �. ���� ����������� ������� �����. ���!!!
	   //lam=370; // V-�����. SiC
	   lam=370*exp(-1.411*log(TK/300)); // 370 W/(m*K)
} // SiC4H

// ������ Sapphire
void my_sapphire_properties(doublereal TiP,
	                    doublereal &rho, doublereal &cp,
						doublereal &lam) {
       // Sapphire Al2O3
       doublereal TK=TiP+273.15;
	   rho=3980; // kg/m^3
	   //cp=750; // J/(kg*K)
	   // ������� ��� ����������� ����������� � ��������� ���������� 77K - 773K
	   // TK 77 173 273 373 573 773
	   // Cp 60 403 718 907 1089 1168
	   if (TK<774) {
		   cp=-703.3920492+19101.78790/TK+0.5623375490e-5*TK*TK*TK+7.499240665*TK-0.01095728614*TK*TK;
	   }
	   else cp=1168;

	   // ������� ��������� ���������������� �� ����������� 
	   // �  ����� �. ���� ����������� ������� �����. ���!!!
	  // lam=23; // ������ �.�. �������������
	   // �� ������ ��������� SYMMIC.
	   lam=42*exp((-1.134821)*log(TK/298)); // 42 W/(m*K) ��� 298�

	   // �������� �� ������������ ����������.
	   diagnostic_critical_temperature(TiP);
} // Sapphire

// ����� Diamond
void my_diamond_properties(doublereal TiP,
	                    doublereal &rho, doublereal &cp,
						doublereal &lam) {
       // Diamond
       doublereal TK=TiP+273.15;
	   rho=3500; // kg/m^3
	   //cp=520; // ��� 300� J/(kg*K)
	   // ������ �� ����������� ����������� � ��������� ����������:
	   // 77K - 773K
	   // TK 77  173 273 373 573 773
	   // Cp 8  140  420 770 1300 1590 � ���������� �����, �� ������ ���.
	   if (TK < 774) {
		   cp=-696.4289+32660.38060/TK-0.2774926361e-5*TK*TK*TK+3.566362474*TK+0.1285766705e-2*TK*TK;
	   }
	   else cp=1590;
	   // cp=1260; // �� ������ �.�. �������������
	   // ������� ��������� ���������������� �� ����������� ����� ��
	   // ����� �. ���� ����������� ������� �����.
	   //lam=800; // ������ �.�. �������������
	   lam=2300*exp(-1.85*log(TK/300)); // 130 W/(m*K)
	   // �� ������ �. ���� ���������������� ������ ��� 300� ����� ����� 2000 � 2500.
} // Diamond

// ����������� CuMo � 40% ����������� ���� : ��40
void my_md40_properties(doublereal TiP,
	                    doublereal &rho, doublereal &cp,
						doublereal &lam) {
       // �� ��������� ����� ������������ ������ ������ �������:
	   // ���������, ������� ������ � ������. ������ �������������.
       // MD40
       doublereal TK=TiP+273.15;
	   rho=9660; // kg/m^3
	   // cp=540 �� ������ ��������� ��������, �����������.
	   // ������ � ��������� �������� � ����������� ����� ������� ����������� ���������.
	   cp=318.2; // J/(kg*K)
	   // heat capacity cp
	   // TK 173 273 373 573 
	   // cp J/(kg-K) 268.8 302.5 318.17 335.1
	   if ((TK > 173) && (TK < 573))
	   {
		   cp = 1.66e-6*TK*TK*TK - 2.263e-3*TK*TK + 1.095*TK + 138.5;
	   }
	   else if (TK <= 173) cp = 268.77;
	   else cp = 335.111;
	   lam=210; // W/(m*K)
	   if ((TK >= 273) && (TK < 1600)) {
		   lam = 7.15e-9*TK*TK*TK - 2.18e-6*TK*TK - 6.03e-2*TK + 271.0;
	   }
	   else if (TK < 273) lam = 254.52;
	   else lam = 198.0;
} // MD40

// ������ Au
void my_au_properties(doublereal TiP,
	                  doublereal &rho, doublereal &cp,
					  doublereal &lam) {
       // Au
       doublereal TK=TiP+273.15;
	   rho=19300; // kg/m^3
	   //cp=126; // J/(kg*K)

	   // �������������� ����������� �������� �� ��������� ������
	   // � ������� ���:
	   // TK 77 173 273 373 573 773
	   // Cp 97 121 128 131 135 140
	   if ((TK>77.0)&&(TK<773.0)) {
		   cp=140.2979168-3345.749125/TK+0.3399388027e-7*TK*TK*TK+0.3605589168e-2*TK-0.2418961279e-4*TK*TK;
	   }
	   else if (TK<=77.0) cp=97;
	   else cp=140;

	   //lam=293; // W/(m*K)
	   // ��� �����������, ������������� � ����� �. ���� �� ������� ���
	   // ���������������� ������.
	   // ������� ���� �������� ���� ������������ ������� � ������� ���.
	   if (TK<974) {
		   lam=327.5268055+146.5916497/TK+0.5614880962e-7*TK*TK*TK-0.008865308782*TK-0.1043207287e-3*TK*TK;
	   }
	   else lam=272;

	   // �������� �� ������������ ����������.
	   diagnostic_critical_temperature(TiP);
} // Au

// SiO2
// ���������� ���������� �� ������� ����.
// ������������� ������ ������� ������������� ���������� SYMMIC.
void my_sio2_properties(doublereal TiP,
	                  doublereal &rho, doublereal &cp,
					  doublereal &lam) {
       // SiO2
       doublereal TK=TiP+273.15;
	   rho=2100; // kg/m^3
	   cp=741; // J/(kg*K)
	   if ((TK >= 273.0) && (TK <= 773))
	   {
		   cp = -1.6667e-7*TK*TK*TK - 9.635e-4*TK*TK + 1.975*TK + 236.02;
	   }
	   else if (TK < 273.0) cp = 700.0;
	   else cp = 1110.0;

	   //lam=7; // 7-11.5 W/(m*K) quartz crystal
	   lam = 1.27; // ���������� ���������� �� ������� ����.

} // SiO2

// ���� Copper
void my_copper_properties(doublereal TiP,
	                  doublereal &rho, doublereal &cp,
					  doublereal &lam) {
       // Copper
       doublereal TK=TiP+273.15;
	   rho=8930; // kg/m^3
	   //cp=390; // J/(kg*K)
	   // �������������� ����������� ����������� �� �����������
	   // �������� �� ��������� ������ � ������� ���.
	   // TK    77  173 273 373 573 773
	   // Cp    195 341 379 397 419 430 
	   if ((TK>77.0)&&(TK<773.0)) {
		   cp=506.2148755-22501.73085/TK-0.3085157062e-6*TK*TK*TK-0.2854473337*TK+0.5289207596e-3*TK*TK;
	   }
	   else if (TK<77.0) cp=195;
	   else cp=430;

	   //lam=390; // W/(m*K)
	   // �������������� ����������� ���������������� �� �����������
	   // �������� �� ��������� ������ � ������� ���.
	   // TK      173.2 273.2 373.2 573.2 973.2
	   // Lambda  420.0 403.0 395.0 381   354
	   if ((TK>173.2) && (TK<973.2)) {
		   lam=318.4875333+12313.69842/TK+0.1748333335e-6*TK*TK*TK+0.2380247005*TK-0.3905912003e-3*TK*TK;
	   }
	   else if (TK<=173.2) lam=420.0;
	   else lam=354.0;
} // Copper

// ����� Kovar
void my_kovar_properties(doublereal TiP,
	                  doublereal &rho, doublereal &cp,
					  doublereal &lam) {
       // Kovar
	   // ����� ��� ����� ������ �������: C, Fe 54%, Mn, Ni 29%
       doublereal TK=TiP+273.15;
	   rho=8300; // kg/m^3
	   cp=669; // J/(kg*K)
	   // heat capacity kovar
	   // TK 273 703
	   // cp 439.614 648.954
	   if ((TK>273) && (TK < 703))
	   {
		   cp = 0.486*TK + 306.6;
	   }
	   else if (TK < 273) cp = 439.6;
	   else cp = 648.954;
	   //lam=19; // W/(m*K) 
	   // �������������� ����������� ���������������� �� �����������
	   // �������� �� ��������� ������ � ������� ���.
	   // TK      273  373  573  773  973
	   // Lambda  14.1 14.7 15.6 17.5 19.3
	   if ((TK>273.0) && (TK<973.0)) {
		   lam=44.13357753-3650.246187/TK-0.4572641192e-7*TK*TK*TK-0.8854732756e-1*TK+0.1132267331e-3*TK*TK;
	   } else if (TK<273.0) lam=14.1;
	   else lam=19.3;
} // Kovar

// ������ ��-59-1-� Brass
void my_brass_properties(doublereal TiP,
	                  doublereal &rho, doublereal &cp,
					  doublereal &lam) {
       // Brass
	   // ������ Cu 70%, Zn 30%
       doublereal TK=TiP+273.15;
	   rho=8440; // kg/m^3
	   cp=377; // J/(kg*K)
	   //lam=108.784; //  W/(m*K)
	   // ������ � ������������� ����������� ���������������� ������:
	   // T GC   -100 0 100 200 300 400
	   // TK      173.2 273.2 373.2 473.2 573.2 673.2
	   // Lambda  90 106 131 143 145 148
	   if ((TK>=173.2)&&(TK<=673.2)) {
		   lam=-591.598556543738+2.90903837477216*TK+52133.0458047673/TK-0.00454066869215317*TK*TK+0.249629141483149e-5*TK*TK*TK;
	   } else if (TK<173.2) lam=90.0;
	   else lam=148.0;

} // Brass

// ����������� Duralumin �16
void my_duralumin_properties(doublereal TiP,
	                  doublereal &rho, doublereal &cp,
					  doublereal &lam) {
       // Duralumin
       doublereal TK=TiP+273.15;
	   rho=2780; // kg/m^3
	   cp=922; // J/(kg*K)
	   // heat capacity duralumin
	   // TK  273 373 473 573 673
	   // cp J/(kg-K) 750.13 921 1047 1130 1172
	   if ((TK>273.0) && (TK < 673.0)) {
		   cp = 3.33e-7*TK*TK*TK - 2.62e-3*TK*TK + 3.3*TK + 38.0;
	   }
	   else if (TK <= 273.0) cp = 750.13;
	   else cp = 1172.0;
	   lam=130; // W/(m*K)
	   if ((TK>150.0) && (TK < 573)) {
		   lam = 0.171*TK + 66.1;
	   }
	   else if (TK <= 150.0) lam = 90.0;
	   else lam = 164.0;
} // Duralumin

// ���� ����
void my_glueeches_properties(doublereal TiP,
	                  doublereal &rho, doublereal &cp,
					  doublereal &lam) {
       // glue ECHES
	   // �� ������ �.�. �������������.
       doublereal TK=TiP+273.15;
	   rho=1000; // kg/m^3
	   cp=100; // J/(kg*K)
	   lam=4; // W/(m*K)
} // glue ECHES

// ������ �������� (���������������)
void my_aluminium_nitride_properties(doublereal TiP,
	doublereal &rho, doublereal &cp,
	doublereal &lam) {
	// aluminium nitride
	// �� ������ ��������� SYMMIC.
	doublereal TK = TiP + 273.15;
	rho = 3255; // kg/m^3
	cp = 600; // J/(kg*K) // SYMMIC
	//cp=748; // J/(kg*K) // �. ����.
	//lam = 319; // W/(m*K)
	// lam=285; // W/(m*K) �. ����

	// TSIDE,K lam W/(mxK)
	// 200 780
	// 300 319
	// 400 195
	// 600 100
	// 1000 49

	// ������� ��������� ���������������� �� ����������� 
	// �  ����� �. ���� ����������� ������� �����. ������������, ��� ��� ���������������� ��� � ��� ��������.
	// lam=319; // ������ ��������� SYMMIC
	// �� ������ ��������� SYMMIC.
	lam = 319 * exp((-1.57)*log(TK / 300)); // 319 W/(m*K) ��� 300�
} // aluminium nitride


// ����������������� ���������� ������ ����������.
void my_fluid_properties(doublereal TiP, doublereal PiP, 
	                   doublereal &rho, doublereal &cp,
					   doublereal &lam, doublereal &mu,
					   doublereal &beta, integer ilibident) {
	switch (ilibident) {
	  case MY_AIR : my_air_properties(TiP, PiP, rho, cp, lam, mu, beta); break;
	  case MY_WATER : my_water_properties(TiP, rho, cp,lam, mu, beta); break;
	  default : my_air_properties(TiP, PiP, rho, cp, lam, mu, beta);  break;
	} // end switch
} // my_fluid_properties

 // ����������������� ���������� ������ ����������.
void my_solid_properties(doublereal TiP, doublereal &rho, doublereal &cp,
	                     doublereal &lam, integer ilibident) {
	switch (ilibident) {
	  //case MY_AUSN : my_ausn_properties(TiP,rho,cp,lam); break; // ������ ������ ������
	  case MY_ALUMINA:  my_alumina_properties(TiP, rho, cp, lam); break; // ������� Al2O3.
	  case MY_SI : my_si_properties(TiP,rho,cp,lam); break; // �������
	  case MY_GAAS : my_gaas_properties(TiP,rho,cp,lam); break; // ������� �����
	  case MY_GAN : my_gan_properties(TiP,rho,cp,lam); break; // ������ �����
	  case MY_SIC4H : my_sic4h_properties(TiP,rho,cp,lam); break; // ������ �������
	  case MY_SAPPHIRE : my_sapphire_properties(TiP,rho,cp,lam); break; // ������
	  case MY_DIAMOND : my_diamond_properties(TiP,rho,cp,lam); break; // �����
	  case MY_MD40 : my_md40_properties(TiP,rho,cp,lam); break; // ����������� ��40
	  case MY_AU : my_au_properties(TiP,rho,cp,lam); break; // ������
	  case MY_SIO2 : my_sio2_properties(TiP,rho,cp,lam); break; // SiO2
	  case MY_COPPER : my_copper_properties(TiP,rho,cp,lam); break; // ����
	  case MY_KOVAR : my_kovar_properties(TiP,rho,cp,lam); break; // �����
	  case MY_BRASS : my_brass_properties(TiP,rho,cp,lam); break; // ������ ��-59-1-� Brass
	  case MY_DURALUMIN : my_duralumin_properties(TiP,rho,cp,lam); break; // �����������
	  case MY_ALUMINIUM_NITRIDE: my_aluminium_nitride_properties(TiP, rho, cp, lam); break; // aluminium nitride
	  case MY_GLUE_ECHES: my_glueeches_properties(TiP, rho, cp, lam); break; // ���� ����
	  default : my_duralumin_properties(TiP,rho,cp,lam); break; // ����������� - ������������ �������� �� ���������
	} // end switch
} // my_solid_properties

// ������ �������� �������� ��������� � ��������� ��.
void gran_prop(TEMPER &t, FLOW* &f, BLOCK* b, integer lb, integer iP, integer G, integer ib, TPROP* matlist) {

	// t.ptr[1][iP] - ������������� Fluid domain ��� -1 ���� ��� ������ Solid.

	doublereal rho, cp, lam;
	integer iG; // ����� ������ ������� ����� ��������� ��������� �����.
	iG=t.sosedi[G][iP].iNODE1;
	if (iG>=t.maxelm) {
		// ��� ��������� ����
        rho=1.1614; cp=1005; lam=0.025; // ������������� default  dry air 300K 1atm properties
		if (matlist[b[ib].imatid].blibmat==1) {
			if (b[ib].itype==SOLID) {
		        my_solid_properties(t.potent[iG], rho, cp, lam, matlist[b[ib].imatid].ilibident); // ������������� ����������� � ��������� ����
		    } // SOLID
		    else if (b[ib].itype==FLUID) {
		       doublereal mu, beta_t; // �������� �� ������������ �� ���������.
		       doublereal pressure;
		       if (t.ptr[1][iP]==-1) {
			       pressure=0.0; // �������� ������ ������� ���� (����� �� ����� ����, �.�. ����� ����������� ��������).
		       }
			   else {
				   if (f[t.ptr[1][iP]].sosedi[G][t.ptr[0][iP]].iNODE1 > -1) {
					   pressure = f[t.ptr[1][iP]].potent[PRESS][f[t.ptr[1][iP]].sosedi[G][t.ptr[0][iP]].iNODE1];
				   }
				   else {
					   // STUB 15.09.2018
					   pressure = 0.0;  // � ������ ����.

#if doubleintprecision == 1
					  // printf("error in gran_prop in my_material_properties.c NODE1 G=%lld\n", G);
#else
					   //printf("error in gran_prop in my_material_properties.c NODE1 G=%d\n", G);
#endif

					   
					   //getchar();
					   //system("PAUSE");
					   //exit(1);
				   }
			   }
			   my_fluid_properties(t.potent[iG], pressure, rho, cp, lam, mu, beta_t, matlist[b[ib].imatid].ilibident);
		    } // FLUID
		}
		else if (matlist[b[ib].imatid].blibmat==0) {
            // �������� ����������� �������������:
			// ���������� ��������.
			rho=matlist[b[ib].imatid].rho;
			//cp=matlist[b[ib].imatid].cp;
			//lam=matlist[b[ib].imatid].lam;
			cp = get_lam(matlist[b[ib].imatid].n_cp, matlist[b[ib].imatid].temp_cp, matlist[b[ib].imatid].arr_cp, t.potent[iG]);
			lam = get_lam(matlist[b[ib].imatid].n_lam, matlist[b[ib].imatid].temp_lam, matlist[b[ib].imatid].arr_lam, t.potent[iG]);

			// ��������� ���������� �������� ��������� �� ���������� ����� �������
			// � ��������� ��������� �����.
		}
		// �������� ��� ����������� ������������ ������.
		t.prop_b[RHO][iG-t.maxelm]=rho;
		t.prop_b[CP][iG-t.maxelm]=cp;
		t.prop_b[LAM][iG-t.maxelm]=lam;
	} // G Side


	iG = t.sosedi[G][iP].iNODE2;
	if (iG >= t.maxelm) {
		// ��� ��������� ����
		rho = 1.1614; cp = 1005; lam = 0.025; // ������������� default  dry air 300K 1atm properties
		if (matlist[b[ib].imatid].blibmat == 1) {
			if (b[ib].itype == SOLID) {
				my_solid_properties(t.potent[iG], rho, cp, lam, matlist[b[ib].imatid].ilibident); // ������������� ����������� � ��������� ����
			} // SOLID
			else if (b[ib].itype == FLUID) {
				doublereal mu, beta_t; // �������� �� ������������ �� ���������.
				doublereal pressure;
				if (t.ptr[1][iP] == -1) {
					pressure = 0.0; // �������� ������ ������� ���� (����� �� ����� ����, �.�. ����� ����������� ��������).
				}
				else {
					if (f[t.ptr[1][iP]].sosedi[G][t.ptr[0][iP]].iNODE2 > -1) {
						pressure = f[t.ptr[1][iP]].potent[PRESS][f[t.ptr[1][iP]].sosedi[G][t.ptr[0][iP]].iNODE2];
					}
					else {
						// STUB 15.09.2018
						pressure = 0.0; // � ������ ����.

#if doubleintprecision == 1
						//printf("error in gran_prop in my_material_properties.c NODE2 G=%lld\n", G);
#else
						//printf("error in gran_prop in my_material_properties.c NODE2 G=%d\n", G);
#endif
						
						//getchar();
						//system("PAUSE");
						//exit(1);
					}
				}
				my_fluid_properties(t.potent[iG], pressure, rho, cp, lam, mu, beta_t, matlist[b[ib].imatid].ilibident);
			} // FLUID
		}
		else if (matlist[b[ib].imatid].blibmat == 0) {
			// �������� ����������� �������������:
			// ���������� ��������.
			rho = matlist[b[ib].imatid].rho;
			//cp = matlist[b[ib].imatid].cp;
			//lam = matlist[b[ib].imatid].lam;
			cp = get_lam(matlist[b[ib].imatid].n_cp, matlist[b[ib].imatid].temp_cp, matlist[b[ib].imatid].arr_cp, t.potent[iG]);
			lam = get_lam(matlist[b[ib].imatid].n_lam, matlist[b[ib].imatid].temp_lam, matlist[b[ib].imatid].arr_lam, t.potent[iG]);

			// ��������� ���������� �������� ��������� �� ���������� ����� �������
			// � ��������� ��������� �����.
		}
		// �������� ��� ����������� ������������ ������.
		t.prop_b[RHO][iG - t.maxelm] = rho;
		t.prop_b[CP][iG - t.maxelm] = cp;
		t.prop_b[LAM][iG - t.maxelm] = lam;
	} // G Side

	iG = t.sosedi[G][iP].iNODE3;
	if (iG >= t.maxelm) {
		// ��� ��������� ����
		rho = 1.1614; cp = 1005; lam = 0.025; // ������������� default  dry air 300K 1atm properties
		if (matlist[b[ib].imatid].blibmat == 1) {
			if (b[ib].itype == SOLID) {
				my_solid_properties(t.potent[iG], rho, cp, lam, matlist[b[ib].imatid].ilibident); // ������������� ����������� � ��������� ����
			} // SOLID
			else if (b[ib].itype == FLUID) {
				doublereal mu, beta_t; // �������� �� ������������ �� ���������.
				doublereal pressure;
				if (t.ptr[1][iP] == -1) {
					pressure = 0.0; // �������� ������ ������� ���� (����� �� ����� ����, �.�. ����� ����������� ��������).
				}
				else {
					if (f[t.ptr[1][iP]].sosedi[G][t.ptr[0][iP]].iNODE3 > -1) {
						pressure = f[t.ptr[1][iP]].potent[PRESS][f[t.ptr[1][iP]].sosedi[G][t.ptr[0][iP]].iNODE3];
					}
					else {
						// STUB 15.09.2018
						pressure = 0.0; // � ������ ����.

#if doubleintprecision == 1
						//printf("error in gran_prop in my_material_properties.c NODE3 G=%lld\n", G);
#else
						//printf("error in gran_prop in my_material_properties.c NODE3 G=%d\n", G);
#endif
						
						//getchar();
						//system("PAUSE");
						//exit(1);
					}
				}
				my_fluid_properties(t.potent[iG], pressure, rho, cp, lam, mu, beta_t, matlist[b[ib].imatid].ilibident);
			} // FLUID
		}
		else if (matlist[b[ib].imatid].blibmat == 0) {
			// �������� ����������� �������������:
			// ���������� ��������.
			rho = matlist[b[ib].imatid].rho;
			//cp = matlist[b[ib].imatid].cp;
			//lam = matlist[b[ib].imatid].lam;
			cp = get_lam(matlist[b[ib].imatid].n_cp, matlist[b[ib].imatid].temp_cp, matlist[b[ib].imatid].arr_cp, t.potent[iG]);
			lam = get_lam(matlist[b[ib].imatid].n_lam, matlist[b[ib].imatid].temp_lam, matlist[b[ib].imatid].arr_lam, t.potent[iG]);

			// ��������� ���������� �������� ��������� �� ���������� ����� �������
			// � ��������� ��������� �����.
		}
		// �������� ��� ����������� ������������ ������.
		t.prop_b[RHO][iG - t.maxelm] = rho;
		t.prop_b[CP][iG - t.maxelm] = cp;
		t.prop_b[LAM][iG - t.maxelm] = lam;
	} // G Side


	iG = t.sosedi[G][iP].iNODE4;
	if (iG >= t.maxelm) {
		// ��� ��������� ����
		rho = 1.1614; cp = 1005; lam = 0.025; // ������������� default  dry air 300K 1atm properties
		if (matlist[b[ib].imatid].blibmat == 1) {
			if (b[ib].itype == SOLID) {
				my_solid_properties(t.potent[iG], rho, cp, lam, matlist[b[ib].imatid].ilibident); // ������������� ����������� � ��������� ����
			} // SOLID
			else if (b[ib].itype == FLUID) {
				doublereal mu, beta_t; // �������� �� ������������ �� ���������.
				doublereal pressure;
				if (t.ptr[1][iP] == -1) {
					pressure = 0.0; // �������� ������ ������� ���� (����� �� ����� ����, �.�. ����� ����������� ��������).
				}
				else {
					if (f[t.ptr[1][iP]].sosedi[G][t.ptr[0][iP]].iNODE4 > -1) {
						pressure = f[t.ptr[1][iP]].potent[PRESS][f[t.ptr[1][iP]].sosedi[G][t.ptr[0][iP]].iNODE4];
					}
					else {
						// STUB 15.09.2018
						pressure = 0.0; // � ������ ����.

#if doubleintprecision == 1
						//printf("error in gran_prop in my_material_properties.c NODE4 G=%lld\n", G);
#else
						//printf("error in gran_prop in my_material_properties.c NODE4 G=%d\n", G);
#endif
						
						//getchar();
						//system("PAUSE");
						//exit(1);
					}
				}
				my_fluid_properties(t.potent[iG], pressure, rho, cp, lam, mu, beta_t, matlist[b[ib].imatid].ilibident);
			} // FLUID
		}
		else if (matlist[b[ib].imatid].blibmat == 0) {
			// �������� ����������� �������������:
			// ���������� ��������.
			rho = matlist[b[ib].imatid].rho;
			//cp = matlist[b[ib].imatid].cp;
			//lam = matlist[b[ib].imatid].lam;
			cp = get_lam(matlist[b[ib].imatid].n_cp, matlist[b[ib].imatid].temp_cp, matlist[b[ib].imatid].arr_cp, t.potent[iG]);
			lam = get_lam(matlist[b[ib].imatid].n_lam, matlist[b[ib].imatid].temp_lam, matlist[b[ib].imatid].arr_lam, t.potent[iG]);

			// ��������� ���������� �������� ��������� �� ���������� ����� �������
			// � ��������� ��������� �����.
		}
		// �������� ��� ����������� ������������ ������.
		t.prop_b[RHO][iG - t.maxelm] = rho;
		t.prop_b[CP][iG - t.maxelm] = cp;
		t.prop_b[LAM][iG - t.maxelm] = lam;
	} // G Side

} // gran_prop

bool bswitch_print_message = true;

// ���������� ������� ����������
// ��� ��������� ����������������
void update_temp_properties(TEMPER &t, FLOW* &f, BLOCK* b, integer lb, TPROP* matlist) {
	// �������� ����������, ����� ��� ���������, ����������� � ����������������
	// ����������� ������� �� �����������. � ������ ������� ������������ ����������
	// ������� ���������� � ������ �� ����������� �� �������� �������� �����������.
	// ���� ���������������� ��������� ������ � ������ �����������, �� �� ����� �������
	// � ������������� �������� ������ � ������� ��������� ������������ ����������� 
	// ����� ���� ������� ���� ��� ��������������� ��������� ����������� � �������� 
	// ������� � ����������� ����������.
    integer iP=0;
	//TOCHKA p; // ����� - ����� ���������������� ��.
	integer ib; // ����� ����� �������� ����������� ����������� �����.
	doublereal rho, cp, lam;
	double dmin = 1.0e30;
	double dmax = -1.0e30;
	for (iP=0; iP<t.maxelm; iP++) {
		// ������ �� ���� ���������� ����������� ������� ��������� �������.

		//center_cord3D(iP, t.nvtx, t.pa, p); // ���������� ��������� ������ ��.
		//in_model_temp(p,ib,b,lb); // ���������� ����� ����� ib �������� ����������� ����������� ����� � ������� iP.
		// 8 ������ 2016 ��������� ���������� ib
		
		ib = t.whot_is_block[iP];
		rho=1.1614; cp=1005; lam=0.025; // ������������� default  dry air 300K 1atm properties
		if (matlist[b[ib].imatid].blibmat==1) {
			// ������������, ����������������� ��������.
			if (b[ib].itype==SOLID) {
			    my_solid_properties(t.potent[iP], rho, cp, lam, matlist[b[ib].imatid].ilibident);
		    } // SOLID
		    if (b[ib].itype==FLUID) {
			   doublereal mu, beta_t; // �������� �� ������������ �� ���������.
		       doublereal pressure;
			   if (t.ptr[1][iP]==-1) {
				   pressure=0.0; // �������� ������ ������� ���� (����� �� ����� ����, �.�. ����� ����������� ��������).
			   }
			   else pressure=f[t.ptr[1][iP]].potent[PRESS][t.ptr[0][iP]];
			   my_fluid_properties(t.potent[iP], pressure, rho, cp, lam, mu, beta_t, matlist[b[ib].imatid].ilibident);
		    } // FLUID
		}
		else if (matlist[b[ib].imatid].blibmat==0) {
			// �������� ����������� �������������:
			// ���������� ��������.
			rho=matlist[b[ib].imatid].rho;
			//cp=matlist[b[ib].imatid].cp;
			//lam=matlist[b[ib].imatid].lam;
			cp = get_lam(matlist[b[ib].imatid].n_cp, matlist[b[ib].imatid].temp_cp, matlist[b[ib].imatid].arr_cp, t.potent[iP]);
			lam = get_lam(matlist[b[ib].imatid].n_lam, matlist[b[ib].imatid].temp_lam, matlist[b[ib].imatid].arr_lam, t.potent[iP]);

		}
		// �������� ��� ����������� ������������ ������.
		if (b[ib].itype == FLUID) {
			if (lam > dmax) dmax = lam;
			if (lam < dmin) dmin = lam;
		}
		t.prop[RHO][iP]=rho;
		t.prop[CP][iP]=cp;
		t.prop[LAM][iP]=lam;

		// ������ ��������� ���������� ��������� ����������� ������,
		// ������� ����������� � ������ ���������� ��.
		//
		// 25 �������� 2016 gran_prop ������ �������� �� ���� �����. 
		gran_prop(t, f, b, lb, iP, ESIDE, ib, matlist); // East Side
		gran_prop(t, f, b, lb, iP, WSIDE, ib, matlist); // West Side
		gran_prop(t, f, b, lb, iP, NSIDE, ib, matlist); // North Side
		gran_prop(t, f, b, lb, iP, SSIDE, ib, matlist); // South Side
		gran_prop(t, f, b, lb, iP, TSIDE, ib, matlist); // Top Side
		gran_prop(t, f, b, lb, iP, BSIDE, ib, matlist); // Bottom Side

	}
	if (bswitch_print_message) {
		printf("lam_min=%e lam_max=%e \n", dmin, dmax);
	}
	bswitch_print_message = !bswitch_print_message;
} // update_temp_properties 

  // ���������� ������� ����������
  // ��� ��������� ����������������
void update_temp_properties1(TEMPER &t, FLOW* &f, BLOCK* b, integer lb, 
	TPROP* matlist, doublereal* &temperature, integer iadd, integer iu,
	doublereal* &lam_export, integer** &nvtx_global) {
	// �������� ����������, ����� ��� ���������, ����������� � ����������������
	// ����������� ������� �� �����������. � ������ ������� ������������ ����������
	// ������� ���������� � ������ �� ����������� �� �������� �������� �����������.
	// ���� ���������������� ��������� ������ � ������ �����������, �� �� ����� �������
	// � ������������� �������� ������ � ������� ��������� ������������ ����������� 
	// ����� ���� ������� ���� ��� ��������������� ��������� ����������� � �������� 
	// ������� � ����������� ����������.
	integer iP = 0;
	//TOCHKA p; // ����� - ����� ���������������� ��.
	integer ib; // ����� ����� �������� ����������� ����������� �����.
	doublereal rho, cp, lam;
	double dmin = 1.0e30;
	double dmax = -1.0e30;
	for (iP = iadd; iP<iadd+t.maxelm; iP++) {
		// ������ �� ���� ���������� ����������� ������� ��������� �������.

		
		doublereal Temperature_in_cell = 0.125*(temperature[nvtx_global[0][iP]-1]+ temperature[nvtx_global[1][iP] - 1] + temperature[nvtx_global[2][iP] - 1] + temperature[nvtx_global[3][iP] - 1] + temperature[nvtx_global[4][iP] - 1] + temperature[nvtx_global[5][iP] - 1] + temperature[nvtx_global[6][iP] - 1] + temperature[nvtx_global[7][iP] - 1]);

		if (iu == -1) {
			//center_cord3D(iP, t.nvtx, t.pa, p); // ���������� ��������� ������ ��.
			//in_model_temp(p,ib,b,lb); // ���������� ����� ����� ib �������� ����������� ����������� ����� � ������� iP.
			// 8 ������ 2016 ��������� ���������� ib

			ib = t.whot_is_block[iP];

		}
		else {
			ib = t.whot_is_block[iP - iadd];
		}
		rho = 1.1614; cp = 1005; lam = 0.025; // ������������� default  dry air 300K 1atm properties
		if (matlist[b[ib].imatid].blibmat == 1) {
			// ������������, ����������������� ��������.
			if (b[ib].itype == SOLID) {
				my_solid_properties(Temperature_in_cell, rho, cp, lam, matlist[b[ib].imatid].ilibident);
			} // SOLID
			if (b[ib].itype == FLUID) {
				doublereal mu, beta_t; // �������� �� ������������ �� ���������.
				doublereal pressure;
				if (t.ptr[1][iP-iadd] == -1) {
					pressure = 0.0; // �������� ������ ������� ���� (����� �� ����� ����, �.�. ����� ����������� ��������).
				}
				else pressure = f[t.ptr[1][iP-iadd]].potent[PRESS][t.ptr[0][iP-iadd]];
				my_fluid_properties(Temperature_in_cell, pressure, rho, cp, lam, mu, beta_t, matlist[b[ib].imatid].ilibident);
			} // FLUID
		}
		else if (matlist[b[ib].imatid].blibmat == 0) {
			// �������� ����������� �������������:
			// ���������� ��������.
			rho = matlist[b[ib].imatid].rho;
			//cp=matlist[b[ib].imatid].cp;
			//lam=matlist[b[ib].imatid].lam;
			cp = get_lam(matlist[b[ib].imatid].n_cp, matlist[b[ib].imatid].temp_cp, matlist[b[ib].imatid].arr_cp, Temperature_in_cell);
			lam = get_lam(matlist[b[ib].imatid].n_lam, matlist[b[ib].imatid].temp_lam, matlist[b[ib].imatid].arr_lam, Temperature_in_cell);

		}
		// �������� ��� ����������� ������������ ������.
		if (b[ib].itype == FLUID) {
			if (lam > dmax) dmax = lam;
			if (lam < dmin) dmin = lam;
		}
		t.prop[RHO][iP - iadd] = rho;
		t.prop[CP][iP - iadd] = cp;
		t.prop[LAM][iP - iadd] = lam;
		lam_export[iP] = lam;

		// ������ ��������� ���������� ��������� ����������� ������,
		// ������� ����������� � ������ ���������� ��.
		//
		// 25 �������� 2016 gran_prop ������ �������� �� ���� �����. 
		gran_prop(t, f, b, lb, iP - iadd, ESIDE, ib, matlist); // East Side
		gran_prop(t, f, b, lb, iP - iadd, WSIDE, ib, matlist); // West Side
		gran_prop(t, f, b, lb, iP - iadd, NSIDE, ib, matlist); // North Side
		gran_prop(t, f, b, lb, iP - iadd, SSIDE, ib, matlist); // South Side
		gran_prop(t, f, b, lb, iP - iadd, TSIDE, ib, matlist); // Top Side
		gran_prop(t, f, b, lb, iP - iadd, BSIDE, ib, matlist); // Bottom Side

	}
	if (bswitch_print_message) {
		printf("lam_min=%e lam_max=%e \n", dmin, dmax);
	}
	//bswitch_print_message = !bswitch_print_message;
} // update_temp_properties1 

// ���������� ����������� ������������ �������� �������� �������������
// 26 ������ 2012 ���������� �� ������������ ���������.
doublereal return_dynamic_viscosity(TPROP* matlist, integer imatid, 
	                          bool bfirst_start, doublereal SInvariantStrainRateTensor) {
	doublereal mu;

	// ���������� ������������ ��������:
	switch (matlist[imatid].ilawmu) {
	case 0 : // ���������� ��������
		mu=matlist[imatid].mu;
		break;
	case 1 : // power-law fluid
		// ����� ���������-�� ���� (Ostwald de Vel)
		if ((!bfirst_start)&&(SInvariantStrainRateTensor>0.0)) {
		   mu=fmax(matlist[imatid].mumin,fmin(2.0*matlist[imatid].Amu*exp((matlist[imatid].degreennmu-1.0)*log(SInvariantStrainRateTensor)),matlist[imatid].mumax));
		}
		else {
			if (matlist[imatid].degreennmu>1.0) {
				mu=matlist[imatid].mumin;
			}
			else if ((matlist[imatid].degreennmu<1.0)&&(matlist[imatid].degreennmu>0.0)) {
				mu=matlist[imatid].mumax;
			} else mu=matlist[imatid].mu;
		}
		break;
	case 2 : // ����� ������� Caisson
		// �������� ������ ������� �������� ������� � ������������ ���������� ������.
		if ((!bfirst_start)&&(SInvariantStrainRateTensor>0.0)) {
			if (matlist[imatid].Amu>=0.0) {
				mu=fmax(matlist[imatid].mumin,fmin(((sqrt(matlist[imatid].Amu/SInvariantStrainRateTensor))+matlist[imatid].Bmu)*((sqrt(matlist[imatid].Amu/SInvariantStrainRateTensor))+matlist[imatid].Bmu),matlist[imatid].mumax));
			}
			else {
				printf("A constant is given incorrectly on the law of the caisson\n");
					printf("for the non-Newtonian fluid");
					printf("please, press any key to exit...\n");
					exit(0);
			}
		}
		else mu=matlist[imatid].mumax;
		break;
	case 3 : // ����� ��������
		// �������� ������ �������� ��� B>0 �������� ���������� � ����������� ���������� ������,
		// � ��� B<0 ������������ �������� ������� � ������ ���������� ������.
		if ((!bfirst_start)&&(SInvariantStrainRateTensor>0.0)) {
			if ( (fabs(matlist[imatid].Amu)>1e-30) && (fabs(matlist[imatid].Bmu)>1e-30) && ((SInvariantStrainRateTensor/matlist[imatid].Bmu>-1.0)&&(SInvariantStrainRateTensor/matlist[imatid].Bmu<1.0))) {
				mu=fmax(matlist[imatid].mumin,fmin(matlist[imatid].mumax,(matlist[imatid].Amu/SInvariantStrainRateTensor)*asin(SInvariantStrainRateTensor/matlist[imatid].Bmu)));
			}
			else {
				printf("A or B constant is given incorrectly on the law of the Prandtl\n");
				printf("for the non-Newtonian fluid");
				printf("please, press any key to exit...\n");
				exit(0);
			}
		}
		else {
			if (matlist[imatid].Bmu>0.0) {
	    		mu=matlist[imatid].mumin;
			}
			else mu=matlist[imatid].mumax;
		}
		break;
	case 4 : // Carreau fluid
		mu=fmax(matlist[imatid].mumin,fmin(matlist[imatid].mumax,matlist[imatid].Amu+(matlist[imatid].Bmu-matlist[imatid].Amu)*exp(0.5*(matlist[imatid].degreennmu-1.0)*log(1.0+(matlist[imatid].Cmu*SInvariantStrainRateTensor)*(matlist[imatid].Cmu*SInvariantStrainRateTensor)))));
		break;
	case 5 : // ������-������
		// ���� B*C>0.0 �� ������������ �������� ������� � ������ ���������� ������,
		// ���� B*C<0.0 �� ������������ �������� ���������� � ������ ���������� ������.
		if ((!bfirst_start)&&(SInvariantStrainRateTensor>0.0)) {
			// ����-����� Arsh(x)=log(x+sqrt(x*x+1.0));
			mu=fmax(matlist[imatid].mumin,fmin(matlist[imatid].mumax,matlist[imatid].Amu+(matlist[imatid].Bmu/SInvariantStrainRateTensor)*log(matlist[imatid].Cmu*SInvariantStrainRateTensor+sqrt(1.0+(matlist[imatid].Cmu*SInvariantStrainRateTensor)*(matlist[imatid].Cmu*SInvariantStrainRateTensor)))));
		} else 
		{
			if (matlist[imatid].Bmu*matlist[imatid].Cmu>0.0) {
				mu=matlist[imatid].mumax;
			} else mu=matlist[imatid].mumin;
		}
		break;
	case 6 : // Williamson non-Newtonian fluid
		// ����� ������� �������������� ��� ��������� ������� ������ ��������� (��������������� ������� � ��������� ������� �� ������������).
		mu=fmax(matlist[imatid].mumin,fmin(matlist[imatid].mumax,matlist[imatid].Amu/(matlist[imatid].Bmu+SInvariantStrainRateTensor)+matlist[imatid].Cmu));
		break;
	default : // ���������� ��������
		mu=matlist[imatid].mu;
		break;
	}

	return mu;
} // return_dynamic_viscosity


// ��������� �������� ���������� � ������ �������������
// ������ �������� ���������� � ��������� ��.
void gran_prop_flow(TEMPER &t, FLOW* &f, BLOCK* b, integer lb, integer iflow,
	                integer iP, integer G, integer ib, TPROP* matlist, bool bfirst_start) {

       doublereal rho=0.0, mu=0.0, beta_t=0.0;
	   integer iG=0; // ����� ������ ������� ����� ��������� ��������� �����.
	   iG=f[iflow].sosedi[G][iP].iNODE1;
	   if (iG>=f[iflow].maxelm) {
		   // ��� ��������� ����
           rho=1.1614; mu=1.84e-5; beta_t=0.003331; // ������������� default dry air 300K 1atm properties
		   if (matlist[b[ib].imatid].blibmat==1) {
				doublereal cp, lam;
				cp=1005; lam=0.025;
				// ������������ ����������������� ��������
				doublereal temperature=20.0;
				if (t.sosedi[G][f[iflow].ptr[iP]].iNODE1>=t.maxelm) {
					// ��������� ��� ����������� �����
                    temperature=t.potent[t.sosedi[G][f[iflow].ptr[iP]].iNODE1];
				} else {
					if (t.sosedi[G][f[iflow].ptr[iP]].iNODE1 > -1) {
						// ����������� �� ����� ����������������� �������� �������������:
						TOCHKA p1, p2;
						center_cord3D(f[iflow].ptr[iP], t.nvtx, t.pa, p1,100); // ���������� ��������� ������ ��.
						center_cord3D(t.sosedi[G][f[iflow].ptr[iP]].iNODE1, t.nvtx, t.pa, p2, G);
						doublereal fgplus = 0.0;
						doublereal dx = 0.0, dy = 0.0, dz = 0.0;
						volume3D(f[iflow].ptr[iP], t.nvtx, t.pa, dx, dy, dz);
						switch (G) {
						case ESIDE: case WSIDE: fgplus = dx / fabs(p1.x - p2.x); break;
						case NSIDE: case SSIDE: fgplus = dy / fabs(p1.y - p2.y); break;
						case TSIDE: case BSIDE: fgplus = dz / fabs(p1.z - p2.z); break;
						}

						temperature = (1.0 - fgplus)*t.potent[f[iflow].ptr[iP]] + fgplus*t.potent[t.sosedi[G][f[iflow].ptr[iP]].iNODE1];
					}
					else {
#if doubleintprecision == 1
						printf("error in gran_prop_flow in my_material_properties.c NODE1 G=%lld\n", G);
#else
						printf("error in gran_prop_flow in my_material_properties.c NODE1 G=%d\n", G);
#endif
						
						//getchar();
						system("PAUSE");
						exit(1);
					}
				}
                // ������������ ����������������� ��������
				if (b[ib].itype==FLUID) {
					my_fluid_properties(temperature, f[iflow].potent[PRESS][iG], rho, cp, lam, mu, beta_t, matlist[b[ib].imatid].ilibident);
				}
		   }
		   else if (matlist[b[ib].imatid].blibmat==0) {
				// �������� ����������� �������������:
				// ���������� ��������.

			    // ���������
				rho=matlist[b[ib].imatid].rho;
				// ������������ ��������
                mu=return_dynamic_viscosity(matlist, b[ib].imatid, bfirst_start, f[iflow].SInvariantStrainRateTensor[iG]);
				// ����������� ��������� �������������� ����������.
				beta_t=matlist[b[ib].imatid].beta_t;
				// ��������� ���������� �������� ��������� �� ���������� ����� �������
				// � ��������� ��������� �����.
			}
		    // �������� ���������� ������������ ������:
		    f[iflow].prop_b[RHO][iG-f[iflow].maxelm]=rho;
		    f[iflow].prop_b[MU][iG-f[iflow].maxelm]=mu;
		    f[iflow].prop_b[BETA_T][iG-f[iflow].maxelm]=beta_t;

	   } // G Side

	   iG = f[iflow].sosedi[G][iP].iNODE2;
	   if (iG >= f[iflow].maxelm) {
		   // ��� ��������� ����
		   rho = 1.1614; mu = 1.84e-5; beta_t = 0.003331; // ������������� default dry air 300K 1atm properties
		   if (matlist[b[ib].imatid].blibmat == 1) {
			   doublereal cp, lam;
			   cp = 1005; lam = 0.025;
			   // ������������ ����������������� ��������
			   doublereal temperature = 20.0;
			   if (t.sosedi[G][f[iflow].ptr[iP]].iNODE2 >= t.maxelm) {
				   // ��������� ��� ����������� �����
				   temperature = t.potent[t.sosedi[G][f[iflow].ptr[iP]].iNODE2];
			   }
			   else {
				   if (t.sosedi[G][f[iflow].ptr[iP]].iNODE2>-1) {
					   // ����������� �� ����� ����������������� �������� �������������:
					   TOCHKA p1, p2;
					   center_cord3D(f[iflow].ptr[iP], t.nvtx, t.pa, p1,100); // ���������� ��������� ������ ��.
					   center_cord3D(t.sosedi[G][f[iflow].ptr[iP]].iNODE2, t.nvtx, t.pa, p2,G);
					   doublereal fgplus = 0.0;
					   doublereal dx = 0.0, dy = 0.0, dz = 0.0;
					   volume3D(f[iflow].ptr[iP], t.nvtx, t.pa, dx, dy, dz);
					   switch (G) {
					   case ESIDE: case WSIDE: fgplus = dx / fabs(p1.x - p2.x); break;
					   case NSIDE: case SSIDE: fgplus = dy / fabs(p1.y - p2.y); break;
					   case TSIDE: case BSIDE: fgplus = dz / fabs(p1.z - p2.z); break;
					   }

					   temperature = (1.0 - fgplus)*t.potent[f[iflow].ptr[iP]] + fgplus*t.potent[t.sosedi[G][f[iflow].ptr[iP]].iNODE2];
				   }
				   else {
#if doubleintprecision == 1
					   printf("error in gran_prop_flow in my_material_properties.c NODE2 G=%lld\n", G);
#else
					   printf("error in gran_prop_flow in my_material_properties.c NODE2 G=%d\n", G);
#endif
					   
					   //getchar();
					   system("PAUSE");
					   exit(1);
				   }
			   }
			   // ������������ ����������������� ��������
			   if (b[ib].itype == FLUID) {
				   my_fluid_properties(temperature, f[iflow].potent[PRESS][iG], rho, cp, lam, mu, beta_t, matlist[b[ib].imatid].ilibident);
			   }
		   }
		   else if (matlist[b[ib].imatid].blibmat == 0) {
			   // �������� ����������� �������������:
			   // ���������� ��������.

			   // ���������
			   rho = matlist[b[ib].imatid].rho;
			   // ������������ ��������
			   mu = return_dynamic_viscosity(matlist, b[ib].imatid, bfirst_start, f[iflow].SInvariantStrainRateTensor[iG]);
			   // ����������� ��������� �������������� ����������.
			   beta_t = matlist[b[ib].imatid].beta_t;
			   // ��������� ���������� �������� ��������� �� ���������� ����� �������
			   // � ��������� ��������� �����.
		   }
		   // �������� ���������� ������������ ������:
		   f[iflow].prop_b[RHO][iG - f[iflow].maxelm] = rho;
		   f[iflow].prop_b[MU][iG - f[iflow].maxelm] = mu;
		   f[iflow].prop_b[BETA_T][iG - f[iflow].maxelm] = beta_t;

	   } // G Side

	   iG = f[iflow].sosedi[G][iP].iNODE3;
	   if (iG >= f[iflow].maxelm) {
		   // ��� ��������� ����
		   rho = 1.1614; mu = 1.84e-5; beta_t = 0.003331; // ������������� default dry air 300K 1atm properties
		   if (matlist[b[ib].imatid].blibmat == 1) {
			   doublereal cp, lam;
			   cp = 1005; lam = 0.025;
			   // ������������ ����������������� ��������
			   doublereal temperature = 20.0;
			   if (t.sosedi[G][f[iflow].ptr[iP]].iNODE3 >= t.maxelm) {
				   // ��������� ��� ����������� �����
				   temperature = t.potent[t.sosedi[G][f[iflow].ptr[iP]].iNODE3];
			   }
			   else {
				   if (t.sosedi[G][f[iflow].ptr[iP]].iNODE3>-1) {
					   // ����������� �� ����� ����������������� �������� �������������:
					   TOCHKA p1, p2;
					   center_cord3D(f[iflow].ptr[iP], t.nvtx, t.pa, p1,100); // ���������� ��������� ������ ��.
					   center_cord3D(t.sosedi[G][f[iflow].ptr[iP]].iNODE3, t.nvtx, t.pa, p2,G);
					   doublereal fgplus = 0.0;
					   doublereal dx = 0.0, dy = 0.0, dz = 0.0;
					   volume3D(f[iflow].ptr[iP], t.nvtx, t.pa, dx, dy, dz);
					   switch (G) {
					   case ESIDE: case WSIDE: fgplus = dx / fabs(p1.x - p2.x); break;
					   case NSIDE: case SSIDE: fgplus = dy / fabs(p1.y - p2.y); break;
					   case TSIDE: case BSIDE: fgplus = dz / fabs(p1.z - p2.z); break;
					   }

					   temperature = (1.0 - fgplus)*t.potent[f[iflow].ptr[iP]] + fgplus*t.potent[t.sosedi[G][f[iflow].ptr[iP]].iNODE3];
				   }
				   else {
#if doubleintprecision == 1
					   printf("error in gran_prop_flow in my_material_properties.c NODE3 G=%lld\n", G);
#else
					   printf("error in gran_prop_flow in my_material_properties.c NODE3 G=%d\n", G);
#endif
					   
					   //getchar();
					   system("PAUSE");
					   exit(1);
				   }
			   }
			   // ������������ ����������������� ��������
			   if (b[ib].itype == FLUID) {
				   my_fluid_properties(temperature, f[iflow].potent[PRESS][iG], rho, cp, lam, mu, beta_t, matlist[b[ib].imatid].ilibident);
			   }
		   }
		   else if (matlist[b[ib].imatid].blibmat == 0) {
			   // �������� ����������� �������������:
			   // ���������� ��������.

			   // ���������
			   rho = matlist[b[ib].imatid].rho;
			   // ������������ ��������
			   mu = return_dynamic_viscosity(matlist, b[ib].imatid, bfirst_start, f[iflow].SInvariantStrainRateTensor[iG]);
			   // ����������� ��������� �������������� ����������.
			   beta_t = matlist[b[ib].imatid].beta_t;
			   // ��������� ���������� �������� ��������� �� ���������� ����� �������
			   // � ��������� ��������� �����.
		   }
		   // �������� ���������� ������������ ������:
		   f[iflow].prop_b[RHO][iG - f[iflow].maxelm] = rho;
		   f[iflow].prop_b[MU][iG - f[iflow].maxelm] = mu;
		   f[iflow].prop_b[BETA_T][iG - f[iflow].maxelm] = beta_t;

	   } // G Side


	   iG = f[iflow].sosedi[G][iP].iNODE4;
	   if (iG >= f[iflow].maxelm) {
		   // ��� ��������� ����
		   rho = 1.1614; mu = 1.84e-5; beta_t = 0.003331; // ������������� default dry air 300K 1atm properties
		   if (matlist[b[ib].imatid].blibmat == 1) {
			   doublereal cp, lam;
			   cp = 1005; lam = 0.025;
			   // ������������ ����������������� ��������
			   doublereal temperature = 20.0;
			   if (t.sosedi[G][f[iflow].ptr[iP]].iNODE4 >= t.maxelm) {
				   // ��������� ��� ����������� �����
				   temperature = t.potent[t.sosedi[G][f[iflow].ptr[iP]].iNODE4];
			   }
			   else {
				   if (t.sosedi[G][f[iflow].ptr[iP]].iNODE4 > -1) {
					   // ����������� �� ����� ����������������� �������� �������������:
					   TOCHKA p1, p2;
					   center_cord3D(f[iflow].ptr[iP], t.nvtx, t.pa, p1,100); // ���������� ��������� ������ ��.
					   center_cord3D(t.sosedi[G][f[iflow].ptr[iP]].iNODE4, t.nvtx, t.pa, p2,G);
					   doublereal fgplus = 0.0;
					   doublereal dx = 0.0, dy = 0.0, dz = 0.0;
					   volume3D(f[iflow].ptr[iP], t.nvtx, t.pa, dx, dy, dz);
					   switch (G) {
					   case ESIDE: case WSIDE: fgplus = dx / fabs(p1.x - p2.x); break;
					   case NSIDE: case SSIDE: fgplus = dy / fabs(p1.y - p2.y); break;
					   case TSIDE: case BSIDE: fgplus = dz / fabs(p1.z - p2.z); break;
					   }

					   temperature = (1.0 - fgplus)*t.potent[f[iflow].ptr[iP]] + fgplus*t.potent[t.sosedi[G][f[iflow].ptr[iP]].iNODE4];
				   }
				   else {
#if doubleintprecision == 1
					   printf("error in gran_prop_flow in my_material_properties.c NODE4 G=%lld\n", G);
#else
					   printf("error in gran_prop_flow in my_material_properties.c NODE4 G=%d\n", G);
#endif
					   
					   //getchar();
					   system("PAUSE");
					   exit(1);
				   }
			   }
			   // ������������ ����������������� ��������
			   if (b[ib].itype == FLUID) {
				   my_fluid_properties(temperature, f[iflow].potent[PRESS][iG], rho, cp, lam, mu, beta_t, matlist[b[ib].imatid].ilibident);
			   }
		   }
		   else if (matlist[b[ib].imatid].blibmat == 0) {
			   // �������� ����������� �������������:
			   // ���������� ��������.

			   // ���������
			   rho = matlist[b[ib].imatid].rho;
			   // ������������ ��������
			   mu = return_dynamic_viscosity(matlist, b[ib].imatid, bfirst_start, f[iflow].SInvariantStrainRateTensor[iG]);
			   // ����������� ��������� �������������� ����������.
			   beta_t = matlist[b[ib].imatid].beta_t;
			   // ��������� ���������� �������� ��������� �� ���������� ����� �������
			   // � ��������� ��������� �����.
		   }
		   // �������� ���������� ������������ ������:
		   f[iflow].prop_b[RHO][iG - f[iflow].maxelm] = rho;
		   f[iflow].prop_b[MU][iG - f[iflow].maxelm] = mu;
		   f[iflow].prop_b[BETA_T][iG - f[iflow].maxelm] = beta_t;

	   } // G Side
} // gran_prop_flow

// ���������� ������� ���������� ��� �������������
void update_flow_properties(TEMPER &t, FLOW* &f, BLOCK* b, integer lb, integer flow_interior, TPROP* matlist, bool bfirst_start) {

	// ���� ������ ��� ��� �� �������� ���������� ������� ���������� ����������� �� �����.
	for (integer ifi=0; ifi<flow_interior; ifi++) {
		// ������ �� ���� ������ �����.
		integer iP=0;
		//TOCHKA p; // ����� ����� ���������������� ��.
		integer ib; // ����� ����� �������� ����������� ��������������� ����������� �����.
		doublereal rho, mu, beta_t;
		double dmin = 1.0e30;
		double dmax = -1.0e30;

		for (iP=0; iP<f[ifi].maxelm; iP++) {
			// ������ �� ���� ���������� ����������� �������
			//center_cord3D(iP, f[ifi].nvtx, f[ifi].pa, p,100); // ���������� ��������� ������ ��.
			//in_model_flow(p, ib, b, lb); // ���������� ����� ����� ib �������� ����������� ����������� ����� � ������� iP.
			// ����� ������� �������� - ������������� � hash �������.
			ib=t.whot_is_block[f[ifi].ptr[iP]];

			rho=1.1614; mu=1.84e-5; beta_t=0.003331; // ������������� default dry air 300K 1atm properties

			if (ib > -1) {
				if (matlist[b[ib].imatid].blibmat == 1) {
					doublereal cp, lam;
					cp = 1005; lam = 0.025;
					// ������������ ����������������� ��������
					if (b[ib].itype == FLUID) {
						//printf("rho=%e, mu=%e",rho,mu); // debug
						//getchar();
						my_fluid_properties(t.potent[f[ifi].ptr[iP]], f[ifi].potent[PRESS][iP], rho, cp, lam, mu, beta_t, matlist[b[ib].imatid].ilibident);
						//printf("rho=%e, mu=%e",rho,mu); // debug
						//getchar();
					}
				}
				else if (matlist[b[ib].imatid].blibmat == 0) {
					// �������� ����������� �������������:
					// ���������� ��������.
					rho = matlist[b[ib].imatid].rho;
					// ���������� ������������ ��������:
					// � ��� ����� � ��� �������������� ���������.
					mu = return_dynamic_viscosity(matlist, b[ib].imatid, bfirst_start, f[ifi].SInvariantStrainRateTensor[iP]);
					// ����������� ��������� �������������� ����������
					beta_t = matlist[b[ib].imatid].beta_t;
				}
			}
			// �������� ����������� ������������ ������:
			if (mu > dmax) dmax = mu;
			if (mu < dmin) dmin = mu;
			f[ifi].prop[RHO][iP]=rho;
			f[ifi].prop[MU][iP]=mu;
			f[ifi].prop[BETA_T][iP]=beta_t;
			//printf("rho=%e, mu=%e",f[ifi].prop[RHO][iP],f[ifi].prop[MU][iP]); // debug
			//getchar();

			// ������ ��������� ���������� ��������� ����������� ������,
			// ������� ����������� � ������ ���������� ��.
			//
			gran_prop_flow(t, f, b, lb, ifi, iP, ESIDE, ib, matlist, bfirst_start); // East Side
            gran_prop_flow(t, f, b, lb, ifi, iP, WSIDE, ib, matlist, bfirst_start); // West Side
			gran_prop_flow(t, f, b, lb, ifi, iP, NSIDE, ib, matlist, bfirst_start); // North Side
            gran_prop_flow(t, f, b, lb, ifi, iP, SSIDE, ib, matlist, bfirst_start); // South Side
			gran_prop_flow(t, f, b, lb, ifi, iP, TSIDE, ib, matlist, bfirst_start); // Top Side
            gran_prop_flow(t, f, b, lb, ifi, iP, BSIDE, ib, matlist, bfirst_start); // Bottom Side
		}

		printf("\nmu_min=%e mu_max=%e \n",dmin, dmax);
	}
} // update_flow_properties

// ��������� ����� �������������� ������.
// 12.03.2017
doublereal massa_cabinet(TEMPER &t, FLOW* &f, integer &inx, integer &iny, integer &inz,
	doublereal* &xpos, doublereal* &ypos, doublereal* &zpos, integer &flow_interior,
	BLOCK* b, integer lb, doublereal temp_ref,
	TPROP* matlist) {

	doublereal massa = 0.0;

	for (integer iP = 0; iP < t.maxelm; iP++) {
		// ���������� �������� �������� ������������ ������:
		doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
		volume3D(iP, t.nvtx, t.pa, dx, dy, dz);
		integer ib = t.whot_is_block[iP];
		doublereal rho, cp, lam;
		rho = 1.1614; cp = 1005; lam = 0.025; // ������������� default  dry air 300K 1atm properties
		if (matlist[b[ib].imatid].blibmat == 1) {
			// ������������, ����������������� ��������.
			if (b[ib].itype == SOLID) {
				my_solid_properties(t.potent[iP], rho, cp, lam, matlist[b[ib].imatid].ilibident);
			} // SOLID
			if (b[ib].itype == FLUID) {
				doublereal mu, beta_t; // �������� �� ������������ �� ���������.
				doublereal pressure;
				if (t.ptr[1][iP] == -1) {
					pressure = 0.0; // �������� ������ ������� ���� (����� �� ����� ����, �.�. ����� ����������� ��������).
				}
				else pressure = f[t.ptr[1][iP]].potent[PRESS][t.ptr[0][iP]];
				my_fluid_properties(t.potent[iP], pressure, rho, cp, lam, mu, beta_t, matlist[b[ib].imatid].ilibident);
			} // FLUID
		}
		else if (matlist[b[ib].imatid].blibmat == 0) {
			// �������� ����������� �������������:
			// ���������� ��������.
			rho = matlist[b[ib].imatid].rho;
			//cp=matlist[b[ib].imatid].cp;
			//lam=matlist[b[ib].imatid].lam;
			cp = get_lam(matlist[b[ib].imatid].n_cp, matlist[b[ib].imatid].temp_cp, matlist[b[ib].imatid].arr_cp, t.potent[iP]);
			lam = get_lam(matlist[b[ib].imatid].n_lam, matlist[b[ib].imatid].temp_lam, matlist[b[ib].imatid].arr_lam, t.potent[iP]);

		}
		// �������� ��� ����������� ������������ ������.
		massa += rho*dx*dy*dz;
	}

	printf("massa=%1.3f kg\n", massa);

	return massa;
}


#endif