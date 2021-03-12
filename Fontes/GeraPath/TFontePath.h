//---------------------------------------------------------------------------
#ifndef TFontePathH
#define TFontePathH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTArvore;
class VTEqpto;
//---------------------------------------------------------------------------
class TFontePath : public TObject
   {
   public:
				 __fastcall TFontePath(VTEqpto* eqpto);
				 __fastcall ~TFontePath(void);
		  bool __fastcall AddCaminho(VTArvore* arv);
	 VTEqpto* __fastcall Eqpto(void);
	 TList*   __fastcall LisPath(void);

   private: //métodos

   private: //dados externos

   private: //dados locais
      VTEqpto* eqpto;
		TList* lisPath;   //lista de caminhos - árvores extremas
  };

#endif
//---------------------------------------------------------------------------
//eof
