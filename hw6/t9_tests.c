#include <string.h>
#include <stdio.h>
#include "safe_assert.h"
#include "t9_lib.h"

void AssertReturnedStringEquals(char* expected, char* actual);

suite("T9") {
  // Initialize Empty 
  test("Empty initialization") {
    T9* dict = InitializeEmptyT9();
    safe_assert(dict != NULL);
    DestroyT9(dict);
  }

  // PredictT9 with #
  test("Adding # to input picks next matching word") {
    T9* dict = InitializeEmptyT9();
    safe_assert(dict != NULL);

    AddWordToT9(dict, "book");
    AddWordToT9(dict, "cool");

    char* word = PredictT9(dict, "2665#");
    AssertReturnedStringEquals("cool", word);

    DestroyT9(dict);
  }

  // InitializeFromFile with Fake File
  test("Non-existing file") {
    char* fakefile = NULL;
    T9* dict = InitializeFromFileT9(fakefile);
    safe_assert(dict == NULL);
  }

  // InitializeFromFile with Normal File is not null
  test("Existing File initialize") {
    T9* dict = InitializeFromFileT9("dictionary.txt");
    safe_assert(dict != NULL);
    DestroyT9(dict);
  }

  // InitializeFromFile with Normal File tests words
  test("Existing File initialize has correct words") {
    T9* dict = InitializeFromFileT9("dictionary.txt");
    char* word1 = PredictT9(dict, "22");
    char* word2 = PredictT9(dict, "224");
    char* word3 = PredictT9(dict, "22433");
    char* word4 = PredictT9(dict, "2227");
    AssertReturnedStringEquals("aa", word1);
    AssertReturnedStringEquals("aah", word2);
    AssertReturnedStringEquals("aahed", word3);
    AssertReturnedStringEquals("abas", word4);
    DestroyT9(dict);
  }

  // AddWordToT9 Empty Word
  test("Empty word") {
    T9* dict = InitializeEmptyT9();
    AddWordToT9(dict, "");
    safe_assert(dict != NULL);
    char* word = PredictT9(dict, "");
    safe_assert(word  == NULL);
    DestroyT9(dict);
  }

  // AddWordToT9 Incorrect Word 
  test("Incorrect word") {
    T9* dict = InitializeEmptyT9();
    
    AddWordToT9(dict, "hello5");
    AddWordToT9(dict, "hello-");
    AddWordToT9(dict, "HELLO");
    AddWordToT9(dict, "].[hello;.'");
    AddWordToT9(dict, "43556");
    AddWordToT9(dict, "Hello");
    AddWordToT9(dict, "hello!@#$%^&*");
    
    safe_assert(dict != NULL);
    char* word = PredictT9(dict, "43556");
    safe_assert(word  == NULL);
    
    AddWordToT9(dict, "hello");
    word = PredictT9(dict, "43556");
    AssertReturnedStringEquals("hello", word);
    DestroyT9(dict);
  }

  // AddWordToT9 Existing Word
  test("Already Present Word") {
    T9* dict = InitializeEmptyT9();
    AddWordToT9(dict, "book");
    AddWordToT9(dict, "book");
    char* word = PredictT9(dict, "2665#");
    safe_assert(word  == NULL);
    DestroyT9(dict);
  }

  // AddWordToT9 Unique Word
  test("Unique Word") {
    T9* dict = InitializeEmptyT9();
    AddWordToT9(dict, "book");
    safe_assert(dict != NULL);

    char* word  = PredictT9(dict, "2665");
    AssertReturnedStringEquals("book", word);
    DestroyT9(dict);
  }

  //PredictT9 nums is Null
  test("Null Nums") {
    T9* dict = InitializeFromFileT9("dictionary.txt");
    char* word = PredictT9(dict, NULL);
    safe_assert(word  == NULL);
    DestroyT9(dict);
  }

  // PredictT9 nums is empty
  test("Empty Nums") {
    T9* dict = InitializeFromFileT9("dictionary.txt");
    char* word = PredictT9(dict, "");
    safe_assert(word  == NULL);
    DestroyT9(dict);
  }

  // PredictT9 nusm has unusable character
  test("Incorrect Char") {
    T9* dict = InitializeFromFileT9("dictionary.txt");
    char* word = PredictT9(dict, "a2665");
    safe_assert(word  == NULL);
    DestroyT9(dict);
  }

   // PredictT9 has incorrect nums
  test("Incorrect Num") {
    T9* dict = InitializeFromFileT9("dictionary.txt");
    char* word = PredictT9(dict, "1111");
    safe_assert(word  == NULL);
    DestroyT9(dict);
  }

  // PredictT9 nums has # in front
  test("Nums starts with #") {
    T9* dict = InitializeFromFileT9("dictionary.txt");
    char* word = PredictT9(dict, "#2665");
    safe_assert(word  == NULL);
    DestroyT9(dict);
  }

  // PredictT9 nums has number after # at the end
  test("Number after # in Nums") {
    T9* dict = InitializeFromFileT9("dictionary.txt");
    char* word = PredictT9(dict, "2665#2");
    safe_assert(word  == NULL);
    DestroyT9(dict);
  }

  // PredictT9 nums has doesn't work with 0 or 1
  test("0 or 1 in Nums") {
    T9* dict = InitializeFromFileT9("dictionary.txt");
    char* word = PredictT9(dict, "0665");
    safe_assert(word  == NULL);
    DestroyT9(dict);
  }

  // PredictT9 nums is equal length to the word it returns
  test("Length of Nums Equals Length of Word") {
    T9* dict = InitializeEmptyT9();
    AddWordToT9(dict, "book");
    char* word = PredictT9(dict, "2665");
    safe_assert(strlen(word) == strlen("book"));
    DestroyT9(dict);
  }

  // PredictT9 no # returns first word in list
  test("No # Present") {
    T9* dict = InitializeEmptyT9();
    AddWordToT9(dict, "book");
    AddWordToT9(dict, "cool");
    char* word = PredictT9(dict, "2665");
    AssertReturnedStringEquals("book", word);
    DestroyT9(dict);
  }

  // PredictT9 two # returns correct word
  test("## Chooses Correct Word") {
    T9* dict = InitializeEmptyT9();
    AddWordToT9(dict, "book");
    AddWordToT9(dict, "cool");
    AddWordToT9(dict, "bool");
    char* word = PredictT9(dict, "2665##");
    AssertReturnedStringEquals("bool", word); 
    DestroyT9(dict);
  }

  // Adds all words correctly
  test("All words work") {
    T9* dict = InitializeEmptyT9();
    AddWordToT9(dict, "book");
    AddWordToT9(dict, "cool");
    AddWordToT9(dict, "bool");
    AddWordToT9(dict, "hello");
    char* word1 = PredictT9(dict, "2665");
    char* word2 = PredictT9(dict, "2665#");
    char* word3 = PredictT9(dict, "2665##");
    char* word4 = PredictT9(dict, "43556");
    AssertReturnedStringEquals("book", word1);
    AssertReturnedStringEquals("cool", word2);
    AssertReturnedStringEquals("bool", word3);
    AssertReturnedStringEquals("hello", word4);
    DestroyT9(dict);
  }

  // 3+ words of same number sequence
  test("3+ similar words") {
    T9* dict = InitializeEmptyT9();
    AddWordToT9(dict, "book");
    AddWordToT9(dict, "cool");
    AddWordToT9(dict, "cook");
    AddWordToT9(dict, "bool");

    char* word1 = PredictT9(dict, "2665");
    char* word2 = PredictT9(dict, "2665#");
    char* word3 = PredictT9(dict, "2665##");
    char* word4 = PredictT9(dict, "2665###");

    AssertReturnedStringEquals("book", word1);
    AssertReturnedStringEquals("cool", word2);
    AssertReturnedStringEquals("cook", word3);
    AssertReturnedStringEquals("bool", word4);
    DestroyT9(dict);
  }

  
   // AddWordto can't add word with space 
  test("Space in words") {
    T9* dict = InitializeEmptyT9();
    AddWordToT9(dict, "book ");
    AddWordToT9(dict, " cool");
    char* word = PredictT9(dict, "2665");
    safe_assert(word == NULL);
    DestroyT9(dict);
  }

   // InitializeFromFile with Normal File is not null
  test("Existing File initialize") {
    T9* dict = InitializeFromFileT9("small_dictionary.txt");
    safe_assert(dict != NULL);
    char* word = PredictT9(dict, "93272");
    AssertReturnedStringEquals("zebra", word);
    DestroyT9(dict);
  }
}

void AssertReturnedStringEquals(char* expected, char* actual) {
  safe_assert(actual > 0);
  safe_assert(strlen(actual) == strlen(expected));
  safe_assert(strcmp(actual, expected) == 0);
}
