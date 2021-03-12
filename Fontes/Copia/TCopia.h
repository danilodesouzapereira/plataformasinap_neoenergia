//---------------------------------------------------------------------------
#ifndef TCopiaH
#define TCopiaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCopia.h"

//---------------------------------------------------------------------------
class VTLigacao;

//---------------------------------------------------------------------------
class TCopia : public VTCopia
   {
   public:
               __fastcall  TCopia(VTApl *apl);
               __fastcall ~TCopia(void);
      bool     __fastcall  CopiaLisRedeParaRedes(TList *lisREDE, VTRedes *redes);
      bool     __fastcall  CopiaRedes1ParaRedes2(VTRedes *redes1, VTRedes *redes2);
      bool     __fastcall  DuplicaCargas(TList *lisCARGA);
      bool     __fastcall  DuplicaCargas(VTRedes *redes, TList *lisCARGA);
      VTRedes* __fastcall  DuplicaRedes(VTRedes *redes);
      VTRedes* __fastcall  DuplicaUmaRede(VTRedes *redes, VTRede *rede);

   private: //métodos
      bool __fastcall CopiaBarras(TList *lisBARRA);
      bool __fastcall CopiaDominios(TList *lisDOMINIO);
      bool __fastcall CopiaEqbar(TList *lisBARRA);
		bool __fastcall CopiaClusters(TList *lisCLUSTER);
		bool __fastcall CopiaLigacoes(TList *lisREDE);
      bool __fastcall CopiaLigacoes(VTRede *rede_orig, VTRede *rede_dest);
      bool __fastcall CopiaMutuas(TList *lisREDE);
      bool __fastcall CopiaRedes(TList *lisREDE);
      bool __fastcall RedefineBarrasLigacao(VTLigacao *ligacao, VTLigacao *ligacao_clone);
      void __fastcall ReiniciaPonteiroObj(TList *lisREDE);

   private: //dados externos
      VTApl   *apl;

   private: //dados locais
      VTRedes *redes_clone;
      TList   *lisEQP;
   };

#endif
//---------------------------------------------------------------------------
//eof
