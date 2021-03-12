//---------------------------------------------------------------------------
#ifndef ComplexoH
#define ComplexoH
using namespace std;

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <complex>

//---------------------------------------------------------------------------
//funções de uso geral
double          __fastcall Abs(complex<double> valor, double precisao=1.e-10);
double          __fastcall Arg(complex<double> valor);
double          __fastcall ArgGrau(complex<double> valor);
AnsiString      __fastcall ComplexToStr(AnsiString fmt, complex<double> valor, byte separador='.');
AnsiString      __fastcall ComplexPolarToStr(AnsiString fmt, complex<double> valor, byte separador='.');
complex<double> __fastcall Alfa(void);
complex<double> __fastcall Alfa2(void);
complex<double> __fastcall CZero(void);
bool            __fastcall IsComplexZero(complex<double> valor, double precisao=1.e-8);
complex<double> __fastcall RoundZero(complex<double> valor, double precisao=1.e-8);

#endif
//---------------------------------------------------------------------------
//eof
