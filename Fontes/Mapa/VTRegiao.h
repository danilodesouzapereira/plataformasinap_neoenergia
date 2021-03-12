//---------------------------------------------------------------------------
#ifndef VTRegiaoH
#define VTRegiaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\Estrutura.h"

//-----------------------------------------------------------------------------
class VTApl;
class VTEqpto;
class VTPoligono;
class VTRede;
class VTRedes;

//-----------------------------------------------------------------------------
class VTRegiao : public TObject
   {
   public:  //property
      __property int         Id              = {read=PD.id,       write=PD.id};
      __property AnsiString  Codigo          = {read=PD.codigo,   write=PD.codigo};
      __property TColor      Color           = {read=PD.color,    write=PD.color};
      __property bool        Visible         = {read=PD.visible,  write=PD.visible};
      __property VTPoligono* Poligono        = {read=PD.poligono, write=PD.poligono};
      __property VTRegiao*   RegiaoPai       = {read=PD.regiaoPai, write=PD.regiaoPai};
      __property TList*      LisRegiaoFilhas = {read=PD.lisFilhas, write=PD.lisFilhas};
      __property int         Level           = {read=PD.level, write=PD.level};
      __property strUTM      Centrocarga_cm  = {read=PD.centro_carga};
      __property TObject*    Obj             = {read=PD.obj,      write=PD.obj};
      __property unsigned    Tag             = {read=PD.tag,      write=PD.tag};

   public:
                       __fastcall  VTRegiao(void) {};
      virtual          __fastcall ~VTRegiao(void) {};
      virtual bool     __fastcall  CalculaCentroCarga(int ind_pat) = 0;
      virtual VTEqpto* __fastcall  ExisteEqpto(VTEqpto *eqpto) = 0;
      virtual int      __fastcall  LisEqpto(TList *lisEXT, int tipo_eqpto=-1) = 0;
      virtual int      __fastcall  LisEqpto(TList *lisEXT, VTRede *rede, int tipo_eqpto=-1) = 0;
      virtual bool     __fastcall  ReiniciaLisEqpto(VTRede *rede) = 0;
      virtual bool     __fastcall  ReiniciaLisEqpto(VTRedes *redes) = 0;

   protected:  //dados acessados via property
      struct   {
               int         id;
               AnsiString  codigo;
               TColor      color;
               bool        visible;
               int         level;
               VTRegiao   *regiaoPai;
               TList      *lisFilhas;
               strUTM      centro_carga;
               VTPoligono *poligono;
               TObject    *obj;
               unsigned   tag;
               }PD;
   };

//---------------------------------------------------------------------------
VTRegiao* __fastcall NewObjRegiao(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof
