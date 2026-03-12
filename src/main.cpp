#include "CIPHER.h"  // Added .h extension to fix the error
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <string>
#include <ctime>
#include <windows.h>
#include <cstring>

using namespace std;

// simple gcd
int gcd(int a, int b)
{
    while (b)
    {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// simple prime test
bool isPrime(int n)
{
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

// simple power mod
int simple_pow_mod(int base, int exp, int mod)
{
    int r = 1;
    for (int i = 0; i < exp; i++)
        r = (r * base) % mod;
    return r;
}

int get_inverse(int a, int m)
{
    a = a % m;
    if (a < 0) a += m;

    // brute-force: try all x from 1 to m-1
    for (int x = 1; x < m; x++)
    {
        if ((a * x) % m == 1)
            return x;   // x is inverse
    }

    // no inverse found
    return -1;
}

int get_prime()
{
    int prime;

    while (true)
    {
        prime = (rand() % 1000) + 2;

        if (isPrime(prime))
        {
            return prime;
        }
    }
}

// ================== CAESAR CIPHER ==================
void caesar()
{
    
    printThis("\n==== CAESAR CIPHER ====\n", YELLOW);

    char text[1000];
    printThis("\n Enter text: ", CYAN);
    cin.ignore();
    cin.getline(text, 1000);

    if (strlen(text) == 0)
    {
        printThis("\n Text is empty !\n", RED);
        return;
    }

    int key;
    printThis("\n Enter shift 1-25: ", CYAN);
    if (!(cin >> key))
	 {
    cin.clear();
    cin.ignore(1000, '\n');
    printThis("\n Error: Shift must be a number!\n", RED);
    return; // Exit safely
}

    while (key < 1 || key > 25)
    {
        printThis("\n Wrong input ! Enter (1-25): ", RED);
        cin >> key;
    }

    key = key % 26;
    if (key == 0)
    {
        key = 3;
    }

    int n = (int)strlen(text);
    char enc[1000];
    char dec[1000];

    for (int i = 0; i < n; i++)
    {
        char c = text[i];

        if (c >= 'A' && c <= 'Z')
        {
            int p = c - 'A';
            int np = (p + key) % 26;
            enc[i] = 'A' + np;
        }
        else if (c >= 'a' && c <= 'z')
        {
            int p = c - 'a';
            int np = (p + key) % 26;
            enc[i] = 'a' + np;
        }
        else
        {
            enc[i] = c;
        }
    }
    enc[n] = '\0';

    for (int i = 0; i < n; i++)
    {
        char c = enc[i];

        if (c >= 'A' && c <= 'Z')
        {
            int p = c - 'A';
            int np = (p - key + 26) % 26;
            dec[i] = 'A' + np;
        }
        else if (c >= 'a' && c <= 'z')
        {
            int p = c - 'a';
            int np = (p - key + 26) % 26;
            dec[i] = 'a' + np;
        }
        else
        {
            dec[i] = c;
        }
    }
    dec[n] = '\0';

    printThis("\nOriginal: ", MAGENTA);
    cout << text;
    printThis("\nKey: ", MAGENTA);
    cout << key;
    printThis("\nEncrypted: ", MAGENTA);
    cout << enc;
    printThis("\nDecrypted: ", MAGENTA);
    cout << dec;

    bool ok = true;
    for (int i = 0; i < n; i++)
    {
        if (text[i] != dec[i])
        {
            ok = false;
            break;
        }
    }

    if (ok)
    {
        printThis("\nSuccess!\n", GREEN);
    }
    else
    {
        printThis("\nFailed!\n", RED);
    }

    ofstream f("caesar.txt");
    if (f)
    {
        f << "Original: " << text << endl;
        f << "Key: " << key << endl;
        f << "Encrypted: " << enc << endl;
        f << "Decrypted: " << dec << endl;
        f.close();
        printThis("Saved to caesar.txt\n", GREEN);
    }

    printThis("\n====================\n", BLUE);
}

// ================== PLAYFAIR CIPHER HELPERS ==================

void generateKeyTable(string key, char table[5][5]) {
    bool used[26] = {false};
    int row = 0, col = 0;

    for (int i = 0; i < key.length(); i++) {
    char ch = key[i];
    if (ch >= 'a' && ch <= 'z') ch = ch - 'a' + 'A';
    if (ch == 'J') ch = 'I'; 
    if (ch >= 'A' && ch <= 'Z' && !used[ch - 'A']) {
        table[row][col] = ch;
        used[ch - 'A'] = true;
        col++;
        if (col == 5) { row++; col = 0; }
    }
}

    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (ch == 'J') continue;
        if (!used[ch - 'A']) {
            table[row][col] = ch;
            used[ch - 'A'] = true;
            col++;
            if (col == 5) { row++; col = 0; }
        }
    }
}

void findPosition(char table[5][5], char ch, int &r, int &c) {
    if (ch == 'J') ch = 'I';
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (table[i][j] == ch) { r = i; c = j; return; }
}

// Playfair encryption
string playfairEncrypt(string text, string key) {
    char table[5][5];
    generateKeyTable(key, table);

    string msg = "";
    for (int i = 0; i < (int)text.length(); i++) {
        char ch = text[i];
        if (ch >= 'a' && ch <= 'z') ch = ch - 'a' + 'A';
        if (ch < 'A' || ch > 'Z') continue;
        if (ch == 'J') ch = 'I';
        if (!msg.empty() && msg.back() == ch) msg += 'X';
        msg += ch;
    }
    if (msg.length() % 2 != 0) msg += 'X'; 

    string cipher = "";
    for (int i = 0; i < (int)msg.length(); i += 2) {
        int r1, c1, r2, c2;
        findPosition(table, msg[i], r1, c1);
        findPosition(table, msg[i + 1], r2, c2);

        if (r1 == r2) { // Same row
            cipher += table[r1][(c1 + 1) % 5];
            cipher += table[r2][(c2 + 1) % 5];
        } else if (c1 == c2) { // Same column
            cipher += table[(r1 + 1) % 5][c1];
            cipher += table[(r2 + 1) % 5][c2];
        } else { // Rectangle
            cipher += table[r1][c2];
            cipher += table[r2][c1];
        }
    }
    return cipher;
}

// Playfair decryption
string playfairDecrypt(string cipher, string key) {
    char table[5][5];
    generateKeyTable(key, table);

    string text = "";
    for (int i = 0; i < (int)cipher.length(); i += 2) {
        int r1, c1, r2, c2;
        findPosition(table, cipher[i], r1, c1);
        findPosition(table, cipher[i + 1], r2, c2);

        if (r1 == r2) { // Same row
            text += table[r1][(c1 + 4) % 5]; // move left
            text += table[r2][(c2 + 4) % 5];
        } else if (c1 == c2) { // Same column
            text += table[(r1 + 4) % 5][c1]; // move up
            text += table[(r2 + 4) % 5][c2];
        } else { // Rectangle
            text += table[r1][c2];
            text += table[r2][c1];
        }
    }
    return text;
}

// Wrapper using  UI
void playfair()
{
  
    printThis("\n==== PLAYFAIR CIPHER ====\n", YELLOW);

    string key;
    printThis("Enter key: ", CYAN);
    cin.ignore();
    getline(cin, key);

    string plaintext;
    printThis("Enter text: ", CYAN);
    getline(cin, plaintext);

    string cipher = playfairEncrypt(plaintext, key);
    string decrypted = playfairDecrypt(cipher, key);

    printThis("\nPlaintext: ", MAGENTA);
    cout << plaintext;
    printThis("\nCiphertext: ", MAGENTA);
    cout << cipher;
    printThis("\nDecrypted: ", MAGENTA);
    cout << decrypted << endl;

    ofstream f("playfair.txt");
    if (f)
    {
        f << "Key: " << key << endl;
        f << "Plaintext: " << plaintext << endl;
        f << "Ciphertext: " << cipher << endl;
        f << "Decrypted: " << decrypted << endl;
        f.close();
        printThis("Saved to playfair.txt\n", GREEN);
    }

    printThis("\n==================\n", BLUE);
}

// ================== RSA ==================

/*chinese remainder theorem speed up decryption*/
void rsa_crt()
{
    printThis("\n==== RSA WITH CRT ====\n", YELLOW);

    int p = get_prime();
    int q = get_prime();

    while (q == p)
    {
        q = get_prime();
    }

    int n = p * q;
    int phi = (p - 1) * (q - 1);

    int e = 17;
    if (e >= phi)
        e = 3;

    while (gcd(e, phi) != 1)
    {
        e = e + 1;
        if (e >= phi)
            e = 3;
    }

    int d = get_inverse(e, phi);
    if (d == -1)
    {
        printThis("No inverse found!\n", RED);
        return;
    }

    printThis("\nRSA setup:\n", CYAN);
    printThis("p = ", MAGENTA);
    cout << p << endl;
    printThis("q = ", MAGENTA);
    cout << q << endl;
    printThis("n = ", MAGENTA);
    cout << n << endl;
    printThis("phi = ", MAGENTA);
    cout << phi << endl;
    printThis("e = ", MAGENTA);
    cout << e << endl;
    printThis("d = ", MAGENTA);
    cout << d << endl;

    int msg;
    printThis("\nEnter number (< ", CYAN);
    cout << n;
    printThis("): ", CYAN);
    if (!(cin >> msg) || msg >= n || msg < 0) 
	{
    cin.clear();              // Reset if non-numeric
    cin.ignore(1000, '\n');     // Clear buffer
    printThis(" Error: Invalid message! Must be a positive number less than n.\n", RED);
    return; // Exit function safely to avoid mathematical crash
}

    int c = simple_pow_mod(msg, e, n);
    printThis("Encrypted: ", MAGENTA);
    cout << c << endl;

    printThis("\nUsing CRT...\n", CYAN);

    int dp = d % (p - 1);
    int dq = d % (q - 1);

    int m1 = simple_pow_mod(c, dp, p);
    int m2 = simple_pow_mod(c, dq, q);

    int qinv = get_inverse(q, p);
    if (qinv == -1)
    {
        printThis("Q inverse fail\n", RED);
        return;
    }

    int diff = m1 - m2;
    if (diff < 0)
        diff = diff + p;

    int h = (qinv * diff) % p;
    int m = m2 + h * q;
    m = m % n;

    printThis("Decrypted: ", MAGENTA);
    cout << m << endl;

    if (m == msg)
    {
        printThis("RSA works!\n", GREEN);
    }
    else
    {
        printThis("RSA failed!\n", RED);
    }

    ofstream f("rsa.txt");
    if (f)
    {
        f << "p = " << p << endl;
        f << "q = " << q << endl;
        f << "n = " << n << endl;
        f << "phi = " << phi << endl;
        f << "e = " << e << endl;
        f << "d = " << d << endl;
        f << "Message: " << msg << endl;
        f << "Encrypted: " << c << endl;
        f << "Decrypted: " << m << endl;
        f.close();
        printThis("Saved to rsa.txt\n", GREEN);
    }

    printThis("==================\n", BLUE);
}

// ================== DIFFIE HELLMAN ==================

void diffie_hell()
{
    printThis("\n==== DIFFIE-HELLMAN ====\n", YELLOW);

    int p = get_prime();
    int g = 5;

    printThis("\nPublic numbers:\n", CYAN);
    printThis("Prime p = ", MAGENTA);
    cout << p << endl;
    printThis("Base g = ", MAGENTA);
    cout << g << endl;

    int secret1 = get_prime();
    int secret2 = get_prime();

    if (secret1 == secret2)
    {
        secret2 = get_prime();
    }

    printThis("\nPrivate numbers:\n", CYAN);
    printThis("Person 1 secret = ", MAGENTA);
    cout << secret1 << endl;
    printThis("Person 2  secret = ", MAGENTA);
    cout << secret2 << endl;

    int public1 = simple_pow_mod(g, secret1, p);
    int public2 = simple_pow_mod(g, secret2, p);

    printThis("\nPublic numbers:\n", CYAN);
    printThis("Person1 sends = ", MAGENTA);
    cout << public1 << endl;
    printThis("Person2 sends = ", MAGENTA);
    cout << public2 << endl;

    int key1 = simple_pow_mod(public2, secret1, p);
    int key2 = simple_pow_mod(public1, secret2, p);

    printThis("\nShared secret:\n", CYAN);
    printThis("Person1 gets = ", MAGENTA);
    cout << key1 << endl;
    printThis("Person2 gets = ", MAGENTA);
    cout << key2 << endl;

    if (key1 == key2)
    {
        printThis("\nSuccess! Keys match.\n", GREEN);
        printThis("Shared key = ", MAGENTA);
        cout << key1 << endl;
        printThis("Use for symmetric cipher!\n", GREEN);
    }
    else
    {
        printThis("\nFailed! Keys don't match.\n", RED);
    }

    ofstream f("dh.txt");
    if (f)
    {
        f << "Prime = " << p << endl;
        f << "Base = " << g << endl;
        f << "Alice secret = " << secret1 << endl;
        f << "Bob secret = " << secret2 << endl;
        f << "Alice public = " << public1 << endl;
        f << "Bob public = " << public2 << endl;
        f << "Alice shared = " << key1 << endl;
        f << "Bob shared = " << key2 << endl;
        f << "Match = " << (key1 == key2 ? "YES" : "NO") << endl;
        f.close();
        printThis("Saved to dh.txt\n", GREEN);
    }

    printThis("====================\n", BLUE);
}

// ================== ASCII ART / MENU ==================

void printMainBanner()
{
    printThis("\n", YELLOW);
    printThis("  _______ _            _____ _       _                  \n", YELLOW);
    printThis(" |__   __| |          / ____(_)     | |                 \n", YELLOW);
    printThis("    | |  | |__   ___ | |     _ _ __ | |__   ___ _ __    \n", YELLOW);
    printThis("    | |  | '_ \\ / _ \\| |    | | '_ \\| '_ \\ / _ \\ '__|   \n", YELLOW);
    printThis("    | |  | | | |  __/| |____| | |_) | | | |  __/ |      \n", YELLOW);
    printThis("    |_|  |_| |_|\\___| \\_____|_| .__/|_| |_|\\___|_|      \n", YELLOW);
    printThis("                              | |                       \n", YELLOW);
    printThis("  _____                 _     |_|                       \n", YELLOW);
    printThis(" |  __ \\               (_)                              \n", YELLOW);
    printThis(" | |  | | ___  ___ ___  _  ___ _ __                     \n", YELLOW);
    printThis(" | |  | |/ _ \\/ __/ __|| |/ _ \\ '__|                    \n", YELLOW);
    printThis(" | |__| | (_) \\__ \\__ \\| |  __/ |                       \n", YELLOW);
    printThis(" |_____/ \\___/|___/___/|_|\\___|_|                       \n", YELLOW);
    printThis("\n", BLUE);
}

void printCaesarArt()
{
    printThis("\n", YELLOW);
    printThis("   _____                             _____ _       _               \n", YELLOW);
    printThis("  / ____|                           / ____(_)     | |              \n", YELLOW);
    printThis(" | |     __ _  ___  ___  __ _ _ __ | |     _ _ __ | |__   ___ _ __ \n", YELLOW);
    printThis(" | |    / _` |/ _ \\/ __|/ _` | '__|| |    | | '_ \\| '_ \\ / _ \\ '__|\n", YELLOW);
    printThis(" | |___| (_| |  __/\\__ \\ (_| | |   | |____| | |_) | | | |  __/ |   \n", YELLOW);
    printThis("  \\_____\\__,_|\\___||___/\\__,_|_|    \\_____|_| .__/|_| |_|\\___|_|   \n", YELLOW);
    printThis("                                            | |                    \n", YELLOW);
    printThis("                                            |_|                    \n", YELLOW);
    printThis("\n", YELLOW);
}

void printPlayfairArt()
{
    printThis("\n", CYAN);
    printThis("  _____  _              __       _      \n", CYAN);
    printThis(" |  __ \\| |            / _|     (_)     \n", CYAN);
    printThis(" | |__) | | __ _ _   _| |_ __ _ _ _ __ \n", CYAN);
    printThis(" |  ___/| |/ _` | | | |  _/ _` | | '__|\n", CYAN);
    printThis(" | |    | | (_| | |_| | || (_| | | |   \n", CYAN);
    printThis(" |_|    |_|\\__,_|\\__, |_| \\__,_|_|_|   \n", CYAN);
    printThis("                  __/ |                 \n", CYAN);
    printThis("                 |___/                  \n", CYAN);
    printThis("\n", CYAN);
}

void printRsaArt()
{
    printThis("\n", MAGENTA);
    printThis("  _____   _____          \n", MAGENTA);
    printThis(" |  __ \\ / ____|   /\\    \n", MAGENTA);
    printThis(" | |__) | (___    /  \\   \n", MAGENTA);
    printThis(" |  _  / \\___ \\  / /\\ \\  \n", MAGENTA);
    printThis(" | | \\ \\ ____) |/ ____ \\ \n", MAGENTA);
    printThis(" |_|  \\_\\_____//_/    \\_\\\n", MAGENTA);
    printThis("\n", MAGENTA);
}

void printDhArt()
{
    printThis("\n", GREEN);
    printThis("  _____  _____ ______ ______ _____ ______    _    _ ______ _      _      __  __          _   _ \n", GREEN);
    printThis(" |  __ \\|_   _|  ____|  ____|_   _|  ____|  | |  | |  ____| |    | |    |  \\/  |   /\\   | \\ | |\n", GREEN);
    printThis(" | |  | | | | | |__  | |__    | | | |__     | |__| | |__  | |    | |    | \\  / |  /  \\  |  \\| |\n", GREEN);
    printThis(" | |  | | | | |  __| |  __|   | | |  __|    |  __  |  __| | |    | |    | |\\/| | / /\\ \\ | . ` |\n", GREEN);
    printThis(" | |__| |_| |_| |    | |     _| |_| |____   | |  | | |____| |____| |____| |  | |/ ____ \\| |\\  |\n", GREEN);
    printThis(" |_____/|_____|_|    |_|    |_____|______|  |_|  |_|______|______|______|_|  |_/_/    \\_\\_| \\_|\n", GREEN);
    printThis("\n", GREEN);
}

void printExitArt()
{
    printThis("\n", GREEN);
    printThis("   _____                 _ _               \n", GREEN);
    printThis("  / ____|               | | |              \n", GREEN);
    printThis(" | |  __  ___   ___   __| | |__  _   _ ___ \n", GREEN);
    printThis(" | | |_ |/ _ \\ / _ \\ / _` | '_ \\| | | / _ \\\n", GREEN);
    printThis(" | |__| | (_) | (_) | (_| | |_) | |_| |  __/\n", GREEN);
    printThis("  \\_____|\\___/ \\___/ \\__,_|_.__/ \\__, |\\___|\n", GREEN);
    printThis("                                  __/ |    \n", GREEN);
    printThis("                                 |___/     \n", GREEN);
    printThis("\n", GREEN);
}

void printMenuBorder()
{
    printThis(" ==================================\n", BLUE);
}

void printMenuBottom()
{
    printThis(" ==================================\n", BLUE);
}
void setColor(int textColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, textColor);
}

// Reset colors to default
void resetColor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7); // Default gray on black
}

void clearScreen()
{
    system("cls");
}

void printThis(string line, int textColor )
{
    setColor(textColor);
    cout << line;
    resetColor();
}


// ================== APP STRUCT ==================


    void CryptApp::run() {
        printMainBanner();
        printThis("=================================\n", BLUE);
        printThis("   The Cipher Dossier V1.0      \n", CYAN);
        printThis("=================================\n", BLUE);

        srand(time(0));
        int pick;

        do
        {
            printThis("\n", WHITE);
            printMenuBorder();
            printThis("           MAIN MENU          \n", BLUE);
            printMenuBorder();
            printThis("       1. Caesar Cipher           \n", CYAN);
            printThis("       2. Playfair Cipher         \n", CYAN);
            printThis("       3. RSA with CRT            \n", CYAN);
            printThis("       4. Diffie-Hellman          \n", CYAN);
            printThis("       5. Exit                    \n", RED);
            printMenuBottom();
            printThis(" Your choice (1-5): ", CYAN);
            if (!(cin >> pick)) 
			{                                     // Error Handling: Check if input is not a number
            cin.clear();                    // Clear error flag
            cin.ignore(1000, '\n');        // Discard bad input and prevents infinite loop
            printThis(" Invalid input! Please enter a number.\n", RED);
            continue;
        }

            if (pick == 1)
            {
                clearScreen();
                printCaesarArt();
                caesar();
            }
            else if (pick == 2)
            {
                clearScreen();
                printPlayfairArt();
                playfair();
            }
            else if (pick == 3)
            {
                clearScreen();
                printRsaArt();
                rsa_crt();
            }
            else if (pick == 4)
            {
                clearScreen();
                printDhArt();
                diffie_hell();
            }
            else if (pick == 5)
            {
                clearScreen();
                printExitArt();
            }
            else
            {
                printThis("\n", RED);
                printThis("   Wrong choice! Try again.\n", RED);
            }

        } while (pick != 5);
    }

int main()
{
    CryptApp app;
    app.run();
    return 0;
}


