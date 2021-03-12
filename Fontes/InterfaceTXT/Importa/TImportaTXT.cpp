//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TImportaTXT.h"
#include "VTExtrator.h"
#include "VTParse.h"
#include "VTPreMonta.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Importa\Monta\VTMonta.h"
#include "..\Log\VTLog.h"
#include "..\MontaZ\VTMontaZ.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Importa.h"
#include "..\..\DLL_Inc\Log.h"
#include "..\..\DLL_Inc\MontaZ.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTImportaTXT* NewObjImportaTXT(VTApl *apl_owner)
   {
   return(new TImportaTXT(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TImportaTXT::TImportaTXT(VTApl *apl_owner)
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
__fastcall TImportaTXT::~TImportaTXT(void)
   {
   //destrói objeto Apl local, caso tenha sido criado
   if (apl != apl_owner) {delete apl; apl = NULL;}
   }

//-----------------------------------------------------------------------------
bool __fastcall TImportaTXT::ExecutaImportacao(AnsiString arq_txt)
   {
   //variáveis locais
   bool       sucesso;
   VTParse    *parse;
   VTExtrator *extrator;

   try{//cria objeto Apl e todos os objetos necessários p/ a importação
      //if ((apl = IniciaObjAplLocal()) == NULL) return(false);
      //configura Extrator
      extrator  = (VTExtrator*)apl->GetObject(__classid(VTExtrator));
      extrator->ArranjoEnabled      = true;
      extrator->CurvaTipicaEnabled  = true;
      extrator->TipoChaveEnabled    = true;
      extrator->TipoRedeEnabled     = true;
      extrator->RedeEnabled         = true;
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

//-----------------------------------------------------------------------------
bool __fastcall TImportaTXT::ImportaArranjos(AnsiString arq_txt)
   {
   //variáveis locais
   bool       sucesso;
   VTParse    *parse;
   VTExtrator *extrator;

   try{//cria objeto Apl e todos os objetos necessários p/ a importação
      //if ((apl = IniciaObjAplLocal()) == NULL) return(false);
      //configura Extrator
      extrator  = (VTExtrator*)apl->GetObject(__classid(VTExtrator));
      extrator->ArranjoEnabled      = true;
      extrator->CurvaTipicaEnabled  = false;
      extrator->TipoChaveEnabled    = false;
      extrator->TipoRedeEnabled     = false;
      extrator->RedeEnabled         = false;
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

//-----------------------------------------------------------------------------
bool __fastcall TImportaTXT::ImportaConfiguracao(AnsiString arq_txt)
   {
   //variáveis locais
   bool       sucesso;
   VTParse    *parse;
   VTExtrator *extrator;

   try{//cria objeto Apl e todos os objetos necessários p/ a importação
      //if ((apl = IniciaObjAplLocal()) == NULL) return(false);
      //configura Extrator
      extrator  = (VTExtrator*)apl->GetObject(__classid(VTExtrator));
      extrator->ArranjoEnabled      = true;
      extrator->CurvaTipicaEnabled  = true;
      extrator->TipoChaveEnabled    = true;
      extrator->TipoRedeEnabled     = true;
      extrator->RedeEnabled         = false;
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

//-----------------------------------------------------------------------------
bool __fastcall TImportaTXT::ImportaCurvasTipicas(AnsiString arq_txt)
   {
   //variáveis locais
   bool       sucesso;
   VTParse    *parse;
   VTExtrator *extrator;

   try{//cria objeto Apl e todos os objetos necessários p/ a importação
      //if ((apl = IniciaObjAplLocal()) == NULL) return(false);
      //configura Extrator
      extrator  = (VTExtrator*)apl->GetObject(__classid(VTExtrator));
      extrator->ArranjoEnabled      = false;
      extrator->CurvaTipicaEnabled  = true;
      extrator->TipoChaveEnabled    = false;
      extrator->TipoRedeEnabled     = false;
      extrator->RedeEnabled         = false;
      //obtém objeto VTparse
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

//-----------------------------------------------------------------------------
bool __fastcall TImportaTXT::ImportaRedes(AnsiString arq_txt)
   {
   //variáveis locais
   bool       sucesso;
   VTParse    *parse;
   VTExtrator *extrator;

   try{//cria objeto Apl e todos os objetos necessários p/ a importação
      //if ((apl = IniciaObjAplLocal()) == NULL) return(false);
      //configura Extrator
      extrator  = (VTExtrator*)apl->GetObject(__classid(VTExtrator));
      extrator->ArranjoEnabled      = false;
      extrator->CurvaTipicaEnabled  = false;
      extrator->TipoChaveEnabled    = false;
      extrator->TipoRedeEnabled     = false;
      extrator->RedeEnabled         = true;
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
void __fastcall TImportaTXT::IniciaLog(void)
   {
   //variáveis locais
   VTPath *path  = (VTPath*)apl->GetObject(__classid(VTPath));

   //inicia Log completo
   if ((LogEnabled)&&(plog != NULL))
      {//abre arquivo
      plog->Open(path->DirTmp() + "\\ImportaTXT_Detalhado.log");
      //inicia campos do Log completo
      plog->InsereCampo("Arquivo");
      plog->InsereCampo("Bloco");
      plog->InsereCampo("Linha");
      plog->InsereCampo("Rede");
		plog->InsereCampo("Eqpto");
		plog->InsereCampo("Código");  //DVK 2015.10.22
		plog->InsereCampo("Descrição");
		}
	}
/*
//---------------------------------------------------------------------------
void __fastcall TImportaTXT::IniciaObjAplLocal(void)
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
			apl->Add(DLL_NewObjMontaRede(apl));
			apl->Add(NewObjExtrator(apl));
			apl->Add(NewObjParse(apl));
			apl->Add(NewObjPreMonta(apl));
			}
		}catch(Exception &e)
			{
			apl = NULL;
			}
	}
*/ //DVK 2015.03.05 reescrito
/*
//---------------------------------------------------------------------------
void __fastcall TImportaTXT::IniciaObjAplLocal(void)
   {
	try{//cria objeto Apl
		apl  = NewObjApl(NULL, apl_owner);
		//cria os objetos necessário p/ importação
		apl->Add(DLL_NewObjMontaZ(apl));
		apl->Add(DLL_NewObjMontaRede(apl));
      apl->Add(NewObjExtrator(apl));
      apl->Add(NewObjParse(apl));
      apl->Add(NewObjPreMonta(apl));
		}catch(Exception &e)
         {
         apl = NULL;
         }
   }
*/ //DVK 2015.10.22 reescrito
//---------------------------------------------------------------------------
void __fastcall TImportaTXT::IniciaObjAplLocal(void)
	{
	//variaveis locais
	VTExtrator  *extrator;
	VTMontaZ    *montaZ;
	VTMonta     *monta_rede;
	VTParse     *parse;
	VTPreMonta  *pre_monta;

	try{//verifica se o objeto apl já possui os objetos necessários
		apl  = NewObjApl(NULL, apl_owner);
		if (apl_owner->GetObject(__classid(VTMontaZ)) == NULL)
			{//cria monta z
			apl->Add(DLL_NewObjMontaZ(apl));
			}
		if (apl_owner->GetObject(__classid(VTMonta)) == NULL)
			{//cria monta rede
			apl->Add(DLL_NewObjMontaRede(apl));
			}
		if (apl_owner->GetObject(__classid(VTExtrator)) == NULL)
			{//cria extrator
			apl->Add(NewObjExtrator(apl));
			}
		if (apl_owner->GetObject(__classid(VTParse)) == NULL)
			{//cria parse
			apl->Add(NewObjParse(apl));
			}
		if (apl_owner->GetObject(__classid(VTPreMonta)) == NULL)
			{//cria pre monta
			apl->Add(NewObjPreMonta(apl));
			}
		}catch(Exception &e)
			{
			apl = NULL;
			}
	}

//-----------------------------------------------------------------------------
// eof

