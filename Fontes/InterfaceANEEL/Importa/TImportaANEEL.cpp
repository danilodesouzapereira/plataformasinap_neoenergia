//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TImportaANEEL.h"
#include "VTImporta.h"
#include "VTPreMonta.h"
#include "..\Apl\VTApl.h"
#include "..\Importa\Monta\VTMonta.h"
#include "..\Log\VTLog.h"
#include "..\MontaZ\VTMontaZ.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Importa.h"
#include "..\..\DLL_Inc\MontaZ.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTImportaANEEL* NewObjImportaANEEL(VTApl *apl_owner)
   {
   return(new TImportaANEEL(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TImportaANEEL::TImportaANEEL(VTApl *apl_owner)
   {
   //salva ponteiro para objeto Apl
   this->apl_owner = apl_owner;
   //se necess�rio, cria objeto Apl
	IniciaObjAplLocal();
	//configura log
   IniciaLog();
   //inicia valor default p/ posicionamento de Eqbar nas Barras
   dxbar =  4000;
   dybar = -4000;
   }

//---------------------------------------------------------------------------
__fastcall TImportaANEEL::~TImportaANEEL(void)
   {
   //destr�i objeto Apl local, caso tenha sido criado
   if (apl != apl_owner) {delete apl; apl = NULL;}
   }

//-----------------------------------------------------------------------------
bool __fastcall TImportaANEEL::ExecutaImportacao(AnsiString filename)
	{
	//vari�veis locais
	bool       sucesso;
	VTImporta *importa = (VTImporta*)apl->GetObject(__classid(VTImporta));

	try{//executa leitura da base de dados
		sucesso = importa->ExecutaImportacao(filename);
		}catch(Exception &e)
			{
			sucesso = false;
			}
	return(sucesso);
	}

//---------------------------------------------------------------------------
void __fastcall TImportaANEEL::IniciaLog(void)
	{
	//vari�veis locais
	VTLog  *plog  = (VTLog*)apl->GetObject(__classid(VTLog));

	//inicia Log completo
	if (plog != NULL)
		{//inicia campos do Log completo
		plog->InsereCampo("Arquivo");
		plog->InsereCampo("Tabela");
		plog->InsereCampo("Registro");
		plog->InsereCampo("Rede");
      plog->InsereCampo("Eqpto");
      plog->InsereCampo("Descri��o");
      }
   }

//---------------------------------------------------------------------------
void __fastcall TImportaANEEL::IniciaObjAplLocal(void)
	{
	try{//cria objeto Apl
		apl  = NewObjApl(NULL, apl_owner);
		//cria os objetos necess�rio p/ importa��o
		apl->Add(DLL_NewObjMontaZ(apl));
		apl->Add(DLL_NewObjMontaRede(apl));
		apl->Add(NewObjPreMonta(apl));
		apl->Add(NewObjImportaMDB(apl));
		}catch(Exception &e)
         {
         apl = NULL;
         }
   }

//-----------------------------------------------------------------------------
// eof


