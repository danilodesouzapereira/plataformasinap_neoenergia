//---------------------------------------------------------------------------
#ifndef TBalancoH
#define TBalancoH

//---------------------------------------------------------------------------
#include "VTBalanco.h"

//---------------------------------------------------------------------------
class TBalanco : public VTBalanco
   {
   public:
           __fastcall  TBalanco(void);
           __fastcall ~TBalanco(void);
      bool __fastcall  Agrega(VTBalanco *balanco);

   private:  //métodos acessados via property
      AnsiString __fastcall PM_GetDescricao(void);
      double     __fastcall PM_GetEnergiaCarga_mwh(void);
      double     __fastcall PM_GetEnergiaEntrada_mwh(void);
      double     __fastcall PM_GetEnergiaEntregueExt_mwh(void);
      double     __fastcall PM_GetEnergiaEntregueInt_mwh(void);
      double     __fastcall PM_GetEnergiaGerador_mwh(void);
      double     __fastcall PM_GetEnergiaPerdaFluxo_mwh(void);
      double     __fastcall PM_GetEnergiaPerdaFora_mwh(void);
      double     __fastcall PM_GetEnergiaPerdaTotal_mwh(void);
      double     __fastcall PM_GetEnergiaRecebidaExt_mwh(void);
      double     __fastcall PM_GetEnergiaRecebidaInt_mwh(void);
      double     __fastcall PM_GetEnergiaSuprimento_mwh(void);
      TObject*   __fastcall PM_GetObject(void);
      //
      void       __fastcall PM_SetDescricao(AnsiString descricao);
      void       __fastcall PM_SetEnergiaCarga_mwh(double energia_mwh);
      void       __fastcall PM_SetEnergiaEntregueExt_mwh(double energia_mwh);
      void       __fastcall PM_SetEnergiaEntregueInt_mwh(double energia_mwh);
      void       __fastcall PM_SetEnergiaGerador_mwh(double energia_mwh);
      void       __fastcall PM_SetEnergiaPerdaFluxo_mwh(double energia_mwh);
      void       __fastcall PM_SetEnergiaPerdaFora_mwh(double energia_mwh);
      void       __fastcall PM_SetEnergiaPerdaTotal_mwh(double energia_mwh);
      void       __fastcall PM_SetEnergiaRecebidaExt_mwh(double energia_mwh);
      void       __fastcall PM_SetEnergiaRecebidaInt_mwh(double energia_mwh);
      void       __fastcall PM_SetEnergiaSuprimento_mwh(double energia_mwh);
      void       __fastcall PM_SetObject(TObject *object);

   private: //dados locais acessados via property
      struct   {
               AnsiString descricao;
               TObject   *object;
               struct   {
                        double carga_mwh;
                        double entregue_ext_mwh;
                        double entregue_int_mwh;
                        double gerador_mwh;
                        double recebida_ext_mwh;
                        double recebida_int_mwh;
                        double suprimento_mwh;
                        double perda_fluxo_mwh;
                        double perda_fora_mwh;
                        double perda_total_mwh;
                        }energia;
               }PD;
   };

#endif
//---------------------------------------------------------------------------
//eof

