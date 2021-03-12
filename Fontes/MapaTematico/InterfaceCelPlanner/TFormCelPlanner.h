//---------------------------------------------------------------------------
#ifndef TFormCelPlannerH
#define TFormCelPlannerH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Mask.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTInterfaceCelPlanner;
class VTRedes;

//---------------------------------------------------------------------------
class TFormCelPlanner : public TForm
{
__published:	// IDE-managed Components
   TRichEdit *RichEdit;
   TToolBar *ToolBar1;
	TToolButton *btnCreateData;
   TToolButton *ToolButton2;
   TActionList *ActionList;
   TImageList *ImageList;
   TAction *ActionGeraDados;
   TAction *ActionSalvaArquivo;
	TToolButton *btnSaveFile;
   TAction *ActionHelp;
   TSaveDialog *SaveDialog;
   TPanel *Panel1;
   TGroupBox *gbCargas;
   TGroupBox *gbGeracao;
   TGroupBox *gbChaves;
   TCheckBox *checkCargas;
   TCheckBox *checkGeradores;
   TCheckBox *checkChaves;
   TGroupBox *gbConfig;
	TComboBox *cboxFeeder;
   TPanel *panelRedes;
	TLabel *lblFeeder;
   TRadioGroup *radioCoordenadas;
   TCheckBox *checkReligadores;
   TGroupBox *gbCapacitores;
   TCheckBox *checkCapacitores;
   TGroupBox *gbReguladores;
   TCheckBox *checkReguladores;
   TGroupBox *gbTipoChave;
   TCheckBox *checkSeccionadoras;
   TGroupBox *gbSubestacao;
   TCheckBox *checkSubestacao;
   TPanel *Panel2;
   TPanel *Panel3;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   //void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall ActionGeraDadosExecute(TObject *Sender);
   void __fastcall ActionSalvaArquivoExecute(TObject *Sender);
   //void __fastcall clboxPatamarClick(TObject *Sender);
   //void __fastcall comboPatamarChange(TObject *Sender);
   //void __fastcall cboxIAlClick(TObject *Sender);
   //void __fastcall cboxFDbtClick(TObject *Sender);
   //void __fastcall cboxFDa4Click(TObject *Sender);
   //void __fastcall editFDbtMouseActivate(TObject *Sender, TMouseButton Button, TShiftState Shift,
   //       int X, int Y, int HitTest, TMouseActivate &MouseActivate);
   //void __fastcall editFDa4MouseActivate(TObject *Sender, TMouseButton Button, TShiftState Shift,
   //       int X, int Y, int HitTest, TMouseActivate &MouseActivate);

public:		// User declarations
   __fastcall  TFormCelPlanner(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormCelPlanner(void);

private:	//métodos
   void  __fastcall ConfiguraInterfaceCelPlanner(void);
   //void  __fastcall DesabilitaCombo(void);
   bool  __fastcall ExisteRedeBTEquivalente(void);
   //bool  __fastcall IniciaCBoxPatamar(void);
   bool  __fastcall IniciaForm(void);
   void  __fastcall PreencheTextoForm(void);
   //bool  __fastcall ValidaFatorDemandaA4(void);
   //bool  __fastcall ValidaFatorDemandaTrafo(void);
   //bool  __fastcall ValidaInputForm(void);

private:	//objetos externos
   VTApl *apl;

private:	//dados
   VTInterfaceCelPlanner *celplanner;
   //TList          *lisCARREGADAS;
   //int            index_patamar;
   //int            index_pat_usado;
   //double         fdA4, fdTrafo;
   #define IS_ENGLISH 0
   enum strFORMEXPCELPLAN
	{
		sfcpTITLE =0,
		sfcpCREATEDATA,
		sfcpSAVETOFILE,
		sfcpSETTINGS,
		sfcpSELFEEDER,
		sfcpALL,
		sfcpCOORDSYS,
		sfcpEXPORT,
		sfcpLOAD,
		sfcpSWITCHES,
		sfcpSWITCHTYPES,
		sfcpSUBS,
		sfcpGENS,
		sfcpCAPS,
		sfcpADD,
		sfcpRECLOSERS,
		sfcpSWITCHGEARS,
		sfcpVOLTAGEREG,
		sfcpCOUNT
	};
};

//---------------------------------------------------------------------------
#endif
//eof

