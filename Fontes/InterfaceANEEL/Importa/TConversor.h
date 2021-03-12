//---------------------------------------------------------------------------
#ifndef TConversorH
#define TConversorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTConversor.h"

//---------------------------------------------------------------------------
class VTLog;
class VTRede;
class VTBarra;

//---------------------------------------------------------------------------
class TConversor : public VTConversor
{
   public:
                    __fastcall  TConversor(VTApl *apl);
                    __fastcall ~TConversor(void);
      bool          __fastcall  ImportaLisArqPrimario(TStrings *files_pri, strOPImporta *opcao);
      bool          __fastcall  ImportaLisArqSubestacao(TStrings *files_pri, strOPImporta *opcao);
      strOPImporta* __fastcall  OpcoesImportacao(void);

  private:  //métodos
      //void       __fastcall DefineCor(void);
      //VTBarra*   __fastcall DeterminaBarraInicial(VTRede *rede);
      bool       __fastcall ExportaRedesSecundarias(void);
      bool       __fastcall ExtractTXT(AnsiString filefullpath, int ind_arq, int num_arq);
      bool       __fastcall ImportaLisArquivoTxt(TStrings  *txtFiles);
      bool       __fastcall IniciaObjApl(void);
		bool       __fastcall LeListaArquivos(AnsiString arq_sub, TStringList *files_pri);
      void       __fastcall LogErro(AnsiString arq_txt, AnsiString msg_erro);
      void       __fastcall LogFecha(void);
      void       __fastcall LogInicia(void);
      void       __fastcall LogSucesso(AnsiString arq_txt, TDateTime start, TDateTime stop);
      void       __fastcall VerificaFases(void);
      void       __fastcall VerificaPerdaFerro(void);

  private: //objetos externos
      VTApl *apl_owner;

  private: //dados locais
      TList       *lisEQP;
      TStringList *files_pri;
      TStringList *lis_arq;
      VTApl       *apl;
      VTLog       *log_resumo;
      //dados de importação
      strOPImporta *opcao;

};
//---------------------------------------------------------------------------
#endif
