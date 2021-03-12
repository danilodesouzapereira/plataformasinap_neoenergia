//---------------------------------------------------------------------------
#ifndef TFormEdtTreH
#define TFormEdtTreH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <Menus.hpp>
#include <ImgList.hpp>
#include "..\Rede\VTLigacao.h"
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTArranjo;
class VTGeral;
class VTMontaZ;
class VTTrecho;
class VTRede;

//---------------------------------------------------------------------------
class TFormEdtTre : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *gboxTrecho;
   TLabel *Label1;
   TEdit *EditCodigo;
   TLabel *Label4;
   TComboBox *cboxRede;
   TToolBar *ToolBar2;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TToolButton *ToolButton3;
   TImageList *ImageList1;
   TActionList *ActionList2;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TLabel *LabelComp;
   TEdit *edtComp;
   TRadioGroup *RGroupArranjo;
   TGroupBox *GBoxArranjoProprio;
   TGroupBox *GBoxArranjoTipico;
   TComboBox *CBoxArranjo;
   TLabel *Label2;
   TEdit *EditIadm;
   TRadioGroup *RGroupTipoCfg;
   TGroupBox *GBoxZseq;
   TStringGrid *SGridZseq;
   TPanel *Panelfases;
   TLabel *Label3;
   TComboBox *CBoxFases;
   TGroupBox *GBoxDivisao;
   TLabel *Label5;
   TEdit *EditDistBarRef;
   TCheckBox *CheckBoxDivisao;
   TLabel *Label9;
   TComboBox *CBoxBarRef;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
   TPanel *Panel5;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionCancela1Execute(TObject *Sender);
   void __fastcall ActionConfirma1Execute(TObject *Sender);
   void __fastcall ActionRetira1Execute(TObject *Sender);
   void __fastcall RGroupTipoCfgClick(TObject *Sender);
   void __fastcall RGroupArranjoClick(TObject *Sender);
   void __fastcall CheckBoxDivisaoClick(TObject *Sender);

public:		// User declarations
			__fastcall  TFormEdtTre(TComponent* Owner, VTApl *apl, VTTrecho *ptrecho, bool op_retira);
            __fastcall ~TFormEdtTre(void);
   VTBarra* __fastcall  ExisteDivisao(double &dist_m);

private: //métodos
	void    __fastcall CBoxArranjoInicia(void);
   void    __fastcall CBoxBarraInicia(void);
	void    __fastcall CBoxFasesInicia(void);
	void    __fastcall DefValoresDefaultTrecho(void);
	void    __fastcall ExibeDadosArranjoProprio(void);
	void    __fastcall ExibeDadosTrecho(void);
	void    __fastcall FormIniciaPosicao(void);
	void    __fastcall FormAjustaPosicao(void);
	void    __fastcall IniciaCBoxRede(void);
	bool    __fastcall LeComprimento(void);
	bool    __fastcall LeCorrenteAdmissivel(void);
	bool    __fastcall LeDadosArranjoProprio(void);
	bool    __fastcall LeDadosArranjoTipico(void);
   bool    __fastcall LeDadosDivisao(void);
	bool    __fastcall LeDadosTrecho(void);
	void    __fastcall LimpaSGrid(void);
   void    __fastcall Moldura(VTEqpto *eqpto=NULL);
	VTRede* __fastcall SelecionaRede(TList *lisREDE);
	bool    __fastcall TrechoComCoordenadaUtm(void);
	bool    __fastcall ValidaDadosArranjoProprio(void);
	bool    __fastcall ValidaDadosTrecho(void);

private: //dados externos
	VTApl    *apl;
	VTGeral  *geral;
	VTTrecho *ptrecho;

private: //dados locais
	VTArranjo *arranjo;
	VTMontaZ  *montaZ;
	TList     *lisEQP;
	TList     *lisLIG;
   struct   {
            VTBarra *bar_ref;
            double   dist_m;
            }divisao;
};

#endif
//---------------------------------------------------------------------------
//eof

