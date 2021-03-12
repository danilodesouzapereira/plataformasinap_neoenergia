//---------------------------------------------------------------------------
#ifndef TReduzH
#define TReduzH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTReduz.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTCarga;
class VTLigacao;
class VTNet;
class VTNo;

//---------------------------------------------------------------------------
class TReduz : public VTReduz
   {
   public:
           __fastcall TReduz(VTApl *apl);
           __fastcall ~TReduz(void);
      bool __fastcall ReiniciaReducaoCarga(VTMNet *mnet);
      bool __fastcall ReiniciaReducaoSuprimento(VTMNet *mnet);
      bool __fastcall ReiniciaReducaoSuprimento1pu(VTMNet *mnet);

   private: //métodos
      int  __fastcall LigacaoConectaNo(VTLigacao* ligacao, VTNo *no);
      void __fastcall LisCargaDeReducaoMNet(VTMNet *mnet, VTBarra *barra, TList *lisCAR);
      void __fastcall LisLigacaoMNet(VTMNet *mnet, VTBarra *barra, TList *lisLIG);
      void __fastcall SomaDemandaCarga(int np, TList *lisCAR, double *p, double *q);
      void __fastcall SomaFluxoLigacao(int np, TList *lisLIG, double *p, double *q, VTNo *no);

   private: //dados externos
      VTApl  *apl;
      VTMNet *mnet;
      VTNet  *net;

   private: //dados locais
      TList *lisCAR;
      TList *lisEQP;
      TList *lisLIG;
   };

#endif
//---------------------------------------------------------------------------
//eof

