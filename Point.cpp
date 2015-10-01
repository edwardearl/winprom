#include <stdafx.h>
#include <string.h>
#include "point.h"


const char *Topo_point::format="loc=%L elev=%E";
const char *Topo_point::field_format="%s";
const char *Topo_point::hdr_string=0;
const char *Topo_point::undef_string=0;

bool zero_prob=true;

static char buffer[1024],buf1[256];

#ifdef io_support

void Topo_point::print(FILE *f) const
{
  sprint(buffer);
  fprintf(f,"%s",buffer);
}

void Topo_point::trace() const
{
  sprint(buffer);
  TRACE("%s",buffer);
}

char *Topo_point::sprint(char *buf) const
{
  if (undef_string && field_format && !location && elev.is_empty())
    return buf+sprintf(buf,field_format,undef_string);
  for (const char *fc=format; *fc; ++fc) {
    if (*fc=='%') {
      switch (*++fc) {
      case 'L': case 'l':
	buf=location.sprint(buf);
	break;
      case 'E': case 'e':
	buf=elev.sprint(buf);
	break;
      case '%':
	*(buf++)='%';
	break;
      default:
	fprintf(stderr,"Topo_point format error! (%c)\n",*fc);
	abort();
      }
    }
    else *(buf++)=*fc;
  }
  *buf='\0';
  return buf;
}

void Topo_point::print_header(FILE *f)
{
  if (field_format && hdr_string) {
    fprintf(f,field_format,hdr_string);
    return;
  }
  for (const char *fc=format; *fc; ++fc) {
    if (*fc=='%') {
      switch (*++fc) {
      case 'L': case 'l':
	GridPoint::print_header(f);
	break;
      case 'E': case 'e':
	Elev_intvl::print_header(f);
	break;
      case '%':
	putc('%',f);
	break;
      default:
	fprintf(stderr,"Topo_point format error! (%c)\n",*fc);
	abort();
      }
    }
    else putc(*fc,f);
  }
}

#endif // def io_support


const char *Feature::format=0;
const char *Feature::field_format=0;
const char *Feature::name_format="%s";
const char *Feature::hdr_string=0;
const char *Feature::name_hdr=0;
const char *Feature::edit_string="edited";
const char *Feature::noedit_string="not edited";
const char *Feature::edit_hdr=0;
const char *Feature::edge_string="edge effect";
const char *Feature::noedge_string="";
const char *Feature::edge_hdr=0;

#ifdef io_support
const Database *Feature::data;
#endif


Feature& Feature::operator=(const Topo_point& tp)
{
  (Topo_point&)(*this)=tp;
  name=0;
  edited=!location;
  edge_effect=false;
  return *this;
}

Feature& Feature::operator=(const char *n)
{
  delete[] name;
  if (n && *n) name=strdup(n);
  else name=0;
  return *this;
}

Feature& Feature::operator=(const Feature& ftr)
{
  if (&ftr!=this) {
    (Topo_point&)(*this)=ftr;
    delete[] name;
    name=strdup(ftr.name);
    edited=ftr.edited;
    edge_effect=ftr.edge_effect;
  }
  return *this;
}

bool Feature::check_limit(float u, float v, bool trans_edit) const
{
  return !location || (!trans_edit && edited) || elev.check_limit(u,v);
}

void Feature::transform(float m, float b, bool trans_edit)
{
  if (trans_edit || !edited) elev.transform(m,b);
}

void Feature::set_range(Elevation new_range)
{
  if (!edited && !elev.is_empty()) {
    Elevation midpt=elev.midpt();
    elev.low=midpt-new_range;
    elev.high=midpt+new_range;
  }
}

void Feature::edit(const Feature& a)
{
  if (a.name!=0 && name==0) name=strdup(a.name);
  if (edited==a.edited) elev|=a.elev;
  else if (a.edited) {
    elev=a.elev;
    edited=true;
  }
}

void Feature::min_range(Elevation& range) const
{
  if (location && !edited && elev.high!=elev_undef && elev.low!=elev_undef) {
    Elevation es=elev.size();
    if (es>=0 && es<range)
      range=es;
  }
}

#define EON '\x01'

void Feature::read(FILE *f) throw(file_error)
{
  fread((Topo_point&)(*this),f);
  fread(edited,f);
  fread(edge_effect,f);
  unsigned i=0;
  while (buffer[i]=getc(f), buffer[i]!=EON && buffer[i]!='\0') {
    if (buffer[i]==EOF) throw file_error();
    ++i;
  }
  delete[] name;
  if (buffer[i]=='\0') {
    name=i>0?strdup(buffer):0;
    return;
  }
  buffer[i]='\0';
  name=i>0?strdup(buffer):0;
  i=0;
  while (buffer[i]=getc(f), buffer[i]!='\0') {
    if (buffer[i]==EOF) throw file_error();
    ++i;
  }
}

void Feature::write(FILE *f) const throw(file_error)
{
  fwrite((Topo_point&)(*this),f);
  fwrite(edited,f);
  fwrite(edge_effect,f);
  if (name)
    if (fwrite(name,strlen(name),1,f)!=1)
      throw file_error();
  fwrite('\0',f);
}

#ifdef io_support

void Feature::print(FILE *f) const
{
  sprint(buffer);
  fprintf(f,"%s",buffer);
}

#endif

void data_field_fmt(const char *&fc, int& iwidth)
{
  iwidth=0;
  for (++fc; *fc>='0' && *fc<='9'; ++fc)
    iwidth=iwidth*10+*fc-'0';
  char del=*fc;
  switch (del) { 
  case '(': del=')'; break;
  case '[': del=']'; break;
  case '{': del='}'; break;
  case '<': del='>'; break;
  }
  const char *fc1;
  for (fc1=++fc; *fc1!=del; ++fc1)
    if (!*fc1) {
      fprintf(stderr,"Feature format error- undelimited data field\n");
      abort();
    }
  memcpy(buf1,fc,fc1-fc);
  buf1[fc1-fc]='\0';
  fc=fc1;
}

#ifdef io_support

char *Feature::sprint(char *buf) const
{
  char loc_buf[32];
  Database::const_FieldID fid;
  Database::RecordID rid;
  const char *svalue;
  int ivalue,iwidth;
  float rvalue;
  GridPoint lvalue;
  if (undef_string && field_format && !location && elev.is_empty())
    return buf+sprintf(buf,field_format,undef_string);
  if (!format) return Topo_point::sprint(buf);
  for (const char *fc=format; *fc; ++fc) {
    if (*fc=='%') {
      switch (*++fc) {
      case 'P': case 'p': case 'T': case 't':
	buf=Topo_point::sprint(buf);
	break;
      case 'A': case 'a':
	++fc;
	if (name_format && name) buf+=sprintf(buf,name_format,name);
	else switch (*fc) {
	case 'L': case 'l':
	  buf=location.sprint(buf);
	  break;
	case 'E': case 'e':
	  buf=elev.sprint(buf);
	  break;
	default:
	  fprintf(stderr,"Feature %A format error! (%c)\n",*fc);
	  abort();
	}
	break;
      case 'L': case 'l':
	buf=location.sprint(buf);
	break;
      case 'E': case 'e':
	buf=elev.sprint(buf);
	break;
      case 'M': case 'm':
	buf+=sprintf(buf,"%s",edited?edit_string:noedit_string);
	break;
      case 'O': case 'o':
	buf+=sprintf(buf,"%s",edge_effect?edge_string:noedge_string);
	break;
      case 'N': case 'n':
	if (name_format) buf+=sprintf(buf,name_format,name?name:"");
	break;
      case 'D': case 'd':
	data_field_fmt(fc,iwidth);
	fid=data->get_field(buf1);
	if (data->valid_field(fid)) {
	  rid=data->get_record(location);
	  switch ((*fid).get_type()) {
	  case FLD_INTEGER:
	    if (rid!=REC_NONE) {
	      data->get_value(rid,fid,ivalue);
	      buf+=sprintf(buf,"%*d",iwidth,ivalue);
	    }
	    else buf+=sprintf(buf,"%*s",iwidth,"");
	    break;
	  case FLD_REAL:
	    if (rid!=REC_NONE) {
	      data->get_value(rid,fid,rvalue);
	      buf+=sprintf(buf,"%*g",iwidth,rvalue);
	    }
	    else buf+=sprintf(buf,"%*s",iwidth,"");
	    break;
	  case FLD_STRING:
	    if (rid!=REC_NONE) data->get_value(rid,fid,svalue);
	    else svalue="";
	    buf+=sprintf(buf,"%-*s",iwidth,svalue);
	    break;
	  case FLD_LOC:
	    if (rid!=REC_NONE) data->get_value(rid,fid,lvalue);
	    lvalue.sprint(loc_buf);
	    buf+=sprintf(buf,"%-*s",iwidth,loc_buf);
	    break;
	  }
	}
	else buf+=sprintf(buf,"########");
	break;
      case '%':
	*(buf++)='%';
	break;
      default:
	fprintf(stderr,"Feature format error! (%c)\n",*fc);
	abort();
      }
    }
    else *(buf++)=*fc;
  }
  *buf='\0';
  return buf;
}

void Feature::print_header(FILE *f)
{
  if (field_format && hdr_string) {
    fprintf(f,field_format,hdr_string);
    return;
  }
  if (!format) {
    Topo_point::print_header(f);
    return;
  }
  Database::const_FieldID fid;
  int iwidth;
  Field_type ft;
  for (const char *fc=format; *fc; ++fc) {
    if (*fc=='%') {
      switch (*++fc) {
      case 'P': case 'p': case 'T': case 't':
	Topo_point::print_header(f);
	break;
      case 'A': case 'a':
	++fc;
	if (name_hdr) fprintf(f,name_format,name_hdr);
	else switch (*fc) {
	case 'L': case 'l':
	  GridPoint::print_header(f);
	  break;
	case 'E': case 'e':
	  Elev_intvl::print_header(f);
	  break;
	default:
	  fprintf(stderr,"Feature %A format error! (%c)\n",*fc);
	  abort();
	}
	break;
      case 'L': case 'l':
	GridPoint::print_header(f);
	break;
      case 'E': case 'e':
	Elev_intvl::print_header(f);
	break;
      case 'M': case 'm':
	if (edit_hdr) fprintf(f,"%s",edit_hdr);
	break;
      case 'O': case 'o':
	if (edge_hdr) fprintf(f,"%s",edge_hdr);
	break;
      case 'N': case 'n':
	if (name_hdr) fprintf(f,name_format,name_hdr);
	break;
      case 'D': case 'd':
	data_field_fmt(fc,iwidth);
	fid=data->get_field(buf1);
	if (data->valid_field(fid)) {
	  ft=(*fid).get_type();
	  fprintf(f,ft==FLD_STRING||ft==FLD_LOC?"%-*s":"%*s",iwidth,buf1);
	}
	else fprintf(f,"########");
	break;
      case '%':
	putc('%',f);
	break;
      default:
	fprintf(stderr,"Feature format error! (%c)\n",*fc);
	abort();
      }
    }
    else putc(*fc,f);
  }
}


/////////////////////////////////////////////////////////////////////////////
// FeatureFilter implementation

FeatureFilter::FeatureFilter(bool ej)
  : elev(-elev_infty,elev_infty),
    strict(false),edge(ej),nonedge(true),edit(true),unedit(true),
    name_filter_type(IS),case_sens(false),isolatin1_sens(false),
    limits(-coord_infty,coord_infty,-coord_infty,coord_infty)
{
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

bool FeatureFilter::test(const Feature& featr, const Database& db) const
{
  if (strict) {
    if (!elev.contains(featr.elev)) return false;
  }
  else {
    if (!(featr.elev && elev)) return false;
  }
  if (featr.edge_effect ? !edge : !nonedge) return false;
  if (featr.edited ? !edit : !unedit) return false;
  if (!name.IsEmpty()) {
    CString featr_name(featr.name),filter_name(name);
    if (!case_sens) {
      featr_name.MakeUpper();
      filter_name.MakeUpper();
    }
    if (!isolatin1_sens) {
      make_ASCII(featr_name);
      make_ASCII(filter_name);
    }
    switch (name_filter_type) {
    case IS:
      if (filter_name!=featr_name) return false;
      break;
    case ISNOT:
      if (filter_name==featr_name) return false;
      break;
    case CONTAINS:
      if (featr_name.Find(filter_name)<0) return false;
      break;
    case CONTAINSNOT:
      if (featr_name.Find(filter_name)>=0) return false;
      break;
    case BEGINS:
      if (filter_name!=featr_name.Left(filter_name.GetLength())) return false;
      break;
    case ENDS:
      if (filter_name!=featr_name.Right(filter_name.GetLength())) return false;
      break;
    }
  }
  if (!limits.contains(featr.location)) return false;

  return rec_filter.test(db,featr.location);
}

bool FeatureTypeFilter::test(FT_index ft) const
{
  switch (ft) {
  case FT_PEAK: return peaks;
  case FT_SADDLE: return saddles;
  case FT_RUNOFF: return runoffs;
  case FT_BASINSADL: return bsnsdls;
  default: assert(0);
  }
  return false;
}


#endif // def io_support
