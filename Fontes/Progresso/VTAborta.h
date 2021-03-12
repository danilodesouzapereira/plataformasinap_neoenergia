//-----------------------------------------------------------------------------
#ifndef VTAbortaH
#define VTAbortaH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTAborta : public TObject
   {
   public: //property
      __property bool Enabled  = {read=PD.enabled, write=PD.enabled};
      __property bool Requested = {read=PD.requested,    write=PD.requested};

   public:
      __fastcall  VTAborta(void) {};
      __fastcall ~VTAborta(void) {};

   public:
      struct{
            bool enabled;
            bool requested;
            }PD;
   };

//-----------------------------------------------------------------------------
//VTAborta* __fastcall NewObjAborta(VTApl *apl_owner);

#endif
//-----------------------------------------------------------------------------
// eof
