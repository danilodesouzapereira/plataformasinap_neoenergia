//---------------------------------------------------------------------------
#ifndef TMontaMedicaoDesequilibradaH
#define TMontaMedicaoDesequilibradaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMontaMedicao.h"
#include "..\Constante\Fases.h"

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTCarga;
class VTEqpto;
class VTCanal;
class VTMedidor;

//---------------------------------------------------------------------------
class TMontaMedicaoDesequilibrada : public VTMontaMedicao
   {
   public:  //métodos
             __fastcall  TMontaMedicaoDesequilibrada(VTApl *apl);
             __fastcall ~TMontaMedicaoDesequilibrada(void);
      bool   __fastcall  InsereMedidores(void);
      TList* __fastcall  LisMedidor(void);
      bool   __fastcall  RefleteMedicaoNasCargas(void);
      void   __fastcall  RetiraMedidores(void);

   private:  //métodos
      bool       __fastcall  ConverteCargaTipica(TList *lisCARGA);
      bool       __fastcall  ExisteCapacitor(VTBarra *barra, TList *lisEQP);
      bool       __fastcall  ExisteCargaAjustavel(VTBarra *barra, TList *lisEQP);
      bool       __fastcall  ExisteMedicaoP(VTBarra *barra, int fases);
      bool       __fastcall  ExisteMedicaoQ(VTBarra *barra, int fases);
      VTCanal*   __fastcall  InsereCanal_P(VTMedidor *medidor, VTBarra *barra, int fases);
      VTCanal*   __fastcall  InsereCanal_Q(VTMedidor *medidor, VTBarra *barra, int fases);
      bool       __fastcall  InsereCurvaNula(VTCanal *canal);
      VTMedidor* __fastcall  InsereMedidorPQ(VTBarra *barra, TList *lisCAR, TList *lisCAP);
      VTMedidor* __fastcall  InsereMedidorQ(VTBarra *barra, TList *lisCAP);
      bool       __fastcall  RefleteMedicaoCargaUnica(VTMedidor *medidor, VTCarga *carga, TList *lisCAP);
      bool       __fastcall  RefleteMedicaoLisCargas(VTMedidor *medidor, TList *lisCAR, TList *lisCAP);
      void       __fastcall  TotalizaQ_mvar(TList *lisEQP, int ind_pat, double q_mvar[MAX_FASE]);

   private:  //objetos externos
      VTApl  *apl;

   private: //dados locais
      TList *lisBAR;
      TList *lisCAP;
      TList *lisCAR;
      TList *lisEQP;
      TList *lisTMP;
      TList *lisMEDIDOR;
   };

#endif
//---------------------------------------------------------------------------
//eof
