//---------------------------------------------------------------------------
#ifndef VTFasorH
#define VTFasorH

//arquivos incluídos-----------------------------------------------------------
using namespace std;
#include <Classes.hpp>
#include <complex>

//---------------------------------------------------------------------------
class VTEqpto;
class VTBarra;
class VTLigacao;
class VTBarCC;
class VTLigCC;

//---------------------------------------------------------------------------
enum enumTIPO_FASOR  {fasorCURTO_Ifas=1, fasorCURTO_Iseq, fasorCURTO_Vfas, fasorCURTO_Vseq,
                      fasorFLUXO_Ifas,   fasorFLUXO_Iseq, fasorFLUXO_Vfas, fasorFLUXO_Vseq,
                      fasorANGULO,       fasorAJUSTE,
                      fasorINDEFINIDO=-1
                     };

//---------------------------------------------------------------------------
class VTFasor : public TObject
   {
   public:
                   __fastcall  VTFasor(void) {};
      virtual      __fastcall ~VTFasor(void) {};
      virtual void __fastcall  DefinePatamar(int ind_pat=0) = 0;
      virtual void __fastcall  RedefineBarraReferencia(VTBarra *bar_ref) = 0;
      virtual int  __fastcall  Tipo(void) = 0;
      virtual bool __fastcall  Valor(complex<double> &valor, bool pu=true) {return(false);};
      virtual bool __fastcall  ValorAngulo(double &ang_rad)  {return(false);};
      virtual bool __fastcall  ValorModulo(double &modulo, bool pu=true)   {return(false);};

   public: //dados
      #define    V_KV_NULA    1e-4
      AnsiString codigo;
      bool       enabled;
      bool       visible;
      double     dim, raio;
      bool       draw_seta;
      int        fases;        //Fases.h
      int        ind_seq;      //Fases.h
      int        tipo_curto;   //enumTipoCC
      TColor     color;
      struct     {
                 double x, y;
                 }coord;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTFasor
//---------------------------------------------------------------------------
VTFasor* __fastcall NewObjFasorAjuste(VTFasor  *fasorI, VTFasor *fasorV, char classe);
VTFasor* __fastcall NewObjFasorAngulo(VTFasor  *fasor1, VTFasor *fasor2);
VTFasor* __fastcall NewObjFasorCurto(VTBarra   *barra);
VTFasor* __fastcall NewObjFasorCurto(VTLigacao *ligacao, VTBarra *bar_ref);
VTFasor* __fastcall NewObjFasorFluxo(VTBarra   *barra);
VTFasor* __fastcall NewObjFasorFluxo(VTLigacao *ligacao, VTBarra *bar_ref);

//---------------------------------------------------------------------------
#endif
//eof
