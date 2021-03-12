//---------------------------------------------------------------------------
#ifndef TDataEqptoH
#define TDataEqptoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTDataEqpto.h"
//---------------------------------------------------------------------------
class TDataEqpto : public VTDataEqpto
	{
	public:
				 __fastcall TDataEqpto(VTEqpto* eqpto);
				 __fastcall ~TDataEqpto(void);
		  void __fastcall CopiaDados(VTDataEqpto* dataExt);

	private: //métodos

	private: //dados externos

	private: //dados locais
	public:
	};

#endif
//---------------------------------------------------------------------------
//eof
