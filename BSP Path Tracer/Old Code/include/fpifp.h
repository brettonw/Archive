//
// Floating Point Image Format (FPIF) Reference Implementation (RI) source.
// FPIF private header file.  Includes type definitions that shouldn't be
// seen by clients of FPIF and inline function definitions.
//

#ifndef FPIF_PRIVATE_RI_H
#define	FPIF_PRIVATE_RI_H

#ifndef FPIF_RI_H
#include "fpif.h"
#endif /* FPIF_RI_H */

#ifndef NULL
#define	NULL	0
#endif

//
// supported byte orders
//
#define	FPIF_LITTLE_ENDIAN	1234
#define	FPIF_BIG_ENDIAN		4321

//
// different methods for checking NaN.  see FPIF::IsNaN
//
#define	FPIF_NAN_FUNCTION	0
#define	FPIF_NAN_BITMASK	1
#define	FPIF_NAN_NOTEQUAL	2

//
// make sure these get set below
//
#if defined(FPIF_BYTE_ORDER)
#undef FPIF_BYTE_ORDER
#endif
#if defined(FPIF_NAN)
#undef FPIF_NAN
#endif

//
// PLATFORM DEPENDENT -- start of platform dependencies
//

// this type is only used internally to avoid excessive loss of
// precision when converting from FPIFULong or FPIFLong to FPIFFloat.
typedef double		FPIFDouble;		// double precision float

// integer and floating point literal suffix macros.  appends the
// appropriate suffix to a literal, depending on the underlying type,
// and type casts.
#define	FPIFBYTE(_x)	(_x)
#define	FPIFUBYTE(_x)	(_x ## u)
#define	FPIFSHORT(_x)	(_x)
#define	FPIFUSHORT(_x)	(_x ## u)
#define	FPIFLONG(_x)	(_x)
#define	FPIFULONG(_x)	(_x ## u)
#define	FPIFFLOAT(_x)	(_x ## f)
#define	FPIFDOUBLE(_x)	(_x)
#define	FPIFSIZE(_x)	(_x)

// get FLT_MAX, the maximum positive value representable by FPIFFloat
#include <float.h>

// uncomment this is the implementation is fully 64 bit.
// this means pointers are 64 bits, FPIFSize is 64 bits, and all
// I/O operations accept 64 bit offsets and sizes and operate on
// a 64 bit filesystem.
//#define	FPIF_64BIT

// determine host byte order
#if defined(__sgi)
# include <sys/endian.h>
# if BYTE_ORDER == BIG_ENDIAN
#  define FPIF_BYTE_ORDER	FPIF_BIG_ENDIAN
# elif BYTE_ORDER == LITTLE_ENDIAN
#  define FPIF_BYTE_ORDER	FPIF_LITTLE_ENDIAN
# endif
#endif

// FPIF::IsNaN needs a way to identify a FPIFFloat not-a-number.
// this is not as easy and cross platform as it should be.
#if defined(__sgi)
# include <ieeefp.h>
# define FPIF_NAN		FPIF_NAN_FUNCTION
#endif

//
// PLATFORM DEPENDENT -- end of platform dependencies
//

#if !defined(FPIF_BYTE_ORDER)
#error FPIF_BYTE_ORDER undefined -- FPIF_BIG_ENDIAN assumed
#define FPIF_BYTE_ORDER		FPIF_BIG_ENDIAN
#endif
#if !defined(FPIF_NAN)
#error FPIF_NAN undefined -- FPIF_NAN_NOTEQUAL assumed
#define FPIF_NAN		FPIF_NAN_NOTEQUAL
#endif

//
// FPIF inlines
//
// FPIF_BIG_ENDIAN
//

#if FPIF_BYTE_ORDER == FPIF_BIG_ENDIAN

inline FPIFShort	FPIF::ToShort(const FPIFVoid* _buffer)
{
  return *((const FPIFShort*)_buffer);
}

inline FPIFLong		FPIF::ToLong(const FPIFVoid* _buffer)
{
  return *((const FPIFLong*)_buffer);
}

inline FPIFUShort	FPIF::ToUShort(const FPIFVoid* _buffer)
{
  return *((const FPIFUShort*)_buffer);
}

inline FPIFULong	FPIF::ToULong(const FPIFVoid* _buffer)
{
  return *((const FPIFULong*)_buffer);
}

inline FPIFFloat	FPIF::ToFloat(const FPIFVoid* _buffer)
{
  return *((const FPIFFloat*)_buffer);
}

inline void		FPIF::FromShort(FPIFShort data, FPIFVoid* _buffer)
{
  *((FPIFShort*)_buffer) = data;
}

inline void		FPIF::FromLong(FPIFLong data, FPIFVoid* _buffer)
{
  *((FPIFLong*)_buffer) = data;
}

inline void		FPIF::FromUShort(FPIFUShort data, FPIFVoid* _buffer)
{
  *((FPIFUShort*)_buffer) = data;
}

inline void		FPIF::FromULong(FPIFULong data, FPIFVoid* _buffer)
{
  *((FPIFULong*)_buffer) = data;
}

inline void		FPIF::FromFloat(FPIFFloat data, FPIFVoid* _buffer)
{
  *((FPIFFloat*)_buffer) = data;
}


#elif FPIF_BYTE_ORDER == FPIF_LITTLE_ENDIAN
//
// FPIF_LITTLE_ENDIAN
//

inline FPIFShort	FPIF::ToShort(const FPIFVoid* _buffer)
{
  FPIFShort data;
  FPIFUByte* raw = (FPIFUByte*)&data;
  const FPIFUByte* buffer = (const FPIFUByte*)_buffer;
  raw[0] = buffer[1];
  raw[1] = buffer[0];
  return data;
}

inline FPIFLong		FPIF::ToLong(const FPIFVoid* _buffer)
{
  FPIFLong data;
  FPIFUByte* raw = (FPIFUByte*)&data;
  const FPIFUByte* buffer = (const FPIFUByte*)_buffer;
  raw[0] = buffer[3];
  raw[1] = buffer[2];
  raw[2] = buffer[1];
  raw[3] = buffer[0];
  return data;
}

inline FPIFUShort	FPIF::ToUShort(const FPIFVoid* _buffer)
{
  FPIFUShort data;
  FPIFUByte* raw = (FPIFUByte*)&data;
  const FPIFUByte* buffer = (const FPIFUByte*)_buffer;
  raw[0] = buffer[1];
  raw[1] = buffer[0];
  return data;
}

inline FPIFULong	FPIF::ToULong(const FPIFVoid* _buffer)
{
  FPIFULong data;
  FPIFUByte* raw = (FPIFUByte*)&data;
  const FPIFUByte* buffer = (const FPIFUByte*)_buffer;
  raw[0] = buffer[3];
  raw[1] = buffer[2];
  raw[2] = buffer[1];
  raw[3] = buffer[0];
  return data;
}

inline FPIFFloat	FPIF::ToFloat(const FPIFVoid* _buffer)
{
  FPIFFloat data;
  FPIFUByte* raw = (FPIFUByte*)&data;
  const FPIFUByte* buffer = (const FPIFUByte*)_buffer;
  raw[0] = buffer[3];
  raw[1] = buffer[2];
  raw[2] = buffer[1];
  raw[3] = buffer[0];
  return data;
}

inline void		FPIF::FromShort(FPIFShort data, FPIFVoid* _buffer)
{
  const FPIFUByte* raw = (FPIFUByte*)&data;
  FPIFUByte* buffer = (FPIFUByte*)_buffer;
  buffer[0] = raw[1];
  buffer[1] = raw[0];
}

inline void		FPIF::FromLong(FPIFLong data, FPIFVoid* _buffer)
{
  const FPIFUByte* raw = (FPIFUByte*)&data;
  FPIFUByte* buffer = (FPIFUByte*)_buffer;
  buffer[0] = raw[3];
  buffer[1] = raw[2];
  buffer[2] = raw[1];
  buffer[3] = raw[0];
}

inline void		FPIF::FromUShort(FPIFUShort data, FPIFVoid* _buffer)
{
  const FPIFUByte* raw = (FPIFUByte*)&data;
  FPIFUByte* buffer = (FPIFUByte*)_buffer;
  buffer[0] = raw[1];
  buffer[1] = raw[0];
}

inline void		FPIF::FromULong(FPIFULong data, FPIFVoid* _buffer)
{
  const FPIFUByte* raw = (FPIFUByte*)&data;
  FPIFUByte* buffer = (FPIFUByte*)_buffer;
  buffer[0] = raw[3];
  buffer[1] = raw[2];
  buffer[2] = raw[1];
  buffer[3] = raw[0];
}

inline void		FPIF::FromFloat(FPIFFloat data, FPIFVoid* _buffer)
{
  const FPIFUByte* raw = (FPIFUByte*)&data;
  FPIFUByte* buffer = (FPIFUByte*)_buffer;
  buffer[0] = raw[3];
  buffer[1] = raw[2];
  buffer[2] = raw[1];
  buffer[3] = raw[0];
}


#else
//
// unknown byte order
//

#error Unknown byte order

#endif

#endif /* FPIF_PRIVATE_RI_H */
