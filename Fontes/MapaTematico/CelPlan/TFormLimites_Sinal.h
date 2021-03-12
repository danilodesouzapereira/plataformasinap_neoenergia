//---------------------------------------------------------------------------
#ifndef TFormLimites_SinalH
#define TFormLimites_SinalH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Grids.hpp>
#include <System.Actions.hpp>
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormLimites_Sinal : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TToolButton *ButFechar;
	TActionList *ActionList;
	TAction *ActionFecha;
	TImageList *ImageList;
	TToolButton *ButConfirmar;
   TStringGrid *StringGrid;
	TAction *ActionConfirma;
   TToolButton *ButAdic;
   TAction *ActionAdiciona;
   TAction *ActionRetira;
   TToolButton *ButRet;
   TToolButton *ToolButton1;

	void __fastcall ActionFechaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionAdicionaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
//   void __fastcall RadioGroupUnidadeClick(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);

public:		// User declarations
	__fastcall  TFormLimites_Sinal(TComponent* Owner, VTApl *apl, TAction *ActionRemontaMapaPadrao);
	__fastcall ~TFormLimites_Sinal(void);

private:	// User declarations
	bool __fastcall EditaLimites(void);
   void __fastcall HabilitaInsereRetiraFaixa(void);
   void  __fastcall PreencheTextoForm(void);
   bool __fastcall StringGridCellToDouble(int col, int row, double &valor);
//   void __fastcall StringGridInicia(void);
   void __fastcall StringGridIniciadBm(void);
   //void __fastcall StringGridIniciaString(void);
   bool __fastcall StringGridLeValores(bool so_validar);

private:   //objetos externos
	VTApl   *apl;
	TAction *ActionRemontaMapaPadrao;

private: //dados locais
   struct{
         int min;
         int max;
         int enabled;
         }num_faixa;
   AnsiString codigo;

   #define IS_ENGLISH 0
   enum strFORMLIMITSCELPLAN
	{
		sflcpTITLE =0,
		sflcpCLOSE,
		sflcpACCEPT,
		sflcpCOUNT
	};
};

//---------------------------------------------------------------------------
#endif
//eof

