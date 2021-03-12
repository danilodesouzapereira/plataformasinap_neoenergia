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

try{// Inicializa��o
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
// Esta fun��o � chamada pelo Sinap a cada vez que o usu�rio seleciona
// uma barra para c�lculo de curto-circuito

// Vari�veis locais
bool sucesso;

// Salva defini��es
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
   // Vari�veis locais
   strSinapCurtoMA *str;
   TList           *lisTMP;
   TDefeito        *defeito;
   bool            sucesso;

   // Salva defini��es
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
   //destr� lista e seus objetos
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
// Esta fun��o calcula um tipo espec�fico de curto-circuito, numa barra
// especificada. Deve ser chamada ap�s a montagem da rede, a qual � feita
// pela fun��o Inicia().

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
// Esta fun��o calcula um tipo espec�fico de curto-circuito, num ponto
// especificado de um trecho especificado. Deve ser chamada ap�s a montagem
// da rede, a qual � feita pela fun��o Inicia().

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
   // Esta fun��o monta a rede uma �nica vez, para c�lculos de curto-circuito
   // solicitados pelo Localizador.
   // Os c�lculos de curto-circuito podem ser repetidos com a mesma rede,
   // e s�o executados pelas fun��es:
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

