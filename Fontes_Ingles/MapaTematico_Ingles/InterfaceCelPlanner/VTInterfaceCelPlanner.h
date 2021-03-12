//---------------------------------------------------------------------------
#ifndef VTInterfaceCelPlannerH
#define VTInterfaceCelPlannerH

//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
struct strCFGICelPlanner{
       bool exporta_cargas;
       bool exporta_capacitores;
       bool exporta_chaves;
       bool exporta_religadores;
       bool exporta_seccionadoras;
       bool exporta_geradores;
       bool exporta_reguladores;
       bool exporta_subestacao;
       bool exporta_trafos;
       bool utm;
       };
//---------------------------------------------------------------------------
struct strCPAntena{
       int        ota;
       int        tal;
       AnsiString typ;
       int        aht;
       int        pwr;
       int        txl;
       int        rxl;
       int        rxs;
       double     rnf;
       double     trd;
       double     tru;
       double     drd;
       double     dru;
       int        prb;
       AnsiString ant;
       AnsiString celld;
       AnsiString cellu;
       double     dst;
       double     azt;
       double     azm;
       double     ind;
       double     inu;
       };
//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTInterfaceCelPlanner : public TObject
   {
   public:
                           __fastcall  VTInterfaceCelPlanner(void) {};
                           __fastcall ~VTInterfaceCelPlanner(void) {};
      virtual strCPAntena  __fastcall  AntenaDefault(void) = 0;
      virtual AnsiString   __fastcall  AntenaDefaultAsString(void) = 0;
      virtual bool         __fastcall  ExportaArqCelPlanner(TStrings *lines) = 0;

//      virtual bool __fastcall  ExportaArqNix(TStrings *lines,
//                                                bool pr_com_corrente=false,
//                                                bool al_com_corrente=false,
//                                                int  patamar = -1) = 0;
   public: //property
      __property bool   exp_Capacitores= {read  = cfg.exporta_capacitores,
                                             write = cfg.exporta_capacitores};
      __property bool   exp_Cargas     = {read  = cfg.exporta_cargas,
                                             write = cfg.exporta_cargas};
      __property bool   exp_Chaves     = {read  = cfg.exporta_chaves,
                                             write = cfg.exporta_chaves};
      __property bool   exp_Geradores  = {read  = cfg.exporta_geradores,
                                             write = cfg.exporta_geradores};
      __property bool   exp_Reguladores  = {read  = cfg.exporta_reguladores,
                                             write = cfg.exporta_reguladores};
      __property bool   exp_Seccionadoras  = {read  = cfg.exporta_seccionadoras,
                                             write = cfg.exporta_seccionadoras};
      __property bool   exp_Religadores  = {read  = cfg.exporta_religadores,
                                             write = cfg.exporta_religadores};
      __property bool   exp_Subestacao  = {read  = cfg.exporta_subestacao,
                                             write = cfg.exporta_subestacao};
      __property bool   exp_Trafos  = {read  = cfg.exporta_trafos,
                                             write = cfg.exporta_trafos};
      __property bool   is_UTM  = {read  = cfg.utm,
                                             write = cfg.utm};
   protected: //dados acessados por property
   strCFGICelPlanner   cfg;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTInterfaceCelPlanner
//---------------------------------------------------------------------------
VTInterfaceCelPlanner* __fastcall NewObjInterfaceCelPlanner(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 