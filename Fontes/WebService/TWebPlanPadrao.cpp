//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TWebPlanPadrao.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Registro\VTRegistro.h>
#include "TListarPadraoConstrutivoWebService.h"
//---------------------------------------------------------------------------
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\Registro.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função para criar objeto VTWebPlanPadrao
VTWebPlanPadrao *NewObjWebPlanPadrao(VTApl *apl)
   {
	return(new TWebPlanPadrao(apl));
   }

//---------------------------------------------------------------------------
__fastcall TWebPlanPadrao::TWebPlanPadrao(VTApl *apl)
	{
	//salva ponteiro
	this->apl = apl;
	//cria objetos
	lines = new TStringList();
   }

//---------------------------------------------------------------------------
__fastcall TWebPlanPadrao::~TWebPlanPadrao(void)
   {
   //destrói objetos
   if (lines) {delete lines; lines = NULL;}
   }
/*
//---------------------------------------------------------------------------
void __fastcall TWebPlanPadrao::Aviso(WideString txt)
   {
   //variáveis locais
   WideString aviso = "AVISO!";

   MessageBox(NULL, txt.c_bstr(), aviso.c_bstr(), MB_TOPMOST+MB_OK+MB_ICONWARNING);
   }
*/
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422293
bool __fastcall TWebPlanPadrao::ImportaPadrao(AnsiString arq_txt)
   {
   //variáveis locais
   AnsiString         wsdl_address, service, port, ext_wsdl;
   AnsiString         txt;
   bool               sucesso;
   char               separador  = ';';
   int                dominio_id;
   padraoConstrutivo *padrao;
   listarResponse     vet_padrao;
   _di_ListarPadraoConstrutivoWebService  ListarPadraoConstrutivoWebService;

   try{//cria objeto ListarPadraoConstrutivoWebService para acessar o WebService
      //ListarPadraoConstrutivoWebService = GetListarPadraoConstrutivoWebService();
      //obtém domínio da Empresa
      if (! LeDominio(dominio_id)) dominio_id = 1;
      //obtém endereço do WebService cadastrado no registro do Windows
      if (! LeEnderecoWebService(wsdl_address)) return(false);
      //inclui o string "?wsdl" no endereço, se ele ainda não tiver
      ext_wsdl = "?wsdl";
      if (wsdl_address.AnsiPos(ext_wsdl) <= 0) wsdl_address += ext_wsdl;
      //extrai nome do serviço e da porta a partir do endereço wsdl
//		if (ExtraiCampos(wsdl_address, lines) < 4) return(false);
		if (ExtraiStrings(wsdl_address, "/?", lines) < 4) return(false);
//		service = lines->Strings[2];
//		port    = lines->Strings[3] + "Port";
		service = lines->Strings[3];
		port    = lines->Strings[4] + "Port";
      //obtém ponteiro para WebService
      ListarPadraoConstrutivoWebService = GetListarPadraoConstrutivoWebService(wsdl_address.c_str(), service.c_str(), port.c_str());
      //obtém dados do WebService e grava no arquivo txt
		vet_padrao = ListarPadraoConstrutivoWebService->listar(dominio_id);
      //lê padrões e salva em lines
      lines->Clear();
      for (int n = 0; n < vet_padrao.Length; n++)
         {
		 padrao = vet_padrao[n];
         //monta uma linha com dados do padrao
         txt  = IntToStr(padrao->idPadraoConstrutivo) + separador;
         txt += IntToStr(padrao->idEquipamentoSinap)  + separador;
		 txt += padrao->codigo                        + separador;
		 txt += padrao->descricao                     + separador;
		 txt += DoubleToStr("%3.2f",padrao->custo)    + separador;
         lines->Add(txt);
         }
      //salva padrões no arquivo
      lines->SaveToFile(arq_txt);
      //não destrói objeto porque ocorre Access Violation
		//delete ListarPadraoConstrutivoWebService;
      return(true);
      }catch(Exception &e)
         {
			return(false);
         }
	}
/*
//---------------------------------------------------------------------------
int __fastcall TWebPlanPadrao::ExtraiCampos(AnsiString txt, TStrings *campos)
   {
   //variáveis locais
   TSysCharSet separator;
   TSysCharSet whitespace;
   WideString  ws_txt   = txt;

   //define conjunto de caracteres separadores de campos
   separator << '/' << '?' ;
	//define conjunto de caracteres de espaçamento
   whitespace  << ' ';
	//reinicia campos e extrai os campos do string
   campos->Clear();
	ExtractStrings(separator, whitespace, ws_txt.c_bstr(), campos);
   return(campos->Count);
   }
*/ //DVK 2015.05.06
/*
//---------------------------------------------------------------------------
int __fastcall TWebPlanPadrao::ExtraiCampos(AnsiString txt, TStrings *campos)
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
//https://sinapsisenergia.teamwork.com/#tasks/14422293
//bool __fastcall TWebPlanPadrao::GravaArquivoPadrao(AnsiString arq_txt)
//   {
//   //variáveis locais
//   bool  sucesso;
//
//   try{//proteção: destrói objeto estudo anterior
//      }catch(Exception &e)
//         {
//         sucesso = false;
//         }
//   return(sucesso);
//   }

//---------------------------------------------------------------------------
bool __fastcall TWebPlanPadrao::LeDominio(int &dominio_id)
   {
   //variáveis locais
   bool       sucesso = false;
   AnsiString txt;
   VTRegistro *registro;

   try{//cria objeto para leitura do registro do Windows
//		if ((registro = NewObjRegistro("Sinap")) != NULL)
		if ((registro = DLL_NewObjRegistro("Sinapsis")) != NULL)
         {
         sucesso = registro->ExisteInfo("IPE", "dominio", txt);
         if (sucesso) dominio_id = StrToInt(txt);
         //destrói objeto registro
         delete registro;
         }
      }catch(Exception &e)
         {//nada a fazer
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TWebPlanPadrao::LeEnderecoWebService(AnsiString &wsdl)
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
         sucesso = registro->ExisteInfo("IPE", "ws_padrao", wsdl);
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
//eof
