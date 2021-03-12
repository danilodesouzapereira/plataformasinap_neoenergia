//---------------------------------------------------------------------------
#ifndef TFormEdtMedidorH
#define TFormEdtMedidorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <CheckLst.hpp>
#include <System.Actions.hpp>
#include <Vcl.Dialogs.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
//class TFormEdtPlan;
class TFormEdtCurvaMed;
class VTApl;
class VTBarra;
class VTEqpto;
class VTCurva;
class VTMedidor;
class VTMedicao;
class VTNet;

//---------------------------------------------------------------------------
class TFormEdtMedidor : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TActionList *ActionList;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TSplitter *Splitter;
   TGroupBox *GBoxCanal;
   TPanel *Panel1;
   TGroupBox *GBoxMedidor;
	TGroupBox *GBoxMedidores;
   TEdit *EditCodigo;
	TToolButton *ToolButton1;
	TAction *ActionImportaCurva;
	TOpenDialog *OpenDialog;
	TImageList *ImageList1;
	TGroupBox *GBoxEqptoCanal;
	TListBox *LBoxEqpto;
	TCheckListBox *CLBoxCanal;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
	void __fastcall ActionImportaCurvaExecute(TObject *Sender);
	void __fastcall LBoxEqptoClick(TObject *Sender);
	void __fastcall CLBoxCanalClick(TObject *Sender);
	void __fastcall CLBoxCanalClickCheck(TObject *Sender);

public:		// métodos
        __fastcall  TFormEdtMedidor(TComponent* Owner, VTApl *apl, VTMedidor* filtro);
        __fastcall ~TFormEdtMedidor(void);
   void __fastcall  OpcaoRetira(bool enabled);

private:    //métodos
   void     __fastcall AtualizaCLBoxCanal(void);
	void     __fastcall AtualizaCurvas(void);
   void     __fastcall CLBoxCanalInicia(VTEqpto *eqpto);
   void     __fastcall CriaCanal(VTEqpto *eqpto, int tipo_canal, int fases);
   void     __fastcall CriaCanais(TList *lisEQP);
   void     __fastcall CriaCanaisBarra(VTEqpto *eqpto);
   void     __fastcall CriaCanaisCarga(VTEqpto *eqpto);
   void     __fastcall CriaCanaisGerador(VTEqpto *eqpto);
   void     __fastcall CriaCanaisLigacao(VTEqpto *eqpto);
   void     __fastcall CriaCanaisSuprimento(VTEqpto *eqpto);
   void     __fastcall CriaFormEdtCurvaMedicao(void);
	void     __fastcall ExibeDadosCanal(void);
   void     __fastcall FormIniciaPosicao(void);
	int      __fastcall FasesDaBarra(VTBarra *barra);
	bool     __fastcall FasesDeTodasBarras(void);
   void     __fastcall IniciaLisEqptoComMedicao(TList *lisEQP);
   void     __fastcall LBoxEqptoInicia(TList* lisEQP);
   bool     __fastcall LeDadosCanal(void);
   bool     __fastcall LeDadosMedidor(void);
   void     __fastcall LimpaCanalTemporario(void);

private: //objetos externos
   VTApl     *apl;
   VTMedidor *medidor;

private: //dados locais
   TList            *lisEQP;
   TList            *lisFORM;
	TFormEdtCurvaMed *FormEdtCurvaMed;
	VTNet				  *net;

};

#endif
//---------------------------------------------------------------------------
//eof
