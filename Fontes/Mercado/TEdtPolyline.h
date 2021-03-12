//---------------------------------------------------------------------------
#ifndef TEdtPolylineH
#define TEdtPolylineH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdt.h"
#include "..\Rede\Estrutura.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTTrecho;
class VTRedegraf;
class TFormEdtPolyline;

//---------------------------------------------------------------------------
class TEdtRegiao : public TEdt
   {
   public:
            __fastcall  TEdtRegiao(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone, TWinControl *parent);
            __fastcall ~TEdtRegiao(void);
      void  __fastcall  IniciaNovaSequenciaTrechos(void);

   private:  //métodos
      VTBarra*  __fastcall BuscaBarra(void);
      void      __fastcall EstimaCoordEsquematico(strUTM &utm, strESQ_BARRA &esq);
      void      __fastcall EstimaCoordUtm(strUTM &utm, strESQ_BARRA &esq);
      void      __fastcall EvtEnter(void);
      void      __fastcall EvtMouseDown(void);
      void      __fastcall EvtMouseDownMove(void);
      void      __fastcall EvtMouseMove(void);
      void      __fastcall EvtMouseUp(void);
      void      __fastcall InsereBarraExistente(VTBarra *barra);
      VTBarra*  __fastcall InsereBarraNova(void);
      VTTrecho* __fastcall InsereTrechoNovo(void);
      void      __fastcall LisEqptoNovoApaga(void);
      void      __fastcall LisEqptoNovoExibe(void);
      bool      __fastcall ValidaDadosTrecho(void);

   private:  //objetos externos
      VTRedegraf *redegraf;

   private:  //dados
      bool             evt_mouse_enabled;
      TFormEdtPolyline *FormEdtPolyline;
      VTBarra          *barra_sel;
      TList            *lisBARRA;
      struct   {
               VTBarra  *barra;
               VTTrecho *trecho;
               }rastro;
   };

#endif
//---------------------------------------------------------------------------
//eof
