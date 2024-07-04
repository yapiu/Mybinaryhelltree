#include <stdio.h>
#include <stdlib.h>

typedef struct binarytree{
    int key;
    struct binarytree *leftChild;
    struct binarytree *rightChild;
    int leftCount;
    int rightCount;
}binarytree;

binarytree *create_tree(int key) {
    binarytree *newNode = (binarytree *)malloc(sizeof(binarytree));
    newNode->key = key;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;
    newNode->leftCount = 0;
    newNode->rightCount = 0;
    return newNode;
}

void insert(int number, binarytree *first) {

    if (first->leftChild != NULL && number < first->key) {
        return insert(number, first->leftChild);
    }

    if (first->rightChild != NULL && number > first->key) {
        return insert(number, first->rightChild);
    }

    if (number > first->key) {
        first->rightChild = create_tree(number);
    }

    if (number < first->key) {
        first->leftChild = create_tree(number);
    }
}

void print_repeated_str(const char *s, unsigned int times) {
    for (int i = 0; i < times; ++i) {
        printf("%s", s);
    }
}

void print_node_recursive(binarytree *first, int space) {
    if (first == NULL) {
        return;
    }

    space += 10;
    print_node_recursive(first->rightChild, space);
    printf("\n");
    print_repeated_str(" ", space);
    printf("%d\n", first->key);
    print_node_recursive(first->leftChild, space);
}

int Counter(int count, binarytree *first) {
    if(first->leftChild != NULL) {
        count = Counter(count + 1, first->leftChild);
    }
    if(first->rightChild != NULL) {
        count = Counter(count + 1, first->rightChild);
    }
    return count;
}

void nodesCounter(binarytree *first) {
    first->leftCount = 0;
    first->rightCount = 0;
    if(first->leftChild != NULL) {
        first->leftCount = Counter(0, first->leftChild) + 1;
        nodesCounter(first->leftChild);
    }
    if(first->rightChild != NULL) {
        first->rightCount = Counter(0, first->rightChild) + 1;
        nodesCounter(first->rightChild);
    }
}

// 0 - баланс сохранен
// 1 - балансировка по правому ротейту
// 2 - балансировка по левому ротейту
// 1 + 2 = 3 - балансировка по правому ротейту ноду с потомком
// 1 + 3 = 4 - балансировка по левому ротейту ноду с потомком
int Balancefactor(binarytree *first) {

    if(first->leftChild == NULL && first->rightChild == NULL) {
        return 0;
    }

    int factor = first->leftCount - first->rightCount;
    if(factor == 1 || factor == 0 || factor == -1) {
        return 0;
    }
    if(factor < -1 && first->leftChild != NULL && first->rightChild != NULL) {
        return 2;
    }
    if(factor > 1 && first->leftChild != NULL && first->rightChild != NULL) {
        return 1;
    }
    if(factor > 1 && (first->leftChild != NULL || first->rightChild != NULL)) {
        return 3;
    }
    if(factor < -1 && (first->leftChild != NULL || first->rightChild != NULL)) {
        return 4;
    }
}

void searcherRight(int *highest, binarytree *first) {
    if (first == NULL || highest == NULL) {
        return;
    }
    nodesCounter(first);
    if(first->rightChild != NULL && first->leftChild != NULL && first->rightChild->rightCount < first->leftChild->leftCount) {
        searcherRight(highest, first->rightChild);
    }
    if(first->rightChild != NULL && first->leftChild != NULL && first->rightChild->rightCount < first->leftChild->leftCount) {
        searcherRight(highest, first->leftChild);
    }

    if((first->leftChild == NULL && first->rightChild == NULL) || (first->leftChild != NULL && first->rightChild == NULL && first->leftCount > first->leftChild->leftCount && first->key > first->leftChild->leftCount)) {
        first->rightChild = highest;
        highest = NULL;
        return;
    } // проблема
}

void searcherRight_childish(int *highest, binarytree *first) {
    if (first == NULL || highest == NULL) {
        return;
    }
    nodesCounter(first);
    if(first->leftChild != NULL && first->leftCount > first->leftChild->leftCount) {
        first->rightChild = highest;
        highest = NULL;
        return;
    }
    if(first->rightChild != NULL) {
        searcherRight(highest, first->rightChild);
    }
    if(first->rightChild != NULL) {
        searcherRight(highest, first->leftChild);
    }
}

void searcherLeft(int *highest, binarytree *first) {
    if (first == NULL || highest == NULL) {
        return;
    }
    nodesCounter(first);
    if(first->rightChild != NULL && first->leftChild != NULL && first->rightChild->rightCount > first->leftChild->leftCount) {
        searcherLeft(highest, first->leftChild);
    }
    if(first->rightChild != NULL && first->leftChild != NULL && first->rightChild->rightCount > first->leftChild->leftCount) {
        searcherLeft(highest, first->rightChild);
    }
    if((first->leftChild == NULL && first->rightChild == NULL) || (first->leftChild == NULL && first->rightChild != NULL && first->rightCount > first->rightChild->rightCount && first->key > first->rightChild->key)) {
        first->leftChild = highest;
        highest = NULL;
        return;
    }
}

void searcherLeft_childish(int *highest, binarytree *first) {
    if (first == NULL || highest == NULL) {
        return;
    }
    nodesCounter(first);
    if(first->rightChild != NULL && first->rightCount > first->rightChild->rightCount) {
        first->leftChild = highest;
        highest = NULL;
        return;
    }
    if(first->leftChild != NULL) {
        searcherLeft(highest, first->leftChild);
    }
    if(first->rightChild != NULL) {
        searcherLeft(highest, first->rightChild);
    }
}

void rightRotate(binarytree **first) {
    nodesCounter(*first);
    int factor = Balancefactor(*first);
    binarytree *highest = *first;
    binarytree *address = (*first)->leftChild;
    (*first)->leftChild = NULL;
    if(factor == 3) {
        searcherRight_childish(highest, address);
    }
    else {
        searcherRight(highest, address);
    }
    *first = address;
}

void leftRotate(binarytree **first) {
    int factor = Balancefactor(*first);
    binarytree *highest = *first;
    binarytree *address = (*first)->rightChild;
    (*first)->rightChild = NULL;
    if(factor == 4) {
        searcherLeft_childish(highest, address);
    }
    else {
        searcherLeft(highest, address);
    }
    *first = address;
}

void balance_tree(binarytree **first) {
    if(*first == NULL) {
        return;
    }
    nodesCounter(*first);
    int factor = Balancefactor(*first);

    if(factor == 1 || factor == 3) {
        rightRotate(first);
        balance_tree(first);
    }
    if(factor == 2 || factor == 4) {
        leftRotate(first);
        balance_tree(first);
    }
    if((*first)->leftChild != NULL && factor == 0) {
        balance_tree(&(*first)->leftChild);
    }
    if((*first)->rightChild != NULL && factor == 0) {
        balance_tree(&(*first)->rightChild);
    }
}

void free_tree(binarytree *first) {
    if (first == NULL) {
        return;
    }
    if(first->leftChild != NULL) {
        free_tree(first->leftChild);
    }
    if(first->rightChild != NULL) {
        free_tree(first->rightChild);
    }
    free(first);
}

int main(void) {
    binarytree *first = create_tree(50);
    insert(30, first);
    insert(70, first);
    insert(78, first);
    insert(79, first);
    insert(67, first);
    insert(62, first);
    insert(69, first);
    insert(61, first);
    insert(64, first);
    insert(66, first);

    print_node_recursive(first, -10);

    balance_tree(&first);

    print_node_recursive(first, -10);

    free_tree(first);

    return 0;
}
