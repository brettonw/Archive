//
// Floating Point Image Format (FPIF) Reference Implementation (RI)
// encoder header file.
//

#ifndef FPIF_ENCODE_RI_H
#define	FPIF_ENCODE_RI_H

#ifndef FPIF_RI_H
#include "fpif.h"
#endif /* !FPIF_RI_H */

// FPIF encoder exception class hierarchy:
//	Exception (FPIFBaseEncoder)
//	  BadArgument (FPIFBaseEncoder)
//	    BadChannel (FPIFImageEncoder)
//	    BadSize (FPIFImageEncoder)
//	    BadType (FPIFImageEncoder)
//	  BadBlock (FPIFEncoder)
//	  BadDataBlock (FPIFBaseEncoder)
//	  BadField (FPIFEncoder)
//	  BadRow (FPIFImageEncoder)
//	  UnrecoverableException (FPIFBaseEncoder)
//	    StreamFailure (FPIFBaseEncoder)

//
// FPIFOutputStream -- abstract base class of all output stream classes
//	concrete classes must implement template method:
//	  DoWrite() -- write numBytes from buffer to stream, return success
//	  DoFlush() -- flush any output buffers
//

class FPIFOutputStream {
  public:
			FPIFOutputStream();
    virtual		~FPIFOutputStream();

    void		Write(const FPIFVoid* buffer, FPIFSize numBytes);
    void		Flush();

    // BadArgument -- thrown by Write() when given invalid arguments.
    class BadArgument { };

    // Failed -- thrown by Write() or Flush() on failure.
    class Failed { };

  protected:
    // override this to do the actual writing of numBytes in buffer to
    // the stream.  bytes should be written directly from buffer without
    // interpretation (e.g. byte swizzling).
    virtual FPIFBoolean	DoWrite(const FPIFVoid* buffer, FPIFSize numBytes) = 0;
 
    // override this to flush buffered output to the stream.
    virtual void	DoFlush() = 0;

  private:
    // no copying
			FPIFOutputStream(const FPIFOutputStream&);
    FPIFOutputStream&	operator=(const FPIFOutputStream&);
};

//
// Concrete FPIFOutputStream classes
//	FPIFOutputStreamToOStream -- read from a standard C++ ostream
//

class ostream;
class FPIFOutputStreamToOStream : public FPIFOutputStream {
  public:
			FPIFOutputStreamToOStream(ostream&);
			~FPIFOutputStreamToOStream();

  private:
    // no copying
    FPIFOutputStreamToOStream(const FPIFOutputStreamToOStream&);
    FPIFOutputStreamToOStream& operator=(const FPIFOutputStreamToOStream&);

    FPIFBoolean		DoWrite(const FPIFVoid* buffer, FPIFSize numBytes);
    void		DoFlush();

  private:
    ostream&		stream;
};

//
// FPIFBaseEncoder --
//	lowest level floating point image format encoder;
//	understands only gross block structure of FPIF,
//	including when data blocks follow control blocks.
//

class FPIFBaseEncoder {
  public:
			FPIFBaseEncoder(FPIFOutputStream&,
					const FPIFUByte version[4]);
    virtual		~FPIFBaseEncoder();

    void		WriteControlBlock(FPIFUByte tag, FPIFUByte size,
						const FPIFVoid* buffer);
    void		WriteData(const FPIFVoid* buffer,
						FPIFSize numBytes);
    void		WriteTextData(const FPIFUShort* buffer,
						FPIFSize numChars);

    // Exception -- base class for encoder exceptions
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

    // StreamFailure -- thrown by Write...() members when the stream
    // fails to write the requested number of bytes.  the stream is
    // assumed to be unrecoverable.
    class StreamFailure : public UnrecoverableException { };

    // BadDataBlock exception class -- thrown by WriteDataBlock() when
    // called when the FPIF format doesn't allow a data block (in
    // particular, after a control block whose tag indicates that
    // data does *not* follow).
    class BadDataBlock : public Exception { };

  protected:
    // closes stream to further access.  normally called when an
    // unrecoverable error is encountered.
    void		Close();

  private:
    // no copying
			FPIFBaseEncoder(const FPIFBaseEncoder&);
    FPIFBaseEncoder&	operator=(const FPIFBaseEncoder&);

    void		WriteStream(const FPIFVoid* buffer, FPIFSize numBytes);
    void		WriteUShort(FPIFUShort);
    void		WriteULong(FPIFULong);

  private:
    FPIFOutputStream&	stream;
    FPIFBoolean		streamOpen;
    FPIFBoolean		endOfDataPending;	// need to write last data block
};

//
// FPIFEncoder --
//	knows meanings of control blocks and higher level structure
//	of FPIF, including restrictions on when and where certain
//	control blocks may appear.
//

class FPIFEncoder : public FPIFBaseEncoder {
  public:
			FPIFEncoder(FPIFOutputStream&);
			~FPIFEncoder();

    void		SetSize(FPIFUShort horizontal, FPIFUShort vertical);
    void		SetResolution(FPIFFloat horiz, FPIFFloat vert);
    void		SetOffset(FPIFLong horiz, FPIFLong vert);
    void		SetFieldOfView(FPIFFloat horiz, FPIFFloat vert);
    void		SetNearAndFar(FPIFFloat horiz, FPIFFloat vert);

    void		AddStandardChannel(
				FPIF::DataType type, FPIF::Channel channel);
    void		AddExtensionChannel(
				FPIF::DataType type, const FPIFByte* channel,
				FPIFVoid* data = 0, FPIFSize numBytes = 0);

    // type is a zero terminated ASCII string
    void		WriteBinaryUserData(const FPIFByte* type,
				const FPIFVoid* data = 0,
				FPIFSize numBytes = 0);
    // type is a zero terminated ASCII string.  data is a Unicode string.
    // numChars is the number of Unicode characters in data.
    void		WriteTextUserData(const FPIFByte* type,
				const FPIFUShort* data, FPIFSize numChars);

    // get image size
    FPIFUShort		GetSizeHorizontal() const;
    FPIFUShort		GetSizeVertical() const;

    // get info about added channel control blocks
    const FPIFChannelList&	GetChannels() const;

    // FIXME -- helper code to format history (etc.) user-data?

    // BadBlock -- thrown when client attempts to write a block
    // that would create an invalid FPIF file;  for example, writing
    // a Size block after the image data, writing a Size block twice,
    // writing an Image block before a Size block, etc.
    class BadBlock : public Exception  { };

    // BadField -- thrown when client attempts to write a block with
    // an invalid field;  for example, writing a Size block with the
    // horizontal argument equal to zero, writing a user data block
    // with a type string with invalid characters or longer than 255
    // bytes, etc.
    class BadField : public Exception { };

  public:
    static const FPIFUByte	version[4];	// latest supported version

  private:
    // no copying
			FPIFEncoder(const FPIFEncoder&);
    FPIFEncoder&	operator=(const FPIFEncoder&);

    friend class FPIFImageEncoder;
    void		PrepareForImage();

  private:
    FPIFBoolean		imageWritten;
    FPIFBoolean		sizeWritten;
    FPIFBoolean		resolutionWritten;
    FPIFBoolean		offsetWritten;
    FPIFBoolean		fovWritten;
    FPIFBoolean		depthWritten;
    FPIFBoolean		standardChannelWritten[FPIF::NumChannelTypes];
    FPIFUShort		width, height;
    FPIFChannelList	channels;
};

//
// FPIFChannelEncoder --
//	abstract base class of all channel encoders.  a channel
//	encoder converts client data for a row into the type
//	to be stored in the stream.  the encoded type depends
//	on the derived class.
//
//	Encode() will encode the same row over and over until
//	you call NextRow() on the FPIFImageEncoder passed to
//	the constructor of the channel encoder.
//

class FPIFImageEncoder;
class FPIFChannelEncoder {
  public:
			FPIFChannelEncoder(FPIFImageEncoder&,
						FPIFUShort channel);
    virtual		~FPIFChannelEncoder();

    FPIFImageEncoder&	GetImageEncoder() const;
    FPIFUShort		GetChannel() const;

    // encodes a channel in the current row.  derived classes will have
    // a method for setting a buffer to decode from.  derived classes
    // must override this to do the encoding.  should throw
    // FPIFBaseEncoder::BadArgument if the buffer argument is
    // invalid (e.g. NULL).
    virtual void	Encode() = 0;

  protected:
    FPIFVoid*		GetRow() const;

  private:
    FPIFImageEncoder&	imageEncoder;
    FPIFUShort		channel;
};

//
// FPIFUByteChannelEncoder
// FPIFUShortChannelEncoder
// FPIFULongChannelEncoder
// FPIFFloatChannelEncoder --
//	concrete FPIFChannelEncoder types.  use SetBuffer() to
//	set the buffer where data should be encoded from before
//	calling Encode().  Encode() will continue to use this
//	buffer until another call to SetBuffer().
//

class FPIFUByteChannelEncoder : public FPIFChannelEncoder {
  public:
			FPIFUByteChannelEncoder(FPIFImageEncoder&,
						FPIFUShort channel);
			~FPIFUByteChannelEncoder();

    void		SetBuffer(const FPIFUByte* buffer);
    void		Encode();

  private:
    const FPIFUByte*	buffer;
};

class FPIFUShortChannelEncoder : public FPIFChannelEncoder {
  public:
			FPIFUShortChannelEncoder(FPIFImageEncoder&,
						FPIFUShort channel);
			~FPIFUShortChannelEncoder();

    void		SetBuffer(const FPIFUShort* buffer);
    void		Encode();

  private:
    const FPIFUShort*	buffer;
};

class FPIFULongChannelEncoder : public FPIFChannelEncoder {
  public:
			FPIFULongChannelEncoder(FPIFImageEncoder&,
						FPIFUShort channel);
			~FPIFULongChannelEncoder();

    void		SetBuffer(const FPIFULong* buffer);
    void		Encode();

  private:
    const FPIFULong*	buffer;
};

class FPIFFloatChannelEncoder : public FPIFChannelEncoder {
  public:
			FPIFFloatChannelEncoder(FPIFImageEncoder&,
						FPIFUShort channel);
			~FPIFFloatChannelEncoder();

    void		SetBuffer(const FPIFFloat* buffer);
    void		Encode();

  private:
    const FPIFFloat*	buffer;
};

//
// FPIFImageEncoder --
//	encodes and writes Image block data, providing a client
//	interface for writing image data row by row.  It also
//	writes the Image control block itself.
//

class FPIFImageEncoder {
  public:
			FPIFImageEncoder(FPIFEncoder&);
			FPIFImageEncoder(FPIFBaseEncoder&,
				FPIFUShort width, FPIFUShort height,
				FPIFUShort numChannels,
				const FPIFUByte* types,
				const FPIFFloat* scale);
			~FPIFImageEncoder();

    // get the type of a channel
    FPIFUByte		GetType(FPIFUShort channelNumber) const;

    // get the scale factor of a channel
    FPIFFloat		GetScale(FPIFUShort channelNumber) const;

    // get width and height of the image
    FPIFUShort		GetWidth() const;
    FPIFUShort		GetHeight() const;

    // returns the row number of the row being encoded
    FPIFUShort		GetRowNumber() const;

    // writes the row data and moves the stream to the next row.
    // if a channel's data isn't encoded, this implementation will
    // write the data from the previous row.  the first row's data
    // is all zero.
    void		NextRow();

    // BadSize -- thrown by constructor if image size is invalid.
    class BadSize : public FPIFBaseEncoder::BadArgument { };

    // BadType -- thrown by constructor if any channel storage type is invalid
    class BadType : public FPIFBaseEncoder::BadArgument { };

    // BadChannel -- thrown by members() when given an invalid channel number
    class BadChannel : public FPIFBaseEncoder::BadArgument { };

    // BadRow -- thrown by NextRow() or GetRow() when past last row
    class BadRow : public FPIFBaseEncoder::Exception { };

  private:
			FPIFImageEncoder(const FPIFImageEncoder&);
    FPIFImageEncoder&	operator=(const FPIFImageEncoder&);

    void		Init1();
    void		Init2();

    // returns a pointer to the buffer where a channel's raw data
    // is stored in preparation for NextRow().
    // FPIFChannelEncoder::GetRow will call this and derived channel
    // encoders can call that.  encoders will write directly into
    // this buffer, which will have an appropriate alignment for the
    // data type.  this member is private to allow the implementation
    // to change without sacrificing backward compatibility.
    friend FPIFVoid*	FPIFChannelEncoder::GetRow() const;
    FPIFVoid*		GetRow(FPIFUShort channelNumber);

  private:
    FPIFBaseEncoder&	encoder;
    const FPIFUShort	width, height;
    const FPIFUShort	numChannels;
    FPIFUShort		row;
    FPIFUByte*		type;
    FPIFFloat*		scale;
    FPIFULong*		channelSize;
    FPIFSize*		channelOffset;
    FPIFUByte*		channelBuffer;
};

#endif /* !FPIF_ENCODE_RI_H */
