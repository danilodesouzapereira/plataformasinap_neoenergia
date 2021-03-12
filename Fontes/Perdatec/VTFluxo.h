//---------------------------------------------------------------------------
#ifndef VTFluxoH
#define VTFluxoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRede;
class VTCorte;
class VTReducao;
class VTTrafo;

//---------------------------------------------------------------------------
class VTFluxo : public TObject
   {
   public:
      __property double EnergiaSuprimento_mwh  = {read=PM_GetEnergiaSuprimento_mwh};
      __property double EnergiaGerador_mwh     = {read=PM_GetEnergiaGerador_mwh};
      __property double EnergiaRecebidaExt_mwh = {read=PM_GetEnergiaRecebidaExt_mwh};
      __property double EnergiaRecebidaInt_mwh = {read=PM_GetEnergiaRecebidaInt_mwh};
      __property double EnergiaEntregueExt_mwh = {read=PM_GetEnergiaEntregueExt_mwh};
      __property double EnergiaEntregueInt_mwh = {read=PM_GetEnergiaEntregueInt_mwh};
      __property double EnergiaCarga_mwh       = {read=PM_GetEnergiaCarga_mwh};

   public:
                     __fastcall VTFluxo(void) {};
      virtual        __fastcall ~VTFluxo(void) {};
      virtual void   __fastcall Executa(VTRede    *rede) = 0;
	  virtual void   __fastcall Executa(VTReducao *reducao) = 0;
	  virtual void   __fastcall Executa(VTTrafo *trafo) = 0;
      //virtual bool   __fastcall FluxoEnergiaDaRedeNoCorte_mwh(VTRede *rede, VTCorte *corte, double &Eext_mwh, double &Eint_mwh) = 0;

   protected:  //métodos acessados via property
      virtual double __fastcall PM_GetEnergiaCarga_mwh(void) = 0;
      virtual double __fastcall PM_GetEnergiaEntregueExt_mwh(void) = 0;
      virtual double __fastcall PM_GetEnergiaEntregueInt_mwh(void) = 0;
      virtual double __fastcall PM_GetEnergiaGerador_mwh(void) = 0;
      virtual double __fastcall PM_GetEnergiaRecebidaExt_mwh(void) = 0;
      virtual double __fastcall PM_GetEnergiaRecebidaInt_mwh(void) = 0;
      virtual double __fastcall PM_GetEnergiaSuprimento_mwh(void) = 0;
      };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTFluxo
//---------------------------------------------------------------------------
VTFluxo* __fastcall NewObjFluxo(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 