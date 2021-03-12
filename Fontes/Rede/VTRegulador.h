//---------------------------------------------------------------------------
#ifndef VTReguladorH
#define VTReguladorH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "TLigacao.h"
#include "..\Constante\Fases.h"

//---------------------------------------------------------------------------
enum enumREGT {reguPADRAO=1, reguENERQCT, reguAUTO_BI, reguAUTO_UNI, reguAUTO_FIXA, reguFIXO};  //modelo reguPADRAO obsoleto

//---------------------------------------------------------------------------
class VTRegulador : public TLigacao
   {
   public:  //property
	  __property int    Max_pat  = {                     write=PM_SetMaxPat};
	  __property double Spas_mva = {read=PM_GetSpas_mva, write=PM_SetSpas_mva};

   public:
						   __fastcall  VTRegulador(void) {};
	  virtual              __fastcall ~VTRegulador(void) {};
	  virtual VTRegulador* __fastcall  Clone(void) = 0;
	  virtual void         __fastcall  DefineBarraEntrada(VTBarra *barra_ent) = 0;
	  virtual void         __fastcall  IniciaLisUM(void) = 0;
	  virtual double       __fastcall  InomA(void) = 0;
	  virtual TList*       __fastcall  LisUM(void) = 0;
	  virtual double       __fastcall  NivelTensaoDiretoV(void) = 0;
	  virtual double       __fastcall  NivelTensaoInversoV(void) = 0;
	  virtual int          __fastcall  NumUM(void) = 0;
	  virtual int          __fastcall  Passo(int fase) = 0;
	  virtual double       __fastcall  LarguraBandaMorta_pu(void) = 0;
	  virtual double       __fastcall  LarguraBandaMortaInv_pu(void) = 0;

   protected:  //métodos acessados via property
	  virtual double __fastcall PM_GetSpas_mva(void) = 0;
	  virtual void   __fastcall PM_SetSpas_mva(double s_mva) = 0;
	  virtual void   __fastcall PM_SetMaxPat(int max_pat) = 0;

   public:  //dados
	  int     modelo;      //ver enumREGT
	  int     ligacao;     //ver enumLIGTRF em VTEqpto
	  int     fases;       //estrela ou delta fechado: faseABC
						   //delta aberto: faseA, faseB, fase C (fase comum às 2 unidades)
						   //monofásico: faseAN, faseBN, faseCN, faseABN, faseBCN, faseCAN
	  float   snom;        //MVA
	  strIMP  z0, z1;      //pu de Snom
	  bool    by_pass;     //true: equivale a chave fechada
//    bool    ajuste_auto; //true: utiliza cfg_auto; false: utiliza cfg_fixo  =>   ver enumREGT
	  float   var_tensao;  //% para baixo ou para cima
	  int     num_passo;   //passos para baixo ou para cima
	  int     rel_tap;     //relação de tap
	  struct   {
//             bool    reversivel;
			   VTBarra *bar_ref;   //barra de entrada
			   }sentido;
	  struct   {
			   int    passo; //passo fixo selecionado
			   }cfg_fixo;
	  struct   {
			   VTBarra *bar_ref;     //barra de referência sentido direto
			   VTBarra *bar_ref_inv; //barra de referência sentido inverso (se tipo reguAUTO_FIXA, igual bar_ref)
			   float    v_pu;      //tensão alvo (pu) da Barra de referência direta
			   float    vinv_pu;   //tensão alvo (pu) da Barra de referência inversa
			   float    dv_pu;     //Tolerância de tensão. Nenhum ajuste é feito no tap do regulador
								   //quando a tensão está na faixa (v_pu-dv_pu) e (v_pu+dv_pu)
			   float    larg_bandamorta_pu; 	//largura da banda morta em pu, faixa centrada na tensão alvo
													//que não deve gerar alteração de tap
			   float    larg_bandamorta_inv_pu;  //largura da banda morta no sentido inverso
			   }cfg_auto;
	  TList    *lisUM;             //lista de unidades monofásicas
	  //JCG 2018.04.03 - passo determindo pelo cálculo do fluxo
	  int **passo_calc;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe Regulador
//---------------------------------------------------------------------------
VTRegulador* __fastcall NewObjRegulador(int num_pat);

//---------------------------------------------------------------------------
#endif
//eof

 