//---------------------------------------------------------------------------
#ifndef TMicroH
#define TMicroH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMicro.h"

//---------------------------------------------------------------------------
class TMicro : public VTMicro
   {
   public:
           __fastcall TMicro(void);
           __fastcall ~TMicro(void);

   private: //m�todos acessados via property
      AnsiString __fastcall PM_GetCPU_Id(void);
      AnsiString __fastcall PM_GetCPU_Vendor(void);

   private: //m�todos
      bool __fastcall CPU_DataAvailable(void);
      void __fastcall CPU_GetID(unsigned *cpu_id);
      void __fastcall CPU_GetVendor(char *vendor);
   };

#endif
//---------------------------------------------------------------------------
//eof

