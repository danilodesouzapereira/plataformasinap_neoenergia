//---------------------------------------------------------------------------
#ifndef VTDirH
#define VTDirH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTDir : public TObject
   {
   public:
      __property AnsiString DirBase = {read=PM_GetDirBase};
      __property AnsiString ArqRede = {read=PM_GetArqRede, write=PM_SetArqRede};

   public:
                   __fastcall  VTDir(void) {};
	  virtual      __fastcall ~VTDir(void) {};
	  virtual bool __fastcall  Arquivos(AnsiString dir_orig, TStrings *files, AnsiString filtro="*.*") = 0;
	  virtual bool __fastcall  CopiaArquivo(AnsiString arq_orig, AnsiString arq_dest) = 0;
	  virtual bool __fastcall  CopiaDiretorio(AnsiString dir_orig, AnsiString dir_dest, AnsiString filtro="*.*") = 0;
	  virtual bool __fastcall  CriaCopiaRedeEmpresa(AnsiString &arq_dest, bool consulta_usuario=true) = 0;
	  virtual bool __fastcall  CriaCopiaRedeSinap(AnsiString &arq_dest, bool consulta_usuario=true) = 0;
	  virtual bool __fastcall  CriaDiretorio(AnsiString dir, AnsiString &subdir) = 0;
	  virtual bool __fastcall  DeleteDirectory(AnsiString dir) = 0;
	  virtual bool __fastcall  DeleteRede(void) = 0;
	  virtual bool __fastcall  DeleteTmp(void) = 0;
	  virtual bool __fastcall  LisDirectory(AnsiString dir, TStringList *slisDir) = 0;
	  virtual bool __fastcall  SelArquivoRedeToOpen(AnsiString &arq) = 0;
	  virtual bool __fastcall  SelArquivoRedeToSave(AnsiString &arq) = 0;
	  virtual bool __fastcall  SelDir(AnsiString &dirname) = 0;
	  virtual bool __fastcall  SelDirBase(void) = 0;
	  virtual bool __fastcall  ValidaArqRede(void) = 0;
	  virtual bool __fastcall  ValidaDirBase(void) = 0;

   protected:  //métodos acessados via property
      virtual AnsiString __fastcall PM_GetDirBase(void) = 0;
      virtual AnsiString __fastcall PM_GetArqRede(void) = 0;
      virtual void       __fastcall PM_SetArqRede(AnsiString arq_rede) = 0;
   };

//---------------------------------------------------------------------------
//função global
VTDir* __fastcall NewObjDir(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
