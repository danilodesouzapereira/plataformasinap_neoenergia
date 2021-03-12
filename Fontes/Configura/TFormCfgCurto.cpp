//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCfgCurto.h"
#include "..\DLL_Stub\Basico.h"
#include "..\BDado\VTBDado.h"
#include "..\Sisplan\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TformCfgCurto::TformCfgCurto(TComponent* Owner, AnsiString dirBase, AnsiString dirRede)
   : TForm(Owner)
   {

   //cria objeto BDado
   bdado = DLL_NewObjBDado();
   bdado->DefDiretorio(dirBase, dirRede);
   //cria objeto Geral
   geral = bdado->LeGeral();
   //recebe valor do geral
   CURTO=geral->CURTO;

   }
//---------------------------------------------------------------------------
__fastcall TformCfgCurto::~TformCfgCurto(void)
   {

   //destrói objetos criados
   if (geral) {delete geral; geral = NULL;}
   if (bdado) {delete bdado; bdado = NULL;}

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgCurto::ActionCancelaExecute(TObject *Sender)
   {

   ModalResult = mrCancel;

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgCurto::ActionConfirmaExecute(TObject *Sender)
   {

   //lê e valida dados editados
   if (! LeDados()) return;
   //atualiza base de dados
   bdado->GravaGeral(geral);
   //fecha o form
   ModalResult = mrOk;

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgCurto::ActionDefaultExecute(TObject *Sender)
   {

   CURTO.R_DEF = 40;
   CURTO.X_DEF = 0;
   //atualiza tela
   ExibeDados();

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgCurto::ExibeDados(void)
   {

   //impedância de defeito
   edtRdef->Text = FloatToStr(CURTO.R_DEF);
   edtXdef->Text = FloatToStr(CURTO.X_DEF);

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgCurto::FormClose(TObject *Sender, TCloseAction &Action)
   {

   Action = caFree;

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgCurto::FormShow(TObject *Sender)
   {

   //apresenta dados na tela
   ExibeDados();

   }
//---------------------------------------------------------------------------
bool __fastcall TformCfgCurto::LeDados(void)
   {
   try
   {
   //impedância de defeito
   CURTO.R_DEF   = StrToDouble(edtRdef->Text);
   CURTO.X_DEF   = StrToDouble(edtXdef->Text);
   }//try
   //tratamento de erro
   catch (EConvertError &exception)
      {
      Erro("Curto: valor não numérico");
      return(false);
      }
   //leu os dados corretamente: atualiza objeto geral
   geral->CURTO = CURTO;

   return(true);
   }
//---------------------------------------------------------------------------






