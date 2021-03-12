//---------------------------------------------------------------------------
#ifndef TExportaKmlH
#define TExportaKmlH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTExportaKml.h"

//---------------------------------------------------------------------------
class VTScg;

//---------------------------------------------------------------------------
class TExportaKml : public VTExportaKml
   {
   public:
           __fastcall  TExportaKml(VTApl *apl);
           __fastcall ~TExportaKml(void);
      bool __fastcall  Executa(TList *lisREDE, AnsiString arq_kml);
      bool __fastcall  Executa(VTRede *rede, AnsiString arq_kml);

   private: //m�todos
		AnsiString __fastcall ConverteCor(TColor Cor);
		void       __fastcall DocumentClose(void);
      void       __fastcall DocumentOpen(AnsiString nome);
      void       __fastcall FolderClose(void);
      void       __fastcall FolderOpen(AnsiString nome);
      void       __fastcall Header(void);
		void       __fastcall InsereLinhas(TList *lisLIGACAO, AnsiString codigo_rede);
		void       __fastcall InsereMarcadorChave(TList *lisLIGACAO);
		void       __fastcall InsereMarcadorTrafo(TList *lisLIGACAO);
		void       __fastcall InserePonto(int x, int y, AnsiString nome_ligacao, TColor cor, AnsiString marcador, AnsiString descricao);

   private: //dados externos
      VTApl   *apl;

   private: //dados locais
      VTScg       *scg;
      TStringList *Lines;
      int          map_type;
      double       z0;  //rela��o pixel/grau do zoom 0 do google, emp�rico = 0,713
	};

//---------------------------------------------------------------------------
#endif
//eof

