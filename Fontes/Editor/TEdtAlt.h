//---------------------------------------------------------------------------
#ifndef TEdtAltH
#define TEdtAltH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdt.h"

//---------------------------------------------------------------------------
class VTEqbar;
class VTEqpto;
class VTBarra;
class VTBateria;
class VTCapacitor;
class VTCapserie;
class VTCarga;
class VTCargaNL;
class VTChave;
class VTFiltro;
class VTGerador;
class VTMedidor;
class VTMutua;
class VTReator;
class VTReducao;
class VTRegulador;
class VTSuprimento;
class VTTrafo;
class VTTrafo3E;
class VTTrafoZZ;
class VTTrecho;
class VTYref;
class VTMarca;
class VTCaminho;
class VTRedegraf;
class VTMotor;
class VTFigura;
class VTFiguras;

//---------------------------------------------------------------------------
class TEdtAlt : public TEdt
   {
   public:
             __fastcall  TEdtAlt(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
             __fastcall ~TEdtAlt(void);
      TList* __fastcall  LisEqptoSel(void);

   protected:  //métodos
      void __fastcall AvisoEqptoBloqueado(VTEqpto *eqpto);
	  bool __fastcall BuscaEqp(void);
      bool __fastcall BuscaEqp(int x1, int y1, int x2, int y2);
	  bool __fastcall BuscaFig(void);
	  bool __fastcall BuscaFig(int x1, int y1, int x2, int y2);
	  bool __fastcall BuscaMarca(void);
      void __fastcall DefineEstadoSEL0(void);
	  int  __fastcall EditaBarra(VTBarra *pbarra);
	  int __fastcall  EditaBateria(VTBateria *pbat);
      int  __fastcall EditaCapacitor(VTCapacitor *pcap);
      int  __fastcall EditaCapserie(VTCapserie *capserie);
	  int  __fastcall EditaCarga(VTCarga *pcarga);
	  int  __fastcall EditaCargaNL(VTCargaNL *pcarga);
	  int  __fastcall EditaMotor(VTMotor *pmotor);
      int  __fastcall EditaCargaIP(VTCarga *pcarga);
      int  __fastcall EditaChave(VTChave *pchave);
      int  __fastcall EditaEqptos(void);
	  int  __fastcall EditaFigComentario(void);
	  int  __fastcall EditaFigRetangulo(void);
      int  __fastcall EditaFiltro(VTFiltro *filtro);
      int  __fastcall EditaGerador(VTGerador *pger);
      int  __fastcall EditaMedidor(VTMedidor *pmed);
      int  __fastcall EditaMutua(VTMutua *mutua);
      int  __fastcall EditaReator(VTReator *reator);
      int  __fastcall EditaRegulador(VTRegulador *regulador);
      int  __fastcall EditaSuprimento(VTSuprimento *psup);
      int  __fastcall EditaTrafo(VTTrafo *ptrafo);
      int  __fastcall EditaTrafo3E(VTTrafo3E *ptrafo3e);
      int  __fastcall EditaTrafoZZ(VTTrafoZZ *trafoZZ);
      int  __fastcall EditaTrecho(VTTrecho *ptrecho);
      int  __fastcall EditaYref(VTYref *yref);
      void __fastcall EstadoErro_EvtMouseDown(void);
      void __fastcall EstadoErro_EvtMouseDownMove(void);
      void __fastcall EstadoErro_EvtMouseMove(void);
      void __fastcall EstadoErro_EvtMouseUp(void);
      void __fastcall EstadoSel0_EvtMouseDown(void);
      void __fastcall EstadoSel0_EvtMouseDownMove(void);
      void __fastcall EstadoSel0_EvtMouseMove(void);
      void __fastcall EstadoSel0_EvtMouseUp(void);
      void __fastcall EstadoSel1_EvtMouseDown(void);
      void __fastcall EstadoSel1_EvtMouseDownMove(void);
      void __fastcall EstadoSel1_EvtMouseMove(void);
      void __fastcall EstadoSel1_EvtMouseUp(void);
      void __fastcall EstadoSelN_EvtMouseDown(void);
      void __fastcall EstadoSelN_EvtMouseDownMove(void);
      void __fastcall EstadoSelN_EvtMouseMove(void);
      void __fastcall EstadoSelN_EvtMouseUp(void);
      void __fastcall EstadoMove_EvtMouseDown(void);
      void __fastcall EstadoMove_EvtMouseDownMove(void);
      void __fastcall EstadoMove_EvtMouseMove(void);
      void __fastcall EstadoMove_EvtMouseUp(void);
	  void __fastcall EstadoMoveFig_EvtMouseDownMove(void);
	  void __fastcall EstadoMoveFig_EvtMouseUp(void);
      void __fastcall EstadoMoveMarca_EvtMouseDown(void);
      void __fastcall EstadoMoveMarca_EvtMouseDownMove(void);
      void __fastcall EstadoMoveMarca_EvtMouseUp(void);
      void __fastcall EstadoSpin_EvtMouseDown(void);
      void __fastcall EstadoSpin_EvtMouseDownMove(void);
      void __fastcall EstadoSpin_EvtMouseMove(void);
      void __fastcall EstadoSpin_EvtMouseUp(void);
      void __fastcall EvtDelete(void);
      void __fastcall EvtEscape(void);
      void __fastcall EvtMove(bool down, bool left, bool right, bool up);
      void __fastcall EvtMouseDown(void);
      void __fastcall EvtMouseDownMove(void);
      void __fastcall EvtMouseMove(void);
      void __fastcall EvtMouseSelAll(void); //DVK 2014.08.01
      void __fastcall EvtMouseUp(void);
      void __fastcall EvtSelArea(int x1, int y1, int x2, int y2);
      void __fastcall EvtSelEqpto(TList *lisEXT);
      bool __fastcall ExisteBarraDeCorteComReducao(TList *lisEQP);
      bool __fastcall ExisteBarraInicialDeRede(TList *lisEQP);
      int  __fastcall InsereRede(VTReducao *reducao);
      void __fastcall MarcaCancela(void);
      void __fastcall MarcaInicia(void);
	  bool __fastcall PreparaMoveEqpto(void);
	  void __fastcall PreparaMoveFigura(void);
      void __fastcall PreparaMoveMarca(void);
      void __fastcall MontaLisBarraSelecionada(void);
      void __fastcall PreparaSpin(void);
      void __fastcall SelecionaCaminhoEletrico(VTEqpto* eqpto1, VTEqpto *eqpto2, TList *lisEQP);
      bool __fastcall VerificaEqptoBloqueado(TList *lisEQP);
      bool __fastcall VerificaEqptoBloqueado(VTEqpto *eqpto);

   private:  //objetos externos
	   VTFiguras  *figuras;
       VTRedegraf *redegraf;

   protected:  //dados
      enum ESTADO {estERRO, estSEL0, estSEL1, estSELN,
                  estMOVE_MARCA, estMOVE, estSPIN,
                  estFIG_1, estMOVE_FIG} estado;
	  VTCaminho  *caminho;
	  VTEqpto    *eqpto;   //eqpto atualmente selecionado
	  VTFigura   *figura;  //figura atualmente selecionada
	  VTMarca    *marca;   //marca atualmente selecionada
	  TList      *lisBAR;
      struct{
            VTEqpto  *eqpto;       //eqpto localizado por BuscaEqpto
            VTFigura *figura;      //figura localizada por BuscaFigura
            }sel;

   };

#endif
//---------------------------------------------------------------------------
//eof
