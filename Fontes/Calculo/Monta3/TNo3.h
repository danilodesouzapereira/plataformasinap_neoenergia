
//---------------------------------------------------------------------------
#ifndef TNo3H
#define TNo3H

//---------------------------------------------------------------------------
#include <Classes.hpp>

class TBarra;

//---------------------------------------------------------------------------
class TNo3: public TObject
   {
   public:
                         __fastcall  TNo3(TBarra *barra,
                                          int    ind_fase);
                         __fastcall ~TNo3(void);

   public: // Dados
      TBarra *barra;
      int    ind_fase;
   };

#endif
//---------------------------------------------------------------------------
//eof
