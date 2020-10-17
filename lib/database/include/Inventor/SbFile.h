#pragma once

#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>

class INVENTOR_API SbFile {
  public:
    /// Constructs a new file object.
    SbFile();

    /// Constructs a new file object.
    SbFile(FILE *fp);

    /// Destroys the file object, closing it if necessary.
    ~SbFile();

    /// Opens the device with the specified mode. Returns true if successful;
    /// otherwise returns false.
    bool open(const SbString &fileName, const SbString &mode);

    /// Returns true if the file is open; otherwise returns false.
    bool isOpen() const;

    /// Closes the file.
    void close();

    /// Sets file pointer to read from.
    void setFilePointer(FILE *fp);

    /// Returns the file pointer.
    FILE *getFilePointer() const;

    /// Returns true if the file exists; otherwise returns false.
    bool exists() const;

    /// Returns the current position indicator of the stream.
    int64_t pos() const;

    /// Sets the current position to pos, returning true on success, or false if
    /// an error occurred.
    bool seek(int64_t offset);

    /// Returns true if the end of the file has been reached; otherwise returns
    /// false.
    bool eof() const;

    /// Reads a single character from the stream and stores it in c.
    bool getChar(char *c);

    /// Stores the character c back into the stream.
    void ungetChar(char c);

    /// Returns the number of full items actually read, which may be less than
    /// count if an error occurs or if the end of the file is encountered before
    /// reaching count.
    size_t read(void *data, size_t size, size_t count);

    /// Writes up to count items.
    size_t write(const void *data, size_t size, size_t count);

    /// Flushes any buffered data to the file. Returns true if successful;
    /// otherwise returns false.
    bool flush();

    /// Returns the size of the file.
    static int64_t size(const SbString &fileName);

    /// Returns true if the file specified by fileName exists; otherwise returns
    /// false.
    static bool exists(const SbString &fileName);

    /// Returns the base name of the file without the path.
    static SbString baseName(const SbString &filename);

    /// Returns the complete extension of the file.
    static SbString extension(const SbString &filename);

    /// Gets directory part of file name including the trailing @c /.
    static SbString dirName(const SbString &filename);

  private:
    // Disables the use of copy constructors and assignment operators
    SbFile(const SbFile &);
    SbFile &operator=(const SbFile &);

    bool  mOwner; // TRUE if SbFile opened file
    FILE *mFilePointer;
};

