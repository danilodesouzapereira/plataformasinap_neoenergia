//---------------------------------------------------------------------------
#ifndef VTDataEqptoH
#define VTDataEqptoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTConfiab.h"

//---------------------------------------------------------------------------
class VTEqpto;
//---------------------------------------------------------------------------
class VTDataEqpto : public TObject
	{
	public:
								 __fastcall VTDataEqpto(void) {};
		virtual            __fastcall ~VTDataEqpto(void) {};
		virtual       void __fastcall CopiaDados(VTDataEqpto* dataExt) = 0;

	public: //dados
		VTEqpto* eqpto;
		TObject* eqpto_aux;//Classificacao auxiliar p.ex tipo de cabo
		int      itipo;    //tipo do equipamento
		int      itipo_aux;//tipo auxiliar de equipamento
		AnsiString codigo;//Código do tipo
		PAR_CONF pconf;
		double   vmin, vmax;
		//TDataEqpto* dataIni;

	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTChave
//---------------------------------------------------------------------------
VTDataEqpto* __fastcall NewObjDataEqpto(VTEqpto* eqpto);

//---------------------------------------------------------------------------
#endif
//eof



