//---------------------------------------------------------------------------

#ifndef TBDfuncH
#define TBDfuncH
using namespace std;

#include <classes.hpp>
#include <graphics.hpp>
#include <complex>
#include "BD_inc.h"
#include "..\Constante\Fases.h"
#include "..\Rede\Estrutura.h"
#include "..\Importa\Monta\VTMonta.h"

//---------------------------------------------------------------------------
// Estruturas auxiliares
#define MAXCRESCPERIODO 256
struct strCURVACRESCIMENTO
   {
   short         shtAnoIni;
   short         shtAnoFim;
   unsigned char uchPontos;
   float         fltTaxaPu[MAXCRESCPERIODO];   // Note que o índice é unsigned char (uchPontos)
   };

//---------------------------------------------------------------------------
class VTApl;
class VTChave;
class VTClasses;
class VTCrescimento;
class VTCurva;
class VTCurvas;
class VTEqpto;
class VTLog;
class VTMatZ;
class VTMatY;
class VTMatC;
class VTPoligono;
class VTRedes;
class VTTrafo;

//---------------------------------------------------------------------------
class TBDfunc : public TObject
   {
   public:
      __fastcall TBDfunc(VTApl* apl);
      __fastcall ~TBDfunc();

   private:
      // objetos externos
      VTApl*     apl;
      VTLog*     plog;
      VTCurvas*  curvas;
      VTClasses* classes;

   public:
      // Funções de uso geral
      bool     __fastcall DefineStatusEqpto(TList* lisEqp, bool status);
      bool     __fastcall DefineStatusEqpto(VTEqpto* eqpto, bool status);

      VTChave* __fastcall ExisteChave(VTRedes* redes, int intRedeId, int intChaveId);

      TStream* __fastcall MontaStreamMatComplex(complex<double> matComplex[4][4]);
      bool     __fastcall MontaStreamMatComplex(complex<double> matComplex[4][4], TStream* stream);
      bool     __fastcall DesmontaStreamMatComplex(TStream* ptrStream, complex<double> matComplex[4][4]);

      TStream* __fastcall MontaStreamPoligono(VTPoligono* poligono);
      bool     __fastcall MontaStreamPoligono(VTPoligono* poligono, TStream* stream);
      bool     __fastcall DesmontaStreamPoligono(TStream* ptrStream, VTPoligono* poligono);

      TStream* __fastcall MontaStreamVTMatZ(VTMatZ* ptrMatZ);
      bool     __fastcall MontaStreamVTMatZ(VTMatZ* ptrMatZ, TStream* stream);
      bool     __fastcall DesmontaStreamVTMatZ(TStream* ptrStream, strIMP matZ[4][4]);

      TStream* __fastcall MontaStreamTrafoEqv(VTTrafo *trafo);
      bool     __fastcall MontaStreamTrafoEqv(VTTrafo *trafo, TStream* stream);
      bool     __fastcall DesmontaStreamTrafoEqv(TStream* ptrStream, strTRAFO_EQV *eqv);

      TStream* __fastcall MontaStreamVTMatC(VTMatC* ptrMatC);
      bool     __fastcall MontaStreamVTMatC(VTMatC* ptrMatC, TStream* stream);
      bool     __fastcall DesmontaStreamVTMatC(TStream* ptrStream, double matC[4][4]);

      TStream* __fastcall MontaStreamCurvaEquipamento(int intTipoEquipId, VTCurva* curva);
      bool     __fastcall MontaStreamCurvaEquipamento(int intTipoEquipId, VTCurva* curva, TStream* stream);

      bool     __fastcall DesmontaStreamCurvaEquipamento(TStream* ptrStream, struct strCURVA* ptrStrCurva, strCURHEADER& curhdr);
      bool     __fastcall DesmontaStreamCurvaTipica(TStream* ptrStream, int intConjCurvaId, AnsiString strDescricao, int intTipoCurvaId, int intStatus);

      bool     __fastcall DesmontaStreamCurvaCrescimento(VTCrescimento* crescimento, TStream* ptrStream);
	  bool     __fastcall MontaStreamCurvaCrescimento(TStream* stream, VTCrescimento* crescimento);

	  bool     __fastcall DesmontaStreamDescricao(TStream* ptrStream, AnsiString &descricao);
	  bool     __fastcall MontaStreamDescricao(TStream* stream, AnsiString descricao);
   };


//---------------------------------------------------------------------------
#endif
