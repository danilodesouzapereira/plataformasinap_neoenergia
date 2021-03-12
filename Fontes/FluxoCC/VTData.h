//---------------------------------------------------------------------------
#ifndef VTDataH
#define VTDataH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Interface.h"

//---------------------------------------------------------------------------
class VTApl;
class TBar;
class TLig;

//---------------------------------------------------------------------------
class VTData : public TObject
   {
   protected:  //métodos acessados via property
      virtual int    __fastcall PM_GetNumBar(void) = 0;
      virtual int    __fastcall PM_GetNumLig(void) = 0;
      virtual double __fastcall PM_GetSbaseMVA(void) = 0;

   public: //property
      __property int    NumBar   = {read=PM_GetNumBar};
      __property int    NumLig   = {read=PM_GetNumLig};
      __property double SbaseMVA = {read=PM_GetSbaseMVA};

   public:
                     __fastcall VTData(void) {};
      virtual        __fastcall ~VTData(void) {};
      virtual bool   __fastcall Inicia(double sbase) = 0;
      virtual bool   __fastcall Insere(smcBARRA      &barra) = 0;
      virtual bool   __fastcall Insere(smcCAPACITOR  &capacitor) = 0;
      virtual bool   __fastcall Insere(smcCAPSERIE   &capserie) = 0;
      virtual bool   __fastcall Insere(smcCARGA      &carga) = 0;
      virtual bool   __fastcall Insere(smcFILTRO     &filtro) = 0;
      virtual bool   __fastcall Insere(smcGERADOR    &gerador) = 0;
      virtual bool   __fastcall Insere(smcMUTUA      &mutua) = 0;
      virtual bool   __fastcall Insere(smcREATOR     &reator) = 0;
      virtual bool   __fastcall Insere(smcREGULADOR  &regulador) = 0;
      virtual bool   __fastcall Insere(smcSUPRIMENTO &suprimento) = 0;
      virtual bool   __fastcall Insere(smcTRAFO2E    &trafo2E) = 0;
      virtual bool   __fastcall Insere(smcTRAFO3E    &trafo3E) = 0;
      virtual bool   __fastcall Insere(smcTRAFOZZ    &trafoZZ) = 0;
      virtual bool   __fastcall Insere(smcTRECHO     &trecho) = 0;
      virtual bool   __fastcall Insere(smcYREF       &yref) = 0;
      virtual bool   __fastcall Insere(smcZREF       &zref) = 0;
      virtual TList* __fastcall LisBarra(void)   = 0;
      virtual TList* __fastcall LisLigacao(void) = 0;
      virtual bool   __fastcall Resultado(smcBARRA      &barra) = 0;
      virtual bool   __fastcall Resultado(smcGERADOR    &gerador) = 0;
      virtual bool   __fastcall Resultado(smcSUPRIMENTO &suprimento) = 0;
      virtual bool   __fastcall Resultado(smcTRAFO2E    &trafo2E) = 0;
      virtual bool   __fastcall Resultado(smcTRECHO     &trecho) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTData
//---------------------------------------------------------------------------
VTData* __fastcall NewObjData(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


