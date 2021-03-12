//---------------------------------------------------------------------------
#ifndef TFluxoH3H
#define TFluxoH3H
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "..\..\Fluxo\VTFluxo.h"

//---------------------------------------------------------------------------
class  TBarra;
class  TBarraH3;
class  TImpressao3;
class  TTrecho;
class  VTAtualiza3;
class  VTCalculo3;
class  VTData;
class  VTGauss;
class  VTGauss3;
class  VTGeralC;
class  VTMonta3;
class  VTRede3;
class  VTRedeFH;
class  VTResultadosH3;
class  VTSisLin;
struct FluxoH3_G3;

//---------------------------------------------------------------------------
class TFluxoH3 : public VTFluxo
{
public:
              __fastcall  TFluxoH3(VTApl *apl);
              __fastcall ~TFluxoH3(void);
   void       __fastcall  CfgAvisoErro(bool enabled);
   bool       __fastcall  Executa(VTNet      *net,
                                  double     Sbase,
                                  AnsiString arqRelFlow,
                                  AnsiString arqRelRede);
   AnsiString __fastcall  Modelo(void);
   bool       __fastcall  UtilizaBarraPV(void);
	bool       __fastcall  ValidaAderencia(VTNet *net, VTLog *plog);

protected:  // Métodos acessados via property
   bool       __fastcall  PM_GetEquilibrado(void);

private:  // Objetos externos
   TImpressao3    *impr3;
   VTApl          *apl;
   VTData         *data;
   VTAtualiza3    *atual3;
   VTCalculo3     *calculo3;
   VTGauss3       *gauss3;
   VTGeralC       *geralC;
   VTMonta3       *monta3;
   VTRede3        *rede3;
   VTRedeFH       *redeFH;
   VTResultadosH3 *resultH3;

private:  // Dados locais
   bool            aviso_enabled;
   int             num_grav_ynodal;  // Controle de gravação de Ynodal
   double          raiz3, raiz3_inverso;
   complex<double> cum, czero, zerov;
   complex<double> *vet_TC, *vet_corr_bar_pu;
   FluxoH3_G3      *strG3;
   clock_t         fim, fim_main, inicio, inicio_main;
   TList           *lisBAR_H3;          // Lista de barras
   complex<double> *vet_perda_total_2;  // Perda total em cada frequência [kVA]


private: //métodos
   bool            __fastcall AlocaMemoria(void);
   bool            __fastcall AnaliseHarmonica(int np);
   complex<double> __fastcall AtualizaAdmitancia(int             ordem,
                                                 complex<double> admitancia);
   complex<double> __fastcall CalculaCorrenteLigacoes(TBarra *bar1,
                                                      int    nfa);
   double          __fastcall CalculaDH(double valor1,
                                        double valor2);
   bool            __fastcall CalculaResultadosGlobais(int np);
   bool            __fastcall CalculaResultadosHarmonica(int np,
                                                         int indfreq);
   bool            __fastcall CalculaSuprimentosH(complex<double> *vet_tensoes,
                                                  complex<double> *carga_total,
                                                  complex<double> *geracao_total);
   bool            __fastcall CalculaTC(int ordem);
   bool            __fastcall ConverteCargasLineares(void);
   bool            __fastcall DescobreFrequencias(void);
   bool            __fastcall ExecutaFluxoH3(VTNet *net);
   bool            __fastcall Finaliza(void);
   void            __fastcall GravaYnodal(int ordem);
   bool            __fastcall IniciaRedeFH(VTNet *net);
   bool            __fastcall MontaListaLigacoes(TBarra   *bar1,
                                                 TBarraH3 *barH3);
   bool            __fastcall MontaRedeHarmonica(void);
   bool            __fastcall SalvaResultadosFundamental(void);
   bool            __fastcall SalvaResultadosLigacoes(TBarraH3 *barH3,
                                                      int      indfreq);
};

#endif
//---------------------------------------------------------------------------
//eof
