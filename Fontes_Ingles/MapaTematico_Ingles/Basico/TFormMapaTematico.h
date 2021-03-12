//---------------------------------------------------------------------------
#ifndef TFormMapaTematicoH
#define TFormMapaTematicoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TFormEdt.h"
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <Buttons.hpp>
#include <System.Actions.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTTema;
class VTProgresso;

//---------------------------------------------------------------------------
class TFormMapaTematico : public TFormEdt
{
__published:   // IDE-managed Components
   TGroupBox *GroupBoxTema;
   TComboBox *CBoxTema;
   TAction *ActionAtualiza;
   TGroupBox *GBoxResolucao;
   TComboBox *CBoxResolucao;
   TGroupBox *GBoxPatamar;
   TComboBox *CBoxPatamar;
   TBitBtn *BitBtn1;
   TAction *ActionPlay;
   TTimer *Timer;
   TGroupBox *GBoxCor;
   TLabel *Label1;
   TAction *ActionAtualizaCor;
   TAction *ActionHelp;
   TToolButton *ToolButton1;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall CBoxTemaChange(TObject *Sender);
   void __fastcall ActionAtualizaExecute(TObject *Sender);
   void __fastcall CBoxResolucaoInicia(void);
   void __fastcall CBoxResolucaoChange(TObject *Sender);
   void __fastcall CBoxPatamarChange(TObject *Sender);
   void __fastcall ActionPlayExecute(TObject *Sender);
   void __fastcall TimerTimer(TObject *Sender);
   void __fastcall ActionAtualizaCorExecute(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);

public:      // User declarations
          __fastcall  TFormMapaTematico(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
          __fastcall ~TFormMapaTematico(void);
   void   __fastcall  Atualiza(void);
   void   __fastcall  InsereTema(VTTema *tema, bool show=false);

private:   //métodos
   //void   __fastcall CadastraActionAtualiza(bool insere);
   void   __fastcall CBoxPatamarInicia(void);
   void   __fastcall CBoxTemaInicia(void);
   bool   __fastcall ExecutaCalculoFluxo(void);
   void   __fastcall ExibeTemaSelecionado(void);
   void   __fastcall FormIniciaPosicao(void);
   void   __fastcall SelecionaModoGrafico(int modo);
   void   __fastcall SelecionaResolucao(void);

private:   //objetos externos
   VTApl *apl;

private:   //dados locais
   TList *lisTEMA;
   TForm *formCOR;
   VTProgresso *progresso;

   struct   {
         //int ano_mercado;
         int modo_grafico;
         }original; //FKM 2015.10.01
};

//---------------------------------------------------------------------------
#endif
//eof

