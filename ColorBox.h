#ifndef COLOR_BOX
#define COLOR_BOX


class CDrawBox : public CStatic {
public:
  virtual BOOL Create(DWORD, const RECT&, CWnd *, UINT);
  afx_msg void OnPaint();
  //virtual void OnDraw(CDC *) {assert(0);}
  virtual void OnDraw(CDC *) = 0;
protected:
  DECLARE_MESSAGE_MAP()
  //DECLARE_DYNCREATE(CDrawBox)
};

class CColorBox : public CDrawBox {
  COLORREF color;
public:
  CColorBox(COLORREF cr=GetSysColor(COLOR_WINDOW)) : color(cr) {}
  virtual void OnDraw(CDC *);
  COLORREF get_color() const {return color;}
  void set_color(COLORREF);
  //DECLARE_DYNCREATE(CColorBox)
};


#endif // ndef COLOR_BOX
