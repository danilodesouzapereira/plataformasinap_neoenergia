//---------------------------------------------------------------------------
#ifndef TDiagFlowH
#define TDiagFlowH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTDiagFlow.h"

//---------------------------------------------------------------------------
class VTRedes;
class VTEqpto;

//---------------------------------------------------------------------------
class TDiagFlow : public VTDiagFlow
{
public: //m�todos p�blicos
          __fastcall TDiagFlow(VTApl *apl);
          __fastcall ~TDiagFlow(void);
   TList* __fastcall Executa(void);       //retorna lista de erros preenchida pelo Execute

private: //m�todos locais

private: //objetos externos
   VTApl *apl;

private: //vari�veis locais
   TList *lisERRO;
};
//---------------------------------------------------------------------------
#endif
