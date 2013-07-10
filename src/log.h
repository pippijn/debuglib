/* Copyright © 2010 Pippijn van Steenhoven
 * See COPYING.AGPL for licence information.
 */
#pragma once

template<size_t Base>
static inline unsigned char
log (size_t x)
{
  unsigned char i = 0;
  while (x /= Base)
    i++;
  return i;
}

template<size_t Base, size_t N>
struct log_t
{
  static unsigned char const value = 1 + log_t<Base, N / Base>::value;
};

template<size_t Base>
struct log_t<Base, 0>
{
  static unsigned char const value = 0;
};
