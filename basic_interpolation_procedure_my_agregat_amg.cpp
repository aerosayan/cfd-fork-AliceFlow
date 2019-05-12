// �������� ������� ���������� ��������� ������������� � �������������� ������������� ������.

#pragma once
#ifndef BASIC_INTERPOLATION_PROCEDURE_MY_AGREGAT_AMG_CPP
#define BASIC_INTERPOLATION_PROCEDURE_MY_AGREGAT_AMG_CPP 1

// ���������������� ��������� �10.
void my_interpolation_procedure_number10(integer &the_number_of_neighbors_that_are_not_�_nodes,
	integer &number_of_F_nodes_with_one_single_strong_C_neighbor,
	integer* &n_a, bool* &this_is_F_node, integer* &row_startA,
	integer* &nnz_a, bool &bpositive_connections, Ak2 &Amat,
	bool &bweSholdbeContinue, bool* &this_is_C_node, integer &iadditionalCstatistic,
	const doublereal RealZERO, integer &icount1, Ak1* &P, integer &nsizePR, integer &ilevel,
	integer &iadd, doublereal &theta, integer &n,  integer* &C_numerate)
{
	// ����� ���� F �� ������� Strong � ������� ��� ���������� � �����.
	// ���� F ������� ������ Strong  � ������ �������� ���� �������� �� ����� Strong C ������.

	the_number_of_neighbors_that_are_not_�_nodes = 0;
	number_of_F_nodes_with_one_single_strong_C_neighbor = 0;
	// ���������� ����������� ��� ����� ������� ���������� F nodes.
	// ������ F-nodes ������ C-nodes.
	for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {

		// ��� ����� ����������� �������� ��������.
		// 5 ������� 2015 ���� �� ��������� ��������� �������������
		// ��������� ������������� � ������ � ��������� ��������.
		doublereal maxelem_threshold = -1.0;
		// O(log2(nnz))
		//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
		// O(1)
		integer ii1 = row_startA[i8];

		if (bpositive_connections) {
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
					//if (this_is_C_node[Amat.j[is0]] == true) {
					if (fabs(Amat.aij[is0]) > maxelem_threshold) {
						maxelem_threshold = fabs(Amat.aij[is0]);
					}
					//}
				}
			}
		}
		else {
			// 23_10_2016 ������ negative connections.
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
					//if (this_is_C_node[Amat.j[is0]] == true) {
					if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold)) {
						maxelem_threshold = fabs(Amat.aij[is0]);
					}
					//}
				}
			}
		}
		// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

		// ����� ������� ������� F-node ������� C-node.
		integer icsos = 0;

		// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
		// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
		doublereal sumP = 0.0;
		if (bpositive_connections) {
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					if (this_is_C_node[Amat.j[is0]] == true) {
						//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
						if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
							sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
							icsos++;
						}
					}
					else {
						// ������������ ���������� ������� ������� �� �������� � ������.
						the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
					}
				}
			}
		}
		else {
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					if (this_is_C_node[Amat.j[is0]] == true) {
						//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
						if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
							sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
							icsos++;
						}
					}
					else {
						// ������������ ���������� ������� ������� �� �������� � ������.
						the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
					}
				}
			}
		}
		if (icsos == 1) number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ �������  � �������.



		if ((false) && (icsos == 1)) {
			// 1 ������ 2016 ���� ����� ��� ������������.
			// ������� � ������ ������ ������ ������ ����� ���� � �����.
			this_is_F_node[i8] = false;
			this_is_C_node[i8] = true;
			bweSholdbeContinue = true;
		}
		else {



			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					if (this_is_C_node[Amat.j[is0]] == true) {

						if (fabs(sumP) < RealZERO) {
							//printf("error interpolation zero diagonal sumP.\n");
							//printf("Fnode all sosed is F");
							//system("pause");
							//printf("i8 is Dirichlet node\n");
							if (this_is_C_node[i8] == false) iadditionalCstatistic++;
							this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
							this_is_C_node[i8] = true;
							bweSholdbeContinue = true;
							//exit(1);
							// ����� ����� �������� �������������.
						}
						else {

							// ��������������� ������� �� ��������� � �����.

							if (bpositive_connections) {

								// ������ ������� ������ ����������� ��������� 
								// ������������� ��������� �������� �� �������� 
								// �������� �������.
								// ����������� 5 ������� 2015.
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
								if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
									P[icount1].j = i8;
									P[icount1].i = C_numerate[Amat.j[is0]];
									P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
									icount1++;
									if (icount1 >= nsizePR*n) {
										printf("memory error!!!\n");
										printf("not enough memory for the interpolation operator.\n");
										//system("PAUSE");
										//exit(1);
										deallocate_prolongation(nsizePR, n,  P);
									}
								}
							}
							else {
								// ������ ������� ������ ����������� ��������� 
								// ������������� ��������� �������� �� �������� 
								// �������� �������.
								// ����������� 5 ������� 2015.
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
								if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
									P[icount1].j = i8;
									P[icount1].i = C_numerate[Amat.j[is0]];
									P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
									icount1++;
									if (icount1 >= nsizePR*n) {
										printf("memory error!!!\n");
										printf("not enough memory for the interpolation operator.\n");
										//system("PAUSE");
										//exit(1);
										deallocate_prolongation(nsizePR, n,  P);
									}
								}
							}

						}

					}
				}
			}

		}

	}
} // my_interpolation_procedure_number10


  // ���������������� ��������� �7.
void my_interpolation_procedure_number7(integer &the_number_of_neighbors_that_are_not_�_nodes,
	integer &number_of_F_nodes_with_one_single_strong_C_neighbor,
	integer* &n_a, bool* &this_is_F_node, integer* &row_startA,
	integer* &nnz_a, bool &bpositive_connections, Ak2 &Amat,
	bool &bweSholdbeContinue, bool* &this_is_C_node, integer &iadditionalCstatistic,
	const doublereal RealZERO, integer &icount1, Ak1* &P, integer &nsizePR, integer &ilevel,
	integer &iadd, doublereal &theta, integer &n,  integer* &C_numerate)
{
	// ������� ����� ����������� �� ������. ����������� �� �������� 3.

	// ������ ������������� � ��������� ��������.
	// �������� ������������� ����������� ������ ��� F ����� �������
	// �� ����� � �������.
	// �� ���� ��� ������ ������������ �������� �������� �� ������� ������ �������������.
	// ���� F ������� ������ Strong  � ������ �������� ���� �������� �� ����� Strong C ������.
	// ���� F �� ������� Strong C �������, �������� �������� �� Strong C ������� �������� Strong F ����� �
	// � ���������� ������������ ������ Strong F �������.
	// ���� �� ����������� ��� Strongly ��������� F ���� � ������� � ������������ ��� ������ �� ������ Strong C ������
	// �� ���� �� ���� Strong F ����� ��������� � ����� � ������������ ������ ������� Strong F ������� ������� ���� F ������������.
	// ����� �� �������� ��������� �������� ���������� � ������ ��� ����������� � �����.


	the_number_of_neighbors_that_are_not_�_nodes = 0;
	number_of_F_nodes_with_one_single_strong_C_neighbor = 0;
	// ���������� ����������� ��� ����� ������� ���������� F nodes.
	// ������ F-nodes ������ C-nodes.
	for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {

		// ��� ����� ����������� �������� ��������.
		// 5 ������� 2015 ���� �� ��������� ��������� �������������
		// ��������� ������������� � ������ � ��������� ��������.
		doublereal maxelem_threshold = -1.0;
		// O(log2(nnz))
		//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
		// O(1)
		integer ii1 = row_startA[i8];
		if (bpositive_connections) {
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
					//if (this_is_C_node[Amat.j[is0]] == true) {
					if (fabs(Amat.aij[is0]) > maxelem_threshold) {
						maxelem_threshold = fabs(Amat.aij[is0]);
					}
					//}
				}
			}
		}
		else {
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
					//if (this_is_C_node[Amat.j[is0]] == true) {
					if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold)) {
						maxelem_threshold = fabs(Amat.aij[is0]);
					}
					//}
				}
			}
		}
		// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

		// ����� ������� ������� F-node ������� C-node.
		integer icsos = 0;

		// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
		// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
		doublereal sumP = 0.0;
		doublereal SumPall = 0.0;
		integer icount_StronglyF = 0;
		if (bpositive_connections) {
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					if (this_is_C_node[Amat.j[is0]] == true) {
						//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
						if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
							sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
							icsos++;
						}
					}
					else {
						if (this_is_F_node[Amat.j[is0]] == true) {
							if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
								SumPall += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
								icount_StronglyF++;
							}
						}
						// ������������ ���������� ������� ������� �� �������� � ������.
						the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
					}
				}
			}
		}
		else {
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					if (this_is_C_node[Amat.j[is0]] == true) {
						//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
						if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
							sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
							icsos++;
						}
					}
					else {
						if (this_is_F_node[Amat.j[is0]] == true) {
							if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
								SumPall += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
								icount_StronglyF++;
							}
						}
						// ������������ ���������� ������� ������� �� �������� � ������.
						the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
					}
				}
			}
		}
		if (icsos == 1) number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ �������  � �������.


																			   // 1 ������ 2015 ���� ����� ��� ������������.
																			   // ������� � ������ ������ ������ ������ ����� ���� � �����.
		if ((false) && (icsos == 1)) {
			this_is_F_node[i8] = false;
			this_is_C_node[i8] = true;
			bweSholdbeContinue = true;
		}
		else {

			if (fabs(sumP) < RealZERO) {
				//printf("error interpolation zero diagonal sumP.\n");
				//printf("Fnode all sosed is F");
				//system("pause");
				//printf("i8 is Dirichlet node\n");

				//exit(1);
				// ����� ����� �������� �������������.
				bool* empty_interpol = NULL;
				//empty_interpol = new bool[icount_StronglyF];
				empty_interpol = (bool*)malloc(icount_StronglyF * sizeof(bool));
				char c1[27] = "classic_aglomerative_amg_4";
				char c2[15] = "empty_interpol";
				handle_error(empty_interpol, c2, c1, icount_StronglyF);

				for (integer i_38 = 0; i_38 < icount_StronglyF; i_38++) {
					empty_interpol[i_38] = false; // initialization
				}

				integer i_38_count = 0;

				bool bCinsert = false;

				if (bpositive_connections) {

					// ��������� ������� F ������� ������� F ���� ��� �������� ���� ��������� �������������.
					for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							if (this_is_F_node[Amat.j[is0]] == true) {



								// ��������������� ������� �� ��������� � �����.

								// ������ ������� ������ ����������� ��������� 
								// ������������� ��������� �������� �� �������� 
								// �������� �������.
								// ����������� 5 ������� 2015.
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
								if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {

									integer iFpoint = Amat.j[is0];
									doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
									// ��������� ���� ������� iFpointeger 
									// ����� ����� ����� ��� � ����.

									// �������������� ��������� �����.
									doublereal maxelem_threshold_loc = -1.0;
									//O(log2(nnz))
									//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
									// O(1)
									integer ii1_loc = row_startA[iFpoint];
									for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
											//if (this_is_C_node[Amat.j[is0_loc]] == true) {
											// ����� ������������� ������ ������������� ������� C ���������� ������� ������.
											if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc) {
												maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
											}
											//}
										}
									}


									// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

									// ����� ������� ������� F-node ������� C-node.
									integer icsos_loc = 0;
									bool bvisit_sumPloc = false;

									// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
									// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
									doublereal sumP_loc = 0.0;
									for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {




											if (this_is_C_node[Amat.j[is0_loc]] == true) {
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
												if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
													sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
													icsos_loc++;
													bvisit_sumPloc = true;
													empty_interpol[i_38_count] = true;
												}
											}
											else {
												// ������������ ���������� ������� ������� �� �������� � ������.
												//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
											}
										}
									}


									if (bvisit_sumPloc) {
										// � ����� ��� ������� ����������������� ����� 
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													// ��������������� ������� �� ��������� � �����.

													// ������ ������� ������ ����������� ��������� 
													// ������������� ��������� �������� �� �������� 
													// �������� �������.
													// ����������� 5 ������� 2015.
													//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
													if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
														P[icount1].j = i8;
														P[icount1].i = C_numerate[Amat.j[is0_loc]];
														//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
														P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
														icount1++;
														if (icount1 >= nsizePR*n) {
															printf("memory error!!!\n");
															printf("not enough memory for the interpolation operator.\n");
															//system("PAUSE");
															//exit(1);
															deallocate_prolongation(nsizePR, n, P);
														}
													}
												}
											}
										}
									}
									else {
										// �� ���� � ������� ������������� ����� ���������� ��������.
										// ��� �������� ��� ����� ���� ��� ������ ��������� F ���� � ������� ������ ��� ������� � �������.
										// ������ �������� ����������� ��� �� ������ ������.
										//printf("problem situation");
										//getchar();
										this_is_F_node[iFpoint] = false;
										this_is_C_node[iFpoint] = true;
										bweSholdbeContinue = true;
										bCinsert = true;
										// �������� ������ ������������ ������� F ������� ������� F ���� ��� �������� ���� ��������������,
										// ����� � ����� �� ����� �������� ������� ����� � ����� �� ������������� �������.
										break;
										// ���� ��� ����� ����� �.�. �� ��������� ������������� ���������� �� ������ ������� �����.

									}

									i_38_count++;

								}
							}
						}
					}

				}
				else {
					// ��������� ������� F ������� ������� F ���� ��� �������� ���� ��������� �������������.
					for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							if (this_is_F_node[Amat.j[is0]] == true) {



								// ��������������� ������� �� ��������� � �����.

								// ������ ������� ������ ����������� ��������� 
								// ������������� ��������� �������� �� �������� 
								// �������� �������.
								// ����������� 5 ������� 2015.
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
								if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {

									integer iFpoint = Amat.j[is0];
									doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
									// ��������� ���� ������� iFpointeger 
									// ����� ����� ����� ��� � ����.

									// �������������� ��������� �����.
									doublereal maxelem_threshold_loc = -1.0;
									//O(log2(nnz))
									//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
									// O(1)
									integer ii1_loc = row_startA[iFpoint];
									for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
											//if (this_is_C_node[Amat.j[is0_loc]] == true) {
											// ����� ������������� ������ ������������� ������� C ���������� ������� ������.
											if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc)) {
												maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
											}
											//}
										}
									}


									// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

									// ����� ������� ������� F-node ������� C-node.
									integer icsos_loc = 0;
									bool bvisit_sumPloc = false;

									// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
									// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
									doublereal sumP_loc = 0.0;
									for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {




											if (this_is_C_node[Amat.j[is0_loc]] == true) {
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
												if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta)) {
													sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
													icsos_loc++;
													bvisit_sumPloc = true;
													empty_interpol[i_38_count] = true;
												}
											}
											else {
												// ������������ ���������� ������� ������� �� �������� � ������.
												//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
											}
										}
									}


									if (bvisit_sumPloc) {
										// � ����� ��� ������� ����������������� ����� 
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													// ��������������� ������� �� ��������� � �����.

													// ������ ������� ������ ����������� ��������� 
													// ������������� ��������� �������� �� �������� 
													// �������� �������.
													// ����������� 5 ������� 2015.
													//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
													if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta)) {
														P[icount1].j = i8;
														P[icount1].i = C_numerate[Amat.j[is0_loc]];
														//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
														P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
														icount1++;
														if (icount1 >= nsizePR*n) {
															printf("memory error!!!\n");
															printf("not enough memory for the interpolation operator.\n");
															//system("PAUSE");
															//exit(1);
															deallocate_prolongation(nsizePR, n,  P);
														}
													}
												}
											}
										}
									}
									else {
										// �� ���� � ������� ������������� ����� ���������� ��������.
										// ��� �������� ��� ����� ���� ��� ������ ��������� F ���� � ������� ������ ��� ������� � �������.
										// ������ �������� ����������� ��� �� ������ ������.
										//printf("problem situation");
										//getchar();
										this_is_F_node[iFpoint] = false;
										this_is_C_node[iFpoint] = true;
										bweSholdbeContinue = true;
										bCinsert = true;
										// �������� ������ ������������ ������� F ������� ������� F ���� ��� �������� ���� ��������������,
										// ����� � ����� �� ����� �������� ������� ����� � ����� �� ������������� �������.
										break;
										// ���� ��� ����� ����� �.�. �� ��������� ������������� ���������� �� ������ ������� �����.

									}

									i_38_count++;

								}
							}
						}
					}
				}

				if (!bCinsert) {
					// �������� ���� ������� ��� ��������� ������ 3��� 42� �� 3��� 29�.

					bool bstandart_vjzov = true;
					for (integer i_38 = 0; i_38 < icount_StronglyF; i_38++) {
						// 3 ������. �� ����� ���� ����� ������ �� ���������� �������
						// ����� � �� ����� ��� ���� ������ ������������� ���������, ����� 
						// �� ������������ ���� ���� � ����� �� ������ ��� �������.
						if (empty_interpol[i_38] == true) {
							bstandart_vjzov = false;
						}
					}
					if (bstandart_vjzov) {
						if (this_is_C_node[i8] == false) iadditionalCstatistic++;
						this_is_F_node[i8] = false;
						this_is_C_node[i8] = true;
						bweSholdbeContinue = true;
					}
				}

				free(empty_interpol);

			}



			else {

				if (bpositive_connections) {
					for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							if (this_is_C_node[Amat.j[is0]] == true) {



								// ��������������� ������� �� ��������� � �����.

								// ������ ������� ������ ����������� ��������� 
								// ������������� ��������� �������� �� �������� 
								// �������� �������.
								// ����������� 5 ������� 2015.
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
								if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
									P[icount1].j = i8;
									P[icount1].i = C_numerate[Amat.j[is0]];
									P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
									icount1++;
									if (icount1 >= nsizePR*n) {
										printf("memory error!!!\n");
										printf("not enough memory for the interpolation operator.\n");
										//system("PAUSE");
										//exit(1);
										deallocate_prolongation(nsizePR, n,  P);
									}
								}

							}
						}
					}

				}
				else {
					for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							if (this_is_C_node[Amat.j[is0]] == true) {



								// ��������������� ������� �� ��������� � �����.

								// ������ ������� ������ ����������� ��������� 
								// ������������� ��������� �������� �� �������� 
								// �������� �������.
								// ����������� 5 ������� 2015.
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
								if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
									P[icount1].j = i8;
									P[icount1].i = C_numerate[Amat.j[is0]];
									P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
									icount1++;
									if (icount1 >= nsizePR*n) {
										printf("memory error!!!\n");
										printf("not enough memory for the interpolation operator.\n");
										//system("PAUSE");
										//exit(1);
										deallocate_prolongation(nsizePR, n, P);
									}
								}

							}
						}
					}

				}
			} // standart

		} // i8

	}

} // my_interpolation_procedure_number7

  // ���������������� ��������� �2.
void my_interpolation_procedure_number2(integer &the_number_of_neighbors_that_are_not_�_nodes,
	integer &number_of_F_nodes_with_one_single_strong_C_neighbor,
	integer* &n_a, bool* &this_is_F_node, integer* &row_startA,
	integer* &nnz_a, bool &bpositive_connections, Ak2 &Amat,
	bool &bweSholdbeContinue, bool* &this_is_C_node, integer &iadditionalCstatistic,
	const doublereal RealZERO, integer &icount1, Ak1* &P, integer &nsizePR, integer &ilevel,
	integer &iadd, doublereal &theta, integer &n,  integer* &C_numerate,
	integer &number_of_F_nodes_with_one_single_strong_C_neighborF)
{
	// ������� ����� ����������� �� ������. ����������� �� �������� 3.

	// ������ ������������� � ��������� ��������. ����������������� �������. TODO.
	// �������� ������������� ����������� ������ ��� F ����� �������
	// �� ����� � ������� ��� ����� ������������� ����� � �������.
	// �� ���� ��� ������ ������������ �������� �������� �� ������� ������ �������������.

	// ���� ��������� ������������� ��������� �������� �� ��� ���� ������� �������������.
	// ���� � ��� ����� ���� F ������� ���� ������ Strong � ������ ���������� ��� ����� � ���� ���� �� ����� 
	// ����  C ������� �������� ������ �������� �������������.


	the_number_of_neighbors_that_are_not_�_nodes = 0;
	number_of_F_nodes_with_one_single_strong_C_neighbor = 0;
	number_of_F_nodes_with_one_single_strong_C_neighborF = 0;

	if (bpositive_connections) {
		// ���������� ����������� ��� ����� ������� ���������� F nodes.
		// ������ F-nodes ������ C-nodes.
		for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {

			// ��� ����� ����������� �������� ��������.
			// 5 ������� 2015 ���� �� ��������� ��������� �������������
			// ��������� ������������� � ������ � ��������� ��������.
			doublereal maxelem_threshold = -1.0;
			//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
			integer ii1 = row_startA[i8];
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
					//if (this_is_C_node[Amat.j[is0]] == true) {
					if (fabs(Amat.aij[is0]) > maxelem_threshold) {
						maxelem_threshold = fabs(Amat.aij[is0]);
					}
					//}
				}
			}
			// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

			// ����� ������� ������� F-node ������� C-node.
			integer icsos = 0;
			integer icsosF = 0;

			// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
			// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
			doublereal sumP = 0.0;
			doublereal SumPall = 0.0;
			integer icount_StronglyF = 0;
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					if (this_is_C_node[Amat.j[is0]] == true) {
						//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
						if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
							sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
							icsos++;
						}
					}
					else {
						if (this_is_F_node[Amat.j[is0]] == true) {
							if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
								SumPall += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
								icount_StronglyF++;
								icsosF++;
							}
						}
						// ������������ ���������� ������� ������� �� �������� � ������.
						the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
					}
				}
			}
			if (icsos == 1) {
				number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ ������� � �������.
																	   // ��������� ������ ������ "����������".
																	   // ���������� ������ ����������� ��� ���������.
																	   // � ������� �� �������� ������� ����� ���������� ������ ���������� ���� ��������� ������� � 
																	   // ������������� �� ���� ������� ����� ��������.
				if (icsosF == 0) number_of_F_nodes_with_one_single_strong_C_neighborF++; // ���������� F ����� � ����� ������������ ������� C ������� � � ����-�� �� ������� ������� F �������.
			}


			// 1 ������ 2015 ���� ����� ��� ������������.
			// ������� � ������ ������ ������ ������ ����� ���� � �����.
			if ((false) && (icsos == 1)) {
				this_is_F_node[i8] = false;
				this_is_C_node[i8] = true;
				bweSholdbeContinue = true;
			}
			else {

				if (fabs(sumP) < RealZERO) {
					//printf("error interpolation zero diagonal sumP.\n");
					//printf("Fnode all sosed is F");
					//system("pause");
					//printf("i8 is Dirichlet node\n");

					//exit(1);
					// ����� ����� �������� �������������.
					bool* empty_interpol = NULL;
					//empty_interpol = new bool[icount_StronglyF];
					empty_interpol = (bool*)malloc(icount_StronglyF * sizeof(bool));
					char c1[27] = "classic_aglomerative_amg_4";
					char c2[15] = "empty_interpol";
					handle_error(empty_interpol, c2, c1, icount_StronglyF);

					for (integer i_38 = 0; i_38 < icount_StronglyF; i_38++) {
						empty_interpol[i_38] = false; // initialization
					}

					integer i_38_count = 0;

					for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							if (this_is_F_node[Amat.j[is0]] == true) {



								// ��������������� ������� �� ��������� � �����.

								// ������ ������� ������ ����������� ��������� 
								// ������������� ��������� �������� �� �������� 
								// �������� �������.
								// ����������� 5 ������� 2015.
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
								if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {

									integer iFpoint = Amat.j[is0];
									doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
									// ��������� ���� ������� iFpointeger 
									// ����� ����� ����� ��� � ����.

									// �������������� ��������� �����.
									doublereal maxelem_threshold_loc = -1.0;
									//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
									integer ii1_loc = row_startA[iFpoint];
									for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
											//if (this_is_C_node[Amat.j[is0_loc]] == true) {
											// ����� ������������� ������ ������������� ������� � ���������� ������� ������.
											if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc) {
												maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
											}
											//}
										}
									}


									// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

									// ����� ������� ������� F-node ������� C-node.
									integer icsos_loc = 0;
									bool bvisit_sumPloc = false;

									// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
									// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
									doublereal sumP_loc = 0.0;
									for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {




											if (this_is_C_node[Amat.j[is0_loc]] == true) {
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
												if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
													sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
													icsos_loc++;
													bvisit_sumPloc = true;
													empty_interpol[i_38_count] = true;
												}
											}
											else {
												// ������������ ���������� ������� ������� �� �������� � ������.
												//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
											}
										}
									}


									if (bvisit_sumPloc) {
										// � ����� ��� ������� ����������������� ����� 
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													// ��������������� ������� �� ��������� � �����.

													// ������ ������� ������ ����������� ��������� 
													// ������������� ��������� �������� �� �������� 
													// �������� �������.
													// ����������� 5 ������� 2015.
													//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
													if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
														P[icount1].j = i8;
														P[icount1].i = C_numerate[Amat.j[is0_loc]];
														//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
														P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
														icount1++;
														if (icount1 >= nsizePR*n) {
															printf("memory error!!!\n");
															printf("not enough memory for the interpolation operator.\n");
															//system("PAUSE");
															//exit(1);
															deallocate_prolongation(nsizePR, n,  P);
														}
													}
												}
											}
										}
									}
									else {
										// �� ���� � ������� ������������� ����� ���������� ��������.
										// ��� �������� ��� ����� ���� ��� ������ ��������� F ���� � ������� ������ ��� ������� � �������.
										// ������ �������� ����������� ��� �� ������ ������ �����������.
										//printf("problem situation");
										//getchar();
										this_is_F_node[iFpoint] = false;
										this_is_C_node[iFpoint] = true;
										bweSholdbeContinue = true;
										// �������� ������ ������������ ������� F ������� ������� F ���� ��� �������� ���� ������������,
										// ����� � ����� �� ����� �������� ������� ����� � ����� �� ������ ������� ����������.
										break;
										// ���� ��� ����� ����� �.�. �� ��������� ������������� ���������� �� ������ ������ �����.
									}

									i_38_count++;

								}
							}
						}
					}

					bool bstandart_vjzov = true;
					for (integer i_38 = 0; i_38 < icount_StronglyF; i_38++) {
						// 3 ������. �� ����� ���� ����� ������ �� ���������� �������
						// ����� � �� ����� ��� ���� ������ ������������� ���������, ����� 
						// �� ������������ ���� ���� � ����� �� ������ ��� �������.
						if (empty_interpol[i_38] == true) {
							bstandart_vjzov = false;
						}
					}
					if (bstandart_vjzov) {
						this_is_F_node[i8] = false;
						this_is_C_node[i8] = true;
						bweSholdbeContinue = true;
						iadditionalCstatistic++;
					}

					free(empty_interpol);

				}
				else if ((icsos == 1) && (icsosF != 0)) {
					// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
					//
					SumPall += sumP;

					for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							// ��� ���������� ������ Strong �����.
							if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
								if (this_is_C_node[Amat.j[is0]] == true) {

									if (fabs(sumP) < RealZERO) {
										//printf("error interpolation zero diagonal sumP.\n");
										//printf("Fnode all sosed is F");
										//system("pause");
										//	printf("i8 is Dirichlet node\n");
										this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
										this_is_C_node[i8] = true;
										bweSholdbeContinue = true;
										iadditionalCstatistic++;
										//exit(1);
										// ����� ����� �������� �������������.
									}
									else {
										// ��� ��� ��� ������������ Strong C �����. 
										// ��������������� ������� �� ��������� � �����.

										// ������ ������� ������ ����������� ��������� 
										// ������������� ��������� �������� �� �������� 
										// �������� �������.
										// ����������� 5 ������� 2015.
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
											P[icount1].j = i8;
											P[icount1].i = C_numerate[Amat.j[is0]];
											//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
											P[icount1].aij = fabs(Amat.aij[is0]) / SumPall;
											icount1++;
											if (icount1 >= nsizePR*n) {
												printf("memory error!!!\n");
												printf("not enough memory for the interpolation operator.\n");
												//system("PAUSE");
												//exit(1);
												deallocate_prolongation(nsizePR, n,  P);
											}
										}

									}

								}
								else
									if (this_is_F_node[Amat.j[is0]] == true) {

										// ������������� Strong F �����.

										// ����� :
										// 



										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
										// � ������� F ������.
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {

										integer iFpoint = Amat.j[is0];
										doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
										// ��������� ���� ������� iFpointeger 
										// ����� ����� ����� ��� � ����.

										// �������������� ��������� �����.
										doublereal maxelem_threshold_loc = -1.0;
										//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
										integer ii1_loc = row_startA[iFpoint];
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc) {
														maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
													}
												}
											}
										}


										// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

										// ����� ������� ������� F-node ������� C-node.
										integer icsos_loc = 0;

										// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
										// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
										doublereal sumP_loc = 0.0;
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {




												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
													if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
														sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
														icsos_loc++;
													}
												}
												else {
													// ������������ ���������� ������� ������� �� �������� � ������.
													//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
												}
											}
										}

										// � ����� ��� ������� ����������������� ����� 
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													// ��������������� ������� �� ��������� � �����.

													// ������ ������� ������ ����������� ��������� 
													// ������������� ��������� �������� �� �������� 
													// �������� �������.
													// ����������� 5 ������� 2015.
													//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
													if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
														P[icount1].j = i8;
														P[icount1].i = C_numerate[Amat.j[is0_loc]];
														//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
														P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
														icount1++;
														if (icount1 >= nsizePR*n) {
															printf("memory error!!!\n");
															printf("not enough memory for the interpolation operator.\n");
															//system("PAUSE");
															//exit(1);
															deallocate_prolongation(nsizePR, n,  P);
														}
													}
												}
											}
										}


										//}
									}
							}
						}
					}
				}
				else {

					for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							if (this_is_C_node[Amat.j[is0]] == true) {



								// ��������������� ������� �� ��������� � �����.

								// ������ ������� ������ ����������� ��������� 
								// ������������� ��������� �������� �� �������� 
								// �������� �������.
								// ����������� 5 ������� 2015.
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
								if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
									P[icount1].j = i8;
									P[icount1].i = C_numerate[Amat.j[is0]];
									P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
									icount1++;
									if (icount1 >= nsizePR*n) {
										printf("memory error!!!\n");
										printf("not enough memory for the interpolation operator.\n");
										//system("PAUSE");
										//exit(1);
										deallocate_prolongation(nsizePR, n,  P);
									}
								}



							}
						}
					}
				} // standart

			} // i8


		} ///
	}
	else {
		// ������ negative connections:


		// ���������� ����������� ��� ����� ������� ���������� F nodes.
		// ������ F-nodes ������ C-nodes.
		for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {

			// ��� ����� ����������� �������� ��������.
			// 5 ������� 2015 ���� �� ��������� ��������� �������������
			// ��������� ������������� � ������ � ��������� ��������.
			doublereal maxelem_threshold = -1.0;
			//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
			integer ii1 = row_startA[i8];
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
					//if (this_is_C_node[Amat.j[is0]] == true) {
					if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold)) {
						maxelem_threshold = fabs(Amat.aij[is0]);
					}
					//}
				}
			}
			// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

			// ����� ������� ������� F-node ������� C-node.
			integer icsos = 0;
			integer icsosF = 0;

			// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
			// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
			doublereal sumP = 0.0;
			doublereal SumPall = 0.0;
			integer icount_StronglyF = 0;
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					if (this_is_C_node[Amat.j[is0]] == true) {
						//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
						if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
							sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
							icsos++;
						}
					}
					else {
						if (this_is_F_node[Amat.j[is0]] == true) {
							if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
								SumPall += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
								icount_StronglyF++;
								icsosF++;
							}
						}
						// ������������ ���������� ������� ������� �� �������� � ������.
						the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
					}
				}
			}
			if (icsos == 1) {
				number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ ������� � �������.
																	   // ��������� ������ ������ "����������".
																	   // ���������� ������ ����������� ��� ���������.
																	   // � ������� �� �������� ������� ����� ���������� ������ ���������� ���� ��������� ������� � 
																	   // ������������� �� ���� ������� ����� ��������.
				if (icsosF == 0) number_of_F_nodes_with_one_single_strong_C_neighborF++; // ���������� F ����� � ����� ������������ �������  C ������� � � ����-�� �� ������� ������� F �������.
			}


			// 1 ������ 2015 ���� ����� ��� ������������.
			// ������� � ������ ������ ������ ������ ����� ���� � �����.
			if ((false) && (icsos == 1)) {
				this_is_F_node[i8] = false;
				this_is_C_node[i8] = true;
				bweSholdbeContinue = true;
			}
			else {

				if (fabs(sumP) < RealZERO) {
					//printf("error interpolation zero diagonal sumP.\n");
					//printf("Fnode all sosed is F");
					//system("pause");
					//printf("i8 is Dirichlet node\n");

					//exit(1);
					// ����� ����� �������� �������������.
					bool* empty_interpol = NULL;
					//empty_interpol = new bool[icount_StronglyF];
					empty_interpol = (bool*)malloc(icount_StronglyF * sizeof(bool));
					char c1[27] = "classic_aglomerative_amg_4";
					char c2[15] = "empty_interpol";
					handle_error(empty_interpol, c2, c1, icount_StronglyF);

					for (integer i_38 = 0; i_38 < icount_StronglyF; i_38++) {
						empty_interpol[i_38] = false; // initialization
					}

					integer i_38_count = 0;

					for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							if (this_is_F_node[Amat.j[is0]] == true) {



								// ��������������� ������� �� ��������� � �����.

								// ������ ������� ������ ����������� ��������� 
								// ������������� ��������� �������� �� �������� 
								// �������� �������.
								// ����������� 5 ������� 2015.
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
								if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {

									integer iFpoint = Amat.j[is0];
									doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
									// ��������� ���� ������� iFpointeger 
									// ����� ����� ����� ��� � ����.

									// �������������� ��������� �����.
									doublereal maxelem_threshold_loc = -1.0;
									//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
									integer ii1_loc = row_startA[iFpoint];
									for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
											//if (this_is_C_node[Amat.j[is0_loc]] == true) {
											// ����� ������������� ������ ������������� ������� � ���������� ������� ������.
											if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc)) {
												maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
											}
											//}
										}
									}


									// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

									// ����� ������� ������� F-node ������� C-node.
									integer icsos_loc = 0;
									bool bvisit_sumPloc = false;

									// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
									// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
									doublereal sumP_loc = 0.0;
									for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {




											if (this_is_C_node[Amat.j[is0_loc]] == true) {
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
												if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta)) {
													sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
													icsos_loc++;
													bvisit_sumPloc = true;
													empty_interpol[i_38_count] = true;
												}
											}
											else {
												// ������������ ���������� ������� ������� �� �������� � ������.
												//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
											}
										}
									}


									if (bvisit_sumPloc) {
										// � ����� ��� ������� ����������������� ����� 
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													// ��������������� ������� �� ��������� � �����.

													// ������ ������� ������ ����������� ��������� 
													// ������������� ��������� �������� �� �������� 
													// �������� �������.
													// ����������� 5 ������� 2015.
													//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
													if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta)) {
														P[icount1].j = i8;
														P[icount1].i = C_numerate[Amat.j[is0_loc]];
														//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
														P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
														icount1++;
														if (icount1 >= nsizePR*n) {
															printf("memory error!!!\n");
															printf("not enough memory for the interpolation operator.\n");
															//system("PAUSE");
															//exit(1);
															deallocate_prolongation(nsizePR, n,  P);
														}
													}
												}
											}
										}
									}
									else {
										// �� ���� � ������� ������������� ����� ���������� ��������.
										// ��� �������� ��� ����� ���� ��� ������ ��������� F ���� � ������� ������ ��� ������� � �������.
										// ������ �������� ����������� ��� �� ������ ������ �����������.
										//printf("problem situation");
										//getchar();
										this_is_F_node[iFpoint] = false;
										this_is_C_node[iFpoint] = true;
										bweSholdbeContinue = true;
										// �������� ������ ������������ ������� F ������� ������� F ���� ��� �������� ���� ������������,
										// ����� � ����� �� ����� �������� ������� ����� � ����� �� ������ ������� ����������.
										break;
										// ���� ��� ����� ����� �.�. �� ��������� ������������� ���������� �� ������ ������ �����.
									}

									i_38_count++;

								}
							}
						}
					}

					bool bstandart_vjzov = true;
					for (integer i_38 = 0; i_38 < icount_StronglyF; i_38++) {
						// 3 ������. �� ����� ���� ����� ������ �� ���������� �������
						// ����� � �� ����� ��� ���� ������ ������������� ���������, ����� 
						// �� ������������ ���� ���� � ����� �� ������ ��� �������.
						if (empty_interpol[i_38] == true) {
							bstandart_vjzov = false;
						}
					}
					if (bstandart_vjzov) {
						this_is_F_node[i8] = false;
						this_is_C_node[i8] = true;
						bweSholdbeContinue = true;
						iadditionalCstatistic++;
					}

					free(empty_interpol);

				}
				else if ((icsos == 1) && (icsosF != 0)) {
					// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
					//
					SumPall += sumP;

					for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							// ��� ���������� ������ Strong �����.
							if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
								if (this_is_C_node[Amat.j[is0]] == true) {

									if (fabs(sumP) < RealZERO) {
										//printf("error interpolation zero diagonal sumP.\n");
										//printf("Fnode all sosed is F");
										//system("pause");
										//	printf("i8 is Dirichlet node\n");
										this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
										this_is_C_node[i8] = true;
										bweSholdbeContinue = true;
										iadditionalCstatistic++;
										//exit(1);
										// ����� ����� �������� �������������.
									}
									else {
										// ��� ��� ��� ������������ Strong C �����. 
										// ��������������� ������� �� ��������� � �����.

										// ������ ������� ������ ����������� ��������� 
										// ������������� ��������� �������� �� �������� 
										// �������� �������.
										// ����������� 5 ������� 2015.
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
											P[icount1].j = i8;
											P[icount1].i = C_numerate[Amat.j[is0]];
											//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
											P[icount1].aij = fabs(Amat.aij[is0]) / SumPall;
											icount1++;
											if (icount1 >= nsizePR*n) {
												printf("memory error!!!\n");
												printf("not enough memory for the interpolation operator.\n");
												//system("PAUSE");
												//exit(1);
												deallocate_prolongation(nsizePR, n,  P);
											}
										}

									}

								}
								else
									if (this_is_F_node[Amat.j[is0]] == true) {

										// ������������� Strong F �����.

										// ����� :
										// 



										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
										// � ������� F ������.
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {

										integer iFpoint = Amat.j[is0];
										doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
										// ��������� ���� ������� iFpointeger 
										// ����� ����� ����� ��� � ����.

										// �������������� ��������� �����.
										doublereal maxelem_threshold_loc = -1.0;
										//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
										integer ii1_loc = row_startA[iFpoint];
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc)) {
														maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
													}
												}
											}
										}


										// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

										// ����� ������� ������� F-node ������� C-node.
										integer icsos_loc = 0;

										// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
										// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
										doublereal sumP_loc = 0.0;
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {




												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
													if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta)) {
														sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
														icsos_loc++;
													}
												}
												else {
													// ������������ ���������� ������� ������� �� �������� � ������.
													//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
												}
											}
										}

										// � ����� ��� ������� ����������������� ����� 
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													// ��������������� ������� �� ��������� � �����.

													// ������ ������� ������ ����������� ��������� 
													// ������������� ��������� �������� �� �������� 
													// �������� �������.
													// ����������� 5 ������� 2015.
													//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
													if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta)) {
														P[icount1].j = i8;
														P[icount1].i = C_numerate[Amat.j[is0_loc]];
														//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
														P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
														icount1++;
														if (icount1 >= nsizePR*n) {
															printf("memory error!!!\n");
															printf("not enough memory for the interpolation operator.\n");
															//system("PAUSE");
															//exit(1);
															deallocate_prolongation(nsizePR, n,  P);
														}
													}
												}
											}
										}


										//}
									}
							}
						}
					}
				}
				else {

					for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							if (this_is_C_node[Amat.j[is0]] == true) {



								// ��������������� ������� �� ��������� � �����.

								// ������ ������� ������ ����������� ��������� 
								// ������������� ��������� �������� �� �������� 
								// �������� �������.
								// ����������� 5 ������� 2015.
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
								if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
									P[icount1].j = i8;
									P[icount1].i = C_numerate[Amat.j[is0]];
									P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
									icount1++;
									if (icount1 >= nsizePR*n) {
										printf("memory error!!!\n");
										printf("not enough memory for the interpolation operator.\n");
										//system("PAUSE");
										//exit(1);
										deallocate_prolongation(nsizePR, n,  P);
									}
								}



							}
						}
					}
				} // standart

			} // i8


		}

	}

} // my_interpolation_procedure_number2

  // ���������������� ��������� �3.
void my_interpolation_procedure_number3(integer &the_number_of_neighbors_that_are_not_�_nodes,
	integer &number_of_F_nodes_with_one_single_strong_C_neighbor,
	integer* &n_a, bool* &this_is_F_node, integer* &row_startA,
	integer* &nnz_a, bool &bpositive_connections, Ak2 &Amat,
	bool &bweSholdbeContinue, bool* &this_is_C_node, integer &iadditionalCstatistic,
	const doublereal RealZERO, integer &icount1, Ak1* &P, integer &nsizePR, integer &ilevel,
	integer &iadd, doublereal &theta, integer &n,  integer* &C_numerate,
	integer &number_of_F_nodes_with_one_single_strong_C_neighborF,
	doublereal &theta83, bool &btreshold_on_new_vetv, integer& ifrom_re_operation_protection,
	bool &from_re_operation_protection0, doublereal &magic82, doublereal* &threshold_quick_all,
	doublereal* &threshold_quick_only_negative)
{

	// 6 december 2016.
	// �������� �������� Refactoring.
	integer itypemodifyinterpol = 0; // ����� ����������� �������������. // �������� �������� Refactoring.
	integer inumberadaptpass = 0; // ������������ ���������� ������-�������� � �������������. // �������� �������� Refactoring.


	//theta = 0.24;
	// theta_strong_F iter_number time,s
	// 0.21 56 22.63
	// 0.22 55 21.769
	// 0.23 52 21.488
	// 0.24 52 21.741 == theta // optimum
	// 0.26 69 24.623
	doublereal theta_strong_F = 0.23; // ����������� �����.
	theta_strong_F = theta83; // 3 ���� 2016


							  // �������� ������ ������������.
	integer ioneStrongC_and_0_StrongF = 0;

	// ����� ���� F �� ������� Strong � ������� ��� ���������� � �����.
	// ���� F ������� ������ Strong  � ������ �������������� � ������� ������� � ������� 
	// ������� F �����.

	//6interpolation 0.4 6.77 11 26 28.355
	//6interpolation 0.45 6.6 10 27 28.151
	//6interpolation 0.5 6.42 12 32 28.735
	//4interpolation 0.4 3.7  52 24.736 // best
	//4interpolation 0.3 3.78 13 59 27.525
	//4interpolation 0.5 3.61 12 55 25.533
	//4interpolation 0.45 3.65 10 63 30.24

	// the begining

	bool byes_add = false;
	// ������� ���������� ����������� � �����.
	if (1) {
		// � ���������� 0.4 ��������� ������������ �������� ����� ������� ������.
		//doublerealT magic = 0.4; // 0.4 optimum


		the_number_of_neighbors_that_are_not_�_nodes = 0;
		number_of_F_nodes_with_one_single_strong_C_neighbor = 0;

		if (bpositive_connections) {


			integer i8_candidate_count = 0;
			integer* i8_candidate_arr = NULL;
			i8_candidate_arr = (integer*)malloc((n_a[ilevel - 1] + 1) * sizeof(integer));
			char c1[18] = "i8_candidate_arr";
			char c2[26] = "classic_aglomerative_amg4";
			handle_error(i8_candidate_arr, c1, c2, (n_a[ilevel - 1] + 1));
			doublereal* i8_candidate_val_arr = NULL;
			i8_candidate_val_arr = (doublereal*)malloc((n_a[ilevel - 1] + 1) * sizeof(doublereal));
			char c3[22] = "i8_candidate_val_arr";
			handle_error(i8_candidate_val_arr, c3, c2, (n_a[ilevel - 1] + 1));

			// ���������� ����������� ��� ����� ������� ���������� F nodes.
			// ������ F-nodes ������ C-nodes.
			for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {

#if doubleintprecision == 1
				//printf("i8=%lld n=%lld\n", i8, n_a[ilevel - 1]);
#else
				//printf("i8=%d n=%d\n", i8, n_a[ilevel - 1]);
#endif

				//getchar();


				// ��� ����� ����������� �������� ��������.
				// 5 ������� 2015 ���� �� ��������� ��������� �������������
				// ��������� ������������� � ������ � ��������� ��������.
				doublereal maxelem_threshold = -1.0;
				//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
				integer ii1 = row_startA[i8];
				//for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				integer iend_marker_position = row_startA[Amat.i[ii1] + 1] - 1;
				if (!btreshold_on_new_vetv) {
					for (integer is0 = ii1; (is0 <= iend_marker_position); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
							//if (this_is_C_node[Amat.j[is0]] == true) {
							if (fabs(Amat.aij[is0]) > maxelem_threshold) {
								maxelem_threshold = fabs(Amat.aij[is0]);
							}
							//}
						}
					}
				}
				else {
					maxelem_threshold = threshold_quick_all[Amat.i[ii1]];
				}
				// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

				// ����� ������� ������� F-node ������� C-node.
				integer icsos = 0;
				integer icsosF = 0;

				// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
				// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
				doublereal sumP = 0.0;
				doublereal SumPall = 0.0;
				integer icount_StronglyF = 0;
				doublereal maxelem_threshold_theta = maxelem_threshold*theta;
				doublereal maxelem_threshold_theta_strong_F = maxelem_threshold*theta_strong_F;
				for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
					if (Amat.j[is0] != Amat.i[ii1]) {
						if (this_is_C_node[Amat.j[is0]] == true) {
							//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
							//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
							if (fabs(Amat.aij[is0]) > maxelem_threshold_theta) {
								sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
								icsos++;
							}
						}
						else {
							if (this_is_F_node[Amat.j[is0]] == true) {
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta_strong_F) {
								if (fabs(Amat.aij[is0]) > maxelem_threshold_theta_strong_F) {
									SumPall += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
									icount_StronglyF++;
									icsosF++;
								}
							}
							// ������������ ���������� ������� ������� �� �������� � ������.
							the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
						}
					}
				}
				if (icsos == 1) {
					number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ �������  � �������.
																		   // ��������� ������ ������ "����������".
																		   // ���������� ������ ����������� ��� ���������.
																		   // � ������� �� �������� ������� ����� ���������� ������ ���������� ���� ��������� ������� � 
																		   // ������������� �� ���� ������� ����� ��������.
					if (icsosF == 0) number_of_F_nodes_with_one_single_strong_C_neighborF++; // ���������� F ����� � ����� ������������ �������  C ������� � � ����-�� �� ������� ������� F �������.
				}



				// 1 ������ 2016 ���� ����� ��� ������������.
				// ������� � ������ ������ ������ ������ ����� ���� � �����.
				{

					if ((icsos == 1) && (icsosF == 0)) {
						// ����������� ����������.
						if (itypemodifyinterpol > 0) {

							if (itypemodifyinterpol > 1) {
								// ���������� ������� ��������� �� � ����.
								i8_candidate_arr[i8_candidate_count] = i8;
								// ���������� ���� ��������������� �����, 
								// ����� ����� ������� �������� �������.
								i8_candidate_val_arr[i8_candidate_count] = sumP;
								i8_candidate_count++;
							}

							if (itypemodifyinterpol == 1) {
								// ��������� ������ � ��� ������ ���� ���������� �������� ������ ������������.
								if (ifrom_re_operation_protection < inumberadaptpass) {
									// 3 december 2016 rundom
									// �� ����� ����� ��������� ���������� �� ������ ������.
									integer id_size_873 = (integer)(n_a[ilevel - 1] / 100);
									integer id_873 = rand() % id_size_873 + 1;
									if (id_873 == 1) {
										// � ������ ���� ������ �������� � ������ � ���������� ������� F �������
										// ����� ���� ���������� � �����. �.�. � ����������� ����� ���� ���������� � �����.
										// ����������� 2 ������� 2016 ����.
										this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
										this_is_C_node[i8] = true;
										bweSholdbeContinue = true;
										iadditionalCstatistic++;
										byes_add = true; // ���� ���������� �����.
									}
								}
							}
						}
					}
					else if ((icsos == 1) && (icsosF != 0)) {
						// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
						//
						//SumPall += sumP;

						for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
							if (Amat.j[is0] != Amat.i[ii1]) {
								// ��� ���������� ������ Strong �����.
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
								if (fabs(Amat.aij[is0]) > maxelem_threshold_theta) {
									if (this_is_C_node[Amat.j[is0]] == true) {

										// 20 ������ 2016.
										// ��-����� ���� �������� ������ ���� �� ����� ��� ��� � ��� 
										// �������� ���� � �����.
										if (fabs(sumP) < RealZERO) {
											//printf("error interpolation zero diagonal sumP.\n");
											//printf("Fnode all sosed is F");
											//system("pause");
											//	printf("i8 is Dirichlet node\n");
											this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
											this_is_C_node[i8] = true;
											bweSholdbeContinue = true;
											iadditionalCstatistic++;
											byes_add = true; // ���� ���������� �����.
															 //exit(1);
															 // ����� ����� �������� �������������.
										}


									}

								}
							}
						}
					}
					else {

						integer iend_marker_position = row_startA[Amat.i[ii1] + 1] - 1;
						for (integer is0 = ii1; (is0 <= iend_marker_position); is0++) {
							if (this_is_C_node[Amat.j[is0]] == true) {
								if (Amat.j[is0] != Amat.i[ii1]) {


									// 20 jan 2016.
									// ����� �� ��������� ���� � ������ ���� ���� �� ����� ��� ������� F �������.


									if (fabs(sumP) < RealZERO) {
										// ��� ������ ����� ������ ��� ������� � �������.


										//printf("error interpolation zero diagonal sumP.\n");
										//printf("Fnode all sosed is F");
										//system("pause");
										//printf("i8 is Dirichlet node\n");
										this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
										this_is_C_node[i8] = true;
										bweSholdbeContinue = true;
										iadditionalCstatistic++;
										byes_add = true; // ���� ���������� �����.
														 //exit(1);
														 // ����� ����� �������� �������������.
									}


								}
							}
						}
					}

				}

			}
			/////
			if (itypemodifyinterpol > 0) {

				if (i8_candidate_arr != NULL) {
					i8_candidate_arr = (integer*)realloc(i8_candidate_arr, (i8_candidate_count + 2) * sizeof(integer));
				}
				if (i8_candidate_val_arr != NULL) {
					i8_candidate_val_arr = (doublereal*)realloc(i8_candidate_val_arr, (i8_candidate_count + 2) * sizeof(doublereal));
				}


				// �������� integer i_gold_i8 �������� �������.
				integer i_gold_i8 = i_my_min(i8_candidate_count/20, 100);// 100
				if (i8_candidate_count < 100) {
					i_gold_i8 = i8_candidate_count - 3;
				}
				if (1 && from_re_operation_protection0) {
					// ���������� �� �����������.
					// ���������� ������� �. ����� ������������ �� ������ my_nested_dissection.cpp
					QuickSortnd(i8_candidate_arr, i8_candidate_val_arr, 0, i8_candidate_count - 1);

					if (itypemodifyinterpol == 2) {
						// minimum
						//for (integer i_9 = i8_candidate_count - 1; i_9 >= i8_candidate_count - 1 - i_gold_i8; i_9--) {
						for (integer i_9 = 0; i_9 < i_gold_i8; i_9++) {
							// � ������ ���� ������ �������� � ������ � ���������� ������� F �������
							// ����� ���� ���������� � �����. �.�. � ����������� ����� ���� ���������� � �����.
							// ����������� 2 ������� 2016 ����.
							this_is_F_node[i8_candidate_arr[i_9]] = false; // ���� ���� ������� ������ � �����.
							this_is_C_node[i8_candidate_arr[i_9]] = true;
							bweSholdbeContinue = true;
							iadditionalCstatistic++;
							byes_add = true; // ���� ���������� �����.
						}
					}
					if (itypemodifyinterpol == 3) {
						// maximum
						for (integer i_9 = i8_candidate_count - 1; i_9 >= i8_candidate_count - 1 - i_gold_i8; i_9--) {
							//for (integer i_9 = 0; i_9 < i_gold_i8; i_9++) {
							// � ������ ���� ������ �������� � ������ � ���������� ������� F �������
							// ����� ���� ���������� � �����. �.�. � ����������� ����� ���� ���������� � �����.
							// ����������� 2 ������� 2016 ����.
							this_is_F_node[i8_candidate_arr[i_9]] = false; // ���� ���� ������� ������ � �����.
							this_is_C_node[i8_candidate_arr[i_9]] = true;
							bweSholdbeContinue = true;
							iadditionalCstatistic++;
							byes_add = true; // ���� ���������� �����.
						}
					}

				}

				ifrom_re_operation_protection++;
				if (ifrom_re_operation_protection > inumberadaptpass) {
					from_re_operation_protection0 = false;
				}


			}

			if (i8_candidate_arr != NULL) {
				free(i8_candidate_arr);
				i8_candidate_arr = NULL;
			}
			if (i8_candidate_val_arr != NULL) {
				free(i8_candidate_val_arr);
				i8_candidate_val_arr = NULL;
			}

		}
		else {
			// only negative connections

			// ���������� ����������� ��� ����� ������� ���������� F nodes.
			// ������ F-nodes ������ C-nodes.
			for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {



				// ��� ����� ����������� �������� ��������.
				// 5 ������� 2015 ���� �� ��������� ��������� �������������
				// ��������� ������������� � ������ � ��������� ��������.
				doublereal maxelem_threshold = -1.0;
				//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
				integer ii1 = row_startA[i8];
				//for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				integer iend_marker_position = row_startA[Amat.i[ii1] + 1] - 1;
				if (!btreshold_on_new_vetv) {
					for (integer is0 = ii1; (is0 <= iend_marker_position); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
							//if (this_is_C_node[Amat.j[is0]] == true) {
							if ((Amat.aij[is0] < 0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold)) {
								maxelem_threshold = fabs(Amat.aij[is0]);
							}
							//}
						}
					}
				}
				else {
					maxelem_threshold = threshold_quick_only_negative[Amat.i[ii1]];
				}
				// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

				// ����� ������� ������� F-node ������� C-node.
				integer icsos = 0;
				integer icsosF = 0;

				// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
				// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
				doublereal sumP = 0.0;
				doublereal SumPall = 0.0;
				integer icount_StronglyF = 0;
				doublereal maxelem_threshold_theta = maxelem_threshold*theta;
				doublereal maxelem_threshold_theta_strong_F = maxelem_threshold*theta_strong_F;
				for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
					if (Amat.j[is0] != Amat.i[ii1]) {
						if (this_is_C_node[Amat.j[is0]] == true) {
							//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
							//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
							if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta)) {
								sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
								icsos++;
							}
						}
						else {
							if (this_is_F_node[Amat.j[is0]] == true) {
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta_strong_F) {
								if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_strong_F)) {
									SumPall += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
									icount_StronglyF++;
									icsosF++;
								}
							}
							// ������������ ���������� ������� ������� �� �������� � ������.
							the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
						}
					}
				}
				if (icsos == 1) {
					number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ �������  � �������.
																		   // ��������� ������ ������ "����������".
																		   // ���������� ������ ����������� ��� ���������.
																		   // � ������� �� �������� ������� ����� ���������� ������ ���������� ���� ��������� ������� � 
																		   // ������������� �� ���� ������� ����� ��������.
					if (icsosF == 0) number_of_F_nodes_with_one_single_strong_C_neighborF++; // ���������� F ����� � ����� ������������ �������  C ������� � � ����-�� �� ������� ������� F �������.
				}



				// 1 ������ 2016 ���� ����� ��� ������������.
				// ������� � ������ ������ ������ ������ ����� ���� � �����.
				{

					if ((icsos == 1) && (icsosF != 0)) {
						// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
						//
						//SumPall += sumP;

						for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
							if (Amat.j[is0] != Amat.i[ii1]) {
								// ��� ���������� ������ Strong �����.
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
								if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta)) {
									if (this_is_C_node[Amat.j[is0]] == true) {

										// 20 ������ 2016.
										// ��-����� ���� �������� ������ ���� �� ����� ��� ��� � ��� 
										// �������� ���� � �����.
										if (fabs(sumP) < RealZERO) {
											//printf("error interpolation zero diagonal sumP.\n");
											//printf("Fnode all sosed is F");
											//system("pause");
											//	printf("i8 is Dirichlet node\n");
											this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
											this_is_C_node[i8] = true;
											bweSholdbeContinue = true;
											iadditionalCstatistic++;
											byes_add = true; // ���� ���������� �����.
															 //exit(1);
															 // ����� ����� �������� �������������.
										}


									}

								}
							}
						}
					}
					else {

						integer iend_marker_position = row_startA[Amat.i[ii1] + 1] - 1;
						for (integer is0 = ii1; (is0 <= iend_marker_position); is0++) {
							if (this_is_C_node[Amat.j[is0]] == true) {
								if (Amat.j[is0] != Amat.i[ii1]) {


									// 20 jan 2016.
									// ����� �� ��������� ���� � ������ ���� ���� �� ����� ��� ������� F �������.


									if (fabs(sumP) < RealZERO) {
										// ��� ������ ����� ������ ��� ������� � �������.


										//printf("error interpolation zero diagonal sumP.\n");
										//printf("Fnode all sosed is F");
										//system("pause");
										//printf("i8 is Dirichlet node\n");
										this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
										this_is_C_node[i8] = true;
										bweSholdbeContinue = true;
										iadditionalCstatistic++;
										byes_add = true; // ���� ���������� �����.
														 //exit(1);
														 // ����� ����� �������� �������������.
									}


								}
							}
						}
					}

				}

			}


		}

	}



	if (!byes_add) {

		// � ���������� 0.4 ��������� ������������ �������� ����� ������� ������.
		doublereal magic = 0.4; // 0.4 optimum
								//magic = 0.3; // 3 ���� 2016 ��� ������������ �����
								// �������� ������� �� ���� ���������
								// �� �� ������������� �� �� ����� V ������.
								//magic = 0.5 - 0.2*ilevel / 12.0;
		magic = magic82;



		the_number_of_neighbors_that_are_not_�_nodes = 0;
		number_of_F_nodes_with_one_single_strong_C_neighbor = 0;

		if (bpositive_connections) {

			// ���������� ����������� ��� ����� ������� ���������� F nodes.
			// ������ F-nodes ������ C-nodes.
			for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {

				// ��� ����� ����������� �������� ��������.
				// 5 ������� 2015 ���� �� ��������� ��������� �������������
				// ��������� ������������� � ������ � ��������� ��������.
				doublereal maxelem_threshold = -1.0;
				//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
				integer ii1 = row_startA[i8];
				integer istr_etalon1 = Amat.i[ii1];
				integer iend_for1 = -1;
				if (!btreshold_on_new_vetv) {
					for (integer is0 = ii1; (is0 <= row_startA[istr_etalon1 + 1] - 1); is0++) {
						iend_for1 = is0;
						if (Amat.j[is0] != istr_etalon1) {
							// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
							//if (this_is_C_node[Amat.j[is0]] == true) {
							if (fabs(Amat.aij[is0]) > maxelem_threshold) {
								maxelem_threshold = fabs(Amat.aij[is0]);
							}
							//}
						}
					}
				}
				else {
					for (integer is0 = ii1; (is0 <= row_startA[istr_etalon1 + 1] - 1); is0++) {
						iend_for1 = is0;
					}
					maxelem_threshold = threshold_quick_all[istr_etalon1];
				}
				// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

				// ����� ������� ������� F-node ������� C-node.
				integer icsos = 0;
				integer icsosF = 0;

				doublereal maxelem_threshold_theta = maxelem_threshold*theta;
				doublereal maxelem_threshold_theta_strong_F = maxelem_threshold*theta_strong_F;


				// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
				// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
				doublereal sumP = 0.0;
				doublereal SumPall = 0.0;
				integer icount_StronglyF = 0;
				//	for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				for (integer is0 = ii1; is0 <= iend_for1; is0++) {
					if (Amat.j[is0] != istr_etalon1) {
						if (this_is_C_node[Amat.j[is0]] == true) {
							//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
							//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
							if (fabs(Amat.aij[is0]) > maxelem_threshold_theta) {
								sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
								icsos++;
							}
						}
						else {
							if (this_is_F_node[Amat.j[is0]] == true) {
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta_strong_F) {
								if (fabs(Amat.aij[is0]) > maxelem_threshold_theta_strong_F) {
									SumPall += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
									icount_StronglyF++;
									icsosF++;
								}
							}
							// ������������ ���������� ������� ������� �� �������� � ������.
							the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
						}
					}
				}
				if (icsos == 1) {
					number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ �������  � �������.
																		   // ��������� ������ ������ "����������".
																		   // ���������� ������ ����������� ��� ���������.
																		   // � ������� �� �������� ������� ����� ���������� ������ ���������� ���� ��������� ������� � 
																		   // ������������� �� ���� ������� ����� ��������.
					if (icsosF == 0) number_of_F_nodes_with_one_single_strong_C_neighborF++; // ���������� F ����� � ����� ������������ �������  C ������� � � ����-�� �� ������� ������� F �������.
				}



				// 1 ������ 2016 ���� ����� ��� ������������.
				// ������� � ������ ������ ������ ������ ����� ���� � �����.
				if ((false) && (icsos == 1)) {
					this_is_F_node[i8] = false;
					this_is_C_node[i8] = true;
					bweSholdbeContinue = true;
				}
				else {

					if (((icsos == 1) || (icsos == 2) || (icsos == 3)) && (icsosF != 0)) {
						//if (((icsos == 1) || (icsos == 2) || (icsos == 3) || (icsos >= 4)) && (icsosF != 0)) {
						// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
						//
						SumPall += sumP;

						for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
							if (Amat.j[is0] != Amat.i[ii1]) {
								// ��� ���������� ������ Strong �����.


								if (this_is_C_node[Amat.j[is0]] == true) {
									//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
									if (fabs(Amat.aij[is0]) > maxelem_threshold_theta) {
										if (fabs(sumP) < RealZERO) {
											//printf("error interpolation zero diagonal sumP.\n");
											//printf("Fnode all sosed is F");
											//system("pause");
											//	printf("i8 is Dirichlet node\n");
											this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
											this_is_C_node[i8] = true;
											bweSholdbeContinue = true;
											iadditionalCstatistic++;
											//exit(1);
											// ����� ����� �������� �������������.
										}
										else {
											// ��� ��� ��� ������������ Strong C �����. 
											// ��������������� ������� �� ��������� � �����.

											// ������ ������� ������ ����������� ��������� 
											// ������������� ��������� �������� �� �������� 
											// �������� �������.
											// ����������� 5 ������� 2015.
											//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
											//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
											if (fabs(Amat.aij[is0]) > maxelem_threshold_theta) {
												P[icount1].j = i8;
												P[icount1].i = C_numerate[Amat.j[is0]];
												//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
												if (fabs(SumPall) < 1.0e-33) {
													printf("error 1.0 ! division by zero. SumPall =%e\n", SumPall);
													//getchar();
													system("PAUSE");
													exit(1);
												}
												P[icount1].aij = fabs(Amat.aij[is0]) / SumPall;
												icount1++;
												if (icount1 >= nsizePR*n) {
													printf("memory error!!!\n");
													printf("not enough memory for the interpolation operator.\n");
													//system("PAUSE");
													//exit(1);
													deallocate_prolongation(nsizePR, n,  P);
												}
											}

										}
									}

								}
								else
									if (this_is_F_node[Amat.j[is0]] == true) {
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta_strong_F) {
										if (fabs(Amat.aij[is0]) > maxelem_threshold_theta_strong_F) {
											// ������������� Strong F �����.

											// ����� :
											// 



											//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
											// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
											// � ������� F ������.
											//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta_strong_F) {

											integer iFpoint = Amat.j[is0];
											if (fabs(SumPall) < 1.0e-33) {
												printf("error 2.0 ! division by zero. SumPall =%e\n", SumPall);
												//getchar();
												system("PAUSE");
												exit(1);
											}
											doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
											// ��������� ���� ������� iFpointeger 
											// ����� ����� ����� ��� � ����.

											// �������������� ��������� �����.
											doublereal maxelem_threshold_loc = -1.0;
											//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
											integer ii1_loc = row_startA[iFpoint];
											integer istr_etalon = Amat.i[ii1_loc];
											integer iend_for = -1;
											integer iend_marker_position = row_startA[istr_etalon + 1] - 1;
											for (integer is0_loc = ii1_loc; (is0_loc <= iend_marker_position); is0_loc++) {
												iend_for = is0_loc;
												if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc) {
													if (this_is_C_node[Amat.j[is0_loc]] == true) {
														if (Amat.j[is0_loc] != istr_etalon) {
															maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
														}
													}
												}
											}

											doublereal maxelem_threshold_loc_magic = maxelem_threshold_loc*magic;
											// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

											// ����� ������� ������� F-node ������� C-node.
											integer icsos_loc = 0;

											// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
											// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
											doublereal sumP_loc = 0.0;
											//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

												// ����� �������� ����� ���������� ����� ���������.
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic) {
												if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc_magic) {






													if (this_is_C_node[Amat.j[is0_loc]] == true) {

														if (Amat.j[is0_loc] != istr_etalon) {

															sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
															icsos_loc++;
														}

													}
													else {

														//if (Amat.j[is0_loc] != istr_etalon) {
														// ������������ ���������� ������� ������� �� �������� � ������.
														//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
														//}
													}

												}
											}

											doublereal maxelem_threshold_loc_magic_minus = -maxelem_threshold_loc_magic;

											// � ����� ��� ������� ����������������� ����� 
											//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

												// ��� �������������� ��������� � ������ ������� ���� �������� ���������� ����� ���������.


												// ��������������� ������� �� ��������� � �����.

												// ������ ������� ������ ����������� ��������� 
												// ������������� ��������� �������� �� �������� 
												// �������� �������.
												// ����������� 5 ������� 2015.
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic) {
												if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc_magic) {
													//if ((Amat.aij[is0_loc]<maxelem_threshold_loc_magic_minus) || (Amat.aij[is0_loc]>maxelem_threshold_loc_magic)) {

													if (this_is_C_node[Amat.j[is0_loc]] == true) {
														if (Amat.j[is0_loc] != istr_etalon) {

															P[icount1].j = i8;
															P[icount1].i = C_numerate[Amat.j[is0_loc]];
															//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
															if (fabs(sumP_loc) < 1.0e-33) {
																printf("error 3.0 ! division by zero. sumP_loc =%e\n", sumP_loc);
																//getchar();
																system("PAUSE");
																exit(1);
															}
															P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
															icount1++;
															if (icount1 >= nsizePR*n) {
																printf("memory error!!!\n");
																printf("not enough memory for the interpolation operator.\n");
																//system("PAUSE");
																//exit(1);
																deallocate_prolongation(nsizePR, n,  P);
															}

														}
													}
												}
											}


											//}
										}
									}
							}

						}
					}
					else {

						// �������� bmodify_interpol4_amg==true ��� magic2<0.24 �������� � ����������������� ���������
						// �������� ����������. ��� ���������� ����������� �� bmodify_interpol4_amg==true � magic2>=0.24 
						// �������� ������� �� �������� ����� �� ����������� ���������� ����� �� ������� ��� � ��� 
						// bmodify_interpol4_amg = false;
						// 22 01 2016 ����� �������� bmodify_interpol4_amg = false; � �� � ���� ������ �� ������.
						bool bmodify_interpol4_amg = false;
						doublereal magic2 = 0.5;
						//magic2 = magic83;

						// ���������������� ���� � �������� ��������� ������������ 
						// �������� ������������ ���������� ��� ���������� ����� � �����.
						// �� ��������� ������������ �� ���� �������� ������� Weakly C ����� ���� �������
						// ������� �������.
						// ���������������� ���� ����������� � ����������� ��������� is0_candidate ����� �������
						// Weakly C ����� � ������ � ���������� ������������ ������� ����������� � sum1.
						doublereal sum1 = 0.0;
						integer is0_candidate = -1;
						if (bmodify_interpol4_amg && ((icsos == 1) && (icsosF == 0))) {
							for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
								if (Amat.j[is0] != Amat.i[ii1]) {
									if (this_is_C_node[Amat.j[is0]] == true) {
										//if (fabs(Amat.aij[is0]) <= maxelem_threshold*theta) {
										if (fabs(Amat.aij[is0]) <= maxelem_threshold_theta) {
											if (fabs(Amat.aij[is0]) > sum1) {
												sum1 = fabs(Amat.aij[is0]);
												is0_candidate = is0;
											}
										}
									}
								}
							}

							if (sum1 / sumP < magic2) {
								// ���� ��������� ������� ������� ������ ������ �� ������ � ���� �,
								// � ���� ������ �������� Weakly F ��������� ����� ������� ��� C ����������.
								sum1 = 0.0;
								is0_candidate = -1;

								// ����� ���� ����� Weakly F ����� ���������.
								for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
									if (Amat.j[is0] != Amat.i[ii1]) {
										if (this_is_F_node[Amat.j[is0]] == true) {
											//if (fabs(Amat.aij[is0]) <= maxelem_threshold*theta) {
											if (fabs(Amat.aij[is0]) <= maxelem_threshold_theta) {
												if (fabs(Amat.aij[is0]) > sum1) {
													sum1 = fabs(Amat.aij[is0]);
													is0_candidate = is0;
												}
											}
										}
									}
								}

								// ����� ���� F ������ ����� ����� � �����.
								if (sum1 / sumP >= magic2) {
									this_is_F_node[i8] = false; // ���� ����  ������ � �����.
									this_is_C_node[i8] = true;
									bweSholdbeContinue = true;
									byes_add = true;
									iadditionalCstatistic++;

								}
								// ������������ ����� ����� ��������� ����� �� ���� ������� �� ����.
								sum1 = 0.0;
								is0_candidate = -1;

							}
						}


						for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
							if (Amat.j[is0] != Amat.i[ii1]) {
								if (this_is_C_node[Amat.j[is0]] == true) {



									if (bmodify_interpol4_amg && ((icsos == 1) && (icsosF == 0))) {
										// �� ��������� ���� ������������ ��� ���������� ����� ����� ��
										// ���� ����� �������������� ����� ������� Weakly C �����.
										// ����� ������� Weakly C ����� ����� ��������� sum1 � is0_candidate.

										ioneStrongC_and_0_StrongF++;
										if (is0_candidate != -1) {
											P[icount1].j = i8;
											P[icount1].i = C_numerate[Amat.j[is0_candidate]];
											P[icount1].aij = fabs(Amat.aij[is0_candidate]) / (sumP + sum1);
											icount1++;
											if (icount1 >= nsizePR*n) {
												printf("memory error!!!\n");
												printf("not enough memory for the interpolation operator.\n");
												//system("PAUSE");
												//exit(1);
												deallocate_prolongation(nsizePR, n,  P);
											}
										}

										//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
										if (fabs(Amat.aij[is0]) > maxelem_threshold_theta) {
											P[icount1].j = i8;
											P[icount1].i = C_numerate[Amat.j[is0]];
											P[icount1].aij = fabs(Amat.aij[is0]) / (sumP + sum1);
											icount1++;
											if (icount1 >= nsizePR*n) {
												printf("memory error!!!\n");
												printf("not enough memory for the interpolation operator.\n");
												//system("PAUSE");
												//exit(1);
												deallocate_prolongation(nsizePR, n, P);
											}
										}
									}
									else {

										if (fabs(sumP) < RealZERO) {
											//printf("error interpolation zero diagonal sumP.\n");
											//printf("Fnode all sosed is F");
											//system("pause");
											//printf("i8 is Dirichlet node\n");
											this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
											this_is_C_node[i8] = true;
											bweSholdbeContinue = true;
											iadditionalCstatistic++;
											//exit(1);
											// ����� ����� �������� �������������.
										}
										else {

											// ������������� ������ �� ��������� � �����.

											// ��������������� ������� �� ��������� � �����.

											// ������ ������� ������ ����������� ��������� 
											// ������������� ��������� �������� �� �������� 
											// �������� �������.
											// ����������� 5 ������� 2015.
											//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
											//	if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
											if (fabs(Amat.aij[is0]) > maxelem_threshold_theta) {
												P[icount1].j = i8;
												P[icount1].i = C_numerate[Amat.j[is0]];
												if (fabs(sumP) < 1.0e-33) {
													// ��������� ������� � �����, ��� �������� �������� ������� ��� �������������.
													// (fabs(Amat.aij[is0]) / sumP) �� ����� ���� ������ ������� �� �����������. 
													if ((fabs(sumP) < 1.0e-100) || ((fabs(sumP) >= 1.0e-100) && ((fabs(Amat.aij[is0]) / sumP) < 0.1))) {
														printf("error 4.0 ! division by zero. sumP =%e\n", sumP);
														// �������� : fabs(Amat.aij[is0]) / sumP==1.0, Amat.aij[is0]<1.0e-33;
														printf("diagnostic: aij=%e aij/sumP=%e\n", Amat.aij[is0], fabs(Amat.aij[is0]) / sumP);
														//getchar();
														system("PAUSE");
														exit(1);
													}
												}
												P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
												icount1++;
												if (icount1 >= nsizePR*n) {
													printf("memory error!!!\n");
													printf("not enough memory for the interpolation operator.\n");
													//system("PAUSE");
													//exit(1);
													deallocate_prolongation(nsizePR, n,  P);
												}
											}

										}

									}

								}
							}
						}
					}

				}
			}
			////
		}
		else {
			// only negative connections.

			// ���������� ����������� ��� ����� ������� ���������� F nodes.
			// ������ F-nodes ������ C-nodes.
			for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {

				// ��� ����� ����������� �������� ��������.
				// 5 ������� 2015 ���� �� ��������� ��������� �������������
				// ��������� ������������� � ������ � ��������� ��������.
				doublereal maxelem_threshold = -1.0;
				//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
				integer ii1 = row_startA[i8];
				integer istr_etalon1 = Amat.i[ii1];
				integer iend_for1 = -1;
				if (!btreshold_on_new_vetv) {
					for (integer is0 = ii1; (is0 <= row_startA[istr_etalon1 + 1] - 1); is0++) {
						iend_for1 = is0;
						if (Amat.j[is0] != istr_etalon1) {
							// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
							//if (this_is_C_node[Amat.j[is0]] == true) {
							if ((Amat.aij[is0] < 0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold)) {
								maxelem_threshold = fabs(Amat.aij[is0]);
							}
							//}
						}
					}
				}
				else {
					for (integer is0 = ii1; (is0 <= row_startA[istr_etalon1 + 1] - 1); is0++) {
						iend_for1 = is0;
					}
					maxelem_threshold = threshold_quick_only_negative[istr_etalon1];
				}
				// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

				// ����� ������� ������� F-node ������� C-node.
				integer icsos = 0;
				integer icsosF = 0;

				doublereal maxelem_threshold_theta = maxelem_threshold*theta;
				doublereal maxelem_threshold_theta_strong_F = maxelem_threshold*theta_strong_F;


				// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
				// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
				doublereal sumP = 0.0;
				doublereal SumPall = 0.0;
				integer icount_StronglyF = 0;
				//	for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				for (integer is0 = ii1; is0 <= iend_for1; is0++) {
					if (Amat.j[is0] != istr_etalon1) {
						if (this_is_C_node[Amat.j[is0]] == true) {
							//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
							//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
							if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta)) {
								sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
								icsos++;
							}
						}
						else {
							if (this_is_F_node[Amat.j[is0]] == true) {
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta_strong_F) {
								if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_strong_F)) {
									SumPall += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
									icount_StronglyF++;
									icsosF++;
								}
							}
							// ������������ ���������� ������� ������� �� �������� � ������.
							the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
						}
					}
				}
				if (icsos == 1) {
					number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ ������� � �������.
																		   // ��������� ������ ������ "����������".
																		   // ���������� ������ ����������� ��� ���������.
																		   // � ������� �� �������� ������� ����� ���������� ������ ���������� ���� ��������� ������� � 
																		   // ������������� �� ���� ������� ����� ��������.
					if (icsosF == 0) number_of_F_nodes_with_one_single_strong_C_neighborF++; // ���������� F ����� � ����� ������������ �������  C ������� � � ����-�� �� ������� ������� F �������.
				}



				// 1 ������ 2016 ���� ����� ��� ������������.
				// ������� � ������ ������ ������ ������ ����� ���� � �����.
				if ((false) && (icsos == 1)) {
					this_is_F_node[i8] = false;
					this_is_C_node[i8] = true;
					bweSholdbeContinue = true;
				}
				else {

					if (((icsos == 1) || (icsos == 2) || (icsos == 3)) && (icsosF != 0)) {
						// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
						//
						SumPall += sumP;

						for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
							if (Amat.j[is0] != Amat.i[ii1]) {
								// ��� ���������� ������ Strong �����.


								if (this_is_C_node[Amat.j[is0]] == true) {
									//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
									if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta)) {
										if (fabs(sumP) < RealZERO) {
											//printf("error interpolation zero diagonal sumP.\n");
											//printf("Fnode all sosed is F");
											//system("pause");
											//	printf("i8 is Dirichlet node\n");
											this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
											this_is_C_node[i8] = true;
											bweSholdbeContinue = true;
											iadditionalCstatistic++;
											//exit(1);
											// ����� ����� �������� �������������.
										}
										else {
											// ��� ��� ��� ������������ Strong C �����. 
											// ��������������� ������� �� ��������� � �����.

											// ������ ������� ������ ����������� ��������� 
											// ������������� ��������� �������� �� �������� 
											// �������� �������.
											// ����������� 5 ������� 2015.
											//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
											//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
											if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta)) {
												P[icount1].j = i8;
												P[icount1].i = C_numerate[Amat.j[is0]];
												//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
												if (fabs(SumPall) < 1.0e-23) {
													printf("error 5.0 ! division by zero. SumPall =%e\n", SumPall);
													//getchar();
													system("PAUSE");
													exit(1);
												}
												P[icount1].aij = fabs(Amat.aij[is0]) / SumPall;
												icount1++;
												if (icount1 >= nsizePR*n) {
													printf("memory error!!!\n");
													printf("not enough memory for the interpolation operator.\n");
													//system("PAUSE");
													//exit(1);
													deallocate_prolongation(nsizePR, n,  P);
												}
											}

										}
									}

								}
								else
									if (this_is_F_node[Amat.j[is0]] == true) {
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta_strong_F) {
										if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_strong_F)) {
											// ������������� Strong F �����.

											// ����� :
											// 



											//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
											// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
											// � ������� F ������.
											//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta_strong_F) {

											integer iFpoint = Amat.j[is0];
											doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
											// ��������� ���� ������� iFpointeger 
											// ����� ����� ����� ��� � ����.

											// �������������� ��������� �����.
											doublereal maxelem_threshold_loc = -1.0;
											//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
											integer ii1_loc = row_startA[iFpoint];
											integer istr_etalon = Amat.i[ii1_loc];
											integer iend_for = -1;
											integer iend_marker_position = row_startA[istr_etalon + 1] - 1;
											for (integer is0_loc = ii1_loc; (is0_loc <= iend_marker_position); is0_loc++) {
												iend_for = is0_loc;
												if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc)) {
													if (this_is_C_node[Amat.j[is0_loc]] == true) {
														if (Amat.j[is0_loc] != istr_etalon) {
															maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
														}
													}
												}
											}

											doublereal maxelem_threshold_loc_magic = maxelem_threshold_loc*magic;
											// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

											// ����� ������� ������� F-node ������� C-node.
											integer icsos_loc = 0;

											// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
											// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
											doublereal sumP_loc = 0.0;
											//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

												// ����� �������� ����� ���������� ����� ���������.
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic) {
												if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc_magic)) {






													if (this_is_C_node[Amat.j[is0_loc]] == true) {

														if (Amat.j[is0_loc] != istr_etalon) {

															sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
															icsos_loc++;
														}

													}
													else {

														//if (Amat.j[is0_loc] != istr_etalon) {
														// ������������ ���������� ������� ������� �� �������� � ������.
														//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
														//}
													}

												}
											}

											doublereal maxelem_threshold_loc_magic_minus = -maxelem_threshold_loc_magic;

											// � ����� ��� ������� ����������������� ����� 
											//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

												// ��� �������������� ��������� � ������ ������� ���� �������� ���������� ����� ���������.


												// ��������������� ������� �� ��������� � �����.

												// ������ ������� ������ ����������� ��������� 
												// ������������� ��������� �������� �� �������� 
												// �������� �������.
												// ����������� 5 ������� 2015.
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic) {
												if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc_magic)) {
													//if ((Amat.aij[is0_loc]<maxelem_threshold_loc_magic_minus) || (Amat.aij[is0_loc]>maxelem_threshold_loc_magic)) {

													if (this_is_C_node[Amat.j[is0_loc]] == true) {
														if (Amat.j[is0_loc] != istr_etalon) {

															P[icount1].j = i8;
															P[icount1].i = C_numerate[Amat.j[is0_loc]];
															//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
															if (fabs(sumP_loc) < 1.0e-23) {
																printf("error 6.0 ! division by zero. sumP_loc =%e\n", sumP_loc);
																//getchar();
																system("PAUSE");
																exit(1);
															}
															P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
															icount1++;
															if (icount1 >= nsizePR*n) {
																printf("memory error!!!\n");
																printf("not enough memory for the interpolation operator.\n");
																//system("PAUSE");
																//exit(1);
																deallocate_prolongation(nsizePR, n,  P);
															}

														}
													}
												}
											}


											//}
										}
									}
							}

						}
					}
					else {

						// �������� bmodify_interpol4_amg==true ��� magic2<0.24 �������� � ����������������� ���������
						// �������� ����������. ��� ���������� ����������� �� bmodify_interpol4_amg==true � magic2>=0.24 
						// �������� ������� �� �������� ����� �� ����������� ���������� ����� �� ������� ��� � ��� 
						// bmodify_interpol4_amg = false;
						// 22 01 2016 ����� �������� bmodify_interpol4_amg = false; � �� � ���� ������ �� ������.
						bool bmodify_interpol4_amg = false;
						doublereal magic2 = 0.5;
						//magic2 = magic83;

						// ���������������� ���� � �������� ��������� ������������ 
						// �������� ������������ ���������� ��� ���������� ����� � �����.
						// �� ��������� ������������ �� ���� �������� ������� Weakly C ����� ���� �������
						// ������� �������.
						// ���������������� ���� ����������� � ����������� ��������� is0_candidate ����� �������
						// Weakly C ����� � ������ � ���������� ������������ ������� ����������� � sum1.
						doublereal sum1 = 0.0;
						integer is0_candidate = -1;
						if (bmodify_interpol4_amg && ((icsos == 1) && (icsosF == 0))) {
							for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
								if (Amat.j[is0] != Amat.i[ii1]) {
									if (this_is_C_node[Amat.j[is0]] == true) {
										//if (fabs(Amat.aij[is0]) <= maxelem_threshold*theta) {
										if ((Amat.aij[is0]>0.0) || (fabs(Amat.aij[is0]) <= maxelem_threshold_theta)) {
											if (fabs(Amat.aij[is0]) > sum1) {
												sum1 = fabs(Amat.aij[is0]);
												is0_candidate = is0;
											}
										}
									}
								}
							}

							if (sum1 / sumP < magic2) {
								// ���� ��������� ������� ������� ������ ������ �� ������ � ���� �,
								// � ���� ������ �������� Weakly F ��������� ����� ������� ��� C ����������.
								sum1 = 0.0;
								is0_candidate = -1;

								// ����� ���� ����� Weakly F ����� ���������.
								for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
									if (Amat.j[is0] != Amat.i[ii1]) {
										if (this_is_F_node[Amat.j[is0]] == true) {
											//if (fabs(Amat.aij[is0]) <= maxelem_threshold*theta) {
											if ((Amat.aij[is0]>0.0) || (fabs(Amat.aij[is0]) <= maxelem_threshold_theta)) {
												if (fabs(Amat.aij[is0]) > sum1) {
													sum1 = fabs(Amat.aij[is0]);
													is0_candidate = is0;
												}
											}
										}
									}
								}

								// ����� ���� F ������ ����� ����� � �����.
								if (sum1 / sumP >= magic2) {
									this_is_F_node[i8] = false; // ���� ����  ������ � �����.
									this_is_C_node[i8] = true;
									bweSholdbeContinue = true;
									byes_add = true;
									iadditionalCstatistic++;

								}
								// ������������ ����� ����� ��������� ����� �� ���� ������� �� ����.
								sum1 = 0.0;
								is0_candidate = -1;

							}
						}


						for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
							if (Amat.j[is0] != Amat.i[ii1]) {
								if (this_is_C_node[Amat.j[is0]] == true) {



									if (bmodify_interpol4_amg && ((icsos == 1) && (icsosF == 0))) {
										// �� ��������� ���� ������������ ��� ���������� ����� ����� ��
										// ���� ����� �������������� ����� ������� Weakly C �����.
										// ����� ������� Weakly C ����� ����� ��������� sum1 � is0_candidate.

										ioneStrongC_and_0_StrongF++;
										if (is0_candidate != -1) {
											P[icount1].j = i8;
											P[icount1].i = C_numerate[Amat.j[is0_candidate]];
											P[icount1].aij = fabs(Amat.aij[is0_candidate]) / (sumP + sum1);
											icount1++;
											if (icount1 >= nsizePR*n) {
												printf("memory error!!!\n");
												printf("not enough memory for the interpolation operator.\n");
												//system("PAUSE");
												//exit(1);
												deallocate_prolongation(nsizePR, n,  P);
											}
										}

										//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
										if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta)) {
											P[icount1].j = i8;
											P[icount1].i = C_numerate[Amat.j[is0]];
											P[icount1].aij = fabs(Amat.aij[is0]) / (sumP + sum1);
											icount1++;
											if (icount1 >= nsizePR*n) {
												printf("memory error!!!\n");
												printf("not enough memory for the interpolation operator.\n");
												//system("PAUSE");
												//exit(1);
												deallocate_prolongation(nsizePR, n,  P);
											}
										}
									}
									else {

										if (fabs(sumP) < RealZERO) {
											//printf("error interpolation zero diagonal sumP.\n");
											//printf("Fnode all sosed is F");
											//system("pause");
											//printf("i8 is Dirichlet node\n");
											this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
											this_is_C_node[i8] = true;
											bweSholdbeContinue = true;
											iadditionalCstatistic++;
											//exit(1);
											// ����� ����� �������� �������������.
										}
										else {

											// ��������������� ������� �� ��������� � �����.

											// ������ ������� ������ ����������� ��������� 
											// ������������� ��������� �������� �� �������� 
											// �������� �������.
											// ����������� 5 ������� 2015.
											//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
											//	if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
											if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta)) {
												P[icount1].j = i8;
												P[icount1].i = C_numerate[Amat.j[is0]];
												if (fabs(sumP) < 1.0e-23) {
													printf("error 7.0 ! division by zero. sumP =%e\n", sumP);
													//getchar();
													system("PAUSE");
													exit(1);
												}
												P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
												icount1++;
												if (icount1 >= nsizePR*n) {
													printf("memory error!!!\n");
													printf("not enough memory for the interpolation operator.\n");
													//system("PAUSE");
													//exit(1);
													deallocate_prolongation(nsizePR, n,  P);
												}
											}

										}

									}

								}
							}
						}
					}

				}
			} // end only negative connections

		}

	}

#if doubleintprecision == 1
	//printf("one Strong C and 0 Strong F=%lld additional all %lld", ioneStrongC_and_0_StrongF, iadditionalCstatistic);
#else
	//printf("one Strong C and 0 Strong F=%d additional all %d", ioneStrongC_and_0_StrongF, iadditionalCstatistic);
#endif

	//system("pause");

	// marker END.
} // my_interpolation_procedure_number3

  // ���������������� ��������� �4.
void my_interpolation_procedure_number4(integer &the_number_of_neighbors_that_are_not_�_nodes,
	integer &number_of_F_nodes_with_one_single_strong_C_neighbor,
	integer* &n_a, bool* &this_is_F_node, integer* &row_startA,
	integer* &nnz_a, bool &bpositive_connections, Ak2 &Amat,
	bool &bweSholdbeContinue, bool* &this_is_C_node, integer &iadditionalCstatistic,
	const doublereal RealZERO, integer &icount1, Ak1* &P, integer &nsizePR, integer &ilevel,
	integer &iadd, doublereal &theta, integer &n,  integer* &C_numerate,
	integer &number_of_F_nodes_with_one_single_strong_C_neighborF,
	doublereal &theta83, bool &btreshold_on_new_vetv, integer& ifrom_re_operation_protection,
	bool &from_re_operation_protection0, doublereal &magic82, doublereal* &threshold_quick_all,
	doublereal* &threshold_quick_only_negative)
{
	// ����� �������.
	// ���������� ����� 1.22 ������ ������� � 1.36 � ��������� �������.

	// ����� ���� F �� ������� Strong � ������� ��� ���������� � �����.
	// ���� F ������� ������ Strong  � ������ �������������� � ������� ������� � ������� 
	// ������� F �����.

	the_number_of_neighbors_that_are_not_�_nodes = 0;
	number_of_F_nodes_with_one_single_strong_C_neighbor = 0;

	if (bpositive_connections) {

		// ���������� ����������� ��� ����� ������� ���������� F nodes.
		// ������ F-nodes ������ C-nodes.
		for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {



			// ��� ����� ����������� �������� ��������.
			// 5 ������� 2015 ���� �� ��������� ��������� �������������
			// ��������� ������������� � ������ � ��������� ��������.
			doublereal maxelem_threshold = -1.0;
			//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
			integer ii1 = row_startA[i8];
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
					//if (this_is_C_node[Amat.j[is0]] == true) {
					if (fabs(Amat.aij[is0]) > maxelem_threshold) {
						maxelem_threshold = fabs(Amat.aij[is0]);
					}
					//}
				}
			}
			// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

			// ����� ������� ������� F-node ������� C-node.
			integer icsos = 0;
			integer icsosF = 0;

			// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
			// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
			doublereal sumP = 0.0;
			doublereal SumPall = 0.0;
			integer icount_StronglyF = 0;
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					if (this_is_C_node[Amat.j[is0]] == true) {
						//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
						if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
							sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
							icsos++;
						}
					}
					else {
						if (this_is_F_node[Amat.j[is0]] == true) {
							if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
								SumPall += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
								icount_StronglyF++;
								icsosF++;
							}
						}
						// ������������ ���������� ������� ������� �� �������� � ������.
						the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
					}
				}
			}
			if (icsos == 1) {
				number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ �������  � �������.
																	   // ��������� ������ ������ "����������".
																	   // ���������� ������ ����������� ��� ���������.
																	   // � ������� �� �������� ������� ����� ���������� ������ ���������� ���� ��������� ������� � 
																	   // ������������� �� ���� ������� ����� ��������.
				if (icsosF == 0) number_of_F_nodes_with_one_single_strong_C_neighborF++; // ���������� F ����� � ����� ������������ �������  C ������� � � ����-�� �� ������� ������� F �������.
			}



			// 1 ������ 2016 ���� ����� ��� ������������.
			// ������� � ������ ������ ������ ������ ����� ���� � �����.
			if ((false) && (icsos == 1)) {
				this_is_F_node[i8] = false;
				this_is_C_node[i8] = true;
				bweSholdbeContinue = true;
			}
			else {

				if ((icsos == 1) && (icsosF != 0)) {
					// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
					//
					SumPall += sumP;

					for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							// ��� ���������� ������ Strong �����.
							if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
								if (this_is_C_node[Amat.j[is0]] == true) {

									if (fabs(sumP) < RealZERO) {
										//printf("error interpolation zero diagonal sumP.\n");
										//printf("Fnode all sosed is F");
										//system("pause");
										//	printf("i8 is Dirichlet node\n");
										if (this_is_C_node[i8] == false) iadditionalCstatistic++;
										this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
										this_is_C_node[i8] = true;
										bweSholdbeContinue = true;
										//exit(1);
										// ����� ����� �������� �������������.
									}
									else {
										// ��� ��� ��� ������������ Strong C �����. 
										// ��������������� ������� �� ��������� � �����.

										// ������ ������� ������ ����������� ��������� 
										// ������������� ��������� �������� �� �������� 
										// �������� �������.
										// ����������� 5 ������� 2015.
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
											P[icount1].j = i8;
											P[icount1].i = C_numerate[Amat.j[is0]];
											//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
											if (fabs(SumPall) < 1.0e-23) {
												printf("error 1.0 ! division by zero. SumPall =%e\n", SumPall);
												//getchar();
												system("PAUSE");
												exit(1);
											}
											P[icount1].aij = fabs(Amat.aij[is0]) / SumPall;
											icount1++;
											if (icount1 >= nsizePR*n) {
												printf("memory error!!!\n");
												printf("not enough memory for the interpolation operator.\n");
												//system("PAUSE");
												//exit(1);
												deallocate_prolongation(nsizePR, n,  P);
											}
										}

									}

								}
								else
									if (this_is_F_node[Amat.j[is0]] == true) {

										// ������������� Strong F �����.

										// ����� :
										// 



										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
										// � ������� F ������.
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {

										integer iFpoint = Amat.j[is0];
										if (fabs(SumPall) < 1.0e-23) {
											printf("error 2.0 ! division by zero. SumPall =%e\n", SumPall);
											//getchar();
											system("PAUSE");
											exit(1);
										}
										doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
										// ��������� ���� ������� iFpointeger 
										// ����� ����� ����� ��� � ����.

										// �������������� ��������� �����.
										doublereal maxelem_threshold_loc = -1.0;
										//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
										integer ii1_loc = row_startA[iFpoint];
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc) {
														maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
													}
												}
											}
										}


										// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

										// ����� ������� ������� F-node ������� C-node.
										integer icsos_loc = 0;

										// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
										// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
										doublereal sumP_loc = 0.0;
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {




												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
													if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
														sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
														icsos_loc++;
													}
												}
												else {
													// ������������ ���������� ������� ������� �� �������� � ������.
													//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
												}
											}
										}

										// � ����� ��� ������� ����������������� ����� 
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													// ��������������� ������� �� ��������� � �����.

													// ������ ������� ������ ����������� ��������� 
													// ������������� ��������� �������� �� �������� 
													// �������� �������.
													// ����������� 5 ������� 2015.
													//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
													if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
														P[icount1].j = i8;
														P[icount1].i = C_numerate[Amat.j[is0_loc]];
														//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
														if (fabs(sumP_loc) < 1.0e-23) {
															printf("error 3.0 ! division by zero. sumP_loc =%e\n", sumP_loc);
															//getchar();
															system("PAUSE");
															exit(1);
														}
														P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
														icount1++;
														if (icount1 >= nsizePR*n) {
															printf("memory error!!!\n");
															printf("not enough memory for the interpolation operator.\n");
															//system("PAUSE");
															//exit(1);
															deallocate_prolongation(nsizePR, n,  P);
														}
													}
												}
											}
										}


										//}
									}
							}
						}
					}
				}
				else {

					SumPall += sumP;

					for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {

							if (this_is_C_node[Amat.j[is0]] == true) {

								if (fabs(sumP) < RealZERO) {
									//printf("error interpolation zero diagonal sumP.\n");
									//printf("Fnode all sosed is F");
									//system("pause");
									//printf("i8 is Dirichlet node\n");
									if (this_is_C_node[i8] == false) iadditionalCstatistic++;
									this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
									this_is_C_node[i8] = true;
									bweSholdbeContinue = true;
									//exit(1);
									// ����� ����� �������� �������������.
								}
								else {

									// ��������������� ������� �� ��������� � �����.

									// ������ ������� ������ ����������� ��������� 
									// ������������� ��������� �������� �� �������� 
									// �������� �������.
									// ����������� 5 ������� 2015.
									//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
									if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
										P[icount1].j = i8;
										P[icount1].i = C_numerate[Amat.j[is0]];
										if (fabs(SumPall) < 1.0e-23) {
											printf("error 4.0 ! division by zero. SumPall =%e\n", SumPall);
											//getchar();
											system("PAUSE");
											exit(1);
										}
										P[icount1].aij = fabs(Amat.aij[is0]) / SumPall;
										//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
										icount1++;
										if (icount1 >= nsizePR*n) {
											printf("memory error!!!\n");
											printf("not enough memory for the interpolation operator.\n");
											//system("PAUSE");
											//exit(1);
											deallocate_prolongation(nsizePR, n,  P);
										}
									}

								}

							}
							else
								if (this_is_F_node[Amat.j[is0]] == true) {

									// ������������� Strong F �����.
									// �� ������������� ��������� Strong C ������������ �� ���� ������� Strong F ������������.
									// ����� :
									//



									//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
									// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
									// � ������� F ������.
									if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {

										integer iFpoint = Amat.j[is0];
										if (fabs(SumPall) < 1.0e-23) {
											printf("error 5.0 ! division by zero. SumPall =%e\n", SumPall);
											//getchar();
											system("PAUSE");
											exit(1);
										}
										doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
										// ��������� ���� ������� iFpoint
										// ����� ����� ����� ��� � ����.

										// �������������� ��������� �����.
										doublereal maxelem_threshold_loc = -1.0;
										//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
										integer ii1_loc = row_startA[iFpoint];
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc) {
														maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
													}
												}
											}
										}


										// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

										// ����� ������� ������� F-node ������� C-node.
										integer icsos_loc = 0;

										// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
										// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
										doublereal sumP_loc = 0.0;
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {




												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
													if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
														sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
														icsos_loc++;
													}
												}
												else {
													// ������������ ���������� ������� ������� �� �������� � ������.
													//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������
												}
											}
										}

										// � ����� ��� ������� ����������������� �����
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													// ��������������� ������� �� ��������� � �����.

													// ������ ������� ������ ����������� ���������
													// ������������� ��������� �������� �� ��������
													// �������� �������.
													// ����������� 5 ������� 2015.
													//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
													if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
														P[icount1].j = i8;
														P[icount1].i = C_numerate[Amat.j[is0_loc]];
														//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
														if (fabs(sumP_loc) < 1.0e-23) {
															printf("error 6.0 ! division by zero. sumP_loc =%e\n", sumP_loc);
															//getchar();
															system("PAUSE");
															exit(1);
														}
														P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
														icount1++;
														if (icount1 >= nsizePR*n) {
															printf("memory error!!!\n");
															printf("not enough memory for the interpolation operator.\n");
															//system("PAUSE");
															//exit(1);
															deallocate_prolongation(nsizePR, n,  P);
														}
													}
												}
											}
										}


										//}
									}
								}


						}
					}
				}

			}

		}
		//// positive connections
	}
	else {
		// only negative connections


		// ���������� ����������� ��� ����� ������� ���������� F nodes.
		// ������ F-nodes ������ C-nodes.
		for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {

			// ��� ����� ����������� �������� ��������.
			// 5 ������� 2015 ���� �� ��������� ��������� �������������
			// ��������� ������������� � ������ � ��������� ��������.
			doublereal maxelem_threshold = -1.0;
			//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
			integer ii1 = row_startA[i8];
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
					//if (this_is_C_node[Amat.j[is0]] == true) {
					if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold)) {
						maxelem_threshold = fabs(Amat.aij[is0]);
					}
					//}
				}
			}
			// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

			// ����� ������� ������� F-node ������� C-node.
			integer icsos = 0;
			integer icsosF = 0;

			// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
			// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
			doublereal sumP = 0.0;
			doublereal SumPall = 0.0;
			integer icount_StronglyF = 0;
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					if (this_is_C_node[Amat.j[is0]] == true) {
						//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
						if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
							sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
							icsos++;
						}
					}
					else {
						if (this_is_F_node[Amat.j[is0]] == true) {
							if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
								SumPall += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
								icount_StronglyF++;
								icsosF++;
							}
						}
						// ������������ ���������� ������� ������� �� �������� � ������.
						the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
					}
				}
			}
			if (icsos == 1) {
				number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ �������  � �������.
																	   // ��������� ������ ������ "����������".
																	   // ���������� ������ ����������� ��� ���������.
																	   // � ������� �� �������� ������� ����� ���������� ������ ���������� ���� ��������� ������� � 
																	   // ������������� �� ���� ������� ����� ��������.
				if (icsosF == 0) number_of_F_nodes_with_one_single_strong_C_neighborF++; // ���������� F ����� � ����� ������������ �������  C ������� � � ����-�� �� ������� ������� F �������.
			}



			// 1 ������ 2016 ���� ����� ��� ������������.
			// ������� � ������ ������ ������ ������ ����� ���� � �����.
			if ((false) && (icsos == 1)) {
				this_is_F_node[i8] = false;
				this_is_C_node[i8] = true;
				bweSholdbeContinue = true;
			}
			else {

				if ((icsos == 1) && (icsosF != 0)) {
					// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
					//
					SumPall += sumP;

					for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							// ��� ���������� ������ Strong �����.
							if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
								if (this_is_C_node[Amat.j[is0]] == true) {

									if (fabs(sumP) < RealZERO) {
										//printf("error interpolation zero diagonal sumP.\n");
										//printf("Fnode all sosed is F");
										//system("pause");
										//	printf("i8 is Dirichlet node\n");
										if (this_is_C_node[i8] == false) iadditionalCstatistic++;
										this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
										this_is_C_node[i8] = true;
										bweSholdbeContinue = true;
										//exit(1);
										// ����� ����� �������� �������������.
									}
									else {
										// ��� ��� ��� ������������ Strong C �����. 
										// ��������������� ������� �� ��������� � �����.

										// ������ ������� ������ ����������� ��������� 
										// ������������� ��������� �������� �� �������� 
										// �������� �������.
										// ����������� 5 ������� 2015.
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
											P[icount1].j = i8;
											P[icount1].i = C_numerate[Amat.j[is0]];
											//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
											if (fabs(SumPall) < 1.0e-23) {
												printf("error 7.0 ! division by zero. SumPall =%e\n", SumPall);
												//getchar();
												system("PAUSE");
												exit(1);
											}
											P[icount1].aij = fabs(Amat.aij[is0]) / SumPall;
											icount1++;
											if (icount1 >= nsizePR*n) {
												printf("memory error!!!\n");
												printf("not enough memory for the interpolation operator.\n");
												//system("PAUSE");
												//exit(1);
												deallocate_prolongation(nsizePR, n,  P);
											}
										}

									}

								}
								else
									if (this_is_F_node[Amat.j[is0]] == true) {

										// ������������� Strong F �����.

										// ����� :
										// 



										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
										// � ������� F ������.
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {

										integer iFpoint = Amat.j[is0];
										if (fabs(SumPall) < 1.0e-23) {
											printf("error 8.0 ! division by zero. SumPall =%e\n", SumPall);
											//getchar();
											system("PAUSE");
											exit(1);
										}
										doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
										// ��������� ���� ������� iFpointeger 
										// ����� ����� ����� ��� � ����.

										// �������������� ��������� �����.
										doublereal maxelem_threshold_loc = -1.0;
										//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
										integer ii1_loc = row_startA[iFpoint];
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc)) {
														maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
													}
												}
											}
										}


										// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

										// ����� ������� ������� F-node ������� C-node.
										integer icsos_loc = 0;

										// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
										// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
										doublereal sumP_loc = 0.0;
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {




												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
													if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta)) {
														sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
														icsos_loc++;
													}
												}
												else {
													// ������������ ���������� ������� ������� �� �������� � ������.
													//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
												}
											}
										}

										// � ����� ��� ������� ����������������� ����� 
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													// ��������������� ������� �� ��������� � �����.

													// ������ ������� ������ ����������� ��������� 
													// ������������� ��������� �������� �� �������� 
													// �������� �������.
													// ����������� 5 ������� 2015.
													//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
													if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta)) {
														P[icount1].j = i8;
														P[icount1].i = C_numerate[Amat.j[is0_loc]];
														//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
														if (fabs(sumP_loc) < 1.0e-23) {
															printf("error 9.0 ! division by zero. sumP_loc =%e\n", sumP_loc);
															//getchar();
															system("PAUSE");
															exit(1);
														}
														P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
														icount1++;
														if (icount1 >= nsizePR*n) {
															printf("memory error!!!\n");
															printf("not enough memory for the interpolation operator.\n");
															//system("PAUSE");
															//exit(1);
															deallocate_prolongation(nsizePR, n,  P);
														}
													}
												}
											}
										}


										//}
									}
							}
						}
					}
				}
				else {

					SumPall += sumP;

					for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {

							if (this_is_C_node[Amat.j[is0]] == true) {

								if (fabs(sumP) < RealZERO) {
									//printf("error interpolation zero diagonal sumP.\n");
									//printf("Fnode all sosed is F");
									//system("pause");
									//printf("i8 is Dirichlet node\n");
									if (this_is_C_node[i8] == false) iadditionalCstatistic++;
									this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
									this_is_C_node[i8] = true;
									bweSholdbeContinue = true;
									//exit(1);
									// ����� ����� �������� �������������.
								}
								else {

									// ��������������� ������� �� ��������� � �����.

									// ������ ������� ������ ����������� ��������� 
									// ������������� ��������� �������� �� �������� 
									// �������� �������.
									// ����������� 5 ������� 2015.
									//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
									if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
										P[icount1].j = i8;
										P[icount1].i = C_numerate[Amat.j[is0]];
										if (fabs(SumPall) < 1.0e-23) {
											printf("error 10.0 ! division by zero. SumPall =%e\n", SumPall);
											//getchar();
											system("PAUSE");
											exit(1);
										}
										P[icount1].aij = fabs(Amat.aij[is0]) / SumPall;
										//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
										icount1++;
										if (icount1 >= nsizePR*n) {
											printf("memory error!!!\n");
											printf("not enough memory for the interpolation operator.\n");
											//system("PAUSE");
											//exit(1);
											deallocate_prolongation(nsizePR, n,  P);
										}
									}

								}

							}
							else
								if (this_is_F_node[Amat.j[is0]] == true) {

									// ������������� Strong F �����.
									// �� ������������� ��������� Strong C ������������ �� ���� ������� Strong F ������������.
									// ����� :
									//



									//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
									// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
									// � ������� F ������.
									if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {

										integer iFpoint = Amat.j[is0];
										doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
										// ��������� ���� ������� iFpoint
										// ����� ����� ����� ��� � ����.

										// �������������� ��������� �����.
										doublereal maxelem_threshold_loc = -1.0;
										//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
										integer ii1_loc = row_startA[iFpoint];
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc)) {
														maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
													}
												}
											}
										}


										// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

										// ����� ������� ������� F-node ������� C-node.
										integer icsos_loc = 0;

										// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
										// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
										doublereal sumP_loc = 0.0;
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {




												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
													if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta)) {
														sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
														icsos_loc++;
													}
												}
												else {
													// ������������ ���������� ������� ������� �� �������� � ������.
													//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������
												}
											}
										}

										// � ����� ��� ������� ����������������� �����
										for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
											if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													// ��������������� ������� �� ��������� � �����.

													// ������ ������� ������ ����������� ���������
													// ������������� ��������� �������� �� ��������
													// �������� �������.
													// ����������� 5 ������� 2015.
													//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
													if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta)) {
														P[icount1].j = i8;
														P[icount1].i = C_numerate[Amat.j[is0_loc]];
														//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
														if (fabs(sumP_loc) < 1.0e-23) {
															printf("error 11.0 ! division by zero. sumP_loc =%e\n", sumP_loc);
															//getchar();
															system("PAUSE");
															exit(1);
														}
														P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
														icount1++;
														if (icount1 >= nsizePR*n) {
															printf("memory error!!!\n");
															printf("not enough memory for the interpolation operator.\n");
															//system("PAUSE");
															//exit(1);
															deallocate_prolongation(nsizePR, n,  P);
														}
													}
												}
											}
										}


										//}
									}
								}


						}
					}
				}

			}

		}

	} // end only negative connections

} // my_interpolation_procedure_number4

  // ���������������� ��������� �5.
void my_interpolation_procedure_number5(integer &the_number_of_neighbors_that_are_not_�_nodes,
	integer &number_of_F_nodes_with_one_single_strong_C_neighbor,
	integer* &n_a, bool* &this_is_F_node, integer* &row_startA,
	integer* &nnz_a, bool &bpositive_connections, Ak2 &Amat,
	bool &bweSholdbeContinue, bool* &this_is_C_node, integer &iadditionalCstatistic,
	const doublereal RealZERO, integer &icount1, Ak1* &P, integer &nsizePR, integer &ilevel,
	integer &iadd, doublereal &theta, integer &n,  integer* &C_numerate,
	integer &number_of_F_nodes_with_one_single_strong_C_neighborF,
	doublereal &theta83, bool &btreshold_on_new_vetv, integer& ifrom_re_operation_protection,
	bool &from_re_operation_protection0, doublereal &magic82, doublereal* &threshold_quick_all,
	doublereal* &threshold_quick_only_negative)
{
	bool byes_add = false;
	//  ������� �������� �� ����������.
	if (1) {
		// ������ �������.
		// CGHV1J amg1r5 time 0.26
		// cl_agl_amg_v0_14 time :
		// magic  time number V cycles grid complexity
		// 0.25   0:48  29  6.55A
		// 0.3    0:45  28  6.19A 
		// 0.35  0:43   28  5.98A
		// 0.4   0:41   27  5.77A // �������� 14% ������������������.
		// tgf2023_10 amg1r5 time 1:00
		// cl_agl_amg_v0_14 time
		// magic  time number V cycles grid complexity
		// 0.25 1:49 27 3.61
		// 0.3 1:47 26 3.47
		// 0.35 1:46 27 3.51
		// 0.4 1:44 28 3.413 // �������� 5% ������������������.
		// Electric potencial in FET2 amg1r5 time 1:38
		// cl_agl_amg_v0_14 time :
		// magic  time number V cycles grid complexity
		// 0.25 3:19 36 6.65
		// 0.3 3:02 34 6.34
		// 0.35 2:52 33 6.091
		// 0.4 2:46 33 5.91 // �������� 20% ������������������.

		// magic=0.4 is optimum.11 jan 2016
		doublereal magic = 0.4; // 0.4

								// ����� ���� F �� ������� Strong � ������� ��� ���������� � �����.
								// ���� F ������� ������ Strong  � ������ �������������� � ������� ������� � ������� 
								// ������� F �����.

		the_number_of_neighbors_that_are_not_�_nodes = 0;
		number_of_F_nodes_with_one_single_strong_C_neighbor = 0;

		if (bpositive_connections) {

			// positive connections:

			// ���������� ����������� ��� ����� ������� ���������� F nodes.
			// ������ F-nodes ������ C-nodes.
			for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {

				// ��� ����� ����������� �������� ��������.
				// 5 ������� 2015 ���� �� ��������� ��������� �������������
				// ��������� ������������� � ������ � ��������� ��������.
				doublereal maxelem_threshold = -1.0;
				//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
				integer ii1 = row_startA[i8];
				for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
					if (Amat.j[is0] != Amat.i[ii1]) {
						// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
						//if (this_is_C_node[Amat.j[is0]] == true) {
						if (fabs(Amat.aij[is0]) > maxelem_threshold) {
							maxelem_threshold = fabs(Amat.aij[is0]);
						}
						//}
					}
				}
				// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

				// ����� ������� ������� F-node ������� C-node.
				integer icsos = 0;
				integer icsosF = 0;

				// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
				// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
				doublereal sumP = 0.0;
				doublereal SumPall = 0.0;
				integer icount_StronglyF = 0;
				for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
					if (Amat.j[is0] != Amat.i[ii1]) {
						if (this_is_C_node[Amat.j[is0]] == true) {
							//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
							if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
								sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
								icsos++;
							}
						}
						else {
							if (this_is_F_node[Amat.j[is0]] == true) {
								if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
									SumPall += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
									icount_StronglyF++;
									icsosF++;
								}
							}
							// ������������ ���������� ������� ������� �� �������� � ������.
							the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
						}
					}
				}
				if (icsos == 1) {
					number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ �������  � �������.
																		   // ��������� ������ ������ "����������".
																		   // ���������� ������ ����������� ��� ���������.
																		   // � ������� �� �������� ������� ����� ���������� ������ ���������� ���� ��������� ������� � 
																		   // ������������� �� ���� ������� ����� ��������.
					if (icsosF == 0) number_of_F_nodes_with_one_single_strong_C_neighborF++; // ���������� F ����� � ����� ������������ �������  C ������� � � ����-�� �� ������� ������� F �������.
				}



				// 1 ������ 2016 ���� ����� ��� ������������.
				// ������� � ������ ������ ������ ������ ����� ���� � �����.
				{

					if ((icsos == 1) && (icsosF != 0)) {
						// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
						//
						SumPall += sumP;

						for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
							if (Amat.j[is0] != Amat.i[ii1]) {
								// ��� ���������� ������ Strong �����.
								if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
									if (this_is_C_node[Amat.j[is0]] == true) {

										if (fabs(sumP) < RealZERO) {
											//printf("error interpolation zero diagonal sumP.\n");
											//printf("Fnode all sosed is F");
											//system("pause");
											//	printf("i8 is Dirichlet node\n");
											this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
											this_is_C_node[i8] = true;
											bweSholdbeContinue = true;
											iadditionalCstatistic++;
											byes_add = true;
											//exit(1);
											// ����� ����� �������� �������������.
										}


									}

								}
							}
						}
					}
					else {



						for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
							if (Amat.j[is0] != Amat.i[ii1]) {

								if (this_is_C_node[Amat.j[is0]] == true) {

									if (fabs(sumP) < RealZERO) {
										//printf("error interpolation zero diagonal sumP.\n");
										//printf("Fnode all sosed is F");
										//system("pause");
										//printf("i8 is Dirichlet node\n");
										this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
										this_is_C_node[i8] = true;
										bweSholdbeContinue = true;
										iadditionalCstatistic++;
										byes_add = true;
										//exit(1);
										// ����� ����� �������� �������������.
									}


								}



							}
						}



					}

				}

			}

			/////
		}
		else {
			// only negative connections:

			// ���������� ����������� ��� ����� ������� ���������� F nodes.
			// ������ F-nodes ������ C-nodes.
			for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {

				// ��� ����� ����������� �������� ��������.
				// 5 ������� 2015 ���� �� ��������� ��������� �������������
				// ��������� ������������� � ������ � ��������� ��������.
				doublereal maxelem_threshold = -1.0;
				//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
				integer ii1 = row_startA[i8];
				for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
					if (Amat.j[is0] != Amat.i[ii1]) {
						// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
						//if (this_is_C_node[Amat.j[is0]] == true) {
						if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold)) {
							maxelem_threshold = fabs(Amat.aij[is0]);
						}
						//}
					}
				}
				// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

				// ����� ������� ������� F-node ������� C-node.
				integer icsos = 0;
				integer icsosF = 0;

				// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
				// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
				doublereal sumP = 0.0;
				doublereal SumPall = 0.0;
				integer icount_StronglyF = 0;
				for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
					if (Amat.j[is0] != Amat.i[ii1]) {
						if (this_is_C_node[Amat.j[is0]] == true) {
							//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
							if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
								sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
								icsos++;
							}
						}
						else {
							if (this_is_F_node[Amat.j[is0]] == true) {
								if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
									SumPall += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
									icount_StronglyF++;
									icsosF++;
								}
							}
							// ������������ ���������� ������� ������� �� �������� � ������.
							the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
						}
					}
				}
				if (icsos == 1) {
					number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ � �������.
																		   // ��������� ������ ������ "����������".
																		   // ���������� ������ ����������� ��� ���������.
																		   // � ������� �� �������� ������� ����� ���������� ������ ���������� ���� ��������� ������� � 
																		   // ������������� �� ���� ������� ����� ��������.
					if (icsosF == 0) number_of_F_nodes_with_one_single_strong_C_neighborF++; // ���������� F ����� � ����� ������������ C ������� � � ����-�� �� ������� ������� F �������.
				}



				// 1 ������ 2016 ���� ����� ��� ������������.
				// ������� � ������ ������ ������ ������ ����� ���� � �����.
				{

					if ((icsos == 1) && (icsosF != 0)) {
						// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
						//
						SumPall += sumP;

						for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
							if (Amat.j[is0] != Amat.i[ii1]) {
								// ��� ���������� ������ Strong �����.
								if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
									if (this_is_C_node[Amat.j[is0]] == true) {

										if (fabs(sumP) < RealZERO) {
											//printf("error interpolation zero diagonal sumP.\n");
											//printf("Fnode all sosed is F");
											//system("pause");
											//	printf("i8 is Dirichlet node\n");
											this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
											this_is_C_node[i8] = true;
											bweSholdbeContinue = true;
											iadditionalCstatistic++;
											byes_add = true;
											//exit(1);
											// ����� ����� �������� �������������.
										}


									}

								}
							}
						}
					}
					else {



						for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
							if (Amat.j[is0] != Amat.i[ii1]) {

								if (this_is_C_node[Amat.j[is0]] == true) {

									if (fabs(sumP) < RealZERO) {
										//printf("error interpolation zero diagonal sumP.\n");
										//printf("Fnode all sosed is F");
										//system("pause");
										//printf("i8 is Dirichlet node\n");
										this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
										this_is_C_node[i8] = true;
										bweSholdbeContinue = true;
										iadditionalCstatistic++;
										byes_add = true;
										//exit(1);
										// ����� ����� �������� �������������.
									}


								}



							}
						}



					}

				}

			}


		} // end only negative connecions.
	}


	// �������� �� ���������� �������� �������, 
	// ���������� �� ���� � ����� ��������������� !!!.
	if (byes_add == false) {

		// ������ �������.
		// CGHV1J amg1r5 time 0.26
		// cl_agl_amg_v0_14 time :
		// magic  time number V cycles grid complexity
		// 0.25   0:48  29  6.55A
		// 0.3    0:45  28  6.19A 
		// 0.35  0:43   28  5.98A
		// 0.4   0:41   27  5.77A // �������� 14% ������������������.
		// tgf2023_10 amg1r5 time 1:00
		// cl_agl_amg_v0_14 time
		// magic  time number V cycles grid complexity
		// 0.25 1:49 27 3.61
		// 0.3 1:47 26 3.47
		// 0.35 1:46 27 3.51
		// 0.4 1:44 28 3.413 // �������� 5% ������������������.
		// Electric potencial in FET2 amg1r5 time 1:38
		// cl_agl_amg_v0_14 time :
		// magic  time number V cycles grid complexity
		// 0.25 3:19 36 6.65
		// 0.3 3:02 34 6.34
		// 0.35 2:52 33 6.091
		// 0.4 2:46 33 5.91 // �������� 20% ������������������.

		// magic=0.4 is optimum.11 jan 2016
		doublereal magic = 0.4; // 0.4

								// ����� ���� F �� ������� Strong � ������� ��� ���������� � �����.
								// ���� F ������� ������ Strong  � ������ �������������� � ������� ������� � ������� 
								// ������� F �����.

		the_number_of_neighbors_that_are_not_�_nodes = 0;
		number_of_F_nodes_with_one_single_strong_C_neighbor = 0;

		if (bpositive_connections) {

			// positive connections :

			// ���������� ����������� ��� ����� ������� ���������� F nodes.
			// ������ F-nodes ������ C-nodes.
			for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {

				// ��� ����� ����������� �������� ��������.
				// 5 ������� 2015 ���� �� ��������� ��������� �������������
				// ��������� ������������� � ������ � ��������� ��������.
				doublereal maxelem_threshold = -1.0;
				//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
				integer ii1 = row_startA[i8];
				for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
					if (Amat.j[is0] != Amat.i[ii1]) {
						// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
						//if (this_is_C_node[Amat.j[is0]] == true) {
						if (fabs(Amat.aij[is0]) > maxelem_threshold) {
							maxelem_threshold = fabs(Amat.aij[is0]);
						}
						//}
					}
				}
				// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

				// ����� ������� ������� F-node ������� C-node.
				integer icsos = 0;
				integer icsosF = 0;

				// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
				// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
				doublereal sumP = 0.0;
				doublereal SumPall = 0.0;
				integer icount_StronglyF = 0;
				for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
					if (Amat.j[is0] != Amat.i[ii1]) {
						if (this_is_C_node[Amat.j[is0]] == true) {
							//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
							if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
								sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
								icsos++;
							}
						}
						else {
							if (this_is_F_node[Amat.j[is0]] == true) {
								if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
									SumPall += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
									icount_StronglyF++;
									icsosF++;
								}
							}
							// ������������ ���������� ������� ������� �� �������� � ������.
							the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
						}
					}
				}
				if (icsos == 1) {
					number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ �������  � �������.
																		   // ��������� ������ ������ "����������".
																		   // ���������� ������ ����������� ��� ���������.
																		   // � ������� �� �������� ������� ����� ���������� ������ ���������� ���� ��������� ������� � 
																		   // ������������� �� ���� ������� ����� ��������.
					if (icsosF == 0) number_of_F_nodes_with_one_single_strong_C_neighborF++; // ���������� F ����� � ����� ������������ �������  C ������� � � ����-�� �� ������� ������� F �������.
				}



				// 1 ������ 2016 ���� ����� ��� ������������.
				// ������� � ������ ������ ������ ������ ����� ���� � �����.
				if ((false) && (icsos == 1)) {
					this_is_F_node[i8] = false;
					this_is_C_node[i8] = true;
					bweSholdbeContinue = true;
				}
				else {

					//if ((icsos == 1) && (icsosF != 0)) {
					// 28.01.2017
					if (((icsos == 1) || (icsos == 2) || (icsos == 3)) && (icsosF != 0)) {
						// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
						//
						SumPall += sumP;

						for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
							if (Amat.j[is0] != Amat.i[ii1]) {
								// ��� ���������� ������ Strong �����.
								if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
									if (this_is_C_node[Amat.j[is0]] == true) {

										if (fabs(sumP) < RealZERO) {
											//printf("error interpolation zero diagonal sumP.\n");
											//printf("Fnode all sosed is F");
											//system("pause");
											//	printf("i8 is Dirichlet node\n");
											this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
											this_is_C_node[i8] = true;
											bweSholdbeContinue = true;
											iadditionalCstatistic++;
											//exit(1);
											// ����� ����� �������� �������������.
										}
										else {
											// ��� ��� ��� ������������ Strong C �����. 
											// ��������������� ������� �� ��������� � �����.

											// ������ ������� ������ ����������� ��������� 
											// ������������� ��������� �������� �� �������� 
											// �������� �������.
											// ����������� 5 ������� 2015.
											//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
											if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
												P[icount1].j = i8;
												P[icount1].i = C_numerate[Amat.j[is0]];
												//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
												P[icount1].aij = fabs(Amat.aij[is0]) / SumPall;
												icount1++;
												if (icount1 >= nsizePR*n) {
													printf("memory error!!!\n");
													printf("not enough memory for the interpolation operator.\n");
													//system("PAUSE");
													//exit(1);
													deallocate_prolongation(nsizePR, n,  P);
												}
											}

										}

									}
									else
										if (this_is_F_node[Amat.j[is0]] == true) {

											// ������������� Strong F �����.

											// ����� :
											// 



											//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
											// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
											// � ������� F ������.
											//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {

											integer iFpoint = Amat.j[is0];
											doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
											// ��������� ���� ������� iFpointeger 
											// ����� ����� ����� ��� � ����.

											// �������������� ��������� �����.
											doublereal maxelem_threshold_loc = -1.0;
											//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
											integer ii1_loc = row_startA[iFpoint];
											for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
												if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
													if (this_is_C_node[Amat.j[is0_loc]] == true) {
														if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc) {
															maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
														}
													}
												}
											}


											// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

											// ����� ������� ������� F-node ������� C-node.
											integer icsos_loc = 0;

											// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
											// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
											doublereal sumP_loc = 0.0;
											for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
												if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {




													if (this_is_C_node[Amat.j[is0_loc]] == true) {
														//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
														//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
														if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic) {
															sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
															icsos_loc++;
														}
													}
													else {
														// ������������ ���������� ������� ������� �� �������� � ������.
														//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
													}
												}
											}

											// � ����� ��� ������� ����������������� ����� 
											for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
												if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
													if (this_is_C_node[Amat.j[is0_loc]] == true) {
														// ��������������� ������� �� ��������� � �����.

														// ������ ������� ������ ����������� ��������� 
														// ������������� ��������� �������� �� �������� 
														// �������� �������.
														// ����������� 5 ������� 2015.
														//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
														//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
														if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic) {
															P[icount1].j = i8;
															P[icount1].i = C_numerate[Amat.j[is0_loc]];
															//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
															P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
															icount1++;
															if (icount1 >= nsizePR*n) {
																printf("memory error!!!\n");
																printf("not enough memory for the interpolation operator.\n");
																//system("PAUSE");
																//exit(1);
																deallocate_prolongation(nsizePR, n,  P);
															}
														}
													}
												}
											}


											//}
										}
								}
							}
						}
					}
					else {

						SumPall += sumP;
						doublereal sum_ok = 0.0;
						bool bbackreturn = false;
						integer icount1_mem = icount1;
						bool* bbackrarr = NULL;
						//bbackrarr = new bool[icount_StronglyF];
						bbackrarr = (bool*)malloc(icount_StronglyF * sizeof(bool));
						char c1[27] = "classic_aglomerative_amg_4";
						char c2[10] = "bbackrarr";
						handle_error(bbackrarr, c2, c1, icount_StronglyF);

						for (integer i_63 = 0; i_63 < icount_StronglyF; i_63++) {
							bbackrarr[i_63] = false;
						}
						integer icounter_bbackarr = -1;

						for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
							if (Amat.j[is0] != Amat.i[ii1]) {

								if (this_is_C_node[Amat.j[is0]] == true) {

									if (fabs(sumP) < RealZERO) {
										//printf("error interpolation zero diagonal sumP.\n");
										//printf("Fnode all sosed is F");
										//system("pause");
										//printf("i8 is Dirichlet node\n");
										this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
										this_is_C_node[i8] = true;
										bweSholdbeContinue = true;
										iadditionalCstatistic++;
										//exit(1);
										// ����� ����� �������� �������������.
									}
									else {

										// ��������������� ������� �� ��������� � �����.

										// ������ ������� ������ ����������� ��������� 
										// ������������� ��������� �������� �� �������� 
										// �������� �������.
										// ����������� 5 ������� 2015.
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
											P[icount1].j = i8;
											P[icount1].i = C_numerate[Amat.j[is0]];
											//P[icount1].aij = fabs(Amat.aij[is0]) / SumPall;
											P[icount1].aij = fabs(Amat.aij[is0]);
											sum_ok += fabs(Amat.aij[is0]);
											//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
											icount1++;
											if (icount1 >= nsizePR*n) {
												printf("memory error!!!\n");
												printf("not enough memory for the interpolation operator.\n");
												//system("PAUSE");
												//exit(1);
												deallocate_prolongation(nsizePR, n,  P);
											}
										}

									}

								}
								else
									if (this_is_F_node[Amat.j[is0]] == true) {

										// ������������� Strong F �����.
										// �� ������������� ��������� Strong C ������������ �� ���� ������� Strong F ������������.
										// ����� :
										//



										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
										// � ������� F ������.
										if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {

											icounter_bbackarr++;

											integer iFpoint = Amat.j[is0];
											//doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
											doublereal mult1 = fabs(Amat.aij[is0]);
											// ��������� ���� ������� iFpoint
											// ����� ����� ����� ��� � ����.

											// �������������� ��������� �����.
											doublereal maxelem_threshold_loc = -1.0;
											//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
											integer ii1_loc = row_startA[iFpoint];
											for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
												if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
													if (this_is_C_node[Amat.j[is0_loc]] == true) {
														if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc) {
															maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
														}
													}
												}
											}


											// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

											// ����� ������� ������� F-node ������� C-node.
											integer icsos_loc = 0;

											// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
											// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
											doublereal sumP_loc = 0.0;
											for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
												if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {




													if (this_is_C_node[Amat.j[is0_loc]] == true) {
														//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
														//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
														if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic) {
															sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
															icsos_loc++;
														}
													}
													else {
														// ������������ ���������� ������� ������� �� �������� � ������.
														//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������
													}
												}
											}

											icsos_loc = 0;
											// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
											// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
											doublereal sumP_loc1 = 0.0;
											for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
												if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {




													if (this_is_C_node[Amat.j[is0_loc]] == true) {
														//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
														//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
														if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic) {
															//	if ((mult1*fabs(Amat.aij[is0_loc]) / sumP_loc)>maxelem_threshold*theta) {
															sumP_loc1 += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
															icsos_loc++;
															//	}
														}
													}
													else {
														// ������������ ���������� ������� ������� �� �������� � ������.
														//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������
													}
												}
											}

											if (icsos_loc > 0) {
												// � ����� ��� ������� ����������������� �����
												for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
													if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
														if (this_is_C_node[Amat.j[is0_loc]] == true) {
															// ��������������� ������� �� ��������� � �����.

															// ������ ������� ������ ����������� ���������
															// ������������� ��������� �������� �� ��������
															// �������� �������.
															// ����������� 5 ������� 2015.
															//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
															//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
															if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic) {
																//	if ((mult1*fabs(Amat.aij[is0_loc]) / sumP_loc) > maxelem_threshold*theta) {
																P[icount1].j = i8;
																P[icount1].i = C_numerate[Amat.j[is0_loc]];
																//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
																//P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc1;
																P[icount1].aij = mult1*fabs(Amat.aij[is0_loc]) / sumP_loc1;
																sum_ok += mult1*fabs(Amat.aij[is0_loc]) / sumP_loc1;
																icount1++;
																if (icount1 >= nsizePR*n) {
																	printf("memory error!!!\n");
																	printf("not enough memory for the interpolation operator.\n");
																	//system("PAUSE");
																	//exit(1);
																	deallocate_prolongation(nsizePR, n,  P);
																}
															}
															//}
														}
													}
												}
											}
											else {
												//bbackreturn = true;
												bbackrarr[icounter_bbackarr] = true;
											}


											//}
										}
									}


							}
						}


						//bbackreturn = true;
						//for (integer i_63 = 0; i_63 < icount_StronglyF; i_63++) {
						//if (!bbackrarr[i_63]) {
						//	bbackreturn = false;
						//}
						//}

						//if (bbackreturn) {
						// ������� F ����� �� ���� ����������, ������� �� ������������ � ��������� 
						// �������� ������������.
						//for (integer i_95 = icount1_mem; i_95 < icount1; i_95++) {
						//P[i_95].aij = P[i_95].aij*SumPall / sumP;
						//}
						//}

						// ������������� ��������
						for (integer i_95 = icount1_mem; i_95 < icount1; i_95++) {
							P[i_95].aij = P[i_95].aij / sum_ok;
						}

						free(bbackrarr);

					}

				}

			}

			////
		}
		else {
			// only negative connections

			// ���������� ����������� ��� ����� ������� ���������� F nodes.
			// ������ F-nodes ������ C-nodes.
			for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {

				// ��� ����� ����������� �������� ��������.
				// 5 ������� 2015 ���� �� ��������� ��������� �������������
				// ��������� ������������� � ������ � ��������� ��������.
				doublereal maxelem_threshold = -1.0;
				//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
				integer ii1 = row_startA[i8];
				for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
					if (Amat.j[is0] != Amat.i[ii1]) {
						// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
						//if (this_is_C_node[Amat.j[is0]] == true) {
						if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold)) {
							maxelem_threshold = fabs(Amat.aij[is0]);
						}
						//}
					}
				}
				// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

				// ����� ������� ������� F-node ������� C-node.
				integer icsos = 0;
				integer icsosF = 0;

				// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
				// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
				doublereal sumP = 0.0;
				doublereal SumPall = 0.0;
				integer icount_StronglyF = 0;
				for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
					if (Amat.j[is0] != Amat.i[ii1]) {
						if (this_is_C_node[Amat.j[is0]] == true) {
							//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
							if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
								sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
								icsos++;
							}
						}
						else {
							if (this_is_F_node[Amat.j[is0]] == true) {
								if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
									SumPall += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
									icount_StronglyF++;
									icsosF++;
								}
							}
							// ������������ ���������� ������� ������� �� �������� � ������.
							the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
						}
					}
				}
				if (icsos == 1) {
					number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ �������  � �������.
																		   // ��������� ������ ������ "����������".
																		   // ���������� ������ ����������� ��� ���������.
																		   // � ������� �� �������� ������� ����� ���������� ������ ���������� ���� ��������� ������� � 
																		   // ������������� �� ���� ������� ����� ��������.
					if (icsosF == 0) number_of_F_nodes_with_one_single_strong_C_neighborF++; // ���������� F ����� � ����� ������������ ������� C ������� � � ����-�� �� ������� ������� F �������.
				}



				// 1 ������ 2016 ���� ����� ��� ������������.
				// ������� � ������ ������ ������ ������ ����� ���� � �����.
				if ((false) && (icsos == 1)) {
					this_is_F_node[i8] = false;
					this_is_C_node[i8] = true;
					bweSholdbeContinue = true;
				}
				else {

					if ((icsos == 1) && (icsosF != 0)) {
						// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
						//
						SumPall += sumP;

						for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
							if (Amat.j[is0] != Amat.i[ii1]) {
								// ��� ���������� ������ Strong �����.
								if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
									if (this_is_C_node[Amat.j[is0]] == true) {

										if (fabs(sumP) < RealZERO) {
											//printf("error interpolation zero diagonal sumP.\n");
											//printf("Fnode all sosed is F");
											//system("pause");
											//	printf("i8 is Dirichlet node\n");
											this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
											this_is_C_node[i8] = true;
											bweSholdbeContinue = true;
											iadditionalCstatistic++;
											//exit(1);
											// ����� ����� �������� �������������.
										}
										else {
											// ��� ��� ��� ������������ Strong C �����. 
											// ��������������� ������� �� ��������� � �����.

											// ������ ������� ������ ����������� ��������� 
											// ������������� ��������� �������� �� �������� 
											// �������� �������.
											// ����������� 5 ������� 2015.
											//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
											if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
												P[icount1].j = i8;
												P[icount1].i = C_numerate[Amat.j[is0]];
												//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
												P[icount1].aij = fabs(Amat.aij[is0]) / SumPall;
												icount1++;
												if (icount1 >= nsizePR*n) {
													printf("memory error!!!\n");
													printf("not enough memory for the interpolation operator.\n");
													//system("PAUSE");
													//exit(1);
													deallocate_prolongation(nsizePR, n,  P);
												}
											}

										}

									}
									else
										if (this_is_F_node[Amat.j[is0]] == true) {

											// ������������� Strong F �����.

											// ����� :
											// 



											//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
											// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
											// � ������� F ������.
											//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {

											integer iFpoint = Amat.j[is0];
											doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
											// ��������� ���� ������� iFpointeger 
											// ����� ����� ����� ��� � ����.

											// �������������� ��������� �����.
											doublereal maxelem_threshold_loc = -1.0;
											//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
											integer ii1_loc = row_startA[iFpoint];
											for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
												if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
													if (this_is_C_node[Amat.j[is0_loc]] == true) {
														if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc)) {
															maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
														}
													}
												}
											}


											// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

											// ����� ������� ������� F-node ������� C-node.
											integer icsos_loc = 0;

											// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
											// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
											doublereal sumP_loc = 0.0;
											for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
												if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {




													if (this_is_C_node[Amat.j[is0_loc]] == true) {
														//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
														//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
														if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic)) {
															sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
															icsos_loc++;
														}
													}
													else {
														// ������������ ���������� ������� ������� �� �������� � ������.
														//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
													}
												}
											}

											// � ����� ��� ������� ����������������� ����� 
											for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
												if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
													if (this_is_C_node[Amat.j[is0_loc]] == true) {
														// ��������������� ������� �� ��������� � �����.

														// ������ ������� ������ ����������� ��������� 
														// ������������� ��������� �������� �� �������� 
														// �������� �������.
														// ����������� 5 ������� 2015.
														//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
														//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
														if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic)) {
															P[icount1].j = i8;
															P[icount1].i = C_numerate[Amat.j[is0_loc]];
															//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
															P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
															icount1++;
															if (icount1 >= nsizePR*n) {
																printf("memory error!!!\n");
																printf("not enough memory for the interpolation operator.\n");
																//system("PAUSE");
																//exit(1);
																deallocate_prolongation(nsizePR, n,  P);
															}
														}
													}
												}
											}


											//}
										}
								}
							}
						}
					}
					else {

						SumPall += sumP;
						doublereal sum_ok = 0.0;
						bool bbackreturn = false;
						integer icount1_mem = icount1;
						bool* bbackrarr = NULL;
						//bbackrarr = new bool[icount_StronglyF];
						bbackrarr = (bool*)malloc(icount_StronglyF * sizeof(bool));
						char c1[27] = "classic_aglomerative_amg_4";
						char c2[10] = "bbackrarr";
						handle_error(bbackrarr, c2, c1, icount_StronglyF);

						for (integer i_63 = 0; i_63 < icount_StronglyF; i_63++) {
							bbackrarr[i_63] = false;
						}
						integer icounter_bbackarr = -1;

						for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
							if (Amat.j[is0] != Amat.i[ii1]) {

								if (this_is_C_node[Amat.j[is0]] == true) {

									if (fabs(sumP) < RealZERO) {
										//printf("error interpolation zero diagonal sumP.\n");
										//printf("Fnode all sosed is F");
										//system("pause");
										//printf("i8 is Dirichlet node\n");
										this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
										this_is_C_node[i8] = true;
										bweSholdbeContinue = true;
										iadditionalCstatistic++;
										//exit(1);
										// ����� ����� �������� �������������.
									}
									else {

										// ��������������� ������� �� ��������� � �����.

										// ������ ������� ������ ����������� ��������� 
										// ������������� ��������� �������� �� �������� 
										// �������� �������.
										// ����������� 5 ������� 2015.
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
											P[icount1].j = i8;
											P[icount1].i = C_numerate[Amat.j[is0]];
											//P[icount1].aij = fabs(Amat.aij[is0]) / SumPall;
											P[icount1].aij = fabs(Amat.aij[is0]);
											sum_ok += fabs(Amat.aij[is0]);
											//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
											icount1++;
											if (icount1 >= nsizePR*n) {
												printf("memory error!!!\n");
												printf("not enough memory for the interpolation operator.\n");
												//system("PAUSE");
												//exit(1);
												deallocate_prolongation(nsizePR, n,  P);
											}
										}

									}

								}
								else
									if (this_is_F_node[Amat.j[is0]] == true) {

										// ������������� Strong F �����.
										// �� ������������� ��������� Strong C ������������ �� ���� ������� Strong F ������������.
										// ����� :
										//



										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
										// � ������� F ������.
										if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {

											icounter_bbackarr++;

											integer iFpoint = Amat.j[is0];
											//doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
											doublereal mult1 = fabs(Amat.aij[is0]);
											// ��������� ���� ������� iFpoint
											// ����� ����� ����� ��� � ����.

											// �������������� ��������� �����.
											doublereal maxelem_threshold_loc = -1.0;
											//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
											integer ii1_loc = row_startA[iFpoint];
											for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
												if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
													if (this_is_C_node[Amat.j[is0_loc]] == true) {
														if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc)) {
															maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
														}
													}
												}
											}


											// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

											// ����� ������� ������� F-node ������� C-node.
											integer icsos_loc = 0;

											// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
											// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
											doublereal sumP_loc = 0.0;
											for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
												if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {




													if (this_is_C_node[Amat.j[is0_loc]] == true) {
														//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
														//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
														if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic)) {
															sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
															icsos_loc++;
														}
													}
													else {
														// ������������ ���������� ������� ������� �� �������� � ������.
														//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������
													}
												}
											}

											icsos_loc = 0;
											// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
											// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
											doublereal sumP_loc1 = 0.0;
											for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
												if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {




													if (this_is_C_node[Amat.j[is0_loc]] == true) {
														//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
														//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
														if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic)) {
															//	if ((mult1*fabs(Amat.aij[is0_loc]) / sumP_loc)>maxelem_threshold*theta) {
															sumP_loc1 += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
															icsos_loc++;
															//	}
														}
													}
													else {
														// ������������ ���������� ������� ������� �� �������� � ������.
														//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������
													}
												}
											}

											if (icsos_loc > 0) {
												// � ����� ��� ������� ����������������� �����
												for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
													if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
														if (this_is_C_node[Amat.j[is0_loc]] == true) {
															// ��������������� ������� �� ��������� � �����.

															// ������ ������� ������ ����������� ���������
															// ������������� ��������� �������� �� ��������
															// �������� �������.
															// ����������� 5 ������� 2015.
															//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
															//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
															if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic)) {
																//	if ((mult1*fabs(Amat.aij[is0_loc]) / sumP_loc) > maxelem_threshold*theta) {
																P[icount1].j = i8;
																P[icount1].i = C_numerate[Amat.j[is0_loc]];
																//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
																//P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc1;
																P[icount1].aij = mult1*fabs(Amat.aij[is0_loc]) / sumP_loc1;
																sum_ok += mult1*fabs(Amat.aij[is0_loc]) / sumP_loc1;
																icount1++;
																if (icount1 >= nsizePR*n) {
																	printf("memory error!!!\n");
																	printf("not enough memory for the interpolation operator.\n");
																	//system("PAUSE");
																	//exit(1);
																	deallocate_prolongation(nsizePR, n,  P);
																}
															}
															//}
														}
													}
												}
											}
											else {
												//bbackreturn = true;
												bbackrarr[icounter_bbackarr] = true;
											}


											//}
										}
									}


							}
						}


						//bbackreturn = true;
						//for (integer i_63 = 0; i_63 < icount_StronglyF; i_63++) {
						//if (!bbackrarr[i_63]) {
						//	bbackreturn = false;
						//}
						//}

						//if (bbackreturn) {
						// ������� F ����� �� ���� ����������, ������� �� ������������ � ��������� 
						// �������� ������������.
						//for (integer i_95 = icount1_mem; i_95 < icount1; i_95++) {
						//P[i_95].aij = P[i_95].aij*SumPall / sumP;
						//}
						//}

						// ������������� ��������
						for (integer i_95 = icount1_mem; i_95 < icount1; i_95++) {
							P[i_95].aij = P[i_95].aij / sum_ok;
						}

						free(bbackrarr);

					}

				}

			}


		} // end only negative connections 

	}

} // my_interpolation_procedure_number5


  // ���������������� ��������� �6.
void my_interpolation_procedure_number6(integer &the_number_of_neighbors_that_are_not_�_nodes,
	integer &number_of_F_nodes_with_one_single_strong_C_neighbor,
	integer* &n_a, bool* &this_is_F_node, integer* &row_startA,
	integer* &nnz_a, bool &bpositive_connections, Ak2 &Amat,
	bool &bweSholdbeContinue, bool* &this_is_C_node, integer &iadditionalCstatistic,
	const doublereal RealZERO, integer &icount1, Ak1* &P, integer &nsizePR, integer &ilevel,
	integer &iadd, doublereal &theta, integer &n,  integer* &C_numerate,
	integer &number_of_F_nodes_with_one_single_strong_C_neighborF,
	doublereal &theta83, bool &btreshold_on_new_vetv, integer& ifrom_re_operation_protection,
	bool &from_re_operation_protection0, doublereal &magic82, doublereal* &threshold_quick_all,
	doublereal* &threshold_quick_only_negative)
{
	// INTERPOLATION SIX

	// ����������������� ������������� 1 ������ 2016.
	printf("interpolation SIX: experimental version 1 jan 2016\n");
	system("PAUSE");

	the_number_of_neighbors_that_are_not_�_nodes = 0;
	number_of_F_nodes_with_one_single_strong_C_neighbor = 0;

	if (bpositive_connections) {

		// ���������� ����������� ��� ����� ������� ���������� F nodes.
		// ������ F-nodes ������ C-nodes.
		for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {

			// ��� ����� ����������� �������� ��������.
			// 5 ������� 2015 ���� �� ��������� ��������� �������������
			// ��������� ������������� � ������ � ��������� ��������.
			doublereal maxelem_threshold = -1.0;
			//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
			integer ii1 = row_startA[i8];
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					// ������� ������� ��� threshold �������� �� ���� ������, � �� ������ �� � ����� � ������.
					//if (this_is_C_node[Amat.j[is0]] == true) {
					if (fabs(Amat.aij[is0]) > maxelem_threshold) {
						maxelem_threshold = fabs(Amat.aij[is0]);
					}
					//}
				}
			}
			// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

			// ����� ������� ������� F-node ������� C-node.
			integer icsos = 0;

			doublereal SumPall = 0.0;
			// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
			// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
			doublereal sumP = 0.0;
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {

					// ����� ������ ���� ��������������� ���������.
					//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
					if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
						SumPall += fabs(Amat.aij[is0]);
					}


					if (this_is_C_node[Amat.j[is0]] == true) {
						//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
						if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
							sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
							icsos++;
						}
					}
					else {
						// ������������ ���������� ������� ������� �� �������� � ������.
						the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
					}
				}
			}
			if (icsos == 1) number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ �������  � �������.


																				   // 1 ������ 2015 ���� ����� ��� ������������.
																				   // ������� � ������ ������ ������ ������ ����� ���� � �����.
			if ((false) && (icsos == 1)) {
				this_is_F_node[i8] = false;
				this_is_C_node[i8] = true;
				bweSholdbeContinue = true;
			}
			else {



				for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
					if (Amat.j[is0] != Amat.i[ii1]) {
						if (this_is_C_node[Amat.j[is0]] == true) {

							if (fabs(sumP) < RealZERO) {
								//printf("error interpolation zero diagonal sumP.\n");
								//printf("Fnode all sosed is F");
								//system("pause");
								//	printf("i8 is Dirichlet node\n");
								this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
								this_is_C_node[i8] = true;
								bweSholdbeContinue = true;
								iadditionalCstatistic++;
								//exit(1);
								// ����� ����� �������� �������������.
							}
							else {

								// ��������������� ������� �� ��������� � �����.

								// ������ ������� ������ ����������� ��������� 
								// ������������� ��������� �������� �� �������� 
								// �������� �������.
								// ����������� 5 ������� 2015.
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
								if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
									P[icount1].j = i8;
									P[icount1].i = C_numerate[Amat.j[is0]];
									//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
									P[icount1].aij = fabs(Amat.aij[is0]) / SumPall;
									icount1++;
									if (icount1 >= nsizePR*n) {
										printf("memory error!!!\n");
										printf("not enough memory for the interpolation operator.\n");
										//system("PAUSE");
										//exit(1);
										deallocate_prolongation(nsizePR, n,  P);
									}
								}

							}

						}
						else
							if (this_is_F_node[Amat.j[is0]] == true) {

								// ����� :
								// 



								//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
								if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {

									integer iFpoint = Amat.j[is0];
									doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
									// ��������� ���� ������� iFpointeger 
									// ����� ����� ����� ��� � ����.

									// �������������� ��������� �����.
									doublereal maxelem_threshold_loc = -1.0;
									//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
									integer ii1_loc = row_startA[iFpoint];
									for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
											if (this_is_C_node[Amat.j[is0_loc]] == true) {
												if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc) {
													maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
												}
											}
										}
									}


									// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

									// ����� ������� ������� F-node ������� C-node.
									integer icsos_loc = 0;

									// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
									// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
									doublereal sumP_loc = 0.0;
									for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {




											if (this_is_C_node[Amat.j[is0_loc]] == true) {
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
												if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
													sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
													icsos_loc++;
												}
											}
											else {
												// ������������ ���������� ������� ������� �� �������� � ������.
												//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
											}
										}
									}

									// � ����� ��� ������� ����������������� ����� 
									for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
											if (this_is_C_node[Amat.j[is0_loc]] == true) {
												// ��������������� ������� �� ��������� � �����.

												// ������ ������� ������ ����������� ��������� 
												// ������������� ��������� �������� �� �������� 
												// �������� �������.
												// ����������� 5 ������� 2015.
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
												if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
													P[icount1].j = i8;
													P[icount1].i = C_numerate[Amat.j[is0_loc]];
													//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
													P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
													icount1++;
													if (icount1 >= nsizePR*n) {
														printf("memory error!!!\n");
														printf("not enough memory for the interpolation operator.\n");
														//system("PAUSE");
														//exit(1);
														deallocate_prolongation(nsizePR, n,  P);
													}
												}
											}
										}
									}


								}
							}
					}
				}

			}


		}
		////
	}
	else {
		// only negative connections

		// ���������� ����������� ��� ����� ������� ���������� F nodes.
		// ������ F-nodes ������ C-nodes.
		for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {

			// ��� ����� ����������� �������� ��������.
			// 5 ������� 2015 ���� �� ��������� ��������� �������������
			// ��������� ������������� � ������ � ��������� ��������.
			doublereal maxelem_threshold = -1.0;
			//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
			integer ii1 = row_startA[i8];
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					// ������� ������� ��� threshold �������� �� ���� ������, � �� ������ �� � ����� � ������.
					//if (this_is_C_node[Amat.j[is0]] == true) {
					if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold)) {
						maxelem_threshold = fabs(Amat.aij[is0]);
					}
					//}
				}
			}
			// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

			// ����� ������� ������� F-node ������� C-node.
			integer icsos = 0;

			doublereal SumPall = 0.0;
			// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
			// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
			doublereal sumP = 0.0;
			for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {

					// ����� ������ ���� ��������������� ���������.
					//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
					if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
						SumPall += fabs(Amat.aij[is0]);
					}


					if (this_is_C_node[Amat.j[is0]] == true) {
						//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
						if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
							sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
							icsos++;
						}
					}
					else {
						// ������������ ���������� ������� ������� �� �������� � ������.
						the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
					}
				}
			}
			if (icsos == 1) number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ ������� � �������.


																				   // 1 ������ 2015 ���� ����� ��� ������������.
																				   // ������� � ������ ������ ������ ������ ����� ���� � �����.
			if ((false) && (icsos == 1)) {
				this_is_F_node[i8] = false;
				this_is_C_node[i8] = true;
				bweSholdbeContinue = true;
			}
			else {



				for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
					if (Amat.j[is0] != Amat.i[ii1]) {
						if (this_is_C_node[Amat.j[is0]] == true) {

							if (fabs(sumP) < RealZERO) {
								//printf("error interpolation zero diagonal sumP.\n");
								//printf("Fnode all sosed is F");
								//system("pause");
								//	printf("i8 is Dirichlet node\n");
								this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
								this_is_C_node[i8] = true;
								bweSholdbeContinue = true;
								iadditionalCstatistic++;
								//exit(1);
								// ����� ����� �������� �������������.
							}
							else {

								// ��������������� ������� �� ��������� � �����.

								// ������ ������� ������ ����������� ��������� 
								// ������������� ��������� �������� �� �������� 
								// �������� �������.
								// ����������� 5 ������� 2015.
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
								if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {
									P[icount1].j = i8;
									P[icount1].i = C_numerate[Amat.j[is0]];
									//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
									P[icount1].aij = fabs(Amat.aij[is0]) / SumPall;
									icount1++;
									if (icount1 >= nsizePR*n) {
										printf("memory error!!!\n");
										printf("not enough memory for the interpolation operator.\n");
										//system("PAUSE");
										//exit(1);
										deallocate_prolongation(nsizePR, n,  P);
									}
								}

							}

						}
						else
							if (this_is_F_node[Amat.j[is0]] == true) {

								// ����� :
								// 



								//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
								if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold*theta)) {

									integer iFpoint = Amat.j[is0];
									doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
									// ��������� ���� ������� iFpointeger 
									// ����� ����� ����� ��� � ����.

									// �������������� ��������� �����.
									doublereal maxelem_threshold_loc = -1.0;
									//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
									integer ii1_loc = row_startA[iFpoint];
									for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
											if (this_is_C_node[Amat.j[is0_loc]] == true) {
												if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc)) {
													maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
												}
											}
										}
									}


									// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

									// ����� ������� ������� F-node ������� C-node.
									integer icsos_loc = 0;

									// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
									// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
									doublereal sumP_loc = 0.0;
									for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {




											if (this_is_C_node[Amat.j[is0_loc]] == true) {
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
												if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta)) {
													sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
													icsos_loc++;
												}
											}
											else {
												// ������������ ���������� ������� ������� �� �������� � ������.
												//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
											}
										}
									}

									// � ����� ��� ������� ����������������� ����� 
									for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										if (Amat.j[is0_loc] != Amat.i[ii1_loc]) {
											if (this_is_C_node[Amat.j[is0_loc]] == true) {
												// ��������������� ������� �� ��������� � �����.

												// ������ ������� ������ ����������� ��������� 
												// ������������� ��������� �������� �� �������� 
												// �������� �������.
												// ����������� 5 ������� 2015.
												//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
												if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta)) {
													P[icount1].j = i8;
													P[icount1].i = C_numerate[Amat.j[is0_loc]];
													//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
													P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
													icount1++;
													if (icount1 >= nsizePR*n) {
														printf("memory error!!!\n");
														printf("not enough memory for the interpolation operator.\n");
														//system("PAUSE");
														//exit(1);
														deallocate_prolongation(nsizePR, n,  P);
													}
												}
											}
										}
									}


								}
							}
					}
				}

			}


		}

	} // end only negative connections


} // my_interpolation_procedure_number6

  // ���������������� ��������� �1.
void my_interpolation_procedure_number1(integer &the_number_of_neighbors_that_are_not_�_nodes,
	integer &number_of_F_nodes_with_one_single_strong_C_neighbor,
	integer* &n_a, bool* &this_is_F_node, integer* &row_startA,
	integer* &nnz_a, bool &bpositive_connections, Ak2 &Amat,
	bool &bweSholdbeContinue, bool* &this_is_C_node, integer &iadditionalCstatistic,
	const doublereal RealZERO, integer &icount1, Ak1* &P, integer &nsizePR, integer &ilevel,
	integer &iadd, doublereal &theta, integer &n, Ak1* &R, integer* &C_numerate,
	integer &number_of_F_nodes_with_one_single_strong_C_neighborF,
	doublereal &theta83, bool &btreshold_on_new_vetv, integer& ifrom_re_operation_protection,
	bool &from_re_operation_protection0, doublereal &magic82, doublereal* &threshold_quick_all,
	doublereal* &threshold_quick_only_negative)
{

	// 6 december 2016.
// �������� �������� Refactoring.
	integer itypemodifyinterpol = 0; // ����� ����������� �������������. // �������� �������� Refactoring.
	integer inumberadaptpass = 0; // ������������ ���������� ������-�������� � �������������. // �������� �������� Refactoring.


	// 1.04.2017
	// ������� ���� � ��� ����� ��������� ������������ �� ������,
	// �������� ������������� ������������ � �������� �������������,
	// � ����� ����������� � �� � ��.


	//theta = 0.24;
	// theta_strong_F iter_number time,s
	// 0.21 56 22.63
	// 0.22 55 21.769
	// 0.23 52 21.488
	// 0.24 52 21.741 == theta // optimum
	// 0.26 69 24.623
	doublereal theta_strong_F = 0.23; // ����������� �����.
	theta_strong_F = theta83; // 3 ���� 2016


							  // �������� ������ ������������.
	integer ioneStrongC_and_0_StrongF = 0;

	// ����� ���� F �� ������� Strong � ������� ��� ���������� � �����.
	// ���� F ������� ������ Strong  � ������ �������������� � ������� ������� � ������� 
	// ������� F �����.

	//6interpolation 0.4 6.77 11 26 28.355
	//6interpolation 0.45 6.6 10 27 28.151
	//6interpolation 0.5 6.42 12 32 28.735
	//4interpolation 0.4 3.7  52 24.736 // best
	//4interpolation 0.3 3.78 13 59 27.525
	//4interpolation 0.5 3.61 12 55 25.533
	//4interpolation 0.45 3.65 10 63 30.24

	// the begining

	bool byes_add = false;
	// ������� ���������� ����������� � �����.
	if (1) {
		// � ���������� 0.4 ��������� ������������ �������� ����� ������� ������.
		//doublerealT magic = 0.4; // 0.4 optimum


		the_number_of_neighbors_that_are_not_�_nodes = 0;
		number_of_F_nodes_with_one_single_strong_C_neighbor = 0;



		integer i8_candidate_count = 0;
		integer* i8_candidate_arr = NULL;
		i8_candidate_arr = (integer*)malloc((n_a[ilevel - 1] + 1) * sizeof(integer));
		char c1[26] = "classic_aglomerative_amg4";
		char c2[17] = "i8_candidate_arr";
		char c3[21] = "i8_candidate_val_arr";
		handle_error(i8_candidate_arr, c2, c1, (n_a[ilevel - 1] + 1));
		doublereal* i8_candidate_val_arr = NULL;
		i8_candidate_val_arr = (doublereal*)malloc((n_a[ilevel - 1] + 1) * sizeof(doublereal));
		handle_error(i8_candidate_val_arr, c3, c1, (n_a[ilevel - 1] + 1));

		// ���������� ����������� ��� ����� ������� ���������� F nodes.
		// ������ F-nodes ������ C-nodes.
		for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {

#if doubleintprecision == 1
			//printf("i8=%lld n=%lld\n", i8, n_a[ilevel - 1]);
#else
			//printf("i8=%d n=%d\n", i8, n_a[ilevel - 1]);
#endif

			//getchar();


			// ��� ����� ����������� �������� ��������.
			// 5 ������� 2015 ���� �� ��������� ��������� �������������
			// ��������� ������������� � ������ � ��������� ��������.
			doublereal maxelem_threshold = -1.0;
			//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
			integer ii1 = row_startA[i8];
			//for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
			integer iend_marker_position = row_startA[Amat.i[ii1] + 1] - 1;
			if (!btreshold_on_new_vetv) {
				for (integer is0 = ii1; (is0 <= iend_marker_position); is0++) {
					if (Amat.j[is0] != Amat.i[ii1]) {
						// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
						//if (this_is_C_node[Amat.j[is0]] == true) {
						if (fabs(Amat.aij[is0]) > maxelem_threshold) {
							maxelem_threshold = fabs(Amat.aij[is0]);
						}
						//}
					}
				}
			}
			else {
				maxelem_threshold = threshold_quick_all[Amat.i[ii1]];
			}
			// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

			// ����� ������� ������� F-node ������� C-node.
			integer icsos = 0;
			integer icsos_pos = 0;
			integer icsosF = 0;
			integer icsosF_pos = 0;

			// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
			// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
			doublereal sumP = 0.0;
			doublereal sumP_pos = 0.0;
			doublereal SumPall = 0.0;
			doublereal SumPall_pos = 0.0;
			integer icount_StronglyF = 0;
			integer icount_StronglyF_pos = 0;

			doublereal maxelem_threshold_theta = maxelem_threshold*theta;
			doublereal maxelem_threshold_theta_strong_F = maxelem_threshold*theta_strong_F;
			for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					if (this_is_C_node[Amat.j[is0]] == true) {
						//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
						//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
						if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta)) {
							sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
							icsos++;
						}
						if ((Amat.aij[is0]>0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta)) {
							sumP_pos += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
							icsos_pos++;
						}
					}
					else {
						if (this_is_F_node[Amat.j[is0]] == true) {
							//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta_strong_F) {
							if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_strong_F)) {
								SumPall += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
								icount_StronglyF++;
								icsosF++;
							}
							if ((Amat.aij[is0]>0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_strong_F)) {
								SumPall_pos += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
								icount_StronglyF_pos++;
								icsosF_pos++;
							}
						}
						// ������������ ���������� ������� ������� �� �������� � ������.
						the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
					}
				}
			}
			if (icsos == 1) {
				number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ ������� � �������.
																	   // ��������� ������ ������ "����������".
																	   // ���������� ������ ����������� ��� ���������.
																	   // � ������� �� �������� ������� ����� ���������� ������ ���������� ���� ��������� ������� � 
																	   // ������������� �� ���� ������� ����� ��������.
				if (icsosF == 0) number_of_F_nodes_with_one_single_strong_C_neighborF++; // ���������� F ����� � ����� ������������ �������  C ������� � � ����-�� �� ������� ������� F �������.
			}



			// 1 ������ 2016 ���� ����� ��� ������������.
			// ������� � ������ ������ ������ ������ ����� ���� � �����.
			{

				if ((icsos == 1) && (icsosF == 0)) {



					// ����������� ����������.
					if (itypemodifyinterpol > 0) {



						if (itypemodifyinterpol > 1) {
							// ���������� ������� ��������� �� � ����.
							i8_candidate_arr[i8_candidate_count] = i8;
							// ���������� ���� ��������������� �����, 
							// ����� ����� ������� �������� �������.
							i8_candidate_val_arr[i8_candidate_count] = sumP;
							i8_candidate_count++;
						}

						if (itypemodifyinterpol == 1) {
							// ��������� ������ � ��� ������ ���� ���������� �������� ������ ������������.
							if (ifrom_re_operation_protection < inumberadaptpass) {
								// 3 december 2016 rundom
								// �� ����� ����� ��������� ���������� �� ������ ������.
								integer id_size_873 = (integer)(n_a[ilevel - 1] / 100);
								integer id_873 = rand() % id_size_873 + 1;
								if (id_873 == 1) {
									// � ������ ���� ������ �������� � ������ � ���������� ������� F �������
									// ����� ���� ���������� � �����. �.�. � ����������� ����� ���� ���������� � �����.
									// ����������� 2 ������� 2016 ����.
									this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
									this_is_C_node[i8] = true;
									bweSholdbeContinue = true;
									iadditionalCstatistic++;
									byes_add = true; // ���� ���������� �����.
								}
							}
						}
					}
				}
				else if ((icsos == 1) && (icsosF != 0)) {
					// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
					//
					//SumPall += sumP;

					for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							// ��� ���������� ������ Strong �����.
							//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
							if (fabs(Amat.aij[is0]) > maxelem_threshold_theta) {
								if (this_is_C_node[Amat.j[is0]] == true) {

									// 20 ������ 2016.
									// ��-����� ���� �������� ������ ���� �� ����� ��� ��� � ��� 
									// �������� ���� � �����.
									if ((fabs(sumP) < RealZERO) && (fabs(sumP_pos) < RealZERO)) {
										//printf("error interpolation zero diagonal sumP.\n");
										//printf("Fnode all sosed is F");
										//system("pause");
										//	printf("i8 is Dirichlet node\n");
										this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
										this_is_C_node[i8] = true;
										bweSholdbeContinue = true;
										iadditionalCstatistic++;
										byes_add = true; // ���� ���������� �����.
														 //exit(1);
														 // ����� ����� �������� �������������.
									}


								}

							}
						}
					}
				}
				else {

					integer iend_marker_position = row_startA[Amat.i[ii1] + 1] - 1;
					for (integer is0 = ii1; (is0 <= iend_marker_position); is0++) {
						if (this_is_C_node[Amat.j[is0]] == true) {
							if (Amat.j[is0] != Amat.i[ii1]) {


								// 20 jan 2016.
								// ����� �� ��������� ���� � ������ ���� ���� �� ����� ��� ������� F �������.


								if ((fabs(sumP) < RealZERO) && (fabs(sumP_pos) < RealZERO)) {
									// ��� ������ ����� ������ ��� ������� � �������.


									//printf("error interpolation zero diagonal sumP.\n");
									//printf("Fnode all sosed is F");
									//system("pause");
									//printf("i8 is Dirichlet node\n");
									this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
									this_is_C_node[i8] = true;
									bweSholdbeContinue = true;
									iadditionalCstatistic++;
									byes_add = true; // ���� ���������� �����.
													 //exit(1);
													 // ����� ����� �������� �������������.
								}


							}
						}
					}
				}

			}

		}
		/////
		if (itypemodifyinterpol > 0) {

			if (i8_candidate_arr != NULL) {
				i8_candidate_arr = (integer*)realloc(i8_candidate_arr, (i8_candidate_count + 2) * sizeof(integer));
			}
			if (i8_candidate_val_arr != NULL) {
				i8_candidate_val_arr = (doublereal*)realloc(i8_candidate_val_arr, (i8_candidate_count + 2) * sizeof(doublereal));
			}


			// �������� integer i_gold_i8 �������� �������.
			integer i_gold_i8 = i_my_min(i8_candidate_count/20, 100);// 100
			if (i8_candidate_count < 100) {
				i_gold_i8 = i8_candidate_count - 3;
			}
			if (1 && from_re_operation_protection0) {
				// ���������� �� �����������.
				// ���������� ������� �. ����� ������������ �� ������ my_nested_dissection.cpp
				QuickSortnd(i8_candidate_arr, i8_candidate_val_arr, 0, i8_candidate_count - 1);

				if (itypemodifyinterpol == 2) {
					// minimum
					//for (integer i_9 = i8_candidate_count - 1; i_9 >= i8_candidate_count - 1 - i_gold_i8; i_9--) {
					for (integer i_9 = 0; i_9 < i_gold_i8; i_9++) {
						// � ������ ���� ������ �������� � ������ � ���������� ������� F �������
						// ����� ���� ���������� � �����. �.�. � ����������� ����� ���� ���������� � �����.
						// ����������� 2 ������� 2016 ����.
						this_is_F_node[i8_candidate_arr[i_9]] = false; // ���� ���� ������� ������ � �����.
						this_is_C_node[i8_candidate_arr[i_9]] = true;
						bweSholdbeContinue = true;
						iadditionalCstatistic++;
						byes_add = true; // ���� ���������� �����.
					}
				}
				if (itypemodifyinterpol == 3) {
					// maximum
					for (integer i_9 = i8_candidate_count - 1; i_9 >= i8_candidate_count - 1 - i_gold_i8; i_9--) {
						//for (integer i_9 = 0; i_9 < i_gold_i8; i_9++) {
						// � ������ ���� ������ �������� � ������ � ���������� ������� F �������
						// ����� ���� ���������� � �����. �.�. � ����������� ����� ���� ���������� � �����.
						// ����������� 2 ������� 2016 ����.
						this_is_F_node[i8_candidate_arr[i_9]] = false; // ���� ���� ������� ������ � �����.
						this_is_C_node[i8_candidate_arr[i_9]] = true;
						bweSholdbeContinue = true;
						iadditionalCstatistic++;
						byes_add = true; // ���� ���������� �����.
					}
				}

			}

			ifrom_re_operation_protection++;
			if (ifrom_re_operation_protection > inumberadaptpass) {
				from_re_operation_protection0 = false;
			}


		}

		if (i8_candidate_arr != NULL) {
			free(i8_candidate_arr);
			i8_candidate_arr = NULL;
		}
		if (i8_candidate_val_arr != NULL) {
			free(i8_candidate_val_arr);
			i8_candidate_val_arr = NULL;
		}



	}



	if (!byes_add) {

		// � ���������� 0.4 ��������� ������������ �������� ����� ������� ������.
		doublereal magic = 0.4; // 0.4 optimum
								//magic = 0.3; // 3 ���� 2016 ��� ������������ �����
								// �������� ������� �� ���� ���������
								// �� �� ������������� �� �� ����� V ������.
								//magic = 0.5 - 0.2*ilevel / 12.0;
		magic = magic82;



		the_number_of_neighbors_that_are_not_�_nodes = 0;
		number_of_F_nodes_with_one_single_strong_C_neighbor = 0;



		// ���������� ����������� ��� ����� ������� ���������� F nodes.
		// ������ F-nodes ������ C-nodes.
		for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {

			// ��� ����� ����������� �������� ��������.
			// 5 ������� 2015 ���� �� ��������� ��������� �������������
			// ��������� ������������� � ������ � ��������� ��������.
			doublereal maxelem_threshold = -1.0;
			//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
			integer ii1 = row_startA[i8];
			integer istr_etalon1 = Amat.i[ii1];
			integer iend_for1 = -1;
			if (!btreshold_on_new_vetv) {
				for (integer is0 = ii1; (is0 <= row_startA[istr_etalon1 + 1] - 1); is0++) {
					iend_for1 = is0;
					if (Amat.j[is0] != istr_etalon1) {
						// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
						//if (this_is_C_node[Amat.j[is0]] == true) {
						if (fabs(Amat.aij[is0]) > maxelem_threshold) {
							maxelem_threshold = fabs(Amat.aij[is0]);
						}
						//}
					}
				}
			}
			else {
				for (integer is0 = ii1; (is0 <= row_startA[istr_etalon1 + 1] - 1); is0++) {
					iend_for1 = is0;
				}
				maxelem_threshold = threshold_quick_all[istr_etalon1];
			}
			// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

			// ����� ������� ������� F-node ������� C-node.
			integer icsos = 0;
			integer icsosF = 0;
			integer icsos_pos = 0;
			integer icsosF_pos = 0;

			doublereal maxelem_threshold_theta = maxelem_threshold*theta;
			doublereal maxelem_threshold_theta_strong_F = maxelem_threshold*theta_strong_F;


			// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
			// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
			doublereal sumP = 0.0;
			doublereal SumPall = 0.0;
			integer icount_StronglyF = 0;
			doublereal sumP_pos = 0.0;
			doublereal SumPall_pos = 0.0;
			integer icount_StronglyF_pos = 0;

			//	for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
			for (integer is0 = ii1; is0 <= iend_for1; is0++) {
				if (Amat.j[is0] != istr_etalon1) {
					if (this_is_C_node[Amat.j[is0]] == true) {
						//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
						//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
						if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta)) {
							sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
							icsos++;
						}
						if ((Amat.aij[is0]>0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta)) {
							sumP_pos += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
							icsos_pos++;
						}
					}
					else {
						if (this_is_F_node[Amat.j[is0]] == true) {
							//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta_strong_F) {
							if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_strong_F)) {
								SumPall += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
								icount_StronglyF++;
								icsosF++;
							}
							if ((Amat.aij[is0] > 0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_strong_F)) {
								SumPall_pos += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
								icount_StronglyF_pos++;
								icsosF_pos++;
							}
						}
						// ������������ ���������� ������� ������� �� �������� � ������.
						the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
					}
				}
			}
			if (icsos == 1) {
				number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ �������  � �������.
																	   // ��������� ������ ������ "����������".
																	   // ���������� ������ ����������� ��� ���������.
																	   // � ������� �� �������� ������� ����� ���������� ������ ���������� ���� ��������� ������� � 
																	   // ������������� �� ���� ������� ����� ��������.
				if (icsosF == 0) number_of_F_nodes_with_one_single_strong_C_neighborF++; // ���������� F ����� � ����� ������������ �������  C ������� � � ����-�� �� ������� ������� F �������.
			}



			// 1 ������ 2016 ���� ����� ��� ������������.
			// ������� � ������ ������ ������ ������ ����� ���� � �����.
			if ((false) && (icsos == 1)) {
				this_is_F_node[i8] = false;
				this_is_C_node[i8] = true;
				bweSholdbeContinue = true;
			}
			else {

				if (((icsos == 1) || (icsos == 2) || (icsos == 3)) && (icsosF != 0)) {
					//if (((icsos == 1) || (icsos == 2) || (icsos == 3) || (icsos >= 4)) && (icsosF != 0)) {
					// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
					//
					SumPall += sumP;
					SumPall_pos += sumP_pos;


					for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							// ��� ���������� ������ Strong �����.


							if (this_is_C_node[Amat.j[is0]] == true) {
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
								if (fabs(Amat.aij[is0]) > maxelem_threshold_theta) {
									if ((fabs(sumP) < RealZERO) && (fabs(sumP_pos) < RealZERO)) {
										//printf("error interpolation zero diagonal sumP.\n");
										//printf("Fnode all sosed is F");
										//system("pause");
										//	printf("i8 is Dirichlet node\n");
										this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
										this_is_C_node[i8] = true;
										bweSholdbeContinue = true;
										iadditionalCstatistic++;
										//exit(1);
										// ����� ����� �������� �������������.
									}
									else {
										// ��� ��� ��� ������������ Strong C �����. 
										// ��������������� ������� �� ��������� � �����.

										// ������ ������� ������ ����������� ��������� 
										// ������������� ��������� �������� �� �������� 
										// �������� �������.
										// ����������� 5 ������� 2015.
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
										if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta)) {
											P[icount1].j = i8;
											P[icount1].i = C_numerate[Amat.j[is0]];
											//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
											if (fabs(SumPall) < 1.0e-33) {
												printf("error 1.0 ! division by zero. SumPall =%e\n", SumPall);
												//getchar();
												system("PAUSE");
												exit(1);
											}
											P[icount1].aij = fabs(Amat.aij[is0]) / SumPall;
											icount1++;
											if (icount1 >= nsizePR*n) {
												printf("memory error!!!\n");
												printf("not enough memory for the interpolation operator.\n");
												//system("PAUSE");
												//exit(1);
												deallocate_prolongation(nsizePR, n,  P);
											}
										}
										if ((Amat.aij[is0]>0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta)) {
											P[icount1].j = i8;
											P[icount1].i = C_numerate[Amat.j[is0]];
											//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
											if (fabs(SumPall_pos) < 1.0e-33) {
												printf("error 1.0 ! division by zero. SumPall_pos =%e\n", SumPall_pos);
												//getchar();
												system("PAUSE");
												exit(1);
											}
											// �������� !!! ������ �����.
											P[icount1].aij = -fabs(Amat.aij[is0]) / SumPall_pos;
											icount1++;
											if (icount1 >= nsizePR*n) {
												printf("memory error!!!\n");
												printf("not enough memory for the interpolation operator.\n");
												//system("PAUSE");
												//exit(1);
												deallocate_prolongation(nsizePR, n,  P);
											}
										}


									}
								}

							}
							else
								if (this_is_F_node[Amat.j[is0]] == true) {
									//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta_strong_F) {
									if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_strong_F)) {
										// ������������� Strong F �����.

										// ����� :
										// 



										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
										// � ������� F ������.
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta_strong_F) {

										integer iFpoint = Amat.j[is0];
										if (fabs(SumPall) < 1.0e-33) {
											printf("error 2.0 ! division by zero. SumPall =%e\n", SumPall);
											//getchar();
											system("PAUSE");
											exit(1);
										}
										doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
										// ��������� ���� ������� iFpointeger 
										// ����� ����� ����� ��� � ����.

										// �������������� ��������� �����.
										doublereal maxelem_threshold_loc = -1.0;
										//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
										integer ii1_loc = row_startA[iFpoint];
										integer istr_etalon = Amat.i[ii1_loc];
										integer iend_for = -1;
										integer iend_marker_position = row_startA[istr_etalon + 1] - 1;
										for (integer is0_loc = ii1_loc; (is0_loc <= iend_marker_position); is0_loc++) {
											iend_for = is0_loc;
											if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													if (Amat.j[is0_loc] != istr_etalon) {
														maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
													}
												}
											}
										}

										doublereal maxelem_threshold_loc_magic = maxelem_threshold_loc*magic;
										// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

										// ����� ������� ������� F-node ������� C-node.
										integer icsos_loc = 0;

										// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
										// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
										doublereal sumP_loc = 0.0;
										//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

											// ����� �������� ����� ���������� ����� ���������.
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic) {
											if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc_magic)) {






												if (this_is_C_node[Amat.j[is0_loc]] == true) {

													if (Amat.j[is0_loc] != istr_etalon) {

														sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
														icsos_loc++;
													}

												}
												else {

													//if (Amat.j[is0_loc] != istr_etalon) {
													// ������������ ���������� ������� ������� �� �������� � ������.
													//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
													//}
												}

											}
										}

										doublereal maxelem_threshold_loc_magic_minus = -maxelem_threshold_loc_magic;

										// � ����� ��� ������� ����������������� ����� 
										//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

											// ��� �������������� ��������� � ������ ������� ���� �������� ���������� ����� ���������.


											// ��������������� ������� �� ��������� � �����.

											// ������ ������� ������ ����������� ��������� 
											// ������������� ��������� �������� �� �������� 
											// �������� �������.
											// ����������� 5 ������� 2015.
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic) {
											if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc_magic)) {
												//if ((Amat.aij[is0_loc]<maxelem_threshold_loc_magic_minus) || (Amat.aij[is0_loc]>maxelem_threshold_loc_magic)) {

												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													if (Amat.j[is0_loc] != istr_etalon) {

														P[icount1].j = i8;
														P[icount1].i = C_numerate[Amat.j[is0_loc]];
														//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
														if (fabs(sumP_loc) < 1.0e-33) {
															printf("error 3.0 ! division by zero. sumP_loc =%e\n", sumP_loc);
															//getchar();
															system("PAUSE");
															exit(1);
														}
														P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
														icount1++;
														if (icount1 >= nsizePR*n) {
															printf("memory error!!!\n");
															printf("not enough memory for the interpolation operator.\n");
															//system("PAUSE");
															//exit(1);
															deallocate_prolongation(nsizePR, n,  P);
														}

													}
												}
											}
										}


										//}
									}
									// todo
									if ((Amat.aij[is0]>0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_strong_F)) {
										// ������������� Strong F �����.

										// ����� :
										// 



										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
										// � ������� F ������.
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta_strong_F) {

										integer iFpoint = Amat.j[is0];
										if (fabs(SumPall_pos) < 1.0e-33) {
											printf("error 2.0 ! division by zero. SumPall_pos =%e\n", SumPall_pos);
											//getchar();
											system("PAUSE");
											exit(1);
										}
										// �������� �����.
										doublereal multiplyer_nu = -fabs(Amat.aij[is0]) / SumPall_pos;
										// ��������� ���� ������� iFpointeger 
										// ����� ����� ����� ��� � ����.

										// �������������� ��������� �����.
										doublereal maxelem_threshold_loc = -1.0;
										//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
										integer ii1_loc = row_startA[iFpoint];
										integer istr_etalon = Amat.i[ii1_loc];
										integer iend_for = -1;
										integer iend_marker_position = row_startA[istr_etalon + 1] - 1;
										for (integer is0_loc = ii1_loc; (is0_loc <= iend_marker_position); is0_loc++) {
											iend_for = is0_loc;
											if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													if (Amat.j[is0_loc] != istr_etalon) {
														maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
													}
												}
											}
										}

										doublereal maxelem_threshold_loc_magic = maxelem_threshold_loc*magic;
										// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

										// ����� ������� ������� F-node ������� C-node.
										integer icsos_loc = 0;

										// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
										// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
										doublereal sumP_loc = 0.0;
										//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

											// ����� �������� ����� ���������� ����� ���������.
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic) {
											if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc_magic)) {






												if (this_is_C_node[Amat.j[is0_loc]] == true) {

													if (Amat.j[is0_loc] != istr_etalon) {

														sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
														icsos_loc++;
													}

												}
												else {

													//if (Amat.j[is0_loc] != istr_etalon) {
													// ������������ ���������� ������� ������� �� �������� � ������.
													//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
													//}
												}

											}
										}

										doublereal maxelem_threshold_loc_magic_minus = -maxelem_threshold_loc_magic;

										// � ����� ��� ������� ����������������� ����� 
										//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

											// ��� �������������� ��������� � ������ ������� ���� �������� ���������� ����� ���������.


											// ��������������� ������� �� ��������� � �����.

											// ������ ������� ������ ����������� ��������� 
											// ������������� ��������� �������� �� �������� 
											// �������� �������.
											// ����������� 5 ������� 2015.
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic) {
											if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc_magic)) {
												//if ((Amat.aij[is0_loc]<maxelem_threshold_loc_magic_minus) || (Amat.aij[is0_loc]>maxelem_threshold_loc_magic)) {

												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													if (Amat.j[is0_loc] != istr_etalon) {

														P[icount1].j = i8;
														P[icount1].i = C_numerate[Amat.j[is0_loc]];
														//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
														if (fabs(sumP_loc) < 1.0e-33) {
															printf("error 3.0 ! division by zero. sumP_loc =%e\n", sumP_loc);
															//getchar();
															system("PAUSE");
															exit(1);
														}
														P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
														icount1++;
														if (icount1 >= nsizePR*n) {
															printf("memory error!!!\n");
															printf("not enough memory for the interpolation operator.\n");
															//system("PAUSE");
															//exit(1);
															deallocate_prolongation(nsizePR, n,  P);
														}

													}
												}
											}
										}


										//}
									}

								}
						}

					}
				}
				else {

					// �������� bmodify_interpol4_amg==true ��� magic2<0.24 �������� � ����������������� ���������
					// �������� ����������. ��� ���������� ����������� �� bmodify_interpol4_amg==true � magic2>=0.24 
					// �������� ������� �� �������� ����� �� ����������� ���������� ����� �� ������� ��� � ��� 
					// bmodify_interpol4_amg = false;
					// 22 01 2016 ����� �������� bmodify_interpol4_amg = false; � �� � ���� ������ �� ������.
					bool bmodify_interpol4_amg = false;
					// 01.04.2017 
					// bmodify_interpol4_amg = true ���������� �� �����������
					// � � ������ ������������.

					doublereal magic2 = 0.5;
					//magic2 = magic83;

					// ���������������� ���� � �������� ��������� ������������ 
					// �������� ������������ ���������� ��� ���������� ����� � �����.
					// �� ��������� ������������ �� ���� �������� ������� Weakly C ����� ���� �������
					// ������� �������.
					// ���������������� ���� ����������� � ����������� ��������� is0_candidate ����� �������
					// Weakly C ����� � ������ � ���������� ������������ ������� ����������� � sum1.
					doublereal sum1 = 0.0;
					integer is0_candidate = -1;
					if (bmodify_interpol4_amg && ((icsos == 1) && (icsosF == 0))) {
						for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
							if (Amat.j[is0] != Amat.i[ii1]) {
								if (this_is_C_node[Amat.j[is0]] == true) {
									//if (fabs(Amat.aij[is0]) <= maxelem_threshold*theta) {
									if (fabs(Amat.aij[is0]) <= maxelem_threshold_theta) {
										if (fabs(Amat.aij[is0]) > sum1) {
											sum1 = fabs(Amat.aij[is0]);
											is0_candidate = is0;
										}
									}
								}
							}
						}

						if (sum1 / sumP < magic2) {
							// ���� ��������� ������� ������� ������ ������ �� ������ � ���� �,
							// � ���� ������ �������� Weakly F ��������� ����� ������� ��� C ����������.
							sum1 = 0.0;
							is0_candidate = -1;

							// ����� ���� ����� Weakly F ����� ���������.
							for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
								if (Amat.j[is0] != Amat.i[ii1]) {
									if (this_is_F_node[Amat.j[is0]] == true) {
										//if (fabs(Amat.aij[is0]) <= maxelem_threshold*theta) {
										if (fabs(Amat.aij[is0]) <= maxelem_threshold_theta) {
											if (fabs(Amat.aij[is0]) > sum1) {
												sum1 = fabs(Amat.aij[is0]);
												is0_candidate = is0;
											}
										}
									}
								}
							}

							// ����� ���� F ������ ����� ����� � �����.
							if (sum1 / sumP >= magic2) {
								this_is_F_node[i8] = false; // ���� ����  ������ � �����.
								this_is_C_node[i8] = true;
								bweSholdbeContinue = true;
								byes_add = true;
								iadditionalCstatistic++;

							}
							// ������������ ����� ����� ��������� ����� �� ���� ������� �� ����.
							sum1 = 0.0;
							is0_candidate = -1;

						}
					}


					for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							if (this_is_C_node[Amat.j[is0]] == true) {



								if (bmodify_interpol4_amg && ((icsos == 1) && (icsosF == 0))) {
									// �� ��������� ���� ������������ ��� ���������� ����� ����� ��
									// ���� ����� �������������� ����� ������� Weakly C �����.
									// ����� ������� Weakly C ����� ����� ��������� sum1 � is0_candidate.

									ioneStrongC_and_0_StrongF++;
									if (is0_candidate != -1) {
										P[icount1].j = i8;
										P[icount1].i = C_numerate[Amat.j[is0_candidate]];
										P[icount1].aij = fabs(Amat.aij[is0_candidate]) / (sumP + sum1);
										icount1++;
										if (icount1 >= nsizePR*n) {
											printf("memory error!!!\n");
											printf("not enough memory for the interpolation operator.\n");
											//system("PAUSE");
											//exit(1);
											deallocate_prolongation(nsizePR, n,  P);
										}
									}

									//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
									if (fabs(Amat.aij[is0]) > maxelem_threshold_theta) {
										P[icount1].j = i8;
										P[icount1].i = C_numerate[Amat.j[is0]];
										P[icount1].aij = fabs(Amat.aij[is0]) / (sumP + sum1);
										icount1++;
										if (icount1 >= nsizePR*n) {
											printf("memory error!!!\n");
											printf("not enough memory for the interpolation operator.\n");
											//system("PAUSE");
											//exit(1);
											deallocate_prolongation(nsizePR, n,  P);
										}
									}
								}
								else {

									if ((fabs(sumP) < RealZERO) && (fabs(sumP_pos) < RealZERO)) {
										//printf("error interpolation zero diagonal sumP.\n");
										//printf("Fnode all sosed is F");
										//system("pause");
										//printf("i8 is Dirichlet node\n");
										this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
										this_is_C_node[i8] = true;
										bweSholdbeContinue = true;
										iadditionalCstatistic++;
										//exit(1);
										// ����� ����� �������� �������������.
									}
									else {

										// ������������� ������ �� ��������� � �����.

										// ��������������� ������� �� ��������� � �����.

										// ������ ������� ������ ����������� ��������� 
										// ������������� ��������� �������� �� �������� 
										// �������� �������.
										// ����������� 5 ������� 2015.
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										//	if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
										if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta)) {
											P[icount1].j = i8;
											P[icount1].i = C_numerate[Amat.j[is0]];
											if (fabs(sumP) < 1.0e-33) {
												printf("error 4.0 ! division by zero. sumP =%e\n", sumP);
												//getchar();
												system("PAUSE");
												exit(1);
											}
											P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
											icount1++;
											if (icount1 >= nsizePR*n) {
												printf("memory error!!!\n");
												printf("not enough memory for the interpolation operator.\n");
												//system("PAUSE");
												//exit(1);
												deallocate_prolongation(nsizePR, n,  P);
											}
										}

										if ((Amat.aij[is0]>0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta)) {
											P[icount1].j = i8;
											P[icount1].i = C_numerate[Amat.j[is0]];
											if (fabs(sumP_pos) < 1.0e-33) {
												printf("error 4.0 ! division by zero. sumP_pos =%e\n", sumP_pos);
												//getchar();
												system("PAUSE");
												exit(1);
											}
											P[icount1].aij = -fabs(Amat.aij[is0]) / sumP_pos;
											icount1++;
											if (icount1 >= nsizePR*n) {
												printf("memory error!!!\n");
												printf("not enough memory for the interpolation operator.\n");
												//system("PAUSE");
												//exit(1);
												deallocate_prolongation(nsizePR, n,  P);
											}
										}

									}

								}

							}
						}
					}
				}

			}
		}
		////

	}

#if doubleintprecision == 1
	//printf("one Strong C and 0 Strong F=%lld additional all %lld", ioneStrongC_and_0_StrongF, iadditionalCstatistic);
#else
	//printf("one Strong C and 0 Strong F=%d additional all %d", ioneStrongC_and_0_StrongF, iadditionalCstatistic);
#endif

	//system("pause");

} // my_interpolation_procedure_number1

  // ���������������� ��������� �0.
void my_interpolation_procedure_number0(integer &the_number_of_neighbors_that_are_not_�_nodes,
	integer &number_of_F_nodes_with_one_single_strong_C_neighbor,
	integer* &n_a, bool* &this_is_F_node, integer* &row_startA,
	integer* &nnz_a, bool &bpositive_connections, Ak2 &Amat,
	bool &bweSholdbeContinue, bool* &this_is_C_node, integer &iadditionalCstatistic,
	const doublereal RealZERO, integer &icount1, Ak1* &P, integer &nsizePR, integer &ilevel,
	integer &iadd, doublereal &theta, integer &n, Ak1* &R, integer* &C_numerate,
	integer &number_of_F_nodes_with_one_single_strong_C_neighborF,
	doublereal &theta83, bool &btreshold_on_new_vetv, integer& ifrom_re_operation_protection,
	bool &from_re_operation_protection0, doublereal &magic82, doublereal* &threshold_quick_all,
	doublereal* &threshold_quick_only_negative)
{

	// 6 december 2016.
// �������� �������� Refactoring.
	integer itypemodifyinterpol = 0; // ����� ����������� �������������. // �������� �������� Refactoring.
	integer inumberadaptpass = 0; // ������������ ���������� ������-�������� � �������������. // �������� �������� Refactoring.


	// 1.04.2017; 28.04.2017;
	// ������� ���� � ��� ����� ��������� ������������ �� ������, �������� ������������� ������������ � �������� �������������,
	// � ����� ����������� � �� � ��.


	//theta = 0.24;
	// theta_strong_F iter_number time,s
	// 0.21 56 22.63
	// 0.22 55 21.769
	// 0.23 52 21.488
	// 0.24 52 21.741 == theta // optimum
	// 0.26 69 24.623

	// PIONER RS2
	// theta_interpol magic iter_number residual #levels
	// 0.24 0.24  0.4 21 1.850831e7 13
	// 0.24 0.23 0.4 21 1.661238e+07 14 
	// 0.24 0.22 0.4 21 1.268147e+07 14 optimum
	// 0.24 0.21 0.4 20 1.086504e+07 14
	// 0.24 0.2 0.4 21 2.116516e+07 14
	// 0.25 0.2 0.4 21 1.756896e+07 13
	// 0.25 0.2 0.4 21 1.631888e+07 10 RS1
	// 0.25 0.2 0.4 21 1.928085e+07 11 RS1
	// 0.25 0.2 0.35 21 1.426122e+07 11 RS1
	// 0.25 0.2 0.35 21 1.342786e+07 10 RS1 0.5pos con
	// 0.25 0.2 0.4 21 1.148286e+07 11 RS1 0.5pos con
	// 0.25 0.22 0.4 22 1.430474e+07 11 RS1 0.5pos con
	// 0.25 0.25 0.4

	// 3 ���� 2016
	//doublereal theta_strong_C = 0.23;
	//doublereal theta_strong_F = 0.23; // ����������� �����.
	const doublereal theta_strong_F = theta83; // 0.23 is recomended
	const doublereal theta_strong_C = theta83; // 0.23 is recomended
	//theta_strong_F = 0.22; // 29 ������ 2017
	//theta_strong_C = 0.22;
	// �������� ������ ������������.
	integer ioneStrongC_and_0_StrongF = 0;

	// ����� ���� F �� ������� Strong � ������� ��� ���������� � �����.
	// ���� F ������� ������ Strong  � ������ �������������� � ������� ������� � ������� 
	// ������� F �����.

	//6interpolation 0.4 6.77 11 26 28.355
	//6interpolation 0.45 6.6 10 27 28.151
	//6interpolation 0.5 6.42 12 32 28.735
	//4interpolation 0.4 3.7  52 24.736 // best
	//4interpolation 0.3 3.78 13 59 27.525
	//4interpolation 0.5 3.61 12 55 25.533
	//4interpolation 0.45 3.65 10 63 30.24

	// the begining

	bool byes_add = false;
	// ������� ���������� ����������� � �����.
	if (1) {
		// � ���������� 0.4 ��������� ������������ �������� ����� ������� ������.
		//doublerealT magic = 0.4; // 0.4 optimum


		the_number_of_neighbors_that_are_not_�_nodes = 0;
		number_of_F_nodes_with_one_single_strong_C_neighbor = 0;



		integer i8_candidate_count = 0;
		integer* i8_candidate_arr = NULL;
		i8_candidate_arr = (integer*)malloc((n_a[ilevel - 1] + 1) * sizeof(integer));
		char c1[26] = "classic_aglomerative_amg4";
		char c2[17] = "i8_candidate_arr";
		char c3[21] = "i8_candidate_val_arr";
		handle_error(i8_candidate_arr, c2, c1, (n_a[ilevel - 1] + 1));
		doublereal* i8_candidate_val_arr = NULL;
		i8_candidate_val_arr = (doublereal*)malloc((n_a[ilevel - 1] + 1) * sizeof(doublereal));
		handle_error(i8_candidate_val_arr, c3, c1, (n_a[ilevel - 1] + 1));

		// ���������� ����������� ��� ����� ������� ���������� F nodes.
		// ������ F-nodes ������ C-nodes.
		for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {

#if doubleintprecision == 1
			//printf("i8=%lld n=%lld\n", i8, n_a[ilevel - 1]);
#else
			//printf("i8=%d n=%d\n", i8, n_a[ilevel - 1]);
#endif

			//getchar();


			// ��� ����� ����������� �������� ��������.
			// 5 ������� 2015 ���� �� ��������� ��������� �������������
			// ��������� ������������� � ������ � ��������� ��������.
			doublereal maxelem_threshold_plus = -1.0;
			doublereal maxelem_threshold_minus = -1.0;
			//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
			integer ii1 = row_startA[i8];
			//for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
			integer iend_marker_position = row_startA[Amat.i[ii1] + 1] - 1;
			for (integer is0 = ii1; (is0 <= iend_marker_position); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
					//if (this_is_C_node[Amat.j[is0]] == true) {
					if ((Amat.aij[is0]>0) && (fabs(Amat.aij[is0]) > maxelem_threshold_plus)) {
						maxelem_threshold_plus = fabs(Amat.aij[is0]);
					}
					if ((Amat.aij[is0]<0) && (fabs(Amat.aij[is0]) > maxelem_threshold_minus)) {
						maxelem_threshold_minus = fabs(Amat.aij[is0]);
					}
					//}
				}
			}
			// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

			// ����� ������� ������� F-node ������� C-node.
			integer icsos = 0;
			integer icsos_pos = 0;
			integer icsosF = 0;
			integer icsosF_pos = 0;

			// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
			// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
			doublereal sumP = 0.0;
			doublereal sumP_pos = 0.0;
			doublereal SumPall = 0.0;
			doublereal SumPall_pos = 0.0;
			integer icount_StronglyF = 0;
			integer icount_StronglyF_pos = 0;

			doublereal maxelem_threshold_theta_plus = maxelem_threshold_plus*theta_strong_C;
			doublereal maxelem_threshold_theta_strong_F_plus = maxelem_threshold_plus*theta_strong_F;
			// �����������.
			//doublerealT maxelem_threshold_theta_plus = maxelem_threshold_plus*0.5;
			//doublerealT maxelem_threshold_theta_strong_F_plus = maxelem_threshold_plus*0.5;
			doublereal maxelem_threshold_theta_minus = maxelem_threshold_minus*theta_strong_C;
			doublereal maxelem_threshold_theta_strong_F_minus = maxelem_threshold_minus*theta_strong_F;
			for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
				if (Amat.j[is0] != Amat.i[ii1]) {
					if (this_is_C_node[Amat.j[is0]] == true) {
						//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
						//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
						if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_minus)) {
							sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
							icsos++;
						}
						if ((Amat.aij[is0]>0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_plus)) {
							sumP_pos += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
							icsos_pos++;
						}
					}
					else {
						if (this_is_F_node[Amat.j[is0]] == true) {
							//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta_strong_F) {
							if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_strong_F_minus)) {
								SumPall += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
								icount_StronglyF++;
								icsosF++;
							}
							if ((Amat.aij[is0]>0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_strong_F_plus)) {
								SumPall_pos += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
								icount_StronglyF_pos++;
								icsosF_pos++;
							}
						}
						// ������������ ���������� ������� ������� �� �������� � ������.
						the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
					}
				}
			}
			if (icsos == 1) {
				number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ �������  � �������.
																	   // ��������� ������ ������ "����������".
																	   // ���������� ������ ����������� ��� ���������.
																	   // � ������� �� �������� ������� ����� ���������� ������ ���������� ���� ��������� ������� � 
																	   // ������������� �� ���� ������� ����� ��������.
				if (icsosF == 0) number_of_F_nodes_with_one_single_strong_C_neighborF++; // ���������� F ����� � ����� ������������ �������  C ������� � � ����-�� �� ������� ������� F �������.
			}



			// 1 ������ 2016 ���� ����� ��� ������������.
			// ������� � ������ ������ ������ ������ ����� ���� � �����.
			{

				if ((icsos == 1) && (icsosF == 0)) {



					// ����������� ����������.
					if (itypemodifyinterpol > 0) {



						if (itypemodifyinterpol > 1) {
							// ���������� ������� ��������� �� � ����.
							i8_candidate_arr[i8_candidate_count] = i8;
							// ���������� ���� ��������������� �����, 
							// ����� ����� ������� �������� �������.
							i8_candidate_val_arr[i8_candidate_count] = sumP;
							i8_candidate_count++;
						}

						if (itypemodifyinterpol == 1) {
							// ��������� ������ � ��� ������ ���� ���������� �������� ������ ������������.
							if (ifrom_re_operation_protection < inumberadaptpass) {
								// 3 december 2016 rundom
								// �� ����� ����� ��������� ���������� �� ������ ������.
								integer id_size_873 = (integer)(n_a[ilevel - 1] / 100);
								integer id_873 = rand() % id_size_873 + 1;
								if (id_873 == 1) {
									// � ������ ���� ������ �������� � ������ � ���������� ������� F �������
									// ����� ���� ���������� � �����. �.�. � ����������� ����� ���� ���������� � �����.
									// ����������� 2 ������� 2016 ����.
									this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
									this_is_C_node[i8] = true;
									bweSholdbeContinue = true;
									iadditionalCstatistic++;
									byes_add = true; // ���� ���������� �����.
								}
							}
						}
					}
				}
				else if ((icsos == 1) && (icsosF != 0)) {
					// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
					//
					//SumPall += sumP;

					for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							// ��� ���������� ������ Strong �����.
							//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
							if (((Amat.aij[is0]<0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_minus)) || (((Amat.aij[is0]>0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_plus)))) {
								if (this_is_C_node[Amat.j[is0]] == true) {

									// 20 ������ 2016.
									// ��-����� ���� �������� ������ ���� �� ����� ��� ��� � ��� 
									// �������� ���� � �����.
									if ((fabs(sumP) < RealZERO) && (fabs(sumP_pos) < RealZERO)) {
										//printf("error interpolation zero diagonal sumP.\n");
										//printf("Fnode all sosed is F");
										//system("pause");
										//	printf("i8 is Dirichlet node\n");
										this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
										this_is_C_node[i8] = true;
										bweSholdbeContinue = true;
										iadditionalCstatistic++;
										byes_add = true; // ���� ���������� �����.
														 //exit(1);
														 // ����� ����� �������� �������������.
									}


								}

							}
						}
					}
				}
				else {

					integer iend_marker_position = row_startA[Amat.i[ii1] + 1] - 1;
					for (integer is0 = ii1; (is0 <= iend_marker_position); is0++) {
						if (this_is_C_node[Amat.j[is0]] == true) {
							if (Amat.j[is0] != Amat.i[ii1]) {


								// 20 jan 2016.
								// ����� �� ��������� ���� � ������ ���� ���� �� ����� ��� ������� F �������.


								if ((fabs(sumP) < RealZERO) && (fabs(sumP_pos) < RealZERO)) {
									// ��� ������ ����� ������ ��� ������� � �������.


									//printf("error interpolation zero diagonal sumP.\n");
									//printf("Fnode all sosed is F");
									//system("pause");
									//printf("i8 is Dirichlet node\n");
									this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
									this_is_C_node[i8] = true;
									bweSholdbeContinue = true;
									iadditionalCstatistic++;
									byes_add = true; // ���� ���������� �����.
													 //exit(1);
													 // ����� ����� �������� �������������.
								}


							}
						}
					}
				}

			}

		}
		/////
		if (itypemodifyinterpol > 0) {


			if (i8_candidate_arr != NULL) {
				i8_candidate_arr = (integer*)realloc(i8_candidate_arr, (i8_candidate_count + 2) * sizeof(integer));
			}
			if (i8_candidate_val_arr != NULL) {
				i8_candidate_val_arr = (doublereal*)realloc(i8_candidate_val_arr, (i8_candidate_count + 2) * sizeof(doublereal));
			}


			// �������� integer i_gold_i8 �������� �������.
			integer i_gold_i8 = i_my_min(i8_candidate_count/20, 100);// 100
			if (i8_candidate_count < 100) {
				i_gold_i8 = i8_candidate_count - 3;
			}
			if (1 && from_re_operation_protection0) {
				// ���������� �� �����������.
				// ���������� ������� �. ����� ������������ �� ������ my_nested_dissection.cpp
				QuickSortnd(i8_candidate_arr, i8_candidate_val_arr, 0, i8_candidate_count - 1);

				if (itypemodifyinterpol == 2) {
					// minimum
					//for (integer i_9 = i8_candidate_count - 1; i_9 >= i8_candidate_count - 1 - i_gold_i8; i_9--) {
					for (integer i_9 = 0; i_9 < i_gold_i8; i_9++) {
						// � ������ ���� ������ �������� � ������ � ���������� ������� F �������
						// ����� ���� ���������� � �����. �.�. � ����������� ����� ���� ���������� � �����.
						// ����������� 2 ������� 2016 ����.
						this_is_F_node[i8_candidate_arr[i_9]] = false; // ���� ���� ������� ������ � �����.
						this_is_C_node[i8_candidate_arr[i_9]] = true;
						bweSholdbeContinue = true;
						iadditionalCstatistic++;
						byes_add = true; // ���� ���������� �����.
					}
				}
				if (itypemodifyinterpol == 3) {
					// maximum
					for (integer i_9 = i8_candidate_count - 1; i_9 >= i8_candidate_count - 1 - i_gold_i8; i_9--) {
						//for (integer i_9 = 0; i_9 < i_gold_i8; i_9++) {
						// � ������ ���� ������ �������� � ������ � ���������� ������� F �������
						// ����� ���� ���������� � �����. �.�. � ����������� ����� ���� ���������� � �����.
						// ����������� 2 ������� 2016 ����.
						this_is_F_node[i8_candidate_arr[i_9]] = false; // ���� ���� ������� ������ � �����.
						this_is_C_node[i8_candidate_arr[i_9]] = true;
						bweSholdbeContinue = true;
						iadditionalCstatistic++;
						byes_add = true; // ���� ���������� �����.
					}
				}

			}

			ifrom_re_operation_protection++;
			if (ifrom_re_operation_protection > inumberadaptpass) {
				from_re_operation_protection0 = false;
			}


		}

		if (i8_candidate_arr != NULL) {
			free(i8_candidate_arr);
			i8_candidate_arr = NULL;
		}
		if (i8_candidate_val_arr != NULL) {
			free(i8_candidate_val_arr);
			i8_candidate_val_arr = NULL;
		}



	}



	if (!byes_add) {

		// � ���������� 0.4 ��������� ������������ �������� ����� ������� ������.
		doublereal magic = 0.4; // 0.4 optimum
								//magic = 0.3; // 3 ���� 2016 ��� ������������ �����
								// �������� ������� �� ���� ���������
								// �� �� ������������� �� �� ����� V ������.
								//magic = 0.5 - 0.2*ilevel / 12.0;
		magic = magic82;
		//magic = 0.35;



		the_number_of_neighbors_that_are_not_�_nodes = 0;
		number_of_F_nodes_with_one_single_strong_C_neighbor = 0;



		// ���������� ����������� ��� ����� ������� ���������� F nodes.
		// ������ F-nodes ������ C-nodes.
		for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {

			// ��� ����� ����������� �������� ��������.
			// 5 ������� 2015 ���� �� ��������� ��������� �������������
			// ��������� ������������� � ������ � ��������� ��������.
			doublereal maxelem_threshold_plus = -1.0;
			doublereal maxelem_threshold_minus = -1.0;
			//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
			integer ii1 = row_startA[i8];
			integer istr_etalon1 = Amat.i[ii1];
			integer iend_for1 = -1;
			for (integer is0 = ii1; (is0 <= row_startA[istr_etalon1 + 1] - 1); is0++) {
				iend_for1 = is0;
				if (Amat.j[is0] != istr_etalon1) {
					// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
					//if (this_is_C_node[Amat.j[is0]] == true) {
					if ((Amat.aij[is0]>0) && (fabs(Amat.aij[is0]) > maxelem_threshold_plus)) {
						maxelem_threshold_plus = fabs(Amat.aij[is0]);
					}
					if ((Amat.aij[is0]<0) && (fabs(Amat.aij[is0]) > maxelem_threshold_minus)) {
						maxelem_threshold_minus = fabs(Amat.aij[is0]);
					}
					//}
				}
			}
			// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

			// ����� ������� ������� F-node ������� C-node.
			integer icsos = 0;
			integer icsosF = 0;
			integer icsos_pos = 0;
			integer icsosF_pos = 0;

			doublereal maxelem_threshold_theta_plus = maxelem_threshold_plus*theta_strong_C;
			doublereal maxelem_threshold_theta_strong_F_plus = maxelem_threshold_plus*theta_strong_F;
			//doublereal maxelem_threshold_theta_plus = maxelem_threshold_plus*0.5;
			//doublereal maxelem_threshold_theta_strong_F_plus = maxelem_threshold_plus*0.5;
			doublereal maxelem_threshold_theta_minus = maxelem_threshold_minus*theta_strong_C;
			doublereal maxelem_threshold_theta_strong_F_minus = maxelem_threshold_minus*theta_strong_F;

			// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
			// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
			doublereal sumP = 0.0;
			doublereal SumPall = 0.0;
			integer icount_StronglyF = 0;
			doublereal sumP_pos = 0.0;
			doublereal SumPall_pos = 0.0;
			integer icount_StronglyF_pos = 0;

			//	for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0] == Amat.i[ii1]); is0++) {
			for (integer is0 = ii1; is0 <= iend_for1; is0++) {
				if (Amat.j[is0] != istr_etalon1) {
					if (this_is_C_node[Amat.j[is0]] == true) {
						//	if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
						//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
						if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_minus)) {
							sumP += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
							icsos++;
						}
						if ((Amat.aij[is0]>0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_plus)) {
							sumP_pos += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
							icsos_pos++;
						}
					}
					else {
						if (this_is_F_node[Amat.j[is0]] == true) {
							//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta_strong_F) {
							if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_strong_F_minus)) {
								SumPall += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
								icount_StronglyF++;
								icsosF++;
							}
							if ((Amat.aij[is0] > 0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_strong_F_plus)) {
								SumPall_pos += fabs(Amat.aij[is0]); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
								icount_StronglyF_pos++;
								icsosF_pos++;
							}
						}
						// ������������ ���������� ������� ������� �� �������� � ������.
						the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
					}
				}
			}
			if (icsos == 1) {
				number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ ������� � �������.
																	   // ��������� ������ ������ "����������".
																	   // ���������� ������ ����������� ��� ���������.
																	   // � ������� �� �������� ������� ����� ���������� ������ ���������� ���� ��������� ������� � 
																	   // ������������� �� ���� ������� ����� ��������.
				if (icsosF == 0) number_of_F_nodes_with_one_single_strong_C_neighborF++; // ���������� F ����� � ����� ������������ �������  C ������� � � ����-�� �� ������� ������� F �������.
			}



			// 1 ������ 2016 ���� ����� ��� ������������.
			// ������� � ������ ������ ������ ������ ����� ���� � �����.
			if ((false) && (icsos == 1)) {
				this_is_F_node[i8] = false;
				this_is_C_node[i8] = true;
				bweSholdbeContinue = true;
			}
			else {

				if (((icsos == 1) || (icsos == 2) || (icsos == 3)) && (icsosF != 0)) {
					// ��� ����� �����.
					//if ((((icsos == 1) || (icsos == 2) || (icsos == 3)) && (icsosF != 0))||(((icsos_pos == 1) || (icsos_pos == 2) || (icsos_pos == 3)) && (icsosF_pos != 0))) {
					//	if (((icsos == 1) || (icsos == 2) || (icsos == 3) || (icsos >= 4)) && (icsosF != 0)) {
					//if ((((icsos == 1) || (icsos == 2) || (icsos == 3) || (icsos >= 4)) && (icsosF != 0)) || (((icsos_pos == 1) || (icsos_pos == 2) || (icsos_pos == 3) || (icsos_pos >= 4)) && (icsosF_pos != 0))) {

					// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
					//
					SumPall += sumP;
					SumPall_pos += sumP_pos;


					for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							// ��� ���������� ������ Strong �����.


							if (this_is_C_node[Amat.j[is0]] == true) {
								//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
								if (((Amat.aij[is0]<0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_minus)) || ((Amat.aij[is0]>0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_plus))) {
									if ((fabs(sumP) < RealZERO) && (fabs(sumP_pos) < RealZERO)) {
										//printf("error interpolation zero diagonal sumP.\n");
										//printf("Fnode all sosed is F");
										//system("pause");
										//	printf("i8 is Dirichlet node\n");
										this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
										this_is_C_node[i8] = true;
										bweSholdbeContinue = true;
										iadditionalCstatistic++;
										//exit(1);
										// ����� ����� �������� �������������.
									}
									else {
										// ��� ��� ��� ������������ Strong C �����. 
										// ��������������� ������� �� ��������� � �����.

										// ������ ������� ������ ����������� ��������� 
										// ������������� ��������� �������� �� �������� 
										// �������� �������.
										// ����������� 5 ������� 2015.
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
										if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_minus)) {
											P[icount1].j = i8;
											P[icount1].i = C_numerate[Amat.j[is0]];
											//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
											if (fabs(SumPall) < 1.0e-33) {
												printf("error 1.0 ! division by zero. SumPall =%e\n", SumPall);
												//getchar();
												system("PAUSE");
												exit(1);
											}
											P[icount1].aij = fabs(Amat.aij[is0]) / SumPall;
											icount1++;
											if (icount1 >= nsizePR*n) {
												printf("memory error!!!\n");
												printf("not enough memory for the interpolation operator.\n");
												//system("PAUSE");
												//exit(1);
												deallocate_prolongation(nsizePR, n,  P);
											}
										}
										if ((Amat.aij[is0]>0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_plus)) {
											P[icount1].j = i8;
											P[icount1].i = C_numerate[Amat.j[is0]];
											//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
											if (fabs(SumPall_pos) < 1.0e-33) {
												printf("error 1.0 ! division by zero. SumPall_pos =%e\n", SumPall_pos);
												//getchar();
												system("PAUSE");
												exit(1);
											}
											// �������� !!! ������ �����.
											P[icount1].aij = -fabs(Amat.aij[is0]) / SumPall_pos;
											icount1++;
											if (icount1 >= nsizePR*n) {
												printf("memory error!!!\n");
												printf("not enough memory for the interpolation operator.\n");
												//system("PAUSE");
												//exit(1);
												deallocate_prolongation(nsizePR, n,  P);
											}
										}


									}
								}

							}
							else
								if (this_is_F_node[Amat.j[is0]] == true) {
									//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta_strong_F) {
									if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_strong_F_minus)) {
										// ������������� Strong F �����.

										// ����� :
										// 



										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
										// � ������� F ������.
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta_strong_F) {

										integer iFpoint = Amat.j[is0];
										if (fabs(SumPall) < 1.0e-33) {
											printf("error 2.0 ! division by zero. SumPall =%e\n", SumPall);
											//getchar();
											system("PAUSE");
											exit(1);
										}
										doublereal multiplyer_nu = fabs(Amat.aij[is0]) / SumPall;
										// ��������� ���� ������� iFpointeger 
										// ����� ����� ����� ��� � ����.

										// �������������� ��������� �����.
										doublereal maxelem_threshold_loc = -1.0;
										//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
										integer ii1_loc = row_startA[iFpoint];
										integer istr_etalon = Amat.i[ii1_loc];
										integer iend_for = -1;
										integer iend_marker_position = row_startA[istr_etalon + 1] - 1;
										for (integer is0_loc = ii1_loc; (is0_loc <= iend_marker_position); is0_loc++) {
											iend_for = is0_loc;
											if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc)) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													if (Amat.j[is0_loc] != istr_etalon) {
														maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
													}
												}
											}
										}

										doublereal maxelem_threshold_loc_magic = maxelem_threshold_loc*magic;
										// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

										// ����� ������� ������� F-node ������� C-node.
										integer icsos_loc = 0;

										// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
										// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
										doublereal sumP_loc = 0.0;
										//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

											// ����� �������� ����� ���������� ����� ���������.
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic) {
											if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc_magic)) {






												if (this_is_C_node[Amat.j[is0_loc]] == true) {

													if (Amat.j[is0_loc] != istr_etalon) {

														sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
														icsos_loc++;
													}

												}
												else {

													//if (Amat.j[is0_loc] != istr_etalon) {
													// ������������ ���������� ������� ������� �� �������� � ������.
													//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
													//}
												}

											}
										}

										doublereal maxelem_threshold_loc_magic_minus = -maxelem_threshold_loc_magic;

										// � ����� ��� ������� ����������������� ����� 
										//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

											// ��� �������������� ��������� � ������ ������� ���� �������� ���������� ����� ���������.


											// ��������������� ������� �� ��������� � �����.

											// ������ ������� ������ ����������� ��������� 
											// ������������� ��������� �������� �� �������� 
											// �������� �������.
											// ����������� 5 ������� 2015.
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic) {
											if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc_magic)) {
												//if ((Amat.aij[is0_loc]<maxelem_threshold_loc_magic_minus) || (Amat.aij[is0_loc]>maxelem_threshold_loc_magic)) {

												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													if (Amat.j[is0_loc] != istr_etalon) {

														P[icount1].j = i8;
														P[icount1].i = C_numerate[Amat.j[is0_loc]];
														//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
														if (fabs(sumP_loc) < 1.0e-33) {
															printf("error 3.0 ! division by zero. sumP_loc =%e\n", sumP_loc);
															//getchar();
															system("PAUSE");
															exit(1);
														}
														P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
														icount1++;
														if (icount1 >= nsizePR*n) {
															printf("memory error!!!\n");
															printf("not enough memory for the interpolation operator.\n");
															//system("PAUSE");
															//exit(1);
															deallocate_prolongation(nsizePR, n,  P);
														}

													}
												}
											}
										}


										//}
									}
									// todo
									if ((Amat.aij[is0]>0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_strong_F_plus)) {
										// ������������� Strong F �����.

										// ����� :
										// 



										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
										// � ������� F ������.
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta_strong_F) {

										integer iFpoint = Amat.j[is0];
										if (fabs(SumPall_pos) < 1.0e-33) {
											printf("error 2.0 ! division by zero. SumPall_pos =%e\n", SumPall_pos);
											//getchar();
											system("PAUSE");
											exit(1);
										}
										// �������� �����.
										doublereal multiplyer_nu = -fabs(Amat.aij[is0]) / SumPall_pos;
										// ��������� ���� ������� iFpointeger 
										// ����� ����� ����� ��� � ����.

										// �������������� ��������� �����.
										doublereal maxelem_threshold_loc = -1.0;
										//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
										integer ii1_loc = row_startA[iFpoint];
										integer istr_etalon = Amat.i[ii1_loc];
										integer iend_for = -1;
										integer iend_marker_position = row_startA[istr_etalon + 1] - 1;
										for (integer is0_loc = ii1_loc; (is0_loc <= iend_marker_position); is0_loc++) {
											iend_for = is0_loc;
											if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc)) {
												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													if (Amat.j[is0_loc] != istr_etalon) {
														maxelem_threshold_loc = fabs(Amat.aij[is0_loc]);
													}
												}
											}
										}

										doublereal maxelem_threshold_loc_magic = maxelem_threshold_loc*magic;
										// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

										// ����� ������� ������� F-node ������� C-node.
										integer icsos_loc = 0;

										// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
										// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
										doublereal sumP_loc = 0.0;
										//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

											// ����� �������� ����� ���������� ����� ���������.
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic) {
											if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc_magic)) {






												if (this_is_C_node[Amat.j[is0_loc]] == true) {

													if (Amat.j[is0_loc] != istr_etalon) {

														sumP_loc += fabs(Amat.aij[is0_loc]); // ����� ������� ��������������� ��������� ������� ����������� � �����.
														icsos_loc++;
													}

												}
												else {

													//if (Amat.j[is0_loc] != istr_etalon) {
													// ������������ ���������� ������� ������� �� �������� � ������.
													//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
													//}
												}

											}
										}

										doublereal maxelem_threshold_loc_magic_minus = -maxelem_threshold_loc_magic;

										// � ����� ��� ������� ����������������� ����� 
										//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat.i[is0_loc] == Amat.i[ii1_loc]); is0_loc++) {
										for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

											// ��� �������������� ��������� � ������ ������� ���� �������� ���������� ����� ���������.


											// ��������������� ������� �� ��������� � �����.

											// ������ ������� ������ ����������� ��������� 
											// ������������� ��������� �������� �� �������� 
											// �������� �������.
											// ����������� 5 ������� 2015.
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*barjer) {
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*theta) {
											//if (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc*magic) {
											if ((Amat.aij[is0_loc]<0.0) && (fabs(Amat.aij[is0_loc]) > maxelem_threshold_loc_magic)) {
												//if ((Amat.aij[is0_loc]<maxelem_threshold_loc_magic_minus) || (Amat.aij[is0_loc]>maxelem_threshold_loc_magic)) {

												if (this_is_C_node[Amat.j[is0_loc]] == true) {
													if (Amat.j[is0_loc] != istr_etalon) {

														P[icount1].j = i8;
														P[icount1].i = C_numerate[Amat.j[is0_loc]];
														//P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
														if (fabs(sumP_loc) < 1.0e-33) {
															printf("error 3.0 ! division by zero. sumP_loc =%e\n", sumP_loc);
															//getchar();
															system("PAUSE");
															exit(1);
														}
														P[icount1].aij = multiplyer_nu*fabs(Amat.aij[is0_loc]) / sumP_loc;
														icount1++;
														if (icount1 >= nsizePR*n) {
															printf("memory error!!!\n");
															printf("not enough memory for the interpolation operator.\n");
															//system("PAUSE");
															//exit(1);
															deallocate_prolongation(nsizePR, n,  P);
														}

													}
												}
											}
										}


										//}
									}

								}
						}

					}
				}
				else {

					// �������� bmodify_interpol4_amg==true ��� magic2<0.24 �������� � ����������������� ���������
					// �������� ����������. ��� ���������� ����������� �� bmodify_interpol4_amg==true � magic2>=0.24 
					// �������� ������� �� �������� ����� �� ����������� ���������� ����� �� ������� ��� � ��� 
					// bmodify_interpol4_amg = false;
					// 22 01 2016 ����� �������� bmodify_interpol4_amg = false; � �� � ���� ������ �� ������.
					bool bmodify_interpol4_amg = false;
					// 01.04.2017 
					// bmodify_interpol4_amg = true ���������� �� �����������
					// � � ������ ������������.

					doublereal magic2 = 0.5;
					//magic2 = magic83;

					// ���������������� ���� � �������� ��������� ������������ 
					// �������� ������������ ���������� ��� ���������� ����� � �����.
					// �� ��������� ������������ �� ���� �������� ������� Weakly C ����� ���� �������
					// ������� �������.
					// ���������������� ���� ����������� � ����������� ��������� is0_candidate ����� �������
					// Weakly C ����� � ������ � ���������� ������������ ������� ����������� � sum1.
					doublereal sum1 = 0.0;
					integer is0_candidate = -1;
					if (bmodify_interpol4_amg && ((icsos == 1) && (icsosF == 0))) {
						for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
							if (Amat.j[is0] != Amat.i[ii1]) {
								if (this_is_C_node[Amat.j[is0]] == true) {
									//if (fabs(Amat.aij[is0]) <= maxelem_threshold*theta) {
									if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) <= maxelem_threshold_theta_minus)) {
										if (fabs(Amat.aij[is0]) > sum1) {
											sum1 = fabs(Amat.aij[is0]);
											is0_candidate = is0;
										}
									}
								}
							}
						}

						if (sum1 / sumP < magic2) {
							// ���� ��������� ������� ������� ������ ������ �� ������ � ���� �,
							// � ���� ������ �������� Weakly F ��������� ����� ������� ��� C ����������.
							sum1 = 0.0;
							is0_candidate = -1;

							// ����� ���� ����� Weakly F ����� ���������.
							for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
								if (Amat.j[is0] != Amat.i[ii1]) {
									if (this_is_F_node[Amat.j[is0]] == true) {
										//if (fabs(Amat.aij[is0]) <= maxelem_threshold*theta) {
										if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) <= maxelem_threshold_theta_minus)) {
											if (fabs(Amat.aij[is0]) > sum1) {
												sum1 = fabs(Amat.aij[is0]);
												is0_candidate = is0;
											}
										}
									}
								}
							}

							// ����� ���� F ������ ����� ����� � �����.
							if (sum1 / sumP >= magic2) {
								this_is_F_node[i8] = false; // ���� ����  ������ � �����.
								this_is_C_node[i8] = true;
								bweSholdbeContinue = true;
								byes_add = true;
								iadditionalCstatistic++;

							}
							// ������������ ����� ����� ��������� ����� �� ���� ������� �� ����.
							sum1 = 0.0;
							is0_candidate = -1;

						}
					}


					for (integer is0 = ii1; (is0 <= row_startA[Amat.i[ii1] + 1] - 1); is0++) {
						if (Amat.j[is0] != Amat.i[ii1]) {
							if (this_is_C_node[Amat.j[is0]] == true) {



								if (bmodify_interpol4_amg && ((icsos == 1) && (icsosF == 0))) {
									// �� ��������� ���� ������������ ��� ���������� ����� ����� ��
									// ���� ����� �������������� ����� ������� Weakly C �����.
									// ����� ������� Weakly C ����� ����� ��������� sum1 � is0_candidate.

									ioneStrongC_and_0_StrongF++;
									if (is0_candidate != -1) {
										P[icount1].j = i8;
										P[icount1].i = C_numerate[Amat.j[is0_candidate]];
										P[icount1].aij = fabs(Amat.aij[is0_candidate]) / (sumP + sum1);
										icount1++;
										if (icount1 >= nsizePR*n) {
											printf("memory error!!!\n");
											printf("not enough memory for the interpolation operator.\n");
											//system("PAUSE");
											//exit(1);
											deallocate_prolongation(nsizePR, n,  P);
										}
									}

									//if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
									if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_minus)) {
										P[icount1].j = i8;
										P[icount1].i = C_numerate[Amat.j[is0]];
										P[icount1].aij = fabs(Amat.aij[is0]) / (sumP + sum1);
										icount1++;
										if (icount1 >= nsizePR*n) {
											printf("memory error!!!\n");
											printf("not enough memory for the interpolation operator.\n");
											//system("PAUSE");
											//exit(1);
											deallocate_prolongation(nsizePR, n,  P);
										}
									}
								}
								else {

									if ((fabs(sumP) < RealZERO) && (fabs(sumP_pos) < RealZERO)) {
										//printf("error interpolation zero diagonal sumP.\n");
										//printf("Fnode all sosed is F");
										//system("pause");
										//printf("i8 is Dirichlet node\n");
										this_is_F_node[i8] = false; // ���� ���� ������� ������ � �����.
										this_is_C_node[i8] = true;
										bweSholdbeContinue = true;
										iadditionalCstatistic++;
										//exit(1);
										// ����� ����� �������� �������������.
									}
									else {

										// ������������� ������ �� ��������� � �����.

										// ��������������� ������� �� ��������� � �����.

										// ������ ������� ������ ����������� ��������� 
										// ������������� ��������� �������� �� �������� 
										// �������� �������.
										// ����������� 5 ������� 2015.
										//if (fabs(Amat.aij[is0]) > maxelem_threshold*barjer) {
										//	if (fabs(Amat.aij[is0]) > maxelem_threshold*theta) {
										if ((Amat.aij[is0]<0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_minus)) {
											P[icount1].j = i8;
											P[icount1].i = C_numerate[Amat.j[is0]];
											if (fabs(sumP) < 1.0e-33) {
												printf("error 4.0 ! division by zero. sumP =%e\n", sumP);
												//getchar();
												system("PAUSE");
												exit(1);
											}
											P[icount1].aij = fabs(Amat.aij[is0]) / sumP;
											icount1++;
											if (icount1 >= nsizePR*n) {
												printf("memory error!!!\n");
												printf("not enough memory for the interpolation operator.\n");
												//system("PAUSE");
												//exit(1);
												deallocate_prolongation(nsizePR, n,  P);
											}
										}

										if ((Amat.aij[is0]>0.0) && (fabs(Amat.aij[is0]) > maxelem_threshold_theta_plus)) {
											P[icount1].j = i8;
											P[icount1].i = C_numerate[Amat.j[is0]];
											if (fabs(sumP_pos) < 1.0e-33) {
												printf("error 4.0 ! division by zero. sumP_pos =%e\n", sumP_pos);
												//getchar();
												system("PAUSE");
												exit(1);
											}
											P[icount1].aij = -fabs(Amat.aij[is0]) / sumP_pos;
											icount1++;
											if (icount1 >= nsizePR*n) {
												printf("memory error!!!\n");
												printf("not enough memory for the interpolation operator.\n");
												//system("PAUSE");
												//exit(1);
												deallocate_prolongation(nsizePR, n,  P);
											}
										}

									}

								}

							}
						}
					}
				}

			}
		}
		////

	}

#if doubleintprecision == 1
	//printf("one Strong C and 0 Strong F=%lld additional all %lld", ioneStrongC_and_0_StrongF, iadditionalCstatistic);
#else
	//printf("one Strong C and 0 Strong F=%d additional all %d", ioneStrongC_and_0_StrongF, iadditionalCstatistic);
#endif

	//system("pause");


} // my_interpolation_procedure_number0

#endif // !BASIC_INTERPOLATION_PROCEDURE_MY_AGREGAT_AMG_CPP