//---------------------------------------------------------------------------
#ifndef TMercadoH
#define TMercadoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMercado.h"

//---------------------------------------------------------------------------
class  VTCrescimento;

//---------------------------------------------------------------------------
class TMercado : public VTMercado
   {
   public:  //métodos
						__fastcall  TMercado(VTApl *apl);
						__fastcall ~TMercado(void);
		void        __fastcall  AssociaCargaSegmento(void);
		VTCenario*  __fastcall  CriaCenarioNovo(void);
		VTSegmento* __fastcall  CriaSegmentoDefault(void);
		VTSegmento* __fastcall  CriaSegmentoNova(void);
		VTSegmento* __fastcall  CriaSegmentoNova(VTRede *rede);
		VTSegmento* __fastcall  CriaSegmentoNova(VTTipoRede *tiporede);
      //VTSegmento* __fastcall  CriaSegmentoNova(VTRegiao *regiao);
      void        __fastcall DefineCrescimento(VTCarga *carga, VTRede *rede);
		//void        __fastcall  DefineCrescimento(VTCarga *carga, VTRedes *redes);
		//void        __fastcall  DefineSegmento(VTRede *rede);
		//VTSegmento* __fastcall  ExisteSegmento(bool isRegiao, strUTM &utm_cm);
		VTSegmento* __fastcall  ExisteSegmento(strUTM &utm_cm);
		VTSegmento* __fastcall  ExisteSegmento(VTRede *rede, bool proprio = false);
		VTSegmento* __fastcall  ExisteSegmento(VTTipoRede *tiporede);
      //VTSegmento* __fastcall  ExisteSegmento(VTRegiao *regiao);
		VTSegmento* __fastcall  ExisteSegmentoDefault(void);
      //VTSegmento* __fastcall  ExisteSegmentoProprio(VTRede *rede);
		void        __fastcall  IniciaCrescimentoNulo(VTSegmento *regiao);
		void        __fastcall  IniciaRedes(VTRedes *redes);
		bool        __fastcall  InsereCenario(VTCenario *cenario);
		bool        __fastcall  InsereCrescimento(VTSegmento *regiao, VTCrescimento *crescimento);
		bool        __fastcall  InsereSegmento(VTSegmento *regiao);
		TList*      __fastcall  LisCenario(void);
		TList*      __fastcall  LisSegmento(void);
		void        __fastcall  RedefinePeriodo(int ano_ini, int ano_fim);
		void        __fastcall  Reinicia(void);
		void        __fastcall  RetiraCenario(VTCenario *cenario);
		void        __fastcall  RetiraSegmento(VTSegmento *regiao);

	private: //métodos
		//VTCrescimento* __fastcall ExisteCrescimento(VTCarga *carga); //FKM 20.08.2015 Não era usada...
      //VTRede*     __fastcall  ExisteRede(VTCarga *carga, VTRedes *redes);
      VTSegmento* __fastcall  ExisteSegmentoRedeSupridora(VTRede *rede);

   protected:  //métodos acessados via property
      int        __fastcall PM_GetAnoAtual(void);
      int        __fastcall PM_GetAnoFinal(void);
      int        __fastcall PM_GetAnoInicial(void);
      int        __fastcall PM_GetAnoRede(void);
      VTCenario* __fastcall PM_GetCenario(void);
      int        __fastcall PM_GetIndiceAnoAtual(void);
      int        __fastcall PM_GetNumAnos(void);
      void       __fastcall PM_SetAnoAtual(int ano_atual);
      void       __fastcall PM_SetAnoFinal(int ano_fim);
      void       __fastcall PM_SetAnoInicial(int ano_ini);
      void       __fastcall PM_SetAnoRede(int ano_rede);
      void       __fastcall PM_SetCenario(VTCenario *cenario);

   protected:  //dados acessados via property
      struct   {
               int        ano_atual;
               int        ano_inicial;
               int        ano_final;
               int        ano_rede;
					VTCenario  *cenario;
					VTSegmento *segmento_default;
               }PD;

	private:  //objetos externos
      VTApl *apl;

   private:  //dados locais
		TList *lisSEGMENTO;
      TList *lisCENARIO;
      TList *lisEQP;
   };

//---------------------------------------------------------------------------
#endif
//eof
