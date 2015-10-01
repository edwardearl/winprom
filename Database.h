#ifndef _DATABASE_
#define _DATABASE_

#include <vector>
#include "GridPoint.h"
#include "Rectangle.h"
using namespace std;

#define REC_NONE ~0u


enum Field_type {FLD_VOID,FLD_INTEGER,FLD_REAL,FLD_STRING,FLD_LOC};

class Field {
  Field_type type;
  vector<int> int_values;
  vector<float> real_values;
  vector<CString> string_values;
  vector<GridPoint> loc_values;
  unsigned size() const;
  void resize(unsigned);
  friend class Database;
public:
  static const char ft_char[];
  CString name;
  Field() : type(FLD_VOID) {}
  Field(const char *n, Field_type t) : name(n),type(t) {}
  Field_type get_type() const {return type;}
  unsigned get_width() const;
  const vector<int>& get_ints() const {return int_values;}
  const vector<float>& get_reals() const {return real_values;}
  const vector<CString>& get_strings() const {return string_values;}
  const vector<GridPoint>& get_locs() const {return loc_values;}
  bool is_virgin() const;
  void read(FILE *) throw(file_error);
  void write(FILE *) const throw(file_error);
};


class Database {
#ifndef NDEBUG
  bool ordered;
#endif
public:
  typedef unsigned RecordID;
private:
  friend struct Record_filter;
  friend struct Field_filter;
  vector<GridPoint> locations;
  vector<Field> fields;
  void swap(RecordID, RecordID);
public:
  typedef vector<Field>::iterator FieldID;
  typedef vector<Field>::const_iterator const_FieldID;
#ifndef NDEBUG
  Database() : ordered(true) {}
#endif
  unsigned get_nfield() const {return fields.size();}
  FieldID get_field(const char *);
  const_FieldID get_field(const char *) const;
  bool valid_field(const_FieldID fid) const
    {return fid>=fields.begin() && fid<fields.end();}
  bool valid_field_name(const char *n) const
    {return valid_field(get_field(n));}
  const vector<GridPoint>& get_locations() const {return locations;}
  FieldID begin_field() {return fields.begin();}
  const_FieldID begin_field() const {return fields.begin();}
  FieldID end_field() {return fields.end();}
  const_FieldID end_field() const {return fields.end();}
  FieldID add_field(const char *, Field_type);
  void swap_field(FieldID);
  void remove_field(FieldID fid) {fields.erase(fid);}
  void reserve_nfield(unsigned nf) {fields.reserve(nf);}
  unsigned capacity_nfield() const {return fields.capacity();}
  Field_type field_type(const_FieldID fid) const {return (*fid).type;}
  RecordID get_nrec() const {return locations.size();}
  RecordID get_record(const GridPoint&) const;
  RecordID add_record(const GridPoint&);
  void change_location(RecordID, const GridPoint&);
  void bubble_sort();
  void remove_record(RecordID);
  void set_value(RecordID, FieldID, int);
  void set_value(RecordID, FieldID, float);
  void set_value(RecordID, FieldID, const char *);
  void set_value(RecordID, FieldID, const GridPoint&);
  const GridPoint& get_location(RecordID rid) const {return locations[rid];}
  void get_value(RecordID, const_FieldID, int&) const;
  void get_value(RecordID, const_FieldID, float&) const;
  void get_value(RecordID, const_FieldID, const char *&) const;
  void get_value(RecordID, const_FieldID, CString&) const;
  void get_value(RecordID, const_FieldID, GridPoint&) const;
  bool purge(const bool[]);
  const char *merge(const Database&);
  void read(FILE *);
  void read(const char *) throw(file_error_name);
  void write(FILE *) const;
  void write(const char *) const;
  void empty() {fields.resize(0); locations.resize(0);}
};


struct Field_filter {
  enum String_rule {SR_EQ,SR_NE,SR_CONT,SR_DNC,SR_BEGIN,SR_END};
  CString name;
  Field_type type;
  union {
    struct Int_filter {
      bool have_min,have_max;
      int min,max;
      void init() {have_min=have_max=false;}
    } int_filter;
    struct Real_filter {
      bool have_min,have_max;
      float min,max;
      void init() {have_min=have_max=false;}
    } real_filter;
    struct String_filter {
      bool casens,isosens;
      String_rule rule;
    } string_filter;
  } type_filter;
  void init_string();
  CString string_ftr_val; // cannot be in union
  Rectangl loc_limits; // cannot be in union
  void init(Database::const_FieldID);
  bool test(const Database&, Database::RecordID, Database::const_FieldID) const;
};

struct Record_filter {
  bool accept_norec;
  vector<Field_filter> field_filters;
  Record_filter() : accept_norec(true) {}
  bool is_aligned(const Database&) const;
  void align(const Database&);
  bool get_casens() const;
  bool get_isosens() const;
  void set_sens(bool, bool);
  bool test(const Database&, const GridPoint&) const;
  bool test(const Database&, Database::RecordID) const;
};

struct Record_location_filter : Record_filter {
  Rectangl limits;
  bool nonorph,orphan;
  Record_location_filter();
  bool test(const Database&, const GridPoint&, bool) const;
  bool test(const Database&, Database::RecordID, bool) const;
};

#endif // ndef _DATABASE_
