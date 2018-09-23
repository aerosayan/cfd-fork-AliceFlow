// classic_aglomerative_amg6_2018year.cpp
// ��������� �� ������� ���� �������������� ������������������ �����_v0_14.
// ��� �������� ������������ ���������� � �����������.

// ������ ��� ���� �����.
// ������ ��� ���� ����� ������ ���������� 30.06.2018 - ��������� 21.09.2018
// ��� �������� �� log(log(U))
#include "veb.h"
#include "veb.cpp"

// 29.07.2018 - xx.xx.xxxx ������ 6 �� ������ ������ 4.
// 25.04.2018 ������ ������ classic_aglomerative_amg4 ��� �������� �������������� ������.
// ����� ������ classic_aglomerative_amg5 ����� �� �������������� (����������).
// ���� 2017 - �������� �����-����� smoother, �������� ��������� ilu0 ���������� � ���������. 
// ���� 2017 - �������������� ������������ ���������� ������� ����������� 100 � �����. 
// ������ �������� 2016-2017 ���� - �������� bicgStab.
// ���� 2017 - �������������� ������������� ����� ������ �� ���� � ���� ������������ ���
// �������������������
// � ��������� ����� ��� ��� ������ BiCGStab. ��� ������ ���������� ����� ���������� �
// �������� ������ ��� ������ �������� amg.
// 4-6 ������ 2016. �������� ILU0. ��������� ����� ���������� ��� �� Solution Phase.
// 9 ������� 2016. ������� �� ����������� � �������� ������������� ��������� ������ ���� 
// � ������� ������� amg,
// ��� �� ����������� � �� ������������ amg1r5. 9 ������� ������ ��������� ������������ 
// ������ � ������� 
// �� ������ ������ ����������� � ������� ILU2 ������������. ��� ������������ ������ 
// �.�.��������, �.�.��������
// "������������� ����� ������� ������ �������������� ������" ������ ���, ������-��-����,
// ������. ��� ���
// ���������� ������������ ����������� �� ������ ������� ��� ����� � ������������ ������������
/// �������� �
// ����������� �������� ������� �� ���2 ����� (����������� ���������������� �����). � ������ 
//��������� � ��� ���� 
// �������� ���� ������������� ILU2 ������������������� �� ���������� SPARSKIT2 �.����� 
// ������� ������ ���2 � ��� 
// ����� ILU2.
// 22 ������ 2016 ������� ��������������� ������� ����.
// ����� : 1. ������� ������ amg3. 
// � ��� : 2. �������� ��� �������� �� ����������� ����������� ������ �� ������������� 
// ������� ������ ��� �����.
// ��� �������� ������� �������� ����������� ���.
// � ��� 3. ��� V ����� �������� � ���� ����� for. ����� �� ����� ����� ����������� ��������
// direct ����� ��� ������ ������� ������.
// ��� �� ������� ����������� ������� �� amg ��������� ������������������� ��� BiCGStab.
// 15 ������ 2016 �������� ������ ��������� �� Ak1.
// 10 ������ 2016 �������� ����� ������ �� �����������.
// 15 ������� 2015. ������ ������ ���� ����� ��������� ������� �� ����������� ����.
// 13 ������� 2015. �������� ��� ������. ��� ��������� ��� ������ ���������� ���������� ������ �
// ���������� �-F ���������, ������ C-F ��������� �������� ���������.
// ��������� � ������ quicksort (qs,qsj)
// ������� � ���� ��� ������� ������������� ����������.
// ������ ����� �� band_size!!!.
// ����� ������ ��������� �� 1.7� ����������� � 3D ��������� ����� 1 ������.
// 18 ������� 2015. ��������� ��������������� ����������.
// ������������� �� �������� ������� �� ������ �������� �� ����� 
// ������� �������. 18 ������� 2015 ���������� ������ 0.04. ������ 0.04 �� �����
// ������� ������ 0_03. ���� �������� ��� �������� ���������� C-F ���������, 
// ��� � ���������� ��������� ��������. ��� ���������� �-F ��������� 
// ����������� ��� ������������ ��� ����� � ������� ����� ������������ ��
// �� ������� ������ ����������� ��������� ������ �� ����������� �����.
// ��� ���������� ����������� �������� �������� ����� ����������� �� 
// ������������� ������,
// ���������� �� ��������� ������� ��������������� �������.
// 4 ������� 2015 ���������� ���������� ������������������ ��������� ������.
// 30 �������� 2015 ���������� ����������� ������. ������ ������������ 
// �������������� ������������� ����� �� ������  C-F ���������.
// 16 �������� 2015 ���� ���������� ��� �������� 
// ���������� � ������������ ������� ���������� �������,
// � ���� ���������� ��� � �����-�� ���� ���������� �� ������������ ������ �������.
// �������� ���������� � ������������� ����� ������� ������ �� ������ ������ 
// �.�. ������� � ����� ������ �������.
// 3 september 2015 Villa Borgese.
// ���������� divergence detected.
template <typename doublerealT>
bool classic_aglomerative_amg6(Ak1* &Amat,
	integer nsizeA, // ���������� ����� ���������� ����� ��� ��������� ������ �
	integer &nsizePR, // ������ ��� P � ����������� n.
	integer nnz, // number of non zero elements
	integer n, // dimension of vectors x and b.
	Ak1* &R, // restriction
	Ak1* &P, // prolongation
	doublereal* &x, doublereal* &b,
	doublerealT &theta, doublerealT &theta83,
	doublerealT &magic82, doublerealT &magic83,
	doublerealT &ret74, integer iVar,
	bool bmemory_savings,
	BLOCK* &my_body, integer &lb, integer maxelm_out
) {


	
	//  ���������� ��� �������.
	construct_hash_table_Gus_struct01(n);
	

	// 23.12.2016 ��������� ����� ���������� ����� :
	// �������� ������ ����������� ����� ������� V �����,
	// ��� ����� ������ ��������� 
	// b � lb.

	integer iaddFCcolor = 0;
	integer nsize;
	integer istart4;
	integer iend4;
	integer* row_ind_PE = NULL;
	integer* row_ind_PS = NULL;
	integer istart2;
	integer iend2;
	integer* row_ind_AS = NULL;
	integer* row_ind_AE = NULL;
	integer istartAnew2;
	integer index_size = 0;
	integer* index_visit = NULL;
	doublerealT* vector_sum = NULL;
	integer istartAnew_mem;
	integer istart3;
	integer iend3;
	integer* row_ind_SA = NULL;
	integer* row_ind_EA = NULL;
	integer istart1;
	integer iend1;
	integer* row_ind_ER = NULL;
	integer* row_ind_SR = NULL;
	integer iend_marker_position;
	doublerealT* ap_coarse = NULL;
	integer icounter = 1;
	integer icount1;
	integer numberofcoarcenodes;
	integer* C_numerate = NULL;	
	bool bweSholdbeContinue = true;
	integer the_number_of_neighbors_that_are_not_�_nodes = 0;
	integer number_of_F_nodes_with_one_single_strong_C_neighbor = 0;
	integer number_of_F_nodes_with_one_single_strong_C_neighborF = 0;

	integer iadditionalCstatistic = 0;
	node_AVL_Gus* root_Gus_set = 0;

	integer newCcount = 0;

	node_AVL* root = 0;
	Tree_splay* root_splay = 0;
	size_splay_Tree = 0;
	TreapNode* random_tree_root = NULL;
	RBtree RBroot; // ������ ������-׸����� ������.

	integer istartflag_scan = 1;
	bool *bmarkervisit = NULL;

	integer n_coarce = 1; // ��������� ����� C ����.

	const integer NULL_SOSED = -1;
	integer vacant = NULL_SOSED;
	bool bcontinue = true;

	// ���������� C-F ���������.
	//while (icandidate != 0)
	integer icountprohod = 0;

	integer maxsosed = 0;
	integer icandidate = 0;

	integer* row_startA = NULL;
	integer* count_sosed = NULL;

	bool identiti = true;
	

	// ������� ���������� ������� ������������������ ����������� ���� : BiCGStab + camg(�����).
	// 1. ������������� ����������.
	// 2. ������������������.
	// 3. ������������.
	// ���� my_amg_manager.istabilization == 1 �� �� ���������� ����� ������������� ���������� �� ������������� � ������������������� 
	// ������������ �������������� ������������� ������� �����.
	// ������ ���������� 5.01.2017.(more robust).
	// ���� my_amg_manager.istabilization == 0 - �� ������ ������������ 
	// ������������� �������� ��� ������ ���� ������ ����������� ���������������.
	// ���� my_amg_manager.istabilization == 2 - �� ������������ fgmres - 
	// �������� ����� � ������ (������ ������� ����������� ������ ����������� �������) � ������� 
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
	// 0 - ILU �� ������������. ������������ Gaus-Seidel.
	// 1 - ILU0 ������������.
	// 2 - ILU2 ������������.
	integer bILU2smoother = 0;
	if (my_amg_manager.ilu2_smoother == 1) {
		// �������� ILU0 ������������. 
		// �� ��� ������ ������ �� ����� ������ ��������.
		// ���� ������� ��� �� ��������� � ������� ����� ������������������� ���������.

		bILU2smoother = 1; // ILU0

						   // �� - �������� �������� 
						   // ilu0_(maxelm_plus_maxbound, milu0.val, milu0.col_ind, milu0.row_ptr, milu0.alu, milu0.jlu, milu0.ju, milu0.iw, ierr);
						   // �������� ���������. � �� ������� � ��� ���������� ��� �� �������. ���� �������� iluk � lfil=0 ������� ���� ��������� 
						   // ������� � � ��� ���������� � �������������. ��� ����������� � ����� ���� my_amg_manager.ilu2_smoother == 2.
						   // ������ iluk � lfil=0 �������� �� ���� ������� � ��������� ���� ��������.

						   // ���������������.
		bILU2smoother = 2; // ILU0
	}
	if (my_amg_manager.ilu2_smoother == 2) {
		// �������� ILU2 ������������. 
		// �� ��� ������ ������ �� ����� ������ ��������.

		// ��� ������������� ��������� ������ ��� �������� ������� - ������� ����.
		// ���� ��� ��������� �� ����� �������� ������� �� ���������� ���� �����������.

		bILU2smoother = 2; // ILU2

						   // ILU2 ��� ������� ����� ����������� ������ � � ��� ������� �� ILU0 ������������ �� ������ ������ : iluk � lfil=0.
						   // �������� � ��� ������� � ilu2 ������ �� ������� ������, �.�. ��� �� �������� �����.
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
	if (my_amg_manager.icoarseningtype >= 4) {
		// only negative connections 
		// ��������������� ������������� ����� ������������ ��� �������� C-F ���������.
		bpositive_connections_CF_decomp = false;
		my_amg_manager.icoarseningtype -= 4;
	}
	// 19.01.2016 ��� ���������� C-F ��������� � ������������� ������������ ������ ������
	// � ������� ������������� � �� ������������� positive connections.
	// ������� ���������� ��������� ������: 
	// 1. ��� ����������� ��������� ������������� ����� ��� ����� ��� ������� ��� � �������.
	// 2. ��� ���������� C-F ������������ ����� ������ ������� �����. 
	// ��� �������� ��������� �������������.
	// ���������� ����� bpositive_connections_CF_decomp ������������ ��� ���������� C-F ������������ � bpositive_connections
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

	// ������ 12mm hfet thermal resistance. 1.7��� �����������.
	// AVL_TREE_ID   3��� 29� 590��      {5}
	// SPLAY_TREE_ID  3��� 16� 430�� {2}
	// BINARY_HEAP 3��� 4� 0�� {1 *����� �������.}
	// RANDOM_TREE_ID (�������) 3��� 28� 90�� {4}
	// RED_BLACK_TREE_ID 3��� 27� 210�� {3}


	const integer AVL_TREE_ID = 0;   // ��� ������ ������. 12.12.2015.
	const integer SPLAY_TREE_ID = 1; // ��������� ������ ������.
	const integer BINARY_HEAP = 2; // �������� ����. 16.06.2017.
	const integer RANDOM_TREE_ID = 3; // (�������) ����������������� ������ ������. 24.08.2017.
	const integer RED_BLACK_TREE_ID = 4; // ������-׸���� ������ ������. 22.06.2018.
	const integer FIBONACCI_HEAP_ID = 5; // ������������ ����. 11.07.2018.
	const integer VAN_EMDE_BOAS_TREE_ID = 6; // ��� ���� ���� ������ ������. 30.06.2018
	//integer id_tree = BINARY_HEAP; // AVL_TREE_ID; // SPLAY_TREE_ID; // BINARY_HEAP; // RANDOM_TREE_ID; // RED_BLACK_TREE_ID;
	// 28.01.2018 �� ����� ������������.
	integer id_tree = my_amg_manager.iCFalgorithm_and_data_structure;

	// �������� ������ ��� �������� ����.
	// ���������� ���������� ��������� ��� ����� �� ������� ��������� ������� �����������.
	const integer isize_priority_queue01 = (integer)(0.4*n); // 0.238
	integer ikonst1 = isize_priority_queue01, ikonst2 = n;
	if (id_tree != BINARY_HEAP) {
		ikonst1 = 0;
		ikonst2 = 0;
	}
	PQ<integer> binary_heap(ikonst1, ikonst2); // 500K ��� 2.1M

	FibonacciHeap<integer> fibo_heap;

	if (id_tree == FIBONACCI_HEAP_ID) {
		fibo_heap.WakeUp2(n + 1);// alloc memory hash table
	}

	
	

	
	// ��� ���������� grid complexity ��������� �������������:
	integer nnz_P_memo_0 = 0;
	integer nnz_P_memo_all = 0;

	// ���� �������� ��� new �� malloc.
	//theta = 0.25;

	bool bonly_serial = true;
	

	

								   // �������� ����� ������� ������ ����� �����������.
								   // doublerealT theta = 0.25;  // 0.25 for 2D and 0.5 for 3D 


								   //const integer QUICK_SORT_ALG = 1; // ������� ���������� �����.
								   // ������������ �� quicksort qs and qsj.
								   // ���������� � ��������� ������� quickSort.
								   // ������������ �� ���������� ��������� ������� 
								   //��� ��������� ������ (������ ��� ����� � ������� ���� ����������).
								   //const integer COUNTING_SORT_ALG = 0; // ���������� � ��������� ������ �����.
								   // ���������� � �������� �������� ������ ��� ����� ������������ � 
								   // ����� � �������� ��������� ����������.
								   //const integer HEAP_SORT_ALG = 2; // ������������ ����������.
								   // ���������� ����������� ������� ����������, ������� QuickSort �� ��������.
								   // � ����������� ���� ��������� ������ ����� �� 4��.
								   //bmemory_savings =false ��� QUICK_SORT_ALG � HEAP_SORT_ALG;
	integer imy_sort_algorithm = my_amg_manager.imySortAlgorithm;// COUNTING_SORT_ALG;

	const doublereal RealZERO = 1.0e-300;// 1.0e-10;
	const doublereal divisionZERO = 1.0e-300;
	const doublereal RealMAXIMUM = 1.0e300;
	// ���������/���������� ����������� ������.
	bool debug_reshime = false;

	const integer max_sosed = 111400;
	// �� ����� ������������ ���������� � ������������ ���������� �������.
	integer Maximumsosedcount = -1;
	// �� ������� ���������� ����� �������� � ������������ ����� ������� � ������� �,
	// ����� � ���������� ���������� C-F ���������.
	bool bmaxsosedinfoactive = false;


	const integer maxlevel = 101; // (51 �� 5.06.2017) 30
	integer ilevel = 1;
	integer *n_a = new integer[maxlevel];
	integer *nnz_a = new integer[maxlevel];
	nnz_a[0] = nnz;
	n_a[0] = n;

	const int iKnumber_thread = 8;

	char c1[27] = "classic_aglomerative_amg_6";

	//#ifdef _NONAME_STUB29_10_2017
#ifdef _OPENMP 
	// ������ ������������ ��� ���������� ������������� �����.
	// 8 - ��� ����� �������.

	Ak1** AccumulqtorA_m = NULL;
	AccumulqtorA_m = new Ak1*[iKnumber_thread];
	doublerealT** vector_sum_m = NULL;
	vector_sum_m = new doublerealT*[iKnumber_thread];
	integer** index_visit_m = NULL;
	index_visit_m = new integer*[iKnumber_thread];
	bool** hash_table_m = new bool*[iKnumber_thread];
	integer* index_size_m = NULL;
	integer*  istartAnew_m = new integer[iKnumber_thread];
	index_size_m = new integer[iKnumber_thread];
	for (integer i_9 = 0; i_9 < iKnumber_thread; i_9++) {
		AccumulqtorA_m[i_9] = new Ak1[(integer)(0.125*4.55*nnz + 1)];
		//vector_sum_m[i_9] = new doublerealT[n_a[ilevel - 1] + 1];
		vector_sum_m[i_9] = (doublerealT*)malloc((n + 1) * sizeof(doublerealT));
		handle_error(vector_sum_m[i_9], "vector_sum_m[i_9]", "classic_aglomerative_amg_6", (n + 1));

		//index_visit_m[i_9] = new integer[n_a[ilevel - 1] + 1];
		index_visit_m[i_9] = (integer*)malloc((n + 1) * sizeof(integer));
		handle_error(index_visit_m[i_9], "index_visit_m[i_9]", "classic_aglomerative_amg_6", (n + 1));

		hash_table_m[i_9] = (bool*)malloc((10 * n + 1) * sizeof(bool));
		handle_error(hash_table_m[i_9], "hash_table_m[i_9]", "classic_aglomerative_amg_6", (10 * n + 1));

		for (integer i_91 = 0; i_91 < 10 * n + 1; i_91++) hash_table_m[i_9][i_91] = false;// inicialization
		index_size_m[i_9] = 0;
		istartAnew_m[i_9] = 0;
	}
#endif

	doublerealT* threshold_quick_all = NULL;
	threshold_quick_all = (doublerealT*)malloc((n + 1) * sizeof(doublerealT));
	handle_error(threshold_quick_all, "threshold_quick_all", "classic_aglomerative_amg_6", (n + 1));

	doublerealT* threshold_quick_only_negative = NULL;
	threshold_quick_only_negative = (doublerealT*)malloc((n + 1) * sizeof(doublerealT));
	handle_error(threshold_quick_only_negative, "threshold_quick_only_negative", "classic_aglomerative_amg_6", (n + 1));
	bool btreshold_on_new_vetv = true; // false ����� ��������� ����� �� ������ ���������� ����� ����.


									  
	bool* flag = NULL;
	flag = (bool*)malloc((n + 1) * sizeof(bool));
	handle_error(flag, "flag", "classic_aglomerative_amg_6", (n + 1));

	//bool* flag_shadow = new bool[n + 1];
	bool* flag_shadow = NULL;
	flag_shadow = (bool*)malloc((n + 1) * sizeof(bool));
	handle_error(flag_shadow, "flag_shadow", "classic_aglomerative_amg_6", (n + 1));

	//bool* hash_table = new bool[nnz + 1]; // ��������� ������� hash �������.
	bool* hash_table = NULL;
	hash_table = (bool*)malloc((nnz + 1) * sizeof(bool));
	handle_error(hash_table, "hash_table", "classic_aglomerative_amg_6", (nnz + 1));

	// �������� ������ ������� ������������� �������� ���� ��������.
#pragma omp parallel for
	for (integer isc = 0; isc <= nnz; isc++) hash_table[isc] = false; // initialization 
	const integer istack_size_limit = n + 1; // 128000
											 //integer ipool[ipool_size_limit];
	integer* istack = NULL;
	istack = (integer*)malloc((istack_size_limit) * sizeof(integer));
	handle_error(istack, "istack", "classic_aglomerative_amg_6", (istack_size_limit));

	// ��� ���������� C-F ������������ ��� ���� ����������� ��� �������
	// � ���� ��� ������� ��� �������.
	bool* hash_table2 = NULL;
	hash_table2 = (bool*)malloc((n + 1) * sizeof(bool));
	handle_error(hash_table2, "hash_table2", "classic_aglomerative_amg_6", (n + 1));

	// �������������.
#pragma omp parallel for
	for (integer isc = 0; isc <= n; isc++) hash_table2[isc] = false;
	// � ������ ���� ��� ������� ��� �������.
	integer* istack2 = NULL;
	//istack2 = new integer[n + 1];
	istack2 = (integer*)malloc((n + 1) * sizeof(integer));
	handle_error(istack2, "istack2", "classic_aglomerative_amg_6", (n + 1));


	integer iadd = 0;
	integer nnzR = 1;
	integer iaddR = 0;
	integer nnz_aRP[maxlevel];
	bool bcontinue_global = true;
	
	bool* this_is_C_node = NULL;
	this_is_C_node = (bool*)malloc((n + 1) * sizeof(bool));
	handle_error(this_is_C_node, "this_is_C_node", "classic_aglomerative_amg_6", (n + 1));

	bool* this_is_F_node = NULL;
	this_is_F_node = (bool*)malloc((n + 1) * sizeof(bool));
	handle_error(this_is_F_node, "this_is_F_node", "classic_aglomerative_amg_6", (n + 1));

	const integer isize_row_ptr_1 = 4 * n + 1;
	bool* F_false_C_true = false;
	F_false_C_true = (bool*)malloc((isize_row_ptr_1) * sizeof(bool));
	handle_error(F_false_C_true, "F_false_C_true", "classic_aglomerative_amg_6", (isize_row_ptr_1));

	for (integer i_1 = 0; i_1 < isize_row_ptr_1; i_1++) F_false_C_true[i_1] = false; // �������������.

	bool bStrongTransposeON = true; // ��� � ���������� ���������� Strong Transpose.
	if (my_amg_manager.icoarseningtype == 0) {
		bStrongTransposeON = false;
	}
	
	node_AVLST** hash_StrongTranspose_collection = NULL;
	Taccumulqtor_list** hash_StrongTranspose_collection1 = NULL;
	integer isize_memory_alloc_hash_StrongTranspose_collection1 = -1;
	integer *isize_hash_StrongTranspose_collection = NULL;

	while ((ilevel < maxlevel - 1) && (n_a[ilevel - 1] > 50) && (bcontinue_global)) {


		RBroot.Clear();
		if (id_tree == FIBONACCI_HEAP_ID) {
			fibo_heap.UpdateSize(n_a[ilevel - 1] + 1);
		}

		// ������ �� ���������� ������������ �� ���������� � �������������.
		from_re_operation_protection0 = true;
		ifrom_re_operation_protection = 0;		


		Maximumsosedcount = -1;
		bmaxsosedinfoactive = false;		

		if (ilevel > 1) {
			doublerealT procent = (100.0*(abs(n_a[ilevel - 1] - n_a[ilevel - 2]))) / (1.0*n_a[ilevel - 2]);
			if (procent<2.0) break;
		}

		if (((ilevel > 1) && (nnz_a[ilevel - 1] > nnz_a[ilevel - 2]))) {
			//break;
		}

		// 19.04.2018
		print_control_volume_statistics(n_a, nnz_a, ilevel, bprint_mesage_diagnostic, debug_reshime);

		//nnzR = 1;

#pragma omp parallel for
		for (integer ii = 1; ii <= n_a[ilevel - 1]; ii++) {
			this_is_C_node[ii] = false;
			this_is_F_node[ii] = false;
		}

#pragma omp parallel for
		for (integer ii = n_a[ilevel - 1] + 1; ii <= n; ii++) {
#if doubleintprecision == 1
			//printf("warning: n!=n_a[ilevel-1] : n=%lld, n_a=%lld, ilevel=%lld\n", n, n_a[ilevel - 1], ilevel);
#else
			//printf("warning: n!=n_a[ilevel-1] : n=%d, n_a=%d, ilevel=%d\n", n, n_a[ilevel - 1], ilevel);
#endif

			//getchar();
			this_is_C_node[ii] = false;
			this_is_F_node[ii] = false;
		}

		// ���������� ����� ���� �� ������ ������, �.�.
		// ��������� ��������� ������������ �� ���������� ��� 
		// ��� �� ������ ��������������� �� ������� �������.
		if (ilevel == 1) {
			// ���������� ��������  �  �� i.
			//heapsort(Amat, key=i*n_a[ilevel - 1] + j, 1, nnz_a[ilevel - 1]);

			// 7 ������ 2016. ����������� ����� ��� ����������.
			switch (imy_sort_algorithm) {
			case COUNTING_SORT_ALG:
				Counting_Sort(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd, bmemory_savings);
				break;
			case HEAP_SORT_ALG:
				HeapSort(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd);
				break;
			case QUICK_SORT_ALG:
				// quicksort
				qs(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd);
				// ������������ ��������. O(nlog(n)).
				// �� ���������� ������ ������.
				//std::sort(Amat + (1 + iadd) * sizeof(Ak1), Amat + (nnz_a[ilevel - 1] + iadd + 1) * sizeof(Ak1), compAi);

				//QuickSort(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd);
				break;
			default:
				Counting_Sort(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd, bmemory_savings);
				break;
			}

		} // ilevel == 1


		if (my_amg_manager.bMatrixPortrait == 1) {
			// ������ �������� �������.

			FILE* fp_portrait;
			errno_t err_portrait;
			err_portrait = fopen_s(&fp_portrait, "matrix_load.txt", "w");
			fprintf_s(fp_portrait, "%d %d\n", n_a[ilevel - 1], nnz_a[ilevel - 1]);
			for (integer i58 = 1 + iadd; i58 <= nnz_a[ilevel - 1] + iadd; i58++) {
				fprintf_s(fp_portrait, "%d %d\n", Amat[i58].i, Amat[i58].j);
			}
			fclose(fp_portrait);
			printf("matrix portrait in level export\n");
			getchar();
		}



		

#pragma omp parallel for
		for (integer i_1 = 1; i_1 <= n; i_1++) {
			flag[i_1] = false;
		}

#pragma omp parallel for
		for (integer ii = 1; ii <= n_a[ilevel - 1]; ii++) {
			flag[ii] = false;
		}

		// ������� ������ ������ ������ � �������.
		row_startA = NULL;
		//row_startA = new integer[n_a[ilevel - 1] + 1];
		row_startA = (integer*)malloc((n_a[ilevel - 1] + 2) * sizeof(integer));
		handle_error(row_startA, "row_startA", "classic_aglomerative_amg_6", (n_a[ilevel - 1] + 2));

		for (integer ii = 1 + iadd; ii <= nnz_a[ilevel - 1] + iadd; ii++) {
			if (flag[Amat[ii].i] == false) {
				flag[Amat[ii].i] = true;
				row_startA[Amat[ii].i] = ii;
			}
		}
		row_startA[n_a[ilevel - 1] + 1] = nnz_a[ilevel - 1] + iadd + 1; // �������� �� ��������� �������.

#pragma omp parallel for
		for (integer ii = 1; ii <= n_a[ilevel - 1]; ii++) {
			flag[ii] = false;
		}


		// ��������� ��� ������ ���� ����� ��� �������.
		count_sosed = NULL;
		//count_sosed = new integer[n_a[ilevel - 1] + 1];
		count_sosed = (integer*)malloc((n_a[ilevel - 1] + 1) * sizeof(integer));
		handle_error(count_sosed, "count_sosed", "classic_aglomerative_amg_6", (n_a[ilevel - 1] + 1));

#pragma omp parallel for
		for (integer ii = 1; ii <= n_a[ilevel - 1]; ii++) {
			count_sosed[ii] = 0; // ��� �������.
		}
		

		if (bStrongTransposeON) {
			// ������������ ���.
			
			
				// ��� ����� ������� ��� ��� �������� �������.

				// ������� ���������� - �������� ������ � ������� ��������.
				if (hash_StrongTranspose_collection1 != NULL) {
#pragma omp parallel for
					//for (integer i_1 = 0; i_1 <= n_a[ilevel - 2]; i_1++)
					//isize_memory_alloc_hash_StrongTranspose_collection1
					for (integer i_1 = 0; i_1 <= isize_memory_alloc_hash_StrongTranspose_collection1; i_1++)
					{
						clear_list(hash_StrongTranspose_collection1[i_1]);
					}
					delete[] hash_StrongTranspose_collection1;
					hash_StrongTranspose_collection1 = NULL;
				}
				if (isize_hash_StrongTranspose_collection != NULL) {
					delete[] isize_hash_StrongTranspose_collection;
					isize_hash_StrongTranspose_collection = NULL;
				}
				// �������� ������ ��� ��� �������� ���������������� �������.
				hash_StrongTranspose_collection1 = new Taccumulqtor_list*[n_a[ilevel - 1] + 1];
				isize_memory_alloc_hash_StrongTranspose_collection1 = n_a[ilevel - 1];
				isize_hash_StrongTranspose_collection = new integer[n_a[ilevel - 1] + 1];
#pragma omp parallel for
				for (integer i_1 = 0; i_1 <= n_a[ilevel - 1]; i_1++) {
					hash_StrongTranspose_collection1[i_1] = NULL;
					isize_hash_StrongTranspose_collection[i_1] = 0;
				}
			
		}

		// ��� ����� ���� ���� ������� ���� ����� ������, ����� ��� 
		// ���������� ���� ������� ������ � ���� ����� �������.
		// ������� ��������� �� ����� ������ ������� � (����� �����).
		for (integer ii = 1 + iadd; ii <= nnz_a[ilevel - 1] + iadd; ii++) {
			if (flag[Amat[ii].i] == false) {
				integer ic = -1;
				//integer cand[max_sosed];
				node_AVL_Gus* root_Gus_cand = 0;
				
				
					
						// �������� ����� ����: 11.06.2017.
						// �������� ����� ����� ������� �������� �������� ��� ������� ��������� �������� ���������� threshold.
						// �� � ���� ������ �� ������� 0 � if.
						// ��� ����� ���������� ������ �����. Ÿ �������� �������� � ������������������� ����� ����������.
						threshold_quick_all[Amat[ii].i] = -1.0;
						threshold_quick_only_negative[Amat[ii].i] = -1.0;
						for (integer is0 = ii; (is0 <= row_startA[Amat[ii].i + 1] - 1); is0++) {
							if (Amat[is0].j != Amat[ii].i) {
								if (fabs(Amat[is0].aij) > threshold_quick_all[Amat[ii].i]) {
									// ���������� ������������ ��������������� �������.
									threshold_quick_all[Amat[ii].i] = fabs(Amat[is0].aij);
								}
							}
						}
						for (integer is0 = ii; (is0 <= row_startA[Amat[ii].i + 1] - 1); is0++) {
							if (Amat[is0].j != Amat[ii].i) {
								if (Amat[is0].aij < 0.0) {
									if (fabs(Amat[is0].aij) > threshold_quick_only_negative[Amat[ii].i]) {
										// ���������� ������������ ��������������� �������.
										threshold_quick_only_negative[Amat[ii].i] = fabs(Amat[is0].aij);
									}
								}
							}
						}
					
					
					if (bpositive_connections_CF_decomp) {
						//doublerealT theta_threshold3 = theta*threshold;
						doublerealT theta_threshold3 = theta*threshold_quick_all[Amat[ii].i];
						integer istopmarker3 = row_startA[Amat[ii].i + 1] - 1;
						for (integer is0 = ii; (is0 <= istopmarker3); is0++) {
							if (Amat[is0].j != Amat[ii].i) {
								if (fabs(Amat[is0].aij) > theta_threshold3) {
									// ��������� ������ ������ ��������� �������.
									ic++; //i,j
										  //cand[ic] = Amat[is0].j;
									
									insert_hash_table_Gus_struct01(Amat[is0].j);
									

									if (bStrongTransposeON) {
										data_BalTreeST d32;
										d32.i = Amat[ii].i;
										// O(1) ������� � ������ ��������� ������.
										insert_list(hash_StrongTranspose_collection1[Amat[is0].j], Amat[ii].i);
										isize_hash_StrongTranspose_collection[Amat[is0].j]++;
									}
								}
							}
						}
					}
					else {
						for (integer is0 = ii; (is0 <= row_startA[Amat[ii].i + 1] - 1); is0++) {
							if (Amat[is0].j != Amat[ii].i) {
								if (Amat[is0].aij < 0.0) {
									if (fabs(Amat[is0].aij) > theta*threshold_quick_only_negative[Amat[ii].i]) {
										// ��������� ������ ������ ��������� �������.
										ic++; //i,j
											  //cand[ic] = Amat[is0].j;
										
											insert_hash_table_Gus_struct01(Amat[is0].j);
										

										if (bStrongTransposeON) {
											data_BalTreeST d32;
											d32.i = Amat[ii].i;
											// O(1) ������� � ������ ��������� ������.
											insert_list(hash_StrongTranspose_collection1[Amat[is0].j], Amat[ii].i);
											isize_hash_StrongTranspose_collection[Amat[is0].j]++;
										}
									}
								}
							}
						}
					}
				
				integer len_sosed = ic;
				

				count_sosed[Amat[ii].i] = ic;
				// 22_12_2016
				if (ic == 0) {
					// ������� ������ ������� �� ��� ������ 8.������ 2017 ???

					// �� ������ ������ ��������� ��� ������� ������� ���������� F ������.
					this_is_C_node[Amat[ii].i] = false;
					this_is_F_node[Amat[ii].i] = true;

				}
				if (ic > Maximumsosedcount) {
					Maximumsosedcount = ic;
					bmaxsosedinfoactive = true;
				}
				flag[Amat[ii].i] = true;

				
				clear_hash_table_Gus_struct01();
				
			}
		}

		if (bStrongTransposeON) {
			// 5.01.2017. StrongTranspose.
			// ������� labda ��������������� �������� ������������ ��������� ����� Strong Transpose.
			for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) {

				// 20.05.2017 �������� ������� ������ �� ����� ��� ���������� ��������� � ������.
				//count_sosed[i_1] = getnumber_AVL_node_global(hash_StrongTranspose_collection[i_1]);
				count_sosed[i_1] = isize_hash_StrongTranspose_collection[i_1];
				if (count_sosed[i_1] == 0) {
					// 14.04.2017 ��������� ������������� ����������� 
					// ����������� ���������� ��������:
					// # ������; ����� ��. ��; ����� ��. �����;
					// 1. passiv_module6 (APPARAT); 179; 97;
					// 2. CGHV 12mm HFET; 18, 8, 6, 3, 2; 17, 8, 6, 3, 2;
					// 3. PIONER; 77; 73;

					// �� ������ ������ ��������� ��� ������� ������� ���������� F ������.
					this_is_C_node[i_1] = false;
					this_is_F_node[i_1] = true;

				}			
			}
		}


		maxsosed = 0;
		icandidate = 0;
		for (integer ii = 1; ii <= n_a[ilevel - 1]; ii++) {
			flag[ii] = false; // init flag
		}
		// ������� ���� � ���������� ������ ������� � ���������� ���.
		// ��� ������ ������������� ���� � ���������� ������ �������.
	
		for (integer i7 = 1; i7 <= n_a[ilevel - 1]; i7++) {
			if (count_sosed[i7] > maxsosed) {
				maxsosed = count_sosed[i7];
				icandidate = row_startA[i7];
				if (bmaxsosedinfoactive) {
					// ���������� ��������� ����� �� ����� for.
					// ��� ������ ������ ��������� ���������� ������������.
					if (maxsosed == Maximumsosedcount) break;
				}
			}
		}

		for (integer ii = 1; ii <= n_a[ilevel - 1]; ii++) {
			flag[ii] = false; // init flag
		}


		// ����� �������� ��� ����� � coarse, � ��� � ���� ��� ����� � Fine �������� ���� ��� �����������
		// � ������ Fine ��������� �� ������� �������.

		n_coarce = 1; // ��������� ����� C ����.
		nnzR = 1;


		vacant = NULL_SOSED;
		bcontinue = true;

		// ���������� C-F ���������.
		//while (icandidate != 0)
		icountprohod = 0;
		// �� ����� ��������� � ����� ������� ���������� ��� �� ���������� ����,
		// ��� �������� ���������� ������������ ��������� � ������ ���� � ������������ 
		// ����������� �������.
		//integer ibegining_start_index_found_maximum = 1 + iadd;
		// ������ �� ���� ������� ��� ���� �������� ��� ���������������.
		bmarkervisit = NULL;
		//bmarkervisit = new bool[n + 1];
		bmarkervisit = (bool*)malloc((n + 1) * sizeof(bool));
		handle_error(bmarkervisit, "bmarkervisit", "classic_aglomerative_amg_6", (n + 1));

		// �������� ��� ���� �������� ��� ������������.
		for (integer i_1 = 1; i_1 <= n; i_1++) bmarkervisit[i_1] = false;



		// ���������� �������������� ����������� 
		// ����������� �������� ������������
		// ����� �������� ������ ������ ������������ flag.
		istartflag_scan = 1;

		root = 0;
		root_splay = 0;
		size_splay_Tree = 0;
		random_tree_root = NULL;

		if (id_tree == BINARY_HEAP) {
			binary_heap.clear();
		}

		

		int64_t res_vanEMDE_BOAS_Tree;
		int64_t universe = 4294967296; // 2 ^32=2^(2^5) (4294 ���) ��������
		//int64_t universe = 67108864; // 2^26 �� ��������
		//int64_t universe = 134217728; // 2^27 �� ��������
		TvEB * vanEMDE_BOAS_Tree = NULL;

		if (id_tree == VAN_EMDE_BOAS_TREE_ID) {
			vanEMDE_BOAS_Tree = new TvEB(universe);
		}

		newCcount = 0;

		// 4 ���� 2016.
		// ��� ������ ����� ��������� ������� ����������� ������ �� �� ���� ������� � ��� 
		// ���������� ������� ������ �����.
		if ((icandidate == 0) && (maxsosed == 0)) {

			//getchar();
			// ������� ��������� ������ ������� ��������� ����� �� �����.
			break;
		}


		// �������� ��������� �������� � ���������� ������ � ������� �����, 
		// ����� ������ �������� � ���� ���. ��. ��������� ������ ��� set.
		// 23.04.2017

		
		root_Gus_set = 0;
		

		while (bcontinue)
		{

			integer ic = 0;
			integer ic_end_F_SiTranspose = 0;

			integer ii = icandidate;
			if (flag[Amat[ii].i] == false) {

				ic = 0; // ������������ �������������.
				

				ic_end_F_SiTranspose = 0;
				integer set0 = Amat[ii].i;




				//A20.05.2017//this_is_C_node[set[0]] = true;
				//A20.05.2017//bmarkervisit[set[0]] = true;
				this_is_C_node[set0] = true;
				bmarkervisit[set0] = true;

				doublerealT max_vnediagonal = -1.0; // ������������ �������� ������ ��� ������������� ��������. 
													// ��������� ������������ �������.
													// ���� set[0]==Amat[is0].i.
													// ���������� �������� ������������� �������������� ��������, � 
													// ������ ���� ��� ���� ���� ������� ������ � ����� ���������� ����� ��������� �������.
													// 17 ������ 2016 ���������� ����������� ������������� ���������������� ��������.
													// ������������ ��������� � ����� ������ ������.
				integer ii_back = ii;
				while ((ii_back > iadd) && (Amat[ii_back].i == set0)) ii_back--;
				ii_back++;

				doublerealT max_vnediagonal1 = -1.0e30;
				doublerealT min_vnediagonal1 = 1.0e30;
				doublerealT counter_vnediagonal = 0.0;
				doublerealT avg_vnediagonal1 = 0.0;

				// ���� ������ �� ������������� ���������������� �������� �� �� ������� ����� ����� ��������� �� ������ �������,
				// � ����������� ��������� ����������.

				if (bpositive_connections_CF_decomp) {
					// 23_10_2016
					for (integer is0 = ii_back; (is0 <= row_startA[set0 + 1] - 1); is0++) {
						if (Amat[is0].j != set0) {
							counter_vnediagonal = counter_vnediagonal + 1.0;
							avg_vnediagonal1 += fabs(Amat[is0].aij);
							if (fabs(Amat[is0].aij) > max_vnediagonal1) {
								max_vnediagonal1 = fabs(Amat[is0].aij); //i,j
																		// ������� ���������� ��������� �� ������ �������,
																		// ����� ��������� ����������.
																		//if (Amat[is0].j == set[0]) break; 
							}
							if (fabs(Amat[is0].aij) < min_vnediagonal1) {
								min_vnediagonal1 = fabs(Amat[is0].aij); //i,j

							}
						}
					}
				}
				else {
					for (integer is0 = ii_back; (is0 <= row_startA[set0 + 1] - 1); is0++) {
						if (Amat[is0].j != set0) {
							if (Amat[is0].aij < 0.0) {
								counter_vnediagonal = counter_vnediagonal + 1.0;
								avg_vnediagonal1 += fabs(Amat[is0].aij);
								if (fabs(Amat[is0].aij) > max_vnediagonal1) {
									max_vnediagonal1 = fabs(Amat[is0].aij); //i,j
																			// ������� ���������� ��������� �� ������ �������,
																			// ����� ��������� ����������.
																			//if (Amat[is0].j == set[0]) break; 
								}
								if (fabs(Amat[is0].aij) < min_vnediagonal1) {
									min_vnediagonal1 = fabs(Amat[is0].aij); //i,j

								}
							}
						}
					}
				}
				if (fabs(counter_vnediagonal) > 0.5) {
					avg_vnediagonal1 = avg_vnediagonal1 / counter_vnediagonal;
				}
				else {
					avg_vnediagonal1 = max_vnediagonal1;
				}
				//max_vnediagonal = avg_vnediagonal1;
				//max_vnediagonal = max_vnediagonal1;  // 1
				//max_vnediagonal = 0.5* avg_vnediagonal1 + 0.5*max_vnediagonal1;
				//max_vnediagonal = avg_vnediagonal1 + 0.05*(max_vnediagonal1 - avg_vnediagonal1);
				// �������� ������ � �����������. -85%. �� ���������� ����� max_vnediagonal = -1.0;
				//max_vnediagonal = avg_vnediagonal1 - 0.85*(avg_vnediagonal1 - min_vnediagonal1);
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
						formirate_F_SiTranspose_hash_table_Gus2_struct02(hash_StrongTranspose_collection1[Amat[ii].i], imarker75_scan, this_is_F_node, this_is_C_node);
						
						ic = imarker75_scan + 1;
					}

					ic_end_F_SiTranspose = ic; // � ���� ������� ������������� F ������� �� Si_Transpose.

											   // ���� ���� j ��� �� ��� �������� � �������.
					if (bpositive_connections_CF_decomp) {
						if (flag[Amat[ii].j] == false) {
							if ((Amat[ii].j != set0) && (fabs(Amat[ii].aij) >= theta*max_vnediagonal)) {
								// 21.05.2017
								bool bfound_vacant = false;
								
									bfound_vacant = isfound_hash_table_Gus_struct01(Amat[ii].j);
									if (!bfound_vacant) {
										insert_hash_table_Gus_struct01(Amat[ii].j);
										ic++;
									}						
								
							}
						}
					}
					else {
						if (flag[Amat[ii].j] == false) {
							if ((Amat[ii].j != set0) && (Amat[ii].aij < 0.0) && (fabs(Amat[ii].aij) >= theta*max_vnediagonal)) {
								// 21.05.2017
								bool bfound_vacant = false;
								
									bfound_vacant = isfound_hash_table_Gus_struct01(Amat[ii].j);
									if (!bfound_vacant) {
										insert_hash_table_Gus_struct01(Amat[ii].j);
										ic++;
									}	
								
							}
						}
					}

					//printf("sboi start");

					integer iscan = ii + 1;
					iscan = ii_back + 1; // ������ ����������� 19 ������ 2016�.
										 // TODO 19 jan 2016.

					if (bpositive_connections_CF_decomp) {
						while ((iscan <= nnz_a[ilevel - 1] + iadd) && (Amat[iscan].i == set0)) {
							// 14 ������� 2016 ��� ������ ���������� � ����.
							//while (iscan <= row_startA[set0 + 1] - 1) { // ��� ������ ���������� � ���� �� ���������� �������.
							// ���� ���� j ��� �� ��� �������� � �������.
							if (flag[Amat[iscan].j] == false) {
								if ((Amat[iscan].j != set0) && (fabs(Amat[iscan].aij) >= theta*max_vnediagonal)) {
									// 21.05.2017
									bool bfound_vacant = false;
									
										bfound_vacant = isfound_hash_table_Gus_struct01(Amat[iscan].j);
										if (!bfound_vacant) {
											insert_hash_table_Gus_struct01(Amat[iscan].j);
											ic++;
										}
									
									/*
									// ��������� ������ � �������� �������.
									vacant = Amat[iscan].j;
									for (integer js = 0; js < ic; js++) {
									if (vacant == set[js]) {
									vacant = NULL_SOSED;
									}
									}
									if (vacant != NULL_SOSED) {
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
						while ((iscan <= nnz_a[ilevel - 1] + iadd) && (Amat[iscan].i == set0)) {
							// 14 ������� 2016 ��� ������ ���������� � ����.
							//while (iscan <= row_startA[set0 + 1] - 1) { // ��� ������ ���������� � ���� �� ���������� �������.
							// ���� ���� j ��� �� ��� �������� � �������.
							if (flag[Amat[iscan].j] == false) {
								if ((Amat[iscan].j != set0) && (Amat[iscan].aij < 0.0) && (fabs(Amat[iscan].aij) >= theta*max_vnediagonal)) {
									// 21.05.2017
									bool bfound_vacant = false;
									
										bfound_vacant = isfound_hash_table_Gus_struct01(Amat[iscan].j);
										if (!bfound_vacant) {
											insert_hash_table_Gus_struct01(Amat[iscan].j);
											ic++;
										}
									
									/*
									vacant = Amat[iscan].j;
									for (integer js = 0; js < ic; js++) {
									if (vacant == set[js]) {
									vacant = NULL_SOSED;
									}
									}
									if (vacant != NULL_SOSED) {
									set[ic] = vacant;

									ic++;

									}
									*/
								}
							}

							iscan++;

						} // while
					}

					//printf("sboi end");
					// ��� ���� ������ ������ ����� i,j

				


				

				// � ���� ����� ��������� set ������� ������������:
				// 1. ����������� �� root_Gus_set � set.
				// 2. root_Gus_set ������ �� ������������.
				// 3. ������ ����� ���� �������� ������ ��� set.
				//integer* set = new integer[max_sosed];
				integer* set = NULL;
				set = new integer[ic + 2];
				if (set == NULL) {
					printf("error!!! memory for set is NULL. Problem allocate detected.\n");
					printf("in function classic_aglomerative_amg4.\n");
					system("pause");
					exit(1);
				}

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



				
					TreapNode* nrt_temp = NULL;
					TreapNode* save_root = NULL;

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
						ddel.countsosed = count_sosed[set[js]];
						// ���������� ���� ��� ������ ��� ���� �����.
						integer  veb_del_key = (count_sosed[set[js]])*(n_a[ilevel - 1] + 1) + (set[js]);
						if (veb_del_key > universe - 2) {
							printf("perepolnenie veb-Van Emde Boas 2^2^5\n");
							getchar();
						}
						if (veb_del_key < 1) {
							printf("perepolnenie veb-Van Emde Boas < 1\n");
							getchar();
						}

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
							save_root = NULL;
							if (nrt_temp != NULL) {
								nrt_temp = NULL;
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
							// ���� ������� ������������ �� �� ��� ������.
							res_vanEMDE_BOAS_Tree = vEB_find(vanEMDE_BOAS_Tree, veb_del_key);
							if (!res_vanEMDE_BOAS_Tree) {
								
							}
							else {
								res_vanEMDE_BOAS_Tree = vEB_delete(vanEMDE_BOAS_Tree, veb_del_key);
								if (!res_vanEMDE_BOAS_Tree) {
									printf("nevozmochno udalit post factum delete %d %d\n", ddel.countsosed, ddel.i);
									getchar();
								}
							}
							
							break;
						default: root = remove_AVL(root, ddel);
							break;
						}
					}
				


				 
					//printf("additional and modify new neighbour\n");

					// 10 ������ 2016. ����� ������.
					// �������� ��������� ��������� ����������� (��� ������ ������� �������� �� ��� ������).
					// ��� ����������� ��� ���������� ���������� V ������ ������� ��������� �� ����������
					// ��� ����������� �������� ������ � ������������ ��������� � ���������� ��� ��� ����������.
					// �� ������ 13 ������ 2016 ��� ������ �������� �� �������� ����������.
					integer itop_stack2 = 0;

					// 10 ������ 2016. ������ ������� ������ ��������� �� ����������� ����.
					for (integer js = 1; js < ic; js++) {

						integer i_11 = set[js];
						integer ii_11 = row_startA[i_11];
						integer iend2 = nnz_a[ilevel - 1] + iadd;
						integer istart3 = ii_11;
						//while ((istart3 >= 1 + iadd) && (Amat[istart3].i == Amat[ii_11].i)) istart3--;
						//istart3++;
						istart3 = row_startA[Amat[ii_11].i];
						bool bvisitsos = false;
						for (integer is0 = istart3; (is0 <= row_startA[Amat[ii_11].i + 1] - 1); is0++) {
							//for (integer is0 = istart3; (is0 <= iend2) && (Amat[is0].i == Amat[ii_11].i); is0++) {
							// � ����������� � U!!!
							if (flag[Amat[is0].j] == false) {


								integer isc = Amat[is0].j;

								
								
									// ����������� �� ��������� �������������.
									// � 2D �� ������������ ������� ��������� ������������� ����������
									// ����� 33%.
									// ��� ��� ����������� �������� ���������� �������� � �������, ��
									// �� ���� �������� ����� ��� ����� ������� ����������� ��������� �����
									// ����� (������������ �� ������, �� ������� ���� ������������ ������� �������� �������).
									// ��� �������� ���������� �� ������� :
									// CGHV1J006D, ��������� ����, ������������� ��������� � FET, Module 2.
									// ������ �������� ����������� ���������� ������� ������� ��-�� ������� ������� ������������.
									// �������� �� � ���� ����� ����.
									if (hash_table2[isc] == false) {
										hash_table2[isc] = true;
										istack2[itop_stack2] = isc;
										itop_stack2++;
										// ������������������ �����.
										//}
										
										//21_12_2016
										integer ii_2 = row_startA[isc];


										integer ic2 = 0;
										integer iend2loc = nnz_a[ilevel - 1] + iadd;
										integer istart2 = ii_2;										
										istart2 = row_startA[Amat[ii_2].i];
										integer istopmarker2 = row_startA[Amat[ii_2].i + 1] - 1;

											// 22 _12_2016
											// ��� ������ ������� : ������������ ���������� ���������� ��������
											// ������� �� ������ passive module 6 � �� ����� ��� ��� ��������� 
											// �������� �� ����� ������� ������ ����.
											doublerealT max_vnediagonal33 = -1.0e30;
											for (integer is01 = istart2; (is01 <= istopmarker2); is01++) {
												if (Amat[is01].j != Amat[is01].i) {
													if ((Amat[is01].aij < 0.0) && (fabs(Amat[is01].aij) > max_vnediagonal33)) {
														max_vnediagonal33 = fabs(Amat[is01].aij);
													}
												}
											}
											for (integer is01 = istart2; (is01 <= istopmarker2); is01++) {
												// 0.2375 ����������� ��������� ��� passive module 6.
												if ((Amat[is01].aij < 0.0) && (fabs(Amat[is01].aij) > 0.2375*max_vnediagonal33)) {
													if (Amat[is01].j == set[js]) {
														if ((my_amg_manager.ipatch_number == 7) && (bStrongTransposeON)) {
															if (js < ic_end_F_SiTranspose) {
																// ����������� �������� ������ ��� ������� F ����� �������
																// �������� �������� F ����� ������� ���� �������� �� Si_Transpose ������.
																// ������ ��� �������� � ���� � �������.
																ic2++;
															}
														}
														else {
															ic2++;
														}
													}
												}
												// ��������� ������� weakly ������ ?
											}
										

										data_BalTree dsearch;
										dsearch.countsosed = count_sosed[isc];
										//dsearch.ii = ii_2;
										dsearch.i = isc;
										// ����������� �� ���������� ������ � ������ F ������.
										count_sosed[isc] += ic2;
										data_BalTree dadd;
										dadd.countsosed = count_sosed[isc];
										//dadd.ii = ii_2;
										dadd.i = isc;

										// ���������� ���� ��� ������ ��� ���� �����.
										integer  veb_dadd_key = (dadd.countsosed)*(n_a[ilevel - 1] + 1) + (dadd.i);
										integer  veb_dsearch_key = (dsearch.countsosed)*(n_a[ilevel - 1] + 1) + (dsearch.i);
										//integer  veb_dadd_key = (dadd.countsosed)*(n + 1) + (dadd.i);
										//integer  veb_dsearch_key = (dsearch.countsosed)*(n + 1) + (dsearch.i);

										if (veb_dadd_key > universe - 2) {
											printf("perepolnenie veb-Van Emde Boas 2^2^5\n");
											getchar();
										}
										if (veb_dsearch_key > universe - 2) {
											printf("perepolnenie veb-Van Emde Boas 2^2^5\n");
											getchar();
										}

										if (veb_dadd_key < 1) {
											printf("perepolnenie veb-Van Emde Boas <1 \n");
											getchar();
										}
										if (veb_dsearch_key < 1) {
											printf("perepolnenie veb-Van Emde Boas <1 \n");
											getchar();
										}
										

											TreapNode* nrt_temp = NULL;
											TreapNode* save_root = NULL;

											// ��������� ������� � ��� ������,
											// ������ ���� ������� ��� ��������� � ������ �� �� ��������������.
											// 12 ������� 2015.
											// ���������� ���� ���������� �� ��������������� �� ��������� 2 �����,
											// ������ ����� ���������� ������ ������� ����������������.
											// ��������-�������� � ������ 1962.
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
													binary_heap.insert(count_sosed[isc], isc);
												}
												else {
													// �����������.
													// ������������ ������� ������ ��������.
													binary_heap.insert(count_sosed[isc], isc);
												}
												break;
											case RANDOM_TREE_ID:
												nrt_temp = NULL;
												save_root = random_tree_root;
												nrt_temp = search(random_tree_root, dsearch);
												random_tree_root = save_root;
												save_root = NULL;
												if (nrt_temp == NULL) {
													// ������� � ������ �����������.
													random_tree_root = insert(random_tree_root, dadd);
												}
												else {
													nrt_temp = NULL;
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
												
												res_vanEMDE_BOAS_Tree = vEB_find(vanEMDE_BOAS_Tree, veb_dsearch_key);
												if (!res_vanEMDE_BOAS_Tree) {
													// �� ������
													res_vanEMDE_BOAS_Tree = vEB_find(vanEMDE_BOAS_Tree, veb_dadd_key);
													if (!res_vanEMDE_BOAS_Tree) {
														// �� ������
														res_vanEMDE_BOAS_Tree = vEB_insert(vanEMDE_BOAS_Tree, veb_dadd_key);
														if (!res_vanEMDE_BOAS_Tree) printf("insert problem veb %lld\n", veb_dadd_key);
													}
												}
												else {
													res_vanEMDE_BOAS_Tree = vEB_delete(vanEMDE_BOAS_Tree, veb_dsearch_key);
													if (!res_vanEMDE_BOAS_Tree) {
														printf("nevozmochno udalit post factum delete %lld\n", veb_dsearch_key);
														getchar();
													}
													// ������, ������ � �������� == �������.
													res_vanEMDE_BOAS_Tree = vEB_insert(vanEMDE_BOAS_Tree, veb_dadd_key);
													if (!res_vanEMDE_BOAS_Tree) printf("insert problem veb %lld\n", veb_dadd_key);
												}
												
												break;
											default: root = insert_and_modify(root, dadd, dsearch);
												break;
											}
											
										

										if (bmaxsosedinfoactive) {
											// ��������� ���������� � ������������ ���������� �������.
											if (count_sosed[isc] >= Maximumsosedcount) {
												Maximumsosedcount = count_sosed[isc];
											}
										}
									}
								

							}

						}
					}

					// ������� (�������������� ��� �������).
					// �� � ���� ������ �� ����������� �� OPENMP � ���� �����.!!!
					for (integer i_54 = 0; i_54 < itop_stack2; i_54++) {
						hash_table2[istack2[i_54]] = false;
					}
					itop_stack2 = 0; // ���� ����� ����� � ������.


			
			

				if (set != NULL) {
					delete[] set;
					set = NULL;
				}

				n_coarce++; // ��������� ���������� � �����.

							// ���� ������� ������.

			} // ���� �� ��� ��� ������� � �������.



			bcontinue = false;
			for (integer i_1 = istartflag_scan; i_1 <= n_a[ilevel - 1]; i_1++) {
				if (flag[i_1] == false) {
					bcontinue = true;
					istartflag_scan = i_1; // ��������� ������� ������������.
					break; // ��������� ����� �� ����� for.
						   //if (maxsosed == -1) {
#if doubleintprecision == 1
						   //printf("ERROR!!!!  i_1=%lld\n", i_1);
#else
						   //printf("ERROR!!!!  i_1=%d\n", i_1);
#endif

						   //system("pause");
						   //}
				}
			}

			// ���������� ���� � ������������ ����������� �������.
			maxsosed = -1;
			icandidate = 0;			
			
				
				// ������ ��� ����������� ���������.
				bmaxsosedinfoactive = true;
				// ���� ����� ������������ ������� � ��� ������.
				node_AVL* emax = 0;
				Tree_splay* emax_splay = 0;
				TreapNode* emax_random_tree = NULL;
				TreapNode* save_root = NULL;
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
						maxsosed = -1;
						bcontinue = false;
					}
					break;
				case RANDOM_TREE_ID:
					save_root = random_tree_root;
					if (emax_random_tree != NULL) {
						delete[] emax_random_tree;
						emax_random_tree = NULL;
					}
					emax_random_tree = findmax_random_tree(random_tree_root);
					random_tree_root = save_root;
					save_root = NULL;

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
						dbt_emax.countsosed = ((ui_emax) / (n_a[ilevel - 1] + 1));					
					}
					break;
				case VAN_EMDE_BOAS_TREE_ID:
					
					if (!((vanEMDE_BOAS_Tree==NULL)||(vanEMDE_BOAS_Tree->summary == NULL) && (vanEMDE_BOAS_Tree->cluster == NULL))) {
						vEB_max(vanEMDE_BOAS_Tree, ui_emax);
						if (ui_emax <= 0) {
							// ������ ��� ���� ����� ������.
							dbt_emax.i = -1;
							dbt_emax.countsosed = -1;
						}
						else {
							dbt_emax.i = ((ui_emax) % (n_a[ilevel - 1] + 1));
							dbt_emax.countsosed = ((ui_emax) / (n_a[ilevel - 1] + 1));
						}
						
					}
					else {
						// ������ ��� ���� ����� ������.
						dbt_emax.i = -1;
						dbt_emax.countsosed = -1;
					}
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
						maxsosed = -1;
						bcontinue = false;

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
						random_tree_root = NULL;
						icandidate = 0;
						maxsosed = -1;
						bcontinue = false;

					}
					break;
				case BINARY_HEAP:
					break;
				case RANDOM_TREE_ID:
					// Random TREE
					if (emax_random_tree != NULL) {
						icandidate = row_startA[emax_random_tree->key.i];
						if (emax_random_tree != NULL) {
							delete[] emax_random_tree;
							emax_random_tree = NULL;
						}
						emax_random_tree = NULL;
					}
					else {
						RBroot.Clear();
						root_splay = 0;
						size_splay_Tree = 0;
						random_tree_root = NULL;
						icandidate = 0;
						maxsosed = -1;
						bcontinue = false;
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
						random_tree_root = NULL;
						icandidate = 0;
						maxsosed = -1;
						bcontinue = false;
					}
					break;
				case FIBONACCI_HEAP_ID:
					if (dbt_emax.i == -1)
					{
						// ������ �����.
						RBroot.Clear();
						root_splay = 0;
						size_splay_Tree = 0;
						random_tree_root = NULL;
						icandidate = 0;
						maxsosed = -1;
						bcontinue = false;
					}
					else {
						// ������� ���� � ������ ��� ���� ����� �� �����.
						icandidate = row_startA[dbt_emax.i];
						//printf("row_startA = %d %d\n", icandidate, dbt_emax.i);

					}
					break;
				case VAN_EMDE_BOAS_TREE_ID:
					if (dbt_emax.i == -1)
					{
						// ������ �����.
						RBroot.Clear();
						root_splay = 0;
						size_splay_Tree = 0;
						random_tree_root = NULL;						
						icandidate = 0;
						maxsosed = -1;
						bcontinue = false;
					}
					else {
						// ������� ���� � ������ ��� ���� ����� �� �����.
						icandidate = row_startA[dbt_emax.i];
						//printf("row_startA = %d\n", icandidate);
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
						maxsosed = -1;
						bcontinue = false;

					}
					break;
				}



#if doubleintprecision == 1
			//printf("maximum number of sosed=%lld\n",maxsosed);
#else
			//printf("maximum number of sosed=%d\n",maxsosed);
#endif

			if (maxsosed == -1) if (debug_reshime) system("pause");
			//getchar();

			if ((icandidate == 0) && (maxsosed == -1)) {
				bcontinue = false;
			}
			// 4 june 2016

			icountprohod++;

		} // ���������� C-F ���������. �������.

		

		  //delete[] count_sosed;
		if (count_sosed != NULL) {
			free(count_sosed);
			count_sosed = NULL;
		}

		//delete[] bmarkervisit;
		if (bmarkervisit != NULL) {
			free(bmarkervisit);
			bmarkervisit = NULL;
		}

		if (bprint_mesage_diagnostic) {
			if (n_a[ilevel - 1] == 0) {
				printf("n_a is zero\n");
				system("pause");
			}
			printf("additional C=%3.1f\n", (doublerealT)(100.0*newCcount / n_a[ilevel - 1]));
			//system("pause");
		}

		
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
				random_tree_root = NULL;
				break;
			case RED_BLACK_TREE_ID:
				RBroot.Clear();
				break;
			case VAN_EMDE_BOAS_TREE_ID:
				if (!((vanEMDE_BOAS_Tree == NULL) || (vanEMDE_BOAS_Tree->summary == NULL) && (vanEMDE_BOAS_Tree->cluster == NULL))) {
					vanEMDE_BOAS_Tree->~TvEB();
				}
				break;
			default: clear_AVL(root);
				root = 0;
				break;
			}

		


		// � ������ ����������� ������������ ������������ ��� ���������� �������������,
		// ��� ���� ����� ��������� ���������������� ��� F ���������� C ����������� ����
		// ��������� ���������� � ����������.
		the_number_of_neighbors_that_are_not_�_nodes = 0;
		number_of_F_nodes_with_one_single_strong_C_neighbor = 0;
		number_of_F_nodes_with_one_single_strong_C_neighborF = 0;

		iadditionalCstatistic = 0;

		bweSholdbeContinue = true;
		while (bweSholdbeContinue) {
			bweSholdbeContinue = false;

			bool *bvacant_candidates = NULL;
			bvacant_candidates = new bool[n_a[ilevel - 1] + 1];
			if (bvacant_candidates == NULL) {
				printf("error memory alloc in bvacant_candidates\n");
				system("pause");
				exit(1);
			}
#pragma omp parallel for
			for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) bvacant_candidates[i_1] = false;

			// ���������� ����������� ��� ����� ������� ���������� F nodes.
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
					//for (integer is0 = i_2; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[i_2].i); is0++) {
					integer iend_merker_position = row_startA[Amat[i_2].i + 1] - 1;
					for (integer is0 = i_2; (is0 <= iend_merker_position); is0++) {
						if (Amat[is0].j != Amat[i_2].i) {
							bvisit = true;
							if (this_is_C_node[Amat[is0].j] == true) {
								icsos++;
							}
							else {
								//the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
							}
						}
					}
					//if (icsos == 1) number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ � �������.
					// ���� bvisit �� ��������������� �������� ���� �� ��� ��� Fnodes. ����� ��� ������������ ������� �������.
					if ((icsos == 0) && (bvisit)) {

						// � ���� �� F ���� ������� ��� �������, �� ����� ���� ����� ���� ������� � ��� ������� ������� �� ����.
						// ���� ������� ����� ���� ��� ������� �� ��������� �������, ��� ������������� � ����� ������ � ����
						// ��������� ������������ ���������� ����� � �� ���� ��������.
						// ����� ���������� ��� ����������� ����� �������� � �������� ������.

						// ��� � �������, ���� ���� ������ � �����.
						bvacant_candidates[i_1] = true;
					}
				}
			}

		 

				// ������������ ���������� �� ����� ��� � 40 �������
				integer newCcount_arr[40];
				integer the_number_of_neighbors_that_are_not_�_nodes_arr[40];
				integer number_of_F_nodes_with_one_single_strong_C_neighbor_arr[40];
				bool bweSholdbeContinue_arr[40];

				for (integer i_1 = 0; i_1 < 40; i_1++) {
					newCcount_arr[i_1] = 0;
					the_number_of_neighbors_that_are_not_�_nodes_arr[i_1] = 0;
					number_of_F_nodes_with_one_single_strong_C_neighbor_arr[i_1] = 0;
					bweSholdbeContinue_arr[i_1] = false;
				}

				// ���������� ����������� ��� ����� ������� ���������� F nodes.
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
						//for (integer is0 = i_2; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[i_2].i); is0++) {
						integer iend_merker_position = row_startA[Amat[i_2].i + 1] - 1;
						for (integer is0 = i_2; (is0 <= iend_merker_position); is0++) {
							if (Amat[is0].j != Amat[i_2].i) {
								bvisit = true;
								if (this_is_C_node[Amat[is0].j] == true) {
									icsos++;
								}
								else {
									//the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
									the_number_of_neighbors_that_are_not_�_nodes_arr[tid]++;
								}
							}
						}
						if (icsos == 1) {
							//	number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ ������� � �������.
							number_of_F_nodes_with_one_single_strong_C_neighbor_arr[tid]++;
						}
						// ���� bvisit �� ��������������� �������� ���� �� ��� ��� Fnodes. ����� ��� ������������ ������� �������.
						if ((icsos == 0) && (bvisit)) {

							// � ���� �� F ���� ������� ��� �������, �� ����� ���� ����� ���� ������� � ��� ������� ������� �� ����.
							// ���� ������� ����� ���� ��� ������� �� ��������� �������, ��� ������������� � ����� ������ � ����
							// ��������� ������������ ���������� ����� � �� ���� ��������.
							// ����� ���������� ��� ����������� ����� �������� � �������� ������.							

							// ��� � �������, ���� ���� ������ � �����.
							this_is_F_node[i_1] = false;
							this_is_C_node[i_1] = true;
							// F node ���� C_node!!! ���� ����������� ������������ 
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
					the_number_of_neighbors_that_are_not_�_nodes += the_number_of_neighbors_that_are_not_�_nodes_arr[i_1];
					number_of_F_nodes_with_one_single_strong_C_neighbor += number_of_F_nodes_with_one_single_strong_C_neighbor_arr[i_1];
					if (bweSholdbeContinue_arr[i_1]) {
						bweSholdbeContinue = true;
					}
				}

			


			if (bprint_mesage_diagnostic) {
#if doubleintprecision == 1
				printf("newCcount=%lld, n_a=%lld %e\n", newCcount, n_a[ilevel - 1], 100.0*newCcount / n_a[ilevel - 1]);
#else
				printf("newCcount=%d, n_a=%d %e\n", newCcount, n_a[ilevel - 1], 100.0*newCcount / n_a[ilevel - 1]);
#endif

			}
			if (bvacant_candidates != NULL) {
				delete[] bvacant_candidates;
			}

			if (bprint_mesage_diagnostic) {
				if (bweSholdbeContinue) {
					printf(" prohod succseful\n");
				}
				else {
					printf("prohod empty\n");
				}
			}

		}


		// 01.01.2017 �������� ��������� �������� C-F ���������. ������ 2. 
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
		if ((my_amg_manager.icoarseningtype == 1)||((my_amg_manager.icoarseningtype == 3))) { // RS2 ������ 2.
			for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) if (this_is_F_node[i_1] == true) {
				// i_1 ��� F ���������� Fi.
				//Amat.���������� ����� - threshold ��� ������ ������.
				doublerealT thresholdRS = -1.0;
				integer i_2 = row_startA[i_1];
				
					// ������� ��� �������.
					clear_hash_table_Gus_struct01();
					// ��������� ������ �� ��������� ������ � ��� ������� ��� ������ � ������ � Amat[i_2].i.
					//integer imarker75_scan = 0;
					//formirate_F_SiTranspose_hash_table_Gus_struct02(hash_StrongTranspose_collection1[Amat[i_2].i], imarker75_scan);

				integer iend_merker_position = row_startA[Amat[i_2].i + 1] - 1;
				if (!btreshold_on_new_vetv) {
					for (integer is0 = i_2; (is0 <= iend_merker_position); is0++) {
						if (Amat[is0].j != Amat[i_2].i) {
							if (Amat[is0].aij < 0.0) {
								if (fabs(Amat[is0].aij) > thresholdRS) thresholdRS = fabs(Amat[is0].aij);
							}
						}
					}
				}
				else {
					// �������� ����� ����: 11.06.2017.
					thresholdRS = threshold_quick_only_negative[Amat[i_2].i];
				}
				if (thresholdRS > 0.0) {
					// ��������� ������� �� ����� � ����� ����� thresholdRS.
					hashlist_i* ivacant_F2C = NULL;
					//  �. ������� ���� ������� � ������� � ����������� �������� ������.
					hashlist_i* ibuffer_strongC = NULL;
					integer ibuffer_strongC_marker = -1;
					integer inumber_strongF_count_Fi = 0;
					hashlist_i* ibuffer_strongF = NULL;
					integer ibuffer_strongF_marker = -1;
					for (integer is0 = i_2; (is0 <= iend_merker_position); is0++) {
						if (Amat[is0].j != Amat[i_2].i) {
							if (Amat[is0].aij < 0.0) {
								if (fabs(Amat[is0].aij) > theta*thresholdRS) {
									if (this_is_C_node[Amat[is0].j] == true) {
										ibuffer_strongC_marker++;
										insertion_list_i(ibuffer_strongC, Amat[is0].j);
										insert_hash_table_Gus_struct01(Amat[is0].j);// 11.08.2018
									}
									if (this_is_F_node[Amat[is0].j] == true) {

										//if (1) 19.01.2017
										if (0) {// if (0) 11.08.2018
											// ������� 19.01.2017
											
											// �������� hash_StrongTranspose_collection ������ ���� ���������������� �������
											// ������ ��������� ��� �������������, � ����� �� �������� �� ������� �.�. ������������
											//hash_StrongTranspose_collection1. ������ �������� isfound ��� hash_StrongTranspose_collection1
											// ��� �.�. ��� ����� ��������� �.�. �� ������ �������� ������.

												if (hash_StrongTranspose_collection != NULL) {
													data_BalTreeST dat_key;
													dat_key.i = Amat[is0].j;
													if (isfound(hash_StrongTranspose_collection[Amat[is0].i], dat_key)) {
														// ����� ������� 19.01.2017

														// ������� Fj ����� ������.
														// �������� Fi � Fj ������ �������.
														inumber_strongF_count_Fi++;
														ibuffer_strongF_marker++;
														insertion_list_i(ibuffer_strongF, Amat[is0].j);
													}
												}
												else {
													// ������� Fj ����� ������.
													// �������� Fi � Fj ������ �������.
													inumber_strongF_count_Fi++;
													ibuffer_strongF_marker++;
													insertion_list_i(ibuffer_strongF, Amat[is0].j);
												}
										
										}
										else {
											// ������� Fj ����� ������.
											// �������� Fi � Fj ������ �������.
											inumber_strongF_count_Fi++;
											ibuffer_strongF_marker++;
											
											insertion_list_i(ibuffer_strongF, Amat[is0].j);
										}
									}
								}
							}
						}
					}
					// ������� ��� �������.
					clear_hash_table_Gus_struct01();
					// ���������� ������� ibuffer_strongC �� �����������.
					integer* ibuffer_strong_C_bs = NULL;
					// ������������� ������������ iusage_old_version = 0
					// ��� ������� ������������ ������������� ����������������� ��� �������.
					// ����������� ��������� ������� ����� ������� ������ ��� ���������� RS2 coarsening
					// �� 7.5% �� ��������� � �������� ������� �� �������. 
					// ��������� �������� ������������� � ������������� ��������� ����������.
					// 11.06.2017.
					integer iusage_old_version = 0; // 1 ������ ������� ������. // 0 ����� ������ �� ������ ��� �������.
					integer i_5 = 0;
					if (iusage_old_version) {
						// �������� ������ ����� � �������, ����� �������� ������������� � �����������.
						//ibuffer_strong_C_bs = (integer*)malloc((ibuffer_strongC_marker + ibuffer_strongF_marker + 1) * sizeof(integer));
						//handle_error(ibuffer_strong_C_bs, "ibuffer_strong_C_bs", "classic_aglomerative_amg_6", (ibuffer_strongC_marker + ibuffer_strongF_marker + 1));
						// iend_merker_position - i_2 +3
						ibuffer_strong_C_bs = (integer*)malloc((iend_merker_position - i_2 + 3) * sizeof(integer));
						handle_error(ibuffer_strong_C_bs, "ibuffer_strong_C_bs", "classic_aglomerative_amg_6", (iend_merker_position - i_2 + 3));
						hashlist_i* ibuffer_strongC_scan = ibuffer_strongC;
						i_5 = 0;
						while (ibuffer_strongC_scan != NULL) {
							ibuffer_strong_C_bs[i_5] = ibuffer_strongC_scan->item;
							i_5++;
							ibuffer_strongC_scan = ibuffer_strongC_scan->next;
						}
						ibuffer_strongC_scan = NULL;
					}
					else {
						// ������ ���������� � ��������� ������ ���������� ��� �������.
						clear_hash_table_Gus_struct01();
						hashlist_i* ibuffer_strongC_scan = ibuffer_strongC;
						while (ibuffer_strongC_scan != NULL) {
							insert_hash_table_Gus_struct01(ibuffer_strongC_scan->item);
							ibuffer_strongC_scan = ibuffer_strongC_scan->next;
						}
						ibuffer_strongC_scan = NULL;
					}

					// ���������� �������������� ������� ��� ���������� � ����!!!
					if (iusage_old_version) {
						/*
						if ((i_5 - 1 <= 7)) {
						// ���������� �� �����������.
						// ���������� ��������� ��  10 ��������� ����������� ����������.
						for (integer i_8 = 1; i_8 <= i_5 - 1; i_8++) {
						integer i_7 = i_8;
						while ((i_7>0)&&(ibuffer_strong_C_bs[i_7] < ibuffer_strong_C_bs[i_7 - 1])) {
						integer ibuf31 = ibuffer_strong_C_bs[i_7];
						ibuffer_strong_C_bs[i_7] = ibuffer_strong_C_bs[i_7 - 1];
						ibuffer_strong_C_bs[i_7 - 1] = ibuf31;
						i_7--;
						}
						}
						}
						else {
						// ���� ��������� ������������ ������� ����������
						// ��� ����������� ������� ����� ��.
						qsort(ibuffer_strong_C_bs, i_5, sizeof(integer), intcompare);

						}
						*/
						// 3 ������ 2017.
						std::sort(ibuffer_strong_C_bs, ibuffer_strong_C_bs + i_5);
					}
					// ��� ������� F ������ �������� � ������ ibuffer_strongF. 
					hashlist_i* ibuffer_strongF_current = ibuffer_strongF;
					for (integer i_3 = 0; i_3 <= ibuffer_strongF_marker; i_3++) {
						if (ibuffer_strongF_current != NULL) {
							// ��������� ���� ������� F ������� ���������������.
							//1. ���������� threshold ��� Fj.
							doublerealT thresholdRS1 = -1.0;
							integer i_4 = row_startA[ibuffer_strongF_current->item];
							integer iend_merker_position1 = row_startA[Amat[i_4].i + 1] - 1;
							if (!btreshold_on_new_vetv) {
								for (integer is01 = i_4; (is01 <= iend_merker_position1); is01++) {
									if (Amat[is01].j != Amat[i_4].i) {
										if (Amat[is01].aij < 0.0) {
											if (fabs(Amat[is01].aij) > thresholdRS1) thresholdRS1 = fabs(Amat[is01].aij);
										}
									}
								}
							}
							else {
								// �������� ����� ����: 11.06.2017.
								thresholdRS1 = threshold_quick_only_negative[Amat[i_4].i];
							}
							integer inumber_strongF_count_Fj = 0;
							// ������� ����� thresholdRS1.
							
							hashlist_i* ibuffer_strongCFj = NULL;
							integer ibuffer_strongCFj_marker = -1;
							for (integer is01 = i_4; (is01 <= iend_merker_position1); is01++) {
								if (Amat[is01].j != Amat[i_4].i) {
									if (Amat[is01].aij < 0.0) {
										if (fabs(Amat[is01].aij) > theta*thresholdRS1) {
											if (this_is_C_node[Amat[is01].j] == true) {
												ibuffer_strongCFj_marker++;
												insertion_list_i(ibuffer_strongCFj, Amat[is01].j);
											}
											if (this_is_F_node[Amat[is01].j] == true) {
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
							if (iusage_old_version) {
								while ((bfound_32 == false) && (ibuffer_strongCFj_scan != NULL)) {
									if (BinarySearch(ibuffer_strong_C_bs, ibuffer_strongCFj_scan->item, i_5 - 1) > -1) {
										// ���������� ������� �� ������ �� ������.
										bfound_32 = true;
									}
									ibuffer_strongCFj_scan = ibuffer_strongCFj_scan->next;
								}
							}
							else {
								// ������ �� ������ ��� �������.
								while ((bfound_32 == false) && (ibuffer_strongCFj_scan != NULL)) {
									// ���������� ������� �� ������ �� ������.
									bfound_32 = isfound_hash_table_Gus_struct01(ibuffer_strongCFj_scan->item);
									ibuffer_strongCFj_scan = ibuffer_strongCFj_scan->next;
								}
							}
							ibuffer_strongCFj_scan = NULL;
							if (bfound_32 == false) {
								// ���� �� ��� ������ � �����.
								if ((ibuffer_strongF_current->item > i_1) && (inumber_strongF_count_Fj >= inumber_strongF_count_Fi)) {
									// ���� Fj ��������� � ��� ��������������� ����� ������ F ����� �
									// ����� �� ��������� � F ����� Fi ������ ������� F ������.								

									// Fj ���������� �.
									insertion_list_i(ivacant_F2C, ibuffer_strongF_current->item);
									this_is_C_node[ibuffer_strongF_current->item] = true;
									this_is_F_node[ibuffer_strongF_current->item] = false;
									ibuffer_strongC_marker++;
									inumber_strongF_count_Fi--;
									insertion_list_i(ibuffer_strongC, ibuffer_strongF_current->item);
									// ����������������� ������ ������ � ������� ������ �������� ������.
									// �� ����� �������� ���� ����������� ����� ����������� ������, 
									// ������� ������� ������ malloc � free ����� �������.
									// 2.01.2017
									//if (ibuffer_strong_C_bs != NULL) {
									//free(ibuffer_strong_C_bs);
									//}
									//ibuffer_strong_C_bs = (integer*)malloc((ibuffer_strongC_marker + 1) * sizeof(integer));
									//handle_error(ibuffer_strong_C_bs, "ibuffer_strong_C_bs", "classic_aglomerative_amg_6", (ibuffer_strongC_marker + 1));

									hashlist_i* ibuffer_strongC_scan = ibuffer_strongC;
									integer i_5 = 0;

									if (iusage_old_version) {
										while (ibuffer_strongC_scan != NULL) {
											ibuffer_strong_C_bs[i_5] = ibuffer_strongC_scan->item;
											i_5++;
											ibuffer_strongC_scan = ibuffer_strongC_scan->next;
										}
									}
									else {
										// ������� ��� ������� � ��������� � �� �����.
										clear_hash_table_Gus_struct01();
										while (ibuffer_strongC_scan != NULL) {
											insert_hash_table_Gus_struct01(ibuffer_strongC_scan->item);
											ibuffer_strongC_scan = ibuffer_strongC_scan->next;
										}
									}


									ibuffer_strongC_scan = NULL;
									// ���������� �������������� ������� ��� ���������� � ����!!!
									if (iusage_old_version) {
										/*
										if ((i_5 - 1 <= 7)) {
										// ���������� �� �����������.
										// ���������� ��������� ��  10 ��������� ����������� ����������.
										for (integer i_8 = 1; i_8 <= i_5 - 1; i_8++) {
										integer i_7 = i_8;
										while ((i_7>0)&&(ibuffer_strong_C_bs[i_7] < ibuffer_strong_C_bs[i_7 - 1])) {
										integer ibuf31 = ibuffer_strong_C_bs[i_7];
										ibuffer_strong_C_bs[i_7] = ibuffer_strong_C_bs[i_7 - 1];
										ibuffer_strong_C_bs[i_7 - 1] = ibuf31;
										i_7--;
										}
										}
										}
										else {
										// ���� ��������� ������������ ������� ����������
										// ��� ����������� ������� ����� ��.
										qsort(ibuffer_strong_C_bs, i_5, sizeof(integer), intcompare);

										}
										*/
										// 3 ������ 2017.
										std::sort(ibuffer_strong_C_bs, ibuffer_strong_C_bs + i_5);
									}
								}
								else {
									// Fi ���������� �.
									this_is_C_node[i_1] = true;
									this_is_F_node[i_1] = false;
									// ���������� ��� Fj � � �� F.
									hashlist_i* ivacant_F2C_marker = ivacant_F2C;
									while (ivacant_F2C_marker != NULL) {
										this_is_F_node[ivacant_F2C_marker->item] = true;
										this_is_C_node[ivacant_F2C_marker->item] = false;
										ivacant_F2C_marker = ivacant_F2C_marker->next;
									}
									ivacant_F2C_marker = NULL;
									if (ivacant_F2C != NULL) {
										clear_hash_list_i(ivacant_F2C);
										ivacant_F2C = NULL;
									}
									ivacant_F2C = NULL;
									// ������� ���.
									if (ibuffer_strongCFj != NULL) {
										clear_hash_list_i(ibuffer_strongCFj);
										ibuffer_strongCFj = NULL;
									}
									ibuffer_strongCFj = NULL;
									// �������� ��������� ������� ������������ 
									// ������ ������� F �����.
									break;
								}

							}


							// ������� ���.
							if (ibuffer_strongCFj != NULL) {
								clear_hash_list_i(ibuffer_strongCFj);
								ibuffer_strongCFj = NULL;
							}
							ibuffer_strongCFj = NULL;
							// ������� � ���������� ���������.
							ibuffer_strongF_current = ibuffer_strongF_current->next;
						}
					}

					// ������������ ���.
					if (ibuffer_strong_C_bs != NULL) {
						free(ibuffer_strong_C_bs);
						ibuffer_strong_C_bs = NULL;
					}
					if (ibuffer_strongC != NULL) {
						clear_hash_list_i(ibuffer_strongC);
						ibuffer_strongC = NULL;
					}
					ibuffer_strongC = NULL;
					if (ibuffer_strongF != NULL) {
						clear_hash_list_i(ibuffer_strongF);
						ibuffer_strongF = NULL;
					}
					ibuffer_strongF = NULL;
					if (ivacant_F2C != NULL) {
						clear_hash_list_i(ivacant_F2C);
						ivacant_F2C = NULL;
					}
					ivacant_F2C = NULL;

				}
			}
		} // �������� ��������� �������� C-F ���������. ������ 2.
		}
		else {
			// ������� ����������������� �����. ����� �������.

			// 01.01.2017 �������� ��������� �������� C-F ���������. ������ 2. 
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

					// ������� ��� �������.
					clear_hash_table_Gus_struct01();
					// ��������� ������ �� ��������� ������ � ��� ������� ��� ������ � ������ � Amat[i_2].i.
					//!!!TODOinteger imarker75_scan = 0;
					//!!!TODOformirate_F_SiTranspose_hash_table_Gus_struct02(hash_StrongTranspose_collection1[Amat[i_2].i], imarker75_scan);
					

					integer iend_merker_position = row_startA[Amat[i_2].i + 1] - 1;
					if (!btreshold_on_new_vetv) {
						for (integer is0 = i_2; (is0 <= iend_merker_position); is0++) {
							if (Amat[is0].j != Amat[i_2].i) {
								if (Amat[is0].aij < 0.0) {
									if (fabs(Amat[is0].aij) > thresholdRS) thresholdRS = fabs(Amat[is0].aij);
								}
							}
						}
					}
					else {
						// �������� ����� ����: 11.06.2017.
						thresholdRS = threshold_quick_only_negative[Amat[i_2].i];
					}
					if (thresholdRS > 0.0) {
						// ��������� ������� �� ����� � ����� ����� thresholdRS.
						hashlist_i* ivacant_F2C = NULL;
						//  �. ������� ���� ������� � ������� � ����������� �������� ������.
						hashlist_i* ibuffer_strongC = NULL;
						integer ibuffer_strongC_marker = -1;
						integer inumber_strongF_count_Fi = 0;
						hashlist_i* ibuffer_strongF = NULL;
						integer ibuffer_strongF_marker = -1;
						for (integer is0 = i_2; (is0 <= iend_merker_position); is0++) {
							if (Amat[is0].j != Amat[i_2].i) {
								if (Amat[is0].aij < 0.0) {
									if (fabs(Amat[is0].aij) > theta*thresholdRS) {
										if (this_is_C_node[Amat[is0].j] == true) {
											ibuffer_strongC_marker++;
											insertion_list_i(ibuffer_strongC, Amat[is0].j);
											insert_hash_table_Gus_struct01(Amat[is0].j);// 11.08.2018
										}
										if (this_is_F_node[Amat[is0].j] == true) {

											//if (1) 19.01.2017
											if (1) {// if (0) 11.08.2018
													// ������� 19.01.2017

												if (hash_StrongTranspose_collection1 != NULL) {
													//data_BalTreeST dat_key;
													//dat_key.i = Amat[is0].j;
													if (isfound(hash_StrongTranspose_collection1[Amat[i_2].i], Amat[is0].j)) {
														// ����� ������� 19.01.2017
														// ���� ������-�� ������ �� ������� ��� ����������???todo

														// ������� Fj ����� ������.
														// �������� Fi � Fj ������ �������.
														inumber_strongF_count_Fi++;
														ibuffer_strongF_marker++;
														insertion_list_i(ibuffer_strongF, Amat[is0].j);
													}
												}
												else {
													// ������� Fj ����� ������.
													// �������� Fi � Fj ������ �������.
													inumber_strongF_count_Fi++;
													ibuffer_strongF_marker++;
													insertion_list_i(ibuffer_strongF, Amat[is0].j);
												}

											}
											else {
												// ������� Fj ����� ������.
												// �������� Fi � Fj ������ �������.
												inumber_strongF_count_Fi++;
												ibuffer_strongF_marker++;

												insertion_list_i(ibuffer_strongF, Amat[is0].j);
											}
										}
									}
								}
							}
						}
						// ������� ��� �������.
						clear_hash_table_Gus_struct01();
						// ���������� ������� ibuffer_strongC �� �����������.
						integer* ibuffer_strong_C_bs = NULL;
						// ������������� ������������ iusage_old_version = 0
						// ��� ������� ������������ ������������� ����������������� ��� �������.
						// ����������� ��������� ������� ����� ������� ������ ��� ���������� RS2 coarsening
						// �� 7.5% �� ��������� � �������� ������� �� �������. 
						// ��������� �������� ������������� � ������������� ��������� ����������.
						// 11.06.2017.
						integer iusage_old_version = 0; // 1 ������ ������� ������. // 0 ����� ������ �� ������ ��� �������.
						integer i_5 = 0;
						
						
							// ������ ���������� � ��������� ������ ���������� ��� �������.
							clear_hash_table_Gus_struct01();
							hashlist_i* ibuffer_strongC_scan = ibuffer_strongC;
							while (ibuffer_strongC_scan != NULL) {
								insert_hash_table_Gus_struct01(ibuffer_strongC_scan->item);
								ibuffer_strongC_scan = ibuffer_strongC_scan->next;
							}
							ibuffer_strongC_scan = NULL;
						

						// ���������� �������������� ������� ��� ���������� � ����!!!
						
						// ��� ������� F ������ �������� � ������ ibuffer_strongF. 
						hashlist_i* ibuffer_strongF_current = ibuffer_strongF;
						for (integer i_3 = 0; i_3 <= ibuffer_strongF_marker; i_3++) {
							if (ibuffer_strongF_current != NULL) {
								// ��������� ���� ������� F ������� ���������������.
								//1. ���������� threshold ��� Fj.
								doublerealT thresholdRS1 = -1.0;
								integer i_4 = row_startA[ibuffer_strongF_current->item];
								integer iend_merker_position1 = row_startA[Amat[i_4].i + 1] - 1;
								if (!btreshold_on_new_vetv) {
									for (integer is01 = i_4; (is01 <= iend_merker_position1); is01++) {
										if (Amat[is01].j != Amat[i_4].i) {
											if (Amat[is01].aij < 0.0) {
												if (fabs(Amat[is01].aij) > thresholdRS1) thresholdRS1 = fabs(Amat[is01].aij);
											}
										}
									}
								}
								else {
									// �������� ����� ����: 11.06.2017.
									thresholdRS1 = threshold_quick_only_negative[Amat[i_4].i];
								}
								integer inumber_strongF_count_Fj = 0;
								// ������� ����� thresholdRS1.

								hashlist_i* ibuffer_strongCFj = NULL;
								integer ibuffer_strongCFj_marker = -1;
									for (integer is01 = i_4; (is01 <= iend_merker_position1); is01++) {
										if (Amat[is01].j != Amat[i_4].i) {
											if (Amat[is01].aij < 0.0) {
												if (fabs(Amat[is01].aij) > theta*thresholdRS1) {
													if (this_is_C_node[Amat[is01].j] == true) {
														ibuffer_strongCFj_marker++;
														insertion_list_i(ibuffer_strongCFj, Amat[is01].j);
													}
													if (this_is_F_node[Amat[is01].j] == true) {
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
								if (iusage_old_version) {
									while ((bfound_32 == false) && (ibuffer_strongCFj_scan != NULL)) {
										if (BinarySearch(ibuffer_strong_C_bs, ibuffer_strongCFj_scan->item, i_5 - 1) > -1) {
											// ���������� ������� �� ������ �� ������.
											bfound_32 = true;
										}
										ibuffer_strongCFj_scan = ibuffer_strongCFj_scan->next;
									}
								}
								else {
									// ������ �� ������ ��� �������.
									while ((bfound_32 == false) && (ibuffer_strongCFj_scan != NULL)) {
										// ���������� ������� �� ������ �� ������.
										bfound_32 = isfound_hash_table_Gus_struct01(ibuffer_strongCFj_scan->item);
										ibuffer_strongCFj_scan = ibuffer_strongCFj_scan->next;
									}
								}
								ibuffer_strongCFj_scan = NULL;
								if (bfound_32 == false) {
									// ���� �� ��� ������ � �����.
									if ((ibuffer_strongF_current->item > i_1) && (inumber_strongF_count_Fj >= inumber_strongF_count_Fi)) {
										// ���� Fj ��������� � ��� ��������������� ����� ������ F ����� �
										// ����� �� ��������� � F ����� Fi ������ ������� F ������.								

										// Fj ���������� �.
										insertion_list_i(ivacant_F2C, ibuffer_strongF_current->item);
										this_is_C_node[ibuffer_strongF_current->item] = true;
										this_is_F_node[ibuffer_strongF_current->item] = false;
										ibuffer_strongC_marker++;
										inumber_strongF_count_Fi--;
										insertion_list_i(ibuffer_strongC, ibuffer_strongF_current->item);
										// ����������������� ������ ������ � ������� ������ �������� ������.
										// �� ����� �������� ���� ����������� ����� ����������� ������, 
										// ������� ������� ������ malloc � free ����� �������.
										// 2.01.2017
										
										hashlist_i* ibuffer_strongC_scan = ibuffer_strongC;
										integer i_5 = 0;

										if (iusage_old_version) {
											while (ibuffer_strongC_scan != NULL) {
												ibuffer_strong_C_bs[i_5] = ibuffer_strongC_scan->item;
												i_5++;
												ibuffer_strongC_scan = ibuffer_strongC_scan->next;
											}
										}
										else {
											// ������� ��� ������� � ��������� � �� �����.
											clear_hash_table_Gus_struct01();
											while (ibuffer_strongC_scan != NULL) {
												insert_hash_table_Gus_struct01(ibuffer_strongC_scan->item);
												ibuffer_strongC_scan = ibuffer_strongC_scan->next;
											}
										}


										ibuffer_strongC_scan = NULL;
										// ���������� �������������� ������� ��� ���������� � ����!!!
										
									}
									else {
										// Fi ���������� �.
										this_is_C_node[i_1] = true;
										this_is_F_node[i_1] = false;
										// ���������� ��� Fj � � �� F.
										hashlist_i* ivacant_F2C_marker = ivacant_F2C;
										while (ivacant_F2C_marker != NULL) {
											this_is_F_node[ivacant_F2C_marker->item] = true;
											this_is_C_node[ivacant_F2C_marker->item] = false;
											ivacant_F2C_marker = ivacant_F2C_marker->next;
										}
										ivacant_F2C_marker = NULL;
										if (ivacant_F2C != NULL) {
											clear_hash_list_i(ivacant_F2C);
											ivacant_F2C = NULL;
										}
										ivacant_F2C = NULL;
										// ������� ���.
										if (ibuffer_strongCFj != NULL) {
											clear_hash_list_i(ibuffer_strongCFj);
											ibuffer_strongCFj = NULL;
										}
										ibuffer_strongCFj = NULL;
										// �������� ��������� ������� ������������ 
										// ������ ������� F �����.
										break;
									}

								}


								// ������� ���.
								if (ibuffer_strongCFj != NULL) {
									clear_hash_list_i(ibuffer_strongCFj);
									ibuffer_strongCFj = NULL;
								}
								ibuffer_strongCFj = NULL;
								// ������� � ���������� ���������.
								ibuffer_strongF_current = ibuffer_strongF_current->next;
							}
						}

						// ������������ ���.
						if (ibuffer_strong_C_bs != NULL) {
							free(ibuffer_strong_C_bs);
							ibuffer_strong_C_bs = NULL;
						}
						if (ibuffer_strongC != NULL) {
							clear_hash_list_i(ibuffer_strongC);
							ibuffer_strongC = NULL;
						}
						ibuffer_strongC = NULL;
						if (ibuffer_strongF != NULL) {
							clear_hash_list_i(ibuffer_strongF);
							ibuffer_strongF = NULL;
						}
						ibuffer_strongF = NULL;
						if (ivacant_F2C != NULL) {
							clear_hash_list_i(ivacant_F2C);
							ivacant_F2C = NULL;
						}
						ivacant_F2C = NULL;

					}
				}
			} // �������� ��������� �������� C-F ���������. ������ 2.
		}
		

		// ����� ��������� ���������� ���� �������,
		// ���� F ���� �������� ����� ������� ��� ������� �� ��� ���� ������� � �����,
		// �� ������ ����� ���� ����� ���� � �������� ���������� ��������� ������ �� ���� ����������.
		// ������� ����� ������������� ��������� � ������ ������ (�������� �����).


		C_numerate = NULL;
		//C_numerate = new integer[n_a[ilevel - 1] + 1];
		C_numerate = (integer*)malloc((n_a[ilevel - 1] + 1) * sizeof(integer));
		handle_error(C_numerate, "C_numerate", "classic_aglomerative_amg_6", (n_a[ilevel - 1] + 1));

		icounter = 1;
		ap_coarse = NULL;

		bweSholdbeContinue = true;
		while (bweSholdbeContinue) {
			bweSholdbeContinue = false;

			integer n_coarce15 = 0;
#pragma omp parallel for reduction(+:n_coarce15)
			for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) {
				if (this_is_C_node[i_1]) {
					n_coarce15++;
				}
			}
			n_coarce += n_coarce15;
			n_coarce--;


			// debug
			// �������� �������� C-F ���������.
			//doublerealT* exp1 = new doublerealT[n_a[ilevel - 1] + 1];
			//for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) exp1[i_1] = 0.0;
			//for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) if (this_is_C_node[i_1]) exp1[i_1] = 2.0;
			//for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) if (this_is_F_node[i_1]) exp1[i_1] = 1.0;
			//exporttecplot(exp1,n);
			//delete[] exp1;

			//printf("export ready");

			//system("pause");



			// C-F ��������� ���������, ����� ����� ��������� �������� ������������.
			// ����� ����� �������� ��������, ��� ����������������� �������� ������������.
			// �� ��������� ���������� ������� ������ ��������� ������ � ����� ��������� ����� �������.

			// ���������� ��������� ������������� : 
			// coarse 2 fine.
			//P*coarse==fine


			// ���������� (����������) ���� ������ �����.
#pragma omp parallel for
			for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) C_numerate[i_1] = 0;
			icounter = 1;
			for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) if (this_is_C_node[i_1] == true) {
#if doubleintprecision == 1
				//printf("C ind= %lld", i_1); getchar();
#else
				//printf("C ind= %d", i_1); getchar();
#endif

				C_numerate[i_1] = icounter;
				icounter++;
			}

			// TODO 22_10_2016
			integer icount1_injection = 1 + iaddR;

			// C_numerate - ������������� �� ��������� Coarse �����.
			// ���������� ����������� ��� ����� ������� ���������� ������ �����.
			icount1 = 1 + iaddR; // nnz_R
			for (integer i_1 = 1; i_1 <= n_a[ilevel - 1]; i_1++) if (this_is_C_node[i_1] == true) {
				P[icount1].aij = 1.0;
				P[icount1].i = C_numerate[i_1]; // coarse number
				P[icount1].j = i_1; // fine number.
				icount1++;
				if (icount1 >= nsizePR*n) {
					printf("memory error!!!\n");
					printf("not enough memory for the interpolation operator.\n");
					//system("PAUSE");
					//exit(1);
					deallocate_prolongation(nsizePR, n, R, P);
				}
			}


			// �������� icount1 ����� �����.�� ������� !!!.
			numberofcoarcenodes = icount1 - 1 - iaddR;

		

			// ��� ����������� R  ���� transpose(P)/ap.
			if (bprint_mesage_diagnostic) {
#if doubleintprecision == 1
				printf("countloc=%lld\n", numberofcoarcenodes);
#else
				printf("countloc=%d\n", numberofcoarcenodes);
#endif

				if (debug_reshime) system("pause");
			}

			//ap_coarse = new doublerealT[numberofcoarcenodes + 1];
			if (ap_coarse != NULL) {
				free(ap_coarse);
				ap_coarse = NULL;
			}
			ap_coarse = (doublerealT*)malloc((numberofcoarcenodes + 1) * sizeof(doublerealT));
			handle_error(ap_coarse, "ap_coarse", "classic_aglomerative_amg_6", (numberofcoarcenodes + 1));

			ap_coarse[0] = 0.0;




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
					//for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[ii1].i); is0++) {
					integer iend_marker_position = row_startA[Amat[ii1].i + 1] - 1;
					for (integer is0 = ii1; (is0 <= iend_marker_position); is0++) {
#if doubleintprecision == 1
						//printf("i=%lld j=%lld Amat[is0].aij=%e ", Amat[is0].i, Amat[is0].j, Amat[is0].aij);
#else
						//printf("i=%d j=%d Amat[is0].aij=%e ", Amat[is0].i, Amat[is0].j, Amat[is0].aij);
#endif

						if (Amat[is0].j == Amat[ii1].i) {

							if (fabs(Amat[is0].aij) > RealMAXIMUM) {
								printf("perepolnenie error!");
								//getchar();
								system("pause");
							}
							ap_coarse[C_numerate[i8]] = fabs(Amat[is0].aij);
							//printf("find = %e", fabs(Amat[is0].aij));
						}
					}
				}
				//printf("\n");
				//getchar();
			}

#if doubleintprecision == 1
			//printf("incoming=%lld\n", my_amg_manager.number_interpolation_procedure);
#else
			//printf("incoming=%d\n", my_amg_manager.number_interpolation_procedure);
#endif

			//getchar();

			// ����� 2 �������.

			bool the_good_old_interpolation = true;

			if (the_good_old_interpolation) {

				// ������ ������ ������, ����������� �������������.
				// � ���� �� ����������� �������.

				//my_amg_manager.number_interpolation_procedure == 0
				// 0
				if (my_amg_manager.number_interpolation_procedure == 10) {

					// ���������������� ��������� �10. 
					my_interpolation_procedure_number10(the_number_of_neighbors_that_are_not_�_nodes,
						number_of_F_nodes_with_one_single_strong_C_neighbor,
						n_a, this_is_F_node, row_startA,
						nnz_a, bpositive_connections, Amat,
						bweSholdbeContinue, this_is_C_node, iadditionalCstatistic,
						RealZERO, icount1, P, nsizePR, ilevel, iadd, theta, n, R, C_numerate);
				}
				//my_amg_manager.number_interpolation_procedure == 1
				// 0
				if (my_amg_manager.number_interpolation_procedure == 7) {

					// ������ ������������� � ��������� ��������.
					// �������� ������������� ����������� ������ ��� F ����� �������
					// �� ����� � �������.
					// �� ���� ��� ������ ������������ �������� �������� �� ������� ������ �������������.
					// ���� F ������� ������ Strong  � ������ �������� ���� �������� �� ����� Strong C ������.
					// ���� F �� ������� Strong C �������, �������� �������� �� Strong C ������� �������� Strong F ����� �
					// � ���������� ������������ ������ Strong F �������.
					// ���� ��� ����������� ��� Strongly ��������� F ���� � ������� � ������������ ��� ������ �� ������ Strong C ������
					// �� ���� �� ���� Strong F ����� ��������� � ����� � ������������ ������ ������� Strong F ������� ������� ���� F ������������.
					// ����� �� �������� ��������� �������� ���������� � ������ ��� ����������� � �����.


					// ���������������� ��������� �7. 
					my_interpolation_procedure_number7(the_number_of_neighbors_that_are_not_�_nodes,
						number_of_F_nodes_with_one_single_strong_C_neighbor,
						n_a, this_is_F_node, row_startA,
						nnz_a, bpositive_connections, Amat,
						bweSholdbeContinue, this_is_C_node, iadditionalCstatistic,
						RealZERO, icount1, P, nsizePR, ilevel,
						iadd, theta, n, R, C_numerate);
				}
				//my_amg_manager.number_interpolation_procedure == 2
				// 0
				if (my_amg_manager.number_interpolation_procedure == 2) {


					// ���������������� ��������� �2.
					my_interpolation_procedure_number2(the_number_of_neighbors_that_are_not_�_nodes,
						number_of_F_nodes_with_one_single_strong_C_neighbor,
						n_a, this_is_F_node, row_startA,
						nnz_a, bpositive_connections, Amat,
						bweSholdbeContinue, this_is_C_node, iadditionalCstatistic,
						RealZERO, icount1, P, nsizePR, ilevel,
						iadd, theta, n, R, C_numerate, number_of_F_nodes_with_one_single_strong_C_neighborF);

				}
				//my_amg_manager.number_interpolation_procedure == 3
				// 1
				if (my_amg_manager.number_interpolation_procedure == 3)
				{
					// �������, �������� ����� ������������ ���������������� ���������.

					// ���������������� ��������� �3.
					my_interpolation_procedure_number3(the_number_of_neighbors_that_are_not_�_nodes,
						number_of_F_nodes_with_one_single_strong_C_neighbor,
						n_a, this_is_F_node, row_startA,
						nnz_a, bpositive_connections, Amat,
						bweSholdbeContinue, this_is_C_node, iadditionalCstatistic,
						RealZERO, icount1, P, nsizePR, ilevel,
						iadd, theta, n, R, C_numerate,
						number_of_F_nodes_with_one_single_strong_C_neighborF,
						theta83, btreshold_on_new_vetv, ifrom_re_operation_protection,
						from_re_operation_protection0, magic82, threshold_quick_all,
						threshold_quick_only_negative);
				}

				//my_amg_manager.number_interpolation_procedure == 7
				// 1
				if (my_amg_manager.number_interpolation_procedure == 1) {

					// 1.04.2017
					// ������� ���� � ��� ����� ��������� ������������ �� ������,
					// �������� ������������� ������������ � �������� �������������,
					// � ����� ����������� � �� � ��.

					// ���������������� ��������� �1.
					/*
					my_interpolation_procedure_number1(the_number_of_neighbors_that_are_not_�_nodes,
					number_of_F_nodes_with_one_single_strong_C_neighbor,
					n_a, this_is_F_node, row_startA,
					nnz_a, bpositive_connections, Amat,
					bweSholdbeContinue, this_is_C_node, iadditionalCstatistic,
					RealZERO, icount1, P, nsizePR, ilevel,
					iadd, theta, n, R, C_numerate,
					number_of_F_nodes_with_one_single_strong_C_neighborF,
					theta83, btreshold_on_new_vetv, ifrom_re_operation_protection,
					from_re_operation_protection0, magic82, threshold_quick_all,
					threshold_quick_only_negative);
					*/

					// ���������������� ��������� �3.amg1r5 Ruge-Stuben
					my_interpolation_procedure_number3B(the_number_of_neighbors_that_are_not_�_nodes,
						number_of_F_nodes_with_one_single_strong_C_neighbor,
						n_a, this_is_F_node, row_startA,
						nnz_a, bpositive_connections, Amat,
						bweSholdbeContinue, this_is_C_node, iadditionalCstatistic,
						RealZERO, icount1, P, nsizePR, ilevel,
						iadd, theta, n, R, C_numerate,
						number_of_F_nodes_with_one_single_strong_C_neighborF,
						theta83, btreshold_on_new_vetv, ifrom_re_operation_protection,
						from_re_operation_protection0, magic82, threshold_quick_all,
						threshold_quick_only_negative);
				}

				if (my_amg_manager.number_interpolation_procedure == 0) {

					// 1.04.2017; 28.04.2017;
					// ������� ���� � ��� ����� ��������� ������������ �� ������, �������� ������������� ������������ � �������� �������������,
					// � ����� ����������� � �� � ��.

					// ���������������� ��������� �0.
					/*
					my_interpolation_procedure_number0(the_number_of_neighbors_that_are_not_�_nodes,
					number_of_F_nodes_with_one_single_strong_C_neighbor,
					n_a, this_is_F_node, row_startA,
					nnz_a, bpositive_connections, Amat,
					bweSholdbeContinue, this_is_C_node, iadditionalCstatistic,
					RealZERO, icount1, P, nsizePR, ilevel,
					iadd, theta, n, R, C_numerate,
					number_of_F_nodes_with_one_single_strong_C_neighborF,
					theta83, btreshold_on_new_vetv, ifrom_re_operation_protection,
					from_re_operation_protection0, magic82, threshold_quick_all,
					threshold_quick_only_negative);
					*/

					// ���������������� ��������� �3.
					// ���������� ������� �������.
					my_interpolation_procedure_number3A(the_number_of_neighbors_that_are_not_�_nodes,
						number_of_F_nodes_with_one_single_strong_C_neighbor,
						n_a, this_is_F_node, row_startA,
						nnz_a, bpositive_connections, Amat,
						bweSholdbeContinue, this_is_C_node, iadditionalCstatistic,
						RealZERO, icount1, P, nsizePR, ilevel,
						iadd, theta, n, R, C_numerate,
						number_of_F_nodes_with_one_single_strong_C_neighborF,
						theta83, btreshold_on_new_vetv, ifrom_re_operation_protection,
						from_re_operation_protection0, magic82, threshold_quick_all,
						threshold_quick_only_negative);

				}

				//my_amg_manager.number_interpolation_procedure == 4
				// 0
				if (my_amg_manager.number_interpolation_procedure == 4) {

					// ����� ������� (�������).
					// ���������� ����� 1.22 ������ ������� � 1.36 � ��������� �������.

					// ����� ���� F �� ������� Strong � ������� ��� ���������� � �����.
					// ���� F ������� ������ Strong  � ������ �������������� � ������� ������� � ������� 
					// ������� F �����.

					// ���������������� ��������� �4.
					my_interpolation_procedure_number4(the_number_of_neighbors_that_are_not_�_nodes,
						number_of_F_nodes_with_one_single_strong_C_neighbor,
						n_a, this_is_F_node, row_startA,
						nnz_a, bpositive_connections, Amat,
						bweSholdbeContinue, this_is_C_node, iadditionalCstatistic,
						RealZERO, icount1, P, nsizePR, ilevel,
						iadd, theta, n, R, C_numerate,
						number_of_F_nodes_with_one_single_strong_C_neighborF,
						theta83, btreshold_on_new_vetv, ifrom_re_operation_protection,
						from_re_operation_protection0, magic82, threshold_quick_all,
						threshold_quick_only_negative);

				}
				//my_amg_manager.number_interpolation_procedure == 5
				// 0
				if (my_amg_manager.number_interpolation_procedure == 5) {

					// �������.

					// ���������������� ��������� �5.
					my_interpolation_procedure_number5(the_number_of_neighbors_that_are_not_�_nodes,
						number_of_F_nodes_with_one_single_strong_C_neighbor,
						n_a, this_is_F_node, row_startA,
						nnz_a, bpositive_connections, Amat,
						bweSholdbeContinue, this_is_C_node, iadditionalCstatistic,
						RealZERO, icount1, P, nsizePR, ilevel,
						iadd, theta, n, R, C_numerate,
						number_of_F_nodes_with_one_single_strong_C_neighborF,
						theta83, btreshold_on_new_vetv, ifrom_re_operation_protection,
						from_re_operation_protection0, magic82, threshold_quick_all,
						threshold_quick_only_negative);
				}

			}
			else if (true) {

				// INTERPOLATION SIX

				// ����������������� ������������� 1 ������ 2016.

				// ���������������� ��������� �6.
				my_interpolation_procedure_number6(the_number_of_neighbors_that_are_not_�_nodes,
					number_of_F_nodes_with_one_single_strong_C_neighbor,
					n_a, this_is_F_node, row_startA,
					nnz_a, bpositive_connections, Amat,
					bweSholdbeContinue, this_is_C_node, iadditionalCstatistic,
					RealZERO, icount1, P, nsizePR, ilevel,
					iadd, theta, n, R, C_numerate,
					number_of_F_nodes_with_one_single_strong_C_neighborF,
					theta83, btreshold_on_new_vetv, ifrom_re_operation_protection,
					from_re_operation_protection0, magic82, threshold_quick_all,
					threshold_quick_only_negative);
			}
			else {
				// �� ������������� ������� ����� ����� �������
				// ��������� ��������� ����������������� ���� �������������.
				// ��� ������������� ���������� ���� ����������� ������������ ������,
				// ��������� �� ������ ������������ ������.

				printf("interpolation SIX: Theoretical approach in Montenegro.\n");
				system("PAUSE");

				the_number_of_neighbors_that_are_not_�_nodes = 0;
				number_of_F_nodes_with_one_single_strong_C_neighbor = 0;

				if (bpositive_connections) {

					// positive connections.

					// ���������� ����������� ��� ����� ������� ���������� F nodes.
					// ������ F-nodes ������ C-nodes.
					for (integer i8 = 1; i8 <= n_a[ilevel - 1]; i8++) if (this_is_F_node[i8] == true) {

						// ��� ����� ����������� �������� ��������.
						// 5 ������� 2015 ���� �� ��������� ��������� �������������
						// ��������� ������������� � ������ � ��������� ��������.
						doublerealT maxelem_threshold = -1.0;
						//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
						integer ii1 = row_startA[i8];
						for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[ii1].i); is0++) {
							if (Amat[is0].j != Amat[ii1].i) {
								// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
								//if (this_is_C_node[Amat[is0].j] == true) {
								if (fabs(Amat[is0].aij) > maxelem_threshold) {
									maxelem_threshold = fabs(Amat[is0].aij);
								}
								//}
							}
						}
						// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

						// ����� ������� ������� F-node ������� C-node.
						//integer icsos = 0;

						// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
						// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
						doublerealT sumP = 0.0;
						doublerealT sumPindicator = 0.0;
						for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[ii1].i); is0++) {
							if (Amat[is0].j != Amat[ii1].i) {
								if (this_is_C_node[Amat[is0].j] == true) {
									if (fabs(Amat[is0].aij) <= maxelem_threshold*theta) {
										// Weak connectors
										sumP += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� � �����.
																	 //icsos++;
									}
									else {
										sumPindicator += fabs(Amat[is0].aij);
									}
								}
								else {
									// ������������ ���������� ������� ������� �� �������� � ������.
									if (fabs(Amat[is0].aij) <= maxelem_threshold*theta) {
										// Weak connectors
										sumP += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� � �����.
																	 //icsos++;
									}
									the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
								}
							}
							else {
								// ������������ �������.
								sumP += fabs(Amat[is0].aij);
							}
						}
						//if (icsos == 1) number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ �������  � �������.


						// 1 ������ 2015 ���� ����� ��� ������������.
						// ������� � ������ ������ ������ ������ ����� ���� � �����.
						//if ((false) && (icsos == 1)) {
						//this_is_F_node[i8] = false;
						//this_is_C_node[i8] = true;
						//bweSholdbeContinue = true;
						//}
						//else
						{

							if (fabs(sumPindicator) < RealZERO) {
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

								// �� �� ����� �������� � ����, �� ����� ������������ �������� �������������.



							}
							else {

								integer icount1_frozen = icount1;

								for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[ii1].i); is0++) {
									if (Amat[is0].j != Amat[ii1].i) {
										if (this_is_C_node[Amat[is0].j] == true) {

											// ��������������� ������� �� ��������� � �����.

											// ������ ������� ������ ����������� ��������� 
											// ������������� ��������� �������� �� �������� 
											// �������� �������.
											// ����������� 5 ������� 2015.
											
											if (fabs(Amat[is0].aij) > maxelem_threshold*theta) {
												// Strongly C connectors.

												P[icount1].j = i8;
												P[icount1].i = C_numerate[Amat[is0].j];
												P[icount1].aij = fabs(Amat[is0].aij) / sumP;
												icount1++;
												if (icount1 >= nsizePR*n) {
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

								integer ilength_n = icount1 - icount1_frozen;
								integer* jposition_in_P = NULL;
								//jposition_in_P = new integer[ilength_n];
								jposition_in_P = (integer*)malloc(ilength_n * sizeof(integer));
								handle_error(jposition_in_P, "jposition_in_P", "classic_aglomerative_amg_6", ilength_n);



								integer i_97 = 0;

								for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[ii1].i); is0++) {
									if (Amat[is0].j != Amat[ii1].i) {
										if (this_is_C_node[Amat[is0].j] == true) {
											if (fabs(Amat[is0].aij) > maxelem_threshold*theta) {
												// Strongly C connections j position.
												jposition_in_P[i_97] = Amat[is0].j;
												i_97++;
											}
										}
									}
								}


								for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[ii1].i); is0++) {
									if (Amat[is0].j != Amat[ii1].i) {
										if (this_is_F_node[Amat[is0].j] == true) {
											if (fabs(Amat[is0].aij) > maxelem_threshold*theta) {
												// Strong F connections
												doublerealT my_mult = fabs(Amat[is0].aij);
												integer iFpoint = Amat[is0].j;
												//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
												integer ii1_loc = row_startA[iFpoint];

												// ������� ���� ������� ���� iFpoint
												// ���� ����� ��� �������� ������� � ������ 
												// ������������� ���������������� ���� Amat[ii1].i
												// �� �� ����� ����������� � ��������� sum23 
												// ������ ������� �������.
												doublerealT sum23 = 0.0;
												bool bvisit23 = false;
												for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat[is0_loc].i == Amat[ii1_loc].i); is0_loc++) {
													if (Amat[is0_loc].j != Amat[ii1_loc].i) {
														if (this_is_C_node[Amat[is0_loc].j] == true) {
															for (i_97 = 0; i_97 < ilength_n; i_97++) {
																if (Amat[is0_loc].j == jposition_in_P[i_97]) {
																	sum23 += fabs(Amat[is0_loc].aij);
																	bvisit23 = true;
																	break;
																}
															}
														}
													}
												}

												//if (fabs(sum23) > RealZERO) {
												if (bvisit23) {
													// �� ����� �� ����� �� ����.

													// ��������� ���� ������� ���� F.
													for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat[is0_loc].i == Amat[ii1_loc].i); is0_loc++) {
														if (Amat[is0_loc].j != Amat[ii1_loc].i) {
															if (this_is_C_node[Amat[is0_loc].j] == true) {
																for (i_97 = 0; i_97 < ilength_n; i_97++) {
																	if (Amat[is0_loc].j == jposition_in_P[i_97]) {
																		//P[icount1_frozen + i_97].j = i8;
																		//P[icount1_frozen+i_97].i = C_numerate[Amat[is0].j];
																		P[icount1_frozen + i_97].aij += (my_mult*fabs(Amat[is0_loc].aij)) / (sumP*sum23);
																		break;
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

								//delete[] jposition_in_P;
								free(jposition_in_P);

							}

						}


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
						doublerealT maxelem_threshold = -1.0;
						//integer ii1 = BinarySearchAi(Amat, i8, 1 + iadd, nnz_a[ilevel - 1] + iadd);
						integer ii1 = row_startA[i8];
						for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[ii1].i); is0++) {
							if (Amat[is0].j != Amat[ii1].i) {
								// ���� ��������������� �� ��������� ������������ ��������������� ������� � ������.
								//if (this_is_C_node[Amat[is0].j] == true) {
								if ((Amat[is0].aij<0.0) && (fabs(Amat[is0].aij) > maxelem_threshold)) {
									maxelem_threshold = fabs(Amat[is0].aij);
								}
								//}
							}
						}
						// ����� maxelem_threshold ��� ������ ������������� ���������������� �������� � ������ ����� � �������.

						// ����� ������� ������� F-node ������� C-node.
						//integer icsos = 0;

						// ��������� ������ ��������������� ��������� �� � ����� ������� ������ ������.
						// ��� ������� ������ ����� ����� ���������� ������� iscos. �� ���� iscos ������ ���� 2 � �����.
						doublerealT sumP = 0.0;
						doublerealT sumPindicator = 0.0;
						for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[ii1].i); is0++) {
							if (Amat[is0].j != Amat[ii1].i) {
								if (this_is_C_node[Amat[is0].j] == true) {
									if ((Amat[is0].aij>0.0) || (fabs(Amat[is0].aij) <= maxelem_threshold*theta)) {
										// Weak connectors
										sumP += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� � �����.
																	 //icsos++;
									}
									else {
										sumPindicator += fabs(Amat[is0].aij);
									}
								}
								else {
									// ������������ ���������� ������� ������� �� �������� � ������.
									if ((Amat[is0].aij>0.0) || (fabs(Amat[is0].aij) <= maxelem_threshold*theta)) {
										// Weak connectors
										sumP += fabs(Amat[is0].aij); // ����� ������� ��������������� ��������� ������� ����������� � �����.
																	 //icsos++;
									}
									the_number_of_neighbors_that_are_not_�_nodes++; // ������������ �������� ������������ 
								}
							}
							else {
								// ������������ �������.
								sumP += fabs(Amat[is0].aij);
							}
						}
						//if (icsos == 1) number_of_F_nodes_with_one_single_strong_C_neighbor++; // ���������� F ����� � ����� ������������ ������� � �������.


						// 1 ������ 2015 ���� ����� ��� ������������.
						// ������� � ������ ������ ������ ������ ����� ���� � �����.
						//if ((false) && (icsos == 1)) {
						//this_is_F_node[i8] = false;
						//this_is_C_node[i8] = true;
						//bweSholdbeContinue = true;
						//}
						//else
						{

							if (fabs(sumPindicator) < RealZERO) {
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

								// �� �� ����� �������� � ����, �� ����� ������������ �������� �������������.



							}
							else {

								integer icount1_frozen = icount1;

								for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[ii1].i); is0++) {
									if (Amat[is0].j != Amat[ii1].i) {
										if (this_is_C_node[Amat[is0].j] == true) {

											// ��������������� ������� �� ��������� � �����.

											// ������ ������� ������ ����������� ��������� 
											// ������������� ��������� �������� �� �������� 
											// �������� �������.
											// ����������� 5 ������� 2015.
											
											if ((Amat[is0].aij<0.0) && (fabs(Amat[is0].aij) > maxelem_threshold*theta)) {
												// Strongly C connectors.

												P[icount1].j = i8;
												P[icount1].i = C_numerate[Amat[is0].j];
												P[icount1].aij = fabs(Amat[is0].aij) / sumP;
												icount1++;
												if (icount1 >= nsizePR*n) {
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

								integer ilength_n = icount1 - icount1_frozen;
								integer* jposition_in_P = NULL;
								//jposition_in_P = new integer[ilength_n];
								jposition_in_P = (integer*)malloc(ilength_n * sizeof(integer));
								handle_error(jposition_in_P, "jposition_in_P", "classic_aglomerative_amg_6", ilength_n);



								integer i_97 = 0;

								for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[ii1].i); is0++) {
									if (Amat[is0].j != Amat[ii1].i) {
										if (this_is_C_node[Amat[is0].j] == true) {
											if ((Amat[is0].aij<0.0) && (fabs(Amat[is0].aij) > maxelem_threshold*theta)) {
												// Strongly C connections j position.
												jposition_in_P[i_97] = Amat[is0].j;
												i_97++;
											}
										}
									}
								}


								for (integer is0 = ii1; (is0 <= nnz_a[ilevel - 1] + iadd) && (Amat[is0].i == Amat[ii1].i); is0++) {
									if (Amat[is0].j != Amat[ii1].i) {
										if (this_is_F_node[Amat[is0].j] == true) {
											if ((Amat[is0].aij<0.0) && (fabs(Amat[is0].aij) > maxelem_threshold*theta)) {
												// Strong F connections
												doublerealT my_mult = fabs(Amat[is0].aij);
												integer iFpoint = Amat[is0].j;
												//integer ii1_loc = BinarySearchAi(Amat, iFpoint, 1 + iadd, nnz_a[ilevel - 1] + iadd);
												integer ii1_loc = row_startA[iFpoint];

												// ������� ���� ������� ���� iFpoint
												// ���� ����� ��� �������� ������� � ������ 
												// ������������� ���������������� ���� Amat[ii1].i
												// �� �� ����� ����������� � ��������� sum23 
												// ������ ������� �������.
												doublerealT sum23 = 0.0;
												bool bvisit23 = false;
												for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat[is0_loc].i == Amat[ii1_loc].i); is0_loc++) {
													if (Amat[is0_loc].j != Amat[ii1_loc].i) {
														if (this_is_C_node[Amat[is0_loc].j] == true) {
															for (i_97 = 0; i_97 < ilength_n; i_97++) {
																if (Amat[is0_loc].j == jposition_in_P[i_97]) {
																	sum23 += fabs(Amat[is0_loc].aij);
																	bvisit23 = true;
																	break;
																}
															}
														}
													}
												}

												//if (fabs(sum23) > RealZERO) {
												if (bvisit23) {
													// �� ����� �� ����� �� ����.

													// ��������� ���� ������� ���� F.
													for (integer is0_loc = ii1_loc; (is0_loc <= nnz_a[ilevel - 1] + iadd) && (Amat[is0_loc].i == Amat[ii1_loc].i); is0_loc++) {
														if (Amat[is0_loc].j != Amat[ii1_loc].i) {
															if (this_is_C_node[Amat[is0_loc].j] == true) {
																for (i_97 = 0; i_97 < ilength_n; i_97++) {
																	if (Amat[is0_loc].j == jposition_in_P[i_97]) {
																		//P[icount1_frozen + i_97].j = i8;
																		//P[icount1_frozen+i_97].i = C_numerate[Amat[is0].j];
																		P[icount1_frozen + i_97].aij += (my_mult*fabs(Amat[is0_loc].aij)) / (sumP*sum23);
																		break;
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

								//delete[] jposition_in_P;
								free(jposition_in_P);

							}

						}


					}


				} // end only negatyive connections

			}



			if (bweSholdbeContinue) {
				//delete[] ap_coarse;
				if (ap_coarse != NULL) {
					free(ap_coarse);
					ap_coarse = NULL;
				}
				if (bprint_mesage_diagnostic) {
					printf("obratnaq svqz restart...\n");
				}
			}

			if (bprint_mesage_diagnostic) {
				printf("addition C nodes procent %3.1f", (doublerealT)(100.0*iadditionalCstatistic / n_a[ilevel - 1]));
			}
			iadditionalCstatistic = 0;
			//system("pause");


		}

		nnzR = icount1 - iaddR;



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
			// ��� ������� � ��� ��� �������� ������������� ��� �������������� ��� ������������ �� j.
			switch (imy_sort_algorithm ) {
			case COUNTING_SORT_ALG :
			//Counting_Sortj(P, 1 + iaddR, iaddR + nnzR - 1, false);
			qsj(P, 1 + iaddR, iaddR + nnzR - 1);
			//HeapSort_j(P, 1 + iaddR, iaddR + nnzR - 1);
			break;
			case QUICK_SORT_ALG :
			qsj(P, 1 + iaddR, iaddR + nnzR - 1);
			// ������������ ��������. O(nlog(n)).
			// �� ���������� ������ ������.
			//std::sort(P + (1 + iaddR) * sizeof(Ak1), P + (iaddR + nnzR - 1+1) * sizeof(Ak1), compAi);
			break;
			case HEAP_SORT_ALG :
			HeapSort_j(P, 1 + iaddR, iaddR + nnzR - 1);
			break;
			default :
			//Counting_Sortj(P, 1 + iaddR, iaddR + nnzR - 1, false);
			qsj(P, 1 + iaddR, iaddR + nnzR - 1);
			//HeapSort_j(P, 1 + iaddR, iaddR + nnzR - 1);
			break;
			}
			*/


			// ������� ����� ������ ����������� ��������� ��������� ���������.
			// ������� ������ ������ � ��������� �������������, �������� � ���������� 
			// ���������� ��� ������������.
			// �������� ��������� ������ ������:
			//doublerealT const alpha_truncation = 0.2;
			doublerealT alpha_truncation = my_amg_manager.truncation_interpolation;
			// ���������� ������ ������ ��������� �������������.
			// ������ ����� ��������� ������ ������ ������� �����.
			// ����� ������������ �� ������ ������� ������� �����.
			// ������ ��� �������� � ��������� ������������� ������� ����� 
			// ������� ������ ������������� �� ������ ����-�� ����� * �� alpha_truncation.
			// ������� ������������������� ����� ����� �������� ����������.
			// ������� ��� � ������ R.
#pragma omp parallel for
			for (integer i_1 = 1; i_1 <= n; i_1++) {
				flag[i_1] = false; // init flag.
			}
			integer icounter_truncation = 1 + iaddR;

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
				for (integer i_1 = 1; i_1 <= n; i_1++) {
					flag[i_1] = false; // init flag.
				}

				integer* row_ind_SRloc = NULL;
				//row_ind_SR = new integer[numberofcoarcenodes + 1];
				//row_ind_SRloc = (integer*)malloc((numberofcoarcenodes + 1) * sizeof(integer));
				//handle_error(row_ind_SRloc, "row_ind_SRloc", "classic_aglomerative_amg_6", (numberofcoarcenodes + 1));
				row_ind_SRloc = (integer*)malloc((i_size_75 + 1) * sizeof(integer));
				handle_error(row_ind_SRloc, "row_ind_SRloc", "classic_aglomerative_amg_6", (i_size_75 + 1));

#if doubleintprecision == 1
				//printf("numberofcoarcenodes=%lld i_size_75=%lld\n", numberofcoarcenodes, i_size_75);
#else
				//printf("numberofcoarcenodes=%d i_size_75=%d\n", numberofcoarcenodes, i_size_75);
#endif

				//system("pause");
				/*
				#pragma omp parallel for
				for (integer i_1 = 1; i_1 <= numberofcoarcenodes; i_1++) {
				row_ind_SRloc[i_1] = -1;
				}
				*/
				// inicialization
#pragma omp parallel for
				for (integer i_1 = 1; i_1 <= i_size_75; i_1++) {
					row_ind_SRloc[i_1] = -1;
				}


				// ��� ������ ��������������.
				for (integer ii = 1 + iaddR; ii <= iend_marker_position; ii++) {
					if (flag[P[ii].j] == false) {
						row_ind_SRloc[P[ii].j] = ii;
						flag[P[ii].j] = true;
					}
				}

				//for (integer ii = 1 + iaddR; ii <= iend_marker_position; ii++) {
#pragma omp parallel for
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
								R[icounter_truncation] = P[ii_65];
								R[icounter_truncation].aij = fabs(dsum_plus / dsum_plus_new)*P[ii_65].aij;
								icounter_truncation++;
							}
							if ((P[ii_65].aij < 0) && (fabs(P[ii_65].aij) > alpha_truncation*dmax_minus)) {
								R[icounter_truncation] = P[ii_65];
								R[icounter_truncation].aij = fabs(dsum_minus / dsum_minus_new)*P[ii_65].aij;
								icounter_truncation++;
							}
							ii_65++;
						}

					}
				}

				free(row_ind_SRloc);
				row_ind_SRloc = NULL;
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
								R[icounter_truncation] = P[ii_65];
								R[icounter_truncation].aij = fabs(dsum_plus / dsum_plus_new)*P[ii_65].aij;
								icounter_truncation++;
							}
							if ((P[ii_65].aij < 0) && (fabs(P[ii_65].aij) > alpha_truncation*dmax_minus)) {
								R[icounter_truncation] = P[ii_65];
								R[icounter_truncation].aij = fabs(dsum_minus / dsum_minus_new)*P[ii_65].aij;
								icounter_truncation++;
							}
							ii_65++;
						}

					}
				}
			}

			//iend_marker_position = iaddR + nnzR - 1;

			iend_marker_position = icounter_truncation - 1;
			nnzR = iend_marker_position - iaddR + 1;
			//nnzR = icount1 - iaddR;
			icount1 = nnzR + iaddR;

#pragma omp parallel for
			for (integer ii = 1 + iaddR; ii <= iend_marker_position; ii++) {
				P[ii] = R[ii];
			}

#pragma omp parallel for
			for (integer i_1 = 1; i_1 <= n; i_1++) {
				flag[i_1] = false; // init flag.
			}
		}

#pragma omp parallel for
		for (integer ii = 1 + iaddR; ii <= iend_marker_position; ii++) {
			R[ii] = P[ii];
		}

		// ���� �������� ����� ��� ���������� grid complexity ��� ��������� 
		// ������������� � ��������. ������ ���������� ����� ��� ����������� ���������� ���������� ������.
		if (ilevel - 1 == 0) {
			nnz_P_memo_0 = iend_marker_position - (iaddR + 1) + 1;
		}
		else {
			nnz_P_memo_all = iend_marker_position;
		}

		

		// ��� �� ���� ��������� �� ap, �.�. 
		// R=P/ap. ????  
		// ��� ������ �� ����!!! �.�. � ������ R=transpose(P).



		// heapsort(R,key==i,iaddR+1,iaddR+nnzR - 1);

		switch (imy_sort_algorithm) {
		case COUNTING_SORT_ALG:
			Counting_Sort(R, 1 + iaddR, iaddR + nnzR - 1, false);
			break;
		case QUICK_SORT_ALG:
			qs(R, 1 + iaddR, iaddR + nnzR - 1);
			// ������������ ��������. O(nlog(n)).
			// �� ���������� ������ ������.
			//std::sort(R + (1 + iaddR) * sizeof(Ak1), R + (iaddR + nnzR - 1+1) * sizeof(Ak1), compAi);
			break;
		case HEAP_SORT_ALG:
			HeapSort(R, 1 + iaddR, iaddR + nnzR - 1);
			break;
		default:
			Counting_Sort(R, 1 + iaddR, iaddR + nnzR - 1, false);
			break;
		}
		

		if (bprint_mesage_diagnostic) {
#if doubleintprecision == 1
			printf("first level size n=%lld numberofcoarcenodes=%lld\n", n, numberofcoarcenodes);
#else
			printf("first level size n=%d numberofcoarcenodes=%d\n", n, numberofcoarcenodes);
#endif

		}

		// �������� Restriction ��� �� ��������� ����� ��� �������������: 
		if (1) {
#pragma omp parallel for
			for (integer i_1 = 1; i_1 <= numberofcoarcenodes; i_1++) {
				flag[i_1] = false; // init flag.
			}
			for (integer i_1 = 1 + iaddR; i_1 <= iaddR + nnzR - 1; i_1++) {
				if (flag[R[i_1].i] == false)
				{
					doublerealT dsum27 = 0.0;
					for (integer i_2 = i_1; (i_2 <= iaddR + nnzR - 1) && (R[i_2].i == R[i_1].i); i_2++) {
						dsum27 += fabs(R[i_2].aij);
					}
					if (dsum27 < 1.0e-37) {
#if doubleintprecision == 1
						printf("fatal error!!! zero string R[%lld][j]=%e\n", R[i_1].i, dsum27);
#else
						printf("fatal error!!! zero string R[%d][j]=%e\n", R[i_1].i, dsum27);
#endif

						system("PAUSE");
					}
					flag[R[i_1].i] = true;
				}
			}
			for (integer i_1 = 1; i_1 <= numberofcoarcenodes; i_1++) {
				if (flag[i_1] == false) {
					// ������� ������ ����� i_1
#if doubleintprecision == 1
					printf("fatal error!!! string number %lld propushena\n", i_1);
#else
					printf("fatal error!!! string number %d propushena\n", i_1);
#endif

					system("PAUSE");
				}
			}
#pragma omp parallel for
			for (integer i_1 = 1; i_1 <= numberofcoarcenodes; i_1++) {
				flag[i_1] = false; // init flag.
			}
		}

		
		if (ap_coarse != NULL) {
			free(ap_coarse);
			ap_coarse = NULL;
		}
	

		// MARKER GUSTAVSON

		// ���������� ������� �������������� ������ :
		// Acorse=R*Afine*P;
		// ����� 1 : R*Afine.
		//         xxxxxx
		//         xxxxxx
		//  xxxxxx xxxxxx xxxxxx
		//  xxxxxx xxxxxx xxxxxx
		//         xxxxxx
		//         xxxxxx
		//    R       Amat     [RA]


		if (bprint_mesage_diagnostic) {
#if doubleintprecision == 1
			printf("nnz left operand=%lld, nnz right operand=%lld\n", nnzR, nnz_a[ilevel - 1]);
#else
			printf("nnz left operand=%d, nnz right operand=%d\n", nnzR, nnz_a[ilevel - 1]);
#endif

		}

		integer istartAnew;


		// ���� ��������� IBM 1978.
		// 23 ������� 2015 ����.

		// ����� 1 : R*Afine.
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
		case COUNTING_SORT_ALG :
		Counting_Sort(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd);
		break;
		case QUICK_SORT_ALG  :
		qs(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd);
		// ������������ ��������. O(nlog(n)).
		// �� ���������� ������ ������.
		//std::sort(Amat + (1 + iadd)*sizeof(Ak1), Amat + (nnz_a[ilevel - 1] + iadd)*sizeof(Ak1), compAi);
		break;
		case HEAP_SORT_ALG:
		HeapSort(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd);
		break;
		default:
		Counting_Sort(Amat, 1 + iadd, nnz_a[ilevel - 1] + iadd);
		break;
		}
		*/
		// �������������� � ������� CRS.

		row_ind_SR = NULL;
		//row_ind_SR = new integer[numberofcoarcenodes + 1];
		row_ind_SR = (integer*)malloc((numberofcoarcenodes + 1) * sizeof(integer));
		handle_error(row_ind_SR, "row_ind_SR", "classic_aglomerative_amg_6", (numberofcoarcenodes + 1));

		row_ind_ER = NULL;
		//row_ind_ER = new integer[numberofcoarcenodes + 1];
		row_ind_ER = (integer*)malloc((numberofcoarcenodes + 1) * sizeof(integer));
		handle_error(row_ind_ER, "row_ind_ER", "classic_aglomerative_amg_6", (numberofcoarcenodes + 1));

#pragma omp parallel for
		for (integer i_1 = 1; i_1 <= numberofcoarcenodes; i_1++) {
			row_ind_SR[i_1] = -1;
			row_ind_ER[i_1] = -2;
		}
		istart1 = 1 + iaddR;
		iend1 = nnzR - 1 + iaddR;
#pragma omp parallel for
		for (integer i = 1; i <= icounter - 1; i++) {
			flag[i] = false;
		}

		
			integer i_size_75 = 0;
			// ��� ������ ��������������.
			for (integer ii = istart1; ii <= iend1; ii++) if (flag[R[ii].i] == false) {
				row_ind_SR[R[ii].i] = ii;
				flag[R[ii].i] = true;
				i_size_75++;
			}
#pragma omp parallel for
			for (integer istr = 1; istr <= i_size_75; istr++) {
				integer kf = row_ind_SR[istr];
				while ((kf <= iend1) && (R[kf].i == istr)) {
					kf++;
				}
				kf--;
				row_ind_ER[istr] = kf;
			}		


		row_ind_SA = NULL;
		row_ind_SA = (integer*)malloc((n_a[ilevel - 1] + 1) * sizeof(integer));
		handle_error(row_ind_SA, "row_ind_SA", "classic_aglomerative_amg_6", (n_a[ilevel - 1] + 1));

		row_ind_EA = NULL;
		row_ind_EA = (integer*)malloc((n_a[ilevel - 1] + 1) * sizeof(integer));
		handle_error(row_ind_EA, "row_ind_EA", "classic_aglomerative_amg_6", (n_a[ilevel - 1] + 1));

		istart3 = 1 + iadd;
		iend3 = nnz_a[ilevel - 1] + iadd;
#pragma omp parallel for
		for (integer i = 1; i <= n_a[ilevel - 1]; i++) {
			flag[i] = false;
		}

		
		
			// ������������� ������.

			 i_size_75 = 0;
			// ��� ������ ��������������.
			for (integer ii = istart3; ii <= iend3; ii++) {
				if (flag[Amat[ii].i] == false) {
					row_ind_SA[Amat[ii].i] = ii;
					flag[Amat[ii].i] = true;
					i_size_75++;
				}
			}
#pragma omp parallel for
			for (integer istr = 1; istr <= i_size_75; istr++) {
				integer kf = row_ind_SA[istr];
				while ((kf <= iend3) && (Amat[kf].i == istr)) {
					kf++;
				}
				kf--;
				row_ind_EA[istr] = kf;

			}
		





		istartAnew = nnz_a[ilevel - 1] + 1 + iadd;
		istartAnew_mem = istartAnew;

		// ������ ������������ ��� ���������� ������������� �����.
		vector_sum = NULL;
		vector_sum = (doublerealT*)malloc((n_a[ilevel - 1] + 1) * sizeof(doublerealT));
		handle_error(vector_sum, "vector_sum", "classic_aglomerative_amg_6", (n_a[ilevel - 1] + 1));

		// ������ ������� ��������� ��������� � ��������������� �������.
		index_visit = NULL;
		index_visit = (integer*)malloc((n_a[ilevel - 1] + 1) * sizeof(integer));
		handle_error(index_visit, "index_visit", "classic_aglomerative_amg_6", (n_a[ilevel - 1] + 1));

		index_size = 0;



		//#ifdef _NONAME_STUB29_10_2017
#ifdef _OPENMP

		// ������ ������������ ��� ���������� ������������� �����.

		for (integer i_9 = 0; i_9 < iKnumber_thread; i_9++) {

			for (integer i_91 = 0; i_91 < 10 * n + 1; i_91++) hash_table_m[i_9][i_91] = false;// inicialization
			index_size_m[i_9] = 0;
			istartAnew_m[i_9] = 0;
		}

		// ��������� ������ ������� ���������.
		// ���������� ���������� �� �������������.
#pragma omp parallel for 
		for (integer istr = 1; istr <= numberofcoarcenodes; istr++) {
			int tid = omp_get_thread_num();

			// �� ������ hash �������. 

			// ��������� ������� i-�� ������ �����������
			for (integer ii = row_ind_SR[istr]; ii <= row_ind_ER[istr]; ii++) {
				integer col_ind = R[ii].j;
				// ��������� col_ind ������ ������� ��������

				// ����� ���������� ������ �� ������� ����.
				doublerealT left_operand = R[ii].aij;
				for (integer i_1 = row_ind_SA[col_ind]; i_1 <= row_ind_EA[col_ind]; i_1++) {

					doublerealT right_operand = Amat[i_1].aij;
					integer iaddind = Amat[i_1].j;
					bool foundnow = false;


					foundnow = hash_table_m[tid][iaddind];


					if (foundnow) {

						vector_sum_m[tid][iaddind] += left_operand*right_operand;
					}
					else {
						// ������ ����������.

						index_size_m[tid]++;
						index_visit_m[tid][index_size_m[tid]] = iaddind;

						hash_table_m[tid][iaddind] = true;

						vector_sum_m[tid][iaddind] = left_operand*right_operand;

					}
				}
			}

			doublerealT maxth = -1.0;
			// 22 ������� 2016 �� ���������� ������ �� ����� P*Amat.
			for (integer i_6 = 1; i_6 <= index_size_m[tid]; i_6++) {
				integer jstr = index_visit_m[tid][i_6];
				hash_table_m[tid][jstr] = false; // �������������� hash ������� ��� ��������� ��������.
			}

			// ��� ����� ����������� ������� ��� ������� �� ������������
			// ����������.
			bool bCheck_ok = false; // ���������� ������� ��������� � ������ �������.
			integer istartAnew_8 = istartAnew; // ���������� ��� ����������� ������.
			
			if (nsizeA > istartAnew + index_size_m[tid]) {
				for (integer i_6 = 1; i_6 <= index_size_m[tid]; i_6++) {
					integer jstr = index_visit_m[tid][i_6];

					doublerealT vs1 = vector_sum_m[tid][jstr];

					if ((istr == jstr) && (vs1 > 1.0e-20)) {
						bCheck_ok = true;
					}



					// 7 ������ 2016 ���������� ������ ����:
					if (fabs(vs1) > 1.0e-37) {
						// �� �� ���������� � ������� ������ ����.
						Ak1 Atemp;
						Atemp.aij = vs1;
						Atemp.i = istr;
						Atemp.j = jstr;



						AccumulqtorA_m[tid][istartAnew_m[tid]++] = Atemp;

					}


				}
			}
			else {
				// ������� ���� ������ �������� ��� ������� � � � �� �� ��������� ��� ������.
				// ����� ��������� ����� ���������� ������ ��� � � ������������� ����������.
				printf("Amat lack of memory\n");
				printf("yuo mast increase the size of the matrix Amat and restart solver\n");
				printf("please, press any key to exit.\n");
				system("pause");
				exit(1);
			}



			index_size_m[tid] = 0; // ����� ����������, ������ ����������.			

		}



		printf("oK. Counting Sort start.\n");
		for (integer i_9 = 0; i_9 < iKnumber_thread; i_9++)
		{
			for (integer i_92 = 0; i_92 < istartAnew_m[i_9]; i_92++) {
				Amat[istartAnew++] = AccumulqtorA_m[i_9][i_92];
			}
		}

		Counting_Sort(Amat, istartAnew_mem, istartAnew - 1, false);
		printf("Counting Sort End. \n");

		//getchar();

#else

		// ��������� ������ ������� ���������.
		for (integer istr = 1; istr <= numberofcoarcenodes; istr++) {

			// �������� ������������ ����� ������.
			// ����� ������������� ����� � ����.
			
			node_AVL_Gus* root_Gus = 0;

		
		
			

				// �� ������ hash �������. 

				// ��������� ������� i-�� ������ �����������
				for (integer ii = row_ind_SR[istr]; ii <= row_ind_ER[istr]; ii++) {
					integer col_ind = R[ii].j;
					// ��������� col_ind ������ ������� ��������

					// ����� ���������� ������ �� ������� ����.
					doublerealT left_operand = R[ii].aij;
					for (integer i_1 = row_ind_SA[col_ind]; i_1 <= row_ind_EA[col_ind]; i_1++) {

						doublerealT right_operand = Amat[i_1].aij;
						integer iaddind = Amat[i_1].j;
						bool foundnow = false;
						
						// ����� .
						foundnow = hash_table[iaddind];

						if (foundnow) {
							vector_sum[iaddind] += left_operand*right_operand;
						}
						else {
							// ������ ����������.
							index_size++;
							index_visit[index_size] = iaddind;
							// ������� 							
							hash_table[iaddind] = true;
							vector_sum[iaddind] = left_operand*right_operand;
						}

						
					}
				}
			


			doublerealT maxth = -1.0;
			// 22 ������� 2016 �� ���������� ������ �� ����� P*Amat.
			for (integer i_6 = 1; i_6 <= index_size; i_6++) {
				integer jstr = index_visit[i_6];
				hash_table[jstr] = false; // �������������� hash ������� ��� ��������� ��������.
			}			

			// ��� ����� ����������� ������� ��� ������� �� ������������
			// ����������.
			bool bCheck_ok = false; // ���������� ������� ��������� � ������ �������.
			integer istartAnew_8 = istartAnew; // ���������� ��� ����������� ������.
			
			if (nsizeA > istartAnew + index_size) {
				for (integer i_6 = 1; i_6 <= index_size; i_6++) {
					integer jstr = index_visit[i_6];

					doublerealT vs1 = vector_sum[jstr];

					if ((istr == jstr) && (vs1 > 1.0e-20)) {
						bCheck_ok = true;
					}

					// 22 ������� 2016. ��������� �������� ������ �� ����� P*Amat ������������.
					

					// 7 ������ 2016 ���������� ������ ����:
					if (fabs(vs1) > 1.0e-37) {
						// �� �� ���������� � ������� ������ ����.
						Ak1 Atemp;
						Atemp.aij = vs1;
						Atemp.i = istr;
						Atemp.j = jstr;
						Amat[istartAnew++] = Atemp;
					}

				}
			}
			else {
				// ������� ���� ������ �������� ��� ������� � � � �� �� ��������� ��� ������.
				// ����� ��������� ����� ���������� ������ ��� � � ������������� ����������.
				printf("Amat lack of memory\n");
				printf("yuo mast increase the size of the matrix Amat and restart solver\n");
				printf("please, press any key to exit.\n");
				system("pause");
				exit(1);
			}			

			index_size = 0; // ����� ����������, ������ ����������.
			clear_AVL_Gus(root_Gus);
			root_Gus = 0;

		}

#endif

		
		if (index_visit != NULL) {
			free(index_visit);
			index_visit = NULL;
		}
		if (row_ind_SR!=NULL) {
			free(row_ind_SR);
			row_ind_SR = NULL;
		}
		if (row_ind_ER != NULL) {
			free(row_ind_ER);
			row_ind_ER = NULL;
		}
		if (row_ind_SA != NULL) {
			free(row_ind_SA);
			row_ind_SA = NULL;
		}
		if (row_ind_EA != NULL) {
			free(row_ind_EA);
			row_ind_EA = NULL;
		}
		if (vector_sum != NULL) {
			free(vector_sum);
			vector_sum = NULL;
		}



		// ����� 2. [R*Afine]*P=Abuf*P.
		// ���������� [R*�] �� i.
		//heapsort(Amat, key=i*n_coarce + j, 1, nnz_a[ilevel - 1]);

		// � ���������� ������ ��������� ������������ ���������� ��������� �� ����������,
		// �� � ��� ����� ��������������� �� ������� ���������, ������� �������������� 
		// ���������� �� ���������. ��� ��������� 11 ������ 2016.
		// 11 ������ 2016.

		


		// Prolongation ������ ���� ����������� �� j.
		// ��������� ������� ��������� ������� �������������� ������.
		istartAnew2 = istartAnew;



		// ������� ����� ���� �� ������ ���� ������� ������� ��� �� �����.
		// 17 ������� 2015. ����� ��������� � ������� ����������.
		if (bprint_mesage_diagnostic) {
#if doubleintprecision == 1
			printf("nnz left operand=%lld, nnz right operand=%lld\n", istartAnew - (nnz_a[ilevel - 1] + 1 + iadd), nnzR);
#else
			printf("nnz left operand=%d, nnz right operand=%d\n", istartAnew - (nnz_a[ilevel - 1] + 1 + iadd), nnzR);
#endif

		}



		// ���� ��������� IBM 1978
		// � ���� ���� ���������� ���� �� ������ ���������,
		// � �� ����� ��� � ���������� ����������� ���������� ������ ���� :
		// (�������, �������, ����������) ������� ���� �������������� ������ �������
		// �� ��������� (���������) � ��������� �������� 30 � 1. 30 ��������� �� ���� �����������.
		// 23 ������� 2015 ����.
		// 6 ������ 2016 ���� ��������� ��� ������.


		// ������� ������ ��������� ����� ����������.
		// IBM 1978 Sparse Matrix multiplication.

		// ���������� ����������� ���������.
		// �������������� ����� ������ � ������ CRS.
		switch (imy_sort_algorithm) {
		case COUNTING_SORT_ALG:
			Counting_Sortj(P, 1 + iaddR, iaddR + nnzR - 1);
			break;
		case QUICK_SORT_ALG:
			qsj(P, 1 + iaddR, iaddR + nnzR - 1);
			// ������������ ��������. O(nlog(n)).
			// �� ���������� ������ ������.
			//std::sort(P + (1 + iaddR) * sizeof(Ak1), P + (iaddR + nnzR - 1+1) * sizeof(Ak1), compAj);
			break;
		case HEAP_SORT_ALG:
			HeapSort_j(P, 1 + iaddR, iaddR + nnzR - 1);
			break;
		default:
			Counting_Sortj(P, 1 + iaddR, iaddR + nnzR - 1);
			break;
		}


		row_ind_AS = NULL;
		//row_ind_AS = new integer[numberofcoarcenodes + 1];
		row_ind_AS = (integer*)malloc((numberofcoarcenodes + 1) * sizeof(integer));
		handle_error(row_ind_AS, "row_ind_AS", "classic_aglomerative_amg_6", (numberofcoarcenodes + 1));

		row_ind_AE = NULL;
		//row_ind_AE = new integer[numberofcoarcenodes + 1];
		row_ind_AE = (integer*)malloc((numberofcoarcenodes + 1) * sizeof(integer));
		handle_error(row_ind_AE, "row_ind_AE", "classic_aglomerative_amg_6", (numberofcoarcenodes + 1));

		istart2 = nnz_a[ilevel - 1] + 1 + iadd;
		iend2 = istartAnew - 1;
#pragma omp parallel for
		for (integer i = 1; i <= n; i++) {
			flag[i] = false;
		}
		//int istr_memo = -1;
		for (integer ii = istart2; ii <= iend2; ii++) {
			if (flag[Amat[ii].i] == false) {
				// ��������� ���������.
				integer istr = Amat[ii].i;
				integer ic = ii;
				//istr_memo = istr;
				integer kf = ic;

				while ((kf <= iend2) && (Amat[kf].i == istr)) {
					kf++;
				}
				kf--;
				row_ind_AS[istr] = ic;
				row_ind_AE[istr] = kf;
				//if (ii > istart2) {
				//row_ind_AE[istr - 1] = ic - 1;
				//}
				flag[Amat[ii].i] = true;
				ii = kf;

			}
		}
		//row_ind_AE[istr_memo] = iend2;

		row_ind_PS = NULL;
		//row_ind_PS = new integer[n_a[ilevel - 1] + 1];
		row_ind_PS = (integer*)malloc((n_a[ilevel - 1] + 1) * sizeof(integer));
		handle_error(row_ind_PS, "row_ind_PS", "classic_aglomerative_amg_6", (n_a[ilevel - 1] + 1));

		row_ind_PE = NULL;
		//row_ind_PE = new integer[n_a[ilevel - 1] + 1];
		row_ind_PE = (integer*)malloc((n_a[ilevel - 1] + 1) * sizeof(integer));
		handle_error(row_ind_PE, "row_ind_PE", "classic_aglomerative_amg_6", (n_a[ilevel - 1] + 1));


		// ������������� ����������� �����, �.�. 
		// ����������� ������������ ������ ������ �������
		// ���� ��������� ������������.
#pragma omp parallel for
		for (integer ii = 1; ii <= n_a[ilevel - 1]; ii++) {
			row_ind_PS[ii] = -1; // �������������.
			row_ind_PE[ii] = -2;
		}
		istart4 = 1 + iaddR;
		iend4 = nnzR - 1 + iaddR;
#pragma omp parallel for
		for (integer i = 1; i <= n; i++) {
			flag[i] = false;
		}
		for (integer ii = istart4; ii <= iend4; ii++) {
			if (flag[P[ii].j] == false) {
				// ��������� ���������.
				integer istr = P[ii].j;
				integer ic = ii;

				integer kf = ic;

				while ((kf <= iend4) && (P[kf].j == istr)) {
					kf++;
				}
				kf--;
				row_ind_PS[istr] = ic;
				row_ind_PE[istr] = kf;
				flag[P[ii].j] = true;
				ii = kf;

			}
		}

		

		// ���������� ����������.
		//vector_sum = new doublerealT[numberofcoarcenodes + 1];
		if (vector_sum != NULL) {
			free(vector_sum);
			vector_sum = NULL;
		}
		vector_sum = (doublerealT*)malloc((numberofcoarcenodes + 1) * sizeof(doublerealT));
		handle_error(vector_sum, "vector_sum", "classic_aglomerative_amg_6", (numberofcoarcenodes + 1));

		//integer size_v = sizeof(doublerealT)*(1 + numberofcoarcenodes);
		// ������ ������� ��������� ��������� � ��������������� �������.
		//index_visit = new integer[n_a[ilevel - 1] + 1];
		if (index_visit != NULL) {
			free(index_visit);
			index_visit = NULL;
		}
		index_visit = (integer*)malloc((n_a[ilevel - 1] + 1) * sizeof(integer));
		handle_error(index_visit, "index_visit", "classic_aglomerative_amg_6", (n_a[ilevel - 1] + 1));

		index_visit[0] = 0;
		index_size = 0;

		//#ifdef _NONAME_STUB29_10_2017
#ifdef _OPENMP

		// ������ ������������ ��� ���������� ������������� �����.

		for (integer i_9 = 0; i_9 < iKnumber_thread; i_9++) {

			for (integer i_91 = 0; i_91 < 10 * n + 1; i_91++) hash_table_m[i_9][i_91] = false;// inicialization
			index_size_m[i_9] = 0;
			istartAnew_m[i_9] = 0;
		}


		// �� ����� ����������� ����� ������� ���������, �
		// ����� ��������� ��������� ����� ������ ������ ��������
		// �������� ������� ������ ����������.

		// ��������� ������ ������� ���������.
		// ���������� ���������� �� �������������.
#pragma omp parallel for 
		for (integer istr = 1; istr <= numberofcoarcenodes; istr++) {

			int tid = omp_get_thread_num();

			// �� ������ hash �������.
			// ��������� ��� �������� ������ ������ ��������.
			for (integer ii1 = row_ind_AS[istr]; ii1 <= row_ind_AE[istr]; ii1++) {
				integer col_ind = Amat[ii1].j;
				doublerealT left_operand = Amat[ii1].aij;

				// ��������� col_ind ������ ������� �������� ���������� �����.
				for (integer ii2 = row_ind_PS[col_ind]; ii2 <= row_ind_PE[col_ind]; ii2++) {

					doublerealT right_operand = P[ii2].aij;

					integer iaddind = P[ii2].i;
					bool foundnow = false;

					// ���������� ����� �� O(1).
					foundnow = hash_table_m[tid][iaddind];

					if (foundnow) {
						//vector_sum[index_visit[ifoundind]] += left_operand*right_operand;
						vector_sum_m[tid][iaddind] += left_operand*right_operand;
					}
					else {
						// ������ ����������.
						index_size_m[tid]++;
						index_visit_m[tid][index_size_m[tid]] = iaddind;

						// ���������� ������� � hash table �� O(1).
						hash_table_m[tid][iaddind] = true;

						//ifoundind = index_size;
						//vector_sum[index_visit[ifoundind]] = left_operand*right_operand;
						vector_sum_m[tid][iaddind] = left_operand*right_operand;
					}
					// ��������� ����������� ��������� ������ :
					// 1. ����� �������� �� ����� 
					// 2. ���� ������� �� ������ �� ���������� ������ ���� �� ��������� ����� �������� ������������.
					// ���� ������� ������ �� ����� ������ �������� foundnow �� true. 
					// �.�. ���������� ������ ������ � �������.
					// 3. � ����� ������ ���������� �������������.
					// ��� ������ ������������� ����.


					//vector_sum[P[ii2].i] += rleft*rright;
				}
			}

			doublerealT maxth = -1.0;
			for (integer i_6 = 1; i_6 <= index_size_m[tid]; i_6++) {
				integer jstr = index_visit_m[tid][i_6];
				hash_table_m[tid][jstr] = false; // initialization hash.
				if (istr != jstr) {
					// 14 ������ 2016 ����.
					// ��������� ���������� ������ ������ �� ��������������� ���������.
					if (fabs(vector_sum_m[tid][jstr]) > maxth) maxth = fabs(vector_sum_m[tid][jstr]);
				}
			}

			// huck : 16.04.2017

			for (integer i_61 = 1; i_61 <= index_size_m[tid]; i_61++) {

				integer jstr61 = index_visit_m[tid][i_61];
				doublerealT vs161 = vector_sum_m[tid][jstr61];
#if doubleintprecision == 1
				//printf("i=%lld j=%lld aij=%e\n", istr, jstr61, vs161);
#else
				//printf("i=%d j=%d aij=%e\n", istr, jstr61, vs161);
#endif


				if ((istr == jstr61) && (vs161 < 1.0e-20)) {
					// ������������� ������� �� ���������.

					printf("Negative diagonal coefficient found. No panic. Upwind patching. 16.04.2017. \n");

					printf("bad string: \n");
					for (integer i_63 = 1; i_63 <= index_size_m[tid]; i_63++) {

						integer jstr63 = index_visit_m[tid][i_63];
						doublerealT vs163 = vector_sum_m[tid][jstr63];
#if doubleintprecision == 1
						printf("i=%lld j=%lld aij=%e\n", istr, jstr63, vs163);
#else
						printf("i=%d j=%d aij=%e\n", istr, jstr63, vs163);
#endif

					}


					// �������������� ��� ������� ���-�����:
					// Amat. ��������� ����������� ����� ������� ������ ������������� ��������������� ������������� +
					// �������� �� ����� ������������� ���������. ����� �������� �� ���.
					// B. �������� ������������� ��������������� �������������.
					// C. ������ ��������� ������������� ��������������� ������������� (�������������).
					printf("patching string 16.04.2017 : \n");
					for (integer i_62 = 1; i_62 <= index_size_m[tid]; i_62++) {
						integer jstr62 = index_visit_m[tid][i_62];
						if (istr != jstr62) {
							if (vector_sum_m[tid][jstr62] > 0.0) {
								index_visit_m[tid][i_62] = -1; // �� ���������� ������ �������� (�������������).
								vector_sum_m[tid][jstr61] += vector_sum_m[tid][jstr62];
								vector_sum_m[tid][jstr62] = 0.0;
							}
						}
					}

					if (vector_sum_m[tid][jstr61] < 0.0) {
						vector_sum_m[tid][jstr61] = 0.0;
						for (integer i_62 = 1; i_62 <= index_size_m[tid]; i_62++) {
							integer jstr62 = index_visit_m[tid][i_62];
							if (jstr62 > -1) {
								if (istr != jstr62) {
									if (vector_sum_m[tid][jstr62] < 0.0) {
										vector_sum_m[tid][jstr61] += fabs(vector_sum_m[tid][jstr62]);
									}
								}
							}
						}
					}

					// ����� �� ����� for �� ���������� i_61.
					break;
				}


			}


			bool bCheck_ok = false; // ���������� ������� ��������� � ������ �������.
									// ��� ����� ����������� ������� ��� ������� �� ������������ ����������.
			
			if (nsizeA > istartAnew2 + index_size_m[tid]) {
				for (integer i_6 = 1; i_6 <= index_size_m[tid]; i_6++) {

					integer jstr = index_visit_m[tid][i_6];
					doublerealT vs1 = vector_sum_m[tid][jstr];
					//if (fabs(vs1) < 1.0e-37) {
#if doubleintprecision == 1
					//printf("zero vs1=%e, i==%lld j==%lld\n",vs1,istr,jstr);
#else
					//printf("zero vs1=%e, i==%d j==%d\n",vs1,istr,jstr);
#endif

					//}
					// 7 ������ 2016 ���������� ������ ����:
					if ((jstr>-1) && (fabs(vs1) > 1.0e-37)) {
						// �� ���������� ������ ����. 
						// �� ������ ������ ��������� ������ ��� ����������.

						
							// �������������� ���������� ��������.
							// 22_10_2016.
							Ak1 Atemp;
							Atemp.aij = vs1;
							Atemp.i = istr;
							Atemp.j = jstr;

							if (istr == jstr) bCheck_ok = true;

							if ((istr == jstr) && (vs1 < 1.0e-20)) {
								// ������ ����������� � ���������� ������������� �������� � �������������� ������� �������
								// ������������ ���������. ���� �������� �������� ���� �� ����.
								// 22737
								// ��������� ��� �������� ������ ������ ��������.
								//for (integer ii1_8 = row_ind_AS[istr]; ii1_8 <= row_ind_AE[istr]; ii1_8++) {
								//if (Amat[ii1_8].i == 22737) {
#if doubleintprecision == 1
								//printf("i=%lld j=%lld aij=%e\n", Amat[ii1_8].i, Amat[ii1_8].j, Amat[ii1_8].aij);
#else
								//printf("i=%d j=%d aij=%e\n", Amat[ii1_8].i, Amat[ii1_8].j, Amat[ii1_8].aij);
#endif

								//}
								//integer col_ind = Amat[ii1_8].j;
								//}
#if doubleintprecision == 1
								printf("bad string %lld\n", istr);
#else
								printf("bad string %d\n", istr);
#endif

								printf("error : diagonal element is negative...\n");
								switch (iVar) {
								case PAM: printf("PAM equation\n"); break;
								case VX: printf("VX equation\n"); break;
								case VY: printf("VY equation\n"); break;
								case VZ: printf("VZ equation\n"); break;
								case TEMP: printf("TEMP equation\n"); break;
								case TOTALDEFORMATIONVAR: printf("STRESS system equation\n"); break;
								}
#if doubleintprecision == 1
								//for (integer ii1_8 = row_ind_AS[istr]; ii1_8 <= row_ind_AE[istr]; ii1_8++) {
								//printf("i=%lld j=%lld aij=%e\n", Amat[ii1_8].i, Amat[ii1_8].j, Amat[ii1_8].aij);
								//}
#else
								//for (integer ii1_8 = row_ind_AS[istr]; ii1_8 <= row_ind_AE[istr]; ii1_8++) {
								//printf("i=%d j=%d aij=%e\n", Amat[ii1_8].i, Amat[ii1_8].j, Amat[ii1_8].aij);
								//}
#endif

								for (integer i_61 = 1; i_61 <= index_size_m[tid]; i_61++) {

									integer jstr61 = index_visit_m[tid][i_61];
									doublerealT vs161 = vector_sum_m[tid][jstr61];
#if doubleintprecision == 1
									printf("i=%lld j=%lld aij=%e\n", istr, jstr61, vs161);
#else
									printf("i=%d j=%d aij=%e\n", istr, jstr61, vs161);
#endif

								}
								//getchar();
								system("pause");
								// ���������� ������� �������� �������.
								bcontinue_global = false;
								//goto BAD_STRING_MARKER;
								printf("fatall error bad string: goto BAD_STRING_MARKER;\n");
								system("pause");
								exit(1);

								doublerealT sum_dia = 0.0;
								for (integer i_8 = 1; i_8 <= index_size_m[tid]; i_8++) {
									if (i_8 != i_6) {
										integer jstr_8 = index_visit_m[tid][i_8];
										doublerealT vs1_8 = vector_sum_m[tid][jstr_8];
										sum_dia += fabs(vs1_8);
									}
								}
								// �������������� ���������� �������� ���������.
								Atemp.aij = sum_dia;
								// ������ �������� �� ���������� ���������.
								// 22 ������� 2016
								//system("pause");
							}

							//Amat[istartAnew2].aij = vs1;
							//Amat[istartAnew2].i = istr;
							//Amat[istartAnew2].j = jstr;
							//istartAnew2++;

							//Amat[istartAnew2++] = Atemp;
							AccumulqtorA_m[tid][istartAnew_m[tid]++] = Atemp;
						
					}
				}
			}
			else {
				// ������� ���� ������ �������� ��� ������� � � � �� �� ��������� ��� ������.
				// ����� ��������� ����� ���������� ������ ��� � � ������������� ����������.
				printf("Amat lack of memory\n");
				printf("yuo mast increase the size of the matrix Amat and restart solver\n");
				printf("please, press any key to exit.\n");
				system("pause");
				exit(1);
			}

			if (!bCheck_ok) {
#if doubleintprecision == 1
				printf("bad string %lld\n", istr);
#else
				printf("bad string %d\n", istr);
#endif

				// ���������� ������� �������� �������.
				bcontinue_global = false;
				//goto BAD_STRING_MARKER;
				printf("fatall error bad string: goto BAD_STRING_MARKER 2;\n");
				system("pause");
				exit(1);

				for (integer i_6 = 1; i_6 <= index_size_m[tid]; i_6++) {

					integer jstr = index_visit_m[tid][i_6];
					doublerealT vs1 = vector_sum_m[tid][jstr];
#if doubleintprecision == 1
					printf("%lld %lld %e\n", istr, jstr, vs1);
#else
					printf("%d %d %e\n", istr, jstr, vs1);
#endif

				}
				system("pause");
			}

			index_size_m[tid] = 0;

		}

		istartAnew_mem2 = istartAnew2;
		printf("oK2. Counting Sort start.\n");
		for (integer i_9 = 0; i_9 < iKnumber_thread; i_9++)
		{
			for (integer i_92 = 0; i_92 < istartAnew_m[i_9]; i_92++) {
				Amat[istartAnew2++] = AccumulqtorA_m[i_9][i_92];
			}
		}

		Counting_Sort(Amat, istartAnew_mem2, istartAnew2 - 1, false);
		printf("Counting Sort End. \n");

#else

		// �� ����� ����������� ����� ������� ���������, �
		// ����� ��������� ��������� ����� ������ ������ ��������
		// �������� ������� ������ ����������.

		for (integer istr = 1; istr <= numberofcoarcenodes; istr++) {

			node_AVL_Gus* root_Gus = 0;
			
				// �� ������ hash �������.
				// ��������� ��� �������� ������ ������ ��������.
				for (integer ii1 = row_ind_AS[istr]; ii1 <= row_ind_AE[istr]; ii1++) {
					integer col_ind = Amat[ii1].j;
					doublerealT left_operand = Amat[ii1].aij;

					// ��������� col_ind ������ ������� �������� ���������� �����.
					for (integer ii2 = row_ind_PS[col_ind]; ii2 <= row_ind_PE[col_ind]; ii2++) {

						doublerealT right_operand = P[ii2].aij;

						integer iaddind = P[ii2].i;
						bool foundnow = false;
					
						// ���������� ����� �� O(1).
						foundnow = hash_table[iaddind];

						if (foundnow) {
							
							vector_sum[iaddind] += left_operand*right_operand;
						}
						else {
							// ������ ����������.
							index_size++;
							index_visit[index_size] = iaddind;
							// �������
							// ���������� ������� � hash table �� O(1).
							hash_table[iaddind] = true;
							
							vector_sum[iaddind] = left_operand*right_operand;
						}
						// ��������� ����������� ��������� ������ :
						// 1. ����� �������� �� ����� 
						// 2. ���� ������� �� ������ �� ���������� ������ ���� �� ��������� ����� �������� ������������.
						// ���� ������� ������ �� ����� ������ �������� foundnow �� true. 
						// �.�. ���������� ������ ������ � �������.
						// 3. � ����� ������ ���������� �������������.
						// ��� ������ ������������� ����.

					}
				}

			


			doublerealT maxth = -1.0;
			for (integer i_6 = 1; i_6 <= index_size; i_6++) {
				integer jstr = index_visit[i_6];
				hash_table[jstr] = false; // initialization hash.
				if (istr != jstr) {
					// 14 ������ 2016 ����.
					// ��������� ���������� ������ ������ �� ��������������� ���������.
					if (fabs(vector_sum[jstr]) > maxth) maxth = fabs(vector_sum[jstr]);
				}
			}

			

			// huck : 16.04.2017

			for (integer i_61 = 1; i_61 <= index_size; i_61++) {

				integer jstr61 = index_visit[i_61];
				doublerealT vs161 = vector_sum[jstr61];



				if ((istr == jstr61) && (vs161 < 1.0e-20)) {
					// ������������� ������� �� ���������.

					printf("Negative diagonal coefficient found. No panic. Upwind patching. 16.04.2017. \n");

					printf("bad string: \n");
					for (integer i_63 = 1; i_63 <= index_size; i_63++) {

						integer jstr63 = index_visit[i_63];
						doublerealT vs163 = vector_sum[jstr63];
#if doubleintprecision == 1
						printf("i=%lld j=%lld aij=%e\n", istr, jstr63, vs163);
#else
						printf("i=%d j=%d aij=%e\n", istr, jstr63, vs163);
#endif

					}


					// �������������� ��� ������� ���-�����:
					// Amat. ��������� ����������� ����� ������� ������ ������������� ��������������� ������������� +
					// �������� �� ����� ������������� ���������. ����� �������� �� ���.
					// B. �������� ������������� ��������������� �������������.
					// C. ������ ��������� ������������� ��������������� ������������� (�������������).
					printf("patching string 16.04.2017 : \n");
					for (integer i_62 = 1; i_62 <= index_size; i_62++) {
						integer jstr62 = index_visit[i_62];
						if (istr != jstr62) {
							if (vector_sum[jstr62] > 0.0) {
								index_visit[i_62] = -1; // �� ���������� ������ �������� (�������������).
								vector_sum[jstr61] += vector_sum[jstr62];
								vector_sum[jstr62] = 0.0;
							}
						}
					}

					if (vector_sum[jstr61] < 0.0) {
						vector_sum[jstr61] = 0.0;
						for (integer i_62 = 1; i_62 <= index_size; i_62++) {
							integer jstr62 = index_visit[i_62];
							if (jstr62 > -1) {
								if (istr != jstr62) {
									if (vector_sum[jstr62] < 0.0) {
										vector_sum[jstr61] += fabs(vector_sum[jstr62]);
									}
								}
							}
						}
					}

					// ����� �� ����� for �� ���������� i_61.
					break;
				}


			}


			bool bCheck_ok = false; // ���������� ������� ��������� � ������ �������.
									// ��� ����� ����������� ������� ��� ������� �� ������������ ����������.
			
			if (nsizeA > istartAnew2 + index_size) {
				for (integer i_6 = 1; i_6 <= index_size; i_6++) {

					integer jstr = index_visit[i_6];
					doublerealT vs1 = vector_sum[jstr];
					
					// 7 ������ 2016 ���������� ������ ����:
					if ((jstr>-1) && (fabs(vs1) > 1.0e-37)) {
						// �� ���������� ������ ����. 
						// �� ������ ������ ��������� ������ ��� ����������.

						
							// �������������� ���������� ��������.
							// 22_10_2016.
							Ak1 Atemp;
							Atemp.aij = vs1;
							Atemp.i = istr;
							Atemp.j = jstr;

							if (istr == jstr) bCheck_ok = true;						

							Amat[istartAnew2++] = Atemp;
						
					}
				}
			}
			else {
				// ������� ���� ������ �������� ��� ������� � � � �� �� ��������� ��� ������.
				// ����� ��������� ����� ���������� ������ ��� � � ������������� ����������.
				printf("Amat lack of memory\n");
				printf("yuo mast increase the size of the matrix Amat and restart solver\n");
				printf("please, press any key to exit.\n");
				system("pause");
				exit(1);
			}			

			index_size = 0;
			// ������������ ������ �� ��� ��� ������.
			clear_AVL_Gus(root_Gus);
			root_Gus = 0;
		}

#endif

		
		if (vector_sum != NULL) {
			free(vector_sum);
			vector_sum = NULL;
		}
		if (index_visit != NULL) {
			free(index_visit);
			index_visit = NULL;
		}



		
		if (row_ind_AS != NULL) {
			free(row_ind_AS);
			row_ind_AS = NULL;
		}
		if (row_ind_AE != NULL) {
			free(row_ind_AE);
			row_ind_AE = NULL;
		}
		if (row_ind_PS != NULL) {
			free(row_ind_PS);
			row_ind_PS = NULL;
		}
		if (row_ind_PE != NULL) {
			free(row_ind_PE);
			row_ind_PE = NULL;
		}

		
		// �������� ������� � ���������� ������ ����� �������� � ������ ��������������� ������.
		//integer icounter3 = 1;
		nsize = istartAnew2 - (istartAnew);
		for (integer i_1 = nnz_a[ilevel - 1] + 1 + iadd, i_2 = 1; i_2 <= nsize; i_1++, i_2++) {
			integer i_right_position = istartAnew - 1 + i_2;
			Amat[i_1] = Amat[i_right_position];
		}

		if (bprint_mesage_diagnostic) {
			printf("Prolongation is construct.\n");
			// ����� ���������� ����� �� ���������� ��������, �� �� � �������� 
#if doubleintprecision == 1
			printf("diagnostic: the number of neighbors that are not Coarse (C) nodes %lld\n", the_number_of_neighbors_that_are_not_�_nodes);
			// ���������� F ����� � ������� ������ ���� ���������������� � �����.
			printf("diagnostic: the number of Fine (F) nodes with one single strong Coarse (C) neighbor=%lld \n", number_of_F_nodes_with_one_single_strong_C_neighbor);
			printf("diagnostic: the number of Fine (F) nodes with one single strong Coarse (C) neighbor\n");
			printf("and to the same not having strong Fine(F) neighbors %lld\n", number_of_F_nodes_with_one_single_strong_C_neighborF);
			//system("pause");
#else
			printf("diagnostic: the number of neighbors that are not Coarse (C) nodes %d\n", the_number_of_neighbors_that_are_not_�_nodes);
			// ���������� F ����� � ������� ������ ���� ���������������� � �����.
			printf("diagnostic: the number of Fine (F) nodes with one single strong Coarse (C) neighbor=%d \n", number_of_F_nodes_with_one_single_strong_C_neighbor);
			printf("diagnostic: the number of Fine (F) nodes with one single strong Coarse (C) neighbor\n");
			printf("and to the same not having strong Fine(F) neighbors %d\n", number_of_F_nodes_with_one_single_strong_C_neighborF);
			//system("pause");
#endif

		}
		if (debug_reshime) system("pause");


		//delete[] C_numerate;
		if (C_numerate != NULL) {
			free(C_numerate);
			C_numerate = NULL;
		}

		// ������������� �������������� ���� F-C �������� ���������� ��������������� ��������,
		// �������� ����� V ������ ��������� ��� ���������� ����������.
		iaddFCcolor = 0;
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

			if (bStrongTransposeON) {
				// ������������ ���.
				
				
					// ������� �������� ������.
					if (hash_StrongTranspose_collection1 != NULL) {
						// otkl_parr_aug2017
#pragma omp parallel for
						//for (integer i_1 = 0; i_1 <= n_a[ilevel - 2]; i_1++) {
						//isize_memory_alloc_hash_StrongTranspose_collection1
						for (integer i_1 = 0; i_1 <= isize_memory_alloc_hash_StrongTranspose_collection1; i_1++) {
							clear_list(hash_StrongTranspose_collection1[i_1]);
						}
						delete[] hash_StrongTranspose_collection1;
						hash_StrongTranspose_collection1 = NULL;
					}
				
				if (isize_hash_StrongTranspose_collection != NULL) {
					delete isize_hash_StrongTranspose_collection;
					isize_hash_StrongTranspose_collection = NULL;
				}
			}
		}
		else {
			if (bStrongTransposeON) {
				// ������������ ���.
				
					// ������� �������� ������.
					if (hash_StrongTranspose_collection1 != NULL) {
						//for (integer i_1 = 0; i_1 <= n_a[ilevel - 2]; i_1++)
						//isize_memory_alloc_hash_StrongTranspose_collection1
						for (integer i_1 = 0; i_1 <= isize_memory_alloc_hash_StrongTranspose_collection1; i_1++)
						{
							clear_list(hash_StrongTranspose_collection1[i_1]);
						}
						delete[] hash_StrongTranspose_collection1;
						hash_StrongTranspose_collection1 = NULL;
					}
				
				if (isize_hash_StrongTranspose_collection != NULL) {
					delete isize_hash_StrongTranspose_collection;
					isize_hash_StrongTranspose_collection = NULL;
				}
			}
		}

	
		if (count_sosed != NULL) {
			free(count_sosed);
			count_sosed = NULL;
		}
		
		if (row_startA != NULL) {
			free(row_startA);
			row_startA = NULL;
		}


		// ���������� �������� ������� �������� ����������.
	//BAD_STRING_MARKER:


		//exit(1);
		if (bprint_mesage_diagnostic) {
			printf("one level construct OK.\n");
		}
		if (debug_reshime) system("pause");	

		//�������� �����

	}// �������� ����� ���������.

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
		printf("Prolongation operator complexity is %1.2f %1.2f\n", (doublerealT)(nnz_P_memo_all / nnz_P_memo_0), (doublerealT)(nnz_P_memo_all / n_a[0]));
		doublerealT sizegb = 16 * iadd / 1.0e9;
		printf("memory usage is %e Gb. reserved %e Gb. ratio is equal = %e\n", sizegb, 16 * nsizeA / 1.0e9, sizegb / (16 * nsizeA / 1.0e9));
	}

	

	// 31.224s [50.986] 2D m=81 debug x64 acumulqtor
	// 13.792 [18.156] 2D m=81 realese x64 acumulqtor
	// 8.028s 2D m=81 debug x64 rozetka
	// 3.827 2D m=81 realese x64 rozetka

#if doubleintprecision == 1
	if (bprint_mesage_diagnostic) {
		printf("ilevel=%lld\n", ilevel);
		// <= ilevel 4.01.2017
		for (integer i_1 = 0; i_1 <= ilevel; i_1++) {
			printf("n_a[%lld]=%lld nnz_a[%lld]=%lld nnz_a[%lld]/n_a[%lld]=%lld\n", i_1, n_a[i_1], i_1, nnz_a[i_1], i_1, i_1, (integer)(nnz_a[i_1] / n_a[i_1]));
		}
		printf("Graph(Mesh) ierarhion is construct sucsseful...\n");
	}
#else
	if (bprint_mesage_diagnostic) {
		printf("ilevel=%d\n", ilevel);
		// <= ilevel 4.01.2017
		for (integer i_1 = 0; i_1 <= ilevel; i_1++) {
			printf("n_a[%d]=%d nnz_a[%d]=%d nnz_a[%d]/n_a[%d]=%d\n", i_1, n_a[i_1], i_1, nnz_a[i_1], i_1, i_1, (integer)(nnz_a[i_1] / n_a[i_1]));
		}
		printf("Graph(Mesh) ierarhion is construct sucsseful...\n");
	}
#endif

	if (debug_reshime) system("pause");
	//system("pause");
	//exit(1);

	if (bprint_mesage_diagnostic) {
		printf("memory optimization 13 november 2016.\n");
		printf("ierarhion matrix Amat...");
	}
	// ���������� ������ ��������� ��� �������� ������� �.
	// ������� ������ �������� � ������ �� ����� ��� ��� �� ����� � �� ���������� ������.
	if (Amat != NULL) {
		Amat = (Ak1*)realloc(Amat, (iadd + 2) * sizeof(Ak1));
	}
	if (Amat == NULL) {
		printf("application crash for Amat. Please send message on email: kirill7785@mail.ru\n");
		system("pause");
		exit(1);
	}
	if (bprint_mesage_diagnostic) {
		printf(" 1 of 3 compleated.  OK!! ierarhion matrix Amat realloc successfully...\n");
	}

	if (bprint_mesage_diagnostic) {
		printf("Prolongation ierarhion...\n");
	}
	if (P != NULL) {
		P = (Ak1*)realloc(P, ((integer)(nnz_P_memo_all)+2) * sizeof(Ak1));
	}
	if (P == NULL) {
		printf("application crash for P. Please send message on email: kirill7785@mail.ru\n");
		system("pause");
		exit(1);
	}
	if (bprint_mesage_diagnostic) {
		printf("2 of 3 compleated. OK!! ierarhion matrix Prolongation realloc successfully...\n");
	}

	if (bprint_mesage_diagnostic) {
		printf("Restriction ierarhion...\n");
	}
	if (R != NULL) {
		R = (Ak1*)realloc(R, ((integer)(nnz_P_memo_all)+2) * sizeof(Ak1));
	}
	if (R == NULL) {
		printf("application crash for R. Please send message on email: kirill7785@mail.ru\n");
		system("pause");
		exit(1);
	}
	if (bprint_mesage_diagnostic) {
		printf("3 of 3 compleated. OK!! ierarhion matrix Restriction realloc successfully...\n");
		printf("memory optimization successfully.\n");
	}

	


	// 4-5-6 30-31 dec 2016 �������������� �� ����� 50 ������� �����������
	//  5.06.2017 �������������� �� ����� 100 ������� �����������.
	// ������������.
	const integer idim_diag = 101;
	doublerealT **diag = NULL;
	diag = new doublerealT*[idim_diag];
	if (diag == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for diag my_gregat_amg.cpp...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}
	for (integer i_id_level_local = 0; i_id_level_local < idim_diag; i_id_level_local++) {
		diag[i_id_level_local] = NULL; // �������������.
		if (i_id_level_local == 0) {
			// �.�. ������� ����� ��������� �� �� ������� ���� ���� ���.
			//doublerealT *diag0 = NULL;
			//diag[0] = new doublerealT[n_a[0] + 1];
			diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
			handle_error(diag[i_id_level_local], "diag[0]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
		}
		else if (i_id_level_local == 1) {
			//doublerealT *diag1 = NULL;
			if (ilevel > i_id_level_local) {
				//diag[1] = new doublerealT[n_a[1] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[1]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 2) {
			//doublerealT *diag2 = NULL;
			if (ilevel > i_id_level_local) {
				//diag[2] = new doublerealT[n_a[2] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[2]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 3) {
			//doublerealT *diag3 = NULL;
			if (ilevel > i_id_level_local) {
				//diag[3] = new doublerealT[n_a[3] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[3]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 4) {
			//doublerealT *diag4 = NULL;
			if (ilevel > i_id_level_local) {
				//diag[4] = new doublerealT[n_a[4] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[4]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 5) {
			//doublerealT *diag5 = NULL;
			if (ilevel > i_id_level_local) {
				//diag[5] = new doublerealT[n_a[5] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[5]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 6) {
			//doublerealT *diag6 = NULL;
			if (ilevel > i_id_level_local) {
				//diag[6] = new doublerealT[n_a[6] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[6]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 7) {
			//doublerealT *diag7 = NULL;
			if (ilevel > i_id_level_local) {
				//diag[7] = new doublerealT[n_a[7] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[7]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 8) {
			//doublerealT *diag8 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[8] = new doublerealT[n_a[8] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[8]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 9) {
			//doublerealT *diag9 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[9] = new doublerealT[n_a[9] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[9]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 10) {
			//doublerealT *diag10 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[10] = new doublerealT[n_a[10] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[10]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 11) {
			//doublerealT *diag11 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[11] = new doublerealT[n_a[11] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[11]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 12) {
			//doublerealT *diag12 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[12] = new doublerealT[n_a[12] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[12]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 13) {
			//doublerealT *diag13 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[13] = new doublerealT[n_a[13] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[13]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 14) {
			//doublerealT *diag14 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[14] = new doublerealT[n_a[14] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[14]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 15) {
			//doublerealT *diag15 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[15] = new doublerealT[n_a[15] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[15]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 16) {
			//doublerealT *diag16 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[16] = new doublerealT[n_a[16] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[16]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 17) {
			//doublerealT *diag17 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[17] = new doublerealT[n_a[17] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[17]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 18) {
			//doublerealT *diag18 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[18] = new doublerealT[n_a[18] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[18]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 19) {
			//doublerealT *diag19 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[19] = new doublerealT[n_a[19] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[19]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 20) {
			//doublerealT *diag20 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[20] = new doublerealT[n_a[20] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[20]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 21) {
			//doublerealT *diag21 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[21] = new doublerealT[n_a[21] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[21]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 22) {
			//doublerealT *diag22 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[22] = new doublerealT[n_a[22] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[22]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 23) {
			//doublerealT *diag23 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[23] = new doublerealT[n_a[23] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[23]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 24) {
			//doublerealT *diag24 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[24] = new doublerealT[n_a[24] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[24]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 25) {
			//doublerealT *diag25 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[25] = new doublerealT[n_a[25] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[25]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 26) {
			//doublerealT *diag26 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[26] = new doublerealT[n_a[26] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[26]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 27) {
			//doublerealT *diag27 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[27] = new doublerealT[n_a[27] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[27]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 28) {
			//doublerealT *diag28 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[28] = new doublerealT[n_a[28] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[28]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 29) {
			//doublerealT *diag29 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[29] = new doublerealT[n_a[29] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[29]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 30) {
			//doublerealT *diag30 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[30] = new doublerealT[n_a[30] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[30]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 31) {
			//doublerealT *diag31 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[31] = new doublerealT[n_a[31] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[31]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 32) {
			//doublerealT *diag32 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[32] = new doublerealT[n_a[32] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[32]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 33) {
			//doublerealT *diag33 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[33] = new doublerealT[n_a[33] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[33]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 34) {
			//doublerealT *diag34 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[34] = new doublerealT[n_a[34] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[34]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 35) {
			//doublerealT *diag35 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[35] = new doublerealT[n_a[35] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[35]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 36) {
			//doublerealT *diag36 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[36] = new doublerealT[n_a[36] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[36]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 37) {
			//doublerealT *diag37 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[37] = new doublerealT[n_a[37] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[37]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 38) {
			//doublerealT *diag38 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[38] = new doublerealT[n_a[38] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[38]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 39) {
			//doublerealT *diag39 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[39] = new doublerealT[n_a[39] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[39]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 40) {
			//doublerealT *diag40 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[40] = new doublerealT[n_a[40] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[40]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 41) {
			//doublerealT *diag41 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[41] = new doublerealT[n_a[41] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[41]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 42) {
			//doublerealT *diag42 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[42] = new doublerealT[n_a[42] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[42]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 43) {
			//doublerealT *diag43 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[43] = new doublerealT[n_a[43] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[43]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 44) {
			//doublerealT *diag44 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[44] = new doublerealT[n_a[44] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[44]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 45) {
			//doublerealT *diag45 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[45] = new doublerealT[n_a[45] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[45]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 46) {
			//doublerealT *diag46 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[46] = new doublerealT[n_a[46] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[46]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 47) {
			//doublerealT *diag47 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[47] = new doublerealT[n_a[47] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[47]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 48) {
			//doublerealT *diag48 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[48] = new doublerealT[n_a[48] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[48]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 49) {
			//doublerealT *diag49 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[49] = new doublerealT[n_a[49] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[49]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 50) {
			//doublerealT *diag50 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[50] = new doublerealT[n_a[50] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[50]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 51) {
			//doublerealT *diag51 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[51] = new doublerealT[n_a[51] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[51]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 52) {
			//doublerealT *diag52 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[52] = new doublerealT[n_a[52] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[52]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 53) {
			//doublerealT *diag53 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[53] = new doublerealT[n_a[53] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[53]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 54) {
			//doublerealT *diag54 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[54] = new doublerealT[n_a[54] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[54]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 55) {
			//doublerealT *diag55 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[55] = new doublerealT[n_a[55] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[55]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 56) {
			//doublerealT *diag56 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[56] = new doublerealT[n_a[56] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[56]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 57) {
			//doublerealT *diag57 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[57] = new doublerealT[n_a[57] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[57]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 58) {
			//doublerealT *diag58 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[58] = new doublerealT[n_a[58] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[58]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 59) {
			//doublerealT *diag59 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[59] = new doublerealT[n_a[59] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[59]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 60) {
			//doublerealT *diag60 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[60] = new doublerealT[n_a[60] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[60]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 61) {
			//doublerealT *diag61 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[61] = new doublerealT[n_a[61] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[61]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 62) {
			//doublerealT *diag62 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[62] = new doublerealT[n_a[62] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[62]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 63) {
			//doublerealT *diag63 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[63] = new doublerealT[n_a[63] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[63]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 64) {
			//doublerealT *diag64 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[64] = new doublerealT[n_a[64] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[64]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 65) {
			//doublerealT *diag65 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[65] = new doublerealT[n_a[65] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[65]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 66) {
			//doublerealT *diag66 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[66] = new doublerealT[n_a[66] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[66]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 67) {
			//doublerealT *diag67 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[67] = new doublerealT[n_a[67] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[67]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 68) {
			//doublerealT *diag68 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[68] = new doublerealT[n_a[68] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[68]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 69) {
			//doublerealT *diag69 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[69] = new doublerealT[n_a[69] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[69]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 70) {
			//doublerealT *diag70 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[70] = new doublerealT[n_a[70] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[70]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 71) {
			//doublerealT *diag71 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[71] = new doublerealT[n_a[71] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[71]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 72) {
			//doublerealT *diag72 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[72] = new doublerealT[n_a[72] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[72]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 73) {
			//doublerealT *diag73 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[73] = new doublerealT[n_a[73] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[73]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 74) {
			//doublerealT *diag74 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[74] = new doublerealT[n_a[74] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[74]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 75) {
			//doublerealT *diag75 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[75] = new doublerealT[n_a[75] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[75]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 76) {
			//doublerealT *diag76 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[76] = new doublerealT[n_a[76] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[76]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 77) {
			//doublerealT *diag77 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[77] = new doublerealT[n_a[77] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[77]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		if (i_id_level_local == 78) {
			//doublerealT *diag78 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[78] = new doublerealT[n_a[78] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[78]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 79) {
			//doublerealT *diag79 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[79] = new doublerealT[n_a[79] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[79]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 80) {
			//doublerealT *diag80 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[80] = new doublerealT[n_a[80] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[80]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 81) {
			//doublerealT *diag81 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[81] = new doublerealT[n_a[81] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[81]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 82) {
			//doublerealT *diag82 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[82] = new doublerealT[n_a[82] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[82]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 83) {
			//doublerealT *diag83 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[83] = new doublerealT[n_a[83] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[83]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 84) {
			//doublerealT *diag84 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[84] = new doublerealT[n_a[84] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[84]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 85) {
			//doublerealT *diag85 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[85] = new doublerealT[n_a[85] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[85]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 86) {
			//doublerealT *diag86 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[86] = new doublerealT[n_a[86] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[86]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 87) {
			//doublerealT *diag87 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[87] = new doublerealT[n_a[87] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[87]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 88) {
			//doublerealT *diag88 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[88] = new doublerealT[n_a[88] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[88]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 89) {
			//doublerealT *diag89 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[89] = new doublerealT[n_a[89] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[89]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 90) {
			//doublerealT *diag90 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[90] = new doublerealT[n_a[90] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[90]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 91) {
			//doublerealT *diag91 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[91] = new doublerealT[n_a[91] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[91]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 92) {
			//doublerealT *diag92 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[92] = new doublerealT[n_a[92] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[92]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 93) {
			//doublerealT *diag93 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[93] = new doublerealT[n_a[93] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[93]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 94) {
			//doublerealT *diag94 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[94] = new doublerealT[n_a[94] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[94]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 95) {
			//doublerealT *diag95 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[95] = new doublerealT[n_a[95] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[95]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 96) {
			//doublerealT *diag96 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[96] = new doublerealT[n_a[96] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[96]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 97) {
			//doublerealT *diag97 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[97] = new doublerealT[n_a[97] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[97]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 98) {
			//doublerealT *diag98 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[98] = new doublerealT[n_a[98] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[98]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 99) {
			//doublerealT *diag99 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[99] = new doublerealT[n_a[99] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[99]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
		else if (i_id_level_local == 100) {
			//doublerealT *diag100 = NULL;
			if (ilevel > i_id_level_local) {
				// diag[100] = new doublerealT[n_a[100] + 1];
				diag[i_id_level_local] = (doublerealT*)malloc((n_a[i_id_level_local] + 1) * sizeof(doublerealT));
				handle_error(diag[i_id_level_local], "diag[100]", "classic_aglomerative_amg_6", (n_a[i_id_level_local] + 1));
			}
		}
	}


	bnested_desection_global_amg = NULL;
	bool **nested_desection = NULL;
	nested_desection = new bool*[idim_diag];
	if (nested_desection == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for nested_desection my_gregat_amg.cpp...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}
	for (integer i_id_level_local = 0; i_id_level_local < idim_diag; i_id_level_local++) {
		nested_desection[i_id_level_local] = NULL;
	}

	if (!bonly_serial) {
		// nested desection start
		bnested_desection_global_amg = (bool*)malloc((n_a[0] + 1) * sizeof(bool));
		handle_error(bnested_desection_global_amg, "bnested_desection_global_amg", "classic_aglomerative_amg_6", (n_a[0] + 1));


		nested_desection[0] = (bool*)malloc((n_a[0] + 1) * sizeof(bool));
		handle_error(nested_desection[0], "nested_desection[0]", "classic_aglomerative_amg_6", (n_a[0] + 1));


		if (ilevel > 1) {
			nested_desection[1] = (bool*)malloc((n_a[1] + 1) * sizeof(bool));
			handle_error(nested_desection[1], "nested_desection[1]", "classic_aglomerative_amg_6", (n_a[1] + 1));
		}

		if (ilevel > 2) {
			nested_desection[2] = (bool*)malloc((n_a[2] + 1) * sizeof(bool));
			handle_error(nested_desection[2], "nested_desection[2]", "classic_aglomerative_amg_6", (n_a[2] + 1));
		}

		if (ilevel > 3) {
			nested_desection[3] = (bool*)malloc((n_a[3] + 1) * sizeof(bool));
			handle_error(nested_desection[3], "nested_desection[3]", "classic_aglomerative_amg_6", (n_a[3] + 1));
		}

		if (ilevel > 4) {
			nested_desection[4] = (bool*)malloc((n_a[4] + 1) * sizeof(bool));
			handle_error(nested_desection[4], "nested_desection[4]", "classic_aglomerative_amg_6", (n_a[4] + 1));
		}

		if (ilevel > 5) {
			nested_desection[5] = (bool*)malloc((n_a[5] + 1) * sizeof(bool));
			handle_error(nested_desection[5], "nested_desection[5]", "classic_aglomerative_amg_6", (n_a[5] + 1));
		}

		if (ilevel > 6) {
			nested_desection[6] = (bool*)malloc((n_a[6] + 1) * sizeof(bool));
			handle_error(nested_desection[6], "nested_desection[6]", "classic_aglomerative_amg_6", (n_a[6] + 1));
		}

		if (ilevel > 7) {
			nested_desection[7] = (bool*)malloc((n_a[7] + 1) * sizeof(bool));
			handle_error(nested_desection[7], "nested_desection[7]", "classic_aglomerative_amg_6", (n_a[7] + 1));
		}

		if (ilevel > 8) {
			nested_desection[8] = (bool*)malloc((n_a[8] + 1) * sizeof(bool));
			handle_error(nested_desection[8], "nested_desection[8]", "classic_aglomerative_amg_6", (n_a[8] + 1));
		}

		if (ilevel > 9) {
			nested_desection[9] = (bool*)malloc((n_a[9] + 1) * sizeof(bool));
			handle_error(nested_desection[9], "nested_desection[9]", "classic_aglomerative_amg_6", (n_a[9] + 1));
		}

		if (ilevel > 10) {
			nested_desection[10] = (bool*)malloc((n_a[10] + 1) * sizeof(bool));
			handle_error(nested_desection[10], "nested_desection[10]", "classic_aglomerative_amg_6", (n_a[10] + 1));
		}

		if (ilevel > 11) {
			nested_desection[11] = (bool*)malloc((n_a[11] + 1) * sizeof(bool));
			handle_error(nested_desection[11], "nested_desection[11]", "classic_aglomerative_amg_6", (n_a[11] + 1));
		}

		if (ilevel > 12) {
			nested_desection[12] = (bool*)malloc((n_a[12] + 1) * sizeof(bool));
			handle_error(nested_desection[12], "nested_desection[12]", "classic_aglomerative_amg_6", (n_a[12] + 1));
		}

		if (ilevel > 13) {
			nested_desection[13] = (bool*)malloc((n_a[13] + 1) * sizeof(bool));
			handle_error(nested_desection[13], "nested_desection[13]", "classic_aglomerative_amg_6", (n_a[13] + 1));
		}

		if (ilevel > 14) {
			nested_desection[14] = (bool*)malloc((n_a[14] + 1) * sizeof(bool));
			handle_error(nested_desection[14], "nested_desection[14]", "classic_aglomerative_amg_6", (n_a[14] + 1));
		}

		if (ilevel > 15) {
			nested_desection[15] = (bool*)malloc((n_a[15] + 1) * sizeof(bool));
			handle_error(nested_desection[15], "nested_desection[15]", "classic_aglomerative_amg_6", (n_a[15] + 1));
		}

		if (ilevel > 16) {
			nested_desection[16] = (bool*)malloc((n_a[16] + 1) * sizeof(bool));
			handle_error(nested_desection[16], "nested_desection[16]", "classic_aglomerative_amg_6", (n_a[16] + 1));
		}

		if (ilevel > 17) {
			nested_desection[17] = (bool*)malloc((n_a[17] + 1) * sizeof(bool));
			handle_error(nested_desection[17], "nested_desection[17]", "classic_aglomerative_amg_6", (n_a[17] + 1));
		}

		if (ilevel > 18) {
			nested_desection[18] = (bool*)malloc((n_a[18] + 1) * sizeof(bool));
			handle_error(nested_desection[18], "nested_desection[18]", "classic_aglomerative_amg_6", (n_a[18] + 1));
		}
		if (ilevel > 19) {
			nested_desection[19] = (bool*)malloc((n_a[19] + 1) * sizeof(bool));
			handle_error(nested_desection[19], "nested_desection[19]", "classic_aglomerative_amg_6", (n_a[19] + 1));
		}
		if (ilevel > 20) {
			nested_desection[20] = (bool*)malloc((n_a[20] + 1) * sizeof(bool));
			handle_error(nested_desection[20], "nested_desection[20]", "classic_aglomerative_amg_6", (n_a[20] + 1));
		}
		if (ilevel > 21) {
			nested_desection[21] = (bool*)malloc((n_a[21] + 1) * sizeof(bool));
			handle_error(nested_desection[21], "nested_desection[21]", "classic_aglomerative_amg_6", (n_a[21] + 1));
		}
		if (ilevel > 22) {
			nested_desection[22] = (bool*)malloc((n_a[22] + 1) * sizeof(bool));
			handle_error(nested_desection[22], "nested_desection[22]", "classic_aglomerative_amg_6", (n_a[22] + 1));
		}
		if (ilevel > 23) {
			nested_desection[23] = (bool*)malloc((n_a[23] + 1) * sizeof(bool));
			handle_error(nested_desection[23], "nested_desection[23]", "classic_aglomerative_amg_6", (n_a[23] + 1));
		}
		if (ilevel > 24) {
			nested_desection[24] = (bool*)malloc((n_a[24] + 1) * sizeof(bool));
			handle_error(nested_desection[24], "nested_desection[24]", "classic_aglomerative_amg_6", (n_a[24] + 1));
		}
		if (ilevel > 25) {
			nested_desection[25] = (bool*)malloc((n_a[25] + 1) * sizeof(bool));
			handle_error(nested_desection[25], "nested_desection[25]", "classic_aglomerative_amg_6", (n_a[25] + 1));
		}
		if (ilevel > 26) {
			nested_desection[26] = (bool*)malloc((n_a[26] + 1) * sizeof(bool));
			handle_error(nested_desection[26], "nested_desection[26]", "classic_aglomerative_amg_6", (n_a[26] + 1));
		}
		if (ilevel > 27) {
			nested_desection[27] = (bool*)malloc((n_a[27] + 1) * sizeof(bool));
			handle_error(nested_desection[27], "nested_desection[27]", "classic_aglomerative_amg_6", (n_a[27] + 1));
		}
		if (ilevel > 28) {
			nested_desection[28] = (bool*)malloc((n_a[28] + 1) * sizeof(bool));
			handle_error(nested_desection[28], "nested_desection[28]", "classic_aglomerative_amg_6", (n_a[28] + 1));
		}
		if (ilevel > 29) {
			nested_desection[29] = (bool*)malloc((n_a[29] + 1) * sizeof(bool));
			handle_error(nested_desection[29], "nested_desection[29]", "classic_aglomerative_amg_6", (n_a[29] + 1));
		}
		if (ilevel > 30) {
			nested_desection[30] = (bool*)malloc((n_a[30] + 1) * sizeof(bool));
			handle_error(nested_desection[30], "nested_desection[30]", "classic_aglomerative_amg_6", (n_a[30] + 1));
		}
		if (ilevel > 31) {
			nested_desection[31] = (bool*)malloc((n_a[31] + 1) * sizeof(bool));
			handle_error(nested_desection[31], "nested_desection[31]", "classic_aglomerative_amg_6", (n_a[31] + 1));
		}
		if (ilevel > 32) {
			nested_desection[32] = (bool*)malloc((n_a[32] + 1) * sizeof(bool));
			handle_error(nested_desection[32], "nested_desection[32]", "classic_aglomerative_amg_6", (n_a[32] + 1));
		}
		if (ilevel > 33) {
			nested_desection[33] = (bool*)malloc((n_a[33] + 1) * sizeof(bool));
			handle_error(nested_desection[33], "nested_desection[33]", "classic_aglomerative_amg_6", (n_a[33] + 1));
		}
		if (ilevel > 34) {
			nested_desection[34] = (bool*)malloc((n_a[34] + 1) * sizeof(bool));
			handle_error(nested_desection[34], "nested_desection[34]", "classic_aglomerative_amg_6", (n_a[34] + 1));
		}
		if (ilevel > 35) {
			nested_desection[35] = (bool*)malloc((n_a[35] + 1) * sizeof(bool));
			handle_error(nested_desection[35], "nested_desection[35]", "classic_aglomerative_amg_6", (n_a[35] + 1));
		}
		if (ilevel > 36) {
			nested_desection[36] = (bool*)malloc((n_a[36] + 1) * sizeof(bool));
			handle_error(nested_desection[36], "nested_desection[36]", "classic_aglomerative_amg_6", (n_a[36] + 1));
		}
		if (ilevel > 37) {
			nested_desection[37] = (bool*)malloc((n_a[37] + 1) * sizeof(bool));
			handle_error(nested_desection[37], "nested_desection[37]", "classic_aglomerative_amg_6", (n_a[37] + 1));
		}
		if (ilevel > 38) {
			nested_desection[38] = (bool*)malloc((n_a[38] + 1) * sizeof(bool));
			handle_error(nested_desection[38], "nested_desection[38]", "classic_aglomerative_amg_6", (n_a[38] + 1));
		}
		if (ilevel > 39) {
			nested_desection[39] = (bool*)malloc((n_a[39] + 1) * sizeof(bool));
			handle_error(nested_desection[39], "nested_desection[39]", "classic_aglomerative_amg_6", (n_a[39] + 1));
		}
		if (ilevel > 40) {
			nested_desection[40] = (bool*)malloc((n_a[40] + 1) * sizeof(bool));
			handle_error(nested_desection[40], "nested_desection[40]", "classic_aglomerative_amg_6", (n_a[40] + 1));
		}
		if (ilevel > 41) {
			nested_desection[41] = (bool*)malloc((n_a[41] + 1) * sizeof(bool));
			handle_error(nested_desection[41], "nested_desection[41]", "classic_aglomerative_amg_6", (n_a[41] + 1));
		}
		if (ilevel > 42) {
			nested_desection[42] = (bool*)malloc((n_a[42] + 1) * sizeof(bool));
			handle_error(nested_desection[42], "nested_desection[42]", "classic_aglomerative_amg_6", (n_a[42] + 1));
		}
		if (ilevel > 43) {
			nested_desection[43] = (bool*)malloc((n_a[43] + 1) * sizeof(bool));
			handle_error(nested_desection[43], "nested_desection[43]", "classic_aglomerative_amg_6", (n_a[43] + 1));
		}
		if (ilevel > 44) {
			nested_desection[44] = (bool*)malloc((n_a[44] + 1) * sizeof(bool));
			handle_error(nested_desection[44], "nested_desection[44]", "classic_aglomerative_amg_6", (n_a[44] + 1));
		}
		if (ilevel > 45) {
			nested_desection[45] = (bool*)malloc((n_a[45] + 1) * sizeof(bool));
			handle_error(nested_desection[45], "nested_desection[45]", "classic_aglomerative_amg_6", (n_a[45] + 1));
		}
		if (ilevel > 46) {
			nested_desection[46] = (bool*)malloc((n_a[46] + 1) * sizeof(bool));
			handle_error(nested_desection[46], "nested_desection[46]", "classic_aglomerative_amg_6", (n_a[46] + 1));
		}
		if (ilevel > 47) {
			nested_desection[47] = (bool*)malloc((n_a[47] + 1) * sizeof(bool));
			handle_error(nested_desection[47], "nested_desection[47]", "classic_aglomerative_amg_6", (n_a[47] + 1));
		}
		if (ilevel > 48) {
			nested_desection[48] = (bool*)malloc((n_a[48] + 1) * sizeof(bool));
			handle_error(nested_desection[48], "nested_desection[48]", "classic_aglomerative_amg_6", (n_a[48] + 1));
		}
		if (ilevel > 49) {
			nested_desection[49] = (bool*)malloc((n_a[49] + 1) * sizeof(bool));
			handle_error(nested_desection[49], "nested_desection[49]", "classic_aglomerative_amg_6", (n_a[49] + 1));
		}
		if (ilevel > 50) {
			nested_desection[50] = (bool*)malloc((n_a[50] + 1) * sizeof(bool));
			handle_error(nested_desection[50], "nested_desection[50]", "classic_aglomerative_amg_6", (n_a[50] + 1));
		}

		//idim_diag==101
		for (integer i_17 = 51; i_17 <= idim_diag - 1; i_17++) {
			if (ilevel > i_17) {
				nested_desection[i_17] = (bool*)malloc((n_a[i_17] + 1) * sizeof(bool));
				handle_error(nested_desection[i_17], "nested_desection[i_17]", "classic_aglomerative_amg_6", (n_a[i_17] + 1));
			}
		}

	}
	// nested_desection_end

	integer *row_ptr_start = NULL;
	const integer isize_row_ptr = 4 * n_a[0] + 1;
	//row_ptr_start = new integer[4 * n_a[0] + 1];
	row_ptr_start = (integer*)malloc((isize_row_ptr) * sizeof(integer));
	handle_error(row_ptr_start, " row_ptr_start", "classic_aglomerative_amg_6", (isize_row_ptr));

	integer *row_ptr_end = NULL;
	//row_ptr_end = new integer[4 * n_a[0] + 1];
	row_ptr_end = (integer*)malloc((isize_row_ptr) * sizeof(integer));
	handle_error(row_ptr_end, " row_ptr_end", "classic_aglomerative_amg_6", (isize_row_ptr));

	// ILU2
	LEVEL_ADDITIONAL_DATA* milu2 = NULL;
	// �������������.
	init_level_additional_data(milu2, ilevel);

	// ILU0
	LEVEL_ADDITIONAL_DATA0* milu0 = NULL;
	// �������������.
	init_level_additional_data(milu0, ilevel);

	// ������������ ����� ������ � ILU �������.
	if (milu_gl_buffer.alu_copy != NULL) delete[] milu_gl_buffer.alu_copy;
	if (milu_gl_buffer.jlu_copy != NULL) delete[] milu_gl_buffer.jlu_copy;
	if (milu_gl_buffer.ju_copy != NULL) delete[] milu_gl_buffer.ju_copy;
	milu_gl_buffer.alu_copy = NULL;
	milu_gl_buffer.jlu_copy = NULL;
	milu_gl_buffer.ju_copy = NULL;

	// istart - ��������� ������� ��������� ��������� � ������� �.
	// iend - �������� ������� ��������� ��������� � ������� �.
#pragma omp parallel for
	for (integer i = 1; i <= n; i++) {
		flag[i] = false;
	}
	for (integer ii = 1; ii <= nnz_a[0]; ii++) {
		if (flag[Amat[ii].i] == false) {
			integer istr = Amat[ii].i;
			integer ic = ii;
			integer icdiag = ii;
			if (istr >= isize_row_ptr) {
#if doubleintprecision == 1
				printf("nado uvelichit isize_row_ptr %lld", istr);
#else
				printf("nado uvelichit isize_row_ptr %d", istr);
#endif

				//getchar();
				system("PAUSE");
				exit(1);
			}
			row_ptr_start[istr] = ii;
			doublerealT ap = 0.0; // �������� �� ���������.
								  //x[istr] = b[istr];
			while ((ic <= nnz_a[0]) && (Amat[ic].i == istr)) {
				if (Amat[ic].j != istr) {
					//x[istr] += -Amat[ic].aij*x[Amat[ic].j];
					// ��� ��������������� �������� ������ ���� ������ ������������.
					// ���� ��� �� ��� �� ���� �������� �������������� � ���������� ������ ������������.
					if (Amat[ic].aij >= 0.0) {
#if doubleintprecision == 1
						//printf("polochitelnji vnediagonalnj element %e in matrix level 0 in string %lld...\n", Amat[ic].aij, istr);
#else
						//printf("polochitelnji vnediagonalnj element %e in matrix level 0 in string %d...\n", Amat[ic].aij, istr);
#endif

						// ����� ��� �� ������� 26 ������� 2016.
						// �� �� �� ������ � ���������� ����������� ���� � �� ������� ������ �����������.
						//system("PAUSE");
					}
				}
				else {
					// ������������ ������� ������ �����������.
					ap = Amat[ic].aij;
					icdiag = ic;
				}
				ic++;
			}
			if (istr >= isize_row_ptr) {
#if doubleintprecision == 1
				printf("nado uvelichit isize_row_ptr %lld", istr);
#else
				printf("nado uvelichit isize_row_ptr %d", istr);
#endif

				//getchar();
				system("PAUSE");
				exit(1);
			}
			row_ptr_end[istr] = ic - 1;
			if (fabs(ap) < RealZERO) {
#if doubleintprecision == 1
				printf("zero diagonal elements in string %lld in basic matrix", istr);
#else
				printf("zero diagonal elements in string %d in basic matrix", istr);
#endif

				system("PAUSE");
				exit(1);
			}
			else {
				//x[istr] /= ap;
			}

			flag[Amat[ii].i] = true;
			Ak1 temp = Amat[ii];
			Amat[ii] = Amat[icdiag];
			Amat[icdiag] = temp;
			if (bmemory_savings) {
				// �� ��������� ������ �������� �������,
				// �� ������ ��� ������� ����������.
				the_original_order_of_values[the_original_order_of_values_reverse[ii]] = icdiag;
				the_original_order_of_values[the_original_order_of_values_reverse[icdiag]] = ii;
			}

			diag[0][Amat[ii].i] = ap; // ��� ��������� ���������� �������.
			Amat[ii].aij = 1.0 / ap; // ��������� ������� �������.
		}
	}

	if (bILU2smoother == 2) {
		// ILU2
		printf("apply ilu0 smoother for number 0 level\n");
		equation3DtoCRSRUMBA1(milu2[0], true, Amat, 1, n_a[0], row_ptr_start, row_ptr_end, 0, 0);
	}
	else if (bILU2smoother == 1) {
		// ILU0
		printf("apply ilu0 smoother for number 0 level\n");
		equation3DtoCRSRUMBA0(milu0[0], true, Amat, 1, n_a[0], row_ptr_start, row_ptr_end, 0, 0);
	}
	else if (my_amg_manager.iFinnest_ilu == 1) {
		// ILU0 �� ������ �� ����� ��������� �����.
		printf("apply ilu0 smoother for number 0 level\n");
		equation3DtoCRSRUMBA1(milu2[0], true, Amat, 1, n_a[0], row_ptr_start, row_ptr_end, 0, 0);
	}
	bool bstop = false;


	// 14 �������� 2015 ����������� �������� ������� �����������.
	// ������ ����������� � ������� �� ������� �����. 12.07.2016.

	// ��������� 29.10.2016.
	if (bprint_mesage_diagnostic) {
		printf("1. positive connections %%, 2. max positive/ diagonal %%\n");
	}

	for (integer ilevel_detector = 1; ilevel_detector <= idim_diag - 1; ilevel_detector++) {

		// ��������� ������� ��������� �� 99 ������ ������������, ��
		// ���� ���������� ���������� ������� ��� ���� �������.
		const integer istop_level_scan = idim_diag - 2;

		if (ilevel > ilevel_detector) {

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
				if (flag[Amat[ii].i] == false) {

					integer istr = Amat[ii].i;
					integer ic = ii;
					integer icdiag = ii;
					integer istart_row_ptr = istr;
					for (integer ilev = 0; ilev < ilevel_detector; ilev++) {
						istart_row_ptr += n_a[ilev];
					}



					max_positive_connections_element = -1.0;
					dn_num += 1.0;

					
					max_positive_connections_element = -1.0;


					if (istart_row_ptr >= isize_row_ptr) {
#if doubleintprecision == 1
						printf("nado uvelichit isize_row_ptr %lld", istart_row_ptr);
#else
						printf("nado uvelichit isize_row_ptr %d", istart_row_ptr);
#endif

						//getchar();
						system("PAUSE");
						exit(1);
					}
					if (ilevel_detector <= istop_level_scan) {
						row_ptr_start[istart_row_ptr] = ii;
					}
					doublerealT ap = 0.0;
					doublerealT sum_4 = 0.0;




					const doublerealT theta7 = theta; // ��������� � ������� �����.
					b_ne_menee_2_positive_con_in_string = false;
					integer inum_pos_con_in_string = 0;
					doublerealT threshold7 = -1.0;
					integer ic7 = ic;
					while ((ic7 <= iend) && (Amat[ic7].i == istr)) {
						if (Amat[ic7].j != istr) {
							if (Amat[ic7].aij >= 0.0) {
								inum_pos_con_in_string++;
								if (fabs(Amat[ic7].aij) > threshold7) threshold7 = fabs(Amat[ic7].aij);
							}
						}
						ic7++;
					}
					// �� ���������� �� ����� ���� ������������� ������ � ������ ������.
					if (inum_pos_con_in_string >= 2) {
						inum_pos_con_in_string = 0;
						ic7 = ic;
						while ((ic7 <= iend) && (Amat[ic7].i == istr)) {
							if (Amat[ic7].j != istr) {
								if ((Amat[ic7].aij >= 0.0) && (fabs(Amat[ic7].aij) >= theta7*threshold7)) {
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
					while ((ic <= iend) && (Amat[ic].i == istr)) {
						if (Amat[ic].j != istr) {
							//x[istr] += -Amat[ic].aij*x[Amat[ic].j];
							inum_vnediagonal_all += 1.0;
							// ��� ��������������� �������� ������ ���� ������ ������������.
							// ���� ��� �� ��� �� ���� �������� �������������� � ���������� ������ ������������.
							if (Amat[ic].aij >= 0.0) {
#if doubleintprecision == 1
								//printf("polochitelnji vnediagonalnj element %e in matrix level %lld in string %lld...\n", Amat[ic].aij, ilevel_detector, istr);
#else
								//printf("polochitelnji vnediagonalnj element %e in matrix level %d in string %d...\n", Amat[ic].aij, ilevel_detector, istr);
#endif
								//system("PAUSE");
								inum_only_positive_vnediagonal += 1.0;

								if (b_ne_menee_2_positive_con_in_string) {
									if (fabs(Amat[ic7].aij) >= theta7*threshold7) {
										inum_only_positive_vnediagonal_ne_menee2_in_string += 1.0;
									}
								}

								// ����������� �������� ������������ ��������������� �����.
								if (max_positive_connections_element < Amat[ic].aij) {
									max_positive_connections_element = Amat[ic].aij;
								}
							}
						}
						else {
							ap = Amat[ic].aij;
							memo_diagonal_element = ap;
							icdiag = ic;							
						}
						ic++;
					}


					if (istart_row_ptr >= isize_row_ptr) {
#if doubleintprecision == 1
						printf("nado uvelichit isize_row_ptr %lld", istart_row_ptr);
#else
						printf("nado uvelichit isize_row_ptr %d", istart_row_ptr);
#endif

						//getchar();
						system("PAUSE");
						exit(1);
					}
					if (ilevel_detector <= istop_level_scan) {
						row_ptr_end[istart_row_ptr] = ic - 1;
					}
					if (fabs(ap) < RealZERO) {
#if doubleintprecision == 1
						printf("zero diagonal elements in string %lld in level %lld matrix", istr, ilevel);
#else
						printf("zero diagonal elements in string %d in level %d matrix", istr, ilevel);
#endif

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
					flag[Amat[ii].i] = true;
					if (ilevel_detector <= istop_level_scan) {
						Ak1 temp = Amat[ii];
						Amat[ii] = Amat[icdiag];
						Amat[icdiag] = temp;
						switch (ilevel_detector) {
						case 1: diag[1][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 2: diag[2][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 3: diag[3][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 4: diag[4][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 5: diag[5][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 6: diag[6][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 7: diag[7][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 8: diag[8][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 9: diag[9][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 10: diag[10][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 11: diag[11][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 12: diag[12][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 13: diag[13][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 14: diag[14][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 15: diag[15][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 16: diag[16][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 17: diag[17][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 18: diag[18][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 19: diag[19][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 20: diag[20][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 21: diag[21][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 22: diag[22][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 23: diag[23][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 24: diag[24][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 25: diag[25][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 26: diag[26][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 27: diag[27][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 28: diag[28][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 29: diag[29][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 30: diag[30][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 31: diag[31][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 32: diag[32][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 33: diag[33][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 34: diag[34][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 35: diag[35][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 36: diag[36][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 37: diag[37][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 38: diag[38][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 39: diag[39][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 40: diag[40][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 41: diag[41][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 42: diag[42][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 43: diag[43][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 44: diag[44][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 45: diag[45][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 46: diag[46][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 47: diag[47][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 48: diag[48][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 49: diag[49][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 50: diag[50][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 51: diag[51][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 52: diag[52][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 53: diag[53][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 54: diag[54][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 55: diag[55][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 56: diag[56][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 57: diag[57][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 58: diag[58][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 59: diag[59][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 60: diag[60][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 61: diag[61][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 62: diag[62][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 63: diag[63][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 64: diag[64][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 65: diag[65][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 66: diag[66][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 67: diag[67][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 68: diag[68][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 69: diag[69][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 70: diag[70][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 71: diag[71][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 72: diag[72][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 73: diag[73][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 74: diag[74][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 75: diag[75][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 76: diag[76][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 77: diag[77][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 78: diag[78][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 79: diag[79][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 80: diag[80][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 81: diag[81][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 82: diag[82][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 83: diag[83][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 84: diag[84][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 85: diag[85][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 86: diag[86][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 87: diag[87][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 88: diag[88][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 89: diag[89][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 90: diag[90][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 91: diag[91][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 92: diag[92][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 93: diag[93][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 94: diag[94][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 95: diag[95][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 96: diag[96][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 97: diag[97][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 98: diag[98][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 99: diag[99][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						case 100: diag[100][Amat[ii].i] = ap; break;// ��� ��������� ���������� �������.
						default: printf("ilevel_detector incorrect\n");
							//getchar();
							system("PAUSE");
							break;
						}

						Amat[ii].aij = 1.0 / ap; // ��������� ������� �������.
					}


				}
			}

			integer iadd_now = 0;
			for (integer i54 = 1; i54 <= ilevel_detector; i54++) {
				iadd_now += n_a[i54 - 1];
			}
			if (ilevel_detector <= istop_level_scan) {
				if (bILU2smoother == 2) {
#if doubleintprecision == 1
					printf("apply ilu0 smoother for number %lld level\n", ilevel_detector);
#else
					printf("apply ilu0 smoother for number %d level\n", ilevel_detector);
#endif

					equation3DtoCRSRUMBA1(milu2[ilevel_detector], true,
						Amat, 1, n_a[ilevel_detector], row_ptr_start, row_ptr_end, iadd_now, ilevel_detector);
				}
			}
			if (ilevel_detector <= istop_level_scan) {
				if (bILU2smoother == 1) {
#if doubleintprecision == 1
					// ILU0
					printf("apply ilu0 smoother for number %lld level\n", ilevel_detector);
#else
					// ILU0
					printf("apply ilu0 smoother for number %d level\n", ilevel_detector);
#endif

					// iadd_now=n_a[0]+...+n_a[ilevel_detector-1];
					equation3DtoCRSRUMBA0(milu0[ilevel_detector], true,
						Amat, 1, n_a[ilevel_detector], row_ptr_start, row_ptr_end, iadd_now, ilevel_detector);
				}
			}
			if (ilevel_detector <= istop_level_scan) {
				if (my_amg_manager.iFinnest_ilu == 1) {
					if (my_amg_manager.b_ilu_smoothers_in_nnz_n_LE_6) {
						doublerealT dn = 1.0*n_a[ilevel_detector];
						doublerealT dnnz = 1.0*nnz_a[ilevel_detector];
						if (dnnz / dn <= dapply_ilu_max_pattern_size) {
							// ��������� (����������) ������.
#if doubleintprecision == 1
							printf("apply ilu0 smoother for number %lld level\n", ilevel_detector);
#else
							printf("apply ilu0 smoother for number %d level\n", ilevel_detector);
#endif

							equation3DtoCRSRUMBA1(milu2[ilevel_detector], true,
								Amat, 1, n_a[ilevel_detector], row_ptr_start, row_ptr_end, iadd_now, ilevel_detector);
						}
					}
				}
			}


			// statistic log :
			if (bprint_mesage_diagnostic) {
				//printf("procent positive connections %e \n", 100.0*inum_only_positive_vnediagonal / inum_vnediagonal_all);
				//printf("the ratio of the maximum positive connections to the diagonal\n");
				//printf("element in the row, in procent %e\n", 100.0*ratio_positive_connections_by_diagonalelement);
				//printf("\n");
#if doubleintprecision == 1
				printf("%lld %2.1f %% [ %2.2f %% ] %3.1f  [%2.2f ]\n", ilevel_detector, 1.00*inum_only_positive_vnediagonal / inum_vnediagonal_all, 100 * inum_only_positive_vnediagonal_ne_menee2_in_string / inum_vnediagonal_all, 1e-4*ratio_positive_connections_by_diagonalelement, 1e-4*ratio_positive_connections_by_diagonalelement_avg / dn_num);

#else
				printf("%d %2.1f %% [ %2.2f %% ] %3.1f  [%2.2f ]\n", ilevel_detector, 1.00*inum_only_positive_vnediagonal / inum_vnediagonal_all, 100 * inum_only_positive_vnediagonal_ne_menee2_in_string / inum_vnediagonal_all, 1e-4*ratio_positive_connections_by_diagonalelement, 1e-4*ratio_positive_connections_by_diagonalelement_avg / dn_num);

#endif
			}
		}


	}





	if (bstop) exit(1);
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
	else if (my_amg_manager.iFinnest_ilu == 1) {
		// ILU0 �� ������ �� ����� ��������� �����.
		memory_allocation_apostoriory_buffer_ilu(milu2, ilevel - 1); // 7.06.2017.
	}


	// ����������������� ��� ��������� � �����������. 
	// 4 ������ 2016 ��������� 15 ������� �����������.
	// ������� ������� �����������.
	/*
	if (ilevel > 8) {
	for (integer i = 1; i <= n; i++) {
	flag[i] = false;
	}
	integer ist = nnz_a[0] + nnz_a[1] + nnz_a[2] + nnz_a[3] + nnz_a[4] + nnz_a[5] + nnz_a[6] + nnz_a[7] + 1;
	integer iend = nnz_a[0] + nnz_a[1] + nnz_a[2] + nnz_a[3] + nnz_a[4] + nnz_a[5] + nnz_a[6] + nnz_a[7] + nnz_a[8];
	for (integer ii = ist; ii <= iend; ii++) {
	if (flag[Amat[ii].i] == false) {
	integer istr = Amat[ii].i;
	integer ic = ii;
	integer icdiag = ii;
	row_ptr_start[istr + n_a[0] + n_a[1] + n_a[2] + n_a[3] + n_a[4] + n_a[5] + n_a[6] + n_a[7]] = ii;
	doublerealT ap = 0.0;
	//x[istr] = b[istr];
	while ((ic <= iend) && (Amat[ic].i == istr)) {
	if (Amat[ic].j != istr) {
	//x[istr] += -Amat[ic].aij*x[Amat[ic].j];
	}
	else {
	ap = Amat[ic].aij;
	icdiag = ic;
	}
	ic++;
	}
	row_ptr_end[istr + n_a[0] + n_a[1] + n_a[2] + n_a[3] + n_a[4] + n_a[5] + n_a[6] + n_a[7]] = ic - 1;
	if (fabs(ap) < RealZERO) {
	#if doubleintprecision == 1
	printf("zero diagonal elements in string %lld in level 7 matrix", istr);
	#else
	printf("zero diagonal elements in string %d in level 7 matrix", istr);
	#endif

	system("PAUSE");
	exit(1);
	}
	else {
	//x[istr] /= ap;
	}

	flag[Amat[ii].i] = true;
	Ak1 temp = Amat[ii];
	Amat[ii] = Amat[icdiag];
	Amat[icdiag] = temp;
	diag[8][Amat[ii].i] = ap; // ��� ��������� ���������� �������.
	Amat[ii].aij = 1.0 / ap; // ��������� ������� �������.
	}
	}
	}
	*/


	if (!bonly_serial) {
		// ������� nested desection
		// ��� ���� �������.
		// ����� ��������� ������� 0.
		// nested_desection[0]
		nested_desection_patch(Amat, n_a[0], nested_desection[0], row_ptr_start, row_ptr_end, 0);
		if (bprint_mesage_diagnostic) {
			printf("part1\n");
		}


		// ������� nested desection
		// ��� ���� �������.
		// ����� ��������� ������� 1.
		// nested_desection[1]
		if (ilevel > 1) {
			nested_desection_patch(Amat, n_a[1], nested_desection[1], row_ptr_start, row_ptr_end, n_a[0]);
			if (bprint_mesage_diagnostic) {
				printf("part2\n");
			}
		}
		if (ilevel > 2) {
			nested_desection_patch(Amat, n_a[2], nested_desection[2], row_ptr_start, row_ptr_end, n_a[0] + n_a[1]);
			if (bprint_mesage_diagnostic) {
				printf("part3\n");
			}
		}
		if (ilevel > 3) {
			nested_desection_patch(Amat, n_a[3], nested_desection[3], row_ptr_start, row_ptr_end, n_a[0] + n_a[1] + n_a[2]);
			if (bprint_mesage_diagnostic) {
				printf("part4\n");
			}
		}
		if (ilevel > 4) {
			nested_desection_patch(Amat, n_a[4], nested_desection[4], row_ptr_start, row_ptr_end, n_a[0] + n_a[1] + n_a[2] + n_a[3]);
			if (bprint_mesage_diagnostic) {
				printf("part5\n");
			}
		}
		if (ilevel > 5) {
			nested_desection_patch(Amat, n_a[5], nested_desection[5], row_ptr_start, row_ptr_end, n_a[0] + n_a[1] + n_a[2] + n_a[3] + n_a[4]);
			if (bprint_mesage_diagnostic) {
				printf("part6\n");
			}
		}
		if (ilevel > 6) {
			nested_desection_patch(Amat, n_a[6], nested_desection[6], row_ptr_start, row_ptr_end, n_a[0] + n_a[1] + n_a[2] + n_a[3] + n_a[4] + n_a[5]);
			if (bprint_mesage_diagnostic) {
				printf("part7\n");
			}
		}
		if (ilevel > 7) {
			nested_desection_patch(Amat, n_a[7], nested_desection[7], row_ptr_start, row_ptr_end, n_a[0] + n_a[1] + n_a[2] + n_a[3] + n_a[4] + n_a[5] + n_a[6]);
			if (bprint_mesage_diagnostic) {
				printf("part8 \n");
				printf("nested desection is finish\n");
			}
		}
		if (ilevel > 8) {
			nested_desection_patch(Amat, n_a[8], nested_desection[8], row_ptr_start, row_ptr_end, n_a[0] + n_a[1] + n_a[2] + n_a[3] + n_a[4] + n_a[5] + n_a[6] + n_a[7]);
			if (bprint_mesage_diagnostic) {
				printf("part9 \n");
				printf("nested desection is finish\n");
			}
		}
		if (ilevel > 9) {
			nested_desection_patch(Amat, n_a[9], nested_desection[9], row_ptr_start, row_ptr_end, n_a[0] + n_a[1] + n_a[2] + n_a[3] + n_a[4] + n_a[5] + n_a[6] + n_a[7] + n_a[8]);
			if (bprint_mesage_diagnostic) {
				printf("part10 \n");
				printf("nested desection is finish\n");
			}
		}

		// idim_diag==101
		for (integer i_17 = 10; i_17 <= idim_diag - 1; i_17++) {
			if (ilevel > i_17) {
				integer inasum = 0;
				for (integer i_18 = 0; i_18 < i_17; i_18++) inasum += n_a[i_18];
				nested_desection_patch(Amat, n_a[i_17], nested_desection[i_17], row_ptr_start, row_ptr_end, inasum);
				if (bprint_mesage_diagnostic) {
#if doubleintprecision == 1
					printf("part%lld \n", i_17);
#else
					printf("part%d \n", i_17);
#endif

					printf("nested desection is finish\n");
				}
			}
		}


	}


	// smoother.
	// 9 september 2015.
	// q - quick.
	// seidelq(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0);
	//void seidelq(Ak1* &Amat, integer istartq, integer iendq, doublerealT* &x, doublerealT* &b, integer * &row_ptr_start, integer * &row_ptr_end, integer iadd)
	//{
	// istart - ��������� ������� ��������� ��������� � ������� �.
	// iend - �������� ������� ��������� ��������� � ������� �.
	//integer startpos = istartq + iadd;
	//integer endpos = iendq+iadd;
	//for (integer ii = startpos; ii <= endpos; ii++) {
	//integer istr = ii - iadd;
	//x[istr] = b[istr];
	//for (integer ii1 = row_ptr_start[ii] + 1; ii1 <= row_ptr_end[ii]; ii1++)
	//{
	//x[istr] += -Amat[ii1].aij*x[Amat[ii1].j];
	//}
	//x[istr] *= Amat[row_ptr_start[ii]].aij;
	//}


	//} // seidelq

	// ������������ ������ ������������ �� ����� ���������� �������� ������.

	//delete[] this_is_C_node;
	//delete[] this_is_F_node;
	if (this_is_C_node != NULL) {
		free(this_is_C_node);
		this_is_C_node = NULL;
	}
	if (this_is_F_node != NULL) {
		free(this_is_F_node);
		this_is_F_node = NULL;
	}
	//delete[] hash_table;
	if (hash_table != NULL) {
		free(hash_table);
		hash_table = NULL;
	}
	if (hash_table2 != NULL) {
		free(hash_table2);
		hash_table2 = NULL;
	}
	//delete[] istack;
	//delete[] istack2;
	if (istack != NULL) {
		free(istack);
		istack = NULL;
	}
	if (istack2 != NULL) {
		free(istack2);
		istack2 = NULL;
	}


	if (bprint_mesage_diagnostic) {
		printf("cycling: V cycle.\n");
#if doubleintprecision == 1
		printf("level=%lld\n", ilevel);
#else
		printf("level=%d\n", ilevel);
#endif

		printf("multigrid R.P.Fedorenko 1961.\n");
		printf("standart aglomerative algebraic multigrid method.\n");
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
	// �.�. ���� ���������������� ������, �� ������������ ������ �����.
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



	// ������� ��������� ���������� �������� �������� ���������� :
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

	if (bprint_mesage_diagnostic) {
		printf("grid complexity is %1.2f\n", dr_grid_complexity);
		printf("Prolongation operator complexity is %1.2f  %1.2f\n", (doublerealT)(nnz_P_memo_all / nnz_P_memo_0), (doublerealT)(nnz_P_memo_all / n_a[0]));
#if doubleintprecision == 1
		printf("nu1=%lld, nu2=%lld\n", nu1, nu2);
#else
		printf("nu1=%d, nu2=%d\n", nu1, nu2);
#endif

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

	doublerealT **residual_fine = NULL;
	residual_fine = new doublerealT*[idim_diag];
	if (residual_fine == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for residual_fine my_gregat_amg4.cpp...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}
	doublerealT **residual_coarse = NULL;
	residual_coarse = new doublerealT*[idim_diag];
	if (residual_coarse == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for residual_coarse my_gregat_amg4.cpp...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}
	doublerealT **error_approx_coarse = NULL;
	error_approx_coarse = new doublerealT*[idim_diag];
	if (error_approx_coarse == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for error_approx_coarse my_gregat_amg4.cpp...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}
	doublerealT **error_approx_fine = NULL;
	error_approx_fine = new doublerealT*[idim_diag];
	if (error_approx_fine == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for error_approx_fine my_gregat_amg4.cpp...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}
	for (integer i_id_level_local = 0; i_id_level_local < idim_diag; i_id_level_local++) {
		residual_fine[i_id_level_local] = NULL;
		residual_coarse[i_id_level_local] = NULL;
		error_approx_coarse[i_id_level_local] = NULL;
		error_approx_fine[i_id_level_local] = NULL;
	}


	// ���������� ��� ������������� ��������� NULL 4 ������� 2016. 

	// ����������������� ��� ��������� �������. � ������ ������ 
	//5.06.2017 �������������� 100 ������� �����������.

	// 25.04.2018 �� ���� ����� ����� ������� �������� ����������� ����.

	// ����� �������� ����������� ������ ���������� ������� �.�.
	// ����������� ������ ��������������� ���������� dll �
	// �������� ���������� ����� ����� �� �������.




	// idim_diag==101
	for (integer i_17 = 1; i_17 <= idim_diag - 1; i_17++) {
		// 05.06.2017
		integer i_17_prev = i_17 - 1;

		if (ilevel + 1 > i_17) {

			// residual
			//residual_fine[i_17_prev] = new doublerealT[n_a[i_17_prev] + 1];
			residual_fine[i_17_prev] = (doublerealT*)malloc((n_a[i_17_prev] + 1) * sizeof(doublerealT));
			handle_error(residual_fine[i_17_prev], "residual_fine[", i_17_prev, "]", "classic_aglomerative_amg_6", (n_a[i_17_prev] + 1));


			//residual_coarse[i_17_prev] = new doublerealT[n_a[i_17] + 1];
			residual_coarse[i_17_prev] = (doublerealT*)malloc((n_a[i_17] + 1) * sizeof(doublerealT));
			handle_error(residual_coarse[i_17_prev], "residual_coarse[", i_17_prev, "]", "classic_aglomerative_amg_6", (n_a[i_17] + 1));

			//error_approx_coarse[i_17_prev] = new doublerealT[n_a[i_17] + 1];
			error_approx_coarse[i_17_prev] = (doublerealT*)malloc((n_a[i_17] + 1) * sizeof(doublerealT));
			handle_error(error_approx_coarse[i_17_prev], "error_approx_coarse[", i_17_prev, "]", "classic_aglomerative_amg_6", (n_a[i_17] + 1));

			//error_approx_fine[i_17_prev] = new doublerealT[n_a[i_17_prev] + 1];
			error_approx_fine[i_17_prev] = (doublerealT*)malloc((n_a[i_17_prev] + 1) * sizeof(doublerealT));
			handle_error(error_approx_fine[i_17_prev], "error_approx_fine[", i_17_prev, "]", "classic_aglomerative_amg_6", (n_a[i_17_prev] + 1));
		}
	}




	// 2 jan 2016. 
	integer igam = 1; // 1-V; 2-W
	const integer ZERO_INIT = 0;
	const integer RANDOM_INIT = 1;// ���� ����������� nu1, nu2 � 1,2 �� 5 �������.
	integer imyinit = ZERO_INIT; // ZERO_INIT optimum

	doublerealT* x_copy = NULL;
	x_copy = (doublerealT*)malloc((n_a[0] + 1) * sizeof(doublerealT));
	handle_error(x_copy, "x_copy", "classic_aglomerative_amg_6", (n_a[0] + 1));

	// ��� ��������� ����� � ��������� ����������.
	doublerealT* x_old = NULL;
	x_old = (doublerealT*)malloc((n_a[0] + 1) * sizeof(doublerealT));
	handle_error(x_old, "x_old", "classic_aglomerative_amg_6", (n_a[0] + 1));

#pragma omp parallel for
	for (integer i47 = 1; i47 <= n_a[0]; i47++) {
		x_copy[i47] = x[i47];
		x_old[i47] = x[i47];
		//x_copy[i47] = 0.0; // 28.07.2016
	}

	doublereal* x_best_search = NULL;
	x_best_search = (doublereal*)malloc((n_a[0] + 1) * sizeof(doublereal));
	handle_error(x_best_search, "x_best_search", "classic_aglomerative_amg_6", (n_a[0] + 1));

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

	residualq2(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0], diag[0]);
	doublerealT dres_initial = norma(residual_fine[0], n_a[0]);
	if (((iVar == VX) || (iVar == VY) || (iVar == VZ)) && (dres_initial > 20.0)) {
		// ��� ������� ������ � ������ ������� ���� �� ���������� ��������.
		printf("my be problem convergence : very big dres0=%e\n", dres_initial);
		printf("run residualq2 analysys.\n");
		residualq2_analysys(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0], diag[0]);
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
#if doubleintprecision == 1
		printf("%d %e rho=%e\n", 0, dres_initial, dres_initial / rho);
#else
		printf("%d %e rho=%e\n", 0, dres_initial, dres_initial / rho);
#endif

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
	// �.�. ���� ���������������� ������, �� ������������ ������ �����.
	// �������� ������ ������� ���� ���������� �������� ���������� �������������.
	//if (ilevel > my_amg_manager.maximum_levels) {
	//ilevel = my_amg_manager.maximum_levels;
	//}
	ilevel -= my_amg_manager.maximum_delete_levels;



	doublereal* x_best_search2 = NULL;
	x_best_search2 = new doublereal[n_a[0] + 1];
	if (x_best_search2 == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for x_best_search2 my_agregat_amg.cpp...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}
	doublereal* x_best_search_init = NULL;
	x_best_search_init = new doublereal[n_a[0] + 1];
	if (x_best_search_init == NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment for x_best_search_init my_agregat_amg.cpp...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}
#pragma omp parallel for
	for (integer i47 = 1; i47 <= n_a[0]; i47++) {
		x_best_search_init[i47] = x[i47];
		x_best_search2[i47] = x[i47];
	}


	integer istop_speed_cycling = 10;

	if ((my_amg_manager.istabilization == 0) || ((iVar == TEMP) && (my_amg_manager.istabilization == 3))) {


		// ((iVar==TEMP)&&(my_amg_manager.istabilization == 3)) - ���������� ��������� ������� � ��������� �������������.

		// ������ �������������� ������������� �����.

		if ((iVar == VX) || (iVar == VY) || (iVar == VZ)) tolerance *= 1e-11;
		if (iVar == PAM) tolerance *= 1e-14;
		if (iVar == TEMP) tolerance *= 1e-6;
		if (iVar == TOTALDEFORMATIONVAR) tolerance = 1.0e-17;
		doublereal minx_gl = 1.0e40;
		doublereal maxx_gl = -1.0e40;
		//for (integer iprohod = 0; iprohod < 20; iprohod++) {
		//while ((iflag_cont == 1) && ((dres>tolerance) || ((iVar != TEMP) && (icount_V_cycle<5)))) {
		///	while ((iflag_cont == 1) && ((dres>tolerance) )) {
		while (((iflag_cont == 1) && ((dres>tolerance))) || ((iVar == TEMP) && bSIMPLErun_now_for_temperature && (icount_V_cycle<9)) || ((iVar == TOTALDEFORMATIONVAR) && (icount_V_cycle<9))) {

			// ������������ ������������ �������������� ��� ������ ����������.

			if (bSIMPLErun_now_for_temperature) {
				// �������������.

				//  ���� ��� ���������, ���� �����������.
				if (icount_V_cycle > istop_speed_cycling) {

					// 4 ������ 2016 ����.
					switch (iVar) {
					case VX: vx_res = 1.2*dres; break;
					case VY: vy_res = 1.2*dres; break;
					case VZ: vz_res = 1.2*dres; break;
					}

					if ((iVar == VX) || (iVar == VY) || (iVar == VZ)) {
						if (dres < 1.0e-3*dres_initial) {
							break;
						}
						else {
							istop_speed_cycling += 2;
						}
					}
					if (iVar == PAM) {
						if (dres < 1.0e-1*dres_initial) {
							//break;
						}
						else {
							istop_speed_cycling += 2;
						}
					}
				}
			}


			/*
			if (bSIMPLErun_now_for_temperature) {
			if (icount_V_cycle > istop_speed_cycling) {
			// �����: ����� ������������ ��� ��������� ��������.
			// � ������ ������������ �� ���������� ������������.
			if (bfirst_now_speed) {
			switch (iVar) {
			case VX: vx_res = 1.2*dres; break;
			case VY: vy_res = 1.2*dres; break;
			case VZ: vz_res = 1.2*dres;
			if (iglnumberSimpleit > 2) {
			bfirst_now_speed = false;
			}
			iglnumberSimpleit++;
			break;
			}
			break;
			}
			else {
			bool bstop83 = false;
			switch (iVar) {
			case VX: if (dres < 1.0e6*vx_res) {
			//vx_res = 2.2*dres;
			bstop83 = true;
			}
			else {
			istop_speed_cycling += 2;
			printf("VX_");
			}
			break;
			case VY: if (dres < 1.0e6*vy_res) {
			//vy_res = 2.2*dres;
			bstop83 = true;
			}
			else {
			istop_speed_cycling += 2;
			printf("VY_");
			}
			break;
			case VZ:
			if (dres < 1.0e6*vz_res) {
			//vz_res = 2.2*dres;
			bstop83 = true;
			}
			else {
			istop_speed_cycling += 2;
			printf("VZ_");
			}
			break;
			}
			if (bstop83) {
			iflag_cont = 0;
			break;

			}
			}
			}

			}
			*/

			if (fabs(dres / rho)<1.0) {
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
							printf("break bPhysics_stop, dres<1e-2 && (fabs(maxnew - maxold) < 1.0e-9)\n");
							break;
						}
						else {
							maxold = maxnew;
						}
					}
					else {
						if ((fabs(dres) < 1.0e-2) && (fabs(maxnew - maxold) < 0.0005)) {
							printf("break bPhysics_stop, dres<1e-2 && (fabs(maxnew - maxold) < 0.0005)\n");
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
				if (((iVar == TEMP) && (my_amg_manager.istabilization == 3)) && (iVar == TEMP)) {
					if (bonly_solid_calculation == true) {
						if (bvacuumPrism) {
							// �������������� ��������� ������� ���������� ������� � x
							// � rthdsd.

							doublereal* x_temper = NULL;
							//x_temper = new doublerealT[n_a[0] + 1];
							x_temper = (doublereal*)malloc(((integer)(n_a[0]) + 1) * sizeof(doublereal));
							if (x_temper == NULL) {
								// ������������ ������ �� ������ ������������.
								printf("Problem : not enough memory on your equipment for x_temper my_agregat_amg.cpp...\n");
								printf("Please any key to exit...\n");
								exit(1);
							}
#pragma omp parallel for
							for (integer i23 = 0; i23 < n_a[0]; i23++) {
								if (x[i23 + 1] < -272.15) x[i23 + 1] = -272.15;
								//x_temper[i23] = x[i23 + 1];
								// 0.01 �������� ������ ����������.
								// 0.25
								x_temper[i23] = x_old[i23 + 1] + 0.2*(x[i23 + 1] - x_old[i23 + 1]);
								if (x_temper[i23] < -272.15) x_temper[i23] = -272.15;
								x[i23 + 1] = x_temper[i23];
							}

							// �� ������ �� ��������� ������� ��������� ��� ������� �������� �������.
							// ���������� ���������� ���������� � � �� �������� ��������� ����������� :
							for (integer i23 = 0; i23 < lb; i23++) {
								update_avg_temperatures(x_temper, my_body[i23]);
							}
							// ���������� ���������� ������������ �������� ������� :
							for (integer i23 = 0; i23 < lb; i23++) {
								calculation_density_radiation_heat_flux(my_body[i23]);
							}


							doublereal* rthdsd_loc123 = NULL;
							//rthdsd_loc123 = new doublerealT[n_a[0] + 1];
							rthdsd_loc123 = (doublereal*)malloc(((integer)(n_a[0]) + 1) * sizeof(doublereal));
							if (rthdsd_loc123 == NULL) {
								// ������������ ������ �� ������ ������������.
								printf("Problem : not enough memory on your equipment for rthdsd_loc123 my_agregat_amg.cpp...\n");
								printf("Please any key to exit...\n");
								exit(1);
							}

							for (integer i23 = 0; i23 < n_a[0]; i23++) {
								rthdsd_loc123[i23] = rthdsd_no_radiosity_patch[i23];
								if ((i23 >= iadd_qnbc_maxelm) && (qnbc[i23 - iadd_qnbc_maxelm].bactive) && (qnbc[i23 - iadd_qnbc_maxelm].bStefanBolcman_q_on)) {
									doublerealT alpha_relax142 = 0.25;
									rthdsd_loc123[i23] = alpha_relax142 *(-qnbc[i23 - iadd_qnbc_maxelm].emissivity*5.670367e-8*((273.15 + x_temper[i23])*(273.15 + x_temper[i23])*(273.15 + x_temper[i23])*(273.15 + x_temper[i23]) - (273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)*(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)*(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)*(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb))) +
										(1.0 - alpha_relax142)*(-qnbc[i23 - iadd_qnbc_maxelm].emissivity*5.670367e-8*((273.15 + x_old[i23 + 1])*(273.15 + x_old[i23 + 1])*(273.15 + x_old[i23 + 1])*(273.15 + x_old[i23 + 1]) - (273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)*(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)*(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)*(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)));
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

							if (rthdsd_loc123 != NULL) {
								free(rthdsd_loc123);
							}
							rthdsd_loc123 = NULL;

							if (x_temper != NULL) {
								free(x_temper);
							}
							x_temper = NULL;
						}
						else if (b_sign_on_nonlinear_bc) {
							//  25 ������� 2015. ��������� ���������� ��� ������������� 
							// ���������� ��������� �������.
							doublerealT* x_temper = NULL;
							//x_temper = new doublerealT[n_a[0] + 1];
							x_temper = (doublerealT*)malloc(((integer)(n_a[0]) + 1) * sizeof(doublerealT));
							if (x_temper == NULL) {
								// ������������ ������ �� ������ ������������.
								printf("Problem : not enough memory on your equipment for x_temper my_agregat_amg.cpp...\n");
								printf("Please any key to exit...\n");
								exit(1);
							}
#pragma omp parallel for
							for (integer i23 = 0; i23 < n_a[0]; i23++) {
								if (x[i23 + 1] < -272.15) x[i23 + 1] = -272.15;
								//x_temper[i23] = x[i23 + 1];
								// 0.01 �������� ������ ����������.
								// 0.25
								// 0.2
								// 10 ���� 2018 ���� ������� �� ����������� ������ ���������� ������ 0.9.
								x_temper[i23] = x_old[i23 + 1] + 0.9*(x[i23 + 1] - x_old[i23 + 1]);
								if (x_temper[i23] < -272.15) x_temper[i23] = -272.15;
								x[i23 + 1] = x_temper[i23];
							}

							doublerealT* rthdsd_loc123 = NULL;
							//rthdsd_loc123 = new doublerealT[n_a[0] + 1];
							rthdsd_loc123 = (doublerealT*)malloc(((integer)(n_a[0]) + 1) * sizeof(doublerealT));
							if (rthdsd_loc123 == NULL) {
								// ������������ ������ �� ������ ������������.
								printf("Problem : not enough memory on your equipment for rthdsd_loc123 my_agregat_amg.cpp...\n");
								printf("Please any key to exit...\n");
								exit(1);
							}

							for (integer i23 = 0; i23 < n_a[0]; i23++) {
								rthdsd_loc123[i23] = rthdsd_no_radiosity_patch[i23];
								if ((i23 >= iadd_qnbc_maxelm) && (qnbc[i23 - iadd_qnbc_maxelm].bactive) && (qnbc[i23 - iadd_qnbc_maxelm].bStefanBolcman_q_on) && (qnbc[i23 - iadd_qnbc_maxelm].bNewtonRichman_q_on == false)) {
									// ������ ��������.
									doublerealT alpha_relax142 = 0.25;
									rthdsd_loc123[i23] = alpha_relax142 *(-qnbc[i23 - iadd_qnbc_maxelm].emissivity*5.670367e-8*((273.15 + x_temper[i23])*(273.15 + x_temper[i23])*(273.15 + x_temper[i23])*(273.15 + x_temper[i23]) - (273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)*(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)*(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)*(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb))) +
										(1.0 - alpha_relax142)*(-qnbc[i23 - iadd_qnbc_maxelm].emissivity*5.670367e-8*((273.15 + x_old[i23 + 1])*(273.15 + x_old[i23 + 1])*(273.15 + x_old[i23 + 1])*(273.15 + x_old[i23 + 1]) - (273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)*(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)*(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)*(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)));
									rthdsd_loc123[i23] *= qnbc[i23 - iadd_qnbc_maxelm].dS;
								}
								if ((i23 >= iadd_qnbc_maxelm) && (qnbc[i23 - iadd_qnbc_maxelm].bactive) && (qnbc[i23 - iadd_qnbc_maxelm].bNewtonRichman_q_on) && (qnbc[i23 - iadd_qnbc_maxelm].bStefanBolcman_q_on == false)) {
									// ������-������.
									//doublerealT alpha_relax142 = 0.25;
									rthdsd_loc123[i23] = -qnbc[i23 - iadd_qnbc_maxelm].film_coefficient*(x_temper[i23] - qnbc[i23 - iadd_qnbc_maxelm].Tamb);
									rthdsd_loc123[i23] *= qnbc[i23 - iadd_qnbc_maxelm].dS;
								}
								if ((i23 >= iadd_qnbc_maxelm) && (qnbc[i23 - iadd_qnbc_maxelm].bactive) && (qnbc[i23 - iadd_qnbc_maxelm].bNewtonRichman_q_on) && (qnbc[i23 - iadd_qnbc_maxelm].bStefanBolcman_q_on)) {
									// ������� ���������� ����.
									//doublerealT alpha_relax142 = 0.25;
									rthdsd_loc123[i23] = (-qnbc[i23 - iadd_qnbc_maxelm].emissivity*5.670367e-8*((273.15 + x_temper[i23])*(273.15 + x_temper[i23])*(273.15 + x_temper[i23])*(273.15 + x_temper[i23]) - (273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)*(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)*(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)*(273.15 + qnbc[i23 - iadd_qnbc_maxelm].Tamb)));
									rthdsd_loc123[i23] += -qnbc[i23 - iadd_qnbc_maxelm].film_coefficient*(x_temper[i23] - qnbc[i23 - iadd_qnbc_maxelm].Tamb);
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

							if (rthdsd_loc123 != NULL) {
								free(rthdsd_loc123);
							}
							rthdsd_loc123 = NULL;

							if (x_temper != NULL) {
								free(x_temper);
							}
							x_temper = NULL;

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
			if (((adiabatic_vs_heat_transfer_coeff > 0) || (breakRUMBAcalc_for_nonlinear_boundary_condition)) && (count_iter_for_film_coef>1250)) break;

			// 1 dec 2016.
			//  ���������� ����� 2 ��� 5 V ������ ����������� ���������� ����� �� ����� ����������.
			if (bvacuumPrism) {
				// 5
				// 250
				if (icount_V_cycle > 250) break;
			}


			if ((iter_limit == 5000) || ((iVar == PAM) && (fabs(dres)>7.0e3))) {
#pragma omp parallel for
				for (integer i47 = 1; i47 <= n_a[0]; i47++) {
					x[i47] = x_copy[i47];
				}
				if (iVar == PAM) {
					printf("pressure amendment divergence...\n");
				}
				printf("amg divergence detected dres=%e...\n", dres);
#if doubleintprecision == 1
				printf("nV=%lld dres0=%e\n", icount_V_cycle, dres_initial);
#else
				printf("nV=%d dres0=%e\n", icount_V_cycle, dres_initial);
#endif

				printf("CopA=%1.2f  CopP=%1.2f...\n", dr_grid_complexity, (doublerealT)(nnz_P_memo_all / n_a[0]));
				printf("res_best_search=%e\n", res_best_search);
				//getchar();
				// ����� ������ 22 12 2016
				//system("PAUSE");
				break;
			}

			if (iter_limit == 1) {
				// ��������� �������.
				res0start = fabs(dres);
			}

			// ������� �� ����������� :
			// ��� ���������� ��� ���� ���������� � ����������������� �������� � ��������� �� ��������.
			//if (iVar == TEMP) printf("temp res=%e\n", fabs(dres));

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
			// ������� ��� ���� � ��� ������ 1.0 ��� ����������� ��� �� �� ������ �������.
			for (integer i47 = 1; i47 <= n_a[0]; i47++) {
			x_best_search[i47] = x[i47];
			}
			}
			}
			*/

			// debug 7 ���� 2016
			//if (iter_limit > 300) {
			//printf("amg divergense detected...9 june 2016\n");
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
					if ((fabs(res_best_search / res0start) < 0.23) && (fabs(res_best_search) < 1.0e-3*sqrt(n_a[0]))) {
						// ���� ������� ������ �������������� �� ��� �������.
						// �������� ����������� � ����� � ���������.
#pragma omp parallel for
						for (integer i47 = 1; i47 <= n_a[0]; i47++) {
							x[i47] = x_best_search[i47];
						}
						break;
					}
					else if ((fabs(res_best_search / res0start) <= 1.0) && (fabs(res_best_search) < 1.0e-4*sqrt(n_a[0]))) {
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
					// ��� ����� ���� ������ ������� �� ���������.
					printf("Fatal amg error : Strong divergence amg solver...%e \n", fabs(res_best_search / res0start));
					printf("res_best_search=%e, res0start=%e\n", fabs(res_best_search), fabs(res0start));
					printf("BiCGStab+ILU2 is start now...\n");
					printf("please wait...");
					system("pause");
					break; // ��������� ����� �� while �����.
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
			//printf("\ndivergence AMG detected...solver will be restart... please wait... ... ...\n");
			//printf("\a\a\a\a\a\a\a\a");
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

			if ((icount_bad_convergence_Vcycles >= istop_porog_reconst) || (fabs(dres) / sqrt(n_a[0]) > 1.0e30)) {
				// ������������� 10 ����� ������������ ������ ��-�������� ����� ����������.
				// ����� � ������������ ������� ������� ������� 1.0e30.

				//if (fabs(dres) < 1.0e-3) break; // ����� ������� ���������� ���������� �������.
				if ((fabs(res_best_search / res0start) < 1.0e-1) && (fabs(dres) / sqrt(n_a[0]) < 1.0e-3)) {
					// ���� ������� ������ �������������� �� ��� �������.
					// �������� ����������� � ����� � ���������.
#pragma omp parallel for
					for (integer i47 = 1; i47 <= n_a[0]; i47++) {
						x[i47] = x_best_search[i47];
					}
					printf("stagnaion break out\n");
					break;
				}
				i_count_stagnation++;

				printf("\ndivergence AMG detected...solver will be restart... please wait... ... ...\n");
				//printf("\a\a\a\a\a\a\a\a");
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
						x[i47] = signumnow *1.0*(rand() % 90 + 10) / 100.0; // ��������� ����� � ��������� �� 0 �� 1.
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
							printf("buffers1omega=%1.4f, buffers2omega=%1.4f, buffers3omega=%1.4f\n", buffers1omega, buffers2omega, buffers3omega);
						}
					}
					else {
						// ����� omega.
						bproblem_amg_convergence2 = true;
						icount_bad_convergence_Vcycles = 0;
						buffers2omega = dres / dres_previos;
						printf("buffers1omega=%1.4f, buffers2omega=%1.4f\n", buffers1omega, buffers2omega);
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
				residualq2(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0], diag[0]);
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
					residualq2(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0], diag[0]);
					Rnext_0 = norma(residual_fine[0], n_a[0]);
					// this is process flow logic
					if (Rnext_0 > process_flow_beta*Rprev_0) {
						// ����� ����������� � ��� ��� �� �������� �������� �� ����������.
						break; // �������� ���������� �� ��������� ������� ���� �� ���� �������.
					}
					else {
						Rprev_0 = Rnext_0;
					}
				}
				if (iter == nu1) {
					printf("level 0 limit presmother iteration is reached\n");
				}

			}
			else {
				// smoother
				for (integer iter = 0; iter < nu1; iter++) {
					//seidel(Amat, 1, nnz_a[0], x, b, flag, n_a[0]);
					//quick seidel
					if (bonly_serial) {
						if (bILU2smoother == 1) {
							// ILU0
							seidelq<doublereal>(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, F_false_C_true, 0);
							residualq2(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0], diag[0]);
#pragma omp parallel for
							for (integer i43 = 0; i43 < n_a[0]; i43++) {
								milu0[0].zbuf[i43 + 1] = residual_fine[0][i43 + 1];
							}
							lusol_1patchforRUMBA(n_a[0], milu0[0].zbuf, milu0[0].zbuf2, milu0[0]);
#pragma omp parallel for
							for (integer i43 = 0; i43 < n_a[0]; i43++) {
								x[i43 + 1] += milu0[0].zbuf2[i43 + 1];
							}
						}
						else if ((bILU2smoother == 2) || (my_amg_manager.iFinnest_ilu == 1)) {
							seidelq<doublereal>(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, F_false_C_true, 0);
							residualq2(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0], diag[0]);
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
							seidelq<doublereal>(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, F_false_C_true, 0);
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
				residualq2(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0], diag[0]);
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
					//  ���������� ��������� - ��������� ����� �� ������� ���������� ������.
					if ((fabs(minx - minx_gl) < 1.0e-2) && (fabs(maxx - maxx_gl) < 1.0e-2)) {
						printf("Solution nonlinear problem converged succsefull. Ok...\n");
						break;
					}
				}

				minx_gl = minx;
				maxx_gl = maxx;

#if doubleintprecision == 1
				printf("%lld %e rho=%e min=%e max=%e\n", iiter, dres, dres / rho, minx, maxx);
#else
				printf("%d %e rho=%e min=%e max=%e\n", iiter, dres, dres / rho, minx, maxx);
#endif

				if (!((iVar == TEMP) && (my_amg_manager.istabilization == 3))) {
					if (fabs(1.0 - fabs(dres / rho)) < 1.0e-3) {
						printf("stagnation in amg solver determinate ...\n");
						// 28_10_2016.
						// ������������ ��������� ����� �� ������������, 
						// �.�. ������� ��������� ��������.
						break;
					}
				}
				if (icount_V_cycle == 1) {
					if (fabs(dres / rho)<1.0) {
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

				printf("amg solver divergence detected.\n");
				system("pause");

#pragma omp parallel for
				for (integer i47 = 1; i47 <= n_a[0]; i47++) {
					///x[i47] = x_best_search[i47];
					//x_copy[i47] = x[i47]; // 4 ������ 2016.
					x[i47] = x_copy[i47];
				}
				residualq2_analysys(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0], diag[0]);

				printf("dres_initial=%e res_best_search=%e dres=%e current=%e\n", dres_initial, res_best_search, dres, norma(residual_fine[0], n_a[0]));
				printf("break. amg divergence detected. fabs(dres) > 1.0e7\n");
				//getchar();
				if ((bILU2smoother == 2) || (bILU2smoother == 0)) {
					printf("apply ilu2 smoother for number 0 level\n");
					equation3DtoCRSRUMBA1(milu2[0], true, Amat, 1, n_a[0], row_ptr_start, row_ptr_end, 0, 0);

				}
				if (bILU2smoother == 0) {
					// ������������.
					memory_allocation_apostoriory_buffer_ilu(milu2, ilevel - 1);
					bILU2smoother = 2;
				}

				// ��� �� ��������� ��� �������� ��������.
				doublerealT dresfinish_probably = 0.1*norma(residual_fine[0], n_a[0]);
				if ((iVar == VX) || (iVar == VY) || (iVar == VZ)) {
					// ��� �� ��������� ��� ��������� �������� ������ SIMPLE ���������.
					dresfinish_probably = 1.0e-3*norma(residual_fine[0], n_a[0]);
				}
				if (bSIMPLErun_now_for_temperature == true) {
					if (iVar == TEMP) {
						// ��� ���� ���������� ��� ����������������� �������.
						// � BiCGStab Internal 3 ����������� �� 1e-10.
						dresfinish_probably = 1.0e-3*norma(residual_fine[0], n_a[0]);
					}
				}
				if (iVar == TOTALDEFORMATIONVAR) {
					// ��� ������������ ����������
					dresfinish_probably = 1.0e-3*norma(residual_fine[0], n_a[0]);
				}
				if (bonly_solid_calculation) {
					dresfinish_probably = 1.0e-5*norma(residual_fine[0], n_a[0]);
				}
				integer i943 = 0;
				for (integer i_prob_detect_i = 0; i_prob_detect_i < 1000; i_prob_detect_i++) {
					i943 = i_prob_detect_i;
					seidelq<doublereal>(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, F_false_C_true, 0);
					residualq2(Amat, 1, n_a[0], x, b, row_ptr_start, row_ptr_end, 0, residual_fine[0], diag[0]);

					doublereal minx = 1.0e30;
					doublereal maxx = -1.0e30;
					for (integer i_83 = 1; i_83 <= n_a[0]; i_83++) {
						if (x[i_83] < minx) minx = x[i_83];
						if (x[i_83] > maxx) maxx = x[i_83];
					}

#if doubleintprecision == 1
					printf("%lld residual=%e min=%e max=%e \n", i_prob_detect_i, norma(residual_fine[0], n_a[0]), minx, maxx);
#else
					printf("%d residual=%e min=%e max=%e \n", i_prob_detect_i, norma(residual_fine[0], n_a[0]), minx, maxx);
#endif

					// ��������� ����� �� �����.
					if (norma(residual_fine[0], n_a[0]) < dresfinish_probably) {
						printf("Ok!!! calculation local compleate... \n");
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
					printf("Fatal error !!! ilu2 divergence detected... \n");
					printf("residual curent=%e target residual=%e\n", norma(residual_fine[0], n_a[0]), dresfinish_probably);
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
#if doubleintprecision == 1
						printf("iter = %lld\n", iiter);
#else
						printf("iter = %d\n", iiter);
#endif

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
				row_ptr_end, residual_fine, diag, n_a, bonly_serial,
				process_flow_beta, F_false_C_true, nu1, nu2, bILU2smoother,
				ilevel, 1, imyinit, idim_diag, milu2, milu0, nested_desection,
				R, P, nnz_aRP, flag, residual_coarse, igam, nnz_a,
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
		doublereal* val75 = NULL;
		val75 = new doublereal[nnz_a[0]];
		integer* col_ind75 = NULL;
		col_ind75 = new integer[nnz_a[0]];
		integer* row_ptr75 = NULL;
		row_ptr75 = new integer[n_a[0] + 1];
		if ((val75 == NULL) || (col_ind75 == NULL) || (row_ptr75 == NULL)) {
			// ������������ ������ �� ������ ������������.
			printf("Problem : not enough memory on your equipment for val, col_ind or row_ptr: bicgStab + camg...\n");
			printf("Please any key to exit...\n");
			exit(1);
		}
		
		// ������������� �������.
#pragma omp parallel for
		for (integer i_1 = 1; i_1 <= n_a[0]; i_1++) {

			for (integer i_2 = row_ptr_start[i_1]; i_2 <= row_ptr_end[i_1]; i_2++) {
				if (Amat[i_2].i == Amat[i_2].j) {
					if (i_1 != Amat[i_2].i) {
						printf("err i!=i\n");
						system("PAUSE");
					}
					val75[i_2 - 1] = diag[0][i_1];
					col_ind75[i_2 - 1] = i_1 - 1;
				}
				else {
					val75[i_2 - 1] = Amat[i_2].aij;
					col_ind75[i_2 - 1] = Amat[i_2].j - 1;
				}
			}
			row_ptr75[i_1 - 1] = row_ptr_start[i_1] - 1;
		}
#if doubleintprecision == 1
		//printf("nnz=%lld rpe=%lld rps=%lld\n",nnz_a[0], row_ptr_end[n_a[0]], row_ptr_start[n_a[0]+1]-1);
#else
		//printf("nnz=%d rpe=%d rps=%d\n",nnz_a[0], row_ptr_end[n_a[0]], row_ptr_start[n_a[0]+1]-1);
#endif

		//system("PAUSE");
		row_ptr75[n_a[0]] = row_ptr_end[n_a[0]];
		// ������� ����������� ��� ������ BiCGStab.
		doublereal* ri75 = NULL;
		doublereal* roc75 = NULL;
		doublereal* s75 = NULL;
		doublereal* t75 = NULL;
		doublereal* vec75 = NULL;
		doublereal* vi75 = NULL;
		doublereal* pi75 = NULL;
		doublereal* dx75 = NULL;
		doublereal* dax75 = NULL;
		doublereal* y75 = NULL;
		doublereal* z75 = NULL;
		// ������ ������������������:
		doublereal* y76 = NULL;
		doublereal* pi76 = NULL;
		y76 = new doublereal[n75 + 1];
		pi76 = new doublereal[n75 + 1];
		// ������ ������������������:
		doublereal* z76 = NULL;
		doublereal* s76 = NULL;
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
		if ((ri75 == NULL) || (roc75 == NULL) || (s75 == NULL) || (t75 == NULL) || (vi75 == NULL) || (pi75 == NULL) || (dx75 == NULL) || (dax75 == NULL) || (y75 == NULL) || (z75 == NULL)) {
			// ������������ ������ �� ������ ������������.
			printf("Problem : not enough memory on your equipment for : bicgStab + camg...\n");
			printf("Please any key to exit...\n");
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
		for (i75 = 0; i75<n75; i75++) {
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
			if (fabs(delta075)<1.0e-4*dterminatedTResudual) iflag75 = 0;
		}
		else {
			if (fabs(delta075)<dterminatedTResudual) iflag75 = 0;
		}
		integer iflag175 = 1;
		if (fabs(delta075)<1e-14) iflag175 = 0;
		if ((iVar == TEMP) && (iflag75 == 0) && (iflag175 == 0)) {
#if doubleintprecision == 1
			printf("bicgStab+camg: iflag=%lld, iflag1=%lld, delta0=%e\n", iflag75, iflag175, delta075);
#else
			printf("bicgStab+camg: iflag=%d, iflag1=%d, delta0=%e\n", iflag75, iflag175, delta075);
#endif

			system("PAUSE");
		}

		integer iN75 = 10;
		if (n75<30000) {
			// ������ ����� ����� ����������� !
			if ((iVar == VX) || (iVar == VY) || (iVar == VZ)) {
				iN75 = 1; // ����������� ����� ���� �� ���� ��������.
						  // ���� ����� ����� ������������ �� �� �� ����� ����� ����������� �� ��� ��� ���� ������� �� ������ ������ epsilon.
				if (1.0e-3*fabs(delta075)<epsilon75) {
					epsilon75 = 1.0e-3*fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
			}
			if (iVar == TEMP) {
				iN75 = 2;
				epsilon75 = fmin(0.1*fabs(delta075), epsilon75);
				if (bSIMPLErun_now_for_temperature == true) {
					//printf("epsilon=%e \n",epsilon);
					//getchar();
					// ����������������� ������� ���������� ������������������ �� ����������� ��� ������������������ ��������.
					// ������� �������� ���� ������ ��������� �� 5 ��������.
					// 27.07.2016
					epsilon75 *= 1e-10;
					iN75 = 20;
					//epsilon75 *= 1e-16;
					//iN75 = 30;
				}
			}
			if (iVar == PAM) {
				iN75 = 3; // ������� ��� �������� �������� ������ ���� �������� �����.
				if (1.0e-3*fabs(delta075)<epsilon75) {
					epsilon75 = 1.0e-3*fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
				//printf("%e",epsilon75); getchar();
			}
		}
		else if ((n75 >= 30000) && (n75 < 100000)) {
			// ����� � ������� �������� ����� �������� � 
			// ������������� ������� ��������� ����� ������� 
			// ��������, �� ��� �� ��������.
			// ������� ������ � ��� ��� ������� �� ����������� ������-�� �� ��������.
			// ������ ��������� �����������.
			if ((iVar == VX) || (iVar == VY) || (iVar == VZ)) {
				iN75 = 3; // ����������� ����� ���� �� ���� ��������.
						  // ���� ����� ����� ������������ �� �� �� ����� ����� ����������� �� ��� ��� ���� ������� �� ������ ������ epsilon.
				if (1.0e-3*fabs(delta075)<epsilon75) {
					epsilon75 = 1.0e-3*fabs(delta075);
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
				epsilon75 = fmin(0.1*fabs(delta075), epsilon75);
				if (bSIMPLErun_now_for_temperature == true) {
					//printf("epsilon75=%e \n",epsilon75);
					//getchar();
					// ����������������� ������� ���������� ������������������ �� ����������� ��� ������������������ ��������.
					// ������� �������� ���� ������ ��������� �� 5 ��������.
					// 27.07.2016
					epsilon75 *= 1e-10;
					iN75 = 20;
					//epsilon75 *= 1e-16;
					//iN75 = 30;
				}
			}
			if (iVar == PAM) {
				iN75 = 6; // ������� ��� �������� �������� ������ ���� �������� �����.
				if (1.0e-3*fabs(delta075)<epsilon75) {
					epsilon75 = 1.0e-3*fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
				//printf("%e",epsilon75); getchar();
				// 27.07.2016.
				epsilon75 *= 1e-2;
				iN75 = 20;
			}
		}
		else if ((n75 >= 100000) && (n75<300000)) {
			// ������ ��������� ������� �����������.
			if ((iVar == VX) || (iVar == VY) || (iVar == VZ)) {
				iN75 = 3; // ����������� ����� ���� �� ���� ��������.
						  // ������ ������ ������� ��� ��������� ������ ����� ������ ������� ������ ���������� iN �������� ��� ��������.
						  // ���� ����� ����� ������������ �� �� �� ����� ����� ����������� �� ��� ��� ���� ������� �� ������ ������ epsilon.
				if (1.0e-3*fabs(delta075)<epsilon75) {
					epsilon75 = 1.0e-3*fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
			}
			if (iVar == TEMP) {
				iN75 = 4;
				epsilon75 = fmin(0.1*fabs(delta075), epsilon75);
				if (bSIMPLErun_now_for_temperature == true) {
					//printf("epsilon75=%e \n",epsilon75);
					//getchar();
					// ����������������� ������� ���������� ������������������ �� ����������� ��� ������������������ ��������.
					// ������� �������� ���� ������ ��������� �� 5 ��������.
					// 27.07.2016
					epsilon75 *= 1e-10;
					iN75 = 20;
					//epsilon75 *= 1e-16;
					//iN75 = 30;
				}
			}
			if (iVar == PAM) {
				iN75 = 8; // ������� ��� �������� �������� ������ ���� �������� �����.
				if (1.0e-4*fabs(delta075)<epsilon75) {
					epsilon75 = 1.0e-4*fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
				//printf("%e",epsilon75); getchar();
			}
		}
		else if ((n75 >= 300000) && (n75<1000000)) {
			// ������ ������� ������� �����������.
			if ((iVar == VX) || (iVar == VY) || (iVar == VZ)) {
				iN75 = 3; // ����������� ����� ���� �� ���� ��������.
						  // ���� ����� ����� ������������ �� �� �� ����� ����� ����������� �� ��� ��� ���� ������� �� ������ ������ epsilon.
				if (1.0e-3*fabs(delta075)<epsilon75) {
					epsilon75 = 1.0e-3*fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
			}
			if (iVar == TEMP) {
				iN75 = 4;
				epsilon75 = 1e-5*fmin(0.1*fabs(delta075), epsilon75);
				if (bSIMPLErun_now_for_temperature == true) {
					//printf("epsilon75=%e \n",epsilon75);
					//getchar();
					// ����������������� ������� ���������� ������������������ �� ����������� ��� ������������������ ��������.
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
				if (1.0e-4*fabs(delta075)<epsilon75) {
					epsilon75 = 1.0e-4*fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
				//printf("%e",epsilon75); getchar();
			}
		}
		else if ((n75 >= 1000000) && (n75<3000000)) {
			// ������ ���������� ������� �����������.
			if ((iVar == VX) || (iVar == VY) || (iVar == VZ)) {
				iN75 = 6; // ����������� ����� ���� �� ���� ��������.
						  // ���� ����� ����� ������������ �� �� �� ����� ����� ����������� �� ��� ��� ���� ������� �� ������ ������ epsilon.
				if (1.0e-3*fabs(delta075)<epsilon75) {
					epsilon75 = 1.0e-3*fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
			}
			if (iVar == TEMP) {
				iN75 = 8;
				epsilon75 = 1e-5*fmin(0.1*fabs(delta075), epsilon75);
				if (bSIMPLErun_now_for_temperature == true) {
					//printf("epsilon75=%e \n",epsilon75);
					//getchar();
					// ����������������� ������� ���������� ������������������ �� ����������� ��� ������������������ ��������.
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
				if (1.0e-4*fabs(delta075)<epsilon75) {
					epsilon75 = 1.0e-4*fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
				//printf("%e",epsilon75); getchar();
			}
		}
		else if (n75 >= 3000000) {
			// ������ ����� ������� �����������.
			if ((iVar == VX) || (iVar == VY) || (iVar == VZ)) {
				iN75 = 6; // ����������� ����� ���� �� ���� ��������.
						  // ���� ����� ����� ������������ �� �� �� ����� ����� ����������� �� ��� ��� ���� ������� �� ������ ������ epsilon.
				if (1.0e-3*fabs(delta075)<epsilon75) {
					epsilon75 = 1.0e-3*fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
			}
			if (iVar == TEMP) {
				iN75 = 8;
				epsilon75 = 1e-10*fmin(0.1*fabs(delta075), epsilon75);
			}
			if (iVar == PAM) {
				iN75 = 36; // ������� ��� �������� �������� ������ ���� �������� �����.
				if (1.0e-4*fabs(delta075)<epsilon75) {
					epsilon75 = 1.0e-4*fabs(delta075);
				}
				if (iflag175 == 1) {
					iflag75 = 1;
				}
				//printf("%e",epsilon); getchar();
			}
		}

		integer maxit75 = 2000;
		if (iVar == TEMP) {
			maxit75 = 2000;
		}
		if (iVar == PAM) {
			maxit75 = 2000; // 2000
		}
		if ((iVar == VX) || (iVar == VY) || (iVar == VZ)) {
			maxit75 = 100;//100
		}
		if (iVar == TOTALDEFORMATIONVAR) {
			maxit75 = 2000; // 2000
			if (1.0e-4*fabs(delta075) < epsilon75) {
				epsilon75 = 1.0e-4*fabs(delta075);
			}
			epsilon75 = 1.0e-16;
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


		// �� ����������� ������ ������� ��������� ��������. (�� ����� 10).
		// ���� ������ ������� �� ������������� ��������� ������������.
		while (((icount75 < iN75) && (iflag175 != 0)) || (iflag75 != 0 && icount75 < maxit75)) {

			// 6.01.2017: Body BiCGStab + AMG. (BiCGStab_internal4).


			icount75++;

			count_iter_for_film_coef75++;
			// � ������ ������ ������� - �������, �������-��������� � ��������� ������� �� ��������� �� ����� ��������, 
			// �.�. ��� ��������� ������ ���������� �������. 13 ����� 2016.
			//if (((adiabatic_vs_heat_transfer_coeff > 0) || (breakRUMBAcalc_for_nonlinear_boundary_condition)) && (count_iter_for_film_coef75>5)) break;

			roi75 = Scal(roc75, ri75, n75);
			bet75 = (roi75 / roim175)*(al75 / wi75);


			//printf("%e %e %e %e\n",roi75,roim175,al75,wi75);
			//getchar();

#pragma omp parallel for 
			for (i75 = 0; i75<n75; i75++) {
				doublereal pibuf75 = ri75[i75] + (pi75[i75] - vi75[i75] * wi75)*bet75;
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
			// TODO begin
			// ��������� 6.01.2017 begin
			// ������ V ����� ������������.
			// A*y76=pi76;
			V_cycle_solve<doublerealT>(Amat, y76, pi76, process_flow_logic, row_ptr_start,
				row_ptr_end, residual_fine, diag, n_a, bonly_serial,
				process_flow_beta, F_false_C_true, nu1, nu2, bILU2smoother,
				ilevel, inumberVcyclelocbicgstab, imyinit, idim_diag, milu2, milu0, nested_desection,
				R, P, nnz_aRP, flag, residual_coarse, igam, nnz_a,
				error_approx_coarse, dapply_ilu_max_pattern_size,
				process_flow_alpha,
				error_approx_fine, nFinestSweeps);
			// TODO end
			// ��������� 6.01.2017 end

			// ����������� ����������.
#pragma omp parallel for
			for (i75 = 0; i75 < n75; i75++) {
				y75[i75] = y76[i75 + 1];
			}

			MatrixCRSByVector(val75, col_ind75, row_ptr75, y75, vi75, n75); // vi==A*y;

			if ((fabs(roi75)<1e-30) && (fabs(Scal(roc75, vi75, n75))<1e-30)) {
				al75 = 1.0;
			}
			else if (fabs(roi75)<1e-30) {
				al75 = 0.0;
			}
			else {
				al75 = roi75 / Scal(roc75, vi75, n75);
			}


#pragma omp parallel for
			for (i75 = 0; i75<n75; i75++) {
				s75[i75] = ri75[i75] - al75*vi75[i75];
			}

			// ������ ������������������.
			// Kz=s

#pragma omp parallel for
			for (i75 = 0; i75<n75; i75++) z75[i75] = 0.0; // ���� �������� �� � ���� �� ������� ���������� ��� PAM !.

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
				row_ptr_end, residual_fine, diag, n_a, bonly_serial,
				process_flow_beta, F_false_C_true, nu1, nu2, bILU2smoother,
				ilevel, inumberVcyclelocbicgstab, imyinit, idim_diag, milu2, milu0, nested_desection,
				R, P, nnz_aRP, flag, residual_coarse, igam, nnz_a,
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
			// printf("%e %e",Scal(t75,s75,n75),Scal(t75,t75,n75));

#pragma omp parallel for
			for (i75 = 0; i75<n75; i75++) {
				//dx75[i75]+=al75*pi75[i75]+wi75*s75[i75]; // ��� ���� ��� �������������������
				dx75[i75] += al75*y75[i75] + wi75*z75[i75]; // ��� ����� � ��������������������
				ri75[i75] = s75[i75] - wi75*t75[i75];
			}
			deltai75 = NormaV(ri75, n75);

			//printf("deltai75=%e\n",deltai75); getchar();

			// ������ ������� �� �������
			if (bprint_mesage_diagnostic) {
				if ((icount75 % 10) == 0) {
					printf("iter  residual\n");
					//fprintf(fp_log, "iter  residual\n");
				}
#if doubleintprecision == 1
				printf("%lld %e\n", icount75, deltai75);
				//fprintf(fp_log, "%lld %e \n", icount75, deltai75);
#else
				printf("%d %e\n", icount75, deltai75);
				//fprintf(fp_log, "%d %e \n", icount75, deltai75);
#endif

			}

			// 28.07.2016.
#if doubleintprecision == 1
			//printf("%lld %e\n", icount75, deltai75);
			//fprintf(fp_log, "%lld %e \n", icount75, deltai75);
#else
			//printf("%d %e\n", icount75, deltai75);
			//fprintf(fp_log, "%d %e \n", icount75, deltai75);
#endif

			//getchar();
			if (deltai75 > delta_old_iter75) i_signal_break_pam_opening75++;
			delta_old_iter75 = deltai75;
			if (iVar == PAM) {
				if (i_signal_break_pam_opening75 > i_limit_signal_pam_break_opening75) {
					// ��������� ����� �� �����.
#if doubleintprecision == 1
					printf("icount PAM=%lld\n", icount75);
#else
					printf("icount PAM=%d\n", icount75);
#endif

					break;
				}
			}

			if (deltai75 <epsilon75) iflag75 = 0; // ����� ����������
			else roim175 = roi75;

			if (iVar == TEMP) {
#if doubleintprecision == 1
				//printf("epsilon=%e deltai=%e icount=%lld\n",epsilon75,deltai75, icount75);
#else
				//printf("epsilon=%e deltai=%e icount=%d\n",epsilon75,deltai75, icount75);
#endif

				//getchar();
			}

			icount_V_cycle = icount75; // ���������� �������� � BiCGStabP ��� ����.

			if (icount75 > 2600) break; // 15.02.2017

		}

		// ����������� ���������� ����������.
#pragma omp parallel for
		for (i75 = 0; i75 < n75; i75++) {
			x[i75 + 1] = dx75[i75];
			x_best_search[i75 + 1] = dx75[i75];
		}

		// ������������ ����������� ������.
		// ������ ������������������
		if (pi76 != NULL) {
			delete[] pi76;
			pi76 = NULL;
		}
		if (y76 != NULL) {
			delete[] y76;
			y76 = NULL;
		}
		// ������ ������������������
		if (z76 != NULL) {
			delete[] z76;
			z76 = NULL;
		}
		if (s76 != NULL) {
			delete[] s76;
			s76 = NULL;
		}
		if (ri75 != NULL) {
			delete[] ri75;
			ri75 = NULL;
		}
		if (roc75 != NULL) {
			delete[] roc75;
			roc75 = NULL;
		}
		if (s75 != NULL) {
			delete[] s75;
			s75 = NULL;
		}
		if (t75 != NULL) {
			delete[] t75;
			t75 = NULL;
		}
		if (vec75 != NULL) {
			delete[] vec75;
			vec75 = NULL;
		}
		if (vi75 != NULL) {
			delete[] vi75;
			vi75 = NULL;
		}
		if (pi75 != NULL) {
			delete[] pi75;
			pi75 = NULL;
		}
		if (dx75 != NULL) {
			delete[] dx75;
			dx75 = NULL;
		}
		if (dax75 != NULL) {
			delete[] dax75;
			dax75 = NULL;
		}
		if (y75 != NULL) {
			delete[] y75;
			y75 = NULL;
		}
		if (z75 != NULL) {
			delete[] z75;
			z75 = NULL;
		}

		// ������������ ����������� ������.
		if (val75 != NULL) {
			delete[] val75;
			val75 = NULL;
		}
		if (col_ind75 != NULL) {
			delete[] col_ind75;
			col_ind75 = NULL;
		}
		if (row_ptr75 != NULL) {
			delete[] row_ptr75;
			row_ptr75 = NULL;
		}

	}
	else
	{   //  09.01.2018
		// ������������� ������������ ��������� ��������: ������� ��� FGMRES � ��������� ��� ������������������ � ������� V - �����.
		// FGMRes if (my_amg_manager.istabilization == 2)
		// ������ ������� ����������� ������ ����������� ������� � ������� �� ������ ��������
		// ���������� ����������� ������������� �������������������. ������� ����� � ������ 1986 ����.

		integer inumberVcyclelocbicgstab = 1;

		// ��������� �������� ���������� � ����.
		integer n75 = n_a[0]; // ����� ����������� �� ��������� ������.
		doublereal* val75 = NULL;
		val75 = new doublereal[nnz_a[0]];
		integer* col_ind75 = NULL;
		col_ind75 = new integer[nnz_a[0]];
		integer* row_ptr75 = NULL;
		row_ptr75 = new integer[n_a[0] + 1];
		if ((val75 == NULL) || (col_ind75 == NULL) || (row_ptr75 == NULL)) {
			// ������������ ������ �� ������ ������������.
			printf("Problem : not enough memory on your equipment for val, col_ind or row_ptr: bicgStab + camg...\n");
			printf("Please any key to exit...\n");
			exit(1);
		}
	
		// ������������� �������.
#pragma omp parallel for
		for (integer i_1 = 1; i_1 <= n_a[0]; i_1++) {

			for (integer i_2 = row_ptr_start[i_1]; i_2 <= row_ptr_end[i_1]; i_2++) {
				if (Amat[i_2].i == Amat[i_2].j) {
					if (i_1 != Amat[i_2].i) {
						printf("err i!=i\n");
						system("PAUSE");
					}
					val75[i_2 - 1] = diag[0][i_1];
					col_ind75[i_2 - 1] = i_1 - 1;
				}
				else {
					val75[i_2 - 1] = Amat[i_2].aij;
					col_ind75[i_2 - 1] = Amat[i_2].j - 1;
				}
			}
			row_ptr75[i_1 - 1] = row_ptr_start[i_1] - 1;
		}

		row_ptr75[n_a[0]] = row_ptr_end[n_a[0]];


		bool bnorelax = true; // ��� ��������� ���������������� �� ������������ ����������.
		integer m_restart = my_amg_manager.m_restart;

		doublereal resid;
		integer i, j = 1, k;
		//Vector s(m + 1), cs(m + 1), sn(m + 1), w;
		doublereal* w = new doublereal[n75];
		doublereal* s = new doublereal[m_restart + 2];
		doublereal* cs = new doublereal[m_restart + 2];
		doublereal* sn = new doublereal[m_restart + 2];

		doublereal *dx = new doublereal[n75];
		doublereal *buffer = new doublereal[n75];
		doublereal *Zcopy = new doublereal[n75 + 1];
		doublereal *vCopy = new doublereal[n75 + 1];

		// A*x=b, x - �������, b - ������ �����. 
		// ���������� � � � b ���������� � �������.

		// ��������� �����������
		for (i = 0; i<n75; i++) dx[i] = x[i + 1];


		//doublereal normb = norm(M.solve(b));
		doublereal normb = 0.0;
		// ����� ����������� ��� ��� �����
		// ������ ������ ��� ��� ������������



		normb = NormaV_for_gmres(&b[1], n75);
		//normb = NormaV(buffer, n75);

		//Vector r = &b[1] - A * x;
		doublereal *r = new doublereal[n75];
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

		integer maxit = 2000;

		if ((resid = norm_r / normb) <= dterminatedTResudual) {
			//tol = resid;
			maxit = 0;
			return 0;
		}

		//integer i_1 = 0; // ������� ����� for

		doublereal** H = new doublereal*[m_restart + 2]; // Hessenberg
		for (integer i_1 = 0; i_1 < m_restart + 2; i_1++) H[i_1] = new doublereal[m_restart + 2];


		for (integer i_1 = 0; i_1 < m_restart + 2; i_1++)
		{
			for (integer j_1 = 0; j_1 < m_restart + 2; j_1++)
			{
				H[i_1][j_1] = 0.0;
			}
		}

		//Vector *v = new Vector[m_restart + 1];
		doublereal** v = new doublereal*[m_restart + 2];
		for (integer i_1 = 0; i_1 <= m_restart + 1; i_1++) v[i_1] = new doublereal[n75];


		for (integer i_1 = 0; i_1 <= m_restart + 1; i_1++) {
			for (integer j_1 = 0; j_1 < n75; j_1++)
			{
				v[i_1][j_1] = 0.0;
			}
		}

		doublereal** Z = new doublereal*[m_restart + 2];
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
						row_ptr_end, residual_fine, diag, n_a, bonly_serial,
						process_flow_beta, F_false_C_true, nu1, nu2, bILU2smoother,
						ilevel, inumberVcyclelocbicgstab, imyinit, idim_diag, milu2, milu0, nested_desection,
						R, P, nnz_aRP, flag, residual_coarse, igam, nnz_a,
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

				// ��������������� ��� ������������������.
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

				printf("%d %e \n", j, fabs(s[i + 1]) / normb);
				//printf("%d %e \n", j, beta*fabs(s[i + 1]));
				//getchar();

				resid = fabs(s[i + 1]) / normb;
				//resid = beta*fabs(s[i + 1]);

				if ((resid) < dterminatedTResudual) {
					printf("dosrochnji vjhod\n");
					//getchar();				
					Update(dx, i, n75, H, s, Z);
					//tol = resid;
					//maxit = j;

					for (integer i_1 = 0; i_1<n75; i_1++) {
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
					if (val75 != NULL) {
						delete[] val75;
						val75 = NULL;
					}
					if (col_ind75 != NULL) {
						delete[] col_ind75;
						col_ind75 = NULL;
					}
					if (row_ptr75 != NULL) {
						delete[] row_ptr75;
						row_ptr75 = NULL;
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

				printf("end\n");
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
				if (val75 != NULL) {
					delete[] val75;
					val75 = NULL;
				}
				if (col_ind75 != NULL) {
					delete[] col_ind75;
					col_ind75 = NULL;
				}
				if (row_ptr75 != NULL) {
					delete[] row_ptr75;
					row_ptr75 = NULL;
				}

				goto LABEL_FGMRES_CONTINUE;


			}
		}

		//tol = resid;
		for (integer i_1 = 0; i_1<n75; i_1++) {
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
		if (val75 != NULL) {
			delete[] val75;
			val75 = NULL;
		}
		if (col_ind75 != NULL) {
			delete[] col_ind75;
			col_ind75 = NULL;
		}
		if (row_ptr75 != NULL) {
			delete[] row_ptr75;
			row_ptr75 = NULL;
		}
		goto LABEL_FGMRES_CONTINUE;




	}

LABEL_FGMRES_CONTINUE:

	if (debug_reshime) system("pause");
	




	// �������� : ������ ��� ������� ������������ ����������.
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
		if (iVar != TOTALDEFORMATIONVAR) {
			printf("identity situation\n");
			// ���� ������� x_best_search ������ �� ���� �����������.
#pragma omp parallel for
			for (integer i47 = 1; i47 <= n_a[0]; i47++) {
				x[i47] = x_best_search2[i47];
			}
		}
	}

	// ����� � ������� �� �������� ���� �������� ������� ��������� 1.0e7.
FULL_DIVERGENCE_DETECTED:

	// ��������������� ��������� ����� ���������� �� ������.
	if (bprint_mesage_diagnostic) {
		switch (iVar) {
		case PAM: printf("PAM\n");  break;
		case VX:  printf("VX\n"); break;
		case VY:  printf("VY\n"); break;
		case VZ:  printf("VZ\n"); break;
		case TEMP:  printf("TEMP\n"); break;
		case TOTALDEFORMATIONVAR: printf("Stress system\n"); break;
		}
	}
	else {
#if doubleintprecision == 1
		//switch (iVar) {
		// ������ ilevel_VX_VY_VZ=10, ilevel_PAM=5 ��� 6.

		//case PAM: printf("PAM %lld %e %e %e %e\n", ilevel, n_a[ilevel - 4] / n_a[ilevel - 3], n_a[ilevel - 3] / n_a[ilevel-2], n_a[ilevel - 2] / n_a[ilevel - 1], n_a[ilevel - 1] / n_a[ilevel]);  break;
		//case VX:  printf("VX %lld %e %e %e %e\n", ilevel, n_a[ilevel - 4] / n_a[ilevel - 3], n_a[ilevel - 3] / n_a[ilevel - 2], n_a[ilevel - 2] / n_a[ilevel - 1], n_a[ilevel - 1] / n_a[ilevel]); break;
		//case VY:  printf("VY %lld %e %e %e %e\n", ilevel, n_a[ilevel - 4] / n_a[ilevel - 3], n_a[ilevel - 3] / n_a[ilevel - 2], n_a[ilevel - 2] / n_a[ilevel - 1], n_a[ilevel - 1] / n_a[ilevel]); break;
		//case VZ:  printf("VZ %lld %e %e %e %e\n", ilevel, n_a[ilevel - 4] / n_a[ilevel - 3], n_a[ilevel - 3] / n_a[ilevel - 2], n_a[ilevel - 2] / n_a[ilevel - 1], n_a[ilevel - 1] / n_a[ilevel]); break;
		//case TEMP:  printf("TEMP %lld %e %e %e %e\n", ilevel, n_a[ilevel - 4] / n_a[ilevel - 3], n_a[ilevel - 3] / n_a[ilevel - 2], n_a[ilevel - 2] / n_a[ilevel - 1], n_a[ilevel - 1] / n_a[ilevel]); break;
		//case TOTALDEFORMATIONVAR:  printf("Stress system %lld %e %e %e %e\n", ilevel, n_a[ilevel - 4] / n_a[ilevel - 3], n_a[ilevel - 3] / n_a[ilevel - 2], n_a[ilevel - 2] / n_a[ilevel - 1], n_a[ilevel - 1] / n_a[ilevel]); break;
		//}
#else
		//switch (iVar) {
		// ������ ilevel_VX_VY_VZ=10, ilevel_PAM=5 ��� 6.

		//case PAM: printf("PAM %d %e %e %e %e\n", ilevel, n_a[ilevel - 4] / n_a[ilevel - 3], n_a[ilevel - 3] / n_a[ilevel-2], n_a[ilevel - 2] / n_a[ilevel - 1], n_a[ilevel - 1] / n_a[ilevel]);  break;
		//case VX:  printf("VX %d %e %e %e %e\n", ilevel, n_a[ilevel - 4] / n_a[ilevel - 3], n_a[ilevel - 3] / n_a[ilevel - 2], n_a[ilevel - 2] / n_a[ilevel - 1], n_a[ilevel - 1] / n_a[ilevel]); break;
		//case VY:  printf("VY %d %e %e %e %e\n", ilevel, n_a[ilevel - 4] / n_a[ilevel - 3], n_a[ilevel - 3] / n_a[ilevel - 2], n_a[ilevel - 2] / n_a[ilevel - 1], n_a[ilevel - 1] / n_a[ilevel]); break;
		//case VZ:  printf("VZ %d %e %e %e %e\n", ilevel, n_a[ilevel - 4] / n_a[ilevel - 3], n_a[ilevel - 3] / n_a[ilevel - 2], n_a[ilevel - 2] / n_a[ilevel - 1], n_a[ilevel - 1] / n_a[ilevel]); break;
		//case TEMP:  printf("TEMP %d %e %e %e %e\n", ilevel, n_a[ilevel - 4] / n_a[ilevel - 3], n_a[ilevel - 3] / n_a[ilevel - 2], n_a[ilevel - 2] / n_a[ilevel - 1], n_a[ilevel - 1] / n_a[ilevel]); break;
		//case TOTALDEFORMATIONVAR:  printf("Stress system %d %e %e %e %e\n", ilevel, n_a[ilevel - 4] / n_a[ilevel - 3], n_a[ilevel - 3] / n_a[ilevel - 2], n_a[ilevel - 2] / n_a[ilevel - 1], n_a[ilevel - 1] / n_a[ilevel]); break;
		//}
#endif


#if doubleintprecision == 1
		switch (iVar) {
			// ������ ilevel_VX_VY_VZ=10, ilevel_PAM=5 ��� 6.

		case PAM: printf("PAM level=%lld  CopA=%1.2f CopP=%1.2f nV=%lld res0=%e %lld %lld %lld\n", ilevel, dr_grid_complexity, (doublereal)(nnz_P_memo_all / n_a[0]), icount_V_cycle, dres_initial, n_a[ilevel - 2], n_a[ilevel - 1], n_a[ilevel]);  break;
		case VX:  printf("VX level=%lld CopA=%1.2f CopP=%1.2f nV=%lld res0=%e %lld %lld %lld\n", ilevel, dr_grid_complexity, (doublereal)(nnz_P_memo_all / n_a[0]), icount_V_cycle, dres_initial, n_a[ilevel - 2], n_a[ilevel - 1], n_a[ilevel]); break;
		case VY:  printf("VY level=%lld CopA=%1.2f CopP=%1.2f nV=%lld res0=%e %lld %lld %lld\n", ilevel, dr_grid_complexity, (doublereal)(nnz_P_memo_all / n_a[0]), icount_V_cycle, dres_initial, n_a[ilevel - 2], n_a[ilevel - 1], n_a[ilevel]); break;
		case VZ:  printf("VZ level=%lld CopA=%1.2f CopP=%1.2f nV=%lld res0=%e %lld %lld %lld\n", ilevel, dr_grid_complexity, (doublereal)(nnz_P_memo_all / n_a[0]), icount_V_cycle, dres_initial, n_a[ilevel - 2], n_a[ilevel - 1], n_a[ilevel]); break;
		case TEMP:  printf("TEMP level=%lld CopA=%1.2f CopP=%1.2f nV=%lld res0=%e %lld %lld %lld\n", ilevel, dr_grid_complexity, (doublereal)(nnz_P_memo_all / n_a[0]), icount_V_cycle, dres_initial, n_a[ilevel - 2], n_a[ilevel - 1], n_a[ilevel]); break;
		case TOTALDEFORMATIONVAR:  printf("Stress system level=%lld CopA=%1.2f CopP=%1.2f nV=%lld res0=%e %lld %lld %lld\n", ilevel, dr_grid_complexity, (doublereal)(nnz_P_memo_all / n_a[0]), icount_V_cycle, dres_initial, n_a[ilevel - 2], n_a[ilevel - 1], n_a[ilevel]); break;
		}
#else
		switch (iVar) {
			// ������ ilevel_VX_VY_VZ=10, ilevel_PAM=5 ��� 6.

		case PAM: printf("PAM level=%d  CopA=%1.2f CopP=%1.2f nV=%d res0=%e %d %d %d\n", ilevel, dr_grid_complexity, (doublereal)(nnz_P_memo_all / n_a[0]), icount_V_cycle, dres_initial, n_a[ilevel - 2], n_a[ilevel - 1], n_a[ilevel]);  break;
		case VX:  printf("VX level=%d CopA=%1.2f CopP=%1.2f nV=%d res0=%e %d %d %d\n", ilevel, dr_grid_complexity, (doublereal)(nnz_P_memo_all / n_a[0]), icount_V_cycle, dres_initial, n_a[ilevel - 2], n_a[ilevel - 1], n_a[ilevel]); break;
		case VY:  printf("VY level=%d CopA=%1.2f CopP=%1.2f nV=%d res0=%e %d %d %d\n", ilevel, dr_grid_complexity, (doublereal)(nnz_P_memo_all / n_a[0]), icount_V_cycle, dres_initial, n_a[ilevel - 2], n_a[ilevel - 1], n_a[ilevel]); break;
		case VZ:  printf("VZ level=%d CopA=%1.2f CopP=%1.2f nV=%d res0=%e %d %d %d\n", ilevel, dr_grid_complexity, (doublereal)(nnz_P_memo_all / n_a[0]), icount_V_cycle, dres_initial, n_a[ilevel - 2], n_a[ilevel - 1], n_a[ilevel]); break;
		case TEMP:  printf("TEMP level=%d CopA=%1.2f CopP=%1.2f nV=%d res0=%e %d %d %d\n", ilevel, dr_grid_complexity, (doublereal)(nnz_P_memo_all / n_a[0]), icount_V_cycle, dres_initial, n_a[ilevel - 2], n_a[ilevel - 1], n_a[ilevel]); break;
		case TOTALDEFORMATIONVAR:  printf("Stress system  level=%d CopA=%1.2f CopP=%1.2f nV=%d res0=%e %d %d %d\n", ilevel, dr_grid_complexity, (doublereal)(nnz_P_memo_all / n_a[0]), icount_V_cycle, dres_initial, n_a[ilevel - 2], n_a[ilevel - 1], n_a[ilevel]); break;
		}
#endif

	}
	

	// free
	if (x_best_search2 != NULL) {
		delete[] x_best_search2;
		x_best_search2 = NULL;
	}
	if (x_best_search_init != NULL) {
		delete[] x_best_search_init;
		x_best_search_init = NULL;
	}


	// free	
	if (bnested_desection_global_amg != NULL) {
		free(bnested_desection_global_amg);  // ���������� ������.
		bnested_desection_global_amg = NULL;
	}
	for (integer i_scan_levels = 0; i_scan_levels <= maxlevel - 1; i_scan_levels++) {
		if (ilevel + 1 > i_scan_levels) {
			// free
			if (i_scan_levels <= maxlevel - 1) {
				if (diag[i_scan_levels] != NULL) {
					free(diag[i_scan_levels]);
					diag[i_scan_levels] = NULL;
				}
				if (nested_desection[i_scan_levels] != NULL) {
					free(nested_desection[i_scan_levels]);
					nested_desection[i_scan_levels] = NULL;
				}
				integer i_scan_levels_prev = i_scan_levels - 1;
				if (i_scan_levels_prev >= 0) {
					if (error_approx_fine[i_scan_levels_prev] != NULL) {
						free(error_approx_fine[i_scan_levels_prev]);
						error_approx_fine[i_scan_levels_prev] = NULL;
					}
					if (error_approx_coarse[i_scan_levels_prev] != NULL) {
						free(error_approx_coarse[i_scan_levels_prev]);
						error_approx_coarse[i_scan_levels_prev] = NULL;
					}
					if (residual_coarse[i_scan_levels_prev] != NULL) {
						free(residual_coarse[i_scan_levels_prev]);
						residual_coarse[i_scan_levels_prev] = NULL;
					}
				}
				if (residual_fine[i_scan_levels] != NULL) {
					free(residual_fine[i_scan_levels]);
					residual_fine[i_scan_levels] = NULL;
				}
			}
		}
	}


	// ����� ����������.
	my_amg_manager.icoarseningtype = memo_icoarseningtype;


	if (F_false_C_true != NULL) {
		free(F_false_C_true);
		F_false_C_true = NULL;
	}

	if (diag != NULL) {
		delete[] diag;
		diag = NULL;
	}
	if (nested_desection != NULL) {
		delete[] nested_desection;
		nested_desection = NULL;
	}

	//#ifdef	_NONAME_STUB29_10_2017
#ifdef _OPENMP
	// ������������ ��� ��������� ��������� ����������� ������.
	// ��������!!!
	for (integer i_9 = 0; i_9 < iKnumber_thread; i_9++) {
		free(vector_sum_m[i_9]);
		free(index_visit_m[i_9]);
		free(hash_table_m[i_9]);
	}
	delete[] vector_sum_m;
	delete[] index_visit_m;
	delete[] hash_table_m;
	delete[] index_size_m;
	vector_sum_m = NULL;
	index_visit_m = NULL;
	hash_table_m = NULL;
	index_size_m = NULL;

	for (integer i_9 = 0; i_9 < iKnumber_thread; i_9++) {
		delete[] AccumulqtorA_m[i_9];
	}
	delete[] AccumulqtorA_m;
	AccumulqtorA_m = NULL;
	delete[] istartAnew_m;
	istartAnew_m = NULL;
#endif

	delete[] n_a;
	delete[] nnz_a;

	// ������������ ����������� ������.
	free_level_additional_data(milu0, ilevel);
	free_level_additional_data(milu2, ilevel);

	// ������������ ����� ������ � ILU �������.
	if (milu_gl_buffer.alu_copy != NULL) delete[] milu_gl_buffer.alu_copy;
	if (milu_gl_buffer.jlu_copy != NULL) delete[] milu_gl_buffer.jlu_copy;
	if (milu_gl_buffer.ju_copy != NULL) delete[] milu_gl_buffer.ju_copy;
	milu_gl_buffer.alu_copy = NULL;
	milu_gl_buffer.jlu_copy = NULL;
	milu_gl_buffer.ju_copy = NULL;

	
	if (residual_fine[0] != NULL) {
		free(residual_fine[0]);
		residual_fine[0] = NULL;
	}

	
	if (residual_fine != NULL) {
		delete[] residual_fine;
		residual_fine = NULL;
	}

	
	if (error_approx_fine != NULL) {
		delete[] error_approx_fine;
		error_approx_fine = NULL;
	}

	
	if (error_approx_coarse != NULL) {
		delete[] error_approx_coarse;
		error_approx_coarse = NULL;
	}

	
	if (residual_coarse != NULL) {
		delete[] residual_coarse;
		residual_coarse = NULL;
	}

	
	if (row_ptr_start != NULL) {
		free(row_ptr_start);
		row_ptr_start = NULL;
	}
	if (row_ptr_end != NULL) {
		free(row_ptr_end);
		row_ptr_end = NULL;
	}	
	if (flag_shadow != NULL) {
		free(flag_shadow);
		flag_shadow = NULL;
	}	
	if (flag != NULL) {
		free(flag);
		flag = NULL;
	}
	if (x_copy != NULL) {
		free(x_copy);
		x_copy = NULL;
	}
	if (x_old != NULL) {
		free(x_old);
		x_old = NULL;
	}
	if (x_best_search != NULL) {
		free(x_best_search);
		x_best_search = NULL;
	}

	// ��� ������������:

	if (row_ptr_start != NULL) {
		free(row_ptr_start);
		row_ptr_start = NULL;
	}
	if (row_ptr_end != NULL) {
		free(row_ptr_end);
		row_ptr_end = NULL;
	}


	if (x_jacoby_buffer != NULL) {
		delete[] x_jacoby_buffer;
		x_jacoby_buffer = NULL;
	}

	
	free_hash_table_Gus_struct01();
	

	// ������������ ����������� ������.
	if (threshold_quick_all != NULL) {
		free(threshold_quick_all);
		threshold_quick_all = NULL;
	}

	if (threshold_quick_only_negative != NULL) {
		free(threshold_quick_only_negative);
		threshold_quick_only_negative = NULL;
	}

	
	return ret_value;

} // classic_aglomerative_amg6