//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TExtratorCustom.h"
#include "TParseCustom.h"
#include "..\Reclamacao\VTMedicao.h"
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
VTExtrator* __fastcall NewObjExtratorCustom(VTApl *apl)
	{
	try{
		return(new TExtratorCustom(apl));
      }catch(Exception &e)
         {
			return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TExtratorCustom::TExtratorCustom(VTApl *apl)
	{
   //salva ponteiro p/ objetos
   this->apl  = apl;
   }

//---------------------------------------------------------------------------
__fastcall TExtratorCustom::~TExtratorCustom(void)
   {
	//nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TExtratorCustom::Executa(AnsiString arqTxt, VTMedicao *medicao,
                                         strMEDIDOR *medidor)
	{
   //vari�veis locais
   bool       resp = true;
	TParseCustom parse(apl);

	try{//reinicia objeto Medicao
      medicao->Reinicia();
      //executa o processo de extra��o
		resp = parse.Executa(arqTxt, medicao, medidor);  //extrai os dados do arq.texto
      }//try
   catch (EOutOfMemory &exception)
		{
      Erro("N�o h� mem�ria dispon�vel.\nExtra��o ser� abortada!");
      resp = false;
      }
   catch (EAccessViolation   &exception)
      {
      Erro("Acesso inv�lido � mem�ria.\nExtra��o ser� abortada!");
      resp = false;
      }
   catch (Exception &exception)
      {
      Erro("Erro Irrecuper�vel.\nPrograma ser� abortado!");
      Application->Terminate();
		resp = false;
      }
	//retorna
   return(resp);
   }

//---------------------------------------------------------------------------
// eof
