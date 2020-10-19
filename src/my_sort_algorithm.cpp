// ��������� ����������
// ������������ � �������������� ������������� ������.
#pragma once
#ifndef _MY_SORT_ALGORITHM_CPP_
#define _MY_SORT_ALGORITHM_CPP_ 1

// https://github.com/timsort/cpp-TimSort
#include "gfx/timsort.hpp" // ���������� ���� ��������� 2002.

// MY_SORT_ALGORITHM::QUICK_SORT // ������� ���������� �. �����.
// ������������ �� quicksort qs and qsj.
// ���������� � ��������� ������� quickSort.
// ������������ �� ���������� ��������� ������� 
//���������� ��������� ������ (������ ��� ����� � ������� ���� ����������).
// MY_SORT_ALGORITHM::COUNTING_SORT // ���������� � ��������� ������ �����.
// ���������� � ��������� �������� ������ ��� ����� ������������ � 
// ����� � �������� ��������� ����������.
// MY_SORT_ALGORITHM::HEAP_SORT // ������������� ����������.
// ���������� ����������� ������� ����������, ������� QuickSort �� ��������.
// � ����������� ���� ��������� ������ ����� �� 4��.
// ���������� ���� ���������. 15-16.03.2019
// MY_SORT_ALGORITHM::TIM_SORT

// ��� std::sort 
bool compareAk1R(Ak1 i1, Ak1 i2)
{
	return (i1.i < i2.i);
}

// ��� std::sort  
bool compareAk1P(Ak1 i1, Ak1 i2)
{
	return (i1.j < i2.j);
}

// ��� ��������� ������� ���� ��������� � ������ ����������
// �� ������������ �������� ������������������ ���������:
// ����������. ����� ����� ����������� ������� ����������.
// ������ �������� � ����� ����� "The C programming language".
// swap: ����� ������� v[i] � v[j]
template <typename doublerealT>
void swapnd(integer*& v, doublerealT*& dkey, integer i, integer j)
{
	integer temp;

	// change v[i] <-> v[j]
	temp = v[i];
	v[i] = v[j];
	v[j] = temp;

	doublerealT dtemp;

	// change v[i] <-> v[j]
	dtemp = dkey[i];
	dkey[i] = dkey[j];
	dkey[j] = dtemp;
} // swap

  // ��� �������� PivotList
template <typename doublerealT>
integer PivotListnd(integer*& list, doublerealT*& dkey, integer first, integer last) {
	// list �������������� ������
	// first ����� ������� ��������
	// last ����� ���������� ��������

	doublerealT PivotValue = dkey[first];
	integer PivotPoint = first;

	for (integer index = (first + 1); index <= last; index++) {
		if (dkey[index] < PivotValue) {
			PivotPoint++;
			swapnd<doublerealT>(list, dkey, PivotPoint, index);
		}
	}

	swapnd<doublerealT>(list, dkey, first, PivotPoint);

	return PivotPoint;
} // PivotList


  // ������� ���������� �����.
  // ����������������� � �������������� ��. ��������� ������ ����������
  // ���. 106.
template <typename doublerealT>
void QuickSortnd(integer*& list, doublerealT*& dkey, integer first, integer last) {
	// list ��������������� ������ ���������
	// first ����� ������� �������� � ����������� ����� ������
	// last ����� ���������� �������� � ����������� ����� ������

	integer pivot;

	if (first < last) {
		pivot = PivotListnd<doublerealT>(list, dkey, first, last);
		QuickSortnd<doublerealT>(list, dkey, first, pivot - 1);
		QuickSortnd<doublerealT>(list, dkey, pivot + 1, last);
	}
} // QuickSort

// ��� ������������ ������� ���������� �������������� �������.
integer intcompare(const void* i, const void* j)
{
	if (*(integer*)i > * (integer*)j) return (1);
	if (*(integer*)i < *(integer*)j) return (-1);
	return (0);
} // ��� ������������ ������� ������� ����������.

// ��� ��������� ������� ���� ��������� � ������ ����������
// �� ������������ �������� ������������������ ���������:
// ����������. ����� ����� ����������� ������� ����������.
// ������ �������� � ����� ����� "The C programming language".
// swap: ����� ������� Amat[i] � Amat[j]
/*
// ������ ��� ��������� � ����� uniformsimplemeshgen.cpp
template <typename TMatrixElm>
void swap(TMatrixElm*& Amat, integer i, integer j)
{
	TMatrixElm A_temp;

	// change Amat[i] <-> Amat[j]
	A_temp = Amat[i];
	Amat[i] = Amat[j];
	Amat[j] = A_temp;

} // swap
*/
/*
void swap(Ak1*& Amat, integer i, integer j)
{
	Ak1 A_temp;

	// change Amat[i] <-> Amat[j]
	A_temp = Amat[i];
	Amat[i] = Amat[j];
	Amat[j] = A_temp;

} // swap
*/

  /*
  // ��� �������� PivotList
  integer PivotList(Ak* &Amat, integer n, integer first, integer last) {
  // list==jptr and altr �������������� ������
  // first ����� ������� ��������
  // last ����� ���������� ��������

  integer PivotValue = Amat[first].i*n+Amat[first].j;
  integer PivotPointeger = first;

  for (integer index = (first + 1); index <= last; index++) {
  if (Amat[index].i*n+Amat[index].j<PivotValue) {
  PivotPoint++;
  swap(Amat, PivotPoint, index);
  }
  }

  swap(Amat, first, PivotPoint);

  return PivotPoint;
  } // PivotListamg
  */

template <typename TMatrixElm>
bool comparei(TMatrixElm& Amat1, TMatrixElm& Amat2) {
	return ((Amat1.i<Amat2.i)||((Amat1.i == Amat2.i)&&(Amat1.j < Amat2.j)));
}

template <typename TMatrixElm>
bool comparej(TMatrixElm& Amat1, TMatrixElm& Amat2) {
	return ((Amat1.j < Amat2.j) || ((Amat1.j == Amat2.j) && (Amat1.i < Amat2.i)));
}

  // PivotList �������� ������.���������� 12 ������� 2015.
  // ��� �������� PivotList
template <typename TMatrixElm>
integer PivotList(TMatrixElm*& Amat, integer first, integer last,
	bool (*compare)(TMatrixElm& Amat1, TMatrixElm& Amat2)) {
	// list==jptr and altr �������������� ������
	// first ����� ������� ��������
	// last ����� ���������� ��������

	TMatrixElm Afirst = Amat[first];
	integer PivotPoint = first;

	for (integer index = (first + 1); index <= last; index++) {
		if (compare(Amat[index], Afirst)) {
			PivotPoint++;
			swap(Amat, PivotPoint, index);
		}
	}

	swap(Amat, first, PivotPoint);

	return PivotPoint;
} // PivotList


 // ������������� ����������
  // ��������������� ��������
template <typename TMatrixElm>
void FixHeap(TMatrixElm*& Amat,
	integer root,
	TMatrixElm m,
	integer bound,
	integer iadd,
	bool (*compare)(TMatrixElm& Amat1, TMatrixElm& Amat2))
{
	integer vacant;
	integer largerChild;

	// list ����������� ������ ��������
	// root ����� ����� ��������
	// m �������� �������� ����������� � ��������
	// bound ������ ������� (�����) � ��������
	vacant = root;
	while (2 * vacant <= bound)
	{
		largerChild = 2 * vacant;
		integer lCadd = largerChild + iadd;
		integer lCadd1 = lCadd + 1;

		// ����� ����������� �� ���� ���������������� ��������
		bool compare_result = compare(Amat[lCadd], Amat[lCadd1]);
		
		if ((largerChild < bound) && compare_result)
		{
			largerChild = largerChild + 1;
		}

		lCadd = largerChild + iadd;
		// ��������� �� ���� ���� �������� ������� ?
		compare_result = compare(Amat[lCadd], m);
	
		if (compare_result)
		{
			// ��, ���� �����������
			break;
		}
		else
		{
			// ���, �������� ����������������� �������
			// ������� �������
			Amat[vacant + iadd] = Amat[lCadd];
			vacant = largerChild;
		}
	}
	Amat[vacant + iadd] = m;
} // FixHeap


  // HeapSort ����� ������� ������ �� ������������ �� 10^5, � ����� ������ �� ������������ 10^8 ��� ����� ����������� ���� � ���. 

  // ������������� ���������� ���������� ���
  // �� ������, ��� � �� ��������������, � ���� �� � ��������
  // ����� ���������.
  // ����������� ������� � ���, ��� ��������� ������� ������ ���������� � 1.
template <typename TMatrixElm>
void HeapSort(TMatrixElm* &Amat, integer first, integer last, bool (*(compare))(Ak1& Amat1, Ak1& Amat2))
{

	TMatrixElm maxelm; // ������� � ���������� ��������� �����

			   // ��������������� ��������
	for (integer i = ((last - first + 1) / 2); i >= 1; i--)
	{
		FixHeap(Amat, i, Amat[i + first - 1], last - first + 1, first - 1, compare);
	}
	for (integer i = last - first + 1; i >= 2; i--)
	{
		// ����������� ������ �������� � ������
		// ��������������� ��������
		maxelm = Amat[first];
		FixHeap(Amat, 1, Amat[i + first - 1], i - 1, first - 1, compare);
		Amat[i + first - 1] = maxelm;
	}
} // HeapSort

 
  // ���������� ��������. 
  // ������� �������������� ������.
template <typename TMatrixElm>
void MergeSort(TMatrixElm*& Aorig, integer size, bool (*compare)(TMatrixElm& Amat1, TMatrixElm& Amat2)) {
	// �������������� ���������� �� ���� �� size-1.
	// ������ � �������������� �� ����� �������� �������.
	// ������� ������ ��� ���������� ������� ���������.
	TMatrixElm* Amat = Aorig;
	TMatrixElm* Bm = Amat + size;
	TMatrixElm* C;

	// Amat - ����������� ������, B - ��������������� ������ (������ �� �������� ������ � � ������-�� ������� ��� � �������� ������ � �).
	// C - ��������� ��� ������.
	for (integer i = 1; i < size; i = i * 2) // ������ ������������ ����������
	{
		for (integer j = 0; j < size; j = j + 2 * i) // ������ ������� �� ������������ 
												 // ����������
		{
			integer r = j + i; // ������ ������� �� ������������ ����������
			integer n1 = 0, n2 = 0;
			if (i < size - j) { n1 = i; }
			else { n1 = size - j; };
			if (i < size - r) { n2 = i; }
			else { n2 = size - r; };

			if (n1 < 0) n1 = 0;
			if (n2 < 0) n2 = 0;

			// ������� ������������� ����������
			for (integer ia = 0, ib = 0, k = 0; k < n1 + n2; k++)
			{
				if (ia >= n1) Bm[j + k] = Amat[r + ib++];
				else
					if (ib >= n2) Bm[j + k] = Amat[j + ia++];
					else {
						integer lCadd = j + ia;
						integer lCadd1 = r + ib;
						bool compare_result = compare(Amat[lCadd], Amat[lCadd1]);
						
						if (compare_result) {
							Bm[j + k] = Amat[j + ia++];
						}
						else {
							Bm[j + k] = Amat[r + ib++];
						}
					}
			}



		}
		C = Amat; Amat = Bm; Bm = C;

	}

	C = Amat; Amat = Bm; Bm = C;

	// �����������, ���� ��������� �������� �� � �������� � � ��������������� �������
	if (Bm != Aorig)
		memcpy(Aorig, Bm, size * sizeof(Ak));

	Amat = nullptr; Bm = nullptr; C = nullptr;
} // MergeSort

template <typename TMatrixElm>
integer indx_comparei(TMatrixElm& Amat) {
	return Amat.i;
}

template <typename TMatrixElm>
integer indx_comparej(TMatrixElm& Amat) {
	return Amat.j;
}


  // ���������� ���������.
  // �� ����� O(n)
  // ����� ������ ���. 224.
  // ��������: �������� ���������� ����� ����� ����������� ������.
  // ������� ���������� �������� (H.H.Seward) � 1954 ����.
void Counting_Sort(Ak*& Amat, integer first, integer last,
	integer (*indx_compare)(Ak& Amat))
{
	// ����� �� malloc � calloc 7 ������ 2016.

	integer k = -1;
	for (integer j = first; j <= last; j++) {
		if (indx_compare(Amat[j]) > k) k = indx_compare(Amat[j]);
	}
	//integer* C = new integer[k + 1];
	integer* C = (integer*)malloc((k + 1) * sizeof(integer));
	char c1[2] = "C";
	char c2[14] = "Counting_Sort";
	handle_error<integer>(C, c1, c2, (k + 1));


	for (integer i = 0; i <= k; i++) {
		C[i] = 0; // �������������.
	}
	for (integer j = first; j <= last; j++) {
		C[indx_compare(Amat[j])]++;
	}
	// � C[i] �������� ���������� ��������� ������ i.
	for (integer i = 1; i <= k; i++) {
		C[i] += C[i - 1];
	}
	// � C[i] ���������� ��������� �� ����������� i
	//Ak* Bm = new Ak[last - first + 2];
	Ak* Bm = (Ak*)malloc((last - first + 2) * sizeof(Ak));
	char c3[3] = "Bm";
	char c4[14] = "Counting_Sort";
	handle_error<Ak>(Bm, c3, c4, (last - first + 2));

	integer ind;
	for (integer j = last; j >= first; j--) {
		ind = indx_compare(Amat[j]);
		Bm[C[ind]] = Amat[j];
		C[ind]--;
	}
	// �������� �����������.
	for (integer j = first, i = 1; j <= last; j++, i++) {
		//Amat[j] = B[j - first + 1];
		Amat[j] = Bm[i];
	}
	//delete[] Bm;
	free(Bm);
	//delete[] C;
	free(C);

}


// �� ����������� ������ �� ��� �������� ������� ����,
// � ����� � ���������������. ��� �������������� ������� ���� ���� 
// ��������� �������������� ������� ���������� ��������� �� ���������� ����������.
integer* the_original_order_of_values = nullptr;
integer* the_original_order_of_values_reverse = nullptr;


// ���������� ���������.
// �� ����� O(n)
// ����� ������ ���. 224.
// ��������: �������� ���������� ����� ����� ����������� ������.
// ������� ���������� �������� (H.H.Seward) � 1954 ����.
void Counting_Sort(Ak1*& Amat, integer first, integer last, bool bmemo, integer (*indx_compare)(Ak1& Amat))
{
	// ����� �� malloc � calloc 7 ������ 2016.
	//���� bmemo==true �� ���������� �������������� ������� ��������.
	integer* the_original_order_of_values_buf = nullptr;

	integer k = -1;
	for (integer j = first; j <= last; j++) {
		if (indx_compare(Amat[j]) > k) k = indx_compare(Amat[j]);
	}
	//integer* C = new integer[k + 1];
	integer* C = (integer*)malloc((k + 1) * sizeof(integer));
	char c1[2] = "C";
	char c2[14] = "Counting_Sort";
	handle_error<integer>(C, c1, c2, (k + 1));

	the_original_order_of_values_buf = (integer*)malloc((last + 1) * sizeof(integer));
	char c7[34] = "the_original_order_of_values_buf";
	char c6[14] = "Counting_Sort";
	handle_error<integer>(the_original_order_of_values_buf, c7, c6, (last + 1));

	if (bmemo) {
		if (the_original_order_of_values == nullptr) {
			the_original_order_of_values = (integer*)malloc((last + 1) * sizeof(integer));
			char c5[29] = "the_original_order_of_values";

			handle_error<integer>(the_original_order_of_values, c5, c6, (last + 1));
		}
		else {
			free(the_original_order_of_values);
			the_original_order_of_values = nullptr;

			the_original_order_of_values = (integer*)malloc((last + 1) * sizeof(integer));
			char c5[29] = "the_original_order_of_values";

			handle_error<integer>(the_original_order_of_values, c5, c6, (last + 1));
		}

		if (the_original_order_of_values_reverse == nullptr) {
			the_original_order_of_values_reverse = (integer*)malloc((last + 1) * sizeof(integer));
			char c8[38] = "the_original_order_of_values_reverse";
			handle_error<integer>(the_original_order_of_values_reverse, c8, c6, (last + 1));
		}
		else {
			free(the_original_order_of_values_reverse);
			the_original_order_of_values_reverse = nullptr;

			the_original_order_of_values_reverse = (integer*)malloc((last + 1) * sizeof(integer));
			char c8[38] = "the_original_order_of_values_reverse";
			handle_error<integer>(the_original_order_of_values_reverse, c8, c6, (last + 1));

		}

	}

#pragma omp parallel for
	for (integer i = 0; i <= k; i++) {
		C[i] = 0; // �������������.
	}
	for (integer j = first; j <= last; j++) {
		C[indx_compare(Amat[j])]++;
	}
	// � C[i] �������� ���������� ��������� ������ i.
//�� � ���� ������ !!! #pragma omp parallel for
	for (integer i = 1; i <= k; i++) {
		C[i] += C[i - 1];
	}
	// � C[i] ���������� ��������� �� ����������� i
	//Ak1* Bm = new Ak1[last - first + 2];
	Ak1* Bm = (Ak1*)malloc((last - first + 2) * sizeof(Ak1));
	char c3[3] = "Bm";
	char c4[14] = "Counting_Sort";
	handle_error<Ak1>(Bm, c3, c4, (last - first + 2));

	integer ind;
	for (integer j = last; j >= first; j--) {
		ind = indx_compare(Amat[j]);
		Bm[C[ind]] = Amat[j];
		if (bmemo) {
			// j ���� C[ind]
			if (the_original_order_of_values_buf != nullptr) {
				the_original_order_of_values_buf[C[ind]] = j;
			}
		}
		C[ind]--;
	}
	// �������� �����������.

	for (integer jnew = first, i = 1; jnew <= last; jnew++, i++) {
		//Amat[jnew] = B[jnew - first + 1];
		// i ���� jnew. i ������������� � C[ind].
		Amat[jnew] = Bm[i];
		if (bmemo) {
			if (the_original_order_of_values_buf != nullptr) {
				if (the_original_order_of_values != nullptr) {
					the_original_order_of_values[the_original_order_of_values_buf[i]] = jnew;
					the_original_order_of_values_reverse[jnew] = the_original_order_of_values_buf[i];
				}
			}
		}
	}
	//delete[] Bm;
	if (Bm != nullptr) {
		free(Bm);
		Bm = nullptr;
	}
	//delete[] C;
	if (C != nullptr) {
		free(C);
		C = nullptr;
	}

	if (the_original_order_of_values_buf != nullptr) {
		free(the_original_order_of_values_buf);
		the_original_order_of_values_buf = nullptr;
	}
}


// ���������� ���������.
// �� ����� O(n)
// ����� ������ ���. 224.
// ��������: �������� ���������� ����� ����� ����������� ������.
// ������� ���������� �������� (H.H.Seward) � 1954 ����.
void Counting_Sort(Ak1*& Amat, integer first, integer last, integer (*indx_compare)(Ak1& Amat))
{

	integer k = -1;
	for (integer j = first; j <= last; j++) {
		if (indx_compare(Amat[j]) > k) k = indx_compare(Amat[j]);
	}
	//integer* C = new integer[k + 1];
	integer* C = (integer*)malloc((k + 1) * sizeof(integer));
	char c1[2] = "C";
	char c2[14] = "Counting_Sort";
	handle_error<integer>(C, c1, c2, (k + 1));

#pragma omp parallel for
	for (integer i = 0; i <= k; i++) {
		C[i] = 0; // �������������.
	}
	for (integer j = first; j <= last; j++) {
		C[indx_compare(Amat[j])]++;
	}
	// � C[i] �������� ���������� ��������� ������ i.
	for (integer i = 1; i <= k; i++) {
		C[i] += C[i - 1];
	}
	// � C[i] ���������� ��������� �� ����������� i
	//Ak1* Bm = new Ak1[last - first + 2];
	Ak1* Bm = (Ak1*)malloc((last - first + 2) * sizeof(Ak1));
	char c3[3] = "Bm";
	char c4[14] = "Counting_Sort";
	handle_error<Ak1>(Bm, c3, c4, (last - first + 2));

	integer ind;
	for (integer j = last; j >= first; j--) {
		ind = indx_compare(Amat[j]);
		Bm[C[ind]] = Amat[j];
		C[ind]--;
	}
	// �������� �����������.
	for (integer j = first, i = 1; j <= last; j++, i++) {
		//Amat[j] = B[j - first + 1];
		Amat[j] = Bm[i];
	}
	//delete[] Bm;
	free(Bm);
	//delete[] C;
	free(C);

}

integer compAi(Ak1 a, Ak1 b) {
	if (a.i > b.i) return (1);
	if (a.i < b.i) return (-1);
	return (0);
}

integer compAj(Ak1 a, Ak1 b) {
	if (a.j > b.j) return (1);
	if (a.j < b.j) return (-1);
	return (0);
}

/*
// ��� ������������ std::sort
integer compAi(const void  * a, const void  * b)
{
	if ((*(Ak1*)a).i > (*(Ak1*)b).i) return (1);
	if ((*(Ak1*)a).i < (*(Ak1*)b).i) return (-1);
	return (0);
} // compAi

// ��� ������������ std::sort
integer compAj(const void  * a, const void  * b)
{
	if ((*(Ak1*)a).j > (*(Ak1*)b).j) return (1);
	if ((*(Ak1*)a).j < (*(Ak1*)b).j) return (-1);
	return (0);
} // compAj
*/

// �������� ����� �������� ��������� ��������.
// TODO ��������� ���������� ����� �� ���� ������� ���������.
integer qs_abbys_heigh = 0; // ������� ������ � ������� ����������.

// ���������� ������ ���������� ������� ����� ������� ��� � 5 ������� ���,
// ������������� ����������. �� ��� ������� ������� ���� TimSort (Futures).
template <typename TMatrixElm>
void qs(TMatrixElm*& Amat, integer first, integer last, integer (*indx_compare)(Ak1& Amat)) {
	integer i = first, j = last;
	TMatrixElm tmp;

	qs_abbys_heigh++;

	/*
	� ������ ����� ��������, ��� � ��������� ����, � ����� ����������� �� ������ ������� �����������, �� � ��� ���������� �������� ����������, ����� ��� ��������� ����������. ���� ����������� ���� ����������, ��� ������ ����� ��������� � ��������� ���.
	��� �� ����� ������ ����� ����� �������� ������ - ��� �����. ������ � �������� �������� ������������� ����� ������� �� ����, � ���� ������ ���������� ����� - �� �� ������ ������������ ���������.
	����� ������� ������������������ ����� ����� ��� ���������. ��������, �� ���������� ���������, ����� ������� ������� ���������� ������ ��� ���������. ��� ������� QuickSort ���������� � ������ "��������" ? ����� ������ - �������� � �������� �������� ��������� ������� �������� �������. ����� ����� ���������� �������������� �� ������� ������ ����� ��������������. ������ ������� - ����������� ����� ����������� �������� ������� ��������� �������.
	������� ���������� ����� ������������ � ��� ���������� �������. ������������ �������� ��� ���� - ���������� ����������������� ������� � ���������� ��������. ��� ��� � �������� �������� ���������� �������� ������ �������, � ���� ��������� �� ������� �������� ������, ���� ��������� ������� ����������� �������� ����� �����������.
	*/
	integer pivot;
	if (1) {
		if (last - first < 3000) {
			pivot = indx_compare(Amat[(integer)((first + last) / 2)]);
		}
		else if (last - first < 800000) {
			pivot = (integer)((indx_compare(Amat[first + 100]) +
				indx_compare(Amat[(first + last) / 2]) +
					indx_compare(Amat[last - 100])) / 3.0);
		}
		else {
			//pivot = 0;
			pivot = (integer)((indx_compare(Amat[first]) +
				indx_compare(Amat[first + 100000]) +
				indx_compare(Amat[first + 200000]) +
				indx_compare(Amat[first + 300000]) +
				indx_compare(Amat[first + 400000]) +
				indx_compare(Amat[first + 500000]) +
				indx_compare(Amat[first + 600000]) +
				indx_compare(Amat[first + 700000]) +
				indx_compare(Amat[last])) / 9.0);
		}

	}
	else {
		pivot = indx_compare(Amat[(first + last) / 2]);
	}

	// partition
	while (i <= j) {
		//while ((Amat[i].i < pivot) || ((Amat[i].i == pivot) && (Amat[i].j < pivot.j)))
		//i++;
		//while ((Amat[j].i > pivot) || ((Amat[j].i == pivot) && (Amat[j].j > pivot.j)))
		//j--;
		while (indx_compare(Amat[i]) < pivot)
			i++;
		while (indx_compare(Amat[j]) > pivot)
			j--;
		if (i <= j) {
			tmp = Amat[i];
			Amat[i] = Amat[j];
			Amat[j] = tmp;
			i++;
			j--;
		}
	}

	// recursion
	/*
#ifdef _OPENMP
	if (qs_abbys_heigh < 16) {
#pragma omp parallel sections
		{
			{
				if (first < j)
					qs(Amat, first, j, indx_compare);
			}

#pragma omp section
			{
				if (i < last)
					qs(Amat, i, last, indx_compare);
			}
		}
	}
	else {
		if (first < j)
			qs(Amat, first, j, indx_compare);
		if (i < last)
			qs(Amat, i, last, indx_compare);
	}
#else
*/
	if (first < j)
		qs(Amat, first, j, indx_compare);
	if (i < last)
		qs(Amat, i, last, indx_compare);
	//#endif
}

// ������� ���������� �����.
// ����������������� � �������������� ��. ��������� ������ ����������
// ���. 106.
template <typename TMatrixElm>
void QuickSort(TMatrixElm*& Amat, integer first, integer last,
	bool (*compare)(TMatrixElm& Amat1, TMatrixElm& Amat2)) {
	// list ��������������� ������ ���������
	// first ����� ������� �������� � ����������� ����� ������
	// last ����� ���������� �������� � ����������� ����� ������

	if (0) {
		/*
		// BubbleSort
		integer numberOfPairs = last - first + 1;
		bool swappedElements = true;
		while (swappedElements) {
		numberOfPairs--;
		swappedElements = false;
		for (integer i = first; i <= first + numberOfPairs - 1; i++) {
		if (compare(Amat[i + 1],Amat[i])) {
			swap(Amat, i, i + 1);
			swappedElements = true;
		}
		}
		}
		*/
	}
	else
	{
		integer pivot;

		if (first < last) {
			pivot = PivotList(Amat, first, last, compare);
			QuickSort(Amat, first, pivot - 1, compare);
			QuickSort(Amat, pivot + 1, last, compare);
		}
	}
} // QuickSort


// comparator function to make min heap 
struct greaters {
	bool operator()(const std::pair<integer, integer>& a, const std::pair<integer, integer>& b) const {
		return a.first < b.first;
	}
};

/*
// ���������� �� ����������� �� ������� i	
void mySTDHeapSort2(Ak1*& Amat, integer first, integer last, 
	integer (*indx_compare)(Ak1& Amat))
{
	integer n = last - first + 1;


	//std::vector<std::pair<integer,integer>> v1(n);
	std::vector<std::pair<integer, integer>> v1;

	for (integer i = first; i <= last; i++) {

		integer ind = i - first;

		// ��������� Amat[i].i ���������� � �������.
				
		//v1[i - first] = std::make_pair(indx_compare(Amat[i]), ind);
		v1.push_back(std::make_pair(indx_compare(Amat[i]), ind));
	}

	std::make_heap(v1.begin(), v1.end(), greaters());

	doublereal* a = new doublereal[last - first + 1];
	integer* i_a = new integer[last - first + 1];
	integer* j_a = new integer[last - first + 1];	
	for (integer i = first; i <= last; i++) {
		a[i - first] = Amat[i].aij;
		i_a[i - first] = Amat[i].i;
		j_a[i - first] = Amat[i].j;		
	}

	//std::sort(v1.begin(), v1.end(), greaters());
	std::sort_heap(v1.begin(), v1.end(), greaters());

	integer i = first;
	
	for (auto it = v1.begin(); it != v1.end(); ++it)
	{
		//integer i_ = std::get<1>(*it);
		integer i_ = (*it).second;
		//integer i_ = (v1.back()).second; // ���������� ����������� �������.

		Amat[i].i = i_a[i_];
		Amat[i].j = j_a[i_];
		Amat[i].aij = a[i_];
		i++;

	}
	delete[] a;
	delete[] j_a;
	delete[] i_a;


	//v1.erase(v1.begin(),v1.end());
	v1.clear();
}
*/

// ���������� �� ����������� �� ������� i	
void mySTDHeapSort(Ak1*& Amat, integer first, integer last,
	integer(*indx_compare)(Ak1& Amat))
{
	integer n = last - first + 1;

	//std::vector<std::pair<integer,integer>> v1(n);
	std::vector<std::pair<integer, integer>> v1;

	for (integer i = first; i <= last; i++) {

		integer ind = i - first;

		// ��������� Amat[i].i ���������� � �������.
		
		//v1[i - first] = std::make_pair(indx_compare(Amat[i]),ind);
		v1.push_back(std::make_pair(indx_compare(Amat[i]), ind));
	}

	std::make_heap(v1.begin(), v1.end(), greaters());

	real_mix_precision* a = new real_mix_precision[last - first + 1];
	integer_mix_precision* j_a = new integer_mix_precision[last - first + 1];
	integer_mix_precision* i_a = new integer_mix_precision[last - first + 1];
	for (integer i = first; i <= last; i++) {
		integer ind = i - first;
		a[ind] = Amat[i].aij;
		j_a[ind] = Amat[i].j;
		i_a[ind] = Amat[i].i;
	}

	integer i = last;
	
	while (!v1.empty())
	{
		std::pop_heap(v1.begin(), v1.end(), greaters()); // ������� ������������ ������� �� ����.

		integer i_ = (v1.back()).second; // ���������� ������������ �������.
		//printf("i==back=%lld front=%lld\n",(v1.back()).first, (v1.front()).first);

		v1.pop_back();

		Amat[i].i = i_a[i_];
		Amat[i].j = j_a[i_];
		Amat[i].aij = a[i_];
		i--;

	}
	delete[] a;
	delete[] j_a;
	delete[] i_a;


	//v1.erase(v1.begin(),v1.end());
	v1.clear();

}

#endif