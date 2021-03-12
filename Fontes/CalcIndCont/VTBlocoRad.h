//---------------------------------------------------------------------------
#ifndef VTBlocoRadH
#define VTBlocoRadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTChave;
class VTBloco;
class VTRede;
class VTLisBlocoChave; //hk201611
//---------------------------------------------------------------------------
class VTBlocoRad: public TObject
	{
	public:  //property
		__property VTBloco*      Bloco               = {read=PM_GetBloco};
		__property VTBlocoRad*   BlocoRadMon         = {read=PM_GetBlocoRadMon, write=PM_SetBlocoRadMon};
		__property VTChave*      Chave               = {read=PM_GetChave, write=PM_SetChave};
		__property VTBarra*      BarraChave          = {read=PM_GetBarraChave, write=PM_SetBarraChave};//hk201802a
		__property VTLisBlocoChave* LisBlocoChave    = {read=PM_GetLisBlocoChave, write=PM_SetLisBlocoChave};
		__property TList*        LisBlocoRad         = {read=PM_GetLisBlocoRad};
		__property TList*        LisBlocoJus         = {read=PM_GetLisBlocoJus};
		__property TList*        LisBlocoRadJus      = {read=PM_GetLisBlocoRadJus};
		__property TList*        LisBlocoJusProt     = {read=PM_GetLisBlocoJusProt};
		__property TList*        LisBlocoMon         = {read=PM_GetLisBlocoMon};
		__property int           NumPat              = {read=PM_GetNumPat};
		__property int           NCons               = {read=PM_GetNCons, write=PM_SetNCons};
		__property int           NConsJus            = {read=PM_GetNConsJus, write=PM_SetNConsJus};//hk201902
		__property double        CargaBlRad[int iPat]= {read=PM_GetCargaBlRad, write=PM_SetCargaBlRad};
		__property double        CargaBloco[int iPat]= {read=PM_GetCargaBloco, write=PM_SetCargaBloco};
		__property bool          FlagSup             = {read=PM_GetFlagSup, write=PM_SetFlagSup};
		__property VTRede*       Rede                = {read=PM_GetRede, write=PM_SetRede};


	public:
									__fastcall VTBlocoRad(void) {};
									__fastcall ~VTBlocoRad(void) {};
				 virtual void  __fastcall IniciaCarga(int numPat) = 0;
				 virtual void  __fastcall LisBlRadJus(TList *lisEXT) = 0;

	protected: //métodos acessados via property
	  virtual VTBloco*     __fastcall PM_GetBloco(void) = 0;
	  virtual VTBlocoRad*  __fastcall PM_GetBlocoRadMon(void) = 0;
	  virtual void         __fastcall PM_SetBlocoRadMon(VTBlocoRad* blocoRadMon) = 0;
	  virtual VTChave*     __fastcall PM_GetChave(void) = 0;
	  virtual void         __fastcall PM_SetChave(VTChave* pchave) = 0;
	  virtual VTBarra*     __fastcall PM_GetBarraChave(void) = 0;
	  virtual void         __fastcall PM_SetBarraChave(VTBarra* barraChave) = 0;
virtual VTLisBlocoChave*  __fastcall PM_GetLisBlocoChave(void) = 0;                           //hk201611
	  virtual void         __fastcall PM_SetLisBlocoChave(VTLisBlocoChave* plisBlocoChave) = 0;//hk201611
	  virtual TList*       __fastcall PM_GetLisBlocoRad(void) = 0;
	  virtual TList*       __fastcall PM_GetLisBlocoRadJus(void) = 0;
	  virtual TList*       __fastcall PM_GetLisBlocoJus(void) = 0;
	  virtual TList*       __fastcall PM_GetLisBlocoJusProt(void) = 0;
	  virtual TList*       __fastcall PM_GetLisBlocoMon(void) = 0;
	  virtual int          __fastcall PM_GetNumPat(void) = 0;
	  virtual double       __fastcall PM_GetCargaBlRad(int iPat) = 0;
	  virtual void         __fastcall PM_SetCargaBlRad(int iPat, double valor) = 0;
	  virtual double       __fastcall PM_GetCargaBloco(int iPat) = 0;
	  virtual void         __fastcall PM_SetCargaBloco(int iPat, double valor) = 0;
	  virtual int          __fastcall PM_GetNCons(void) = 0;
	  virtual void         __fastcall PM_SetNCons(int ncons) = 0;
	  virtual int 		   __fastcall PM_GetNConsJus(void) = 0;//hk201902
	  virtual void 		   __fastcall PM_SetNConsJus(int nconsJus) = 0;//hk201902
	  virtual bool         __fastcall PM_GetFlagSup(void) = 0;
	  virtual void         __fastcall PM_SetFlagSup(bool flagSup) = 0;
	  virtual VTRede*      __fastcall PM_GetRede(void) = 0;
	  virtual void         __fastcall PM_SetRede(VTRede *rede) = 0;

	public:
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto
//---------------------------------------------------------------------------
VTBlocoRad* __fastcall NewObjBlocoRad(VTBloco *bloco, VTApl *apl);  //hk201610: definição de rede de bloco: inclusao de apl

//---------------------------------------------------------------------------
#endif

