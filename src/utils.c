/* utils.c
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

#include <ctype.h>

/*
 * Function: strcasecmp
 * ---------------------
 * Compare two C-style strings in a case-insensitive manner.
 *
 * This function converts each character of the input strings to lowercase and compares them using the
 * standard subtraction operator. It returns an integer value indicating the relationship between the
 * strings:
 * - 0 if the strings are equal (case-insensitive)
 * - A positive value if the first string is greater than the second string (case-insensitive)
 * - A negative value if the first string is less than the second string (case-insensitive)
 *
 * Parameters:
 *   s1 - A pointer to the first C-style string to compare.
 *   s2 - A pointer to the second C-style string to compare.
 *
 * Returns:
 *   An integer value indicating the relationship between the two strings.
 *
 * Note:
 *   This function treats uppercase and lowercase letters as equal.
 */
int
strcasecmp (const char *s1,
            const char *s2)
{
  int i = 0;

  while (s1[i] && s2[i])
    {
      if (tolower (s1[i]) != tolower (s2[i]))
        return tolower (s1[i]) - tolower (s2[i]);
      i++;
    }

  return tolower (s1[i]) - tolower (s2[i]);
}

