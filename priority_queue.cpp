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
	void change(integer key_search, integer key_new, integer item_new)
	{
		if (hash[key_search] == 0) {
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

// ������� �� ����������� �� ������ ���� ����������.
// 21.13.2018
// ��� �������� O(1) � ������ �������� delete log2(N).

// ���� ������ � ��� ������.
struct data_BalTree
{
	// --> high priority --> for operation <,>
	//integer ii;
	integer  i, countsosed;
	// countsosed ���� key.
};


/*Copyright (c) 2010, Robin Message <Robin.Message@cl.cam.ac.uk>
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
* Neither the name of the Univsersity of Cambridge nor the
names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE UNIVERSITY OF CAMBRIDGE OR ROBIN MESSAGE
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Copyright(c) 2010, Robin Message <Robin.Message@cl.cam.ac.uk>
��� ����� ��������.
��������������� � ������������� � ���� ��������� � ��������� ����, � ��� ���
����������� ����������� ��� ���������� ��������� ������� :
*��� ��������������� ��������� ���� ������ ����������� ������������� ��������� �����
�������� ��������, ���� ������ ������� � ��������� ����� �� ���������������.
* ��� ��������� ��������������� ��������� ���� ������ �����������
�������� ��������, ���� ������ ������� � ��������� ����� �� ��������������� �
������������ � / ��� ������ ���������, ������������ ������ � �������������.
* �� �������� Univsersity ���������, ��
����� ��� ���������� ����� �������������� ��� ��������� ��� ����������� ���������
����������� �� ������� ������������ ����������� ��� ���������������� ����������� ����������.
��� ����������� ����������� ��������������� ����������� ��������� ���� � ����������� "��� ����" �
����� ����� ��� ��������������� ��������, �������, �� �� �������������, ���������������
�������� �������� ����������� � ����������� ��� ������������ ����
������������.�� � ���� ������ ������������ �����������
*/
/*
template <class V> class FibonacciHeap;

template <class V> struct node {
private:
	// ��������� �� ����� ����������� ����.
	node<V>* prev;
	// ��������� �� ������ ����������� ����.
	node<V>* next;
	// ��������� �� ���� �� �������� �����.
	node<V>* child;
	// ��������� �� ������������ ����.
	node<V>* parent;
	V value;

	// ���������� �������� �����.
	int degree;

	
	//���������� ��������, ������� ���������,
	//���� �� ������ ����� x �������� �����,
	//������� � �������, �����  x ���� ��������
	//����� ������-�� ������� ����.
	//FIBONNACCI_HEAP
	bool marked;
public:
	friend class FibonacciHeap<V>;
	node<V>* getPrev() { return prev; }
	node<V>* getNext() { return next; }
	node<V>* getChild() { return child; }
	node<V>* getParent() { return parent; }
	V getValue() { return value; }
	bool isMarked() { return marked; }

	bool hasChildren() { return child; }
	bool hasParent() { return parent; }
};

template <class V> class FibonacciHeap {
protected:
	node<V>* heap;
public:

	FibonacciHeap() {
		heap = _empty();
	}
	virtual ~FibonacciHeap() {
		if (heap) {
			_deleteAll(heap);
		}
	}
	node<V>* insert(V value) {
		node<V>* ret = _singleton(value);
		heap = _merge(heap, ret);
		return ret;
	}
	void merge(FibonacciHeap& other) {
		heap = _merge(heap, other.heap);
		other.heap = _empty();
	}

	bool isEmpty() {
		return heap == NULL;
	}

	V getMinimum() {
		return heap->value;
	}

	V removeMinimum() {
		node<V>* old = heap;
		heap = _removeMinimum(heap);
		V ret = old->value;
		delete old;
		old = NULL;
		return ret;
	}

	void decreaseKey(node<V>* n, V value) {
		heap = _decreaseKey(heap, n, value);
	}

	void deleteKey(V value) {
		node<V>* find_ = find(value);
		if (find_ != NULL) {
			decreaseKey(find_, -4294967296);
			removeMinimum();
		}
	}

	node<V>* find(V value) {
		return _find(heap, value);
	}
private:
	node<V>* _empty() {
		return NULL;
	}

	node<V>* _singleton(V value) {
		node<V>* n = new node<V>;
		n->value = value;
		n->prev = n->next = n;
		n->degree = 0;
		n->marked = false;
		n->child = NULL;
		n->parent = NULL;
		return n;
	}

	node<V>* _merge(node<V>* a, node<V>* b) {
		if (a == NULL)return b;
		if (b == NULL)return a;
		if (a->value>b->value) {
			node<V>* temp = a;
			a = b;
			b = temp;
		}
		node<V>* an = a->next;
		node<V>* bp = b->prev;
		a->next = b;
		b->prev = a;
		an->prev = bp;
		bp->next = an;
		return a;
	}

	void _deleteAll(node<V>* n) {
		if (n != NULL) {
			node<V>* c = n;
			do {
				node<V>* d = c;
				c = c->next;
				_deleteAll(d->child);
				delete d;
				d = NULL;
			} while (c != n);
		}
	}

	void _addChild(node<V>* parent, node<V>* child) {
		child->prev = child->next = child;
		child->parent = parent;
		parent->degree++;
		parent->child = _merge(parent->child, child);
	}

	void _unMarkAndUnParentAll(node<V>* n) {
		if (n == NULL)return;
		node<V>* c = n;
		do {
			c->marked = false;
			c->parent = NULL;
			c = c->next;
		} while (c != n);
	}

	node<V>* _removeMinimum(node<V>* n) {
		if (n == NULL)return n;
		_unMarkAndUnParentAll(n->child);
		if (n->next == n) {
			n = n->child;
		}
		else {
			n->next->prev = n->prev;
			n->prev->next = n->next;
			n = _merge(n->next, n->child);
		}
		if (n == NULL)return n;
		node<V>* trees[64] = { NULL };

		while (true) {
			if (trees[n->degree] != NULL) {
				node<V>* t = trees[n->degree];
				if (t == n)break;
				trees[n->degree] = NULL;
				if (n->value<t->value) {
					t->prev->next = t->next;
					t->next->prev = t->prev;
					_addChild(n, t);
				}
				else {
					t->prev->next = t->next;
					t->next->prev = t->prev;
					if (n->next == n) {
						t->next = t->prev = t;
						_addChild(t, n);
						n = t;
					}
					else {
						n->prev->next = t;
						n->next->prev = t;
						t->next = n->next;
						t->prev = n->prev;
						_addChild(t, n);
						n = t;
					}
				}
				continue;
			}
			else {
				trees[n->degree] = n;
			}
			n = n->next;
		}
		node<V>* min = n;
		node<V>* start = n;
		do {
			if (n->value<min->value)min = n;
			n = n->next;
		} while (n != start);
		return min;
	}

	node<V>* _cut(node<V>* heap, node<V>* n) {
		if (n->next == n) {
			n->parent->child = NULL;
		}
		else {
			n->next->prev = n->prev;
			n->prev->next = n->next;
			n->parent->child = n->next;
		}
		n->next = n->prev = n;
		n->marked = false;
		return _merge(heap, n);
	}

	node<V>* _decreaseKey(node<V>* heap, node<V>* n, V value) {
		if (n->value<value)return heap;
		n->value = value;
		if (n->parent) {
			if (n->value<n->parent->value) {
				heap = _cut(heap, n);
				node<V>* parent = n->parent;
				n->parent = NULL;
				while (parent != NULL && parent->marked) {
					heap = _cut(heap, parent);
					n = parent;
					parent = n->parent;
					n->parent = NULL;
				}
				if (parent != NULL && parent->parent != NULL)parent->marked = true;
			}
		}
		else {
			if (n->value < heap->value) {
				heap = n;
			}
		}
		return heap;
	}

	// ���������� ��������� ����� _find.
	// � ��������� ������ �� ������� ��������
	//������ �� ����������������� ��� �������.
	node<V>* _find(node<V>* heap, V value) {
		node<V>* n = heap;
		if (n == NULL)return NULL;
		do {
			if (n->value == value)return n;
			node<V>* ret = _find(n->child, value);
			if (ret)return ret;
			n = n->next;
		} while (n != heap);
		return NULL;
	}
};
*/

/* Copyright(c) 2010, Robin Message <Robin.Message@cl.cam.ac.uk>
��� ����� ��������.
��������������� � ������������� � ���� ��������� � ��������� ����, � ��� ���
����������� ����������� ��� ���������� ��������� ������� :
*��� ��������������� ��������� ���� ������ ����������� ������������� ��������� �����
�������� ��������, ���� ������ ������� � ��������� ����� �� ���������������.
* ��� ��������� ��������������� ��������� ���� ������ �����������
�������� ��������, ���� ������ ������� � ��������� ����� �� ��������������� �
������������ � / ��� ������ ���������, ������������ ������ � �������������.
* �� �������� Univsersity ���������, ��
����� ��� ���������� ����� �������������� ��� ��������� ��� ����������� ���������
����������� �� ������� ������������ ����������� ��� ���������������� ����������� ����������.
��� ����������� ����������� ��������������� ����������� ��������� ���� � ����������� "��� ����" �
����� ����� ��� ��������������� ��������, �������, �� �� �������������, ���������������
�������� �������� ����������� � ����������� ��� ������������ ����
������������.�� � ���� ������ ������������ �����������
*/
/*
template <class V> class FibonacciHeap;
const integer size_fibonacci_cashe = 2047483647;

template <class V> struct node {
private:
// ��������� �� ����� ����������� ����.
node<V>* prev;
// ��������� �� ������ ����������� ����.
node<V>* next;
// ��������� �� ���� �� �������� �����.
node<V>* child;
// ��������� �� ������������ ����.
node<V>* parent;
V value;

// ���������� �������� �����.
int degree;


//���������� ��������, ������� ���������,
//���� �� ������ ����� x �������� �����,
//������� � �������, �����  x ���� ��������
//����� ������-�� ������� ����.
//FIBONNACCI_HEAP
bool marked;
public:
	friend class FibonacciHeap<V>;
		node<V>* getPrev() { return prev; }
		node<V>* getNext() { return next; }
		node<V>* getChild() { return child; }
		node<V>* getParent() { return parent; }
		V getValue() { return value; }
		bool isMarked() { return marked; }

		bool hasChildren() { return child; }
		bool hasParent() { return parent; }
};

template <class V> class FibonacciHeap {
protected:
	node<V>* heap;
	node<V>** hash_index; // ��� ������� !!!
public:

FibonacciHeap() {
	heap = _empty();

	hash_index = NULL;
}

void WakeUp2() {
	hash_index = new node<V>*[size_fibonacci_cashe];
	for (integer i = 0; i < size_fibonacci_cashe; i++) hash_index[i] = NULL;
}

void WakeUp() {
	heap = _empty();

	hash_index = new node<V>*[size_fibonacci_cashe];
	for (integer i = 0; i < size_fibonacci_cashe; i++) hash_index[i] = NULL;
}

void Clear() {
	if (heap) {
		if (hash_index != NULL) {
			for (integer i = 0; i < size_fibonacci_cashe; i++) hash_index[i] = NULL;
		}
		_deleteAll(heap);
	}
}

virtual ~FibonacciHeap() {
	if (heap) {
		for (integer i = 0; i < size_fibonacci_cashe; i++) hash_index[i] = NULL;
		delete[] hash_index;
		hash_index = NULL;
		_deleteAll(heap);
	}
	else {
		if (hash_index != NULL) {
			for (integer i = 0; i < size_fibonacci_cashe; i++) hash_index[i] = NULL;
			delete[] hash_index;
			hash_index = NULL;
		}
	}
}
node<V>* insert(V value) {
	node<V>* ret = _singleton(value);
	heap = _merge(heap, ret);
	hash_index[-value] = ret;
	return ret;
}
void merge(FibonacciHeap& other) {
	heap = _merge(heap, other.heap);
	other.heap = _empty();
}

bool isEmpty() {
	return heap == NULL;
}

V getMinimum() {
	return heap->value;
}

V removeMinimum() {
	node<V>* old = heap;
	heap = _removeMinimum(heap);
	V ret = old->value;
	delete old;
	old = NULL;
	return ret;
}

void decreaseKey(node<V>* n, V value) {
heap = _decreaseKey(heap, n, value);
}

void deleteKey(V value) {
//node<V>* find_ = find(value);
	if (hash_index != NULL) {
		node<V>* find_ = hash_index[-value];
		if (find_ != NULL) {
			hash_index[-value] = NULL;
			decreaseKey(find_, -4294967296);
			removeMinimum();
		}
	}
}

//fibo_n = fibo_heap.find(-veb_dsearch_key);
//if (fibo_n == NULL) {
//	fibo_heap.insert(-veb_dadd_key);
//}
//else {
//	fibo_heap.decreaseKey(fibo_n, -veb_dadd_key);
//}
//fibo_n = NULL;
void insert_and_modify(V value_search, V value_add) {
	//node<V>* find_ = find(value_search);
	if (hash_index == NULL) {
		insert(value_add);
	}
	else {
		node<V>* find_ = hash_index[-value_search];
		if (find_ == NULL) {
			insert(value_add);
		}
		else {
			// ������ ������� ��������� �� find_.
			hash_index[-value_search] = NULL;
			hash_index[-value_add] = find_;
			decreaseKey(find_, value_add);
		}
		find_ = NULL;
	}
}

node<V>* find(V value) {
  //return _find(heap, value);
	return hash_index[-value];
}
private:
	node<V>* _empty() {
	return NULL;
}

node<V>* _singleton(V value) {
	node<V>* n = new node<V>;
	n->value = value;
	n->prev = n->next = n;
	n->degree = 0;
	n->marked = false;
	n->child = NULL;
	n->parent = NULL;
	return n;
}

node<V>* _merge(node<V>* a, node<V>* b) {
	if (a == NULL)return b;
	if (b == NULL)return a;
	if (a->value>b->value) {
		node<V>* temp = a;
		a = b;
		b = temp;
	}
	node<V>* an = a->next;
	node<V>* bp = b->prev;
	a->next = b;
	b->prev = a;
	an->prev = bp;
	bp->next = an;
	return a;
}

void _deleteAll(node<V>* n) {
	if (n != NULL) {
		node<V>* c = n;
		do {
			node<V>* d = c;
			c = c->next;
			_deleteAll(d->child);
			delete d;
			d = NULL;
		} while (c != n);
	}
}

void _addChild(node<V>* parent, node<V>* child) {
	child->prev = child->next = child;
	child->parent = parent;
	parent->degree++;
	parent->child = _merge(parent->child, child);
}

void _unMarkAndUnParentAll(node<V>* n) {
	if (n == NULL)return;
	node<V>* c = n;
	do {
		c->marked = false;
		c->parent = NULL;
		c = c->next;
	} while (c != n);
}

node<V>* _removeMinimum(node<V>* n) {
	if (n == NULL)return n;
	_unMarkAndUnParentAll(n->child);
	if (n->next == n) {
		n = n->child;
	}
	else {
		n->next->prev = n->prev;
		n->prev->next = n->next;
		n = _merge(n->next, n->child);
	}
	if (n == NULL)return n;
	node<V>* trees[64] = { NULL };

	while (true) {
		if (trees[n->degree] != NULL) {
			node<V>* t = trees[n->degree];
			if (t == n)break;
			trees[n->degree] = NULL;
			if (n->value<t->value) {
				t->prev->next = t->next;
				t->next->prev = t->prev;
				_addChild(n, t);
			}
			else {
				t->prev->next = t->next;
				t->next->prev = t->prev;
				if (n->next == n) {
					t->next = t->prev = t;
					_addChild(t, n);
					n = t;
				}
				else {
					n->prev->next = t;
					n->next->prev = t;
					t->next = n->next;
					t->prev = n->prev;
					_addChild(t, n);
					n = t;
				}
			}
			continue;
		}
		else {
			trees[n->degree] = n;
		}
		n = n->next;
	}
	node<V>* min = n;
	node<V>* start = n;
	do {
		if (n->value<min->value)min = n;
		n = n->next;
	} while (n != start);
	return min;
}

node<V>* _cut(node<V>* heap, node<V>* n) {
	if (n->next == n) {
		n->parent->child = NULL;
	}
	else {
		n->next->prev = n->prev;
		n->prev->next = n->next;
		n->parent->child = n->next;
	}
	n->next = n->prev = n;
	n->marked = false;
	return _merge(heap, n);
}

node<V>* _decreaseKey(node<V>* heap, node<V>* n, V value) {
	if (n->value<value)return heap;
	n->value = value;
	if (n->parent) {
		if (n->value<n->parent->value) {
			heap = _cut(heap, n);
			node<V>* parent = n->parent;
			n->parent = NULL;
			while (parent != NULL && parent->marked) {
				heap = _cut(heap, parent);
				n = parent;
				parent = n->parent;
				n->parent = NULL;
			}
			if (parent != NULL && parent->parent != NULL)parent->marked = true;
		}
	}
	else {
		if (n->value < heap->value) {
			heap = n;
		}
	}
	return heap;
}

// ���������� ��������� ����� _find.
// � ��������� ������ �� ������� ��������
//������ �� ����������������� ��� �������.
// ���������� � ��� ��� ��� ������� ���������� ������� �������,
// ����� ������� �� ��������� ������ � �� � ������������.
node<V>* _find(node<V>* heap, V value) {
	node<V>* n = heap;
	if (n == NULL)return NULL;
	do {
		if (n->value == value)return n;
		node<V>* ret = _find(n->child, value);
		if (ret)return ret;
		n = n->next;
	} while (n != heap);
	return NULL;
}
};
*/

// �������� �������������� � ��������� AliceFlow ������ ������������� ����.
// 15.07.2018
/* Copyright(c) 2010, Robin Message <Robin.Message@cl.cam.ac.uk>
��� ����� ��������.
��������������� � ������������� � ���� ��������� � ��������� ����, � ��� ���
����������� ����������� ��� ���������� ��������� ������� :
*��� ��������������� ��������� ���� ������ ����������� ������������� ��������� �����
�������� ��������, ���� ������ ������� � ��������� ����� �� ���������������.
* ��� ��������� ��������������� ��������� ���� ������ �����������
�������� ��������, ���� ������ ������� � ��������� ����� �� ��������������� �
������������ � / ��� ������ ���������, ������������ ������ � �������������.
* �� �������� Univsersity ���������, ��
����� ��� ���������� ����� �������������� ��� ��������� ��� ����������� ���������
����������� �� ������� ������������ ����������� ��� ���������������� ����������� ����������.
��� ����������� ����������� ��������������� ����������� ��������� ���� � ����������� "��� ����" �
����� ����� ��� ��������������� ��������, �������, �� �� �������������, ���������������
�������� �������� ����������� � ����������� ��� ������������ ����
������������.�� � ���� ������ ������������ �����������
*/

template <class V> class FibonacciHeap;


template <class V> struct node {
private:
	// ��������� �� ����� ����������� ����.
	node<V>* prev=NULL;
	// ��������� �� ������ ����������� ����.
	node<V>* next=NULL;
	// ��������� �� ���� �� �������� �����.
	node<V>* child=NULL;
	// ��������� �� ������������ ����.
	node<V>* parent=NULL;
	V value;

	// ���������� �������� �����.
	int degree=0;


	//���������� ��������, ������� ���������,
	//���� �� ������ ����� x �������� �����,
	//������� � �������, �����  x ���� ��������
	//����� ������-�� ������� ����.
	//FIBONNACCI_HEAP
	bool marked = false;
public:
	friend class FibonacciHeap<V>;
	node<V>* getPrev() { return prev; }
	node<V>* getNext() { return next; }
	node<V>* getChild() { return child; }
	node<V>* getParent() { return parent; }
	V getValue() { return value; }
	bool isMarked() { return marked; }

	bool hasChildren() { return child; }
	bool hasParent() { return parent; }
};

template <class V> struct FiboHashNode {
	node<V>* link=NULL;
	integer count_sosed=0;
};

template <class V> class FibonacciHeap {
protected:
	node<V>* heap=NULL;
	FiboHashNode<V>* hash_index=NULL; // ��� ������� !!!
	integer isize;
public:

	FibonacciHeap() {
		heap = _empty();

		isize = 0;//future n_a+1
		hash_index = NULL;
	}

	void WakeUp2(integer isize_loc) {
		isize = isize_loc;
		hash_index = new FiboHashNode<V>[isize];
		for (integer i = 0; i < isize; i++) {
			hash_index[i].link = NULL;
			hash_index[i].count_sosed = 0;
		}
	}

	void WakeUp(integer isize_loc) {
		heap = _empty();

		isize = isize_loc;
		hash_index = new FiboHashNode<V>[isize];
		for (integer i = 0; i < isize; i++) {
			hash_index[i].link = NULL;
			hash_index[i].count_sosed = 0;
		}
	}

	void Clear() {
		if (heap) {
			if (hash_index != NULL) {
				for (integer i = 0; i < isize; i++) {
					hash_index[i].link = NULL;
					hash_index[i].count_sosed = 0;
				}
			}
			_deleteAll(heap);
		}
	}

	void UpdateSize(integer isize_loc) {
		isize = isize_loc;
	}

	virtual ~FibonacciHeap() {
		if (heap) {
			for (integer i = 0; i < isize; i++) {
				hash_index[i].link = NULL;
				hash_index[i].count_sosed = 0;
			}
			delete[] hash_index;
			hash_index = NULL;
			_deleteAll(heap);
		}
		else {
			if (hash_index != NULL) {
				for (integer i = 0; i < isize; i++) {
					hash_index[i].link = NULL;
					hash_index[i].count_sosed = 0;
				}
				delete[] hash_index;
				hash_index = NULL;
			}
		}
	}
	node<V>* insert(V value) {
		node<V>* ret = _singleton(value);
		heap = _merge(heap, ret);

		integer i = ((-value) % (isize));
		integer countsosed = ((-value) / (isize));
		hash_index[i].link = ret;
		hash_index[i].count_sosed = countsosed;

		return ret;
	}
	void merge(FibonacciHeap& other) {
		heap = _merge(heap, other.heap);
		other.heap = _empty();
	}

	bool isEmpty() {
		return heap == NULL;
	}

	V getMinimum() {
		return heap->value;
	}

	V removeMinimum() {
		node<V>* old = heap;
		heap = _removeMinimum(heap);
		V ret = old->value;
		delete old;
		old = NULL;
		return ret;
	}

	void decreaseKey(node<V>* n, V value) {
		heap = _decreaseKey(heap, n, value);
	}

	void deleteKey(V value) {
		//node<V>* find_ = find(value);
		integer i = ((-value) % (isize));
		integer countsosed = ((-value) / (isize));

		if (hash_index != NULL) {
			node<V>* find_ = hash_index[i].link;
			if (find_ != NULL) {
				hash_index[i].link = NULL;
				hash_index[i].count_sosed = 0;

				decreaseKey(find_, -4294967296);
				removeMinimum();
			}
		}
	}

	void deleteKey(data_BalTree ddel) {

		if (hash_index != NULL) {
			node<V>* find_ = hash_index[ddel.i].link;
			if (find_ != NULL) {
				hash_index[ddel.i].link = NULL;
				hash_index[ddel.i].count_sosed = 0;

				decreaseKey(find_, -4294967296);
				removeMinimum();
			}
		}
	}

	//fibo_n = fibo_heap.find(-veb_dsearch_key);
	//if (fibo_n == NULL) {
	//	fibo_heap.insert(-veb_dadd_key);
	//}
	//else {
	//	fibo_heap.decreaseKey(fibo_n, -veb_dadd_key);
	//}
	//fibo_n = NULL;
	void insert_and_modify(V value_search, V value_add) {
		//node<V>* find_ = find(value_search);
		integer i = ((-value_search) % (isize));
		integer countsosed = ((-value_search) / (isize));

		if (hash_index == NULL) {
			insert(value_add);
		}
		else {
			node<V>* find_ = hash_index[i].link;
			if (find_ == NULL) {
				insert(value_add);
			}
			else {
				// ������ ������� ��������� �� find_.
				hash_index[i].link = NULL;
				hash_index[i].count_sosed = 0; 

				i = ((-value_add) % (isize));
				countsosed = ((-value_add) / (isize));

				hash_index[i].link = find_;
				hash_index[i].count_sosed = countsosed;
				decreaseKey(find_, value_add);
			}
			find_ = NULL;
		}
	}

	node<V>* find(V value) {
		//return _find(heap, value);
		return hash_index[-value];
	}
private:
	node<V>* _empty() {
		return NULL;
	}

	node<V>* _singleton(V value) {
		node<V>* n = new node<V>;
		n->value = value;
		n->prev = n->next = n;
		n->degree = 0;
		n->marked = false;
		n->child = NULL;
		n->parent = NULL;
		return n;
	}

	node<V>* _merge(node<V>* a, node<V>* b) {
		if (a == NULL)return b;
		if (b == NULL)return a;
		if (a->value>b->value) {
			node<V>* temp = a;
			a = b;
			b = temp;
		}
		node<V>* an = a->next;
		node<V>* bp = b->prev;
		a->next = b;
		b->prev = a;
		an->prev = bp;
		bp->next = an;
		return a;
	}

	void _deleteAll(node<V>* n) {
		if (n != NULL) {
			node<V>* c = n;
			do {
				node<V>* d = c;
				c = c->next;
				_deleteAll(d->child);
				delete d;
				d = NULL;
			} while (c != n);
		}
	}

	void _addChild(node<V>* parent, node<V>* child) {
		child->prev = child->next = child;
		child->parent = parent;
		parent->degree++;
		parent->child = _merge(parent->child, child);
	}

	void _unMarkAndUnParentAll(node<V>* n) {
		if (n == NULL)return;
		node<V>* c = n;
		do {
			c->marked = false;
			c->parent = NULL;
			c = c->next;
		} while (c != n);
	}

	node<V>* _removeMinimum(node<V>* n) {
		if (n == NULL)return n;
		_unMarkAndUnParentAll(n->child);
		if (n->next == n) {
			n = n->child;
		}
		else {
			n->next->prev = n->prev;
			n->prev->next = n->next;
			n = _merge(n->next, n->child);
		}
		if (n == NULL)return n;
		node<V>* trees[64] = { NULL };

		while (true) {
			if (n != NULL) {
				if (trees[n->degree] != NULL) {
					node<V>* t = trees[n->degree];
					if (t == n) break;
					trees[n->degree] = NULL;
					if (n->value < t->value) {
						t->prev->next = t->next;
						t->next->prev = t->prev;
						_addChild(n, t);
					}
					else {
						t->prev->next = t->next;
						t->next->prev = t->prev;
						if (n->next == n) {
							t->next = t->prev = t;
							_addChild(t, n);
							n = t;
						}
						else {
							n->prev->next = t;
							n->next->prev = t;
							t->next = n->next;
							t->prev = n->prev;
							_addChild(t, n);
							n = t;
						}
					}
					continue;
				}
				else {
					trees[n->degree] = n;
				}
				n = n->next;
			}
			else {
				break;
			}
		}
		node<V>* min_1 = n;
		node<V>* start = n;
		if (n != NULL) {
			do {
				if (n->value < min_1->value) min_1 = n;
				n = n->next;
			} while (n != start);
		}
		return min_1;
	}

	node<V>* _cut(node<V>* heap, node<V>* n) {
		if (n->next == n) {
			n->parent->child = NULL;
		}
		else {
			n->next->prev = n->prev;
			n->prev->next = n->next;
			n->parent->child = n->next;
		}
		n->next = n->prev = n;
		n->marked = false;
		return _merge(heap, n);
	}

	node<V>* _decreaseKey(node<V>* heap, node<V>* n, V value) {
		if (n->value<value)return heap;
		n->value = value;
		if (n->parent) {
			if (n->value<n->parent->value) {
				heap = _cut(heap, n);
				node<V>* parent = n->parent;
				n->parent = NULL;
				while (parent != NULL && parent->marked) {
					heap = _cut(heap, parent);
					n = parent;
					parent = n->parent;
					n->parent = NULL;
				}
				if (parent != NULL && parent->parent != NULL)parent->marked = true;
			}
		}
		else {
			if (n->value < heap->value) {
				heap = n;
			}
		}
		return heap;
	}

	// ���������� ��������� ����� _find.
	// � ��������� ������ �� ������� ��������
	//������ �� ����������������� ��� �������.
	// ���������� � ��� ��� ��� ������� ���������� ������� �������,
	// ����� ������� �� ��������� ������ � �� � ������������.
	node<V>* _find(node<V>* heap, V value) {
		node<V>* n = heap;
		if (n == NULL)return NULL;
		do {
			if (n->value == value)return n;
			node<V>* ret = _find(n->child, value);
			if (ret)return ret;
			n = n->next;
		} while (n != heap);
		return NULL;
	}
};
