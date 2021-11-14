#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#define ALPHABET_COUNT 27
typedef struct Node_s *Node;
typedef struct Huffman_Node_s *Huffman_Node;
char **lookUpTable;
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

void printTable(char **codeTable,char alphabet[ALPHABET_COUNT]){
	int i;
	printf("\n PRINTING LOOUK UP TABLE \n");
	for(i=0;i<ALPHABET_COUNT;i++) {
		printf("\n %c %s \n", alphabet[i] ,codeTable[i]);
	}
}
int isLeaf(Huffman_Node root) {
	if( root->leftChild == NULL && root->rightChild == NULL) {
		return 1;
	} else {
		return 0;
	}
}

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
    lookUpTable[controlChar(&root->letter)] = &root->codes;
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


int writeBit(int bit,int *counter,unsigned char *byte,int forceWrite,FILE *fp) {
	if(*counter < 8) {
		if(!forceWrite) {
			if(*counter == 7) {
				*byte = *byte + bit;
				fwrite(byte,sizeof(char),1,fp);
				*byte = *byte - *byte; 
				*counter = -1;
				return 1;
			} else {
			
				*byte = *byte + bit;
				*byte = (*byte)<<1;
				return 1;
			}	
		} else { 
				*byte = *byte + bit;
				
				*byte = *byte - *byte; 
				fwrite(byte,sizeof(char),1,fp);
				*counter = -1;
				return 1;
			}
	} else {
		return 0; 
	}
}

void compress(char letter,unsigned char *byte,int *counter,int value,int forceWrite,FILE *fp) {
	int i = 0;
	char *str = "";
	
		str = lookUpTable[value];
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
	for(i=0;i<ALPHABET_COUNT;i++) {
		//printf("CONTROL LOOK UP TABLE, LOOKUPTABLE %s  , OKUNAN CODE %s \n",lookUpTable[i], code);
		if(!strcmp(lookUpTable[i],code)) { //strcmp buldu esit diye
			//printf("BULDUM !!!!!!!! \n ");
			return i;
		}
	}
	return -1;
}

void decompress(unsigned char *byte,char letters[ALPHABET_COUNT],char **code,int readableBits,FILE *decompFile) {
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
		if(control != -1) {
			fwrite(&letters[control],sizeof(char),1,decompFile);
			*code = "";
		}
		counter = counter - 1;
		shifter = shifter -1;
		copyByte = *byte;
	}
}

typedef struct letters_s {
	char *first_letter;
	char *second_letter;
	int frequency;
	double probability;
	double entropty;
} *letter, letter_t[1];

typedef struct single_letter_s {
	char *letter;
	int frequency;
	double probability;
	double entropty;
} *single_letter, single_letter_t[1];

int controlCharTurkish(char *letter) {
	switch (*letter) {
	case 97:
		return 0; //a
	case 98:
		return 1; //b
	case 99:
		return 2; //c
	case -89:
		return 3; //ç
	case 100:
		return 4; //d
	case 101:
		return 5; //e
	case 102:
		return 6; //f
	case 103:
		return 7; //g
	case -97:
		return 8; //ð
	case 104:
		return 9; //h
	case -79:
		return 10;  //ý
	case 105:
		return 11; //i
	case 106:
		return 12; //j
	case 107:
		return 13; //k
	case 108:
		return 14; //l
	case 109:
		return 15; //m
	case 110:
		return 16; //n
	case 111:
		return 17; //o
	case -74:
		return 18; //ö
	case 112:
		return 19; //p
	case 114:
		return 20; //r
	case 115:
		return 21; //s
	case -59:
		return 22; //þ
	case 116:
		return 23; //t
	case 117:
		return 24; //u
	case -68:
		return 25; //ü
	case 118:
		return 26; //v
	case 121:
		return 27; //y
	case 122:
		return 28; //z
	default:
		printf("UNDETECTED %c ASCII %d \n", *letter, *letter);
		return -1;
	}

}
int controlChar(char *letter) {
	switch (*letter) {
	case 97:
		return 0; //a
	case 98:
		return 1; //b
	case 99:
		return 2; //c
	case 100:
		return 3; //d
	case 101:
		return 4; //e
	case 102:
		return 5; //f
	case 103:
		return 6; //g
	case 104:
		return 7; //h
	case 105:
		return 8; //i
	case 106:
		return 9; //j
	case 107:
		return 10; //k
	case 108:
		return 11; //l
	case 109:
		return 12; //m
	case 110:
		return 13; //n
	case 111:
		return 14; //o
	case 112:
		return 15; //p
	case 113:      
		return 16; //q
	case 114:
		return 17; //r
	case 115:
		return 18; //s
	case 116:
		return 19; //t
	case 117:
		return 20; //u
	case 118:
		return 21; //v
	case 119:
		return 22; //w
	case 120:
		return 23; //x
	case 121:
		return 24; //y
	case 122:
		return 25; //z
	case 32:
		return 26;
	default:
		printf("UNDETECTED %c ASCII %d \n", *letter, *letter);
		return -1;
	}

}

void initLookUpTable() {
	int i = 0;
	for(i=0;i<ALPHABET_COUNT;i++) {
		lookUpTable[i] = "";
		printf("LOOK UP TALBE EGE : %s", lookUpTable[i]);
	}
}
//log2(x) = log10(x) / log10(2)

int main() {

	char alphabet[ALPHABET_COUNT] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
			'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u',
			'v', 'w', 'x', 'y', 'z', ' ' };
	FILE *fp;
	single_letter singleLetters = (single_letter) calloc((ALPHABET_COUNT),
			sizeof(single_letter_t));
	int *counter = (int*) malloc(sizeof(int));
	int i, j = 0;
	char *first = (char*) malloc(sizeof(char));
	char *second = (char*) malloc(sizeof(char));
	int *doubleLetterEachGroupFreq = (int*) calloc((ALPHABET_COUNT),
			sizeof(int));
	*counter = 0;
	letter doubleLetters = (letter) malloc(
			sizeof(letter_t) * (ALPHABET_COUNT * ALPHABET_COUNT));
	j = 0;

	for (i = 0; i < ALPHABET_COUNT; i++) {
		(singleLetters + i)->letter = alphabet + i;
		(singleLetters + i)->frequency = 0;
		(singleLetters + i)->probability = 0.0;
		(singleLetters + i)->entropty = 0.0;

	}
	for (i = 0; i < ALPHABET_COUNT * ALPHABET_COUNT; i++) {
		(doubleLetters + i)->first_letter = alphabet + (i / ALPHABET_COUNT);
		(doubleLetters + i)->second_letter = alphabet + (i % ALPHABET_COUNT);
		(doubleLetters + i)->frequency = 0;
		(doubleLetters + i)->probability = 0.0;
		(doubleLetters + i)->entropty = 0.0;
	}

	i = 0;
	int ascii_first, ascii_second = -1;
	fp = fopen("input.txt", "r");

	while (1) {
		(*counter)++;
		fread(first, 1, 1, fp);

		fread(second, 1, 1, fp);
		ascii_first = controlChar(first);
		ascii_second = controlChar(second);
		if (feof(fp)) {
			if (ascii_first != -1) {
				(singleLetters + (ascii_first))->frequency++;
			}
			break;
		}
		if (ascii_first != -1) {
			(singleLetters + (ascii_first))->frequency++;
		}
		if (ascii_second != -1) {
			(doubleLetters + (ascii_first * ALPHABET_COUNT) + ascii_second)->frequency++;
			(*(doubleLetterEachGroupFreq + ascii_first))++;
		}
		fseek(fp, -1, SEEK_CUR);
		i++;
	}
	printf("\n\n TABLE 1 !!!! \n\n");
	for (j = 0; j < ALPHABET_COUNT; j++) {
		//log2(x) = log10(x)/ log10(2)
		(singleLetters + j)->probability =
				(double) ((singleLetters + j)->frequency) / (*counter);
		(singleLetters + j)->entropty = (-1)*(singleLetters + j)->probability
				* ((double) log((singleLetters + j)->probability)
						/ (double) log(2));

		printf("%c => FREQUENCY %d  PROBABILITY %f ENTROPY %f \n",
				*(singleLetters + j)->letter, (singleLetters + j)->frequency,
				(singleLetters + j)->probability,
				(singleLetters + j)->entropty);
	}
	printf("\n\n TABLE 2 !!!! \n\n");
	for (i = 0; i < ALPHABET_COUNT * ALPHABET_COUNT; i++) {
		(doubleLetters + i)->probability = (double) ((doubleLetters + i)->frequency) / *(doubleLetterEachGroupFreq+(i/ALPHABET_COUNT));
		(doubleLetters + i)->entropty = (-1)*((double) (doubleLetters + i)->probability) * ((double) log((doubleLetters + i)->probability)/(double) log(2));
		printf("%c%c => FREQUENCY %d  PROPABILITY %f ENTROPY %f \n", *(doubleLetters + i)->first_letter,
				*(doubleLetters + i)->second_letter,
				(doubleLetters + i)->frequency, (doubleLetters + i)->probability,(doubleLetters + i)->entropty);
	}
	printf("TOTAL LETTERS %d", *counter);

	printf("\n\n TABLE 3 !!!! \n\n");
	for (i = 0; i < ALPHABET_COUNT * ALPHABET_COUNT; i++) {
			printf("%c/%c =>  ENTROPY %f \n", *(doubleLetters + i)->first_letter,
					*(doubleLetters + i)->second_letter,((singleLetters + (i/ALPHABET_COUNT))->entropty)-(doubleLetters + i)->entropty);
	}
	
	//huffman baslar
	lookUpTable = (char **)malloc(sizeof(char *) *ALPHABET_COUNT);
	initLookUpTable();
	
	char codeTable[ALPHABET_COUNT];
	Queue q = initQueue();
	int curFreq =0;
	for(i=0;i<ALPHABET_COUNT;i++) {
		curFreq = singleLetters[i].frequency;
		if(curFreq != 0) {
			Node newNode = initNode();
			newNode->h_node->freq = (singleLetters + i)->frequency;
			newNode->h_node->letter = (singleLetters[i].letter)[0];
			enqueue(q,newNode);
		}
		
	}
	
	printf("\n BUILDING HUFFMAN TREE \n");
	huffmanTree(q);
	printInorder(q->top->h_node);
	char *str = "";
	//encode(q->top->h_node,&str,codeTable);
	printf("\n PRINTING HUFFMAN CODES \n");
	printHCodes(q->top->h_node, codeTable, 0);
	printTable(lookUpTable,alphabet);
	
	unsigned char byte = 'a';
	int readCounter = 0;
	byte = byte - byte;
	i = 0;
	fp = fopen("input.txt", "r");
	FILE* binaryFile = fopen("output.bin", "wb");
	fseek(fp, 0, SEEK_END);
	int readableFileLen = ftell(fp);
	rewind(fp); 
	for(i=0;i<readableFileLen;i++) {
		fread(first, 1, 1, fp);
		ascii_first = controlChar(first);
		compress(alphabet[ascii_first],&byte,&readCounter,ascii_first,0,binaryFile);
	}
	int readableBits;
	if(readCounter == 0) { // baska byte kalmadi
		readableBits = 0;
	} else {
		byte = byte << (7-readCounter); //geriye kalan bitleri yazdirdim/
		//printf("unutulan OLAN BYTE %d \n\n ", byte);
		fwrite(&byte,sizeof(unsigned char),1,binaryFile);
		readableBits = readCounter-1;
	}

	fclose(fp);
	fclose(binaryFile);
	fp = fopen("output.bin", "rb");
	fseek(fp, 0, SEEK_END);
	int filelen = ftell(fp);
	rewind(fp); 
	unsigned char harf = 'z';
	char *code = "";
	int count;
	i = 0;
	int k,l;
	
	FILE *decompFile = fopen("decompressed.txt", "a");
	for(i=0;i<filelen;i++){
		if(i == filelen-1){
			if(readableBits != 0) {
				fread(&harf,sizeof(char),1,fp);
				decompress(&harf,alphabet,&code,readableBits,decompFile);
			} else {
				fread(&harf,sizeof(char),1,fp);
				decompress(&harf,alphabet,&code,7,decompFile);
			}
		} else {
			fread(&harf,sizeof(char),1,fp);
			decompress(&harf,alphabet,&code,7,decompFile);
		}
		
	}
	
	fclose(decompFile);
	fclose(fp);
	printf("\nDONE \n");
	
	
	return 0;
}
