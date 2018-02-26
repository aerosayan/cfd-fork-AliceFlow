// ������� �� ����������� �� ������ ������������ ������
// 12.06.2017 (����).
// priority_queue.cpp

// ������ ��������� ��������, ������ ������ ������������ ������� �����
// ���������, ������ ������� ������������ �������.
// �� �� ������� � ����� ������ ������ ��� ��� ��������.
// ����� ���������� ���������������.

// ������������������ ���������� ��� �������� 
// (��� �������� �������� �������� � ������ �� �����) ������ ����������, �� 
// ��������� ���������� �������� ���� ����������, � ��� ������� ���
// ��������� � ����� ��� ����������� 100% ������� ������ ���� ���� 
// � ����������������� ��� ��������. 
// ��� ������ ���� ����� ������� ��� �.�. 1. ��� ���������� new � delete -
// ������ ���������� ���� ���� ��� � ������ ������������� � ������������� ��������������
// ������������. 2. �������� �������� ���������������������� ���������.
// ������� : � ��� ������������ �������������� ��������������� ����� ������ ?
// �����: ������������ �� ��������� �.�. �� ������ ����������� �������� ���� ��� ��������� ����������� ��������
// ������ � ������� ���� ��������� ������� �������� ������� ��� ���������� ����� ���������. ��� �����
// ������ ������ �������� ���� ������ �������� �� ��������� 2 �� ����� ��������� � ����.

/*

// ���������� ��������� ��������� ������������ ������.
// ������ ������� �. 366 � ����� 2002 ����.
template <class Item>
void fixUp(Item a[], integer k)
{
	while (k > 1 && a[k / 2] < a[k])
	{
		integer kdiv2 = k / 2;
		Item buf = a[k];
		a[k] = a[kdiv2];
		a[kdiv2] = buf;

		k = kdiv2;
	}
}

// ���������� ��������� ��������� ������������ ������.
template <class Item>
void fixDown(Item a[], integer k, integer N)
{
	while (2 * k <= N)
	{
		integer j = 2 * k;
		if (j < N&&a[j] < a[j + 1]) j++;
		if (!(a[k] < a[j])) break;

		Item buf = a[k];
		a[k] = a[j];
		a[j] = buf;

		k = j;
	}
}

template <class Item>
class PQ
{
private:
	Item *pq;
	integer N;
	integer isize;
public:
	PQ(integer maxN)
	{
		isize = maxN;
		pq = new Item[maxN + 1]; 
		N = 0;
	}
	~PQ()
	{
		if (pq != NULL) delete[] pq;
	    N = 0;
	}
	integer empty() const
	{
		return N == 0;
	}
	// �������� ������� � ������� �� 
	// �����������.
	void insert(Item item)
	{
		if (N + 1 > isize) {
			printf("ERROR!!! priority_queue stack overflow...\n");
			system("pause");
			exit(1);
		}
		else {
			pq[++N] = item;
			fixUp(pq, N);
		}
	}
	// ������ ��������� ����������� �������.
	Item readmax()
	{
		return pq[1];
	}
	// ���������� ������������ �������
	// � ������� ���.
	Item getmax()
	{
		Item buf = pq[1];
		pq[1] = pq[N];
		pq[N] = buf;
		fixDown(pq, 1, N - 1);
		return pq[N--];
	}
};

*/

// ������������ � �������������� ������������� ������.
// ������������ ������ ��������� �������:
// clear, remove, insert, readkeymaxelm.

// ��������� � ����������������� ��� ��������.
template <class Item>
void exch(integer i, integer j, Item* &pq, integer* &qp, integer* &hash) {
	// exchange
	/*
	// begin
	integer t;

	Item buf1 = pq[qp[i]];
	Item buf2 = pq[qp[j]];
	t = qp[i];

	qp[i] = qp[j];
	qp[j] = t;

	pq[qp[i]] = buf1;
	pq[qp[j]] = buf2;

	// end
	*/
	//printf("exchange\n");

	Item t;

	t = pq[j];
	pq[j] = pq[i];
	pq[i] = t;

	integer p;

	p = hash[qp[i]];
	hash[qp[i]] = hash[qp[j]];
	hash[qp[j]] = p;

	p = qp[j];
	qp[j] = qp[i];
	qp[i] = p;

	

}

// ���������� ��������� ��������� ������������ ������.
// ������ ������� �. 366 � ����� 2002 ����.
template <class Item>
void fixUp(Item* &a, integer* &inda, integer* &hash, integer k)
{
	while (k > 1 && a[k / 2] < a[k])
	{
		integer kdiv2 = k / 2;

		exch(k, kdiv2, a, inda, hash);

		k = kdiv2;
	}
}

// ���������� ��������� ��������� ������������ ������.
template <class Item>
void fixDown(Item* &a, integer* &inda, integer* &hash, integer k, integer N)
{
	while (2 * k <= N)
	{
		

		integer j = 2 * k;
		if (j < N&&a[j] < a[j + 1]) j++;
		if (!(a[k] < a[j])) break;


		exch(k, j, a, inda, hash);

		k = j;
	}
}

//PQ(integer maxN, integer max_key_size);
//~PQ();
// ���� �� ������� � ������ ������ � ������� ?
//bool isfound(integer key);
//bool empty() const;
// ������� ���������� � ��� ����� ������ � �������������.
//void clear();
// ������� ������� � �������� ������:
// ����������� �������������� ��� ���� ���������� ������ �������.
//Item get(integer key);
//Item readmax();
//integer readkeymaxelm();
// �������� ������� item � ������� �� 
// ����������� ���� ������� item ����� ���� key.
//template <class Item>
//void insert(Item item, integer key);
// ���������� ������������ �������
// � ������� ���.
//Item getmax();
// �������� ������� � ������ key �� ������� val � ��� �� ������ key.
// ��� ���� ���� key ������ ���� ����������.
//void modify(integer key, Item val);
// �������� �������� � �������� ��������� �����.
//void remove(integer key);
// � �������� ������� �������� ������� ����� �� ����� ����
// ��� ���� �������� � ���� ���������� ��������.
//void change(integer key_serch, integer key_new, integer item_new);


// ����� ������ ���� ���������, ������������ � ��������.
// ���� ���������� ������ ���� ��������, ����� �������� � ��� �������.

template <class Item>
class PQ
{
private:
	// �������� binary heap.
	Item *pq;
	// �������� ������ �� ������ � qp �� ������ � hash.
	integer *qp; // ������ �� ��� �������.
	// ������ �� ����� � ���� � pq.
	integer *hash; // ��� ������� !!!
	integer N;
	integer isize;
	integer ihash_size;

public:
	PQ(integer maxN, integer max_key_size)
	{
		isize = maxN;
		pq = new Item[maxN + 1];
		qp = new integer[maxN + 1];
		for (integer i_1 = 0; i_1 < maxN + 1; i_1++) {
			// �������������: ������� ����� �.�. ���� 0
			// � ������� pq ������� �� ������������.
			qp[i_1] = 0;
		}
		N = 0;
		// ��� ������� !!!
		ihash_size = max_key_size;
		hash = new integer[max_key_size+2];
		for (integer i_1 = 0; i_1 < max_key_size + 2; i_1++) {
			// �������������: ������� ����� �.�. ���� 0
			// � ������� pq ������� �� ������������.
			hash[i_1] = 0;
		}
	}
	~PQ()
	{
		if (pq != NULL) delete[] pq;
		N = 0;
		if (qp != NULL) delete[] qp;
		if (hash != NULL) delete[] hash;
	}
	void print_log(char ch) {
		printf("%c\n",ch);
		for (integer i_1 = 1; i_1 <= N; i_1++) {
#if doubleintprecision == 1
			printf("[%lld %lld] ",pq[i_1],qp[i_1]);
#else
			printf("[%d %d] ", pq[i_1], qp[i_1]);
#endif
		}
		printf("\n");
		system("PAUSE");
	}

	// ������ ������� �������� ��������� � ������� i � j. 
	// ��������� ������� ����.
	void exchange_speshial_for_Saad(integer i, integer j) {

		//Item t;



		//t = pq[hash[j]];
		//pq[hash[j]] = pq[hash[i]];
		//pq[hash[i]] = t;

		// ���� ����� ������� ��������� ������� ����.
		Item t1 = get(i);
		//Item t2 = get(j);

		//this->remove(i);
		//this->remove(j);
		//this->insert(t1, j);
		//this->insert(t2, i);

		// ���������������� �����������
		this->remove(j);
		this->insert(t1, j);

	}

	// ������� ���������� � ��� ����� ������ � �������������.
	void clear()
	{
		for (integer i_1 = 0; i_1 < N + 1; i_1++) {
			// ���������� ������� ��� �������.
			hash[qp[i_1]] = 0;
		}
		/*
		// ��������� ������ �������.
		for (integer i_1 = 0; i_1 < isize + 1; i_1++) {
			// �������������: ������� ����� �.�. ���� 0
			// � ������� pq ������� �� ������������.
			qp[i_1] = 0;
		}
		*/
		// 25.02.2018
		// �������������� ���� �.�. �� ������������� ������ 
		// ����� ������� � ������� � �� ���� �������� � �������.
		for (integer i_1 = 0; i_1 < N + 1; i_1++) {
			qp[i_1] = 0;
		}
		N = 0;
		/*
		for (integer i_1 = 0; i_1 < ihash_size + 2; i_1++) {
			// �������������: ������� ����� �.�. ���� 0
			// � ������� pq ������� �� ������������.
			hash[i_1] = 0;
		}
		*/
	}
	bool empty() const
	{
		return N == 0;
	}
	// ���� �� ������� � ������ ������ � ������� ?
	bool isfound(integer key) {
		if (hash[key] == 0) {
			// ������� ����������� � ��� �������.
			return false;
		}
		return true;
	}
	// ������� ������� � �������� ������:
	// ����������� �������������� ��� ���� ���������� ������ �������.
	Item get(integer key) {
		if (hash[key] == 0) {
			// ������� ����������� � ��� �������.
			printf("priority queue get ERROR: get element not found.\n");
			system("pause");
			exit(1);
		}
		return pq[hash[key]];
	}
	// ������ ��������� ����������� �������.
	Item readmax()
	{
		return pq[1];
	}
	integer readkeymaxelm() {
		return qp[1];
	}

	

	// �������� ������� item � ������� �� 
	// ����������� ���� ������� item ����� ���� key.
	template <class Item>
	void insert(Item item, integer key)
	{
		if (N + 1 > isize) {
			printf("ERROR!!! priority_queue stack overflow...\n");
#if doubleintprecision == 1
			printf("N=%lld\n",N);
#else
			printf("N=%d\n", N);
#endif
			system("pause");
			exit(1);
		}
		else {
			pq[++N] = item;
			hash[key] = N;
			qp[N] = key;
			fixUp(pq, qp, hash, N);
		}
		//print_log('i');
	}
	
	


	// ���������� ������������ �������
	// � ������� ���.
	Item getmax()
	{
		exch(1, N, pq, qp, hash);

		fixDown(pq, qp, hash, 1, N - 1);
		return pq[N--];
	}

	

	// �������� ������� � ������ key �� ������� val � ��� �� ������ key.
	// ��� ���� ���� key ������ ���� ����������.
	void modify(integer key, Item val)
	{
		if (hash[key] == 0) {
			// ������� ����������� � ��� �������.
			printf("priority queue modify ERROR: get element not found.\n");
			system("pause");
			exit(1);
		}

		pq[hash[key]] = val;
		// ������ ���������� ������������ ������� ����.
		integer i = hash[key];
		fixUp(pq, qp, hash, i);
		fixDown(pq, qp, hash, i, N);
	}


	

	// �������� �������� � �������� ��������� �����.
	void remove(integer key)
	{
		if (N > 0) {
			if (hash[key] == 0) {
				// ������� ����������� � ��� �������.
				// ������ �� ������ �.�. �������� ��� ���.
			}
			else {

				// ��������.
				if (hash[key] == N) {
					N--;
					hash[key] = 0;
					qp[N + 1] = 0;
					// ���� �������� �� �������.
				}
				else {

					integer i = hash[key];

					exch(hash[key], N, pq, qp, hash);

					hash[qp[N]] = 0;
					qp[N] = 0;
					N--;

					// ������ ���������� ������������ ������� ����.
					fixUp(pq, qp, hash, i);
					fixDown(pq, qp, hash, i, N);

				}

			}
		}
		//print_log('r');

	}
	
	// � �������� ������� �������� ������� ����� �� ����� ����
	// ��� ���� �������� � ���� ���������� ��������.
	void change(integer key_serch, integer key_new, integer item_new)
	{
		if (hash[key_serch] == 0) {
			// ������� ����������� � ��� �������.
			if (hash[key_new] != 0) {
				// ������� ������������ � ��� �������.
				pq[hash[key_new]] = item_new;
				// ������ ���������� ������������ ������� ����.
				fixUp(pq, qp, hash, hash[key_new]);
				fixDown(pq, qp, hash, hash[key_new], N);
			}
			else {
				// ������� ������ ����� � ������ �������.
				insert(item_new, key_new);
			}
		}
		else {
			if (hash[key_new] != 0) {
				// �������� ������� ����� �� ���� ��� ����������.
				remove(key_search);
				// ������� ������������ � ��� �������.
				pq[hash[key_new]] = item_new;
				// ������ ���������� ������������ ������� ����.
				fixUp(pq, qp, hash[key_new]);
				fixDown(pq, qp, hash[key_new], N);
				
			}
			else {
				// key_new �����������.

				hash[key_new] = hash[key_search];
				hash[key_search] = 0; // ���������� �� ������.
				pq[hash[key_new]] = item_new;
				qp[hash[key_new]] = key_new;
				// ������ ���������� ������������ ������� ����.
				fixUp(pq, qp, hash, hash[key_new]);
				fixDown(pq, qp, hash, hash[key_new], N);

			}
		}
	}

	
};

// ��� �������� O(1) � ������ �������� delete log2(N).

typedef struct T_Fibbonacci_node {
	integer key;

	// ��������� �� ������������ ����.
	T_Fibbonacci_node* p;
	// ��������� �� ���� �� �������� �����.
	T_Fibbonacci_node* child;
	// ��������� �� ����� ����������� ����.
	T_Fibbonacci_node* left;
	// ��������� �� ������ ����������� ����.
	T_Fibbonacci_node* right;

	// ���������� �������� �����.
	integer degree;
	/*
	���������� ��������, ������� ���������,
	���� �� ������ ����� x �������� �����,
	������� � �������, �����  x ���� �������� 
	����� ������-�� ������� ����.
	*/
	bool mark;

} Fibbonacci_node;

