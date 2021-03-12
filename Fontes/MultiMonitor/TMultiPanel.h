//---------------------------------------------------------------------------
#ifndef TMultiPanelH
#define TMultiPanelH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMultiPanel.h"

//---------------------------------------------------------------------------
class TMultiPanel : public VTMultiPanel
   {
   private: //dados acessados via property
      struct   {
               TPanel *panel1;
               TPanel *panel2;
               bool    panel2_enabled;
               }PD;

   private: //dados locais

   public:  //métodos
            __fastcall  TMultiPanel(void);
            __fastcall ~TMultiPanel(void);

   protected:  //métodos acessados via property
      TPanel* __fastcall PM_GetPanelAtivo(void);
      TPanel* __fastcall PM_GetPanel1(void);
      TPanel* __fastcall PM_GetPanel2(void);
      void    __fastcall PM_SetPanel1(TPanel *panel1);
      void    __fastcall PM_SetPanel2(TPanel *panel2);
      void    __fastcall PM_SetPanel2_Enabled(bool enabled);

   private: //métodos
      bool __fastcall ObjectTipoForm(TObject *obj);
      void __fastcall TransfereForms(TPanel *panel_orig, TPanel *panel_dest);
   };

//---------------------------------------------------------------------------
#endif
//eof
