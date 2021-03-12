//---------------------------------------------------------------------------
#ifndef TCtrlPanel_H
#define TCtrlPanel_H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCtrlPanel.h"

//---------------------------------------------------------------------------
class VTRegistro;

//---------------------------------------------------------------------------

class TCtrlPanel : public VTCtrlPanel
   {
   public:
           __fastcall TCtrlPanel(void);
           __fastcall ~TCtrlPanel(void);
      bool __fastcall  DefineSeperadorDecimal(AnsiString dec_separator);
      bool __fastcall  RestauraSeperadorDecimal(void);
      bool __fastcall  ValidaSeparadorDecimal(AnsiString dec_separator);

   private: //dados locais
      VTRegistro *registro;
      struct   {
               AnsiString  dec_separator;
               }ctrlpanel;
      
   };

#endif
//-----------------------------------------------------------------------------
// eof


