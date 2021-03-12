//---------------------------------------------------------------------------
#ifndef VTBalancoH
#define VTBalancoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBalanco : public TObject
   {
   public:  //property
      __property TObject*   Object                 = {read=PM_GetObject,                 write=PM_SetObject                 };
      __property AnsiString Descricao              = {read=PM_GetDescricao,              write=PM_SetDescricao              };
      __property double     EnergiaEntrada_mwh     = {read=PM_GetEnergiaEntrada_mwh                                         };
      __property double     EnergiaSuprimento_mwh  = {read=PM_GetEnergiaSuprimento_mwh,  write=PM_SetEnergiaSuprimento_mwh  };
      __property double     EnergiaGerador_mwh     = {read=PM_GetEnergiaGerador_mwh,     write=PM_SetEnergiaGerador_mwh     };
      __property double     EnergiaRecebidaExt_mwh = {read=PM_GetEnergiaRecebidaExt_mwh, write=PM_SetEnergiaRecebidaExt_mwh };
      __property double     EnergiaRecebidaInt_mwh = {read=PM_GetEnergiaRecebidaInt_mwh, write=PM_SetEnergiaRecebidaInt_mwh };
      __property double     EnergiaEntregueExt_mwh = {read=PM_GetEnergiaEntregueExt_mwh, write=PM_SetEnergiaEntregueExt_mwh };
      __property double     EnergiaEntregueInt_mwh = {read=PM_GetEnergiaEntregueInt_mwh, write=PM_SetEnergiaEntregueInt_mwh };
      __property double     EnergiaCarga_mwh       = {read=PM_GetEnergiaCarga_mwh,       write=PM_SetEnergiaCarga_mwh       };
      __property double     EnergiaPerdaFluxo_mwh  = {read=PM_GetEnergiaPerdaFluxo_mwh,  write=PM_SetEnergiaPerdaFluxo_mwh  };
      __property double     EnergiaPerdaFora_mwh   = {read=PM_GetEnergiaPerdaFora_mwh,   write=PM_SetEnergiaPerdaFora_mwh   };
      __property double     EnergiaPerdaTotal_mwh  = {read=PM_GetEnergiaPerdaTotal_mwh,  write=PM_SetEnergiaPerdaTotal_mwh  };

   public:
                     __fastcall  VTBalanco(void) {};
      virtual        __fastcall ~VTBalanco(void) {};
      virtual bool   __fastcall  Agrega(VTBalanco *balanco) = 0;

   protected:  //métodos acessados via property
      virtual AnsiString __fastcall PM_GetDescricao(void) = 0;
      virtual double     __fastcall PM_GetEnergiaCarga_mwh(void) = 0;
      virtual double     __fastcall PM_GetEnergiaEntrada_mwh(void) = 0;
      virtual double     __fastcall PM_GetEnergiaEntregueExt_mwh(void) = 0;
      virtual double     __fastcall PM_GetEnergiaEntregueInt_mwh(void) = 0;
      virtual double     __fastcall PM_GetEnergiaGerador_mwh(void) = 0;
      virtual double     __fastcall PM_GetEnergiaPerdaFluxo_mwh(void) = 0;
      virtual double     __fastcall PM_GetEnergiaPerdaFora_mwh(void) = 0;
      virtual double     __fastcall PM_GetEnergiaPerdaTotal_mwh(void) = 0;
      virtual double     __fastcall PM_GetEnergiaRecebidaExt_mwh(void) = 0;
      virtual double     __fastcall PM_GetEnergiaRecebidaInt_mwh(void) = 0;
      virtual double     __fastcall PM_GetEnergiaSuprimento_mwh(void) = 0;
      virtual TObject*   __fastcall PM_GetObject(void) = 0;
      //
      virtual void       __fastcall PM_SetDescricao(AnsiString descricao) = 0;
      virtual void       __fastcall PM_SetEnergiaCarga_mwh(double energia_mwh) = 0;
      virtual void       __fastcall PM_SetEnergiaEntregueExt_mwh(double energia_mwh) = 0;
      virtual void       __fastcall PM_SetEnergiaEntregueInt_mwh(double energia_mwh) = 0;
      virtual void       __fastcall PM_SetEnergiaGerador_mwh(double energia_mwh) = 0;
      virtual void       __fastcall PM_SetEnergiaPerdaFluxo_mwh(double energia_mwh) = 0;
      virtual void       __fastcall PM_SetEnergiaPerdaFora_mwh(double energia_mwh) = 0;
      virtual void       __fastcall PM_SetEnergiaPerdaTotal_mwh(double energia_mwh) = 0;
      virtual void       __fastcall PM_SetEnergiaRecebidaExt_mwh(double energia_mwh) = 0;
      virtual void       __fastcall PM_SetEnergiaRecebidaInt_mwh(double energia_mwh) = 0;
      virtual void       __fastcall PM_SetEnergiaSuprimento_mwh(double energia_mwh) = 0;
      virtual void       __fastcall PM_SetObject(TObject *object) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTBalanco
//---------------------------------------------------------------------------
VTBalanco* __fastcall NewObjBalanco(void);

//---------------------------------------------------------------------------
#endif
//eof



