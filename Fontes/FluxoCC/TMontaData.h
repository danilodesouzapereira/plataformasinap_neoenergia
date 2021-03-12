//---------------------------------------------------------------------------
#ifndef TMontaDataH
#define TMontaDataH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\VTMontaData.h"

//---------------------------------------------------------------------------
class VTDemanda;
class VTFornecimento;

//---------------------------------------------------------------------------
class TMontaData : public VTMontaData
   {
   public:
           __fastcall TMontaData(VTApl *apl);
           __fastcall ~TMontaData(void);
      bool __fastcall Inicia(VTNet *net, VTData *data, int ind_pat, double sbase);
      bool __fastcall InsereBarra(void);
      bool __fastcall InsereCapacitor(void);
      bool __fastcall InsereCapserie(void);
      bool __fastcall InsereCarga(void);
      bool __fastcall InsereChave(void);
      bool __fastcall InsereEqbar(int tipo);
      bool __fastcall InsereFiltro(void);
      bool __fastcall InsereGerador(void);
      bool __fastcall InsereLigacao(int tipo);
      bool __fastcall InsereMutua(void);
      bool __fastcall InsereReator(void);
      bool __fastcall InsereRegulador(void);
      bool __fastcall InsereSuprimento(void);
      bool __fastcall InsereTrafo2E(void);
      bool __fastcall InsereTrafo3E(void);
      bool __fastcall InsereTrafoZZ(void);
      bool __fastcall InsereTrecho(void);
      bool __fastcall InsereYref(void);
      bool __fastcall SalvaResBarra(void);
      bool __fastcall SalvaResGerador(void);
      bool __fastcall SalvaResSuprimento(void);
      bool __fastcall SalvaResTrafo2E(void);
      bool __fastcall SalvaResTrecho(void);
      int  __fastcall SentidoFluxo(double fluxo);

   private: //objetos externos
      VTData *data;
      VTNet  *net;

   private: //dados locais
      int            ind_pat;
      double         Sbase;      //MVA
      VTDemanda      *demanda;
      VTFornecimento *fornecimento;
      TList          *lisEQP;
   };

#endif
//---------------------------------------------------------------------------
//eof
