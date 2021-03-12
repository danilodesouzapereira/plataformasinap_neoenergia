//-----------------------------------------------------------------------------
#ifndef VTCtrlPanel_H
#define VTCtrlPanel_H

//arquivos inclu�dos-----------------------------------------------------------
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
// fun��o global para criar objeto VTCtrlPanel
//-----------------------------------------------------------------------------
VTCtrlPanel* NewObjCtrlPanel(void);

#endif
//-----------------------------------------------------------------------------
// eof
