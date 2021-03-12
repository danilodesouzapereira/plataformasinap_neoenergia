//---------------------------------------------------------------------------
#ifndef VTAjustaH
#define VTAjustaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTSuprimento;
class VTPerfilAlt;
class VTPerfilRede;

//---------------------------------------------------------------------------
class VTAjusta : public TObject
   {
   public:  //property
      __property VTPerfilRede* PerfilRedeInicial  = {read=perfil_rede.inicial };
//      __property VTPerfilRede* PerfilRedeSemObra  = {read=perfil_rede.sem_obra};
//      __property VTPerfilRede* PerfilRedeComObra  = {read=perfil_rede.com_obra};
      __property TList*        LisPerfilAlt       = {read=lisPERFIL_ALT};

   public:
                     __fastcall  VTAjusta(void) {};
                     __fastcall ~VTAjusta(void) {};
      virtual bool   __fastcall  CalculaFluxoComObra(void) = 0;
      virtual bool   __fastcall  Executa(VTSuprimento *suprimento, VTBarra *barra_ini, VTBarra *barra_cons) = 0;

   protected:   //dados acessados via property
      TList          *lisPERFIL_ALT;
   struct   {
            VTPerfilRede *inicial;
//            VTPerfilRede *sem_obra;
//            VTPerfilRede *com_obra;
            }perfil_rede;

   };

//---------------------------------------------------------------------------
VTAjusta* __fastcall NewObjAjusta(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof

