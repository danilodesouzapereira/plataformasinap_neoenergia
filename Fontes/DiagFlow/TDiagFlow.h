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
public: //métodos públicos
          __fastcall TDiagFlow(VTApl *apl);
          __fastcall ~TDiagFlow(void);
   TList* __fastcall Executa(void);       //retorna lista de erros preenchida pelo Execute

private: //métodos locais

private: //objetos externos
   VTApl *apl;

private: //variáveis locais
   TList *lisERRO;
};
//---------------------------------------------------------------------------
#endif
