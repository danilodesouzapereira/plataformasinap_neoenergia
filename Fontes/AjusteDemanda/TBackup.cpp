// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TBackup.h"
#include "..\Apl\VTApl.h"
#include "..\Copia\VTCopia.h"
#include "..\Curvas\VTCurva.h"
#include "..\ImportaMedicao\VTMedicao.h"
#include "..\Radial\VTRadial.h"
#include "..\Radial\VTPrimario.h"
#include "..\Radial\VTSecundario.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include <ModulosOpcionais\Fontes\AjusteBatch\VTBatchAjusteTask.h>
#include "..\..\DLL_Inc\Copia.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TPricarga::TPricarga(VTPrimario *primario)
{
	// inicia dados
	this->primario = primario;
	// cria lista de Cargas
	lisCARGA = new TList();
	cargas_originais = true;
}

// ---------------------------------------------------------------------------
__fastcall TPricarga::~TPricarga(void)
{
	primario = NULL;
	// destrói lista e seus objetos
	if (lisCARGA)
	{
		if(!cargas_originais)
			LimpaTList(lisCARGA);
		delete lisCARGA;
		lisCARGA = NULL;
	}
}

// ---------------------------------------------------------------------------
__fastcall TBackup::TBackup(VTApl *apl)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
	// cria lista
	lisPRI_CAR = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TBackup::~TBackup(void)
{
	// destrói lista e seus objetos
	if (lisPRI_CAR)
	{
		LimpaTList(lisPRI_CAR);
		delete lisPRI_CAR;
		lisPRI_CAR = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TBackup::Clear(void)
{
	// destrói objetos da lista
	LimpaTList(lisPRI_CAR);
}

// ---------------------------------------------------------------------------
bool __fastcall TBackup::DuplicaCargas(TList *lisCARGA)
{
	// variáveis locais
	VTCopia *copia;
	bool sucesso = false;

	// cria objeto VTCopia para clonar as Cargas
	if ((copia = DLL_NewObjCopia(apl)) != NULL)
	{ // a lista é remontada com as Cargas copiadas
		sucesso = copia->DuplicaCargas(lisCARGA);
	}
	// destrói objeto Copia
	delete copia;

	return sucesso;
}

// ---------------------------------------------------------------------------
TPricarga* __fastcall TBackup::ExistePricarga(VTPrimario *primario)
{
	// variáveis locais
	TPricarga *pricarga;

	for (int nr = 0; nr < lisPRI_CAR->Count; nr++)
	{
		pricarga = (TPricarga*)lisPRI_CAR->Items[nr];
		if (pricarga->primario == primario)
			return (pricarga);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15314553
void __fastcall TBackup::LimpaTList2(TList *lisCARGA)
{
	// var locais
	AnsiString nome = "";
	VTCarga *carga;
	VTBatchAjusteTask *batch = (VTBatchAjusteTask*)apl->GetObject(__classid(VTBatchAjusteTask));

	for (int n = lisCARGA->Count - 1; n >= 0; n--)
	{
		carga = (VTCarga*)lisCARGA->Items[n];
		try
		{
			nome = carga->Codigo;
			delete carga;
			lisCARGA->Remove(carga);
		}
		catch (Exception &e)
		{
			if (batch != NULL)
			{
				batch->LogAdd("Erro ao destruir carga " + nome);
			}
		}
		catch (...)
		{
			if (batch != NULL)
			{
				batch->LogAdd("Erro ao destruir carga " + nome);
			}
		}
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TBackup::RestauraCarga(TList *lisCARGA)
{
	// variáveis locais
	VTCarga *carga_orig, *carga_atual;

	for (int nc = 0; nc < lisCARGA->Count; nc++)
	{
		carga_orig = (VTCarga*)lisCARGA->Items[nc];
		carga_atual = (VTCarga*)(carga_orig->Obj);
		if ((carga_orig == NULL) || (carga_atual == NULL))
			continue;
		// retira a Carga atual da Barra
		carga_atual->pbarra->RemoveEqbar(carga_atual);
		// ATENÇÃO: insere carga_atual na mesma posição da lista onde estava a carga_orig
		lisCARGA->Items[nc] = carga_atual;
		// insere a Carga original na Barra
		carga_orig->pbarra->InsereEqbar(carga_orig);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBackup::RestauraCargaMedicao(TList *lisMED_ERRO)
{
	// variáveis locais
	VTMedicao *medicao;
	VTPrimario *primario;
	VTRadial *radial = (VTRadial*)apl->GetObject(__classid(VTRadial));
	TList *lisPRI_AJUS = new TList();

	for (int nr = 0; nr < lisMED_ERRO->Count; nr++)
	{ // retorna cargas ajustaveis ao estado original
		medicao = (VTMedicao*)lisMED_ERRO->Items[nr];
		// RestauraCarga(medicao->lisCAR_AJUS);
		// DVK 2016.05.17
		// retorna todas as cargas do primario onde está a medição
		primario = radial->ExistePrimario(medicao->chave_med->rede);
		if (primario == NULL)
			continue;
		// verifica se o primario ja foi restaurado
		if (lisPRI_AJUS->IndexOf(primario) >= 0)
			continue;
		RestauraCargaUmPrimario(primario);
		lisPRI_AJUS->Add(primario);
	}
	// destroi lista
	delete lisPRI_AJUS;
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBackup::RestauraCargaTodosPrimarios(void)
{
	// variáveis locais
	TPricarga *pricarga;
	// VTCarga    *carga;

	for (int nr = 0; nr < lisPRI_CAR->Count; nr++)
	{
		pricarga = (TPricarga*)lisPRI_CAR->Items[nr];
		RestauraCargaUmPrimario(pricarga->primario);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBackup::RestauraCargaUmPrimario(VTPrimario *primario)
{
	// variáveis locais
	TPricarga *pricarga;

	// proteção
	if (!primario)
		return (false);
	// determina Pricarga do Primario
	if ((pricarga = ExistePricarga(primario)) == NULL)
		return (false);
	// restauras Cargas da Rede
	return (RestauraCarga(pricarga->lisCARGA));
}

// ---------------------------------------------------------------------------
bool __fastcall TBackup::RestauraCurvaCargaAtual(TList *lisCARGA)
{
	// variáveis locais
	VTCarga *carga_orig, *carga_atual; // , *carga_nova;

	// proteção
	if (lisCARGA == NULL)
		return (false);
	for (int nc = 0; nc < lisCARGA->Count; nc++)
	{
		carga_atual = (VTCarga*)lisCARGA->Items[nc];
		if (carga_atual->IP)
			continue;
		carga_orig = (VTCarga*)(carga_atual->Obj);
		if ((carga_orig == NULL) || (carga_atual == NULL))
			continue;
		// destroi curva própria da carga atual
		if (!carga_orig->curva->Tipica)
			delete carga_atual->curva;
		// copia curva da carga original
		if (carga_orig->curva->Tipica)
			carga_atual->curva = carga_orig->curva;
		else
			carga_atual->curva = carga_orig->curva->Clone();
	}
	return (true);
}
/*
 //---------------------------------------------------------------------------
 bool __fastcall  TBackup::SalvaCargaUmPrimario(VTPrimario *primario)
 {
 //variáveis locais
 TList        *lisSEC;
 TPricarga    *pricarga;
 VTCarga      *carga;
 VTSecundario *secundario;
 VTCopia      *copia;

 //verifica se já existe Pricarga da Primario
 if ((pricarga = ExistePricarga(primario)) != NULL) return(true);
 //cria um novo Pricarga e insere em lisPRI_CAR
 if ((pricarga = new TPricarga(primario)) == NULL) return(false);
 lisPRI_CAR->Add(pricarga);
 //inicia lista com Cargas originais do Primario
 primario->Rede->LisEqbar(pricarga->lisCARGA, eqptoCARGA);
 //completa lista com Cargas originais dos Secundários do Primario
 lisSEC = primario->LisSecundario();
 for (int n = 0; n < lisSEC->Count; n++)
 {
 secundario = (VTSecundario*)lisSEC->Items[n];
 secundario->Rede->LisEqbar(pricarga->lisCARGA, eqptoCARGA);
 }
 //cria objeto VTCopia para clonar as Cargas
 if ((copia = DLL_NewObjCopia(apl)) != NULL)
 {//a lista é remontada com as Cargas copiadas
 copia->DuplicaCargas(pricarga->lisCARGA);
 }
 //destrói objeto Copia
 delete copia;

 return(true);
 }
 */

// ---------------------------------------------------------------------------
bool __fastcall TBackup::SalvaCargaUmPrimario(VTPrimario *primario)
{
	// variáveis locais
	TList *lisSEC;
	TPricarga *pricarga;
	// VTCarga      *carga;
	VTSecundario *secundario;
	// VTCopia      *copia;
	VTBatchAjusteTask *batch = (VTBatchAjusteTask*)apl->GetObject(__classid(VTBatchAjusteTask));

	try
	{ //proteçao
		if(primario == NULL)
		{
			if (batch != NULL)
				batch->LogAdd("Primário nulo");
			return false;
		}
		if(primario->Rede == NULL)
		{
			if (batch != NULL)
				batch->LogAdd("Rede do Primário nula");
			return false;
		}
		// verifica se já existe Pricarga da Primario
		try
		{
			if ((pricarga = ExistePricarga(primario)) == NULL)
			{ // cria um novo Pricarga e insere em lisPRI_CAR
				if ((pricarga = new TPricarga(primario)) == NULL)
					return (false);
				lisPRI_CAR->Add(pricarga);
			}
		}
		catch (...)
		{
			if (batch != NULL)
				batch->LogDetalheAdd("ERRO no bloco 'Verifica se já existe pricarga do primario'");
			if (batch != NULL)
				batch->LogDetalheAdd("Erro no backup das cargas do primário " + primario->Rede->Codigo);
		}
		try
		{
			// destroi cargas atuais e inicia lista com Cargas originais do Primario
			if(!pricarga->cargas_originais)
				{LimpaTList2(pricarga->lisCARGA);}
		}
		catch (...)
		{
			if (batch != NULL)
				batch->LogDetalheAdd("ERRO no bloco 'destroi cargas atuais e inicia lista com Cargas originais do Primario'");
			if (batch != NULL)
				batch->LogDetalheAdd("Erro no backup das cargas do primário " + primario->Rede->Codigo);
		}
		try
		{
			primario->Rede->LisEqbar(pricarga->lisCARGA, eqptoCARGA);
		}
		catch (...)
		{
			if (batch != NULL)
				batch->LogDetalheAdd("ERRO no bloco '	primario->Rede->LisEqbar(pricarga->lisCARGA, eqptoCARGA)'");
			if (batch != NULL)
				batch->LogDetalheAdd("Erro no backup das cargas do primário " + primario->Rede->Codigo);
		}
		try
		{
			// completa lista com Cargas originais dos Secundários do Primario
			lisSEC = primario->LisSecundario();
			for (int n = 0; n < lisSEC->Count; n++)
			{
				secundario = (VTSecundario*)lisSEC->Items[n];
				secundario->Rede->LisEqbar(pricarga->lisCARGA, eqptoCARGA);
			}
		}
		catch (...)
		{
			if (batch != NULL)
				batch->LogDetalheAdd("ERRO no bloco 'completa lista com Cargas originais dos Secundários do Primario'");
			if (batch != NULL)
				batch->LogDetalheAdd("Erro no backup das cargas do primário " + primario->Rede->Codigo);
		}
		try
		{
			// remonta lista com as Cargas clonadas
			if(DuplicaCargas(pricarga->lisCARGA))
			{
				pricarga->cargas_originais = true;
            }
		}
		catch (...)
		{
			if (batch != NULL)
				batch->LogDetalheAdd("ERRO no bloco '// remonta lista com as Cargas clonadas'");
            if (batch != NULL)
				batch->LogDetalheAdd("Erro no backup das cargas do primário " + primario->Rede->Codigo);
		}
	}
	catch (Exception &e)
	{
		if (batch != NULL)
			batch->LogDetalheAdd("Erro no backup das cargas do primário " + primario->Rede->Codigo);
		return false;
	}
	return (true);
}

// ---------------------------------------------------------------------------
// eof
