//---------------------------------------------------------------------------
#ifndef TPerdaregH
#define TPerdaregH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTPerdareg.h"

//---------------------------------------------------------------------------
class TPerdareg : public VTPerdareg
   {
   public:
            __fastcall  TPerdareg(VTApl *apl_owner);
            __fastcall ~TPerdareg(void);
      void  __fastcall  ClearMontaRede(void);
      bool  __fastcall  Executa(TDateTime data, bool batch = false);
      bool  __fastcall  ImportaMedicao(TDateTime data, TStrings *files);
      bool  __fastcall  PreparaRede(TDateTime data);
      //bool  __fastcall  TESTE_IniciaMedicoes(int num_dias);

   private: //métodos
      bool __fastcall CalculaPerdaRegulatoria(TDateTime data);
      bool __fastcall CalculaPerdaTecnicaDiaUtil(TDateTime data);
      bool __fastcall CalculaPerdaTecnicaDomingo(TDateTime data);
      bool __fastcall CalculaPerdaTecnicaSabado(TDateTime data);
      void __fastcall Clear(void);
	  bool __fastcall ExisteMedicao(void);
	  int __fastcall MensagemErroMedicoes(void);
      bool __fastcall MontaArvoreResumoPerdaRegulatoria(void);
      void __fastcall DeterminaDiasMes(TDateTime data);
      bool __fastcall SalvaPerdaTecnicaNaMedicao(void);
      bool __fastcall SelecionaCurvaTipica(int dia);

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      bool	avisoON;
      struct{
            VTApl     *apl;
            } sabado, domingo, diautil;
      struct{
            int sab;
            int dom;
            int dut;
            int mes;
			} num_dias;
	  #define max_ERROAJUSTE 0.01
	  #define max_ITAJUSTE 7
   };

#endif
//---------------------------------------------------------------------------
//eof
