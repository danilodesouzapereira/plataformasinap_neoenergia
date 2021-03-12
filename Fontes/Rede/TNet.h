//---------------------------------------------------------------------------
#ifndef TNetH
#define TNetH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTNet.h"

//---------------------------------------------------------------------------
class VTGrupo;

//---------------------------------------------------------------------------
class TNet : public VTNet
   {
   public:
                 __fastcall  TNet(void);
                 __fastcall ~TNet(void);
      bool       __fastcall  ExisteDesequilibrio(void);
      VTGrupo*   __fastcall  ExisteGrupoMutua(VTTrecho *trecho);
      VTMutua*   __fastcall  ExisteMutua(VTTrecho *trecho);
      bool       __fastcall  InsereBarra(VTBarra *barra);
      void       __fastcall  InsereEqbar(VTEqbar *eqbar);
      void       __fastcall  InsereLigacao(VTLigacao *ligacao);
      void       __fastcall  InsereLisBarra(TList *lisEQP);
		void       __fastcall  InsereLisEqbar(TList *lisEQP);
      void       __fastcall  InsereLisLigacao(TList *lisEQP);
      void       __fastcall  InsereLisLigacaoIsolada(TList *lisEQP);
      void       __fastcall  InsereLisMutua(TList *lisEQP);
      void       __fastcall  InsereMutua(VTArranjo *arranjo, VTLigacao *ligacao);
      void       __fastcall  InsereMutua(VTMutua *mutua);
      void       __fastcall  InsereTramo(VTTramo *tramo);
      void       __fastcall  InsereNo(VTNo *pno);
      TList*     __fastcall  LisBarra(void);
      TList*     __fastcall  LisBarraIsolada(void);
      TList*     __fastcall  LisEqbar(void);
      void       __fastcall  LisEqbar(TList *lisEQP, int tipo);
      TList*     __fastcall  LisGrupoMutua(void);
      TList*     __fastcall  LisLigacao(void);
      void       __fastcall  LisLigacao(TList *lisEQP, int tipo);
      TList*     __fastcall  LisLigacaoIsolada(void);
      TList*     __fastcall  LisLigacaoSemChave(void);
      TList*     __fastcall  LisLigacaoSemChaveSemMutua(void);
      TList*     __fastcall  LisMutua(void);
		TList*     __fastcall  LisNo(void);
		void       __fastcall  LisZona(TList *lisEQP);
      int        __fastcall  NumGerSup(void);
      int        __fastcall  NumGerSupPV(void);
      void       __fastcall  Reinicia(void);
      void       __fastcall  RetiraNo(VTNo *no);
      void       __fastcall  RemoveLigacao(VTLigacao *ligacao);
		int        __fastcall  Tipo(void);
      AnsiString __fastcall  TipoAsString(void);
      bool       __fastcall ValidaFasesTrafo(void);

   protected:  //métodos acessados via property
      AnsiString __fastcall PM_GetCodigo(void);
      int        __fastcall PM_GetModeloCarga(void);
      int        __fastcall PM_GetNumeroCapserie(void);
      int        __fastcall PM_GetNumeroNo(void);
      int        __fastcall PM_GetNumeroRegulador(void);
      int        __fastcall PM_GetNumeroTrafo(void);
      int        __fastcall PM_GetNumeroTrafo3E(void);
      int        __fastcall PM_GetNumeroTramo(void);
      int        __fastcall PM_GetNumeroTrechoComMutua(void);
      int        __fastcall PM_GetNumeroTrechoSemMutua(void);
		bool       __fastcall PM_GetRadial(void);
      bool       __fastcall PM_GetRede4Fios(void);
		int        __fastcall PM_GetStatusFluxo(void);
		int        __fastcall PM_GetStatusFluxoNumPatOK(void);
		//write
      void       __fastcall PM_SetCodigo(AnsiString codigo);
		void       __fastcall PM_SetModeloCarga(int modelo_carga);
		void       __fastcall PM_SetRadial(bool radial);
		void       __fastcall PM_SetStatusFluxo(int status);
		void       __fastcall PM_SetStatusFluxoNumPatOK(int num_pat);

   private:  //métodos
      //void     __fastcall CancelaNoDaBarra(void);
      VTGrupo* __fastcall ExisteGrupoMutua(VTArranjo *arranjo, VTLigacao *ligacao);
      VTGrupo* __fastcall ExisteGrupoMutua(VTMutua *mutua);
      void     __fastcall MontaGrupoDeMutua(VTMutua *mutua);
      int      __fastcall NumeroEqpto(TList *lisEQP, int tipo);

   private:  //dados acessados via property
      struct   {
               AnsiString codigo;
					int        modelo_carga;
					int        status_fluxo;
					int        status_fluxo_num_pat_ok;
               bool       radial;
               }PD;

   private:  //dados locais
      TList *lisBAR;    //lista de objetos VTBarra
      TList *lisEQB;    //lista de objetos VTEqbar
		TList *lisLIG;    //lista de objetos VTLigacao
      TList *lisLSC;    //lista de objetos VTLigacao sem TChave
      TList *lisLSCM;   //lista de objetos VTLigacao sem TChave e sem Trechos c/ Mutua
      TList *lisGRP;    //lista de objetos VTGrupo c/ objetos VTMutua
      TList *lisMUT;    //lista de objetos VTMutua
      TList *lisNO;     //lista de objetos VTNo
      TList *lisTRM;    //lista de objetos VTTramo
      struct   {//estrutura p/ manter a parte isolada da rede
               TList *lisBAR;    //lista de objetos VTBarra
               TList *lisLIG;    //lista de objetos VTLigacao
               }isolada;
   };

//---------------------------------------------------------------------------
#endif
//eof
