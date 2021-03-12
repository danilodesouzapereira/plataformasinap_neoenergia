//---------------------------------------------------------------------------
#ifndef TFormEdtPolylineH
#define TFormEdtPolylineH

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
#include <Mask.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
//#include "..\Rede\VTLigacao.h"

//---------------------------------------------------------------------------
class VTApl;
class VTArranjo;
class VTBarra;
class VTGeral;
class VTTrecho;
class VTRede;
class TEdtPolyline;

//---------------------------------------------------------------------------
class TFormEdtPolyline : public TForm
{
__published:   // IDE-managed Components
   TGroupBox *GroupBoxTrecho;
   TLabel *LabelCodigo;
   TLabel *LabelRede;
   TComboBox *CBoxRede;
   TToolBar *ToolBar2;
   TToolButton *ToolButton1;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionAltera;
   TLabel *LabelComp;
   TComboBox *CBoxArranjo;
   TStringGrid *SGrid;
   TLabel *LabelArranjo;
   TAction *ActionAlignNone;
   TGroupBox *GroupBoxBarra;
   TLabel *Label2;
   TLabel *Label5;
   TPageControl *PageControl;
   TTabSheet *TabSheetConfigura;
   TTabSheet *TabSheetTrechos;
   TGroupBox *GroupBoxMouse;
   TCheckBox *CheckBoxRastro;
   TRichEdit *EditCodigoTrecho;
   TRichEdit *EditCodigoBarra;
   TRichEdit *EditVnom;
   TRichEdit *EditComprimento;
   TAction *ActionNovaSequencia;
   TToolBar *ToolBar1;
   TToolButton *ButFecha;
   TToolButton *ButNovaSequencia;
   TToolButton *ToolButton2;
   TAction *ActionFecha;
   TLabel *LabelAviso;
   TPanel *PanelCodigo;
   TPanel *PanelRede;
   TPanel *PanelArranjo;
   TPanel *PanelComp;
   TPanel *Panel1;
   TPanel *Panel2;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionAlteraExecute(TObject *Sender);
   void __fastcall ActionAlignNoneExecute(TObject *Sender);
   void __fastcall ActionNovaSequenciaExecute(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);

public:      //propoperty
   __property bool       ExibirRastro   = {read=PM_GetExibirRastro};
   __property double     CodigoBarra    = {read=PM_GetCodigoBarra};
   __property double     CodigoTrecho   = {read=PM_GetCodigoTrecho};
   __property double     Comprimento_km = {read=PM_GetComprimento_km};
   __property double     Vnom_kv        = {read=PM_GetVnom_kv};
   __property VTRede*    Rede           = {read=PM_GetRede};
   __property VTArranjo* Arranjo        = {read=PM_GetArranjo};

public:      // User declarations
             __fastcall  TFormEdtPolyline(TComponent* Owner, VTApl *apl,
                                          TEdtPolyline *EdtPolyline, TWinControl *parent);
             __fastcall ~TFormEdtPolyline(void);
   void      __fastcall  InsereBarra(VTBarra *barra);
   void      __fastcall  InsereTrecho(VTTrecho *trecho);
   TList*    __fastcall  LisBarra(void);
   TList*    __fastcall  LisTrecho(void);

private: //métodos acesssados via property
   VTArranjo* __fastcall PM_GetArranjo(void);
   AnsiString __fastcall PM_GetCodigoBarra(void);
   AnsiString __fastcall PM_GetCodigoTrecho(void);
   double     __fastcall PM_GetComprimento_km(void);
   bool       __fastcall PM_GetExibirRastro(void);
   VTRede*    __fastcall PM_GetRede(void);
   double     __fastcall PM_GetVnom_kv(void);

private: //métodos
   void __fastcall CBoxArranjoInicia(void);
   void __fastcall CBoxRedeInicia(void);
   void __fastcall FormIniciaPosicao(void);
   void __fastcall SGridInicia(void);
   void __fastcall SGridLedados(void);

private: //dados externos
   TEdtPolyline *EdtPolyline;
   VTApl        *apl;
   VTGeral      *geral;

private: //dados locais
   TList   *lisBARRA;
   TList   *lisTRECHO;
   struct   {//coordenadas p/ janela flutuante
            int top;
            int left;
            int width;
            int height;
            } janela;
};

#endif
//---------------------------------------------------------------------------
//eof

