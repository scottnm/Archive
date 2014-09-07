int list_size(const struct arraylist list)
{
    return list.size;
}

void list_add(n_list *list, int value)
{
    if(list.size >= list.cap)
    {
        list.cap = 2*list.cap;
        static int new_con[cap];
        int index = 0;
        
        for(; index < cap; index++)
            new_con[index] = list.con[index];
        
        list->con = new_con;
        
        list.con[size] = value;
        
        list.size++;
    }
}