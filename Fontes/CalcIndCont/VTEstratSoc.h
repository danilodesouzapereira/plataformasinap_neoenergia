//---------------------------------------------------------------------------
#ifndef VTEstratSocH
#define VTEstratSocH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTVisRede;
class VTBlocoRad;
class VTBloco;
class VTDefeito;
//---------------------------------------------------------------------------
class VTEstratSoc: public TObject
	{
	public:  //property
		__property int           NBlocosSoc          = {read=PM_GetNBlocosSoc};
		__property VTDefeito*    Defeito             = {read=PM_GetDefeito, write=PM_SetDefeito};

	public:
							__fastcall VTEstratSoc(void) {};
							__fastcall ~VTEstratSoc(void) {};
	  virtual bool    __fastcall AdicionaVisRedeSoc(VTVisRede *visRede, TList *lisBlocosSoc, TList *lisChvJus) = 0;
	  virtual bool    __fastcall AdicionaVisRedeSoc1(VTVisRede *visRede, VTBlocoRad *blRadRapida, VTBlocoRad *blRadLenta=NULL) = 0;
	  virtual bool    __fastcall BlocoSoc(VTBloco *bloco) = 0;
	  virtual void    __fastcall Inicia(void) = 0;
	  virtual void    __fastcall ListaVisRede(TList *lisEXT) = 0;
	  virtual void    __fastcall ListaRedeTransf(TList *lisEXT) = 0;
	  virtual void    __fastcall ListaChvIsola(TList *lisEXT) = 0;

	protected: //m�todos acessados via property
	  virtual int         __fastcall PM_GetNBlocosSoc(void) = 0;
	  virtual VTDefeito*  __fastcall PM_GetDefeito(void) = 0;
	  virtual void        __fastcall PM_SetDefeito(VTDefeito* pdef) = 0;

	public:
	 int npat; //patamar da manobra
	 TList *lisVisRedeSoc;
	 TList *lisBlocosSoc;
	 TList *lisBlocosReparo;   //lista de blocos n�o socorridos, que permanecem em reparo
	 double    cargaMw_Transf; //carga transferida
	 double    iAmp_Transf;    //Corrente transferida
	 double    iAmp_Ligacao;   //Corrente existente no trecho cr�tico
	 double    iAmp_AdmLiga;   //Corrente admissivel no trecho critico
	 double    puTransf;       //valor m�nimo caso n�o haja transfer�ncia
	};

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto
//---------------------------------------------------------------------------
VTEstratSoc* __fastcall NewObjEstratSoc(int npat);

//---------------------------------------------------------------------------
#endif

