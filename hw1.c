#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct letters_s {
	char *first_letter;
	char *second_letter;
	int value;
}*letter,letter_t[1];

int controlChar(char *letter) {
	switch(*letter) {
		case 97: 
			return 0; //a
		case 98 : 
			return 1; //b
		case 99 :
			return 2; //c
		case 135:
			return 3; //ç
		default:
			printf("UNDETECTED %c ASCII %d \n",*letter,*letter);
			return -1;
	}
	
}

//log2(x) = log10(x) / log10(2)


int main() {
	char alphabet[30] ={ 'a', 'b', 'c', 'ç', 'd', 'e', 'f', 'g', 'ð', 'h', 'ý', 'i', 'j', 'k', 'l','m','n',
        'o','ö','p','r','s','þ','t','u','ü','v','y','z'};
	FILE *fp;
	int* letterFrequency = (int *)calloc(30,sizeof(int));
	int* counter = (int*)malloc(sizeof(int));
	int i,j = 0;
	char first,second;
	*counter = 0;
	letter doubleLetters = (letter)malloc(sizeof(letter_t)*900);
	j = 0;
	int k = 0;
	for(i = 0;i< 900;i++){
		(doubleLetters+i)->first_letter = alphabet+(i/30);			
		(doubleLetters+i)->second_letter = alphabet+(i%30);
		(doubleLetters+i)->value = 0;		
	}
		
	i = 0;
	int index = 0;
	fp = fopen("hw.txt", "r");
	
	while (1) {
		fread(&first, 1, 1, fp);
		fread(&second, 1, 1, fp);
		if (feof(fp)) {
			letterFrequency[controlChar(&first)]++;
			(doubleLetters+(controlChar(&first)*30) + controlChar(&second))->value++;
			break;
		}
		letterFrequency[controlChar(&first)]++;
		(doubleLetters+(controlChar(&first)*30) + controlChar(&second))->value++;
		fseek(fp, -1, SEEK_CUR);
		i++;
	}
	for(j=0;j < 30;j++) {
		printf("count %c is %d \n",'a'+j,letterFrequency[j]);
	}
		printf("count %c is %d \n",135, letterFrequency[3]);
	

	
	
	
	
	for(i = 0;i< 90;i++){
		printf("Letter %d is : %c, second is : %c , value is %d \n",i,*(doubleLetters+i)->first_letter,*(doubleLetters+i)->second_letter,doubleLetters[i].value);
	}
	
	
	return 0;
}
