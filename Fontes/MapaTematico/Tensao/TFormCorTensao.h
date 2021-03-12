//---------------------------------------------------------------------------
#ifndef TFormCorTensaoH
#define TFormCorTensaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTCfgCor;

//---------------------------------------------------------------------------
class TFormCorTensao : public TForm
{
__published:   // IDE-managed Components
   TLabel *lblV1;
   TLabel *lblV2;
   TPanel *PanCorV1;
   TPanel *PanCorV2;
   TColorDialog *ColorDialog;
   TPanel *PanCorV3;
   TPanel *PanCorV4;
   TPanel *PanCorV5;
   TLabel *lblV3;
   TLabel *lblV5;
   TLabel *lblV4;
	TPanel *pnlV1;
	TPanel *pnlV2;
	TPanel *pnlV3;
	TPanel *pnlV4;
	TPanel *pnlV5;
	TTabControl *tabControlNivelV;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall PanCorV1Click(TObject *Sender);
   void __fastcall PanCorV2Click(TObject *Sender);
   void __fastcall PanCorV3Click(TObject *Sender);
   void __fastcall PanCorV4Click(TObject *Sender);
   void __fastcall PanCorV5Click(TObject *Sender);
	void __fastcall tabControlNivelVChange(TObject *Sender);

public:      // User declarations
   __fastcall TFormCorTensao(TComponent *Owner, VTApl *apl_owner, TAction *Action);
   __fastcall ~TFormCorTensao(void);

private:   //métodos
   void __fastcall ExibeDados(int tipoSegmento);
   void __fastcall IniciaCor(void);

private: //objetos externos
   AnsiString arq_infoset;
   VTApl     *apl;
   VTCfgCor  *cfg_cor;
   TAction   *ActionAtualiza;

};

//---------------------------------------------------------------------------
#endif
//eof

