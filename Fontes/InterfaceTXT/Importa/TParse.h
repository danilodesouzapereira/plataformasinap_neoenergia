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
      void __fastcall AtualizaBloco(AnsiString bloco);
      void __fastcall AtualizaLogProgresso(void);
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
					//diag
                    estDIAGNOSTICO,
                    //tipos
                    estTIPO_CHAVE,
                    estTIPO_CONSUMIDOR,
                    estTIPO_REDE,
                    //curvas típicas
                    estCURVA_CAPACITOR,
                    estCURVA_CARGA,
                    estCURVA_CARGA_IP,
                    estCURVA_GERADOR,
                    estCURVA_REATOR,
                    //suportes, cabos e arranjos
                    estSUPORTE,
                    estCABO_UNIPOLAR,
                    estCABO_MULTIPLEXADO,
                    estARRANJO_Z0Z1,
                    estARRANJO_PUSB100,
                    estARRANJO_CABO_Z0Z1,
                    estARRANJO_CABOxFASE,
                    //eqptos
					estBARRA,
					estBATERIA,
                    estCANAL,
                    estCAPACITOR,
                    estCAPSERIE,
                    estCARGA_IP,
                    estCARGA_MEDICAO,
                    estCARGA_TIPICA,
                    estCHAVE,
                    estFILTRO,
                    estGERADOR,
                    estMEDIDOR,
                    estMUTUA,
                    estREATOR,
                    estREDE,
                    estREGULADOR_AUTO,
                    estREGULADOR_FIXO,
                    estSUPRIMENTO,
                    estTRAFO_2E,
                    estTRAFO_3E,
                    estTRAFO_MONO,
                    estTRAFO_ZZ,
                    estTRECHO_TIPICO,
                    estTRECHO_Z0Z1
                    };
   };

#endif
//---------------------------------------------------------------------------
//eof

