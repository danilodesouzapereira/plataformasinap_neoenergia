//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormValEqpto.h"
#include "VTParametros.h"
#include "VTParametro.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
//---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormValEqpto::TFormValEqpto(TComponent* Owner, VTApl *apl_owner)
   : TForm(Owner)
	{
	//salva ponteiro
	this->apl = apl_owner;
	//cria lista
	lisTipoParam = new TList();
	//inicia
	Inicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormValEqpto::~TFormValEqpto(void)
	{
	//variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
	if (geral) geral->Infoset->SaveToFile(this);
	//destroi lista
	if(lisTipoParam) {delete lisTipoParam; lisTipoParam = NULL;}
	}

//---------------------------------------------------------------------------
void __fastcall TFormValEqpto::ActionConfirmaExecute(TObject *Sender)
   {
   //salva
   SalvaDados();
   salvou = true;
   //fecha o Form
   ModalResult = mrOk;
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormValEqpto::ActionFechaExecute(TObject *Sender)
   {
   //sair sem salvar
   salvou = false;
   //fecha o Form
   ModalResult = mrCancel;
   Close();
	}

//---------------------------------------------------------------------------
void __fastcall TFormValEqpto::CBoxInicia(void)
	{
	//limpa CBox
	CBoxTipoEqpto->Clear();
	//preenche com tipos de eqptos
	CBoxTipoEqpto->AddItem("Arranjo",NULL);
	CBoxTipoEqpto->AddItem("Cabo",NULL);
	CBoxTipoEqpto->AddItem("Capacitor",NULL);
	CBoxTipoEqpto->AddItem("Carga Grupo A",NULL);
	CBoxTipoEqpto->AddItem("Carga Grupo B",NULL);
	CBoxTipoEqpto->AddItem("Chave",NULL);
	CBoxTipoEqpto->AddItem("Gerador",NULL);
	CBoxTipoEqpto->AddItem("Regulador",NULL);
	CBoxTipoEqpto->AddItem("Suporte",NULL);
	CBoxTipoEqpto->AddItem("Suprimento",NULL);
	CBoxTipoEqpto->AddItem("Trafo SED",NULL);
	CBoxTipoEqpto->AddItem("Trafo MT/BT",NULL);
	CBoxTipoEqpto->AddItem("Trecho MT",NULL);
	CBoxTipoEqpto->AddItem("Trecho BT",NULL);
	//seleciona primeiro
	CBoxTipoEqpto->ItemIndex = 0;
	tipo_anterior = CBoxTipoEqpto->ItemIndex;
	//limpa SGrid
	LimpaSGrid();
	//insere parametros
	InsereParametros(CBoxTipoEqpto->ItemIndex);
	//redefine dimensões do form
	RedimensionaForm();
	}

//---------------------------------------------------------------------------
void __fastcall TFormValEqpto::CBoxTipoEqptoChange(TObject *Sender)
	{
	//proteção
	if(CBoxTipoEqpto->ItemIndex < 0) return;
	//guarda última alteração
	SalvaValores();
	//limpa SGrid
	cbox1->Visible = false;
	cbox2->Visible = false;
	LimpaSGrid();
	//insere parametros
	InsereParametros(CBoxTipoEqpto->ItemIndex);
	tipo_anterior = CBoxTipoEqpto->ItemIndex;
	//redefine dimensões do form
	RedimensionaForm();
	}

//---------------------------------------------------------------------------
void __fastcall TFormValEqpto::FormCloseQuery(TObject *Sender,
      bool &CanClose)
	{

   //assume que o form pode ser fechado
   if(salvou) CanClose = true;
   else
      {//pede confirmação do usuário
      switch(Confirma("Deseja salvar os parâmetros default ?", "", MB_YESNOCANCEL))
         {
         case IDYES:
            SalvaDados();
            ModalResult = mrOk;
            CanClose = true;
            break;
         case IDNO:
            CanClose = true;
            ModalResult = mrCancel;
            break;
         case IDCANCEL:
         default:
            CanClose = false;
            break;
         }
      }
	}

//---------------------------------------------------------------------------
void __fastcall TFormValEqpto::FormShow(TObject *Sender)
	{
	//variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
		{//lê parâmetros do Form
		geral->Infoset->LoadFromFile(this);
		}
	RedimensionaForm();
	}

//---------------------------------------------------------------------------
void __fastcall TFormValEqpto::Inicia(void)
	{
	//variáveis locais
	VTParametros *grupo = (VTParametros*)apl->GetObject(__classid(VTParametros));
	VTParametro 		*par;

	if(! grupo)
		{//cria grupo e adiciona no apl
		apl->Add(grupo = NewObjParametros(apl));
		}
	for(int np = 0; np < grupo->LisParametros->Count; np++)
		{//lê do infoset os valores salvos
		par = (VTParametro*)grupo->LisParametros->Items[np];
//		LeValorSalvo(par);  //lendo no VTParametros
		}
	//esconde cbox usado no SGrid
	cbox1->Visible = false;
	cbox2->Visible = false;
	//insere tipos de eqpto
	CBoxInicia();
	salvou = false;
	}

//---------------------------------------------------------------------------
void __fastcall TFormValEqpto::InsereParametros(int tipo)
	{
	//variáveis locais
	VTParametro  *par;
	VTParametros *parametros = (VTParametros*)apl->GetObject(__classid(VTParametros));

	//limpa lista
	lisTipoParam->Clear();
	parametros->Busca(tipo, lisTipoParam);
	//define quantidade de linhas conforme quantidade de parametros
	SGridParam->RowCount = lisTipoParam->Count;
	for(int np = 0; np < lisTipoParam->Count; np++)
		{//insere cada parametro no SGrid
		par = (VTParametro*)lisTipoParam->Items[np];
		InsereSGrid(par, np);
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormValEqpto::InsereSGrid(VTParametro *par, int row)
	{
	//variáveis locais
	TComboBox *cbox;

	//insere parâmetros no SGrid
	SGridParam->Cells[0][row] = par->Atributo;
	if(par->Opcoes->Count > 0)
		{//insere e posiciona cbox com opções
		if(cbox1->Visible) cbox = cbox2;
		else					 cbox = cbox1;
		cbox->Visible = true;
		cbox->Clear();
		cbox->Top = 17 + 22 * row;
		for(int no = 0; no < par->Opcoes->Count; no++)
			{
			cbox->AddItem(par->Opcoes->Strings[no], NULL);
			}
		//seleciona valor
		cbox->ItemIndex = (int)par->Valor;
		}
	else
		{
		SGridParam->Cells[1][row] = DoubleToStr(par->Formato, par->Valor, ',');
      }
	}

//---------------------------------------------------------------------------
void __fastcall TFormValEqpto::LeValorSalvo(VTParametro *par)
	{
	//variáveis locais
	VTGeral    *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	AnsiString valor;

	if(! geral) return;
	if(! par) return;
	if(! geral->Infoset->LoadFromFile("ParamImporta")) return;
	if(! geral->Infoset->GetInfo(par->TipoStr + par->Atributo, valor)) return;
	par->Valor = StrToDouble(valor);
	}

//---------------------------------------------------------------------------
void __fastcall TFormValEqpto::LimpaSGrid(void)
	{
	//percorre todas as linhas e limpa
	for(int nr = 0; nr < SGridParam->RowCount; nr++)
		{
		SGridParam->Rows[nr]->Clear();
		}
	//apaga todas as linhas
	SGridParam->RowCount = 1;
	}

//---------------------------------------------------------------------------
void __fastcall TFormValEqpto::RedimensionaForm(void)
	{
	//verifica quantidade de linhas
	this->Height = 140 + 22 * SGridParam->RowCount;
	//limita tamanho do form
	if(this->Height > 600) this->Height = 600;
	}

//---------------------------------------------------------------------------
void __fastcall TFormValEqpto::SalvaDados(void)
	{
	//variáveis locais
	AnsiString   valor;
	VTParametro  *parametro;
	VTGeral      *geral      = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTParametros *parametros = (VTParametros*)apl->GetObject(__classid(VTParametros));

	//guarda última alteração
	SalvaValores();
	if(! geral) return;
	try{//salva cada parametro no infoset
		for(int np = 0; np < parametros->LisParametros->Count; np++)
			{
			parametro = (VTParametro*)parametros->LisParametros->Items[np];
			valor     = DoubleToStr("%16.15f", parametro->Valor);
			geral->Infoset->AddInfo(parametro->TipoStr + parametro->Atributo, valor);
			}
	}catch(Exception &e)
		{
		Aviso("Erro ao salvar parâmetro " + parametro->TipoStr + parametro->Atributo);
      }
   if(! geral->Infoset->SaveToFile("ParamImporta")) return;
	}

//---------------------------------------------------------------------------
void __fastcall TFormValEqpto::SalvaValores(void)
	{
	//variáveis locais
	VTParametros *parametros = (VTParametros*)apl->GetObject(__classid(VTParametros));
	AnsiString 			atributo;
	double 				valor;

	try{//guarda somente os valores de cada par
		for(int np = 0; np < SGridParam->RowCount; np++)
			{
			atributo = SGridParam->Cells[0][np];
			//verifica se há cbox alinhados à linha
			if((cbox1->Visible) && (cbox1->Top == (17 + 22 * np)))
				{
				valor = (double)cbox1->ItemIndex;
				}
			else if((cbox2->Visible) && (cbox2->Top == (17 + 22 * np)))
				{
				valor = (double)cbox2->ItemIndex;
				}
			else valor = StrToDouble(SGridParam->Cells[1][np]);
			parametros->Atualiza(tipo_anterior, atributo, valor);
			}
	}catch(...)
		{
		Aviso("Erro no parâmetro " + atributo);
		}
	}

//---------------------------------------------------------------------------

