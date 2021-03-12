#ifndef VTRedeCCH
#define VTRedeCCH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "ResCurto.h"
#include "..\Rede\VTLigacao.h"

//---------------------------------------------------------------------------
class  VTApl;
class  VTBarra;
class  VTEqpto;
class  VTNet;
class  VTLigacao;
class  VTRedes;
class VTTrecho;
class  VTBarCC;
class  VTLigCC;
struct STR_ZDef;

//---------------------------------------------------------------------------
class VTRedeCC : public TObject
   {
   public:  //property
      __property int             ShowDefeito            = {read=PM_GetShowDefeito,        write=PM_SetShowDefeito};
      __property complex<double> S3f_mva                = {read=PM_GetS3f_mva,            write=PM_SetS3f_mva};
      __property complex<double> Sft_mva                = {read=PM_GetSft_mva,            write=PM_SetSft_mva};
      __property complex<double> Z0entrada_ohm          = {read=PM_GetZ0entrada,          write=PM_SetZ0entrada};
      __property complex<double> Z0entradaSoTrechos_ohm = {read=PM_GetZ0entradaSoTrechos, write=PM_SetZ0entradaSoTrechos};
      __property complex<double> Z1entrada_ohm          = {read=PM_GetZ1entrada,          write=PM_SetZ1entrada};
      __property complex<double> Z1entradaSoTrechos_ohm = {read=PM_GetZ1entradaSoTrechos, write=PM_SetZ1entradaSoTrechos};
      __property VTNet*          Net                    = {read=PM_GetNet,                write=PM_SetNet};

   public:
                        __fastcall  VTRedeCC(void) {};
      virtual           __fastcall ~VTRedeCC(void) {};
      virtual void      __fastcall  AssociaBarraBarCC(void) = 0;
      virtual VTBarra*  __fastcall  BarraDefeito(void) = 0;
      virtual VTBarCC*  __fastcall  BarCCDefeito(void) = 0;
      virtual VTBarCC*  __fastcall  CorrenteDefeito(void) = 0;
      virtual void      __fastcall  DefineBarraDefeito(VTBarra *barra) = 0;
      virtual void      __fastcall  DefineTrechoDefeito(VTTrecho *trecho, double dist_bar1_km) = 0;
      virtual VTEqpto*  __fastcall  EqptoDefeito(void) = 0;
      virtual VTBarCC*  __fastcall  ExisteBarCC(VTBarra *barra) = 0;
      virtual VTLigCC*  __fastcall  ExisteLigCC(VTLigacao *ligacao) = 0;
      virtual bool      __fastcall  GetZdef(STR_ZDef &Zdef) = 0;
      virtual void      __fastcall  Inicia(VTRedes *redes) = 0;
      virtual TList*    __fastcall  LisBarra(void) = 0;
      virtual TList*    __fastcall  LisBarraCC(void) = 0;
      virtual TList*    __fastcall  LisCarga(void) = 0;
      virtual TList*    __fastcall  LisEqbarAterrado(void) = 0;
      virtual TList*    __fastcall  LisGeradorSuprimento(void) = 0;
      virtual TList*    __fastcall  LisLigacao(void) = 0;
      virtual TList*    __fastcall  LisLigacaoCC(void) = 0;
      virtual TList*    __fastcall  LisLigacaoCC_Ficticia(void) = 0;
      virtual TList*    __fastcall  LisMutua(void) = 0;
      virtual void      __fastcall  SalvaIccBarDef(VTBarCC *pbcc, STR_FASE *ifase, STR_SEQ *iseq, STR_FASE *assim_ifase) = 0;
      virtual void      __fastcall  SalvaIccBarSup(VTBarCC *pbcc, STR_FASE *ifase, STR_SEQ *iseq) = 0;
      virtual void      __fastcall  SalvaIccLigCC(VTLigCC *plcc, VTBarCC *pbcc_ref, STR_FASE ifase[][NBAR_LIG], STR_SEQ iseq[][NBAR_LIG]) = 0;
      virtual void      __fastcall  SalvaVccBarCC(VTBarCC *pbcc, STR_FASE *vfase, STR_SEQ *vseq) = 0;
      virtual bool      __fastcall  SetZdef(STR_ZDef &Zdef) = 0;
      virtual VTTrecho* __fastcall  TrechoDefeito(void) = 0;
      virtual VTTrecho* __fastcall  TrechoFicticio(int ind_trecho) = 0;

   protected:  //métodos acessados via property
      virtual VTNet*          __fastcall PM_GetNet(void) = 0;
      virtual int             __fastcall PM_GetShowDefeito(void) = 0;
      virtual complex<double> __fastcall PM_GetS3f_mva(void) = 0;
      virtual complex<double> __fastcall PM_GetSft_mva(void) = 0;
      virtual complex<double> __fastcall PM_GetZ0entrada(void) = 0;
      virtual complex<double> __fastcall PM_GetZ0entradaSoTrechos(void) = 0;
      virtual complex<double> __fastcall PM_GetZ1entrada(void) = 0;
      virtual complex<double> __fastcall PM_GetZ1entradaSoTrechos(void) = 0;
      //set
      virtual void            __fastcall PM_SetNet(VTNet *net) = 0;
      virtual void            __fastcall PM_SetShowDefeito(int defeito) = 0;
      virtual void            __fastcall PM_SetS3f_mva(complex<double> s3f_mva) = 0;
      virtual void            __fastcall PM_SetSft_mva(complex<double> sft_mva) = 0;
      virtual void            __fastcall PM_SetZ0entrada(complex<double> z0) = 0;
      virtual void            __fastcall PM_SetZ0entradaSoTrechos(complex<double> z0) = 0;
      virtual void            __fastcall PM_SetZ1entrada(complex<double> z1) = 0;
      virtual void            __fastcall PM_SetZ1entradaSoTrechos(complex<double> z1) = 0;
   };

//-----------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRedeCC
//-----------------------------------------------------------------------------
VTRedeCC* __fastcall NewObjRedeCC(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
