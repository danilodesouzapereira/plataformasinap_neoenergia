//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFormCfgEdtRamal.h"
#include "..\Geral\CfgPerdaTec\VTCfgPerdaTec.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormCfgEdtRamal *FormCfgEdtRamal;
//---------------------------------------------------------------------------
__fastcall TFormCfgEdtRamal::TFormCfgEdtRamal(TComponent* Owner)
	: TForm(Owner)
{  //variaveis locais
	int width = 0;

	//booleano pra saber se as mudanças foram salvas
	salvo = false;

	//dimensiona o stringGrid
	SGridRamal->ColWidths[ncTENSAO] = 70;
	SGridRamal->ColWidths[ncFASES] = 50;
	SGridRamal->ColWidths[ncCOMPRI] = 65;
	SGridRamal->ColWidths[ncRESIST] = 65;
	//totaliza a largura
	for(int i = 0; i < SGridRamal->ColCount ; i++)
	{
		width = width + SGridRamal->ColWidths[i];
	}
	//estipula o tamanho do form como sendo o da soma das colunas + o scroll
	this->Width = width + GetSystemMetrics(SM_CXVSCROLL);
}
//---------------------------------------------------------------------------
__fastcall TFormCfgEdtRamal::~TFormCfgEdtRamal(void)
{
	//nada a fazer
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgEdtRamal::ActionCancelaExecute(TObject *Sender)
{
	this->Close();
}

//---------------------------------------------------------------------------
void __fastcall TFormCfgEdtRamal::ActionConfirmaExecute(TObject *Sender)
{  //variaveis locais
	bool falta = false;
	TStringGrid   *SGrid = SGridRamal;
	AnsiString	  msg1, msg2;

	//mensagens de erro
	msg1 = "Existem equipamentos com parâmetros em branco.";
	msg2 = "Eles serão excluídos, deseja continuar?";

	//primeiramente verifica se possui alguma linha com paremetros faltantes
	for(int nl = 0 ; nl < SGrid->RowCount ; nl++)
	{//se faltar um parametro e NÃO for uma linha vazia
		if( FaltaParam(nl) && !IsRowEmpty(nl) ){falta = true;}
	}
	//caso falte mostra mensagem de confirmacao do usuario
	if(falta)
	{
		if( (Confirma(msg1 , msg2, MB_YESNO)) == IDYES )
		{
			SalvaCfg();
			ModalResult = mrOk;
			this->CloseModal();
		}
		else{} //nada
	}
	else
	{
		SalvaCfg();
		ModalResult = mrOk;
		this->CloseModal();
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormCfgEdtRamal::DestroiItemsList(TList *listaPerda)
{
	PERDA_RAMAL *perda_ramal;

	//destroi todos items da lista
	for(int i = (listaPerda->Count - 1) ; i >= 0 ; i--)
	{
		perda_ramal = (PERDA_RAMAL*)listaPerda->Items[i];
		listaPerda->Delete(i);
		if(perda_ramal) delete perda_ramal;
	}
}

//---------------------------------------------------------------------------
bool __fastcall TFormCfgEdtRamal::FaltaParam(int row)
{  //Variaveis locais
	bool falta = false;
	TStringGrid   *SGrid = SGridRamal;
	AnsiString	  string;

	//Percorre a linha do stringgrid verificando se falta algum parametro
	for(int nc = 0; nc < SGrid->ColCount ; nc++)
	{
		string = SGrid->Cells[nc][row];
		if( string.IsEmpty() )  falta = true;
	}

	return falta;
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgEdtRamal::FormCloseQuery(TObject *Sender, bool &CanClose)
{	//variaveis locais
	AnsiString	  msg1, msg2;

	//mensagem
	msg1 = "Todas mudanças serão descartadas.";
	msg2 = "Deseja continuar?";
	//se não foram salvas as mudanças
	if(!salvo)
	{
		if(Confirma(msg1,msg2, MB_YESNO) == IDYES) CanClose = true;
		else{CanClose = false;}
	}

}
//---------------------------------------------------------------------------
void 	__fastcall TFormCfgEdtRamal::Inicia(TList *LisPerdasRamal)
{	//variáveis locais
	PERDA_RAMAL *perda_ramal;
	TStringGrid   *SGrid = SGridRamal;

	//guarda lista
	this->LisPerdasRamal = LisPerdasRamal;
	//inicializa as dimensoes do SGridRamal
	SGrid->ColCount = ncRESIST + 1;
	SGrid->RowCount = LisPerdasRamal->Count + 2;
	//Preenche título do SGrid
	SGrid->Cells[ncTENSAO][0] = "Tensão(kV)";
	SGrid->Cells[ncFASES ][0] = "Fases";
	SGrid->Cells[ncCOMPRI][0] = "Comp.(m)";
	SGrid->Cells[ncRESIST][0] = "R(ohm/km)";
	//loop p/ todos PERDA_RAMAL em LisPerdasRamal
	for (int n = 0; n < LisPerdasRamal->Count; n++)
		{
		perda_ramal = (PERDA_RAMAL*)LisPerdasRamal->Items[n];
		SGrid->Cells[ncTENSAO][n + 1] = DoubleToStr("%4.3f", perda_ramal->vmax_kv);
		SGrid->Cells[ncFASES ][n + 1] = IntToStr(perda_ramal->num_fases);
		SGrid->Cells[ncCOMPRI][n + 1] = DoubleToStr("%2.1f", perda_ramal->comp_m);
		SGrid->Cells[ncRESIST][n + 1] = DoubleToStr("%5.4f", perda_ramal->r_ohm_km);
		}
}
//---------------------------------------------------------------------------
bool  __fastcall TFormCfgEdtRamal::IsRowEmpty(int row)
{  //Variaveis locais
	bool isRowEmpty = true;
	TStringGrid   *SGrid = SGridRamal;
	AnsiString 	  string;

	//Percorre a linha do stringGrid verificando se ela está vazia
	for(int nc = 0; nc < SGrid->ColCount ; nc++)
	{
		string = SGrid->Cells[nc][row];
		if( !string.IsEmpty() )  isRowEmpty = false;
	}

	return isRowEmpty;
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgEdtRamal::SalvaCfg(void)
{  //variaveis locais
	TStringGrid   *SGrid = SGridRamal;
	PERDA_RAMAL *perda_ramal;

	//destroi todos items da lista
	DestroiItemsList(LisPerdasRamal);
	//cria novos objetos e preenche novamente a lista
	for(int ni = 1 ; ni < SGrid->RowCount; ni++)
	{
		//verifica se a linha está vazia
		if(IsRowEmpty(ni)) continue;
		//verifica se a linha possui espaços vazios
		if(FaltaParam(ni)) continue;
		//cria novo objeto
		perda_ramal = new PERDA_RAMAL;
		/* TODO 1 -oKenndy : Verificar com o Guaraldo se existem outros parametros a serem configurados na criação. */
		perda_ramal->vmax_kv =   StrToDouble(SGrid->Cells[ncTENSAO][ni]);
		perda_ramal->num_fases = StrToInteger(SGrid->Cells[ncFASES][ni]);
		perda_ramal->comp_m = 	 StrToDouble(SGrid->Cells[ncCOMPRI][ni]);
		perda_ramal->r_ohm_km =  StrToDouble(SGrid->Cells[ncRESIST][ni]);
		//adiciona o novo objeto à lista
		LisPerdasRamal->Add(perda_ramal);
	}
	salvo = true;
}

//---------------------------------------------------------------------------
void __fastcall TFormCfgEdtRamal::SGridRamalSetEditText(TObject *Sender, int ACol,
			 int ARow, const UnicodeString Value)
{  //Variaveis locais
	bool mudou = true;
	TStringGrid   *SGrid = SGridRamal;


	if(! ValidaValor(Value, ACol)){Aviso("Valor inválido");}
	//verifica se o valor foi editado
	if(valorAntigo.CompareIC(Value) ==  0 )
	{
		mudou = false;
	}
	//se for editado e da ultima linha, uma nova linha é adicionada
	if( (ARow == (SGrid->RowCount - 1)) && mudou )
	{
		SGrid->RowCount = SGrid->RowCount + 1;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormCfgEdtRamal::SGridRamalSelectCell(TObject *Sender, int ACol,
			 int ARow, bool &CanSelect)
{  //variaveis locais
	TStringGrid   *SGrid = SGridRamal;

	//guarda o valor da célula para posterior comparação
	valorAntigo = SGrid->Cells[ACol][ARow];
}
//---------------------------------------------------------------------------
bool __fastcall TFormCfgEdtRamal::ValidaValor(AnsiString valor, int col)
{  //variaveis locais
	double 	aux;
	int		fases;
	bool 		valido;

	switch (col)
	{
		case ncTENSAO:
			valido = StrToDouble(valor, aux);
			return valido;

		case ncFASES:
			valido = StrToInteger(valor, fases);
			return valido;

		case ncCOMPRI:
			valido = StrToDouble(valor, aux);
			return valido;

		case ncRESIST:
         valido = StrToDouble(valor, aux);
			return valido;

		default:
			return false;
		;
	}
}

//---------------------------------------------------------------------------


