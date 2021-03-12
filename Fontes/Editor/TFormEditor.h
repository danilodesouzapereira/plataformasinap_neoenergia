//---------------------------------------------------------------------------
#ifndef TFormEditorH
#define TFormEditorH

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
#include <Graphics.hpp>
#include <Menus.hpp>
#include <System.Actions.hpp>
#include <Vcl.Dialogs.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class TFormGrupo;
class VTApl;
class VTEditor;
class VTRedes;

//---------------------------------------------------------------------------
class TFormEditor : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBarEqpto;
   TToolButton *butSwitch;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionEdtAltera;
   TAction *ActionEdtBarra;
   TAction *ActionEdtCapacitor;
   TAction *ActionEdtCarga;
   TAction *ActionEdtChave;
   TAction *ActionEdtGerador;
   TAction *ActionEdtSwitch;
   TAction *ActionEdtMoveDown;
   TAction *ActionEdtMoveLeft;
   TAction *ActionEdtMoveRight;
   TAction *ActionEdtMoveUp;
   TAction *ActionEdtSuprimento;
   TAction *ActionEdtTrafo;
   TAction *ActionEdtTrafo3E;
   TAction *ActionEdtTrecho;
   TAction *ActionEdtRetira;
   TAction *ActionEsqAlign;
   TToolButton *butAltera;
   TToolButton *butGerador;
   TToolButton *butSuprimento;
   TToolButton *butChave;
   TToolButton *butTrecho;
   TAction *ActionEdtPolyline;
   TToolButton *butCarga;
   TAction *ActionMouseOff;
   TToolButton *butTrafo;
   TToolButton *butBarra;
   TToolButton *butUndo;
   TToolButton *butRedo;
   TToolButton *butRede;
   TAction *ActionEdtRede;
   TAction *ActionUndo;
   TAction *ActionRedo;
   TAction *ActionAreaRedeMaior;
   TAction *ActionMouseOn;
   TPanel *panelMouse;
   TImage *ImageMouseOff;
   TImage *ImageMouseOn;
   TAction *ActionEdtTrafoZZ;
   TAction *ActionEdtMutua;
   TAction *ActionEdtFiltro;
   TAction *ActionEdtCapserie;
   TAction *ActionEdtRegulador;
   TToolButton *ButTrafo3E;
   TToolButton *ButTrafoZZ;
   TToolButton *ButCapacitor;
   TToolButton *ButCapserie;
   TToolButton *ButRegulador;
   TToolButton *ButEdtFiltro;
   TAction *ActionEdtReator;
   TToolButton *ButEdtReator;
   TToolButton *ButMutua;
   TToolButton *ButCargaIP;
   TAction *ActionEdtCargaIP;
   TAction *ActionEdtCopia;
   TAction *ActionEdtCola;
   TToolButton *ButEdtCopia;
   TToolButton *ButEdtCola;
   TToolButton *ButDelete;
   TAction *ActionDelete;
   TToolButton *ButSinc;
   TAction *ActionEdtSinc;
   TAction *ActionEdtManobra;
   TToolButton *butManobra;
   TAction *ActionAreaRedeMenor;
   TAction *ActionEdtMedicao;
   TToolButton *ButMedicao;
   TToolBar *ToolBarEsq;
   TToolButton *ButAreaRedeMaior;
   TToolButton *ButAreaRedeMenor;
   TToolButton *ButMoveLeft;
   TToolButton *ButMoveUp;
   TToolButton *ButMoveRight;
   TToolButton *ButMoveDown;
   TToolButton *ButEsqInverteY;
   TAction *ActionEsqInverteY;
   TToolButton *ToolButton1;
   TAction *ActionEsqInverteX;
   TToolButton *ButReiniciaEsquematico;
   TAction *ActionEsqReinicia;
   TToolButton *ButTrafoMono;
   TAction *ActionEdtTrafoMono;
   TToolButton *ButSelEqpto;
   TAction *ActionSelEqpto;
   TToolButton *ButAlign;
   TPopupMenu *PopupMenuAlign;
   TMenuItem *mnuAlignLeft;
   TMenuItem *mnuAlignRight;
   TMenuItem *mnuAlignDown;
   TMenuItem *mnuAlignUp;
   TMenuItem *mnuAlignNode;
   TAction *ActionAlignBottom;
   TAction *ActionAlignLeft;
   TAction *ActionAlignRight;
   TAction *ActionAlignTop;
   TAction *ActionAlignNone;
   TToolButton *ButEdtPolyline;
   TToolButton *ButClear;
   TAction *ActionClear;
   TGroupBox *GBoxGrupo;
   TAction *ActionInsGrupo;
   TAction *ActionFormFilhoClosed;
   TAction *ActionGiro;
   TToolButton *ButGiro;
   TToolButton *ButZoom;
   TAction *ActionZoom;
   TAction *ActionSelNone;
   TToolButton *ButEsqAlign;
	TToolButton *ButEdtTensao;
	TAction *ActionEdtTensao;
	TToolButton *butSuprimentoPorFase;
	TAction *ActionEdtSuprimentoPorFase;
   TPanel *PanelToolBars;
   TToolButton *ToolButton2;
   TAction *ActionHelp;
   TToolButton *butSeparadorHelp;
   TActionList *ActionListAvancado;
   TAction *ActionEditaSED;
   TAction *ActionEqvBT;
   TAction *ActionRemoveBT;
   TAction *ActionRemoveTrechos;
   TAction *ActionInsereFusivel;
   TToolButton *ToolButton4;
   TImageList *ImageListAvancado;
   TToolBar *ToolBarAvancado;
   TToolButton *ToolButton5;
   TToolButton *ToolButton6;
   TToolButton *ToolButton7;
   TToolButton *ToolButton8;
   TToolButton *ToolButton9;
   TAction *ActionRemoveFusivel;
   TToolButton *ToolButton10;
   TOpenDialog *OpenDialog;
   TSaveDialog *SaveDialog;
   TToolButton *ButEdtFases;
   TAction *ActionAlteraFases;
   TToolButton *ToolButton11;
   TAction *ActionImportaBarras;
	TAction *ActionEdtMotor;
	TToolButton *btnMotor;
	TToolButton *ToolButton3;
	TAction *ActionInsCarroEletrico;
	TToolButton *butBateria;
	TAction *ActionEdtBateria;
	TToolButton *ToolButton12;
	TAction *ActionRecondutorar;
	TActionList *ActionListFigura;
	TAction *ActionFigComentario;
	TAction *ActionFigLink;
	TAction *ActionFigRetangulo;
	TImageList *ImageListFigura;
	TToolBar *ToolBarFigura;
	TToolButton *ButFigComentario;
	TToolButton *ButFigLink;
	TToolButton *ButFigRetangulo;
   void __fastcall ActionEdtSwitchExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionEdtAlteraExecute(TObject *Sender);
   void __fastcall ActionEdtGeradorExecute(TObject *Sender);
   void __fastcall ActionEdtSuprimentoExecute(TObject *Sender);
   void __fastcall ActionEdtChaveExecute(TObject *Sender);
   void __fastcall ActionEdtTrechoExecute(TObject *Sender);
   void __fastcall ActionEdtPolylineExecute(TObject *Sender);
   void __fastcall ActionEdtCargaExecute(TObject *Sender);
   void __fastcall ActionMouseOffExecute(TObject *Sender);
   void __fastcall ActionEdtBarraExecute(TObject *Sender);
   void __fastcall ActionEdtTrafoExecute(TObject *Sender);
   void __fastcall ActionEdtMoveDownExecute(TObject *Sender);
   void __fastcall ActionEdtMoveLeftExecute(TObject *Sender);
   void __fastcall ActionEdtMoveRightExecute(TObject *Sender);
   void __fastcall ActionEdtMoveUpExecute(TObject *Sender);
   void __fastcall ActionEdtRedeExecute(TObject *Sender);
   void __fastcall ActionRedoExecute(TObject *Sender);
   void __fastcall ActionUndoExecute(TObject *Sender);
   void __fastcall ActionAreaRedeMaiorExecute(TObject *Sender);
   void __fastcall ActionMouseOnExecute(TObject *Sender);
   void __fastcall ActionEdtTrafo3EExecute(TObject *Sender);
   void __fastcall ActionEdtTrafoZZExecute(TObject *Sender);
   void __fastcall ActionEdtMutuaExecute(TObject *Sender);
   void __fastcall ActionEdtFiltroExecute(TObject *Sender);
   void __fastcall ActionEdtCapserieExecute(TObject *Sender);
   void __fastcall ActionEdtReguladorExecute(TObject *Sender);
   void __fastcall ActionEdtCapacitorExecute(TObject *Sender);
   void __fastcall ActionEdtReatorExecute(TObject *Sender);
   void __fastcall ActionEdtCargaIPExecute(TObject *Sender);
   void __fastcall ActionEdtCopiaExecute(TObject *Sender);
   void __fastcall ActionEdtColaExecute(TObject *Sender);
   void __fastcall ActionDeleteExecute(TObject *Sender);
   void __fastcall ActionEdtSincExecute(TObject *Sender);
   void __fastcall ActionEdtManobraExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionAreaRedeMenorExecute(TObject *Sender);
   void __fastcall ActionEdtMedicaoExecute(TObject *Sender);
   void __fastcall ActionEsqInverteYExecute(TObject *Sender);
   void __fastcall ActionEsqInverteXExecute(TObject *Sender);
   void __fastcall ActionEsqReiniciaExecute(TObject *Sender);
   void __fastcall ActionEdtTrafoMonoExecute(TObject *Sender);
   void __fastcall ActionSelEqptoExecute(TObject *Sender);
   void __fastcall ActionAlignBottomExecute(TObject *Sender);
   void __fastcall ActionAlignLeftExecute(TObject *Sender);
   void __fastcall ActionAlignNoneExecute(TObject *Sender);
   void __fastcall ActionAlignRightExecute(TObject *Sender);
   void __fastcall ActionAlignTopExecute(TObject *Sender);
   void __fastcall ActionClearExecute(TObject *Sender);
   void __fastcall ActionInsCarroEletricoExecute(TObject *Sender);
   void __fastcall ActionFormFilhoClosedExecute(TObject *Sender);
   void __fastcall ActionGiroExecute(TObject *Sender);
   void __fastcall ActionZoomExecute(TObject *Sender);
   void __fastcall ActionSelNoneExecute(TObject *Sender);
   void __fastcall ActionEsqAlignExecute(TObject *Sender);
	void __fastcall ActionEdtTensaoExecute(TObject *Sender);
	void __fastcall ActionEdtSuprimentoPorFaseExecute(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall ActionEditaSEDExecute(TObject *Sender);
   void __fastcall ActionEqvBTExecute(TObject *Sender);
   void __fastcall ActionRemoveBTExecute(TObject *Sender);
   void __fastcall ActionRemoveTrechosExecute(TObject *Sender);
   void __fastcall ActionInsereFusivelExecute(TObject *Sender);
   void __fastcall ActionRemoveFusivelExecute(TObject *Sender);
   void __fastcall ActionAlteraFasesExecute(TObject *Sender);
   void __fastcall ActionImportaBarrasExecute(TObject *Sender);
	void __fastcall ActionEdtMotorExecute(TObject *Sender);
	void __fastcall ActionEdtBateriaExecute(TObject *Sender);
	void __fastcall ActionRecondutorarExecute(TObject *Sender);
	void __fastcall ActionFigComentarioExecute(TObject *Sender);
	void __fastcall ActionFigLinkExecute(TObject *Sender);
	void __fastcall ActionFigRetanguloExecute(TObject *Sender);

public:      // User declarations
        __fastcall  TFormEditor(TComponent* Owner, VTApl *apl_owner, TWinControl *parent, bool planejamento);
        __fastcall ~TFormEditor(void);

private:   //métodos
   void       __fastcall AtualizaIndicacaoEditorOn(TToolButton *button=NULL);
   void       __fastcall AtualizaIndicacaoEditorOff(void);
   void       __fastcall ConfiguraOpcaoPlanejamento(void);
   void       __fastcall ConfiguraOpcaoUsuario(void);
   bool       __fastcall ExisteReducao(VTRedes *redes);
   void       __fastcall FormIniciaPosicao(void);
   void       __fastcall HabilitaEditor(void);
   AnsiString __fastcall ImportaArquivo(void);
   void       __fastcall SalvaFormFilho(TForm *form_filho);
   bool       __fastcall ValidaInsercaoGeracao(void);

private:   //dados locais
   TForm       *FormFilho;
   TFormGrupo  *FormGrupo;
   VTApl       *apl;
   VTEditor    *editor;
   TToolButton *button_down;
   TAction     *action_align;
   struct   {//coordenadas p/ janela flutuante
            int top;
            int left;
            int width;
            int height;
            } janela;
};

//---------------------------------------------------------------------------
#endif
//eof

