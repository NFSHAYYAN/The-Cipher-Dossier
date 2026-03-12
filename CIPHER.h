#ifndef CIPHER_H
#define CIPHER_H

#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

// Color Constants
#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define WHITE 7

// Utility & Math
void setColor(int textColor);
void resetColor();
void clearScreen();
void printThis(string line, int textColor = WHITE);
int gcd(int a, int b);
bool isPrime(int n);
int simple_pow_mod(int base, int exp, int mod);
int get_inverse(int a, int m);
int get_prime();

// Cipher Logic
void generateKeyTable(string key, char table[5][5]);
void findPosition(char table[5][5], char ch, int &r, int &c);
string playfairEncrypt(string text, string key);
string playfairDecrypt(string cipher, string key);

// Menu Actions
void caesar();
void playfair();
void rsa_crt();
void diffie_hell();

// UI/Art
void printMainBanner();
void printCaesarArt();
void printPlayfairArt();
void printRsaArt();
void printDhArt();
void printExitArt();
void printMenuBorder();
void printMenuBottom();

struct CryptApp {
    void run();
};

#endif