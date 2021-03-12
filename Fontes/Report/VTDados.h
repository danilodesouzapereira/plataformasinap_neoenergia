//---------------------------------------------------------------------------
#ifndef VTDadosH
#define VTDadosH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTDados;

//---------------------------------------------------------------------------
class VTDados : public TObject
   {
   public:
      __property AnsiString Celula[int ind_reg][int ind_col] = {read=PM_GetCelula, write=PM_SetCelula};

   public:
						          __fastcall  VTDados(void) {};
      virtual               __fastcall ~VTDados(void) {};
		virtual  void         __fastcall  Clear(void) = 0;
		virtual  TStringList* __fastcall  InsereStringList(void) = 0;
      virtual TList*        __fastcall  LisStringList(void) = 0;

   protected:
      virtual AnsiString __fastcall PM_GetCelula(int ind_reg, int ind_col) = 0;
      virtual void       __fastcall PM_SetCelula(int ind_reg, int ind_col, AnsiString valor) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto VTExporta
//---------------------------------------------------------------------------
VTDados* __fastcall NewObjDados(void);

//---------------------------------------------------------------------------
#endif
//eof


