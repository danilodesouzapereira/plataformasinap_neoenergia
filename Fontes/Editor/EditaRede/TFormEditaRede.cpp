//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEditaRede.h"
#include "TEditaSED.h"
#include "TInsereFusivel.h"
#include "TRemoveBT.h"
#include "TRemoveTrecho.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Progresso\VTProgresso.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\Importa.h>
#include <PlataformaSinap\DLL_Inc\Progresso.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEditaRede::TFormEditaRede(TComponent* Owner, VTApl *apl_owner)
   : TForm(Owner)
   {
   //salva ponteiro do apl
   apl = apl_owner;
   }

//---------------------------------------------------------------------------
__fastcall TFormEditaRede::~TFormEditaRede(void)
   {
   //nada
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEditaRede::EditaSED(void)
   {
   //variaveis locais
   bool        sucesso = false;
   AnsiString  arquivo;
   TEditaSED   *edita_sed;
   VTProgresso *prog    = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

   if(prog == NULL)
      {//cria progresso
      prog = DLL_NewObjProgresso(this, this->Parent);
      if(! prog) apl->Add(prog);
      }
   //importa arquivo
   arquivo = ImportaArquivo();
   if(arquivo.IsEmpty()) return(true);
   switch(Confirma("Essa ação é irreversível.", "Deseja continuar?"))
      {
      case IDYES:
         //mostra progresso
         if(prog) prog->Start(progDEFAULT);
         //cria objeto p/ editar a SED
         edita_sed  = new TEditaSED(apl);
         sucesso = edita_sed->Executa(arquivo);
         delete edita_sed;
         break;
      case IDNO:     return(true);
      case IDCANCEL: return(true);
      }
   if(prog) prog->Stop();
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEditaRede::EquivalentaBT(void)
   {
   //variaveis locais
   bool        sucesso = false;
   AnsiString  arquivo;
   TRemoveBT   *remove_bt;
   VTProgresso *prog    = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

   if(prog == NULL)
      {//cria progresso
      prog = DLL_NewObjProgresso(this, this->Parent);
      if(! prog) apl->Add(prog);
      }
   switch(Confirma("Essa ação é irreversível.", "Deseja continuar?"))
      {
      case IDYES:
         //mostra progresso
         if(prog) prog->Start(progDEFAULT);
         //cria objeto p/ remover redes BT
         remove_bt  = new TRemoveBT(apl);
         sucesso = remove_bt->Equivalenta();
         delete remove_bt;
         break;
      case IDNO:     return(true);
      case IDCANCEL: return(true);
      }
   if(prog) prog->Stop();
   return(sucesso);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormEditaRede::ImportaArquivo(void)
   {
   //variaveis locais
   AnsiString filename = "";
   VTPath     *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //define pasta default
   if(! path) return("");
   OpenDialog->InitialDir = path->DirImporta();
   //abre arquivo
   if(OpenDialog->Execute())
      {
      filename = OpenDialog->FileName;
      //verifica se o arquivo existe
      if(! FileExists(filename)) return("");
      }
   return(filename);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEditaRede::InsereFusivelTrafos(void)
   {
   //variaveis locais
   bool           sucesso = false;
   AnsiString     arquivo;
   TInsereFusivel *insere_fus;
   VTProgresso    *prog    = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

   if(prog == NULL)
      {//cria progresso
      prog = DLL_NewObjProgresso(this, this->Parent);
      if(! prog) apl->Add(prog);
      }
   //importa arquivo
   arquivo = ImportaArquivo();
   if(arquivo.IsEmpty()) return(true);
   switch(Confirma("A inserção de chaves fusíveis é feita nos trafos MT/BT",
                   "Deseja continuar?"))
      {
      case IDYES:
         //mostra progresso
         if(prog) prog->Start(progDEFAULT);
         //cria objeto p/ inserir fusiveis
         insere_fus  = new TInsereFusivel(apl);
         sucesso     = insere_fus->Executa(arquivo);
         delete insere_fus;
         break;
      case IDNO:     return(true);
      case IDCANCEL: return(true);
      }
   if(prog) prog->Stop();
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEditaRede::RemoveBT(void)
   {
   //variaveis locais
   bool        sucesso = false;
   AnsiString  arquivo;
   TRemoveBT   *remove_bt;
   VTProgresso *prog    = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

   if(prog == NULL)
      {//cria progresso
      prog = DLL_NewObjProgresso(this, this->Parent);
      if(! prog) apl->Add(prog);
      }
   switch(Confirma("A remoção das redes BT deve ser feita \n"
                   "depois de equivalentar as cargas.",
                   "Deseja continuar?"))
      {
      case IDYES:
         //mostra progresso
         if(prog) prog->Start(progDEFAULT);
         //cria objeto p/ remover redes BT
         remove_bt  = new TRemoveBT(apl);
         sucesso = remove_bt->Executa();
         delete remove_bt;
         break;
      case IDNO:     return(true);
      case IDCANCEL: return(true);
      }
   if(prog) prog->Stop();
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEditaRede::RemoveTrechos(void)
   {
   //variaveis locais
   bool            sucesso = false;
   TRemoveTrecho   *remove;
   VTProgresso     *prog    = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

   if(prog == NULL)
      {//cria progresso
      prog = DLL_NewObjProgresso(this, this->Parent);
      if(! prog) apl->Add(prog);
      }
   switch(Confirma("Essa ação é irreversível.", "Deseja continuar?"))
      {
      case IDYES:
         //mostra progresso
         if(prog) prog->Start(progDEFAULT);
         //cria objeto p/ remover trechos duplicados
         remove  = new TRemoveTrecho(apl);
         sucesso = remove->Executa();
         delete remove;
         break;
      case IDNO:     return(true);
      case IDCANCEL: return(true);
      }
   if(prog) prog->Stop();
   return(sucesso);
   }

//---------------------------------------------------------------------------

