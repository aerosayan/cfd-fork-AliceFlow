// classic_aglomerative_amg6_2018year.cpp
// ��������� �� ������� ���� �������������� ������������� ����� �����_v0_14.
// ��� �������� ����� ��������� ���������� � �����������.

#pragma once
#ifndef CLASSIC_AGLOMERATIVE_AMG6_2018YEAR_CPP
#define CLASSIC_AGLOMERATIVE_AMG6_2018YEAR_CPP 1

#include "basic_interpolation_procedure_my_agregat_amg.cpp" // interpolation procedure for Ak2 type
#include "basic_functions_my_agregat_amg.cpp"

#define VEB_FLAG 1

#if VEB_FLAG
// ���������� 21.03.2019
// ��� �������� ��� ����� ������ min � �������� min(a,b) ����� ��.
// ���������� ��� ���� ������ max. ���� ������� min � max ������������� 
// � ������ ������ veb_min, veb_max. ������ �������� ��� �����������.
// ������ ��� ���� �����.
// ������ ��� ���� ����� ������ ���������� 30.06.2018 - ��������� 21.09.2018
// ��� �������� �� log(log(U))
//#include "veb.h"
#include "veb.cpp"
#endif

// ������ � ������������� ���������� ����������,
// ��������� �� ����� 1.0. ��� ������������
// ��������� ������ ����� �� ����� 1.0.
typedef struct TBAD_STRING_PATCHING {
	integer ilevel;
	integer istring_number;
} BAD_STRING_PATCHING;

// ��������� ������������ ��������� ������� �� �������.
#include "spgemm_Matrix_by_Matrix_sparse_multiplication.cpp"


void calculate_row_ptr(integer istart, integer iend,
	integer* &row_ind_SA, integer* & row_ind_EA,
	bool* &flag, const Ak2& Amat)
{
	// ������������� ������.

	integer i_size_75 = 0;
	// ��� ������ ��������������.
	for (integer ii = istart; ii <= iend; ii++) {
		if (flag[Amat.i[ii]] == false) {
			row_ind_SA[Amat.i[ii]] = ii;
			flag[Amat.i[ii]] = true;
			i_size_75++;
		}
	}
#pragma omp parallel for
	for (integer istr = 1; istr <= i_size_75; istr++) {
		integer kf = row_ind_SA[istr];
		while ((kf <= iend) && (Amat.i[kf] == istr)) {
			kf++;
		}
		kf--;
		row_ind_EA[istr] = kf;

	}

}

void calculate_row_ptr(integer istart, integer iend,
	integer*& row_ind_SA, integer*& row_ind_EA,
	bool*& flag, Ak1* &Amat)
{
	// ������������� ������.

	integer i_size_75 = 0;
	// ��� ������ ��������������.
	for (integer ii = istart; ii <= iend; ii++) {
		if (flag[Amat[ii].i] == false) {
			row_ind_SA[Amat[ii].i] = ii;
			flag[Amat[ii].i] = true;
			i_size_75++;
		}
	}
#pragma omp parallel for
	for (integer istr = 1; istr <= i_size_75; istr++) {
		integer kf = row_ind_SA[istr];
		while ((kf <= iend) && (Amat[kf].i == istr)) {
			kf++;
		}
		kf--;
		row_ind_EA[istr] = kf;

	}

}

void calculate_row_ptr_j(integer istart, integer iend,
	integer*& row_ind_SA, integer*& row_ind_EA,
	bool*& flag, Ak1*& Amat)
{
	// ������������� ������.

	
	// ��� ������ ��������������.
	for (integer ii = istart; ii <= iend; ii++) {
		if (flag[Amat[ii].j] == false) {
			// ��������� ���������.
			integer istr = Amat[ii].j;
			integer ic = ii;

			integer kf = ic;

			while ((kf <= iend) && (Amat[kf].j == istr)) {
				kf++;
			}
			kf--;
			row_ind_SA[istr] = ic;
			row_ind_EA[istr] = kf;
			flag[Amat[ii].j] = true;
			ii = kf;
		}
	}

}

void calculate_row_ptr(integer istart, integer iend,
	integer*& row_ind_SA, 
	bool*& flag, const Ak2& Amat)
{
	// ������������� ������.

	integer i_size_75 = 0;
	// ��� ������ ��������������.
	for (integer ii = istart; ii <= iend; ii++) {
		if (flag[Amat.i[ii]] == false) {
			row_ind_SA[Amat.i[ii]] = ii;
			flag[Amat.i[ii]] = true;
			i_size_75++;
		}
	}
	row_ind_SA[Amat.i[iend] + 1] = iend + 1;

}

void calculate_row_ptr(integer istart, integer iend,
	integer*& row_ind_SA, 
	bool*& flag, Ak1*& Amat)
{
	// ������������� ������.

	integer i_size_75 = 0;
	// ��� ������ ��������������.
	for (integer ii = istart; ii <= iend; ii++) {
		if (flag[Amat[ii].i] == false) {
			row_ind_SA[Amat[ii].i] = ii;
			flag[Amat[ii].i] = true;
			i_size_75++;
		}
	}
	row_ind_SA[Amat[iend].i + 1] = iend + 1;

}

// ��� ����� Solution � Setup phase
typedef struct Tamg_precond_param {
	integer maxlevel;
	integer ilevel;
	bool bprint_mesage_diagnostic;
	bool bonly_serial;
	integer bILU2smoother;
	integer nnz_P_memo_0;
	integer nnz_P_memo_all;
	doublereal dr_grid_complexity;
	bool debug_reshime;
	doublereal dapply_ilu_max_pattern_size;
	doublereal RealZERO;
	bool identiti;
	integer memo_icoarseningtype;
} amg_precond_param;

// ���� ������� � amg ������ 14.04.2020.
template <typename doublerealT>
bool solution_phase(Ak2& Amat,
	integer nsizeA, // ���������� ����� ���������� ����� ��� ��������� ������ �	
	integer nnz, // number of non zero elements
	integer n, // dimension of vectors x and b.	
	doublereal*& x, // solution (�������) 
	doublereal*& b, // rthdsd (������ �����).
	doublerealT& ret74,
	integer iVar,
	bool bmemory_savings,	
	BLOCK*& my_body, integer& lb, integer maxelm_out,
	// ��������� ������ ������ ��� solution phase.
	amg_precond_param& amg_pp,
	integer* &n_a, integer*& nnz_a, integer*& nnz_aRP,
	integer& ibsp_length,	
	BAD_STRING_PATCHING*& bsp,
	integer i_bsp_LIMIT,
	bool* &flag,	
	bool* &F_false_C_true, Ak1* &P,
	INIT_SELECTOR_CASE_CAMG_RUMBAv_0_14 imyinit
)
{


	 integer maxlevel = amg_pp.maxlevel;
	 integer ilevel = amg_pp.ilevel;
	 bool bprint_mesage_diagnostic = amg_pp.bprint_mesage_diagnostic;
	 bool bonly_serial = amg_pp.bonly_serial;
	 integer bILU2smoother = amg_pp.bILU2smoother;
	 integer nnz_P_memo_0 = amg_pp.nnz_P_memo_0;
	 integer nnz_P_memo_all = amg_pp.nnz_P_memo_all;
	 doublerealT dr_grid_complexity = amg_pp.dr_grid_complexity;
	 bool debug_reshime = amg_pp.debug_reshime;
	 doublerealT dapply_ilu_max_pattern_size = amg_pp.dapply_ilu_max_pattern_size;
	 doublerealT RealZERO = amg_pp.RealZERO;
	 bool identiti = amg_pp.identiti;
	 integer memo_icoarseningtype = amg_pp.memo_icoarseningtype;



	// 4-5-6 30-31 dec 2016 �������������� �� ����� 50 ������� �����������
	// 5.06.2017 �������������� �� ����� 100 ������� ����������� ������������. const integer maxlevel=101;
	// 16.02.2019.
	doublerealT** diag = nullptr;
	diag = new doublerealT * [maxlevel];
	if (diag == nullptr) {
		// ������������ ������ �� ������ ������������.
		std::cout << "Problem: not enough memory on your equipment for diag my_gregat_amg.cpp..." << std::endl;
		std::cout << "Please any key to exit..." << std::endl;
		exit(1);
	}
	for (integer i_id_level_local = 0; i_id_level_local < maxlevel; i_id_level_local++) {
		diag[i_id_level_local] = nullptr; // �������������.
		if (ilevel > i_id_level_local) {
			diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
			handle_error<doublerealT>(diag[i_id_level_local], "diag[", i_id_level_local, "]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
		}
	}


	doublerealT** diag_minus_one = nullptr;
	diag_minus_one = new doublerealT * [maxlevel];
	if (diag_minus_one == nullptr) {
		// ������������ ������ �� ������ ������������.
		std::cout <<  "Problem: not enough memory on your equipment for diag_minus_one my_gregat_amg.cpp..." << std::endl;
		std::cout <<  "Please any key to exit..." << std::endl;
		exit(1);
	}
	for (integer i_id_level_local = 0; i_id_level_local < maxlevel; i_id_level_local++) {
		diag_minus_one[i_id_level_local] = nullptr; // �������������.
		if (ilevel > i_id_level_local) {
			diag_minus_one[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
			handle_error<doublerealT>(diag_minus_one[i_id_level_local], "diag_minus_one[", i_id_level_local, "]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
		}
	}

	for (integer i_id_level_local = 0; i_id_level_local < maxlevel; i_id_level_local++) {
		if (ilevel > i_id_level_local) {
			for (integer i_96 = 1; i_96 <= n_a[i_id_level_local]; i_96++) {
				diag_minus_one[i_id_level_local][i_96] = 1.0;
			}
		}
	}
	// �������� ������� ��� ������ ����� ��� � ���������� ��������� �� ����� 1,0.
	for (integer i_96 = 0; i_96 < ibsp_length; i_96++) {
		diag_minus_one[bsp[i_96].ilevel][bsp[i_96].istring_number] = -1.0;
	}

	bnested_desection_global_amg = nullptr;
	bool** nested_desection = nullptr;
	nested_desection = new bool* [maxlevel];
	if (nested_desection == nullptr) {
		// ������������ ������ �� ������ ������������.
		std::cout <<  "Problem: not enough memory on your equipment for nested_desection my_gregat_amg.cpp..." << std::endl;
		std::cout <<  "Please any key to exit..." << std::endl;
		exit(1);
	}
	for (integer i_id_level_local = 0; i_id_level_local < maxlevel; i_id_level_local++) {
		nested_desection[i_id_level_local] = nullptr;
	}

	if (!bonly_serial) {
		// nested desection start
		bnested_desection_global_amg = (bool*)malloc((n_a[0] + 1) * sizeof(bool));
		handle_error<bool>(bnested_desection_global_amg, "bnested_desection_global_amg", "classic_aglomerative_amg_6", (n_a[0] + 1));


		nested_desection[0] = (bool*)malloc((n_a[0] + 1) * sizeof(bool));
		handle_error<bool>(nested_desection[0], "nested_desection[0]", "classic_aglomerative_amg_6", (n_a[0] + 1));

		//maxlevel==201
		for (integer i_17 = 1; i_17 <= maxlevel - 1; i_17++) {
			if (ilevel > i_17) {
				nested_desection[i_17] = (bool*)malloc((n_a[i_17] + 1) * sizeof(bool));
				handle_error<bool>(nested_desection[i_17], "nested_desection[i_17]", "classic_aglomerative_amg_6", (n_a[i_17] + 1));
			}
		}

	}
	// nested_desection_end


	const integer isize_row_ptr = 4 * n_a[0] + 1;

	integer* row_ptr_start = nullptr;
	//row_ptr_start = new integer[4 * n_a[0] + 1];
	row_ptr_start = (integer*)malloc((isize_row_ptr) * sizeof(integer));
	handle_error<integer>(row_ptr_start, " row_ptr_start", "classic_aglomerative_amg_6", (isize_row_ptr));

	integer* row_ptr_end = nullptr;
	//row_ptr_end = new integer[4 * n_a[0] + 1];
	row_ptr_end = (integer*)malloc((isize_row_ptr) * sizeof(integer));
	handle_error<integer>(row_ptr_end, " row_ptr_end", "classic_aglomerative_amg_6", (isize_row_ptr));

	// ILU2
	LEVEL_ADDITIONAL_DATA* milu2 = nullptr;
	// �������������.
	init_level_additional_data(milu2, ilevel);

	// ILU0
	LEVEL_ADDITIONAL_DATA0* milu0 = nullptr;
	// �������������.
	init_level_additional_data(milu0, ilevel);

	// ������������ ����� ������ � ILU ������.
	if (milu_gl_buffer.alu_copy != nullptr) delete[] milu_gl_buffer.alu_copy;
	if (milu_gl_buffer.jlu_copy != nullptr) delete[] milu_gl_buffer.jlu_copy;
	if (milu_gl_buffer.ju_copy != nullptr) delete[] milu_gl_buffer.ju_copy;
	milu_gl_buffer.alu_copy = nullptr;
	milu_gl_buffer.jlu_copy = nullptr;
	milu_gl_buffer.ju_copy = nullptr;

	// istart - ��������� ������� ��������� ��������� � ������� �.
	// iend - �������� ������� ��������� ��������� � ������� �.
#pragma omp parallel for
	for (integer i = 1; i <= n; i++) {
		flag[i] = false;
	}
	for (integer ii = 1; ii <= nnz_a[0]; ii++) {
		if (flag[Amat.i[ii]] == false) {
			integer istr = Amat.i[ii];
			integer ic = ii;
			integer icdiag = ii;
			if (istr >= isize_row_ptr) {
				std::cout << "need to increase isize_row_ptr " <<  istr << std::endl;

				//getchar();
				system("PAUSE");
				exit(1);
			}
			row_ptr_start[istr] = ii;
			doublerealT ap = 0.0; // �������� �� ���������.
								  //x[istr] = b[istr];
			while ((ic <= nnz_a[0]) && (Amat.i[ic] == istr)) {
				if (Amat.j[ic] != istr) {
					//x[istr] += -Amat.aij[ic]*x[Amat.j[ic]];
					// ��� ��������������� �������� ������ ���� ������ ������������.
					// ���� ��� �� ��� �� ���� �������� �������������� � ���������� ������ ������������.
					if (Amat.aij[ic] >= 0.0) {
						//std::cout << "polochitelnji vnediagonalnj element " <<  Amat.aij[ic] << " in matrix level 0 in string " << istr << "..." << std::endl;

						// ����� ��� �� ������� 26 ������� 2016.
						// �� �� �� ������ � ���������� ����������� ���� � �� ������� ������ �����������.
						//system("PAUSE");
					}
				}
				else {
					// ������������ ������� ������ �����������.
					ap = Amat.aij[ic];
					icdiag = ic;
				}
				ic++;
			}
			if (istr >= isize_row_ptr) {
				std::cout <<  "need to increase isize_row_ptr " << istr << std::endl;
				//getchar();
				system("PAUSE");
				exit(1);
			}
			row_ptr_end[istr] = ic - 1;
			if (fabs(ap) < RealZERO) {
				std::cout <<  "zero diagonal elements in string " << istr <<"in basic matrix" << std::endl;
				system("PAUSE");
				exit(1);
			}
			else {
				//x[istr] /= ap;
			}

			flag[Amat.i[ii]] = true;
			swap(Amat, ii, icdiag);

			if (bmemory_savings) {
				// �� ��������� ������ �������� �������,
				// �� ������ ��� ������� ����������.
				the_original_order_of_values[the_original_order_of_values_reverse[ii]] = icdiag;
				the_original_order_of_values[the_original_order_of_values_reverse[icdiag]] = ii;
			}

			diag[0][Amat.i[ii]] = ap; // ��� ��������� ���������� �������.
			Amat.aij[ii] = 1.0 / ap; // ��������� ������� �������.
		}
	}

	if (bILU2smoother == 2) {
		// ILU2
		if (bprint_mesage_diagnostic) {
			std::cout << "apply ilu" << my_amg_manager.lfil <<" smoother for number 0 level\n";
		}
		equation3DtoCRSRUMBA1(milu2[0], true, Amat, 1, n_a[0], row_ptr_start, row_ptr_end, 0, 0);
	}

	// 14 �������� 2015 ����������� �������� ������� �����������.
	// ������ ����������� � ������� �� ������� �����. 12.07.2016.

	// ��������� 29.10.2016.
	if (bprint_mesage_diagnostic) {
		std::cout <<  "1. positive connections %, 2. max positive/ diagonal %" << std::endl;
	}

	for (integer ilevel_detector = 1; ilevel_detector <= maxlevel - 1; ilevel_detector++) {

		// ��������� ������� ��������� �� 99 ������ ������������, ��
		// ���� ���������� ���������� ������� ��� ���� �������.
		const integer istop_level_scan = maxlevel - 2;

		if (ilevel > ilevel_detector) {

			integer istart_row_ptr0 = 0;
			for (integer ilev = 0; ilev < ilevel_detector; ilev++) {
				istart_row_ptr0 += n_a[ilev];
			}

			doublerealT inum_vnediagonal_all = 0.0;
			doublerealT inum_only_positive_vnediagonal = 0.0;
			doublerealT memo_diagonal_element = 0.0;
			doublerealT max_positive_connections_element = -1.0;
			doublerealT ratio_positive_connections_by_diagonalelement = -1.0;
			doublerealT ratio_positive_connections_by_diagonalelement_avg = 0.0;
			bool b_ne_menee_2_positive_con_in_string = false;
			doublerealT inum_only_positive_vnediagonal_ne_menee2_in_string = 0.0;

			for (integer i = 1; i <= n; i++) {
				flag[i] = false;
			}
			integer ist = 1;
			for (integer ilev = 0; ilev < ilevel_detector; ilev++) {
				ist += nnz_a[ilev];
			}
			integer iend = 0;
			for (integer ilev = 0; ilev <= ilevel_detector; ilev++) {
				iend += nnz_a[ilev];
			}
			integer istPR = 1;
			for (integer ilev = 0; ilev < ilevel_detector; ilev++) {
				istPR += nnz_aRP[ilev];
			}
			integer iendPR = 0;
			for (integer ilev = 0; ilev <= ilevel_detector; ilev++) {
				iendPR += nnz_aRP[ilev];
			}
			double dn_num = 0.0;
			for (integer ii = ist; ii <= iend; ii++) {
				if (flag[Amat.i[ii]] == false) {

					integer istr = Amat.i[ii];
					integer ic = ii;
					integer icdiag = ii;
					integer istart_row_ptr = istr + istart_row_ptr0;

					max_positive_connections_element = -1.0;
					dn_num += 1.0;


					if (istart_row_ptr >= isize_row_ptr) {
						std::cout <<  "need to increase isize_row_ptr " << istart_row_ptr << std::endl;
						//getchar();
						system("PAUSE");
						exit(1);
					}
					if (ilevel_detector <= istop_level_scan) {
						row_ptr_start[istart_row_ptr] = ii;
					}
					doublerealT ap = 0.0;
					doublerealT sum_4 = 0.0;




					const doublerealT theta7 = theta(ilevel); // ��������� � ������� �����.
					b_ne_menee_2_positive_con_in_string = false;
					integer inum_pos_con_in_string = 0;
					doublerealT threshold7 = -1.0;
					integer ic7 = ic;
					while ((ic7 <= iend) && (Amat.i[ic7] == istr)) {
						if (Amat.j[ic7] != istr) {
							if (Amat.aij[ic7] >= 0.0) {
								inum_pos_con_in_string++;
								if (fabs(Amat.aij[ic7]) > threshold7) threshold7 = fabs(Amat.aij[ic7]);
							}
						}
						ic7++;
					}
					// �� ���������� �� ����� ���� ������������� ������ � ������ ������.
					if (inum_pos_con_in_string >= 2) {
						inum_pos_con_in_string = 0;
						ic7 = ic;
						while ((ic7 <= iend) && (Amat.i[ic7] == istr)) {
							if (Amat.j[ic7] != istr) {
								if ((Amat.aij[ic7] >= 0.0) && (fabs(Amat.aij[ic7]) >= theta7 * threshold7)) {
									inum_pos_con_in_string++;
								}
							}
							ic7++;
						}

						if (inum_pos_con_in_string >= 2) {
							b_ne_menee_2_positive_con_in_string = true;
						}
					}


					//x[istr] = b[istr];
					while ((ic <= iend) && (Amat.i[ic] == istr)) {
						if (Amat.j[ic] != istr) {
							//x[istr] += -Amat.aij[ic]*x[Amat.j[ic]];
							inum_vnediagonal_all += 1.0;
							// ��� ��������������� �������� ������ ���� ������ ������������.
							// ���� ��� �� ��� �� ���� �������� �������������� � ���������� ������ ������������.
							if (Amat.aij[ic] >= 0.0) {
								//std::cout << "polochitelnji vnediagonalnj element " << Amat.aij[ic] << " in matrix level " << ilevel_detector << " in string " << istr << "..."<< std::endl;

								//system("PAUSE");
								inum_only_positive_vnediagonal += 1.0;

								if (b_ne_menee_2_positive_con_in_string) {
									if (fabs(Amat.aij[ic7]) >= theta7 * threshold7) {
										inum_only_positive_vnediagonal_ne_menee2_in_string += 1.0;
									}
								}

								// ����������� �������� ������������ ��������������� �����.
								if (max_positive_connections_element < Amat.aij[ic]) {
									max_positive_connections_element = Amat.aij[ic];
								}
							}
						}
						else {
							ap = Amat.aij[ic];
							memo_diagonal_element = ap;
							icdiag = ic;
						}
						ic++;
					}


					if (istart_row_ptr >= isize_row_ptr) {
						std::cout <<  "need to increase isize_row_ptr "<<  istart_row_ptr << std::endl;
						//getchar();
						system("PAUSE");
						exit(1);
					}
					if (ilevel_detector <= istop_level_scan) {
						row_ptr_end[istart_row_ptr] = ic - 1;
					}
					if (fabs(ap) < RealZERO) {
						std::cout <<  "zero diagonal elements in string " << istr << " in level "<< ilevel <<" matrix";
						system("PAUSE");
						exit(1);
					}
					else {
						//x[istr] /= ap;
					}


					ratio_positive_connections_by_diagonalelement_avg += fabs(max_positive_connections_element / memo_diagonal_element);
					if (ratio_positive_connections_by_diagonalelement < fabs(max_positive_connections_element / memo_diagonal_element)) {
						ratio_positive_connections_by_diagonalelement = fabs(max_positive_connections_element / memo_diagonal_element);
					}
					flag[Amat.i[ii]] = true;
					if (ilevel_detector <= istop_level_scan) {
						swap(Amat,ii,icdiag);
						diag[ilevel_detector][Amat.i[ii]] = ap;// ��� ��������� ���������� �������.						
						Amat.aij[ii] = 1.0 / ap; // ��������� ������� �������.
					}


				}
			}

			integer iadd_now = 0;
			for (integer i54 = 1; i54 <= ilevel_detector; i54++) {
				iadd_now += n_a[i54 - 1];
			}
			if (ilevel_detector <= istop_level_scan) {
				if (bILU2smoother == 2) {
					if (bprint_mesage_diagnostic) {
						std::cout <<  "apply ilu" << my_amg_manager.lfil <<" smoother for number "<< ilevel_detector <<" level\n";
					}

					equation3DtoCRSRUMBA1(milu2[ilevel_detector], true,
						Amat, 1, n_a[ilevel_detector], row_ptr_start, row_ptr_end, iadd_now, ilevel_detector);
				}
			}

			// statistic log:
			if (bprint_mesage_diagnostic) {
				//std::cout << "procent positive connections " << 100.0*inum_only_positive_vnediagonal / inum_vnediagonal_all << " " << std::endl;
				//std::cout << "the ratio of the maximum positive connections to the diagonal" << std::endl;
				//std::cout << "element in the row, in procent " << 100.0*ratio_positive_connections_by_diagonalelement << std::endl;
				//std::cout << "\n";

				std::cout <<  ilevel_detector << " " << 1.00 * inum_only_positive_vnediagonal / inum_vnediagonal_all << " % [ " << 100 * inum_only_positive_vnediagonal_ne_menee2_in_string / inum_vnediagonal_all << " % ] " <<  1e-4 * ratio_positive_connections_by_diagonalelement << "  [" << 1e-4 * ratio_positive_connections_by_diagonalelement_avg / dn_num << " ]" << std::endl;
			}
		}


	}
	
	if (bILU2smoother > 0) {
		// ����� ������ � ������ ���������� ILU ������������.
		//system("PAUSE");
		if (bILU2smoother == 2) {
			// ��������� �������� ��� ������ ��������.
			// ��������� ����������� ������ ��� ���������������� ��������� 
			// ��� ILU.
			memory_allocation_apostoriory_buffer_ilu(milu2, ilevel - 1);
			//memory_allocation_apostoriory_buffer_ilu(milu2, ilevel-1);// 4.01.2017
		}
	}


	if (!bonly_serial) {

		// ������� nested desection
		// ��� ���� �������.
		// ����� ��������� ������� 1.
		// nested_desection[1]		
		// maxlevel==201
		for (integer i_17 = 0; i_17 <= maxlevel - 1; i_17++) {
			if (ilevel > i_17) {
				integer inasum = 0;
				for (integer i_18 = 0; i_18 < i_17; i_18++) inasum += n_a[i_18];
				nested_desection_patch(Amat, n_a[i_17], nested_desection[i_17], row_ptr_start, row_ptr_end, inasum);
				if (bprint_mesage_diagnostic) {

					std::cout <<  "part" << i_17 <<std::endl; 
					std::cout <<  "nested desection is finish" << std::endl;
				}
			}
		}


	}


	if (Amat.i != nullptr) {
		// ����������� ����� ����� ������ ��� �������� ������, �.�. ������ 
		// ��������� � ������� i � ��� ���� row_ptr 
		// row_ptr_start, row_ptr_end (������ �� ������ � ����� ������ ������).
		free(Amat.i);
		Amat.i = nullptr;
	}
	//if (R != nullptr) {
		// ������������ ������ �������� P, �������� R ����� ����� �� ��� � P � ��������� �� ����������.
		// ������ restriction � prolongation �� ������������� � ����������.
		//free(R);
		//R = nullptr;
	//}

	if (flag != nullptr) {
		free(flag);
		flag = nullptr;
	}

	if (bprint_mesage_diagnostic) {
		std::cout <<  "cycling: V cycle." << std::endl;
		std::cout << "level=" << ilevel << std::endl;

		std::cout << "multigrid R.P.Fedorenko 1961." << std::endl;
		std::cout << "standart aglomerative algebraic multigrid method." << std::endl;
	}
	if (debug_reshime) system("pause");
	//exit(1);

	// 10 11 21 multigrid tutorial ����� �����.
	// ������������������� ��������� �������� � DavisTest,
	// �������� ������� �� x-���������� ��������. ����� ���������
	// � ����������� �������� ���������� ������. ��� ���� ������������
	// amg v0.08 �������� �� ������ ������ � ����������� nu1=4, nu2=3.
	// ��������� nu1=8, nu2=7 ���������� ���������� ��������������� ��������.

	// �������� ����� ����� ������� ����������� ����� �� ������������, ��������
	// ���� ������� ���������� ���� �������������� � 0.1 ��� �� ����� ����� �������� 
	// �������� �������� ������������.


	// nu1=0; nu2=2; nFinestSweeps=2 is recomended 
	// Masashi Imano. Optimization of parameter setting for GAMG
	// solver in simple solver. 
	// Aug 26th 2012. OpeanFOAM study. 
	// nu1=0 ����� ������������ �� BSK Dmitrii.
	// nFinestSweeps=2 ����� ������������ �� BSK Dmitrii.
	// BSK Dmitrii �������� ��� nu1=1, nu2=2, nFinestSweeps=3.

	integer nu1 = 1; // minimum value 1 // 4 // 8
	integer nu2 = 2; // minimum value 2 // 3 // 7

					 // �� ������ Finned Heat Sync �� ������� ��������� Icepak ���� ���������� ������������ 
					 // ��� Y �������� � �������� ��������. ��� ���� ��������� ���� ������� ��������� �� �����������
					 // ����� � nu1=1, nu2=2 ���� ��� ������ ������� ������ ����������.
					 // ��� nu1=10, nu2=10 �������� ����������� ������ � ������� � ���� ���, �� �������� �������� �� �������� ��� ����.
					 // ��� nu==20 ���� �� ����� ����.
					 // �� �������.
					 //nu1 = 40;
					 //nu2 = 40;

	integer nFinestSweeps = 2;


	// � 26 ������� 2016 �� ������� ��������� �� ���������� AliceMesh_v0_39.
	// �.�. ���� ���������������� ������, �� ��� ��������� ������ ������.
	nu1 = my_amg_manager.nu1;
	nu2 = my_amg_manager.nu2;
	nFinestSweeps = my_amg_manager.nFinnest;

	//if (iVar == PAM) {
	//nFinestSweeps = 300;
	//nu1 = 0;
	//nu2 = 20;
	//}
	// ��� Finner Heat Sink ���� ������� �����������.
	// ��� �� �������� ����� ��������������� �� ������ ��������.
	//if (iVar == PAM) {
	//nu1 = 3;
	//nu2 = 3;
	//nFinestSweeps = 6;
	//}
	const bool btheoryGuideANSYSFluent = false;
	if (iVar != PAM) {
		if (btheoryGuideANSYSFluent) {
			// ��� �������� � Theory Guide ANSYS Fluent.
			nu1 = 0;
			nu2 = 1;
			nFinestSweeps = 1;
		}
	}



	// ������� ��������� ���������� �������� �������� ����������:
	//nu1 = 10;
	//nu2 = 20;

	// ����� ���� ���������� � ��� ��� ��� �������� ������� ����������
	// � ������ ����������� ��������� ����� �� ���� � ���� �����������.
	// �.�. ��������� nu1,nu2 �������� � ������� � �������� ��� ����������
	// ������� ��� ����� �������� ��� ����������� ������ (����������). 
	// ������������ �� ������ ������ ����� �������� ��������� nu1, nu2 
	// � ����� �� ������� ���������� ��������. 
	doublerealT process_flow_beta = 0.7;
	doublerealT process_flow_alpha = 0.1;
	bool process_flow_logic = false;

	if (process_flow_logic) {
		nu1 = 40;
		nu2 = 40;
	}

	d_my_optimetric2_6_12_2019 = dr_grid_complexity;
	if (bprint_mesage_diagnostic) {
		std::cout << "grid complexity is " << dr_grid_complexity << std::endl;
		std::cout << "Prolongation operator complexity is |Psigma|/|P1|=" << (doublerealT)(1.0 * nnz_P_memo_all / nnz_P_memo_0) <<"  "<< (doublerealT)(1.0 * nnz_P_memo_all / n_a[0]) <<"*n" << std::endl;
		std::cout << "number pre-cycles nu1=" << nu1 <<", number post-cycles nu2=" << nu2 <<"\n";
	}

	//ilevel = 1; //debug
	doublerealT rho = 1.0;
	doublerealT dres = 1.0;
	integer iiter = 1;
	//const doublerealT tolerance = 1.0e-12;
	// 13 ������� 2016 ��������� �������� ������� � ����� ��������� ��������� ����������.
	// �.�. ��� ������� �������� ���� ��� ������� ���� ������� �� �����������.
	// ��������� �������� ������� ���������� �������� 7000.0.
	doublerealT tolerance = 0.0001; // �������� ������ �� ������������� ����������� L2 �����.
									// 23 ������� 2016
	if (bSIMPLErun_now_for_temperature) {
		// ������ cfd ������.
		tolerance = 1.0e-8;
	}

	doublerealT** residual_fine = nullptr;
	residual_fine = new doublerealT * [maxlevel];
	if (residual_fine == nullptr) {
		// ������������ ������ �� ������ ������������.
		std::cout << "Problem: not enough memory on your equipment for residual_fine my_gregat_amg6.cpp..." << std::endl;
		std::cout <<  "Please any key to exit..." << std::endl;
		exit(1);
	}
	doublerealT** residual_coarse = nullptr;
	residual_coarse = new doublerealT * [maxlevel];
	if (residual_coarse == nullptr) {
		// ������������ ������ �� ������ ������������.
		std::cout <<  "Problem: not enough memory on your equipment for residual_coarse my_gregat_amg6.cpp..."  << std::endl;
		std::cout <<  "Please any key to exit..." << std::endl;
		exit(1);
	}
	doublerealT** error_approx_coarse = nullptr;
	error_approx_coarse = new doublerealT * [maxlevel];
	if (error_approx_coarse == nullptr) {
		// ������������ ������ �� ������ ������������.
		std::cout <<  "Problem: not enough memory on your equipment for error_approx_coarse my_gregat_amg6.cpp..."  << std::endl;
		std::cout <<  "Please any key to exit..."  << std::endl;
		exit(1);
	}
	doublerealT** error_approx_fine = nullptr;
	error_approx_fine = new doublerealT * [maxlevel];
	if (error_approx_fine == nullptr) {
		// ������������ ������ �� ������ ������������.
		std::cout <<  "Problem: not enough memory on your equipment for error_approx_fine my_gregat_amg6.cpp..." << std::endl;
		std::cout <<  "Please any key to exit..." << std::endl;
		exit(1);
	}
	for (integer i_id_level_local = 0; i_id_level_local < maxlevel; i_id_level_local++) {
		residual_fine[i_id_level_local] = nullptr;
		residual_coarse[i_id_level_local] = nullptr;
		error_approx_coarse[i_id_level_local] = nullptr;
		error_approx_fine[i_id_level_local] = nullptr;
	}


	// ���������� ��� ������������� ��������� nullptr 4 ������� 2016. 

	// ������������������ ��� ��������� �������. � ������ ������ 
	//5.06.2017 �������������� 100 ������� �����������.

	// 25.04.2018 �� ���� ����� ����� ������� �������� ����������� ����.

	// ����� �������� ����������� ������ ���������� ������� �.�.
	// ����������� ������ ��������������� ���������� dll �
	// �������� ���������� ����� ����� �� �������.




	// maxlevel==201
	for (integer i_17 = 1; i_17 <= maxlevel - 1; i_17++) {
		// 05.06.2017
		integer i_17_prev = i_17 - 1;

		if (ilevel + 1 > i_17) {

			// residual
			//residual_fine[i_17_prev] = new doublerealT[n_a[i_17_prev] + 1];
			residual_fine[i_17_prev] = (doublerealT*)malloc((n_a[i_17_prev] + 1) * sizeof(doublerealT));
			handle_error<doublerealT>(residual_fine[i_17_prev], "residual_fine[", i_17_prev, "]", "classic_aglomerative_amg_6", (n_a[i_17_prev] + 1));


			//residual_coarse[i_17_prev] = new doublerealT[n_a[i_17] + 1];
			residual_coarse[i_17_prev] = (doublerealT*)malloc((n_a[i_17] + 1) * sizeof(doublerealT));
			handle_error<doublerealT>(residual_coarse[i_17_prev], "residual_coarse[", i_17_prev, "]", "classic_aglomerative_amg_6", (n_a[i_17] + 1));

			//error_approx_coarse[i_17_prev] = new doublerealT[n_a[i_17] + 1];
			error_approx_coarse[i_17_prev] = (doublerealT*)malloc((n_a[i_17] + 1) * sizeof(doublerealT));
			handle_error<doublerealT>(error_approx_coarse[i_17_prev], "error_approx_coarse[", i_17_prev, "]", "classic_aglomerative_amg_6", (n_a[i_17] + 1));

			//error_approx_fine[i_17_prev] = new doublerealT[n_a[i_17_prev] + 1];
			error_approx_fine[i_17_prev] = (doublerealT*)malloc((n_a[i_17_prev] + 1) * sizeof(doublerealT));
			handle_error<doublerealT>(error_approx_fine[i_17_prev], "error_approx_fine[", i_17_prev, "]", "classic_aglomerative_amg_6", (n_a[i_17_prev] + 1));
		}
	}




	// 2 jan 2016. 
	integer igam = 1; // 1-V; 2-W
	//INIT_SELECTOR_CASE_CAMG_RUMBAv_0_14 imyinit = ZERO_INIT; // ZERO_INIT optimum

	doublerealT* x_copy = nullptr;
	x_copy = (doublerealT*)malloc((n_a[0] + 1) * sizeof(doublerealT));
	handle_error<doublerealT>(x_copy, "x_copy", "classic_aglomerative_amg_6", (n_a[0] + 1));

	// ��� ��������� ����� � ��������� ����������.
	doublerealT* x_old = nullptr;
	x_old = (doublerealT*)malloc((n_a[0] + 1) * sizeof(doublerealT));
	handle_error<doublerealT>(x_old, "x_old", "classic_aglomerative_amg_6", (n_a[0] + 1));

#pragma omp parallel for
	for (integer i47 = 1; i47 <= n_a[0]; i47++) {
		x_copy[i47] = x[i47];
		x_old[i47] = x[i47];
		//x_copy[i47] = 0.0; // 28.07.2016
	}

	doublereal* x_best_search = nullptr;
	x_best_search = (doublerealT*)malloc((n_a[0] + 1) * sizeof(doublereal));
	handle_error<doublerealT>(x_best_search, "x_best_search", "classic_aglomerative_amg_6", (n_a[0] + 1));

	doublerealT res_best_search = 1e40;
#pragma omp parallel for
	for (integer i47 = 1; i47 <= n_a[0]; i47++) {
		x_best_search[i47] = x[i47];
		//x_best_search[i47] = 0.0; // 28.07.2016
	}


	// ��� �������� �������� ��������� ������ ����� �� ���� �������� ����� ������� �������,
	// ��� �������� � ���� ��� ����� �������� ���������� ����� � �� ����� �������, ������� ����� 
	// �������� �������� �������� �� ���������� ���������� ���������� �������� (�� ����� 1000 ��������).
	// 1000 �������� ��� ����� ����� ������� ��� �������� �������� ���� ��������� �������� ���������� 
	// �������� �.�. �� ����� ������������ ������� ������� �������������� ������������������ ���������.
	integer iter_limit = 0;
	integer istop_porog_reconst = 5000;// 50

	bool ret_value = false;
	doublerealT dres_previos = 1.0e40;
	integer icount_bad_convergence_Vcycles = 0;
	integer i_count_stagnation = 0;
	doublerealT res0start = 1.0e-40;
	bool bfirst_divergence = true;

	residualq2(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0], diag[0], diag_minus_one[0]);
	doublerealT dres_initial = norma(residual_fine[0], n_a[0]);
	if (((iVar == VELOCITY_X_COMPONENT) || (iVar == VELOCITY_Y_COMPONENT) || (iVar == VELOCITY_Z_COMPONENT)) && (dres_initial > 20.0)) {
		// ��� ������� ������ � ������ ������� ���� �� ���������� ��������.
		std::cout <<  "may be problem convergence Speed Flow: very big dres0=" << dres_initial << std::endl;
		std::cout <<  "run residualq2 analysys." << std::endl;
		residualq2_analysys(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0], diag[0], diag_minus_one[0]);
	}
	if (((iVar == NUSHA) || (iVar == TURBULENT_KINETIK_ENERGY) ||
		(iVar == TURBULENT_KINETIK_ENERGY_STD_K_EPS) || (iVar == TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS)) && (dres_initial > 20.0)) {
		// ��� ������� ������ � ������ ������� ���� �� ������������ ��������������.
		std::cout <<  "may be problem convergence Turbulence equations: very big dres0=" << dres_initial << std::endl;
		std::cout <<  "run residualq2 analysys." << std::endl;
		residualq2_analysys(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0], diag[0], diag_minus_one[0]);
	}
	if ((iVar == TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA) && (dres_initial > 6.0e6)) {
		// ��� ������� ������ � ������ ������� ���� �� ������������ �������������� - �������� �������� ����������.
		std::cout <<  "may be problem convergence Turbulence equations: very big dres0=" << dres_initial << std::endl;
		std::cout <<  "run residualq2 analysys." << std::endl;
		residualq2_analysys(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0], diag[0], diag_minus_one[0]);
	}
	/*
	// ��� �������������� �� amg5:
	integer iflag_cont = 1;
	if (iVar != PAM) {
	dres = fabs(dres_initial);

	if (iVar != TEMP) {
	if (dres < dterminatedTResudual) {
	// ������ � ��� ����� ������������� �������, ��� �� ���� �������� �� ������� ����.
	iflag_cont = 0;
	}
	}
	else {
	if (dres < 1.0e-4*dterminatedTResudual) {
	// ������ � ��� ����� ������������� �������, ��� �� ���� �������� �� ������� ����.
	iflag_cont = 0;
	}
	}
	}
	iflag_cont = 1;
	*/

	if (bprint_mesage_diagnostic) {
		// start residual.
		std::cout <<  0 << " " << dres_initial <<" rho=" << dres_initial / rho << std::endl;
	}

	// TODO 25 10 2016
	integer iflag_cont = 1;
	if (iVar != PAM) {
		dres = fabs(dres_initial);
	}


	integer count_iter_for_film_coef = 0;
	// ���� ����� ������������� �������� ���������� ��������� �� ��������� ����� �� ���������.
	integer i_signal_break_pam_opening = 0;
	// x ������� ��������.
	const integer i_limit_signal_pam_break_opening = 1000; // 8
	doublerealT delta_old_iter = 1.0e10;



	//if (iVar == PAM) {// ����
	//for (integer iter = 0; iter < 2; iter++) {
	//seidelq(Amat, 1, n_a[0], b, x, row_ptr_start, row_ptr_end, 0);
	//}
	//}
	integer icount_V_cycle = 0;

	doublerealT dres_initial_ = 1e-6;


	doublerealT maxold = -1.0e30;
	for (integer i = 1; i <= n_a[0]; i++) {
		if (x[i] > maxold) maxold = x[i];
	}

	// � 26 ������� 2016 �� ������� ��������� �� ���������� AliceMesh_v0_39.
	// �.�. ���� ���������������� ������, �� ��� ��������� ������ ������.
	// �������� ������ ������� ���� ���������� �������� ���������� �������������.
	//if (ilevel > my_amg_manager.maximum_levels) {
	//ilevel = my_amg_manager.maximum_levels;
	//}
	ilevel -= my_amg_manager.maximum_delete_levels;



	doublereal* x_best_search2 = nullptr;
	x_best_search2 = new doublereal[n_a[0] + 1];
	if (x_best_search2 == nullptr) {
		// ������������ ������ �� ������ ������������.
		std::cout << "Problem: not enough memory on your equipment for x_best_search2 my_agregat_amg.cpp..." << std::endl;
		std::cout << "Please any key to exit..." << std::endl;
		exit(1);
	}
	doublereal* x_best_search_init = nullptr;
	x_best_search_init = new doublereal[n_a[0] + 1];
	if (x_best_search_init == nullptr) {
		// ������������ ������ �� ������ ������������.
		std::cout << "Problem: not enough memory on your equipment for x_best_search_init my_agregat_amg.cpp..." << std::endl;
		std::cout << "Please any key to exit..." << std::endl;
		exit(1);
	}
#pragma omp parallel for
	for (integer i47 = 1; i47 <= n_a[0]; i47++) {
		x_best_search_init[i47] = x[i47];
		x_best_search2[i47] = x[i47];
	}

	dbgmres_smoother = new DATA_BASE_GMRES[maxlevel];
	for (int i_52 = 0; i_52 < maxlevel; i_52++) {
		dbgmres_smoother[i_52].val = nullptr;
		dbgmres_smoother[i_52].col_ind = nullptr;
		dbgmres_smoother[i_52].row_ptr = nullptr;
	}

	integer istop_speed_cycling = 10;

	if ((my_amg_manager.istabilization == 0) || ((iVar == TEMP) && (my_amg_manager.istabilization == 3))) {


		// ((iVar==TEMP)&&(my_amg_manager.istabilization == 3)) - ���������� ��������� ������� � ��������� �������������.

		// ������ �������������� ������������� �����.

		if ((iVar == VELOCITY_X_COMPONENT) || (iVar == VELOCITY_Y_COMPONENT) || (iVar == VELOCITY_Z_COMPONENT)) tolerance *= 1e-11;
		if ((iVar == NUSHA) || (iVar == TURBULENT_KINETIK_ENERGY) ||
			(iVar == TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA) ||
			(iVar == TURBULENT_KINETIK_ENERGY_STD_K_EPS) ||
			(iVar == TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS)) tolerance *= 1e-11;
		if (iVar == PAM) tolerance *= 1e-14;
		if (iVar == TEMP) tolerance *= 1e-6;
		if (iVar == TOTALDEFORMATIONVAR) tolerance = 1.0e-17;
		doublereal minx_gl = 1.0e40;
		doublereal maxx_gl = -1.0e40;
		//for (integer iprohod = 0; iprohod < 20; iprohod++) {
		//while ((iflag_cont == 1) && ((dres>tolerance) || ((iVar != TEMP) && (icount_V_cycle<5)))) {
		///	while ((iflag_cont == 1) && ((dres>tolerance) )) {
		while (((iflag_cont == 1) && ((dres > tolerance))) ||
			((iVar == TEMP) && bSIMPLErun_now_for_temperature && (icount_V_cycle < 9))
			|| ((iVar == TOTALDEFORMATIONVAR) && (icount_V_cycle < 9))) {

			// ������������ ����������� �������������� ��� ������ ����������.

			if (bSIMPLErun_now_for_temperature) {
				// �������������.

				//  ���� ��� ���������, ���� �����������.
				if (icount_V_cycle > istop_speed_cycling) {



					if ((iVar == VELOCITY_X_COMPONENT) || (iVar == VELOCITY_Y_COMPONENT) || (iVar == VELOCITY_Z_COMPONENT) || (iVar == NUSHA) ||
						(iVar == TURBULENT_KINETIK_ENERGY) || (iVar == TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA) ||
						(iVar == TURBULENT_KINETIK_ENERGY_STD_K_EPS) || (iVar == TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS))
					{
						if (dres < 1.0e-3 * dres_initial) {
							break;
						}
						else {
							istop_speed_cycling += 2;
						}
					}
					if (iVar == PAM) {
						if (dres < 1.0e-1 * dres_initial) {
							//break;
						}
						else {
							istop_speed_cycling += 2;
						}
					}
				}
			}




			if (fabs(dres / rho) < 1.0) {
#pragma omp parallel for
				for (integer i47 = 1; i47 <= n_a[0]; i47++) {
					x_best_search2[i47] = x[i47];
				}
			}

			if (bPhysics_stop == true) {
				if (icount_V_cycle > 0) {
					doublerealT maxnew = -1.0e30;
					for (integer i = 1; i <= n_a[0]; i++) {
						if (x[i] > maxnew) maxnew = x[i];
					}
					if (iVar == TOTALDEFORMATIONVAR) {
						if ((fabs(dres) < 1.0e-2) && (fabs(maxnew - maxold) < 1.0e-9)) {
							std::cout <<  "break bPhysics_stop, dres<1e-2 && (fabs(maxnew - maxold) < 1.0e-9)" << std::endl;
							break;
						}
						else {
							maxold = maxnew;
						}
					}
					else {
						if ((fabs(dres) < 1.0e-2) && (fabs(maxnew - maxold) < 0.0005)) {
							std::cout <<  "break bPhysics_stop, dres<1e-2 && (fabs(maxnew - maxold) < 0.0005)" << std::endl;
							break;
						}
						else {
							maxold = maxnew;
						}
					}

				}
			}


			if (icount_V_cycle > 0) {
				// ���������� 0 � ������ ������ �� ���������� ���������� ��������-�������� ������ ���������.
				// ������� ������� ��������� �������� 1 �������� ���������� ����� �������� ��� ����������
				// ������������ � ������� � 26 �� 4. ��� ��������� 1 � ������ ����� ���� ���� � ������
				// mysolver_v0_03 ���������� fHORF=1.0; 
				if ((iVar == TEMP) && (my_amg_manager.istabilization == 3)) {
					if (bonly_solid_calculation == true) {
						if (bvacuumPrism) {
							// �������������� ���������� ������� ���������� ������� � x
							// � rthdsd.

							doublereal* x_temper = nullptr;
							//x_temper = new doublerealT[n_a[0] + 1];
							x_temper = (doublereal*)malloc(((integer)(n_a[0]) + 1) * sizeof(doublereal));
							if (x_temper == nullptr) {
								// ������������ ������ �� ������ ������������.
								std::cout <<  "Problem: not enough memory on your equipment for x_temper my_agregat_amg.cpp..." << std::endl;
								std::cout <<  "Please any key to exit..." << std::endl;
								exit(1);
							}
#pragma omp parallel for
							for (integer i23 = 0; i23 < n_a[0]; i23++) {
								if (x[i23 + 1] < -272.15) x[i23 + 1] = -272.15;
								//x_temper[i23] = x[i23 + 1];
								// 0.01 �������� ������ ����������.
								// 0.25; 0.2; 0.01.
								// 0.005
								// etalon 0.01 (1250it; 891it; 2it; 7s 770ms)
								// experimental 0.02 (1250it; 28it; 2it; 5s 120ms)
								// experimental 0.04 (740it; 2it; 3s 360ms)
								// experimental 0.05 (618it; 2it; 3s 50ms)
								// experimental 0.06 (533it; 2it; 2s 900ms)
								// experimental 0.07 (469it; 2it; 2s 800ms)
								// experimental 0.08 (420it; 2it; 2s 570ms)
								// experimental 0.09 (381 it; 2it; 2s 390ms)
								// experimental 0.1 (1250it � �� ��������, ����������).
								if (fabs(x[i23 + 1] - x_old[i23 + 1]) > 5.0) {
									// ����� 5� �������.
									// 0.04 14s 440ms 358it
									// 0.09 13s 290ms 314it optimum
									x_temper[i23] = x_old[i23 + 1] + 0.084 * (x[i23 + 1] - x_old[i23 + 1]);
								}
								else if (fabs(x[i23 + 1] - x_old[i23 + 1]) > 1.0) {
									// ����� 1� �������.
									// 0.09 13s 290ms 314it
									// 0.095 17s 10ms 318; 102; 22;
									// 0.091 316it; 10; 10; 13s 350ms;
									// 0.092 optimum
									x_temper[i23] = x_old[i23 + 1] + 0.092 * (x[i23 + 1] - x_old[i23 + 1]);
								}
								else {
									// 0.09 0.25 �������.
									// 0,09; 0.11; 314it; 9it; 9it; 2s 310ms optimum
									// 0.09; 0.12; 300it; 33it; 20it; 2s 530ms; 
									// 0.09; 0.1; 308it; 35it; 2s 400ms;
									// 0.11 optimum
									x_temper[i23] = x_old[i23 + 1] + 0.11 * (x[i23 + 1] - x_old[i23 + 1]);
								}
								if (x_temper[i23] < -272.15) x_temper[i23] = -272.15;
								x[i23 + 1] = x_temper[i23];
							}

							// �� ������ �� ��������� ������� ��������� ��� ������� �������� �������.
							// ���������� ���������� ���������� � � �� �������� ��������� �����������:
							for (integer i23 = 0; i23 < lb; i23++) {
								update_avg_temperatures(x_temper, my_body[i23]);
							}
							// ���������� ���������� ������������ �������� �������:
							for (integer i23 = 0; i23 < lb; i23++) {
								calculation_density_radiation_heat_flux(my_body[i23]);
							}


							doublereal* rthdsd_loc123 = nullptr;
							//rthdsd_loc123 = new doublerealT[n_a[0] + 1];
							rthdsd_loc123 = (doublereal*)malloc(((integer)(n_a[0]) + 1) * sizeof(doublereal));
							if (rthdsd_loc123 == nullptr) {
								// ������������ ������ �� ������ ������������.
								std::cout <<  "Problem: not enough memory on your equipment for rthdsd_loc123 my_agregat_amg.cpp..." << std::endl;
								std::cout <<  "Please any key to exit..." << std::endl;
								exit(1);
							}

							for (integer i23 = 0; i23 < n_a[0]; i23++) {
								rthdsd_loc123[i23] = rthdsd_no_radiosity_patch[i23];
								if ((i23 >= iadd_qnbc_maxelm) &&
									(qnbc[i23 - iadd_qnbc_maxelm].bactive) &&
									(qnbc[i23 - iadd_qnbc_maxelm].bStefanBolcman_q_on)) {
									// 0.25 13s 60ms 314; 10; 13; old value
									// 0.26 13s 0ms; 310; 21; 13;
									// 0.27 318; 13; 3;
									// 0.28 325; 42; 13;
									// 0.35 13s 560ms 347; //new optimum
									// 0.09 1250; 1250; ����� ����������� �������.
									doublerealT alpha_relax142 = 0.35;// d_my_optimetric1_6_12_2019;// 0.35;
									rthdsd_loc123[i23] = alpha_relax142 *
										(-qnbc[i23 - iadd_qnbc_maxelm].emissivity * STEFAN_BOLCMAN_CONST *
											((273.15 + x_temper[i23]) * (273.15 + x_temper[i23]) *
												(273.15 + x_temper[i23]) * (273.15 + x_temper[i23]) -
												(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb) *
												(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb) *
												(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb) *
												(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb))) +
										(1.0 - alpha_relax142) *
										(-qnbc[i23 - iadd_qnbc_maxelm].emissivity * STEFAN_BOLCMAN_CONST *
											((273.15 + x_old[i23 + 1]) * (273.15 + x_old[i23 + 1]) *
												(273.15 + x_old[i23 + 1]) * (273.15 + x_old[i23 + 1]) -
												(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb) *
												(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb) *
												(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb) *
												(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)));
									rthdsd_loc123[i23] *= qnbc[i23 - iadd_qnbc_maxelm].dS;
								}
							}

							radiosity_patch_for_vacuum_Prism_Object_(rthdsd_loc123, my_body, lb, maxelm_out);
#pragma omp parallel for
							for (integer i23 = 0; i23 < n_a[0]; i23++) {
								x_old[i23 + 1] = x_temper[i23];
								//x_old[i23 + 1] = x[i23 + 1];
							}
#pragma omp parallel for
							for (integer i23 = 0; i23 < n_a[0]; i23++) {
								b[i23 + 1] = rthdsd_loc123[i23];
							}

							if (rthdsd_loc123 != nullptr) {
								free(rthdsd_loc123);
							}
							rthdsd_loc123 = nullptr;

							if (x_temper != nullptr) {
								free(x_temper);
							}
							x_temper = nullptr;
						}
						else if (b_sign_on_nonlinear_bc) {
							//  25 ������� 2015. ��������� ���������� ��� ������������� 
							// ���������� ��������� �������.


							bool bNewtonRichman = false;
							bool bStefanBolcman = false;

							for (integer i23 = 0; i23 < n_a[0]; i23++) {
								if (i23 >= iadd_qnbc_maxelm) {
									if ((qnbc[i23 - iadd_qnbc_maxelm].bactive) &&
										(qnbc[i23 - iadd_qnbc_maxelm].bStefanBolcman_q_on) &&
										(qnbc[i23 - iadd_qnbc_maxelm].bNewtonRichman_q_on == false)) {
										// ������-��������.
										bStefanBolcman = true;
									}
									if ((qnbc[i23 - iadd_qnbc_maxelm].bactive) &&
										(qnbc[i23 - iadd_qnbc_maxelm].bNewtonRichman_q_on) &&
										(qnbc[i23 - iadd_qnbc_maxelm].bStefanBolcman_q_on == false)) {

										// ������-������.
										bNewtonRichman = true;
									}
									if ((qnbc[i23 - iadd_qnbc_maxelm].bactive) &&
										(qnbc[i23 - iadd_qnbc_maxelm].bNewtonRichman_q_on) &&
										(qnbc[i23 - iadd_qnbc_maxelm].bStefanBolcman_q_on)) {
										// ������� ���������� ����.
										bStefanBolcman = true;
										bNewtonRichman = true;
									}
								}

							}


							doublerealT* x_temper = nullptr;
							//x_temper = new doublerealT[n_a[0] + 1];
							x_temper = (doublerealT*)malloc(((integer)(n_a[0]) + 1) * sizeof(doublerealT));
							if (x_temper == nullptr) {
								// ������������ ������ �� ������ ������������.
								std::cout <<  "Problem: not enough memory on your equipment for x_temper my_agregat_amg.cpp..." << std::endl;
								std::cout <<  "Please any key to exit..."<< std::endl;
								exit(1);
							}
#pragma omp parallel for
							for (integer i23 = 0; i23 < n_a[0]; i23++) {
								if (x[i23 + 1] < -272.15) x[i23 + 1] = -272.15;
								// x_temper[i23] = x[i23 + 1];
								// 0.01 �������� ������ ����������.
								// 0.25
								// 0.2
								// 10 ���� 2018 ���� ������� �� ����������� ������ ���������� ������ 0.9.
								// 0.01
								// 0.005

								doublereal alpha_relax_nonlinear_boundary_condition = 0.01;
								if (bNewtonRichman && (bStefanBolcman == false)) {
									alpha_relax_nonlinear_boundary_condition = 0.9;
								}

								x_temper[i23] = x_old[i23 + 1] + alpha_relax_nonlinear_boundary_condition * (x[i23 + 1] - x_old[i23 + 1]);
								if (x_temper[i23] < -272.15) x_temper[i23] = -272.15;
								x[i23 + 1] = x_temper[i23];
							}

							doublerealT* rthdsd_loc123 = nullptr;
							//rthdsd_loc123 = new doublerealT[n_a[0] + 1];
							rthdsd_loc123 = (doublerealT*)malloc(((integer)(n_a[0]) + 1) * sizeof(doublerealT));
							if (rthdsd_loc123 == nullptr) {
								// ������������ ������ �� ������ ������������.
								std::cout <<  "Problem: not enough memory on your equipment for rthdsd_loc123 my_agregat_amg.cpp..." << std::endl;
								std::cout <<  "Please any key to exit..." << std::endl;
								exit(1);
							}

							for (integer i23 = 0; i23 < n_a[0]; i23++) {
								rthdsd_loc123[i23] = rthdsd_no_radiosity_patch[i23];
								if ((i23 >= iadd_qnbc_maxelm) && (qnbc[i23 - iadd_qnbc_maxelm].bactive) && (qnbc[i23 - iadd_qnbc_maxelm].bStefanBolcman_q_on) && (qnbc[i23 - iadd_qnbc_maxelm].bNewtonRichman_q_on == false)) {
									// ������ ��������.
									doublerealT alpha_relax142 = 0.25;
									rthdsd_loc123[i23] = alpha_relax142 * (-qnbc[i23 - iadd_qnbc_maxelm].emissivity * STEFAN_BOLCMAN_CONST * ((273.15 + x_temper[i23]) * (273.15 + x_temper[i23]) * (273.15 + x_temper[i23]) * (273.15 + x_temper[i23]) - (273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb) * (273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb) * (273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb) * (273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb))) +
										(1.0 - alpha_relax142) * (-qnbc[i23 - iadd_qnbc_maxelm].emissivity * STEFAN_BOLCMAN_CONST * ((273.15 + x_old[i23 + 1]) * (273.15 + x_old[i23 + 1]) * (273.15 + x_old[i23 + 1]) * (273.15 + x_old[i23 + 1]) - (273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb) * (273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb) * (273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb) * (273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)));
									rthdsd_loc123[i23] *= qnbc[i23 - iadd_qnbc_maxelm].dS;
								}
								if ((i23 >= iadd_qnbc_maxelm) && (qnbc[i23 - iadd_qnbc_maxelm].bactive) && (qnbc[i23 - iadd_qnbc_maxelm].bNewtonRichman_q_on) && (qnbc[i23 - iadd_qnbc_maxelm].bStefanBolcman_q_on == false)) {
									// ������-������.
									//doublerealT alpha_relax142 = 0.25;
									rthdsd_loc123[i23] = -qnbc[i23 - iadd_qnbc_maxelm].film_coefficient * (x_temper[i23] - qnbc[i23 - iadd_qnbc_maxelm].Tamb);
									rthdsd_loc123[i23] *= qnbc[i23 - iadd_qnbc_maxelm].dS;
								}
								if ((i23 >= iadd_qnbc_maxelm) && (qnbc[i23 - iadd_qnbc_maxelm].bactive) && (qnbc[i23 - iadd_qnbc_maxelm].bNewtonRichman_q_on) && (qnbc[i23 - iadd_qnbc_maxelm].bStefanBolcman_q_on)) {
									// ������� ���������� ����.
									//doublerealT alpha_relax142 = 0.25;
									rthdsd_loc123[i23] = (-qnbc[i23 - iadd_qnbc_maxelm].emissivity * STEFAN_BOLCMAN_CONST * ((273.15 + x_temper[i23]) * (273.15 + x_temper[i23]) * (273.15 + x_temper[i23]) * (273.15 + x_temper[i23]) - (273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb) * (273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb) * (273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb) * (273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)));
									rthdsd_loc123[i23] += -qnbc[i23 - iadd_qnbc_maxelm].film_coefficient * (x_temper[i23] - qnbc[i23 - iadd_qnbc_maxelm].Tamb);
									rthdsd_loc123[i23] *= qnbc[i23 - iadd_qnbc_maxelm].dS;
								}
							}
#pragma omp parallel for
							for (integer i23 = 0; i23 < n_a[0]; i23++) {
								x_old[i23 + 1] = x_temper[i23];
								//x_old[i23 + 1] = x[i23 + 1];
							}
#pragma omp parallel for
							for (integer i23 = 0; i23 < n_a[0]; i23++) {
								b[i23 + 1] = rthdsd_loc123[i23];
							}

							if (rthdsd_loc123 != nullptr) {
								free(rthdsd_loc123);
							}
							rthdsd_loc123 = nullptr;

							if (x_temper != nullptr) {
								free(x_temper);
							}
							x_temper = nullptr;

						}
					}
				}
			}

			//getchar();
			icount_V_cycle++;
			//if (icount_V_cycle > 8) break;
			count_iter_for_film_coef++;
			// � ������ ������ ������� - �������, �������-��������� � ��������� ������� �� ��������� �� ����� ��������, 
			// �.�. ��� ��������� ������ ���������� �������. 13 ����� 2016.
			if (((adiabatic_vs_heat_transfer_coeff > ADIABATIC_WALL_BC) ||
				(breakRUMBAcalc_for_nonlinear_boundary_condition)) &&
				(count_iter_for_film_coef > 1250)) break;

			// 1 dec 2016.
			//  ���������� ����� 2 ��� 5 V ������ ����������� ���������� ����� �� ����� ����������.
			if (bvacuumPrism) {
				// 5
				// 250
				if (icount_V_cycle > 2250) break;
			}


			if ((iter_limit == 5000) || ((iVar == PAM) && (fabs(dres) > 7.0e3))) {
#pragma omp parallel for
				for (integer i47 = 1; i47 <= n_a[0]; i47++) {
					x[i47] = x_copy[i47];
				}
				if (iVar == PAM) {
					std::cout <<  "pressure amendment divergence..." << std::endl;
				}
				std::cout <<  "amg divergence detected dres="<< dres <<"..."<< std::endl;
				std::cout <<  "number V-cycle=" << icount_V_cycle << " initial residual dres0=" << dres_initial << std::endl;

				std::cout <<  "Operator A complexity=" <<dr_grid_complexity<<"  Opertator P complexity="<< (doublerealT)(nnz_P_memo_all / n_a[0]) <<"..."<<std::endl;
				std::cout <<  "res_best_search=" << res_best_search << std::endl;
				//getchar();
				// ����� ������ 22 12 2016
				//system("PAUSE");
				break;
			}

			if (iter_limit == 1) {
				// ��������� �������.
				res0start = fabs(dres);
			}

			// ������� �� �����������:
			// ��� ���������� ��� ���� ���������� � ����������������� �������� � ��������� �� ��������.
			//if (iVar == TEMP) std::cout << "temp res=" << fabs(dres) << std::endl;

			if (fabs(dres) < res_best_search)
			{
				// ���������� ������ �������.
				res_best_search = fabs(dres);
#pragma omp parallel for
				for (integer i47 = 1; i47 <= n_a[0]; i47++) {
					x_best_search[i47] = x[i47];
				}
			}
			/*
			if (iVar == PAM) {
			if (fabs(dres) < 1.0) {
			// ���� � ��� ��� ��� ����� ���� �����-�� �������� ��������,
			// �� ����� ��� ������������ ������� �������������.
			// ������� ��� ���� � ��� ����� 1.0 ��� ����������� ��� �� �� ������ �������.
			for (integer i47 = 1; i47 <= n_a[0]; i47++) {
			x_best_search[i47] = x[i47];
			}
			}
			}
			*/

			// debug 7 ���� 2016
			//if (iter_limit > 300) {
			//std::cout << "amg divergense detected...9 june 2016" << std::endl;
			//system("pause");
			//break;
			//}

			//if (dres < 1.0e-14) break;

			// 100
			if (iter_limit > 5000) { // Finned Heat Sink



				if (bfirst_divergence) {
					iter_limit = 3;
					nu1 += 2;
					nu2 += 2;
					nFinestSweeps += 2;
					bfirst_divergence = false;
				}
				else {
					if ((fabs(res_best_search / res0start) < 0.23) && (fabs(res_best_search) < 1.0e-3 * sqrt((doublereal)(n_a[0])))) {
						// ���� ������� ������ �������������� �� ��� �������.
						// �������� ����������� � ����� � ���������.
#pragma omp parallel for
						for (integer i47 = 1; i47 <= n_a[0]; i47++) {
							x[i47] = x_best_search[i47];
						}
						break;
					}
					else if ((fabs(res_best_search / res0start) <= 1.0) && (fabs(res_best_search) < 1.0e-4 * sqrt((doublereal)(n_a[0])))) {
						// ���� ������� ������ �������������� �� ��� �������.
						// �������� ����������� � ����� � ���������.
#pragma omp parallel for
						for (integer i47 = 1; i47 <= n_a[0]; i47++) {
							x[i47] = x_best_search[i47];
						}
						break;
					}
					// �������� ����������� � ����� � ���������.
#pragma omp parallel for
					for (integer i47 = 1; i47 <= n_a[0]; i47++) {
						x[i47] = x_best_search[i47];
					}
					break;
					// ��������������� 11.01.2020
					// ��� ������������ ���.
					// ��� ����� ���� ������ ������� �� ���������.
					/*
					std::cout << "Fatal amg error: Strong divergence amg solver..."<<  fabs(res_best_search / res0start) << std::endl;
					std::cout << "res_best_search=" << fabs(res_best_search)<<", res0start=" <<  fabs(res0start) << std::endl;
					std::cout << "BiCGStab+ILU2 is start now..." << std::endl;
					std::cout << "please wait...";
					system("pause");
					break; // ��������� ����� �� while �����.
					*/
				}
			}
			iter_limit++;

			if (fabs(dres) < fabs(dres_previos)) {
				// ��� ��������� ������� ��������.
				icount_bad_convergence_Vcycles = 0;
			}
			else {
				icount_bad_convergence_Vcycles++;
			}

			//if (_finite(dres) == 0) {
			//if (fabs(dres) > 1.0e30)
			//{
			//std::cout << "\ndivergence AMG detected...solver will be restart... please wait... ... ...\n";
			//std::cout << "\a\a\a\a\a\a\a\a";
			//system("pause");
			//exit(1);
			//return true;
			//for (integer i47 = 1; i47 <= n_a[0]; i47++) {
			//	x[i47] = x_copy[i47];
			//}
			//if (iter_limit > 100) {
			//	ret_value = true;
			//	break;
			//}
			//else {
			// ���������� ���������� ������������ �������� �� ���� ������� �� 
			// ���������� ���� ������������. 
			//	nu1++;
			//	nu2++;
			//	nFinestSweeps++;
			// �� �������� ���� ����������� ����� ������ ���������� ��������� ������� ���������� omega optimal.
			// ��������� omega optimal ������ ���� ��������������� (�������������� � ������).
			//}
			//}

			// 24 10 2016
			if (icount_bad_convergence_Vcycles > 40) break;

			if ((icount_bad_convergence_Vcycles >= istop_porog_reconst) || (fabs(dres) / sqrt((doublereal)(n_a[0])) > 1.0e30)) {
				// ������������� 10 ����� ������������ ������ ��-�������� ����� ����������.
				// ����� � ������������ ������� ������� ������� 1.0e30.

				//if (fabs(dres) < 1.0e-3) break; // ����� ������� ���������� ���������� �������.
				if ((fabs((doublereal)(res_best_search / res0start)) < 1.0e-1) && (fabs(dres) / sqrt((doublereal)(n_a[0])) < 1.0e-3)) {
					// ���� ������� ������ �������������� �� ��� �������.
					// �������� ����������� � ����� � ���������.
#pragma omp parallel for
					for (integer i47 = 1; i47 <= n_a[0]; i47++) {
						x[i47] = x_best_search[i47];
					}
					std::cout <<  "stagnaion break out" << std::endl;
					break;
				}
				i_count_stagnation++;

				std::cout << "\ndivergence AMG detected...solver will be restart... please wait... ... ..." << std::endl;
				//std::cout << "\a\a\a\a\a\a\a\a";
				//system("pause");
				//exit(1);
				//return true;
				if (i_count_stagnation < 20) {
#pragma omp parallel for
					for (integer i47 = 1; i47 <= n_a[0]; i47++) {
						//x[i47] = x_copy[i47];
						x[i47] = x_best_search[i47]; // ������ ��������� �������
					}
				}
				if (i_count_stagnation == 20 || i_count_stagnation == 21) gold_const = 0.2;
				if ((i_count_stagnation >= 20) && (i_count_stagnation < 30)) {
#pragma omp parallel for
					for (integer i47 = 1; i47 <= n_a[0]; i47++) {
						//x[i47] = x_copy[i47];
						// ����� ��� ����������� ������� �������� nu1 � nu2.
						doublerealT signumnow = 1.0;
						if (rand() % 2 == 0) signumnow = -1.0;
						doublereal drand = (doublereal)(((double)(rand()))/((double)(RAND_MAX+1)));
						x[i47] = signumnow * 1.0 * drand; // ��������� ����� � ��������� �� 0 �� 1.
					}
				}
				if (i_count_stagnation == 30 || i_count_stagnation == 31) gold_const = 0.2;
				if (i_count_stagnation >= 30) {
#pragma omp parallel for
					for (integer i47 = 1; i47 <= n_a[0]; i47++) {
						x[i47] = 1.0;
					}
				}
				if (bproblem_amg_convergence1) {
					if (bproblem_amg_convergence2) {
						if (bproblem_amg_convergence3) {
							// ����� � ������ BiCGStab+ILU2.
							ret_value = true;
							break;
						}
						else {
							// ����� omega.
							bproblem_amg_convergence3 = true;
							icount_bad_convergence_Vcycles = 0;
							buffers3omega = dres / dres_previos;
							std::cout << "buffers1omega="<< buffers1omega <<", buffers2omega="<< buffers2omega <<", buffers3omega="<< buffers3omega <<std::endl; 
						}
					}
					else {
						// ����� omega.
						bproblem_amg_convergence2 = true;
						icount_bad_convergence_Vcycles = 0;
						buffers2omega = dres / dres_previos;
						std::cout << "buffers1omega=" << buffers1omega <<", buffers2omega="<< buffers2omega <<std::endl; 
						//istop_porog_reconst += 50; // 10, 20, 30, 40
						// ���������� ���������� ������������ �������� ������ �� ���.
						//nu1++;
						//nu2++;
						//nFinestSweeps++;
					}
				}
				else {

					bproblem_amg_convergence1 = true; // ������� � SOR �� ���������� �������.
					icount_bad_convergence_Vcycles = 0;
					buffers1omega = dres / dres_previos;
				}
			}


			dres_previos = dres;


			doublerealT R0_0 = 0.0;
			doublerealT Rprev_0 = 0.0, Rnext_0 = 0.0;
			if (process_flow_logic) {
				// calculate initial residual.
				//residualq<doublereal>(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0]);
				residualq2(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0], diag[0], diag_minus_one[0]);
				R0_0 = norma(residual_fine[0], n_a[0]);
				Rprev_0 = R0_0;

				// smother
				integer iter = 0;
				for (iter = 0; iter < nu1; iter++) {
					//quick seidel
					if (bonly_serial) {
						seidelq<doublereal>(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0);
					}
					else {
						seidelq<doublereal>(Amat, 1, n_a[0], x, b, nested_desection[0], row_ptr_start, row_ptr_end, 0);
					}
					//residualq(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0]);
					residualq2(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0], diag[0], diag_minus_one[0]);
					Rnext_0 = norma(residual_fine[0], n_a[0]);
					// this is process flow logic
					if (Rnext_0 > process_flow_beta * Rprev_0) {
						// ����� ����������� � ��� ��� �� �������� �������� �� ����������.
						break; // �������� ���������� �� ��������� ������� ���� �� ���� �������.
					}
					else {
						Rprev_0 = Rnext_0;
					}
				}
				if (iter == nu1) {
					std::cout << "level 0 limit presmother iteration is reached" << std::endl;
				}

			}
			else {
				// smoother
				for (integer iter = 0; iter < nu1; iter++) {
					//seidel(Amat, 1, nnz_a[0], x, b, flag, n_a[0]);
					//quick seidel
					if (bonly_serial) {
						if (bILU2smoother == 2) {
							seidelq<doublereal>(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, F_false_C_true, 0, 0);
							residualq2(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0], diag[0], diag_minus_one[0]);
#pragma omp parallel for
							for (integer i43 = 0; i43 < n_a[0]; i43++) {
								milu2[0].zbuf[i43 + 1] = residual_fine[0][i43 + 1];
							}
							lusol_1patchforRUMBA(n_a[0], milu2[0].zbuf, milu2[0].zbuf2, milu2[0]);
#pragma omp parallel for
							for (integer i43 = 0; i43 < n_a[0]; i43++) {
								x[i43 + 1] += milu2[0].zbuf2[i43 + 1];
							}
						}
						else {
							seidelq<doublereal>(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, F_false_C_true, 0, 0);
						}
					}
					else {
						seidelq<doublereal>(Amat, 1, n_a[0], x, b, nested_desection[0], row_ptr_start, row_ptr_end, 0, F_false_C_true, 0);
					}
				}
			}

			//exporttecplot(x, n);

			move_down(nu1, nu2);

			if (!process_flow_logic) {
				// residual_r
				//doublerealT *residual_fine[0] = new doublerealT[n_a[0] + 1];
				//residual(Amat, 1, nnz_a[0], x, b, flag, n_a[0], residual_fine[0]);
				//residualq(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0]);
				residualq2(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0], diag[0], diag_minus_one[0]);
			}
			dres = norma(residual_fine[0], n_a[0]);
			ret74 += fabs(dres);
			if (bprint_mesage_diagnostic) {


				doublereal minx = 1.0e30;
				doublereal maxx = -1.0e30;
				for (integer i_83 = 1; i_83 <= n_a[0]; i_83++) {
					if (x[i_83] < minx) minx = x[i_83];
					if (x[i_83] > maxx) maxx = x[i_83];
				}

				if (((iVar == TEMP) && (my_amg_manager.istabilization == 3))) {
					//  ���������� ���������� - ��������� ����� �� ������� ���������� ������.
					if ((fabs(minx - minx_gl) < 2.0e-3) && (fabs(maxx - maxx_gl) < 2.0e-3)) {
						std::cout <<  "Solution nonlinear problem converged succsefull. Ok..." << std::endl;
						break;
					}
				}

				minx_gl = minx;
				maxx_gl = maxx;

				if (iiter < 10) {
					std::cout << iiter << "   " << dres << " rho=" << dres / rho << " min=" << minx << " max=" << maxx << "\n";
				}
				else if (iiter < 100) {
					std::cout << iiter << "  " << dres << " rho=" << dres / rho << " min=" << minx << " max=" << maxx << "\n";
				}
				else {
					std::cout << iiter << " " << dres << " rho=" << dres / rho << " min=" << minx << " max=" << maxx << "\n";
				}

				if (!((iVar == TEMP) && (my_amg_manager.istabilization == 3))) {
					if (fabs(1.0 - fabs(dres / rho)) < 1.0e-3) {
						std::cout <<  "stagnation in amg solver determinate ..." << std::endl;
						// ������� �� ��������. ���������� ������ ���������� �������� ��������� �����������.
						// ���������� divergence detected � ������������� ������� ������� ���������� 
						// ������� ������������ ��������� ����������� ���� � ����� ������� ������������.
						printf("Recomended: Set up an external Krylov-type\n");
						printf("iterative process BiCGStab or FGMRes.\n");
						return true;
						// 28_10_2016.
						// ������������ ��������� ����� �� ������������, 
						// �.�. ������� ��������� ��������.
						//break;
					}
				}
				if (icount_V_cycle == 1) {
					if (fabs(dres / rho) < 1.0) {
#pragma omp parallel for
						for (integer i47 = 1; i47 <= n_a[0]; i47++) {
							x_best_search[i47] = x[i47];
						}
					}
				}
			}
			iiter++;
			// 28.07.2016

			if (fabs(dres) > 1.0e9) {

				std::cout <<  "amg solver divergence detected." << std::endl;
				system("pause");

#pragma omp parallel for
				for (integer i47 = 1; i47 <= n_a[0]; i47++) {
					///x[i47] = x_best_search[i47];
					//x_copy[i47] = x[i47]; // 4 ������ 2016.
					x[i47] = x_copy[i47];
				}
				residualq2_analysys(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0], diag[0], diag_minus_one[0]);

				std::cout <<  "dres_initial="<< dres_initial <<" res_best_search="<< res_best_search <<" dres="<< dres <<" current="<< norma(residual_fine[0], n_a[0]) <<std::endl; 
				std::cout << "break. amg divergence detected. fabs(dres) > 1.0e7" << std::endl;
				//getchar();
				if ((bILU2smoother == 2) || (bILU2smoother == 0)) {
					std::cout <<  "apply ilu" <<  my_amg_manager.lfil <<" smoother for number 0 level" << std::endl;
					equation3DtoCRSRUMBA1(milu2[0], true, Amat, 1, n_a[0], row_ptr_start, row_ptr_end, 0, 0);

				}
				if (bILU2smoother == 0) {
					// ������������.
					memory_allocation_apostoriory_buffer_ilu(milu2, ilevel - 1);
					bILU2smoother = 2;
				}

				// ��� �� ��������� ��� �������� ��������.
				doublerealT dresfinish_probably = 0.1 * norma(residual_fine[0], n_a[0]);
				if ((iVar == VELOCITY_X_COMPONENT) || (iVar == VELOCITY_Y_COMPONENT) || (iVar == VELOCITY_Z_COMPONENT) ||
					(iVar == NUSHA) || (iVar == TURBULENT_KINETIK_ENERGY) ||
					(iVar == TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA) ||
					(iVar == TURBULENT_KINETIK_ENERGY_STD_K_EPS) ||
					(iVar == TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS)) {
					// ��� �� ��������� ��� ��������� �������� ������ SIMPLE ���������.
					dresfinish_probably = 1.0e-3 * norma(residual_fine[0], n_a[0]);
				}
				if (bSIMPLErun_now_for_temperature == true) {
					if (iVar == TEMP) {
						// ��� ���� ���������� ��� ����������������� �������.
						// � BiCGStab Internal 3 ����������� �� 1e-10.
						dresfinish_probably = 1.0e-3 * norma(residual_fine[0], n_a[0]);
					}
				}
				if (iVar == TOTALDEFORMATIONVAR) {
					// ��� ������������ ����������
					dresfinish_probably = 1.0e-3 * norma(residual_fine[0], n_a[0]);
				}
				if (bonly_solid_calculation) {
					dresfinish_probably = 1.0e-5 * norma(residual_fine[0], n_a[0]);
				}
				integer i943 = 0;
				for (integer i_prob_detect_i = 0; i_prob_detect_i < 1000; i_prob_detect_i++) {
					i943 = i_prob_detect_i;
					seidelq<doublereal>(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, F_false_C_true, 0, 0);
					residualq2(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0], diag[0], diag_minus_one[0]);

					doublereal minx = 1.0e30;
					doublereal maxx = -1.0e30;
					for (integer i_83 = 1; i_83 <= n_a[0]; i_83++) {
						if (x[i_83] < minx) minx = x[i_83];
						if (x[i_83] > maxx) maxx = x[i_83];
					}

					std::cout <<  i_prob_detect_i << " residual=" << norma(residual_fine[0], n_a[0]) << " min=" << minx << " max=" << maxx << " \n";

					// ��������� ����� �� �����.
					if (norma(residual_fine[0], n_a[0]) < dresfinish_probably) {
						std::cout << "Ok!!! calculation local compleate... " << std::endl;
						break;
					}
#pragma omp parallel for
					for (integer i43 = 0; i43 < n_a[0]; i43++) {
						milu2[0].zbuf[i43 + 1] = residual_fine[0][i43 + 1];
					}
					lusol_1patchforRUMBA(n_a[0], milu2[0].zbuf, milu2[0].zbuf2, milu2[0]);
#pragma omp parallel for
					for (integer i43 = 0; i43 < n_a[0]; i43++) {
						x[i43 + 1] += milu2[0].zbuf2[i43 + 1];
					}
				}

				// ����������� ��������� �������� �� �����������:
				if (norma(residual_fine[0], n_a[0]) >= dresfinish_probably) {
					std::cout << "Fatal error !!! ilu2 divergence detected... "<< std::endl;
					std::cout << "residual curent=" << norma(residual_fine[0], n_a[0]) <<" target residual="<< dresfinish_probably <<std::endl;
					if (i943 < 997) {
						break;
					}
				}
#pragma omp parallel for
				for (integer i47 = 1; i47 <= n_a[0]; i47++) {
					x_best_search[i47] = x[i47];
					x_copy[i47] = x[i47]; // 4 ������ 2016.
				}
				system("PAUSE");

				goto FULL_DIVERGENCE_DETECTED;
				//break;
			}

			if (iVar == PAM) {
				if ((fabs(dres / rho) > 0.99999) || (fabs(dres) > 1.0e7)) {
					// ����� �� ����������� ���� ���������� 20 ������ ������������.
					delta_old_iter = fabs(dres);
					i_signal_break_pam_opening++;
					if (i_signal_break_pam_opening > i_limit_signal_pam_break_opening) {

						std::cout <<  "iter = " << iiter << std::endl;

						// �������� ����������� � ����� � ���������.
#pragma omp parallel for
						for (integer i47 = 1; i47 <= n_a[0]; i47++) {
							x[i47] = x_best_search[i47];
						}
						break;
					}
				}
			}


			//rho=norma(residual_fine[0], n_a[0]);
			rho = dres;
			// start 08.01.2018
			V_cycle_solve<doublerealT>(Amat, x, b, process_flow_logic, row_ptr_start,
				row_ptr_end, residual_fine, diag, diag_minus_one, n_a, bonly_serial,
				process_flow_beta, F_false_C_true, nu1, nu2, bILU2smoother,
				ilevel, 1, imyinit, maxlevel, milu2, milu0, nested_desection,
				P, nnz_aRP, residual_coarse, igam, nnz_a,
				error_approx_coarse, dapply_ilu_max_pattern_size,
				process_flow_alpha,
				error_approx_fine, nFinestSweeps);
			// end 08.01.2018

			//if (bfirst_start_nonlinear_process) {
			// �� ��������� ������������ �� ���������� ������� � ������� 
			// ��������� ����������.
			//bfirst_start_nonlinear_process = false;
			//break;
			//}
			if (iVar != PAM) {
				if (btheoryGuideANSYSFluent) break; // ������ ���� ���� V  ����.
			}
			//system("pause");
		}
	} // bBiCGStab_plus_RUMBA_camg if (my_amg_manager.istabilization == 1)
	else if (my_amg_manager.istabilization == 1) {
		// ������������� ������������ ��������� ��������: ������� ��� BiCGStab � ��������� ��� ������������������ � ������� V - �����.
		// �������������� ������������� ����� ��� �������������������
		// � ��������� ����������� ���� ����� ��� ��� ������ BiCGStab
		// �� �������������.
		// ������� ��� ���� ������ ��� ������� � �� ����� ��������� ������.
		// 5.01.2017 �������� BiCGStab �������� � 1992 ����.

		integer inumberVcyclelocbicgstab = 1;

		// ��������� �������� ���������� � ����.
		integer n75 = n_a[0]; // ����� ����������� �� ��������� ������.
		doublereal* val75 = nullptr;
		val75 = new doublereal[nnz_a[0]];
		integer* col_ind75 = nullptr;
		col_ind75 = new integer[nnz_a[0]];
		integer* row_ptr75 = nullptr;
		row_ptr75 = new integer[n_a[0] + 1];
		if ((val75 == nullptr) || (col_ind75 == nullptr) || (row_ptr75 == nullptr)) {
			// ������������ ������ �� ������ ������������.
			std::cout << "Problem: not enough memory on your equipment for val, col_ind or row_ptr: bicgStab + camg..." <<std::endl;
			std::cout <<  "Please any key to exit..." <<std::endl;
			exit(1);
		}

		// ������������� �������.
#pragma omp parallel for
		for (integer i_1 = 1; i_1 <= n_a[0]; i_1++) {

			for (integer i_2 = row_ptr_start[i_1]; i_2 <= row_ptr_end[i_1]; i_2++) {
				//if (Amat.i[i_2] == Amat.j[i_2]) {
					//if (i_1 != Amat.i[i_2]) {
						//std::cout << "err i!=i" << std::endl;
						//system("PAUSE");
					//}
				if (i_1 == Amat.j[i_2]) {
					val75[i_2 - 1] = diag[0][i_1];
					col_ind75[i_2 - 1] = i_1 - 1;
				}
				else {
					val75[i_2 - 1] = Amat.aij[i_2];
					col_ind75[i_2 - 1] = Amat.j[i_2] - 1;
				}
			}
			row_ptr75[i_1 - 1] = row_ptr_start[i_1] - 1;
		}

		//std::cout << "nnz=" << nnz_a[0] << " rpe="<< row_ptr_end[n_a[0]] <<" rps=" << row_ptr_start[n_a[0]+1]-1<< std::endl;

		//system("PAUSE");
		row_ptr75[n_a[0]] = row_ptr_end[n_a[0]];
		// ������� ����������� ��� ������ BiCGStab.
		doublereal* ri75 = nullptr;
		doublereal* roc75 = nullptr;
		doublereal* s75 = nullptr;
		doublereal* t75 = nullptr;
		doublereal* vec75 = nullptr;
		doublereal* vi75 = nullptr;
		doublereal* pi75 = nullptr;
		doublereal* dx75 = nullptr;
		doublereal* dax75 = nullptr;
		doublereal* y75 = nullptr;
		doublereal* z75 = nullptr;
		// ������ ������������������:
		doublereal* y76 = nullptr;
		doublereal* pi76 = nullptr;
		y76 = new doublereal[n75 + 1];
		pi76 = new doublereal[n75 + 1];
		// ������ ������������������:
		doublereal* z76 = nullptr;
		doublereal* s76 = nullptr;
		z76 = new doublereal[n75 + 1];
		s76 = new doublereal[n75 + 1];

		ri75 = new doublereal[n75];
		roc75 = new doublereal[n75];
		s75 = new doublereal[n75];
		t75 = new doublereal[n75];
		vec75 = new doublereal[n75];
		vi75 = new doublereal[n75];
		pi75 = new doublereal[n75];
		dx75 = new doublereal[n75];
		dax75 = new doublereal[n75];
		y75 = new doublereal[n75];
		z75 = new doublereal[n75];
		if ((ri75 == nullptr) || (roc75 == nullptr) || (s75 == nullptr) || (t75 == nullptr) || (vi75 == nullptr) || (pi75 == nullptr) || (dx75 == nullptr) || (dax75 == nullptr) || (y75 == nullptr) || (z75 == nullptr)) {
			// ������������ ������ �� ������ ������������.
			std::cout << "Problem: not enough memory on your equipment for: bicgStab + camg..."<< std::endl;
			std::cout << "Please any key to exit..." << std::endl;
			exit(1);
		}

		integer iflag75 = 1, icount75 = 0;
		doublereal delta075 = 1.0e30, deltai75 = 1.0e30;
		doublereal bet75 = 0.0, roi75 = 0.0;
		doublereal roim175 = 1.0, al75 = 1.0, wi75 = 1.0;

		doublereal epsilon75 = dterminatedTResudual;  // �������� ����������
		if (iVar == TEMP) {
			epsilon75 *= 1.0e-4; // 1.0e-4
		}
		if (iVar == TOTALDEFORMATIONVAR) {
			epsilon75 *= 1.0e-4; // 1.0e-4
								 //epsilon75 *= 1.0e-12;
		}
		integer i75 = 0;

		// initialize
#pragma omp parallel for
		for (i75 = 0; i75 < n75; i75++) {
			s75[i75] = 0.0;
			t75[i75] = 0.0;
			vi75[i75] = 0.0;
			pi75[i75] = 0.0;
			// ������������� �������� ��� ������������������
			y75[i75] = 0.0;
			z75[i75] = 0.0;
			// ��������� ��������� ������� �� ������.
			dax75[i75] = 0.0;
			// ��������� �����������.
			dx75[i75] = x[i75 + 1];
		}

		// ��������� ������� �� ������. ��������� �������� ���������� � ����.
		MatrixCRSByVector(val75, col_ind75, row_ptr75, dx75, dax75, n75); // ��������� ������ �  dax75

		// ���������� ri75 � roc75.
#pragma omp parallel for
		for (i75 = 0; i75 < n75; i75++) {
			ri75[i75] = b[i75 + 1] - dax75[i75];
			roc75[i75] = 1.0;
		}
		delta075 = NormaV(ri75, n75);


		// ���� ������� ����� ������� �� �� �������:
		if (iVar == TEMP) {
			if (fabs(delta075) < 1.0e-4 * dterminatedTResudual) iflag75 = 0;
		}
		else {
			if (fabs(delta075) < dterminatedTResudual) iflag75 = 0;
		}
		integer iflag175 = 1;
		if (iVar == TURBULENT_KINETIK_ENERGY) {
			if (fabs(delta075) < 1e-21) iflag175 = 0;
		}
		else {
			if (fabs(delta075) < 1e-14) iflag175 = 0;
		}
		if ((iVar == TEMP) && (iflag75 == 0) && (iflag175 == 0)) {
			std::cout << "bicgStab+camg: iflag="<< iflag75 <<", iflag1="<< iflag175 <<", delta0="<< delta075 <<std::endl; 
			system("PAUSE");
		}
		if ((iVar == NUSHA) || (iVar == TURBULENT_KINETIK_ENERGY) ||
			(iVar == TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA) ||
			(iVar == TURBULENT_KINETIK_ENERGY_STD_K_EPS) ||
			(iVar == TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS)) {
			std::cout << "Turbulence equations: bicgStab+camg: iflag=" << iflag75 << ", iflag1=" << iflag175 << ", delta0=" << delta075 <<std::endl; 
		}
		if ((iVar == VELOCITY_X_COMPONENT) || (iVar == VELOCITY_Y_COMPONENT) || (iVar == VELOCITY_Z_COMPONENT)) {
			std::cout << "Velocity equations: bicgStab+camg: iflag=" << iflag75 << ", iflag1=" << iflag175 << ", delta0=" << delta075 << std::endl; 
		}

		integer iN75 = 10;
		if (n75 < 30000) {
			// ������ ����� ����� ����������� !
			if ((iVar == VELOCITY_X_COMPONENT) || (iVar == VELOCITY_Y_COMPONENT) || (iVar == VELOCITY_Z_COMPONENT) ||
				(iVar == NUSHA) || (iVar == TURBULENT_KINETIK_ENERGY) ||
				(iVar == TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA) ||
				(iVar == TURBULENT_KINETIK_ENERGY_STD_K_EPS) ||
				(iVar == TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS)) {
				iN75 = 1; // ����������� ����� ���� �� ���� ��������.
						  // ���� ����� ����� ������������ �� �� �� ����� ����� ����������� �� ��� ��� ���� ������� �� ������ ������ epsilon.
				if (1.0e-3 * fabs(delta075) < epsilon75) {
					epsilon75 = 1.0e-3 * fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
			}
			if (iVar == TEMP) {
				iN75 = 2;
				epsilon75 = fmin(0.1 * fabs(delta075), epsilon75);
				if (bSIMPLErun_now_for_temperature == true) {
					//std::cout << "epsilon=" << epsilon << std::endl;
					//system("PAUSE");
					// ����������������� ������� ���������� ������������������� �� ����������� ��� ������������������ ��������.
					// ������� �������� ���� ������ ��������� �� 5 ��������.
					// 27.07.2016
					epsilon75 *= 1e-10;
					iN75 = 20;
					//epsilon75 *= 1e-16;
					//iN75 = 30;
				}
			}
			if (iVar == PAM) {
				iN75 = 12; // ������� ��� �������� �������� ������ ���� �������� �����.

				if (1.0e-10 * fabs(delta075) < epsilon75) {
					epsilon75 = 1.0e-10 * fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
				//std::cout << epsilon75 << std::endl; system("pause");
			}
		}
		else if ((n75 >= 30000) && (n75 < 100000)) {
			// ����� � ������� �������� ����� �������� � 
			// �������������� ������� ��������� ����� ������� 
			// ��������, �� ��� �� ��������.
			// ������� ������ � ��� ��� ������� �� ����������� ������-�� �� ��������.
			// ������ ��������� �����������.
			if ((iVar == VELOCITY_X_COMPONENT) || (iVar == VELOCITY_Y_COMPONENT) || (iVar == VELOCITY_Z_COMPONENT) ||
				(iVar == NUSHA) || (iVar == TURBULENT_KINETIK_ENERGY) ||
				(iVar == TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA) ||
				(iVar == TURBULENT_KINETIK_ENERGY_STD_K_EPS) ||
				(iVar == TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS)) {
				iN75 = 3; // ����������� ����� ���� �� ���� ��������.
						  // ���� ����� ����� ������������ �� �� �� ����� ����� ����������� �� ��� ��� ���� ������� �� ������ ������ epsilon.
				if (1.0e-3 * fabs(delta075) < epsilon75) {
					epsilon75 = 1.0e-3 * fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
				// 27.07.2016
				iN75 = 12;
				epsilon75 *= 1e-2;
			}
			if (iVar == TEMP) {
				iN75 = 4;
				epsilon75 = fmin(0.1 * fabs(delta075), epsilon75);
				if (bSIMPLErun_now_for_temperature == true) {
					//std::cout << "epsilon75="<< epsilon75 << std::endl;
					//system("PAUSE");
					// ����������������� ������� ���������� ������������������� �� ����������� ��� ������������������ ��������.
					// ������� �������� ���� ������ ��������� �� 5 ��������.
					// 27.07.2016
					epsilon75 *= 1e-10;
					iN75 = 20;
					//epsilon75 *= 1e-16;
					//iN75 = 30;
				}
			}
			if (iVar == PAM) {
				iN75 = 20; // ������� ��� �������� �������� ������ ���� �������� �����.

				if (1.0e-10 * fabs(delta075) < epsilon75) {
					epsilon75 = 1.0e-10 * fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
				//std::cout << epsilon75; system("PAUSE");
				// 27.07.2016.
				//epsilon75 *= 1e-2;
				//iN75 = 20;
			}
		}
		else if ((n75 >= 100000) && (n75 < 300000)) {
			// ������ ��������� ������� �����������.
			if ((iVar == VELOCITY_X_COMPONENT) || (iVar == VELOCITY_Y_COMPONENT) || (iVar == VELOCITY_Z_COMPONENT) ||
				(iVar == NUSHA) || (iVar == TURBULENT_KINETIK_ENERGY) ||
				(iVar == TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA) ||
				(iVar == TURBULENT_KINETIK_ENERGY_STD_K_EPS) ||
				(iVar == TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS)) {
				iN75 = 3; // ����������� ����� ���� �� ���� ��������.
						  // ������ ������ ������� ��� ��������� ������ ����� ������ ������� ������ ���������� iN �������� ��� ��������.
						  // ���� ����� ����� ������������ �� �� �� ����� ����� ����������� �� ��� ��� ���� ������� �� ������ ������ epsilon.
				if (1.0e-3 * fabs(delta075) < epsilon75) {
					epsilon75 = 1.0e-3 * fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
			}
			if (iVar == TEMP) {
				iN75 = 4;
				epsilon75 = fmin(0.1 * fabs(delta075), epsilon75);
				if (bSIMPLErun_now_for_temperature == true) {
					//std::cout << "epsilon75=" << epsilon75 << std::endl;
					//system("PAUSE");
					// ����������������� ������� ���������� ������������������� �� ����������� ��� ������������������ ��������.
					// ������� �������� ���� ������ ��������� �� 5 ��������.
					// 27.07.2016
					epsilon75 *= 1e-10;
					iN75 = 20;
					//epsilon75 *= 1e-16;
					//iN75 = 30;
				}
			}
			if (iVar == PAM) {
				iN75 = 30; // ������� ��� �������� �������� ������ ���� �������� �����.
				if (1.0e-10 * fabs(delta075) < epsilon75) {
					epsilon75 = 1.0e-10 * fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
				//std::cout << epsilon75; system("PAUSE");
			}
		}
		else if ((n75 >= 300000) && (n75 < 1000000)) {
			// ������ ������� ������� �����������.
			if ((iVar == VELOCITY_X_COMPONENT) || (iVar == VELOCITY_Y_COMPONENT) || (iVar == VELOCITY_Z_COMPONENT) ||
				(iVar == NUSHA) || (iVar == TURBULENT_KINETIK_ENERGY) ||
				(iVar == TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA) ||
				(iVar == TURBULENT_KINETIK_ENERGY_STD_K_EPS) ||
				(iVar == TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS)) {
				iN75 = 3; // ����������� ����� ���� �� ���� ��������.
						  // ���� ����� ����� ������������ �� �� �� ����� ����� ����������� �� ��� ��� ���� ������� �� ������ ������ epsilon.
				if (1.0e-3 * fabs(delta075) < epsilon75) {
					epsilon75 = 1.0e-3 * fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
			}
			if (iVar == TEMP) {
				iN75 = 4;
				epsilon75 = 1e-5 * fmin(0.1 * fabs(delta075), epsilon75);
				if (bSIMPLErun_now_for_temperature == true) {
					//std::cout << "epsilon75=" << epsilon75 << std::endl;
					//system("PAUSE");
					// ����������������� ������� ���������� ������������������� �� ����������� ��� ������������������ ��������.
					// ������� �������� ���� ������ ��������� �� 5 ��������.
					// 27.07.2016
					epsilon75 *= 1e-8;
					iN75 = 20;
					//epsilon75 *= 1e-16;
					//iN75 = 30;
				}
			}
			if (iVar == PAM) {
				iN75 = 16; // ������� ��� �������� �������� ������ ���� �������� �����.
				if (1.0e-4 * fabs(delta075) < epsilon75) {
					epsilon75 = 1.0e-4 * fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
				//std::cout << epsilon75; system("PAUSE");
			}
		}
		else if ((n75 >= 1000000) && (n75 < 3000000)) {
			// ������ ���������� ������� �����������.
			if ((iVar == VELOCITY_X_COMPONENT) || (iVar == VELOCITY_Y_COMPONENT) || (iVar == VELOCITY_Z_COMPONENT) ||
				(iVar == NUSHA) || (iVar == TURBULENT_KINETIK_ENERGY) ||
				(iVar == TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA) ||
				(iVar == TURBULENT_KINETIK_ENERGY_STD_K_EPS) ||
				(iVar == TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS)) {
				iN75 = 6; // ����������� ����� ���� �� ���� ��������.
						  // ���� ����� ����� ������������ �� �� �� ����� ����� ����������� �� ��� ��� ���� ������� �� ������ ������ epsilon.
				if (1.0e-3 * fabs(delta075) < epsilon75) {
					epsilon75 = 1.0e-3 * fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
			}
			if (iVar == TEMP) {
				iN75 = 8;
				epsilon75 = 1e-5 * fmin(0.1 * fabs(delta075), epsilon75);
				if (bSIMPLErun_now_for_temperature == true) {
					//std::cout << "epsilon75="<< epsilon75 << std::endl;
					//system("PAUSE");
					// ����������������� ������� ���������� ������������������� �� ����������� ��� ������������������ ��������.
					// ������� �������� ���� ������ ��������� �� 5 ��������.
					// 27.07.2016
					epsilon75 *= 1e-8;
					iN75 = 20;
					//epsilon75 *= 1e-16;
					//iN75 = 30;
				}
			}
			if (iVar == PAM) {
				iN75 = 23; // ������� ��� �������� �������� ������ ���� �������� �����.
				if (1.0e-4 * fabs(delta075) < epsilon75) {
					epsilon75 = 1.0e-4 * fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
				//std::cout << epsilon75; system("PAUSE");
			}
		}
		else if (n75 >= 3000000) {
			// ������ ����� ������� �����������.
			if ((iVar == VELOCITY_X_COMPONENT) || (iVar == VELOCITY_Y_COMPONENT) || (iVar == VELOCITY_Z_COMPONENT) ||
				(iVar == NUSHA) || (iVar == TURBULENT_KINETIK_ENERGY) ||
				(iVar == TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA) ||
				(iVar == TURBULENT_KINETIK_ENERGY_STD_K_EPS) ||
				(iVar == TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS)) {
				iN75 = 6; // ����������� ����� ���� �� ���� ��������.
						  // ���� ����� ����� ������������ �� �� �� ����� ����� ����������� �� ��� ��� ���� ������� �� ������ ������ epsilon.
				if (1.0e-3 * fabs(delta075) < epsilon75) {
					epsilon75 = 1.0e-3 * fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
			}
			if (iVar == TEMP) {
				iN75 = 8;
				epsilon75 = 1e-10 * fmin(0.1 * fabs(delta075), epsilon75);
			}
			if (iVar == PAM) {
				iN75 = 36; // ������� ��� �������� �������� ������ ���� �������� �����.
				if (1.0e-4 * fabs(delta075) < epsilon75) {
					epsilon75 = 1.0e-4 * fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
				//std::cout << epsilon75; system("PAUSE");
			}
		}

		integer maxit75 = 2000;
		if (iVar == TEMP) {
			maxit75 = 2000;
		}
		if (iVar == PAM) {
			maxit75 = 2000; // 2000
		}
		if ((iVar == VELOCITY_X_COMPONENT) || (iVar == VELOCITY_Y_COMPONENT) || (iVar == VELOCITY_Z_COMPONENT)) {
			maxit75 = 100; // 100
		}
		if ((iVar == NUSHA) || (iVar == TURBULENT_KINETIK_ENERGY) ||
			(iVar == TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA) ||
			(iVar == TURBULENT_KINETIK_ENERGY_STD_K_EPS) ||
			(iVar == TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS))
		{
			maxit75 = 100; // 100
		}
		if (iVar == TOTALDEFORMATIONVAR) {
			maxit75 = 800; // 2000
			if (1.0e-4 * fabs(delta075) < epsilon75) {
				epsilon75 = 1.0e-4 * fabs(delta075);
			}
			epsilon75 = 1.0e-12;
			iN75 = 8; // ���������� ������������ ��������.
			if (iflag175 == 1) {
				iflag75 = 1;
			}

		}

		// ���� ����� ������������� �������� ���������� ��������� �� ��������� ����� �� ���������.
		integer i_signal_break_pam_opening75 = 0;
		// x ������� ��������.
		const integer i_limit_signal_pam_break_opening75 = 4000;//20
		doublereal delta_old_iter75 = 1.0e10;

		integer count_iter_for_film_coef75 = 0;

		// ������������ ��� ���������� ���������� ��������������� ��������
		// ��� � ��������� FGMRES ����� ����� � ������� �. ������.
		doublereal norma_b = NormaV_for_gmres(b, n75);

		// �� ����������� ������ ������� ��������� ��������. (�� ����� 10).
		// ���� ������ ������� �� ������������� ��������� ������������.
		//if (iVar == TURBULENT_KINETIK_ENERGY) {
			//std::cout << "TURBULENT_KINETIK_ENERGY: iN75=="<< iN75 <<" iflag75==" << iflag75 << " iflag175==" << iflag175 << " maxit75=" << maxit75 << "\n delta075="<<delta075<<"  epsilon75=" << epsilon75<< "\n";
			//system("pause");
		//}
		//if (iVar == TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA) {
			//std::cout << "TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA: iN75=="<< iN75 <<" iflag75==" << iflag75 << " iflag175==" << iflag175 << " maxit75=" << maxit75 << "\n delta075="<<delta075<<"  epsilon75=" << epsilon75<< "\n";
			//system("pause");
		//}
		while (((icount75 < iN75) && (iflag175 != 0)) || (iflag75 != 0 && icount75 < maxit75)) {

			// 6.01.2017: Body BiCGStab + AMG. (BiCGStab_internal4).

			icount75++;

			count_iter_for_film_coef75++;
			// � ������ ������ ������� - �������, �������-��������� � ��������� ������� �� ��������� �� ����� ��������, 
			// �.�. ��� ��������� ������ ���������� �������. 13 ����� 2016.
			//if (((adiabatic_vs_heat_transfer_coeff > ADIABATIC_WALL_BC) || (breakRUMBAcalc_for_nonlinear_boundary_condition)) && (count_iter_for_film_coef75>5)) break;

			roi75 = Scal(roc75, ri75, n75);
			bet75 = (roi75 / roim175) * (al75 / wi75);


			//std::cout << "roi75=="<< roi75 << ", roim175=="<< roim175<< ", al75==" << al75 << ", wi75==" << wi75 << std::endl;
			//getchar();

#pragma omp parallel for 
			for (i75 = 0; i75 < n75; i75++) {
				doublereal pibuf75 = ri75[i75] + (pi75[i75] - vi75[i75] * wi75) * bet75;
				pi75[i75] = pibuf75;
			}

			// ������ ������������������.
			// Ky=pi
#pragma omp parallel for
			for (i75 = 0; i75 < n75; i75++) {
				y75[i75] = 0.0; // ���� �������� �� � ���� �� ������� ���������� ��� PAM !.
				y76[i75 + 1] = 0.0;
				pi76[i75 + 1] = pi75[i75];
			}

			// multigrid RUMBA preconditioner
			// ��������� 6.01.2017 begin
			// ������ V ����� ������������.
			// A*y76=pi76;
			V_cycle_solve<doublerealT>(Amat, y76, pi76, process_flow_logic, row_ptr_start,
				row_ptr_end, residual_fine, diag, diag_minus_one, n_a, bonly_serial,
				process_flow_beta, F_false_C_true, nu1, nu2, bILU2smoother,
				ilevel, inumberVcyclelocbicgstab, imyinit, maxlevel, milu2, milu0, nested_desection,
				P, nnz_aRP, residual_coarse, igam, nnz_a,
				error_approx_coarse, dapply_ilu_max_pattern_size,
				process_flow_alpha,
				error_approx_fine, nFinestSweeps);
			// ��������� 6.01.2017 end

			// ����������� ����������.
#pragma omp parallel for
			for (i75 = 0; i75 < n75; i75++) {
				y75[i75] = y76[i75 + 1];
			}

			MatrixCRSByVector(val75, col_ind75, row_ptr75, y75, vi75, n75); // vi==A*y;

			if ((fabs(roi75) < 1e-30) && (fabs(Scal(roc75, vi75, n75)) < 1e-30)) {
				al75 = 1.0;
			}
			else if (fabs(roi75) < 1e-30) {
				al75 = 0.0;
			}
			else {
				al75 = roi75 / Scal(roc75, vi75, n75);
			}


#pragma omp parallel for
			for (i75 = 0; i75 < n75; i75++) {
				s75[i75] = ri75[i75] - al75 * vi75[i75];
			}

			// ������ ������������������.
			// Kz=s

#pragma omp parallel for
			for (i75 = 0; i75 < n75; i75++) z75[i75] = 0.0; // ���� �������� �� � ���� �� �� ����� ���������� ��� PAM !.

#pragma omp parallel for
			for (i75 = 0; i75 < n75; i75++) {
				vec75[i75] = s75[i75];
				z76[i75 + 1] = 0.0;
				s76[i75 + 1] = s75[i75];
			}

			// multigrid RUMBA preconditioner
			// ��������� 6.01.2017 begin
			// ������ V ����� ������������.
			// A*z76=s76;
			V_cycle_solve<doublerealT>(Amat, z76, s76, process_flow_logic, row_ptr_start,
				row_ptr_end, residual_fine, diag, diag_minus_one, n_a, bonly_serial,
				process_flow_beta, F_false_C_true, nu1, nu2, bILU2smoother,
				ilevel, inumberVcyclelocbicgstab, imyinit, maxlevel, milu2, milu0, nested_desection,
				P, nnz_aRP, residual_coarse, igam, nnz_a,
				error_approx_coarse, dapply_ilu_max_pattern_size,
				process_flow_alpha,
				error_approx_fine, nFinestSweeps);
			// ��������� 6.01.2017 end

#pragma omp parallel for
			for (i75 = 0; i75 < n75; i75++) {
				s75[i75] = vec75[i75];
				// ���������� ���������.
				z75[i75] = z76[i75 + 1];
			}

			MatrixCRSByVector(val75, col_ind75, row_ptr75, z75, t75, n75); // t==A*z;

			wi75 = Scal(t75, s75, n75) / Scal(t75, t75, n75);
			// std::cout << "Scal(t75,s75,n75)==" << Scal(t75,s75,n75) << ", Scal(t75,t75,n75)=="<< Scal(t75,t75,n75) << std::endl;

#pragma omp parallel for
			for (i75 = 0; i75 < n75; i75++) {
				//dx75[i75]+=al75*pi75[i75]+wi75*s75[i75]; // ��� ���� ��� �������������������
				dx75[i75] += al75 * y75[i75] + wi75 * z75[i75]; // ��� ����� � ��������������������
				ri75[i75] = s75[i75] - wi75 * t75[i75];
			}
			deltai75 = NormaV(ri75, n75);

			//std::cout << "deltai75=" << deltai75 << std::endl; system("PAUSE");

			// ������ ������� �� �������
			if (bprint_mesage_diagnostic) {
				if ((icount75 % 10) == 0) {
					std::cout << "iter  residual" << std::endl;
				}

			std::cout << icount75 << " " << deltai75 << std::endl;
			}

			// 28.07.2016.
			std::cout << icount75 << " " << deltai75 << std::endl;

			//getchar();
			if (deltai75 > delta_old_iter75) i_signal_break_pam_opening75++;
			delta_old_iter75 = deltai75;
			if (iVar == PAM) {
				if (i_signal_break_pam_opening75 > i_limit_signal_pam_break_opening75) {
					// ��������� ����� �� �����.
					std::cout <<  "icount PAM=" << icount75 << "\n";

					break;
				}
			}

			if (deltai75 < epsilon75) iflag75 = 0; // ����� ����������
			else roim175 = roi75;

			if (iVar == TEMP) {
				//std::cout << "epsilon=" << epsilon75 <<" deltai="<< deltai75<< " icount="<< icount75 << std::endl;
				//getchar();
			}

			//04.04.2019
			// �������� ������� ��������� ��������������� �������� ������ ��������� FGMRES �.�����.
			if (0 && ((NormaV_for_gmres(ri75, n75) / norma_b) <= 0.1 * dterminatedTResudual)) {
				iflag75 = 0; // ����� ����������
				std::cout << "dosrochnji vjhod"<< std::endl;
				icount_V_cycle = icount75; // ���������� �������� � BiCGStabP ��� ����.
				break;
			}

			icount_V_cycle = icount75; // ���������� �������� � BiCGStabP ��� ����.

			if (icount75 > 2600) break; // 15.02.2017

		}


		if (icount75 == maxit75) {
			// amg solver divergence detected 
			// ��������� ���������� ����� ��������.
			return true;
		}

		// ����������� ���������� ����������.
#pragma omp parallel for
		for (i75 = 0; i75 < n75; i75++) {
			x[i75 + 1] = dx75[i75];
			x_best_search[i75 + 1] = dx75[i75];
		}

		// ������������ ����������� ������.
		// ������ ������������������
		if (pi76 != nullptr) {
			delete[] pi76;
			pi76 = nullptr;
		}
		if (y76 != nullptr) {
			delete[] y76;
			y76 = nullptr;
		}
		// ������ ������������������
		if (z76 != nullptr) {
			delete[] z76;
			z76 = nullptr;
		}
		if (s76 != nullptr) {
			delete[] s76;
			s76 = nullptr;
		}
		if (ri75 != nullptr) {
			delete[] ri75;
			ri75 = nullptr;
		}
		if (roc75 != nullptr) {
			delete[] roc75;
			roc75 = nullptr;
		}
		if (s75 != nullptr) {
			delete[] s75;
			s75 = nullptr;
		}
		if (t75 != nullptr) {
			delete[] t75;
			t75 = nullptr;
		}
		if (vec75 != nullptr) {
			delete[] vec75;
			vec75 = nullptr;
		}
		if (vi75 != nullptr) {
			delete[] vi75;
			vi75 = nullptr;
		}
		if (pi75 != nullptr) {
			delete[] pi75;
			pi75 = nullptr;
		}
		if (dx75 != nullptr) {
			delete[] dx75;
			dx75 = nullptr;
		}
		if (dax75 != nullptr) {
			delete[] dax75;
			dax75 = nullptr;
		}
		if (y75 != nullptr) {
			delete[] y75;
			y75 = nullptr;
		}
		if (z75 != nullptr) {
			delete[] z75;
			z75 = nullptr;
		}

		// ������������ ����������� ������.
		if (val75 != nullptr) {
			delete[] val75;
			val75 = nullptr;
		}
		if (col_ind75 != nullptr) {
			delete[] col_ind75;
			col_ind75 = nullptr;
		}
		if (row_ptr75 != nullptr) {
			delete[] row_ptr75;
			row_ptr75 = nullptr;
		}

	}
	else
	{   //  09.01.2018
		// ������������� ������������ ��������� ��������: ������� ��� FGMRES � ��������� ��� ������������������ � ������� V - �����.
		// FGMRes if (my_amg_manager.istabilization == 2)
		// ������ ������� ����������� ������ ����������� ������� � ������� �� ������ ��������
		// ���������� ����������� ������������� �������������������. ������� ����� ����� � ������� �. ������ 1986 ����.

		integer inumberVcyclelocbicgstab = 1;

		// ��������� �������� ���������� � ����.
		integer n75 = n_a[0]; // ����� ����������� �� ��������� ������.
		doublereal* val75 = nullptr;
		val75 = new doublereal[nnz_a[0]];
		integer* col_ind75 = nullptr;
		col_ind75 = new integer[nnz_a[0]];
		integer* row_ptr75 = nullptr;
		row_ptr75 = new integer[n_a[0] + 1];
		if ((val75 == nullptr) || (col_ind75 == nullptr) || (row_ptr75 == nullptr)) {
			// ������������ ������ �� ������ ������������.
			std::cout << "Problem: not enough memory on your equipment for val, col_ind or row_ptr: FGMRes + camg..." << std::endl;
			std::cout << "Please any key to exit..." << std::endl;
			exit(1);
		}

		// ������������� �������.
		// �������������� � ������� CRS.
#pragma omp parallel for
		for (integer i_1 = 1; i_1 <= n_a[0]; i_1++) {

			for (integer i_2 = row_ptr_start[i_1]; i_2 <= row_ptr_end[i_1]; i_2++) {
				//if (Amat.i[i_2] == Amat.j[i_2]) {
					//if (i_1 != Amat.i[i_2]) {
						//std::cout << "err i!=i"<< std::endl;
						//system("PAUSE");
					//}
				if (i_1 == Amat.j[i_2]) {
					val75[i_2 - 1] = diag[0][i_1];
					col_ind75[i_2 - 1] = i_1 - 1;
				}
				else {
					val75[i_2 - 1] = Amat.aij[i_2];
					col_ind75[i_2 - 1] = Amat.j[i_2] - 1;
				}
			}
			row_ptr75[i_1 - 1] = row_ptr_start[i_1] - 1;
		}

		row_ptr75[n_a[0]] = row_ptr_end[n_a[0]];
		// ������������� � ������� CRS.


		bool bnorelax = true; // ��� ��������� ���������������� �� ������������ ����������.
		integer m_restart = my_amg_manager.m_restart;

		doublereal resid;
		integer i, j = 1, k;
		//Vector s(m + 1), cs(m + 1), sn(m + 1), w;
		doublereal* w = new doublereal[n75];
		doublereal* s = new doublereal[m_restart + 2];
		doublereal* cs = new doublereal[m_restart + 2];
		doublereal* sn = new doublereal[m_restart + 2];

		doublereal* dx = new doublereal[n75];
		doublereal* buffer = new doublereal[n75];
		doublereal* Zcopy = new doublereal[n75 + 1];
		doublereal* vCopy = new doublereal[n75 + 1];

		// A*x=b, x - �������, b - ������ �����. 
		// ���������� � � � b ���������� � �������.

		// ��������� �����������
		for (i = 0; i < n75; i++) dx[i] = x[i + 1];


		//doublereal normb = norm(M.solve(b));
		doublereal normb = 0.0;
		// ����� ����������� ��� ��� �����
		// ������ ������ ��� ��� ������������



		normb = NormaV_for_gmres(&b[1], n75);
		//normb = NormaV(buffer, n75);

		//Vector r = &b[1] - A * x;
		doublereal* r = new doublereal[n75];
		MatrixCRSByVector(val75, col_ind75, row_ptr75, dx, r, n75); // ��������� ������ �  r
		for (i = 0; i < n75; i++) r[i] = b[i + 1] - r[i];

		//  calculate residual precontidioning;


		//doublereal beta = norm(r);
		doublereal beta = 0.0;



		beta = NormaV_for_gmres(r, n75);

		if (fabs(normb) < 1.0e-30)
			normb = 1;

		doublereal norm_r = 0.0;


		norm_r = NormaV_for_gmres(r, n75);

		const integer maxit = 2000;

		if ((resid = norm_r / normb) <= dterminatedTResudual) {
			//tol = resid;
			//maxit = 0;
			//return 0;
			delete[] val75;
			delete[] col_ind75;
			delete[] row_ptr75;
			delete[] w;
			delete[] s;
			delete[] cs;
			delete[] sn;
			delete[] dx;
			delete[] buffer;
			delete[] Zcopy;
			delete[] vCopy;
			delete[] r;
			goto LABEL_FGMRES_CONTINUE;
		}

		//integer i_1 = 0; // ������� ����� for

		doublereal** H = new doublereal * [m_restart + 2]; // Hessenberg
		for (integer i_1 = 0; i_1 < m_restart + 2; i_1++) H[i_1] = new doublereal[m_restart + 2];


		for (integer i_1 = 0; i_1 < m_restart + 2; i_1++)
		{
			for (integer j_1 = 0; j_1 < m_restart + 2; j_1++)
			{
				H[i_1][j_1] = 0.0;
			}
		}

		//Vector *v = new Vector[m_restart + 1];
		doublereal** v = new doublereal * [m_restart + 2];
		for (integer i_1 = 0; i_1 <= m_restart + 1; i_1++) v[i_1] = new doublereal[n75];


		for (integer i_1 = 0; i_1 <= m_restart + 1; i_1++) {
			for (integer j_1 = 0; j_1 < n75; j_1++)
			{
				v[i_1][j_1] = 0.0;
			}
		}

		doublereal** Z = new doublereal * [m_restart + 2];
		for (integer i_1 = 0; i_1 <= m_restart + 1; i_1++) Z[i_1] = new doublereal[n75];

		for (integer i_1 = 0; i_1 <= m_restart + 1; i_1++) {
			for (integer j_1 = 0; j_1 < n75; j_1++)
			{
				Z[i_1][j_1] = 0.0;
			}
		}

		j = 1; // ����� ������ ��������
			   //doublereal delta = 1.0e-3;// DOPOLNENIE

		integer i_copy;

		while (j <= maxit) {

			//v[0] = r * (1.0 / beta);    // ??? r / beta
			for (integer j_1 = 0; j_1 < n75; j_1++)
			{
				v[0][j_1] = r[j_1] * (1.0 / beta);
			}

			//s = 0.0;
			for (integer i_1 = 0; i_1 <= m_restart + 1; i_1++) s[i_1] = 0.0;
			s[0] = beta;
			//s[0] = 1.0;


			for (integer i_1 = 0; i_1 < m_restart + 2; i_1++)
			{ // DOPOLNENIE
				for (integer j_1 = 0; j_1 < m_restart + 2; j_1++)
				{
					H[i_1][j_1] = 0.0;
				}
			}


			// ��������������� ��������.
			for (i = 0; i < m_restart && j <= maxit; i++, j++) {

				i_copy = i;


				// KZ[i]=v[i]

				// (LU)Z[i]=v[i];

				// multigrid Ruge and Stuben preconditioning [1986].
				// ���������� ������ V �����.
				// K*Z = v;
				for (integer i_1 = 0; i_1 < n75; i_1++) {
					Zcopy[i_1 + 1] = 0.0;
					vCopy[i_1 + 1] = v[i][i_1];
				}

				// ������������������ � ������� V ����� �������������� ������.
				// ������� ��������� �����������
				for (integer i_numberV_cycle = 0; i_numberV_cycle < 1; i_numberV_cycle++) {
					// ���������� ������ V �����.
					// A*Zcopy=vCopy;
					// � Zcopy � vCopy ��������� ���������� � �������.
					V_cycle_solve<doublerealT>(Amat, Zcopy, vCopy, process_flow_logic, row_ptr_start,
						row_ptr_end, residual_fine, diag, diag_minus_one, n_a, bonly_serial,
						process_flow_beta, F_false_C_true, nu1, nu2, bILU2smoother,
						ilevel, inumberVcyclelocbicgstab, imyinit, maxlevel, milu2, milu0, nested_desection,
						P, nnz_aRP, residual_coarse, igam, nnz_a,
						error_approx_coarse, dapply_ilu_max_pattern_size,
						process_flow_alpha,
						error_approx_fine, nFinestSweeps);
					//getchar();
				}

				for (integer i_1 = 0; i_1 < n75; i_1++) {
					Z[i][i_1] = Zcopy[i_1 + 1];
				}


				// ������ ��� �������������������.
				//for (i_1 = 0; i_1 < n75; i_1++) Z[i][i_1] = v[i][i_1];

				// ���������������� ��� ������������������.
				//w = A * Z[i];
				MatrixCRSByVector(val75, col_ind75, row_ptr75, Z[i], w, n75); // ��������� ������ �  w

				for (k = 0; k <= i; k++) {
					H[k][i] = Scal(w, v[k], n75);

					for (integer j_1 = 0; j_1 < n75; j_1++)
					{
						w[j_1] -= H[k][i] * v[k][j_1];
					}
				}
				H[i + 1][i] = NormaV_for_gmres(w, n75);



				for (integer j_1 = 0; j_1 < n75; j_1++)
				{
					v[i + 1][j_1] = w[j_1] * (1.0 / H[i + 1][i]); // ??? w / H(i+1, i)
				}
				// ��������� ��������������� ��������.
				// � v - �������� ����������������� ����� ��������������� ������� ����������� m_restart.
				// H - ����������������� ������� ����������� - ������� ������������� ���������������.


				// 26.11.2017
				// ��� ����������� � ���������� ����� ����.
				for (k = 0; k < i; k++)
					ApplyPlaneRotation(H[k][i], H[k + 1][i], cs[k], sn[k]);

				GeneratePlaneRotation(H[i][i], H[i + 1][i], cs[i], sn[i]);
				ApplyPlaneRotation(H[i][i], H[i + 1][i], cs[i], sn[i]);
				ApplyPlaneRotation(s[i], s[i + 1], cs[i], sn[i]);



				// ������� ��������� ������ ������� ���������� ������� �� ���� �������� ���������,
				// �.�. ����� ��� �������� � ������� �������.
				//if (fabs(s[i] - s[i + 1]) < 1.0e-37) s[i + 1] = 1.05*s[i];

				std::cout << j << " " << fabs(s[i + 1]) / normb <<" \n";
				//std::cout << j <<" "<< beta*fabs(s[i + 1]) << " \n";
				//getchar();

				resid = fabs(s[i + 1]) / normb;
				//resid = beta*fabs(s[i + 1]);

				if ((resid) < dterminatedTResudual) {
					std::cout << "dosrochnji vjhod" << std::endl;
					//getchar();				
					Update(dx, i, n75, H, s, Z);
					//tol = resid;
					//maxit = j;

					for (integer i_1 = 0; i_1 < n75; i_1++) {
						x[i_1 + 1] = dx[i_1];
						x_best_search[i_1 + 1] = dx[i_1];
					}

					for (integer i_1 = 0; i_1 <= m_restart + 1; i_1++) delete[] v[i_1];
					delete[] v;
					for (integer i_1 = 0; i_1 <= m_restart + 1; i_1++) delete[] Z[i_1];
					delete[] Z;
					for (integer i_1 = 0; i_1 < m_restart + 2; i_1++) delete[] H[i_1];
					delete[] H;
					delete[] dx;
					delete[] buffer;
					delete[] r;
					delete[] w;
					delete[] s;
					delete[] cs;
					delete[] sn;
					delete[] Zcopy;
					delete[] vCopy;

					// ������������ ����������� ������.
					if (val75 != nullptr) {
						delete[] val75;
						val75 = nullptr;
					}
					if (col_ind75 != nullptr) {
						delete[] col_ind75;
						col_ind75 = nullptr;
					}
					if (row_ptr75 != nullptr) {
						delete[] row_ptr75;
						row_ptr75 = nullptr;
					}

					goto LABEL_FGMRES_CONTINUE;

				}
			}



			// i-1 -> m_restart-1
			Update(dx, i - 1, n75, H, s, Z);//i-1 //ERROR

											//r = M.solve(b - A * x);
			MatrixCRSByVector(val75, col_ind75, row_ptr75, dx, r, n75); // ��������� ������ � r
			for (integer i_1 = 0; i_1 < n75; i_1++) r[i_1] = b[i_1 + 1] - r[i_1];

			//beta = norm(r);
			beta = NormaV_for_gmres(r, n75);

			resid = beta / normb;
			//resid = beta;

			if ((resid) < dterminatedTResudual) {
				//tol = resid;
				//maxit = j;

				std::cout << "end" << std::endl;
				//getchar();

				for (integer i_1 = 0; i_1 < n75; i_1++) {
					x[i_1 + 1] = dx[i_1];
					x_best_search[i_1 + 1] = dx[i_1];
				}

				for (integer i_1 = 0; i_1 <= m_restart + 1; i_1++) delete[] v[i_1];
				delete[] v;
				for (integer i_1 = 0; i_1 <= m_restart + 1; i_1++) delete[] Z[i_1];
				delete[] Z;
				for (integer i_1 = 0; i_1 < m_restart + 2; i_1++) delete[] H[i_1];
				delete[] H;
				delete[] dx;
				delete[] buffer;
				delete[] r;
				delete[] w;
				delete[] s;
				delete[] cs;
				delete[] sn;
				delete[] Zcopy;
				delete[] vCopy;

				// ������������ ����������� ������.
				if (val75 != nullptr) {
					delete[] val75;
					val75 = nullptr;
				}
				if (col_ind75 != nullptr) {
					delete[] col_ind75;
					col_ind75 = nullptr;
				}
				if (row_ptr75 != nullptr) {
					delete[] row_ptr75;
					row_ptr75 = nullptr;
				}

				goto LABEL_FGMRES_CONTINUE;


			}
		}

		if (j-1 == maxit) {
			// amg solver divergence detected 
			// ��������� ���������� ����� ��������.
			return true;
		}


		//tol = resid;
		for (integer i_1 = 0; i_1 < n75; i_1++) {
			x[i_1 + 1] = dx[i_1];
			x_best_search[i_1 + 1] = dx[i_1];
		}

		for (integer i_1 = 0; i_1 <= m_restart + 1; i_1++) delete[] v[i_1];
		delete[] v;
		for (integer i_1 = 0; i_1 <= m_restart + 1; i_1++) delete[] Z[i_1];
		delete[] Z;
		for (integer i_1 = 0; i_1 < m_restart + 2; i_1++) delete[] H[i_1];
		delete[] H;
		delete[] dx;
		delete[] buffer;
		delete[] r;
		delete[] w;
		delete[] s;
		delete[] cs;
		delete[] sn;
		delete[] Zcopy;
		delete[] vCopy;

		// ������������ ����������� ������.
		if (val75 != nullptr) {
			delete[] val75;
			val75 = nullptr;
		}
		if (col_ind75 != nullptr) {
			delete[] col_ind75;
			col_ind75 = nullptr;
		}
		if (row_ptr75 != nullptr) {
			delete[] row_ptr75;
			row_ptr75 = nullptr;
		}
		goto LABEL_FGMRES_CONTINUE;




	}




	for (int i_52 = 0; i_52 < maxlevel; i_52++) {
		if (dbgmres_smoother[i_52].val != nullptr) {
			delete[] dbgmres_smoother[i_52].val;
			dbgmres_smoother[i_52].val = nullptr;
		}
		if (dbgmres_smoother[i_52].col_ind != nullptr) {
			delete[] dbgmres_smoother[i_52].col_ind;
			dbgmres_smoother[i_52].col_ind = nullptr;
		}
		if (dbgmres_smoother[i_52].row_ptr != nullptr) {
			delete[] dbgmres_smoother[i_52].row_ptr;
			dbgmres_smoother[i_52].row_ptr = nullptr;
		}
	}
	delete[] dbgmres_smoother;
	dbgmres_smoother = nullptr;

LABEL_FGMRES_CONTINUE:

	if (dbgmres_smoother != nullptr) {
		for (int i_52 = 0; i_52 < maxlevel; i_52++) {
			if (dbgmres_smoother[i_52].val != nullptr) {
				delete[] dbgmres_smoother[i_52].val;
				dbgmres_smoother[i_52].val = nullptr;
			}
			if (dbgmres_smoother[i_52].col_ind != nullptr) {
				delete[] dbgmres_smoother[i_52].col_ind;
				dbgmres_smoother[i_52].col_ind = nullptr;
			}
			if (dbgmres_smoother[i_52].row_ptr != nullptr) {
				delete[] dbgmres_smoother[i_52].row_ptr;
				dbgmres_smoother[i_52].row_ptr = nullptr;
			}
		}
		delete[] dbgmres_smoother;
		dbgmres_smoother = nullptr;
	}

	if (debug_reshime) system("pause");





	// ��������: ������ ��� ������� ������������ ����������.
#pragma omp parallel for
	for (integer i47 = 1; i47 <= n_a[0]; i47++) {
		x[i47] = x_best_search[i47];
	}

	identiti = true;
	for (integer i47 = 1; i47 <= n_a[0]; i47++) {
		if (fabs(x[i47] - x_best_search_init[i47]) > 1e-5) {
			identiti = false;
		}
	}
	if (identiti) {
		if ((iVar != TOTALDEFORMATIONVAR) && (iVar != TURBULENT_KINETIK_ENERGY)) {
			std::cout << "identity situation" << std::endl;
			// ���� ������� x_best_search ������ �� ���� �����������.
#pragma omp parallel for
			for (integer i47 = 1; i47 <= n_a[0]; i47++) {
				x[i47] = x_best_search2[i47];
			}
		}
	}

	// ����� � ������� �� �������� ���� �������� ������� ��������� 1.0e7.
FULL_DIVERGENCE_DETECTED:

	std::cout << "number of negative diagonals: ibsp_length=" << ibsp_length << std::endl;

	// ��������������� ��������� ����� ���������� �� ������.
	if (bprint_mesage_diagnostic) {
		switch (iVar) {
		case PAM: std::cout << "PAM" << std::endl;  break;
		case VELOCITY_X_COMPONENT:  std::cout << "VELOCITY_X_COMPONENT"<< std::endl; break;
		case VELOCITY_Y_COMPONENT:  std::cout << "VELOCITY_Y_COMPONENT"<< std::endl; break;
		case VELOCITY_Z_COMPONENT:  std::cout << "VELOCITY_Z_COMPONENT"<< std::endl; break;
		case NUSHA: std::cout << "NU"<< std::endl;  break;
		case TURBULENT_KINETIK_ENERGY: std::cout << "TURBULENT_KINETIK_ENERGY"<< std::endl;  break;
		case TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA: std::cout << "TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA"<< std::endl;  break;
		case TURBULENT_KINETIK_ENERGY_STD_K_EPS:  std::cout << " TURBULENT_KINETIK_ENERGY_STD_K_EPS"<< std::endl; break;
		case TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS:  std::cout << "TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS"<< std::endl; break;
		case TEMP:  std::cout << "TEMP"<< std::endl; break;
		case TOTALDEFORMATIONVAR: std::cout << "Stress system"<< std::endl; break;
		}
	}
	else {

		//switch (iVar) {
		// �������� �������� ���������� 3�/��� ilevel_VX_VY_VZ=10, ilevel_PAM=5 ��� 6.

		//case PAM: std::cout << "PAM "<< ilevel<<" "<< n_a[ilevel - 4] / n_a[ilevel - 3]<<" "<< n_a[ilevel - 3] / n_a[ilevel-2]<< " "<< n_a[ilevel - 2] / n_a[ilevel - 1]<<" "<< n_a[ilevel - 1] / n_a[ilevel]<<std::endl;  break;
		//case VELOCITY_X_COMPONENT:  std::cout << "VELOCITY_X_COMPONENT "<< ilevel<<" "<< n_a[ilevel - 4] / n_a[ilevel - 3] <<" "<< n_a[ilevel - 3] / n_a[ilevel - 2] << " " << n_a[ilevel - 2] / n_a[ilevel - 1]<<" "<< n_a[ilevel - 1] / n_a[ilevel]<<std::endl; break;
		//case VELOCITY_Y_COMPONENT:  std::cout << "VELOCITY_Y_COMPONENT "<< ilevel<<" " << n_a[ilevel - 4] / n_a[ilevel - 3]<<" "<< n_a[ilevel - 3] / n_a[ilevel - 2]<<" "<< n_a[ilevel - 2] / n_a[ilevel - 1]<<" "<< n_a[ilevel - 1] / n_a[ilevel]<<std::endl; break;
		//case VELOCITY_Z_COMPONENT:  std::cout << "VELOCITY_Z_COMPONENT "<< ilevel<< " "<< n_a[ilevel - 4] / n_a[ilevel - 3]<<" "<< n_a[ilevel - 3] / n_a[ilevel - 2]<<" "<< n_a[ilevel - 2] / n_a[ilevel - 1]<<" "<< n_a[ilevel - 1] / n_a[ilevel]<<std::endl; break;
		//case NUSHA:  std::cout << "NUSHA %lld %e %e %e %e\n"<< ilevel<< " "<<  n_a[ilevel - 4] / n_a[ilevel - 3]<<" "<< n_a[ilevel - 3] / n_a[ilevel - 2]<<" "<< n_a[ilevel - 2] / n_a[ilevel - 1]<<" "<< n_a[ilevel - 1] / n_a[ilevel]<<std::endl; break;
		//case TURBULENT_KINETIK_ENERGY:  std::cout << "TURBULENT_KINETIK_ENERGY "<< ilevel<<" "<< n_a[ilevel - 4] / n_a[ilevel - 3]<<" " << n_a[ilevel - 3] / n_a[ilevel - 2]<<" "<< n_a[ilevel - 2] / n_a[ilevel - 1]<<" "<< n_a[ilevel - 1] / n_a[ilevel]<<std::endl; break;
		//case TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA:  std::cout << "TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA "<< ilevel<< " " << n_a[ilevel - 4] / n_a[ilevel - 3]<<" "<< n_a[ilevel - 3] / n_a[ilevel - 2]<<" "<< n_a[ilevel - 2] / n_a[ilevel - 1]<<" "<< n_a[ilevel - 1] / n_a[ilevel]<<std::endl; break;
		//case TURBULENT_KINETIK_ENERGY_STD_K_EPS:  std::cout << " TURBULENT_KINETIK_ENERGY_STD_K_EPS " << ilevel<< " " <<  n_a[ilevel - 4] / n_a[ilevel - 3]<<" " << n_a[ilevel - 3] / n_a[ilevel - 2]<< " " << n_a[ilevel - 2] / n_a[ilevel - 1]<<" "<< n_a[ilevel - 1] / n_a[ilevel]<<std::endl; break;
		//case TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS:  std::cout << "TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS  " << ilevel<<" "<< n_a[ilevel - 4] / n_a[ilevel - 3]<<" "<< n_a[ilevel - 3] / n_a[ilevel - 2]<< " " << n_a[ilevel - 2] / n_a[ilevel - 1]<<" "<< n_a[ilevel - 1] / n_a[ilevel]<<std::endl; break;
		//case TEMP:  std::cout << "TEMP "<< ilevel<< " "<<  n_a[ilevel - 4] / n_a[ilevel - 3]<< " "<<  n_a[ilevel - 3] / n_a[ilevel - 2]<< " " << n_a[ilevel - 2] / n_a[ilevel - 1]<<" "<< n_a[ilevel - 1] / n_a[ilevel]<<std::endl; break;
		//case TOTALDEFORMATIONVAR:  std::cout << "Stress system "<< ilevel<< " "<<  n_a[ilevel - 4] / n_a[ilevel - 3]<< " "<< n_a[ilevel - 3] / n_a[ilevel - 2]<<" "<< n_a[ilevel - 2] / n_a[ilevel - 1]<< " "<< n_a[ilevel - 1] / n_a[ilevel]<<std::endl; break;
		//}

		switch (iVar) {
			// �������� �������� ���������� 3�/��� ilevel_VX_VY_VZ=10, ilevel_PAM=5 ��� 6.

		case PAM: std::cout << "PAM level="<<ilevel<<"  CopA="<<dr_grid_complexity<<" CopP="<<(doublereal)(nnz_P_memo_all / n_a[0])<<" nV="<<icount_V_cycle<<" res0="<<dres_initial<<" n_a[ilevel - 2]="<<n_a[ilevel - 2]<<" n_a[ilevel - 1]="<<n_a[ilevel - 1]<<" n_a[ilevel]="<< n_a[ilevel]<<std::endl;  break;
		case VELOCITY_X_COMPONENT:  std::cout << "VELOCITY_X_COMPONENT level="<<ilevel<<" CopA="<<dr_grid_complexity<<" CopP="<<(doublereal)(nnz_P_memo_all / n_a[0])<<" nV="<<icount_V_cycle<<" res0="<<dres_initial<<"  n_a[ilevel - 2]="<<n_a[ilevel - 2]<<"  n_a[ilevel - 1]="<<n_a[ilevel - 1]<<" n_a[ilevel]="<< n_a[ilevel]<<std::endl; break;
		case VELOCITY_Y_COMPONENT:  std::cout << "VELOCITY_Y_COMPONENT level="<<ilevel<<" CopA="<<dr_grid_complexity<<" CopP="<<(doublereal)(nnz_P_memo_all / n_a[0])<<" nV="<<icount_V_cycle<<" res0="<<dres_initial<<"  n_a[ilevel - 2]="<<n_a[ilevel - 2]<<"  n_a[ilevel - 1]="<<n_a[ilevel - 1]<<" n_a[ilevel]="<< n_a[ilevel]<<std::endl; break;
		case VELOCITY_Z_COMPONENT:  std::cout << "VELOCITY_Z_COMPONENT level="<<ilevel<<" CopA="<<dr_grid_complexity<<" CopP="<<(doublereal)(nnz_P_memo_all / n_a[0])<<" nV="<<icount_V_cycle<<" res0="<<dres_initial<<"  n_a[ilevel - 2]="<<n_a[ilevel - 2]<<"  n_a[ilevel - 1]="<<n_a[ilevel - 1]<<" n_a[ilevel]="<< n_a[ilevel]<<std::endl; break;
		case NUSHA:  std::cout << "NUSHA level="<<ilevel<<" CopA="<<dr_grid_complexity<<" CopP="<<(doublereal)(nnz_P_memo_all / n_a[0])<<" nV="<<icount_V_cycle<<" res0="<<dres_initial<<"  n_a[ilevel - 2]="<<n_a[ilevel - 2]<<"  n_a[ilevel - 1]="<<n_a[ilevel - 1]<<" n_a[ilevel]="<< n_a[ilevel]<<std::endl; break;
		case TURBULENT_KINETIK_ENERGY:  std::cout << "TURBULENT_KINETIK_ENERGY level="<<ilevel<<" CopA="<<dr_grid_complexity<<" CopP="<<(doublereal)(nnz_P_memo_all / n_a[0])<<" nV="<<icount_V_cycle<<" res0="<<dres_initial<<"  n_a[ilevel - 2]="<<n_a[ilevel - 2]<<"  n_a[ilevel - 1]="<<n_a[ilevel - 1]<<" n_a[ilevel]="<< n_a[ilevel]<<std::endl; break;
		case TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA:  std::cout << "TURBULENT_SPECIFIC_DISSIPATION_RATE_OMEGA level="<<ilevel<<" CopA="<<dr_grid_complexity<<" CopP="<<(doublereal)(nnz_P_memo_all / n_a[0])<<" nV="<<icount_V_cycle<<" res0="<<dres_initial<<"  n_a[ilevel - 2]="<<n_a[ilevel - 2]<<"  n_a[ilevel - 1]="<<n_a[ilevel - 1]<<" n_a[ilevel]="<< n_a[ilevel]<<std::endl; break;
		case TURBULENT_KINETIK_ENERGY_STD_K_EPS:  std::cout << " TURBULENT_KINETIK_ENERGY_STD_K_EPS level = "<<ilevel<<" CopA = "<<dr_grid_complexity<<" CopP = "<<(doublereal)(nnz_P_memo_all / n_a[0])<<" nV = "<<icount_V_cycle<<" res0 = "<<dres_initial<<"  n_a[ilevel - 2] ="<<n_a[ilevel - 2]<<"  n_a[ilevel - 1] = "<<n_a[ilevel - 1]<<" n_a[ilevel] = "<< n_a[ilevel]<<std::endl; break;
		case TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS:  std::cout << "TURBULENT_DISSIPATION_RATE_EPSILON_STD_K_EPS level = "<<ilevel<<" CopA = "<<dr_grid_complexity<<" CopP = "<<(doublereal)(nnz_P_memo_all / n_a[0])<<" nV = "<<icount_V_cycle<<" res0 ="<<dres_initial<<"  n_a[ilevel - 2] = "<<n_a[ilevel - 2]<<"  n_a[ilevel - 1] = "<<n_a[ilevel - 1]<<" n_a[ilevel] = "<< n_a[ilevel]<<std::endl;  break;
		case TEMP:  std::cout << "TEMP level="<<ilevel<<" CopA="<<dr_grid_complexity<<" CopP="<<(doublereal)(nnz_P_memo_all / n_a[0])<<" nV="<<icount_V_cycle<<" res0="<<dres_initial<<"  n_a[ilevel - 2]="<<n_a[ilevel - 2]<<"  n_a[ilevel - 1]="<<n_a[ilevel - 1]<<" n_a[ilevel]="<< n_a[ilevel]<<std::endl; break;
		case TOTALDEFORMATIONVAR:  std::cout << "Stress system  level="<<ilevel<<" CopA="<<dr_grid_complexity<<" CopP="<<(doublereal)(nnz_P_memo_all / n_a[0])<<" nV="<<icount_V_cycle<<" res0="<<dres_initial<<"  n_a[ilevel - 2]="<<n_a[ilevel - 2]<<"  n_a[ilevel - 1]="<<n_a[ilevel - 1]<<" n_a[ilevel]="<< n_a[ilevel]<<std::endl; break;
		}


	}


	// free
	if (x_best_search2 != nullptr) {
		delete[] x_best_search2;
		x_best_search2 = nullptr;
	}
	if (x_best_search_init != nullptr) {
		delete[] x_best_search_init;
		x_best_search_init = nullptr;
	}


	// free	
	if (bnested_desection_global_amg != nullptr) {
		free(bnested_desection_global_amg);  // ���������� ������.
		bnested_desection_global_amg = nullptr;
	}
	for (integer i_scan_levels = 0; i_scan_levels <= maxlevel - 1; i_scan_levels++) {
		if (ilevel + 1 > i_scan_levels) {
			// free			
				if (diag[i_scan_levels] != nullptr) {
					free(diag[i_scan_levels]);
					diag[i_scan_levels] = nullptr;
				}
				if (diag_minus_one[i_scan_levels] != nullptr) {
					free(diag_minus_one[i_scan_levels]);
					diag_minus_one[i_scan_levels] = nullptr;
				}
				if (nested_desection[i_scan_levels] != nullptr) {
					free(nested_desection[i_scan_levels]);
					nested_desection[i_scan_levels] = nullptr;
				}
				integer i_scan_levels_prev = i_scan_levels - 1;
				if (i_scan_levels_prev >= 0) {
					if (error_approx_fine[i_scan_levels_prev] != nullptr) {
						free(error_approx_fine[i_scan_levels_prev]);
						error_approx_fine[i_scan_levels_prev] = nullptr;
					}
					if (error_approx_coarse[i_scan_levels_prev] != nullptr) {
						free(error_approx_coarse[i_scan_levels_prev]);
						error_approx_coarse[i_scan_levels_prev] = nullptr;
					}
					if (residual_coarse[i_scan_levels_prev] != nullptr) {
						free(residual_coarse[i_scan_levels_prev]);
						residual_coarse[i_scan_levels_prev] = nullptr;
					}
				}
				if (residual_fine[i_scan_levels] != nullptr) {
					free(residual_fine[i_scan_levels]);
					residual_fine[i_scan_levels] = nullptr;
				}
			
		}
	}


	// ����� ����������.
	my_amg_manager.icoarseningtype = memo_icoarseningtype;


	

	if (diag != nullptr) {
		delete[] diag;
		diag = nullptr;
	}
	if (diag_minus_one != nullptr) {
		delete[] diag_minus_one;
		diag_minus_one = nullptr;
	}
	if (nested_desection != nullptr) {
		delete[] nested_desection;
		nested_desection = nullptr;
	}

	

	

	

	// ������������ ����������� ������.
	free_level_additional_data(milu0, ilevel);
	free_level_additional_data(milu2, ilevel);

	// ������������ ����� ������ � ILU ������.
	if (milu_gl_buffer.alu_copy != nullptr) delete[] milu_gl_buffer.alu_copy;
	if (milu_gl_buffer.jlu_copy != nullptr) delete[] milu_gl_buffer.jlu_copy;
	if (milu_gl_buffer.ju_copy != nullptr) delete[] milu_gl_buffer.ju_copy;
	milu_gl_buffer.alu_copy = nullptr;
	milu_gl_buffer.jlu_copy = nullptr;
	milu_gl_buffer.ju_copy = nullptr;


	if (residual_fine[0] != nullptr) {
		free(residual_fine[0]);
		residual_fine[0] = nullptr;
	}


	if (residual_fine != nullptr) {
		delete[] residual_fine;
		residual_fine = nullptr;
	}


	if (error_approx_fine != nullptr) {
		delete[] error_approx_fine;
		error_approx_fine = nullptr;
	}


	if (error_approx_coarse != nullptr) {
		delete[] error_approx_coarse;
		error_approx_coarse = nullptr;
	}


	if (residual_coarse != nullptr) {
		delete[] residual_coarse;
		residual_coarse = nullptr;
	}


	if (row_ptr_start != nullptr) {
		free(row_ptr_start);
		row_ptr_start = nullptr;
	}
	if (row_ptr_end != nullptr) {
		free(row_ptr_end);
		row_ptr_end = nullptr;
	}

	
	if (x_copy != nullptr) {
		free(x_copy);
		x_copy = nullptr;
	}
	if (x_old != nullptr) {
		free(x_old);
		x_old = nullptr;
	}
	if (x_best_search != nullptr) {
		free(x_best_search);
		x_best_search = nullptr;
	}

	// ��� ������������:

	if (row_ptr_start != nullptr) {
		free(row_ptr_start);
		row_ptr_start = nullptr;
	}
	if (row_ptr_end != nullptr) {
		free(row_ptr_end);
		row_ptr_end = nullptr;
	}


	if (x_jacoby_buffer != nullptr) {
		delete[] x_jacoby_buffer;
		x_jacoby_buffer = nullptr;
	}


	free_hash_table_Gus_struct01();

	return ret_value;
}


// ��������� ��������� ����� �������������� ����� �� �-���� � 
// F - ����. � -���� �������� ������ ����� ���������� ������ �����������.
// �������� �� ������� � F ����� ������ ���� ������������� �� �������� �������
// � ��������� � ����� (��. ������ ������������). 
template <typename doublerealT>
void Ruge_and_Stuben_CF_decomposition(Ak2& Amat, bool*& this_is_F_node,
	bool*& this_is_C_node, integer ilevel,
	integer*& count_neighbour, integer*& n_a,
	integer& newCcount,
	doublereal*& threshold_quick_only_negative,
	integer*& row_startA,
	Taccumulqtor_list**& hash_StrongTranspose_collection1,
	bool bprint_mesage_diagnostic,
	bool*& flag, integer iadd, 
	bool bpositive_connections_CF_decomp,
	bool bStrongTransposeON,
	bool* &hash_table2,
	integer* &istack, integer*& nnz_a,
	bool debug_reshime)
{

	

	integer ii_end1 = n_a[ilevel - 1];
	integer max_neighbour = 0;
	integer icandidate = 0;

	// ������� ���� � ���������� ������ ������� � ���������� ���.
	// ��� ������ ������������� ���� � ���������� ������ �������.
	// ��� ��������� ��� ���� ����� ��������� �������� C/F ���������.

	for (integer i7 = 1; i7 <= ii_end1; i7++) {
		if (count_neighbour[i7] > max_neighbour) {
			max_neighbour = count_neighbour[i7];
			icandidate = row_startA[i7];
		}
	}




	// ����� �������� ��� ����� � coarse, � ��� � ���� ��� ����� � Fine �������� ���� ��� �����������
	// � ������ Fine ��������� �� ������� �������.


	const integer NULL_NEIGHBOUR = -1;
	// ���������� C/F ���������.
	integer vacant = NULL_NEIGHBOUR;
	bool bcontinue_gl_1 = true;
	// ���������� C/F ���������.
	integer icountprohod = 0;

	// ������ �� ���� ������� ��� ���� �������� ��� ���������������.
	// �������� ��� ���� �������� ��� ������������.
	bool* bmarkervisit = nullptr;
	if (bmarkervisit != nullptr) {
		free(bmarkervisit);
		bmarkervisit = nullptr;
	}
	bmarkervisit = my_declaration_array<bool>(n_a[ilevel - 1], false, "bmarkervisit");


	// ���������� �������������� ����������� 
	// ����������� �������� ������������
	// ����� �������� ������ ������ ������������ flag.
	integer istartflag_scan = 1;
	
	// ������ 12mm hfet thermal resistance. 1.7��� �����������.
	// AVL_TREE_ID   3��� 29� 590��      {5}
	// SPLAY_TREE_ID  3��� 16� 430�� {2}
	// BINARY_HEAP 3��� 4� 0�� {1 *����� �������.}
	// RANDOM_TREE_ID (��������) 3��� 28� 90�� {4}
	// RED_BLACK_TREE_ID 3��� 27� 210�� {3}


	const integer AVL_TREE_ID = 0;   // ��� ������ ������. 12.12.2015.
	const integer SPLAY_TREE_ID = 1; // ��������� ������ ������.
	const integer BINARY_HEAP = 2; // �������� ����. 16.06.2017.
	const integer RANDOM_TREE_ID = 3; // (��������) ����������������� ������ ������. 24.08.2017.
	const integer RED_BLACK_TREE_ID = 4; // ������-׸���� ������ ������. 22.06.2018.
	const integer FIBONACCI_HEAP_ID = 5; // ������������ ����. 11.07.2018.
	const integer VAN_EMDE_BOAS_TREE_ID = 6; // ��� ���� ���� ������ ������. 30.06.2018
	//integer id_tree = BINARY_HEAP; // AVL_TREE_ID; // SPLAY_TREE_ID; // BINARY_HEAP; // RANDOM_TREE_ID; // RED_BLACK_TREE_ID;
	// 28.01.2018 �� ����� ������������.

	integer id_tree = my_amg_manager.iCFalgorithm_and_data_structure;

	integer n = n_a[0];

	// �������� ������ ��� �������� ����.
	// ���������� ���������� ��������� ��� ����� �� ������� ��������� ������� �����������.
	const integer isize_priority_queue01 = (integer)(0.4 * n); // 0.238
	integer ikonst1 = isize_priority_queue01, ikonst2 = n;
	if (id_tree != BINARY_HEAP) {
		ikonst1 = 0;
		ikonst2 = 0;
	}
	PQ<integer> binary_heap(ikonst1, ikonst2); // 500K ��� 2.1M

	// ������������ ����.
	FibonacciHeap<integer> fibo_heap;

	if (id_tree == FIBONACCI_HEAP_ID) {
		fibo_heap.WakeUp2(n + 1);// alloc memory hash table
	}

	node_AVL* root = 0;
	Tree_splay* root_splay = 0;
	size_splay_Tree = 0;
	TreapNode* random_tree_root = nullptr;
	RBtree RBroot; // ������ ������-׸����� ������.

	root = 0;
	root_splay = 0;
	size_splay_Tree = 0;
	random_tree_root = nullptr;

	if (id_tree == BINARY_HEAP) {
		binary_heap.clear();
	}


	RBroot.Clear();
	if (id_tree == FIBONACCI_HEAP_ID) {
		fibo_heap.UpdateSize(n_a[ilevel - 1] + 1);
	}

#if VEB_FLAG
	int64_t res_vanEMDE_BOAS_Tree;
	int64_t universe = 4294967296; // 2 ^32=2^(2^5) (4294 ���) ��������
	//int64_t universe = 67108864; // 2^26 �� ��������
	//int64_t universe = 134217728; // 2^27 �� ��������
	TvEB* vanEMDE_BOAS_Tree = nullptr;

	if (id_tree == VAN_EMDE_BOAS_TREE_ID) {
		vanEMDE_BOAS_Tree = new TvEB(universe);
	}
#endif

	newCcount = 0;

	


	// �������� ��������� �������� � ���������� ������ � ������� �����, 
	// ����� ������ �������� � ���� ���. ��. ��������� ������ ��� set.
	// 23.04.2017


	

	if (bprint_mesage_diagnostic) {
		std::cout << "   ***   CAMG SELECTOR "<< ilevel <<"  ***\n";
	}
	while (bcontinue_gl_1)
	{

		integer ic = 0;
		integer ic_end_F_SiTranspose = 0;

		integer ii = icandidate;
		if (flag[Amat.i[ii]] == false) {

			ic = 0; // ������������ �������������.


			ic_end_F_SiTranspose = 0;
			integer set0 = Amat.i[ii];




			//A20.05.2017//this_is_C_node[set[0]] = true;
			//A20.05.2017//bmarkervisit[set[0]] = true;
			this_is_C_node[set0] = true;
			bmarkervisit[set0] = true;

			doublerealT max_vnediagonal = -1.0; // ������������ �������� ������ ��� ������������� ��������. 
												// ��������� ������������ �������.
												// ���� set[0]==Amat.i[is0].
												// ���������� �������� ������������� ���������������� ��������, � 
												// ������ ���� ��� ���� ���� ������� ������ � ����� ���������� ����� ��������� �������.
												// 17 ������ 2016 ���������� ����������� ������������� ���������������� ��������.
												// ������������ ��������� � ����� ������ ������.
			integer ii_back = ii;
			while ((ii_back > iadd) && (Amat.i[ii_back] == set0)) ii_back--;
			ii_back++;

			doublerealT max_vnediagonal1 = -1.0e30;
			
			// ���� ������ �� ������������� ���������������� �������� �� �� ������� ����� ����� ��������� �� ������ �������,
			// � ����������� ��������� ����������.

			if (bpositive_connections_CF_decomp) {
				// 23_10_2016
				for (integer is0 = ii_back; (is0 <= row_startA[set0 + 1] - 1); is0++) {
					if (Amat.j[is0] != set0) {
						if (Amat.abs_aij[is0] > max_vnediagonal1) {
							max_vnediagonal1 = Amat.abs_aij[is0]; //i,j
							// ������� ���������� ��������� �� ������ �������,
							// ����� ��������� ����������.
							//if (Amat.j[is0] == set[0]) break; 
						}						
					}
				}
			}
			else {
				for (integer is0 = ii_back; (is0 <= row_startA[set0 + 1] - 1); is0++) {
					if (Amat.j[is0] != set0) {
						if (Amat.aij[is0] < 0.0) {
							if (Amat.abs_aij[is0] > max_vnediagonal1) {
								max_vnediagonal1 = Amat.abs_aij[is0]; //i,j
								// ������� ���������� ��������� �� ������ �������,
								// ����� ��������� ����������.
								//if (Amat.j[is0] == set[0]) break; 
							}							
						}
					}
				}
			}
			
			
			//max_vnediagonal = max_vnediagonal1;  // 1			
			// �������� ������ � �����������. -85%. �� ���������� ����� max_vnediagonal = -1.0;
			// 19 ������ 2016 ����������� ��� ����� ��� �����, �� ����� ��������� threshold
			// max_vnediagonal ������ ���� -1.0. ������ ��� �������� ������������ ��������� 
			// �������� ����������� � ��������� �������� ����������.
			max_vnediagonal = -1.0e30;  // ��� �����!!!											

			ic++;


			//  � set ������� � ������� � �� <ic ����� ��������� ����� ����� F.
			// 5.01.2017
			// 01.04.2017 ����������� F ������ �� Si_Transpose ������.
			if ((my_amg_manager.ipatch_number == 7) && (bStrongTransposeON)) {

				integer imarker75_scan = 0;

				// ������� �������� ������.
				formirate_F_SiTranspose_hash_table_Gus2_struct02(hash_StrongTranspose_collection1[Amat.i[ii]], imarker75_scan, this_is_F_node, this_is_C_node);

				ic = imarker75_scan + 1;
			}

			ic_end_F_SiTranspose = ic; // � ���� ������� ������������� F ������� �� Si_Transpose.

									   // ���� ���� j ��� �� ��� �������� � �������.
			if (bpositive_connections_CF_decomp) {
				if (flag[Amat.j[ii]] == false) {
					if ((Amat.j[ii] != set0) && (Amat.abs_aij[ii] >= theta(ilevel) * max_vnediagonal)) {
						// 21.05.2017
						bool bfound_vacant = false;

						bfound_vacant = isfound_hash_table_Gus_struct01(Amat.j[ii]);
						if (!bfound_vacant) {
							insert_hash_table_Gus_struct01(Amat.j[ii]);
							ic++;
						}

					}
				}
			}
			else {
				if (flag[Amat.j[ii]] == false) {
					if ((Amat.j[ii] != set0) && (Amat.aij[ii] < 0.0) && (Amat.abs_aij[ii] >= theta(ilevel) * max_vnediagonal)) {
						// 21.05.2017
						bool bfound_vacant = false;

						bfound_vacant = isfound_hash_table_Gus_struct01(Amat.j[ii]);
						if (!bfound_vacant) {
							insert_hash_table_Gus_struct01(Amat.j[ii]);
							ic++;
						}

					}
				}
			}

			//std::cout<<"sboi start";

			// iscan = ii+1; // ���������� ���
			integer iscan = ii_back + 1; // ������ ����������� 19 ������ 2016�.
								 // TODO 19 jan 2016.

			if (bpositive_connections_CF_decomp) {
				while ((iscan <= nnz_a[ilevel - 1] + iadd) && (Amat.i[iscan] == set0)) {
					// 14 ������� 2016 ��� ������ ���������� � ����.
					//while (iscan <= row_startA[set0 + 1] - 1) { // ��� ������ ���������� � ���� �� ���������� �������.
					// ���� ���� j ��� �� ��� �������� � �������.
					if (flag[Amat.j[iscan]] == false) {
						if ((Amat.j[iscan] != set0) && (Amat.abs_aij[iscan] >= theta(ilevel) * max_vnediagonal)) {
							// 21.05.2017
							bool bfound_vacant = false;

							bfound_vacant = isfound_hash_table_Gus_struct01(Amat.j[iscan]);
							if (!bfound_vacant) {
								insert_hash_table_Gus_struct01(Amat.j[iscan]);
								ic++;
							}

							/*
							// ��������� ������ � �������� �������.
							vacant = Amat.j[iscan];
							for (integer js = 0; js < ic; js++) {
							if (vacant == set[js]) {
							vacant = NULL_NEIGHBOUR;
							}
							}
							if (vacant != NULL_NEIGHBOUR) {
							set[ic] = vacant;

							ic++;

							}
							*/
						}
					}

					iscan++;

				} // while
			}
			else {
				while ((iscan <= nnz_a[ilevel - 1] + iadd) && (Amat.i[iscan] == set0)) {
					// 14 ������� 2016 ��� ������ ���������� � ����.
					//while (iscan <= row_startA[set0 + 1] - 1) { // ��� ������ ���������� � ���� �� ���������� �������.
					// ���� ���� j ��� �� ��� �������� � �������.
					if (flag[Amat.j[iscan]] == false) {
						if ((Amat.j[iscan] != set0) && (Amat.aij[iscan] < 0.0) && (Amat.abs_aij[iscan] >= theta(ilevel) * max_vnediagonal)) {
							// 21.05.2017
							bool bfound_vacant = false;

							bfound_vacant = isfound_hash_table_Gus_struct01(Amat.j[iscan]);
							if (!bfound_vacant) {
								insert_hash_table_Gus_struct01(Amat.j[iscan]);
								ic++;
							}

							/*
							vacant = Amat.j[iscan];
							for (integer js = 0; js < ic; js++) {
							if (vacant == set[js]) {
							vacant = NULL_NEIGHBOUR;
							}
							}
							if (vacant != NULL_NEIGHBOUR) {
							set[ic] = vacant;

							ic++;

							}
							*/
						}
					}

					iscan++;

				} // while
			}

			//std::cout<<"sboi end";
			// ��� ���� ������ ������ ����� i,j






		// � ���� ����� ��������� set ������� ������������:
		// 1. ����������� �� root_Gus_set � set.
		// 2. root_Gus_set ������ �� ������������.
		// 3. ������ ����� ���� �������� ������ ��� set.
			integer* set = nullptr;
			set = new integer[ic + 2];
			//if (set == nullptr) {
				//std::cout<<"error!!! memory for set is nullptr. Problem allocate detected."<<std::endl;
				//std::cout<<"in function classic_aglomerative_amg6."<<std::endl;
				//system("pause");
				//exit(1);
			//}

			integer ic_986 = 1;
			set[0] = set0;



			formirate_hash_table_Gus_struct01__2__set(set, ic_986);

			clear_hash_table_Gus_struct01();


			for (integer isc = 1; isc < ic; isc++) {
				this_is_F_node[set[isc]] = true; // ��� ������ ����� F ����.
				bmarkervisit[set[isc]] = true;
			}




			// �������� ���� ��� ���������� � �������.
			for (integer js = 0; js < ic; js++) {
				flag[set[js]] = true;
			}






			// �������� (5 ������� 2015 revised) 
			// 1. ��������� ��� F ������� ������ ������� � �� ������ �������.
			// 2. ��� ������� �������������� F ��������� ��� "��������" �������.
			// 3. ���� ���� ��� �� ��� ������� � ������� �� ���� ���� ������� ������� ���� �� ������� 
			// ��������� � ������������� ������� F �� ������ 1.




			TreapNode* nrt_temp = nullptr;
			TreapNode* save_root = nullptr;

			/*
			if (id_tree == FIBONACCI_HEAP_ID) {
			if (!fibo_heap.isEmpty()) {
			fibo_heap.removeMinimum();
			}
			}
			*/
			// 12 ������� 2015.
			// ���� ������� �� ��� ������ C � F ����.
			// ��� �������� ������� ��� ������ � �������� ��� �
			// �������� ���������. �������� �������������� � ������ �����
			// ������������ ���������. �������� ������������ �� ���������������
			// �� ��������� 2  ����� �� ���������� ��������� � ������
			// ������������������ ������ ��� ���� �����������.
			for (integer js = 0; js < ic; js++) {
				data_BalTree ddel;
				ddel.i = set[js];
				ddel.count_neighbour = count_neighbour[set[js]];
				// ���������� ���� ��� ������ ��� ���� �����.
#if VEB_FLAG
				integer  veb_del_key = (count_neighbour[set[js]]) * (n_a[ilevel - 1] + 1) + (set[js]);
				if (id_tree == VAN_EMDE_BOAS_TREE_ID) {
					if (veb_del_key > universe - 2) {
						std::cout<<"overflow veb-Van Emde Boas 2^2^5"<<std::endl;
						system("PAUSE");
					}
					if (veb_del_key < 1) {
						std::cout<<"overflow veb-Van Emde Boas < 1"<<std::endl;
						system("PAUSE");
					}
				}
#endif
				//ddel.ii = row_startA[ddel.i];
				switch (id_tree) {
				case AVL_TREE_ID: root = remove_AVL(root, ddel);
					break;
				case SPLAY_TREE_ID: root_splay = delete_splay_Tree(ddel, root_splay);
					break;
				case BINARY_HEAP:
					// ���������� ������ �������� �������� set[js].
					binary_heap.remove(set[js]);
					break;
				case RANDOM_TREE_ID:
					save_root = random_tree_root;
					nrt_temp = search(random_tree_root, ddel);
					random_tree_root = save_root;
					save_root = nullptr;
					if (nrt_temp != nullptr) {
						nrt_temp = nullptr;
						random_tree_root = deleteNode(random_tree_root, ddel);
					}
					break;
				case RED_BLACK_TREE_ID:
					RBroot.Remove(ddel);
					break;
				case FIBONACCI_HEAP_ID:
					if (!fibo_heap.isEmpty()) {
						fibo_heap.deleteKey(ddel);
					}
					break;
				case VAN_EMDE_BOAS_TREE_ID:
#if VEB_FLAG
					// ���� ������� ������������ �� �� ��� ������.
					res_vanEMDE_BOAS_Tree = vEB_find(vanEMDE_BOAS_Tree, veb_del_key);
					if (!res_vanEMDE_BOAS_Tree) {

					}
					else {
						res_vanEMDE_BOAS_Tree = vEB_delete(vanEMDE_BOAS_Tree, veb_del_key);
						if (!res_vanEMDE_BOAS_Tree) {
							std::cout<<"cannot be deleted post factum delete ddel.count_neighbour=="<<ddel.count_neighbour<<", ddel.i=="<<ddel.i<<std::endl;
							system("PAUSE");
						}
					}
#endif
					break;
				default: root = remove_AVL(root, ddel);
					break;
				}
			}




			//std::cout<<"additional and modify new neighbour"<<std::endl;

			// 10 ������ 2016. ����� ������.
			// �������� ��������� ��������� ����������� (��� ������ ������� �������� �� ��� ������).
			// ��� ����������� ��� ���������� ���������� V ������ ������� ��������� �� ����������
			// ��� ����������� �������� ������ � ������������ ��������� � ���������� ��� ��� ����������.
			// �� ������ 13 ������ 2016 ��� ������ ������� �� �������� ����������.
			integer itop_stack2 = 0;

			// 10 ������ 2016. ������ ������� ������ ��������� �� ����������� ����.
			for (integer js = 1; js < ic; js++) {

				integer i_11 = set[js];
				integer ii_11 = row_startA[i_11];
				//integer iend5 = nnz_a[ilevel - 1] + iadd;
				//integer istart73 = ii_11;
				//while ((istart73 >= 1 + iadd) && (Amat.i[istart73] == Amat.i[ii_11])) istart73--;
				//istart73++;
				integer istart73 = row_startA[Amat.i[ii_11]];
				integer iend73 = row_startA[Amat.i[ii_11] + 1] - 1;
				bool bvisitsos = false;
				for (integer is0 = istart73; (is0 <= iend73); is0++) {
					//for (integer is0 = istart73; (is0 <= iend5) && (Amat.i[is0] == Amat.i[ii_11]); is0++) {
					// � ����������� � U!!!
					if (flag[Amat.j[is0]] == false) {


						integer isc = Amat.j[is0];



						// ����������� �� ��������� �������������.
						// � 2D �� ������������ ������� ��������� ������������� ����������
						// ����� 33%.
						// ��� ��� ����������� �������� ����������, �������� � �������, ��
						// �� ���� �������� ����� ��� ����� ������� ����������� ��������� �����
						// ����� (������������ �� ������, �� ������� ���� ������������ ������� �������� �������).
						// ��� �������� ���������� �� �������:
						// CGHV1J006D, ��������� ����, ������������� ��������� � FET, Module 2.
						// ������ �������� ����������� ���������� ������� ������� ��-�� ������� ������� ������������.
						// �������� �� � ���� ����� ����.
						if (hash_table2[isc] == false) {
							hash_table2[isc] = true;
							istack[itop_stack2] = isc;
							itop_stack2++;
							// ������������������ �����.
							//}

							//21_12_2016
							integer ii_2 = row_startA[isc];


							integer ic2 = 0;
							integer iend2loc = nnz_a[ilevel - 1] + iadd;
							//integer istart72 = ii_2;										
							integer istart72 = row_startA[Amat.i[ii_2]];
							integer istopmarker2 = row_startA[Amat.i[ii_2] + 1] - 1;

							// 22 _12_2016
							// ��� ������ �������: ������������ ���������� ���������� ��������
							// ������� �� ������ passive module 6 � �� ����� ��� ��� ��������� 
							// �������� �� ����� ������� ������ ����.
							doublerealT max_vnediagonal33 = -1.0e30;
							for (integer is01 = istart72; (is01 <= istopmarker2); is01++) {
								if (Amat.j[is01] != Amat.i[is01]) {
									if ((Amat.aij[is01] < 0.0) && (Amat.abs_aij[is01] > max_vnediagonal33)) {
										max_vnediagonal33 = Amat.abs_aij[is01];
									}
								}
							}
							for (integer is01 = istart72; (is01 <= istopmarker2); is01++) {
								// 0.2375 ����������� ��������� ��� passive module 6.
								if ((Amat.aij[is01] < 0.0) && (Amat.abs_aij[is01] > 0.2375 * max_vnediagonal33)) {
									if (Amat.j[is01] == set[js]) {
										if ((my_amg_manager.ipatch_number == 7) && (bStrongTransposeON)) {
											if (js < ic_end_F_SiTranspose) {
												// ����������� �������� ������ ��� ������� F ����� �������
												// �������� �������� F ����� ������� ���� �������� �� Si_Transpose ������.
												// ������ ��� �������� � ����� ���� � ������ �������.
												ic2++;
											}
										}
										else {
											ic2++;
										}
									}
								}
								// ��������� ������� ������� (weakly) ������ ?
							}


							data_BalTree dsearch;
							dsearch.count_neighbour = count_neighbour[isc];
							//dsearch.ii = ii_2;
							dsearch.i = isc;
							// ����������� �� ���������� ������ � ������ F ������.
							count_neighbour[isc] += ic2;
							data_BalTree dadd;
							dadd.count_neighbour = count_neighbour[isc];
							//dadd.ii = ii_2;
							dadd.i = isc;

							// ���������� ���� ��� ������ ��� ���� �����.
							integer  veb_dadd_key = (dadd.count_neighbour) * (n_a[ilevel - 1] + 1) + (dadd.i);
							integer  veb_dsearch_key = (dsearch.count_neighbour) * (n_a[ilevel - 1] + 1) + (dsearch.i);
							//integer  veb_dadd_key = (dadd.count_neighbour)*(n + 1) + (dadd.i);
							//integer  veb_dsearch_key = (dsearch.count_neighbour)*(n + 1) + (dsearch.i);

#if VEB_FLAG
							if (id_tree == VAN_EMDE_BOAS_TREE_ID) {
								if (veb_dadd_key > universe - 2) {
									std::cout<<"overflow veb-Van Emde Boas 2^2^5"<<std::endl;
									system("PAUSE");
								}
								if (veb_dsearch_key > universe - 2) {
									std::cout<<"overflow veb-Van Emde Boas 2^2^5"<<std::endl;
									system("PAUSE");
								}
							}
#endif
							if (veb_dadd_key < 1) {
								std::cout<<"overflow veb-Van Emde Boas <1 "<<std::endl;
								system("PAUSE");
							}
							if (veb_dsearch_key < 1) {
								std::cout<<"overflow veb-Van Emde Boas <1 "<<std::endl;
								system("PAUSE");
							}


							TreapNode* nrt_temp_1 = nullptr;
							TreapNode* save_root_1 = nullptr;

							// ��������� ������� � ��� ������,
							// ������ ���� ������� ��� ��������� � ������ �� �� ��������������.
							// 12 ������� 2015.
							// ���������� ���� ���������� �� ��������������� �� ��������� 2 �����,
							// ������ ����� ���������� ������ ������� ����������������.
							// �.�. ��������-�������� � �.�. ������ 1962.
							switch (id_tree)
							{
							case AVL_TREE_ID: root = insert_and_modify(root, dadd, dsearch);
								break;
							case SPLAY_TREE_ID: root_splay = insert_and_modify(root_splay, dadd, dsearch);
								break;
							case BINARY_HEAP:
								if (binary_heap.isfound(isc)) {
									// ������
									// ������� ������������ ������� � ��������� �����.
									binary_heap.remove(isc);
									// ������������ ������� ������ ��������.
									binary_heap.insert(count_neighbour[isc], isc);
								}
								else {
									// �����������.
									// ������������ ������� ������ ��������.
									binary_heap.insert(count_neighbour[isc], isc);
								}
								break;
							case RANDOM_TREE_ID:
								nrt_temp_1 = nullptr;
								save_root_1 = random_tree_root;
								nrt_temp_1 = search(random_tree_root, dsearch);
								random_tree_root = save_root_1;
								save_root_1 = nullptr;
								if (nrt_temp_1 == nullptr) {
									// ������� � ������ �����������.
									random_tree_root = insert(random_tree_root, dadd);
								}
								else {
									nrt_temp_1 = nullptr;
									// ��������
									random_tree_root = deleteNode(random_tree_root, dsearch);
									// �������
									random_tree_root = insert(random_tree_root, dadd);
								}
								break;
							case RED_BLACK_TREE_ID:
								RBroot.InsertAndModify(dadd, dsearch);
								break;
							case FIBONACCI_HEAP_ID:
								fibo_heap.insert_and_modify(-veb_dsearch_key, -veb_dadd_key);
								break;
							case VAN_EMDE_BOAS_TREE_ID:
#if VEB_FLAG
								res_vanEMDE_BOAS_Tree = vEB_find(vanEMDE_BOAS_Tree, veb_dsearch_key);
								if (!res_vanEMDE_BOAS_Tree) {
									// �� ������
									res_vanEMDE_BOAS_Tree = vEB_find(vanEMDE_BOAS_Tree, veb_dadd_key);
									if (!res_vanEMDE_BOAS_Tree) {
										// �� ������
										res_vanEMDE_BOAS_Tree = vEB_insert(vanEMDE_BOAS_Tree, veb_dadd_key);
										if (!res_vanEMDE_BOAS_Tree) {
											std::cout<<"insert problem veb veb_dadd_key=="<< veb_dadd_key<<std::endl;
										}
									}
								}
								else {
									res_vanEMDE_BOAS_Tree = vEB_delete(vanEMDE_BOAS_Tree, veb_dsearch_key);
									if (!res_vanEMDE_BOAS_Tree) {
										std::cout<<"cannot be deleted post factum delete veb_dsearch_key=="<< veb_dsearch_key<<std::endl;
										system("PAUSE");
									}
									// ������, ������ � �������� == �������.
									res_vanEMDE_BOAS_Tree = vEB_insert(vanEMDE_BOAS_Tree, veb_dadd_key);
									if (!res_vanEMDE_BOAS_Tree) {
										std::cout<<"insert problem veb veb_dadd_key=="<< veb_dadd_key<<std::endl;
									}
								}
#endif
								break;
							default: root = insert_and_modify(root, dadd, dsearch);
								break;
							}




						}


					}

				}
			}

			// ������� (�������������� ���-�������).
			// �� � ���� ������ �� ����������� �� OPENMP � ���� �����.!!!
			for (integer i_54 = 0; i_54 < itop_stack2; i_54++) {
				hash_table2[istack[i_54]] = false;
			}
			itop_stack2 = 0; // ���� ����� ����� � ������.





			if (set != nullptr) {
				delete[] set;
				set = nullptr;
			}

			newCcount++;
			// ���� ������� ������.

		} // ���� �� ��� ��� ������� � �������.



		bcontinue_gl_1 = false;
		for (integer i_1 = istartflag_scan; i_1 <= n_a[ilevel - 1]; i_1++) {
			if (flag[i_1] == false) {
				bcontinue_gl_1 = true;
				istartflag_scan = i_1; // ��������� ������� ������������.
				break; // ��������� ����� �� ����� for.
			}
		}

		// ���������� ���� � ������������ ����������� �������.
		icandidate = 0;


		// ������ ��� ����������� ���������.
		// ���� ����� ������������ ������� � ��� ������.
		node_AVL* emax = 0;
		Tree_splay* emax_splay = 0;
		TreapNode* emax_random_tree = nullptr;
		TreapNode* save_root = nullptr;
		data_BalTree dbt_emax;

		integer ui_emax;

		switch (id_tree)
		{
		case AVL_TREE_ID: emax = findmax(root);
			break;
		case SPLAY_TREE_ID: emax_splay = findmax(root_splay);
			break;
		case BINARY_HEAP:
			if (!binary_heap.empty()) {
				// ���� �� �����.
				icandidate = row_startA[binary_heap.readkeymaxelm()];
			}
			else {
				size_splay_Tree = 0;
				icandidate = 0;
				bcontinue_gl_1 = false;
			}
			break;
		case RANDOM_TREE_ID:
			save_root = random_tree_root;
			if (emax_random_tree != nullptr) {
				delete[] emax_random_tree;
				emax_random_tree = nullptr;
			}
			emax_random_tree = findmax_random_tree(random_tree_root);
			random_tree_root = save_root;
			save_root = nullptr;

			break;
		case RED_BLACK_TREE_ID:
			dbt_emax = RBroot.GetMaxElm();
			break;
		case FIBONACCI_HEAP_ID:
			if (fibo_heap.isEmpty()) {
				dbt_emax.i = -1;
			}
			else {
				ui_emax = -fibo_heap.getMinimum();
				dbt_emax.i = ((ui_emax) % (n_a[ilevel - 1] + 1));
				dbt_emax.count_neighbour = ((ui_emax) / (n_a[ilevel - 1] + 1));
			}
			break;
		case VAN_EMDE_BOAS_TREE_ID:
#if VEB_FLAG
			if (!((vanEMDE_BOAS_Tree == nullptr) || ((vanEMDE_BOAS_Tree->summary == nullptr) && (vanEMDE_BOAS_Tree->cluster == nullptr)))) {
				vEB_max(vanEMDE_BOAS_Tree, ui_emax);
				if (ui_emax <= 0) {
					// ������ ��� ���� ����� ������.
					dbt_emax.i = -1;
					dbt_emax.count_neighbour = -1;
				}
				else {
					dbt_emax.i = ((ui_emax) % (n_a[ilevel - 1] + 1));
					dbt_emax.count_neighbour = ((ui_emax) / (n_a[ilevel - 1] + 1));
				}

			}
			else {
				// ������ ��� ���� ����� ������.
				dbt_emax.i = -1;
				dbt_emax.count_neighbour = -1;
			}
#endif
			break;
		default: emax = findmax(root);
			break;
		}


		switch (id_tree) {
		case AVL_TREE_ID:
			// AVL tree
			if (emax != 0) {

				//icandidate = emax->key.ii; 23 jan 2016
				icandidate = row_startA[emax->key.i];
				emax = 0;
			}
			else {
				root = 0;
				icandidate = 0;
				bcontinue_gl_1 = false;

			}
			break;
		case SPLAY_TREE_ID:
			// SPLAY tree
			if (emax_splay != 0) {


				//icandidate = emax_splay->item.ii; 23 jan 2016
				icandidate = row_startA[emax_splay->item.i];
				emax_splay = 0;

			}
			else {
				RBroot.Clear();
				root_splay = 0;
				size_splay_Tree = 0;
				random_tree_root = nullptr;
				icandidate = 0;
				bcontinue_gl_1 = false;

			}
			break;
		case BINARY_HEAP:
			break;
		case RANDOM_TREE_ID:
			// Random TREE
			if (emax_random_tree != nullptr) {
				icandidate = row_startA[emax_random_tree->key.i];
				delete emax_random_tree;
				emax_random_tree = nullptr;
			}
			else {
				RBroot.Clear();
				root_splay = 0;
				size_splay_Tree = 0;
				random_tree_root = nullptr;
				icandidate = 0;
				bcontinue_gl_1 = false;
			}
			break;
		case RED_BLACK_TREE_ID:
			if (RBroot.Find(dbt_emax)) {
				icandidate = row_startA[dbt_emax.i];
			}
			else {
				RBroot.Clear();
				root_splay = 0;
				size_splay_Tree = 0;
				random_tree_root = nullptr;
				icandidate = 0;
				bcontinue_gl_1 = false;
			}
			break;
		case FIBONACCI_HEAP_ID:
			if (dbt_emax.i == -1)
			{
				// ������ �����.
				RBroot.Clear();
				root_splay = 0;
				size_splay_Tree = 0;
				random_tree_root = nullptr;
				icandidate = 0;
				bcontinue_gl_1 = false;
			}
			else {
				// ������� ���� � ������ ��� ���� ����� �� �����.
				if (!fibo_heap.isEmpty()) {
					fibo_heap.removeMinimum();
				}
				icandidate = row_startA[dbt_emax.i];
				//std::cout << "row_startA = " << icandidate<< " " << dbt_emax.i << std::endl;

			}
			break;
		case VAN_EMDE_BOAS_TREE_ID:
			if (dbt_emax.i == -1)
			{
				// ������ �����.
				RBroot.Clear();
				root_splay = 0;
				size_splay_Tree = 0;
				random_tree_root = nullptr;
				icandidate = 0;
				bcontinue_gl_1 = false;
			}
			else {
				// ������� ���� � ������ ��� ���� ����� �� �����.
				icandidate = row_startA[dbt_emax.i];
				//std::cout << "row_startA = "<< icandidate << std::endl;
			}
			break;
		default:
			// AVL tree
			if (emax != 0) {

				//icandidate = emax->key.ii; 23 jan 2016
				icandidate = row_startA[emax->key.i];
				emax = 0;
			}
			else {
				root = 0;
				icandidate = 0;
				bcontinue_gl_1 = false;

			}
			break;
		}



		if (debug_reshime) system("pause");
		//getchar();

		if ((icandidate == 0) ) {
			bcontinue_gl_1 = false;
		}
		// 4 june 2016

		icountprohod++;

	} // ���������� C/F ���������. �������.


// ������������ ����������� ������ �� ��� ��� ������.
			// 12 ������� 2015.
	switch (id_tree)
	{
	case AVL_TREE_ID: clear_AVL(root);
		root = 0;
		break;
	case SPLAY_TREE_ID:
		clear_SPLAY(root_splay);
		root_splay = 0;
		break;
	case BINARY_HEAP:
		binary_heap.clear();
		break;
	case RANDOM_TREE_ID:
		clear_random_tree(random_tree_root);
		random_tree_root = nullptr;
		break;
	case RED_BLACK_TREE_ID:
		RBroot.Clear();
		break;
	case VAN_EMDE_BOAS_TREE_ID:
#if VEB_FLAG
		if (!((vanEMDE_BOAS_Tree == nullptr) || ((vanEMDE_BOAS_Tree->summary == nullptr) && (vanEMDE_BOAS_Tree->cluster == nullptr)))) {
			vanEMDE_BOAS_Tree->~TvEB();
		}
#endif
		break;
	default: clear_AVL(root);
		root = 0;
		break;
	}

	//delete[] bmarkervisit;
	if (bmarkervisit != nullptr) {
		free(bmarkervisit);
		bmarkervisit = nullptr;
	}


} // Ruge_and_Stuben_CF_decomposition

// comparator function to make min heap 
struct greaters_Stuben {
	bool operator()(const std::pair<integer, integer>& a, const std::pair<integer, integer>& b) const {
		return a.second < b.second;
	}
};

#include <map>    //���������� ���������� ��� ������ � map
//#include <iomanip>

// ��������� ��������� ����� �������������� ����� �� �-���� � 
// F - ����. � -���� �������� ������ ����� ���������� ������ �����������.
// �������� �� ������� � F ����� ������ ���� ������������� �� �������� �������
// � ��������� � ����� (��. ������ ������������). 
template <typename doublerealT>
void Ruge_and_Stuben_CF_decomposition_std(Ak2& Amat, bool*& this_is_F_node,
	bool*& this_is_C_node, integer ilevel,
	integer*& count_neighbour, integer*& n_a,
	integer& newCcount,
	doublereal*& threshold_quick_only_negative,
	integer*& row_startA,
	Taccumulqtor_list**& hash_StrongTranspose_collection1,
	bool bprint_mesage_diagnostic,
	bool*& flag, integer iadd,
	bool bpositive_connections_CF_decomp,
	bool bStrongTransposeON,
	bool*& hash_table2,
	integer*& istack, integer*& nnz_a,
	bool debug_reshime)
{



	integer ii_end1 = n_a[ilevel - 1];
	integer max_neighbour = 0;
	integer icandidate = 0;

	// ������� ���� � ���������� ������ ������� � ���������� ���.
	// ��� ������ ������������� ���� � ���������� ������ �������.
	// ��� ��������� ��� ���� ����� ��������� �������� C/F ���������.

	for (integer i7 = 1; i7 <= ii_end1; i7++) {
		if (count_neighbour[i7] > max_neighbour) {
			max_neighbour = count_neighbour[i7];
			icandidate = row_startA[i7];
		}
	}




	// ����� �������� ��� ����� � coarse, � ��� � ���� ��� ����� � Fine �������� ���� ��� �����������
	// � ������ Fine ��������� �� ������� �������.


	const integer NULL_NEIGHBOUR = -1;
	// ���������� C/F ���������.
	integer vacant = NULL_NEIGHBOUR;
	bool bcontinue_gl_1 = true;
	// ���������� C/F ���������.
	integer icountprohod = 0;

	// ������ �� ���� ������� ��� ���� �������� ��� ���������������.
	// �������� ��� ���� �������� ��� ������������.
	bool* bmarkervisit = nullptr;
	if (bmarkervisit != nullptr) {
		free(bmarkervisit);
		bmarkervisit = nullptr;
	}
	bmarkervisit = my_declaration_array<bool>(n_a[ilevel - 1], false, "bmarkervisit");


	// ���������� �������������� ����������� 
	// ����������� �������� ������������
	// ����� �������� ������ ������ ������������ flag.
	integer istartflag_scan = 1;

	// ������ 12mm hfet thermal resistance. 1.7��� �����������.
	// AVL_TREE_ID   3��� 29� 590��      {5}
	// SPLAY_TREE_ID  3��� 16� 430�� {2}
	// BINARY_HEAP 3��� 4� 0�� {1 *����� �������.}
	// RANDOM_TREE_ID (��������) 3��� 28� 90�� {4}
	// RED_BLACK_TREE_ID 3��� 27� 210�� {3}

	integer n = n_a[0];

	
	std::map<integer, integer> m1;
	//std::vector<std::pair<integer, integer>> v1;
	
	newCcount = 0;

	// �������� ��������� �������� � ���������� ������ � ������� �����, 
	// ����� ������ �������� � ���� ���. ��. ��������� ������ ��� set.
	// 23.04.2017


	if (bprint_mesage_diagnostic) {
		std::cout << "   ***   CAMG SELECTOR " << ilevel <<"  ***\n";
	}
	while (bcontinue_gl_1)
	{

		integer ic = 0;
		integer ic_end_F_SiTranspose = 0;

		integer ii = icandidate;
		if (flag[Amat.i[ii]] == false) {

			ic = 0; // ������������ �������������.


			ic_end_F_SiTranspose = 0;
			integer set0 = Amat.i[ii];




			//A20.05.2017//this_is_C_node[set[0]] = true;
			//A20.05.2017//bmarkervisit[set[0]] = true;
			this_is_C_node[set0] = true;
			bmarkervisit[set0] = true;

			doublerealT max_vnediagonal = -1.0; // ������������ �������� ������ ��� ������������� ��������. 
												// ��������� ������������ �������.
												// ���� set[0]==Amat.i[is0].
												// ���������� �������� ������������� ���������������� ��������, � 
												// ������ ���� ��� ���� ���� ������� ������ � ����� ���������� ����� ��������� �������.
												// 17 ������ 2016 ���������� ����������� ������������� ���������������� ��������.
												// ������������ ��������� � ����� ������ ������.
			integer ii_back = ii;
			while ((ii_back > iadd) && (Amat.i[ii_back] == set0)) ii_back--;
			ii_back++;

			doublerealT max_vnediagonal1 = -1.0e30;
						

			// ���� ������ �� ������������� ���������������� �������� �� �� ������� ����� ����� ��������� �� ������ �������,
			// � ����������� ��������� ����������.

			if (bpositive_connections_CF_decomp) {
				// 23_10_2016
				for (integer is0 = ii_back; (is0 <= row_startA[set0 + 1] - 1); is0++) {
					if (Amat.j[is0] != set0) {
						
						if (Amat.abs_aij[is0] > max_vnediagonal1) {
							max_vnediagonal1 = Amat.abs_aij[is0]; //i,j
							// ������� ���������� ��������� �� ������ �������,
							// ����� ��������� ����������.
							//if (Amat.j[is0] == set[0]) break; 
						}						
					}
				}
			}
			else {
				for (integer is0 = ii_back; (is0 <= row_startA[set0 + 1] - 1); is0++) {
					if (Amat.j[is0] != set0) {
						if (Amat.aij[is0] < 0.0) {
							
							if (Amat.abs_aij[is0] > max_vnediagonal1) {
								max_vnediagonal1 = Amat.abs_aij[is0]; //i,j
								// ������� ���������� ��������� �� ������ �������,
								// ����� ��������� ����������.
								//if (Amat.j[is0] == set[0]) break; 
							}							
						}
					}
				}
			}
			
			
			//max_vnediagonal = max_vnediagonal1;  // 1			
			// �������� ������ � �����������. -85%. �� ���������� ����� max_vnediagonal = -1.0;
			// 19 ������ 2016 ����������� ��� ����� ��� �����, �� ����� ��������� threshold
			// max_vnediagonal ������ ���� -1.0. ������ ��� �������� ������������ ��������� 
			// �������� ����������� � ��������� �������� ����������.
			max_vnediagonal = -1.0e30;  // ��� �����!!!											

			ic++;


			//  � set ������� � ������� � �� <ic ����� ��������� ����� ����� F.
			// 5.01.2017
			// 01.04.2017 ����������� F ������ �� Si_Transpose ������.
			if ((my_amg_manager.ipatch_number == 7) && (bStrongTransposeON)) {

				integer imarker75_scan = 0;

				// ������� �������� ������.
				formirate_F_SiTranspose_hash_table_Gus2_struct02(hash_StrongTranspose_collection1[Amat.i[ii]], imarker75_scan, this_is_F_node, this_is_C_node);

				ic = imarker75_scan + 1;
			}

			ic_end_F_SiTranspose = ic; // � ���� ������� ������������� F ������� �� Si_Transpose.

									   // ���� ���� j ��� �� ��� �������� � �������.
			if (bpositive_connections_CF_decomp) {
				if (flag[Amat.j[ii]] == false) {
					if ((Amat.j[ii] != set0) && (Amat.abs_aij[ii] >= theta(ilevel) * max_vnediagonal)) {
						// 21.05.2017
						bool bfound_vacant = false;

						bfound_vacant = isfound_hash_table_Gus_struct01(Amat.j[ii]);
						if (!bfound_vacant) {
							insert_hash_table_Gus_struct01(Amat.j[ii]);
							ic++;
						}

					}
				}
			}
			else {
				if (flag[Amat.j[ii]] == false) {
					if ((Amat.j[ii] != set0) && (Amat.aij[ii] < 0.0) && (Amat.abs_aij[ii] >= theta(ilevel) * max_vnediagonal)) {
						// 21.05.2017
						bool bfound_vacant = false;

						bfound_vacant = isfound_hash_table_Gus_struct01(Amat.j[ii]);
						if (!bfound_vacant) {
							insert_hash_table_Gus_struct01(Amat.j[ii]);
							ic++;
						}

					}
				}
			}

			//std::cout << "sboi start";

			// iscan = ii+1; // ���������� ���
			integer iscan = ii_back + 1; // ������ ����������� 19 ������ 2016�.
								 // TODO 19 jan 2016.

			if (bpositive_connections_CF_decomp) {
				while ((iscan <= nnz_a[ilevel - 1] + iadd) && (Amat.i[iscan] == set0)) {
					// 14 ������� 2016 ��� ������ ���������� � ����.
					//while (iscan <= row_startA[set0 + 1] - 1) { // ��� ������ ���������� � ���� �� ���������� �������.
					// ���� ���� j ��� �� ��� �������� � �������.
					if (flag[Amat.j[iscan]] == false) {
						if ((Amat.j[iscan] != set0) && (Amat.abs_aij[iscan] >= theta(ilevel) * max_vnediagonal)) {
							// 21.05.2017
							bool bfound_vacant = false;

							bfound_vacant = isfound_hash_table_Gus_struct01(Amat.j[iscan]);
							if (!bfound_vacant) {
								insert_hash_table_Gus_struct01(Amat.j[iscan]);
								ic++;
							}

							/*
							// ��������� ������ � �������� �������.
							vacant = Amat.j[iscan];
							for (integer js = 0; js < ic; js++) {
							if (vacant == set[js]) {
							vacant = NULL_NEIGHBOUR;
							}
							}
							if (vacant != NULL_NEIGHBOUR) {
							set[ic] = vacant;

							ic++;

							}
							*/
						}
					}

					iscan++;

				} // while
			}
			else {
				while ((iscan <= nnz_a[ilevel - 1] + iadd) && (Amat.i[iscan] == set0)) {
					// 14 ������� 2016 ��� ������ ���������� � ����.
					//while (iscan <= row_startA[set0 + 1] - 1) { // ��� ������ ���������� � ���� �� ���������� �������.
					// ���� ���� j ��� �� ��� �������� � �������.
					if (flag[Amat.j[iscan]] == false) {
						if ((Amat.j[iscan] != set0) && (Amat.aij[iscan] < 0.0) && (Amat.abs_aij[iscan] >= theta(ilevel) * max_vnediagonal)) {
							// 21.05.2017
							bool bfound_vacant = false;

							bfound_vacant = isfound_hash_table_Gus_struct01(Amat.j[iscan]);
							if (!bfound_vacant) {
								insert_hash_table_Gus_struct01(Amat.j[iscan]);
								ic++;
							}

							/*
							vacant = Amat.j[iscan];
							for (integer js = 0; js < ic; js++) {
							if (vacant == set[js]) {
							vacant = NULL_NEIGHBOUR;
							}
							}
							if (vacant != NULL_NEIGHBOUR) {
							set[ic] = vacant;

							ic++;

							}
							*/
						}
					}

					iscan++;

				} // while
			}

			//std::cout << "sboi end";
			// ��� ���� ������ ������ ����� i,j






		// � ���� ����� ��������� set ������� ������������:
		// 1. ����������� �� root_Gus_set � set.
		// 2. root_Gus_set ������ �� ������������.
		// 3. ������ ����� ���� �������� ������ ��� set.
			integer* set = nullptr;
			set = new integer[ic + 2];
			//if (set == nullptr) {
				//std::cout << "error!!! memory for set is nullptr. Problem allocate detected."<<std::endl;
				//std::cout << "in function classic_aglomerative_amg6."<<std::endl;
				//system("pause");
				//exit(1);
			//}

			integer ic_986 = 1;
			set[0] = set0;



			formirate_hash_table_Gus_struct01__2__set(set, ic_986);

			clear_hash_table_Gus_struct01();


			for (integer isc = 1; isc < ic; isc++) {
				this_is_F_node[set[isc]] = true; // ��� ������ ����� F ����.
				bmarkervisit[set[isc]] = true;
			}

			// �������� ���� ��� ���������� � �������.
			for (integer js = 0; js < ic; js++) {
				flag[set[js]] = true;
			}

			// �������� (5 ������� 2015 revised) 
			// 1. ��������� ��� F ������� ������ ������� � �� ������ �������.
			// 2. ��� ������� �������������� F ��������� ��� "��������" �������.
			// 3. ���� ���� ��� �� ��� ������� � ������� �� ���� ���� ������� ������� ���� �� ������� 
			// ��������� � ������������� ������� F �� ������ 1.

			
			// 12 ������� 2015.
			// ���� ������� �� ��� ������ C � F ����.
			// ��� �������� ������� ��� ������ � �������� ��� �
			// �������� ���������. �������� �������������� � ������ �����
			// ������������ ���������. �������� ������������ �� ���������������
			// �� ��������� 2  ����� �� ���������� ��������� � ������
			// ������������������ ������ ��� ���� �����������.
			for (integer js = 0; js < ic; js++) {
				
				if (!m1.empty()) {
					m1.erase(set[js]);
					//v1.erase(std::pair<integer, integer>(set[js], count_neighbour[set[js]]));
					//v1.erase(set[js]);
				}		
			}




			//std::cout<<"additional and modify new neighbour"<<std::endl;

			// 10 ������ 2016. ����� ������.
			// �������� ��������� ��������� ����������� (��� ������ ������� �������� �� ��� ������).
			// ��� ����������� ��� ���������� ���������� V ������ ������� ��������� �� ����������
			// ��� ����������� �������� ������ � ������������ ��������� � ���������� ��� ��� ����������.
			// �� ������ 13 ������ 2016 ��� ������ ������� �� �������� ����������.
			integer itop_stack2 = 0;

			// 10 ������ 2016. ������ ������� ������ ��������� �� ����������� ����.
			for (integer js = 1; js < ic; js++) {

				integer i_11 = set[js];
				integer ii_11 = row_startA[i_11];
				//integer iend5 = nnz_a[ilevel - 1] + iadd;
				//integer istart73 = ii_11;
				//while ((istart73 >= 1 + iadd) && (Amat.i[istart73] == Amat.i[ii_11])) istart73--;
				//istart73++;
				integer istart73 = row_startA[Amat.i[ii_11]];
				integer iend73 = row_startA[Amat.i[ii_11] + 1] - 1;
				bool bvisitsos = false;
				for (integer is0 = istart73; (is0 <= iend73); is0++) {
					//for (integer is0 = istart73; (is0 <= iend5) && (Amat.i[is0] == Amat.i[ii_11]); is0++) {
					// � ����������� � U!!!
					if (flag[Amat.j[is0]] == false) {


						integer isc = Amat.j[is0];



						// ����������� �� ��������� �������������.
						// � 2D �� ������������ ������� ��������� ������������� ����������
						// ����� 33%.
						// ��� ��� ����������� �������� ����������, �������� � �������, ��
						// �� ���� �������� ����� ��� ����� ������� ����������� ��������� �����
						// ����� (������������ �� ������, �� ������� ���� ������������ ������� �������� �������).
						// ��� �������� ���������� �� �������:
						// CGHV1J006D, ��������� ����, ������������� ��������� � FET, Module 2.
						// ������ �������� ����������� ���������� ������� ������� ��-�� ������� ������� ������������.
						// �������� �� � ���� ����� ����.
						if (hash_table2[isc] == false) {
							hash_table2[isc] = true;
							istack[itop_stack2] = isc;
							itop_stack2++;
							// ������������������ �����.
							//}

							//21_12_2016
							integer ii_2 = row_startA[isc];


							integer ic2 = 0;
							integer iend2loc = nnz_a[ilevel - 1] + iadd;
							//integer istart72 = ii_2;										
							integer istart72 = row_startA[Amat.i[ii_2]];
							integer istopmarker2 = row_startA[Amat.i[ii_2] + 1] - 1;

							// 22 _12_2016
							// ��� ������ �������: ������������ ���������� ���������� ��������
							// ������� �� ������ passive module 6 � �� ����� ��� ��� ��������� 
							// �������� �� ����� ������� ������ ����.
							doublerealT max_vnediagonal33 = -1.0e30;
							for (integer is01 = istart72; (is01 <= istopmarker2); is01++) {
								if (Amat.j[is01] != Amat.i[is01]) {
									if ((Amat.aij[is01] < 0.0) && (Amat.abs_aij[is01] > max_vnediagonal33)) {
										max_vnediagonal33 = Amat.abs_aij[is01];
									}
								}
							}
							for (integer is01 = istart72; (is01 <= istopmarker2); is01++) {
								// 0.2375 ����������� ��������� ��� passive module 6.
								if ((Amat.aij[is01] < 0.0) && (Amat.abs_aij[is01] > 0.2375 * max_vnediagonal33)) {
									if (Amat.j[is01] == set[js]) {
										if ((my_amg_manager.ipatch_number == 7) && (bStrongTransposeON)) {
											if (js < ic_end_F_SiTranspose) {
												// ����������� �������� ������ ��� ������� F ����� �������
												// �������� �������� F ����� ������� ���� �������� �� Si_Transpose ������.
												// ������ ��� �������� � ����� ���� � ������ �������.
												ic2++;
											}
										}
										else {
											ic2++;
										}
									}
								}
								// ��������� ������� ������� (weakly) ������ ?
							}


							data_BalTree dsearch;
							dsearch.count_neighbour = count_neighbour[isc];
							//dsearch.ii = ii_2;
							dsearch.i = isc;
							// ����������� �� ���������� ������ � ������ F ������.
							count_neighbour[isc] += ic2;
							data_BalTree dadd;
							dadd.count_neighbour = count_neighbour[isc];
							//dadd.ii = ii_2;
							dadd.i = isc;

							std::map<integer, integer>::const_iterator it; // ��������� ��������
							//it = m1.begin(); // ����������� ��� ������ map-�
							it = m1.find(isc);
							if (it == m1.end()) {
								// �������� � map ����.
								m1.insert(std::pair<integer, integer>(isc, count_neighbour[isc]));
								//v1.push_back(std::pair<integer, integer>(isc, count_neighbour[isc]));
							}
							else {
								// �������� � map ������������.
								m1.erase(isc);
								//v1.erase(std::pair<integer, integer>(isc, dsearch.count_neighbour));
								
								m1.insert(std::pair<integer, integer>(isc, count_neighbour[isc]));
								//v1.push_back(std::pair<integer, integer>(isc, count_neighbour[isc]));
							}							

						}


					}

				}
			}

			// ������� (�������������� ���-�������).
			// �� � ���� ������ �� ����������� �� OPENMP � ���� �����.!!!
			for (integer i_54 = 0; i_54 < itop_stack2; i_54++) {
				hash_table2[istack[i_54]] = false;
			}
			itop_stack2 = 0; // ���� ����� ����� � ������.





			if (set != nullptr) {
				delete[] set;
				set = nullptr;
			}

			newCcount++;
			// ���� ������� ������.

		} // ���� �� ��� ��� ������� � �������.



		bcontinue_gl_1 = false;
		for (integer i_1 = istartflag_scan; i_1 <= n_a[ilevel - 1]; i_1++) {
			if (flag[i_1] == false) {
				bcontinue_gl_1 = true;
				istartflag_scan = i_1; // ��������� ������� ������������.
				break; // ��������� ����� �� ����� for.
			}
		}

		// ���������� ���� � ������������ ����������� �������.
		icandidate = 0;


		// ������ ��� ����������� ���������.
		// ���� ����� ������������ ������� � ��� ������.
		

		if (!m1.empty()) {
			std::map<integer, integer>::const_iterator it; // ��������� ��������
			// ��� ����� ��������� ���.
			it = std::max_element(m1.begin(), m1.end(), greaters_Stuben());
			//std::cout << (*it).first << "= ";

			

			//std::make_heap(v1.begin(), v1.end(), greaters_Stuben());
			//std::pop_heap(v1.begin(), v1.end(), greaters_Stuben()); // ������� ������������ ������� �� ����.
			

			icandidate = row_startA[(*it).first];
			//icandidate = row_startA[(v1.back()).first];  // ���������� ������������ �������.
		}
		else {
			icandidate = 0;
			bcontinue_gl_1 = false;
		}
		


		if (debug_reshime) system("pause");
		//getchar();

		if ((icandidate == 0) ) {
			bcontinue_gl_1 = false;
		}
		// 4 june 2016

		icountprohod++;

	} // ���������� C/F ���������. �������.


    // ������������ ����������� ������ �� ��� ��� ������.
	// 12 ������� 2015.
	m1.clear();
	//v1.clear();

	//delete[] bmarkervisit;
	if (bmarkervisit != nullptr) {
		free(bmarkervisit);
		bmarkervisit = nullptr;
	}


} // Ruge_and_Stuben_CF_decomposition_std

// ��������� ��������� ����� �������������� ����� �� �-���� � 
// F - ����. � -���� �������� ������ ����� ���������� ������ �����������.
// �������� �� ������� � F ����� ������ ���� ������������� �� �������� �������
// � ��������� � ����� (��. ������ ������������). 
void PMIS_CF_decomposition(Ak2& Amat, bool*& this_is_F_node,
	bool*& this_is_C_node, integer ilevel, 
	integer*& count_neighbour, integer*& n_a,
	integer &newCcount,
	doublereal*& threshold_quick_only_negative,
	integer*& row_startA,
	Taccumulqtor_list** &hash_StrongTranspose_collection1)
{

	integer ii_end1 = n_a[ilevel - 1];

	doublereal* dcount_neighbour = new doublereal[n_a[ilevel - 1] + 1];

	integer inumber_isolated_F_nodes = 0;
	for (integer i7 = 1; i7 <= ii_end1; i7++) {
		// ����� ������� ���� ��������� ����� �� ���� �� �������.
		if (count_neighbour[i7] == 0) {
			this_is_F_node[i7] = true;
			inumber_isolated_F_nodes++;
		}
		// drand ��������� ������������ ����� �� ���� �� �������.
		doublereal drand = (doublereal)(((double)(1.0 * rand()) / ((double)(RAND_MAX + 1))));
		dcount_neighbour[i7] = count_neighbour[i7] + drand;
	}


	bool bflag_empty = false;
	bool bcontinue = true;
	while (bcontinue) {
		bcontinue = false;
		integer inumber_of_nodes_viewed = 0; // ����� ������������� ����� �� ����� ������� (������������).
		integer inumber_of_C_nodes = 0; // ����� ����������� ���������.


		for (integer i7 = 1; i7 <= ii_end1; i7++) {
			if ((this_is_C_node[i7] == false) && (this_is_F_node[i7] == false)) {
				inumber_of_nodes_viewed++;

				if (bflag_empty) {
					// ���������� ������, �� ��������� ���������� ������ � ������.
					inumber_of_C_nodes++;
					newCcount++;
					this_is_C_node[i7] = true;
					//std::cout<<"lambdai="<< count_neighbour[i7]<<std::endl;
					//system("PAUSE");
				}
				else {

					bcontinue = true;

					doublereal id_diag = dcount_neighbour[i7];

					// ����������� S(i)
					doublereal max_vnediagonal = -1.0;
					//for (integer is0 = row_startA[i7]; (is0 <= row_startA[i7 + 1] - 1); is0++) {
						//if (Amat.j[is0] != i7) {
							//if ((Amat.aij[is0]<0.0)&&(Amat.abs_aij[is0] > max_vnediagonal)) {
								//max_vnediagonal = Amat.abs_aij[is0];
							//}
						//}
					//}
					max_vnediagonal = threshold_quick_only_negative[i7];
					//  ������ ������ transpose(S(i)).
					bool bunion_on = true;
					//bool bOk = false;
					integer is0_end = row_startA[i7 + 1] - 1;
					for (integer is0 = row_startA[i7]; (is0 <= is0_end); is0++) {
						if (Amat.j[is0] != i7) {
							if ((Amat.aij[is0] < 0.0) && (Amat.abs_aij[is0] > max_vnediagonal)) {
								if (dcount_neighbour[Amat.j[is0]] >= id_diag) {
									// ����� ��� �� �������������.
									if ((this_is_C_node[Amat.j[is0]] == false) && (this_is_F_node[Amat.j[is0]] == false)) {
										bunion_on = false;
									}
								}
								//else {
									// ����� ��� �� �������������.
									//if ((this_is_C_node[Amat.j[is0]] == false) && (this_is_F_node[Amat.j[is0]] == false)) {
									//	bOk = true;
								//	}
								//}
							}
						}
					}
					// ��������� transpose(S(i)):
					if (hash_StrongTranspose_collection1 != nullptr) {
						if (hash_StrongTranspose_collection1[i7] != nullptr) {
							Taccumulqtor_list* list_scan = hash_StrongTranspose_collection1[i7];
							while (list_scan != nullptr) {
								integer icandidate73 = list_scan->ikey;
								if (icandidate73 != i7) {
									if (dcount_neighbour[icandidate73] >= id_diag) {
										// ����� ��� �� �������������.
										if ((this_is_C_node[icandidate73] == false) && (this_is_F_node[icandidate73] == false)) {
											bunion_on = false;
										}
									}
									//else {
									//	if ((this_is_C_node[icandidate73] == false) && (this_is_F_node[icandidate73] == false)) {
									//		bOk = true;
									//	}
									//}
								}
								list_scan = list_scan->next;
							}
						}
					}
					// ���� bunion_on �� �� ��������� ����� ������� ����� � �� ������ �������.
					// ���� � ������� ������ ����� �� S(i).
					/*
					if (bunion_on) {
						inumber_of_C_nodes++;
						this_is_C_node[i7] = true;
						// ����� ������� ����� ������ �� transpose(S(i)).
						for (integer is0 = row_startA[i7]; (is0 <= row_startA[i7 + 1] - 1); is0++) {
							// ����� ��� �� �������������.
							if ((this_is_C_node[Amat.j[is0]] == false) && (this_is_F_node[Amat.j[is0]] == false)) {
								if ((Amat.j[is0] != i7) && (Amat.abs_aij[is0] >= theta(ilevel) * max_vnediagonal)) {
									this_is_F_node[Amat.j[is0]] = true;
								}
							}
						}
						newCcount++;
						// ���� ������� ������.
					}
					*/
					// ���� bunion_on �� �� ��������� ����� ������� ����� � �� ������ �������.
					// ���� � ������� ������ ����� �� transpose(S(i)).
					//if (true||bOk) 
					{
						if (bunion_on) {
							inumber_of_C_nodes++;
							this_is_C_node[i7] = true;
							// ����� ������� ����� ������ �� transpose(S(i)).
							if (hash_StrongTranspose_collection1 != nullptr) {
								if (hash_StrongTranspose_collection1[i7] != nullptr) {
									Taccumulqtor_list* list_scan = hash_StrongTranspose_collection1[i7];
									while (list_scan != nullptr) {
										integer icandidate72 = list_scan->ikey;
										if (icandidate72 != i7) {
											// ����� ��� �� �������������.
											if ((this_is_C_node[icandidate72] == false) &&
												(this_is_F_node[icandidate72] == false)) {
												this_is_F_node[icandidate72] = true;
											}
										}
										list_scan = list_scan->next;
									}
								}
							}
							newCcount++;
							// ���� ������� ������.
						}
					}
					//else {
						//this_is_F_node[i7] = true;
						// �������� � ������� ����������� ��������� � �������� ������� �������
					//}
				}

			}
		}

		if ((inumber_of_C_nodes == 0) && (inumber_of_nodes_viewed > 0)) {
			bflag_empty = true;
		}

		std::cout << "view candidates=" << inumber_of_nodes_viewed << " additional aggregates=" << inumber_of_C_nodes << " inumber_isolated_F_nodes=" << inumber_isolated_F_nodes << "\n";
		//getchar();
	}

	delete[]  dcount_neighbour;
}

template <typename myARRT>
void my_realloc_memory(myARRT* &x, integer n) {
	if (x != nullptr) {
		myARRT* x_temp = nullptr;
		x_temp = (myARRT*)realloc(x, ((n) * sizeof(myARRT)));
		if (x_temp == nullptr) {
			std::cout<<"application crash for bx array in procedure my_realloc_memory."<<std::endl;
			system("pause");
			exit(1);
		}
		else {
			x = x_temp;
		}
	}
}

// 29.07.2018 - xx.xx.xxxx ������ 6 �� ������ ������ 4.
// 24.04.2020 ������� �������� ���������(C/F ���������) � ��������� �������.
// 14.04.2020 �������� �� ��� ������� setup � solution phase.
// 13.04.2020 ����� ���������� CountingSort �� ������������� ���������
// ����������� ������. ������������� #pragma omp for schedule (static)
// ���������� ������ ����� ���������������. ������ ������� �� 8 ������ (�������),
// ������� ���� ������������ ������ �����, ������ ������ � �.�. �����
// ����� ����������������� ����������� ������ ����� �������� ������� �
// �������� ��������������� �� �������. ��� ���� ��������� 12%.
// 12.04.2020 ��������� ����������� ������ �������� � ��������� �������.
// 3.02.2019 ������ ��������� ����� ������� ���� ������ Ak2, 
// ��� �������� ���������� ����������� ������.
// 25.04.2018 ������ �4 classic_aglomerative_amg4 ��� �������� �������������� ������.
// ����� ������ classic_aglomerative_amg5 ����� �� �������������� (����������).
// ���� 2017 - �������� �����-����� smoother, �������� ��������� ilu0 ���������� � ���������. 
// ���� 2017 - �������������� ������������ ���������� ������� ����������� 100 � �����. 
// ������ �������� 2016-2017 ���� - �������� bicgStab.
// ���� 2017 - �������������� ������������� ����� ������ �� ���� � ���� ������������ ���
// ������������������� � ��������� ����� ��� ��� ������ BiCGStab.
// ��� ������ ���������� ����� ���������� �
// �������� ������ ��� ������ �������� amg.
// 4-6 ������ 2016. �������� ILU0. ��������� ����� ���������� ��� �� Solution Phase.
// 9 ������� 2016. ������� �� ����������� � �������� ������������� ��������� ������ ���� 
// � ������� ������� amg. ��� �� ����������� � �� ������������ amg1r5. 
// 9 ������� ������ ��������� ������������ ������ � ������� 
// �� ������ ������ ����������� � ������� ILU2 ������������. ��� ������������ ������ 
// �.�.��������, �.�.��������
// "������������� ����� ������� ������ �������������� ������" ������ ���, ������-��-����,
// ������. ��� ���
// ���������� ������������ ����������� �� ������ ������� ��� ����� � ������������ ������������
// �������� � ����������� �������� ������� �� ���2 �����
// (����������� ���������������� �����). � ������ 
// ��������� � ��� ���� 
// �������� ���� ������������� ILU2 ������������������� �� ���������� SPARSKIT2 �.����� 
// ������� ������ ���2 � ��� 
// ����� ILU2.
// 22 ������ 2016 ������� ��������������� ������� ����.
// �����: 1. ������� ������ amg3. 
// � ���: 2. �������� ��� �������� �� ����������� ����������� ������ �� ������������� 
// ������� ������ ��� �����.
// ��� �������� ������� �������� ����������� ���.
// � ��� 3. ��� V ����� �������� � ���� ����� for. ����� �� ����� ����� ����������� ��������
// direct ����� ��� ������ ������� ������.
// ��� �� ������� ����������� ������� �� amg ��������� ������������������� ��� BiCGStab.
// 15 ������ 2016 �������� ������ ��������� �� Ak1.
// 10 ������ 2016 �������� ����� ������ �� �����������.
// 15 ������� 2015. ������ ������ ���� ����� ��������� ������� �� ����������� ����.
// 13 ������� 2015. �������� ��� ������. ��� ��������� ��� ������ ���������� ���������� ������ �
// ���������� �/F ���������, ������ C/F ��������� �������� ���������.
// ��������� � ������ quicksort (qs,qsj)
// ������� � ���� ��� ������� ������������� ����������.
// ������ ����� �� band_size!!!.
// ����� ������ ��������� �� 1.7� ����������� � 3D ��������� ����� 1 ������.
// 18 ������� 2015. ��������� ��������������� ����������.
// ������������� �� �������� ������� �� ������ �������� �� ����� 
// ������� �������. 18 ������� 2015 ���������� ������ 0.04. ������ 0.04 �� �����
// ������� ������ 0_03. ���� �������� ��� �������� ���������� C/F ���������, 
// ��� � ���������� ��������� ��������. ��� ���������� �/F ��������� 
// ����������� ��� ����������� ��� ����� � ������� ����� ������������ ��
// �� ������� ������ ����������� ��������� ������ �� ����������� �����.
// ��� ���������� ������������ �������� �������� ����� ����������� �� 
// �������������� ������,
// ���������� �� ��������� ������� ��������������� �������.
// 4 ������� 2015 ���������� ���������� ������������������ ��������� ������.
// 30 �������� 2015 ���������� ����������� ������. ������ ������������ 
// �������������� ������������� ����� �� ������  C/F ���������.
// 16 �������� 2015 ���� ���������� ��� �������� 
// ���������� � ������������ ������� ���������� �������,
// � ���� ���������� ��� � �����-�� ���� ���������� �� ������������ ������ �������.
// �������� ���������� � ������������ ����� ������� ������ �� ������ ������ 
// �.�. ������� � ����� ������ �������.
// 3 september 2015 Villa Borgese.
// ���������� divergence detected.
// ���������� ������ ��������� ������ my_amg_manager.
template <typename doublerealT>
void setup_phase_classic_aglomerative_amg6(Ak2 &Amat,
	integer nsizeA, // ���������� ����� ���������� ����� ��� ��������� ������ �	
	integer nnz, // number of non zero elements
	integer n, // dimension of vectors x and b.	
	doublereal* &x, //solution (�������) 
	doublereal* &b, // rthdsd (������ �����).
	doublerealT &ret74, 
	integer iVar,
	bool bmemory_savings,
	BLOCK* &my_body, integer &lb, integer maxelm_out,
	// ��������� ������ ������ ��� solution phase.
	amg_precond_param &amg_pp,
	integer*& n_a, integer*& nnz_a, integer*& nnz_aRP,
	integer& ibsp_length,
	BAD_STRING_PATCHING*& bsp,
	integer i_bsp_LIMIT,
	bool*& flag,
	bool*& F_false_C_true, Ak1*& P,
	bool bQuick_sort_for_reorder
) {



	// �������������� �������������� �� �������� � ������ � 
	// ������� ��� ��� ����� ����� ��������������.
	// ��� ����� ������������� ���������.
	//const bool bQuick_sort_for_reorder = false;

	// Parallel Modifined Independed Set
	bool bPMIS = false;

	//doublereal theta = my_amg_manager.theta;
	//doublereal theta83 = my_amg_manager.theta;
	doublereal magic82 = my_amg_manager.magic;
	doublereal magic83 = my_amg_manager.magic;


	const bool b_REALLOC = false;

	

	// �� ����� 100 ��������� �����
	// � ������������� ����������.
	//const int i_bsp_LIMIT = 100;
	if (bsp != nullptr) {
		delete[] bsp;
		bsp = nullptr;
	}
	bsp=new BAD_STRING_PATCHING[i_bsp_LIMIT];
	ibsp_length = 0;

	//integer &nsizePR, // ������ ��� P � ����������� n.
	//Ak1* &R, // restriction
	//Ak1* &P, // prolongation  

	//Ak1* R = nullptr; 
	// �������� �������� ����� ��������� ������������ � ��������� �� ����������������.
	//Ak1* P = nullptr;

	integer nsizePR = 35;// ������ ��� P � ����������� n.
	nsizePR = 135;
	if (bonly_solid_calculation) {
		// 31 ������� 2016.
		// �� ���� ������� � ������� ������� � 30% ��� ���� ������������ 
		// ����� ������ ������� �������� 12.
		nsizePR = 135; // ���� 12. 01,02,2020 ����� 35.
	}



	//R = new Ak1[(integer)(35 * n) + 1]; // 3*nnz 2.4 // 35
	//--->R = (Ak1*)malloc(((nsizePR * n) + 1) * sizeof(Ak1));
	//char c7[2] = "R";
	//handle_error<Ak1>(R, c7, c1, ((nsizePR * n) + 1));

	//P = new Ak1[(integer)(35 * n) + 1]; // 3*nnz 2.4 // 35
	P = (Ak1*)malloc(((nsizePR * n) + 1) * sizeof(Ak1));
	char c1[22] = "my_agr_amg_loc_memory";
	char c8[2] = "P";
	handle_error<Ak1>(P, c8, c1, ((nsizePR * n) + 1));
	std::cout<<"Prolongation alloc succseful..."<<std::endl;

	//if (R != nullptr) {
		// ������������ ������ �������� P, �������� R ����� ����� �� ��� � P � ��������� �� ����������.
		// ������ restriction � prolongation �� ������������� � ����������.
		//free(R);
		//R = nullptr;
	//}

	//  ���������� ���-�������.
	construct_hash_table_Gus_struct01(n);
	

	// 23.12.2016 ��������� ����� ���������� �����:
	// �������� ������ ����������� ����� ������� V �����,
	// ��� ����� ������ ��������� 
	// b � lb.


	integer* row_ind_PE = nullptr;
	integer* row_ind_PS = nullptr;
	integer* row_ind_AS = nullptr;
	integer* row_ind_AE = nullptr;
	integer* index_visit = nullptr;
	doublerealT* vector_sum = nullptr;
	integer istartAnew_mem;		
	integer* row_ind_ER = nullptr;
	integer* row_ind_SR = nullptr;
	integer iend_marker_position;
	doublerealT* ap_coarse = nullptr;
	integer icounter = 1;
	integer icount1;
	integer numberofcoarcenodes;
	integer* C_numerate = nullptr;	
	bool bweSholdbeContinue = true;
	integer the_number_of_neighbors_that_are_not_C_nodes = 0;
	integer number_of_F_nodes_with_one_single_strong_C_neighbor = 0;
	integer number_of_F_nodes_with_one_single_strong_C_neighborF = 0;

	integer iadditionalCstatistic = 0;	

	integer newCcount = 0;	

	integer* row_startA = nullptr;
	integer* count_neighbour = nullptr;

	bool identiti = true;
	

	// ������� ���������� ������� ������������������ ����������� ����: BiCGStab + camg(�����).
	// 1. ������������� ����������.
	// 2. ������������������.
	// 3. ������������.
	// ���� my_amg_manager.istabilization == 1 �� �� ���������� ����� ������������� ���������� �� ������������� � ������������������� 
	// ������������ �������������� ������������� ������� �����.
	// ������ ���������� 5.01.2017.(more robust).
	// ���� my_amg_manager.istabilization == 0 - �� ������ ������������ 
	// ������������� �������� ��� ������ ���� ������ ����������� ���������������.
	// ���� my_amg_manager.istabilization == 2 - �� ������������ fgmres - 
	// �������� ����� ����� � ������� �. ������ (������ ������� ����������� ������ ����������� �������) � ������� 
	// �� ������ �������� ��������� fgmres �������� ���� ������������� ������������������ (���� V ����). 
	//bool bBiCGStab_plus_RUMBA_camg = true;
	//if (my_amg_manager.istabilization == 0) {
	// ������ ������������� ����� ��� ������-���� ����������� ���������������.
	// none
	//bBiCGStab_plus_RUMBA_camg = false;
	//}


	bfirst_jacoby_start = true;

	bool from_re_operation_protection0 = true;
	integer ifrom_re_operation_protection = 0;

	// ������������� ������������ ���������. 4 ������ 2016.
	// ILU2 smoother
	// 0 - ILU �� ������������. ������������ Gauss-Seidel.
	// 1 - ILUk(k==lfil) ������������.
	// 2 - 
	integer bILU2smoother = 0;
	if (my_amg_manager.ilu2_smoother == 1) {
		// �������� ILUk(k==lfil) ������������. 
		// �� ��� ������ ������ �� ����� ������ ��������.
		// ���� ������� ��� �� ��������� � ������� ����� ������������������� ���������.

		//---->bILU2smoother = 1; // ILU0

						   // �� - �������� �������� 
						   // ilu0_(maxelm_plus_maxbound, milu0.val, milu0.col_ind, milu0.row_ptr, milu0.alu, milu0.jlu, milu0.ju, milu0.iw, ierr);
						   // �������� ���������. � �� ������� � ��� ���������� ��� �� �������. ���� �������� iluk � lfil=0 ������� ���� ��������� 
						   // ������� � � ��� ���������� � �������������. ��� ����������� � ����� ���� my_amg_manager.ilu2_smoother == 2.
						   // ������ iluk � lfil=0 �������� �� ���� ������� � ��������� ���� ���������.

		// �������� ILU2 ������������. 
		// �� ��� ������ ������ �� ����� ������ ��������.

		// ��� ������������� ��������� ������ ��� �������� ������� - ������� ����.
		// ���� ��� ��������� �� ����� �������� ������� �� ���������� ���� �����������.

		 // ILU2 ��� ������� ����� ����������� ������ � � ��� ������� �� ILU0 ������������ �� ������ ������: iluk � lfil=0.
		// �������� � ��� ������� � ilu2 ����-�� �� ������� ������, �.�. ��� �� �������� �����.

						   // ���������������.
		bILU2smoother = 2; // ILU0 // ILU2
	}

	if (my_amg_manager.ilu2_smoother == 2) {
		// ����� ����� ������������ �������� �������.
		my_amg_manager.iRunge_Kutta_smoother = 3;
	}
	if (my_amg_manager.ilu2_smoother == 3) {
		// ����� ����� ������������ ������ �������.
		my_amg_manager.iRunge_Kutta_smoother = 5;
	}

	if (my_amg_manager.ilu2_smoother == 6) {
	    // gmres smmother. K-smoother.
		my_amg_manager.b_gmres = true;
	}
	if (my_amg_manager.ilu2_smoother == 7) {
		// spai0 smoother.
		my_amg_manager.b_spai0 = true;
	}


	//bILU2smoother = 0; // only seidel sor smoother.
	const doublerealT dapply_ilu_max_pattern_size = 9.2;

	// ��������� ���������� �� �������������� ��������� SOR.
	// �� ��� ������ �� �������� �������� � �� � ������ 
	// ������������� ���������� �������� ���������� omega_optimal.
	// �������� �������������� ������� ������ ��� ������� �������� �������������.
	bproblem_amg_convergence1 = false;
	bproblem_amg_convergence2 = false;
	bproblem_amg_convergence3 = false;
	gold_const = 0.2;

	bool bprint_mesage_diagnostic = true;
	if (my_amg_manager.iprint_log == 0) {
		bprint_mesage_diagnostic = false;
	}


	bool bpositive_connections_CF_decomp = true;
	integer memo_icoarseningtype = my_amg_manager.icoarseningtype;
	if ((my_amg_manager.icoarseningtype >= 4)&&(my_amg_manager.icoarseningtype<=7)) {
		// only negative connections 
		// ��������������� ������������� ����� ������������ ��� �������� C/F ���������.
		bpositive_connections_CF_decomp = false;
		my_amg_manager.icoarseningtype -= 4;
	}
	else if (my_amg_manager.icoarseningtype == 8) {
		// Parallel Modified Independed Set.
		bPMIS = true;
	}
	// 19.01.2016 ��� ���������� C/F ��������� � ������������ ������������ ������ ������
	// � ������� ������������� � �� ������������� positive connections.
	// ������� ���������� ��������� ������: 
	// 1. ��� ���������� ��������� ������������ ����� ��� ����� ��� ������� ��� � �������.
	// 2. ��� ���������� C/F ������������ ����� ������ ������� �����. 
	// ��� �������� ��������� �������������.
	// ���������� ����� bpositive_connections_CF_decomp ������������ ��� ����������
	// C/F ������������ � bpositive_connections
	// ��������� 19.01.2017.


	bool bpositive_connections = true;
	// 23 ������� 2016
	if (bSIMPLErun_now_for_temperature) {
		// ������ cfd ������.
		//bprint_mesage_diagnostic = false;

		// �������� � ���, ��� positive connections 
		// ���������� ������ ������������� � ������ ���� �������� 
		// � ������������ � ����������������� �������:
		// �������� �������, � ��������� positive connections ���������� ������ ����.
		//bpositive_connections = false;
	}	

	
	// ��� ���������� grid complexity ��������� ������������:
	integer nnz_P_memo_0 = 0;
	integer nnz_P_memo_all = 0;

	// ���� �������� ��� new �� malloc.
	//theta = 0.25;

	bool bonly_serial = true;	

	// �������� ����� ������� ������ ����� �����������.
	// doublerealT theta = 0.25;  // 0.25 for 2D and 0.5 for 3D 


	//const integer QUICK_SORT_ALG = 1; // ������� ���������� �. �����.
	// ������������ �� quicksort qs and qsj.
	// ���������� � ��������� ������� quickSort.
	// ������������ �� ���������� ��������� ������� 
	//���������� ��������� ������ (������ ��� ����� � ������� ���� ����������).
	//const integer COUNTING_SORT_ALG = 0; // ���������� � ��������� ������ �����.
	// ���������� � ��������� �������� ������ ��� ����� ������������ � 
	// ����� � �������� ��������� ����������.
	//const integer HEAP_SORT_ALG = 2; // ������������� ����������.
	// ���������� ����������� ������� ����������, ������� QuickSort �� ��������.
	// � ����������� ���� ��������� ������ ����� �� 4��.
	//bmemory_savings =false ��� QUICK_SORT_ALG � HEAP_SORT_ALG;
	integer imy_sort_algorithm = my_amg_manager.imySortAlgorithm;// COUNTING_SORT_ALG;

	const doublereal RealZERO = 1.0e-300;// 1.0e-10;
	const doublereal divisionZERO = 1.0e-300;
	const doublereal RealMAXIMUM = 1.0e300;
	// ���������/���������� ����������� ������.
	bool debug_reshime = false;


	const integer maxlevel = 201; // (101 �� 16.02.2019) (51 �� 5.06.2017) 30
	integer ilevel = 1;
	if (n_a != nullptr) {
		delete[] n_a;
		n_a = nullptr;
	}
	if (nnz_a != nullptr) {
		delete[] nnz_a;
		nnz_a = nullptr;
	}
	n_a = new integer[maxlevel];
	nnz_a = new integer[maxlevel];
	nnz_a[0] = nnz;
	n_a[0] = n;

	const int iKnumber_thread = 8;

	//char *str_Function_name = "classic_aglomerative_amg_6";

	//#ifdef _NONAME_STUB29_10_2017
#ifdef _OPENMP 
	// ������ ������������ ��� ���������� ������������� �����.
	// iKnumber_thread=8 - ��� ����� �������.

	Ak1** AccumulqtorA_m = nullptr;
	AccumulqtorA_m = new Ak1*[iKnumber_thread];
	doublerealT** vector_sum_m = nullptr;
	vector_sum_m = new doublerealT*[iKnumber_thread];
	integer** index_visit_m = nullptr;
	index_visit_m = new integer*[iKnumber_thread];
	bool** hash_table_m = new bool*[iKnumber_thread];
	integer* index_size_m = nullptr;
	integer*  istartAnew_m = new integer[iKnumber_thread];
	index_size_m = new integer[iKnumber_thread];
	// ���� ���������� ������ ������������� � ����� ������� ���������������� ������ ��� 
	// ������� �������� 5.55*� ����. ����������� �������� � ������� 11.0*�.
	// �������� �������� �� ��������� 7.82*�.
	//  ��� ������ - ��������� �������� ��� 1500�� ��� � ������ ������ - ���������������
	// ��������� � ���� ��������: �� 17.5�� �� 0.5���. 
	doublereal AccumulqtorA_m_SIZE8 = (doublereal)(2.0*my_amg_manager.memory_size); // ���� �������� 5.55; 11; 30.
	if (n > 20000000) {
		// �������� ������
		AccumulqtorA_m_SIZE8 = 11.0;
	}
	for (integer i_9 = 0; i_9 < iKnumber_thread; i_9++) {
		AccumulqtorA_m[i_9] = new Ak1[(integer)(0.125* AccumulqtorA_m_SIZE8 *nnz + 1)];
		std::cout << "AccumulqtorA_m["<< i_9 <<"] alloc succseful..."<< std::endl;
		vector_sum_m[i_9] = new doublerealT[n + 1];
		//vector_sum_m[i_9] = (doublerealT*)malloc((n + 1) * sizeof(doublerealT));
		//handle_error<doublerealT>(vector_sum_m[i_9], "vector_sum_m[i_9]", "classic_aglomerative_amg_6", (n + 1));

		index_visit_m[i_9] = new integer[n + 1];
		//index_visit_m[i_9] = (integer*)malloc((n + 1) * sizeof(integer));
		//handle_error<integer>(index_visit_m[i_9], "index_visit_m[i_9]", "classic_aglomerative_amg_6", (n + 1));

		hash_table_m[i_9] = new bool[(10 * n + 1)];
		//hash_table_m[i_9] = (bool*)malloc((10 * n + 1) * sizeof(bool));
		//handle_error<bool>(hash_table_m[i_9], "hash_table_m[i_9]", "classic_aglomerative_amg_6", (10 * n + 1));

		integer i_91_end = 10 * n + 1;
		for (integer i_91 = 0; i_91 < i_91_end; i_91++) hash_table_m[i_9][i_91] = false;// inicialization
		index_size_m[i_9] = 0;
		istartAnew_m[i_9] = 0;
	}
#endif

	// quick - ����������� ���� ���, � ����� ����� ������ ������������.
	// threshold - ��������� �������� �������������� ����� ������� ������ ����� �����������.
	// ������������ �� ������� ��������������� �������������.
	// ������ �� 0 �� n ������������.
	doublerealT* threshold_quick_all = my_declaration_array<doublerealT>(n, -1.0, "threshold_quick_all");

	// threshold - ��������� �������� �������������� ����� ������� ������ ����� �����������.
	// ������������ ������ �� ������������� ��������������� �������������.
	// ������ �� 0 �� n ������������.
	doublerealT* threshold_quick_only_negative = my_declaration_array<doublerealT>(n, -1.0, "threshold_quick_only_negative");
	bool btreshold_on_new_vetv = true; // false ����� ��������� ����� �� ������ ���������� ����� ����.


	// flag n+1	
	// ������ �� 0 �� n ������������.
	if (flag != nullptr) {
		free(flag);
		flag = nullptr;
	}
	flag = my_declaration_array<bool>(n, false, "flag");
	

	// hash_table2  n+1
	// ��� ���������� C/F ������������ ��� ���� ����������� ���-�������
	// � ���� ��� ������� ���-�������.
	// ������������� ��������� � ���������. 
	// ������ �� 0 �� n ������������.
	bool* hash_table2 = my_declaration_array<bool>(n, false, "hash_table2");

	// istack n+1
	// � ������ ���� ��� ������� ���-�������.
	// ������ �� 0 �� n ������������.
	integer* istack = my_declaration_array<integer>(n, -1, "istack");
	


	integer iadd = 0;
	integer nnzR = 1;
	integer iaddR = 0;
	if (nnz_aRP != nullptr) {
		delete[] nnz_aRP;
		nnz_aRP = nullptr;
	}
	nnz_aRP=new integer[maxlevel];
	bool bcontinue_global = true;
	
	bool* this_is_C_node = my_declaration_array<bool>(n, false, "this_is_C_node");
	bool* this_is_F_node = my_declaration_array<bool>(n, false, "this_is_F_node");

	// ������������� ��������� �����������.
	if (F_false_C_true != nullptr) {
		free(F_false_C_true);
		F_false_C_true = nullptr;
	}
	F_false_C_true = my_declaration_array<bool>(4 * n, false, "F_false_C_true");

	

	bool bStrongTransposeON = true; // ��� � ���������� ���������� Strong Transpose.
	if (my_amg_manager.icoarseningtype == 0) {
		bStrongTransposeON = false;
	}
	
	node_AVLST** hash_StrongTranspose_collection = nullptr;
	Taccumulqtor_list** hash_StrongTranspose_collection1 = nullptr;
	integer isize_memory_alloc_hash_StrongTranspose_collection1 = -1;
	integer *isize_hash_StrongTranspose_collection = nullptr;

	while ((ilevel < maxlevel - 1) && (n_a[ilevel - 1] > 50) && (bcontinue_global)) {
		

		// ������ �� ���������� ������������ �� ���������� � ������������.
		from_re_operation_protection0 = true;
		ifrom_re_operation_protection = 0;		

		if (ilevel > 1) {

			if (my_amg_manager.icoarseningtype == 9) {
				// HMIS
				// Hybrid Modifyed Independed Set.
				// RS �� ������ ������, PMIS �� ��������� �������.
				bPMIS = true;
			}
			
			//threshold_quick_all
			my_realloc_memory<doublerealT>(threshold_quick_all, ((n_a[ilevel - 1]) + 1));
						
			//threshold_quick_only_negative
			my_realloc_memory<doublerealT>(threshold_quick_only_negative, ((n_a[ilevel - 1]) + 1));

			// istack
			my_realloc_memory<integer>(istack, ((n_a[ilevel - 1]) + 1));
			
			//hash_table2
			my_realloc_memory<bool>(hash_table2, ((n_a[ilevel - 1]) + 1));
			
			// this_is_C_node
			my_realloc_memory<bool>(this_is_C_node, ((n_a[ilevel - 1]) + 1));

			
			// this_is_F_node
			my_realloc_memory<bool>(this_is_F_node, ((n_a[ilevel - 1]) + 1));			
			
			// �������� ����������� ������ ��� ���-������� ��������.
			construct_hash_table_Gus_struct01(n_a[ilevel - 1]+2);

		}

		// ��������� �������� ������ 3.3 � 1.2 ����� ���� �������� � �������� � �����������.
		// ��� ����� ��������� ������������ �� ������� ����� ������� �����.
		doublerealT dsize_memory_for_Amat = 3.9; // �� ������� � ���������� ���� ���� 3.9.
		if ((my_amg_manager.icoarseningtype == 1) || 
			((my_amg_manager.icoarseningtype == 3))) {
			// RS2 �������. ���� ���� � ����� ������ ������ ������.
			dsize_memory_for_Amat = 3.9;
		}
		if (b_on_adaptive_local_refinement_mesh) {
			if ((1 && steady_or_unsteady_global_determinant == CFD_STEADY)) {
				if (iVar == PAM) {
					// cfd ��� �������� �������� �� ���� �����.
					//dsize_memory_for_Amat = 10;
					dsize_memory_for_Amat = 4.9;
				}
			}
			else {
				dsize_memory_for_Amat = 4.9;
			}
		}
		if (b_REALLOC) {
			// ���������� ������ ��������� ��� �������� ������� �.
			// ������� ������ �������� � ������ �� ����� ��� ��� �� ����� � �� ���������� ������.
			my_realloc_memory<doublerealT>(Amat.aij, ((iadd + 2 + (integer)(dsize_memory_for_Amat * nnz_a[ilevel - 1]))));

			my_realloc_memory<doublerealT>(Amat.abs_aij, ((iadd + 2 + (integer)(dsize_memory_for_Amat * nnz_a[ilevel - 1]))));
			
			my_realloc_memory<integer>(Amat.i, ((iadd + 2 + (integer)(dsize_memory_for_Amat * nnz_a[ilevel - 1]))));
			
			my_realloc_memory<integer>(Amat.j, ((iadd + 2 + (integer)(dsize_memory_for_Amat * nnz_a[ilevel - 1]))));
						
			if (bprint_mesage_diagnostic) {
				std::cout<<" 1 of 3 compleated.  OK!! ierarhion matrix Amat realloc successfully..."<<std::endl;
				std::cout<<"Prolongation ierarhion..."<<std::endl;
			}
			my_realloc_memory<Ak1>(P, ((nnz_P_memo_all + (integer)(1.2 * nnz_a[ilevel - 1])) + 2));
			
			if (bprint_mesage_diagnostic) {
				std::cout<<"2 of 3 compleated. OK!! ierarhion matrix Prolongation realloc successfully..."<<std::endl;
			}
		}
		

		if (ilevel > 1) {
			doublerealT procent_n = (100.0*(abs(n_a[ilevel - 1] - n_a[ilevel - 2]))) / (1.0*n_a[ilevel - 2]);
			//doublerealT procent_nnz = (100.0 * (abs(nnz_a[ilevel - 1] - nnz_a[ilevel - 2]))) / (1.0 * nnz_a[ilevel - 2]);
			//if ((procent_n<2.0)&&(procent_nnz < 2.0)) break;
			if (procent_n < 2.0) break;
		}

		if (((ilevel > 1) && (nnz_a[ilevel - 1] > nnz_a[ilevel - 2]))) {
			//break;
		}

		// 19.04.2018
		print_control_volume_statistics(n_a, nnz_a, ilevel, bprint_mesage_diagnostic, debug_reshime);

		

#pragma omp parallel for
		for (integer ii = 1; ii <= n_a[ilevel - 1]; ii++) {
			this_is_C_node[ii] = this_is_F_node[ii] = false;
		}

		// ���������� ����� ���� �� ������ ������, �.�.
		// ��������� ��������� ������������ �� �. ���������� 1978 ��� 
		// ��� �� ������ ��������������� �� ������� �������.
		if (ilevel == 1) {
			// ���������� ��������  �  �� i.
			//heapsort(Amat, key=i*n_a[ilevel - 1] + j, 1, nnz_a[ilevel - 1]);

			// 7 ������ 2016. ����������� ����� ��� ����������.
			switch (imy_sort_algorithm) {
			case COUNTING_SORT_ALG:
				Counting_Sort(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd, bmemory_savings,n_a[ilevel-1]);	//�������� ������ n_a[ilevel - 1]			
				break;
			case HEAP_SORT_ALG:
				HeapSort(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd);
				//LeftistHeapSort(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd);
				break;
			case QUICK_SORT_ALG:
				qs_abbys_heigh = 0;
				// quicksort
				qs(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd);
				// ������������ ��������. O(n*log2(n)).
				// �� ���������� ������ ������.
				//std::sort(Amat + (1 + iadd) * sizeof(Ak1), Amat + (nnz_a[ilevel - 1] + iadd + 1) * sizeof(Ak1), compAi);

				//QuickSort(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd);
				
				break;
			case TIM_PETERSON_SORT_ALG:
				// ���������� ���� ��������� 2002�.
				timSort_amg(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd);
				break;
			default:
				Counting_Sort(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd, bmemory_savings, n_a[ilevel - 1]);//�������� ������ n_a[ilevel - 1]
				break;
			}

		} // ilevel == 1


		if (my_amg_manager.bMatrixPortrait == 1) {
			// ������ �������� �������.

			FILE* fp_portrait = NULL;
			errno_t err_portrait = 0;
#ifdef MINGW_COMPILLER
			fp_portrait = fopen64("matrix_load.txt", "w");
			if (fp_portrait != NULL) {
#if doubleintprecision == 1
				fprintf(fp_portrait, "%lld %lld\n", n_a[ilevel - 1], nnz_a[ilevel - 1]);
				for (integer i58 = 1 + iadd; i58 <= nnz_a[ilevel - 1] + iadd; i58++) {
					fprintf(fp_portrait, "%lld %lld\n", Amat.i[i58], Amat.j[i58]);
				}
#else
				fprintf(fp_portrait, "%d %d\n", n_a[ilevel - 1], nnz_a[ilevel - 1]);
				for (integer i58 = 1 + iadd; i58 <= nnz_a[ilevel - 1] + iadd; i58++) {
					fprintf(fp_portrait, "%d %d\n", Amat.i[i58], Amat.j[i58]);
				}
#endif
		    }
#else
			err_portrait = fopen_s(&fp_portrait, "matrix_load.txt", "w");
#if doubleintprecision == 1
			fprintf_s(fp_portrait, "%lld %lld\n", n_a[ilevel - 1], nnz_a[ilevel - 1]);
			for (integer i58 = 1 + iadd; i58 <= nnz_a[ilevel - 1] + iadd; i58++) {
				fprintf_s(fp_portrait, "%lld %lld\n", Amat.i[i58], Amat.j[i58]);
			}
#else
			fprintf_s(fp_portrait, "%d %d\n", n_a[ilevel - 1], nnz_a[ilevel - 1]);
			for (integer i58 = 1 + iadd; i58 <= nnz_a[ilevel - 1] + iadd; i58++) {
				fprintf_s(fp_portrait, "%d %d\n", Amat.i[i58], Amat.j[i58]);
			}
#endif
#endif
			if (fp_portrait != NULL) {
				fclose(fp_portrait);
			}
			printf("matrix portrait in level export\n");
			system("PAUSE");
		}
		
		integer ii_end1 = n_a[ilevel - 1];
#pragma omp parallel for
		for (integer ii = 1; ii <= ii_end1; ii++) {
			flag[ii] = false;
		}

		
		// ������� ������ ������ ������ � �������.
		if (row_startA != nullptr) {
			free(row_startA);
			row_startA = nullptr;
		}
		row_startA = my_declaration_array<integer>((n_a[ilevel - 1] + 1), 0, "row_startA");
		

		ii_end1 = nnz_a[ilevel - 1] + iadd;
		calculate_row_ptr(1 + iadd, ii_end1, row_startA, flag, Amat);


		ii_end1 = n_a[ilevel - 1];
#pragma omp parallel for
		for (integer ii = 1; ii <= ii_end1; ii++) {
			flag[ii] = false;
		}

		if (bQuick_sort_for_reorder) {
			for (integer ii = 1; ii <= ii_end1; ii++) {
				// 14.04.2020
				// � ������ ������ i ������� ������� j ������������� �� �����������.		
				qs_abbys_heigh = 0;
				// quicksort
				qsj(Amat, row_startA[ii], row_startA[ii + 1] - 1);
			}
		}

		integer is_1 = row_startA[1];
		integer is_e = row_startA[n_a[ilevel - 1] + 1] - 1;
		// ������� ���� ��� ��������� ������ ��������.
		for (integer iscan = is_1; iscan <= is_e; iscan++) {
			Amat.abs_aij[iscan] = fabs(Amat.aij[iscan]);
		}

		// ��������� ��� ������� ���� ����� ��� �������.
		// ������������� �����������.
		if (count_neighbour != nullptr) {
			free(count_neighbour);
			count_neighbour = nullptr;
		}
		count_neighbour = my_declaration_array<integer>(n_a[ilevel - 1], 0, "count_neighbour"); // 0 - ��� �������.		

		if (bStrongTransposeON) 
		{
			// ������������ ���.
			// ����������.
			//std::cout<<"usage strong transpose"<<std::endl;
			//system("PAUSE"); // debug
			
			// ��� ����� ������� ��� ��� �������� �������.

			// ������� ���������� - �������� ������ � ������� ��������.
			if (hash_StrongTranspose_collection1 != nullptr) {
#pragma omp parallel for
				//for (integer i_1 = 0; i_1 <= n_a[ilevel - 2]; i_1++)
				//isize_memory_alloc_hash_StrongTranspose_collection1
				for (integer i_1 = 0; i_1 <= isize_memory_alloc_hash_StrongTranspose_collection1; i_1++)
				{
					clear_list(hash_StrongTranspose_collection1[i_1]);
				}
				delete[] hash_StrongTranspose_collection1;
				hash_StrongTranspose_collection1 = nullptr;
			}
			if (isize_hash_StrongTranspose_collection != nullptr) {
				delete[] isize_hash_StrongTranspose_collection;
				isize_hash_StrongTranspose_collection = nullptr;
			}
			// �������� ������ ��� ��� �������� ���������������� �������.
			hash_StrongTranspose_collection1 = new Taccumulqtor_list*[n_a[ilevel - 1] + 1];
			ii_end1 = n_a[ilevel - 1];
#pragma omp parallel for
			for (integer i_1 = 0; i_1 <= ii_end1; i_1++) {
				hash_StrongTranspose_collection1[i_1] = nullptr;
			}

			isize_memory_alloc_hash_StrongTranspose_collection1 = n_a[ilevel - 1];
			if (isize_hash_StrongTranspose_collection != nullptr) {
				free(isize_hash_StrongTranspose_collection);
				isize_hash_StrongTranspose_collection = nullptr;
			}
			isize_hash_StrongTranspose_collection = my_declaration_array<integer>(n_a[ilevel - 1], 0, "isize_hash_StrongTranspose_collection");		
			
		}

		// ��� ����� ���� ���� ������� ���� ����� ������, ����� ��� 
		// ���������� ���� ������� ������ � ���� ����� �������.
		// ������� ��������� �� ����� ������ ������� � (����� �����).
		ii_end1 = nnz_a[ilevel - 1] + iadd;
		for (integer ii = 1 + iadd; ii <= ii_end1; ii++) {
			if (flag[Amat.i[ii]] == false) {
				integer ic = -1;
				
				
				
				// �������� ����� ����: 11.06.2017.
				// �������� ����� ����� ������� �������� �������� ��� ������� ��������� �������� ���������� threshold.
				// �� � ���� ������ �� ������� 0 � if.
				// ��� ����� ����������� ������ �����. Ÿ �������� �������� � ������������������� ����� ����������.
				threshold_quick_all[Amat.i[ii]] = -1.0;
				threshold_quick_only_negative[Amat.i[ii]] = -1.0;
				integer is0_end = row_startA[Amat.i[ii] + 1] - 1;

				if (bpositive_connections_CF_decomp) {
					for (integer is0 = ii; (is0 <= is0_end); is0++) {
						if (Amat.j[is0] != Amat.i[ii]) {
							if (Amat.abs_aij[is0] > threshold_quick_all[Amat.i[ii]]) {
								// ���������� ������������ ��������������� �������.
								threshold_quick_all[Amat.i[ii]] = Amat.abs_aij[is0];
							}
						}
					}
				}
				else {
					for (integer is0 = ii; (is0 <= is0_end); is0++) {
						if (Amat.j[is0] != Amat.i[ii]) {
							if (Amat.aij[is0] < 0.0) {
								if (Amat.abs_aij[is0] > threshold_quick_only_negative[Amat.i[ii]]) {
									// ���������� ������������ ��������������� �������.
									threshold_quick_only_negative[Amat.i[ii]] = Amat.abs_aij[is0]; 
								}
							}
						}
					}
				}
					
					
				if (bpositive_connections_CF_decomp) {
				    //doublerealT theta_threshold3 = theta(ilevel)*threshold;
					doublerealT theta_threshold3 = theta(ilevel)*threshold_quick_all[Amat.i[ii]];
					for (integer is0 = ii; (is0 <= is0_end); is0++) {
						if (Amat.j[is0] != Amat.i[ii]) {
							if (Amat.abs_aij[is0] > theta_threshold3) {
								// ��������� ������ ������ ��������� �������.
								ic++; //i,j
					    		 									
								if (bStrongTransposeON) {
									// O(1) ������� � ������ ��������� ������.
									insert_list(hash_StrongTranspose_collection1[Amat.j[is0]], Amat.i[ii]);
									isize_hash_StrongTranspose_collection[Amat.j[is0]]++;
								}
							}
						}
					}
				}
				else {
					for (integer is0 = ii; (is0 <= is0_end); is0++) {
						if (Amat.j[is0] != Amat.i[ii]) {
							if (Amat.aij[is0] < 0.0) {
								if (Amat.abs_aij[is0] > theta(ilevel)*threshold_quick_only_negative[Amat.i[ii]]) {
									// ��������� ������ ������ ��������� �������.
									ic++; //i,j
									 					 					
									if (bStrongTransposeON) {
										// O(1) ������� � ������ ��������� ������.
										insert_list(hash_StrongTranspose_collection1[Amat.j[is0]], Amat.i[ii]);
										isize_hash_StrongTranspose_collection[Amat.j[is0]]++;
									}
								}
							}
						}
					}
				}
				
				
				

				count_neighbour[Amat.i[ii]] = ic;
				// 01.03.2019
				// ��� �������� ���� � 895 ������.
				//count_neighbour[Amat.i[ii]] = isize_hash_StrongTranspose_collection[Amat.i[ii]];
				// 22_12_2016
				if (ic == 0) {
					// ������� ������ ������� �� ��� ������ 8.������ 2017 ???

					// �� ������ ������ ��������� ��� ������� ������� ���������� F ������.
					this_is_C_node[Amat.i[ii]] = false;
					this_is_F_node[Amat.i[ii]] = true;

				}
				flag[Amat.i[ii]] = true;				
				
			}
		}

		if (bStrongTransposeON) {
			// 5.01.2017. StrongTranspose.
			// ������� lambda ��������������� �������� ������������ ��������� ����� Strong Transpose.
			ii_end1 = n_a[ilevel - 1];
			for (integer i_1 = 1; i_1 <= ii_end1; i_1++) {

				// 20.05.2017 �������� ������� ������ �� ����� ��� ���������� ��������� � ������.
				//count_neighbour[i_1] = getnumber_AVL_node_global(hash_StrongTranspose_collection[i_1]);
				count_neighbour[i_1] = isize_hash_StrongTranspose_collection[i_1];
				if (count_neighbour[i_1] == 0) {
					// 14.04.2017 ��������� ������������� ����������� 
					// ����������� ���������� ��������:
					// # ������; ����� ��. ��; ����� ��. �����;
					// 1. passiv_module6; 179; 97;
					// 2. CGHV 12mm HFET; 18, 8, 6, 3, 2; 17, 8, 6, 3, 2;
					// 3. PIONER; 77; 73;

					// �� ������ ������ ��������� ��� ������� ������� ���������� F ������.
					this_is_C_node[i_1] = false;
					this_is_F_node[i_1] = true;

				}			
			}
		}


		
		
		ii_end1 = n_a[ilevel - 1];
		for (integer ii = 1; ii <= ii_end1; ii++) {
			flag[ii] = false; // init flag
		}

		

		if (bPMIS) {
			// ��������� ��������� ����� �������������� ����� �� �-���� � 
			// F - ����. � -���� �������� ������ ����� ���������� ������ �����������.
			// �������� �� ������� � F ����� ������ ���� ������������� �� �������� �������
			// � ��������� � ����� (��. ������ ������������). 
			PMIS_CF_decomposition(Amat, this_is_F_node,
				this_is_C_node, ilevel,
				count_neighbour, n_a,
				newCcount,
				threshold_quick_only_negative,
				row_startA,
				hash_StrongTranspose_collection1);
		}
		else {
			
			integer max_neighbour = 0;
			integer icandidate = 0;

			// ������� ���� � ���������� ������ ������� � ���������� ���.
			// ��� ������ ������������� ���� � ���������� ������ �������.
			// ��� ��������� ��� ���� ����� ��������� �������� C/F ���������.

			for (integer i7 = 1; i7 <= ii_end1; i7++) {
				if (count_neighbour[i7] > max_neighbour) {
					max_neighbour = count_neighbour[i7];
					icandidate = row_startA[i7];
				}
			}

			// 4 ���� 2016.
	        // ��� ������ ����� ��������� ������� ����������� ������ �� �� ���� ������� � ��� 
	        // ���������� ������� ������ �����.
			if ((icandidate == 0) && (max_neighbour == 0)) {

				//getchar();
				// ������� ��������� ������ ������� ��������� ����� �� �����.
				break;
			}

			// ��������� ��������� ����� �������������� ����� �� �-���� � 
            // F - ����. � -���� �������� ������ ����� ���������� ������ �����������.
			// �������� �� ������� � F ����� ������ ���� ������������� �� �������� �������
			// � ��������� � ����� (��. ������ ������������). 
			Ruge_and_Stuben_CF_decomposition<doublerealT>(Amat, this_is_F_node,
				this_is_C_node, ilevel,
				count_neighbour, n_a,
				newCcount,
				threshold_quick_only_negative,
				row_startA,
				hash_StrongTranspose_collection1,
				bprint_mesage_diagnostic,
				flag, iadd, bpositive_connections_CF_decomp,
				bStrongTransposeON, hash_table2,
				istack, nnz_a, debug_reshime);
		}

		  //delete[] count_neighbour;
		if (count_neighbour != nullptr) {
			free(count_neighbour);
			count_neighbour = nullptr;
		}

		

		if (bprint_mesage_diagnostic) {
			if (n_a[ilevel - 1] == 0) {
				std::cout<<"n_a is zero"<<std::endl;
				system("pause");
			}
			std::cout<<"additional C="<<(doublerealT)(100.0*newCcount / n_a[ilevel - 1])<<"%"<<std::endl;
			//system("pause");
		}

		
			
		if (!bPMIS) {

			// ���������� ����� � ����� ��� �������������� ������ RS ������������.

			// � ������ ����������� ������������ ������������ ��� ���������� �������������,
			// ��� ���� ����� ��������� ���������������� ��� F ���������� C ����������� ����
			// ��������� ���������� � ����������.
			the_number_of_neighbors_that_are_not_C_nodes = 0;
			number_of_F_nodes_with_one_single_strong_C_neighbor = 0;
			number_of_F_nodes_with_one_single_strong_C_neighborF = 0;

			iadditionalCstatistic = 0;

			bweSholdbeContinue = true;
			while (bweSholdbeContinue) {
				bweSholdbeContinue = false;

				bool* bvacant_candidates = nullptr;
				bvacant_candidates = new bool[n_a[ilevel - 1] + 1];
				if (bvacant_candidates == nullptr) {
					std::cout<<"error memory alloc in bvacant_candidates"<<std::endl;
					system("pause");
					exit(1);
				}
#pragma omp parallel for
				for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) bvacant_candidates[i_1] = false;

				// ���������� ����������� ��� ����� ������� ���������� F-nodes.
				// ������ F-nodes ������ C-nodes.
#pragma omp parallel for
				for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) {
					if (this_is_F_node[i_1] == true) {
						// ����� ������� ������� F-node ������� C-node.
						integer icsos = 0;
						// ������ ������ �� 10 ������ 2016.
						//integer i_2 = BinarySearchAi(Amat, i_1, 1 + iadd, nnz_a[ilevel - 1] + iadd);
						// ������� ������� ��� ������, ������ �������������� �� ������ "��� �������".
						// 10 ������ 2016. �� ������ �����������.
						integer i_2 = row_startA[i_1];

						bool bvisit = false;
						//for (integer is0 = i_2; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[i_2]); is0++) {
						integer iend_merker_position = row_startA[Amat.i[i_2] + 1] - 1;
						for (integer is0 = i_2; (is0 <= iend_merker_position); is0++) {
							if (Amat.j[is0] != Amat.i[i_2]) {
								bvisit = true;
								if (this_is_C_node[Amat.j[is0]] == true) {
									icsos++;
								}
								else {
									//the_number_of_neighbors_that_are_not_C_nodes++; // ������������ �������� ������������ 
								}
							}
						}
						//if (icsos == 1) number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ � �������.
						// ���� bvisit �� ��������������� �������� ���� �� ��� ��� F-nodes. ����� ��� ������������ ������� �������.
						if ((icsos == 0) && (bvisit)) {

							// � ���� �� F ���� ������� ��� �������, �� ����� ���� ����� ���� ������� � ��� ������� ������� �� ����.
							// ���� ������� ����� ���� ��� ������� �� ��������� �������, ��� ������������� � ����� ������ � ����
							// ��������� ������������ ���������� ����� � �� ���� � �������.
							// ����� ���������� ��� ����������� ����� �������� � �������� ������.

							// ��� � �������, ���� ���� ������ � �����.
							bvacant_candidates[i_1] = true;
						}
					}
				}


#ifdef _OPENMP
				// ������������ ���������� �� ����� ��� � 40 �������
				integer newCcount_arr[40];
				integer the_number_of_neighbors_that_are_not_C_nodes_arr[40];
				integer number_of_F_nodes_with_one_single_strong_C_neighbor_arr[40];
				bool bweSholdbeContinue_arr[40];

				for (integer i_1 = 0; i_1 < 40; i_1++) {
					newCcount_arr[i_1] = 0;
					the_number_of_neighbors_that_are_not_C_nodes_arr[i_1] = 0;
					number_of_F_nodes_with_one_single_strong_C_neighbor_arr[i_1] = 0;
					bweSholdbeContinue_arr[i_1] = false;
				}

				// ���������� ����������� ��� ����� ������� ���������� F-nodes.
				// ������ F-nodes ������ C-nodes.
#pragma omp parallel for
				for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++)
				{
					if (this_is_F_node[i_1] == true) {

						integer tid = omp_get_thread_num();

						// ����� ������� ������� F-node ������� C-node.
						integer icsos = 0;
						// ������ ������ �� 10 ������ 2016.
						//integer i_2 = BinarySearchAi(Amat, i_1, 1 + iadd, nnz_a[ilevel - 1] + iadd);
						// ������� ������� ��� ������, ������ �������������� �� ������ "��� �������".
						// 10 ������ 2016. �� ������ �����������.
						integer i_2 = row_startA[i_1];

						bool bvisit = false;
						//for (integer is0 = i_2; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[i_2]); is0++) {
						integer iend_merker_position = row_startA[Amat.i[i_2] + 1] - 1;
						for (integer is0 = i_2; (is0 <= iend_merker_position); is0++) {
							if (Amat.j[is0] != Amat.i[i_2]) {
								bvisit = true;
								if (this_is_C_node[Amat.j[is0]] == true) {
									icsos++;
								}
								else {
									//the_number_of_neighbors_that_are_not_C_nodes++; // ������������ �������� ������������ 
									the_number_of_neighbors_that_are_not_C_nodes_arr[tid]++;
								}
							}
						}
						if (icsos == 1) {
							//	number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ ������� � �������.
							number_of_F_nodes_with_one_single_strong_C_neighbor_arr[tid]++;
						}
						// ���� bvisit �� ��������������� �������� ���� �� ��� ��� F-nodes. ����� ��� ������������ ������� �������.
						if ((icsos == 0) && (bvisit)) {

							// � ���� �� F ���� ������� ��� �������, �� ����� ���� ����� ���� ������� � ��� ������� ������� �� ����.
							// ���� ������� ����� ���� ��� ������� �� ��������� �������, ��� ������������� � ����� ������ � ����
							// ��������� ������������ ���������� ����� � �� ���� � �������.
							// ����� ���������� ��� ����������� ����� �������� � �������� ������.							

							// ��� � �������, ���� ���� ������ � �����.
							this_is_F_node[i_1] = false;
							this_is_C_node[i_1] = true;
							// F-node ���� C-node!!! ���� ����������� ������������ 
							// �������� � ���������� ������������� ��������� ��������.
							bweSholdbeContinue_arr[tid] = true;
							newCcount_arr[tid]++;
						}

						// 1 ������ 2015 ���� ����� ��� ������������.
						// ������� � ������ ������ ������ ������ ����� ���� � �����.
						if ((false) && (icsos == 1)) {
							// bvisit � ��� true �.�. icsos==1.
							this_is_F_node[i_1] = false;
							this_is_C_node[i_1] = true;
							//bweSholdbeContinue = true;
							bweSholdbeContinue_arr[tid] = true;
						}

					}

				}

				for (integer i_1 = 0; i_1 < 40; i_1++) {
					newCcount += newCcount_arr[i_1];
					the_number_of_neighbors_that_are_not_C_nodes += the_number_of_neighbors_that_are_not_C_nodes_arr[i_1];
					number_of_F_nodes_with_one_single_strong_C_neighbor += number_of_F_nodes_with_one_single_strong_C_neighbor_arr[i_1];
					if (bweSholdbeContinue_arr[i_1]) {
						bweSholdbeContinue = true;
					}
				}
#else

				// ������������ ���������� �� ����� ��� � 40 �������
				integer newCcount_arr = 0;
				integer the_number_of_neighbors_that_are_not_C_nodes_arr = 0;
				integer number_of_F_nodes_with_one_single_strong_C_neighbor_arr = 0;
				bool bweSholdbeContinue_arr = false;


				// ���������� ����������� ��� ����� ������� ���������� F-nodes.
				// ������ F-nodes ������ C-nodes.
				for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++)
				{
					if (this_is_F_node[i_1] == true) {


						// ����� ������� ������� F-node ������� C-node.
						integer icsos = 0;
						// ������ ������ �� 10 ������ 2016.
						//integer i_2 = BinarySearchAi(Amat, i_1, 1 + iadd, nnz_a[ilevel - 1] + iadd);
						// ������� ������� ��� ������, ������ �������������� �� ������ "��� �������".
						// 10 ������ 2016. �� ������ �����������.
						integer i_2 = row_startA[i_1];

						bool bvisit = false;
						//for (integer is0 = i_2; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[i_2]); is0++) {
						integer iend_merker_position = row_startA[Amat.i[i_2] + 1] - 1;
						for (integer is0 = i_2; (is0 <= iend_merker_position); is0++) {
							if (Amat.j[is0] != Amat.i[i_2]) {
								bvisit = true;
								if (this_is_C_node[Amat.j[is0]] == true) {
									icsos++;
								}
								else {
									//the_number_of_neighbors_that_are_not_C_nodes++; // ������������ �������� ������������ 
									the_number_of_neighbors_that_are_not_C_nodes_arr++;
								}
							}
						}
						if (icsos == 1) {
							//	number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ ������� � �������.
							number_of_F_nodes_with_one_single_strong_C_neighbor_arr++;
						}
						// ���� bvisit �� ��������������� �������� ���� �� ��� ��� F-nodes. ����� ��� ������������ ������� �������.
						if ((icsos == 0) && (bvisit)) {

							// � ���� �� F ���� ������� ��� �������, �� ����� ���� ����� ���� ������� � ��� ������� ������� �� ����.
							// ���� ������� ����� ���� ��� ������� �� ��������� �������, ��� ������������� � ����� ������ � ����
							// ��������� ������������ ���������� ����� � �� ���� � �������.
							// ����� ���������� ��� ����������� ����� �������� � �������� ������.							

							// ��� � �������, ���� ���� ������ � �����.
							this_is_F_node[i_1] = false;
							this_is_C_node[i_1] = true;
							// F node ���� C_node!!! ���� ����������� ������������ 
							// �������� � ���������� ������������� ��������� ��������.
							bweSholdbeContinue_arr = true;
							newCcount_arr++;
						}

						// 1 ������ 2015 ���� ����� ��� ������������.
						// ������� � ������ ������ ������ ������ ����� ���� � �����.
						if ((false) && (icsos == 1)) {
							// bvisit � ��� true �.�. icsos==1.
							this_is_F_node[i_1] = false;
							this_is_C_node[i_1] = true;
							//bweSholdbeContinue = true;
							bweSholdbeContinue_arr = true;
						}

					}

				}

				newCcount += newCcount_arr;
				the_number_of_neighbors_that_are_not_C_nodes += the_number_of_neighbors_that_are_not_C_nodes_arr;
				number_of_F_nodes_with_one_single_strong_C_neighbor += number_of_F_nodes_with_one_single_strong_C_neighbor_arr;
				if (bweSholdbeContinue_arr) {
					bweSholdbeContinue = true;
				}

#endif



				if (bprint_mesage_diagnostic) {

					std::cout << "newCcount=" << newCcount <<", n_a=" << n_a[ilevel - 1] <<" "<< 100.0 * newCcount / n_a[ilevel - 1] << "\n";

				}
				if (bvacant_candidates != nullptr) {
					delete[] bvacant_candidates;
				}

				if (bprint_mesage_diagnostic) {
					if (bweSholdbeContinue) {
						std::cout<<" prohod succseful: bweSholdbeContinue==true\n";
					}
					else {
						std::cout<<" prohod empty: bweSholdbeContinue=false\n";
					}
				}

			}

			// TODO ***

			// 01.01.2017 �������� ��������� �������� C/F ���������. ������ 2. RS2
			// ���� �� ���� F ����������, ���������� ����� ������� �������.
			// ����� Fi ������� F ���������� � � �� ��������� ������� �� �����.
			// ��������� ������ ��������� ��� Fi ���� ������������ �������.
			// Amat. ���������� ����� - threshold ��� ������ ������.
			// �. ������� ���� ������� � ������� � ����������� �������� ������.
			// C. ���� �� ��������� �������� F ������  (Fj), ��� ��� Fi � Fj ������ �������,
			// �� ���� ���� ������� � ������� ���� Fj � ��������� �� ��� �������� ������.
			// � ������� ��������� ������� �� �������� ����� ���������� ��� �������������� ��������������� ��������
			// ������ �� ������� ����� � �����.
			// D. ���� ����� � ���� ���� �� ������ �� ������.
			// E. ���� ������ �������� � ���� �� ���������� �� ���� �� ����� Fi ��� Fj ���������� � �����.
			// ����� Fi � Fj ��� ���������� � ����� � �������� ������ ������� F �������. ���� � ����� ���� Fj 
			// �� �������� ������ � ������� ���� Fi �����������. ���� � ����� ���� ���� Fi �� �� ����������� ��������� Fi 
			// ��������� ���� ���������� Fj ����� � F ���.
			//  30.12.2016
			// 11.06.2017 ����� ��� ���������� ������������ ������������ std::sort �� �������.
			if (1) {
				if ((my_amg_manager.icoarseningtype == 1) || ((my_amg_manager.icoarseningtype == 3))) { // RS2 ������ 2.

					if (bprint_mesage_diagnostic) {
						std::cout << "   ***   CAMG SELECTOR RS2 " << ilevel << "  ***\n"; 
					}


					for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) if (this_is_F_node[i_1] == true) {
						// i_1 ��� F ���������� Fi.
						//Amat.���������� ����� - threshold ��� ������ ������.
						doublerealT thresholdRS = -1.0;
						integer i_2 = row_startA[i_1];

						// ������� ���-�������.
						clear_hash_table_Gus_struct01();
						// ��������� ������ �� ��������� ������ � ���-������� ��� ������ � ������ � Amat.i[i_2].
						//integer imarker75_scan = 0;
						//formirate_F_SiTranspose_hash_table_Gus_struct02(hash_StrongTranspose_collection1[Amat.i[i_2]], imarker75_scan);

						integer iend_merker_position = row_startA[Amat.i[i_2] + 1] - 1;
						if (!btreshold_on_new_vetv) {
							for (integer is0 = i_2; (is0 <= iend_merker_position); is0++) {
								if (Amat.j[is0] != Amat.i[i_2]) {
									if (Amat.aij[is0] < 0.0) {
										if (Amat.abs_aij[is0] > thresholdRS) thresholdRS = Amat.abs_aij[is0];
									}
								}
							}
						}
						else {
							// �������� ����� ����: 11.06.2017.
							thresholdRS = threshold_quick_only_negative[Amat.i[i_2]];
						}
						if (thresholdRS > 0.0) {
							// ��������� ������� �� ����� � ����� ����� thresholdRS.
							hashlist_i* ivacant_F2C = nullptr;
							//  �. ������� ���� ������� � ������� � ����������� �������� ������.
							hashlist_i* ibuffer_strongC = nullptr;
							integer ibuffer_strongC_marker = -1;
							integer inumber_strongF_count_Fi = 0;
							hashlist_i* ibuffer_strongF = nullptr;
							integer ibuffer_strongF_marker = -1;
							for (integer is0 = i_2; (is0 <= iend_merker_position); is0++) {
								if (Amat.j[is0] != Amat.i[i_2]) {
									if (Amat.aij[is0] < 0.0) {
										if (fabs(Amat.aij[is0]) > theta(ilevel)* thresholdRS) {
											if (this_is_C_node[Amat.j[is0]] == true) {
												ibuffer_strongC_marker++;
												insertion_list_i(ibuffer_strongC, Amat.j[is0]);
												insert_hash_table_Gus_struct01(Amat.j[is0]);// 11.08.2018
											}
											if (this_is_F_node[Amat.j[is0]] == true) {

												//if (1) 19.01.2017
												if (0) {// if (0) 11.08.2018
													// ������� 19.01.2017

													// �������� hash_StrongTranspose_collection ������ ���� ���������������� �������
													// � ����� ��������� ��� �������������, � ����� �� �������� �� ������� �.�. ������������
													//hash_StrongTranspose_collection1. ������ �������� isfound ��� hash_StrongTranspose_collection1
													// ��� �.�. ��� ����� ��������� �.�. �� ������ �������� ������.

													if (hash_StrongTranspose_collection != nullptr) {
														data_BalTreeST dat_key;
														dat_key.i = Amat.j[is0];
														if (isfound(hash_StrongTranspose_collection[Amat.i[is0]], dat_key)) {
															// ����� ������� 19.01.2017

															// ������� Fj ����� ������.
															// �������� Fi � Fj ������ �������.
															inumber_strongF_count_Fi++;
															ibuffer_strongF_marker++;
															insertion_list_i(ibuffer_strongF, Amat.j[is0]);
														}
													}
													else {
														// ������� Fj ����� ������.
														// �������� Fi � Fj ������ �������.
														inumber_strongF_count_Fi++;
														ibuffer_strongF_marker++;
														insertion_list_i(ibuffer_strongF, Amat.j[is0]);
													}

												}
												else {
													// ������� Fj ����� ������.
													// �������� Fi � Fj ������ �������.
													inumber_strongF_count_Fi++;
													ibuffer_strongF_marker++;

													insertion_list_i(ibuffer_strongF, Amat.j[is0]);
												}
											}
										}
									}
								}
							}
							// ������� ���-�������.
							clear_hash_table_Gus_struct01();
							// ���������� ������� ibuffer_strongC �� �����������.
							// ������������� ������������ iusage_old_version = 0
							// ��� ������� ������������ ������������� ����������������� ���-�������.
							// ����������� ��������� ������� ����� ������� ������ ��� ���������� RS2 coarsening
							// �� 7.5% �� ��������� � �������� ������� �� �������. 
							// ��������� �������� ������������� � ������������� ��������� ����������.
							// 11.06.2017.
							//integer iusage_old_version = 0; // 1 ������ ������� ������. // 0 ����� ������ �� ������ ��� �������.
							
							
							// ������ ���������� � ��������� ������ ���������� ���-�������.
							clear_hash_table_Gus_struct01();
							hashlist_i* ibuffer_strongC_scan = ibuffer_strongC;
							while (ibuffer_strongC_scan != nullptr) {
								insert_hash_table_Gus_struct01(ibuffer_strongC_scan->item);
								ibuffer_strongC_scan = ibuffer_strongC_scan->next;
							}
							ibuffer_strongC_scan = nullptr;
							

							
							// ��� ������� F-������ �������� � ����� ibuffer_strongF. 
							hashlist_i* ibuffer_strongF_current = ibuffer_strongF;
							for (integer i_3 = 0; i_3 <= ibuffer_strongF_marker; i_3++) {
								if (ibuffer_strongF_current != nullptr) {
									// ��������� ���� ������� F ������� ���������������.
									//1. ���������� threshold ��� Fj.
									doublerealT thresholdRS1 = -1.0;
									integer i_4 = row_startA[ibuffer_strongF_current->item];
									integer iend_merker_position1 = row_startA[Amat.i[i_4] + 1] - 1;
									if (!btreshold_on_new_vetv) {
										for (integer is01 = i_4; (is01 <= iend_merker_position1); is01++) {
											if (Amat.j[is01] != Amat.i[i_4]) {
												if (Amat.aij[is01] < 0.0) {
													if (fabs(Amat.aij[is01]) > thresholdRS1) thresholdRS1 = fabs(Amat.aij[is01]);
												}
											}
										}
									}
									else {
										// �������� ����� ����: 11.06.2017.
										thresholdRS1 = threshold_quick_only_negative[Amat.i[i_4]];
									}
									integer inumber_strongF_count_Fj = 0;
									// ������� ����� thresholdRS1.

									hashlist_i* ibuffer_strongCFj = nullptr;
									integer ibuffer_strongCFj_marker = -1;
									for (integer is01 = i_4; (is01 <= iend_merker_position1); is01++) {
										if (Amat.j[is01] != Amat.i[i_4]) {
											if (Amat.aij[is01] < 0.0) {
												if (fabs(Amat.aij[is01]) > theta(ilevel)* thresholdRS1) {
													if (this_is_C_node[Amat.j[is01]] == true) {
														ibuffer_strongCFj_marker++;
														insertion_list_i(ibuffer_strongCFj, Amat.j[is01]);
													}
													if (this_is_F_node[Amat.j[is01]] == true) {
														inumber_strongF_count_Fj++;
													}
												}
											}
										}
									}
									// � ibuffer_strongCFj ������ ������� � �������.

									// ���� �� ����� � ���� �� �������� �����.
									// ������ �� ������ ������ ibuffer_strongC
									// ������������� ������.
									// ��������� ���. ������  ibuffer_strongCFj_marker 
									// �������� ������� � ���� ��������������� ������� 
									// �� ��� ��� ���� �� ���������� �������� �����.
									bool bfound_32 = false;
									hashlist_i* ibuffer_strongCFj_scan = ibuffer_strongCFj;
									
									// ������ �� ������ ���-�������.
									while ((bfound_32 == false) && (ibuffer_strongCFj_scan != nullptr)) {
										// ���������� ������� �� ������ �� ������.
										bfound_32 = isfound_hash_table_Gus_struct01(ibuffer_strongCFj_scan->item);
										ibuffer_strongCFj_scan = ibuffer_strongCFj_scan->next;
									}
									
									ibuffer_strongCFj_scan = nullptr;
									if (bfound_32 == false) {
										// ���� �� ��� ������ � �����.
										if ((ibuffer_strongF_current->item > i_1) && (inumber_strongF_count_Fj >= inumber_strongF_count_Fi)) {
											// ���� Fj ��������� � ��� ��������������� ����� ������ F ����� �
											// � ���� �� ��������� � F ����� Fi ������ ������� F ������.								

											// Fj ���������� �.
											insertion_list_i(ivacant_F2C, ibuffer_strongF_current->item);
											this_is_C_node[ibuffer_strongF_current->item] = true;
											this_is_F_node[ibuffer_strongF_current->item] = false;
											ibuffer_strongC_marker++;
											inumber_strongF_count_Fi--;
											insertion_list_i(ibuffer_strongC, ibuffer_strongF_current->item);
											
											hashlist_i* ibuffer_strongC_scan = ibuffer_strongC;

											// ������� ���-������� � ��������� � �� �����.
											clear_hash_table_Gus_struct01();
											while (ibuffer_strongC_scan != nullptr) {
												insert_hash_table_Gus_struct01(ibuffer_strongC_scan->item);
												ibuffer_strongC_scan = ibuffer_strongC_scan->next;
											}

											ibuffer_strongC_scan = nullptr;
											
										}
										else {
											// Fi ���������� �.
											this_is_C_node[i_1] = true;
											this_is_F_node[i_1] = false;
											// ���������� ��� Fj � � �� F.
											hashlist_i* ivacant_F2C_marker = ivacant_F2C;
											while (ivacant_F2C_marker != nullptr) {
												this_is_F_node[ivacant_F2C_marker->item] = true;
												this_is_C_node[ivacant_F2C_marker->item] = false;
												ivacant_F2C_marker = ivacant_F2C_marker->next;
											}
											ivacant_F2C_marker = nullptr;
											if (ivacant_F2C != nullptr) {
												clear_hash_list_i(ivacant_F2C);
												ivacant_F2C = nullptr;
											}
											ivacant_F2C = nullptr;
											// ������� ���.
											if (ibuffer_strongCFj != nullptr) {
												clear_hash_list_i(ibuffer_strongCFj);
												ibuffer_strongCFj = nullptr;
											}
											ibuffer_strongCFj = nullptr;
											// �������� ��������� ������� ������������ 
											// ������ ������� F �����.
											break;
										}

									}


									// ������� ���.
									if (ibuffer_strongCFj != nullptr) {
										clear_hash_list_i(ibuffer_strongCFj);
										ibuffer_strongCFj = nullptr;
									}
									ibuffer_strongCFj = nullptr;
									// ������� � ���������� ���������.
									ibuffer_strongF_current = ibuffer_strongF_current->next;
								}
							}

							// ������������ ���.
							if (ibuffer_strongC != nullptr) {
								clear_hash_list_i(ibuffer_strongC);
								ibuffer_strongC = nullptr;
							}
							ibuffer_strongC = nullptr;
							if (ibuffer_strongF != nullptr) {
								clear_hash_list_i(ibuffer_strongF);
								ibuffer_strongF = nullptr;
							}
							ibuffer_strongF = nullptr;
							if (ivacant_F2C != nullptr) {
								clear_hash_list_i(ivacant_F2C);
								ivacant_F2C = nullptr;
							}
							ivacant_F2C = nullptr;

						}
					}
				} // �������� ��������� �������� C/F ���������. ������ 2.
			}
			else {
				// ������� ����������������� �����. ����� �������.

				// 01.01.2017 �������� ��������� �������� C/F ���������. ������ 2. 
				// ���� �� ���� F ����������, ���������� ����� ������� �������.
				// ����� Fi ������� F ���������� � � �� ��������� ������� �� �����.
				// ��������� ������ ��������� ��� Fi ���� ������������ �������.
				// Amat. ���������� ����� - threshold ��� ������ ������.
				// �. ������� ���� ������� � ������� � ����������� �������� ������.
				// C. ���� �� ��������� �������� F ������  (Fj), ��� ��� Fi � Fj ������ �������,
				// �� ���� ���� ������� � ������� ���� Fj � ��������� �� ��� �������� ������.
				// � ������� ��������� ������� �� �������� ����� ���������� ��� �������������� ��������������� ��������
				// ������ �� ������� ����� � �����.
				// D. ���� ����� � ���� ���� �� ������ �� ������.
				// E. ���� ������ �������� � ���� �� ���������� �� ���� �� ����� Fi ��� Fj ���������� � �����.
				// ����� Fi � Fj ��� ���������� � ����� � �������� ������ ������� F �������. ���� � ����� ���� Fj 
				// �� �������� ������ � ������� ���� Fi �����������. ���� � ����� ���� ���� Fi �� �� ����������� ��������� Fi 
				// ��������� ���� ���������� Fj ����� � F ���.
				//  30.12.2016
				if ((my_amg_manager.icoarseningtype == 1) || ((my_amg_manager.icoarseningtype == 3))) { // RS2 ������ 2.
					for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) if (this_is_F_node[i_1] == true) {
						// i_1 ��� F ���������� Fi.
						//Amat.���������� ����� - threshold ��� ������ ������.
						doublerealT thresholdRS = -1.0;
						integer i_2 = row_startA[i_1];

						// ������� ���-�������.
						clear_hash_table_Gus_struct01();
						// ��������� ������ �� ��������� ������ � ���-������� ��� ������ � ������ � Amat.i[i_2].
						//!!!TODOinteger imarker75_scan = 0;
						//!!!TODOformirate_F_SiTranspose_hash_table_Gus_struct02(hash_StrongTranspose_collection1[Amat.i[i_2]], imarker75_scan);


						integer iend_merker_position = row_startA[Amat.i[i_2] + 1] - 1;
						if (!btreshold_on_new_vetv) {
							for (integer is0 = i_2; (is0 <= iend_merker_position); is0++) {
								if (Amat.j[is0] != Amat.i[i_2]) {
									if (Amat.aij[is0] < 0.0) {
										if (fabs(Amat.aij[is0]) > thresholdRS) thresholdRS = fabs(Amat.aij[is0]);
									}
								}
							}
						}
						else {
							// �������� ����� ����: 11.06.2017.
							thresholdRS = threshold_quick_only_negative[Amat.i[i_2]];
						}
						if (thresholdRS > 0.0) {
							// ��������� ������� �� ����� � ����� ����� thresholdRS.
							hashlist_i* ivacant_F2C = nullptr;
							//  �. ������� ���� ������� � ������� � ����������� �������� ������.
							hashlist_i* ibuffer_strongC = nullptr;
							integer ibuffer_strongC_marker = -1;
							integer inumber_strongF_count_Fi = 0;
							hashlist_i* ibuffer_strongF = nullptr;
							integer ibuffer_strongF_marker = -1;
							for (integer is0 = i_2; (is0 <= iend_merker_position); is0++) {
								if (Amat.j[is0] != Amat.i[i_2]) {
									if (Amat.aij[is0] < 0.0) {
										if (fabs(Amat.aij[is0]) > theta(ilevel)* thresholdRS) {
											if (this_is_C_node[Amat.j[is0]] == true) {
												ibuffer_strongC_marker++;
												insertion_list_i(ibuffer_strongC, Amat.j[is0]);
												insert_hash_table_Gus_struct01(Amat.j[is0]);// 11.08.2018
											}
											if (this_is_F_node[Amat.j[is0]] == true) {

												//if (1) 19.01.2017
												if (1) {// if (0) 11.08.2018
														// ������� 19.01.2017

													if (hash_StrongTranspose_collection1 != nullptr) {
														//data_BalTreeST dat_key;
														//dat_key.i = Amat.j[is0];
														if (isfound(hash_StrongTranspose_collection1[Amat.i[i_2]], Amat.j[is0])) {
															// ����� ������� 19.01.2017
															// ���� ������-�� ������ �� ������� ��� ����������???  TODO

															// ������� Fj ����� ������.
															// �������� Fi � Fj ������ �������.
															inumber_strongF_count_Fi++;
															ibuffer_strongF_marker++;
															insertion_list_i(ibuffer_strongF, Amat.j[is0]);
														}
													}
													else {
														// ������� Fj ����� ������.
														// �������� Fi � Fj ������ �������.
														inumber_strongF_count_Fi++;
														ibuffer_strongF_marker++;
														insertion_list_i(ibuffer_strongF, Amat.j[is0]);
													}

												}
												else {
													// ������� Fj ����� ������.
													// �������� Fi � Fj ������ �������.
													inumber_strongF_count_Fi++;
													ibuffer_strongF_marker++;

													insertion_list_i(ibuffer_strongF, Amat.j[is0]);
												}
											}
										}
									}
								}
							}
							
							// ���������� ������ ibuffer_strongC �� �����������.
							// ������������� ������������ iusage_old_version = 0
							// ��� ������� ������������ ������������� ����������������� ���-�������.
							// ����������� ��������� ������� ����� ������� ������ ��� ���������� RS2 coarsening
							// �� 7.5% �� ��������� � �������� ������� �� �������. 
							// ��������� �������� ������������� � ������������� ��������� ����������.
							// 11.06.2017.
							//integer iusage_old_version = 0; // 1 ������ ������� ������. // 0 ����� ������ �� ������ ���-�������.
							

							// ������ ���������� � ��������� ������ ���������� ���-�������.
							// ������� ���-�������.
							clear_hash_table_Gus_struct01();
							hashlist_i* ibuffer_strongC_scan = ibuffer_strongC;
							while (ibuffer_strongC_scan != nullptr) {
								insert_hash_table_Gus_struct01(ibuffer_strongC_scan->item);
								ibuffer_strongC_scan = ibuffer_strongC_scan->next;
							}
							ibuffer_strongC_scan = nullptr;


							// ���������� �������������� ������� ��� ���������� � ����!!!

							// ��� ������� F ������ �������� � ����� ibuffer_strongF. 
							hashlist_i* ibuffer_strongF_current = ibuffer_strongF;
							for (integer i_3 = 0; i_3 <= ibuffer_strongF_marker; i_3++) {
								if (ibuffer_strongF_current != nullptr) {
									// ��������� ���� ������� F ������� ���������������.
									//1. ���������� threshold ��� Fj.
									doublerealT thresholdRS1 = -1.0;
									integer i_4 = row_startA[ibuffer_strongF_current->item];
									integer iend_merker_position1 = row_startA[Amat.i[i_4] + 1] - 1;
									if (!btreshold_on_new_vetv) {
										for (integer is01 = i_4; (is01 <= iend_merker_position1); is01++) {
											if (Amat.j[is01] != Amat.i[i_4]) {
												if (Amat.aij[is01] < 0.0) {
													if (fabs(Amat.aij[is01]) > thresholdRS1) thresholdRS1 = fabs(Amat.aij[is01]);
												}
											}
										}
									}
									else {
										// �������� ����� ����: 11.06.2017.
										thresholdRS1 = threshold_quick_only_negative[Amat.i[i_4]];
									}
									integer inumber_strongF_count_Fj = 0;
									// ������� ����� thresholdRS1.

									hashlist_i* ibuffer_strongCFj = nullptr;
									integer ibuffer_strongCFj_marker = -1;
									for (integer is01 = i_4; (is01 <= iend_merker_position1); is01++) {
										if (Amat.j[is01] != Amat.i[i_4]) {
											if (Amat.aij[is01] < 0.0) {
												if (fabs(Amat.aij[is01]) > theta(ilevel)* thresholdRS1) {
													if (this_is_C_node[Amat.j[is01]] == true) {
														ibuffer_strongCFj_marker++;
														insertion_list_i(ibuffer_strongCFj, Amat.j[is01]);
													}
													if (this_is_F_node[Amat.j[is01]] == true) {
														inumber_strongF_count_Fj++;
													}
												}
											}
										}
									}

									// � ibuffer_strongCFj ������ ������� � �������.

									// ���� �� ����� � ���� �� �������� �����.
									// ������ �� ������ ������ ibuffer_strongC
									// ������������� ������.
									// ��������� ���. ������  ibuffer_strongCFj_marker 
									// �������� ������� � ���� ��������������� ������� 
									// �� ��� ��� ���� �� ���������� �������� �����.
									bool bfound_32 = false;
									hashlist_i* ibuffer_strongCFj_scan = ibuffer_strongCFj;
									
									// ������ �� ������ ���-�������.
									while ((bfound_32 == false) && (ibuffer_strongCFj_scan != nullptr)) {
										// ���������� ������� �� ������ �� ������.
										bfound_32 = isfound_hash_table_Gus_struct01(ibuffer_strongCFj_scan->item);
										ibuffer_strongCFj_scan = ibuffer_strongCFj_scan->next;
									}
									
									ibuffer_strongCFj_scan = nullptr;

									if (bfound_32 == false) {
										// ���� �� ��� ������ � �����.
										if ((ibuffer_strongF_current->item > i_1) && (inumber_strongF_count_Fj >= inumber_strongF_count_Fi)) {
											// ���� Fj ��������� � ��� �� ������������� ����� ������ F ����� �
											// � ���� �� ��������� � F ����� Fi ������ ������� F ������.								

											// Fj ���������� �.
											insertion_list_i(ivacant_F2C, ibuffer_strongF_current->item);
											this_is_C_node[ibuffer_strongF_current->item] = true;
											this_is_F_node[ibuffer_strongF_current->item] = false;
											ibuffer_strongC_marker++;
											inumber_strongF_count_Fi--;
											insertion_list_i(ibuffer_strongC, ibuffer_strongF_current->item);											

											hashlist_i* ibuffer_strongC_scan_1 = ibuffer_strongC;
											
											// ������� ���-������� � ��������� � �� �����.
											clear_hash_table_Gus_struct01();
											while (ibuffer_strongC_scan_1 != nullptr) {
												insert_hash_table_Gus_struct01(ibuffer_strongC_scan_1->item);
												ibuffer_strongC_scan_1 = ibuffer_strongC_scan_1->next;
											}
											
											ibuffer_strongC_scan_1 = nullptr;
											// ���������� �������������� ������� ��� ���������� � ����!!!

										}
										else {
											// Fi ���������� �.
											this_is_C_node[i_1] = true;
											this_is_F_node[i_1] = false;
											// ���������� ��� Fj � � �� F.
											hashlist_i* ivacant_F2C_marker = ivacant_F2C;
											while (ivacant_F2C_marker != nullptr) {
												this_is_F_node[ivacant_F2C_marker->item] = true;
												this_is_C_node[ivacant_F2C_marker->item] = false;
												ivacant_F2C_marker = ivacant_F2C_marker->next;
											}
											ivacant_F2C_marker = nullptr;
											if (ivacant_F2C != nullptr) {
												clear_hash_list_i(ivacant_F2C);
												ivacant_F2C = nullptr;
											}
											ivacant_F2C = nullptr;
											// ������� ���.
											if (ibuffer_strongCFj != nullptr) {
												clear_hash_list_i(ibuffer_strongCFj);
												ibuffer_strongCFj = nullptr;
											}
											ibuffer_strongCFj = nullptr;
											// �������� ��������� ������� ������������ 
											// ������ ������� F �����.
											break;
										}

									}


									// ������� ���.
									if (ibuffer_strongCFj != nullptr) {
										clear_hash_list_i(ibuffer_strongCFj);
										ibuffer_strongCFj = nullptr;
									}
									ibuffer_strongCFj = nullptr;
									// ������� � ���������� ���������.
									ibuffer_strongF_current = ibuffer_strongF_current->next;
								}
							}

							// ������������ ���.
							if (ibuffer_strongC != nullptr) {
								clear_hash_list_i(ibuffer_strongC);
								ibuffer_strongC = nullptr;
							}
							ibuffer_strongC = nullptr;
							if (ibuffer_strongF != nullptr) {
								clear_hash_list_i(ibuffer_strongF);
								ibuffer_strongF = nullptr;
							}
							ibuffer_strongF = nullptr;
							if (ivacant_F2C != nullptr) {
								clear_hash_list_i(ivacant_F2C);
								ivacant_F2C = nullptr;
							}
							ivacant_F2C = nullptr;

						}
					}
				} // �������� ��������� �������� C/F ���������. ������ 2.
			}


		}
		// ����������� ����������� ������ ��� ������ �����. 
		// ��� ������ C/F ��������� ��������� ������ hash_StrongTranspose_collection1 � ������ ��� �� ������������.
		if (bStrongTransposeON) {
			// ������������ ���.

			// ������� �������� ������.
			if (hash_StrongTranspose_collection1 != nullptr) {
				//for (integer i_1 = 0; i_1 <= n_a[ilevel - 2]; i_1++)
				//isize_memory_alloc_hash_StrongTranspose_collection1
				for (integer i_1 = 0; i_1 <= isize_memory_alloc_hash_StrongTranspose_collection1; i_1++)
				{
					clear_list(hash_StrongTranspose_collection1[i_1]);
				}
				delete[] hash_StrongTranspose_collection1;
				hash_StrongTranspose_collection1 = nullptr;
			}

			if (isize_hash_StrongTranspose_collection != nullptr) {
				free(isize_hash_StrongTranspose_collection);
				isize_hash_StrongTranspose_collection = nullptr;
			}
		}

		// ����� ��������� ���������� ���� �������,
		// ���� F ���� �������� ����� ������� ��� ������� �� ��� ���� ������� � �����,
		// �� ������ ����� ���� ����� ���� � �������� ���������� ��������� ������ �� ���� ����������.
		// ������� ����� ������������� ��������� � ������ ������ (�������� �����).

		if (C_numerate != nullptr) {
			free(C_numerate);
			C_numerate = nullptr;
		}
		C_numerate = my_declaration_array<integer>(n_a[ilevel - 1], 0, "C_numerate");

		icounter = 1;
		ap_coarse = nullptr;

		// ����� � ��� ����� ��������� �� ����������� � ������� �� �� ���� ������, �� ��� 
		// ������� ��� ����� �������� � ������� ������� ���� ������������.
		//bool no_FeedBack = true;
		

		if (bprint_mesage_diagnostic) {
			std::cout << "   ***   CAMG PROLONGATOR "<< ilevel <<"  ***\n";
		}

		if (!bPMIS) {
			bweSholdbeContinue = true;
			while (bweSholdbeContinue) {
				bweSholdbeContinue = false;

				integer n_coarce = 0;
#pragma omp parallel for reduction(+:n_coarce)
				for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) {
					if (this_is_C_node[i_1]) {
						n_coarce++;
					}
				}
				
				// debug
				// �������� �������� C/F ���������.
				//doublerealT* exp1 = new doublerealT[n_a[ilevel - 1] + 1];
				//for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) exp1[i_1] = 0.0;
				//for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) if (this_is_C_node[i_1]) exp1[i_1] = 2.0;
				//for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) if (this_is_F_node[i_1]) exp1[i_1] = 1.0;
				//exporttecplot(exp1,n);
				//delete[] exp1;

				//std::cout<<"export ready";

				//system("pause");



				// C/F ��������� ���������, ����� ����� ��������� �������� ������������.
				// ����� ����� �������� ��������, ��� ����������������� �������� ������������.
				// �� ��������� ���������� ������� ������ ��������� ������ � ����� ��������� ����� �������.

				// ���������� ��������� ������������: 
				// coarse 2 fine.
				//P*coarse==fine


				// ���������� (����������) ���� ������ �����.
#pragma omp parallel for
				for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) C_numerate[i_1] = 0;
				icounter = 1;
				for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) if (this_is_C_node[i_1] == true) {

					//std::cout<<"C ind= "<< i_1; system("PAUSE");

					C_numerate[i_1] = icounter;
					icounter++;
				}

				

				// C_numerate - ������������� �� ��������� Coarse �����.
				// ���������� ����������� ��� ����� ������� ���������� ������ �����.
				icount1 = 1 + iaddR; // nnz_R
				for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) if (this_is_C_node[i_1] == true) {
					P[icount1].aij = 1.0;
					P[icount1].i = C_numerate[i_1]; // coarse number
					P[icount1].j = i_1; // fine number.
					icount1++;
					if (icount1 >= nsizePR * n) {
						std::cout<<"memory error!!!"<< std::endl;
						std::cout<<"not enough memory for the interpolation operator."<< std::endl;
						//system("PAUSE");
						//exit(1);
						deallocate_prolongation(nsizePR, n, P);
					}
				}


				// �������� icount1 ����� �����.�� ������� !!!.
				numberofcoarcenodes = icount1 - 1 - iaddR;



				// ��� ����������� R  ���� transpose(P)/ap.
				if (bprint_mesage_diagnostic) {

					std::cout << "number of coarce nodes=" << numberofcoarcenodes << "\n"; 

					if (debug_reshime) system("pause");
				}


				if (ap_coarse != nullptr) {
					free(ap_coarse);
					ap_coarse = nullptr;
				}
				ap_coarse = my_declaration_array<doublerealT>(numberofcoarcenodes, 0.0, "ap_coarse");



				// ��� ������� � ���� ���������� � ap_coarse[C_numerate[i8]] 
				// ������ ������������� ��������.
#pragma omp parallel for
				for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) {
					if (this_is_C_node[i8] == true) {
						// ������ ������ �� 10 ������ 2016. ����� O(log2(nnz))
						//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
						// 10 ������ 2016 ����� ������ �� ������ �����������. ����� O(1).
						integer ii1 = row_startA[i8];
						// �������� ����� ������ ����������� ���������� ������ ������ �������������.
						//for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
						integer iend_marker_position_1 = row_startA[Amat.i[ii1] + 1] - 1;
						for (integer is0 = ii1; (is0 <= iend_marker_position_1); is0++) {
							//std::cout<<"i="<< Amat.i[is0] <<" j=" << Amat.j[is0] <<" Amat.aij[is0]="<< Amat.aij[is0]<<" "<< std::endl;
							
							if (Amat.j[is0] == Amat.i[ii1]) {

								if (fabs(Amat.aij[is0]) > RealMAXIMUM) {
									std::cout<<"overflow error #1: fabs(Amat.aij["<< is0 <<"]) > RealMAXIMUM \n";
									system("pause");
								}
								ap_coarse[C_numerate[i8]] = fabs(Amat.aij[is0]);
								//std::cout<<"find = "<< fabs(Amat.aij[is0]) << std::endl;
							}
						}
					}
					//std::cout<<std::endl;
					//system("pause");
				}

				//std::cout<<"incoming="<< my_amg_manager.number_interpolation_procedure<< std::endl;

			//system("pause");


				

				// ����� 2 �������.				

				my_interpolation_procedure_universal<doublerealT>(the_number_of_neighbors_that_are_not_C_nodes,
						number_of_F_nodes_with_one_single_strong_C_neighbor,
						n_a, this_is_F_node, row_startA,
						nnz_a, bpositive_connections, Amat,
						bweSholdbeContinue, this_is_C_node, iadditionalCstatistic,
						RealZERO, icount1, P, nsizePR, ilevel,
						iadd, n, C_numerate,
						number_of_F_nodes_with_one_single_strong_C_neighborF,
						btreshold_on_new_vetv, ifrom_re_operation_protection,
						from_re_operation_protection0, magic82, threshold_quick_all,
						threshold_quick_only_negative);
								

				if (bprint_mesage_diagnostic) {
					std::cout<<"Additional C nodes in interpolation procedure. Statistics:"<< std::endl;
					std::cout << "number firstable C nodes=" << n_coarce <<", number secondary C nodes="<< iadditionalCstatistic <<"\n";
				}

				if (bweSholdbeContinue) {
					//delete[] ap_coarse;
					if (ap_coarse != nullptr) {
						free(ap_coarse);
						ap_coarse = nullptr;
					}
					if (bprint_mesage_diagnostic) {
						std::cout<<"Feedback restart..."<< std::endl;
					}
				}

				if (bprint_mesage_diagnostic) {
					// ��������� ����������� ����� � ���������� � ����� �� ���������� ������.
					//std::cout << "addition C nodes "<< (doublerealT)(100.0*iadditionalCstatistic / n_a[ilevel - 1])<<"%\n";
					// ��������� ���������� ����������� � ����� � ��������������� ���������� � ����� �� ������ ������.
					std::cout << "addition C nodes = "<< (doublerealT)(100.0 * iadditionalCstatistic / n_coarce) <<"% firstable C nodes,  level population "<<(doublerealT)(100.0 * (n_coarce + iadditionalCstatistic) / n_a[ilevel - 1]) <<"%\n";
				}
				iadditionalCstatistic = 0;
				//system("pause");


			}
		}
		else {
		    //������������ ������������ PMIS
			
			bweSholdbeContinue = true;
			while (bweSholdbeContinue) {
				bweSholdbeContinue = false;

				integer n_coarce = 0;
#pragma omp parallel for reduction(+:n_coarce)
				for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) {
					if (this_is_C_node[i_1]) {
						n_coarce++;
					}
				}
				
				// C/F ��������� ���������, ����� ����� ��������� �������� ������������.
				// ����� ����� �������� ��������, ��� ����������������� �������� ������������.
				// �� ��������� ���������� ������� ������ ��������� ������ � ����� ��������� ����� �������.

				// ���������� ��������� ������������: 
				// coarse -> fine (2 == ->).
				// P*coarse==fine


				// ���������� (����������) ���� ������ �����.
#pragma omp parallel for
				for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) C_numerate[i_1] = 0;
				icounter = 1;
				for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) if (this_is_C_node[i_1] == true) {
					//std::cout << "C ind= "<< i_1 << std::endl; system("PAUSE");

					C_numerate[i_1] = icounter;
					icounter++;
				}

				
				// C_numerate - ������������� �� ��������� Coarse �����.
				// ���������� ����������� ��� ����� ������� ���������� ������ �����.
				icount1 = 1 + iaddR; // nnz_R
				for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) if (this_is_C_node[i_1] == true) {
					P[icount1].aij = 1.0;
					P[icount1].i = C_numerate[i_1]; // coarse number
					P[icount1].j = i_1; // fine number.
					icount1++;
					if (icount1 >= nsizePR * n) {
						std::cout << "memory error!!!"<< std::endl;
						std::cout << "not enough memory for the interpolation operator."<< std::endl;
						//system("PAUSE");
						//exit(1);
						deallocate_prolongation(nsizePR, n, P);
					}
				}


				// �������� icount1 ����� �����.�� ������� !!!.
				numberofcoarcenodes = icount1 - 1 - iaddR;



				// ��� ����������� R  ���� transpose(P)/ap.
				if (bprint_mesage_diagnostic) {

					std::cout << "number of coarce nodes=" << numberofcoarcenodes << "\n";

					if (debug_reshime) system("pause");
				}


				if (ap_coarse != nullptr) {
					free(ap_coarse);
					ap_coarse = nullptr;
				}
				ap_coarse = my_declaration_array<doublerealT>(numberofcoarcenodes, 0.0, "ap_coarse");



				// ��� ������� � ���� ���������� � ap_coarse[C_numerate[i8]] 
				// ������ ������������� ��������.
#pragma omp parallel for
				for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) {
					if (this_is_C_node[i8] == true) {
						// ������ ������ �� 10 ������ 2016. ����� O(log2(nnz))
						//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
						// 10 ������ 2016 ����� ������ �� ������ �����������. ����� O(1).
						integer ii1 = row_startA[i8];
						// �������� ����� ������ ����������� ���������� ������ ������ �������������.
						//for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
						integer iend_marker_position_1 = row_startA[Amat.i[ii1] + 1] - 1;
						for (integer is0 = ii1; (is0 <= iend_marker_position_1); is0++) {
							//std::cout << "i="<<Amat.i[is0]<<" j="<<Amat.j[is0]<<" Amat.aij[is0]="<< Amat.aij[is0]<<" "<<std::endl;

							if (Amat.j[is0] == Amat.i[ii1]) {

								if (fabs(Amat.aij[is0]) > RealMAXIMUM) {
									std::cout << "overflow error: fabs(Amat.aij["<< is0 <<"]) > RealMAXIMUM !" << std::endl;
									system("pause");
								}
								ap_coarse[C_numerate[i8]] = fabs(Amat.aij[is0]);
								//std::cout << "find = "<< fabs(Amat.aij[is0])<<std::endl;
							}
						}
					}
					//std::cout << std::endl;
					//system("PAUSE");
				}
				
				//std::cout << "incoming="<< my_amg_manager.number_interpolation_procedure <<std::endl;
				
				//system("PAUSE");


				integer is_1 = row_startA[1];
				integer is_e = row_startA[n_a[ilevel - 1] + 1] - 1;
				// ������� ���� ��� ��������� ������ ��������.
				for (integer iscan = is_1; iscan <= is_e; iscan++) {
					Amat.abs_aij[iscan] = fabs(Amat.aij[iscan]);
				}

				// ������������ PMIS
				// 1.04.2017; 28.04.2017;
				// ������� ���� � ��� ����� ��������� ������������ �� ������, �������� ������������� ������������ � �������� �������������,
				// � ����� ����������� � �� � ��.
					

				// ���������������� ��������� �3.
				// ���������� ������� �������.
				doublereal theta_loc = theta(ilevel);
				doublereal theta83_loc = theta83(ilevel);
				// 31,01,2020. ������������ ��� ������� �� ����������� �� ������ ������ � ����, �������� � ����
				// ��� � ������ F ��� strong C ������, ���� �� ������. ��� �������� � ���������� ���������� 
				// ����������. ����������� ����� �������� C ���� ����� � ������� F ��� ���� �� ���� Strong C ����.
				my_interpolation_procedure_number3A_PMIS(the_number_of_neighbors_that_are_not_C_nodes,
					number_of_F_nodes_with_one_single_strong_C_neighbor,
					n_a, this_is_F_node, row_startA,
					nnz_a, bpositive_connections, Amat,
					bweSholdbeContinue, this_is_C_node, iadditionalCstatistic,
					RealZERO, icount1, P, nsizePR, ilevel,
					iadd, theta_loc, n, C_numerate,
					number_of_F_nodes_with_one_single_strong_C_neighborF,
					theta83_loc, btreshold_on_new_vetv, ifrom_re_operation_protection,
					from_re_operation_protection0, magic82, threshold_quick_all,
					threshold_quick_only_negative);
				/*
				// ����� ���������� ���������� ����������� � ����� � ���������� ����������� ����������.
				my_interpolation_procedure_number3A(the_number_of_neighbors_that_are_not_C_nodes,
					number_of_F_nodes_with_one_single_strong_C_neighbor,
					n_a, this_is_F_node, row_startA,
					nnz_a, bpositive_connections, Amat,
					bweSholdbeContinue, this_is_C_node, iadditionalCstatistic,
					RealZERO, icount1, P, nsizePR, ilevel,
					iadd, theta_loc, n, C_numerate,
					number_of_F_nodes_with_one_single_strong_C_neighborF,
					theta83_loc, btreshold_on_new_vetv, ifrom_re_operation_protection,
					from_re_operation_protection0, magic82, threshold_quick_all,
					threshold_quick_only_negative);
					*/

				if (bprint_mesage_diagnostic) {
					std::cout << "Additional C nodes in interpolation procedure. Statistics:\n";
					std::cout << "number firstable C nodes=" << n_coarce << ", number secondary C nodes="<< iadditionalCstatistic <<"\n";
				}

				if (bweSholdbeContinue) {
					//delete[] ap_coarse;
					if (ap_coarse != nullptr) {
						free(ap_coarse);
						ap_coarse = nullptr;
					}
					if (bprint_mesage_diagnostic) {
						std::cout << "Feedback restart...\n";
					}
				}

				if (bprint_mesage_diagnostic) {
					// ��������� ����������� ����� � ���������� � ����� �� ���������� ������.
					//std::cout << "addition C nodes "<< (doublerealT)(100.0*iadditionalCstatistic / n_a[ilevel - 1]) <<"%\n";
					// ��������� ���������� ����������� � ����� � ��������������� ���������� � ����� �� ������ ������.
					std::cout << "addition C nodes = " << (doublerealT)(100.0 * iadditionalCstatistic / n_coarce) <<"% firstable C nodes,  level population " << (doublerealT)(100.0 * (n_coarce + iadditionalCstatistic) / n_a[ilevel - 1])<<"%\n";
				}
				iadditionalCstatistic = 0;
				//system("pause");

			}

        }
		nnzR = icount1 - iaddR;
		if (bprint_mesage_diagnostic) {
			std::cout << "Prolongation operator complexity = "<< (doublerealT)(1.0*icount1 / n) << "*n\n";
		}
		//system("pause");

		// ����� ���������� nnzR ���������� ��������� ��������� � ������� R � P.

		// �������� restriction �������� � �� ���������� �� i.
		// ����� ��������� ��������� nnzR-1.
		// P=Copy(R);
		iend_marker_position = iaddR + nnzR - 1;

		// truncation of interpolation.
		// 30.04.2017.
		if (my_amg_manager.itruncation_interpolation == 1) {

			/*
			// ������������ ������� �������� � ��� ����������,
			// ��� ������� � ��� ��� �������� ������������ ��� �������������� ��� ������������ �� j.
			switch (imy_sort_algorithm ) {
			case COUNTING_SORT_ALG:
			//Counting_Sortj(P, 1 + iaddR, iaddR + nnzR - 1, false);
			qsj(P, 1 + iaddR, iaddR + nnzR - 1);
			//HeapSort(P, 1 + iaddR, iaddR + nnzR - 1, comparej);
			break;
			case QUICK_SORT_ALG:
			qsj(P, 1 + iaddR, iaddR + nnzR - 1);
			// ������������ ��������. O(nlog(n)).
			// �� ���������� ������ ������.
			//std::sort(P + (1 + iaddR) * sizeof(Ak1), P + (iaddR + nnzR - 1+1) * sizeof(Ak1), compAi);
			break;
			case HEAP_SORT_ALG:
			HeapSort(P, 1 + iaddR, iaddR + nnzR - 1, comparej);
			break;
			case TIM_PETERSON_SORT_ALG:
				// ���������� ���� ���������.
				timSort_amg_j(P, 1 + iaddR, iaddR + nnzR - 1);
				break;
			default:
			//Counting_Sortj(P, 1 + iaddR, iaddR + nnzR - 1, false);
			qsj(P, 1 + iaddR, iaddR + nnzR - 1);
			//HeapSort(P, 1 + iaddR, iaddR + nnzR - 1, comparej);
			break;
			}
			*/


			// ������� ����� ������ ����������� ��������� ��������� ��������.
			// ������� ������ ������ � ��������� ������������, �������� � ���������� 
			// ���������� ��� ������������.
			// �������� ���������� ������ ������:
			//doublerealT const alpha_truncation = 0.2;
			doublerealT alpha_truncation = my_amg_manager.truncation_interpolation;
			// ���������� ������ ������ ��������� ������������.
			// ������ ����� ��������� ������ ������ ������� �����.
			// ����� ������������ �� ������ ������� ������� �����.
			// ������ ��� �������� � ��������� ������������ ������� ����� 
			// ������� ������ ������������� �� ������ ����-�� ����� * �� alpha_truncation.
			// ������� ������������������� ����� ����� �������� ����������.
			// ������� ��� � ������ P. 17.02.2019
#pragma omp parallel for
			for (integer i_1 = 1; i_1 <= n_a[ilevel-1]; i_1++) {
				flag[i_1] = false; // init flag.
			}
			integer icounter_truncation = iend_marker_position + 1;//1 + iaddR;

			if (1) {
				// ������������� ������.

				integer i_size_75 = 0;
				// ��� ������ ��������������.
				for (integer ii = 1 + iaddR; ii <= iend_marker_position; ii++) {
					if (flag[P[ii].j] == false) {
						//row_ind_SRloc[P[ii].j] = ii;
						flag[P[ii].j] = true;
						//i_size_75++;
						if (P[ii].j > i_size_75) i_size_75 = P[ii].j;
					}
				}

#pragma omp parallel for
				for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) {
					flag[i_1] = false; // init flag.
				}

				// inicialization �����������.
				integer* row_ind_SRloc = my_declaration_array<integer>(i_size_75, -1, "row_ind_SRloc");

				//std::cout << "number of coarcenodes=" << numberofcoarcenodes << " i_size_75=" << i_size_75 <<"\n";

				//system("pause");

				// ��� ������ ��������������.
				for (integer ii = 1 + iaddR; ii <= iend_marker_position; ii++) {
					if (flag[P[ii].j] == false) {
						row_ind_SRloc[P[ii].j] = ii;
						flag[P[ii].j] = true;
					}
				}

				//for (integer ii = 1 + iaddR; ii <= iend_marker_position; ii++) {
				// ��� ������ ��������������. 06.07.2019
				// �������� ������������ P ����������� ������ ���������������.
				//#pragma omp parallel for
				for (integer i_75 = 1; i_75 <= i_size_75; i_75++) {
					if (row_ind_SRloc[i_75] != -1) {
						integer ii = row_ind_SRloc[i_75];

						//if (flag[P[ii].j] == false) {
						//flag[P[ii].j] = true;
						integer istr_65 = P[ii].j;
						integer ii_65 = ii;
						doublerealT dsum_plus = 0.0;
						doublerealT dsum_minus = 0.0;
						doublerealT dmax_plus = -1.0;
						doublerealT dmax_minus = -1.0;
						while ((ii_65 <= iend_marker_position) && (P[ii_65].j == istr_65)) {
							if (P[ii_65].aij > 0) {
								dsum_plus += P[ii_65].aij;
								if (P[ii_65].aij > dmax_plus) dmax_plus = P[ii_65].aij;
							}
							if (P[ii_65].aij < 0) {
								dsum_minus += fabs(P[ii_65].aij);
								if (fabs(P[ii_65].aij) > dmax_minus) dmax_minus = fabs(P[ii_65].aij);
							}
							ii_65++;
						}
						ii_65 = ii;
						doublerealT dsum_plus_new = 0.0;
						doublerealT dsum_minus_new = 0.0;
						
						while ((ii_65 <= iend_marker_position) && (P[ii_65].j == istr_65)) {
							if ((P[ii_65].aij > 0) && (fabs(P[ii_65].aij) > alpha_truncation*dmax_plus)) {
								dsum_plus_new += fabs(P[ii_65].aij);
							}
							if ((P[ii_65].aij < 0) && (fabs(P[ii_65].aij) > alpha_truncation*dmax_minus)) {
								dsum_minus_new += fabs(P[ii_65].aij);
							}
							ii_65++;
						}
						// ���������� ���������������������.
						ii_65 = ii;
						while ((ii_65 <= iend_marker_position) && (P[ii_65].j == istr_65)) {
							if ((P[ii_65].aij > 0) && (fabs(P[ii_65].aij) > alpha_truncation*dmax_plus)) {
								P[icounter_truncation] = P[ii_65];
								P[icounter_truncation].aij = fabs(dsum_plus / dsum_plus_new)*P[ii_65].aij;
								icounter_truncation++;
							}
							if ((P[ii_65].aij < 0) && (fabs(P[ii_65].aij) > alpha_truncation*dmax_minus)) {
								P[icounter_truncation] = P[ii_65];
								P[icounter_truncation].aij = fabs(dsum_minus / dsum_minus_new)*P[ii_65].aij;
								icounter_truncation++;
							}
							ii_65++;
						}

					}
				}

				free(row_ind_SRloc);
				row_ind_SRloc = nullptr;
			}
			else {

				// ������������ ������.

				for (integer ii = 1 + iaddR; ii <= iend_marker_position; ii++) {
					if (flag[P[ii].j] == false) {
						flag[P[ii].j] = true;
						integer istr_65 = P[ii].j;
						integer ii_65 = ii;
						doublerealT dsum_plus = 0.0;
						doublerealT dsum_minus = 0.0;
						doublerealT dmax_plus = -1.0;
						doublerealT dmax_minus = -1.0;
						while ((ii_65 <= iend_marker_position) && (P[ii_65].j == istr_65)) {
							if (P[ii_65].aij > 0) {
								dsum_plus += P[ii_65].aij;
								if (P[ii_65].aij > dmax_plus) dmax_plus = P[ii_65].aij;
							}
							if (P[ii_65].aij < 0) {
								dsum_minus += fabs(P[ii_65].aij);
								if (fabs(P[ii_65].aij) > dmax_minus) dmax_minus = fabs(P[ii_65].aij);
							}
							ii_65++;
						}
						ii_65 = ii;
						doublerealT dsum_plus_new = 0.0;
						doublerealT dsum_minus_new = 0.0;
						
						while ((ii_65 <= iend_marker_position) && (P[ii_65].j == istr_65)) {
							if ((P[ii_65].aij > 0) && (fabs(P[ii_65].aij) > alpha_truncation*dmax_plus)) {
								dsum_plus_new += fabs(P[ii_65].aij);
							}
							if ((P[ii_65].aij < 0) && (fabs(P[ii_65].aij) > alpha_truncation*dmax_minus)) {
								dsum_minus_new += fabs(P[ii_65].aij);
							}
							ii_65++;
						}
						// ���������� ���������������������.
						ii_65 = ii;
						while ((ii_65 <= iend_marker_position) && (P[ii_65].j == istr_65)) {
							if ((P[ii_65].aij > 0) && (fabs(P[ii_65].aij) > alpha_truncation*dmax_plus)) {
								P[icounter_truncation] = P[ii_65];
								P[icounter_truncation].aij = fabs(dsum_plus / dsum_plus_new)*P[ii_65].aij;
								icounter_truncation++;
							}
							if ((P[ii_65].aij < 0) && (fabs(P[ii_65].aij) > alpha_truncation*dmax_minus)) {
								P[icounter_truncation] = P[ii_65];
								P[icounter_truncation].aij = fabs(dsum_minus / dsum_minus_new)*P[ii_65].aij;
								icounter_truncation++;
							}
							ii_65++;
						}

					}
				}
			}

			// ������ (�������� �����������).
			integer ist_in_P = 1 + iaddR;
			// �� ���������� ����� ������������ � ����� ������� ������������ P.
			// ��� ������ ���������������� ��� ����.
//#pragma omp parallel for
			for (integer ii = iend_marker_position+1; ii <= icounter_truncation-1; ii++) {
				P[ist_in_P++] = P[ii];
			}
			iend_marker_position = ist_in_P - 1;

			//iend_marker_position = iaddR + nnzR - 1;
			nnzR = iend_marker_position - iaddR + 1;
			//nnzR = icount1 - iaddR;
			icount1 = nnzR + iaddR;

#pragma omp parallel for
			for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) {
				flag[i_1] = false; // init flag.
			}
		}

		

		// ���� �������� ����� ��� ���������� grid complexity ��� ��������� 
		// ������������ � ��������. ������ ���������� ����� ��� ����������� ���������� ���������� ������.
		if (ilevel - 1 == 0) {
			nnz_P_memo_0 = iend_marker_position - (iaddR + 1) + 1;
		}
		nnz_P_memo_all = iend_marker_position;
		

		// ��� �� ���� ��������� �� ap, �.�. 
		// R=P/ap. ????  
		// ��� ������ �� ����!!! �.�. � ������ R=transpose(P).

		// ���������� ��������� ������������ P �� ������� 17.02.2018
		// heapsort(P,key==i,iaddR+1,iaddR+nnzR - 1);

		
		switch (imy_sort_algorithm) {
		case COUNTING_SORT_ALG:
			Counting_Sort(P, 1 + iaddR, iaddR + nnzR - 1, false, numberofcoarcenodes, indx_comparei);// numberofcoarcenodes <-> n_a[ilevel - 1]
			break;
		case QUICK_SORT_ALG:
			//qs_abbys_heigh = 0;
			//qs(P, 1 + iaddR, iaddR + nnzR - 1);
			// ������������ ��������. O(nlog(n)).
			// �� ���������� ������ ������.
			std::sort(P + 1 + iaddR, P + iaddR + nnzR - 1 + 1, compareAk1R);
			break;
		case HEAP_SORT_ALG:
			//HeapSort(P, 1 + iaddR, iaddR + nnzR - 1,comparei);
			//LeftistHeapSort(P, 1 + iaddR, iaddR + nnzR - 1);
			mySTDHeapSort(P, 1 + iaddR, iaddR + nnzR - 1, indx_comparei);
			break;
		case TIM_PETERSON_SORT_ALG:
			// ���������� ���� ��������� 2002�.
			//timSort_amg(P, 1 + iaddR, iaddR + nnzR - 1, indx_comparei);
			gfx::timsort(P + 1 + iaddR, P + iaddR + nnzR - 1 + 1, compareAk1R);
			break;
		default:
			Counting_Sort(P, 1 + iaddR, iaddR + nnzR - 1, false, numberofcoarcenodes, indx_comparei);// numberofcoarcenodes <-> n_a[ilevel - 1]
			break;
		}
		

		if (bprint_mesage_diagnostic) {

			std::cout << "first level size n="<< n <<"; number of coarcenodes="<< numberofcoarcenodes <<", procent = "<<100.0*numberofcoarcenodes/n<<"%\n";
		}

		// �������� Restriction ��� �� ��������� ����� ��� ������������: 
		// ���� R ������ P ������������� �� ������� (�����������������).
		if (1) {// �������� �����������. ��� �������� �� ������ � ����� �������
			// ��������������� �������������� ���������.
#pragma omp parallel for
			for (integer i_1 = 1; i_1 <= numberofcoarcenodes; i_1++) {
				flag[i_1] = false; // init flag.
			}
			for (integer i_1 = 1 + iaddR; i_1 <= iaddR + nnzR - 1; i_1++) {
				if (flag[P[i_1].i] == false)
				{
					doublerealT dsum27 = 0.0;
					for (integer i_2 = i_1; (i_2 <= iaddR + nnzR - 1) && (P[i_2].i == P[i_1].i); i_2++) {
						dsum27 += fabs(P[i_2].aij);
					}
					if (dsum27 < 1.0e-37) {
						std::cout << "fatal error!!! zero string R["<< P[i_1].i <<"][j]="<< dsum27 <<std::endl;

						system("PAUSE");
					}
					flag[P[i_1].i] = true;
				}
			}
			for (integer i_1 = 1; i_1 <= numberofcoarcenodes; i_1++) {
				if (flag[i_1] == false) {
					//06.07.2019
					// ������� ������ ����� i_1
					std::cout << "fatal error!!! string number " << i_1 << " propushena\n";

					system("PAUSE");
				}
			}
#pragma omp parallel for
			for (integer i_1 = 1; i_1 <= numberofcoarcenodes; i_1++) {
				flag[i_1] = false; // init flag.
			}
		}

		
		if (ap_coarse != nullptr) {
			free(ap_coarse);
			ap_coarse = nullptr;
		}
	
		// ������������ ����������� ������ �� ��� ���-�������.02.02.2019
		free_hash_table_Gus_struct01();

		if (bprint_mesage_diagnostic) {
			std::cout << "Prolongation ierarhion..." << std::endl;
		}
		if (b_REALLOC) {
			my_realloc_memory<Ak1>(P, ((integer)(nnz_P_memo_all)+2));
			
			if (bprint_mesage_diagnostic) {
				std::cout << "2 of 3 compleated. OK!! ierarhion matrix Prolongation realloc successfully..." << std::endl;
			}
		}

		// MARKER GUSTAVSON

		// ���������� ������� �������������� ������:
		// Acorse=R*Afine*P;
		// ����� 1: R*Afine.
		//         xxxxxx
		//         xxxxxx
		//  xxxxxx xxxxxx xxxxxx
		//  xxxxxx xxxxxx xxxxxx
		//         xxxxxx
		//         xxxxxx
		//    R       Amat     [RA]


		if (bprint_mesage_diagnostic) {
			std::cout << "   ***   CAMG GALERKIN MULTIPLICATOR " << ilevel <<"  ***\n";

			std::cout << "nnz left operand=" << nnzR << ", nnz right operand="<< nnz_a[ilevel - 1] <<"\n";

		}

		integer istartAnew;


		// ���� ��������� IBM 1978.
		// 23 ������� 2015 ����.

		// ����� 1: R*Afine.
		//         xxxxxx
		//         xxxxxx
		//  xxxxxx xxxxxx xxxxxx
		//  xxxxxx xxxxxx xxxxxx
		//         xxxxxx
		//         xxxxxx
		//    R       Amat     [RA]
		// ���������� � �� �������.
		/*
		// 7 ������ 2016. ���������� ������� � ���� ��������� �������� ��� ������ ��������� ������� ������.
		switch (imy_sort_algorithm) {
		case COUNTING_SORT_ALG:
		Counting_Sort(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd);
		break;
		case QUICK_SORT_ALG :
		qs(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd);
		// ������������ ��������. O(nlog(n)).
		// �� ���������� ������ ������.
		//std::sort(Amat + (1 + iadd)*sizeof(Ak1), Amat + (nnz_a[ilevel - 1] + iadd)*sizeof(Ak1), compAi);
		break;
		case HEAP_SORT_ALG:
		HeapSort(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd, comparei);
		break;
		case TIM_PETERSON_SORT_ALG:
			// ���������� ���� ���������.
			timSort_amg(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd);
			break;
		default:
		Counting_Sort(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd);
		break;
		}
		*/
		// �������������� � ������� CRS.

		if (row_ind_SR != nullptr) {
			free(row_ind_SR);
			row_ind_SR = nullptr;
		}
		row_ind_SR = my_declaration_array<integer>(numberofcoarcenodes, -1, "row_ind_SR");
		if (row_ind_ER != nullptr) {
			free(row_ind_ER);
			row_ind_ER = nullptr;
		}
		row_ind_ER = my_declaration_array<integer>(numberofcoarcenodes, -2, "row_ind_ER");

		
		
#pragma omp parallel for
		for (integer i = 1; i <= icounter - 1; i++) {
			flag[i] = false;
		}

		integer istart1 = 1 + iaddR;
		integer iend1 = nnzR - 1 + iaddR;
		// ���� R ������ ����������������� (������������� �� �������) �������� ������������.
		calculate_row_ptr(istart1, iend1, row_ind_SR, row_ind_ER, flag, P);
		
			if (bQuick_sort_for_reorder) {
				for (integer ii = 1; ii <= numberofcoarcenodes; ii++) {
					// 14.04.2020
					// � ������ ������ i ������� ������� j ������������� �� �����������.		
					//qs_abbys_heigh = 0;
					// quicksort
					//qsj(P, row_ind_SR[ii], row_ind_ER[ii]);
					std::sort(P + row_ind_SR[ii], P + row_ind_ER[ii] + 1, compareAk1P);
				}
			}

#pragma omp parallel for
		for (integer i = 1; i <= n_a[ilevel - 1]; i++) {
			flag[i] = false;
		}			

		istartAnew = nnz_a[ilevel - 1] + 1 + iadd;
		istartAnew_mem = istartAnew;

		// ������ ������������ ��� ���������� ������������� �����.
		if (vector_sum != nullptr) {
			free(vector_sum);
			vector_sum = nullptr;
		}
		vector_sum = my_declaration_array<doublerealT>(n_a[ilevel - 1], 0.0, "vector_sum");

		// ������ ������� ��������� ��������� � ��������������� �������.
		if (index_visit != nullptr) {
			free(index_visit);
			index_visit = nullptr;
		}
		index_visit = my_declaration_array<integer>(n_a[ilevel - 1], 0, "index_visit");


		// hash_table nnz+1
		// ��������� ������� ���-�������.
		// �������� ������ ������� ������������� �������� ���� ��������.
		// ������ �� 0 �� nnz ������������.
		bool* hash_table = my_declaration_array<bool>(n_a[ilevel - 1], false, "hash_table");


		//#ifdef _NONAME_STUB29_10_2017
#ifdef _OPENMP

		// ������������ ��������� R*A. ��������� ������� � � ������� � ������� istartAnew.
        // P - ��� R, Amat - ��� �.
		my_parallel8_sparse_matrix_by_matrix_multiplication_RA<doublereal>(Amat,
			P, istartAnew, istartAnew_m,
			row_ind_SR, row_ind_ER, row_startA, 
			numberofcoarcenodes, iKnumber_thread,
			hash_table_m, index_visit_m,
			vector_sum_m, index_size_m,
			nsizeA,n,nnz, ilevel,
			bprint_mesage_diagnostic, n_a, AccumulqtorA_m_SIZE8,
			AccumulqtorA_m, istartAnew_mem);

#else

		// ��������� R*A. ��������� ������� � � ������� � ������� istartAnew.
		// P - ��� R, Amat - ��� �.
		  my_sparse_matrix_by_matrix_multiplication_RA<doublereal>(Amat, P, istartAnew,
			      row_ind_SR, row_ind_ER, row_startA, numberofcoarcenodes,
				  // ��������������� ������.
				  hash_table, index_visit, vector_sum, nsizeA);

#endif

		
		if (index_visit != nullptr) {
			free(index_visit);
			index_visit = nullptr;
		}
		if (row_ind_SR!=nullptr) {
			free(row_ind_SR);
			row_ind_SR = nullptr;
		}
		if (row_ind_ER != nullptr) {
			free(row_ind_ER);
			row_ind_ER = nullptr;
		}
		
		if (vector_sum != nullptr) {
			free(vector_sum);
			vector_sum = nullptr;
		}



		// ����� 2. [R*Afine]*P=Abuf*P.
		// ���������� [R*�] �� i.
		//heapsort(Amat, key=i*n_coarce + j, 1, nnz_a[ilevel - 1]);

		// � ���������� ������ ��������� ������������ ���������� ��������� �� �. ����������,
		// �� � ��� ����� ��������������� �� ������� ���������, ������� �������������� 
		// ���������� �� ���������. ��� ��������� 11 ������ 2016.
		// 11 ������ 2016.

		


		// Prolongation ������ ���� ����������� �� j.
		// ��������� ������� ��������� ������� �������������� ������.
		integer istartAnew2 = istartAnew;



		// ������� ����� ���� �� ������ ���� ������� ������� ��� �� �����.
		// 17 ������� 2015. ����� ��������� � ������� ����������.
		if (bprint_mesage_diagnostic) {

			std::cout << "nnz left operand=" << istartAnew - (nnz_a[ilevel - 1] + 1 + iadd) << ", nnz right operand="<< nnzR <<"\n";
		}
		
		// ���� ��������� IBM 1978
		// � ���� ���� ���������� ���� �� ������ ���������,
		// � �� ����� ��� � ���������� ����������� ���������� ������ ����:
		// (�������, �������, ����������) ������� ���� �������������� ������ �������
		// �� ��������� (���������) � ��������� �������� 30 � 1. 30 ��������� �� ���� ������������.
		// 23 ������� 2015 ����.
		// 6 ������ 2016 ���� ��������� ��� ������.


		// ������� ������ ��������� ����� ����������.
		// IBM 1978 Sparse Matrix multiplication.

		// ���������� ����������� ���������.
		// �������������� ����� ������ � ������ CRS.
		// ���������� ������� ������������ �� ��������.
		switch (imy_sort_algorithm) {
		case COUNTING_SORT_ALG:
			Counting_Sort(P, 1 + iaddR, iaddR + nnzR - 1, n_a[ilevel - 1], indx_comparej);//�������� ������ n_a[ilevel - 1]
			break;
		case QUICK_SORT_ALG:
			//qs_abbys_heigh = 0;
			//qsj(P, 1 + iaddR, iaddR + nnzR - 1);
			// ������������ ��������. O(nlog(n)).
			// �� ���������� ������ ������.
			std::sort(P + 1 + iaddR, P + iaddR + nnzR - 1+1, compareAk1P);
			break;
		case HEAP_SORT_ALG:
			//HeapSort(P, 1 + iaddR, iaddR + nnzR - 1, comparej);
			//LeftistHeapSort_j(P, 1 + iaddR, iaddR + nnzR - 1);
			mySTDHeapSort(P, 1 + iaddR, iaddR + nnzR - 1, indx_comparej);
			break;
		case TIM_PETERSON_SORT_ALG:
			// ���������� ���� ��������� 2002.
			//timSort_amg(P, 1 + iaddR, iaddR + nnzR - 1, indx_comparej);
			gfx::timsort(P + 1 + iaddR, P + iaddR + nnzR - 1 + 1, compareAk1P);
			break;
		default:
			Counting_Sort(P, 1 + iaddR, iaddR + nnzR - 1, n_a[ilevel - 1], indx_comparej);//�������� ������ n_a[ilevel - 1]
			break;
		}

		if (row_ind_AS != nullptr) {
			free(row_ind_AS);
			row_ind_AS = nullptr;
		}
		row_ind_AS = my_declaration_array<integer>(numberofcoarcenodes, -1, "row_ind_AS");
		if (row_ind_AE != nullptr) {
			free(row_ind_AE);
			row_ind_AE = nullptr;
		}
		row_ind_AE = my_declaration_array<integer>(numberofcoarcenodes, -2, "row_ind_AE");

		
#pragma omp parallel for
		for (integer i = 1; i <= n; i++) {
			flag[i] = false;
		}
		integer istart2 = nnz_a[ilevel - 1] + 1 + iadd;
		integer iend2 = istartAnew - 1;
		calculate_row_ptr(istart2, iend2, row_ind_AS, row_ind_AE, flag, Amat);

		if (bQuick_sort_for_reorder) {
			for (integer ii = 1; ii <= numberofcoarcenodes; ii++) {
				// 14.04.2020
				// � ������ ������ i ������� ������� j ������������� �� �����������.		
				qs_abbys_heigh = 0;
				// quicksort
				qsj(Amat, row_ind_AS[ii], row_ind_AE[ii]);
			}
		}

		// ������������� ����������� �����, �.�. 
		// ����������� ������������ ������ ������ �������
		// ���� ��������� ������������.
		if (row_ind_PS != nullptr) {
			free(row_ind_PS);
			row_ind_PS = nullptr;
		}
		row_ind_PS = my_declaration_array<integer>(n_a[ilevel - 1], -1, "row_ind_PS");
		if (row_ind_PE != nullptr) {
			free(row_ind_PE);
			row_ind_PE = nullptr;
		}
		row_ind_PE = my_declaration_array<integer>(n_a[ilevel - 1], -2, "row_ind_PE");

		

		integer istart4 = 1 + iaddR;
		integer iend4 = nnzR - 1 + iaddR;
#pragma omp parallel for
		for (integer i = 1; i <= n_a[ilevel - 1]; i++) {//n
			flag[i] = false;
		}
		calculate_row_ptr_j(istart4, iend4, row_ind_PS, row_ind_PE, flag, P);

		if (bQuick_sort_for_reorder) {
			for (integer ii = 1; ii <= n_a[ilevel - 1]; ii++) {
				// 14.04.2020
				// � ������ ������ j ������� ������� i ������������� �� �����������.		
				//qs_abbys_heigh = 0;
				// quicksort
				//qs(P, row_ind_PS[ii], row_ind_PE[ii]);
				std::sort(P + row_ind_PS[ii], P + row_ind_PE[ii] + 1, compareAk1R);
			}
		}

		// ���������� ����������.
		//vector_sum = new doublerealT[numberofcoarcenodes + 1];
		/*
		// ������ ��� ���� ����������� ���� �� ������ ���������.
		if (vector_sum != nullptr) {
			free(vector_sum);
			vector_sum = nullptr;
		}
		*/
		// ������ ������������ ��� ���������� ������������� �����.
		if (vector_sum != nullptr) {
			free(vector_sum);
			vector_sum = nullptr;
		}
		vector_sum = my_declaration_array<doublerealT>(numberofcoarcenodes, 0.0, "vector_sum");

		//integer size_v = sizeof(doublerealT)*(1 + numberofcoarcenodes);
		// ������ ������� ��������� ��������� � ��������������� �������.
		if (index_visit != nullptr) {
			free(index_visit);
			index_visit = nullptr;
		}
		index_visit = my_declaration_array<integer>(n_a[ilevel - 1], 0, "index_visit");

		if (index_visit != nullptr) {
			index_visit[0] = 0;
		}

		//#ifdef _NONAME_STUB29_10_2017
#ifdef _OPENMP

		my_parallel8_sparse_matrix_by_matrix_multiplication_AP<doublerealT>(Amat,
			P,  istartAnew, istartAnew_m,
			row_ind_AS, row_ind_AE, row_ind_PS, row_ind_PE,
			numberofcoarcenodes,
			iKnumber_thread,
			hash_table_m, index_visit_m,
			vector_sum_m, index_size_m,
			nsizeA, n, nnz, ilevel,
			bprint_mesage_diagnostic,
			n_a, AccumulqtorA_m_SIZE8,
			AccumulqtorA_m, istartAnew2,
			// ��� ������������� ��������� �����
	        // � ������������� ����������
			ibsp_length, i_bsp_LIMIT, bsp, iVar,
			bcontinue_global);

#else

		// ��������� ����������� ������� A �� ����������� ������� P.
		// ��������� ������������ � ������� � ������� � ������� istartAnew2.
		my_sparse_matrix_by_matrix_multiplication_AP<doublerealT>(Amat,
			P, istartAnew2,
			row_ind_AS, row_ind_AE, row_ind_PS, row_ind_PE, numberofcoarcenodes,
			// ��������������� ������.
			hash_table, index_visit, vector_sum, nsizeA,
			// ��� ������������� ��������� �����
			// � ������������� ����������
			ibsp_length, i_bsp_LIMIT, ilevel, bsp);

#endif

		
		if (hash_table != nullptr) {
			free(hash_table);
			hash_table = nullptr;
		}
		
		if (vector_sum != nullptr) {
			free(vector_sum);
			vector_sum = nullptr;
		}
		if (index_visit != nullptr) {
			free(index_visit);
			index_visit = nullptr;
		}
		
		if (row_ind_AS != nullptr) {
			free(row_ind_AS);
			row_ind_AS = nullptr;
		}
		if (row_ind_AE != nullptr) {
			free(row_ind_AE);
			row_ind_AE = nullptr;
		}
		if (row_ind_PS != nullptr) {
			free(row_ind_PS);
			row_ind_PS = nullptr;
		}
		if (row_ind_PE != nullptr) {
			free(row_ind_PE);
			row_ind_PE = nullptr;
		}

		
		// �������� ������� � ���������� ������ ����� �������� � ������� ��������������� ������.
		//integer icounter3 = 1;
		 integer nsize = istartAnew2 - (istartAnew);
		//doublereal mH = pow(icounter - 1, 0.33333);
		//doublereal alphaH = 2.0*(mH - 1)*(mH - 1) / ((2.0*mH - 1.0)*(2.0*mH - 1.0));
		//std::cout << "alphaH=" << alphaH << "\n";
		integer i_1start1 = nnz_a[ilevel - 1] + 1 + iadd;
		for (integer i_1 = i_1start1, i_2 = 1; i_2 <= nsize; i_1++, i_2++) {
			integer i_right_position = istartAnew - 1 + i_2;
			//if (my_amg_manager.baglomeration_with_consistency_scaling > 0) {
				// agglomeration with consistency scaling 24.05.2019
				//Amat.aij[i_1] = alphaH*Amat.aij[i_right_position];
			//}
			//else {
				Amat.aij[i_1] = Amat.aij[i_right_position];
			//}
			Amat.i[i_1] = Amat.i[i_right_position];
			Amat.j[i_1] = Amat.j[i_right_position];
		}

		if (bprint_mesage_diagnostic) {
			std::cout << "Prolongation is construct.\n";			 

			// ����� ���������� ����� F � ������� ��� �������� � �����.
			std::cout << "diagnostic: the number of neighbors that are not Coarse (C) nodes " << the_number_of_neighbors_that_are_not_C_nodes << "\n";
			// ���������� F ����� � ������� ������ ���� ���������������� � �����.
			std::cout << "diagnostic: the number of Fine (F) nodes with one single strong Coarse (C) neighbor=" << number_of_F_nodes_with_one_single_strong_C_neighbor << " \n";
			std::cout << "diagnostic: the number of Fine (F) nodes with one single strong Coarse (C) neighbor\n";
			std::cout << "and to the same not having strong Fine(F) neighbors " << number_of_F_nodes_with_one_single_strong_C_neighborF << "\n";
			//system("pause");

		}
		if (debug_reshime) system("pause");


		//delete[] C_numerate;
		if (C_numerate != nullptr) {
			free(C_numerate);
			C_numerate = nullptr;
		}

		// ������������� �������������� ���� F/C �������� ���������� ��������������� ��������,
		// �������� ����� V ������ ��������� ��� ���������� ����������.
		integer iaddFCcolor = 0;
		for (integer i_71 = 0; i_71 < ilevel - 1; i_71++) iaddFCcolor += n_a[i_71];
		for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) if (this_is_C_node[i_1] == true) {
			F_false_C_true[iaddFCcolor + i_1] = true;
		}

		nnz_aRP[ilevel - 1] = nnzR - 1;
		iaddR += nnzR - 1;
		n_a[ilevel] = icounter - 1;
		nnz_a[ilevel] = nsize;
		iadd += nnz_a[ilevel - 1];



		if (bcontinue_global) {
			// ���� bad string �� �����������.
			ilevel++;	
		}
		

		if (bStrongTransposeON) {
			// ������������ ���.

			// ������� �������� ������.
			if (hash_StrongTranspose_collection1 != nullptr) {
				//for (integer i_1 = 0; i_1 <= n_a[ilevel - 2]; i_1++)
				//isize_memory_alloc_hash_StrongTranspose_collection1
				for (integer i_1 = 0; i_1 <= isize_memory_alloc_hash_StrongTranspose_collection1; i_1++)
				{
					clear_list(hash_StrongTranspose_collection1[i_1]);
				}
				delete[] hash_StrongTranspose_collection1;
				hash_StrongTranspose_collection1 = nullptr;
			}

			if (isize_hash_StrongTranspose_collection != nullptr) {
				delete isize_hash_StrongTranspose_collection;
				isize_hash_StrongTranspose_collection = nullptr;
			}
		}

	
		if (count_neighbour != nullptr) {
			free(count_neighbour);
			count_neighbour = nullptr;
		}
		
		if (row_startA != nullptr) {
			free(row_startA);
			row_startA = nullptr;
		}


		// ���������� �������� ������� �������� ����������.
	//BAD_STRING_MARKER:

		
		if (bprint_mesage_diagnostic) {
			std::cout << "one level construct OK." << std::endl;
		}
		if (debug_reshime) system("pause");	

		//�������� �����

	} // �������� ����� ���������.

	if (bprint_mesage_diagnostic) {
		std::cout << "   ***   CAMG ITERATOR   ***" << std::endl;
	}


	 // ������������ ������ ������������ �� ����� ���������� �������� ������.
	 // ������������ ����������� ������.
	if (threshold_quick_all != nullptr) {
		free(threshold_quick_all);
		threshold_quick_all = nullptr;
	}

	if (threshold_quick_only_negative != nullptr) {
		free(threshold_quick_only_negative);
		threshold_quick_only_negative = nullptr;
	}	

	if (istack != nullptr) {
		free(istack);
		istack = nullptr;
	}

	if (hash_table2 != nullptr) {
		free(hash_table2);
		hash_table2 = nullptr;
	}

	if (this_is_C_node != nullptr) {
		free(this_is_C_node);
		this_is_C_node = nullptr;
	}
	if (this_is_F_node != nullptr) {
		free(this_is_F_node);
		this_is_F_node = nullptr;
	}

	ilevel--; // 4.01.2017
	if (n_a[ilevel] < 5) {
		// ����� �� ���� ��������� ������� ��� ������ 5 ����� �����.
		ilevel--;
	}

	

	// ��������� � ���������� grid complexity
	// ����������� ���������.
	doublerealT dr_grid_complexity = (((double)(1.0*iadd)) / ((double)(1.0*nnz_a[0])));
	if (bprint_mesage_diagnostic) {
		printf("grid complexity is %1.2f\n", dr_grid_complexity);
		printf("Prolongation operator complexity is |Psigma|/|P1|=%1.2f %1.2f*n\n", (doublerealT)(1.0*nnz_P_memo_all / nnz_P_memo_0), (doublerealT)(1.0*nnz_P_memo_all / n_a[0]));
		doublerealT sizegb = 16 * iadd / 1.0e9;
		printf("memory usage is %e Gb. reserved %e Gb. ratio is equal = %e\n", sizegb, 16 * nsizeA / 1.0e9, sizegb / (16 * nsizeA / 1.0e9));
	}

	

	// 31.224s [50.986] 2D m=81 debug x64 acumulqtor
	// 13.792 [18.156] 2D m=81 realese x64 acumulqtor
	// 8.028s 2D m=81 debug x64 rozetka
	// 3.827 2D m=81 realese x64 rozetka
	if (bprint_mesage_diagnostic) {
		std::cout << "number of levels=" << ilevel << std::endl;
		std::cout << "levels   unknowns        nonzeros     sample_pattern"<<std::endl;
		// <= ilevel 4.01.2017
		for (integer i_1 = 0; i_1 <= ilevel; i_1++) {
			if (i_1 == 0) {
				std::cout << i_1 << " \t " << n_a[i_1]<< "       " << nnz_a[i_1] << "    \t " << (integer)(nnz_a[i_1] / n_a[i_1]) << "\n";
			}
			else {
				std::cout << i_1 << " \t " << n_a[i_1] << " " << (100.0 * n_a[i_1] / n_a[i_1 - 1]) << "%   " << nnz_a[i_1] << " " << (100.0 * nnz_a[i_1] / nnz_a[i_1 - 1]) << "% \t " << (integer)(nnz_a[i_1] / n_a[i_1]) << "\n";
			}
		}
		std::cout << "Graph(Mesh) ierarhion is construct sucsseful..." << std::endl;
	}

	if (debug_reshime) system("pause");
	//system("pause");
	//exit(1);

	if (bprint_mesage_diagnostic) {
		std::cout << "memory optimization 13 november 2016."<< std::endl;
		std::cout << "ierarhion matrix Amat..."<< std::endl;
	}
		// ���������� ������ ��������� ��� �������� ������� �.
		// ������� ������ �������� � ������ �� ����� ��� ��� �� ����� � �� ���������� ������.
		my_realloc_memory<doublerealT>(Amat.aij, ((iadd + 2)));
		
		
		if (Amat.abs_aij != nullptr) {
			//delete[] Amat;
			free(Amat.abs_aij);
			Amat.abs_aij = nullptr;
		}

		my_realloc_memory<integer>(Amat.i, ((iadd + 2)));
				
		my_realloc_memory<integer>(Amat.j, ((iadd + 2)));
		
		
		if (bprint_mesage_diagnostic) {
			std::cout << " 1 of 3 compleated.  OK!! ierarhion matrix Amat realloc successfully..." << std::endl;
			std::cout << "Prolongation ierarhion..." << std::endl;
		}
		my_realloc_memory<Ak1>(P, ((integer)(nnz_P_memo_all)+2));
				
		if (bprint_mesage_diagnostic) {
			std::cout << "2 of 3 compleated. OK!! ierarhion matrix Prolongation realloc successfully..." << std::endl;
		}

		//#ifdef	_NONAME_STUB29_10_2017
#ifdef _OPENMP
	// ������������ ��� ��������� ��������� ����������� ������.
	// ��������!!!
		for (integer i_9 = 0; i_9 < iKnumber_thread; i_9++) {
			//free(vector_sum_m[i_9]);
			//free(index_visit_m[i_9]);
			//free(hash_table_m[i_9]);

			delete[] vector_sum_m[i_9];
			delete[] index_visit_m[i_9];
			delete[] hash_table_m[i_9];
		}
		delete[] vector_sum_m;
		delete[] index_visit_m;
		delete[] hash_table_m;
		delete[] index_size_m;
		vector_sum_m = nullptr;
		index_visit_m = nullptr;
		hash_table_m = nullptr;
		index_size_m = nullptr;

		for (integer i_9 = 0; i_9 < iKnumber_thread; i_9++) {
			delete[] AccumulqtorA_m[i_9];
			AccumulqtorA_m[i_9] = nullptr;
		}
		delete[] AccumulqtorA_m;
		AccumulqtorA_m = nullptr;
		delete[] istartAnew_m;
		istartAnew_m = nullptr;
#endif

		amg_pp.maxlevel = maxlevel;
		amg_pp.ilevel = ilevel;
		amg_pp.bprint_mesage_diagnostic = bprint_mesage_diagnostic;
		amg_pp.bonly_serial = bonly_serial;
		amg_pp.bILU2smoother = bILU2smoother;
		amg_pp.nnz_P_memo_0 = nnz_P_memo_0;
		amg_pp.nnz_P_memo_all = nnz_P_memo_all;
		amg_pp.dr_grid_complexity = dr_grid_complexity;
		amg_pp.debug_reshime = debug_reshime;
		amg_pp.dapply_ilu_max_pattern_size = dapply_ilu_max_pattern_size;
		amg_pp.RealZERO = RealZERO;
		amg_pp.identiti = identiti;
		amg_pp.memo_icoarseningtype = memo_icoarseningtype;
		

} // setup_phase_classic_aglomerative_amg6


template <typename doublerealT>
bool classic_aglomerative_amg6(Ak2& Amat,
	integer nsizeA, // ���������� ����� ���������� ����� ��� ��������� ������ �	
	integer nnz, // number of non zero elements
	integer n, // dimension of vectors x and b.	
	doublereal*& x, //solution (�������) 
	doublereal*& b, // rthdsd (������ �����).
	doublerealT& ret74,
	integer iVar,
	bool bmemory_savings,
	BLOCK*& my_body, integer& lb, integer maxelm_out
) {

	amg_precond_param amg_pp;
	
	// �� ����� 100 ��������� �����
	// � ������������� ����������.
	const int i_bsp_LIMIT = 100;
	BAD_STRING_PATCHING* bsp = nullptr;
	integer ibsp_length = 0;

	// �������� �������� ����� ��������� ������������ � ��������� �� ����������������.
	Ak1* P = nullptr;

	integer* n_a = nullptr; // ����� ����������� �� ������ �� �������.
	integer* nnz_a = nullptr; // ����� ��������� ������������� � ������� �
	bool* flag = nullptr;

	integer* nnz_aRP = nullptr;
	bool* F_false_C_true = nullptr;

	// �������������� �������������� �� �������� � ������ � 
	// ������� ��� ��� ����� ����� ��������������.
	// ��� ����� ������������� ���������.
	const bool bQuick_sort_for_reorder = false;

	// ���� ���������� ���������� ���� ��� ��� ������������� ������� ����.
	setup_phase_classic_aglomerative_amg6<doublerealT>(Amat,
		nsizeA, // ���������� ����� ���������� ����� ��� ��������� ������ �	
		nnz, // number of non zero elements
		n, // dimension of vectors x and b.	
		x, //solution (�������) 
		b, // rthdsd (������ �����).
		ret74,
		iVar,
		bmemory_savings,
		my_body, lb, maxelm_out,
		// ��������� ������ ������ ��� solution phase.
		amg_pp,
		n_a, nnz_a, nnz_aRP,
		ibsp_length,
		bsp,
		i_bsp_LIMIT,
		flag,
		F_false_C_true, P,
		bQuick_sort_for_reorder);


	INIT_SELECTOR_CASE_CAMG_RUMBAv_0_14 imyinit = ZERO_INIT;

	// ���� ������� ����� ���������� �����������,
	// ��������� ��������� ������ ����� ����.
	// ������� ���� �� ������ ��������.
	bool ret_value = false;
	ret_value = solution_phase<doublerealT>(Amat,
		nsizeA, // ���������� ����� ���������� ����� ��� ��������� ������ �	
		nnz, // number of non zero elements
		n, // dimension of vectors x and b.	
		x, // solution (�������) 
		b, // rthdsd (������ �����).
		ret74,
		iVar,
		bmemory_savings,
		my_body, lb, maxelm_out,
		// ������ ��� solution phase
		amg_pp,
		n_a, nnz_a, nnz_aRP,
		ibsp_length,
		bsp,
		i_bsp_LIMIT,
		flag,
		F_false_C_true, P,
		imyinit);


	
	if (P != nullptr) {
		//delete[] P;
		free(P);
		P = nullptr;
	}

	if (F_false_C_true != nullptr) {
		free(F_false_C_true);
		F_false_C_true = nullptr;
	}

	delete[] bsp;

	delete[] nnz_aRP;
	delete[] n_a;
	delete[] nnz_a;

	if (flag != nullptr) {
		free(flag);
		flag = nullptr;
	}

	return ret_value;
} // classic_aglomerative_amg6

// ����������� ���������� ������ amg1r5 ���������.
#include "amg1r5_nonlinear.cpp"

#endif /*CLASSIC_AGLOMERATIVE_AMG6_2018YEAR_CPP*/