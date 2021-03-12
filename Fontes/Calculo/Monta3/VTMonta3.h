//---------------------------------------------------------------------------
#ifndef VTMonta3H
#define VTMonta3H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class TCurtoAux3;
class TImpressao3;
class TTrafo32;
class VTApl;
class VTData;
class VTGeralC;
class VTRede3;
class VTSisLin;

//---------------------------------------------------------------------------
class VTMonta3 : public TObject
   {
   public:  // Métodos
                   __fastcall  VTMonta3() {};
      virtual      __fastcall ~VTMonta3(void) {};
      virtual bool __fastcall  AnalisaSuprimentosPatamar(int np) = 0;
      virtual bool __fastcall  AtribuiLinhasY(void) = 0;
      virtual bool __fastcall  CalculaZentrada_1Vez3nx3n(bool so_trechos=false) = 0;
      // virtual bool __fastcall  CalculaZentrada_nVezes3x3(void) = 0;
      virtual bool __fastcall  DefineAterramentoBarras(int             tipo_ater,
                                                       complex<double> zater_ohm=complex<double>(0., 0.)) = 0;
      virtual void __fastcall  ImprimeNumeracaoGlobal(int chave) = 0;
      virtual bool __fastcall  InicializaYnodal(void) = 0;
      virtual bool __fastcall  MontaLigacoesBarra(void) = 0;
      virtual bool __fastcall  MontaRedeEstimador3(void) = 0;
      virtual bool __fastcall  MontaRedeFluxoH3(void) = 0;
      virtual bool __fastcall  MontaRedeGauss3(void) = 0;
      virtual bool __fastcall  MontaRedeNR3(void) = 0;
      virtual bool __fastcall  MontaRedeRedutor3(void) = 0;
      virtual bool __fastcall  MontaY(int  ordemh=1,
                                      bool flag_inclui_mutuas=true,
                                      bool so_trechos=false) = 0;
      virtual bool __fastcall  MontaY_Conexa(void) = 0;
      virtual bool __fastcall  OrdenaBarrasSup(TList    *lisBAR,
                                               VTGeralC *geralC_aux,
                                               TList    *lisSUP=NULL) = 0;
      // virtual bool __fastcall  OrdenaTrechos(bool  flag_troca_barras,
      //                                        TList *lisTRECHO) = 0;
      // virtual bool __fastcall  VerificaNeutroConexo(void) = 0;
      virtual bool __fastcall  VerificaRedeConexa(bool flag_ordena_barras) = 0;

   public:  // Dados
      VTSisLin *Ynod;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMonta3
//---------------------------------------------------------------------------
VTMonta3 * __fastcall NewObjMonta3(TImpressao3 *impr3,
                                   VTRede3     *rede3,
                                   VTGeralC    *geralC,
                                   TCurtoAux3  *curto_aux3=NULL);

#endif
//---------------------------------------------------------------------------

