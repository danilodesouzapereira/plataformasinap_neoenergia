//---------------------------------------------------------------------------
#ifndef TMontaMedicaoRedeLivroH
#define TMontaMedicaoRedeLivroH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTEqpto;
class VTCanal;
class VTMedidor;

//---------------------------------------------------------------------------
// ESTA CLASSE INSERE OS MEDIDORES DA REDE DE 6 BARRAS DO LIVRO
// WOOD & WOLLENBERG, PÁGINA 477
//---------------------------------------------------------------------------
class TMontaMedicaoRedeLivro : public TObject
   {
   public:  //métodos
                 __fastcall  TMontaMedicaoRedeLivro(VTApl *apl);
                 __fastcall ~TMontaMedicaoRedeLivro(void);
      bool       __fastcall  Executa(bool por_fase);

   private:  //métodos
      void       __fastcall IniciaCurva(VTCanal *canal, double valor);
      bool       __fastcall InsereMedidor(bool por_fase);
      VTMedidor* __fastcall InsereMedidor(VTBarra *barra, AnsiString codigo);
      bool       __fastcall InsereCanal_I(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc, double i_amp);
      bool       __fastcall InsereCanal_P(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc, double p_mw);
      bool       __fastcall InsereCanal_Q(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc, double q_mvar);
      bool       __fastcall InsereCanal_V(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc, double v_pu);
      bool       __fastcall InsereCurva(VTCanal *canal);

   private:  //ojetos externos
      VTApl  *apl;

   private: //dados locais
      TList *lisEQP;
   };

#endif
//---------------------------------------------------------------------------
//eof
