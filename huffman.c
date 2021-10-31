#include <stdio.h>
#include <stdlib.h>

#define SIZE 6

typedef struct Node_s *Node;
typedef struct Huffman_Node_s *Huffman_Node;

typedef struct Node_s { 
	Huffman_Node h_node;
	Node next;
}Node_t[1];

typedef struct Huffman_Node_s{
	Huffman_Node leftChild;
	Huffman_Node rightChild;
	int freq; //freq = prio
	char letter;
	char *codes;
}Huffman_Node_t[1];


typedef struct queue_s {
	Node top;
	Node tail;
	int size;
}*Queue,Queue_t[1];


Queue initQueue() {
	Queue qu = (Queue)malloc(sizeof(Queue_t));
	qu->size = 0;
	qu->top = NULL;
	return qu;
}

int isEmpty(Queue qu) {
	if(qu->top == NULL || qu->size == 0) {
		return 1;
	} else {
		return 0;
	}
}

Node initNode() {
	Node n = (Node)malloc(sizeof(Node_t));
	n->h_node = (Huffman_Node)malloc(sizeof(Huffman_Node_t));
	n->h_node->freq = 0;
	n->h_node->letter = 'a';
	n->h_node->leftChild = NULL;
	n->h_node->rightChild = NULL;
	n->next = NULL;
	n->h_node->codes = "";
	return n;
}

void enqueue(Queue q, Node newNode) {
	
	if(!isEmpty(q)) {
		if(newNode->h_node->freq < q->top->h_node->freq) {
			newNode->next = q->top;
			q->top = newNode;
			q->size = q->size + 1;
		} else {
			Node prevTemp = q->top;
			Node temp = q->top->next;
			while(temp != NULL && temp->h_node->freq < newNode->h_node->freq) {
				prevTemp = temp;
				temp = temp->next;
			}
			if(temp == NULL) {
				prevTemp->next = newNode;
				q->tail = newNode;
				q->size = q->size + 1;
			} else {
				newNode->next = temp;
				prevTemp->next = newNode;
				q->size = q->size + 1;
			}
		}
	} else {
		q->top = newNode;
		q->tail = newNode;
		q->size = 1;
	}
}

Node dequeu(Queue q) {
	if(isEmpty(q)) {
		printf("\n !!!Queue is Empty!!! \n");
		return;
	} else {
		Node topNode = initNode();
		topNode->h_node->freq = q->top->h_node->freq;
		topNode->h_node->letter = q->top->h_node->letter;
		topNode->h_node->leftChild = q->top->h_node->leftChild;
		topNode->h_node->rightChild = q->top->h_node->rightChild;
		topNode->next = NULL;
		Node temp = q->top;
		q->top = q->top->next;
		free(temp);
		q->size = q->size - 1;
		return topNode;
	}
}

void printQueue(Queue q) {
	int size = q->size;
	Node traversal = q->top;
	printf("\n Size of q is %d \n", q->size);
	while(size != 0) {
		printf(" %d ",traversal->h_node->freq);
		traversal = traversal->next;
		size--;
	}
}

Huffman_Node initHuffNode() {
	Huffman_Node n = (Huffman_Node)malloc(sizeof(Huffman_Node_t));
	n->freq = 0;
	n->letter = 'a';
	n->leftChild = NULL;
	n->rightChild = NULL;
	n->codes = "";
	return n;
}

void huffmanTree(Queue q) {
	while(q->size > 1) {
		Huffman_Node leftChild = initHuffNode();
		leftChild = q->top->h_node;
		dequeu(q);
		Huffman_Node rightChild = initHuffNode();
		rightChild = q->top->h_node;
		dequeu(q);
		Node newNode = initNode();
		newNode->h_node->freq = leftChild->freq + rightChild->freq;
		newNode->h_node->leftChild = leftChild;
		newNode->h_node->rightChild = rightChild;
		enqueue(q,newNode);
		printQueue(q);
	}
}

void printTable(char **codeTable) {
	int i;
	printf("\n PRINTING LOOUK UP TABLE \n");
	for(i=0;i<SIZE;i++) {
		printf("\n %c %s \n", 97+i ,&codeTable[i]);
	}
}
int isLeaf(Huffman_Node root) {
	if( root->leftChild == NULL && root->rightChild == NULL) {
		return 1;
	} else {
		return 0;
	}
}

void encode(Huffman_Node root, char** str, int codeTable[])
{
    if (root == NULL) {
        return;
    }
 
    // found a leaf node
    if (isLeaf(root)) {
        codeTable[root->letter] = "1";
    }
    encode(root->leftChild, cats(&str,"0"), codeTable);
    encode(root->rightChild, cats(&str,"1"), codeTable);
}
char **lookUpTable;

void printHCodes(Huffman_Node root, char arr[], int top) {
  if (root->leftChild) {
    arr[top] = '0';
    printHCodes(root->leftChild, arr, top + 1);
  }
  if (root->rightChild) {
    arr[top] = '1';
    printHCodes(root->rightChild, arr, top + 1);
  }
  if (isLeaf(root)) {
    printf("  %c   | ", root->letter);
    printArray(arr, top,root->letter,root);
    printf("CODE %d OF THIS %s \n",root->letter%97,&root->codes);
    lookUpTable[root->letter%97] = root->codes;
  }
}


void printArray(char arr[], int n,char c,Huffman_Node root) {
  int i;
  FILE *fp;
  fp = fopen("lookup.txt", "a");
  fprintf(fp,"%c |", c);
  for (i = 0; i < n; ++i) {
  	fprintf(fp," %c",arr[i]);
  	strncat(&root->codes,&arr[i],1);
  }
  printf("%s \n",&root->codes);
  
  fprintf(fp,"\n");
  printf("\n");
  fclose(fp);
}

void printInorder(Huffman_Node node)
{
    if (node == NULL)
        return;
 
    /* first recur on left child */
    printInorder(node->leftChild);
 
    /* then print the data of node */
    printf("%d ", node->freq);
 
    /* now recur on right child */
    printInorder(node->rightChild);
}





void cats(char **str, const char *str2) {
    char *tmp = NULL;

    // Reset *str
    if ( *str != NULL && str2 == NULL ) {
        free(*str);
        *str = NULL;
        return;
    }

    // Initial copy
    if (*str == NULL) {
        *str = calloc( strlen(str2)+1, sizeof(char) );
        memcpy( *str, str2, strlen(str2) );
    }
    else { // Append
        tmp = calloc( strlen(*str)+1, sizeof(char) );
        memcpy( tmp, *str, strlen(*str) );
        *str = calloc( strlen(*str)+strlen(str2)+1, sizeof(char) );
        memcpy( *str, tmp, strlen(tmp) );
        memcpy( *str + strlen(*str), str2, strlen(str2) );
        free(tmp);
    }

} // cats


int writeBit(int bit,int *counter,unsigned char *byte,int forceWrite,FILE *fp) {
	if(*counter < 8) {
		if(!forceWrite) {
			if(*counter == 7) {
				//write the text file;
				*byte = *byte + bit;
				printf("YAZILACAK OLAN BYTE %d \n ", *byte);
				fwrite(byte,sizeof(char),1,fp);
				//printf bytei yazdigimi temsil ediyor
				*byte = *byte - *byte; // byte 0landi
				*counter = -1;
				return 1;
			} else {
				//add bit to byte
				*byte = *byte + bit;
				*byte = (*byte)<<1;
				return 1;
			}	
		} else { 
				*byte = *byte + bit;
				printf("FORCE RUN AKTIF YAZILACAK OLAN BYTE %d \n ", *byte);
				//printf bytei yazdigimi temsil ediyor
				*byte = *byte - *byte; // byte 0landi
				fwrite(byte,sizeof(char),1,fp);
				*counter = -1;
				return 1;
			}
		
	} else {
		return 0; //mean can not fit 1 byte error with writing
	}
	
}

void compress(char letter,unsigned char *byte,int *counter,int value,int forceWrite,FILE *fp) {
	int i = 0;
	char *str = "";
	
		str = &lookUpTable[value];
		for(i = 0 ; i< strlen(str);i++){
			if(str[i] == '0') {
				
				if(writeBit(0,counter,byte,forceWrite,fp)) {
					*counter = *counter + 1;
				} 
			} else {
			
				if(writeBit(1,counter,byte,forceWrite,fp)) {
					*counter = *counter + 1;
				} 		
			}
		}
	
}

int controlLookUpTable(char **code) {
	int i = 0;
	for(i=0;i<SIZE;i++) {
		if(!strcmp(&lookUpTable[i],code)) { //strcmp buldu esit diye
			return i;
		}
	}
	return -1;
}

void decompress(unsigned char *byte,char letters[SIZE],char **code,int readableBits) {
	int counter = readableBits;
	int shifter = 7;
	char copyByte = *byte;
	char bit;
	int control;
	char writableBit = '1';
	while(counter >= 0) {
		bit = (copyByte >> shifter) & 1;
		if(bit) {
			writableBit = '1';
			strncat(code,&writableBit,1);
		} else {
			writableBit = '0';
			strncat(code,&writableBit,1);
		}
		control = controlLookUpTable(code);
		if(control != -1) { //buldu
			printf("%c ",letters[control]);
			*code = "";
		}
		counter = counter - 1;
		shifter = shifter -1;
		copyByte = *byte;
	}
	printf("ILK BYTE BITTI \n\n\n");
}

int main() {
	lookUpTable = (char *)malloc(sizeof(char ) *SIZE);
	char codeTable[SIZE];
	Queue q = initQueue();
	Node ege = initNode();
	ege->h_node->freq = 9;
	ege->h_node->letter = 'a';
	Node pervin = initNode();
	pervin->h_node->freq = 7;
	pervin->h_node->letter = 'b';
	Node ender = initNode();
	ender->h_node->freq = 10;
	ender->h_node->letter = 'c';
	Node hasan = initNode();
	hasan->h_node->freq = 8;
	hasan->h_node->letter = 'd';
	Node onur = initNode();
	onur->h_node->freq = 25;
	onur->h_node->letter = 'e';
	Node mehmet = initNode();
	mehmet->h_node->freq = 13;
	mehmet->h_node->letter = 'f';
	enqueue(q,ege);
	printQueue(q);
	enqueue(q,pervin);
	printQueue(q);
	enqueue(q,ender);
	printQueue(q);
	enqueue(q,hasan);
	printQueue(q);
	enqueue(q,onur);
	printQueue(q);
	enqueue(q,mehmet);
	printQueue(q);
	printf("\n BUILDING HUFFMAN TREE \n");
	huffmanTree(q);
	printf("\n INDORDER TRAVELS \n");
	printInorder(q->top->h_node);
	char *str = "";
	//encode(q->top->h_node,&str,codeTable);
	printf("\n PRINTING HUFFMAN CODES \n");
	printHCodes(q->top->h_node, codeTable, 0);
	printTable(lookUpTable);
	
	char letters[SIZE] = {'a','b','c','d','e','f'};
	unsigned char byte = 'a';
	int  counter = 0;
	byte = byte - byte;
	int i = 0;
	FILE * fp = fopen("output.bin", "wb");
	
	for(i = 0 ; i < SIZE; i++) {
		compress(letters[i%SIZE],&byte,&counter,i%SIZE,0,fp);
	}
	
	for(i = SIZE-1; i >= 0; i--) {
		compress(letters[i],&byte,&counter,i,0,fp);
	}
	int readableBits;
	if(counter == 0) { // baska byte kalmadi
		readableBits = 0;
	} else {
		byte = byte << (7-counter); //geriye kalan bitleri yazdirdim/
		printf("unutulan OLAN BYTE %d \n\n ", byte);
		fwrite(&byte,sizeof(unsigned char),1,fp);
		readableBits = counter-1;
	}
	
	
	fclose(fp);
	
	fp = fopen("output.bin", "rb");
	fseek(fp, 0, SEEK_END);
	int filelen = ftell(fp);
	rewind(fp); 
	unsigned char harf = 'z';
	char *code = "";
	int count;
	i = 0;
	for(i=0;i<filelen;i++){
		if(i == filelen-1){
			if(readableBits != 0) {
				fread(&harf,sizeof(char),1,fp);
				decompress(&harf,letters,&code,readableBits);
			} else {
				fread(&harf,sizeof(char),1,fp);
				decompress(&harf,letters,&code,7);
			}
		} else {
			fread(&harf,sizeof(char),1,fp);
			decompress(&harf,letters,&code,7);
		}
		
	}
	// A   B   C    C D   E E D C B A
    //110 100 11 || 1 101 0 1 11 01011111 00110000
	
	

	
}


