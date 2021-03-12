//---------------------------------------------------------------------------
#ifndef VTRegioesH
#define VTRegioesH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRede;
class VTRegiao;

//---------------------------------------------------------------------------
class VTRegioes : public TObject
   {
   public:
                        __fastcall  VTRegioes(void) {};
      virtual           __fastcall ~VTRegioes(void) {};
      virtual void      __fastcall  Clear(void) = 0;
      virtual VTRegiao* __fastcall  CriaRegiao(void) = 0;
      virtual VTRegiao* __fastcall  ExisteRegiao(int regiao_id, bool somenteVisible = false) = 0;
      virtual VTRegiao* __fastcall  ExisteRegiao(AnsiString codigo, bool somenteVisible = false) = 0;
      virtual VTRegiao* __fastcall  ExisteRegiao(TPoint *point_cm, bool somenteVisible = false) = 0;
      virtual void      __fastcall  ExisteRegioes(TList *lisREGIAO_vazia, TPoint *point_cm, bool somenteVisible = false) =0;
      virtual bool      __fastcall  ImportaRegiaoLatLon(AnsiString arq_latlon) = 0;
      virtual bool      __fastcall  ImportaRegiaoUtm(AnsiString arq_utm) = 0;
      virtual void      __fastcall  InsereRegiao(VTRegiao *regiao) = 0;
      virtual TList*    __fastcall  LisRegiao(void) = 0;
      virtual void      __fastcall  OrdenaArvore(TList *lisRegiaoParcial) =0;
      virtual bool      __fastcall  ReiniciaLisEqptoRegiao(void) = 0;
      virtual bool      __fastcall  ReiniciaLisEqptoRegiao(VTRede *rede) = 0;
      virtual void      __fastcall  RegioesVisible(TList *lisRegiaoParcial, bool visible) = 0;
      virtual void      __fastcall  RegioesVisibleComplementar(TList *lisRegiaoParcial,
                                                                bool visible) = 0;
      virtual void      __fastcall  RetiraRegiao(VTRegiao *regiao) = 0;

   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRegioes
//---------------------------------------------------------------------------
VTRegioes* __fastcall NewObjRegioes(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof

 