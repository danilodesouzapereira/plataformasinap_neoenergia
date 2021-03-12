//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCustomImporta.h"
#include "TMedidores.h"
#include "..\ImportaMedicao\TParseCustom.h"
#include "..\..\DLL_Inc\Funcao.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Diretorio\VTPath.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormCustomImporta *FormCustomImporta;

//---------------------------------------------------------------------------
__fastcall TFormCustomImporta::TFormCustomImporta(TComponent* Owner, VTApl *apl_owner, TMedidores *medidores)
	: TForm(Owner)
{
   //salva ponteiro p/ objeto
	this->apl = apl_owner;
	mMedidores = medidores;
	Inicia();
}
//---------------------------------------------------------------------------
__fastcall TFormCustomImporta::~TFormCustomImporta(void)
	{
	//nada
	}
//---------------------------------------------------------------------------
void __fastcall TFormCustomImporta::ActionCancelExecute(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TFormCustomImporta::ActionSalvarExecute(TObject *Sender)
{
	//implementar
	bool erro = false;
	int numFases =0;
	AnsiString msgErro;

	msgErro = "Valores inválidos para as colunas:";

	try
	{  //verifica campo Nome
		if(edNome->Text.IsEmpty())
		{
			erro = true;
			msgErro = msgErro + "/nNome;";
		}
		else{strMedidor.nome = edNome->Text;}
		//verifica campo Data
		if(edData->Text.IsEmpty())
		{
			erro = true;
			msgErro = msgErro + "/nData;";
		}
		else{strMedidor.colData = StrToInt(edData->Text);}
		//verifica campo Hora
		if(edHora->Text.IsEmpty())
		{
			erro = true;
			msgErro = msgErro + "/nHora;";
		}
		else{strMedidor.colHora = StrToInt(edHora->Text);}
		//verifica campo V1
		if(edV1->Text.IsEmpty())
		{
			erro = true;
			msgErro = msgErro + "/nV1;";
		}
		else{strMedidor.colV1 = StrToInt(edV1->Text); numFases++;}
		//verifica campo V2
		if(edV2->Text.IsEmpty())
		{
			strMedidor.colV2 = 0;
		}
		else{strMedidor.colV2 = StrToInt(edV2->Text); numFases++;}
		//verifica campo V3
		if(edV3->Text.IsEmpty())
		{
			strMedidor.colV3 = 0;
		}
		else{strMedidor.colV3 = StrToInt(edV3->Text); numFases++;}
		//guarda o valor do numero de fases de acordo com o preenchimento do form
		strMedidor.numFases = numFases;

		if(!erro)
		{
			mMedidores->AddMedidorTXT(&strMedidor);
			ModalResult = mrOk;
		}
		else
		{
			Aviso(msgErro);
		}
	}catch (Exception &e)
	{
		Aviso("Valores inválidos.");
   }
}
//---------------------------------------------------------------------------
void __fastcall TFormCustomImporta::Inicia(void)
{
   //variáveis locais
   VTPath         *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //guarda pasta Importa no OpenDialog
   OpenDialogMed->InitialDir = path->DirImporta();
	//abre arquivo de medição
	OpenDialogMed->Execute();
	if(PreencheGrid(OpenDialogMed->FileName))
	{
		tbSalva->Enabled = true;
		this->Width = (sgFilePreview->ColCount+1)*(sgFilePreview->DefaultColWidth);
	}
	else{
		tbSalva->Enabled = false;
	}
	//LeArqMedicao(OpenDialogMed->FileName);
}
//---------------------------------------------------------------------------
bool __fastcall TFormCustomImporta::PreencheGrid(AnsiString filename)
{
	TParseCustom parse(apl);

	if(filename.IsEmpty()){return false;}
	else
	{
		try
		{
			parse.PreencheStringGrid(filename,sgFilePreview);
			return true;
		}
		catch (Exception &e)
		{
			return false;
		}
	}
}

//---------------------------------------------------------------------------


