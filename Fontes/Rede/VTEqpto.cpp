// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "VTEqpto.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall VTEqpto::VTEqpto(void)
{
	// inicia atributos do status
	Status[sttNOVO] = true;
	Status[sttENABLED] = true;
	// inicia atributos p/ visualização gráfica
	Show[eqp_VISIBLE] = true;
	Show[eqp_FIXO] = false;
	Show[eqp_CODIGO] = false;
	// inicia Id e Tag
	Id = -1;
	Tag = 0;
}

// ---------------------------------------------------------------------------
__fastcall VTEqpto::~VTEqpto(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
VTEqpto* __fastcall VTEqpto::Clone(void)
{
	return (NULL);
}

// ---------------------------------------------------------------------------
void __fastcall VTEqpto::CoordenadasEsquematico(int &x1, int &y1, int &x2, int &y2)
{
	// método a ser definido nas classes derivadas
}

// ---------------------------------------------------------------------------
void __fastcall VTEqpto::CoordenadasEsquematico(int &xm, int &ym, int &x1, int &y1, int &x2,
	int &y2, int &x3, int &y3)
{
	// método a ser definido nas classes derivadas
}

// ---------------------------------------------------------------------------
bool __fastcall VTEqpto::CoordenadasUtm_cm(int &x, int &y)
{
	// método a ser definido nas classes derivadas
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall VTEqpto::CoordenadasUtm_cm(int &x1, int &y1, int &x2, int &y2)
{
	// método a ser definido nas classes derivadas
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall VTEqpto::CoordenadasUtm_cm(int &xm, int &ym, int &x1, int &y1, int &x2, int &y2,
	int &x3, int &y3)
{
	// método a ser definido nas classes derivadas
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall VTEqpto::CoordenadasUtm_m(int &x, int &y)
{
	// método a ser definido nas classes derivadas
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall VTEqpto::CoordenadasUtm_m(int &x1, int &y1, int &x2, int &y2)
{
	// método a ser definido nas classes derivadas
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall VTEqpto::CoordenadasUtm_m(int &xm, int &ym, int &x1, int &y1, int &x2, int &y2,
	int &x3, int &y3)
{
	// método a ser definido nas classes derivadas
	return (false);
}

// ---------------------------------------------------------------------------
void __fastcall VTEqpto::CopiaAtributosDe(VTEqpto &ref)
{
	// copia atributos do Eqpto indicado
	PD = ref.PD;
}

// ---------------------------------------------------------------------------
void __fastcall VTEqpto::DefineStatus(unsigned mask_bit, bool enabled)
{
	// reinicia status
	// PD.status = 0x0;
	// define status indicado
	Status[mask_bit] = enabled;
}

// ---------------------------------------------------------------------------
VTCurva* __fastcall VTEqpto::ExisteCurva(void)
{
	return (NULL);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall VTEqpto::PM_GetCodigo(void)
{
	/*
	 if (! PD.codigo.IsEmpty())    return(PD.codigo);
	 if (! PD.extern_id.IsEmpty()) return(PD.extern_id);
	 */
	return (PD.codigo);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall VTEqpto::PM_GetExtern_id(void)
{
	return (PD.extern_id);
}

// ---------------------------------------------------------------------------
int __fastcall VTEqpto::PM_GetId(void)
{
	return (PD.id);
}

// ---------------------------------------------------------------------------
TObject* __fastcall VTEqpto::PM_GetObj(void)
{
	return (PD.obj);
}

// ---------------------------------------------------------------------------
VTObra* __fastcall VTEqpto::PM_GetObra(void)
{
	return (PD.obra);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall VTEqpto::PM_GetPadrao(void)
{
	return ("");
}

// ---------------------------------------------------------------------------
bool __fastcall VTEqpto::PM_GetShow(unsigned bit)
{
	return ((PD.show & bit) == bit);
}

// ---------------------------------------------------------------------------
unsigned __fastcall VTEqpto::PM_GetShowAsUnsigned(void)
{
	return (PD.show);
}

// ---------------------------------------------------------------------------
bool __fastcall VTEqpto::PM_GetStatus(unsigned bit)
{
	return ((PD.status & bit) == bit);
}

// ---------------------------------------------------------------------------
unsigned __fastcall VTEqpto::PM_GetStatusAsUnsigned(void)
{
	return (PD.status);
}

// ---------------------------------------------------------------------------
unsigned __fastcall VTEqpto::PM_GetTag(void)
{
	return (PD.tag);
}

// ---------------------------------------------------------------------------
void __fastcall VTEqpto::PM_SetObra(VTObra *obra)
{
	PD.obra = obra;
}

// ---------------------------------------------------------------------------
void __fastcall VTEqpto::PM_SetCodigo(AnsiString codigo)
{
	// proteção contra dimensão do string
	if (codigo.Length() > 30)
		codigo = codigo.SubString(1, 30);
	PD.codigo = codigo;
}

// ---------------------------------------------------------------------------
void __fastcall VTEqpto::PM_SetExtern_id(AnsiString extern_id)
{
	PD.extern_id = extern_id;
}

// ---------------------------------------------------------------------------
void __fastcall VTEqpto::PM_SetId(int id)
{
	PD.id = id;
}

// ---------------------------------------------------------------------------
void __fastcall VTEqpto::PM_SetObj(TObject* obj)
{
	PD.obj = obj;
}

// ---------------------------------------------------------------------------
void __fastcall VTEqpto::PM_SetPadrao(AnsiString padrao)
{
	// nada a fazer: método a sere redefinido nas classes que tem padrão associado
}

// ---------------------------------------------------------------------------
void __fastcall VTEqpto::PM_SetShow(unsigned mask_bit, bool enabled)
{
	if (enabled)
	{
		PD.show |= mask_bit;
	}
	else
	{
		PD.show &= ~mask_bit;
	}
}

// ---------------------------------------------------------------------------
void __fastcall VTEqpto::PM_SetShowAsUnsigned(unsigned show)
{
	PD.show = show;
}

// ---------------------------------------------------------------------------
void __fastcall VTEqpto::PM_SetStatus(unsigned mask_bit, bool enabled)
{
	if (enabled)
	{
		PD.status |= mask_bit;
	}
	else
	{
		PD.status &= ~mask_bit;
	}
}

// ---------------------------------------------------------------------------
void __fastcall VTEqpto::PM_SetStatusAsUnsigned(unsigned status)
{
	PD.status = status;
}

// ---------------------------------------------------------------------------
void __fastcall VTEqpto::PM_SetTag(unsigned tag)
{
	PD.tag = tag;
}

// ---------------------------------------------------------------------------
bool __fastcall VTEqpto::StatusAlterado(void)
{
	return ((PD.status & sttALTERADO) == sttALTERADO);
}

// ---------------------------------------------------------------------------
bool __fastcall VTEqpto::StatusDeslocado(void)
{
	return ((PD.status & sttDESLOCADO) == sttDESLOCADO);
}

// ---------------------------------------------------------------------------
bool __fastcall VTEqpto::StatusExcluido(void)
{
	return ((PD.status & sttEXCLUIDO) == sttEXCLUIDO);
}

// ---------------------------------------------------------------------------
bool __fastcall VTEqpto::StatusNovo(void)
{
	return ((PD.status & sttNOVO) == sttNOVO);
}

// ---------------------------------------------------------------------------
bool __fastcall VTEqpto::TipoBarra(void)
{
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall VTEqpto::TipoEqbar(void)
{
	return (false);
}

// ---------------------------------------------------------------------------
VTEqpto* __fastcall VTEqpto::TipoEqpto(TObject *obj)
{
	// variáveis locais
	TMetaClass *obj_class = obj->ClassType();
	AnsiString obj_class_name;

	while (obj_class)
	{
		obj_class_name = obj_class->ClassName();
		if (obj_class_name.AnsiCompareIC("VTEqpto") == 0)
			return ((VTEqpto*)obj);
		obj_class = obj_class->ClassParent();
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall VTEqpto::TipoEqpto_AsString(int tipo)
{
	switch (tipo)
	{
	case eqptoINDEF:
		return ("Indefinido");
	case eqptoARRANJO:
		return ("Arranjo");
	case eqptoBARRA:
		return ("Barra");
	case eqptoEQBAR:
		return ("Eqbar");
	case eqptoCABO:
		return ("Cabo");
	case eqptoCAPACITOR:
		return ("Capacitor");
	case eqptoCAPSERIE:
		return ("Capacitor série");
	case eqptoCARGA:
		return ("Carga");
	case eqptoCARGANL:
		return ("CargaNL");
	case eqptoCORTE:
		return ("Corte");
	case eqptoCHAVE:
		return ("Chave");
	case eqptoGERADOR:
		return ("Gerador");
	case eqptoFILTRO:
		return ("Filtro");
	case eqptoLIGACAO:
		return ("Ligacao");
	case eqptoMEDIDOR:
		return ("Medidor");
	case eqptoMREDE:
		return ("MRede");
	case eqptoMUTUA:
		return ("Mútua");
	case eqptoNO:
		return ("Nó");
	case eqptoNET:
		return ("Net");
	case eqptoREATOR:
		return ("Reator");
	case eqptoREDE:
		return ("Rede");
	case eqptoREDUCAO:
		return ("Redução");
	case eqptoREGULADOR:
		return ("Regulador");
	case eqptoSUPORTE:
		return ("Suporte");
	case eqptoSUPRIMENTO:
		return ("Suprimento");
	case eqptoTRAFO:
		return ("Trafo");
	case eqptoTRAFO3E:
		return ("Trafo 3E");
	case eqptoTRAFOZZ:
		return ("Trafo zig-zag");
	case eqptoTRAMO:
		return ("Tramo");
	case eqptoTRECHO:
		return ("Trecho");
	case eqptoYREF:
		return ("Yref");
	case eqptoARRANJOS:
		return ("Arranjos");
	case eqptoCLUSTER:
		return ("Cluster");
	case eqptoCANAL:
		return ("Canal");
	case eqptoBATERIA:
		return ("Bateria");
	default:
		return ("Indefinido");
	}
}

// ---------------------------------------------------------------------------
int __fastcall VTEqpto::TipoEqpto_BarraEqbarLigacao(int tipo)
{
	switch (tipo)
	{
		// case eqptoINDEF:      return(eqptoINDEF);
	case eqptoARRANJO:
		return (eqptoINDEF);
	case eqptoBARRA:
		return (eqptoBARRA);
	case eqptoEQBAR:
		return (eqptoEQBAR);
		// case eqptoCABO:       return(eqptoINDEF);
	case eqptoCAPACITOR:
		return (eqptoEQBAR);
	case eqptoCAPSERIE:
		return (eqptoLIGACAO);
	case eqptoCARGA:
		return (eqptoEQBAR);
	case eqptoCARGANL:
		return (eqptoEQBAR);
		// case eqptoCORTE:      return(eqptoINDEF);
	case eqptoCHAVE:
		return (eqptoLIGACAO);
	case eqptoGERADOR:
		return (eqptoEQBAR);
	case eqptoFILTRO:
		return (eqptoEQBAR);
	case eqptoLIGACAO:
		return (eqptoLIGACAO);
	case eqptoMEDIDOR:
		return (eqptoEQBAR);
	case eqptoMOTOR:
		return (eqptoEQBAR);
		// case eqptoMREDE:      return(eqptoINDEF);
	case eqptoMUTUA:
		return (eqptoMUTUA);
		// case eqptoNO:         return(eqptoINDEF);
		// case eqptoNET:        return(eqptoINDEF);
	case eqptoREATOR:
		return (eqptoEQBAR);
		// case eqptoREDE:       return(eqptoINDEF);
		// case eqptoREDUCAO:    return(eqptoINDEF);
	case eqptoREGULADOR:
		return (eqptoLIGACAO);
		// case eqptoSUPORTE:    return(eqptoINDEF);
	case eqptoSUPRIMENTO:
		return (eqptoEQBAR);
	case eqptoTRAFO:
		return (eqptoLIGACAO);
	case eqptoTRAFO3E:
		return (eqptoLIGACAO);
	case eqptoTRAFOZZ:
		return (eqptoEQBAR);
	case eqptoTRAMO:
		return (eqptoLIGACAO);
	case eqptoTRECHO:
		return (eqptoLIGACAO);
	case eqptoYREF:
		return (eqptoEQBAR);
	case eqptoBATERIA:
		return (eqptoEQBAR);
	}
	return (eqptoINDEF);
}

// ---------------------------------------------------------------------------
bool __fastcall VTEqpto::TipoLigacao(void)
{
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall VTEqpto::Visible(void)
{
	return (Show[eqp_VISIBLE]);
}

// ---------------------------------------------------------------------------
// eof
