//-----------------------------------------------------------------------------
#ifndef VTCtrlPanel_H
#define VTCtrlPanel_H

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTCtrlPanel : public TObject
   {

   public:
                   __fastcall  VTCtrlPanel(void) {};
      virtual      __fastcall ~VTCtrlPanel(void) {};
      virtual bool __fastcall  DefineSeperadorDecimal(AnsiString dec_separator) = 0;
      virtual bool __fastcall  RestauraSeperadorDecimal(void) = 0;
      virtual bool __fastcall  ValidaSeparadorDecimal(AnsiString dec_separator) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTCtrlPanel
//-----------------------------------------------------------------------------
VTCtrlPanel* NewObjCtrlPanel(void);

#endif
//-----------------------------------------------------------------------------
// eof
