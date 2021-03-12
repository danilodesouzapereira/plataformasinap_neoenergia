//---------------------------------------------------------------------------
#ifndef TParseH
#define TParseH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTParse.h"

//---------------------------------------------------------------------------
class VTExtrator;
class VTLog;
class VTProgresso;

//---------------------------------------------------------------------------
class TParse : public VTParse
   {
   public:
           __fastcall  TParse(VTApl *apl);
           __fastcall ~TParse(void);
      bool __fastcall  Executa(AnsiString arq_txt);
      bool __fastcall  Executa(AnsiString arq_lista, TStrings *lines);

   private: //métodos
      void __fastcall ActionCancelaExecute(TObject *Sender);
      void __fastcall AtualizaLogProgresso(AnsiString bloco);
      void __fastcall ExtraiCampos(AnsiString txt, TStrings *campos);
      bool __fastcall PalavraChave(int &estado);

   private: //objetos externos
      VTApl       *apl;
      VTExtrator  *extrator;
      VTLog       *plog;
		VTProgresso *progresso;

   private: //dados locais
      int      estado;
      int      versao;
      TAction  *ActionCancela;
      TStrings *campos;
      TStrings *lines;
      //estados do parser
      enum eESTADO {estBEGIN=0,
						  estVERSAO,
                    estCANCELA,
						  estEND,
						  estCONDUTOR,
						  estCOORDENADA,
						  estCARGA_BT,
						  estCARGA_MT,
						  estCHAVE_BT,
						  estCHAVE_MT,
						  estCIRCUITO,
						  estGERADOR_BT,
						  estGERADOR_MT,
						  estRAMAL_BT,
						  estREGULADOR_MT,
						  estSEGMENTO_BT,
						  estSEGMENTO_MT,
						  estTRAFO_AT_MT,
						  estTRAFO_MT_BT
						  };
   };

#endif
//---------------------------------------------------------------------------
//eof

