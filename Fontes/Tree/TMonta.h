//---------------------------------------------------------------------------
#ifndef TMontaH
#define TMontaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMonta.h"

//---------------------------------------------------------------------------
class VTReducao;

//---------------------------------------------------------------------------
class TMonta : public VTMonta
   {
   public:
             __fastcall TMonta(VTApl *apl);
             __fastcall ~TMonta(void);
      VTNet* __fastcall Net(VTRede *rede);
      VTNet* __fastcall Net(TList *lisREDE);
      VTNet* __fastcall NetParaReducao(VTRede *rede, VTCorte *corte);
      VTNet* __fastcall Net(TList *lisREDE, TList *lisREDU);

   private: //métodos
      void   __fastcall FiltraReducaoComSuprimento(TList *lisREDU);
      void   __fastcall InsereEqptoRede(VTNet *net, VTRede *rede);
      void   __fastcall InsereEqptoReducao(VTNet *net, VTReducao *reducao);
      void   __fastcall MontaLisRedeParaReducao(VTRede *rede, VTCorte *corte);
      void   __fastcall MontaLisReducaoParaReducao(TList *lisREDE, VTCorte *corte);
      bool   __fastcall TodasRedesSimples(TList *lisREDE);

   private: //dados externos
      VTApl  *apl;

   private: //dados locais
      TList *lisCORTE;
      TList *lisREDU;
      TList *lisRR;
      TList *lisTMP;
   };

//---------------------------------------------------------------------------
#endif
//eof

