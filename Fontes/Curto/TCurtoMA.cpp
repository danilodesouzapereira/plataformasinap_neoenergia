//---------------------------------------------------------------------------
#include <vcl.h>         
#pragma hdrstop
#include <math.hpp>
#include ".\TCurtoMA.h"
#include "..\Apl\VTApl.h"
#include "..\Calculo\Comum13\VTGeralC.h"
#include "..\CurtoCircuito\TDefeitos.h"
#include "..\Gauss3\VTGauss3.h"
#include "..\..\DLL_Inc\Gauss3.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCurtoMA * __fastcall NewObjCurtoMA(VTApl *apl)
   {
   try{
      return(new TCurtoMA(apl));
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TCurtoMA::TCurtoMA(VTApl *apl)
{
// Salva ponteiro p/ objeto
this->apl = apl;

// Cria objeto VTGauss3
gauss3 = DLL_NewObjGauss3(apl);

try{// Inicialização
   TipoRelatorio = trSEM_RELATORIO;
   }catch (Exception &e)
      {
      }
}

//---------------------------------------------------------------------------
__fastcall TCurtoMA::~TCurtoMA(void)
{
if(gauss3 != NULL) {delete gauss3; gauss3 = NULL;}
}

//---------------------------------------------------------------------------
bool __fastcall TCurtoMA::Executa(int      ind_pat,
                                  VTData   *data,
                                  VTBarra  *barra,
                                  VTNet    *net,
                                  VTRedeCC *redeCC)
{
// Esta função é chamada pelo Sinap a cada vez que o usuário seleciona
// uma barra para cálculo de curto-circuito

// Variáveis locais
bool sucesso;

// Salva definições
gauss3->TipoRelatorio = TipoRelatorio;

// Executa Gauss3
sucesso = gauss3->ExecutaCurtoMA(ind_pat, data, barra, net, redeCC);

return(sucesso);
}

//---------------------------------------------------------------------------
bool __fastcall TCurtoMA::Executa(int      ind_pat,
                                  VTData   *data,
                                  TList    *lisDEF,
                                  VTNet    *net,
                                  VTRedeCC *redeCC)
   {
   // Variáveis locais
   strSinapCurtoMA *str;
   TList           *lisTMP;
   TDefeito        *defeito;
   bool            sucesso;

   // Salva definições
   gauss3->TipoRelatorio = TipoRelatorio;
   //cria lista com strSinapCurtoMA
   lisTMP = new TList();
   for (int n = 0; n < lisDEF->Count; n++)
      {
      defeito          = (TDefeito*)lisDEF->Items[n];
      lisTMP->Add(str  = new strSinapCurtoMA);
      str->barra_sinap = defeito->barra;
      str->fases_sinap = defeito->fases;
      }
   // Executa Gauss3
   sucesso = gauss3->ExecutaCurtoMA(ind_pat, data, NULL, net, redeCC, lisTMP);
   //destró lista e seus objetos
   for (int n = 0;  n < lisTMP->Count; n++)
      {
      str = (strSinapCurtoMA*)lisTMP->Items[n];
      delete str;
      }
   delete lisTMP;

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoMA::Executa(int     tipo_def,
                                  strIMP  zdef_fase_ohm,
                                  strIMP  zdef_terra_ohm,
                                  VTBarra *barra)
{
// Esta função calcula um tipo específico de curto-circuito, numa barra
// especificada. Deve ser chamada após a montagem da rede, a qual é feita
// pela função Inicia().

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TCurtoMA::Executa(int      tipo_def,
                                  strIMP   zdef_fase_ohm,
                                  strIMP   zdef_terra_ohm,
                                  VTTrecho *trecho,
                                  VTBarra  *barra_ref,
                                  double   dist_m)
{
// Esta função calcula um tipo específico de curto-circuito, num ponto
// especificado de um trecho especificado. Deve ser chamada após a montagem
// da rede, a qual é feita pela função Inicia().

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TCurtoMA::Iamp(VTLigacao       *ligacao,
                               complex<double> iamp[MAX_FASE+MAX_FASE])
   {

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoMA::Inicia(VTData *data,
                                 VTNet  *net,
                                 int    ind_pat)
   {
   // Esta função monta a rede uma única vez, para cálculos de curto-circuito
   // solicitados pelo Localizador.
   // Os cálculos de curto-circuito podem ser repetidos com a mesma rede,
   // e são executados pelas funções:
   //    bool __fastcall TCurto::Executa(int     tipo_def,
   //                                    strIMP  zdef_fase_ohm,
   //                                    strIMP  zdef_terra_ohm,
   //                                    VTBarra *barra)
   //    bool __fastcall TCurto::Executa(int      tipo_def,
   //                                    strIMP   zdef_fase_ohm,
   //                                    strIMP   zdef_terra_ohm,
   //                                    VTTrecho *trecho,
   //                                    VTBarra  *barra_ref,
   //                                    double   dist_m)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TCurtoMA::Vfn_kv(VTBarra         *barra,
                                 complex<double> vkv[MAX_FASE])
   {

   return(true);
   }

//---------------------------------------------------------------------------
//eof

