//---------------------------------------------------------------------------
#ifndef VTPathH
#define VTPathH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTPath : public TObject
   {
	public:  //property
      __property AnsiString ArqHistorico    = {read=PM_GetArqHistorico, write=PM_SetArqHistorico};
      __property AnsiString ArqRede         = {read=PM_GetArqRede,      write=PM_SetArqRede     };
      __property AnsiString DirBase         = {read=PM_GetDirBase,      write=PM_SetDirBase};
	  __property AnsiString DirBin          = {read=PM_GetDirBin,       write=PM_SetDirBin      };
	  __property AnsiString DirTmpBatch          = {read=PM_GetDirTmpBatch,       write=PM_SetDirTmpBatch      };

   public:
                         __fastcall  VTPath(void) {};
      virtual            __fastcall ~VTPath(void) {};
      virtual AnsiString __fastcall  ArqBitmap(int bmp_id) = 0;
      virtual AnsiString __fastcall  ArqRelCurto(void) = 0;
      virtual AnsiString __fastcall  ArqRedeEmpresa(void) = 0;
      virtual AnsiString __fastcall  ArqRedeSinap(void) = 0;
      virtual AnsiString __fastcall  ArqRelFlow(void) = 0;
      virtual AnsiString __fastcall  ArqRelFlowDebug(void) = 0;
      virtual AnsiString __fastcall  ArqTermoUso(void) = 0;
      virtual AnsiString __fastcall  ArqTermoUsoDemo(void) = 0;
      virtual AnsiString __fastcall  ArqTermoUsoDidatico(void) = 0;
      virtual AnsiString __fastcall  DirBmp(void) = 0;
      virtual AnsiString __fastcall  DirDat(void) = 0;
      virtual AnsiString __fastcall  DirExporta(void) = 0;
	  virtual AnsiString __fastcall  DirGeral(void) = 0;
	  virtual AnsiString __fastcall  DirHelp(void) = 0;
	  virtual AnsiString __fastcall  DirHistorico(void) = 0;
	  virtual AnsiString __fastcall  DirIcone(void) = 0;
      virtual AnsiString __fastcall  DirImporta(void) = 0;
      virtual AnsiString __fastcall  DirModuloConstrutivo(void) = 0;
      virtual AnsiString __fastcall  DirRedes(void) = 0;
      virtual AnsiString __fastcall  DirTmp(void) = 0;
      virtual AnsiString __fastcall  DirUsuario(void) = 0;

   protected:  //métodos acessados via property
      virtual AnsiString __fastcall  PM_GetArqHistorico(void) = 0;
      virtual AnsiString __fastcall  PM_GetArqRede(void) = 0;
      virtual AnsiString __fastcall  PM_GetDirBase(void) = 0;
	  virtual AnsiString __fastcall  PM_GetDirBin(void) = 0;
	  virtual AnsiString __fastcall  PM_GetDirTmpBatch(void) = 0;

      //set
      virtual void __fastcall  PM_SetArqHistorico(AnsiString arq_historico) = 0;
      virtual void __fastcall  PM_SetArqRede(AnsiString arq_rede) = 0;
	  virtual void __fastcall  PM_SetDirBase(AnsiString dir_base) = 0;
	  virtual void __fastcall  PM_SetDirBin(AnsiString dir_bin) = 0;
	  virtual void __fastcall  PM_SetDirTmpBatch(AnsiString dir_tmp) = 0;
   };

//---------------------------------------------------------------------------
//função global
VTPath* NewObjPath(void);

#endif
//---------------------------------------------------------------------------
//eof
