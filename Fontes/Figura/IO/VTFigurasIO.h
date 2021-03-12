//---------------------------------------------------------------------------
#ifndef VTFigurasIOH
#define VTFigurasIOH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEstudo;

//---------------------------------------------------------------------------
class VTFigurasIO : public TObject
   {
   public:
                   __fastcall  VTFigurasIO(void) {};
	  virtual      __fastcall ~VTFigurasIO(void) {};
	  virtual bool __fastcall  ImportaTXT(VTEstudo *estudo, AnsiString full_filepath = "", TList* lisREDE_SELE = NULL) = 0;
	  virtual bool __fastcall  ExportaTXT(VTEstudo *estudo, AnsiString full_filepath) = 0;
   };

//---------------------------------------------------------------------------
VTFigurasIO* NewObjFigurasIO(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof