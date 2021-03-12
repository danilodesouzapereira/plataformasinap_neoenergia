//---------------------------------------------------------------------------
#ifndef TAlocaRTH
#define TAlocaRTH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAlocaRT.h"

//---------------------------------------------------------------------------
class VTBanco;
class VTBancos;
class VTBarra;
class VTCarga;
class VTRede;
class VTLigacao;
class VTRegulador;
class VTPrimario;
class VTTronco;
class VTRadial;
class VTProgresso;

//---------------------------------------------------------------------------
class TAlocaRT : public VTAlocaRT
   {
   public:
            __fastcall  TAlocaRT(VTApl *apl);
            __fastcall ~TAlocaRT(void);
      bool  __fastcall  Executa(TList *lisTabReg);
      bool  __fastcall  InsereBancosAlocadosNaRede(void);
      int   __fastcall  ListaReguladorAlocado(TList *lisEXT);
      void  __fastcall  Reinicia(void);
		bool  __fastcall  RetiraBancosAlocadosDaRede(void);
		bool  __fastcall  RetiraBancosAlocadosDaRede(TList *lisReg, bool remove_banco); //DVK 2014.02.11

   private: //método da classe
            static int __fastcall ComparaSnom(void *cap1, void *cap2);

   private:
      void         __fastcall AcumulaQFilho(VTLigacao* ligacao, VTBarra* barra_filho);
      bool         __fastcall AlocaRT(VTPrimario* primario);
      bool         __fastcall AlocaRTPorLigacao(VTPrimario* primario, TList *lisLig, bool op_tronco);
      VTBarra*     __fastcall BarraIn(VTLigacao* ligacao);
      VTBarra*     __fastcall BarraOut(VTLigacao* ligacao);
      void         __fastcall BuscaLigacaoAjusante(VTLigacao *ligPai, TList *lisLigJus);
		void         __fastcall DeleteObject(TObject *obj);
      VTLigacao*   __fastcall DeterminaLigTronco(VTLigacao* ligacao);
		int          __fastcall DeterminaPico(void);
		double 		 __fastcall Distancia(TList *lisCAMINHO);
      double       __fastcall DvPerTap(void);
      bool         __fastcall ExisteFluxo(VTLigacao *ligacao);
		VTLigacao*   __fastcall ExisteLigacaoMontanteComRamal(VTLigacao *lig_ref);
		int          __fastcall IndexFluxoPositivo(VTLigacao *ligacao);
      //bool         __fastcall InsereRegulador(VTRede *rede, VTLigacao *lig, VTRegulador* novo_reg);
      TList*       __fastcall LisRamal(void);
      TList*       __fastcall LisTronco(void);
      VTBanco*     __fastcall MontaNovoBanco(VTLigacao *ligacao);
      void         __fastcall MoveLigacoesB1B2(TList *lisLig, VTBarra *b_orig, VTBarra *b_dest);
      void         __fastcall ResetResflow(void);
      VTRegulador* __fastcall SelecionaModeloRegulador(double s_mva);
      void         __fastcall SetGanhoAjusante(VTBanco *banco);
		double       __fastcall SetTapAdequado(VTRegulador *reg, double vpu);
		bool 			 __fastcall VerificaDistanciaMinima(VTLigacao *ligacao);

	private:
	/*
      int     __fastcall PM_GetMAX_REG(void);
      bool    __fastcall PM_GetOP_TRONCO(void);
      double  __fastcall PM_GetV_ALVO(void) ;
      double  __fastcall PM_GetV_MIN(void) ;

      void    __fastcall PM_SetMAX_REG(int value);
      void    __fastcall PM_SetOP_TRONCO(bool value) ;
      void    __fastcall PM_SetV_ALVO(double value) ;
      void    __fastcall PM_SetV_MIN(double value) ;
	*/
   private: //objetos externos
      VTApl      *apl;

   private: //objetos locais
		TList       *lisEQP;
      TList       *lisEQP_ALT;
      TList       *lisEQP_INS;
		TList       *lisPri;
      TList       *lisTabReg;       //lista de reguladores disponiveis
      TList       *lisBar, *lisLig; //listas ordenadas
      TList       *lisRegClone;
      TList       *lisTmp;
      VTBancos    *bancos;
      VTTronco    *tronco;
      VTRadial    *radial;
      VTProgresso *prog;

	private:
	/*
		bool   op_tronco;  //opção de alocar no tronco
		int    max_reg;
      double v_min;      //pu tensão minima a ser considerado
		double v_alvo;     //pu tensão a ser controlada
		double dist_m;     //distancia minima para alocar, em metros
	*/
		double fator;
		int    indPatMax; //patamar de carregamento máximo
   };

//---------------------------------------------------------------------------
#endif
//eof

