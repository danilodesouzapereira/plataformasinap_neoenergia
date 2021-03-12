//---------------------------------------------------------------------------
#ifndef VTExtratorH
#define VTExtratorH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "EstruturaMed.h"
//---------------------------------------------------------------------------
class VTApl;
class VTMedicao;

//---------------------------------------------------------------------------
class VTExtrator : public TObject
   {
	public:
                   __fastcall  VTExtrator(void) {};
      virtual      __fastcall ~VTExtrator(void) {};
		virtual bool __fastcall  Executa(AnsiString arqTxt, VTMedicao *medicao, strMEDIDOR *medidor) = 0;

   private: //dados
      AnsiString   dirBase, dirRede;
      VTApl       *apl;
   };

//---------------------------------------------------------------------------
//VTExtrator* __fastcall NewObjExtratorElpa(VTApl *apl);
//VTExtrator* __fastcall NewObjExtratorSul(VTApl *apl);
VTExtrator* __fastcall NewObjExtratorCustom(VTApl *apl);
#endif
//-----------------------------------------------------------------------------
// eof
