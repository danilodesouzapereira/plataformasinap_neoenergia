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
   //se necessário, cria objeto Apl
   IniciaObjAplLocal();
   LogEnabled = false;
   //inicia valor default p/ posicionamento de Eqbar nas Barras
   dxbar =  4000;
   dybar = -4000;
   }

//---------------------------------------------------------------------------
__fastcall TImportaANEEL::~TImportaANEEL(void)
   {
   //destrói objeto Apl local, caso tenha sido criado
   if (apl != apl_owner) {delete apl; apl = NULL;}
   }

//-----------------------------------------------------------------------------
bool __fastcall TImportaANEEL::ExecutaImportacao(AnsiString arq_txt)
	{
	//variáveis locais
	bool       sucesso;
	VTParse    *parse;
	VTExtrator *extrator;

	try{//cria objeto Apl e todos os objetos necessários p/ a importação
		//if ((apl = IniciaObjAplLocal()) == NULL) return(false);
		//configura Extrator
		extrator  = (VTExtrator*)apl->GetObject(__classid(VTExtrator));
		//obtém objeto VTParse
		parse  = (VTParse*)apl->GetObject(__classid(VTParse));
		//executa importação
		sucesso = parse->Executa(arq_txt);
		//destrói objeto Apl local
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
	//variáveis locais
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
      plog->InsereCampo("Descrição");
      }
   }

//---------------------------------------------------------------------------
void __fastcall TImportaANEEL::IniciaObjAplLocal(void)
   {
	try{//verifica se o objeto apl já possui os objetos necessários
      if (apl_owner->GetObject(__classid(VTParse)) != NULL)
         {//não é necessário cria o objeto Apl local
         apl = apl_owner;
         }
      else
			{//cria objeto Apl
			apl  = NewObjApl(NULL, apl_owner);
         //cria os objetos necessário p/ importação
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


