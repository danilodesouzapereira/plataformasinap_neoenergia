//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMotor.h"
#include "..\Curvas\VTCurva.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMotor* __fastcall NewObjMotor(void)
   {
   return(new TMotor());
   }

//---------------------------------------------------------------------------
__fastcall TMotor::TMotor(void)
   {
   //define tipo de motor: bobinado
   TipoMotor = tmBOBINADO;
   //define parâmerros defualt para eventual eqpto auxiliar de partida
   eqpto_partida.tipo                            = eqptoNENHUM;
   eqpto_partida.auto_trafo.tap_inicial          = 0.5;
   eqpto_partida.chave_YD.ligacao_inicial        = lgEST_ISO;
   eqpto_partida.soft_starter.freq_variavel      = true;
   eqpto_partida.soft_starter.duracao_partida_ms = 500;
   eqpto_partida.soft_starter.tensao_inicial_pu  = 0.6;
   //inicia atributos p/ visualização gráfica
   Show[eqp_FIXO]   = true;
   Show[eqp_CODIGO] = true;
   }

//---------------------------------------------------------------------------
__fastcall TMotor::~TMotor(void)
   {
   //destrói Curva somente se não for típica
   if ((curva != NULL)&&(! curva->Tipica)) delete curva;
   curva = NULL;
   }

//---------------------------------------------------------------------------
VTMotor* __fastcall TMotor::Clone(void)
   {
   //variáveis locais
	TMotor *motor;

   //cria um novo Motor
//	if ((motor = new TMotor(curva->Escala)) != NULL)
   if ((motor = new TMotor()) != NULL)
      {//copia dados deste Motor p/ o novo Motor
		motor->CopiaAtributosDe(*this);
      }
	return(motor);
   }

//---------------------------------------------------------------------------
void __fastcall TMotor::CopiaAtributosDe(VTEqpto &ref)
	{
	//variáveis locais
	TMotor &motor = (TMotor&)ref;

	//proteção
	if (ref.Tipo() != eqptoMOTOR) return;
	//copia atributos de TEqbar
	TEqbar::CopiaAtributosDe(motor);
	//copia atributos de VTMotor
   circ_eqv      = motor.circ_eqv;
   valor_nom     = motor.valor_nom;
   carga_mec     = motor.carga_mec;
   eqpto_partida = motor.eqpto_partida;
	//destrói Curva própria
//	if ((curva)&&(! curva->Tipica)) delete curva;
	//verifica se a Curva do motor de referência é típica
//	if (motor.curva->Tipica)
//		{//assume a mesma Curva típica
//		curva = motor.curva;
//		}
//	else
//		{//cria cópia de Curva própria
//		curva = motor.curva->Clone();
//		curva->Eqpto = this;
//      }
	}

//---------------------------------------------------------------------------
void __fastcall TMotor::DefineCurva(VTCurva *curva)
   {
   //salva ponteiro p/ objeto
   this->curva = curva;
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TMotor::ExisteCurva(void)
   {
   return(curva);
   }

//---------------------------------------------------------------------------
AnsiString  __fastcall TMotor::PM_GetPadrao(void)
	{
	return(padrao);
   }

//---------------------------------------------------------------------------
AnsiString  __fastcall TMotor::PM_GetTipoMotorAsString(void)
   {
   //define string o tipo de motor
   if (TipoMotor == tmBOBINADO) return("bobinado");
   if (TipoMotor == tmGAIOLA)   return("gaiola");

   return("indefinido");
   }
//---------------------------------------------------------------------------
void __fastcall TMotor::PM_SetPadrao(AnsiString padrao)
   {
   this->padrao = padrao;
   }

//---------------------------------------------------------------------------
int __fastcall TMotor::Tipo(void)
   {
   return(eqptoMOTOR);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMotor::TipoAsString(void)
   {
   return("Motor");
   }

//---------------------------------------------------------------------------
//eof
