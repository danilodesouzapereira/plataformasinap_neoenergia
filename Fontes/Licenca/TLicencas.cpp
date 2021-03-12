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
      //lê arquivo local de licenças
      LeLicencas();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TLicencas::~TLicencas(void)
   {
   //destrói objetos
   if (bcd  )       {delete bcd;        bcd        = NULL;}
   if (cript)       {delete cript;      cript      = NULL;}
   if (pacote)      {delete pacote;     pacote     = NULL;}
   if (micro)       {delete micro;      micro      = NULL;}
   //if (StrListArq)  {delete StrListArq; StrListArq = NULL;}
   //destrói lists e seus objetos
   if (lisLICENCA) {LimpaTList(lisLICENCA); delete lisLICENCA; lisLICENCA = NULL;}
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::AlocaLicenca(VTServidor *servidor, AnsiString login,
                                        AnsiString password, bool demo, AnsiString &diagnostico)
   { /*
   //variáveis locais
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
      //OBS: os próximos dados serão redefinidos pelo Servidor
      licenca->DH_alocada  = DateOf(Now());
      licenca->DH_inicio   = DateOf(Now());
      licenca->DH_controle = licenca->DH_inicio;
      licenca->DH_termino  = IncDay(licenca->DH_inicio, 30);
      licenca->Status      = sttALOCADA;
      licenca->Tipo        = ((demo) ? tipoDEMO : tipoUSER);
      licenca->Empresa     = 0;
      licenca->LoginLocal  = false;
      licenca->CtrlRemoto  = false;
      //solicita alocação da Licenca ao Servidor
      if (! servidor->AlocaLicenca(licenca))
         {//erro na comunicação c/ o Servidor
         diagnostico = "Não foi possível alocar Licença de Uso." ;
//                       "Falha de comunicação com o Servidor de Licenças";
         //destrói Licenca
         delete licenca;
         return(false);
         }
      //define diagnóstico
      diagnostico = licenca->Diagnostico();
      //verifica se a Licenca é válida
      if (licenca->Status == sttALOCADA)
         {//insere Licenca em lisLICENCA
         lisLICENCA->Add(licenca);
         //grava arquivo local de licenças
         GravaLicencas();
         sucesso = true;
         }
      else
         {//destrói Licenca
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
   //variáveis locais
   bool       sucesso;
   VTServidor *servidor;

   //inicia diagnostico default
   diagnostico = "Não foi possível alocar Licença de Uso.";
   try{//cria Servidor de licença local
      if ((servidor = DLL_NewObjServidorLocal()) != NULL)
         {//aloca Licenca
         sucesso = AlocaLicenca(servidor, login, password, demo, diagnostico);
         //destrói Servidor
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
   //variáveis locais
   bool       sucesso;
   VTServidor *servidor;

   //inicia diagnostico default
   diagnostico = "Não foi possível alocar Licença de Uso.";
   try{//cria Servidor de licença remoto
      if ((servidor = DLL_NewObjServidorRemoto()) != NULL)
         {//aloca Licenca
         sucesso = AlocaLicenca(servidor, login, password, demo, diagnostico);
         //destrói Servidor
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
   //variáveis locais
   int     len;
//   struct  {char bin[512], hex[1024];} buf;
   struct  {char bin[1024], hex[2048];} buf;

   //copia texto a ser criptografado em buf.orig sem o EOF
   len = txt.Length();
   if (len > sizeof(buf.bin)) return(false);
   memcpy(buf.bin, txt.c_str(), len);
   //criptografa buffer
   if (! cript->Executa(buf.bin, len)) return(false);
   //converte buffer criptografado de bin p/ hexa, já com EOF
   if (! bcd->BinToHexa(buf.bin, len, buf.hex, sizeof(buf.hex))) return(false);
   //atualiza txt
   txt = buf.hex;
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::Descriptografa(AnsiString &txt)
   {
   //variáveis locais
   int     len;
//   struct  {char bin[512], hex[1024];} buf;
   struct  {char bin[1024], hex[2048];} buf;

   //copia texto (que esté em hexa) em buf.dest sem o EOF
   len = txt.Length();
   if (len > sizeof(buf.hex)) return(false);
   memcpy(buf.hex, txt.c_str(), len);
   //converte texto em buf.dest de hexa p/ bin, já com EOF
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
   //variáveis locais
   VTLicenca *licenca;
   VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

   //verifica se já foi lido o arquivo de licenças
   if (lisLICENCA->Count == 0)
      {//lê arqruivo de licenças
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
   //variáveis locais
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
   //variáveis locais
   AnsiString filename, txt;
   TStrings   *lines;
   VTLicenca  *licenca;
   VTPath     *path    = (VTPath*)apl->GetObject(__classid(VTPath));

   //define pathname do arquivo de licencas
   filename = path->DirBin + "\\" + ARQ_LICENCA;
   //cria objeto TStringList
   if ((lines = new TStringList()) == NULL) return;
   //loop para todas licenças
   for (int n = 0; n < lisLICENCA->Count; n++)
      {
      licenca = (VTLicenca*)lisLICENCA->Items[n];
      //monta string criptografado c/ dados da Licenca
      licenca->WriteToPacote(pacote);
      txt = pacote->CamposAsStr;
      if (! Criptografa(txt)) continue;
      //salva texto da licença no Strings
      lines->Add(txt);
      }
   //grava arquivo com licenças criptografadas
   lines->SaveToFile(filename);
   //destrói objeto
   delete lines;
   }

//-----------------------------------------------------------------------------
void __fastcall TLicencas::GravaLicencasRegWin(void)
   {
   //variáveis locais
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
   //destrói objeto
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
   //lê Licencas do registro do Windows
   LeLicencasRegWin();
   //lê Licencas do arquivo texto
   LeLicencasArquivo();
   }

//-----------------------------------------------------------------------------
void __fastcall TLicencas::LeLicencasArquivo(void)
   {
   //variáveis locais
   AnsiString filename, txt;
   TStrings   *lines;
   VTLicenca  *licenca;
   VTPath     *path    = (VTPath*)apl->GetObject(__classid(VTPath));

   //verifica se existe o arquivo de licencas
   filename = path->DirBin + "\\" + ARQ_LICENCA;
   if (! FileExists(filename)) return;
   //cria objeto TStringList
   if ((lines = new TStringList()) == NULL) return;
   //lê arquivo com licencças criptografadas
   lines->LoadFromFile(filename);
   //loop para todas licenças
   for (int n = 0; n < lines->Count; n++)
      {//descriptograva txt
      if (! Descriptografa(txt = lines->Strings[n])) continue;
      //monta Pacote c/ campos da linha do arquivo
      pacote->CamposAsStr = txt;
      //cria um novo objeto Licenca
      if ((licenca = NewObjLicenca()) == NULL) continue;
      //lê dados da Licenca do Pacote
      licenca->ReadFromPacote(pacote);
      //insere a licenca em lisLICENCA somente se não for repetida
      SalvaLicenca(licenca);
      }
   //destrói objeto
   delete lines;
   }

//-----------------------------------------------------------------------------
void __fastcall TLicencas::LeLicencasRegWin(void)
   {
   //variáveis locais
   AnsiString txt;
   VTLicenca  *licenca;
   VTRegistro *registro;
   VTProduto  *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

   //cria objeto Registro
   if ((registro = DLL_NewObjRegistro(produto->Sinapsis)) == NULL) return;
   //lê string criptografado c/ dados das Licencas do registro do Windows
   for (int n = 0; registro->ExisteInfo("licencas", "licenca_" + IntToStr(n+1), txt); n++)
      {//descriptograva txt
      if (! Descriptografa(txt)) continue;
      //monta Pacote c/ campos da linha do arquivo
      pacote->CamposAsStr = txt;
      //cria um novo objeto Licenca e insere em lisLICENCA
      if ((licenca = NewObjLicenca()) == NULL) continue;
      //lê dados da Licenca do Pacote
      licenca->ReadFromPacote(pacote);
      //insere a licenca em lisLICENCA somente se não for repetida
      SalvaLicenca(licenca);
      }
   //destrói objeto
   delete registro;
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::LiberaLicenca(AnsiString &diagnostico)
   {  /*
   //variáveis locais
   bool       sucesso;
   VTServidor *servidor;
   VTLicenca  *licenca;
   VTProduto  *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

   //verifica se existe uma Licenca p/ o produto
   if ((licenca = ExisteLicenca()) == NULL)
      {//prepara texto de diagnóstico
      diagnostico = "Não existe Licença de Uso para o programa " + produto->Codigo;
      return(false);
      }
   //verifica validade da Licenca (mudando seu Status, se necessário)
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
   //solicita liberação da Licenca ao Servidor
   if (! servidor->LiberaLicenca(licenca))
      {//erro na comunicação c/ o Servidor
      diagnostico = "Não foi possível liberar Licença de Uso.";
//                    "Falha de comunicação com o Servidor de Licenças";
      sucesso = false;
      }
   else
      {//elimina Licenca da lista de licencas e destrói objeto
      lisLICENCA->Remove(licenca);
      delete licenca;
      //define diagnóstico
      diagnostico = "Licença de Uso liberada.\n"
                    "Para utilizar o programa " + produto->Codigo + " é necessário alocar uma Licença de Uso.";
      //grava arquivo de licenças
      GravaLicencas();
      sucesso = true;
      }
   //destrói objeto Servidor
   delete servidor;
   return(sucesso);
   */
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::LicencaTipoDemo(void)
   {
   //variáveis locais
   VTLicenca *licenca;

   //obtém Licenca
   if ((licenca = ExisteLicenca()) == NULL) return(true);
   //verifica se a Licenca é do tipo Demo
   return(licenca->Tipo == tipoDEMO);
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::LicencaTipoDidatica(void)
   {
   //variáveis locais
   VTLicenca *licenca;

   //obtém Licenca
   if ((licenca = ExisteLicenca()) == NULL) return(true);
   //verifica se a Licenca é do tipo Demo
   return(licenca->Tipo == tipoDIDATICA);
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::PM_GetLoginLocal(void)
   {
   //variáveis locais
   VTLicenca *licenca;

   //obtém Licenca
   if ((licenca = ExisteLicenca()) == NULL) return(false);
   //verifica se a Licenca está configurada c/ Login local
   return(licenca->LoginLocal);
   }

//-----------------------------------------------------------------------------
bool __fastcall TLicencas::PM_GetValidadeLimitada(void)
	{
	//variáveis locais
	VTLicenca *licenca;

	//obtém Licenca
	if ((licenca = ExisteLicenca()) == NULL) return(true);
	//verifica se a Licenca está configurada c/ validade limitada
	return(licenca->Validade >= 0);
	}

//-----------------------------------------------------------------------------
void __fastcall TLicencas::PM_SetLoginLocal(bool enabled)
   {
   //variáveis locais
   VTLicenca *licenca;

   //obtém Licenca
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
   //verifica se já existe uma Licenca de mesmo ID em lisLICENCA
   if (ExisteLicenca(licenca->Produto) != NULL)
      {//destrói Licenca repetida
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
   //variáveis locais
   VTLicenca *licenca;
   VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

   //verifica se existe uma Licenca p/ o produto
   if ((licenca = ExisteLicenca()) == NULL)
	  {//prepara texto de diagnóstico
	  diagnostico = "Não existe Licença de Uso para o programa " + produto->Codigo;
	  return(false);
	  }
   //verifica validade da Licenca (mudando seu Status, se necessário)
   licenca->VerificaPeriodoValidade();
   licenca->VerificaCPU_ID(micro->CPU_ID);
   //define diagnóstico
   diagnostico = licenca->Diagnostico();
   //grava arquivo de licenças
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
//   //variáveis locais
//   VTLicenca *licenca;
//   VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));
//
//   //verifica se existe uma Licenca p/ o produto
//   if ((licenca = ExisteLicenca()) == NULL)
//	  {//prepara texto de diagnóstico
//	  diagnostico = "Não existe Licença de Uso para o programa " + produto->Codigo;
//	  return(false);
//	  }
//   //verifica se a licença possui operações
//   if(licenca->Operacoes.Length == 0)
//   {
//	  diagnostico = "A licença de uso alocada não possui operações e será liberada para realocação";
//	  return(false);
//   }
   return(true);
   }

//-----------------------------------------------------------------------------
//eof



