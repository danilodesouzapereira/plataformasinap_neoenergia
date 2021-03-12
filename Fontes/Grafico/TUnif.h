//---------------------------------------------------------------------------
#ifndef TUnifH
#define TUnifH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTUnif.h"

//---------------------------------------------------------------------------
class VTArea;
class VTBarra;
class VTBateria;
class VTCarga;
class VTCargaNL;
class VTCapserie;
class VTCapacitor;
class VTChave;
class VTEdita;
class VTEqbar;
class VTFiltro;
class VTGerador;
class VTLigacao;
class VTMedidor;
class VTMotor;
class VTMutua;
class VTReator;
class VTRede;
class VTRedes;
class VTReducao;
class VTRegulador;
class VTSuprimento;
class VTTrafo;
class VTTrafo3E;
class VTTrafoZZ;
class VTTrecho;
class VTYref;
class VTConsulta;
class VTDraw;
class VTGeral;
class VTGrid;
class VTRedegraf;
class VTRegioes;

//---------------------------------------------------------------------------
class TUnif : public VTUnif
   {
   public:
                    __fastcall TUnif(VTApl *apl, int tipo_graf, AnsiString nome);
                    __fastcall ~TUnif(void);
      virtual void  __fastcall Animacao(void);
      virtual void  __fastcall DefObjAnimacao(VTAnimacao *animacao);
      virtual void  __fastcall DefObjFormGraf(TFormGraf *form);
      virtual void  __fastcall DefObjPadrao(TPadrao *padrao);
      virtual void  __fastcall DestacaEqpto(TList *lisEQP, TColor color, int dim);
      virtual void  __fastcall LimpaTela(void);
      virtual void  __fastcall Moldura(TList *lisEQP);
      virtual void  __fastcall MolduraSolida(TList *lisEQP, TColor color);
      //virtual void  __fastcall Redraw(VTEdt *editor);
      //virtual void  __fastcall Redraw(int op, VTEdt *editor);
      virtual int   __fastcall Tipo(void);

   protected: //métodos que podem ser redefinidos nas classes derivadas
      virtual TColor __fastcall Color(VTEqpto *eqpto, TColor cor);
      virtual void   __fastcall HabilitaLegenda(void);
      virtual TList* __fastcall LisEqptoRede(int eqp_tipo=-1);
      virtual TList* __fastcall LisEqptoRede(VTRede *rede, int eqp_tipo=-1);
      virtual TList* __fastcall LisEqptoReducao(VTReducao *reducao, int eqp_tipo=-1);
      virtual void   __fastcall RedrawAnimacao(void);
      virtual void   __fastcall RedrawAll(void);
      virtual void   __fastcall RedrawAllBarra(void);
	  //virtual void   __fastcall RedrawAllBarraIsolada(void);
	  virtual void   __fastcall RedrawAllBateria(void);
      virtual void   __fastcall RedrawAllCapacitor(void);
      virtual void   __fastcall RedrawAllCapserie(void);
      virtual void   __fastcall RedrawAllCarga(void);
      virtual void   __fastcall RedrawAllCargaNL(void);
      virtual void   __fastcall RedrawAllChave(void);
      virtual void   __fastcall RedrawAllEqbar(void);
      //virtual void   __fastcall RedrawAllEqbarIsolado(void);
	  virtual void   __fastcall RedrawAllGerador(void);
	  virtual void   __fastcall RedrawAllFigura(void);
      virtual void   __fastcall RedrawAllFiltro(void);
      virtual void   __fastcall RedrawAllLigacao(bool chave_enabled=true);
      //virtual void   __fastcall RedrawAllLigacaoIsolada(void);
      virtual void   __fastcall RedrawAllMedidor(void);
      virtual void   __fastcall RedrawAllMotor(void);
      virtual void   __fastcall RedrawAllMutua(void);
      virtual void   __fastcall RedrawAllReator(void);
      virtual void   __fastcall RedrawAllRedeReduzida(void);
      virtual void   __fastcall RedrawAllRegulador(void);
      virtual void   __fastcall RedrawAllSuprimento(void);
      virtual void   __fastcall RedrawAllTrafo(void);
      virtual void   __fastcall RedrawAllTrafo3E(void);
      virtual void   __fastcall RedrawAllTrafoZZ(void);
      virtual void   __fastcall RedrawAllTrecho(void);
      virtual void   __fastcall RedrawAllZref(void);
      virtual void   __fastcall RedrawBarra(VTBarra *pbar, TColor cor);
	  virtual void   __fastcall RedrawBarraSwing(VTBarra *pbar);
	  virtual void   __fastcall RedrawBateria(VTBateria *pbateria, TColor cor);
      virtual void   __fastcall RedrawBordas(void);
      virtual void   __fastcall RedrawCapacitor(VTCapacitor *pcap, TColor cor);
      virtual void   __fastcall RedrawCapserie(VTCapserie *capserie, TColor cor);
      virtual void   __fastcall RedrawCarga(VTCarga *pcar, TColor cor);
      virtual void   __fastcall RedrawCargaNL(VTCargaNL *cargaNL, TColor cor);
      virtual void   __fastcall RedrawChave(VTChave *pchv, TColor cor);
      virtual void   __fastcall RedrawFiltro(VTFiltro *filtro, TColor cor);
      virtual void   __fastcall RedrawGerador(VTGerador *pger, TColor cor);
      virtual void   __fastcall RedrawGrid(void);
      virtual void   __fastcall RedrawMapaTematico(void);
      virtual void   __fastcall RedrawMedidor(VTMedidor *medidor, TColor cor);
      virtual void   __fastcall RedrawMotor(VTMotor *motor, TColor cor);
      virtual void   __fastcall RedrawMutua(VTMutua *mutua, TColor cor);
      virtual void   __fastcall RedrawReator(VTReator *reator, TColor cor);
      virtual void   __fastcall RedrawRedeEsquematico(VTRede *rede, TColor cor);
      virtual void   __fastcall RedrawReducao(VTReducao *reducao, TColor cor);
      virtual void   __fastcall RedrawReducaoEqptos(VTReducao *reducao, TColor cor);
      virtual void   __fastcall RedrawReducaoEsquematico(VTReducao *reducao, TColor cor);
      virtual void   __fastcall RedrawRegiao(void);
      virtual void   __fastcall RedrawRegulador(VTRegulador *regulador, TColor cor);
      virtual void   __fastcall RedrawSuprimento(VTSuprimento *psup, TColor cor);
      virtual void   __fastcall RedrawTrafo(VTTrafo *ptrf, TColor cor);
      virtual void   __fastcall RedrawTrafo3E(VTTrafo3E *ptrf, TColor cor);
      virtual void   __fastcall RedrawTrafoZZ(VTTrafoZZ *trafoZZ, TColor cor);
      virtual void   __fastcall RedrawTrecho(VTTrecho *ptre, TColor cor);
      virtual void   __fastcall RedrawYref(VTYref *yref, TColor cor);
      virtual bool   __fastcall ValidaEqptoPlaneja(VTEqpto *eqpto);

   protected:  //métodos acessados via property
       AnsiString __fastcall PM_GetNome(void);
       void       __fastcall PM_SetAnimacaoEnabled(bool enabled);
       void       __fastcall PM_SetMapaTematicoEnabled(bool enabled);

   protected: //dados
      int        tipo_graf;
      AnsiString nome;
      VTAnimacao *animacao;
      VTApl      *apl;
      VTEdita    *edita;
      VTGeral    *geral;
      VTGrid     *grid;
      VTRedes    *redes;
      TFormGraf  *form;
      VTEdt      *editor;
      TPadrao    *padrao;
      VTDraw     *draw;
      VTArea     *area;
      VTRedegraf *redegraf;
      VTConsulta *consulta;
      VTRegioes  *regioes;
      struct   {
               bool apagar;
               int  postre;
               } redraw_animacao;
      struct   {
               bool enabled;
               } mapa_tematico;
   };

//---------------------------------------------------------------------------
#endif
//eof
