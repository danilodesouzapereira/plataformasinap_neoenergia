//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TExportaEstudo.h"
#include "..\Apl\VTApl.h"
#include "..\Produto\VTProduto.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Registro\VTRegistro.h"
#include "..\WebService\VTWebPlan.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Registro.h"
#include "..\..\DLL_Inc\\WebService.h"
//---------------------------------------------------------------------------
VTExportaEstudo* NewObjExportaEstudo(VTApl *apl_owner)
   {
	return(new TExportaEstudo(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TExportaEstudo::TExportaEstudo(VTApl *apl_owner)
   {
   //salva ponteiros p/ objeto
   this->apl = apl_owner;
   //inicia mensagem de erro
   msg_erro = "";
   }

//---------------------------------------------------------------------------
__fastcall TExportaEstudo::~TExportaEstudo(void)
	{
	//nada a fazer
	}
/*
//---------------------------------------------------------------------------
bool __fastcall TExportaEstudo::Executa(AnsiString estudo_codigo, AnsiString arq_txt)
	{
	//vari�veis locais
	bool       sucesso;
	VTPath     *path   = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString arq_exe = path->DirBin + "\\XE\\WebServiceEstudo.exe";

	//verifica se o execut�vels est� instalado no diret�rio <dir_base>\Bin\XE
   if (! FileExists(arq_exe))
		{
		msg_erro = "O programa de comunica��o com WebService n�o est� instalado";
      return(false);
		}
	try{//executa o programa de interface com o WebService e aguarda seu t�rmino
		sucesso = ExecutaWebClient(arq_exe, estudo_codigo, arq_txt);
      if (sucesso)
         {//l� resposta do WebService gravada no registro do Windows
         LeRespostaWebClient();
			}
		else
         {
         msg_erro = "O Estudo n�o foi transferido para WebService";
         }
		}catch(Exception &e)
			{
			return(false);
			}
	return(sucesso);
	}
*/
//---------------------------------------------------------------------------
bool __fastcall TExportaEstudo::Executa(AnsiString estudo_codigo, AnsiString arq_txt)
	{
	//vari�veis locais
	bool        sucesso;
	VTProgresso *prog   = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
	VTWebPlan   *web_plan;

	try{//executa a DLL de interface com o WebService e aguarda seu t�rmino
		if (prog)
			{prog->Start(progTEXTO);}
		if (prog )
			{prog->Add("Transferindo Estudo p/ WebService"); }
		web_plan = DLL_WebService_Estudo(apl);
		if (prog)
			{prog->Add("Sinap => WebService");}
		sucesso = web_plan->EnviaEstudo(arq_txt);
		LeRespostaWebClient();
		if (prog)
			{prog->Stop();}
		//destr�i objs
		delete web_plan;
		}catch(Exception &e)
			{
			sucesso = false;
			if(web_plan) delete web_plan;
			}
	return(sucesso);
	}
/*
//---------------------------------------------------------------------------
bool __fastcall TExportaEstudo::ExecutaWebClient(AnsiString arq_exe, AnsiString estudo_codigo, AnsiString arq_txt)
	{
   //vari�veis locais
   bool                sucesso;
   TFormCreateProcess *form;
   AnsiString          cmd_line = arq_exe + " " + arq_txt;

	try{//cria TFormCreateProcess para executar o programa WebClient
		form = new TFormCreateProcess(NULL, cmd_line);
		form->Caption             = "Aguarde: Transferindo Estudo p/ WebService";
		form->GBoxCodigo->Caption = "Sinap => WebService";
		form->EditCodigo->Text    = estudo_codigo;
		sucesso = (form->ShowModal() == mrOk);
		//destr�i o form
		delete form;
		}catch(Exception &e)
			{
			sucesso = false;
			}
	return(sucesso);
	}
*/
//---------------------------------------------------------------------------
void __fastcall TExportaEstudo::LeRespostaWebClient(void)
   {
   //vari�veis locais
   bool       sucesso = false;
   AnsiString ws_status;
   VTRegistro *registro;
   VTProduto  *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

   try{//cria objeto para escrita do registro do Windows
//		if ((registro = DLL_NewObjRegistro(produto->Codigo)) != NULL)
		if ((registro = DLL_NewObjRegistro(produto->Sinapsis)) != NULL) //DVK 2015.09.28
         {
         sucesso = registro->ExisteInfo("IPE", "ws_estudo_status", ws_status);
         //destr�i objeto registro
         delete registro;
			}
      }catch(Exception &e)
         {//nada a fazer
         sucesso = false;
         }
   //verifica se recebeu resposta do WebService
   if ((sucesso) && (! ws_status.IsEmpty()))
      {
      msg_erro = ws_status;
      }
   else
      {
      msg_erro = "O WebService n�o respondeu.\nO Estudo pode n�o ter sido enviado corretamente";
      }
   }

//-----------------------------------------------------------------------------
AnsiString __fastcall TExportaEstudo::MsgErro(void)
   {
   return(msg_erro);
   }

//-----------------------------------------------------------------------------
// eof
