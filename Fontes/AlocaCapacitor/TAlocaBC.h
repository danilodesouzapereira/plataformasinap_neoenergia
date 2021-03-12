//---------------------------------------------------------------------------
#ifndef TAlocaBCH
#define TAlocaBCH

//---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
#include <complex>
#include "VTAlocaBC.h"

//---------------------------------------------------------------------------
class VTBanco;
class VTBancos;
class VTBarra;
class VTCarga;
class VTCapacitor;
class VTLigacao;
class VTPatamar;
class VTProgresso;
class VTPrimario;
class VTRadial;
class VTTronco;

//---------------------------------------------------------------------------
class TAlocaBC : public VTAlocaBC
   {
   public:
             __fastcall  TAlocaBC(VTApl *apl);
             __fastcall ~TAlocaBC(void);
      bool   __fastcall  Executa(TList* lisTabCap);
      TList* __fastcall  ListaBancoAlocado(void);
      int    __fastcall  ListaCapacitorAlocado(TList *lisEXT);
      int    __fastcall  ListaCloneCapacitorAlocado(TList *lisEXT);
		void   __fastcall  Reinicia(void);
		bool   __fastcall  RetiraBancosAlocadosDaRede(TList *lisCap);
      int    __fastcall  VerificaPrimariosIsolados(TList *lisEXT);

   private:
            VTBanco*        __fastcall AlocaBancoBarraFilho(VTLigacao* ligacao, VTBarra* barra);
            bool            __fastcall AlocaBC(VTPrimario* primario);
            void            __fastcall AjustaCurvaCapacitor(VTBanco *banco);
            VTBarra*        __fastcall BarraFilho(VTLigacao* ligacao);
            VTBarra*        __fastcall BarraPai(VTLigacao* ligacao);
            complex<double> __fastcall CalculaDemandaTotal(VTPatamar* patamar);
            bool            __fastcall CalculoFluxo(void);
            void            __fastcall CalculaQmvarExcedente1(void);
            void            __fastcall CalculaQmvarExcedente2(void);
            bool            __fastcall CriaCurvaPropria(VTCapacitor* cap);
            int             __fastcall DeterminaPico(void);
            void            __fastcall DesabilitaLigacaoMontante(VTLigacao *ligacao);
            void            __fastcall DesabilitaLigacaoPorDistanciaMinima(void);
            void            __fastcall DesabilitaLigacaoRamais(void);
            double          __fastcall Gambi_GetQinj_mvar(VTBarra* barra, int pat);
            void            __fastcall Gambi_SetQinj_mvar(VTBarra* barra, int pat, double q_mvar);
            int             __fastcall InsereCapacitoresNaRede(void);
            VTBanco*        __fastcall MontaBancoCapacitor(VTLigacao *ligacao, VTBarra* barra, VTCapacitor* capac, int unidades);
            double          __fastcall QMax(void);
            bool            __fastcall QMaxLimitado(void);
            double          __fastcall QMin(void);
            double          __fastcall QTotalMvarAlocado(void);


   private: //objetos externos
      VTApl       *apl;
      VTProgresso *prog;

   private: //dados locais
      VTBancos *bancos;
      VTTronco *tronco;
      VTRadial *radial;
      TList    *lisTabCap; //lista de capacitores disponiveis
      TList    *lisBar;
      TList    *lisLig;
      int      indPatMax; //patamar de carregamento máximo
      int      ind_cap;
   };

//---------------------------------------------------------------------------
#endif
