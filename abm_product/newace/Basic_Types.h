// -*- C++ -*-

//=============================================================================
/**
 *  @file    Basic_Types.h
 *
 *  Basic_Types.h,v 4.111 2003/11/06 18:19:38 dhinton Exp
 *
 *  @author David L. Levine
 *
 *  #defines the list of preprocessor macros below.  The config.h file can
 *  pre-define any of these to short-cut the definitions.  This is usually
 *  only necessary if the preprocessor does all of its math using integers.
 *
 *  Sizes of built-in types:
 *    - ACE_SIZEOF_CHAR
 *    - ACE_SIZEOF_WCHAR
 *    - ACE_SIZEOF_SHORT
 *    - ACE_SIZEOF_INT
 *    - ACE_SIZEOF_LONG
 *    - ACE_SIZEOF_LONG_LONG
 *    - ACE_SIZEOF_VOID_P
 *    - ACE_SIZEOF_FLOAT
 *    - ACE_SIZEOF_DOUBLE
 *    - ACE_SIZEOF_LONG_DOUBLE
 *
 *  Wrappers for built-in types of specific sizes:
 *    - ACE_USHORT16 (For backward compatibility.  Use ACE_UINT16 instead.)
 *    - ACE_INT16
 *    - ACE_UINT16
 *    - ACE_INT32
 *    - ACE_UINT32
 *    - ACE_UINT64
 *  (Note: ACE_INT64 is not defined, because there is no ACE_LongLong for
 *   platforms that don't have a native 8-byte integer type.)
 *
 *  Byte-order (endian-ness) determination:
 *    ACE_BYTE_ORDER, to either ACE_BIG_ENDIAN or ACE_LITTLE_ENDIAN
 *
 *
 */
//=============================================================================

#include "config.h"

#ifndef ACE_BASIC_TYPES_H
# define ACE_BASIC_TYPES_H

// Pull in definitions
# include <limits.h>   // Integer limits
# include <stdlib.h>   // Other types
# include <stddef.h>   // Get ptrdiff_t - see further comments below


# include <sys/types.h>


// A char always has 1 byte, by definition.
# define ACE_SIZEOF_CHAR 1

// Unfortunately, there isn't a portable way to determine the size of a wchar.
// So we just define them on a platform basis.
# if defined (ACE_HAS_WCHAR)
#   if !defined (ACE_SIZEOF_WCHAR)
#     if defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)
#       define ACE_SIZEOF_WCHAR sizeof (wchar_t)
#     else
// 0 so the Basic_Types test will catch this.
#       define ACE_SIZEOF_WCHAR 0
#     endif /* ACE_HAS_XPG4_MULTIBYTE_CHAR */
#   endif /* !ACE_SIZEOF_WCHAR */
# endif /* ACE_HAS_WCHAR */

// The number of bytes in a short.
# if !defined (ACE_SIZEOF_SHORT)
#   if (USHRT_MAX) == 255U
#     define ACE_SIZEOF_SHORT 1
#   elif (USHRT_MAX) == 65535U
#     define ACE_SIZEOF_SHORT 2
#   elif (USHRT_MAX) == 4294967295U
#     define ACE_SIZEOF_SHORT 4
#   elif (USHRT_MAX) == 18446744073709551615U
#     define ACE_SIZEOF_SHORT 8
#   else
#     error: unsupported short size, must be updated for this platform!
#   endif /* USHRT_MAX */
# endif /* !defined (ACE_SIZEOF_SHORT) */

// The number of bytes in an int.
# if !defined (ACE_SIZEOF_INT)
#   if (UINT_MAX) == 65535U
#     define ACE_SIZEOF_INT 2
#   elif (UINT_MAX) == 4294967295U
#     define ACE_SIZEOF_INT 4
#   elif (UINT_MAX) == 18446744073709551615U
#     define ACE_SIZEOF_INT 8
#   else
#     error: unsupported int size, must be updated for this platform!
#   endif /* UINT_MAX */
# endif /* !defined (ACE_SIZEOF_INT) */

// The number of bytes in a long.
// NOTE - since preprocessors only need to do integer math, this is a likely
// place for a preprocessor to not properly support being able to figure out
// the proper size.  HP aC++ and GNU gcc have this difficulty so watch out.
# if !defined (ACE_SIZEOF_LONG)
#   if (ULONG_MAX) == 65535UL
#     define ACE_SIZEOF_LONG 2
#   elif ((ULONG_MAX) == 4294967295UL)
#     define ACE_SIZEOF_LONG 4
#   elif ((ULONG_MAX) == 18446744073709551615UL)
#     define ACE_SIZEOF_LONG 8
#   else
#     error: unsupported long size, must be updated for this platform!
#   endif /* ULONG_MAX */
# endif /* !defined (ACE_SIZEOF_LONG) */

#ifdef OS_LINUX
typedef unsigned long long ACE_UINT64;
typedef long long ACE_INT64;
#else
typedef unsigned long ACE_UINT64;
typedef long ACE_INT64;
#endif
// The number of bytes in a long long.
# if !defined (ACE_SIZEOF_LONG_LONG)
#   if defined (ACE_LACKS_LONGLONG_T)
#     define ACE_SIZEOF_LONG_LONG 8
#   else  /* ! ACE_WIN32 && ! ACE_LACKS_LONGLONG_T */
#     if ACE_SIZEOF_LONG == 8
#       define ACE_SIZEOF_LONG_LONG 8
       typedef unsigned long ACE_UINT64;
       typedef long ACE_INT64;
#     elif defined (ULLONG_MAX) && !defined (__GNUG__)
       // Some compilers use ULLONG_MAX and others, e.g. Irix, use
       // ULONGLONG_MAX.
#       if (ULLONG_MAX) == 18446744073709551615ULL
#         define ACE_SIZEOF_LONG_LONG 8
#       elif (ULLONG_MAX) == 4294967295ULL
#         define ACE_SIZEOF_LONG_LONG 4
#       else
#         error Unsupported long long size needs to be updated for this platform
#       endif
       typedef unsigned long long ACE_UINT64;
#     elif defined (ULONGLONG_MAX) && !defined (__GNUG__)
       // Irix 6.x, for example.
#       if (ULONGLONG_MAX) == 18446744073709551615ULL
#         define ACE_SIZEOF_LONG_LONG 8
#       elif (ULONGLONG_MAX) == 4294967295ULL
#         define ACE_SIZEOF_LONG_LONG 4
#       else
#         error Unsupported long long size needs to be updated for this platform
#       endif
       typedef unsigned long long ACE_UINT64;
#     else
       // ACE_SIZEOF_LONG_LONG is not yet known, but the platform doesn't
       // claim ACE_LACKS_LONGLONG_T, so assume it has 8-byte long longs.
#       define ACE_SIZEOF_LONG_LONG 8
#       if defined (sun) && !defined (ACE_LACKS_U_LONGLONG_T)
         // sun #defines u_longlong_t, maybe other platforms do also.
         // Use it, at least with g++, so that its -pedantic doesn't
         // complain about no ANSI C++ long long.
         typedef u_longlong_t ACE_UINT64;
#       else
         // LynxOS 2.5.0 and Linux don't have u_longlong_t.
         typedef unsigned long long ACE_UINT64;
#       endif /* sun */
#     endif /* ULLONG_MAX && !__GNUG__ */
#   endif /* ! ACE_WIN32 && ! ACE_LACKS_LONGLONG_T */
# endif /* !defined (ACE_SIZEOF_LONG_LONG) */

// The sizes of the commonly implemented types are now known.  Set up
// typedefs for whatever we can.  Some of these are needed for certain cases
// of ACE_UINT64, so do them before the 64-bit stuff.

# if ACE_SIZEOF_SHORT == 2
  typedef short ACE_INT16;
  typedef unsigned short ACE_UINT16;
# elif ACE_SIZEOF_INT == 2
  typedef int ACE_INT16;
  typedef unsigned int ACE_UINT16;
# elif (ACE_SIZEOF_SHORT) == 4 && defined(_CRAYMPP)
  // mpp cray - uses Alpha processors
  //   Use the real 32-bit quantity for ACE_INT32's, and use a "long"
  //   for ACE_INT16's.  This gets around conflicts with size_t in some ACE
  //   method signatures, among other things.
  typedef long ACE_INT16;
  typedef unsigned long ACE_UINT16;
  typedef short ACE_INT32;
  typedef unsigned short ACE_UINT32;
# elif (ACE_SIZEOF_SHORT) == 8 && defined(_UNICOS)
  // vector cray - hard 64-bit, all 64 bit types
  typedef short ACE_INT16;
  typedef unsigned short ACE_UINT16;
# else
#   error Have to add to the ACE_UINT16 type setting
# endif

typedef ACE_UINT16 ACE_USHORT16;  // @@ Backward compatibility.

// Define a generic byte for use in codecs
typedef unsigned char ACE_Byte;

// Define a pseudo wide character type when wchar is not supported so we
// can support basic wide character string operations.

# if defined (ACE_HAS_WCHAR) || defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)
#   define ACE_WINT_T wint_t
#   define ACE_WCHAR_T wchar_t
# else
#   define ACE_WINT_T ACE_UINT16
#   define ACE_WCHAR_T ACE_UINT16
# endif /* ACE_HAS_WCHAR */

# if ACE_SIZEOF_INT == 4
  typedef int ACE_INT32;
  typedef unsigned int ACE_UINT32;
#   if defined (__KCC) && !defined (ACE_LACKS_LONGLONG_T)
  typedef unsigned long long ACE_UINT64;
#   endif /* __KCC */
# elif ACE_SIZEOF_LONG == 4
  typedef long ACE_INT32;
  typedef unsigned long ACE_UINT32;
# elif (ACE_SIZEOF_INT) == 8 && defined(_UNICOS)
  // vector cray - hard 64-bit, all 64 bit types
#   if !defined(_CRAYMPP)
      typedef int ACE_INT32;
      typedef unsigned int ACE_UINT32;
#   endif
  typedef unsigned long long ACE_UINT64;
# else
#   error Have to add to the ACE_UINT32 type setting
# endif

// The number of bytes in a void *.
# ifndef ACE_SIZEOF_VOID_P
#   define ACE_SIZEOF_VOID_P ACE_SIZEOF_LONG
# endif /* ACE_SIZEOF_VOID_P */

// Type for doing arithmetic on pointers ... as elsewhere, we assume
// that unsigned versions of a type are the same size as the signed
// version of the same type.
// NOTE! ptr_arith_t is an ACE-defined type and should not be used.
// It has been superseded by the standard type ptrdiff_t. This definition
// is simply a placeholder til all ptr_arith_t usage can be expunged from
// ACE and TAO.
# if defined (ACE_HAS_WINCE) && (_WIN32_WCE < 400)
typedef unsigned long  ptrdiff_t;    // evc3, PocketPC don't defined ptrdiff_t
# endif
typedef ptrdiff_t ptr_arith_t;

// Byte-order (endian-ness) determination.
# if defined (BYTE_ORDER)
#   if (BYTE_ORDER == LITTLE_ENDIAN)
#     define ACE_LITTLE_ENDIAN 0x0123
#     define ACE_BYTE_ORDER ACE_LITTLE_ENDIAN
#   elif (BYTE_ORDER == BIG_ENDIAN)
#     define ACE_BIG_ENDIAN 0x3210
#     define ACE_BYTE_ORDER ACE_BIG_ENDIAN
#   else
#     error: unknown BYTE_ORDER!
#   endif /* BYTE_ORDER */
# elif defined (_BYTE_ORDER)
#   if (_BYTE_ORDER == _LITTLE_ENDIAN)
#     define ACE_LITTLE_ENDIAN 0x0123
#     define ACE_BYTE_ORDER ACE_LITTLE_ENDIAN
#   elif (_BYTE_ORDER == _BIG_ENDIAN)
#     define ACE_BIG_ENDIAN 0x3210
#     define ACE_BYTE_ORDER ACE_BIG_ENDIAN
#   else
#     error: unknown _BYTE_ORDER!
#   endif /* _BYTE_ORDER */
# elif defined (__BYTE_ORDER)
#   if (__BYTE_ORDER == __LITTLE_ENDIAN)
#     define ACE_LITTLE_ENDIAN 0x0123
#     define ACE_BYTE_ORDER ACE_LITTLE_ENDIAN
#   elif (__BYTE_ORDER == __BIG_ENDIAN)
#     define ACE_BIG_ENDIAN 0x3210
#     define ACE_BYTE_ORDER ACE_BIG_ENDIAN
#   else
#     error: unknown __BYTE_ORDER!
#   endif /* __BYTE_ORDER */
# else /* ! BYTE_ORDER && ! __BYTE_ORDER */
  // We weren't explicitly told, so we have to figure it out . . .
#   if defined (i386) || defined (__i386__) || defined (_M_IX86) || \
     defined (vax) || defined (__alpha) || defined (__LITTLE_ENDIAN__) ||\
     defined (ARM) || defined (_M_IA64)
    // We know these are little endian.
#     define ACE_LITTLE_ENDIAN 0x0123
#     define ACE_BYTE_ORDER ACE_LITTLE_ENDIAN
#   else
    // Otherwise, we assume big endian.
#     define ACE_BIG_ENDIAN 0x3210
#     define ACE_BYTE_ORDER ACE_BIG_ENDIAN
#   endif
# endif /* ! BYTE_ORDER && ! __BYTE_ORDER */

// Byte swapping macros to deal with differences between little endian
// and big endian machines.  Note that "long" here refers to 32 bit
// quantities.
# define ACE_SWAP_LONG(L) ((ACE_SWAP_WORD ((L) & 0xFFFF) << 16) \
            | ACE_SWAP_WORD(((L) >> 16) & 0xFFFF))
# define ACE_SWAP_WORD(L) ((((L) & 0x00FF) << 8) | (((L) & 0xFF00) >> 8))

# if defined (ACE_LITTLE_ENDIAN)
#   define ACE_HTONL(X) ACE_SWAP_LONG (X)
#   define ACE_NTOHL(X) ACE_SWAP_LONG (X)
#   define ACE_IDL_NCTOHL(X) (X)
#   define ACE_IDL_NSTOHL(X) (X)
# else
#   define ACE_HTONL(X) X
#   define ACE_NTOHL(X) X
#   define ACE_IDL_NCTOHL(X) (X << 24)
#   define ACE_IDL_NSTOHL(X) ((X) << 16)
# endif /* ACE_LITTLE_ENDIAN */

# if defined (ACE_LITTLE_ENDIAN)
#   define ACE_HTONS(x) ACE_SWAP_WORD(x)
#   define ACE_NTOHS(x) ACE_SWAP_WORD(x)
# else
#   define ACE_HTONS(x) x
#   define ACE_NTOHS(x) x
# endif /* ACE_LITTLE_ENDIAN */

#if defined (ACE_LACKS_LONGLONG_T)
  // This throws away the high 32 bits.  It's very unlikely that a
  // pointer will be more than 32 bits wide if the platform does not
  // support 64-bit integers.
# define ACE_LONGLONG_TO_PTR(PTR_TYPE, L) \
  ACE_reinterpret_cast (PTR_TYPE, L.lo ())
#else  /* ! ACE_LACKS_LONGLONG_T */
# define ACE_LONGLONG_TO_PTR(PTR_TYPE, L) \
  ACE_reinterpret_cast (PTR_TYPE, ACE_static_cast (ptrdiff_t, L))
#endif /* ! ACE_LACKS_LONGLONG_T */

// If the platform lacks a long long, define one.
# if defined (ACE_LACKS_LONGLONG_T)
// Forward declaration for streams
#   include <iosfwd>

/**
 * @class ACE_U_LongLong
 *
 * @brief Unsigned long long for platforms that don't have one.
 *
 * Provide our own unsigned long long.  This is intended to be
 * use with ACE_High_Res_Timer, so the division operator assumes
 * that the quotient fits into a u_long.
 * Please note that the constructor takes (optionally) two values.
 * The high one contributes 0x100000000 times its value.  So,
 * for example, (0, 2) is _not_ 20000000000, but instead
 * 0x200000000.  To emphasize this, the default values are expressed
 * in hex, and output () dumps the value in hex.
 */
  class  ACE_U_LongLong
  {
  public:
    // = Initialization and termination methods.
    ACE_U_LongLong (const ACE_UINT32 lo = 0x0, const ACE_UINT32 hi = 0x0);
    ACE_U_LongLong (const ACE_U_LongLong &);
    ACE_U_LongLong &operator= (const ACE_U_LongLong &);
    ACE_U_LongLong &operator= (const ACE_INT32 &);
    ACE_U_LongLong &operator= (const ACE_UINT32 &);
    ~ACE_U_LongLong (void);

    // = Overloaded relation operators.
    int operator== (const ACE_U_LongLong &) const;
    int operator== (const ACE_UINT32) const;
    int operator!= (const ACE_U_LongLong &) const;
    int operator!= (const ACE_UINT32) const;
    int operator< (const ACE_U_LongLong &) const;
    int operator< (const ACE_UINT32) const;
    int operator<= (const ACE_U_LongLong &) const;
    int operator<= (const ACE_UINT32) const;
    int operator> (const ACE_U_LongLong &) const;
    int operator> (const ACE_UINT32) const;
    int operator>= (const ACE_U_LongLong &) const;
    int operator>= (const ACE_UINT32) const;

    ACE_U_LongLong operator+ (const ACE_U_LongLong &) const;
    ACE_U_LongLong operator+ (const ACE_UINT32) const;
    ACE_U_LongLong operator- (const ACE_U_LongLong &) const;
    ACE_U_LongLong operator- (const ACE_UINT32) const;
    ACE_U_LongLong operator* (const ACE_UINT32) const;
    ACE_U_LongLong &operator*= (const ACE_UINT32);

    ACE_U_LongLong operator<< (const unsigned int) const;
    ACE_U_LongLong &operator<<= (const unsigned int);
    ACE_U_LongLong operator>> (const unsigned int) const;
    ACE_U_LongLong &operator>>= (const unsigned int);

    double operator/ (const double) const;

    ACE_U_LongLong &operator+= (const ACE_U_LongLong &);
    ACE_U_LongLong &operator+= (const ACE_UINT32);
    ACE_U_LongLong &operator-= (const ACE_U_LongLong &);
    ACE_U_LongLong &operator-= (const ACE_UINT32);
    ACE_U_LongLong &operator++ ();
    ACE_U_LongLong &operator-- ();
    const ACE_U_LongLong operator++ (int);
    const ACE_U_LongLong operator-- (int);
    ACE_U_LongLong &operator|= (const ACE_U_LongLong);
    ACE_U_LongLong &operator|= (const ACE_UINT32);
    ACE_U_LongLong &operator&= (const ACE_U_LongLong);
    ACE_U_LongLong &operator&= (const ACE_UINT32);

    // Note that the following take ACE_UINT32 arguments.  These are
    // typical use cases, and easy to implement.  But, they limit the
    // return values to 32 bits as well.  There are no checks for
    // overflow.
    ACE_UINT32 operator/ (const ACE_UINT32) const;
    ACE_UINT32 operator% (const ACE_UINT32) const;

    // The following only operate on the lower 32 bits (they take only
    // 32 bit arguments).
    ACE_UINT32 operator| (const ACE_INT32) const;
    ACE_UINT32 operator& (const ACE_INT32) const;

    // The following operators convert their arguments to
    // ACE_UINT32.  So, there may be information loss if they are
    // used.
    ACE_U_LongLong operator* (const ACE_INT32) const;
    ACE_U_LongLong &operator*= (const ACE_INT32);
    ACE_UINT32 operator/ (const ACE_INT32) const;
#   if ACE_SIZEOF_INT == 4
    ACE_UINT32 operator/ (const unsigned long) const;
    ACE_UINT32 operator/ (const long) const;
#   else  /* ACE_SIZEOF_INT != 4 */
    ACE_UINT32 operator/ (const unsigned int) const;
    ACE_UINT32 operator/ (const int) const;
#   endif /* ACE_SIZEOF_INT != 4 */

    // = Helper methods.
    /// Outputs the value to the FILE, in hex.
    void output (FILE * = stdout) const;

    ACE_TCHAR *as_string (ACE_TCHAR *string,
                          unsigned int base = 10,
                          unsigned int uppercase = 0) const;

    ACE_UINT32 hi (void) const;
    ACE_UINT32 lo (void) const;

    void hi (const ACE_UINT32 hi);
    void lo (const ACE_UINT32 lo);

  private:
    union
      {
        struct
          {
#     if defined (ACE_BIG_ENDIAN)
            /// High 32 bits.
            ACE_UINT32 hi_;

            /// Low 32 bits.
            ACE_UINT32 lo_;
#     else
            /// Low 32 bits.
            ACE_UINT32 lo_;

            /// High 32 bits.
            ACE_UINT32 hi_;
#     endif /* ! ACE_BIG_ENDIAN */

          } data_;

        /// To ensure alignment on 8-byte boundary.
        /**
         * @note "double" isn't usually usable with
         *       ACE_LACKS_FLOATING_POINT, but this seems OK.
         */
        double for_alignment_;
      };

    // NOTE:  the following four accessors are inlined here in
    // order to minimize the extent of the data_ struct.  It's
    // only used here; the .i and .cpp files use the accessors.

    /// Internal utility function to hide access through struct.
    const ACE_UINT32 &h_ () const { return data_.hi_; }

    /// Internal utility function to hide access through struct.
    ACE_UINT32 &h_ () { return data_.hi_; }

    /// Internal utility function to hide access through struct.
    const ACE_UINT32 &l_ () const { return data_.lo_; }

    /// Internal utility function to hide access through struct.
    ACE_UINT32 &l_ () { return data_.lo_; }

    // NOTE:  the above four accessors are inlined here in
    // order to minimize the extent of the data_ struct.  It's
    // only used here; the .i and .cpp files use the accessors.

    /// These functions are used to implement multiplication.
    ACE_UINT32 ul_shift (ACE_UINT32 a,
                         ACE_UINT32 c_in,
                         ACE_UINT32 *c_out) const;
    ACE_U_LongLong ull_shift (ACE_U_LongLong a,
                              ACE_UINT32 c_in,
                              ACE_UINT32 *c_out) const;
    ACE_U_LongLong ull_add (ACE_U_LongLong a,
                            ACE_U_LongLong b,
                            ACE_UINT32 *carry) const;
    ACE_U_LongLong ull_mult (ACE_U_LongLong a,
                             ACE_UINT32 b,
                             ACE_UINT32 *carry) const;
  };

  typedef ACE_U_LongLong ACE_UINT64;

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
  ostream &operator<< (ostream &, const ACE_U_LongLong &);
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */

# endif /* ACE_LACKS_LONGLONG_T */

// Conversions from ACE_UINT64 to ACE_UINT32.  ACE_CU64_TO_CU32 should
// be used on const ACE_UINT64's.
# if defined (ACE_LACKS_LONGLONG_T)
#   define ACE_U64_TO_U32(n) ((n).lo ())
#   define ACE_CU64_TO_CU32(n) ((n).lo ())
# else  /* ! ACE_LACKS_LONGLONG_T */
#   define ACE_U64_TO_U32(n) (ACE_static_cast (ACE_UINT32, (n)))
#   define ACE_CU64_TO_CU32(n) \
     (ACE_static_cast (ACE_CAST_CONST ACE_UINT32, (n)))
# endif /* ! ACE_LACKS_LONGLONG_T */

// 64-bit literals require special marking on some platforms.
# if defined (ACE_LACKS_LONGLONG_T)
    // Can only specify 32-bit arguments.
#   define ACE_UINT64_LITERAL(n) (ACE_U_LongLong (n))
      // This one won't really work, but it'll keep
      // some compilers happy until we have better support
#   define ACE_INT64_LITERAL(n) (ACE_U_LongLong (n))
# elif defined (ACE_WIN32)
#  if defined (__IBMCPP__) && (__IBMCPP__ >= 400)
#   define ACE_UINT64_LITERAL(n) n ## LL
#   define ACE_INT64_LITERAL(n) n ## LL
#  elif defined (__MINGW32__)
#   define ACE_UINT64_LITERAL(n) n ## ull
#   define ACE_INT64_LITERAL(n) n ## ll
#  else
#   define ACE_UINT64_LITERAL(n) n ## ui64
#   define ACE_INT64_LITERAL(n) n ## i64
#  endif /* defined (__IBMCPP__) && (__IBMCPP__ >= 400) */
# else  /* ! ACE_WIN32  &&  ! ACE_LACKS_LONGLONG_T */
#   define ACE_UINT64_LITERAL(n) n ## ull
#   define ACE_INT64_LITERAL(n) n ## ll
# endif /* ! ACE_WIN32  &&  ! ACE_LACKS_LONGLONG_T */

#if !defined (ACE_UINT64_FORMAT_SPECIFIER)
# define ACE_UINT64_FORMAT_SPECIFIER ACE_LIB_TEXT ("%llu")
#endif /* ACE_UINT64_FORMAT_SPECIFIER */

#if !defined (ACE_INT64_FORMAT_SPECIFIER)
# define ACE_INT64_FORMAT_SPECIFIER ACE_LIB_TEXT ("%lld")
#endif /* ACE_INT64_FORMAT_SPECIFIER */

#if !defined (ACE_SSIZE_T_FORMAT_SPECIFIER)
# if defined (ACE_WIN64)
#  define ACE_SSIZE_T_FORMAT_SPECIFIER ACE_LIB_TEXT ("%I64d")
# else
#  define ACE_SSIZE_T_FORMAT_SPECIFIER ACE_LIB_TEXT ("%d")
# endif /* ACE_WIN64 */
#endif /* ACE_SSIZE_T_FORMAT_SPECIFIER */

#if !defined (ACE_SIZE_T_FORMAT_SPECIFIER)
# if defined (ACE_WIN64)
#  define ACE_SIZE_T_FORMAT_SPECIFIER ACE_LIB_TEXT ("%I64u")
# else
#  define ACE_SIZE_T_FORMAT_SPECIFIER ACE_LIB_TEXT ("%u")
# endif /* ACE_WIN64 */
#endif /* ACE_SIZE_T_FORMAT_SPECIFIER */

// Cast from UINT64 to a double requires an intermediate cast to INT64
// on some platforms.
# if defined (ACE_LACKS_LONGLONG_T)
   // Only use the low 32 bits.
#   define ACE_UINT64_DBLCAST_ADAPTER(n) ACE_U64_TO_U32 (n)
# elif defined (ACE_WIN32)
#   define ACE_UINT64_DBLCAST_ADAPTER(n) ACE_static_cast (__int64, n)
# else  /* ! ACE_WIN32 && ! ACE_LACKS_LONGLONG_T */
#   define ACE_UINT64_DBLCAST_ADAPTER(n) (n)
# endif /* ! ACE_WIN32 && ! ACE_LACKS_LONGLONG_T */


// The number of bytes in a float.
# ifndef ACE_SIZEOF_FLOAT
#   if FLT_MAX_EXP == 128
#     define ACE_SIZEOF_FLOAT 4
#   elif FLT_MAX_EXP == 1024
#     define ACE_SIZEOF_FLOAT 8
#   else
// #     error: unsupported float size, must be updated for this platform!
#   endif /* FLT_MAX_EXP */
# endif /* ACE_SIZEOF_FLOAT */

// The number of bytes in a double.
# ifndef ACE_SIZEOF_DOUBLE
#   if DBL_MAX_EXP == 128
#     define ACE_SIZEOF_DOUBLE 4
#   elif DBL_MAX_EXP == 1024
#     define ACE_SIZEOF_DOUBLE 8
#   else
//#     error: unsupported double size, must be updated for this platform!
#   endif /* DBL_MAX_EXP */
# endif /* ACE_SIZEOF_DOUBLE */

// The number of bytes in a long double.
# ifndef ACE_SIZEOF_LONG_DOUBLE
#   if LDBL_MAX_EXP == 128
#     define ACE_SIZEOF_LONG_DOUBLE 4
#   elif LDBL_MAX_EXP == 1024
#     define ACE_SIZEOF_LONG_DOUBLE 8
#   elif LDBL_MAX_EXP == 16384
#     if defined (LDBL_DIG)  &&  LDBL_DIG == 18
#       if defined (__ia64)
#         define ACE_SIZEOF_LONG_DOUBLE 16
#       else /* ! __ia64 */
#       define ACE_SIZEOF_LONG_DOUBLE 12
#       endif /* __ia64 */
#     else  /* ! LDBL_DIG  ||  LDBL_DIG != 18 */
#       define ACE_SIZEOF_LONG_DOUBLE 16
#     endif /* ! LDBL_DIG  ||  LDBL_DIG != 18 */
#   else
#     error: unsupported double size, must be updated for this platform!
#   endif /* LDBL_MAX_EXP */
# endif /* ACE_SIZEOF_LONG_DOUBLE */

// Max and min sizes for the ACE integer types.
#define ACE_CHAR_MAX 0x7F
#define ACE_CHAR_MIN -(ACE_CHAR_MAX)-1
#define ACE_OCTET_MAX 0xFF
#define ACE_INT16_MAX 0x7FFF
#define ACE_INT16_MIN -(ACE_INT16_MAX)-1
#define ACE_UINT16_MAX 0xFFFF
#define ACE_WCHAR_MAX ACE_UINT16_MAX
#define ACE_INT32_MAX 0x7FFFFFFF
#define ACE_INT32_MIN -(ACE_INT32_MAX)-1
#define ACE_UINT32_MAX 0xFFFFFFFF
#define ACE_INT64_MAX ACE_INT64_LITERAL(0x7FFFFFFFFFFFFFFF)
#define ACE_INT64_MIN -(ACE_INT64_MAX)-1
#define ACE_UINT64_MAX ACE_UINT64_LITERAL(0xFFFFFFFFFFFFFFFF)
// These use ANSI/IEEE format.
#define ACE_FLT_MAX 3.402823466e+38F
#define ACE_DBL_MAX 1.7976931348623158e+308

#if defined (ACE_LACKS_LONGLONG_T)

ACE_INLINE
ACE_U_LongLong::ACE_U_LongLong (const ACE_UINT32 lo, const ACE_UINT32 hi)
{
  h_ () = hi;
  l_ () = lo;
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::hi (void) const
{
  return h_ ();
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::lo (void) const
{
  return l_ ();
}

ACE_INLINE void
ACE_U_LongLong::hi (const ACE_UINT32 hi)
{
  h_ () = hi;
}

ACE_INLINE void
ACE_U_LongLong::lo (const ACE_UINT32 lo)
{
  l_ () = lo;
}

ACE_INLINE
ACE_U_LongLong::~ACE_U_LongLong (void)
{
}

ACE_INLINE int
ACE_U_LongLong::operator== (const ACE_U_LongLong &n) const
{
  return h_ () == n.h_ ()  &&  l_ () == n.l_ ();
}

ACE_INLINE int
ACE_U_LongLong::operator== (const ACE_UINT32 n) const
{
  return h_ () == 0  &&  l_ () == n;
}

ACE_INLINE int
ACE_U_LongLong::operator!= (const ACE_U_LongLong &n) const
{
  return ! (*this == n);
}

ACE_INLINE int
ACE_U_LongLong::operator!= (const ACE_UINT32 n) const
{
  return ! (*this == n);
}

ACE_INLINE int
ACE_U_LongLong::operator< (const ACE_U_LongLong &n) const
{
  return h_ () < n.h_ () ? 1
                         : h_ () > n.h_ () ? 0
                                           : l_ () < n.l_ ();
}

ACE_INLINE int
ACE_U_LongLong::operator< (const ACE_UINT32 n) const
{
  return operator< (ACE_static_cast (const ACE_U_LongLong, n));
}

ACE_INLINE int
ACE_U_LongLong::operator<= (const ACE_U_LongLong &n) const
{
  return h_ () < n.h_ () ? 1
                         : h_ () > n.h_ () ? 0
                                           : l_ () <= n.l_ ();
}

ACE_INLINE int
ACE_U_LongLong::operator<= (const ACE_UINT32 n) const
{
  return operator<= (ACE_static_cast (const ACE_U_LongLong, n));
}

ACE_INLINE int
ACE_U_LongLong::operator> (const ACE_U_LongLong &n) const
{
  return h_ () > n.h_ () ? 1
                         : h_ () < n.h_ () ? 0
                                           : l_ () > n.l_ ();
}

ACE_INLINE int
ACE_U_LongLong::operator> (const ACE_UINT32 n) const
{
  return operator> (ACE_static_cast (const ACE_U_LongLong, n));
}

ACE_INLINE int
ACE_U_LongLong::operator>= (const ACE_U_LongLong &n) const
{
  return h_ () > n.h_ () ? 1
                         : h_ () < n.h_ () ? 0
                                           : l_ () >= n.l_ ();
}

ACE_INLINE int
ACE_U_LongLong::operator>= (const ACE_UINT32 n) const
{
  return operator>= (ACE_static_cast (const ACE_U_LongLong, n));
}

ACE_INLINE
ACE_U_LongLong::ACE_U_LongLong (const ACE_U_LongLong &n)
{
  h_ () = n.h_ ();
  l_ () = n.l_ ();
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator= (const ACE_U_LongLong &n)
{
  h_ () = n.h_ ();
  l_ () = n.l_ ();

  return *this;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator= (const ACE_INT32 &rhs)
{
  if (rhs >= 0)
    {
      l_ () = ACE_static_cast (ACE_UINT32, rhs);
      h_ () = 0;
    }
  else
    {
      // We do not handle the case where a negative 32 bit integer is
      // assigned to this representation of a 64 bit unsigned integer.
      // The "undefined behavior" behavior performed by this
      // implementation is to simply set all bits to zero.
      l_ () = 0;
      h_ () = 0;
    }

  return *this;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator= (const ACE_UINT32 &rhs)
{
  l_ () = rhs;
  h_ () = 0;

  return *this;
}


ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator+ (const ACE_U_LongLong &n) const
{
  ACE_U_LongLong ret (l_ () + n.l_ (), h_ () + n.h_ ());
  if (ret.l_ () < n.l_ ()) /* carry */ ++ret.h_ ();

  return ret;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator+ (const ACE_UINT32 n) const
{
  return operator+ (ACE_static_cast (const ACE_U_LongLong, n));
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator- (const ACE_U_LongLong &n) const
{
  ACE_U_LongLong ret (l_ () - n.l_ (), h_ () - n.h_ ());
  if (l_ () < n.l_ ()) /* borrow */ --ret.h_ ();

  return ret;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator- (const ACE_UINT32 n) const
{
  return operator- (ACE_static_cast (const ACE_U_LongLong, n));
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator<< (const u_int n) const
{
  const ACE_UINT32 carry_mask = l_ () >> (32 - n);
  ACE_U_LongLong ret (n < 32  ?  l_ () << n  :  0,
                      n < 32  ?  (h_ () << n) | carry_mask  :  carry_mask);

  return ret;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator<<= (const u_int n)
{
  const ACE_UINT32 carry_mask = l_ () >> (32 - n);
  h_ () = n < 32  ?  (h_ () << n) | carry_mask  :  carry_mask;

  // g++ 2.7.2.3/Solaris 2.5.1 doesn't modify l_ () if shifted by 32.
  l_ () = n < 32  ?  l_ () << n  :  0;

  return *this;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator>> (const u_int n) const
{
  const ACE_UINT32 carry_mask = h_ () << (32 - n);
  ACE_U_LongLong ret (n < 32  ?  (l_ () >> n) | carry_mask  :  0,
                      n < 32  ?  h_ () >> n  :  0);

  return ret;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator>>= (const u_int n)
{
  const ACE_UINT32 carry_mask = h_ () << (32 - n);
  l_ () = n < 32  ?  (l_ () >> n) | carry_mask  :  carry_mask;
  h_ () = n < 32  ?  h_ () >> n  :  0;

  return *this;
}

ACE_INLINE double
ACE_U_LongLong::operator/ (const double n) const
{
  // See the derivation above in operator/ (const ACE_UINT32).

  return ((double) 0xffffffffu - n + 1.0) / n * h_ ()  +
         (double) h_ ()  +  (double) l_ () / n;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator+= (const ACE_U_LongLong &n)
{
  h_ () += n.h_ ();
  l_ () += n.l_ ();
  if (l_ () < n.l_ ()) /* carry */ ++h_ ();

  return *this;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator+= (const ACE_UINT32 n)
{
  return operator+= (ACE_static_cast (const ACE_U_LongLong, n));
}

#define ACE_HIGHBIT (~(~0UL >> 1))

ACE_INLINE ACE_UINT32
ACE_U_LongLong::ul_shift (ACE_UINT32 a, ACE_UINT32 c_in, ACE_UINT32 *c_out) const
{
  const ACE_UINT32 b = (a << 1) | c_in;
  *c_out = (*c_out << 1) + ((a & ACE_HIGHBIT) > 0);

  return b;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::ull_shift (ACE_U_LongLong a,
                           ACE_UINT32 c_in,
                           ACE_UINT32 *c_out) const
{
  ACE_U_LongLong b;

  b.l_ () = (a.l_ () << 1) | c_in;
  c_in = ((a.l_ () & ACE_HIGHBIT) > 0);
  b.h_ () = (a.h_ () << 1) | c_in;
  *c_out = (*c_out << 1) + ((a.h_ () & ACE_HIGHBIT) > 0);

  return b;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::ull_add (ACE_U_LongLong a, ACE_U_LongLong b, ACE_UINT32 *carry) const
{
  ACE_U_LongLong r (0, 0);
  ACE_UINT32 c1, c2, c3, c4;

  c1 = a.l_ () % 2;
  c2 = b.l_ () % 2;
  c3 = 0;

  r.l_ () = a.l_ ()/2 +  b.l_ ()/2 + (c1+c2)/2;
  r.l_ () = ul_shift (r.l_ (), (c1+c2)%2, &c3);

  c1 = a.h_ () % 2;
  c2 = b.h_ () % 2;
  c4 = 0;

  r.h_ () = a.h_ ()/2 + b.h_ ()/2 + (c1+c2+c3)/2;
  r.h_ () = ul_shift (r.h_ (), (c1+c2+c3)%2, &c4);

  *carry = c4;

  return r;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::ull_mult (ACE_U_LongLong a, ACE_UINT32 b, ACE_UINT32 *carry) const
{
  register ACE_UINT32 mask = ACE_HIGHBIT;
  const ACE_U_LongLong zero (0, 0);
  ACE_U_LongLong accum (0, 0);
  ACE_UINT32 c;

  *carry = 0;
  if (b > 0)
    do
      {
        accum = ull_shift (accum, 0U, carry);
        if (b & mask)
          accum = ull_add (accum, a, &c);
        else
          accum = ull_add (accum, zero, &c);
        *carry += c;
        mask >>= 1;
      }
    while (mask > 0);

  return accum;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator* (const ACE_UINT32 n) const
{
  ACE_UINT32 carry;  // will throw the carry away

  return ull_mult (*this, n, &carry);
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator*= (const ACE_UINT32 n)
{
  ACE_UINT32 carry;  // will throw the carry away

  return *this = ull_mult (*this, n, &carry);
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator-= (const ACE_U_LongLong &n)
{
  h_ () -= n.h_ ();
  if (l_ () < n.l_ ()) /* borrow */ --h_ ();
  l_ () -= n.l_ ();

  return *this;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator-= (const ACE_UINT32 n)
{
  return operator-= (ACE_static_cast (const ACE_U_LongLong, n));
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator++ ()
{
  ++l_ ();
  if (l_ () == 0) /* carry */ ++h_ ();

  return *this;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator-- ()
{
  if (l_ () == 0) /* borrow */ --h_ ();
  --l_ ();

  return *this;
}

ACE_INLINE const ACE_U_LongLong
ACE_U_LongLong::operator++ (int)
{
  // Post-increment operator should always be implemented in terms of
  // the pre-increment operator to enforce consistent semantics.
  ACE_U_LongLong temp (*this);
  ++*this;
  return temp;
}

ACE_INLINE const ACE_U_LongLong
ACE_U_LongLong::operator-- (int)
{
  // Post-decrement operator should always be implemented in terms of
  // the pre-decrement operator to enforce consistent semantics.
  ACE_U_LongLong temp (*this);
  --*this;
  return temp;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator|= (const ACE_U_LongLong n)
{
  l_ () |= n.l_ ();
  h_ () |= n.h_ ();

  return *this;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator|= (const ACE_UINT32 n)
{
  return operator|= (ACE_static_cast (const ACE_U_LongLong, n));
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator&= (const ACE_U_LongLong n)
{
  l_ () &= n.l_ ();
  h_ () &= n.h_ ();

  return *this;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator&= (const ACE_UINT32 n)
{
  return operator&= (ACE_static_cast (const ACE_U_LongLong, n));
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator/ (const ACE_UINT32 n) const
{
  // This takes advantage of the fact that the return type has only 32
  // bits.  Replace 0x100000000 with 0xffffffff + 1 because the former
  // has 33 bits.
  // Quotient = (0x100000000u * hi_ + lo_) / n
  //          = ((0x100000000u - n + n) * hi_ + lo_) / n
  //          = ((0x100000000u - n) / n * hi_  +  hi_ * n / n  +  lo_ / n
  //          = (0x100000000u - n) / n * hi_ +  hi_  +  lo_ / n
  //          = (0xffffffffu - n + 1) / n * hi_ +  hi_  +  lo_ / n

  return (0xffffffffu - n + 1) / n * h_ ()  +  h_ ()  +  l_ () / n;
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator% (const ACE_UINT32 n) const
{
  // Because the argument is an ACE_UINT32, the result can never be
  // bigger than 32 bits.  Replace 0x100000000 with 0xffffffff + 1
  // because the former has 33 bits.
  // Mod = (0x100000000u * hi_ + lo_) % n
  //     = (0x100000000u % n * hi_  +  lo_ % n) % n
  //     = ((0x100000000u - n) % n * hi_  +  lo_ % n) % n
  //     = ((0xffffffffu - n + 1) % n * hi_  +  lo_ % n) % n

  return ((0xffffffff - n + 1)  % n * h_ ()  +  l_ () % n) % n;
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator| (const ACE_INT32 n) const
{
  return l_ () | n;
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator& (const ACE_INT32 n) const
{
  return l_ () & n;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator* (const ACE_INT32 n) const
{
  return operator* ((ACE_UINT32) n);
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator*= (const ACE_INT32 n)
{
  return operator*= ((ACE_UINT32) n);
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator/ (const ACE_INT32 n) const
{
  return operator/ ((ACE_UINT32) n);
}

#if ACE_SIZEOF_INT == 4
ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator/ (const u_long n) const
{
  return operator/ ((ACE_UINT32) n);
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator/ (const long n) const
{
  return operator/ ((ACE_UINT32) n);
}

#else  /* ACE_SIZEOF_INT != 4 */
ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator/ (const u_int n) const
{
  return operator/ ((ACE_UINT32) n);
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator/ (const int n) const
{
  return operator/ ((ACE_UINT32) n);
}
#endif /* ACE_SIZEOF_INT != 4 */

#endif /* ACE_LACKS_LONGLONG_T */

#endif /* ACE_BASIC_TYPES_H */
