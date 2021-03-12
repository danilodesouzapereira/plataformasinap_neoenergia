//---------------------------------------------------------------------------
#ifndef StrPaintH
#define StrPaintH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
struct StrBrush{
               TBrushStyle style;
               TColor      color;
               };
/*
//---------------------------------------------------------------------------
struct StrFont {
               AnsiString name;
               TFontStyle style;
               int        size;
               TColor     color;
               };
*/
//---------------------------------------------------------------------------
struct StrPen  {
               int       width;
               TPenMode  mode;
               TPenStyle style;
               TColor    color;
               };
//---------------------------------------------------------------------------
struct StrPaint{
               int        x, y;
               int        x1, y1, x2, y2;
               int        dx, dy;
               StrPen     pen;
               StrBrush   brush;
               //StrFont    font;
               AnsiString texto;
               TStrings  *lines;
               TFont     *font;
               };

#endif
//---------------------------------------------------------------------------
//eof
