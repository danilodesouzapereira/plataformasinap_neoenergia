//---------------------------------------------------------------------------
#ifndef FuncaoH
#define FuncaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <Forms.hpp>
#include "Export.h"
//-----------------------------------------------------------------------------
// HPS - 2019/12/10
// Verifica a plataforma (Win32 ou Win64)
// IMPORTANTE: a estrutura do "if" tem que ser a utilizada abaixo.
//             _WIN32 retorna "true" para plataforma x64.
#if _WIN64
   // Nada a fazer
#elif _WIN32
   #define __PRETTY_FUNCTION__ __FUNCTION__
#endif
//-----------------------------------------------------------------------------
class VTEqpto;
class VTRede;
//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void       __fastcall Alerta(AnsiString txt);
EXPORT void       __fastcall AlertaMalha(VTRede *rede);
EXPORT AnsiString __fastcall ASCIIFilter(AnsiString text);
EXPORT AnsiString __fastcall ASCIIFilterWithAccents(AnsiString text);
EXPORT void       __fastcall Aviso(AnsiString txt);
EXPORT AnsiString __fastcall ComplexToStr(AnsiString fmt, double real, double imag, byte separador='.');
EXPORT AnsiString __fastcall ComplexPolarToStr(AnsiString fmt, double modulo, double fase, byte separador='.');
EXPORT int        __fastcall Confirma(AnsiString txt1, AnsiString txt2, int buttons=-1);
EXPORT void       __fastcall CopiaTList(TList *lisORI, TList *lisDES);
EXPORT AnsiString __fastcall DataToDoubleToStr(TDate date);
EXPORT void		  __fastcall DestroiTObject(TObject *object);
EXPORT int        __fastcall Distancia_cm(int x1, int y1, int x2, int y2);
EXPORT int        __fastcall Distancia_m(int x1, int y1, int x2, int y2);
EXPORT AnsiString __fastcall DoubleToStr(AnsiString fmt, double dval, byte separador='.');
EXPORT void       __fastcall Erro(AnsiString txt);
EXPORT TForm*     __fastcall ExisteForm(AnsiString class_name, TComponent *Owner);
EXPORT int        __fastcall ExtraiStrings(AnsiString txt, AnsiString  separadores, TStrings *campos);
EXPORT int        __fastcall FalsoId(void);
EXPORT AnsiString __fastcall FormataData(TDateTime dtt);
EXPORT AnsiString __fastcall FormataDataHora(TDateTime dtt);
EXPORT AnsiString __fastcall FormataHora(TDateTime dtt);
EXPORT TObject*	  __fastcall GetEqptoObj(VTEqpto *eqpto, AnsiString obj_class_name);
EXPORT bool       __fastcall IsComplexZero(double real, double imag, double precisao=1e-5);
EXPORT bool       __fastcall IsDoubleZero(double valor, double precisao=1e-5);
EXPORT void       __fastcall LimpaTList(TList *lisOBJ);
EXPORT bool       __fastcall Lista1ContemLista2(TList *lisOBJ1, TList *lisOBJ2);
EXPORT bool       __fastcall Lista1IgualLista2(TList *lisOBJ1, TList *lisOBJ2);
EXPORT int        __fastcall Lista1IntersecaoLista2(TList *lisOBJ1, TList *lisOBJ2);
EXPORT bool       __fastcall RetaContemPonto(int x1, int y1, int x2, int y2, int x, int y);
EXPORT AnsiString __fastcall RemoveCaracterInvalidoFilename(AnsiString filename);
EXPORT void       __fastcall RetiraLis1DeLis2(TList *lisOBJ1, TList *lisOBJ2);
EXPORT int        __fastcall StrToInteger(AnsiString text);
EXPORT bool       __fastcall StrToInteger(AnsiString text, int &valor);
EXPORT double     __fastcall StrToDouble(AnsiString text);
EXPORT bool       __fastcall StrToDouble(AnsiString text, double &valor);
EXPORT TDate      __fastcall StrToDoubleToData(AnsiString data);
EXPORT int        __fastcall TempoDecorridoMS(TDateTime dttIni, TDateTime dttFim = Now());

//---------------------------------------------------------------------------
#endif
//eof
