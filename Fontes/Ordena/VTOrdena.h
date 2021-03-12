//---------------------------------------------------------------------------
#ifndef VTOrdenaH
#define VTOrdenaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTNet;
class VTMNet;
class VTRede;
class VTRedes;

//---------------------------------------------------------------------------
class VTOrdena : public TObject
   {
   public:  //property
      __property bool ChaveNA_Enabled  = {read=PD.chaveNA_enabled,  write=PD.chaveNA_enabled};
      __property bool ChaveNF_Enabled  = {read=PD.chaveNF_enabled,  write=PD.chaveNF_enabled};
      __property bool TramoEnabled     = {read=PD.tramo_enabled,    write=PD.tramo_enabled  };
      __property bool ReguladorBypass  = {read=PD.regulador_bypass, write=PD.regulador_bypass  };

   public:
                     __fastcall  VTOrdena(void) {};
                     __fastcall ~VTOrdena(void) {};
      virtual VTNet* __fastcall  Executa(TList *lisBAR, TList *lisEQB, TList *lisLIG,
                                         TList *lisMUTUA) = 0;
      virtual VTNet* __fastcall  Executa(VTMNet *mnet) = 0;
      virtual VTNet* __fastcall  Executa(VTRede *rede) = 0;
      virtual VTNet* __fastcall  Executa(VTRedes *redes) = 0;
      virtual bool   __fastcall  Executa(VTNet *net) = 0;
      virtual void   __fastcall  MontaGrupoMutuaDefinidaPorArranjo(void) = 0;

   protected:  //dados acessados via property
      struct   {
               bool chaveNA_enabled;  //true => mantém chave NA na lista de ligações
               bool chaveNF_enabled;  //true => mantém chave NF na lista de ligações
               bool tramo_enabled;    //true => cria Tramos
               bool regulador_bypass;  //true => trata todo Regulador como by_pass true
               }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTOrdena
//---------------------------------------------------------------------------
VTOrdena* __fastcall NewObjOrdena(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 