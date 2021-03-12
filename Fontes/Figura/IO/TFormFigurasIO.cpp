//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "..\Apl\VTApl.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "VTFigurasIO.h"
#include "TFormFigurasIO.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormFigurasIO *FormFigurasIO;
//---------------------------------------------------------------------------
__fastcall TFormFigurasIO::TFormFigurasIO(TComponent* Owner, VTApl *apl)
	: TForm(Owner)
{
	this->apl = apl;
	figuraIO = NewObjFigurasIO(apl);
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));

	if (planejamento != NULL)
	{
		estudo = planejamento->ExisteEstudo();
	}
	else
	{
		estudo = NULL;
	}
}
//---------------------------------------------------------------------------
__fastcall  TFormFigurasIO::~TFormFigurasIO(void)
{
	if (figuraIO)
	{
		delete figuraIO;
        figuraIO = NULL;
    }
}
//---------------------------------------------------------------------------
bool __fastcall TFormFigurasIO::ValidaDiretorio(AnsiString filepath)
{
	AnsiString diretorio;

	if (filepath.IsEmpty())
	{
		return false;
	}
	diretorio = ExtractFileDir(filepath);
	//verifica diretorio
	if (!DirectoryExists(diretorio))
	{
		return false;
	}

	if (!FileExists(filepath))
	{
		return false;
	}


	return true;
}
//---------------------------------------------------------------------------
void __fastcall TFormFigurasIO::Button3Click(TObject *Sender)
{
	if (FileOpenDialog->Execute())
	{
		filenameImp =  FileOpenDialog->FileName;

	}
	else
	{
		filenameImp = "";
	}

	EditFilename->Text = filenameImp;
}
//---------------------------------------------------------------------------
void __fastcall TFormFigurasIO::Button2Click(TObject *Sender)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	if (ValidaDiretorio(filenameImp))
	{
		figuraIO->ImportaTXT(estudo,filenameImp);
		filenameImp = filenameImp;
		EditFilename->Text = filenameImp;
        grafico->Redraw();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFigurasIO::Button1Click(TObject *Sender)
{


	if (SaveDialog->Execute())
	{
		figuraIO->ExportaTXT(estudo,SaveDialog->FileName);
	}
}
//---------------------------------------------------------------------------
