//---------------------------------------------------------------------------

#pragma hdrstop
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Figura\Estilos\VTEstilos.h>
#include <PlataformaSinap\Fontes\Figura\VTFiguraComentario.h>
#include <PlataformaSinap\Fontes\Obra\VTAcao.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
//---------------------------------------------------------------------------
#include <PlataformaSinap\DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#include "VTTagFormatter.h"
#include "TTaggerChaveManobrada.h"
#include "VTAutoTag.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TTaggerChaveManobrada::TTaggerChaveManobrada(VTApl *apl, VTTagger *tagger, VTTagFormatter *formatter)
: TTaggerDecorator(apl, tagger, formatter)
{

}
//---------------------------------------------------------------------------
__fastcall TTaggerChaveManobrada::~TTaggerChaveManobrada(void)
{
	#ifdef _DEBUG
//	Application->MessageBoxW(L" TTaggerChaveManobrada destructor", L"Aviso", MB_OKCANCEL);
	#endif
}
//---------------------------------------------------------------------------
void __fastcall TTaggerChaveManobrada::AvisoEstadoChave(VTChave *chave, bool antes)
{
	AnsiString msg = "";

	msg = "Chave " + chave->Codigo;
	if (antes)
	{
		msg += " ANTES:";
	}
	else
	{
		msg += " DEPOIS:";
    }


	if (chave->Estado == chvABERTA)
	{
		msg += " ABERTA";
	}
	else if (chave->Estado == chvFECHADA)
	{
		msg += " FECHADA";
	}

    Aviso(msg);
}

//---------------------------------------------------------------------------
bool __fastcall TTaggerChaveManobrada::DoInsere(VTAcao *acao, VTChave *chave , int estado_final)
{
	bool sucesso = true;
//	AnsiString texto = "";
	VTFiguraComentario *comentario = NULL;

	try
	{
		comentario = ExisteComentario(chave);
		if (comentario != NULL)
		{
            comentario->Visible = true;
		}
		else
		{
			//insere comentario
			comentario = InsereComentario(chave);
			if (DeveDeletarComentario(comentario, TextoTag(chave)))
			{
				RemoveTag(comentario,chave);
				comentario = NULL;
			}
			//guarda no map
			mapEQPTO_COMETARIO_.insert(std::pair<VTEqpto*,VTFiguraComentario*>(chave, comentario));
			//formata de acordo   com o estado final da ação (eqpto_antes->eqpto_depois)
			switch (estado_final)
			{
				case chvABERTA:
					formatter_->FormataTagAberturaChave(comentario, chave);
					break;
				case chvFECHADA:
					formatter_->FormataTagFechamentoChave(comentario, chave);
					break;
				default:
				;
			}
		}
	}
	catch (...)
	{
		sucesso = false;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TTaggerChaveManobrada::DoRetira(VTAcao *acao, VTChave *chave , int estado_final)
{
	bool sucesso = true;
//	AnsiString texto = "";
	VTFiguraComentario *comentario = NULL;

	try
	{
		comentario = ExisteComentario(chave);
		if (DeveDeletarComentario(comentario, TextoTag(chave)))
		{
			RemoveTag(comentario,chave);
			comentario = NULL;
		}
	}
	catch (...)
	{
		sucesso = false;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TTaggerChaveManobrada::InsereTags(VTAcao *acao, bool undo)
{
	bool sucesso = true;
	VTChave *chave = NULL;

	sucesso &= TTaggerDecorator::InsereTags(acao,undo);

	if (IsAcaoTipoEqpto(acao, eqptoCHAVE))
	{
		chave = (VTChave*) acao->Eqpto();
		sucesso &= InsereTags(acao, chave, undo);
	}

	return sucesso;
}
////---------------------------------------------------------------------------
//bool __fastcall TTaggerChaveManobrada::InsereTags(VTAcao *acao, VTChave *chave, bool undo)
//{
//	bool sucesso = true;
////	AnsiString texto = "";
//	VTFiguraComentario *comentario = NULL;
//	int estado_final = 0;
//	bool deve_criar_tag = false;
//
//	try
//	{
//		//verifica se a acao trocou com o estado original
//		if (IsAcaoTrocaEstado(acao, estado_final, deve_criar_tag))
//		{
//			//SE For undo, inverte deve criar
//			if (undo)
//			{
//				deve_criar_tag = !deve_criar_tag;
//				TrocaEstado(estado_final);
//			}
//			//verifica se deve criar tag
//			if (deve_criar_tag)
//			{
//				sucesso = DoInsere(acao,chave, estado_final);
//			}
//			else
//			{
//				sucesso = UndoInsere(acao,chave, estado_final);
//			}
//		}
//		else
//		{ //faz nada pois não houve troca de estado
//
//
//		}
//	}
//	catch (...)
//	{
//		sucesso = false;
//	}
//
//	return sucesso;
//}//---------------------------------------------------------------------------
bool __fastcall TTaggerChaveManobrada::InsereTags(VTAcao *acao, VTChave *chave, bool undo)
{
	bool sucesso = true;
//	AnsiString texto = "";
	VTFiguraComentario *comentario = NULL;
	int estado_final = 0;
	int estado_final_msg = 0;
	bool deve_criar_tag = false;

	try
	{
		//verifica se a acao trocou com o estado original
		IsAcaoTrocaEstado(acao, estado_final, deve_criar_tag, undo);
		if (deve_criar_tag)
		{   // houve troca de estado, entao ou é um doInsere ou undoRetira
			if (!undo)
			{
				sucesso = DoInsere(acao,chave, estado_final);
			}
			else
			{
				sucesso = UndoRetira(acao,chave, estado_final);
			}
		}
		else
		{ //se nao houve troca, ou é um undoInsere ou um DoRetira
			if (!undo)
			{
				sucesso = DoRetira(acao,chave, estado_final);
			}
			else
			{
				sucesso = UndoInsere(acao,chave, estado_final);
			}
		}
	}
	catch (...)
	{
		sucesso = false;
	}

	return sucesso;
}

//---------------------------------------------------------------------------
bool  __fastcall TTaggerChaveManobrada::IsAcaoTrocaEstado(VTAcao *acao, int &estado_final, bool &cria_tag, bool undo)
{
	VTChave *chaveAntes = NULL;
	VTChave *chaveDepois = NULL;
	VTChave *chaveOrig = NULL;
	int diferente = false;

	bool abertoAntigo;
	bool abertoNovo;

	try
	{
		//verifica se é um eqpto chave
		if ((acao == NULL) || (acao->Eqpto() == NULL) || (acao->Eqpto()->Tipo() != eqptoCHAVE))
			{return false;}
		//guarda
		chaveOrig 	= (VTChave*)acao->Eqpto();
		if (undo)
		{
			chaveDepois	= (VTChave*)acao->CopiaDoEqptoOriginal();
			chaveAntes = (VTChave*)acao->CopiaDoEqptoAlterado();
		}
		else
		{
			chaveAntes 	= (VTChave*)acao->CopiaDoEqptoOriginal();
			chaveDepois = (VTChave*)acao->CopiaDoEqptoAlterado();
		}
		//protecao
		if ((chaveAntes == NULL) || (chaveDepois == NULL))
		{
			return false;
		}
		//compara o estado original com o alterado
		if (chaveAntes->Estado == chaveDepois->Estado)
		{
			return false;
        }
		else
		{
			diferente = true;
//			AvisoEstadoChave(chaveAntes,true);
//			AvisoEstadoChave(chaveDepois,false);
			//guarda o estado final
			estado_final = chaveDepois->Estado;
			//chave estava aberta e ficou fechada ou chave estava fechada e ficou aberta (XOR)
			abertoAntigo = chaveOrig->Status[sttCHAVE_ABERTO_ORIGINAL];
			abertoNovo = chaveDepois->Aberta;
			if ((!chaveOrig->Status[sttCHAVE_ABERTO_ORIGINAL]) != (!chaveDepois->Aberta))
			{
				cria_tag = true;
			}
			else
			{   //houve mudança de estado, mas não implica em criacao da tag
				cria_tag = false;
			}
		}

	}
	catch(...)
	{
		diferente = false;
	}

	return diferente;
}

//---------------------------------------------------------------------------
AnsiString __fastcall TTaggerChaveManobrada::TextoTag(VTChave *chave)
{	//o texto da tag só depende do estado original da chave
	AnsiString text = "";

	if (chave->Status[sttCHAVE_ABERTO_ORIGINAL])
	{
		text = formatter_->TextoTagFechamentoChave(chave);
	}
	else
	{
        text = formatter_->TextoTagAberturaChave(chave);
    }

	return text;
}
//---------------------------------------------------------------------------
void __fastcall TTaggerChaveManobrada::TrocaEstado(int &estado_final)
{
	switch (estado_final)
	{
		case chvABERTA:
			estado_final = chvFECHADA;
			break;
		case chvFECHADA:
			estado_final = chvABERTA;
			break;
    	default:
        ;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TTaggerChaveManobrada::UndoInsere(VTAcao *acao, VTChave *chave , int estado_final)
{
	bool sucesso = true;
//	AnsiString texto = "";
	VTFiguraComentario *comentario = NULL;

	try
	{
		//verifica existencia de comentario
		comentario = ExisteComentario(chave);
		if (comentario != NULL)
		{
			comentario->Visible = false;
		}
	}
	catch (...)
	{
		sucesso = false;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TTaggerChaveManobrada::UndoRetira(VTAcao *acao, VTChave *chave , int estado_final)
{
	bool sucesso = true;
//	AnsiString texto = "";
	VTFiguraComentario *comentario = NULL;
	int estado_final_msg;

	try
	{
		comentario = ExisteComentario(chave);
		if (comentario != NULL)
		{
            comentario->Visible = true;
		}
		else
		{
			//insere comentario
			comentario = InsereComentario(chave);
			if (DeveDeletarComentario(comentario, TextoTag(chave)))
			{
				RemoveTag(comentario,chave);
				comentario = NULL;
			}
			//guarda no map
			mapEQPTO_COMETARIO_.insert(std::pair<VTEqpto*,VTFiguraComentario*>(chave, comentario));
			//formata de acordo   com o estado final da ação (eqpto_antes->eqpto_depois)
			estado_final_msg = estado_final;
//			TrocaEstado(estado_final_msg);
			switch (estado_final_msg)
			{
				case chvABERTA:
					formatter_->FormataTagAberturaChave(comentario, chave);
					break;
				case chvFECHADA:
					formatter_->FormataTagFechamentoChave(comentario, chave);
					break;
				default:
				;
			}
		}
	}
	catch (...)
	{
		sucesso = false;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
