#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <DLL_Inc\Funcao.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall Alerta(AnsiString txt)
   {
   }
// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14569628
EXPORT void __fastcall AlertaMalha(VTRede *rede)
{
}
//-----------------------------------------------------------------------------
EXPORT AnsiString __fastcall ASCIIFilter(AnsiString text)
   {
   return(AnsiString(""));
   }
//-----------------------------------------------------------------------------
EXPORT AnsiString __fastcall ASCIIFilterWithAccents(AnsiString text)
   {
   return(AnsiString(""));
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall Aviso(AnsiString txt)
   {
   }

//-----------------------------------------------------------------------------
EXPORT AnsiString __fastcall ComplexToStr(AnsiString fmt, double real, double imag, byte separador)
   {
   return(AnsiString(""));
   }

//-----------------------------------------------------------------------------
EXPORT AnsiString __fastcall ComplexPolarToStr(AnsiString fmt, double modulo, double fase, byte separador)
   {
   return(AnsiString(""));
   }

//-----------------------------------------------------------------------------
EXPORT int __fastcall Confirma(AnsiString txt1, AnsiString txt2, int buttons)
   {
   return(IDNO);
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall CopiaTList(TList *lisORI, TList *lisDES)
   {
   }

//-----------------------------------------------------------------------------
EXPORT AnsiString __fastcall DataToDoubleToStr(TDate date)
   {
   return(AnsiString(""));
   }

 //---------------------------------------------------------------------------
EXPORT void __fastcall DestroiTObject(TObject *object)
   {
   }

//-----------------------------------------------------------------------------
EXPORT int __fastcall Distancia_cm(int x1, int y1, int x2, int y2)
   {
   return(0);
   }

//-----------------------------------------------------------------------------
EXPORT int __fastcall Distancia_m(int x1, int y1, int x2, int y2)
   {
   return(0);
   }

//-----------------------------------------------------------------------------
EXPORT AnsiString __fastcall DoubleToStr(AnsiString fmt, double dval, byte separador)
   {
   return(AnsiString(""));
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall Erro(AnsiString txt)
   {
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall ExisteForm(AnsiString class_name, TComponent *Owner)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT int __fastcall ExtraiStrings(AnsiString txt, AnsiString  separadores, TStrings *campos)
	{
	return(0);
	}

//-----------------------------------------------------------------------------
EXPORT int __fastcall FalsoId(void)
   {
   return(0);
   }

//-----------------------------------------------------------------------------
EXPORT AnsiString __fastcall FormataData(TDateTime dtt)
   {
   return("");
   }

//-----------------------------------------------------------------------------
EXPORT AnsiString __fastcall FormataDataHora(TDateTime dtt)
   {
   return("");
   }

//-----------------------------------------------------------------------------
EXPORT AnsiString __fastcall FormataHora(TDateTime dtt)
   {
   return("");
   }

//-----------------------------------------------------------------------------
EXPORT TObject*	  __fastcall GetEqptoObj(VTEqpto *eqpto, AnsiString obj_class_name)
{
    return (NULL);
}
//-----------------------------------------------------------------------------
EXPORT bool __fastcall IsComplexZero(double real, double imag, double precisao)
   {
   return(true);
   }

//-----------------------------------------------------------------------------
EXPORT bool __fastcall IsDoubleZero(double valor, double precisao)
   {
   return(true);
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall LimpaTList(TList *lisOBJ)
   {
   }

//-----------------------------------------------------------------------------
EXPORT bool __fastcall Lista1ContemLista2(TList *lisOBJ1, TList *lisOBJ2)
   {
   return(false);
   }

//-----------------------------------------------------------------------------
EXPORT bool  __fastcall Lista1IgualLista2(TList *lisOBJ1, TList *lisOBJ2)
   {
   return(false);
	}

//-----------------------------------------------------------------------------
EXPORT int __fastcall Lista1IntersecaoLista2(TList *lisOBJ1, TList *lisOBJ2)
   {
   return(0);
   }

//-----------------------------------------------------------------------------
EXPORT AnsiString __fastcall RemoveCaracterInvalidoFilename(AnsiString filename)
   {
   return("");
   }
//-----------------------------------------------------------------------------
EXPORT bool __fastcall RetaContemPonto(int x1, int y1, int x2, int y2, int x, int y)
   {
   return(false);
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall RetiraLis1DeLis2(TList *lisOBJ1, TList *lisOBJ2)
   {
   }

//-----------------------------------------------------------------------------
EXPORT int __fastcall StrToInteger(AnsiString text)
   {
   return(0);
	}

//-----------------------------------------------------------------------------
EXPORT bool __fastcall StrToInteger(AnsiString text, int &valor)
	{
	return(false);
	}

//-----------------------------------------------------------------------------
EXPORT double __fastcall StrToDouble(AnsiString text)
   {
   return(0);
   }

//-----------------------------------------------------------------------------
EXPORT bool __fastcall StrToDouble(AnsiString text, double &valor)
	{
	return(false);
	}

//-----------------------------------------------------------------------------
EXPORT TDate __fastcall StrToDoubleToData(AnsiString data)
   {
   return(TDate(0.));
   }

//-----------------------------------------------------------------------------
EXPORT int __fastcall TempoDecorridoMS(TDateTime dttIni, TDateTime dttFim)
   {
   return(0);
   }

//---------------------------------------------------------------------------
//eof


