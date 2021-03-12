//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TImportaANEEL.h"
#include "VTExtrator.h"
#include "VTParse.h"
#include "VTPreMonta.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Importa\Monta\VTMonta.h"
#include "..\Log\VTLog.h"
#include "..\MontaZ\VTMontaZ.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Log.h"
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
   LogEnabled = false;
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
bool __fastcall TImportaANEEL::ExecutaImportacao(AnsiString arq_txt)
	{
	//vari�veis locais
	bool       sucesso;
	VTParse    *parse;
	VTExtrator *extrator;

	try{//cria objeto Apl e todos os objetos necess�rios p/ a importa��o
		//if ((apl = IniciaObjAplLocal()) == NULL) return(false);
		//configura Extrator
		extrator  = (VTExtrator*)apl->GetObject(__classid(VTExtrator));
		//obt�m objeto VTParse
		parse  = (VTParse*)apl->GetObject(__classid(VTParse));
		//executa importa��o
		sucesso = parse->Executa(arq_txt);
		//destr�i objeto Apl local
		//delete apl;
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
	VTPath *path  = (VTPath*)apl->GetObject(__classid(VTPath));

	//inicia Log completo
	if ((LogEnabled)&&(plog != NULL))
		{//abre arquivo
		plog->Open(path->DirTmp() + "\\ImportaANEEL_Detalhado.log");
		//inicia campos do Log completo
		plog->InsereCampo("Arquivo");
		plog->InsereCampo("Bloco");
		plog->InsereCampo("Linha");
		plog->InsereCampo("Rede");
      plog->InsereCampo("Eqpto");
      plog->InsereCampo("Descri��o");
      }
   }

//---------------------------------------------------------------------------
void __fastcall TImportaANEEL::IniciaObjAplLocal(void)
   {
	try{//verifica se o objeto apl j� possui os objetos necess�rios
      if (apl_owner->GetObject(__classid(VTParse)) != NULL)
         {//n�o � necess�rio cria o objeto Apl local
         apl = apl_owner;
         }
      else
			{//cria objeto Apl
			apl  = NewObjApl(NULL, apl_owner);
         //cria os objetos necess�rio p/ importa��o
			apl->Add(DLL_NewObjMontaZ(apl));
         apl->Add(NewObjMontaRede(apl));
         apl->Add(NewObjExtrator(apl));
         apl->Add(NewObjParse(apl));
         apl->Add(NewObjPreMonta(apl));
         }
      }catch(Exception &e)
         {
         apl = NULL;
         }
   }

//-----------------------------------------------------------------------------
// eof


