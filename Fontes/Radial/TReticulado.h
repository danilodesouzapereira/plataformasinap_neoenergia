//---------------------------------------------------------------------------
#ifndef TReticuladoH
#define TReticuladoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTReticulado.h"

//---------------------------------------------------------------------------
class VTCorte;

//---------------------------------------------------------------------------
class TReticulado : public VTReticulado
   {
   private: //dados acessados via property
      struct   {
               VTCluster    *cluster;
               VTSubestacao *subestacao;
               } PD;

   private: //dados locais
      TList  *lisPRI;
      TList  *lisSEC;

   public:  //métodos
                    __fastcall  TReticulado(void);
                    __fastcall ~TReticulado(void);
      bool          __fastcall  ExistePrimario(VTPrimario *primario);
      bool          __fastcall  ExisteSecundario(VTSecundario *secundario);
      void          __fastcall  InserePrimario(VTPrimario *primario);
      void          __fastcall  InsereSecundario(VTSecundario *secundario);
      TList*        __fastcall  LisPrimario(void);
      TList*        __fastcall  LisSecundario(void);

   protected:  //métodos acessados via property
      VTCluster*    __fastcall PM_GetCluster(void);
      VTSubestacao* __fastcall PM_GetSubestacao(void);
      //
      void          __fastcall PM_SetCluster(VTCluster *cluster);
      void          __fastcall PM_SetSubestacao(VTSubestacao* subestacao);
   };

//---------------------------------------------------------------------------
#endif
//eof
