// ���������������� ������� ������.
// ������������ � �������������� ������������� ������.
#pragma once
#ifndef _AVL_BALANCED_TREE_CPP_
#define _AVL_BALANCED_TREE_CPP_ 1

// ��� ��� ������
// 12.12.2015
// �� �������� � 3D ����� ������� ���������� ������� � ������� 
// �������� ����������� ������ �� ����������� � ����� ������� 
// ����������� ��������� �� ��������� ��������������. 
// ��� ��������� � 2D ��� 1� ����������� ����� ��� ��������
// ������ ����������� ���������� ������� �� ����� 2D ������ �������
// � ���� �� ����������� 
// ����������� �� ����� ����� �� ��������� �������-������ ��������
// ��������. 

// ��� ������ ��� ��������� ������ ����������� ����������� ��� 
// �������� �� log_2 (number of neighbour count) ��������. 

// �.�. ��������-�������� � �.�. ������ 1962 
// (����� �� ��������� ������ ������� ������ �� �����.).
// ���� � ������������ ��������� ����� ������� ������.

// ��� ���� ������ ���� ���������� ������������� ��������,
// ������, ������ � �����.

// ���� ������ � ��� ������.
// ���������� � ���� priority_queue.cpp
//class data_BalTree
//{
	// public:	
	// --> high priority --> for operation <,>
	//integer ii;
	//integer  i, count_neighbour;
	// count_neighbour ���� key.
	// ������������� �������� ��������� ��� ���������� �����.
	//bool operator <(const data_BalTree&);
	//bool operator >(const data_BalTree&);
	//bool operator ==(const data_BalTree&);
//};

//��������� ������� ������ � insert � remove

// ���� ��� ������.
struct node_AVL
{
	data_BalTree key;
	// ������ ��������� � ������ � ������ ����.
	unsigned char height;
	node_AVL* left;
	node_AVL* right;
	// �����������.
	node_AVL(data_BalTree k) { key = k; left = right = 0; height = 1; }
	node_AVL() {
		height=1;
		left = nullptr;
		right = nullptr;
	}
};

// �������� ����� � � ������� ���������.
// ������ ���� height
unsigned char height(node_AVL* p)
{
	return p ? p->height : 0;
};

// ��������� balance factor ��������� ����
// �������� ������ � ���������� �����������.
integer bfactor(node_AVL* p)
{
	return height(p->right) - height(p->left);
};

// ��������������� ���������� �������� ���� height
// ��������� ���� (��� �������, ��� �������� ����� ���� 
// � ������ � ����� �������� ����� �������� �����������).
void fixheight(node_AVL* p)
{
	unsigned char hl = height(p->left);
	unsigned char hr = height(p->right);
	p->height = (hl > hr ? hl : hr) + 1;
};

// ������������ �����.
node_AVL* rotateright(node_AVL* p)
{
	// ������ ������� ������ p
	node_AVL* q = p->left;
	p->left = q->right;
	q->right = p;
	fixheight(p);
	fixheight(q);
	return q;
};

// ����� ������� �������� ������������ ������ �������:
node_AVL* rotateleft(node_AVL* q)
{
	// ����� ������� ������ q
	node_AVL* p = q->right;
	q->right = p->left;
	p->left = q;
	fixheight(q);
	fixheight(p);
	return p;
};

// ��� ����������� ������������ �������� � �������� ������� � ���������� ���������
node_AVL* balance(node_AVL* p) // ������������ ���� p
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
node_AVL* insert(node_AVL*& p, data_BalTree k)
{
	// ������� ����� k � ������ � ������ p
	if (p == nullptr) {
		node_AVL* r1 = nullptr;
		r1 = new node_AVL(k);
		if (r1 == nullptr) {
			// ������������ ������ �� ������ ������������.
			printf("Problem: not enough memory on your equipment for r1 in insert my_agregat_amg...\n");
			printf("Please any key to exit...\n");
			//getchar();
			system("pause");
			exit(1);
		}
		return r1;
	}
	if (k < p->key)
		p->left = insert(p->left, k);
	else
		p->right = insert(p->right, k);

	return balance(p);
} // insert

  // ���������� true ���� ���� ������ � ������
bool isfound(node_AVL* p, data_BalTree k)
{
	if (p == 0) return false; // ��������.
	if (k < p->key)
		return isfound(p->left, k);
	else if (k > p->key)
		return isfound(p->right, k);
	else return true; // ������.
}

// ������ �������� ��������� ������.
void clear_AVL(node_AVL* p)
{
	if (p != 0) {
		clear_AVL(p->left);
		clear_AVL(p->right);
		// ������� ����.
		delete p;
		p = 0;
	}
} // clear_AVL

  // �������� ���� � ��������� �������� � ����������� ������������������.
node_AVL* findmin(node_AVL* p)
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

node_AVL* findmax(node_AVL* p)
{
	// ����� ���� � ������������ ������ � ������ p
	if (p != 0) {
		return p->right ? findmax(p->right) : p;
		/*
		#if doubleintprecision == 1
			if (p->right == 0) {
				//printf("%lld %lld %lld\n", p->key.count_neighbour, p->key.i, p->key.ii);
				//system("pause");
				return p;
			}
			else {
				//printf("%lld %lld %lld\n",p->key.count_neighbour,p->key.i,p->key.ii);
				findmax(p->right);
			}
		#else
			if (p->right == 0) {
				//printf("%d %d %d\n", p->key.count_neighbour, p->key.i, p->key.ii);
				//system("pause");
				return p;
			}
			else {
				//printf("%d %d %d\n",p->key.count_neighbour,p->key.i,p->key.ii);
				findmax(p->right);
			}
		#endif

		*/
	}
	else {
		// �� ����� ��������� ����� ������� ���������.
		return 0;
	}
} // findmax

data_BalTree get_max_AVL(node_AVL* p)
{
	// ����������� ������������� ���� � ������.
	return p->right ? get_max_AVL(p->right) : p->key;
}

node_AVL* removemin(node_AVL* p)
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
node_AVL* remove_AVL(node_AVL* p, data_BalTree k)
{
	// ��������� ������� ���������� 
	// ��� ��������� �� ��� ����� �����.
	// �������� ����� k �� ������ p
	if (p == 0) return 0;
	// �������� ����� ������� ��������.
	if (k < p->key)
		p->left = remove_AVL(p->left, k);
	else if (k > p->key)
		p->right = remove_AVL(p->right, k);
	else // k==p->key
	{
		node_AVL* q = p->left;
		node_AVL* r = p->right;
		delete p;
		p = 0;
		if (r == 0) return q;
		node_AVL* min = findmin(r);
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
node_AVL* insert_and_modify(node_AVL* p, data_BalTree k, data_BalTree k_search)
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
void print_AVL(node_AVL* p)
{
	if (p != 0) {
		print_AVL(p->left);
		for (integer i = 0; i <= p->height; i++) {
			printf(" ");
		}
#if doubleintprecision == 1
		//printf("%lld %lld %lld\n", p->key.count_neighbour, p->key.i, p->key.ii);
		printf("%lld %lld\n", p->key.count_neighbour, p->key.i);
#else
		//printf("%d %d %d\n", p->key.count_neighbour, p->key.i, p->key.ii);
		printf("%d %d\n", p->key.count_neighbour, p->key.i);
#endif

		print_AVL(p->right);
	}
} // print_AVL

  // ������������ ��� ������
  // 12 ������� 2015 �����������������.
void test_AVL()
{
	node_AVL* root = 0;
	data_BalTree d3;
	d3.count_neighbour = rand();
	d3.i = rand();
	//d3.ii = rand();
	root = remove_AVL(root, d3);
	for (integer i = 0; i < 10; i++)
	{
		data_BalTree d;
		d.count_neighbour = rand();
		d.i = rand();
		//d.ii = rand();
		root = insert_and_modify(root, d, d);
		print_AVL(root);
		if (i == 5) {

			//41 18467 6334


			data_BalTree d1;
			d1.count_neighbour = 41;
			d1.i = 18467;
			//d1.ii = 6334;
			data_BalTree d2;
			d2.count_neighbour = rand();
			d2.i = rand();
			//d2.ii = rand();
			root = insert_and_modify(root, d2, d1);
			print_AVL(root);
			printf("remove 41 18467 6334\n");
			root = remove_AVL(root, d1);
			print_AVL(root);
			printf("found max\n");
			node_AVL* emax;
			emax = findmax(root);
#if doubleintprecision == 1
			printf("maximum id %lld\n", emax->key.count_neighbour);
#else
			printf("maximum id %d\n", emax->key.count_neighbour);
#endif

			emax = 0;
			//clear_AVL(root);
			//root = 0;
			print_AVL(root);
		}

		system("pause");
	}
	system("pause");
}



// ����� ������ � ������� ������������ � ��� ������.

#endif