//---------------------------------------------------------------------------
#ifndef VTVisRedeH
#define VTVisRedeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTBarra;
class VTBloco;
class VTChave;
class VTRede;
class VTEqpto;//class VTLigacao;
class VTLigaCrit;
class VTBlocoRad;
class VTLisBlocoChave;
//---------------------------------------------------------------------------
struct strDelta : TObject
					 {
					 VTEqpto *pliga;
					 int    ipat;
					 double ia_nom; //corrente admissível de um dos cabos
					 double ifluxo;
					 double dMax;
					 };
//---------------------------------------------------------------------------
class VTVisRede: public TObject
	{
	public:  //property
//		__property VTBlocoRad*   BlocoRadMon         = {read=PM_GetBlocoRadMon, write=PM_SetBlocoRadMon};


	public:
									 __fastcall VTVisRede(void) {};
									 __fastcall ~VTVisRede(void) {};
			  virtual  void    __fastcall IniciaDadosDefeito(void) = 0;
			  virtual  double  __fastcall FolgaIA(int indPat) = 0;
			  virtual  double  __fastcall IA_Crit(int indPat) = 0;
			  virtual VTEqpto* __fastcall LigaCrit(int indPat) = 0;

	protected: //métodos acessados via property
//	  virtual VTBlocoRad*  __fastcall PM_GetBlocoRadMon(void) = 0;
//	  virtual void         __fastcall PM_SetBlocoRadMon(VTBlocoRad* blocoRadMon) = 0;

	public:
		VTChave         *pchave;
		VTLisBlocoChave *plisBlocoChave;
		//Valores pré-calculados
		VTRede     *rede1;
		VTRede     *rede2;
		VTBarra    *pbarra1;
		VTBarra    *pbarra2;
		VTBloco    *bloco1;
		VTBloco    *bloco2;
		VTBlocoRad *blocoRad1;
		VTBlocoRad *blocoRad2;


		TList      *lisLiga1,  *lisLiga2; //Lista de lig. ao suprimento da rede1 e da rede2
		TList      *lisBlocosVis1, *lisBlocosVis2;//lista de blocos da chave vis ao suprimento
		VTLigaCrit *ligaCrit1, *ligaCrit2; //Objetos que aponta


		//Variáveis em função do defeito
		VTBloco   *blocoDesl;            //bloco ligado a chave de vis do lado do circuito em falha
		VTBlocoRad *blocoRadDesl;         //blocoRad ligado a chave de vis do lado do circuito em falha
      VTBlocoRad *blocoRadSup;          //blocoRad de suprimento
		TList     *lisBlocosDefVis;      //Lista de blocos a justante do defeito até chave vis (nao inclui o bloco de defeito)
		TList     *lisBlocoRadIsolaJus;  //lista de blocosRad que apontam para chaves de isolamento da jusante para a montante até chvJus
		VTChave   *chvJus;               //chave imediatamente a jusante do blocos de defeito que liga a chave de vis
		VTBlocoRad *blRadJusDef;         //blocoRad imediatamente a justante do defeito, 1o elemento de lisBlocoRadIsolaJus
		TList     *lisChvBlocoVis;       //lista de chaves no circuito de vis até o suprimento de socorro
		TList     *lisChvAlivioVis;      //lista de chaves de alivio no circuito de vis até os suprimento de socorro
		TList     *lisBlocosVis;         //lista de blocos radiais da chave vis ao suprimento


		double    deltaDEC;              //variável auxiliar para ordencao das chaves

	};

//---------------------------------------------------------------------------
//função global p/ criar objeto
//---------------------------------------------------------------------------
VTVisRede* __fastcall NewObjVisRede(VTChave* pchave);

//---------------------------------------------------------------------------
#endif

