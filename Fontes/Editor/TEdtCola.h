//---------------------------------------------------------------------------
#ifndef TEdtColaH
#define TEdtColaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdt.h"

//---------------------------------------------------------------------------
class VTArea;
class VTBarra;
class VTEqpto;
class VTRede;

//---------------------------------------------------------------------------
class TEdtCola : public TEdt
   {
   public:  //property
      __property VTEdt*  EdtCopia = {read=PD.edt_copia, write=PM_SetEdtCopia};
      //__property VTArea* AreaRede = {read=PD.area_rede, write=PD.area_rede};

   public:
          __fastcall TEdtCola(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
          __fastcall ~TEdtCola(void);

   private:  //métodos acessados via property
      void __fastcall PM_SetEdtCopia(VTEdt *edt);

   private:  //métodos
      void     __fastcall EstimaCoordEsquematico(void);
      void     __fastcall EstimaCoordUtm(void);
      void     __fastcall EvtEscape(void);
      void     __fastcall EvtMouseDown(void);
      void     __fastcall EvtMouseMove(void);
      VTEqpto* __fastcall ExisteEqpto(int eqpto_id, TList *lisEQP);
      bool     __fastcall ExisteEqpto(VTEqpto *eqpto, TList *lisEQP);
      bool     __fastcall ExisteRede(int id, AnsiString codigo, TList *lisEQP);
      void     __fastcall CopiaEqptos(void);
      void     __fastcall IniciaLisEqptoOriginal(void);
      void     __fastcall MoveAtualizaCoordEsquematico(void);
      void     __fastcall MoveAtualizaCoordUtm(void);
      void     __fastcall RedefineCoordenadas(void);
      void     __fastcall SalvaArea(void);
      void     __fastcall SalvaAreaCoordEsquematico(void);
      void     __fastcall SalvaAreaCoordUtm(void);

   private:  //cados acessados via property
      struct   {
               VTEdt  *edt_copia;
               //VTArea *area_rede;
               }PD;

   private:  //dados
      bool  first_move;
      struct   {
               int x1, y1, x2, y2;
               }coord;
      TList *lisBAR;
      TList *lisEQP;
      TList *lisREDE;
      TList *lisCLUSTER;
   };

#endif
//---------------------------------------------------------------------------
//eof
