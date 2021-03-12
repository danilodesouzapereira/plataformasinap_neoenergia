//---------------------------------------------------------------------------
#ifndef TPatamaresH
#define TPatamaresH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTPatamares.h"

//---------------------------------------------------------------------------
//class VTApl;
class TPonto;

//---------------------------------------------------------------------------
class TPatamares : public VTPatamares
   {
   public:  //funções públicas
                 __fastcall  TPatamares(VTApl *apl);
                 __fastcall ~TPatamares(void);
      void       __fastcall  CopiaAtributosDe(VTPatamares &patamares);
      void       __fastcall  Enable(VTPatamar *patamar, bool enabled);
      VTPatamar* __fastcall  ExistePatamar(int hora, int minuto);
      TList*     __fastcall  LisPatamar(void);
      void       __fastcall  LisPatamar(TList *lisEXT);
      void       __fastcall  LisPatamarHabilitado(TList *lisEXT);
      int        __fastcall  NumPatamar(void);
      int        __fastcall  NumPatamarHabilitado(void);
      void       __fastcall  operator<<(VTPatamares &patamares);
      VTPatamar* __fastcall  PrimeiroPatamarHabilitado(void);
      void       __fastcall  ReiniciaDiagFluxo(bool fluxo_correto);

   private: //métodos acessados via property
      int        __fastcall PM_GetTipo(void);
      AnsiString __fastcall PM_GetNome(void);
      VTPatamar* __fastcall PM_GetPatamar(int np);
      VTPatamar* __fastcall PM_GetPatamarAtivo(void);
      void       __fastcall PM_SetTipo(int tipo);
      void       __fastcall PM_SetPatamarAtivo(VTPatamar *patamar);

   private: //métodos
      void __fastcall CriaPatamar01HORA(void);
      void __fastcall CriaPatamar02HORA(void);
      void __fastcall CriaPatamar03HORA(void);
      void __fastcall CriaPatamar04HORA(void);
      void __fastcall CriaPatamar06HORA(void);
      void __fastcall CriaPatamar08HORA(void);
      void __fastcall CriaPatamar12HORA(void);
      void __fastcall CriaPatamar24HORA(void);
      void __fastcall CriaPatamar05MIN(void);
      void __fastcall CriaPatamar10MIN(void);
      void __fastcall CriaPatamar15MIN(void);
      void __fastcall CriaPatamarMMTN(void);
      void __fastcall CriaPatamarLMP(void);

   private:  //objetos externos
      VTApl *apl;
      
   protected: //dados acessados via property
      struct   {
               int        tipo;
               AnsiString nome;
               VTPatamar  *patamar_ativo;
               } PD;

   private:  //dados locais
      TList *lisPAT;
   };

//---------------------------------------------------------------------------
#endif
//eof
