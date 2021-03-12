//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <dir.h>
#include <winbase.h>
//#include <stdio.h>
#include <Registry.hpp>
#include "TDir.h"
#include "TDelDir.h"
#include "DirArq.h"
#include "TFormCriaDir.h"
#include "TFormSelDir.h"
#include "VTPath.h"
#include "..\Apl\VTApl.h"
#include "..\Produto\VTProduto.h"
#include "..\Registro\VTRegistro.h"
//#include "..\Thread\VTMultiThread.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Registro.h"
//#include "..\..\DLL_Inc\Thread.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global
VTDir* __fastcall NewObjDir(VTApl *apl)
   {
   return(new TDir(apl));
   }

//---------------------------------------------------------------------------
__fastcall TDir::TDir(VTApl *apl)
   {
   //variáveis locais
   AnsiString dir_base, arq_rede;

   //salva ponteiro p/ objetos externos
   this->apl = apl;
   path      = (VTPath*)apl->GetObject(__classid(VTPath));
   produto   = (VTProduto*)apl->GetObject(__classid(VTProduto));
   //salva diretório corrente em Path
   path->DirBin = GetCurrentDir();
   //lê arq. c/ nomes dos diretório de instalaçao e da base de dados
   LeArqDiretorios();
   //verifica se o diretório de instalação é válido
   dir_base = path->DirBase;
   if (! ValidaDirBase(dir_base))
      {//diretório de instalação inválido: diretório de instalação como o dir. que contêm dirBIN
      SetCurrentDir(path->DirBin);
      dir_base = AnsiString("..\\");
      if (SetCurrentDir(dir_base))
         {//mudou o diretório, logo o diretório existe
         dir_base = GetCurrentDir();
         //verifica se dirBASE é válido
         if (! ValidaDirBase(dir_base)) dir_base = "";
         }
      }
   path->DirBase = dir_base;
   //verifica se existe o arquivo da base da rede
   if (! FileExists(path->ArqRede))
      {//não existe o arquivo: assume arquivo da rede default
      path->ArqRede = path->DirRedes() + "\\" + DBNAME_REDE;
      }
   }

//---------------------------------------------------------------------------
__fastcall TDir::~TDir(void)
   {
   //grava arquivo c/ nomes de dirBASE e dirREDE
   GravaArqDiretorios();
	}

//---------------------------------------------------------------------------
bool __fastcall TDir::Arquivos(AnsiString dir_orig, TStrings *files, AnsiString filtro)
	{
	//vars locais
	TSearchRec sr;
	int        atrib = 0;//faAnyFile;

	//proteção: verifica se existe o diretório origem
	if (! DirectoryExists(dir_orig)) return(false);
	//proteção: se filtro não definido, assume todos arquivos
	if (filtro.IsEmpty()) filtro = "*.*";
	dir_orig += "\\" + filtro;
	//localiza primeira entrada no diretório
	for (int erro = FindFirst(dir_orig, atrib, sr); erro == 0; erro = FindNext(sr))
		{//descarta diretórios '.' e '..'
		if ((sr.Name == ".")||(sr.Name == "..")) continue;
		//salva nome do arquivo em files
		files->Add(sr.Name);
		}
	//libera recursos alocados por FindFirst()
	FindClose(sr);
	return(true);
	}

/*
//---------------------------------------------------------------------------
bool __fastcall TDir::Arquivos(AnsiString dir_orig, TStrings *files, AnsiString filtro)
	{
	//vars locais
	struct ffblk sffblk;
	int          i;

	//proteção: verifica se existe o diretório origem
	if (! DirectoryExists(dir_orig)) return(false);
	//proteção: se filtro não definido, assume todos arquivos
	if (filtro.IsEmpty()) filtro = "*.*";
	//posiciona no diretório origem
	if (chdir(dir_orig.c_str()) != 0) return false;   //dir origem inexistente
	//busca primeiro arquivo do dirétório origem
	i = findfirst(filtro.c_str(), &sffblk, 0);
	while (!i)
		{//salva nome do arquivo em files
		files->Add(sffblk.ff_name);
		//determina próximo arquivo no diretório origem
		i = findnext(&sffblk);
	  }
   return(true);
	}
*/
//---------------------------------------------------------------------------
bool __fastcall TDir::CopiaArquivo(AnsiString arq_orig, AnsiString arq_dest)
   {
   //variáveis locais
	WideString  ws_orig = arq_orig;
   WideString  ws_dest = arq_dest;

   CopyFile(ws_orig.c_bstr(), ws_dest.c_bstr(), false);
   return(FileExists(arq_dest));
   }

//---------------------------------------------------------------------------
bool __fastcall TDir::CopiaDiretorio(AnsiString dir_orig, AnsiString dir_dest, AnsiString filtro)
	{
	//vars locais
	struct ffblk ffblk;
	int          i;
	WideString   arq_orig, arq_dest;

	//proteção: verifica se existe o diretório origem
	if (! DirectoryExists(dir_orig)) return(false);
	//proteção: se filtro não definido, assume todos arquivos
	if (filtro.IsEmpty()) filtro = "*.*";
	//posiciona no diretório origem
	if (chdir(dir_orig.c_str()) != 0) return false;   //dir origem inexistente
	//busca primeiro arquivo do dirétório origem
	i = findfirst(filtro.c_str(), &ffblk, 0);
	while (!i)
		{
		arq_orig = dir_orig + "\\" + ffblk.ff_name;
      arq_dest = dir_dest + "\\" + ffblk.ff_name;
      CopyFile(arq_orig.c_bstr(), arq_dest.c_bstr(), false);
		//determina próximo arquivo no diretório origem
		i = findnext(&ffblk);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TDir::CriaDiretorio(AnsiString dir, AnsiString &subdir)
	{
	//variáveis locais
	TformCriaDir *form;
	bool         resp = false;

	if ((form = new TformCriaDir(NULL, dir, subdir)) == NULL)
		{
		Erro("Não há memória disponível");
		return(false);
		}
	if (form->ShowModal() == mrOk)
		{//obtêm nome do diretório criado
		subdir = form->Dir();
      resp   = true;
      }
   //destrói o form
   delete form;

   return(resp);
   }

//---------------------------------------------------------------------------
bool __fastcall TDir::CriaCopiaRedeEmpresa(AnsiString &arq_dest, bool consulta_usuario)
   {
   //garante que arq_dest seja um nome de aqruivo que ainda não existe
   arq_dest = DefineNomeArquivoInexistente(arq_dest);
   //solicita que usuário defina o nome do arquivo
   if (consulta_usuario)
      {
      if (! SelArquivoRedeToSave(arq_dest)) return(false);
      }
   //copia arquivo padrão da rede da empresa
   if (! CopiaArquivo(path->ArqRedeEmpresa(), arq_dest)) return(false);
   //assume a nova rede criada como rede selecionada
   path->ArqRede = arq_dest;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TDir::CriaCopiaRedeSinap(AnsiString &arq_dest, bool consulta_usuario)
   {
   //garante que arq_dest seja um nome de aqruivo que ainda não existe
   arq_dest = DefineNomeArquivoInexistente(arq_dest);
   //solicita que usuário defina o nome do arquivo
   if (consulta_usuario)
      {
      if (! SelArquivoRedeToSave(arq_dest)) return(false);
      }
   //copia arquivo padrão da rede Sinap
   if (! CopiaArquivo(path->ArqRedeSinap(), arq_dest)) return(false);
   //assume a nova rede criada como rede selecionada
   path->ArqRede = arq_dest;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TDir::CriaDiretorioRede(AnsiString &dir_rede, bool consulta_usuario)
   {
   //variáveis locais
   AnsiString dir_base, txt;

   //proteção
   if (produto == NULL) return(false);
   //valida diretório de instalação
   dir_base = path->DirBase;
   if (! ValidaDirBase(dir_base))
      {//gera aviso p/ usuário
      if (consulta_usuario)
         {
         txt  = "O diretório de instalação do programa " + produto->Codigo + " não pode ser encontrado.\n";
         txt += "Selecione o diretório " + produto->Codigo + " através do menu:\n";
         txt += "Rede\\Diretório de instalação\n";
         Aviso(txt);
         }
      return(false);
      }
   //verifica se o arquivo da rede está definido
   if (dir_rede.IsEmpty()) dir_rede = "Rede";
   if (DirectoryExists(dir_base + "\\" + dir_rede))
      {//redefine nome do diretório da rede
      dir_rede = DefineNomeDiretorioInexistente(dir_base, dir_rede);
      }
   //cria diretório base p/ a nova rede
   if (consulta_usuario)
      {
      if (! CriaDiretorio(dir_base, dir_rede)) return(false);
      }
   else
      {
      dir_rede = dir_base + "\\" + dir_rede;
      if (! CreateDir(dir_rede)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TDir::DefineNomeArquivoInexistente(AnsiString pathname)
   {
   //variáveis locais
   int        count = 1;
   AnsiString file_name, file_ext;

   //verifica se existe o arquivo
   if (! FileExists(pathname)) return(pathname);
   //define um nome pathname que ainda não exista
   file_ext  = ExtractFileExt(pathname);
   file_name = ChangeFileExt(pathname, "");
   do {
      pathname = file_name + "_" + IntToStr(count++) + file_ext;
      }while (FileExists(pathname));
   return(pathname);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TDir::DefineNomeDiretorioInexistente(AnsiString dir_base, AnsiString radical)
   {
   //variáveis locais
   int        count;
   AnsiString nome_dir;

   //redefine nome do diretório da rede
   count = 1;
   do {
      nome_dir = radical + "_" + IntToStr(count++);
      } while (DirectoryExists(dir_base + "\\" + nome_dir));
   return(nome_dir);
   }

//---------------------------------------------------------------------------
bool __fastcall TDir::DeleteDirectory(AnsiString dir)
   {
   //variáveis locais
   AnsiString pathName, arq, unidade;
   TSearchRec sr;
   int        atrib = faAnyFile;

   //proteção p/ não apagar um disco inteiro
   for (char raiz = 'A'; raiz <= 'Z'; raiz++)
	  {
	  unidade = AnsiString(raiz) + ":\\" ;
	  if (dir.AnsiCompareIC(unidade) == 0)
		 {
		 Erro("Tentativa de apagar a unidade de disco " + dir);
		 return(false);
		 }
	  }
   //verifica se existe o diretório
   if (! DirectoryExists(dir)) return(false);
   //verifica se o diretório a ser eliminado é o diretório corrente
   if (dir.AnsiCompareIC(GetCurrentDir()) == 0)
      {//muda diretório corrente
	  SetCurrentDir("..");
      }
   //procura por todos os subdiretórios e arquivos
   pathName = dir + "\\*.*";
   //localiza primeira entrada no diretório
   for (int erro = FindFirst(pathName, atrib, sr); erro == 0; erro = FindNext(sr))
      {
      //descarta diretórios '.' e '..'
	  if ((sr.Name == ".")||(sr.Name == "..")) continue;
      //monta nome do arquivo ou diretório encontrado
      arq = dir + "\\" + sr.Name;
      //verifica se é um diretório
	  if ((sr.Attr & faDirectory) == faDirectory)
         {//diretório: elimina o subdiretório (usando recursão)
         DeleteDirectory(arq);
         }
	  else
		 {//arquivo: elimina o arquivo
		 DeleteFile(arq);
		 }
	  }
   FindClose(sr); //libera recursos alocados por FindFirst()
   //elimina o diretório propriamente
   RemoveDir(dir);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TDir::DeleteRede(void)
   {
   //variáveis locais
   AnsiString arq_rede;

   //seleciona Rede a ser eliminada
   if (! SelArquivoRedeToOpen(arq_rede)) return(false);
   //proteção: verifica se a rede existe
   if (! FileExists(arq_rede))
	  {
	  Aviso("Não existe o arquivo da rede selecionada");
	  return(false);
      }
   //verifica se é a rede atualmente selecionada
   if (arq_rede.AnsiCompareIC(path->ArqRede) == 0)
      {
      Aviso(arq_rede + "\nRede atualmente em uso pelo programa.\nNão é possível eliminar a rede");
      return(false);
      }
   //pede confirmação do usuário
   if (Confirma(arq_rede, "Confirma exclusão da rede ?") != IDYES) return(false);
   //elimina toda a árvore de diretórios da rede
   return(DeleteFile(arq_rede));
   }

//---------------------------------------------------------------------------
bool __fastcall TDir::DeleteTmp(void)
   {
   //variaveis locais
   TDelDir          *deldir = (TDelDir*)apl->GetObject(__classid(TDelDir));

   try{//cria objeto TDelDir e insere no Apl (ja estara executando e o Apl vai destruir)
	  //sem thread: roda direto
	  if(deldir == NULL)
		 {
		 if ((deldir = new TDelDir(apl)) == NULL)
			{
			return false;
			}
//		 apl->Add(deldir);
		 }
	  //executa MultiThread
//	  deldir->Executa();
	  //destroi objetos criados
	  delete deldir;
	  }catch(Exception &e)
		 {
		 return false;
		 }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TDir::GravaArqDiretorios(void)
   {
   //variáveis locais
   VTRegistro *registro;
   AnsiString dir_base = path->DirBase;
   AnsiString arq_rede = path->ArqRede;

   //proteção
   if (produto == NULL) return;
   //cria objeto de acesso ao Registro do Windows
   if ((registro = DLL_NewObjRegistro(produto->Sinapsis)) == NULL) return;
   //lê diretório base
   registro->GravaInfo(produto->WinRegister, "dir_base", dir_base);
   registro->GravaInfo(produto->WinRegister, "arq_rede", arq_rede);
   //destrói objeto
   delete registro;
   }

//---------------------------------------------------------------------------
void __fastcall TDir::LeArqDiretorios(void)
   {
   //variáveis locais
   AnsiString  dir_base, arq_rede;
   VTRegistro *registro;

   //proteção
   if (produto == NULL) return;
   //cria objeto de acesso ao Registro do Windows
   if ((registro = DLL_NewObjRegistro(produto->Sinapsis)) == NULL) return;
   //lê diretório base
   registro->ExisteInfo(produto->WinRegister, "dir_base", dir_base);
   registro->ExisteInfo(produto->WinRegister, "arq_rede", arq_rede);
   //atualiza objeto Path
   path->DirBase = dir_base;
   path->ArqRede = arq_rede;
   //destrói objeto
   delete registro;
   }

//---------------------------------------------------------------------------
bool __fastcall  TDir::LisDirectory(AnsiString dir, TStringList *slisDir)
{
   //variáveis locais
   AnsiString pathName, arq, unidade;
   TSearchRec sr;
   int        atrib = faAnyFile;

   //verifica se existe o diretório
   if (! DirectoryExists(dir)) return(false);
   //verifica se o diretório a ser listado é o diretório corrente
   if (dir.AnsiCompareIC(GetCurrentDir()) == 0)
	  {//muda diretório corrente
	  SetCurrentDir("..");
	  }
   //procura por todos os subdiretórios e arquivos
   pathName = dir + "\\*.*";
   //localiza primeira entrada no diretório
   for (int erro = FindFirst(pathName, atrib, sr); erro == 0; erro = FindNext(sr))
	  {
	  //descarta diretórios '.' e '..'
	  if ((sr.Name == ".")||(sr.Name == "..")) continue;
	  //monta nome do arquivo ou diretório encontrado
	  arq = dir + "\\" + sr.Name;
	  //verifica se é um diretório
	  if ((sr.Attr & faDirectory) == faDirectory)
		 {//diretório: insere no stringLIst
		 slisDir->Add(arq);
		 }
	  }
   FindClose(sr); //libera recursos alocados por FindFirst()

   return(true);
}

//---------------------------------------------------------------------------
AnsiString __fastcall TDir::PM_GetDirBase(void)
   {
   return(path->DirBase);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TDir::PM_GetArqRede(void)
   {
   return(path->ArqRede);
   }

//---------------------------------------------------------------------------
void __fastcall TDir::PM_SetArqRede(AnsiString arq_rede)
   {
   //salva diretório da rede
   path->ArqRede = arq_rede;
   }

//---------------------------------------------------------------------------
bool __fastcall TDir::SelArquivoRedeToOpen(AnsiString &arq_rede)
   {
   //variáveis locais
   bool         sucesso = false;
   TOpenDialog *OpenDialog;

   try{//cria objeto TOpenDialog
      if ((OpenDialog = new TOpenDialog(NULL)) == NULL) return(false);
      OpenDialog->Title = "Selecionar Base de Dados";
      //define diretório inicial
      if (arq_rede.IsEmpty()) OpenDialog->InitialDir = path->DirRedes();
      else                    OpenDialog->InitialDir = ExtractFileDir(arq_rede);
      OpenDialog->FileName    = ExtractFileName(arq_rede);
      OpenDialog->DefaultExt  = ".mdb";
      OpenDialog->Filter      =  "Base de dados Sinap (*.mdb)|*.mdb|Todos arquivos (*.*)|*.*;";
      OpenDialog->FilterIndex = 1;
      if (OpenDialog->Execute())
         {
         arq_rede = OpenDialog->FileName;
         sucesso  = true;
         }
      //destrói objeto TOpenDialog
      delete OpenDialog;
      }catch(Exception &e)
         {
         return(false);
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TDir::SelArquivoRedeToSave(AnsiString &arq_rede)
   {
   //variáveis locais
   bool         sucesso = false;
   TSaveDialog *SaveDialog;


   try{//cria objeto TOpenDialog
      if ((SaveDialog = new TSaveDialog(NULL)) == NULL) return(false);
      SaveDialog->Title = "Criar Nova Base de Dados";
      //define diretório inicial
      if (arq_rede.IsEmpty()) SaveDialog->InitialDir = path->DirRedes();
      else                    SaveDialog->InitialDir = ExtractFileDir(arq_rede);
      SaveDialog->FileName    = ExtractFileName(arq_rede);
      SaveDialog->DefaultExt  = ".mdb";
      SaveDialog->Filter      =  "Base de dados Sinap (*.mdb)|*.mdb|Todos arquivos (*.*)|*.*;";
      SaveDialog->FilterIndex = 1;
      //FKM 2015.09.17 - Prompt de overwrite
      SaveDialog->Options << ofOverwritePrompt;
      //FKM 2015.09.17
      if (SaveDialog->Execute())
         {
         arq_rede = SaveDialog->FileName;
         sucesso  = true;
         }
      //destrói objeto TOpenDialog
      delete SaveDialog;
      }catch(Exception &e)
         {
         return(false);
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TDir::SelDir(AnsiString &dirname)
   {
   //variáveis locais
   TformSelDir *FormSelDir;

   //cria form modal de seleção de diretório
   FormSelDir = new TformSelDir(NULL, dirname);
   if (FormSelDir == NULL)
      {
      Erro("Não há memória disponível");
      return(false);
      }
   //se selecionou Dir, atualiza diretório
   if (FormSelDir->ShowModal() == mrOk)
      {
      dirname = FormSelDir->Dir();
      }
   //destró ForSelDir
   delete FormSelDir;

   return(DirectoryExists(dirname));
   }

//---------------------------------------------------------------------------
bool __fastcall TDir::SelDirBase(void)
   {
   //variáveis locais
   AnsiString dir_aux;
   AnsiString dir  = "";

   //verifica se existe diretório de execução
   dir_aux  = path->DirBin;
   if (DirectoryExists(dir_aux)) dir = dir_aux ;
   //verifica se existe diretório de instalação
   dir_aux  = path->DirBase;
   if (DirectoryExists(dir_aux)) dir = dir_aux;
   //cria form modal de seleção de diretório
   TformSelDir *frmSel = new TformSelDir(NULL, dir);
   if (frmSel == NULL)
      {
      Erro("Não há memória disponível");
      return(false);
      }
   //se selecionou Dir, atualiza diretório
   if (frmSel->ShowModal() == mrOk)
      {
      dir = frmSel->Dir();
      path->DirBase = dir;
      }
   //destrói objeto criado
   delete frmSel;
   //retorna diretório selecionado
   return(ValidaDirBase(dir));
   }

//---------------------------------------------------------------------------
bool __fastcall TDir::ValidaArqRede(void)
   {
   //variáveis locais
   AnsiString txt1, txt2, arq_rede;

   //proteção
   if (produto == NULL) return(false);
   // valida diretório de instalação
   if (! ValidaDirBase(path->DirBase))
      {//gera aviso p/ usuário
      txt1  = "O Diretório de Instalação do programa " + produto->Codigo + " não pode ser encontrado.\n";
      txt1 += "Nenhuma funcionalidade do programa  " + produto->Codigo + " está disponível.\n";
      txt2  = "Deseja selecionar o Diretório de Instalação?\n";
      if (Confirma(txt1, txt2) != IDYES) return(false);
      //abre janela p/ seleção do diretório
      if (! SelDirBase()) return(false);
      }
   //verifica se existe arquivo da rede
   arq_rede = path->ArqRede;
   if (! ValidaArqRede(arq_rede))
      {
      if (! SelArquivoRedeToOpen(arq_rede)) return(false);
      //redefine arquivo da rede
      path->ArqRede = arq_rede;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TDir::ValidaArqRede(AnsiString arq_rede)
   {
   return(FileExists(arq_rede));
   }

//---------------------------------------------------------------------------
bool __fastcall TDir::ValidaDiretorios(void)
   {
   //variáveis locais
   AnsiString txt1, txt2;

   //valida diretório dirBASE
   if (ValidaDirBase(path->DirBase)) return(true);
   //gera aviso p/ usuário
   txt1  = "O Diretório de Instalação do programa " + produto->Codigo + " não pode ser encontrado.\n";
   txt1 += "Nenhuma funcionalidade do programa  " + produto->Codigo + " está disponível.\n";
   txt2  = "Deseja selecionar o Diretório de Instalação?\n";
   if (Confirma(txt1, txt2) != IDYES) return(false);
   //abre janela p/ seleção do diretório
   return(SelDirBase());
   }

//---------------------------------------------------------------------------
bool __fastcall TDir::ValidaDirBase(void)
   {
   return(ValidaDirBase(path->DirBase));
   }

//---------------------------------------------------------------------------
bool __fastcall TDir::ValidaDirBase(AnsiString dir)
   {
   //variáveis locais
   AnsiString subdir;

   //verifica se o nome do diretório é válido
   if (dir.IsEmpty()) return(false);
   //verifica se o diretório existe
   if (! DirectoryExists(dir)) return(false);
   //verifica se o diretório contêm o subdiretório DIR_DAT
   subdir = dir + "\\" + DIR_DAT;
   return(DirectoryExists(subdir));
   }

//---------------------------------------------------------------------------
//eof
