//---------------------------------------------------------------------------
#ifndef VTImpressaoH
#define VTImpressaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

//---------------------------------------------------------------------------
class VTImpressao : public TObject
   {
   public:  // Métodos
                         __fastcall  VTImpressao() {};
      virtual            __fastcall ~VTImpressao(void) {};
      virtual bool       __fastcall  ImprimeResumoProc(int arq_alt) = 0;
      virtual bool       __fastcall  ImprimeVetorComplex(AnsiString      nome,
                                                         int             count,
                                                         complex<double> *vetor) = 0;
      virtual bool       __fastcall  ImprimeVetorDouble(AnsiString nome,
                                                        int        count,
                                                        double     *vetor) = 0;
      virtual bool       __fastcall  ImprimeVetorInt(AnsiString nome,
                                                     int        count,
                                                     int        *vetor) = 0;
      virtual AnsiString __fastcall  MontaString(int num1,
                                                 int num_max1,
                                                 int num2,
                                                 int num_max2) = 0;
   };

#endif
//---------------------------------------------------------------------------

