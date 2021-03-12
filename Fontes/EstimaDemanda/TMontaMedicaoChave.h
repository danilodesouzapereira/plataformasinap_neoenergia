//---------------------------------------------------------------------------
#ifndef TMontaMedicaoChaveH
#define TMontaMedicaoChaveH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
enum enumTIPO_MEDICAO {medicaoI=1, medicaoPQ, medicaoV};

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTChave;
class VTEqpto;
class VTCanal;
class VTMedidor;

//---------------------------------------------------------------------------
class TMontaMedicaoChave : public TObject
   {
   public:  //properties
      __property bool   DJ_Enabled      = {read=PD.dj_enabled,         write=PD.dj_enabled};
      __property bool   RA_Enabled      = {read=PD.ra_enabled,         write=PD.ra_enabled};
      __property double FatMultMedChave = {read=PD.fat_mult_med_chave, write=PD.fat_mult_med_chave};

   public:  //métodos
                 __fastcall  TMontaMedicaoChave(VTApl *apl);
                 __fastcall ~TMontaMedicaoChave(void);
      bool       __fastcall  Executa(int tipo_medicao, bool por_fase);
      bool       __fastcall  RetiraMedicao(void);

   private:  //métodos
      void       __fastcall HabilitaDesabilitaCanais(void);
      int        __fastcall IniciaLisChave(void);
      int        __fastcall IniciaLisMedidor(void);
      bool       __fastcall InsereMedidor(int tipo_medicao, bool por_fase);
      VTMedidor* __fastcall InsereMedidor(VTBarra *barra, AnsiString codigo="");
      bool       __fastcall InsereCanaisI_porfase(VTMedidor *medidor, VTChave* chave);
      bool       __fastcall InsereCanaisI_trifasico(VTMedidor *medidor, VTChave* chave);
      bool       __fastcall InsereCanaisPQ_porfase(VTMedidor *medidor, VTChave* chave);
      bool       __fastcall InsereCanaisPQ_trifasico(VTMedidor *medidor, VTChave* chave);
      bool       __fastcall InsereCanaisV_porfase(VTMedidor *medidor, VTChave* chave);
      VTCanal*   __fastcall InsereCanal_I(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc);
      VTCanal*   __fastcall InsereCanal_P(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc);
      VTCanal*   __fastcall InsereCanal_Q(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc);
      VTCanal*   __fastcall InsereCanal_V(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc);
      bool       __fastcall InsereCurva(VTCanal *canal);

   private: //objetos externos
      VTApl  *apl;

   private: //dados acessados via property
      struct{
            bool   dj_enabled;
            bool   ra_enabled;
            double fat_mult_med_chave;
            }PD;

   private: //dados locais
      // #define FATMULT    0.8
      // #define FATMULT    1.0
      // #define FATMULT    1.1
      // #define FATMULT    1.2
      TList  *lisCHV;
      TList  *lisEQP;
      TList  *lisMED_ALL;
      TList  *lisMED_NEW;
   };

#endif
//---------------------------------------------------------------------------
//eof
