//---------------------------------------------------------------------------
#ifndef TFormArranjoH
#define TFormArranjoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTArranjo;
class VTArranjos;
class VTApl;
class VTFases;

//---------------------------------------------------------------------------
class TFormArranjo : public TForm
{
__published:	// IDE-managed Components
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TGroupBox *GBoxLimite;
   TRadioGroup *RGroupTipo;
   TStringGrid *SGridZ0Z1;
   TLabel *Label2;
   TEdit *EditIadm;
   TGroupBox *GBoxSupCabo;
   TPanel *Panel1;
   TLabel *Label3;
   TPanel *PanelNeutro;
   TCheckBox *CheckBoxNeutro;
   TComboBox *CBoxCaboNeutro;
   TComboBox *CBoxPtoFixacaoNeutro;
   TPanel *PanelFaseC;
   TCheckBox *CheckBoxFaseC;
   TComboBox *CBoxCaboFaseC;
   TComboBox *CBoxPtoFixacaoFaseC;
   TPanel *PanelFaseB;
   TCheckBox *CheckBoxFaseB;
   TComboBox *CBoxCaboFaseB;
   TComboBox *CBoxPtoFixacaoFaseB;
   TPanel *PanelFaseA;
   TCheckBox *CheckBoxFaseA;
   TComboBox *CBoxCaboFaseA;
   TComboBox *CBoxPtoFixacaoFaseA;
   TPanel *Panel5;
   TComboBox *CBoxSupAereo;
   TLabel *Label4;
   TGroupBox *GBoxCaboZ0Z1;
   TComboBox *CBoxCaboZ0Z1;
   TAction *ActionAlteraTipoCfg;
   TAction *ActionAlteraFases;
   TAction *ActionAlteraSuporte;
   TAction *ActionAlteraCaboZ0Z1;
   TAction *ActionAlteraPtoFixacaoFase;
   TAction *ActionAlteraCaboFase;
   TLabel *Label6;
   TEdit *EditResTerra;
   TGroupBox *GBoxCodigo;
   TLabel *Label7;
   TEdit *EditCodigo;
   TLabel *Label1;
   TEdit *EditVmin;
   TEdit *EditVmax;
   TLabel *Label8;
   TGroupBox *GBoxSuporte;
   TComboBox *CBoxSuporte;
	TPanel *Panel2;
	TGroupBox *GroupBoxfase;
	TComboBox *CBoxFase;
   TPanel *Panel3;
	TPanel *Panel4;
	TGroupBox *GroupBox1;
	TCheckBox *CheckBoxRamal;
   TPageControl *PageControl;
   TTabSheet *TabSheetZ0Z1;
   TTabSheet *TabSheetMatZ;
   TTabSheet *TabSheetMatC;
   TStringGrid *SGridMatZ;
   TStringGrid *SGridMatC;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionAlteraTipoCfgExecute(TObject *Sender);
   void __fastcall ActionAlteraFasesExecute(TObject *Sender);
   void __fastcall ActionAlteraSuporteExecute(TObject *Sender);
   void __fastcall ActionAlteraCaboZ0Z1Execute(TObject *Sender);
   void __fastcall ActionAlteraCaboFaseExecute(TObject *Sender);
   void __fastcall ActionAlteraPtoFixacaoFaseExecute(TObject *Sender);
   void __fastcall CBoxCaboZ0Z1Change(TObject *Sender);

public:		// User declarations
   __fastcall TFormArranjo(TComponent* Owner, VTApl *apl, VTArranjo *arranjo);
   __fastcall ~TFormArranjo(void);

private:	//métodos
   bool       __fastcall CalculaMatZ(void);
	void       __fastcall ExibeDados(void);
   void       __fastcall ExibeDadosCaboZ0Z1(void);
   void       __fastcall ExibeDadosLimite(void);
   void       __fastcall ExibeDadosMatC(void);
   void       __fastcall ExibeDadosMatZ(void);
   void       __fastcall ExibeDadosSuporteCabo(void);
   void       __fastcall ExibeDadosZ0Z1(void);
   VTArranjo* __fastcall ExisteArranjo(AnsiString codigo);
   void       __fastcall IniciaCBoxCaboFase(void);
   void       __fastcall IniciaCBoxCaboZ0Z1(void);
   void       __fastcall IniciaCBoxFase(void);
   void       __fastcall IniciaCBoxPtoFixacao(void);
   void       __fastcall IniciaCBoxSupAereo(void);
   void       __fastcall IniciaCBoxSuporte(void);
   void       __fastcall LeCaboDasFases(void);
   void       __fastcall LeCaboZ0Z1(void);
   void       __fastcall LeCodigo(void);
   void       __fastcall LeDados(void);
   void       __fastcall LeDadosEditaveis(void);
   void       __fastcall LeFases(void);
   void       __fastcall LeLimites(void);
   void       __fastcall LeMatC(void);
   void       __fastcall LeMatZ(void);
	void       __fastcall LePtoFixacaoDasFases(void);
	void       __fastcall LeRamal(void);
	void       __fastcall LeResTerra(void);
   bool       __fastcall LeStrComplexo(AnsiString txt, double &r, double &x);
   void       __fastcall LeSuporte(void);
   void       __fastcall LeSuporteAereo(void);
   void       __fastcall LeTipoConfiguracao(void);
   void       __fastcall LeZ0Z1(void);
   bool       __fastcall ValidaCaboDasFases(void);
   bool       __fastcall ValidaCaboPreReunido(void);
   bool       __fastcall ValidaCodigo(void);
   bool       __fastcall ValidaDados(void);
   bool       __fastcall ValidaFases(void);
   bool       __fastcall ValidaLimites(void);
   bool       __fastcall ValidaMatZ(void);
   bool       __fastcall ValidaPtoFixacaoDasFases(void);
   bool       __fastcall ValidaResTerra(void);
   bool       __fastcall ValidaSuporte(void);
   bool       __fastcall ValidaTipoConfiguracao(void);
   bool       __fastcall ValidaZ0Z1(void);

private:	//objetos externos
   VTApl      *apl;
   VTArranjo  *arranjo_orig;
   VTArranjos *arranjos;
   VTFases    *fases;

private:	//dados locais
   VTArranjo *arranjo;
};

//---------------------------------------------------------------------------
#endif
//eof
