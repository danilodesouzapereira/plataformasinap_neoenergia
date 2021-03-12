// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes

#include <Fontes\Apl\VTApl.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>

#include "TWebServiceConnection.h"
#include "Entities\TRetornoAlocacaoLicenca.h"
#include "Entities\TRetornoLiberacaoLicenca.h"
#include "Entities\TLicencaRequest.h"
#include "Exceptions\ELicencaConnection.h"
#include "Utils\TMessageBroker.h"

#include <DLL_Inc\Funcao.h>
#include <REST.Exception.hpp>

// --------------------------------------------------------------------------------------- namespace
using namespace Rest::Exception;
//using namespace sinapsis::sinap::gestaoLicenca;//::TRetornoLiberacaoLicenca;

// ------------------------------------------------------------------------------------- constructor
__fastcall TWebServiceConnection::TWebServiceConnection(VTApl *apl)
{
	// cria objetos
	message = new TMessageBroker();
	RESTRequest = new TRESTRequest(NULL);
	RESTClient = new TRESTClient(NULL);
	RESTResponse = new TRESTResponse(NULL);
	this->apl = apl;
	// configura objetos REST
	ConfiguraREST();
}

// ------------------------------------------------------------------------------------------------
__fastcall TWebServiceConnection::~TWebServiceConnection(void)
{
	// destroi objetos
	DestroiTObject(message);
	DestroiTObject(RESTRequest);
	DestroiTObject(RESTClient);
	DestroiTObject(RESTResponse);
}

// ----------------------------------------------------------------------------------------- methods
TRetornoAlocacaoLicenca* TWebServiceConnection::Alocar(TLicencaRequest* request) throw (ELicencaConnection)
{
	// variaveis locais
	TRetornoAlocacaoLicenca *retornoAloca;

	//inicia diagnostico
	diagnostico = "";
	// insere parametros em JSON para REST
	RESTRequest->Params->ParameterByIndex(0)->Value = request->ToJSON();
	// define endereço do webService
//	RESTClient->BaseURL = "http://192.168.1.98:8080/Geli/alocarlicencawebservice/alocarFake";
//	RESTClient->BaseURL = "http://192.168.1.21:8080/gelitreina/alocarlicencawebservice/alocar";
//	RESTClient->BaseURL = "http://licenca.sinapsisenergia.com/gelitreina/alocarlicencawebservice/alocar";
	//oficial
	RESTClient->BaseURL = "http://licenca.sinapsisenergia.com/geli/alocarlicencawebservice/alocar";
	//temporario
//	RESTClient->BaseURL = "http://200.98.139.179/geli/alocarlicencawebservice/alocar";

	try
	{
		RESTRequest->Execute();
	}
	catch (ERESTException &e)
	{
		diagnostico = "Erro na conexão com servidor";
		throw ELicencaConnection(diagnostico);
//		return NULL;
	}
	//verifica status code
	if(RESTResponse->StatusCode == 404)
	{
		diagnostico = "Servidor de licença offline";
		throw ELicencaConnection(diagnostico);
//		return NULL;
    }
	// recebe resposta
	retornoAloca = message->ToRetornoAlocacaoLicenca(RESTResponse->JSONValue);

	return retornoAloca;
}

// ------------------------------------------------------------------------------------------------
void TWebServiceConnection::ConfiguraREST(void)
{
	//RESTRequest
	RESTRequest->Client = RESTClient;
	RESTRequest->Response = RESTResponse;
	RESTRequest->Accept = "application/json, text/plain; q=0.9, text/html;q=0.8,";
	RESTRequest->AcceptCharset = "UTF-8, *;q=0.8";
	RESTRequest->AutoCreateParams = true;
	RESTRequest->HandleRedirects = true;
	RESTRequest->Method = rmPOST;
	RESTRequest->SynchronizedEvents = false;
	RESTRequest->Tag = 0;
	RESTRequest->Timeout = 30000;
	TRESTRequestParameter *item = RESTRequest->Params->AddItem();
	item->ContentType = ctAPPLICATION_JSON;
	item->name = "usuarioDominio";
	item->Value = "";
	item->Kind = pkGETorPOST;

	//RESTClient
	RESTClient->Accept = "application/json, text/plain; q=0.9, text/html;q=0.8,";
	RESTClient->AcceptCharset = "UTF-8, *;q=0.8";
	RESTClient->AcceptEncoding = "identity";
	RESTClient->AllowCookies = true;
	RESTClient->AutoCreateParams = true;
	RESTClient->BaseURL = ""; // a ser definido no Aloca() ou Libera()
	RESTClient->ContentType = "application/json";
	RESTClient->FallbackCharsetEncoding = "UTF-8";
	RESTClient->HandleRedirects = true;
	RESTClient->RaiseExceptionOn500 = true;
	RESTClient->SynchronizedEvents = true;

	//RESTResponse
	RESTResponse->ContentType = "text/html";

}
// ------------------------------------------------------------------------------------------------
void TWebServiceConnection::ConfiguraProxy(AnsiString server, int port,AnsiString user,AnsiString password)
{
    // variáveis locais
//	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
//	AnsiString nome_arq;
//	int valor;
//	AnsiString texto;

	try
	{
//		// define nome do arquivo
//		nome_arq = FILE_CFG_PROXY;
//		//cria um novo arquivo caso não exista
//		CriaProxyCFGVazio();
//		if (geral)
//		{ // lê parâmetros do Form
//			if (!geral->Infoset->LoadFromFile(nome_arq))
//				return;
//			geral->Infoset->GetInfo("ProxyAddress", texto);
//			RESTClient->ProxyServer = texto;
//			geral->Infoset->GetInfo("ProxyPort", valor);
//			RESTClient->ProxyPort = valor;
//		}
		if ((!server.IsEmpty()) && (port >= 0))
		{
			RESTClient->ProxyServer = server;
			RESTClient->ProxyPort = port;
			RESTClient->ProxyUsername = user;
            RESTClient->ProxyPassword = password;
		}
	}
	catch (...)
	{
		Aviso("Erro ao ler as configurações de proxy.");
	}
}
// ------------------------------------------------------------------------------------------------
void TWebServiceConnection::CriaProxyCFGVazio(void)
{
	//variáveis locais
   AnsiString diretorio, valor, arquivo;
   VTPath     *path = (VTPath*)apl->GetObject(__classid(VTPath));
   TStringList *lines;


   lines = new TStringList;
   //verifica se existe o diretório do arquivo
   diretorio = path->DirUsuario();
   arquivo = diretorio + "\\" + FILE_CFG_PROXY;
   if (! DirectoryExists(diretorio))
      {//cria o diretorio
	  CreateDir(diretorio);
	  }
   if (!FileExists(arquivo))
   {	//monta lines
	   lines->Clear();
	   lines->Add("ProxyAddress=;ProxyPort=;");
	   //salva lines no arquivo
	   lines->SaveToFile(arquivo);
   }

   delete lines;
}
// ------------------------------------------------------------------------------------------------
AnsiString TWebServiceConnection::Diagnostico(void)
{
	return diagnostico;
}

// ------------------------------------------------------------------------------------------------
TRetornoLiberacaoLicenca* TWebServiceConnection::Liberar(TLicencaRequest* request) throw (ELicencaConnection)
{
	// variaveis locais
	TRetornoLiberacaoLicenca *retornoLibera;

	//inicia diagnostico
	diagnostico = "";
	// insere parametros em JSON para REST
	RESTRequest->Params->ParameterByIndex(0)->Value = request->ToJSON();
	// define endereço do webService
//	RESTClient->BaseURL = "http://192.168.1.98:8080/Geli/alocarlicencawebservice/liberar";
//	RESTClient->BaseURL = "http://192.168.1.21:8080/gelitreina/alocarlicencawebservice/liberar";
//	RESTClient->BaseURL = "http://licenca.sinapsisenergia.com/gelitreina/alocarlicencawebservice/liberar";
	//oficial
	RESTClient->BaseURL = "http://licenca.sinapsisenergia.com/geli/alocarlicencawebservice/liberar";
	//temporario
//	RESTClient->BaseURL = "http://200.98.139.179/geli/alocarlicencawebservice/liberar";

	try
	{
		RESTRequest->Execute();
	}
	catch (ERESTException &e)
	{
		diagnostico = "Erro durante conexão com servidor";
		throw ELicencaConnection(diagnostico);
//		return NULL;
	}
	//verifica status code
	if(RESTResponse->StatusCode == 404)
	{
		diagnostico = "Servidor de licença offline";
		throw ELicencaConnection(diagnostico);
//		return NULL;
	}
	// recebe resposta
	retornoLibera = message->ToRetornoLiberacaoLicenca(RESTResponse->JSONValue);

	return retornoLibera;
}

// --------------------------------------------------------------------------------------------- eof
