//---------------------------------------------------------------------------
#ifndef TFluxoReducaoH
#define TFluxoReducaoH
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "VTFluxoReducao.h"
#include "..\..\Rede\VTLigacao.h"

//---------------------------------------------------------------------------
class VTCarga;
class VTCorte;
class VTDemanda;
class VTLigacao;
class VTPatamares;
class VTSuprimento;
class VTReducao;

//---------------------------------------------------------------------------
class TFluxoReducao : public VTFluxoReducao
   {
   public:
           __fastcall TFluxoReducao(VTApl *apl);
           __fastcall ~TFluxoReducao(void);
      bool __fastcall Executa(VTRedes *redes, double Sbase);

   private: //métodos
      void __fastcall Executa(VTReducao *reducao);
      void __fastcall FluxoNaLigacaoFicticia(VTReducao *reducao, VTLigacao *pliga_fic);
      void __fastcall ReiniciaFluxo(VTReducao *reducao);
      int  __fastcall SentidoFluxo(double fluxo_mva);
      void __fastcall SomaFluxoEqbar(TList *lisEQP, VTBarra *pbarra, int np, complex<double> &s_mva);
      void __fastcall SomaFluxoEqbarReducao(TList *lisEQP, VTBarra *pbarra, int np, complex<double> &s_mva);
      void __fastcall SomaFluxoLigacao(TList *lisLIG, VTBarra *pbarra, int np, complex<double> &s_mva);

   private: //dados
      double       Sbase;
      VTApl       *apl;
      VTDemanda   *demanda;
      VTPatamares *patamares;
      VTRedes     *redes;
      TList       *lisEQP;
   };

#endif
//---------------------------------------------------------------------------
//eof

 