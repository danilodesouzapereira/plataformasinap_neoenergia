//---------------------------------------------------------------------------
#ifndef VTNetH
#define VTNetH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTEqpto.h"

//-----------------------------------------------------------------------------
enum sttFLUXO {fluxoNAO_CALCULADO=0, fluxoCALCULANDO=1, fluxoCALCULADO=2};

//---------------------------------------------------------------------------
class VTArranjo;
class VTBarra;
class VTEqbar;
class VTGrupo;
class VTLigacao;
class VTMutua;
class VTNo;
class VTRede;
class VTTramo;
class VTTrecho;

//---------------------------------------------------------------------------
class VTNet : public VTEqpto
   {
   public:  //property
		__property AnsiString Codigo               = {read=PM_GetCodigo, write=PM_SetCodigo};
      __property int        NumeroCapserie       = {read=PM_GetNumeroCapserie};
      __property int        NumeroNo             = {read=PM_GetNumeroNo};
      __property int        NumeroRegulador      = {read=PM_GetNumeroRegulador};
      __property int        NumeroTrafo          = {read=PM_GetNumeroTrafo};
      __property int        NumeroTrafo3E        = {read=PM_GetNumeroTrafo3E};
      __property int        NumeroTramo          = {read=PM_GetNumeroTramo};
      __property int        NumeroTrechoComMutua = {read=PM_GetNumeroTrechoComMutua};
      __property int        NumeroTrechoSemMutua = {read=PM_GetNumeroTrechoSemMutua};
      __property bool       Rede4Fios            = {read=PM_GetRede4Fios};
      __property int        ModeloCarga          = {read=PM_GetModeloCarga,         write=PM_SetModeloCarga};
		__property bool       Radial               = {read=PM_GetRadial,              write=PM_SetRadial     };
      __property int        StatusFluxo          = {read=PM_GetStatusFluxo,         write=PM_SetStatusFluxo};
      __property int        StatusFluxoNumPatOK  = {read=PM_GetStatusFluxoNumPatOK, write=PM_SetStatusFluxoNumPatOK};

   public:
                       __fastcall  VTNet(void) {};
      virtual          __fastcall ~VTNet(void) {};
      virtual bool     __fastcall  ExisteDesequilibrio(void) = 0;
      virtual VTGrupo* __fastcall  ExisteGrupoMutua(VTTrecho *trecho) = 0;
      virtual VTMutua* __fastcall  ExisteMutua(VTTrecho *trecho) = 0;
      virtual bool     __fastcall  InsereBarra(VTBarra *barra) = 0;
      virtual void     __fastcall  InsereEqbar(VTEqbar *eqbar) = 0;
      virtual void     __fastcall  InsereLigacao(VTLigacao *ligacao) = 0;
      virtual void     __fastcall  InsereLisBarra(TList *lisEQP) = 0;
      virtual void     __fastcall  InsereLisEqbar(TList *lisEQP) = 0;
      virtual void     __fastcall  InsereLisLigacao(TList *lisEQP) = 0;
      virtual void     __fastcall  InsereLisLigacaoIsolada(TList *lisEQP) = 0;
      virtual void     __fastcall  InsereLisMutua(TList *lisEQP) = 0;
      virtual void     __fastcall  InsereMutua(VTMutua *mutua) = 0;
      virtual void     __fastcall  InsereMutua(VTArranjo *arranjo, VTLigacao *ligacao) = 0;
      virtual void     __fastcall  InsereNo(VTNo *pno) = 0;
      virtual void     __fastcall  InsereTramo(VTTramo *tramo) = 0;
      virtual TList*   __fastcall  LisBarra(void) = 0;
      virtual TList*   __fastcall  LisBarraIsolada(void) = 0;
      virtual TList*   __fastcall  LisEqbar(void) = 0;
      virtual void     __fastcall  LisEqbar(TList *lisEQP, int tipo) = 0;
      virtual TList*   __fastcall  LisGrupoMutua(void) = 0;
      virtual TList*   __fastcall  LisLigacao(void) = 0;
      virtual void     __fastcall  LisLigacao(TList *lisEQP, int tipo) = 0;
      virtual TList*   __fastcall  LisLigacaoIsolada(void) = 0;
      virtual TList*   __fastcall  LisLigacaoSemChave(void) = 0;
      virtual TList*   __fastcall  LisLigacaoSemChaveSemMutua(void) = 0;
      virtual TList*   __fastcall  LisMutua(void) = 0;
      virtual TList*   __fastcall  LisNo(void) = 0;
      virtual void     __fastcall  LisZona(TList *lisEQP) = 0;
		virtual int      __fastcall  NumGerSup(void) = 0;
      virtual int      __fastcall  NumGerSupPV(void) = 0;
      virtual void     __fastcall  Reinicia(void) = 0;
      virtual void     __fastcall  RemoveLigacao(VTLigacao *ligacao) = 0;
      virtual void     __fastcall  RetiraNo(VTNo *no) = 0;
      virtual bool     __fastcall  ValidaFasesTrafo(void) = 0;

   protected:  //métodos acessados via property
      virtual AnsiString __fastcall PM_GetCodigo(void) = 0;
      virtual int        __fastcall PM_GetModeloCarga(void) = 0;
      virtual int        __fastcall PM_GetNumeroCapserie(void) = 0;
      virtual int        __fastcall PM_GetNumeroNo(void) = 0;
      virtual int        __fastcall PM_GetNumeroRegulador(void) = 0;
      virtual int        __fastcall PM_GetNumeroTrafo(void) = 0;
      virtual int        __fastcall PM_GetNumeroTrafo3E(void) = 0;
      virtual int        __fastcall PM_GetNumeroTramo(void) = 0;
      virtual int        __fastcall PM_GetNumeroTrechoComMutua(void) = 0;
      virtual int        __fastcall PM_GetNumeroTrechoSemMutua(void) = 0;
		virtual bool       __fastcall PM_GetRadial(void) = 0;
		virtual bool       __fastcall PM_GetRede4Fios(void) = 0;
		virtual int        __fastcall PM_GetStatusFluxo(void) = 0;
		virtual int        __fastcall PM_GetStatusFluxoNumPatOK(void) = 0;
      //write
      virtual void       __fastcall PM_SetCodigo(AnsiString codigo) = 0;
		virtual void       __fastcall PM_SetModeloCarga(int modelo_carga) = 0;
		virtual void       __fastcall PM_SetRadial(bool radial) = 0;
		virtual void       __fastcall PM_SetStatusFluxo(int status) = 0;
		virtual void       __fastcall PM_SetStatusFluxoNumPatOK(int num_pat) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTNet
//---------------------------------------------------------------------------
VTNet* __fastcall NewObjNet(void);

//---------------------------------------------------------------------------
#endif
//eof


 