// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.hpp>
#include "TInterfaceNIX.h"
#include "VTNixAL.h"
#include "TNixBc.h"
#include "TNixCb.h"
#include "TNixIt.h"
#include "TNixNo.h"
// #include "TNixPr.h"
// #include "TNixPrNeoEnergia.h"
#include "VTNixPr.h"
#include "TNixRg.h"
//#include "TNixTf.h"
#include "VTNixTf.h"
#include "TNixTr.h"
#include "..\Apl\VTApl.h"
#include "..\Edita\VTEdita.h"  //added FKM 2016.01.28
#include "..\InfoZcc\VTInfoZccManager.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTrafo.h"
#include "..\Radial\VTRadial.h"
#include "..\Radial\VTPrimario.h"
#include "..\Radial\VTSecundario.h"
#include "..\Radial\VTSubestacao.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\InfoZcc.h"
#include "..\..\DLL_Inc\Radial.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTInterfaceNIX* __fastcall NewObjInterfaceNIX(VTApl *apl)
{
	return (new TInterfaceNIX(apl));
}

// ---------------------------------------------------------------------------
__fastcall TInterfaceNIX::TInterfaceNIX(VTApl *apl)
{
	VTInfoZccManager *infoZccManager;

	// salva ponteiro p/ objeto
	this->apl = apl;

	infoZccManager = (VTInfoZccManager*)apl->GetObject(__classid(VTInfoZccManager));
	// cria objetos
	lisEQP = new TList();
	lisBARRAS = new TList();
	lisRedesCarregadas = new TList(); // FKM 2016.04.27
	radial = DLL_NewObjRadial(apl);
	// inicializa o maioID
	maiorId = 0;
	// inicializa cfg
	cfg.pr_com_corrente = false;
	cfg.al_com_corrente = true;
	cfg.fd_unico_a4 = false;
	cfg.fd_unico_trafo = false;
	cfg.patamar = -1;
	cfg.fdA4 = 1.;
	cfg.fdTrafo = 1.;
	cfg.carregDiurno = false;
	// adiciona o InfoZcc no apl caso nao exista
	if (infoZccManager == NULL)
	{
		apl->Add((infoZccManager = DLL_NewObjInfoZccManager(apl)));
	}

	if (infoZccManager != NULL)
	{
		infoZccManager->LeInfoZcc();
	}
}

// ---------------------------------------------------------------------------
__fastcall TInterfaceNIX::~TInterfaceNIX(void)
{
	// destrói objetos
	if (radial)
	{
		delete radial;
		radial = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	if (lisBARRAS)
	{
		delete lisBARRAS;
		lisBARRAS = NULL;
	}
	if (lisRedesCarregadas)
	{
		delete lisRedesCarregadas;
		lisRedesCarregadas = NULL;
	} // FKM 2016.04.27
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceNIX::AcertaIdBarras(void)
{ // variáveis locais
	VTBarra *barra;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisBARRASidNeg = new TList;

	// monta lista com Barras dos Primarios
	lisEQP->Clear();
	lisBARRAS->Clear();
	redes->LisBarra(lisEQP);
	// verifica os ids de todas barras
	maiorId = 0;
	for (int nb = 0; nb < lisEQP->Count; nb++)
	{
		barra = (VTBarra*) lisEQP->Items[nb];
		// verifica se a barra possu id positivo
		if (barra->Id > 0)
		{
			lisBARRAS->Add(barra);
			// verifica qual o maior id
			if (barra->Id > maiorId)
				maiorId = barra->Id;
		}
		else
		{
			lisBARRASidNeg->Add(barra);
		}
	}
	// apos verificar qual o maior id, transforma os ids negativos em positivos
	for (int nbn = 0; nbn < lisBARRASidNeg->Count; nbn++)
	{
		barra = (VTBarra*) lisBARRASidNeg->Items[nbn];
		// soma o modulo do id ao maior id encontrado.
		barra->Id = maiorId + abs(barra->Id);
		// adiciona à lsita de barras
		lisBARRAS->Add(barra);
	}
	// destrói lista sem destruir seus objetos
	if (lisBARRASidNeg)
	{
		delete lisBARRASidNeg;
		lisBARRASidNeg = NULL;
	}
	return true;
}

// ---------------------------------------------------------------------------
// bool __fastcall  TInterfaceNIX::ArranjosCodigoFalso(TList *lisARRANJOS)
// {  //variaveis locais
// VTArranjo *arranjo;
//
// //proteção
// if (!lisARRANJOS) {return false;}
// //percorre todos arranjos
// for (int na = 0; na < lisARRANJOS->Count ; nc++)
// {
// arranjo = (VTArranjo*) lisARRANJOS->Items[nc];
// }
//
// }
////---------------------------------------------------------------------------
// bool __fastcall  TInterfaceNIX::ArranjosCodigoOriginal(TList *lisARRANJOS)
// {
//
// }

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceNIX::BarraInicialIDFalso(VTPrimario *primario)
{ // variáveis locais
	VTBarra *barra;
	VTRede *rede;
	int falso_id;

	// proteção
	if (primario == NULL)
		return false;
	// recupera a rede
	rede = primario->Rede;
	// recupera o falso id
	falso_id = rede->Tag;
	// altera a barra inicial
	barra = primario->BarraInicial;
	// guarda o id original
	barra->Tag = barra->Id;
	// muda pro falso
	barra->Id = falso_id;
	return true;
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceNIX::BarraInicialIDOriginal(VTPrimario *primario)
{ // variáveis locais
	VTBarra *barra;
	int orig_id;

	// proteção
	if (primario == NULL)
		return false;
	// altera a barra inicial
	barra = primario->BarraInicial;
	// recupera o id original
	barra->Id = barra->Tag;
	return true;
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceNIX::BarrasIniciaisCriaFalsoID(void)
{ // variáveis locais
	VTBarra *barra;
	VTRede *rede;
	VTPrimario *primario;
	TList *lisPRIMARIO;

	lisPRIMARIO = radial->LisPrimario();
	// verifica se ja foi acertados os ids negativos
	if (maiorId == 0)
		AcertaIdBarras();
	// percorre todos primarios
	for (int np = 0; np < lisPRIMARIO->Count; np++)
	{
		primario = (VTPrimario*) lisPRIMARIO->Items[np];
		rede = primario->Rede;
		// guarda no tag da rede, um id provisorio
		rede->Tag = maiorId + abs(FalsoId());
	}
	return true;
}

// ---------------------------------------------------------------------------
VTNixAl* __fastcall TInterfaceNIX::CriaNIXAl(VTPrimario *primario, strIMP &z0, strIMP &z1,
	strIMP &z2, bool al_com_corrente, int patamar)
{
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca *licenca = licencas->ExisteLicenca();
	VTNixAl *nix_al = NULL;

	switch (licenca->Empresa)
	{

	case EDP_BANDEIRANTES:
		nix_al = NewObjNixAlEDPSP(apl, primario, z0, z1, z2, al_com_corrente, patamar);
		break;
	case EDP_ESCELSA: // por enquanto´é igual à da SP, mas vai saber
		nix_al = NewObjNixAlEDPSP(apl, primario, z0, z1, z2, al_com_corrente, patamar);
		break;
	default:
		nix_al = NewObjNixAlDefault(primario, z0, z1, z2, al_com_corrente, patamar);

	}
	return (nix_al);
}

// ---------------------------------------------------------------------------
VTNixPr* __fastcall TInterfaceNIX::CriaNIXPr(VTChave *chave, int index, bool pr_com_corrente)
{
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca *licenca = licencas->ExisteLicenca();
	VTNixPr *nix_pr = NULL;

	switch (licenca->Empresa)
	{
	//Considera o caso ESCELSA da mesma forma que o caso BANDEIRANTES
	case EDP_ESCELSA:
	case EDP_BANDEIRANTES:
		nix_pr = NewObjNixPrEDPSP(chave, index, pr_com_corrente);
		break;
	case COELBA:
		nix_pr = NewObjNixPrNeoEnergia(chave, index, pr_com_corrente);
		break;
	default:
		nix_pr = NewObjNixPrDefault(chave, index, pr_com_corrente);

	}
	return (nix_pr);
}

// ---------------------------------------------------------------------------
// bool __fastcall TInterfaceNIX::DeterminaZeqv(VTPrimario *primario, strIMP &z0, strIMP &z1, strIMP &z2)
// {
// //variáveis locais
// VTSuprimento *suprimento = NULL;
//
// //código temporário
// if (primario->Subestacao != NULL)
// {//determina Suprimento da Subestacao
// lisEQP->Clear();
// primario->Subestacao->Rede->LisEqbar(lisEQP, eqptoSUPRIMENTO);
// if (lisEQP->Count > 0) suprimento = (VTSuprimento*)lisEQP->First();
// }
// //verifica se foi encontrado um SUprimento
// if (suprimento == NULL)
// {//assume valores default
// z0.r = 0.04167;
// z0.x = 0.12500;
// z1.r = 0.01667;
// z1.x = 0.05000;
// z2   = z1;
// }
// else
// {//assume impedância equivalente do Suprimento da Subestacao
// z0 = suprimento->zeq0;
// z1 = suprimento->zeq1;
// z2 = z1;
// }
// return(true);
// }
// ---------------------------------------------------------------------------
// FKM 2016.01.28 - subia valores zerados
bool __fastcall TInterfaceNIX::DeterminaZeqv(VTPrimario *primario, strIMP &z0, strIMP &z1,
	strIMP &z2)
{
	// variáveis locais
	VTSuprimento *suprimento = NULL;

	// código temporário
	if (primario->Subestacao != NULL)
	{ // determina Suprimento da Subestacao
		lisEQP->Clear();
		primario->Subestacao->Rede->LisEqbar(lisEQP, eqptoSUPRIMENTO);
		if (lisEQP->Count > 0)
			suprimento = (VTSuprimento*)lisEQP->First();
		else
		{
			primario->BarraInicial->LisEqbar(lisEQP, eqptoSUPRIMENTO);
			if (lisEQP->Count > 0)
				suprimento = (VTSuprimento*)lisEQP->First();
		}
	}
	else
	{ // determina Suprimento do Primario
		lisEQP->Clear();
		if (primario->BarraInicial != NULL)
		{
			primario->BarraInicial->LisEqbar(lisEQP, eqptoSUPRIMENTO);
		}
		if (lisEQP->Count > 0)
			suprimento = (VTSuprimento*)lisEQP->First();
	}
	// verifica se foi encontrado um SUprimento
	if (suprimento == NULL)
	{ // assume valores default   // z0 = 0.85*z1
		z0.r = 0.01; // 0.04167;     //0.01
		z0.x = 0.04; // 0.12500;     //0.4
		z1.r = 0.01; // 0.01667;     //0.01
		z1.x = 0.5; // 0.05000;     //0.5
		z2 = z1;
	}
	else
	{ // caso os valores sejam zerados...
		if (IsDoubleZero(suprimento->zeq0.r) && IsDoubleZero(suprimento->zeq0.x))
		{ // assume valores default
			z0.r = 0.01; // 0.04167;
			z0.x = 0.04; // 0.12500;
		}
		else
		{ // assume impedância equivalente do Suprimento da Subestacao
			z0 = suprimento->zeq0;
		}
		if (IsDoubleZero(suprimento->zeq1.r) && IsDoubleZero(suprimento->zeq1.x))
		{ // assume valores default
			z1.r = 0.01; // 0.01667;
			z1.x = 0.5; // 0.05000;
		}
		else
		{ // assume impedância equivalente do Suprimento da Subestacao
			z1 = suprimento->zeq1;
		}
		z2 = z1;
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceNIX::ExportaArqNix(TStrings *lines)
{
	// variáveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// obtem a lista de redes carregadas
	redes->LisRedeCarregada(lisRedesCarregadas);
	// salva ponteiro para objeto
	this->lines = lines;
	// inicia objeto Radial
	if (!radial->Inicia(redes))
	{
		Aviso("A rede selecionada não é radial");
		return (false);
	}
	// exporta blocos de dados do arquivo NIX
	lines->Clear();
	// troca os ids negativos por outros positivos
	AcertaIdBarras(); // FKM 2015.12.17
	BarrasIniciaisCriaFalsoID(); // FKM 2016.01.28
	if (!ExportaBlocoAL(cfg.al_com_corrente, cfg.patamar))
		return (false);
	if (!ExportaBlocoNO())
		return (false);
	if (!ExportaBlocoTF())
		return (false);
	if (!ExportaBlocoTR())
		return (false);
	if (!ExportaBlocoBC())
		return (false);
	if (!ExportaBlocoRG())
		return (false);
	if (!ExportaBlocoCB())
		return (false);
	if (!ExportaBlocoPR(cfg.pr_com_corrente))
		return (false);
	// if (! ExportaBlocoIT()) return(false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceNIX::ExportaBlocoAL(bool al_com_corrente, int patamar)
{
	// variáveis locais
	AnsiString txt;
	strIMP z0, z1, z2;
	VTPrimario *primario;
	TList *lisPRIMARIO = radial->LisPrimario();
	VTNixAl *nix_al;

	// loop p/ todos Primarios
	for (int n = 0; n < lisPRIMARIO->Count; n++)
	{
		primario = (VTPrimario*)lisPRIMARIO->Items[n];
		// FKM 2016.04.27 caso o primário não seja valido
		if (!PrimarioValido(primario))
		{
			continue;
		}
		// altera o id da barra inicial para um id falso
		BarraInicialIDFalso(primario);
		// determina impedâncias equivalentes na entrada do primário
		DeterminaZeqv(primario, z0, z1, z2);
		// inicia objeto NixAl
		// TNixAl nix_al(primario, z0, z1, z2, al_com_corrente, patamar);
		nix_al = CriaNIXAl(primario, z0, z1, z2, al_com_corrente, patamar);
		// insere linha em StringLista
		lines->Add(nix_al->LinhaArqCsv());
		// altera o id da barra inicial para o id original
		BarraInicialIDOriginal(primario);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceNIX::ExportaBlocoBC(void)
{
	// variáveis locais
	VTCapacitor *capacitor;
	VTPrimario *primario;
	TList *lisPRIMARIO = radial->LisPrimario();

	for (int n = 0; n < lisPRIMARIO->Count; n++)
	{
		primario = (VTPrimario*)lisPRIMARIO->Items[n];
		// FKM 2016.04.27 caso o primário não seja valido
		if (!PrimarioValido(primario))
		{
			continue;
		}
		// altera o id da barra inicial para um falso
		BarraInicialIDFalso(primario);
		// monta lista com Capacitores dos Primarios
		lisEQP->Clear();
		primario->Rede->LisEqbar(lisEQP, eqptoCAPACITOR);
		// loop p/ todos Capacitores
		for (int n = 0; n < lisEQP->Count; n++)
		{
			capacitor = (VTCapacitor*)lisEQP->Items[n];
			TNixBc nix_bc(capacitor);
			// insere linha em StringLista
			lines->Add(nix_bc.LinhaArqCsv());
		}
		// altera o id da barra inicial para o id original
		BarraInicialIDOriginal(primario);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceNIX::ExportaBlocoCB(void)
{
	// variáveis locais
	VTArranjo *arranjo;
	VTPrimario *primario;
	TList *lisPRIMARIO = radial->LisPrimario();

	// monta lista com Arranjos dos Trechos dos Primarios
	lisEQP->Clear();
	for (int n = 0; n < lisPRIMARIO->Count; n++)
	{
		primario = (VTPrimario*)lisPRIMARIO->Items[n];
		// FKM 2016.04.27 caso o primário não seja valido
		if (!PrimarioValido(primario))
		{
			continue;
		}
		// obtém lista de Arranjos usado no Primario
		primario->Rede->LisArranjo(lisEQP);
	}
	// loop p/ todos Arranjos
	for (int n = 0; n < lisEQP->Count; n++)
	{
		arranjo = (VTArranjo*)lisEQP->Items[n];
		TNixCb nix_cb(arranjo);
		// insere linha em StringLista
		lines->Add(nix_cb.LinhaArqCsv());
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceNIX::ExportaBlocoIT(void)
{
	return (true);
}

// ---------------------------------------------------------------------------
// bool __fastcall TInterfaceNIX::ExportaBlocoNO(void)
// {
// //variáveis locais
// VTBarra    *barra;
//
// //loop p/ todas Barras
// for (int n = 0; n < lisBARRAS->Count; n++)
// {
// barra = (VTBarra*)lisBARRAS->Items[n];
// TNixNo nix_no(barra);
// //insere linha em StringLista
// lines->Add(nix_no.LinhaArqCsv());
// }
// return(true);
// }
// ---------------------------------------------------------------------------
bool __fastcall TInterfaceNIX::ExportaBlocoNO(void) // FKM 2016.01.28
{
	// variáveis locais
	VTBarra *barra;
	TList *lisPRIMARIO = radial->LisPrimario();
	// TList      *lisBARRA;
	VTPrimario *primario;
	VTRede *rede;

	// loop p/ todos Primarios adicionando somente as barras iniciais
	for (int n = 0; n < lisPRIMARIO->Count; n++)
	{
		primario = (VTPrimario*)lisPRIMARIO->Items[n];
		// FKM 2016.04.27 caso o primário não seja valido
		if (!PrimarioValido(primario))
		{
			continue;
		}
		// altera o id da barra inicial para um falso
		BarraInicialIDFalso(primario);
		barra = primario->BarraInicial;
		TNixNo nix_no(barra);
		// insere linha em StringLista
		lines->Add(nix_no.LinhaArqCsv());
		// altera o id da barra inicial para o id original
		BarraInicialIDOriginal(primario);
	}
	// limpa a lista de barras;
	lisBARRAS->Clear();
	// loop p/ todos Primarios,
	for (int n = 0; n < lisPRIMARIO->Count; n++)
	{
		primario = (VTPrimario*)lisPRIMARIO->Items[n];
		// FKM 2016.04.27 caso o primário não seja valido
		if (!PrimarioValido(primario))
		{
			continue;
		}
		rede = primario->Rede;
		rede->LisBarra(lisBARRAS);
	}
	// loop p/ todas Barras da rede
	for (int n = 0; n < lisBARRAS->Count; n++)
	{
		barra = (VTBarra*)lisBARRAS->Items[n];
		TNixNo nix_no(barra);
		// insere linha em StringLista
		lines->Add(nix_no.LinhaArqCsv());
	}
	return (true);
}

// ---------------------------------------------------------------------------
// bool __fastcall TInterfaceNIX::ExportaBlocoPR(bool pr_com_corrente)
// {
// //variáveis locais
// VTChave     *chave;
// VTPrimario  *primario;
// TList       *lisPRIMARIO = radial->LisPrimario();
//
// //determina lista de Chaves de todos Primarioa
// lisEQP->Clear();
// //loop p/ todos Primarios
// for (int n = 0; n < lisPRIMARIO->Count; n++)
// {
// primario = (VTPrimario*)lisPRIMARIO->Items[n];
// primario->Rede->LisLigacao(lisEQP, eqptoCHAVE);
// }
// for (int i = 0; i < lisEQP->Count; i++)
// {
// chave = (VTChave*)lisEQP->Items[i];
// //inicia objeto NixPr
// TNixPr nix_pr(chave, i+1, pr_com_corrente);
// //insere linha em StringLista
// lines->Add(nix_pr.LinhaArqCsv());
// }
// return(true);
// }
// ---------------------------------------------------------------------------
bool __fastcall TInterfaceNIX::ExportaBlocoPR(bool pr_com_corrente)
{
	// variáveis locais
	VTChave *chave;
	VTPrimario *primario;
	TList *lisPRIMARIO = radial->LisPrimario();

	// determina lista de Chaves de todos Primarioa
	lisEQP->Clear();
	// loop p/ todos Primarios
	for (int n = 0; n < lisPRIMARIO->Count; n++)
	{
		primario = (VTPrimario*)lisPRIMARIO->Items[n];
		// FKM 2016.04.27 caso o primário não seja valido
		if (!PrimarioValido(primario))
		{
			continue;
		}
		// altera o id da barra inicial para um falso
		BarraInicialIDFalso(primario);
		// determina lista de CHAVES do Primario
		lisEQP->Clear();
		primario->Rede->LisLigacao(lisEQP, eqptoCHAVE);
		for (int i = 0; i < lisEQP->Count; i++)
		{
			chave = (VTChave*)lisEQP->Items[i];
			// inicia objeto NixPr
			// TNixPr nix_pr(chave, i+1, pr_com_corrente);
			// TNixPr nix_pr(chave, chave->Id, pr_com_corrente); //FKM 2016.02.03 - nix necessita de códigos unicos paras chaves
			VTNixPr *nix_pr = CriaNIXPr(chave, chave->Id, pr_com_corrente);
			// insere linha em StringLista
			lines->Add(nix_pr->LinhaArqCsv());
			delete nix_pr;
		}
		// altera o id da barra inicial para o id original
		BarraInicialIDOriginal(primario);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceNIX::ExportaBlocoRG(void)
{
	// variáveis locais
	VTRegulador *regulador;
	VTPrimario *primario;
	TList *lisPRIMARIO = radial->LisPrimario();

	// loop p/ todos Primarios
	for (int n = 0; n < lisPRIMARIO->Count; n++)
	{
		primario = (VTPrimario*)lisPRIMARIO->Items[n];
		// FKM 2016.04.27 caso o primário não seja valido
		if (!PrimarioValido(primario))
		{
			continue;
		}
		// altera o id da barra inicial para um falso
		BarraInicialIDFalso(primario);
		// determina lista de Reguladores do Primario
		lisEQP->Clear();
		primario->Rede->LisLigacao(lisEQP, eqptoREGULADOR);
		for (int i = 0; i < lisEQP->Count; i++)
		{
			regulador = (VTRegulador*)lisEQP->Items[i];
			// inicia objeto NixRg
			TNixRg nix_rg(regulador);
			// insere linha em StringLista
			lines->Add(nix_rg.LinhaArqCsv());
		}
		// altera o id da barra inicial para o id original
		BarraInicialIDOriginal(primario);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceNIX::ExportaBlocoTF(void)
{
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca *licenca = licencas->ExisteLicenca();
	bool sucesso = false;

	switch (licenca->Empresa)
	{

	case ELETROPAULO:
		sucesso = ExportaBlocoTFELPA();
		break;
	default:
		sucesso = ExportaBlocoTFDefault();

	}
	return (sucesso);
}
// ---------------------------------------------------------------------------
bool __fastcall  TInterfaceNIX::ExportaBlocoTFDefault(void)
{
	// variáveis locais
	VTCarga *carga;
	VTBarra *barraCarga;
	VTPrimario *primario;
	VTSecundario *secundario;
	TList *lisSECUNDARIO;
	TList *lisPRIMARIO = radial->LisPrimario();
	TList *lisTRFAux = new TList;
	TList *lisBANCOAux = new TList;
	VTTrafo *trafo;
	VTZona *zonaPrincipal;

	// loop p/ todos Primarios
	for (int n = 0; n < lisPRIMARIO->Count; n++)
	{
		primario = (VTPrimario*)lisPRIMARIO->Items[n];
		// FKM 2016.04.27 caso o primário não seja valido
		if (!PrimarioValido(primario))
		{
			continue;
		}
		// guarda a zona principal
		zonaPrincipal = primario->Rede->BarraInicial()->zona;
		// altera o id da barra inicial para um falso
		BarraInicialIDFalso(primario);
		// obtém lista de Cargas do Primário
		lisEQP->Clear();
		primario->Rede->LisEqbar(lisEQP, eqptoCARGA);
		for (int nc = 0; nc < lisEQP->Count; nc++)
		{
			carga = (VTCarga*)lisEQP->Items[nc];
			// verifica se a carga pertence à zona principal
			barraCarga = carga->pbarra;
			if (barraCarga->zona != zonaPrincipal)
			{
				continue;
			}
			// inicia objeto NixTf
			VTNixTf *nix_tf = NewObjNixTfDefault(apl, carga, cfg);
			// insere linha em StringLista
			lines->Add(nix_tf->LinhaArqCsv());
			delete nix_tf;
		}
		// determina lista de Secundarios do Primario
		lisSECUNDARIO = primario->LisSecundario();
		for (int ns = 0; ns < lisSECUNDARIO->Count; ns++)
		{
			secundario = (VTSecundario*)lisSECUNDARIO->Items[ns];
			// inicia objeto NixTf
			//VTNixTf *nix_tf = NewObjNixTfDefault(apl, secundario, cfg);
			// insere linha em StringLista
			//lines->Add(nix_tf->LinhaArqCsv());
			// É necessário analisar os trafos em cada secundario  //DSF
			// copia lista de todos trafos do secundario
			lisTRFAux->Clear();
			secundario->Rede->LisEqpto(lisTRFAux, eqptoTRAFO);
			// percorre lista de trafos do secundario
			while (lisTRFAux->Count > 0)
			{ // pega o 1o trafo da lista
				trafo = (VTTrafo*)lisTRFAux->First();
				// verifica se ele tem trafos em paralelo
				lisBANCOAux->Clear();
				LisTrafoParalelo(trafo, lisBANCOAux);
				// inicia objeto NixTf
				VTNixTf *nix_tf = NewObjNixTfDefault(apl, lisBANCOAux, cfg);
				// insere linha em StringLista
				lines->Add(nix_tf->LinhaArqCsv());
				delete nix_tf;
				// remove os trafos analisados da lista de trafos
				for (int nt = 0; nt < lisBANCOAux->Count; nt++)
				{ // se o trafo existir na lsita auxiliar, o remove
					if (lisTRFAux->IndexOf(lisBANCOAux->Items[nt]) >= 0)
					{
						lisTRFAux->Remove(lisBANCOAux->Items[nt]);
					}
				}
			}
			lisTRFAux->Clear();
		}
		// copia lista de todos trafos do primario
		primario->Rede->LisEqpto(lisTRFAux, eqptoTRAFO);
		// percorre lista de trafos do primario
		while (lisTRFAux->Count > 0)
		{ // pega o 1o trafo da lista
			trafo = (VTTrafo*)lisTRFAux->First();
			// verifica se ele tem trafos em paralelo
			lisBANCOAux->Clear();
			LisTrafoParalelo(trafo, lisBANCOAux);
			// inicia objeto NixTf
			VTNixTf *nix_tf = NewObjNixTfDefault(apl, lisBANCOAux, cfg);
			// insere linha em StringLista
			lines->Add(nix_tf->LinhaArqCsv());
			delete nix_tf;
			// remove os trafos analisados da lista de trafos
			for (int nt = 0; nt < lisBANCOAux->Count; nt++)
			{ // se o trafo existir na lsita auxiliar, o remove
				if (lisTRFAux->IndexOf(lisBANCOAux->Items[nt]) >= 0)
				{
					lisTRFAux->Remove(lisBANCOAux->Items[nt]);
				}
			}
		}
		// altera o id da barra inicial para o id original
		BarraInicialIDOriginal(primario);
	}
	return (true);
}
// ---------------------------------------------------------------------------
bool __fastcall  TInterfaceNIX::ExportaBlocoTFELPA(void)
{
	// variáveis locais
	VTCarga *carga;
	VTBarra *barraCarga;
	VTPrimario *primario;
	VTSecundario *secundario;
	TList *lisSECUNDARIO;
	TList *lisPRIMARIO = radial->LisPrimario();
	TList *lisTRFAux = new TList;
	TList *lisBANCOAux = new TList;
	VTTrafo *trafo;
	VTZona *zonaPrincipal;

	// loop p/ todos Primarios
	for (int n = 0; n < lisPRIMARIO->Count; n++)
	{
		primario = (VTPrimario*)lisPRIMARIO->Items[n];
		// FKM 2016.04.27 caso o primário não seja valido
		if (!PrimarioValido(primario))
		{
			continue;
		}
		// guarda a zona principal
		zonaPrincipal = primario->Rede->BarraInicial()->zona;
		// altera o id da barra inicial para um falso
		BarraInicialIDFalso(primario);
		// obtém lista de Cargas do Primário
		lisEQP->Clear();
		primario->Rede->LisEqbar(lisEQP, eqptoCARGA);
		for (int nc = 0; nc < lisEQP->Count; nc++)
		{
			carga = (VTCarga*)lisEQP->Items[nc];
			// verifica se a carga pertence à zona principal
			barraCarga = carga->pbarra;
			if (barraCarga->zona != zonaPrincipal)
			{
				continue;
			}
			// inicia objeto NixTf
			VTNixTf *nix_tf = NewObjNixTfElpa(apl, carga, cfg);
			// insere linha em StringLista
			lines->Add(nix_tf->LinhaArqCsv());
			delete nix_tf;
		}
		// determina lista de Secundarios do Primario
		lisSECUNDARIO = primario->LisSecundario();
		for (int ns = 0; ns < lisSECUNDARIO->Count; ns++)
		{
			secundario = (VTSecundario*)lisSECUNDARIO->Items[ns];
			// inicia objeto NixTf
			//VTNixTf *nix_tf = NewObjNixTfElpa(apl, secundario, cfg);
			// insere linha em StringLista
			//lines->Add(nix_tf->LinhaArqCsv());
			// É necessário analisar os trafos em cada secundario  //DSF
			// copia lista de todos trafos do secundario
			lisTRFAux->Clear();
			secundario->Rede->LisEqpto(lisTRFAux, eqptoTRAFO);
			// percorre lista de trafos do secundario
			while (lisTRFAux->Count > 0)
			{ // pega o 1o trafo da lista
				trafo = (VTTrafo*)lisTRFAux->First();
				// verifica se ele tem trafos em paralelo
				lisBANCOAux->Clear();
				LisTrafoParalelo(trafo, lisBANCOAux);
				// inicia objeto NixTf
				VTNixTf *nix_tf = NewObjNixTfElpa(apl, lisBANCOAux, cfg);
				// insere linha em StringLista
				lines->Add(nix_tf->LinhaArqCsv());
				delete nix_tf;
				// remove os trafos analisados da lista de trafos
				for (int nt = 0; nt < lisBANCOAux->Count; nt++)
				{ // se o trafo existir na lsita auxiliar, o remove
					if (lisTRFAux->IndexOf(lisBANCOAux->Items[nt]) >= 0)
					{
						lisTRFAux->Remove(lisBANCOAux->Items[nt]);
					}
				}
			}
			lisTRFAux->Clear();
		}
		// copia lista de todos trafos do primario
		primario->Rede->LisEqpto(lisTRFAux, eqptoTRAFO);
		// percorre lista de trafos do primario
		while (lisTRFAux->Count > 0)
		{ // pega o 1o trafo da lista
			trafo = (VTTrafo*)lisTRFAux->First();
			// verifica se ele tem trafos em paralelo
			lisBANCOAux->Clear();
			LisTrafoParalelo(trafo, lisBANCOAux);
			// inicia objeto NixTf
			VTNixTf *nix_tf = NewObjNixTfElpa(apl, lisBANCOAux, cfg);
			// insere linha em StringLista
			lines->Add(nix_tf->LinhaArqCsv());
			delete nix_tf;
			// remove os trafos analisados da lista de trafos
			for (int nt = 0; nt < lisBANCOAux->Count; nt++)
			{ // se o trafo existir na lsita auxiliar, o remove
				if (lisTRFAux->IndexOf(lisBANCOAux->Items[nt]) >= 0)
				{
					lisTRFAux->Remove(lisBANCOAux->Items[nt]);
				}
			}
		}
		// altera o id da barra inicial para o id original
		BarraInicialIDOriginal(primario);
	}
	return (true);
}
// ---------------------------------------------------------------------------
bool __fastcall TInterfaceNIX::ExportaBlocoTR(void)
{
	// variáveis locais
	VTTrecho *trecho;
	VTPrimario *primario;
	TList *lisPRIMARIO = radial->LisPrimario();

	// loop p/ todos Primarios
	for (int n = 0; n < lisPRIMARIO->Count; n++)
	{
		primario = (VTPrimario*)lisPRIMARIO->Items[n];
		// FKM 2016.04.27 caso o primário não seja valido
		if (!PrimarioValido(primario))
		{
			continue;
		}
		// altera o id da barra inicial para um falso
		BarraInicialIDFalso(primario);
		// determina lista de Trechos do Primario
		lisEQP->Clear();
		primario->Rede->LisLigacao(lisEQP, eqptoTRECHO);
		for (int i = 0; i < lisEQP->Count; i++)
		{
			trecho = (VTTrecho*)lisEQP->Items[i];
			// inicia objeto NixTr
			TNixTr nix_tr(trecho);
			// insere linha em StringLista
			lines->Add(nix_tr.LinhaArqCsv());
		}
		// altera o id da barra inicial para o id original
		BarraInicialIDOriginal(primario);
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TInterfaceNIX::LisTrafoParalelo(VTTrafo *trafo, TList *lisEXT)
{ // variaveis locais
	VTRede *rede;
	VTBarra *barra1, *barra2, *barra3, *barraPri, *barraSec;
	VTLigacao *ligacao;
	TList *lisLIG = new TList;

	rede = trafo->rede;
	// lista todas ligacoes daquela rede naquela barra
	barraPri = trafo->BarraPrimario();
	barraSec = trafo->BarraSecundario();
	rede->LisLigacao(lisLIG, barraPri);
	// percorre a lista de ligacoes
	for (int nl = 0; nl < lisLIG->Count; nl++)
	{
		ligacao = (VTLigacao*)lisLIG->Items[nl];
		barra1 = ligacao->pbarra1;
		barra2 = ligacao->pbarra2;
		barra3 = ligacao->pbarra3;
		// verifica se alguma barra é igual a barra do primario
		if ((barraPri == barra1) || (barraPri == barra2) || (barraPri == barra3))
		{ // verifica se alguma barra é igual a barra do secundario
			if ((barraSec == barra1) || (barraSec == barra2) || (barraSec == barra3))
			{ // se a ligacao for um trafo
				if (ligacao->Tipo() == eqptoTRAFO)
				{ // adiciona o trafo À lista
					if (lisEXT->IndexOf(ligacao) < 0)
					{
						lisEXT->Add(ligacao);
					}
				}
			}
		}
	}
	if (lisLIG)
	{
		delete lisLIG;
		lisLIG = NULL;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceNIX::PrimarioValido(VTPrimario *primario)
{
	// FKM 2016.04.25 - Caso o alimentador não possua barra inicial
	if (primario->Rede->BarraInicial() == NULL)
		return (false);
	// FKM 2016.04.27 - Caso o alimentador não tenha sido carregado
	if (!primario->Rede->Carregada)
		return (false);

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceNIX::SecundarioValido(VTSecundario *secundario)
{
	// FKM 2016.04.27 - Caso o alimentador não tenha sido carregado
	if (!secundario->Rede->Carregada)
		return (false);

	return (true);
}
// ---------------------------------------------------------------------------
// eof
