//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormGrupo.h"
#include "..\TFormEditor.h"
#include "..\VTEdt.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Editor\VTEditor.h>
#include <Fontes\InterfaceTXT\Importa\VTImportaTXT.h>
#include <Fontes\InterfaceXML\VTImportaXML.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTEqpto.h>
#include <Fontes\Rede\VTLigacao.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Zona\VTZonas.h>
#include <Fontes\Topologia\VTTopologia.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\InterfaceTxt.h>
#include <DLL_Inc\InterfaceXml.h>
#include <DLL_Inc\Rede.h>
#include <DLL_Inc\Topologia.h>
#include <DLL_Inc\Zona.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormGrupo::TFormGrupo(TFormEditor *FormEditor, VTApl *apl_owner, TWinControl *parent)
   : TForm(Owner)
	{
	//salva ponteiro p/ objetos
   this->FormEditor = FormEditor;
   this->apl_owner  = apl_owner;
   //insere este Form em seu Parente
   if (parent != NULL)
      {
      Parent = parent;
      Align  = alClient;
      }
   //cria objeto Apl local
   apl_local = NewObjApl(this, apl_owner);
   //cria objeto VRedes local
	apl_local->Add(DLL_NewObjRedes(apl_local));
	apl_local->Add(DLL_NewObjZonas(apl_local));
	apl_local->Add(DLL_NewObjTopologia(apl_local));
   //cria objetos
   lisEQP  = new TList();
   //inicia TView com grupos disponíveis na base
   TViewGrupoInicia(TViewGrupo);
   }

//---------------------------------------------------------------------------
__fastcall TFormGrupo::~TFormGrupo(void)
   {
   //destrói objetos
   //if (edt_cola)  {delete edt_cola;  edt_cola  = NULL;}
   //if (edt_copia) {delete edt_copia; edt_copia = NULL;}
   if (apl_local) {delete apl_local; apl_local = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormGrupo::ActionExibeModuloExecute(TObject *Sender)
   {
   //exibe grupo selecionado
   ExibeGrupo(TViewGrupo->Selected);
   }

//---------------------------------------------------------------------------
void __fastcall TFormGrupo::ActionOnDoneExecute(TObject *Sender)
   {
   //destrói objeto
   //if (edt_cola  != NULL) {delete edt_cola;  edt_cola  = NULL;}
   //if (edt_copia != NULL) {delete edt_copia; edt_copia = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormGrupo::Atualiza(void)
   {
   TViewGrupoInicia(TViewGrupo);
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormGrupo::CancelaAssociacaoLigacaoRede(TList *lisEXT)
   {
   //variáveis locais
   VTEqpto   *eqpto;
   VTLigacao *ligacao;

   for (int n = 0; n < lisEXT->Count; n++)
      {
      eqpto = (VTEqpto*)lisEXT->Items[n];
      if (eqpto->TipoLigacao())
         {
         ligacao = (VTLigacao*)eqpto;
         //ligacao->rede->RemoveLigacao(ligacao);
         ligacao->rede = NULL;
         }
      }
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormGrupo::CarregaArquivo(AnsiString arq_xml)
   {
   //variáveis locais
   VTImportaXML *importaXML;
   VTRede       *rede;
   TList        *lisREDE;
   VTRedes      *redes = (VTRedes*)apl_local->GetObject(__classid(VTRedes));

   try{//verifica se existe o arquivo XML
      if (! FileExists(arq_xml))
         {
         Aviso("O módulo não está disponível");
         return;
         }
      //reinicia objeto Redes local
      redes->Clear();
      //cria objeto p/ importar arquivo XML
      importaXML = DLL_NewObjImportaXML(apl_local);
      //importa arquivo XML em Redes do Apl local
      if (importaXML->ExecutaImportacao(arq_xml))
         {//copia Eqptos das redes importadas p/ lisEQP
         lisEQP->Clear();
         lisREDE = redes->LisRede();
         for (int n = 0; n < lisREDE->Count; n++)
            {
            rede = (VTRede*)lisREDE->Items[n];
            //insere os eqptos de Rede em lisEQP
            rede->LisEqpto(lisEQP);
            }
         //cancela associação de todas as Ligacoes com suas respectivas Redes
         CancelaAssociacaoLigacaoRede(lisEQP);
         //prepara p/ copiar a Rede lida
         edt_copia->EvtSelEqpto(lisEQP);
         }
      //destrói objeto criado
      delete importaXML;
      }catch(Exception &e)
         {
         Aviso("Não foi possível criar o módulo indicado");
         }
   }
*/

//---------------------------------------------------------------------------
void __fastcall TFormGrupo::CarregaArquivoTXT(AnsiString arq_txt)
   {
   //variáveis locais
   VTImportaTXT *importaTXT;
   VTRede       *rede;
   TList        *lisREDE;
   VTRedes      *redes = (VTRedes*)apl_local->GetObject(__classid(VTRedes));

   try{//verifica se existe o arquivo XML
      if (! FileExists(arq_txt))
         {
         Aviso("O módulo de rede não está disponível");
         return;
         }
      //reinicia objeto Redes local
      redes->Clear();
      //cria objeto p/ importar arquivo XML
      importaTXT = DLL_NewObjImportaTXT(apl_local);
      //importa arquivo XML em Redes do Apl local
      //if (importaTXT->ImportaRedes(arq_txt))
      if (importaTXT->ExecutaImportacao(arq_txt))
         {//copia Eqptos das redes importadas p/ lisEQP
         lisEQP->Clear();
         lisREDE = redes->LisRede();
         for (int n = 0; n < lisREDE->Count; n++)
            {
            rede = (VTRede*)lisREDE->Items[n];
            //insere os eqptos de Rede em lisEQP lisEP->Add
            lisEQP->Add(rede);
            rede->LisEqpto(lisEQP);
            }
         //cancela associação de todas as Ligacoes com suas respectivas Redes
         //CancelaAssociacaoLigacaoRede(lisEQP);
         //prepara p/ copiar a Rede lida
         edt_copia->EvtSelEqpto(lisEQP);
         }
      //destrói objeto criado
      delete importaTXT;
      }catch(Exception &e)
         {
         Aviso("Não foi possível criar o módulo de rede indicado");
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormGrupo::CarregaArquivoXML(AnsiString arq_xml)
   {
   //variáveis locais
   VTImportaXML *importaXML;
   VTRede       *rede;
   TList        *lisREDE;
   VTRedes      *redes = (VTRedes*)apl_local->GetObject(__classid(VTRedes));

   try{//verifica se existe o arquivo XML
      if (! FileExists(arq_xml))
         {
         Aviso("O módulo não está de rede disponível");
         return;
         }
      //reinicia objeto Redes local
      redes->Clear();
      //cria objeto p/ importar arquivo XML
      importaXML = DLL_NewObjImportaXML(apl_local);
      //importa arquivo XML em Redes do Apl local
      if (importaXML->ExecutaImportacao(arq_xml))
         {//copia Eqptos das redes importadas p/ lisEQP
         lisEQP->Clear();
         lisREDE = redes->LisRede();
         for (int n = 0; n < lisREDE->Count; n++)
            {
            rede = (VTRede*)lisREDE->Items[n];
            //insere os eqptos de Rede em lisEQP lisEP->Add
            lisEQP->Add(rede);
            rede->LisEqpto(lisEQP);
            }
         //cancela associação de todas as Ligacoes com suas respectivas Redes
         //CancelaAssociacaoLigacaoRede(lisEQP);
         //prepara p/ copiar a Rede lida
         edt_copia->EvtSelEqpto(lisEQP);
         }
      //destrói objeto criado
      delete importaXML;
      }catch(Exception &e)
         {
         Aviso("Não foi possível criar o módulo de rede indicado");
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormGrupo::ExibeGrupo(TTreeNode *Node)
   {
   //variáveis locais
   int        tipo_arq;
   AnsiString arquivo;
   TTreeNode *NodeTipo;
   VTPath    *path   = (VTPath*)apl_owner->GetObject(__classid(VTPath));
   VTEditor  *editor = (VTEditor*)apl_owner->GetObject(__classid(VTEditor));


   //verifica se há um Grupo selecionado
   if ((Node == NULL)||(Node->Level != 1))
      {//cancela Grupo atual
      //if (edt_copia) edt_copia->EvtEscape();
      //Aviso("Selecione um módulo de rede ");
      return;
      }
   //determina Node pai
   NodeTipo = Node->Parent;
   //cria novos objetos p/ copiar/colar Eqptos
   edt_copia = editor->EditaCopiaEqptos(FormEditor->ActionMouseOff, FormEditor->ActionMouseOn);
   edt_cola  = editor->EditaColaEqptos(FormEditor->ActionEdtAltera, FormEditor->ActionMouseOn);
   FormEditor->ActionMouseOff->Execute();
   //monta nome do arquivo, em função do tipo do arquivo
   arquivo = path->DirModuloConstrutivo() + "\\" + NodeTipo->Text + "\\" + Node->Text;
   tipo_arq = (int)(Node->Data);
   if      (tipo_arq == 0) CarregaArquivoXML(arquivo + ".xml");
   else if (tipo_arq == 1) CarregaArquivoTXT(arquivo + ".txt");
   }

//---------------------------------------------------------------------------
void __fastcall TFormGrupo::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
int __fastcall TFormGrupo::NumeroGrupo(void)
   {
   return(TViewGrupo->Items->Count);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormGrupo::TViewGrupoInicia(void)
   {
   //variáveis locais
   TTreeNode *nodetipo;

   //cria objeto para ler árvore de diretórios em \Dat\Grupo
   LeListaArquivosXML();
   //desabilita evento OnChange
   //TViewGrupo->OnChange = NULL;
   //reinicia TView
   TViewGrupo->Items->Clear();
   //insere TreeNode raiz p/ subestações
   nodetipo = TViewGrupo->Items->Add(NULL, "Subestações");
   //insere TreeNode p/ os grupos
   TViewGrupo->Items->AddChildObject(nodetipo, "Padrão 88/13.8kV, 1 Trafo 15 MVA",  (void*)1);
   TViewGrupo->Items->AddChildObject(nodetipo, "Padrão 88/13.8kV, 2 Trafos 10 MVA", (void*)2);
   //expande TView
   TViewGrupo->FullExpand();
   //cancela TreeNode selecionado
   TViewGrupo->Selected = nodetipo;
   //reabilita evento OnChange
   //TViewGrupo->OnChange = TViewGrupoChange;
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormGrupo::TViewGrupoInicia(TTreeView *TView)
   {
   //variáveis locais
   int        tipo_arq;
   AnsiString diretorio, arquivo, ext;
   TTreeNode *nodetipo;
   VTPath    *path = (VTPath*)apl_owner->GetObject(__classid(VTPath));

   //reinicia TView
   TView->Items->Clear();
   //proteção
   if (! DirectoryExists(path->DirModuloConstrutivo())) return;
   //define diretório inicial de DirectoryListBox: força uma alteração p/ atualizar
   //DirectoryListBox->Directory = path->DirGeral();
   DirectoryListBox->Directory = path->DirModuloConstrutivo();
   DirectoryListBox->Update();
   //loop p/ todos subdiretórios, na ordem inversa, até o diretório inicial do DirectoryListBox
   for (int i = DirectoryListBox->Count - 1; i >= 0; i--)
      {
      diretorio = DirectoryListBox->GetItemPath(i);
      if (diretorio.AnsiCompareIC(DirectoryListBox->Directory) == 0) break;
      //atualiza diretório do FileListBox: força uma alteração p/ atualizar
      //FileListBox->Directory = path->DirGeral();
      FileListBox->Directory = diretorio;
      FileListBox->Update();
      //verifica se há arquivos no diretório
      //if (FileListBox->Items->Count == 0) continue;
      //insere TreeNode raiz p/ tipo
      nodetipo = TView->Items->Add(NULL, ExtractFileName(diretorio));
      //loop p/ todos arquivos do FileListBox
      for (int j = 0; j < FileListBox->Items->Count; j++)
         {//filtra nome do arquivo
         arquivo = FileListBox->Items->Strings[j];
         arquivo = ExtractFileName(arquivo);
         ext     = ExtractFileExt(arquivo);
         arquivo = ChangeFileExt(arquivo, "");
         //destermina se arquivo XML ou TXT
         if      (ext.AnsiCompareIC(".xml") == 0) tipo_arq = 0;
         else if (ext.AnsiCompareIC(".txt") == 0) tipo_arq = 1;
         else                                     continue;
         //insere TreeNode p/ os grupos
         TView->Items->AddChildObject(nodetipo, arquivo, (void*)tipo_arq);
         }
      }
   //expande TView
   TView->FullExpand();
   }

//---------------------------------------------------------------------------
//eof



