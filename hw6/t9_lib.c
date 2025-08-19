#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "t9_lib.h"

// Helper method to get T9 number from letter
int charToInt(char letter);

// T9 structure contains word for the given node
// and pointers to children T9 structures, one for
// each valid input, 2-9 and #
typedef struct T9 {
  char* word;
  struct T9* children[9];
} T9;

T9* InitializeEmptyT9() {
  T9* node = (T9*)malloc(sizeof(T9));
  if (node == NULL) {
    fprintf(stderr, "Error: Failure to allocate memory\n");
    return NULL;
  }

  // Initializes empty node's word and children to be null
  node->word = NULL;
  for (int i = 0; i < 9; i++) {
    node->children[i] = NULL;
  }

  return node;
}

T9* InitializeFromFileT9(const char* filename) {
  T9* node = InitializeEmptyT9();
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "Error: Failure to open file\n");
    DestroyT9(node);
    return NULL;
  }

  char word[50];
  // Assumes the max word length is 50 characters and
  // reads in each word in the file and adds them to the T9 structure
  while (fscanf(file, "%49s", word) == 1) {
    AddWordToT9(node, word);
  }
  fclose(file);

  return node;
}

void AddWordToT9(T9* dict, const char* word) {
  if (dict == NULL) {
    fprintf(stderr, "Error: T9 dictionary is NULL\n");
    return;
  }

  if (word == NULL) {
    fprintf(stderr, "Error: Word is NULL\n");
    return;
  }

  if (strlen(word) == 0) {
    return;
  }

  // Iterates through each letter in the word and
  // adds it to the T9 structure
  T9* curr = dict;
  for (int i = 0; word[i] != '\0'; i++) {
    if (!islower(word[i])) {
        fprintf(stderr, "Error: Word has invalid character\n");
        return;
    }

    // Subtracts two from the T9 number associated with the
    // letter to match the indices of the T9 children array
    // which are 0-7, while the permissible input values are 2-9
    int num = charToInt(word[i]);
    if (num > 9 || num < 2) {
      fprintf(stderr, "Error: Invalid index\n");
      return;
    }
    if (curr->children[num-2] == NULL) {
      curr->children[num-2] = InitializeEmptyT9();
    }
    curr = curr->children[num-2];
  }

  // If the word associated with a T9 sequence is already being used
  // # symbols are added to the sequence until a non-used sequence
  // is found
  while (curr->word != NULL) {
    // Do nothing if a duplicate word is added
    if (strncmp(curr->word, word, strlen(word)) == 0) {
      return;
    } else if (curr->children[8] == NULL) {
      curr->children[8] = InitializeEmptyT9();
    }
    curr = curr->children[8];
  }

  // Memory is allocated for the word stored in the T9 struct based
  // on the size of the word plus the null terminator
  curr->word = (char*)malloc((strlen(word) + 1) * sizeof(char));
  if (curr->word == NULL) {
    fprintf(stderr, "Error: Failed to allocate memory for word\n");
    return;
  }
  strncpy(curr->word, word, strlen(word));
  curr->word[strlen(word)] = '\0';
}

char* PredictT9(T9* dict, const char* nums) {
  if (dict == NULL) {
    fprintf(stderr, "Error: T9 dictionary is NULL\n");
    return NULL;
  }

  if (nums == NULL) {
    fprintf(stderr, "Error: nums is NULL\n");
    return NULL;
  }

  if (strlen(nums) < 1) {
    fprintf(stderr, "Error: nums is empty\n");
    return NULL;
  }

  // Input sequence nums is checked for validity and NULL is
  // return if a non-valid character is used, the sequence is out
  // of order, or if no word exists for the given sequence
  T9* curr = dict;
  int poundCheck = 0;
  size_t len = strlen(nums);
  for (int i = 0; i < len; i++) {
    if (nums[i] == '#' && i == 0) {
      fprintf(stderr, "Error: # can not be the first number\n");
      return NULL;
    } else if (nums[i] == '#' && poundCheck == 0) {
      if (curr->children[8] == NULL) {
        return NULL;
      }
      curr = curr->children[8];
      poundCheck = 1;
    } else if (nums[i] == '#' && poundCheck == 1) {
      if (curr->children[8] == NULL) {
        return NULL;
      }
      curr = curr->children[8];
    } else if (nums[i] > '9' || nums[i] < '2') {
      fprintf(stderr, "Error: Invalid input\n");
      return NULL;
    } else if (poundCheck == 1) {
      fprintf(stderr, "Error: Invalid sequence\n");
      return NULL;
    } else {
      int num = nums[i] - '2';
      if (curr->children[num] == NULL) {
        return NULL;
      }
      curr = curr->children[num];
    }
  }

  if (curr->word == NULL) {
    return NULL;
  }

  return curr->word;
}

void DestroyT9(T9* dict) {
  if (dict == NULL) {
    return;
  }

  // The memory used for the children T9 structures and
  // the word associated with the node is cleared
  for (int i = 0; i < 9; i++) {
    DestroyT9(dict->children[i]);
  }

  if (dict->word != NULL) {
    free(dict->word);
  }

  free(dict);
}

int charToInt(char letter) {
  // An array is used to hold the T9 value of letters at their
  // ASCII value's index so the T9 value for any letter
  // can be easily retrieved
  static int t9[128];
  static int created = 0;

  if (!created) {
    for (int i = 0; i < 128; i++) {
      t9[i] = 0;
    }

    t9['a'] = t9['b'] = t9['c'] = 2;
    t9['d'] = t9['e'] = t9['f'] = 3;
    t9['g'] = t9['h'] = t9['i'] = 4;
    t9['j'] = t9['k'] = t9['l'] = 5;
    t9['m'] = t9['n'] = t9['o'] = 6;
    t9['p'] = t9['q'] = t9['r'] = t9['s'] = 7;
    t9['t'] = t9['u'] = t9['v'] = 8;
    t9['w'] = t9['x'] = t9['y'] = t9['z'] = 9;

    created = 1;
  }

  return t9[(int)letter];
}
