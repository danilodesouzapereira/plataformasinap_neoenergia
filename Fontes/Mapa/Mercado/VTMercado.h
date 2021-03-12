//---------------------------------------------------------------------------
#ifndef VTMercadoH
#define VTMercadoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class  VTApl;
class  VTCarga;
class  VTCenario;
class  VTCrescimento;
class  VTRede;
class  VTRedes;
class  VTTipoRede;
class  VTSegmento;
struct strUTM;

//---------------------------------------------------------------------------
#define ANO_INDEF  -1

//---------------------------------------------------------------------------
class VTMercado : public TObject
   {
   public:  //property
      __property int        AnoAtual       = {read=PM_GetAnoAtual,   write=PM_SetAnoAtual  };
      __property int        AnoFinal       = {read=PM_GetAnoFinal,   write=PM_SetAnoFinal  };
      __property int        AnoInicial     = {read=PM_GetAnoInicial, write=PM_SetAnoInicial};
      __property int        AnoRede        = {read=PM_GetAnoRede,    write=PM_SetAnoRede   };
      __property int        IndiceAnoAtual = {read=PM_GetIndiceAnoAtual                    };
      __property int        NumAnos        = {read=PM_GetNumAnos                           };
      __property VTCenario* Cenario        = {read=PM_GetCenario,    write=PM_SetCenario   };

   public:  //métodos
									__fastcall  VTMercado(void) {};
		virtual              __fastcall ~VTMercado(void) {};
		virtual VTCenario*   __fastcall  CriaCenarioNovo(void) = 0;
		virtual VTSegmento*  __fastcall  CriaSegmentoDefault(void) = 0;
		virtual VTSegmento*  __fastcall  CriaSegmentoNova(void) = 0;
		virtual VTSegmento*  __fastcall  CriaSegmentoNova(VTRede *rede) = 0;
		virtual VTSegmento*  __fastcall  CriaSegmentoNova(VTTipoRede *tiporede) = 0;
      virtual void        __fastcall DefineCrescimento(VTCarga *carga, VTRede *rede) = 0;
		//virtual void         __fastcall  DefineCrescimento(VTCarga *carga, VTRedes *redes) = 0;
		//virtual void         __fastcall  DefineSegmento(VTRede *rede) = 0;
		virtual VTSegmento*  __fastcall  ExisteSegmento(strUTM &utm_cm) = 0;
		virtual VTSegmento*  __fastcall  ExisteSegmento(VTRede *rede, bool proprio= false) = 0;
		virtual VTSegmento*  __fastcall  ExisteSegmento(VTTipoRede *tiporede) = 0;
		virtual VTSegmento*  __fastcall  ExisteSegmentoDefault(void) = 0;
      //virtual VTSegmento*  __fastcall  ExisteSegmentoProprio(VTRede *rede) = 0;
		virtual void         __fastcall  IniciaCrescimentoNulo(VTSegmento *regiao) = 0;
		virtual void         __fastcall  IniciaRedes(VTRedes *redes) = 0;
		virtual bool         __fastcall  InsereCenario(VTCenario *cenario) = 0;
		virtual bool         __fastcall  InsereCrescimento(VTSegmento *regiao, VTCrescimento *crescimento) = 0;
		virtual bool         __fastcall  InsereSegmento(VTSegmento *regiao) = 0;
		virtual TList*       __fastcall  LisCenario(void) = 0;
		virtual TList*       __fastcall  LisSegmento(void) = 0;
		virtual void         __fastcall  RedefinePeriodo(int ano_ini, int ano_fim) = 0;
		virtual void         __fastcall  Reinicia(void) = 0;
		virtual void         __fastcall  RetiraCenario(VTCenario *cenario) = 0;
		virtual void         __fastcall  RetiraSegmento(VTSegmento *regiao) = 0;

   protected:  //métodos acessados via property
      virtual int        __fastcall PM_GetAnoAtual(void) = 0;
      virtual int        __fastcall PM_GetAnoFinal(void) = 0;
      virtual int        __fastcall PM_GetAnoInicial(void) = 0;
      virtual int        __fastcall PM_GetAnoRede(void) = 0;
      virtual VTCenario* __fastcall PM_GetCenario(void) = 0;
      virtual int        __fastcall PM_GetIndiceAnoAtual(void) = 0;
      virtual int        __fastcall PM_GetNumAnos(void) = 0;
      virtual void       __fastcall PM_SetAnoAtual(int ano_atual) = 0;
      virtual void       __fastcall PM_SetAnoFinal(int ano_fim) = 0;
      virtual void       __fastcall PM_SetAnoInicial(int ano_ini) = 0;
      virtual void       __fastcall PM_SetAnoRede(int ano_rede) = 0;
      virtual void       __fastcall PM_SetCenario(VTCenario *cenario) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMercado
//---------------------------------------------------------------------------
VTMercado* __fastcall NewObjMercado(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof
