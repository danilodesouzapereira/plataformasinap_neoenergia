//---------------------------------------------------------------------------
#ifndef TDiagH
#define TDiagH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <map>
#include "VTDiag.h"

//---------------------------------------------------------------------------
class VTConsulta;
class VTGeral;
class VTLigacao;
class VTPatamares;
class VTBarra;

//---------------------------------------------------------------------------
class TDiag : public VTDiag
   {
   public:
           __fastcall  TDiag(VTApl *apl);
           __fastcall ~TDiag(void);
      void __fastcall  Executa(VTNet *net);
      void __fastcall  Executa(VTRede *rede);
      void __fastcall  ReiniciaResFluxo(void);
      void __fastcall  ReiniciaDiagFluxo(bool diag_flow);
      void __fastcall  ReiniciaResFluxoBarra(TList *lisBAR);
      void __fastcall  ReiniciaResFluxoEqbar(TList *lisEQB);
      void __fastcall  ReiniciaResFluxoLigacao(TList *lisLIG);
      void __fastcall  ReiniciaResFluxoLigacaoNo(TList *lisNO);

   private: //métodos
	  void __fastcall DiagBarra(TList *lisBAR);
      void __fastcall DiagCapserie(TList *lisLIG);
      void __fastcall DiagChave(TList *lisLIG);
      void __fastcall DiagGerador(TList *lisEQB);
      void __fastcall DiagRegulador(TList *lisLIG);
      void __fastcall DiagSuprimento(TList *lisEQB);
      void __fastcall DiagTrafo(TList *lisLIG);
      void __fastcall DiagTrafo3E(TList *lisLIG);
      void __fastcall DiagTramo(TList *lisLIG);
      void __fastcall DiagTrecho(TList *lisLIG);
	  bool __fastcall LigacaoIsolada(VTLigacao *ligacao);

   private: //objetos externos
      VTApl       *apl;
      VTGeral     *geral;
      VTPatamares *patamares;

   private: //dados locais
      TList      *lisEQP;
	  VTConsulta *consulta;
	  std::map <double, double> capacidade_max; //faixa de tensão (key) e capacidade (value)
   };

#endif
//---------------------------------------------------------------------------
//eof

