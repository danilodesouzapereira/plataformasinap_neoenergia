//---------------------------------------------------------------------------
#ifndef VTVerifCjtoH
#define VTVerifCjtoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTVerifCjto : public TObject
   {
	public:
		__property AnsiString Codigo     = {read=PM_GetCodigo,   write=PM_SetCodigo};
		__property double     DecAjust   = {read=PM_GetDecAjust, write=PM_SetDecAjust};
		__property double     DecVerif   = {read=PM_GetDecVerif, write=PM_SetDecVerif};
		__property double     FecAjust   = {read=PM_GetFecAjust, write=PM_SetFecAjust};
		__property double     FecVerif   = {read=PM_GetFecVerif, write=PM_SetFecVerif};
		__property TList*     LisVerifSe = {read=PM_GetLisVerifSe};

   public:
								 __fastcall VTVerifCjto(void) {};
		virtual            __fastcall ~VTVerifCjto(void) {};

	public:  //métodos acessados via property
		virtual AnsiString __fastcall PM_GetCodigo(void) = 0;
		virtual double     __fastcall PM_GetDecAjust(void) = 0;
		virtual double     __fastcall PM_GetDecVerif(void) = 0;
		virtual double     __fastcall PM_GetFecAjust(void) = 0;
		virtual double     __fastcall PM_GetFecVerif(void) = 0;
		virtual TList*     __fastcall PM_GetLisVerifSe(void) = 0;
		virtual void       __fastcall PM_SetCodigo(AnsiString codigo) = 0;
		virtual void       __fastcall PM_SetDecAjust(double decAjust) = 0;
		virtual void       __fastcall PM_SetDecVerif(double decVerif) = 0;
		virtual void       __fastcall PM_SetFecAjust(double fecAjust) = 0;
		virtual void       __fastcall PM_SetFecVerif(double fecVerif) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTVerifCjto* __fastcall NewObjVerifCjto(void);

#endif
//---------------------------------------------------------------------------
//eof
