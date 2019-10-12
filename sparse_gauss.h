// ���� sparse_gauss.h 
// ���������� ������ ������ ��� ����������� �������.

#pragma once
#ifndef SPARSE_GAUSS_H
#define SPARSE_GAUSS_H 1

// ���������� ������ ������������ ���������� ���������� � ����� ������ ��������� � ������ ������ 
// AliceFlow_v0_27
//#define doublereal double

// ��� �������
typedef struct tagTERM {
	integer key;
	doublereal val;
	// ����������� ���� ���
	// ��� ������, ��� ����������
	// ���������� ����� � �������
	// �� ������� ��� �� ������������.
	// left � right ������ 10,10,2019
	//struct tagTERM *left=NULL;
	//struct tagTERM *right=NULL;
	integer bal;
} TERM;

// ������ ��� ������� � ������� ����
typedef struct tagIRow{
    doublereal eps0; // ��� ����������� ������������� ����

	// ��������� ��� ���� ������������
	// ������ ��� ���������� ���������� 
	// ����� � �������� �� ������������ �������.
	integer POOL_SIZE; // ������ ������� ������� ������� ��������
	integer n; // ����� ��������� ���������

	TERM *elm=NULL;

} IRow;

// ����������� ������� ����:
// ���������� nxn � ������������ �������������,
// �������� ��������������. ��������� ���������� 
// � ����.
typedef struct tagIMatrix{
    doublereal eps0; // ��� ����������� ������������� ����

	integer n; // ����������� ������� nxn.
    // jp - ������ ������� �����������
    // jm - ������� ������ �����������
    // dd - ������� ���������
	IRow *jp=NULL, *jm=NULL;
	doublereal *dd;
} IMatrix;

// ����� ������ � �������� key  
integer search_i(TERM* list, integer n, integer key);

// ������� ������� � ������ ������ key
void deleteAt(TERM* &list, integer key, integer &n, integer &pool);

// �������� ������� �� ���������� : num, val.
void add(TERM* &list, integer &n, integer &pool, integer num, doublereal val);

// ���������� �������� ������ ����
// ������� ����� key
doublereal get_val(TERM* list, integer n, integer key);

// ��������� ����� value � ������
// � ������ key
void modify_add(TERM* &list, integer n, integer key, doublereal value);

// ������������� ����� value � ������
// � ������ key
void modify_set(TERM* &list, integer n, integer key, doublereal value);

// ������� �� ����������� �������������
// ���������� ��� ��������� ��������
void get_values(TERM *list, integer n, integer* &indexes, doublereal* &values);

// ���������� 0 ��� i<=0, i ��� i>0
integer sigma(integer i);

// �������������� row,col � ���������� ���������� (d,j)
// ��� ������� �����������: d=0..(n-1), j=-(n-d-1)..-1
// ��� ������ �����������:  d=0..(n-1), j=1..(n-d-1)
integer getD(integer row, integer col);
integer getJ(integer row, integer col);

// �������� �������������� ��������� ���������� (d,j) � row,col
integer getRow(integer d, integer j);
integer getCol(integer d, integer j);


// ���������� �������� value � ������ � �������� num
void setValueIRow(IRow *xO, integer num, doublereal value);

// ��������� value � ������������� �������� � ������ num
void addValueIRow(IRow *xO, integer num, double value);

// ���������� �������� ������ num
doublereal getValueIRow(IRow *xO, integer num);

// ���������� ��� ��������� ������ ������/�������: 
// ������� ����� - � indexes, �������� � values
integer getValuesIRow(IRow *xO, integer* &indexes, doublereal* &values);

// ��������� ������ ��� ����������� �������
void initIMatrix(IMatrix *xO, integer n);

// ������������ ������ �� ��� �������
void freeIMatrix(IMatrix* xO);

// ������������� �������� value � ������ � ������������ [row,col];
// row - ����� ������ �������
// col - ����� ������� �������
void setValueIMatrix(IMatrix *xO, integer row, integer col, doublereal value);

// ��������� �������� value � ������ [row,col]
void addValueIMatrix(IMatrix *xO, integer row, integer col, double value);

// ���������� �������� ������ [row,col]
doublereal  getValueIMatrix(IMatrix *xO, integer  row, integer  col);

// ���������� ��������� �������� � ������� ����� ������ d,
// ������� ��������� ������ ������� ���������
integer  getJRowIMatrix(IMatrix *xO, integer  d, integer* &indexes, doublereal* &values);

// ���������� ��������� �������� � ������� ����� ������� d, 
// ������� ��������� ���� ������� ���������
integer  getJColIMatrix(IMatrix *xO, integer  d, integer* &indexes, doublereal* &values);

// ������� �����, ������������ ������� x,
// ��������� ������ ��������� ������ b � 
// ���������� ������� xO � ����������� ����������� �������.
void calculateSPARSEgaussArray(IMatrix *xO, doublereal *x, doublereal *b);

// ����������� ������� � ������� IMatrix � CSIR ������ 
// ����������� � ����������� ITL ��� ���������� ILU ����������.
void convertIMatrixtoCSIR_ILU_ITL(IMatrix *xO, doublereal* &U_val, integer* &U_ind, integer* &U_ptr, doublereal* &L_val, integer* &L_ind, integer* &L_ptr);

#endif // !SPARSE_GAUSS_H