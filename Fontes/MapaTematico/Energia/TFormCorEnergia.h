//---------------------------------------------------------------------------
#ifndef TFormCorEnergiaH
#define TFormCorEnergiaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTCfgCor;

//---------------------------------------------------------------------------
class TFormCorEnergia : public TForm
{
__published:   // IDE-managed Components
   TLabel *lblCor;
   TColorDialog *ColorDialog;
   TPanel *PanCor;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall PanCorClick(TObject *Sender);

public:      // User declarations
   __fastcall  TFormCorEnergia(TComponent *Owner, VTApl *apl_owner, TAction *Action);
   __fastcall ~TFormCorEnergia(void);

private:   //métodos
   void __fastcall ExibeDados(void);
   void __fastcall IniciaCor(void);

private:   //objetos externos
   AnsiString arq_infoset;
   VTApl     *apl;
   VTCfgCor  *cfg_cor;
   TAction   *ActionAtualiza;

};

//---------------------------------------------------------------------------
#endif
