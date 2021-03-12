//-----------------------------------------------------------------------------
#ifndef VTReticuladoH
#define VTReticuladoH

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTCluster;
class VTRede;
class VTPrimario;
class VTSecundario;
class VTSubestacao;

//-----------------------------------------------------------------------------
class VTReticulado  : public TObject
   {
  public:  //property
      __property VTCluster*    Cluster     = {read=PM_GetCluster,    write=PM_SetCluster};
      __property VTSubestacao* Subestacao  = {read=PM_GetSubestacao, write=PM_SetSubestacao};

   public:
                            __fastcall  VTReticulado(void)  {};
      virtual               __fastcall ~VTReticulado(void) {};
      virtual bool          __fastcall  ExistePrimario(VTPrimario *primario) = 0;
      virtual bool          __fastcall  ExisteSecundario(VTSecundario *secundario) = 0;
      virtual void          __fastcall  InserePrimario(VTPrimario *primario) = 0;
      virtual void          __fastcall  InsereSecundario(VTSecundario *secundario) = 0;
      virtual TList*        __fastcall  LisPrimario(void) = 0;
      virtual TList*        __fastcall  LisSecundario(void) = 0;

   protected:  //métodos acessados via property
      virtual VTCluster*    __fastcall PM_GetCluster(void) = 0;
      virtual VTSubestacao* __fastcall PM_GetSubestacao(void) = 0;
      //
      virtual void          __fastcall PM_SetCluster(VTCluster *cluster) = 0;
      virtual void          __fastcall PM_SetSubestacao(VTSubestacao *subestacao) = 0;
    };

//-----------------------------------------------------------------------------
//função global
VTReticulado* __fastcall NewObjReticulado(void);

#endif
//---------------------------------------------------------------------------
//eof
