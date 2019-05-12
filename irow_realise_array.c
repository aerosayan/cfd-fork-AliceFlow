// ���� irow_realise_array.c 
// ���������� ���������� ����� � �������� �� ��������.

#include "sparse_gauss.h" // ���������� ���� �������

// ������������ ���������� �� ������� 
// ������������ ������� �������������� �
// ����� ������ ���������� ��� �������.
// ��� ��������� ������ �������� �������� 1.
const integer CLUSTER_SIZE=20; // core 2 quad Q6600 2.4GGH 4cores nodes=2274: i - 10s, 5 - 9s, 10 - 8s.

// ����� ������ � ������ key  
integer search_i(TERM* list, integer n, integer key) {
	/*
	// �������� ����� ������ � ������ key
	integer i=-1;
	integer i1; // ������� ����� for
    // �������� ����� ������ � �������� key
	for (i1=0; i1<n; i1++) if (list[i1].key==key) i=i1;
	return i;
	*/
	// �������� ����� ������ � ������ key
	// ������ list �������������� ������������� �� ���� key.
    integer iL=0, iR=n-1;
	integer im;
	integer i=-1;
	while (iL<=iR) {
       im=(integer)(iL+iR)/2;
	   if (list[im].key < key) { iL = im + 1; }
	   else if (list[im].key > key) { iR = im - 1; }
	   else {
		   iR = iL - 1;
		   i = im;
	   }
	}
	return i;

} // search_i

// ������� ������� � ������ ������ key
// ��� �������� �� �������� ��������������� ������� list
void deleteAt(TERM* &list, integer key, integer &n, integer &pool) {

   /* ���������� ��� ��� pool`a:
   // ��������� ������
   TERM *listloc=new TERM[n-1];
   integer i1, i2;

   i2=0; // ������� � ��������� �����
   for (i1=0; i1<n; i1++) {
      if (i1!=key) {
		   listloc[i2]=list[i1];
		   i2++;
      }
   }
   
   delete list;
   list = new TERM[n-1];


   // �������� �����������
   for (i1=0; i1<(n-1); i1++) {
	   list[i1]=listloc[i1];
   }
   // ������������ ������
   delete listloc; 
   */

   integer ipos=-1,i1; // ������� ����� for
   ipos=search_i( list, n, key);
   //for (i1=0; i1<n; i1++) if (list[i1].key==key) ipos=i1;
   //printf("i=%d",ipos);

   if ((pool-n)<CLUSTER_SIZE) {
        for (i1=ipos; i1<(n-1); i1++) list[i1]=list[i1+1];
		n--;
   }
   else
   {
        // ��������� ������
	   TERM *listloc = NULL;
	   listloc=new TERM[n - 1];
        integer i2=0; // ������� � ��������� �����

        for (i1=0; i1<n; i1++) {
            if (i1!=ipos) {
		        listloc[i2]=list[i1];
		        i2++;
            }
        }
   
		delete list;
        list = new TERM[n-1];

        // �������� �����������
        for (i1=0; i1<(n-1); i1++) {
	        list[i1]=listloc[i1];
        }
        // ������������ ������
		if (listloc != NULL) {
			delete[] listloc;
			listloc = NULL;
		}
		n--;
		pool=n;
   }

} // deleteAt

// �������� ������� �� ���������� : num, val.
// ���������� � pool`�� ������������� ��� ����������
// ������� �������������� �� ��������� ������ � �����������.
// �������� ��������� CLUSTER_SIZE ������� �� ����������� ������� ����
// � ��������� ��� ���� ��� ������������� ������� �� ��� ������ 
// ����������� ���������������� �� ����� ������.
void add(TERM* &list, integer &n, integer &pool, integer num, doublereal val) {
    TERM *listloc=NULL;
    integer i1=0;

	/* ���������� ��� pool`�:
    // ��������� ������
	if (n>0) {
		listloc=new TERM[n];
		// ������ ��������� �����
		for (i1=0; i1<n; i1++) {
			listloc[i1]=list[i1];
		}
        delete list;
	}
	list=new TERM[n+1];
		    
    // �������� �����������
    for (i1=0; i1<n; i1++) {
		list[i1]=listloc[i1];
	}
	list[n].key=num;
	list[n].val=val;
   
	// ������������ ������
	if (n>0) delete listloc;
	n++;
	*/

	/*
	// ������ list �� ����������
	if (n==0) {
		// ���� ������ �� ��� ������
		pool=CLUSTER_SIZE;
		list=new TERM[pool];
		n=1;
        list[n-1].key=num;
	    list[n-1].val=val;  
	}
	 else
	{
		if (n<pool) {
			n++;
            list[n-1].key=num;
	        list[n-1].val=val;
		}
		else
		{
			// n==pool
			pool+=CLUSTER_SIZE;

            listloc=new TERM[n];
		    // ������ ��������� �����
		    for (i1=0; i1<n; i1++) {
			     listloc[i1]=list[i1];
		    }
            delete list; 

            list=new TERM[pool];
			// �������� �����������
            for (i1=0; i1<n; i1++) {
		         list[i1]=listloc[i1];
	        }
	        list[n].key=num;
	        list[n].val=val;
			n++;

			delete listloc;
		}
	}
	*/


	// ������ list ���������� �� ����������� ����� key
    if (n==0) {
		// ���� ������ �� ��� ������
		pool=CLUSTER_SIZE;
		list=new TERM[pool];
		n=1;
        list[n-1].key=num;
	    list[n-1].val=val;  
	}
	 else
	{
		if (n<pool) {
			// ������� �������� � ����������� ���������������
			n++;
            i1=0;
			while ((list[i1].key<num) && (i1<(n-1))) i1++;
			if (i1==(n-1)) {
                 list[n-1].key=num;
	             list[n-1].val=val;
			}
			else
			{
				for (integer i2=(n-2); i2>=i1; i2--) list[i2+1]=list[i2];
                list[i1].key=num;
	            list[i1].val=val;
			}
		}
		else
		{
			// n==pool
			pool+=CLUSTER_SIZE;

            listloc=new TERM[n];
			if (listloc != NULL) {
				// ������ ��������� �����
				for (i1 = 0; i1 < n; i1++) {
					listloc[i1] = list[i1];
				}
				delete[] list;

				list = new TERM[pool];
				if (list != NULL) {
					// �������� �����������
					// � ����������� ���������������
					i1 = 0;
					while ((i1 < n) && (listloc[i1].key < num) ) {
						list[i1] = listloc[i1];
						i1++;
					}
					if (i1 < pool) {
						list[i1].key = num;
						list[i1].val = val;
					}
					else 
					{
						printf("error 2 : razmera pool nedostatochno\n");
						printf("see function add\n");
						system("pause");
						exit(1);
					}
					for (integer i2 = i1 + 1; i2 < n + 1; i2++) {
						if (i2 < pool)
						{
							list[i2] = listloc[i2 - 1];
						}
						else {
							printf("razmera pool nedostatochno\n");
							printf("see function add\n");
							system("pause");
							exit(1);
						}
					}
					n++;

					if (listloc != NULL) {
						delete[] listloc;
						listloc = NULL;
					}
				}
				else {
					printf("cannot allocate memory for list\n");
					system("pause");
					exit(1);
				}
			}
			else {
				printf("cannot allocate memory for listloc\n");
				printf("see irow_realise_array.c in function add\n");
				system("pause");
				exit(1);
			}
		}
	}

} // add

// ���������� �������� ������ ����
// ������� ����� key
doublereal get_val(TERM* list, integer n, integer key) {
	integer i=-1;
	//integer i1; // ������� ����� for
    // �������� ����� ������ � �������� key
	//for (i1=0; i1<n; i1++) if (list[i1].key==key) i=i1;
	i=search_i( list, n, key);
	if (i!=-1) return (doublereal) list[i].val;
	return 0.0;
} // get_val

// ��������� ����� value � ������
// � ������ key
void modify_add(TERM* &list, integer n, integer key, doublereal value) {
	//integer i1; // ������� ����� for
    // �������� ����� ������ � �������� key
	//for (i1=0; i1<n; i1++) if (list[i1].key==key) list[i1].val+=value;
	integer i=-1;
	i=search_i( list, n, key); // ����� ������ �������
	list[i].val+=value;

} // modify_add

// ������������� ����� value � ������
// � ������ key
void modify_set(TERM* &list, integer n, integer key, doublereal value) {
	//integer i1; // ������� ����� for
    // �������� ����� ������ � �������� key
	//for (i1=0; i1<n; i1++) if (list[i1].key==key) list[i1].val=value;
	integer i=-1;
	i=search_i( list, n, key); // ����� ������ �������
    list[i].val=value;
} // modify_set


// ������� �� ����������� �������������
void get_values(TERM *list, integer n, integer* &indexes, doublereal* &values) {
    integer i1; // ������� ����� for
	for (i1=0; i1<n; i1++) {
		indexes[i1]=list[i1].key; 
		values[i1]=list[i1].val;  
	}
} // get_values