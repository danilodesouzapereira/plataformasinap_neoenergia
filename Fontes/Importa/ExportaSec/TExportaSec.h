//---------------------------------------------------------------------------
#ifndef TExportaSecH
#define TExportaSecH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTExportaSec.h"

//---------------------------------------------------------------------------
class VTRadial;
class VTBarra;
class VTSecundario;


//---------------------------------------------------------------------------
class TExportaSec : public VTExportaSec
   {
   public:
           __fastcall  TExportaSec(VTApl *apl);
			  __fastcall ~TExportaSec(void);
      bool __fastcall  Executa(bool showProgress);

   private: //métodos

      bool __fastcall CriaArquivoTXT(VTSecundario *secundario, AnsiString dir);
      bool __fastcall CriaDiretorio(AnsiString dir);
      bool __fastcall InsereSuprimento(VTBarra *barra, AnsiString codigo);
      bool __fastcall RemoveSuprimento(VTBarra *barra);

   private: //objetos externos
      VTApl       *apl;

   private: //dados locais
      VTRadial    *radial;
      TList       *lisEQP;
   };

#endif
//-----------------------------------------------------------------------------
// eof
