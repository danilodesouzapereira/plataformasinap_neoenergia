//---------------------------------------------------------------------------

#ifndef TSinapPerdaH
#define TSinapPerdaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTSinapPerda.h"
//---------------------------------------------------------------------------
//classes
 class VTApl;
 class VTRede;
 class VTRedes;
//---------------------------------------------------------------------------
class TSinapPerda : public VTSinapPerda
{
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
				__fastcall  TSinapPerda(VTApl *apl_owner);
				__fastcall ~TSinapPerda(void);
		bool 	__fastcall  Executa(VTRedes *redes);
		bool 	__fastcall  Executa(VTRede *rede);
		bool 	__fastcall  Executa(TList *lisRedes);

	public:

	protected:  //métodos acessados via property
		//getters
		bool 	  	  __fastcall PM_GetIsAgregada(void);
		VTRede* 	  __fastcall PM_GetRede(void);
		int		  __fastcall PM_GetSegmento(void);
		//getters perda
		double  __fastcall PM_GetPerdaTrafo_mwh(void);
		//double  __fastcall PM_GetPerdaTrafoFerro_mwh(void);
		double  __fastcall PM_GetPerdaCabo_mwh(void);
		double  __fastcall PM_GetPerdaCapacitor_mwh(void);
		double  __fastcall PM_GetPerdaReator_mwh(void);
		double  __fastcall PM_GetPerdaRegulador_mwh(void);
		//double  __fastcall PM_GetPerdaMedidor_mwh(void);
		//double  __fastcall PM_GetPerdaRamal_mwh(void);
		double  __fastcall PM_GetPerdaOutros_mwh(void);
		double  __fastcall PM_GetPerdaTotal_mwh(void);

		//getters balanco
		double  __fastcall PM_GetEnergiaEntrada_mwh(void);
		double  __fastcall PM_GetEnergiaEntregue_mwh(void);
		double  __fastcall PM_GetEnergiaGerada_mwh(void);

	private: //métodos
		double 	__fastcall CalcEEntradaDia(void);
		double 	__fastcall CalcEEntregueDia(void);
		double 	__fastcall CalcEGeradaDia(void);
		void 	 	__fastcall CalcPerdasDia(void);
		void 	 	__fastcall DestroiLista(TList *lista);
		bool		__fastcall ExecutaFluxoPotencia(void);
		void 		__fastcall InicializaPerdas(void);
		void		__fastcall SomaPerdas(strPERDAS *perda, strPERDAS *perdaAgregada);
	private: //dados externos
		VTApl 	*apl;
		VTRedes 	*redes;
		VTRede   *rede;

	private: //dados locais
		//strPERDAS	perdasAux;

};

#endif
//---------------------------------------------------------------------------
//eof
