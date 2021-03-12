// ---------------------------------------------------------------------------
#pragma hdrstop
#include <map>

#include "TRBACManager.h"
#include "..\Apl\VTApl.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// -----------------------------------------------------------------------------
VTRBACManager* __fastcall NewObjRBACManager(VTApl *apl)
{
	return (new TRBACManager(apl));
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
__fastcall TRBACManager::TRBACManager(VTApl *apl)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
    //cria lista
    lisOP = new TList();
    //cria operações
    CriaOperacoes();
	// inicia map
	MontaMap();
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
__fastcall TRBACManager::~TRBACManager(void)
{
	// destroi lista e seus objetos
    LimpaTList(lisOP);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TRBACManager::CriaOperacoes(void)
{
    TModOp *op;

    //viewer
    op = new TModOp();
    op->operacao_id = opVIEW;
    op->operacao_codigo = "[ACESSO] SINAPviewer";
    op->modulo_id.Length = 16;
    op->modulo_id = {modALOCALICENCA, modEXIBELICENCA, modLIBERALICENCA, modRESTRINGE,
                    modPLANEJAMENTO, modEXPLORER, modFASOMETRO, modGRAFICO,
                    modLOCALIZAR, modTOPOLOGIA, modEDITOR, modCABOARRANJO,
                    modCURVATIPICA, modPATAMARES, modTIPOCHAVE, modTIPOREDE};
    lisOP->Add(op);

    //basico
    op = new TModOp();
    op->operacao_id = opBASICO;
    op->operacao_codigo = "[ACESSO] Básico";
    op->modulo_id.Length = 16;
    op->modulo_id = {modEXPLORER, modFASOMETRO, modGRAFICO, modIMPORTAGIS,
                    modIMPORTAGISLOTE, modLOCALIZAR, modMEDIDOR, modTOPOLOGIA,
                    modZOOM, modANALISEMALHA, modEDITOR, modCABOARRANJO,
                    modCURVATIPICA, modPATAMARES, modTIPOCHAVE, modTIPOREDE};
    lisOP->Add(op);

    //developer
    op = new TModOp();
    op->operacao_id = opDEVELOPER;
    op->operacao_codigo = "[ACESSO] Developer";
    op->modulo_id.Length = 1;
    op->modulo_id = {modDEVELOPER};
    lisOP->Add(op);

    //aloca regulador
    op = new TModOp();
    op->operacao_id = opREGULADOR;
    op->operacao_codigo = "[ACESSO] Alocação de Regulador de Tensão";
    op->modulo_id.Length = 1;
    op->modulo_id = {modALOCART};
    lisOP->Add(op);

    //contingencias
    op = new TModOp();
    op->operacao_id = opCONTINGENCIAS;
    op->operacao_codigo = "[ACESSO] Análise de Contingências";
    op->modulo_id.Length = 23;
//    op->modulo_id = {modANALISECONT};
    op->modulo_id = {modTOPOLOGIA, modFLUXO, modEDITOR, modGRAFICO, modEXPLORER, modMEDIDOR, modLOCALIZAR,
                    modLOCALIZAR, modMANOBRAS, modAJUSTEDEMANDA, modANALISEMALHA, modRELFLUXO, modRELJUSANTE,
                    modRELDEMANDA, modRELEQUIPAMENTO,  modMAPAAI,  modMAPADCLOTE, modMAPACC, modMAPADC,
                    modMAPATENSAO, modMAPAPT, modMAPAFP, modIMPORTAGIS};
    lisOP->Add(op);

    //continuidade
    op = new TModOp();
    op->operacao_id = opCONTINUIDADE;
    op->operacao_codigo = "[ACESSO] Análise de Continuidade";
    op->modulo_id.Length = 4;
    op->modulo_id = {modALOCACHAVE, modALOCATRIPOLAR, modENUMRELIGADOR, modCONTINUIDADE};
    lisOP->Add(op);

    //continuidade
    op = new TModOp();
    op->operacao_id = opCURTO;
    op->operacao_codigo = "[ACESSO] Análise de Curto-Circuito";
    op->modulo_id.Length = 2;
    op->modulo_id = {modCURTO, modMULTFALTA};
    lisOP->Add(op);

    //microgrids
    op = new TModOp();
    op->operacao_id = opMICROGRIDS;
    op->operacao_codigo = "[ACESSO] Análise de Microgrids";
    op->modulo_id.Length = 1;
    op->modulo_id = {modMICROGRID};
    lisOP->Add(op);

    //partida motor
    op = new TModOp();
    op->operacao_id = opMOTOR;
    op->operacao_codigo = "[ACESSO] Análise de Partida de Motor";
    op->modulo_id.Length = 1;
    op->modulo_id = {modPARTIDAMOTOR};
    lisOP->Add(op);

    //rede
    op = new TModOp();
    op->operacao_id = opREDE;
    op->operacao_codigo = "[ACESSO] Análise de Rede";
    op->modulo_id.Length = 27;
    op->modulo_id = {modBALANCEAMENTO, modPERDATECLOTE, modPERDATEC, modPERDATECREL,
                    modABERTURAFASE, modCAMINHOELETRICO, modDIAGEQUIPAMENTO, modDIAGFLUXO,
                    modFLUXOLOTE, modFLUXO, modDIAGTECNICO, modMANOBRAS,
                    modRELFLUXO,  modRELJUSANTE, modRELDEMANDA, modRELEQUIPAMENTO,
                    modSOLICITACAO, modMAPAAI, modMAPADCLOTE, modMAPACC,
                    modMAPADC, modMAPATENSAO, modMAPAPT,  modMAPAFP,
                    modOCORRENCIA,  modOTIMIZARAMAL, modPLANEJAMENTO};
    lisOP->Add(op);

    //regulatoria
    op = new TModOp();
    op->operacao_id = opREGULATORIA;
    op->operacao_codigo = "[ACESSO] Análise Regulatória";
    op->modulo_id.Length = 2;
    op->modulo_id = {modANALISEREG, modENDERECOARIES};
    lisOP->Add(op);

    //vtcd
    op = new TModOp();
    op->operacao_id = opVTCD;
    op->operacao_codigo = "[ACESSO] Análise VTCD";
    op->modulo_id.Length = 1;
    op->modulo_id = {modANALISEVTCD};
    lisOP->Add(op);

    //confiabilidade malha
    op = new TModOp();
    op->operacao_id = opCONFIABMALHA;
    op->operacao_codigo = "[ACESSO] Confiabilidade em Malha";
    op->modulo_id.Length = 1;
    op->modulo_id = {modCONFIABILIDADE};
    lisOP->Add(op);

    //constantes
    op = new TModOp();
    op->operacao_id = opCONSTANTES;
    op->operacao_codigo = "[ACESSO] Cálculo de Constantes das Redes";
    op->modulo_id.Length = 1;
    op->modulo_id = {modTELIN};
    lisOP->Add(op);

    //aloca capacitor
    op = new TModOp();
    op->operacao_id = opREATIVOS;
    op->operacao_codigo = "[ACESSO] Compensação de Reativos e Alocação de Capacitores";
    op->modulo_id.Length = 1;
    op->modulo_id = {modALOCABC};
    lisOP->Add(op);

    //estabilidade
    op = new TModOp();
    op->operacao_id = opESTABILIDADE;
    op->operacao_codigo = "[ACESSO] Estabilidade e Oscilação";
    op->modulo_id.Length = 1;
    op->modulo_id = {modESTABILIDADE};
    lisOP->Add(op);

    //cargas
    op = new TModOp();
    op->operacao_id = opCARGAS;
    op->operacao_codigo = "[ACESSO] Evolução e Alocação de Cargas";
    op->modulo_id.Length = 3;
    op->modulo_id = {modAJUSTEDEMANDA, modESTIMADEMANDA, modMERCADO};
    lisOP->Add(op);

    //harmonico
    op = new TModOp();
    op->operacao_id = opHARMONICO;
    op->operacao_codigo = "[ACESSO] Fluxo Harmônico";
    op->modulo_id.Length = 1;
    op->modulo_id = {modFLUXOHARMONICO};
    lisOP->Add(op);

    //harmonico
    op = new TModOp();
    op->operacao_id = opMOTORCALCULO;
    op->operacao_codigo = "[ACESSO] Motor de Cálculo";
    op->modulo_id.Length = 1;
    op->modulo_id = {modMOTORCALCULO};
    lisOP->Add(op);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
bool __fastcall TRBACManager::ExistePermissaoModulo(int modulo_id)
{
    // variaveis locais
    VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
    VTLicenca *licenca = licencas->ExisteLicenca();
    int operacao_id;
    TModOp *op;

    // proteção
    if (licenca == NULL)
        return (false);

    for (int n = 0; n < licenca->Operacoes.Length; n++)
    {// verifica cada uma das operacoes da licença
        operacao_id = licenca->Operacoes[n];
        if(operacao.find(operacao_id) != operacao.end())
        {//localiza operação
            op = operacao.find(operacao_id)->second;
            for(int m = 0; m < op->modulo_id.Length; m++)
            { //localiza modulo
                if(op->modulo_id[m] == modulo_id)
                    return (true);
            }
        }
    }

	return (false);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
UnicodeString __fastcall TRBACManager::GetDescricaoOperacao(int operacao_id)
{
	//variaveis locais
	UnicodeString descricao = "";
    TModOp *op;

	if(operacao.find(operacao_id) != operacao.end())
	{//localiza nome da operação
		op = operacao.find(operacao_id)->second;
        descricao = op->operacao_codigo;
    }
	return descricao;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/14642326
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TRBACManager::MontaMap(void)
{
    TModOp *op;

    for(int n = 0; n < lisOP->Count; n++)
    {
        op = (TModOp*)lisOP->Items[n];
        operacao[op->operacao_id] = op;
    }
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
bool __fastcall TRBACManager::SinapViewer(void)
{
    // variaveis locais
    VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
    VTLicenca *licenca = licencas->ExisteLicenca();

    // proteção
    if (licenca == NULL)
        return (false);
    // retorna se a licença possui operação opVIEW
    for(int n = 0; n < licenca->Operacoes.Length; n++)
    {
        if(licenca->Operacoes[n] == opVIEW)
            return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// eof
