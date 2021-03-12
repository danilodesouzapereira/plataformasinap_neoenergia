//---------------------------------------------------------------------------
#ifndef TMontaMedicaoEquilibradaH
#define TMontaMedicaoEquilibradaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMontaMedicao.h"

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTCarga;
class VTEqpto;
class VTCanal;
class VTMedidor;

//---------------------------------------------------------------------------
class TMontaMedicaoEquilibrada : public VTMontaMedicao
   {
   public:  //métodos
             __fastcall  TMontaMedicaoEquilibrada(VTApl *apl);
             __fastcall ~TMontaMedicaoEquilibrada(void);
      bool   __fastcall  InsereMedidores(void);
      TList* __fastcall  LisMedidor(void);
      bool   __fastcall  RefleteMedicaoNasCargas(void);
      void   __fastcall  RetiraMedidores(void);

   private:  //métodos
      bool       __fastcall  ConverteCargaTipica(TList *lisCARGA);
      bool       __fastcall  ExisteCapacitor(VTBarra *barra, TList *lisEQP);
      bool       __fastcall  ExisteCargaAjustavel(VTBarra *barra, TList *lisEQP);
      VTCanal*   __fastcall  InsereCanal_P(VTMedidor *medidor, VTEqpto *eqpto, int fases);
      VTCanal*   __fastcall  InsereCanal_Q(VTMedidor *medidor, VTEqpto *eqpto, int fases);
      bool       __fastcall  InsereCurvaNula(VTCanal *canal);
      VTMedidor* __fastcall  InsereMedidorPQ(VTBarra *barra, TList *lisCAR, TList *lisCAP);
      VTMedidor* __fastcall  InsereMedidorQ(VTBarra *barra, TList *lisCAP);
      bool       __fastcall  RefleteMedicaoCargaUnica(VTMedidor *medidor, VTCarga *carga, TList *lisCAP);
      bool       __fastcall  RefleteMedicaoLisCargas(VTMedidor *medidor, TList *lisCAR, TList *lisCAP);
      double     __fastcall  TotalizaQ_mvar(TList *lisEQP, int ind_pat);

   private:  //objetos externos
      VTApl  *apl;

   private: //dados locais
      TList *lisBAR;
      TList *lisCAP;
      TList *lisCAR;
      TList *lisMEDIDOR;
   };

#endif
//---------------------------------------------------------------------------
//eof
