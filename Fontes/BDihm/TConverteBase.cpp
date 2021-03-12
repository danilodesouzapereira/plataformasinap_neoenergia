// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TConverteBase.h"
#include "VTAbre.h"
#include "..\Apl\VTApl.h"
#include "..\BD\VTBD.h"
#include "..\BD\Rede\VTBDeqpto.h"
#include "..\BD\Rede\VTBDgeral.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTRedes.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\Util\VTDesconectaVisTensaoDif.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Progresso.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TConverteBase::TConverteBase(VTApl *apl_owner)
{
	// guarda ponteiro do Apl
	this->apl = apl_owner;
}

// ---------------------------------------------------------------------------
__fastcall TConverteBase::~TConverteBase(void)
{
	// nada
}

// ---------------------------------------------------------------------------
bool __fastcall TConverteBase::Executa(void)
{
	// variáveis locais
	VTAbre *abre = (VTAbre*)apl->GetObject(__classid(VTAbre));
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
	VTProgresso *prog = DLL_NewObjProgresso(NULL, NULL);
	VTPlanejamento *planejamento;
	VTEstudo *estudo;
	TList *lisESTUDO;
	DynamicArray<int>id_estudos;
	VTDesconectaVisTensaoDif *desconecta;

	try
	{ // abre estudo original
		if(prog) prog->Start(progDEFAULT);
		abre->AvisoNumRede = 0;
		base_original = path->ArqRede;
		abre->CarregaRedeCompleta();
		//cria o objeto q separa as chaves vis de tensao diferente
		desconecta = NewObjDesconectaVisTensaoDif(apl);

		if (desconecta->Executa())
		{
			#ifdef _DEBUG
			Aviso("sucesso: Desconectou as chaves vis com tensões diferentes com sucesso");
			#endif
		}
		else
		{
			#ifdef _DEBUG
			Aviso("FALHA: Desconectar as chaves vis com tensões diferentes falhou");
			#endif
		}

		 delete desconecta;
		 desconecta = NULL;
		// salva original na base nova (convertida)
		if (!sinap_child->ActionSalvarBaseConvertida->Execute())
		{
			if(prog) prog->Stop();
			if(prog) delete prog;
			return false;
		}
		// guarda nome da base convertida
		base_convertida = path->ArqRede;
		// volta para base original
		TrocaArqRede(base_original);
		// guarda na memoria os demais estudos (se tiver)
		planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
		lisESTUDO = planejamento->LisEstudo();
		id_estudos.Length = lisESTUDO->Count;
		// guarda ID dos demais estudos
		for (int n = 0; n < lisESTUDO->Count; n++)
		{
			estudo = (VTEstudo*)lisESTUDO->Items[n];
			id_estudos[n] = estudo->Id;
		}
		//salva cada estudo na mesma base nova
		for (int n = 0; n < id_estudos.Length; n++)
		{
			TrocaEstudo(id_estudos[n]);
		}
		// volta para base nova
		TrocaArqRede(base_convertida);
		//reinicia valores default do geral
        if(geral) geral->ReiniciaValoresDefault();
		if(prog) prog->Stop();
		if(prog) delete prog;
		Aviso("Base convertida com sucesso");
	}
	catch (Exception &e)
	{
		if(prog) prog->Stop();
		if(prog) delete prog;
		if(desconecta) delete desconecta;
		return false;
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TConverteBase::TrocaArqRede(AnsiString arq_rede)
{
	// variáveis locais
	VTAbre *abre = (VTAbre*)apl->GetObject(__classid(VTAbre));
	VTBD *bd = (VTBD*)apl->GetObject(__classid(VTBD));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// volta para base determinada
	path->ArqRede = arq_rede;
	abre->IniciaConexao(); // bool
	bd->Eqpto->LeListaEstudo(); // bool
}

// ---------------------------------------------------------------------------
void __fastcall TConverteBase::TrocaEstudo(int estudo_id)
{
	// variáveis locais
	VTAbre *abre = (VTAbre*)apl->GetObject(__classid(VTAbre));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTEstudo *estudo;

	// localiza estudo pelo id
	estudo = planejamento->ExisteEstudo(estudo_id);
	if (estudo->Tipo == estudoREDE_ORIGINAL)
		return;
	// carrega da base original
	abre->CarregaEstudoExistente(estudo);
	// volta para base nova
	path->ArqRede = base_convertida;
	// salva na base nova
	sinap_child->ActionSalvarBaseConvertida->Execute();
	// volta para base original
	TrocaArqRede(base_original);
}

// ---------------------------------------------------------------------------
//eof
