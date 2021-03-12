//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TTipoChave.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTTipoChave
//---------------------------------------------------------------------------
VTTipoChave* __fastcall NewObjTipoChave(void)
   {
   try{
      return (new TTipoChave());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TTipoChave::TTipoChave(void)
   {
   //inicia valores default
   Id        = -1;
   Tipo      = chaveINDEFINIDA;
   Operacao  = chaveOP_INDEFINIDA;
   Codigo    = "Indefinida";
   }

//---------------------------------------------------------------------------
__fastcall TTipoChave::~TTipoChave(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTTipoChave* __fastcall TTipoChave::Clone(void)
   {
	//variáveis locais
   VTTipoChave *tipochave;

   //cria um novo TipoChave
   if ((tipochave = NewObjTipoChave()) != NULL)
      {//copia dados deste TipoChave p/ novo TipoChave
      tipochave->CopiaAtributosDe(*this);
      }
   return(tipochave);
   }

//---------------------------------------------------------------------------
void __fastcall TTipoChave::CopiaAtributosDe(VTTipoChave &tipochave)
   {
   //copia atributos de VTTipoChave
   PD.id             = tipochave.Id;
   PD.tipo           = tipochave.Tipo;
   PD.modo_operacao  = tipochave.Operacao;
   PD.status         = tipochave.StatusAsUnsigned;
   PD.color          = tipochave.Color;
   PD.codigo         = tipochave.Codigo;
   PD.extern_id      = tipochave.Extern_id;
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTipoChave::PM_GetCodigo(void)
   {
   return(PD.codigo);
   }

//---------------------------------------------------------------------------
TColor __fastcall TTipoChave::PM_GetColor(void)
   {
   return(PD.color);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTipoChave::PM_GetExtern_id(void)
   {
   return(PD.extern_id);
   }

//---------------------------------------------------------------------------
int __fastcall TTipoChave::PM_GetId(void)
   {
   return(PD.id);
   }

//---------------------------------------------------------------------------
TObject* __fastcall TTipoChave::PM_GetObj(void)
   {
   return(PD.obj);
   }

//---------------------------------------------------------------------------
int __fastcall TTipoChave::PM_GetOperacao(void)
   {
   return(PD.modo_operacao);
   }

//---------------------------------------------------------------------------
bool __fastcall TTipoChave::PM_GetStatus(unsigned bit)
   {
   return((PD.status & bit) == bit);
   }

//---------------------------------------------------------------------------
unsigned __fastcall TTipoChave::PM_GetStatusAsUnsigned(void)
   {
   return(PD.status);
   }

//---------------------------------------------------------------------------
int __fastcall TTipoChave::PM_GetTipo(void)
   {
   return(PD.tipo);
   }

//---------------------------------------------------------------------------
void __fastcall TTipoChave::PM_SetCodigo(AnsiString codigo)
   {
   //salva Codigo
   PD.codigo = codigo;
   }

//---------------------------------------------------------------------------
void __fastcall TTipoChave::PM_SetColor(TColor color)
   {
   PD.color = color;
   }

//---------------------------------------------------------------------------
void __fastcall TTipoChave::PM_SetExtern_id(AnsiString extern_id)
   {
   //salva extern_id
   PD.extern_id = extern_id;
   }

//---------------------------------------------------------------------------
void __fastcall TTipoChave::PM_SetId(int id)
   {
   //salva ID
   PD.id = id;
   }

//---------------------------------------------------------------------------
void __fastcall TTipoChave::PM_SetObj(TObject* obj)
   {
   PD.obj = obj;
   }

//---------------------------------------------------------------------------
void __fastcall TTipoChave::PM_SetOperacao(int modo)
   {
   PD.modo_operacao = modo;
   }

//---------------------------------------------------------------------------
void __fastcall TTipoChave::PM_SetStatus(unsigned bit, bool enabled)
   {
   if (enabled)
      {PD.status |= bit;}
   else
      {PD.status &= ~bit;}
   }

//---------------------------------------------------------------------------
void __fastcall TTipoChave::PM_SetStatusAsUnsigned(unsigned status)
   {
   PD.status = status;
   }

//---------------------------------------------------------------------------
void __fastcall TTipoChave::PM_SetTipo(int tipo)
   {
   PD.tipo = tipo;
   }

//---------------------------------------------------------------------------
//eof