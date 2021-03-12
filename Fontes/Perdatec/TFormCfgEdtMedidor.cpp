//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFormCfgEdtMedidor.h"
#include "..\Geral\CfgPerdaTec\VTCfgPerdaTec.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormCfgEdtMedidor *FormCfgEdtMedidor;
//---------------------------------------------------------------------------
__fastcall TFormCfgEdtMedidor::TFormCfgEdtMedidor(TComponent* Owner)
	: TForm(Owner)
{   //variaveis locais
	int width = 0;

	//booleano pra saber se as mudanças foram salvas
	salvo = false;
	//dimensiona o stringGrid
	SGridMedidor->ColWidths[ncCODIGO] = 120;
	SGridMedidor->ColWidths[ncPERDA] = 130;
	//totaliza a largura
	for(int i = 0; i < SGridMedidor->ColCount ; i++)
	{
		width = width + SGridMedidor->ColWidths[i];
	}
	//estipula o tamano do form como sendo o da soma das colunas + o scroll
	this->Width = width + GetSystemMetrics(SM_CXVSCROLL);
}
//---------------------------------------------------------------------------
__fastcall TFormCfgEdtMedidor::~TFormCfgEdtMedidor(void)
{
//nada a fazer
}

//---------------------------------------------------------------------------
void __fastcall TFormCfgEdtMedidor::ActionCancelaExecute(TObject *Sender)
{
	this->Close();
}

//---------------------------------------------------------------------------
void __fastcall TFormCfgEdtMedidor::ActionConfirmaExecute(TObject *Sender)
{//variaveis locais
	bool falta = false;
	TStringGrid   *SGrid = SGridMedidor;
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
void __fastcall TFormCfgEdtMedidor::DestroiItemsList(TList *listaPerda)
{
	PERDA_MEDIDOR *perda_medidor;

	//destroi todos items da lista
	for(int i = (listaPerda->Count - 1) ; i >= 0 ; i--)
	{
		perda_medidor = (PERDA_MEDIDOR*)listaPerda->Items[i];
		listaPerda->Delete(i);
		if(perda_medidor) delete perda_medidor;
	}
}

//---------------------------------------------------------------------------
bool __fastcall TFormCfgEdtMedidor::FaltaParam(int row)
{  //Variaveis locais
	bool falta = false;
	TStringGrid   *SGrid = SGridMedidor;
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
void __fastcall TFormCfgEdtMedidor::FormCloseQuery(TObject *Sender, bool &CanClose)
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
void __fastcall TFormCfgEdtMedidor::Inicia(TList *LisPerdasMedidor)
{	//variáveis locais
	PERDA_MEDIDOR *perda_medidor;
	TStringGrid   *SGrid = SGridMedidor;

   //guarda lista
	this->LisPerdasMedidor = LisPerdasMedidor;
	//inicializa as dimensoes do SGridMedidor
	SGrid->ColCount = ncPERDA + 1;
	SGrid->RowCount = LisPerdasMedidor->Count + 2;
	//Preenche título do SGrid
	SGrid->Cells[ncCODIGO][0] = "Código";
	SGrid->Cells[ncPERDA ][0] = "Perda por elemento(W)";

	//loop p/ todos PERDA_RAMAL em LisPerdasRamal
	for (int n = 0; n < LisPerdasMedidor->Count; n++)
		{
		perda_medidor = (PERDA_MEDIDOR*)LisPerdasMedidor->Items[n];
		SGrid->Cells[ncCODIGO][n + 1] = perda_medidor->codigo;
		SGrid->Cells[ncPERDA ][n + 1] = DoubleToStr("%2.1f", perda_medidor->perda_w);

		}
}
//---------------------------------------------------------------------------
bool  __fastcall TFormCfgEdtMedidor::IsRowEmpty(int row)
{  //Variaveis locais
	bool isRowEmpty = true;
	TStringGrid   *SGrid = SGridMedidor;
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
void __fastcall TFormCfgEdtMedidor::SalvaCfg(void)
{  //variaveis locais
	TStringGrid   *SGrid = SGridMedidor;
	PERDA_MEDIDOR *perda_medidor;

	//destroi todos items da lista
	DestroiItemsList(LisPerdasMedidor);
	//cria novos objetos e preenche novamente a lista
	for(int ni = 1 ; ni < SGrid->RowCount; ni++)
	{
		//verifica se a linha está vazia
		if(IsRowEmpty(ni)) continue;
		//verifica se a linha possui espaços vazios
		if(FaltaParam(ni)) continue;
		//cria novo objeto
		perda_medidor = new PERDA_MEDIDOR;
		/* TODO 1 -oKenndy : Verificar com o Guaraldo se existem outros parametros a serem configurados na criação. */
		perda_medidor->codigo  =   SGrid->Cells[ncCODIGO][ni];
		perda_medidor->perda_w =   StrToDouble(SGrid->Cells[ncPERDA][ni]);
		//adiciona o novo objeto à lista
		LisPerdasMedidor->Add(perda_medidor);
	}
	salvo = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgEdtMedidor::SGridMedidorSelectCell(TObject *Sender, int ACol, int ARow,
			 bool &CanSelect)
{	//variaveis locais
	TStringGrid   *SGrid = SGridMedidor;

	//guarda o valor da célula para posterior comparação
	valorAntigo = SGrid->Cells[ACol][ARow];
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgEdtMedidor::SGridMedidorSetEditText(TObject *Sender, int ACol,
			 int ARow, const UnicodeString Value)
{	//Variaveis locais
	bool mudou = true;
	TStringGrid   *SGrid = SGridMedidor;


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
bool __fastcall TFormCfgEdtMedidor::ValidaValor(AnsiString valor, int col)
{  //variaveis locais
	double 	aux;
	int		fases;
	bool 		valido;

	switch (col)
	{
		case ncCODIGO:
			valido = true;
			return valido;

		case ncPERDA:
			valido = StrToDouble(valor, aux);
			return valido;

		default:
			return false;
		;
	}
}

//---------------------------------------------------------------------------






