//---------------------------------------------------------------------------
#ifndef VTCfgFluxoH3H
#define VTCfgFluxoH3H

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTCfgFluxoH3 : public TObject
   {
   public:  //property
      __property bool GravaYnodal      = {read=PM_GetGravaYnodal, write=PM_SetGravaYnodal};
      __property int  NumeroHarmonicas = {read=PM_GetNumeroHarmonicas};

   public:
                             __fastcall  VTCfgFluxoH3(void) {};
      virtual                __fastcall ~VTCfgFluxoH3(void) {};
      virtual bool           __fastcall  InsereOrdemHarmonica(int ordem) = 0;
      virtual int            __fastcall  OrdemHarmonica(int indice) = 0;
      virtual void           __fastcall  Reinicia(void) = 0;

   protected:
      virtual bool           __fastcall  PM_GetGravaYnodal(void) = 0;
      virtual int            __fastcall  PM_GetNumeroHarmonicas(void) = 0;

      virtual void           __fastcall  PM_SetGravaYnodal(bool grava_ynodal) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCfgFluxoH3
//---------------------------------------------------------------------------
VTCfgFluxoH3 * __fastcall NewObjCfgFluxoH3(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


