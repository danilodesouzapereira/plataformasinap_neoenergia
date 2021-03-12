// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTTrafo.h>
#include <Fontes\Rede\VTTrafo3E.h>
#include <Fontes\Rede\VTRede.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Rede.h>
#include <TSe.h>
#include <TPri.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
TSe* __fastcall NewObjSe(void)
{
	try
	{
		return (new TSe());
	}
	catch (Exception &e)
	{
	}
	// erro na criação do objeto
	return (NULL);
}

// ---------------------------------------------------------------------------
__fastcall TSe::TSe(void)
{
	// cria listas
	lisBARRA = new TList();
	lisPRI = new TList();
	lisTRF = new TList();
	lisTRF3E = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TSe::~TSe(void)
{
	// destrói lista sem destruir seus objetos
	if (lisBARRA)
	{
		delete lisBARRA;
		lisBARRA = NULL;
	}
	if (lisTRF)
	{
		delete lisTRF;
		lisTRF = NULL;
	}
	if (lisTRF3E)
	{
		delete lisTRF3E;
		lisTRF3E = NULL;
	}
	// destrói lista e seus objetos
	if (lisPRI)
	{
		LimpaTList(lisPRI);
		delete lisPRI;
		lisPRI = NULL;
	}
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TSe::CodigoBarraAT(void)
{
	// assume que existe uma única barra AT
	return (nome + ".AT");
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TSe::ExisteBarra(int barra_id)
{
	// variáveis locais
	VTBarra *barra;

	//proteção
	if(barra_id == 0)
		return(NULL);
	for (int n = 0; n < lisBARRA->Count; n++)
	{
		barra = (VTBarra*)lisBARRA->Items[n];
		if (barra->Id == barra_id)
			return (barra);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TSe::ExisteBarra(double vnom_kv)
{
	// variáveis locais
	VTBarra *barra;

	for (int n = 0; n < lisBARRA->Count; n++)
	{
		barra = (VTBarra*)lisBARRA->Items[n];
		if (IsDoubleZero(barra->vnom - vnom_kv))
			return (barra);
	}
	return (NULL);
}

// -------------------------------------------------------------------------------------------------
TPri* __fastcall TSe::ExistePri(VTRede *rede)
{
	// variáveis locais
	TPri *pri;
	VTRede *rede_ext;
	for (int n = 0; n < lisPRI->Count; n++)
	{
		pri = (TPri*)lisPRI->Items[n];
		rede_ext = pri->rede;
		if (rede_ext == rede)
			return (pri);
	}
	return (NULL);
}

// -------------------------------------------------------------------------------------------------
TPri* __fastcall TSe::ExistePri(int rede_extid)
{
	// variáveis locais
	TPri *pri;
	int rede_ext_id;
	for (int n = 0; n < lisPRI->Count; n++)
	{
		pri = (TPri*)lisPRI->Items[n];
		try
		{
			rede_ext_id = StrToInt(pri->rede->Extern_id);
		}
		catch (Exception &e)
		{
			// erro na converscao
			break;
		}
		if (rede_ext_id == rede_extid)
			return (pri);
	}
	return (NULL);
}

// -------------------------------------------------------------------------------------------------
TPri* __fastcall TSe::ExistePri(AnsiString codigo)
{
	// variáveis locais
	TPri *pri;
	for (int n = 0; n < lisPRI->Count; n++)
	{
		pri = (TPri*)lisPRI->Items[n];
		if (pri->rede->Codigo.AnsiCompareIC(codigo) == 0)
			return (pri);
	}
	return (NULL);
}


// ---------------------------------------------------------------------------
VTSuprimento* __fastcall TSe::ExisteSuprimento(int id_barra_at)
//(int id_circuito)
{
	// variáveis locais
	VTBarra *barra;
	VTSuprimento *sup;
	TList *lisSUP = new TList();

   //procura o primeiro suprimento na barra AT do trafo
	   //	barra = circuito->trafo->BarraPrimario();
		barra = ExisteBarra(id_barra_at);
		lisSUP->Clear();
		barra->LisEqbar(lisSUP, eqptoSUPRIMENTO);
		if(lisSUP->Count > 0)
			{
			sup = (VTSuprimento*)lisSUP->First();
			delete lisSUP;
			return(sup);
			}
	//}
	delete lisSUP;
	return (NULL);
}

// ---------------------------------------------------------------------------
VTTrafo* __fastcall TSe::ExisteTrafo(int id)
{
	// variáveis locais
	VTTrafo *trafo;

	for (int n = 0; n < lisTRF->Count; n++)
	{
		trafo = (VTTrafo*)lisTRF->Items[n];
		if (trafo->Id == id)
			return (trafo);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTTrafo* __fastcall TSe::ExisteTrafo(double vsec_kv)
{
	// variáveis locais
	VTTrafo *trafo;

	for (int n = 0; n < lisTRF->Count; n++)
	{
		trafo = (VTTrafo*)lisTRF->Items[n];
		if (IsDoubleZero(trafo->sec.vnom - vsec_kv))
			return (trafo);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTTrafo3E* __fastcall TSe::ExisteTrafo3E(int id)
{
	// variáveis locais
	VTTrafo3E *trafo3E;

	for (int n = 0; n < lisTRF3E->Count; n++)
	{
		trafo3E = (VTTrafo3E*)lisTRF3E->Items[n];
		if (trafo3E->Id == id)
			return (trafo3E);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTTrafo3E* __fastcall TSe::ExisteTrafo3E(double vbaixa_kv)
{
	// variáveis locais
	VTTrafo3E *trafo3E;

	for (int n = 0; n < lisTRF3E->Count; n++)
	{
		trafo3E = (VTTrafo3E*)lisTRF3E->Items[n];
		if (IsDoubleZero(trafo3E->sec.vnom - vbaixa_kv))
			return (trafo3E);
		if (IsDoubleZero(trafo3E->ter.vnom - vbaixa_kv))
			return (trafo3E);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
bool __fastcall TSe::InsereBarra(VTBarra *barra)
{
	// proteção
	if (barra == NULL)
		return (false);
	// insere Barra na lista
	if (lisBARRA->IndexOf(barra) < 0)
	{
		lisBARRA->Add(barra);
	}
	return (true);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TSe::InserePri(TPri *pri)
{
	// proteção
	if (pri == NULL)
		return (false);
	// insere Prio na lista
	if (lisPRI->IndexOf(pri) < 0)
		lisPRI->Add(pri);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TSe::InsereTrafo(VTTrafo *trafo)
{
	// proteção
	if (trafo == NULL)
		return (false);
	// insere Trafo na lista
	if (lisTRF->IndexOf(trafo) < 0)
		lisTRF->Add(trafo);

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TSe::InsereTrafo3E(VTTrafo3E *trafo3E)
{
	// proteção
	if (trafo3E == NULL)
		return (false);
	// insere Trafo na lista
	if (lisTRF3E->IndexOf(trafo3E) < 0)
		lisTRF3E->Add(trafo3E);

	return (true);
}

// ---------------------------------------------------------------------------
// eof
