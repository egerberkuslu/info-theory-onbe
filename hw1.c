#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#define ALPHABET_COUNT 27

typedef struct letters_s {
	char *first_letter;
	char *second_letter;
	int frequency;
	float probability;
} *letter, letter_t[1];

typedef struct single_letter_s {
	char *letter;
	int frequency;
	float probability;
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
		return 8; //ğ
	case 104:
		return 9; //h
	case -79:
		return 10;  //ı
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
		return 22; //ş
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
		return 16;
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
		return 2;
	}

}

//log2(x) = log10(x) / log10(2)

int main() {

	char alphabet[ALPHABET_COUNT] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
			'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
			'y', 'z',' '};
	FILE *fp;
	single_letter singleLetters = (single_letter) calloc((ALPHABET_COUNT),
			sizeof(single_letter_t));
	int *counter = (int*) malloc(sizeof(int));
	int i, j = 0;
	char *first = (char*) malloc(sizeof(char));
	char *second = (char*) malloc(sizeof(char));
	*counter = 0;
	letter doubleLetters = (letter) malloc(sizeof(letter_t) * (ALPHABET_COUNT * ALPHABET_COUNT));
	j = 0;

	for (i = 0; i < ALPHABET_COUNT; i++) {
		(singleLetters + i)->letter = alphabet + i;
		(singleLetters + i)->frequency = 0;
		(singleLetters + i)->probability = 0.0;

	}
	for (i = 0; i < ALPHABET_COUNT * ALPHABET_COUNT; i++) {
		(doubleLetters + i)->first_letter = alphabet + (i / ALPHABET_COUNT);
		(doubleLetters + i)->second_letter = alphabet + (i % ALPHABET_COUNT);
		(doubleLetters + i)->frequency = 0;
		(doubleLetters + i)->probability = 0.0;

	}

	i = 0;
	int ascii_first, ascii_second = -1;
	fp = fopen("/home/ege/eclipse-workspace-c/hw1/src/hw.txt", "r");

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
		}
		fseek(fp, -1, SEEK_CUR);
		i++;
	}





	for (j = 0; j < ALPHABET_COUNT; j++) {

		(singleLetters + j)->probability = (float)((singleLetters + j)->frequency) / (*counter)*100;
		printf("%c => FREQUENCY %d  PROBABILITY %f  \n", *(singleLetters + j)->letter,
				(singleLetters + j)->frequency,(singleLetters + j)->probability);
	}


	for (i = 0; i < ALPHABET_COUNT * ALPHABET_COUNT; i++) {
		printf("%c%c => %d \n",
				*(doubleLetters + i)->first_letter,
				*(doubleLetters + i)->second_letter, doubleLetters[i].frequency);
	}
	printf("TOTAL LETTERS %d",*counter);

	return 0;
}
