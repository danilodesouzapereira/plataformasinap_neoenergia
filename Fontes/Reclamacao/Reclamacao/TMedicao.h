//---------------------------------------------------------------------------
#ifndef TMedicaoH
#define TMedicaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMedicao.h"

//---------------------------------------------------------------------------
class VTPatamares;
class VTEqpto;
class VTTensao;

//---------------------------------------------------------------------------
class TMedicao  : public VTMedicao
   {
   public:
                __fastcall  TMedicao(VTApl *apl);
                __fastcall ~TMedicao(void);
      bool      __fastcall  AlteraMedicao(int ind_dia, int ind_pat, double vfn_kv[MAX_FASE]);
      bool      __fastcall  ExisteMedicao(TDateTime data, int ind_pat);
      bool      __fastcall  ExisteMedicao(int ind_dia, int ind_pat, double vfn_kv[MAX_FASE]);
      bool      __fastcall  ExisteMedicao(TDateTime data, int ind_pat, double vfn_kv[MAX_FASE]);
      VTTensao* __fastcall  ExisteTensao(TDateTime data);
      int       __fastcall  InsereMedicao(strREGISTRO &reg);
      void      __fastcall  Reinicia(void);

   protected:  //métodos acessados vias property
      VTCarga*  __fastcall  PM_GetCarga(void);
      TDateTime __fastcall  PM_GetData(int ind_dia);
      bool      __fastcall  PM_GetFaseEnabled(int ind_fase);
      int       __fastcall  PM_GetFasesRede(void);
      int       __fastcall  PM_GetNumeroDia(void);
      int       __fastcall  PM_GetNumeroMedicaoPorDia(void);
      int       __fastcall  PM_GetNumeroMedicaoTotal(void);
      void      __fastcall  PM_SetCarga(VTCarga* carga);
      void      __fastcall  PM_SetFaseEnabled(int ind_fase, bool enabled);
      void      __fastcall  PM_SetFasesRede(int fases_rede);

   private: //métodos
      bool      __fastcall DeterminaNumeroMedicaoPorDia(strREGISTRO &reg1, strREGISTRO &reg2);
      VTTensao* __fastcall ExisteTensao(int dia, int mes, int ano);
      void      __fastcall RedefinePatamares(void);
      int       __fastcall SalvaMedicao(strREGISTRO &reg);

   private: //objetos externos
      VTApl       *apl;
      VTPatamares *patamares;

   private: //dados
      bool        ask_user;
      strREGISTRO reg_ini, ult_reg;
      TList       *lisTENSAO, *lisEQP;
      struct   {
               int      num_medicao_dia, fases_rede;
               bool     fase_enabled[MAX_FASE];
               VTCarga *carga;
               }PD;
   };

//---------------------------------------------------------------------------
#endif
//eof


