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
	// destr�i lista e seus objetos
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
	// destr�i lista e seus objetos
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
	// destr�i objetos da lista
	LimpaTList(lisPRI_CAR);
}

// ---------------------------------------------------------------------------
bool __fastcall TBackup::DuplicaCargas(TList *lisCARGA)
{
	// vari�veis locais
	VTCopia *copia;
	bool sucesso = false;

	// cria objeto VTCopia para clonar as Cargas
	if ((copia = DLL_NewObjCopia(apl)) != NULL)
	{ // a lista � remontada com as Cargas copiadas
		sucesso = copia->DuplicaCargas(lisCARGA);
	}
	// destr�i objeto Copia
	delete copia;

	return sucesso;
}

// ---------------------------------------------------------------------------
TPricarga* __fastcall TBackup::ExistePricarga(VTPrimario *primario)
{
	// vari�veis locais
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
	// vari�veis locais
	VTCarga *carga_orig, *carga_atual;

	for (int nc = 0; nc < lisCARGA->Count; nc++)
	{
		carga_orig = (VTCarga*)lisCARGA->Items[nc];
		carga_atual = (VTCarga*)(carga_orig->Obj);
		if ((carga_orig == NULL) || (carga_atual == NULL))
			continue;
		// retira a Carga atual da Barra
		carga_atual->pbarra->RemoveEqbar(carga_atual);
		// ATEN��O: insere carga_atual na mesma posi��o da lista onde estava a carga_orig
		lisCARGA->Items[nc] = carga_atual;
		// insere a Carga original na Barra
		carga_orig->pbarra->InsereEqbar(carga_orig);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBackup::RestauraCargaMedicao(TList *lisMED_ERRO)
{
	// vari�veis locais
	VTMedicao *medicao;
	VTPrimario *primario;
	VTRadial *radial = (VTRadial*)apl->GetObject(__classid(VTRadial));
	TList *lisPRI_AJUS = new TList();

	for (int nr = 0; nr < lisMED_ERRO->Count; nr++)
	{ // retorna cargas ajustaveis ao estado original
		medicao = (VTMedicao*)lisMED_ERRO->Items[nr];
		// RestauraCarga(medicao->lisCAR_AJUS);
		// DVK 2016.05.17
		// retorna todas as cargas do primario onde est� a medi��o
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
	// vari�veis locais
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
	// vari�veis locais
	TPricarga *pricarga;

	// prote��o
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
	// vari�veis locais
	VTCarga *carga_orig, *carga_atual; // , *carga_nova;

	// prote��o
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
		// destroi curva pr�pria da carga atual
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
 //vari�veis locais
 TList        *lisSEC;
 TPricarga    *pricarga;
 VTCarga      *carga;
 VTSecundario *secundario;
 VTCopia      *copia;

 //verifica se j� existe Pricarga da Primario
 if ((pricarga = ExistePricarga(primario)) != NULL) return(true);
 //cria um novo Pricarga e insere em lisPRI_CAR
 if ((pricarga = new TPricarga(primario)) == NULL) return(false);
 lisPRI_CAR->Add(pricarga);
 //inicia lista com Cargas originais do Primario
 primario->Rede->LisEqbar(pricarga->lisCARGA, eqptoCARGA);
 //completa lista com Cargas originais dos Secund�rios do Primario
 lisSEC = primario->LisSecundario();
 for (int n = 0; n < lisSEC->Count; n++)
 {
 secundario = (VTSecundario*)lisSEC->Items[n];
 secundario->Rede->LisEqbar(pricarga->lisCARGA, eqptoCARGA);
 }
 //cria objeto VTCopia para clonar as Cargas
 if ((copia = DLL_NewObjCopia(apl)) != NULL)
 {//a lista � remontada com as Cargas copiadas
 copia->DuplicaCargas(pricarga->lisCARGA);
 }
 //destr�i objeto Copia
 delete copia;

 return(true);
 }
 */

// ---------------------------------------------------------------------------
bool __fastcall TBackup::SalvaCargaUmPrimario(VTPrimario *primario)
{
	// vari�veis locais
	TList *lisSEC;
	TPricarga *pricarga;
	// VTCarga      *carga;
	VTSecundario *secundario;
	// VTCopia      *copia;
	VTBatchAjusteTask *batch = (VTBatchAjusteTask*)apl->GetObject(__classid(VTBatchAjusteTask));

	try
	{ //prote�ao
		if(primario == NULL)
		{
			if (batch != NULL)
				batch->LogAdd("Prim�rio nulo");
			return false;
		}
		if(primario->Rede == NULL)
		{
			if (batch != NULL)
				batch->LogAdd("Rede do Prim�rio nula");
			return false;
		}
		// verifica se j� existe Pricarga da Primario
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
				batch->LogDetalheAdd("ERRO no bloco 'Verifica se j� existe pricarga do primario'");
			if (batch != NULL)
				batch->LogDetalheAdd("Erro no backup das cargas do prim�rio " + primario->Rede->Codigo);
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
				batch->LogDetalheAdd("Erro no backup das cargas do prim�rio " + primario->Rede->Codigo);
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
				batch->LogDetalheAdd("Erro no backup das cargas do prim�rio " + primario->Rede->Codigo);
		}
		try
		{
			// completa lista com Cargas originais dos Secund�rios do Primario
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
				batch->LogDetalheAdd("ERRO no bloco 'completa lista com Cargas originais dos Secund�rios do Primario'");
			if (batch != NULL)
				batch->LogDetalheAdd("Erro no backup das cargas do prim�rio " + primario->Rede->Codigo);
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
				batch->LogDetalheAdd("Erro no backup das cargas do prim�rio " + primario->Rede->Codigo);
		}
	}
	catch (Exception &e)
	{
		if (batch != NULL)
			batch->LogDetalheAdd("Erro no backup das cargas do prim�rio " + primario->Rede->Codigo);
		return false;
	}
	return (true);
}

// ---------------------------------------------------------------------------
// eof
