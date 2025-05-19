#include "list.h"
#include "grupp26.c"



//Returnera en tom lista - funktionen är färdig
List create_empty_list(void)
{
    return NULL;
}

//Här skapas nya noder. Denna funktion är den enda som ska använda malloc.
//Denna funktion ska i sin tur bara kallas av add_first och add_last.
//Notera att den är static och inte finns med i interfacet
static Node * create_list_node(const Data data)
{
    
    Node* newlist = malloc(sizeof(Node));

    if (newlist == NULL)
    {
        return(NULL);
    }

    newlist->data = data;
    newlist->next = NULL;
    

    return(newlist);
	//glöm inte att kolla så att malloc inte returnerade NULL
    

}

//Är listan tom?
//Returnerar 1 om listan är tom, annars 0
int is_empty(const List list)
{
    if (list == NULL)
    {
        return (1);
    }

    else 
    {
        return(0);
    }    
}

//Lägg till en nod först i listan
void add_first(List *list, const Data data)
{
    Node *newNode = create_list_node(data);
    
    if (is_empty(*list) == 1)
    {
         *list = create_list_node(data);

    }
 
    else
    {
         newNode->next = *list;
         *list = newNode;

    }

    
}

//lägg till nod sist i listan
void add_last(List *list, const Data data)
{
    Node *newNode = create_list_node(data);

    if(is_empty(*list) == 1)
    {
        *list = newNode;
    }
    else
    {
        Node *current = *list;
        while(current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;

    }


}

//Ta bort första noden i listan
//precondition: listan är inte tom (testa med assert)
void remove_first(List *list)
{
assert(!is_empty(*list));

Node *temp = *list;
*list = temp->next;
free(temp);

}

//ta bort sista noden i listan
//precondition: listan är inte tom (testa med assert)
void remove_last(List *list)
{
    assert(!is_empty(*list));

    if((*list)->next == NULL)
    {
        free (*list);
        *list = NULL;
    }
    else
    {
        Node *current = *list;
        while (current->next->next != NULL)
        {
            current = current->next;
        }
        free (current->next);
        current->next = NULL;

    }

}

//töm listan (ta bort alla noder ur listan)
//postcondition: Listan är tom, *list är NULL
void clear_list(List *list)
{
	while (!is_empty (*list))
    {
         remove_first( list);
    }

}

//Skriv ut listan genom UART
void print_list(const List list)
{   

    Send_Int(list->data);

    if (list->next != NULL)
    {

       print_list(list->next);

    }
}

//returnera första datat i listan
//precondition: listan är inte tom (testa med assert)
Data get_first_element(const List list)
{
    assert(!is_empty(list));
    
    return list->data;


}

//returnera sista datat i listan. 
//precondition: listan är inte tom (testa med assert)
Data get_last_element(const List list)
{
    assert(!is_empty(list));
    Node *current = list;

    while (current->next != NULL)
    {
        current = current->next;
    }

    return current->data;

}

//Returnera hur många noder som finns i listan
int number_of_nodes(const List list)
{
    int count = 1;
    Node *current = list;
    if(is_empty(list) == 1)
    {
        return 0;
    }
    else
    {
    
        while(current->next != NULL)
        {
        count++;
        current = current->next;
        }
    }

    return count;
}

//Sök efter data i listan, returnera 1 om datat finns, annars 0.
int search(const List list, const Data data)
{
 
    while (list != NULL)
    {
    
        if (list->data == data)
        {
        return (1);
        }
	    return search(list->next , data);

    }

    return(0);
}

//Ta bort data ur listan (första förekomsten), returnera 0 om datat inte finns, annars 1
int remove_element(List *list, const Data data)
{
     Node *current = *list;
     Node *prev = NULL;

       
    while (current != NULL) 
    {
        if (current->data == data) 
        {
            if (prev == NULL) 
            {
                *list = current->next;
                free(current);
                return 1;
               
            } 
            else if (prev != NULL)
            {
                prev->next = current->next;
                free(current); 
                return 1;
               
            }
        }
        prev = current;
        current = current->next;
    }
    return 0;

}


int is_sorted(List list)
{
     while (list != NULL && list->next != NULL)
    {
        if (list->data > list->next->data)
        {
            return 0;  // Inte sorterad
        }
        list = list->next;
    }
    return 1;  // Sorterad
}
    void array_build(List *list, const int arr[], int size)
    {

        for ( int i = 0; i < size ; i ++)
        {

            add_last( list , arr[i]);

        }

        

    }

void bubble(List *list)
{
    if (is_empty(*list) || (*list)->next == NULL)
        return;

    int swapped;
    Node **ptr;

    do {
        swapped = 0;
        ptr = list;

        while ((*ptr)->next != NULL) {
            Node *a = *ptr;
            Node *b = a->next;

            if (a->data < b->data) {
                
                a->next = b->next;
                b->next = a;
                *ptr = b;

                swapped = 1;
            }

           
            ptr = &(*ptr)->next;
        }
    } while (swapped);
}





