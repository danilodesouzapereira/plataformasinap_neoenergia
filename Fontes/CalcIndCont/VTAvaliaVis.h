//---------------------------------------------------------------------------
#ifndef VTAvaliaVisH
#define VTAvaliaVisH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTApl;
//---------------------------------------------------------------------------
class VTAvaliaVis: public TObject
   {
	public:  //property
		 __property int  IPat           = {read=PM_GetIPat, write=PM_SetIPat};
		 __property bool FlagAvaliaChave= {read=PM_GetFlagAvaliaChave, write=PM_SetFlagAvaliaChave};

	public:
									__fastcall VTAvaliaVis(void) {};
									__fastcall ~VTAvaliaVis(void) {};
	  virtual bool __fastcall ExecutaVisRede(TList *lisVisRede, bool flagFluxo = false) = 0;
	  //hk201609-Sai virtual bool __fastcall Executa(TList *lisChvVis, bool flagFluxo = false) = 0;

	public:

	protected: //métodos acessados via property
	  virtual  int  __fastcall PM_GetIPat(void) = 0;
	  virtual  void __fastcall PM_SetIPat(int iPat) = 0;
	  virtual  bool __fastcall PM_GetFlagAvaliaChave(void) = 0;
	  virtual  void __fastcall PM_SetFlagAvaliaChave(bool flag) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto
//---------------------------------------------------------------------------
VTAvaliaVis* __fastcall NewObjAvaliaVis(VTApl* apl);

//---------------------------------------------------------------------------
#endif
//eof
