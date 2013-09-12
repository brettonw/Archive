//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_TYPE_DEFINES_H_
#define		_BW_TYPE_DEFINES_H_

//------------------------------------------------------------------------------
//	type definitions
//------------------------------------------------------------------------------
typedef	char			    sInt1;												//	a signed integer 1 byte in length
typedef	short			    sInt2;												//	a signed integer 2 bytes in length
typedef	long			    sInt4;												//	a signed integer 4 bytes in length

typedef	unsigned char	    uInt1;												//	an unsigned integer 1 byte in length
typedef	unsigned short	    uInt2;												//	an unsigned integer 2 bytes in length
typedef	unsigned long	    uInt4;												//	an unsigned integer 4 bytes in length

typedef	sInt4			    sInt;												//	signed integer
typedef	uInt4			    uInt;												//	unsigned integer

typedef sInt1               Character;                                          //  a character
typedef Character*	        String;												//	a common string type
typedef	const Character*    cString;											//	a common string type that is const

typedef	sInt1*			    sPointer;											//	standard pointer type
typedef	uInt1*			    uPointer;											//	standard pointer type

//------------------------------------------------------------------------------
//	type limits
//------------------------------------------------------------------------------
#define	SINT1_MIN		    CHAR_MIN											//	minimum value
#define	SINT1_MAX		    CHAR_MAX											//	maximum value
#define	SINT2_MIN		    SHRT_MIN											//	minimum value
#define	SINT2_MAX		    SHRT_MAX											//	maximum value
#define	SINT4_MIN		    LONG_MIN											//	minimum value
#define	SINT4_MAX		    LONG_MAX											//	maximum value

#define	UINT1_MIN		    0													//	minimum value
#define	UINT1_MAX		    UCHAR_MAX											//	maximum value
#define	UINT2_MIN		    0													//	minimum value
#define	UINT2_MAX		    USHRT_MAX											//	maximum value
#define	UINT4_MIN		    0													//	minimum value
#define	UINT4_MAX		    ULONG_MAX											//	maximum value

#define	SINT_MIN		    SINT4_MIN											//	minimum value
#define	SINT_MAX		    SINT4_MAX											//	maximum value

#undef	UINT_MAX
#define	UINT_MIN		    UINT4_MIN											//	minimum value
#define	UINT_MAX		    UINT4_MAX											//	maximum value

//------------------------------------------------------------------------------

#endif // BW_TYPE_DEFINES
