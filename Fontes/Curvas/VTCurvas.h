//---------------------------------------------------------------------------
#ifndef VTCurvasH
#define VTCurvasH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTClasse;
class VTCurva;
class VTEqpto;


//---------------------------------------------------------------------------
class VTCurvas : public TObject
   {
   public:  //property
		__property TDateTime DataReferencia = {read=PM_GetDataReferencia, write=PM_SetDataReferencia};
      __property int       DiaMedicao     = {read=PM_GetDiaMedicao,     write=PM_SetDiaMedicao    };

   public:
                       __fastcall  VTCurvas(void) {};
                       __fastcall ~VTCurvas(void) {};
      virtual void     __fastcall  Clear(void) = 0;
      virtual bool     __fastcall  ExisteAlteracao(void) = 0;
      virtual VTCurva* __fastcall  ExisteCurvaTipicaCapacitor(int curva_id=-1) = 0;
      virtual VTCurva* __fastcall  ExisteCurvaTipicaCarga(int curva_id) = 0;
		virtual VTCurva* __fastcall  ExisteCurvaTipicaCarga(AnsiString Codigo) = 0;
		virtual VTCurva* __fastcall  ExisteCurvaTipicaCarga(VTClasse *classe, double energia_kwmes, int dia_medicao=-1) = 0;
      virtual VTCurva* __fastcall  ExisteCurvaTipicaGeradorFotovoltaicoId(int curva_id) = 0;
      virtual VTCurva* __fastcall  ExisteCurvaTipicaGeradorFotovoltaicoMes(int mes) = 0;
		virtual VTCurva* __fastcall  ExisteCurvaTipicaIP(int curva_id=-1) = 0;
		virtual VTCurva* __fastcall  ExisteCurvaTipicaIP(AnsiString codigo) = 0;
		virtual VTCurva* __fastcall  ExisteCurvaTipicaReator(int curva_id=-1) = 0;
      virtual bool     __fastcall  InsereCurvaTipicaCapacitor(VTCurva *curva) = 0;
      virtual bool     __fastcall  InsereCurvaTipicaGeradorFotovoltaico(VTCurva *curva) = 0;
      virtual bool     __fastcall  InsereCurvaTipicaCarga(VTCurva *curva) = 0;
      virtual bool     __fastcall  InsereCurvaTipicaIP(VTCurva *curva) = 0;
      virtual bool     __fastcall  InsereCurvaTipicaReator(VTCurva *curva) = 0;
      virtual TList*   __fastcall  ListaCurvaTipicaCapacitor(void) = 0;
      virtual TList*   __fastcall  ListaCurvaTipicaCarga(void) = 0;
      virtual int      __fastcall  LisCurvaTipicaCarga(VTClasse *classe, int dia_medicao, TList *lisEXT) = 0;
      virtual TList*   __fastcall  ListaCurvaTipicaGeradorFotovoltaico(void) = 0;
      virtual TList*   __fastcall  ListaCurvaTipicaIP(void) = 0;
      virtual TList*   __fastcall  ListaCurvaTipicaReator(void) = 0;

   protected:  //métodos acessados via property
		virtual TDateTime __fastcall PM_GetDataReferencia(void) = 0;
		virtual int       __fastcall PM_GetDiaMedicao(void) = 0;
		virtual void      __fastcall PM_SetDataReferencia(TDateTime data) = 0;
		virtual void      __fastcall PM_SetDiaMedicao(int dia_medicao) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCurvas
//---------------------------------------------------------------------------
VTCurvas* __fastcall NewObjCurvas(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 