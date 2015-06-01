# Library of Babel

Just some tools and utilities relating to the Library of Babel.  
See http://libraryofbabel.info for more information.

## lob.c

This contains some things I should probably detail here.

Requirements:

- You need `gcc`. I mean, this should be expected.
- Also `gmp`, which is [this thing](http://gmplib.org).

Terms:

- `loc`
    - A location, specified with a hexagon, a shelf, a row, and a book.
- `locbook`
    - A `book`/index, which specifies the "location" of a book.
    - Assumes you have a theoretical list of all books.
    - Then, you split up the theoretical list by hexagons, etc.
    - That is how you turn this into `loc`.
- `book`
    - A `book`/index of a book in a straightforward 0 -> maximum list.
    - This is the contents of the book encoded as a number.
- `str`
    - The base 29 string of a `book`.
    - **Not** the contents of the book.
- `strbook`
    - The contents of the `book`, usually converted from a `str`.

Useful Functions:

- `set_random_book`
- `init_set_random_book`
    - Does `mpz_init(book)` and then calls `set_random_book`.
- `book2strbook`
- `strbook2book`
- `locbook2strbook`
- `strbook2locbook`
