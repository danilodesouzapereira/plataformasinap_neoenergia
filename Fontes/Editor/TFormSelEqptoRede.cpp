//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormSelEqptoRede.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormSelEqptoRede::TFormSelEqptoRede(TComponent* Owner, VTApl *apl)
	: TForm(Owner)
	{
	//salva ponteiro dos objetos
	this->apl = apl;
	//cria objetos
	lisEQP  = new TList();
	lisREDE = new TList();
	//inicia CLBoxTipoRede
	CLBoxTipoRedeInicia();
	//determina Redes selecionadas
	DeterminaRedesSelecionadas(lisREDE);
	//inicia CLBoxTipoRede
	CLBoxRedeInicia(lisREDE);
	//inicia CLBoxEqpto
	CLBoxEqptoInicia();
   //posiciona o Form
   FormIniciaPosicao();
	}

//---------------------------------------------------------------------------
__fastcall TFormSelEqptoRede::~TFormSelEqptoRede(void)
	{//variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
	//destrói lista sem destruir seus objetos
	if (lisEQP)  {delete lisEQP;  lisEQP  = NULL;}
	if (lisREDE) {delete lisREDE; lisREDE = NULL;}
	}

//---------------------------------------------------------------------------
void __fastcall TFormSelEqptoRede::ActionCancelaExecute(TObject *Sender)
	{
	//fecha o Form
	ModalResult = mrCancel;
	}

//---------------------------------------------------------------------------
void __fastcall TFormSelEqptoRede::ActionConfirmaExecute(TObject *Sender)
	{
	//monta lisEQP com os Eqptos selecionados
	if (DeterminaEqptosSelecionados(lisEQP) == 0)
		{
		Aviso("Não há nenhum equipamento selecionado");
		return;
		}
	//fecha o Form
	ModalResult = mrOk;
	}

//---------------------------------------------------------------------------
void __fastcall TFormSelEqptoRede::ActionEqptoAllExecute(TObject *Sender)
	{
	//marca todos os Items como selecionados
	for (int nt = 0; nt < CLBoxEqpto->Count; nt++)
		{
		CLBoxEqpto->Checked[nt] = true;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormSelEqptoRede::ActionEqptoNoneExecute(TObject *Sender)
	{
	//marca todos os Items como selecionados
	for (int nt = 0; nt < CLBoxEqpto->Count; nt++)
		{
		CLBoxEqpto->Checked[nt] = false;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormSelEqptoRede::ActionRedeAllExecute(TObject *Sender)
	{
	//marca todos os Items como selecionados
	for (int nt = 0; nt < CLBoxRede->Count; nt++)
		{
		CLBoxRede->Checked[nt] = true;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormSelEqptoRede::ActionRedeNoneExecute(TObject *Sender)
	{
	//marca todos os Items como selecionados
	for (int nt = 0; nt < CLBoxRede->Count; nt++)
		{
		CLBoxRede->Checked[nt] = false;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormSelEqptoRede::ActionTipoRedeAllExecute(TObject *Sender)
	{
	//marca todos os Items como selecionados
	for (int nt = 0; nt < CLBoxTipoRede->Count; nt++)
		{
		CLBoxTipoRede->Checked[nt] = true;
		}
	//determina Redes selecionadas
	DeterminaRedesSelecionadas(lisREDE);
	//reinicia CLBoxRede
	CLBoxRedeInicia(lisREDE);
	}

//---------------------------------------------------------------------------
void __fastcall TFormSelEqptoRede::ActionTipoRedeNoneExecute(TObject *Sender)
	{
	//marca todos os Items como não selecionados
	for (int nt = 0; nt < CLBoxTipoRede->Count; nt++)
		{
		CLBoxTipoRede->Checked[nt] = false;
		}
	//não há Redes selecionadas
	lisREDE->Clear();
	//reinicia CLBoxRede
	CLBoxRedeInicia(lisREDE);
	}

//---------------------------------------------------------------------------
void __fastcall TFormSelEqptoRede::CLBoxEqptoInicia(void)
	{
	//variáveis locais
	int index;
	int vet_eqpto[] =	{eqptoBARRA,
							 eqptoCAPACITOR,
							 eqptoCAPSERIE,
							 eqptoCARGA,
							 eqptoCHAVE,
							 eqptoGERADOR,
							 eqptoFILTRO,
							 eqptoMEDIDOR,
							 eqptoMUTUA,
							 eqptoREATOR,
							 eqptoREGULADOR,
							 eqptoSUPRIMENTO,
					 	 	 eqptoTRAFO,
					 	 	 eqptoTRAFO3E,
					 	 	 eqptoTRAFOZZ,
							 eqptoTRECHO
							};
	//reinicia CheckListBox
	CLBoxEqpto->Clear();
	//preenche CheckListBox
	for (int n = 0; n < sizeof(vet_eqpto)/sizeof(int); n++)
		{
		index = CLBoxEqpto->Items->AddObject(VTEqpto::TipoEqpto_AsString(vet_eqpto[n]), (TObject*)vet_eqpto[n]);
		CLBoxEqpto->Checked[index] = false;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormSelEqptoRede::CLBoxRedeInicia(TList *lisEXT)
	{
	//variáveis locais
	int        index;
	VTRede     *rede;

	//reinicia CheckListBox
	CLBoxRede->Clear();
	for (int nr = 0; nr < lisEXT->Count; nr++)
		{
		rede = (VTRede*)lisEXT->Items[nr];
		if (! rede->Carregada) continue;
		//insere Rede no CheckListBox
		index = CLBoxRede->Items->AddObject(rede->Codigo, rede);
		CLBoxRede->Checked[index] = false;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormSelEqptoRede::CLBoxTipoRedeClickCheck(TObject *Sender)
	{
	//determina Redes selecionadas
	DeterminaRedesSelecionadas(lisREDE);
	//reinicia CLBoxRede
	CLBoxRedeInicia(lisREDE);
	}

//---------------------------------------------------------------------------
void __fastcall TFormSelEqptoRede::CLBoxTipoRedeInicia(void)
	{
	//variáveis locais
	int        index;
	VTTipoRede *tiporede;
   VTRedes    *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTTipos    *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList      *lisTIPO = tipos->LisTipoRede();

	//reinicia CheckListBox
	CLBoxTipoRede->Clear();
	//preenche CheckListBox
	for (int n = 0; n < lisTIPO->Count; n++)
		{
		tiporede = (VTTipoRede*)lisTIPO->Items[n];
		//verifica se existem Redes do TipoRede
		lisEQP->Clear();
		redes->LisRede(lisEQP, tiporede->Id);
		if (lisEQP->Count > 0)
			{//insere TipoRede no CheckListBox
			index = CLBoxTipoRede->Items->AddObject(tiporede->Codigo, tiporede);
			CLBoxTipoRede->Checked[index] = true;
			}
		}
	}

//---------------------------------------------------------------------------
int __fastcall TFormSelEqptoRede::DeterminaEqptosSelecionados(TList *lisEXT)
	{
	//variáveis locais
	int     tipo_eqpto;
	VTRede  *rede;

	//reinicia lisEXT
	lisEXT->Clear();
	//verifica os Eqptos selecionados
	for (int n = 0; n < CLBoxEqpto->Count; n++)
		{
		if (! CLBoxEqpto->Checked[n]) continue;
		//determina tipo do Eqpto
		tipo_eqpto = (int)(CLBoxEqpto->Items->Objects[n]);
		//loop p/ todas Redes selecionadas
		for (int nr = 0; nr < CLBoxRede->Count; nr++)
			{
			if (! CLBoxRede->Checked[nr]) continue;
			rede = (VTRede*)CLBoxRede->Items->Objects[nr];
			//obtém Eqptos da rede
			rede->LisEqpto(lisEXT, tipo_eqpto);
			}
		}
	return(lisEXT->Count);
	}

//---------------------------------------------------------------------------
int __fastcall TFormSelEqptoRede::DeterminaRedesSelecionadas(TList *lisEXT)
	{
	//variáveis locais
	VTRede     *rede;
	VTTipoRede *tiporede;
	VTRedes    *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//reinicia lisREDE
	lisEXT->Clear();
	//verifica os TipoRede selecionados
	for (int nt = 0; nt < CLBoxTipoRede->Count; nt++)
		{
		if (! CLBoxTipoRede->Checked[nt]) continue;
		//obtém Redes do TipoRede
		tiporede = (VTTipoRede*)CLBoxTipoRede->Items->Objects[nt];
		lisEQP->Clear();
		redes->LisRede(lisEQP, tiporede->Id);
		for (int nr = 0; nr < lisEQP->Count; nr++)
			{
			rede = (VTRede*)lisEQP->Items[nr];
			if (rede->Carregada) lisEXT->Add(rede);
			}
		}
	return(lisEXT->Count);
	}

//---------------------------------------------------------------------------
void __fastcall TFormSelEqptoRede::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      geral->Infoset->LoadFromFile(this);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TFormSelEqptoRede::LisEqpto(void)
	{
	return(lisEQP);
	}

//---------------------------------------------------------------------------
//eof

