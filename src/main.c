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

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "data/library_catalog.csv"
#define MAX_LINE_LENGTH 1024
#define MAX_FIELD_LENGTH 256
#define MAX_NUM_FIELDS 10
#define MAX_BOOKS 1000

typedef struct {
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

int
write_catalog (Book book)
{
  FILE *fp;

  fp = fopen (FILE_NAME, "a");
  if (fp == NULL) {
    printf ("Error: '%s': %s\n", FILE_NAME, "error opening file");
    return 1;
  }

  fprintf (fp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
          book.title, book.author, book.publisher, book.publication_year,
          book.isbn, book.accession_number, book.genre, book.checked_out_by,
          book.checked_out_date, book.return_date);

  fclose(fp);
  return 0;
}

int
add_book (Book *books, Book book, int num_books)
{
  /* check if the books array is full */
  if (num_books >= MAX_BOOKS) {
    printf("Error: Maximum number of books reached.\n");
    return -1;
  }

  // copy the book information to the next available slot in the books array
  memcpy(&books[num_books], &book, sizeof(Book));
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

  return 0;
}

int
get_book_info (Book *book)
{
  char buffer[MAX_FIELD_LENGTH];

  printf("Enter the book title: ");
  fgets(buffer, MAX_FIELD_LENGTH, stdin);
  buffer[strcspn(buffer, "\n")] = '\0'; // remove the trailing newline
  strncpy(book->title, buffer, MAX_FIELD_LENGTH - 1);
  book->title[MAX_FIELD_LENGTH - 1] = '\0';

  printf("Enter the author name: ");
  fgets(buffer, MAX_FIELD_LENGTH, stdin);
  buffer[strcspn(buffer, "\n")] = '\0';
  strncpy(book->author, buffer, MAX_FIELD_LENGTH - 1);
  book->author[MAX_FIELD_LENGTH - 1] = '\0';

  printf("Enter the publisher name: ");
  fgets(buffer, MAX_FIELD_LENGTH, stdin);
  buffer[strcspn(buffer, "\n")] = '\0';
  strncpy(book->publisher, buffer, MAX_FIELD_LENGTH - 1);
  book->publisher[MAX_FIELD_LENGTH - 1] = '\0';

  printf("Enter the publication year: ");
  fgets(buffer, MAX_FIELD_LENGTH, stdin);
  buffer[strcspn(buffer, "\n")] = '\0';
  strncpy(book->publication_year, buffer, MAX_FIELD_LENGTH - 1);
  book->publication_year[MAX_FIELD_LENGTH - 1] = '\0';

  printf("Enter the ISBN: ");
  fgets(buffer, MAX_FIELD_LENGTH, stdin);
  buffer[strcspn(buffer, "\n")] = '\0';
  strncpy(book->isbn, buffer, MAX_FIELD_LENGTH - 1);
  book->isbn[MAX_FIELD_LENGTH - 1] = '\0';

  printf("Enter the accession number: ");
  fgets(buffer, MAX_FIELD_LENGTH, stdin);
  buffer[strcspn(buffer, "\n")] = '\0';
  strncpy(book->accession_number, buffer, MAX_FIELD_LENGTH - 1);
  book->accession_number[MAX_FIELD_LENGTH - 1] = '\0';

  printf("Enter the genre: ");
  fgets(buffer, MAX_FIELD_LENGTH, stdin);
  buffer[strcspn(buffer, "\n")] = '\0';
  strncpy(book->genre, buffer, MAX_FIELD_LENGTH - 1);
  book->genre[MAX_FIELD_LENGTH - 1] = '\0';

  // initialize the remaining fields to "-" strings
  strncpy(book->checked_out_by, "-", MAX_FIELD_LENGTH - 1);
  book->checked_out_by[MAX_FIELD_LENGTH - 1] = '\0';

  strncpy(book->checked_out_date, "-", MAX_FIELD_LENGTH - 1);
  book->checked_out_date[MAX_FIELD_LENGTH - 1] = '\0';

  strncpy(book->return_date, "-", MAX_FIELD_LENGTH - 1);
  book->return_date[MAX_FIELD_LENGTH - 1] = '\0';

  return 0;
}

void
print_info (const Book book)
{
  printf("Title:            %s\n", book.title);
  printf("Author:           %s\n", book.author);
  printf("Publisher:        %s\n", book.publisher);
  printf("Publication Year: %s\n", book.publication_year);
  printf("ISBN:             %s\n", book.isbn);
  printf("Accession Number: %s\n", book.accession_number);
  printf("Genre:            %s\n", book.genre);
  printf("Checked Out By:   %s\n", book.checked_out_by);
  printf("Checked Out Date: %s\n", book.checked_out_date);
  printf("Return Date:      %s\n", book.return_date);
}

int
load_catalog (Book *books)
{
  FILE *fp;
  char line[MAX_LINE_LENGTH];
  char *field;
  int num_books;

  fp = fopen (FILE_NAME, "r");
  if (fp == NULL) {
    fprintf (stderr, "Error: '%s': %s.\n", FILE_NAME, "opening file");
    return 1;
  }

  num_books = 0;
  while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
    field = strtok(line, ",");
    if (field != NULL) {
      strncpy(books[num_books].title, field, MAX_FIELD_LENGTH - 1);
      books[num_books].title[MAX_FIELD_LENGTH - 1] = '\0';
    }

    field = strtok(NULL, ",");
    if (field != NULL) {
      strncpy(books[num_books].author, field, MAX_FIELD_LENGTH - 1);
      books[num_books].author[MAX_FIELD_LENGTH - 1] = '\0';
    }

    field = strtok(NULL, ",");
    if (field != NULL) {
      strncpy(books[num_books].publisher, field, MAX_FIELD_LENGTH - 1);
      books[num_books].publisher[MAX_FIELD_LENGTH - 1] = '\0';
    }

    field = strtok(NULL, ",");
    if (field != NULL) {
      strncpy(books[num_books].publication_year, field, MAX_FIELD_LENGTH - 1);
      books[num_books].publication_year[MAX_FIELD_LENGTH - 1] = '\0';
    }

    field = strtok(NULL, ",");
    if (field != NULL) {
      strncpy(books[num_books].isbn, field, MAX_FIELD_LENGTH - 1);
      books[num_books].isbn[MAX_FIELD_LENGTH - 1] = '\0';
    }

    field = strtok(NULL, ",");
    if (field != NULL) {
      strncpy(books[num_books].accession_number, field, MAX_FIELD_LENGTH - 1);
      books[num_books].accession_number[MAX_FIELD_LENGTH - 1] = '\0';
    }

    field = strtok(NULL, ",");
    if (field != NULL) {
      strncpy(books[num_books].genre, field, MAX_FIELD_LENGTH - 1);
      books[num_books].genre[MAX_FIELD_LENGTH - 1] = '\0';
    }

    field = strtok(NULL, ",");
    if (field != NULL) {
      strncpy(books[num_books].checked_out_by, field, MAX_FIELD_LENGTH - 1);
      books[num_books].checked_out_by[MAX_FIELD_LENGTH - 1] = '\0';
    }

    field = strtok(NULL, ",");
    if (field != NULL) {
      strncpy(books[num_books].checked_out_date, field, MAX_FIELD_LENGTH - 1);
      books[num_books].checked_out_date[MAX_FIELD_LENGTH - 1] = '\0';
    }

    field = strtok(NULL, ",");
    if (field != NULL) {
      strncpy(books[num_books].return_date, field, MAX_FIELD_LENGTH - 1);
      books[num_books].return_date[MAX_FIELD_LENGTH - 1] = '\0';
    }

    num_books++;
  }

  if (ferror (fp)) {
    fprintf (stderr, "Error: '%s': %s.\n", FILE_NAME, "reading file");
    fclose (fp);
    return 1;
  }

  fclose(fp);
  return num_books;
}

void
prog_ver (void)
{
  puts ("librlog 0.1-a");
  puts ("Copyright 2023 Francis John Baldon");
  puts ("This is free software with ABSOLUTELY NO WARRANTY.");
  puts ("For help type 'h'.");
}

int
run_program (void)
{
  Book *books, book;
  char c;
  int i, num_books;

  books = (Book *) malloc (sizeof (Book) * MAX_BOOKS);
  if (books == NULL) {
    fprintf (stderr, "Error: %s.\n", "failed to allocate memory from malloc()");
    return 1;
  }

  prog_ver ();
  num_books = load_catalog (books);
  while (1) {

    printf (">>> ");
    if (scanf (" %c", &c) != 1) {
      fprintf (stderr, "Error: %s.\n", "failed to read character from stdin");
      return 1;
    }
    while (getchar () != '\n');

    switch (c)
      {
      case 'a':
        get_book_info (&book);
        add_book (books, book, num_books);
        break;
      case 'b':
        puts ("Feature not yet implemented.");
        break;
      case 'c':
        num_books = load_catalog (books);
        puts ("Library catalog loaded successfully.");
        break;
      case 'f':
        puts ("Feature not yet implemented.");
        break;
      case 'h':
        puts ("Type [?]: [a]dd_book, [b]orrow_book, [f]ind_book, load_[c]atalog.");
        puts ("          [l]ist_books, [p]rint_info, [v]erify_user");
        puts ("          [h]elp, [q]uit [w]rite_catalog.");
        break;
      case 'l':
        for (i = 1; i < num_books; ++i)
          print_info (books[i]);
        break;
      case 'p':
        print_info (book);
        puts ("Feature not yet implemented.");
        break;
      case 'q':
        puts ("Exited.");
        return -1;
      case 'r':
        puts ("Feature not yet implemented.");
        break;
      case 'v':
        puts ("Feature not yet implemented.");
        break;
      case 'w':
        write_catalog (book);
        break;
      default:
        fprintf (stderr, "Error: '%c': %s\n", c, "invalid input. Type 'h' for help.");
        continue;
      }
  }

  free (books);
  return 0;
}

int
main (int argc,
      char *argv[])
{
  run_program ();
  return EXIT_SUCCESS;
}
