//---------------------------------------------------------------------------
#ifndef TCurvasH
#define TCurvasH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCurvas.h"

//---------------------------------------------------------------------------
class TCurvas : public VTCurvas
   {
   public:
               __fastcall  TCurvas(VTApl *apl);
               __fastcall ~TCurvas(void);
      void     __fastcall  Clear(void);
      bool     __fastcall  ExisteAlteracao(void);
      VTCurva* __fastcall  ExisteCurvaTipicaCapacitor(int curva_id);
		VTCurva* __fastcall  ExisteCurvaTipicaCarga(int curva_id);
		VTCurva* __fastcall  ExisteCurvaTipicaCarga(AnsiString Codigo);
      VTCurva* __fastcall  ExisteCurvaTipicaCarga(VTClasse *classe, double energia_kwmes, int dia_medicao=-1);
      VTCurva* __fastcall  ExisteCurvaTipicaGeradorFotovoltaicoId(int curva_id);
      VTCurva* __fastcall  ExisteCurvaTipicaGeradorFotovoltaicoMes(int mes);
      VTCurva* __fastcall  ExisteCurvaTipicaIP(int curva_id);
      VTCurva* __fastcall  ExisteCurvaTipicaReator(int curva_id);
      bool     __fastcall  InsereCurvaTipicaCapacitor(VTCurva *curva);
      bool     __fastcall  InsereCurvaTipicaCarga(VTCurva *curva);
      bool     __fastcall  InsereCurvaTipicaGeradorFotovoltaico(VTCurva *curva);
		bool     __fastcall  InsereCurvaTipicaIP(VTCurva *curva);
		VTCurva* __fastcall  ExisteCurvaTipicaIP(AnsiString codigo);
      bool     __fastcall  InsereCurvaTipicaReator(VTCurva *curva);
      TList*   __fastcall  ListaCurvaTipicaCapacitor(void);
      TList*   __fastcall  ListaCurvaTipicaCarga(void);
      int      __fastcall  LisCurvaTipicaCarga(VTClasse *classe, int dia_medicao, TList *lisEXT);
      TList*   __fastcall  ListaCurvaTipicaGeradorFotovoltaico(void);
      TList*   __fastcall  ListaCurvaTipicaIP(void);
      TList*   __fastcall  ListaCurvaTipicaReator(void);


   protected:  //métodos acessados via property
		TDateTime __fastcall PM_GetDataReferencia(void);
		int       __fastcall PM_GetDiaMedicao(void);
		void      __fastcall PM_SetDataReferencia(TDateTime data);
		void      __fastcall PM_SetDiaMedicao(int dia_medicao);

   private: //métodos
      void     __fastcall DeterminaDiasMes(TDateTime data);
      VTCurva* __fastcall ExisteCurvaEqpto(TList *lisCURVA, AnsiString codigo);
      VTCurva* __fastcall ExisteCurvaEqpto(TList *lisCURVA, int eqpto_id);
      VTCurva* __fastcall ExisteCurvaTipica(TList *lisCURVA, TList *lisDUP, int curva_id);
      VTCurva* __fastcall ExisteCurvaTipicaCarga(VTClasse *classe, double lim_inf, double lim_sup, int dia_medicao);
      VTCurva* __fastcall ExisteCurvaTipicaCarga(TList *lisCURVA, AnsiString codigo, int dia_medicao); //DVK 2014.02.21
      void     __fastcall MontaCurvaSiamesaMVA_PUmedia(VTCurva *curva);
      void     __fastcall MontaCurvaSiamesaPUmaxima_PUmedia(VTCurva *curva);
      bool     __fastcall RecalculaFatorCorrecaoEnergiaDasCurvas(TDateTime data);

   private: //dados externos
      VTApl *apl;

   protected:  //dados acessados via property
      struct{
            TDateTime data_referencia;
            int       dia_medicao;
            }PD;

   private: //dados locais
      TList *lisCUR_TIP_CAP;  //lista de Curvas Típicas de Capacitores
      TList *lisCUR_TIP_CAR;  //lista de Curvas Típicas de Cargas
      TList *lisCUR_TIP_IP;   //lista de Curvas de Consumidores
      TList *lisCUR_TIP_REA;  //lista de Curvas de Reatores
      TList *lisCUR_TIP_GFV;  //lista de Curvas de Geradores Fotovoltaico
      //lista de objetos duplicados (código igual mas ID diferente)
      struct{
            TList *lisCUR_TIP_CAP;
            TList *lisCUR_TIP_CAR;
            TList *lisCUR_TIP_IP; 
            TList *lisCUR_TIP_REA;
            TList *lisCUR_TIP_GFV;
            }dup;
      struct{
            int sab;
            int dom;
            int dut;
            int mes;
            } num_dias;
   };

#endif
//---------------------------------------------------------------------------
//eof

