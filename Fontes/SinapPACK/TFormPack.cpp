// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFormPack.h"
#include "IO\TUserPack.h"
#include "IO\TUserPlugin.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Diretorio\DirArq.h>
#include <Fontes\Diretorio\VTDir.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Licenca\VTEmpresa.h>
#include <Fontes\Produto\VTProduto.h>
#include <Fontes\Registro\VTRegistro.h>
#include <DLL_Inc\Diretorio.h>
#include <DLL_Inc\Produto.h>
#include <DLL_Inc\Registro.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPack *FormPack;

// ---------------------------------------------------------------------------
__fastcall TFormPack::TFormPack(TComponent* Owner) : TForm(Owner)
{
	// initializes the Component Object Model(COM) library
	CoInitialize(NULL);
	// cria objetos
	apl = NewObjApl(this, NULL);
	VTProduto *produto = DLL_NewObjProduto();
	apl->Add(produto);
	apl->Add(DLL_NewObjRegistro(produto->Sinapsis));
	apl->Add(DLL_NewObjPath());
	apl->Add(DLL_NewObjDir(apl));
	apl->Add(NewObjEmpresa());

}

// ---------------------------------------------------------------------------
__fastcall TFormPack::~TFormPack(void)
{
	// destroi apl
	if (apl != NULL)
	{
		delete apl;
		apl = NULL;
	}
}

// ---------------------------------------------------------------------------
bool TFormPack::EncerraSinap(void)
{
	// variaveis locais
	HWND hWnd;
	DWORD procId;
	HANDLE hProc;
	WideString sinap, vazio;

	// define parametros
	sinap = "SINAPgrid: Análise Integrada de Redes AT/MT/BT";
	vazio = "";
	// if ((hWnd = FindWindow(sinap.c_bstr(), vazio.c_bstr())) && GetWindowThreadProcessId(hWnd, &procId))
	if ((hWnd = FindWindow(vazio.c_bstr(), sinap.c_bstr())) && GetWindowThreadProcessId(hWnd,
		&procId))
	{
		hProc = OpenProcess(PROCESS_TERMINATE, FALSE, procId);
		if (hProc)
		{
			if (TerminateProcess(hProc, 0))
			{
				// MessageBox(NULL, "CLOSED", NULL, 0);
				CloseHandle(hProc);
				return true;
			}
			else
			{
				// MessageBox(NULL, "NOT CLOSED", NULL, 0);
				return false;
			}
		}
		else
		{
			// MessageBox(NULL, "I CANT CLOSE", NULL, 0);
			return false;
		}
	}
	else
	{
		// MessageBox(NULL, "THE PROCESS WAS NOT FOUND", NULL, 0);
		return false;
	}
}

// ---------------------------------------------------------------------------
void TFormPack::Executa(void)
{
	// variaveis locais
	UnicodeString diag;
	TUserPack *user;
	TUserPlugin *plugin;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	WideString wsCmd;

	// encerra o sinap
	Timer->Enabled = false;
	if (!EncerraSinap())
	{ // encerra este
		Close();
	}
	// descompacta arquivos do usuario
	RichEdit->Lines->Add("Descompactando arquivos do usuário...");
	Application->ProcessMessages();
	user = new TUserPack(apl);
	if(! user->Executa(diag))
		RichEdit->Lines->Add(diag);
	delete user;
	Application->ProcessMessages();
	// descompacta arquivos do plugin
	RichEdit->Lines->Add("Descompactando arquivos da empresa...");
	Application->ProcessMessages();
	plugin = new TUserPlugin(apl);
	if(! plugin->Executa(diag))
		RichEdit->Lines->Add(diag);
	delete plugin;
	Application->ProcessMessages();
	// executa o sinap
	wsCmd = WideString(path->DirBin + "\\SINAPgrid.exe");
	ShellExecute(NULL, NULL, wsCmd.c_bstr(), NULL, WideString(path->DirBin).c_bstr(), SW_SHOW);
	// encerra
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPack::FormShow(TObject *Sender)
{
	Application->ProcessMessages();
	RichEdit->Lines->Add("Encerrando SINAPgrid...");
	Application->ProcessMessages();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPack::TimerTimer(TObject *Sender)
{
	Executa();
}

//---------------------------------------------------------------------------

