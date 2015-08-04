#include <stdio.h>
#include <gmp.h>
#include <string.h>
#include <time.h> // for seeding from time, probably should be changed to something with /dev/urandom

static const unsigned int num_chars = 410 * 40 * 80 + 26; // pages * lines * characters per line + title
static const unsigned int num_base = 29; // if you change this, be sure to change strbook2str and friends.
static const unsigned int loc_multiplier = 29; // must be coprime to max_book_plus_one. hint: use coprime_check() to find this! :)

// DO NOT TOUCH THESE... please?
mpz_t max_book_plus_one;
gmp_randstate_t randstate;
mpz_t inv_loc_multiplier;

// This is what you use to help you pick a loc_multiplier. Well, can. You don't have to.
unsigned long coprime_check(unsigned long num) {
    mpz_t result;
    mpz_init(result);

    mpz_gcd_ui(result, max_book_plus_one, num);
    unsigned long res = mpz_get_ui(result);

    mpz_clear(result);

    return res;
}

void init() {
    // make a maximum str that represents our last book
    char s[num_chars + 1];
    memset(s, 's', num_chars); // 's' depends on our alphabet. see strbook2book and str2strbook
    s[num_chars] = '\0';

    mpz_init_set_str(max_book_plus_one, s, num_base);
    // --------------------------------------------------------------------------------------------

    // initialize random number generate, to generate random book
    gmp_randinit_default(randstate);
    gmp_randseed_ui(randstate, time(NULL)); // probably should be changed to /dev/urandom or something, but who cares? :P

    // --------------------------------------------------------------------------------------------

    if (loc_multiplier == 0 || coprime_check(loc_multiplier) != 1) {
        mpz_set_ui(inv_loc_multiplier, 0);
    } else {
        mpz_t locmul;
        mpz_init_set_ui(locmul, loc_multiplier);

        mpz_invert(inv_loc_multiplier, locmul, max_book_plus_one);

        mpz_clear(locmul);
    }

}

void clean() {
    // clean up stuff from above
    mpz_clear(max_book_plus_one);
    mpz_clear(inv_loc_multiplier);
    gmp_randclear(randstate);
}

// left-pads a string with a specified character pad
void leftpad(char *str, char pad) {
    int len = num_chars - strlen(str);
    if (len) {
        memmove(str+len, str, strlen(str));
        memset(str, pad, len);
    }
}

int strbook2str(char *strbook, char *str) {
    leftpad(strbook, 't');

    while (*strbook) {
        if (*strbook == ' ') {
            *str = '9';
        } else if (*strbook == ',') {
            *str = '8';
        } else if (*strbook == '.') {
            *str = '7';
        } else if (*strbook >= 't' && *strbook <= 'z') {
            *str = *strbook - 68;
        } else if (*strbook >= 'a' && *strbook <= 's') {
            *str = *strbook;
        } else {
            *str = '\0';
            return 1;
        }

        *strbook++;
        *str++;
    }

    *str = '\0';
    return 0;
}

int str2strbook(char *str, char *strbook) {
    leftpad(str, '0');

    while (*str) {
        if(*str == '9') {
            *strbook = ' ';
        } else if (*str == '8') {
            *strbook = ',';
        } else if (*str == '7') {
            *strbook = '.';
        } else if (*str >= '0' && *str <= '6') {
            *strbook = *str + 68;
        } else if (*str >= 'a' && *str <= 's') {
            *strbook = *str;
        } else {
            *strbook = '\0';
            return 1;
        }

        *str++;
        *strbook++;
    }

    *strbook = '\0';
    return 0;
}

void set_random_book(mpz_t book) {
    mpz_urandomm(book, randstate, max_book_plus_one);
}

void init_set_random_book(mpz_t book) {
    mpz_init(book);
    set_random_book(book);
}

void book2strbook(mpz_t book, char *strbook) {
    char str[num_chars + 1];
    mpz_get_str(str, num_base, book);
    str2strbook(str, strbook);
}

void strbook2book(char *strbook, mpz_t book) {
    char str[num_chars + 1];
    strbook2str(strbook, str);
    mpz_set_str(book, str, num_base);
}

void set_random_strbook(char *strbook) {
    mpz_t book;
    init_set_random_book(book);
    book2strbook(book, strbook);
    mpz_clear(book);
}

void strbook2locbook(char *strbook, mpz_t locbook) {
    char locstrbook[num_chars + 1];
    for (int i = 0, j = num_chars - 1, k = 0; i <= j; i++, j--, k+=2) {
        locstrbook[k] = strbook[i];
        if (i != j) {
            locstrbook[k+1] = strbook[j];
        }
    }
    locstrbook[num_chars] = '\0';
    strbook2book(locstrbook, locbook);

    mpz_mul_ui(locbook, locbook, loc_multiplier);
    mpz_mod(locbook, locbook, max_book_plus_one);
}

void locbook2strbook(mpz_t locbook, char *strbook) {
    mpz_t book;
    mpz_init(book);

    mpz_mul(book, locbook, inv_loc_multiplier);
    mpz_mod(book, book, max_book_plus_one);

    char locstrbook[num_chars + 1];
    book2strbook(book, locstrbook);

    mpz_clear(book);

    for (int i = 0, j = num_chars - 1, k = 0; i <= j; i++, j--, k+=2) {
        strbook[i] = locstrbook[k];
        if (i != j) {
            strbook[j] = locstrbook[k+1];
        }
    }

    strbook[num_chars] = '\0';
}

int main() {
    init(); // Always call in beginning!

    mpz_t book, locbook;
    init_set_random_book(book);
    mpz_init(locbook);

    char strbook[num_chars + 1]; // + 1 is for null-terminated
    book2strbook(book, strbook);

    strbook2locbook(strbook, locbook);

    printf("%s\n", strbook);

    mpz_clear(book);
    mpz_clear(locbook);

    clean(); // Always call right at the end of the program!
    return 0;   
}
