#ifndef VTPadraoRede_H
#define VTPadraoRede_H

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTPadraoRede : public TObject
   {
   public:  //property
      __property int        PadraoRedeId  = {read=PD.intPadraoRedeId, write=PD.intPadraoRedeId};
      __property AnsiString Descricao     = {read=PD.strDescricao   , write=PD.strDescricao   };
      __property int        NumFases      = {read=PD.intNumFases    , write=PD.intNumFases    };
      __property double     VFF           = {read=PD.dblVFF         , write=PD.dblVFF         };
      __property double     VFN           = {read=PD.dblVFN         , write=PD.dblVFN         };
      __property double     V1            = {read=PD.dblV1          , write=PD.dblV1          };
      __property double     V2            = {read=PD.dblV2          , write=PD.dblV2          };
      __property double     V3            = {read=PD.dblV3          , write=PD.dblV3          };
      __property double     V4            = {read=PD.dblV4          , write=PD.dblV4          };
      __property double     FPI1          = {read=PD.dblFPI1        , write=PD.dblFPI1        };
      __property double     FPI2          = {read=PD.dblFPI2        , write=PD.dblFPI2        };
      __property double     FPS1          = {read=PD.dblFPS1        , write=PD.dblFPS1        };
      __property double     FPS2          = {read=PD.dblFPS2        , write=PD.dblFPS2        };

   public:
      virtual               __fastcall  VTPadraoRede(void) {};
      virtual               __fastcall ~VTPadraoRede(void) {};
      virtual VTPadraoRede* __fastcall  Clone(void) = 0;

   protected: //dados
      //dados acessados via property
      struct   {
               int        intPadraoRedeId;
               AnsiString strDescricao;
               int        intNumFases;
               double     dblVFF;
               double     dblVFN;
               double     dblV1;
               double     dblV2;
               double     dblV3;
               double     dblV4;
               double     dblFPI1;
               double     dblFPI2;
               double     dblFPS1;
               double     dblFPS2;
               } PD;
      
   };

//---------------------------------------------------------------------------
//protótipo de função para criar objetos VTPadraoRede
//---------------------------------------------------------------------------
VTPadraoRede* __fastcall NewObjPadraoRede(void);

#endif
//-----------------------------------------------------------------------------
// eof