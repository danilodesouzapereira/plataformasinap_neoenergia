//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TImportaPadrao.h"
#include <DateUtils.hpp>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Progresso\VTProgresso.h>
#include <Fontes\WebService\VTWebPlanPadrao.h>
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\WebService.h"
//---------------------------------------------------------------------------
VTImportaPadrao* NewObjImportaPadrao(VTApl *apl_owner)
   {
   return(new TImportaPadrao(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TImportaPadrao::TImportaPadrao(VTApl *apl_owner)
   {
   //salva ponteiros p/ objeto
   this->apl = apl_owner;
   //inicia dados
   msg_erro = "";
   }

//---------------------------------------------------------------------------
__fastcall TImportaPadrao::~TImportaPadrao(void)
   {
   //nada a fazer
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TImportaPadrao::Executa(AnsiString arq_txt)
   {
   //variáveis locais
   bool       sucesso;
   VTPath     *path   = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString arq_exe = path->DirBin + "\\XE\\WebServicePadrao.exe";

   try{//executa programa WebPlan instalado no diretório <dir_base>\Bin\XE4
      if (! FileExists(arq_exe))
         {
         msg_erro = "O programa de comunicação com o WebService não está instalado";
         return(false);
         }
      //executa o programa de interface com o WebService e aguarda seu término
      sucesso = ExecutaWebClient(arq_exe, arq_txt);
      }catch(Exception &e)
         {
         sucesso = false;
         }             
   return(sucesso);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TImportaPadrao::Executa(AnsiString arq_txt)
	{
	//variáveis locais
	bool        		sucesso;
	VTProgresso 		*prog   = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
	VTWebPlanPadrao   *web_plan;

	try{//executa o programa de interface com o WebService e aguarda seu término
//		if(! prog) prog = DLL_NewObjProgresso(NULL, NULL);
		if (prog)
			{prog->Start(progTEXTO);}
		if (prog)
			{prog->Add("Lendo Padrões de Obras do WebService");}
		web_plan = DLL_WebService_Padrao(apl);
		if (prog)
			{prog->Add("WebService => Sinap");}
		sucesso = web_plan->ImportaPadrao(arq_txt);
		if (! sucesso) msg_erro = "Os padrões não foram lidos do WebService";
		if (prog)
			{prog->Stop();}
		//destrói objs
		delete web_plan;
		}catch (Exception &e)
			{
			sucesso = false;
			if (web_plan)
				{delete web_plan;}
			}
	return(sucesso);
	}
/*
//---------------------------------------------------------------------------
bool __fastcall TImportaPadrao::ExecutaWebClient(AnsiString arq_exe, AnsiString arq_txt)
   {
   //variáveis locais
   bool                sucesso;
   TFormCreateProcess *form;
   AnsiString          cmd_line = arq_exe + " " + arq_txt;

   try{//cria TFormCreateProcess para executar o programa WebClient
      form                      = new TFormCreateProcess(NULL, cmd_line);
      form->Caption             = "Aguarde: Lendo Padrões de Obras do WebService";
		form->GBoxCodigo->Caption = "WebService => Sinap";
      form->EditCodigo->Text    = "Padrões de Obras";
      sucesso = (form->ShowModal() == mrOk);
		if (! sucesso) msg_erro = "Os padrões não foram lidos do WebService";
		//destrói o form
		delete form;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }
*/
//-----------------------------------------------------------------------------
AnsiString __fastcall TImportaPadrao::MsgErro(void)
   {
   return(msg_erro);
   }

//-----------------------------------------------------------------------------
// eof
