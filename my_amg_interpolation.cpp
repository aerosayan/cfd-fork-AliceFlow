// ���������������� ��������� �3.
// ���������������� ������� ������������� �� ������� ����.
// �� ���� ������������ �� ������� ���� �������� ������� ����.
// 12.05.2018
void my_interpolation_procedure_number3A(integer &the_number_of_neighbors_that_are_not_�_nodes,
	integer &number_of_F_nodes_with_one_single_strong_C_neighbor,
	integer* &n_a, bool* &this_is_F_node, integer* &row_startA,
	integer* &nnz_a, bool &bpositive_connections, Ak1* &Amat,
	bool &bweSholdbeContinue, bool* &this_is_C_node, integer &iadditionalCstatistic,
	const doublereal RealZERO, integer &icount1, Ak1* &P, integer &nsizePR, integer &ilevel,
	integer &iadd, doublereal &theta, integer &n, Ak1* &R, integer* &C_numerate,
	integer &number_of_F_nodes_with_one_single_strong_C_neighborF,
	doublereal &theta83, bool &btreshold_on_new_vetv, integer& ifrom_re_operation_protection,
	bool &from_re_operation_protection0, doublereal &magic82, doublereal* &threshold_quick_all,
	doublereal* &threshold_quick_only_negative)
{

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
			

			// ���������� ����������� ��� ����� ������� ���������� F nodes.
			// ������ F-nodes ������ C-nodes.
			for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {


				// ����� ������� ������� F-node ������� C-node.
				integer icsos = 0;
				integer icsosF = 0;

				// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
				// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
				doublereal sumP = 0.0;
				doublereal SumPall = 0.0;
				integer icount_StronglyF = 0;

				//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
				integer ii1 = row_startA[i8];
				integer iend_marker_position = row_startA[Amat[ii1].i + 1] - 1;

				if (bpositive_connections) {


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
				
					//for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[ii1].i); is0++) {
					
					if (!btreshold_on_new_vetv) {
						for (integer is0 = ii1; (is0 <= iend_marker_position); is0++) {
							if (Amat[is0].j != Amat[ii1].i) {
								// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
								//if (this_is_C_node[Amat[is0].j] == true) {
								if (fabs(Amat[is0].aij) > maxelem_threshold) {
									maxelem_threshold = fabs(Amat[is0].aij);
								}
								//}
							}
						}
					}
					else {
						maxelem_threshold = threshold_quick_all[Amat[ii1].i];
					}
					// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

					
					doublereal maxelem_threshold_theta = maxelem_threshold * theta;
					doublereal maxelem_threshold_theta_strong_F = maxelem_threshold * theta_strong_F;
					for (integer is0 = ii1; (is0 <= row_startA[Amat[ii1].i + 1] - 1); is0++) {
						if (Amat[is0].j != Amat[ii1].i) {
							if (this_is_C_node[Amat[is0].j] == true) {
								//	if (fabs(Amat[is0].aij) > maxelem_threshold*barjer) {
								//if (fabs(Amat[is0].aij) > maxelem_threshold*theta) {
								if (fabs(Amat[is0].aij) > maxelem_threshold_theta) {
									sumP += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
									icsos++;
								}
							}
							else {
								if (this_is_F_node[Amat[is0].j] == true) {
									//if (fabs(Amat[is0].aij) > maxelem_threshold*theta_strong_F) {
									if (fabs(Amat[is0].aij) > maxelem_threshold_theta_strong_F) {
										SumPall += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
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
				}
				else {
					// only negative connections

					
						// ��� ����� ����������� �������� ��������.
						// 5 ������� 2015 ���� �� ��������� ��������� �������������
						// ��������� ������������� � ������ � ��������� ��������.
						doublereal maxelem_threshold = -1.0;
						
						//for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[ii1].i); is0++) {
		
						if (!btreshold_on_new_vetv) {
							for (integer is0 = ii1; (is0 <= iend_marker_position); is0++) {
								if (Amat[is0].j != Amat[ii1].i) {
									// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
									//if (this_is_C_node[Amat[is0].j] == true) {
									if ((Amat[is0].aij < 0.0) && (fabs(Amat[is0].aij) > maxelem_threshold)) {
										maxelem_threshold = fabs(Amat[is0].aij);
									}
									//}
								}
							}
						}
						else {
							maxelem_threshold = threshold_quick_only_negative[Amat[ii1].i];
						}
						// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

						
						doublereal maxelem_threshold_theta = maxelem_threshold * theta;
						doublereal maxelem_threshold_theta_strong_F = maxelem_threshold * theta_strong_F;
						for (integer is0 = ii1; (is0 <= row_startA[Amat[ii1].i + 1] - 1); is0++) {
							if (Amat[is0].j != Amat[ii1].i) {
								if (this_is_C_node[Amat[is0].j] == true) {
									//	if (fabs(Amat[is0].aij) > maxelem_threshold*barjer) {
									//if (fabs(Amat[is0].aij) > maxelem_threshold*theta) {
									if ((Amat[is0].aij < 0.0) && (fabs(Amat[is0].aij) > maxelem_threshold_theta)) {
										sumP += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
										icsos++;
									}
								}
								else {
									if (this_is_F_node[Amat[is0].j] == true) {
										//if (fabs(Amat[is0].aij) > maxelem_threshold*theta_strong_F) {
										if ((Amat[is0].aij < 0.0) && (fabs(Amat[is0].aij) > maxelem_threshold_theta_strong_F)) {
											SumPall += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
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

				}

				/*
				9.05.2018
				1. ���������� threshold�.
				2. ������� �����. SumP - ��� strong C, SumPall - ��� strong F.
				3. icsos - ���������� strong C. icsosF - ���������� strong F.
				*/

				// 1 ������ 2016 ���� ����� ��� ������������.
				// ������� � ������ ������ ������ ������ ����� ���� � �����.


				// �� ������ �� F ���� C ���� ������ � ��� ������ ���� � ��� ���� � ������.
				//integer iend_marker_position = row_startA[Amat[ii1].i + 1] - 1;
				for (integer is0 = ii1; (is0 <= iend_marker_position); is0++) {
					if (this_is_F_node[Amat[is0].j] == true) {
						if (Amat[is0].j != Amat[ii1].i) {


							// 20 jan 2016.
							// ����� �� ��������� ���� � ������ ���� ���� �� ����� ��� ������� F �������.


							if (fabs(sumP) < RealZERO) {
								// ��� ������ ����� ������ ��� ������� � �������.

								// if (icsosF==0) ������ ������ ����������.
								//if (icsosF<2)// ����������� ���������� �������� ���������� � ������� �������.
								{
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
				}//end

			}

	}
	




	/*
	9.05.2018
	1. ���������� threshold�.
	2. ������� �����. SumP - ��� strong C, SumPall - ��� strong F.
	3. icsos - ���������� strong C. icsosF - ���������� strong F.
	4. ���� SumP ==0 �� ���� ���������� � �����. iadditionalCstatistic++;
	0;
	47; 0;
	186; 0;
	267; 0;
	296; 0;
	276; 0;
	221; 0;
	257.6; 0;
	335.2; 0;
	11L 3min 10s
	*/

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
				integer istr_etalon1 = Amat[ii1].i;
				integer iend_for1 = -1;
				if (!btreshold_on_new_vetv) {
					for (integer is0 = ii1; (is0 <= row_startA[istr_etalon1 + 1] - 1); is0++) {
						iend_for1 = is0;
						if (Amat[is0].j != istr_etalon1) {
							// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
							//if (this_is_C_node[Amat[is0].j] == true) {
							if (fabs(Amat[is0].aij) > maxelem_threshold) {
								maxelem_threshold = fabs(Amat[is0].aij);
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

				doublereal maxelem_threshold_theta = maxelem_threshold * theta;
				doublereal maxelem_threshold_theta_strong_F = maxelem_threshold * theta_strong_F;


				// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
				// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
				doublereal sumP = 0.0;
				doublereal SumPall = 0.0;
				integer icount_StronglyF = 0;
				//	for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[ii1].i); is0++) {
				for (integer is0 = ii1; is0 <= iend_for1; is0++) {
					if (Amat[is0].j != istr_etalon1) {
						if (this_is_C_node[Amat[is0].j] == true) {
							//	if (fabs(Amat[is0].aij) > maxelem_threshold*barjer) {
							//if (fabs(Amat[is0].aij) > maxelem_threshold*theta) {
							if (fabs(Amat[is0].aij) > maxelem_threshold_theta) {
								sumP += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
								icsos++;
							}
						}
						else {
							if (this_is_F_node[Amat[is0].j] == true) {
								//if (fabs(Amat[is0].aij) > maxelem_threshold*theta_strong_F) {
								if (fabs(Amat[is0].aij) > maxelem_threshold_theta_strong_F) {
									SumPall += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
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



				 {

					//if (((icsos == 1) || (icsos == 2) || (icsos == 3)) && (icsosF != 0)) {
					if (1) {
						//if (((icsos == 1) || (icsos == 2) || (icsos == 3) || (icsos >= 4)) && (icsosF != 0)) {
						// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
						//
						SumPall += sumP;

						for (integer is0 = ii1; (is0 <= row_startA[Amat[ii1].i + 1] - 1); is0++) {
							if (Amat[is0].j != Amat[ii1].i) {
								// ��� ���������� ������ Strong �����.


								if (this_is_C_node[Amat[is0].j] == true) {
									//if (fabs(Amat[is0].aij) > maxelem_threshold*theta) {
									if (fabs(Amat[is0].aij) > maxelem_threshold_theta) {
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
											//if (fabs(Amat[is0].aij) > maxelem_threshold*barjer) {
											//if (fabs(Amat[is0].aij) > maxelem_threshold*theta) {
											if (fabs(Amat[is0].aij) > maxelem_threshold_theta) {
												P[icount1].j = i8;
												P[icount1].i = C_numerate[Amat[is0].j];
												//P[icount1].aij = fabs(Amat[is0].aij) / sumP;
												if (fabs(SumPall) < 1.0e-33) {
													printf("error 1.0 ! division by zero. SumPall =%e\n", SumPall);
													//getchar();
													system("PAUSE");
													exit(1);
												}
												P[icount1].aij = fabs(Amat[is0].aij) / SumPall;
												icount1++;
												if (icount1 >= nsizePR * n) {
													printf("memory error!!!\n");
													printf("not enough memory for the interpolation operator.\n");
													//system("PAUSE");
													//exit(1);
													deallocate_prolongation(nsizePR, n, R, P);
												}
											}

										}
									}

								}
								else
									if (this_is_F_node[Amat[is0].j] == true) {
										//if (fabs(Amat[is0].aij) > maxelem_threshold*theta_strong_F) {
										if (fabs(Amat[is0].aij) > maxelem_threshold_theta_strong_F) {
											// ������������� Strong F �����.

											// ����� :
											// 



											//if (fabs(Amat[is0].aij) > maxelem_threshold*barjer) {
											// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
											// � ������� F ������.
											//if (fabs(Amat[is0].aij) > maxelem_threshold*theta_strong_F) {

											integer iFpoint = Amat[is0].j;
											if (fabs(SumPall) < 1.0e-33) {
												printf("error 2.0 ! division by zero. SumPall =%e\n", SumPall);
												//getchar();
												system("PAUSE");
												exit(1);
											}
											doublereal multiplyer_nu = fabs(Amat[is0].aij) / SumPall;
											// ��������� ���� ������� iFpointeger 
											// ����� ����� ����� ��� � ����.

											// �������������� ��������� �����.
											doublereal maxelem_threshold_loc = -1.0;
											//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
											integer ii1_loc = row_startA[iFpoint];
											integer istr_etalon = Amat[ii1_loc].i;
											integer iend_for = -1;
											integer iend_marker_position_loc = row_startA[istr_etalon + 1] - 1;
											for (integer is0_loc = ii1_loc; (is0_loc <= iend_marker_position_loc); is0_loc++) {
												iend_for = is0_loc;
												if (fabs(Amat[is0_loc].aij) > maxelem_threshold_loc) {
													// ����� ��� threshold ������ ���� ��������� ������ �� � �����.
													// ����� ������ ����������.
													if (this_is_C_node[Amat[is0_loc].j] == true) {
														if (Amat[is0_loc].j != istr_etalon) {
															maxelem_threshold_loc = fabs(Amat[is0_loc].aij);
														}
													}
												}
											}

											doublereal maxelem_threshold_loc_magic = maxelem_threshold_loc * magic;
											// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

											// ����� ������� ������� F-node ������� C-node.
											integer icsos_loc = 0;

											// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
											// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
											doublereal sumP_loc = 0.0;
											//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat[is0_loc].i == Amat[ii1_loc].i); is0_loc++) {
											for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

												// ����� �������� ����� ���������� ����� ���������.
												if (fabs(Amat[is0_loc].aij) > maxelem_threshold_loc_magic) {

													if (this_is_C_node[Amat[is0_loc].j] == true) {

														if (Amat[is0_loc].j != istr_etalon) {

															sumP_loc += fabs(Amat[is0_loc].aij); // ����� ������� ��������������� ��������� ������� ����������� � �����.
															icsos_loc++;
														}

													}
													else {

														//if (Amat[is0_loc].j != istr_etalon) {
														// ������������ ���������� ������� ������� �� �������� � ������.
														//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
														//}
													}

												}
											}

											doublereal maxelem_threshold_loc_magic_minus = -maxelem_threshold_loc_magic;

											// � ����� ��� ������� ����������������� ����� 
											//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat[is0_loc].i == Amat[ii1_loc].i); is0_loc++) {
											for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

												// ��� �������������� ��������� � ������ ������� ���� �������� ���������� ����� ���������.


												// ��������������� ������� �� ��������� � �����.

												// ������ ������� ������ ����������� ��������� 
												// ������������� ��������� �������� �� �������� 
												// �������� �������.
												// ����������� 5 ������� 2015.
												if (fabs(Amat[is0_loc].aij) > maxelem_threshold_loc_magic) {
													
													if (this_is_C_node[Amat[is0_loc].j] == true) {
														if (Amat[is0_loc].j != istr_etalon) {

															P[icount1].j = i8;
															P[icount1].i = C_numerate[Amat[is0_loc].j];
															//P[icount1].aij = fabs(Amat[is0].aij) / sumP;
															if (fabs(sumP_loc) < 1.0e-33) {
																printf("error 3.0 ! division by zero. sumP_loc =%e\n", sumP_loc);
																//getchar();
																system("PAUSE");
																exit(1);
															}
															P[icount1].aij = multiplyer_nu * fabs(Amat[is0_loc].aij) / sumP_loc;
															icount1++;
															if (icount1 >= nsizePR * n) {
																printf("memory error!!!\n");
																printf("not enough memory for the interpolation operator.\n");
																//system("PAUSE");
																//exit(1);
																deallocate_prolongation(nsizePR, n, R, P);
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
				integer istr_etalon1 = Amat[ii1].i;
				integer iend_for1 = -1;
				if (!btreshold_on_new_vetv) {
					for (integer is0 = ii1; (is0 <= row_startA[istr_etalon1 + 1] - 1); is0++) {
						iend_for1 = is0;
						if (Amat[is0].j != istr_etalon1) {
							// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
							//if (this_is_C_node[Amat[is0].j] == true) {
							if ((Amat[is0].aij < 0.0) && (fabs(Amat[is0].aij) > maxelem_threshold)) {
								maxelem_threshold = fabs(Amat[is0].aij);
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

				doublereal maxelem_threshold_theta = maxelem_threshold * theta;
				doublereal maxelem_threshold_theta_strong_F = maxelem_threshold * theta_strong_F;


				// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
				// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
				doublereal sumP = 0.0;
				doublereal SumPall = 0.0;
				integer icount_StronglyF = 0;
				//	for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[ii1].i); is0++) {
				for (integer is0 = ii1; is0 <= iend_for1; is0++) {
					if (Amat[is0].j != istr_etalon1) {
						if (this_is_C_node[Amat[is0].j] == true) {
							//	if (fabs(Amat[is0].aij) > maxelem_threshold*barjer) {
							//if (fabs(Amat[is0].aij) > maxelem_threshold*theta) {
							if ((Amat[is0].aij<0.0) && (fabs(Amat[is0].aij) > maxelem_threshold_theta)) {
								sumP += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
								icsos++;
							}
						}
						else {
							if (this_is_F_node[Amat[is0].j] == true) {
								//if (fabs(Amat[is0].aij) > maxelem_threshold*theta_strong_F) {
								if ((Amat[is0].aij<0.0) && (fabs(Amat[is0].aij) > maxelem_threshold_theta_strong_F)) {
									SumPall += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
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



				 {

					if (1) {
					//if (((icsos == 1) || (icsos == 2) || (icsos == 3)) && (icsosF != 0)) {
						// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
						//
						SumPall += sumP;

						for (integer is0 = ii1; (is0 <= row_startA[Amat[ii1].i + 1] - 1); is0++) {
							if (Amat[is0].j != Amat[ii1].i) {
								// ��� ���������� ������ Strong �����.


								if (this_is_C_node[Amat[is0].j] == true) {
									//if (fabs(Amat[is0].aij) > maxelem_threshold*theta) {
									if ((Amat[is0].aij<0.0) && (fabs(Amat[is0].aij) > maxelem_threshold_theta)) {
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
											//if (fabs(Amat[is0].aij) > maxelem_threshold*barjer) {
											//if (fabs(Amat[is0].aij) > maxelem_threshold*theta) {
											if ((Amat[is0].aij<0.0) && (fabs(Amat[is0].aij) > maxelem_threshold_theta)) {
												P[icount1].j = i8;
												P[icount1].i = C_numerate[Amat[is0].j];
												//P[icount1].aij = fabs(Amat[is0].aij) / sumP;
												if (fabs(SumPall) < 1.0e-23) {
													printf("error 5.0 ! division by zero. SumPall =%e\n", SumPall);
													//getchar();
													system("PAUSE");
													exit(1);
												}
												P[icount1].aij = fabs(Amat[is0].aij) / SumPall;
												icount1++;
												if (icount1 >= nsizePR * n) {
													printf("memory error!!!\n");
													printf("not enough memory for the interpolation operator.\n");
													//system("PAUSE");
													//exit(1);
													deallocate_prolongation(nsizePR, n, R, P);
												}
											}

										}
									}

								}
								else
									if (this_is_F_node[Amat[is0].j] == true) {
										//if (fabs(Amat[is0].aij) > maxelem_threshold*theta_strong_F) {
										if ((Amat[is0].aij<0.0) && (fabs(Amat[is0].aij) > maxelem_threshold_theta_strong_F)) {
											// ������������� Strong F �����.

											// ����� :
											// 



											//if (fabs(Amat[is0].aij) > maxelem_threshold*barjer) {
											// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
											// � ������� F ������.
											//if (fabs(Amat[is0].aij) > maxelem_threshold*theta_strong_F) {

											integer iFpoint = Amat[is0].j;
											doublereal multiplyer_nu = fabs(Amat[is0].aij) / SumPall;
											// ��������� ���� ������� iFpointeger 
											// ����� ����� ����� ��� � ����.

											// �������������� ��������� �����.
											doublereal maxelem_threshold_loc = -1.0;
											//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
											integer ii1_loc = row_startA[iFpoint];
											integer istr_etalon = Amat[ii1_loc].i;
											integer iend_for = -1;
											integer iend_marker_position = row_startA[istr_etalon + 1] - 1;
											for (integer is0_loc = ii1_loc; (is0_loc <= iend_marker_position); is0_loc++) {
												iend_for = is0_loc;
												if ((Amat[is0_loc].aij<0.0) && (fabs(Amat[is0_loc].aij) > maxelem_threshold_loc)) {
													if (this_is_C_node[Amat[is0_loc].j] == true) {
														if (Amat[is0_loc].j != istr_etalon) {
															maxelem_threshold_loc = fabs(Amat[is0_loc].aij);
														}
													}
												}
											}

											doublereal maxelem_threshold_loc_magic = maxelem_threshold_loc * magic;
											// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

											// ����� ������� ������� F-node ������� C-node.
											integer icsos_loc = 0;

											// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
											// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
											doublereal sumP_loc = 0.0;
											//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat[is0_loc].i == Amat[ii1_loc].i); is0_loc++) {
											for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

												// ����� �������� ����� ���������� ����� ���������.
												if ((Amat[is0_loc].aij<0.0) && (fabs(Amat[is0_loc].aij) > maxelem_threshold_loc_magic)) {
													
													if (this_is_C_node[Amat[is0_loc].j] == true) {

														if (Amat[is0_loc].j != istr_etalon) {

															sumP_loc += fabs(Amat[is0_loc].aij); // ����� ������� ��������������� ��������� ������� ����������� � �����.
															icsos_loc++;
														}

													}
													else {

														//if (Amat[is0_loc].j != istr_etalon) {
														// ������������ ���������� ������� ������� �� �������� � ������.
														//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
														//}
													}

												}
											}

											doublereal maxelem_threshold_loc_magic_minus = -maxelem_threshold_loc_magic;

											// � ����� ��� ������� ����������������� ����� 
											//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat[is0_loc].i == Amat[ii1_loc].i); is0_loc++) {
											for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

												// ��� �������������� ��������� � ������ ������� ���� �������� ���������� ����� ���������.


												// ��������������� ������� �� ��������� � �����.

												// ������ ������� ������ ����������� ��������� 
												// ������������� ��������� �������� �� �������� 
												// �������� �������.
												// ����������� 5 ������� 2015.
												if ((Amat[is0_loc].aij<0.0) && (fabs(Amat[is0_loc].aij) > maxelem_threshold_loc_magic)) {
													
													if (this_is_C_node[Amat[is0_loc].j] == true) {
														if (Amat[is0_loc].j != istr_etalon) {

															P[icount1].j = i8;
															P[icount1].i = C_numerate[Amat[is0_loc].j];
															//P[icount1].aij = fabs(Amat[is0].aij) / sumP;
															if (fabs(sumP_loc) < 1.0e-23) {
																printf("error 6.0 ! division by zero. sumP_loc =%e\n", sumP_loc);
																//getchar();
																system("PAUSE");
																exit(1);
															}
															P[icount1].aij = multiplyer_nu * fabs(Amat[is0_loc].aij) / sumP_loc;
															icount1++;
															if (icount1 >= nsizePR * n) {
																printf("memory error!!!\n");
																printf("not enough memory for the interpolation operator.\n");
																//system("PAUSE");
																//exit(1);
																deallocate_prolongation(nsizePR, n, R, P);
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

		}

	}



	//system("pause");


} // my_interpolation_procedure_number3A

  // ���������������� ��������� �3.
  // ���������������� ������� ������������� �� ������� ����.
  // �� ���� ������������ �� ������� ���� �������� ������� ����.
  // 12.05.2018
void my_interpolation_procedure_number3B(integer &the_number_of_neighbors_that_are_not_�_nodes,
	integer &number_of_F_nodes_with_one_single_strong_C_neighbor,
	integer* &n_a, bool* &this_is_F_node, integer* &row_startA,
	integer* &nnz_a, bool &bpositive_connections, Ak1* &Amat,
	bool &bweSholdbeContinue, bool* &this_is_C_node, integer &iadditionalCstatistic,
	const doublereal RealZERO, integer &icount1, Ak1* &P, integer &nsizePR, integer &ilevel,
	integer &iadd, doublereal &theta, integer &n, Ak1* &R, integer* &C_numerate,
	integer &number_of_F_nodes_with_one_single_strong_C_neighborF,
	doublereal &theta83, bool &btreshold_on_new_vetv, integer& ifrom_re_operation_protection,
	bool &from_re_operation_protection0, doublereal &magic82, doublereal* &threshold_quick_all,
	doublereal* &threshold_quick_only_negative)
{

	//theta = 0.24;
	// theta_strong_F iter_number time,s
	// 0.21 56 22.63
	// 0.22 55 21.769
	// 0.23 52 21.488
	// 0.24 52 21.741 == theta // optimum
	// 0.26 69 24.623
	doublereal theta_strong_F = 0.23; // ����������� �����.
	theta_strong_F = theta83; // 3 ���� 2016

	// ��� �������
	bool* ht = new bool[n_a[ilevel-1]+1];
	for (integer i_1 = 0; i_1 <= n_a[ilevel - 1]; i_1++) {
		// ������������� ��� �������.
		ht[i_1] = false;
	}


	// �������� ������ ������������.
	// ������������� ���� � �������.
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


		// ���������� ����������� ��� ����� ������� ���������� F nodes.
		// ������ F-nodes ������ C-nodes.
		for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {


			bool badd_amg1r5 = false;

			// ����� ������� ������� F-node ������� C-node.
			integer icsos = 0;
			integer icsosF = 0;

			// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
			// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
			doublereal sumP = 0.0;
			doublereal SumPall = 0.0;
			integer icount_StronglyF = 0;

			//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
			integer ii1 = row_startA[i8];
			integer iend_marker_position = row_startA[Amat[ii1].i + 1] - 1;

			if (bpositive_connections) {


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

				//for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[ii1].i); is0++) {

				if (!btreshold_on_new_vetv) {
					for (integer is0 = ii1; (is0 <= iend_marker_position); is0++) {
						if (Amat[is0].j != Amat[ii1].i) {
							// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
							//if (this_is_C_node[Amat[is0].j] == true) {
							if (fabs(Amat[is0].aij) > maxelem_threshold) {
								maxelem_threshold = fabs(Amat[is0].aij);
							}
							//}
						}
					}
				}
				else {
					maxelem_threshold = threshold_quick_all[Amat[ii1].i];
				}
				// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

				doublereal maxelem_threshold_theta = maxelem_threshold * theta;
				doublereal maxelem_threshold_theta_strong_F = maxelem_threshold * theta_strong_F;

				// ������������� ��� �������.
				for (integer is0 = ii1; (is0 <= row_startA[Amat[ii1].i + 1] - 1); is0++) {
					if (Amat[is0].j != Amat[ii1].i) {
						if (this_is_C_node[Amat[is0].j] == true) {
							if (fabs(Amat[is0].aij) > maxelem_threshold_theta) {
								// ������� ���� ������� � �������.
								ht[Amat[is0].j] = true;
							}
						}
					}
				}

				
				for (integer is0 = ii1; (is0 <= row_startA[Amat[ii1].i + 1] - 1); is0++) {
					if (Amat[is0].j != Amat[ii1].i) {
						if (this_is_C_node[Amat[is0].j] == true) {
							//	if (fabs(Amat[is0].aij) > maxelem_threshold*barjer) {
							//if (fabs(Amat[is0].aij) > maxelem_threshold*theta) {
							if (fabs(Amat[is0].aij) > maxelem_threshold_theta) {
								sumP += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
								icsos++;
							}
						}
						else {
							if (this_is_F_node[Amat[is0].j] == true) {
								//if (fabs(Amat[is0].aij) > maxelem_threshold*theta_strong_F) {
								if (fabs(Amat[is0].aij) > maxelem_threshold_theta_strong_F) {
									
									integer ii2 = row_startA[Amat[is0].j];
									integer iend_marker_position2 = row_startA[Amat[ii2].i + 1] - 1;
									bool bfound = false;
									for (integer is2 = ii2; (is2 <= iend_marker_position2); is2++) {
										if (Amat[is2].j != Amat[ii2].i) {
											if (ht[Amat[is2].j]) {
												bfound = true;
											}
										}
									}

									
										if (bfound) {
											// ������� ������ ��� ������� F ������� ������� ����� � �������(����� �� ����������� �������) �������������� ������� � �������.

											SumPall += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
												icount_StronglyF++;
												icsosF++;
										}
										else {
											badd_amg1r5 = true;
										}
								}
							}
							// ������������ ���������� ������� ������� �� �������� � ������.
							the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
						}
					}
				}

				// ����� - ������� ��� �������.
				for (integer is0 = ii1; (is0 <= row_startA[Amat[ii1].i + 1] - 1); is0++) {
					if (Amat[is0].j != Amat[ii1].i) {
						if (this_is_C_node[Amat[is0].j] == true) {
							if (fabs(Amat[is0].aij) > maxelem_threshold_theta) {
								ht[Amat[is0].j] = false;
							}
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
			}
			else {
				// only negative connections


				// ��� ����� ����������� �������� ��������.
				// 5 ������� 2015 ���� �� ��������� ��������� �������������
				// ��������� ������������� � ������ � ��������� ��������.
				doublereal maxelem_threshold = -1.0;

				//for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[ii1].i); is0++) {

				if (!btreshold_on_new_vetv) {
					for (integer is0 = ii1; (is0 <= iend_marker_position); is0++) {
						if (Amat[is0].j != Amat[ii1].i) {
							// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
							//if (this_is_C_node[Amat[is0].j] == true) {
							if ((Amat[is0].aij < 0.0) && (fabs(Amat[is0].aij) > maxelem_threshold)) {
								maxelem_threshold = fabs(Amat[is0].aij);
							}
							//}
						}
					}
				}
				else {
					maxelem_threshold = threshold_quick_only_negative[Amat[ii1].i];
				}
				// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.


				doublereal maxelem_threshold_theta = maxelem_threshold * theta;
				doublereal maxelem_threshold_theta_strong_F = maxelem_threshold * theta_strong_F;
				
				// ������������� ��� �������.
				for (integer is0 = ii1; (is0 <= row_startA[Amat[ii1].i + 1] - 1); is0++) {
					if (Amat[is0].j != Amat[ii1].i) {
						if (this_is_C_node[Amat[is0].j] == true) {
							if ((Amat[is0].aij < 0.0) && (fabs(Amat[is0].aij) > maxelem_threshold_theta)) {
								ht[Amat[is0].j] = true;
							}
						}
					}
				}

				for (integer is0 = ii1; (is0 <= row_startA[Amat[ii1].i + 1] - 1); is0++) {
					if (Amat[is0].j != Amat[ii1].i) {
						if (this_is_C_node[Amat[is0].j] == true) {
							//	if (fabs(Amat[is0].aij) > maxelem_threshold*barjer) {
							//if (fabs(Amat[is0].aij) > maxelem_threshold*theta) {
							if ((Amat[is0].aij < 0.0) && (fabs(Amat[is0].aij) > maxelem_threshold_theta)) {
								sumP += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
								icsos++;
							}
						}
						else {
							if (this_is_F_node[Amat[is0].j] == true) {
								//if (fabs(Amat[is0].aij) > maxelem_threshold*theta_strong_F) {
								if ((Amat[is0].aij < 0.0) && (fabs(Amat[is0].aij) > maxelem_threshold_theta_strong_F)) {
									
									
									integer ii2 = row_startA[Amat[is0].j];
									integer iend_marker_position2 = row_startA[Amat[ii2].i + 1] - 1;
									bool bfound = false;
									for (integer is2 = ii2; (is2 <= iend_marker_position2); is2++) {
										if (Amat[is2].j != Amat[ii2].i) {
											if (ht[Amat[is2].j]) {												
												bfound = true;
											}
										}
									}
									
									if (bfound) {

										// ������� ������ ��� ������� F ������� ������� ����� � �������(����� �� ����������� �������) �������������� ������� � �������.
										SumPall += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
											icount_StronglyF++;
											icsosF++;
									}
									else {
										badd_amg1r5 = true;
									}
								}
							}
							// ������������ ���������� ������� ������� �� �������� � ������.
							the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
						}
					}
				}

				// ����� - ������� ��� �������.
				for (integer is0 = ii1; (is0 <= row_startA[Amat[ii1].i + 1] - 1); is0++) {
					if (Amat[is0].j != Amat[ii1].i) {
						if (this_is_C_node[Amat[is0].j] == true) {
							if ((Amat[is0].aij < 0.0) && (fabs(Amat[is0].aij) > maxelem_threshold_theta)) {
								ht[Amat[is0].j] = false;
							}
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

			}

			/*
			9.05.2018
			1. ���������� threshold�.
			2. ������� �����. SumP - ��� strong C, SumPall - ��� strong F.
			3. icsos - ���������� strong C. icsosF - ���������� strong F.
			*/

			// 1 ������ 2016 ���� ����� ��� ������������.
			// ������� � ������ ������ ������ ������ ����� ���� � �����.


			// �� ������ �� F ���� C ���� ������ � ��� ������ ���� � ��� ���� � ������.
			//integer iend_marker_position = row_startA[Amat[ii1].i + 1] - 1;
			for (integer is0 = ii1; (is0 <= iend_marker_position); is0++) {
				if (this_is_F_node[Amat[is0].j] == true) {
					if (Amat[is0].j != Amat[ii1].i) {


						// 20 jan 2016.
						// ����� �� ��������� ���� � ������ ���� ���� �� ����� ��� ������� F �������.


						if ((fabs(sumP) < RealZERO)||(badd_amg1r5)) {
							// ��� ������ ����� ������ ��� ������� � �������.

							// if (icsosF==0) ������ ������ ����������.
							//if (icsosF<2)// ����������� ���������� �������� ���������� � ������� �������.
							{
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
			}//end





		}

	}





	/*
	9.05.2018
	1. ���������� threshold�.
	2. ������� �����. SumP - ��� strong C, SumPall - ��� strong F.
	3. icsos - ���������� strong C. icsosF - ���������� strong F.
	4. ���� SumP ==0 �� ���� ���������� � �����. iadditionalCstatistic++;
	0;
	47; 0;
	186; 0;
	267; 0;
	296; 0;
	276; 0;
	221; 0;
	257.6; 0;
	335.2; 0;
	11L 3min 10s
	*/

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
				integer istr_etalon1 = Amat[ii1].i;
				integer iend_for1 = -1;
				if (!btreshold_on_new_vetv) {
					for (integer is0 = ii1; (is0 <= row_startA[istr_etalon1 + 1] - 1); is0++) {
						iend_for1 = is0;
						if (Amat[is0].j != istr_etalon1) {
							// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
							//if (this_is_C_node[Amat[is0].j] == true) {
							if (fabs(Amat[is0].aij) > maxelem_threshold) {
								maxelem_threshold = fabs(Amat[is0].aij);
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

				doublereal maxelem_threshold_theta = maxelem_threshold * theta;
				doublereal maxelem_threshold_theta_strong_F = maxelem_threshold * theta_strong_F;

				for (integer is0 = ii1; (is0 <= row_startA[Amat[ii1].i + 1] - 1); is0++) {
					if (Amat[is0].j != Amat[ii1].i) {
						if (this_is_C_node[Amat[is0].j] == true) {
							if (fabs(Amat[is0].aij) > maxelem_threshold_theta) {
								ht[Amat[is0].j] = true;
							}
						}
					}
				}



				// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
				// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
				doublereal sumP = 0.0;
				doublereal SumPall = 0.0;
				integer icount_StronglyF = 0;
				//	for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[ii1].i); is0++) {
				for (integer is0 = ii1; is0 <= iend_for1; is0++) {
					if (Amat[is0].j != istr_etalon1) {
						if (this_is_C_node[Amat[is0].j] == true) {
							//	if (fabs(Amat[is0].aij) > maxelem_threshold*barjer) {
							//if (fabs(Amat[is0].aij) > maxelem_threshold*theta) {
							if (fabs(Amat[is0].aij) > maxelem_threshold_theta) {
								sumP += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
								icsos++;
							}
						}
						else {
							if (this_is_F_node[Amat[is0].j] == true) {
								//if (fabs(Amat[is0].aij) > maxelem_threshold*theta_strong_F) {
								if (fabs(Amat[is0].aij) > maxelem_threshold_theta_strong_F) {

									integer ii2 = row_startA[Amat[is0].j];
									integer iend_marker_position2 = row_startA[Amat[ii2].i + 1] - 1;
									bool bfound = false;
									for (integer is2 = ii2; (is2 <= iend_marker_position2); is2++) {
										if (Amat[is2].j != Amat[ii2].i) {
											if (ht[Amat[is2].j]) {
												bfound = true;
											}
										}
									}
									if (bfound) {


										SumPall += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
											icount_StronglyF++;
										icsosF++;
									}
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



				{

					//if (((icsos == 1) || (icsos == 2) || (icsos == 3)) && (icsosF != 0)) {
					if (1) {
						//if (((icsos == 1) || (icsos == 2) || (icsos == 3) || (icsos >= 4)) && (icsosF != 0)) {
						// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
						//
						SumPall += sumP;

						for (integer is0 = ii1; (is0 <= row_startA[Amat[ii1].i + 1] - 1); is0++) {
							if (Amat[is0].j != Amat[ii1].i) {
								// ��� ���������� ������ Strong �����.


								if (this_is_C_node[Amat[is0].j] == true) {
									//if (fabs(Amat[is0].aij) > maxelem_threshold*theta) {
									if (fabs(Amat[is0].aij) > maxelem_threshold_theta) {
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
											//if (fabs(Amat[is0].aij) > maxelem_threshold*barjer) {
											//if (fabs(Amat[is0].aij) > maxelem_threshold*theta) {
											if (fabs(Amat[is0].aij) > maxelem_threshold_theta) {
												P[icount1].j = i8;
												P[icount1].i = C_numerate[Amat[is0].j];
												//P[icount1].aij = fabs(Amat[is0].aij) / sumP;
												if (fabs(SumPall) < 1.0e-33) {
													printf("error 1.0 ! division by zero. SumPall =%e\n", SumPall);
													//getchar();
													system("PAUSE");
													exit(1);
												}
												P[icount1].aij = fabs(Amat[is0].aij) / SumPall;
												icount1++;
												if (icount1 >= nsizePR * n) {
													printf("memory error!!!\n");
													printf("not enough memory for the interpolation operator.\n");
													//system("PAUSE");
													//exit(1);
													deallocate_prolongation(nsizePR, n, R, P);
												}
											}

										}
									}

								}
								else
									if (this_is_F_node[Amat[is0].j] == true) {
										//if (fabs(Amat[is0].aij) > maxelem_threshold*theta_strong_F) {
										if (fabs(Amat[is0].aij) > maxelem_threshold_theta_strong_F) {
											// ������������� Strong F �����.

											// ����� :
											// 

											integer ii2 = row_startA[Amat[is0].j];
											integer iend_marker_position2 = row_startA[Amat[ii2].i + 1] - 1;
											bool bfound = false;
											for (integer is2 = ii2; (is2 <= iend_marker_position2); is2++) {
												if (Amat[is2].j != Amat[ii2].i) {
													if (ht[Amat[is2].j]) {
														bfound = true;
													}
												}
											}
											if (bfound) {

												//if (fabs(Amat[is0].aij) > maxelem_threshold*barjer) {
												// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
												// � ������� F ������.
												//if (fabs(Amat[is0].aij) > maxelem_threshold*theta_strong_F) {

												integer iFpoint = Amat[is0].j;
												if (fabs(SumPall) < 1.0e-33) {
													printf("error 2.0 ! division by zero. SumPall =%e\n", SumPall);
													//getchar();
													system("PAUSE");
													exit(1);
												}
												doublereal multiplyer_nu = fabs(Amat[is0].aij) / SumPall;
												// ��������� ���� ������� iFpointeger 
												// ����� ����� ����� ��� � ����.

												// �������������� ��������� �����.
												doublereal maxelem_threshold_loc = -1.0;
												//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
												integer ii1_loc = row_startA[iFpoint];
												integer istr_etalon = Amat[ii1_loc].i;
												integer iend_for = -1;
												integer iend_marker_position_loc = row_startA[istr_etalon + 1] - 1;
												for (integer is0_loc = ii1_loc; (is0_loc <= iend_marker_position_loc); is0_loc++) {
													iend_for = is0_loc;
													//if (fabs(Amat[is0_loc].aij) > maxelem_threshold_loc) {
														// ����� ��� threshold ������ ���� ��������� ������ �� � �����.
														// ����� ������ ����������.
														if (this_is_C_node[Amat[is0_loc].j] == true) {
															if (ht[Amat[is0_loc].j]) {
																if (Amat[is0_loc].j != istr_etalon) {
																	maxelem_threshold_loc = fabs(Amat[is0_loc].aij);
																}
															}
														}
													//}
												}

												doublereal maxelem_threshold_loc_magic = maxelem_threshold_loc * magic;
												// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

												// ����� ������� ������� F-node ������� C-node.
												integer icsos_loc = 0;

												// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
												// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
												doublereal sumP_loc = 0.0;
												//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat[is0_loc].i == Amat[ii1_loc].i); is0_loc++) {
												for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

													// ����� �������� ����� ���������� ����� ���������.
													//if (fabs(Amat[is0_loc].aij) > maxelem_threshold_loc_magic) {

													if (ht[Amat[is0_loc].j]) {
														if (this_is_C_node[Amat[is0_loc].j] == true) {

															if (Amat[is0_loc].j != istr_etalon) {

																sumP_loc += fabs(Amat[is0_loc].aij); // ����� ������� ��������������� ��������� ������� ����������� � �����.
																icsos_loc++;
															}

														}
														else {

															//if (Amat[is0_loc].j != istr_etalon) {
															// ������������ ���������� ������� ������� �� �������� � ������.
															//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
															//}
														}
													}

													//}
												}

												doublereal maxelem_threshold_loc_magic_minus = -maxelem_threshold_loc_magic;

												// � ����� ��� ������� ����������������� ����� 
												//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat[is0_loc].i == Amat[ii1_loc].i); is0_loc++) {
												for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

													// ��� �������������� ��������� � ������ ������� ���� �������� ���������� ����� ���������.


													// ��������������� ������� �� ��������� � �����.

													// ������ ������� ������ ����������� ��������� 
													// ������������� ��������� �������� �� �������� 
													// �������� �������.
													// ����������� 5 ������� 2015.
													//if (fabs(Amat[is0_loc].aij) > maxelem_threshold_loc_magic) {
													if (ht[Amat[is0_loc].j]) {

														if (this_is_C_node[Amat[is0_loc].j] == true) {
															if (Amat[is0_loc].j != istr_etalon) {

																P[icount1].j = i8;
																P[icount1].i = C_numerate[Amat[is0_loc].j];
																//P[icount1].aij = fabs(Amat[is0].aij) / sumP;
																if (fabs(sumP_loc) < 1.0e-33) {
																	printf("error 3.0 ! division by zero. sumP_loc =%e\n", sumP_loc);
																	//getchar();
																	system("PAUSE");
																	exit(1);
																}
																P[icount1].aij = multiplyer_nu * fabs(Amat[is0_loc].aij) / sumP_loc;
																icount1++;
																if (icount1 >= nsizePR * n) {
																	printf("memory error!!!\n");
																	printf("not enough memory for the interpolation operator.\n");
																	//system("PAUSE");
																	//exit(1);
																	deallocate_prolongation(nsizePR, n, R, P);
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

				}

				for (integer is0 = ii1; (is0 <= row_startA[Amat[ii1].i + 1] - 1); is0++) {
					if (Amat[is0].j != Amat[ii1].i) {
						if (this_is_C_node[Amat[is0].j] == true) {
							if (fabs(Amat[is0].aij) > maxelem_threshold_theta) {
								ht[Amat[is0].j] = false;
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
				integer istr_etalon1 = Amat[ii1].i;
				integer iend_for1 = -1;
				if (!btreshold_on_new_vetv) {
					for (integer is0 = ii1; (is0 <= row_startA[istr_etalon1 + 1] - 1); is0++) {
						iend_for1 = is0;
						if (Amat[is0].j != istr_etalon1) {
							// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
							//if (this_is_C_node[Amat[is0].j] == true) {
							if ((Amat[is0].aij < 0.0) && (fabs(Amat[is0].aij) > maxelem_threshold)) {
								maxelem_threshold = fabs(Amat[is0].aij);
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

				doublereal maxelem_threshold_theta = maxelem_threshold * theta;
				doublereal maxelem_threshold_theta_strong_F = maxelem_threshold * theta_strong_F;


				for (integer is0 = ii1; (is0 <= row_startA[Amat[ii1].i + 1] - 1); is0++) {
					if (Amat[is0].j != Amat[ii1].i) {
						if (this_is_C_node[Amat[is0].j] == true) {
							if ((Amat[is0].aij<0.0) && (fabs(Amat[is0].aij) > maxelem_threshold_theta)) {
								ht[Amat[is0].j] = true;
							}
						}
					}
				}


				// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
				// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
				doublereal sumP = 0.0;
				doublereal SumPall = 0.0;
				integer icount_StronglyF = 0;
				//	for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[ii1].i); is0++) {
				for (integer is0 = ii1; is0 <= iend_for1; is0++) {
					if (Amat[is0].j != istr_etalon1) {
						if (this_is_C_node[Amat[is0].j] == true) {
							//	if (fabs(Amat[is0].aij) > maxelem_threshold*barjer) {
							//if (fabs(Amat[is0].aij) > maxelem_threshold*theta) {
							if ((Amat[is0].aij<0.0) && (fabs(Amat[is0].aij) > maxelem_threshold_theta)) {
								sumP += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� Strongly � �����.
								icsos++;
							}
						}
						else {
							if (this_is_F_node[Amat[is0].j] == true) {
								//if (fabs(Amat[is0].aij) > maxelem_threshold*theta_strong_F) {
								if ((Amat[is0].aij<0.0) && (fabs(Amat[is0].aij) > maxelem_threshold_theta_strong_F)) {
									
									integer ii2 = row_startA[Amat[is0].j];
									integer iend_marker_position2 = row_startA[Amat[ii2].i + 1] - 1;
									bool bfound = false;
									for (integer is2 = ii2; (is2 <= iend_marker_position2); is2++) {
										if (Amat[is2].j != Amat[ii2].i) {
											if (ht[Amat[is2].j]) {
												bfound = true;
											}
										}
									}
									if (bfound) {


										SumPall += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� Strongly F �����.
										icount_StronglyF++;
										icsosF++;
									}
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



				{

					if (1) {
						//if (((icsos == 1) || (icsos == 2) || (icsos == 3)) && (icsosF != 0)) {
						// ������ ������ Strong C ������ � ������� � �������� ���� ��������� ���� ���� Strong F �����.
						//
						SumPall += sumP;

						for (integer is0 = ii1; (is0 <= row_startA[Amat[ii1].i + 1] - 1); is0++) {
							if (Amat[is0].j != Amat[ii1].i) {
								// ��� ���������� ������ Strong �����.


								if (this_is_C_node[Amat[is0].j] == true) {
									//if (fabs(Amat[is0].aij) > maxelem_threshold*theta) {
									if ((Amat[is0].aij<0.0) && (fabs(Amat[is0].aij) > maxelem_threshold_theta)) {
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
											//if (fabs(Amat[is0].aij) > maxelem_threshold*barjer) {
											//if (fabs(Amat[is0].aij) > maxelem_threshold*theta) {
											if ((Amat[is0].aij<0.0) && (fabs(Amat[is0].aij) > maxelem_threshold_theta)) {
												P[icount1].j = i8;
												P[icount1].i = C_numerate[Amat[is0].j];
												//P[icount1].aij = fabs(Amat[is0].aij) / sumP;
												if (fabs(SumPall) < 1.0e-23) {
													printf("error 5.0 ! division by zero. SumPall =%e\n", SumPall);
													//getchar();
													system("PAUSE");
													exit(1);
												}
												P[icount1].aij = fabs(Amat[is0].aij) / SumPall;
												icount1++;
												if (icount1 >= nsizePR * n) {
													printf("memory error!!!\n");
													printf("not enough memory for the interpolation operator.\n");
													//system("PAUSE");
													//exit(1);
													deallocate_prolongation(nsizePR, n, R, P);
												}
											}

										}
									}

								}
								else
									if (this_is_F_node[Amat[is0].j] == true) {
										//if (fabs(Amat[is0].aij) > maxelem_threshold*theta_strong_F) {
										if ((Amat[is0].aij<0.0) && (fabs(Amat[is0].aij) > maxelem_threshold_theta_strong_F)) {
											// ������������� Strong F �����.

											// ����� :
											// 



											//if (fabs(Amat[is0].aij) > maxelem_threshold*barjer) {
											// ��� ������ �������, ����� ��� ���� ��������� ��� �� ����� ����
											// � ������� F ������.
											//if (fabs(Amat[is0].aij) > maxelem_threshold*theta_strong_F) {

											integer iFpoint = Amat[is0].j;
											doublereal multiplyer_nu = fabs(Amat[is0].aij) / SumPall;
											// ��������� ���� ������� iFpointeger 
											// ����� ����� ����� ��� � ����.

											// �������������� ��������� �����.
											doublereal maxelem_threshold_loc = -1.0;
											//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
											integer ii1_loc = row_startA[iFpoint];
											integer istr_etalon = Amat[ii1_loc].i;
											integer iend_for = -1;
											integer iend_marker_position = row_startA[istr_etalon + 1] - 1;
											for (integer is0_loc = ii1_loc; (is0_loc <= iend_marker_position); is0_loc++) {
												iend_for = is0_loc;
												//if ((Amat[is0_loc].aij<0.0) && (fabs(Amat[is0_loc].aij) > maxelem_threshold_loc)) {
													if (this_is_C_node[Amat[is0_loc].j] == true) {
														if (ht[Amat[is0_loc].j]) {
															if (Amat[is0_loc].j != istr_etalon) {
																maxelem_threshold_loc = fabs(Amat[is0_loc].aij);
															}
														}
													}
												//}
											}

											doublereal maxelem_threshold_loc_magic = maxelem_threshold_loc * magic;
											// ����� maxelem_threshold_loc ��� ������ ������������� ���������������� �������� � ������ ����� � ������� ��������.

											// ����� ������� ������� F-node ������� C-node.
											integer icsos_loc = 0;

											// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
											// ��� ������� ������ ����� ����� ���������� ������� iscos_loc. �� ���� iscos_loc ������ ���� 2 � �����.
											doublereal sumP_loc = 0.0;
											//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat[is0_loc].i == Amat[ii1_loc].i); is0_loc++) {
											for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

												// ����� �������� ����� ���������� ����� ���������.
												//if ((Amat[is0_loc].aij<0.0) && (fabs(Amat[is0_loc].aij) > maxelem_threshold_loc_magic)) {
												if (ht[Amat[is0_loc].j]) {


													if (this_is_C_node[Amat[is0_loc].j] == true) {

														if (Amat[is0_loc].j != istr_etalon) {

															sumP_loc += fabs(Amat[is0_loc].aij); // ����� ������� ��������������� ��������� ������� ����������� � �����.
															icsos_loc++;
														}

													}
													else {

														//if (Amat[is0_loc].j != istr_etalon) {
														// ������������ ���������� ������� ������� �� �������� � ������.
														//the_number_of_neighbors_that_are_not_�_nodes_loc++; // ������������ �������� ������������ 
														//}
													}
												}
												//}
											}

											doublereal maxelem_threshold_loc_magic_minus = -maxelem_threshold_loc_magic;

											// � ����� ��� ������� ����������������� ����� 
											//for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat[is0_loc].i == Amat[ii1_loc].i); is0_loc++) {
											for (integer is0_loc = ii1_loc; is0_loc <= iend_for; is0_loc++) {

												// ��� �������������� ��������� � ������ ������� ���� �������� ���������� ����� ���������.


												// ��������������� ������� �� ��������� � �����.

												// ������ ������� ������ ����������� ��������� 
												// ������������� ��������� �������� �� �������� 
												// �������� �������.
												// ����������� 5 ������� 2015.
												//if ((Amat[is0_loc].aij<0.0) && (fabs(Amat[is0_loc].aij) > maxelem_threshold_loc_magic)) {
												if (ht[Amat[is0_loc].j]) {


													if (this_is_C_node[Amat[is0_loc].j] == true) {
														if (Amat[is0_loc].j != istr_etalon) {

															P[icount1].j = i8;
															P[icount1].i = C_numerate[Amat[is0_loc].j];
															//P[icount1].aij = fabs(Amat[is0].aij) / sumP;
															if (fabs(sumP_loc) < 1.0e-23) {
																printf("error 6.0 ! division by zero. sumP_loc =%e\n", sumP_loc);
																//getchar();
																system("PAUSE");
																exit(1);
															}
															P[icount1].aij = multiplyer_nu * fabs(Amat[is0_loc].aij) / sumP_loc;
															icount1++;
															if (icount1 >= nsizePR * n) {
																printf("memory error!!!\n");
																printf("not enough memory for the interpolation operator.\n");
																//system("PAUSE");
																//exit(1);
																deallocate_prolongation(nsizePR, n, R, P);
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

				for (integer is0 = ii1; (is0 <= row_startA[Amat[ii1].i + 1] - 1); is0++) {
					if (Amat[is0].j != Amat[ii1].i) {
						if (this_is_C_node[Amat[is0].j] == true) {
							if ((Amat[is0].aij<0.0) && (fabs(Amat[is0].aij) > maxelem_threshold_theta)) {
								ht[Amat[is0].j] = false;
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

	delete[] ht;
	ht = NULL;

} // my_interpolation_procedure_number3B