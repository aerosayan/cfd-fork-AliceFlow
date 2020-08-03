// ���������������� ������� ������.
// ������������ � �������������� ������������� ������.
#pragma once
#ifndef _Red_Black_BALANCED_TREE_CPP_
#define _Red_Black_BALANCED_TREE_CPP_ 1

// ������ ������ ������ ������ ������.
// ������
// ������-������ ������ c++ ���������� � ���������� yandex.
// http://www.cyberforum.ru/cpp-beginners/thread1009501.html
// 22.06.2018.

class RBtree {
	struct node_st { 
		node_st* p1, * p2; 
		data_BalTree value;
		bool red; 
	
		node_st() {
             p1 = nullptr;  p2 = nullptr; 
		     value = data_BalTree(-1, -1);
		     red = true; 
		}
	}; // ��������� ����
	node_st* tree_root;                 //!< ������
	integer nodes_count; //!< ����� ����� ������

private:
	node_st* NewNode(data_BalTree value);        //!< ��������� ����� �������
	void DelNode(node_st*);             //!< �������� �������
	void Clear(node_st*);               //!< ����(��������) ������ (����������� �����)
	node_st* Rotate21(node_st*);        //!< �������� �����
	node_st* Rotate12(node_st*);        //!< �������� ������
	void BalanceInsert(node_st**);      //!< ������������ �������
	bool BalanceRemove1(node_st**);     //!< ����� ������������ ��������
	bool BalanceRemove2(node_st**);     //!< ������ ������������ ��������
	bool Insert(data_BalTree, node_st**);         //!< ����������� ����� �������
	bool GetMin(node_st**, node_st**);   //!< ����� � ������ ������������ ���� ���������
	bool Remove(node_st**, data_BalTree);         //!< ����������� ����� ��������
public: // ���������� �����
	enum check_code { error_balance, error_struct, ok }; // ��� ������
	void Show();                        //!< ����� ������
										//check_code Check();                 //!< �������� ������
										//bool TreeWalk(bool*, integer);           //!< ����� ������ � ������ �������� � ��������
private: // ���������� �����
	void Show(node_st*, integer, char);       //!< ����� ������, ����������� �����
											  //check_code Check(node_st*, integer, integer&);//!< �������� ������ (����������� �����)
											  //bool TreeWalk(node_st*, bool*, integer);  //!< ����� ������ � ������ �������� � �������� (����������� �����)
public:
	RBtree();
	~RBtree();
	void Clear();           //!< ������ ������              
	bool Find(data_BalTree);         //!< ����� ��������
	void Insert(data_BalTree);       //!< �������� ��������
	void Remove(data_BalTree);       //!< ������� ��������
	void InsertAndModify(data_BalTree, data_BalTree); //!< �������� ���� ��� ��������, �������������� ������������ ������� ���� �� ��� ����.
	integer GetNodesCount();    //!< ������ ����� �����
	data_BalTree GetMaxElm(); //!< ���������� ������������ ������� � ������.
};

//!< ���������� ������������ ������� � ������.
data_BalTree RBtree::GetMaxElm() {
	if (tree_root) {
		node_st* node = tree_root;
		node_st* p2_loc = tree_root->p2;
		if (p2_loc != nullptr) {
			while (p2_loc != nullptr) {
				node = p2_loc;
				p2_loc = p2_loc->p2;
			}
			data_BalTree ir = node->value;
			node = nullptr;
			p2_loc = nullptr;
			return ir;
		}
		else {
			data_BalTree ir = node->value;
			node = nullptr;
			return ir;
		}
	}
	else {
		printf("Red - Black tree is empty...\n");
		//getchar();
		//exit(1);
		data_BalTree ir;
		ir.count_neighbour = -1;
		ir.i = -1;
		return ir;
	}

} // GetMaxElm

// �����������.
RBtree::RBtree()
{
	tree_root = nullptr;
	nodes_count = 0;
}

// ����������.
RBtree::~RBtree()
{
	Clear(tree_root);
}

integer RBtree::GetNodesCount()
{
	return nodes_count;
}

// ��������� ����� �������
RBtree::node_st* RBtree::NewNode(data_BalTree value)
{
	nodes_count++;
	node_st* node = new node_st;
	node->value = value;
	node->p1 = node->p2 = nullptr;
	node->red = true;
	return node;
}

// �������� �������
void RBtree::DelNode(node_st* node)
{
	nodes_count--;
	delete node;
}

// ���� ������ (����������� �����)
void RBtree::Clear(node_st* node)
{
	if (!node) return;
	Clear(node->p1);
	Clear(node->p2);
	DelNode(node);
}

// ����� ������, ����������� �����
//! \param node ����
//! \param depth �������
//! \param dir   ������
//! \code Show(root,0,'*'); \endcode
void RBtree::Show(node_st* node, integer depth, char dir)
{
	integer n;
	if (!node) return;
	for (n = 0; n < depth; n++) putchar(' ');
	printf("%c[%lld %lld] (%s)\n", dir, node->value.i, node->value.count_neighbour, node->red ? "red" : "black");
	Show(node->p1, depth + 2, '-');
	Show(node->p2, depth + 2, '+');
}


// �������� �����
//! \param index ������ �������
//! \result ����� ������� ������
RBtree::node_st* RBtree::Rotate21(node_st* node)
{
	node_st* p2 = node->p2;
	node_st* p21 = p2->p1;
	p2->p1 = node;
	node->p2 = p21;
	return p2;
}

// �������� ������
//! \param index ������ �������
//! \result ����� ������� ������
RBtree::node_st* RBtree::Rotate12(node_st* node)
{
	node_st* p1 = node->p1;
	node_st* p12 = p1->p2;
	p1->p2 = node;
	node->p1 = p12;
	return p1;
}


// ������������ �������
void RBtree::BalanceInsert(node_st** root)
{
	node_st* p1, * p2, * px1, * px2;
	node_st* node = *root;
	if (node->red) return;
	p1 = node->p1;
	p2 = node->p2;
	if ((p1 != nullptr) && p1->red) {
		px2 = p1->p2;             // ������ ����� ��� ����� ������� ������� �������
		if ((px2 != nullptr) && px2->red) p1 = node->p1 = Rotate21(p1);
		px1 = p1->p1;
		if ((px1 != nullptr) && px1->red) {
			node->red = true;
			p1->red = false;
			if ((p2 != nullptr) && p2->red) { // ���������� ����������� ������
				px1->red = true;
				p2->red = false;
				return;
			}
			*root = Rotate12(node);
			return;
		}
	}
	// ���� ����� � ������ �������
	if ((p2 != nullptr) && p2->red) {
		px1 = p2->p1;             // ������ ����� ��� ����� ������� ������� �������
		if ((px1 != nullptr) && px1->red) p2 = node->p2 = Rotate12(p2);
		px2 = p2->p2;
		if ((px2 != nullptr) && px2->red) {
			node->red = true;
			p2->red = false;
			if ((p1 != nullptr) && p1->red) { // ���������� ����������� ������
				px2->red = true;
				p1->red = false;
				return;
			}
			*root = Rotate21(node);
			return;
		}
	}
}


bool RBtree::BalanceRemove1(node_st** root)
{
	node_st* node = *root;
	node_st* p1 = node->p1;
	node_st* p2 = node->p2;
	if ((p1 != nullptr) && (p1->red)) {
		p1->red = false; return false;
	}
	if ((p2 != nullptr) && (p2->red)) { // ������ 1
		node->red = true;
		p2->red = false;
		node = *root = Rotate21(node);
		if (BalanceRemove1(&node->p1)) node->p1->red = false;
		return false;
	}
	unsigned int mask = 0;
	node_st* p21 = nullptr;
	node_st* p22 = nullptr;
	if (p2 != nullptr) {
		p21 = p2->p1;
		p22 = p2->p2;
	}

	if ((p21 != nullptr) && (p21->red)) mask |= 1;
	if ((p22 != nullptr) && (p22->red)) mask |= 2;
	switch (mask)
	{
	case 0:     // ������ 2 - if((!p21 || !p21->red) && (!p22 || !p22->red))
		p2->red = true;
		return true;
	case 1:
	case 3:     // ������ 3 - if(p21 && p21->red)
		p2->red = true;
		p21->red = false;
		p2 = node->p2 = Rotate12(p2);
		p22 = p2->p2;
	case 2:     // ������ 4 - if(p22 && p22->red)
		p2->red = node->red;
		p22->red = node->red = false;
		*root = Rotate21(node);
	}
	return false;
}

bool RBtree::BalanceRemove2(node_st** root)
{
	node_st* node = *root;
	node_st* p1 = node->p1;
	node_st* p2 = node->p2;
	if ((p2 != nullptr) && p2->red) { p2->red = false; return false; }
	if ((p1 != nullptr) && p1->red) { // ������ 1
		node->red = true;
		p1->red = false;
		node = *root = Rotate12(node);
		if (BalanceRemove2(&node->p2)) node->p2->red = false;
		return false;
	}
	unsigned int mask = 0;
	node_st* p11 = nullptr;
	node_st* p12 = nullptr;

	if (p1 != nullptr) {
		p11 = p1->p1;
		p12 = p1->p2;
	}

	if ((p11 != nullptr) && p11->red) mask |= 1;
	if ((p12 != nullptr) && p12->red) mask |= 2;
	switch (mask) {
	case 0:     // ������ 2 - if((!p12 || !p12->red) && (!p11 || !p11->red))
		p1->red = true;
		return true;
	case 2:
	case 3:     // ������ 3 - if(p12 && p12->red)
		p1->red = true;
		p12->red = false;
		p1 = node->p1 = Rotate21(p1);
		p11 = p1->p1;
	case 1:     // ������ 4 - if(p11 && p11->red)
		p1->red = node->red;
		p11->red = node->red = false;
		*root = Rotate12(node);
	}
	return false;
}


bool RBtree::Find(data_BalTree value)
{
	node_st* node = tree_root;
	while (node != nullptr) {
		if (node->value == value) return true;
		//node = node->value>value ? node->p1: node->p2;
		if (value < node->value)
			node = node->p1;
		else if (value > node->value)
			node = node->p2;
	}
	return false;
}


// ����������� ����� �������
//! \result true ���� ��������� ������ ��� ������������ � ������ ������� �� �����
bool RBtree::Insert(data_BalTree value, node_st** root)
{
	node_st* node = *root;
	if (node == nullptr) *root = NewNode(value);
	else {
		if (node->value == value)  return true;
		//if (Insert(value, value<node->value ? &node->p1: &node->p2)) return true; 
		if (value < node->value) {
			if (Insert(value, &node->p1))  return true;
		}
		else if (value > node->value) {
			if (Insert(value, &node->p2))  return true;
		}

		BalanceInsert(root);
	}

	return false;
}


// ����� � ������ ������������ ���� ���������
//! \param root ������ ������ � ������� ���� ����� �������
//! \retval res ������� ������� ��� �����
//! \result true ���� ����� ������
bool RBtree::GetMin(node_st** root, node_st** res)
{
	node_st* node = *root;
	if (node->p1 != nullptr) {
		if (GetMin(&node->p1, res)) return BalanceRemove1(root);
	}
	else {
		*root = node->p2;
		*res = node;
		return !node->red;
	}
	return false;
}


// ����������� ����� ��������
//! \result true ���� ����� ������
bool RBtree::Remove(node_st** root, data_BalTree value)
{
	node_st* t, * node = *root;
	if (node == nullptr) return false;
	if (node->value < value) {
		if (Remove(&node->p2, value)) return BalanceRemove2(root);
	}
	else if (node->value > value) {
		if (Remove(&node->p1, value)) return BalanceRemove1(root);
	}
	else {
		bool res;
		if (node->p2 == nullptr) {
			*root = node->p1;
			res = !node->red;
		}
		else {
			res = GetMin(&node->p2, root);
			t = *root;
			t->red = node->red;
			t->p1 = node->p1;
			t->p2 = node->p2;
			if (res) res = BalanceRemove2(root);
		}
		DelNode(node);
		return res;
	}
	return 0;
}


// ����� ������
void RBtree::Show()
{
	printf("[tree]\n");
	Show(tree_root, 0, '*');
}

// ������� �������
void RBtree::Insert(data_BalTree value)
{
	Insert(value, &tree_root);
	if (tree_root) tree_root->red = false;
}

// �������� ����
void RBtree::Remove(data_BalTree value)
{
	Remove(&tree_root, value);
}

// ���� ������
void RBtree::Clear()
{
	Clear(tree_root);
	tree_root = 0;
}

//!< �������� ���� ��� ��������, �������������� ������������ ������� ���� �� ��� ����.
void RBtree::InsertAndModify(data_BalTree value, data_BalTree ksearch) {

	if (Find(ksearch)) {
		// ������ ��� ����������
		Remove(ksearch);
		// ������ ksearch �� value.
		Insert(value);
	}
	else {
		// ������� �� ����������
		Insert(value);
	}
}

/*
// �������� ������ (����������� �����)
//! \param tree ������
//! \param d    ������� ������ �������
//! \param h    ��������� ������ �������
//! \result 0 ��� ��� ������
RBtree::check_code RBtree::Check(node_st *tree, integer d, integer &h)
{
if (!tree) {
// ���������� ������ ������ �� ����� ���� ����������
if (h<0) h = d;
return h == d ? ok: error_balance;
}
node_st *p1 = tree->p1;
node_st *p2 = tree->p2;
// ������� ������� ������ ����� ������ ��������
if (tree->red && (p1 && p1->red || p2 && p2->red)) return error_struct;
if (p1 && tree->value<p1->value || p2 && tree->value>p2->value) return error_struct;
if (!tree->red) d++;
check_code n = Check(p1, d, h); if (n) return n;
return Check(p2, d, h);
}


// �������� ������
RBtree::check_code RBtree::Check()
{
integer d = 0;
integer h = -1;
if (!tree_root) return ok;
if (tree_root->red) return error_struct;
return Check(tree_root, d, h);
}

// ����� ������ � ������ �������� � �������� (����������� �����)
//! \param node  ������ ������
//! \param array ������ ��� ������
//! \param size  ������ �������
bool RBtree::TreeWalk(node_st *node, bool *array, integer size)
{
if (!node) return false;
integer value = node->value;
if (value<0 || value >= size || !array[value]) return true;
array[value] = false;
return TreeWalk(node->p1, array, size) || TreeWalk(node->p2, array, size);
}

// ����� ������ � ������ �������� � ��������
//! \param array ������ ��� ������
//! \param size  ������ �������
bool RBtree::TreeWalk(bool *array, integer size)
{
if (TreeWalk(tree_root, array, size)) return true;
for (integer n = 0; n<size; n++) if (array[n]) return true;
return false;
}
*/

//================================================================


void test_Red_Black_Tree() {
	RBtree root;
	data_BalTree d3;
	d3.count_neighbour = rand();
	d3.i = rand();
	root.Insert(d3);
	//root.Show();
	d3.count_neighbour = rand();
	d3.i = rand();
	root.Insert(d3);
	//root.Show();
	d3.count_neighbour = rand();
	d3.i = rand();
	root.Insert(d3);
	d3.count_neighbour = rand();
	d3.i = rand();
	root.Insert(d3);
	root.Show();
	d3 = (root.GetMaxElm());
	printf("i==%lld count_neighbour==%lld\n", d3.i, d3.count_neighbour);
	d3.count_neighbour = rand();
	d3.i = rand();
	root.Insert(d3);
	root.Show();
	d3.i = rand();
	root.Insert(d3);
	d3.i = rand();
	root.Insert(d3);
	d3.i = rand();
	root.Insert(d3);
	d3.i = rand();
	d3 = (root.GetMaxElm());
	printf("i==%lld count_neighbour==%lld\n", d3.i, d3.count_neighbour);
	root.Remove(d3);

	d3 = (root.GetMaxElm());
	printf("i==%lld count_neighbour==%lld\n", d3.i, d3.count_neighbour);
	root.Show();
	system("pause");
}

// ������ ������ ������ ������ �����.

#endif