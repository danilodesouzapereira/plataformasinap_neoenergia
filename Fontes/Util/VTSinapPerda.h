//---------------------------------------------------------------------------
#ifndef VTSinapPerdaH
#define VTSinapPerdaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//classes

//---------------------------------------------------------------------------
class VTSinapPerda
	{
	public:
		public:  //property
		__property bool 		isAgregada = {read=PM_GetIsAgregada}; //,   write=PM_SetMultiThreadEnabled};
		__property VTRede* 	Rede= {read=PM_GetRede};
		__property int 		Segmento= {read=PM_GetSegmento};
		//perda
		__property double 	PerdaTrafo_mwh = {read=PM_GetPerdaTrafo_mwh};
		//__property double		PerdaTrafoFerro = {read=PM_GetPerdaTrafo_mwh};
		__property double		PerdaCabo_mwh = {read=PM_GetPerdaCabo_mwh};
		__property double		PerdaCapacitor_mwh = {read=PM_GetPerdaCabo_mwh};
		__property double		PerdaReator_mwh = {read=PM_GetPerdaReator_mwh};
		__property double		PerdaRegulador_mwh = {read=PM_GetPerdaRegulador_mwh};
		//__property double		PerdaMedidor_mwh = {read=PM_GetPerdaMedidor_mwh};
		//__property double		PerdaRamal_mwh = {read=PM_GetPerdaRamal_mwh};
		__property double		PerdaOutros_mwh = {read=PM_GetPerdaOutros_mwh};
		__property double		PerdaTotal_mwh = {read=PM_GetPerdaTotal_mwh};
		//balanco
		__property double		EnergiaEntrada_mwh = {read=PM_GetEnergiaEntrada_mwh};
		__property double		EnergiaEntregue_mwh = {read=PM_GetEnergiaEntregue_mwh};
		__property double		EnergiaGerada_mwh = {read=PM_GetEnergiaGerada_mwh};

	public:
						 __fastcall  VTSinapPerda(void) {};
						 __fastcall ~VTSinapPerda(void) {};
		virtual bool __fastcall  Executa(VTRede *rede) = 0;
		virtual bool __fastcall  Executa(VTRedes *redes) = 0;
		virtual bool __fastcall  Executa(TList *listaRedes) =0;

	protected:  //métodos acessados via property
		//getters
		virtual	bool		__fastcall PM_GetIsAgregada(void) = 0;
		virtual	VTRede*	__fastcall PM_GetRede(void) = 0;
		virtual	int		__fastcall PM_GetSegmento(void) = 0;
		//getters perda
		virtual	double  	__fastcall PM_GetPerdaTrafo_mwh(void) = 0;
		//virtual	double  __fastcall PM_GetPerdaTrafoFerro_mwh(void) = 0;
		virtual	double  	__fastcall PM_GetPerdaCabo_mwh(void) = 0;
		virtual	double  	__fastcall PM_GetPerdaCapacitor_mwh(void) = 0;
		virtual	double  	__fastcall PM_GetPerdaReator_mwh(void) = 0;
		virtual	double  	__fastcall PM_GetPerdaRegulador_mwh(void) = 0;
		//virtual	double  	__fastcall PM_GetPerdaMedidor_mwh(void) = 0;
		//virtual	double  	__fastcall PM_GetPerdaRamal_mwh(void) = 0;
		virtual	double  	__fastcall PM_GetPerdaOutros_mwh(void) = 0;
		virtual	double  	__fastcall PM_GetPerdaTotal_mwh(void) = 0;
		//getters balanco
		virtual	double 	__fastcall PM_GetEnergiaEntrada_mwh(void) = 0;
		virtual	double  	__fastcall PM_GetEnergiaEntregue_mwh(void) = 0;
		virtual	double  	__fastcall PM_GetEnergiaGerada_mwh(void) = 0;

	protected:  //dados acessados via property
		bool 	isAgregado;
		int	idSeg;
		VTRede	*rede;

		struct strPERDAS
		{
			double		trafo_mwh;
			//double		trafoFerro_mwh;
			double		cabo_mwh;
			double		capacitor_mwh;
			double		reator_mwh;
			double		regulador_mwh;
			//double		medidor_mwh;
			//double		ramal_mwh;
			double		outros_mwh;
			double		total_mwh;
		}perdas, perdasAux;
		struct strBALANCO
		{
			double	eEntrada_mwh;
			double	eEntregue_mwh;
			double	eGerada_mwh;
			//double	eSaida_mwh;
			//double      eInjExt_mwh;
			//double      eInjMontante_mwh;
			//double      eInjGD_mwh;
			//double      eForExt_mwh;
			//double      eForConsumidor_mwh;
			//double      eForJusante_mwh;

		}balanco;
	};
//-----------------------------------------------------------------------------
// função global para criar objeto VTSinapPerda
//-----------------------------------------------------------------------------
VTSinapPerda* __fastcall NewObjSinapPerda(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof
