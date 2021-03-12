//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TWebPlan.h"
#include "..\ImportaEstudo\VTImportaEstudo.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Registro\VTRegistro.h>
#include "TReceberEstudoWebService.h"
//---------------------------------------------------------------------------
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\Registro.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função para criar objeto VTWebPlan
VTWebPlan *NewObjWebPlan(VTApl *apl)
   {
	return(new TWebPlan(apl));
   }

//---------------------------------------------------------------------------
__fastcall TWebPlan::TWebPlan(VTApl *apl)
   {
   //cria objetos
//   apl   = NewObjApl(NULL, NULL);
	this->apl = apl;
	lines = new TStringList();
   }

//---------------------------------------------------------------------------
__fastcall TWebPlan::~TWebPlan(void)
   {
   //destrói objetos
//   if (apl)   {delete apl;   apl   = NULL;}
	if (lines) {delete lines; lines = NULL;}
   }
/*
//---------------------------------------------------------------------------
void __fastcall TWebPlan::Aviso(WideString txt)
   {
   //variáveis locais
   WideString aviso = "AVISO!";

   MessageBox(NULL, txt.c_bstr(), aviso.c_bstr(), MB_TOPMOST+MB_OK+MB_ICONWARNING);
	}
*/
/*
//TESTES
//---------------------------------------------------------------------------
bool __fastcall TWebPlan::EnviaEstudo(AnsiString arq_txt)
   {
   //variáveis locais
   bool       sucesso;

   try{//inicia objeto Estudo com dados para testes
      if (! EstudoWebCreate()) return(false);
      //cria objeto ReceberEstudoWebService para acessar o WebService
      ReceberEstudoWebService  = GetReceberEstudoWebService();
      //envia o objeto XMLEstudo para o ReceberEstudoWebService
      resposta_web = ReceberEstudoWebService->receber(estudo_web);
      //verifica se o envio do estudo foi correto
      if (resposta_web->recebido)
         {
         sucesso = true;
         Aviso("Estudo enviado com sucesso");
         }
      else
         {
         sucesso = false;
         Aviso(resposta_web->mensagemErro);
         }
      //destrói objetos
      EstudoWebDelete();
      if (resposta_web) {delete resposta_web; resposta_web = NULL;}
      //retorna resultado
      return(sucesso);
      }catch(Exception &e)
         {
         return(false);
         }
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TWebPlan::EnviaEstudo(AnsiString arq_txt)
   {
   //variáveis locais
   bool       sucesso = true;
   AnsiString wsdl_address, service, port, ext_wsdl;

   try{
	  //inicializa o valor da mensagem no registro
      GravaRespostaNoRegistro("Estudo NÂO foi enviado, contate o suporte.");
	  //inicia objeto Estudo a partir do arquivo TXT
      if (! MontaEstudoWeb(arq_txt)) return(false);
      //cria objeto ReceberEstudoWebService para acessar o WebService
      //ReceberEstudoWebService  = GetReceberEstudoWebService();
      //obtém endereço do WebService cadastrado no registro do Windows
      if (! LeEnderecoWebService(wsdl_address)) return(false);
      //se não tiver, inclui o string "?wsdl"
      ext_wsdl = "?wsdl";
      if (wsdl_address.AnsiPos(ext_wsdl) <= 0) wsdl_address += ext_wsdl;
		//extrai nome do serviço e da porta a partir do endereço wsdl
//		if (ExtraiCampos(wsdl_address, lines) < 4) return(false);
		if (ExtraiStrings(wsdl_address, "/?", lines) < 4) return(false);
//      service = lines->Strings[2];
//      port    = lines->Strings[3] + "Port";
		service = lines->Strings[3];
		port    = lines->Strings[4] + "Port";
		//cria objeto ReceberEstudoWebService para acessar o WebService
      //ReceberEstudoWebService = GetReceberEstudoWebService(use_wsdl, wsdl);
      ReceberEstudoWebService = GetReceberEstudoWebService(wsdl_address.c_str(), service.c_str(), port.c_str());
      //envia o objeto XMLEstudo para o ReceberEstudoWebService
      resposta_web = ReceberEstudoWebService->receber(estudo_web);
      //verifica se o envio do estudo foi correto
      sucesso = resposta_web->recebido;
      //obtém mensagem retornada pelo WebService
      msg_erro = resposta_web->mensagemErro;
      if (msg_erro.IsEmpty()) msg_erro = "Estudo enviado com sucesso.";
      //acrescenta identificação do Estudo na mensagem
		msg_erro = estudo_web->descricao + ": " + msg_erro;
		//guarda no registro a mensagem //DVK 2015.09.28
		GravaRespostaNoRegistro();
      //destrói objetos
      EstudoWebDelete();
      if (resposta_web) {delete resposta_web; resposta_web = NULL;}
      //retorna resultado
      return(sucesso);
      }catch(Exception &e)
         {
         return(false);
         }
   }

//---------------------------------------------------------------------------
void __fastcall TWebPlan::EstudoWebDelete(void)
   {
   //proteção
   if (estudo_web == NULL) return;
   //retira objeto estudo de Apl
   apl->Remove(estudo_web);
   //destrói estudo
   delete estudo_web;
   estudo_web = NULL;
   }
/*
//---------------------------------------------------------------------------
int __fastcall TWebPlan::ExtraiCampos(AnsiString txt, TStrings *campos)
	{
   //variáveis locais
   TSysCharSet separator;
   TSysCharSet whitespace;
   WideString  ws_txt   = txt;

   //define conjunto de caracteres separadores de campos
	separator << '/' << '?' ;
   //define conjunto de caracteres de espaçamento
//   whitespace  << ' ';
   //reinicia campos e extrai os campos do string
   campos->Clear();
	ExtractStrings(separator, whitespace, ws_txt.c_bstr(), campos);

for (int n = 0; n < campos->Count; n++)
{
txt = campos->Strings[n];
int a=0;
}
	return(campos->Count);
	}
*/
/*
//---------------------------------------------------------------------------
int __fastcall TWebPlan::ExtraiCampos(AnsiString txt, TStrings *campos)
	{
   //variáveis locais
	int  ind_ini, ind_fim;
	int  len  = txt.Length();

   //reinicia StrinList campos
	campos->Clear();
	//proteção
	if (len == 0) return(0);
	//loop de identificação dos campos
	ind_ini = 1;
	while (ind_ini <= len)
		{//determina fim de um campo
		ind_fim = ind_ini;
		while (ind_fim <= len)
			{//procura por delimitador
			if ((txt[ind_fim] == '/')||(txt[ind_fim] == '?')) break;
			//incrementa ind_fim
			ind_fim++;
			}
		//encontrou delimitador ou fim do string
		if (ind_fim > ind_ini)
			{//salva o campo
			campos->Add(txt.SubString(ind_ini, ind_fim - ind_ini));
			}
		//atualiza ind_ini
		ind_ini = ind_fim + 1;
		}

	return(campos->Count);
	}
*/

//---------------------------------------------------------------------------
void __fastcall TWebPlan::GravaRespostaNoRegistro(AnsiString msg)
   { //DVK 2015.09.28
   //variáveis locais
   bool       sucesso = false;
   VTRegistro *registro;

   try{//reinicia string com endereço
      //cria objeto para escrita do registro do Windows
		if ((registro = DLL_NewObjRegistro("Sinapsis")) != NULL)
         {
			if (msg.IsEmpty())
			{
				sucesso = registro->GravaInfo("IPE", "ws_estudo_status", msg_erro);
			}
			else
			{
				sucesso = registro->GravaInfo("IPE", "ws_estudo_status", msg);
            }

         //destrói objeto registro
         delete registro;
         }
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
bool __fastcall TWebPlan::LeEnderecoWebService(AnsiString &wsdl)
   {
   //variáveis locais
   bool       sucesso = false;
   VTRegistro *registro;

   try{//reinicia string com endereço
      wsdl = "";
		//cria objeto para leitura do registro do Windows
//		if ((registro = NewObjRegistro("Sinap")) != NULL)
		if ((registro = DLL_NewObjRegistro("Sinapsis")) != NULL)
         {
         sucesso = registro->ExisteInfo("IPE", "ws_estudo", wsdl);
         //destrói objeto registro
         delete registro;
         }
      }catch(Exception &e)
         {//reinicia string com endereço
			wsdl = "";
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TWebPlan::MsgErroWebService(void)
   {
   return(msg_erro);
   }

//---------------------------------------------------------------------------
bool __fastcall TWebPlan::MontaEstudoWeb(AnsiString arq_txt)
   {
   //variáveis locais
   bool             sucesso;
   VTImportaEstudo *importa;

   try{//proteção: destrói objeto estudo anterior
      if (estudo_web != NULL) EstudoWebDelete();
      //cria novo estudo
      apl->Add(estudo_web = new NS_ReceberEstudoWebService::estudo());
      //cria objeto ImportaEstudoTxt p/ montar o estudo a partir do arquivo TXT
      importa = NewObjImportaEstudo(apl);
      //importa arquivo TXT
      sucesso = importa->Executa(arq_txt);
      //destrói objetos
      delete importa;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
//eof
