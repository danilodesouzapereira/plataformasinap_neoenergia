//---------------------------------------------------------------------------

#ifndef TImportaCurvaH
#define TImportaCurvaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\Constante\Fases.h>

//---------------------------------------------------------------------------
class VTApl;
class VTCanal;
class VTChave;
class VTCurva;
class VTEqpto;
class VTMedidor;
class VTPatamar;
class VTProgresso;
class VTLog;

//---------------------------------------------------------------------------
class TImportaCurva : public TObject
   {
   public:
           __fastcall  TImportaCurva(VTApl *apl, VTCurva *curva=NULL);
           __fastcall ~TImportaCurva(void);
//      bool __fastcall  Executa(AnsiString arq_txt, VTMedidor *medidor=NULL);
      bool __fastcall  Executa(AnsiString arq_txt, VTEqpto *eqpto=NULL, VTMedidor *medidor=NULL);

   private: //métodos
      void     __fastcall ActionCancelaExecute(TObject *Sender);
//      bool     __fastcall AtualizaMedidor(VTPatamar *patamar, double valor[MAX_FASE], VTChave *chave);
      void     __fastcall ConfiguraCanal(void);
      void     __fastcall ConverteUnidade(double valor[MAX_FASE]);
//      VTChave* __fastcall ExisteChave(AnsiString cod_chave);
      void     __fastcall DesativaCanais(TList *lisCanal);
      bool     __fastcall InsereCurva(TStrings *campos);
	  bool     __fastcall InsereCurvaMedidor(TStrings *campos);
	  bool 	   __fastcall InsereCurvaPQ(TStrings *campos);
	  bool 	   __fastcall InsereCurvaPQVT(TStrings *campos);
//      bool     __fastcall InsereValorFase(double valor[MAX_FASE], VTChave *chave, VTMedidor *medidor, VTPatamar *patamar);
//      bool     __fastcall InsereValorTri(double valor_tri, VTChave *chave, VTMedidor *medidor, VTPatamar *patamar);
      void     __fastcall PriorizaCanais(void);
      int      __fastcall TraduzTipoMedicao(AnsiString tipo_med);
      bool     __fastcall VerificaComentario(AnsiString linha);

   private: //objetos externos
      VTApl       *apl;
      VTLog       *plog;
      VTProgresso *progresso;
      VTCurva     *curva;
      VTEqpto     *eqpto;
      VTMedidor   *medidor;

   private: //dados locais
      bool     por_fase;
      int      medI, medPfase, medPtrif;
      int      tipo_canal;
      TAction  *ActionCancela;
      TStrings *campos;
      TStrings *lines;
      TList    *lisCANAL;
   };
   
//---------------------------------------------------------------------------
#endif
