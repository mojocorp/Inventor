#include <Inventor/SbFile.h>

#include <algorithm>

#if defined(SB_OS_WIN)
#include <io.h>
#include <sys/types.h>
#define SB_FTELL ::_ftelli64
#define SB_FSEEK ::_fseeki64
#else
#include <unistd.h>
#define SB_FTELL ftello
#define SB_FSEEK fseeko
#endif

#include <sys/stat.h>

SbFile::SbFile() : mOwner(false), mFilePointer(NULL) {}

SbFile::SbFile(FILE *fp) : mOwner(false), mFilePointer(NULL) {
    setFilePointer(fp);
}

SbFile::~SbFile() { close(); }

bool
SbFile::open(const SbString &fileName, const SbString &mode) {
    close();

    mOwner = true;

#ifdef SB_OS_WIN
    mFilePointer =
        _wfopen(fileName.toStdWString().data(), mode.toStdWString().data());
#else
    mFilePointer = fopen(fileName.getString(), mode.getString());
#endif
    return isOpen();
}

bool
SbFile::isOpen() const {
    return (mFilePointer != NULL);
}

void
SbFile::close() {
    if (mFilePointer) {
        if (mOwner)
            fclose(mFilePointer);
        mFilePointer = NULL;
    }
}

void
SbFile::setFilePointer(FILE *fp) {
    close();

    mFilePointer = fp;
    mOwner = false;
}

FILE *
SbFile::getFilePointer() const {
    return mFilePointer;
}

int64_t
SbFile::pos() const {
    if (!isOpen())
        return 0;

    return SB_FTELL(mFilePointer);
}

bool
SbFile::seek(int64_t offset) {
    if (!isOpen())
        return false;

    return SB_FSEEK(mFilePointer, offset, SEEK_SET) == 0;
}

bool
SbFile::eof() const {
    if (!isOpen())
        return true;

    return feof(mFilePointer) != 0 ? true : false;
}

bool
SbFile::getChar(char *c) {
    int i = getc(mFilePointer);

    if (c)
        *c = (char)i;

    return (i != EOF);
}

void
SbFile::ungetChar(char c) {
    ungetc(c, mFilePointer);
}

size_t
SbFile::read(void *data, size_t size, size_t count) {
    return fread(data, size, count, mFilePointer);
}

size_t
SbFile::write(const void *data, size_t size, size_t count) {
    return fwrite(data, size, count, mFilePointer);
}

bool
SbFile::flush() {
    if (!isOpen())
        return false;

    return (fflush(mFilePointer) == 0);
}

int64_t
SbFile::size(const SbString &fileName) {
#if defined(SB_OS_WIN)
    struct _stati64 st;
    if (_wstati64(fileName.toStdWString().data(), &st) == 0) {
        return st.st_size;
    }
#else
    struct stat st;
    if (stat(fileName.getString(), &st) == 0) {
        return st.st_size;
    }
#endif

    return -1;
}

bool
SbFile::exists(const SbString &fileName) {
#ifdef SB_OS_WIN
    return (::_waccess(fileName.toStdWString().data(), 00) == 0) ? true : false;
#else
    return (access(fileName.getString(), F_OK) == 0) ? true : false;
#endif
}

SbString
SbFile::baseName(const SbString &filename) {
    int index = std::max(filename.rfind("/"), filename.rfind("\\"));

    return filename.getSubString(index + 1);
}

SbString
SbFile::extension(const SbString &filename) {
    int firstDot = filename.find(".", (int)baseName(filename).getLength());
    if (firstDot == -1)
        return "";

    return filename.getSubString(firstDot + 1);
}

SbString
SbFile::dirName(const SbString &filename) {
    int index = std::max(filename.rfind("/"), filename.rfind("\\"));

    return filename.getSubString(0, index);
}
