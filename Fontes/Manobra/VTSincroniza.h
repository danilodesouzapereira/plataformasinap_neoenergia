//---------------------------------------------------------------------------
#ifndef VTSincronizaH
#define VTSincronizaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTChave;

//---------------------------------------------------------------------------
class VTSincroniza : public TObject
   {
   public:
                     __fastcall VTSincroniza(void) {};
      virtual        __fastcall ~VTSincroniza(void) {};
      virtual TList* __fastcall Abre(VTChave *chave) = 0;
      virtual void   __fastcall Disable(void) = 0;
      virtual TList* __fastcall Enable(void) = 0;
      virtual TList* __fastcall Fecha(VTChave *chave) = 0;
      virtual TList* __fastcall Manobra(VTChave *chave) = 0;
      virtual TList* __fastcall Manobra(TList *lisEXT) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTSincroniza* __fastcall NewObjSincroniza(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
