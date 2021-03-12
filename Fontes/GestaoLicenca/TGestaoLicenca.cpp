// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TGestaoLicenca.h"
#include "BO\TLicencaBO.h"
// #include "IO\TUserPack.h"
// #include "IO\TUserPlugin.h"
#include "WebServices\Entities\TLicencaAlocada.h"
#include "WebServices\Entities\TLicencaRequest.h"
#include "WebServices\Entities\TRetornoAlocacaoLicenca.h"
#include "Forms\TFormAlocaLicenca.h"
#include "..\Apl\VTApl.h"
#include "..\Cript\VTBcd.h"
#include "..\Cript\VTCript.h"
#include "..\Diretorio\DirArq.h"
#include "..\Diretorio\VTPath.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Licenca\VTPacote.h"
#include "..\Micro\VTMicro.h"
#include "..\Produto\VTProduto.h"
#include "..\Progresso\VTProgresso.h"
#include "..\RBAC\VTRBACManager.h"
#include "..\Registro\VTRegistro.h"
#include "..\Servidor\VTServidor.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Registro.h"
#include "..\..\DLL_Inc\Licenca.h"
#include "..\..\DLL_Inc\Micro.h"
#include "..\..\DLL_Inc\Servidor.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// -----------------------------------------------------------------------------
VTGestaoLicenca* __fastcall NewObjGestaoLicenca(VTApl *apl)
{
    return (new TGestaoLicenca(apl));
}

// ---------------------------------------------------------------------------
__fastcall TGestaoLicenca::TGestaoLicenca(VTApl *apl)
{
    // salva ponteiro p/ objeto
    this->apl = apl;

   // Inicia vari�veis
   empresa = "";

    try
    { // cria objetos
        micro = DLL_NewObjMicro();
        request = new TLicencaRequest();
    }
    catch (System::Sysutils::Exception &e)
    {
        // nada a fazer
    }
}

// ---------------------------------------------------------------------------
__fastcall TGestaoLicenca::~TGestaoLicenca(void)
{
    // destr�i objetos
    DestroiTObject(micro);
    DestroiTObject(request);
}

/*
 // -----------------------------------------------------------------------------
 bool __fastcall TGestaoLicenca::AlocaLicenca(void)
 {
 // vari�veis locais
 bool sucesso = false;
 TForm *formAloca;

 try
 { // cria form e exibe como janela modal
 formAloca = new TFormAlocaLicenca(this, apl);
 formAloca->ShowModal();
 // destroi form
 delete formAloca;
 sucesso = true;
 }
 catch (System::Sysutils::Exception &e)
 {
 return (false);
 }
 return (sucesso);
 }
 */
// -----------------------------------------------------------------------------
void __fastcall TGestaoLicenca::AlocaLicenca(TLicencaAlocada *retorno, UnicodeString &diagnostico)
{
    // variaveis locais
    VTLicenca *licenca;
    VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

//    // prote��o
//    if (request == NULL)
//    {
//        diagnostico = "Dados de entrada inv�lidos";
//        return;
//    }
//    if (retorno == NULL)
//    {
//        diagnostico = "Dados de retorno inv�lidos";
//        return;
//    }
    // cria licenca
	 licenca = CriaLicenca(retorno, diagnostico);
    if (licenca == NULL)
        return;
    // atualiza barra de progresso
    progresso->Add("Armazenando licen�a");
    // grava licenca onde tem que gravar e segue processo
	 if (!ArmazenaLicenca(licenca, diagnostico))
        return;
    // atualiza barra de progresso
    progresso->Add("Configurando usu�rio");
    // libera as DLLs SinapToolKit e ImportaRedeGis, que serao alteradas nos packs
    // if (!LiberaDLL(diagnostico))
    // return;
    // altera o registro do Windows com os endere�os ARIES/VIP
	 if (!AlteraIPE(licenca->Empresa, diagnostico))
        return;
    // altera o registro do Windows com a empresaId
    if (!AlteraEmpresaRegistro(licenca->Empresa, diagnostico, true))
        return;
    // altera userPack em fun��o da empresa da licen�a  => feito pelo SINAPpack.exe
    // if (!DescompactaUsuario(diagnostico))
    // return;

}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
bool __fastcall TGestaoLicenca::AlocaLicencaSinapViewer(UnicodeString &diagnostico)
{
    // variaveis locais
    VTLicenca *licenca;
    VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
    TLicencaAlocada *retorno;

    //cria request SINAPviewer
    this->request = MontaRequest("viewer", "sinapviewer", SINAPSIS);
    // cria retorno SINAPviewer
    retorno = CriaRetornoSinapViewer(diagnostico);
    if (retorno == NULL)
        return false;
    // cria licenca
    licenca = CriaLicenca(retorno, diagnostico);
    if (licenca == NULL)
    {
        delete retorno;
        return false;
    }
    //licen�a local
    licenca->CtrlRemoto = false;
    // atualiza barra de progresso
    progresso->Add("Armazenando licen�a");
    // grava licenca onde tem que gravar e segue processo
    if (!ArmazenaLicenca(licenca, diagnostico))
    {
        delete retorno;
        return false;
    }
    // atualiza barra de progresso
    progresso->Add("Configurando usu�rio");
    // libera as DLLs SinapToolKit e ImportaRedeGis, que serao alteradas nos packs
    // if (!LiberaDLL(diagnostico))
    // return;
    // altera o registro do Windows com os endere�os ARIES/VIP
    if (!AlteraIPE(licenca->Empresa, diagnostico))
    {
        delete retorno;
        return false;
    }
    // altera o registro do Windows com a empresaId
    if (!AlteraEmpresaRegistro(licenca->Empresa, diagnostico, true))
    {
        delete retorno;
        return false;
    }
    // destroi retorno
    delete retorno;
    return true;
}

// -----------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/15920983
bool __fastcall TGestaoLicenca::AlteraEmpresaRegistro(int empresa_id, UnicodeString &diagnostico,
    bool aloca)
{
    // vari�veis locais
    VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));
    VTRegistro *registro;
//  AnsiString   empresa;

    try
    { // converte para string
        empresa = IntToStr(empresa_id);
        // cria objeto Registro
        registro = DLL_NewObjRegistro(produto->Sinapsis);
        // verifica se esta alocando ou liberando licenca
        if (aloca)
        { // grava dominio da empresa
            registro->GravaInfo(produto->CodRev, "empresa_new_id", empresa);
        }
        else
        { // grava dominio da empresa
            registro->GravaInfo(produto->CodRev, "empresa_old_id", empresa);
        }
    }
    catch (System::Sysutils::Exception &e)
    { // destroi objeto
        diagnostico = "Erro ao acessar registro do Windows";
        DestroiTObject(registro);
        return false;
    }
    // destroi objeto
    DestroiTObject(registro);
    return true;
}

// -----------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/15920983
// https://sinapsisenergia.teamwork.com/#/tasks/17338082
// https://sinapsisenergia.teamwork.com/#/tasks/18194486
bool __fastcall TGestaoLicenca::AlteraIPE(int empresa_id, UnicodeString &diagnostico)
{
    // determina conjunto de empresas para inserir os WS no registro
    empresa = IntToStr(empresa_id);
    switch (empresa_id)
    { // cooperativas (todas)
    case FECOERGS:
    case CERILUZ:
    case COOPERLUZ:
    case CRERAL:
    case CEJAMA:
    case CERGAPA:
    case CERBRANORTE:
    case CERGRAL:
    case COOPERMILA:
    case CERPALO:
    case COOPERALIANCA:
    case CEPRAG:
    case CERSUL:
    case CERMOFUL:
    case COOPERCOCAL:
    case CEREJ:
    case COORSEL:
    case CERAL_ANITAPOLIS:
    case CERACA:
    case FECOERUSC:
    case CEGERO:
    case CERSAD:
    case CEESAM:
    case COOPERZEM:
    case CERTHIL:
    case CERTREL:
    case CERMISSOES:
    case CERTAJA:
    case CERTEL:
    case COPREL:
    case CRELUZ:
    case CERGAL:
// nova empresa
    case CERFOX:
//      InsereEnderecoVIP(diagnostico);
        InsereEnderecoARIES(diagnostico);
        return true;

        // eletrobras e copel
    case ELETROBRAS_ACRE:
    case ELETROBRAS_AMAZONAS:
    case ELETROBRAS_RONDONIA:
    case ELETROBRAS_RORAIMA:
    case ELETROBRAS_PIAUI:
    case CEPEL:
    case ELETROBRAS_HOLDING:
        // case CEAL:
    case ELETROBRAS_ALAGOAS:
    case COPEL:
        InsereEnderecoARIES(diagnostico);
        return true;

    default:
        InsereEnderecoVazio(diagnostico);
        return true;
    }
    return true;
}

// -----------------------------------------------------------------------------
bool __fastcall TGestaoLicenca::ArmazenaLicenca(VTLicenca *licenca, UnicodeString &diagnostico)
{
    // vari�veis locais
    bool sucesso = false;
    VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

    try
    { // define diagn�stico
        diagnostico = licenca->Diagnostico();
        // verifica se a Licenca � v�lida
        if (licenca->Status == sttALOCADA)
        { // insere Licenca em lisLICENCA
            sucesso = licencas->InsereLicenca(licenca);
            // grava arquivo local de licen�as
            licencas->GravaLicencas();
        }
        else
        { // destr�i Licenca
            delete licenca;
            sucesso = false;
        }
    }
    catch (System::Sysutils::Exception &e)
    {
        diagnostico = "N�o foi poss�vel alocar Licen�a de Uso: " + e.Message;
        return (false);
    }
    return (sucesso);
}

// -----------------------------------------------------------------------------
AnsiString __fastcall TGestaoLicenca::CPUId(void)
{
    return (micro->CPU_ID);
}

// -----------------------------------------------------------------------------
VTLicenca* __fastcall TGestaoLicenca::CriaLicenca(TLicencaAlocada *retorno,
    UnicodeString &diagnostico)
{
    // vari�veis locais
    VTLicenca *licenca;
    VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

    try
    { // cria uma nova Licenca
		  licenca = DLL_NewObjLicenca();
        // dados comuns
        licenca->CPU_ID = micro->CPU_ID;
        licenca->Produto = produto->CodLicenca;
        licenca->CtrlRemoto = true;
        licenca->DH_controle = DateOf(Now());
        licenca->DH_inicio = DateOf(Now());
        licenca->Status = sttALOCADA;
        // dados do request
        licenca->Login = request->Login;
        licenca->Password = request->Senha;
        licenca->Empresa = request->Dominio;
        // dados do retorno
        licenca->DH_alocada = retorno->DataAlocada;
        licenca->DH_termino = retorno->DataTermino;
        licenca->Tipo = retorno->Tipo;
        licenca->Validade = retorno->Validade;
        licenca->Operacoes = retorno->Operacoes;
    }
    catch (System::Sysutils::Exception &e)
    {
        diagnostico = "Erro ao criar licen�a de uso: " + e.Message;
        DestroiTObject(licenca);
        return (NULL);
    }
    return (licenca);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
TLicencaAlocada* __fastcall TGestaoLicenca::CriaRetornoSinapViewer(UnicodeString &diagnostico)
{
    // vari�veis locais
    TLicencaAlocada *retorno;
    DynamicArray<int> operacoes;

    try
    { // cria retorno SINAPviewer
        retorno = new TLicencaAlocada();
        retorno->DataAlocada = Today();
        retorno->DataTermino = Yesterday();
        retorno->Tipo = tipoUSER | tipoGOOGLE;
        retorno->Validade = -1;
        //opera��es
        operacoes.Length = 1;
        operacoes[0] = opVIEW;
        retorno->Operacoes = operacoes;
//        retorno->Operacoes.Length = 1;
//        retorno->Operacoes[ 0] = opVIEW;
        return retorno;
    }
    catch (...)
    {
        diagnostico = "Erro ao criar licen�a do tipo SINAPviewer";
        return NULL;
    }
}

/*
 // -----------------------------------------------------------------------------
 bool __fastcall TGestaoLicenca::Criptografa(AnsiString &txt)
 {
 // vari�veis locais
 int len;
 struct
 {
 char bin[512], hex[1024];
 } buf;

 // copia texto a ser criptografado em buf.orig sem o EOF
 len = txt.Length();
 if (len > sizeof(buf.bin))
 return (false);
 memcpy(buf.bin, txt.c_str(), len);
 // criprografa buffer
 if (!cript->Executa(buf.bin, len))
 return (false);
 // converte buffer criptografado de bin p/ hexa, j� com EOF
 if (!bcd->BinToHexa(buf.bin, len, buf.hex, sizeof(buf.hex)))
 return (false);
 // atualiza txt
 txt = buf.hex;
 return (true);
 }

 // -----------------------------------------------------------------------------
 bool __fastcall TGestaoLicenca::Descriptografa(AnsiString &txt)
 {
 // vari�veis locais
 int len;
 struct
 {
 char bin[512], hex[1024];
 } buf;

 // copia texto (que est� em hexa) em buf.dest sem o EOF
 len = txt.Length();
 if (len > sizeof(buf.hex))
 return (false);
 memcpy(buf.hex, txt.c_str(), len);
 // converte texto em buf.dest de hexa p/ bin, j� com EOF
 if (!bcd->HexaToBin(buf.hex, len, buf.bin, sizeof(buf.bin)))
 return (false);
 // descriprografa buffer
 if (!cript->Executa(buf.bin, len / 2))
 return (false);
 // atualiza txt
 txt = buf.bin;
 return (true);
 }
 */
/*
 // -----------------------------------------------------------------------------
 bool __fastcall TGestaoLicenca::DescompactaUsuario(UnicodeString &diagnostico)
 {
 // vari�veis locais
 TUserPack *pack;
 TUserPlugin *plugin;
 bool sucesso = false;

 try
 { // cria objeto userPack
 pack = new TUserPack(apl);
 sucesso = pack->Executa(diagnostico);
 // destroi objeto
 delete pack;
 }
 catch (System::Sysutils::Exception &e)
 {
 diagnostico = "Erro ao descompactar arquivos de usu�rio: " + e.Message;
 DestroiTObject(pack);
 return false;
 }
 if (!sucesso)
 return sucesso;
 try
 { // cria objeto userPlugin
 plugin = new TUserPlugin(apl);
 sucesso = plugin->Executa(diagnostico);
 // destroi objeto
 delete plugin;
 }
 catch (System::Sysutils::Exception &e)
 {
 diagnostico = "Erro ao descompactar plugins de usu�rio: " + e.Message;
 DestroiTObject(plugin);
 return false;
 }
 return sucesso;
 }
 */

// -----------------------------------------------------------------------------
int __fastcall TGestaoLicenca::EmpresaId(AnsiString empresa_nome)
{
    // vari�veis locais
    VTEmpresa *empresa = NewObjEmpresa();
    int id;

    // determina id da empresa
    id = empresa->EmpresaAsInt(empresa_nome.UpperCase());
    delete empresa;

    return (id);
}

// -----------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/15920983
// https://sinapsisenergia.teamwork.com/#/tasks/17302005
void __fastcall TGestaoLicenca::InsereEnderecoARIES(UnicodeString &diagnostico)
{
    // vari�veis locais
    VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));
    VTRegistro *registro;
//  AnsiString  dominio     = "100";
    AnsiString  dominio;
    AnsiString  wsdl_estudo =
        "http://aries.sinapsisenergia.com/aries/ReceberEstudoWebService";
    AnsiString wsdl_padrao =
        "http://aries.sinapsisenergia.com/aries/ListarPadraoConstrutivoWebService";
	AnsiString wsdl_grupo =
		"http://aries.sinapsisenergia.com/aries/ListarGrupoWebService";

    try
    { // cria objeto Registro
        registro = DLL_NewObjRegistro(produto->Sinapsis);
        // grava dominio da empresa
        dominio = "-" + empresa;
        registro->GravaInfo("IPE", "dominio", dominio);
        // grava wsdl do WebSite de estudos
        registro->GravaInfo("IPE", "ws_estudo", wsdl_estudo);
        // grava wsdl do WebSite de padr�es
        registro->GravaInfo("IPE", "ws_padrao", wsdl_padrao);
		// grava wsdl do WebSite de grupos
		registro->GravaInfo("IPE", "ws_grupo", wsdl_grupo);
    }
    catch (System::Sysutils::Exception &e)
    { // destroi objeto
        DestroiTObject(registro);
        diagnostico = "Erro ao acessar registro do Windows";
        return;
    }
    // destroi objeto
    DestroiTObject(registro);
}

// -----------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17302005
void __fastcall TGestaoLicenca::InsereEnderecoVazio(UnicodeString &diagnostico)
{
    // vari�veis locais
    VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));
    VTRegistro *registro;
    AnsiString dominio = "";
    AnsiString wsdl_estudo = "";
    AnsiString wsdl_padrao = "";
	AnsiString wsdl_grupo  = "";

    try
    { // cria objeto Registro
        registro = DLL_NewObjRegistro(produto->Sinapsis);
        // grava dominio da empresa
        registro->GravaInfo("IPE", "dominio", dominio);
        // grava wsdl do WebSite de estudos
        registro->GravaInfo("IPE", "ws_estudo", wsdl_estudo);
        // grava wsdl do WebSite de padr�es
        registro->GravaInfo("IPE", "ws_padrao", wsdl_padrao);
		// grava wsdl do WebSite de grupos
		registro->GravaInfo("IPE", "ws_grupo", wsdl_grupo);
    }
    catch (System::Sysutils::Exception &e)
    { // destroi objeto
        DestroiTObject(registro);
        diagnostico = "Erro ao acessar registro do Windows";
        return;
    }
    // destroi objeto
    DestroiTObject(registro);
}

// -----------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17302005
void __fastcall TGestaoLicenca::InsereEnderecoVIP(UnicodeString &diagnostico)
{
    // vari�veis locais
    VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));
    VTRegistro *registro;
    AnsiString dominio = "100";
	AnsiString wsdl_estudo =
        "http://vip.sinapsisenergia.com/vip/ReceberEstudoWebService";
	AnsiString wsdl_padrao = 
        "http://vip.sinapsisenergia.com/vip/ListarPadraoConstrutivoWebService";
	AnsiString wsdl_grupo =
        "http://vip.sinapsisenergia.com/vip/ListarGrupoWebService";

    try
    { // cria objeto Registro
        registro = DLL_NewObjRegistro(produto->Sinapsis);
        // grava dominio da empresa
        registro->GravaInfo("IPE", "dominio", dominio);
        // grava wsdl do WebSite de estudos
        registro->GravaInfo("IPE", "ws_estudo", wsdl_estudo);
        // grava wsdl do WebSite de padr�es
        registro->GravaInfo("IPE", "ws_padrao", wsdl_padrao);
		// grava wsdl do WebSite de grupos
		registro->GravaInfo("IPE", "ws_grupo", wsdl_grupo);
    }
    catch (System::Sysutils::Exception &e)
    { // destroi objeto
        DestroiTObject(registro);
        diagnostico = "Erro ao acessar registro do Windows";
        return;
    }
    // destroi objeto
    DestroiTObject(registro);
}

/*
 // -----------------------------------------------------------------------------
 bool __fastcall TGestaoLicenca::LiberaDLL(UnicodeString &diagnostico)
 {
 // vari�veis locais
 HINSTANCE hinstLib;
 BOOL fFreeResult;

 // Get a handle to the DLL module.
 hinstLib = LoadLibrary(TEXT("SinapToolKit.dll")); // 1: load the DLL
 // If the handle is valid, unload the DLL
 if (hinstLib != NULL)
 {
 fFreeResult = FreeLibrary(hinstLib); // 2: unload the DLL
 }
 else
 {
 diagnostico = "Erro ao liberar SinapToolKit.dll";
 return false;
 }

 // Get a handle to the DLL module.
 hinstLib = LoadLibrary(TEXT("ImportaRedeGis.dll")); // 1: load the DLL
 // If the handle is valid, unload the DLL
 if (hinstLib != NULL)
 {
 fFreeResult = FreeLibrary(hinstLib); // 2: unload the DLL
 }
 else
 {
 diagnostico = "Erro ao liberar ImportaRedeGis.dll";
 return false;
 }

 return true;
 }
 */
// -----------------------------------------------------------------------------
void __fastcall TGestaoLicenca::LiberaLicencaRemota(UnicodeString &diagnostico)
{
    // vari�veis locais
    VTLicenca *licenca;
    VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
    VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

    // verifica se existe uma Licenca p/ o produto
    licenca = licencas->ExisteLicenca();
    // verifica validade da Licenca (mudando seu Status, se necess�rio)
    licenca->VerificaCPU_ID(micro->CPU_ID);
    // altera o registro do Windows com a empresaId
    if (AlteraEmpresaRegistro(licenca->Empresa, diagnostico, false))
    { // elimina Licenca da lista de licencas e destr�i objeto
        licenca->Status = sttLIVRE;
        licencas->RemoveLicenca(licenca);
        delete licenca;
        // define diagn�stico
        diagnostico = "Licen�a de Uso liberada.\n" "Para utilizar o programa " + produto->Codigo +
            " � necess�rio alocar uma Licen�a de Uso.";
        // grava arquivo de licen�as
        licencas->GravaLicencas();
    }
}

// -----------------------------------------------------------------------------
void __fastcall TGestaoLicenca::LiberaLicencaLocal(UnicodeString &diagnostico)
{
    // vari�veis locais
    VTLicenca *licenca;
    VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
    VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));
    VTServidor *servidor;

    // verifica se existe uma Licenca p/ o produto
    licenca = licencas->ExisteLicenca();
    // cria servidor local
    servidor = DLL_NewObjServidorLocal();
    // solicita libera��o da Licenca ao Servidor
    if (!servidor->LiberaLicenca(licenca))
    { // erro na comunica��o c/ o Servidor
        diagnostico = "N�o foi poss�vel liberar Licen�a de Uso.";
    }
    else
    { // elimina Licenca da lista de licencas e destr�i objeto
        licencas->RemoveLicenca(licenca);
        delete licenca;
        // define diagn�stico
        diagnostico = "Licen�a de Uso liberada.\n" "Para utilizar o programa " + produto->Codigo +
            " � necess�rio alocar uma Licen�a de Uso.";
        // grava arquivo de licen�as
        licencas->GravaLicencas();
    }
    // destr�i objeto Servidor
    delete servidor;
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
bool __fastcall TGestaoLicenca::LiberaLicencaSinapViewer(UnicodeString &diagnostico)
{
    // variaveis locais
    VTLicenca *licenca;
    VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
    VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

    try
    { // verifica se existe uma Licenca p/ o produto
        licenca = licencas->ExisteLicenca();
        // altera o registro do Windows com a empresaId
        if (AlteraEmpresaRegistro(licenca->Empresa, diagnostico, false))
        { // elimina Licenca da lista de licencas e destr�i objeto
            licenca->Status = sttLIVRE;
            licencas->RemoveLicenca(licenca);
            delete licenca;
            // define diagn�stico
            diagnostico = "Licen�a de Uso liberada.\n" "Para utilizar o programa " +
                produto->Codigo + " � necess�rio alocar uma Licen�a de Uso.";
            // grava arquivo de licen�as
            licencas->GravaLicencas();
        }
    }
    catch (...)
    {
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
bool __fastcall TGestaoLicenca::LicencaRemota(void)
{
    // vari�veis locais
    VTLicenca *licenca;
    VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

    // prote��o
    licenca = licencas->ExisteLicenca();
    if (licenca == NULL)
        return false;
	 // retorna se a licen�a foi alocada localmente ou remotamente

    return licenca->CtrlRemoto;
}

// -----------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16131085
bool __fastcall TGestaoLicenca::LicencaSinapViewer(void)
{
    // vari�veis locais
    VTLicenca *licenca;
    VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

    // prote��o
    licenca = licencas->ExisteLicenca();
    if (licenca == NULL)
        return false;
    // retorna se a licen�a possui opera��o opVIEW
    for(int n = 0; n < licenca->Operacoes.Length; n++)
    {
        if(licenca->Operacoes[n] == opVIEW)
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
TLicencaRequest* __fastcall TGestaoLicenca::MontaRequest(void)
{
    // vari�veis locais
    VTLicenca *licenca;
    VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

    // prote��o
    licenca = licencas->ExisteLicenca();
    if (licenca == NULL)
        return NULL;
    // retorna request a partir da licen�a alocada
    return (MontaRequest(licenca->Login, licenca->Password, licenca->Empresa));
}

// -----------------------------------------------------------------------------
TLicencaRequest* __fastcall TGestaoLicenca::MontaRequest(UnicodeString login, UnicodeString senha,
    int dominio)
{
    // preenche request
    request->Dominio = dominio;
    request->Login = login;
    request->Senha = senha;
    request->Produto = Produto();
    request->CPUid = CPUId();
    return (request);
}

// -----------------------------------------------------------------------------
AnsiString __fastcall TGestaoLicenca::Produto(void)
{
    VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));
    return (produto->CodLicenca);
}

// -----------------------------------------------------------------------------
// eof
