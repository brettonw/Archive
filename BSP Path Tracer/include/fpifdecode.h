//
// Floating Point Image Format (FPIF) Reference Implementation (RI)
// decoder header file.
//

#ifndef FPIF_DECODE_RI_H
#define	FPIF_DECODE_RI_H

#ifndef FPIF_RI_H
#include "fpif.h"
#endif /* !FPIF_RI_H */

// FPIF decoder exception class hierarchy:
//	Exception (FPIFBaseDecoder)
//	  BadArgument (FPIFBaseDecoder)
//	    BadChannel (FPIFImageDecoder)
//	    BadSize (FPIFImageDecoder)
//	    BadType (FPIFImageDecoder)
//	  BadField (FPIFDecoder)
//	  BadRow (FPIFImageDecoder)
//	  IgnoredBlock (FPIFDecoder)
//	  UnrecoverableException (FPIFBaseDecoder)
//	    BadBlock (FPIFDecoder)
//	    BadDataBlock (FPIFBaseDecoder)
//	    BadHeader (FPIFBaseDecoder)
//	    StreamFailure (FPIFBaseDecoder)
//	    UnsupportedVersion (FPIFDecoder)

//
// FPIFInputStream -- abstract base class of all input stream classes
//	concrete classes must implement template methods:
//	  DoRead() -- read numBytes from stream into buffer, return success
//	  DoSkip() -- skip numBytes in stream, return success
//

class FPIFInputStream {
  public:
			FPIFInputStream();
    virtual		~FPIFInputStream();

    void		Read(FPIFVoid* buffer, FPIFSize numBytes);
    void		Skip(FPIFSize numBytes);

    // BadArgument -- thrown by Read() or Skip() when given invalid arguments.
    class BadArgument { };

    // Failed -- thrown by Read() or Skip() on failure.
    class Failed { };

  protected:
    // override this to do the actual reading of numBytes from the
    // stream into buffer.  you may assume that buffer is a valid
    // pointer to a large enough buffer.  bytes should be read directly
    // to buffer without interpretation (e.g. byte swizzling).
    virtual FPIFBoolean	DoRead(FPIFVoid* buffer, FPIFSize numBytes) = 0;

    // override this to skip numBytes in the stream.
    virtual FPIFBoolean	DoSkip(FPIFSize numBytes) = 0;

  private:
    // no copying
			FPIFInputStream(const FPIFInputStream&);
    FPIFInputStream&	operator=(const FPIFInputStream&);
};

//
// Concrete FPIFInputStream classes
//	FPIFInputStreamFromIStream -- read from a standard C++ istream
//

class istream;
class FPIFInputStreamFromIStream : public FPIFInputStream {
  public:
			FPIFInputStreamFromIStream(istream&);
			~FPIFInputStreamFromIStream();

  protected:
    FPIFBoolean		DoRead(FPIFVoid* buffer, FPIFSize numBytes);
    FPIFBoolean		DoSkip(FPIFSize numBytes);

  private:
    // no copying
    FPIFInputStreamFromIStream(const FPIFInputStreamFromIStream&);
    FPIFInputStreamFromIStream& operator=(const FPIFInputStreamFromIStream&);

  private:
    istream&		stream;
};

//
// FPIFBaseDecoder --
//	lowest level floating point image format decoder;
//	understands only gross block structure of FPIF,
//	including when data blocks follow control blocks.
//

class FPIFBaseDecoder {
  public:
			FPIFBaseDecoder(FPIFInputStream&);
    virtual		~FPIFBaseDecoder();

    // returns byte 4 through 7 of the header block as an array of 4
    // unsigned bytes.
    const FPIFUByte*	GetVersion() const;

    // (skip data blocks then) read next control block into buffer
    FPIFUByte		ReadControlBlock(FPIFVoid* buffer);

    // read data from data block
    FPIFSize		ReadData(FPIFVoid* buffer, FPIFSize numBytes);

    // read data up to next control block.  client is responsible for
    // calling delete[] on the returned data.  number of bytes read
    // is returned in numBytes.
    FPIFVoid*		ReadData(FPIFSize& numBytes);

    // skip numBytes of data
    void		SkipData(FPIFSize numBytes);

    // skip data up to next control block
    void		SkipData();

    // convert a data buffer of Unicode characters to host byte order.
    // note that this method takes the number of bytes, not the number
    // of characters.  the buffer is modified in place and nul terminated.
    // the buffer returned by ReadData(FPIFSize&) will have enough space
    // reserved for the nul terminator.
    static void		UnicodeHostOrder(FPIFVoid* buffer, FPIFSize numBytes);

    // Exception -- base class for decoder exceptions
    class Exception {
      public:
			Exception() { }
	virtual		~Exception() { }
    };

    // BadArgument -- thrown by member functions when given invalid arguments.
    class BadArgument : public Exception { };

    // UnrecoverableException -- base class for unrecoverable exceptions.
    // if an exception derived from this class is thrown, the stream
    // should be closed to prevent further access.
    class UnrecoverableException : public Exception { };

    // StreamFailure -- thrown by Read...() and SkipData() members
    // when the stream fails to read or skip the requested number of
    // bytes.  the stream is assumed to be unrecoverable.
    class StreamFailure : public UnrecoverableException { };

    // BadHeader -- thrown by constructors when the supplied stream
    // is not a valid FPIF stream.
    class BadHeader : public UnrecoverableException { };

    // BadDataBlock -- thrown by ReadData() and SkipData() when:
    //   the length of a data block is out of range
    class BadDataBlock : public UnrecoverableException { };

  protected:
    // closes stream to further access.  normally called when an
    // unrecoverable error is encountered.
    void		Close();

  private:
    void		ReadStream(FPIFVoid* buffer, FPIFSize numBytes);
    FPIFULong		ReadULong();

  private:
    FPIFInputStream&	stream;
    FPIFBoolean		streamOpen;
    FPIFUByte		version[4];
    FPIFSize		bytesLeftInDataBlock;	// bytes to end of data block
};

//
// FPIFDecoder --
//	knows meanings of control blocks and higher level structure
//	of FPIF, including restrictions on when and where certain
//	control blocks may appear.  decodes known blocks and saves
//	relevant data for easy client access. provides a method for
//	getting an FPIFImageDecoder to read image data.
//

class FPIFDecoder : public FPIFBaseDecoder {
  public:
			FPIFDecoder(FPIFInputStream&);
			~FPIFDecoder();

    // returns an array of 4 bytes:  major number, minor number,
    // reserved, reserved.  it indicates the latest version of the
    // format understood by the decoder.
    static const FPIFUByte*	GetSupportedVersion();

    // (skip data blocks then) read and decode next control block
    FPIFUByte		ReadControlBlock();	// returns tag
    FPIFUByte		GetTag() const;		// control block's tag
    FPIFUByte		GetSize() const;	// bytes of data in block
    const FPIFUByte*	GetData() const;	// control block data

    // get info about decoded channel control blocks
    const FPIFChannelList&	GetChannels() const;

    // Info -- summarizes interpreted control block data
    class Info {
      public:
			Info();

      public:
	FPIFBoolean	imageRead;		// true if image data was read
	FPIFBoolean	sizeValid;		// true if size data is valid
	FPIFBoolean	resolutionValid;	// etc.
	FPIFBoolean	offsetValid;
	FPIFBoolean	fovValid;
	FPIFBoolean	depthValid;
	FPIFULong	sizeCount;		// number of size blocks read
	FPIFULong	resolutionCount;	// etc.
	FPIFULong	offsetCount;
	FPIFULong	fovCount;
	FPIFULong	depthCount;

	FPIFUShort	sizeHorizontal;
	FPIFUShort	sizeVertical;
	FPIFFloat	resolutionHorizontal;
	FPIFFloat	resolutionVertical;
	FPIFLong	offsetHorizontal;
	FPIFLong	offsetVertical;
	FPIFFloat	fovHorizontal;
	FPIFFloat	fovVertical;
	FPIFFloat	depthNear;
	FPIFFloat	depthFar;
    };
    const Info*		GetInfo() const;

    // UnsupportedVersion -- thrown by constructor when:
    //   the stream has an unsupported major version
    class UnsupportedVersion : public UnrecoverableException {
      public:
			UnsupportedVersion(const FPIFUByte version[4]);
	const FPIFUByte* GetVersion() const;

      private:
        FPIFUByte	streamVersion[4];
    };

    // BadBlock -- thrown by ReadControlBlock() when:
    //   a block required for correct decoding has invalid data
    //     (e.g. a size block with an invalid size or a
    //      standard channel with an unknown channel type)
    //   a block appears in an invalid place in stream
    //     (e.g. an Image block before a valid Size block
    //      or an End-Of-Stream block before an Image block)
    //   a block appears more times than allowed
    //     (e.g. a 2nd Image block)
    // and the error is unrecoverable.
    class BadBlock : public UnrecoverableException {
      public:
			BadBlock(FPIFUByte _tag) : tag(_tag) {}
	FPIFUByte	GetTag() const { return tag; }

      public:
	const FPIFUByte	tag;
    };

    // IgnoredBlock -- thrown by ReadControlBlock() when:
    //   a block appears in an invalid place in stream
    //     (e.g. a Size block after an Image block)
    //   a block appears more times than allowed
    //     (e.g. a 2nd Size block)
    // and the error is recoverable.
    class IgnoredBlock : public Exception {
      public:
			IgnoredBlock(FPIFUByte _tag) : tag(_tag) {}
	FPIFUByte	GetTag() const { return tag; }

      public:
	const FPIFUByte	tag;
    };

    // BadField -- thrown by ReadControlBlock() when:
    //   an invalid value appears in a field (or fields)
    // and the error is recoverable.
    class BadField : public Exception {
      public:
			BadField(FPIFUByte _tag) : tag(_tag) {}
	FPIFUByte	GetTag() const { return tag; }

      public:
	const FPIFUByte	tag;
    };

  private:
    // no copying
			FPIFDecoder(const FPIFDecoder&);
    FPIFDecoder&	operator=(const FPIFDecoder&);

    // decode most recently read block
    void		DecodeBlock();

    // decode individual block
    void		DecodeEndOfStream();
    void		DecodeFieldOfView();
    void		DecodeImage();
    void		DecodeNearAndFar();
    void		DecodeOffset();
    void		DecodeResolution();
    void		DecodeSize();

    void		DecodeCommonChannel(FPIFVoid*& data, FPIFSize& size);
    void		DecodeExtensionChannel();
    void		DecodeStandardChannel();

  private:
    Info		info;
    FPIFUByte*		block;			// most recent control block
    FPIFBoolean		standardChannelRead[FPIF::NumChannelTypes];
    FPIFChannelList	channels;
    static const FPIFUByte version[4];		// latest supported version
};

//
// FPIFChannelDecoder --
//	abstract base class of all channel decoders.  a channel
//	decoder converts the data in a row from whatever type
//	was in the stream into any desired type.  the decoded
//	type depends on the derived class.
//
//	Decode() will decode the same row over and over until
//	you call NextRow() on the FPIFImageDecoder passed to
//	the constructor of the channel decoder.
//

class FPIFImageDecoder;
class FPIFChannelDecoder {
  public:
			FPIFChannelDecoder(FPIFImageDecoder&,
						FPIFUShort channel);
    virtual		~FPIFChannelDecoder();

    FPIFImageDecoder&	GetImageDecoder() const;
    FPIFUShort		GetChannel() const;

    // decodes a channel in the current row.  derived classes will have
    // a method for setting a buffer to decode into.  derived classes
    // must override this to do the decoding.  should throw
    // FPIFBaseDecoder::BadArgument if the buffer argument is
    // invalid (e.g. NULL).
    virtual void	Decode() = 0;

  protected:
    const FPIFVoid*	GetRow() const;

  private:
    FPIFImageDecoder&	imageDecoder;
    FPIFUShort		channel;
};

//
// FPIFUByteChannelDecoder
// FPIFUShortChannelDecoder
// FPIFULongChannelDecoder
// FPIFFloatChannelDecoder --
//	concrete FPIFChannelDecoder types.  use SetBuffer() to
//	set the buffer where data should be decoded to before
//	calling Decode().  Decode() will continue to use this
//	buffer until another call to SetBuffer().  it must be
//	large enough to hold all the channel data for an entire
//	row (image-width * sizeof(type)).  it should also have
//	alignment appropriate to the platform.
//

class FPIFUByteChannelDecoder : public FPIFChannelDecoder {
  public:
			FPIFUByteChannelDecoder(FPIFImageDecoder&,
						FPIFUShort channel);
			~FPIFUByteChannelDecoder();

    void		SetBuffer(FPIFUByte* buffer);
    void		Decode();

  private:
    FPIFUByte*		buffer;
};

class FPIFUShortChannelDecoder : public FPIFChannelDecoder {
  public:
			FPIFUShortChannelDecoder(FPIFImageDecoder&,
						FPIFUShort channel);
			~FPIFUShortChannelDecoder();

    void		SetBuffer(FPIFUShort* buffer);
    void		Decode();

  private:
    FPIFUShort*		buffer;
};

class FPIFULongChannelDecoder : public FPIFChannelDecoder {
  public:
			FPIFULongChannelDecoder(FPIFImageDecoder&,
						FPIFUShort channel);
			~FPIFULongChannelDecoder();

    void		SetBuffer(FPIFULong* buffer);
    void		Decode();

  private:
    FPIFULong*		buffer;
};

class FPIFFloatChannelDecoder : public FPIFChannelDecoder {
  public:
			FPIFFloatChannelDecoder(FPIFImageDecoder&,
						FPIFUShort channel);
			~FPIFFloatChannelDecoder();

    void		SetBuffer(FPIFFloat* buffer);
    void		Decode();

  private:
    FPIFFloat*		buffer;
};

//
// FPIFImageDecoder --
//	reads Image block data, working with FPIFChannelDecoder
//	classes to provide a client interface for getting decoded
//	image data row by row.  FPIFBaseDecoder (or derived class)
//	must be positioned at the start of data following the
//	Image block.
//

class FPIFImageDecoder {
  public:
			FPIFImageDecoder(FPIFDecoder&);
			FPIFImageDecoder(FPIFBaseDecoder&,
				FPIFUShort width, FPIFUShort height,
				FPIFUShort numChannels,
				const FPIFUByte* types,
				const FPIFFloat* scale);
			~FPIFImageDecoder();

    // get the type of a channel
    FPIFUByte		GetType(FPIFUShort channelNumber) const;

    // get the scale factor of a channel
    FPIFFloat		GetScale(FPIFUShort channelNumber) const;

    // get width and height of the image
    FPIFUShort		GetWidth() const;
    FPIFUShort		GetHeight() const;

    // returns the row number of the row being decoded
    FPIFUShort		GetRowNumber() const;

    // moves the stream to the next row.  you needn't decode any
    // channels between calls to NextRow().  returns true if and
    // only if there are no more rows (i.e. when all rows have
    // been decoded).
    FPIFBoolean		NextRow();

    // BadSize -- thrown by constructor if image size is invalid.
    class BadSize : public FPIFBaseDecoder::BadArgument { };

    // BadType -- thrown by constructor if any channel storage type is invalid
    class BadType : public FPIFBaseDecoder::BadArgument { };

    // BadChannel -- thrown by members when given an invalid channel number
    class BadChannel : public FPIFBaseDecoder::BadArgument { };

    // BadRow -- thrown by NextRow() and GetRow() when past last row
    class BadRow : public FPIFBaseDecoder::Exception { };

  private:
			FPIFImageDecoder(const FPIFImageDecoder&);
    FPIFImageDecoder&	operator=(const FPIFImageDecoder&);

    void		Init();

    // returns a pointer to the raw data from the given channel
    // of the current row.  FPIFChannelDecoder::GetRow will call
    // this and derived channel decoders can call that.
    // this member is private to allow the implementation to
    // change without sacrificing backward compatibility.
    friend const FPIFVoid* FPIFChannelDecoder::GetRow() const;
    const FPIFVoid*	GetRow(FPIFUShort channelNumber);

  private:
    FPIFBaseDecoder&	decoder;
    const FPIFUShort	width, height;
    const FPIFUShort	numChannels;
    FPIFBoolean		needToRead;
    FPIFUShort		row;
    FPIFUByte*		type;
    FPIFFloat*		scale;
    FPIFULong*		channelSize;
    FPIFSize*		channelOffset;
    FPIFUByte*		channelBuffer;
};

#endif /* !FPIF_DECODE_RI_H */
