//---------------------------------------------------------------------------
#ifndef VTPatamaresH
#define VTPatamaresH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTPatamar;

//---------------------------------------------------------------------------
class VTPatamares : public TObject
   {
   public:  //property
      __property int        Tipo             = {read=PM_GetTipo, write=PM_SetTipo};
      __property AnsiString Nome             = {read=PM_GetNome};
      __property VTPatamar  *Patamar[int np] = {read=PM_GetPatamar};
      __property VTPatamar  *Patamar_ativo   = {read=PM_GetPatamarAtivo, write=PM_SetPatamarAtivo};

   public:  //property OBSOLETO p/ manter compatibilidade c/ demais módulos
      __property int        Escala           = {read=PM_GetTipo, write=PM_SetTipo};

   public:  //funções públicas
                         __fastcall  VTPatamares(void) {};
                         __fastcall ~VTPatamares(void) {};
      virtual void       __fastcall  CopiaAtributosDe(VTPatamares &patamares) = 0;
      virtual VTPatamar* __fastcall  ExistePatamar(int hora, int minuto) = 0;
      virtual TList*     __fastcall  LisPatamar(void) = 0;
      virtual void       __fastcall  LisPatamar(TList *lisEXT) = 0;
      virtual void       __fastcall  LisPatamarHabilitado(TList *lisEXT) = 0;
      virtual int        __fastcall  NumPatamar(void) = 0;
      virtual int        __fastcall  NumPatamarHabilitado(void) = 0;
      virtual VTPatamar* __fastcall  PrimeiroPatamarHabilitado(void) = 0;
      virtual void       __fastcall  ReiniciaDiagFluxo(bool fluxo_correto) = 0;

   protected: //métodos acessados via property
      virtual int        __fastcall PM_GetTipo(void) = 0;
      virtual AnsiString __fastcall PM_GetNome(void) = 0;
      virtual VTPatamar* __fastcall PM_GetPatamar(int np) = 0;
      virtual VTPatamar* __fastcall PM_GetPatamarAtivo(void) = 0;
      virtual void       __fastcall PM_SetTipo(int tipo) = 0;
      virtual void       __fastcall PM_SetPatamarAtivo(VTPatamar *patamar) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTPatamares
//---------------------------------------------------------------------------
VTPatamares* __fastcall NewObjPatamares(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof
