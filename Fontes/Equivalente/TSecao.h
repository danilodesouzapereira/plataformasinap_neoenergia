//---------------------------------------------------------------------------
#ifndef TSecaoH
#define TSecaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBarra;
class VTLigacao;

//---------------------------------------------------------------------------
class TSecao : public TObject
   {
   public:  //métodos
             __fastcall  TSecao(void);
             __fastcall ~TSecao(void);
      bool   __fastcall  InsereBarra(VTBarra *barra);
      bool   __fastcall  InsereLigacao(VTLigacao *ligacao);
      TList* __fastcall  LisBarra(void);
      TList* __fastcall  LisLigacao(void);

   private:  //métodos

   private:  //objetos externos

   private: //dados locais
      TList    *lisBAR;
      TList    *lisLIG;
   };

//---------------------------------------------------------------------------
TSecao* __fastcall NewObjSecao(void);

#endif
//---------------------------------------------------------------------------
//eof
