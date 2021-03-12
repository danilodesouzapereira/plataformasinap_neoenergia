//---------------------------------------------------------------------------
#ifndef VTEnvoltoriaH
#define VTEnvoltoriaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTPtoCarga;

//---------------------------------------------------------------------------
class VTEnvoltoria : public TObject
   {
   public:
                     __fastcall  VTEnvoltoria(void) {};
                     __fastcall ~VTEnvoltoria(void) {};
      virtual TList* __fastcall Executa(TList *lisPTC) = 0;
   };

//---------------------------------------------------------------------------
VTEnvoltoria* __fastcall NewObjEnvoltoria(void);

#endif
//---------------------------------------------------------------------------
//eof

