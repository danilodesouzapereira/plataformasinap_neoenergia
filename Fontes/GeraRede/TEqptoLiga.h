//---------------------------------------------------------------------------

#ifndef TEqptoLigaH
#define TEqptoLigaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTLigacao;
class VTEqpto;
//---------------------------------------------------------------------------
class TEqptoLiga : public TObject
   {
   public:
             __fastcall TEqptoLiga(VTEqpto* eqpto);
             __fastcall ~TEqptoLiga(void);
        bool __fastcall AddLiga(VTLigacao* pliga, int nl);

   private: //métodos

   private: //dados externos

   private: //dados locais
   public:
      VTEqpto* eqpto;
      TList* lisLiga;
      int    nlig;
   };

#endif
//---------------------------------------------------------------------------
//eof