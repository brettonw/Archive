//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "text.h"

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
/* void */
Text::~Text (void)
{
}

//-----------------------------------------------------------------------------
Text
Text::FromFile (const Text& fileName)
{
    // open the file
    HANDLE  file = CreateFile (fileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL bitor FILE_FLAG_SEQUENTIAL_SCAN, 0);
    if (file != INVALID_HANDLE_VALUE)
    {
        // determine its length
        DWORD   size = GetFileSize (file, 0);
        
        // allocate a buffer the right size
        String  buffer = NewCall Character[size + 1];
        
        // read the contents into a buffer
        DWORD   bytesRead;
        if (not (ReadFile (file, buffer, size, &bytesRead, 0) and (bytesRead == size)))
        {
            // truncate the buffer so a null text object is created
            buffer[0] = 0;
            size = 0;
        }
        
        // close the file
        CloseHandle (file);
        
        // create a text object from the buffer
        Text    text (buffer, size);
            
        // release the buffer
        delete[] buffer;
            
        // return the text object
        return text;
    }
    
    // return an empty text object
    return Text ();
}

//-----------------------------------------------------------------------------
void
Text::ToFile (const Text& fileName, const Text& text)
{
    // open the file
    HANDLE  file = CreateFile (fileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (file != INVALID_HANDLE_VALUE)
    {
        // determine its length
        DWORD   size = text.Length ();
        
        // write the text contents
        DWORD   bytesWritten;
        WriteFile (file, text.GetText (), size, &bytesWritten, 0);
        
        // close the file
        CloseHandle (file);
    }
}

//-----------------------------------------------------------------------------
Text
Text::HexEncode (const void* buffer, uInt length)
{
    // create the buffer for encoding
    uInt        encodedLength = length * 2;
    String      encodedBuffer = NewCall Character[encodedLength + 1];
    
    // encode the buffer
    cString     bufferString = reinterpret_cast<cString> (buffer);
    uInt2*      tmpEncodedBuffer = reinterpret_cast<uInt2*> (encodedBuffer);
    for (uInt i = 0; i < length; i++)
    {
        Character   sprintfBuffer[9];
        sprintf (sprintfBuffer, "%08x", *bufferString++);
        *tmpEncodedBuffer++ = *(reinterpret_cast<uInt2*> (sprintfBuffer + 6));
    }
    
    // build the text object
    Text    text (encodedBuffer, encodedLength);
    
    // clean up
    delete[] encodedBuffer;
    
    // return the resulting Text object
    return text;
}

//-----------------------------------------------------------------------------
void
Text::HexDecode (const Text& text, void* buffer)
{
    // calculate the decoded buffer size
    uInt    decodedBufferLength = text.Length () / 2;
    
    // decode the buffer
    cString         encodedBuffer = static_cast<cString> (text);
    String          decodedBuffer = static_cast<String> (buffer);
    for (uInt i = 0; i < decodedBufferLength; i++)
    {
        uInt    value;
        sscanf (encodedBuffer, "%2x", &value);
        *decodedBuffer++ = static_cast<Character> (value);
        encodedBuffer += 2;
    }
}

//-----------------------------------------------------------------------------
