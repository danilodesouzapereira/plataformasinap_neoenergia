//---------------------------------------------------------------------------
#ifndef TFluxoH
#define TFluxoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTFluxo.h"

//---------------------------------------------------------------------------
class VTArvore;
class VTDemanda;
class VTPatamares;
class VTBarra;
class VTCorte;
class VTRede;
class VTRedes;
class VTFases;

//---------------------------------------------------------------------------
class TFluxo : public VTFluxo
   {
   public:
               __fastcall TFluxo(VTApl *apl);
               __fastcall ~TFluxo(void);
      void     __fastcall Executa(VTRede *rede);
	  void     __fastcall Executa(VTReducao *reducao);
      void     __fastcall Executa(VTTrafo *trafo);

   private:  //tipo de dados
      struct strLKC  {
                     double Esup_mwh;
                     double Eext_mwh;
                     double Eint_mwh;
                     };
   private: //métodos
      VTCorte* __fastcall BarraPertenceCorte(VTRede *rede, VTBarra *barra);
      double   __fastcall CalculaEnergia_mwh(double p_mw, int num_pat);
      //double   __fastcall CalculaEnergaCargasDoCorte(VTCorte *corte);
      void     __fastcall CalculaEnergiaCarga(VTRede *rede, TList *lisBAR_CAR);
      void     __fastcall CalculaEnergiaGerador(TList *lisGER);
	  void     __fastcall CalculaEnergiaSuprimento(TList *lisSUP);
	  void     __fastcall CalculaEnergiaTrafo(VTTrafo *trafo);
      void     __fastcall CalculaEnergiaRecebidaEntregue(VTRede *rede);
      bool     __fastcall FluxoEnergiaDaRedeNoCorte_mwh(VTRede *rede, VTCorte *corte, strLKC &lkc);
      double   __fastcall FluxoEnergiaDaReducaoNoCorte_mwh(VTReducao *reducao);
      void     __fastcall IniciaLisBarraCarga(VTRede *rede, TList *lisBAR_DEST);
      void     __fastcall IniciaLisEqbar(TList *lisCORTE, int tipo_eqbar, TList *lisEQB);
      void     __fastcall IniciaLisEqbar(VTRede *rede, int tipo_eqbar, TList *lisEQB);
      void     __fastcall LisLigacao(TList *lisLIG, VTBarra *barra, TList *lisEQP);

   protected:  //métodos acessados via property
      double __fastcall PM_GetEnergiaCarga_mwh(void);
      double __fastcall PM_GetEnergiaEntregueExt_mwh(void);
      double __fastcall PM_GetEnergiaEntregueInt_mwh(void);
      double __fastcall PM_GetEnergiaGerador_mwh(void);
      double __fastcall PM_GetEnergiaRecebidaExt_mwh(void);
      double __fastcall PM_GetEnergiaRecebidaInt_mwh(void);
      double __fastcall PM_GetEnergiaSuprimento_mwh(void);

   private: //objetos externos
      VTApl       *apl;
      VTArvore    *arvore;
      VTDemanda   *demanda;
      VTPatamares *patamares;
      VTFases     *fases;

   private:  //dados acessados via property
      struct{
            double suprimento_mwh;       //valor positivo
            double gerador_mwh;          //valor positivo
            double recebida_ext_mwh;     //valor positivo
            double recebida_int_mwh;     //valor positivo
            double entregue_ext_mwh;     //valor negativo
            double entregue_int_mwh;     //valor negativo
            double carga_mwh;            //negativo
            }energia;

   private: //dados locais
      TList *lisBAR;
      TList *lisEQP;
   };

#endif
//---------------------------------------------------------------------------
//eof

