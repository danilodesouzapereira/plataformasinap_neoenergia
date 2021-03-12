// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include "TLicencaBO.h"
#include "..\WebServices\Entities\TRetornoAlocacaoLicenca.h"
#include "..\WebServices\Entities\TRetornoLiberacaoLicenca.h"
#include "..\WebServices\Entities\TLicencaAlocada.h"
#include "..\WebServices\Entities\TLicencaRequest.h"
#include "..\WebServices\TWebServiceConnection.h"
#include "..\Exceptions\ELicencaConnection.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\GestaoLicenca\VTGestaoLicenca.h>
#include <Fontes\Progresso\VTProgresso.h>
#include <DLL_Inc\Funcao.h>

// ------------------------------------------------------------------------------------------- class
// using sinapsis::sinap::gestaoLicenca;

// ------------------------------------------------------------------------------------- constructor
VTLicencaBO* __fastcall NewObjLicencaBO(VTApl *apl)
{
	return (new TLicencaBO(apl));
}

// ------------------------------------------------------------------------------------------------
__fastcall TLicencaBO::TLicencaBO(VTApl *apl_owner)
{
	// salva ponteiro
	this->apl = apl_owner;
	// cria objetos
	wsConnection = new TWebServiceConnection(apl);
	gestaoLicenca = NewObjGestaoLicenca(apl);
}

// ------------------------------------------------------------------------------------------------
__fastcall TLicencaBO::~TLicencaBO(void)
{
	// destroi objetos
	DestroiTObject(wsConnection);
	DestroiTObject(gestaoLicenca);
}

// ----------------------------------------------------------------------------------------- methods
bool TLicencaBO::Alocar(TLicencaRequest *request, UnicodeString &diagnostico) throw(ELicencaConnection)
{
	// variaveis locais
	TRetornoAlocacaoLicenca* retorno;
	VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

	// inicia barra de progresso
	progresso->Start(progTEXTO);
//	progresso->Add("Conectando-se ao servidor", true);

	// chama webServiceConnection
	try
	{
//		retorno = wsConnection->Alocar(request);
		retorno = new TRetornoAlocacaoLicenca();
		retorno->Sucesso = true;
		retorno->Mensagem = "";

		TLicencaAlocada* licencaAlocada = new TLicencaAlocada();
		licencaAlocada = new TLicencaAlocada();

		TDateTime dt1 = Now();
		unsigned short ano, mes, dia;
		dt1.DecodeDate(&ano, &mes, &dia);
		TDateTime dt2 = TDateTime((int)ano + 5, (int)mes, (int)dia);
		licencaAlocada->DataAlocada = dt1;
		licencaAlocada->DataTermino = dt2;

		DynamicArray<int> operacoes;
		operacoes.Length = 19;
		operacoes[0]  = 001;
		operacoes[1]  = 100;
		operacoes[2]  = 101;
		operacoes[3]  = 102;
		operacoes[4]  = 201;
		operacoes[5]  = 202;
		operacoes[6]  = 203;
		operacoes[7]  = 204;
		operacoes[8]  = 205;
		operacoes[9]  = 206;
		operacoes[10] = 207;
		operacoes[11] = 208;
		operacoes[12] = 209;
		operacoes[13] = 301;
		operacoes[14] = 302;
		operacoes[15] = 303;
		operacoes[16] = 304;
		operacoes[17] = 305;
		operacoes[18] = 306;
		licencaAlocada->Operacoes = operacoes;
		licencaAlocada->Validade = 100;
		licencaAlocada->Tipo = 1;
		retorno->LicencaAlocada = licencaAlocada;
	}
	catch (ELicencaConnection &e)
	{
        throw;
	 }
	// valida retorno
//	if (!ValidaRetorno(retorno))
//	{
//		progresso->Stop();
//		return false;
//	}
	// atualiza barra de progresso
//	progresso->Add("Licença alocada no servidor");
	progresso->Add("Licença alocada");
	// cria a licenca no sinap, guarda nos arquivos e altera userPack da empresa
	gestaoLicenca->AlocaLicenca(retorno->LicencaAlocada, diagnostico);
	progresso->Add(diagnostico);
	// apresenta diagnostico
//	if (!diagnostico.IsEmpty())
//		Aviso(diagnostico);

	progresso->Stop();
	return true;
}
// ------------------------------------------------------------------------------------------------
void  TLicencaBO::ConfiguraProxy(AnsiString server, int port, AnsiString user, AnsiString password)
{
	if (wsConnection)
	{
    	wsConnection->ConfiguraProxy(server,port,user,password);
	}
}
// ------------------------------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16131085
bool TLicencaBO::Liberar(UnicodeString &diagnostico) throw(ELicencaConnection)
{
	// variaveis locais
	TLicencaRequest* request;
	TRetornoLiberacaoLicenca* retorno;
	VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

	// inicia barra de progresso
	progresso->Start(progTEXTO);
	progresso->Add("Conectando-se ao servidor", true);

    //verifica se é licença SINAPviewer
    if(gestaoLicenca->LicencaSinapViewer())
    {
        gestaoLicenca->LiberaLicencaSinapViewer(diagnostico);
        progresso->Stop();
		return true;
    }
	// proteção: verifica se a licença foi alocada localmente
	if (!gestaoLicenca->LicencaRemota())
	{
		gestaoLicenca->LiberaLicencaLocal(diagnostico);
		// apresenta diagnostico
//		if (!diagnostico.IsEmpty())
//			Aviso(diagnostico);
		progresso->Stop();
		return true;
	}
	// cria request
	request = gestaoLicenca->MontaRequest();
	// chama webServiceConnection
	try
	{
//		retorno = wsConnection->Liberar(request);
		retorno = new TRetornoLiberacaoLicenca();
		retorno->Sucesso = true;
		retorno->Mensagem = "Licença liberada.";
	}
	catch (ELicencaConnection &e)
	{
        progresso->Stop();
		throw;
    }
	// valida retorno
	if (!ValidaRetorno(retorno))
	{ // apresenta diagnostico
//		if (!diagnostico.IsEmpty())
//			Aviso(diagnostico);
		progresso->Stop();
		return false;
	}
	// atualiza barra de progresso
	progresso->Add("Licença liberada no servidor");
	// libera a licenca no sinap e grava nos arquivos
	gestaoLicenca->LiberaLicencaRemota(diagnostico);
	progresso->Add(diagnostico);
	// apresenta diagnostico
//	if (!diagnostico.IsEmpty())
//		Aviso(diagnostico);

	progresso->Stop();
	return true;
}
// ------------------------------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
bool TLicencaBO::LicencaSinapViewer(UnicodeString login, UnicodeString senha, UnicodeString dominio, UnicodeString &diagnostico)
{
	// verifica se os parametros são de licença SINAPviewer
    if(login.CompareIC("viewer") != 0)
        return false;

    if(dominio.CompareIC("sinapsis") != 0)
        return false;

    if(senha.CompareIC("sinapviewer") != 0)
        return false;

    //cria licenca SINAPviewer
    return(gestaoLicenca->AlocaLicencaSinapViewer(diagnostico));
}

// ---------------------------------------------------------------------------
TLicencaRequest* TLicencaBO::MontaRequest(UnicodeString login, UnicodeString senha, UnicodeString dominio)
{
	// converte dominio em inteiro
//	int dominio_id = gestaoLicenca->EmpresaId(dominio);
	int dominio_id = 29;

	return (gestaoLicenca->MontaRequest(login, senha, dominio_id));
}

// ---------------------------------------------------------------------------
bool TLicencaBO::ValidaRetorno(TRetornoAlocacaoLicenca *retorno)
{
	if (retorno == NULL)
	{ // avisa
		Aviso(wsConnection->Diagnostico());
		return false;
	}
	if (!retorno->Sucesso)
	{ // avisa
		Aviso(retorno->Mensagem);
		return false;
	}
	if (retorno->LicencaAlocada == NULL)
	{ // avisa
		Aviso("Dados inválidos do WebService");
		return false;
	}
	return true;
}

// ---------------------------------------------------------------------------
bool TLicencaBO::ValidaRetorno(TRetornoLiberacaoLicenca *retorno)
{
	if (retorno == NULL)
	{ // avisa
		Aviso(wsConnection->Diagnostico());
		return false;
	}
	if (!retorno->Sucesso)
	{ // avisa
		Aviso(retorno->Mensagem);
		return false;
	}
	return true;
}

// --------------------------------------------------------------------------------------------- eof
