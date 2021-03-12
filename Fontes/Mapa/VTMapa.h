//---------------------------------------------------------------------------
#ifndef VTMapaH
#define VTMapaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;
class VTRegiao;
class VTRede;

//-----------------------------------------------------------------------------
class VTMapa : public TObject
   {
   public:
      __property int         Id        = {read=PD.id,        write=PD.id};
      __property AnsiString  Codigo    = {read=PD.codigo,    write=PD.codigo};
      __property AnsiString  Descricao = {read=PD.descricao, write=PD.descricao};

   public:
                        __fastcall  VTMapa(void) {};
      virtual           __fastcall ~VTMapa(void) {};
      virtual VTRegiao* __fastcall  CriaRegiao(void) = 0;
      virtual VTRegiao* __fastcall  ExisteRegiao(int regiao_id) = 0;
      virtual VTRegiao* __fastcall  ExisteRegiao(AnsiString codigo) = 0;
      virtual bool      __fastcall  ImportaRegiaoLatLon(AnsiString arq_latlon) = 0;
      virtual bool      __fastcall  ImportaRegiaoUtm(AnsiString arq_utm) = 0;
      virtual void      __fastcall  InsereRegiao(VTRegiao *regiao) = 0;
      virtual TList*    __fastcall  LisRegiao(void) = 0;
      virtual bool      __fastcall  ReiniciaLisEqptoRegiao(void) = 0;
      virtual bool      __fastcall  ReiniciaLisEqptoRegiao(VTRede *rede) = 0;
      virtual void      __fastcall  RetiraRegiao(VTRegiao *regiao) = 0;

   protected:  //dados acessados via property
      struct   {
               int         id;
               AnsiString  codigo;
               AnsiString  descricao;
               }PD;
   };

//---------------------------------------------------------------------------
VTMapa* __fastcall NewObjMapa(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof
