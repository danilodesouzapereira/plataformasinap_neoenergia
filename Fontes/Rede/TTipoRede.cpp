//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TTipoRede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTTipoRede
//---------------------------------------------------------------------------
VTTipoRede* __fastcall NewObjTipoRede(void)
   {
   try{
      return (new TTipoRede());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TTipoRede::TTipoRede(void)
   {
   //inicia valores defaul
   Id        = -1;
   Codigo    = "INDEF";
   Descricao = "tipo não definido";
   }

//---------------------------------------------------------------------------
__fastcall TTipoRede::~TTipoRede(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTTipoRede* __fastcall TTipoRede::Clone(void)
   {
	//variáveis locais
   VTTipoRede *tiporede;

   //cria um novo TipoRede
   if ((tiporede = NewObjTipoRede()) != NULL)
      {//copia dados deste TipoRede p/ novo TipoRede
      tiporede->CopiaAtributosDe(*this);
      }
   return(tiporede);
   }

//---------------------------------------------------------------------------
void __fastcall TTipoRede::CopiaAtributosDe(VTTipoRede &tiporede)
   {
   //copia atributos de VTTipoRede
   PD.id          = tiporede.Id;
   PD.seg_id      = tiporede.Segmento;
   PD.status      = tiporede.StatusAsUnsigned;
   PD.color       = tiporede.Color;
   PD.codigo      = tiporede.Codigo;
   PD.descricao   = tiporede.Descricao;
   PD.vmax_kv     = tiporede.Vmax_kv;
   PD.vmin_kv     = tiporede.Vmin_kv;
   PD.valor_aux   = tiporede.ValorAux;
   }

//---------------------------------------------------------------------------
TColor __fastcall TTipoRede::PM_GetColor(void)
   {
   return(PD.color);
   }

//---------------------------------------------------------------------------
int __fastcall TTipoRede::PM_GetId(void)
   {
   return(PD.id);
   }

//---------------------------------------------------------------------------
int __fastcall TTipoRede::PM_GetSegmento(void)
   {
   return(PD.seg_id);
   }

//---------------------------------------------------------------------------
bool __fastcall TTipoRede::PM_GetStatus(unsigned bit)
   {
   return((PD.status & bit) == bit);
   }

//---------------------------------------------------------------------------
unsigned __fastcall TTipoRede::PM_GetStatusAsUnsigned(void)
   {
   return(PD.status);
   }

//---------------------------------------------------------------------------
void __fastcall TTipoRede::PM_SetColor(TColor color)
   {
   PD.color = color;
   }

//---------------------------------------------------------------------------
void __fastcall TTipoRede::PM_SetId(int tipo_id)
   {
   //salva ID
   PD.id = tipo_id;
   //inicia dados c/ o ID
   switch(PD.id)
      {
      case 1:  //ETT
         PD.seg_id = redeETT;
         PD.color  = clLtGray;
         break;
      case 2:  //Subtransmissao
         PD.seg_id = redeSUB;
         PD.color  = clFuchsia;
         break;
      case 3:  //Subestação
         PD.seg_id = redeETD;
         PD.color  = clLtGray;
         break;
      case 4:  //Primária
         PD.seg_id = redePRI;
         PD.color  = clBlue;
         break;
      case 5:  //Secundária
         PD.seg_id = redeSEC;
         PD.color  = clPurple;
         break;
      case 6:  //transmissao
         PD.color = clPurple;
         PD.seg_id = redeTRANS;
         break;
      case 7:  //reticulado
         PD.seg_id = redePRI;
         PD.color  = clPurple;
         break;
      case 8:  //mini-reticulado
         PD.color  = clPurple;
         PD.seg_id = redeSEC;
         break;
      case 9:  //ECH
         PD.color  = clPurple;
         PD.seg_id = redePRI;
         break;
      case 10: //rede de geração
         PD.color  = clPurple;
         PD.seg_id = redePRI;
         break;
      case 11: //híbrido
         PD.color  = clPurple;
         PD.seg_id = redeSEC;
         break;
      case 12: //spot
         PD.color  = clPurple;
         PD.seg_id = redeSEC;
         break;
      case 13: //Monofásica c/ retorno por terra
         PD.color  = clFuchsia;
         PD.seg_id = redeSEC;
         break;
      default:
         PD.color  = clWhite;
         PD.seg_id = redePRI;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TTipoRede::PM_SetSegmento(int seg_id)
   {
   PD.seg_id = seg_id;
   }

//---------------------------------------------------------------------------
void __fastcall TTipoRede::PM_SetStatus(unsigned bit, bool enabled)
   {
   if (enabled)
      {PD.status |= bit;}
   else
      {PD.status &= ~bit;}
   }

//---------------------------------------------------------------------------
void __fastcall TTipoRede::PM_SetStatusAsUnsigned(unsigned status)
   {
   PD.status = status;
   }

//---------------------------------------------------------------------------
//eof