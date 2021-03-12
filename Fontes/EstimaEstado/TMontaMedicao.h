//---------------------------------------------------------------------------
#ifndef TMontaMedicaoH
#define TMontaMedicaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTChave;
class VTEqpto;
class VTCanal;
class VTMedidor;

//---------------------------------------------------------------------------
class TMontaMedicao : public TObject
   {
   public:  //métodos
                 __fastcall  TMontaMedicao(VTApl *apl);
                 __fastcall ~TMontaMedicao(void);
      bool       __fastcall  Executa(bool por_fase);

   private:  //métodos
      int        __fastcall IniciaLisChave(void);
      int        __fastcall IniciaLisMedidor(void);
      bool       __fastcall InsereMedidor(bool por_fase);
      VTMedidor* __fastcall InsereMedidor(VTBarra *barra, AnsiString codigo="");
      bool       __fastcall InsereCanaisPQ_porfase(VTMedidor *medidor, VTChave* chave);
      bool       __fastcall InsereCanaisPQ_trifasico(VTMedidor *medidor, VTChave* chave);
      VTCanal*   __fastcall InsereCanal_P(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc);
      VTCanal*   __fastcall InsereCanal_Q(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc);
      bool       __fastcall InsereCurva(VTCanal *canal);

   private:  //ojetos externos
      VTApl  *apl;

   private: //dados locais
      TList *lisCHV;
      TList *lisEQP;
      TList *lisMED;
   };

#endif
//---------------------------------------------------------------------------
//eof
