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

#define FILE_NAME "data/library_catalog.csv"
#define PROG_VERSION "librlog 0.2"
#define MAX_LINE_LENGTH 2560
#define MAX_FIELD_LENGTH 256
#define MAX_NUM_FIELDS 10
#define MAX_BOOKS 1000

typedef struct
{
  char title[MAX_FIELD_LENGTH];
  char author[MAX_FIELD_LENGTH];
  char publisher[MAX_FIELD_LENGTH];
  char publication_year[MAX_FIELD_LENGTH];
  char isbn[MAX_FIELD_LENGTH];
  char accession_number[MAX_FIELD_LENGTH];
  char genre[MAX_FIELD_LENGTH];
  char checked_out_by[MAX_FIELD_LENGTH];
  char checked_out_date[MAX_FIELD_LENGTH];
  char return_date[MAX_FIELD_LENGTH];
} Book;

enum {
  TITLE,
  AUTHOR,
  PUBLISHER,
  PUBLICATION_YEAR,
  ACCESSION_NUMBER,
  GENRE,
};

static Book *books;
static int num_books;
static int d;

static int  verify_user                     (void);
static void print_info                      (void);
static int  load_catalog                    (void);
static void print_help                      (void);
static int  save_catalog                    (void);

static int  add_book                        (void);
static int  delete_book                     (void);
static int  borrow_book                     (void);
static int  return_book                     (void);
static int  find_book                       (void);
static int  sort_books                      (void);
static int  list_books                      (void);
static void print_warranty                  (void);
static void quit_prog                       (void);

static int  get_fields_of_new_book          (Book       *book);
static int  search_book_by_field_and_string (int         field,
                                             const char *str,
                                             int         i);
static void print_a_book_s_fields           (const Book  book);

/*
 * This function prints out the fields of a Book structure.
 * It takes a Book structure as its argument.
 * The fields printed are:
 *   - Title
 *   - Author
 *   - Publisher
 *   - Publication Year
 *   - ISBN
 *   - Accession Number
 *   - Genre
 *   - Checked Out By
 *   - Checked Out Date
 *   - Return Date
 */
static void
print_a_book_s_fields (const Book book)
{
  puts ("");
  printf ("Title:            %s\n", book.title);
  printf ("Author:           %s\n", book.author);
  printf ("Publisher:        %s\n", book.publisher);
  printf ("Publication Year: %s\n", book.publication_year);
  printf ("ISBN:             %s\n", book.isbn);
  printf ("Accession Number: %s\n", book.accession_number);
  printf ("Genre:            %s\n", book.genre);
  printf ("Checked Out By:   %s\n", book.checked_out_by);
  printf ("Checked Out Date: %s\n", book.checked_out_date);
  printf ("Return Date:      %s\n", book.return_date);
}

/*
 * This function searches for a book in the library by comparing a given field
 * of the Book structure with a given string. It takes three arguments:
 *   - field: an integer representing the field of the Book structure to be searched.
 *   - str: a pointer to a const char string containing the value to be searched for.
 *   - i: an integer representing the index of the starting book for the search.
 * The function loops through the books in the library starting at the given index
 * until it finds a match for the given field and string. It returns the index of
 * the matching book in the library, or 0 if no match is found. If an invalid field
 * value is given, the function returns -1 and prints an error message to stderr.
 */
static int
search_book_by_field_and_string (int         field,
                                 const char *str,
                                 int         i)
{
  while (i < num_books)
    {
      switch (field)
        {
        case TITLE:
          if (strcmp (books[i].title, str) == 0)
            return i;
          break;

        case AUTHOR:
          if (strcmp (books[i].author, str) == 0)
            return i;
          break;

        case PUBLISHER:
          if (strcmp (books[i].publisher, str) == 0)
            return i;
          break;

        case PUBLICATION_YEAR:
          if (strcmp (books[i].publication_year, str) == 0)
            return i;
          break;

        case ACCESSION_NUMBER:
          if (strcmp (books[i].accession_number, str) == 0)
            return i;
          break;

        case GENRE:
          if (strcmp (books[i].genre, str) == 0)
            return i;
          break;

        default:
          fprintf (stderr, "Error: invalid field.\n");
          return -1;
        }

      i++;
    }

  return 0;
}

/*
 * This function prompts the user to enter the fields of a new Book structure.
 * It takes a pointer to a Book structure as its argument.
 * The fields entered are:
 *   - Title
 *   - Author
 *   - Publisher
 *   - Publication Year
 *   - ISBN
 *   - Accession Number
 *   - Genre
 * The function reads each field from standard input using fgets and stores it in
 * a buffer. It then removes any trailing newline characters and copies the field
 * value to the corresponding field of the Book structure using strncpy. The
 * function also initializes the checked_out_by, checked_out_date, and return_date
 * fields to empty strings.
 * The function returns 0 on success and -1 if fgets fails to read input.
 */
static int
get_fields_of_new_book (Book *book)
{
  char buffer[MAX_FIELD_LENGTH];

  printf ("Enter the book title:       ");
  if (fgets (buffer, MAX_FIELD_LENGTH, stdin) == NULL)
    {
      fprintf (stderr, "Error: fgets failed.\n");
      return -1;
    }
  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  buffer[strcspn(buffer, "\n")] = '\0';
  strncpy (book->title, buffer, MAX_FIELD_LENGTH - 1);
  book->title[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Enter the author name:      ");
  if (fgets (buffer, MAX_FIELD_LENGTH, stdin) == NULL)
    {
      fprintf (stderr, "Error: fgets failed.\n");
      return -1;
    }
  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  buffer[strcspn(buffer, "\n")] = '\0';
  strncpy (book->author, buffer, MAX_FIELD_LENGTH - 1);
  book->author[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Enter the publisher name:   ");
  if (fgets (buffer, MAX_FIELD_LENGTH, stdin) == NULL)
    {
      fprintf (stderr, "Error: fgets failed.\n");
      return -1;
    }
  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  buffer[strcspn(buffer, "\n")] = '\0';
  strncpy (book->publisher, buffer, MAX_FIELD_LENGTH - 1);
  book->publisher[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Enter the publication year: ");
  if (fgets (buffer, MAX_FIELD_LENGTH, stdin) == NULL)
    {
      fprintf (stderr, "Error: fgets failed.\n");
      return -1;
    }
  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  buffer[strcspn(buffer, "\n")] = '\0';
  strncpy (book->publication_year, buffer, MAX_FIELD_LENGTH - 1);
  book->publication_year[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Enter the ISBN:             ");
  if (fgets (buffer, MAX_FIELD_LENGTH, stdin) == NULL)
    {
      fprintf (stderr, "Error: fgets failed.\n");
      return -1;
    }
  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  buffer[strcspn(buffer, "\n")] = '\0';
  strncpy (book->isbn, buffer, MAX_FIELD_LENGTH - 1);
  book->isbn[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Enter the accession number: ");
  if (fgets (buffer, MAX_FIELD_LENGTH, stdin) == NULL)
    {
      fprintf (stderr, "Error: fgets failed.\n");
      return -1;
    }
  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  buffer[strcspn(buffer, "\n")] = '\0';
  strncpy (book->accession_number, buffer, MAX_FIELD_LENGTH - 1);
  book->accession_number[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Enter the genre:            ");
  if (fgets (buffer, MAX_FIELD_LENGTH, stdin) == NULL)
    {
      fprintf (stderr, "Error: fgets failed.\n");
      return -1;
    }
  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  buffer[strcspn(buffer, "\n")] = '\0';
  strncpy (book->genre, buffer, MAX_FIELD_LENGTH - 1);
  book->genre[MAX_FIELD_LENGTH - 1] = '\0';

  strncpy (book->checked_out_by, "", MAX_FIELD_LENGTH - 1);
  book->checked_out_by[MAX_FIELD_LENGTH - 1] = '\0';

  strncpy (book->checked_out_date, "", MAX_FIELD_LENGTH - 1);
  book->checked_out_date[MAX_FIELD_LENGTH - 1] = '\0';

  strncpy (book->return_date, "", MAX_FIELD_LENGTH - 1);
  book->return_date[MAX_FIELD_LENGTH - 1] = '\0';

  return 0;
}

/*
 * This function simply prints the message "Exited." to standard output.
 * It is intended to be used as the action of an "exit" menu item or button
 * in a graphical user interface or other interactive program.
 */
static void
quit_prog (void)
{
  puts ("Exited.");
}

/*
 * This function prints information about the program's warranty and licensing to standard output.
 * The information includes the program's version number, copyright notice, and the terms of
 * the GNU General Public License under which the program is distributed.
 * The function is intended to be used as a menu item or button in a graphical user interface or
 * other interactive program to display legal information about the program.
 */
static void
print_warranty (void)
{
  puts ("");
  puts (PROG_VERSION);
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
}

/*
 * This function lists the fields of all Book structures in the global array "books",
 * except for the first element (index 0), which is assumed to be unused. The function
 * does this by calling the function "print_a_book_s_fields" for each Book structure
 * in the array, passing it the Book structure as an argument.
 * The function returns 0 on success.
 */
static int
list_books (void)
{
  int i;

  for (i = 1; i < num_books; i++)
    print_a_book_s_fields (books[i]);
  return 0;
}

/*
 * This function prompts the user to select a field by which to sort the Book structures in the global array "books",
 * and then lists the fields of all Book structures that match the user's search string for that field. The function
 * does this by calling the function "search_book_by_field_and_string" for each Book structure in the array, passing
 * it the field and search string specified by the user. The function then calls the function "print_a_book_s_fields"
 * for each Book structure that matches the search criteria. The function handles input and output using standard
 * input and output functions such as scanf, fgets, and printf.
 * The function returns 0 on success and -1 if scanf or fgets fails to read input.
 */
static int
sort_books (void)
{
  char c;
  char buffer[MAX_FIELD_LENGTH];
  int i = 0;

  puts ("Type [?]: [a]uthor, [g]enre, [p]ublisher, publication_[y]ear, [b]ack.");
  printf ("     [ ]: ");
  if (scanf (" %c", &c) == EOF)
    {
      fprintf (stderr, "Error: scanf failed.\n");
      return -1;
    }
  while ((d = getchar ()) != '\n' && d != EOF) {}

  switch (c)
    {
    case 'a':
      printf ("Enter book author: ");
      if (fgets (buffer, MAX_FIELD_LENGTH, stdin) == NULL)
        {
          fprintf (stderr, "Error: fgets failed.\n");
          return -1;
        }
      if (strchr (buffer, '\n') == NULL)
        while ((d = getchar ()) != '\n' && d != EOF) {}
      buffer[strcspn(buffer, "\n")] = '\0';
      while (0 < (i = search_book_by_field_and_string (AUTHOR, buffer, ++i)))
        print_a_book_s_fields (books[i]);
      break;

    case 'b':
      return 0;

    case 'g':
      printf ("Enter book genre: ");
      if (fgets (buffer, MAX_FIELD_LENGTH, stdin) == NULL)
        {
          fprintf (stderr, "Error: fgets failed.\n");
          return -1;
        }
      if (strchr (buffer, '\n') == NULL)
        while ((d = getchar ()) != '\n' && d != EOF) {}
      buffer[strcspn(buffer, "\n")] = '\0';
      while (0 < (i = search_book_by_field_and_string (GENRE, buffer, ++i)))
        print_a_book_s_fields (books[i]);
      break;

    case 'p':
      printf ("Enter book publisher: ");
      if (fgets (buffer, MAX_FIELD_LENGTH, stdin) == NULL)
        {
          fprintf (stderr, "Error: fgets failed.\n");
          return -1;
        }
      if (strchr (buffer, '\n') == NULL)
        while ((d = getchar ()) != '\n' && d != EOF) {}
      buffer[strcspn(buffer, "\n")] = '\0';
      while (0 < (i = search_book_by_field_and_string (PUBLISHER, buffer, ++i)))
        print_a_book_s_fields (books[i]);
      break;

    case 'y':
      printf ("Enter publication year: ");
      if (fgets (buffer, MAX_FIELD_LENGTH, stdin) == NULL)
        {
          fprintf (stderr, "Error: fgets failed.\n");
          return -1;
        }
      if (strchr (buffer, '\n') == NULL)
        while ((d = getchar ()) != '\n' && d != EOF) {}
      buffer[strcspn(buffer, "\n")] = '\0';
      while (0 < (i = search_book_by_field_and_string (PUBLICATION_YEAR, buffer, ++i)))
        print_a_book_s_fields (books[i]);
      break;

    default:
      fprintf (stderr, "Error: invalid input.\n");
      return -1;
    }

  return 0;
}

/*
 * This function prompts the user to enter a book title, reads the input using the function "fgets",
 * and then searches the global array "books" for Book structures that match the search string for
 * the TITLE field. The function does this by calling the function "search_book_by_field_and_string"
 * for each Book structure in the array, passing it the TITLE field and search string specified by
 * the user. The function then calls the function "print_a_book_s_fields" for each Book structure that
 * matches the search criteria. The function returns 0 on success and -1 if fgets fails to read input.
 */
static int
find_book (void)
{
  char buffer[MAX_FIELD_LENGTH];
  int i = 0;

  printf ("Enter the book title: ");
  if (fgets (buffer, MAX_FIELD_LENGTH, stdin) == NULL)
    {
      fprintf (stderr, "Error: fgets failed.\n");
      return -1;
    }

  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}

  buffer[strcspn (buffer, "\n")] = '\0';
  while (0 < (i = search_book_by_field_and_string (TITLE, buffer, ++i)))
    print_a_book_s_fields (books[i]);

  return 0;
}

/*
 * This function prompts the user to enter an accession number for a book that is being returned.
 * It then searches the global array "books" for a Book structure that matches the accession number
 * by calling the function "search_book_by_field_and_string". If a match is found, the function
 * checks if the Book structure is already returned by checking the "checked_out_by" field.
 * If the book is already returned, the function outputs a message and returns 0. Otherwise, the
 * function prompts the user to enter a return date in the format "YYYY-MM-DD", reads the input
 * using the function "fgets", and updates the relevant fields in the Book structure to reflect
 * the return. The function outputs a message indicating that the book has been returned and the
 * return date. The function returns 0 on success and -1 if fgets fails to read input.
 */
static int
return_book (void)
{
  char accession_number[MAX_FIELD_LENGTH];
  char return_date[MAX_FIELD_LENGTH];
  int i;

  printf ("Enter accession number: ");
  if (fgets (accession_number, MAX_FIELD_LENGTH, stdin) == NULL)
    {
      fprintf (stderr, "Error: fgets failed.\n");
      return -1;
    }

  if (strchr (accession_number, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}

  accession_number[strcspn (accession_number, "\n")] = '\0';

  i = search_book_by_field_and_string (ACCESSION_NUMBER, accession_number, 1);
  if (i == -1)
    return 0;

  if (strcmp (books[i].checked_out_by, "") == 0)
    {
      puts ("Book is already returned.");
      return 0;
    }

  printf ("Enter return date (YYYY-MM-DD): ");
  if (fgets (return_date, MAX_FIELD_LENGTH, stdin) == NULL)
    {
      fprintf (stderr, "Error: fgets failed.\n");
      return -1;
    }

  if (strchr (return_date, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}

  return_date[strcspn (return_date, "\n")] = '\0';

  strncpy (books[i].checked_out_by, "", MAX_FIELD_LENGTH - 1);
  books[i].checked_out_by[MAX_FIELD_LENGTH - 1] = '\0';

  strncpy (books[i].checked_out_date, "", MAX_FIELD_LENGTH - 1);
  books[i].checked_out_date[MAX_FIELD_LENGTH - 1] = '\0';

  strncpy (books[i].return_date, return_date, MAX_FIELD_LENGTH - 1);
  books[i].return_date[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Book %s has been returned on %s.\n", books[i].title, return_date);
  return 0;
}

/*
 * This function prompts the user to enter an accession number for a book that they want to borrow.
 * It then searches the global array "books" for a Book structure that matches the accession number
 * by calling the function "search_book_by_field_and_string". If a match is found, the function
 * checks if the Book structure is already checked out by checking the "checked_out_by" field.
 * If the book is already checked out, the function outputs a message and returns 0. Otherwise, the
 * function prompts the user to enter their name and the current date in the format "YYYY-MM-DD"
 * using the function "fgets". The function then updates the relevant fields in the Book structure
 * to reflect the checkout. The function outputs a message indicating that the book has been
 * borrowed successfully. The function returns 0 on success and -1 if fgets fails to read input.
 */
static int
borrow_book (void)
{
  int i;
  char accession_number[MAX_FIELD_LENGTH];
  char checked_out_by[MAX_FIELD_LENGTH];
  char checked_out_date[MAX_FIELD_LENGTH];

  printf ("Enter accession number: ");
  if (fgets (accession_number, MAX_FIELD_LENGTH, stdin) ==  NULL)
    {
      fprintf (stderr, "Error: fgets failed.\n");
      return -1;
    }
  if (strchr (accession_number, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  accession_number[strcspn (accession_number, "\n")] = '\0';

  i = search_book_by_field_and_string (ACCESSION_NUMBER, accession_number, 1);
  if (i == -1)
    return 0;

  if (strcmp (books[i].checked_out_by, "") != 0)
    {
      puts ("Book is already checked out.");
      return 0;
    }

  printf ("Enter your name: ");
  if (fgets (checked_out_by, MAX_FIELD_LENGTH, stdin) == NULL)
    {
      fprintf (stderr, "Error: fgets failed.\n");
      return -1;
    }
  if (strchr (checked_out_by, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  checked_out_by[strcspn (checked_out_by, "\n")] = '\0';
  strncpy (books[i].checked_out_by, checked_out_by, MAX_FIELD_LENGTH - 1);
  books[i].checked_out_by[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Enter the date you are borrowing the book (YYYY-MM-DD): ");
  if (fgets (checked_out_date, MAX_FIELD_LENGTH, stdin) == NULL)
    {
      fprintf (stderr, "Error: fgets failed.\n");
      return -1;
    }
  if (strchr (checked_out_by, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  checked_out_date[strcspn (checked_out_date, "\n")] = '\0';
  strncpy (books[i].checked_out_date, checked_out_date, MAX_FIELD_LENGTH -1);
  books[i].checked_out_date[MAX_FIELD_LENGTH - 1] = '\0';

  puts ("Book borrowed successfully.");
  return 0;
}

/*
 * This function prompts the user to enter an accession number for a book that they want to delete.
 * It then searches the global array "books" for a Book structure that matches the accession number
 * by calling the function "search_book_by_field_and_string". If a match is found, the function
 * deletes the Book structure by shifting all subsequent elements of the array "books" one index to
 * the left and decrementing the variable "num_books". The function outputs a message indicating that
 * the book has been deleted successfully. If the Book structure is not found in the array "books",
 * the function outputs a message indicating that the book was not found. The function returns 0 on
 * success, -1 if fgets fails to read input, and -1 if search_book_by_field_and_string fails to find
 * a matching book.
 */
static int
delete_book (void)
{
  char buffer[MAX_FIELD_LENGTH];
  int i, j;

  printf ("Enter accession number: ");
  if (fgets (buffer, MAX_FIELD_LENGTH, stdin) == NULL)
    {
      fprintf (stderr, "Error: fgets failed.\n");
      return -1;
    }
  if (strchr (buffer, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}
  buffer[strcspn (buffer, "\n")] = '\0';

  i = search_book_by_field_and_string (ACCESSION_NUMBER, buffer, 1);
  if (i == 0)
    {
      puts ("Book not found.");
      return 0;
    }
  else if (i > 0)
    {
      for (j = i; j < num_books - 1; j++)
        books[j] = books[j + 1];
      num_books--;
      puts ("Book deleted.");
      return 0;
    }
  else
    {
      return -1;
    }
}

/*
 * This function prompts the user to enter information for a new book by calling the function
 * "get_fields_of_new_book". If the function returns -1, indicating that the user did not provide
 * valid input for all fields, the function returns -1. Otherwise, the function checks if the
 * maximum number of books has been reached. If so, the function outputs an error message and
 * returns -1. Otherwise, the function copies the Book structure into the global array "books"
 * and increments the variable "num_books". Finally, the function returns 0 on success and -1 if
 * the user does not provide valid input for all fields or if the maximum number of books has been
 * reached.
 */
static int
add_book (void)
{
  Book book;

  if (get_fields_of_new_book (&book) == -1)
    return -1;

  if (num_books >= MAX_BOOKS)
    {
      puts ("Maximum number of books reached.");
      return 0;
    }

  memcpy (&books[num_books], &book, sizeof (Book));
  books[num_books].title[MAX_FIELD_LENGTH - 1] = '\0';
  books[num_books].author[MAX_FIELD_LENGTH - 1] = '\0';
  books[num_books].publisher[MAX_FIELD_LENGTH - 1] = '\0';
  books[num_books].publication_year[MAX_FIELD_LENGTH - 1] = '\0';
  books[num_books].isbn[MAX_FIELD_LENGTH - 1] = '\0';
  books[num_books].accession_number[MAX_FIELD_LENGTH - 1] = '\0';
  books[num_books].genre[MAX_FIELD_LENGTH - 1] = '\0';
  books[num_books].checked_out_by[MAX_FIELD_LENGTH - 1] = '\0';
  books[num_books].checked_out_date[MAX_FIELD_LENGTH - 1] = '\0';
  books[num_books].return_date[MAX_FIELD_LENGTH - 1] = '\0';

  num_books++;
  return 0;
}

/*
 * This function saves the current catalog of books to a file named "FILE_NAME". If the file
 * cannot be opened for writing, the function outputs an error message and returns -1. Otherwise,
 * the function writes the column headers to the file, followed by the data for each book in the
 * catalog. The function assumes that the global array "books" contains valid Book structures up
 * to the index "num_books". Finally, the function closes the file and returns 0 on success and -1
 * if an error occurs while writing to the file.
 */
static int
save_catalog (void)
{
  FILE *fp;
  int i;

  fp = fopen (FILE_NAME, "w");
  if (fp == NULL)
    {
      fprintf (stderr, "Error: could not open file for writing.\n");
      return -1;
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
              books[i].accession_number,
              books[i].genre,
              books[i].checked_out_by,
              books[i].checked_out_date,
              books[i].return_date);
    }

  fclose(fp);
  return 0;
}

/*
 * This function prints a list of commands that the user can enter to perform various actions in the
 * program. The list includes abbreviations for each command in square brackets, followed by a brief
 * description of what the command does. The function uses the standard output stream to print the
 * list of commands and descriptions, and does not return a value.
 */
static void
print_help (void)
{
  puts ("Type [?]: [a]dd_book, [d]elete_book, [b]orrow_book, [r]eturn_book");
  puts ("          [f]ind_book, [s]ort_books, [l]ist_books");
  puts ("          [h]elp, [w]arranty, [q]uit");
}

/*
 * This function prints information about the program, including its version number, copyright
 * holder, and a disclaimer that the program comes with no warranty. The function also prints a
 * message telling the user how to get help, by typing 'h'. The function uses the standard output
 * stream to print the information, and does not return a value.
 */
static void
print_info (void)
{
  puts (PROG_VERSION);
  puts ("Copyright 2023 Francis John Baldon");
  puts ("This is free software with ABSOLUTELY NO WARRANTY.");
  puts ("For help type 'h'.");
}

/*
 * This function loads a catalog of books from a file named "FILE_NAME". The function assumes that
 * the file contains comma-separated values for each field of a Book structure, and that the global
 * array "books" is empty when the function is called. The function reads the file line by line,
 * parsing each line into the fields of a Book structure and storing it in the next available index
 * of the "books" array. The function keeps track of the number of books added to the array and
 * returns this value on success, or -1 if an error occurs. If the file does not exist or cannot be
 * opened for reading, the function outputs an error message and returns -1. If an error occurs
 * while reading the file, the function outputs an error message, closes the file, and returns -1.
 */
static int
load_catalog (void)
{
  FILE *fp;
  char line[MAX_LINE_LENGTH];
  char *field;

  fp = fopen (FILE_NAME, "r");
  if (fp == NULL)
    {
      fprintf (stderr, "Error: '%s': %s.\n", FILE_NAME, "when opening file");
      return -1;
    }

  num_books = 0;
  while (fgets (line, MAX_LINE_LENGTH, fp) != NULL)
    {
      field = strtok (line, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].title, field, MAX_FIELD_LENGTH - 1);
          books[num_books].title[MAX_FIELD_LENGTH - 1] = '\0';
        }

      field = strtok (NULL, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].author, field, MAX_FIELD_LENGTH - 1);
          books[num_books].author[MAX_FIELD_LENGTH - 1] = '\0';
        }

      field = strtok (NULL, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].publisher, field, MAX_FIELD_LENGTH - 1);
          books[num_books].publisher[MAX_FIELD_LENGTH - 1] = '\0';
        }

      field = strtok (NULL, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].publication_year, field, MAX_FIELD_LENGTH - 1);
          books[num_books].publication_year[MAX_FIELD_LENGTH - 1] = '\0';
        }

      field = strtok (NULL, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].isbn, field, MAX_FIELD_LENGTH - 1);
          books[num_books].isbn[MAX_FIELD_LENGTH - 1] = '\0';
        }

      field = strtok (NULL, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].accession_number, field, MAX_FIELD_LENGTH - 1);
          books[num_books].accession_number[MAX_FIELD_LENGTH - 1] = '\0';
        }

      field = strtok (NULL, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].genre, field, MAX_FIELD_LENGTH - 1);
          books[num_books].genre[MAX_FIELD_LENGTH - 1] = '\0';
        }

      field = strtok (NULL, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].checked_out_by, field, MAX_FIELD_LENGTH - 1);
          books[num_books].checked_out_by[MAX_FIELD_LENGTH - 1] = '\0';
        }

      field = strtok (NULL, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].checked_out_date, field, MAX_FIELD_LENGTH - 1);
          books[num_books].checked_out_date[MAX_FIELD_LENGTH - 1] = '\0';
        }

      field = strtok (NULL, ",");
      if (field != NULL)
        {
          field[strcspn (field, "\n")] = '\0';
          strncpy (books[num_books].return_date, field, MAX_FIELD_LENGTH - 1);
          books[num_books].return_date[MAX_FIELD_LENGTH - 1] = '\0';
        }

      num_books++;
    }

  if (ferror (fp))
    {
      fprintf (stderr, "Error: failure reading file.\n");
      fclose (fp);
      return -1;
    }

  fclose (fp);
  return num_books;
}

/*
 * This function verifies the user's password by prompting the user to enter a password and comparing
 * it to a stored password. The function assumes that the maximum length of the entered password is
 * MAX_LINE_LENGTH. If the user's input exceeds this length, the function discards any extra characters.
 * If the user enters a password that matches the stored password, the function returns 1. Otherwise,
 * the function returns 0. If an error occurs while reading the user's input, the function outputs an
 * error message and returns -1.
 */
static int
verify_user (void)
{
  const char stored_pass[] = "bisu";
  char entered_pass[MAX_LINE_LENGTH];

  printf ("Enter password: ");
  if (fgets (entered_pass, MAX_LINE_LENGTH, stdin) == NULL)
    {
      fprintf (stderr, "Error: fgets failed.\n");
      return -1;
    }

  if (strchr (entered_pass, '\n') == NULL)
    while ((d = getchar ()) != '\n' && d != EOF) {}

  entered_pass[strcspn (entered_pass, "\n")] = '\0';

  if (!strcmp (stored_pass, entered_pass))
    return 1;

  return 0;
}

/*
 * This function is the entry point of the program. It allocates memory for the global array "books"
 * of Book structures using malloc(), loads a catalog of books from a file, verifies the user's
 * password, and enters a loop to read user input and perform various operations on the catalog
 * of books. The function returns EXIT_SUCCESS if the program terminates successfully, or
 * EXIT_FAILURE if an error occurs. If an error occurs while reading user input or performing
 * operations on the catalog of books, the function outputs an error message and exits the program.
 */
int
main (void)
{
  char c;
  int ret;

  books = (Book *) malloc (sizeof (Book) * MAX_BOOKS);

  if (books == NULL)
    {
      fprintf (stderr, "Error: %s.\n", "failed to allocate memory from malloc()");
      return EXIT_FAILURE;
    }

  while (1)
    {
      ret = verify_user ();
      if (ret == -1)
        goto err_quit;
      else if (ret == 1)
        break;
      else
        continue;
    }
  system ("clear");
  print_info ();

  num_books = load_catalog ();
  if (num_books == -1)
    goto err_quit;

  while (1)
    {
      printf (">>> ");
      if (scanf (" %c", &c) == EOF)
        {
          fprintf (stderr, "Error: scanf failed.\n");
          goto err_quit;
        }
      while ((d = getchar ()) != '\n' && d != EOF) {}

      switch (c)
        {
        case 'a':
          if (add_book () == -1)
            goto err_quit;
          break;

        case 'b':
          if (borrow_book () == -1)
            goto err_quit;
          break;

        case 'd':
          if (delete_book () == -1)
            goto err_quit;
          break;

        case 'f':
          if (find_book () == -1)
            goto err_quit;
          break;

        case 'h':
          print_help ();
          break;

        case 'l':
          list_books ();
          break;

        case 'q':
          quit_prog ();
          goto safe_quit;

        case 's':
          if (sort_books () == -1)
            goto err_quit;
          break;

        case 'r':
          if (return_book () == -1)
            goto err_quit;
          break;

        case 'w':
          print_warranty ();
          break;

        default:
          fprintf (stderr, "Error: invalid input. Type 'h' for help.\n");
          continue;
        }
    }

safe_quit:
  save_catalog ();

err_quit:
  free (books);
  return EXIT_SUCCESS;
}

