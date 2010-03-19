#ifndef _SB_FILE_
#define _SB_FILE_

#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>

class INVENTOR_API SbFile {
public:
    /// Constructs a new file object to represent the file with the given fileName.
    SbFile(const SbString & fileName);
    
    /// Destroys the file object, closing it if necessary.
    ~SbFile();

    /// Returns the name set to the SbFile constructor.
    const SbString & fileName() const{ return mFileName; }

    /// Opens the device with the specified mode. Returns true if successful; otherwise returns false.
    bool open( const SbString & mode );
    
    /// Returns true if the file is open; otherwise returns false. 
    bool isOpen() const;

    /// Closes the file.
    void close();

    /// Returns true if the file exists; otherwise returns false.
    bool exists() const;

    /// Returns the current position indicator of the stream.
    int64_t pos() const;

    /// Sets the current position to pos, returning true on success, or false if an error occurred.
    bool seek(int64_t offset);

    /// Returns the size of the file.
    int64_t size() const;

    /// Returns the number of full items actually read, which may be less than count if an error occurs or if the end of the file is encountered before reaching count.
    size_t read(char * data, size_t count);

    /// Writes up to count items.
    size_t write(const char * data, size_t count);

    /// Returns true if the file specified by fileName exists; otherwise returns false.
    static bool exists( const SbString & fileName );
private:
    // Disables the use of copy constructors and assignment operators
    SbFile(const SbFile &);
    SbFile &operator=(const SbFile &);

    SbString    mFileName;
    FILE *      mFilePointer;
};

#endif /* _SB_FILE_ */
