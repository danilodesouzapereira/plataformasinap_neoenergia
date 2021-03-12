// ---------------------------------------------------------------------------
#ifndef TEstiloH
#define TEstiloH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEstilo.h"
#include <map>
// ---------------------------------------------------------------------------
class VTApl;
// ---------------------------------------------------------------------------
class TEstilo : public VTEstilo
{
	public:
					 __fastcall TEstilo(void);
		virtual		 __fastcall ~TEstilo(void);
				int  __fastcall PM_Tipo(void) {return(elemvESTILO);};
				bool __fastcall InsereCopiaEstiloAtributo(strESTILO_ATRIBUTO *ref);
				bool __fastcall LeEstiloAtributos(void);
				bool __fastcall SalvaEstiloAtributos(void);

	protected:
        strESTILO_ATRIBUTO* __fastcall CriaAtributo(int estiloAtributoId, AnsiString valor);
		strESTILO_ATRIBUTO* __fastcall ExisteAtributo(int atributoGraficoId);
				bool __fastcall InsereEstiloAtributo(strESTILO_ATRIBUTO *atributo);
                bool __fastcall LeEstiloAtributos(strESTILO_ATRIBUTO *ref);
				void __fastcall LimpaLisAtributos(void);
		//dados externos
		VTApl *apl;
		// dados locais
		//provisorio
//		DynamicArray<strESTILO_ATRIBUTO> atributos;
		AnsiString mErrorFunction;
};

#endif
// ---------------------------------------------------------------------------
// eof
