// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TCalculaZcc.h"
#include <complex>
// ---------------------------------------------------------------------------
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Complexo\Complexo.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\ResCC\VTResCC.h>
#include <Fontes\Legenda\VTDado.h>
#include <Fontes\Legenda\VTGrupo.h>
#include <Fontes\Legenda\VTVetval.h>
#include <Fontes\Curtocircuito\VTCurtocircuito.h>

// ---------------------------------------------------------------------------
#include <DLL_Inc\CurtoCircuito.h>
#include <DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTCalculaZcc* __fastcall NewObjCalculaZcc(VTApl *apl)
{
	try
	{
		return (new TCalculaZcc(apl));
	}
	catch (Exception &e)
	{
	}
	// erro na criação do objeto
	return (NULL);
}

// ---------------------------------------------------------------------------
__fastcall TCalculaZcc::TCalculaZcc(VTApl *apl)
{
	this->apl = apl;

	curtocircuito =  DLL_NewObjCurtocircuito(apl);
	lisSTRCALCULAZCC = new TList;

	//inicia impedâncias de defeito com valores nulos
   zDef.Zdef3F_ff.r  = zDef.Zdef3F_ff.x  = 0;
   zDef.Zdef2F_ff.r  = zDef.Zdef2F_ff.x  = 0;
   zDef.Zdef2FT_ff.r = zDef.Zdef2FT_ff.x = 0;
   zDef.Zdef2FT_ft.r = zDef.Zdef2FT_ft.x = 0;
   zDef.Zdef1FT_ft.r = zDef.Zdef1FT_ft.x = 0;
}

// ---------------------------------------------------------------------------
__fastcall TCalculaZcc::~TCalculaZcc(void)
{

   if(curtocircuito)
   {
		delete curtocircuito;
		curtocircuito = NULL;
   }
   if(lisSTRCALCULAZCC)
   {
		LimpaTList(lisSTRCALCULAZCC);
		delete lisSTRCALCULAZCC;
		lisSTRCALCULAZCC = NULL;
   }
}
// ---------------------------------------------------------------------------
void __fastcall TCalculaZcc::CalculaZcc(VTBarra *barra, strIMP &z0, strIMP &z1)
{
	strCalculaZcc *str = NULL;
	VTRedes      *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
    VTResCC *resCC;
	VTGrupo *grupo;
	VTDado	*dado;
	complex<double> z0_ohm, z1_ohm, z0_pu, z1_pu;
	double vBase;
	double zBase;
	AnsiString desc;
	TList *lisDado;

	//inicializa
	z0.r  = 0;
	z0.x  = 0;
	z1.r  = 0;
	z1.x  = 0;

	try
	{
		//verifica se ja foi calculado
		if ((str = ExisteCalculado(barra)) == NULL)
		{
			//salva Vbase
			vBase = barra->vnom;
			//calcula o z entrada na barra selecionada
			curtocircuito->Executa(redes,zDef,false,barra,0,false);
			//pega o rescc
			resCC = (VTResCC*)curtocircuito->Apl->GetObject(__classid(VTResCC));
			//teste
			if (resCC == NULL)
				{return;}
			//pega os valores de zentrada
			grupo = resCC->IniciaGrupoZentrada();
			//le os valores de zentrada
			desc = grupo->Descricao;
			//pega os dados
			lisDado = grupo->LisDado;
			//percorre todos dados
			//devem ser 2
			if (lisDado->Count != 2)
				{throw new Exception("LisDado inválido");}
			dado = (VTDado*) lisDado->Items[0];
			//valores->Add(dado->valor);
			z0_ohm = StrToComplex(dado->valor);
			dado = (VTDado*) lisDado->Items[1];
			//valores->Add(dado->valor);
			z1_ohm = StrToComplex(dado->valor);
			//calcula valor em pu
			if (IsDoubleZero(vBase))
			{
				z0_pu = CZero();
				z0_pu = CZero();
			}
			else
			{	//calcula z base
				zBase = (vBase*vBase)*0.01; //k^2/M = 1
				z0_pu = z0_ohm/zBase;
				z1_pu = z1_ohm/zBase;
			}
			//adicionaValores
			lisSTRCALCULAZCC->Add(str = new strCalculaZcc);
			str->eqpto = (VTEqpto*) barra;
			str->z0cc_pu.r =  z0_pu.real();//r0
			str->z1cc_pu.r =  z1_pu.real();//r1
			str->z0cc_pu.x =  z0_pu.imag();//x0
			str->z1cc_pu.x =  z1_pu.imag();//x1
		}
		//atribui o valor calculado
		z0.r  = str->z0cc_pu.r ;
		z0.x  = str->z0cc_pu.x ;
		z1.r  = str->z1cc_pu.r ;
		z1.x  = str->z1cc_pu.x ;
	}
	catch (Exception &e)
	{
		z0.r  = 0;
		z0.x  = 0;
		z1.r  = 0;
		z1.x  = 0;
    }
}
// ---------------------------------------------------------------------------
strCalculaZcc* __fastcall TCalculaZcc::ExisteCalculado(VTEqpto *eqpto)
{
	strCalculaZcc *str = NULL;
	strCalculaZcc *strRef = NULL;

	for (int ns = 0; ns < lisSTRCALCULAZCC->Count; ns++)
	{
		str = (strCalculaZcc*) lisSTRCALCULAZCC->Items[ns];
		if (eqpto == str->eqpto)
		{
			strRef = str;
		}
	}

	return strRef;
}
// ---------------------------------------------------------------------------
complex<double> __fastcall TCalculaZcc::StrToComplex(AnsiString dadoValor)
{
	double dreal, dimag;
	AnsiString real, imag;
	bool imagPositivo = false;
	int pos;

	//verifica se possui valor positivo ou negativo
	if ((pos = dadoValor.Pos("+")) >= 0)
		{imagPositivo = true;}
	else if ((pos = dadoValor.Pos("-")) >= 0)
		{imagPositivo = false;}
	//pega a parte real
//	real = dadoValor.SubString(0,8);
//	imag = dadoValor.SubString(11,18);
	real = dadoValor.SubString(0,pos-1);
	imag = dadoValor.SubString(pos+2,dadoValor.Length());
	//atribui valor
	try
	{
		dreal = StrToDouble(real);
		dimag = StrToDouble(imag);
	}
	catch (Exception &e)
	{
		dreal = 0.;
		dimag = 0.;
    }
	if(!imagPositivo)
	{
    	dimag = dimag*(-1.);
    }
	complex<double> z(dreal,dimag);
	return z;
}
//---------------------------------------------------------------------------
// eof
