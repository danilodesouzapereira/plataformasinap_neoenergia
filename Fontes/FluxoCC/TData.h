//---------------------------------------------------------------------------
#ifndef TDataH
#define TDataH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTData.h"

//---------------------------------------------------------------------------
class TBar;
class TLig;

//---------------------------------------------------------------------------
class TData : public VTData
   {
   public:
             __fastcall TData(VTApl *apl);
             __fastcall ~TData(void);
      bool   __fastcall Inicia(double sbase);
      bool   __fastcall Insere(smcBARRA      &barra);
      bool   __fastcall Insere(smcCAPACITOR  &capacitor);
      bool   __fastcall Insere(smcCAPSERIE   &capserie);
      bool   __fastcall Insere(smcCARGA      &carga);
      bool   __fastcall Insere(smcFILTRO     &filtro);
      bool   __fastcall Insere(smcGERADOR    &gerador);
      bool   __fastcall Insere(smcMUTUA      &mutua);
      bool   __fastcall Insere(smcREATOR     &reator);
      bool   __fastcall Insere(smcREGULADOR  &regulador);
      bool   __fastcall Insere(smcSUPRIMENTO &suprimento);
      bool   __fastcall Insere(smcTRAFO2E    &trafo2E);
      bool   __fastcall Insere(smcTRAFO3E    &trafo3E);
      bool   __fastcall Insere(smcTRAFOZZ    &trafoZZ);
      bool   __fastcall Insere(smcTRECHO     &trecho);
      bool   __fastcall Insere(smcYREF       &yref);
      bool   __fastcall Insere(smcZREF       &zref);
      TList* __fastcall LisBarra(void);
      TList* __fastcall LisLigacao(void);
      bool   __fastcall Resultado(smcBARRA      &barra);
      bool   __fastcall Resultado(smcGERADOR    &gerador);
      bool   __fastcall Resultado(smcSUPRIMENTO &suprimento);
      bool   __fastcall Resultado(smcTRAFO2E    &trafo2E);
      bool   __fastcall Resultado(smcTRECHO     &trecho);

   private: //métodos acessados via property
      int    __fastcall PM_GetNumBar(void);
      int    __fastcall PM_GetNumLig(void);
      double __fastcall PM_GetSbaseMVA(void);

   private: //métodos
      TBar*  __fastcall CriaNovaBar(void *eqpto, int bar_id, AnsiString codigo);
      TLig*  __fastcall CriaNovaLig(void *eqpto, int bar1_id, int bar2_id);
      TBar*  __fastcall ExisteBar(int bar_id);
      TBar*  __fastcall ExisteBar(void *eqpto);
      TLig*  __fastcall ExisteLig(int bar1_id, int bar2_id);
      TLig*  __fastcall ExisteLig(void *eqpto);
      double __fastcall Pinjetada_pu(TBar *bar);
      double __fastcall Susceptancia_pu(double r_pu, double x_pu);
      double __fastcall Susceptancia_pu(double r_ohm, double x_ohm, double Vbase_kv);

   private: //objetos externos
      VTApl *apl;


   private: //dados locais
      double Sbase_mva;
      TList *lisBAR;
      TList *lisLIG;
   };

#endif
//---------------------------------------------------------------------------
//eof
