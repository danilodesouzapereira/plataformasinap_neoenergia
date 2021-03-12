//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TCentrocarga.h"
#include "VTRegiao.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
//#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TCentrocarga::TCentrocarga(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //cria objetos
   lisCAR = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TCentrocarga::~TCentrocarga(void)
   {
   //destrói lista sem destrui seus seus objetos
   if (lisCAR) {delete lisCAR; lisCAR = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TCentrocarga::CalculaCoordenadas(void)
   {
   //variáveis locais
   int              x_m, y_m;
   double           s_mva_carga = 0.;
   double           s_mva_total = 0.;
   complex<double>  dist_X_smva = CZero();
   VTCarga         *carga;
   VTDemanda       *demanda    = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //loop p/ todosmPtoCarga
   for (int n = 0; n < lisCAR->Count; n++)
      {
      carga      = (VTCarga*)lisCAR->Items[n];
      //obtém demanda e coordenadas da Carga
      s_mva_carga = Abs(demanda->S_mva[carga][ind_pat]);
      carga->pbarra->CoordenadasUtm_m(x_m, y_m);
      //acumula o produto (x, y) * s_mva
      dist_X_smva += (complex<double>(x_m, y_m) * 0.001) * s_mva_carga;
      //acumula demanda total das cargas
      s_mva_total += s_mva_carga;
      }
   //salva do centro de carga
   if (s_mva_total > 0)
      {
      regiao->Centrocarga_cm.x = (dist_X_smva.real() / s_mva_total) * 1.0e5;
      regiao->Centrocarga_cm.y = (dist_X_smva.imag() / s_mva_total) * 1.0e5;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TCentrocarga::Executa(VTRegiao *regiao, int ind_pat)
   {
   //salva parâmetros
   this->regiao  = regiao;
   this->ind_pat = ind_pat;
   //obtém lista de Cargas da Regiao
   lisCAR->Clear();
   regiao->LisEqpto(lisCAR, eqptoCARGA);
   //determina coordenadas do centro de carga
   CalculaCoordenadas();
   //define cor das Celulas
   return(true);
   }

//---------------------------------------------------------------------------
//eof

