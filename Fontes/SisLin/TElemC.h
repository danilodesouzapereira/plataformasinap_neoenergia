
//---------------------------------------------------------------------------
#ifndef TElemCH
#define TElemCH

//---------------------------------------------------------------------------
#include <Classes.hpp>
using namespace std;
#include <complex>

//---------------------------------------------------------------------------
class TElemC : public TObject
   {
   public:
                         __fastcall  TElemC(int             coluna,
                                            complex<double> valor);
                         __fastcall ~TElemC(void);

   public: // Dados
      int             coluna;  // Coluna do elemento
      complex<double> valor;   // Valor  do elemento
   };

#endif
//---------------------------------------------------------------------------
//eof
