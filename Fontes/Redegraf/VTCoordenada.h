//---------------------------------------------------------------------------
#ifndef VTCoordenadaH
#define VTCoordenadaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
struct TArea      {
						int x1, y1, x2, y2;
                  };
struct TLine      {
                  int x1, y1, x2, y2, x3, y3, xm, ym;
                  };
struct TPolyline  {
                  int count;
                  TPoint point[10];
                  };

//---------------------------------------------------------------------------
class VTCoordenada : public TObject
   {
	public: //property
		__property TArea     *Area     = {read=PD.area,     write=PM_SetArea};
		__property TPoint    *Point    = {read=PD.point,    write=PM_SetPoint};
		__property TLine     *Line     = {read=PD.line,     write=PM_SetLine};
      __property TPolyline *Polyline = {read=PD.polyline, write=PM_SetPolyline};

   public:
              __fastcall VTCoordenada(void) {};
		virtual __fastcall ~VTCoordenada(void) {};

	protected: //métodos acessados via property
		virtual void __fastcall PM_SetArea(TArea *area) = 0;
		virtual void __fastcall PM_SetPoint(TPoint *point) = 0;
		virtual void __fastcall PM_SetLine(TLine *line) = 0;
		virtual void __fastcall PM_SetPolyline(TPolyline *polyline) = 0;

	protected: //dados acessados via property
		struct   {
					TArea     *area;
					TPoint    *point;
					TLine     *line;
					TPolyline *polyline;
					} PD;
	};

//---------------------------------------------------------------------------
//protótipo de função para criar objetos VTCoordenada
//---------------------------------------------------------------------------
VTCoordenada* __fastcall NewObjCoordenada(void);

#endif
//-----------------------------------------------------------------------------
// eof
