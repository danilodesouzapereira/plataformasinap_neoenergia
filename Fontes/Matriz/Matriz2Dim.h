
//---------------------------------------------------------------------------
#ifndef Matriz2DimH
#define Matriz2DimH

//---------------------------------------------------------------------------
#include <Classes.hpp>
using namespace std;
#include <complex>

//---------------------------------------------------------------------------
complex<double> ** __fastcall NewMatriz2DimC(int num1,
                                             int num2);
double          ** __fastcall NewMatriz2DimD(int num1,
                                             int num2);
int             ** __fastcall NewMatriz2DimI(int num1,
                                             int num2);
void               __fastcall DeleteMatriz2DimC(complex<double> ***matriz);
void               __fastcall DeleteMatriz2DimD(double          ***matriz);
void               __fastcall DeleteMatriz2DimI(int             ***matriz);

#endif
//---------------------------------------------------------------------------
//eof

