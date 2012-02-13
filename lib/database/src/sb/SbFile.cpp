#include <Inventor/SbFile.h>

#include <algorithm>

#if defined(SB_OS_WIN)
#   include <io.h>
#   include <sys/types.h>
#   define SB_FTELL ::_ftelli64
#   define SB_FSEEK  ::_fseeki64
#   define SB_STAT   _stati64
#   define SB_ACCESS ::_access
#   define SB_F_OK 00
#   define SB_R_OK 04
#   define SB_W_OK 02
#else
#   include <unistd.h>
#   define SB_FTELL ftello
#   define SB_FSEEK  fseeko
#   define SB_STAT   stat
#   define SB_ACCESS access
#   define SB_F_OK F_OK
#   define SB_R_OK R_OK
#   define SB_W_OK W_OK
#endif

#include <sys/stat.h>

SbFile::SbFile(const SbString & fn)
: mFileName(fn),
  mFilePointer(NULL)
{

}

SbFile::~SbFile()
{
    close();
}

bool 
SbFile::open( const SbString & mode )
{
    if (isOpen())
        return false;

    mFilePointer = fopen(mFileName.getString(), mode.getString());

    return isOpen();
}

bool 
SbFile::isOpen() const
{
    return (mFilePointer != NULL);
}

void 
SbFile::close()
{
    if (mFilePointer) {
        fclose(mFilePointer);
        mFilePointer = NULL;
    }
}


bool
SbFile::exists() const
{
    return SbFile::exists(mFileName);
}

int64_t 
SbFile::pos() const
{
    return SB_FTELL(mFilePointer);
}

bool 
SbFile::seek(int64_t offset)
{
    return SB_FSEEK(mFilePointer, offset, SEEK_SET) == 0;
}


int64_t
SbFile::size() const
{
#if defined(SB_OS_WIN)
    struct _stati64 st;
#else
    struct stat st;
#endif

    if (SB_STAT(mFileName.getString (), &st) == 0) {
        return st.st_size;
    }

    return -1;
}

size_t 
SbFile::read(char * data, size_t count)
{
    return fread(data, 1, count, mFilePointer);
}

size_t 
SbFile::write(const char * data, size_t count)
{
    return fwrite(data, 1, count, mFilePointer);
}

bool
SbFile::exists( const SbString & fileName )
{
    return (SB_ACCESS(fileName.getString(),SB_F_OK)==0) ? true : false;
}

SbString
SbFile::baseName() const
{
    return SbFile::baseName(mFileName);
}

SbString
SbFile::extension() const
{
    return SbFile::extension(mFileName);
}

SbString
SbFile::baseName(const SbString & string)
{
    int index = std::max( string.rfind("/"), string.rfind("\\"));

    return string.getSubString(index+1);
}

SbString
SbFile::extension(const SbString & string)
{
    int firstDot = string.find(".");
    if (firstDot == -1)
        return "";

    return string.getSubString(firstDot + 1);
}
