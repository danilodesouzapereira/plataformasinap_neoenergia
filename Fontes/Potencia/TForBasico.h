//---------------------------------------------------------------------------
#ifndef TForBasicoH
#define TForBasicoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTFornecimento.h"

//---------------------------------------------------------------------------
class VTCalculaSup;
class VTFases;
class VTGerador;
class VTNo;
class VTSuprimento;
class VTListaSupFase;
class VTPatamares;

//---------------------------------------------------------------------------
class TForBasico : public VTFornecimento
   {
   public:
                      __fastcall  TForBasico(VTApl *apl);
                      __fastcall ~TForBasico(void);
      bool            __fastcall  Get_PQVT_porfase(VTEqpto *eqpto, int np, strSUP vet_sup[MAX_FASE]);
      bool            __fastcall  Get_PQVT_trifasico(VTEqpto *eqpto, int np, strSUP *str_sup);
      bool            __fastcall  Set_PQVT_porfase(VTEqpto *eqpto, int np, strSUP vet_sup[MAX_FASE]);
      bool            __fastcall  Set_PQVT_trifasico(VTEqpto *eqpto, int np, strSUP *str_sup);
      void            __fastcall  ReiniciaCurva(VTEqpto *eqpto, double defasagem_rad);

   protected: //métodos a serem implementados em classes derivadas
      virtual bool __fastcall GetPQVTNo(VTNo *no, int np, strSUP vet_sup[MAX_FASE]) = 0;

   protected: //métodos acessados via property
      bool            __fastcall PM_GetAtivo(VTEqpto  *eqpto, int np);
      double          __fastcall PM_GetP_mw(VTEqpto   *eqpto, int np);
      double          __fastcall PM_GetQ_mvar(VTEqpto *eqpto, int np);
      complex<double> __fastcall PM_GetS_mva(VTEqpto  *eqpto, int np);
      double          __fastcall PM_GetV_pu(VTEqpto   *eqpto, int np);
      double          __fastcall PM_GetT_grau(VTEqpto *eqpto, int np);
      double          __fastcall PM_GetT_rad(VTEqpto  *eqpto, int np);
      void            __fastcall PM_SetP_mw(VTEqpto   *eqpto, int np, double p);
      void            __fastcall PM_SetQ_mvar(VTEqpto *eqpto, int np, double q);
      void            __fastcall PM_SetS_mva(VTEqpto  *eqpto, int np, complex<double> s_mva);
      void            __fastcall PM_SetV_pu(VTEqpto   *eqpto, int np, double v);
      void            __fastcall PM_SetT_grau(VTEqpto *eqpto, int np, double t);
      void            __fastcall PM_SetT_rad(VTEqpto  *eqpto, int np, double t);

   protected: //métodos
      //void            __fastcall CalculaPQVT_porfase(strSUP *str_sup, strSUP vet_sup[MAX_FASE]);
      void            __fastcall CalculaPQVT_porfase(strSUP *str_sup, strSUP vet_sup[MAX_FASE], int fases_eqpto);
      void            __fastcall CalculaPQVT_trifasico(strSUP *str_sup, strSUP vet_sup[MAX_FASE]);
      bool            __fastcall GetPQVTGeradorDeseq(VTGerador *gerador, int np, strSUP vet_sup[MAX_FASE]);
      bool            __fastcall GetPQVTGeradorEquil(VTGerador *gerador, int np, strSUP *str_sup);
      bool            __fastcall GetPQVTSuprimentoDeseq(VTSuprimento *suprimento, int np, strSUP vet_sup[MAX_FASE]);
      bool            __fastcall GetPQVTSuprimentoEquil(VTSuprimento *suprimento, int np, strSUP *str_sup);
      complex<double> __fastcall GetSmvaGerador(VTGerador *gerador, int np);
      complex<double> __fastcall GetSmvaSuprimento(VTSuprimento *suprimento, int np);
      complex<double> __fastcall GetSmvaSuprimentoReducao(VTSuprimento *suprimento, int np);
      bool            __fastcall GetUmValorGerador(VTGerador *gerador, int np, int ind_valor, double &valor);
      bool            __fastcall GetUmValorSuprimento(VTSuprimento *suprimento, int np, int ind_valor, double &valor);
      bool            __fastcall GetUmValorSuprimentoReducao(VTSuprimento *suprimento, int np, int ind_valor, double &valor);
      void            __fastcall ReiniciaCurvaGerador(VTGerador *gerador, double defasagem_rad);
      void            __fastcall ReiniciaCurvaSuprimento(VTSuprimento *suprimento, double defasagem_rad);
      void            __fastcall ReiniciaCurvaSuprimentoReducao(VTSuprimento *suprimento, double defasagem_rad);
      bool            __fastcall SetPQVTGeradorDeseq(VTGerador *gerador, int np, strSUP vet_sup[MAX_FASE]);
      bool            __fastcall SetPQVTGeradorEquil(VTGerador *gerador, int np, strSUP *str_sup);
      bool            __fastcall SetPQVTSuprimentoDeseq(VTSuprimento *suprimento, int np, strSUP vet_sup[MAX_FASE]);
      bool            __fastcall SetPQVTSuprimentoEquil(VTSuprimento *suprimento, int np, strSUP *str_sup);
      bool            __fastcall SetSmvaGerador(VTGerador *gerador, int np, complex<double> s_mva);
      bool            __fastcall SetSmvaSuprimento(VTSuprimento *suprimento, int np, complex<double> s_mva);
      bool            __fastcall SetSmvaSuprimentoReducao(VTSuprimento *suprimento, int np, complex<double> s_mva);
      bool            __fastcall SetUmValorGerador(VTGerador *gerador, int np, int ind_valor, double &valor);
      bool            __fastcall SetUmValorSuprimento(VTSuprimento *suprimento, int np, int ind_valor, double &valor);
      bool            __fastcall SetUmValorSuprimentoReducao(VTSuprimento *suprimento, int np, int ind_valor, double &valor);

   protected: //objetos externos
      VTApl            *apl;
      VTFases          *fases;
      VTPatamares *patamares;

   protected: //dados locais
      enum         indVALOR {indP=0, indQ, indV, indT};
      VTCalculaSup *calcula;
   };

#endif
//---------------------------------------------------------------------------
//eof
