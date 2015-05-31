#include <stdio.h>
#include <gmp.h>
#include <string.h>
#include <time.h> // for seeding from time, probably should be changed

static const unsigned int num_chars = 410 * 40 * 80 + 26;
mpz_t max_book_plus_one;
gmp_randstate_t randstate;

void init() {
    char s[num_chars + 1];
    memset(s, 's', num_chars);
    s[num_chars] = '\0';

    mpz_init_set_str(max_book_plus_one, s, 29);
    mpz_add_ui(max_book_plus_one, max_book_plus_one, 1);

    gmp_randinit_default(randstate);
    gmp_randseed_ui(randstate, time(NULL));
}

void deinit() {
    mpz_clear(max_book_plus_one);
    gmp_randclear(randstate);
}

void leftpad(char *str, char pad) {
    int len = num_chars - strlen(str);
    if (len) {
        memmove(str+len, str, strlen(str));
        memset(str, pad, len);
    }
}

int strbook2str(char *strbook, char *str) {
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
    leftpad(str, '0');
    return 0;
}

int str2strbook(char *str, char *strbook) {
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
    leftpad(str, 't');
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
    mpz_get_str(str, 29, book);
    str2strbook(str, strbook);
}

void strbook2book(char *strbook, mpz_t book) {
    char str[num_chars + 1];
    strbook2str(strbook, str);
    mpz_set_str(book, str, 29);
}

void set_random_strbook(char *strbook) {
    mpz_t book;
    init_set_random_book(book);
    book2strbook(book, strbook);
    mpz_clear(book);
}

void strbook2locbook(char *strbook, mpz_t locbook) {
    char locstrbook[num_chars + 1];
    for (int i = 0, j = num_chars, k = 0; i < j; i++, j--, k+=2) {
        locstrbook[k] = strbook[i];
        if (i != j) {
            locstrbook[k+1] = strbook[j];
        }
    }
    strbook2book(locstrbook, locbook);
    mpz_mul_ui(locbook, locbook, 7);
    mpz_add_ui(locbook, locbook, 
}

void book2locbook(mpz_t book, mpz_t locbook) {
    char strbook[num_chars + 1];
    book2strbook(book, strbook);
    strbook2locbook(strbook, locbook);
}

int main() {
    init();

    char strbook[num_chars + 1];
    set_random_strbook(strbook);

    printf("%s", strbook);

    deinit();
    return 0;   
}
