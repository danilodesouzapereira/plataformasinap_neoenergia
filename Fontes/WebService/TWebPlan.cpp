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
//fun��o para criar objeto VTWebPlan
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
   //destr�i objetos
//   if (apl)   {delete apl;   apl   = NULL;}
	if (lines) {delete lines; lines = NULL;}
   }
/*
//---------------------------------------------------------------------------
void __fastcall TWebPlan::Aviso(WideString txt)
   {
   //vari�veis locais
   WideString aviso = "AVISO!";

   MessageBox(NULL, txt.c_bstr(), aviso.c_bstr(), MB_TOPMOST+MB_OK+MB_ICONWARNING);
	}
*/
/*
//TESTES
//---------------------------------------------------------------------------
bool __fastcall TWebPlan::EnviaEstudo(AnsiString arq_txt)
   {
   //vari�veis locais
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
      //destr�i objetos
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
   //vari�veis locais
   bool       sucesso = true;
   AnsiString wsdl_address, service, port, ext_wsdl;

   try{
	  //inicializa o valor da mensagem no registro
      GravaRespostaNoRegistro("Estudo N�O foi enviado, contate o suporte.");
	  //inicia objeto Estudo a partir do arquivo TXT
      if (! MontaEstudoWeb(arq_txt)) return(false);
      //cria objeto ReceberEstudoWebService para acessar o WebService
      //ReceberEstudoWebService  = GetReceberEstudoWebService();
      //obt�m endere�o do WebService cadastrado no registro do Windows
      if (! LeEnderecoWebService(wsdl_address)) return(false);
      //se n�o tiver, inclui o string "?wsdl"
      ext_wsdl = "?wsdl";
      if (wsdl_address.AnsiPos(ext_wsdl) <= 0) wsdl_address += ext_wsdl;
		//extrai nome do servi�o e da porta a partir do endere�o wsdl
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
      //obt�m mensagem retornada pelo WebService
      msg_erro = resposta_web->mensagemErro;
      if (msg_erro.IsEmpty()) msg_erro = "Estudo enviado com sucesso.";
      //acrescenta identifica��o do Estudo na mensagem
		msg_erro = estudo_web->descricao + ": " + msg_erro;
		//guarda no registro a mensagem //DVK 2015.09.28
		GravaRespostaNoRegistro();
      //destr�i objetos
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
   //prote��o
   if (estudo_web == NULL) return;
   //retira objeto estudo de Apl
   apl->Remove(estudo_web);
   //destr�i estudo
   delete estudo_web;
   estudo_web = NULL;
   }
/*
//---------------------------------------------------------------------------
int __fastcall TWebPlan::ExtraiCampos(AnsiString txt, TStrings *campos)
	{
   //vari�veis locais
   TSysCharSet separator;
   TSysCharSet whitespace;
   WideString  ws_txt   = txt;

   //define conjunto de caracteres separadores de campos
	separator << '/' << '?' ;
   //define conjunto de caracteres de espa�amento
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
   //vari�veis locais
	int  ind_ini, ind_fim;
	int  len  = txt.Length();

   //reinicia StrinList campos
	campos->Clear();
	//prote��o
	if (len == 0) return(0);
	//loop de identifica��o dos campos
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
   //vari�veis locais
   bool       sucesso = false;
   VTRegistro *registro;

   try{//reinicia string com endere�o
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

         //destr�i objeto registro
         delete registro;
         }
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
bool __fastcall TWebPlan::LeEnderecoWebService(AnsiString &wsdl)
   {
   //vari�veis locais
   bool       sucesso = false;
   VTRegistro *registro;

   try{//reinicia string com endere�o
      wsdl = "";
		//cria objeto para leitura do registro do Windows
//		if ((registro = NewObjRegistro("Sinap")) != NULL)
		if ((registro = DLL_NewObjRegistro("Sinapsis")) != NULL)
         {
         sucesso = registro->ExisteInfo("IPE", "ws_estudo", wsdl);
         //destr�i objeto registro
         delete registro;
         }
      }catch(Exception &e)
         {//reinicia string com endere�o
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
   //vari�veis locais
   bool             sucesso;
   VTImportaEstudo *importa;

   try{//prote��o: destr�i objeto estudo anterior
      if (estudo_web != NULL) EstudoWebDelete();
      //cria novo estudo
      apl->Add(estudo_web = new NS_ReceberEstudoWebService::estudo());
      //cria objeto ImportaEstudoTxt p/ montar o estudo a partir do arquivo TXT
      importa = NewObjImportaEstudo(apl);
      //importa arquivo TXT
      sucesso = importa->Executa(arq_txt);
      //destr�i objetos
      delete importa;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
//eof
