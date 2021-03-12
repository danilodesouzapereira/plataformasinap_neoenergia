//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCfgGraf.h"
#include "TFormCfgPadrao.h"
#include "..\DLL_Stub\Basico.h"
#include "..\BDado\VTBDado.h"
#include "..\Geral\VTGeral.h"
#include "..\Sisplan\Funcao.h"
#include "..\Grafico\TPad.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TformCfgGraf::TformCfgGraf(TComponent* Owner, AnsiString dirBase, AnsiString dirRede)
   : TForm(Owner)
   {

   //cria objeto BDado
   bdado = DLL_NewObjBDado();
   bdado->DefDiretorio(dirBase, dirRede);
   //cria objeto Geral
   geral = bdado->LeGeral();
   //recebe valor do geral
   lisPad=new TList;

   CopiaTList (geral->lisPAD,lisPad);

   formCfgPad = new TformCfgPadrao(this, NULL);
   formCfgPad->BorderStyle = bsNone;

   formCfgPad->Parent = pnlParCfg;

   formCfgPad->Align = alClient;

   }
//---------------------------------------------------------------------------
__fastcall TformCfgGraf::~TformCfgGraf(void)
   {

   //destrói objetos criados
   if (geral) {delete geral; geral = NULL;}
   if (bdado) {delete bdado; bdado = NULL;}
   if (lisPad) {delete lisPad; lisPad = NULL;}
   if (formCfgPad) {delete formCfgPad; formCfgPad = NULL;}

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgGraf::ActionCancelaExecute(TObject *Sender)
   {

   ModalResult = mrCancel;

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgGraf::ActionConfirmaExecute(TObject *Sender)
   {

   bdado->GravaGeral(geral);
   //fecha o form
   ModalResult = mrOk;

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgGraf::ActionDesfazExecute(TObject *Sender)
   {

   geral = bdado->LeGeral();
   //recebe valor do geral
   LimpaTList (lisPad);
   CopiaTList (geral->lisPAD,lisPad);
   LoadPadrao();
   EscolhePadraoExecute(NULL);

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgGraf::EscolhePadraoExecute(TObject *Sender)
   {

   tmpPad = (TPad *) cboPadrao->Items->Objects[cboPadrao->ItemIndex];

   formCfgPad->DefObjPadrao(tmpPad);
   formCfgPad->Show();

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgGraf::FormShow(TObject *Sender)
   {

   LoadPadrao();

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgGraf::LoadPadrao(void)
   {

   TPad *tmpPad;

   cboPadrao->Clear();

   //   lisPad->Count;
   for (int np=0; np < lisPad->Count; np ++)
      {
      tmpPad=(TPad *)lisPad->Items[np];
      cboPadrao->AddItem(tmpPad->nome, tmpPad);
      }

   if (cboPadrao->Items->Count > 0)
      {
      cboPadrao->ItemIndex=0;
      EscolhePadraoExecute (NULL);
      }

   }
//---------------------------------------------------------------------------



