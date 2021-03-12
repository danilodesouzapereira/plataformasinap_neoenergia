//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TObra.h"
#include "VTAcao.h"
#include "VTLixeira.h"
#include "VTObras.h"
#include "TAcaoComposta.h"
#include "TDuplicacao.h"
#include "ItemObra\VTItemObra.h"
#include "ItemObra\VTItemObraARIES.h"
#include "ItemObra\VTItemObraCOPEL.h"
//---------------------------------------------------------------------------
#include "..\Apl\VTApl.h"
#include "..\Figura\VTFiguras.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\MultiObra\VTMultiObra.h"
#include "..\MultiObra\VTMultiObraBO.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTRede.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//funções globais p/ criar objetos da classe TObra
//---------------------------------------------------------------------------
VTObra* NewObjObra(VTApl *apl)
   {
   return(new TObra(apl));
   }

//---------------------------------------------------------------------------
__fastcall TObra::TObra(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //cria objeto Lixeira
   lixeira = NewObjLixeira();
   //cria listas p/ Acoes de Undo e Redo
   lisREDO = new TList();
   lisUNDO = new TList();
   //cria lista para armazenar Redes da Obra
   lisREDE = new TList();
   //cria lista para armazenar objetos externos assocaido à Obra
   lisOBJ = new TList();
   //cria lista para armazenar os ItemObra
   lisITEMOBRA = new TList();
   //inicia dados
	Id           = -1;
	Incorporada  = true;
	Visible      = true;
	Fixa         = false;
	Color        = clAqua;
	ColorEnabled = true;
   PD.ob_esp    = 0.;
   PD.part_otr  = 0.;
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15938942
__fastcall TObra::~TObra(void)
   {
   VTMultiObraBO *multiObraBO = NULL;

   //verifica se a obra eh do planejamento
   if(this->Obras != NULL)
   {
	  multiObraBO = (VTMultiObraBO*)apl->GetObject(__classid(VTMultiObraBO));
   }

   //cancela Action externas
   ActionUndo     = NULL;
   ActionRedo     = NULL;
   //destrói Eqptos associados às Acoes e à Lixeira
   Clear();
   //destrói objeto Lixeira
   if (lixeira) {delete lixeira; lixeira = NULL;}
   //destrói listas sem destruir seus objetos
   if (lisUNDO)   {delete lisUNDO;   lisUNDO   = NULL;}
   if (lisREDO)   {delete lisREDO;   lisREDO   = NULL;}
   if (lisREDE)   {delete lisREDE;   lisREDE   = NULL;}
   if (lisOBJ)    {delete lisOBJ;    lisOBJ    = NULL;}
   if (multiObraBO != NULL)
   {
	  multiObraBO->ReiniciaMultiObra(this);
   }
   //destroi o conteudo
   if (lisITEMOBRA)
	   {
		LimpaTList(lisITEMOBRA);
		DestroiTObject(lisITEMOBRA);
	   }
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16265994
void __fastcall TObra::AddAcao(VTAcao *acao)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //associa Acao a este objeto Obra
   acao->Obra = this;
   //insere Acao em lisUNDO
   acao->Index = lisUNDO->Add(acao);
   //proteção: cancela todas Acoes em lisREDO
   CancelaRedo();
   //habilita ActionUndo/ActionRedo
   HabilitaActionsUndoRedo();
   //redefine Obra associada aos Eqptos da Acao
   eqpto = acao->Eqpto();
   if ((eqpto != NULL)&&(eqpto->Obra != NULL)) eqpto->Obra = this;
   //redefine Obra associada aos Eqptos da Acao
   eqpto = acao->CopiaDoEqptoOriginal();
   if ((eqpto != NULL)&&(eqpto->Obra != NULL)) eqpto->Obra = this;
   //redefine Obra associada aos Eqptos da Acao
   eqpto = acao->CopiaDoEqptoAlterado();
   if ((eqpto != NULL)&&(eqpto->Obra != NULL)) eqpto->Obra = this;
   //indica alteração de Obras
   if (Obras != NULL) Obras->Modificada = true;
   //atualiza o multi obra
   AtualizaMultiObra();
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16265994
VTAcao* __fastcall TObra::AddAcaoAltera(VTEqpto *original, VTEqpto *alterado, VTEqpto *copia_original)
   {
   //variáveis locais
   VTAcao *acao;

   //associa novo Eqpto à este objeto Obra
   alterado->Obra = this;
   //verifica se há uma Acao anterior envolvendo o mesmo Eqpto
   if ( (Obras != NULL)&&((acao = Obras->LastAcao(acaoINDEFINIDA, original)) != NULL) )
      {//atualiza Obra da cópia do Eqpto original (pois ele foi alterado na Acao anterior)
      if (copia_original->Obra == NULL) copia_original->Obra = acao->Obra;
      }
   //cria objeto VTAcao e associa Acao a este objeto Obra
   if ((acao = NewObjAcaoAltera(original, alterado, copia_original)) == NULL) return(NULL);
   acao->Obra = this;
   //insere Acao na lista lisUNDO
   acao->Index = lisUNDO->Add(acao);
   //proteção: cancela todas Acoes em lisREDO
   CancelaRedo();
   //habilita ActionUndo/ActionRedo
   //if (ActionUndo) ActionUndo->Enabled = (lisUNDO->Count > 0);
   HabilitaActionsUndoRedo();
   //indica alteração de Obras
   if (Obras != NULL) Obras->Modificada = true;
   //atualiza o multi obra
   AtualizaMultiObra();
   return(acao);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16265994
VTAcao* __fastcall TObra::AddAcaoAltera(VTEqpto *original, VTEqpto *alterado, VTAcao *owner)
   {
   //variáveis locais
   VTAcao *acao;

   //associa novo Eqpto à este objeto Obra
   alterado->Obra = this;
   //cria objeto VTAcao e associa Acao a este objeto Obra
   if ((acao = NewObjAcaoAltera(original, alterado)) == NULL) return(NULL);
   acao->Obra = this;
   //verifica se a Acao deve ser inserida em uma Acao composta
   if (owner)
      {//insere Acao na Acao composta
      owner->Add(acao);
      }
   else
      {//insere Acao na lista lisUNDO
      acao->Index = lisUNDO->Add(acao);
      //proteção: cancela todas Acoes em lisREDO
      CancelaRedo();
      //habilita ActionUndo/ActionRedo
      //if (ActionUndo) ActionUndo->Enabled = (lisUNDO->Count > 0);
      HabilitaActionsUndoRedo();
      }
   //indica alteração de Obras
   if (Obras != NULL) Obras->Modificada = true;
   //atualiza o multi obra
   AtualizaMultiObra();
   return(acao);
   }

//---------------------------------------------------------------------------
VTAcao* __fastcall TObra::AddAcaoComposta(void)
   {
   //variáveis locais
   VTAcao *acao;

   //cria objeto VTAcao e associa Acao a este objeto Obra
   if ((acao = NewObjAcaoComposta()) == NULL) return(NULL);
   acao->Obra = this;
   //insere Acao na lista lisUNDO
   acao->Index = lisUNDO->Add(acao);
   //proteção: cancela todas Acoes em lisREDO
   CancelaRedo();
   //habilita ActionUndo
   //if (ActionUndo) ActionUndo->Enabled = (lisUNDO->Count > 0);
   HabilitaActionsUndoRedo();
   return(acao);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16265994
VTAcao* __fastcall TObra::AddAcaoInsere(VTEqpto *eqpto, VTAcao *owner)
   {
   //variáveis locais
   VTAcao *acao;

   //associa novo Eqpto à este objeto Obra
   eqpto->Obra = this;
   //cria objeto VTAcao e associa Acao a este objeto Obra
   if ((acao = NewObjAcaoInsere(eqpto)) == NULL) return(NULL);
   acao->Obra = this;
   //insere Eqpto na Lixeira p/ ele ser destruído se não estiver incluido na rede
   Lixeira->InsereEqpto(eqpto);
   //verifica se a Acao deve ser inserida em uma Acao composta
   if (owner)
      {//insere Acao na Acao composta
      owner->Add(acao);
      }
   else
      {//insere Acao na lista lisUNDO
      acao->Index = lisUNDO->Add(acao);
      //proteção: cancela todas Acoes em lisREDO
      CancelaRedo();
      //habilita ActionUndo
      //if (ActionUndo) ActionUndo->Enabled = (lisUNDO->Count > 0);
      HabilitaActionsUndoRedo();
      }
   //indica alteração de Obras
   if (Obras != NULL) Obras->Modificada = true;
   //atualiza o multi obra
   AtualizaMultiObra();
   return(acao);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16265994
VTAcao* __fastcall TObra::AddAcaoRetira(VTEqpto *eqpto, VTAcao *owner)
   {
   //variáveis locais
   VTAcao *acao;

   //NÃO associa Eqpto à este objeto Obra
   //eqpto->Obra = this;
   //cria objeto VTAcao e associa Acao a este objeto Obra
   if ((acao = NewObjAcaoRetira(eqpto)) == NULL) return(NULL);
   acao->Obra = this;
   //verifica se a Acao deve ser inserida em uma Acao composta
   if (owner)
      {//insere Acao na Acao composta
      owner->Add(acao);
      }
   else
      {//insere Acao na lista lisUNDO
      acao->Index = lisUNDO->Add(acao);
      //proteção: cancela todas Acoes em lisREDO
      CancelaRedo();
      //habilita ActionUndo
      //if (ActionUndo) ActionUndo->Enabled = (lisUNDO->Count > 0);
      HabilitaActionsUndoRedo();
      }
   //indica alteração de Obras
   if (Obras != NULL) Obras->Modificada = true;
   //atualiza o multi obra
   AtualizaMultiObra();
   return(acao);
   }
//---------------------------------------------------------------------------
VTItemObra* __fastcall TObra::AddItemObra(void)
{
	VTItemObra *itemObra;

	itemObra = CriaItemObra();
	if (!AddItemObra(itemObra))
	{
		if(itemObra)
		{
			delete itemObra;
			itemObra = NULL;
		}
	}
	return (itemObra);
}
//---------------------------------------------------------------------------
bool __fastcall TObra::AddItemObra(VTItemObra *itemObra)
{
	bool sucesso = true;

	if (itemObra != NULL)
	{
		if (lisITEMOBRA->IndexOf(itemObra) < 0)
		{
			lisITEMOBRA->Add(itemObra);
		}
		else
		{
			sucesso = false;
		}
	}
	return (sucesso);
}
//---------------------------------------------------------------------------
bool __fastcall TObra::AddRede(VTRede *rede)
{
    bool sucesso = true;

	if (rede != NULL)
	{
		if (lisREDE->IndexOf(rede) < 0)
		{
			lisREDE->Add(rede);
		}
		else
		{
			sucesso = false;
		}
	}
	return (sucesso);
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16265994
void __fastcall TObra::AtualizaMultiObra(void)
{
	VTMultiObraBO *multiObraBO = (VTMultiObraBO*)apl->GetObject(__classid(VTMultiObraBO));
	VTMultiObra *multiObra;

	if (multiObraBO != NULL)
	{
		multiObra = multiObraBO->ExisteMultiObra(this);
		if (multiObra != NULL)
		{
			multiObra->UpToDate = false;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TObra::CancelaRedo(void)
   {
   //os Eqptos das Acoes em lisREDO não fazem parte da Rede e e ja devem estar na Lixeira
   if (lisREDO->Count > 0)
      {//destrói todas Acoes em lisREDO;
      LimpaTList(lisREDO);
      //desabilita ActionRedo
      HabilitaActionsUndoRedo();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TObra::CancelaUndo(void)
   {
   //os Eqptos das Acoes em lisUNDO fazem parte da Rede e não devem ser destruídos
   if (lisUNDO->Count > 0)
      {//destrói todas Acoes em lisUNDO;
      LimpaTList(lisUNDO);
	  //desabilita ActionUndo
      HabilitaActionsUndoRedo();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TObra::Clear(void)
   {
   //cancela Undo e Redo
   CancelaRedo();
   CancelaUndo();
   //limpa lixeira
   lixeira->Clear();
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15987292
//https://sinapsisenergia.teamwork.com/#/tasks/16265994
VTObra* __fastcall TObra::Clone(TDuplicacao &duplicacao)
   {
   //variáveis locais
   TObra   *obra_clone;
   VTAcao  *acao, *acao_clone;
   VTItemObra *item, *item_clone;
   TList *lisACAOORIG;

   try{//cria novo objeto Obra
	  obra_clone = new TObra(apl);
      //copia dados e Atributos em lisOBJ
      obra_clone->PD = PD;
	  CopiaTList(lisOBJ, obra_clone->lisOBJ);
//	  CopiaTList(lisREDE, obra_clone->LisRede());
	  CopiaTListRedes(lisREDE, obra_clone->LisRede());
	  //define Actions externas
	  obra_clone->DefineActions(ActionUndo, ActionRedo);
	  //duplica Acoes em lisREDO
	  for (int n = lisREDO->Count-1; n >= 0; n--)
		 {
		 acao = (VTAcao*)lisREDO->Items[n];
		 if ((acao_clone = duplicacao.Duplica(obra_clone, acao)) == NULL)
			{
			delete obra_clone;
			return(NULL);
			}
		 //redefine Obra da Acao clone e insere em lisREDO
		 acao_clone->Obra = obra_clone;
		 obra_clone->lisREDO->Insert(0, acao_clone);
		 //atualiza dados da Acao
         acao_clone->Fixa = false;
		 //se for AcaoInsere, insere o Eqpto na Lixeira p/ ser destruído caso não seja incluido na rede
		 if (acao_clone->Tipo() == acaoINSERE)
            {
			obra_clone->Lixeira->InsereEqpto(acao_clone->Eqpto());
            }
		 }
	  //clona os items Obra
	  for (int ni = 0; ni < lisITEMOBRA->Count; ni++)
		 {
		 item = (VTItemObra*)lisITEMOBRA->Items[ni];
		 if((item_clone = item->Clone(false,true)) == NULL)
			{
			delete item_clone;
			return(NULL);
			}
		 obra_clone->AddItemObra(item_clone);
		 }
	  //reinicia a lista de redes
      obra_clone->IniciaLisRede(NULL);
	  }catch(Exception &e)
         {
         return(NULL);
         }
   return(obra_clone);
   }
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15987292
void __fastcall  TObra::CopiaTListRedes(TList *origem, TList* destino)
	{
	VTEstudo *estudo;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;

	//copia toda a lista
	CopiaTList(origem, destino);
	if (planejamento != NULL)
		{
        // obtém objeto Estudo
		estudo = planejamento->ExisteEstudo();
		if((estudo != NULL) && (estudo->Tipo != estudoREDE_ORIGINAL))
			{//remove as redes novas q foram criadas pelas obras
			for (int nr = (destino->Count - 1); nr >= 0; nr--)
				{
				rede = (VTRede*)destino->Items[nr];
//				if (rede->StatusNovo())
//					{
//					destino->Remove(rede);
//					}
				if(redes->LisRede()->IndexOf(rede) < 0)
                    {
					destino->Remove(rede);
					}
				}
            }
		}

	}
//---------------------------------------------------------------------------
VTItemObra* __fastcall TObra::CriaItemObra(void)
{
    // variaveis locais
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca *licenca = licencas->ExisteLicenca();
	VTItemObra *itemObra;

	if (licenca != NULL)
	{
		switch (licenca->Empresa)
		{
			case COPEL:
				itemObra = NewObjItemObraCOPEL(apl);
				break;
			default: //todas vai ter aries haha - temporario
				itemObra = NewObjItemObraARIES(apl);
				;
		}
	}

	return (itemObra);
}
//---------------------------------------------------------------------------
void __fastcall TObra::DefineActions(TAction *ActionUndo, TAction *ActionRedo)
   {
   //salva ponteiros p/ objetos
   this->ActionUndo = ActionUndo;
   this->ActionRedo = ActionRedo;
   //habilita/desabilita Actions
   //if (ActionUndo) ActionUndo->Enabled = (lisUNDO->Count > 0);
   //if (ActionRedo) ActionRedo->Enabled = (lisREDO->Count > 0);
   HabilitaActionsUndoRedo();
   }

//---------------------------------------------------------------------------
void __fastcall TObra::DeleteRedo(VTAcao *acao)
   {
   //proteção
   if (acao != lisREDO->Last()) return;
   //remove Acao da lista lisREDO
   lisREDO->Remove(acao);
   //destrói Acao
   delete acao;
   //habilita/desabilita ActionRedo
   //if (ActionRedo) ActionRedo->Enabled = (Id < 0)&&(lisREDO->Count > 0);
   if (ActionRedo) ActionRedo->Enabled = (lisREDO->Count > 0);
   }
//---------------------------------------------------------------------------
bool __fastcall TObra::DeleteItemObra(VTItemObra *itemObra)
{

	//proteção
	if (itemObra == NULL)
		{return false;}
	if (lisITEMOBRA->IndexOf(itemObra)<0)
	{  //item nao pertence à obra, nao deve ser deletado
		return false;
	}
	else
	{
		lisITEMOBRA->Remove(itemObra);
		delete itemObra;
	}

    return true;
}

//---------------------------------------------------------------------------
void __fastcall TObra::EliminaAcao(TList *lisACAO, VTEqpto *eqpto, int acao_tipo)
   {
   //variáveis locais
   VTAcao *acao;

   //loop p/ toda Acao em lisACAO, na ordem inversa
   for (int n = lisACAO->Count-1; n >= 0; n--)
      {
      acao = (VTAcao*)lisACAO->Items[n];
      //verifica se a Acao atende à especificação
      if (acao->Eqpto() != eqpto)     continue;
      if ((acao_tipo >= 0)&&(acao->Tipo() != acao_tipo)) continue;
      //verifica se Acao é do tipo acaoALTERA
      if (acao_tipo == acaoALTERA)
         {//copia dados do objeto original
         }
      //retira Acao da lista e destrói objeto
      lisACAO->Delete(n);
      delete acao;
      }
   }

//---------------------------------------------------------------------------
VTAcao* __fastcall TObra::ExisteAcao(TList *lisACAO, VTEqpto *eqpto, int acao_tipo)
   {
   //variáveis locais
   VTAcao *acao;

   //loop p/ toda Acao em lisACAO
   for (int n = 0; n < lisACAO->Count; n++)
      {
      acao = (VTAcao*)lisACAO->Items[n];
      //verifica se a Acao atende à especificação
      if (acao->Eqpto() != eqpto) continue;
      if ((acao_tipo >= 0)&&(acao->Tipo() != acao_tipo)) continue;
      //encontrou Acao
      return(acao);
      }
   return(NULL);
   }
//---------------------------------------------------------------------------
VTItemObra* __fastcall TObra::ExisteItemObra(int id)
{
	VTItemObra *itemObra = NULL;

	for (int ni = 0;  ni < lisITEMOBRA->Count; ni++)
	{
		itemObra = (VTItemObra*)lisITEMOBRA->Items[ni];
		if (itemObra->Id == id)
		{
			return itemObra;
		}
	}

    return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TObra::FundeAcoes(void)
   {
   //verifica se a Obra está incorporada na rede
   if (Incorporada)
      {//funde Acoes que estão em lisUNDO
      FundeAcoesUndo();
      }
   else
      {//funde Acoes que estão em lisREDO
      FundeAcoesRedo();
      }
   }
/*
//---------------------------------------------------------------------------
void __fastcall TObra::FundeAcaoPosterior(VTAcao *acao, TList *lisDEST)
   {
   //variáveis locais
   struct{
         VTAcao  *acao;
         VTEqpto *eqpto, *copia_eqpto_ori, *copia_eqpto_alt;
         }acao_ant, acao_pos;

   //inicia dados ca acao posterior
   acao_pos.acao            = acao;
   acao_pos.eqpto           = acao_pos.acao->Eqpto();
   acao_pos.copia_eqpto_ori = acao_pos.acao->CopiaDoEqptoOriginal();
   acao_pos.copia_eqpto_alt = acao_pos.acao->CopiaDoEqptoAlterado();
   //verifica o tipo da Acao
   switch(acao_pos.acao->Tipo())
      {
      case acaoALTERA:
         //verifica se existe AcaoAltera p/ o mesmo Eqpto em lisDEST
         if ((acao_ant.acao = ExisteAcao(lisDEST, acao_pos.eqpto, acaoALTERA)) != NULL)

ISSO NÃO ESTÁ BOM:
A AÇÃO POSTERIOR DEVERIA SER MANTIDA E A AÇÃO ANTERIOR DEVERIA SER ELIMINADA
         
            {//atualiza cópia do Eqpto alterado de acao_ant c/ cópia do Eqpto alterado de acao_pos
            acao_ant.eqpto           = acao_ant.acao->Eqpto();
            acao_ant.copia_eqpto_ori = acao_ant.acao->CopiaDoEqptoOriginal();
            acao_ant.copia_eqpto_alt = acao_ant.acao->CopiaDoEqptoAlterado();
            //copia copia_eqpto_alt da Acao posterior
            (*acao_ant.copia_eqpto_alt) << (*acao_pos.copia_eqpto_alt);
            //descarta Acao posterior
            delete acao_pos.acao;
            }
         else
            {//insere a Acao posterior em lisDEST
            acao_pos.acao->Index = lisDEST->Add(acao_pos.acao);
            }
         break;
      case acaoINSERE:
         //insere a Acao posterior em lisDEST
         acao_pos.acao->Index = lisDEST->Add(acao_pos.acao);
         break;
      case acaoRETIRA:
         //insere a Acao posterior em lisDEST
         acao_pos.acao->Index = lisDEST->Add(acao_pos.acao);
         break;
      }
   }
*/

//---------------------------------------------------------------------------
void __fastcall TObra::FundeAcaoPosterior(VTAcao *acao, TList *lisDEST)
   {
   //insere a Acao em lisDEST
   acao->Index = lisDEST->Add(acao);
   }

//---------------------------------------------------------------------------
void __fastcall TObra::FundeAcoesRedo(void)
   {
   //variáveis locais
   VTEqpto       *eqpto, *eqpto_alt;
   TAcaoComposta *acao_composta;
   VTAcao        *acao, *acao_pos;
   TList         *lisACAO;

   //elimina todas as Acoes em lisUNDO
   LimpaTList(lisUNDO);
   //transfere Acoes de lisREDO p/ lisUNDO, na ordem inversa, eliminando as Acoes compostas
   for (int n = lisREDO->Count-1; n >= 0; n--)
      {
      acao = (VTAcao*)lisREDO->Items[n];
      //verifica se é uma Acao composta
      if (acao->Tipo() == acaoCOMPOSTA)
         {//salva ponteiro p/ Acao composta
         acao_composta = (TAcaoComposta*)acao;
         //descarta a Acao composta e copia todas suas Acoes
         lisACAO = acao_composta->LisAcao();
         for (int i = 0; i < lisACAO->Count; i++)
            {
            acao = (VTAcao*)lisACAO->Items[i];
            //cancela aasociação da Acao c/ Acao composta
            acao->Pai = NULL;
            //funde a Acao em lisUNDO
            FundeAcaoPosterior(acao, lisUNDO);
            }
         //destrói objeto AcaoComposta mas antes limpa sua lista de Acao (sem destruir as Acoes)
         lisACAO->Clear();
         delete acao_composta;
         }
      else
         {//funde a Acao em lisUNDO
         FundeAcaoPosterior(acao, lisUNDO);
         }
      }
   //reinicia lisREDO
   lisREDO->Clear();
   //transfere as Acoes em lisUNDO p lisREDO na ordem inversa
   for (int n = lisUNDO->Count-1; n >= 0; n--)
      {
      acao = (VTAcao*)lisUNDO->Items[n];
      lisREDO->Add(acao);
      }
   //IMPORTANTE: limpa lisUNDO sem destruir os objetos
   lisUNDO->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TObra::FundeAcoesUndo(void)
   {
   //variáveis locais
   VTEqpto       *eqpto, *eqpto_ori;
   TAcaoComposta *acao_composta;
   VTAcao        *acao, *acao_ant;
   TList         *lisACAO;

   //elimina todas as Acoes em lisREDO
   LimpaTList(lisREDO);
   //transfere Acoes de lisUNDO p/ lisREDO, elimando as Acoes compostas
   for (int n = 0; n < lisUNDO->Count; n++)
      {
      acao = (VTAcao*)lisUNDO->Items[n];
      //verifica se é uma Acao composta
      if (acao->Tipo() == acaoCOMPOSTA)
         {//salva ponteiro p/ Acao composta
         acao_composta = (TAcaoComposta*)acao;
         //descarta a Acao composta e copia todas suas Acoes
         lisACAO = acao_composta->LisAcao();
         for (int i = lisACAO->Count-1; i >= 0; i--)
            {
            acao = (VTAcao*)lisACAO->Items[i];
            //cancela aasociação da Acao c/ Acao composta
            acao->Pai = NULL;
            //insere a Acao em lisREDO
            lisREDO->Add(acao);
            }
         //destrói objeto AcaoComposta mas antes limpa sua lista de Acao (sem destruir as Acoes)
         lisACAO->Clear();
         delete acao_composta;
         }
      else
         {//insere Acao em lisREDO
         lisREDO->Add(acao);
         }
      }
   //reinicia lisUNDO
   lisUNDO->Clear();
   //loop p/ todas as Acoes em lisREDO
   for (int n = 0; n < lisREDO->Count; n++)
      {
      acao = (VTAcao*)lisREDO->Items[n];
      FundeAcaoPosterior(acao, lisUNDO);
      }
   //IMPORTANTE: limpa lisREDO sem destruir os objetos
   lisREDO->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TObra::HabilitaActionsUndoRedo(void)
   {
   //variáveis locais
   VTAcao *acao;

   //habilita/desabilita ActionRedo
   if (ActionRedo) ActionRedo->Enabled = (lisREDO->Count > 0);
   //habilita/desabilita ActionUndo
   if (ActionUndo)
      {
      if (lisUNDO->Count == 0)
         {ActionUndo->Enabled = false;}
      else
         {
         acao = (VTAcao*)lisUNDO->Last();
         //verifica Acao fixa
         ActionUndo->Enabled = (! acao->Fixa);
         }
      }
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422280
//https://sinapsisenergia.teamwork.com/#tasks/16089745
void __fastcall TObra::IniciaLisRede(TList *lisEXT)
   {
   // Variáveis locais
   VTAcao    *acao;
   VTEqpto   *eqpto;
   VTLigacao *ligacao;
   VTEqbar   *eqbar;
   VTRede    *rede;
   TList     *lisTMP, *lisLIG, *lisEQBAR;
   VTFiguras  *figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));

   //reinicia lisREDE
   lisREDE->Clear();
   //verifica se foi passada uma lista
   if (lisEXT != NULL)
      {//copia redes da lista lisEXT
//      lisREDE->Clear();
      CopiaTList(lisEXT, lisREDE);
      }
   else //identifica as Redes a partir dos Eqptos das Acoes
      {//cria lista temporária
      lisTMP = new TList();
      //inicia lisTMP com todas Acoes da Obra
      CopiaTList(lisUNDO, lisTMP);
      CopiaTList(lisREDO, lisTMP);
      for (int n = 0; n < lisTMP->Count; n++)
         {
         acao = (VTAcao*)lisTMP->Items[n];
         if (acao->Tipo() == acaoCOMPOSTA)
            {//incui Acoes da Acao composta em lisTMP
            CopiaTList(acao->LisAcao(), lisTMP);
            continue;
            }
         //determina Eqpto da Acao
         if ((eqpto = acao->Eqpto()) == NULL) continue;
         //determina Rede do Eqpto
		 if (eqpto->Tipo() == eqptoREDE)
            {
            rede = (VTRede*)eqpto;
            if (lisREDE->IndexOf(rede) < 0) lisREDE->Add(rede);
			}
		 // No caso de ser um eqpto tipo EQBAR  (Conectado em uma barra)
		 else if (eqpto->TipoEqbar())
			{
				eqbar = (VTEqbar*)eqpto;
				lisLIG = ((VTBarra*)eqbar->pbarra)->LisLigacao();
                for (int nl = 0; nl < lisLIG->Count; nl++)
				   {
				   ligacao = (VTLigacao*)lisLIG->Items[nl];
				   rede = ligacao->rede;
				   if ((rede != NULL) && (lisREDE->IndexOf(rede) < 0)) lisREDE->Add(rede);
				   }
			}
		 // No caso de ser um eqto Tipo ligacao (Ligado em duas barras)
		 else if (eqpto->TipoLigacao())
            {
			rede = ((VTLigacao*)eqpto)->rede;
            if ((rede != NULL) && (lisREDE->IndexOf(rede) < 0)) lisREDE->Add(rede);
			}
         else if (eqpto->TipoBarra())
			{
			lisLIG = ((VTBarra*)eqpto)->LisLigacao();
			for (int nl = 0; nl < lisLIG->Count; nl++)
			   {
               ligacao = (VTLigacao*)lisLIG->Items[nl];
               rede = ligacao->rede;
               if ((rede != NULL) && (lisREDE->IndexOf(rede) < 0)) lisREDE->Add(rede);
			   }
			}
         }
      //destrói lista temporária sem destruir seus objetos
      delete lisTMP;
	  }
//jss20190918
	  if (figuras)
		figuras->ObraAtiva(this);
   }

//---------------------------------------------------------------------------
VTAcao* __fastcall TObra::LastAcao(void)
   {
   //verifica se há Acao na lista lisUNDO
   if (lisUNDO->Count == 0) return(NULL);
   //retorna última Acao da lista lisUNDO
   return((VTAcao*)lisUNDO->Last());
   }

//---------------------------------------------------------------------------
VTAcao* __fastcall TObra::LastAcao(int tipo, VTEqpto *eqpto)
   {
   //variáveis locais
   VTAcao *acao;
   TList  *lisACAO;

   //loop p/ toda Acao em lisUNDO
   for (int n = lisUNDO->Count-1; n >= 0; n--)
      {
      acao = (VTAcao*)lisUNDO->Items[n];
      if (acao->Tipo() == acaoCOMPOSTA)
         {//verifica Acoes da Acao composta
         lisACAO = acao->LisAcao();
         for (int n = lisACAO->Count-1; n >= 0; n--)
            {
            acao = (VTAcao*)lisACAO->Items[n];
            //verifica se a Acao atende à especificação
            if ((tipo == acaoINDEFINIDA)||(acao->Tipo() == tipo))
               {
               if (acao->Eqpto() == eqpto) return(acao);
               }
            }
			}
      else
         {//verifica se a Acao atende à especificação
         if ((tipo == acaoINDEFINIDA)||(acao->Tipo() == tipo))
            {
            if (acao->Eqpto() == eqpto) return(acao);
            }
			}
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
TList* __fastcall TObra::LisRede(void)
   {
   return(lisREDE);
   }

//---------------------------------------------------------------------------
bool __fastcall TObra::PM_GetAlteraTaxaFalha(void)
	{
	return(PD.altera_taxa_falha);
	}

//---------------------------------------------------------------------------
int __fastcall TObra::PM_GetAnoAtivacao(void)
   {
   return(YearOf(PD.data_ativacao));
   }

//---------------------------------------------------------------------------
int __fastcall TObra::PM_GetAnoInicio(void)
   {
   return(YearOf(PD.data_inicio));
   }

//---------------------------------------------------------------------------
TColor __fastcall TObra::PM_GetColor(void)
	{
	return(PD.color);
	}

//---------------------------------------------------------------------------
bool __fastcall TObra::PM_GetColorEnabled(void)
	{
	return(PD.color_enabled);
	}

//---------------------------------------------------------------------------
double __fastcall TObra::PM_GetCusto(void)
   {
   return(PD.custo);
   }

//---------------------------------------------------------------------------
TDateTime __fastcall TObra::PM_GetDataAtivacao(void)
   {
   return(PD.data_ativacao);
   }

//---------------------------------------------------------------------------
TDateTime __fastcall TObra::PM_GetDataInicio(void)
   {
   return(PD.data_inicio);
   }

//---------------------------------------------------------------------------
bool __fastcall TObra::PM_GetFixa(void)
   {
   return(PD.fixa);
   }

//---------------------------------------------------------------------------
int __fastcall TObra::PM_GetId(void)
   {
   return(PD.id);
   }

//---------------------------------------------------------------------------
bool __fastcall TObra::PM_GetIncorporada(void)
   {
   return(PD.incorporada);
   }

//---------------------------------------------------------------------------
TList* __fastcall TObra::PM_GetLisAcao(void)
   {
   return((Incorporada) ? lisUNDO : lisREDO);
   }

//---------------------------------------------------------------------------
TList* __fastcall TObra::PM_GetLisObject(void)
   {
   return(lisOBJ);
   }
//---------------------------------------------------------------------------
TList* __fastcall TObra::PM_GetLisItemObra(void)
   {
   return(lisITEMOBRA);
   }
//---------------------------------------------------------------------------
VTLixeira* __fastcall TObra::PM_GetLixeira(void)
   {
   return(lixeira);
   }

//---------------------------------------------------------------------------
int __fastcall TObra::PM_GetModalResult(void)
    {
   return(PD.modal_result);
   }
//---------------------------------------------------------------------------
AnsiString __fastcall TObra::PM_GetNome(void)
   {
   return(PD.nome);
   }

//---------------------------------------------------------------------------
double __fastcall TObra::PM_GetObEspecial(void)
   {
   return(PD.ob_esp);
   }

//---------------------------------------------------------------------------
VTObras* __fastcall TObra::PM_GetObras(void)
   {
   return(PD.obras);
   }

//---------------------------------------------------------------------------
double __fastcall TObra::PM_GetPartOtr(void)
   {
   return(PD.part_otr);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TObra::PM_GetResponsavel(void)
   {
   return(PD.responsavel);
   }

//---------------------------------------------------------------------------
double __fastcall TObra::PM_GetTaxaFalhaInsercao(void)
   {
   return(PD.taxa_falha_insercao);
   }

//---------------------------------------------------------------------------
double __fastcall TObra::PM_GetTaxaFalhaSubstituicao(void)
   {
   return(PD.taxa_falha_substituicao);
   }

//---------------------------------------------------------------------------
int __fastcall TObra::PM_GetTipo(void)
   {
   return(PD.tipo);
   }

//---------------------------------------------------------------------------
bool __fastcall TObra::PM_GetVisible(void)
   {
   return(PD.visible);
   }

//---------------------------------------------------------------------------
void __fastcall TObra::PM_SetAlteraTaxaFalha(bool altera)
	{
	PD.altera_taxa_falha = altera;
	}

//---------------------------------------------------------------------------
void __fastcall TObra::PM_SetColor(TColor color)
	{
	PD.color = color;
	}

//---------------------------------------------------------------------------
void __fastcall TObra::PM_SetColorEnabled(bool enabled)
	{
	PD.color_enabled = enabled;
	}

//---------------------------------------------------------------------------
void __fastcall TObra::PM_SetCusto(double custo)
	{
	PD.custo = custo;
	}

//---------------------------------------------------------------------------
void __fastcall TObra::PM_SetDataAtivacao(TDateTime data)
   {
   PD.data_ativacao = data;
   }

//---------------------------------------------------------------------------
void __fastcall TObra::PM_SetDataInicio(TDateTime data)
   {
   PD.data_inicio = data;
   }

//---------------------------------------------------------------------------
void __fastcall TObra::PM_SetFixa(bool fixa)
   {
   //variáveis locais
   VTAcao *acao;

   //atualiza status
   PD.fixa = fixa;
   //atualiza todas Acoes em lisUNDO
   for (int n = 0; n < lisUNDO->Count; n++)
      {
      acao = (VTAcao*)lisUNDO->Items[n];
      acao->Fixa = Fixa;
      }
   //atualiza todas Acoes em lisREDO
   for (int n = 0; n < lisREDO->Count; n++)
      {
      acao = (VTAcao*)lisREDO->Items[n];
      acao->Fixa = Fixa;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TObra::PM_SetId(int id)
    {
   PD.id = id;
   }

//---------------------------------------------------------------------------
void __fastcall TObra::PM_SetIncorporada(bool incorporada)
   {
   PD.incorporada = incorporada;
   }

//---------------------------------------------------------------------------
void __fastcall TObra::PM_SetModalResult(int modal_result)
   {
   PD.modal_result = modal_result;
   }

//---------------------------------------------------------------------------
void __fastcall TObra::PM_SetNome(AnsiString nome)
   {
   PD.nome = nome;
   }

//---------------------------------------------------------------------------
void __fastcall TObra::PM_SetObEspecial(double ob_esp)
   {
   PD.ob_esp = ob_esp;
   }

//---------------------------------------------------------------------------
void __fastcall TObra::PM_SetObras(VTObras *obras)
   {
   PD.obras = obras;
   }

//---------------------------------------------------------------------------
void __fastcall TObra::PM_SetPartOtr(double part_otr)
   {
   PD.part_otr = part_otr;
   }

//---------------------------------------------------------------------------
void __fastcall TObra::PM_SetResponsavel(AnsiString responsavel)
   {
   PD.responsavel = responsavel;
   }

//---------------------------------------------------------------------------
void __fastcall TObra::PM_SetTaxaFalhaInsercao(double taxa)
   {
   PD.taxa_falha_insercao = taxa;
   }

//---------------------------------------------------------------------------
void __fastcall TObra::PM_SetTaxaFalhaSubstituicao(double taxa)
   {
   PD.taxa_falha_substituicao = taxa;
   }

//---------------------------------------------------------------------------
void __fastcall TObra::PM_SetTipo(int tipo)
   {
   PD.tipo = tipo;
   }

//---------------------------------------------------------------------------
void __fastcall TObra::PM_SetVisible(bool visible)
   {
   PD.visible = visible;
   }

//---------------------------------------------------------------------------
VTAcao* __fastcall TObra::Redo(bool valida_acao_fixa)
   {
   //variáveis locais
   VTAcao *acao;

   //verifica se há Acao na lista lisREDO
   if (lisREDO->Count == 0) return(NULL);
   //determina última Acao da lista lisREDO
   acao = (VTAcao*)lisREDO->Last();
   //transfere última Acao da lista lisREDO p/ lisUNDO
   lisREDO->Delete(lisREDO->Count-1);
   lisUNDO->Add(acao);
   //habilita/desabilita ActionUndo e ActionRedo
   HabilitaActionsUndoRedo();
   return(acao);
   }

//---------------------------------------------------------------------------
VTAcao* __fastcall TObra::Undo(bool valida_acao_fixa)
   {
   //variáveis locais
   VTAcao *acao;

   //verifica se há Acao na lista lisUNDO
   if (lisUNDO->Count == 0) return(NULL);
   //determina última Acao da lista lisUNDO
   acao = (VTAcao*)lisUNDO->Last();
   //verifica Acao fixa
   if ((valida_acao_fixa) &&(acao->Fixa)) return(NULL);
   //transfere última Acao da lista lisUNDO p/ lisREDO
   lisUNDO->Delete(lisUNDO->Count-1);
   lisREDO->Add(acao);
   //habilita/desabilita ActionUndo e ActionRedo
   HabilitaActionsUndoRedo();
   return(acao);
   }

//---------------------------------------------------------------------------
//eof
