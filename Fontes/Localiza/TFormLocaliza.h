//---------------------------------------------------------------------------
#ifndef TFormLocalizaH
#define TFormLocalizaH

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
#include "TFormEdt.h"
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEdt;
class VTEqpto;
class VTGrafico;
class VTRede;
class VTRedes;
class VTConsulta;
class VTLegenda;
class VTLocaliza;
class TAtributo;;

//---------------------------------------------------------------------------
class TFormLocaliza : public TFormEdt
{
__published:   // IDE-managed Components
   TListBox *ListBox;
   TAction *ActionLocaliza;
   TGroupBox *GroupBoxTipo;
   TComboBox *CBoxTipoEqpto;
   TComboBox *CBoxAtributo;
   TComboBox *CBoxOperador;
   TRichEdit *RichEditValor;
   TAction *ActionCancelaSelecao;
   TGroupBox *GroupBox1;
   TComboBox *CBoxConsulta;
   TBitBtn *ButLocaliza;
   TPanel *PanelFiltro;
   TActionList *ActionListConsulta;
   TAction *ActionFiltroPorAtributo;
   TAction *ActionFiltroEqptoErroGis;
   TPanel *PanelCorEqpto;
   TPanel *PanelCor;
   TColorDialog *ColorDialog;
   TAction *ActionCor;
   TGroupBox *GroupBox2;
   TPanel *Panel1;
   TGroupBox *GBoxEqpto;
   TRadioGroup *RadioGroupArea;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionLocalizaExecute(TObject *Sender);
   void __fastcall ListBoxClick(TObject *Sender);
   void __fastcall CBoxTipoEqptoChange(TObject *Sender);
   void __fastcall CBoxOperadorChange(TObject *Sender);
   void __fastcall CBoxAtributoChange(TObject *Sender);
   void __fastcall ListBoxDblClick(TObject *Sender);
   void __fastcall ActionCancelaSelecaoExecute(TObject *Sender);
   void __fastcall ActionFiltroPorAtributoExecute(TObject *Sender);
   void __fastcall ActionFiltroEqptoErroGisExecute(TObject *Sender);
   void __fastcall CBoxConsultaChange(TObject *Sender);
   void __fastcall ActionCorExecute(TObject *Sender);
   void __fastcall RichEditValorKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
   void __fastcall ListBoxMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
   void __fastcall RadioGroupAreaClick(TObject *Sender);
	void __fastcall ActionMouseOffExecute(TObject *Sender);
	void __fastcall ActionMouseOnExecute(TObject *Sender);
	void __fastcall ActionEdtExecute(TObject *Sender);
	void __fastcall FormMouseActivate(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int HitTest, TMouseActivate &MouseActivate);


public:      // User declarations
   __fastcall TFormLocaliza(TForm* Owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormLocaliza(void);

private:   //métodos
   int     __fastcall Atributo(void);
   bool    __fastcall AtributoValido(TAtributo *atributo);
   void    __fastcall CBoxAtributoInicia(VTEqpto *eqpto);
   void    __fastcall CBoxConsultaInicia(void);
   void    __fastcall CBoxOperadorInicia(void);
   void    __fastcall CBoxTipoEqptoInicia(void);
   void    __fastcall ExecutaBusca(void);
   void    __fastcall ExecutaConsultaSelecionada(void);
   void    __fastcall FormIniciaPosicao(void);
   void    __fastcall GraficoDestacaEqpto(TList *lisEQP=NULL);
   void    __fastcall GraficoMoldura(VTEqpto *eqpto=NULL);
   void    __fastcall GraficoZoom(VTEqpto *eqpto);
   void    __fastcall ListBoxInicia(TList *lisEQP);
   VTRede* __fastcall LocalizaRede(VTEqpto *eqpto);
   void    __fastcall Moldura(VTEqpto *eqpto);
   int     __fastcall Operador(void);
   int     __fastcall TipoEquipamento(void);

private:   //objetos externos
   VTGrafico  *grafico;
   VTRedes    *redes;

private:   //objetos locais
   VTApl      *apl;
   VTEdt      *edt;
   VTConsulta *consulta;
   VTLegenda  *legenda;
   VTLocaliza *localiza;
   TList      *lisATRIB;
   struct{
         TShiftState shift_state;
         }zoom;
};

//---------------------------------------------------------------------------
#endif
//eof
