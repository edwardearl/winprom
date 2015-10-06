// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#include "stdafx.h"
#include "Database.h"

static void read_string(CString& s, FILE *f)
{
  s.Empty();
  char c;
  while (fread(c,f),c!='\0') s+=c;
}

static void write_string(const CString& s, FILE *f)
{
  fwrite((const char *)s,s.GetLength()+1,1,f);
}

const char Field::ft_char[]={'I','R','S','L'};

unsigned Field::size() const
{
  switch (type) {
  case FLD_INTEGER: return int_values.size();
  case FLD_REAL: return real_values.size();
  case FLD_STRING: return string_values.size();
  case FLD_LOC: return loc_values.size();
  }
  assert(0);
  return 0;
}

void Field::resize(unsigned n)
{
  switch (type) {
  case FLD_INTEGER: int_values.resize(n,0); break;
  case FLD_REAL: real_values.resize(n,0.0f); break;
  case FLD_STRING: string_values.resize(n); break;
  case FLD_LOC: loc_values.resize(n); break;
  default: assert(0);
  }
}

unsigned Field::get_width() const
{
  unsigned max_width=name.GetLength(),width,n;
  switch (type) {
  case FLD_REAL:
    if (max_width<6) max_width=6;
    break;
  case FLD_INTEGER:
    {
      for (vector<int>::const_iterator i=int_values.begin();
	   i!=int_values.end(); ++i) {
	for (width=0,n=(*i); n!=0; ++width) n/=10;
	if (width>max_width) max_width=width;
      }
    }
    break;
  case FLD_STRING:
    {
      for (vector<CString>::const_iterator s=string_values.begin();
	   s!=string_values.end(); ++s) {
	width=(*s).GetLength();
	if (width>max_width) max_width=width;
      }
    }
    break;
  case FLD_LOC:
    if (max_width<22) max_width=22;
    break;
  }
  return max_width;
}

bool Field::is_virgin() const
{
  Database::RecordID rec;
  switch (type) {
  case FLD_INTEGER:
    for (rec=0; rec<int_values.size(); ++rec)
      if (int_values[rec]) return false;
    break;
  case FLD_REAL:
    for (rec=0; rec<real_values.size(); ++rec)
      if (real_values[rec]) return false;
    break;
  case FLD_STRING:
    for (rec=0; rec<int_values.size(); ++rec)
      if (!string_values[rec].IsEmpty()) return false;
    break;
  case FLD_LOC:
    for (rec=0; rec<loc_values.size(); ++rec)
      if (loc_values[rec]) return false;
    break;
  default: assert(0);
  }
  return true;
}

void Field::read(FILE *f) throw(file_error)
{
  read_string(name,f);
  fread(type,f);
  unsigned i,n;
  fread(n,f);
  resize(n);
  for (i=0; i<n; ++i) {
    switch (type) {
    case FLD_INTEGER: fread(int_values[i],f); break;
    case FLD_REAL: fread(real_values[i],f); break;
    case FLD_STRING: read_string(string_values[i],f); break;
    case FLD_LOC: fread(loc_values[i],f); break;
    default: throw file_error("Malformed database file");
    }
  }
}

void Field::write(FILE *f) const throw(file_error)
{
  write_string(name,f);
  fwrite(type,f);
  unsigned i,n=size();
  fwrite(n,f);
  for (i=0; i<n; ++i) {
    switch (type) {
    case FLD_INTEGER: fwrite(int_values[i],f); break;
    case FLD_REAL: fwrite(real_values[i],f); break;
    case FLD_STRING: write_string(string_values[i],f); break;
    case FLD_LOC: fwrite(loc_values[i],f); break;
    default: assert(0);
    }
  }
}


Database::FieldID Database::get_field(const char *name)
{
  FieldID fid;
  for (fid=fields.begin(); fid!=fields.end(); ++fid)
    if ((*fid).name==name) return fid;
  return fid;
}

Database::const_FieldID Database::get_field(const char *name) const
{
  const_FieldID fid;
  for (fid=fields.begin(); fid!=fields.end(); ++fid)
    if ((*fid).name==name) return fid;
  return fid;
}

Database::FieldID Database::add_field(const char *name, Field_type type)
{
  fields.push_back(Field(name,type));
  unsigned n=locations.size();
  fields.back().resize(n);
  return fields.end()-1;
}

void Database::swap_field(FieldID fid)
{
  assert(fid!=fields.begin() && fid!=fields.end());
  Field temp=(*fid);
  *fid=*(fid-1);
  *(fid-1)=temp;
}

Database::RecordID Database::get_record(const GridPoint& gp) const
{
  assert(ordered);
  RecordID lo=0,hi=locations.size(),mid;
  while (hi>lo) {
    mid=(hi+lo)/2;
    if (gp<locations[mid]) hi=mid;
    else if (gp>locations[mid]) lo=mid+1;
    else return mid;
  }
  return REC_NONE;
}

Database::RecordID Database::add_record(const GridPoint& gp)
{
  assert(ordered);
  RecordID lo=0,hi=locations.size(),mid;
  while (hi>lo) {
    mid=(hi+lo)/2;
    if (gp<locations[mid]) hi=mid;
    else if (gp>locations[mid]) lo=mid+1;
    else return REC_NONE;
  }
  locations.insert(locations.begin()+lo,gp);
  for (FieldID fid=fields.begin(); fid!=fields.end(); ++fid) {
    switch ((*fid).type) {
    case FLD_INTEGER:
      (*fid).int_values.insert((*fid).int_values.begin()+lo,0);
      break;
    case FLD_REAL:
      (*fid).real_values.insert((*fid).real_values.begin()+lo,0.0);
      break;
    case FLD_STRING:
      (*fid).string_values.insert((*fid).string_values.begin()+lo, "");
      break;
    case FLD_LOC:
      (*fid).loc_values.insert((*fid).loc_values.begin()+lo, GridPoint());
      break;
    default:
      assert(0);
    }
  }
  return lo;
}

void Database::swap(RecordID rec1, RecordID rec2)
{
  GridPoint location;
  int ival;
  float rval;
  CString sval;
  GridPoint lval;
  location=locations[rec1];
  locations[rec1]=locations[rec2];
  locations[rec2]=location;
  for (FieldID fid=fields.begin(); fid!=fields.end(); ++fid) {
    switch ((*fid).get_type()) {
    case FLD_INTEGER:
      ival=(*fid).int_values[rec1];
      (*fid).int_values[rec1]=(*fid).int_values[rec2];
      (*fid).int_values[rec2]=ival;
      break;
    case FLD_REAL:
      rval=(*fid).real_values[rec1];
      (*fid).real_values[rec1]=(*fid).real_values[rec2];
      (*fid).real_values[rec2]=rval;
      break;
    case FLD_STRING:
      sval=(*fid).string_values[rec1];
      (*fid).string_values[rec1]=(*fid).string_values[rec2];
      (*fid).string_values[rec2]=sval;
      break;
    case FLD_LOC:
      lval=(*fid).loc_values[rec1];
      (*fid).loc_values[rec1]=(*fid).loc_values[rec2];
      (*fid).loc_values[rec2]=lval;
      break;
    default: assert(0);
    }
  }
#ifndef NDEBUG
  ordered=false;
#endif
}

void Database::change_location(RecordID rec, const GridPoint& gp)
{
  locations[rec]=gp;
#ifndef NDEBUG
  ordered=false;
#endif
}

void Database::bubble_sort()
{
  bool changed;
  RecordID rid,nrec=locations.size();
  if (nrec<2) return;
  while (true) {
    changed=false;
    for (rid=1; rid<nrec; ++rid)
      if (locations[rid-1]>locations[rid]) {
	swap(rid-1,rid);
	changed=true;
      }
    if (!changed) break;
    changed=false;
    for (rid=nrec-1; rid>0; --rid)
      if (locations[rid-1]>locations[rid]) {
	swap(rid-1,rid);
	changed=true;
      }
    if (!changed) break;
  }
#ifndef NDEBUG
  ordered=true;
#endif
}

void Database::remove_record(RecordID rec)
{
  locations.erase(locations.begin()+rec);
  for (FieldID fid=fields.begin(); fid!=fields.end(); ++fid) {
    switch ((*fid).type) {
    case FLD_INTEGER:
      (*fid).int_values.erase((*fid).int_values.begin()+rec);
      break;
    case FLD_REAL:
      (*fid).real_values.erase((*fid).real_values.begin()+rec);
      break;
    case FLD_STRING:
      (*fid).string_values.erase((*fid).string_values.begin()+rec);
      break;
    case FLD_LOC:
      (*fid).loc_values.erase((*fid).loc_values.begin()+rec);
      break;
    default: assert(0);
    }
  }
}

void Database::set_value(RecordID rec, FieldID fld, int value)
{
  assert((*fld).type==FLD_INTEGER && rec<locations.size());
  (*fld).int_values[rec]=value;
}

void Database::set_value(RecordID rec, FieldID fld, float value)
{
  assert((*fld).type==FLD_REAL && rec<locations.size());
  (*fld).real_values[rec]=value;
}

void Database::set_value(RecordID rec, FieldID fld, const char *value)
{
  assert((*fld).type==FLD_STRING && rec<locations.size());
  (*fld).string_values[rec]=value;
}

void Database::set_value(RecordID rec, FieldID fld, const GridPoint& value)
{
  assert((*fld).type==FLD_LOC && rec<locations.size());
  (*fld).loc_values[rec]=value;
}

void Database::get_value(RecordID rec, const_FieldID fld, int& value) const
{
  assert((*fld).type==FLD_INTEGER && rec<locations.size());
  value=(*fld).int_values[rec];
}

void Database::get_value(RecordID rec, const_FieldID fld, float& value) const
{
  assert((*fld).type==FLD_REAL && rec<locations.size());
  value=(*fld).real_values[rec];
}

void Database::get_value(RecordID rec, const_FieldID fld, const char *&value) const
{
  assert((*fld).type==FLD_STRING && rec<locations.size());
  value=(*fld).string_values[rec];
}

void Database::get_value(RecordID rec, const_FieldID fld, CString& value) const
{
  assert((*fld).type==FLD_STRING && rec<locations.size());
  value=(*fld).string_values[rec];
}

void Database::get_value(RecordID rec, const_FieldID fld, GridPoint& value) const
{
  assert((*fld).type==FLD_LOC && rec<locations.size());
  value=(*fld).loc_values[rec];
}

static const short unsigned VERSION_1=257;

void Database::read(FILE *f) throw(file_error)
{
  unsigned short version;
  fread(version,f);
  switch (version) {
  case VERSION_1:
    GridPoint::read_stamp(f);
    break;
  default:
    if (GridPoint::lat_step==0 || GridPoint::lon_step==0)
      throw file_error("Unstamped database. You must load another element first.");
    fseek(f,- (int) sizeof version,SEEK_CUR); // we don't have a file version stamp
  }

  unsigned i,n,nf;
  fread(n,f); // # of records
  locations.resize(n);
  for (i=0; i<n; ++i)
    fread(locations[i],f);
  fread(nf,f); // # of fields
  fields.resize(nf);
  for (i=0; i<nf; ++i) {
    fields[i].read(f);
    if (fields[i].size()!=n) {
      empty();
      throw file_error("Malformed database file");
    }
  }
}

void Database::write(FILE *f) const throw(file_error)
{
  fwrite(VERSION_1,f);
  GridPoint::write_stamp(f);

  unsigned i,n=locations.size(),nf=fields.size();
  fwrite(n,f); // # of records
  for (i=0; i<n; ++i)
    fwrite(locations[i],f);
  fwrite(nf,f); // # of fields
  for (i=0; i<nf; ++i)
    fields[i].write(f);
}

void Database::read(const char *fn) throw(file_error_name)
{
  FILE *file=fopen(fn,"rb");
  if (!file) throw file_error_name(fn);
  try {read(file);}
  catch (file_error e) {
    fclose(file);
    throw file_error_name(e,fn);
  }
  catch (...) {
    fclose(file);
    throw;
  }
  fclose(file);
}

void Database::write(const char *fn) const throw(file_error_name)
{
  FILE *file=fopen(fn,"wb");
  if (!file) throw file_error_name(fn);
  try {write(file);}
  catch (file_error e) {
    fclose(file);
    throw file_error_name(e,fn);
  }
  catch (...) {
    fclose(file);
    throw;
  }
  fclose(file);
}

bool Database::purge(const bool keep_rec[])
{
  RecordID irec,jrec,nrec=get_nrec();
  for (irec=jrec=0; irec<nrec; ++irec)
    if (keep_rec[irec]) {
      if (jrec<irec) {
	locations[jrec]=locations[irec];
	for (FieldID fi=fields.begin(); fi!=fields.end(); ++fi) {
	  if (!(*fi).int_values.empty())
	    (*fi).int_values[jrec]=(*fi).int_values[irec];
	  if (!(*fi).real_values.empty())
	    (*fi).real_values[jrec]=(*fi).real_values[irec];
	  if (!(*fi).string_values.empty())
	    (*fi).string_values[jrec]=(*fi).string_values[irec];
	  if (!(*fi).loc_values.empty())
	    (*fi).loc_values[jrec]=(*fi).loc_values[irec];
	}
      }
      ++jrec;
    }

  if (jrec<nrec) {
    locations.resize(jrec);
    for (FieldID fi=fields.begin(); fi!=fields.end(); ++fi) {
      if (!(*fi).int_values.empty()) (*fi).int_values.resize(jrec);
      if (!(*fi).real_values.empty()) (*fi).real_values.resize(jrec);
      if (!(*fi).string_values.empty()) (*fi).string_values.resize(jrec);
      if (!(*fi).loc_values.empty()) (*fi).loc_values.resize(jrec);
    }
  }

  return jrec<nrec;
}

const char *Database::merge(const Database& source)
{
  unsigned nfield_src=source.fields.size(),nfield_dst=fields.size();
  unsigned *fld_xfer=new unsigned[nfield_src];
  // find if there are any fields in common
  const_FieldID src_fid;
  FieldID dst_fid;
  unsigned isf,idf;
  for (src_fid=source.fields.begin(),isf=0;
       src_fid!=source.fields.end(); ++src_fid,++isf) {
    // look for destination field with same name as source field
    for (idf=0,dst_fid=fields.begin(); dst_fid!=fields.end(); ++dst_fid,++idf) {
      if ((*src_fid).name==(*dst_fid).name) {
	if ((*src_fid).type!=(*dst_fid).type) {
	  delete[] fld_xfer;
	  return (*dst_fid).name;
	}
	fld_xfer[isf]=idf;
	break;
      }
    }
    if (idf==fields.size()) {
      // no field match.  Will add destination field.
      fld_xfer[isf]=nfield_dst;
      ++nfield_dst;
    }
  }
  // add new source fields to destination database
  nfield_dst=fields.size();
  for (src_fid=source.fields.begin(),isf=0;
       src_fid!=source.fields.end(); ++src_fid,++isf)
    if (fld_xfer[isf]>=nfield_dst)
      add_field((*src_fid).name,(*src_fid).get_type());
  // transfer record data
  RecordID src_rid,dst_rid;
  GridPoint src_loc;
  for (src_rid=0; src_rid<source.locations.size(); ++src_rid) {
    src_loc=source.get_location(src_rid);
    dst_rid=get_record(src_loc);
    if (dst_rid==REC_NONE)
      // source record no found in destination table.  add it.
      dst_rid=add_record(src_loc);
    for (src_fid=source.fields.begin(),isf=0;
	 src_fid!=source.fields.end(); ++src_fid,++isf) {
      switch ((*src_fid).get_type()) {
      case FLD_INTEGER:
	fields[fld_xfer[isf]].int_values[dst_rid]=
	  (*src_fid).int_values[src_rid];
	break;
      case FLD_REAL:
	fields[fld_xfer[isf]].real_values[dst_rid]=
	  (*src_fid).real_values[src_rid];
	break;
      case FLD_STRING:
	fields[fld_xfer[isf]].string_values[dst_rid]=
	  (*src_fid).string_values[src_rid];
	break;
      case FLD_LOC:
	fields[fld_xfer[isf]].loc_values[dst_rid]=
	  (*src_fid).loc_values[src_rid];
	break;
      }
    }
  }
  delete[] fld_xfer;
  return 0;
}


void Field_filter::init_string()
{
  string_ftr_val.Empty();
  type_filter.string_filter.rule=SR_EQ;
  type_filter.string_filter.casens=type_filter.string_filter.isosens=false;
}

void Field_filter::init(Database::const_FieldID fid)
{
  name=(*fid).name;
  type=(*fid).get_type();
  switch (type) {
  case FLD_INTEGER:
    type_filter.int_filter.init();
    break;
  case FLD_REAL:
    type_filter.real_filter.init();
    break;
  case FLD_STRING:
    init_string();
    break;
  case FLD_LOC:
    loc_limits.fill();
    break;
  default:
    assert(0);
  }
}

static void make_ASCII(CString& text)
{
  static const char *ASCII= // translation from ISO Latin-1 letters
    "AAAAAAÆCEEEEIIIIDNOOOOOxOUUUUYPBaaaaaaæceeeeiiiidnooooo/ouuuuypy";
  text.Replace("Æ","AE");
  text.Replace("æ","ae");
  int n=text.GetLength();
  char *buf=text.GetBuffer(n);
  for (unsigned i=0; i<n; ++i) {
    if ((unsigned)buf[i]>='\xC0') buf[i]=ASCII[buf[i]-'\xC0'];
  }
  text.ReleaseBuffer(n);
}

bool Field_filter::test(const Database& db, Database::RecordID rid,
			Database::const_FieldID fid) const
{
  assert(type==(*fid).get_type());
  int int_val;
  float real_val;
  CString string_val,sfv;
  GridPoint loc_val;
  switch ((*fid).get_type()) {
  case FLD_INTEGER:
    db.get_value(rid,fid,int_val);
    return
      (!type_filter.int_filter.have_min || int_val>=type_filter.int_filter.min) &&
      (!type_filter.int_filter.have_max || int_val<=type_filter.int_filter.max);
  case FLD_REAL:
    db.get_value(rid,fid,real_val);
    return
      (!type_filter.real_filter.have_min || real_val>=type_filter.real_filter.min) &&
      (!type_filter.real_filter.have_max || real_val<=type_filter.real_filter.max);
  case FLD_STRING:
    if (string_ftr_val.IsEmpty()) return true;
    db.get_value(rid,fid,string_val);
    sfv=string_ftr_val;
    if (!type_filter.string_filter.casens) {
      sfv.MakeLower();
      string_val.MakeLower();
    }
    if (!type_filter.string_filter.isosens) {
      make_ASCII(sfv);
      make_ASCII(string_val);
    }
    switch (type_filter.string_filter.rule) {
    case SR_EQ: return string_val==sfv;
    case SR_NE: return string_val!=sfv;
    case SR_CONT: return string_val.Find(sfv)>=0;
    case SR_DNC: return string_val.Find(sfv)<0;
    case SR_BEGIN: return sfv==string_val.Left(sfv.GetLength());
    case SR_END: return sfv==string_val.Right(sfv.GetLength());
    }
    break;
  case FLD_LOC:
    db.get_value(rid,fid,loc_val);
    return loc_limits.contains(loc_val);
  }
  assert(0);
  return false;
}

bool Record_filter::is_aligned(const Database& db) const
{
  if (db.fields.size()!=field_filters.size()) return false;
  Database::const_FieldID fid;
  vector<Field_filter>::const_iterator ffi;
  for (fid=db.fields.begin(),ffi=field_filters.begin();
       fid!=db.fields.end(); ++fid,++ffi)
    if ((*ffi).name!=(*fid).name || (*ffi).type!=(*fid).get_type())
      return false;
  return true;
}

void Record_filter::align(const Database& db)
{
  vector<Field_filter> temp_ffs;
  Field_filter temp_ff;
  vector<Field_filter>::iterator ffi;
  for (Database::const_FieldID fid=db.fields.begin();
       fid!=db.fields.end(); ++fid) {
    for (ffi=field_filters.begin(); ffi!=field_filters.end(); ++ffi) {
      if ((*ffi).name==(*fid).name) {
	if ((*ffi).type==(*fid).get_type())
	  temp_ffs.push_back(*ffi);
	else {
	  temp_ff.init(fid);
	  temp_ffs.push_back(temp_ff);
	}
	break;
      }
    }
    if (ffi==field_filters.end()) {
      temp_ff.init(fid);
      temp_ffs.push_back(temp_ff);
    }
  }
  field_filters=temp_ffs;
}

bool Record_filter::get_casens() const
{
  for (unsigned ifld=0; ifld<field_filters.size(); ++ifld) {
    if (field_filters[ifld].type==FLD_STRING)
      return field_filters[ifld].type_filter.string_filter.casens;
  }
  return false;
}

bool Record_filter::get_isosens() const
{
  for (unsigned ifld=0; ifld<field_filters.size(); ++ifld) {
    if (field_filters[ifld].type==FLD_STRING)
      return field_filters[ifld].type_filter.string_filter.isosens;
  }
  return false;
}

void Record_filter::set_sens(bool casens, bool isosens)
{
  for (unsigned ifld=0; ifld<field_filters.size(); ++ifld) {
    if (field_filters[ifld].type==FLD_STRING) {
      field_filters[ifld].type_filter.string_filter.casens=casens;
      field_filters[ifld].type_filter.string_filter.isosens=isosens;
    }
  }
}

bool Record_filter::test(const Database& db, const GridPoint& gp) const
{
  return test(db,db.get_record(gp));
}

bool Record_filter::test(const Database& db, Database::RecordID rid) const
{
  assert(is_aligned(db));
  if (rid==REC_NONE) return accept_norec;
  unsigned ifld=0;
  for (Database::const_FieldID fid=db.fields.begin();
       fid!=db.fields.end(); ++fid,++ifld) {
    if (!field_filters[ifld].test(db,rid,fid)) return false;
  }
  return true;
}

Record_location_filter::Record_location_filter() :
  limits(-coord_infty,coord_infty,-coord_infty,coord_infty),
    nonorph(true),orphan(true)
{}

bool Record_location_filter::test(const Database& db, Database::RecordID rid,
				  bool is_nonorph) const
{
  assert(rid!=REC_NONE);
  if (!nonorph) {
    if (!orphan || is_nonorph) return false;
  }
  else if (!orphan) {
    if (!is_nonorph) return false;
  }
  return limits.contains(db.get_location(rid)) && Record_filter::test(db,rid);
}

bool Record_location_filter::test(const Database& db, const GridPoint& gp,
				  bool is_nonorph) const
{
  return test(db,db.get_record(gp),is_nonorph);
}
