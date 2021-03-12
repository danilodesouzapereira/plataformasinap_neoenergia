//---------------------------------------------------------------------------
#ifndef TTopologiaH
#define TTopologiaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTTopologia.h"

//---------------------------------------------------------------------------
class VTClassifica;
class VTMontaMNet;
class VTReconfigura;
class VTRebuild;
class VTRede;

//---------------------------------------------------------------------------
class TTopologia : public VTTopologia
   {
   public:
           __fastcall  TTopologia(VTApl *apl);
           __fastcall ~TTopologia(void);
		void __fastcall  ClassificaRedes(VTRedes *redes);
		bool __fastcall  Rebuild(VTRedes *redes, TList *lisREDE, TList *lisBARRA, TList *lisLIGACAO);
      bool __fastcall  Reconfigura(VTRedes *redes);
		bool __fastcall  ReconfiguraRede(VTRedes *redes, TList *lisREDE);
		bool __fastcall  ReconfiguraRede(VTRedes *redes, VTRede *rede);

	private: //métodos
		void __fastcall  LimpaListas(void);
		void __fastcall  ReiniciaLisBarraRede(TList *lisREDE);
		bool __fastcall  ReiniciaCorteMRede(void);
		void __fastcall  VerificaBarraIsolada(void);

   private: //dados externos
		VTApl   *apl;
		VTRedes *redes;

	private: //dados locais
      VTClassifica  *classifica;
      VTRebuild     *rebuild;
      VTReconfigura *reconfigura;
      VTMontaMNet   *monta_mnet;
      TList         *lisNR;    //lista de Redes NÃO reconfiguráveis
      TList         *lisRR;    //lista de Redes reconfiguráveis
      TList         *lisVIZ;   //lista de Redes vizinhas a uma Rede
   };

#endif
//---------------------------------------------------------------------------
//eof

