#include <assert.h>
#include "BSTree.h"
#include "grupp26.c"
#include <math.h>
/*Det är helt tillåtet att lägga till egna hjälpfunktioner men de befintliga funktionerna ska implementeras enligt instruktionerna*/

/* Statiska hjälpfunktioner används av andra funktioner i trädet och ska inte ligga i interfacet (användaren behöver inte känna till dessa) */

/* Skapar en trädnod med det givna datat. Denna funktion bör vara den enda som använder malloc för att skapa en ny treeNode*/
static struct treeNode* create_tree_node(int data)
{
    struct treeNode* newNode = malloc(sizeof(struct treeNode));

    if (newNode == NULL)
    {
        return NULL;
    }

    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL; 

    return (newNode);

}


/* Implementation av trädet, funktionerna i interfacet */

/* Skapar ett tomt träd - denna funktion ar färdig */
BSTree create_empty_tree(void)
{
	return NULL;
}

/* Returnerar 1 ifall trädet är tomt, 0 annars */
int is_empty(const BSTree tree)
{
    if(tree == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}


/* Sätter in 'data' sorterat i *tree */
void insert_sorted(BSTree* tree, int data)
{
	/*Tänk på att trädet kan vara tomt vid insättning
    I vanliga fall kan man lösa dubletter på olika sätt. Eftersom vi vill att era lösningar
    fungerar likadant inför examinationen så bestämmer jag att dubletter ej ska tillåtas i trädet.
    Post-condition kan verifieras med hjälp av find(...)*/

    //BSTree *current = *tree;
 
        if (is_empty(*tree))
        {
            *tree = create_tree_node(data);
        }
        else if (data < (*tree)->data)
        {
            insert_sorted(&((*tree)->left), data);
        }
        else if (data > (*tree)->data)
        {
            insert_sorted(&((*tree)->right), data);
        }

        
    
}

/* Utskriftsfunktioner
Det räcker att ni implementerar LR-ordningarna*/
void print_preorder(const BSTree tree)
{
    if (!is_empty(tree))
    {

    Send_Int(tree->data);

    print_preorder(tree->left);
    
    print_preorder(tree->right);
    
    }

}

void print_inorder(const BSTree tree)
{
    if (!is_empty(tree))
    {
        print_inorder(tree->left);

        Send_Int(tree->data);
    
        print_inorder(tree->right);
    }

}

void print_postorder(const BSTree tree)
{
    if (!is_empty(tree))
    {
        print_postorder(tree->left);
    
        print_postorder(tree->right);
        
        Send_Int(tree->data);
    }

}

/* Returnerar 1 om 'data' finns i tree, 0 annars */
int find(const BSTree tree, int data)
{
    // Tänk pa att trädet kan vara tomt
    if (is_empty(tree))
    {
        return 0;
    }
  
    if (tree->data > data){

    find(tree->left, data);
    
    }

    if(tree->data < data){

    find(tree->right, data);
    }
    
    else 
    {
        return 1;
    }

}

/* Tar bort 'data' från trädet om det finns */
void remove_element(BSTree* tree, int data)
{
	/* Inget data ska/kan tas bort från ett tomt träd
    Tre fall: Ett löv (inga barn), ett barn (vänster eller höger), två barn
    
    Glöm inte att frigöra noden när den länkats ur trädet'*/

  
     if (*tree == NULL)
    {
        return; // Tomt träd, inget att ta bort
    }

    if (data < (*tree)->data)
    {
        // Leta vidare i vänstra subträdet
        remove_element(&((*tree)->left), data);
    }
    else if (data > (*tree)->data)
    {
        // Leta vidare i högra subträdet
        remove_element(&((*tree)->right), data);
    }
    else
    {
        // Hittade noden som ska tas bort

        // Fall 1: Inga barn
        if ((*tree)->left == NULL && (*tree)->right == NULL)
        {
            free(*tree);
            *tree = NULL;
        }
        // Fall 2: Endast höger barn
        else if ((*tree)->left == NULL)
        {
            struct treeNode* temp = *tree;
            *tree = (*tree)->right;
            free(temp);
        }
        // Fall 3: Endast vänster barn
        else if ((*tree)->right == NULL)
        {
            struct treeNode* temp = *tree;
            *tree = (*tree)->left;
            free(temp);
        }
        // Fall 4: Två barn
        else
        {
            // Hitta minsta värdet i högra subträdet (in-order successor)
            struct treeNode* successor = (*tree)->right;
            while (successor->left != NULL)
            {
                successor = successor->left;
            }

            // Kopiera efterträdarens data till denna nod
            (*tree)->data = successor->data;

            // Ta bort efterträdaren rekursivt
            remove_element(&((*tree)->right), successor->data);
        }
    }

}

/* Returnerar hur många noder som totalt finns i trädet */
int number_of_nodes(const BSTree tree)
{
    int count = 1;
    if (is_empty(tree))
    {
        return 0;
    }

    if (tree->left != NULL)
    {
        count += number_of_nodes(tree->left);
        
    }

    if (tree->right != NULL)
    {
        count += number_of_nodes(tree->right);

    }

    return count;

}

/* Returnerar hur djupt trädet är */
int depth(const BSTree tree)
{   
    //struct treeNode *current = tree;
    if (is_empty(tree))
    {
        return 0;
    }

    int countleft = 1, countright = 1;
    countleft += depth(tree->left);
    countright += depth(tree->right);


    if(countleft>=countright)
    {
        return countleft;
    }
    else if (countleft<countright)
    {
        return countright;
    }
  
    
    
}

/* Returnerar minimidjupet för trädet
Se math.h för användbara funktioner*/
int min_depth(const BSTree tree)
{
    int count = number_of_nodes(tree);

    return (int)ceil(log2(count+1));


}

static void inorder_copy(const BSTree node, int* array, int* index)
{
   if (node == NULL) return;

    inorder_copy(node->left, array, index);
    array[(*index)++] = node->data;
    inorder_copy(node->right, array, index);
}


/* Returnerar en dynamiskt allokerad array som innehåller trädets data sorterat */
static int* write_sorted_to_array(const BSTree tree)
{
    /* Skapa en dynamisk array med rätt storlek
    
    Skriv datat från trädet sorterat till arrayen (minsta till största)
       - till detta kanske du behöver en hjälpfunktion */

    int count = number_of_nodes(tree);
    int* array = malloc(count * sizeof(int));
    if (array == NULL) 
    {
        return NULL;
    }

    int index = 0;
    inorder_copy(tree, array, &index);
    return array;

}

/* Bygger upp ett sorterat, balanserat träd från en sorterad array */
static void build_tree_sorted_from_array(BSTree* tree, const int arr[], int size)
{
    /* Bygg rekursivt från mitten.
    Mittenelementet i en delarray skapar rot i delträdet
    Vänster delarray bygger vänster delträd
    Höger delarray bygger höger delträd*/

    if (size <= 0) 
    {
        *tree = NULL;
        return;
    }

    int mid = size / 2;
    insert_sorted(tree, arr[mid]);

    // Vänster subträd
    build_tree_sorted_from_array(&((*tree)->left), arr, mid);

    // Höger subträd
    build_tree_sorted_from_array(&((*tree)->right), arr + mid + 1, size - mid - 1);
}


/* Balansera trädet så att depth(tree) == minDepth(tree) */
void balance_tree(BSTree* tree)
{
    /* Förslag på algoritm:
    - överfor trädet till en dynamiskt allokerad array med writeSortedToArray()
    - töm trädet med freeTree()
    - bygg upp trädet rekursivt från arrayen med buildTreeSortedFromArray()
    - frigör minne för den dynamiskt allokerade arrayen


    Post-conditions:
    - tree har lika många noder som tidigare
  - djupet för trädet är samma som minimumdjupet för trädet */

    int count = number_of_nodes(*tree);
    if (count == 0) return;

    int* array = write_sorted_to_array(*tree);
    if (array == NULL) return;

    free_tree(tree); // töm det gamla obalanserade trädet

    build_tree_sorted_from_array(tree, array, count);

    free(array);


}


/* Töm trädet och frigör minnet för de olika noderna */
void free_tree(BSTree* tree)
{
    //struct treeNode *temp = *tree;
    if(*tree == NULL)
    {

        return;
    }

    free_tree(&(*tree)->left);
    free_tree(&(*tree)->right);

    free (*tree);
    *tree = NULL;
  
}



