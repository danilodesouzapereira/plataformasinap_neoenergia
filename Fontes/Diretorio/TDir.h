//---------------------------------------------------------------------------
#ifndef TDirH
#define TDirH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTDir.h"

//---------------------------------------------------------------------------
class VTPath;
class VTProduto;

//---------------------------------------------------------------------------
class TDir : public VTDir
   {
   public:
           __fastcall  TDir(VTApl *apl);
			  __fastcall ~TDir(void);
	  bool __fastcall  Arquivos(AnsiString dir_orig, TStrings *files, AnsiString filtro="*.*");
	  bool __fastcall  CopiaArquivo(AnsiString arq_orig, AnsiString arq_dest);
	  bool __fastcall  CopiaDiretorio(AnsiString dir_orig, AnsiString dir_dest, AnsiString filtro="*.*");
	  bool __fastcall  CriaCopiaRedeEmpresa(AnsiString &arq_dest, bool consulta_usuario=true);
	  bool __fastcall  CriaCopiaRedeSinap(AnsiString &arq_dest, bool consulta_usuario);
	  bool __fastcall  CriaDiretorio(AnsiString dir, AnsiString &subdir);
	  bool __fastcall  DeleteDirectory(AnsiString dir);
	  bool __fastcall  DeleteRede(void);
	  bool __fastcall  DeleteTmp(void);
	  bool __fastcall  LisDirectory(AnsiString dir, TStringList *slisDir);
	  bool __fastcall  SelDir(AnsiString &dirname);
	  bool __fastcall  SelArquivoRedeToOpen(AnsiString &arq_rede);
	  bool __fastcall  SelArquivoRedeToSave(AnsiString &arq_rede);
	  bool __fastcall  SelDirBase(void);
	  bool __fastcall  ValidaArqRede(void);
	  bool __fastcall  ValidaDirBase(void);

   protected:  //métodos acessados via property
	  AnsiString __fastcall PM_GetDirBase(void);
	  AnsiString __fastcall PM_GetArqRede(void);
	  void       __fastcall PM_SetArqRede(AnsiString arq_rede);

   private: //métodos
      bool       __fastcall CriaDiretorioRede(AnsiString &dir_rede, bool consulta_usuario);
      AnsiString __fastcall DefineNomeArquivoInexistente(AnsiString pathname);
      AnsiString __fastcall DefineNomeDiretorioInexistente(AnsiString dir_base, AnsiString radical);
      void       __fastcall GravaArqDiretorios(void);
      void       __fastcall LeArqDiretorios(void);
      bool       __fastcall ValidaDiretorios(void);
      bool       __fastcall ValidaArqRede(AnsiString dir);
      bool       __fastcall ValidaDirBase(AnsiString dir);

   private: //objetos externos
      VTApl     *apl;
      VTPath    *path;
      VTProduto *produto;
   };

#endif
//---------------------------------------------------------------------------
//eof
