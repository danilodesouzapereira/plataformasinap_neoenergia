//---------------------------------------------------------------------------
#ifndef TFormMonopolarH
#define TFormMonopolarH

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
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEdt;
class VTArranjo;
class VTMatZ;
class VTMontaZ;
class VTTrecho;

//---------------------------------------------------------------------------
class TFormMonopolar : public TFormEdt
{
__published:	// IDE-managed Components
   TAction *ActionSelEqptoClickSimples;
   TGroupBox *GBoxFase;
   TComboBox *CBoxFase;
   TGroupBox *GroupBox1;
   TComboBox *CBoxRede;
   TGroupBox *GroupBox2;
   TComboBox *CBoxTrecho;
   TAction *ActionRedeAlterada;
   TAction *ActionSelEqptoClickDuplo;
   TAction *ActionHelp;
   TToolButton *ButHelp;
   void __fastcall ActionEdtExecute(TObject *Sender);
   void __fastcall ActionSelEqptoClickSimplesExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall CBoxFaseChange(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall CBoxRedeChange(TObject *Sender);
   void __fastcall CBoxTrechoChange(TObject *Sender);
   void __fastcall ActionRedeAlteradaExecute(TObject *Sender);
   void __fastcall ActionSelEqptoClickDuploExecute(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);

private:	//objetos externos
   VTApl *apl;

private:	//dados locais
   int       ind_pat;
   int       modelo_carga, tipo_fluxo; //para flow
   VTEdt     *edt;
   VTMontaZ  *montaZ;
   TList     *lisEQP;
   struct   {
            int       ind_fase;
            VTTrecho  *trecho;
            VTArranjo *arranjo_original;
            }defeito;

private:	//métodos 
   void __fastcall AlteraMatZ(VTMatZ *matZ, int ind_fase, double comp_km);
   void __fastcall AnalisaDefeito(VTTrecho *trecho, int ind_fase);
   void __fastcall CBoxFaseInicia(int fases_tag);
   void __fastcall CBoxRedeInicia(void);
   void __fastcall CBoxTrechoInicia(void);
   void __fastcall ConfiguraObjEdita(bool insere_action);
   void __fastcall DefineTrechoComDefeito(VTTrecho *trecho);
   void __fastcall ModificaTrechoComDefeito(VTTrecho *trecho, int ind_fase);
   void __fastcall RestauraCfgFluxo(void);
   void __fastcall RestauraTrechoComDefeito(void);
   void __fastcall SalvaCfgFluxo(void);

public:		// User declarations
   __fastcall TFormMonopolar(TForm *form_owner, VTApl *apl_owner, TWinControl *parent, int ind_pat);
   __fastcall ~TFormMonopolar(void);
};

//---------------------------------------------------------------------------
#endif
//eof
