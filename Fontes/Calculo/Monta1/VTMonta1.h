//---------------------------------------------------------------------------
#ifndef VTMonta1H
#define VTMonta1H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class TImpressao1;
class VTApl;
class VTData;
class VTAtualiza1;
class VTGeralC;
class VTRede1;
class TTrafo2;
class TTrafo3;
class VTSisLin;

//---------------------------------------------------------------------------
class VTMonta1 : public TObject
   {
   public:  // Métodos
                   __fastcall  VTMonta1() {};
      virtual      __fastcall ~VTMonta1(void) {};
      virtual bool __fastcall  AnalisaSuprimentosPatamar(int np) = 0;
      virtual bool __fastcall  InicializaYnodal(int seq) = 0;
      virtual bool __fastcall  MontaLigacoesBarra(void) = 0;
      virtual bool __fastcall  MontaRedeCurtoCS(void) = 0;
      virtual bool __fastcall  MontaRedeEstabilidade(void) = 0;
      virtual bool __fastcall  MontaRedeEstimador1(void) = 0;
      virtual bool __fastcall  MontaRedeFluxoRadial1(void) = 0;
      virtual bool __fastcall  MontaRedeGauss1(void) = 0;
      virtual bool __fastcall  MontaRedeNR1(void) = 0;
      virtual bool __fastcall  MontaRedePartidaMotor(void) = 0;
      virtual bool __fastcall  MontaRedeRedutor1(void) = 0;
      virtual bool __fastcall  MontaY_Seq0(bool so_trechos=false) = 0;
      virtual bool __fastcall  MontaY_Seq1(bool            flag_admit_sup=false,
                                           complex<double> *vet_ycarga=NULL,
                                           bool            flag_inclui_mutuas=true,
                                           bool            so_trechos=false) = 0;
      virtual bool __fastcall  MontaY_Seq1_Conexa(void) = 0;
      virtual int  __fastcall  OffsetBarra(int linY) = 0;
      virtual bool __fastcall  OrdenaBarrasSup(TList    *lisBAR,
                                               VTGeralC *geralC_aux,
                                               TList    *lisSUP=NULL) = 0;
      // virtual bool __fastcall  OrdenaTrechos(bool  flag_troca_barras,
      //                                        TList *lisTRECHO) = 0;
      virtual bool __fastcall  VerificaRedeConexa(bool flag_ordena_barras) = 0;

   public:  // Dados
      VTSisLin *Ynod1; // Sequência direta: CURTO_CS, EQV1, ESTAB, ESTIM1, G1, NR1, PM
      VTSisLin *Ynod0; // Sequência zero:   CURTO_CS, EQV1
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMonta1
//---------------------------------------------------------------------------
VTMonta1* __fastcall NewObjMonta1(TImpressao1 *impr1,
                                  VTRede1     *rede1,
                                  VTGeralC    *geralC);

#endif
//---------------------------------------------------------------------------

