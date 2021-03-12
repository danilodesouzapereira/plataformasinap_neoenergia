//-----------------------------------------------------------------------------
#ifndef TStkEqvH
#define TStkEqvH

//-----------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTStkEqv.h"

//---------------------------------------------------------------------------
class VTBD;
class VTCarga;
class VTEstudo;
class VTRede;
class VTRedes;

//---------------------------------------------------------------------------
class TStkEqv : public VTStkEqv
   {
   public:
		   __fastcall  TStkEqv(VTApl *apl_owner);
		   __fastcall ~TStkEqv(void);
	  bool __fastcall  Executa(bool a4NaBT);

   private: //métodos
      VTCarga* __fastcall  CriaCargaEqv(AnsiString codigo);
      void     __fastcall  DefineConsumoCarga(VTCarga *carga_eqv, TList *lisCARGA);
      void     __fastcall  DefineNConsCarga(VTCarga *carga_eqv, TList *lisCARGA);
      int      __fastcall  DeterminaFasePriTrafos(TList *lisEQP);
      int      __fastcall  DeterminaFaseSecTrafos(TList *lisEQP);
      void     __fastcall  EliminaEqptos(VTRede *rede);
      void     __fastcall  EliminaRede(VTRede *rede);
      VTRede*  __fastcall  ExisteRedeOriginal(AnsiString codigo);
      void     __fastcall  IniciaStrListOrdenadaRedesOriginais(void);
      bool     __fastcall  LeListaRedesOriginais(void);
      int      __fastcall  QtdeConsumidorRede(VTRede *rede);
      bool     __fastcall  RetiraRedeBT(void);
      bool     __fastcall  RetiraRedeBTmenosTrafo(bool a4NaBT);
      bool     __fastcall  RetiraRedeEquivalente(void);
      void     __fastcall TransfereTrafosParaPrimario(TList *lisTRAFO, VTRede *rede_pri);

   private: //objetos externos
      VTApl *apl;

   private: //dados
      TList       *lisEQP;
      TStringList *StrList;
      VTRadial    *radial;
      struct{
            VTBD     *bd;
            VTEstudo *estudo;
            VTRedes  *redes;
            }local;
   };

#endif
//---------------------------------------------------------------------------
//eof
