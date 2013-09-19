//
// Floating Point Image Format (FPIF) Reference Implementation (RI)
// common header file.
//

#ifndef FPIF_RI_H
#define	FPIF_RI_H

//
// basic types with consistent sizes across platforms;  except
// FPIFBoolean which only needs to store true and false; and
// FPIFSize which must be able to store the difference of two
// pointers.
//
// FPIFSize is either a 32 or 64 bit signed intenger, depending on
// the platform.  If the platform doesn't support 64 bit integers,
// 64 bit pointers, and a 64 bit interface to the file I/O system,
// then FPIFSize should be 32 bits and the implementation is limited
// to FPIF streams less than 2^31 bytes long.  The standard type
// ptrdiff_t should automatically answer the first two questions.
//
// Do not use FPIFSize for anything except specifying the number of
// bytes to read from, write to, or skip over in a stream (using an
// FPIFSize for the size of data to read from or write to a data
// block is allowed).
//
// PLATFORM DEPENDENT -- these typedefs are platform dependent
//
typedef void		FPIFVoid;		// untyped storage
typedef bool		FPIFBoolean;		// arbitrary size
typedef signed char	FPIFByte;		// 8 bit signed
typedef unsigned char	FPIFUByte;		// 8 bit unsigned
typedef short		FPIFShort;		// 16 bit signed
typedef unsigned short	FPIFUShort;		// 16 bit unsigned
typedef int		FPIFLong;		// 32 bit signed
typedef unsigned int	FPIFULong;		// 32 bit unsigned
typedef float		FPIFFloat;		// IEEE single precision
#include <stddef.h>
typedef ptrdiff_t	FPIFSize;		// difference of pointers

//
// FPIF -- global data and functions for floating point image format.
//

class FPIF {
  public:
    enum Tag {
			EndOfStream =			0x00,
			Image =				0x01,
			StandardChannel =		0x02,
			ExtensionChannel =		0x04,
			ExtensionChannelWithData =	0x05,
			Size =				0x06,
			BinaryUserData =		0x80,
			BinaryUserDataWithData =	0x81,
			TextUserDataWithData =		0x83,
			Resolution =			0xc0,
			Offset =			0xc4,
			FieldOfView =			0xc6,
			NearAndFar =			0xc8
    };
    enum DataType {
			UnsignedByte =			0x01,
			UnsignedShort =			0x02,
			UnsignedLong =			0x04,
			Float =				0xc4
    };
    enum Channel {
			// don't reorder these!
			Chromaticity,
			Intensity,
			Alpha,
			DepthLinear,
			DepthInverse,
			Item,
			NumChannelTypes		// always put me last
    };

    static const FPIFUByte*	GetMagic();	// header magic number

    // see if this implementation supports 64 bit streams
    static FPIFBoolean	Is64Bit();

    // check tag for characteristics
    static FPIFBoolean	DataFollowsBlock(FPIFUByte tag);
    static FPIFBoolean	MayIgnoreBlock(FPIFUByte tag);
    static FPIFBoolean	LossyIgnoreBlock(FPIFUByte tag);

    // convert raw bytes in network byte order into host data.
    static FPIFShort	ToShort(const FPIFVoid*);
    static FPIFLong	ToLong(const FPIFVoid*);
    static FPIFUShort	ToUShort(const FPIFVoid*);
    static FPIFULong	ToULong(const FPIFVoid*);
    static FPIFFloat	ToFloat(const FPIFVoid*);

    // convert host data to raw bytes in network byte order.
    static void		FromShort(FPIFShort, FPIFVoid*);
    static void		FromLong(FPIFLong, FPIFVoid*);
    static void		FromUShort(FPIFUShort, FPIFVoid*);
    static void		FromULong(FPIFULong, FPIFVoid*);
    static void		FromFloat(FPIFFloat, FPIFVoid*);

    // test finiteness of an FPIFFloat
    static FPIFBoolean	IsFinite(FPIFFloat);

    // test if FPIFFloat is NaN (not a number)
    static FPIFBoolean	IsNaN(FPIFFloat);

  private:
    // no instances of this class
    FPIF();

  private:
    static const FPIFUByte	magic[4];
};

//
// FPIFChannel -- holds information about one channel.
//

class FPIFChannel {
  public:
			FPIFChannel(FPIFUByte storageType,
				FPIF::Channel standardType,
				FPIFVoid* adoptedData = 0,
				FPIFSize numBytes = 0,
				FPIFBoolean isYChromaticity = false);
			FPIFChannel(FPIFUByte storageType,
				const FPIFByte* extensionType,
				FPIFUByte extensionTypeLength,
				FPIFVoid* adoptedData = 0,
				FPIFSize numBytes = 0);
			~FPIFChannel();

    FPIFBoolean		IsStandardChannel() const;
    FPIFBoolean		IsYChromaticity() const;
    FPIFUByte		GetStorageType() const;
    FPIF::Channel	GetStandardType() const;
    const FPIFByte*	GetExtensionType() const;
    FPIFSize		GetDataSize() const;
    const FPIFVoid*	GetData() const;

  private:
    FPIFBoolean		yChromaticity;
    FPIFUByte		storageType;
    FPIF::Channel	stdDataType;
    FPIFByte*		extDataType;
    FPIFVoid*		data;
    FPIFSize		numBytes;
};

//
// FPIFChannelList -- holds an ordered unbounded list of FPIFChannel.
//

class FPIFChannelList {
  public:
			FPIFChannelList();
			~FPIFChannelList();

    FPIFUShort		GetNumChannels() const;
    const FPIFChannel*	GetChannel(FPIFUShort index) const;

    void		Append(FPIFChannel* adopt);

    // ListFull -- thrown by Append() if the list cannot grow.
    class ListFull { };

  private:
			FPIFChannelList(const FPIFChannelList&);
    const FPIFChannelList& operator=(const FPIFChannelList&);

    void		Grow(FPIFUShort size);

  private:
    FPIFUShort		numChannels;
    FPIFUShort		sizeChannels;
    FPIFChannel**	channel;
};

#endif /* !FPIF_RI_H */
