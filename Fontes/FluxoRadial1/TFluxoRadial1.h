//---------------------------------------------------------------------------
#ifndef TFluxoRadial1H
#define TFluxoRadial1H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;
#include "..\Fluxo\VTFluxo.h"

class  TImpressao1;
class  VTAtualiza1;
class  VTCalculo1;
class  VTData;
class  VTGeralC;
class  VTMonta1;
class  VTRede1;
class  VTResultados1;
class  TBarra;
class  TLigacaoC;
class  TTrafo2;
class  TTrafo3;
class  TTrafo3Radial;
class  TTrafoEquiv;
class  VTLigacaoG1;
class  VTSisLin;

//---------------------------------------------------------------------------
class TFluxoRadial1 : public VTFluxo
{
public:  // Métodos
              __fastcall  TFluxoRadial1(VTApl *apl);
              __fastcall ~TFluxoRadial1(void);
   void       __fastcall  CfgAvisoErro(bool enabled);
   bool       __fastcall  Executa(VTNet      *net,
                                  double     Sbase,
                                  AnsiString arqRelFlow,
                                  AnsiString arqRelRede);
   AnsiString __fastcall  Modelo(void);
   bool       __fastcall  UtilizaBarraPV(void);
	bool       __fastcall  ValidaAderencia(VTNet *net, VTLog *plog);

protected:  //métodos acessados via property
   bool       __fastcall  PM_GetEquilibrado(void);

private:  // Objetos externos
   TImpressao1   *impr1;
   VTApl         *apl;
   VTData        *data;
   VTAtualiza1   *atual1;
   VTCalculo1    *calculo1;
   VTGeralC      *geralC;
   VTMonta1      *monta1;
   VTRede1       *rede1;
   VTResultados1 *result1;

private:  // Dados locais
   bool            aviso_enabled, flag_tensao_inicializada;
   double          grau_rad, raiz3, raiz3_inverso;
   complex<double> cum, czero;
   complex<double> *vet_cor_acm, *vet_deltav, *vet_tensoes, *vet_TC;
   TList           *lisLIG_RADIAL;

private:  // Métodos
   void            __fastcall AjustaTapReguladores(void);
   bool            __fastcall AlocaMemoria2(void);
   bool            __fastcall CalculaDeltaV(void);
   void            __fastcall CalculaDeltaVLig2(int         nl,
                                                VTLigacaoG1 *ligG);
   void            __fastcall CalculaDeltaVLig3(int         nl,
                                                VTLigacaoG1 *ligG);
   bool            __fastcall CalculaRede(int np);
   bool            __fastcall CalculaTensao(void);
   bool            __fastcall ExecutaFluxoRadial1(void);
   bool            __fastcall Finaliza(void);
   // bool            __fastcall Inicia(void);
   void            __fastcall IniciaCorrenteLig2(int         nl,
                                                 VTLigacaoG1 *ligG);
   void            __fastcall IniciaCorrenteLig3(VTLigacaoG1 *ligG);
   bool            __fastcall MontaYnodalTrafosReguladores(bool flag_todos);
   bool            __fastcall OrdenaRadial(void);
   bool            __fastcall VerificaConvergencia(bool *flag_tap_alterado);
};

#endif
//---------------------------------------------------------------------------

