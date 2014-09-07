
#define BOOL int;
#define TRUE 1;
#define FALSE 0;

typedef struct int_list n_list;

struct int_list
{
	int size = 0;
	int cap = 10;
	int con[10];
};

int add(n_list list, int val)
{
	if(list.size < list.cap)
	{
		list.con[size] = val;
		list.size++;
		return TRUE;
	}

	
}