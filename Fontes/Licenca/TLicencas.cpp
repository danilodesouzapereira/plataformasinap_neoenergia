//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TLicencas.h"
#include "..\Apl\VTApl.h"
#include "..\Cript\VTBcd.h"
#include "..\Cript\VTCript.h"
#include "..\Diretorio\DirArq.h"
#include "..\Diretorio\VTPath.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTPacote.h"
#include "..\Micro\VTMicro.h"
#include "..\Produto\VTProduto.h"
#include "..\Registro\VTRegistro.h"
#include "..\Servidor\VTServidor.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Registro.h"
#include "..\..\DLL_Inc\Micro.h"
#include "..\..\DLL_Inc\Servidor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
VTLicencas* __fastcall NewObjLicencas(VTApl *apl)
   {
   return(new TLicencas(apl));
   }

//---------------------------------------------------------------------------
__fastcall TLicencas::TLicencas(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   try{//cria objetos
      bcd        = NewObjBcd();
      cript      = NewObjCript();
      pacote     = NewObjPacote();
      micro      = DLL_NewObjMicro();
      //StrListArq = new TStringList();
      lisLICENCA = new TList();
      //inicia chave de criptografica usada p/ Cript
      cript->DefineChave("Copyright Guaraldo 2006");
      //l� arquivo local de licen�as
      LeLicencas();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TLicencas::~TLicencas(void)
   {
   //destr�i objetos
   if (bcd  )       {delete bcd;        bcd        = NULL;}
   if (cript)       {delete cript;      cript      = NULL;}
   if (pacote)      {delete pacote;     pacote     = NULL;}
   if (micro)       {delete micro;      micro      = NULL;}
   //if (StrListArq)  {delete StrListArq; StrListArq = NULL;}
   //destr�i lists e seus objetos
   if (lisLICENCA) {LimpaTList(lisLICENCA); delete lisLICENCA; lisLICENCA = NULL;}
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::AlocaLicenca(VTServidor *servidor, AnsiString login,
                                        AnsiString password, bool demo, AnsiString &diagnostico)
   { /*
   //vari�veis locais
   bool      sucesso;
   VTLicenca *licenca;
   VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

   try{//cria uma nova Licenca
      licenca              = NewObjLicenca();
      //licenca->Empresa     = ;
      licenca->CPU_ID      = micro->CPU_ID;
      licenca->Produto     = produto->CodLicenca;
      licenca->Login       = login;
      licenca->Password    = password;
      //OBS: os pr�ximos dados ser�o redefinidos pelo Servidor
      licenca->DH_alocada  = DateOf(Now());
      licenca->DH_inicio   = DateOf(Now());
      licenca->DH_controle = licenca->DH_inicio;
      licenca->DH_termino  = IncDay(licenca->DH_inicio, 30);
      licenca->Status      = sttALOCADA;
      licenca->Tipo        = ((demo) ? tipoDEMO : tipoUSER);
      licenca->Empresa     = 0;
      licenca->LoginLocal  = false;
      licenca->CtrlRemoto  = false;
      //solicita aloca��o da Licenca ao Servidor
      if (! servidor->AlocaLicenca(licenca))
         {//erro na comunica��o c/ o Servidor
         diagnostico = "N�o foi poss�vel alocar Licen�a de Uso." ;
//                       "Falha de comunica��o com o Servidor de Licen�as";
         //destr�i Licenca
         delete licenca;
         return(false);
         }
      //define diagn�stico
      diagnostico = licenca->Diagnostico();
      //verifica se a Licenca � v�lida
      if (licenca->Status == sttALOCADA)
         {//insere Licenca em lisLICENCA
         lisLICENCA->Add(licenca);
         //grava arquivo local de licen�as
         GravaLicencas();
         sucesso = true;
         }
      else
         {//destr�i Licenca
         delete licenca;
         sucesso = false;
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(sucesso);
   */
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::AlocaLicencaLocal(AnsiString login, AnsiString password,
                                             bool demo, AnsiString &diagnostico)
   {    /*
   //vari�veis locais
   bool       sucesso;
   VTServidor *servidor;

   //inicia diagnostico default
   diagnostico = "N�o foi poss�vel alocar Licen�a de Uso.";
   try{//cria Servidor de licen�a local
      if ((servidor = DLL_NewObjServidorLocal()) != NULL)
         {//aloca Licenca
         sucesso = AlocaLicenca(servidor, login, password, demo, diagnostico);
         //destr�i Servidor
         delete servidor;
         }
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   */
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::AlocaLicencaRemota(AnsiString login, AnsiString password,
                                              bool demo, AnsiString &diagnostico)
   {  /*
   //vari�veis locais
   bool       sucesso;
   VTServidor *servidor;

   //inicia diagnostico default
   diagnostico = "N�o foi poss�vel alocar Licen�a de Uso.";
   try{//cria Servidor de licen�a remoto
      if ((servidor = DLL_NewObjServidorRemoto()) != NULL)
         {//aloca Licenca
         sucesso = AlocaLicenca(servidor, login, password, demo, diagnostico);
         //destr�i Servidor
         delete servidor;
         }
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   */
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::Criptografa(AnsiString &txt)
   {
   //vari�veis locais
   int     len;
//   struct  {char bin[512], hex[1024];} buf;
   struct  {char bin[1024], hex[2048];} buf;

   //copia texto a ser criptografado em buf.orig sem o EOF
   len = txt.Length();
   if (len > sizeof(buf.bin)) return(false);
   memcpy(buf.bin, txt.c_str(), len);
   //criptografa buffer
   if (! cript->Executa(buf.bin, len)) return(false);
   //converte buffer criptografado de bin p/ hexa, j� com EOF
   if (! bcd->BinToHexa(buf.bin, len, buf.hex, sizeof(buf.hex))) return(false);
   //atualiza txt
   txt = buf.hex;
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::Descriptografa(AnsiString &txt)
   {
   //vari�veis locais
   int     len;
//   struct  {char bin[512], hex[1024];} buf;
   struct  {char bin[1024], hex[2048];} buf;

   //copia texto (que est� em hexa) em buf.dest sem o EOF
   len = txt.Length();
   if (len > sizeof(buf.hex)) return(false);
   memcpy(buf.hex, txt.c_str(), len);
   //converte texto em buf.dest de hexa p/ bin, j� com EOF
   if (! bcd->HexaToBin(buf.hex, len, buf.bin, sizeof(buf.bin))) return(false);
   //descriprografa buffer
   if (! cript->Executa(buf.bin, len/2)) return(false);
   //atualiza txt
   txt = buf.bin;
   return(true);
   }

//-----------------------------------------------------------------------------
VTLicenca* __fastcall TLicencas::ExisteLicenca(void)
   {
   //vari�veis locais
   VTLicenca *licenca;
   VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

   //verifica se j� foi lido o arquivo de licen�as
   if (lisLICENCA->Count == 0)
      {//l� arqruivo de licen�as
      LeLicencas();
      }
   //verifica se existe uma Licenca p/ o Produto
   for (int n = 0; n < lisLICENCA->Count; n++)
      {
      licenca = (VTLicenca*)lisLICENCA->Items[n];
      if (produto->Codigo.AnsiCompareIC(licenca->Produto)     == 0) return(licenca);
      if (produto->CodLicenca.AnsiCompareIC(licenca->Produto) == 0) return(licenca);
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
VTLicenca* __fastcall TLicencas::ExisteLicenca(AnsiString produto)
   {
   //vari�veis locais
   VTLicenca *licenca;

   //verifica se existe uma Licenca p/ o Produto
   for (int n = 0; n < lisLICENCA->Count; n++)
      {
      licenca = (VTLicenca*)lisLICENCA->Items[n];
      if (licenca->Produto.AnsiCompareIC(produto) == 0) return(licenca);
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
void __fastcall TLicencas::GravaLicencas(void)
   {
   //grava licencas em arquivo
   GravaLicencasArquivo();
   //grava licencas no registro do Windows
   GravaLicencasRegWin();
   }

//-----------------------------------------------------------------------------
void __fastcall TLicencas::GravaLicencasArquivo(void)
   {
   //vari�veis locais
   AnsiString filename, txt;
   TStrings   *lines;
   VTLicenca  *licenca;
   VTPath     *path    = (VTPath*)apl->GetObject(__classid(VTPath));

   //define pathname do arquivo de licencas
   filename = path->DirBin + "\\" + ARQ_LICENCA;
   //cria objeto TStringList
   if ((lines = new TStringList()) == NULL) return;
   //loop para todas licen�as
   for (int n = 0; n < lisLICENCA->Count; n++)
      {
      licenca = (VTLicenca*)lisLICENCA->Items[n];
      //monta string criptografado c/ dados da Licenca
      licenca->WriteToPacote(pacote);
      txt = pacote->CamposAsStr;
      if (! Criptografa(txt)) continue;
      //salva texto da licen�a no Strings
      lines->Add(txt);
      }
   //grava arquivo com licen�as criptografadas
   lines->SaveToFile(filename);
   //destr�i objeto
   delete lines;
   }

//-----------------------------------------------------------------------------
void __fastcall TLicencas::GravaLicencasRegWin(void)
   {
   //vari�veis locais
   AnsiString txt;
   VTLicenca  *licenca;
   VTRegistro *registro;
   VTProduto  *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

   //cria objeto Registro
   if ((registro = DLL_NewObjRegistro(produto->Sinapsis)) == NULL) return;
   //elimina Secao
   registro->RetiraSecao("licencas");
   //grava Licencas no registro do Windows
   for (int n = 0; n < lisLICENCA->Count; n++)
      {
      licenca = (VTLicenca*)lisLICENCA->Items[n];
      //monta string criptografado c/ dados da Licenca
      licenca->WriteToPacote(pacote);
      txt = pacote->CamposAsStr;
      if (! Criptografa(txt)) continue;
      //atualiza registro c/ dados da Licenca
      registro->GravaInfo("licencas", "licenca_" + IntToStr(n+1), txt);
      }
   //destr�i objeto
   delete registro;
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::InsereLicenca(VTLicenca *licenca)
{
	if(licenca == NULL)
		return(false);
	//verifica se a licenca existe na lista
	if(lisLICENCA->IndexOf(licenca) >= 0)
		return(false);
	//insere licenca da lista
	lisLICENCA->Add(licenca);
	return(true);
}

//-----------------------------------------------------------------------------
void __fastcall TLicencas::LeLicencas(void)
   {
   //l� Licencas do registro do Windows
   LeLicencasRegWin();
   //l� Licencas do arquivo texto
   LeLicencasArquivo();
   }

//-----------------------------------------------------------------------------
void __fastcall TLicencas::LeLicencasArquivo(void)
   {
   //vari�veis locais
   AnsiString filename, txt;
   TStrings   *lines;
   VTLicenca  *licenca;
   VTPath     *path    = (VTPath*)apl->GetObject(__classid(VTPath));

   //verifica se existe o arquivo de licencas
   filename = path->DirBin + "\\" + ARQ_LICENCA;
   if (! FileExists(filename)) return;
   //cria objeto TStringList
   if ((lines = new TStringList()) == NULL) return;
   //l� arquivo com licenc�as criptografadas
   lines->LoadFromFile(filename);
   //loop para todas licen�as
   for (int n = 0; n < lines->Count; n++)
      {//descriptograva txt
      if (! Descriptografa(txt = lines->Strings[n])) continue;
      //monta Pacote c/ campos da linha do arquivo
      pacote->CamposAsStr = txt;
      //cria um novo objeto Licenca
      if ((licenca = NewObjLicenca()) == NULL) continue;
      //l� dados da Licenca do Pacote
      licenca->ReadFromPacote(pacote);
      //insere a licenca em lisLICENCA somente se n�o for repetida
      SalvaLicenca(licenca);
      }
   //destr�i objeto
   delete lines;
   }

//-----------------------------------------------------------------------------
void __fastcall TLicencas::LeLicencasRegWin(void)
   {
   //vari�veis locais
   AnsiString txt;
   VTLicenca  *licenca;
   VTRegistro *registro;
   VTProduto  *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

   //cria objeto Registro
   if ((registro = DLL_NewObjRegistro(produto->Sinapsis)) == NULL) return;
   //l� string criptografado c/ dados das Licencas do registro do Windows
   for (int n = 0; registro->ExisteInfo("licencas", "licenca_" + IntToStr(n+1), txt); n++)
      {//descriptograva txt
      if (! Descriptografa(txt)) continue;
      //monta Pacote c/ campos da linha do arquivo
      pacote->CamposAsStr = txt;
      //cria um novo objeto Licenca e insere em lisLICENCA
      if ((licenca = NewObjLicenca()) == NULL) continue;
      //l� dados da Licenca do Pacote
      licenca->ReadFromPacote(pacote);
      //insere a licenca em lisLICENCA somente se n�o for repetida
      SalvaLicenca(licenca);
      }
   //destr�i objeto
   delete registro;
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::LiberaLicenca(AnsiString &diagnostico)
   {  /*
   //vari�veis locais
   bool       sucesso;
   VTServidor *servidor;
   VTLicenca  *licenca;
   VTProduto  *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

   //verifica se existe uma Licenca p/ o produto
   if ((licenca = ExisteLicenca()) == NULL)
      {//prepara texto de diagn�stico
      diagnostico = "N�o existe Licen�a de Uso para o programa " + produto->Codigo;
      return(false);
      }
   //verifica validade da Licenca (mudando seu Status, se necess�rio)
   licenca->VerificaCPU_ID(micro->CPU_ID);
   //verifica se Licenca alocada local ou remotamente
   if (licenca->CtrlRemoto)
      {
      servidor = DLL_NewObjServidorRemoto();
      }
   else
      {
      servidor = DLL_NewObjServidorLocal();
      }
   //solicita libera��o da Licenca ao Servidor
   if (! servidor->LiberaLicenca(licenca))
      {//erro na comunica��o c/ o Servidor
      diagnostico = "N�o foi poss�vel liberar Licen�a de Uso.";
//                    "Falha de comunica��o com o Servidor de Licen�as";
      sucesso = false;
      }
   else
      {//elimina Licenca da lista de licencas e destr�i objeto
      lisLICENCA->Remove(licenca);
      delete licenca;
      //define diagn�stico
      diagnostico = "Licen�a de Uso liberada.\n"
                    "Para utilizar o programa " + produto->Codigo + " � necess�rio alocar uma Licen�a de Uso.";
      //grava arquivo de licen�as
      GravaLicencas();
      sucesso = true;
      }
   //destr�i objeto Servidor
   delete servidor;
   return(sucesso);
   */
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::LicencaTipoDemo(void)
   {
   //vari�veis locais
   VTLicenca *licenca;

   //obt�m Licenca
   if ((licenca = ExisteLicenca()) == NULL) return(true);
   //verifica se a Licenca � do tipo Demo
   return(licenca->Tipo == tipoDEMO);
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::LicencaTipoDidatica(void)
   {
   //vari�veis locais
   VTLicenca *licenca;

   //obt�m Licenca
   if ((licenca = ExisteLicenca()) == NULL) return(true);
   //verifica se a Licenca � do tipo Demo
   return(licenca->Tipo == tipoDIDATICA);
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::PM_GetLoginLocal(void)
   {
   //vari�veis locais
   VTLicenca *licenca;

   //obt�m Licenca
   if ((licenca = ExisteLicenca()) == NULL) return(false);
   //verifica se a Licenca est� configurada c/ Login local
   return(licenca->LoginLocal);
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::PM_GetValidadeLimitada(void)
	{
	//vari�veis locais
	VTLicenca *licenca;

	//obt�m Licenca
	if ((licenca = ExisteLicenca()) == NULL) return(true);
	//verifica se a Licenca est� configurada c/ validade limitada
	return(licenca->Validade >= 0);
	}

//-----------------------------------------------------------------------------
void __fastcall TLicencas::PM_SetLoginLocal(bool enabled)
   {
   //vari�veis locais
   VTLicenca *licenca;

   //obt�m Licenca
   if ((licenca = ExisteLicenca()) == NULL) return;
   //atualiza Licenca
   licenca->LoginLocal = enabled;
   //grava arquivo c/ dados da Licenca
   GravaLicencas();
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::RemoveLicenca(VTLicenca *licenca)
{
	if(licenca == NULL)
		return(false);
	//verifica se a licenca existe na lista
	if(lisLICENCA->IndexOf(licenca) < 0)
		return(false);
	//retira licenca da lista
	lisLICENCA->Remove(licenca);
	return(true);
}

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::RenovaLicenca(AnsiString &diagnostico)
   {
   return(true);
   }

//-----------------------------------------------------------------------------
void __fastcall TLicencas::SalvaLicenca(VTLicenca *licenca)
   {
   //verifica se j� existe uma Licenca de mesmo ID em lisLICENCA
   if (ExisteLicenca(licenca->Produto) != NULL)
      {//destr�i Licenca repetida
      delete licenca;
      }
   else
      {//insere Licenca em lisLICENCA
      lisLICENCA->Add(licenca);
      }
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::ValidaLicenca(AnsiString &diagnostico)
   {
   //vari�veis locais
   VTLicenca *licenca;
   VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

   //verifica se existe uma Licenca p/ o produto
   if ((licenca = ExisteLicenca()) == NULL)
	  {//prepara texto de diagn�stico
	  diagnostico = "N�o existe Licen�a de Uso para o programa " + produto->Codigo;
	  return(false);
	  }
   //verifica validade da Licenca (mudando seu Status, se necess�rio)
   licenca->VerificaPeriodoValidade();
   licenca->VerificaCPU_ID(micro->CPU_ID);
   //define diagn�stico
   diagnostico = licenca->Diagnostico();
   //grava arquivo de licen�as
   GravaLicencas();
   //retorna true somente se Licenca alocada
   //return(licenca->Status == sttALOCADA);
   switch(licenca->Status)
	  {
	  case sttALOCADA:   return(true);
	  case sttEXPIRADA:  return(licenca->Tipo == tipoMASTER);
	  case sttBLOQUEADA: return(false);
	  }
	return(false);
	}

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::ValidaOperacoes(AnsiString &diagnostico)
   {
//   //vari�veis locais
//   VTLicenca *licenca;
//   VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));
//
//   //verifica se existe uma Licenca p/ o produto
//   if ((licenca = ExisteLicenca()) == NULL)
//	  {//prepara texto de diagn�stico
//	  diagnostico = "N�o existe Licen�a de Uso para o programa " + produto->Codigo;
//	  return(false);
//	  }
//   //verifica se a licen�a possui opera��es
//   if(licenca->Operacoes.Length == 0)
//   {
//	  diagnostico = "A licen�a de uso alocada n�o possui opera��es e ser� liberada para realoca��o";
//	  return(false);
//   }
   return(true);
   }

//-----------------------------------------------------------------------------
//eof



