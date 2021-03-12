//---------------------------------------------------------------------------
#ifndef VTFlowH
#define VTFlowH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
enum enumFLUXO {fluxoGAUSS1=0,  fluxoGAUSS3,  fluxoHARMO3, fluxoNR1,   fluxoNR3,
                fluxoRADIAL1, fluxoRADIAL3, fluxoCC1,    fluxoBARRA, /*fluxoMRT, */
                fluxoEQUILIBRADO, fluxoDESEQUILIBRADO, fluxoAUTO, fluxoDISABLED};


//---------------------------------------------------------------------------
enum enumRELATORIO {relNENHUM=0,  relRESUMIDO,  relCOMPLETO};

//---------------------------------------------------------------------------
class VTApl;
class VTRede;
class VTRedes;

//---------------------------------------------------------------------------
class VTFlow : public TObject
   {
   public:  //property
      __property bool RedeComCarga       = {read=PM_GetRedeComCarga,       write=PM_SetRedeComCarga};
      __property int  ModeloCarga        = {read=PM_GetModeloCarga,        write=PM_SetModeloCarga};
      __property int  TipoFluxo          = {read=PM_GetTipoFluxo,          write=PM_SetTipoFluxo};
      __property int  TipoFluxoAlt       = {read=PM_GetTipoFluxoAlt,       write=PM_SetTipoFluxoAlt};
      __property int  TipoRelatorio      = {read=PM_GetTipoRelatorio,      write=PM_SetTipoRelatorio};
      __property bool UpToDate           = {read=PM_GetUpToDate,           write=PM_SetUpToDate};
      __property bool AvisoEnabled       = {read=PM_GetAvisoEnabled,       write=PM_SetAvisoEnabled};
      __property bool CronometroEnabled  = {read=PM_GetCronometroEnabled,  write=PM_SetCronometroEnabled};
      __property bool MultiThreadEnabled = {read=PM_GetMultiThreadEnabled, write=PM_SetMultiThreadEnabled};
      __property bool PorRedeEnabled     = {read=PM_GetPorRedeEnabled,     write=PM_SetPorRedeEnabled};
      __property bool PerdaFerroEnabled  = {read=PM_GetPerdaFerroEnabled,  write=PM_SetPerdaFerroEnabled};
      __property bool ProgressoEnabled   = {read=PM_GetProgressoEnabled,   write=PM_SetProgressoEnabled};
      __property bool ReguladorBypass    = {read=PM_GetReguladorBypass,    write=PM_SetReguladorBypass};
      __property bool TramoEnabled       = {read=PM_GetTramoEnabled,       write=PM_SetTramoEnabled};
      __property bool RelatorioBarDesc   = {read=PM_GetRelatorioBarDesc,   write=PM_SetRelatorioBarDesc};

   public:
                   __fastcall  VTFlow(void) {};
                   __fastcall ~VTFlow(void) {};
      virtual bool __fastcall  CalculaPorRede(VTRedes *redes, bool dupla_precisao=false) = 0;
      virtual bool __fastcall  CalculaRedeUnica(VTRedes *redes) = 0;
      virtual bool __fastcall  CalculaUmaRede(VTRedes *redes, VTRede *rede) = 0;

   protected:  //métodos acessados via property
      virtual bool __fastcall PM_GetAvisoEnabled(void) = 0;
      virtual bool __fastcall PM_GetCronometroEnabled(void) = 0;
      virtual int  __fastcall PM_GetModeloCarga(void) = 0;
      virtual bool __fastcall PM_GetMultiThreadEnabled(void) = 0;
      virtual bool __fastcall PM_GetPerdaFerroEnabled(void) = 0;
      virtual bool __fastcall PM_GetPorRedeEnabled(void) = 0;
      virtual bool __fastcall PM_GetProgressoEnabled(void) = 0;
      virtual bool __fastcall PM_GetRedeComCarga(void) = 0;
      virtual bool __fastcall PM_GetReguladorBypass(void) = 0;
      virtual int  __fastcall PM_GetTipoFluxo(void) = 0;
      virtual int  __fastcall PM_GetTipoFluxoAlt(void) = 0;
      virtual int  __fastcall PM_GetTipoRelatorio(void) = 0;
      virtual bool __fastcall PM_GetTramoEnabled(void) = 0;
      virtual bool __fastcall PM_GetRelatorioBarDesc(void) = 0;
      virtual bool __fastcall PM_GetUpToDate(void) = 0;
      //set
      virtual void __fastcall PM_SetAvisoEnabled(bool enabled) = 0;
      virtual void __fastcall PM_SetCronometroEnabled(bool enabled) = 0;
      virtual void __fastcall PM_SetModeloCarga(int modelo_carga) = 0;
      virtual void __fastcall PM_SetMultiThreadEnabled(bool enabled) = 0;
      virtual void __fastcall PM_SetPerdaFerroEnabled(bool enabled) = 0;
      virtual void __fastcall PM_SetPorRedeEnabled(bool enabled) = 0;
      virtual void __fastcall PM_SetProgressoEnabled(bool enabled) = 0;
      virtual void __fastcall PM_SetRedeComCarga(bool rede_com_carga) = 0;
      virtual void __fastcall PM_SetReguladorBypass(bool enabled) = 0;
      virtual void __fastcall PM_SetTipoFluxo(int tipo_fluxo) = 0;
      virtual void __fastcall PM_SetTipoFluxoAlt(int tipo_fluxo) = 0;
      virtual void __fastcall PM_SetTipoRelatorio(int tipo_rel) = 0;
      virtual void __fastcall PM_SetTramoEnabled(bool enabled) = 0;
      virtual void __fastcall PM_SetRelatorioBarDesc(bool enabled) = 0;
      virtual void __fastcall PM_SetUpToDate(bool uptodate) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTFlow
//---------------------------------------------------------------------------
VTFlow* __fastcall NewObjFlow(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof



 