//---------------------------------------------------------------------------
#ifndef TMontaCCH
#define TMontaCCH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMontaCC.h"

//---------------------------------------------------------------------------
class VTNet;
class VTBarra;
class VTRede;

//---------------------------------------------------------------------------
class TMontaCC : public VTMontaCC
   {
   public:
             __fastcall TMontaCC(VTApl *apl);
             __fastcall ~TMontaCC(void);
      VTNet* __fastcall Executa(VTRedeCC *redeCC);

   private:
      //VTNet*  __fastcall Executa(VTRedeCC *redeCC);
      //void    __fastcall MontaLisMNetDefeito(VTRede *rede_def, TList *lisDEST);
      void    __fastcall OrdenaBarraLigacao(VTRedeCC *redeCC, VTBarra *bar_ref);
      //VTRede* __fastcall RedeDaBarra(VTBarra *barra);

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      TList *lisEQP;
      TList *lisBAR, *lisLIG;
      //TList *lisMNET;
   };

//---------------------------------------------------------------------------
#endif
//eof

