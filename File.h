#ifndef _FILE_
#define _FILE_

/***********************************************************************
  These two template functions are convenience versions of the standard
  C library functions of the same names.  They eliminate the need to
  supply the size of the data type to be transferred.  For writing, the
  requirement that the data to be written be stored in an lvalue is
  eliminated; thus, a constant or temporary can be supplied.
 ***********************************************************************/

#include <exception>
#include <string.h>

class file_error : public std::exception {
  const char *text;
public:
  file_error() : text(strerror(errno)) {}
  file_error(const char *t) : text(t) {}
  virtual const char *what() const {return text;}
};

class file_error_name : public file_error {
public:
  const char *file_name;
  file_error_name(const char *fn) : file_name(fn) {}
  file_error_name(const char *text, const char *fn) :
    file_error(text),file_name(fn) {}
  file_error_name(const file_error& e, const char *fn) :
    file_error(e),file_name(fn) {}
};

template <class T>
inline void fread(T& data, FILE *stream) throw(file_error)
{
  if (fread(&data,sizeof(T),1,stream)!=1)
    throw file_error();
}

template <class T>
inline void fwrite(const T& data, FILE *stream) throw(file_error)
{
  if (fwrite(&data,sizeof(T),1,stream)!=1)
    throw file_error();
}

#endif // ndef _FILE_
