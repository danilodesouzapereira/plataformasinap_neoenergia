//---------------------------------------------------------------------------
#ifndef TCrescimentoH
#define TCrescimentoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCrescimento.h"

//---------------------------------------------------------------------------
class TPeriodo;

//---------------------------------------------------------------------------
class TCrescimento : public VTCrescimento
   {
   public:  //funções públicas
                     __fastcall TCrescimento(void);
                     __fastcall ~TCrescimento(void);
      VTCrescimento* __fastcall Clone(void);
      bool           __fastcall InserePeriodo(int ano_ini, int ano_fim, double taxa_pu);
      TList*         __fastcall LisPeriodo(void);
      bool           __fastcall Nulo(void);
      void           __fastcall RedefinePeriodo(int ano_ini, int ano_fim);
      void           __fastcall Reinicia(void);

   protected: //métodos acessados via property
      int        __fastcall PM_GetAnoFinal(void);
      int        __fastcall PM_GetAnoInicial(void);
      VTClasse*  __fastcall PM_GetClasse(void);
      AnsiString __fastcall PM_GetCodigo(void);
      int        __fastcall PM_GetNumPeriodo(void);
      double     __fastcall PM_GetTaxa_perc(int ano);
      double     __fastcall PM_GetTaxa_pu(int ano);
      double     __fastcall PM_GetTaxaAcumulada_pu(int index);
      void       __fastcall PM_SetClasse(VTClasse* classe);
      void       __fastcall PM_SetTaxa_perc(int ano, double taxa);
      void       __fastcall PM_SetTaxa_pu(int ano, double taxa);

   private:  //métodos
      TPeriodo* __fastcall ExistePeriodo(int ano);
      void      __fastcall MontaTaxaAcumulada(void);

   private:  //dados locais
      AnsiString          codigo;
      DynamicArray<float> taxa_acum;   //valor em pu
      TList               *lisPERIODO;
   };

//---------------------------------------------------------------------------
#endif
//eof
