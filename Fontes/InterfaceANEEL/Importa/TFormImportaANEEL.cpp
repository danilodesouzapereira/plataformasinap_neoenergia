//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <vcl.h>
#include <DateUtils.hpp>
//---------------------------------------------------------------------------
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Flow\VTFlow.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <Fontes\Grafico\VTGrafico.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Sinap\VTSinapChild.h>
#include <Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>
//---------------------------------------------------------------------------
#include "TFormImportaANEEL.h"
#include "..\Importa\VTConversor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormImportaANEEL::TFormImportaANEEL(TComponent* Owner, VTApl *apl_owner)
   : TForm(Owner)
   {
	//vari�veis locais
	VTSinapChild *sinap_child = (VTSinapChild*)apl_owner->GetObject(__classid(VTSinapChild));

	//salva ponteiro p/ objeto
   this->apl = apl_owner;
   //insere Form em seu Parent
   //if (parent) {Parent = parent;}
   //cria objetos
   StrListAllFiles = new TStringList();
   //inicia data de refer�ncia dos dados
   DateTimePicker->DateTime = DateOf(Now());
	//exibe o gr�fico de topologia
	sinap_child->ActionTopo->Execute();
   }

//---------------------------------------------------------------------------
__fastcall TFormImportaANEEL::~TFormImportaANEEL(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destr�i objetos
   if(StrListAllFiles) {delete StrListAllFiles; StrListAllFiles = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaANEEL::ActionCancelaSelecaoExecute(TObject *Sender)
   {
   //reinicia LBoxArquivo
   LBoxArquivo->Items->Clear();
   //reinicia StrListAllFiles
   StrListAllFiles->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaANEEL::ActionImportaExecute(TObject *Sender)
   {
	//vari�veis locais
   bool        sucesso;
   VTFlow      *flow  = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTGrafico   *graf  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //inicia StrList de arquivos selecionados
   if (LeArquivos() == 0)
      {
      Aviso("Selecione arquivos das redes prim�rias");
      return;
      }
   //esconde este form
   Visible = false;
   //executa a importa��o das redes selecionadas
   sucesso = ImportaRede();
   if (! sucesso)
      {//torna a exibir este Form
      Visible = true;
      return;
      }
   //atualiza o gr�fico
   graf->AreaRedeDefine();
   //marca que o fluxo n�o est� atualziado
   flow->UpToDate = false;
   //fecha form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaANEEL::ActionFechaExecute(TObject *Sender)
   {
   //fecha form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaANEEL::ActionReiniciarRedeExecute(TObject *Sender)
   {
   //var local
   VTGrafico *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //solicita aviso p/ apagar redes
   if(! ReiniciaRede(true)) return;
   //atualiza o gr�fico
   grafico->AreaRedeDefine();
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaANEEL::ActionRetArquivoExecute(TObject *Sender)
   {
   //prote��o
   if (LBoxArquivo->Items->Count == 0)
      {
      Aviso("N�o h� arquivos de rede selecionados");
      return;
      }
   if (LBoxArquivo->ItemIndex < 0)
      {
      Aviso("Selecione o arquivo de rede a ser removido");
      return;
      }
   if (Confirma("Deseja remover o arquivo selecionado ?",
       LBoxArquivo->Items->Strings[LBoxArquivo->ItemIndex]) == ID_YES)
      {//remove arquivo
      LBoxArquivo->Items->Delete(LBoxArquivo->ItemIndex);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaANEEL::ActionSelArquivoTxtExecute(TObject *Sender)
   {
   //esconde este Form  p/ n�o esconde janela de sele��o do arquivo
   Visible = false;
   Application->ProcessMessages();
   //solicita sele��o de arquivos
   //SelecionaArquivos();
   //verifica o tipo de arquivo a ser selecionado
   if (RadioGroupTipoArquivo->ItemIndex == 0)
      {//arquivos com dados dados de rede
      SelecionaArqTxt(StrListAllFiles);
      }
   else
      {//arquivo com lista de arquivos
      SelecionaLisArqTxt(StrListAllFiles);
      }
   //reordena StrListAllFiles
   StrListAllFiles->Sorted = true;
   //reinicia LBoxArquivo
   if (StrListAllFiles->Count) LBoxArquivoInicia();
   //reapresenta este Form
   Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaANEEL::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaANEEL::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form no canto superior esquerdo
   Left = 0;
   Top  = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaANEEL::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormImportaANEEL::ImportaRede(void)
   {
	strOPImporta opcao;
   bool         resp;
   VTConversor  *conversor;

	opcao.date               = DateTimePicker->Date;
   opcao.sub.topologia      = CheckBoxEtd->Checked ? opSUB_TROCAR : opSUB_GIS;
   opcao.pri.consA4         = opCURVA;
	opcao.sec.topologia      = RadioGroupCampoAcoplamento->ItemIndex;
	opcao.valida_fase        = CheckBoxValidaFase->Checked;
   opcao.corrige_fase       = CheckBoxCorrigeFase->Checked;
   opcao.corrige_perda_fe   = CheckBoxCorrigePerdaFerro->Checked;
	opcao.gerarTXT           = false;
	//op��es de ihm
   opcao.ihm.log            = CheckBoxLog->Checked;
   opcao.ihm.progresso      = true;
   try{//cria objeto conversor de txt
      conversor = NewObjConversor(apl);
      //importa arquivos txt
      if (RadioGroupTipoArquivo->ItemIndex == 0)
         {//arquivo de dados de um prim�rio
         resp = conversor->ImportaLisArqPrimario(StrListAllFiles, &opcao);
         }
      else if (RadioGroupTipoArquivo->ItemIndex == 1)
         {//lista de arquivos dos prim�rios de uma subesta��o
         resp = conversor->ImportaLisArqSubestacao(StrListAllFiles, &opcao);
         }
      //destr�i objeto Conversor
      delete conversor;
      }catch(Exception &e)
         {
         return(false);
         }
   return(resp);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormImportaSoltec::LBoxArquivoInsere(AnsiString filename)
   {
   //verifica se o arquivo j� existe no LBox
   if (LBoxArquivo->Items->IndexOf(filename) >= 0) return;
   //insere arquivo no LBox
   LBoxArquivo->Items->Add(filename);
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormImportaANEEL::LBoxArquivoInicia(int num_colunas)
   {
   //reinicia LBoxArquivo
   LBoxArquivo->Clear();
   LBoxArquivo->Sorted = false;
   LBoxArquivo->Columns = num_colunas;
   //copia apenas o nome do arquivo (sem o path completo)
   for (int n = 0; n < StrListAllFiles->Count; n++)
      {
      LBoxArquivo->Items->Add(ExtractFileName(StrListAllFiles->Strings[n]));
      }
   //IMPORTANTE: n�o ordena LBoxArquivo p/ manter mesma sequencia de StrListAllFiles
   //LBoxArquivo->Sorted = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaANEEL::LBoxArquivoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
   {
   //verifica se usu�rio pressionou "DELETE"
   if(Key != VK_DELETE) return;

   //remove item selecionado
   ActionRetArquivo->Execute();
   }
/*
//---------------------------------------------------------------------------
int __fastcall TFormImportaSoltec::LeArquivos(void)
   {
   //var local
   AnsiString  arquivo;

   //prote��o
   if (LBoxArquivo->Items->Count == 0) return(0);
   //reinicia StrListAllFiles
   StrListAllFiles->Clear();
   for(int n = 0; n < LBoxArquivo->Items->Count; n++)
      {
      arquivo = LBoxArquivo->Items->Strings[n].Trim();
      if (! FileExists(arquivo)) continue;
      StrListAllFiles->Add(arquivo);
      }
   return(StrListAllFiles->Count);
   }
*/
//---------------------------------------------------------------------------
int __fastcall TFormImportaANEEL::LeArquivos(void)
   {
   return(StrListAllFiles->Count);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormImportaANEEL::ReiniciaRede(bool aviso)
   {
   //var local
   VTRedes  *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList    *lisRedes;

   //prote��o
   if(! redes) return false;
   lisRedes = redes->LisRede();
   if(lisRedes->Count == 0) return true;

   //aviso
   if(aviso)
      if(Confirma("As redes ser�o apagadas da mem�ria.\n"
               "Deseja realmente continuar ?","") != ID_YES)
         {
         return false;
         }
      
   //remove redes da memoria
   if(redes) redes->Clear();

   return true;
   }
/*
//---------------------------------------------------------------------------
void  __fastcall TFormImportaANEEL::SelecionaArquivos(void)
   {
   //vari�veis locais
   AnsiString filename;   //full path
   AnsiString arquivo, extensao;
   int        index;

   //configura tipo de arquivo
   if (RadioGroupTipoArquivo->ItemIndex == 0)
      {//arquivos com dados dados de rede
      OpenTextFileDialog->Filter = "Arquivos texto (*.txt)|*.txt|Todos arquivos (*.*)|*.*";
      }
   else
      {//arquivo com lista de arquivos
      OpenTextFileDialog->Filter = "Arquivos tipo lista (*.lis)|*.lis|Todos arquivos (*.*)|*.*";
      }
   //exibe OpenTextFileDialog
   if (OpenTextFileDialog->Execute())
      {//valida sele��o
      if (OpenTextFileDialog->Files->Count == 0) return;
      //cancela ordena��o do LBoxArquivo
      LBoxArquivo->Sorted = false;
      //loop para todos arquivos selecionados
      for(int n = 0; n < OpenTextFileDialog->Files->Count; n++)
         {//insere os arquivos no diret�rio correspondente
         filename = OpenTextFileDialog->Files->Strings[n];
         arquivo  = ExtractFileName(filename);
         extensao = ExtractFileExt(arquivo);
         //verifica a op��o de importa��o selecionada
         if (RadioGroupTipoArquivo->ItemIndex == 0)
            {//inclui somente arquivos com estens�o .txt
            if (extensao.AnsiCompareIC(".txt") == 0)
               {//adiciona arquivo para importa��o
               if (StrListAllFiles->IndexOf(filename) < 0) StrListAllFiles->Add(filename);
               }
            }
         else
            {//inclui somente arquivos com estens�o .lis
            if (extensao.AnsiCompareIC(".lis") == 0)
               {//adiciona arquivo para importa��o
               if (StrListAllFiles->IndexOf(filename) < 0) StrListAllFiles->Add(filename);
               }
            }
         }
      //reordena StrListAllFiles
      StrListAllFiles->Sorted = true;
      //reinicia LBoxArquivo
      if (StrListAllFiles->Count) LBoxArquivoInicia();
      }
   }
*/
//---------------------------------------------------------------------------
void  __fastcall TFormImportaANEEL::SelecionaArqTxt(TStrings *files)
   {
   //vari�veis locais
   AnsiString filename, extensao;
   VTPath     *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //limpa files
   //files->Clear();
   //configura OpenFileDialog
   if (OpenTextFileDialog->InitialDir.IsEmpty()) OpenTextFileDialog->InitialDir = path->DirImporta();
   //exibe OpenTextFileDialog
   if (OpenTextFileDialog->Execute())
      {//valida sele��o
      if (OpenTextFileDialog->Files->Count == 0) return;
      //loop para todos arquivos selecionados
      for(int n = 0; n < OpenTextFileDialog->Files->Count; n++)
         {//inclui somente arquivo com extens�o diferente de "LIS"
         filename = OpenTextFileDialog->Files->Strings[n];
         extensao = ExtractFileExt(filename).UpperCase();
         if (extensao.AnsiPos("LIS") == 0 )
            {//adiciona arquivo para importa��o
            if (files->IndexOf(filename) < 0)
               {
               files->Add(filename);
               }
            }
         }
      }
   }

//---------------------------------------------------------------------------
void  __fastcall TFormImportaANEEL::SelecionaLisArqTxt(TStrings *files)
   {
   //vari�veis locais
   AnsiString filename, extensao;

   //limpa files
   //files->Clear();
   //configura OpenFileDialog
   OpenTextFileDialog->Filter = "Lista de arquivos (*.lis)|*.lis|Todos arquivos (*.*)|*.*";
   //exibe OpenTextFileDialog
   if (OpenTextFileDialog->Execute())
      {//valida sele��o
      if (OpenTextFileDialog->Files->Count == 0) return;
      //loop para todos arquivos selecionados
      for(int n = 0; n < OpenTextFileDialog->Files->Count; n++)
         {//inclui somente arquivo com extens�o "LIS"
         filename = OpenTextFileDialog->Files->Strings[n];
         extensao = ExtractFileExt(filename).UpperCase();
         if (extensao.AnsiPos("LIS") != 0 )
            {//adiciona arquivo para importa��o
            if (files->IndexOf(filename) < 0)
               {
               files->Add(filename);
               }
            }
         }
      }
   }


//---------------------------------------------------------------------------
//eof


