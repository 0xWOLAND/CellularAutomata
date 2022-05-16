/*
 * C++ Program To Implement BST
 */
#ifndef BSTREE_H
#define BSTREE_H
#include <stack>


/*
 * Node Declaration
 */
struct node
{
   unsigned int key_value;
    struct node *p_left;
    struct node *p_right;
};

/*
 * Class Declaration
 */
class BST
{
public:
    node* root;
    void find(int, node **, node **);
    void insert(node *, node *);
    void remove(int);
    void case_0(node *,node *);
    void case_1(node *,node *);
    void case_2(node *,node *);
    void display(node *,unsigned int);
    vector<unsigned int> getArray();
    BST()
    {
        root = NULL;
    }
};



/*
 * Binary Search Tree class
 * Do not alter the display() function, this will break the Repl.it
 * tests
 */


#include <iostream>
#include <cstdlib>

using namespace std;



/*
 * Find Element in the Tree
 * find a node by item_key and provide an updated parent node
 *   pointer and location node pointer
 */


void BST::find(int item_key, node **prnt, node **loc)
{
    node *ptr, *ptrsave;
    if (root == NULL)
    {
        *loc = NULL;
        *prnt = NULL;
        return;
    }
    if (item_key == root->key_value)
    {
        *loc = root;
        *prnt = NULL;
        return;
    }
    if (item_key < root->key_value)
        ptr = root->p_left;
    else
        ptr = root->p_right;
    ptrsave = root;
    while (ptr != NULL)
    {
        if (item_key == ptr->key_value)
        {
            *loc = ptr;
            *prnt = ptrsave;
            return;
        }
        ptrsave = ptr;
        if (item_key < ptr->key_value)
            ptr = ptr->p_left;
        else
            ptr = ptr->p_right;
    }
    *loc = NULL;
    *prnt = ptrsave;
}

/*
 * Inserting Elementunsigned into the Tree
 */
void BST::insert(node *tree, node *newnode)
{
    // if (we are at root)
    if(tree == NULL){
        tree = newnode;
        if(root == NULL){
            root = tree;
        }
    }
//    else if(tree->p_right == NULL && tree->p_left == NULL)
//    {
//        tree->p_left = newnode;
//        // Just create a single node for root to point to,
//        //    with all the data in it.
//        return;
//    }
        // if (there is already a node with our key value)
    else if(tree->key_value == newnode->key_value)
    {
        //Just return
        return;
    }
        //if (the new node is less than the current node)
    else if(newnode->key_value < tree->key_value)
    {
        //if (see if something is already a child on the left)
        if(tree->p_left != NULL)
        {
            insert(tree->p_left, newnode);
            // if so, the recurse insert at this subtree
        }
        else
        {
            // otherwise the new node goes here as a leaf (ie no children)
            tree->p_left = newnode;
            return;
        }
    }
    else  // it must be to the right
    {
        //if (is there already a child on right?)
        if(tree->p_right != NULL)
        {
            // if so, the recurse insert at this subtree
            insert(tree->p_right, newnode);
        }
        else
        {
            // otherwise the new node goes here as a leaf (ie no children)
            tree->p_right = newnode;
            return;
        }
    }
}

/*
 * Delete Element from the tree
 */
void BST::remove(int item)
{
    node *parent, *location;
    if (root == NULL)
    {
        cout<<"Tree empty"<<endl;
        return;
    }
    find(item, &parent, &location);
    if (location == NULL)
    {
        cout<<"Item not present in tree"<<endl;
        return;
    }
    //if (there is no child on left or right)
    if(location->p_right == NULL && location->p_left == NULL)
    case_0(parent, location);
    //if (there is one child on left only)
    else if((location->p_right == NULL) != (location->p_left == NULL))
    case_1(parent, location);
    // if (there are childred on left and right)
    else if(location->p_right != NULL && location->p_left != NULL)
    case_2(parent, location);
}

/*
 * Case 1
 * We have only one child so promote the child and replace the target
 * node
 */
void BST::case_1(node *prnt, node *loc)
{
    node *child;
    bool flag = prnt == NULL;
    if(loc->p_right != NULL){
        if(flag){
            *root = *loc->p_right;
        }
        else{
            *loc = *loc->p_right;
        }
    }
    else if(loc->p_left != NULL){
        if(flag){
            *root = *loc->p_left;
        }
        else{
            *loc = *loc->p_left;
        }
    }
//    free(loc);
}

/*
 * Case 0
 * the node is simply removed no other updates necessary.
 */
void BST::case_0(node *prnt, node *loc )
{
    //if (we are at the root)
    if(prnt == NULL)

    {
        //update the root
        loc->key_value = -1;
        loc->p_right = loc->p_left = NULL;
        root = NULL;
    }
    else
    {
        // otherwise simply remove node
        prnt->p_right = prnt->p_left = NULL;
//        free(loc);
    }
}

/*
 * Case case_2
 * We have to find and promote a successor or predecessor
 */
void BST::case_2(node *prnt, node *loc)
{
    node *cur = loc->p_right;
    while(cur->p_left != NULL){
        cur = cur->p_left;
    }
   unsigned int val = cur->key_value;
    // printf("SUCCESSOR %d\n", val);
    // remove(val);
    if(cur->p_left == NULL && cur->p_right == NULL){
        cur->key_value = -1;
    }
    else if(cur->p_right != NULL){
        loc->p_right = cur->p_right;
    }
    loc->key_value = val;
    
}


/*
 * Display Tree Structure
 */
void BST::display(node *ptr,unsigned int level)
{
   unsigned int i;
    if (ptr != NULL && ptr->key_value != -1)
    {
        display(ptr->p_right, level+1);
        cout<<endl;
        if (ptr == root)
            cout<<"Root->:  ";
        else
        {
            for (i = 0;i < level;i++)
                cout<<"       ";
        }
        cout<<ptr->key_value;
        display(ptr->p_left, level+1);
    }
}

vector<unsigned int> BST::getArray(){
    node* curr = root;
    std::stack<node*> s;
    vector<unsigned int> ans;
    while(curr != NULL || !s.empty()){
        while(curr != NULL){
            s.push(curr);
            curr = curr->p_left;
        }
        curr = s.top();
        s.pop();

        ans.push_back(curr->key_value);

        curr = curr->p_right;
    }

    return ans;
}

/*
41 70 16 68 12 20 45 100 25 48 80 -1
68 100 -1


 * */
#endif	//BSTREE_H