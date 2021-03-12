//-----------------------------------------------------------------------------
#ifndef VTValFasesH
#define VTValFasesH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTValFases : public TObject
   {
   public:
      __property bool CorrigeFases = {read=PD.corrige_fases, write=PD.corrige_fases};

   public:
                      __fastcall  VTValFases(void) {};
      virtual         __fastcall ~VTValFases(void) {};
      virtual bool    __fastcall  Executa(void)=0;
      virtual int     __fastcall  ExisteEqptoComFaseInvalida(void)=0;
      virtual TList*  __fastcall  LisEqptoComFaseInvalida(void)=0;
      virtual bool    __fastcall  SubstituiTrechosFasesInvalidas(int faseNOVA) =0;

   protected:  //dados acessados via property
      struct{
            bool corrige_fases;
            }PD;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTMonta
//-----------------------------------------------------------------------------
VTValFases* __fastcall NewObjValFases(VTApl *apl);
VTValFases* __fastcall NewObjValFasesMalha(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
