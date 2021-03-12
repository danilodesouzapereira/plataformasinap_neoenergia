//---------------------------------------------------------------------------
#ifndef TTipoRedeH
#define TTipoRedeH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTTipoRede.h"

//---------------------------------------------------------------------------
class TTipoRede : public VTTipoRede
   {
   public:
                  __fastcall  TTipoRede(void);
                  __fastcall ~TTipoRede(void);
      VTTipoRede* __fastcall  Clone(void);
      void        __fastcall  CopiaAtributosDe(VTTipoRede &tiporede);

   protected: //métodos acessados via property
      TColor   __fastcall PM_GetColor(void);
      int      __fastcall PM_GetId(void);
      int      __fastcall PM_GetSegmento(void);
      bool     __fastcall PM_GetStatus(unsigned bit);
      unsigned __fastcall PM_GetStatusAsUnsigned(void);
      void     __fastcall PM_SetColor(TColor color);
      void     __fastcall PM_SetId(int tipo_id);
      void     __fastcall PM_SetSegmento(int seg_id);
      void     __fastcall PM_SetStatus(unsigned bit, bool enabled);
      void     __fastcall PM_SetStatusAsUnsigned(unsigned status);
   };

//---------------------------------------------------------------------------
#endif
//eof

