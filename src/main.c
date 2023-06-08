/* main.c
 *
 * Copyright 2023 Francis John Baldon <francisjohnt.baldon@bisu.edu.ph>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#define FILE_NAME "data/library_catalog.csv"
#define PROG_VER "librlog 0.4"
#define MAX_LINE_LEN 2560
#define MAX_FIELD_LEN 256
#define MAX_NUM_FIELDS 10
#define EOF_ERR -1
#define INPUT_ERR -2
#define IO_ERR -3

/* A struct representing a book in a library. */
typedef struct
{
  char title[MAX_FIELD_LEN];            /* The title of the book. */
  char author[MAX_FIELD_LEN];           /* The author of the book. */
  char publisher[MAX_FIELD_LEN];        /* The publisher of the book. */
  char publication_year[MAX_FIELD_LEN]; /* The year the book was published. */
  char isbn[MAX_FIELD_LEN];             /* The International Standard Book Number (ISBN) of the book. */
  char accession_num[MAX_FIELD_LEN];    /* The accession number of the book. */
  char genre[MAX_FIELD_LEN];            /* The genre of the book. */
  char checked_out_by[MAX_FIELD_LEN];   /* The name of the borrower who has checked out the book. */
  char checked_out_date[MAX_FIELD_LEN]; /* The date the book was checked out. */
  char return_date[MAX_FIELD_LEN];      /* The date the book is due to be returned. */
} Book;

/* Variable: books
 * ---------------
 * A pointer to an array of Book structs.
 *
 * This variable is used to store the library's collection of books.
 * The `load_catalog` function reads the contents of a file in CSV format and
 * populates the `books` array with the details of the books.
 * The `add_book`, `delete_book`, `borrow_book`, and `return_book` functions
 * modify the contents of the `books` array.
 */
static Book *books;

/* Variable: num_books
 * -------------------
 * An integer indicating the number of books in the library's collection.
 *
 * This variable is used to keep track of the number of books
 * in the `books` array.
 * It is initialized to 0 at startup and updated by the
 * `load_catalog`, `add_book`, `delete_book`, `borrow_book`,
 * and `return_book` functions.
 */
static int   num_books;

/* Variable: max_books
 * -------------------
 * An integer indicating the maximum number of books that the library can hold.
 *
 * This variable is used to set the initial capacity of the `books` array.
 * It is initialized to a default value at startup and will be updated
 * automatically if the library needs to hold more books.
 */
static int   max_books;

/* Variable: d
 * -----------
 * An integer used to discard excess input characters from stdin.
 *
 * This variable is used to discard excess input characters from stdin
 * after reading a character input with the `scanf` function.
 * It is necessary to prevent these excess characters from causing errors
 * when reading subsequent input.
 */
static int   d;

static int  verify_user                     (void);
static int  print_info                      (void);
static int  load_catalog                    (void);
static int  print_help                      (void);
static int  save_catalog                    (void);
static int  add_book                        (void);
static int  edit_book                       (void);
static int  delete_book                     (void);
static int  borrow_book                     (void);
static int  return_book                     (void);
static int  find_books                      (void);
static int  list_books                      (void);
static int  print_warranty                  (void);
static int  print_book                      (const Book  book);

/* Function: print_book
 * --------------------
 * Print the details of a book to the console in a formatted manner.
 *
 * book: A Book struct containing the details of a book.
 *
 * returns: An integer indicating the success of the function (always 0).
 */
static int
print_book (const Book book)
{
  printf ("Title:            %s\n", book.title);
  printf ("Author:           %s\n", book.author);
  printf ("Publisher:        %s\n", book.publisher);
  printf ("Publication Year: %s\n", book.publication_year);
  printf ("ISBN:             %s\n", book.isbn);
  printf ("Accession Number: %s\n", book.accession_num);
  printf ("Genre:            %s\n", book.genre);
  printf ("Checked Out By:   %s\n", book.checked_out_by);
  printf ("Checked Out Date: %s\n", book.checked_out_date);
  printf ("Return Date:      %s\n", book.return_date);
  putchar ('\n');

  return 0;
}

/* Function: print_warranty
 * ------------------------
 * Print the program's warranty and licensing information to the console.
 *
 * returns: An integer indicating the success of the function (always 0).
 */
static int
print_warranty (void)
{
  puts ("");
  puts (PROG_VER);
  puts ("Copyright 2023 Francis John Baldon\n");

  puts ("  This program is free software; you can redistribute it and/or modify");
  puts ("  it under the terms of the GNU General Public License as published by");
  puts ("  the Free Software Foundation; either version 3 of the License , or");
  puts ("  (at your option) any later version.\n");

  puts ("  This program is distributed in the hope that it will be useful,");
  puts ("  but WITHOUT ANY WARRANTY; without even the implied warranty of");
  puts ("  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the");
  puts ("  GNU General Public License for more details.\n");

  puts ("  You should have received a copy of the GNU General Public License");
  puts ("  along with this program. If not, write to\n");

  puts ("    The Free Software Foundation, Inc.");
  puts ("    51 Franklin Street, Fifth Floor");
  puts ("    Boston, MA 02110-1335  USA\n");

  return 0;
}

/* Function: list_books
 * ---------------------
 * Print the details of all books in the books array
 * to the console in a formatted manner.
 *
 * returns: An integer indicating the success of the function (always 0).
 */
static int
list_books (void)
{
  int i;

  for (i = 1; i < num_books; i++)
    print_book (books[i]);

  return 0;
}

/* Function: find_books
 * --------------------
 * Find books in the library's collection that match a given search criteria.
 *
 * This function prompts the user to enter a search criteria from a menu of options,
 * and then prompts for the specific value to search for.
 * It then searches the books array for books that match the criteria, and prints them to the console.
 *
 * If no books are found that match the criteria, a message is printed to the console.
 *
 * returns: An integer indicating the success of the function.
 * If an error occurs, the appropriate error code is returned.
 */
static int
find_books (void)
{
  char c;
  char buffer[MAX_FIELD_LEN];
  int num_books_found, i;

  puts ("Finding books...");
  puts ("  a - author");
  puts ("  b - go back");
  puts ("  g - genre");
  puts ("  p - publisher");
  puts ("  t - title");
  puts ("  y - publication year");
  printf ("> ");

  if (scanf (" %c", &c) == EOF)
    return EOF_ERR;
  while ((d = getchar ()) != '\n' && d != EOF) {}

  switch (c)
    {
    case 'a':
      printf ("Enter book author: ");
      if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
        {
          if (feof (stdin))
            return EOF_ERR;
          else
            {
              fprintf (stderr, "Error reading from stdin.\n");
              return IO_ERR;
            }
        }
      if (strchr (buffer, '\n') == NULL)
        while ((d = getchar ()) != '\n' && d != EOF) {}
      buffer[strcspn(buffer, "\n")] = '\0';
      num_books_found = 0;
      for (i = 1; i < num_books; i++)
        {
        if (!strcasecmp (buffer, books[i].author))
            {
              num_books_found++;
              print_book (books[i]);
            }
        }
      break;

    case 'b':
      return 0;

    case 'g':
      printf ("Enter book genre: ");
      if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
        {
          if (feof (stdin))
            return EOF_ERR;
          else
            {
              fprintf (stderr, "Error reading from stdin.\n");
              return IO_ERR;
            }
        }
      if (strchr (buffer, '\n') == NULL)
        while ((d = getchar ()) != '\n' && d != EOF) {}
      buffer[strcspn(buffer, "\n")] = '\0';
      num_books_found = 0;
      for (i = 1; i < num_books; i++)
        {
        if (!strcasecmp (buffer, books[i].genre))
            {
              num_books_found++;
              print_book (books[i]);
            }
        }
      break;

    case 'p':
      printf ("Enter book publisher: ");
      if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
        {
          if (feof (stdin))
            return EOF_ERR;
          else
            {
              fprintf (stderr, "Error reading from stdin.\n");
              return IO_ERR;
            }
        }
      if (strchr (buffer, '\n') == NULL)
        while ((d = getchar ()) != '\n' && d != EOF) {}
      buffer[strcspn(buffer, "\n")] = '\0';
      num_books_found = 0;
      for (i = 1; i < num_books; i++)
        {
        if (!strcasecmp (buffer, books[i].publisher))
            {
              num_books_found++;
              print_book (books[i]);
            }
        }
      break;

    case 't':
      printf ("Enter book title: ");
      if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
        {
          if (feof (stdin))
            return EOF_ERR;
          else
            {
              fprintf (stderr, "Error reading from stdin.\n");
              return IO_ERR;
            }
        }
      if (strchr (buffer, '\n') == NULL)
        while ((d = getchar ()) != '\n' && d != EOF) {}
      buffer[strcspn(buffer, "\n")] = '\0';
      num_books_found = 0;
      for (i = 1; i < num_books; i++)
        {
        if (!strcasecmp (buffer, books[i].title))
            {
              num_books_found++;
              print_book (books[i]);
            }
        }
      break;

    case 'y':
      printf ("Enter publication year: ");
      if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
        {
          if (feof (stdin))
            return EOF_ERR;
          else
            {
              fprintf (stderr, "Error reading from stdin.\n");
              return IO_ERR;
            }
        }
      if (strchr (buffer, '\n') == NULL)
        while ((d = getchar ()) != '\n' && d != EOF) {}
      buffer[strcspn(buffer, "\n")] = '\0';
      num_books_found = 0;
      for (i = 1; i < num_books; i++)
        {
        if (!strcasecmp (buffer, books[i].publication_year))
            {
              num_books_found++;
              print_book (books[i]);
            }
        }
      break;

    default:
      fprintf (stderr, "Error invalid input choice.\n");
      return INPUT_ERR;
    }

  if (num_books_found < 1)
    puts ("No match found.");
  else
    printf ("Found %d match/s.\n", num_books_found);

  return 0;
}

/* Function: return_book
 * ---------------------
 * Return a book to the library.
 *
 * This function prompts the user to enter an accession number
 * and a return date for the book to be returned.
 * If the book is not checked out, the function returns successfully.
 * Otherwise, the function updates
 * the `checked_out_by`, `checked_out_date`, and `return_date` fields of the book
 * to indicate that it has been returned,
 * and prints a success message to the console.
 *
 * returns: An integer indicating the success of the function.
 * If the function returns successfully, it returns 0.
 * Otherwise, it returns an error code.
 */
static int
return_book (void)
{
  char accession_num[MAX_FIELD_LEN];
  char return_date[MAX_FIELD_LEN];
  int i;

  printf ("Enter accession number: ");
  if (fgets (accession_num, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }

  if (strchr (accession_num, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}

  accession_num[strcspn (accession_num, "\n")] = '\0';

  for (i = 1; i < num_books; i++)
    {
      if (!strcmp (accession_num, books[i].accession_num))
        break;
    }

  if (i == num_books)
    {
      puts ("Book not found.");
      return 0;
    }

  if (!strcmp (books[i].checked_out_by, ""))
    {
      puts ("Book was already returned.");
      return 0;
    }

  printf ("Enter return date (YYYY-MM-DD): ");
  if (fgets (return_date, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }

  if (strchr (return_date, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}

  return_date[strcspn (return_date, "\n")] = '\0';
  strncpy (books[i].return_date, return_date, MAX_FIELD_LEN);

  strncpy (books[i].checked_out_by, "", MAX_FIELD_LEN - 1);
  strncpy (books[i].checked_out_date, "", MAX_FIELD_LEN - 1);

  printf ("Book %s has been returned on %s.\n", books[i].title, return_date);
  return 0;
}

/* Function: borrow_book
 * ---------------------
 * Borrow a book from the library and update the book's status in the books array.
 *
 * This function prompts the user to enter an accession number
 * and checks if the book is available for borrowing.
 * If the book is available, the user is prompted to enter their name
 * and the date they are borrowing the book.
 * The book's checked out status is then updated in the books array.
 *
 * If the book is already checked out,
 * an error message is printed to the console and the function returns successfully.
 *
 * returns: An integer indicating the success of the function.
 * If an error occurs, the appropriate error code is returned.
 */
static int
borrow_book (void)
{
  int i;
  char accession_num[MAX_FIELD_LEN];
  char checked_out_by[MAX_FIELD_LEN];
  char checked_out_date[MAX_FIELD_LEN];

  printf ("Enter accession number: ");
  if (fgets (accession_num, MAX_FIELD_LEN, stdin) ==  NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }

  if (strchr (accession_num, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}

  accession_num[strcspn (accession_num, "\n")] = '\0';

  for (i = 1; i < num_books; i++)
    {
      if (!strcmp (accession_num, books[i].accession_num))
        break;
    }

  if (i == num_books)
    {
      puts ("Book not found.");
      return 0;
    }

  if (strcmp (books[i].checked_out_by, ""))
    {
      puts ("Book is already checked out.");
      return 0;
    }

  printf ("Enter borrower's name: ");
  if (fgets (checked_out_by, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }

  if (strchr (checked_out_by, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}

  checked_out_by[strcspn (checked_out_by, "\n")] = '\0';
  strncpy (books[i].checked_out_by, checked_out_by, MAX_FIELD_LEN);

  printf ("Enter checked out date (YYYY-MM-DD): ");
  if (fgets (checked_out_date, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }

  if (strchr (checked_out_date, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}

  checked_out_date[strcspn (checked_out_date, "\n")] = '\0';
  strncpy (books[i].checked_out_date, checked_out_date, MAX_FIELD_LEN);

  puts ("Book borrowed successfully.");
  return 0;
}

/* Function: delete_book
 * ---------------------
 * Delete a book from the library's collection.
 *
 * This function prompts the user to enter an accession number
 * and searches the books array for a matching book.
 * If a matching book is found, it is deleted from the array
 * by shifting all subsequent elements one position to the left.
 *
 * If the book is not found, an error message is printed to the console
 * and the function returns successfully.
 *
 * returns: An integer indicating the success of the function.
 * If an error occurs, the appropriate error code is returned.
 */
static int
delete_book (void)
{
  char accession_num[MAX_FIELD_LEN];
  char c;
  int i, j;

  printf ("Enter accession number: ");
  if (fgets (accession_num, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }

  if (strchr (accession_num, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}

  accession_num[strcspn (accession_num, "\n")] = '\0';

  for (i = 1; i < num_books; i++)
    {
      if (!strcmp (accession_num, books[i].accession_num))
        break;
    }

  if (i == num_books)
    {
      puts ("Book not found.");
      return 0;
    }

  print_book (books[i]);
  printf ("Are you sure you want to delete this book? [y/n]: ");
  if (scanf (" %c", &c) == EOF)
    return EOF_ERR;
  while ((d = getchar ()) != '\n' && d != EOF) {}

  if (c == 'n')
    {
      puts ("Operation canceled.");
      return 0;;
    }
  else if (c != 'y' && c != 'n')
    {
      puts ("Invalid input choice. Canceling operation.");
      return INPUT_ERR;
    }

  for (j = i; j < num_books - 1; j++)
    books[j] = books[j + 1];

  num_books--;
  puts ("Book deleted.");
  return 0;
}

/* Function: edit_book
 * -------------------
 * Modify the fields of an existing book in the library's collection.
 *
 * This function prompts the user to enter an accession number
 * and searches the books array for a matching book.
 * If a matching book is found, the user is prompted to enter new values
 * for each field of the book.
 * If the user enters a blank line for a field, the original value for that field is kept.
 *
 * If the book is not found, an error message is printed to the console
 * and the function returns successfully.
 *
 * returns: An integer indicating the success of the function.
 * If an error occurs, the appropriate error code is returned.
 */
static int
edit_book (void)
{
  char accession_num[MAX_FIELD_LEN];
  char buffer[MAX_FIELD_LEN];
  char c;
  Book book;
  int  i;

  puts ("Editing book...");
  printf ("Enter accession number: ");
  if (fgets (accession_num, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }
  if (strchr (accession_num, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  accession_num[strcspn (accession_num, "\n")] = '\0';

  for (i = 1; i < num_books; i++)
    {
      if (!strcmp (accession_num, books[i].accession_num))
        break;
    }

  if (i == num_books)
    {
      puts ("Book not found.");
      return 0;
    }

  print_book (books[i]);
  printf ("Do you want to continue editing? [y/n]: ");
  if (scanf (" %c", &c) == EOF)
    return EOF_ERR;
  while ((d = getchar ()) != '\n' && d != EOF) {}

  if (c == 'n')
    {
      puts ("Operation canceled.");
      return 0;;
    }
  else if (c != 'y' && c != 'n')
    {
      puts ("Invalid input choice. Canceling operation.");
      return INPUT_ERR;
    }

  printf ("Enter book title (%s): ", books[i].title);
  if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }
  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  buffer[strcspn(buffer, "\n")] = '\0';
  if (!strcmp (buffer, ""))
    strncpy (book.title, books[i].title, MAX_FIELD_LEN);
  else
    strncpy (book.title, buffer, MAX_FIELD_LEN);

  printf ("Enter book author (%s): ", books[i].author);
  if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }
  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  buffer[strcspn(buffer, "\n")] = '\0';
  if (!strcmp (buffer, ""))
    strncpy (book.author, books[i].author, MAX_FIELD_LEN);
  else
    strncpy (book.author, buffer, MAX_FIELD_LEN);

  printf ("Enter book publisher (%s): ", books[i].publisher);
  if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }
  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  buffer[strcspn(buffer, "\n")] = '\0';
  if (!strcmp (buffer, ""))
    strncpy (book.publisher, books[i].publisher, MAX_FIELD_LEN);
  else
    strncpy (book.publisher, buffer, MAX_FIELD_LEN);

  printf ("Enter publication year (%s): ", books[i].publication_year);
  if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }
  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  buffer[strcspn(buffer, "\n")] = '\0';
  if (!strcmp (buffer, ""))
    strncpy (book.publication_year, books[i].publication_year, MAX_FIELD_LEN);
  else
    strncpy (book.publication_year, buffer, MAX_FIELD_LEN);

  printf ("Enter book ISBN (%s): ", books[i].isbn);
  if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }
  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  buffer[strcspn(buffer, "\n")] = '\0';
  if (!strcmp (buffer, ""))
    strncpy (book.isbn, books[i].isbn, MAX_FIELD_LEN);
  else
    strncpy (book.isbn, buffer, MAX_FIELD_LEN);

  printf ("Enter book accession number (%s): ", books[i].accession_num);
  if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }
  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  buffer[strcspn(buffer, "\n")] = '\0';
  if (!strcmp (buffer, ""))
    strncpy (book.accession_num, books[i].accession_num, MAX_FIELD_LEN);
  else
    strncpy (book.accession_num, buffer, MAX_FIELD_LEN);

  printf ("Enter genre (%s): ", books[i].genre);
  if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }
  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  buffer[strcspn(buffer, "\n")] = '\0';
  if (!strcmp (buffer, ""))
    strncpy (book.genre, books[i].genre, MAX_FIELD_LEN);
  else
    strncpy (book.genre, buffer, MAX_FIELD_LEN);

  printf ("Enter checked out by (%s): ", books[i].checked_out_by);
  if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }
  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  buffer[strcspn(buffer, "\n")] = '\0';
  if (!strcmp (buffer, ""))
    strncpy (book.checked_out_by, books[i].checked_out_by, MAX_FIELD_LEN);
  else
    strncpy (book.checked_out_by, buffer, MAX_FIELD_LEN);

  printf ("Enter checked out date (%s): ", books[i].checked_out_date);
  if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }
  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  buffer[strcspn(buffer, "\n")] = '\0';
  if (!strcmp (buffer, ""))
    strncpy (book.checked_out_date, books[i].checked_out_date, MAX_FIELD_LEN);
  else
    strncpy (book.checked_out_date, buffer, MAX_FIELD_LEN);

  printf ("Enter return date (%s): ", books[i].return_date);
  if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }
  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  buffer[strcspn(buffer, "\n")] = '\0';
  if (!strcmp (buffer, ""))
    strncpy (book.return_date, books[i].return_date, MAX_FIELD_LEN);
  else
    strncpy (book.return_date, buffer, MAX_FIELD_LEN);

  books[i] = book;
  puts ("Book edited successfully.");
  return 0;
}

/* Function: add_book
 * ------------------
 * Add a book to the library's collection.
 *
 * This function prompts the user to enter details
 * about a book and adds the book to the books array.
 *
 * If the maximum number of books has been reached,
 * the books array is resized to hold additional books.
 *
 * returns: An integer indicating the success of the function.
 * If an error occurs, the appropriate error code is returned.
 */
static int
add_book (void)
{
  char buffer[MAX_FIELD_LEN];
  Book book;
  int i;

  if (num_books >= max_books)
    {
      max_books += 500;
      books = (Book *) realloc (books, sizeof (Book) * max_books);
      if (books == NULL)
        {
          fprintf (stderr, "Failed to reallocate memory.\n");
          return IO_ERR;
        }
    }

  printf ("Enter book title: ");
  if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }

  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}

  buffer[strcspn(buffer, "\n")] = '\0';
  strncpy (book.title, buffer, MAX_FIELD_LEN);

  printf ("Enter book author: ");
  if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }

  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}

  buffer[strcspn(buffer, "\n")] = '\0';
  strncpy (book.author, buffer, MAX_FIELD_LEN);

  printf ("Enter book publisher: ");
  if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }

  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}

  buffer[strcspn(buffer, "\n")] = '\0';
  strncpy (book.publisher, buffer, MAX_FIELD_LEN);

  printf ("Enter publication year: ");
  if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }

  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}

  buffer[strcspn(buffer, "\n")] = '\0';
  strncpy (book.publication_year, buffer, MAX_FIELD_LEN);

  printf ("Enter book ISBN: ");
  if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }

  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}

  buffer[strcspn(buffer, "\n")] = '\0';
  strncpy (book.isbn, buffer, MAX_FIELD_LEN);

access_num_not_unique:
  printf ("Enter book accession number (%d): ", num_books);
  if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }

  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}

  buffer[strcspn(buffer, "\n")] = '\0';

  for (i = 1; i < num_books; i++)
    {
      if (!strcmp (buffer, books[i].accession_num))
        {
          fprintf (stderr, "Error: The entered accession number is not unique.\n");
          goto access_num_not_unique;
        }
    }
  strncpy (book.accession_num, buffer, MAX_FIELD_LEN);

  printf ("Enter book genre: ");
  if (fgets (buffer, MAX_FIELD_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }

  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}

  buffer[strcspn(buffer, "\n")] = '\0';
  strncpy (book.genre, buffer, MAX_FIELD_LEN);

  strncpy (book.checked_out_by, "", MAX_FIELD_LEN - 1);
  strncpy (book.checked_out_date, "", MAX_FIELD_LEN - 1);
  strncpy (book.return_date, "", MAX_FIELD_LEN - 1);

  memcpy (&books[num_books], &book, sizeof (Book));
  books[num_books].title[MAX_FIELD_LEN - 1] = '\0';
  books[num_books].author[MAX_FIELD_LEN - 1] = '\0';
  books[num_books].publisher[MAX_FIELD_LEN - 1] = '\0';
  books[num_books].publication_year[MAX_FIELD_LEN - 1] = '\0';
  books[num_books].isbn[MAX_FIELD_LEN - 1] = '\0';
  books[num_books].accession_num[MAX_FIELD_LEN - 1] = '\0';
  books[num_books].genre[MAX_FIELD_LEN - 1] = '\0';
  books[num_books].checked_out_by[MAX_FIELD_LEN - 1] = '\0';
  books[num_books].checked_out_date[MAX_FIELD_LEN - 1] = '\0';
  books[num_books].return_date[MAX_FIELD_LEN - 1] = '\0';

  num_books++;
  puts ("Book added successfully.");
  return 0;
}

/* Function: save_catalog
 * ----------------------
 * Save the library's collection to a file.
 *
 * This function saves the details of the books in the books array
 * to a file in CSV format.
 *
 * If an error occurs while saving the file,
 * an error message is printed to the console
 * and the appropriate error code is returned.
 *
 * returns: An integer indicating the success of the function.
 * If an error occurs, the appropriate error code is returned.
 */
static int
save_catalog (void)
{
  FILE *fp;
  int i;

  fp = fopen (FILE_NAME, "w");
  if (fp == NULL)
    {
      fprintf (stderr, "Error saving catalog.\n");
      return IO_ERR;
    }

  fprintf (fp, "Title,Author,Publisher,Publication Year,ISBN,Accession Number,Genre,Checked Out By,Checked Out Date,Return Date\n");

  for (i = 1; i < num_books; i++)
    {
      fprintf(fp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
              books[i].title,
              books[i].author,
              books[i].publisher,
              books[i].publication_year,
              books[i].isbn,
              books[i].accession_num,
              books[i].genre,
              books[i].checked_out_by,
              books[i].checked_out_date,
              books[i].return_date);
    }

  fclose(fp);
  return 0;
}

/* Function: print_help
 * --------------------
 * Print a help message to the console.
 *
 * This function prints a list of commands
 * and their corresponding functions to the console.
 *
 * returns: An integer indicating the success of the function.
 * In this case, the function always returns successfully.
 */
static int
print_help (void)
{
  puts (" a - add book");
  puts (" b - borrow book");
  puts (" d - delete book");
  puts (" e - edit book");
  puts (" f - find books");
  puts (" h - show program help");
  puts (" l - list books");
  puts (" q - quit program");
  puts (" r - return book");
  puts (" w - show program warranty");

  return 0;
}

/* Function: print_info
 * --------------------
 * Print information about the program to the console.
 *
 * This function prints the program version, copyright information,
 * and a disclaimer to the console.
 *
 * returns: An integer indicating the success of the function.
 * In this case, the function always returns successfully.
 */
static int
print_info (void)
{
  puts (PROG_VER);
  puts ("Copyright 2023 Francis John Baldon");
  puts ("This is free software with ABSOLUTELY NO WARRANTY.");
  puts ("For help type 'h'.");

  return 0;
}

/* Function: load_catalog
 * ----------------------
 * Load the library's collection from a file.
 *
 * This function reads the contents of a file in CSV format
 * and populates the books array with the details of the books.
 *
 * If an error occurs while loading the file,
 * an error message is printed to the console
 * and the appropriate error code is returned.
 *
 * returns: An integer indicating the number of books loaded from the file.
 * If an error occurs, the appropriate error code is returned.
 */
static int
load_catalog (void)
{
  FILE *fp;
  char line[MAX_LINE_LEN];
  char *field;

  fp = fopen (FILE_NAME, "r");
  if (fp == NULL)
    {
      fprintf (stderr, "Error loading catalog.\n");
      return IO_ERR;
    }

  num_books = 0;
  while (fgets (line, MAX_LINE_LEN, fp) != NULL)
    {
      field = strtok (line, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].title, field, MAX_FIELD_LEN - 1);
        }

      field = strtok (NULL, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].author, field, MAX_FIELD_LEN - 1);
        }

      field = strtok (NULL, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].publisher, field, MAX_FIELD_LEN - 1);
        }

      field = strtok (NULL, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].publication_year, field, MAX_FIELD_LEN - 1);
        }

      field = strtok (NULL, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].isbn, field, MAX_FIELD_LEN - 1);
        }

      field = strtok (NULL, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].accession_num, field, MAX_FIELD_LEN - 1);
        }

      field = strtok (NULL, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].genre, field, MAX_FIELD_LEN - 1);
        }

      field = strtok (NULL, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].checked_out_by, field, MAX_FIELD_LEN - 1);
        }

      field = strtok (NULL, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].checked_out_date, field, MAX_FIELD_LEN - 1);
        }

      field = strtok (NULL, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].return_date, field, MAX_FIELD_LEN - 1);
        }

      num_books++;
    }

  if (ferror (fp))
    {
      fprintf (stderr, "Error parsing file.\n");
      fclose (fp);
      return IO_ERR;
    }

  fclose (fp);
  return num_books;
}

/* Function: verify_user
 * ---------------------
 * Verify the user's identity.
 *
 * This function prompts the user to enter a password
 * and compares it to a stored password.
 * If the passwords match, the function returns successfully.
 * Otherwise, the function returns an error code.
 *
 * returns: An integer indicating the success of the function.
 * If the entered password matches the stored password,
 * the function returns successfully.
 * Otherwise, it returns an error code.
 */
static int
verify_user (void)
{
  const char stored_pass[] = "bisu";
  char entered_pass[MAX_LINE_LEN];

  printf ("Enter password: ");
  if (fgets (entered_pass, MAX_LINE_LEN, stdin) == NULL)
    {
      if (feof (stdin))
        return EOF_ERR;
      else
        {
          fprintf (stderr, "Error reading from stdin.\n");
          return IO_ERR;
        }
    }

  if (strchr (entered_pass, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}

  entered_pass[strcspn (entered_pass, "\n")] = '\0';

  if (!strcmp (stored_pass, entered_pass))
    return 1;

  puts ("Sorry, try again.");
  return 0;
}

/* Function: main
 * --------------
 * The main function of the library management program.
 *
 * This function contains the main loop of the program,
 * which repeatedly prompts the user
 * to enter a command and executes the corresponding function.
 * The program loads the library's collection from a file at startup
 * and saves it to the file before exiting.
 * The function also verifies the user's identity with a password
 * before allowing access to the program.
 *
 * returns: An integer indicating the success of the program.
 * If the program exits successfully, the function returns EXIT_SUCCESS.
 * Otherwise, it returns EXIT_FAILURE.
 */
int
main (void)
{
  char c;

  max_books = 1000;
  books = (Book *) malloc (sizeof (Book) * max_books);
  if (books == NULL)
    {
      fprintf (stderr, "Failed to allocate memory.\n");
      return EXIT_FAILURE;
    }

  while (1)
    {
      int is_verified;

      is_verified = verify_user ();
      if (is_verified == 1)
        break;
      else if (is_verified < 0)
        goto failure;
      else
        continue;
    }
  system ("clear");
  print_info ();

  num_books = load_catalog ();
  if (num_books < 0)
    goto failure;

  while (1)
    {
      int status;

      printf (">>> ");
      if (scanf (" %c", &c) == EOF)
        goto success;
      while ((d = getchar ()) != '\n' && d != EOF) {}

      switch (c)
        {
        case 'a':
          status = add_book ();
          break;

        case 'b':
          status = borrow_book ();
          break;

        case 'd':
          status = delete_book ();
          break;

        case 'e':
          status = edit_book ();
          break;

        case 'f':
          status = find_books ();
          break;

        case 'h':
          status = print_help ();
          break;

        case 'l':
          status = list_books ();
          break;

        case 'q':
          goto success;

        case 'r':
          status = return_book ();
          break;

        case 'w':
          status = print_warranty ();
          break;

        default:
          fprintf (stderr, "Invalid input. Type 'h' for help.\n");
          continue;
        }

      switch (status)
        {
        case 0:
          continue;

        case EOF_ERR:
          goto success;

        case INPUT_ERR:
          continue;

        case IO_ERR:
          goto failure;

        default:
          fprintf (stderr, "Unexpected status code.\n");
          continue;
        }
    }

failure:
  free (books);
  return EXIT_FAILURE;

success:
  save_catalog ();
  free (books);
  return EXIT_SUCCESS;
}

