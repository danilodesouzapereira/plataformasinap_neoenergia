//---------------------------------------------------------------------------
#ifndef TRebuildH
#define TRebuildH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTRebuild.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTLigacao;
class VTRede;
class VTRedes;

//---------------------------------------------------------------------------
class TRebuild : public VTRebuild
   {
   public:
           __fastcall  TRebuild(VTApl *apl);
           __fastcall ~TRebuild(void);
		bool __fastcall  Executa(VTRedes *redes, TList *lisREDE, TList *lisBARRA, TList *lisLIGACAO);

   private: //métodos
      void       __fastcall LimpaListas(void);
      void       __fastcall MontaRedeEstatica(TList *lisLIG);
      void       __fastcall MontaRedeReconfiguravel(TList *lisLIG);
      bool       __fastcall OrdenaLigacaoDaRede(VTRede *rede, TList *lisLIG);
      bool       __fastcall OrdenaLigacaoIsolada(VTRede *rede, TList *lisLIG);
      void       __fastcall SeparaRedesEstaticasRedesReconfiguraveis(TList *lisREDE);

   private: //dados externos
		VTApl   *apl;
		VTRedes *redes;

   private: //dados locais
      TList *lisL;     //lista de Ligacoes de uma rede
      TList *lisBAR;   //lista de Barras       ordenadas de uma Rede
      TList *lisCHV;   //lista de Chaves       abertas   de todas Redes
      TList *lisNR;    //lista de Redes NÃO reconfiguráveis
	  TList *lisRR;    //lista de Redes reconfiguráveis
      TList *lisALL_BAR, *lisALL_REDE;
   };

#endif
//---------------------------------------------------------------------------
//eof

