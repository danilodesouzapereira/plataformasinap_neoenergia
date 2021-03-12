//-----------------------------------------------------------------------------
#ifndef VTMultiPanel_H
#define VTMultiPanel_H

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTMultiPanel  : public TObject
   {
  public:  //property
      __property TPanel *PanelAtivo    = {read=PM_GetPanelAtivo};
      __property TPanel *Panel1        = {read=PM_GetPanel1, write=PM_SetPanel1};
      __property TPanel *Panel2        = {read=PM_GetPanel2, write=PM_SetPanel2};
      __property bool   Panel2_Enabled = {write=PM_SetPanel2_Enabled};

   public:
                   __fastcall VTMultiPanel(void) {};
      virtual      __fastcall ~VTMultiPanel(void){};

   protected:  //métodos acessados via property
      virtual TPanel* __fastcall PM_GetPanelAtivo(void) = 0;
      virtual TPanel* __fastcall PM_GetPanel1(void) = 0;
      virtual TPanel* __fastcall PM_GetPanel2(void) = 0;
      virtual void    __fastcall PM_SetPanel1(TPanel *panel1) = 0;
      virtual void    __fastcall PM_SetPanel2(TPanel *panel2) = 0;
      virtual void    __fastcall PM_SetPanel2_Enabled(bool enabled) = 0;
    };

//-----------------------------------------------------------------------------
//função global
VTMultiPanel* __fastcall NewObjMultiPanel(void);

#endif
//---------------------------------------------------------------------------
//eof
