//---------------------------------------------------------------------------
#ifndef TMontaH
#define TMontaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMonta.h"

//---------------------------------------------------------------------------
class VTMRede;
class VTReducao;

//---------------------------------------------------------------------------
class TMonta : public VTMonta
   {
   public:
             __fastcall TMonta(VTApl *apl);
             __fastcall ~TMonta(void);
      VTNet* __fastcall Executa(TList *lisBAR, TList *lisEQB,
                                TList *lisLIG, TList *lisMUTUA);
      VTNet* __fastcall Executa(VTMNet *mnet);
      VTNet* __fastcall Executa(VTRede *rede);
      VTNet* __fastcall Executa(VTRedes *redes);

   private: //métodos
      void __fastcall InsereEqptoIsolado(VTNet *net, VTRedes *redes);
      void __fastcall InsereEqptoMNet(VTNet *net, VTMNet *mnet);
      void __fastcall InsereEqptoMRede(VTNet *net, VTMRede *mrede);
      void __fastcall InsereEqptoReducao(VTNet *net, VTReducao *reducao);

   private: //dados externos
      VTApl  *apl;

   private: //dados locais
      TList *lisREDU;
      TList *lisTMP;
   };

//---------------------------------------------------------------------------
#endif
//eof

