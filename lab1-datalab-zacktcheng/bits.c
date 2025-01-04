/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically and a negative shift value results in zero.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES, FOLLOW THE DIRECTIONS in README.md
 */


#endif
/* Copyright (C) 1991-2022 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
/* 
 * bitOr - x|y using only ~ and & 
 *   Example: bitOr(6, 5) = 7
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitOr(int x, int y) {
  /*
   * De Morgan's law: x | y == ~(~x & ~y).
   */
  return ~(~x & ~y);
}
/*
 * isZero - returns 1 if x == 0, and 0 otherwise 
 *   Examples: isZero(5) = 0, isZero(0) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int isZero(int x) {
  /*
   * Simply apply the logical not ! to convert x.
   * ! a zero will become 1 because 0 is false, doulbe negative is true. 
   * ! a non-zero will become 0 because non-zero is true, falsing a truthy value returns a false value.
   */
  return !x;
}
/* 
 * TMax - return maximum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmax(void) {
  /*
   * For a two's complement binary of w bits, the TMax is (2^(w-1))-1, the binary representation 
   * is a 0 followed by (w - 1) 1s.
   * For example, let w = 8, the TMax is 2^7 - 1 = 127, the binary representation is 0111 1111.
   * 0111 1111 ...1 is the bitwise complemented 1000 0000 ...0, therefore, we can
   * make 1000 0000 ...0 by left shift 31 bits for 0x01, then complement the left-shifted binary to get TMax.
   */
  return ~(1 << 31);
}
/* 
 * anyOddBit - return 1 if any odd-numbered bit in word set to 1
 *   Examples anyOddBit(0x5) = 0, anyOddBit(0x7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyOddBit(int x) {
  /*
   * First, create a 32-bit mask with all odd-numbered bits set to 1, and all even-numbered bits set to 0.
   * Second, use & operator to extract any 1 at any odd-numbered index.
   * If none of odd-numbered 1s are in the binary, the bitwise operation result shall be zero, non-zero otherwise.
   * Finally, use ! twice to convert the non-zero binary to 1.
   */
  int mask = 0xAA + (0xAA << 8) + (0xAA << 16) + (0xAA << 24);
  return !!(x & mask);
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  /*
   * For a two's complement binary of n bits, if it's positive, it only takes [n-2th, ..., 0-th] 
   * bit to render the positive value,
   * that is, bits are 0 from the leftmost till the n-1th bit, 1 otherwise.
   * When x is greater than n's range, it uses the n-1th bit.
   * Perform a left shift then right shift to fill the leading bits with the n-1th,
   * the leading bits will be changed when there was a overflow.
   * Then use ^ to check if the original x and the shifted x are the same.
   * Finally, use ! to convert the xor-tested result, the non-changed result is set to 1, 0 otherwise.
   */
  int shift_pos = 32 + (~n + 1);
  int x_leading_bit_checked = (x << shift_pos) >> shift_pos;
  return !(x ^ x_leading_bit_checked);
}
/* 
 * leastBitPos - return a mask that marks the position of the
 *               least significant 1 bit. If x == 0, return 0
 *   Example: leastBitPos(96) = 0x20
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2 
 */
int leastBitPos(int x) {
  /*
   * We can utilize the fact that -x (~x + 1) has the least sig 1 at the same index as the least sig 1 of x.
   * The rest bit values right to the least sig 1 bit of -x are inverted.
   * Then use & to extract the single bit to create the mask.
   */
  return x & (~x + 1);
}
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  /*
   * Create an upperbound to overflow x to negative when x is greater than 0x39.
   * Create a lowerbound to overflow x to negative when x is smaller than 0x30.
   * Add UpperBound and LowerBound to x respectively，then right shift 31 bit,
   * then & sign to check if either bound of x is negative.
   */
  int sign = 0x1 << 31;
  int upperBound = ~(sign | 0x39);
  int lowerBound = ~0x30;
  upperBound = sign & (upperBound + x) >> 31;
  lowerBound = sign & (lowerBound + 1 + x) >> 31;
  return !(upperBound | lowerBound);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  /*
   * The comparison starts by comparing their sign. isLessOrEqual returns 0 immediately
   * if x is positive while y is negative, it returns 1 otherwise.
   * If x and y have the same sign, then let z = x − y = x + ( ∼ y + 1 ), check if z ≤ 0.
   * If z == 1, then x < y.
   * if z == 0, then x == y.
   */
  int signX = (x >> 31) & 1;
  int signY = (y >> 31) & 1;
  int z = x + (~y + 1);
  int isLess = !((z & (1 << 31)) ^ (1 << 31)) | !(z ^ 0);
  return (!(~signX & signY)) & ((signX & ~signY) | isLess);
}
/* 
 * reverseBytes - reverse the bytes of x
 *   Example: reverseBytes(0x01020304) = 0x04030201
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 25
 *   Rating: 3
 */
int reverseBytes(int x) {
  /*
   * Since the w of x is fixed (32), we can simply rearrange each byte to get the reversed x.
   * For example, x can be expressed as byte3 byte2 byte1 byte0,
   * the reversed x shall be of byte0 byte1 byte2 byte3.
   * First, create a byte mask of all 1s, then use >> and & to extract the byte.
   * Second, use << to shift multiples of 8 bits to rearrange the bytes.
   * Finally, use | to aggregate the bytes, note that every (0 | 1) or (1 | 0) will result in 1.
   */
  int mask = 0xFF;
  int byte0 = x & mask;
  int byte1 = (x >> 8) & mask;
  int byte2 = (x >> 16) & mask;
  int byte3 = (x >> 24) & mask;
  return (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  /*
   * Create a 32-bit mask which contains 1 at only index 0 in every byte to extract every bit in x for further calculation.
   * Sum the bit count for 8 times. In each time, calculate the sum at a unique index in every byte at once. After the iteration, store the sum.
   * Finally, use >> to shift every byte to the rightmost byte position,
   * then use 0xFF and & to extract the bit count to sum the total.
   * Let x = 16843011: 0000 0001 0000 0001 0000 0001 0000 0011
   * sum =             0000 0001 0000 0001 0000 0001 0000 0010
   * return =                  1    +    1    +    1    +    2 = 4
   */
  int onePerByteMask = 1 + (1 << 8) + (1 << 16) + (1 << 24); // 00000001 00000001 00000001 00000001
  int sum = (x & onePerByteMask) + ((x >> 1) & onePerByteMask) + ((x >> 2) & onePerByteMask) + ((x >> 3) & onePerByteMask) + ((x >> 4) & onePerByteMask) + ((x >> 5) & onePerByteMask) + ((x >> 6) & onePerByteMask) + ((x >> 7) & onePerByteMask);
  return (sum & 0xFF) + ((sum >> 8) & 0xFF) + ((sum >> 16) & 0xFF) + ((sum >> 24) & 0xFF);
}
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  /*
   * Based on the fact that if x == 0, then x | (~x + 1) == 0, else x | (~x + 1) < 0.
   * Right shift the above result for x in 31 bits can conpress the negative value to -1.
   * Then, use ^ 1 to make the value at the 0-th bit the opposite of the rest.
   * Finally, extract the 0-th bit and throw away the rest bits by performing & 1.
   */
   int zeroOrNeg = x | (~x + 1);
   int zeroOrAllOneBits = zeroOrNeg >> 31;
   int flippedleadingbit = zeroOrAllOneBits ^ 1;
  return flippedleadingbit & 1;
}
/*
 * trueFiveEighths - multiplies by 5/8 rounding toward 0,
 *  avoiding errors due to overflow
 *  Examples: trueFiveEighths(11) = 6
 *            trueFiveEighths(-9) = -5
 *            trueFiveEighths(0x30000000) = 0x1E000000 (no overflow)
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 4
 */
int trueFiveEighths(int x) {
  /*
   * Start by divide by eight, then get the remainder,
   * then multiply the quotient and remainder by five respectively, and add any new carries.
   * To round negative numbers not divisible by eight towards zero, take advantage of sign extension
   * to get the value 7 for a negative number and 0 for a positive one.
   */
  int eights = x >> 3;
  int rem = x & 7;
  int negMask = x >> 31 & 7;
  return eights + (eights << 2) + ((rem + (rem << 2) + negMask) >> 3);
}
/*
 * Extra credit
 */
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
  /*
   * If the exp bits are all 1s and the mantissa bits has some 1, then uf is NaN, simply return uf.
   * Else xor sign bit with 1 to return.
   */
  int expMask = ~(1 << 31);
  int expMax = 0xFF << 23;
  if ((uf & expMask) > expMax) return uf;
  return uf ^ (1 << 31);
}
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
  /*
   * If the exp bits are all 1s, then uf is NaN, return uf right away.
   * If the exp bits are all 0s, then uf is denormalized, shift one leading bit for the mantissa to return.
   * If the exp byte is not 11111110, then uf is normalized, uf will become infinite, so set the mantissa all 0s.
   * Finally, return the newly created floating point binary with the exp incremented by 1.
   */
  int expMask = 1 << 31;
  int expMax = 0xFF << 23;
  unsigned s = uf & expMask;
  unsigned exp = uf & expMax;
  unsigned frac = uf & ((~(1 << 31)) >> 8);
  if (exp == expMax) return uf;
  if (exp == 0) return s | (frac << 1);
  if (exp == (0x7F << 24)) frac = 0;
  return s | (exp + (1 << 23)) | frac;
}
