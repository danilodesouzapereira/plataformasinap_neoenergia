//-----------------------------------------------------------------------------
#ifndef VTAgregada_H
#define VTAgregada_H

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;
class VTResumo;

//-----------------------------------------------------------------------------
class VTAgregada : public TObject
   {
   public: //property
      __property bool Valida = {read=PM_GetValida};

   public:
                        __fastcall  VTAgregada(void) {};
                        __fastcall ~VTAgregada(void) {};
      virtual void      __fastcall  Inicia(void) = 0;
      virtual TList*    __fastcall  LisSegmentoRaiz(void) = 0;
      virtual VTResumo* __fastcall  ResumoTotal(void) = 0;

   protected:
      virtual bool __fastcall  PM_GetValida(void) = 0;
    };

//-----------------------------------------------------------------------------
//função global
VTAgregada* __fastcall NewObjAgregada(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof
