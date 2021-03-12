//---------------------------------------------------------------------------
#ifndef TFormEqvH
#define TFormEqvH

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
#include <CheckLst.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTData;
class VTEdt;
class VTOrdena;

//---------------------------------------------------------------------------
class TFormEqv : public TFormEdt
{
__published:   // IDE-managed Components
   TGroupBox *GBoxBarraPre;
   TGroupBox *GBoxBarraSel;
   TSplitter *Splitter1;
   TAction *ActionSelEqpto;
   TListBox *LBoxBarraPre;
   TListBox *LBoxBarraSel;
   TAction *ActionRedeEqv;
   TCheckBox *CheckBoxEqvEquilibrado;
   TCheckBox *CheckBoxImpressao;
   TPanel *PanelRodape;
   TGroupBox *GBoxOpcao;
   TCheckListBox *CLBoxOpcao;
   TToolBar *ToolBar1;
   TToolButton *ButFecha;
   TToolButton *ButRedutor;
   TAction *ActionFecha;
   TCheckBox *CheckBoxRedeUnica;
   TButton *Button1;
   TAction *ActionOpcaoWise;
   TButton *Button2;
   TAction *ActionOpcaoDefault;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionSelEqptoExecute(TObject *Sender);
   void __fastcall ActionRedeEqvExecute(TObject *Sender);
   void __fastcall ActionEdtExecute(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall CLBoxOpcaoClickCheck(TObject *Sender);
   void __fastcall ActionOpcaoWiseExecute(TObject *Sender);
   void __fastcall ActionOpcaoDefaultExecute(TObject *Sender);

public:      // User declarations
   __fastcall  TFormEqv(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormEqv(void);

   private:   //métodos
      void __fastcall CLBoxOpcaoDefault(void);
      void __fastcall CLBoxOpcaoWise(void);
      void __fastcall DestacaBarraNoGrafico(void);
      bool __fastcall ExistemLigacoesComFasesDistintas(TList *lisLIG, VTBarra *barra);
      void __fastcall IniciaLBox(TListBox *CLBox, TList *lisEQP);
      void __fastcall IniciaLisBarraPre(void);
      bool __fastcall MontaRedeEquivalentePorRede(void);
      bool __fastcall MontaRedeEquivalenteUnica(void);
      void __fastcall SelecionaBarraChave(void);
      void __fastcall SelecionaBarraComBifurcacao(void);
      void __fastcall SelecionaBarraEqbar(int tipo_eqbar);
      void __fastcall SelecionaBarraEqptoComMedicao(void);
      void __fastcall SelecionaBarraLigacao(int tipo_ligacao);
      void __fastcall SelecionaBarraLigacoesComFasesDistintas(void);
      void __fastcall SelecionaBarraTrechoConectaChave(void);
      void __fastcall SelecionaChavePorEstado(int estado);
      void __fastcall SelecionaChavePorTipo(int tipo);
      void __fastcall TrataAlteracaoEqptosSelecionados(void);

   private:   //objetos externos
      VTApl *apl;

   private:   //dados locais
      VTEdt          *edt;
      VTData         *data;
      VTOrdena       *ordena;
      TList          *lisBAR;
      TList          *lisBAR_PRE;
      TList          *lisBAR_SEL;
      TList          *lisEQP;
      TList          *lisCHV;
      TList          *lisTMP;
};

//---------------------------------------------------------------------------
#endif
//eof
