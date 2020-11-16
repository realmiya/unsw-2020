#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


// data type for avl tree nodes
typedef struct AVLTNode {
    int key; //key of this item
    int value;  //value (int) of this item
    int height; //height of the subtree rooted at this node
    struct AVLTNode *parent; //pointer to parent
    struct AVLTNode *left; //pointer to left child
    struct AVLTNode *right; //pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree {
    int size;      // count of items in avl tree
    struct AVLTNode *root; // root
} AVLTree;


void AVLTNodeToArrayInOrder(AVLTreeNode *N, int *keys, int *values, int *index) {
    //inorder,left room right!
    if (N != NULL) {
        AVLTNodeToArrayInOrder(N->left, keys, values, index);
        keys[*index] = N->key;
        //array keys
        values[*index] = N->value;
        //array values
        *index = *index + 1;
        //index start from 0
        AVLTNodeToArrayInOrder(N->right, keys, values, index);
    }
}





// create a new empty avl tree
AVLTree *newAVLTree() {
    AVLTree *newTree;
    newTree = malloc(sizeof(struct AVLTree));
    if (newTree != NULL) {
        //initialization
        newTree->size = 0;
        newTree->root = NULL;
    }
    return newTree;
}


//O(1)
int heightTreeNode(AVLTreeNode *N) {
    if (N == NULL) {
        return 0;
        //the lowest one is 0, so use -1 for null!
    } else {
        int left = 0;
        if (N->left != NULL) {
            left = N->left->height;
        }
        int right = 0;
        if (N->right != NULL) {
            right = N->right->height;
        }
        if (left > right) {
            return left + 1;
        } else {
            return right + 1;
        }
    }
}


// create a new AVLTreeNode
//new AVLTreeNode belongs to AVLTreeNode, so it uses typedef struct AVLTreenode with a new pointer
//alternative:
AVLTreeNode *newAVLTreeNode(int k, int v) {
    AVLTreeNode *newNode;
    newNode = malloc(sizeof(AVLTreeNode));
    // use malloc to allocate space to new, the way used in template doesn'N need *!
    //alternative: AVLTreeNode *new = malloc(sizeof(AVLTreeNode))
    if (newNode != NULL) {
        //initialization
        newNode->key = k;
        newNode->value = v;
        newNode->height = 0;
        newNode->left = NULL;
        newNode->right = NULL;
    }
    return newNode;
}






/**
 *
 * @param t1
 * @return
 */
AVLTreeNode *CloneAVLTreeNode(AVLTreeNode *t1) {
    AVLTreeNode *CloneNode = NULL;
    if (t1 != NULL) {
        CloneNode = newAVLTreeNode(t1->key, t1->value);
        CloneNode->height = t1->height;
        CloneNode->left = CloneAVLTreeNode(t1->left);
        CloneNode->right = CloneAVLTreeNode(t1->right);
    }
    return CloneNode;
}


// O(n), each node of the TVLree will be traversal
AVLTree *CloneAVLTree(AVLTree *T) {
    AVLTree *newTree = NULL;
    if (T != NULL) {
        newTree = newAVLTree();
        newTree->size = T->size;
        newTree->root = CloneAVLTreeNode(T->root);
    }
    return newTree;
}




int compareTreeNode(int k1, int v1, int k2, int v2) {
    //k1,v1 is the value of the insert one, compare it with k2,v2(current one)
    int result = 0;
    if (k1 == k2) {
        if (v1 == v2) {
            result = 0;
        } else if (v1 < v2) {
            result = -1;
        } else {
            //v1>v2
            result = 1;
        }
    } else if (k1 < k2) {
        result = -1;
    } else {
        result = 1;
    }
    return result;
}
//if the result=1 insert to right side of current, if -1,then place it left side

void ArrayU(int *t1_keys, int *t1_values, int t1length,
            int *t2_keys, int *t2_values, int t2length,
            int *t3_keys, int *t3_values, int *t3Index) {
    int t1Index = 0, t2Index = 0, compare = 0;

    while (t1Index < t1length && t2Index < t2length) {
        compare = compareTreeNode(t1_keys[t1Index], t1_values[t1Index], t2_keys[t2Index], t2_values[t2Index]);
        //when certain item of two set are same,only need to add one of them
        if (compare == 0) {
            t3_keys[*t3Index] = t1_keys[t1Index];
            t3_values[*t3Index] = t1_values[t1Index];
            *t3Index = *t3Index + 1;
            t1Index++;
            t2Index++;
        } else if (compare < 0) {
            //when the k or v of item of t2 is bigger
            t3_keys[*t3Index] = t1_keys[t1Index];
            t3_values[*t3Index] = t1_values[t1Index];
            *t3Index = *t3Index + 1;
            t1Index++;

        } else {
            t3_keys[*t3Index] = t2_keys[t2Index];
            t3_values[*t3Index] = t2_values[t2Index];
            *t3Index = *t3Index + 1;
            t2Index++;
        }
    }
    //need Union,so when the length os larger then the index,continue add
    while (t1Index < t1length) {
        t3_keys[*t3Index] = t1_keys[t1Index];
        t3_values[*t3Index] = t1_values[t1Index];
        *t3Index = *t3Index + 1;
        t1Index++;
    }
    while (t2Index < t2length) {
        t3_keys[*t3Index] = t2_keys[t2Index];
        t3_values[*t3Index] = t2_values[t2Index];
        *t3Index = *t3Index + 1;
        t2Index++;
    }
}


void ArrayIntersection(int *t1k, int *t1V, int t1ArrayTotal, int *t2K, int *t2V, int t2ArrayTotal, int *t3K, int *t3Vs, int *t3Index) {

    int t1_index = 0, t2_index = 0, compare_result = 0;
    while (t1_index < t1ArrayTotal && t2_index < t2ArrayTotal) {
        compare_result = compareTreeNode(t1k[t1_index],
                                         t1V[t1_index],
                                         t2K[t2_index],
                                         t2V[t2_index]);
        if (compare_result == 0) {
            t3K[*t3Index] = t1k[t1_index];
            t3Vs[*t3Index] = t1V[t1_index];
            *t3Index = *t3Index + 1;
            t1_index++;
            t2_index++;
        } else if (compare_result < 0) {
            t1_index++;
        } else {
            t2_index++;
        }
    }
}



AVLTreeNode *ArrayToAvlTreeNode(const int *keys, const int *values, int first, int last) {
    if (first > last) {
        return NULL;
    }
    AVLTreeNode *Node = NULL;
    int middleIndex = (first + last) / 2;
    //int will automaticly change odd number to smaller even number
    int key = keys[middleIndex];
    int value = values[middleIndex];
    Node = newAVLTreeNode(key, value);
    Node->left = ArrayToAvlTreeNode(keys, values, first, middleIndex - 1);
    Node->right = ArrayToAvlTreeNode(keys, values, middleIndex + 1, last);
    Node->height = heightTreeNode(Node);
    return Node;
}


// put your time complexity for ALVTreesUnion() here
//?????????????
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2) {
    if (T1 == NULL || T1->size == 0) {
        return CloneAVLTree(T2);
    }
    else if (T2 == NULL || T2->size == 0) {
        return CloneAVLTree(T1);
    }
    //use function AVLTNodeToArrayInOrder()
    else {
        int *t1_keys = malloc(T1->size * sizeof(int));
        int *t1_values = malloc(T1->size * sizeof(int));
        int t1_index = 0;
        //have an inorder array of T1
        AVLTNodeToArrayInOrder(T1->root, t1_keys, t1_values, &t1_index);

        int *t2_keys = malloc(T2->size * sizeof(int));
        int *t2_values = malloc(T2->size * sizeof(int));
        int t2_index = 0;
        //have an inorder array of T2
        AVLTNodeToArrayInOrder(T2->root, t2_keys, t2_values, &t2_index);
        //make an inorder array of union of tree1+tree2
        int *t3_keys = malloc((T1->size + T2->size) * sizeof(int));
        int *t3_values = malloc((T1->size + T2->size) * sizeof(int));
        int t3_index = 0;

        ArrayU(t1_keys, t1_values, T1->size, t2_keys, t2_values, T2->size, t3_keys, t3_values, &t3_index);
//make a union tree:
        AVLTree *UnionTree = newAVLTree();
        UnionTree->size = t3_index + 1;
        UnionTree->root = ArrayToAvlTreeNode(t3_keys, t3_values, 0, t3_index - 1);
        return UnionTree;
    }
}


// put your time complexity for ALVTreesIntersection() here
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2) {
    if (T1 == NULL || T1->size == 0) {
        return NULL;
    }
    if (T2 == NULL || T2->size == 0) {
        return NULL;
    }

    int *t1_keys = malloc(T1->size * sizeof(int));
    int *t1_values = malloc(T1->size * sizeof(int));
    int t1_index = 0;
    //array of T1'node(k and v)
    AVLTNodeToArrayInOrder(T1->root, t1_keys, t1_values, &t1_index);
    //T1' and T2' root is N of AVLTNodeToArrayInOrder()  [inorder's root]

    int *t2_keys = malloc(T1->size * sizeof(int));
    int *t2_values = malloc(T1->size * sizeof(int));
    int t2_index = 0;
    //array of T1'node(k and v)
    AVLTNodeToArrayInOrder(T2->root, t2_keys, t2_values, &t2_index);

    int *t3_keys = malloc((T1->size + T2->size) * sizeof(int));
    int *t3_values = malloc((T1->size + T2->size) * sizeof(int));
    int t3_index = 0;

    ArrayIntersection(t1_keys, t1_values, T1->size, t2_keys, t2_values, T2->size, t3_keys, t3_values, &t3_index);

    AVLTree *new_tree = newAVLTree();
    new_tree->size = t3_index + 1;
    new_tree->root = ArrayToAvlTreeNode(t3_keys, t3_values, 0, t3_index - 1);

    return new_tree;

}


/**
 *
 * @param n1
 * @return
 */

AVLTreeNode *rotateRight(AVLTreeNode *n1) {
    AVLTreeNode *endRotate = NULL;
    if (n1 != NULL) {
        AVLTreeNode *n2 = n1->left;
        if (n2 != NULL) {
            //AVLTreeNode *t1,*t2,*t3;
            AVLTreeNode *t1 = n2->left;
            AVLTreeNode *t2 = n2->right;
            AVLTreeNode *t3 = n1->right;
//start rotate:
            n1->left = t2;
            n1->right = t3;
            n1->height = heightTreeNode(n1);

            n2->left = t1;
            n2->right = n1;
            n2->height = heightTreeNode(n2);
            if (t1!=NULL){
                t1->parent=n2;
            }

            if (t2!=NULL){
                t2->parent=n1;
            }
            if (t3!=NULL){
                n1->parent=n2;
            }
            endRotate = n2;

        }
    }
    return endRotate;
}

/**
 *
 * @param n2
 * @return
 */
AVLTreeNode *rotateLeft(AVLTreeNode *n2) {
    AVLTreeNode *endRotate = NULL;
    if (n2 != NULL) {
        AVLTreeNode *n1 = n2->left;
        if (n1 != NULL) {
            AVLTreeNode *t1 = n2->left;
            AVLTreeNode *t2 = n1->left;
            AVLTreeNode *t3 = n1->right;
            //start rotate
            n2->left = t1;
            n2->right = t2;
            n2->height = heightTreeNode(n2);

            n1->left = n2;
            n1->right = t3;
            n1->height = heightTreeNode(n1);


            if (t1!=NULL){
                t1->parent=n2;
            }

            if (t2!=NULL){
                t2->parent=n1;
            }
            if (t3!=NULL){
                t3->parent=n1;
            }
            n2->parent=n1;
            endRotate = n1;
        }
    }
    return endRotate;
}


//O(log(n))
// Since the first part of InsertAVLTreeNode is o(log(n)),and height' complexity is o(1),so 1*logN=logN
AVLTreeNode *InsertAVLTreeNode(AVLTreeNode *parent, AVLTreeNode *N, int k, int v, int *success) {
    //pointer of InsertAVLTreeNode directed to struct AVLTreeNode,That's why it can has elements *parent *left and *right
    //*success is global variable, it can be used in InsertNode().
    // int CompareResult=NULL;
    if (N == NULL) {
        N = newAVLTreeNode(k, v);
        //the node is null, insert directly,use function newAVLTreeNode(k,v)
        N->parent = parent;
        *success = 1;
        //the newAVLTreeNode has added successfully ,let *success=1,
    } else {
        //when there has been current node in the tree,we need to compare the current and new,then decide the left and right
        //if the new node's k is bigger go right,otherwise go left;
        // when int k are are the same, bigger int v go right,otherwise go left
        int Compare = compareTreeNode(k, v, N->key, N->value);
        //use function compareTreeNode()
        //k,v is the new insert one's value,     N->k,value is the current one
        if (Compare == 0) {
            return N;
        }
        //If the item (k, v) exists in the tree, this function simply returns 0 without adding the new item (k, v) to the tree
        if (Compare < 0) {
            N->left = InsertAVLTreeNode(N, N->left, k, v, success);
            //recursive
        }
        if (Compare > 0) {
            N->right = InsertAVLTreeNode(N, N->right, k, v, success);
            //recursive
        }
        //height
        int heightOfLeftNode = heightTreeNode(N->left);
        int heightOfRightNode = heightTreeNode(N->right);
        //if unbalanced,when the height difference is less or equal to 1,it is balance,otherwise unbalance,need to rotate
        //insert node, then find left subtree is higher,unbalanced, need Rotated
        if (heightOfLeftNode - heightOfRightNode > 1) {
            Compare = compareTreeNode(k, v, N->left->key, N->left->value);
            if (Compare > 0) {
                N->left = rotateLeft(N->left);
            }
            N = rotateRight(N);
            //in this case,heightOfLeftNode-heightOfRightNode>1, if compareResult<0, only need rotate once
            //continue insert node, if then find right subtree is higher
        } else if (heightOfRightNode - heightOfLeftNode > 1) {
            Compare = compareTreeNode(k, v, N->right->key, N->right->value);
            if (Compare < 0) {
                //new insert node is on the left and unbalanced.need to rotate twice
                N->right = rotateRight(N->right);
            }
            N = rotateLeft(N);
            //in this case,if compareResult>0 ,only need to rotate once
        }
        //initialize height,every time we insert a new node, we need to identify the height as the height of this node;

    }
    N->height = heightTreeNode(N);
    return N;
    //use the function of heightTreeNode(N) that we written above
}






// put the time complexity analysis for InsertNode() here
//o(log(n))
//????????????????????????????????
//As insertAVLTreeNode is o(log(n))

//this function is used to return 0 or 1, and calculate new size
//*T use structure of AVLTree which has int size[count of items in avl tree and AVLTreeNode *root]
//only when T!=NULL, can insert node, when root is null, create a new AVLTreeNode,use function InsertAVLTreeNode()
int InsertNode(AVLTree *T, int k, int v) {
    int success = 0;
    //success is 0 or 1
    if (T != NULL) {
        T->root = InsertAVLTreeNode(NULL, T->root, k, v, &success);
        if (success == 1) {
            T->size++;
            }
        }
        return success;
}







bool readLineInsertNodeEnd(AVLTree *newTree, char *line) {
    char *key = NULL, *value = NULL;
    char delim[10] = " (,)\r\n\t";
    key = strtok(line, delim);
    value = strtok(NULL, delim);
    //use while loop to continue read key and value
    while (key != NULL && value != NULL) {
        // insert key ,value to avl tree
        InsertNode(newTree, atoi(key), atoi(value));
        //  continue to read key and value
        key = strtok(NULL, delim);
        value = strtok(NULL, delim);
    }
    if (key != NULL && strcmp(key, "end") == 0) {
        return true;
        //the time when should end reading
    } else {
        return false;
    }
}
//O(n logn)
AVLTree *CreateAVLTree(const char *filename) {
    AVLTree *newTree = NULL;
    char line[90000];
    newTree = newAVLTree();
    //read the item from std input:
    if (strcmp(filename, "stdin") == 0) {
        bool endRead = false;
        while (true) {
            while (fgets(line, 100000, stdin) != NULL) {
                endRead = readLineInsertNodeEnd(newTree, line);
                //use fgets() get str then use readLineInsertNodeEnd(newTree, line) insert node,
                if (endRead) {
                    //when read"end" ,finally return true,break while
                    break;
                }
            }
            if (endRead) {
                break;
            }
        }
    } else {
        // read item from the txt file
        FILE *file = fopen(filename, "r");
        if (file != NULL) {

            while (fgets(line, 100000, file) != NULL) {
                readLineInsertNodeEnd(newTree, line);
            }
            fclose(file);
        }
    }
    return newTree;
}







//// put your time complexity analysis of CreateAVLTree() here
////O(n logn),???????????????explain why if it is necessary
//AVLTree *CreateAVLTree(const char *filename) {
//    FILE *file = fopen(filename,"r");
//    AVLTree *newAVLTREE = NULL;
////according to the number of the file, create a newAVLtree
//    if (file != NULL) {
//        newAVLTREE = newAVLTree();
//        //use function newAVLTree()
//        //WHEN the file is not empty, we have a new AVLTree
//        char line[1000];
//        char *key,*value;
//        char delim[10] = " (),\r\n\N";
//        //when read anything like " (),\r\n\N", split the number out
//        while (fgets(line, 1000, file) != NULL) {
//            key = strtok(line, delim);
//            value = strtok(NULL, delim);
//            //printf(d%,)
//            while (key != NULL && value != NULL) {
//                InsertNode(newAVLTREE, atoi(key), atoi(value));
//                //InsertNode to the new T, then continue read following k and value from the file
//                key = strtok(line, delim);
//                value = strtok(NULL, delim);
//            }
//        }
//
//        fclose(file);
//        //we have read all info from the file,over,close file
//    }
//    return newAVLTREE;
//}

AVLTreeNode *counterNode(AVLTreeNode *N) {
    if (N != NULL && N->left != NULL) {
        return counterNode(N->left);
    }
    return N;
}

AVLTreeNode *deleteTreeNode(AVLTreeNode *parent, AVLTreeNode *N, int k, int v, int *delete) {
    if (N != NULL) {
        int Compare = compareTreeNode(k, v, N->key, N->value);
        if (Compare < 0) {
            N->left = deleteTreeNode(N, N->left, k, v, delete);
        } else if (Compare > 0) {
            N->right = deleteTreeNode(N, N->right, k, v, delete);
        } else {
            AVLTreeNode *current;
            if (N->left == NULL && N->right == NULL) {
                current = NULL;
            } else if (N->left == NULL) {
                //delete current right become new current
                current = N->right;
            } else if (N->right == NULL) {
                current = N->left;
                //delete current left become new current

            } else {
                AVLTreeNode *counterNodeofRsubtree = counterNode(N->right);
                counterNodeofRsubtree->left = N->left;
                current = counterNodeofRsubtree;
            }
            free(N);
            //???????????????????/

            return N;
        }
    }
}




// O(log(N))
int DeleteNode(AVLTree *N, int k, int v) {
    int success = 0;
    //success can be 1 or 0,delete successfully it is 1 and size-- , otherwise it is 0
    if (N != NULL && N->size > 0) {
        N->root = deleteTreeNode(NULL, N->root, k, v, &success);
        if (success > 0) {
            N->size--;
        }
    }
    return success;
}






AVLTreeNode *SearchTreeNode(AVLTreeNode *N, int k, int v) {
    AVLTreeNode *result = NULL;
    if (N != NULL) {
        int compareResultSearch = compareTreeNode(k, v, N->key, N->value);
        if (compareResultSearch == 0) {
            result = N;
        } else if (compareResultSearch < 0) {
            result = SearchTreeNode(N->left, k, v);
        } else {
            result = SearchTreeNode(N->right, k, v);
        }
    }
    return result;
}

// put your time complexity analysis for Search() here
AVLTreeNode *Search(AVLTree *T, int k, int v) {
    AVLTreeNode *result = NULL;
    if (T != NULL && T->size > 0) {
        result = SearchTreeNode(T->root, k, v);

    }
    return result;

}


void PrintInOrderAVLTreeNode(AVLTreeNode *N) {
    if (N != NULL) {
        PrintInOrderAVLTreeNode(N->left);
        printf("(%d,%d),%d\n", N->key, N->value, N->height-1);
        PrintInOrderAVLTreeNode(N->right);
    }
}

// O(n)
void PrintAVLTree(AVLTree *T) {
    //according to the output of the assignment ,it is inorder
    if (T != NULL && T->size > 0) {
        PrintInOrderAVLTreeNode(T->root);
    }
}


void FreeAVLTreeNode(AVLTreeNode *N) {
    if (N != NULL) {
        FreeAVLTreeNode(N->left);
        FreeAVLTreeNode(N->right);
        free(N);
        //free treeNode:left, right ,then itself
    }
}


//O(n): each node has been traversal, so the time complexity is O(n)
void FreeAVLTree(AVLTree *T) {
    if (T != NULL && T->size > 0) {
        FreeAVLTreeNode(T->root);
        free(T);
    }
}


int main() {
    AVLTree *t1, *t2, *t3, *t4;
    t1 = CreateAVLTree("File1.txt");
    t2 = CreateAVLTree("File3.txt");

    PrintAVLTree(t1);

    DeleteNode(t1, 19, 200);

    PrintAVLTree(t1);
    PrintAVLTree(t2);


    t3 = AVLTreesUnion(t1, t2);
    t4 = AVLTreesIntersection(t1, t2);

    PrintAVLTree(t3);
    PrintAVLTree(t4);

    // free
    FreeAVLTree(t1);
    FreeAVLTree(t2);
    FreeAVLTree(t3);
    FreeAVLTree(t4);

}



//int main() //sample main for testing
//{
//    int i, j;
//    AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
//    AVLTreeNode *node1;

//    tree1=CreateAVLTree("stdin");
//    PrintAVLTree(tree1);
//    FreeAVLTree(tree1);
    //you need to create the text file file1.txt
    // to store a set of items without duplicate items
//    tree2 = CreateAVLTree("file1.txt");
//    PrintAVLTree(tree2);
//    tree3=CloneAVLTree(tree2);
//    PrintAVLTree(tree3);
//    FreeAVLTree(tree2);
//    FreeAVLTree(tree3);
//    //Create tree4
//    tree4=newAVLTree();
//    j=InsertNode(tree4, 10, 10);
//    for (i=0; i<15; i++)
//    {
//        j=InsertNode(tree4, i, i);
//        if (j==0) printf("(%d, %d) already exists\n", i, i);
//    }
//    PrintAVLTree(tree4);
//    node1=Search(tree4,20,20);
//    if (node1!=NULL)
//        printf("key= %d value= %d\n",node1->key,node1->value);
//    else
//        printf("Key 20 does not exist\n");
//
//    for (i=17; i>0; i--)
//    {
//        j=DeleteNode(tree4, i, i);
//        if (j==0)
//            printf("Key %d does not exist\n",i);
//        PrintAVLTree(tree4);
//    }
//    FreeAVLTree(tree4);
//    //Create tree5
//    tree5=newAVLTree();
//    j=InsertNode(tree5, 6, 25);
//    j=InsertNode(tree5, 6, 10);
//    j=InsertNode(tree5, 6, 12);
//    j=InsertNode(tree5, 6, 20);
//    j=InsertNode(tree5, 9, 25);
//    j=InsertNode(tree5, 10, 25);
//    PrintAVLTree(tree5);
//    //Create tree6
//    tree6=newAVLTree();
//    j=InsertNode(tree6, 6, 25);
//    j=InsertNode(tree6, 5, 10);
//    j=InsertNode(tree6, 6, 12);
//    j=InsertNode(tree6, 6, 20);
//    j=InsertNode(tree6, 8, 35);
//    j=InsertNode(tree6, 10, 25);
//    PrintAVLTree(tree6);
//    tree7=AVLTreesIntersection(tree5, tree6);
//    tree8=AVLTreesUnion(tree5,tree6);
//    PrintAVLTree(tree7);
//    PrintAVLTree(tree8);
//    return 0;
//}
