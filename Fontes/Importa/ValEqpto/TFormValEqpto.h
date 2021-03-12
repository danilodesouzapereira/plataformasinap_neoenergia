//---------------------------------------------------------------------------
#ifndef TFormValEqptoH
#define TFormValEqptoH

//---------------------------------------------------------------------------
class VTApl;
class VTParametro;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <System.Actions.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <System.ImageList.hpp>
//---------------------------------------------------------------------------
class TFormValEqpto : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar1;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionConfirma;
   TAction *ActionFecha;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
	TGroupBox *GroupBox1;
	TComboBox *CBoxTipoEqpto;
	TGroupBox *GroupBox2;
	TStringGrid *SGridParam;
	TComboBox *cbox1;
	TComboBox *cbox2;
	void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
	void __fastcall CBoxTipoEqptoChange(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormShow(TObject *Sender);

public:		// User declarations
				__fastcall  TFormValEqpto(TComponent* Owner, VTApl *apl_owner);
				__fastcall ~TFormValEqpto(void);

private:	// métodos
	void 				__fastcall CBoxInicia(void);
	void 		 		__fastcall Inicia(void);
	void 				__fastcall InsereSGrid(VTParametro *par, int row);
	void 				__fastcall InsereLisParam(void);
	void 				__fastcall InsereParametros(int tipo);
	void 				__fastcall LeValorSalvo(VTParametro *par);
	void 				__fastcall LimpaSGrid(void);
	void 			   __fastcall RedimensionaForm(void);
	void 				__fastcall SalvaDados(void);
	void 				__fastcall SalvaValores(void);


private: //externo
	VTApl *apl;


private: //objs locais
	bool   salvou;
	int	 tipo_anterior;
	TList *lisTipoParam;

};

//---------------------------------------------------------------------------
#endif
