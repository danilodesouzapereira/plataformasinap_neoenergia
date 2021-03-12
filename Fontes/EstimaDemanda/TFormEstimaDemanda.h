//---------------------------------------------------------------------------
#ifndef TFormEstimaDemandaH
#define TFormEstimaDemandaH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "TFormEdt.h"
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCronometro;
class VTImportaMedicao;
class VTMedicoes;
class VTEqpto;
class VTRedes;

//---------------------------------------------------------------------------
class TFormEstimaDemanda : public TFormEdt
{
__published:	// IDE-managed Components
   TToolButton *ButEdtMedicao;
   TOpenTextFileDialog *OpenTextFileDialog;
   TAction *ActionFecha;
   TAction *ActionHelp;
   TAction *ActionImportaMedicao;
   TAction *ActionExecuta;
   TAction *ActionRedeChanged;
   TAction *ActionEdtMedicao;
   TToolBar *ToolBar1;
   TToolButton *ButFecha;
   TToolButton *ButImportaMedicao;
   TToolButton *ToolButton3;
   TToolButton *ToolButton2;
   TToolButton *ButHelp;
   TToolButton *ToolButton1;
   TGroupBox *GBoxMedicao;
   TListView *LView;
   TRadioGroup *RadioGroupRelatorio;
   TRadioGroup *RadioGroupEstimacao;
   TPanel *PanelCfg;
   TToolButton *ButTeste;
   TAction *ActionTeste_Redutor;
   TRadioGroup *RadioGroupRede;
   TAction *ActionTeste_CriaMedicaoPQ;
   TRadioGroup *RadioGroupReducao;
   TPopupMenu *PopupMenuTeste;
   TMenuItem *MenuItemRedutor;
   TMenuItem *MenuItemMedicaoPQ;
   TAction *ActionTeste_Reticulado;
   TMenuItem *MenuItemReticulado;
   TGroupBox *GBoxPatamar;
   TComboBox *CBoxPatamar;
   TAction *ActionTeste_CriaMedicaoI;
   TMenuItem *MenuItemMedicaoI;
   TAction *ActionTeste_CriaMedicaoV;
   TMenuItem *MenuItemMedicaoV;
   TAction *ActionTeste_RetiraMedicao;
   TMenuItem *N1;
   TMenuItem *MenuItemRetiraMedicao;
   TCheckBox *CheckBoxHorasAfetadas;
   TAction *ActionTeste_ZeraCarga;
   TMenuItem *MenuItemZeraCarga;
   TRadioGroup *RadioGroupMetodoEE;
   TAction *ActionTeste_RetiraTodasChave;
   TMenuItem *MenuItemRetiraTodasChaves;
   TAction *ActionTeste_RedeWise;
   TMenuItem *N2;
   TMenuItem *MenuItem_RedeWise;
   TAction *ActionTeste_RetiraChaveSemMedicao;
   TMenuItem *MenuItemRetiraChaveSemMedicao;
   TAction *ActionTeste_CriaMedicaoI_ChaveRA;
   TMenuItem *ESTECriarMedioCorrenteChavesRA1;
   TGroupBox *GBoxFatMult;
   TEdit *EditFatMultMedChave;
   TAction *ActionTeste_RedeZip;
   TMenuItem *GerarRedeZIP1;
   TGroupBox *GBoxVDNM;
   TCheckBox *CheckBoxVDNM;
   TEdit *EdtNumMaxMontH;
   TLabel *LabelNumMaxMontH;
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall ActionImportaMedicaoExecute(TObject *Sender);
   void __fastcall ActionExecutaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionEdtExecute(TObject *Sender);
   void __fastcall ActionRedeChangedExecute(TObject *Sender);
   void __fastcall ActionEdtMedicaoExecute(TObject *Sender);
   void __fastcall ActionTeste_RedutorExecute(TObject *Sender);
   void __fastcall ActionTeste_CriaMedicaoPQExecute(TObject *Sender);
   void __fastcall ActionTeste_ReticuladoExecute(TObject *Sender);
   void __fastcall ActionTeste_CriaMedicaoIExecute(TObject *Sender);
   void __fastcall ActionTeste_CriaMedicaoVExecute(TObject *Sender);
   void __fastcall ActionTeste_RetiraMedicaoExecute(TObject *Sender);
   void __fastcall ActionTeste_ZeraCargaExecute(TObject *Sender);
   void __fastcall ActionTeste_RetiraTodasChaveExecute(TObject *Sender);
   void __fastcall ActionTeste_RedeWiseExecute(TObject *Sender);
   void __fastcall ActionTeste_RetiraChaveSemMedicaoExecute(TObject *Sender);
   void __fastcall ActionTeste_CriaMedicaoI_ChaveRAExecute(TObject *Sender);
   void __fastcall ActionTeste_RedeZipExecute(TObject *Sender);
	void __fastcall LViewClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);

public:		// User declarations
   __fastcall  TFormEstimaDemanda(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormEstimaDemanda(void);

private:	//métodos
   void  __fastcall CBoxPatamarInicia(void);
   bool  __fastcall CopiaRedes1ParaRedes2(VTRedes *redes1, VTRedes *redes2);
   void  __fastcall DuplicaRedesOriginal(void);
   bool  __fastcall EstimaDemandaPorRede(void);
   bool  __fastcall EstimaDemandaRedeUnica(void);
   void  __fastcall ExibeMedidores(bool visible);
   bool  __fastcall ExisteMedidor(TList *lisLIG);
   bool  __fastcall ExisteReticulado(void);
   bool  __fastcall ImportaMedicao(void);
   int   __fastcall LicencaCopel(void);
   bool  __fastcall LicencaMaster(void);
   void  __fastcall LViewInicia(void);
   void  __fastcall Moldura(VTEqpto *eqpto);
   void  __fastcall RestauraRedesOriginal(void);
   bool  __fastcall SelecionaArquivo(void);
   bool  __fastcall ValidaMedidoresSuprimentos(void);
   // bool  __fastcall TESTE_GravaArquivoCarga(AnsiString filename);

private:	//objetos externos
   VTApl *apl;

private:	//dados
   double           FatMultMedChave;
   bool             estima_executado;
   TForm            *FormEqv;
   TForm            *FormEqvWise;
   VTCronometro     *cronometro;
   VTImportaMedicao *importa_medicao;
   VTMedicoes       *medicoes;
   TStringList      *files;
   TList            *lisEQP;
   VTRedes          *redes_original;
};

//---------------------------------------------------------------------------
#endif
//eof

