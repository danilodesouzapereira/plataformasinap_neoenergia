//---------------------------------------------------------------------------
#ifndef TEdtH
#define TEdtH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEdt.h"

//---------------------------------------------------------------------------
class VTArea;
class VTBusca;
class VTEdita;
class VTGeral;
class VTGrafico;
class VTRedes;
class VTPatamares;

//---------------------------------------------------------------------------
class TEdt : public VTEdt
   {
   public:
                     __fastcall TEdt(VTApl *apl, TCursor cursor, TAction *ResetFocus,
                                     TAction *SetFocus, TAction *OnDone);
      virtual        __fastcall ~TEdt(void);
      virtual void   __fastcall CoordMouse(int &x, int &y);
      virtual void   __fastcall Done(void);
      virtual void   __fastcall EvtEnter(void);
      virtual void   __fastcall EvtEscape(void);
      virtual void   __fastcall EvtDelete(void);
      virtual void   __fastcall EvtMouse(int Action, TMouseButton Button, TShiftState Shift,
                                int x_utm, int y_utm, int x, int y);
      virtual void   __fastcall EvtMove(bool down, bool left, bool right, bool up);
      virtual void   __fastcall EvtMouseSelAll(void); //DVK 2014.08.01
      virtual void   __fastcall EvtSelArea(int x1, int y1, int x2, int y2);
      virtual void   __fastcall EvtSelEqpto(TList *lisEXT);
      virtual TList* __fastcall LisEqptoNovo(void);
      virtual TList* __fastcall LisEqptoSel(void);
      virtual TList* __fastcall LisMarca(void);
      virtual void   __fastcall MarcaCancela(void);
      virtual void   __fastcall ResetGrafFocus(void);
      virtual void   __fastcall SetGrafFocus(void);

   protected:  //métodos
      virtual void __fastcall EvtMouseDown(void);
      virtual void __fastcall EvtMouseDownMove(void);
      virtual void __fastcall EvtMouseMove(void);
      virtual void __fastcall EvtMouseUp(void);

   protected:  //dados
      TList       *lisEQN; //equipamentos novos
      TList       *lisEQS; //equipamentos selecionados
      TList       *lisMAR; //marcas
      VTArea      *area;
      VTApl       *apl;
      VTBusca     *busca;
      VTEdita     *edita;
      VTGeral     *geral;
      VTGrafico   *graf;
      VTPatamares *patamares;
      VTRedes     *redes;
      struct   {//estrutura c/ Actions externa
               TAction     *OnDone;      //Action a ser executada após uma operação
               TAction     *ResetFocus;  //perda do controle de mouse
               TAction     *SetFocus;    //ganho do controle de mouse
               } extAction;
      
      struct   {//estrutura p/ salvar dados de evento de mouse
               int          x_ant, y_ant;
               int          Action;
               TMouseButton Button;
               TShiftState  Shift;
               int          x, y;
               struct {int x, y;} coord_pixel;
               }evt;
   };

#endif
//---------------------------------------------------------------------------
//eof
