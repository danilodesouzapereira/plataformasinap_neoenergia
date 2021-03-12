//---------------------------------------------------------------------------
#ifndef TFormAbreH
#define TFormAbreH

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
#include <FileCtrl.hpp>
#include <Menus.hpp>
#include <System.Actions.hpp>
#include <Vcl.Dialogs.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class TFormEstudo;
class TFormRede;
class VTApl;
class VTAbre;
class VTEdt;

//---------------------------------------------------------------------------
class TFormAbre : public TForm
{
__published:   // IDE-managed Components
   TActionList *ActionList;
   TAction *ActionCancela;
   TToolBar *ToolBar;
   TToolButton *butCriaPlanejamento;
   TToolButton *butAbrirRede;
   TToolButton *butRedesSel;
   TToolButton *butAbrirEstudo;
   TToolButton *butExcluirEstudo;
   TToolButton *butCancela;
   TAction *ActionCriaPlanejamento;
   TAction *ActionConfirmaAll;
   TAction *ActionConfirmaSel;
   TAction *ActionAbrirEstudo;
   TAction *ActionExcluirEstudo;
   TToolButton *butSeparador1;
   TStatusBar *StatusBar;
   TGroupBox *GBoxRede;
   TToolButton *butSelecionaRede;
   TToolButton *butSelecionaEstudo;
   TAction *ActionSelecionaRede;
   TAction *ActionSelecionaEstudo;
   TSplitter *Splitter2;
   TPanel *PanelGrafico;
   TPanel *PanelAviso;
   TToolButton *butSeparator2;
   TToolButton *ButPanelBD;
   TAction *ActionSelecionaBase;
   TImageList *ImageList;
   TGroupBox *GBoxEstudo;
   TPanel *PanelSelecao;
   TAction *ActionCancelaSel;
   TToolButton *ButCancelaSel;
   TToolButton *ButCriaReclamacaoBT;
   TAction *ActionCriaReclamacaoBT;
   TAction *ActionSelRedeGrafico;
   TOpenDialog *OpenDialog;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionSelecionaRedeExecute(TObject *Sender);
   void __fastcall ActionSelecionaEstudoExecute(TObject *Sender);
   void __fastcall ActionCriaPlanejamentoExecute(TObject *Sender);
   void __fastcall ActionConfirmaAllExecute(TObject *Sender);
   void __fastcall ActionConfirmaSelExecute(TObject *Sender);
   void __fastcall ActionAbrirEstudoExecute(TObject *Sender);
   void __fastcall ActionExcluirEstudoExecute(TObject *Sender);
   void __fastcall ActionSelecionaBaseExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionCancelaSelExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionCriaReclamacaoBTExecute(TObject *Sender);
   void __fastcall ActionSelRedeGraficoExecute(TObject *Sender);

public:      // User declarations
   __fastcall  TFormAbre(TComponent* Owner, VTApl *apl_owner);
   __fastcall ~TFormAbre(void);

private:   //métodos
   void       __fastcall ConfiguraTela(void);
   void       __fastcall ExibeDadosBaseSelecionada(void);
   void       __fastcall ExibeRedesOuEstudos(void);
   void       __fastcall IniciaConexao(void);
   void       __fastcall ReiniciaRegioes(void);
   bool       __fastcall SelecionaBaseRede(void);
   bool       __fastcall SelecaoInicial(void);
   bool       __fastcall ValidaBaseRede(AnsiString sdb_name);
   void       __fastcall VerificaModulos(void);

private:   //objetos externos

private:   //dados locais
   enum enumLEVEL {LevelBaseDado=0,
                   //nodes p/ apresentação das redes
                   tvrLevelEstudo=1, tvrLevelDominio,
                   tvrLevelTipoRede, tvrLevelRede,
                   tvrLevelVizinha,  tvrLevelRedeVizinha,
                   tvrLevelRedeSel=tvrLevelTipoRede,
                   //nodes p/ apresentação dos estudos
                   tveLevelAutor=1,  tveLevelEstudo, tveLevelRede
                   };
   TFormEstudo   *FormEstudo;
   TFormRede     *FormRede;
   TList         *lisEQP;
   VTAbre        *abre;
   VTApl         *apl;
   VTEdt         *edt;
   TList         *lisREDE_PRE;
};

//---------------------------------------------------------------------------
#endif
//eof

