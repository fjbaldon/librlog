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

/* macros */
#define FILE_NAME "data/library_catalog.csv"
#define PROG_VERSION "librlog 0.1-a"
#define MAX_LINE_LENGTH 2560
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

/* enums */
enum {
  TITLE,
  AUTHOR,
  GENRE,
  PUBLISHER,
  PUBLICATION_YEAR,
};

/* function declarations */
int save_catalog (void);
int find_book (int field, const char *strtofind, int starting_i);
int add_book_books (Book book);
int get_str_cleanly (char *dest, size_t size);
int get_book_info (Book *book);
void print_book_info (const Book book);
int load_catalog (void);
void print_prog_warranty (void);
void print_prog_info (void);

/* variables */
static Book *books;
static int num_books;

/* function implementations */
int
save_catalog (void)
{
  FILE *fp;
  int i;

  fp = fopen (FILE_NAME, "w");
  if (fp == NULL) {
    fprintf (stderr, "Error: '%s': %s.\n", FILE_NAME, "could not open file for writing");
    return -1;
  }

  /* Write header row */
  fprintf(fp, "Title,Author,Publisher,Publication Year,ISBN,Accession Number,Genre,Checked Out By,Checked Out Date,Return Date\n");

  /* Write book data */
  for (i = 1; i < num_books; i++) {
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

int
find_book (int field,
           const char *strtofind,
           int starting_i)
{
  int i;

  if (field < TITLE && field > PUBLICATION_YEAR) {
    fprintf (stderr, "Error: %s.\n", "invalid field passed");
    return -1;
  }

  for (i = starting_i; i < num_books; i++) {
    switch (field)
      {
      case TITLE:
        if (strcmp (books[i].title, strtofind) == 0)
          /* return index of matching book */
          return i;
        break;
      case AUTHOR:
        if (strcmp (books[i].author, strtofind) == 0)
          return i;
        break;
      case GENRE:
        if (strcmp (books[i].genre, strtofind) == 0)
          return i;
        break;
      case PUBLISHER:
        if (strcmp (books[i].publisher, strtofind) == 0)
          return i;
        break;
      case PUBLICATION_YEAR:
        if (strcmp (books[i].publication_year, strtofind) == 0)
          return i;
        break;
      default:
        fprintf (stderr, "Error: %s.\n", "this case should never execute");
        return -1;
      }
  }

  /* book not found */
  return 0;
}

int
add_book_books (Book book)
{
  /* check if the books array is full */
  if (num_books >= MAX_BOOKS) {
    fprintf (stderr, "Error: %s.\n", "maximum number of books reached");
    return -1;
  }

  // copy the book information to the next available slot in the books array
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

  return 0;
}

int
get_str_cleanly (char *dest, size_t size)
{
  int d;

  if (!fgets (dest, size, stdin)) {
    puts ("EOF reached.");
    return -1;
  }

  /* discard unread characters if there's any */
  if (strlen (dest) == size - 1 && dest[size - 2] != '\n')
    while ((d = getchar ()) != '\n' && d != EOF);

  /* remove trailing newline */
  dest[strcspn(dest, "\n")] = '\0';

  return 0;
}

int
get_book_info (Book *book)
{
  char buffer[MAX_FIELD_LENGTH];

  printf ("Enter the book title:       ");
  if (get_str_cleanly (buffer, sizeof (buffer)) == -1)
    return -1;
  strncpy (book->title, buffer, MAX_FIELD_LENGTH - 1);
  book->title[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Enter the author name:      ");
  if (get_str_cleanly (buffer, sizeof (buffer)) == -1)
    return -1;
  strncpy (book->author, buffer, MAX_FIELD_LENGTH - 1);
  book->author[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Enter the publisher name:   ");
  if (get_str_cleanly (buffer, sizeof (buffer)) == -1)
    return -1;
  strncpy (book->publisher, buffer, MAX_FIELD_LENGTH - 1);
  book->publisher[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Enter the publication year: ");
  if (get_str_cleanly (buffer, sizeof (buffer)) == -1)
    return -1;
  strncpy (book->publication_year, buffer, MAX_FIELD_LENGTH - 1);
  book->publication_year[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Enter the ISBN:             ");
  if (get_str_cleanly (buffer, sizeof (buffer)) == -1)
    return -1;
  strncpy (book->isbn, buffer, MAX_FIELD_LENGTH - 1);
  book->isbn[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Enter the accession number: ");
  if (get_str_cleanly (buffer, sizeof (buffer)) == -1)
    return -1;
  strncpy (book->accession_number, buffer, MAX_FIELD_LENGTH - 1);
  book->accession_number[MAX_FIELD_LENGTH - 1] = '\0';

  printf ("Enter the genre:            ");
  if (get_str_cleanly (buffer, sizeof (buffer)) == -1)
    return -1;
  strncpy (book->genre, buffer, MAX_FIELD_LENGTH - 1);
  book->genre[MAX_FIELD_LENGTH - 1] = '\0';

  /* initialize the remaining fields to "-" strings */
  strncpy (book->checked_out_by, "", MAX_FIELD_LENGTH - 1);
  book->checked_out_by[MAX_FIELD_LENGTH - 1] = '\0';

  strncpy (book->checked_out_date, "", MAX_FIELD_LENGTH - 1);
  book->checked_out_date[MAX_FIELD_LENGTH - 1] = '\0';

  strncpy (book->return_date, "", MAX_FIELD_LENGTH - 1);
  book->return_date[MAX_FIELD_LENGTH - 1] = '\0';

  return 0;
}

void
print_book_info (const Book book)
{
  printf ("Title:            %s\n", book.title);
  printf ("Author:           %s\n", book.author);
  printf ("Publisher:        %s\n", book.publisher);
  printf ("Publication Year: %s\n", book.publication_year);
  printf ("ISBN:             %s\n", book.isbn);
  printf ("Accession Number: %s\n", book.accession_number);
  printf ("Genre:            %s\n", book.genre);
  printf ("Checked Out By:   %s\n", book.checked_out_by);
  printf ("Checked Out Date: %s\n", book.checked_out_date);
  printf ("Return Date:      %s\n\n", book.return_date);
}

int
load_catalog (void)
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
  while (fgets (line, MAX_LINE_LENGTH, fp) != NULL) {
    field = strtok (line, ",");
    if (field != NULL) {
      field[strcspn (field, "\n")] = '\0';
      strncpy (books[num_books].title, field, MAX_FIELD_LENGTH - 1);
      books[num_books].title[MAX_FIELD_LENGTH - 1] = '\0';
    }

    field = strtok (NULL, ",");
    if (field != NULL) {
      field[strcspn (field, "\n")] = '\0';
      strncpy (books[num_books].author, field, MAX_FIELD_LENGTH - 1);
      books[num_books].author[MAX_FIELD_LENGTH - 1] = '\0';
    }

    field = strtok (NULL, ",");
    if (field != NULL) {
      field[strcspn (field, "\n")] = '\0';
      strncpy (books[num_books].publisher, field, MAX_FIELD_LENGTH - 1);
      books[num_books].publisher[MAX_FIELD_LENGTH - 1] = '\0';
    }

    field = strtok (NULL, ",");
    if (field != NULL) {
      field[strcspn (field, "\n")] = '\0';
      strncpy (books[num_books].publication_year, field, MAX_FIELD_LENGTH - 1);
      books[num_books].publication_year[MAX_FIELD_LENGTH - 1] = '\0';
    }

    field = strtok (NULL, ",");
    if (field != NULL) {
      field[strcspn (field, "\n")] = '\0';
      strncpy (books[num_books].isbn, field, MAX_FIELD_LENGTH - 1);
      books[num_books].isbn[MAX_FIELD_LENGTH - 1] = '\0';
    }

    field = strtok (NULL, ",");
    if (field != NULL) {
      field[strcspn (field, "\n")] = '\0';
      strncpy (books[num_books].accession_number, field, MAX_FIELD_LENGTH - 1);
      books[num_books].accession_number[MAX_FIELD_LENGTH - 1] = '\0';
    }

    field = strtok (NULL, ",");
    if (field != NULL) {
      field[strcspn (field, "\n")] = '\0';
      strncpy (books[num_books].genre, field, MAX_FIELD_LENGTH - 1);
      books[num_books].genre[MAX_FIELD_LENGTH - 1] = '\0';
    }

    field = strtok (NULL, ",");
    if (field != NULL) {
      field[strcspn (field, "\n")] = '\0';
      strncpy (books[num_books].checked_out_by, field, MAX_FIELD_LENGTH - 1);
      books[num_books].checked_out_by[MAX_FIELD_LENGTH - 1] = '\0';
    }

    field = strtok (NULL, ",");
    if (field != NULL) {
      field[strcspn (field, "\n")] = '\0';
      strncpy (books[num_books].checked_out_date, field, MAX_FIELD_LENGTH - 1);
      books[num_books].checked_out_date[MAX_FIELD_LENGTH - 1] = '\0';
    }

    field = strtok (NULL, ",");
    if (field != NULL) {
      field[strcspn (field, "\n")] = '\0';
      strncpy (books[num_books].return_date, field, MAX_FIELD_LENGTH - 1);
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
  Book book;
  char c, buffer[MAX_FIELD_LENGTH];
  int d, i, ret;

  books = (Book *) malloc (sizeof (Book) * MAX_BOOKS);
  if (books == NULL) {
    fprintf (stderr, "Error: %s.\n", "failed to allocate memory from malloc()");
    return EXIT_FAILURE;
  }

  print_prog_info ();
  if ((num_books = load_catalog ()) == -1)
    goto quit;

menu:
  while (1) {

    printf (">>> ");
    if (scanf (" %c", &c) == EOF) {
      puts ("EOF reached.");
      goto quit;
    }
    while ((d = getchar ()) != '\n' && d != EOF);

    switch (c)
      {
      case 'a':
        if (get_book_info (&book) == -1)
          goto quit;
        if (add_book_books (book) == -1)
          continue;
        num_books++;
        break;
      case 'b':
        /* TODO */
        break;
      case 'f':
        printf ("Enter the book title:      ");
        get_str_cleanly (buffer, sizeof (buffer));
        if ((ret = find_book (TITLE, buffer, 1)) > 0)
          print_book_info (books[ret]);
        else
          puts ("Book not found.");
        break;
      case 'h':
        puts ("Type [?]: [a]dd_book, [b]orrow_book, [f]ind_book");
        puts ("          [l]ist_books, [s]ort_books, [v]erify_user");
        puts ("          [h]elp, [q]uit, [w]arranty");
        break;
      case 'l':
        for (i = 1; i < num_books; i++)
          print_book_info (books[i]);
        break;
      case 'q':
        puts ("Exited.");
        goto quit;
      case 's':
        while (1) {

          printf ("Type [?]: [a]uthor, [g]enre, [p]ublisher, publication_[y]ear, [b]ack.\n");
          printf (">>> ");
          if (scanf (" %c", &c) == EOF) {
            puts ("EOF reached.");
            goto quit;
          }
          while ((d = getchar ()) != '\n' && d != EOF);

          ret = 0;
          switch (c)
            {
            case 'a':
              printf ("Enter book author: ");
              get_str_cleanly (buffer, sizeof (buffer));
              while ((ret = find_book (GENRE, buffer, ++ret)) > 0)
                print_book_info (books[ret]);
              break;
            case 'b':
              goto menu;
              break;
            case 'g':
              printf ("Enter book genre: ");
              get_str_cleanly (buffer, sizeof (buffer));
              while ((ret = find_book (GENRE, buffer, ++ret)) > 0)
                print_book_info (books[ret]);
              break;
            case 'p':
              printf ("Enter book publisher: ");
              get_str_cleanly (buffer, sizeof (buffer));
              while ((ret = find_book (GENRE, buffer, ++ret)) > 0)
                print_book_info (books[ret]);
              break;
            case 'y':
              printf ("Enter publication year: ");
              get_str_cleanly (buffer, sizeof (buffer));
              while ((ret = find_book (GENRE, buffer, ++ret)) > 0)
                print_book_info (books[ret]);
              break;
            default:
              fprintf (stderr, "Error: '%c': %s\n", c, "invalid input");
              continue;
            }
          }
        break;
      case 'r':
        /* TODO */
        break;
      case 'v':
        /* TODO */
        break;
      case 'w':
        print_prog_warranty ();
        break;
      default:
        fprintf (stderr, "Error: '%c': %s\n", c, "invalid input. Type 'h' for help");
        continue;
      }
  }

quit:
  save_catalog ();
  free (books);
  return EXIT_SUCCESS;
}

