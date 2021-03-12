//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAtivo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTAtivo* __fastcall NewObjAtivo(void)
   {
   return(new TAtivo());
   }

//---------------------------------------------------------------------------
__fastcall TAtivo::TAtivo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TAtivo::~TAtivo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TAtivo::TipoAcaoAsString(void)
   {
   if (TipoAcao == acaoINSERE_ATIVO) return("Inserir");
   if (TipoAcao == acaoRETIRA_ATIVO) return("Retirar");
   //tipo inválido
   return("inválida");
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TAtivo::TipoAtivoAsString(void)
   {
   switch(TipoAtivo)
      {
      case ativoNAO_DEFINIDO:     return("");
      case ativoBARRA:            return("Barra");
      case ativoCAPACITOR:        return("Bco.Capacitor");
      case ativoCAPSERIE:         return("Cap.Série");
      case ativoCARGA:            return("Carga");
      case ativoCHAVE:            return("Chave");
      case ativoCHAVE_DISJUNTOR:  return("Disjuntor");
      case ativoCHAVE_RELIGADORA: return("Religadora");
      case ativoGERADOR:          return("Gerador");
      case ativoFILTRO:           return("Filtro");
      case ativoREATOR:           return("Bco.Reator");
      case ativoREDE:             return("Rede");
      case ativoREDE_SDBT:        return("Rede SDBT");
      case ativoREDE_SDMT:        return("Rede SDMT");
      case ativoREDE_SED:         return("SED");
      case ativoREGULADOR:        return("Reg.Tensão");
      case ativoSUPRIMENTO:       return("Suprimento");
      case ativoTRAFO:            return("Trafo");
      case ativoTRAFO3E:          return("Trafo 3 enrol.");
      case ativoTRAFOZZ:          return("Trafo ZigZag");
      case ativoTRECHO_REDE:      return("Trecho rede");
      default:                    return("não definido");
      }
   }

//---------------------------------------------------------------------------
//eof

