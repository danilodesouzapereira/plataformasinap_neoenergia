//---------------------------------------------------------------------------

#ifndef TExtratorCustomH
#define TExtratorCustomH

//arquivos inclu�dos---------------------------------------------------------
#include <Classes.hpp>
#include "VTExtrator.h"
#include "EstruturaMed.h"
class TExtratorCustom: public VTExtrator
	{
	public:
				__fastcall  TExtratorCustom(VTApl *apl);
				__fastcall ~TExtratorCustom(void);
		bool  __fastcall  Executa(AnsiString arqTxt, VTMedicao *medicao, strMEDIDOR *medidor);
	private: //dados
		AnsiString   dirBase, dirRede;
		VTApl       *apl;

	};


#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef TExtratorElpaH
#define TExtratorElpaH



//---------------------------------------------------------------------------

#endif
 //-----------------------------------------------------------------------------
// eof