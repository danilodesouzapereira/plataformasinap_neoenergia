//---------------------------------------------------------------------------
#ifndef TFormFasometroH
#define TFormFasometroH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TFORMEDT.h"
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <CheckLst.hpp>
#include "TFormEdt.h"
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEdt;
class VTEqpto;
class VTFasores;

//---------------------------------------------------------------------------
class TFormFasometro : public TFormEdt
{
__published:	// IDE-managed Components
   TAction *ActionRedeChanged;
   TAction *ActionSelEqpto;
   TImage *ImageFasor;
   TComboBox *CBoxPatamar;
   TTimer *Timer;
   TToolButton *butAnimacao;
   TAction *ActionAnimacao;
   TCheckListBox *CLBoxFasor;
   TSplitter *Splitter;
   TAction *ActionHelp;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TPanel *Panel1;
   TListView *lvValores;
   TToolButton *ToolButton3;
   void __fastcall ActionRedeChangedExecute(TObject *Sender);
   void __fastcall ActionSelEqptoExecute(TObject *Sender);
   void __fastcall ActionEdtExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall CBoxPatamarChange(TObject *Sender);
   void __fastcall TimerTimer(TObject *Sender);
   void __fastcall ActionAnimacaoExecute(TObject *Sender);
   void __fastcall CLBoxFasorDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
   void __fastcall CLBoxFasorClickCheck(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);

public:		// User declarations
        __fastcall TFormFasometro(TForm *form_owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormFasometro(void);
   void __fastcall Ativo(void);
   void __fastcall Atualiza(void);

private:	//métodos
   AnsiString __fastcall AvaliaPrefixo(double *valor, double prefixo);
   void       __fastcall DrawCircunferencia(int x, int y, int raio, TColor cor);
   void       __fastcall DrawFasor(void);
   void       __fastcall DrawSeta(int x1, int y1, int x2, int y2, TColor cor);
	void       __fastcall FormIniciaPosicao(void);
	void       __fastcall FormIniciaPropriedades(void);
   void       __fastcall IniciaCBoxPatamar(void);
   void       __fastcall IniciaCLBoxFasor(void);
   void       __fastcall LimpaImageFasor(TColor color);
   void       __fastcall PreencheListView(void);

private:	//objetos externos
   VTEqpto *eqpto;

private:	//dados locais
   int      ind_trecho_fic;   //índice do Trecho fictício a ser exibido no lugar do Trecho de defeito
   double    escala;   //pu/pixel
   VTApl     *apl;
   VTEdt     *edt;
   VTFasores *fasores;
   //memória alocada p/ TImage
   Graphics::TBitmap *graph_bmp;
};

//---------------------------------------------------------------------------
#endif
//eof
