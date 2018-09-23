// ���� mysolverv0_03.c 
// ������ ���� ���������.

#ifndef MY_SOLVER_v0_03_C
#define MY_SOLVER_v0_03_C 1

// Based on Renormalization Group Theory turbulence model.
#include "my_RNG_LES.cpp"
#include "test_filtr.cpp" // �� ��� ���������� ������� ����������.
// ������ ������� ������������ ��� �������.
// ��� �������� ������� ���� ��� ��������� �������������.
void print_temp_slau(TEMPER &t) {
	FILE *fptslau; // ���� � ������� ����� ������������ ������������ ������� ����.
	errno_t err;
	// �������� ����� ��� ������ �������� ��������� ������� ����������������.
	err = fopen_s( &fptslau, "temperslau.txt", "w");
	if (err == 0) {
		if (fptslau != NULL) {
#if doubleintprecision == 1
			fprintf(fptslau, "SLAU maxelm=%lld, maxbound=%lld, maxelm+maxbound=%lld\n", t.maxelm, t.maxbound, t.maxelm + t.maxbound);

			for (integer i = 0; i < t.maxelm; i++) {
				fprintf(fptslau, "id=%lld ap=%e ae=%e aw=%e an=%e as=%e at=%e ab=%e b=%e\n", i, t.slau[i].ap, t.slau[i].ae, t.slau[i].aw, t.slau[i].an, t.slau[i].as, t.slau[i].at, t.slau[i].ab, t.slau[i].b);
		    }

			fprintf(fptslau, "BOUNDARY CONDITION...\n");

			for (integer i = 0; i < t.maxbound; i++) {
				fprintf(fptslau, "id=%lld aw=%e ai=%e b=%e\n", i + t.maxelm, t.slau_bon[i].aw, t.slau_bon[i].ai, t.slau_bon[i].b);
			}
#else
			fprintf(fptslau, "SLAU maxelm=%d, maxbound=%d, maxelm+maxbound=%d\n", t.maxelm, t.maxbound, t.maxelm + t.maxbound);

			for (integer i = 0; i < t.maxelm; i++) {
				fprintf(fptslau, "id=%d ap=%e ae=%e aw=%e an=%e as=%e at=%e ab=%e b=%e\n", i, t.slau[i].ap, t.slau[i].ae, t.slau[i].aw, t.slau[i].an, t.slau[i].as, t.slau[i].at, t.slau[i].ab, t.slau[i].b);
			}

			fprintf(fptslau, "BOUNDARY CONDITION...\n");

			for (integer i = 0; i < t.maxbound; i++) {
				fprintf(fptslau, "id=%d aw=%e ai=%e b=%e\n", i + t.maxelm, t.slau_bon[i].aw, t.slau_bon[i].ai, t.slau_bon[i].b);
			}
#endif

			

			fclose(fptslau); // �������� �����
		}
	}
	
} // print_temp_slau

typedef struct TSortElm {
	TOCHKA p;
	integer i;
} SortElm;

// ���������� �������� ����� �� �������� 1959���.
void ShellSort(SortElm* &rb, integer in) {
	integer i, j;
	SortElm x;
	integer h;

	for (h = 1; h <= in / 9; h = 3 * h + 1);
	for (; h > 0; h /= 3) {
		for (i = h; i <= in; i++) {
			j = i;
			x = rb[i];

			while (j >= h && ((x.p.x < rb[j - h].p.x)||((x.p.x == rb[j - h].p.x)&&(x.p.y < rb[j - h].p.y))||((x.p.x == rb[j - h].p.x) && (x.p.y == rb[j - h].p.y)&&(x.p.z < rb[j - h].p.z)))) {
				rb[j] = rb[j - h]; j -= h;
			}
			rb[j] = x;
		}
	}
} // ShellSort[1959]

// �������� �������� ����� �� �������� ����� � ���� ����������.
// ����� ��������� ��������� ���������� ������� � ��������������� ������.
bool BinarySearch(SortElm* &pa_global, TOCHKA &key, integer &i, integer n) {
	integer left = 0, right = n, mid;
	while (left <= right)
	{
		mid = left + (right - left) / 2;
		if ((key.x<pa_global[mid].p.x)||((key.x==pa_global[mid].p.x)&&(key.y<pa_global[mid].p.y))||((key.x == pa_global[mid].p.x)&&(key.y == pa_global[mid].p.y)&&(key.z < pa_global[mid].p.z))) right = mid - 1;
		else if ((key.x>pa_global[mid].p.x) || ((key.x == pa_global[mid].p.x) && (key.y>pa_global[mid].p.y)) || ((key.x == pa_global[mid].p.x) && (key.y == pa_global[mid].p.y) && (key.z > pa_global[mid].p.z)))  left = mid + 1;
		else {
			i = mid;
			return true;
		}
	}
	i = left;
	return false;
} // BinarySearch

// ��� ������ ������.
// ���� ��� ������.
struct node_AVL1
{
	SortElm key;
	// ������ ��������� � ������ � ������ ����.
	unsigned char height;
	node_AVL1* left;
	node_AVL1* right;
	// �����������.
	node_AVL1(SortElm k) { key = k; left = right = 0; height = 1; }
};

// �������� ����� � � ������� ���������.
// ������ ���� height
unsigned char height(node_AVL1* p)
{
	return p ? p->height : 0;
};

// ��������� balance factor ��������� ����
// �������� ������ � ���������� �����������.
integer bfactor(node_AVL1* p)
{
	return height(p->right) - height(p->left);
};

// ��������������� ���������� �������� ���� height
// ��������� ���� (��� �������, ��� �������� ����� ���� 
// � ������ � ����� �������� ����� �������� �����������).
void fixheight(node_AVL1* p)
{
	unsigned char hl = height(p->left);
	unsigned char hr = height(p->right);
	p->height = (hl > hr ? hl : hr) + 1;
};

// ������������ �����.
node_AVL1* rotateright(node_AVL1* p)
{
	// ������ ������� ������ p
	node_AVL1* q = p->left;
	p->left = q->right;
	q->right = p;
	fixheight(p);
	fixheight(q);
	return q;
};

// ����� ������� �������� ������������ ������ ������� :
node_AVL1* rotateleft(node_AVL1* q)
{
	// ����� ������� ������ q
	node_AVL1* p = q->right;
	q->right = p->left;
	p->left = q;
	fixheight(q);
	fixheight(p);
	return p;
};

// ��� ����������� ������������ �������� � �������� ������� � ���������� ���������
node_AVL1* balance(node_AVL1* p) // ������������ ���� p
{
	fixheight(p);
	if (bfactor(p) == 2)
	{
		if (bfactor(p->right) < 0)
			p->right = rotateright(p->right);
		return rotateleft(p);
	}
	if (bfactor(p) == -2)
	{
		if (bfactor(p->left) > 0)
			p->left = rotateleft(p->left);
		return rotateright(p);
	}
	return p; // ������������ �� �����
}

// ������� ������ � ������.
// ���������� ����� �������� ����� ��� ������.
node_AVL1* insert(node_AVL1* &p, SortElm k)
{
	// ������� ����� k � ������ � ������ p
	if (p == NULL) {
		node_AVL1* r1 = NULL;
		r1 = new node_AVL1(k);
		if (r1 == NULL) {
			// ������������ ������ �� ������ ������������.
			printf("Problem : not enough memory on your equipment for r1 in insert AVL mysolverv0_03...\n");
			printf("Please any key to exit...\n");
			//getchar();
			system("pause");
			exit(1);
		}
		return r1;
	}
	if ((k.p.x<p->key.p.x) || ((k.p.x == p->key.p.x) && (k.p.y<p->key.p.y)) || ((k.p.x == p->key.p.x) && (k.p.y == p->key.p.y) && (k.p.z <  p->key.p.z)))
		p->left = insert(p->left, k);
	else 
		p->right = insert(p->right, k);
	
	return balance(p);
} // insert

  // ���������� true ���� ���� ������ � ������
bool isfound(node_AVL1* p, SortElm &k)
{
	if (p == 0) return false; // ��������.
	if ((k.p.x<p->key.p.x) || ((k.p.x == p->key.p.x) && (k.p.y<p->key.p.y)) || ((k.p.x == p->key.p.x) && (k.p.y == p->key.p.y) && (k.p.z <  p->key.p.z)))
		return isfound(p->left, k);
	else if ((k.p.x>p->key.p.x) || ((k.p.x == p->key.p.x) && (k.p.y>p->key.p.y)) || ((k.p.x == p->key.p.x) && (k.p.y == p->key.p.y) && (k.p.z >  p->key.p.z)))
		return isfound(p->right, k);
	else {
		k.i = p->key.i;
		return true; // ������.
	}
}

// ������ �������� ��������� ������.
void clear_AVL(node_AVL1* p)
{
	if (p != 0) {
		clear_AVL(p->left);
		clear_AVL(p->right);
		// ������� ����.
		delete p;
		p = 0;
	}
} // clear_AVL

  // �������� ���� � ������� �������� � ����������� ������������������.
node_AVL1* findmin(node_AVL1* p)
{
	// ����� ���� � ����������� ������ � ������ p
	//if (!p) {
	return p->left ? findmin(p->left) : p;
	//}
	//else {
	// �� ����� �������� ����� ������� ���������.
	//return 0;
	//}
} // findmin

node_AVL1* findmax(node_AVL1* p)
{
	// ����� ���� � ������������ ������ � ������ p
	if (p != 0) {
		return p->right ? findmax(p->right) : p;
	}
	else {
		// �� ����� ��������� ����� ������� ���������.
		return 0;
	}
} // findmax

SortElm get_max_AVL(node_AVL1* p)
{
	// ����������� ������������� ���� � ������.
	return p->right ? get_max_AVL(p->right) : p->key;
}

node_AVL1* removemin(node_AVL1* p)
{
	// �������� ���� � ����������� ������ �� ������ p
	if (p->left == 0)
		return p->right;
	p->left = removemin(p->left);
	return balance(p);
}

// �������� ��������� �������� �� AVL ������
// � ������ ����������� ������������.
// �� ������������ �������� ����� �� �������� ��������.
node_AVL1* remove_AVL(node_AVL1* p, SortElm k)
{
	// ��������� ������� ���������� 
	// ��� ��������� �� ��� ����� �����.
	// �������� ����� k �� ������ p
	if (p == 0) return 0;
	// �������� ����� ������� ��������.
	if ((k.p.x<p->key.p.x) || ((k.p.x == p->key.p.x) && (k.p.y<p->key.p.y)) || ((k.p.x == p->key.p.x) && (k.p.y == p->key.p.y) && (k.p.z <  p->key.p.z)))
		p->left = remove_AVL(p->left, k);
	else if ((k.p.x>p->key.p.x) || ((k.p.x == p->key.p.x) && (k.p.y>p->key.p.y)) || ((k.p.x == p->key.p.x) && (k.p.y == p->key.p.y) && (k.p.z >  p->key.p.z)))
		p->right = remove_AVL(p->right, k);
	else // k==p->key
	{
		node_AVL1* q = p->left;
		node_AVL1* r = p->right;
		p->left = 0;
		p->right = 0;
		delete p;
		p = 0;
		if (r == 0) return q;
		node_AVL1* min = findmin(r);
		min->right = removemin(r);
		min->left = q;
		return balance(min);
	}

	// ��� ������ �� �������� ������ ������������.
	return balance(p);
}

// ������� ����� � � ������ ���� ����� k_search
// ��� ��� � ������ ��� ����������� ����� �_search �� �.
// ���������� ����� �������� ����� ��� ������.
node_AVL1* insert_and_modify(node_AVL1* p, SortElm k, SortElm k_search)
{
	if (isfound(p, k_search) == false) {
		//   ���� � ������ ���.
		p = insert(p, k);
		return p;
	}
	else {
		// �������� k_search
		p = remove_AVL(p, k_search); // ����������� ��������
									 //remove_AVL(p, k_search); // �������� � ������.
		p = insert(p, k);							 // ������� �.
		return p;
	}
}

// print_AVL for debug.
void print_AVL(node_AVL1* p)
{
	if (p != 0) {
		print_AVL(p->left);
		for (integer i = 0; i <= p->height; i++) {
			printf(" ");
		}
#if doubleintprecision == 1		
		printf("%e %e %e %lld\n", p->key.p.x, p->key.p.y, p->key.p.z, p->key.i);
#else
		printf("%e %e %e %d\n", p->key.p.x, p->key.p.y, p->key.p.z, p->key.i);
#endif

		print_AVL(p->right);
	}
} // print_AVL 


// ��� ������ �����.

  // ������� ����������� ������ � 3D.
  // 6 ������� 2017.
void solve_Thermal(TEMPER &t, FLOW* &fglobal, TPROP* matlist, 
	WALL* &w, integer lw, integer lu,  QuickMemVorst& m,
	bool bThermalStress, doublereal operatingtemperature) 
{



	integer maxelm_global = t.maxnod;
	integer ncell_shadow_gl = t.maxelm;
	for (integer iu_74 = 0; iu_74 < lu; iu_74++) {
		maxelm_global += my_union[iu_74].t.maxnod;
		ncell_shadow_gl += my_union[iu_74].t.maxelm;
	}

	// ��������� ����������� ������ ������ ��� ������ ���������� �� ������������ �����������.
	doublereal epsx = 1.0e+30, epsy = 1.0e+30, epsz = 1.0e+30;
	for (integer ie = 0; ie < t.maxelm; ie++) {
		doublereal hx = 0.0, hy = 0.0, hz = 0.0;
		volume3D(ie, t.nvtx, t.pa, hx, hy, hz);
		if (0.3*hx < epsx) epsx = 0.3*hx;
		if (0.3*hy < epsy) epsy = 0.3*hy;
		if (0.3*hz < epsz) epsz = 0.3*hz;
	}

	for (integer iu_74 = 0; iu_74 < lu; iu_74++) {
		for (integer ie = 0; ie < my_union[iu_74].t.maxelm; ie++) {
			doublereal hx = 0.0, hy = 0.0, hz = 0.0;
			volume3D(ie, my_union[iu_74].t.nvtx, my_union[iu_74].t.pa, hx, hy, hz);
			if (0.3*hx < epsx) epsx = 0.3*hx;
			if (0.3*hy < epsy) epsy = 0.3*hy;
			if (0.3*hz < epsz) epsz = 0.3*hz;
		}
	}

	const integer bARRAYrealesation = 1; // �� ������ ���������������� �������, �� ������� � ��������������� ������ �������� �������� ��������������.
	const integer bAVLrealesation = 2; // �� ������ ��� ������.
	// TODO ���������� 25.08.2018 �� ������ ��� ������ �� �����������.
	integer irealesation_selector = bAVLrealesation;// bARRAYrealesation;

	node_AVL1* root=0;
	SortElm* pa_global1 = NULL;
	if (irealesation_selector == bARRAYrealesation) {
		pa_global1 = new SortElm[maxelm_global];
	}
	TOCHKA* pa_global = new TOCHKA[maxelm_global];
	maxelm_global = t.maxnod;
	for (integer i = 0; i < maxelm_global; i++) {
		pa_global[i] = t.pa[i];
		if (irealesation_selector == bARRAYrealesation) {
			pa_global1[i].p = t.pa[i];
			pa_global1[i].i = i;
		}
		if (irealesation_selector == bAVLrealesation) {
			SortElm inskey;
			inskey.i = i;
			inskey.p = t.pa[i];
			root=insert(root, inskey);
		}
	}
	integer **hash_table_pa = new integer*[lu];
	for (integer iu_74 = 0; iu_74 < lu; iu_74++) {
		hash_table_pa[iu_74] = new integer[my_union[iu_74].t.maxnod];
	}

	if (irealesation_selector == bARRAYrealesation) {
		ShellSort(pa_global1, maxelm_global - 1);
	}

	for (integer iu_74 = 0; iu_74 < lu; iu_74++) {
		for (integer j = 0; j < my_union[iu_74].t.maxnod; j++) {
			bool bfound = false;
			/*
			for (integer i = 0; i < maxelm_global; i++) {
				if ((fabs(pa_global[i].x - my_union[iu_74].t.pa[j].x)<epsx)&& (fabs(pa_global[i].y - my_union[iu_74].t.pa[j].y)<epsy) && (fabs(pa_global[i].z - my_union[iu_74].t.pa[j].z)<epsz)) {
					bfound = true;
					hash_table_pa[iu_74][j] = i;
					break;
				}
			}
			*/
			integer i = -1;
			if (irealesation_selector == bARRAYrealesation) {
				bfound = BinarySearch(pa_global1, my_union[iu_74].t.pa[j], i, maxelm_global - 1);
			}
			if (irealesation_selector == bAVLrealesation) {
				SortElm searchkey;
				searchkey.i = i;// �� ��������� � ������.
				searchkey.p = my_union[iu_74].t.pa[j];
				bfound = isfound(root, searchkey);
				if (bfound) {
					hash_table_pa[iu_74][j] = searchkey.i;
				}
			}
			if (bfound) {
				if (irealesation_selector == bARRAYrealesation) {
					hash_table_pa[iu_74][j] = pa_global1[i].i;
				}
			}
			if (!bfound) {
				// ������� ����������� ������� ����������.
				// ����� ����� ��� �������
				if (irealesation_selector == bARRAYrealesation) {
					integer mid;
					for (integer i75 = i - 1; i75 < maxelm_global; i75++) {
						mid = i75;
						if (((my_union[iu_74].t.pa[j].x < pa_global1[mid].p.x) || ((my_union[iu_74].t.pa[j].x == pa_global1[mid].p.x) && (my_union[iu_74].t.pa[j].y < pa_global1[mid].p.y)) || ((my_union[iu_74].t.pa[j].x == pa_global1[mid].p.x) && (my_union[iu_74].t.pa[j].y == pa_global1[mid].p.y) && (my_union[iu_74].t.pa[j].z < pa_global1[mid].p.z)))) {
							break;
						}
					}
					// �������. 
					// ����� ������ (������������ �� ������.).
					for (integer i95 = maxelm_global; i95 > mid; i95--) {
						pa_global1[i95] = pa_global1[i95 - 1];
					}
					// ������� � ������������� �������. 
					pa_global1[mid].p = my_union[iu_74].t.pa[j];
					pa_global1[mid].i = maxelm_global;
				}
				if (irealesation_selector == bAVLrealesation) {
					SortElm inskey;
					inskey.i = maxelm_global;
					inskey.p = my_union[iu_74].t.pa[j];
					root = insert(root, inskey);
				}
				pa_global[maxelm_global] = my_union[iu_74].t.pa[j];

				hash_table_pa[iu_74][j] = maxelm_global;

				maxelm_global++;
			}
		}
	}

	if (pa_global1 != NULL) {
		delete[] pa_global1;
	}
	if (irealesation_selector == bAVLrealesation) {
		clear_AVL(root);
	}
	pa_global1 = NULL;
	printf("pa ok\n");
	//getchar();

	// ����� nvtx, pa, prop.
	// pa --- Ok.

	// ������ nvtx
	integer** nvtx_global = new integer*[8];
	for (integer i = 0; i < 8; i++) {
		nvtx_global[i] = new integer[ncell_shadow_gl];
	}
		
	// copy first
	for (integer i = 0; i < t.maxelm; i++) {
		for (integer j = 0; j < 8; j++) {
			nvtx_global[j][i] = t.nvtx[j][i];
		}
	}
	integer ic_nvtx = t.maxelm;
	//assembles.
	// ��� ������� ���������� ���� ����� ������� �������� nvtx � ����� ������������ pa.
	for (integer iu_74 = 0; iu_74 < lu; iu_74++) {
		for (integer j = 0; j < my_union[iu_74].t.maxelm; j++) {
			
			TOCHKA pa_loc;
			for (integer k = 0; k < 8; k++) {
				pa_loc = my_union[iu_74].t.pa[my_union[iu_74].t.nvtx[k][j]-1];
				bool bfound = false;
				// ������ ���� lu==1
				if (lu == 1) {
					// �� ������������ ������ ����� pa � ������� �� �� ����� ���� ���� ������ ��-�� ���������� ����� ���� ����� ��� ������,
					// ������� ������� ������� ������ � �������������� ���� ����� ����� ��-�� ���������� �����.
					/*
					for (integer i= min(t.maxnod+ my_union[iu_74].t.nvtx[k][j] - 1, maxelm_global-1); i>=0; i--) {
					    if ((fabs(pa_global[i].x - pa_loc.x) < epsx) && (fabs(pa_global[i].y - pa_loc.y) < epsy) && (fabs(pa_global[i].z - pa_loc.z) < epsz)) {
						   bfound = true;
						   nvtx_global[k][ic_nvtx] = i + 1;
						   break; // ��������� ����� ��� ������ ������� �� ��� ����.
					    }
				    }
				*/
					nvtx_global[k][ic_nvtx]=hash_table_pa[iu_74][my_union[iu_74].t.nvtx[k][j]-1]+1;
				}
				else {
					for (integer i = 0; i < maxelm_global; i++) {
						if ((fabs(pa_global[i].x - pa_loc.x) < epsx) && (fabs(pa_global[i].y - pa_loc.y) < epsy) && (fabs(pa_global[i].z - pa_loc.z) < epsz)) {
							bfound = true;
							nvtx_global[k][ic_nvtx] = i + 1;
							break;
						}
					}
				}
				

			}
			ic_nvtx++;
		}
	}

	printf("nvtx ok\n");
	//getchar();
	// nvtx - Ok.
	doublereal* lam_export = new doublereal[maxelm_global];

	// ������ prop_global
	doublereal** prop_global = new doublereal*[8];
	for (integer i = 0; i < 8; i++) {
		prop_global[i] = new doublereal[ncell_shadow_gl];
	}
	// copy cabinet
	for (integer i = 0; i < t.maxelm; i++) {
		for (integer j = 0; j < 8; j++) {
			prop_global[j][i] = t.prop[j][i];
			lam_export[i]= t.prop[LAM][i];
		}
	}
	ic_nvtx = t.maxelm;
	// copy assembles
	for (integer iu_74 = 0; iu_74 < lu; iu_74++) {
		for (integer j = 0; j < my_union[iu_74].t.maxelm; j++) {
			for (integer k = 0; k < 8; k++) {
				prop_global[k][ic_nvtx] = my_union[iu_74].t.prop[k][j];
				lam_export[ic_nvtx] = my_union[iu_74].t.prop[LAM][j];
			}
			ic_nvtx++;
		}
	}

	printf("prop ok\n");
	//getchar();
	// prop_global �����.

		printf("New temperature solver with all meshes n=%d\n", maxelm_global);

	doublereal* rthdsd = new doublereal[maxelm_global + 2]; // ������ �����.
	doublereal* temp_potent = new doublereal[maxelm_global + 2]; // �����������.
	bool* constr = new bool[maxelm_global + 2]; // ������������� �����������.

	doublereal told_iter = operatingtemperature;

	// �������������.
	for (integer i_1 = 0; i_1 < maxelm_global + 2; i_1++) {
		rthdsd[i_1] = 0.0;
		temp_potent[i_1] = operatingtemperature;
		constr[i_1] = false; // �� ��������� ��� ���� ��������.
	}
	//for (integer i_1 = 0; i_1 < t.maxelm + t.maxnod; i_1++) {
		//t.potent[i_1]= operatingtemperature;
	//}
	//for (integer iu_74 = 0; iu_74 < lu; iu_74++) {
		//for (integer i_1 = 0; i_1 < my_union[iu_74].t.maxelm + my_union[iu_74].t.maxnod; i_1++) {
			//my_union[iu_74].t.potent[i_1] = operatingtemperature;
		//}
	//}

	

	//integer ie = 0;
	//for (integer j = 0; j < 8; j++) {
	//	printf("%e %e %e\n", t.pa[t.nvtx[j][ie] - 1].x, t.pa[t.nvtx[j][ie] - 1].y, t.pa[t.nvtx[j][ie] - 1].z);
	//}
	//	getchar();
	// ���� ��������� �������.
	// �� ��������������� �������� ������� �� ���������� ���� true.
	for (integer i_1 = 0; i_1 < lw; i_1++) {

		//const doublereal eps1 = 1.0e-30;
		// pa ����������  ����.
		for (integer j_1 = 0; j_1 < maxelm_global; j_1++) {
			bool bfound = false;
			switch (w[i_1].iPlane) {
			case XY: if ((fabs(pa_global[j_1].z - w[i_1].g.zS)<epsz) && (pa_global[j_1].x<w[i_1].g.xE + epsx) && (pa_global[j_1].x>w[i_1].g.xS - epsx) && (pa_global[j_1].y>w[i_1].g.yS - epsy) && (pa_global[j_1].y<w[i_1].g.yE + epsy)) {
				bfound = true;
			}
					 break;
			case YZ:
				if ((fabs(pa_global[j_1].x - w[i_1].g.xS)<epsx) && (pa_global[j_1].z<w[i_1].g.zE + epsz) && (pa_global[j_1].z>w[i_1].g.zS - epsz) && (pa_global[j_1].y>w[i_1].g.yS - epsy) && (pa_global[j_1].y<w[i_1].g.yE + epsy)) {
					bfound = true;
				}
				break;
			case XZ:
				if ((fabs(pa_global[j_1].y - w[i_1].g.yS)<epsy) && (pa_global[j_1].z<w[i_1].g.zE + epsz) && (pa_global[j_1].z>w[i_1].g.zS - epsz) && (pa_global[j_1].x>w[i_1].g.xS - epsx) && (pa_global[j_1].x<w[i_1].g.xE + epsx)) {
					bfound = true;
				}
				break;
			}
			if (bfound) {
				// ������������� ���������.
				if (w[i_1].ifamily == 1) {
					constr[j_1] = true;
					temp_potent[j_1] = w[i_1].Tamb;
					rthdsd[j_1]= w[i_1].Tamb;
					//printf("%d Tamb=%e\n",i_1, w[i_1].Tamb);
					//getchar();
				}
			}
		}

	}
	doublereal* temp_potent_old = new doublereal[maxelm_global];
	
	for (integer j_1 = 0; j_1 < maxelm_global; j_1++) {
		temp_potent_old[j_1] = temp_potent[j_1];
	}

	// ��� �������������� �� ������ � ����.
	doublereal* lam_for_export = new doublereal[maxelm_global];
	doublereal* sum_vol = new doublereal[maxelm_global]; // ��������� �����.

	integer iprohod = 0;
	doublereal temp_max= -1.0e30;
	do {

		iprohod++;
		printf("nonlinear prohod number %d\n", iprohod);

		// � ������ �������� ��������� �� ����������� ���������� 
		// ���������� �������� �������� ��������.
		//update_power_temperature_depend(s, ls, t, t.sosedb, gtdps, ltdp, toldtimestep1);
		//for (integer iu_74 = 0; iu_74 < lu; iu_74++) {
			//update_power_temperature_depend(s, ls, my_union[iu_74].t, my_union[iu_74].t.sosedb, gtdps, ltdp, toldtimestep);
		//}
		
		update_temp_properties1(t, fglobal, b, lb, matlist, temp_potent,0,-1, lam_export, nvtx_global); // ��������� �������� ������ ����������
		for (integer iu_74 = 0; iu_74 < lu; iu_74++) {			
			integer iadd = t.maxelm;
			for (integer iu_75 = 0; iu_75 < iu_74; iu_75++) {
				iadd += my_union[iu_75].t.maxelm;
			}
			update_temp_properties1(my_union[iu_74].t, my_union[iu_74].f, b, lb, matlist, temp_potent,iadd,iu_74, lam_export, nvtx_global); // ��������� �������� ������ ����������
		}
		
		// copy cabinet
		for (integer i = 0; i < t.maxelm; i++) {
			for (integer j = 0; j < 8; j++) {
				prop_global[j][i] = t.prop[j][i];
			}
		}
		integer ic_nvtx1 = t.maxelm;
		// copy assembles
		for (integer iu_74 = 0; iu_74 < lu; iu_74++) {
			for (integer j = 0; j < my_union[iu_74].t.maxelm; j++) {
				for (integer k = 0; k < 8; k++) {
					prop_global[k][ic_nvtx1] = my_union[iu_74].t.prop[k][j];
				}
				ic_nvtx1++;
			}
		}


		for (integer i_1 = 0; i_1 < maxelm_global + 2; i_1++) {
			rthdsd[i_1] = 0.0;
		}
		for (integer i_1 = 0; i_1 < lw; i_1++) {

			//const doublereal eps1 = 1.0e-30;
			// pa ����������  ����.
			for (integer j_1 = 0; j_1 < maxelm_global; j_1++) {
				bool bfound = false;
				switch (w[i_1].iPlane) {
				case XY: if ((fabs(pa_global[j_1].z - w[i_1].g.zS) < epsz) && (pa_global[j_1].x < w[i_1].g.xE + epsx) && (pa_global[j_1].x > w[i_1].g.xS - epsx) && (pa_global[j_1].y > w[i_1].g.yS - epsy) && (pa_global[j_1].y < w[i_1].g.yE + epsy)) {
					bfound = true;
				}
						 break;
				case YZ:
					if ((fabs(pa_global[j_1].x - w[i_1].g.xS) < epsx) && (pa_global[j_1].z < w[i_1].g.zE + epsz) && (pa_global[j_1].z > w[i_1].g.zS - epsz) && (pa_global[j_1].y > w[i_1].g.yS - epsy) && (pa_global[j_1].y < w[i_1].g.yE + epsy)) {
						bfound = true;
					}
					break;
				case XZ:
					if ((fabs(pa_global[j_1].y - w[i_1].g.yS) < epsy) && (pa_global[j_1].z < w[i_1].g.zE + epsz) && (pa_global[j_1].z > w[i_1].g.zS - epsz) && (pa_global[j_1].x > w[i_1].g.xS - epsx) && (pa_global[j_1].x < w[i_1].g.xE + epsx)) {
						bfound = true;
					}
					break;
				}
				if (bfound) {
					// ������������� ���������.
					if (w[i_1].ifamily == 1) {
						//constr[j_1] = true;
						//temp_potent[j_1] = w[i_1].Tamb;
						rthdsd[j_1] = w[i_1].Tamb;
						//printf("%d Tamb=%e\n",i_1, w[i_1].Tamb);
						//getchar();
					}
				}
			}
		}

	temp_max = -1.0e30;
	integer imax, imin;
	doublereal temp_min = +1.0e30;
	for (integer i_1 = 0; i_1 < maxelm_global; i_1++) {
		if (temp_potent[i_1] > temp_max) {
			temp_max = temp_potent[i_1];
			imax = i_1;
		}
		if (temp_potent[i_1] < temp_min) {
			temp_min = temp_potent[i_1];
			imin = i_1;
		}
	}
	told_iter = temp_max;
	printf("temperature: min = %e, max=%e\n", temp_min, temp_max);

	

		doublereal** Kmatrix_local = NULL;
		Kmatrix_local = new doublereal*[8];
		for (integer i_1 = 0; i_1 < 8; i_1++) {
			Kmatrix_local[i_1] = new doublereal[8];
		}
		// �������������.
		for (integer i_1 = 0; i_1 < 8; i_1++) {
			for (integer j_1 = 0; j_1 < 8; j_1++) {
				Kmatrix_local[i_1][j_1] = 0.0;
			}
		}

		IMatrix sparseS; // ����������� ������� � ������� IMatrix.
		//if (iswitchsolveramg_vs_BiCGstab_plus_ILU6 == 1) {
		initIMatrix(&sparseS, maxelm_global);
		//	}
		SIMPLESPARSE sparseM; // ����������� �������
		initsimplesparse(sparseM, maxelm_global);


		// ������ ����.
		for (integer ie = 0; ie < ncell_shadow_gl; ie++) {
			// �������������.
			for (integer i_1 = 0; i_1 < 8; i_1++) {
				for (integer j_1 = 0; j_1 < 8; j_1++) {
					Kmatrix_local[i_1][j_1] = 0.0;
				}
			}
			// ������ ��������� ������� ��������.
			// �������������� ������ ������� Ƹ������� ��� ������������ ������. 19.05.2018.
			Thermal_ALICE_assemble(ie, nvtx_global,
				pa_global, prop_global, Kmatrix_local);

			/*
					for (integer i_1 = 0; i_1 < 8; i_1++) {
						for (integer j_1 = 0; j_1 < 8; j_1++) {
							printf("%e ",Kmatrix_local[i_1][j_1]);
						}
						printf("\n");
					}
					getchar();
					*/

			for (integer i_4 = 0; i_4 < 8; i_4++) {
				for (integer j_4 = 0; j_4 < 8; j_4++) {

					//Kmatrix_local[i_4][j_4] *= 4.0;// ����������� 0.5 � ������� ������� �����������.
				}
			}





			// ������ ������ ����� 
			// TODO.

			// �������� �� ������ ����� ����.
			bool bsecond_member_of_equation = true;
			// ���������� ��������� ������� �������� � ����������.


			if (0&&iswitchsolveramg_vs_BiCGstab_plus_ILU6 == 1) {
				// ����� ����������� ������ ����� �������. 
			elembdSparse3(ie, sparseS, nvtx_global,
				constr, rthdsd,
				Kmatrix_local, temp_potent,
				bsecond_member_of_equation);
			}
			else {

			elembdSparse4(ie, sparseM, nvtx_global,
				constr, rthdsd,
				Kmatrix_local, temp_potent,
				bsecond_member_of_equation);
			}


		}

		for (integer i_check = 0; i_check < maxelm_global; i_check++) {
			if (sparseM.root[i_check] == NULL) {
				printf("error: zero string %d \n", i_check);
				system("pause");
			}
			else {
				NONZEROELEM* p;
				p = sparseM.root[i_check];
				if (p != NULL) {
					NONZEROELEM* q = NULL;
					//printf("%e %d %d\n", p->aij, i_check, p->key);
					//getchar();

					q = p->next;
					//p->next = NULL;

					while (q != NULL) {
						p = q;
						//printf("%e %d %d\n", p->aij, i_check, p->key);
						//getchar();
						if (fabs(p->aij) < 1.0e-15) {
							if (p->key == i_check) {
								printf("%e %d %d\n", p->aij, i_check, p->key);
								getchar();
							}
						}

						//printf(" Dirichlet p-aij=%d\n",p->aij);
						//getchar();
						q = p->next;
						//p->next = NULL;
						//delete p;
						p = NULL;
						//M.n--;
					}
					//delete M.root[i];
					//M.root[i] = NULL;
					//M.n--;
				}
			}
		}


		// �������� ��������� �����.
		for (integer i_1 = 0; i_1 < t.maxelm; i_1++) {
			// ���������� �������� �������� ������������ ������:
			doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������ ������
			volume3D(i_1, nvtx_global, pa_global, dx, dy, dz);

			for (integer j = 0; j <= 7; j++) {
				if (!constr[nvtx_global[j][i_1] - 1]) {
					if (i_1 < t.maxelm) {
						rthdsd[nvtx_global[j][i_1] - 1] += 0.125*dx*dy*dz*t.Sc[i_1];
					}
				}
			}

		}
		integer ic_now = t.maxelm;
		for (integer iu_74 = 0; iu_74 < lu; iu_74++) {
			for (integer i_1 = 0; i_1 < my_union[iu_74].t.maxelm; i_1++) {
				// ���������� �������� �������� ������������ ������:
				doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������ ������
				volume3D(i_1, my_union[iu_74].t.nvtx, my_union[iu_74].t.pa, dx, dy, dz);

				for (integer j = 0; j <= 7; j++) {
					if (!constr[hash_table_pa[iu_74][my_union[iu_74].t.nvtx[j][i_1] - 1]]) {
						if (i_1 < my_union[iu_74].t.maxelm) {
							rthdsd[hash_table_pa[iu_74][my_union[iu_74].t.nvtx[j][i_1] - 1]] += 0.125*dx*dy*dz*my_union[iu_74].t.Sc[i_1];
						}
					}
				}
			}
		}

		/*
			doublereal* square = new doublereal[3 * t.maxnod];
			for (integer i_1 = 0; i_1 < 3 * t.maxnod; i_1++) {
				square[i_1] = 0.0;
			}
			// ��������� �������.
			for (integer i_1 = 0; i_1 < t.maxelm; i_1++) {
				doublereal hx = 1.0, hy = 1.0, hz = 1.0;
				volume3D(i_1, t.nvtx, t.pa, hx, hy, hz);
				for (integer j = 0; j <= 7; j++) {
					integer j_1 = t.nvtx[j][i_1] - 1;
					//X
					square[3 * j_1] += 0.25*hy*hz;
					//Y
					square[3 * j_1 + 1] += 0.25*hx*hz;
					//Z
					square[3 * j_1 + 2] += 0.25*hx*hy;
				}
			}
			*/



		for (integer i_1 = 0; i_1 < maxelm_global; i_1++) {
			//rthdsd[i_1] *= 1.0e-6;
		}




		// �������� ���� �� �������.
		// ���� ��������� ��������� ���������� E*vol*betaT*gradT ���
		// E*Square_ortho*betaT*DeltaT, DeltaT=l*gradT.
		for (integer i_1 = 0; i_1 < maxelm_global; i_1++) {
			//rthdsd[i_1] *= square[i_1]; // Newton*m!2.
		}
		//delete[] square;


		printf("matrix is assemble.\n");
		//getchar();
		// ������� ���� TODO.

		if (iswitchsolveramg_vs_BiCGstab_plus_ILU6 == 1) {
			// ������ ����� �������.
			calculateSPARSEgaussArray(&sparseS, temp_potent, rthdsd);
		}
		bool bprintmessage = true;
		integer maxiter = 20000; // !!!
								 //ICCG(TOTALDEFORMATIONVAR, sparseM, rthdsd, deformation, 3 * t.maxnod, bprintmessage, false, maxiter); //->//
								 //doublereal *val = NULL;
								 //integer *col_ind = NULL, *row_ptr = NULL;
								 //simplesparsetoCRS(sparseM, val, col_ind, row_ptr, (3 * t.maxnod)); // �������������� ������� �� ������ ������� �������� � ������.
								 //simplesparsefree(sparseM, 3 * t.maxnod);

								 // ����������� ������� ������� ������ ��� ������������������� ���� ������������.
								 //Bi_CGStabCRS((3 * t.maxnod), val, col_ind, row_ptr, rthdsd, deformation, maxiter);//->//

								 // BiCGStab + ILU6 ���������� ����.
		if (iswitchsolveramg_vs_BiCGstab_plus_ILU6 == 0) {
			Bi_CGStab_internal4(sparseM, (maxelm_global), rthdsd, temp_potent, maxiter, bprintmessage, m);
		}
		// amg1r5 ��� ���������� �� ������ ����������-���������������� ���������.
		//amg_loc_memory_Stress(sparseM, (3*t.maxnod), rthdsd, deformation, m);
		if (iswitchsolveramg_vs_BiCGstab_plus_ILU6 == 2) {
			my_agr_amg_loc_memory_Stress(sparseM, maxelm_global, rthdsd, temp_potent, m);
		}

		// ����� ����������� ������ BicgStab+ILU2.

		temp_max = -1.0e30;
		temp_min = +1.0e30;
		for (integer i_1 = 0; i_1 < maxelm_global; i_1++) {
			if (temp_potent[i_1] > temp_max) temp_max = temp_potent[i_1];
			if (temp_potent[i_1] < temp_min) temp_min = temp_potent[i_1];
		}
		printf("temperature: min = %e, max=%e\n", temp_min, temp_max);

		printf("SLAU is solve.\n");
		//getchar();

		// ������������ ����������� ������.
		if (iswitchsolveramg_vs_BiCGstab_plus_ILU6 == 1) {
			freeIMatrix(&sparseS);
		}
		//simplesparsefree(sparseM, 3 * t.maxnod);
		for (integer i_1 = 0; i_1 < 8; i_1++) {
			delete[] Kmatrix_local[i_1];
		}
		delete[] Kmatrix_local;

		for (integer j_1 = 0; j_1 < maxelm_global; j_1++) {
			// ������ ����������.
			temp_potent[j_1] = temp_potent_old[j_1] + /*0.25*/1.0*(temp_potent[j_1]- temp_potent_old[j_1]);
			temp_potent_old[j_1] = temp_potent[j_1];
		}

	} while (fabs(told_iter - temp_max)>1.0);
	delete[] temp_potent_old;
	
	/*
	// ������ ���������� ��� ������������.
	if (t.total_deformation == NULL) {
		t.total_deformation = new doublereal*[4];
		for (integer j_6 = 0; j_6 < 4; j_6++) {
			t.total_deformation[j_6] = NULL;
			if (t.total_deformation[j_6] == NULL) {
				t.total_deformation[j_6] = new doublereal[t.maxelm + t.maxbound];
			}
		}

	}
	else {
		for (integer j_6 = 0; j_6 < 4; j_6++) {
			delete[] t.total_deformation[j_6];
			t.total_deformation[j_6] = NULL;
		}
		delete[] t.total_deformation;
		t.total_deformation = NULL;

		t.total_deformation = new doublereal*[4];
		for (integer j_6 = 0; j_6 < 4; j_6++) {
			t.total_deformation[j_6] = NULL;
			if (t.total_deformation[j_6] == NULL) {
				t.total_deformation[j_6] = new doublereal[t.maxelm + t.maxbound];
			}
		}
	}
	for (integer j_6 = 0; j_6 < 4; j_6++) {
		for (integer i_1 = 0; i_1 < t.maxelm + t.maxbound; i_1++) {
			t.total_deformation[j_6][i_1] = 0.0;
		}
	}
	*/
	/*	
	if (1) {

		// ����� ��������� �������.
		doublereal min_x = 1e60;
		doublereal min_y = 1e60;
		doublereal min_z = 1e60;
		doublereal max_x = -1e60;
		doublereal max_y = -1e60;
		doublereal max_z = -1e60;

		for (integer i = 0; i < t.maxnod; i++) {
			if (t.pa[i].x < min_x) {
				min_x = t.pa[i].x;
			}
			if (t.pa[i].y < min_y) {
				min_y = t.pa[i].y;
			}
			if (t.pa[i].z < min_z) {
				min_z = t.pa[i].z;
			}
			if (t.pa[i].x > max_x) {
				max_x = t.pa[i].x;
			}
			if (t.pa[i].y > max_y) {
				max_y = t.pa[i].y;
			}
			if (t.pa[i].z > max_z) {
				max_z = t.pa[i].z;
			}
		}

		//min_x *= 1.2;
		//min_y *= 1.2;
		//min_z *= 1.2;



		min_x = 1.05*fabs(max_x - min_x);
		if (min_x < 1.0e-30) {
			min_x = 1.05*fabs(max_x);
		}
		min_y = 1.05*fabs(max_y - min_y);
		if (min_y < 1.0e-30) {
			min_y = 1.05*fabs(max_y);
		}
		min_z = 1.05*fabs(max_z - min_z);
		if (min_z < 1.0e-30) {
			min_z = 1.05*fabs(max_z);
		}

		*/
		/*
		if (min_x < 1.0e-30) {
		printf("error!!! negative min_x MNK!\n");
		printf("min_x=%e max_x=%e\n",min_x,max_x);
		}
		if (min_y < 1.0e-30) {
		printf("error!!! negative min_y MNK!\n");
		printf("min_y=%e max_y=%e\n", min_y, max_y);
		}
		if (min_z < 1.0e-30) {
		printf("error!!! negative min_z MNK!\n");
		printf("min_z=%e max_z=%e\n", min_z, max_z);
		}
		*/
	   /*
		TOCHKA** pointerlist = new TOCHKA*[t.maxelm];
		doublereal** rthdsd_Gauss = new doublereal*[t.maxelm];
		for (integer i_47 = 0; i_47 < t.maxelm; i_47++) {
			pointerlist[i_47] = new TOCHKA[8];
			rthdsd_Gauss[i_47] = new doublereal[8];
		}

		for (integer j_6 = 0; j_6 < 4; j_6++) {

			for (integer i = 0; i < t.maxelm; i++) {
				//doublereal xc47, yc47, zc47;

				TOCHKA p;
				center_cord3D(i, t.nvtx, t.pa, p, 100);
				//xc47 = p.x;
				//yc47 = p.y;
				//zc47 = p.z;


				p.x = p.x + min_x;
				p.y = p.y + min_y;
				p.z = p.z + min_z;

				for (integer j = 0; j <= 7; j++) {
					TOCHKA p1;
					p1.x = t.pa[t.nvtx[j][i] - 1].x;
					p1.y = t.pa[t.nvtx[j][i] - 1].y;
					p1.z = t.pa[t.nvtx[j][i] - 1].z;
					p1.x = p1.x + min_x;
					p1.y = p1.y + min_y;
					p1.z = p1.z + min_z;

					pointerlist[i][j] = p1;
					if (fabs(p1.x) < 1.0e-40) {
						printf("problem x=%e\n", p1.x);
						getchar();
					}
					if (fabs(p1.y) < 1.0e-40) {
						printf("problem y=%e\n", p1.y);
						getchar();
					}
					if (fabs(p1.z) < 1.0e-40) {
						printf("problem z=%e\n", p1.z);
						getchar();
					}
					integer j_1 = t.nvtx[j][i] - 1;
					switch (j_6) {
					case 0:// TOTAL DEFORMATION
						rthdsd_Gauss[i][j] = temp_potent[j_1];

						break;
					case 1: // X deformation
						rthdsd_Gauss[i][j] = temp_potent[j_1];

						break;
					case 2: // Y deformation
						rthdsd_Gauss[i][j] = temp_potent[j_1];

						break;
					case 3: // Z deformation
						rthdsd_Gauss[i][j] = temp_potent[j_1];

						break;
					}

				}


				doublereal** Xmatr = new doublereal*[4];
				for (integer j = 0; j <= 3; j++) {
					Xmatr[j] = new doublereal[4];
				}


				doublereal* bmatr = new doublereal[4];
				doublereal* koefmatr = new doublereal[4];

				for (integer j1 = 0; j1 <= 3; j1++) {
					for (integer j2 = 0; j2 <= 3; j2++) {
						Xmatr[j1][j2] = 0.0;
					}
					bmatr[j1] = 0.0;
					koefmatr[j1] = 0.0;
				}




				for (integer j = 0; j < 8; j++) {

					Xmatr[0][0] += 1.0;
					Xmatr[0][1] += pointerlist[i][j].x;
					Xmatr[0][2] += pointerlist[i][j].y;
					Xmatr[0][3] += pointerlist[i][j].z;

					Xmatr[1][0] += pointerlist[i][j].x;
					Xmatr[1][1] += pointerlist[i][j].x*pointerlist[i][j].x;
					Xmatr[1][2] += pointerlist[i][j].x*pointerlist[i][j].y;
					Xmatr[1][3] += pointerlist[i][j].x*pointerlist[i][j].z;

					Xmatr[2][0] += pointerlist[i][j].y;
					Xmatr[2][1] += pointerlist[i][j].y*pointerlist[i][j].x;
					Xmatr[2][2] += pointerlist[i][j].y*pointerlist[i][j].y;
					Xmatr[2][3] += pointerlist[i][j].y*pointerlist[i][j].z;

					Xmatr[3][0] += pointerlist[i][j].z;
					Xmatr[3][1] += pointerlist[i][j].z*pointerlist[i][j].x;
					Xmatr[3][2] += pointerlist[i][j].z*pointerlist[i][j].y;
					Xmatr[3][3] += pointerlist[i][j].z*pointerlist[i][j].z;

					bmatr[0] += rthdsd_Gauss[i][j];
					bmatr[1] += pointerlist[i][j].x*rthdsd_Gauss[i][j];
					bmatr[2] += pointerlist[i][j].y*rthdsd_Gauss[i][j];
					bmatr[3] += pointerlist[i][j].z*rthdsd_Gauss[i][j];
				}


				for (integer j1 = 0; j1 <= 100; j1++) {
					koefmatr[0] = (bmatr[0] - Xmatr[0][1] * koefmatr[1] - Xmatr[0][2] * koefmatr[2] - Xmatr[0][3] * koefmatr[3]) / Xmatr[0][0];
					koefmatr[1] = (bmatr[1] - Xmatr[1][0] * koefmatr[0] - Xmatr[1][2] * koefmatr[2] - Xmatr[1][3] * koefmatr[3]) / Xmatr[1][1];
					koefmatr[2] = (bmatr[2] - Xmatr[2][0] * koefmatr[0] - Xmatr[2][1] * koefmatr[1] - Xmatr[2][3] * koefmatr[3]) / Xmatr[2][2];
					koefmatr[3] = (bmatr[3] - Xmatr[3][0] * koefmatr[0] - Xmatr[3][1] * koefmatr[1] - Xmatr[3][2] * koefmatr[2]) / Xmatr[3][3];
				}
				//t.total_deformation[j_6][i] = koefmatr[0] + koefmatr[1] * (p.x) + koefmatr[2] * (p.y) + koefmatr[3] * (p.z);
				// �����������.
				t.potent[i]= koefmatr[0] + koefmatr[1] * (p.x) + koefmatr[2] * (p.y) + koefmatr[3] * (p.z);


				for (integer j = 0; j <= 3; j++) {
					delete[] Xmatr[j];
				}
				delete[] Xmatr;
				delete[] bmatr;
				delete[] koefmatr;

			}
		} // j_6

		for (integer i = 0; i < t.maxelm; i++) {
			delete[] pointerlist[i];
			delete[] rthdsd_Gauss[i];
		}
		delete[] pointerlist;
		delete[] rthdsd_Gauss;

	}
	*/
	// ���������� ����������.
	// TODO.

	// ��� �������������� �� ������ � ����.
    //lam_for_export;
    //sum_vol; // ��������� �����.
    for (integer i_72 = 0; i_72 < maxelm_global; i_72++) {
	     // �������������.
	     lam_for_export[i_72] = 0.0;
	     sum_vol[i_72] = 0.0;
    }
	for (integer i_72 = 0; i_72 < ncell_shadow_gl; i_72++) {
		doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
		volume3D(i_72, nvtx_global, pa_global, hx, hy, hz);
		for (integer i_73 = 0; i_73 < 8; i_73++) {
			lam_for_export[nvtx_global[i_73][i_72] - 1] += hx*hy*hz*lam_export[i_72];
			sum_vol[nvtx_global[i_73][i_72] - 1] += hx*hy*hz;
		}
	}
	for (integer i_72 = 0; i_72 < maxelm_global; i_72++) {
		// �������������.
		lam_for_export[i_72] = lam_for_export[i_72]/ sum_vol[i_72];
	}

	// ���������� �������� �������.
	doublereal* Tx = new doublereal[ncell_shadow_gl];
	doublereal* Ty = new doublereal[ncell_shadow_gl];
	doublereal* Tz = new doublereal[ncell_shadow_gl];
	doublereal* HeatFluxMag = new doublereal[ncell_shadow_gl];

	for (integer i_72 = 0; i_72 < ncell_shadow_gl; i_72++) {
		doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
		volume3D(i_72, nvtx_global, pa_global, hx, hy, hz);
		Tx[i_72] = -lam_export[i_72] * (temp_potent[nvtx_global[1][i_72] - 1] - temp_potent[nvtx_global[0][i_72] - 1]) / hx;
		Ty[i_72] = -lam_export[i_72] * (temp_potent[nvtx_global[3][i_72] - 1] - temp_potent[nvtx_global[0][i_72] - 1]) / hy;
		Tz[i_72] = -lam_export[i_72] * (temp_potent[nvtx_global[4][i_72] - 1] - temp_potent[nvtx_global[0][i_72] - 1]) / hz;
		HeatFluxMag[i_72] = sqrt(Tx[i_72]* Tx[i_72]+ Ty[i_72] * Ty[i_72] + Tz[i_72] * Tz[i_72]);
	}

	doublereal* Txgl = new doublereal[maxelm_global];
	doublereal* Tygl = new doublereal[maxelm_global];
	doublereal* Tzgl = new doublereal[maxelm_global];
	doublereal* HeatFluxMaggl = new doublereal[maxelm_global];

	for (integer i_72 = 0; i_72 < maxelm_global; i_72++) {
		// �������������.
		Txgl[i_72] = 0.0;
		Tygl[i_72] = 0.0;
		Tzgl[i_72] = 0.0;
		HeatFluxMaggl[i_72] = 0.0;
		sum_vol[i_72] = 0.0;
	}
	for (integer i_72 = 0; i_72 < ncell_shadow_gl; i_72++) {
		doublereal hx = 1.0, hy = 1.0, hz = 1.0; // ������� ������
		volume3D(i_72, nvtx_global, pa_global, hx, hy, hz);
		for (integer i_73 = 0; i_73 < 8; i_73++) {
			Txgl[nvtx_global[i_73][i_72] - 1] += hx*hy*hz*Tx[i_72];
			Tygl[nvtx_global[i_73][i_72] - 1] += hx*hy*hz*Ty[i_72];
			Tzgl[nvtx_global[i_73][i_72] - 1] += hx*hy*hz*Tz[i_72];
			HeatFluxMaggl[nvtx_global[i_73][i_72] - 1] += hx*hy*hz*HeatFluxMag[i_72];
			sum_vol[nvtx_global[i_73][i_72] - 1] += hx*hy*hz;
		}
	}
	for (integer i_72 = 0; i_72 < maxelm_global; i_72++) {
		// �������������.
		Txgl[i_72] = Txgl[i_72] / sum_vol[i_72];
		Tygl[i_72] = Tygl[i_72] / sum_vol[i_72];
		Tzgl[i_72] = Tzgl[i_72] / sum_vol[i_72];
		HeatFluxMaggl[i_72] = HeatFluxMaggl[i_72] / sum_vol[i_72];
	}

// ������ ��� ������������.
// ncell_shadow_gl - ���������� ����� - �������� ��������� (������ ������������� �����).
// maxelm_global - ���������� ����� �����.
    export_tecplot_temperature_ass(nvtx_global, pa_global, temp_potent, lam_for_export, Txgl, Tygl, Tzgl, HeatFluxMaggl, maxelm_global, ncell_shadow_gl);

	printf("temperature is writing.\n");
	//getchar();

	// ������������ ����������� ������.
	delete[] Txgl;
	delete[] Tygl;
	delete[] Tzgl;
	delete[] HeatFluxMaggl;
	delete[] Tx;
	delete[] Ty;
	delete[] Tz;
	delete[] HeatFluxMag;
	delete[] lam_for_export;
	delete[] sum_vol;
	delete[] lam_export;
	delete[] rthdsd;
	delete[] temp_potent;
	delete[] constr;

	

	// ������������ ������ �� ��� �������� ������ ����������� ���������.
	delete[] pa_global;
	for (integer i = 0; i<8; i++) { // -8N
		if (nvtx_global != NULL) {
			if (nvtx_global[i] != NULL) {
				delete[] nvtx_global[i];
				nvtx_global[i] = NULL;
			}
		}
		
	}
	if (nvtx_global != NULL) {
		delete[] nvtx_global;
		nvtx_global = NULL;
	}
	if (prop_global != NULL) {
		for (integer i = 0; i<8; i++) {
			if (prop_global[i] != NULL) {
				delete[] prop_global[i]; // -3N
			}
		}
	}
	if (prop_global != NULL) {
		delete[] prop_global;
		prop_global = NULL;
	}
} // solve_Thermal

// ������� ����������� ������ � 3D.
// 6 ������� 2017.
void solve_Structural(TEMPER &t, WALL* &w, integer lw, QuickMemVorst& m, bool bThermalStress, doublereal operatingtemperature) {

	printf("Stress n=%d\n", 3 * t.maxnod);

	doublereal* rthdsd = new doublereal[3*t.maxnod + 2]; // ������ �����.
	doublereal* deformation = new doublereal[3*t.maxnod + 2]; // ����������.
	bool* constr = new bool[3 * t.maxnod + 2]; // ������������� ��������.
	CylindricalSupport* cylsup = new CylindricalSupport[3 * t.maxnod + 2];

	// �������������.
	for (integer i_1 = 0; i_1 < 3 * t.maxnod + 2; i_1++) {
		rthdsd[i_1] = 0.0;
		deformation[i_1] = 0.0;
		constr[i_1] = false; // �� ��������� ��� ���� ��������.
		cylsup[i_1].bactive = false; // �� ������������.
	}

	doublereal epsx = 1.0e+30, epsy = 1.0e+30, epsz = 1.0e+30;
	for (integer ie = 0; ie < t.maxelm; ie++) {
		doublereal hx = 0.0, hy = 0.0, hz = 0.0;
		volume3D(ie, t.nvtx, t.pa, hx, hy, hz);
		if (0.3*hx < epsx) epsx = 0.3*hx;
		if (0.3*hy < epsy) epsy = 0.3*hy;
		if (0.3*hz < epsz) epsz = 0.3*hz;
	}

	//integer ie = 0;
	//for (integer j = 0; j < 8; j++) {
	//	printf("%e %e %e\n", t.pa[t.nvtx[j][ie] - 1].x, t.pa[t.nvtx[j][ie] - 1].y, t.pa[t.nvtx[j][ie] - 1].z);
	//}
//	getchar();
	// ���� ��������� �������.
	// �� ��������������� �������� ������� �� ���������� ���� true.
	for (integer i_1 = 0; i_1 < lw; i_1++) {
		
			//const doublereal eps1 = 1.0e-30;
			// pa ����������  ����.
			for (integer j_1 = 0; j_1 < t.maxnod; j_1++) {
				bool bfound = false;
				switch (w[i_1].iPlane) {
				case XY: if ((fabs(t.pa[j_1].z-w[i_1].g.zS)<epsz)&&(t.pa[j_1].x<w[i_1].g.xE+epsx)&&(t.pa[j_1].x>w[i_1].g.xS - epsx)&&(t.pa[j_1].y>w[i_1].g.yS - epsy)&&(t.pa[j_1].y<w[i_1].g.yE + epsy)) {
					bfound = true;
				}
					break;
				case YZ :
					if ((fabs(t.pa[j_1].x - w[i_1].g.xS)<epsx) && (t.pa[j_1].z<w[i_1].g.zE + epsz) && (t.pa[j_1].z>w[i_1].g.zS - epsz) && (t.pa[j_1].y>w[i_1].g.yS - epsy) && (t.pa[j_1].y<w[i_1].g.yE + epsy)) {
						bfound = true;
					}
					break;
				case XZ :
					if ((fabs(t.pa[j_1].y - w[i_1].g.yS)<epsy) && (t.pa[j_1].z<w[i_1].g.zE + epsz) && (t.pa[j_1].z>w[i_1].g.zS - epsz) && (t.pa[j_1].x>w[i_1].g.xS - epsx) && (t.pa[j_1].x<w[i_1].g.xE + epsx)) {
						bfound = true;
					}
					break;
				}
				if (bfound) {
					// ������������� ���������.
					// Thermal-Stress boundary condition
					// 0 - free,
					// 1 - x fixit,
					// 2 - y fixit,
					// 3 - z fixit,
					// 4 - xy fixit,
					// 5 - xz fixit,
					// 6 - yz fixit,
					// 7 - fixit all,
					// 8 - x Force,
					// 9 - y Force,
					// 10 - z Force.
					switch (w[i_1].ithermal_Stress_boundary_condition) {
					case 0: //FREE all
						break;
					case 1:  constr[3 * j_1] = true;//X
						break;
					case 2:  constr[3 * j_1+1] = true;//Y
						break;
					case 3:  constr[3 * j_1+2] = true;//Z
						break;
					case 4:  constr[3 * j_1] = true;//X
						constr[3 * j_1 + 1] = true;//Y
						break;
					case 5:  constr[3 * j_1] = true;//X
						constr[3 * j_1 + 2] = true;//Z
						break;
					case 6: constr[3 * j_1 + 1] = true;//Y 
						constr[3 * j_1 + 2] = true;//Z
						break;
					case 7: 
						//printf("ok");
						//getchar();//ok
						constr[3 * j_1] = true;//X
						constr[3 * j_1 + 1] = true;//Y
						constr[3 * j_1 + 2] = true;//Z
						break;
					case 8 : case 9: case 10:
						rthdsd[3 * j_1] = w[i_1].xForce;
						rthdsd[3 * j_1 + 1] = w[i_1].yForce;
						rthdsd[3 * j_1 + 2] = w[i_1].zForce;
						//printf("Fotce X =%e %e %e\n", w[i_1].xForce, w[i_1].yForce, w[i_1].zForce);
						//getchar();//ok
						break;
					}
					
					
				}
			}
		
	}


	if (0) {
		// �������� ���� ����� ����������� ������������� �������� �� ������� ������� ���������.

		for (integer i_1 = 0; i_1 < lb; i_1++) {
			// �������� �� ������� ������� ��������.
			if (b[i_1].g.itypegeom == 1) {
				// ���������� �������������� ����� ��������.
				for (integer j_1 = 0; j_1 < t.maxnod; j_1++) {

					switch (b[i_1].g.iPlane) {
					case XY: if (sqrt((t.pa[j_1].x - b[i_1].g.xC)*(t.pa[j_1].x - b[i_1].g.xC) + (t.pa[j_1].y - b[i_1].g.yC)*(t.pa[j_1].y - b[i_1].g.yC)) < b[i_1].g.R_out_cyl + sqrt(9 * epsx*epsx + 9 * epsy*epsy)) {
						if ((t.pa[j_1].z > b[i_1].g.zC - 3 * epsz) && (t.pa[j_1].z < b[i_1].g.zC + b[i_1].g.Hcyl + 3 * epsz)) {
							constr[3 * j_1] = true;//X
							constr[3 * j_1 + 1] = true;//Y
							constr[3 * j_1 + 2] = true;//Z
						}
					}
							 break;
					case XZ:
						if (sqrt((t.pa[j_1].x - b[i_1].g.xC)*(t.pa[j_1].x - b[i_1].g.xC) + (t.pa[j_1].z - b[i_1].g.zC)*(t.pa[j_1].z - b[i_1].g.zC)) < b[i_1].g.R_out_cyl + sqrt(9 * epsx*epsx + 9 * epsz*epsz)) {
							if ((t.pa[j_1].y > b[i_1].g.yC - 3 * epsy) && (t.pa[j_1].y < b[i_1].g.yC + b[i_1].g.Hcyl + 3 * epsy)) {
								constr[3 * j_1] = true;//X
								constr[3 * j_1 + 1] = true;//Y
								constr[3 * j_1 + 2] = true;//Z
							}
						}
						break;
					case YZ:
						if (sqrt((t.pa[j_1].y - b[i_1].g.yC)*(t.pa[j_1].y - b[i_1].g.yC) + (t.pa[j_1].z - b[i_1].g.zC)*(t.pa[j_1].z - b[i_1].g.zC)) < b[i_1].g.R_out_cyl + sqrt(9 * epsz*epsz + 9 * epsy*epsy)) {
							if ((t.pa[j_1].x > b[i_1].g.xC - 3 * epsx) && (t.pa[j_1].x < b[i_1].g.xC + b[i_1].g.Hcyl + 3 * epsx)) {
								constr[3 * j_1] = true;//X
								constr[3 * j_1 + 1] = true;//Y
								constr[3 * j_1 + 2] = true;//Z
							}
						}
						break;
					}

				}
			}
		}
	}

	if (1) {
		// �������� ���� ����� ����������� ������������� �������� �� ������� ������� ���������.
		// ����� ������ ������ eps.

		for (integer i_1 = 0; i_1 < lb; i_1++) {
			// �������� �� ������� ������� ��������.
			// b[i_1].CylinderFixed - ������ ���� ����������� ������� � ���������� ������� ��� i_1 �����.
			if ((b[i_1].g.itypegeom == 1)&&(b[i_1].CylinderFixed)) {
				for (integer k_1 = 0; k_1 < t.maxelm; k_1++) {
					doublereal hx = 0.0, hy = 0.0, hz = 0.0;
					volume3D(k_1, t.nvtx, t.pa, hx, hy, hz);

					// ���������� �������������� ����� ��������.
					for (integer j_11 = 0; j_11 < 8; j_11++) {
						integer j_1 = t.nvtx[j_11][k_1] - 1;

						doublereal epsx1=0.3*hx;
						doublereal epsy1=0.3*hy;
						doublereal epsz1=0.3*hz;

						switch (b[i_1].g.iPlane) {
						case XY: if (sqrt((t.pa[j_1].x - b[i_1].g.xC)*(t.pa[j_1].x - b[i_1].g.xC) + (t.pa[j_1].y - b[i_1].g.yC)*(t.pa[j_1].y - b[i_1].g.yC)) < b[i_1].g.R_out_cyl + sqrt(epsx1*epsx1 + epsy1*epsy1)) {
							if ((t.pa[j_1].z > b[i_1].g.zC - 3 * epsz1) && (t.pa[j_1].z < b[i_1].g.zC + b[i_1].g.Hcyl + 3 * epsz1)) {
								constr[3 * j_1] = true;//X
								constr[3 * j_1 + 1] = true;//Y
								constr[3 * j_1 + 2] = true;//Z
							}
						}
								 break;
						case XZ:
							if (sqrt((t.pa[j_1].x - b[i_1].g.xC)*(t.pa[j_1].x - b[i_1].g.xC) + (t.pa[j_1].z - b[i_1].g.zC)*(t.pa[j_1].z - b[i_1].g.zC)) < b[i_1].g.R_out_cyl + sqrt(epsx1*epsx1 + epsz1*epsz1)) {
								if ((t.pa[j_1].y > b[i_1].g.yC - 3 * epsy1) && (t.pa[j_1].y < b[i_1].g.yC + b[i_1].g.Hcyl + 3 * epsy1)) {
									constr[3 * j_1] = true;//X
									constr[3 * j_1 + 1] = true;//Y
									constr[3 * j_1 + 2] = true;//Z
								}
							}
							break;
						case YZ:
							if (sqrt((t.pa[j_1].y - b[i_1].g.yC)*(t.pa[j_1].y - b[i_1].g.yC) + (t.pa[j_1].z - b[i_1].g.zC)*(t.pa[j_1].z - b[i_1].g.zC)) < b[i_1].g.R_out_cyl + sqrt(epsz1*epsz1 + epsy1*epsy1)) {
								if ((t.pa[j_1].x > b[i_1].g.xC - 3 * epsx1) && (t.pa[j_1].x < b[i_1].g.xC + b[i_1].g.Hcyl + 3 * epsx1)) {
									constr[3 * j_1] = true;//X
									constr[3 * j_1 + 1] = true;//Y
									constr[3 * j_1 + 2] = true;//Z
								}
							}
							break;
						}

					}
				}
			}
		}
	}

	if (0) {
		// �������� ���� ����� ����������� ��������� �������� ������ ������� ������ ���������.

		for (integer i_1 = 0; i_1 < lb; i_1++) {
			// �������� �� ������� ������� ��������.
			if (b[i_1].g.itypegeom == 1) {
				for (integer k_1 = 0; k_1 < t.maxelm; k_1++) {
					doublereal hx = 0.0, hy = 0.0, hz = 0.0;
					volume3D(k_1, t.nvtx, t.pa, hx, hy, hz);

					// ���������� �������������� ����� ��������.
					for (integer j_11 = 0; j_11 < 8; j_11++) {
						integer j_1 = t.nvtx[j_11][k_1] - 1;

						doublereal epsx1 = 0.3*hx;
						doublereal epsy1 = 0.3*hy;
						doublereal epsz1 = 0.3*hz;

						switch (b[i_1].g.iPlane) {
						case XY: if (sqrt((t.pa[j_1].x - b[i_1].g.xC)*(t.pa[j_1].x - b[i_1].g.xC) + (t.pa[j_1].y - b[i_1].g.yC)*(t.pa[j_1].y - b[i_1].g.yC)) < b[i_1].g.R_out_cyl + sqrt(epsx1*epsx1 + epsy1*epsy1)) {
							if ((t.pa[j_1].z > b[i_1].g.zC - 3 * epsz1) && (t.pa[j_1].z < b[i_1].g.zC + b[i_1].g.Hcyl + 3 * epsz1)) {
								constr[3 * j_1] = true;//X
								constr[3 * j_1 + 1] = true;//Y
								cylsup[3 * j_1].bactive = true;
								cylsup[3 * j_1].iPlane = b[i_1].g.iPlane;
								cylsup[3 * j_1].Radius = b[i_1].g.R_out_cyl;
								cylsup[3 * j_1].xC = b[i_1].g.xC;
								cylsup[3 * j_1].yC = b[i_1].g.yC;
								cylsup[3 * j_1].zC = b[i_1].g.zC;
								cylsup[3 * j_1].x1 = t.pa[j_1].x;
								cylsup[3 * j_1].y1 = t.pa[j_1].y;
								cylsup[3 * j_1].z1 = t.pa[j_1].z;
								cylsup[3 * j_1 + 1].bactive = true;
								cylsup[3 * j_1 + 1].iPlane = b[i_1].g.iPlane;
								cylsup[3 * j_1 + 1].Radius = b[i_1].g.R_out_cyl;
								cylsup[3 * j_1 + 1].xC = b[i_1].g.xC;
								cylsup[3 * j_1 + 1].yC = b[i_1].g.yC;
								cylsup[3 * j_1 + 1].zC = b[i_1].g.zC;
								cylsup[3 * j_1 + 1].x1 = t.pa[j_1].x;
								cylsup[3 * j_1 + 1].y1 = t.pa[j_1].y;
								cylsup[3 * j_1 + 1].z1 = t.pa[j_1].z;
								constr[3 * j_1 + 2] = true;//Z
							}
						}
								 break;
						case XZ:
							if (sqrt((t.pa[j_1].x - b[i_1].g.xC)*(t.pa[j_1].x - b[i_1].g.xC) + (t.pa[j_1].z - b[i_1].g.zC)*(t.pa[j_1].z - b[i_1].g.zC)) < b[i_1].g.R_out_cyl + sqrt(epsx1*epsx1 + epsz1*epsz1)) {
								if ((t.pa[j_1].y > b[i_1].g.yC - 3 * epsy1) && (t.pa[j_1].y < b[i_1].g.yC + b[i_1].g.Hcyl + 3 * epsy1)) {
									constr[3 * j_1] = true;//X
									cylsup[3 * j_1].bactive = true;
									cylsup[3 * j_1].iPlane = b[i_1].g.iPlane;
									cylsup[3 * j_1].Radius = b[i_1].g.R_out_cyl;
									cylsup[3 * j_1].xC = b[i_1].g.xC;
									cylsup[3 * j_1].yC = b[i_1].g.yC;
									cylsup[3 * j_1].zC = b[i_1].g.zC;
									cylsup[3 * j_1].x1 = t.pa[j_1].x;
									cylsup[3 * j_1].y1 = t.pa[j_1].y;
									cylsup[3 * j_1].z1 = t.pa[j_1].z;
									constr[3 * j_1 + 1] = true;//Y
									constr[3 * j_1 + 2] = true;//Z
									cylsup[3 * j_1 + 2].bactive = true;
									cylsup[3 * j_1 + 2].iPlane = b[i_1].g.iPlane;
									cylsup[3 * j_1 + 2].Radius = b[i_1].g.R_out_cyl;
									cylsup[3 * j_1 + 2].xC = b[i_1].g.xC;
									cylsup[3 * j_1 + 2].yC = b[i_1].g.yC;
									cylsup[3 * j_1 + 2].zC = b[i_1].g.zC;
									cylsup[3 * j_1 + 2].x1 = t.pa[j_1].x;
									cylsup[3 * j_1 + 2].y1 = t.pa[j_1].y;
									cylsup[3 * j_1 + 2].z1 = t.pa[j_1].z;
								}
							}
							break;
						case YZ:
							if (sqrt((t.pa[j_1].y - b[i_1].g.yC)*(t.pa[j_1].y - b[i_1].g.yC) + (t.pa[j_1].z - b[i_1].g.zC)*(t.pa[j_1].z - b[i_1].g.zC)) < b[i_1].g.R_out_cyl + sqrt(epsz1*epsz1 + epsy1*epsy1)) {
								if ((t.pa[j_1].x > b[i_1].g.xC - 3 * epsx1) && (t.pa[j_1].x < b[i_1].g.xC + b[i_1].g.Hcyl + 3 * epsx1)) {
									constr[3 * j_1] = true;//X
									constr[3 * j_1 + 1] = true;//Y
									cylsup[3 * j_1 + 1].bactive = true;
									cylsup[3 * j_1 + 1].iPlane = b[i_1].g.iPlane;
									cylsup[3 * j_1 + 1].Radius = b[i_1].g.R_out_cyl;
									cylsup[3 * j_1 + 1].xC = b[i_1].g.xC;
									cylsup[3 * j_1 + 1].yC = b[i_1].g.yC;
									cylsup[3 * j_1 + 1].zC = b[i_1].g.zC;
									cylsup[3 * j_1 + 1].x1 = t.pa[j_1].x;
									cylsup[3 * j_1 + 1].y1 = t.pa[j_1].y;
									cylsup[3 * j_1 + 1].z1 = t.pa[j_1].z;
									constr[3 * j_1 + 2] = true;//Z
									cylsup[3 * j_1 + 2].bactive = true;
									cylsup[3 * j_1 + 2].iPlane = b[i_1].g.iPlane;
									cylsup[3 * j_1 + 2].Radius = b[i_1].g.R_out_cyl;
									cylsup[3 * j_1 + 2].xC = b[i_1].g.xC;
									cylsup[3 * j_1 + 2].yC = b[i_1].g.yC;
									cylsup[3 * j_1 + 2].zC = b[i_1].g.zC;
									cylsup[3 * j_1 + 2].x1 = t.pa[j_1].x;
									cylsup[3 * j_1 + 2].y1 = t.pa[j_1].y;
									cylsup[3 * j_1 + 2].z1 = t.pa[j_1].z;
								}
							}
							break;
						}

					}
				}
			}
		}
	}


	doublereal** Kmatrix_local = NULL;
	Kmatrix_local = new doublereal*[24];
	for (integer i_1 = 0; i_1 < 24; i_1++) {
		Kmatrix_local[i_1]= new doublereal[24];
	}
	// �������������.
	for (integer i_1 = 0; i_1 < 24; i_1++) {
		for (integer j_1 = 0; j_1 < 24; j_1++) {
			Kmatrix_local[i_1][j_1] = 0.0;
		}
	}

	IMatrix sparseS; // ����������� ������� � ������� IMatrix.
	if (iswitchsolveramg_vs_BiCGstab_plus_ILU6 == 1) {		
		initIMatrix(&sparseS, 3 * t.maxnod);
	}
	SIMPLESPARSE sparseM; // ����������� �������
	initsimplesparse(sparseM, 3 * t.maxnod);

	
	// ������ ����.
	for (integer ie = 0; ie < t.maxelm; ie++) {
		// �������������.
		for (integer i_1 = 0; i_1 < 24; i_1++) {
			for (integer j_1 = 0; j_1 < 24; j_1++) {
				Kmatrix_local[i_1][j_1] = 0.0;
			}
		}
		// ������ ��������� ������� ��������.
		// �������������� ������ ������� Ƹ������� ��� ������������ ������. 4.08.2017.
		Thermal_Structural_assemble(ie, t.nvtx,
			t.pa, t.prop, Kmatrix_local);

		for (integer i_4 = 0; i_4 < 24; i_4++) {
			for (integer j_4 = 0; j_4 < 24; j_4++) {
				//Kmatrix_local[i_4][j_4] *= 1.0e-6;
				//Kmatrix_local[i_4][j_4] *= 4.0;// ����������� 0.5 � ������� ������� �����������.
			}
		}

		//if (ie == 3) {
			for (integer i_4 = 0; i_4 < 24; i_4++) {
				integer ipositive = 0;
				for (integer j_4 = 0; j_4 < 24; j_4++) {
					//printf("%1.2f ", Kmatrix_local[i_4][j_4]);
					//printf("%e ", Kmatrix_local[i_4][j_4]);
					if (fabs(Kmatrix_local[i_4][j_4] - Kmatrix_local[j_4][i_4]) > 1.0e-3) {
						printf("i=%d j=%d %e %e", i_4 + 1, j_4 + 1, Kmatrix_local[i_4][j_4], Kmatrix_local[j_4][i_4]);
					}
					//printf("%d \n%d \n%1.9f\n", i_4 + 1, j_4 + 1, Kmatrix_local[i_4][j_4]);
					/*
					if (Kmatrix_local[i_4][j_4] > 0.0) {
						printf("%d \n%d \n1\n",i_4+1,j_4+1);
						ipositive++;
					}
					else {
						printf("%d \n%d \n0\n",i_4+1,j_4+1);
					}
					*/
				}
				//printf("i=%d %d\n",i_4, ipositive);
				//printf("\n");
			}
			//getchar();
		//}
		

		// ������ ������ ����� 
		// TODO.

		// �������� �� ������ ����� ����.
		bool bsecond_member_of_equation = true;
		// ���������� ��������� ������� �������� � ����������.
		if (0) {
			/*
			elembdSparse2(ie, sparseS, t.nvtx,
				constr, rthdsd,
				Kmatrix_local, deformation,
				bsecond_member_of_equation);
				*/
		}
		else {
			
			//elembdSparse(ie, sparseM, t.nvtx,
				//constr, rthdsd,
				//Kmatrix_local, deformation,
				//bsecond_member_of_equation, cylsup, epsx, epsy, epsz);
				
			elembdSparse_noCylindricalSupport(ie, sparseM, t.nvtx,
				constr, rthdsd,
				Kmatrix_local, deformation,
				bsecond_member_of_equation, cylsup, epsx, epsy, epsz);

		}
	}

	for (integer i_check = 0; i_check < 3 * t.maxnod; i_check++) {
		if (sparseM.root[i_check] == NULL) {
			printf("error: zero string %d \n", i_check);
			system("pause");
		}
		else {
			NONZEROELEM* p;
			p = sparseM.root[i_check];
			if (p != NULL) {
				NONZEROELEM* q = NULL;

				q = p->next;
				//p->next = NULL;

				while (q != NULL) {
					p = q;
					if (fabs(p->aij) < 1.0e-15) {
						if (p->key == i_check) {
							printf("%e %d %d\n", p->aij, i_check, p->key);
							getchar();
						}
					}

					//printf(" Dirichlet p-aij=%d\n",p->aij);
					//getchar();
					q = p->next;
					//p->next = NULL;
					//delete p;
					p = NULL;
					//M.n--;
				}
				//delete M.root[i];
				//M.root[i] = NULL;
				//M.n--;
			}
		}
	}

	doublereal* square = new doublereal[3 * t.maxnod];
	for (integer i_1 = 0; i_1 < 3 * t.maxnod; i_1++) {
		square[i_1] = 0.0;
	}
	// ��������� �������.
	for (integer i_1 = 0; i_1 < t.maxelm; i_1++) {
		doublereal hx = 1.0, hy = 1.0, hz = 1.0;
		volume3D(i_1, t.nvtx, t.pa, hx, hy, hz);
		for (integer j = 0; j <= 7; j++) {
			integer j_1 = t.nvtx[j][i_1] - 1;
			//X
			square[3 * j_1] += 0.25*hy*hz;
			//Y
			square[3 * j_1 + 1] += 0.25*hx*hz;
			//Z
			square[3 * j_1 + 2] += 0.25*hx*hy;
		}
	}

	if (bThermalStress) {

		// ���������� ���������� �����������.
		doublereal *Tx = NULL;
		doublereal *Ty = NULL;
		doublereal *Tz = NULL;
		Tx = new doublereal[t.maxelm + t.maxbound];
		Ty = new doublereal[t.maxelm + t.maxbound];
		Tz = new doublereal[t.maxelm + t.maxbound];

		// ������������� ����.
		for (integer i = 0; i<t.maxelm + t.maxbound; i++) {
			Tx[i] = 0.0;
			Ty[i] = 0.0;
			Tz[i] = 0.0;
		}

		// ���������� ����������.
		for (integer i = 0; i<t.maxelm; i++) {
			// ������ ���������� ����.
			green_gaussTemperature(i, t.potent, t.nvtx, t.pa,
				t.sosedi, t.maxelm, false,
				t.sosedb, Tx, Ty, Tz);
		}

		for (integer i = 0; i<t.maxelm; i++) {
			// ������ ��������� ����.
			green_gaussTemperature(i, t.potent, t.nvtx, t.pa,
				t.sosedi, t.maxelm, true,
				t.sosedb, Tx, Ty, Tz);
		}


		

		// ���� ��������� ��������� ������������.
		doublereal betaT = 1.2e-5; // Structural Steel
		//doublereal betaT = 9.5e-6; // ����������� ��������� ��������� ���������� AlSiC8.
		doublereal* volume = new doublereal[t.maxnod];
		for (integer i_1 = 0; i_1 < t.maxnod; i_1++) {
			volume[i_1] = 0.0; // inicialization.
		}
		doublereal* YoungModule = new doublereal[t.maxnod];
		for (integer i_1 = 0; i_1 < t.maxnod; i_1++) {
			YoungModule[i_1] = 0.0; // inicialization.
		}
		doublereal* Tx_transform = new doublereal[t.maxnod];
		for (integer i_1 = 0; i_1 < t.maxnod; i_1++) {
			Tx_transform[i_1] = 0.0; // inicialization.
		}
		doublereal* Ty_transform = new doublereal[t.maxnod];
		for (integer i_1 = 0; i_1 < t.maxnod; i_1++) {
			Ty_transform[i_1] = 0.0; // inicialization.
		}
		doublereal* Tz_transform = new doublereal[t.maxnod];
		for (integer i_1 = 0; i_1 < t.maxnod; i_1++) {
			Tz_transform[i_1] = 0.0; // inicialization.
		}
		doublereal* T_transform = new doublereal[t.maxnod];
		for (integer i_1 = 0; i_1 < t.maxnod; i_1++) {
			T_transform[i_1] = 0.0; // inicialization.
		}

		for (integer i_1 = 0; i_1 < t.maxelm; i_1++) {
			doublereal hx = 1.0, hy = 1.0, hz = 1.0;
			volume3D(i_1, t.nvtx, t.pa, hx, hy, hz);

			doublereal mu, lambda; // ������������ ����.

			mu = t.prop[MU_LAME][i_1];
			lambda = t.prop[LAMBDA_LAME][i_1];

			for (integer j_1 = 0; j_1 <= 7; j_1++) {
				volume[t.nvtx[j_1][i_1] - 1] += 0.125*hx*hy*hz;

				YoungModule[t.nvtx[j_1][i_1] - 1] += 0.125*hx*hy*hz*((mu*(3*lambda+2*mu))/(lambda+mu));

				Tx_transform[t.nvtx[j_1][i_1] - 1] += 0.125*hx*hy*hz*Tx[i_1];

				Ty_transform[t.nvtx[j_1][i_1] - 1] += 0.125*hx*hy*hz*Ty[i_1];

				Tz_transform[t.nvtx[j_1][i_1] - 1] += 0.125*hx*hy*hz*Tz[i_1];

				T_transform[t.nvtx[j_1][i_1] - 1] += 0.125*hx*hy*hz*(t.potent[i_1]);

			}
			
		}

		for (integer i_1 = 0; i_1 < t.maxnod; i_1++) {
			YoungModule[i_1] = YoungModule[i_1] / volume[i_1];
			Tx_transform[i_1] = Tx_transform[i_1] / volume[i_1];
			Ty_transform[i_1] = Ty_transform[i_1] / volume[i_1];
			Tz_transform[i_1] = Tz_transform[i_1] / volume[i_1];
			T_transform[i_1] = T_transform[i_1] / volume[i_1];
		}
		// E*vol*betaT*gradT

		for (integer i_1 = 0; i_1 < 3 * t.maxnod + 2; i_1++) {
			if ((!constr[i_1])&&(!cylsup[i_1].bactive)) {
				// ���� ���� �� ������������.
				doublereal gradT = 0.0;
				integer inode = (integer)(i_1 / 3);

				if (i_1 % 3 == 0) {
					// X
					gradT = Tx_transform[inode];
				}
				if ((i_1 + 1) % 3 == 0) {
	               // Y
					gradT = Ty_transform[inode];
				}
				if ((i_1 + 2) % 3 == 0) {
					// Z
					gradT = Tz_transform[inode];
				}

				// ������ ���������, �.�. ���������� ����� ���� ��������� ��������������� ���� � ��������.
				// ����� ����������� ���� ��������� �������� �������� �����������.
				rthdsd[i_1] += YoungModule[inode]* volume[inode]* betaT*gradT;
				//rthdsd[i_1] += YoungModule[inode] * square[i_1] * betaT*(T_transform[inode] - operatingtemperature);
				
			}
		}


		// ������������ ����������� ������ �� ��� ���������� �����������.
		if (Tx != NULL) {
			delete[] Tx;
			Tx = NULL;
		}
		if (Ty != NULL) {
			delete[] Ty;
			Ty = NULL;
		}
		if (Tz != NULL) {
			delete[] Tz;
			Tz = NULL;
		}
		if (volume != NULL) {
			delete[] volume;
			volume = NULL;
		}
		if (YoungModule != NULL) {
			delete[] YoungModule;
			YoungModule = NULL;
		}
		if (Tx_transform != NULL) {
			delete[] Tx_transform;
			Tx_transform = NULL;
		}
		if (Ty_transform != NULL) {
			delete[] Ty_transform;
			Ty_transform = NULL;
		}
		if (Tz_transform != NULL) {
			delete[] Tz_transform;
			Tz_transform = NULL;
		}
		if (T_transform != NULL) {
			delete[] T_transform;
			T_transform = NULL;
		}

	}

	for (integer i_1 = 0; i_1 < 3 * t.maxnod; i_1++) {
		//rthdsd[i_1] *= 1.0e-6;
	}

	

	
	// �������� ���� �� �������.
	// ���� ��������� ��������� ���������� E*vol*betaT*gradT ���
	// E*Square_ortho*betaT*DeltaT, DeltaT=l*gradT.
	for (integer i_1 = 0; i_1 < 3 * t.maxnod; i_1++) {
		//rthdsd[i_1] *= square[i_1]; // Newton*m!2.
	}
	delete[] square;


	printf("matrix is assemble.\n");
	//getchar();
	// ������� ���� TODO.

	if (iswitchsolveramg_vs_BiCGstab_plus_ILU6 == 1) {
		calculateSPARSEgaussArray(&sparseS, deformation, rthdsd);
	}
	bool bprintmessage = true;
	integer maxiter = 20000; // !!!
	//ICCG(TOTALDEFORMATIONVAR, sparseM, rthdsd, deformation, 3 * t.maxnod, bprintmessage, false, maxiter); //->//
	//doublereal *val = NULL;
	//integer *col_ind = NULL, *row_ptr = NULL;
	//simplesparsetoCRS(sparseM, val, col_ind, row_ptr, (3 * t.maxnod)); // �������������� ������� �� ������ ������� �������� � ������.
	//simplesparsefree(sparseM, 3 * t.maxnod);

	// ����������� ������� ������� ������ ��� ������������������� ���� ������������.
	//Bi_CGStabCRS((3 * t.maxnod), val, col_ind, row_ptr, rthdsd, deformation, maxiter);//->//
	
	// BiCGStab + ILU6 ���������� ����.
	if (iswitchsolveramg_vs_BiCGstab_plus_ILU6 == 0) {
	Bi_CGStab_internal4(sparseM, (3 * t.maxnod), rthdsd, deformation, maxiter, bprintmessage, m);
	}
	// amg1r5 ��� ���������� �� ������ ����������-���������������� ���������.
	//amg_loc_memory_Stress(sparseM, (3*t.maxnod), rthdsd, deformation, m);
	if (iswitchsolveramg_vs_BiCGstab_plus_ILU6 == 2) {
		my_agr_amg_loc_memory_Stress(sparseM, (3 * t.maxnod), rthdsd, deformation, m);
	}

	// ����� ����������� ������ BicgStab+ILU2.

	doublereal deform_max = -1.0e30;
	doublereal deform_min = +1.0e30;
	for (integer i_1 = 0; i_1 < 3 * t.maxnod + 2; i_1++) {
		if (deformation[i_1] > deform_max) deform_max = deformation[i_1];
		if (deformation[i_1] < deform_max) deform_min = deformation[i_1];
	}
	printf("deformation: min = %e, max=%e\n", deform_min, deform_max);

	printf("SLAU is solve.\n");
	//getchar();

	// ������ ���������� ��� ������������.
	if (t.total_deformation == NULL) {
		t.total_deformation = new doublereal*[4];
		for (integer j_6 = 0; j_6 < 4; j_6++) {
			t.total_deformation[j_6] = NULL;
			if (t.total_deformation[j_6] == NULL) {
				t.total_deformation[j_6] = new doublereal[t.maxelm + t.maxbound];
			}
		}
		
	}
	else {
		for (integer j_6 = 0; j_6 < 4; j_6++) {
			delete[] t.total_deformation[j_6];
			t.total_deformation[j_6] = NULL;
		}
		delete[] t.total_deformation;
		t.total_deformation = NULL;
		
		t.total_deformation = new doublereal*[4];
		for (integer j_6 = 0; j_6 < 4; j_6++) {
			t.total_deformation[j_6] = NULL;
			if (t.total_deformation[j_6] == NULL) {
				t.total_deformation[j_6] = new doublereal[t.maxelm + t.maxbound];
			}
		}
	}
	for (integer j_6 = 0; j_6 < 4; j_6++) {
		for (integer i_1 = 0; i_1 < t.maxelm + t.maxbound; i_1++) {
			t.total_deformation[j_6][i_1] = 0.0;
		}
	}

	if (1) {

		// ����� ��������� �������.
		doublereal min_x = 1e60;
		doublereal min_y = 1e60;
		doublereal min_z = 1e60;
		doublereal max_x = -1e60;
		doublereal max_y = -1e60;
		doublereal max_z = -1e60;

		for (integer i = 0; i < t.maxnod; i++) {
			if (t.pa[i].x < min_x) {
				min_x = t.pa[i].x;
			}
			if (t.pa[i].y < min_y) {
				min_y = t.pa[i].y;
			}
			if (t.pa[i].z < min_z) {
				min_z = t.pa[i].z;
			}
			if (t.pa[i].x > max_x) {
				max_x = t.pa[i].x;
			}
			if (t.pa[i].y > max_y) {
				max_y = t.pa[i].y;
			}
			if (t.pa[i].z > max_z) {
				max_z = t.pa[i].z;
			}
		}

		//min_x *= 1.2;
		//min_y *= 1.2;
		//min_z *= 1.2;



		min_x = 1.05*fabs(max_x - min_x);
		if (min_x < 1.0e-30) {
			min_x = 1.05*fabs(max_x);
		}
		min_y = 1.05*fabs(max_y - min_y);
		if (min_y < 1.0e-30) {
			min_y = 1.05*fabs(max_y);
		}
		min_z = 1.05*fabs(max_z - min_z);
		if (min_z < 1.0e-30) {
			min_z = 1.05*fabs(max_z);
		}


		/*
		if (min_x < 1.0e-30) {
		printf("error!!! negative min_x MNK!\n");
		printf("min_x=%e max_x=%e\n",min_x,max_x);
		}
		if (min_y < 1.0e-30) {
		printf("error!!! negative min_y MNK!\n");
		printf("min_y=%e max_y=%e\n", min_y, max_y);
		}
		if (min_z < 1.0e-30) {
		printf("error!!! negative min_z MNK!\n");
		printf("min_z=%e max_z=%e\n", min_z, max_z);
		}
		*/

		TOCHKA** pointerlist = new TOCHKA*[t.maxelm];
		doublereal** rthdsd_Gauss = new doublereal*[t.maxelm];
		for (integer i_47 = 0; i_47 < t.maxelm; i_47++) {
			pointerlist[i_47] = new TOCHKA[8];
			rthdsd_Gauss[i_47] = new doublereal[8];
		}

		for (integer j_6 = 0; j_6 < 4; j_6++) {

			for (integer i = 0; i < t.maxelm; i++) {
				//doublereal xc47, yc47, zc47;

				TOCHKA p;
				center_cord3D(i, t.nvtx, t.pa, p, 100);
				//xc47 = p.x;
				//yc47 = p.y;
				//zc47 = p.z;


				p.x = p.x + min_x;
				p.y = p.y + min_y;
				p.z = p.z + min_z;

				for (integer j = 0; j <= 7; j++) {
					TOCHKA p1;
					p1.x = t.pa[t.nvtx[j][i] - 1].x;
					p1.y = t.pa[t.nvtx[j][i] - 1].y;
					p1.z = t.pa[t.nvtx[j][i] - 1].z;
					p1.x = p1.x + min_x;
					p1.y = p1.y + min_y;
					p1.z = p1.z + min_z;

					pointerlist[i][j] = p1;
					if (fabs(p1.x) < 1.0e-40) {
						printf("problem x=%e\n", p1.x);
						getchar();
					}
					if (fabs(p1.y) < 1.0e-40) {
						printf("problem y=%e\n", p1.y);
						getchar();
					}
					if (fabs(p1.z) < 1.0e-40) {
						printf("problem z=%e\n", p1.z);
						getchar();
					}
					integer j_1 = t.nvtx[j][i] - 1;
					switch (j_6) {
						case 0 :// TOTAL DEFORMATION
						rthdsd_Gauss[i][j] = sqrt(deformation[3 * j_1] * deformation[3 * j_1] + deformation[3 * j_1 + 1] * deformation[3 * j_1 + 1] + deformation[3 * j_1 + 2] * deformation[3 * j_1 + 2]);

						break;
						case 1 : // X deformation
						rthdsd_Gauss[i][j] = deformation[3 * j_1];

						break;
						case 2 : // Y deformation
						rthdsd_Gauss[i][j] = deformation[3 * j_1+1];

						break;
						case 3 : // Z deformation
						rthdsd_Gauss[i][j] = deformation[3 * j_1+2];

						break;
					}
					
				}


				doublereal** Xmatr = new doublereal*[4];
				for (integer j = 0; j <= 3; j++) {
					Xmatr[j] = new doublereal[4];
				}


				doublereal* bmatr = new doublereal[4];
				doublereal* koefmatr = new doublereal[4];

				for (integer j1 = 0; j1 <= 3; j1++) {
					for (integer j2 = 0; j2 <= 3; j2++) {
						Xmatr[j1][j2] = 0.0;
					}
					bmatr[j1] = 0.0;
					koefmatr[j1] = 0.0;
				}




				for (integer j = 0; j < 8; j++) {

					Xmatr[0][0] += 1.0;
					Xmatr[0][1] += pointerlist[i][j].x;
					Xmatr[0][2] += pointerlist[i][j].y;
					Xmatr[0][3] += pointerlist[i][j].z;

					Xmatr[1][0] += pointerlist[i][j].x;
					Xmatr[1][1] += pointerlist[i][j].x*pointerlist[i][j].x;
					Xmatr[1][2] += pointerlist[i][j].x*pointerlist[i][j].y;
					Xmatr[1][3] += pointerlist[i][j].x*pointerlist[i][j].z;

					Xmatr[2][0] += pointerlist[i][j].y;
					Xmatr[2][1] += pointerlist[i][j].y*pointerlist[i][j].x;
					Xmatr[2][2] += pointerlist[i][j].y*pointerlist[i][j].y;
					Xmatr[2][3] += pointerlist[i][j].y*pointerlist[i][j].z;

					Xmatr[3][0] += pointerlist[i][j].z;
					Xmatr[3][1] += pointerlist[i][j].z*pointerlist[i][j].x;
					Xmatr[3][2] += pointerlist[i][j].z*pointerlist[i][j].y;
					Xmatr[3][3] += pointerlist[i][j].z*pointerlist[i][j].z;

					bmatr[0] += rthdsd_Gauss[i][j];
					bmatr[1] += pointerlist[i][j].x*rthdsd_Gauss[i][j];
					bmatr[2] += pointerlist[i][j].y*rthdsd_Gauss[i][j];
					bmatr[3] += pointerlist[i][j].z*rthdsd_Gauss[i][j];
				}


				for (integer j1 = 0; j1 <= 100; j1++) {
					koefmatr[0] = (bmatr[0] - Xmatr[0][1] * koefmatr[1] - Xmatr[0][2] * koefmatr[2] - Xmatr[0][3] * koefmatr[3]) / Xmatr[0][0];
					koefmatr[1] = (bmatr[1] - Xmatr[1][0] * koefmatr[0] - Xmatr[1][2] * koefmatr[2] - Xmatr[1][3] * koefmatr[3]) / Xmatr[1][1];
					koefmatr[2] = (bmatr[2] - Xmatr[2][0] * koefmatr[0] - Xmatr[2][1] * koefmatr[1] - Xmatr[2][3] * koefmatr[3]) / Xmatr[2][2];
					koefmatr[3] = (bmatr[3] - Xmatr[3][0] * koefmatr[0] - Xmatr[3][1] * koefmatr[1] - Xmatr[3][2] * koefmatr[2]) / Xmatr[3][3];
				}
				t.total_deformation[j_6][i] = koefmatr[0] + koefmatr[1] * (p.x) + koefmatr[2] * (p.y) + koefmatr[3] * (p.z);


				for (integer j = 0; j <= 3; j++) {
					delete[] Xmatr[j];
				}
				delete[] Xmatr;
				delete[] bmatr;
				delete[] koefmatr;

			}
		} // j_6

		for (integer i = 0; i < t.maxelm; i++) {
			delete[] pointerlist[i];
			delete[] rthdsd_Gauss[i];
		}
		delete[] pointerlist;
		delete[] rthdsd_Gauss;

	}

	// ���������� ����������.
	// TODO.
	
	printf("deformation writing.\n");
	//getchar();

	// ������������ ����������� ������.
	if (iswitchsolveramg_vs_BiCGstab_plus_ILU6 == 1) {
	    freeIMatrix(&sparseS);
	}
	//simplesparsefree(sparseM, 3 * t.maxnod);
	delete[] rthdsd;
	delete[] deformation;
	delete[] constr;
	delete[] cylsup;
	for (integer i_1 = 0; i_1 < 24; i_1++) {
		delete[] Kmatrix_local[i_1];
	}
	delete[] Kmatrix_local;
} // solve_Structural


// ������ ���� ���������
// �������� ��������� ����������������.
// res - ������������ �������.
void solve(integer iVar, doublereal &res, FLOW &f,
	FLOW* &fglobal, TEMPER &t,
	doublereal** &rhie_chow,
	SOURCE* &s, WALL* &w, BLOCK* &b,
	integer ls, integer lw, integer lb,
	doublereal dbeta,
	integer flow_interior,
	bool bconvective,
	bool bfirst_start,
	doublereal* toldtimestep, // �������������� ���� ���������� � ����������� ���� �� �������,
	doublereal* told_iter, // ����������� � ���������� ��������.
	doublereal** speedoldtimestep, // �������������� ���� ��������� � ����������� ���� �� �������,
	doublereal** mfoldtimestep, // ������������ ����� ����� ����� ������������ ������ � ����������� ���������� ����, 
	doublereal tauparam,  // ������ ���� �� �������
	bool btimedep, // ������������ ��� �������������� ������.
	doublereal dgx, doublereal dgy, doublereal dgz,
	TPROP* &matlist, integer inumiter,
	bool bprintmessage, doublereal RCh,
	bool bVERYStable,
	doublereal** tau,
	doublereal** &sumanb,
	bool bmyhighorder, bool bdeltapfinish,
	doublereal poweron_multiplier_sequence,
	QuickMemVorst& m, doublereal* &rthdsd,
	doublereal &rfluentresval, 
	integer lu, UNION* &my_union)
{

	

	// QuickMemVorst& m - �������������� ������ ��� ������, ����� �������� ������ ��������� � ����������� ������.

	// btimedep ��������� �� �������� ��� ����� ������ ������ ������ ������� � ��� ����� �� ��������� �� ���� ��������� �� ���������
	// ������ �������.
	// ���� bfirst_start==true �� �� ����� ���� � ������ ��������� ��������� SIMPLE � ���� ������ ������ ���� ��������� �� �������.

	// ������ ������� ����
	integer i = 0; // ������� ����� for
	//doublereal RCh=0.1;//0.1; 1.0;

	bool bRhieChowiPAM = true;
	/*
	// ��������� ��� ������ ������� �� ������ �� �����������
	// ������� �� �� � ����������.
	// ��������� ��� ��������� �������� ���-��� �� ������ ���������
	// �� ������ �� ������� ����������.
	if (inumiter<2) {
		 bRhieChowiPAM=false;
	}
	*/
	integer i75 = 0;

	bool breversedflow = false;
	integer icell = 0; // ���������� ����� � ���������-�������������� ��������.
	integer imyscheme = UDS;

	switch (iVar) {
	case PAM:

		// ������������� ���� ����������� ������
		// ������ �������:


#ifdef _OPENMP

		if (bparallelizm_old) {

			if (inumcore == 1) {
				// serial
				// ��������� ������� ������� ����������� 
				// ������ ���������� � ������ �������
				for (i = 0; i < f.maxbound; i++) {

					breversedflow = false;
					// ���������� ��������� �������.
					// � ��������� ��� �������� ��������.
					// ������� �������� ������ ������� �������
					// ��������� �������� bool bDirichlet ����� true.
					/*
					my_elmatr_quad_PAm_bon( f.slau_bon,
					f.slau, i,
					f.maxelm,
					f.maxbound,
					f.sosedb,
					f.nvtx,
					f.bPressureFix,
					dbeta, f.pa,
					f.potent,
					f.prop, f.prop_b,
					f.alpha,
					ls, lw, w,
					true, f.sosedi,
					f.diag_coef, RCh,
					breversedflow);
					//*/

					// ��������� ������� �� ������ ����������� ����������� tau.
					// ������� �������� ������ ������� �������
					// ��������� �������� bool bDirichlet ����� true.
					//*
					my_elmatr_quad_PAm_bon3(f.slau_bon,
						f.slau, i,
						f.maxelm,
						f.maxbound,
						f.sosedb,
						f.nvtx,
						f.bPressureFix,
						dbeta, f.pa,
						f.potent,
						f.prop, f.prop_b,
						f.alpha,
						ls, lw, w,
						true,
						f.sosedi, RCh,
						breversedflow,
						tau);//*/

					if (breversedflow) icell++;
				}

			}

			if (inumcore == 2) {
				if (nd.b0.active) {

					// ������ �����
					for (integer iscan_par = nd.b0.ileft_start; iscan_par <= nd.b0.ileft_finish; iscan_par++) {
						integer iPloc = f.ifrontregulationgl[iscan_par];
						if (iPloc >= f.maxelm) {
							breversedflow = false;
							// ���������� ��������� �������.
							// � ��������� ��� �������� ��������.
							// ������� �������� ������ ������� �������
							// ��������� �������� bool bDirichlet ����� true.
							/*
							my_elmatr_quad_PAm_bon( f.slau_bon,
											 f.slau, iPloc-f.maxelm,
											 f.maxelm,
											 f.maxbound,
											 f.sosedb,
											 f.nvtx,
											 f.bPressureFix,
											 dbeta, f.pa,
											 f.potent,
											 f.prop, f.prop_b,
											 f.alpha,
											 ls, lw, w,
											 true, f.sosedi,
											 f.diag_coef, RCh,
											 breversedflow);
											 //*/

											 // ��������� ������� �� ������ ����������� ����������� tau.
											 // ������� �������� ������ ������� �������
											 // ��������� �������� bool bDirichlet ����� true.
											 //*
							my_elmatr_quad_PAm_bon3(f.slau_bon,
								f.slau, iPloc - f.maxelm,
								f.maxelm,
								f.maxbound,
								f.sosedb,
								f.nvtx,
								f.bPressureFix,
								dbeta, f.pa,
								f.potent,
								f.prop, f.prop_b,
								f.alpha,
								ls, lw, w,
								true,
								f.sosedi, RCh,
								breversedflow,
								tau);//*/

							if (breversedflow) icell++;
						}
					}
					// ������ �����
					for (integer iscan_par = nd.b0.iright_start; iscan_par <= nd.b0.iright_finish; iscan_par++) {
						integer iPloc = f.ifrontregulationgl[iscan_par];
						if (iPloc >= f.maxelm) {
							breversedflow = false;
							// ���������� ��������� �������.
							// � ��������� ��� �������� ��������.
							// ������� �������� ������ ������� �������
							// ��������� �������� bool bDirichlet ����� true.
							/*
							my_elmatr_quad_PAm_bon( f.slau_bon,
											f.slau, iPloc-f.maxelm,
											f.maxelm,
											f.maxbound,
											f.sosedb,
											f.nvtx,
											f.bPressureFix,
											dbeta, f.pa,
											f.potent,
											f.prop, f.prop_b,
											f.alpha,
											ls, lw, w,
											true, f.sosedi,
											f.diag_coef, RCh,
											breversedflow);
											//*/

											// ��������� ������� �� ������ ����������� ����������� tau.
											// ������� �������� ������ ������� �������
											// ��������� �������� bool bDirichlet ����� true.
											//*
							my_elmatr_quad_PAm_bon3(f.slau_bon,
								f.slau, iPloc - f.maxelm,
								f.maxelm,
								f.maxbound,
								f.sosedb,
								f.nvtx,
								f.bPressureFix,
								dbeta, f.pa,
								f.potent,
								f.prop, f.prop_b,
								f.alpha,
								ls, lw, w,
								true,
								f.sosedi, RCh,
								breversedflow,
								tau);//*/

							if (breversedflow) icell++;
						}
					}
					// �������� ��������� �����
					for (integer iscan_par = nd.b0.iseparate_start; iscan_par <= nd.b0.iseparate_finish; iscan_par++) {
						integer iPloc = f.ifrontregulationgl[iscan_par];
						if (iPloc >= f.maxelm) {
							breversedflow = false;
							// ���������� ��������� �������.
							// � ��������� ��� �������� ��������.
							// ������� �������� ������ ������� �������
							// ��������� �������� bool bDirichlet ����� true.
							/*
							my_elmatr_quad_PAm_bon( f.slau_bon,
											 f.slau, iPloc-f.maxelm,
											 f.maxelm,
											 f.maxbound,
											 f.sosedb,
											 f.nvtx,
											 f.bPressureFix,
											 dbeta, f.pa,
											 f.potent,
											 f.prop, f.prop_b,
											 f.alpha,
											 ls, lw, w,
											 true, f.sosedi,
											 f.diag_coef, RCh,
											 breversedflow);
											 //*/

											 // ��������� ������� �� ������ ����������� ����������� tau.
											 // ������� �������� ������ ������� �������
											 // ��������� �������� bool bDirichlet ����� true.
											 //*
							my_elmatr_quad_PAm_bon3(f.slau_bon,
								f.slau, iPloc - f.maxelm,
								f.maxelm,
								f.maxbound,
								f.sosedb,
								f.nvtx,
								f.bPressureFix,
								dbeta, f.pa,
								f.potent,
								f.prop, f.prop_b,
								f.alpha,
								ls, lw, w,
								true,
								f.sosedi, RCh,
								breversedflow,
								tau);//*/

							if (breversedflow) icell++;
						}
					}


				}
			}
		}
		else {
			// ��������� ������� ������� ����������� 
			// ������ ���������� � ������ �������
			for (i = 0; i < f.maxbound; i++) {

				breversedflow = false;
				// ���������� ��������� �������.
				// � ��������� ��� �������� ��������.
				// ������� �������� ������ ������� �������
				// ��������� �������� bool bDirichlet ����� true.
				/*
				my_elmatr_quad_PAm_bon( f.slau_bon,
				f.slau, i,
				f.maxelm,
				f.maxbound,
				f.sosedb,
				f.nvtx,
				f.bPressureFix,
				dbeta, f.pa,
				f.potent,
				f.prop, f.prop_b,
				f.alpha,
				ls, lw, w,
				true, f.sosedi,
				f.diag_coef, RCh,
				breversedflow);
				//*/

				// ��������� ������� �� ������ ����������� ����������� tau.
				// ������� �������� ������ ������� �������
				// ��������� �������� bool bDirichlet ����� true.
				//*
				my_elmatr_quad_PAm_bon3(f.slau_bon,
					f.slau, i,
					f.maxelm,
					f.maxbound,
					f.sosedb,
					f.nvtx,
					f.bPressureFix,
					dbeta, f.pa,
					f.potent,
					f.prop, f.prop_b,
					f.alpha,
					ls, lw, w,
					true,
					f.sosedi, RCh,
					breversedflow,
					tau);//*/

				if (breversedflow) icell++;
			}
		}

#else
				  // ��������� ������� ������� ����������� 
				  // ������ ���������� � ������ �������
		for (i = 0; i < f.maxbound; i++) {

			breversedflow = false;
			// ���������� ��������� �������.
			// � ��������� ��� �������� ��������.
			// ������� �������� ������ ������� �������
			// ��������� �������� bool bDirichlet ����� true.
			/*
			my_elmatr_quad_PAm_bon( f.slau_bon,
									f.slau, i,
									f.maxelm,
									f.maxbound,
									f.sosedb,
									f.nvtx,
									f.bPressureFix,
									dbeta, f.pa,
									f.potent,
									f.prop, f.prop_b,
									f.alpha,
									ls, lw, w,
									true, f.sosedi,
									f.diag_coef, RCh,
									breversedflow);
									//*/

									// ��������� ������� �� ������ ����������� ����������� tau.
									 // ������� �������� ������ ������� �������
									// ��������� �������� bool bDirichlet ����� true.
									//*
			my_elmatr_quad_PAm_bon3(f.slau_bon,
				f.slau, i,
				f.maxelm,
				f.maxbound,
				f.sosedb,
				f.nvtx,
				f.bPressureFix,
				dbeta, f.pa,
				f.potent,
				f.prop, f.prop_b,
				f.alpha,
				ls, lw, w,
				true,
				f.sosedi, RCh,
				breversedflow,
				tau);//*/

			if (breversedflow) icell++;
		}

#endif

		// printf("step Dirichlet...\n");
		// getchar();


#ifdef _OPENMP

		if (bparallelizm_old) {

			if (inumcore == 1) {
				// serial
				for (i = 0; i < f.maxbound; i++) {


					breversedflow = false;
					// ���������� ��������� �������.
					// � ��������� ��� �������� ��������.
					// ����� ���������� ���������.
					// �������� ���������� ������� �������.
					// ��������� �������� bool bDirichlet ����� false.
					/*
					my_elmatr_quad_PAm_bon( f.slau_bon,
					f.slau, i,
					f.maxelm,
					f.maxbound,
					f.sosedb,
					f.nvtx,
					f.bPressureFix,
					dbeta, f.pa,
					f.potent, f.prop,
					f.prop_b, f.alpha,
					ls, lw, w, false,
					f.sosedi,
					f.diag_coef, RCh,
					breversedflow);
					//					   */

					// ��������� ������� �� ������ ����������� ����������� tau.
					// ����� ���������� ���������.
					// �������� ���������� ������� �������.
					// ��������� �������� bool bDirichlet ����� false.
					//*
					my_elmatr_quad_PAm_bon3(f.slau_bon,
						f.slau, i,
						f.maxelm,
						f.maxbound,
						f.sosedb,
						f.nvtx,
						f.bPressureFix,
						dbeta, f.pa,
						f.potent,
						f.prop, f.prop_b,
						f.alpha,
						ls, lw, w,
						false,
						f.sosedi, RCh,
						breversedflow,
						tau);

					//						  */
					if (breversedflow) icell++;


				}
			}


			if (inumcore == 2) {
				if (nd.b0.active) {

					// ������ �����
					for (integer iscan_par = nd.b0.ileft_start; iscan_par <= nd.b0.ileft_finish; iscan_par++) {
						integer iPloc = f.ifrontregulationgl[iscan_par];
						if (iPloc >= f.maxelm) {
							breversedflow = false;
							// ���������� ��������� �������.
							// � ��������� ��� �������� ��������.
							// ����� ���������� ���������.
							// �������� ���������� ������� �������.
							// ��������� �������� bool bDirichlet ����� false.
							/*
							my_elmatr_quad_PAm_bon( f.slau_bon,
											  f.slau, iPloc-f.maxelm,
											  f.maxelm,
											  f.maxbound,
											  f.sosedb,
											  f.nvtx,
											  f.bPressureFix,
											  dbeta, f.pa,
											  f.potent, f.prop,
											  f.prop_b, f.alpha,
											  ls, lw, w, false,
											  f.sosedi,
											  f.diag_coef, RCh,
											  breversedflow);
							 //					   */

							 // ��������� ������� �� ������ ����������� ����������� tau.
							 // ����� ���������� ���������.
							 // �������� ���������� ������� �������.
							  // ��������� �������� bool bDirichlet ����� false.
							  //*
							my_elmatr_quad_PAm_bon3(f.slau_bon,
								f.slau, iPloc - f.maxelm,
								f.maxelm,
								f.maxbound,
								f.sosedb,
								f.nvtx,
								f.bPressureFix,
								dbeta, f.pa,
								f.potent,
								f.prop, f.prop_b,
								f.alpha,
								ls, lw, w,
								false,
								f.sosedi, RCh,
								breversedflow,
								tau);

							//						  */
							if (breversedflow) icell++;


						}

					}
					// ������ �����
					for (integer iscan_par = nd.b0.iright_start; iscan_par <= nd.b0.iright_finish; iscan_par++) {
						integer iPloc = f.ifrontregulationgl[iscan_par];
						if (iPloc >= f.maxelm) {
							breversedflow = false;
							// ���������� ��������� �������.
							// � ��������� ��� �������� ��������.
							// ����� ���������� ���������.
							// �������� ���������� ������� �������.
							// ��������� �������� bool bDirichlet ����� false.
							/*
							my_elmatr_quad_PAm_bon( f.slau_bon,
											  f.slau, iPloc-f.maxelm,
											  f.maxelm,
											  f.maxbound,
											  f.sosedb,
											  f.nvtx,
											  f.bPressureFix,
											  dbeta, f.pa,
											  f.potent, f.prop,
											  f.prop_b, f.alpha,
											  ls, lw, w, false,
											  f.sosedi,
											  f.diag_coef, RCh,
											  breversedflow);
							//					   */

							// ��������� ������� �� ������ ����������� ����������� tau.
							// ����� ���������� ���������.
							// �������� ���������� ������� �������.
							// ��������� �������� bool bDirichlet ����� false.
							//*
							my_elmatr_quad_PAm_bon3(f.slau_bon,
								f.slau, iPloc - f.maxelm,
								f.maxelm,
								f.maxbound,
								f.sosedb,
								f.nvtx,
								f.bPressureFix,
								dbeta, f.pa,
								f.potent,
								f.prop, f.prop_b,
								f.alpha,
								ls, lw, w,
								false,
								f.sosedi, RCh,
								breversedflow,
								tau);

							//						  */
							if (breversedflow) icell++;


						}
					}
					// �������� ��������� �����
					for (integer iscan_par = nd.b0.iseparate_start; iscan_par <= nd.b0.iseparate_finish; iscan_par++) {
						integer iPloc = f.ifrontregulationgl[iscan_par];
						if (iPloc >= f.maxelm) {
							breversedflow = false;
							// ���������� ��������� �������.
							// � ��������� ��� �������� ��������.
							// ����� ���������� ���������.
							// �������� ���������� ������� �������.
							// ��������� �������� bool bDirichlet ����� false.
							/*
							my_elmatr_quad_PAm_bon( f.slau_bon,
											  f.slau, iPloc-f.maxelm,
											  f.maxelm,
											  f.maxbound,
											  f.sosedb,
											  f.nvtx,
											  f.bPressureFix,
											  dbeta, f.pa,
											  f.potent, f.prop,
											  f.prop_b, f.alpha,
											  ls, lw, w, false,
											  f.sosedi,
											  f.diag_coef, RCh,
											  breversedflow);
							//					   */

							// ��������� ������� �� ������ ����������� ����������� tau.
							// ����� ���������� ���������.
							// �������� ���������� ������� �������.
							// ��������� �������� bool bDirichlet ����� false.
							//*
							my_elmatr_quad_PAm_bon3(f.slau_bon,
								f.slau, iPloc - f.maxelm,
								f.maxelm,
								f.maxbound,
								f.sosedb,
								f.nvtx,
								f.bPressureFix,
								dbeta, f.pa,
								f.potent,
								f.prop, f.prop_b,
								f.alpha,
								ls, lw, w,
								false,
								f.sosedi, RCh,
								breversedflow,
								tau);

							//						  */
							if (breversedflow) icell++;


						}
					}


				}
	}
}
else {
	for (i = 0; i<f.maxbound; i++) {


		breversedflow = false;
		// ���������� ��������� �������.
		// � ��������� ��� �������� ��������.
		// ����� ���������� ���������.
		// �������� ���������� ������� �������.
		// ��������� �������� bool bDirichlet ����� false.
		/*
		my_elmatr_quad_PAm_bon( f.slau_bon,
		f.slau, i,
		f.maxelm,
		f.maxbound,
		f.sosedb,
		f.nvtx,
		f.bPressureFix,
		dbeta, f.pa,
		f.potent, f.prop,
		f.prop_b, f.alpha,
		ls, lw, w, false,
		f.sosedi,
		f.diag_coef, RCh,
		breversedflow);
		//					   */

		// ��������� ������� �� ������ ����������� ����������� tau.
		// ����� ���������� ���������.
		// �������� ���������� ������� �������.
		// ��������� �������� bool bDirichlet ����� false.
		//*
		my_elmatr_quad_PAm_bon3(f.slau_bon,
			f.slau, i,
			f.maxelm,
			f.maxbound,
			f.sosedb,
			f.nvtx,
			f.bPressureFix,
			dbeta, f.pa,
			f.potent,
			f.prop, f.prop_b,
			f.alpha,
			ls, lw, w,
			false,
			f.sosedi, RCh,
			breversedflow,
			tau);

		//						  */
		if (breversedflow) icell++;


				  }
}

#else

			      for (i=0; i<f.maxbound; i++) {


					  breversedflow=false;
                       // ���������� ��������� �������.
                       // � ��������� ��� �������� ��������.
                       // ����� ���������� ���������.
					   // �������� ���������� ������� �������.
                       // ��������� �������� bool bDirichlet ����� false.
					  /*
					   my_elmatr_quad_PAm_bon( f.slau_bon,
						                       f.slau, i,
											   f.maxelm, 
											   f.maxbound,
											   f.sosedb,
											   f.nvtx,
											   f.bPressureFix,
											   dbeta, f.pa,
											   f.potent, f.prop,
											   f.prop_b, f.alpha,
											   ls, lw, w, false, 
											   f.sosedi, 
											   f.diag_coef, RCh,
											   breversedflow);
						//					   */

					   // ��������� ������� �� ������ ����������� ����������� tau.
					  // ����� ���������� ���������.
					   // �������� ���������� ������� �������.
                       // ��������� �������� bool bDirichlet ����� false.
					//*
					my_elmatr_quad_PAm_bon3(f.slau_bon,
						                      f.slau, i, 
											  f.maxelm,
											  f.maxbound, 
							                  f.sosedb,
											  f.nvtx,
											  f.bPressureFix,
							                  dbeta, f.pa,
											  f.potent,
							                  f.prop, f.prop_b,
											  f.alpha,
							                  ls, lw, w, 
											  false, 
							                  f.sosedi, RCh,
							                  breversedflow, 
											  tau);

					//						  */
					   if (breversedflow) icell++;

			      
				  }
#endif

				 // printf("step Neiman...\n");
				 // getchar();

				  /*
				  // debug
				   if (inumiter==1) {
					   for ( i=t.maxbound-2; i<t.maxbound; i++) {
					   #if doubleintprecision == 1
							 printf("id=%lld aw=%e ai=%e b=%e\n", i+f.maxelm, f.slau_bon[PAM][i].aw, f.slau_bon[PAM][i].ai, f.slau_bon[PAM][i].b);
					   #else
							printf("id=%d aw=%e ai=%e b=%e\n", i+f.maxelm, f.slau_bon[PAM][i].aw, f.slau_bon[PAM][i].ai, f.slau_bon[PAM][i].b);
					   #endif
		         	getchar();
					}
		          }
				  */

				  if (icell>0) {
					  // ���������� icell �� ���� ������ �������� � ����������,
					  // ���������� icell ������ �������� � ������������.
					  // ���������� �������� icell>0 ������� � ���������� ������� ����� 
					  // �������� ������� �������� �������� ��������� �������������� �������.
#if doubleintprecision == 1
					  printf("reversed flow in %lld cell pressure outlet...\n", icell);
#else
					  printf("reversed flow in %d cell pressure outlet...\n", icell);
#endif

					  
				  }
				  

				#ifdef _OPENMP

				  if (bparallelizm_old) {
					  if (inumcore == 1) {

						  for (i = 0; i < f.maxelm; i++) {
							  rhie_chow[0][i] = 0.0;
							  rhie_chow[1][i] = 0.0;
							  rhie_chow[2][i] = 0.0;

							  if ((mfoldtimestep == NULL) && (speedoldtimestep == NULL)) {

								  // ������������ ������.
								  // ����� ������������ ������������� ��� �� �������,
								  // ��� ����� ������ ������������ ������� �������������� �� ������� �����.
								  /*
								  my_elmatr_quad_PAm(i, f.slau,
								  f.slau_bon,
								  f.potent,
								  f.pa, f.prop,
								  f.prop_b, f.nvtx,
								  f.sosedi, f.maxelm,
								  f.diag_coef,
								  f.alpha, dbeta,
								  rhie_chow, RCh,
								  btimedep,
								  tauparam,
								  NULL,
								  f.mf[i],
								  NULL);
								  */

								  // ������������� �������� ������� �� ������ ����������� �������������.
								  // ��. ���� ��������� ����� sigma cfd.
								  //*
								  my_elmatr_quad_PAm3(i, f.slau,
									  f.slau_bon,
									  f.potent,
									  f.pa,
									  f.prop,
									  f.prop_b,
									  f.nvtx,
									  f.sosedi,
									  f.maxelm,
									  f.alpha,
									  dbeta,
									  rhie_chow, RCh,
									  btimedep,
									  -0.1, // tauparam
									  NULL,
									  f.mf[i],
									  NULL,
									  tau,
									  bmyhighorder, bdeltapfinish,
									  bRhieChowiPAM, false);//*/

							  }
							  else
							  {
								  // �������� ��������
								  /*
								  my_elmatr_quad_PAm(i, f.slau,
								  f.slau_bon,
								  f.potent,
								  f.pa, f.prop,
								  f.prop_b, f.nvtx,
								  f.sosedi, f.maxelm,
								  f.diag_coef,
								  f.alpha, dbeta,
								  rhie_chow, RCh,
								  btimedep,
								  tauparam,
								  mfoldtimestep[i],
								  f.mf[i],
								  speedoldtimestep);
								  */

								  // ������������� �������� ������� �� ������ ����������� �������������.
								  // ��. ���� ��������� ����� sigma cfd.
								  ///*
								  my_elmatr_quad_PAm3(i, f.slau,
									  f.slau_bon,
									  f.potent,
									  f.pa,
									  f.prop,
									  f.prop_b,
									  f.nvtx,
									  f.sosedi,
									  f.maxelm,
									  f.alpha, dbeta,
									  rhie_chow, RCh,
									  btimedep,
									  tauparam,
									  mfoldtimestep[i],
									  f.mf[i],
									  speedoldtimestep,
									  tau,
									  bmyhighorder, bdeltapfinish,
									  bRhieChowiPAM, false);
								  //*/
							  }
						  }
					  }

					  if (inumcore == 2) {
						  if ((mfoldtimestep == NULL) && (speedoldtimestep == NULL)) {

							  if (nd.b0.active) {

#pragma omp parallel
								  {
#pragma omp sections 
									  {
#pragma omp section
										  {
											  // ������ �����
											  for (integer iscan_par = nd.b0.ileft_start; iscan_par <= nd.b0.ileft_finish; iscan_par++) {
												  integer iPloc = f.ifrontregulationgl[iscan_par];
												  if (iPloc < f.maxelm) {
													  rhie_chow[0][iPloc] = 0.0;
													  rhie_chow[1][iPloc] = 0.0;
													  rhie_chow[2][iPloc] = 0.0;



													  // ������������ ������.
													  // ����� ������������ ������������� ��� �� �������,
													  // ��� ����� ������ ������������ ������� �������������� �� ������� �����.
													 /*
													  my_elmatr_quad_PAm(iPloc, f.slau,
																	 f.slau_bon,
																	 f.potent,
																	 f.pa, f.prop,
																	 f.prop_b, f.nvtx,
																	 f.sosedi, f.maxelm,
																	 f.diag_coef,
																	 f.alpha, dbeta,
																	 rhie_chow, RCh,
																	 btimedep,
																	 tauparam,
																	 NULL,
																	 f.mf[iPloc],
																	 NULL);
													 */

													 // ������������� �������� ������� �� ������ ����������� �������������.
													 // ��. ���� ��������� ����� sigma cfd.
													 //*
													  my_elmatr_quad_PAm3(iPloc, f.slau,
														  f.slau_bon,
														  f.potent,
														  f.pa,
														  f.prop,
														  f.prop_b,
														  f.nvtx,
														  f.sosedi,
														  f.maxelm,
														  f.alpha,
														  dbeta,
														  rhie_chow, RCh,
														  btimedep,
														  -0.1, // tauparam
														  NULL,
														  f.mf[iPloc],
														  NULL,
														  tau,
														  bmyhighorder, bdeltapfinish,
														  bRhieChowiPAM, false);//*/


												  }

											  }
										  }
#pragma omp section
										  {
											  // ������ �����
											  for (integer iscan_par = nd.b0.iright_start; iscan_par <= nd.b0.iright_finish; iscan_par++) {
												  integer iPloc = f.ifrontregulationgl[iscan_par];
												  if (iPloc < f.maxelm) {
													  rhie_chow[0][iPloc] = 0.0;
													  rhie_chow[1][iPloc] = 0.0;
													  rhie_chow[2][iPloc] = 0.0;



													  // ������������ ������.
													  // ����� ������������ ������������� ��� �� �������,
													  // ��� ����� ������ ������������ ������� �������������� �� ������� �����.
													 /*
													  my_elmatr_quad_PAm(iPloc, f.slau,
																	 f.slau_bon,
																	 f.potent,
																	 f.pa, f.prop,
																	 f.prop_b, f.nvtx,
																	 f.sosedi, f.maxelm,
																	 f.diag_coef,
																	 f.alpha, dbeta,
																	 rhie_chow, RCh,
																	 btimedep,
																	 tauparam,
																	 NULL,
																	 f.mf[iPloc],
																	 NULL);
													 */

													 // ������������� �������� ������� �� ������ ����������� �������������.
													 // ��. ���� ��������� ����� sigma cfd.
													 //*
													  my_elmatr_quad_PAm3(iPloc, f.slau,
														  f.slau_bon,
														  f.potent,
														  f.pa,
														  f.prop,
														  f.prop_b,
														  f.nvtx,
														  f.sosedi,
														  f.maxelm,
														  f.alpha,
														  dbeta,
														  rhie_chow, RCh,
														  btimedep,
														  -0.1, // tauparam
														  NULL,
														  f.mf[iPloc],
														  NULL,
														  tau,
														  bmyhighorder, bdeltapfinish,
														  bRhieChowiPAM, false);//*/


												  }
											  }
										  }
									  }
								  }
								  // �������� ��������� �����
								  for (integer iscan_par = nd.b0.iseparate_start; iscan_par <= nd.b0.iseparate_finish; iscan_par++) {
									  integer iPloc = f.ifrontregulationgl[iscan_par];
									  if (iPloc < f.maxelm) {
										  rhie_chow[0][iPloc] = 0.0;
										  rhie_chow[1][iPloc] = 0.0;
										  rhie_chow[2][iPloc] = 0.0;



										  // ������������ ������.
										  // ����� ������������ ������������� ��� �� �������,
										  // ��� ����� ������ ������������ ������� �������������� �� ������� �����.
										 /*
										  my_elmatr_quad_PAm(iPloc, f.slau,
														 f.slau_bon,
														 f.potent,
														 f.pa, f.prop,
														 f.prop_b, f.nvtx,
														 f.sosedi, f.maxelm,
														 f.diag_coef,
														 f.alpha, dbeta,
														 rhie_chow, RCh,
														 btimedep,
														 tauparam,
														 NULL,
														 f.mf[iPloc],
														 NULL);
										 */

										 // ������������� �������� ������� �� ������ ����������� �������������.
										 // ��. ���� ��������� ����� sigma cfd.
										 //*
										  my_elmatr_quad_PAm3(iPloc, f.slau,
											  f.slau_bon,
											  f.potent,
											  f.pa,
											  f.prop,
											  f.prop_b,
											  f.nvtx,
											  f.sosedi,
											  f.maxelm,
											  f.alpha,
											  dbeta,
											  rhie_chow, RCh,
											  btimedep,
											  -0.1, // tauparam
											  NULL,
											  f.mf[iPloc],
											  NULL,
											  tau,
											  bmyhighorder, bdeltapfinish,
											  bRhieChowiPAM, false);//*/


									  }
								  }


							  }

						  }
						  else {

							  if (nd.b0.active) {

#pragma omp parallel
								  {
#pragma omp sections 
									  {
#pragma omp section
										  {
											  // ������ �����
											  for (integer iscan_par = nd.b0.ileft_start; iscan_par <= nd.b0.ileft_finish; iscan_par++) {
												  integer iPloc = f.ifrontregulationgl[iscan_par];
												  if (iPloc < f.maxelm) {
													  rhie_chow[0][iPloc] = 0.0;
													  rhie_chow[1][iPloc] = 0.0;
													  rhie_chow[2][iPloc] = 0.0;


													  // �������� ��������
													 /*
													  my_elmatr_quad_PAm(iPloc, f.slau,
																		 f.slau_bon,
																		 f.potent,
																		 f.pa, f.prop,
																		 f.prop_b, f.nvtx,
																		 f.sosedi, f.maxelm,
																		 f.diag_coef,
																		 f.alpha, dbeta,
																		 rhie_chow, RCh,
																		 btimedep,
																		 tauparam,
																		 mfoldtimestep[iPloc],
																		 f.mf[iPloc],
																		 speedoldtimestep);
													 */

													 // ������������� �������� ������� �� ������ ����������� �������������.
													 // ��. ���� ��������� ����� sigma cfd.
													 ///*
													  my_elmatr_quad_PAm3(iPloc, f.slau,
														  f.slau_bon,
														  f.potent,
														  f.pa,
														  f.prop,
														  f.prop_b,
														  f.nvtx,
														  f.sosedi,
														  f.maxelm,
														  f.alpha, dbeta,
														  rhie_chow, RCh,
														  btimedep,
														  tauparam,
														  mfoldtimestep[iPloc],
														  f.mf[iPloc],
														  speedoldtimestep,
														  tau,
														  bmyhighorder, bdeltapfinish,
														  bRhieChowiPAM, false);
													  //*/

												  }

											  }
										  }
#pragma omp section
										  {
											  // ������ �����
											  for (integer iscan_par = nd.b0.iright_start; iscan_par <= nd.b0.iright_finish; iscan_par++) {
												  integer iPloc = f.ifrontregulationgl[iscan_par];
												  if (iPloc < f.maxelm) {
													  rhie_chow[0][iPloc] = 0.0;
													  rhie_chow[1][iPloc] = 0.0;
													  rhie_chow[2][iPloc] = 0.0;


													  // �������� ��������
													 /*
													  my_elmatr_quad_PAm(iPloc, f.slau,
																		 f.slau_bon,
																		 f.potent,
																		 f.pa, f.prop,
																		 f.prop_b, f.nvtx,
																		 f.sosedi, f.maxelm,
																		 f.diag_coef,
																		 f.alpha, dbeta,
																		 rhie_chow, RCh,
																		 btimedep,
																		 tauparam,
																		 mfoldtimestep[iPloc],
																		 f.mf[iPloc],
																		 speedoldtimestep);
													 */

													 // ������������� �������� ������� �� ������ ����������� �������������.
													 // ��. ���� ��������� ����� sigma cfd.
													 ///*
													  my_elmatr_quad_PAm3(iPloc, f.slau,
														  f.slau_bon,
														  f.potent,
														  f.pa,
														  f.prop,
														  f.prop_b,
														  f.nvtx,
														  f.sosedi,
														  f.maxelm,
														  f.alpha, dbeta,
														  rhie_chow, RCh,
														  btimedep,
														  tauparam,
														  mfoldtimestep[iPloc],
														  f.mf[iPloc],
														  speedoldtimestep,
														  tau,
														  bmyhighorder, bdeltapfinish,
														  bRhieChowiPAM, false);
													  //*/

												  }
											  }
										  }
									  }
								  }
								  // �������� ��������� �����
								  for (integer iscan_par = nd.b0.iseparate_start; iscan_par <= nd.b0.iseparate_finish; iscan_par++) {
									  integer iPloc = f.ifrontregulationgl[iscan_par];
									  if (iPloc < f.maxelm) {
										  rhie_chow[0][iPloc] = 0.0;
										  rhie_chow[1][iPloc] = 0.0;
										  rhie_chow[2][iPloc] = 0.0;


										  // �������� ��������
										 /*
										  my_elmatr_quad_PAm(iPloc, f.slau,
															 f.slau_bon,
															 f.potent,
															 f.pa, f.prop,
															 f.prop_b, f.nvtx,
															 f.sosedi, f.maxelm,
															 f.diag_coef,
															 f.alpha, dbeta,
															 rhie_chow, RCh,
															 btimedep,
															 tauparam,
															 mfoldtimestep[iPloc],
															 f.mf[iPloc],
															 speedoldtimestep);
										 */

										 // ������������� �������� ������� �� ������ ����������� �������������.
										 // ��. ���� ��������� ����� sigma cfd.
										 ///*
										  my_elmatr_quad_PAm3(iPloc, f.slau,
											  f.slau_bon,
											  f.potent,
											  f.pa,
											  f.prop,
											  f.prop_b,
											  f.nvtx,
											  f.sosedi,
											  f.maxelm,
											  f.alpha, dbeta,
											  rhie_chow, RCh,
											  btimedep,
											  tauparam,
											  mfoldtimestep[iPloc],
											  f.mf[iPloc],
											  speedoldtimestep,
											  tau,
											  bmyhighorder, bdeltapfinish,
											  bRhieChowiPAM, false);
										  //*/

									  }
								  }


							  }

						  }
					  }
				  }
				  else {
					  for (i = 0; i<f.maxelm; i++) {
						  rhie_chow[0][i] = 0.0;
						  rhie_chow[1][i] = 0.0;
						  rhie_chow[2][i] = 0.0;

						  if ((mfoldtimestep == NULL) && (speedoldtimestep == NULL)) {

							  // ������������ ������.
							  // ����� ������������ ������������� ��� �� �������,
							  // ��� ����� ������ ������������ ������� �������������� �� ������� �����.
							  /*
							  my_elmatr_quad_PAm(i, f.slau,
							  f.slau_bon,
							  f.potent,
							  f.pa, f.prop,
							  f.prop_b, f.nvtx,
							  f.sosedi, f.maxelm,
							  f.diag_coef,
							  f.alpha, dbeta,
							  rhie_chow, RCh,
							  btimedep,
							  tauparam,
							  NULL,
							  f.mf[i],
							  NULL);
							  */

							  // ������������� �������� ������� �� ������ ����������� �������������.
							  // ��. ���� ��������� ����� sigma cfd.
							  //*
							  my_elmatr_quad_PAm3(i, f.slau,
								  f.slau_bon,
								  f.potent,
								  f.pa,
								  f.prop,
								  f.prop_b,
								  f.nvtx,
								  f.sosedi,
								  f.maxelm,
								  f.alpha,
								  dbeta,
								  rhie_chow, RCh,
								  btimedep,
								  -0.1, // tauparam
								  NULL,
								  f.mf[i],
								  NULL,
								  tau,
								  bmyhighorder, bdeltapfinish,
								  bRhieChowiPAM, false);//*/

						  }
						  else
						  {
							  // �������� ��������
							  /*
							  my_elmatr_quad_PAm(i, f.slau,
							  f.slau_bon,
							  f.potent,
							  f.pa, f.prop,
							  f.prop_b, f.nvtx,
							  f.sosedi, f.maxelm,
							  f.diag_coef,
							  f.alpha, dbeta,
							  rhie_chow, RCh,
							  btimedep,
							  tauparam,
							  mfoldtimestep[i],
							  f.mf[i],
							  speedoldtimestep);
							  */

							  // ������������� �������� ������� �� ������ ����������� �������������.
							  // ��. ���� ��������� ����� sigma cfd.
							  ///*
							  if (mfoldtimestep != NULL) {
								  my_elmatr_quad_PAm3(i, f.slau,
									  f.slau_bon,
									  f.potent,
									  f.pa,
									  f.prop,
									  f.prop_b,
									  f.nvtx,
									  f.sosedi,
									  f.maxelm,
									  f.alpha, dbeta,
									  rhie_chow, RCh,
									  btimedep,
									  tauparam,
									  mfoldtimestep[i],
									  f.mf[i],
									  speedoldtimestep,
									  tau,
									  bmyhighorder, bdeltapfinish,
									  bRhieChowiPAM, false);
							  }
							  else {
								  printf("Fatal error!!!\n");
								  printf("mfoldtimestep==NULL error in call  my_elmatr_quad_PAm3.\n");
								  system("PAUSE");
								  exit(1);
							  }
							  //*/
						  }
				   }
				  }

			

#else  

			      for (i=0; i<f.maxelm; i++) {
					   rhie_chow[0][i]=0.0;
                       rhie_chow[1][i]=0.0;
					   rhie_chow[2][i]=0.0;

					   if ((mfoldtimestep==NULL)&&(speedoldtimestep==NULL)) {

						   // ������������ ������.
						   // ����� ������������ ������������� ��� �� �������,
						   // ��� ����� ������ ������������ ������� �������������� �� ������� �����.
						  /*
						   my_elmatr_quad_PAm(i, f.slau, 
						                  f.slau_bon,
										  f.potent, 
										  f.pa, f.prop,
										  f.prop_b, f.nvtx,
										  f.sosedi, f.maxelm,
										  f.diag_coef,
										  f.alpha, dbeta,
										  rhie_chow, RCh,
										  btimedep,
										  tauparam,
										  NULL,
										  f.mf[i],
										  NULL);
						  */

						   // ������������� �������� ������� �� ������ ����������� �������������.
						   // ��. ���� ��������� ����� sigma cfd.
						   //*
                           my_elmatr_quad_PAm3(i, f.slau,
							                   f.slau_bon,  
						                       f.potent,
											   f.pa,
											   f.prop,
											   f.prop_b,
					                           f.nvtx, 
											   f.sosedi,
											   f.maxelm, 
						                       f.alpha,
											   dbeta,
											   rhie_chow, RCh,
						                       btimedep, 
											   -0.1, // tauparam
											   NULL,
						                       f.mf[i],
											   NULL,
											   tau,
											   bmyhighorder, bdeltapfinish,
											   bRhieChowiPAM,false);//*/

					   }
					   else
					   {
					   // �������� ��������
					  /*
					   my_elmatr_quad_PAm(i, f.slau, 
						                  f.slau_bon,
										  f.potent, 
										  f.pa, f.prop,
										  f.prop_b, f.nvtx,
										  f.sosedi, f.maxelm,
										  f.diag_coef,
										  f.alpha, dbeta,
										  rhie_chow, RCh,
										  btimedep,
										  tauparam,
										  mfoldtimestep[i],
										  f.mf[i],
										  speedoldtimestep);
					  */

					   // ������������� �������� ������� �� ������ ����������� �������������.
					   // ��. ���� ��������� ����� sigma cfd.
					   ///*
						   if (mfoldtimestep != NULL) {
							   my_elmatr_quad_PAm3(i, f.slau,
								   f.slau_bon,
								   f.potent,
								   f.pa,
								   f.prop,
								   f.prop_b,
								   f.nvtx,
								   f.sosedi,
								   f.maxelm,
								   f.alpha, dbeta,
								   rhie_chow, RCh,
								   btimedep,
								   tauparam,
								   mfoldtimestep[i],
								   f.mf[i],
								   speedoldtimestep,
								   tau,
								   bmyhighorder, bdeltapfinish,
								   bRhieChowiPAM, false);
						   }
						   else {
							   printf("Fatal error!!!\n");
							   printf("mfoldtimestep==NULL error in call  my_elmatr_quad_PAm3.\n");
							   system("PAUSE");
							   exit(1);
						   }
										   //*/
					   }
				   }

#endif

				  // printf("step internal...\n");
				  // getchar();
				   
				   #ifdef _OPENMP
				   if (bparallelizm_old) {

				  if (inumcore == 1) {
					  // serial
					  // ����������:
					  for (i = 0; i<f.maxelm; i++) {
						  f.slau[PAM][i].ae /= f.slau[PAM][i].ap;
						  f.slau[PAM][i].aw /= f.slau[PAM][i].ap;
						  f.slau[PAM][i].an /= f.slau[PAM][i].ap;
						  f.slau[PAM][i].as /= f.slau[PAM][i].ap;
						  f.slau[PAM][i].at /= f.slau[PAM][i].ap;
						  f.slau[PAM][i].ab /= f.slau[PAM][i].ap;
						  f.slau[PAM][i].b /= f.slau[PAM][i].ap;
						  f.slau[PAM][i].ap = 1.0;
					  }
					  for (i = 0; i<f.maxbound; i++) {
						  f.slau_bon[PAM][i].ai /= f.slau_bon[PAM][i].aw;
						  f.slau_bon[PAM][i].b /= f.slau_bon[PAM][i].aw;
						  f.slau_bon[PAM][i].aw = 1.0;
					  }
				  }

			if (inumcore==2) {
				if (nd.b0.active) {

					// ������ �����
					for (integer iscan_par=nd.b0.ileft_start; iscan_par<=nd.b0.ileft_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
                               f.slau[PAM][iPloc].ae/=f.slau[PAM][iPloc].ap;
						       f.slau[PAM][iPloc].aw/=f.slau[PAM][iPloc].ap;
						       f.slau[PAM][iPloc].an/=f.slau[PAM][iPloc].ap;
						       f.slau[PAM][iPloc].as/=f.slau[PAM][iPloc].ap;
				        	   f.slau[PAM][iPloc].at/=f.slau[PAM][iPloc].ap;
						       f.slau[PAM][iPloc].ab/=f.slau[PAM][iPloc].ap;
						       f.slau[PAM][iPloc].b/=f.slau[PAM][iPloc].ap;
						       f.slau[PAM][iPloc].ap=1.0;
						}

					}
					// ������ �����
					for (integer iscan_par=nd.b0.iright_start; iscan_par<=nd.b0.iright_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							 f.slau[PAM][iPloc].ae/=f.slau[PAM][iPloc].ap;
						     f.slau[PAM][iPloc].aw/=f.slau[PAM][iPloc].ap;
						     f.slau[PAM][iPloc].an/=f.slau[PAM][iPloc].ap;
						     f.slau[PAM][iPloc].as/=f.slau[PAM][iPloc].ap;
				        	 f.slau[PAM][iPloc].at/=f.slau[PAM][iPloc].ap;
						     f.slau[PAM][iPloc].ab/=f.slau[PAM][iPloc].ap;
						     f.slau[PAM][iPloc].b/=f.slau[PAM][iPloc].ap;
						     f.slau[PAM][iPloc].ap=1.0;
						}

					}
					// �������� ��������� �����
					for (integer iscan_par=nd.b0.iseparate_start; iscan_par<=nd.b0.iseparate_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							 f.slau[PAM][iPloc].ae/=f.slau[PAM][iPloc].ap;
						     f.slau[PAM][iPloc].aw/=f.slau[PAM][iPloc].ap;
						     f.slau[PAM][iPloc].an/=f.slau[PAM][iPloc].ap;
						     f.slau[PAM][iPloc].as/=f.slau[PAM][iPloc].ap;
				        	 f.slau[PAM][iPloc].at/=f.slau[PAM][iPloc].ap;
						     f.slau[PAM][iPloc].ab/=f.slau[PAM][iPloc].ap;
						     f.slau[PAM][iPloc].b/=f.slau[PAM][iPloc].ap;
						     f.slau[PAM][iPloc].ap=1.0;
						}

					}


				}
			}

			if (inumcore==2) {
				if (nd.b0.active) {

					// ������ �����
					for (integer iscan_par=nd.b0.ileft_start; iscan_par<=nd.b0.ileft_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc>=f.maxelm) {
							iPloc-=f.maxelm;
							f.slau_bon[PAM][iPloc].ai/=f.slau_bon[PAM][iPloc].aw;
					    	f.slau_bon[PAM][iPloc].b/=f.slau_bon[PAM][iPloc].aw;
					    	f.slau_bon[PAM][iPloc].aw=1.0;
						}

					}
					// ������ �����
					for (integer iscan_par=nd.b0.iright_start; iscan_par<=nd.b0.iright_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc>=f.maxelm) {
                            iPloc-=f.maxelm;
							f.slau_bon[PAM][iPloc].ai/=f.slau_bon[PAM][iPloc].aw;
					    	f.slau_bon[PAM][iPloc].b/=f.slau_bon[PAM][iPloc].aw;
					    	f.slau_bon[PAM][iPloc].aw=1.0;
						}

					}
					// �������� ��������� �����
					for (integer iscan_par=nd.b0.iseparate_start; iscan_par<=nd.b0.iseparate_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc>=f.maxelm) {
							iPloc-=f.maxelm;
							f.slau_bon[PAM][iPloc].ai/=f.slau_bon[PAM][iPloc].aw;
					    	f.slau_bon[PAM][iPloc].b/=f.slau_bon[PAM][iPloc].aw;
					    	f.slau_bon[PAM][iPloc].aw=1.0;
						}

					}


				}
			}
			}
				   else {
					   // ����������:
					   for (i = 0; i<f.maxelm; i++) {
						   f.slau[PAM][i].ae /= f.slau[PAM][i].ap;
						   f.slau[PAM][i].aw /= f.slau[PAM][i].ap;
						   f.slau[PAM][i].an /= f.slau[PAM][i].ap;
						   f.slau[PAM][i].as /= f.slau[PAM][i].ap;
						   f.slau[PAM][i].at /= f.slau[PAM][i].ap;
						   f.slau[PAM][i].ab /= f.slau[PAM][i].ap;
						   f.slau[PAM][i].b /= f.slau[PAM][i].ap;
						   f.slau[PAM][i].ap = 1.0;
					   }
					   for (i = 0; i<f.maxbound; i++) {
						   f.slau_bon[PAM][i].ai /= f.slau_bon[PAM][i].aw;
						   f.slau_bon[PAM][i].b /= f.slau_bon[PAM][i].aw;
						   f.slau_bon[PAM][i].aw = 1.0;
					   }
				   }

#else
				   
				   // ����������:
				    for ( i=0; i<f.maxelm; i++) {
						f.slau[PAM][i].ae/=f.slau[PAM][i].ap;
						f.slau[PAM][i].aw/=f.slau[PAM][i].ap;
						f.slau[PAM][i].an/=f.slau[PAM][i].ap;
						f.slau[PAM][i].as/=f.slau[PAM][i].ap;
						f.slau[PAM][i].at/=f.slau[PAM][i].ap;
						f.slau[PAM][i].ab/=f.slau[PAM][i].ap;
						f.slau[PAM][i].b/=f.slau[PAM][i].ap;
						f.slau[PAM][i].ap=1.0;
					}
					for (i=0; i<f.maxbound; i++) {
						f.slau_bon[PAM][i].ai/=f.slau_bon[PAM][i].aw;
						f.slau_bon[PAM][i].b/=f.slau_bon[PAM][i].aw;
						f.slau_bon[PAM][i].aw=1.0;
					}
					
					
#endif

#if doubleintprecision == 1
					// debug
					//if (inumiter==2) {
					/*
						for ( i=0; i<f.maxelm; i++) {
							printf("id=%lld ap=%e ae=%e aw=%e an=%e as=%e at=%e ab=%e b=%e\n",i, f.slau[PAM][i].ap, f.slau[PAM][i].ae, f.slau[PAM][i].aw, f.slau[PAM][i].an, f.slau[PAM][i].as, f.slau[PAM][i].at, f.slau[PAM][i].ab, f.slau[PAM][i].b);
							getchar();
						}*/
					//}

					/*
						for (i=0; i<f.maxbound; i++) {
							if (fabs(f.slau_bon[PAM][i].ai)<1.0e-20) {
								printf("id=%lld aw=%e ai=%e b=%e %lld %lld\n",i,f.slau_bon[PAM][i].aw,f.slau_bon[PAM][i].ai,f.slau_bon[PAM][i].b, f.slau_bon[PAM][i].iW, f.slau_bon[PAM][i].iI);
								getchar();
							}
						}
					*/
#else
					// debug
					//if (inumiter==2) {
						/*
						for ( i=0; i<f.maxelm; i++) {
							printf("id=%d ap=%e ae=%e aw=%e an=%e as=%e at=%e ab=%e b=%e\n",i, f.slau[PAM][i].ap, f.slau[PAM][i].ae, f.slau[PAM][i].aw, f.slau[PAM][i].an, f.slau[PAM][i].as, f.slau[PAM][i].at, f.slau[PAM][i].ab, f.slau[PAM][i].b);
							getchar();
						}*/
					//}

					/*
					for (i=0; i<f.maxbound; i++) {
						if (fabs(f.slau_bon[PAM][i].ai)<1.0e-20) {
							printf("id=%d aw=%e ai=%e b=%e %d %d\n",i,f.slau_bon[PAM][i].aw,f.slau_bon[PAM][i].ai,f.slau_bon[PAM][i].b, f.slau_bon[PAM][i].iW, f.slau_bon[PAM][i].iI);
							getchar();
						}
					}
					*/
#endif
				  
				   
				  

			       //getchar(); // debug
				   // � res ������������ ����� ������������������� ���������� �����:
				   // ������ ��� ���������� ��.
				   // �� ����� ���������� ����� ������ � 
				  // ���������� ���� ������� ��������� �����-������.
				  if (0) {
				  for (i=0; i<f.maxelm+f.maxbound; i++) {
					  if (i<f.maxelm) {
						  f.potent[FBUF][i]=f.slau[PAM][i].b;
					  }
					  else {
						  f.potent[FBUF][i]=f.slau_bon[PAM][i-f.maxelm].b;
					  }
				  }
				  } // debug
				  if (0) {
#if doubleintprecision == 1
					  for (i = 0; i<f.maxelm; i++) {
						  if (f.slau[PAM][i].b >= 1e-20) {
							  printf("zero internal elem %lld", i);
							  //getchar();
							  system("pause");
					  }

				  }
					  for (i = 0; i<f.maxbound; i++) {
						  if (f.slau_bon[PAM][i].b >= 1e-20) {
							  printf("zero boundary elem %lld", i);
							  //getchar();
							  system("pause");
						  }
					  }
				  }
#else
					  for (i = 0; i<f.maxelm; i++) {
						  if (f.slau[PAM][i].b >= 1e-20) {
							  printf("zero internal elem %d", i);
							  //getchar();
							  system("pause");
						  }

					  }
					  for (i = 0; i<f.maxbound; i++) {
						  if (f.slau_bon[PAM][i].b >= 1e-20) {
							  printf("zero boundary elem %d", i);
							  //getchar();
							  system("pause");
						  }
					  }
				  }
#endif
				  

				  if (0) {
		              xyplot( fglobal, flow_interior, t);
		              printf("xy plot continity calc. OK.\n");
	                 // getchar(); // debug avtosave
					  system("pause");
	              }
				  // ������ ����� ������.
				  /*
				  if (bHORF) {
					  doublereal fHORF = 0.25;
					  if (btimedep) {
						  fHORF = 0.75;
					  }
					  for (i = 0; i < f.maxelm; i++) {
						  f.slau[PAM][i].b = bPamendment_source_old[i] + fHORF*(f.slau[PAM][i].b - bPamendment_source_old[i]);
						  bPamendment_source_old[i] = f.slau[PAM][i].b;
					  }
					  for (i = 0; i < f.maxbound; i++) {
						  f.slau_bon[PAM][i].b = bPamendment_source_old[i + f.maxelm] + fHORF*(f.slau_bon[PAM][i].b - bPamendment_source_old[i + f.maxelm]);
						  bPamendment_source_old[i + f.maxelm] = f.slau_bon[PAM][i].b;
					  }
				  }
				  */
			       res=0.0;
			       for (i=0; i<f.maxelm; i++) {
					   // ����� ������� ����� �������� �.�.
					   // ��� ���������� �� ���� ����. (������ ������).
					   //if (inumiter==2) printf("b=%e, res=%e\n",f.slau[PAM][i].b,res);
					   //if (i%10==0) getchar(); // debug
				       res=fmax(res,fabs(f.slau[PAM][i].b));
				       //res+=fabs(f.slau[PAM][i].b);
				       //res+=f.slau[PAM][i].b*f.slau[PAM][i].b;
			       }
			       //res=sqrt((doublereal)(res/f.maxelm));

			       break;

		case TEMP :   
			        
			      

			      // 3 ��� 2016. ������ � 2D ���������� �����.
			      t.alpha = 1.0; // ��� ����� ����� ����� ���� �������� �������������������.

#pragma omp parallel for
			      for (integer i74 = 0; i74 < t.maxbound; i74++) {
				        sourse2Dproblem[i74] = true;
						conductivity2Dinsource[i74] = -2.0; // ������������� ����� ������������� ���������.
			      }
				  // ��������� ���������������� �� ����� ����������� ��������� �����
				  // ������������ � ��������� ������� �� ��������� �����.
				  for (i = 0; i < t.maxelm; i++) {
					  conduct2Dsourceconstruct(i, t.slau, t.sosedi, t.maxelm,
						  t.sosedb, ls, t.prop);
				  }

			       // ������ ������ ������� ��������� �������� ������� ����������
			       // ������������ ��� �������������� ������ �� ������
			       // btimedep=false; // ������������
			      
				 
                   // ������������� ����
			       // ������ ������������� ������� ������������ ������
				   // ������ ��� ������� ������������ ������ ��������.
				   // ��� ��������� ����������� �������� ����������, 
				   // ���� ���������� �� �������� ����� 
				   // ������� ��� ���������� ������ � ������ �������� ����������.

				   // ���� ��������� ������� ��� 
                   // ��������� ����������������
#pragma omp parallel for
				   for (i=0; i<t.maxbound; i++) {
					   // ������� �������:
					   my_elmatr_quad_T3D_bound(i, t.maxbound,
						                        t.maxelm,
												t.sosedb,
												ls, lw, w, s, 
												t.slau_bon, 
												true, dbeta,
												t.nvtx, t.pa, 
												t.prop,
												t.prop_b, 
												t.potent,
												told_iter,
												t.ptr,
												fglobal,
												poweron_multiplier_sequence);
				   }

				 
#pragma omp parallel for
                   for (i=0; i<t.maxbound; i++) {
					   // ������� ������� ���������� � ������������:
					   my_elmatr_quad_T3D_bound(i, t.maxbound,
						                        t.maxelm,
												t.sosedb,
												ls, lw, w, s,
												t.slau_bon,
												false, dbeta,
												t.nvtx, t.pa,
												t.prop, 
												t.prop_b,
												t.potent,
												told_iter,
												t.ptr,
												fglobal,
												poweron_multiplier_sequence);
				   }
				 

				   imyscheme = UDS;
				   if (bVERYStable) {
					   // ���������� ��������������� ����� ������� �������
					   imyscheme = UDS; // UDS BULG
				   }
				   else {
					   imyscheme = UNEVEN_WACEB;
					   //imyscheme=UDS; 
				   }
				   imyscheme = UDS; //UNEVEN_WACEB;
				   imyscheme = UNEVEN_WACEB; // 30 07 2015.
											 //imyscheme=UNEVENQUICK;// �� �������� �� 30 07 2015.
											 // ����� �������� �� ������������ ���������� ������������ !
				   imyscheme = iTEMPScheme; //  5 02 2017 // 31 07 2015


				   // ������ ����� ������� ��� ���������� ��.
#pragma omp parallel for
			       for (i=0; i<t.maxelm; i++) {
					   if (toldtimestep==NULL) {
						   my_elmatr_quad_T3D(i, t.slau,
						                  t.slau_bon,
										  btimedep, // ������������ ��� ��������������
										  tauparam, // ������ ���� �� �������.
							              imyscheme, // UDS BULG EXP2 QUICK UNEVENQUICK UNEVEN_WACEB
										  t.nvtx,
										  t.potent, 
										  t.pa, t.prop,
										  t.prop_b,
										  t.sosedi, 
										  t.alpha, 
										  dbeta, 
										  bconvective,
										  t.Sc[i], 
                                          t.ipower_time_depend[i],
										  t.maxelm,
										  flow_interior,
										  t.ptr,
										  t.sosedb,
										  ls,fglobal,
										  t.binternalsource,
										  NULL, // ���� ���������� � ����������� ���������� ����
										  b,lb,
										  matlist,
										  inumiter, s, 
                                          poweron_multiplier_sequence,
										  t.ilevel_alice,
							              t.whot_is_block);
				
					   }
					   else
					   {
					   my_elmatr_quad_T3D(i, t.slau,
						                  t.slau_bon,
										  btimedep, // ������������ ��� ��������������
										  tauparam, // ������ ���� �� �������.
						                  imyscheme, // UDS BULG EXP2 QUICK UNEVENQUICK UNEVEN_WACEB
										  t.nvtx,
										  t.potent, 
										  t.pa, t.prop,
										  t.prop_b,
										  t.sosedi, 
										  t.alpha, 
										  dbeta, 
										  bconvective,
										  t.Sc[i], 
                                          t.ipower_time_depend[i],
										  t.maxelm,
										  flow_interior,
										  t.ptr,
										  t.sosedb,
										  ls,fglobal,
										  t.binternalsource,
										  toldtimestep, // ���� ���������� � ����������� ���������� ����
										  b,lb,
										  matlist,
										  inumiter, s, 
										  poweron_multiplier_sequence,
										  t.ilevel_alice,
						                  t.whot_is_block);
					   }
				   }
                   

				   
				  
				   
				   // debug
				   if ((0)&&(inumiter>=66)) {
					   // ������ ������� � ����.
					   print_temp_slau(t);
					   printf("temperature matrix print...\n");
					   //getchar();
					   system("pause");
				   }

                   /*
                   if (t.free_temper_level1) {
					  free_level1_temp(t);
                      printf("memory : 43N+15N-42N=16N\n");
				      getchar();
				   }
				   */
                   res=0.0;
				   
				   for (integer i74 = 0; i74 < t.maxbound; i74++) {
					   if (sourse2Dproblem[i74] == false) {
						  // printf("found non internal source.\n");
						   i75++;
					   }
				   }
#if doubleintprecision == 1
				   // printf("found non internal source %lld \n ",i75);
#else
				   // printf("found non internal source %d \n ",i75);
#endif
				  

				   // ��� SIMPLE ���������.
				   rfluentresval = fluent_residual_for_x(t.slau, t.slau_bon, t.potent, t.maxelm, t.maxbound, TEMP); // ������� �� ������� fluent.

				  

			       break;

		default: if ((iVar == VX) || (iVar == VY) || (iVar == VZ)) {

			// ������������� ����
			// ������ ������������� ������� ������������ ������
			// ������ ��� ������� ������������ ������ ��������.
			// ��� ��������� ����������� �������� ����������, 
			// ���� ���������� �� �������� ����� 
			// ������� ��� ���������� ������ � ������ �������� ����������.


		   // ������ ���������� ������������ � �������������� ������,
		   // � ����������� �� ������������� ��������� btimedep.
		   // �������� tauparam �������� �������� ���� �� �������.

			// ������������� ���� ����������� ������
			// ������ �������:


#ifdef _OPENMP

			if (bparallelizm_old) {
			if (inumcore == 1) {
				// ��������� ������� ������� ����������� 
				// ������ ���������� � ������ �������
				for (i = 0; i < f.maxbound; i++) {
					// ������� ������� ������ �������� ����� true
					my_elmatr_quad_F3D_bound(i, f.maxelm,
						true,
						f.sosedb,
						ls, lw, w,
						iVar,
						f.slau_bon[iVar],
						dbeta, f.pa,
						f.nvtx,
						f.prop_b,
						f.prop,
						f.potent,
						f.iflowregime);
				}

			}
			
			if (inumcore == 2) {
				if (nd.b0.active) {

#pragma omp parallel
					{
#pragma omp sections
						{
#pragma omp section 
							{
								// ������ �����
								for (integer iscan_par = nd.b0.ileft_start; iscan_par <= nd.b0.ileft_finish; iscan_par++) {
									integer iPloc = f.ifrontregulationgl[iscan_par];
									if (iPloc >= f.maxelm) {
										// ��������� ������� ������� ����������� 
										// ������ ���������� � ������ �������
										// ������� ������� ������ �������� ����� true
										my_elmatr_quad_F3D_bound(iPloc - f.maxelm, f.maxelm,
											true,
											f.sosedb,
											ls, lw, w,
											iVar,
											f.slau_bon[iVar],
											dbeta, f.pa,
											f.nvtx,
											f.prop_b,
											f.prop,
											f.potent,
											f.iflowregime);
									} // if

								} // for
							} // omp section
#pragma omp section
							{
								// ������ �����
								for (integer iscan_par = nd.b0.iright_start; iscan_par <= nd.b0.iright_finish; iscan_par++) {
									integer iPloc = f.ifrontregulationgl[iscan_par];
									if (iPloc >= f.maxelm) {
										// ��������� ������� ������� ����������� 
										// ������ ���������� � ������ �������
										// ������� ������� ������ �������� ����� true
										my_elmatr_quad_F3D_bound(iPloc - f.maxelm, f.maxelm,
											true,
											f.sosedb,
											ls, lw, w,
											iVar,
											f.slau_bon[iVar],
											dbeta, f.pa,
											f.nvtx,
											f.prop_b,
											f.prop,
											f.potent,
											f.iflowregime);
									} // if
								} // for
							} // omp section
						}// omp sections
					} // omp parallel
					// �������� ��������� �����
					for (integer iscan_par = nd.b0.iseparate_start; iscan_par <= nd.b0.iseparate_finish; iscan_par++) {
						integer iPloc = f.ifrontregulationgl[iscan_par];
						if (iPloc >= f.maxelm) {
							// ��������� ������� ������� ����������� 
							// ������ ���������� � ������ �������
							// ������� ������� ������ �������� ����� true
							my_elmatr_quad_F3D_bound(iPloc - f.maxelm, f.maxelm,
								true,
								f.sosedb,
								ls, lw, w,
								iVar,
								f.slau_bon[iVar],
								dbeta, f.pa,
								f.nvtx,
								f.prop_b,
								f.prop,
								f.potent,
								f.iflowregime);
						}
					}


				}
			}
		}
			else {
				// ��������� openmp ��� ������� �������������� inumcore
				// ��������� ������� ������� ����������� 
				// ������ ���������� � ������ �������
				for (i = 0; i<f.maxbound; i++) {
					// ������� ������� ������ �������� ����� true
					my_elmatr_quad_F3D_bound(i, f.maxelm,
						true,
						f.sosedb,
						ls, lw, w,
						iVar,
						f.slau_bon[iVar],
						dbeta, f.pa,
						f.nvtx,
						f.prop_b,
						f.prop,
						f.potent,
						f.iflowregime);
				}

			}

#else

			          // ��������� ������� ������� ����������� 
			          // ������ ���������� � ������ �������
                      for (i=0; i<f.maxbound; i++) {
						  // ������� ������� ������ �������� ����� true
						  my_elmatr_quad_F3D_bound(i, f.maxelm,
							                       true,
												   f.sosedb, 
												   ls, lw, w,
												   iVar, 
												   f.slau_bon[iVar],
												   dbeta, f.pa, 
												   f.nvtx,
												   f.prop_b, 
												   f.prop,
												   f.potent,
												   f.iflowregime);
					  }

#endif


					  #ifdef _OPENMP

					  if (bparallelizm_old) {

						  if (inumcore == 1) {
							  // �� ������ ������� ���������� ���������� ������� �������.
							  for (i = 0; i < f.maxbound; i++) {
								  // ���������� ������� ������� ������ �������� ����� false
								  my_elmatr_quad_F3D_bound(i, f.maxelm,
									  false,
									  f.sosedb,
									  ls, lw, w, iVar,
									  f.slau_bon[iVar],
									  dbeta, f.pa,
									  f.nvtx,
									  f.prop_b,
									  f.prop,
									  f.potent,
									  f.iflowregime);
							  }
						  }

						  if (inumcore == 2) {
							  if (nd.b0.active) {


#pragma omp parallel
								  {
#pragma omp sections
									  {
#pragma omp section 
										  {
											  // ������ �����
											  for (integer iscan_par = nd.b0.ileft_start; iscan_par <= nd.b0.ileft_finish; iscan_par++) {
												  integer iPloc = f.ifrontregulationgl[iscan_par];
												  if (iPloc >= f.maxelm) {
													  // �� ������ ������� ���������� ���������� ������� �������.
													  // ���������� ������� ������� ������ �������� ����� false
													  my_elmatr_quad_F3D_bound(iPloc - f.maxelm, f.maxelm,
														  false,
														  f.sosedb,
														  ls, lw, w, iVar,
														  f.slau_bon[iVar],
														  dbeta, f.pa,
														  f.nvtx,
														  f.prop_b,
														  f.prop,
														  f.potent,
														  f.iflowregime);
												  } // if

											  } // for
										  } // omp section
#pragma omp section 
										  {
											  // ������ �����
											  for (integer iscan_par = nd.b0.iright_start; iscan_par <= nd.b0.iright_finish; iscan_par++) {
												  integer iPloc = f.ifrontregulationgl[iscan_par];
												  if (iPloc >= f.maxelm) {
													  // �� ������ ������� ���������� ���������� ������� �������.
													  // ���������� ������� ������� ������ �������� ����� false
													  my_elmatr_quad_F3D_bound(iPloc - f.maxelm, f.maxelm,
														  false,
														  f.sosedb,
														  ls, lw, w, iVar,
														  f.slau_bon[iVar],
														  dbeta, f.pa,
														  f.nvtx,
														  f.prop_b,
														  f.prop,
														  f.potent,
														  f.iflowregime);
												  } // if


											  } // for
										  } // omp section
									  } // omp sections
								  } // omp parallel
								  // �������� ��������� �����
								  for (integer iscan_par = nd.b0.iseparate_start; iscan_par <= nd.b0.iseparate_finish; iscan_par++) {
									  integer iPloc = f.ifrontregulationgl[iscan_par];
									  if (iPloc >= f.maxelm) {
										  // �� ������ ������� ���������� ���������� ������� �������.
										  // ���������� ������� ������� ������ �������� ����� false
										  my_elmatr_quad_F3D_bound(iPloc - f.maxelm, f.maxelm,
											  false,
											  f.sosedb,
											  ls, lw, w, iVar,
											  f.slau_bon[iVar],
											  dbeta, f.pa,
											  f.nvtx,
											  f.prop_b,
											  f.prop,
											  f.potent,
											  f.iflowregime);
									  }


								  }


							  }
						  }
					  }
					  else {
						  // �� ������ ������� ���������� ���������� ������� �������.
						  for (i = 0; i<f.maxbound; i++) {
							  // ���������� ������� ������� ������ �������� ����� false
							  my_elmatr_quad_F3D_bound(i, f.maxelm,
								  false,
								  f.sosedb,
								  ls, lw, w, iVar,
								  f.slau_bon[iVar],
								  dbeta, f.pa,
								  f.nvtx,
								  f.prop_b,
								  f.prop,
								  f.potent,
								  f.iflowregime);
						  }
					  }

#else

					  // �� ������ ������� ���������� ���������� ������� �������.
                      for (i=0; i<f.maxbound; i++) {
						  // ���������� ������� ������� ������ �������� ����� false
						  my_elmatr_quad_F3D_bound(i, f.maxelm, 
							                       false, 
												   f.sosedb,
												   ls, lw, w, iVar,
												   f.slau_bon[iVar],
												   dbeta, f.pa,
												   f.nvtx,
												   f.prop_b, 
												   f.prop, 
												   f.potent,
												   f.iflowregime);
					  }

					  

#endif

					  imyscheme=UDS;
					  if (bVERYStable) {
						  // ���������� ��������������� ����� ������� �������
						  imyscheme=UDS; // UDS BULG
					  }
					  else {
						  imyscheme=UNEVEN_WACEB;
						  //imyscheme=UDS; 
					  }
					  imyscheme=UDS; //UNEVEN_WACEB;
					  imyscheme=UNEVEN_WACEB; // 30 07 2015.
					  //imyscheme=UNEVENQUICK;// �� �������� �� 30 07 2015.
					  // ����� �������� �� ������������ ���������� ������������ !
					  imyscheme=iFLOWScheme; // 31 07 2015

#ifdef _OPENMP

					  if (bparallelizm_old) { 
					  if (inumcore == 1) {
						  doublereal temp_ref = 0.0;
						  temp_ref = f.OpTemp; // Operating Temperature
											   // ���������� ����������� �������� ����������� � ��������� �������:
						  doublereal tavg = 0.0; // ����������� ����������� � ��������� �������.
						  for (integer i_1 = 0; i_1 < f.maxelm; i_1++) {
							  //if (potent_temper[f.ptr[i_1]] < tmin) {
							  tavg += t.potent[f.ptr[i_1]];
							  //}
						  }
						  tavg = tavg / f.maxelm;
						  // ���������� ���� �������� �� ������ tmin ������� �������������� ���������� ���� �������.
						  temp_ref = tavg;

						  // ������ ����� ������� ��� ���������� ��.
						  for (i = 0; i<f.maxelm; i++) {
							  TOCHKA p;
							  center_cord3D(i, f.nvtx, f.pa, p,100);
							  integer ib; // ����� �������� �����
							  in_model_flow(p, ib, b, lb);
							  
							  bool bBussineskApproach = false;
							  bBussineskApproach = matlist[b[ib].imatid].bBussineskApproach;
							  
							 

							  if ((speedoldtimestep == NULL) && (toldtimestep == NULL)) {
								  // ������ ������� ��� ��������� ��������
								  my_elmatr_quad_F3D(i, f.sosedb, lw, ls, f.slau,
									  f.slau_bon,
									  f.diag_coef,
									  iVar,
									  btimedep, // ������������ ��� ��������������
									  tauparam, // ������ ���� �� �������
									  f.ptr,
									  f.nvtx,
									  f.potent,
									  t.potent,
									  f.pa, f.prop,
									  f.prop_b,
									  f.maxelm,
									  f.sosedi,
									  f.alpha,
									  dgx, dgy, dgz,
									  dbeta, imyscheme, // BULG EXP2 QUICK UNEVENQUICK UNEVEN_WACEB
									  bBussineskApproach, temp_ref, // Bussinesk Approach
									  bfirst_start,
									  RCh, // 0.0 RCh // �� ������� ������ �������� ����������� RCh �������� �� ���� �������� � ������������.
									  f.iflowregime, // ����� ������� ���������� ��� ������������
									  NULL, // ���������� �������� � ����������� ���������� ����
									  NULL, // ����������� � ����������� ���� �� �������.
									  b, lb, matlist,
									  f.mf[i], // �������� ����� ����� ����� ������������ ������ � ������ ���������������� �������� ���-���.
									  bVERYStable,
									  sumanb[iVar][i]);
							  }
							  else
							  {

								  // ������ ������� ��� ��������� ��������
								  my_elmatr_quad_F3D(i, f.sosedb, lw, ls, f.slau,
									  f.slau_bon,
									  f.diag_coef,
									  iVar,
									  btimedep, // ������������ ��� ��������������
									  tauparam, // ������ ���� �� �������
									  f.ptr,
									  f.nvtx,
									  f.potent,
									  t.potent,
									  f.pa, f.prop,
									  f.prop_b,
									  f.maxelm,
									  f.sosedi,
									  f.alpha,
									  dgx, dgy, dgz,
									  dbeta, imyscheme, // BULG EXP2 QUICK UNEVENQUICK UNEVEN_WACEB
									  bBussineskApproach, temp_ref, // Bussinesk Approach
									  bfirst_start,
									  RCh, // 0.0 RCh // �� ������� ������ �������� ����������� RCh �������� �� ���� �������� � ������������.
									  f.iflowregime, // ����� ������� ���������� ��� ������������
									  speedoldtimestep[iVar], // ���������� �������� � ����������� ���������� ����
									  toldtimestep, // ����������� � ����������� ���� �� �������.
									  b, lb, matlist,
									  f.mf[i], bVERYStable,// �������� ����� ����� ����� ������������ ������ � ������ ���������������� �������� ���-���.
									  sumanb[iVar][i]);
							  }
						  }
					  }

			if (inumcore==2) {
				if (nd.b0.active) {


					doublereal temp_ref = 0.0;
					temp_ref = f.OpTemp; // Operating Temperature
										 // ���������� ����������� �������� ����������� � ��������� �������:
					//doublereal tavg = 0.0; // ����������� ����������� � ��������� �������.
					//for (integer i_1 = 0; i_1 < f.maxelm; i_1++) {
						//if (potent_temper[f.ptr[i_1]] < tmin) {
						//tavg += t.potent[f.ptr[i_1]];
						//}
					//}
					//tavg = tavg / f.maxelm;
					// ���������� ���� �������� �� ������ tmin ������� �������������� ���������� ���� �������.
					//temp_ref = tavg;



					#pragma omp parallel
					{
#pragma omp sections
						{
#pragma omp section 
							{
					// ������ �����
					for (integer iscan_par=nd.b0.ileft_start; iscan_par<=nd.b0.ileft_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							TOCHKA p;
						  center_cord3D(iPloc,f.nvtx,f.pa,p,100);
						  integer ib; // ����� �������� �����
						  in_model_flow(p,ib,b,lb);
						  
						  bool bBussineskApproach=false;
						  bBussineskApproach=matlist[b[ib].imatid].bBussineskApproach;
						

						  if ((speedoldtimestep==NULL)&&(toldtimestep==NULL)) {
							  // ������ ������� ��� ��������� ��������
						  my_elmatr_quad_F3D(iPloc, f.sosedb, lw,ls, f.slau,
							                 f.slau_bon,
											 f.diag_coef, 
											 iVar, 
											 btimedep, // ������������ ��� ��������������
											 tauparam, // ������ ���� �� �������
											 f.ptr,
											 f.nvtx,
											 f.potent,
											 t.potent,
											 f.pa, f.prop,
											 f.prop_b,
											 f.maxelm,
											 f.sosedi, 
											 f.alpha,
											 dgx, dgy, dgz,
											 dbeta, imyscheme, // BULG EXP2 QUICK UNEVENQUICK UNEVEN_WACEB
											 bBussineskApproach, temp_ref, // Bussinesk Approach
											 bfirst_start,
											 RCh, // 0.0 RCh // �� ������� ������ �������� ����������� RCh �������� �� ���� �������� � ������������.
											 f.iflowregime, // ����� ������� ���������� ��� ������������
											 NULL, // ���������� �������� � ����������� ���������� ����
											 NULL, // ����������� � ����������� ���� �� �������.
											 b,lb, matlist,
											 f.mf[iPloc], // �������� ����� ����� ����� ������������ ������ � ������ ���������������� �������� ���-���.
											 bVERYStable,
											 sumanb[iVar][iPloc]); 
						  }
						  else
						  {

						  // ������ ������� ��� ��������� ��������
						  my_elmatr_quad_F3D(iPloc, f.sosedb, lw,ls, f.slau,
							                 f.slau_bon,
											 f.diag_coef, 
											 iVar, 
											 btimedep, // ������������ ��� ��������������
											 tauparam, // ������ ���� �� �������
											 f.ptr,
											 f.nvtx,
											 f.potent,
											 t.potent,
											 f.pa, f.prop,
											 f.prop_b,
											 f.maxelm,
											 f.sosedi, 
											 f.alpha,
											 dgx, dgy, dgz,
											 dbeta, imyscheme, // BULG EXP2 QUICK UNEVENQUICK UNEVEN_WACEB
											 bBussineskApproach, temp_ref, // Bussinesk Approach
											 bfirst_start,
											 RCh, // 0.0 RCh // �� ������� ������ �������� ����������� RCh �������� �� ���� �������� � ������������.
											 f.iflowregime, // ����� ������� ���������� ��� ������������
											 speedoldtimestep[iVar], // ���������� �������� � ����������� ���������� ����
											 toldtimestep, // ����������� � ����������� ���� �� �������.
											 b,lb, matlist,
											 f.mf[iPloc],bVERYStable,// �������� ����� ����� ����� ������������ ������ � ������ ���������������� �������� ���-���.
											 sumanb[iVar][iPloc]); 
						  }
				      }
						}
							}
#pragma omp section
							{
					// ������ �����
					for (integer iscan_par=nd.b0.iright_start; iscan_par<=nd.b0.iright_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							TOCHKA p;
						  center_cord3D(iPloc,f.nvtx,f.pa,p,100);
						  integer ib; // ����� �������� �����
						  in_model_flow(p,ib,b,lb);
						  
						  bool bBussineskApproach=false;
						  bBussineskApproach=matlist[b[ib].imatid].bBussineskApproach;
						 

						  if ((speedoldtimestep==NULL)&&(toldtimestep==NULL)) {
							  // ������ ������� ��� ��������� ��������
						  my_elmatr_quad_F3D(iPloc, f.sosedb, lw,ls, f.slau,
							                 f.slau_bon,
											 f.diag_coef, 
											 iVar, 
											 btimedep, // ������������ ��� ��������������
											 tauparam, // ������ ���� �� �������
											 f.ptr,
											 f.nvtx,
											 f.potent,
											 t.potent,
											 f.pa, f.prop,
											 f.prop_b,
											 f.maxelm,
											 f.sosedi, 
											 f.alpha,
											 dgx, dgy, dgz,
											 dbeta, imyscheme, // BULG EXP2 QUICK UNEVENQUICK UNEVEN_WACEB
											 bBussineskApproach, temp_ref, // Bussinesk Approach
											 bfirst_start,
											 RCh, // 0.0 RCh // �� ������� ������ �������� ����������� RCh �������� �� ���� �������� � ������������.
											 f.iflowregime, // ����� ������� ���������� ��� ������������
											 NULL, // ���������� �������� � ����������� ���������� ����
											 NULL, // ����������� � ����������� ���� �� �������.
											 b,lb, matlist,
											 f.mf[iPloc], // �������� ����� ����� ����� ������������ ������ � ������ ���������������� �������� ���-���.
											 bVERYStable,
											 sumanb[iVar][iPloc]); 
						  }
						  else
						  {

						  // ������ ������� ��� ��������� ��������
						  my_elmatr_quad_F3D(iPloc, f.sosedb, lw,ls, f.slau,
							                 f.slau_bon,
											 f.diag_coef, 
											 iVar, 
											 btimedep, // ������������ ��� ��������������
											 tauparam, // ������ ���� �� �������
											 f.ptr,
											 f.nvtx,
											 f.potent,
											 t.potent,
											 f.pa, f.prop,
											 f.prop_b,
											 f.maxelm,
											 f.sosedi, 
											 f.alpha,
											 dgx, dgy, dgz,
											 dbeta, imyscheme, // BULG EXP2 QUICK UNEVENQUICK UNEVEN_WACEB
											 bBussineskApproach, temp_ref, // Bussinesk Approach
											 bfirst_start,
											 RCh, // 0.0 RCh // �� ������� ������ �������� ����������� RCh �������� �� ���� �������� � ������������.
											 f.iflowregime, // ����� ������� ���������� ��� ������������
											 speedoldtimestep[iVar], // ���������� �������� � ����������� ���������� ����
											 toldtimestep, // ����������� � ����������� ���� �� �������.
											 b,lb, matlist,
											 f.mf[iPloc],bVERYStable,// �������� ����� ����� ����� ������������ ������ � ������ ���������������� �������� ���-���.
											 sumanb[iVar][iPloc]); 
						  }
				      }
						}

							} // omp section
							} // omp sections
							} // omp parallel

					// �������� ��������� �����
					for (integer iscan_par=nd.b0.iseparate_start; iscan_par<=nd.b0.iseparate_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
TOCHKA p;
						  center_cord3D(iPloc,f.nvtx,f.pa,p,100);
						  integer ib; // ����� �������� �����
						  in_model_flow(p,ib,b,lb);
						  
						  bool bBussineskApproach=false;
						  bBussineskApproach=matlist[b[ib].imatid].bBussineskApproach;
						 

						  if ((speedoldtimestep==NULL)&&(toldtimestep==NULL)) {
							  // ������ ������� ��� ��������� ��������
						  my_elmatr_quad_F3D(iPloc, f.sosedb, lw,ls, f.slau,
							                 f.slau_bon,
											 f.diag_coef, 
											 iVar, 
											 btimedep, // ������������ ��� ��������������
											 tauparam, // ������ ���� �� �������
											 f.ptr,
											 f.nvtx,
											 f.potent,
											 t.potent,
											 f.pa, f.prop,
											 f.prop_b,
											 f.maxelm,
											 f.sosedi, 
											 f.alpha,
											 dgx, dgy, dgz,
											 dbeta, imyscheme, // BULG EXP2 QUICK UNEVENQUICK UNEVEN_WACEB
											 bBussineskApproach, temp_ref, // Bussinesk Approach
											 bfirst_start,
											 RCh, // 0.0 RCh // �� ������� ������ �������� ����������� RCh �������� �� ���� �������� � ������������.
											 f.iflowregime, // ����� ������� ���������� ��� ������������
											 NULL, // ���������� �������� � ����������� ���������� ����
											 NULL, // ����������� � ����������� ���� �� �������.
											 b,lb, matlist,
											 f.mf[iPloc], // �������� ����� ����� ����� ������������ ������ � ������ ���������������� �������� ���-���.
											 bVERYStable,
											 sumanb[iVar][iPloc]); 
						  }
						  else
						  {

						  // ������ ������� ��� ��������� ��������
						  my_elmatr_quad_F3D(iPloc, f.sosedb, lw,ls, f.slau,
							                 f.slau_bon,
											 f.diag_coef, 
											 iVar, 
											 btimedep, // ������������ ��� ��������������
											 tauparam, // ������ ���� �� �������
											 f.ptr,
											 f.nvtx,
											 f.potent,
											 t.potent,
											 f.pa, f.prop,
											 f.prop_b,
											 f.maxelm,
											 f.sosedi, 
											 f.alpha,
											 dgx, dgy, dgz,
											 dbeta, imyscheme, // BULG EXP2 QUICK UNEVENQUICK UNEVEN_WACEB
											 bBussineskApproach, temp_ref, // Bussinesk Approach
											 bfirst_start,
											 RCh, // 0.0 RCh // �� ������� ������ �������� ����������� RCh �������� �� ���� �������� � ������������.
											 f.iflowregime, // ����� ������� ���������� ��� ������������
											 speedoldtimestep[iVar], // ���������� �������� � ����������� ���������� ����
											 toldtimestep, // ����������� � ����������� ���� �� �������.
											 b,lb, matlist,
											 f.mf[iPloc],bVERYStable,// �������� ����� ����� ����� ������������ ������ � ������ ���������������� �������� ���-���.
											 sumanb[iVar][iPloc]); 
						  }
				      }
						}

					}


				}

				}
					  else {
						  doublereal temp_ref = 0.0;
						  temp_ref = f.OpTemp; // Operating Temperature
											   // ���������� ����������� �������� ����������� � ��������� �������:
						  //doublereal tavg = 0.0; // ����������� ����������� � ��������� �������.
						  //for (integer i_1 = 0; i_1 < f.maxelm; i_1++) {
							  //if (potent_temper[f.ptr[i_1]] < tmin) {
							//  tavg += t.potent[f.ptr[i_1]];
							  //}
						  //}
						  //tavg = tavg / f.maxelm;
						  // ���������� ���� �������� �� ������ tmin ������� �������������� ���������� ���� �������.
						 // temp_ref = tavg;

						  // ������ ����� ������� ��� ���������� ��.
						  for (i = 0; i<f.maxelm; i++) {
							  TOCHKA p;
							  center_cord3D(i, f.nvtx, f.pa, p, 100);
							  integer ib; // ����� �������� �����
							  in_model_flow(p, ib, b, lb);

							  bool bBussineskApproach = false;
							  bBussineskApproach = matlist[b[ib].imatid].bBussineskApproach;


							  if ((speedoldtimestep == NULL) && (toldtimestep == NULL)) {
								  // ������ ������� ��� ��������� ��������
								  my_elmatr_quad_F3D(i, f.sosedb, lw, ls, f.slau,
									  f.slau_bon,
									  f.diag_coef,
									  iVar,
									  btimedep, // ������������ ��� ��������������
									  tauparam, // ������ ���� �� �������
									  f.ptr,
									  f.nvtx,
									  f.potent,
									  t.potent,
									  f.pa, f.prop,
									  f.prop_b,
									  f.maxelm,
									  f.sosedi,
									  f.alpha,
									  dgx, dgy, dgz,
									  dbeta, imyscheme, // BULG EXP2 QUICK UNEVENQUICK UNEVEN_WACEB
									  bBussineskApproach, temp_ref, // Bussinesk Approach
									  bfirst_start,
									  RCh, // 0.0 RCh // �� ������� ������ �������� ����������� RCh �������� �� ���� �������� � ������������.
									  f.iflowregime, // ����� ������� ���������� ��� ������������
									  NULL, // ���������� �������� � ����������� ���������� ����
									  NULL, // ����������� � ����������� ���� �� �������.
									  b, lb, matlist,
									  f.mf[i], // �������� ����� ����� ����� ������������ ������ � ������ ���������������� �������� ���-���.
									  bVERYStable,
									  sumanb[iVar][i]);
							  }
							  else
							  {

								  // ������ ������� ��� ��������� ��������
								  my_elmatr_quad_F3D(i, f.sosedb, lw, ls, f.slau,
									  f.slau_bon,
									  f.diag_coef,
									  iVar,
									  btimedep, // ������������ ��� ��������������
									  tauparam, // ������ ���� �� �������
									  f.ptr,
									  f.nvtx,
									  f.potent,
									  t.potent,
									  f.pa, f.prop,
									  f.prop_b,
									  f.maxelm,
									  f.sosedi,
									  f.alpha,
									  dgx, dgy, dgz,
									  dbeta, imyscheme, // BULG EXP2 QUICK UNEVENQUICK UNEVEN_WACEB
									  bBussineskApproach, temp_ref, // Bussinesk Approach
									  bfirst_start,
									  RCh, // 0.0 RCh // �� ������� ������ �������� ����������� RCh �������� �� ���� �������� � ������������.
									  f.iflowregime, // ����� ������� ���������� ��� ������������
									  speedoldtimestep[iVar], // ���������� �������� � ����������� ���������� ����
									  toldtimestep, // ����������� � ����������� ���� �� �������.
									  b, lb, matlist,
									  f.mf[i], bVERYStable,// �������� ����� ����� ����� ������������ ������ � ������ ���������������� �������� ���-���.
									  sumanb[iVar][i]);
							  }
						  }
					  }

#else
//+

                      doublereal temp_ref = 0.0;
                      temp_ref = f.OpTemp; // Operating Temperature
					  // ���������� ����������� �������� ����������� � ��������� �������:
                      //doublereal tavg = 0.0; // ����������� ����������� � ��������� �������.
                      //for (integer i_1 = 0; i_1 < f.maxelm; i_1++) {
	                       //if (potent_temper[f.ptr[i_1]] < tmin) {
	                    //  tavg += t.potent[f.ptr[i_1]];
	                      //}
                      //}
                      //tavg = tavg / f.maxelm;
                      // ���������� ���� �������� �� ������ tmin ������� �������������� ���������� ���� �������.
                    //  temp_ref = tavg;

                      // ������ ����� ������� ��� ���������� ��.
				      for (i=0; i<f.maxelm; i++) { 
						  TOCHKA p;
						  center_cord3D(i,f.nvtx,f.pa,p,100);
						  integer ib; // ����� �������� �����
						  in_model_flow(p,ib,b,lb);
						  
						  bool bBussineskApproach=false;
						  bBussineskApproach=matlist[b[ib].imatid].bBussineskApproach;
						  

						  if ((speedoldtimestep==NULL)&&(toldtimestep==NULL)) {
							  // ������ ������� ��� ��������� ��������
						  my_elmatr_quad_F3D(i, f.sosedb, lw,ls, f.slau,
							                 f.slau_bon,
											 f.diag_coef, 
											 iVar, 
											 btimedep, // ������������ ��� ��������������
											 tauparam, // ������ ���� �� �������
											 f.ptr,
											 f.nvtx,
											 f.potent,
											 t.potent,
											 f.pa, f.prop,
											 f.prop_b,
											 f.maxelm,
											 f.sosedi, 
											 f.alpha,
											 dgx, dgy, dgz,
											 dbeta, imyscheme, // BULG EXP2 QUICK UNEVENQUICK UNEVEN_WACEB
											 bBussineskApproach, temp_ref, // Bussinesk Approach
											 bfirst_start,
											 RCh, // 0.0 RCh // �� ������� ������ �������� ����������� RCh �������� �� ���� �������� � ������������.
											 f.iflowregime, // ����� ������� ���������� ��� ������������
											 NULL, // ���������� �������� � ����������� ���������� ����
											 NULL, // ����������� � ����������� ���� �� �������.
											 b,lb, matlist,
											 f.mf[i], // �������� ����� ����� ����� ������������ ������ � ������ ���������������� �������� ���-���.
											 bVERYStable,
											 sumanb[iVar][i]); 
						  }
						  else
						  {

						  // ������ ������� ��� ��������� ��������
						  my_elmatr_quad_F3D(i, f.sosedb, lw,ls, f.slau,
							                 f.slau_bon,
											 f.diag_coef, 
											 iVar, 
											 btimedep, // ������������ ��� ��������������
											 tauparam, // ������ ���� �� �������
											 f.ptr,
											 f.nvtx,
											 f.potent,
											 t.potent,
											 f.pa, f.prop,
											 f.prop_b,
											 f.maxelm,
											 f.sosedi, 
											 f.alpha,
											 dgx, dgy, dgz,
											 dbeta, imyscheme, // BULG EXP2 QUICK UNEVENQUICK UNEVEN_WACEB
											 bBussineskApproach, temp_ref, // Bussinesk Approach
											 bfirst_start,
											 RCh, // 0.0 RCh // �� ������� ������ �������� ����������� RCh �������� �� ���� �������� � ������������.
											 f.iflowregime, // ����� ������� ���������� ��� ������������
											 speedoldtimestep[iVar], // ���������� �������� � ����������� ���������� ����
											 toldtimestep, // ����������� � ����������� ���� �� �������.
											 b,lb, matlist,
											 f.mf[i],bVERYStable,// �������� ����� ����� ����� ������������ ������ � ������ ���������������� �������� ���-���.
											 sumanb[iVar][i]); 
						  }
				      }

#endif

			          res=0.0;
					  //printf("assemblage finish...\n"); // diagnostic message
					  //getchar();
				  }

#if doubleintprecision == 1
				  /* // ���������� ����������.
				  // ����������������� ����������� ������ ���������� ������ �������� �������. �������� deltaF ��� ������ �������.
				  // ������ ��������� �� deltaF ������ ������� ����������.
				  if (iVar==VY) {
						integer id6[6]={17325,17347,17369,17391,17413,35495}; // ������������������ ����� � ����� ������ �������� �������
						// debug
						if (inumiter==2) {
							for ( i=0; i<5; i++) {
								printf("id=%lld ap=%e ae=%e aw=%e an=%e as=%e at=%e ab=%e b=%e\n",id6[i], f.slau[VY][id6[i]].ap, f.slau[VY][id6[i]].ae, f.slau[VY][id6[i]].aw, f.slau[VY][id6[i]].an, f.slau[VY][id6[i]].as, f.slau[VY][id6[i]].at, f.slau[VY][id6[i]].ab, f.slau[VY][id6[i]].b);
								printf("mf[SSIDE]=%e, mf[NSIDE]=%e\n",f.mf[id6[i]][SSIDE],f.mf[id6[i]][NSIDE]);

							}
							printf("id=%lld aw=%e ai=%e b=%e\n",id6[5], f.slau_bon[VY][id6[i]-f.maxelm].aw, f.slau_bon[VY][id6[i]-f.maxelm].ai,f.slau_bon[VY][id6[i]-f.maxelm].b);
							getchar();
					}

				  }
				  */
#else
				  /* // ���������� ����������.
				  // ����������������� ����������� ������ ���������� ������ �������� �������. �������� deltaF ��� ������ �������.
				  // ������ ��������� �� deltaF ������ ������� ����������.
				  if (iVar==VY) {
						integer id6[6]={17325,17347,17369,17391,17413,35495}; // ������������������ ����� � ����� ������ �������� �������
						// debug
						if (inumiter==2) {
							for ( i=0; i<5; i++) {
								printf("id=%lld ap=%e ae=%e aw=%e an=%e as=%e at=%e ab=%e b=%e\n",id6[i], f.slau[VY][id6[i]].ap, f.slau[VY][id6[i]].ae, f.slau[VY][id6[i]].aw, f.slau[VY][id6[i]].an, f.slau[VY][id6[i]].as, f.slau[VY][id6[i]].at, f.slau[VY][id6[i]].ab, f.slau[VY][id6[i]].b);
								printf("mf[SSIDE]=%e, mf[NSIDE]=%e\n",f.mf[id6[i]][SSIDE],f.mf[id6[i]][NSIDE]);

							}
							printf("id=%lld aw=%e ai=%e b=%e\n",id6[5], f.slau_bon[VY][id6[i]-f.maxelm].aw, f.slau_bon[VY][id6[i]-f.maxelm].ai,f.slau_bon[VY][id6[i]-f.maxelm].b);
							getchar();
						}

				  }
				  */
#endif
				  
			break;
	}

	
	/*
	// ��� ������������� ���������������� ����� �������� ���� �������.
	#ifdef _OPENMP

			if (inumcore==2) {
				if (nd.b0.active) {

					// ������ �����
					for (integer iscan_par=nd.b0.ileft_start; iscan_par<=nd.b0.ileft_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];

					}
					// ������ �����
					for (integer iscan_par=nd.b0.iright_start; iscan_par<=nd.b0.iright_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];

					}
					// �������� ��������� �����
					for (integer iscan_par=nd.b0.iseparate_start; iscan_par<=nd.b0.iseparate_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];

					}


				}
			}

#else
	*/



	/*
    for (i=0; i<maxelm; i++) {
		printf("%.3f %.3f %.3f %.3f %.3f = %.3f \n", slau[iVar][i].ap, slau[iVar][i].ae, slau[iVar][i].an, slau[iVar][i].as, slau[iVar][i].aw, slau[iVar][i].b);
	}
	printf("\n");
	getchar();
	//*/
	
	

	/*doublereal *rthdsd=NULL; // ������ ����� ������� ���������
	if (iVar!=TEMP) {
		rthdsd = new doublereal[f.maxelm+f.maxbound];
	}
	else {
		rthdsd = new doublereal[t.maxelm+t.maxbound];
	}*/
	if (rthdsd==NULL) {
		// ������������ ������ �� ������ ������������.
		printf("Problem : not enough memory on your equipment...\n");
		printf("Please any key to exit...\n");
		exit(1);
	}

	SIMPLESPARSE sparseM; // ����������� �������
	IMatrix sparseS; // ����������� ������� � ������� IMatrix
	

	// ���� ��� ���������� ����� ������ �� �� �� �������� ����� ������� ������,
	// �� ���������� ������ BiCGStab ��� ��� ������� solve.
	bool bBiCGStabSaad=true;
    
    // ����������� ������� � ������� CSIR
    //doublereal *adiag, *altr;
    //integer *jptr, *iptr;

	

	if (iVar!=TEMP) {

		// ��������� ������ � ������������� ��� 
	    // ���������� ����������� �������.
		if (!bBiCGStabSaad) {
		    initsimplesparse(sparseM, f.maxelm + f.maxbound );
		    initIMatrix(&sparseS, f.maxelm + f.maxbound);
		}
		else {
			/*Lr1sk_up
			if (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2) {
				// LR1sk
				initsimplesparse(sparseM, f.maxelm + f.maxbound);
				initIMatrix(&sparseS, f.maxelm + f.maxbound);
			}
			*/
		}

		// ��� ������������� �������������������. TODO

		// ������������ �������������������:

		    // �������������� ������ �� � ���� ������ ������ �� ���������� �������. TODO
		    // ����� ��� ���������� ��� �������� ����� ���������� � ��������. 
			// 1.0 ����������� ��������.

			// ���������� ����:
            //for (i=0; i<f.maxelm; i++) {
                //f.slau[iVar][i].ae/=f.slau[iVar][i].ap;
                //f.slau[iVar][i].an/=f.slau[iVar][i].ap;
                //f.slau[iVar][i].at/=f.slau[iVar][i].ap;
                //f.slau[iVar][i].as/=f.slau[iVar][i].ap;
			    //f.slau[iVar][i].aw/=f.slau[iVar][i].ap;
			    //f.slau[iVar][i].ab/=f.slau[iVar][i].ap;
                //f.slau[iVar][i].b/=f.slau[iVar][i].ap;
                //f.slau[iVar][i].ap=1.0;
		    //}

			// ��������� ����:
			//for (i=0; i<f.maxbound; i++) {
				//f.slau_bon[iVar][i].ai/=f.slau_bon[iVar][i].aw;
				//f.slau_bon[iVar][i].b/=f.slau_bon[iVar][i].aw;
				//f.slau_bon[iVar][i].aw=1.0;
			//}
			
		// � �������� ����� ���� �� ������ ������� ���������� ��� �������� ��������,
		// �� ��������� � ������ �������� ����� �������������. ����� ������������� � ��������
		// ��������� �� ��� ���� ������������ ������� ��� �������� �������� ����� ���� ��� ������� ��������� ��������.
		// ����� ������ ����� ������ �������������� ������� - ������������ ������������� �������� �������
		// ���������� � ���������� ����� ����� ���������� ��������� - ��� ������ �������� �������� � ������� �������� ��������� ��� ��������.

		// ��������� � ��� ��� ��������� ������ ������ ����������.
		// ���� ��� ������� � ��� ��� ������������ ��������� ������� ����. ������ ����� ������������ �� ������ �� �����������.
		// ������ ������ �������� ������ ������� tau ~ alpha/ap/(1.0-alpha) ��� ��� apzero=(1-alpha)*ap/alpha. ����� ��� ��� alpha=1.0 ��� ������������� ���������� ���������� apzero==0.0.
		// ������ ������ ����������� ������� ���� ����� � ������: ap->ap/alpha; b->b+ap*(1.0-alpha)*Vpolditer/alpha.
		// ���������� �������� � ������ ��������� ������.

		

		// ��� ���������� ����� ��������� �����:
		for (i = 0; i < f.maxelm; i++) {
			switch (iVar) {
			case VX: if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { addelmsimplesparse(sparseM, f.slau[iVar][i].ap / f.alpha[iVar], f.slau[iVar][i].iP, f.slau[iVar][i].iP, true); }
					 if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { setValueIMatrix(&sparseS, f.slau[iVar][i].iP, f.slau[iVar][i].iP, f.slau[iVar][i].ap / f.alpha[iVar]); }
#if doubleintprecision == 1
					 // printf("iP=%lld mm=%lld i=%lld iVar=%lld", f.slau[iVar][i].iP, f.maxelm,i,iVar);
#else
					 // printf("iP=%d mm=%d i=%d iVar=%d", f.slau[iVar][i].iP, f.maxelm,i,iVar);
#endif
					 
					 rthdsd[f.slau[iVar][i].iP] = f.slau[iVar][i].b + (1 - f.alpha[iVar])*f.slau[iVar][i].ap*f.potent[VXCOR][f.slau[iVar][i].iP] / f.alpha[iVar];
					 break;
			case VY: if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { addelmsimplesparse(sparseM, f.slau[iVar][i].ap / f.alpha[iVar], f.slau[iVar][i].iP, f.slau[iVar][i].iP, true); }
					 if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { setValueIMatrix(&sparseS, f.slau[iVar][i].iP, f.slau[iVar][i].iP, f.slau[iVar][i].ap / f.alpha[iVar]); }
					 rthdsd[f.slau[iVar][i].iP] = f.slau[iVar][i].b + (1 - f.alpha[iVar])*f.slau[iVar][i].ap*f.potent[VYCOR][f.slau[iVar][i].iP] / f.alpha[iVar];
					 break;
			case VZ: if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { addelmsimplesparse(sparseM, f.slau[iVar][i].ap / f.alpha[iVar], f.slau[iVar][i].iP, f.slau[iVar][i].iP, true); }
					 if ((!bBiCGStabSaad)/* || (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { setValueIMatrix(&sparseS, f.slau[iVar][i].iP, f.slau[iVar][i].iP, f.slau[iVar][i].ap / f.alpha[iVar]); }
                         rthdsd[f.slau[iVar][i].iP]=f.slau[iVar][i].b+(1-f.alpha[iVar])*f.slau[iVar][i].ap*f.potent[VZCOR][f.slau[iVar][i].iP]/f.alpha[iVar];
				         break;
			   case PAM : // PRESSURE:
				   if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { addelmsimplesparse(sparseM, f.slau[iVar][i].ap, f.slau[iVar][i].iP, f.slau[iVar][i].iP, true); }
				   if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { setValueIMatrix(&sparseS, f.slau[iVar][i].iP, f.slau[iVar][i].iP, f.slau[iVar][i].ap); }
			             rthdsd[f.slau[iVar][i].iP]=f.slau[iVar][i].b;
				         break;
		     } // switch

			 
		
			if ((!bBiCGStabSaad)/* || (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) {
		         const doublereal nonzeroEPS=1e-37; // ��� ��������� ������������� ����

			     if ((f.slau[iVar][i].iE>-1) && (fabs(f.slau[iVar][i].ae) > nonzeroEPS)) {
				    addelmsimplesparse(sparseM, -f.slau[iVar][i].ae, f.slau[iVar][i].iP, f.slau[iVar][i].iE, true);
				    setValueIMatrix(&sparseS, f.slau[iVar][i].iP, f.slau[iVar][i].iE, -f.slau[iVar][i].ae);
			     }
			     if ((f.slau[iVar][i].iN>-1) && (fabs(f.slau[iVar][i].an) > nonzeroEPS)) {
				    addelmsimplesparse(sparseM, -f.slau[iVar][i].an, f.slau[iVar][i].iP, f.slau[iVar][i].iN, true);
				    setValueIMatrix(&sparseS, f.slau[iVar][i].iP, f.slau[iVar][i].iN, -f.slau[iVar][i].an);
			     }
			     if ((f.slau[iVar][i].iT>-1) && (fabs(f.slau[iVar][i].at) > nonzeroEPS)) {
				    addelmsimplesparse(sparseM, -f.slau[iVar][i].at, f.slau[iVar][i].iP, f.slau[iVar][i].iT, true);
                    setValueIMatrix(&sparseS, f.slau[iVar][i].iP, f.slau[iVar][i].iT, -f.slau[iVar][i].at);
			     }
			     if ((f.slau[iVar][i].iS>-1) && (fabs(f.slau[iVar][i].as) > nonzeroEPS)) {
				    addelmsimplesparse(sparseM, -f.slau[iVar][i].as, f.slau[iVar][i].iP, f.slau[iVar][i].iS, true);
                    setValueIMatrix(&sparseS, f.slau[iVar][i].iP, f.slau[iVar][i].iS, -f.slau[iVar][i].as);
			     }
			     if ((f.slau[iVar][i].iW>-1) && (fabs(f.slau[iVar][i].aw) > nonzeroEPS)) {
				    addelmsimplesparse(sparseM, -f.slau[iVar][i].aw, f.slau[iVar][i].iP, f.slau[iVar][i].iW, true);
                    setValueIMatrix(&sparseS, f.slau[iVar][i].iP, f.slau[iVar][i].iW, -f.slau[iVar][i].aw);
			     }
			     if ((f.slau[iVar][i].iB>-1) && (fabs(f.slau[iVar][i].ab) > nonzeroEPS)) {
				    addelmsimplesparse(sparseM, -f.slau[iVar][i].ab, f.slau[iVar][i].iP, f.slau[iVar][i].iB, true);
				    setValueIMatrix(&sparseS, f.slau[iVar][i].iP, f.slau[iVar][i].iB, -f.slau[iVar][i].ab);
			     }
			 }

			
	   } // for

		

       // ��� ��������� ����� ��������� �����:
	   for (i=0; i<f.maxbound; i++) {

           // � ��������� �������� ������� ���������� ��������� ������� ? ��� ������� ������.
           // ���� ������� ������ ����� ������ � ������ ����� � ��������� �������� ������� ����������� ������ ����������.

		   if (f.slau_bon[iVar][i].iI>-1) {
			   // ���� ������� ������� �� ������ ����������:

			   /*
		       switch (iVar) {
			   case VX : if (!bBiCGStabSaad) { addelmsimplesparse(sparseM, f.slau_bon[iVar][i].aw/f.alpha[iVar], f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, true); }
						 if (!bBiCGStabSaad) { setValueIMatrix(&sparseS, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].aw/f.alpha[iVar]);  }
				         rthdsd[f.slau_bon[iVar][i].iW]=f.slau_bon[iVar][i].b+(1-f.alpha[iVar])*f.slau_bon[iVar][i].aw*f.potent[VXCOR][f.slau_bon[iVar][i].iW]/f.alpha[iVar];
				         break;
			   case VY : if (!bBiCGStabSaad) { addelmsimplesparse(sparseM, f.slau_bon[iVar][i].aw/f.alpha[iVar], f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, true);}
						 if (!bBiCGStabSaad) { setValueIMatrix(&sparseS, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].aw/f.alpha[iVar]);}
                         rthdsd[f.slau_bon[iVar][i].iW]=f.slau_bon[iVar][i].b+(1-f.alpha[iVar])*f.slau_bon[iVar][i].aw*f.potent[VYCOR][f.slau_bon[iVar][i].iW]/f.alpha[iVar];
				         break;
			   case VZ : if (!bBiCGStabSaad) { addelmsimplesparse(sparseM, f.slau_bon[iVar][i].aw/f.alpha[iVar], f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, true);}
						 if (!bBiCGStabSaad) { setValueIMatrix(&sparseS, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].aw/f.alpha[iVar]);}
                         rthdsd[f.slau_bon[iVar][i].iW]=f.slau_bon[iVar][i].b+(1-f.alpha[iVar])*f.slau_bon[iVar][i].aw*f.potent[VZCOR][f.slau_bon[iVar][i].iW]/f.alpha[iVar];
				         break;
			       case PAM : // PRESSURE: 
					   if (!bBiCGStabSaad) { addelmsimplesparse(sparseM, f.slau_bon[iVar][i].aw, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, true);}
					   if (!bBiCGStabSaad) { setValueIMatrix(&sparseS, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].aw); }
			             rthdsd[f.slau_bon[iVar][i].iW]=f.slau_bon[iVar][i].b;
				         break;
		       } // switch
			   */

			   //*
			   // ��������� �������� �� ������ ������������ ������ ����������.
			   // ��� ������� ������������������������� � ���� ��������, ��� ������� ���������� ���� � ������
			   // ���� � ��������� ������� ����������� ������ ����������. ������� equation3DtoCRS ���������� ������������� ������� !.
			   // �� ������ � �������� ����� ��������� �� ���� � � ������ �������� !.
			   switch (iVar) {
			   case VX: if ((!bBiCGStabSaad)/* || (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { addelmsimplesparse(sparseM, f.slau_bon[iVar][i].aw, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, true); }
						if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { setValueIMatrix(&sparseS, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].aw); }
				         rthdsd[f.slau_bon[iVar][i].iW]=f.slau_bon[iVar][i].b;
				         break;
			   case VY: if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { addelmsimplesparse(sparseM, f.slau_bon[iVar][i].aw, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, true); }
						if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { setValueIMatrix(&sparseS, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].aw); }
                         rthdsd[f.slau_bon[iVar][i].iW]=f.slau_bon[iVar][i].b;
				         break;
			   case VZ: if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { addelmsimplesparse(sparseM, f.slau_bon[iVar][i].aw, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, true); }
						if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { setValueIMatrix(&sparseS, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].aw); }
                         rthdsd[f.slau_bon[iVar][i].iW]=f.slau_bon[iVar][i].b;
				         break;
			       case PAM : // PRESSURE: 
					   if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { addelmsimplesparse(sparseM, f.slau_bon[iVar][i].aw, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, true); }
					   if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { setValueIMatrix(&sparseS, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].aw); }
			             rthdsd[f.slau_bon[iVar][i].iW]=f.slau_bon[iVar][i].b;
				         break;
		       } // switch
			   //*/
		   }
		   else {
			   // ���� ����� ������� �������:

               switch (iVar) {
			   case VX: if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { addelmsimplesparse(sparseM, f.slau_bon[iVar][i].aw, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, true); }
						if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { setValueIMatrix(&sparseS, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].aw); }
				         rthdsd[f.slau_bon[iVar][i].iW]=f.slau_bon[iVar][i].b;
#if doubleintprecision == 1
						 //printf("%lld, eto ob=%lld \n",i, f.slau_bon[iVar][i].iW); getchar(); // debug
#else
						 //printf("%d, eto ob=%d \n",i, f.slau_bon[iVar][i].iW); getchar(); // debug
#endif
						 
                         /* // ��������� ���� �� ������������ ������ ���������� 
						 if (f.slau_bon[iVar][i].iW==13671) {
							 printf("in=%e, nominal=%e\n",f.potent[VXCOR][13671],f.potent[VXCOR][f.slau_bon[iVar][i].iW]);
	                         getchar();
						 }*/
				         break;
			   case VY: if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { addelmsimplesparse(sparseM, f.slau_bon[iVar][i].aw, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, true); }
						if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { setValueIMatrix(&sparseS, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].aw); }
                         rthdsd[f.slau_bon[iVar][i].iW]=f.slau_bon[iVar][i].b;
				         break;
			   case VZ: if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { addelmsimplesparse(sparseM, f.slau_bon[iVar][i].aw, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, true); }
						if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { setValueIMatrix(&sparseS, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].aw); }
                         rthdsd[f.slau_bon[iVar][i].iW]=f.slau_bon[iVar][i].b;
				         break;
			     case PAM : // PRESSURE: 
					 if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { addelmsimplesparse(sparseM, f.slau_bon[iVar][i].aw, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, true); }
					 if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) { setValueIMatrix(&sparseS, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].aw); }
			             rthdsd[f.slau_bon[iVar][i].iW]=f.slau_bon[iVar][i].b;
				         break;
		       } // switch

		   }

		   if ((!bBiCGStabSaad) /*|| (bBiCGStabSaad && (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2))*/) {
               const doublereal nonzeroEPS=1e-37; // ��� ��������� ������������� ����

			   if ((f.slau_bon[iVar][i].iI>-1) && (fabs(f.slau_bon[iVar][i].ai) > nonzeroEPS)) {
				   addelmsimplesparse(sparseM, -f.slau_bon[iVar][i].ai, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iI, true);
                   setValueIMatrix(&sparseS, f.slau_bon[iVar][i].iW, f.slau_bon[iVar][i].iI, -f.slau_bon[iVar][i].ai);
				   // if (iVar==PAM) printf("neiman...\n"); // debug
			   }
		   }

	   } // for

	   
	   

	}
	else { 
		    // �����������
		
		   if (!bBiCGStabSaad) {
        	   // ��������� ������ � ������������� ��� 
	           // ���������� ����������� �������.
	           initsimplesparse(sparseM, t.maxelm + t.maxbound);
		       initIMatrix(&sparseS, t.maxelm + t.maxbound);
		   }

		  

            // ������������ �������������������:
			// ����� ��� ���������� ��� �������� ����� ���������� � ��������. 
			// 1.0 ����������� ��������.

			/*
			// �� �������� ������������ ������������������� 
			// ������� �� LR ������ � ���� ��� ���������������� �� ����� ������
			// ����� ����� �� ����� � ����������������� ���������.
			// ���������� ����:
            for (i=0; i<t.maxelm; i++) {
                t.slau[i].ae/=t.slau[i].ap;
                t.slau[i].an/=t.slau[i].ap;
                t.slau[i].at/=t.slau[i].ap;
                t.slau[i].as/=t.slau[i].ap;
			    t.slau[i].aw/=t.slau[i].ap;
			    t.slau[i].ab/=t.slau[i].ap;
                t.slau[i].b/=t.slau[i].ap;
                t.slau[i].ap=1.0;
		    }

			// ��������� ����:
			for (i=0; i<t.maxbound; i++) {
				t.slau_bon[i].ai/=t.slau_bon[i].aw;
				t.slau_bon[i].b/=t.slau_bon[i].aw;
				t.slau_bon[i].aw=1.0;
			}
			*/
			for (i=0; i<t.maxelm+t.maxbound; i++) {
				rthdsd[i]=0.0; // ���������
			}
			
			// 7 ������� 2016 ������� ������ ���������� ��� ����������� � ������� ����.

		    // ������ ��������� ��� ���������� ����� � �������.
		    for (i=0; i<t.maxelm; i++) {
#if doubleintprecision == 1
				//printf("%lld %e %e %e %e %e %e %e %e\n",i, t.slau[i].ap,  t.slau[i].ae,  t.slau[i].aw,  t.slau[i].an,  t.slau[i].as,  t.slau[i].at,  t.slau[i].ab);
#else
				//printf("%d %e %e %e %e %e %e %e %e\n",i, t.slau[i].ap,  t.slau[i].ae,  t.slau[i].aw,  t.slau[i].an,  t.slau[i].as,  t.slau[i].at,  t.slau[i].ab);
#endif
				//getchar(); // debug

				if (!bBiCGStabSaad) {
                   addelmsimplesparse(sparseM, t.slau[i].ap, t.slau[i].iP, t.slau[i].iP, true);
				  // addelmsimplesparse(sparseM, t.slau[i].ap/t.alpha, t.slau[i].iP, t.slau[i].iP, true);
				}

				 /*if (!(t.slau[i].ap==t.slau[i].ap)) {
					 printf("error matrix construct node=%i\n",i);
					 getchar();
				 }
				 */

				if (!bBiCGStabSaad) {
					setValueIMatrix(&sparseS,t.slau[i].iP,t.slau[i].iP,t.slau[i].ap);
					//setValueIMatrix(&sparseS, t.slau[i].iP, t.slau[i].iP, t.slau[i].ap/t.alpha);
				}
		       //rthdsd[t.slau[i].iP]=t.slau[i].b;
				//rthdsd[t.slau[i].iP] = t.slau[i].b + (1 - t.alpha)*t.slau[i].ap*t.potent[t.slau[i].iP] / t.alpha;
				rthdsd[t.slau[i].iP] = t.slau[i].b + rthdsd[t.slau[i].iP] + ((1 - t.alpha)*t.slau[i].ap*t.potent[t.slau[i].iP] )/ t.alpha;
				
				//told_iter
            
				if (!bBiCGStabSaad) {
			         const doublereal nonzeroEPS=1e-37; // ��� ��������� ������������� ����

			
			         if ((t.slau[i].iE>-1) && (fabs(t.slau[i].ae) > nonzeroEPS)){
				         addelmsimplesparse(sparseM, -t.slau[i].ae, t.slau[i].iP, t.slau[i].iE, true);
                         setValueIMatrix(&sparseS,t.slau[i].iP,t.slau[i].iE,-t.slau[i].ae);
			         }
			         if ((t.slau[i].iN>-1) && (fabs(t.slau[i].an) > nonzeroEPS)) {
				        addelmsimplesparse(sparseM, -t.slau[i].an, t.slau[i].iP, t.slau[i].iN, true);
				        setValueIMatrix(&sparseS,t.slau[i].iP,t.slau[i].iN,-t.slau[i].an);
			         }
			         if ((t.slau[i].iT>-1) && (fabs(t.slau[i].at) > nonzeroEPS)) {
				        addelmsimplesparse(sparseM, -t.slau[i].at, t.slau[i].iP, t.slau[i].iT, true);
                        setValueIMatrix(&sparseS,t.slau[i].iP,t.slau[i].iT,-t.slau[i].at);
			         }
			         if ((t.slau[i].iS>-1) && (fabs(t.slau[i].as) > nonzeroEPS)) {
				        addelmsimplesparse(sparseM, -t.slau[i].as, t.slau[i].iP, t.slau[i].iS, true);
                       setValueIMatrix(&sparseS,t.slau[i].iP,t.slau[i].iS,-t.slau[i].as);
			         }
			         if ((t.slau[i].iW>-1) && (fabs(t.slau[i].aw) > nonzeroEPS)) {
				        addelmsimplesparse(sparseM, -t.slau[i].aw, t.slau[i].iP, t.slau[i].iW, true);
                        setValueIMatrix(&sparseS,t.slau[i].iP,t.slau[i].iW,-t.slau[i].aw);
			         }
			         if ((t.slau[i].iB>-1) && (fabs(t.slau[i].ab) > nonzeroEPS)) {
				        addelmsimplesparse(sparseM, -t.slau[i].ab, t.slau[i].iP, t.slau[i].iB, true);
                        setValueIMatrix(&sparseS,t.slau[i].iP,t.slau[i].iB,-t.slau[i].ab);
		             }
			
                 // ���������� ���������� ������� ��� ���������� ��.
				}
		    }

			

			if (0) {
				// debug
			    for (i=0; i<t.maxelm; i++) {
#if doubleintprecision == 1
					printf("%lld %e\n", i, rthdsd[i]);
#else
					printf("%d %e\n", i, rthdsd[i]);
#endif
			         
					 if (i % 10 == 0) {
						// getchar();
						 system("pause");
					 }
			    }
			}

            // ������ ��������� ��� ��������� ����� � �������:
            for (i=0; i<t.maxbound; i++) {
                
				if (!bBiCGStabSaad) {
					
				    addelmsimplesparse(sparseM, t.slau_bon[i].aw, t.slau_bon[i].iW, t.slau_bon[i].iW, true);
				    setValueIMatrix(&sparseS,t.slau_bon[i].iW ,t.slau_bon[i].iW, t.slau_bon[i].aw);
					//addelmsimplesparse(sparseM, t.slau_bon[i].aw/t.alpha, t.slau_bon[i].iW, t.slau_bon[i].iW, true);
					//setValueIMatrix(&sparseS, t.slau_bon[i].iW, t.slau_bon[i].iW, t.slau_bon[i].aw/t.alpha);
				}
		         rthdsd[t.slau_bon[i].iW]=t.slau_bon[i].b;				
			    //rthdsd[t.slau_bon[i].iW] = t.slau_bon[i].b + (1 - t.alpha)*t.slau_bon[i].aw*t.potent[t.slau_bon[i].iW] / t.alpha;

				 if (!bBiCGStabSaad) {
				     const doublereal nonzeroEPS=1e-37; // ��� ��������� ������������� ����

				     if ((t.slau_bon[i].iI>-1) && (fabs(t.slau_bon[i].ai) > nonzeroEPS)) {
				        addelmsimplesparse(sparseM, -t.slau_bon[i].ai, t.slau_bon[i].iW, t.slau_bon[i].iI, true);
                        setValueIMatrix(&sparseS,t.slau_bon[i].iW,t.slau_bon[i].iI,-t.slau_bon[i].ai);
		             }
				 }
            }

			if (rthdsd_no_radiosity_patch == NULL) {
				rthdsd_no_radiosity_patch = new doublereal[t.maxelm+t.maxbound + 1];
				if (rthdsd_no_radiosity_patch == NULL) {
					// ������������ ������ �� ������ ������������.
					printf("Problem : not enough memory on your equipment for rthdsd_no_radiosity_patch my_solverv0_03 my_agregat_amg.cpp...\n");
					printf("Please any key to exit...\n");
					exit(1);
				}
			}

			if (rthdsd_no_radiosity_patch != NULL) {
				for (integer i23 = 0; i23 < t.maxelm + t.maxbound; i23++) {
					rthdsd_no_radiosity_patch[i23] = rthdsd[i23];
				}
			}

			radiosity_patch_for_vacuum_Prism_Object_(rthdsd, b, lb, t.maxelm);

            //printM_and_CSIR(sparseM, t.maxelm + t.maxbound);
			

			//printf("matrix printing...\n");
			//getchar();

		    /*
		    printf("add elem in matrix ready\n");
		    // ������������ ������ �� ��� t.slau
		    if (t.free_temper_level2) {
			    printf("max memory control point1...\n");
			    getchar();
			    free_level2_temp(t);
			    printf("t.slau free memory 15N...\n");
			    getchar();
		    }
		    */

			// ���� ������ ��������� � Prism Object:
			// ��� ������������ ������������ ���� � ����.
			//radiosity_patch_for_vacuum_Prism_Object(t.slau, t.slau_bon, b, lb, t.maxelm);
			
			
		 
	}

	


	/*if (iVar==PAm) {
		printM_and_CSIR(sparseM, maxelm); 
	    getchar();
	    printf("\n");
	}// */ // debug 


    // ���� ������� ����:
	// ������� �������� ������ �.�. ���������� ������� �������� � ��� ICCG ������ ��� �������� ��������,
	// � ���� ������ ������ ����� ������� ������������ ������� ����������� ����.

    //1. ������ ������ ������� ����:

    // 1.1. ��� ������� ������.

	// ������ ���� ������� ���������� ����������
	// ��� ������������ ������������ ����������� ������� s.
	// ��� ����� ������������� �������. � ��� ���� ����� 
	// ���������� ��� ����� ������� ���������� ������.
	//eqsolv_simple_holesskii(s, nodes, rthdsd, potent);
	// ������ ���� ������� ���������� ������
	// ��� ������ �������� �������� � ��� 
	// ����� ������������� �������.
	//eqsolve_simple_gauss(s, nodes, rthdsd, potent[Temp]);

    // 1.2. ��� ����������� ������.

	// ����� �.�. ������ ��� ����������� �������� �������������� �������
    //calculateSPARSEgaussArray(&sparseS, potent, rthdsd);


	//2. ������������ ������ ������� ����:

	// 2.1. ��� ������� ������.

	// ����� ������-�������-����������-������� SOR
	// ��� ������������� ������� ����.
	// ����� ��������������� ��������� �����������.
	//Seidel(s, rthdsd, potent, nodes, 1e-5, 1.855);
	// ������ ������������� ���� �������
	// ���������� ����������.
	// ��� ������������ ������������ ����������� ������� s.
	// ����� �������� ����������, �.�. ���� �������� ������
	// ���������� ����������� �� ��� ����� ��������� ������ NULL.
	//potent=SoprGrad(s, rthdsd, NULL, nodes);
		
    // 2.2. ��� ����������� ������.

	// 2.2.1 ��� ��� CRS (CSIR, CSIR_ITL) ������� ������������ ������.
    ///*
    //simplesparsetoCRS(sparseM, val, col_ind, row_ptr, nodes);
	//printM_and_CRS(sparseM,val,col_ind,row_ptr,nodes);
    //potent=SoprGradCRS(val, col_ind, row_ptr, rthdsd, NULL, nodes);//*/

	

	// ��� (Congruate Gradients) ��� CSIR ������� ��������
	// ������������ ������������ ����������� ������.
    //simplesparsetoCSIR(sparseM, adiag, altr, jptr, iptr, nodes);
	//printM_and_CSIR(sparseM, adiag, altr, jptr, iptr,  nodes);
	//integer inz=(int)((sparseM.n-nodes)/2.0);
	//potent=SoprGradCSIR(adiag, altr, jptr, iptr, rthdsd, NULL, nodes, inz);
	//potent=SoloveichikAlgCSIR_SPD(nodes, adiag, altr, jptr, iptr, rthdsd, NULL, true);
	//potent=SoloveichikAlgCSIR_SPDgood(nodes, inz, adiag, altr, jptr, iptr, rthdsd, NULL, true);
	if (iVar==PAM) {

		
         //for (integer i=0; i<230000; i++) { // debug
		// ����� ������������� ���� 9 ������� 2016 ����.
		// ����������� ����� ������������� ����� � ������� ������ ���������� ���� ��������.
		 for (i=0; i<(f.maxelm+f.maxbound); i++) f.potent[PAM][i]=0.0;
		 integer maxiter=2600; //2000 120
		 doublereal *val=NULL;
         integer *col_ind=NULL, *row_ptr=NULL;

		 if (!bBiCGStabSaad) {
			
		    freeIMatrix(&sparseS); // ������������ ����������� ������.
		    simplesparsetoCRS(sparseM, val, col_ind, row_ptr, (f.maxelm+f.maxbound)); // �������������� ������� �� ������ ������� �������� � ������.
			simplesparsefree(sparseM, (f.maxelm + f.maxbound));
		 }
		 else {
			 if (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2) {
				 /*Lr1sK_up
				 freeIMatrix(&sparseS); // ������������ ����������� ������.
				 simplesparsetoCRS(sparseM, val, col_ind, row_ptr, (f.maxelm + f.maxbound)); // �������������� ������� �� ������ ������� �������� � ������.
				 simplesparsefree(sparseM, (f.maxelm + f.maxbound));
				 if (val == NULL) {
					 // �������.
					 printf("NULL pointer val\n");
					 getchar();
				 }
				 */
			 }
		 }

		 
		
		 // ������������ ���������� ������������ ��������� �������� �� �����:
		 // ������������ �� ������ ���� ��� ����� ��������� ���������� ����� ����� � ������������.
		 // ���� �������, �� ������� �� ����������� SOR3DS ��� ����� ������� �����������. 
		 // ��� ����� ������� ����������� ������ �������� MICCGS - ���������� ��������� � ������������������� 
		 // �������� ����������� ���������. ������, �������� (��� ������� ��������), ��� ����� ��� ������� �����������
		 // ������ ����� LR1sk ��������. LR1sk - �������� �� ������� ����� �������� ��� MICCG ������ ������ ��� �������� ����� ����������.
		 // � ��������� ����� ������������ ������� LR1sk Solver. LR1sk - ������ ��� ������������ �� �������� �������.
		 const integer SOR3DS=0; // SOR3D Solver - ����� ���������������� ������� ���������� � 3D ������������. // ������� ��������� ��� ����� ������� �����������.
		 const integer MICCGS=1; // Modify Incomplete Cholesky Congruate Gradient SOLVER // ������������� ������
		 const integer LR1SKS=2; // LR1sk - �.�. �����, �.�. ������ // ������������ �����
		 const integer LRN=3; // ������ ������ ��������� �� ���� ������� ����������������� ���������� ������� ������� �������, ��� ����� ����� ��� ����������. � ���� �� ������ ������ ������������ �.����������.
		 const integer BICGSTAB=4;
		 const integer BICGSTABP=5; // ����������������� �������� ��� ��� ������. ������������������� �� ������ SPARSKIT2.
		 integer iSOLVER=MICCGS;//LR1SKS; //MICCGS; // ����� ��������� ���������.

		 if (bBiCGStabSaad) {
			 iSOLVER=BICGSTABP;
		 }

		 // � ��������� �� CFX ����������� ������� ������������������ ������� ������ 1.0e-4.
		 // �� ��������� ������������� � ������ ���� ���� ����������� ��� ������� ������������������ ������� ������ ���� ����� 1.e-5 
		 dterminatedTResudual=3.0e-6; //f.resICCG; // O(h!2) 1e-40

		 //printf("Ok\n");
		 //getchar();
		 
		 //SOR3D(f.slau[PAM], f.slau_bon[PAM], f.potent[PAM], f.maxelm, f.maxbound, PAM);
		 if (f.bLR1free)
		 {

			 if (0) {
		         // ������� ������������� ������� � ��� �����
		         // ������� �������� �������� �������� ���� ����� ����.

		         //printf("regularization condition for Pamendment...\n");
		         //printf("please, press any key to continue...\n");
		         //getchar();

		         // ������ �.�. ����������, �.�.�������, �.�.���������
		         // ����� ������������ ������� ������������� ��� 
		         // ����������� ���� �������� �������� � ��� ������
		         // ���� �� ���� ������� ��������� ������� ����� 
		         // ���������� ������� �������.
	             // ������� �������������:
	             doublereal pamscal1=0.0;
			     doublereal Vol=0.0;
	         
			     // ������� ����������� ������������.
		         for (integer iP1=0; iP1<(f.maxelm); iP1++) {
		             // ���������� �������� �������� ������������ ������:
	                 doublereal dx=0.0, dy=0.0, dz=0.0;// ����� �������� ������������� ������
	                 volume3D(iP1, f.nvtx, f.pa, dx, dy, dz);
				     Vol+=dx*dy*dz;

			         pamscal1+=rthdsd[iP1]*dx*dy*dz;     
		         }
	
			     pamscal1=(doublereal)(pamscal1/(f.maxelm*Vol));
			     for (integer iP1=0; iP1<(f.maxelm); iP1++) {
				     rthdsd[iP1]-=pamscal1;
			     }

			     // ����� �� ���� ������� ����� ������� �������
			     // �� �������� ���������� ����������� � ����� �����
			     // ��� ���� ����� ��������� ����� ������ ��� ICCG(0) � LR1sk.
			 }
			
			 // ����� ������ ������� ������� �������� ����� �������� �� ����������� �� � ����� �����.
            //SOR3D(f.slau[PAM], f.slau_bon[PAM], f.potent[PAM], f.maxelm, f.maxbound, PAM);

			 //printf("bLR1free Ok\n");
		     //getchar();

			 // ��������� ������������ ������ ���������� ����� ������� ����������.
			 //solveLRn(f.potent[PAM], rthdsd, (f.maxelm+f.maxbound), PAM, 20, true, true, f.sosedi, f.maxelm, f.slau, f.slau_bon, f.iN, f.id, f.iWE, f.iSN, f.iBT, f.alpha, f.maxbound);
			 // ������������ ���������� �������� ������������� ������ ����������� ������
			 // 4*Nequations, ��� Nequations - ����� �������� ���������. 

			 // ICCG solver �������� � ��� �������� ������� �� ���� ������� ��������� ������� ???
			 
			 bool bexporttecplot=false;
			 bool bprintPAM=false;
			 bool bnorelaxPAM=true;
			 //iSOLVER=SOR3DS; // debug TODO

			 switch (iSOLVER) {
			 case MICCGS : // ����� ���������� ���������� � ������������������� �������� ����������� ����������.
				           delete val; delete col_ind; delete row_ptr;
				           //printf("Ok!\n"); // debug
				           //getchar();
			               ICCG(iVar, sparseM, rthdsd, f.potent[PAM], f.maxelm + f.maxbound ,bprintmessage,false,maxiter); //->//
		                   // ��� ������������� ICCG ������ �� ��� sparseM ������������� ���������
				          break;
			 case LR1SKS : // �������� ����� � ������ (BiCGStab ��������� ������������ �������). 
				
				           simplesparsefree(sparseM,(f.maxelm+f.maxbound));
		                   //->//->//solveLRn(f, f.potent[PAM], rthdsd, (f.maxelm+f.maxbound), PAM, maxiter, true, true); // ������������ ������ ��� �������� ��������

						   // �.�. �����, �.�. ������
		                   // ��������� ������������� ������������� ������ � ���������������� �������.
                           // ������� �������� ���������������� ������������. ���������� � �������� �2(14) 2011���.
		                   LR1sK(f, f.slau[PAM], f.slau_bon[PAM], val, col_ind, row_ptr, f.maxelm, f.maxbound, PAM, rthdsd, f.potent[PAM], maxiter, bprintPAM, bexporttecplot);
			               delete val; delete col_ind; delete row_ptr;

				           if (bexporttecplot) {
					           // ������� � ��������� tecplot360 
					           // � ������ ����������� ������������.
					           exporttecplotxy360T_3D_part2(t.maxelm,t.ncell, fglobal, t, flow_interior,i,false,0);
				           }
				          break;
			 case LRN : // ������������ ����� ��������������� ����������.
				        simplesparsefree(sparseM, (f.maxelm+f.maxbound));
						delete val; delete col_ind; delete row_ptr;
				        maxiter=10; // � CFX ������� ��� 10 �������� ������ �������.
						
						solveLRn(f.potent[PAM], rthdsd, (f.maxelm+f.maxbound), PAM, maxiter,
							     bprintPAM, bnorelaxPAM, f.sosedi, f.maxelm, f.slau, f.slau_bon,
								 f.iN, f.id, f.iWE, f.iSN, f.iBT, f.alpha, f.maxbound); // ������������ ������ ��� �������� ��������				        
				       break;
		     case BICGSTAB : // �� ���������� ��������� �� �������������. ������ �������� ��� ������-���� ������������������.
			                // � ������ ����� CRS ������ ��������.
							simplesparsefree(sparseM,(f.maxelm+f.maxbound));
				            Bi_CGStabCRS((f.maxelm+f.maxbound), val, col_ind, row_ptr, rthdsd, f.potent[PAM], maxiter);//->//
							
							delete val; delete col_ind; delete row_ptr;
				          break;
			 case SOR3DS : // ������ �������� ������������ ��� ���������� ��� �������� ������������ ������ �������.
						   if (!bBiCGStabSaad) {
				               simplesparsefree(sparseM,(f.maxelm+f.maxbound));
						       delete val; delete col_ind; delete row_ptr;
						   }
				           SOR3D(f.slau[PAM], f.slau_bon[PAM], f.potent[PAM], NULL, f.maxelm, f.maxbound, PAM, 1.0);
						 break;

			 case BICGSTABP : // ���������������� �������� ��� ��� ������.
				          if (!bBiCGStabSaad) {
				              delete val; delete col_ind; delete row_ptr;
						      simplesparsefree(sparseM,(f.maxelm+f.maxbound));
						  }
						 Bi_CGStab(&sparseS, f.slau[PAM], f.slau_bon[PAM], f.maxelm, f.maxbound, rthdsd, f.potent[PAM], maxiter, 1.0,PAM, m, f.bLR1free,b,lb,f.ifrontregulationgl,f.ibackregulationgl);
				 break;
			 }
			
		 }
		 else {

			 bool bexporttecplot=false;
			 bool bprintPAM=false;
			 bool bnorelaxPAM=true;

			 switch (iSOLVER) {
			 case MICCGS : // ����� ���������� ���������� � ������������������� �������� ����������� ����������.
				           delete val; delete col_ind; delete row_ptr;
				           //printf("Ok!\n"); // debug
				           //getchar();
			               ICCG(iVar, sparseM, rthdsd, f.potent[PAM], f.maxelm + f.maxbound ,bprintmessage,false,maxiter); //->//
		                   // ��� ������������� ICCG ������ �� ��� sparseM ������������� ���������
				          break;
			 case LR1SKS : // �������� ����� � ������ (BiCGStab ��������� ������������ �������). 
				
				           simplesparsefree(sparseM,(f.maxelm+f.maxbound));
		                   //->//->//solveLRn(f, f.potent[PAM], rthdsd, (f.maxelm+f.maxbound), PAM, maxiter, true, true); // ������������ ������ ��� �������� ��������
						  
		                   // �.�. �����, �.�. ������
		                   // ��������� ������������� ������������� ������ � ���������������� �������.
                           // ������� �������� ���������������� ������������. ���������� � �������� �2(14) 2011���.
		                   LR1sK(f, f.slau[PAM], f.slau_bon[PAM], val, col_ind, row_ptr, f.maxelm, f.maxbound, PAM, rthdsd, f.potent[PAM], maxiter, bprintmessage, bexporttecplot);
			               delete val; delete col_ind; delete row_ptr;

				           if (bexporttecplot) {
					           // ������� � ��������� tecplot360 
					           // � ������ ����������� ������������.
					           exporttecplotxy360T_3D_part2(t.maxelm,t.ncell, fglobal, t, flow_interior,i,false,0);
				           }
				          break;
			 case LRN : // ������������ ����� ��������������� ����������.
				        simplesparsefree(sparseM, (f.maxelm+f.maxbound));
						delete val; delete col_ind; delete row_ptr;
				        maxiter=20; // � CFX ������� ��� 10 �������� ������ �������.
						
						solveLRn(f.potent[PAM], rthdsd, (f.maxelm+f.maxbound), PAM, maxiter,
							     bprintPAM, bnorelaxPAM, f.sosedi, f.maxelm, f.slau, f.slau_bon,
								 f.iN, f.id, f.iWE, f.iSN, f.iBT, f.alpha, f.maxbound); // ������������ ������ ��� �������� ��������				        
				       break;

			 case BICGSTABP : 
				       if (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2) {
					       // LR1SK
					       iVar = PAM;
						   Lr1sk_up(f, t,  f.slau[PAM], f.slau_bon[PAM], f.maxelm, f.maxbound, rthdsd, f.potent[PAM], maxiter, 1.0, iVar, false);//->//

						   
					       //LR1sK(f, f.slau[PAM], f.slau_bon[PAM], val, col_ind, row_ptr, f.maxelm, f.maxbound, iVar, rthdsd, f.potent[PAM], maxiter, bprintmessage, bexporttecplot);//->//
					       //freeIMatrix(&sparseS);
					       if (val != NULL) {
						       delete val;
						       val = NULL;
					       }
					       if (col_ind != NULL) {
						       delete col_ind;
						       col_ind = NULL;
					        }
					       if (row_ptr != NULL) {
						      delete row_ptr;
						      row_ptr = NULL;
					       }
						   
				        }
				        else {
					        // ���������������� �������� ��� ��� ������.
					        if (!bBiCGStabSaad) {
						       delete val; delete col_ind; delete row_ptr;
						       simplesparsefree(sparseM, (f.maxelm + f.maxbound));
					        }
					        Bi_CGStab(&sparseS, f.slau[PAM], f.slau_bon[PAM], f.maxelm, f.maxbound, rthdsd, f.potent[PAM], maxiter, 1.0, PAM, m, f.bLR1free, b,lb, f.ifrontregulationgl, f.ibackregulationgl);
				        }
						break;
			 
			 }

			 
		 }
		   

		 // ������ ����������:

         
		 //for (i=0; i<(f.maxelm+f.maxbound); i++) f.potent[PAM][i]=0.0;
			 //SOR3D(f.slau[PAM], f.slau_bon[PAM], f.potent[PAM], NULL, f.maxelm, f.maxbound, PAM, 1.0);
			//BTrules(f.slau[PAM], f.slau_bon[PAM], f.potent[PAM], NULL, f.maxelm, f.maxbound, PAM, 1.0); // BT ������� � �����������.

		 // ���� �����������. ����� ��������� ������ ��� ��� ���.
		 // ����� ������ ������� ������� �������� ����� �������� �� ����������� �� � ����� �����.
         //SOR3D(f.slau[PAM], f.slau_bon[PAM], f.potent[PAM], f.maxelm, f.maxbound, PAM);

		 // getchar(); // debug pressure solver...

         // ������������ ������
	    // delete rthdsd;
		//}
	}
	//*/

	// 2.2.2 ������ ��� CRS (CSIR, CSIR_ITL) ������� �������� �������������� ������.
	if (iVar!=PAM) {
		// ����������� ������� � ������� CRS
        doublereal *val=NULL;
        integer *col_ind=NULL, *row_ptr=NULL;

		if (iVar != TEMP) {

			

		    if (!bBiCGStabSaad) {
			    simplesparsetoCRS(sparseM, val, col_ind, row_ptr, (f.maxelm+f.maxbound));
                simplesparsefree(sparseM,(f.maxelm+f.maxbound));
			}
			else {
				if (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2) {
					/*LR1sK_up
					freeIMatrix(&sparseS);
					simplesparsetoCRS(sparseM, val, col_ind, row_ptr, (f.maxelm + f.maxbound));
					simplesparsefree(sparseM, (f.maxelm + f.maxbound));
					if (val == NULL) {
						printf("Null pointer val\n");
						getchar();
					}
					*/
				}
			}

			

			// ������� ������������ ������������ �������.
			// ��� ������ LR1SK ������� ������������ � ������������� ������������ ���������� ��������
			// ���������� ������ ��������� ���������. �.�. ������ ����� �������� ������� �����������, ��
			// ������ �� ������ �������� ����������� ������� ���������� �������������� �������� ��� �����
			// ������� ������, ��������, �������� ����������� ��� ������������������.
	        integer maxiter=320; //120
			// limititer - ������������ ���������� �������� ��� �������� ������ (��� �������������������). �������
			// ����� ������������ ��� ������� ��� ������������������ � �� ������ �������� ��������� ���� ��������������
			// �������� ������� �������� ������ ��������� ���������� � ��� ���������� ������ ���������� ������� ������ ��������.
			const integer limititer=1000; // 120 � ����������� ������������ ������� ������������ ����������� ���������� ��������.

	        
			// �������� ������������� ��������� ����:
			const integer SOLOVEICHIK=0; // �������� �����������.
			const integer SOR3DALG=1; // ����� ���������� ���������� �� ������ ������-�������.
			const integer SOLVELRN=2; // ������������ �����.
			const integer BICG=3; // ������������ ���������.
			const integer BICGSTAB=4; // �������� ���-���-������
			const integer LR1SK=5; // �����-������ ������ �������� ������������ � �������������.
			const integer BICGSTABP=6;// ����������������� �������� ��� ��� ������. ������������������� �� ���������� SPARSKIT2.

			integer iSOLVER=LR1SK; // LR1SK ����� ��������� ����������.

			if (bBiCGStabSaad) {
				iSOLVER=BICGSTABP;
			}

			// ����� ������������������� :
			const integer NOPRECOND=0; // ��� ������������������.
			const integer ILU0ITL=1; // �������� LU ���������� ������ 0 ������������� � ���������� ITL.
			const integer ILU0SAAD=2; // �������� LU ���������� ������ 0 ��������� � ����� �.�����.

			integer iPRECOND=ILU0SAAD; // ����� �������������������.

			 /*switch (iSOLVER) {
		        case BICGCRS : potent[iVar]=BiSoprGradCRS(val, col_ind, row_ptr, rthdsd, potent[iVar], maxelm, maxiter); break;
		        case SOLOVALGCRS : SoloveichikAlgCRS(maxelm, val, col_ind, row_ptr, rthdsd, potent[iVar], true, maxiter); break;
		        case BICGSTABCRS : potent[iVar]=Bi_CGStab(maxelm, val, col_ind, row_ptr, rthdsd, potent[iVar], maxiter); break;
		      default : potent[iVar]=Bi_CGStab(maxelm, val, col_ind, row_ptr, rthdsd, potent[iVar], maxiter); break;
	        }*/

			//dterminatedTResudual=1e-40*f.resLR1sk; // O(h!3) // 1e-16 ������������ ������������ ����� (��� ���� ���� ����� �����-������ ������������� ������).
			// � ����� ��������� �� CFX �������, ��� ������ �������������������� ������� 1.e-4 ������ �������. ���� ���� ����������� ����������������
			// �� ������� ��������� ������������� ��� ���������� ������� 1.0e-5.
			dterminatedTResudual=1.0e-5;

			// � ������ ����������� ������� �� ����������� ����� ��������� �������
			// � ��������� tecplot360.
		    bool bexporttecplot=false;
			integer iVarCor=VXCOR; // ������������� ����������������� �������� � ������� ����� �������������� ������ ����������.

			

			switch (iSOLVER) {
			case SOLOVEICHIK : // �������� �.�.����������� 1993 ����.
				         switch(iPRECOND) {
						 case NOPRECOND: // �������� �.�.����������� 1993 ����. ��� �������������������.
							           maxiter=limititer;
                                       SoloveichikAlgCRS((f.maxelm+f.maxbound), val, col_ind, row_ptr, rthdsd, f.potent[iVar], true, maxiter); // ��� �������������������.  
									    freeIMatrix(&sparseS);
							           break;
						 case ILU0ITL: // �������� �.�.����������� 1993 ����. ������� ILU0 �������������������. ������ �������� ��� ����������� ������ �� ��� sparseS.
							           maxiter=limititer;
			                           SoloveichikAlg(&sparseS, f.slau[iVar], f.slau_bon[iVar], f.maxelm, f.maxbound, rthdsd, f.potent[iVar], true, false, maxiter, f.alpha[iVar]);
							            break;
						 case ILU0SAAD: // �������� �.�.����������� 1993 ����. ������� ILU0 �������������������. ������ �������� ��� ����������� ������ �� ��� sparseS.
							           maxiter=limititer;
			                           SoloveichikAlg(&sparseS, f.slau[iVar], f.slau_bon[iVar], f.maxelm, f.maxbound, rthdsd, f.potent[iVar], true, true, maxiter, f.alpha[iVar]);
							            break;
						 default : // �������� �.�.����������� 1993 ����. ������� ILU0 �������������������. ������ �������� ��� ����������� ������ �� ��� sparseS.
							       // ilu0 �� ����� � �����.
							       maxiter=limititer;
			                       SoloveichikAlg(&sparseS, f.slau[iVar], f.slau_bon[iVar], f.maxelm, f.maxbound, rthdsd, f.potent[iVar], true, true, maxiter, f.alpha[iVar]);
							       break;
						 }
				         break;
			case SOR3DALG : 
				
				         
				         switch (iVar) {
				            case VX : iVarCor=VXCOR; 
					                  break;
				            case VY : iVarCor=VYCOR;
					                  break;
				            case VZ : iVarCor=VZCOR;
					                  break;
				         }
				         // ����� ������-������� ������������ ������� ������� ��� �������� ������������ ������ �������� ����.
				         // ������ ����� �������� ������ ������ ��������� ���������� � �� ������������ � �������������.
				         // ���� �� ��������� ����� ��� ���������� �������� ������������ ������ ������� ����.
						 SOR3D(f.slau[iVar], f.slau_bon[iVar], f.potent[iVar], f.potent[iVarCor], f.maxelm, f.maxbound, iVar, f.alpha[iVar]);
						 freeIMatrix(&sparseS);
				         break; 
			case SOLVELRN : // ������������ �����
				          // ������ ����� ������� ��������� ����� ��������, �� ������������ � ������������� �. ����������. �� �����
				          // LR1sk �������� ������� ������ �����������.
				          solveLRn(f.potent[iVar], rthdsd, (f.maxelm+f.maxbound), iVar, maxiter, bprintmessage, false, f.sosedi, f.maxelm, f.slau, f.slau_bon, f.iN, f.id, f.iWE, f.iSN, f.iBT, f.alpha, f.maxbound);
						  freeIMatrix(&sparseS);
				         break;
			case BICG :  // �������� ������������ ����������.
				         // ��������� �������� ���������� ������ ilu0 ����������
			             // ���� true �� ������������ �������� �� ����� �. �����
			             // �������� : ilu0 �� ����� ����� �������� ������ ��� ������ � ������������ ���������.
			             // ����������� ���������� ������� ������ � ��� ��� �� ��� ����������� ������ �� ��� sparseS
				         switch(iPRECOND) {
						 case NOPRECOND : maxiter=limititer; 
							             BiSoprGradCRS(val, col_ind, row_ptr, rthdsd, f.potent[iVar], (f.maxelm+f.maxbound), maxiter); 
							             freeIMatrix(&sparseS);  
							             break; // bug ?// � ��������������������:
                           case ILU0ITL: maxiter=limititer;
							             BiSoprGrad(&sparseS, f.slau[iVar], f.slau_bon[iVar], rthdsd, f.potent[iVar], f.maxelm, f.maxbound, false, f.alpha[iVar], maxiter);  
										 break;
						   case ILU0SAAD: maxiter=limititer;
							             BiSoprGrad(&sparseS, f.slau[iVar], f.slau_bon[iVar], rthdsd, f.potent[iVar], f.maxelm, f.maxbound, true, f.alpha[iVar], maxiter); 
										 break;
						   default : maxiter=limititer;
							         BiSoprGrad(&sparseS, f.slau[iVar], f.slau_bon[iVar], rthdsd, f.potent[iVar], f.maxelm, f.maxbound, true, f.alpha[iVar], maxiter);
									 break;
						 }
				         break;
			case BICGSTAB : // �� ���������� ��������� �� �������������. ������ �������� ��� ������-���� ������������������.
			                // � ������ ����� CRS ������ ��������.
				           
				              	maxiter = limititer;
				             	Bi_CGStabCRS((f.maxelm + f.maxbound), val, col_ind, row_ptr, rthdsd, f.potent[iVar], maxiter);//->//
					            freeIMatrix(&sparseS);
				            							
				          break;
			case LR1SK : 
				
				         // �.�. �����, �.�. ������
			             // ��������� ������������� ������������� ������ � ���������������� �������.
                         // ������� �������� ���������������� ������������. ���������� � �������� �2(14) 2011���.
				
			             LR1sK(f, f.slau[iVar], f.slau_bon[iVar], val, col_ind, row_ptr, f.maxelm, f.maxbound, iVar, rthdsd, f.potent[iVar], maxiter,bprintmessage, bexporttecplot);//->//
						
						 freeIMatrix(&sparseS);

						 if (bexporttecplot) {
							 // ������� � ��������� tecplot360 
					         // � ������ ����������� ������������.
					         exporttecplotxy360T_3D_part2(t.maxelm,t.ncell, fglobal, t, flow_interior,i,false,0);
						 }

				      break;
			case BICGSTABP :
				      if (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2) {
						  
					      // LR1SK
					     // LR1sK(f, f.slau[iVar], f.slau_bon[iVar], val, col_ind, row_ptr, f.maxelm, f.maxbound, iVar, rthdsd, f.potent[iVar], maxiter, bprintmessage, bexporttecplot);//->//
						  Lr1sk_up(f, t, f.slau[iVar], f.slau_bon[iVar], f.maxelm, f.maxbound, rthdsd, f.potent[iVar], maxiter, f.alpha[iVar], iVar, false);//->//

						  
						  //freeIMatrix(&sparseS);
						  if (val != NULL) {
							  delete val;
							  val = NULL;
						  }
						  if (col_ind != NULL) {
							  delete col_ind;
							  col_ind = NULL;
						  }
						  if (row_ptr != NULL) {
							  delete row_ptr;
							  row_ptr = NULL;
						  }
				      }
				      else {

				      	if (!bBiCGStabSaad) {
						    delete val; delete col_ind; delete row_ptr;
						    val = NULL; col_ind = NULL; row_ptr = NULL;
					    }
				     	// ������������ ������ �� ��� sparseS ���������� ������ ������ Bi_CGStab.
					    //dterminatedTResudual=1e-6;
					    Bi_CGStab(&sparseS, f.slau[iVar], f.slau_bon[iVar], f.maxelm, f.maxbound, rthdsd, f.potent[iVar], maxiter, f.alpha[iVar], iVar, m, false,b,lb, f.ifrontregulationgl, f.ibackregulationgl);
				      }
				      break;
			default : // �������� ��������������� �� ���������.
				      // �.�. �����, �.�. ������
			          // ��������� ������������� ������������� ������ � ���������������� �������.
                      // ������� �������� ���������������� ������������. ���������� � �������� �2(14) 2011���.
			          LR1sK(f, f.slau[iVar], f.slau_bon[iVar], val, col_ind, row_ptr, f.maxelm, f.maxbound, iVar, rthdsd, f.potent[iVar], maxiter,bprintmessage, bexporttecplot);//->//
			          freeIMatrix(&sparseS);
					  if (bexporttecplot) {
							 // ������� � ��������� tecplot360 
					         // � ������ ����������� ������������.
					         exporttecplotxy360T_3D_part2(t.maxelm,t.ncell, fglobal, t, flow_interior,i,false,0);
					 }

				      break;
			}

			if (!bBiCGStabSaad) {
			   if (val!=NULL) {
			      delete val; 
			   }
			   if (col_ind!=NULL) {
			      delete col_ind;
			   }
			   if (row_ptr!=NULL) {
				   delete row_ptr;
			   }
			}
			// ������������ ������
	       // delete rthdsd;
			//getchar(); // debug
			
		}
		else {

			// ������������ �������� ����������� ������ �� ���� ����������� ������������ ����� ��������
			// � ����������� �� ��������������� � ����. 
			// �������� ������ ��� ����������������� �����.

			bool bmemory_saving = true; // true

			if (!bonly_solid_calculation) {
                // ������ � ������ ������� ������ ������ �������������. 
				bmemory_saving = false;
			}

			if (b_on_adaptive_local_refinement_mesh) {
				// ������ ��� ����������������� �����.
				bmemory_saving = false; 
			}

			if (ireconstruction_free_construct_alloc == 0) {
				// 8 september 2017.
				// ���� ������������ ����� ��� ���������.
				// ��� ������������� ��������� � ������ ��������� ���������� 
				// ��������� ����� �������� ��� ������ � ����� ����� ���� �� 
				// ���������� ��������, ������ ������� ����������� ������������� 
				// �������� �� �� �� �������� �.�. ������ ��� ���� ���������� ���������� ��������.
				// ����� ����������������� � ����� �� �����. // TODO �����������������.
				bmemory_saving = false;
			}

			


			// iVar==TEMP
			if (!bBiCGStabSaad) {
			    simplesparsetoCRS(sparseM, val, col_ind, row_ptr, (t.maxelm+t.maxbound));
                simplesparsefree(sparseM,(t.maxelm+t.maxbound));
			}
            
			integer maxiter=4000; // 10 4000 ��� �������� �������������� ������.

			// �������� ������������� ��������� ����:
			const integer NOSOLVE=7; // �� ������ ��������� ����������������.
			const integer SOLOVEICHIK=0; // �������� �����������.
			const integer SOR3DALG=1; // ����� ���������� ���������� �� ������ ������-�������.
			const integer SOLVELRN=2; // ���������� �����.
			const integer BICG=3; // ������������ ���������.
			const integer BICGSTAB=4; // �������� ���-���-������
			const integer LR1SK=5; // �����-������ ������ �������� ������������ � �������������.
			const integer BICGSTABP=6; // ����������������� �������� ��� ��� ������. ������������������� �� SPARSKIT2.
 
			integer iSOLVER=NOSOLVE; // LR1SK ����� ��������� ����������.
			if (inumiter>100) {
				// ����� �� ��������� ��������� ����.
				// ���� ����� �������� ������ ������������� �� ������ ��������� SIMPL`�
				// ��������� �� ��� ������ ������������ (����� �������� ����������) � �������� ����� ��������
				// � ������������. ���� ����� ������� ��������� ��������������� ��������� ���� ��������
				// � "����� �� ��������" �������� ��� ����� ���������������.
				 maxiter=1000; // 120
                 iSOLVER=LR1SK;
			}
			iSOLVER=LR1SK;
			if (bBiCGStabSaad) {
			   iSOLVER=BICGSTABP;//LR1SK;//BICGSTAB;
			}

			if ((iSOLVER == BICGSTABP) && (iVar == TEMP)&&(iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 2)) {
				// � ������ LR1sk ������� ����������� ������ ������.
				// LR1sk ���������� � ���������� ����� ������ t.sosedi, t.slau_bon, t.slau.
				// ������� ����������� �� ��� ��� ������ ������.
				bmemory_saving = false;
			}

			if (bmemory_saving) {
				// ������������ ��� �������� ������ ������� �� ������������ � ������ ������� ����.
				free_level1_temp(t);
				free_level1_flow(fglobal, flow_interior);
			}

			// ��� ���������� ������ �������� ������� �������������������� ������� 1.0e-4
            // ���� ����� �������� �� ������� ���������� ����� 1.0e-3.
            // �������� ����� �� ������ �� CFX.
			// ������ ��� ������ ��������� ������ ���������������� �����������,
			// ��� ����������� ������� ������������������� ������� ������ 1.0e-5,
			// � ������� 1.0e-4 ������������. (��� ����� 287 ��������).
			dterminatedTResudual=1e-7; //t.resLR1sk; // O(h!3)
			 doublereal alpharelaxmethod=1.0; // �������� ����������.

			// � ������ ������������� ������� �� �����������
			// ����� ��������� ������� � ��������� tecplot360.
			bool bexporttecplot=false;

			if (bconvective) {
				
				switch (iSOLVER) {
				case SOLVELRN : freeIMatrix(&sparseS);
					            delete val; delete col_ind; delete row_ptr;
					            solveLRn_temp(t, t.potent, rthdsd, (t.maxelm + t.maxbound), maxiter, bprintmessage); // ������������ �����
					           break;
				case BICGSTAB : freeIMatrix(&sparseS);
					            Bi_CGStabCRS((t.maxelm+t.maxbound), val, col_ind, row_ptr, rthdsd, t.potent, maxiter);
                                delete val; delete col_ind; delete row_ptr;
					            break; 
				case BICG :  // ��������� �������� ���������� ������ ilu0 ����������
			                 // ���� true �� ������������ �������� �� ����� �. �����
			                 // �������� : ilu0 �� ����� ����� �������� ������ ��� ������ � ������������ ���������.
			                 // ����������� ���������� ������� ������ � ��� ��� �� ��� ����������� ������ �� ��� sparseS
			                 // bug ?//
				             // 9-�� �������� ����������� ���������� == 1.0.
			                 // bug ?//
					         delete val; delete col_ind; delete row_ptr;
					         BiSoprGrad(&sparseS, t.slau, t.slau_bon, rthdsd, t.potent, t.maxelm, t.maxbound, false, 1.0, maxiter);
							 freeIMatrix(&sparseS);
					        break;
				case LR1SK :  freeIMatrix(&sparseS);				

				              // �.�. �����, �.�. ������
			                  // ��������� ������������� ������������� ������ � ���������������� �������.
                              // ������� �������� ���������������� ������������. ���������� � �������� �2(14) 2011���.
			                  LR1sK_temp(t, t.slau, t.slau_bon, val, col_ind, row_ptr, t.maxelm, t.maxbound, rthdsd, t.potent, maxiter, inumiter,bprintmessage,bexporttecplot);
							  delete val; delete col_ind; delete row_ptr;
					        break;	
				case BICGSTABP : if (!bBiCGStabSaad) {
					                delete val; delete col_ind; delete row_ptr;
								 }
					            // ������������ ������ �� ��� sparseS ���������� ������ ������ Bi_CGStab.
				                dterminatedTResudual=1e-6;
								alpharelaxmethod = t.alpha; // 1.0
								if (maxiter<m.icount_vel) {
								    m.icount_vel=maxiter;
								}
								if (iswitchsolveramg_vs_BiCGstab_plus_ILU2==2) {
									dterminatedTResudual=1e-8;
									// ����������� ������ �������� ��� ��������� �������������.
									Lr1sk_up(f, t,  t.slau, t.slau_bon,t.maxelm, t.maxbound,   rthdsd, t.potent, maxiter, alpharelaxmethod, TEMP, false);
								}
								else {
									dterminatedTResudual=1e-6;
				                   Bi_CGStab(&sparseS, t.slau, t.slau_bon, t.maxelm, t.maxbound, rthdsd, t.potent, maxiter, alpharelaxmethod,TEMP, m, false, b,lb, t.ifrontregulationgl, t.ibackregulationgl);
								  // doublereal tmax = 0.0;
								  // for (integer i1 = 0; i1<t.maxelm + t.maxbound; i1++) tmax = fmax(tmax, fabs(t.potent[i1]));
								   //printf("apost solver : maximum temperature in default interior is %1.4e\n", tmax);
								   //getchar();
								}
					         break;
				case NOSOLVE : freeIMatrix(&sparseS);
					           delete val; delete col_ind; delete row_ptr; 
							   // �� ����� ������ ��������� ���������������� ��� ��� ��� ����� ��������� ���� ��� �����������.
							   // �������� ����� �� ������ ��������� ����� ����� � �� �������� ������ �����.
					         break;
				}
				
			}
			else 
			{
				// � ���������������� ������� ������ ������� 0 � ��������������������� �����.
				integer iwork_variant=0; // 0 or 1
				if (bBiCGStabSaad) {
					   iwork_variant=0; 
				}

				/*
				// �������� ������������ ������� ����:
				for (integer i=0; i<t.maxelm+t.maxbound; i++) {
					if (i<t.maxelm) {
					#if doubleintprecision == 1
						printf("numberCV=%lld ap=%1.4e ae=%1.4e aw=%1.4e an=%1.4e as=%1.4e at=%1.4e ab=%1.4e b=%1.4e\n",i,t.slau[i].ap,t.slau[i].ae,t.slau[i].aw,t.slau[i].an,t.slau[i].as,t.slau[i].at,t.slau[i].ab,t.slau[i].b);
					#else
						printf("numberCV=%d ap=%1.4e ae=%1.4e aw=%1.4e an=%1.4e as=%1.4e at=%1.4e ab=%1.4e b=%1.4e\n",i,t.slau[i].ap,t.slau[i].ae,t.slau[i].aw,t.slau[i].an,t.slau[i].as,t.slau[i].at,t.slau[i].ab,t.slau[i].b);
					#endif
					}
					if (i % 1==0) getchar();
				}
				*/
			
			//SOR3D(t.slau, t.slau_bon, t.potent, t.maxelm, t.maxbound);
			//ICCG(sparseM, rthdsd, t.potent, t.maxelm);
			
			//simplesparsetoCRS(sparseM, val, col_ind, row_ptr, t.maxelm);
			//BiSoprGradCRS(val, col_ind, row_ptr, rthdsd, t.potent, t.maxelm, maxiter);
			
			//equation3DtoCRS(t.slau, t.slau_bon, val, col_ind, row_ptr, t.maxelm, t.maxbound);
			//printf("max memory precalculation ...\n");
			//getchar();
			//free_level2_temp(t);
	        
            //SoloveichikAlgCRS(t.maxelm+t.maxbound, val, col_ind, row_ptr, rthdsd, t.potent, true, maxiter);
				// ������� ��� !
				/*
				freeIMatrix(&sparseS); // �������� !!! ����� ����������� ���������� ������ �� ��� sparseS, ����� ����� ������ ������
			    Bi_CGStabCRS(t.maxelm+t.maxbound, val, col_ind, row_ptr, rthdsd, t.potent, maxiter);
			    delete val; delete col_ind; delete row_ptr;
			//*/
			//


			  // ��������� �������� ���������� ������ ilu0 ����������
			  // ���� true �� ������������ �������� �� ����� �. �����
			     //->//BiSoprGrad(&sparseS, t.slau, t.slau_bon, rthdsd, t.potent, t.maxelm, t.maxbound, true);
			  //SoloveichikAlg(&sparseS, t.slau, t.slau_bon, t.maxelm, t.maxbound, rthdsd, t.potent, true, maxiter);
				if (iwork_variant==0) {
				   //*
				   // ������ ��� �������� �� �������� ��� ����� �������� ��������, ������� ��������� ��� ������ Bi_CGStabCRS.
				   // ������ ��������� ��������� � ���������� ������ ����.
					if (!bBiCGStabSaad) {
				       delete val; delete col_ind; delete row_ptr;
					}
				   doublereal alpharelaxmethod=t.alpha; // 1.0 �������� ����������.
				   // ������������ ������ �� ��� sparseS ���������� ������ ������ Bi_CGStab.
				  // dterminatedTResudual=1e-6;
                  dterminatedTResudual=1.0e-24; // ��� ������������� ���������� ����� 1.0�-12.
				   
				   if (iswitchsolveramg_vs_BiCGstab_plus_ILU2==2) {
					    dterminatedTResudual=1e-8;
						Lr1sk_up(f, t,  t.slau, t.slau_bon,t.maxelm, t.maxbound,   rthdsd, t.potent, maxiter, alpharelaxmethod, TEMP, false);
				   }
				   else {
					    dterminatedTResudual=1e-6;
				        Bi_CGStab(&sparseS, t.slau, t.slau_bon, t.maxelm, t.maxbound, rthdsd, t.potent, maxiter, alpharelaxmethod,TEMP, m, false,b,lb, t.ifrontregulationgl, t.ibackregulationgl);
				   }
				   //*/
				}
			  //free_level2_temp(t); // delete t.slau;
			  //freeIMatrix(&sparseS);
			     //-->//solveLRn_temp(t, t.potent, rthdsd, (t.maxelm + t.maxbound), maxiter, true); // ������������ �����

				if (iwork_variant==1) {
					// �������� ! ���� ��������� ������������ � ���� ��������, ��� ������ ����� ������������ ��� �������� �����.
					// ��������� ���� � ������� SPARSKIT2.
			       //*
			       // �.�. �����, �.�. ������
			       // ��������� ������������� ������������� ������ � ���������������� �������.
                   // ������� �������� ���������������� ������������. ���������� � �������� �2(14) 2011���.
				   // ������� ��� !
				   freeIMatrix(&sparseS); // �������� !!! ����� ����������� ���������� ������ �� ��� sparseS, ����� ����� ������ ������
			       LR1sK_temp(t, t.slau, t.slau_bon, val, col_ind, row_ptr, t.maxelm, t.maxbound, rthdsd, t.potent, maxiter, inumiter,bprintmessage,bexporttecplot);
                   delete val; delete col_ind; delete row_ptr; 
			       //*/ 
				}
			}

			if (bmemory_saving) {
				// �������������� ��� �������� ������ ������� �� ������������ � ������ ������� ����.
				bool bextendedprint = false; // ������ �� ��������� ����� ����������� �����.
				// ���� ��������� � t ������� ��������� inx, iny, inz.
				integer iCabinetMarker = 0;
				load_TEMPER_and_FLOW(t, fglobal, t.inx_copy, t.iny_copy, t.inz_copy,
					t.xpos_copy, t.ypos_copy, t.zpos_copy, flow_interior,
					b, lb, lw, w, s, ls, lu, my_union, t.operatingtemperature_copy, matlist,
					bextendedprint, dgx, dgy, dgz, b_on_adaptive_local_refinement_mesh, true, iCabinetMarker);

				for (integer iu = 0; iu < lu; iu++) {
					integer iup1 = iu + 1;
					load_TEMPER_and_FLOW(my_union[iu].t, my_union[iu].f,
						my_union[iu].inx, my_union[iu].iny, my_union[iu].inz,
						my_union[iu].xpos, my_union[iu].ypos, my_union[iu].zpos,
						my_union[iu].flow_interior,
						b, lb, lw, w, s, ls, lu, my_union, my_union[iu].t.operatingtemperature, matlist, bextendedprint,
						dgx, dgy, dgz, b_on_adaptive_local_refinement_mesh, true, iup1);
				}

			}

			// ������������ ������
			
	       // delete rthdsd;

			if (bexporttecplot) {
				// ������������ � ������� ������ � ��� ������ ���� �� �� ������� � �� �� ���� �����.
				if (!b_on_adaptive_local_refinement_mesh) {
					exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, i, false,0);
				}
			}
	        
		}
         
	}
	
	// ������ � ��������������� ILU ��������������������
	// �� ������������ ������� ����������� ������ ��������
	// �������� ���������� � ���� ������������.
	//potent=BiSoprGrad(&sparseS, sparseM,  rthdsd, NULL, nodes);
	// ��� ��������� �.�. ����������� ������������������ ��������� ���������� ?
	//potent[Temp]=SoloveichikAlg( &sparseS, sparseM, rthdsd, NULL, true);

} // solve

// � ������ �������� ��������� �� ����������� ���������� 
// ���������� �������� �������� ��������.
void update_power_temperature_depend(SOURCE* s, integer ls, TEMPER &t, BOUND* sosedb,
	                                 TEMP_DEP_POWER* gtdps, integer ltdp, doublereal* toldtimestep) {

	// ������� ����� ���� ����, � ���������� ����� ������������ ������ ������� ����� ���� ���������.
	// ��������� ������ ��������� ����� ����� ������ ����������� (�� ������������ ��� �������� �����
	// ��������������� ����� ����� ��������� �����������, ������������ ������������ �� ����������� ������� ��������� �����)
	// �� ������ ��������� ����� ��������������� ������ ��������� (��������� �� �����������).
	// ��� ������������� �����, ���� ���������� bmultipowersourse==true; ���� false �� ��� ���� ���������� �����
	// ������������ ������ ������� ����� ��������� ���� � ���� �������� ���������, ��������������� ������������ �����������
	// ����� ���� ���������� ����� ������������ ������ �������.
	bool bmultipowersourse=true; // true ������ �������� �� ����� ���������, false - � ������ ���������� ���������� ��������.

	// ��������� �������� ������������ ��� ����������� �������� �������� ��������� ������� ����� ����������� �� ������� ���� �� �������.
	// toldtimestep - ����������� �� ���������� ���� �� �������.
	// �� ������ ���� ���������� toldtimestep � ������� t.potent ����� ���������� ����� ������
	// �������� �������� ����������� �� ������� ��������� ����, ��� �������� �������������� ������ �� �������� t.potent.
	// ����� ���������� ��� �������� �� �������� ��������������� ���������� :
	// toldtimestep && t.potent. (����� ����� ������������ ������� ������������� ��������).
	const integer AREFM=0; // ������� ��������������.
	const integer GARMONIC=1; // ������� �������������.
	integer itypepowerdepend=GARMONIC;
	bool bavgpower=true; // ���� true �� �������� ��������� �� ������ �������� ��������������� ��� �������� �������������� ���� ����������. 

	doublereal Tmaxtable; // ������������ ����������� �� ����������� ��������� ����� ����� ���������� ����� ������������ ������ �������.
	doublereal* Tmaxtablei = NULL;
	Tmaxtablei=new doublereal[ls];
	doublereal Tmaxtableold; // ������������ ����������� �� ����������� ��������� ����� ����� ���������� ����� ������������ ������ ������� � ����������� ���������� ����.
	doublereal* Tmaxtableiold = NULL;
	Tmaxtableiold=new doublereal[ls];

	// �������� �� ���� ��������.
	for (integer itable=0; itable<ltdp; itable++) {
		
		if (bmultipowersourse==false) {
		    Tmaxtable=-1.0e+30;
			Tmaxtableold=-1.0e+30;
		}
		else if (bmultipowersourse==true) {
			for (integer isor=0; isor<ls; isor++) {
				Tmaxtablei[isor]=-1.0e+30;
				Tmaxtableiold[isor]=-1.0e+30;
			}
		}
		
		// �������� �� ���� ��������� ����������� �������.
		for (integer inumber=0; inumber<t.maxbound; inumber++) {
            // inumber - ����� ���������� ����
            if (sosedb[inumber].MCB<ls) {
				// �������� �����
				if ((s[sosedb[inumber].MCB].bgarber_depend)&&(s[sosedb[inumber].MCB].igarber_depend==itable)) {
					// �������� ��������� ������ �������� �� ����������� � �������� �����,
					// � ������� ����� ������������� ������ itable.

					if (bmultipowersourse==false) {
					    // ���������� ������������ ����������� ��������������� 
					    // ������ �������. (������ ��� ���� ���������� ����� ������������ ������ �������).
						if (bavgpower==false) {
					        Tmaxtable=fmax(Tmaxtable,t.potent[t.maxelm+inumber]); 
						}
						else if (bavgpower==true) {
							Tmaxtableold=fmax(Tmaxtableold,toldtimestep[t.maxelm+inumber]);
						}
					}
					else if (bmultipowersourse==true) {
						if (bavgpower==false) {
						    Tmaxtablei[sosedb[inumber].MCB]=fmax(Tmaxtablei[sosedb[inumber].MCB],t.potent[t.maxelm+inumber]);
						}
						else if (bavgpower==true) {
							Tmaxtableiold[sosedb[inumber].MCB]=fmax(Tmaxtableiold[sosedb[inumber].MCB],toldtimestep[t.maxelm+inumber]);
						}
					}
				}
			}
		}

		// �������� �� ���� ���������� �����
		for (integer isource=0; isource<ls; isource++) {
			if ((s[isource].bgarber_depend)&&(s[isource].igarber_depend==itable)) {

				if (bmultipowersourse==false) {

					doublereal Tmaxf=Tmaxtable;

					if (bavgpower==true) {
						switch (itypepowerdepend) {
						case AREFM : Tmaxf=0.5*(Tmaxtable+Tmaxtableold);
							         break; 
						case GARMONIC : Tmaxf=2.0*Tmaxtable*Tmaxtableold/(Tmaxtable+Tmaxtableold);
							         break;
						default : Tmaxf=2.0*Tmaxtable*Tmaxtableold/(Tmaxtable+Tmaxtableold);
							break;
						}
					}

				    // ���� ��� ������� ��������� �������� ������� �� ����������� � �������� �����,
				    // � ����� ��� ������� �������� ������������ ������� � ��������������� itable, 
				    // �� �������� ������������ ������������ ������������ ����� ������ ���������� ����� ������� ��������� ������ �������.
				    s[isource].power=my_splain_interpol_power_table(gtdps[s[isource].igarber_depend].intemp, 
					                                      gtdps[s[isource].igarber_depend].inoffset_drain, 
														  gtdps[s[isource].igarber_depend].rtemp,
														  gtdps[s[isource].igarber_depend].roffset_drain,
														  gtdps[s[isource].igarber_depend].rpower_table, 
						                                  Tmaxf,
														  s[isource].roperation_offset_drain);
				    s[isource].power*=s[isource].power_multiplyer; // ���������� �� �������������� ���������.
				}
				else if (bmultipowersourse==true) {

					doublereal Tmaxf=Tmaxtablei[isource];

					if (bavgpower==true) {
						switch (itypepowerdepend) {
						case AREFM : Tmaxf=0.5*(Tmaxtablei[isource]+Tmaxtableiold[isource]);
							         break; 
						case GARMONIC : Tmaxf=2.0*Tmaxtablei[isource]*Tmaxtableiold[isource]/(Tmaxtablei[isource]+Tmaxtableiold[isource]);
							         break;
						default : Tmaxf=2.0*Tmaxtablei[isource]*Tmaxtableiold[isource]/(Tmaxtablei[isource]+Tmaxtableiold[isource]);
							break;
						}
					}

					// ���� ��� ������� ��������� �������� ������� �� ����������� � �������� �����,
				    // � ����� ��� ������� �������� ������������ ������� � ��������������� itable, 
				    // �� �������� ������������ ������������ ������������ �� ����������� ������� ��������� �����.
				    s[isource].power=my_splain_interpol_power_table(gtdps[s[isource].igarber_depend].intemp, 
					                                      gtdps[s[isource].igarber_depend].inoffset_drain, 
														  gtdps[s[isource].igarber_depend].rtemp,
														  gtdps[s[isource].igarber_depend].roffset_drain,
														  gtdps[s[isource].igarber_depend].rpower_table, 
						                                  Tmaxf,
														  s[isource].roperation_offset_drain);
				    s[isource].power*=s[isource].power_multiplyer; // ���������� �� �������������� ���������.

				}

			}
		}
	}

	// ������������ ����������� ������.
	
	if (Tmaxtablei != NULL) {
		delete[] Tmaxtablei;
		Tmaxtablei = NULL;
	}
	if (Tmaxtableiold != NULL) {
		delete[] Tmaxtableiold;
		Tmaxtableiold = NULL;
	}

} // update_power_temperature_depend

/*
// ���������� ������� ���������� � � �� �������� ��������� �����������:
// ��� ����� ��� surface - 2 - surface ������ ��������� ������ Prism Object.
void update_avg_temperatures(doublereal* &potent, BLOCK &b) {
	if (b.radiation.binternalRadiation) {
		// ���� ������ ����� �������� ������ ���������.

		if ((b.radiation.nodelistW != NULL) && (b.radiation.nodelistE != NULL) && (b.radiation.nodelistS != NULL) && (b.radiation.nodelistN != NULL) && (b.radiation.nodelistB != NULL) && (b.radiation.nodelistT != NULL))
		{
			// �������� � ������ ������������ � Theory Guide ANSYS Fluent.

			// potent - ���� ���������.
			doublereal Tavg = 0.0;
			doublereal dS = 0.0;

			for (integer i = 0; i < b.radiation.nodelistWsize; i++) {
				doublereal temp= 0.5*((273.15 + potent[b.radiation.nodelistW[i].node1]) + (273.15 + potent[b.radiation.nodelistW[i].node2]));
				Tavg += b.radiation.nodelistW[i].dS*temp*temp*temp*temp;
				dS += b.radiation.nodelistW[i].dS;
			}
			Tavg = pow(Tavg / dS,0.25);
			if (Tavg < 0.0) {
				Tavg = 0.0; // ��������� ����������� ������.
				printf("error : TempW is negative...");
			}
			b.radiation.TempW = Tavg;

			Tavg = 0.0;
			dS = 0.0;

			for (integer i = 0; i < b.radiation.nodelistEsize; i++) {
				doublereal temp= 0.5*((273.15 + potent[b.radiation.nodelistE[i].node1]) + (273.15 + potent[b.radiation.nodelistE[i].node2]));
				Tavg += b.radiation.nodelistE[i].dS*temp*temp*temp*temp;
				dS += b.radiation.nodelistE[i].dS;
			}
			Tavg = pow(Tavg / dS, 0.25);
			if (Tavg < 0.0) {
				Tavg = 0.0; // ��������� ����������� ������.
				printf("error : TempE is negative...");
			}
			b.radiation.TempE = Tavg;

			Tavg = 0.0;
			dS = 0.0;

			for (integer i = 0; i < b.radiation.nodelistSsize; i++) {
				doublereal temp = 0.5*((273.15 + potent[b.radiation.nodelistS[i].node1]) + (273.15 + potent[b.radiation.nodelistS[i].node2]));
				Tavg += b.radiation.nodelistS[i].dS*temp*temp*temp*temp;
				dS += b.radiation.nodelistS[i].dS;
			}
			Tavg = pow(Tavg / dS, 0.25);
			if (Tavg < 0.0) {
				Tavg = 0.0; // ��������� ����������� ������.
				printf("error : TempS is negative...");
			}
			b.radiation.TempS = Tavg;

			Tavg = 0.0;
			dS = 0.0;

			for (integer i = 0; i < b.radiation.nodelistNsize; i++) {
				doublereal temp= 0.5*((273.15 + potent[b.radiation.nodelistN[i].node1]) + (273.15 + potent[b.radiation.nodelistN[i].node2]));
				Tavg += b.radiation.nodelistN[i].dS*temp*temp*temp*temp;
				dS += b.radiation.nodelistN[i].dS;
			}
			Tavg = pow(Tavg / dS, 0.25);
			if (Tavg < 0.0) {
				Tavg = 0.0; // ��������� ����������� ������.
				printf("error : TempN is negative...");
			}
			b.radiation.TempN = Tavg;


			Tavg = 0.0;
			dS = 0.0;

			for (integer i = 0; i < b.radiation.nodelistBsize; i++) {
				doublereal temp = 0.5*((273.15 + potent[b.radiation.nodelistB[i].node1]) + (273.15 + potent[b.radiation.nodelistB[i].node2]));
				Tavg += b.radiation.nodelistB[i].dS*temp*temp*temp*temp;
				dS += b.radiation.nodelistB[i].dS;
			}
			Tavg = pow(Tavg / dS, 0.25);
			if (Tavg < 0.0) {
				Tavg = 0.0; // ��������� ����������� ������.
				printf("error : TempB is negative...");
			}
			b.radiation.TempB = Tavg;


			Tavg = 0.0;
			dS = 0.0;

			for (integer i = 0; i < b.radiation.nodelistTsize; i++) {
				doublereal temp = 0.5*((273.15 + potent[b.radiation.nodelistT[i].node1]) + (273.15 + potent[b.radiation.nodelistT[i].node2]));
				Tavg += b.radiation.nodelistT[i].dS*temp*temp*temp*temp;
				dS += b.radiation.nodelistT[i].dS;
			}
			Tavg = pow(Tavg / dS,0.25);
			if (Tavg < 0.0) {
				Tavg = 0.0; // ��������� ����������� ������.
				printf("error : TempT is negative...");
			}
			b.radiation.TempT = Tavg;
		}
		else {
			printf("error memory: can not calculate avg Temperature in bounds vacuum Prism Object. Memory no allocate.\n");
			getchar();
			exit(1);
		}
	}

} // update_avg_temperatures
*/
// ���������� ������� ���������� � � �� �������� ��������� �����������:
// ��� ����� ��� surface - 2 - surface ������ ��������� ������ Prism Object.
// �������������� � ������ ���� ����� 20 �������� 2016.
void update_avg_temperatures(doublereal* &potent, BLOCK &b) {
	if (b.radiation.binternalRadiation) {
		// ���� ������ ����� �������� ������ ���������.
		// �� ��������� ������� �� ��������.

		if ((b.radiation.nodelistW != NULL) && (b.radiation.nodelistE != NULL) && (b.radiation.nodelistS != NULL) && (b.radiation.nodelistN != NULL) && (b.radiation.nodelistB != NULL) && (b.radiation.nodelistT != NULL))
		{
			// ������� � ������ ������������ � Theory Guide ANSYS Fluent.

			// potent - ���� ���������.
			doublereal Tavg = 0.0;
			doublereal dS = 0.0;

			// potent � �������� ������� !!!!

			for (integer i = 0; i < b.radiation.nodelistWsize; i++) {
				doublereal temp = 0.0;
				if (b.radiation.nodelistW[i].node21>-1) {
					temp = 0.5*((potent[b.radiation.nodelistW[i].node1]) + (potent[b.radiation.nodelistW[i].node21]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistW[i].dS1*temp*temp*temp*temp;
					dS += b.radiation.nodelistW[i].dS1;
				}
				if (b.radiation.nodelistW[i].node22>-1) {
					temp = 0.5*((potent[b.radiation.nodelistW[i].node1]) + (potent[b.radiation.nodelistW[i].node22]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistW[i].dS2*temp*temp*temp*temp;
					dS += b.radiation.nodelistW[i].dS2;
				}
				if (b.radiation.nodelistW[i].node23>-1) {
					temp = 0.5*((potent[b.radiation.nodelistW[i].node1]) + (potent[b.radiation.nodelistW[i].node23]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistW[i].dS3*temp*temp*temp*temp;
					dS += b.radiation.nodelistW[i].dS3;
				}
				if (b.radiation.nodelistW[i].node24>-1) {
					temp = 0.5*((potent[b.radiation.nodelistW[i].node1]) + (potent[b.radiation.nodelistW[i].node24]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistW[i].dS4*temp*temp*temp*temp;
					dS += b.radiation.nodelistW[i].dS4;
				}

			}
			Tavg = pow(Tavg / dS, 0.25);
			if (Tavg < 0.0) {
				Tavg = 0.0; // ��������� ����������� ������.
				printf("error : TempW is negative...");
			}
			b.radiation.TempW = Tavg;

			Tavg = 0.0;
			dS = 0.0;

			for (integer i = 0; i < b.radiation.nodelistEsize; i++) {

				if (b.radiation.nodelistE[i].node21>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistE[i].node1]) + (potent[b.radiation.nodelistE[i].node21]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistE[i].dS1*temp*temp*temp*temp;
					dS += b.radiation.nodelistE[i].dS1;
				}
				if (b.radiation.nodelistE[i].node22>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistE[i].node1]) + (potent[b.radiation.nodelistE[i].node22]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistE[i].dS2*temp*temp*temp*temp;
					dS += b.radiation.nodelistE[i].dS2;
				}
				if (b.radiation.nodelistE[i].node23>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistE[i].node1]) + (potent[b.radiation.nodelistE[i].node23]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistE[i].dS3*temp*temp*temp*temp;
					dS += b.radiation.nodelistE[i].dS3;
				}
				if (b.radiation.nodelistE[i].node24>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistE[i].node1]) + (potent[b.radiation.nodelistE[i].node24]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistE[i].dS4*temp*temp*temp*temp;
					dS += b.radiation.nodelistE[i].dS4;
				}

			}
			Tavg = pow(Tavg / dS, 0.25);
			if (Tavg < 0.0) {
				Tavg = 0.0; // ��������� ����������� ������.
				printf("error : TempE is negative...");
			}
			b.radiation.TempE = Tavg;

			Tavg = 0.0;
			dS = 0.0;

			for (integer i = 0; i < b.radiation.nodelistSsize; i++) {

				if (b.radiation.nodelistS[i].node21>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistS[i].node1]) + (potent[b.radiation.nodelistS[i].node21]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistS[i].dS1*temp*temp*temp*temp;
					dS += b.radiation.nodelistS[i].dS1;
				}
				if (b.radiation.nodelistS[i].node22>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistS[i].node1]) + (potent[b.radiation.nodelistS[i].node22]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistS[i].dS2*temp*temp*temp*temp;
					dS += b.radiation.nodelistS[i].dS2;
				}
				if (b.radiation.nodelistS[i].node23>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistS[i].node1]) + (potent[b.radiation.nodelistS[i].node23]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistS[i].dS3*temp*temp*temp*temp;
					dS += b.radiation.nodelistS[i].dS3;
				}
				if (b.radiation.nodelistS[i].node24>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistS[i].node1]) + (potent[b.radiation.nodelistS[i].node24]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistS[i].dS4*temp*temp*temp*temp;
					dS += b.radiation.nodelistS[i].dS4;
				}

			}
			Tavg = pow(Tavg / dS, 0.25);
			if (Tavg < 0.0) {
				Tavg = 0.0; // ��������� ����������� ������.
				printf("error : TempS is negative...");
			}
			b.radiation.TempS = Tavg;

			Tavg = 0.0;
			dS = 0.0;

			for (integer i = 0; i < b.radiation.nodelistNsize; i++) {

				if (b.radiation.nodelistN[i].node21>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistN[i].node1]) + (potent[b.radiation.nodelistN[i].node21]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistN[i].dS1*temp*temp*temp*temp;
					dS += b.radiation.nodelistN[i].dS1;
				}
				if (b.radiation.nodelistN[i].node22>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistN[i].node1]) + (potent[b.radiation.nodelistN[i].node22]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistN[i].dS2*temp*temp*temp*temp;
					dS += b.radiation.nodelistN[i].dS2;
				}
				if (b.radiation.nodelistN[i].node23>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistN[i].node1]) + (potent[b.radiation.nodelistN[i].node23]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistN[i].dS3*temp*temp*temp*temp;
					dS += b.radiation.nodelistN[i].dS3;
				}
				if (b.radiation.nodelistN[i].node24>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistN[i].node1]) + (potent[b.radiation.nodelistN[i].node24]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistN[i].dS4*temp*temp*temp*temp;
					dS += b.radiation.nodelistN[i].dS4;
				}

			}
			Tavg = pow(Tavg / dS, 0.25);
			if (Tavg < 0.0) {
				Tavg = 0.0; // ��������� ����������� ������.
				printf("error : TempN is negative...");
			}
			b.radiation.TempN = Tavg;


			Tavg = 0.0;
			dS = 0.0;

			for (integer i = 0; i < b.radiation.nodelistBsize; i++) {

				if (b.radiation.nodelistB[i].node21>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistB[i].node1]) + (potent[b.radiation.nodelistB[i].node21]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistB[i].dS1*temp*temp*temp*temp;
					dS += b.radiation.nodelistB[i].dS1;
				}
				if (b.radiation.nodelistB[i].node22>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistB[i].node1]) + (potent[b.radiation.nodelistB[i].node22]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistB[i].dS2*temp*temp*temp*temp;
					dS += b.radiation.nodelistB[i].dS2;
				}
				if (b.radiation.nodelistB[i].node23>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistB[i].node1]) + (potent[b.radiation.nodelistB[i].node23]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistB[i].dS3*temp*temp*temp*temp;
					dS += b.radiation.nodelistB[i].dS3;
				}
				if (b.radiation.nodelistB[i].node24>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistB[i].node1]) + (potent[b.radiation.nodelistB[i].node24]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistB[i].dS4*temp*temp*temp*temp;
					dS += b.radiation.nodelistB[i].dS4;
				}


			}
			Tavg = pow(Tavg / dS, 0.25);
			if (Tavg < 0.0) {
				Tavg = 0.0; // ��������� ����������� ������.
				printf("error : TempB is negative...");
			}
			b.radiation.TempB = Tavg;


			Tavg = 0.0;
			dS = 0.0;

			for (integer i = 0; i < b.radiation.nodelistTsize; i++) {

				if (b.radiation.nodelistT[i].node21>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistT[i].node1]) + (potent[b.radiation.nodelistT[i].node21]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistT[i].dS1*temp*temp*temp*temp;
					dS += b.radiation.nodelistT[i].dS1;
				}
				if (b.radiation.nodelistT[i].node22>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistT[i].node1]) + (potent[b.radiation.nodelistT[i].node22]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistT[i].dS2*temp*temp*temp*temp;
					dS += b.radiation.nodelistT[i].dS2;
				}
				if (b.radiation.nodelistT[i].node23>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistT[i].node1]) + (potent[b.radiation.nodelistT[i].node23]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistT[i].dS3*temp*temp*temp*temp;
					dS += b.radiation.nodelistT[i].dS3;
				}
				if (b.radiation.nodelistT[i].node24>-1) {
					doublereal temp = 0.5*((potent[b.radiation.nodelistT[i].node1]) + (potent[b.radiation.nodelistT[i].node24]));
					temp += 273.15; // ������� � ��������.
					Tavg += b.radiation.nodelistT[i].dS4*temp*temp*temp*temp;
					dS += b.radiation.nodelistT[i].dS4;
				}

			}
			Tavg = pow(Tavg / dS, 0.25);
			if (Tavg < 0.0) {
				Tavg = 0.0; // ��������� ����������� ������.
				printf("error : TempT is negative...");
			}
			b.radiation.TempT = Tavg;
		}
		else {
			printf("error memory: can not calculate avg Temperature in bounds vacuum Prism Object. Memory no allocate.\n");
			//getchar();
			system("PAUSE");
			exit(1);
		}
	}

} // update_avg_temperatures

void update_Stefan_Bolcman_condition_double_vacuum_PRISM(WALL* w, integer lw, integer ls, integer maxbound, BOUND* sosedb,
	doublereal* potent, doublereal** prop_b, TOCHKA* pa, integer** nvtx, integer maxelm)
{
	for (integer inumber = 0; inumber < maxbound; inumber++) {
		if ((bBlockStefanBolcman && (
			(((sosedb[inumber].MCB < (ls + lw)) && (sosedb[inumber].MCB >= ls) && (w[sosedb[inumber].MCB - ls].ifamily == 4)))))) 
		{
		    

			doublereal dl=1.0,  dS=1.0; // �������������� ���������
			

			// ���������� �������
			switch (sosedb[inumber].Norm) {
			case ESIDE:
				dl = pa[nvtx[1][sosedb[inumber].iI] - 1].x - pa[nvtx[0][sosedb[inumber].iI] - 1].x;
				dS = pa[nvtx[2][sosedb[inumber].iI] - 1].y - pa[nvtx[1][sosedb[inumber].iI] - 1].y;
				dS *= (pa[nvtx[4][sosedb[inumber].iI] - 1].z - pa[nvtx[0][sosedb[inumber].iI] - 1].z); // ������� �����

								
				break;

			case NSIDE:
				dl = pa[nvtx[2][sosedb[inumber].iI] - 1].y - pa[nvtx[0][sosedb[inumber].iI] - 1].y;
				dS = pa[nvtx[1][sosedb[inumber].iI] - 1].x - pa[nvtx[0][sosedb[inumber].iI] - 1].x;
				dS *= (pa[nvtx[4][sosedb[inumber].iI] - 1].z - pa[nvtx[0][sosedb[inumber].iI] - 1].z); // ������� �����

				
				break;

			case TSIDE:
				dl = pa[nvtx[4][sosedb[inumber].iI] - 1].z - pa[nvtx[0][sosedb[inumber].iI] - 1].z;
				dS = pa[nvtx[1][sosedb[inumber].iI] - 1].x - pa[nvtx[0][sosedb[inumber].iI] - 1].x;
				dS *= (pa[nvtx[2][sosedb[inumber].iI] - 1].y - pa[nvtx[0][sosedb[inumber].iI] - 1].y); // ������� �����


				break;

			case WSIDE:
				dl = pa[nvtx[1][sosedb[inumber].iI] - 1].x - pa[nvtx[0][sosedb[inumber].iI] - 1].x;
				dS = pa[nvtx[2][sosedb[inumber].iI] - 1].y - pa[nvtx[1][sosedb[inumber].iI] - 1].y;
				dS *= (pa[nvtx[4][sosedb[inumber].iI] - 1].z - pa[nvtx[0][sosedb[inumber].iI] - 1].z); // ������� �����

			
				break;

			case SSIDE :
				dl = pa[nvtx[2][sosedb[inumber].iI] - 1].y - pa[nvtx[0][sosedb[inumber].iI] - 1].y;
				dS = pa[nvtx[1][sosedb[inumber].iI] - 1].x - pa[nvtx[0][sosedb[inumber].iI] - 1].x;
				dS *= (pa[nvtx[4][sosedb[inumber].iI] - 1].z - pa[nvtx[0][sosedb[inumber].iI] - 1].z); // ������� �����

				break;

			case BSIDE :
				dl = pa[nvtx[4][sosedb[inumber].iI] - 1].z - pa[nvtx[0][sosedb[inumber].iI] - 1].z;
				dS = pa[nvtx[1][sosedb[inumber].iI] - 1].x - pa[nvtx[0][sosedb[inumber].iI] - 1].x;
				dS *= (pa[nvtx[2][sosedb[inumber].iI] - 1].y - pa[nvtx[0][sosedb[inumber].iI] - 1].y); // ������� �����
			
			
				break;

			} // end switch

			doublereal lamB = prop_b[LAM][sosedb[inumber].iB - maxelm];

			// ������ ��������� �������������� ��������� ������ �� �������.
			doublereal density_heat_flux = lamB*fabs(potent[sosedb[inumber].iB]-potent[sosedb[inumber].iI])/dl; 
			doublereal sigma = 5.67e-8;
			doublereal r1 = density_heat_flux / (sigma*w[sosedb[inumber].MCB - ls].emissivity);
			doublereal r2 = (273.15+w[sosedb[inumber].MCB - ls].Tamb);
			r1 += r2*r2*r2*r2;
			w[sosedb[inumber].MCB - ls].Tamb = sqrt(sqrt(r1))-273.15;
			// �� �������� ����������� � ������� ���� ����������.
		}

	}
}

// ���� ��������� ���������������� ���������, �.�.
// ������������ ����������� � ������������� �������� � ���� 
// ������� �� ����������� �� ������� ��������� ������ ��������, 
// �������������� ����� ���� �� ������:
// ���������� : ����� ����, ���� ���� �������� ���������� ��������� ��
// ��� ���������� �������� ������� ������������� ��������� ������ �� �������
// ������� ���� ������� (������ BETA==1.33333, ������ BETA==1.2) ����� ���������
// ������ ���������� ������ �.�. � ������ ������������� �������� ������� ������������
// ����������� ������� ������� �� ������� ����������� � ����� �������� ����� ���������� � �������������.
void solve_nonlinear_temp(FLOW &f, FLOW* &fglobal, TEMPER &t, doublereal** &rhie_chow,
	BLOCK* b, integer lb, SOURCE* s, integer ls, WALL* w, integer lw,
	doublereal dbeta, integer flow_interior, bool bconvective,
	doublereal* toldtimestep, doublereal tauparam, doublereal tauparamold, bool btimedep,
	TPROP* matlist, integer inumiter, bool bprintmessage,
	TEMP_DEP_POWER* gtdps, integer ltdp, doublereal  poweron_multiplier_sequence,
	QuickMemVorst& m, doublereal** speedoldtimestep, doublereal** mfoldtimestep,
	integer lu, UNION* &my_union) {

	// �������� ������������� ��������� ��������.	
	errno_t err_inicialization_data;
	FILE* fp_inicialization_data=NULL;
	err_inicialization_data = fopen_s(&fp_inicialization_data, "load.txt", "r");
	if (err_inicialization_data == 0) {
		fclose(fp_inicialization_data);
	}

	t.alpha = 1.0;

	doublereal balancet = 0.0;
	if (bglobal_first_start_radiation) {
		doublereal pdiss = 0.0;

		for (integer i = 0; i < ls; i++) {
			if (s[i].power < 0.0) {
				printf("warning source [%d] is negative power = %e\n",i, s[i].power);
			}
			pdiss += s[i].power;
		}
		//for (integer i = 0; i < lb; i++) {
			//pdiss += b[i].Sc*(fabs(b[i].g.xE - b[i].g.xS)*fabs(b[i].g.yE - b[i].g.yS)*fabs(b[i].g.zE - b[i].g.zS));
		//}
		// 19 november 2016.
		// ���������� �������� �������������� �� ���� ���������� �����.
		for (integer i47 = 0; i47 < t.maxelm; i47++) {
			// �������� � ��� ��� �������� �� ����������� ��� ������ � ������ ��������.
			integer ib = t.whot_is_block[i47];
			t.Sc[i47] = get_power(b[ib].n_Sc, b[ib].temp_Sc, b[ib].arr_Sc, t.potent[i47]);
			// ���������� �������� �������� ������������ ������:
			doublereal dx = 0.0, dy = 0.0, dz = 0.0;// ����� �������� ������������� ������
			volume3D(i47, t.nvtx, t.pa, dx, dy, dz);
			if (t.Sc[i47] * dx*dy*dz < 0.0) {
				printf("ERROR!!!  control volume [%d] is negative power = %e\n", i47, t.Sc[i47] * dx*dy*dz);
				getchar();
			}
			pdiss += t.Sc[i47] * dx*dy*dz;
		}
		printf("power geration is equal=%e\n",pdiss);
		if (pdiss>0.0) {
			doublereal square_bolc = 0.0;
			doublereal emissivity = 1.0;
			for (integer i = 0; i < lw; i++) {
				if (w[i].ifamily == 4) {
					switch (w[i].iPlane) {
					case XY: square_bolc += fabs(w[i].g.xE - w[i].g.xS)*fabs(w[i].g.yE - w[i].g.yS); break;
					case XZ: square_bolc += fabs(w[i].g.xE - w[i].g.xS)*fabs(w[i].g.zE - w[i].g.zS); break;
					case YZ: square_bolc += fabs(w[i].g.yE - w[i].g.yS)*fabs(w[i].g.zE - w[i].g.zS); break;
					}
					// ����� �� ������������ ��� �� ���� ���������� ������������ ���������� ����������� ���� � ����.
					// ���� ��� �� ��� �� ��������� ������.
					emissivity = w[i].emissivity;
				}
			}
			if (fabs(square_bolc)>1e-23) {
				printf("Pdiss=%e, S=%e\n",pdiss, square_bolc);
				balancet = sqrt(sqrt((pdiss / (square_bolc * 5.6704e-8*emissivity)))) - 273.15;
				printf("balance temperature =%f\n", balancet);
				//t.alpha = 0.5;
				//balancet = 50.0;
			}
			else {
				printf("square_bolc is zero!!!\n");
			}
		}
		else {
			// ���������������� ������������� �������� ��������������.
			bool bDirichlet = false;
			for (integer i = 0; i < lw; i++) {
				if (w[i].ifamily == 1) {
					// ������� ���������� ����������� ����������.
					bDirichlet = true;
				}
			}
			if (!bDirichlet) {
				printf("negative power and the lack of Dirichlet conditions \n");
				balancet = -272.15;
			}
		}
	}
	else {
		printf("no Stefan - Bolcman boundary condition...\n");
	}
	//system("pause");

	// � ����� �������� ������������� ����� �� ����� �������������.
	for (integer i_init = 0; i_init < t.maxelm; i_init++) bsource_term_radiation_for_relax[i_init] = 0.0;

	if (adiabatic_vs_heat_transfer_coeff == 1) {
		printf("film coefficient=%e, operating_temperature=%f\n", film_coefficient, operating_temperature_for_film_coeff);
		//t.alpha = 0.8; // �� �������� ����� ������ ����������.
		printf("temperature relax factor is equal %e\n", t.alpha);
		// system("pause");
	}

	if (adiabatic_vs_heat_transfer_coeff == 2) {
		printf(" operating_temperature=%f\n", operating_temperature_for_film_coeff);
		//t.alpha = 0.8; // �� �������� ����� ������ ����������.
		printf("temperature relax factor is equal %e\n", t.alpha);
		// system("pause");
	}

	if (adiabatic_vs_heat_transfer_coeff == 3) {
		printf("film coefficient=%e, operating_temperature=%f\n", film_coefficient, operating_temperature_for_film_coeff);
		//t.alpha = 0.8; // �� �������� ����� ������ ����������.
		printf("temperature relax factor is equal %e\n", t.alpha);
		// system("pause");
	}

	doublereal RCh = 1.0;

	//if (bfirst_start_nonlinear_process) {
	// �� ��������� ������������ �� ���������� ������� � ������� 
	// ��������� ����������.
	//bfirst_start_nonlinear_process = false;
	// � ��������� �� 1 �� 25 ��������.
	// for (integer i_init = 0; i_init < t.maxelm; i_init++) t.potent[i_init] = rand() % 2 + 1;
	//}

	

	// ��������� ���� ���������� ������������ ���������� � ���� �������� ������� �� ���� ��������� �������.
	// ��� ���������� � ����� constr_struct.c.
	bool bupdateproperties = true; // ��������� �������� ��� ����������� ���� (0) �������� �������.
	doublereal deltat = 100.0; // ��������� ������� ����� ���������� 100 �������� �������.
	doublereal *told = NULL;
	told = new doublereal[t.maxelm + t.maxbound];
	integer i = 0;
	doublereal res = 0.0;
#pragma omp parallel for shared (t,told) private (i) schedule (guided)
	for (i = 0; i < t.maxelm + t.maxbound; i++) {
		//t.potent[i] = -269.0;
		told[i] = t.potent[i]; // �����������
		if (bglobal_first_start_radiation) {
			told[i] = balancet;
			t.potent[i] = balancet;
			if (bdouble_vacuum_PRISM) {
				if (i < t.maxelm) {
					//printf("bdouble vacuum prism\n");
					//system("pause");
					//told[i] = 0.1*(rand() % 10) + balancet;
					//t.potent[i] = 0.1*(rand() % 10) + balancet;
				}
			}
		}
	}
	integer ic = 1;
	doublereal tmax = 0.0;

	bool bfreeflag = m.bsignalfreeCRSt;
	if (bfreeflag) {
		// ����� ������ �� ����������� � ������� ������.
		m.bsignalfreeCRSt = false;
	}


	doublereal* rthdsdt = NULL;
	rthdsdt = new doublereal[t.maxelm + t.maxbound];

	integer iprohod = 1;
	if (bdouble_vacuum_PRISM) iprohod = 1; // �� ������ ������� ������� ������� - ��������� ����������� � ���������� �� ���������������� ������.
	for (integer iprohodtek = 0; iprohodtek < iprohod; iprohodtek++) {

		deltat = 100.0; // ��� ���� ����� ����� ��������� ����.

		//if ((iprohodtek == 0) && (iprohod == 2)) {
		if (bdouble_vacuum_PRISM) {
			// ������������� ���������� ������� - ���������.
			//bBlockStefanBolcman = true;
		}
		//}
		//else {
		// ������� ���������� ������� - ���������.
		//	bBlockStefanBolcman = false;
		//}
		// ��������� ���������������� � ������ ������ ���������.
		// ������������ ������� �� ������ ���������� ��������� ���������� �� �����������,
		// �� ������ ��������� ��������� �� �����������. ��� �������� � ���� ��� ������ ���� �� �������
		// ��������� ������������ ������������ �������.
		//integer i87 = 0;
		integer ibreak_counter_25_07_2017 = 0;

		// �� ����� 10 ��������.
		while (deltat > 0.5) {
			ibreak_counter_25_07_2017++;
			// ��������� ����� �� ������������� ��������.
			//if ((err_inicialization_data == 0)&&(ibreak_counter_25_07_2017 > 19)) break;
			// �� ��������� ������� ������� ���������� ����� ��������� �� ����� ����� ����� 7� ������� ����� ��������� �����.
			if ((err_inicialization_data == 0) && (ibreak_counter_25_07_2017 > 6)) break;
			// 25.07.2017 ����� SuperC ������-�� ����� ��������,
			// ����������� ��������� ������� �� ���������������, ������������ ������ 0.5 ���� �.
			// �������� ��������� ���������-�������� ��� ����������� 52 (������ 10� �����) � �������������� ������������ � ��������� 
			// ������ ����� ��������.
			// ����� Super C ������������ �������� ���������� ���� �����������, �.�. ����� UDS ��� ���������� �� 25% ����������� ��-�� 
			// ������� ������������� ��������.

					/*if (i87 == 0) {
						bBlockStefanBolcman = true;
					}
					else if (i87==1) {
						bBlockStefanBolcman = false;
					}
					else*/
					//if (i87%2 == 0) {
						// �� ������ �� ��������� ������� ��������� ��� ������� �������� �������.
						//bBlockStefanBolcman = true;
					////}
					//else {
						bBlockStefanBolcman = false;
					//}
					
						

					// ���� ������� � ������������ ����� ���������� ������ ������ �������, ��:
					if (bupdateproperties) {
						// � ������ �������� ��������� �� ����������� ���������� 
						// ���������� �������� �������� ��������.
						update_power_temperature_depend(s, ls, t, t.sosedb, gtdps, ltdp, toldtimestep);
						update_temp_properties(t, fglobal, b, lb, matlist); // ��������� �������� ������ ����������
						// � ��������� �������� ������ ���������� �.�. ��� ������� ����� ��������� � �������� ���-���
						// ���������, ������������ ��������, ����������� ��������� �������������� ����������.
						update_flow_properties(t, fglobal, b, lb, flow_interior, matlist, false);

						/*if (i87 == 0) {
							// �� ������ �� ��������� ������� ��������� ��� ������� �������� �������.
							// ���������� ���������� ���������� � � �� �������� ��������� ����������� :
							for (integer i23 = 0; i23 < lb; i23++) {
								update_avg_temperatures(t.potent, b[i23]);
							}
							// ���������� ���������� ������������ �������� ������� :
							for (integer i23 = 0; i23 < lb; i23++) {
								calculation_density_radiation_heat_flux(b[i23]);
							}
						}
						else if (i87 == 1) {
							// ������ �� ������.
						}
						else*/// if (i87 % 2 == 0) {
						
							// �� ������ �� ��������� ������� ��������� ��� ������� �������� �������.
							// ���������� ���������� ���������� � � �� �������� ��������� ����������� :
							for (integer i23 = 0; i23 < lb; i23++) {
								update_avg_temperatures(t.potent, b[i23]);
							}
							// ���������� ���������� ������������ �������� ������� :
							for (integer i23 = 0; i23 < lb; i23++) {
								calculation_density_radiation_heat_flux(b[i23]);
							}
							
						//}
							fprintf_s(fp_radiation_log, "\n");
					}
					res = 0.0;

					//printf("%e\n",s[0].power); // debug
					//getchar();

					// ���������� �������� �������������� �� ���� ���������� �����.
					for (integer i47 = 0; i47 < t.maxelm; i47++) {
						// �������� � ��� ��� �������� �� ����������� ��� ������ � ������ ��������.
						integer ib = t.whot_is_block[i47];
						t.Sc[i47]= get_power(b[ib].n_Sc, b[ib].temp_Sc, b[ib].arr_Sc, t.potent[i47]);
						if (t.Sc[i47]  < 0.0) {
							printf("ERROR!!! control volume [%d] is negative t.Sc = %e\n", i47, t.Sc[i47] );
							getchar();
						}
					}


					
					

					if ((err_inicialization_data == 0)||(starting_speed_Vx*starting_speed_Vx + starting_speed_Vy*starting_speed_Vy + starting_speed_Vz*starting_speed_Vz > 1.0e-30)) {
						// ����������� ������ : ��������� ���� ��������� �� ��� �� �����������.
						if (t.ptr == NULL) {
							printf("error t.ptr == NULL.\n");
							system("PAUSE");
						}

						if (fglobal[0].maxelm == 0) {
							printf("Speed not constr struct.\n");
							system("PAUSE");
						}
						
						for (integer iP = 0; iP<fglobal[0].maxelm; iP++) {

							// ��������� ����������������� �������� ����� ����� ����� ��.
							// �������� ����� ����������� �� ������� �������� �� � ������
							// ������ ��� ���������������� �������� ���-���. ��� ��� ���������� ������������
							// ������� �������� ������������ �������� �� ����� ��.

							

							return_calc_correct_mass_flux_only_interpolation(iP,
								fglobal[0].potent,
								fglobal[0].pa,
								fglobal[0].prop,
								fglobal[0].prop_b,
								fglobal[0].nvtx,
								fglobal[0].sosedi,
								fglobal[0].maxelm,
								fglobal[0].mf[iP] // ������������ �������� ��������� ������
								);

							
						}					

						

						// ������������ �������� ������������ ���������� ������� ����������.
						iscorrectmf(fglobal[0].mf, fglobal[0].maxelm, fglobal[0].sosedi, fglobal[0].sosedb, ls, lw, w);

					}

					

						// ������ ���������� �������� ����:
						// �������� dbeta �������� �� �������� ������������� ��������� �������:
						// dbeta==1.0 ������ �������, dbeta==1.33333333333 ������ �������, dbeta=1.2 ������ ������� �������������.
						doublereal** rsumanbstuff = NULL;
						doublereal resfluent_temp = 0.0;
						solve(TEMP, // ������������� ��������� (��� ��������� �������������).
							res, // �������
							f,
							fglobal,
						t,
						rhie_chow,
						s, w, b, ls, lw, lb, // ������� (���������, ������, �����).
						dbeta,
						flow_interior,
						bconvective,
						false,
						toldtimestep, // ���� ���������� � ����������� ���������� ����
						told,
						speedoldtimestep, // �������� � ����������� ���������� ����
						mfoldtimestep, // ������������ ����� ����� ����� �� � ����������� ���������� ����
						tauparam, // ������ ���� �� �������
						btimedep, // ������������ ��� �������������� ������
						0.0, 0.0, 0.0, // ��������� ���������� �������
						matlist, // ��������� ����������
						inumiter, bprintmessage, RCh, false,
						NULL, rsumanbstuff, false, false, poweron_multiplier_sequence,
						m, rthdsdt, resfluent_temp,
						lu, my_union); // ����� ���������� ��������

					for (i = 0; i < t.maxelm + t.maxbound; i++) {
						if (t.potent[i] < -272.15) {
							t.potent[i] = -272.15; // �������������� ���������� ����.
						}
					}

					bglobal_first_start_radiation = false;
					t.alpha = 1.0;

					bupdateproperties = true;
					tmax = 0.0;

					// ��� ��������� ��� �������
					//exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, 0, false, 0);

					// ����������� ��������� �������� ���������� ����� ����������:
					//for (i=0; i<t.maxelm+t.maxbound; i++) tmax=fmax(tmax,fabs(t.potent[i]-told[i])); 
					// 23 ������� 2015
					// �� ��������� ������ ���������� ����� �� ����� ��������� ������� �����������, �������
					// �������� �� ������� ����� � ��������� ����������� ������ �� ���������� ��. 
					for (i = 0; i < t.maxelm + t.maxbound; i++) tmax = fmax(tmax, fabs(t.potent[i] - told[i]));
					doublereal maxdomain = -273.15; // ���������� ����������� � �������� �������.
					for (i = 0; i < t.maxelm + t.maxbound; i++) maxdomain = fmax(maxdomain, t.potent[i]);
					deltat = tmax;
					//if (deltat > 13.0) {
						//t.alpha = 0.9;
					//}
					//else if (deltat<0.3) {
						//t.alpha = 1.0;
				//	}
					// � ������ ����������� ���������� ������� ��������� ������ ����������.
					//if ((err_inicialization_data == 0) || (adiabatic_vs_heat_transfer_coeff > 0) || (breakRUMBAcalc_for_nonlinear_boundary_condition)) {
					if ((adiabatic_vs_heat_transfer_coeff > 0) || (breakRUMBAcalc_for_nonlinear_boundary_condition)) {


						//printf("24.07.2017 incomming\n");
						//system("PAUSE");
						// High Order Term Relaxation 23.3.1.10 Fluent Solver Theory
						// default relaxation factor on steady-state cases is 0.25.
						doublereal fHORF = 0.25; // for steady state problem.
						if (btimedep) { // unsteady problems.
							fHORF = 0.75; // ANSYS Fluent Theory Guide.
						}
						if ((((iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 3) || (iswitchsolveramg_vs_BiCGstab_plus_ILU2 == 7)))&&(my_amg_manager.istabilizationTemp == 3)) {
							fHORF = 1.0;
						}
						//fHORF = 0.00625;
						if (bvacuumPrism) {
							//printf("vacuum determinate 23_11_2016.\n");
							//system("pause");
							//������� ������� ������ ������ ����������.
							// �� � ���� ������ �� ������ fHORF ������ ��� 0.02.
							//fHORF = 0.01; // 0.01!!!
							
						}
						// ���������� ���������� �������� �� ���������� ������������ � �������.
						//fHORF = 1.0;
						for (i = 0; i < t.maxelm + t.maxbound; i++) {
							if (t.potent[i] > -272.15) {
								t.potent[i] = told[i] + fHORF*(t.potent[i] - told[i]);
							}
						}
					}

					// �������� �� ���������� ������������, �� ����� ���������� ���� ���������� ����.
					for (i = 0; i < t.maxelm + t.maxbound; i++)  if (t.potent[i] < -272.15) {
						// � �������� ������� �������� ����������� ���������� ��������� �������������
						// ����������� ���� ����������� ����.
					      t.potent[i] = -272.15;
						  //printf("fatal error: temperature is < -273.15C\n");
						//getchar();
					    //	exit(1);
					}

				

#pragma omp parallel for shared (t,told) private (i) schedule (guided)
					for (i = 0; i < t.maxelm + t.maxbound; i++) told[i] = t.potent[i]; // �����������
					// ������� ���������� ���������� � ��������� tecplot360:
					//exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior,0);
					if (bprintmessage) {
						//printf("exports in techplot successfully completed. \n");
#if doubleintprecision == 1
						printf("temperature nonlinear solver. Global iteration number %lld.\n", ic);
#else
						printf("temperature nonlinear solver. Global iteration number %d.\n", ic);
#endif
						
						printf("temperature difference between iterations %3.2f  oC. %e\n", deltat,  maxdomain);
						doublereal tmaxloc = -272.15e6;
						for (i = 0; i<t.maxelm; i++) tmaxloc = fmax(tmaxloc, t.potent[i]);
						if (bprintmessage) {
							printf("Intermediate maximum temperature in default interior\n");
							printf("is equal %e  oC.\n", tmaxloc);
						}
						doublereal tminloc = 1.0e7;
						for (i = 0; i<t.maxelm; i++) tminloc = fmin(tminloc, t.potent[i]);
						if (bprintmessage) {
							printf("Intermediate minimum temperature in default interior\n");
							printf("is equal %e  oC.\n", tminloc);
						}
						integer ic62 = 0;
						for (i = 0; i < t.maxelm; i++) {
							if (t.potent[i] < t.operatingtemperature) {
								ic62++;
								//printf("anomal control volume %d\n", i);
							}
						}
						if (ic62 > 0) {
							printf("maxelm=%d maxbound=%d anomal internal temperature control volume=%d \n", t.maxelm, t.maxbound, ic62);
							//getchar();
						}

						//getchar();
					}
					tmax = -1.0e23;
					// ���������� �������� ������������ ����������� ������ ��������� ������� � �� � ��������:
					for (i = 0; i < t.maxelm + t.maxbound; i++) tmax = fmax(tmax, t.potent[i]);
					diagnostic_critical_temperature(tmax); // �������������� ��� ���������� ����������� ���������� �����������.
					//getchar(); // debug ����� �������
					ic++;
					//if (ic > 20) break; // �� ������� �� 20 ������
					//break; // �� ����� ������ ������ �� ������������

					// true ��� ������ ������� ������ �������� �� ����������� �����������, ������������ ���������� ������.
					blocker_Newton_Richman = false;
					//system("pause");
				}

				

				if (bdouble_vacuum_PRISM) {
					//	update_Stefan_Bolcman_condition_double_vacuum_PRISM(w, lw, ls, t.maxbound, t.sosedb,
					//	t.potent, t.prop_b, t.pa, t.nvtx, t.maxelm);
					//	getchar();
				}
			
		}
		

		// ���������� �������� ������������ ����������� ������ ��������� ������� � �� � ��������:
		//for (i=0; i<t.maxelm+t.maxbound; i++) tmax=fmax(tmax,fabs(t.potent[i]));
		// 23 ������� 2015
		// �� ��������� ������ ���������� ����� �� ����� ��������� ������� �����������, �������
		// �������� �� ������� ����� � ��������� ����������� ������ �� ���������� ��. 
		//for (i = 0; i<t.maxelm; i++) tmax = fmax(tmax, fabs(t.potent[i]));
		tmax = -272.15;
		for (i = 0; i<t.maxelm; i++) tmax = fmax(tmax, t.potent[i]);
		if (bprintmessage) {
			printf("Finally maximum temperature in default interior\n");
		    printf("is equal %3.2f  oC.\n",tmax);
		}
		
		
		if (bfreeflag) {

			// ���� ������� � ������������ ����� ���������� ������ ������ �������, ��:
			if (bupdateproperties) {
				// � ������ �������� ��������� �� ����������� ���������� 
				// ���������� �������� �������� ��������.
				update_power_temperature_depend(s, ls, t, t.sosedb, gtdps, ltdp, toldtimestep);
				update_temp_properties(t, fglobal, b, lb, matlist); // ��������� �������� ������ ����������
																	// � ��������� �������� ������ ���������� �.�. ��� ������� ����� ��������� � �������� ���-���
																	// ���������, ������������ ��������, ����������� ��������� �������������� ����������.
				update_flow_properties(t, fglobal, b, lb, flow_interior, matlist, false);

				/*if (i87 == 0) {
				// �� ������ �� ��������� ������� ��������� ��� ������� �������� �������.
				// ���������� ���������� ���������� � � �� �������� ��������� ����������� :
				for (integer i23 = 0; i23 < lb; i23++) {
				update_avg_temperatures(t.potent, b[i23]);
				}
				// ���������� ���������� ������������ �������� ������� :
				for (integer i23 = 0; i23 < lb; i23++) {
				calculation_density_radiation_heat_flux(b[i23]);
				}
				}
				else if (i87 == 1) {
				// ������ �� ������.
				}
				else*/// if (i87 % 2 == 0) {

				// �� ������ �� ��������� ������� ��������� ��� ������� �������� �������.
				// ���������� ���������� ���������� � � �� �������� ��������� ����������� :
				for (integer i23 = 0; i23 < lb; i23++) {
					update_avg_temperatures(t.potent, b[i23]);
				}
				// ���������� ���������� ������������ �������� ������� :
				for (integer i23 = 0; i23 < lb; i23++) {
					calculation_density_radiation_heat_flux(b[i23]);
				}

				//}
				fprintf_s(fp_radiation_log, "\n");
			}

			// ���������� �������� �������������� �� ���� ���������� �����.
			for (integer i47 = 0; i47 < t.maxelm; i47++) {
				// �������� � ��� ��� �������� �� ����������� ��� ������ � ������ ��������.
				integer ib = t.whot_is_block[i47];
				t.Sc[i47] = get_power(b[ib].n_Sc, b[ib].temp_Sc, b[ib].arr_Sc, t.potent[i47]);
				if (t.Sc[i47]  < 0.0) {
					printf("ERROR!!! control volume [%d] is negative power t.Sc = %e\n", i47, t.Sc[i47]);
					getchar();
				}
			}
			

			if ((err_inicialization_data == 0) || (starting_speed_Vx*starting_speed_Vx + starting_speed_Vy*starting_speed_Vy + starting_speed_Vz*starting_speed_Vz > 1.0e-30)) {
				// ����������� ������ : ��������� ���� ��������� �� ��� �� �����������.
				if (t.ptr == NULL) {
					printf("error t.ptr == NULL.\n");
					system("PAUSE");
				}

				if (fglobal[0].maxelm == 0) {
					printf("Speed not constr struct.\n");
					system("PAUSE");
				}

				for (integer iP = 0; iP<fglobal[0].maxelm; iP++) {

					// ��������� ����������������� �������� ����� ����� ����� ��.
					// �������� ����� ����������� �� ������� �������� �� � ������
					// ������ ��� ���������������� �������� ���-���. ��� ��� ���������� ������������
					// ������� �������� ������������ �������� �� ����� ��.



					return_calc_correct_mass_flux_only_interpolation(iP,
						fglobal[0].potent,
						fglobal[0].pa,
						fglobal[0].prop,
						fglobal[0].prop_b,
						fglobal[0].nvtx,
						fglobal[0].sosedi,
						fglobal[0].maxelm,
						fglobal[0].mf[iP] // ������������ �������� ��������� ������
					);


				}



				// ������������ �������� ������������ ���������� ������� ����������.
				iscorrectmf(fglobal[0].mf, fglobal[0].maxelm, fglobal[0].sosedi, fglobal[0].sosedb, ls, lw, w);

			}

			// ����������� ������.
            m.bsignalfreeCRSt=true;
			doublereal** rsumanbstuff=NULL;
			doublereal resfluent_temp = 0.0;
			// ��� ���� ����� �������� �� ������ �������� �� �������, �.�. ��� � ��� ��� ��������.
			solve(TEMP, // ������������� ��������� (��� ��������� �������������).
				   res, // �������
				   f, 
				   fglobal,
				   t,
				   rhie_chow,
				   s, w, b, ls, lw, lb, // ������� (���������, ������, �����).
				   dbeta,
				   flow_interior,
				   bconvective,
				   false,
				   toldtimestep, // ���� ���������� � ����������� ���������� ����
				   told,
				   speedoldtimestep, // �������� � ����������� ���������� ����
				   mfoldtimestep, // ������������ ����� ����� ����� �� � ����������� ���������� ����
				   tauparam, // ������ ���� �� �������
				   btimedep, // ������������ ��� �������������� ������
				   0.0, 0.0, 0.0, // ��������� ���������� �������
				   matlist, // ��������� ����������
				   inumiter,bprintmessage,RCh,false,
				   NULL,rsumanbstuff,false,false, poweron_multiplier_sequence,
				   m, rthdsdt, resfluent_temp, lu, my_union); // ����� ���������� ��������

			

			      for (i = 0; i < t.maxelm + t.maxbound; i++) {
				      if (t.potent[i] < -273.15) {
					     t.potent[i] = -273.15; // �������������� ���������� ����.
				      }
			       }

				  

			       tmax = -273.15;
			       for (i = 0; i<t.maxelm; i++) tmax = fmax(tmax, t.potent[i]);
			       if (bprintmessage) {
				       printf("Finally maximum temperature in default interior\n");
				       printf("is equal %3.2f  oC.\n", tmax);
			       }
				   doublereal tmin = 1.0e7;
				   for (i = 0; i<t.maxelm; i++) tmin = fmin(tmin, t.potent[i]);
				   if (bprintmessage) {
					   printf("Finally minimum temperature in default interior\n");
					   printf("is equal %3.2f  oC.\n", tmin);
				   }
				   integer ic62 = 0;
				   for (i = 0; i < t.maxelm; i++) {
					   if (t.potent[i] < t.operatingtemperature) {
						   ic62++;
						   //printf("anomal control volume %d\n",i);
					   }
				   }
				   if (ic62 > 0) {
					   printf("maxelm=%d maxbound=%d anomal internal control volume is negative power=%d \n",t.maxelm, t.maxbound, ic62);
					   //getchar();
				   }
				   
		}

		

		// ������������ ����������� ������:
		if (told != NULL) {
			delete[] told;
			told = NULL;
		}
		
		if (rthdsdt != NULL) {
			delete[] rthdsdt;
			rthdsdt = NULL;
		}
		
} // solve_nonlinear_temp

// ��������������� �������
doublereal mycosh(doublereal x) {
	doublereal r=0.0;
	try
	{
	  // ������� ����������, �� ����� ���� ����� �������� �� ������������.
	  r=0.5*(exp(x)+exp(-x));
	}
	catch(integer iex)
	{
#if doubleintprecision == 1
		printf("exeption identifier=%lld in function cosh(x) for Smagorinsky Model/n", iex);
#else
		printf("exeption identifier=%d in function cosh(x) for Smagorinsky Model/n", iex);
#endif
		
		printf("please, press any key to exit is program/n");
		//getchar();
		system("pause");
		exit(0);
	}
	return r;
} // mycosh

// ���������� ������� ����� �� ������������� �� ������������� �����.
doublereal smagorinsky_length2(doublereal Cs, doublereal dx, doublereal dy, doublereal dz, doublereal dist,
	                     doublereal roughness, integer ipowerroughness,
	                     bool bfdelta, bool bSmagorinsky_Lilly, 
						 bool bsurface_roughness) {
	// Cs - ��������� �������������.
	// � ������ ���� ��� ����� ���� ��� ������������� ��� � �������������. 
	// ���� bfdelta==true �� ���������� �������������� �������� �� ������������� �����.
	// ���� bSmagorinsky_Lilly==true �� ���������� ������ Smagorinsky_Lilly
	// ���� bsurface_roughness==true ������ ����� ��������� ������������� �����������,
	// roghness �������� ������������� ����������� � �� [�], 
	// ipowerroughness - ���������� ������� � ������� ��� �������������.
	// ��� ��� ����� �� ����������� � ����� �������������� � ����� ���������.

	doublereal dV=dx*dy*dz; // ����� ������������ ������.
	//doublereal delta=pow(dV,1.0/3.0); // ���������� ������ �� ������ ������������ ������.
	doublereal delta=exp((1.0/3.0)*log(dV)); 
	doublereal length=Cs*delta; // ����� �������������.
	// ��� ������������� ����� ������������� ������ ��������� ����������� �����������.
	doublereal fdelta=1.0; // �� ��������� �������������� �������� ���������.
	if (bfdelta) {
	   doublereal rK=2.0/(3.0*sqrt(3.0));
	   doublereal *xi=new doublereal[3];
	   xi[0]=dx; xi[1]=dy; xi[2]=dz;
	   // ������������� ������ �� �����������.
 	   bool bflag=true;
	   while (bflag) {
		   bflag=false;
		   for (integer j=0; j<2; j++) {
			   if (xi[j]>xi[j+1]) {
				   // swap
				   doublereal xbuf=xi[j];
				   xi[j]=xi[j+1];
				   xi[j+1]=xbuf;
				   bflag=true;
			   }
		   }
	   }
	   doublereal rloga1=log(xi[0]/xi[2]); // ����������� ��������.
	   doublereal rloga2=log(xi[1]/xi[2]);
	   fdelta=mycosh(rK*sqrt(rloga1*rloga1-rloga1*rloga2+rloga2*rloga2));
	   if (xi != NULL) {
		   delete[] xi;
	   }
	}
	length*=fdelta;
	if (bsurface_roughness) {
		// �������������� ���� ������������� ������ ������.
		if (ipowerroughness==2) {
			doublereal fr=1.0/sqrt(1.0+(length/(0.419*(dist+roughness)))*(length/(0.419*(dist+roughness))));
			length*=fr; // ���������� ������������ �������.
		}
		if (ipowerroughness==1) {
			length=(length*0.419*(dist+roughness))/(0.419*(dist+roughness)+length);
		}
	}
	if (bSmagorinsky_Lilly) {
		// ������ �������������-�����
		// ��� ����� �������������.
		length=fmin(length,0.419*dist);
	}
	doublereal length2;
	length2=length*length;
	if (length<0.0) length2*=-1.0;	
	return length2;
}

// ������������ ������������ ������������ �������� �� ������� ��������� �������.
void my_boundary_musgs_LES(integer inumber, integer maxelm, 
							  BOUND* sosedb, integer ls, integer lw,
							  WALL* w, TOCHKA* pa, integer** nvtx, doublereal* &potent
							  ) 
{

	 // potent - ������ �� ���������� ������������ ��������.
	 // �� ������ ����������� ������ �������� ������������ ������������ �������� ����� ����, �
	 // �� ��������� �������� �������� ������������ ������������ �������� �������� (����������) �� 
	 // ��������� ���������� ����� �� ������� ��������� �������.

     // inumber - ����� ���������� ��.
	 // inumber ���������� �� 0..maxbound-1

     bool bzeromusgs=false; // ������� �������� ������������ ������������ ��������. 


     // ������� ������� ��������� ������� �������
	 if ((sosedb[inumber].MCB<(ls + lw)) && (sosedb[inumber].MCB >= ls) && (!w[sosedb[inumber].MCB - ls].bsymmetry) && (!w[sosedb[inumber].MCB - ls].bpressure) && (!w[sosedb[inumber].MCB - ls].bopening)) {
		// ������ �������� �� �������
        // ��� �� ������� ��������� � �� �������� �������.

		 doublereal vel_mag=sqrt(w[sosedb[inumber].MCB-ls].Vx*w[sosedb[inumber].MCB-ls].Vx+
			          w[sosedb[inumber].MCB-ls].Vy*w[sosedb[inumber].MCB-ls].Vy+
					  w[sosedb[inumber].MCB-ls].Vz*w[sosedb[inumber].MCB-ls].Vz);

		 doublereal epsilon0=1e-32;

		 if (fabs(vel_mag)<epsilon0) {
			 // �������� �� ������ ����� ����.
			 // ������ ��� ������ ����������� ������ �� 
			 // ������� ������������ ������������ �������� ������ ���� ����� ����.
			 bzeromusgs=true;
		 } else bzeromusgs=false;
        
	}
	else if (( (sosedb[inumber].MCB==(ls+lw)) ||(sosedb[inumber].MCB<ls)) ) { // 
		// �������� ���� �������� ������ ����������� �������.
		// ������� �� ��������� ����� �������� ������ ����������� �������.

        // ������� �������� ������������ ��������.
		bzeromusgs=true;

	}
	else  {
		// ��� ��������� �������: ��������, � �������� ���������, �������.
		// �� ��� ������ ������ ���������� ������� �������.
        bzeromusgs=false;
	}

	 if (bzeromusgs) {
			 // �� ������ ����������� ������ �������� ������������
		     // ������������ �������� ����� ����.
		     potent[sosedb[inumber].iB]=0.0; 
	} else  {
		// ���������� ������� �������.
		// ����� ������� �� ��������� �������� ������������ �������� ������� ��������� ������� �� �������.
		potent[sosedb[inumber].iB]=potent[sosedb[inumber].iI];
	}
} // my_boundary_musgs_LES

  // ��������� ��������� ������� ����� ������� ��������� �� ���������� �������� ���
  // ����� ��������� ��������� (� ������ ������ ������� ����� ��������� ���������).
void mass_balance(FLOW &f, integer lw, integer ls, WALL* w)
{
	// 6 ������� 2016 ������ � ����������� �� ����������� ���������������� ����������.
	bool bOk = true;
	integer imarker_fluid_color = 0; // ��������������� ��� ���� ������ ������� � �������.

	bool nobcont = true;
	for (integer i_1 = 0; i_1 < lw; i_1++) {
		if ((!w[i_1].bopening) && (sqrt((w[i_1].Vx)*(w[i_1].Vx) + (w[i_1].Vy)*(w[i_1].Vy) + (w[i_1].Vz)*(w[i_1].Vz)) > 1.0e-20)) {
			nobcont = false;
		}
		if (w[i_1].bopening) nobcont = false;
	}

	// ���� � ��� ���� opening �� �� ������ ���������. 
	// ���� �� � ��� ��� ������ � �������� ���������� �� �� ������� ��������� �� ������.
	if (nobcont) bOk = false;

	imarker_fluid_color = 0;

	while (bOk) {
		bOk = false;
		imarker_fluid_color++;


		doublereal Gset = 0.0; // �������� ������.
		doublereal rhosquare = 0.0;
		doublereal mysquare = 0.0; // ������ ������� �������� ������� (��� ����� �������� �������� ������).
		doublereal deltavel = 0.0; // ���������� �������� � �������� �� �������� �������.
								   // ����� �������, ��� ���� �� ������� ������ ���������� ���������� ��������, ��
								   // �� �������� ����� ������ ������� ������ ��������� �������. ����� ��� �����������
								   // ����� �����.
		for (integer inumber = 0; inumber < f.maxbound; inumber++) {
			if ((f.sosedb[inumber].MCB < (ls + lw)) && (f.sosedb[inumber].MCB >= ls) && (!w[f.sosedb[inumber].MCB - ls].bsymmetry) && (!w[f.sosedb[inumber].MCB - ls].bpressure && (!w[f.sosedb[inumber].MCB - ls].bopening))) {
				if (f.icolor_different_fluid_domain[f.sosedb[inumber].iI] == imarker_fluid_color) {
					bOk = true;

					doublereal dS, rho; // ������� �����.
					rho = f.prop_b[RHO][f.sosedb[inumber].iB - f.maxelm];
					// ��������� ���������� �������.
					switch (f.sosedb[inumber].Norm) {
					case ESIDE:  dS = f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].y;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						Gset += rho*dS*fabs(w[f.sosedb[inumber].MCB - ls].Vx);
						mysquare += dS;
						break;
					case WSIDE:  dS = f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].y;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						Gset += rho*dS*fabs(w[f.sosedb[inumber].MCB - ls].Vx);
						mysquare += dS;
						break;
					case NSIDE:  dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						Gset += rho*dS*fabs(w[f.sosedb[inumber].MCB - ls].Vy);
						mysquare += dS;
						break;
					case SSIDE: dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						Gset += rho*dS*fabs(w[f.sosedb[inumber].MCB - ls].Vy);
						mysquare += dS;
						break;
					case TSIDE:  dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].y); // ������� �����
						Gset += rho*dS*fabs(w[f.sosedb[inumber].MCB - ls].Vz);
						mysquare += dS;
						break;
					case BSIDE:  dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].y); // ������� �����
						Gset += rho*dS*fabs(w[f.sosedb[inumber].MCB - ls].Vz);
						mysquare += dS;
						break;
					}
				}

			}
		}
		//printf("Gset=%e\n",Gset);
		//getchar();
		//system("pause");
		// ����, �� ��������� Gset - �������� ������.
		// �������� �������� �� ������� ������������ ��������� ���������.
		doublereal istinnjiRashod = 0.0; // ������ ���������� ����� ������� ��������� �� ��������.
		for (integer inumber = 0; inumber < f.maxbound; inumber++) {
			if ((f.sosedb[inumber].MCB < (ls + lw)) && (f.sosedb[inumber].MCB >= ls) && ((w[f.sosedb[inumber].MCB - ls].bpressure) || w[f.sosedb[inumber].MCB - ls].bopening)) {
				if (f.icolor_different_fluid_domain[f.sosedb[inumber].iI] == imarker_fluid_color) {

					doublereal dS, rho; // ������� �����.
					rho = f.prop_b[RHO][f.sosedb[inumber].iB - f.maxelm];
					// ��������� ���������� �������.
					switch (f.sosedb[inumber].Norm) {
					case ESIDE:  dS = f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].y;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						istinnjiRashod -= rho*dS*f.potent[VX][f.sosedb[inumber].iB];
						rhosquare += rho*dS;
						mysquare += dS;
						break;
					case WSIDE:  dS = f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].y;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						istinnjiRashod += rho*dS*f.potent[VX][f.sosedb[inumber].iB];
						rhosquare += rho*dS;
						mysquare += dS;
						break;
					case NSIDE:  dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						istinnjiRashod -= rho*dS*f.potent[VY][f.sosedb[inumber].iB];
						rhosquare += rho*dS;
						mysquare += dS;
						break;
					case SSIDE: dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						istinnjiRashod += rho*dS*f.potent[VY][f.sosedb[inumber].iB];
						rhosquare += rho*dS;
						mysquare += dS;
						break;
					case TSIDE:  dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].y); // ������� �����
						istinnjiRashod -= rho*dS*f.potent[VZ][f.sosedb[inumber].iB];
						rhosquare += rho*dS;
						mysquare += dS;
						break;
					case BSIDE:  dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].y); // ������� �����
						istinnjiRashod += rho*dS*f.potent[VZ][f.sosedb[inumber].iB];
						rhosquare += rho*dS;
						mysquare += dS;
						break;
					}
				}
			}
		}

		//printf("istinnjiRashod=%e\n",istinnjiRashod);
		//getchar();
		//system("pause");
		// ��������� ���������� �������� � ��������� ������ �� �������� �������
		// � ����������� � �������� ��������. ��. �������� ������ ���� CFD. sigma flow.

		//deltavel=(Gset-istinnjiRashod)/rhosquare;
		deltavel = (Gset - istinnjiRashod) / mysquare; // ��. �������� ������.
													   // ������������ �������������� ���������� ��������.
													   // ����� ���� �� ������������ ������������ ���������� ������� �������.
		for (integer inumber = 0; inumber < f.maxbound; inumber++) {
			if ((f.sosedb[inumber].MCB < (ls + lw)) && (f.sosedb[inumber].MCB >= ls) && ((w[f.sosedb[inumber].MCB - ls].bpressure) || (w[f.sosedb[inumber].MCB - ls].bopening))) {
				if (f.icolor_different_fluid_domain[f.sosedb[inumber].iI] == imarker_fluid_color) {

					doublereal dS, rho; // ������� �����.
					rho = f.prop_b[RHO][f.sosedb[inumber].iB - f.maxelm];

					bool bperpendicularzero = false; // ���������������� ������������ �������� � �������� ������� ����� ����.

													 // ��������� ���������� �������.
													 // ����� �������� ����������������� �������� ������ � ��������� ���� �� ������ ��������� ���������� ����.
					switch (f.sosedb[inumber].Norm) {
					case ESIDE:  f.potent[VX][f.sosedb[inumber].iB] += deltavel / rho;
						f.potent[VXCOR][f.sosedb[inumber].iB] += deltavel / rho;
						//f.potent[VX][f.sosedb[inumber].iI]=f.potent[VX][f.sosedb[inumber].iB];
						dS = f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].y;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						f.mf[f.sosedb[inumber].iI][WSIDE] += deltavel*dS;
						// ��������� ���������������� ������������ �������� ������ ����.
						if (bperpendicularzero) {
							f.potent[VY][f.sosedb[inumber].iB] = 0.0;
							f.potent[VYCOR][f.sosedb[inumber].iB] = 0.0;
							f.potent[VZ][f.sosedb[inumber].iB] = 0.0;
							f.potent[VZCOR][f.sosedb[inumber].iB] = 0.0;
						}
						break;
					case WSIDE:  f.potent[VX][f.sosedb[inumber].iB] += deltavel / rho;
						f.potent[VXCOR][f.sosedb[inumber].iB] += deltavel / rho;
						//f.potent[VX][f.sosedb[inumber].iI]=f.potent[VX][f.sosedb[inumber].iB];
						dS = f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].y;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						f.mf[f.sosedb[inumber].iI][ESIDE] += deltavel*dS;
						// ��������� ���������������� ������������ �������� ������ ����.
						if (bperpendicularzero) {
							f.potent[VY][f.sosedb[inumber].iB] = 0.0;
							f.potent[VYCOR][f.sosedb[inumber].iB] = 0.0;
							f.potent[VZ][f.sosedb[inumber].iB] = 0.0;
							f.potent[VZCOR][f.sosedb[inumber].iB] = 0.0;
						}
						break;
					case NSIDE:  f.potent[VY][f.sosedb[inumber].iB] += deltavel / rho;
						f.potent[VYCOR][f.sosedb[inumber].iB] += deltavel / rho;
						//f.potent[VY][f.sosedb[inumber].iI]=f.potent[VY][f.sosedb[inumber].iB];
						dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						f.mf[f.sosedb[inumber].iI][SSIDE] += deltavel*dS;
						// ��������� ���������������� ������������ �������� ������ ����.
						if (bperpendicularzero) {
							f.potent[VX][f.sosedb[inumber].iB] = 0.0;
							f.potent[VXCOR][f.sosedb[inumber].iB] = 0.0;
							f.potent[VZ][f.sosedb[inumber].iB] = 0.0;
							f.potent[VZCOR][f.sosedb[inumber].iB] = 0.0;
						}
						break;
					case SSIDE: f.potent[VY][f.sosedb[inumber].iB] += deltavel / rho;
						f.potent[VYCOR][f.sosedb[inumber].iB] += deltavel / rho;
						//f.potent[VY][f.sosedb[inumber].iI]=f.potent[VY][f.sosedb[inumber].iB];
						dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						f.mf[f.sosedb[inumber].iI][NSIDE] += deltavel*dS;
						// ��������� ���������������� ������������ �������� ������ ����.
						if (bperpendicularzero) {
							f.potent[VX][f.sosedb[inumber].iB] = 0.0;
							f.potent[VXCOR][f.sosedb[inumber].iB] = 0.0;
							f.potent[VZ][f.sosedb[inumber].iB] = 0.0;
							f.potent[VZCOR][f.sosedb[inumber].iB] = 0.0;
						}
						break;
					case TSIDE:  f.potent[VZ][f.sosedb[inumber].iB] += deltavel / rho;
						f.potent[VZCOR][f.sosedb[inumber].iB] += deltavel / rho;
						//f.potent[VZ][f.sosedb[inumber].iI]=f.potent[VZ][f.sosedb[inumber].iB];
						dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].y); // ������� ����� 
						f.mf[f.sosedb[inumber].iI][BSIDE] += deltavel*dS;
						// ��������� ���������������� ������������ �������� ������ ����.
						if (bperpendicularzero) {
							f.potent[VX][f.sosedb[inumber].iB] = 0.0;
							f.potent[VXCOR][f.sosedb[inumber].iB] = 0.0;
							f.potent[VY][f.sosedb[inumber].iB] = 0.0;
							f.potent[VYCOR][f.sosedb[inumber].iB] = 0.0;
						}
						break;
					case BSIDE:  f.potent[VZ][f.sosedb[inumber].iB] += deltavel / rho;
						f.potent[VZCOR][f.sosedb[inumber].iB] += deltavel / rho;
						//f.potent[VZ][f.sosedb[inumber].iI]=f.potent[VZ][f.sosedb[inumber].iB];
						dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].y); // ������� �����
						f.mf[f.sosedb[inumber].iI][TSIDE] += deltavel*dS;
						// ��������� ���������������� ������������ �������� ������ ����.
						if (bperpendicularzero) {
							f.potent[VX][f.sosedb[inumber].iB] = 0.0;
							f.potent[VXCOR][f.sosedb[inumber].iB] = 0.0;
							f.potent[VY][f.sosedb[inumber].iB] = 0.0;
							f.potent[VYCOR][f.sosedb[inumber].iB] = 0.0;
						}
						break;
					}
				}
			}
		}
	} // ���� �� ���� ����������� ����������������� �����������.
} // mass_balance


// ��������� ��������� ������� ����� ������� ��������� �� ���������� �������� ���
// ����� ��������� ��������� (� ������ ������ ������� ����� ��������� ���������).
void mass_balance_09_5_2017(FLOW &f, integer lw, integer ls, WALL* w) 
{
	// 6 ������� 2016 ������ � ����������� �� ����������� ���������������� ����������.
	bool bOk = true;
	integer imarker_fluid_color = 0; // ��������������� ��� ���� ������ ������� � �������.

	bool nobcont = true;
	for (integer i_1 = 0; i_1 < lw; i_1++) {
		if ((!w[i_1].bopening)&&(sqrt((w[i_1].Vx)*(w[i_1].Vx) + (w[i_1].Vy)*(w[i_1].Vy) + (w[i_1].Vz)*(w[i_1].Vz)) > 1.0e-20) ){
			nobcont = false;
	    }
		if (w[i_1].bopening) nobcont = false;
	}

	// ���� � ��� ���� opening �� �� ������ ���������. 
	// ���� �� � ��� ��� ������ � �������� ���������� �� �� ������� ��������� �� ������.
	if (nobcont) bOk = false;

	imarker_fluid_color = 0;

	while (bOk) {
		bOk = false;
		imarker_fluid_color++;


		doublereal Gset = 0.0; // �������� ������.
		// ���������� ���������� �� ��������� ������� �������� �������� ������������� � ��������� ��������.
		doublereal rhosquare = 0.0;
		doublereal mysquare = 0.0; // ������ ������� �������� ������� (��� ����� �������� �������� ������).
		doublereal deltavel = 0.0; // ���������� �������� � �������� �� �������� �������.
		// ����� �������, ��� ���� �� ������� ������ ���������� ���������� ��������, ��
		// �� �������� ����� ������ ������� ������ ��������� �������. ����� ��� �����������
		// ����� �����.
		for (integer inumber = 0; inumber < f.maxbound; inumber++) {
			if ((f.sosedb[inumber].MCB < (ls + lw)) && (f.sosedb[inumber].MCB >= ls) && (!w[f.sosedb[inumber].MCB - ls].bsymmetry) && (!w[f.sosedb[inumber].MCB - ls].bpressure && (!w[f.sosedb[inumber].MCB - ls].bopening))) {
				if (f.icolor_different_fluid_domain[f.sosedb[inumber].iI] == imarker_fluid_color) {
					bOk = true;

					if (w[f.sosedb[inumber].MCB - ls].Vx*w[f.sosedb[inumber].MCB - ls].Vx + w[f.sosedb[inumber].MCB - ls].Vy*w[f.sosedb[inumber].MCB - ls].Vy + w[f.sosedb[inumber].MCB - ls].Vz*w[f.sosedb[inumber].MCB - ls].Vz > 1.0e-20) {
						// ������ �� ������� ������� �� ������� ������������� ������ ��������� ��������.


						doublereal dS, rho; // ������� �����.
						rho = f.prop_b[RHO][f.sosedb[inumber].iB - f.maxelm];
						// ��������� ���������� �������.
						switch (f.sosedb[inumber].Norm) {
						case ESIDE:  dS = f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].y;
							dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
							Gset -= rho*dS*(w[f.sosedb[inumber].MCB - ls].Vx);
							// ������ ��������� ���������� ���������� ��������.
							if (w[f.sosedb[inumber].MCB - ls].Vx*w[f.sosedb[inumber].MCB - ls].Vx > 1.0e-20) {
								mysquare += dS;
								rhosquare += rho*dS;
							}
							break;
						case WSIDE:  dS = f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].y;
							dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
							Gset += rho*dS*(w[f.sosedb[inumber].MCB - ls].Vx);
							if (w[f.sosedb[inumber].MCB - ls].Vx*w[f.sosedb[inumber].MCB - ls].Vx > 1.0e-20) {
								mysquare += dS;
								rhosquare += rho*dS;
							}
							break;
						case NSIDE:  dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
							dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
							Gset -= rho*dS*(w[f.sosedb[inumber].MCB - ls].Vy);
							if (w[f.sosedb[inumber].MCB - ls].Vy*w[f.sosedb[inumber].MCB - ls].Vy > 1.0e-20) {
								mysquare += dS;
								rhosquare += rho*dS;
							}
							break;
						case SSIDE: dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
							dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
							Gset += rho*dS*(w[f.sosedb[inumber].MCB - ls].Vy);
							if (w[f.sosedb[inumber].MCB - ls].Vy*w[f.sosedb[inumber].MCB - ls].Vy > 1.0e-20) {
								mysquare += dS;
								rhosquare += rho*dS;
							}
							break;
						case TSIDE:  dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
							dS *= (f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].y); // ������� �����
							Gset -= rho*dS*(w[f.sosedb[inumber].MCB - ls].Vz);
							if (w[f.sosedb[inumber].MCB - ls].Vz*w[f.sosedb[inumber].MCB - ls].Vz > 1.0e-20) {
								mysquare += dS;
								rhosquare += rho*dS;
							}
							break;
						case BSIDE:  dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
							dS *= (f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].y); // ������� �����
							Gset += rho*dS*(w[f.sosedb[inumber].MCB - ls].Vz);
							if (w[f.sosedb[inumber].MCB - ls].Vz*w[f.sosedb[inumber].MCB - ls].Vz > 1.0e-20) {
								mysquare += dS;
								rhosquare += rho*dS;
							}
							break;
						}
					}
				}

			}
		}
		//printf("Gset=%e\n",Gset);
		//getchar();
		//system("pause");
		// ����, �� ��������� Gset - �������� ������. ��� ���������� ���������� �� ��������� ������� �������� � ������ ������������ � ���
		// ��� ����� ������������ � ��������� ��������.
		// �������� �������� �� ������� ������������ ��������� ���������.
		doublereal istinnjiRashod = 0.0; // ������ ���������� ����� ������� ��������� �� ��������.
		// istinnjiRashod - ���������� ��������� �������� ����� pressure � opening �������.
		for (integer inumber = 0; inumber < f.maxbound; inumber++) {
			if ((f.sosedb[inumber].MCB < (ls + lw)) && (f.sosedb[inumber].MCB >= ls) && ((w[f.sosedb[inumber].MCB - ls].bpressure) || w[f.sosedb[inumber].MCB - ls].bopening)) {
				if (f.icolor_different_fluid_domain[f.sosedb[inumber].iI] == imarker_fluid_color) {

					doublereal dS, rho; // ������� �����.
					rho = f.prop_b[RHO][f.sosedb[inumber].iB - f.maxelm];
					// ��������� ���������� �������.
					switch (f.sosedb[inumber].Norm) {
					case ESIDE:  dS = f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].y;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						istinnjiRashod -= rho*dS*f.potent[VX][f.sosedb[inumber].iB];
						rhosquare += rho*dS;
						mysquare += dS;
						break;
					case WSIDE:  dS = f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].y;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						istinnjiRashod += rho*dS*f.potent[VX][f.sosedb[inumber].iB];
						rhosquare += rho*dS;
						mysquare += dS;
						break;
					case NSIDE:  dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						istinnjiRashod -= rho*dS*f.potent[VY][f.sosedb[inumber].iB];
						rhosquare += rho*dS;
						mysquare += dS;
						break;
					case SSIDE : dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						istinnjiRashod += rho*dS*f.potent[VY][f.sosedb[inumber].iB];
						rhosquare += rho*dS;
						mysquare += dS;
						break;
					case TSIDE:  dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].y); // ������� �����
						istinnjiRashod -= rho*dS*f.potent[VZ][f.sosedb[inumber].iB];
						rhosquare += rho*dS;
						mysquare += dS;
						break;
					case BSIDE :  dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].y); // ������� �����
						istinnjiRashod += rho*dS*f.potent[VZ][f.sosedb[inumber].iB];
						rhosquare += rho*dS;
						mysquare += dS;
						break;
					}
				}
			}
		}

		//printf("istinnjiRashod=%e\n",istinnjiRashod);
		//getchar();
		//system("pause");
		// ��������� ���������� �������� � ��������� ������ �� �������� �������
		// � ����������� � �������� ��������. ��. �������� ������ ���� CFD. sigma flow.

		doublereal alpharelax = 1.0;
		doublereal v_zv_zv;

		deltavel=-(Gset-istinnjiRashod)/rhosquare;
		//deltavel = -(Gset - istinnjiRashod) / mysquare; // ��. �������� ������.
		// deltavel - ��� ��������� ��������� �� �������� ������ �������� �������� ������ ���������� �� ��������� ������� ��������.
		// � ������ ��� �������� ������ ���� ����� ����.
		// ������ ���������� : ��� �������� �� ��������� �� ��� �������� (����� deltavel),
		// ��� ������� �� �� ����������� �������� �� ��� �������� (���� deltavel)
		// ���� ��������� �� �������� ��������� �������� �������� ��������� �� ��� ��������.
		// ������������ �������������� ���������� ��������.
		// ����� ���� �� ������������ ������������ ���������� ������� �������.
		for (integer inumber = 0; inumber < f.maxbound; inumber++) {
			if ((f.sosedb[inumber].MCB < (ls + lw)) && (f.sosedb[inumber].MCB >= ls) && ((w[f.sosedb[inumber].MCB - ls].bpressure) || (w[f.sosedb[inumber].MCB - ls].bopening))) {
				if (f.icolor_different_fluid_domain[f.sosedb[inumber].iI] == imarker_fluid_color) {

					doublereal dS, rho; // ������� �����.
					rho = f.prop_b[RHO][f.sosedb[inumber].iB - f.maxelm];

					bool bperpendicularzero = false; // ���������������� ������������ �������� � �������� ������� ����� ����.

					// ��������� ���������� �������.
					// ����� �������� ����������������� �������� ������ � ��������� ���� �� ������ ��������� ���������� ����.
					switch (f.sosedb[inumber].Norm) {
					case ESIDE: 
						v_zv_zv = f.potent[VXCOR][f.sosedb[inumber].iB] + deltavel;
						//f.potent[VX][f.sosedb[inumber].iB] += deltavel;
						//f.potent[VXCOR][f.sosedb[inumber].iB] += deltavel;
						f.potent[VX][f.sosedb[inumber].iB] = alpharelax*(v_zv_zv) + (1 - alpharelax)*f.potent[VX][f.sosedb[inumber].iB];
						f.potent[VXCOR][f.sosedb[inumber].iB] = alpharelax*(v_zv_zv)+(1 - alpharelax)*f.potent[VXCOR][f.sosedb[inumber].iB];
						//f.potent[VX][f.sosedb[inumber].iI]=f.potent[VX][f.sosedb[inumber].iB];
						dS = f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].y;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						//f.mf[f.sosedb[inumber].iI][WSIDE] += rho*deltavel*dS;
						f.mf[f.sosedb[inumber].iI][WSIDE] = alpharelax*(v_zv_zv*rho*dS) + (1 - alpharelax)*f.mf[f.sosedb[inumber].iI][WSIDE];
						// ��������� ���������������� ������������ �������� ������ ����.
						if (bperpendicularzero) {
							f.potent[VY][f.sosedb[inumber].iB] = 0.0;
							f.potent[VYCOR][f.sosedb[inumber].iB] = 0.0;
							f.potent[VZ][f.sosedb[inumber].iB] = 0.0;
							f.potent[VZCOR][f.sosedb[inumber].iB] = 0.0;
						}
						break;
					case WSIDE:
						v_zv_zv = f.potent[VXCOR][f.sosedb[inumber].iB] - deltavel;
						//f.potent[VX][f.sosedb[inumber].iB] -= deltavel;
						//f.potent[VXCOR][f.sosedb[inumber].iB] -= deltavel;
						f.potent[VX][f.sosedb[inumber].iB] = alpharelax*(v_zv_zv)+(1 - alpharelax)*f.potent[VX][f.sosedb[inumber].iB];
						f.potent[VXCOR][f.sosedb[inumber].iB] = alpharelax*(v_zv_zv)+(1 - alpharelax)*f.potent[VXCOR][f.sosedb[inumber].iB];
						//f.potent[VX][f.sosedb[inumber].iI]=f.potent[VX][f.sosedb[inumber].iB];
						dS = f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].y;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						//f.mf[f.sosedb[inumber].iI][ESIDE] -= rho*deltavel*dS;
						f.mf[f.sosedb[inumber].iI][ESIDE] = alpharelax*(v_zv_zv*rho*dS) + (1 - alpharelax)*f.mf[f.sosedb[inumber].iI][ESIDE];
						// ��������� ���������������� ������������ �������� ������ ����.
						if (bperpendicularzero) {
							f.potent[VY][f.sosedb[inumber].iB] = 0.0;
							f.potent[VYCOR][f.sosedb[inumber].iB] = 0.0;
							f.potent[VZ][f.sosedb[inumber].iB] = 0.0;
							f.potent[VZCOR][f.sosedb[inumber].iB] = 0.0;
						}
						break;
					case NSIDE:  
						v_zv_zv = f.potent[VYCOR][f.sosedb[inumber].iB] + deltavel;
						//f.potent[VY][f.sosedb[inumber].iB] += deltavel;
						//f.potent[VYCOR][f.sosedb[inumber].iB] += deltavel;
						f.potent[VY][f.sosedb[inumber].iB] = alpharelax*(v_zv_zv)+(1 - alpharelax)*f.potent[VY][f.sosedb[inumber].iB];
						f.potent[VYCOR][f.sosedb[inumber].iB] = alpharelax*(v_zv_zv)+(1 - alpharelax)*f.potent[VYCOR][f.sosedb[inumber].iB];
						//f.potent[VY][f.sosedb[inumber].iI]=f.potent[VY][f.sosedb[inumber].iB];
						dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						//f.mf[f.sosedb[inumber].iI][SSIDE] += rho*deltavel*dS;
						f.mf[f.sosedb[inumber].iI][SSIDE] = alpharelax*(v_zv_zv*rho*dS) + (1 - alpharelax)*f.mf[f.sosedb[inumber].iI][SSIDE];
						// ��������� ���������������� ������������ �������� ������ ����.
						if (bperpendicularzero) {
							f.potent[VX][f.sosedb[inumber].iB] = 0.0;
							f.potent[VXCOR][f.sosedb[inumber].iB] = 0.0;
							f.potent[VZ][f.sosedb[inumber].iB] = 0.0;
							f.potent[VZCOR][f.sosedb[inumber].iB] = 0.0;
						}
						break;
					case SSIDE : 
						v_zv_zv = f.potent[VYCOR][f.sosedb[inumber].iB] - deltavel;
						// �������� � �� ��������� �� ������ ����������.
						//f.potent[VY][f.sosedb[inumber].iB] -= deltavel;
						//f.potent[VYCOR][f.sosedb[inumber].iB] -= deltavel;
						f.potent[VY][f.sosedb[inumber].iB] = alpharelax*(v_zv_zv)+(1 - alpharelax)*f.potent[VY][f.sosedb[inumber].iB];
						f.potent[VYCOR][f.sosedb[inumber].iB] = alpharelax*(v_zv_zv)+(1 - alpharelax)*f.potent[VYCOR][f.sosedb[inumber].iB];
						//f.potent[VY][f.sosedb[inumber].iI]=f.potent[VY][f.sosedb[inumber].iB];
						dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[4][f.sosedb[inumber].iI] - 1].z - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].z); // ������� �����
						//f.mf[f.sosedb[inumber].iI][NSIDE] -= rho*deltavel*dS;
						f.mf[f.sosedb[inumber].iI][NSIDE] = alpharelax*(v_zv_zv*rho*dS) + (1 - alpharelax)*f.mf[f.sosedb[inumber].iI][NSIDE];
						// ��������� ���������������� ������������ �������� ������ ����.
						if (bperpendicularzero) {
							f.potent[VX][f.sosedb[inumber].iB] = 0.0;
							f.potent[VXCOR][f.sosedb[inumber].iB] = 0.0;
							f.potent[VZ][f.sosedb[inumber].iB] = 0.0;
							f.potent[VZCOR][f.sosedb[inumber].iB] = 0.0;
						}
						break;
					case TSIDE: 
						v_zv_zv = f.potent[VZCOR][f.sosedb[inumber].iB] + deltavel;
						//f.potent[VZ][f.sosedb[inumber].iB] += deltavel;
						//f.potent[VZCOR][f.sosedb[inumber].iB] += deltavel;
						f.potent[VZ][f.sosedb[inumber].iB] = alpharelax*(v_zv_zv)+(1 - alpharelax)*f.potent[VZ][f.sosedb[inumber].iB];
						f.potent[VZCOR][f.sosedb[inumber].iB] = alpharelax*(v_zv_zv)+(1 - alpharelax)*f.potent[VZCOR][f.sosedb[inumber].iB];
						//f.potent[VZ][f.sosedb[inumber].iI]=f.potent[VZ][f.sosedb[inumber].iB];
						dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].y); // ������� ����� 
						//f.mf[f.sosedb[inumber].iI][BSIDE] += rho*deltavel*dS;
						f.mf[f.sosedb[inumber].iI][BSIDE] = alpharelax*(v_zv_zv*rho*dS) + (1 - alpharelax)*f.mf[f.sosedb[inumber].iI][BSIDE];
						// ��������� ���������������� ������������ �������� ������ ����.
						if (bperpendicularzero) {
							f.potent[VX][f.sosedb[inumber].iB] = 0.0;
							f.potent[VXCOR][f.sosedb[inumber].iB] = 0.0;
							f.potent[VY][f.sosedb[inumber].iB] = 0.0;
							f.potent[VYCOR][f.sosedb[inumber].iB] = 0.0;
						}
						break;
					case BSIDE : 
						v_zv_zv = f.potent[VZCOR][f.sosedb[inumber].iB] - deltavel;
						// �������� � �� ��������� �� ������ ����������.
						//f.potent[VZ][f.sosedb[inumber].iB] -= deltavel;
						//f.potent[VZCOR][f.sosedb[inumber].iB] -= deltavel;
						f.potent[VZ][f.sosedb[inumber].iB] = alpharelax*(v_zv_zv)+(1 - alpharelax)*f.potent[VZ][f.sosedb[inumber].iB];
						f.potent[VZCOR][f.sosedb[inumber].iB] = alpharelax*(v_zv_zv)+(1 - alpharelax)*f.potent[VZCOR][f.sosedb[inumber].iB];
						//f.potent[VZ][f.sosedb[inumber].iI]=f.potent[VZ][f.sosedb[inumber].iB];
						dS = f.pa[f.nvtx[1][f.sosedb[inumber].iI] - 1].x - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].x;
						dS *= (f.pa[f.nvtx[2][f.sosedb[inumber].iI] - 1].y - f.pa[f.nvtx[0][f.sosedb[inumber].iI] - 1].y); // ������� �����
						//f.mf[f.sosedb[inumber].iI][TSIDE] -= rho*deltavel*dS;
						f.mf[f.sosedb[inumber].iI][TSIDE] = alpharelax*(v_zv_zv*rho*dS) + (1 - alpharelax)*f.mf[f.sosedb[inumber].iI][TSIDE];
						// ��������� ���������������� ������������ �������� ������ ����.
						if (bperpendicularzero) {
							f.potent[VX][f.sosedb[inumber].iB] = 0.0;
							f.potent[VXCOR][f.sosedb[inumber].iB] = 0.0;
							f.potent[VY][f.sosedb[inumber].iB] = 0.0;
							f.potent[VYCOR][f.sosedb[inumber].iB] = 0.0;
						}
						break;
					}
				}
			}
		}
	} // ���� �� ���� ����������� ����������������� �����������.
} // mass_balance


// ����������� ��������� ��� ����������� �������� ��������� �.��������, �.�������� 1972���.
void my_version_SIMPLE_Algorithm3D(doublereal &continity, integer inumiter, FLOW &f, FLOW* &fglobal, TEMPER &t, doublereal** &rhie_chow,
								   BLOCK* &b, integer lb, SOURCE* &s, integer ls, WALL* &w, integer lw, doublereal dbeta,
								   integer flow_interior, integer iflow, bool bfirst_start, doublereal dgx, doublereal dgy, doublereal dgz,
								   TPROP* &matlist, bool btimedep, doublereal dtimestep, doublereal dtimestepold, doublereal phisicaltime,
								   doublereal* toldtimestep, doublereal** speedoldtimestep, doublereal** mfoldtimestep, 
								   bool bprintmessage, TEMP_DEP_POWER* gtdps, integer ltdp, FLUENT_RESIDUAL &rfluentres,
								   doublereal &rfluentrestemp, doublereal* &smagconstolditer, doublereal** &mfold, integer itempersolve, 
								   QuickMemVorst& m, bool bextendedprint, doublereal** &SpeedCorOld, doublereal* &xb,
								   doublereal* &rthdsd, doublereal* &rthdsdt, integer lu, UNION* &my_union) {

	

	// �������� ������ �� �������� ������� ��������������� ��������� �������������.
	mass_balance(f, lw, ls, w); // ��� ����� �����. ��. �������� ������.
	// �������� 6.05.2017
	
	// rfluentrestemp ������� � ����� Fluent ��� �����������.
	// ���� itempersolve == 0 �� ��������� ���������������� ������ �������, 
	// � ���� itempersolve== 1 �� ��������� ������ ����� � ��������� ����������������.

	bdontstartsolver = false;

	doublereal RCh=1.0;
	// ���� bVERYStable==true �� �� ��������� � ������������ ��������������� ��������:
	// 1. ���������� �� ������� ������������� ������� ������� dbeta==1.0;
	// 2. ���������� ��� ������������� ������������� ����� ����� ������� ������� (���������������).
	// ����� ������� ��� ������ 100 �������� ����� ��������� ����� bVERYStable==true. �� 100 �������� ������� 
	// "�����������" � ����� ����� ��������� ����� ����� �������� ������� �������� �� ���� ������ ���������� ���������.
	bool bVERYStable=true;
	if (inumiter>2000) bVERYStable=false; // ��������� � ������������� �������� �������.

	if (0) {
		xyplot( fglobal, flow_interior, t);
		printf("my version SIMPLE Algorith. OK.\n");
	    //getchar(); // debug avtosave
		system("pause");
	}

	// ���� bfirst_start==true ������ �� ����� ���� � ������ ��������� ��������� SIMPLE.
	// smagconstolditer - ��������� ������������� � ���������� ��������. (������������ ��� ������ ����������).
	const doublereal smagconstURF=0.001; // ������������� � ���������� ��������.

	// inumiter - ����� ��������.
	if (bprintmessage) {
#if doubleintprecision == 1
		printf("inumber iter SIMPLEC=%lld\n", inumiter);
		fprintf(fp_log, "inumber iter SIMPLEC=%lld\n", inumiter);
#else
		printf("inumber iter SIMPLEC=%d\n", inumiter);
		fprintf(fp_log, "inumber iter SIMPLEC=%d\n", inumiter);
#endif
		
	}

	//printf("turb\n");
	//getchar();


	// ��������� ���� �������� �������������� ��������.
	
	doublereal res=0.0;
	// ������� ������������� ���� ���������:
	// ��������� ���� �� ������������ ������ ���������� !!!.

	// UPDATE PROPERTIES 
	// ��������� �������� ���������� ��������� �� ����������� � ��������:
	// ���������, ������������ ��������, ����������� ��������� �������������� ����������.
    update_flow_properties(t, fglobal, b, lb, flow_interior, matlist, bfirst_start);
	

	// � ������ Zero Equation Turbulence Model ����������
	// ������������ ������������ ������������ ��������.
	if (f.iflowregime==ZEROEQMOD) {
		for (integer i=0; i<(f.maxelm+f.maxbound); i++) {
			doublereal rho=0.0; // ���������� ���������.
			if (i<f.maxelm) rho=f.prop[RHO][i];
			else rho=f.prop_b[RHO][i-f.maxelm];
			doublereal PrandtlLength=fmin(0.419*f.rdistWall[i],0.09*f.rdistWallmax); // ������� ��������� (1966).
			//f.potent[MUT][i]=rho*PrandtlLength*PrandtlLength*f.SInvariantStrainRateTensor[i];
			if (i < f.maxelm) {
				f.potent[MUT][i] = rho*PrandtlLength*PrandtlLength*f.potent[CURL][i]/sqrt(2.0);
			}
			else {
				f.potent[MUT][i] = rho*PrandtlLength*PrandtlLength*f.potent[CURL][i]/sqrt(2.0);
			}
			if (0&&(inumiter == 40)) {
				printf("rho=%e dw=%e dwmax=%e plen=%e sigma=%e\n", rho, f.rdistWall[i], f.rdistWallmax, PrandtlLength, f.SInvariantStrainRateTensor[i]);
				system("pause");
			}
	    }
		// ������������� ������� ����� ��������� �������� ��������� � ���������� �����������.
		// ��� ����� ������ ����� ��� ����������� ������� ��������� �������.
		for (integer iP=0; iP<f.maxelm; iP++) {
            integer iE, iN, iT, iW, iS, iB; // ������ �������� ����������� �������
			iE = f.sosedi[ESIDE][iP].iNODE1; iN = f.sosedi[NSIDE][iP].iNODE1; iT = f.sosedi[TSIDE][iP].iNODE1; iW = f.sosedi[WSIDE][iP].iNODE1; iS = f.sosedi[SSIDE][iP].iNODE1; iB = f.sosedi[BSIDE][iP].iNODE1;
			if (iE>=f.maxelm) {
				f.potent[MUT][iE]=f.potent[MUT][iP];
			}
			if (iW>=f.maxelm) {
				f.potent[MUT][iW]=f.potent[MUT][iP];
			}
			if (iN>=f.maxelm) {
				f.potent[MUT][iN]=f.potent[MUT][iP];
			}
			if (iS>=f.maxelm) {
				f.potent[MUT][iS]=f.potent[MUT][iP];
			}
			if (iT>=f.maxelm) {
				f.potent[MUT][iT]=f.potent[MUT][iP];
			}
			if (iB>=f.maxelm) {
				f.potent[MUT][iB]=f.potent[MUT][iP];
			}
		}
	} // Zero Equation Model

	if (f.iflowregime==SMAGORINSKY) {
		// ���� �� �������������� ������ ������������� (LES �������������).

		// ���������� ��� Selective Smagorinsky
		bool* bfibeta=NULL; // ��������� ������ � ������������� ���������� ������ ������� calc_selective_smagorinsky.

		if (f.smaginfo.bSelectiveSmagorinsky) {
			// ������������ ������ Selective Smagorinsky
			calc_selective_smagorinsky(f, bfibeta, f.smaginfo.itypeFILTRSelectiveSmagorinsky, f.smaginfo.SSangle);
		}

		

		// ������������ ������ ��������������
		// ������� ������������ � 1991 ����.
		// Dynamic Subgrid Scale Model Germano [1991].

		// ������� ���������� �������������, ��������� ������ ���������� ������ ������� my_Germano_model.
		doublereal* Cs2=NULL; // ������ ������ 0..maxelm-1.

		if (f.smaginfo.bDynamic_Stress) {
			my_Germano_model(f,Cs2,f.smaginfo.itypeFiltrGermano); // ����������� �������� ��������� �������������.
			for (integer i=0; i<f.maxelm; i++) {
				doublereal myCs=f.smaginfo.Cs; // ���������� �������������.
				// ������ ��� Cs2 �������� ������� �� 0 �� maxelm-1.
				myCs=sqrt(fabs(Cs2[i]));
				if (Cs2[i]<0.0) myCs*=-1.0;
				// ��������� ������������� � ���������� ������ ����������.
				// ���� ��� �� ������ �� ��������� ������������� ����� ��������� ���������� �������� +,- ������, � ����� ���� � ������.
				// ������ ������ ���������� �� ������� �� ������� ��������, ��� ���� ��������� �������� ���������� ��������.
				myCs=(1.0-smagconstURF)*smagconstolditer[i]+smagconstURF*myCs;
				f.potent[FBUF][i]=myCs; // ������� ����������� ������������� ��������� ������������� � ������ ��� ������� � ��������� tecplot360.
		    }
		}
		

		for (integer i=0; i<f.maxelm; i++) {
			// ���������� ����������� ������.
            
			doublereal rho=0.0; // ���������� ���������.
			rho=f.prop[RHO][i]; // �������� ��������� �� ���������� ����������� ������.
			doublereal dx=0.0, dy=0.0, dz=0.0; // ������� �������� ������������ ������.
			volume3D(i, f.nvtx, f.pa, dx, dy, dz);
			doublereal length2=0.0; // ������� ����� ���� ��������
			doublereal myCs=f.smaginfo.Cs; // ���������� �������������.
			if (f.smaginfo.bDynamic_Stress) {
				myCs=sqrt(fabs(Cs2[i]));
				if (Cs2[i]<0.0) myCs*=-1.0;
			}
			length2=smagorinsky_length2(myCs, dx, dy, dz, f.rdistWall[i],
				                        f.smaginfo.roughness, f.smaginfo.ipowerroughness,
										f.smaginfo.bfdelta, f.smaginfo.bSmagorinsky_Lilly, 
										f.smaginfo.bsurface_roughness);

			doublereal fRichardson=1.0; // �������� ��� ������� � ��������� ����� ����.
			if (f.smaginfo.bRichardsonCorrect) {
				doublereal SInv=f.SInvariantStrainRateTensor[i];
				if (fabs(SInv)<1e-30) {
					// ������� ������ ����� �����������, ������� � �������� �������.
					fRichardson=1.0;
				}
				else {
					// ������������ ����� ����������.
					doublereal Risgs=(f.potent[CURL][i]/SInv)*(f.potent[CURL][i]/SInv)+f.potent[CURL][i]/SInv;
					if ((1.0-f.smaginfo.rRichardsonMultiplyer*Risgs)>0.0) {
                       fRichardson=sqrt(1.0-f.smaginfo.rRichardsonMultiplyer*Risgs);
					}
					else {
						// ������ ����� ��������� ������� ��������� ���������� ������� ���,
						// ��� ����� �������� ���������� ������� � ������ �������� ���������
						// � ��������� ����� ���� �� ����� => ������� �������� 1.0.
						fRichardson=1.0;
					}
				}
			}

			f.potent[MUT][i]=rho*fRichardson*length2*f.SInvariantStrainRateTensor[i];

			if (f.smaginfo.bSelectiveSmagorinsky) {
				// ������������ ������ Selective Smagorinsky
				if (bfibeta[i]) {
				    // ���� ����� ������ � ���������� ������ ����������
				    // ����� ������� ��������� �������� (���������) ��������� �������
				    // �������������� ���������� � ���� ������������ ��������.
					f.potent[MUT][i]=f.potent[MUT][i]*1.0;
			    }
			    else {
				    // ���� ����� ������ � ���������� ������
				    // ���������� ���, ������ 15 ��������, 
				    // ������� ����� ��������� ������������ �����������
				    // ������������ ��������.
				    f.potent[MUT][i]=0.0;
			    }
			}
		}

		if (f.smaginfo.bDynamic_Stress) {
			// ������������ ����������� ������.
		    delete Cs2;
		}

		if (f.smaginfo.bSelectiveSmagorinsky) {
			delete bfibeta; // ������������ ����������� ������.
		}

		// �������� ������������ �������� �� ������ ����������� ������ ����� ����.
		// ��� ���� ��������� ������ ��������� ������� �������� ������������ �������� 
		// ����� �������� ���� ������������ (��������) �� ���������� � ������� ���������
		// ������������ ������.
		for (integer i=0; i<f.maxbound; i++) {
			my_boundary_musgs_LES(i, f.maxelm, f.sosedb, ls, lw,
							  w, f.pa, f.nvtx, f.potent[MUT]);
		}
	}

	if (f.iflowregime==RNG_LES) {
		// ������ �������������� ������������ �� Renormalization Group Theory.
		// � ������ LES �������. ������ ���������� �������������� �������������
		// �������� �� CFD-Wiki.

		for (integer i=0; i<f.maxelm; i++) {
			// ���������� ����������� ������.
            doublereal rho=0.0; // ���������� ���������.
			rho=f.prop[RHO][i]; // �������� ��������� �� ���������� ����������� ������.
			doublereal dx=0.0, dy=0.0, dz=0.0; // ������� �������� ������������ ������.
			volume3D(i, f.nvtx, f.pa, dx, dy, dz);
			doublereal dV=dx*dy*dz; // ����� ������������ ������.
	        //doublereal delta=pow(dV,1.0/3.0); // ���������� ������ �� ������ ������������ ������.
	        doublereal delta=exp((1.0/3.0)*log(dV)); 
			doublereal Crng=0.157; // ��������� RNG_LES ������.
            doublereal length2=(Crng*delta)*(Crng*delta); // ����� �������������.
			doublereal mu_sgs=rho*length2*fabs(f.SInvariantStrainRateTensor[i]);
			doublereal mu_lam=f.prop[MU][i]; 
			doublereal mu_eff=my_dixtomiq_RNG_LES(mu_lam, mu_sgs);
			f.potent[MUT][i]=fmax(mu_eff-mu_lam,0.0); // ������������ ������������ ��������.
		}
		// �������� ������������ �������� �� ������ ����������� ������ ����� ����.
		// ��� ���� ��������� ������ ��������� ������� �������� ������������ �������� 
		// ����� �������� ���� ������������ (��������) �� ���������� � ������� ���������
		// ������������ ������.
		for (integer i=0; i<f.maxbound; i++) {
			my_boundary_musgs_LES(i, f.maxelm, f.sosedb, ls, lw,
							  w, f.pa, f.nvtx, f.potent[MUT]);
		}
	}

	/*
	// ���� ������ �� ������������� �������� ������ �������, � ����������� � � �������� �����
	// � ������ ������������� � ������������ � ���� ��� ����������� �������� ��������� �������������.
	if (f.iflowregime==GERMANO) {
		// ������������ ������ ��������������
		// ������� ������������ � 1991 ����.
		// Dynamic Subgrid Scale Model Germamno [1991].

		// ������� ���������� �������������, ��������� ������ ���������� ������ ������� my_Germano_model.
		doublereal* Cs2; // ������ ������ 0..maxelm-1.
		my_Germano_model(f,Cs2,SIMPSON_FILTR); // ����������� �������� ��������� �������������.

		for (i=0; i<f.maxelm; i++) {
		    // ���������� ����������� ������.
            doublereal rho=0.0; // ���������� ���������.
			rho=f.prop[RHO][i]; // �������� ��������� �� ���������� ����������� ������.
			doublereal dx=0.0, dy=0.0, dz=0.0; // ������� �������� ������������ ������.
			volume3D(i, f.nvtx, f.pa, dx, dy, dz);
			doublereal dV=dx*dy*dz; // ����� ������������ ������.
	        //doublereal delta=pow(dV,1.0/3.0); // ���������� ������ �� ������ ������������ ������.
	        doublereal delta2=exp((2.0/3.0)*log(dV)); 
			doublereal length2=Cs2[i]*delta2; // ������� ���� �������� ��������.
			f.potent[MUT][i]=rho*length2*f.SInvariantStrainRateTensor[i]; // ������������ ������������ ��������.
		}

		// ������������ ����������� ������.
		delete Cs2;

		// �������� ������������ �������� �� ������ ����������� ������ ����� ����.
		// ��� ���� ��������� ������ ��������� ������� �������� ������������ �������� 
		// ����� �������� ���� ������������ (��������) �� ���������� � ������� ���������
		// ������������ ������.
		for (i=0; i<f.maxbound; i++) {
			my_boundary_musgs_LES(i, f.maxelm, f.sosedb, ls, lw,
							  w, f.pa, f.nvtx, f.potent[MUT]);
		}

	}
	*/
	if (0&&(inumiter>=66)) {
		// debug
		integer i=0;
		while (i<f.maxelm+f.maxbound) {
			for (integer ic=0; (ic<10)&&(i+2*ic<f.maxelm+f.maxbound); ic++) {
#if doubleintprecision == 1
				printf("%lld %e %lld %e\n", i + ic, f.SInvariantStrainRateTensor[i + ic], i + 2 * ic, f.SInvariantStrainRateTensor[i + 2 * ic]);
#else
				printf("%d %e %d %e\n", i + ic, f.SInvariantStrainRateTensor[i + ic], i + 2 * ic, f.SInvariantStrainRateTensor[i + 2 * ic]);
#endif
				
			}
			i=i+20;
			//getchar();
			system("pause");
		}
	}
	

	

	// ������� ���������� ���������� � ��������� tecplot360:
	if (0&&inumiter==60) {
		 exporttecplotxy360T_3D_part2(t.maxelm,t.ncell, fglobal, t, flow_interior, iflow,false,0);
	     printf("update flow properties. OK.\n");
	    // getchar(); // debug avtosave
		 system("pause");
	}

	
	doublereal bonbeta=dbeta;
	if (!bVERYStable) {
		// ����������� �������������� ������ ������� ������������� �� �������.
		 bonbeta=1.0; // ������ �������
		//bonbeta=1.33333; // ������ �������� �������������.
		// bonbeta=1.2; // ������ �������
	}

	// ����������� ������������ ������ �� �����������-������������ ������������ � 
	// ����������� �� �������� ������������� ����. ������� �������� ����������� ��������� ������������ ��������
	// ��������������� ���� ����������� ������������ ������������.
	// ������ ��� ���������� ����������� �������.
	doublereal** sumanb=new doublereal*[3];
	for (integer i=0; i<3; i++) sumanb[i]=new doublereal[f.maxelm+f.maxbound];
	// �������������.
	for (integer i=0; i<f.maxelm+f.maxbound; i++) {
		sumanb[VX][i]=0.0;
		sumanb[VY][i]=0.0;
		sumanb[VZ][i]=0.0;
	}


	// ������ ����� ��� ���� ���������� ������������, ��	
	// ���� � ��� ��� �������� ����� ���������� ������������ ���� ��������.
	// ���������� �������� ������������ ��� ����������� ����������� �������
	// ��� ���������� ���������� F �� ������ �������� ��������� � ����� VELCOR.
	// ���� ������� ������������ ���������� �� ������� ��������, ������� �������� ��
	// ����� ��-�������� �������� ������������ ��������������� ��������.
	if (bprintmessage) {
		printf("VX \n");
	    fprintf(fp_log,"VX \n");
	}

	// ���� ��� ��������� �������� SIMPLE ��������� � ��� ����� ���������� ������
	// � BICGSTAB_internal3 �� �� �������� ������ ���� ������������ ������ � ��������� ��� ����� PAM.
	bool bflag_free_memory_cfd=m.bsignalfreeCRScfd;
	if (m.bsignalfreeCRScfd) {
        m.bsignalfreeCRScfd=false;
	}
#if doubleintprecision == 1
	//printf("inumiter=%lld\n",inumiter);
#else
	//printf("inumiter=%d\n",inumiter);
#endif
	
	//getchar();

	//printf("turb VX\n");
	//getchar();
	if (inumiter > 1) {
		bdontstartsolver = false;
	}
	else {
		bdontstartsolver = true;
	}

	bool bHORF_speed_on = true; // ������ ���� ������ true.
	// ���� false �� ����������� ���������� �� ������� � ����������� opening ���������.
	// �� ������� ���� ���������� ������ ����������� ��� ���������� ���� �����.

		solve(VX, res, f, fglobal, t, rhie_chow,
			s, w, b, ls, lw, lb, bonbeta,
			flow_interior, false,
			bfirst_start, toldtimestep, NULL,
			speedoldtimestep, mfoldtimestep,
			dtimestep, btimedep, dgx, dgy, dgz,
			matlist, inumiter, bprintmessage,
			RCh, bVERYStable, NULL, sumanb, false, false, 1.0, m,
			rthdsd, rfluentres.res_vx, lu, my_union);

		// ������ ����� �����. 04.05.2017
		rfluentres.res_vx = fluent_residual_for_x(f.slau[VX], f.slau_bon[VX], f.potent[VX], f.maxelm, f.maxbound, VX); // ������� �� ������� fluent.

		for (integer i = 0; i < f.maxbound; i++) {
			sumanb[VX][f.maxelm + i] = f.slau_bon[VX][i].aw;
		}

		if (bHORF_speed_on&&bHORF) {
			for (integer i = 0; i < f.maxelm + f.maxbound; i++) {
				doublereal fHORF = 0.25;
				if (btimedep) {
					fHORF = 0.75;
				}
				f.potent[VX][i] = f.potent[VXCOR][i] + fHORF*(f.potent[VX][i] - f.potent[VXCOR][i]);
			}
		}
	
	//rfluentres.res_vx=fluent_residual_for_x(f.slau[VX], f.slau_bon[VX], f.potent[VX], f.maxelm, f.maxbound); // ������� �� ������� fluent.
	//rfluentres.res_vx = fluent_residual_for_x_new(f.slau[VX], f.slau_bon[VX], f.potent[VX], f.maxelm, f.maxbound, rthdsd,f.alpha[VX]); // ������� �� ������� fluent.
	bdontstartsolver = false;
	//interpolatevel(f, lw, ls,  w, VX); // �������� ����������
	//getchar();
	
	// 28.07.2016
	//exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, inumiter, bextendedprint);
	//getchar(); // debug
	
	
	if (bprintmessage) {
		printf("VY \n");
		fprintf(fp_log,"VY \n");
	}

	if (0) {
		xyplot( fglobal, flow_interior, t);
		printf("presolve. OK.\n");
	    //getchar(); // debug avtosave
		system("pause");
	}

	
	if (inumiter > 1) {
		bdontstartsolver = false;
	}
	else {
		bdontstartsolver = true;
	}

	

		//printf("VX VY\n");
		//getchar();
		solve(VY, res, f, fglobal, t, rhie_chow,
			s, w, b, ls, lw, lb, bonbeta,
			flow_interior, false,
			bfirst_start, toldtimestep, NULL,
			speedoldtimestep, mfoldtimestep,
			dtimestep, btimedep, dgx, dgy, dgz,
			matlist, inumiter, bprintmessage,
			RCh, bVERYStable, NULL, sumanb, false, false, 1.0, m,
			rthdsd, rfluentres.res_vy, lu, my_union);

		// ������ ����� �����. 04.05.2017
		rfluentres.res_vy = fluent_residual_for_x(f.slau[VY], f.slau_bon[VY], f.potent[VY], f.maxelm, f.maxbound, VY); // ������� �� ������� fluent.

		for (integer i = 0; i < f.maxbound; i++) {
			sumanb[VY][f.maxelm + i] = f.slau_bon[VY][i].aw;
		}

		if (bHORF_speed_on&&bHORF) {
			for (integer i = 0; i < f.maxelm + f.maxbound; i++) {
				doublereal fHORF = 0.25;
				if (btimedep) {
					fHORF = 0.75;
				}
				f.potent[VY][i] = f.potent[VYCOR][i] + fHORF*(f.potent[VY][i] - f.potent[VYCOR][i]);
			}
		}
	
	//rfluentres.res_vy=fluent_residual_for_x(f.slau[VY], f.slau_bon[VY], f.potent[VY], f.maxelm, f.maxbound); // ������� �� ������� fluent.
	//rfluentres.res_vy = fluent_residual_for_x_new(f.slau[VY], f.slau_bon[VY], f.potent[VY], f.maxelm, f.maxbound, rthdsd, f.alpha[VY]); // ������� �� ������� fluent.
	
	bdontstartsolver = false;

	// 28.07.2016
	//exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, inumiter, bextendedprint);
	//getchar(); // debug

	//interpolatevel(f, lw, ls,  w, VY); // �������� ����������
	//getchar();
	if (bprintmessage) {
		printf("VZ \n");
		fprintf(fp_log,"VZ \n");
	}
	//printf("VY VZ\n");
	//getchar();
	if (inumiter > 1) {
		bdontstartsolver = false;
	}
	else {
		bdontstartsolver = true;
	}

	

		solve(VZ, res, f, fglobal, t, rhie_chow,
			s, w, b, ls, lw, lb, bonbeta,
			flow_interior, false,
			bfirst_start, toldtimestep, NULL,
			speedoldtimestep, mfoldtimestep,
			dtimestep, btimedep, dgx, dgy, dgz,
			matlist, inumiter, bprintmessage,
			RCh, bVERYStable, NULL, sumanb, false, false, 1.0, m,
			rthdsd, rfluentres.res_vz, lu, my_union);

		// ������ ����� �����. 04.05.2017
		rfluentres.res_vz = fluent_residual_for_x(f.slau[VZ], f.slau_bon[VZ], f.potent[VZ], f.maxelm, f.maxbound, VZ); // ������� �� ������� fluent.


		for (integer i = 0; i < f.maxbound; i++) {
			sumanb[VZ][f.maxelm + i] = f.slau_bon[VZ][i].aw;
		}

		

		if (bHORF_speed_on&&bHORF) {
			for (integer i = 0; i < f.maxelm + f.maxbound; i++) {
				doublereal fHORF = 0.25;
				if (btimedep) {
					fHORF = 0.75;
				}
				f.potent[VZ][i] = f.potent[VZCOR][i] + fHORF*(f.potent[VZ][i] - f.potent[VZCOR][i]);
			}
		}
	
	//rfluentres.res_vz=fluent_residual_for_x(f.slau[VZ], f.slau_bon[VZ], f.potent[VZ], f.maxelm, f.maxbound); // ������� �� ������� fluent.
	//rfluentres.res_vz = fluent_residual_for_x_new(f.slau[VZ], f.slau_bon[VZ], f.potent[VZ], f.maxelm, f.maxbound, rthdsd, f.alpha[VZ]);
	
	bdontstartsolver = false;

	// 28.07.2016
	//exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, inumiter, bextendedprint);
	//getchar(); // debug

	//interpolatevel(f, lw, ls,  w, VZ); // �������� ����������
	// printf("one iteration\n"); // debug
	//getchar();

	// debug:
	/*
	for (i=0; i<f.maxelm; i++) {
		if (fabs(f.potent[VY][i]+0.02089433689)<1e-6) {
		#if doubleintprecision == 1
			printf("iP==%lld, Vely=%e\n",i,f.potent[VY][i]);
		#else
			printf("iP==%d, Vely=%e\n",i,f.potent[VY][i]);
		#endif
			
			getchar();
		}
	}
	*/
	//printf("VZ end\n");
	//getchar();

	if (0) {
		xyplot( fglobal, flow_interior, t);
		printf("solve velocity. OK.\n");
	    //getchar(); // debug avtosave
		system("pause");
	}


	

	// ���������� ������������ ������������ � ������� ��� ������ ���������� �������� :
	// ������������ ������������ ����������� ��� ���������� �������� ���-���.
	// � ������ ��������� � ����������� �� ��� �� ���.23 � ������ ��� ���������������
	// �������� �������� ���������, ��� �������� ����������������� �� ����� ������ �� 
	// ������� ���-��� ����� ���� �� ��������������, � ��������� ������ �� ������ �����������������
	// ����� �������� � �������� � ������� ��.
	// 10 ������� 2012.
	// ������ ����� ������� sumanb ���� ��-�������� ������������ ����� ��, �
	// f.diag_coef ���� ������� �� �������������.
	// � sumanb ���������� ������������ �������� � �� ������� ������� ��� ���� �������� � ���� ���� 
	// ������ � sumanb ��������� ��������.
	/* // ��������������� 28 ������� 2014.
	for (i=0; i<(f.maxelm+f.maxbound); i++) {
		if (i<f.maxelm) {
			f.diag_coef[VX][i]=f.slau[VX][i].ap; // VX
			f.diag_coef[VY][i]=f.slau[VY][i].ap; // VY
			f.diag_coef[VZ][i]=f.slau[VZ][i].ap; // VZ
		}
		else {
			f.diag_coef[VX][i]=f.slau_bon[VX][i-f.maxelm].aw; // VX
			f.diag_coef[VY][i]=f.slau_bon[VY][i-f.maxelm].aw; // VY 
			f.diag_coef[VZ][i]=f.slau_bon[VZ][i-f.maxelm].aw; // VZ
		}
	}
	*/
    
	// ������� ���������� ���������� � ��������� tecplot360:
	if (0) {
		if (inumiter>82) {
		   exporttecplotxy360T_3D_part2(t.maxelm,t.ncell, fglobal, t, flow_interior,iflow,false,0);
	       printf("solve momentum. OK.\n");
	       //getchar(); // debug avtosave
		   system("pause");
		}
	}
	


	// ������ ��������� ��� �������� ��������:
	// ����� �������� ������������� �������� �������� ����.
    if (bprintmessage) {
		printf("PAM\n"); // dbeta
		fprintf(fp_log,"PAM\n");
	}

	

	// �������� ������ �� �������� ������� ��������������� ��������� �������������.
	//mass_balance(f, lw, ls, w); // ��� ����� �����. ��. �������� ������.

	//doublereal* tau=new doublereal[f.maxelm+f.maxbound];

	doublereal** tau=new doublereal*[3];
	for (integer i=0; i<3; i++) tau[i]=new doublereal[f.maxelm+f.maxbound];

	bool bVERYstabtau=false;
	doublereal dusertimestep=dtimestep;
	// ������������� ��� �� ������� ������ ���������� �������� ����������,
	// ��� ����� ������������ ���������� ��� �� ������� ������ ������ �������� ���������������
	// ����� ���� �������������� ����������� ��������������� �����.
	if (!btimedep) {
	   dusertimestep=0.0;
	}
	// �� ������ 20 ��������� ����� ����� ������ ������������ ���������� ��� �� �������������, �.�. ��� ����� ������ ��������������� ������������.
	/*
	if (inumiter<20) {
		bVERYstabtau=true;	
	}
	*/
	bool boldscheme=true;
	if (0&&(inumiter==1)) {
		boldscheme=true;
	}
	// ���������� ����������� ���� �� �������������.
	/*
	tau_calc(tau[0], f.maxelm, f.maxbound,
		     f.prop, f.prop_b, f.alpha, 
			 f.nvtx, f.pa, f.slau,
			 f.sosedi, f.slau_bon,
			 btimedep, dusertimestep, 1.0,
			  inumiter,bVERYstabtau,boldscheme);*/


	doublereal CFL1=1.0; // 1.0
	// �� �������� ����������� ��� �� ���� ��������� ���� � �����
	// ��� ��������� ���� � 3D ������, �.�. ��� ������ ���������� ��������
	// ��� ��������� ����������� (��� �� ��� ������ ��� � � ��������� ������������ 3 ��� Ox, Oy, Oz).
	tau_calc3(tau, f.maxelm, f.maxbound,
		     f.prop, f.prop_b, f.alpha, 
			 f.nvtx, f.pa, 
			 f.sosedi, sumanb,
			 btimedep, dusertimestep, CFL1,
			 inumiter,bVERYstabtau,boldscheme);

	/*
	if (inumiter==2) {
	for (i=0; i<f.maxelm; i++) {
	#if doubleintprecision == 1
		printf("%lld %e, %e %e\n",i, tau[VX][i],tau[VY][i],tau[VZ][i]);
	#else
		printf("%d %e, %e %e\n",i, tau[VX][i],tau[VY][i],tau[VZ][i]);
	#endif
		
		if (i%10==0) getchar();
	}
	}
	*/
	//������������� ��������������� �������� �� ������������� (������� �������������� ����� ���� ��������)
	// �������� � ���������� ������������ ��� �� ������ ��������,
	// ��� ��� ���� �������� ���������� ������ ����������.
	// ���� �� ������������ ����������� ����� ���� ��������� �������� tau
	// (��� �������� ������������� ��������� � ����� ������������ �������� ����� ��������)
	// �� ���������� ���������� ����� �����.
	
	/*
	// ������� ������� ��������� ����.
	for (integer i=0; i<f.maxelm; i++) {
		// ���������� �������� �������� ������������ ������:
	    doublereal dx=0.0, dy=0.0, dz=0.0;// ����� �������� ������������ ������
	    volume3D(i, f.nvtx, f.pa, dx, dy, dz);
		doublereal CFL=f.alpha[VX]/(1.0-alpha[VX]); // ������� ������� ��������� ����.
		tau[VX][i]=fmin(fabs(CFL/(f.potent[VX][i])/dx),tau[VX][i]);
		if (f.sosedi[ESIDE][i].iNODE1>=f.maxelm) tau[VX][f.sosedi[ESIDE][i].iNODE1]=tau[VX][i];
		if (f.sosedi[WSIDE][i].iNODE1>=f.maxelm) tau[VX][f.sosedi[WSIDE][i].iNODE1]=tau[VX][i];
		tau[VY][i]=fmin(fabs(CFL/(f.potent[VY][i])/dy),tau[VY][i]);
		if (f.sosedi[NSIDE][i].iNODE1>=f.maxelm) tau[VY][f.sosedi[NSIDE][i].iNODE1]=tau[VY][i];
		if (f.sosedi[SSIDE][i].iNODE1>=f.maxelm) tau[VY][f.sosedi[SSIDE][i].iNODE1]=tau[VY][i];
		tau[VZ][i]=fmin(fabs(CFL/(f.potent[VZ][i])/dz),tau[VZ][i]);
		if (f.sosedi[TSIDE][i].iNODE1>=f.maxelm) tau[VZ][f.sosedi[TSIDE][i].iNODE1]=tau[VZ][i];
		if (f.sosedi[BSIDE][i].iNODE1>=f.maxelm) tau[VZ][f.sosedi[BSIDE][i].iNODE1]=tau[VZ][i];
	}

	// ������� �� �������� ����� ����������.
	for (integer i=0; i<f.maxelm; i++) {
		// ���������� �������� �������� ������������ ������:
	    doublereal dx=0.0, dy=0.0, dz=0.0;// ����� �������� ������������ ������
	    volume3D(i, f.nvtx, f.pa, dx, dy, dz);
		doublereal Rec=1.5; // �������� ����� ����������
		tau[VX][i]=fmin((dx*dx)/(f.prop[RHO][i]*f.prop[MU][i]*Rec),tau[VX][i]);
		if (f.sosedi[ESIDE][i].iNODE1>=f.maxelm) tau[VX][f.sosedi[ESIDE][i].iNODE1]=tau[VX][i];
		if (f.sosedi[WSIDE][i].iNODE1>=f.maxelm) tau[VX][f.sosedi[WSIDE][i].iNODE1]=tau[VX][i];
		tau[VY][i]=fmin((dy*dy)/(f.prop[RHO][i]*f.prop[MU][i]*Rec),tau[VY][i]);
		if (f.sosedi[NSIDE][i].iNODE1>=f.maxelm) tau[VY][f.sosedi[NSIDE][i].iNODE1]=tau[VY][i];
		if (f.sosedi[SSIDE][i].iNODE1>=f.maxelm) tau[VY][f.sosedi[SSIDE][i].iNODE1]=tau[VY][i];
		tau[VZ][i]=fmin((dz*dz)/(f.prop[RHO][i]*f.prop[MU][i]*Rec),tau[VZ][i]);
		if (f.sosedi[TSIDE][i].iNODE1>=f.maxelm) tau[VZ][f.sosedi[TSIDE][i].iNODE1]=tau[VZ][i];
		if (f.sosedi[BSIDE][i].iNODE1>=f.maxelm) tau[VZ][f.sosedi[BSIDE][i].iNODE1]=tau[VZ][i];
	}

	// ������� �� �������� ����� ����������.
	for (integer i=0; i<f.maxelm; i++) {
		// ���������� �������� �������� ������������ ������:
	    doublereal dx=0.0, dy=0.0, dz=0.0;// ����� �������� ������������ ������
	    volume3D(i, f.nvtx, f.pa, dx, dy, dz);
		doublereal Rec=1.5; // �������� ����� ����������
		tau[VX][i]=fmin((f.prop[RHO][i]*f.prop[MU][i]*Rec)/(fmax(1e-15,f.potent[VX][i]*f.potent[VX][i])),tau[VX][i]);
		if (f.sosedi[ESIDE][i].iNODE1>=f.maxelm) tau[VX][f.sosedi[ESIDE][i].iNODE1]=tau[VX][i];
		if (f.sosedi[WSIDE][i].iNODE1>=f.maxelm) tau[VX][f.sosedi[WSIDE][i].iNODE1]=tau[VX][i];
		tau[VY][i]=fmin((f.prop[RHO][i]*f.prop[MU][i]*Rec)/(fmax(1e-15,f.potent[VY][i]*f.potent[VY][i])),tau[VY][i]);
		if (f.sosedi[NSIDE][i].iNODE1>=f.maxelm) tau[VY][f.sosedi[NSIDE][i].iNODE1]=tau[VY][i];
		if (f.sosedi[SSIDE][i].iNODE1>=f.maxelm) tau[VY][f.sosedi[SSIDE][i].iNODE1]=tau[VY][i];
		tau[VZ][i]=fmin((f.prop[RHO][i]*f.prop[MU][i]*Rec)/(fmax(1e-15,f.potent[VZ][i]*f.potent[VZ][i])),tau[VZ][i]);
		if (f.sosedi[TSIDE][i].iNODE1>=f.maxelm) tau[VZ][f.sosedi[TSIDE][i].iNODE1]=tau[VZ][i];
		if (f.sosedi[BSIDE][i].iNODE1>=f.maxelm) tau[VZ][f.sosedi[BSIDE][i].iNODE1]=tau[VZ][i];
	}
	*/
	/*
	doublereal tauX=1.0e+30, tauY=1.0e+30, tauZ=1.0e+30;
	for (integer i=0; i<f.maxelm+f.maxbound; i++) {
		tauX=fmin(tauX,tau[VX][i]);
		tauY=fmin(tauY,tau[VY][i]);
		tauZ=fmin(tauZ,tau[VZ][i]);
	}
	//doublereal rn=1.0*(f.maxelm+f.maxbound);
	//tauX/=rn;
	//tauY/=rn;
	//tauZ/=rn;
	for (integer i=0; i<f.maxelm+f.maxbound; i++) {
		tau[VX][i]=0.1*tauX;
		tau[VY][i]=0.1*tauY;
		tau[VZ][i]=0.1*tauZ;
	}
	*/

	// ��������� �������� � ��������� ������ �� �������� ������
	// ����� ��������� ��������� ��. �������� ������.
	//mass_balance(f, lw, ls, w);

	
	//printf("PAM\n");
	//getchar();
	doublereal** rsumanbstuff=NULL; // NULL ������� ��� ����� ������������ �������������
	bool bhighorder_pressure=false;
	bool bdeltafinish=true; // ���� true �� ���������� �������� ��������� ������ �� �����. ���� false �� ������������. 
	// false ����� ���� ������ � ��� ������ ���� ��������� ��� �������� �������� ����� �������� �������� � ����� �������� ��������
	// �������������.
	if (bflag_free_memory_cfd) {
		m.bsignalfreeCRScfd=true; // ����������� ������.
	}

	doublereal rfluentResPAM = 0.0;
    solve(PAM,continity,f,fglobal,t,rhie_chow,
		  s, w, b, ls, lw, lb, 1.0, 
		  flow_interior, false, bfirst_start,
		  toldtimestep, NULL, speedoldtimestep,
		  mfoldtimestep,dtimestep,btimedep,
		  dgx,dgy,dgz,matlist,inumiter,
		  bprintmessage, RCh,false,
		  tau,rsumanbstuff,bhighorder_pressure,
		  bdeltafinish, 1.0, m, rthdsd, rfluentResPAM, lu, my_union);

	
	// 9 ������� 2016 ����. (������ ���������� ��� �������� ��������).
	// ������ ������ ����������.
	if (0&&bHORF) {
		for (integer i = 0; i < f.maxelm + f.maxbound; i++) {
			doublereal fHORF = 0.25;
			if (btimedep) {
				fHORF = 0.75;
			}
			f.potent[PAM][i] = f.potent[PAMOLDITER][i] + fHORF*(f.potent[PAM][i] - f.potent[PAMOLDITER][i]);
			f.potent[PAMOLDITER][i] = f.potent[PAM][i];
		}
	}

	/*
	bhighorder=false;
	bdeltafinish=true;
	// ����� �������� ����� ���� � ��������� �������� ���� �� ����� ���������� ����������� �������� �������������.
	// dbeta 1.2 ������������ ������ ������� �� ������� �������.
	solve(PAM,continity,f,fglobal,t,rhie_chow,
		  s, w, b, ls, lw, lb, 1.0, 
		  flow_interior, false, bfirst_start,
		  toldtimestep,speedoldtimestep,
		  mfoldtimestep,dtimestep,btimedep,
		  dgx,dgy,dgz,matlist,inumiter,
		  bprintmessage, RCh,false, false, tau, rsumanbstuff,bhighorder,bdeltafinish);*/
		  

	// 28.07.2016
	//exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, inumiter, bextendedprint);
	//getchar(); // debug

	// ������� ���������� ���������� � ��������� tecplot360:
	if (0) {
		 exporttecplotxy360T_3D_part2(t.maxelm,t.ncell, fglobal, t, flow_interior,iflow,bextendedprint,0);
	    // printf("temperature calculate begin now... OK.\n");
	     //getchar(); // debug avtosave
		 system("pause");
	}

	// 13 08 2015 �������� � ���������� ������� ��������� ��.
	// my_unsteady_temperature.c ����.
	//doublereal* xb=new doublereal[f.maxelm+f.maxbound];
	for (integer i=0; i<f.maxelm; i++) {
		xb[i]=f.slau[PAM][i].b;
	}
	for (integer i=0; i<f.maxbound; i++) {
		xb[i+f.maxelm]=f.slau_bon[PAM][i].b;
	}
	rfluentres.res_no_balance=no_balance_mass_flux_fluent(xb, rfluentres.operating_value_b, f.maxelm+f.maxbound);
	//delete xb; // �� �������� ����������� ������.
	
	/*
	// ������������ �� ��������� ����������.
	// ������������ ��������� ��������� ������� :
	// ����� ������ ����� ��������� � ������ ������ ����� ��������� ��� ����� 
	// ������. ������ �� ��� ����� ����������������. �� ����� ���� ���������������� ����� 
	// ����������� ������ (������) ������ �����������. ���� ������ ���������� ������������ � �����
	// ���� ���������� ����� ������� ����. ������������ �� ����� ������ ������ ��� ���� ������ ��� :
	// ������ �� ���������������� �������� ���������� ��� ����� �����������, � ����� ����� ���� 
	// ���������������� ��������  ��� ������� ������ ��� � ���������� ��� �������.
	// ������ ����� ��������� ������������ � ��������� �������� 
	// �������� ��� ����� ��� ����� ������� ��������.
	doublereal* potentPAMfiltr=new doublereal[f.maxelm+f.maxbound];
		
	doublereal *nullpointer=NULL;
	// ��������� ������������ ���� ���������.
	integer iciclenumerical=300;  // ��� ������ ������ ��� ������ ������������.
	for (integer icicle=0; icicle<iciclenumerical; icicle++) {
		double_average_potent(f.potent[PAM], potentPAMfiltr,
	                         f.maxelm, f.maxbound, f.sosedi, 
							 f.nvtx, f.pa, nullpointer,
							 SIMPSON_FILTR, f.sosedb,0); // VOLUME_AVERAGE_FILTR
		// �����������.
	    for (integer i=0; i<f.maxelm+f.maxbound; i++) {
		    f.potent[PAM][i]=potentPAMfiltr[i];
	    }
	}

	// ������������ ����������� ������.
	delete potentPAMfiltr;
	*/

	//printf("Ok\n");
	//getchar();

	//if (0&&(f.bPressureFix)) {
	if (0&&(f.bLR1free)) {
	//if (1) {
		// ������� ������������� ������� � ��� �����
		// ������� �������� �������� �������� ���� ����� ����.

		//printf("regularization condition for Pamendment...\n");
		//printf("please, press any key to continue...\n");
		//getchar(); // ���������� ��������.

		// ������ �.�. ����������, �.�.�������, �.�.���������
		// ����� ������������ ������� ������������� ��� 
		// ����������� ���� �������� �������� � ��� ������
		// ���� �� ���� ������� ��������� ������� ����� 
		// ���������� ������� �������.
	    // ������� �������������:
	    doublereal pamscal1=0.0;
	    // pamscal1=Scal(f.potent[PAM],1.0); ������ ��������� ����
		/*
		// ���� ����� ���� ��� ������������ ������.
		// ��� ������� ���������������� ��� ������ �� ������� ������.
	    for (i=0; i<(f.maxelm+f.maxbound); i++) pamscal1+=f.potent[PAM][i];
		pamscal1=(doublereal)(pamscal1/(f.maxelm+f.maxbound));
		for (i=0; i<(f.maxelm+f.maxbound); i++) f.potent[PAM][i]-=pamscal1;
		*/

		doublereal Vol=0.0;
		for (integer iP=0; iP<(f.maxelm); iP++) {
		    // ���������� �������� �������� ������������ ������:
	        doublereal dx=0.0, dy=0.0, dz=0.0;// ����� �������� ������������� ������
	        volume3D(iP, f.nvtx, f.pa, dx, dy, dz);
			Vol+=dx*dy*dz;

			pamscal1+=f.potent[PAM][iP]*dx*dy*dz;
		}
		pamscal1=(doublereal)(pamscal1/(f.maxelm*Vol));
		for (integer iP=0; iP<(f.maxelm+f.maxbound); iP++) {
			f.potent[PAM][iP]-=pamscal1;
		}

		// �������� ����� ����� ����� �������� �� ������ �� �������� ��������,
		// � ����� ��������� �� ����� ��������������� FLUID ����.
		// �������� ��������� ������ ���� ��������� TODO.

		// ������������ �������� �������� �� ������������ ��������� 
	    // ������� �� �������.
	    //free_pressure(f);
	}
	//else free_pressure(f);

	// ������������ �������� �������� �� ������������ ��������� 
	// ������� �� �������.
	// ��� �������������� ���������� �������� �������������.
	// ���������� ����� � ���, ��� ���������� ��������� ������� ������� ��� �������� ��������
	// ���� ��������� �������� �������� � ��������� � ��������� ���������� ���� � �� ����� ���
	// �������� �������� �� ���� ������� ������� ������������ ���� �������� ������� � ������ ������� �����.
	// �.�. ������� �� ������ ������ ���� �������� � ��������� ���� � ��������� ���������� �� �����, ��� ������ ����
	// ������ ����� �������������� �� ������������� ��������� �������� �� ������������ ��������� �������.
	// ����� ������� ������� ����������� � ���� �������� ������ ��������� �������, �������� ������������ ��������� ��������,
	// �.�. ����� �������� ������ ��������� ������ ����������� ���� � ��� ���� ����������� ����� �� ������� ��� ���������� �� ����.
	// (�������� ���� ��������������� ������� ���������� ���������).
	// ��� ���� ������ ������� ��������� ������ ����������� ���� �� ������� ��� ���������� �������� �� ������������� ������� ��������� ������� �� �������.
	// ���� ������� ����� ���������������.
	// ��� ����������� freepressure ��� ������ ������ �������� � ������� �� ����������� freepressure ��������� ��������� ��������� ������� continity ���������� �� 17%.
	// �� �������� ������� ���������� ������� freepressure �� ������ � ��� ����� ����� ���������. ������� ���������� ��������� 
	// ������ �������� ������� ��� ����������� freepressure �� �����������.
	//free_pressure(f); // ������ ������ ��� �������������� � ��� ����� ������� ���������� (��������� �� ��).


	//printf("PAM end\n");
	//getchar();

	
	
// ���������� ���������� �������� ��������:
	#ifdef _OPENMP

	// � ������������ ����� ���� �������� SIMPLE ��������� ����������� �� 2.2% �������.

			if (inumcore==2) {
				if (nd.b0.active) {

#pragma omp parallel 
					{
#pragma omp sections 
						{
#pragma omp section
							{
					// ������ �����
					for (integer iscan_par=nd.b0.ileft_start; iscan_par<=nd.b0.ileft_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							// ��������� �������� �������� ��� ���������� ��.
		                    green_gaussPAM(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, false, f.sosedb,ls,lw,w,f.bLR1free);
						}

					}
							}
							#pragma omp section
							{
					// ������ �����
					for (integer iscan_par=nd.b0.iright_start; iscan_par<=nd.b0.iright_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							// ��������� �������� �������� ��� ���������� ��.
		                    green_gaussPAM(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, false, f.sosedb,ls,lw,w,f.bLR1free);
						}
					}
							} // section
						} // sections
					} // parallel

					// �������� ��������� �����
					for (integer iscan_par=nd.b0.iseparate_start; iscan_par<=nd.b0.iseparate_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							// ��������� �������� �������� ��� ���������� ��.
		                    green_gaussPAM(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, false, f.sosedb,ls,lw,w,f.bLR1free);
						}
					}


				}
			}

			if (inumcore==2) {
				if (nd.b0.active) {

					#pragma omp parallel 
					{
#pragma omp sections 
						{
#pragma omp section
							{

					// ������ �����
					for (integer iscan_par=nd.b0.ileft_start; iscan_par<=nd.b0.ileft_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							// ��������� �������� �������� ��� ���������� ��.
		                    green_gaussPAM(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, true, f.sosedb,ls,lw,w,f.bLR1free);
						}

					}
							}
#pragma omp section
							{

					// ������ �����
					for (integer iscan_par=nd.b0.iright_start; iscan_par<=nd.b0.iright_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							// ��������� �������� �������� ��� ���������� ��.
		                    green_gaussPAM(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, true, f.sosedb,ls,lw,w,f.bLR1free);
						}
					}
							} // section
						} // sections
					} // parallel

					// �������� ��������� �����
					for (integer iscan_par=nd.b0.iseparate_start; iscan_par<=nd.b0.iseparate_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							// ��������� �������� �������� ��� ���������� ��.
		                    green_gaussPAM(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, true, f.sosedb,ls,lw,w,f.bLR1free);
						}
					}


				}
			}

#else

	if (0) {
		// bLRfree==false
		if (f.bLR1free) {
			printf("bLRfree_on\n");
			system("pause");
		}
		else {
			printf("bLRfree_off\n");
			system("pause");
		}
	}

	// ���������� ���������� �������� ��������:
	for (integer i=0; i<f.maxelm; i++) {
		// ��������� �������� �������� ��� ���������� ��.
		green_gaussPAM(i, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, false, f.sosedb,ls,lw,w,f.bLR1free);
	}
	for (integer i=0; i<f.maxelm; i++) {
		// ��������� ��������� ��� ��������� ��.
	    green_gaussPAM(i, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, true, f.sosedb,ls,lw,w,f.bLR1free);
    }

#endif


	// 10 ������� 2012 ���� ���������� ������.
	/*
	for (i=0; i<f.maxelm+f.maxbound; i++) {
	    f.potent[FBUF][i]=f.potent[GRADYPAM][i];
	}
	*/
	if (0) {
		xyplot( fglobal, flow_interior, t);
		printf("xy plot pam solve. OK.\n");
	   // getchar(); // debug avtosave
		system("pause");

		//exporttecplotxy360T_3D_part2(t.maxelm,t.ncell, fglobal, t, flow_interior,iflow);
	    //printf("solve vel ok fbuf gradypam... OK.\n");
	    //getchar(); // debug avtosave
	}

	

	// ������������ ����������� ��������� �������� ��������.
    //green_gaussPAMminmax(f.potent, f.sosedi, f.maxelm, f.maxbound);

	/*
	// ������������ ����� ��������������� � ������ �������� �� ������������� ����� �
	// �� ������ �������� �� �����������. ���������� ���������� ��� ������������ ����������� 
	// ������������� �����. ���������� �������� �� ������� ��������� ��� �������� ��������.
	// ��������� ��� ��������� �������� ��� ����� ������������ ������ � ������������ ������������
	// ����������� ��������. ���� �� ����� ������������� ������� ������� �� ������������� �����.
	// ��� ������ ��� �������� ����� ��������� ���� �������� �������� ����� ������� ���� �����
	// ���������� �����, �� ��� ���� ������ ����������� �� �������� �������� ����� ���� � ����������� �����
	// ��������� ��� ���������� ������������ ���������. ����� ��������������, ��� ������� ����� ���������������
	// ��������� �����. ���� ����������� �� �������� ������� ���� �������� ������������ ����. ��������� �� ������ 
	// ���� ������������ ���� �������� �������� ������ �������� - �������� ����������������� �������� � �.�. �� �������.
	// �� ��� ���������� ����������� ������ ����������� � �� ����� ������������. �������������� ��� ������� ��������� ������ � 
	// ������������� : 1. ����������� ���������� � ������� ���������� ������� (�����������). ��� ������� ������� �������.
	// 2. ���������� �������� ������������� ���������� ������� �� ������� ��� ����� ������ �� ����������� ������� ������ �����������
	// ���� ������� �������� (�����������).
	// ���������� � ������������ ����������� ��������� ��������.
	// ��� ������������ ���������� ����������� ������������.
	

	for (i=0; i<f.maxelm+f.maxbound; i++) {
		f.potent[FBUF][i]=f.potent[GRADYPAM][i];
	}
	if (1) {
		xyplot( fglobal, flow_interior, t);
		printf("grad PAM calc. OK.\n");
	    getchar(); // debug avtosave
	}

	// ������ ����� ��������� ������������ :
	doublereal** potentgradPAMfiltr=new doublereal*[3];
	for (i=0; i<3; i++) {
		potentgradPAMfiltr[i]=new doublereal[f.maxelm+f.maxbound];
	}
	
	doublereal *nullpointer=NULL;
	// ��������� ������������ ���� ���������.
	integer iciclenumerical=3;  // ��� ������ ������ ��� ������ ������������.
	for (integer icicle=0; icicle<iciclenumerical; icicle++) {
		double_average_potent(potentgradPAMfiltr[0], f.potent[GRADXPAM], 
	                         f.maxelm, f.maxbound, f.sosedi, 
							 f.nvtx, f.pa, nullpointer,
							 VOLUME_AVERAGE_FILTR, f.sosedb,0);
		double_average_potent(potentgradPAMfiltr[1], f.potent[GRADYPAM], 
	                         f.maxelm, f.maxbound, f.sosedi, 
							 f.nvtx, f.pa, nullpointer,
							 VOLUME_AVERAGE_FILTR, f.sosedb,0);
		double_average_potent(potentgradPAMfiltr[2], f.potent[GRADZPAM], 
	                         f.maxelm, f.maxbound, f.sosedi, 
							 f.nvtx, f.pa, nullpointer,
							 VOLUME_AVERAGE_FILTR, f.sosedb,0);
		// �����������.
	    for (integer i=0; i<f.maxelm+f.maxbound; i++) {
		    f.potent[GRADXPAM][i]=potentgradPAMfiltr[0][i];
	    	f.potent[GRADYPAM][i]=potentgradPAMfiltr[1][i];
		    f.potent[GRADZPAM][i]=potentgradPAMfiltr[2][i];
	    }
	}

	// ������������ ����������� ������.
	for (i=0; i<3; i++) {
		delete potentgradPAMfiltr[i];
	}
	delete potentgradPAMfiltr;
	*/
	
	// �������������� �������� � �������� �������� ��� ���������� ������������� �������
	// ������� �������.
	// �� ���� ������ ��������� ��������� ���� ������� �������� ����������.
    //correctpressureoutlet(f, lw, ls, w);
	//free_pressure(f);
	
	// ������� ���������� ���������� � ��������� tecplot360:
	if (0) {
		if (1||inumiter>82) {
		   exporttecplotxy360T_3D_part2(t.maxelm,t.ncell, fglobal, t, flow_interior,iflow,false,0);
	       printf("solve pressure. OK.\n");
	       //getchar(); // debug avtosave
		   system("pause");
		}
	}
	if (0) {
		xyplot( fglobal, flow_interior, t);
		printf("solve pressure. OK.\n");
	    //getchar(); // debug avtosave
		system("pause");
	}

	// ��������� ��������:
	doublereal ralphaP=1.0; // �������������
	if (iSIMPLE_alg==SIMPLEC_Van_Doormal_and_Raithby) {
		// SIMPLEC 
		ralphaP=1.0;
		//ralphaP=1.0-(f.alpha[VX]+f.alpha[VY]+f.alpha[VZ])/3.0;
	}
	if (iSIMPLE_alg==SIMPLE_Carretto) {
		// SIMPLE
		ralphaP=f.alpha[PRESS];
	}
	// ������ ��� ���������� ��.
#pragma omp parallel for
	for (integer i=0; i<f.maxelm; i++)  {
		//f.potent[PRESS][i]+=ralphaP*f.potent[PAM][i];
		// 06.05.2017 �� ������� �� ������ � SIMPLE ���������.
		// ����������� : CFD - Solution Algorithms
		// SOE3213/4: CFD Lecture 3.
		// p_zv_zv - Presure �������� ��������.
		doublereal p_zv_zv = f.potent[PRESS][i] + f.potent[PAM][i];
		f.potent[PRESS][i] = ralphaP*p_zv_zv + (1.0 - ralphaP)*f.potent[PRESS][i];
	}

#pragma omp parallel for
	for (integer i=f.maxelm; i<(f.maxelm+f.maxbound); i++)  {
		// ��-����� ����� �� ����������� ��������, � ��������� ��� 
		// ���������������� � ������� ����� �������.
		// ��� ������� ������������ TODO.
		//if (f.slau_bon[PAM][i-f.maxelm].iI>-1) {

            // ������ ������ (���������������� ���� ���� �������).
			// ����� ���������� ������� �������
           // f.potent[PRESS][i]+=ralphaP*f.potent[PAM][i];

		   // 06.05.2017 �� ������� �� ������ � SIMPLE ���������.
		   // ����������� : CFD - Solution Algorithms
		   // SOE3213/4: CFD Lecture 3.
		   // p_zv_zv - Presure �������� ��������.
			doublereal p_zv_zv = f.potent[PRESS][i] + f.potent[PAM][i];
			f.potent[PRESS][i] = ralphaP*p_zv_zv + (1.0 - ralphaP)*f.potent[PRESS][i];

			//printf("corect boundary pressure...\n");  // debug
			//getchar();
		//}
/*
			integer inumber=i-f.maxelm;
			if ((f.sosedb[inumber].MCB>=ls) && (f.sosedb[inumber].MCB<(ls+lw)) && w[f.sosedb[inumber].MCB-ls].bpressure) {
				f.potent[PRESS][i]=w[f.sosedb[inumber].MCB-ls].P;
			}
			*/
	}

	
// ���������� ���������� ��������:
	// �� ������ ������������������ ���� ��������.
	// ��������� �������� ����������� ��� ���������� �������� ���-���.
	#ifdef _OPENMP

	// � ������������ ����� ���� �������� SIMPLE ��������� ����������� �� 0.9% �������.

			if (inumcore==2) {
				if (nd.b0.active) {

#pragma omp parallel 
					{
#pragma omp sections 
						{
#pragma omp section
							{
					// ������ �����
					for (integer iscan_par=nd.b0.ileft_start; iscan_par<=nd.b0.ileft_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							// ��������� �������� �������� ��� ���������� ��.
		                    green_gaussPRESS(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, false, f.sosedb,ls,lw,w,f.bLR1free);
						}

					}
							}
							#pragma omp section
							{
					// ������ �����
					for (integer iscan_par=nd.b0.iright_start; iscan_par<=nd.b0.iright_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							// ��������� �������� �������� ��� ���������� ��.
		                    green_gaussPRESS(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, false, f.sosedb,ls,lw,w,f.bLR1free);
						}
					}
							} // section
						} // sections
					} // parallel

					// �������� ��������� �����
					for (integer iscan_par=nd.b0.iseparate_start; iscan_par<=nd.b0.iseparate_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							// ��������� �������� �������� ��� ���������� ��.
		                    green_gaussPRESS(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, false, f.sosedb,ls,lw,w,f.bLR1free);
						}
					}


				}
			}

			if (inumcore==2) {
				if (nd.b0.active) {

#pragma omp parallel 
					{
#pragma omp sections 
						{
#pragma omp section
							{

					// ������ �����
					for (integer iscan_par=nd.b0.ileft_start; iscan_par<=nd.b0.ileft_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							// ��������� �������� �������� ��� ���������� ��.
		                    green_gaussPRESS(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, true, f.sosedb,ls,lw,w,f.bLR1free);
						}

					}
							}
#pragma omp section
							{

					// ������ �����
					for (integer iscan_par=nd.b0.iright_start; iscan_par<=nd.b0.iright_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							// ��������� �������� �������� ��� ���������� ��.
		                    green_gaussPRESS(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, true, f.sosedb,ls,lw,w,f.bLR1free);
						}
					}
							} // section
						} // sections
					} // parallel

					// �������� ��������� �����
					for (integer iscan_par=nd.b0.iseparate_start; iscan_par<=nd.b0.iseparate_finish; iscan_par++) {
						integer iPloc=f.ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							// ��������� �������� �������� ��� ���������� ��.
		                    green_gaussPRESS(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, true, f.sosedb,ls,lw,w,f.bLR1free);
						}
					}


				}
			}

#else

	// ���������� ���������� �������� :
	// �� ������ ������������������ ���� ��������.
	// ��������� �������� ����������� ��� ���������� �������� ���-���.
	for (integer i=0; i<f.maxelm; i++) {
		// ��������� �������� ��� ���������� ��.
	    green_gaussPRESS(i, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, false,f.sosedb,ls,lw,w,f.bLR1free);
	}
	for (integer i=0; i<f.maxelm; i++) {
		// ��������� �������� ��� ��������� ��.
	    green_gaussPRESS(i, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, true,f.sosedb,ls,lw,w,f.bLR1free);
    }
	

	
#endif

	//printf("CORRECT start\n");
	//getchar();

	// ���������� ���������������� �������� � ���������� ��������.
	// 13 08 2015 �������� � ���������� ������� ��������� ��.
	// my_unsteady_temperature.c ����.
	//doublereal **SpeedCorOld=new doublereal*[3];
	//for (i=0; i<3; i++) {
	//	SpeedCorOld[i]=new doublereal[f.maxelm+f.maxbound];
	//}
	// �� ������ �������� � ��� ������ ���� ����, �� ����� 
	// �������� ���� ������ �����.
	//if (bfirst_start) {
		//for (i=VX; i<=VZ; i++) {
		  //  for (integer j=0; j<f.maxelm+f.maxbound; j++) {
			//    SpeedCorOld[i][j]=0.0;
		    //}
		//}
	//}
	//else {
#pragma omp parallel for
    for (integer j=0; j<f.maxelm+f.maxbound; j++) {
	    SpeedCorOld[VX][j]=f.potent[VXCOR][j];
	    SpeedCorOld[VY][j]=f.potent[VYCOR][j];
	    SpeedCorOld[VZ][j]=f.potent[VZCOR][j];
    }
	
	//}

	// ��������� ��������:
	// ������ ��� ���������� ��.
	/*
	for (i=0; i<f.maxelm; i++) {
		correct_internal_volume(i, VX, f.slau, f.nvtx, f.potent, f.maxelm, f.alpha, f.pa, f.sosedi, inumiter);
		correct_internal_volume(i, VY, f.slau, f.nvtx, f.potent, f.maxelm, f.alpha, f.pa, f.sosedi, inumiter);
		correct_internal_volume(i, VZ, f.slau, f.nvtx, f.potent, f.maxelm, f.alpha, f.pa, f.sosedi, inumiter);
        
		// �������� ����� !!!
		// �.�. ��������� ���� �� ������������ ��������� ��� ����� ��������� � ������������.
		// ��������� ���� ���� ��������� - ����� ������� ������� � ��������� �� ���������, ����
		// �� ������� ����� ���������� ������� ������� � �������� �� ������� ����� �������� � 
		// ��������� ��������� ��.
		// ���� ����� �������� �� ����������� ����� ����� ������ ���������������� ��������, 
		// ��������������� ��������� ������������� �� ����������� ���� � ���������.


	}
	*/
	
	
	// ��������� �������� �� ������ �������������� ������������ �� ������� �����-������ ��������� �������� ��������.
	/*
	for (i=0; i<f.maxelm; i++) {
		correct_internal_volume2(i, VX, f.slau, f.nvtx, f.potent, f.maxelm, f.alpha, f.pa, f.sosedi, inumiter);
		correct_internal_volume2(i, VY, f.slau, f.nvtx, f.potent, f.maxelm, f.alpha, f.pa, f.sosedi, inumiter);
		correct_internal_volume2(i, VZ, f.slau, f.nvtx, f.potent, f.maxelm, f.alpha, f.pa, f.sosedi, inumiter);
	}
	*/

	// ��������� �������� �� ������ �������������� ������������ �� ������� �����-������ ��������� �������� ��������.
	//  � ����������� ����������� ������������� !!!.
	// �� ����������� ������������� ������ ����������� �������� ������� ���������������� ���������������,
	// ������� �������� �� ������ ��������� � ����� ���������� ��������������� ����� ����� ����� ����� ����������
	// � ������������ ��� �� ������������. ��� ����������� �������� ������.
	/*
	for (i=0; i<f.maxelm; i++) {
	    correct_internal_volume3(i, VX, f.prop, f.potent,  tau[0]);
		correct_internal_volume3(i, VY, f.prop, f.potent,  tau[0]);
		correct_internal_volume3(i, VZ, f.prop, f.potent,  tau[0]);
	}*/

	// �������� �� ��� ��������� ���������� ����� �������������.
	for (integer i=0; i<f.maxelm; i++) {
	    correct_internal_volume4(i, VX, f.prop, f.potent,  tau);
		correct_internal_volume4(i, VY, f.prop, f.potent,  tau);
		correct_internal_volume4(i, VZ, f.prop, f.potent,  tau);
	}
	


	// �������� ����� !!!
	// �.�. ��������� ���� �� ������������ ��������� ��� ����� ��������� � ������������.
	// ��������� ���� ���� ��������� - ����� ������� ������� � ��������� �� ���������, ����
	// �� ������� ����� ���������� ������� ������� � �������� �� ������� ����� �������� � 
	// ��������� ��������� ��.
	// ���� ����� �������� �� ����������� ����� ����� ������ ���������������� ��������, 
	// ��������������� ��������� ������������� �� ����������� ���� � ���������.

	// ��������� ��������� ��������� ������ ��� ������� �� ������� ������ ��������:
	// ����� ����� ������������� � ���������������� �������� �� ���������� �������� 28.07.2016.
	correct_boundary_volume(VX, f.potent, f.maxelm, f.nvtx, f.pa, f.sosedi, f.sosedb, ls, lw, w, SpeedCorOld[VX]);
	correct_boundary_volume(VY, f.potent, f.maxelm, f.nvtx, f.pa, f.sosedi, f.sosedb, ls, lw, w, SpeedCorOld[VY]);
	correct_boundary_volume(VZ, f.potent, f.maxelm, f.nvtx, f.pa, f.sosedi, f.sosedb, ls, lw, w, SpeedCorOld[VZ]);

	
    for (integer i=0; i<(f.maxelm+f.maxbound); i++) {
		// ���������� ���� �������� 
		// ��������������� ���������
		// �������������, �����, �����
		// ����� ����������� � ���� ������ 
		// ����������.
		f.potent[VXCOR][i]=f.potent[VX][i];
        f.potent[VYCOR][i]=f.potent[VY][i];
        f.potent[VZCOR][i]=f.potent[VZ][i];
	}
	
	

	// �������� ������������ ��������� � ��������� ����� ��� ������� ����������.
    iscorrectOk(f.potent, f.maxelm, f.sosedi, f.sosedb, ls, lw, w);

	//printf("SINVARIANT end\n");
	//getchar();
	// �������� ��� ����� ��������� ����������� �� ������ ����� ������, �.�. � ������ ������-������
	// ������������ ������������� �� �����, � ������ ��������� ��� ����������� �������� ���-���.
	if (0) {

		// �������� 22 ���� 2012 ����.

		// �� ��������� ��������� ������, � ���������� �� ������ ����������������
		// ����� �������� � ��������.
		// ����������� �� ����� ���������� � ��������� �������� ������ ������� �������� �������� ?
		
		for (integer iP=0; iP<f.maxelm; iP++) {

			// ��������� ����������������� �������� ����� ����� ����� ��.
			// �������� ����� ����������� �� ������� �������� � ������
			// ���������������� �������� ���-��� �� ��� ��� ���������� ������������
			// ����������������� �������� � ���������������� ��������.
			if (btimedep)
			{
            /*
			return_calc_correct_mass_flux(iP, 
									 f.potent,
									 f.pa,
									 f.prop,
									 f.prop_b,
					                 f.nvtx,
									 f.sosedi,
									 f.maxelm,
									 f.diag_coef,
									 f.alpha,
						             RCh, // 1.0; 0.1;
									 btimedep,
						             dtimestep,
									 mfoldtimestep[iP],
						             f.mf[iP],  // ������������ �������� ��������� ������
									 speedoldtimestep,false,
									 SpeedCorOld,
									 mfold[iP]);
									 */

		  // ������ ������� ������������ �� ���������� ������������� &&
		  // ������������ � ������������� (��. ���� ��������� ������ sigmaflow).
          /*return_calc_correct_mass_flux2(iP,
			                             f.potent,
										 f.pa, 
										 f.prop,
										 f.prop_b,
					                     f.nvtx,
										 f.sosedi,
										 f.maxelm,
										 f.alpha,
										 RCh, // 1.0; 0.1;
						                 btimedep,
										 dtimestep, 
										 mfoldtimestep[iP],
						                 f.mf[iP], // ������������ �������� ��������� ������
										 speedoldtimestep, false,
						                 SpeedCorOld,
										 mfold[iP],
										 tau);*/

			// ������ �������������� ���������� �������������.
		   return_calc_correct_mass_flux3(iP,
			                             f.potent,
										 f.pa, 
										 f.prop,
										 f.prop_b,
					                     f.nvtx,
										 f.sosedi,
										 f.maxelm,
										 f.alpha,
										 RCh, // 1.0; 0.1;
						                 btimedep,
										 dtimestep, 
										 mfoldtimestep[iP],
						                 f.mf[iP], // ������������ �������� ��������� ������
										 speedoldtimestep, false,
						                 SpeedCorOld,
										 mfold[iP],
										 tau);

			}
			else {
				// ������������ ������.
				/*
            return_calc_correct_mass_flux(iP, 
									 f.potent,
									 f.pa,
									 f.prop,
									 f.prop_b,
					                 f.nvtx,
									 f.sosedi,
									 f.maxelm,
									 f.diag_coef,
									 f.alpha,
						             RCh, // 1.0; 0.1;
									 btimedep,
						             dtimestep,
									 NULL,
						             f.mf[iP], // ������������ �������� ��������� ������
									 NULL,false,
									 SpeedCorOld,
									 mfold[iP]);
									 */

			// ������ ������� ������������ �� ���������� ������������� &&
		    // ������������ � ������������� (��. ���� ��������� ������ sigmaflow).
			// ��� ��� �� ���� �� ����� ����� ��� �� ������� ������� � ������������ ������,
			// �� ������� ������������� ��������, ��� ����� �������� ������������ ������� ��������������
			// ����� �������������� ����������� ��������������� ������ � ����������� ����� ������������ 
			// � ����� �������� ��������������� ��������.
           /* return_calc_correct_mass_flux2(iP,
			                             f.potent,
										 f.pa, 
										 f.prop,
										 f.prop_b,
					                     f.nvtx,
										 f.sosedi,
										 f.maxelm,
										 f.alpha,
										 RCh, // 1.0; 0.1;
						                 btimedep,
										 -0.1,   //dtimestep, 
										 NULL,
						                 f.mf[iP], // ������������ �������� ��������� ������
										 NULL, false,
						                 SpeedCorOld,
										 mfold[iP],
										 tau);*/

            // ������ �������������� ���������� �������������.
			return_calc_correct_mass_flux3(iP,
			                             f.potent,
										 f.pa, 
										 f.prop,
										 f.prop_b,
					                     f.nvtx,
										 f.sosedi,
										 f.maxelm,
										 f.alpha,
										 RCh, // 1.0; 0.1;
						                 btimedep,
										 -0.1,   //dtimestep, 
										 NULL,
						                 f.mf[iP], // ������������ �������� ��������� ������
										 NULL, false,
						                 SpeedCorOld,
										 mfold[iP],
										 tau);

			}
						         

		}
	}

	// ��������� ��������� ������ �� ������� ������ � �������� ��������� ��������.
	if (1) {
    	correct_mf(f.mf, f.potent,  tau,
	               f.pa, f.sosedi, f.nvtx, f.maxelm,
				   f.sosedb, ls, lw, w, f.prop_b);
	}

	// ��������� �������� � ��������� ������ �� �������� ������
	// ����� ��������� ��������� ��. �������� ������.
	//mass_balance(f, lw, ls, w);

	
	// ������������ ����������� ������ �� ����.
	// 13 ������� 2015 �������� � ���������� ������� ��������� ��� ��������� ��������������.
	//for (i=0; i<3; i++) {
		//delete SpeedCorOld[i];
	//}
	//delete SpeedCorOld; 

	for (integer i=0; i<f.maxelm; i++) {
		for (integer j=0; j<6; j++) {
			// ������ ���� ����� ������ �������������� ��� ������ ����������
			// �������� ���-��� ������������ � ������ I.Sezai.
			mfold[i][j]=f.mf[i][j];
		}
	}

	// ������������ �������� ������������ ���������� ������� ����������.
	iscorrectmf(f.mf, f.maxelm, f.sosedi, f.sosedb, ls, lw, w);
	
	//printf("MASS FLUX end\n");
	//getchar();
	/*
	for (i=0; i<f.maxelm; i++) {
		f.potent[FBUF][i]=tau[VY][i];
	}
	for (i=0; i<f.maxbound; i++) {
		f.potent[FBUF][i+f.maxelm]=tau[VY][i];
	}
	*/


	// ������� ���������� ���������� � ��������� tecplot360:
	if (0) {
		//if (inumiter>82) {
		   exporttecplotxy360T_3D_part2(t.maxelm,t.ncell, fglobal, t, flow_interior,iflow,bextendedprint,0);
	       printf("corect values. OK.\n");
	       //getchar(); // debug avtosave
		   system("pause");
		//}
	}
	if (0) {
		xyplot( fglobal, flow_interior, t);
		printf("xy plot corect values. OK.\n");
	    //getchar(); // debug avtosave
		system("pause");
	}

	//printf("CORRECT end\n");
	//getchar();

#ifdef D_OPENMP

			if (inumcore==2) {
				if (nd.b0.active) {

					#pragma omp parallel 
					{
#pragma omp sections 
						{
#pragma omp section
							{

					// ������ �����
					for (integer iscan_par=nd.b0.ileft_start; iscan_par<=nd.b0.ileft_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							//green_gauss(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, false, f);
							green_gaussO1(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, false, f.mf[iPloc], f.prop[RHO], f.prop_b[RHO]);
						}

					}
							}
#pragma omp section
							{
					// ������ �����
					for (integer iscan_par=nd.b0.iright_start; iscan_par<=nd.b0.iright_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							//green_gauss(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, false, f);
							green_gaussO1(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, false, f.mf[iPloc], f.prop[RHO], f.prop_b[RHO]);
						}

					}
							} // section
						} // sections
					} // parallel
					// �������� ��������� �����
					for (integer iscan_par=nd.b0.iseparate_start; iscan_par<=nd.b0.iseparate_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							//green_gauss(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, false, f);
							green_gaussO1(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, false, f.mf[iPloc], f.prop[RHO], f.prop_b[RHO]);
						}
					}


				}
			}

			if (inumcore==2) {
				if (nd.b0.active) {

					#pragma omp parallel 
					{
#pragma omp sections 
						{
#pragma omp section
							{

					// ������ �����
					for (integer iscan_par=nd.b0.ileft_start; iscan_par<=nd.b0.ileft_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							//green_gauss(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, true, f);
						    green_gaussO1(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, true, f.mf[iPloc], f.prop[RHO], f.prop_b[RHO]);
						}

					}
							}
#pragma omp section
							{
					// ������ �����
					for (integer iscan_par=nd.b0.iright_start; iscan_par<=nd.b0.iright_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							//green_gauss(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, true, f);
					     	green_gaussO1(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, true, f.mf[iPloc], f.prop[RHO], f.prop_b[RHO]);
						}

					}
							} // section
						} // sections
					} // parallel
					// �������� ��������� �����
					for (integer iscan_par=nd.b0.iseparate_start; iscan_par<=nd.b0.iseparate_finish; iscan_par++) {
						integer iPloc=ifrontregulationgl[iscan_par];
						if (iPloc<f.maxelm) {
							//green_gauss(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, true, f);
						    green_gaussO1(iPloc, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, true, f.mf[iPloc], f.prop[RHO], f.prop_b[RHO]);
						}

					}


				}
			}

#else

	// ���������� ���������� �������� :
	// �� ������ ���� �������� ���������������� ��������� �������������.
	for (integer i=0; i<f.maxelm; i++) {
		// ��������� ��������� ��� ���������� ��.
	    green_gauss(i, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, false, f);
	}
	for (integer i=0; i<f.maxelm; i++) {
		// ��������� ��������� ��� ��������� ��.
	    green_gauss(i, f.potent, f.nvtx, f.pa, f.sosedi, f.maxelm, true, f);
    }

#endif

	

	// �� ������ ������ ������������ ������������ �������� ����� ����.
	// ���������� S ���������� ������� ���������-���������� ��� ����
	// ���������� � ��������� ����������� ������� ��������� �������.
	#pragma omp parallel for shared (f)  schedule (guided)
	for (integer i=0; i<(f.maxelm+f.maxbound); i++) {
		// �� ������ ������������ ������� ��. user_manual.
		doublereal sum=0.0;
		sum+=2.0*f.potent[GRADXVX][i]*f.potent[GRADXVX][i];
		sum+=2.0*f.potent[GRADYVY][i]*f.potent[GRADYVY][i];
		sum+=2.0*f.potent[GRADZVZ][i]*f.potent[GRADZVZ][i];
		sum+=(f.potent[GRADYVX][i]+f.potent[GRADXVY][i])*(f.potent[GRADYVX][i]+f.potent[GRADXVY][i]);
		sum+=(f.potent[GRADZVX][i]+f.potent[GRADXVZ][i])*(f.potent[GRADZVX][i]+f.potent[GRADXVZ][i]);
		sum+=(f.potent[GRADYVZ][i]+f.potent[GRADZVY][i])*(f.potent[GRADYVZ][i]+f.potent[GRADZVY][i]);
		// ��������� ��������� ����� ������� ����������� ��������� �������������.
		// �������� ��� ����� �������� �����������.
		sum-=(2.0/3.0)*(f.potent[GRADXVX][i]+f.potent[GRADYVY][i]+f.potent[GRADZVZ][i])*(f.potent[GRADXVX][i]+f.potent[GRADYVY][i]+f.potent[GRADZVZ][i]); // ������� ��������� � ��������������/������������
		f.SInvariantStrainRateTensor[i]=sqrt(fmax(0.0,sum));

		// ����� (������ ������ ��������).
		sum=0.0;
		sum+=(f.potent[GRADYVZ][i]-f.potent[GRADZVY][i])*(f.potent[GRADYVZ][i]-f.potent[GRADZVY][i]); // ���������.
		sum+=(f.potent[GRADZVX][i]-f.potent[GRADXVZ][i])*(f.potent[GRADZVX][i]-f.potent[GRADXVZ][i]);
		sum+=(f.potent[GRADXVY][i]-f.potent[GRADYVX][i])*(f.potent[GRADXVY][i]-f.potent[GRADYVX][i]);
		f.potent[CURL][i]=sqrt(sum);
	}
	

    // ������� ���������� ���������� � ��������� tecplot360:
	if (0) {
		 exporttecplotxy360T_3D_part2(t.maxelm,t.ncell, fglobal, t, flow_interior,iflow,false,0);
	     printf("temperature calculate begin now... OK.\n");
	     //getchar(); // debug avtosave
		 system("pause");
	}

	
	// btempdepend == true ���� ��������� ���������� ������� ������������� � ����������������.
	// ���������� ������� ��������� � ������ ���� ��������� ������� ����� ������� �� ����������� ���
	// � ������ ���� �� ����� ���� � ������������ ����������.
	// btempdepend == false ���� ��������� ������������� � ���������������� ����� ������ ���������:
	// ������� �������������, � ����� ����������������.
	bool btempdepend=true;
	if (itempersolve==1) {
         btempdepend=true; // ����� ������ ��������� ����������������.
	}
	else if (itempersolve==0) {
		btempdepend=false; // ��������� ���������������� ������������� ������ �������.
	}
	if (1&&(btempdepend)) {
		// �������� !!! ������� ��������� ������������� ���� ��������� � ���� ��� ���������� �������� �� �����
		// ������������� ���������� ��������� ��������� m ���������� �� ������ � BICGStab_internal3. 
		// ���� �� ������ ��������� � �������� �� �����. 12 ������ 2013 ����.

		// ��������� �������� ������ ���������� � �������� �������������.
		update_temp_properties(t, fglobal, b, lb, matlist);

        // �.�. ����������� ����������� ������� ���������� ������� �� �����������,
		// �� ���������� ������ ��������� ���������������� �� ������ ��������
		// SIMPLE ���������.
        //printf("Temp\n");
	    //solve(TEMP, res, f, fglobal, t, rhie_chow, s, w, b, ls, lw, lb, dbeta, flow_interior, true, false,NULL,0.01,false,dgx,dgy,dgz); 
		doublereal dbeta_temp=1.0; // 1.0 - ������ �������, 1.2 ������ �������
		// ���������� ������ ����������������:
		if (bprintmessage) {
		    printf("TEMP\n");
		    fprintf(fp_log,"TEMP\n");
	    }

		// ���������� �������� �������������� �� ���� ���������� �����.
		for (integer i47 = 0; i47 < t.maxelm; i47++) {
			// �������� � ��� ��� �������� �� ����������� ��� ������ � ������ ��������.
			integer ib = t.whot_is_block[i47];
			t.Sc[i47] = get_power(b[ib].n_Sc, b[ib].temp_Sc, b[ib].arr_Sc, t.potent[i47]);
		}
		
		
		// ���� �� �������� solve_nonlinear_temp �� �� ����� ������ ��������� ������������� �� �����,
		// � ���� ����� ��� ������������� ������������ ��������� ���� �������� ��� �� ������� � �� ������� ����������� �������� � ��� 
		// ����� ������������ ��� ������� ������. � ������� � ������������ ���������� ����� ������� ������� �� �������� �� ����������� ������� � �������� 
		// ��� ��������� ��������.
		
		/*		
		solve_nonlinear_temp(f, fglobal, t, rhie_chow,
			                 b, lb, s, ls, w, lw,
							 dbeta_temp, flow_interior,
							 true, toldtimestep, dtimestep, dtimestepold,
							 btimedep, matlist, inumiter,
							 bprintmessage, gtdps, ltdp, 1.0, m,
							 speedoldtimestep, mfoldtimestep);*/
		// ��������� �������� ������ ������� �������� ��� �������� �������.

		// ����������� ������.
            //m.bsignalfreeCRSt=true;
			doublereal** rsumanbstuff=NULL;
			bool  bconvective=true;
			doublereal power_on=1.0;

			// ����� ���-�� �������� �������� ������� ������ ����������� �� ������� ����,
			// �.�. ������� ����� ������� ���� ������������� ��������� ������������.
			// ������ ��������� ������� �.�. ������� ��� ���������.
			//rfluentrestemp = fluent_residual_for_x(t.slau, t.slau_bon, t.potent, t.maxelm, t.maxbound); // ������� �� ������� fluent.
			

			for (integer i = 0; i < t.maxelm + t.maxbound; i++) told_temperature_global_for_HOrelax[i] = t.potent[i];

		

			// ��� ���� ����� �������� �� ������ �������� �� �������, �.�. ��� � ��� ��� ��������.
			solve(TEMP, // ������������� ��������� (��� ��������� �������������).
				   res, // �������
				   f, 
				   fglobal,
				   t,
				   rhie_chow,
				   s, w, b, ls, lw, lb, // ������� (���������, ������, �����).
				   dbeta,
				   flow_interior,
				   bconvective,
				   false,
				   toldtimestep, // ���� ���������� � ����������� ���������� ����
				   NULL,
				   speedoldtimestep, // �������� � ����������� ���������� ����
				   mfoldtimestep, // ������������ ����� ����� ����� �� � ����������� ���������� ����
				   dtimestep, // ������ ���� �� �������
				   btimedep, // ������������ ��� �������������� ������
				   0.0, 0.0, 0.0, // ��������� ���������� �������
				   matlist, // ��������� ����������
				   inumiter,bprintmessage,RCh,false,
				   NULL,rsumanbstuff,false,false, power_on, m,
				   rthdsdt, rfluentrestemp, lu, my_union); // ����� ���������� ��������

			//doublereal tmax = 0.0;
			//for (integer i1 = 0; i1<t.maxelm + t.maxbound; i1++) tmax = fmax(tmax, fabs(t.potent[i1]));
			//printf("apost SIMPLE internal : maximum temperature in default interior is %1.4e\n", tmax);
			//getchar();

		doublereal fHORF = 0.25; // for steady state problem.
		if (btimedep) { // unsteady problems.
			fHORF = 0.75; // ANSYS Fluent Theory Guide.
		}
		for (integer i = 0; i < t.maxelm + t.maxbound; i++)  t.potent[i] = told_temperature_global_for_HOrelax[i] + fHORF*(t.potent[i] - told_temperature_global_for_HOrelax[i]);
		for (integer i = 0; i < t.maxelm + t.maxbound; i++) told_temperature_global_for_HOrelax[i] = t.potent[i];

		// ��������� �������� ������ ���������� � �������� �������������.
		update_temp_properties(t, fglobal, b, lb, matlist);

	}

	// 28.07.2016
	//exporttecplotxy360T_3D_part2(t.maxelm, t.ncell, fglobal, t, flow_interior, inumiter, bextendedprint);
	//getchar(); // debug

	//printf("TEMP end\n");
	//getchar();

	//exporttecplotxy360_3D( f.maxelm, f.ncell, f.nvtx, f.nvtxcell, f.pa, f.potent, rhie_chow);
	//getchar();

	// ������� ���������� ���������� � ��������� tecplot360:
	if (0) {
		 exporttecplotxy360T_3D_part2(t.maxelm,t.ncell, fglobal, t, flow_interior,iflow,false,0);
	     printf("one iteration finish... OK.\n");
	     //getchar(); // debug avtosave
		 system("pause");
	}

	/*if (inumiter==30) {
		getchar();
	}*/

	// ������������ ���������� ����������� ������ 
	for (integer i=0; i<3; i++) delete[] tau[i];
	delete[] tau; // ����������� ������ �� ��� ����������� ���� �� �������������.

	// ������������ ������ �� ��� ������������� ������������ ������������� ����.
	for (integer i=0; i<3; i++) delete[] sumanb[i];
	delete[] sumanb;

} // my_version_SIMPLE_Algorithm3D



#endif