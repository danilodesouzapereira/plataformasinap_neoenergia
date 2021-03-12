//---------------------------------------------------------------------------
#ifndef VTVerifSeH
#define VTVerifSeH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTRede;
class VTVerifCjto;

//---------------------------------------------------------------------------
class VTVerifSe : public TObject
   {
	public:
		__property AnsiString   Codigo      = {read=PM_GetCodigo,      write=PM_SetCodigo};
		__property AnsiString   CodigoCjto  = {read=PM_GetCodigoCjto,  write=PM_SetCodigoCjto};
		__property VTVerifCjto* Conjunto    = {read=PM_GetConjunto,    write=PM_SetConjunto};
		__property double       DecAjust    = {read=PM_GetDecAjust,    write=PM_SetDecAjust};
		__property double       DecVerif    = {read=PM_GetDecVerif,    write=PM_SetDecVerif};
		__property double       FecAjust    = {read=PM_GetFecAjust,    write=PM_SetFecAjust};
		__property double       FecVerif    = {read=PM_GetFecVerif,    write=PM_SetFecVerif};
//		__property VTRede*      Rede        = {read=PM_GetRede,        write=PM_SetRede};
		__property bool         Selecionada = {read=PM_GetSelecionada, write=PM_SetSelecionada};

	public:
									__fastcall VTVerifSe(void) {};
		virtual              __fastcall ~VTVerifSe(void) {};

	public:  //métodos acessados via property
		virtual AnsiString   __fastcall PM_GetCodigo(void) = 0;
		virtual AnsiString   __fastcall PM_GetCodigoCjto(void) = 0;
		virtual VTVerifCjto* __fastcall PM_GetConjunto(void) = 0;
		virtual double       __fastcall PM_GetDecAjust(void) = 0;
		virtual double       __fastcall PM_GetDecVerif(void) = 0;
		virtual double       __fastcall PM_GetFecAjust(void) = 0;
		virtual double       __fastcall PM_GetFecVerif(void) = 0;
//		virtual VTRede*      __fastcall PM_GetRede(void) = 0;
		virtual bool         __fastcall PM_GetSelecionada(void) = 0;
		virtual void         __fastcall PM_SetCodigo(AnsiString codigo) = 0;
		virtual void         __fastcall PM_SetCodigoCjto(AnsiString codigo) = 0;
		virtual void         __fastcall PM_SetConjunto(VTVerifCjto* cjto) = 0;
		virtual void         __fastcall PM_SetDecAjust(double decAjust) = 0;
		virtual void         __fastcall PM_SetDecVerif(double decVerif) = 0;
		virtual void         __fastcall PM_SetFecAjust(double fecAjust) = 0;
		virtual void         __fastcall PM_SetFecVerif(double fecVerif) = 0;
//		virtual void         __fastcall PM_SetRede(VTRede* rede) = 0;
		virtual void         __fastcall PM_SetSelecionada(bool selecionada) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTVerifSe* __fastcall NewObjVerifSe(void);

#endif
//---------------------------------------------------------------------------
//eof
