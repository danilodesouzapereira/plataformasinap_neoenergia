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
   //verifica se já existe o objeto Regioes
   if ((regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes))) == NULL)
      {//cria objeto Regioes e insere em Apl recebido
      apl->Add(regioes = NewObjRegioes(apl));
      }
   //carrega Regioes disponíveis
   ActionImportaTxt->Execute();
   }

//---------------------------------------------------------------------------
__fastcall TFormRegiao::~TFormRegiao(void)
   {
   //destrói objetos
   if (regioes) {apl->Remove(regioes); delete regioes; regioes = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormRegiao::ActionCentroCargaExecute(TObject *Sender)
   {
   //variáveis locais
   TFormCentroCarga *FormCentroCarga;

   //esconde este Form
   Hide();
   //cria FormCentroCarga
   FormCentroCarga = new TFormCentroCarga(this, apl, Parent);
   FormCentroCarga->ActionOnClose = ActionShow;
   //exibe centro de carga das Regioes
   FormCentroCarga->Show();
   //detrói FormCentroCarga
   //delete FormCentroCarga;
   //reapresenta este Form
   //Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRegiao::ActionImportaTxtExecute(TObject *Sender)
   {
   //variáveis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	if (OpenDialog->InitialDir.IsEmpty())
		{//assume diretório default de importação
		OpenDialog->InitialDir = path->DirImporta();
		}
	if (OpenDialog->Execute())
		{//importa Regiões do arquivo
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
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //redesenha o gráfico
   grafico->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRegiao::CLBoxRegiaoClick(TObject *Sender)
   {
   //variáveis locais
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
   //variáveis locais
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
      //pré-seleciona Regiao se sua área fizer parte da área das Redes
      CLBoxRegiao->Checked[index] = regiao->Visible;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRegiao::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
bool __fastcall TFormRegiao::LeArquivoLatLon(AnsiString arq_txt)
   {
   //importa Regioes do arquivo
   if (! regioes->ImportaRegiaoLatLon(OpenDialog->FileName))
      {
      Erro("Não foi possível ler o arquivo das regiões");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

