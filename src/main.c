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
#define PROG_VERSION "librlog 0.1-a"
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
find_book (Book* books, int num_books, const char title[])
{
  int i;

  for (i = 0; i < num_books; i++)
    if (strcmp (books[i].title, title) == 0)
      return i; // return index of matching book

  return 0; // book not found
}

int
update_catalog (Book book)
{
  FILE *fp;

  fp = fopen (FILE_NAME, "a");
  if (fp == NULL) {
    fprintf (stderr, "Error: '%s': %s\n", FILE_NAME, "error opening file");
    fclose (fp);
    return -1;
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
    fprintf (stderr, "Error: %s.\n", "maximum number of books reached.\n");
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
  int d;
  char buffer[MAX_FIELD_LENGTH];

  printf ("Enter the book title:      ");
  fgets (buffer, MAX_FIELD_LENGTH, stdin);
  if (strlen (buffer) == MAX_FIELD_LENGTH - 1 && buffer[MAX_FIELD_LENGTH - 2] != '\n')
    while ((d = getchar ()) != '\n' && d != EOF);
  buffer[strcspn (buffer, "\n")] = '\0'; // remove the trailing newline
  strncpy (book->title, buffer, MAX_FIELD_LENGTH - 1);
  book->title[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Enter the author name:      ");
  fgets (buffer, MAX_FIELD_LENGTH, stdin);
  if (strlen (buffer) == MAX_FIELD_LENGTH - 1 && buffer[MAX_FIELD_LENGTH - 2] != '\n')
    while ((d = getchar ()) != '\n' && d != EOF);
  while ((d = getchar ()) != '\n' && d != EOF);
  buffer[strcspn (buffer, "\n")] = '\0';
  strncpy (book->author, buffer, MAX_FIELD_LENGTH - 1);
  book->author[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Enter the publisher name:   ");
  fgets (buffer, MAX_FIELD_LENGTH, stdin);
  if (strlen (buffer) == MAX_FIELD_LENGTH - 1 && buffer[MAX_FIELD_LENGTH - 2] != '\n')
    while ((d = getchar ()) != '\n' && d != EOF);
  while ((d = getchar ()) != '\n' && d != EOF);
  buffer[strcspn (buffer, "\n")] = '\0';
  strncpy (book->publisher, buffer, MAX_FIELD_LENGTH - 1);
  book->publisher[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Enter the publication year: ");
  fgets (buffer, MAX_FIELD_LENGTH, stdin);
  if (strlen (buffer) == MAX_FIELD_LENGTH - 1 && buffer[MAX_FIELD_LENGTH - 2] != '\n')
    while ((d = getchar ()) != '\n' && d != EOF);
  while ((d = getchar ()) != '\n' && d != EOF);
  buffer[strcspn (buffer, "\n")] = '\0';
  strncpy (book->publication_year, buffer, MAX_FIELD_LENGTH - 1);
  book->publication_year[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Enter the ISBN:             ");
  fgets (buffer, MAX_FIELD_LENGTH, stdin);
  if (strlen (buffer) == MAX_FIELD_LENGTH - 1 && buffer[MAX_FIELD_LENGTH - 2] != '\n')
    while ((d = getchar ()) != '\n' && d != EOF);
  while ((d = getchar ()) != '\n' && d != EOF);
  buffer[strcspn (buffer, "\n")] = '\0';
  strncpy (book->isbn, buffer, MAX_FIELD_LENGTH - 1);
  book->isbn[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Enter the accession number: ");
  fgets (buffer, MAX_FIELD_LENGTH, stdin);
  if (strlen (buffer) == MAX_FIELD_LENGTH - 1 && buffer[MAX_FIELD_LENGTH - 2] != '\n')
    while ((d = getchar ()) != '\n' && d != EOF);
  while ((d = getchar ()) != '\n' && d != EOF);
  buffer[strcspn (buffer, "\n")] = '\0';
  strncpy (book->accession_number, buffer, MAX_FIELD_LENGTH - 1);
  book->accession_number[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Enter the genre:            ");
  fgets (buffer, MAX_FIELD_LENGTH, stdin);
  if (strlen (buffer) == MAX_FIELD_LENGTH - 1 && buffer[MAX_FIELD_LENGTH - 2] != '\n')
    while ((d = getchar ()) != '\n' && d != EOF);
  buffer[strcspn (buffer, "\n")] = '\0';
  strncpy (book->genre, buffer, MAX_FIELD_LENGTH - 1);
  book->genre[MAX_FIELD_LENGTH - 1] = '\0';

  // initialize the remaining fields to "-" strings
  strncpy (book->checked_out_by, "-", MAX_FIELD_LENGTH - 1);
  book->checked_out_by[MAX_FIELD_LENGTH - 1] = '\0';

  strncpy (book->checked_out_date, "-", MAX_FIELD_LENGTH - 1);
  book->checked_out_date[MAX_FIELD_LENGTH - 1] = '\0';

  strncpy (book->return_date, "-", MAX_FIELD_LENGTH - 1);
  book->return_date[MAX_FIELD_LENGTH - 1] = '\0';

  return 0;
}

void
print_book_info (const Book book)
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
    return -1;
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
    return -1;
  }

  fclose (fp);
  return num_books;
}

void
print_prog_warranty (void)
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

void
print_prog_info (void)
{
  puts (PROG_VERSION);
  puts ("Copyright 2023 Francis John Baldon");
  puts ("This is free software with ABSOLUTELY NO WARRANTY.");
  puts ("For help type 'h'.");
}

int
main (int argc,
      char *argv[])
{
  Book *books, book;
  char c, buffer[MAX_FIELD_LENGTH];
  int d, i, ret, num_books;

  books = (Book *) malloc (sizeof (Book) * MAX_BOOKS);
  if (books == NULL) {
    fprintf (stderr, "Error: %s.\n", "failed to allocate memory from malloc()");
    return EXIT_FAILURE;
  }

  print_prog_info ();
  if ((num_books = load_catalog (books)) == -1)
    return EXIT_FAILURE;

  while (1) {
    printf (">>> ");
    if (scanf (" %c", &c) == EOF)
      goto quit;
    while ( (d = getchar ()) != '\n' && d != EOF);

    switch (c)
      {
      case 'a':
        get_book_info (&book);
        add_book (books, book, num_books);
        break;
      case 'b':
        /* TODO */
        break;
      case 'c':
        num_books = load_catalog (books);
        puts ("Library catalog loaded successfully.");
        break;
      case 'f':
        printf ("Enter the book title:      ");
        fgets (buffer, MAX_FIELD_LENGTH, stdin);
        if (strlen (buffer) == MAX_FIELD_LENGTH - 1 && buffer[MAX_FIELD_LENGTH - 2] != '\n')
          while ((d = getchar ()) != '\n' && d != EOF);
        buffer[strcspn (buffer, "\n")] = '\0';
        printf("buffer: %s\n", buffer);
        if ((ret = find_book (books, num_books, buffer)) != 0)
          print_book_info (books[ret]);
        else
          puts ("Book not found.");
        printf ("ret: %d\n", ret);
        break;
      case 'h':
        puts ("Type [?]: [a]dd_book, [b]orrow_book, [f]ind_book, [p]rint_book_info");
        puts ("          [l]ist_books, load_[c]atalog, [v]erify_user, [u]pdate_catalog");
        puts ("          [h]elp, [q]uit, [w]arranty");
        break;
      case 'l':
        for (i = 1; i < num_books; i++)
          print_book_info (books[i]);
        break;
      case 'p':
        print_book_info (book);
        break;
      case 'q':
        goto quit;
      case 'r':
        /* TODO */
      case 'u':
        update_catalog (book);
        break;
      case 'v':
        /* TODO */
        break;
      case 'w':
        print_prog_warranty ();
        break;
      default:
        fprintf (stderr, "Error: '%c': %s\n", c, "invalid input. Type 'h' for help.");
        continue;
      }
  }

quit:
  puts ("Exited.");
  free (books);
  return EXIT_SUCCESS;
}

