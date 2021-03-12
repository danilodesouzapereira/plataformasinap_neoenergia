//-----------------------------------------------------------------------------
#ifndef VTSinapServerH
#define VTSinapServerH

//-----------------------------------------------------------------------------
#include <Classes.hpp>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>

//-----------------------------------------------------------------------------
class VTApl;
class VTChave;
class VTPatamar;

//-----------------------------------------------------------------------------
class VTSinapServer  : public TObject
   {
   public:
      __property bool Ativo = {read=PM_GetAtivo};

   public:
                         __fastcall  VTSinapServer(void) {};
      virtual            __fastcall ~VTSinapServer(void) {};
		virtual bool       __fastcall  Conecta(int server_port) = 0;
      virtual void       __fastcall  Desconecta(void) = 0;
		virtual int        __fastcall  LisEqptoSupervisionado(TList *lisEXT, int eqpto_tipo=eqptoINDEF) = 0;
		virtual VTPatamar* __fastcall  PatamarAtual(void) = 0;
		virtual VTPatamar* __fastcall  SelecionaProximoPatamar(void) = 0;
		virtual bool       __fastcall  SupervisaoAutomaticaStart(TList *lisEXT) = 0;
		virtual bool       __fastcall  SupervisaoAutomaticaStop(void) = 0;

   protected:
      virtual bool  __fastcall  PM_GetAtivo(void) = 0;
   };

//-----------------------------------------------------------------------------
// função para criar objeto VTSinapServer
//-----------------------------------------------------------------------------
VTSinapServer* __fastcall NewObjSinapServer(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof
