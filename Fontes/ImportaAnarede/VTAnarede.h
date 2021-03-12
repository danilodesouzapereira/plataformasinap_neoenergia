//-----------------------------------------------------------------------------
#ifndef VTAnaredeH
#define VTAnaredeH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTAnarede : public TObject
   {
   public:
                   __fastcall VTAnarede(void) {};
      virtual      __fastcall ~VTAnarede(void) {};
      virtual bool __fastcall Executa(TStrings *Lines, int num_area=0) = 0;
   };

//-----------------------------------------------------------------------------
// fun��o global para criar objeto VTAnarede
//-----------------------------------------------------------------------------
VTAnarede* NewObjAnarede(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
