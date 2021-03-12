//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TFluxoCC.h"
#include ".\VTFPcc.h"
#include ".\VTData.h"
#include ".\VTMontaData.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Patamar\VTPatamares.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Rede\VTNet.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
using namespace std;

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTFluxo
//---------------------------------------------------------------------------
VTFluxo* __fastcall NewObjFluxoCC(VTApl *apl_owner)
   {
   return(new TFluxoCC(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TFluxoCC::TFluxoCC(VTApl *apl_owner)
   {
   //salva objetos
   this->apl = apl_owner;

   //cria objetos
   data  = NewObjData(apl);
   monta = NewObjMontaData(apl);
   fpcc  = NewObjFPcc(apl);

   // Inicialização
   NomeRede = AnsiString("");
   }

//---------------------------------------------------------------------------
__fastcall TFluxoCC::~TFluxoCC(void)
   {
   //destrói objetos
   if (data)  {delete data;  data  = NULL;}
   if (monta) {delete monta; monta = NULL;}
   if (fpcc)  {delete fpcc;  fpcc  = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoCC::CfgAvisoErro(bool enabled)
   {
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoCC::Executa(VTNet      *net,
                                  double     Sbase,
                                  AnsiString arqRelFlow,
                                  AnsiString arqRelRede)

   {
   //variáveis locais
   bool        sucesso    = true;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   // Proteção
   if(net == NULL) return(false);

   //calcula Fluxo de Potência p/ VTNet em todos os patamares
   for(int ind_pat = 0; (sucesso) && (ind_pat < patamares->NumPatamar()); ind_pat++)
      {
      if(ind_pat > 0) continue;  // Só o primeiro patamar (temporário)
      sucesso = Executa(net, ind_pat);
      }

   // Retorna
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoCC::Executa(VTNet *net,
                                  int   ind_pat)
   {
   // Variáveis locais
   VTPath *path;

   // Define nome da rede
   path     = (VTPath *) apl->GetObject(__classid(VTPath));
   NomeRede = ChangeFileExt(ExtractFileName(path->ArqRede), "");

   //reinicia objeto Data p/ o cálculo
   if (! monta->Inicia(net, data, ind_pat)) return(false);

   //insere Eqptos que interessam p/ fluxo CC
   if (! monta->InsereBarra())      return(false);
   if (! monta->InsereLigacao())    return(false);
   if (! monta->InsereCarga())      return(false);
   if (! monta->InsereGerador())    return(false);
   if (! monta->InsereSuprimento()) return(false);

   //executa cálculo de fluxo
   if (! fpcc->Executa(data, NomeRede, ind_pat)) return(false);

   //obtém resultados
   if (! monta->SalvaResBarra())      return(false);
   if (! monta->SalvaResTrafo2E())    return(false);
   if (! monta->SalvaResTrecho())     return(false);
   if (! monta->SalvaResGerador())    return(false);
   if (! monta->SalvaResSuprimento()) return(false);

   // Retorna
   return(true);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFluxoCC::Modelo(void)
   {
   return("Corrente contínua");
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoCC::PM_GetEquilibrado(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoCC::UtilizaBarraPV(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoCC::ValidaAderencia(VTNet *net,
                                          VTLog *plo)
	{
	return(true);
	}
	
//---------------------------------------------------------------------------
//eof
