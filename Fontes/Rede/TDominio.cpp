//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDominio.h"
#include "VTRede.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe VTDominio
//---------------------------------------------------------------------------
VTDominio* __fastcall NewObjDominio(void)
   {
   return(new TDominio);
   }

//---------------------------------------------------------------------------
__fastcall TDominio::TDominio(void)
   {
   // inicia a data de cria��o
   data_criacao = TDateTime::CurrentDateTime();
   //cria objetos
   lisREDE = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TDominio::~TDominio(void)
   {
   //destr�i listas sem destruir seus objetos
   if (lisREDE) {delete lisREDE; lisREDE = NULL;}
   }

//---------------------------------------------------------------------------
VTDominio* __fastcall TDominio::Clone(void)
   {
   //vari�veis locais
	VTDominio *dominio;

   //cria um novo Dominio
	if ((dominio = new TDominio()) != NULL)
      {//copia dados desta Dominio p/ o novo Dominio
		dominio->CopiaAtributosDe(*this);
		}
	return(dominio);
   }

//---------------------------------------------------------------------------
void __fastcall TDominio::CopiaAtributosDe(VTEqpto &ref)
	{
	//vari�veis locais
	VTDominio &dominio = (VTDominio&)ref;

	//copia atributos de VTEqpto
	VTEqpto::CopiaAtributosDe(dominio);
	//copia atributos do Doiminio
   DataCriacao = dominio.DataCriacao;
	}

//---------------------------------------------------------------------------
bool __fastcall TDominio::ExisteRede(VTRede *rede)
   {
   return(lisREDE->IndexOf(rede) >= 0);
   }

//---------------------------------------------------------------------------
void __fastcall TDominio::InsereRede(VTRede *rede)
   {
   //prote��o
   if (rede == NULL) return;
   //verifica se a Rede j� existe na lista
   if (lisREDE->IndexOf(rede) < 0) lisREDE->Add(rede);
   }

//---------------------------------------------------------------------------
TList* __fastcall TDominio::LisRede(void)
   {
   return(lisREDE);
   }

//---------------------------------------------------------------------------
void __fastcall TDominio::LisRede(TList *lisEXT)
   {
   CopiaTList(lisREDE, lisEXT);
   }

//---------------------------------------------------------------------------
TDateTime __fastcall TDominio::PM_GetDataCriacao(void)
   {
   return(data_criacao);
   }

//---------------------------------------------------------------------------
void __fastcall TDominio::PM_SetDataCriacao(TDateTime dttDataCriacao)
   {
   data_criacao = dttDataCriacao;
   }

//---------------------------------------------------------------------------
void __fastcall TDominio::Reinicia(void)
   {
   lisREDE->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TDominio::RemoveRede(VTRede *rede)
   {
   //retira Rede da lista lisREDE
   lisREDE->Remove(rede);
   }

//---------------------------------------------------------------------------
int __fastcall TDominio::Tipo(void)
   {
   return(eqptoDOMINIO);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TDominio::TipoAsString(void)
   {
   return("Dom�nio");
   }
//---------------------------------------------------------------------------
//eof

