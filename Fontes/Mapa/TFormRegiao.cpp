//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Grafico\VTGrafico.h>
#include <Fontes\Rede\VTArea.h>
#include <Fontes\Rede\VTRedes.h>
#include <DLL_Inc\Funcao.h>
#include "TFormRegiao.h"
#include "VTPoligono.h"
#include "VTRegiao.h"
#include "VTRegioes.h"
#include "..\MapaTematico\TFormCentroCarga.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormRegiao::TFormRegiao(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   : TForm(Owner)
   {
   //salva ponteiros p/ objetos
   apl = apl_owner;
   //define Parent
   if (parent)
      {
      Parent = parent;
      Top    = 0;
      Left   = 0;
      }
   //verifica se j� existe o objeto Regioes
   if ((regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes))) == NULL)
      {//cria objeto Regioes e insere em Apl recebido
      apl->Add(regioes = NewObjRegioes(apl));
      }
   //carrega Regioes dispon�veis
   ActionImportaTxt->Execute();
   }

//---------------------------------------------------------------------------
__fastcall TFormRegiao::~TFormRegiao(void)
   {
   //destr�i objetos
   if (regioes) {apl->Remove(regioes); delete regioes; regioes = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormRegiao::ActionCentroCargaExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormCentroCarga *FormCentroCarga;

   //esconde este Form
   Hide();
   //cria FormCentroCarga
   FormCentroCarga = new TFormCentroCarga(this, apl, Parent);
   FormCentroCarga->ActionOnClose = ActionShow;
   //exibe centro de carga das Regioes
   FormCentroCarga->Show();
   //detr�i FormCentroCarga
   //delete FormCentroCarga;
   //reapresenta este Form
   //Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRegiao::ActionImportaTxtExecute(TObject *Sender)
   {
   //vari�veis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	if (OpenDialog->InitialDir.IsEmpty())
		{//assume diret�rio default de importa��o
		OpenDialog->InitialDir = path->DirImporta();
		}
	if (OpenDialog->Execute())
		{//importa Regi�es do arquivo
      if (LeArquivoLatLon(OpenDialog->FileName))
         {//atualiza CLBoxRegiao
         CLBoxRegiaoInicia();
         //atualiza Grafico
         AtualizaGrafico();
         }
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormRegiao::ActionShowExecute(TObject *Sender)
   {
   Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRegiao::AtualizaGrafico(void)
   {
   //vari�veis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //redesenha o gr�fico
   grafico->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRegiao::CLBoxRegiaoClick(TObject *Sender)
   {
   //vari�veis locais
   VTRegiao *regiao;

   //identifica as Regioes selecionadas
   for (int n = 0; n < CLBoxRegiao->Items->Count; n++)
      {
      regiao = (VTRegiao*)CLBoxRegiao->Items->Objects[n];
      regiao->Visible = CLBoxRegiao->Checked[n];
      }
   //atualiza o Grafico
   AtualizaGrafico();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRegiao::CLBoxRegiaoInicia(void)
   {
   //vari�veis locais
   int      index;
   VTRegiao *regiao;
   TList    *lisREGIAO = regioes->LisRegiao();

   //reinicia LBoxRegiao
   CLBoxRegiao->Clear();
   //loop p/ todas Regioes em lisREGIAO
   for (int n = 0; n < lisREGIAO->Count; n++)
      {
      regiao = (VTRegiao*)lisREGIAO->Items[n];
      index = CLBoxRegiao->Items->AddObject(regiao->Codigo, regiao);
      //pr�-seleciona Regiao se sua �rea fizer parte da �rea das Redes
      CLBoxRegiao->Checked[index] = regiao->Visible;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRegiao::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destr�i o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
bool __fastcall TFormRegiao::LeArquivoLatLon(AnsiString arq_txt)
   {
   //importa Regioes do arquivo
   if (! regioes->ImportaRegiaoLatLon(OpenDialog->FileName))
      {
      Erro("N�o foi poss�vel ler o arquivo das regi�es");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

