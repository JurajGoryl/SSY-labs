/*
 * uloha.h
 *
 * Created: 2/18/2025 13:47:32
 *  Author: Student
 */ 


#include <stdio.h>
#include <ctype.h>

#define UPPER_CASE 1
#define NORMAL_CASE 2

#define DIRECTION_UP 1
// #define DIRECTION_DOWN 1   // Pokud je odkomentováno, vypíše sestupn?

#define ALPHABET_SIZE 52

char alphabet[ALPHABET_SIZE]; // Pole pro písmena

// Funkce pro generování pole písmen podle režimu
void generateField(int mode) {
	if (mode != UPPER_CASE && mode != NORMAL_CASE) {
		printf("PROGRAM ERROR: Invalid mode in generateField\n");
		return;
	}

	int index = 0;
	if (mode == NORMAL_CASE) {
		for (char c = 'a'; c <= 'z'; c++) alphabet[index++] = c;
		for (char c = 'A'; c <= 'Z'; c++) alphabet[index++] = c;
		} else if (mode == UPPER_CASE) {
		for (char c = 'A'; c <= 'Z'; c++) alphabet[index++] = c;
		for (char c = 'a'; c <= 'z'; c++) alphabet[index++] = c;
	}
}


void capsLetters() {
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (islower(alphabet[i]))
		alphabet[i] = toupper(alphabet[i]);
		else if (isupper(alphabet[i]))
		alphabet[i] = tolower(alphabet[i]);
	}
}


void printField() {
	#if defined(DIRECTION_UP)
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		printf("%c ", alphabet[i]);
	}
	printf("\n");
	#elif defined(DIRECTION_DOWN)
	for (int i = ALPHABET_SIZE - 1; i >= 0; i--) {
		printf("%c ", alphabet[i]);
	}
	printf("\n");
	#else
	printf("PROGRAM ERROR: No direction defined\n");
	#endif
}