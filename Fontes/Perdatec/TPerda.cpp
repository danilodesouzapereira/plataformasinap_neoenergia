//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "ConstantesPerda.h"
#include "TPerda.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPerda* __fastcall NewObjPerda(void)
   {
   return(new TPerda());
   }

//---------------------------------------------------------------------------
__fastcall TPerda::TPerda()
   {
   //inicia valores de perdas
   PD.cabo    = 0.;
   PD.trf     = 0.;
   PD.trf_cu  = 0.;
   PD.trf_fe  = 0.;
   PD.ramal   = 0.;
   PD.medidor = 0.;
   PD.outros  = 0.;
   PD.fluxo   = 0.;
   PD.fora    = 0.;
   PD.total   = 0.;
   }

//---------------------------------------------------------------------------
__fastcall TPerda::~TPerda(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TPerda::PM_GetDescricao(void)
   {
   return(PD.descricao);
   }

//---------------------------------------------------------------------------
double __fastcall TPerda::PM_GetEnergia_mwh(int seg_id)
   {
   switch(seg_id)
      {
      case segCABO:     return(PD.cabo   );
      case segTRF:      return(PD.trf    );
      case segTRF_CU:   return(PD.trf_cu );
      case segTRF_FE:   return(PD.trf_fe );
      case segRAMAL:    return(PD.ramal  );
      case segMEDIDOR:  return(PD.medidor);
      case segOUTROS:   return(PD.outros );
      case segFLUXO:    return(PD.fluxo  );
      case segFORA:     return(PD.fora   );
      case segTOTAL:    return(PD.total  );
      default:          return(0.);
      }
   }

//---------------------------------------------------------------------------
TObject* __fastcall TPerda::PM_GetObject(void)
   {
   return(PD.object);
   }

//---------------------------------------------------------------------------
void __fastcall TPerda::PM_SetDescricao(AnsiString descricao)
   {
   PD.descricao = descricao;
   }

//---------------------------------------------------------------------------
void __fastcall TPerda::PM_SetEnergia_mwh(int seg_id, double energia_mwh)
   {
   //2016.04.27 - trunca valores abaixo de ROUND_PRECISION
   energia_mwh = RoundTo(energia_mwh, ROUND_PRECISION);
   switch(seg_id)
      {
      case segCABO:     PD.cabo    = energia_mwh; break;
      case segTRF:      PD.trf     = energia_mwh; break;
      case segTRF_CU:   PD.trf_cu  = energia_mwh; break;
      case segTRF_FE:   PD.trf_fe  = energia_mwh; break;
      case segRAMAL:    PD.ramal   = energia_mwh; break;
      case segMEDIDOR:  PD.medidor = energia_mwh; break;
      case segOUTROS:   PD.outros  = energia_mwh; break;
      case segFLUXO:    PD.fluxo   = energia_mwh; break;
      case segFORA:     PD.fora    = energia_mwh; break;
      case segTOTAL:    PD.total   = energia_mwh; break;
      default:          break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TPerda::PM_SetObject(TObject *object)
   {
   PD.object = object;
   }

//---------------------------------------------------------------------------
void __fastcall TPerda::Soma(VTPerda *perda)
   {
   //proteção
   if (perda == NULL) return;
   //soma todos os valores
   PD.cabo    += ((TPerda*)perda)->PD.cabo;
   PD.trf     += ((TPerda*)perda)->PD.trf;
   PD.trf_cu  += ((TPerda*)perda)->PD.trf_cu;
   PD.trf_fe  += ((TPerda*)perda)->PD.trf_fe;
   PD.ramal   += ((TPerda*)perda)->PD.ramal;
   PD.medidor += ((TPerda*)perda)->PD.medidor;
   PD.outros  += ((TPerda*)perda)->PD.outros;
   PD.fluxo   += ((TPerda*)perda)->PD.fluxo;
   PD.fora    += ((TPerda*)perda)->PD.fora;
   PD.total   += ((TPerda*)perda)->PD.total;
   }

//---------------------------------------------------------------------------
//eof



