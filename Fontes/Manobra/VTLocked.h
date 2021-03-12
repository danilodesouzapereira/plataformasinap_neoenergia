//---------------------------------------------------------------------------
#ifndef VTLockedH
#define VTLockedH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTChave;

//---------------------------------------------------------------------------
class VTLocked : public TObject
   {
   public:
      __fastcall  VTLocked(void) {};
      __fastcall ~VTLocked(void) {};
      
   public:
		VTChave *chave;
      int      estado;
      int      estado_original;
   };
   
#endif
//---------------------------------------------------------------------------
//eof
