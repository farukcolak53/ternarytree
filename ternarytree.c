 //Ahmet Faruk Çolak
 
 #include <stdio.h>
 #include <dirent.h>
 #include <stdlib.h>
 
struct TSTNodeType{
	int data;
	struct TSTNodeType *right;
	struct TSTNodeType *left;
	struct TSTNodeType *middle;
};
typedef struct TSTNodeType TSTNodeType; 

int depthLevel;
	
void insertKey(TSTNodeType **root, int key){
	
	//Base Case
	if (*root == NULL) {
		TSTNodeType *temp = malloc(sizeof(TSTNodeType)); 
   		temp->data = key;  
    	temp->left = NULL;
		temp->middle = NULL;
		temp->right = NULL; 
		
		*root = temp;
		return;
	}
	 
	// If the key is smaller than root's data, 
    // then insert this key in left subtree of the root 
    if (key < (*root)->data){
    	insertKey(&((*root)->left), key); 
	}	

	else if ((key <= ((*root)->data)*(*root)->data) && (key > ((*root)->data))) 
        insertKey(&((*root)->middle), key);
        
	// If the key is greater than root's data, 
    // then insert this key in right subtree of the root 
	else if ((key > ((*root)->data)*(*root)->data)) 
        insertKey(&((*root)->right), key); 
}

//Returns the minimum value of the root's middle subtree
TSTNodeType* minValueNodeOfMid(TSTNodeType **middleRoot){
	
	TSTNodeType *current = *middleRoot;
	
	while(current->left != NULL){
			current = current->left;
	}
	return current;  
}

//Returns the minimum value of the root's right subtree
TSTNodeType* minValueNodeOfRight(TSTNodeType **rightRoot){
	
	TSTNodeType *current = *rightRoot;
	
	while(current->left != NULL){
			current = current->left;
	}
	return current; 
}

//Returns the maximum value of the root's left subtree
TSTNodeType* maxValueNodeOfLeft(TSTNodeType **leftRoot){
	
	TSTNodeType *current = *leftRoot;
	
	while(current != NULL){
		if(current->right != NULL){
			current = current->right;
		}
		else if(current->middle != NULL){
			current = current->middle;
		}
		else
			return current;
	}
	return current; 
}

//Inserts the middle subtree of root into the left subtree of the root(For 3 child case)
void reorderOfMidTree(TSTNodeType *leftSubtree, TSTNodeType *midTemp) {
	
    if (midTemp != NULL){
    	
        insertKey(&leftSubtree, midTemp->data);
        reorderOfMidTree(leftSubtree, midTemp->left);
        reorderOfMidTree(leftSubtree, midTemp->middle);
        reorderOfMidTree(leftSubtree, midTemp->right);
    }
}

TSTNodeType* removeKey(TSTNodeType **root, int key){
	
	//Base case
	if(*root == NULL){
		printf("%d is not in the tree\n", key);
		return *root;
	}
	
	// If the key to be removed is smaller than the root's data, 
    // then it lies in left subtree 
    if(key < (*root)->data)
    	(*root)->left = removeKey(&(*root)->left, key);	
	else if ((key <= ((*root)->data)*(*root)->data) && (key > ((*root)->data)))
		(*root)->middle = removeKey(&(*root)->middle, key); 	
	else if ((key > ((*root)->data)*(*root)->data)) 
    	(*root)->right = removeKey(&((*root)->right), key); 
    
	// if key is same as root's data, then This is the node 
    // to be removed    
    else{
    	// node with no child (leaf)
    	if((*root)->left == NULL && (*root)->middle == NULL && (*root)->right == NULL){
    		free(*root);
    		*root = NULL;
    		return *root;
		}
		
		//Node with one child
		if((*root)->left == NULL && (*root)->middle == NULL) {
    		TSTNodeType *temp = (*root)->right; //Holds the rest of the subtree and removes the corresponding key
    		free(*root);
    		*root = NULL;
    		return temp;
		}
		
    	else if((*root)->left == NULL && (*root)->right == NULL) {
    		TSTNodeType *temp = (*root)->middle;
    		free(*root);
    		*root = NULL;
    		return temp;
		}
		
		else if((*root)->middle == NULL && (*root)->right == NULL){
    		TSTNodeType *temp = (*root)->left;
    		free(*root);
    		*root = NULL;
    		return temp;
		}
		
		//Node with two child
		if((*root)->right == NULL){
			TSTNodeType *temp = minValueNodeOfMid(&(*root)->middle); //Gets the minimum value of middle subtree
			(*root)->data = temp->data;
			(*root)->middle = removeKey(&(*root)->middle, temp->data); //Returns the organized middle subtree
		}
		
		else if((*root)->middle == NULL){
			TSTNodeType *temp = maxValueNodeOfLeft(&(*root)->left);
			(*root)->data = temp->data;
			(*root)->left = removeKey(&(*root)->left, temp->data);
		}
		
		else if((*root)->left == NULL){
			TSTNodeType *temp = minValueNodeOfRight(&(*root)->right); //Gets the maximum value of the right subtree
			TSTNodeType *temp2 = (*root)->right->right; 
			
			(*root)->data = temp->data;
			(*root)->right = removeKey(&(*root)->right, temp->data); //Returns the organized right subtree
			(*root)->left = (*root)->middle; //Middle subtree is moved to left subtree
			(*root)->middle = NULL; 
						
			(*root)->right->right = NULL;
			(*root)->middle = (*root)->right; //Right subtree is moved to middle subtree
			(*root)->right = NULL;
			
			if(temp2 != NULL){ //If the right subtree of the root's right subtree is not null, 
				(*root)->right = temp2; //It is moved to right subtree
				temp2 = NULL;
			}
		}
		
		//Node with three child
		else if((*root)->left != NULL && (*root)->middle != NULL && (*root)->right != NULL){
			TSTNodeType *temp = minValueNodeOfRight(&(*root)->right); //Gets the minimum value of the right subtree
			TSTNodeType *tempMid = (*root)->middle;	
			TSTNodeType *temp2 = (*root)->right->right; 
			(*root)->right->right = NULL;
			
			(*root)->data = temp->data;
			(*root)->right = removeKey(&(*root)->right, temp->data);  //Returns the organized right subtree
				 
			reorderOfMidTree((*root)->left, tempMid); //Reorganizes the middle subtree(Inside the function it inserts the middle into the left)
						
			(*root)->middle = (*root)->right;
			(*root)->right = NULL;
			
			if(temp2 != NULL){
				(*root)->right = temp2;
				temp2 = NULL;
			}
		}
	} 
	return *root;  
}

int findKey(TSTNodeType **root, int key){
	
	//Base Case
	if(*root == NULL){
		printf("Looked for %d but it is not in the tree\n", key);
		return 1;
	}
	
	if(*root != NULL){
		if(key < (*root)->data){
			depthLevel++;
			findKey(&(*root)->left, key);
		}

		else if ((key > ((*root)->data)*(*root)->data)) {
			depthLevel++;
			findKey(&(*root)->right, key);	
		}
			
		else if ((key <= ((*root)->data)*(*root)->data) && (key > ((*root)->data))) {
			depthLevel++;
			findKey(&(*root)->middle, key);	
		}
			
		else if((*root)->data == key){
			printf("%d has found at depth level %d\n", key, depthLevel);
			return 1;
		}
	}
}

void print_preorder(TSTNodeType * tree) {
    if (tree != NULL){
    	
        printf("%d\n",tree->data);
        print_preorder(tree->left);
        print_preorder(tree->middle);
        print_preorder(tree->right);
    }
}

void print_inorder(TSTNodeType * tree) {
    if (tree != NULL){
    	
        print_inorder(tree->left);
        printf("%d\n",tree->data);
        print_inorder(tree->middle);
        print_inorder(tree->right);
    }
}

void print_postorder(TSTNodeType * tree) {
    if (tree != NULL) {
    	
        print_postorder(tree->left);
        print_postorder(tree->middle);
        print_postorder(tree->right);
        printf("%d\n",tree->data);
    }
}

int main(){
	
	TSTNodeType *root = NULL; 
	FILE *fp;
	int number;
	int option;
	int num;
	
	fp = fopen("input.txt","r");
	
	while (!feof(fp)) { 
		fscanf(fp,"%d", &number); 
    	insertKey(&root, number);
	}
    
    printf("Tree has created with the given input file\n");
    
    do {
    	printf("--------------------------------------------\n");
    	printf("Please choose an option\n1: find, 2: insert, 3:remove, 4: pre order display, 5: in order display, 6: post order display, 0: exit\n");
    	scanf("%d",&option);
		switch (option) { 
			case 1: printf("--------------------------------------------\n");
					  printf("Find a number: ");
					  scanf("%d", &num);
					  depthLevel = 0;
					  findKey(&root, num);
					  break;
	    	case 2: printf("--------------------------------------------\n");
					  printf("Insert a number: ");
					  scanf("%d",&num); 
					  insertKey(&root, num); 
					  printf("%d inserted\n", num);
	                  break;
	        case 3: printf("--------------------------------------------\n");
			 		  printf("Remove a number: ");
					  scanf("%d",&num); 
					  removeKey(&root, num);
					  printf("%d removed\n", num);
	                  break;
			case 4: printf("--------------------------------------------\n");
					  printf("Pre Order Display\n");
	    			  print_preorder(root); 
	                  break;
	        case 5: printf("--------------------------------------------\n");
					  printf("In Order Display\n");
	   			      print_inorder(root);
	   				  break;
	        case 6: printf("--------------------------------------------\n");
			 		  printf("Post Order Display\n");
	   				  print_postorder(root);
	   				  break;
	        case 0: printf("--------------------------------------------\n");
					  printf("Program is over!");
					  exit(1);
			default : printf("Invalid operation\n");
					  break;		  
		}
  	}while (option != 0);
  	
  	return 1;
}
