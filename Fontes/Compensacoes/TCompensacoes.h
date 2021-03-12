//---------------------------------------------------------------------------
#ifndef TCompensacoesH
#define TCompensacoesH

//---------------------------------------------------------------------------
#include "VTCompensacoes.h"

//---------------------------------------------------------------------------
class TCompensacoes : public VTCompensacoes
{
	public:
							 __fastcall TCompensacoes(TComponent* Owner, VTApl* AplOwner, AnsiString empresa);
							 __fastcall ~TCompensacoes(void);
		bool            __fastcall Inicializa(AnsiString dirEmpresa);
//		double          __fastcall Calcula(VTSubestacao* SE, VTCarga* Carga, double DIC, double FIC, double DMIC = 0., double DICRI = 0., eTipoPeriodo Periodo = etpAnual);
		double          __fastcall Calcula(VTSubestacao* SE, VTCarga* Carga, double *DICsMensais, double *FICsMensais);
		double 			 __fastcall Calcula(VTSubestacao* SE, VTCarga* Carga, double DIC, double FIC, double DMIC = 0. , eTipoPeriodo Periodo = etpAnual, int index = 0);
		void   			 __fastcall Parametros(VTSubestacao* SE, VTCarga* Carga, strParamComp &param);
		TConjuntoAneel* __fastcall RetornaConjuntoAneel(VTSubestacao* SE);
		strFaixa*       __fastcall RetornaFaixaDEC(TConjuntoAneel* Conjunto);
		strFaixa*       __fastcall RetornaFaixaFEC(TConjuntoAneel* Conjunto);

	private:
		bool __fastcall   BuscaLimites(VTSubestacao* SE, VTCarga* Carga, double *DICp, double *FICp, double *DMICp, eTipoPeriodo Periodo = etpAnual);
		double __fastcall CalculaCompensacao(VTCarga * Carga, double TUSD, double transgressao, double kei, eTipoPeriodo Periodo);
		double __fastcall CalculaTransgressao(double DICv, double FICv, double DMICv, double DICp, double FICp, double DMICp, double *DICnv, double *FICnv,int index, eTipoPeriodo Periodo);
		double __fastcall CalculaTUSD(VTCarga* Carga, double *kei);
		void   __fastcall CarregaConjuntosAneel(AnsiString empresa);
		void   __fastcall CarregaLimitesAneel(void);
		void   __fastcall CarregaValoresTarifas(AnsiString empresa);
//		double __fastcall RetornaLimiteDICRI(eTipoLocal Local = etlUrbano, eTipoNivelTensao = etntBT);

	public:
		TComponent* Owner;
		VTApl*      AplOwner;
		TList*      LisConjuntos;
		TList*      LisFaixas;
		// Acrescimo para consideração de indicadores não violados
		double DICnv[12], FICnv[12];

		TConjuntoAneel* ConjuntoDefault;

		struct
		{
			double B1;
			double B2;
			double B3;
			double B4;
			double A4;
		} TUSD;
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
//eof