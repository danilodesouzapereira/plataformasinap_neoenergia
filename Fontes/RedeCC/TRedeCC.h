#ifndef TRedeCCH
#define TRedeCCH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTRedeCC.h"
#include "ResCurto.h"
#include "..\Rede\Estrutura.h"

//---------------------------------------------------------------------------
class TRedeCC : public VTRedeCC
   {
   public:
                __fastcall  TRedeCC(VTApl *apl);
                __fastcall ~TRedeCC(void);
      void      __fastcall  AssociaBarraBarCC(void);
      VTBarra*  __fastcall  BarraDefeito(void);
      VTBarCC*  __fastcall  BarCCDefeito(void);
      VTBarCC*  __fastcall  CorrenteDefeito(void);
      void      __fastcall  DefineBarraDefeito(VTBarra *barra);
      void      __fastcall  DefineTrechoDefeito(VTTrecho *trecho, double dist_bar1_km);
      VTEqpto*  __fastcall  EqptoDefeito(void);
      VTBarCC*  __fastcall  ExisteBarCC(VTBarra *barra);
      VTLigCC*  __fastcall  ExisteLigCC(VTLigacao *ligacao);
      bool      __fastcall  GetZdef(STR_ZDef &Zdef);
      void      __fastcall  Inicia(VTRedes *redes);
      TList*    __fastcall  LisBarra(void);
      TList*    __fastcall  LisBarraCC(void);
      TList*    __fastcall  LisCarga(void);
      TList*    __fastcall  LisEqbarAterrado(void);
      TList*    __fastcall  LisGeradorSuprimento(void);
      TList*    __fastcall  LisLigacao(void);
      TList*    __fastcall  LisLigacaoCC(void);
      TList*    __fastcall  LisLigacaoCC_Ficticia(void);
      TList*    __fastcall  LisMutua(void);
      void      __fastcall  SalvaIccBarDef(VTBarCC *pbcc, STR_FASE *ifase, STR_SEQ *iseq, STR_FASE *assim_ifase);
      void      __fastcall  SalvaIccBarSup(VTBarCC *pbcc, STR_FASE *ifase, STR_SEQ *iseq);
      void      __fastcall  SalvaIccLigCC(VTLigCC *plcc, VTBarCC *pbcc_ref, STR_FASE ifase[][NBAR_LIG], STR_SEQ iseq[][NBAR_LIG]);
      void      __fastcall  SalvaVccBarCC(VTBarCC *pbcc, STR_FASE *vfase, STR_SEQ *vseq);
      bool      __fastcall  SetZdef(STR_ZDef &Zdef);
      VTTrecho* __fastcall  TrechoDefeito(void);
      VTTrecho* __fastcall  TrechoFicticio(int ind_trecho);

   protected:  //métodos acessados via property
      VTNet*          __fastcall PM_GetNet(void);
      int             __fastcall PM_GetShowDefeito(void);
      complex<double> __fastcall PM_GetS3f_mva(void);
      complex<double> __fastcall PM_GetSft_mva(void);
      complex<double> __fastcall PM_GetZ0entrada(void);
      complex<double> __fastcall PM_GetZ0entradaSoTrechos(void);
      complex<double> __fastcall PM_GetZ1entrada(void);
      complex<double> __fastcall PM_GetZ1entradaSoTrechos(void);
      //set
      void            __fastcall PM_SetNet(VTNet *net);
      void            __fastcall PM_SetShowDefeito(int defeito);
      void            __fastcall PM_SetS3f_mva(complex<double> s3f_mva);
      void            __fastcall PM_SetSft_mva(complex<double> sft_mva);
      void            __fastcall PM_SetZ0entrada(complex<double> z0);
      void            __fastcall PM_SetZ0entradaSoTrechos(complex<double> z0);
      void            __fastcall PM_SetZ1entrada(complex<double> z1);
      void            __fastcall PM_SetZ1entradaSoTrechos(complex<double> z1);

   private: //métodos
      void     __fastcall AjustaBarraFicticia(double dist_bar1);
      void     __fastcall AjustaTrechoFicticio(double dist_bar1);
      void     __fastcall IniciaLisBarCC(void);
      void     __fastcall IniciaLisLigCC(void);
      void     __fastcall IniciaLisLigCC_Ficticia(VTRedes *redes);
      int      __fastcall MaiorId(TList *lisEQP, int tipo);
      void     __fastcall ReconfiguraRedeCC(void);
      void     __fastcall ReiniciaResultados(void);
      void     __fastcall RestauraRedeCC(void);
      void     __fastcall SalvaIccBar(VTBarCC *pbcc, STR_FASE *ifase, STR_SEQ *iseq);
      void     __fastcall SalvaIccBar_Assimetrico(VTBarCC *pbcc, STR_FASE *assim_ifase);
      void     __fastcall VerificaSentidoFluxo(VTLigCC *ligCC);
      //void     __fastcall VerificaSentidoFluxoLigacaoFicticia(VTLigCC *ligCC);

   private: //objetos externos
      VTApl *apl;
      VTNet *net;

   private: //dados locais
      TList *lisGS;      //lista de Geradores e Suprimentos da RedeCC
      TList *lisZT;      //lista de Eqbar   da RedeCC (Filtro, Reator, Zref, TrafoZZ)
      TList *lisBAR;     //lista de Barra   da RedeCC
      TList *lisCAR;     //lista de Cargas  da RedeCC
      TList *lisLIG;     //lista de Ligacao da RedeCC
      TList *lisMUT;     //lista de Mutuas  da RedeCC
      TList *lisBAR_CC;  //lista de BarCC   da RedeCC
      TList *lisLIG_CC;  //lista de LigCC   da RedeCC
      TList *lisEQP;
      struct   {
               TList *lisLIG_CC; //lista de LigCC das Ligacoes fictícas das Reducoes
               }ficticia;
      struct   {//ponteiro p/ Eqpto (Barra ou Trecho) com defeito e ponteiro p/ BarCC da Barra de defeito
               VTBarCC  *barCC;
               VTEqpto  *eqpto;
               STR_ZDef Zdef;
               } defeito;
      struct   {//ponteiro para BarCC usado apenas para armazenar correntes de defeito (não está associado a nenhuma barra)
               VTBarCC  *barCC;
               } corrente;
      struct   {//estrutura de controle da troca do Trecho original por Barra e Trechos fictícios
               VTBarra  *bar_fic;
               VTTrecho *tre_fic1;
               VTTrecho *tre_fic2;
               VTTrecho *tre_orig;
               VTBarCC  *barCC_fic;
               VTLigCC  *ligCC_fic1;
               VTLigCC  *ligCC_fic2;
               //VTLigCC  *ligCC_tre_orig;
               } troca;
      struct   {
               int             show_defeito;          //flag p/ animação gráfica
               complex<double> s3f_mva;
               complex<double> sft_mva;
               complex<double> z0_entrada;            //ohm
               complex<double> z1_entrada;            //ohm
               complex<double> z0_entrada_so_trechos; //ohm
               complex<double> z1_entrada_so_trechos; //ohm
               }PD;
   };

#endif
//-----------------------------------------------------------------------------
// eof
