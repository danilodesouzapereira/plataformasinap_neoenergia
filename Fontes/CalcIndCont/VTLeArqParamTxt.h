//---------------------------------------------------------------------------
#ifndef VTLeArqParamTxtH
#define VTLeArqParamTxtH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTParamConf;
//---------------------------------------------------------------------------
class VTLeArqParamTxt: public TObject
	{
	public:  //property

	public:
						__fastcall VTLeArqParamTxt(void) {};
						__fastcall ~VTLeArqParamTxt(void) {};
	  virtual bool __fastcall LeArqParam(VTParamConf *paramConf, TList *lisChavesClone, TList *lisChavesNovas, AnsiString arqParam) = 0;
	  virtual bool __fastcall GravaArqParam(VTParamConf *paramConf, AnsiString arqParam) = 0;
	protected: //métodos acessados via property
	public:
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto
//---------------------------------------------------------------------------
VTLeArqParamTxt* __fastcall NewObjLeArqParamTxt(VTApl* apl_owner);

//---------------------------------------------------------------------------
#endif
//eof
