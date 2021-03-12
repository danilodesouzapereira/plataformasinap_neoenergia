//---------------------------------------------------------------------------
#ifndef VTDiagH
#define VTDiagH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTNet;
class VTRede;

//---------------------------------------------------------------------------
class VTDiag : public TObject
   {
   public:
                   __fastcall  VTDiag(void) {};
      virtual      __fastcall ~VTDiag(void) {};
      virtual void __fastcall  Executa(VTNet *net) = 0;
      virtual void __fastcall  Executa(VTRede *rede) = 0;
      virtual void __fastcall  ReiniciaDiagFluxo(bool diag_flow) = 0;
      virtual void __fastcall  ReiniciaResFluxo(void) = 0;
      virtual void __fastcall  ReiniciaResFluxoBarra(TList *lisBAR) = 0;
      virtual void __fastcall  ReiniciaResFluxoEqbar(TList *lisEQB) = 0;
      virtual void __fastcall  ReiniciaResFluxoLigacao(TList *lisLIG) = 0;
      virtual void __fastcall  ReiniciaResFluxoLigacaoNo(TList *lisNO) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTDiag
//-----------------------------------------------------------------------------
VTDiag* NewObjDiag(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
